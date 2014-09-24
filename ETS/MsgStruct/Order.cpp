// Order.cpp : Implementation of COrder
#include "stdafx.h"
#include "MsgStruct.h"
#include "Order.h"

/////////////////////////////////////////////////////////////////////////////
// COrder
STDMETHODIMP COrder::GenerateUID(BSTR* pUID)
{
	if(!pUID)
		return E_POINTER;

	HRESULT hr;
	GUID guid;
	if(FAILED( hr = ::CoCreateGuid(&guid) ))
		return  hr;

	TCHAR tchBuf[33] = {0};
	int iLen = _stprintf_s(tchBuf,33,_T("%08X%04X%04X"),guid.Data1,guid.Data2,guid.Data3);

	for( int i = 0; i < 8; i++)
	{		
		iLen += _stprintf_s(tchBuf + iLen, 33-iLen ,_T("%02X"),guid.Data4[i]);
	}

	*pUID = _bstr_t(tchBuf).copy();

	return hr;
}

