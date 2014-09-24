// EtsContractBySymColl.cpp : Implementation of CEtsContractBySymColl
#include "stdafx.h"
#include "EtsContractBySymColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsContractBySymColl::Add(BSTR Key, IEtsContractAtom* Value, IEtsContractAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(CComBSTR(Key)) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Contract with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsContractAtom>* pNewVal;

			_CHK(CComObject<CEtsContractAtom>::CreateInstance(&pNewVal), _T("Fail to add contract."));
			pNewVal->AddRef();
			if(FAILED(IEtsContractBySymCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add contract."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsContractBySymCollImpl::Add(Key, Value), _T("Fail to add contract."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsContractBySymColl, e.Error());
	}

	return S_OK;
}

