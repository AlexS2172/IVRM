// MmShOptRootColl.cpp : Implementation of CMmShOptRootColl

#include "stdafx.h"
#include "MmShOptRootColl.h"


// CMmShOptRootColl
/////////////////////////////////////////////////////////////////////////////
//

STDMETHODIMP CMmShOptRootColl::Add(LONG Key, IMmShOptRootAtom* Value, IMmShOptRootAtom** pRetVal)
{
	__CHECK_POINTER(pRetVal);

	try
	{
		if(m_coll.find(Key) != m_coll.end())
		{
			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Option root with the same key is already exists."));
		}

		CComObject<CMmShOptRootAtom>* pExp;

		if(!Value)
		{
			_CHK(CComObject<CMmShOptRootAtom>::CreateInstance(&pExp), _T("Fail to add option root."));
			pExp->AddRef();
			if(FAILED(IMmShOptRootCollImpl::Add(Key, pExp)))
			{
				pExp->Release();
				CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Fail to add option root."));
			}
			*pRetVal = pExp;
		}
		else
		{
			_CHK(IMmShOptRootCollImpl::Add(Key, Value), _T("Fail to add option root."));
			Value->AddRef();
			*pRetVal = Value;
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmShOptRootColl, e.Error());
	}

	return S_OK;
}
