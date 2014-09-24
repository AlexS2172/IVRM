// UndBySymColl.cpp : Implementation of CUndBySymColl

#include "stdafx.h"
#include "UndBySymColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CUndBySymColl::Add(BSTR Key, IUndAtom* Value, IUndAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(CComBSTR(Key)) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CUndAtom>* pNewVal;

			_CHK(CComObject<CUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
			pNewVal->AddRef();
			if(FAILED(IUndBySymCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IUndBySymCollImpl::Add(Key, Value), _T("Fail to add underlying."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IUndBySymColl, e.Error());
	}

	return S_OK;
}

