// EtsMmEntityAtomColl.cpp : Implementation of CEtsMmEntityAtomColl

#include "stdafx.h"
#include "EtsMmEntityAtomColl.h"
#include "EtsMmEntityAtom.h"


// CEtsMmEntityAtomColl
STDMETHODIMP CEtsMmEntityAtomColl::Add(BSTR Key, IEtsMmEntityAtom* Value, IEtsMmEntityAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);
	_bstr_t bsKey(Key);

	try
	{
		if(m_coll.find(bsKey) != m_coll.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Filter component with the same key is already exists."));

		CComObject<CEtsMmEntityAtom>* pEntityComp = NULL;

		if(!Value)
		{
			_CHK(CComObject<CEtsMmEntityAtom>::CreateInstance(&pEntityComp), _T("Fail to add Entity component."));

			IEtsMmEntityAtomPtr	spEntityComp;
			spEntityComp.Attach(pEntityComp, TRUE);

			if(FAILED(IEtsMmEntityAtomCollImpl::Add(bsKey, pEntityComp)))
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add Entity component."));

			*pRetVal = spEntityComp.Detach();
		}
		else
		{
			_CHK(IEtsMmEntityAtomCollImpl::Add(bsKey, Value), _T("Fail to add Entity component."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsMmEntityAtomColl, e.Error());
	}

	return S_OK;


}
