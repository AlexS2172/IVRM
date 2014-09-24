#ifndef __SMSETTINGSMGR_H__
#define __SMSETTINGSMGR_H__

#include "EodGeneral.h"

namespace EODSM
{


/////////////////////////////////////////////////////////////////////////////
//
class CSmSettingsMgr :  public EODCO::CEgTraceable
{
public:
	CSmSettingsMgr(void)
		: m_bInitialized(false), m_nPriceProviderType(0L), m_nVolaProviderType(0L),
		m_nDbConnectionTimeout(220L), m_nDbCommandTimeout(500L),
		m_nIVolServerPort(0L), m_enProxyAuthType(MGR::enProxyAuthDisable),
		m_nProxyPort(0L), m_sUseProxy(MGR::enProxyDisable),
		m_nIvReconnectAttempt(0L), m_nIvWaitInterval(0L)
	{
	}

	~CSmSettingsMgr(void)
	{
	}

	void Init() throw (exception);

	CString DbConnection() throw (exception)
	{
		_CheckInitialized();
		return m_sDbConnection;
	}

	long DbConnectionTimeout() throw (exception)
	{
		_CheckInitialized();
		return m_nDbConnectionTimeout;
	}

	long DbCommandTimeout() throw (exception)
	{
		_CheckInitialized();
		return m_nDbCommandTimeout;
	}


	long PriceProviderType() throw (exception)
	{
		_CheckInitialized();
		return m_nPriceProviderType;
	}

	long VolaProviderType() throw (exception)
	{
		_CheckInitialized();
		return m_nVolaProviderType;
	}
	
	CString	IVolalilityServerName() throw (exception)
	{
		_CheckInitialized();
		return m_sIVolServerName;
	}

	long IVolatilityServerPort() throw (exception)
	{
		_CheckInitialized();
		return m_nIVolServerPort;
	}

	CString	IVolatilityLogin() throw (exception)
	{
		_CheckInitialized();
		return m_sIVolLogin;
	}
	
	CString	IVolatilityPassword() throw (exception)
	{
		_CheckInitialized();
		return m_sIVolPassword;
	}

	MGR::ProxyAuthentificationType	ProxyAuthType() throw (exception)
	{
		_CheckInitialized();
		return m_enProxyAuthType;
	}
	CString	ProxyName() throw (exception)
	{
		_CheckInitialized();
		return m_sProxyName;
	}
	long ProxyPort() throw (exception)
	{
		_CheckInitialized();
		return m_nProxyPort;
	}
	MGR::UseProxyType	UseProxy() throw (exception)
	{
		_CheckInitialized();
		return m_sUseProxy;
	}
	CString	ProxyLogin() throw (exception)
	{
		_CheckInitialized();
		return m_sProxyLogin;
	}
	CString	ProxyPassword() throw (exception)
	{
		_CheckInitialized();
		return m_sProxyPassword;
	}
	long IvReconnectAttempt() throw (exception)
	{
		_CheckInitialized();
		return m_nIvReconnectAttempt;
	}
	long IvWaitInterval() throw (exception)
	{
		_CheckInitialized();
		return m_nIvWaitInterval;
	}
	long PpReconnectAttempt() throw (exception)
	{
		_CheckInitialized();
		return m_nPpReconnectAttempt;
	}
	long PpWaitInterval() throw (exception)
	{
		_CheckInitialized();
		return m_nPpWaitInterval;
	}
private:
	void _CheckInitialized() throw (exception)
	{
		if(!m_bInitialized)
			throw logic_error(_T("Settings manager not initialized."));
	}
	void _LoadFromReg();

protected:
	bool							m_bInitialized;

	CString							m_sDbConnection;
	long							m_nDbConnectionTimeout;
	long							m_nDbCommandTimeout;

	long							m_nPriceProviderType;
	long							m_nVolaProviderType;


	CString							m_sIVolServerName;
	long							m_nIVolServerPort;
	CString							m_sIVolLogin;
	CString							m_sIVolPassword;
	MGR::ProxyAuthentificationType	m_enProxyAuthType;
	CString							m_sProxyName;
	long							m_nProxyPort;
	MGR::UseProxyType				m_sUseProxy;
	CString							m_sProxyLogin;
	CString							m_sProxyPassword;

	long							m_nIvReconnectAttempt;
	long							m_nIvWaitInterval;
	long							m_nPpReconnectAttempt;
	long							m_nPpWaitInterval;
};

// object pointer
typedef boost::shared_ptr<CSmSettingsMgr>		CSmSettingsMgrPtr;

};

#endif //__SMSETTINGSMGR_H__