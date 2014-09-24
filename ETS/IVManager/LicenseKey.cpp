// LicenseKey.cpp: implementation of the CLicenseKey class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ivanalyzer.h"
#include "LicenseKey.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LPCTSTR CLicenseKey::m_szAppName = _T("VolatilityAnalyzer");

double  CLicenseKey::m_dParityKey		= 78.34646;
long	CLicenseKey::m_nProductKeyLen	= 7;
long	CLicenseKey::m_nExpDateKeyLen	= 6;
long	CLicenseKey::m_nModulesKeyLen	= 7;
long	CLicenseKey::m_nParityKeyLen	= 4;


CLicenseKey::CLicenseKey() 
{
	InitMembers();
}

CLicenseKey::CLicenseKey(LPCTSTR szKey)
{
	InitMembers();
	InitFromStr (szKey);
}


CLicenseKey::~CLicenseKey()
{

}
CString CLicenseKey::GetProductID()
{
	CString strID;
	strID.Format (_T("%X"), m_nUID);

	return strID;
}

void CLicenseKey::InitUID()
{
	USES_CONVERSION;

	TCHAR szDir [MAX_PATH];
	::GetWindowsDirectory (szDir, MAX_PATH);
	
	TCHAR szDrive [MAX_PATH];  
	_tsplitpath_s (szDir, szDrive,MAX_PATH, NULL,0, NULL,0, NULL,0);
	
	DWORD dwSerialNumber=0;
    ::GetVolumeInformation (szDrive, NULL, 0, &dwSerialNumber, NULL, NULL, NULL, 0 );

	int nLen = _tcslen (szDrive);
	if (nLen < MAX_PATH)
	{
		szDrive[nLen] = _T('\\');
		szDrive[nLen+1] = _T('\0');
	}

	CString m_strUID;
	m_strUID.Format (_T("%s%X%s%s"), m_szAppName, dwSerialNumber, 
		(LPCTSTR)theApp.GetUser(), (LPCTSTR)theApp.GetCompany() );

	LPCSTR szUID = T2CA (   m_strUID );

	long nUID = CalcCRC32(szUID, m_strUID.GetLength() );
	nUID = nUID < 0 ? -nUID : nUID;

	m_nUID = nUID;
}

bool CLicenseKey::InitFromStr(LPCTSTR szKey)
{
	CString strKey = szKey;
	if (strKey.IsEmpty())
		return false;
	
	strKey.MakeUpper();

	CString strKeyLic = strKey.Left (m_nProductKeyLen + m_nModulesKeyLen + m_nExpDateKeyLen);
	CString strParity = strKey.Mid (m_nProductKeyLen + m_nModulesKeyLen + m_nExpDateKeyLen, m_nParityKeyLen);
	
	if (CalculateParity(strKeyLic)  !=  Base36ToDec (strParity) )
		return false;
	
	long nUID = Base36ToDec (  strKeyLic.Left (m_nProductKeyLen) );
	long nModules = Base36ToDec (  strKeyLic.Mid (m_nProductKeyLen, m_nModulesKeyLen));

	DATE dtExp = DecodeExpireDate (Base36ToDec(strKeyLic.Mid (m_nProductKeyLen + m_nModulesKeyLen, m_nExpDateKeyLen)));
	if ( (long)dtExp == 0 )
		return false;

	m_nUID = nUID;
	m_nModules = nModules;
	m_dtExpiration = dtExp;
	m_strKey = strKey;

	return true;
}

void CLicenseKey::InitMembers()
{
	m_nUID = 0;
	m_nModules = 0x7FFFFFFF;
	m_dtExpiration = 0;
}

long CLicenseKey::Base36ToDec(LPCTSTR szSource)
{
	int nLast = _tcslen (szSource) -1 ;

	long nResult = 0;
	for (int i = nLast; i>=0 ; i-- )
	{
		TCHAR ch = szSource[i];
		long nDigit = 0;

		if (IsCharAlpha (ch))
           nDigit = ch  - _T('A') + 10;
		else
           nDigit = ch  - _T('0');
		
		nResult += nDigit *  static_cast<long>(pow (36., nLast - i));
	}

	return nResult;
}


long CLicenseKey::CalculateParity(LPCTSTR szSource)
{
	int nLen = _tcslen (szSource);
	
	double dRetVal = 0;
	for (int i = 0; i < nLen; i++ )
        dRetVal += szSource[i] * m_dParityKey;

	return static_cast<long>(dRetVal);
}


DATE CLicenseKey::DecodeExpireDate(long nCode)
{
	long nDecoded = ((nCode & 0x2AAAAAAA) /2) | ((nCode & 0x15555555) *2);
	
	BYTE nDay   = static_cast<BYTE>((nDecoded / (long) pow (2., 22)) & 0xFF);
	BYTE nMonth = static_cast<BYTE>((nDecoded /(long) pow (2., 16)) & 0xF);
	WORD nYear  = static_cast<WORD>(nDecoded & 0xFFFF);

	SYSTEMTIME st;
	ZeroMemory (&st, sizeof (SYSTEMTIME) );

	st.wDay = nDay;
	st.wMonth = nMonth;
	st.wYear = nYear;
	
	DATE dtExpiration = 0;
	::SystemTimeToVariantTime(&st, &dtExpiration);

	return dtExpiration;
}

bool CLicenseKey::IsExpired()
{
	SYSTEMTIME st;
	::GetLocalTime(&st);
	
	DATE dtCur = 0;
	::SystemTimeToVariantTime(&st, &dtCur);

	return  (long) m_dtExpiration <  (long) dtCur;
}
