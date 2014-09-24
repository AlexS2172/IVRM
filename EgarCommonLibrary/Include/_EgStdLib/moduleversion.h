#ifndef __EGSTDLIB_MODULE_VERSION_H__
#define __EGSTDLIB_MODULE_VERSION_H__

#include <EgStdLib\EgStdLibCommon.h>

namespace EgStd
{

class EGSTD_EXT_CLASS CModuleVersion :
	public VS_FIXEDFILEINFO
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
    CModuleVersion();
    virtual ~CModuleVersion();

    BOOL     GetModuleVersionInfo(HINSTANCE hInstance);
    BOOL     GetModuleVersionInfo(LPCTSTR szModuleName);
    BOOL     GetFileVersionInfo(LPCTSTR szFileName);
	BOOL	 GetValue(LPCTSTR szKeyName, _bstr_t& sVal);

    static BOOL DllGetVersion(LPCTSTR szModuleName, DLLVERSIONINFO& dvi);
};

}

#endif
