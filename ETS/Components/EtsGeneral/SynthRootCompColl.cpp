// SynthRootCompColl.cpp : Implementation of CSynthRootCompColl

#include "stdafx.h"
#include "SynthRootCompColl.h"


// CSynthRootCompColl

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CSynthRootCompColl::Add(LONG Key, ISynthRootCompAtom* Value, ISynthRootCompAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic Root component with the same key is already exists."));
		}

		CComObject<CSynthRootCompAtom>* pSynthRootComp;

		if(!Value)
		{
			_CHK(CComObject<CSynthRootCompAtom>::CreateInstance(&pSynthRootComp), _T("Fail to add Synthetic Root component."));
			pSynthRootComp->AddRef();
			if(FAILED(ISynthRootCompCollImpl::Add(Key, pSynthRootComp)))
			{
				pSynthRootComp->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Synthetic Root component."));
			}
			*pRetVal = pSynthRootComp;
		}
		else
		{
			_CHK(ISynthRootCompCollImpl::Add(Key, Value), _T("Fail to add Synthetic Root component."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISynthRootCompColl, e.Error());
	}

	return S_OK;
}

