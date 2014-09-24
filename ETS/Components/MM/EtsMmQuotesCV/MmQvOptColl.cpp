// MmQvOptColl.cpp : Implementation of CMmQvOptColl

#include "stdafx.h"
#include "MmQvOptColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmQvOptColl::Add(LONG Key, IMmQvOptAtom* Value, IMmQvOptAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option with the same key is already exists."));
		}

		CComObject<CMmQvOptAtom>* pValue;

		if(!Value)
		{
			_CHK(CComObject<CMmQvOptAtom>::CreateInstance(&pValue), _T("Fail to add option."));
			pValue->AddRef();
			if(FAILED(IMmQvOptCollImpl::Add(Key, pValue)))
			{
				pValue->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option."));
			}
			*pRetVal = pValue;
		}
		else
		{
			_CHK(IMmQvOptCollImpl::Add(Key, Value), _T("Fail to add option."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmQvOptColl, e.Error());
	}

	return S_OK;
}

