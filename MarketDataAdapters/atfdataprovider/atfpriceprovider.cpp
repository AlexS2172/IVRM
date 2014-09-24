// ATFPriceProvider.cpp : Implementation of CATFPriceProvider
#include "stdafx.h"
#include "ATFDataProvider.h"
#include "ATFPriceProvider.h"

/////////////////////////////////////////////////////////////////////////////
// CATFPriceProvider

STDMETHODIMP CATFPriceProvider::RequestLastQuote(QuoteUpdateParams *Params)
{
	try
	{
		_QuoteUpdateParams param;
		param.Init();

		CComVariant vt;
		param->Symbol = SysAllocString(Params->Symbol);
		param->Type = Params->Type;
		param->Exchange = SysAllocString(Params->Exchange);
		param.CopyTo(vt);
		AddRequest(vt, _enRequestLast);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CATFPriceProvider::CancelLastQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestLast);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return DISP_E_BADVARTYPE;
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;

}
STDMETHODIMP CATFPriceProvider::SubscribeQuote(QuoteUpdateParams *Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		_QuoteUpdateParams param;
		param.Init();
		CComVariant vt;
		param->Symbol = SysAllocString(Params->Symbol);
		param->Type = Params->Type;
		param->Exchange = SysAllocString(Params->Exchange);
		param.CopyTo(vt);
		AddRequest(vt, _enRequestRealtime);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceProvider::UnSubscribeQuote(VARIANT Params)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		RemoveRequest(Params, _enRequestRealtime);
	}
	catch (_com_error &err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return DISP_E_BADVARTYPE;
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceProvider::Connect()
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}
STDMETHODIMP CATFPriceProvider::Disconnect()
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try 
	{
		DisconnectProvider();
	}
	catch(_com_error& err)
	{
		ATFPROVTRACE(__FUNCTION__, _T("End"));
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
	return S_OK;
}


HRESULT CATFPriceProvider::OnQuoteUpdate(const CComVariant &varParams,const  CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	
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
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	ATFPROVTRACE(__FUNCTION__, _T("End"));
	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CATFPriceProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[4];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	
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

	ATFPROVTRACE(__FUNCTION__, _T("End"));
	if(FAILED(hr)) return hr;
	return varResult.scode;
}

HRESULT CATFPriceProvider::OnLastQuote(const CComVariant &varParams,const CComVariant &varResults)
{
//	CCriticalSectionWrapper cs(m_csEvent);
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
	int nConnections = m_vec.GetSize();
	HRESULT hr = S_OK;
	
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
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	ATFPROVTRACE(__FUNCTION__, _T("End"));
	if(FAILED(hr)) return hr;
	return varResult.scode;
}

void CATFPriceProvider::PublicQuoteData(const CComVariant& request, PB* pbData)
{
	if(!pbData) return;
	ATFPROVTRACE(__FUNCTION__, _T("Start"));

	_QuoteUpdateResult result;
	_QuoteUpdateParams param(request);
	result.Init();
	result->BidPrice = SetDoubleValue(ATRealPrice(pbData, AT_BID, FALSE));
	result->AskPrice = SetDoubleValue(ATRealPrice(pbData, AT_ASK, FALSE));

	result->BidSize =(long) ATRealPrice(pbData, AT_BIDSIZE, FALSE);
	result->AskSize =(long) ATRealPrice(pbData, AT_ASKSIZE, FALSE);
	result->SeqNum = 0;
	
	result->Currency = SysAllocString(L"USD");

	result->LastPrice = SetDoubleValue(ATRealPrice(pbData, AT_LAST, FALSE));
	if( IsInvalidDoubleValue(result->LastPrice) /*&& fEQZero(result->BidPrice) && fEQZero(result->AskPrice)*/)
		result->LastPrice = SetDoubleValue(ATRealPrice(pbData, AT_CLOSE, FALSE));

	result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

	result->OpenInterest = SetDoubleValue(ATRealPrice(pbData, AT_OPENINTEREST, FALSE));
	result->Volume = (long)ATRealPrice(pbData, AT_VOLUME, FALSE);

	result->Exchange = ::SysAllocString(param->Exchange);
	result->LotSize = param->Type == enOPT ? 100 : 1;

	CComVariant vtQuote;
	result.CopyTo(vtQuote);
	PublicResponce(_enRequestLast, request, vtQuote);
	ATFPROVTRACE(__FUNCTION__, _T("End"));
} 

