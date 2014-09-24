// HFServer.cpp: implementation of the CHFServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "HFServer.h"
#include "Price.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CHFServer::CHFServer() : 
	m_nStatus(DBA_ERR_NO_ATTACH),
	m_bTerminate(true),
	m_hThread(0),
	m_nCount(0),
	m_etiPtr(0)
{
	// EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer Object Constructor") );
}

CHFServer::~CHFServer()
{
	// EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer Object Destructor") );
	if (m_hThread != 0)
		Terminate();
	_ASSERTE(m_setSubs.size() == 0);
}

//////////////////////////////////////////////////////////////////////
// CHFServer member functions

long CHFServer::Initialize()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer Initialize Enter") );
	if (m_hThread != 0)
	{
		if (!m_bTerminate)
		{
			::InterlockedIncrement(&m_nCount);
			return m_nStatus = DBA_ERR_NO_ERROR;
		}
		else
		{
			::WaitForSingleObject(m_hThread, INFINITE);
		}
	}

	m_hThread = ::CreateEvent(0, FALSE, FALSE, 0);
	m_bTerminate = false;
        
	EgLib::CThread::Start();
	HANDLE hThread = EgLib::CThread::GetThreadHandle();

	if (hThread != 0)
	{
		::WaitForSingleObject(m_hThread, INFINITE);
		::CloseHandle(m_hThread);
		m_hThread = hThread;
		if (m_nStatus == DBA_ERR_NO_ERROR)
		{
			::InterlockedIncrement(&m_nCount);
			return DBA_ERR_NO_ERROR;
		}
	}

	return -1;
}

long CHFServer::Uninitialize()
{
	if (::InterlockedDecrement(&m_nCount) == 0 && m_hThread)
	{
		Terminate();
		return m_nStatus;
	}
	return DBA_ERR_NO_ERROR;
}

void CHFServer::Terminate()
{
	EgLib::CThread::Stop(true, HFC_TIMEOUT);
	m_hThread = 0;
}

UINT CHFServer::Run()
{
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer Run Enter") );

	m_nStatus = eti(m_etiPblk, ETI_ATTACH, &m_etiPtr);
	::SetEvent(m_hThread);

	if (m_nStatus != DBA_ERR_NO_ERROR)
		return 0;
	
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer ETI_ATTACH Succeed") );

	while (!m_bTerminate)
	{
		const short status = eti(m_etiPblk, ETI_READ, &m_etiPtr);
		if(EgLib::CThread::IsStoping() || !EgLib::CThread::IsRunning())
			m_bTerminate = true;

		if(m_bTerminate)
			break;
		else if(status == RC_ETI_PERIODIC)
		{
			continue;
		}
		else if (status != RC_ETI_NOERROR)
		{
			m_nStatus = status;
			EgLib::CAutoLock lock(m_csQueue);
			std::for_each(m_setSubs.begin(), 
				          m_setSubs.end(), 
						   boost::bind(&CPrice::RealtimeMessage, _1, (ETI_TABLE*)NULL));

			if (status == RC_ETI_UNLOAD)
				break;
		}
		else
		{
			switch(m_etiPtr->packetPtr->undefined.header.type)
			{
			case MT_FILE_TRANSFER:
				if(FILE_CONTROL == m_etiPtr->packetPtr->fileTransfer.transferMask[0])
				{
					switch(m_etiPtr->packetPtr->fileTransfer.data.control.command)
					{
					case FT_CTRL_HEART_BEAT:		// 32 - heart beat
						{

						}break;
					case FT_CTRL_NONE:				// 0 - no state implied (Sharky: simple message while reconnecting)
						{
							std::string strText;
							strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
							strText.insert(strText.end(),(CHAR)0);

							EgLib::CAutoLock lock(m_csQueue);
							std::for_each(	m_setSubs.begin(), 
								m_setSubs.end(), 
								boost::bind(&CPrice::NotificationMessage, _1, enProviderConnecting, strText));


						}break;
					case FT_CTRL_ETI_SESS_RESET:	// 8 - Eti Session reset or restored
						{
							std::string strText;
							strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
							strText.insert(strText.end(),(CHAR)0);

							EgLib::CAutoLock lock(m_csQueue);
							std::for_each(	m_setSubs.begin(), 
								m_setSubs.end(), 
								boost::bind(&CPrice::NotificationMessage, _1, enProviderConnected, strText));

						}break;
					case FT_CTRL_ETI_SESS_DOWN:		// 4 - Eti Session down
						{
							std::string strText;
							strText.insert(strText.end(), m_etiPtr->packetPtr->fileTransfer.data.control.text, m_etiPtr->packetPtr->fileTransfer.data.control.text+sizeof(m_etiPtr->packetPtr->fileTransfer.data.control.text));
							strText.insert(strText.end(),(CHAR)0);
							EgLib::CAutoLock lock(m_csQueue);
							std::for_each(	m_setSubs.begin(), 
								m_setSubs.end(), 
								boost::bind(&CPrice::NotificationMessage, _1, enConnectionWasDisconnected, strText));

							int i = 0;
						}break;
					}
				}break;
			case MT_TRADE:
			case MT_QUOTE:
				{
					EgLib::CAutoLock lock(m_csQueue);
					std::for_each(	m_setSubs.begin(), 
									m_setSubs.end(), 
										boost::bind(&CPrice::RealtimeMessage, _1, m_etiPtr));
				}break;
			}
		}
	}

	_ASSERTE(m_setSubs.size() == 0);

	m_nStatus = eti(m_etiPblk, ETI_DETACH, &m_etiPtr);
	
	EgStd::CEgTracingClass::TraceStatic(enlogDebug, __FUNCTION__, _T("HFServer Run Exit") );
	return 0;
}

