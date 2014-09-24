// ActivFeedBatchPriceInfo.cpp : Implementation of CActivFeedBatchPriceInfo

#include "stdafx.h"
#include "ActivFeedBatchPriceInfo.h"
#include ".\activfeedcontentgateway.h"
#include "ActivFeedAppication.h"
#include "CustomerInfo.h"
#include "ActivMiddleware\ActivBase\OsException.h"
#include <stdlib.h>

static int sessionCounter = 0;

HRESULT CActivFeedBatchPriceInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Enter"));
	HRESULT hr = S_OK;
	CComVariant varResult;

	try
	{								   

		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = m_vec.GetSize();


		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			Unlock();
			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[3] = enumError;
				pvars[2] = (BSTR)bstrDescription;
				pvars[1] = enumRequest;
				pvars[0] = varRequest;
				DISPPARAMS disp = { pvars, NULL, 4, 0 };
				hr = pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;

		if(FAILED(hr))
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Exit with error hr = %d, file: %s line: %d"),  hr, __FILE__, __LINE__);
			hr = E_FAIL;
		}

	}
	_CATCH_COM_EXCEPTION_RETURN_COM(L"On Error Failed")
		_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;

	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Exit"));
	if (hr == E_FAIL)
		return E_FAIL;
	else
		return varResult.scode;
}
HRESULT CActivFeedBatchPriceInfo::OnSubscribed(const CComVariant &varParams)
{
	CComVariant varResult;
	HRESULT hr = S_OK;
	try
	{
		ATLTRACE(_T("CActivFeedBatchPriceInfo::OnSubscribed\n"));

		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[1];
		int nConnections = m_vec.GetSize();


		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			Lock();
			CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
			Unlock();

			IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
			if (pDispatch != NULL)
			{
				VariantClear(&varResult);
				pvars[0] = varParams;
				DISPPARAMS disp = { pvars, NULL, 1, 0 };
				hr = pDispatch->Invoke(0x4, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
			}
		}
		delete[] pvars;
	}
	_CATCH_COM_EXCEPTION_RETURN_COM( L"OnSubscribed Failed")
		_CATCH_UNHANDLED_EXCEPTION_RETURN_COM;
	if(FAILED(hr)) 
		return hr;
	return varResult.scode;
}


HRESULT CActivFeedBatchPriceInfo::OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
{
	HRESULT hr = S_OK;
	CComVariant varResult;
	CComVariant *pvars = new CComVariant[2];

	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			pvars[1] = varParams; 
			pvars[0] = varResults;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return S_OK;
}
// RequestLastQuote - called by client to obtain last quote's value
STDMETHODIMP CActivFeedBatchPriceInfo::RequestLastQuote( QuoteUpdateParams *Params )
{
	HRESULT hr = S_OK;
	if( Params && m_spActiv ){
		try					
		{
			_QuoteRequestPtr m_spRequest( new _QuoteRequest( *Params) );
			m_spActiv->RequestLastQuote( m_spRequest, CActivFeedContentGateway::enRegularRequest); 
		}
		catch ( _com_error& e)
		{
			hr = e.Error();
		}
		catch (... )
		{ 
			EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") );
		}
	}
	return m_spActiv ? hr : E_INVALIDARG;
}

STDMETHODIMP CActivFeedBatchPriceInfo::RequestMultipleQuotes (SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams)
{
	HRESULT hr = S_OK;
	if( pParams && m_spActiv )
	{
		QuoteUpdateParams* pvData = NULL;
		if( SUCCEEDED( hr = ::SafeArrayAccessData( *pParams, (void**)&pvData) ) )
		{
			long lLBound = 0;
			long lUBound = 0;
			HRESULT hRes = SafeArrayGetLBound(*pParams, 1L, &lLBound);
			hRes = SafeArrayGetUBound(*pParams, 1L, &lUBound);
			long lArraySize = lUBound - lLBound +1 ;
			if(lArraySize)
			{	
				try	
				{
					long lNonOptionRequestsCount = 0;
					// Request All non option types
					for(int i = 0; i< lArraySize; ++i)
					{	
						if(pvData[i].Type != enOPT)
						{
							_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
							m_spActiv->RequestLastQuote( spRequest, CActivFeedContentGateway::enRegularRequest); 
							++lNonOptionRequestsCount;
						}
					}
					if(lNonOptionRequestsCount!=lArraySize)
					{
						long lOptionRequestsLeft = lArraySize - lNonOptionRequestsCount;
						if(lOptionRequestsLeft == 1)
						{
							for(int i = 0; i<= lArraySize; ++i)
							{	
								if(pvData[i].Type == enOPT)
								{
									_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
									m_spActiv->RequestLastQuote( spRequest, CActivFeedContentGateway::enRegularRequest); 
									++lNonOptionRequestsCount;
								}
							}
						}
						else
						{
							bool bFirst = true;
							for(int i = 0; i< lArraySize; ++i)
							{	
								if(pvData[i].Type == enOPT)
								{
									_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
									--lOptionRequestsLeft;
									CActivFeedContentGateway::CRequestType enType = CActivFeedContentGateway::enMultipleRequest;
									if(bFirst)
									{
										bFirst = false;
										enType = CActivFeedContentGateway::enStartMultipleRequest;
									}
									if(!lOptionRequestsLeft)
										enType = CActivFeedContentGateway::enEndMultipleRequest;

									m_spActiv->RequestLastQuote( spRequest, enType); 
								}
							}
						} 
					}
				}
				catch( _com_error& e)
				{
					hr = e.Error(); 
				}
				catch(... )	
				{ 
					EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); 
				}

			}
			::SafeArrayUnaccessData(*pParams);
		}
	}
	return m_spActiv ? hr : E_INVALIDARG;
}

