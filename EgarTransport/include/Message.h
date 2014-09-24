// Message.h : Declaration of the CMessage

#ifndef __MESSAGE_H_
#define __MESSAGE_H_

#include "MessageParser.h"
//#include "Transport.h"
#include "CRtClasses.h"

using namespace __dev__;

namespace EgTpMessage
{
	enum MsgTypeEnum
	{
		enUnknown = -1,
		enLogon = 0,
		enReLogon,
		enLogonConfirm,
		enSetSubject,
		enRemoveSubject,
		enLogout,
		enData,
		enTestRequest,
		enHeartBeat,
		enReject, // bad message received
		enResend, // GAPS detected
		enCheckSubject,
	};


	class CEgTpBaseMessage : public CLock
	{
	public:
		CEgTpBaseMessage(MsgTypeEnum Type) : m_enType(Type), m_ulSize(0),m_enMsgProp(enMpUnspecified) {};

		virtual ~CEgTpBaseMessage(){};

		bool Pack(CEgPacket& Packet)
		{
			if (!this->Packing(Packet))
				return false;

			return true;		
		};

		bool Unpack(CEgPacket& Packet)
		{
			if (!this->Packing(Packet, true))
				return false;

			return true;
		};

	protected:

		virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;
			m_ulSize.GetData(tlng);
			lng += tlng;
			m_enType.GetData(tlng);
			lng += tlng;
			m_enMsgProp.GetData(tlng);
			lng += tlng;
			return lng;
		};

		BEGIN_PACKING					
			FIELD_ENTRY(m_ulSize)			
			FIELD_ENTRY(m_enType)			
			FIELD_ENTRY(m_enMsgProp)		
			END_PACKING

	public:
		// !!!URGENT!!! CAN consist not only Message size 
		// BUT also and size of 2 ULong :
		// Message send and Message Received (for example Data message)
		ULongF								m_ulSize;
		/////////////////////////////////////////////
		TStaticField<MsgTypeEnum>			m_enType;
		TStaticField<MsgPropertiesEnum>		m_enMsgProp;
	};
	typedef boost::shared_ptr<CEgTpBaseMessage> CEgTpBaseMessagePtr;


