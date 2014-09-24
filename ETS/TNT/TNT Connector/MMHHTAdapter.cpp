// MMHHTAdapter.cpp : Implementation of CMMHHTAdapter

#include "stdafx.h"
#include "MMHHTAdapter.h"
#include "WaitWithEventFunction.h"

#define MESSAGE_HEADER_SIZE (int)28
#define MESSAGE_LOGON_OF_SIZE (int)10
#define MESSAGE_TRADE_ADD_SIZE (int)100

// CMMHHTAdapter

CMMHHTAdapter::CMMHHTAdapter():
		m_hListenerThread(NULL),
		m_hSenderThread(NULL),
		m_MMHHISocket(NULL),
		//m_PrevoiusMMHHISocket(NULL),
		m_LastSequenceNo(0),
		m_lLoggingStatus(0),
		m_lLogOnSequenceNumber(0),
		m_lLogOffSequenceNumber(0),
		m_bTCPDriverInitialized(false)

{
	m_hEventStop = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hTCPConnectedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ValidIncomingCommands.push_back( new CTNTMessage);
	m_ValidIncomingCommands.push_back( new CAcknowlegementMessage);
	m_ValidIncomingCommands.push_back( new CTerminateConnectionMessage);


	DWORD dwErr =m_TcpDriver.Init();
	if(dwErr == 0)
	{
//		dwErr = m_TcpDriver.Start(this, COINIT_MULTITHREADED);
//		if(dwErr == 0)
			m_bTCPDriverInitialized = true;
	}

}

bool CMMHHTAdapter::GetErrorDescription(DWORD dwErr,_bstr_t *pbsErrDesc)
{
	*pbsErrDesc = L"";
	LPVOID lpMsgBuf = NULL;
	if (!::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL ))
	{
		// Handle the error.
		return false;
	}
	*pbsErrDesc = (LPCTSTR)lpMsgBuf;
	LocalFree( lpMsgBuf );
	return true;


}

CMMHHTAdapter::~CMMHHTAdapter()
{
	for(COMMANDS_TYPE::iterator index = m_ValidIncomingCommands.begin(); index != m_ValidIncomingCommands.end(); index++)
	{
		CMMHHMessage* pitem= *index;
		delete pitem;
	}
	m_ValidIncomingCommands.clear();
	if(m_bTCPDriverInitialized)
	{
//		m_TcpDriver.Stop();
		m_TcpDriver.Exit();
	}
}

HRESULT CMMHHTAdapter::FinalConstruct()
{
	if(m_bTCPDriverInitialized)
		return S_OK;
	return Error(_T("Failed to initialize TCP\\IP driver."));
}

void CMMHHTAdapter::FinalRelease() 
{
	Logoff();
}

bool CMMHHTAdapter::IsStopped()
{
	return (WAIT_TIMEOUT!=WaitForSingleObject(m_hEventStop,0));
}

CMMHHTAdapter::enLoggingStatus CMMHHTAdapter:: GetLoggingStatus()
{
	return (enLoggingStatus)m_lLoggingStatus;
}

void CMMHHTAdapter::SetLoggingStatus(enLoggingStatus bLogOn)
{
	if(GetLoggingStatus() == bLogOn)
		return;
	InterlockedExchange(&m_lLoggingStatus, (LONG)bLogOn);
}


HRESULT CMMHHTAdapter::LaunchWorkingThreads()
{
	HRESULT hRet = S_OK;
	try
	{	
		if((m_hListenerThread)&& (WaitForSingleObject( m_hListenerThread, 0L ) == WAIT_TIMEOUT))
			return S_OK;

		unsigned uListenerThreadID = 0;
		unsigned uSenderThreadID = 0;
	
		SetEvent(m_hEventStop);

		_bstr_t bsMessage;
		DWORD dwConnect = InitializeAndConnectDriver(&bsMessage);

		if(dwConnect!=0)
		{
			RaiseErrorMessage(enLogonResponse, 0,E_FAIL, _bstr_t(L"Failed to initialize and connect TCP\\IP driver: ") + bsMessage);
			return E_FAIL;
			//return Error(_T("Initialize and connect TCP\\IP driver."));
		}
		
		ResetEvent(m_hEventStop);
		m_hSenderThread  = (HANDLE)_beginthreadex( NULL, 0, _SenderThread, (void*)this, 0, &uSenderThreadID );
		if(m_hSenderThread == INVALID_HANDLE_VALUE)
		{
			m_hListenerThread = NULL;
			RaiseErrorMessage(enLogonResponse ,0,E_FAIL, L"Failed to start sender thread.");
			return E_FAIL;
			//return Error(_T("Failed to start sender thread"), __uuidof(IMMHHTAdapter),E_FAIL);
		}

		m_hListenerThread = (HANDLE)_beginthreadex( NULL, 0, _ListenerThread, (void*)this, 0, &uListenerThreadID );
		if(m_hListenerThread == INVALID_HANDLE_VALUE)
		{
			m_hListenerThread = NULL;
			RaiseErrorMessage(enLogonResponse, 0,E_FAIL, L"Failed to start listener thread.");
			return E_FAIL;
		}

	}catch( const _com_error& e )
	{
		RaiseErrorMessage(enLogonResponse, 0,e.Error(), CComErrorWrapper::ErrorDescription( e ));
		return e.Error();
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), __uuidof(IMMHHTAdapter), e.Error() );
	}catch(...)
	{
		RaiseErrorMessage(enLogonResponse, 0,E_FAIL, L"Unknown exception thrown in CMMHHTAdapter::LaunchWorkingThreads().");
		return E_FAIL;
		//return Error( _T("Unknown exception thrown in CMMHHTAdapter::LaunchWorkingThreads()."), __uuidof(IMMHHTAdapter), E_FAIL);
	}
	return hRet;
}

