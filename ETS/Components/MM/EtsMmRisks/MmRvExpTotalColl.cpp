// MmRvExpTotalColl.cpp : Implementation of CMmRvExpTotalColl

#include "stdafx.h"
#include "MmRvExpTotalColl.h"


// CMmRvExpTotalColl
IMmRvExpTotalAtomPtr CMmRvExpTotalColl::AddNew(DATE dtExpiry, CComObject<CMmRvExpTotalAtom>** pObject)
{
	CComObject<CMmRvExpTotalAtom>* pNewVal = NULL;
	IMmRvExpTotalAtomPtr spNewVal;

	__CHECK_HRESULT(CComObject<CMmRvExpTotalAtom>::CreateInstance(&pNewVal), _T("Fail to add expiry."));
	spNewVal.Attach(pNewVal, TRUE);
	if(FAILED(IMmRvExpTotalCollImpl::Add(dtExpiry, pNewVal)))
		CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add expiry."));
	if(pObject != NULL)
		*pObject = pNewVal;

	pNewVal->m_nExpiryMonth = static_cast<LONG>(dtExpiry);
	return spNewVal;
}

STDMETHODIMP CMmRvExpTotalColl::Add(DATE Key, IMmRvExpTotalAtom* Value, IMmRvExpTotalAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Expiry with the same key is already exists."));

		if(!Value)
			*pRetVal  = AddNew(Key).Detach();
		else
		{
			__CHECK_HRESULT(IMmRvExpTotalCollImpl::Add(Key, Value), _T("Fail to add expiry."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmRvExpTotalColl, e.Error());
	}

	return S_OK;
}

void CMmRvExpTotalColl::Calc()
{
	try
	{
		CollType::iterator itr    = m_coll.begin();
		CollType::iterator itrEnd = m_coll.end();
		CMmRvExpTotalAtom* expiry_atom = 0;
		for(; itr!=itrEnd; ++itr){
			itr->second->CalcTotals();

			expiry_atom = 
				dynamic_cast<CMmRvExpTotalAtom*>(itr->second);

			if (expiry_atom){
				expiry_atom->CalcCV();
			};
		}
	}
	catch (_com_error& /*e*/)
	{
		
	}
}
