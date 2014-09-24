#include "StdAfx.h"
#include "SmSettingsMgr.h"

namespace FXGSM
{

static const _bstr_t    SETTINGS_MAIN_XML_KEY(L"ETS\\Asp\\Groups\\");
static const _bstr_t    SETTINGS_USER_XML_KEY(L"ETS\\OrdersGateway\\");

/////////////////////////////////////////////////////////////////////////////
//
void CSmSettingsMgr::Init()
{
	try
	{
		const _bstr_t bsEmptyString(_T(""));

		MGR::ISettingsPtr spSettings;
		_com_util::CheckError(spSettings.CreateInstance(MGR::CLSID_Settings));

		m_sDbConnection = (LPTSTR)spSettings->DatabaseConnection;
		m_sDbConnection.Trim().MakeUpper();

		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		_bstr_t bsTime;
		if(SUCCEEDED(XMLParams.GetXMLString(SETTINGS_USER_XML_KEY, L"FixStartTime", &bsTime, bsEmptyString)) && bsTime.length())
			m_dtStartFixTime.ParseDateTime(bsTime, VAR_TIMEVALUEONLY);

		if(SUCCEEDED(XMLParams.GetXMLString(SETTINGS_USER_XML_KEY, L"FixStopTime", &bsTime, bsEmptyString)) && bsTime.length())
			m_dtStopFixTime.ParseDateTime(bsTime, VAR_TIMEVALUEONLY);

		long nAutoStart = 0;
		XMLParams.GetXMLLong(SETTINGS_USER_XML_KEY, L"AutoStartStop", &nAutoStart);
		m_bAutoStartStopFix = nAutoStart != 0;

		//CRegKey aKeyLoc, aKeyCur;
		//bool bIsLM = false, bIsCU = false;
		//LONG lResult = aKeyCur.Open(HKEY_CURRENT_USER, REG_KEY, KEY_READ);		
		//if(lResult == ERROR_SUCCESS)
		//	bIsCU = true;

		//lResult = aKeyLoc.Open (HKEY_LOCAL_MACHINE, REG_KEY, KEY_READ);
		//if(lResult == ERROR_SUCCESS)
		//	bIsLM = true;

		//lResult = -1L;
		//DWORD dwVal;
		//if(bIsCU)
		//	lResult = aKeyCur.QueryDWORDValue(_T("CheckGatewaysTime"), dwVal);				
		//
		//if (lResult != ERROR_SUCCESS && bIsLM)
		//	lResult = aKeyLoc.QueryDWORDValue(_T("CheckGatewaysTime"), dwVal);

		//if(ERROR_SUCCESS == lResult)
		//	m_nCheckGatewaysTime = dwVal;

		//lResult = -1L;
		//if(bIsCU)
		//	lResult = aKeyCur.QueryDWORDValue(_T("AutoStartStop"), dwVal);				

		//if (lResult != ERROR_SUCCESS && bIsLM)
		//	lResult = aKeyLoc.QueryDWORDValue(_T("AutoStartStop"), dwVal);

		//if(ERROR_SUCCESS == lResult)
		//	m_bAutoStartStopFix = (dwVal != 0L);

		//lResult = -1L;
		//unsigned long  ulSize = 0;
		//CString sTime;

		//if(bIsCU)
		//	lResult = aKeyCur.QueryStringValue(_T("StartTime"),NULL,&ulSize);

		//if (lResult == ERROR_SUCCESS && ulSize > 0)
		//{
		//	long nLen = ulSize / sizeof (TCHAR);
		//	TCHAR* szBuff = new TCHAR [nLen + 1 ];

		//	lResult = aKeyCur.QueryStringValue(_T("StartTime"), szBuff, &ulSize );
		//	if (lResult == ERROR_SUCCESS)
		//		sTime = szBuff;
		//	delete [] szBuff;
		//}
		//
		//ulSize = 0;
		//if (lResult != ERROR_SUCCESS && bIsLM)
		//	lResult = aKeyLoc.QueryStringValue(_T("StartTime"),NULL,&ulSize);

		//if (lResult == ERROR_SUCCESS && ulSize > 0)
		//{
		//	long nLen = ulSize / sizeof (TCHAR);
		//	TCHAR* szBuff = new TCHAR [nLen + 1 ];

		//	lResult = aKeyLoc.QueryStringValue(_T("StartTime"), szBuff, &ulSize );
		//	if (lResult == ERROR_SUCCESS)
		//		sTime = szBuff;
		//	delete [] szBuff;
		//}

		//if(ERROR_SUCCESS == lResult)
		//	m_dtStartFixTime.ParseDateTime(sTime, VAR_TIMEVALUEONLY);

		//lResult = -1L;
		//ulSize = 0;

		//if(bIsCU)
		//	lResult = aKeyCur.QueryStringValue(_T("StopTime"),NULL,&ulSize);

		//if (lResult == ERROR_SUCCESS && ulSize > 0)
		//{
		//	long nLen = ulSize / sizeof (TCHAR);
		//	TCHAR* szBuff = new TCHAR [nLen + 1 ];

		//	lResult = aKeyCur.QueryStringValue(_T("StopTime"), szBuff, &ulSize );
		//	if (lResult == ERROR_SUCCESS)
		//		sTime = szBuff;
		//	delete [] szBuff;
		//}

		//ulSize = 0;
		//if (lResult != ERROR_SUCCESS && bIsLM)
		//	lResult = aKeyLoc.QueryStringValue(_T("StopTime"),NULL,&ulSize);

		//if (lResult == ERROR_SUCCESS && ulSize > 0)
		//{
		//	long nLen = ulSize / sizeof (TCHAR);
		//	TCHAR* szBuff = new TCHAR [nLen + 1 ];

		//	lResult = aKeyLoc.QueryStringValue(_T("StopTime"), szBuff, &ulSize );
		//	if (lResult == ERROR_SUCCESS)
		//		sTime = szBuff;
		//	delete [] szBuff;
		//}

		//if(ERROR_SUCCESS == lResult)
		//	m_dtStopFixTime.ParseDateTime(sTime, VAR_TIMEVALUEONLY);

		m_bInitialized = true;
	}
	catch(_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}
}

void CSmSettingsMgr::StartFixTime(COleDateTime& newVal)
{
	try
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		XMLParams.SetXMLString(SETTINGS_USER_XML_KEY, L"FixStartTime", (LPCTSTR)newVal.Format(VAR_TIMEVALUEONLY));
		XMLParams.SaveXMLParams();
	}
	catch (_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("Can't save FixStartTime value: %08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}

	m_dtStartFixTime = newVal;
}

void CSmSettingsMgr::StopFixTime(COleDateTime& newVal)
{
	try
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		XMLParams.SetXMLString(SETTINGS_USER_XML_KEY, L"FixStopTime", (LPCTSTR)newVal.Format(VAR_TIMEVALUEONLY));
		XMLParams.SaveXMLParams();
	}
	catch (_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("Can't save FixStopTime value: %08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}

	m_dtStopFixTime = newVal;
}

void CSmSettingsMgr::AutoStartStopFix(bool& newVal)
{
	try
	{
		CXMLParamsHelper XMLParams;
		XMLParams.LoadXMLParams();

		XMLParams.SetXMLLong(SETTINGS_USER_XML_KEY, L"AutoStartStop", (long)newVal ? 1 : 0);
		XMLParams.SaveXMLParams();
	}
	catch (_com_error& e)
	{
		CString strDescr;
		strDescr.Format(_T("Can't save AutoStartStop value: %08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}

	m_bAutoStartStopFix = newVal;
}

};