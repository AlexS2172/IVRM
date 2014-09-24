#pragma once

class CModuleVersionEx : public VS_FIXEDFILEINFO
{
public:
	CModuleVersionEx();
	virtual ~CModuleVersionEx();

	BOOL SetModuleHandle(HMODULE hModule);
	BOOL SetModuleFileName(LPCTSTR szFileName);
	BOOL SetModulePROGID(LPCTSTR szProgID);
	BOOL SetModuleCLSID(CLSID& clsid);

	CString GetFixedFileVersion();
	CString GetFileDescription();
	CString GetLegalCopyright();
	
	CString GetFilePath()
	{
		return m_strFilePath;
	}

	CString GetFileName()
	{
		return m_strFileName;
	}

	static CString GetLongPathName(CString strShortPathName);
	static CString GetFileName(CString strPathName);
	static int GetFileNamePos(CString strPathName);
	static CString GetFilePath(CString strPathName);

protected:
	BOOL LoadFileVersionInfo();
	BOOL GetValue(LPCTSTR szKeyName, CString& sVal);
	void CleanUp();
	void AdjustFilePathAndName(LPCTSTR szPath);

protected:
	struct TRANSLATION
	{
		WORD dwLangID;			// language ID
		WORD dwCharset;			// character	set	(code page)
	}; 

	TRANSLATION		m_Translation;
	BYTE*			m_pVersionInfo;		// all version info
	CString			m_strFilePath;
	CString			m_strFileName;
};
