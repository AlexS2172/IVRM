// XMLParams.cpp : Implementation of CXMLParams

#include "stdafx.h"
#include "XMLParams.h"


// CXMLParams

STDMETHODIMP CXMLParams::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IXMLParams
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CXMLParams::SetLongValue(BSTR bsPath, BSTR bsName, long nValue)
{
	CComBSTR sbsFullName;
	GetFullName(bsPath, bsName, &sbsFullName);

	XMLParameterType XMLParam;
	XMLParam.Value = nValue;
	XMLParam.IsChanged = true;

	m_Params[sbsFullName] = XMLParam;

	return S_OK;
}

STDMETHODIMP CXMLParams::SetStringValue(BSTR bsPath, BSTR bsName, BSTR bsValue)
{
	CComBSTR sbsFullName;
	GetFullName(bsPath, bsName, &sbsFullName);

	XMLParameterType XMLParam;
	XMLParam.Value = bsValue;
	XMLParam.IsChanged = true;
	
	m_Params[sbsFullName] = XMLParam;

	return S_OK;
}

STDMETHODIMP CXMLParams::GetLongValue(BSTR bsPath, BSTR bsName, long nDefValue, long* pnValue)
{
	__CHECK_POINTER(pnValue);

	CComBSTR sbsFullName;
	GetFullName(bsPath, bsName, &sbsFullName);

	ParametersMap::const_iterator it = m_Params.find(sbsFullName);
	if((it != m_Params.end()) && (it->second.Value.vt == VT_I4))
		*pnValue = it->second.Value.lVal;
	else
		*pnValue = nDefValue;
		
	return S_OK;
}

STDMETHODIMP CXMLParams::GetStringValue(BSTR bsPath, BSTR bsName, BSTR bsDefValue, BSTR* pbsValue)
{
	__CHECK_POINTER(pbsValue);

	CComBSTR sbsFullName;
	GetFullName(bsPath, bsName, &sbsFullName);

	ParametersMap::const_iterator it = m_Params.find(sbsFullName);
	if((it != m_Params.end()) && (it->second.Value.vt == VT_BSTR) && (SysStringLen(it->second.Value.bstrVal) != 0))
		*pbsValue = ::SysAllocString(it->second.Value.bstrVal);
	else
		*pbsValue = ::SysAllocString(bsDefValue);

	return S_OK;
}

inline void CXMLParams::GetFullName(const BSTR bsPath, const BSTR bsName, BSTR* pbsFullName) const
{
	CComBSTR sbsFullName = bsPath;
	if(sbsFullName[sbsFullName.Length()-1] != L'\\')
		sbsFullName += L"\\";
	sbsFullName += bsName;

	*pbsFullName = sbsFullName.Detach();
}

STDMETHODIMP CXMLParams::get_Count(long* pnCount)
{
	__CHECK_POINTER(pnCount);
	*pnCount = static_cast<long>(m_Params.size());
	return S_OK;
}

STDMETHODIMP CXMLParams::get_Item(long idx, BSTR* pbsFullName, VARIANT* pvtParameter, VARIANT_BOOL* pIsChanged)
{
	__CHECK_POINTER(pbsFullName);
	__CHECK_POINTER(pvtParameter);

	ParametersMap::const_iterator it = m_Params.begin();
	for(long i = 0; (i < idx) && (it != m_Params.end()); ++i)
		++it;
	if(it == m_Params.end())
		return E_INVALIDARG;

	*pbsFullName = it->first.m_T.Copy();
	if(pIsChanged)
		*pIsChanged = it->second.IsChanged;

	CComVariant svtValue(it->second.Value);
	return svtValue.Detach(pvtParameter);
}

STDMETHODIMP CXMLParams::Add(BSTR bsFullName, VARIANT vtParameter)
{
	if((vtParameter.vt != VT_I4) && (vtParameter.vt != VT_BSTR))
		return E_INVALIDARG;

	XMLParameterType XMLParam;
	XMLParam.Value = vtParameter;
	XMLParam.IsChanged = false;

	m_Params[CComBSTR(bsFullName)] = XMLParam;

	return S_OK;
}