void CMMHHTAdapter::RaiseErrorMessage(enErrmessageResponseType ResponseType, long lSeqNumber, HRESULT sCode, BSTR Description)
{
	try
	{
		CComRecPtr<ACKData> dataRec;
		if(ResponseType == enTradeAddResponse)
		{
			
			dataRec.Init();
			dataRec->SequenceNo = lSeqNumber;
			dataRec->POEID = SysAllocString(L"");
		}

		CComRecPtr<ErrorDescription> errDesc;
		errDesc.Init();
		if(FAILED(sCode))
			errDesc->ErrCode  =enMmhtErUnknownError;
		else
			errDesc->ErrCode  =enMmhtErNoError;

		errDesc->Description  =SysAllocString(Description);

		SAFEARRAYBOUND sab;
		sab.cElements = 1;
		sab.lLbound = 0;

		LPSAFEARRAY  pArray = ::SafeArrayCreateEx(VT_RECORD,1,&sab,errDesc.GetRecordInfo());
		if(pArray)
		{
			::SafeArrayLock(pArray);
			LPVOID pData = NULL;
			HRESULT hr = ::SafeArrayAccessData(pArray, &pData);
			if(SUCCEEDED(hr))
			{
				ErrorDescription* pDesc = reinterpret_cast<ErrorDescription*>(pData);
				pDesc->Description = SysAllocString(errDesc->Description);
				pDesc->ErrCode = ( (errDesc->ErrCode == ' ') ? enMmhtErNoError : errDesc->ErrCode);
				hr = ::SafeArrayUnaccessData(pArray);
				if(SUCCEEDED(hr))
				{
					switch(ResponseType)
					{
					
						case enTradeAddResponse: raise__TradeAddResponse(dataRec, sCode, pArray);
							break;

						case enLogoffResponse: raise__LogoffResponse(sCode, pArray);
							break;

						case enLogonResponse: raise__LogonResponse(sCode, pArray);
							break;
					}
				}
			}
			::SafeArrayUnlock(pArray);
			::SafeArrayDestroy(pArray);
		}		 
	}catch(...)
	{
		ATLASSERT(0);
	}
}

DWORD CMMHHTAdapter::InitializeAndConnectDriver(_bstr_t* pbMess)
{
	HRESULT hError = E_FAIL;
	DWORD dwErr = 0;
	try
	{
		/*
		if (m_spSettings==NULL)
				{
					HRESULT hr = m_spSettings.CreateInstance(__uuidof(Settings::CMMHHTSettings));
					if(FAILED(hr))
					{
						*pbMess = _T("Unable to create Settings Object.");
		
						RaiseErrorMessage(0, hr, bstrErrorDescr);
						return hr;
					}
				}*/
		

		dwErr =m_TcpDriver.Start(this, COINIT_MULTITHREADED);
		if(dwErr != 0)
		{	
			_bstr_t bsError;
			if(GetErrorDescription(dwErr, &bsError))
			{
				*pbMess  = "TcpDriver initialization error: ";
				*pbMess  +=bsError;
			}else
			{
				*pbMess  = "TcpDriver initialization error.";
			}
			hError = E_FAIL;
		}

/*
		if(dwErr == 0)
			dwErr = m_TcpDriver.Start(this, COINIT_MULTITHREADED);
		if (dwErr != 0)
			*pbMess  = _T("TcpDriver starting error.");


		if (dwErr != 0)
		{
			RaiseErrorMessage(enLogonResponse, m_LastSequenceNo, hError, *pbMess );
			return dwErr;
		}
*/


	
		dwErr =m_TcpDriver.Connect(CW2A(m_Settings.GetPrimaryIP()), m_Settings.GetPort());
		
	
		if( dwErr == 0)
		{
			return 0;
		}

		_bstr_t bsErr1;
		if(GetErrorDescription(dwErr,&bsErr1) )
		{
			bsErr1 =  L"Failed to connect to primary IP: " + bsErr1;
		}else
		{
			bsErr1 = L"Failed to connect to primary IP.";
		}
		
		dwErr = m_TcpDriver.Connect(CW2T(m_Settings.GetSecondaryIP()), m_Settings.GetPort());
		if(dwErr == 0)
		{
			*pbMess = bsErr1 + " Connected to secondary MMHHTT server.";
			raise__ConnectorError( S_OK, *pbMess);
			dwErr = S_OK;
		}else
		{
			_bstr_t bsErr2;
			if( GetErrorDescription(dwErr,&bsErr2) )
			{
				bsErr2 =  L"Failed to connect to secondary IP: " + bsErr2;
			}else
			{
				bsErr2 =  L"Failed to connect to secondary IP.";
			}
			m_TcpDriver.Stop();
			*pbMess = bsErr1 + L" " + bsErr2;//"Failed to connect both to primary and secondary MMHHTT servers.";
			dwErr = E_FAIL;
		}
	//	RaiseErrorMessage(enLogonResponse, 0, hError, bstrErrorDescr);
		

	}catch( const _com_error& e )
	{
		*pbMess = (PTCHAR)CComErrorWrapper::ErrorDescription( e );
	}catch(DWORD /*dRet*/)
	{


	}
	catch(...)
	{
		*pbMess = _T("Unknown exception thrown during TCP\\IP driver initialisation).");
	}

	return dwErr;


}


