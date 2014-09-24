// SymbolObject.cpp : Implementation of CSymbolObject
#include "stdafx.h"
#include "MsgStruct.h"
#include "SymbolObject.h"

/////////////////////////////////////////////////////////////////////////////
// CSymbolObject

STDMETHODIMP CSymbolObject::get_Type(SYMBOL_TYPE *pVal)
{
	if (!pVal) 
		return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_Type;

	return S_OK;
}

STDMETHODIMP CSymbolObject::put_Type(SYMBOL_TYPE newVal)
{
	ObjectLock lock(this);

	m_Type = newVal;

	return S_OK;
}

STDMETHODIMP CSymbolObject::get_Name(BSTR *pVal)
{
	if (!pVal) 
		return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_Name.Copy();

	return S_OK;
}

STDMETHODIMP CSymbolObject::put_Name(BSTR newVal)
{
	ObjectLock lock(this);

	m_Name = newVal;

	return S_OK;
}

STDMETHODIMP CSymbolObject::get_Exchange(BSTR *pVal)
{
	if (!pVal) 
		return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_Exchange.Copy();

	return S_OK;
}

STDMETHODIMP CSymbolObject::put_Exchange(BSTR newVal)
{
	ObjectLock lock(this);

	m_Exchange = newVal;

	return S_OK;
}

STDMETHODIMP CSymbolObject::get_Currency(BSTR *pVal)
{
	if (!pVal) 
		return E_POINTER;

	ObjectLock lock(this);

	*pVal = m_Currency.Copy();

	return S_OK;
}

STDMETHODIMP CSymbolObject::put_Currency(BSTR newVal)
{
	ObjectLock lock(this);

	m_Currency = newVal;

	return S_OK;
}

STDMETHODIMP CSymbolObject::Set(SYMBOL_TYPE Type, BSTR Name, BSTR Exchange, BSTR Currency)
{
	ObjectLock lock(this);

	m_Type = Type;
	m_Name = Name;
	m_Exchange = Exchange;
	m_Currency = Currency;

	return S_OK;
}


STDMETHODIMP CSymbolObject::GetString(BSTR* RetVal)
{
	if (RetVal == NULL)
		return E_POINTER;

	ObjectLock lock(this);

	CComBSTR bstr; 

	switch (m_Type)
	{
	case enStStock: bstr = L"S/"; break;
	case enStIndex: bstr = L"I/"; break;
	case enStOption: bstr = L"O/"; break;
	case enStFuture: bstr = L"F/"; break;
	case enStFutureOption: bstr = L"FO/"; break;
	case enStFutUnd: bstr = L"U/"; break;
	default:
		_ASSERT(FALSE);
		return E_FAIL;
	}

	bstr += m_Name;
	bstr += L"/";
	bstr += m_Exchange;
	bstr += L"/";
	bstr += m_Currency;

	*RetVal = bstr.Copy();

	return S_OK;
}

STDMETHODIMP CSymbolObject::SetFromString(BSTR Val)
{
	ObjectLock lock(this);

	const wchar_t chDelim = L'/';

	wstring sString = _bstr_t(Val);

	long nTypeEndPos = sString.find(chDelim);
	long nNameEndPos = string::npos;
	long nExchEndPos = string::npos;

	wstring sType;

	if (nTypeEndPos != string::npos)
	{
		sType = sString.substr(0, nTypeEndPos).c_str();
		nNameEndPos = sString.find(chDelim, nTypeEndPos+1);
	}

	if (sType == L"S")
		m_Type = enStStock;
	else if (sType == L"I")
		m_Type = enStIndex;
	else if (sType == L"O")
		m_Type = enStOption;
	else if (sType == L"F")
		m_Type = enStFuture;
	else if (sType == L"FO")
		m_Type = enStFutureOption;
	else if (sType == L"U")
		m_Type = enStFutUnd;
	else
	{
		_ASSERT(FALSE);
		return E_FAIL;
	}

	if (nNameEndPos != string::npos)
	{
		m_Name = sString.substr(nTypeEndPos+1, nNameEndPos - (nTypeEndPos+1)).c_str();
		nExchEndPos = sString.find(chDelim, nNameEndPos+1);
	}

	if (nExchEndPos != string::npos)
	{
		m_Exchange = sString.substr(nNameEndPos+1, nExchEndPos - (nNameEndPos+1)).c_str();
		m_Currency = sString.substr(nExchEndPos+1).c_str();
	}

	return S_OK;
}
