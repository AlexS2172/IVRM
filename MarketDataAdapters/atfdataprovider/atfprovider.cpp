// ATFProvider.cpp : Implementation of CATFProvider

#include "stdafx.h"
#include "ATFProvider.h"


void CATFProvider::PublicGroupData(const CComVariant& request, PB* pData, int iLotSize, bool bLast)
{
	_Options result;
	result.Init();

	if(bLast)
	{
		result->BidPrice = 0;
		result->AskPrice = 0;

		result->BidSize = 0;
		result->AskSize = 0;
			
		result->Currency = SysAllocString(L"");
		result->LotSize = 0;
		result->LastPrice = 0;
		result->UpdateDateTime = 0;

		result->OpenInterest = 0;
		result->Volume = 0;
		result->Exchange = 0;
		result->OpenPrice  = 0;
		result->ClosePrice = 0;

		result->HighPrice = 0;
		result->LowPrice  = 0;


		result->StrikePrice = 0;
		result->ExpirationDate = 0;
		result->Symbol = SysAllocString(L"");
		result->Currency = SysAllocString(L"");
		result->LotSize = 0;
		
		result->Exchange = SysAllocString(L"");

		result->OpenInterest = 0;
		result->Volume = 0;
		result->YVolume = 0;

		result->NetChange = 0.;
		bLast = true;
	}
	else
	{
		if(!pData)
			return;

		_StockParams param;
		param.Init(request);

		result->BidPrice = ATRealPrice(pData, AT_BID, TRUE);
		result->AskPrice = ATRealPrice(pData, AT_ASK, TRUE);

		result->BidSize = (long)ATRealPrice(pData, AT_BIDSIZE, TRUE);
		result->AskSize = (long)ATRealPrice(pData, AT_ASKSIZE, TRUE);
			
		result->Currency = SysAllocString(L"USD");
		result->LotSize = iLotSize;
		result->LastPrice = ATRealPrice(pData, AT_LAST, FALSE);;
		result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();

		result->OpenInterest = (long)ATRealPrice(pData, AT_OPENINTEREST, FALSE);
		result->Volume = (long)ATRealPrice(pData, AT_VOLUME, FALSE);
//		result->Exchange = GetMTOptionExchange(pData->atheader.sym.exchange).copy();
		result->Exchange = ::SysAllocString(param->Exchange);

		result->OpenPrice  = ATRealPrice(pData, AT_OPEN, FALSE);
		result->ClosePrice = ATRealPrice(pData, AT_CLOSE, FALSE);

		result->NetChange = ATRealPrice(pData, AT_NETCHANGE, TRUE);

		result->HighPrice = ATRealPrice(pData, AT_HIGH, FALSE);
		result->LowPrice  = ATRealPrice(pData, AT_LOW, FALSE);;
		if( fEQZero(result->LastPrice) && fEQZero(result->BidPrice) && fEQZero(result->AskPrice))
			result->LastPrice = ATRealPrice(pData, AT_CLOSE, FALSE);;


		result->StrikePrice =  ATRealPrice(pData, AT_STRIKE, FALSE );
		result->ExpirationDate = eg_lib::vt_date(pData->atstkopt.exprdate.year,pData->atstkopt.exprdate.month, pData->atstkopt.exprdate.day);
		_bstr_t bsSymbol = ExtractOptionSymbol(pData->atstkopt.sym);

		if(pData->atstkopt.sym.symbol[3]<'M')
			result->OptionType = enCall;
		else
			result->OptionType = enPut;

		result->Symbol = bsSymbol.copy();
		result->Currency = SysAllocString(L"USD");
		result->LotSize = 100;
		
//		result->Exchange = GetMTOptionExchange(pData->atstkopt.sym.exchange).copy();

		result->Volume = pData->atstkopt.volume;
		result->YVolume = pData->atstkopt.yVolume;

	}

	CComVariant vtQuote;
	result.CopyTo(vtQuote);
	PublicResponce(_enRequestGrpOptions, request, vtQuote, bLast);
}

