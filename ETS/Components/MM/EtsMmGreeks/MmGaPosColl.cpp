// MmGaPosColl.cpp : Implementation of CMmGaPosColl

#include "stdafx.h"
#include "MmGaPosColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaPosColl::Add(LONG Key, IMmGaPosAtom* Value, IMmGaPosAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Position with the same key is already exists."));
		}

		CComObject<CMmGaPosAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmGaPosAtom>::CreateInstance(&pNewVal), _T("Fail to add position."));
			pNewVal->AddRef();
			if(FAILED(IMmGaPosCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add position."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmGaPosCollImpl::Add(Key, Value), _T("Fail to add position."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaPosColl, e.Error());
	}

	return S_OK;
}


