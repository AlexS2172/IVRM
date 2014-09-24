#ifndef _FIXMESSAGE_H
#define _FIXMESSAGE_H

#include "FixTag.h"
#include "Trace.h"

#define FIX_BEGIN_STRING    "FIX.4.1"

/*-------------------------------------------------------------------------------
	CFixMessage class
--------------------------------------------------------------------------------*/
typedef map<FixTagEnum, CFixTagValue>	TagsMap;

class CFixMessage : public CFixBuffer
{
protected:

	TagsMap				m_Tags;
	FixMsgTypeEnum		m_Type;
	int					m_nSeqNum;
	BOOL				m_bPossDupFlag;
	BOOL				m_bPossResend;

public:
	
	CFixMessage():m_Type(enFmtUnknown), m_nSeqNum(-1), m_bPossDupFlag(FALSE), m_bPossResend(FALSE)
	{
	}

	virtual ~CFixMessage()
	{
	}

public:

	DWORD Parse(CFixBuffer& Buffer);

	inline const FixMsgTypeEnum GetType() const {return m_Type;}
	inline const TagsMap& GetTagsMap() const {return m_Tags;}
	inline const int GetSeqNum() const {return m_nSeqNum;}

	BOOL	GetTagValue(const FixTagEnum Key, CFixTagValue& Value) const
	{
		const TagsMap::const_iterator it = m_Tags.find(Key);
		if (it == m_Tags.end())
			return FALSE;

		Value = it->second;

		return TRUE;
	}

	BOOL	DeleteTag(const FixTagEnum Key) 
	{
		const TagsMap::iterator it = m_Tags.find(Key);
		if (it == m_Tags.end())
			return FALSE;

		m_Tags.erase(it);

		return TRUE;
	}

	void	DumpTags()
	{
		for(TagsMap::iterator it = m_Tags.begin(); it != m_Tags.end(); it++)
		{
			FixTagEnum	Key;
			string		sValue;

			Key = it->first;
			CFixTagValue& Value = it->second;
			sValue.assign((char*)Value.GetData(), Value.GetLength());

			CTracer::Trace(_T("%d=%s"), Key, sValue.c_str());
		}
	}

// overrides
public:

	void Clear()
	{
		CFixBuffer::Clear();
		m_Tags.clear();
	}

// duplicate flags
public:

	BOOL IsPossDup() const {return m_bPossDupFlag;}
	
	BOOL IsPossResend() const {return m_bPossResend;}

// MakeXXX methods
public:

	// The logon message authenticates a user establishing a connection to a remote 
	// system.  The logon message must be the first message sent by the application 
	// requesting to initiate a FIX session.
	void	MakeLogon(const int nSeqNo, const bool bResetSeqNo);

	// The test request message forces a heartbeat from the opposing application
	void	MakeTestRequest(const int nSeqNo, const char * const szTestReqID = "Egar Test");

	// The Heartbeat monitors the status of the communication link and identifies 
	// when the last of a string of messages was not received
	void	MakeHeartBeat(const int nSeqNo, const char * const szTestReqID = NULL);

	// The resend request is sent by the receiving application to initiate 
	// the retransmission of messages. 
	void	MakeResendRequest(const int nSeqNo, const int nBeginSeqNo, const int nEndSeqNo);

	// The reject message should be issued when a message is received but cannot 
	// be passed through to the application level.
	void	MakeReject(const int nSeqNo, const int nRefSeqNum, const char * const szText = NULL);

	// The sequence reset message is used by the sending application to reset the 
	// incoming sequence number on the opposing side
	void	MakeSequenceReset(const int nSeqNo, const int nNewSeqNo, const char cGapFillFlag = 'N');

	// The logout message initiates or confirms the termination of a FIX session.
	void	MakeLogout(const int nSeqNo, const char * const szText = NULL);

protected:

	DWORD ParseTextTag( const char* const pMsgPtr, 
						const DWORD dwMsgLen,
						FixTagEnum& Key,
						char*& pValue,
						DWORD& dwTagValueLen,
						DWORD& dwLen);

	void  TraceDumpTag(	const TCHAR* const szText, 
						const char* const pTagPtr, 
						const DWORD dwTagLen);

	void  BeginConstructMessage(const FixMsgTypeEnum MsgType,
								const int nSeqNo, CFixMessage & Body);

	void  EndConstructMessage(CFixMessage & Body);

	char* GenerateCheckSum( const char *pBuf, const long lLen );


	CFixMessage& operator << (const CFixTag& Tag)
	{
		m_Tags[Tag.GetKey()] = Tag.GetValue();
		char cChar;

		switch(Tag.GetKey())
		{
			case enFtMsgType:
				if (Tag.GetValue().GetChar(cChar))
					m_Type = (FixMsgTypeEnum)cChar;
				break;

			case enFtMsgSeqNum:
				Tag.GetValue().GetInt(m_nSeqNum);
				break;

			case enFtPossDupFlag:
				if (Tag.GetValue().GetChar(cChar))
					m_bPossDupFlag = (cChar == 'Y');
				break;

			case enFtPossResend:
				if (Tag.GetValue().GetChar(cChar))
					m_bPossResend = (cChar == 'Y');
				break;
		}

		Put(Tag.GetData(), Tag.GetLength());
		return *this;
	}

	CFixMessage& operator << (const CFixMessage& Message)
	{
		Put(Message.GetData(), Message.GetLength());
		return *this;
	}
};

#endif