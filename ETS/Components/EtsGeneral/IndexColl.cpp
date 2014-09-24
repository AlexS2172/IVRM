// IndexColl.cpp : Implementation of CIndexColl

#include "stdafx.h"
#include "IndexColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CIndexColl::Add(LONG Key, BSTR SortKey, IIndexAtom* Value, IIndexAtom** pRetVal)
{
	try
	{
		if(m_collRef.find(Key) != m_collRef.end())
		{
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Index with the same key is already exists."));
		}

		if(!Value)
		{
			CComObject<CIndexAtom>* pNewVal;
			_CHK(CComObject<CIndexAtom>::CreateInstance(&pNewVal), _T("Fail to add index."));
			pNewVal->AddRef();
			if(FAILED(IIndexCollImpl::Add(Key, CComBSTR(SortKey), pNewVal)))
			{
				pNewVal->Release();
				EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add index."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			_CHK(IIndexCollImpl::Add(Key, CComBSTR(SortKey), Value), _T("Fail to add index."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IIndexColl, e.Error());
	}

	return S_OK;
}