#define FIELD_HEADER_ENTRIES			\
	FIELD_ENTRY(m_ulSize)			\
	FIELD_ENTRY(m_enType)			\
	FIELD_ENTRY(m_enMsgProp)		\


	class CEgTpLogonMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpLogonMsg() : CEgTpBaseMessage(enLogon),m_ulMessageReceived(0) {};

	protected:

		BEGIN_PACKING		
			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageReceived)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageReceived.GetData(tlng);
			lng += tlng;
			return lng;
		}

	public:
		ULongF		m_ulMessageReceived;			
	};

	class CEgTpReLogonMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpReLogonMsg() : CEgTpBaseMessage(enReLogon), m_ulMessageReceived(0){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageReceived)			
			FIELD_ENTRY(m_HostID)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageReceived.GetData(tlng);
			lng += tlng;
			m_HostID.GetData(tlng);
			lng += tlng;

			return lng;
		}

	public:
		ULongF		m_ulMessageReceived;			
		GuidF		m_HostID;

	};

	class CEgTpReSendMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpReSendMsg() : CEgTpBaseMessage(enResend), m_ulMessageReceived(0){};

	protected:
		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageReceived)			
			FIELD_ENTRY(m_HostID)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageReceived.GetData(tlng);
			lng += tlng;
			m_HostID.GetData(tlng);
			lng += tlng;

			return lng;
		}
	public:

		ULongF		m_ulMessageReceived;			
		GuidF		m_HostID;

	};


	class CEgTpLogoutMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpLogoutMsg() : CEgTpBaseMessage(enLogout), m_ulMessageReceived(0){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageReceived)			
			FIELD_ENTRY(m_HostID)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageReceived.GetData(tlng);
			lng += tlng;
			m_HostID.GetData(tlng);
			lng += tlng;

			return lng;
		}
	public:

		ULongF		m_ulMessageReceived;			
		GuidF		m_HostID;

	};

	class CEgTpLogonConfirmMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpLogonConfirmMsg() : CEgTpBaseMessage(enLogonConfirm), m_ulMessageReceived(0){};

	protected:
		virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageReceived.GetData(tlng);
			lng += tlng;
			m_HostID.GetData(tlng);
			lng += tlng;

			return lng;
		}

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageReceived)			
			FIELD_ENTRY(m_HostID)			
			END_PACKING

	public:

		ULongF		m_ulMessageReceived;			
		GuidF		m_HostID;

	};


	class CEgTpTestRequestMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpTestRequestMsg() : CEgTpBaseMessage(enTestRequest) {};
	protected:
		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageSend)			
			FIELD_ENTRY(m_ulMessageReceived)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageSend.GetData(tlng);
			lng += tlng;
			m_ulMessageReceived.GetData(tlng);
			lng += tlng;

			return lng;
		}

	public:

		ULongF		m_ulMessageSend;
		ULongF		m_ulMessageReceived;

	};


	class CEgTpHeartBeatMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpHeartBeatMsg() : CEgTpBaseMessage(enHeartBeat) {};
	protected:
		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_ulMessageSend)			
			FIELD_ENTRY(m_ulMessageReceived)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_ulMessageSend.GetData(tlng);
			lng += tlng;
			m_ulMessageReceived.GetData(tlng);
			lng += tlng;

			return lng;
		}

	public:

		ULongF		m_ulMessageSend;
		ULongF		m_ulMessageReceived;

	};


	class CEgTpUpdateSubjectMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpUpdateSubjectMsg() : CEgTpBaseMessage(enSetSubject), m_strSubject(""){};

	protected:
		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)			
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_strSubject.GetData(tlng);
			lng += tlng;

			lng += 2 * sizeof(u_long);

			return lng;
		}

	public:

		StringF			m_strSubject;
	};

	class CEgTpUpdateSubjectWithNumMsg : public CEgTpUpdateSubjectMsg
	{
	public:
		CEgTpUpdateSubjectWithNumMsg() : CEgTpUpdateSubjectMsg(){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)
			FIELD_ENTRY(m_ulMessageSend)
			FIELD_ENTRY(m_ulMessageReceived)
			END_PACKING

	public:
		ULongF		m_ulMessageSend;
		ULongF		m_ulMessageReceived;
	};

	class CEgTpDataMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpDataMsg() : CEgTpBaseMessage(enData), m_strSubject(""){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)
			FIELD_ENTRY(m_blobData)
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_strSubject.GetData(tlng);
			lng += tlng;
			m_blobData.GetData(tlng);
			lng += tlng;

			lng += 2 * sizeof(u_long);

			return lng;
		}

	public:
		StringF			m_strSubject;
		BlobF			m_blobData;
	};


	class CEgTpDataWithNumMsg : public CEgTpDataMsg
	{
	public:
		CEgTpDataWithNumMsg(){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)
			FIELD_ENTRY(m_blobData)
			FIELD_ENTRY(m_ulMessageSend)
			FIELD_ENTRY(m_ulMessageReceived)
			END_PACKING

	public:
		ULongF		m_ulMessageSend;
		ULongF		m_ulMessageReceived;
	};


	class CEgTpCheckSubjectMsg : public CEgTpBaseMessage
	{
	public:
		CEgTpCheckSubjectMsg() : CEgTpBaseMessage(enCheckSubject), m_strSubject(""){};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)
			FIELD_ENTRY(m_guidSession)
			FIELD_ENTRY(m_ulNumSubscribers)
			END_PACKING

			virtual unsigned long GetMsgSize()
		{					
			unsigned long lng = 0, tlng = 0;

			lng = CEgTpBaseMessage::GetMsgSize();

			m_strSubject.GetData(tlng);
			lng += tlng;
			m_guidSession.GetData(tlng);
			lng += tlng;
			m_ulNumSubscribers.GetData(tlng);
			lng += tlng;

			lng += 2 * sizeof(u_long);

			return lng;
		}

	public:
		StringF			m_strSubject;
		GuidF			m_guidSession;
		ULongF			m_ulNumSubscribers;
	};


	class CEgTpCheckSubjectWithNumMsg : public CEgTpCheckSubjectMsg
	{
	public:
		CEgTpCheckSubjectWithNumMsg() {};

	protected:

		BEGIN_PACKING

			FIELD_HEADER_ENTRIES

			FIELD_ENTRY(m_strSubject)
			FIELD_ENTRY(m_guidSession)
			FIELD_ENTRY(m_ulNumSubscribers)
			FIELD_ENTRY(m_ulMessageSend)
			FIELD_ENTRY(m_ulMessageReceived)
			END_PACKING
	public:
		ULongF		m_ulMessageSend;
		ULongF		m_ulMessageReceived;
	};

	__inline MsgTypeEnum GetMessageType(const void* pBuffer)
	{
		MsgTypeEnum	tpEnum;

		::CopyMemory(&tpEnum,(char*)pBuffer + sizeof(u_long), sizeof(MsgTypeEnum));

		if(tpEnum > enCheckSubject || tpEnum < enLogon)
			tpEnum = enUnknown;

		return tpEnum;
	}

	typedef boost::shared_ptr<CEgTpCheckSubjectWithNumMsg>	CEgTpCheckSubjectWithNumMsgPtr;
	typedef boost::shared_ptr<CEgTpCheckSubjectMsg>			CEgTpCheckSubjectMsgPtr;
	typedef boost::shared_ptr<CEgTpDataWithNumMsg>			CEgTpDataWithNumMsgPtr;
	typedef boost::shared_ptr<CEgTpDataMsg>					CEgTpDataMsgPtr;
	typedef boost::shared_ptr<CEgTpLogonMsg>				CEgTpLogonMsgPtr;
	typedef boost::shared_ptr<CEgTpReLogonMsg>				CEgTpReLogonMsgPtr;
	typedef boost::shared_ptr<CEgTpReSendMsg>				CEgTpReSendMsgPtr;
	typedef boost::shared_ptr<CEgTpLogoutMsg>				CEgTpLogoutMsgPtr;
	typedef boost::shared_ptr<CEgTpLogonConfirmMsg>			CEgTpLogonConfirmMsgPtr;
	typedef boost::shared_ptr<CEgTpTestRequestMsg>			CEgTpTestRequestMsgPtr;
	typedef boost::shared_ptr<CEgTpHeartBeatMsg>			CEgTpHeartBeatMsgPtr;
	typedef boost::shared_ptr<CEgTpUpdateSubjectMsg>		CEgTpUpdateSubjectMsgPtr;
	typedef boost::shared_ptr<CEgTpUpdateSubjectWithNumMsg> CEgTpUpdateSubjectWithNumMsgPtr;

}



#endif //__MESSAGE_H_
