#pragma once
#ifndef __MODULE_VERSION_H__
#define __MODULE_VERSION_H__

#include <shlwapi.h>

namespace EODCO
{

class CEgModuleVersion : public VS_FIXEDFILEINFO
{
protected:
    struct TRANSLATION
    {
		WORD dwLangID;         // language ID
		WORD dwCharset;        // character set (code page)
	};

	TRANSLATION		m_Translation;
	BYTE*			m_pVersionInfo;   // all version info

public:
	CEgModuleVersion();
	virtual ~CEgModuleVersion();

	BOOL     GetModuleVersionInfo(HINSTANCE hInstance);
	BOOL     GetModuleVersionInfo(LPCTSTR szModuleName);
	BOOL     GetFileVersionInfo(LPCTSTR szFileName);
	BOOL	 GetValue(LPCTSTR szKeyName, _bstr_t& sVal);

	static BOOL DllGetVersion(LPCTSTR szModuleName, DLLVERSIONINFO& dvi);
};

};

#endif // __MODULE_VERSION_H__