STDMETHODIMP CActivFeedBatchPriceInfo::SubscribeMultipleQuotes(SAFEARRAY**  /*(struct QuoteUpdateParams)*/  pParams)
{
	HRESULT hr = S_OK;
	if( pParams && m_spActiv )
	{
		QuoteUpdateParams* pvData = NULL;
		if( SUCCEEDED( hr = ::SafeArrayAccessData( *pParams, (void**)&pvData) ) )
		{
			long lLBound = 0;
			long lUBound = 0;
			HRESULT hRes = SafeArrayGetLBound(*pParams, 1L, &lLBound);
			hRes = SafeArrayGetUBound(*pParams, 1L, &lUBound);
			long lArraySize = lUBound - lLBound +1;
			if(lArraySize)
			{	
				try	
				{
					long lNonOptionRequestsCount = 0;
					// Request All non option types
					for(int i = 0; i< lArraySize; ++i)
					{	
						if(pvData[i].Type != enOPT)
						{
							_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
							m_spActiv->SubscribeQuote( spRequest, CActivFeedContentGateway::enRegularRequest); 
							++lNonOptionRequestsCount;
						}
					}
					if(lNonOptionRequestsCount!=lArraySize)
					{
						long lOptionRequestsLeft = lArraySize - lNonOptionRequestsCount;
						if(lOptionRequestsLeft == 1)
						{
							for(int i = 0; i< lArraySize; ++i)
							{	
								if(pvData[i].Type == enOPT)
								{
									_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
									m_spActiv->SubscribeQuote( spRequest, CActivFeedContentGateway::enRegularRequest); 
									++lNonOptionRequestsCount;
								}
							}
						}
						else
						{
							bool bFirst = true;
							for(int i = 0; i< lArraySize; ++i)
							{	
								if(pvData[i].Type == enOPT)
								{
									_QuoteRequestPtr spRequest( new _QuoteRequest(pvData[i]) );
									--lOptionRequestsLeft;
									CActivFeedContentGateway::CRequestType enType = CActivFeedContentGateway::enMultipleRequest;
									if(bFirst)
									{
										bFirst = false;
										enType = CActivFeedContentGateway::enStartMultipleRequest;
									}
									if(!lOptionRequestsLeft)
										enType = CActivFeedContentGateway::enEndMultipleRequest;

									m_spActiv->SubscribeQuote( spRequest, enType); 
								}
							}
						} 
					}
				}
				catch( _com_error& e)
				{
					hr = e.Error(); 
				}
				catch(... )	
				{ 
					EgLib::CEgLibTraceManager::TraceText( LogSystem, __FUNCTION__ , _T(" Unhandled exception ") ); 
				}

			}
			::SafeArrayUnaccessData(*pParams);
		}
	}
	return m_spActiv ? hr : E_INVALIDARG;
}

STDMETHODIMP CActivFeedBatchPriceInfo::CancelLastQuote(VARIANT Params)
{
	return S_OK;
}
STDMETHODIMP CActivFeedBatchPriceInfo::SubscribeQuote(QuoteUpdateParams *Params)
{
	_QuoteRequestPtr m_spRequest( new _QuoteRequest( *Params ) );

	if( Params && m_spActiv )
	{
		// real job in gateway
		EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("SQU : %s.%s[%d]"), m_spRequest->Symbol.c_str(), m_spRequest->Exchange.c_str(), m_spRequest->Type );
		if(  m_spActiv->SubscribeQuote( m_spRequest, CActivFeedContentGateway::enRegularRequest) )
			return S_OK;
	}
	EgLib::CEgLibTraceManager::TraceText(LogWarning, __FUNCTION__ , _T("Failed") );
	return E_FAIL;
}

