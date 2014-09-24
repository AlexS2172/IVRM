#include "StdAfx.h"
#include "SmSettingsMgr.h"

namespace EODSM
{

/////////////////////////////////////////////////////////////////////////////
//
void CSmSettingsMgr::Init()
{

	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	try
	{
		MGR::ISettingsPtr spSettings;
		MGR::ISettings2Ptr spSettings2;

		_com_util::CheckError(spSettings.CreateInstance(MGR::CLSID_Settings));
		try
		{
			_com_util::CheckError(spSettings2.CreateInstance(MGR::CLSID_Settings));
			spSettings2->InitForProfile("EOD");
		}
		catch(_com_error& e)
		{
			Trace(EODCO::enTrLogError,__FUNCTION__,_T(" There is no ISettings2 Interface... %d %s"), e.Error(), (LPCTSTR)e.Description());
		}
		
		m_sDbConnection = (LPTSTR)spSettings->DatabaseConnection;

		m_sDbConnection.Trim().MakeUpper();
/*		if(m_sDbConnection.Find(_T("INTEGRATED SECURITY=SSPI")) != -1)
			throw exception(_T("Service running under Local System account and MSSQL Server can't logon it under windows integrated security"));
*/
		m_nPriceProviderType = spSettings->PriceProvider;
		m_nVolaProviderType = spSettings->VolatilitySource;

		MGR::IIVSettingsPtr spIVSettings;
		_com_util::CheckError(spIVSettings.CreateInstance(MGR::CLSID_IVSettings));

		// initalize connection parameters
		m_sIVolServerName = (LPTSTR)spIVSettings->VolServerName;
		m_nIVolServerPort = spIVSettings->VolServerPort;
		m_sIVolLogin = (LPTSTR)spIVSettings->VolLogin;
		m_sIVolPassword = (LPTSTR)spIVSettings->VolPassword;
		m_enProxyAuthType = spIVSettings->ProxyAuthType;
		m_sProxyName = (LPTSTR)spIVSettings->ProxyName;
		m_nProxyPort = spIVSettings->ProxyPort;
		m_sUseProxy = spIVSettings->UseProxy;
		m_sProxyLogin = (LPTSTR)spIVSettings->ProxyLogin;
		m_sProxyPassword = (LPTSTR)spIVSettings->ProxyPassword;

		_LoadFromReg();
		m_bInitialized = true;
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

void CSmSettingsMgr::_LoadFromReg()
{
	m_nIvReconnectAttempt = 3L;
	m_nIvWaitInterval = 30L;
	m_nPpReconnectAttempt =3L;
	m_nPpWaitInterval = 30L;

	CXMLParamsHelper XMLParams;
	XMLParams.LoadXMLParams();

	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("IvReconnectAttempt"), &m_nIvReconnectAttempt, 3);
	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("IvWaitInterval"), &m_nIvWaitInterval, 30);
	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("PpReconnectAttempt"), &m_nPpReconnectAttempt, 3);
	XMLParams.GetXMLLong(c_strSettingsXMLKey, _T("PpWaitInterval"), &m_nPpWaitInterval, 30);
}

};