DWORD CMMHHTAdapter::PostLogOnMessage()
{
	try
	{
		CLogOnData login;
		//login.GetHeader().SetID((LPCSTR)m_spSettings->UserID); //NO ID

		::InterlockedIncrement(&m_LastSequenceNo);
		login.SetSequenceNumber(m_LastSequenceNo);
		login.SetPassword(CW2T(m_Settings.GetPassword()));
		m_lLogOnSequenceNumber = m_LastSequenceNo;
		PostMessage(login.GetNewCopy());
	}catch(...)
	{
		return 1;
	}
	return 0;
}

DWORD CMMHHTAdapter::PostLogOffMessage(_bstr_t *pbsErrMess)
{
	try
	{
		*pbsErrMess = L"";
		CLogOffData login;
		InterlockedIncrement(&m_LastSequenceNo);
		login.SetSequenceNumber(m_LastSequenceNo);
		m_lLogOffSequenceNumber = m_LastSequenceNo;

		login.GetHeader().SetBodySize(login.GetBodyLength());
//		login.GetHeader().SetID((LPCSTR)m_spSettings->UserID); //NO ID
		login.SetPassword(CW2T(m_Settings.GetPassword()));

		login.GetHeader().SetCurrentTime();

		DWORD dwResult =m_TcpDriver.Send(m_MMHHISocket,(LPVOID)login.GetBuffer(),login.GetBufferLength());
		if(WSABASEERR<dwResult)
		{
			_bstr_t bsErrMess;			_bstr_t bsErrDesc;
			if(GetErrorDescription(dwResult, &bsErrDesc))
				*pbsErrMess = L"Unable to send Logoff Message: " + bsErrDesc;
			else
				*pbsErrMess  = L"Unable to send Logoff Message.";

			return 1;
		}

	}catch(...)
	{
		return 1;
	}
	return 0;
}

DWORD CMMHHTAdapter::PostTradeAddMessage(CTNTMessage& Data, _bstr_t *pbsErrMess)
{
	try
	{
		*pbsErrMess = L"";
//		Data.GetHeader().SetSource('N');
//		Data.GetHeader().SetMsgType('A');

		Data.SetSequenceNumber(++m_LastSequenceNo);
//		Data.GetHeader().SetBodySize(TNT.GetBodyLength());
		Data.GetHeader().SetCurrentTime();
		DWORD dwResult = m_TcpDriver.Send(m_MMHHISocket,(LPVOID)Data.GetBuffer(),Data.GetBufferLength());

		if(WSABASEERR<dwResult)
		{
			_bstr_t bsErrDesc;
			if(GetErrorDescription(dwResult, &bsErrDesc))
				*pbsErrMess = L"Unable to send Trade Add Message: " + bsErrDesc;
			else
				*pbsErrMess = L"Unable to send Trade Add Message.";

//				RaiseErrorMessage(enLogoffResponse, 0,E_FAIL,bsErrMess);

//			CString strError;
//			strError.Format(_T("Unable to send Trade Message. Error code: %d"), dwResult);
//			RaiseErrorMessage(enTradeAddResponse, m_LastSequenceNo,E_FAIL,_bstr_t(strError));
			return 1;
		}
	}catch(...)
	{
		return 1;
	}
	return 0;
}
bool CMMHHTAdapter::WaitUntilTCPConnected()
{
	HANDLE	hHandles[] = {
		m_hTCPConnectedEvent,
		m_hEventStop			
	};

	while(1)
	{
		DWORD dw = WWE::WaitWithEvents(2,hHandles);
		if(dw!=WAIT_OBJECT_0)
			return false;
		else
			return true;
	}
	return true;
}

