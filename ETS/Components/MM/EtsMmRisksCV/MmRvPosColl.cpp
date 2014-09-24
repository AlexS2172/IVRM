// MmRvPosColl.cpp : Implementation of CMmRvPosColl
#include "stdafx.h"
#include "MmRvPosColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvPosColl::Add(LONG Key, BSTR SortKey, IMmRvPosAtom* Value, IMmRvPosAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Position with the same key is already exists."));

		if(!Value)
		{
			CComObject<CMmRvPosAtom>* pNewVal;

			_CHK(CComObject<CMmRvPosAtom>::CreateInstance(&pNewVal), _T("Fail to add position."));
			pNewVal->AddRef();
			if(FAILED(IMmRvPosCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add position."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IMmRvPosCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add position."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvPosColl, e.Error());
	}

	return S_OK;
}

