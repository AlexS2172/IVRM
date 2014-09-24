// EtsUndGroupAtom.cpp : Implementation of CEtsUndGroupAtom

#include "stdafx.h"
#include "EtsUndGroupAtom.h"

//-----------------------------------------------------------------------//
STDMETHODIMP CEtsUndGroupAtom::get_Ratio(GroupRatioEnum grIdx, DOUBLE* pVal)
{
	if (pVal == NULL) return E_INVALIDARG;

	try{
		long lIdx = static_cast<long>(grIdx);
		*pVal = (this->operator [](lIdx));
	}
	catch (std::exception){
		return E_INVALIDARG;
	}
	return S_OK;
};
//-----------------------------------------------------------------------//
STDMETHODIMP CEtsUndGroupAtom::put_Ratio(GroupRatioEnum grIdx, DOUBLE newVal)
{
	try {
		long lIdx = static_cast<long>(grIdx);
		(this->operator [](lIdx)) = newVal;
	}
	catch (std::exception){
		return E_INVALIDARG;
	};
	return S_OK;
};
//-----------------------------------------------------------------------//
// CEtsUndGroupAtom

