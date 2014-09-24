// AboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "TransportControl.h"
#include "AboutBox.h"
#include "ModulVer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutBox dialog


CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CPropertyPage(CAboutBox::IDD)
{
	TCHAR	szFileName[1024];
	CString sFmt, sVer = _T("N/A");

	if (GetModuleFileName(theApp.m_hInstance, szFileName, sizeof(szFileName)))
	{
		CModuleVersion ver;
		ver.GetModuleVersionInfo(szFileName);
	
		ver.GetValue(_T("FileVersion"), sVer);
		
		INT iMajor, iMinor, iBuild, iRevision;
		if (_stscanf_s(sVer, _T("%d,%d,%d,%d"), &iMajor, &iMinor, &iBuild, &iRevision) == 4)
		{
			sFmt.Format(_T("%d, %d, %d, %d"),
						iMajor, iMinor, iBuild, iRevision);
		}
	}
	
	CString strFile,strServiceFile,strVer,strProxyFile,strVer2;

	WIN32_FIND_DATA data;
	HANDLE hFind = FindFirstFile (szFileName, &data);
	if( hFind != INVALID_HANDLE_VALUE )
	{
		FindClose(hFind);

		strFile = data.cFileName;					
	}					
	
	FillObjectVersion (__uuidof (Host),strServiceFile,strVer);
	
	FillObjectVersion (__uuidof (HostProxy),strProxyFile,strVer2);

	//{{AFX_DATA_INIT(CAboutBox)
	m_sVersion = sFmt;
	m_sVersion2 = strVer;
	m_sVersion3 = strVer2;
	m_sServiceName = strServiceFile;
	m_sProxyName = strProxyFile;
	m_sName = strFile;
	//}}AFX_DATA_INIT
}


void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutBox)
	DDX_Text(pDX, IDC_VERSION, m_sVersion);
	DDX_Text(pDX, IDC_VERSION2, m_sVersion2);
	DDX_Text(pDX, IDC_VERSION3, m_sVersion3);
	DDX_Text(pDX, IDC_PROXY_NAME, m_sProxyName);
	DDX_Text(pDX, IDC_SERVICE_NAME, m_sServiceName);
	DDX_Text(pDX, IDC_NAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutBox, CPropertyPage)
	//{{AFX_MSG_MAP(CAboutBox)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutBox message handlers


void CAboutBox::FillObjectVersion(LPTSTR szProgID,CString& strFile, CString& strVer)
{
	CLSID cls;
	USES_CONVERSION;
	CLSIDFromProgID(T2W(szProgID), &cls);
	FillObjectVersion(cls,strFile,strVer);
}

void CAboutBox::FillObjectVersion(CLSID clsid,CString& strFile, CString& strVer)
{
	LPOLESTR szCLSID=NULL;
	StringFromCLSID (clsid, &szCLSID);
	
	CString strKey = _T("CLSID\\");
	strKey += CString  (szCLSID);
	CoTaskMemFree (szCLSID);

	CRegKey key;	
	long lResult =  key.Open(HKEY_CLASSES_ROOT, strKey , KEY_READ );
	if (lResult == ERROR_SUCCESS)
	{
		CRegKey keyModule;
		long lResult = keyModule.Open( key,  _T("LocalServer32"), KEY_READ );
		if (lResult != ERROR_SUCCESS)
			lResult = keyModule.Open( key,  _T("InprocServer32"), KEY_READ );

		if (lResult == ERROR_SUCCESS)
		{
			CString strVal;
			LPTSTR szVal = strVal.GetBuffer(MAX_PATH);
			DWORD dwSize = MAX_PATH;
			keyModule.QueryValue (szVal, _T(""), &dwSize);
			strVal.ReleaseBuffer();

			if (!strVal.IsEmpty() )
			{
				CModuleVersion ver;
				strVer = _T("N/A");

				if (ver.GetFileVersionInfo (strVal))
					ver.GetValue(_T("FileVersion"), strVer);
				
				WIN32_FIND_DATA data;
				HANDLE hFind = FindFirstFile (strVal, &data);
				if( hFind != INVALID_HANDLE_VALUE )
				{
					FindClose(hFind);

					strFile = data.cFileName;					
				}					
			}
		}

		key.Close();
	}
}