UINT CMMHHTAdapter::_SenderThread(LPVOID pData)
{
	CMMHHTAdapter* pThis = (CMMHHTAdapter*)pData;
	if(NULL == pThis)
		return 1;

	if(pThis->IsStopped())
		return 0;

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	_bstr_t bstrErrorDescr;
	HRESULT hError = E_FAIL;	
	try
	{
		if(pThis->WaitUntilTCPConnected())
		{
			HANDLE	hHandles[] = {
				pThis->m_queOutgoing.GetNotificationEvent(),
				pThis->m_hEventStop			
			};

			while(1)
			{
				DWORD dw = WWE::WaitWithEvents(2,hHandles);
				if(dw!=WAIT_OBJECT_0)
					break;
				CMMHHMessage* pMessage = NULL;
				while(pThis->m_queOutgoing.Pop(pMessage))
				{
					if(!pThis->IsStopped())
						pThis->SendMessage(pMessage);
					else
						delete pMessage;
				}
			}
		}

		
		pThis->SetLoggingStatus( enLoggedOff );
		pThis->m_TcpDriver.Stop();
	//	pThis->m_TcpDriver.Exit();
		pThis->m_TcpDriver.Close( pThis->m_MMHHISocket );
		pThis->m_lLogOffSequenceNumber =  0;
		pThis->m_lLogOnSequenceNumber = 0;	
		pThis->m_LastSequenceNo = 0;
//		pThis->m_PrevoiusMMHHISocket = pThis->m_MMHHISocket;
		pThis->m_MMHHISocket = 0;
	}
	catch( const _com_error& e )
	{
		bstrErrorDescr = (PTCHAR)CComErrorWrapper::ErrorDescription( e );
	}
	catch(...)
	{
		bstrErrorDescr = _T("Unknown exception thrown inside main thread.");
	}

	if(bstrErrorDescr.length() != 0)
		pThis->raise__ConnectorError(S_FALSE, bstrErrorDescr);
	::CoUninitialize();
	return 0;
}
UINT CMMHHTAdapter::_ListenerThread(LPVOID pData)
{
	CMMHHTAdapter* pThis = (CMMHHTAdapter*)pData;
	if(NULL == pThis)
		return 1;

	if(pThis->IsStopped())
		return 0;

	::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	HRESULT hError = E_FAIL;	
	bstr_t bstrErrorDescr;
	try
	{
		HANDLE	hHandles[] = {	pThis->m_hEventStop, 
			pThis->m_queIncoming.GetNotificationEvent()};
		bool bExit = false;
		do
		{
			switch(WWE::WaitWithEvents(2,hHandles))
			{
				case WAIT_OBJECT_0:		// Exit
					bExit = true;
					break;

				case WAIT_OBJECT_0 + 1:	// OnMessage
					{
						CMMHHMessage* pItem = NULL;
						while(pThis->m_queIncoming.Pop(pItem) == TRUE)
						{
							if(!pThis->IsStopped())
								pThis->OnMessage(pItem);
							delete pItem;
						}
					}
					break;
				default:
					bstrErrorDescr = _T("Internal error inside main thread.");
					bExit =true;
					break;
			}
		}while(!bExit);

	}
	catch( const _com_error& e )
	{
		bstrErrorDescr = (PTCHAR)CComErrorWrapper::ErrorDescription( e );
	}
	catch(...)
	{
		bstrErrorDescr = _T("Unknown exception thrown inside main thread.");
	}

	if(bstrErrorDescr.length() != 0)
		pThis->raise__ConnectorError(S_FALSE, bstrErrorDescr);

	CoUninitialize();
	return 0;


}

