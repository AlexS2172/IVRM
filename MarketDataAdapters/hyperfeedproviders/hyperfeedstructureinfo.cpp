// HyperFeedStructureInfo.cpp : Implementation of CHyperFeedStructureInfo
#include "stdafx.h"
#include "HyperFeedStructureInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureInfo
STDMETHODIMP CHyperFeedStructureInfo::Connect()
{
	try 
	{
		ConnectProvider();
	}
	catch(_com_error& err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureInfo::Disconnect()
{
	Cancel();
	CMasterOptions::CancelRequest();
	COptions::CancelRequest();
	DisconnectProvider();
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
// CHyperFeedStructureInfo
STDMETHODIMP CHyperFeedStructureInfo::RequestStock(StockParams *Params)
{
	try
	{
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new CStockRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureInfo::CancelStock(VARIANT Params)
{
	try
	{
		Cancel();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureInfo::RequestOptions(OptionParams *Params)
{
	try
	{ 
		CRequestBasePtr pRequest = CRequestBasePtr((CRequestBase*)new COptionRequest(Params));
		AddRequest(pRequest);
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}
STDMETHODIMP CHyperFeedStructureInfo::CancelOptions(VARIANT Params)
{
	try
	{
		Cancel();
		COptions::CancelRequest();
		CMasterOptions::CancelRequest();
		if(V_VT(&Params) == VT_ERROR)
			RemoveAllRequest();
	}
	catch (_com_error &err)
	{
		return EgLib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
	return S_OK;
}

void CHyperFeedStructureInfo::Attach()
{
	long nRes = CMasterOptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		Sleep(500);
		nRes = CMasterOptions::Attach();
		if(nRes != DBA_ERR_NO_ERROR)
		{
			HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
			EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to master options database."));
		}
	}
	nRes = COptions::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to price database."));
	}
	nRes = CUnderlyings::Attach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to attach to security profile database."));
	}	
}

void CHyperFeedStructureInfo::Detach()
{
	long nRes = CMasterOptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from price database."));
	}
	nRes = COptions::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from master options database."));
	}
	nRes = CUnderlyings::Detach();
	if(nRes != DBA_ERR_NO_ERROR)
	{
		HRESULT hr = MAKE_HRESULT (SEVERITY_ERROR, FACILITY_ITF, nRes);
		EgLib::CComErrorWrapper::ThrowError(hr, _T("Failed to detach from security profile database."));
	}
}

void CHyperFeedStructureInfo::OnResponse(CResponseBasePtr pResponse)
{
	
	switch(pResponse->m_enType)
	{
	case _enResponseStock:
		{
			CStockInfoResponse* pStockResponse = dynamic_cast<CStockInfoResponse*>(pResponse.get());
			if(pStockResponse)
				OnStock(pStockResponse->m_vtParams, pStockResponse->m_vtInfo);
		}
		break;
	case _enResponseOption:
		{
			COptionInfoResponse* pOptionResponse = dynamic_cast<COptionInfoResponse*>(pResponse.get());
			if(pOptionResponse)
				OnOption(pOptionResponse->m_vtParams, pOptionResponse->m_vtInfo, CComVariant(pOptionResponse->m_bIsLast));
		}
		break;
	}
}

void CHyperFeedStructureInfo::OnRequest(CRequestBasePtr pRequest)
{
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("OnRequest Enter"));	
	switch(pRequest->m_enType)
	{
	case _enRequestStock:
		{
			CStockRequest* pStockRequest = dynamic_cast<CStockRequest*>(pRequest.get());
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Requesting Stock Start"));	
			if(pStockRequest)
			{
				EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Get stock info call"));	
				GetStockInfo(pStockRequest->m_recParams);
			}
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Requesting Stock Done: %s"), _T(pStockRequest->m_recParams->Stock));	
		}
		break;
	case _enRequestOption:
		{
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Requesting Option Start"));	
			
			COptionRequest* pOptionRequest = dynamic_cast<COptionRequest*>(pRequest.get());

			if(pOptionRequest)
			{
				pOptionRequest->m_recParams.CopyTo(m_vtCurrentRequest);
//				if(!IsCanceled())
//				{
//					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Requesting Option Fundamental Call: %s"), _T(pOptionRequest->m_recParams->UnderlyingSymbol));	
//					LoadOptionFundamentalInfo(pOptionRequest->m_recParams->UnderlyingSymbol);
//				}
					
				if(!IsCanceled())
				{
					EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("GetOptions Call: %s"), _T(pOptionRequest->m_recParams->UnderlyingSymbol));	
					GetOptions(pOptionRequest->m_recParams->UnderlyingSymbol);
				}
				
			}
			EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("Requesting Option Done"));	
		}
		break;
	}
	EgLib::CEgLibTraceManager::Trace(LogSubs, __FUNCTION__ , _T("OnRequest Exit"));	
}




//-----------------------------------------------------------------------------------------------//
HRESULT CHyperFeedStructureInfo::OnStock(const CComVariant &varParams,const  CComVariant &varResults)
{
	ATLTRACE(_T("CHyperFeedStructureInfo::OnStock\n"));
	
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
HRESULT CHyperFeedStructureInfo::OnOption(const CComVariant &varParams,const CComVariant &varResults,const CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CHyperFeedStructureInfo::OnOption\n"));

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
			pDispatch->Invoke(0x2, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;
	return varResult.scode;
}
void CHyperFeedStructureInfo::PublicGetOptionError(ErrorNumberEnum nError, BSTR bsDescription)
{
	CErrorResponse* pError = new CErrorResponse();
	pError->m_enRequestType = enRequestOptions;
	pError->m_vtRequest = m_vtCurrentRequest;
	pError->m_bstrDescription = bsDescription;
	pError->m_Error = nError;

	CResponseBasePtr pErrorPtr = CResponseBasePtr((CResponseBase*)pError);
	PublicResponse(pErrorPtr);
}

void CHyperFeedStructureInfo::PublicLastOption()
{
	_OptionInfo info;
	info.Init();
	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionInfoResponse(m_vtCurrentRequest,info, true));
	PublicResponse(pOption);
}

void CHyperFeedStructureInfo::PublicOption(DBA_OPTIONS_FILTER_RECORD* pData)
{
	_OptionInfo info;
	info.Init();
	CopyOptionFilterToInfo(*pData, info);
	CopyFundamentalInfoToInfo(info);

	char szName[MAX_TICKER + 1];
	ZeroMemory(szName,sizeof(szName));
	CreateSz(szName, (const char*)pData->priceRec.dbaRec.item.symbol, MAX_TICKER);
	EgLib::CEgLibTraceManager::Trace(LogInfo, __FUNCTION__,"%s Strike %f", szName, info->StrikePrice);

	CResponseBasePtr pOption = CResponseBasePtr((CResponseBase*)new COptionInfoResponse(m_vtCurrentRequest, info, false));
	PublicResponse(pOption);
	if(info->Series)
	{
		SysFreeString(info->Series);
		info->Series = NULL;
	}
	if(info->Currency)
	{
		SysFreeString(info->Currency);
		info->Currency = NULL;
	}
}
