#ifndef __TRANSPORT_CONTROL_H__
#define __TRANSPORT_CONTROL_H__

class CTpControlApp;

typedef IDispEventImpl<1, CTpControlApp, &DIID__IHostProxyEvents> _IHostProxyEventsImpl;

typedef map < DWORD,CString > IP_MAP;

class CTpControlApp : public CWinApp,
	public _IHostProxyEventsImpl
{
public:
	CTpControlApp();
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	//{{AFX_MSG(CRtControlApp)
	afx_msg void OnTpStart();
	afx_msg void OnUpdateTpStart(CCmdUI* pCmdUI);
	afx_msg void OnTpStop();
	afx_msg void OnUpdateTpStop(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

/************************************************************************************************
	Utility methods
************************************************************************************************/
public:	
	static DWORD GetRouterHostName(_bstr_t& bstrHostName);
	static DWORD SetRouterHostName(_bstr_t bstrHostName);
	static DWORD GetSettingsFromReg(LPCTSTR ValueName,DWORD dwDefaultValue,LPCTSTR KeyName = SETTINGS_NETWORK_REGISTRY_KEY);
	static DWORD SetSettingsToReg(LPCTSTR ValueName,DWORD dwValue,LPCTSTR KeyName = SETTINGS_NETWORK_REGISTRY_KEY);
	static CString GetSettingsFromReg(LPCTSTR ValueName,CString strDefaultValue,LPCTSTR KeyName = SETTINGS_NETWORK_REGISTRY_KEY);
	static DWORD SetSettingsToReg(LPCTSTR ValueName,CString strValue,LPCTSTR KeyName = SETTINGS_NETWORK_REGISTRY_KEY);

	DWORD InitIPAddresses()
	{
		m_mapIPAddresses.clear();

		m_mapIPAddresses[INADDR_ANY] = _T("INADDR_ANY");

		TCHAR	szLocalName[MAX_COMPUTERNAME_LENGTH + 1];
		int	nLocalNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		
		if(gethostname(szLocalName, nLocalNameSize))
		{
			return WSAGetLastError();
		}

	    LPHOSTENT   lpHostEnt;

		if (NULL == (lpHostEnt = gethostbyname(szLocalName)))
		{
			return WSAGetLastError();
		}
		else
		{
			in_addr addr;
			int i = 0;
			while(lpHostEnt->h_addr_list[i])
			{
				ZeroMemory(&addr,sizeof(addr));

				CopyMemory(&addr, 
					lpHostEnt->h_addr_list[i++],
					lpHostEnt->h_length);
					
					m_mapIPAddresses[addr.s_addr] = inet_ntoa(*(in_addr*)&addr);

					DWORD dwAddr1 = inet_addr(m_mapIPAddresses[addr.s_addr]);
					DWORD dwAddr2 = addr.s_addr;
			}
		}
		return 0;		
	}
	
	bool GetFirstIPAddress(DWORD& dwAddr, CString& strAddr)
	{
		IP_MAP::iterator iter = m_mapIPAddresses.begin();
		if(iter != m_mapIPAddresses.end())
		{
			dwAddr = iter->first;
			strAddr = iter->second;
			return true;
		}

		return false;
	}

	bool GetNextIPAddress(DWORD& dwAddr, CString& strAddr)
	{
		IP_MAP::iterator iter = m_mapIPAddresses.upper_bound(dwAddr);
		if(iter != m_mapIPAddresses.end())
		{
			dwAddr = iter->first;
			strAddr = iter->second;
			return true;
		}
		return false;
	}

	long GetIPAddressIndex(DWORD dwAddr)
	{
		if(m_mapIPAddresses.empty())
			return -1;

		IP_MAP::iterator iter = m_mapIPAddresses.begin();		
		long l = 0;

		while(iter != m_mapIPAddresses.end())
		{
			if(iter->first == dwAddr)
				return l;
			l++;
			iter++;
		}
		return -1;
	}
	HRESULT AttachToHost();
	void DetachFromHost();
	HRESULT GetHostStatus(CString& strRouterName);
	void RestartTransportService();
	DWORD StopTransportService(SC_HANDLE hService, DWORD dwTimeout );
	void GetStatistic();

	DWORD getTpSrvStatus()
	{
		CAutoLock lock(m_Lock);

		return m_nTpStatus;
	}

	void setTpSrvStatus(DWORD nTpStatus)
	{
		CAutoLock lock(m_Lock);
		
		m_nTpStatus = nTpStatus;
	}
	HostStatusEnum getTpStatus()
	{
		CAutoLock lock(m_Lock);
		return m_TransportStatus;
	}

	HostStatusEnum setTpStatus(HostStatusEnum Status)
	{
		CAutoLock lock(m_Lock);
		
		m_TransportStatus = Status;

		return m_TransportStatus;

	}

	bool DoesTransportServiceStarted();

public:
BEGIN_SINK_MAP (CTpControlApp)
	SINK_ENTRY_INFO(1, DIID__IHostProxyEvents, DISPID_IHostProxyEventsOnServiceStatus, OnServiceStatus, &m_OnServiceStatus)
	SINK_ENTRY_INFO(1, DIID__IHostProxyEvents, DISPID_IHostProxyEventsOnServiceError, OnServiceError, &m_OnServiceError)
	SINK_ENTRY_INFO(1, DIID__IHostProxyEvents, DISPID_IHostProxyEventsOnLogoned, OnLogoned, &m_OnLogoned)
	SINK_ENTRY_INFO(1, DIID__IHostProxyEvents, DISPID_IHostProxyEventsOnLogout, OnLogout, &m_OnLogout)
	SINK_ENTRY_INFO(1, DIID__IHostProxyEvents, DISPID_IHostProxyEventsOnDisabled, OnDisabled, &m_OnDisabled)
END_SINK_MAP()

protected:
	static _ATL_FUNC_INFO	m_OnServiceStatus;
	static _ATL_FUNC_INFO	m_OnServiceError;
	static _ATL_FUNC_INFO	m_OnLogoned;
	static _ATL_FUNC_INFO	m_OnLogout;
	static _ATL_FUNC_INFO	m_OnDisabled;

	STDMETHOD(OnServiceStatus)(long dwStatus);
	STDMETHOD(OnServiceError)(long nCode, BSTR bsDescription);
	STDMETHOD(OnLogoned)(BSTR bsRouterName);
	STDMETHOD(OnLogout)(BSTR bsRouterName);
	STDMETHOD(OnDisabled)();

	IHostProxyPtr				m_spHost;

public:
	long					m_ulUnsentMessNum;
	long					m_ulRecMessNum;
	long					m_ulSendMessNum;
	long					m_ulConfMessNum;
	DATE					m_dtLogonTime;
	HICON					m_hAppIcon[2];
	_bstr_t				    m_bsRouterName;

private:
	HostStatusEnum			m_TransportStatus;
	DWORD					m_nTpStatus;
	CLock					m_Lock;
	IP_MAP					m_mapIPAddresses;

	HANDLE					m_hSingleInstanceMutex;
}; 

extern CTpControlApp	theApp;

#endif // !defined(__TRANSPORT_CONTROL_H__)
