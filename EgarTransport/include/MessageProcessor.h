#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <EgLib\EgLibThread.h>
#include "Message.h"

using namespace EgTpMessage;

enum MsgProcessorTypeEnum
{
	enMPTNotBuffered,	// Only last message stored
	enMPTBuffered,		// Amessages stored
	enMPTGuarantee,		// All messages stored and guaranteed
	enMPTIncoming,
	enMPTLocal,
};


template<class _SenderCxt, class _ReceiverKey, class _ReceiverCxt,MsgProcessorTypeEnum Type>
class CTEgTpBaseMessageProcessor : public CThread
{
protected:

	typedef set<_ReceiverKey>					TReceiversSet;
	typedef typename TReceiversSet::iterator	TReceiversIter;

	CHandle								m_hNotification;
	_SenderCxt*							m_pcxtSender;
	TReceiversSet						m_setActiveReceivers;

public:

	CTEgTpBaseMessageProcessor(_SenderCxt* pcxtSender): 
	  m_pcxtSender(pcxtSender)
	{
	}
	
	virtual ~CTEgTpBaseMessageProcessor(){};


	DWORD Start()
	{
		EgLib::CAutoLock	Lock(m_Lock);

		if (m_hNotification == NULL)
			m_hNotification.Attach(::CreateEvent(NULL, FALSE, FALSE, NULL));

		if (m_hNotification == NULL)
			return GetLastError();
		
		m_setActiveReceivers.clear();

		return CThread::Start();
	};

	DWORD Stop()
	{
		EgLib::CAutoLock	Lock(m_Lock);

		DWORD dwErr = CThread::Stop(true, 1000L);

		m_hNotification.Close();
		m_setActiveReceivers.clear();

		return dwErr;
	}

	void SetReceiverActive(const _ReceiverKey& keyReceiver)
	{
		EgLib::CAutoLock	Lock(m_Lock);
		
		m_setActiveReceivers.insert(keyReceiver);
		SetEvent(m_hNotification);
	}

	void SetReceiverInactive(const _ReceiverKey& keyReceiver)
	{
		EgLib::CAutoLock	Lock(m_Lock);		
		m_setActiveReceivers.erase(keyReceiver);
	}

private:
	bool GetNextActive(_ReceiverKey& keyReceiver)
	{
		EgLib::CAutoLock	Lock(m_Lock);		

		if(m_setActiveReceivers.empty())
			return false;

		TReceiversIter iter = m_setActiveReceivers.upper_bound(keyReceiver);
		if(iter == m_setActiveReceivers.end())
			iter = m_setActiveReceivers.begin();

		if(iter == m_setActiveReceivers.end())
			return false;
		else
		{
			keyReceiver = *iter;
			return true;
		}
	}

/////////////////////////////////////////////////////////////////////////
// Overridables

	virtual bool OnMessage(	/*in*/ _ReceiverCxt* cxtReceiver,
							/*in*/ CEgTpBaseMessagePtr pMessage,
							/*in*/ u_long keyMessage) = 0;

/////////////////////////////////////////////////////////////////////////	
	UINT Run()
	{
		CoInitializeEx(NULL, COINIT_MULTITHREADED);
		
		HANDLE hStopEvent = GetStopEventHandle();

		const HANDLE hEvents[] = {m_hNotification, hStopEvent};

		_ReceiverKey keyReceiver = _ReceiverKey();

		while(true)
		{
			DWORD dwRes = WaitForMultipleObjects(sizeof(hEvents) / sizeof(HANDLE), 
				hEvents, FALSE, INFINITE);

			if (dwRes == WAIT_OBJECT_0 + 1) /* Stop event */
			{
				CoUninitialize();
				return 0;
			}

			while(true)
			{
				if(IsStopping())
					break;

				CEgTpBaseMessagePtr	pData ;

				_ReceiverCxt* pReceiver = NULL;
					
				if(!GetNextActive(keyReceiver))
					break;

				if(!m_pcxtSender->GetReceiverCxt(keyReceiver,&pReceiver) || !pReceiver)
				{
					SetReceiverInactive(keyReceiver);
					continue;
				}

				u_long keyMessage = 0;

				if(!pReceiver->GetNextMessage(Type, &pData, keyMessage))
				{
					SetReceiverInactive(keyReceiver);
					pReceiver->Release();
					continue;
				}
				if(!OnMessage(pReceiver, pData, keyMessage))
					SetReceiverInactive(keyReceiver);
				else pReceiver->SetMessageSend(Type, keyMessage);

				pReceiver->Release();
			}
		}
	}

private:
	EgLib::CLock m_Lock;
};

