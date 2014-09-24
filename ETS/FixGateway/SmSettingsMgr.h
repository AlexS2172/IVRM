#ifndef __SMSETTINGSMGR_H__
#define __SMSETTINGSMGR_H__

#include "FIXGGeneral.h"

namespace FXGSM
{


/////////////////////////////////////////////////////////////////////////////
//
class CSmSettingsMgr 
{
public:
	CSmSettingsMgr(void)
		: m_bInitialized(false), m_nDbConnectionTimeout(120L), m_nDbCommandTimeout(120L), 
		m_nCheckGatewaysTime(5L), m_dtStartFixTime(0, 0, 0, 6, 0, 0), m_dtStopFixTime(0, 0, 0, 22, 0, 0),
		m_bAutoStartStopFix(false)
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

	long CheckGatewaysTime() throw (exception)
	{
		_CheckInitialized();
		return m_nCheckGatewaysTime;
	}

	COleDateTime& StartFixTime() throw (exception)
	{
		_CheckInitialized();
		return m_dtStartFixTime;
	}

	COleDateTime& StopFixTime() throw (exception)
	{
		_CheckInitialized();
		return m_dtStopFixTime;
	}

	bool& AutoStartStopFix() throw (exception)
	{
		_CheckInitialized();
		return m_bAutoStartStopFix;
	}

	void StartFixTime(COleDateTime& newVal) throw (exception);
	void StopFixTime(COleDateTime& newVal) throw (exception);
	void AutoStartStopFix(bool& newVal) throw (exception);

private:
	void _CheckInitialized() throw (exception)
	{
		if(!m_bInitialized)
			throw logic_error(_T("Settings manager not initialized."));
	}

protected:
	bool							m_bInitialized;

	CString							m_sDbConnection;
	long							m_nDbConnectionTimeout;
	long							m_nDbCommandTimeout;

	long							m_nCheckGatewaysTime;
	COleDateTime					m_dtStartFixTime;
	COleDateTime					m_dtStopFixTime;
	bool							m_bAutoStartStopFix;
};

// object pointer
typedef boost::shared_ptr<CSmSettingsMgr>	CSmSettingsMgrPtr;

};

#endif //__SMSETTINGSMGR_H__