void CATFPriceProvider::PublicRealtime(_bstr_t bsKey, PB* pData)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	try
	{
		_QuoteUpdateResult result;
		result.Init();

		result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_LAST, FALSE));
		result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

		CSubscription& subs = m_Subscription[bsKey];
		CSubscription::REQUESTS::iterator iter = subs.m_Request.begin();
		if(iter!=subs.m_Request.end())
		{
			_QuoteUpdateParams param(iter->second);

			result->Exchange = ::SysAllocString(param->Exchange);
			result->LotSize = param->Type == enOPT ? 100 : 1;
		}

		result->BidPrice = SetDoubleValue(ATRealPrice(pData, AT_BID, FALSE));
		result->AskPrice = SetDoubleValue(ATRealPrice(pData, AT_ASK, FALSE));
		result->BidSize = (long) ATRealPrice(pData, AT_BIDSIZE, FALSE);
		result->AskSize = (long) ATRealPrice(pData, AT_ASKSIZE, FALSE);
		result->Currency = _bstr_t(L"USD").copy();
		result->Volume = (long)ATRealPrice(pData, AT_VOLUME, FALSE);
		result->OpenInterest = SetDoubleValue(ATRealPrice(pData, AT_OPENINTEREST, FALSE));

		if( IsInvalidDoubleValue(result->LastPrice) /*&& fEQZero(result->BidPrice) && fEQZero(result->AskPrice)*/)
			result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_CLOSE, FALSE));

		CComVariant vtResult;
		result.CopyTo(vtResult);
		PublicResponce(bsKey, vtResult, false);
	}
	catch(...)
	{

	}
	ATFPROVTRACE(__FUNCTION__, _T("End"));
}

void CATFPriceProvider::PublicGroupData(const CComVariant& request, PB* pData,int lotSize, bool bLast)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	_QuoteUpdateResult result;
	result.Init();

	if (bLast)
	{
		result->BidPrice = BAD_DOUBLE_VALUE;
		result->AskPrice = BAD_DOUBLE_VALUE;

		result->BidSize = BAD_LONG_VALUE;
		result->AskSize = BAD_LONG_VALUE;
		result->SeqNum = 0;
			
		result->Currency = SysAllocString(L"");
		result->LotSize = BAD_LONG_VALUE;
		result->LastPrice = BAD_DOUBLE_VALUE;
		result->UpdateDateTime = 0;

		result->OpenInterest = BAD_DOUBLE_VALUE;
		result->Volume = BAD_LONG_VALUE;
		result->Exchange = SysAllocString(L"");
	}
	else
	{
		if(!pData)
			return;

		_QuoteUpdateParams param;
		param.Init(request);

		result->BidPrice = SetDoubleValue(ATRealPrice(pData, AT_BID, FALSE));
		result->AskPrice = SetDoubleValue(ATRealPrice(pData, AT_ASK, FALSE));

		result->BidSize = (long)ATRealPrice(pData, AT_BIDSIZE, FALSE);
		result->AskSize = (long)ATRealPrice(pData, AT_ASKSIZE, FALSE);
		result->SeqNum = 0;
			
		result->Currency = SysAllocString(L"USD");
		result->LotSize = lotSize;
		result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_LAST, FALSE));
		if( IsInvalidDoubleValue(result->LastPrice) /*&& fEQZero(result->BidPrice) && fEQZero(result->AskPrice)*/)
			result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_CLOSE, FALSE));
		
		result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

		result->OpenInterest = SetDoubleValue(ATRealPrice(pData, AT_OPENINTEREST, FALSE));
		result->Volume =(long) ATRealPrice(pData, AT_VOLUME, FALSE);

		result->Exchange = ::SysAllocString(param->Exchange);
	}

	CComVariant vtQuote;
	result.CopyTo(vtQuote);
	PublicResponce(_enRequestLast, request, vtQuote);
	ATFPROVTRACE(__FUNCTION__, _T("End"));
}
