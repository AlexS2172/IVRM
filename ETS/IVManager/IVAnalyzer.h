// IVAnalyzer.h : main header file for the IVANALYZER application
//

#if !defined(AFX_IVANALYZER_H__79C5481C_FFDF_42EF_9E7D_CEC6AF3DD4EA__INCLUDED_)
#define AFX_IVANALYZER_H__79C5481C_FFDF_42EF_9E7D_CEC6AF3DD4EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "IVAnalyzer_i.h"

#include "DataDef.h"
#include "LicenseKey.h"

#include "../ETSXMLParams/Helper/XMLParamsHelper.h"

struct CPriceProviderOptions
{
	CPriceProviderOptions():m_iProxyPort(0),m_iUseProxy(0){};
	CString m_strAccount;
	CString m_strPassword;
	
	CString m_strProxy;
	int   m_iProxyPort;
	int   m_iUseProxy;

	bool operator != (const CPriceProviderOptions & opts) const
	{
		return m_strAccount != opts.m_strAccount || 
			m_strPassword != opts.m_strPassword ||
			m_iUseProxy!=opts.m_iUseProxy ||
			m_iProxyPort!=opts.m_iProxyPort ||
			m_strProxy !=opts.m_strProxy;
	}

	bool IsEmpty () const
	{
		return m_strAccount.IsEmpty() || m_strPassword.IsEmpty();
	}
};


struct CIVOptions
{
	CString m_strURL;
	CString m_strAccount;
	CString m_strPassword;
	int m_nUseProxy;
	CString m_strProxy;
	
	CString m_strProxyLogin;
	CString m_strProxyPassword;
	int     m_iProxyAuth;

	DWORD m_nPort;

	CIVOptions (): 
		m_iProxyAuth(0),m_nUseProxy (2), m_strURL(_T("http://www.ivolatility.com/data_load")),
		m_nPort (0)
		{  }
	bool operator != (const CIVOptions & opts) const
	{
		return m_iProxyAuth != opts.m_iProxyAuth ||
			   m_strProxyLogin != opts.m_strProxyLogin ||
			   m_strProxyPassword != opts.m_strProxyPassword ||
			   m_strURL != opts.m_strURL || 
			   m_strAccount != opts.m_strAccount ||
			   m_strPassword != opts.m_strPassword ||
			   m_nUseProxy != opts.m_nUseProxy ||
			   m_strProxy != opts.m_strProxy || 
			   m_nPort != opts.m_nPort;
	}
	
	bool IsEmpty () const
	{
		return m_strAccount.IsEmpty() || m_strPassword.IsEmpty();
	}
};


enum CURVE_MODE
{
	enCall		= 0,
	enPut		= 1,
	enOTM		= 2
};

enum CHART_MODE
{
	enChart_All            =0,
    enChart_5Penny         =1,
	enChart_All_Range	   =2,
    enChart_5Penny_Range   =3

};

enum CALCULATION_MODE
{
	enBinominal,
    enOptBinominal,
    enVSKLog
};

struct CIVAnalyzerOptions
{
	CString		m_strConnection;
	CURVE_MODE  m_enCurveMode;
	CHART_MODE  m_enChartMode;
	CALCULATION_MODE m_enCalculationMode;
	double 		m_fChartRangeFrom;
	double	    m_fChartRangeTo;
	bool		m_bVMAlwaysOnTop;
	bool		m_bVMUseExtrapolation;

	long		m_nSleepDuration;
	long		m_nSleepFrequency;

	long        m_enProviderType;

	CPriceProviderOptions m_PriceProvOpts;
	CIVOptions			  m_IVOpts;
	bool		m_bFlatOutRange;
	bool		m_bFlatNoBid;
	

