// ATFStructureProvider.cpp : Implementation of CATFStructureProvider
#include "stdafx.h"
#include "ATFDataProvider.h"
#include "ATFStructureProvider.h"

/////////////////////////////////////////////////////////////////////////////
// CATFStructureProvider

STDMETHODIMP CATFStructureProvider::RequestStock(StockParams *Params)
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
STDMETHODIMP CATFStructureProvider::CancelStock(VARIANT Params)
{
	try
	{
		RemoveRequest(Params, _enRequestStock);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CATFStructureProvider::RequestOptions(OptionParams *Params)
{
	USES_CONVERSION;
	try
	{ 
		_OptionParams param;
		param.Init();
		if(Params->Exchange)
			param->Exchange = SysAllocString(Params->Exchange);
		else
			param->Exchange = SysAllocString(L"");

		if(Params->UnderlyingSymbol)
			param->UnderlyingSymbol = SysAllocString(Params->UnderlyingSymbol);
		else
			return Error(L"UnderlyingSymbol Field in OptionParams can't be empty", IID_IStructureProvider,E_FAIL);


		CComVariant vr;
		param.CopyTo(vr);
		AddRequest(vr,_enRequestOption);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;

}

STDMETHODIMP CATFStructureProvider::CancelOptions(VARIANT Params)
{
	try
	{
		RemoveRequest(Params, _enRequestOption);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

STDMETHODIMP CATFStructureProvider::Connect()
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
STDMETHODIMP CATFStructureProvider::Disconnect()
{
	DisconnectProvider();
	return S_OK;
}

//-----------------------------------------------------------------------------------------------//
HRESULT CATFStructureProvider::OnStock(const CComVariant &varParams,const CComVariant &varResults)
{
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

	if(FAILED(hr)) return hr;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CATFStructureProvider::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
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
//-----------------------------------------------------------------------------------------------//
HRESULT CATFStructureProvider::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, const CComVariant &varRequest)
{
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

	if(FAILED(hr)) return hr;
	return varResult.scode;
}


void  CATFStructureProvider::PublicOptionData(const CComVariant& request, PB* pData, int lotSize, bool bLast)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	USES_CONVERSION;
	try
	{
		_OptionResults info;
		info.Init();
		if(bLast || !pData)
		{
			info->StrikePrice = 0;
			info->ExpirationDate = 0;
			info->Series = SysAllocString(L"");
			info->Currency = SysAllocString(L"");
			info->LotSize = 0;
			
			info->Exchange = _bstr_t(L"").copy();
			bLast = true;
		}
		else
		{
			if(!pData) return;

			_OptionParams param;
			param.Init(request);

			info->StrikePrice =  ATRealPrice(pData, AT_STRIKE, FALSE );
			info->ExpirationDate = eg_lib::vt_date(pData->atstkopt.exprdate.year,pData->atstkopt.exprdate.month, pData->atstkopt.exprdate.day);
			_bstr_t bsSymbol = ExtractOptionSymbol(pData->atstkopt.sym);

			if(pData->atstkopt.sym.symbol[3]<'M')
				info->OptionType = enCall;
			else
				info->OptionType = enPut;

			info->Series = bsSymbol.copy();
			info->Currency = SysAllocString(L"USD");
			info->LotSize = lotSize;			

//			info->Exchange = GetMTOptionExchange(pData->atstkopt.sym.exchange).copy();
			info->Exchange = ::SysAllocString(param->Exchange);
		}

		CComVariant vtResp;
		info.CopyTo(vtResp);
		PublicResponce(_enRequestOption, request, vtResp, bLast);
	}
	catch(...)
	{
		// Error
	}	
}


void CATFStructureProvider::PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD)
{
	try
	{
		_StockResults info;
		info.Init();

		switch(pFD->equityFP.dividendFrequencyCode)
		{
		case 'Q':
			info->DivFreq  = 4;		break; //Quarterly
		case 'S':
			info->DivFreq  = 2;		break; //Semi-annually
		case 'M':
			info->DivFreq  = 12;	break; //Monthly
		case 'A':
			info->DivFreq  = 1;      break; //Annually
		case 'N':
		default:						   //None
			info->DivFreq  = 0;
		}

		if(0 != info->DivFreq)
		{
			WORD date = pFD->equityFP.prevCashDivDate;
			info->DivLastDate = eg_lib::vt_date((1970 + (date >> 9)), (date & 0x01e0) >> 5, date & 0x001f);

			DWORD  nDecType = pFD->equityFP.dividendPriceType;

			if (nDecType > 20)
				nDecType = 0;

			info->DivAmount = double(pFD->equityFP.prevCashDivAmount)/decTable[nDecType];
		}
		else
		{
			info->DivLastDate = 0.;
			info->DivAmount = 0.;
		}


		info->LotSize = 1;
		info->Exchange = Convert((UCHAR*)&pbData->atstock.sym.exchange,1).copy();

		if(GetPriceBlockFlags(pbData,GBC_ANYSTOCK))
		{
			info->StockType = enStock;
		}
		else
		{
			if(GetPriceBlockFlags(pbData,GBC_INDEX))
			{
				info->StockType = enIndex;
			}
			else
				return;
		}
		
		info->Currency = _bstr_t(L"USD").copy();
		info->Description = Convert((UCHAR*)pFD->equityFP.name, sizeof(pFD->equityFP.name)-1).copy();

		CComVariant vtResp;
		info.CopyTo(vtResp);
		PublicResponce(_enRequestStock, request, vtResp);

	}
	catch(...)
	{
		PublicError(enRequestStock, request, L"Error while getting stock data", enProviderInternalError);

	}

}