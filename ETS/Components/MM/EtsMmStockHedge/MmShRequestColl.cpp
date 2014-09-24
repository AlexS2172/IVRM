// MmShRequestColl.cpp : Implementation of CMmShRequestColl

#include "stdafx.h"
#include "MmShRequestAtom.h"
#include "MmShRequestColl.h"


// CMmShRequestColl
STDMETHODIMP CMmShRequestColl::Add(BSTR Key, IMmShRequestAtom* Value, IMmShRequestAtom **pRetVal)
{
	try
	{
		_bstr_t bsKey(Key);
		if(m_coll.find(bsKey) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Value with the same key is already exists."));
		}

		CComObject<CMmShRequestAtom>* pRequestAtom;

		if(!Value)
		{
			_CHK(CComObject<CMmShRequestAtom>::CreateInstance(&pRequestAtom), _T("Fail to add value."));
			pRequestAtom->AddRef();
			if(FAILED(IMmShRequestCollImpl::Add(bsKey, pRequestAtom)))
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
			_CHK(IMmShRequestCollImpl::Add(bsKey, Value), _T("Fail to add value."));
			if(pRetVal)
			{
				Value->AddRef();
				*pRetVal = Value;
			}
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof (IMmShRequestColl), e.Error());
	}

	return S_OK;
}

STDMETHODIMP CMmShRequestColl::AddRequest(BSTR Key, IMmShUndAtom* pUnd, IMmShOptAtom* pOpt, IMmShRequestAtom** pRetVal)
{
	HRESULT hr = S_OK;
	try
	{
		_bstr_t bsKey(Key);
		CComObject<CMmShRequestAtom>* pRequestAtom;
		_CHK(CComObject<CMmShRequestAtom>::CreateInstance(&pRequestAtom), _T("Fail to add value."));
		pRequestAtom->AddRef();
		if(pUnd !=NULL)
			pRequestAtom->putref_Und(pUnd);

		if(pOpt !=NULL)
			pRequestAtom->putref_Opt(pOpt);

		if(FAILED(IMmShRequestCollImpl::Add(bsKey, pRequestAtom)))
		{
			pRequestAtom->Release();
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add value."));
		}
		if(pRetVal)
			*pRetVal = pRequestAtom;
		else
			pRequestAtom->Release();
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), __uuidof (IMmShRequestColl), e.Error());
	}
	return hr;
}
