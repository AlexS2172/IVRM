// IndexCompColl.cpp : Implementation of CIndexCompColl

#include "stdafx.h"
#include "IndexCompColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CIndexCompColl::Add(LONG Key, IIndexCompAtom* Value, IIndexCompAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Index component with the same key is already exists."));
		}

		CComObject<CIndexCompAtom>* pIndexComp=NULL;

		if(!Value)
		{
			_CHK(CComObject<CIndexCompAtom>::CreateInstance(&pIndexComp), _T("Fail to add index component."));
			pIndexComp->AddRef();
			if(FAILED(IIndexCompCollImpl::Add(Key, pIndexComp)))
			{
				pIndexComp->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add index component."));
			}
			pIndexComp->m_nID = Key;
			*pRetVal = pIndexComp;
		}
		else
		{
			_CHK(IIndexCompCollImpl::Add(Key, Value), _T("Fail to add index component."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IIndexCompColl, e.Error());
	}

	return S_OK;
}

