// EtsFutOptColl.cpp : Implementation of CEtsFutOptColl
#include "stdafx.h"
#include "EtsFutOptColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsFutOptColl::Add(LONG Key, BSTR SortKey, IEtsFutOptAtom* Value, IEtsFutOptAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Futures option with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsFutOptAtom>* pNewVal;
			_CHK(CComObject<CEtsFutOptAtom>::CreateInstance(&pNewVal), _T("Fail to add futures option."));
			pNewVal->AddRef();
			if(FAILED(IEtsFutOptCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add futures option."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsFutOptCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add futures option."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutOptColl, e.Error());
	}

	return S_OK;
}

