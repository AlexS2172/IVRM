// SynthRootColl.cpp : Implementation of CSynthRootColl

#include "stdafx.h"
#include "SynthRootColl.h"


// CSynthRootColl

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CSynthRootColl::Add(LONG Key, ISynthRootAtom* Value, ISynthRootAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic root with the same key is already exists."));
		}

		CComObject<CSynthRootAtom>* pSynthRoot;

		if(!Value)
		{
			_CHK(CComObject<CSynthRootAtom>::CreateInstance(&pSynthRoot), _T("Fail to add synthetic root."));
			pSynthRoot->AddRef();
			if(FAILED(ISynthRootCollImpl::Add(Key, pSynthRoot)))
			{
				pSynthRoot->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add synthetic roo."));
			}
			*pRetVal = pSynthRoot;
		}
		else
		{
			_CHK(ISynthRootCollImpl::Add(Key, Value), _T("Fail to add synthetic roo."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_ISynthRootColl, e.Error());
	}

	return S_OK;
}