void CATFProvider::PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD)
{
	try
	{
		_Stocks result;
		result.Init();

		_StockParams param;
		param.Init(request);

		switch(pFD->equityFP.dividendFrequencyCode)
		{
		case 'Q':
			result->DivFreq  = 4;		break; //Quarterly
		case 'S':
			result->DivFreq  = 2;		break; //Semi-annually
		case 'M':
			result->DivFreq  = 12;	break; //Monthly
		case 'A':
			result->DivFreq  = 1;      break; //Annually
		case 'N':
		default:						   //None
			result->DivFreq  = 0;
		}

		if(0 != result->DivFreq)
		{
			WORD date = pFD->equityFP.prevCashDivDate;
			result->DivLastDate = eg_lib::vt_date((1970 + (date >> 9)), (date & 0x01e0) >> 5, date & 0x001f);

			DWORD  nDecType = pFD->equityFP.dividendPriceType;

			if (nDecType > 20)
				nDecType = 0;

			result->DivAmount = 1.*pFD->equityFP.prevCashDivAmount/decTable[nDecType];
		}
		else
		{
			result->DivLastDate = 0.;
			result->DivAmount = 0.;
		}


		result->LotSize = 1;
//		result->Exchange = GetMTStockExchange(pbData->atstock.sym.exchange).copy();
		result->Exchange = ::SysAllocString(param->Exchange);

		if(GetPriceBlockFlags(pbData,GBC_ANYSTOCK))
		{
			result->StockType = enStock;
		}
		else
		{
			if(GetPriceBlockFlags(pbData,GBC_INDEX))
			{
				result->StockType = enIndex;
			}
			else
				return;
		}
		
		result->Currency = _bstr_t(L"USD").copy();
		result->Description = Convert((UCHAR*)pFD->equityFP.name, sizeof(pFD->equityFP.name)-1).copy();


		result->Volume = (long)ATRealPrice(pbData, AT_VOLUME, FALSE);

		DWORD nDecType = pFD->equityFP.highLowPriceType;
		if (nDecType > 20)
			nDecType = 0;
		 
		result->LastPrice = ATRealPrice(pbData, AT_LAST, FALSE);
		result->OpenPrice  = ATRealPrice(pbData, AT_OPEN, FALSE);
		result->ClosePrice = ATRealPrice(pbData, AT_CLOSE, FALSE);

		result->HighPrice = ATRealPrice(pbData, AT_HIGH, FALSE);
		result->LowPrice  = ATRealPrice(pbData, AT_LOW, FALSE);


		result->BidPrice = ATRealPrice(pbData, AT_BID, TRUE);
		result->AskPrice = ATRealPrice(pbData, AT_ASK, TRUE);

		result->BidSize = (long) ATRealPrice(pbData, AT_BIDSIZE, TRUE);
		result->AskSize = (long) ATRealPrice(pbData, AT_ASKSIZE, TRUE);

		result->NetChange = ATRealPrice(pbData, AT_NETCHANGE, TRUE);
	
		result->Currency = SysAllocString(L"USD");
		result->UpdateDateTime = eg_lib::vt_date::GetCurrentDate();
		result->OpenInterest = ATRealPrice(pbData, AT_OPENINTEREST, FALSE);
	
//		result->Exchange = GetMTStockExchange(pbData->atheader.sym.exchange).copy();
		result->LotSize = 1;

		if( fEQZero(result->LastPrice) && fEQZero(result->BidPrice) && fEQZero(result->AskPrice))
			result->LastPrice = ATRealPrice(pbData, AT_CLOSE, FALSE);;



		CComVariant vtResp;
		result.CopyTo(vtResp);
		PublicResponce(_enRequestGrpStocks, request, vtResp);

	}
	catch(...)
	{

	}
}


HRESULT CATFProvider::OnStock(const CComVariant &varParams,const  CComVariant &varResults)
{
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

	if(FAILED(hr)) return hr;
	return varResult.scode;
	return S_OK;
}
HRESULT CATFProvider::OnOption(const CComVariant &varParams,const  CComVariant &varResults, const CComVariant &varIsLastOption)
{
	HRESULT  hr = S_OK ;
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
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
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
HRESULT CATFProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
			RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{

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

	if(FAILED(hr)) return hr;
	return varResult.scode;
}


// CATFProvider

STDMETHODIMP CATFProvider::Connect(void)
{
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}

STDMETHODIMP CATFProvider::Disconnect(void)
{
	try 
	{
		DisconnectProvider();
	}
	catch(_com_error& err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}

STDMETHODIMP CATFProvider::RequestStock(StockParams* Params)
{
	USES_CONVERSION;
	try
	{
		_StockParams param;
		param.Init();
		if(Params->Exchange)
			param->Exchange = SysAllocString(Params->Exchange);
		else
			param->Exchange = SysAllocString(L"");

		if(Params->Stock)
			param->Stock = SysAllocString(Params->Stock);
		else
			return Error(L"UnderlyingSymbol Field in OptionParams can't be empty", IID_IStructureProvider,E_FAIL);


		CComVariant vr;
		param.CopyTo(vr);
		AddRequest(vr,_enRequestStock);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

STDMETHODIMP CATFProvider::RequestOptions(StockParams* Params, ATProviderRequestType enType)
{
	try
	{
		_StockParams param;
		param.Init();

		CComVariant vt;
		param->Stock = SysAllocString(Params->Stock);
		param->Exchange = SysAllocString(Params->Exchange);
		param.CopyTo(vt);
		if(enAllExchanges == enType)
			AddRequest(vt, _enRequestGrpOptions, enGroupRequestAllExchanges);
		else
			AddRequest(vt, _enRequestGrpOptions);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

STDMETHODIMP CATFProvider::CancelStocks(VARIANT Params)
{
	try
	{
		RemoveRequest(Params, _enRequestStock);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}

STDMETHODIMP CATFProvider::CancelOptions(VARIANT vtParams)
{
	try
	{
		RemoveRequest(vtParams, _enRequestGrpOptions);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
