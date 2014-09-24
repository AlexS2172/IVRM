#include "StdAfx.h"
#include "XMLParamsHelper.h"


HRESULT CXMLParamsHelper::GetMainSettingsFilePath()
{
	CEgRegKey key;	
	long nResult = key.Open(HKEY_LOCAL_MACHINE, DEF_REG_KEY, KEY_READ);
	if(nResult != ERROR_SUCCESS)
	{
		m_sbsMainFile = DEF_STR_VALUE;
		return E_FAIL;
	}

	CComBSTR sbsValue;
	nResult = key.QueryValue(sbsValue, DEF_REG_PARAM);
	if(nResult != ERROR_SUCCESS)
	{
		m_sbsMainFile = DEF_STR_VALUE;
		return E_FAIL;
	}

	m_sbsMainFile = sbsValue;

	return S_OK;
}

HRESULT CXMLParamsHelper::GetUserSettingsFilePath()
{
	GetMainSettingsFilePath();

	DWORD nSize = UNLEN + 1;
	TCHAR szUserName[UNLEN + 1];
	if(!(::GetUserName((LPTSTR)&szUserName, &nSize)))
	{
		m_sbsUserName = DEF_STR_VALUE;
		m_sbsUserFile = DEF_STR_VALUE;
		return E_FAIL;
	}

	m_sbsUserName = szUserName;
		
	try
	{
		CComPtr<IXMLLoader> spXMLLoader;
		if(FAILED(spXMLLoader.CoCreateInstance(CLSID_XMLLoader)))
		{
			m_sbsUserFile = DEF_STR_VALUE;
			return E_FAIL;
		}

		CComPtr<IXMLParams> spXMLParams = spXMLLoader->Load((BSTR)m_sbsMainFile);

		CComBSTR sbsXMLKey = DEF_XML_KEY;
		sbsXMLKey += L"\\";
		sbsXMLKey += szUserName;
		m_sbsUserFile = (BSTR)spXMLParams->GetStringValue((BSTR)sbsXMLKey, PTX_XML_PARAM, DEF_STR_VALUE);
	}
	catch(_com_error&)
	{
		m_sbsUserFile = DEF_STR_VALUE;
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CXMLParamsHelper::LoadXMLParams()
{
	GetUserSettingsFilePath();

	m_spXMLLoader.Release();
	if(FAILED(m_spXMLLoader.CoCreateInstance(CLSID_XMLLoader)))
		return E_FAIL;

	try
	{ m_spXMLMainParams = m_spXMLLoader->Load((BSTR)m_sbsMainFile); }
	catch(_com_error&)
	{ 
		m_spXMLMainParams.Release();
		if(FAILED(m_spXMLMainParams.CoCreateInstance(CLSID_XMLParams)))
			return E_FAIL; 
	}

	try
	{ m_spXMLUserParams = m_spXMLLoader->Load((BSTR)m_sbsUserFile); }
	catch(_com_error&)
	{
		m_spXMLUserParams.Release();
		if(FAILED(m_spXMLUserParams.CoCreateInstance(CLSID_XMLParams)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CXMLParamsHelper::SaveXMLParams()
{
	if(!m_spXMLLoader)
		return E_UNEXPECTED;

	try
	{ m_spXMLLoader->Save((BSTR)m_sbsUserFile, m_spXMLUserParams); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::SaveMainXMLParams()
{
	if(!m_spXMLLoader)
		return E_UNEXPECTED;

	try
	{ m_spXMLLoader->Save((BSTR)m_sbsMainFile, m_spXMLMainParams); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::SetXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long nValue)
{
	if(!m_spXMLUserParams)
		return E_UNEXPECTED;

	try
	{ m_spXMLUserParams->SetLongValue(bsKey, bsName, nValue); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::GetXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long* pnValue, long nDef)
{
	if(!m_spXMLUserParams)
		return E_UNEXPECTED;
	if(!pnValue)
		return E_POINTER;

	try
	{ *pnValue = m_spXMLUserParams->GetLongValue(bsKey, bsName, nDef); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::SetXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, const _bstr_t& bsValue)
{
	if(!m_spXMLUserParams)
		return E_UNEXPECTED;

	try
	{ m_spXMLUserParams->SetStringValue(bsKey, bsName, bsValue); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::GetXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, _bstr_t* pbsValue, const _bstr_t& bsDef)
{
	if(!m_spXMLUserParams)
		return E_UNEXPECTED;
	if(!pbsValue)
		return E_POINTER;

	try
	{ *pbsValue = m_spXMLUserParams->GetStringValue(bsKey, bsName, bsDef); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::SetMainXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long nValue)
{
	if(!m_spXMLMainParams)
		return E_UNEXPECTED;

	try
	{ m_spXMLMainParams->SetLongValue(bsKey, bsName, nValue); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::GetMainXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long* pnValue, long nDef)
{
	if(!m_spXMLMainParams)
		return E_UNEXPECTED;
	if(!pnValue)
		return E_POINTER;

	try
	{ *pnValue = m_spXMLMainParams->GetLongValue(bsKey, bsName, nDef); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::SetMainXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, const _bstr_t& bsValue)
{
	if(!m_spXMLMainParams)
		return E_UNEXPECTED;

	try
	{ m_spXMLMainParams->SetStringValue(bsKey, bsName, bsValue); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::GetMainXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, _bstr_t* pbsValue, const _bstr_t& bsDef)
{
	if(!m_spXMLMainParams)
		return E_UNEXPECTED;
	if(!pbsValue)
		return E_POINTER;

	try
	{ *pbsValue = m_spXMLMainParams->GetStringValue(bsKey, bsName, bsDef); }
	catch(_com_error&)
	{ return E_FAIL; }

	return S_OK;
}

HRESULT CXMLParamsHelper::GetUserGroup(BSTR* pbsUserGroup)
{
	if(!pbsUserGroup)
		return E_POINTER;

	_bstr_t sbsUserGroup;
	_bstr_t sbsXMLKey = DEF_XML_KEY;
	sbsXMLKey += L"\\";
	sbsXMLKey += (BSTR)m_sbsUserName;
	if(FAILED(GetMainXMLString(sbsXMLKey, L"UserGroup", &sbsUserGroup)))
		return E_FAIL;

	*pbsUserGroup = sbsUserGroup.Detach();

	return S_OK;
}

HRESULT CXMLParamsHelper::SetUserGroup(const BSTR bsUserGroup)
{
	_bstr_t sbsXMLKey = DEF_XML_KEY;
	sbsXMLKey += L"\\";
	sbsXMLKey += (BSTR)m_sbsUserName;
	if(FAILED(SetMainXMLString(sbsXMLKey, L"UserGroup", bsUserGroup)))
		return E_FAIL;

	return S_OK;
}


HRESULT CXMLParamsHelper::GetDbConnect(BSTR* pbsDbConnect)
{
	if(!pbsDbConnect)
		return E_POINTER;

	CComBSTR sbsUserGroup;
	if(FAILED(GetUserGroup(&sbsUserGroup)))
		return E_FAIL;

	_bstr_t sbsDbConnect;
	if(FAILED(GetMainXMLString(DB_XML_KEY, (BSTR)sbsUserGroup, &sbsDbConnect)))
		return E_FAIL;

	*pbsDbConnect = sbsDbConnect.Detach();

	return S_OK;
}

HRESULT CXMLParamsHelper::SetDbConnect(const BSTR bsDbConnect)
{
	CComBSTR sbsUserGroup;
	if(FAILED(GetUserGroup(&sbsUserGroup)))
		return E_FAIL;

	if(FAILED(SetMainXMLString(DB_XML_KEY, (BSTR)sbsUserGroup, bsDbConnect)))
		return E_FAIL;

	return S_OK;
}