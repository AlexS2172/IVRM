// BorrowingRatesCurves.cpp : Implementation of CBorrowingRatesCurves

#include "stdafx.h"
#include "BorrowingRatesCurves.h"

//------------------------------------------------------------------------------------------------------//
// CBorrowingRatesCurves
//------------------------------------------------------------------------------------------------------//
STDMETHODIMP CBorrowingRatesCurves::Add(LONG Key, IEtsRateColl* Value, IEtsRateColl** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Rate component with the same key is already exists."));


		if(!Value)
			*pRetVal = AddNew(Key).Detach();
		else
		{
			_CHK(IBorrowingRatesCurvesImpl::Add(Key, Value), _T("Fail to add rate."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsRateColl, e.Error());
	}

	return S_OK;
};
//------------------------------------------------------------------------------------------------------//
IEtsRateCollPtr CBorrowingRatesCurves::AddNew(LONG Key, CComObject<CEtsRateColl>** pAtom)
{
	CComObject<CEtsRateColl>* pNewVal;
	IEtsRateCollPtr spRet;

	_CHK(CComObject<CEtsRateColl>::CreateInstance(&pNewVal), _T("Fail to create rate curve."));
	spRet.Attach(pNewVal, TRUE);

	if(FAILED(IBorrowingRatesCurvesImpl::Add(Key, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add rate curve."));

	if(pAtom)
		*pAtom = pNewVal;
	return spRet;
}
//------------------------------------------------------------------------------------------------------//
