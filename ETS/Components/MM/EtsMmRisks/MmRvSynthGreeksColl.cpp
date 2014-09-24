// MmRvSynthGreeksColl.cpp : Implementation of CMmRvSynthGreeksColl
#include "stdafx.h"
#include "MmRvSynthGreeksColl.h"

IMmRvSynthGreeksAtomPtr CMmRvSynthGreeksColl::AddNew(long lId, _bstr_t bsSymbol, CComObject<CMmRvSynthGreeksAtom>** ppObject)
{
	IMmRvSynthGreeksAtomPtr spReturn;
	CComObject<CMmRvSynthGreeksAtom>* pNewVal = NULL;

	_CHK(CComObject<CMmRvSynthGreeksAtom>::CreateInstance(&pNewVal), _T("Fail to add synthetic greeks."));
	spReturn.Attach(pNewVal, TRUE);

	if(FAILED(IMmRvSynthGreeksCollImpl::Add(lId, bsSymbol, pNewVal)))
		EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add synthetic greeks."));

	pNewVal->m_nSynthUndID = lId;
	pNewVal->m_bstrSynthUndSymbol = bsSymbol;

	if(ppObject)
		*ppObject = pNewVal;
	return spReturn;
}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmRvSynthGreeksColl::Add(LONG Key, BSTR SortKey, IMmRvSynthGreeksAtom* Value, IMmRvSynthGreeksAtom** pRetVal)
{
	try
	{

		if(m_collRef.find(Key) != m_collRef.end())
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic greeks with the same key is already exists."));

		_bstr_t bsKey(SortKey);
		if(!Value)
			*pRetVal = AddNew(Key, bsKey).Detach();
		else
		{
			_CHK(IMmRvSynthGreeksCollImpl::Add(Key, bsKey, Value), _T("Fail to add synthetic greeks."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvSynthGreeksColl, e.Error());
	}

	return S_OK;
}


//// MmRvSynthGreeksColl.cpp : Implementation of CMmRvSynthGreeksColl
//#include "stdafx.h"
//#include "MmRvSynthGreeksColl.h"
//
///////////////////////////////////////////////////////////////////////////////
////
//STDMETHODIMP CMmRvSynthGreeksColl::Add(LONG Key, BSTR SortKey, IMmRvSynthGreeksAtom* Value, IMmRvSynthGreeksAtom** pRetVal)
//{
//	try
//	{
//		if(m_collRef.find(Key) != m_collRef.end())
//			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic greeks with the same key is already exists."));
//
//		_bstr_t bsKey(SortKey);
//		if(!Value)
//		{
//
//			CComObject<CMmRvSynthGreeksAtom>* pNewVal;
//
//			_CHK(CComObject<CMmRvSynthGreeksAtom>::CreateInstance(&pNewVal), _T("Fail to add synthetic greeks."));
//			pNewVal->AddRef();
//			if(FAILED(IMmRvSynthGreeksCollImpl::Add(Key, bsKey, pNewVal)))
//			{
//				pNewVal->Release();
//				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add synthetic greeks."));
//			}
//			*pRetVal = pNewVal;
//		}
//		else
//		{
//			_CHK(IMmRvSynthGreeksCollImpl::Add(Key, bsKey, Value), _T("Fail to add synthetic greeks."));
//			Value->AddRef();
//			*pRetVal = Value;
//		}
//	}
//	catch(const _com_error& e)
//	{
//		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmRvSynthGreeksColl, e.Error());
//	}
//
//	return S_OK;
//}
//