STDMETHODIMP CActivFeedBatchPriceInfo::UnSubscribeQuote(VARIANT Params)
{
	
	_QuoteRequestPtr m_spRequest;

	if( utils::HasValue( Params ) )
	{
		_QuoteUpdateParams p( Params );
		m_spRequest = boost::shared_ptr<_QuoteRequest> ( new _QuoteRequest( *( (QuoteUpdateParams*)p ) ) );
	}
	if( m_spActiv )
		m_spActiv->Unsubscribe( m_spRequest );
	return S_OK;
}

UINT	CActivTimer::Run(){
	while(1){
		if ( WaitForSingleObject( CThread::GetStopEventHandle() , m_Delay ) != WAIT_TIMEOUT ) {
			EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Timer thread exit") );
			break;
		}
		if( m_pAFBPI && m_pAFBPI->m_spActiv->IsUpdated() )
		{
			EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("Sending RTU for %d quotes"), m_pAFBPI->m_spActiv->GetUpdatesCount() );
			HRESULT hr = S_OK;
			CComVariant varResult;

			int nConnectionIndex;
			int nConnections = m_pAFBPI->m_vec.GetSize();

			for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
			{
				m_pAFBPI->Lock();
				CComPtr<IUnknown> sp = m_pAFBPI->m_vec.GetAt(nConnectionIndex);
				m_pAFBPI->Unlock();
				IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
				if (pDispatch != NULL)
				{
					VariantClear(&varResult);
					DISPPARAMS disp = { 0, 0, 0, 0 };
					pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
				}
			}
		}
	}
	return 0;
}
void	CActivFeedBatchPriceInfo::OnRequstLast( QuoteUpdateParams& m_spRequest, QuoteUpdateInfo& m_spResponse )
{
	HRESULT hr = S_OK;
	CComVariant varResult;
	CComVariant* pvars = new CComVariant[2];

	int nConnectionIndex;
	int nConnections = m_vec.GetSize();
	CComRecPtr<QuoteUpdateInfo> rs;
	CComRecPtr<QuoteUpdateParams> rq;

	rq.Init( &m_spRequest );
	rs.Init( &m_spResponse );
	for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
	{
		Lock();
		CComPtr<IUnknown> sp = m_vec.GetAt(nConnectionIndex);
		Unlock();
		IDispatch* pDispatch = reinterpret_cast<IDispatch*>(sp.p);
		if (pDispatch != NULL)
		{
			VariantClear(&varResult);
			rq.CopyTo( pvars[1] );
			rs.CopyTo( pvars[0] );
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
}
STDMETHODIMP CActivFeedBatchPriceInfo::Connect()
{
	HRESULT hr = S_OK;
	EgLib::CEgLibTraceManager::TraceText(LogDebug, __FUNCTION__ , _T("Enter"));
	int i = 1;
	if( !m_spActiv && i)
	{
		try
		{
			char	tmp[20];

			m_Settings.m_systemName = "EGARsystem";
			m_Settings.m_systemName += itoa( sessionCounter++, tmp, 10 );
			m_Settings.m_enableCtrlHandler = false;
			EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Creating new Activ Application...") );
			m_spActiv =  CActivFeedAppicationPtr(new CActivFeedAppication(this, m_Settings));
			m_spActiv->GetProcess()->SetState( IProcess::STATE_RUNNING);
			EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Done") );
		}
		catch( ... )
		{
			EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Failed at %s[%d]"), __FILE__, __LINE__ );
		}

		EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Connecting attempt...") );
		try{
			if( m_spActiv && m_spActiv->Connect())
			{
				EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Done") );
				//TIMECAPS tc;
				//timeGetDevCaps(&tc, sizeof(TIMECAPS));
				//DWORD resolution = __min(__max(tc.wPeriodMin, 0), tc.wPeriodMax);
				//timeBeginPeriod(resolution);     
				// create the timer
				std::string s = ActivFeedSettings::g_spUserSettings->Settings->Settings[_T("BatchNotificationDelay")];
				std::string st = ActivFeedSettings::g_spUserSettings->Settings->Settings[_T("MarketOpenTime")];
				std::string foof = ActivFeedSettings::g_spUserSettings->Settings->Settings[_T("FilterOffHoursUpdates")];
				UINT Delay = atoi( s.c_str() );
				unsigned int mot = atoi( st.c_str() );
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("Creating subscription timer, with delay %d"), Delay );
				m_spActiv->SetMarketOpenTime( mot );
				m_spActiv->SwitchOffHourUpdateFilter( foof == "0" ? false : true );
				EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__ , _T("Off hours update filter is %s"),  foof == "0" ? "OFF" : "ON" );
				if( Delay )
				{
					//m_UpdateTimer = timeSetEvent( Delay, /*resolution*/1000, TimerFunction, reinterpret_cast<DWORD_PTR>(this), TIME_PERIODIC);   
					m_pTimer = ActivTimerPtr( new CActivTimer( this, Delay ) );
					if ( m_pTimer) {
						m_pTimer->Start();
						EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Done. Exit S_OK") );
						return S_OK;
					}
				}
				EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Failed at %s[%d]. Exit E_FAIL"), __FILE__, __LINE__ );
				return E_FAIL;
			}
			else{

				EgLib::CEgLibTraceManager::Trace(LogWarning, __FUNCTION__ , _T("Failed at %s[%d]. Exit E_FAIL"), __FILE__, __LINE__ );
				return Error(L"Internal error: unable to connect to server ", __uuidof(ActivFeedBatchPriceInfo), E_FAIL );
			}
		}
		catch (_com_error& Err ) {
			hr =  Err.Error(); //utils::ComError2ErrInfo(Err, this,__uuidof(IBatchPriceProvider));
		}
	}
	EgLib::CEgLibTraceManager::TraceText(LogInfo, __FUNCTION__ , _T("Exit OK"));
	return hr;

}
STDMETHODIMP CActivFeedBatchPriceInfo::Disconnect()
{
	if( m_pTimer )
		m_pTimer->Stop();
	if(m_spActiv !=NULL)
	{
		try
		{
			m_spActiv->Disconnect();
		}
		catch( Activ::SystemException* pe)
		{
			std::string s = pe->ToString();
			int i= 0;
			i++;
		}
		catch( Activ::SystemException& e)
		{
			std::string s = e.ToString();
			int i= 0;
			i++;
		}
		catch( Activ::Exception& se)
		{
			std::string s = se.ToString();
			int i= 0;
			i++;
		}
		catch (...) {
			int  i = 0;
			i++;
		}
		
	}
	return S_OK;
}
STDMETHODIMP CActivFeedBatchPriceInfo::SetRegionRate(BSTR bstrRegion, double dRate)
{
	USES_CONVERSION;
	ActivFeedSettings::g_spUserSettings->Settings->SetRegionRate(OLE2A(bstrRegion), dRate);
	return S_OK;
}