void CMMHHTAdapter::SendMessage(CMMHHMessage* pMessage)
{
	ATLASSERT(pMessage);
	_bstr_t bstrErrorDescr;
	try
	{
#ifdef _DEBUG
		_bstr_t bsTradeID = pMessage->GetTradeID();
		long lSeq = pMessage->GetSequenceNumber();
#endif
		CLogOnData* pLogOn =  dynamic_cast<CLogOnData*>(pMessage);
		if(pLogOn)
		{
			// Send LoginData;
			pLogOn->GetHeader().SetCurrentTime();
			DWORD dwResult = m_TcpDriver.Send(m_MMHHISocket,(LPVOID)pLogOn->GetBuffer(),pLogOn->GetBufferLength());
			if(WSABASEERR<dwResult)
			{
				CString strError;
				strError.Format(_T("Unable to send Logon Message. Error code: %d"), dwResult);
				bstrErrorDescr = strError;				
			}
		}

		CLogOffData* pLogOff =  dynamic_cast<CLogOffData*>(pMessage);
		if(pLogOff)
		{
			// Send LoginData;
			pLogOff->GetHeader().SetCurrentTime();
			DWORD dwResult = m_TcpDriver.Send(m_MMHHISocket,(LPVOID)pLogOff->GetBuffer(),pLogOff->GetBufferLength());
			if(WSABASEERR<dwResult)
			{
				CString strError;
				strError.Format(_T("Unable to send Logoff Message. Error code: %d"), dwResult);
				bstrErrorDescr = strError;				
			}
			SetEvent(m_hEventStop);
		}
		CTNTMessage* pTNTMessage = dynamic_cast<CTNTMessage*>(pMessage);
		if(pTNTMessage)
		{
			pTNTMessage->GetHeader().SetCurrentTime();
			DWORD dwResult = m_TcpDriver.Send(m_MMHHISocket,(LPVOID)pTNTMessage->GetBuffer(),pTNTMessage->GetBufferLength());
			if(WSABASEERR<dwResult)
			{
				CString strError;
				strError.Format(_T("Unable to send Trade Message. Error code: %d"), dwResult);
				bstrErrorDescr = strError;				
			}
		}
		delete pMessage;

	}catch( const _com_error& e )
	{
		bstrErrorDescr = (PTCHAR)CComErrorWrapper::ErrorDescription( e );
	}
	catch(...)
	{
		bstrErrorDescr = _T("Unknown exception thrown during message processing.");
	}

	if(bstrErrorDescr.length() != 0)
		raise__ConnectorError(S_FALSE, bstrErrorDescr);

}
void CMMHHTAdapter::PostMessage(CMMHHMessage* pMessage)
{
	ATLASSERT(pMessage);
	if(pMessage)
	{
		m_queOutgoing.Push(pMessage);
	}
}

bool CMMHHTAdapter::OnAcknowlegement(CAcknowlegementMessage* pData)
{
	long lSentSeq, lExpectSeq;
	bool bIsSequenceViolated = pData->CheckSequenceViolation(lSentSeq, lExpectSeq);
	if(bIsSequenceViolated)
	{
		lExpectSeq--;
		InterlockedExchange(&m_LastSequenceNo,lExpectSeq);
	}

	if( (GetLoggingStatus() != enLoggedOn) && bIsSequenceViolated)
		PostLogOnMessage();
	else
	{
		SAFEARRAY* sa;
		HRESULT hr    = pData->GetErrorDescriptions(&sa);
		bool bIsError = pData->CheckError(NULL);
		if(lSentSeq == m_lLogOnSequenceNumber)
		{
			m_lLogOnSequenceNumber = 0;
			if(!bIsError)
				SetLoggingStatus(enLoggedOn);
			raise__LogonResponse(bIsError?E_FAIL:S_OK,sa);

		} else
			if(lSentSeq == m_lLogOffSequenceNumber)
			{
				if(!bIsError)
				{
					SetLoggingStatus(enLoggedOff);
					SetEvent(m_hEventStop);
				}
				raise__LogoffResponse(bIsError?E_FAIL:S_OK,sa);
			}
			else
			{
				CComRecPtr<ACKData> dataRec;
				dataRec.Init();

				dataRec->SequenceNo=pData->GetOriginalSequenceNumber();
				dataRec->POEID = pData->GetPOEID().copy();
				raise__TradeAddResponse(dataRec, bIsError?E_FAIL:S_OK, sa);
			}

			::SafeArrayDestroy(sa);
	}
	return true;
}

bool CMMHHTAdapter::OnTNTData(CTNTMessage* pData)
{
	try
	{
		CComRecPtr<TNTData> dataRec;
		dataRec.Init();
		if(pData->GetMessage(dataRec))
			raise__TradeNotification(dataRec);
		else
		{
			_bstr_t err(L"Invalid TNT message ");
			err+=_ltow(pData->GetHeader().GetSequenceNumber(),err.GetBSTR(),10);
			err+=L" ";
			err +=pData->GetErrorDescription();
			raise__ConnectorError(S_OK, err);
	//		RaiseErrorMessage(enTradeAddResponse, pData->GetHeader().GetSequenceNumber(), E_FAIL, pData->GetErrorDescription());
		}
	}catch(...)
	{

	}

	return true;
}

bool CMMHHTAdapter::OnTerminateConnection()
{
	__raise _IMMHHTAdapterEvents_TerminateConnection();
	//SetLoggedOn(false);
	SetEvent(m_hEventStop);
	return true;
}


