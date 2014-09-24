// MmRvReqColl.cpp : Implementation of CMmRvReqColl
#include "stdafx.h"
#include "MmRvReqColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvReqColl::Add(BSTR Key, IMmRvReqAtom* Value, IMmRvReqAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(CComBSTR(Key)) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Quote request with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CMmRvReqAtom>* pNewVal;

			_CHK(CComObject<CMmRvReqAtom>::CreateInstance(&pNewVal), _T("Fail to add quote request."));
			pNewVal->AddRef();
			if(FAILED(IMmRvReqCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add quote request."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmRvReqCollImpl::Add(Key, Value), _T("Fail to add quote request."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvReqColl, e.Error());
	}

	return S_OK;
}

