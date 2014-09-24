// MyTrackStructureInfo.cpp : Implementation of CMyTrackStructureInfo
#include "stdafx.h"
#include "MyTrackProviders.h"
#include "MyTrackStructureInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CMyTrackStructureInfo
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::Connect()
{
	try
	{
		_Module.GetMyTrackCore ()->Connect (static_cast<CBaseNotifier*>(this)); 
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	return S_OK;
}

//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::Disconnect()
{
	_Module.GetMyTrackCore ()->Disconnect (static_cast<CBaseNotifier*>(this));
	return S_OK;
}

//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::RequestStock(StockParams *Params)
{
	CComVariant varParams;
	try
	{
		USES_CONVERSION;
		ATLTRACE(_T("CMyTrackStructureInfo::RequestStock: %s\n"), W2T(Params->Stock));
		_bstr_t bstrSymbol;
		_StockParams op;
		op.Init();

		op->Stock = SysAllocString(Params->Stock);
		op->Exchange = SysAllocString(Params->Exchange);

		op.CopyTo(varParams);
		bstrSymbol = Params->Stock;
		_Module.GetMyTrackCore ()->RequestStock(static_cast<CBaseNotifier*>(this),
												(LPCSTR)bstrSymbol,
												varParams);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::CancelStock(VARIANT Params)
{
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			_Module.GetMyTrackCore ()->CancelStock (static_cast<CBaseNotifier*>(this),NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				_bstr_t bstrSymbol;
				_StockParams op(Params);
				bstrSymbol = op->Stock;
				_Module.GetMyTrackCore ()->CancelStock (static_cast<CBaseNotifier*>(this),(LPCSTR)bstrSymbol);
				return S_OK;
			}
			return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}
//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::RequestOptions(OptionParams *Params)
{
	CComVariant varParams;
	try
	{
		USES_CONVERSION;
		ATLTRACE(_T("CMyTrackStructureInfo::RequestOptions: %s\n"), W2T(Params->UnderlyingSymbol));
	
		_bstr_t bstrSymbol;
		_OptionParams op;
		op.Init();

		op->UnderlyingSymbol = Params->UnderlyingSymbol;
		op->Exchange = Params->Exchange;
		op.CopyTo (varParams);
		bstrSymbol = Params->UnderlyingSymbol;
		_Module.GetMyTrackCore ()->RequestOptions(static_cast<CBaseNotifier*>(this),
													(LPCSTR)bstrSymbol,
													varParams);
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	varParams.Clear ();
	return S_OK;
}

//-----------------------------------------------------------------------------------------------//
STDMETHODIMP CMyTrackStructureInfo::CancelOptions(VARIANT Params)
{
	try
	{
		if (V_ERROR(&Params) == DISP_E_PARAMNOTFOUND)
		{
			_Module.GetMyTrackCore ()->CancelOptions(static_cast<CBaseNotifier*>(this),NULL);
			return S_OK;
		}
		else
			if (V_RECORD(&Params))
			{
				_bstr_t bstrSymbol;
				_OptionParams op(Params);
				bstrSymbol = op->UnderlyingSymbol;

				_Module.GetMyTrackCore ()->CancelOptions(static_cast<CBaseNotifier*>(this),(LPCSTR)bstrSymbol);
				return S_OK;
			}
			return DISP_E_BADVARTYPE;
	}
	catch (_com_error &err)
	{
		return eg_lib::utils::ComError2ErrInfo (err,this);
	}
	catch (...)
	{
		return DISP_E_BADVARTYPE;
	}
}

//-----------------------------------------------------------------------------------------------//
HRESULT CMyTrackStructureInfo::OnStock(CComVariant &varParams, CComVariant &varResults)
{
	ATLTRACE(_T("CMyTrackStructureInfo::OnStock\n"));
	
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
HRESULT CMyTrackStructureInfo::OnOption(CComVariant &varParams, CComVariant &varResults, CComVariant &varIsLastOption)
{
	ATLTRACE(_T("CMyTrackStructureInfo::OnOption\n"));

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
HRESULT CMyTrackStructureInfo::OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription, 
									RequestsTypeEnum enumRequest, CComVariant &varRequest)
{
	ATLTRACE(_T("CMyTrackStructureInfo::OnError\n"));

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


STDMETHODIMP CMyTrackStructureInfo::get_PriceProviderMode(PriceProviderTypeEnum *pVal)
{
	if(!pVal) return E_POINTER;

	m_enMode = _Module.GetMyTrackCore ()->GetPriceProviderMode();
	*pVal = m_enMode;
	
	return S_OK;
}

STDMETHODIMP CMyTrackStructureInfo::put_PriceProviderMode(PriceProviderTypeEnum newVal)
{
	if(newVal != m_enMode)
	{
		m_enMode = newVal;
		_Module.GetMyTrackCore ()->SetPriceProviderMode(m_enMode);
	}
	return S_OK;
}