void CMMHHTAdapter::OnMessage(CMMHHMessage* pMessage)
{
	ATLASSERT(pMessage);
	_bstr_t bstrErrorDescr;
	try
	{
		pMessage->Execute(this);

	}catch( const _com_error& e )
	{
		bstrErrorDescr = (PTCHAR)CComErrorWrapper::ErrorDescription( e );
	}
	catch(...)
	{
		bstrErrorDescr = _T("Unknown exception thrown during message processing.");
	}

	if(bstrErrorDescr.length() != 0)
		raise__ConnectorError(S_OK,bstrErrorDescr);
		//RaiseErrorMessage(enTradeAddResponse,0,E_FAIL, bstrErrorDescr);


}
/////////////////////////////////////////////////////////////////////////////////////////////
// IEgTcpDriverEvents implementation

BOOL CMMHHTAdapter::OnConnect(const CONNECTION_ID& Connection, BOOL bIncoming)
{
	if(!bIncoming)
		m_MMHHISocket = Connection.m_Socket;

	SetEvent(m_hTCPConnectedEvent);

	return TRUE;
}

void CMMHHTAdapter::OnConnectionFailed(const CONNECTION_ID& Connection)
{
//	RaiseErrorMessage(enLogonResponse, 0,E_FAIL,_bstr_t(L"Connection to server failed"));
	

	if(!IsStopped())
	{// Threads already started.

//		if(m_PrevoiusMMHHISocket&& m_PrevoiusMMHHISocket==Connection.m_Socket)
//			return;

		raise__ConnectorError(E_FAIL,_bstr_t(L"Can't establish connection."));
		SetLoggingStatus( enLoggedOff );
		m_lLogOffSequenceNumber =  0;
		m_lLogOnSequenceNumber = 0;	
		SetEvent(m_hEventStop);

//		__raise _IMMHHTAdapterEvents_TerminateConnection();
	}
}

void CMMHHTAdapter::OnClose(const CONNECTION_ID& Connection, const DWORD & dwError)
{
	ResetEvent(m_hTCPConnectedEvent);
	SetEvent(m_hEventStop);
	raise__ConnectorError(E_FAIL,_bstr_t(L"Connection to server closed."));
//	__raise _IMMHHTAdapterEvents_TerminateConnection();
}

void CMMHHTAdapter::OnReceive(	const CONNECTION_ID& Connection, const LPVOID pData, const DWORD dwDataLen)
{
	if(m_MMHHISocket != Connection.m_Socket)
		return;

	m_InputStream.Put(pData, dwDataLen);
	if(m_InputStream.GetLength() < MESSAGE_HEADER_SIZE)
		return; //too small even for message header;

	bool bLoaded = false;
	do
	{
		bLoaded = false;
		for(COMMANDS_TYPE::iterator index = m_ValidIncomingCommands.begin(); index != m_ValidIncomingCommands.end(); index++)
		{
			CMMHHMessage* pitem= *index;
			if(pitem->Parse(m_InputStream) == ERROR_SUCCESS)
			{
				m_queIncoming.Push(pitem->GetNewCopy());
				bLoaded = true;
				break;
			}
		}
		
	}while(bLoaded);

}
// End of IEgTcpDriverEvents implementation
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////
// IMMHHTAdapter implementation
STDMETHODIMP CMMHHTAdapter::Logon(ConnectionSettings* pSettings)
{
	if(pSettings == NULL)
		return E_POINTER;
	ObjectLock lock(this);
	try
	{
		if((GetLoggingStatus() != enLoggedOff) || (m_lLogOnSequenceNumber != 0))
		{
			//RaiseErrorMessage(enLogonResponse, 0, E_FAIL, L"Connecting or already connected.");
			return Error(L"Connecting or already connected.", __uuidof(IMMHHTAdapter), E_PENDING );
		}

		_bstr_t bsCSError;
		if(!m_Settings.CreateFrom(pSettings, &bsCSError))
		{
//			RaiseErrorMessage(enLogonResponse, 0, E_FAIL, bsCSError);
//			return S_OK;
			return Error((BSTR)bsCSError, __uuidof(IMMHHTAdapter), E_INVALIDARG );
		}

		HRESULT hRet = LaunchWorkingThreads();
		if(SUCCEEDED(hRet))
			PostLogOnMessage();
	}
	catch(_com_error& e)
	{
		//RaiseErrorMessage(enLogonResponse, 0, e.Error(), CComErrorWrapper::ErrorDescription( e ));
		return Error((LPCTSTR)CComErrorWrapper::ErrorDescription( e ), __uuidof(IMMHHTAdapter),  e.Error());
	}catch(...)
	{
		return Error(L"Unknown exception thrown.", __uuidof(IMMHHTAdapter),  E_FAIL);
		//RaiseErrorMessage(enLogonResponse, 0, E_FAIL, L"Unknown exception thrown in CMMHHTAdapter::Logon(...).");
	}
	return S_OK;
}

