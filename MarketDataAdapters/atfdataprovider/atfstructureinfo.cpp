// ATFStructureInfo.cpp : Implementation of CATFStructureInfo
#include "stdafx.h"
#include "ATFDataProvider.h"
#include "ATFStructureInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CATFStructureInfo
STDMETHODIMP CATFStructureInfo::RequestStock(StockParams *Params)
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
STDMETHODIMP CATFStructureInfo::CancelStock(VARIANT Params)
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
STDMETHODIMP CATFStructureInfo::RequestOptions(OptionParams *Params)
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
STDMETHODIMP CATFStructureInfo::CancelOptions(VARIANT Params)
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
STDMETHODIMP CATFStructureInfo::Connect()
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
STDMETHODIMP CATFStructureInfo::Disconnect()
{
	DisconnectProvider();
	return S_OK;
}


//-----------------------------------------------------------------------------------------------//

HRESULT CATFStructureInfo::OnStock(const CComVariant &varParams,const  CComVariant &varResults)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	
	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[2];
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
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CATFStructureInfo::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));

	CComVariant varResult;
	int nConnectionIndex;
	CComVariant* pvars = new CComVariant[3];
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
			pvars[2] = varParams;
			pvars[1] = varResults;
			pvars[0] = varIsLastOption;
			DISPPARAMS disp = { pvars, NULL, 3, 0 };
			hr = pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}
//-----------------------------------------------------------------------------------------------//
HRESULT CATFStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest,const CComVariant &varRequest)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));

	CComVariant varResult;
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
			pDispatch->Invoke(0x3, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}


void  CATFStructureInfo::PublicOptionData(const CComVariant& request, PB* pData,int LotSize, bool bLast)
{
	ATFPROVTRACE(__FUNCTION__, _T("Start"));
	USES_CONVERSION;
	try
	{
		_OptionInfo info;
		info.Init();
		if(bLast || !pData)
		{
			info->StrikePrice = 0;
			info->ExpirationDate = 0;
			info->Series = SysAllocString(L"");
			info->Currency = SysAllocString(L"");
			info->LotSize = 0;
			
			info->Exchange = SysAllocString(L"");

			info->Ask = 0;
			info->Bid = 0;
			info->Last = 0;
			info->OpenInterest = 0;
			info->Volume = 0;
			info->YVolume = 0;
			bLast = true;
		}
		else
		{
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
			info->LotSize = LotSize;
			
//			info->Exchange = GetMTOptionExchange(pData->atstkopt.sym.exchange).copy();
			info->Exchange = ::SysAllocString(param->Exchange);

			info->Ask = ATRealPrice(pData, AT_ASK, FALSE);
			info->Bid = ATRealPrice(pData, AT_BID, FALSE);

			double d1 = ATRealPrice(pData, AT_BIDSIZE, TRUE);
			double d2 = ATRealPrice(pData, AT_ASKSIZE, TRUE);

			info->Last = ATRealPrice(pData, AT_LAST, FALSE);
			if(info->Last<0.001)
				info->Last = ATRealPrice(pData, AT_CLOSE, FALSE);
			info->OpenInterest = pData->atstkopt.openint;

			info->Volume = pData->atstkopt.volume;
			info->YVolume = pData->atstkopt.yVolume;
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

void CATFStructureInfo::PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD)
{
	try
	{
		_StockInfo info;
		info.Init();

		_StockParams param;
		param.Init(request);

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

			info->DivAmount = 1.*pFD->equityFP.prevCashDivAmount/decTable[nDecType];
		}
		else
		{
			info->DivLastDate = 0.;
			info->DivAmount = 0.;
		}


		info->LotSize = 1;
//		info->Exchange = GetMTStockExchange(pbData->atstock.sym.exchange).copy();
		info->Exchange = ::SysAllocString(param->Exchange);

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


		info->Ask = ATRealPrice(pbData, AT_ASK, FALSE);
		info->Bid = ATRealPrice(pbData, AT_BID, FALSE);
		info->OpenPrice = ATRealPrice(pbData, AT_OPEN, FALSE);


		 info->Volume		   = pbData->atstock.volume;
		 info->CompositeVolume = pbData->atstock.volume;
		 info->ClosePrice      = ATRealPrice(pbData, AT_CLOSE,FALSE);

		DWORD nDecType = pFD->equityFP.highLowPriceType;

		if (nDecType > 20)
			nDecType = 0;
		 
		 info->High            = pFD->equityFP.highPrice/decTable[nDecType];
         info->Low             = pFD->equityFP.lowPrice/decTable[nDecType];


		CComVariant vtResp;
		info.CopyTo(vtResp);
		PublicResponce(_enRequestStock, request, vtResp);

	}
	catch(...)
	{
		PublicError(enRequestStock, request, L"Error while getting stock data", enProviderInternalError);

	}

}