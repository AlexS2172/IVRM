// LicenseKey.h: interface for the CLicenseKey class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LICENSEKEY_H__2D9C3824_38D3_4CAA_AFF6_808A304E1DBF__INCLUDED_)
#define AFX_LICENSEKEY_H__2D9C3824_38D3_4CAA_AFF6_808A304E1DBF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLicenseKey  
{
public:
	bool IsExpired();
	CLicenseKey();
	CLicenseKey(LPCTSTR szKey);
	~CLicenseKey();
	
	void InitUID();
	CString GetProductID();
	CString GetProductKey() {return m_strKey;}
	DATE	GetExpiration () {return m_dtExpiration;};

	bool operator == (const CLicenseKey & key) const
		{ return  key.m_nUID == m_nUID && key.m_nModules == m_nModules; }

protected:
	long Base36ToDec(LPCTSTR szSource);
	long CalculateParity(LPCTSTR szSource);
	DATE DecodeExpireDate(long nCode);


	void InitMembers();
	bool InitFromStr(LPCTSTR szKey);
    

	long m_nUID;
    long m_nModules;
	DATE m_dtExpiration;
	CString m_strKey;
private:
	static LPCTSTR m_szAppName;

	static double m_dParityKey;
	static long	m_nProductKeyLen;
	static long	m_nExpDateKeyLen;
	static long	m_nModulesKeyLen;
	static long	m_nParityKeyLen;
};

#endif // !defined(AFX_LICENSEKEY_H__2D9C3824_38D3_4CAA_AFF6_808A304E1DBF__INCLUDED_)
