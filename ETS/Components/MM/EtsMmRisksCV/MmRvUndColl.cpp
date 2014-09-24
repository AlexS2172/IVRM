// MmRvUndColl.cpp : Implementation of CMmRvUndColl
#include "stdafx.h"
#include "MmRvUndColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndColl::Add(LONG Key, BSTR SortKey, IMmRvUndAtom* Value, IMmRvUndAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));

		_bstr_t bsSortKey(SortKey);
		if(!Value)
		{
			CComObject<CMmRvUndAtom>* pNewVal;

			_CHK(CComObject<CMmRvUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
			pNewVal->AddRef();
			if(FAILED(IMmRvUndCollImpl::Add(Key, bsSortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));
			}
			if(pRetVal)
				*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmRvUndCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add underlying."));
			Value->AddRef();
			if(pRetVal)
				*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndColl, e.Error());
	}

	return S_OK;
}