// IBatchPriceProvider
STDMETHODIMP CActivFeedBatchPriceInfo::get_IsQuotesUpdated(/*[out, retval]*/ VARIANT_BOOL *pVal)
{
	*pVal =  m_spActiv->IsUpdated() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}
STDMETHODIMP CActivFeedBatchPriceInfo::GetQuotesUpdates(/*[out, retval]*/ SAFEARRAY** pRetVal)
{
	//m_spActiv->GetUpdatedQuotes( pRetVal );
	if( !pRetVal )
	{
		return E_POINTER;
	}

	if(*pRetVal)
	{
		::SafeArrayDestroy(*pRetVal);
		*pRetVal = NULL;
	}

	
	_QuoteUpdateFullInfo fullinfoInit;
	fullinfoInit.Init();
	HRESULT hr;
	long size = m_spActiv->GetUpdatesCount();
	SAFEARRAYBOUND sab = { size, 0 };
	LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_RECORD, 1, &sab, (PVOID) fullinfoInit.GetRecordInfo() );
	if(psa)
	{
		QuoteUpdateFullInfo* pvData = NULL;
		if( SUCCEEDED( hr = ::SafeArrayAccessData( psa, (void**)&pvData) ) )
		{
			memset(pvData, 0, size * sizeof( QuoteUpdateFullInfo ) );
			m_spActiv->CopyUpdatedQuotes( pvData, size );
			hr = S_OK;
			::SafeArrayUnaccessData(psa);
			*pRetVal = psa;
		}
		else
		{
			DWORD dwError = GetLastError();
			hr = HRESULT_FROM_WIN32(dwError);
		}
	}
	else
	{
		DWORD dwError = GetLastError();
		hr = HRESULT_FROM_WIN32(dwError);
	}
	return hr;
}

STDMETHODIMP CActivFeedBatchPriceInfo::put_UserName(BSTR userName)
{
	return S_OK;
}

STDMETHODIMP CActivFeedBatchPriceInfo::UnNotifiedSubscribtionQuantity(VARIANT_BOOL bSkipNotification,  LONG* pVal)
{
	HRESULT hr = S_OK;
	if(!pVal)
		return E_POINTER;
	if(bSkipNotification!=VARIANT_FALSE)
		*pVal = InterlockedExchange(&m_lUnnotifiedSubscriptions, 0);
	else
		*pVal = InterlockedExchange(&m_lUnnotifiedSubscriptions, m_lUnnotifiedSubscriptions);
	return hr;
}
