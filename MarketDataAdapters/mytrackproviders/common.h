#if !defined(AFX_COMMON_H_)
#define AFX_COMMON_H_
#include "MyTrackProviders.h"

#include <string>
#include <windows.h>
#include <atlbase.h>
#include <deque>
#include <recPtr.h>

typedef std::basic_string<TCHAR>  tstring;
//----------------------------------------------------------------------------------------//
typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateParams;
typedef CComRecPtr<QuoteUpdateResult, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateResult;
typedef CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateInfo;
typedef CComRecPtr<StockResults, &LIBID_PRICEPROVIDERSLib>		_StockResults;
typedef CComRecPtr<StockInfo, &LIBID_PRICEPROVIDERSLib>			_StockInfo;
typedef CComRecPtr<OptionResults, &LIBID_PRICEPROVIDERSLib>		_OptionResults;
typedef CComRecPtr<OptionInfo, &LIBID_PRICEPROVIDERSLib>			_OptionInfo;
typedef CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib>			_StockParams;
typedef CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib>		_OptionParams;
//----------------------------------------------------------------------------------------//
struct CMyTrackLoginInfo
{
	std::string	m_strUserName;
	std::string	m_strUserPass;
	std::string	m_strAppName;
	int	m_iRevHi;
	int	m_iRevLow;
	int	m_iTimeShift;
	int	m_iLogonTimeout;
	int	m_iRequestTimeout;
	
	// MyTrack SDK 3.0
	int m_iUseProxy; // 0 - Not Use
					 // 1 - Use Default
	                 // 2 - Use custom
	
	std::string m_strProxyName;
	int         m_iProxyPort;
	
	CMyTrackLoginInfo()
	{
		m_strUserName = "";
		m_strUserPass = "";
		m_strAppName = "";
		m_iRevHi = 1;
		m_iRevLow = 0;
		m_iTimeShift = 0;
		m_iLogonTimeout = 0;
		m_iRequestTimeout = 0;

		m_iUseProxy = 1;
		m_iProxyPort = 0;
		m_strProxyName = "";
	}
};
//----------------------------------------------------------------------------------------//
struct CMyTrackError
{
	int				m_iError;
	LPCTSTR			m_szMessage;
	ErrorNumberEnum	m_enExtError;
};
//----------------------------------------------------------------------------------------//
class CCommonException
{
public:

	static void ThrowError (HRESULT hRes, LPCOLESTR szError )
	{
		if (szError && wcslen (szError) > 0)
		{
			CComPtr <ICreateErrorInfo> spCreateErrorInfo;
			HRESULT hr = ::CreateErrorInfo (&spCreateErrorInfo);
			if (SUCCEEDED (hr))
			{
				spCreateErrorInfo->SetDescription (const_cast <LPOLESTR> (szError));
				CComQIPtr <IErrorInfo> spErrInfo = spCreateErrorInfo;
				ATLASSERT (spErrInfo);
				SetErrorInfo (0, spErrInfo);
				throw _com_error(hRes, spErrInfo);
			}
		}
		throw  _com_error(hRes);
	}
	
	static void ThrowError (HRESULT hRes, UINT nId )
	{
		CComBSTR spBuffer;
		spBuffer.LoadString (nId);
		ThrowError(hRes,(LPCOLESTR)spBuffer.m_str); 
	}
};

template< class _ValueType, long _MaxSize = 0L >
class CDoubleQueue
{
public:
	CDoubleQueue()
	{
		m_lPushQueue = 0L;
		m_lPopQueue = 1L;
	}

	void LockPush()
	{
		m_csPush.Lock();
	}

	void UnlockPush()
	{
		m_csPush.Unlock();
	}

	void Push(const _ValueType& Value)
	{
		if(_MaxSize > 0 && m_Queues[m_lPushQueue].size() == _MaxSize)
			m_Queues[m_lPushQueue].pop_front();

		m_Queues[m_lPushQueue].push_back(Value);
	}

	void LockPop()
	{
		m_csPop.Lock();
	}

	void UnlockPop()
	{
		m_csPop.Unlock();
	}

	void Pop()
	{
		m_Queues[m_lPopQueue].pop_front();
	}

	const _ValueType& Front() const
	{
		return m_Queues[m_lPopQueue].front();
	}

	const _ValueType& Back() const
	{
		return m_Queues[m_lPopQueue].back();
	}

	void Swap()
	{
		long lBuf = m_lPushQueue;
		m_lPushQueue = m_lPopQueue;
		m_lPopQueue = lBuf;
	}

	long PushSize() const
	{
		return m_Queues[m_lPushQueue].size();
	}

	long PopSize() const
	{
		return m_Queues[m_lPopQueue].size();
	}

	void ClearPush()
	{
		m_Queues[m_lPushQueue].clear();
	}

	void ClearPop()
	{
		m_Queues[m_lPopQueue].clear();
	}

	void Reset()
	{
		LockPop();
		LockPush();

		ClearPop();
		ClearPush();

		UnlockPush();
		UnlockPop();
	}

protected:
	long												m_lPushQueue;
	long												m_lPopQueue;
	CComAutoCriticalSection								m_csPush;
	CComAutoCriticalSection								m_csPop;
	std::deque<_ValueType>								m_Queues[2];
};
#endif
