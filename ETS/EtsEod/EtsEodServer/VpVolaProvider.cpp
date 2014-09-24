#include "StdAfx.h"
#include "VpVolaProvider.h"

namespace EODVP
{
/////////////////////////////////////////////////////////////////////////////
//
CVpVolaProvider::CVpVolaProvider(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//
CVpVolaProvider::~CVpVolaProvider(void)
{
}

/////////////////////////////////////////////////////////////////////////////
//
void CVpVolaProvider::Connect(EODSM::CSmSettingsMgrPtr pSettingsMgr)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	EODSM::CSmSettingsMgrPtr spSettingsMgr(pSettingsMgr);
	if(!pSettingsMgr)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Invalid settings manager."));
		throw logic_error(_T("Invalid settings manager."));
	}

	Disconnect();

	try
	{
		IVolatilitySourcesPtr spVolaSources;
		_com_util::CheckError(spVolaSources.CreateInstance(CLSID_VolatilitySources));

		IVADBLayoutPtr spDbLayout;
		_com_util::CheckError(spDbLayout.CreateInstance(CLSID_DBLayout));
		IVAManagementPtr spVAManagement = spDbLayout;

		spDbLayout->ConnectionString = (LPCTSTR)spSettingsMgr->DbConnection();

		m_spVolaSource = spVolaSources->GetVolatilitySource(spSettingsMgr->VolaProviderType());
		m_spVolaSource->DataSource = spVAManagement;

		m_spVolaSource->EnableEvents = VARIANT_FALSE;
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
		m_spVolaSource = NULL;
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
void CVpVolaProvider::Disconnect()
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	m_spVolaSource = NULL;
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
}

/////////////////////////////////////////////////////////////////////////////
//
CVpUndVolaSourcePtr CVpVolaProvider::UndVolaSource(const CString& sUndSymbol, SYMBOL_TYPE enType)
{
	Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Start"));
	if(m_spVolaSource == NULL)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("Invalid settings manager."));
		throw exception(_T("Volatility provider is not initialized."));
	}

	try
	{
		IVSSymbolVolatilityPtr spSymbolVola = m_spVolaSource->GetSymbolVolatility(_bstr_t(sUndSymbol), enType);
	
		CVpUndVolaSourcePtr spUndVolaSource(new CVpUndVolaSource);
		spUndVolaSource->Init(spSymbolVola);
		Trace(EODCO::enTrLogDebug,__FUNCTION__,_T("Completed"));
		return spUndVolaSource;
	}
	catch(_com_error& e)
	{
		Trace(EODCO::enTrLogError,__FUNCTION__,_T("ComError exception %d %s"), e.Error(), (LPCTSTR)e.Description());
		CString strDescr;
		strDescr.Format(_T("%08x %s"), e.Error(), (LPCTSTR)e.Description());
		throw exception(strDescr);
	}
}

};