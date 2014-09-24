#pragma once
#include "resource.h"       // main symbols
#include "ATFBaseFeed.h"


template< class T >
class CATFPriceInfoBaseFeed: public CATFBaseFeed<T>
{
private:
	map<_bstr_t, double> m_mapClosePrices;
	PriceProviderTypeEnum m_enMode;

private:
	void GetParams(CComVariant &varResult, QuoteUpdateParams *Params)
	{
		_QuoteUpdateParams qup;
		qup.Init(Params);
		qup.CopyTo (varResult);
	}
public:

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults)
	{
		ATFPROVTRACE(__FUNCTION__, _T("Start"));
		T* pT = static_cast<T*>(this);

		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = pT->m_vec.GetSize();
		HRESULT hr = S_OK;


		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = pT->m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
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

		if(FAILED(hr)) return hr;
		return varResult.scode;
	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	HRESULT QuoteUpdate(const CComVariant &varParams, const CComVariant &varResults)
	{
		T* pT = static_cast<T*>(this);

		ATFPROVTRACE(__FUNCTION__, _T("Start"));
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[2];
		int nConnections = pT->m_vec.GetSize();
		HRESULT hr = S_OK;

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = pT->m_vec.GetAt(nConnectionIndex);
			pT->Unlock();
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

		if(FAILED(hr)) return hr;
		return varResult.scode;

	}

	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults)
	{
		CComVariant vtNewResult;
		_QuoteUpdateInfo  result(varResults);

		result->TotalRequests = -1L;
		result->WaitTime = GetTickCount() - result->WaitTime;

		if(result->BidPrice<0.005 && result->AskPrice<0.005 && result->LastPrice<0.005)
			return S_OK;

		result.CopyTo(vtNewResult);
		return QuoteUpdate(varParams, vtNewResult);
	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, RequestsTypeEnum enumRequest, const CComVariant &varRequest)
	{
		T* pT = static_cast<T*>(this);
		//	CCriticalSectionWrapper cs(m_csEvent);
		ATFPROVTRACE(__FUNCTION__, _T("Start"));
		CComVariant varResult;
		int nConnectionIndex;
		CComVariant* pvars = new CComVariant[4];
		int nConnections = pT->m_vec.GetSize();
		HRESULT hr = S_OK;

		for (nConnectionIndex = 0; nConnectionIndex < nConnections; nConnectionIndex++)
		{
			pT->Lock();
			CComPtr<IUnknown> sp = pT->m_vec.GetAt(nConnectionIndex);
			pT->Unlock();

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

		if(FAILED(hr)) return hr;
		return varResult.scode;

	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void PublicQuoteData(const CComVariant& request, PB* pbData)
	{
		if(!pbData) return;

		_QuoteUpdateInfo result;
		_QuoteUpdateParams param(request);

		result.Init();
		result->BidPrice = SetDoubleValue(ATRealPrice(pbData, AT_BID, TRUE));
		result->AskPrice = SetDoubleValue(ATRealPrice(pbData, AT_ASK, TRUE));

		result->BidSize = (long) ATRealPrice(pbData, AT_BIDSIZE, TRUE);
		result->AskSize = (long) ATRealPrice(pbData, AT_ASKSIZE, TRUE);

		result->SeqNum = 0;

		result->Currency = SysAllocString(L"USD");

		result->LastPrice = SetDoubleValue(ATRealPrice(pbData, AT_LAST, FALSE));
		result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

		result->OpenInterest = SetDoubleValue(ATRealPrice(pbData, AT_OPENINTEREST, FALSE));
		result->Volume = (long)ATRealPrice(pbData, AT_VOLUME, FALSE);

		result->Exchange = ::SysAllocString(param->Exchange);
		result->LotSize = param->Type == enOPT ? 100 : 1;

		result->OpenPrice  = SetDoubleValue(ATRealPrice(pbData, AT_OPEN, FALSE));
		result->ClosePrice = SetDoubleValue(ATRealPrice(pbData, AT_CLOSE, FALSE));

		_bstr_t bsKey = CreateFullName(request, _enRequestLast);
		m_mapClosePrices[bsKey] = result->ClosePrice;

		result->NetChange = SetDoubleValue(ATRealPrice(pbData, AT_NETCHANGE, FALSE));
		if(fEQZero(result->NetChange) || result->NetChange==BAD_DOUBLE_VALUE)
			result->NetChange = SetDoubleValue(ATRealPrice(pbData, AT_NETCHANGE, TRUE));

		if((fEQZero(result->NetChange) || result->NetChange==BAD_DOUBLE_VALUE) && result->LastPrice > 0. && result->ClosePrice >0.)
			result->NetChange = result->LastPrice - result->ClosePrice;


		result->HighPrice = SetDoubleValue(ATRealPrice(pbData, AT_HIGH, FALSE));
		result->LowPrice  = SetDoubleValue(ATRealPrice(pbData, AT_LOW, FALSE));

		result->Skipped = 0;
		result->TotalRequests = 0;
		result->WaitTime = 0;
		if( IsInvalidDoubleValue(result->LastPrice) /*&& fEQZero(result->BidPrice) && fEQZero(result->AskPrice)*/)
			result->LastPrice = SetDoubleValue(ATRealPrice(pbData, AT_CLOSE, FALSE));

		CComVariant vtQuote;
		result.CopyTo(vtQuote);
		ATFPROVTRACE(__FUNCTION__, _T("PublicQuoteData, key = %s"), (LPCTSTR)bsKey);

		OnLastQuote(request, vtQuote);
	}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void PublicRealtime(_bstr_t bsKey, PB* pData)
	{
		USES_CONVERSION;
		try
		{
			_QuoteUpdateInfo result;
			result.Init();

			CSubscription& subs = m_Subscription[bsKey];
			CSubscription::REQUESTS::iterator iter = subs.m_Request.begin();
			if(iter!=subs.m_Request.end())
			{
				_QuoteUpdateParams param(iter->second);

				result->Exchange = ::SysAllocString(param->Exchange);
				result->LotSize = param->Type == enOPT ? 100 : 1;

				/*			if(info1->Type == enOPT)
				{
				result->Exchange = GetMTOptionExchange(pData->atheader.sym.exchange).copy();
				result->LotSize = 100;
				}
				else
				{
				result->LotSize = 1;
				result->Exchange = GetMTStockExchange(pData->atheader.sym.exchange).copy();
				}*/
			}

			result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_LAST, FALSE));
			result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();
			result->SeqNum = 0;
			result->BidPrice = SetDoubleValue(ATRealPrice(pData, AT_BID, TRUE));
			result->AskPrice = SetDoubleValue(ATRealPrice(pData, AT_ASK, TRUE));
			result->BidSize = (long)ATRealPrice(pData, AT_BIDSIZE, TRUE);
			result->AskSize = (long)ATRealPrice(pData, AT_ASKSIZE, TRUE);
			result->ClosePrice = SetDoubleValue(ATRealPrice(pData, AT_CLOSE, FALSE));
			result->Currency = _bstr_t(L"USD").copy();
			result->LotSize = 100;
			result->Volume = (long)ATRealPrice(pData, AT_VOLUME, FALSE);
			result->OpenInterest = SetDoubleValue(ATRealPrice(pData, AT_OPENINTEREST, FALSE));
			if( IsInvalidDoubleValue(result->LastPrice) /*&& IsInvalidDoubleValue(result->BidPrice) && IsInvalidDoubleValue(result->AskPrice)*/)
				result->LastPrice = result->ClosePrice;

			//		if(result->BidPrice<0.005 && result->AskPrice<0.005 && result->LastPrice<0.005)
			//			return;

			result->Skipped = 0;
			result->TotalRequests = 0;
			result->WaitTime = 0;

			m_mapClosePrices[bsKey] = result->ClosePrice;

			result->NetChange = SetDoubleValue(ATRealPrice(pData, AT_NETCHANGE, FALSE));
			if(fEQZero(result->NetChange) || result->NetChange==BAD_DOUBLE_VALUE)
				result->NetChange = SetDoubleValue(ATRealPrice(pData, AT_NETCHANGE, TRUE));

			if(
				result->NetChange==BAD_DOUBLE_VALUE && result->LastPrice > 0. &&
				(m_mapClosePrices.find(bsKey)!=m_mapClosePrices.end() && m_mapClosePrices[bsKey] >0.))
				result->NetChange = result->LastPrice - m_mapClosePrices[bsKey];

			CComVariant vtResult;
			result.CopyTo(vtResult);
			ATFPROVTRACE(__FUNCTION__, _T("PublicRealtime, key = %s"), (LPCTSTR)bsKey);
			PublicResponce(bsKey, vtResult, true);
		}
		catch(...)
		{

		}
	}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void PublicGroupData(const CComVariant& request, PB* pData, int iLotSize, bool bLast)
	{
		_QuoteUpdateInfo result;
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
			result->OpenPrice  = BAD_DOUBLE_VALUE;
			result->ClosePrice = BAD_DOUBLE_VALUE;

			result->HighPrice = BAD_DOUBLE_VALUE;
			result->LowPrice  = BAD_DOUBLE_VALUE;

			result->Skipped = 0;
			result->TotalRequests = 0;
			result->WaitTime = 0;
			result->NetChange = BAD_DOUBLE_VALUE;
		}
		else
		{
			if(!pData)
				return;

			_QuoteUpdateParams param;
			param.Init(request);

			result->BidPrice = SetDoubleValue(ATRealPrice(pData, AT_BID, TRUE));
			result->AskPrice = SetDoubleValue(ATRealPrice(pData, AT_ASK, TRUE));

			result->BidSize = (long)ATRealPrice(pData, AT_BIDSIZE, TRUE);
			result->AskSize = (long)ATRealPrice(pData, AT_ASKSIZE, TRUE);
			result->SeqNum = 0;

			result->Currency = SysAllocString(L"USD");
			result->LotSize = iLotSize;
			result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_LAST, FALSE));
			result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

			result->OpenInterest = SetDoubleValue(ATRealPrice(pData, AT_OPENINTEREST, FALSE));
			result->Volume = (long)ATRealPrice(pData, AT_VOLUME, FALSE);
			//		result->Exchange = GetMTOptionExchange(pData->atheader.sym.exchange).copy();
			result->Exchange = ::SysAllocString(param->Exchange);

			result->OpenPrice  = SetDoubleValue(ATRealPrice(pData, AT_OPEN, FALSE));
			result->ClosePrice = SetDoubleValue(ATRealPrice(pData, AT_CLOSE, FALSE));

			_bstr_t bsKey = CreateFullName(request, _enRequestLast);
			m_mapClosePrices[bsKey] = result->ClosePrice;

			result->NetChange = SetDoubleValue(ATRealPrice(pData, AT_NETCHANGE, FALSE));
			if(fEQZero(result->NetChange) || result->NetChange==BAD_DOUBLE_VALUE)
				result->NetChange = SetDoubleValue(ATRealPrice(pData, AT_NETCHANGE, TRUE));

			if((fEQZero(result->NetChange) || result->NetChange==BAD_DOUBLE_VALUE) && result->LastPrice > 0. && result->ClosePrice >0.)
				result->NetChange = result->LastPrice - result->ClosePrice;

			result->HighPrice = SetDoubleValue(ATRealPrice(pData, AT_HIGH, FALSE));
			result->LowPrice  = SetDoubleValue(ATRealPrice(pData, AT_LOW, FALSE));
			if( IsInvalidDoubleValue(result->LastPrice) /*&& fEQZero(result->BidPrice) && fEQZero(result->AskPrice)*/)
				result->LastPrice = SetDoubleValue(ATRealPrice(pData, AT_CLOSE, FALSE));

			result->Skipped = 0;
			result->TotalRequests = 0;
			result->WaitTime = 0;
			
			ATFPROVTRACE(__FUNCTION__, _T("PublicGroupData, key = %s"), (LPCTSTR)bsKey);
		}

		CComVariant vtQuote;
		result.CopyTo(vtQuote);
		PublicResponce(_enRequestLast, request, vtQuote);
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	void PublicSubscriptionNotification(const CComVariant& request)
	{
		PublicResponce(_enRequestRealtimeNotification, request);
	}		
};
