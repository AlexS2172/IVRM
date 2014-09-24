// MmRvUndColl.cpp : Implementation of CMmRvUndColl
#include "stdafx.h"
#include "MmRvUndColl.h"

IMmRvUndAtomPtr CMmRvUndColl::AddNew(long lUndId, _bstr_t bsSymbol, CComObject<CMmRvUndAtom>** pObject)
{
	IMmRvUndAtomPtr spAtom;
	CComObject<CMmRvUndAtom>* pNewVal = NULL;
	_CHK(CComObject<CMmRvUndAtom>::CreateInstance(&pNewVal), _T("Fail to add underlying."));
	spAtom.Attach(pNewVal, TRUE);

	if(FAILED(IMmRvUndCollImpl::Add(lUndId, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add underlying."));

	pNewVal->m_nID = lUndId;
	pNewVal->m_bstrSymbol = bsSymbol;

	if(pObject)
		*pObject = pNewVal;
	return spAtom;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvUndColl::Add(LONG Key, BSTR SortKey, IMmRvUndAtom* Value, IMmRvUndAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Underlying with the same key is already exists."));

		_bstr_t bsSortKey(SortKey);
		if(!Value)
		{
			IMmRvUndAtomPtr spNewAtom = AddNew(Key, SortKey);
			if(pRetVal)
				*pRetVal = spNewAtom.Detach();
		}
		else
		{
			_CHK(IMmRvUndCollImpl::Add(Key, bsSortKey, Value), _T("Fail to add underlying."));
			Value->AddRef();
			if(pRetVal)
				*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvUndColl, e.Error());
	}

	return S_OK;
}

void  CMmRvUndColl::SetNetExposureAUM(DOUBLE dNewVal) 
{
	if(m_dNetExposureAUM != dNewVal)
	{
		m_dNetExposureAUM = dNewVal;
		EnumCollType::iterator itr = m_coll.begin();
		while(itr != m_coll.end())
		{
			CMmRvUndAtom* pAtom = dynamic_cast<CMmRvUndAtom*>(itr->second);
			if(pAtom!=NULL)
				pAtom->SetNetExposureAUM(dNewVal);

			++itr;
		}
	}
}

void  CMmRvUndColl::SetDirty()
{
	CollType::iterator itr    = m_collRef.begin();
	CollType::iterator itrEnd = m_collRef.end();
	for(;itr!=itrEnd; ++itr)
	{
		CMmRvUndAtom* pUnd = dynamic_cast<CMmRvUndAtom*>(itr->second->second);
		if(pUnd && pUnd->m_pPrice)
			pUnd->m_pPrice->m_bDirty = VARIANT_TRUE;
	}
}
