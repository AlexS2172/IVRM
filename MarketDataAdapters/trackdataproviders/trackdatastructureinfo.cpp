// TrackDataStructureInfo.cpp : Implementation of CTrackDataStructureInfo
#include "stdafx.h"
#include "TrackDataProviders.h"
#include "TrackDataStructureInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CTrackDataStructureInfo

STDMETHODIMP CTrackDataStructureInfo::RequestStock(StockParams *Params)
{
	CComVariant varParams;
	USES_CONVERSION;
	try
	{

		RT_SYMBOL symbol;
		ZeroMemory(&symbol, sizeof(RT_SYMBOL)); 
		if(Params->Stock)
			strncpy(symbol.name,W2A(Params->Stock),10);

		if(Params->Exchange)
			strncpy(symbol.exchange, W2A(Params->Exchange),2);

		_StockParams op;
		op.Init();
		op->Stock = Params->Stock;
		op->Exchange = Params->Exchange;
		op.CopyTo(varParams);


		if(RT_ERR_OK == m_Client.RequestSymbol(this, &symbol, varParams))
		{
		}
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
STDMETHODIMP CTrackDataStructureInfo::CancelStock(VARIANT Params)
{
	m_Client.CancelStocks();
	return S_OK;
}
STDMETHODIMP CTrackDataStructureInfo::RequestOptions(OptionParams *Params)
{
	CComVariant varParams;
	USES_CONVERSION;
	try
	{ 

		_OptionParams op;
		op.Init();
		op->UnderlyingSymbol = SysAllocString(Params->UnderlyingSymbol);
		op->Exchange = SysAllocString(Params->Exchange);
		op.CopyTo(varParams);

		RT_SYMBOL symbol;
		ZeroMemory(&symbol, sizeof(RT_SYMBOL)); 
		if(Params->UnderlyingSymbol)
			strncpy(symbol.name,W2A(Params->UnderlyingSymbol),10);

		if(Params->Exchange)
			strncpy(symbol.exchange, W2A(Params->Exchange),2);

		m_Client.RequestOptions(this, &symbol, varParams);
	}
	catch (_com_error &err)
	{
		return utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;

}
STDMETHODIMP CTrackDataStructureInfo::CancelOptions(VARIANT Params)
{
	m_Client.CancelOptions();
	return S_OK;

}
STDMETHODIMP CTrackDataStructureInfo::Connect()
{
	try 
	{
		m_Client.Connect(this);
	}
	catch(_com_error& err)
	{
		return utils::ComError2ErrInfo (err,this);		
	}
	return S_OK;
}
STDMETHODIMP CTrackDataStructureInfo::Disconnect()
{
	m_Client.Disconnect(this);
	return S_OK;
}



//-----------------------------------------------------------------------------------------------//
HRESULT CTrackDataStructureInfo::OnStock(CComVariant &varParams, CComVariant &varResults)
{
	ATLTRACE(_T("CTrackDataStructureInfo::OnStock\n"));
	
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
HRESULT CTrackDataStructureInfo::OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CTrackDataStructureInfo::OnOption\n"));

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
//-----------------------------------------------------------------------------------------------//
HRESULT CTrackDataStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, CComVariant &varRequest)
{
	ATLTRACE(_T("CTrackDataStructureInfo::OnError\n"));

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
