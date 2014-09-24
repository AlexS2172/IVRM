// MmTntCardAtom.cpp : Implementation of CMmTntCardAtom

#include "stdafx.h"
#include "MmTntCardAtom.h"

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmTntCardAtom::get_CardStatusString(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		switch(m_enStatus)
		{
			case enTcsOutPendingExecute:
				*pRetVal = ::SysAllocString(L"@EXEC");
				break;

			case enTcsOutExecuted:
				*pRetVal = ::SysAllocString(L"EXEC");
				break;

			case enTcsOutFailed:
				*pRetVal = ::SysAllocString(L"FAIL");
				break;

			case enTcsInNew:
				*pRetVal = ::SysAllocString(L"NEW");
				break;

			case enTcsInPendingAccept:
				*pRetVal = ::SysAllocString(L"@ACPT");
				break;

			case enTcsInAccepted:
				*pRetVal = ::SysAllocString(L"ACPT");
				break;

			case enTcsInFailed:
				*pRetVal = ::SysAllocString(L"FAIL");
				break;

			case enTcsInRejected:
				*pRetVal = ::SysAllocString(L"RJCT");
				break;

			default:
				*pRetVal = ::SysAllocString(L"???");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmTntCardAtom, e.Error());
	}

	return S_OK;
}

