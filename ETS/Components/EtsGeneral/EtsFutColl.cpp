// EtsFutColl.cpp : Implementation of CEtsFutColl
#include "stdafx.h"
#include "EtsFutColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsFutColl::Add(LONG Key, BSTR SortKey, IEtsFutAtom* Value, IEtsFutAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Futures with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsFutAtom>* pNewVal;

			_CHK(CComObject<CEtsFutAtom>::CreateInstance(&pNewVal), _T("Fail to add futures."));
			pNewVal->AddRef();
			if(FAILED(IEtsFutCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add futures."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsFutCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add futures."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutColl, e.Error());
	}

	return S_OK;
}

