// MmGaSynthGreekColl.cpp : Implementation of CMmGaSynthGreekColl

#include "stdafx.h"
#include "MmGaSynthGreekColl.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmGaSynthGreekColl::Add(LONG Key, IMmGaSynthGreekAtom* Value, IMmGaSynthGreekAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Synthetic greek with the same key is already exists."));
		}

		CComObject<CMmGaSynthGreekAtom>* pNewVal;

		if(!Value)
		{
			__CHECK_HRESULT(CComObject<CMmGaSynthGreekAtom>::CreateInstance(&pNewVal), _T("Fail to add synthetic greek."));
			pNewVal->AddRef();
			if(FAILED(IMmGaSynthGreekCollImpl::Add(Key, pNewVal)))
			{
				pNewVal->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add synthetic greek."));
			}
			*pRetVal = pNewVal;
		}
		else
		{
			__CHECK_HRESULT(IMmGaSynthGreekCollImpl::Add(Key, Value), _T("Fail to add synthetic greek."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmGaSynthGreekColl, e.Error());
	}

	return S_OK;
}


