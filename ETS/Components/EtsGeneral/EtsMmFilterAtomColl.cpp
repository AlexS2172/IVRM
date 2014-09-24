// EtsMmFilterAtomColl.cpp : Implementation of CEtsMmFilterAtomColl

#include "stdafx.h"
#include "EtsMmFilterAtomColl.h"
#include "EtsMmFilterAtom.h"
                      
// CEtsMmFilterAtomColl
STDMETHODIMP CEtsMmFilterAtomColl::Add(BSTR Key, IEtsMmFilterAtom* Value, IEtsMmFilterAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);
	_bstr_t bsKey(Key);

	try
	{
		if(m_coll.find(bsKey) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Filter component with the same key is already exists."));

		CComObject<CEtsMmFilterAtom>* pFilterComp;

		if(!Value)
		{
			_CHK(CComObject<CEtsMmFilterAtom>::CreateInstance(&pFilterComp), _T("Fail to add Filter component."));
			IEtsMmFilterAtomPtr	spFilterComp;
			spFilterComp.Attach(pFilterComp, TRUE);
			
			if(FAILED(IEtsMmFilterAtomCollImpl::Add(bsKey, pFilterComp)))
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Filter component."));

			*pRetVal = spFilterComp.Detach();
		}
		else
		{
			_CHK(IEtsMmFilterAtomCollImpl::Add(bsKey, Value), _T("Fail to add Filter component."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMmFilterAtomColl, e.Error());
	}

	return S_OK;
}

