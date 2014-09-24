// FixProviderSpecifics.cpp : Implementation of CFixProviderSpecifics
#include "stdafx.h"
#include "FixProviders.h"
#include "FixProviderSpecifics.h"

/////////////////////////////////////////////////////////////////////////////
// CFixProviderSpecifics

STDMETHODIMP CFixProviderSpecifics::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IFixProviderSpecifics
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}

	return S_FALSE;
}

STDMETHODIMP CFixProviderSpecifics::GetSpecifics(	/*[in, out]*/ long* FixProviderID,
												/*[in, out]*/ BSTR* ProgId,
												/*[in, out]*/ BSTR* Description,
												/*[in, out]*/ VARIANT_BOOL * LoginNeeded,
												/*[in, out]*/ BSTR* UserAccount,
												/*[in, out]*/ BSTR* UserPassword)
{
	BSTR bsBuffer;

	ProgIDFromCLSID(m_clsidFixProvider, &bsBuffer);
	*FixProviderID = m_lID;
	*ProgId = bstr_t(bsBuffer).copy();
	*Description = m_bsDescription.copy();

	CoTaskMemFree(bsBuffer);

	return S_OK;
}

STDMETHODIMP CFixProviderSpecifics::SetSpecifics(	/*[in, out]*/ long* FixProviderID,
												/*[in, out]*/ BSTR* ProgId,
												/*[in, out]*/ BSTR* Description,
												/*[in, out]*/ VARIANT_BOOL * LoginNeeded,
												/*[in, out]*/ BSTR* UserAccount,
												/*[in, out]*/ BSTR* UserPassword)
{
	return E_NOTIMPL;
}
