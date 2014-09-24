// ISEInfo.cpp : Implementation of CISEInfo
#include "stdafx.h"
#include "ISEProvider.h"
#include "ISEInfo.h"
#include "ISEManager.h"

/////////////////////////////////////////////////////////////////////////////
// CISEInfo

STDMETHODIMP CISEInfo::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IISEInfo
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (ATL::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CISEInfo::Connect()
{
	return ((CISEInfoManagerObject*)&theISEManager)->Connect(this);
}

STDMETHODIMP CISEInfo::Disconnect()
{
	return ((CISEInfoManagerObject*)&theISEManager)->Disconnect(this);
}


HRESULT CISEInfo::OnEvent(const ISEInfoTypeEnum Type,const BSTR bsText)
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
			pvars[1] = Type;
			pvars[0] = bsText;
			DISPPARAMS disp = { pvars, NULL, 2, 0 };
			hr = pDispatch->Invoke(0x1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &disp, &varResult, NULL, NULL);
		}
	}
	delete[] pvars;

	if(FAILED(hr)) return hr;
	return varResult.scode;
}