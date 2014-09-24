#include "StdAfx.h"
#include "MarketDataProvider.h"

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnLastQuoteInfo = 
{CC_STDCALL, VT_EMPTY, 2, {VT_RECORD, VT_RECORD} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnErrorInfo = 
{CC_STDCALL, VT_EMPTY, 4, {VT_I4, VT_BSTR, VT_I4, VT_VARIANT} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnQuoteUpdateInfo = 
{CC_STDCALL, VT_EMPTY, 0, {} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnSubscribedInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnUnsubscribedInfo = 
{CC_STDCALL, VT_EMPTY, 0, {} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnRisksUpdateInfo = 
{CC_STDCALL, VT_EMPTY, 0, {} };

_ATL_FUNC_INFO CMarketDataProvider::m_fiOnSubscribedRisksInfo = 
{CC_STDCALL, VT_EMPTY, 1, {VT_RECORD} };
//-----------------------------------------------------------------------------------------------------//
CMarketDataProvider::CMarketDataProvider(void)
	:	m_bInSubscribeMode(false),
		m_bInRequestMode(false)
{
	m_spPriceProvider = NULL;
	m_dwCooke = NULL;
}
//-----------------------------------------------------------------------------------------------------//
CMarketDataProvider::~CMarketDataProvider(void)
{	
	if(GetPriceProvider())	Disconnect();
}
//-----------------------------------------------------------------------------------------------------//
void CMarketDataProvider::Connect()
{
	try
	{
		if(GetPriceProvider())	Disconnect();

		IPriceProviderPtr		spPriceProvider;
		_com_util::CheckError(spPriceProvider.CreateInstance(__uuidof(DataFeedBatchPriceInfo)));

		IProviderPtr spProvider = spPriceProvider;
		//spProvider->Type = 10;
		spPriceProvider->Connect();

		IProvidersPtr spProvidersInfo; 
		_com_util::CheckError(spProvidersInfo.CreateInstance(CLSID_Providers));
		_com_util::CheckError(spProvidersInfo->Initialize());
		IProviderDataPtr spProviderInfo;

		//_com_util::CheckError(spProvidersInfo->GetProvider(spProvider->Type,&spProviderInfo));

		_com_util::CheckError(DispQuotes::DispEventAdvise(spPriceProvider/*, &IID_IBatchPriceProvider*/)); // can't connect error occure
		_com_util::CheckError(DispRisks::DispEventAdvise(spPriceProvider/*, &IID_IRisksProvider*/)); // can't connect error occure

		SetPriceProvider(spPriceProvider);

		m_dwCooke = NULL;

		CComGITPtr<IPriceProvider> ptrProviderGIT(m_spPriceProvider.GetInterfacePtr());
		m_dwCooke = ptrProviderGIT.Detach();

	}
	catch(_com_error& err)
	{
		Disconnect();

		TRACE_COM_ERROR(err);
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
	}
};
//-----------------------------------------------------------------------------------------------------//
void CMarketDataProvider::Disconnect()
{

	try
	{
		IPriceProviderPtr spPriceProvider = GetPriceProvider();

		if(static_cast<bool>(spPriceProvider))
		{
			DispQuotes::DispEventUnadvise(spPriceProvider);
			DispRisks::DispEventUnadvise(spPriceProvider);

			Sleep(1000L);

			CancelLastQuote();

			m_bInSubscribeMode	=	false;
			m_bInRequestMode	=	false;

			spPriceProvider->Disconnect();

			SetPriceProvider(NULL);
		}
	}
	catch(_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch(...)
	{
		TRACE_UNKNOWN_ERROR();
	};
}
//------------------------------------------------------------------------------------------------------------------------//
void CMarketDataProvider::RequestLastQuote(const CTicker& ticker)
{
	IPriceProviderPtr spPriceProvider = GetPriceProvider();
	try
	{
		if (static_cast<bool>(spPriceProvider))
		{
			QuoteUpdateParams	request;
			ticker.CopyTo(request);

			_com_util::CheckError(spPriceProvider->raw_RequestLastQuote(&request));

			SysFreeString(request.Symbol);
			SysFreeString(request.Exchange);

			m_bInRequestMode = true;
		}
	}
	catch (_com_error& err)
	{
		if (static_cast<bool>(spPriceProvider))
			spPriceProvider->raw_CancelLastQuote();

		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return;
}
//------------------------------------------------------------------------------------------------------------------------//
void CMarketDataProvider::SubscribeQuotesSingle(const CTicker& ticker)
{

	IPriceProviderPtr spPriceProvider = GetPriceProvider();
	try
	{
		if (static_cast<bool>(spPriceProvider))
		{
			QuoteUpdateParams	request;
			ticker.CopyTo(request);

			_com_util::CheckError(spPriceProvider->raw_SubscribeQuote(&request));

			SysFreeString(request.Symbol);
			SysFreeString(request.Exchange);

			m_bInRequestMode = true;
		}
	}
	catch (_com_error& err)
	{
		if (static_cast<bool>(spPriceProvider))
			spPriceProvider->raw_CancelLastQuote();

		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};

	return;
}
//------------------------------------------------------------------------------------------------------------------------//
void CMarketDataProvider::SubscribeRisksSingle(const CTicker& ticker)
{
	
	IRisksProviderPtr spProvider = GetRisksProvider();
	try
	{
		if (static_cast<bool>(spProvider))
		{
			ITicker	request;
			ticker.CopyTo(request);

			_com_util::CheckError(spProvider->raw_SubscribeRisks(&request));

			SysFreeString(request.Symbol);
			SysFreeString(request.Exchange);

			m_bInRequestMode = true;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------//
void CMarketDataProvider::CancelLastQuote()
{
	try
	{
		IPriceProviderPtr	spPriceProvider = GetPriceProvider();
		if (static_cast<bool>(spPriceProvider))
		{
			if(m_bInRequestMode)
			{
				spPriceProvider->raw_CancelLastQuote();
				m_bInRequestMode = false;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return;
}
//------------------------------------------------------------------------------------------------------------------------//
void CMarketDataProvider::UnSubscribeQuote()
{
	try
	{	
		IPriceProviderPtr	spPriceProvider = GetPriceProvider();

		if (static_cast<bool>(spPriceProvider))
		{
			if (m_bInSubscribeMode)
			{
				spPriceProvider->raw_UnSubscribeQuote();
				m_bInSubscribeMode = false;
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return;
}
//------------------------------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnLastQuote(struct QuoteUpdateParams* pParams, struct QuoteUpdateInfo* pResults)
{
	CTicker ticker(*pParams);
	CQuote	quote(*pResults);

	OnLastQuote(ticker, quote);
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnError(ErrorNumberEnum enErrorNumber, BSTR bsDescription,
										   RequestsTypeEnum enReqType, VARIANT varRequest)
{

	CString sDesc = (LPCTSTR)_bstr_t(bsDescription);
	
	if(enErrorNumber == enNotConnected || enErrorNumber == enConnectionWasDisconnected)
	{
		OnDisconnect(enErrorNumber, bsDescription);
		return S_OK;
	}

	switch(enReqType)
	{
		case enRequestLastQuote:
		{
				OnError(enErrorNumber, bsDescription);
				break;
		}
		case enSubscribeQuote:
		{
				OnError(enErrorNumber, bsDescription);
				break;
		}
		default:
		{
				OnError(enErrorNumber, bsDescription);
		}
	}
	return S_OK;
}
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnQuoteUpdate()
{
	OnQuoteUpdate();
	return S_OK;
};
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnSubscribed(struct QuoteUpdateParams* pParams)
{
	OnSubscribed(CTicker(*pParams));
	return S_OK;
};
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnSubscribedRisks(struct ITicker* pParams)
{
	OnSubscribedRisks(CTicker(*pParams));
	return S_OK;
};
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_OnUnsubscribed()
{
	OnUnsubscribed();
	return S_OK;
};
//--------------------------------------------------------------------------------------//
//IRisksProvider
STDMETHODIMP CMarketDataProvider::_OnRisksUpdate()
{
	OnRisksUpdate();
	return S_OK;
};
//--------------------------------------------------------------------------------------//
void CMarketDataProvider::Subscribe(RequestVector& request)
{
	HRESULT hr = S_OK;
	
	try
	{
		IBatchPriceProviderPtr spBatchPriceProvider = GetBatchPriceProvider();
		if (static_cast<bool>(spBatchPriceProvider))
		{
			CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib> RequestInfo;
			RequestInfo.Init();

			long outSize = static_cast<long>(request.size());
			SAFEARRAYBOUND sab = { outSize, 0 };
			LPSAFEARRAY psa = ::SafeArrayCreateEx( VT_RECORD, 1, &sab, (PVOID) RequestInfo.GetRecordInfo() );
			
			if(psa)
			{
				QuoteUpdateParams* pvOutData = NULL;
				if(SUCCEEDED(hr = ::SafeArrayAccessData(psa, (void**)&pvOutData)))
				{
					memset(pvOutData, 0,  outSize * sizeof(QuoteUpdateParams));			
					int j = 0;
					for (RequestVector::iterator it = request.begin(); it != request.end(); ++it)
					{
						(*it).CopyTo(pvOutData[j]);
						j++;
					}
					::SafeArrayUnaccessData(psa);
				}
			}
			_com_util::CheckError(spBatchPriceProvider->raw_SubscribeMultipleQuotes(&psa));
			::SafeArrayDestroy(psa);

			m_bInSubscribeMode = true;
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);		
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return;
};
//-----------------------------------------------------------------------------------------------------//
void CMarketDataProvider::GetQuoteUpdate()
{
	HRESULT hr = S_OK;

	try
	{
		IBatchPriceProviderPtr spBatchPriceProvider = GetBatchPriceProvider();

		if (static_cast<bool>(spBatchPriceProvider))
		{
			SAFEARRAY* pSafeArray =	spBatchPriceProvider->GetQuotesUpdates();
			if(pSafeArray)
			{
				QuoteUpdateFullInfo* pvData = NULL;
				if( SUCCEEDED( hr = ::SafeArrayAccessData( pSafeArray, (void**)&pvData) ) )
				{
					long lLBound = 0;
					long lUBound = 0;
					HRESULT hRes = SafeArrayGetLBound(pSafeArray, 1L, &lLBound);
					hRes = SafeArrayGetUBound(pSafeArray, 1L, &lUBound);
					long lArraySize = lUBound - lLBound ;

					for(int i = 0; i<= lArraySize; ++i)
						_AcceptQuote(pvData[i].Params, pvData[i].Info);

					::SafeArrayUnaccessData(pSafeArray);
				}
				::SafeArrayDestroy(pSafeArray);
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);	
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	};
	return;
};
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_AcceptQuote(const QuoteUpdateParams& Params, QuoteUpdateInfo& Info)
{
	HRESULT __Result = E_FAIL;
	try
	{
		CTicker	ticker(Params);
		CQuote	quote(Info);

		AcceptQuote(ticker, quote);
		
		__Result = S_OK;
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
	return __Result;
};
//-----------------------------------------------------------------------------------------------------//
void CMarketDataProvider::GetRisksUpdate()
{
	HRESULT hr = S_OK;

	try
	{
		IRisksProviderPtr spProvider = GetRisksProvider();

		if (static_cast<bool>(spProvider))
		{
			SAFEARRAY* pSafeArray =	spProvider->GetRisksUpdates();
			if(pSafeArray)
			{
				IRisksFullInfo* pvData = NULL;
				if( SUCCEEDED( hr = ::SafeArrayAccessData( pSafeArray, (void**)&pvData) ) )
				{
					long lLBound = 0;
					long lUBound = 0;
					HRESULT hRes = SafeArrayGetLBound(pSafeArray, 1L, &lLBound);
					hRes = SafeArrayGetUBound(pSafeArray, 1L, &lUBound);
					long lArraySize = lUBound - lLBound ;

					for(int i = 0; i<= lArraySize; ++i)
						_AcceptRisks(pvData[i].Ticker, pvData[i].Risks);

					::SafeArrayUnaccessData(pSafeArray);
				}
				::SafeArrayDestroy(pSafeArray);
			}
		}
	}
	catch (_com_error& err)
	{
		TRACE_COM_ERROR(err);
	}
	catch (...)
	{
		TRACE_UNKNOWN_ERROR();
	}
	return;
};
//-----------------------------------------------------------------------------------------------------//
STDMETHODIMP CMarketDataProvider::_AcceptRisks(const ITicker& Params, IRisks& Info)
{
	CTicker	ticker(Params);
	CRisks risks(Info);

	AcceptRisks(ticker, risks);

	return S_OK;
};
//-----------------------------------------------------------------------------------------------------//