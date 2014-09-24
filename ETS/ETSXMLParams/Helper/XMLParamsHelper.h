#ifndef __XML_PARAMS_HELPER_H_
#define __XML_PARAMS_HELPER_H_

#include "Lmcons.h"

#define DEF_REG_KEY		_T("SOFTWARE\\Egar\\ETS\\Asp\\Settings")
#define DEF_REG_PARAM	_T("PathToMainXML")

#define DEF_XML_KEY		L"ETS\\Asp\\Users"
#define PTX_XML_PARAM	L"PathToXML"
#define UG_XML_PARAM	L"UserGroup"

#define DB_XML_KEY		L"ETS\\Asp\\Database"

#define DEF_STR_VALUE	L""

class CXMLParamsHelper
{
public:
	CXMLParamsHelper()
	{};
	virtual ~CXMLParamsHelper()
	{};

	HRESULT LoadXMLParams();

	HRESULT SaveXMLParams();
	HRESULT SaveMainXMLParams();

	HRESULT SetXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long nValue);
	HRESULT GetXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long* pnValue, long nDef = 0);

	HRESULT SetXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, const _bstr_t& bsValue);
	HRESULT GetXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, _bstr_t* pbsValue, const _bstr_t& bsDef = _T(""));

	HRESULT SetMainXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long nValue);
	HRESULT GetMainXMLLong(const _bstr_t& bsKey, const _bstr_t& bsName, long* pnValue, long nDef = 0);

	HRESULT SetMainXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, const _bstr_t& bsValue);
	HRESULT GetMainXMLString(const _bstr_t& bsKey, const _bstr_t& bsName, _bstr_t* pbsValue, const _bstr_t& bsDef = _T(""));

	HRESULT GetUserGroup(BSTR* pbsUserGroup);
	HRESULT SetUserGroup(const BSTR bsUserGroup);

	HRESULT GetDbConnect(BSTR* pbsDbConnect);
	HRESULT SetDbConnect(const BSTR bsDbConnect);

private:

	HRESULT GetMainSettingsFilePath();
	HRESULT GetUserSettingsFilePath();

private:

	CComPtr<IXMLParams> m_spXMLMainParams;
	CComPtr<IXMLParams> m_spXMLUserParams;
	CComPtr<IXMLLoader> m_spXMLLoader;

	CComBSTR m_sbsMainFile;
	CComBSTR m_sbsUserFile;

	CComBSTR m_sbsUserName;
};

#endif //__XML_PARAMS_HELPER_H_
