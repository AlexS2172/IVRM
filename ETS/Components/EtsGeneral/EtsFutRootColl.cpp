// EtsFutRootColl.cpp : Implementation of CEtsFutRootColl
#include "stdafx.h"
#include "EtsFutRootColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsFutRootColl::Add(LONG Key, BSTR SortKey, IEtsFutRootAtom* Value, IEtsFutRootAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Futures root with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsFutRootAtom>* pNewVal;

			_CHK(CComObject<CEtsFutRootAtom>::CreateInstance(&pNewVal), _T("Fail to add futures root."));
			pNewVal->AddRef();
			if(FAILED(IEtsFutRootCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add futures root."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsFutRootCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add futures root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsFutRootColl, e.Error());
	}

	return S_OK;
}

