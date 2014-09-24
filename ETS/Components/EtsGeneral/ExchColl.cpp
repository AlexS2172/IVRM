// ExchColl.cpp : Implementation of CExchColl

#include "stdafx.h"
#include "ExchColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CExchColl::Add(LONG Key, BSTR SortKey, IExchAtom* Value, IExchAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Exchange with the same key is already exists."));


		_bstr_t bsSortKey(SortKey);
		if(!Value)
		{
			CComObject<CExchAtom>* pNewVal;

			_CHK(CComObject<CExchAtom>::CreateInstance(&pNewVal), _T("Fail to add exchange."));
			pNewVal->AddRef();
			if(FAILED(IExchCollImpl::Add(Key, bsSortKey, pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add exchange."));
			}
			pNewVal->m_nID      = Key;
			pNewVal->m_bstrCode = bsSortKey;
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IExchCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add exchange."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IExchColl, e.Error());
	}

	return S_OK;
}

