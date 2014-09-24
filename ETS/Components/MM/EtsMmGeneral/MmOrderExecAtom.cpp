// MmOrderExecAtom.cpp : Implementation of CMmOrderExecExecAtom

#include "stdafx.h"
#include "MmOrderExecAtom.h"


/////////////////////////////////////////////////////////////////////////////
//
//STDMETHODIMP CMmOrderExecAtom::CopyTo(IMmOrderExecAtom* pDest)
//{
//	try
//	{
//		if(!pDest)
//			CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));
//
//		HRESULT hr;
//
//		// core info
//		__CHECK_HRESULT3(pDest->put_ID(m_nID));
//		__CHECK_HRESULT3(pDest->put_SeqNum(m_nSeqNum));
//		__CHECK_HRESULT3(pDest->put_ExecQty(m_nExecQty));
//		__CHECK_HRESULT3(pDest->put_ExecPrice(m_dExecPrice));
//		__CHECK_HRESULT3(pDest->put_ExecDate(m_dtExecDate));
//		__CHECK_HRESULT3(pDest->put_ExecID(m_bstrExecID));
//	}
//	catch(const _com_error& e)
//	{
//		return Error((PTCHAR)CComErrorWrapper::ErrorDescription(e), IID_IMmOrderExecAtom, e.Error());
//	}
//
//	return S_OK;
//}

/////////////////////////////////////////////////////////////////////////////
//
STDMETHODIMP CMmOrderExecAtom::get_OrderExecStatusString(BSTR* pRetVal)
{
	try
	{
		if(!pRetVal)
			EgLib::CComErrorWrapper::ThrowError(E_INVALIDARG, _T("Invalid argument"));

		if(*pRetVal)
			::SysFreeString(*pRetVal);

		switch(m_enExecStatus)
		{
			case enOesNew:
				*pRetVal = ::SysAllocString(L"NEW");
				break;

			case enOesPartiallyFilled:
				*pRetVal = ::SysAllocString(L"PART");
				break;

			case enOesFilled:
				*pRetVal = ::SysAllocString(L"FILL");
				break;

			case enOesDoneForDay:
				*pRetVal = ::SysAllocString(L"DFD");
				break;

			case enOesCanceled:
				*pRetVal = ::SysAllocString(L"CXL");
				break;

			case enOesReplaced:
				*pRetVal = ::SysAllocString(L"CXLR");
				break;

			case enOesPendingCancelReplace:
				*pRetVal = ::SysAllocString(L"@CXLR");
				break;

			case enOesStopped:
				*pRetVal = ::SysAllocString(L"STOP");
				break;

			case enOesRejected:
				*pRetVal = ::SysAllocString(L"RJCT");
				break;

			case enOesSuspended:
				*pRetVal = ::SysAllocString(L"SPND");
				break;

			case enOesPendingNew:
				*pRetVal = ::SysAllocString(L"@NEW");
				break;

			case enOesCalculated:
				*pRetVal = ::SysAllocString(L"CALC");
				break;

			case enOesExpired:
				*pRetVal = ::SysAllocString(L"EXP");
				break;

			default:
				*pRetVal = ::SysAllocString(L"???");
		}
	}
	catch(const _com_error& e)
	{
		return Error((PTCHAR)EgLib::CComErrorWrapper::ErrorDescription(e), IID_IMmOrderExecAtom, e.Error());
	}

	return S_OK;
}

