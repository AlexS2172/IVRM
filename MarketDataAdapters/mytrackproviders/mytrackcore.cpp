// MyTrackCore.cpp: implementation of the CMyTrackCore class.
//C:\Program Files\Egar\Egar Dispersion\EgarDispersion.exe
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyTrackCore.h"
#include "common.h"
  
const _bstr_t g_bstrDefCurrency = DEF_CURRENCY;
char  CMyTrackCore::m_IsAllExchanges  = 0; 

//--------------------------------------------------------------------------------------------------//
CMyTrackCore::CMyTrackCore()
{
	m_bIsConnected = false;
	m_bIsInitialConnection = false;
	m_bIsInitialized = false;	
	m_lReqCounter = 3;	
	m_iDisconnectionMessage = 0;
	m_bThreadStarted = false;
	m_hResponseThread = INVALID_HANDLE_VALUE;
	m_hMsgThread = INVALID_HANDLE_VALUE;
	m_hLostRequestThread = INVALID_HANDLE_VALUE;
	m_hRequestThread = INVALID_HANDLE_VALUE;

	m_hStopEvent = ::CreateEvent (NULL,TRUE,FALSE,NULL);
	m_hOneMsgEvent = ::CreateEvent (NULL,TRUE,FALSE,NULL);
	m_hOneRequest = ::CreateEvent (NULL,TRUE,FALSE,NULL);
	m_hResubscribeAll = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hNextRequestEvent = ::CreateEvent(NULL,FALSE,TRUE,NULL);
	m_hCheckMessages    = ::CreateEvent(NULL,FALSE,FALSE,NULL);

	m_pmapResponse = new CResponceMap ;
	m_pMsgBuffer =	new CMsgBuffer();
	m_pMsgBuffer->Reset ();	
	m_hConnectionMutex = ::CreateMutex (NULL,FALSE,NULL);
	m_hEventMutex = ::CreateMutex (NULL,FALSE,NULL);
	WSADATA WsaData;
	WSAStartup (MAKEWORD(2,2), &WsaData);
	//m_lReqCount = 0;
	//m_lRespCount = 0;
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::ClearMsgBuffer()
{
	// swap push & pop queues
	m_pMsgBuffer->Swap();
	
	// pop all data from incoming positions queue
	while(m_pMsgBuffer->PopSize() > 0)
	{
		// get front data from queue
		const CBufferInfo& data = m_pMsgBuffer->Front();
		if (data.m_pMsg)
			delete (MYTRACK_MSG*)data.m_pMsg;
		m_pMsgBuffer->Pop();
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::ClearRequestBuffer()
{
	while (m_RequestBuffer.size ()!=0)
	{
		CRequestData* pData = m_RequestBuffer.front ();
		if (!::IsBadCodePtr ((FARPROC)pData))
			delete (CRequestData*)pData;
		m_RequestBuffer.pop_front ();
	}
}

//--------------------------------------------------------------------------------------------------//
CMyTrackCore::~CMyTrackCore()
{
	ATLTRACE(_T("~CMyTrackCore begin...\n"));
	
	if (m_hStopEvent!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hStopEvent);

	if (m_hConnectionMutex != INVALID_HANDLE_VALUE)
		::CloseHandle (m_hConnectionMutex);

	if (m_hOneMsgEvent!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hOneMsgEvent);
	
	if (m_hEventMutex!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hEventMutex);
	
	if (m_hOneRequest!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hOneRequest);
	
	if (m_hNextRequestEvent!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hNextRequestEvent);

	if (m_hCheckMessages!=INVALID_HANDLE_VALUE)
		::CloseHandle (m_hCheckMessages);

	if(m_hResubscribeAll!=INVALID_HANDLE_VALUE)
		::CloseHandle(m_hResubscribeAll);
	

	m_pmapResponse->clear ();
	delete (CResponceMap*)m_pmapResponse; 
	
	//for first Queue
	ClearMsgBuffer();
	//for second Queue
	ClearMsgBuffer();
	m_pMsgBuffer->Reset ();	
	delete (CMsgBuffer*)m_pMsgBuffer; 
	if (m_bIsInitialized)
		mtDeInit ();
	WSACleanup ();
	ATLTRACE(_T("~CMyTrackCore end...\n"));
}
//--------------------------------------------------------------------------------------------------//
const CMyTrackError  CMyTrackCore::m_MyTrackErrors[]=
{
	{MT_ERR_WSA,				_T("Error on WSAStartup"),enInternalError},
	{MT_ERR_NO_Q,				_T("Could not init queues"),enInternalError},
	{MT_ERR_NO_MALLOC,			_T("Could not malloc queue elements"),enInternalError},
	{MT_ERR_NO_SOCKET,			_T("Could not get a socket"),enInternalError},
	{MT_ERR_NO_FIND_IP_ADDR,	_T("Cannot find sdk.tdc.com ip address"),enInternalError},
	{MT_ERR_NO_CONNECT,			_T("Could not connect to mytrack"),enCouldNotConnectToProvider},
	{MT_ERR_INPUT_THREAD,		_T("Could not start input thread"),enInternalError},
	{MT_ERR_OUTPUT_THREAD,		_T("Could not start output thread"),enInternalError},
	{MT_ERR_TIMER_THREAD,		_T("Could not start timer thread"),enInternalError},
	{MT_ERR_INIT_NOT_CALLED,	_T("You did not call mtInit"),enInternalError},
	{MT_ERR_NOT_CONNECTED,		_T("You are not connected to a myTrack server"),enNotConnected},
	{MT_ERR_NO_MESSAGE,			_T("There is no message right now"),enInternalError},
	{MT_ERR_SPARE2,				_T("You are already logged on"),enInternalError},
	{MT_ERR_ARG_LENGTH,			_T("# of bytes of an argument is invalid"),enInternalError},
	{MT_ERR_NO_OUTPUT_Q_LINKS,	_T("Could not send a msg - ran out of queue links"),enInternalError},
	{MT_ERR_OUT_MSG_TOO_LARGE,	_T("TDC problem MT_ERR_OUT_MSG_TOO_LARGE - contact Support"),enInternalError},
	{MT_ERR_IN_MSG_TOO_LARGE,	_T("TDC problem MT_ERR_IN_MSG_TOO_LARGE - contact Support"),enInternalError},
	{MT_ERR_DISCONNECT,			_T("Connection was disconnected"),enConnectionWasDisconnected},
	{MT_ERR_NO_HB_FROM_SERVER,	_T("No heart beat from server"),enConnectionWasDisconnected},
	{MT_ERR_INVALID_ARGUMENT,	_T("Passed to the api routine"),enInternalError},
	{MT_ERR_INVALID_ACCT_ID,	_T("Passed to a brokerage api routine"),enInternalError},
	{MT_ERR_DEINIT_CALLED,		_T("Cannot call any api function"),enInternalError},
	{MTS_ERR_NO_ROOM,				_T("No more room for connection"),enCouldNotConnectToProvider},
	{MTS_ERR_UNKNOWN_CONNECTION,	_T("Unknown connection"),enCouldNotConnectToProvider},
	{MTS_ERR_TOO_MANY_CONNECTIONS,	_T("Too many connections already opened"),enCouldNotConnectToProvider},
	{MTS_ERR_NO_HEART_BEAT,			_T("No heartbeat received - disconnecting (mc=2)"),enCouldNotConnectToProvider},
	{MTS_ERR_INVALID_ACCT,			_T("Invalid account-password"),enInvalidAccount},
	{MTS_ERR_NO_SUCH_TICKER,		_T("Ticker not in database"), enSymbolNotSupported},
	{MTS_ERR_NOT_LOGON,				_T("You did not logon"),enNotConnected},
	{MTS_ERR_LOGON_ALREADY,			_T("You logged on already"),enProviderInternalError},
	{MTS_ERR_TOO_MANY_REQUESTS,		_T("Too many requests outstanding"),enProviderInternalError},
	{MTS_ERR_DB_NOT_AVAILABLE,		_T("Database requested is temporarily not available"),enProviderInternalError},
	{MTS_ERR_NO_ACTIVITY,			_T("No activity in a while - connection will be closed"),enConnectionWasDisconnected},
	{MTS_ERR_DB_ERROR,				_T("Database error, with your request"),enProviderInternalError},
	{MTS_ERR_NO_DATA,				_T("No data available for the ticker requested"), enNoDataAvailableForSymbol},
	{MTS_ERR_NO_PRIOR_REQUEST,		_T("No prior request"),enProviderInternalError},
	{MTS_ERR_INVALID_REQUEST,		_T("Invalid request (check your parameters)"),enProviderInternalError},
	{MTS_ERR_UNKNOWN_REQUEST,		_T("No such request (invalid messageCode)"),enProviderInternalError},
	{MTS_ERR_TOO_MANY_LINES,		_T("# of lines of data exceeds system max"),enProviderInternalError},
	{MTS_ERR_UNKNOWN_RQN,			_T("Unknown request # (unknown rqn)"),enProviderInternalError},
	{MTS_ERR_INVALID_RQN,			_T("Rqn of 0 is invalid"),enProviderInternalError},
	{MTS_ERR_TOO_MANY_CONCURRENT,	_T("Too many concurrent requests"),enProviderInternalError},
	{MTS_ERR_INVALID_VERSION,			_T("Your version # is no longer supported"),enVersionIsNoLongerSupport},
	{MTS_ERR_WAREHOUSE_NOT_AVAILABLE,	_T("Data warehouse system not available"),enProviderInternalError},
	{MTS_ERR_TDS_NOT_AVAILABLE,			_T("Tds data base not available"),enProviderInternalError},
	{MTS_ERR_WAREHOUSE_GET,				_T("Data request for key specified not found (get)"),enProviderInternalError},
	{MTS_ERR_WAREHOUSE,					_T("Data warehouse error on access to data base"),enProviderInternalError},
	{MTS_ERR_WAREHOUSE_PUT,				_T("Data warehouse error on put to data base"),enProviderInternalError},
	{MTS_ERR_PROFILE,					_T("Profile data base error"),enProviderInternalError},
	{MTS_ERR_NO_OPTIONS,				_T("Security has no options"),enNoAccessForData},
	{MTS_ERR_NO_FUTURES,				_T("Security has no futures"),enNoAccessForData},
	{MTS_ERR_NO_COMPOSITE,				_T("Security has no composite markets"),enProviderInternalError},
	{MTS_ERR_NO_OTC_BB,					_T("Security not on otc bb"),enProviderInternalError},
	{MTS_ERR_NO_TRADE,					_T("Could not forward trade request to db server"),enProviderInternalError},
	{MTS_ERR_FILE,						_T("File does not exist"),enProviderInternalError},
	{MTS_ERR_NO_SDK_FEATURE,			_T("SDK feature not enabled"),enProviderInternalError},
	{MTS_ERR_NO_SDK_REGISTRATION,		_T("You did not register for the SDK"),enProviderInternalError},
	{-1,NULL}
};
//--------------------------------------------------------------------------------------------------//
CMyTrackError	CMyTrackCore::GetMyTrackError(int err)
{
	int i=0;
	while (m_MyTrackErrors[i].m_szMessage)
	{
		if (m_MyTrackErrors[i].m_iError == err)
			return m_MyTrackErrors[i];
		i++;
	}
	CMyTrackError tagErr = {-1,_T("Unknown error"),enUnknown};
	return tagErr;
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::Connect(CBaseNotifier *pNotifier)
{
	ATLTRACE(_T("Main thread: %x\n"), GetCurrentThreadId ());
	try
	{
		int err_code;
		if(m_bIsInitialConnection)
			return;

		CCriticalSectionWrapper  csWrap(m_csConnect);
		
		if (!m_bIsInitialized)
		{
			err_code = mtInit(); // init the myTrack environment
			if (err_code)
				throw(GetMyTrackError(err_code));
			m_bIsInitialized=true;
		}
		if (!m_bIsConnected)
		{
			_Connect ();
			m_bThreadStarted = true;
			m_uiResponseThreadID = 0;
			m_hResponseThread = (HANDLE)_beginthreadex(NULL, 0, CMyTrackCore::_ResponseThread, (LPVOID)this, 0, &m_uiResponseThreadID);
			m_uiMsgThreadID = 0;
			m_hMsgThread = (HANDLE)_beginthreadex(NULL, 0, CMyTrackCore::_MsgThread, (LPVOID)this, 0, &m_uiMsgThreadID);
			m_uiMsgThreadID = 0;
			m_hRequestThread = (HANDLE)_beginthreadex(NULL, 0, CMyTrackCore::_RequestThread, (LPVOID)this, 0, &m_uiMsgThreadID);

			m_uiLostMsgThreadID = 0;
			m_hLostRequestThread = (HANDLE)_beginthreadex(NULL, 0, _CheckLostRequestsThread, (LPVOID)this, 0, &m_uiLostMsgThreadID);

		}
		FindAndAddNotifier(pNotifier);
		m_bIsInitialConnection = true;
	}
	catch (_com_error &e)
	{
		mtDisconnect ();
		throw e;		
	}		
	catch (CMyTrackError &err)
	{
		mtDisconnect ();
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		ATLTRACE(_T("Error occured.Connect ->%s\n"),szErr);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
}
//--------------------------------------------------------------------------------------------------//
// get last data
void CMyTrackCore::RequestLastQuote(CBaseNotifier *pNotifier, 
									InstrumentTypeEnum enType, 
									LPCSTR lpszSymbol,
									LPCSTR lpszExchange,
									const CComVariant &varRequest)
{
	try
	{
		if(!m_bIsInitialized)
			throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
		if (lpszSymbol==NULL || (lpszSymbol!=NULL && strcmp(lpszSymbol,"")==0))
			CCommonException::ThrowError (E_FAIL,L"Method RequestLastQuote. There is no symbol to update quote");

		std::string strSymbol("");
		std::string strOptionCode("");
		TransformSymbolName(lpszSymbol,lpszExchange,strSymbol,strOptionCode,enType);
		if (enType==enSTK || enType==enIDX || enType==enOPT)
		{
				CRequestData *rd;
				rd = new (CRequestData);
				rd->m_pNotifier = pNotifier;
				rd->m_strSymbol = strSymbol;
				rd->m_strOption = strOptionCode;
				rd->m_strOriginal = lpszSymbol;
				rd->m_varRequest.Copy (&varRequest);
				rd->m_enRequestType = enRequestLastQuote;
				CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
				m_RequestBuffer.push_back (rd);
				::SetEvent (m_hOneRequest);
		}
		else
			CCommonException::ThrowError (E_FAIL,L"Method GetLastData. Instrument not supported");
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CMyTrackError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::CancelLastQuote(CBaseNotifier *pNotifier,LPCSTR lpszSymbol)
{
#ifndef _NO_CANCEL_ERROR
	if(!m_bIsInitialized)
		throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
#endif
	CancelQuoteResponce(pNotifier,lpszSymbol,enRequestLastQuote);
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::CancelStock(CBaseNotifier *pNotifier,LPCSTR lpszSymbol)
{
#ifndef _NO_CANCEL_ERROR
	if(!m_bIsInitialized)
		throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
#endif
	CancelQuoteResponce(pNotifier,lpszSymbol,enRequestStock);
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::SubscribeQuote(CBaseNotifier *pNotifier, 
								  InstrumentTypeEnum enType, 
								  LPCSTR lpszSymbol,
								  LPCSTR lpszExchange,
								  const CComVariant &varRequest,
								  bool bCheck)
{
	try
	{
		if(!m_bIsInitialized)
			throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
		if (lpszSymbol==NULL || (lpszSymbol!=NULL && strcmp(lpszSymbol,"")==0))
			CCommonException::ThrowError (E_FAIL,L"Method SubscribeQuote. There is no symbol to subscribe quote");
		std::string strSymbol("");
		std::string strOptionCode("");
		TransformSymbolName(lpszSymbol,lpszExchange,strSymbol,strOptionCode,enType);
		if (enType==enSTK || enType==enIDX || enType==enOPT)
		{
			CRequestData *rd;
			rd = new (CRequestData);
			rd->m_pNotifier = pNotifier;
			rd->m_strSymbol = strSymbol;
			rd->m_strOption = strOptionCode;
			rd->m_strOriginal = lpszSymbol;
			rd->m_varRequest.Copy (&varRequest);
			rd->m_enRequestType = enSubscribeQuote;
			CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
			m_RequestBuffer.push_back (rd);
			::SetEvent (m_hOneRequest);
		}
		else
			CCommonException::ThrowError (E_FAIL,L"Method SubscribeQuote. Instrument not supported");
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CMyTrackError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::UnsubscribeQuote(CBaseNotifier *pNotifier,LPCSTR lpszSymbol)
{
	try
	{
#ifndef _NO_CANCEL_ERROR
		if(!m_bIsInitialized)
			throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
#endif							
		
		{
			CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
			for (CRequestBuffer::iterator it = m_RequestBuffer.begin ();it!=m_RequestBuffer.end ();it++)
			{
				CRequestData *pData = *it;
				if (pData && pData->m_pNotifier==pNotifier && pData->m_enRequestType==enSubscribeQuote)
				{
					if (lpszSymbol!=NULL &&	stricmp(pData->m_strOriginal.c_str (),lpszSymbol)==0)
					{
						pData->m_bSendResponce = false;
						pData->m_pNotifier = NULL;
						break;
					}
					else
					{
						pData->m_bSendResponce = false;
						pData->m_pNotifier = NULL;
					}
				}
			}
		}
		
		CCriticalSectionWrapper  csWrap(m_csRespMap);
#ifdef _SYNC							
		CBaseNotifier::WaitWithEvents (m_hEventMutex);
#endif							
		
		for(CResponceMap::iterator itr = m_pmapResponse->begin ();itr != m_pmapResponse->end();itr++)
		{
			if (itr->second.m_pNotifier==pNotifier && 
				itr->second.m_enRequestType==enSubscribeQuote)
			{
				int iRn = itr->first;
				if (lpszSymbol==NULL)
					UnsubscribeOneQuote(iRn,&(itr->second));
				else
					if (stricmp(itr->second.m_strOriginal.c_str (),lpszSymbol)==0)
					{
						UnsubscribeOneQuote(iRn,&(itr->second));
						break;
					}
			}
		}
#ifdef _SYNC							
		::ReleaseMutex (m_hEventMutex);
#endif							
	}
	catch (CMyTrackError &err)
	{
#ifdef _SYNC							
		::ReleaseMutex (m_hEventMutex);
#endif							
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
		
}
//--------------------------------------------------------------------------------------------------//
UINT __stdcall CMyTrackCore::_ResponseThread(LPVOID pArg)
{
	ATLTRACE(_T("_ResponseThread thread: %x\n"), GetCurrentThreadId ());
	CMyTrackCore *pMyTrackCore = reinterpret_cast<CMyTrackCore*>(pArg);
	if (::IsBadCodePtr ((FARPROC)pMyTrackCore))
		return 2;
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return 2;

	HANDLE& hStop = pMyTrackCore->GetStopEvent();
	HANDLE& hOneMsg = pMyTrackCore->GetOneMsgEvent();
	
	int iMessageCode;
	MYTRACK_MSG mtkMsg;

	CMsgBuffer	*pBufferQueue =  pMyTrackCore->GetBufferQueue();
	CResponceMap *pmapResponse = pMyTrackCore->GetResponceMap();

	CComAutoCriticalSection& respCS = pMyTrackCore->GetResponceMapCS();

	if (::IsBadCodePtr ((FARPROC)pmapResponse) || 
		::IsBadCodePtr ((FARPROC)pBufferQueue) )
		return 2;

	CResponceMap::iterator itrFind;

	::ResetEvent (hOneMsg);
	int iInterval  = 1000;
	int iSaveCode = MT_ERR_NO_MESSAGE;

	while(true)
	{
		if(::WaitForSingleObject(hStop, 0)==WAIT_OBJECT_0)
			break;
		ZeroMemory(&mtkMsg, sizeof(mtkMsg));
		iMessageCode = mtGetMessage(&mtkMsg, iInterval);

		CMyTrackError tagErr = CMyTrackCore::GetMyTrackError (iMessageCode);
		if( tagErr.m_enExtError == enNoAccessForData
		 || tagErr.m_enExtError == enSymbolNotSupported 
		 || tagErr.m_enExtError == enNoDataAvailableForSymbol)
			continue;

		if(iMessageCode == MT_ERR_NO_MESSAGE && !pMyTrackCore->m_bIsConnected)
		{
			iMessageCode =  pMyTrackCore->m_iDisconnectionMessage;
		}

		if (iMessageCode!=0 && iMessageCode!=MT_ERR_NO_MESSAGE )
		{	
			if(pMyTrackCore->m_tagLoginInfo.m_iUseProxy && iMessageCode==MT_ERR_NO_HB_FROM_SERVER)
			{
				int y=0;
			}
			else
			{
				if(tagErr.m_enExtError==enCouldNotConnectToProvider|| tagErr.m_enExtError==enNotConnected||tagErr.m_enExtError==enConnectionWasDisconnected )
				{
					CInnerErrStatus err = pMyTrackCore->CheckErrConnection(iMessageCode);
					if (err != errConnect)
					{
						CComVariant varStub;
						varStub.Clear();	
						pMyTrackCore->RaiseMyTrackError(iMessageCode,
														varStub);
						iSaveCode = iMessageCode;
						if(::WaitForSingleObject(hStop, 40*1000)==WAIT_OBJECT_0)
							break;
					}
					else
					{
						pMyTrackCore->m_iDisconnectionMessage = MT_ERR_NO_ERROR;
					}
				}
				else
				{
						CComVariant varStub;
						varStub.Clear();	
						pMyTrackCore->RaiseMyTrackError(iMessageCode,
														varStub);
						iSaveCode = iMessageCode;
						if(::WaitForSingleObject(hStop, 0)==WAIT_OBJECT_0)
							break;
				}
			}
			continue;
		} 

		if (iMessageCode == MT_ERR_NO_MESSAGE)
		{
			continue; 
		}
		SetEvent(pMyTrackCore->m_hNextRequestEvent);
		ATLTRACE(_T("Msg code get: %d\n"), iMessageCode);

		bool bIsEnd = false;
		respCS.Lock();

		itrFind = pmapResponse->find (mtkMsg.rqn);
		bIsEnd = (itrFind==pmapResponse->end());

		respCS.Unlock();

		if (bIsEnd)
			continue;
		else
		{
			if (::IsBadCodePtr ((FARPROC)itrFind->second.m_pNotifier))
			{
				pMyTrackCore->EraseSubscription(&mtkMsg.rqn);
				continue;
			}
		}
		pBufferQueue->LockPush();
		CBufferInfo tagInfo;
		tagInfo.m_reqNum = mtkMsg.rqn;
		tagInfo.m_pMsg = new MYTRACK_MSG;
		::memcpy(tagInfo.m_pMsg,&mtkMsg,sizeof(MYTRACK_MSG));
		pBufferQueue->Push(tagInfo);
		pBufferQueue->UnlockPush();
		::SetEvent (hOneMsg);
		
	}
	::CoUninitialize ();
	return 0;
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::RaiseMyTrackError(const int &Error,
									 CComVariant &varRequest, 
									 CBaseNotifier *pNotifier,
									 RequestsTypeEnum enReqType)
{
	CMyTrackError tagErr = CMyTrackCore::GetMyTrackError (Error);

	if(tagErr.m_enExtError != enSymbolNotSupported && 
	   tagErr.m_enExtError != enNoDataAvailableForSymbol &&
	   tagErr.m_enExtError != enNoAccessForData)
	   {
			m_bIsConnected = false;
			m_iDisconnectionMessage = Error;
	   }   
	


	if (pNotifier==NULL)
	{
		for (CClientList::iterator it = m_lstClients.begin (); it != m_lstClients.end();it++)
		{
			CBaseNotifier *pNotif = *it;
			if (pNotif)
			{
#ifdef _SYNC							
				CBaseNotifier::WaitWithEvents (m_hEventMutex);
				pNotif->OnError (tagErr.m_enExtError,_bstr_t(tagErr.m_szMessage),enReqType,varRequest);
				::ReleaseMutex (m_hEventMutex);
#else
				pNotif->OnError (tagErr.m_enExtError,_bstr_t(tagErr.m_szMessage),enReqType,varRequest);
#endif							
			}
		}
	}
	else
	{
#ifdef _SYNC							
		CBaseNotifier::WaitWithEvents (m_hEventMutex);
		pNotifier->OnError (tagErr.m_enExtError,_bstr_t(tagErr.m_szMessage),enReqType,varRequest);
		::ReleaseMutex (m_hEventMutex);
#else
		pNotifier->OnError (tagErr.m_enExtError,_bstr_t(tagErr.m_szMessage),enReqType,varRequest);
#endif							
	}
	
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::RaiseComError(const _com_error & e, CRequestData &Request)
{
	try
	{
		_bstr_t bstrErr(e.ErrorInfo ()==NULL?e.ErrorMessage ():e.Description ());  
		if (Request.m_pNotifier)
			Request.m_pNotifier->OnError (enUnknown,bstrErr,Request.m_enRequestType,Request.m_varRequest);
	}
	catch(_com_error &e)
	{
		if (e.ErrorInfo ())
			ATLTRACE((LPCTSTR)e.Description ());
		else
			ATLTRACE(e.ErrorMessage ());
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::Disconnect(CBaseNotifier *pNotifier)
{
	if (pNotifier==NULL)
		return;
#ifdef _SYNC							
	CBaseNotifier::WaitWithEvents (m_hEventMutex);
#endif							
	
	{
		CCriticalSectionWrapper  csWrap(m_csConnect);
		if (m_hResponseThread!=INVALID_HANDLE_VALUE)
			::SuspendThread (m_hResponseThread);
		if (m_hMsgThread !=INVALID_HANDLE_VALUE)
			::SuspendThread (m_hMsgThread);
		if (m_hRequestThread !=INVALID_HANDLE_VALUE)
			::SuspendThread (m_hRequestThread);

		if (m_hLostRequestThread !=INVALID_HANDLE_VALUE)
			::SuspendThread (m_hLostRequestThread);

	
		RemoveClient (pNotifier);

		if (m_hResponseThread!=INVALID_HANDLE_VALUE)
			::ResumeThread(m_hResponseThread);
		if (m_hMsgThread !=INVALID_HANDLE_VALUE)
			::ResumeThread(m_hMsgThread);
		if (m_hRequestThread !=INVALID_HANDLE_VALUE)
			::ResumeThread(m_hRequestThread);
		if (m_hLostRequestThread !=INVALID_HANDLE_VALUE)
			::ResumeThread(m_hLostRequestThread);

		
	}
	if (m_lstClients.size ()==0)
	{
		if (m_hResponseThread!=INVALID_HANDLE_VALUE && m_bThreadStarted)
		{
			ATLTRACE(_T("Disconnect begin...\n"));
			::SetEvent(m_hOneMsgEvent);
			::SetEvent(m_hStopEvent);
			::SetEvent(m_hOneRequest);

			MSG msg;	
			HANDLE hThreads[3] = {m_hResponseThread,m_hRequestThread,m_hLostRequestThread};
			//::WaitForSingleObject (m_hResponseThread,INFINITE);
			::WaitForMultipleObjects (3,hThreads,TRUE,INFINITE);
			//
			ClearRequestBuffer();

			ATLTRACE(_T("Disconnect m_hResponseThread...\n"));
			bool bTerminate = false;
			while(true)
			{
				DWORD dwRet = MsgWaitForMultipleObjects( 1,&m_hMsgThread,FALSE,10000,QS_ALLEVENTS);
				if (dwRet == WAIT_TIMEOUT)
				{
					bTerminate = true;
					break;
				}

				if(dwRet == WAIT_OBJECT_0)
					break;
				else 
				if(dwRet == WAIT_OBJECT_0 + 1)
				{
					ATLTRACE(_T("Disconnect WAIT_OBJECT_0 + 1...\n"));
					while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
					::SetEvent(m_hOneMsgEvent);
					ATLTRACE(_T("Disconnect WAIT_OBJECT_0 + 1 end...\n"));
				} 
			}	
			ATLTRACE(_T("Disconnect m_hMsgThread...\n"));
			::CloseHandle (m_hResponseThread);
			if (bTerminate)
				::TerminateThread (m_hMsgThread,0);
			::CloseHandle (m_hMsgThread);
			m_hMsgThread = INVALID_HANDLE_VALUE;
			m_hResponseThread = INVALID_HANDLE_VALUE;
			::ResetEvent (m_hStopEvent);
			ATLTRACE(_T("Disconnect end...\n"));
		}
			
		if (m_bIsConnected)
		{
			ATLTRACE(_T("mtDisconnect begin...\n"));
			mtRequestLogoff (GetNewReqNum());
			mtDisconnect ();
			m_bIsConnected = false;
			ATLTRACE(_T("mtDisconnect end...\n"));
		}
	}
	m_bIsInitialConnection = false;
#ifdef _SYNC							
	::ReleaseMutex (m_hEventMutex);
#endif							
}

long GetRegValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, std::string& rValue, REGSAM samDesired = KEY_QUERY_VALUE)
{
	rValue.erase();
	eg_reg::CEgRegKey keySoftware;
	long lResult = keySoftware.Open(hKeyParent, lpszKeyName, samDesired);
	if(lResult != ERROR_SUCCESS)
		return lResult;

	_bstr_t bsVal;
	lResult = keySoftware.QueryValue(bsVal, lpszValueName);
	if(lResult == ERROR_SUCCESS)
		rValue = (LPCTSTR)bsVal;

	return lResult;
}

long GetRegValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, DWORD& dwValue, REGSAM samDesired = KEY_QUERY_VALUE)
{
	eg_reg::CEgRegKey keySoftware;
	long lResult = keySoftware.Open(hKeyParent, lpszKeyName, samDesired);
	if(lResult != ERROR_SUCCESS)
		return lResult;
	return keySoftware.QueryValue(dwValue, lpszValueName);
}

//--------------------------------------------------------------------------------------------------//
//getting registry information
bool CMyTrackCore::LoadProviderInfo()
{
	DWORD dwValue;
	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("RevHi"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("RevHi"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			dwValue = 1; 

	m_tagLoginInfo.m_iRevHi = dwValue;
		
	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("RevLo"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("RevLo"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			dwValue = 0; 
		
	m_tagLoginInfo.m_iRevLow = dwValue;
	
	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("LogonTimeout"), dwValue, KEY_QUERY_VALUE) == ERROR_SUCCESS
		|| GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("LogonTimeout"), dwValue, KEY_QUERY_VALUE) == ERROR_SUCCESS)
		m_tagLoginInfo.m_iLogonTimeout = dwValue;
	else
		m_tagLoginInfo.m_iLogonTimeout = TIMEOUT_LOGON;
	
	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("RequestTimeout"), dwValue, KEY_QUERY_VALUE) == ERROR_SUCCESS
		|| GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("RequestTimeout"), dwValue, KEY_QUERY_VALUE) == ERROR_SUCCESS)
		m_tagLoginInfo.m_iRequestTimeout = dwValue;
	else
		m_tagLoginInfo.m_iRequestTimeout = TIMEOUT_MYTRACK;
	
	std::string sValue;
	m_tagLoginInfo.m_iTimeShift = 0;
	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("TimeShift"), sValue, KEY_QUERY_VALUE) == ERROR_SUCCESS
		|| GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("TimeShift"), sValue, KEY_QUERY_VALUE) == ERROR_SUCCESS)
	{
		if(sValue.length() > 0)
			m_tagLoginInfo.m_iTimeShift = atoi(sValue.c_str());
	}

	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("Account"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("Account"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			return false;

	m_tagLoginInfo.m_strUserName = sValue.c_str();

	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("Password"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("Password"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			return false;
		
	m_tagLoginInfo.m_strUserPass = sValue.c_str();
		

	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("ProductName"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("ProductName"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			sValue = "MyTrackPrvs";
	m_tagLoginInfo.m_strAppName = sValue.c_str();

	dwValue = 0;

	if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("UseProxy"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
	{
		if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("UseProxy"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
		{
			m_tagLoginInfo.m_iUseProxy = 0;
		}
		else
			m_tagLoginInfo.m_iUseProxy = dwValue;
	}
	else
		m_tagLoginInfo.m_iUseProxy = dwValue;

	if(m_tagLoginInfo.m_iUseProxy == 1)
	{
		// load from user registry
		if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("Proxy"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("Proxy"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
				return false;
         m_tagLoginInfo.m_strProxyName = sValue;

		if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("Port"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("Port"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
				return false;
        m_tagLoginInfo.m_iProxyPort = dwValue;
	}
	if(m_tagLoginInfo.m_iUseProxy == 2)
	{
		// load from IE settings
		if(GetRegValue(HKEY_CURRENT_USER, DEF_INET_SETTINGS, _T("ProxyEnable"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS || dwValue == 0)
			m_tagLoginInfo.m_iUseProxy = 0;
		else
		{
			if(GetRegValue(HKEY_CURRENT_USER, DEF_INET_SETTINGS, _T("ProxyServer"), sValue, KEY_QUERY_VALUE) != ERROR_SUCCESS || sValue.size() == 0)
				m_tagLoginInfo.m_iUseProxy = 0;
			else
			{
				if(std::string::npos == sValue.find(';'))
				{
					// One Proxy
					size_t  pos = sValue.find(':');
					if(pos == std::string::npos)
						return false;
					m_tagLoginInfo.m_strProxyName = sValue.substr(0, pos);
					std::string str = sValue.substr(pos+1, std::string::npos);
					m_tagLoginInfo.m_iProxyPort = atoi(str.c_str());
				}
				else
				{
					size_t  pos = sValue.find("http=");
					if(pos == std::string::npos)
						m_tagLoginInfo.m_iUseProxy = 0;
					else
					{
						size_t posEnd = sValue.find(';' , pos);
						std::string strHttp = sValue.substr(pos+5,posEnd-5-pos);
						if(strHttp.size())
						{
							sValue = strHttp;
							size_t  pos = sValue.find(':');
							if(pos == std::string::npos)
								return false;
							m_tagLoginInfo.m_strProxyName = sValue.substr(0, pos);
							std::string str = sValue.substr(pos+1, std::string::npos);
							m_tagLoginInfo.m_iProxyPort = atoi(str.c_str());
						}
					}
				}
				
			}
		}
	}
	
	m_tagLoginInfo.m_strAppName = sValue.c_str();
	return true; 
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::RequestStock(CBaseNotifier *pNotifier, 
								  LPCSTR lpszSymbol,
								  const CComVariant &varRequest)
{
	try
	{
		if(!m_bIsInitialized)
			throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
		if (lpszSymbol==NULL || (lpszSymbol!=NULL && strcmp(lpszSymbol,"")==0))
			CCommonException::ThrowError (E_FAIL,L"Method RequestStock. There is no stock to update");
		CRequestData *rd;
		rd = new (CRequestData);
		rd->m_pNotifier = pNotifier;
		rd->m_strSymbol = lpszSymbol;
		rd->m_strOption = "";
		rd->m_strOriginal = lpszSymbol;
		rd->m_varRequest = varRequest;
		rd->m_strDescription = "";
		rd->m_enRequestType = enRequestStock;
		CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
		m_RequestBuffer.push_back (rd);
		::SetEvent (m_hOneRequest);
		::ReleaseMutex (m_hConnectionMutex);
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CMyTrackError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
	catch(...)
	{
		CCommonException::ThrowError (E_FAIL,L"Connection Error");
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::CommonQuoteResponce(QUOTE_RESPONSE *pMsg,CRequestData *pData)
{
	if (IsBadCodePtr ( (FARPROC) pData) || IsBadCodePtr ( (FARPROC) pData->m_pNotifier)  )
		return;

	if (pData->m_enRequestType==enRequestStock)
	{
		StockQuoteResponce(pMsg,pData);
		return;
	}

	CComVariant varBuffer;
	eg_lib::vt_date dtDate(eg_lib::vt_date::GetCurrentDate ());
	//stock or index
		
	if (pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientPriceProvider)
	{
		if(	0 == pMsg->u.stock.recordType || 
			1 == pMsg->u.stock.recordType)
		{
			QUOTE_RESPONSE_STOCK& tagmsg = pMsg->u.stock;
			_QuoteUpdateResult qur;
			try
			{
				qur.Init();

				qur->UpdateDateTime = (DATE)dtDate;
				qur->SeqNum = 0;

				qur->BidPrice = SetDoubleValue(mtuGetPrice(tagmsg.bid, tagmsg.format));
				qur->AskPrice = SetDoubleValue(mtuGetPrice(tagmsg.ask,tagmsg.format));
				qur->LotSize = DEF_STK_LOTSIZE;
				qur->BidSize = tagmsg.bidSize;
				qur->AskSize = tagmsg.askSize;
				qur->LastPrice = SetDoubleValue(mtuGetPrice(tagmsg.last, tagmsg.format));
				qur->Currency = g_bstrDefCurrency.copy ();

				qur->OpenInterest = BAD_LONG_VALUE; 
				qur->Volume = tagmsg.volume;
				
				std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
				_bstr_t bstrStub = strExchange.c_str();
				qur->Exchange = bstrStub.copy();

				varBuffer.Clear ();
				qur.CopyTo(varBuffer);
				pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
				varBuffer.Clear ();
			}
			catch(_com_error &e)
			{
				varBuffer.Clear ();
				RaiseComError(e,*pData);
			}
		}
		//option
		else if (2 == pMsg->u.stock.recordType)
		{
			QUOTE_RESPONSE_OPTION& msg = pMsg->u.option;
			_QuoteUpdateResult qur;
			try
			{
				qur.Init();

				qur->UpdateDateTime = (DATE)dtDate;
				qur->SeqNum = 0;
				qur->BidPrice = SetDoubleValue(mtuGetPrice(msg.bid, msg.format));
				qur->AskPrice = SetDoubleValue(mtuGetPrice(msg.ask, msg.format));
				qur->LotSize = DEF_OPT_LOTSIZE;
				qur->BidSize = BAD_LONG_VALUE;
				qur->AskSize = BAD_LONG_VALUE;
				qur->LastPrice = SetDoubleValue(mtuGetPrice(msg.last, msg.format));
				varBuffer.Clear ();
				_bstr_t bstrStub(DEF_CURRENCY);
				qur->Currency = bstrStub.copy ();

				qur->OpenInterest = msg.openInterest;
				qur->Volume = msg.volume;

				std::string strExchange(msg.exchangeCode, sizeof(msg.exchangeCode));
				bstrStub = strExchange.c_str();
				qur->Exchange = bstrStub.copy();
					
				qur.CopyTo (varBuffer);
				pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
				varBuffer.Clear ();
			}
			catch(_com_error &e)
			{
				varBuffer.Clear ();
				RaiseComError(e,*pData);
			}
        }
        // market statistic
        else if (4 == pMsg->u.stock.recordType)
        {
            _QuoteUpdateInfo qur;
            try
            {
                qur.Init();

                qur->UpdateDateTime = (DATE)dtDate;
                qur->SeqNum = 0;

                qur->BidPrice = BAD_DOUBLE_VALUE;
                qur->AskPrice = BAD_DOUBLE_VALUE;
                qur->LotSize = BAD_LONG_VALUE;
                qur->BidSize = BAD_LONG_VALUE;
                qur->AskSize = BAD_LONG_VALUE;
                qur->LastPrice = BAD_DOUBLE_VALUE;
                qur->Currency = ::SysAllocString(L"");

                qur->OpenInterest = BAD_LONG_VALUE;
                qur->Volume = BAD_LONG_VALUE;

                qur->OpenPrice = BAD_DOUBLE_VALUE;
                qur->ClosePrice = BAD_DOUBLE_VALUE;
                qur->HighPrice = BAD_DOUBLE_VALUE;
                qur->LowPrice = BAD_DOUBLE_VALUE;
                qur->Skipped = 0;
                qur->TotalRequests = 0;
                qur->WaitTime = 0;

                qur->Exchange = ::SysAllocString(L"");

                varBuffer.Clear ();
                qur.CopyTo(varBuffer);
                pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
            }
            catch(_com_error &e)
            {
                RaiseComError(e,*pData);
            }
        }
		return;
	}

	if ((pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientPriceInfo) ||
		(pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientPriceInfoWithNotify))
	{
		if(pData->m_enRequestType == enRequestLastQuote)
		{
			if ( 0 == pMsg->u.stock.recordType || 
				1 == pMsg->u.stock.recordType)
			{
				QUOTE_RESPONSE_STOCK& tagmsg = pMsg->u.stock;
				_QuoteUpdateInfo qur;
				try
				{
					qur.Init();

					qur->UpdateDateTime = (DATE)dtDate;
					qur->SeqNum = 0;

					qur->BidPrice = SetDoubleValue(mtuGetPrice(tagmsg.bid, tagmsg.format));
					qur->AskPrice = SetDoubleValue(mtuGetPrice(tagmsg.ask,tagmsg.format));
					qur->LotSize = DEF_STK_LOTSIZE;
					qur->BidSize = tagmsg.bidSize;
					qur->AskSize = tagmsg.askSize;
					qur->LastPrice = SetDoubleValue(mtuGetPrice(tagmsg.last, tagmsg.format));
					qur->Currency = g_bstrDefCurrency.copy ();

					qur->OpenInterest = BAD_LONG_VALUE;
					qur->Volume = tagmsg.volume;

					qur->OpenPrice = SetDoubleValue(mtuGetPrice(tagmsg.open, tagmsg.format));
					qur->ClosePrice = SetDoubleValue(mtuGetPrice(tagmsg.yestClose , tagmsg.format));
					qur->HighPrice = SetDoubleValue(mtuGetPrice(tagmsg.high , tagmsg.format));
					qur->LowPrice = SetDoubleValue(mtuGetPrice(tagmsg.low , tagmsg.format));
					qur->Skipped = 0;
					qur->TotalRequests = 0;
					qur->WaitTime = 0;

					
					std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
					_bstr_t bstrStub = strExchange.c_str();
					qur->Exchange = bstrStub.copy();

					varBuffer.Clear ();
					qur.CopyTo(varBuffer);
					pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
					varBuffer.Clear ();
				}
				catch(_com_error &e)
				{
					varBuffer.Clear ();
					RaiseComError(e,*pData);
				}
			}
			//option
			else if (2 == pMsg->u.stock.recordType)
			{
				QUOTE_RESPONSE_OPTION& msg = pMsg->u.option;
				_QuoteUpdateInfo qur;
				try
				{
					qur.Init();

					qur->UpdateDateTime = (DATE)dtDate;
					qur->SeqNum = 0;
					qur->BidPrice = SetDoubleValue(mtuGetPrice(msg.bid, msg.format));
					qur->AskPrice = SetDoubleValue(mtuGetPrice(msg.ask, msg.format));
					qur->LotSize = DEF_OPT_LOTSIZE;
					qur->BidSize = BAD_LONG_VALUE;
					qur->AskSize = BAD_LONG_VALUE;
					qur->LastPrice = SetDoubleValue(mtuGetPrice(msg.last, msg.format));
					varBuffer.Clear ();
					_bstr_t bstrStub(DEF_CURRENCY);
					qur->Currency = bstrStub.copy ();

					qur->OpenInterest = msg.openInterest;
					qur->Volume = msg.volume;

					std::string strExchange(msg.exchangeCode, sizeof(msg.exchangeCode));
					bstrStub = strExchange.c_str();
					qur->Exchange = bstrStub.copy();

					qur->OpenPrice = SetDoubleValue(mtuGetPrice(msg.open, msg.format));
					qur->ClosePrice = SetDoubleValue(mtuGetPrice(msg.yestClose , msg.format));
					qur->HighPrice = SetDoubleValue(mtuGetPrice(msg.high , msg.format));
					qur->LowPrice = SetDoubleValue(mtuGetPrice(msg.low , msg.format));
					qur->Skipped = 0;
					qur->TotalRequests = 0;
					qur->WaitTime = 0;
						
					qur.CopyTo (varBuffer);
					pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
					varBuffer.Clear ();
				}
				catch(_com_error &e)
				{
					varBuffer.Clear ();
					RaiseComError(e,*pData);
				}
			}
			// market statistic
			else if (4 == pMsg->u.stock.recordType)
			{
				_QuoteUpdateInfo qur;
				try
				{
					qur.Init();

					qur->UpdateDateTime = (DATE)dtDate;
					qur->SeqNum = 0;

					qur->BidPrice = BAD_DOUBLE_VALUE;
					qur->AskPrice = BAD_DOUBLE_VALUE;
					qur->LotSize = BAD_LONG_VALUE;
					qur->BidSize = BAD_LONG_VALUE;
					qur->AskSize = BAD_LONG_VALUE;
					qur->LastPrice = BAD_DOUBLE_VALUE;
					qur->Currency = ::SysAllocString(L"");

					qur->OpenInterest = BAD_LONG_VALUE;
					qur->Volume = BAD_LONG_VALUE;

					qur->OpenPrice = BAD_DOUBLE_VALUE;
					qur->ClosePrice = BAD_DOUBLE_VALUE;
					qur->HighPrice = BAD_DOUBLE_VALUE;
					qur->LowPrice = BAD_DOUBLE_VALUE;
					qur->Skipped = 0;
					qur->TotalRequests = 0;
					qur->WaitTime = 0;

					qur->Exchange = ::SysAllocString(L"");

					varBuffer.Clear ();
					qur.CopyTo(varBuffer);
					pData->m_pNotifier->OnLastQuote (pData->m_varRequest,varBuffer);
				}
				catch(_com_error &e)
				{
					RaiseComError(e,*pData);
				}
			}
		}
		else if(pData->m_enRequestType == enSubscribeQuote && 
			(pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientPriceInfoWithNotify))
		{
			if (pData->m_bSendResponce && pData->m_pNotifier)
			{
				pData->m_bSendNotification = false;
				pData->m_pNotifier->OnSubscribed(pData->m_varRequest);
			}

			eg_lib::vt_date dtDate(eg_lib::vt_date::GetCurrentDate ());

			if ( 0 == pMsg->u.stock.recordType || 
				1 == pMsg->u.stock.recordType)
			{
				QUOTE_RESPONSE_STOCK& tagmsg = pMsg->u.stock;
				_QuoteUpdateInfo qur;
				try
				{
					qur.Init();

					qur->UpdateDateTime = (DATE)dtDate;
					qur->SeqNum = 0;

					qur->BidPrice = SetDoubleValue(mtuGetPrice(tagmsg.bid, tagmsg.format));
					qur->AskPrice = SetDoubleValue(mtuGetPrice(tagmsg.ask,tagmsg.format));
					qur->LotSize = DEF_STK_LOTSIZE;
					qur->BidSize = tagmsg.bidSize;
					qur->AskSize = tagmsg.askSize;
					qur->LastPrice = SetDoubleValue(mtuGetPrice(tagmsg.last, tagmsg.format));
					qur->Currency = g_bstrDefCurrency.copy ();

					qur->OpenInterest = BAD_LONG_VALUE;
					qur->Volume = tagmsg.volume;

					qur->OpenPrice = SetDoubleValue(mtuGetPrice(tagmsg.open, tagmsg.format));
					qur->ClosePrice = SetDoubleValue(mtuGetPrice(tagmsg.yestClose , tagmsg.format));
					qur->HighPrice = SetDoubleValue(mtuGetPrice(tagmsg.high , tagmsg.format));
					qur->LowPrice = SetDoubleValue(mtuGetPrice(tagmsg.low , tagmsg.format));
					qur->Skipped = 0;
					qur->TotalRequests = 0;
					qur->WaitTime = 0;


					std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
					_bstr_t bstrStub = strExchange.c_str();
					qur->Exchange = bstrStub.copy();

					varBuffer.Clear ();
					qur.CopyTo(varBuffer);
					pData->m_pNotifier->OnQuoteUpdate(pData->m_varRequest,varBuffer);
					varBuffer.Clear ();
				}
				catch(_com_error &e)
				{
					varBuffer.Clear ();
					RaiseComError(e,*pData);
				}
			}
			//option
			else if (2 == pMsg->u.stock.recordType)
			{
				QUOTE_RESPONSE_OPTION& msg = pMsg->u.option;
				_QuoteUpdateInfo qur;
				try
				{
					qur.Init();

					qur->UpdateDateTime = (DATE)dtDate;
					qur->SeqNum = 0;
					qur->BidPrice = SetDoubleValue(mtuGetPrice(msg.bid, msg.format));
					qur->AskPrice = SetDoubleValue(mtuGetPrice(msg.ask, msg.format));
					qur->LotSize = DEF_OPT_LOTSIZE;
					qur->BidSize = BAD_LONG_VALUE;
					qur->AskSize = BAD_LONG_VALUE;
					qur->LastPrice = SetDoubleValue(mtuGetPrice(msg.last, msg.format));
					varBuffer.Clear ();
					_bstr_t bstrStub(DEF_CURRENCY);
					qur->Currency = bstrStub.copy ();

					qur->OpenInterest = msg.openInterest;
					qur->Volume = msg.volume;

					std::string strExchange(msg.exchangeCode, sizeof(msg.exchangeCode));
					bstrStub = strExchange.c_str();
					qur->Exchange = bstrStub.copy();

					qur->OpenPrice = SetDoubleValue(mtuGetPrice(msg.open, msg.format));
					qur->ClosePrice = SetDoubleValue(mtuGetPrice(msg.yestClose , msg.format));
					qur->HighPrice = SetDoubleValue(mtuGetPrice(msg.high , msg.format));
					qur->LowPrice = SetDoubleValue(mtuGetPrice(msg.low , msg.format));
					qur->Skipped = 0;
					qur->TotalRequests = 0;
					qur->WaitTime = 0;

					qur.CopyTo (varBuffer);
					pData->m_pNotifier->OnQuoteUpdate(pData->m_varRequest,varBuffer);
					varBuffer.Clear ();
				}
				catch(_com_error &e)
				{
					varBuffer.Clear ();
					RaiseComError(e,*pData);
				}
			}
		}
		return;
	}		

}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::StockQuoteResponce(QUOTE_RESPONSE *pMsg,CRequestData *pData)
{
	if (pData->m_enRequestType != enRequestStock)
		return;

	CComVariant varBuffer;
	varBuffer.Clear ();
	//stock or index

    switch (pMsg->u.stock.recordType)
	{
    case 0: /*stock*/
    case 1: /*index*/
        {
		    QUOTE_RESPONSE_STOCK& tagmsg = pMsg->u.stock;
		
		    DATE dtDate;
            SYSTEMTIME st = {0};
    		st.wMonth = tagmsg.divDate[0];
	    	st.wDay = tagmsg.divDate[1];
		    st.wYear = tagmsg.divDate[2];
		    ::SystemTimeToVariantTime(&st, &dtDate);
		
		    try
		    {
			    if (pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureProvider)
			    {
				    _StockResults sr;
				    sr.Init();

				    sr->StockType = (0 == tagmsg.recordType) ? enStock : enIndex;
				    sr->DivFreq = tagmsg.divFrequency;
				    sr->DivLastDate = dtDate;
				    sr->DivAmount = 0.001 * tagmsg.divAmt;

				    _bstr_t bstrStub(DEF_CURRENCY);
				    sr->Currency	= bstrStub.copy ();
    
	    			bstrStub = pData->m_strDescription.c_str ();
				    sr->Description = bstrStub.copy ();
				    sr->LotSize =  DEF_STK_LOTSIZE;

				    std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
				    bstrStub = strExchange.c_str();
				    sr->Exchange = bstrStub.copy();

				    varBuffer.Clear ();
				    sr.CopyTo(varBuffer);
				    pData->m_pNotifier->OnStock(pData->m_varRequest,varBuffer);
				    varBuffer.Clear ();
			    }
			    else if (pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureInfo)
			    {
				    _StockInfo sr;
				    sr.Init();

				    sr->StockType = (0 == tagmsg.recordType) ? enStock : enIndex;
				    sr->DivFreq = tagmsg.divFrequency;
				    sr->DivLastDate = dtDate;
				    sr->DivAmount = 0.001 * tagmsg.divAmt;
				    sr->Currency	= g_bstrDefCurrency.copy ();
				    _bstr_t bstrStub = pData->m_strDescription.c_str ();
				    sr->Description = bstrStub.copy ();
				    sr->LotSize =  DEF_STK_LOTSIZE;

				    std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
				    bstrStub = strExchange.c_str();
				    sr->Exchange = bstrStub.copy();

				    sr->Ask = mtuGetPrice(tagmsg.ask, tagmsg.format);
				    sr->Bid = mtuGetPrice(tagmsg.bid, tagmsg.format);
				    sr->High = mtuGetPrice(tagmsg.high, tagmsg.format);
				    sr->Low	= mtuGetPrice(tagmsg.low, tagmsg.format);
				    sr->OpenPrice	= mtuGetPrice(tagmsg.open, tagmsg.format);
				    sr->ClosePrice = mtuGetPrice(tagmsg.last, tagmsg.format);

				    sr->Volume		= tagmsg.yestVolume;
				    sr->CompositeVolume = tagmsg.compositeVolume;
				    sr->LastTradeVolume = tagmsg.lastTradeVolume;

				    CComVariant vtSr;
				    sr.CopyTo(vtSr);
				    pData->m_pNotifier->OnStock(pData->m_varRequest, vtSr);
			    }
		    }
		    catch(_com_error &e)
		    {
			    RaiseComError(e,*pData);
		    }
        }
        break;
    case 4: /*market statistic*/
        {
            QUOTE_RESPONSE_STOCK& tagmsg = pMsg->u.stock;
            DATE dtDate;
            SYSTEMTIME st = {0};
            st.wMonth = tagmsg.divDate[0];
            st.wDay = tagmsg.divDate[1];
            st.wYear = tagmsg.divDate[2];
            ::SystemTimeToVariantTime(&st, &dtDate);

		    try
		    {
			    if (pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureProvider)
			    {
				    _StockResults sr;
				    sr.Init();

				    sr->StockType = enIndex;
				    sr->DivFreq = tagmsg.divFrequency;
				    sr->DivLastDate = dtDate;
				    sr->DivAmount = 0.001 * tagmsg.divAmt;

				    _bstr_t bstrStub(DEF_CURRENCY);
				    sr->Currency	= bstrStub.copy ();
    
	    			bstrStub = pData->m_strDescription.c_str ();
				    sr->Description = bstrStub.copy ();
				    sr->LotSize =  DEF_STK_LOTSIZE;

				    std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
				    bstrStub = strExchange.c_str();
				    sr->Exchange = bstrStub.copy();

				    sr.CopyTo(varBuffer);
				    pData->m_pNotifier->OnStock(pData->m_varRequest,varBuffer);
			    }
			    else if (pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureInfo)
			    {
				    _StockInfo sr;
				    sr.Init();

				    sr->StockType = enIndex;
				    sr->DivFreq = tagmsg.divFrequency;
				    sr->DivLastDate = dtDate;
				    sr->DivAmount = 0.001 * tagmsg.divAmt;

				    _bstr_t bstrStub(DEF_CURRENCY);
				    sr->Currency = bstrStub.copy ();
    
	    			bstrStub = pData->m_strDescription.c_str ();
				    sr->Description = bstrStub.copy ();
				    sr->LotSize = DEF_STK_LOTSIZE;

				    std::string strExchange(tagmsg.exchange, sizeof(tagmsg.exchange));
				    bstrStub = strExchange.c_str();
				    sr->Exchange = bstrStub.copy();

				    sr->Ask = BAD_DOUBLE_VALUE;
				    sr->Bid = BAD_DOUBLE_VALUE;
				    sr->High = BAD_DOUBLE_VALUE;
				    sr->Low	= BAD_DOUBLE_VALUE;
				    sr->OpenPrice = BAD_DOUBLE_VALUE;
				    sr->ClosePrice = BAD_DOUBLE_VALUE;

				    sr->Volume = BAD_DOUBLE_VALUE;
				    sr->CompositeVolume = BAD_DOUBLE_VALUE;
				    sr->LastTradeVolume = BAD_DOUBLE_VALUE;

				    CComVariant vtSr;
				    sr.CopyTo(vtSr);
				    pData->m_pNotifier->OnStock(pData->m_varRequest, vtSr);
			    }
		    }
		    catch(_com_error &e)
		    {
			    RaiseComError(e,*pData);
		    }
        }
        break;
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::CancelQuoteResponce(CBaseNotifier *pNotifier,LPCSTR lpszSymbol,RequestsTypeEnum enType)
{
	{
		CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
		for (CRequestBuffer::iterator it = m_RequestBuffer.begin ();it!=m_RequestBuffer.end ();it++)
		{
			CRequestData *pData = *it;
			if (pData && pData->m_pNotifier==pNotifier && pData->m_enRequestType==enType)
			{
				if (lpszSymbol!=NULL &&	stricmp(pData->m_strOriginal.c_str (),lpszSymbol)==0)
				{
					pData->m_bSendResponce = false;
					pData->m_pNotifier = NULL;
					break;
				}
				else
				{
					pData->m_bSendResponce = false;
					pData->m_pNotifier = NULL;
				}
			}
		}
	}

	CCriticalSectionWrapper  csWrap(m_csRespMap);
#ifdef _SYNC							
	CBaseNotifier::WaitWithEvents (m_hEventMutex);
#endif							
	

	for(CResponceMap::iterator itr = m_pmapResponse->begin ();itr != m_pmapResponse->end();itr++)
	{
		if (itr->second.m_pNotifier == pNotifier && 
			itr->second.m_enRequestType == enType)
		{
			if (lpszSymbol!=NULL &&	stricmp(itr->second.m_strOriginal.c_str (),lpszSymbol)==0)
			{
				itr->second.m_bSendResponce = false;
				itr->second.m_pNotifier = NULL;
				break;
			}
			else
			{
				itr->second.m_bSendResponce = false;
				itr->second.m_pNotifier = NULL;
			}
		}
	}
#ifdef _SYNC							
	::ReleaseMutex (m_hEventMutex);
#endif							
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::RequestOptions(CBaseNotifier *pNotifier, 
									LPCSTR lpszUnderlyingSymbol,
									const CComVariant &varRequest,
									bool bCheck)
{
	try
	{
		if(!m_bIsInitialized)
			throw(GetMyTrackError(MTS_ERR_NOT_LOGON));
		if (lpszUnderlyingSymbol==NULL)
			CCommonException::ThrowError (E_FAIL,L"Method RequestOptions. Underlying symbol can't be empty");
		CRequestData *rd;
		rd = new (CRequestData);
		rd->m_pNotifier = pNotifier;
		rd->m_strSymbol = lpszUnderlyingSymbol;
		rd->m_strOption = "";
		rd->m_strOriginal = lpszUnderlyingSymbol;
		rd->m_varRequest.Copy (&varRequest);
		rd->m_enRequestType = enRequestOptions;
		CCriticalSectionWrapper  csWrap(m_csRequestBuffer);
		m_RequestBuffer.push_back (rd);
		::SetEvent (m_hOneRequest);
	}
	catch(_com_error &e)
	{
		throw e;
	}
	catch (CMyTrackError &err)
	{
		TCHAR szErr[MAX_PATH];
		_stprintf(szErr,_T("Connection error: %s(%d)"),err.m_szMessage,err.m_iError);
		CCommonException::ThrowError (E_FAIL,(LPCWSTR)_bstr_t(szErr));
	}
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::CancelOptions(CBaseNotifier *pNotifier,LPCSTR lpszUnderlyingSymbol)
{
	CancelQuoteResponce(pNotifier,lpszUnderlyingSymbol,enRequestOptions);
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::RemoveClient(CBaseNotifier *pNotifier)
{
	CCriticalSectionWrapper  csWrap(m_csRespMap);
	//remove all subscription
	CResponceMap::iterator itrFind = m_pmapResponse->begin ();
	for (;itrFind != m_pmapResponse->end();itrFind++)
	{
		if (itrFind->second.m_pNotifier==pNotifier)
		{
			itrFind->second.m_pNotifier = NULL;
			itrFind->second.m_bSendResponce = false;
		}
	}
	m_lstClients.remove (pNotifier);
}
//--------------------------------------------------------------------------------------------------//
//mytarck can't support long Queue
void CMyTrackCore::WaitQueue()
{
	HANDLE hEvents[] = 
	{
		m_hStopEvent,
		m_hNextRequestEvent
	};

	CCriticalSectionWrapper  csWrap(m_csQueue);

	div_t dvFlag = div(m_pmapResponse->size (),DEF_MYTRACK_PACKET);
	if (dvFlag.rem == 0 && m_pmapResponse->size ()!=0)
	{
		while(1)
		{
			if((WAIT_OBJECT_0+1) == MsgWait(2,hEvents,m_tagLoginInfo.m_iRequestTimeout))

			{
				div_t dvF = div(m_pmapResponse->size (),DEF_MYTRACK_PACKET/5);
				if(m_pmapResponse->size()!=0 && dvF.rem != 0)
					continue;
				Sleep(1);
			}

			break;
		}
	}
}
//--------------------------------------------------------------------------------------------------//
//resubscribe all symbols
void CMyTrackCore::ResubscribeAll()
{
	if (m_pmapResponse->size ()==0)
		return;
	CResponceMap mapStub;
	{
		CCriticalSectionWrapper  csWrap(m_csRespMap);
		CResponceMap::iterator itrFind = m_pmapResponse->begin ();
		for (;itrFind != m_pmapResponse->end();itrFind++)
		{
			CRequestData &tagData = itrFind->second;
			if (tagData.m_bSendResponce)
				mapStub.insert (CResponceMap::value_type(itrFind->first,itrFind->second));
		}	
		(*m_pmapResponse).clear ();
	}
	
	CResponceMap::iterator itrFind = mapStub.begin ();
	for (;itrFind != mapStub.end();itrFind++)
	{
		CRequestData tagData = itrFind->second;
		switch(tagData.m_enRequestType) 
		{
		case enRequestLastQuote:
			{
				long lReqNum = GetNewReqNum();
				long lErrorCode = mtRequestQuoteData(lReqNum, 
													(LPSTR)tagData.m_strSymbol.c_str(), 
													(LPSTR)tagData.m_strOption.c_str());
				{
					WaitQueue();
					CCriticalSectionWrapper  csWrap(m_csRespMap);
					(*m_pmapResponse)[lReqNum] = tagData;
				}
					
				break;
			}
		case enSubscribeQuote:
			{
				long lReqNum = GetNewReqNum();

				long lErrorCode = mtRequestIntradayUpdate(lReqNum, 
														(LPSTR)tagData.m_strSymbol.c_str(), 
														(LPSTR)tagData.m_strOption.c_str(),1);
				{
					WaitQueue();
					CCriticalSectionWrapper  csWrap(m_csRespMap);
					(*m_pmapResponse)[lReqNum] = tagData;
				}

				if(CBaseNotifier::enClientPriceInfoWithNotify == tagData.m_pNotifier->GetClientType())
				{					
					lErrorCode = mtRequestQuoteData(lReqNum, 
						(LPSTR)tagData.m_strSymbol.c_str(), 
						(LPSTR)tagData.m_strOption.c_str());
				}

				break;
			}
		case enRequestStock:
			{
				long lReqNum = GetNewReqNum();
				long lErrorCode = mtRequestBackgroundData(lReqNum, (LPSTR)tagData.m_strSymbol.c_str(), "");
				{
					WaitQueue();
					CCriticalSectionWrapper  csWrap(m_csRespMap);
					(*m_pmapResponse)[lReqNum] = tagData;
				}
				break;
			}
		case enRequestOptions:
			{
				long lReqNum = GetNewReqNum();
				long lErrorCode = mtRequestOptionData(lReqNum,(LPSTR)tagData.m_strOriginal.c_str (), 2,m_IsAllExchanges, 0, 0, 1, 0);
				{
					WaitQueue();
					CCriticalSectionWrapper  csWrap(m_csRespMap);
					(*m_pmapResponse)[lReqNum] = tagData;
				}
				break;
			}
		}	
	}
}
//--------------------------------------------------------------------------------------------------//
UINT __stdcall CMyTrackCore::_RequestThread(LPVOID pArg)
{
	ATLTRACE(_T("_RequestThread thread: %x\n"), GetCurrentThreadId ());
	
	CMyTrackCore *pMyTrackCore = reinterpret_cast<CMyTrackCore*>(pArg);
	if (::IsBadCodePtr ((FARPROC)pMyTrackCore))
		return 2;
	//stop event
	HANDLE& hStop = pMyTrackCore->GetStopEvent ();
	//one request present
	HANDLE& hOneRequest = pMyTrackCore->GetOneRequestEvent();

	HANDLE hConnectMutex = pMyTrackCore->GetConnectMutex ();
	CRequestBuffer& Buffer =  pMyTrackCore->GetRequestBuffer();
	CComAutoCriticalSection& RespCS =  pMyTrackCore->GetResponceMapCS();	
	CComAutoCriticalSection& ReqCS =  pMyTrackCore->GetRequestBufferCS();

	CResponceMap *pRespMap = pMyTrackCore->GetResponceMap();
	bool bFlag = false;

	while (true)
	{
		if(Buffer.size () == 0 || !pMyTrackCore->m_bIsConnected)
		{
			if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
				break;

			if(::WaitForSingleObject (hOneRequest,10*1000) == WAIT_TIMEOUT)
			{
				if(bFlag)
				{
					::SetEvent(pMyTrackCore->m_hCheckMessages);
					bFlag = false;
				}
				continue;
			}
		}

		if(::WaitForSingleObject(hStop,0)==WAIT_OBJECT_0)
			break;

		bFlag = true;

		CRequestData data;
		if(pMyTrackCore->m_bIsConnected)
		{
			CCriticalSectionWrapper  csWrap(ReqCS);
			if (Buffer.size ()==0)
			{
				::ResetEvent(hOneRequest);
				continue;
			}
			CRequestData *pData = Buffer.front();
			data = *pData;	
			delete (CRequestData*)pData;
			Buffer.pop_front ();
		}			
		else 			
		{
			Sleep(10*1000);
			continue;
		}

		ATLTRACE("Size req: %d\n",Buffer.size ());
		pMyTrackCore->WaitQueue (); 
		ATLTRACE("Size req1: %d\n",pRespMap->size ());
		
		if (data.m_pNotifier && data.m_bSendResponce)
		{
			switch(data.m_enRequestType)
			{ 
			case enRequestLastQuote:{
										long lReqNum = pMyTrackCore->GetNewReqNum();
										CBaseNotifier::WaitObject (hConnectMutex);
										long lErrorCode = mtRequestQuoteData(lReqNum, 
																			(LPSTR)data.m_strSymbol.c_str(), 
																			(LPSTR)data.m_strOption.c_str());
										::ReleaseMutex(hConnectMutex);	
										if(MT_ERR_NO_ERROR == lErrorCode)
										{
											CCriticalSectionWrapper  csWrap(RespCS);
											(*pRespMap)[lReqNum] = data;
										}
										else
										{
											pMyTrackCore->RaiseMyTrackError(lErrorCode,
																			data.m_varRequest,
																			data.m_pNotifier,
																			data.m_enRequestType);

											{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
											}
										}
										break;

									}
			case enSubscribeQuote:	{
										long lReqNum = pMyTrackCore->GetNewReqNum();
										CBaseNotifier::WaitObject (hConnectMutex);
										
										long  lErrorCode = mtRequestIntradayUpdate(lReqNum, 
																					(LPSTR)data.m_strSymbol.c_str(), 
																					(LPSTR)data.m_strOption.c_str(),1);
										::ReleaseMutex (hConnectMutex);										
											
										if(MT_ERR_NO_ERROR == lErrorCode)
										{
											{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
											}

											if(data.m_pNotifier->GetClientType() == CBaseNotifier::enClientPriceInfoWithNotify)
											{
												lReqNum = pMyTrackCore->GetNewReqNum();
												CBaseNotifier::WaitObject (hConnectMutex);

												lErrorCode = mtRequestQuoteData(lReqNum, 
																			(LPSTR)data.m_strSymbol.c_str(), 
																			(LPSTR)data.m_strOption.c_str());
												::ReleaseMutex (hConnectMutex);

												if(MT_ERR_NO_ERROR == lErrorCode)
												{
													CCriticalSectionWrapper  csWrap(RespCS);
													(*pRespMap)[lReqNum] = data;
												}

											}
										}

										if(MT_ERR_NO_ERROR != lErrorCode)
										{
											pMyTrackCore->RaiseMyTrackError(lErrorCode,
																			data.m_varRequest,
																			data.m_pNotifier,
																			data.m_enRequestType);

											{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
											}
										}
									
										break;
									}
			case enRequestStock:	{
										long lReqNum = pMyTrackCore->GetNewReqNum();
										CBaseNotifier::WaitObject (hConnectMutex);
										long lErrorCode = mtRequestBackgroundData(lReqNum, 
											(LPSTR)data.m_strSymbol.c_str(),
											"");
										::ReleaseMutex (hConnectMutex);										
										
										if(MT_ERR_NO_ERROR == lErrorCode)
										{
											CCriticalSectionWrapper  csWrap(RespCS);
											(*pRespMap)[lReqNum] = data;
										}
										else
										{
											pMyTrackCore->RaiseMyTrackError(lErrorCode,
																			data.m_varRequest,
																			data.m_pNotifier,
																			data.m_enRequestType);

											{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
											}

										}
										break;
									}

			case enRequestOptions:	{
										long lReqNum = pMyTrackCore->GetNewReqNum();
										CBaseNotifier::WaitObject (hConnectMutex);
										long lErrorCode = mtRequestOptionData(lReqNum,(LPSTR)data.m_strOriginal.c_str(), 2, m_IsAllExchanges, 0, 0, 1, 0);
										::ReleaseMutex (hConnectMutex);										
										if(MT_ERR_NO_ERROR == lErrorCode)
										{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
										}
										else
										{
											pMyTrackCore->RaiseMyTrackError(lErrorCode,

																			data.m_varRequest,
																			data.m_pNotifier,
																			data.m_enRequestType);

											{
												CCriticalSectionWrapper  csWrap(RespCS);
												(*pRespMap)[lReqNum] = data;
											}

										}
										break;
									}
			
			}
		}

	}
	return 0;
}
	
//--------------------------------------------------------------------------------------------------//
UINT __stdcall CMyTrackCore::_MsgThread(LPVOID pArg)
{
	ATLTRACE(_T("_MsgThread thread: %x\n"), GetCurrentThreadId ());
	
	CMyTrackCore *pMyTrackCore = reinterpret_cast<CMyTrackCore*>(pArg);
	if (::IsBadCodePtr ((FARPROC)pMyTrackCore))
		return 2;
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
		return 2;
	
	HANDLE& hStop = pMyTrackCore->GetStopEvent ();
	HANDLE& hOneMsg = pMyTrackCore->GetOneMsgEvent();
		
	CResponceMap* pmapResponse = pMyTrackCore->GetResponceMap();
	CComAutoCriticalSection& respCS = pMyTrackCore->GetResponceMapCS();

	CMsgBuffer	*pBufferQueue =  pMyTrackCore->GetBufferQueue();
	HANDLE		*phEventMutex = pMyTrackCore->GetEventMutex();
	
	if (::IsBadCodePtr ((FARPROC)pBufferQueue) ||
		::IsBadCodePtr ((FARPROC)phEventMutex))
		return 2;

	int iTimeShift = pMyTrackCore->GetTimeShift ();
	CResponceMap::iterator itrFind;
		
	while(true)
	{
		if(::WaitForSingleObject(hStop, 0)==WAIT_OBJECT_0)
			break;
		::WaitForSingleObject (hOneMsg,INFINITE);
		// full lock positions queue
		pBufferQueue->LockPop();
		pBufferQueue->LockPush();
		if (pBufferQueue->PopSize()==0 && pBufferQueue->PushSize()==0)
			::ResetEvent (hOneMsg);
		
		// swap push & pop queues
		pBufferQueue->Swap();
		
		// unlock push queue (so updates can push new positions to queue)
		pBufferQueue->UnlockPush();
		
		// pop all data from incoming positions queue
		while(pBufferQueue->PopSize() > 0)
		{
			// get front data from queue
			const CBufferInfo& data = pBufferQueue->Front();

			int iReqNum = data.m_reqNum;
			bool bEnd = false;
			bool bSendResponce = false;
			CRequestData dataReq;

			respCS.Lock();

			itrFind = pmapResponse->find (data.m_reqNum);
			bEnd = (itrFind!=pmapResponse->end());
			bSendResponce = itrFind->second.m_bSendResponce;
			if(bEnd && bSendResponce)
			{
				dataReq = itrFind->second;
			}
			respCS.Unlock();

			if (bEnd && bSendResponce) 
				pMyTrackCore->ProcessMyTrackMsg(data.m_pMsg, &dataReq);
			else	
				pMyTrackCore->EraseSubscription(&iReqNum);
			// pop top data from queue
			if (data.m_pMsg)
				delete (MYTRACK_MSG*)data.m_pMsg;
			pBufferQueue->Pop();
		}
		// ufnlock pop queue
		pBufferQueue->UnlockPop();
	}	
	::CoUninitialize ();
	return 0;
}
//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::ProcessMyTrackMsg(MYTRACK_MSG *pMsg,CRequestData *pData)
{
	ATLTRACE(_T("ProcessMyTrackMsg(Msg code get): %d\n"), pMsg->messageCode);
	switch(pMsg->messageCode)
	{
		case MSG_ERROR_REPORT:
			{
				RaiseMyTrackError(pMsg->u.errorReport.error,
									pData->m_varRequest,
									pData->m_pNotifier,
									pData->m_enRequestType);
				EraseSubscription(&(pMsg->rqn));
				return;
			}
								
		case MSG_QUOTE_RESPONSE:
			{
				if(MT_ERR_NO_ERROR == pMsg->u.quoteResponse.u.stock.error)
				{
					{
						if (!pData->m_bSendResponce)
						{
							EraseSubscription(&(pMsg->rqn));
							return;
						}
					}	
					if (pData->m_bSendResponce&&pData->m_pNotifier)
					{

#ifdef _SYNC							
						CBaseNotifier::WaitWithEvents (m_hEventMutex);
						CommonQuoteResponce(&(pMsg->u.quoteResponse),pData);
						::ReleaseMutex (m_hEventMutex);						
#else
						CommonQuoteResponce(&(pMsg->u.quoteResponse),pData);
#endif							
					}
					else
						EraseSubscription(&(pMsg->rqn));
				}
				else
					if (pData->m_bSendResponce)
						RaiseMyTrackError(pMsg->u.errorReport.error,
											pData->m_varRequest,
											pData->m_pNotifier,
											pData->m_enRequestType);
				//no more need
				EraseSubscription(&(pMsg->rqn));
				return;
			}

		case MSG_INTRADAY_UPDATE:
			{
				if (MT_ERR_NO_ERROR != pMsg->u.tick.error|| !pData->m_bSendResponce)
				{
					if (MT_ERR_NO_ERROR != pMsg->u.tick.error && pData->m_bSendResponce)
						RaiseMyTrackError(pMsg->u.tick.error,
										pData->m_varRequest,
										pData->m_pNotifier,
										pData->m_enRequestType);
					{
						EraseSubscription(&(pMsg->rqn));
						return;
					}
				}
				
				//we don't use trades for stock and options 
				// we don't use quotes for indexes
				if ((pMsg->u.tick.recordType==1 && pMsg->u.tick.msgType==1) ||
					(pMsg->u.tick.recordType!=1 && pMsg->u.tick.msgType==0) )
					return;


				CComVariant varBuffer;
				eg_lib::vt_date dtDate(eg_lib::vt_date::GetCurrentDate ());
				varBuffer.Clear ();
				
				double denominator = (pMsg->u.tick.m.sale.format < 10)?pow (10, pMsg->u.tick.m.sale.format): 
																		1 << (pMsg->u.tick.m.sale.format - 10);
				
				std::string strExchange("");
				if(pData->m_pNotifier->GetClientType()==CBaseNotifier::enClientPriceProvider)
				{
					//option
					if(2 == pMsg->u.tick.recordType)
					{
						QUOTE_UPDATE &msg =  pMsg->u.tick.m.quote;
						try
						{
							_QuoteUpdateResult qur;
							qur.Init();

							qur->UpdateDateTime = (DATE)dtDate;
							qur->SeqNum = msg.seq;
							qur->BidPrice = SetDoubleValue(msg.bid/denominator);
							qur->AskPrice = SetDoubleValue(msg.ask/denominator);							
							qur->BidSize = BAD_LONG_VALUE;
							qur->AskSize = BAD_LONG_VALUE;
							_bstr_t bstrStub(DEF_CURRENCY);
							qur->Currency = bstrStub.copy ();
							qur->LotSize = DEF_OPT_LOTSIZE;
							qur->LastPrice = BAD_DOUBLE_VALUE;
							qur->OpenInterest = BAD_LONG_VALUE;
							qur->Volume = BAD_LONG_VALUE;
							
							bstrStub = strExchange.c_str();
							qur->Exchange = bstrStub.copy();

							varBuffer.Clear ();
							qur.CopyTo(varBuffer);
							
							if (pData->m_bSendResponce && pData->m_pNotifier)
							{
	#ifdef _SYNC							
								CBaseNotifier::WaitWithEvents (m_hEventMutex);
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
								::ReleaseMutex (m_hEventMutex);
	#else
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
	#endif							
							}
							else
								EraseSubscription(&(pMsg->rqn));
							varBuffer.Clear ();
						}
						catch(_com_error &e)
						{
							varBuffer.Clear ();
							RaiseComError(e,*pData);
						}
					}
					else
					{
						try
						{
							_QuoteUpdateResult qur;

							qur.Init();
							qur->UpdateDateTime = (DATE)dtDate;
							qur->LotSize = DEF_STK_LOTSIZE;
							
							if (pMsg->u.tick.msgType == 0) // trade
							{
								SALE_UPDATE & msg = pMsg->u.tick.m.sale;
								
								qur->SeqNum = msg.seq;
                                qur->BidPrice = BAD_DOUBLE_VALUE;
								qur->AskPrice = BAD_DOUBLE_VALUE;
                                qur->BidSize = BAD_LONG_VALUE;
								qur->AskSize = BAD_LONG_VALUE;
								qur->LastPrice = SetDoubleValue(msg.value / denominator);
							}
							else
							{
								QUOTE_UPDATE &msg =  pMsg->u.tick.m.quote;

								qur->SeqNum = msg.seq;
								qur->BidPrice = SetDoubleValue(msg.bid/denominator);
								qur->AskPrice = SetDoubleValue(msg.ask/denominator);
								qur->BidSize = msg.bidSize;
								qur->AskSize = msg.askSize;
                                qur->LastPrice = BAD_DOUBLE_VALUE;
							}

                            qur->OpenInterest = BAD_LONG_VALUE;
							qur->Volume = BAD_LONG_VALUE;

							_bstr_t bstrStub(DEF_CURRENCY);
							qur->Currency = bstrStub.copy ();
							bstrStub = strExchange.c_str();
							qur->Exchange = bstrStub.copy();
                            
                            varBuffer.Clear ();
							qur.CopyTo(varBuffer);
							
							if (pData->m_bSendResponce && pData->m_pNotifier)
							{
	#ifdef _SYNC							
								CBaseNotifier::WaitWithEvents (m_hEventMutex);
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
								::ReleaseMutex (m_hEventMutex);
	#else
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
	#endif					
								
							}
							else
								EraseSubscription(&(pMsg->rqn));
							
							varBuffer.Clear ();
						}
						catch(_com_error &e)
						{
							varBuffer.Clear ();
							RaiseComError(e,*pData);
						}
					}
					return;
				}

				if(pData->m_pNotifier->GetClientType()==CBaseNotifier::enClientPriceInfo
					|| pData->m_pNotifier->GetClientType()==CBaseNotifier::enClientPriceInfoWithNotify)
				{
					//option
					if(2 == pMsg->u.tick.recordType)
					{
						QUOTE_UPDATE &msg =  pMsg->u.tick.m.quote;
						try
						{
							_QuoteUpdateInfo qur;
							qur.Init();

							qur->UpdateDateTime = (DATE)dtDate;
							qur->SeqNum = msg.seq;
							qur->BidPrice = SetDoubleValue(msg.bid/denominator);
							qur->AskPrice = SetDoubleValue(msg.ask/denominator);							
							qur->BidSize = BAD_LONG_VALUE;
							qur->AskSize = BAD_LONG_VALUE;                            
                            _bstr_t bstrStub(DEF_CURRENCY);
							qur->Currency = bstrStub.copy ();
                            qur->LotSize = DEF_STK_LOTSIZE;
							qur->LastPrice = BAD_DOUBLE_VALUE;
                            qur->OpenInterest = BAD_LONG_VALUE;
							qur->Volume = BAD_LONG_VALUE;
							bstrStub = strExchange.c_str();
							qur->Exchange = bstrStub.copy();
							qur->OpenPrice = BAD_DOUBLE_VALUE;
							qur->ClosePrice = BAD_DOUBLE_VALUE;
							qur->HighPrice = BAD_DOUBLE_VALUE;
							qur->LowPrice = BAD_DOUBLE_VALUE;
							qur->Skipped = 0;
							qur->TotalRequests = 0;
							qur->WaitTime = 0;
                            qur->NetChange = BAD_DOUBLE_VALUE;

							varBuffer.Clear ();
							qur.CopyTo(varBuffer);
							
							if (pData->m_bSendResponce && pData->m_pNotifier)
							{
	#ifdef _SYNC							
								CBaseNotifier::WaitWithEvents (m_hEventMutex);
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
								::ReleaseMutex (m_hEventMutex);
	#else
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
	#endif							
							}
							else
								EraseSubscription(&(pMsg->rqn));
							varBuffer.Clear ();
						}
						catch(_com_error &e)
						{
							varBuffer.Clear ();
							RaiseComError(e,*pData);
						}
					}
					else
					{
						try
						{
							_QuoteUpdateInfo qur;
							qur.Init();

							qur->UpdateDateTime = (DATE)dtDate;

							if (pMsg->u.tick.msgType == 0) // trade
							{
								SALE_UPDATE & msg = pMsg->u.tick.m.sale;
								
								qur->SeqNum = msg.seq;
                                qur->BidPrice = BAD_DOUBLE_VALUE;
								qur->AskPrice = BAD_DOUBLE_VALUE;
								qur->BidSize = BAD_LONG_VALUE;
								qur->AskSize = BAD_LONG_VALUE;
								qur->LastPrice = SetDoubleValue(msg.value / denominator);
							}
							else
							{
								QUOTE_UPDATE &msg =  pMsg->u.tick.m.quote;

								qur->SeqNum = msg.seq;
								qur->BidPrice = SetDoubleValue(msg.bid/denominator);
								qur->AskPrice = SetDoubleValue(msg.ask/denominator);
								qur->BidSize = msg.bidSize;
								qur->AskSize = msg.askSize;
                                qur->LastPrice = BAD_DOUBLE_VALUE;
							}

                            _bstr_t bstrStub(DEF_CURRENCY);
							qur->Currency = bstrStub.copy ();
                            qur->LotSize = DEF_STK_LOTSIZE;
                            qur->OpenInterest = BAD_LONG_VALUE;
                            qur->Volume = BAD_LONG_VALUE;
							bstrStub = strExchange.c_str();
							qur->Exchange = bstrStub.copy();

                            qur->OpenPrice = BAD_DOUBLE_VALUE;
							qur->ClosePrice = BAD_DOUBLE_VALUE;
							qur->HighPrice = BAD_DOUBLE_VALUE;
							qur->LowPrice = BAD_DOUBLE_VALUE;
							qur->Skipped = 0;
							qur->TotalRequests = 0;
							qur->WaitTime = 0;
                            qur->NetChange = BAD_DOUBLE_VALUE;

							varBuffer.Clear ();							
							qur.CopyTo(varBuffer);
							
							if (pData->m_bSendResponce && pData->m_pNotifier)
							{
	#ifdef _SYNC							
								CBaseNotifier::WaitWithEvents (m_hEventMutex);
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
								::ReleaseMutex (m_hEventMutex);
	#else
								pData->m_pNotifier->OnQuoteUpdate (pData->m_varRequest,varBuffer);
	#endif					
								
							}
							else
								EraseSubscription(&(pMsg->rqn));
							
							varBuffer.Clear ();
						}
						catch(_com_error &e)
						{
							varBuffer.Clear ();
							RaiseComError(e,*pData);
						}
					}
					return;
				}

			}
		case MSG_OPTION_DATA:
			{
				OPTION_PACKET &msg = pMsg->u.optionPacket;
				
				if (MT_ERR_NO_ERROR != msg.error || !pData->m_bSendResponce)
				{
					//ATLTRACE(_T("Responce options -> %d\n"),InterlockedIncrement (&m_lRespCount));
					
					if (MT_ERR_NO_ERROR != msg.error && pData->m_bSendResponce)
						RaiseMyTrackError(msg.error,pData->m_varRequest,
										pData->m_pNotifier,pData->m_enRequestType);
					{
						EraseSubscription(&(pMsg->rqn));
						return;	
					}
				}
				
				std::string strSymbol(msg.exchangeRoot, sizeof(msg.exchangeRoot));
				if(strSymbol.find(' ') != strSymbol.npos)
					strSymbol.erase(strSymbol.find(' '));
				strSymbol += std::string(msg.designation, sizeof(msg.designation));

				std::string strExchange(msg.exchange, sizeof(msg.exchange));

				DATE dtDate;
				SYSTEMTIME st;
				::GetSystemTime(&st);
				::SystemTimeToVariantTime(&st, &dtDate);
				dtDate = (long)dtDate + msg.daysToExp + m_tagLoginInfo.m_iTimeShift;
				CComVariant varBuffer;
				CComVariant varIsLast;
				bool bLast = (msg.eomFlag == 2);

				try
				{
					if(pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureProvider)
					{
						_OptionResults or;	
						or.Init();

						_bstr_t bstrStub(strSymbol.c_str ());
						or->Series = bstrStub.copy ();
						bstrStub = strExchange.c_str();
						or->Exchange = bstrStub.copy();
						bstrStub = DEF_CURRENCY;
						or->Currency = bstrStub.copy ();
						or->OptionType = msg.callOrPut == 0 ?enCall:enPut;  
						or->LotSize = DEF_OPT_LOTSIZE;
						or->ExpirationDate = dtDate;
						or->StrikePrice = 0.0f;
						if (!bLast)
							or->StrikePrice = mtuGetPrice(msg.strikePrice,msg.format);
						or.CopyTo (varBuffer);
						varIsLast.Clear ();
						varIsLast =bLast;						
					}
					if(pData->m_pNotifier->GetClientType() == CBaseNotifier::enClientStructureInfo)
					{
						_OptionInfo oi;	
						oi.Init();

						_bstr_t bstrStub(strSymbol.c_str ());
						oi->Series = bstrStub.copy ();
						bstrStub = strExchange.c_str();
						oi->Exchange = bstrStub.copy();
						oi->Currency = g_bstrDefCurrency.copy();
						oi->OptionType = (msg.callOrPut == 0 )? enCall:enPut;  
						oi->LotSize = DEF_OPT_LOTSIZE;
						oi->ExpirationDate = dtDate;
						oi->StrikePrice = 0.0f;
						if (!bLast)
						{
							oi->StrikePrice = mtuGetPrice(msg.strikePrice,msg.format);

							oi->Ask = mtuGetPrice(msg.ask, msg.format);
							oi->Bid = mtuGetPrice(msg.bid, msg.format);
							oi->OpenInterest = msg.open_interest;
							oi->Volume		 = msg.volume;
							oi->YVolume		 = msg.yest_volume;
							oi->Last = mtuGetPrice(msg.last, msg.format);
						}
						else
						{
							oi->Ask = 0;
							oi->Bid = 0;
							oi->OpenInterest = 0;
							oi->Volume		 = 0;
							oi->YVolume		 = 0;
							oi->Last         = 0;  
						}
						varIsLast.Clear ();
						varIsLast = bLast;						
						oi.CopyTo (varBuffer);

					}

					if (pData->m_bSendResponce && pData->m_pNotifier)
					{
#ifdef _SYNC							
						CBaseNotifier::WaitWithEvents (m_hEventMutex);
						pData->m_pNotifier->OnOption (pData->m_varRequest,varBuffer,varIsLast);
						::ReleaseMutex (m_hEventMutex);
#else
						pData->m_pNotifier->OnOption (pData->m_varRequest,varBuffer,varIsLast);
#endif							
					}
					else
						EraseSubscription(&(pMsg->rqn));

					varBuffer.Clear ();
				}
				catch(_com_error &e)
				{
					varBuffer.Clear ();
					RaiseComError(e,*pData);
				}
					
				{
					bool bErase = (msg.eomFlag != 0);
					if(msg.eomFlag == 1)	// more data
					{
						CRequestData rd;
						rd = *pData;
						long lReqNum = GetNewReqNum (); 							
						long lErrorCode = mtRequestOptionData(lReqNum,
															(LPSTR)rd.m_strOriginal.c_str (),
															2, m_IsAllExchanges, 0, 0, 50, 1);
						if(MT_ERR_NO_ERROR == lErrorCode)
						{
							CCriticalSectionWrapper  csWrap(m_csRespMap);
							m_pmapResponse->insert (CResponceMap::value_type(lReqNum,rd));		
						}
						else
							ATLTRACE(_T("Erorr mtRequestOptionData\n"));
					}	
					if (bErase)
						EraseSubscription(&(pMsg->rqn));
				}
				return;
			}
			
		case MSG_BACKGROUND_RESPONSE:
			{
			
				if (MT_ERR_NO_ERROR != pMsg->u.backgroundResponse.error || !pData->m_bSendResponce)
				{
					if (MT_ERR_NO_ERROR != pMsg->u.backgroundResponse.error && pData->m_bSendResponce)
						RaiseMyTrackError(pMsg->u.backgroundResponse.error,
											pData->m_varRequest,
											pData->m_pNotifier,
											pData->m_enRequestType);
					{
						EraseSubscription(&(pMsg->rqn));
						return;
					}
				}
				
				BACKGROUND_RESPONSE &msg = pMsg->u.backgroundResponse;
				{
					if (!pData->m_bSendResponce || pData->m_pNotifier==NULL)
					{
						EraseSubscription(&(pMsg->rqn));
						return;
					}
					
					CRequestData rd;
					long lReqNum = GetNewReqNum (); 							
					rd = *pData;
					rd.m_strDescription = msg.description;  
					long lErrorCode = mtRequestQuoteData(lReqNum, (LPSTR)rd.m_strSymbol.c_str () , "");
					if(MT_ERR_NO_ERROR == lErrorCode)
					{
						CCriticalSectionWrapper  csWrap(m_csRespMap);
						(*m_pmapResponse)[lReqNum] = rd;	
					}
					else
						RaiseMyTrackError(lErrorCode,
											pData->m_varRequest,
											pData->m_pNotifier,
											pData->m_enRequestType);
					EraseSubscription(&(pMsg->rqn));
				}
				return;
			}
	}
}

//--------------------------------------------------------------------------------------------------//
void CMyTrackCore::ProcessMessages()
{
	MSG msg;
	while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

		
DWORD CMyTrackCore::MsgWait(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds, BOOL bMode)
{
	DWORD dwStatus = -1;
	ATLASSERT(nCount > 0);
	for(;;)
	{
		dwStatus = ::MsgWaitForMultipleObjects(nCount,pHandles,bMode,dwMilliseconds,QS_ALLEVENTS);
		
		if(dwStatus != WAIT_OBJECT_0+nCount) break;
		
		MSG msg;
		while(PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return dwStatus;
}

void CMyTrackCore::CheckLostRequests()
{
	HANDLE hEvents[] = 
	{
		m_hStopEvent,
		m_hCheckMessages,
		m_hResubscribeAll
	};
	 
	while(1)
	{
		DWORD dw = MsgWait(3,hEvents, TIMEOUT_LOSTMESSAGE*1000);
		if(dw == WAIT_OBJECT_0) break;
		if(!m_bIsConnected) continue;
		if(dw==WAIT_OBJECT_0+2)
		{
			ATLTRACE("Lost Message Processing: Resubscribe all\n");
			ResubscribeAll();
			continue;
		}

		if (m_pmapResponse->size ()==0) continue;
		ATLTRACE("Lost Message Processing: Started\n");

		CResponceMap mapStub;
		{
			CResponceMap mapStubSave;
			vt_date dtCurrent = vt_date::GetCurrentDate();

			CCriticalSectionWrapper  csWrap(m_csRespMap);
			CResponceMap::iterator itrFind = m_pmapResponse->begin();

			while(itrFind != m_pmapResponse->end())
			{
				CRequestData &tagData = itrFind->second;
				if(WaitForSingleObject(m_hStopEvent,0)==WAIT_OBJECT_0) return;

				if(!tagData.CheckOperationTime(dtCurrent))
					mapStub.insert (CResponceMap::value_type(itrFind->first,itrFind->second));
				else
					mapStubSave.insert (CResponceMap::value_type(itrFind->first,itrFind->second));

				itrFind++;
			}
			m_pmapResponse->clear();

			for(itrFind = mapStubSave.begin(); itrFind != mapStubSave.end(); itrFind++)
			{
				m_pmapResponse->insert (CResponceMap::value_type(itrFind->first,itrFind->second));
			}
		}
		
		if(mapStub.size()==0) continue;
		if(WaitForSingleObject(m_hStopEvent,0)==WAIT_OBJECT_0) return;

		ATLTRACE("Size LostMessages buffer: %d\n",mapStub.size());

		CResponceMap::iterator itrFind = mapStub.begin();
		for (;itrFind != mapStub.end();itrFind++)
		{
			CRequestData tagData = itrFind->second;
			if(WaitForSingleObject(m_hStopEvent,0)==WAIT_OBJECT_0) return;

			WaitQueue ();

			switch(tagData.m_enRequestType) 
			{
			case enRequestLastQuote:
				{
					long lReqNum = GetNewReqNum();
					long lErrorCode = mtRequestQuoteData(lReqNum, 
						(LPSTR)tagData.m_strSymbol.c_str(), 
						(LPSTR)tagData.m_strOption.c_str());
					{
						CCriticalSectionWrapper  csWrap(m_csRespMap);
						(*m_pmapResponse)[lReqNum] = tagData;
					}

					break;
				}
			case enSubscribeQuote:
				{
					if(tagData.m_pNotifier->GetClientType() == enPriceInfoWithNotify)
					{
						long lReqNum = GetNewReqNum();
						long lErrorCode = mtRequestQuoteData(lReqNum, 
														(LPSTR)tagData.m_strSymbol.c_str(), 
														(LPSTR)tagData.m_strOption.c_str());
						{
							CCriticalSectionWrapper  csWrap(m_csRespMap);
							(*m_pmapResponse)[lReqNum] = tagData;
						}
					}
					break;
				}
			case enRequestStock:
				{
					long lReqNum = GetNewReqNum();
					long lErrorCode = mtRequestBackgroundData(lReqNum, (LPSTR)tagData.m_strSymbol.c_str(), "");
					{
						CCriticalSectionWrapper  csWrap(m_csRespMap);
						(*m_pmapResponse)[lReqNum] = tagData;
					}
					break;
				}
			case enRequestOptions:
				{
					long lReqNum = GetNewReqNum();
					long lErrorCode = mtRequestOptionData(lReqNum,(LPSTR)tagData.m_strOriginal.c_str (), 2, m_IsAllExchanges, 0, 0, 1, 0);
					{
						CCriticalSectionWrapper  csWrap(m_csRespMap);
						(*m_pmapResponse)[lReqNum] = tagData;
					}
					break;
				}
			}	
		}

	}
}

UINT __stdcall CMyTrackCore::_CheckLostRequestsThread(LPVOID pArg)
{
	CMyTrackCore *pThis = reinterpret_cast<CMyTrackCore*>(pArg);

	if (::IsBadCodePtr ((FARPROC)pThis) || !pThis)	return 2;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

	if (FAILED(hr))
		return 2;
	pThis->CheckLostRequests();
	::CoUninitialize ();
	return 0;
}

PriceProviderTypeEnum CMyTrackCore::GetPriceProviderMode()
{
	return m_enPPMode;
}

void CMyTrackCore::SetPriceProviderMode( PriceProviderTypeEnum type)
{
	if (type == enPPDefault)
		m_IsAllExchanges = 0;

	if (type == enPPAll)
		m_IsAllExchanges = 1;

	m_enPPMode = type;
}

