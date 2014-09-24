// MmIhRequestColl.cpp : Implementation of CMmIhRequestColl

#include "stdafx.h"
#include "MmIhRequestAtom.h"
#include "MmIhRequestColl.h"


// CMmIhRequestColl
STDMETHODIMP CMmIhRequestColl::Add (BSTR Key, IMmIhRequestAtom *Value ,IMmIhRequestAtom **pRetVal )
{
	try
	{
		_bstr_t bsKey(Key);
		if(m_coll.find(bsKey) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key is already exists."));
		}

		CComObject<CMmIhRequestAtom>* pRequestAtom;

		if(!Value)
		{
			_CHK(CComObject<CMmIhRequestAtom>::CreateInstance(&pRequestAtom), _T("Fail to add value."));
			pRequestAtom->AddRef();
			if(FAILED(IMmIhRequestCollImpl::Add(bsKey, pRequestAtom)))
			{
				pRequestAtom->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add value."));
			}
			if(pRetVal)
				*pRetVal = pRequestAtom;
			else
				pRequestAtom->Release();
		}
		else
		{
			_CHK(IMmIhRequestCollImpl::Add(bsKey, Value), _T("Fail to add value."));
			if(pRetVal)
			{
				Value->AddRef();
				*pRetVal = Value;
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof (IMmIhRequestColl), e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmIhRequestColl::AddRequest(BSTR Key, IMmIhUndAtom* pUnd, IMmIhIndexAtom* pIdx, IMmIhOptAtom* pOpt, IMmIhRequestAtom** pRetVal)
{
	try
	{
		_bstr_t bsKey(Key);
		CComObject<CMmIhRequestAtom>* pRequestAtom;
		_CHK(CComObject<CMmIhRequestAtom>::CreateInstance(&pRequestAtom), _T("Fail to add value."));
		pRequestAtom->AddRef();
		if(pUnd !=NULL)
			pRequestAtom->putref_Und(pUnd);

		if(pIdx != NULL)
			pRequestAtom->putref_Idx(pIdx);

		if(pOpt !=NULL)
			pRequestAtom->putref_Opt(pOpt);

		if(FAILED(IMmIhRequestCollImpl::Add(bsKey, pRequestAtom)))
		{
			pRequestAtom->Release();
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add value."));
		}
		if(pRetVal)
			*pRetVal = pRequestAtom;
		else
			pRequestAtom->Release();
	}
	catch (_com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof (IMmIhRequestColl), e.Error());
	}
	return S_OK;
}