class CEgTpGuaranteeMessageStorage : 
	protected EgLib::CLock,
	protected deque<CEgTpBaseMessagePtr>
{
public:

	CEgTpGuaranteeMessageStorage() {};

	virtual ~CEgTpGuaranteeMessageStorage()
	{
		Clear();
	}

	void Push( CEgTpBaseMessagePtr pValue )
	{
		CAutoLock  Lock(*this);
		
		if(pValue==NULL)
			return;

		push_back(pValue);
	}
	bool GetMessage( u_long keyMess,
					 CEgTpBaseMessagePtr* ppValue)
	{
		CAutoLock  Lock(*this);

		if(!ppValue)
			return false;

		if(keyMess >= size())
			return false;

		*ppValue = at(keyMess);
		return true;
	}
	void RemoveBelow(u_long keyMess)
	{
		CAutoLock  Lock(*this);

		u_long ulSize = size();
		
		for(u_long count = 0; count < keyMess && count < ulSize; count++)
			pop_front();
	}

	u_long	GetSize() const
	{
		return size();
	}
	void Clear()
	{
		CAutoLock  Lock(*this);
		clear();
	}
};

class CEgTpBufferedMessageStorage : 
	protected EgLib::CLock,
	protected queue<CEgTpBaseMessagePtr>
{
public:

	CEgTpBufferedMessageStorage() {};

	virtual ~CEgTpBufferedMessageStorage()
	{
		Clear();
	}

	void Push( CEgTpBaseMessagePtr pValue )
	{
		if(!pValue)	return;

		CAutoLock  Lock(*this);
		push(pValue);
	}
	bool GetMessage( CEgTpBaseMessagePtr* ppValue )
	{
		if(!ppValue)
			return false;

		CAutoLock  Lock(*this);

		if(empty())
			return false;

		*ppValue = front();
		pop();

		return true;
	}

	u_long	GetSize() const
	{
		return size();
	}
	void Clear()
	{
		CAutoLock  Lock(*this);
		while(!empty())
			pop();
	}
};
struct SEgTpNotBufferedMessageKey
{
public:
	SEgTpNotBufferedMessageKey(tstring key) : Subject(key){};

	tstring	Subject;

	bool operator < (const SEgTpNotBufferedMessageKey& Origin) const
	{					
		return Subject.compare(Origin.Subject) < 0;
	}
};


class CEgTpNotBufferedMessageStorage : 
	protected EgLib::CLock,
	protected map<SEgTpNotBufferedMessageKey, CEgTpBaseMessagePtr>
{
public:

	CEgTpNotBufferedMessageStorage() : m_strLastSubject(_T("")) {};

	virtual ~CEgTpNotBufferedMessageStorage()
	{
		Clear();
	}

	void Push(	const tstring& strSubject, 
				CEgTpBaseMessagePtr pValue )
	{
		
		if(!pValue) return;

		CAutoLock  Lock(*this);
	
		iterator iter = find(strSubject);
		if(iter != end())
			iter->second = pValue;
		else
			insert(std::make_pair(strSubject, pValue));
	}
	bool GetMessage( CEgTpBaseMessagePtr* ppValue )
	{

		if(!ppValue) return false;

		CAutoLock  Lock(*this);
		if(empty())
			return false;

		iterator iter = upper_bound(m_strLastSubject);
		if(iter == end())
			iter = begin();
		
		m_strLastSubject = iter->first.Subject;

		*ppValue = iter->second;
		erase(iter);

		return true;
	}

	u_long	GetSize() const
	{
		return size();
	}
	void Clear()
	{
		CAutoLock  Lock(*this);
		map<SEgTpNotBufferedMessageKey, CEgTpBaseMessagePtr>::clear();
		m_strLastSubject = _T("");
	}
private:
	tstring m_strLastSubject;
};

#endif //MESSAGEPROCESSOR_H