STDMETHODIMP CMMHHTAdapter::Logoff()
{
	ObjectLock lock(this);
	if(GetLoggingStatus() == enLoggedOff)
	{
		//RaiseErrorMessage(enLogoffResponse, 0,S_OK,L"Logged off.");
		//return S_OK;
		return Error(L"Logged off.", __uuidof(IMMHHTAdapter), E_PENDING );
	}

	if(m_lLogOffSequenceNumber != 0)
	{
		return S_OK;
	}

	if(!IsStopped())
	{
//		SetEvent(m_hEventStop);
		_bstr_t bsErr;
		if(PostLogOffMessage(&bsErr) != 0)
			return Error(CW2T(bsErr.GetBSTR()));
	}
	return S_OK;
}

STDMETHODIMP CMMHHTAdapter::TradeAdd(TNTData* pData,/* BSTR*/MessageDescriptor* pDescriptor)
{
	ObjectLock lock(this);

	if(!pDescriptor || !pData )
		return E_POINTER;
	HRESULT hr = S_OK;
	try
	{
		bool bError = false;
		_bstr_t bsError;
		if(GetLoggingStatus() != enLoggedOn)
		{
			bsError = L"Logged off.";
			bError = true;
		}
		if(m_lLogOffSequenceNumber != 0)
		{
			bsError = L"Logging off.";
			bError = true;
		}
			
		if(!bError)
		{		
			CTNTMessage tntMessage(true); //source == 'N'; Type = 'A'
			if(tntMessage.SetMessage(pData/*, m_Settings.GetID()*/))
			{
				if(PostTradeAddMessage(tntMessage, &bsError) != 0)
				{
					bError = true;
				}else
				{
					pDescriptor->TradeID = tntMessage.GetTradeID().copy();
					pDescriptor->SequenceNo = tntMessage.GetHeader().GetSequenceNumber();
				}
			}else
			{
				bError = true;
				bsError =tntMessage.GetErrorDescription();
			}
		}
			
		if(bError)
		{	pDescriptor->SequenceNo = 0;
			if(pDescriptor->TradeID)
				SysFreeString(pDescriptor->TradeID);
			pDescriptor->TradeID = SysAllocString(L"");
			return  Error((BSTR)bsError,__uuidof(IMMHHTAdapter), E_FAIL);
		}
		
	}
	catch(_com_error& e)
	{
		//RaiseErrorMessage(enTradeAddResponse, m_LastSequenceNo, e.Error(), CComErrorWrapper::ErrorDescription( e ));
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), __uuidof(IMMHHTAdapter), e.Error() );
	}catch(...)
	{
		//RaiseErrorMessage(enTradeAddResponse, m_LastSequenceNo, E_FAIL, L"Unknown exception thrown in CMMHHTAdapter::LaunchWorkingThreads().");
		return Error( _T("Unknown exception thrown in CMMHHTAdapter::LaunchWorkingThreads()."), __uuidof(IMMHHTAdapter), E_FAIL);
	}
	return S_OK;
}


HRESULT CMMHHTAdapter::raise__TradeAddResponse( CComRecPtr<ACKData>& dataRec, ::LONG sCode, ::SAFEARRAY* errDescriptions) 
{
	ObjectLock lock(this);

	HRESULT hr = S_OK;
	IConnectionPointImpl<CMMHHTAdapter, &__uuidof(_IMMHHTAdapterEvents), CComDynamicUnkArray>* p = this;
	VARIANT rgvars[4];
	Lock();
	IUnknown** pp = p->m_vec.begin();
	Unlock();

	_variant_t vtRec;
	dataRec.CopyTo(vtRec);

	while (pp < p->m_vec.end()) {
		if (*pp != NULL) {
			IDispatch* pDispatch = (IDispatch*) *pp;

			::VariantInit(&rgvars[2]);
			rgvars[2] = vtRec;
			::VariantInit(&rgvars[1]);
			rgvars[1].vt = VT_I4;
			V_I4(&rgvars[1])= sCode;
			::VariantInit(&rgvars[0]);

			rgvars[0].vt = (VT_ARRAY | VT_RECORD);
			V_ARRAY(&rgvars[0])= (SAFEARRAY*) errDescriptions;

			DISPPARAMS disp = { rgvars, NULL, 3, 0 };
			VARIANT ret_val;
			hr = __ComInvokeEventHandler(pDispatch, 5, 1, &disp, &ret_val);
			if (FAILED(hr)) {
				break;
			}
		}
		pp++;
	}
	return hr;
}

