// EtsPriceProfileColl.cpp : Implementation of CEtsPriceProfileColl

#include "stdafx.h"
#include "EtsPriceProfileColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CEtsPriceProfileColl::Add(LONG Key, BSTR SortKey, IEtsPriceProfileAtom* Value, IEtsPriceProfileAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Price profile with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CEtsPriceProfileAtom>* pNewVal;
			_CHK(CComObject<CEtsPriceProfileAtom>::CreateInstance(&pNewVal), _T("Fail to add price profile."));
			pNewVal->AddRef();
			if(FAILED(IEtsPriceProfileCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add price profile."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IEtsPriceProfileCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add price profile."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IEtsPriceProfileColl, e.Error());
	}

	return S_OK;
}

IEtsPriceProfileAtomPtr CEtsPriceProfileColl::AddNew(long lID, _bstr_t bsSymbol, CComObject<CEtsPriceProfileAtom>** pAtom )
{
	CComObject<CEtsPriceProfileAtom>* pNewVal;
	IEtsPriceProfileAtomPtr spRet;

	_CHK(CComObject<CEtsPriceProfileAtom>::CreateInstance(&pNewVal), _T("Fail to create PriceProfile."));
	spRet.Attach(pNewVal, TRUE);

	pNewVal->m_nID = lID;

	if(FAILED(IEtsPriceProfileCollImpl::Add(lID, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add PriceProfile."));

	if(pAtom)
		*pAtom = pNewVal;
	return spRet;
}

