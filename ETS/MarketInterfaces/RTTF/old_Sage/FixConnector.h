#ifndef _FIX_CONNECTOR_H
#define _FIX_CONNECTOR_H

#include "sgconst.h"
#include "egtcpdriver.h"
#include "ThreadWrapper.h"
#include "FixMessage.h"
#include "FixGap.h"


class CFixConnector : public IEgTcpDriverEvents,
					   public CThreadWrapper
{

// ThreadWrapper methods
private:

	UINT  virtual __stdcall Thread(void* pParam);

// TcpDriver Events
private:

	virtual BOOL WINAPI OnConnect(const CONNECTION_ID & Connection, BOOL bIncoming);

	virtual void WINAPI OnClose(const CONNECTION_ID & Connection, const DWORD & dwError);
	
	virtual void WINAPI OnReceive(const CONNECTION_ID & Connection, const LPVOID const pData, const DWORD dwDataLen);
	
	virtual void WINAPI OnConnectionFailed(const CONNECTION_ID& Connection);


// Own public methods
public:

	CFixConnector():m_hReconnectTimer(NULL),m_hHeartBeatTimer(NULL), m_hLogoutEvent(NULL), 
			m_hForcedShutdownTimer(NULL),
			m_bLoggingOut(FALSE), m_bConnected(FALSE), m_nOutgoingSeqNo(1), m_nIncomingSeqNo(1),
			m_dwLogonTries(0){}
		
	DWORD __stdcall Start(void* pParam);

	DWORD __stdcall Stop();

// Own private methods
private:

	void OnDropConnection();

	void DropConnection();

	void Logon();

	bool Logout();

	DWORD Send(CFixMessage& Msg);
	
	void QueueReconnectAttempt(long lTimer);
	
	void QueueHeartBeat(long lInterval = HEART_BIT_INTERVAL);

	bool QueueForcedShutdown(long lInterval = SHUTDOWN_TIMEOUT);

	bool CheckSeqNum(const CFixMessage& Msg);

// settings
private:

	DWORD SaveSettings();

	DWORD ReadSettings();

// OnXXXmessage
private:

	bool OnMessage(CFixMessage& Msg);

	bool OnLogonMessage(CFixMessage& Msg);
	
	bool OnLogoutMessage(CFixMessage& Msg);

	bool OnHeartBeatMessage(CFixMessage& Msg);

	bool OnTestRequestMessage(CFixMessage& Msg);

	bool OnResendRequestMessage(CFixMessage& Msg);

	bool OnSequenceResetMessage(CFixMessage& Msg);

	bool OnRejectMessage(CFixMessage& Msg);

protected:

	virtual bool OnApplicationMessage(CFixMessage& Msg) = 0;

private:

	BOOL				m_bConnected;

	CEgTcpDriver		m_Driver;

	HANDLE				m_hReconnectTimer;
	HANDLE				m_hHeartBeatTimer;
	HANDLE				m_hLogoutEvent;
	HANDLE				m_hForcedShutdownTimer;
	DWORD				m_dwLogonTries;

	SOCKET				m_Socket;

	CFixBuffer			m_IncomingDataQueue;

	CLock				m_Lock;
	int					m_nOutgoingSeqNo;
	int					m_nIncomingSeqNo;
	int					m_nReconnectTime;
	_bstr_t				m_bsServerAddr;
	int					m_nServerPort;

	BOOL				m_bLoggingOut;

	CFixGap				m_Gap;
};

template <class T>
void GetSetting(LPCTSTR lpszValue, T& Value) throw(_com_error)
{
	USES_CONVERSION;

   	CEgRegKey key;
	LONG lResult = ERROR_INVALID_FUNCTION;//key.Open(HKEY_CURRENT_USER, SETTINGS_REGISTRY_KEY, KEY_READ);
    if (lResult == ERROR_SUCCESS)
    {
       	lResult = key.QueryValue(Value, lpszValue);
        key.Close();
    }
    else
    {
	    lResult = key.Open (HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_READ);        
        if (lResult == ERROR_SUCCESS)
        {
           	lResult = key.QueryValue(Value, lpszValue);
            key.Close();
        }
    }

    HRESULT hr = HRESULT_FROM_WIN32(lResult);
	if ( FAILED(hr))
	{
        try
        {
            _bstr_t bsMsg = L"Failed to read <";
            bsMsg += lpszValue;
            bsMsg += L"> setting value from the registry.";
            utils::ThrowErrorNoSetErrorInfo(hr, bsMsg);
        }
	    catch (const _com_error& e)
		{   
			CTracer::TraceError(e.Error(), W2T(e.Description()) );
            throw;
		}
	}
}

template <class T>
void SetSetting(LPCTSTR lpszValue, const T& Value) throw(_com_error)
{
   	CEgRegKey key;
	LONG lResult = key.Open(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY, KEY_WRITE);
	if (lResult != ERROR_SUCCESS )
        lResult = key.Create(HKEY_LOCAL_MACHINE, SETTINGS_REGISTRY_KEY);

    HRESULT hr = HRESULT_FROM_WIN32(lResult);
	if ( SUCCEEDED(hr))
	{
        try
	    {
		    lResult = key.SetValue(Value, lpszValue);
			if (lResult)
            {
                _bstr_t bsMsg = L"Failed to write <";
                bsMsg += lpszValue;
                bsMsg += L"> setting value to the registry.";
                utils::ThrowErrorNoSetErrorInfo(HRESULT_FROM_WIN32(lResult), bsMsg);
            }
    	}
	    catch (const _com_error& e)
		{
            USES_CONVERSION;
			CTracer::TraceError(e.Error(), W2T(e.Description()) );
            throw;
		}
	
		key.Close();
	}
}

#endif //_FIX_CONNECTOR_H