HRESULT CMMHHTAdapter::raise__TradeNotification(CComRecPtr<TNTData>& dataRec) 
{
	HRESULT hr = S_OK;
	IConnectionPointImpl<CMMHHTAdapter, &__uuidof(_IMMHHTAdapterEvents), CComDynamicUnkArray>* p = this;
	VARIANT rgvars[1];
	Lock();
	IUnknown** pp = p->m_vec.begin();
	Unlock();
	_variant_t vtRec;
	dataRec.CopyTo(vtRec);

	while (pp < p->m_vec.end()) {
		if (*pp != NULL) {
			IDispatch* pDispatch = (IDispatch*) *pp;
			::VariantInit(&rgvars[0]);
			rgvars[0] = vtRec;
			DISPPARAMS disp = { rgvars, NULL, 1, 0 };
			VARIANT ret_val;
			hr = __ComInvokeEventHandler(pDispatch, 2, 1, &disp, &ret_val);
			if (FAILED(hr)) {
				break;
			}
		}
		pp++;
	}
	return hr;
}
HRESULT CMMHHTAdapter::raise__LogonResponse(::LONG sCode,::SAFEARRAY* errDescription) 
{
	HRESULT hr = S_OK;
	IConnectionPointImpl<CMMHHTAdapter, &__uuidof(_IMMHHTAdapterEvents), CComDynamicUnkArray>* p = this;
	VARIANT rgvars[2];
	Lock();
	IUnknown** pp = p->m_vec.begin();
	Unlock();
	while (pp < p->m_vec.end()) {
		if (*pp != NULL) {
			IDispatch* pDispatch = (IDispatch*) *pp;
			::VariantInit(&rgvars[1]);
			rgvars[1].vt = VT_I4;
			V_I4(&rgvars[1])= sCode;
			::VariantInit(&rgvars[0]);
			rgvars[0].vt = (VT_ARRAY | VT_RECORD);
			V_ARRAY(&rgvars[0])= (SAFEARRAY*) errDescription;
			DISPPARAMS disp = { rgvars, NULL, 2, 0 };
			VARIANT ret_val;
			hr = __ComInvokeEventHandler(pDispatch, 3, 1, &disp, &ret_val);
			if (FAILED(hr)) {
				break;
			}
		}
		pp++;
	}
	return hr;
}
HRESULT CMMHHTAdapter::raise__LogoffResponse(::LONG sCode,::SAFEARRAY* errDescription) 
{
	HRESULT hr = S_OK;
	IConnectionPointImpl<CMMHHTAdapter, &__uuidof(_IMMHHTAdapterEvents), CComDynamicUnkArray>* p = this;
	VARIANT rgvars[2];
	Lock();
	IUnknown** pp = p->m_vec.begin();
	Unlock();
	while (pp < p->m_vec.end()) {
		if (*pp != NULL) {
			IDispatch* pDispatch = (IDispatch*) *pp;
			::VariantInit(&rgvars[1]);
			rgvars[1].vt = VT_I4;
			V_I4(&rgvars[1])= sCode;
			::VariantInit(&rgvars[0]);
			rgvars[0].vt = (VT_ARRAY | VT_RECORD);
			V_ARRAY(&rgvars[0])= (SAFEARRAY*) errDescription;
			DISPPARAMS disp = { rgvars, NULL, 2, 0 };
			VARIANT ret_val;
			hr = __ComInvokeEventHandler(pDispatch, 4, 1, &disp, &ret_val);
			if (FAILED(hr)) {
				break;
			}
		}
		pp++;
	}
	return hr;
}

HRESULT CMMHHTAdapter::raise__ConnectorError(::LONG sCode,::BSTR errDescription)
{
	return __raise _IMMHHTAdapterEvents_ConnectorError(sCode, errDescription); 
	/*
	HRESULT hr = S_OK;
		IConnectionPointImpl<CMMHHTAdapter, &__uuidof(_IMMHHTAdapterEvents), CComDynamicUnkArray>* p = this;
		VARIANT rgvars[2];
		Lock();
		IUnknown** pp = p->m_vec.begin();
		Unlock();
		while (pp < p->m_vec.end()) {
			if (*pp != NULL) {
				IDispatch* pDispatch = (IDispatch*) *pp;
				::VariantInit(&rgvars[1]);
				rgvars[1].vt = VT_I4;
				V_I4(&rgvars[1])= sCode;
				::VariantInit(&rgvars[0]);
				rgvars[0].vt = (VT_ARRAY | VT_RECORD);
				V_ARRAY(&rgvars[0])= (SAFEARRAY*) errDescription;
				DISPPARAMS disp = { rgvars, NULL, 2, 0 };
				VARIANT ret_val;
				hr = __ComInvokeEventHandler(pDispatch, 5, 1, &disp, &ret_val);
				if (FAILED(hr)) {
					break;
				}
			}
			pp++;
		}
		return hr;*/
	
}


// End of IMMHHTAdapter implementation
/////////////////////////////////////////////////////////////////////////////////////////////