void CHFServer::Subscribe(CPrice* pSub)
{
	EgLib::CAutoLock lock(m_csQueue);
	m_setSubs.insert(pSub);
}

void CHFServer::Unsubscribe(CPrice* pSub)
{
	EgLib::CAutoLock lock(m_csQueue);
	subset::const_iterator it = m_setSubs.find(pSub);
	_ASSERTE(it != m_setSubs.end());
	if (it != m_setSubs.end())
		m_setSubs.erase(pSub);
}


/*
LONG CALLBACK EtiHandler(LONG nStatus, ETI_TABLE* pTable, PVOID pUserData)
{
	if(MT_FILE_TRANSFER == pTable->packetPtr->info.header.type
		&& FILE_CONTROL == pTable->packetPtr->fileTransfer.transferMask[0])

	{

		//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_HEART_BEAT) // 32 - heart beat

		//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_NONE) // 0 - no state implied (Sharky: simple message while reconnecting)

		//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_ETI_SESS_RESET) // 8 - Eti Session reset or restored

		//if(pTable->packetPtr->fileTransfer.function == FT_CTRL_ETI_SESS_DOWN) // 4 - Eti Session down

		/*

		Sequence:

		FT_CTRL_ETI_SESS_RESET - may not raised if connection already was established

		...

		FT_CTRL_HEART_BEAT - connected

		...

		FT_CTRL_ETI_SESS_DOWN - when disconnected

		...

		FT_CTRL_NONE - while disconnected

		...

		FT_CTRL_ETI_SESS_RESET - when reconnected

		...

		FT_CTRL_HEART_BEAT - connected

		...

		

		printf("MT_FILE_TRANSFER: %d\t%d\t%02d%02d\t%s\n",

			pTable->packetPtr->fileTransfer.sequenceNumber,

			pTable->packetPtr->fileTransfer.function,

			pTable->packetPtr->fileTransfer.data.control.source,

			pTable->packetPtr->fileTransfer.data.control.command,

			(FT_CTRL_HEART_BEAT == pTable->packetPtr->fileTransfer.data.control.command ? ""

			: (LPSTR)pTable->packetPtr->fileTransfer.data.control.text));

	}

	return 0L;

}
*/