// MmVaStrikeColl.cpp : Implementation of CMmVaStrikeColl

#include "stdafx.h"
#include "MmVaStrikeColl.h"


/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmVaStrikeColl::Add(DOUBLE Key, IMmVaStrikeAtom* Value, IMmVaStrikeAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Strike with the same key is already exists."));
		}

		CComObject<CMmVaStrikeAtom>* pStrike;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmVaStrikeAtom>::CreateInstance(&pStrike), _T("Fail to add strike."));
			pStrike->AddRef();
			if(FAILED(IMmVaStrikeCollImpl::Add(Key, pStrike)))
			{
				pStrike->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add strike."));
			}
			*pRetVal = pStrike;
		}
		else
		{
			__CHECK_HRESULT(IMmVaStrikeCollImpl::Add(Key, Value), _T("Fail to add strike."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmVaStrikeColl, e.Error());
	}

	return S_OK;
}

