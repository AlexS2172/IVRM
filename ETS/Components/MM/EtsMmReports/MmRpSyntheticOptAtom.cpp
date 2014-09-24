// MmRpSyntheticOptAtom.cpp : Implementation of CMmRpSyntheticOptAtom

#include "stdafx.h"
#include "MmRpSyntheticOptAtom.h"


// CMmRpSyntheticOptAtom

STDMETHODIMP CMmRpSyntheticOptAtom::Clone(IMmRpSyntheticOptAtom** ppRetVal)
{
	__CHECK_POINTER(ppRetVal);
	HRESULT hr = S_OK;

	try
	{
		if(*ppRetVal)
		{
			(*ppRetVal)->Release();
			*ppRetVal = 0;
		}
	
		IMmRpSyntheticOptAtomPtr spSyntheticOptAtom;

		__CHECK_HRESULT(spSyntheticOptAtom.CreateInstance(CLSID_MmRpSyntheticOptAtom), 
			_T("Failed to create MmRpSyntheticOptAtom object."));

		__CHECK_HRESULT3(spSyntheticOptAtom->put_Delta(m_dDelta))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_Expiry(m_dtExpiry))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_Gamma(m_dGamma))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_ID(m_nID))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_NetPos(m_dNetPos))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_OTMDelta(m_dOTMDelta))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_RootID(m_nRootID))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_RootSymbol(m_bstrRootSymbol))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_StockID(m_nStockID))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_Strike(m_dStrike))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_Theta(m_dTheta))
		__CHECK_HRESULT3(spSyntheticOptAtom->put_Vega(m_dVega))

		*ppRetVal = spSyntheticOptAtom.Detach();
	}
	catch(const _com_error& e)
	{
		hr = Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRpSyntheticOptAtom, e.Error());
	}

	return hr;
}

STDMETHODIMP CMmRpSyntheticOptAtom::ClearValues(VARIANT_BOOL DataOnly)
{
	__MmRpSyntheticOptAtom::ClearValues(DataOnly);
	return S_OK;
}