	CIVAnalyzerOptions():
		m_enCalculationMode(enBinominal),
		m_enCurveMode(enCall), 
		m_enChartMode(enChart_All),
		m_fChartRangeFrom(0.),   
		m_fChartRangeTo(0.),
		m_bVMAlwaysOnTop(true),
		m_bVMUseExtrapolation(true),
		m_nSleepDuration(0),
		m_enProviderType(-1),
		m_nSleepFrequency(1),
		m_bFlatOutRange(false),
		m_bFlatNoBid(false)
	{}
};


/////////////////////////////////////////////////////////////////////////////
// CIVAnalyzerApp:
// See IVAnalyzer.cpp for the implementation of this class
//

class CIVAnalyzerApp : public CWinApp
{
public:
	CIVAnalyzerApp();
	static LPCTSTR m_szRegKeyRoot;
	static LPCTSTR m_szRegKeyRootEgar;
	static LPCTSTR m_szRegKeyRootEgarETS;
	static LPCTSTR m_szRegKeyRootEgarETSVM;
	static LPCTSTR m_szRegKeyRootEgarETSVMS;

	static LPCTSTR m_szRegKey;
	static LPCTSTR m_szMyTrackKey;

	static LPCTSTR m_sbsXMLKey;
	static LPCTSTR m_sbsXMLKeyRoot;
	static LPCSTR  m_sbsXMLGroupsKeyRoot;
		
	void GetOptions (CIVAnalyzerOptions & opts);
	bool SaveOptions (CIVAnalyzerOptions & opts);
	bool ReadOptions(bool Flag = true);
	
	CString GetConnection();
	
	CString GetCompany() {return m_strCompany;}
	CString GetUser()  {return m_strUser;}
	CString GetSymbol() {return m_strSymbol;}
	void    ClearSymbol() {m_strSymbol=_T("");}
	HMODULE GetAppearance() {return m_hAppearanceModule;}

	CString GetProductID() {return m_license.GetProductID();}
	CString GetProductKey() {return m_license.GetProductKey();}
	DATE GetKeyExpiration() {return m_license.GetExpiration(); }
	
	double GetThreshold()  {return m_dThreshold;};

	CVolaNotificationClient  * GetVolaNotificationClient ();

	bool IsMultyScreenSupported()
	{
		return (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS && m_osvi.dwMajorVersion >= 4 && m_osvi.dwMinorVersion >= 10 )|| //Win98 and older
			   (m_osvi.dwPlatformId == VER_PLATFORM_WIN32_NT	  && m_osvi.dwMajorVersion >= 5 && m_osvi.dwMinorVersion >= 0 );   //Win2000 and older
	}
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIVAnalyzerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIVAnalyzerApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

	HACCEL m_hAccel;
	BOOL m_bATLInited;
	HANDLE m_hMutex;
	CCriticalSection m_csOptions;
	CIVAnalyzerOptions m_opts;
	
	CString m_strUser;
	CString m_strCompany;
	CString m_strSymbol;

	OSVERSIONINFO m_osvi;

	CLicenseKey m_license;
	bool m_bExpired;
	double m_dThreshold;
	
	PP::IProvidersPtr m_spProvider;

	HMODULE  m_hAppearanceModule;

#ifdef _VOLA_MANAGER
	Stg::ISettingsPtr    m_spDBSettings;
	Stg::IIVSettingsPtr  m_spIVSettings;
#endif

private:
	static LRESULT CALLBACK IVCbtFilterHook(int code, WPARAM wParam, LPARAM lParam);

	HHOOK m_hCBTHook;
	BOOL InitATL();
protected:
	bool CheckLicense(LPCTSTR szKey);
};

extern CIVAnalyzerApp theApp;

/////////////////////////////////////////////////////////////////////////////
int encode64(const char *_in, unsigned inlen, char *_out, unsigned outmax, unsigned *outlen);
int decode64(const char *in, unsigned inlen, char *out, unsigned *outlen);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IVANALYZER_H__79C5481C_FFDF_42EF_9E7D_CEC6AF3DD4EA__INCLUDED_)
