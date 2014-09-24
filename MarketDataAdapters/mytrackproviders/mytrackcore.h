// MyTrackCore.h: interface for the CMyTrackCore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTRACKCORE_H__C00C4BAD_C048_4068_B44F_4A2B6A4BF730__INCLUDED_)
#define AFX_MYTRACKCORE_H__C00C4BAD_C048_4068_B44F_4A2B6A4BF730__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BaseNotifier.h"
#include "common.h"
#include <string>
#include <list>
#include <map>
#include <algorithm>
#include <math.h>

#include <cswrapper.h>
#include <eg_lib/egMisc.h>
using namespace eg_lib;

#define TIMEOUT_CONNECT			2*1000 //2 seconds waiting for send next package
#ifdef _DEBUG
	#define TIMEOUT_MYTRACK		1*1000 
#else
	#define TIMEOUT_MYTRACK		2*1000 //2 seconds waiting while send next symbols
#endif

#define TIMEOUT_LOGON			60 //1 minute wating timeout
#define DEF_OPT_LOTSIZE			100
#define DEF_STK_LOTSIZE			1
#define DEF_MYTRACK_PACKET		10
#define DEF_CURRENCY			L"USD"
#define DEF_INET_SETTINGS		_T("Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings")

#define TIMEOUT_LOSTMESSAGE TIMEOUT_LOGON*2
#define TIMEOUT_LOSTMSG     TIMEOUT_LOGON	   

//--------------------------------------------------------------------------------------------------//
class CNotifierFind
{
public:
	CNotifierFind(CBaseNotifier* pNotifier)
	{
		m_pNotifier = pNotifier;
	}
	bool operator()(CBaseNotifier* pNotifier)
	{
		return memcmp(pNotifier,m_pNotifier,sizeof(CBaseNotifier))==0;
	}	
protected:
	CBaseNotifier*		m_pNotifier;
	
};
//--------------------------------------------------------------------------------------------------//
struct CRequestData
{
	CBaseNotifier*		m_pNotifier;
	std::string			m_strOriginal;
	std::string			m_strSymbol;
	std::string			m_strOption;
	std::string			m_strDescription;
	
	vt_date				m_dtRequest;

	bool				m_bSendResponce;
	CComVariant			m_varRequest; //now it QuoteUpdateParams
	RequestsTypeEnum	m_enRequestType;
	
	bool				m_bSendNotification;
	CRequestData()
	{
		m_pNotifier = NULL;
		m_strSymbol  = "";
		m_strOption = "";
		m_strOriginal = "";
		m_bSendResponce = true;
		m_strDescription = "";
		m_dtRequest = vt_date::GetCurrentDate();

		m_bSendNotification = true;
		m_varRequest.Clear ();
	}
	CRequestData &operator=(const CRequestData& tagData)
	{
		m_pNotifier = tagData.m_pNotifier;
		m_strSymbol  = tagData.m_strSymbol;
		m_strOption = tagData.m_strOption;
		m_strOriginal = tagData.m_strOriginal;
		m_bSendResponce = tagData.m_bSendResponce;
		m_varRequest = tagData.m_varRequest;
		m_enRequestType = tagData.m_enRequestType;
		m_strDescription = tagData.m_strDescription;
		m_dtRequest = tagData.m_dtRequest;

		m_bSendNotification = tagData.m_bSendNotification;
		return *this;
	}

	~CRequestData()
	{
		m_varRequest.Clear ();
	}

	bool CheckOperationTime(const vt_date& dt)
	{
		switch(m_enRequestType)
		{
		case enNoRequest:
			return true;
		case enSubscribeQuote:
			if(m_bSendNotification && m_pNotifier && m_pNotifier->GetClientType() == enPriceInfoWithNotify)
			{
				int   lPeriod = abs((dt.get_minute()*60+dt.get_second())-(m_dtRequest.get_minute()*60+dt.get_second()));
				return lPeriod<TIMEOUT_LOSTMSG;
			}
			return true;
		case enRequestOptions:
			{
				int   lPeriod = abs((dt.get_minute()*60+dt.get_second())-(m_dtRequest.get_minute()*60+dt.get_second()));
				return lPeriod<TIMEOUT_LOSTMSG*5;
			}			 
		case enRequestStock:
		case enRequestLastQuote:
			{
				int   lPeriod = abs((dt.get_minute()*60+dt.get_second())-(m_dtRequest.get_minute()*60+dt.get_second()));
				return lPeriod<TIMEOUT_LOSTMSG;
			}
		}
		return true;
	}

};
//--------------------------------------------------------------------------------------------------//
typedef std::map<int,CRequestData>			CResponceMap;
typedef std::list<CBaseNotifier*>			CClientList;
typedef std::list<CRequestData*>			CRequestBuffer;

struct CBufferInfo
{
	int				m_reqNum;
	MYTRACK_MSG		*m_pMsg;
};
typedef CDoubleQueue<CBufferInfo> CMsgBuffer;

//--------------------------------------------------------------------------------------------------//
typedef enum 
{	
	errConnect,
	errOther,
	errConnectFail
} CInnerErrStatus;	
//--------------------------------------------------------------------------------------------------//
class CMyTrackCore  
{
public:
    
	CMyTrackCore();
	virtual ~CMyTrackCore();
	long &GetNewReqNum()
	{ 
		InterlockedIncrement(&m_lReqCounter);
		return 	m_lReqCounter;
	}

	void Connect(CBaseNotifier *pNotifier);
	void Disconnect(CBaseNotifier *pNotifier);

	void RequestLastQuote(	CBaseNotifier *pNotifier, 
							InstrumentTypeEnum enType, 
							LPCSTR lpszSymbol,
							LPCSTR lpszExchange,
							const CComVariant &varRequest);

	void CancelLastQuote(CBaseNotifier *pNotifier,LPCSTR lpszSymbol);
	
	void RequestStock(	CBaseNotifier *pNotifier, 
						LPCSTR lpszSymbol,
						const CComVariant &varRequest);
	
	void CancelStock(CBaseNotifier *pNotifier,LPCSTR lpszSymbol);
	

	void SubscribeQuote(CBaseNotifier *pNotifier, 
						InstrumentTypeEnum enType, 
						LPCSTR lpszSymbol,
						LPCSTR lpszExchange,
						const CComVariant &varRequest,
						bool bCheck =  true);

	void UnsubscribeQuote(CBaseNotifier *pNotifier,LPCSTR lpszSymbol);

	void RequestOptions(CBaseNotifier *pNotifier, 
						LPCSTR lpszUnderlyingSymbol,
						const CComVariant &varRequest,
						bool bCheck =  true);
	void CancelOptions(CBaseNotifier *pNotifier,LPCSTR lpszUnderlyingSymbol);
	
	
	HANDLE& GetStopEvent()
	{ return m_hStopEvent; }

	HANDLE& GetOneMsgEvent()
	{ return m_hOneMsgEvent; }
	
	HANDLE& GetOneRequestEvent()
	{ return m_hOneRequest; }
	
	HANDLE& GetConnectMutex()
	{ return m_hConnectionMutex; }
		
	
	CResponceMap* GetResponceMap()
	{ return m_pmapResponse; }

	CRequestBuffer& GetRequestBuffer()
	{ return m_RequestBuffer; }
	

	CClientList	*GetClients()
	{ return &m_lstClients; }
	
	void RaiseMyTrackError(const int &Error,
							CComVariant &varRequest, 
							CBaseNotifier *pNotifier=NULL,
							RequestsTypeEnum enReqType=enNoRequest);
	
	void RaiseComError(const _com_error & e, CRequestData &Request);	
/*	
	bool IsDisconnected(int iErrCode)
	{
		return (iErrCode==MT_ERR_DISCONNECT || 
			iErrCode==MT_ERR_NO_HB_FROM_SERVER || 
			iErrCode==MT_ERR_NO_CONNECT ||
			iErrCode==MTS_ERR_NO_HEART_BEAT);
	}
*/
	//call only from single thread
	CInnerErrStatus CheckErrConnection(int iErrCode)
	{
//		if (!IsDisconnected(iErrCode))
//			return errOther;
		CInnerErrStatus Result  = errConnectFail;
		CBaseNotifier::WaitObject (m_hConnectionMutex);
		try
		{
			m_bIsConnected = false;
			mtRequestLogoff (GetNewReqNum());
			mtDisconnect ();
			if(WAIT_TIMEOUT==CBaseNotifier::WaitWithEvents(1,&m_hStopEvent,TIMEOUT_CONNECT*10))
			{
				if(_Reconnect())
				{
					Result  = errConnect;
					SetEvent(m_hResubscribeAll);
				}
				else
				{
					mtRequestLogoff (GetNewReqNum());
					mtDisconnect();
				}
			}
		}
		catch (...)
		{}
		::ReleaseMutex (m_hConnectionMutex);
		return Result;
	}	

	CComAutoCriticalSection& GetResponceMapCS(){return m_csRespMap;}

	CComAutoCriticalSection& GetRequestBufferCS(){return m_csRequestBuffer;}
	
	void CommonQuoteResponce(QUOTE_RESPONSE *pMsg,CRequestData *pData);

	const int &GetTimeShift()
	{ return m_tagLoginInfo.m_iTimeShift; }

	void EraseSubscription(int *iReqNum)
	{
		CCriticalSectionWrapper  csWrap(m_csRespMap);
		CResponceMap::iterator itrFind = m_pmapResponse->find (*iReqNum);
		if (itrFind!=m_pmapResponse->end())
			m_pmapResponse->erase (itrFind);
	}

	CMsgBuffer	*GetBufferQueue()
	{ return m_pMsgBuffer;}
	
	HANDLE	*GetEventMutex()
	{ return &m_hEventMutex;}

	void SetPriceProviderMode ( PriceProviderTypeEnum type);
	PriceProviderTypeEnum GetPriceProviderMode();

	
protected:
	void ProcessMessages();

	void ResubscribeAll();
	
	void StockQuoteResponce(QUOTE_RESPONSE *pMsg,CRequestData *pData);
	
	void RemoveClient(CBaseNotifier *pNotifier);
	
	void CancelQuoteResponce(CBaseNotifier *pNotifier,LPCSTR lpszSymbol,RequestsTypeEnum enType);
	
	void WaitQueue();

//-----------------------------------------------------------------------------------------------//
	bool _Reconnect()
	{
		bool bRet = false;
		int err_code =0;
		try
		{

			if (!LoadProviderInfo()) return false;
			if(!m_tagLoginInfo.m_strUserName.length()) return false;
			if(!m_tagLoginInfo.m_strUserPass.length()) return false;

			if(m_tagLoginInfo.m_iUseProxy)
			{
				err_code =	mtSetUsingTunnel(TRUE);
				if (err_code)
					 return false;
				if(1 == m_tagLoginInfo.m_iUseProxy)
				{
					err_code = mtSetProxy((LPSTR)m_tagLoginInfo.m_strProxyName.c_str(), m_tagLoginInfo.m_iProxyPort);
					if (err_code)
						return false;
				}
			}
			else
			{
				err_code =	mtSetUsingTunnel(FALSE);
				if (err_code)
					 return false;

			}

			err_code = mtConnectToServer();
			if (err_code)
			{
				 return false;
			}


			err_code = mtRequestLogon(GetNewReqNum(),
									(LPSTR) m_tagLoginInfo.m_strUserName.c_str(),
									(LPSTR) m_tagLoginInfo.m_strUserPass.c_str(),
									(LPSTR) m_tagLoginInfo.m_strAppName.c_str(),
									m_tagLoginInfo.m_iRevHi,
									m_tagLoginInfo.m_iRevLow);
			if (err_code)
					return false;

			//delay for logon responce
			MYTRACK_MSG tagMyTrackMsg;
			eg_lib::vt_date dateNow(eg_lib::vt_date::GetCurrentDate ());
			while (true)
			{
				if(WaitForSingleObject(m_hStopEvent,0)!=WAIT_TIMEOUT)
					break;
				err_code = mtGetMessage(&tagMyTrackMsg,0);
				ATLTRACE(_T("_Connect: Return Msg code: %d\n"), err_code);
				ATLTRACE(_T("_Connect: Msg code: %d\n"), tagMyTrackMsg.messageCode);
				if (err_code==MT_ERR_NO_MESSAGE)
					Sleep(TIMEOUT_CONNECT);
				else			
				{
					if (err_code!=MT_ERR_NO_ERROR)	return false;

					if (tagMyTrackMsg.messageCode==MSG_LOGON_RESPONSE && 
						tagMyTrackMsg.u.logonResponse.error) return false;

					if (tagMyTrackMsg.messageCode==MSG_LOGON_RESPONSE)
						break;
					else
						Sleep(TIMEOUT_CONNECT);
				}
				long lSec = (eg_lib::vt_date::GetCurrentDate () - dateNow).get_total_seconds();
				if (lSec>=m_tagLoginInfo.m_iLogonTimeout) 
					return false;
			}
			ATLTRACE(_T("Connected...\n"));
			m_bIsConnected=true;
			bRet = true;
		}
		catch(...)
		{
			bRet = false;
		}
		return bRet;
	}

	void _Connect()
	{
		int err_code =0;
		try
		{

			if (!LoadProviderInfo())
				CCommonException::ThrowError (E_FAIL,L"Loading registry settings failed");
			if(!m_tagLoginInfo.m_strUserName.length())
				CCommonException::ThrowError (E_FAIL,L"The Price Provider Account could not be empty");
			if(!m_tagLoginInfo.m_strUserPass.length())
				CCommonException::ThrowError (E_FAIL,L"The Price Provider Password could not be empty");

			if(m_tagLoginInfo.m_iUseProxy)
			{
				err_code =	mtSetUsingTunnel(TRUE);
				if (err_code)
					throw(GetMyTrackError(err_code));
				
				if(1 == m_tagLoginInfo.m_iUseProxy)
				{
					err_code = mtSetProxy((LPSTR)m_tagLoginInfo.m_strProxyName.c_str(), m_tagLoginInfo.m_iProxyPort);
					if (err_code)
						throw(GetMyTrackError(err_code));
				}
			}
			else
			{
				err_code =	mtSetUsingTunnel(FALSE);
				if (err_code)
					throw(GetMyTrackError(err_code));

			}

			err_code = mtConnectToServer();
			if (err_code)
			{
				mtDisconnect();
				throw(GetMyTrackError(err_code));
			}


			err_code = mtRequestLogon(GetNewReqNum(),
									(LPSTR) m_tagLoginInfo.m_strUserName.c_str(),
									(LPSTR) m_tagLoginInfo.m_strUserPass.c_str(),
									(LPSTR) m_tagLoginInfo.m_strAppName.c_str(),
									m_tagLoginInfo.m_iRevHi,
									m_tagLoginInfo.m_iRevLow);
			if (err_code)
			{
				mtRequestLogoff (GetNewReqNum());
				mtDisconnect();
				throw(GetMyTrackError(err_code));
			}
			//delay for logon responce
			MYTRACK_MSG tagMyTrackMsg;
			eg_lib::vt_date dateNow(eg_lib::vt_date::GetCurrentDate ());
			while (true)
			{
				if(WaitForSingleObject(m_hStopEvent,0)!=WAIT_TIMEOUT)
					break;
				err_code = mtGetMessage(&tagMyTrackMsg,0);
				ATLTRACE(_T("_Connect: Return Msg code: %d\n"), err_code);
				ATLTRACE(_T("_Connect: Msg code: %d\n"), tagMyTrackMsg.messageCode);
				if (err_code==MT_ERR_NO_MESSAGE)
					Sleep(TIMEOUT_CONNECT);
				else			
				{
					if (err_code!=MT_ERR_NO_ERROR)
						throw(GetMyTrackError(err_code));
					if (tagMyTrackMsg.messageCode==MSG_LOGON_RESPONSE && 
						tagMyTrackMsg.u.logonResponse.error)
						throw(GetMyTrackError(tagMyTrackMsg.u.logonResponse.error));
					if (tagMyTrackMsg.messageCode==MSG_LOGON_RESPONSE)
						break;
					else
						Sleep(TIMEOUT_CONNECT);
				}
				long lSec = (eg_lib::vt_date::GetCurrentDate () - dateNow).get_total_seconds();
				if (lSec>=m_tagLoginInfo.m_iLogonTimeout) 
				{
					CCommonException::ThrowError (E_FAIL,L"Logon timeout. Check login and password.");
				}
			}
			ATLTRACE(_T("Connected...\n"));
			m_bIsConnected=true;
		}
		catch(_com_error &e)
		{
			mtRequestLogoff (GetNewReqNum());
			mtDisconnect();
			throw e;
		}
		catch (CMyTrackError &err)
		{
			mtRequestLogoff (GetNewReqNum());
			mtDisconnect();
			throw err;
		}
		catch(...)
		{
			mtRequestLogoff (GetNewReqNum());
			mtDisconnect();
			CCommonException::ThrowError (E_FAIL,L"Logon failed.");
		}

	}
//-----------------------------------------------------------------------------------------------//
	void FindAndAddNotifier(CBaseNotifier *pNotifier)
	{
		CNotifierFind finder(pNotifier);
		CClientList::iterator it = std::find_if (m_lstClients.begin (),m_lstClients.end(),finder);
		if (it==m_lstClients.end ())
			m_lstClients.push_back (pNotifier);
	}
//-----------------------------------------------------------------------------------------------//
	void TransformSymbolName(LPCTSTR pszSymbol,LPCTSTR pszExchange, tstring &Symbol,
		tstring &Option,const InstrumentTypeEnum &enType)
	{
		Symbol = pszSymbol;
		Option = "";
		if (enType==enOPT)
		{
			if(strlen(pszSymbol)<3)
			{
				char buffer[100];
				USES_CONVERSION;
				ZeroMemory(buffer, 100);
				_snprintf(buffer, 99,"Wrong option symbol: %s", pszSymbol);
				CCommonException::ThrowError (E_FAIL,A2W(buffer));
			}

			//get stock\index name
			Symbol.erase(Symbol.end() - 2, Symbol.end());
			// get option code	
			Option = pszSymbol;
			Option.erase(Option.begin(), Option.end() - 2);
		}	
		if(pszExchange && *pszExchange)
		{
			Symbol += _T("=");
			Symbol += (enType == enOPT) ? GetMTOptionExchange(*pszExchange) : GetMTStockExchange(*pszExchange);
		}
	}
//-----------------------------------------------------------------------------------------------//
	void UnsubscribeOneQuote(int &iReqRn, CRequestData *pData)
	{
		pData->m_bSendResponce = false;
		pData->m_pNotifier = NULL;
		if (m_bIsConnected)
			mtRequestIntradayUpdate(iReqRn, 
									(LPSTR)pData->m_strSymbol.c_str(), 
									(LPSTR)pData->m_strOption.c_str(),2);
	}
//-----------------------------------------------------------------------------------------------//
	void ClearMsgBuffer();
	void ClearRequestBuffer();

//-----------------------------------------------------------------------------------------------//
	bool LoadProviderInfo();

	void ProcessMyTrackMsg(MYTRACK_MSG *pMsg,CRequestData *pData);

	static DWORD MsgWait(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE, BOOL bMode = FALSE);
	void CheckLostRequests();
	
//-----------------------------------------------------------------------------------------------//
	
	static UINT __stdcall _ResponseThread(LPVOID pArg);
	static UINT __stdcall _RequestThread(LPVOID pArg);
	static UINT __stdcall _MsgThread(LPVOID pArg);
	static UINT __stdcall _CheckLostRequestsThread(LPVOID pArg);
	
	static CMyTrackError		GetMyTrackError(int err);	
	
	HANDLE						m_hResponseThread;
	HANDLE						m_hMsgThread;
	HANDLE						m_hLostRequestThread;
	HANDLE						m_hRequestThread;

	HANDLE						m_hStopEvent;
	HANDLE						m_hOneMsgEvent;
	HANDLE						m_hConnectionMutex;
	HANDLE                      m_hResubscribeAll;

	HANDLE						m_hOneRequest;

	HANDLE						m_hCheckMessages;
	HANDLE						m_hNextRequestEvent;

	HANDLE						m_hEventMutex;
		
	UINT						m_uiResponseThreadID;
	UINT						m_uiMsgThreadID;
	UINT						m_uiLostMsgThreadID	;
	
	CComAutoCriticalSection		m_csConnect;
	CComAutoCriticalSection		m_csRespMap;
	CComAutoCriticalSection		m_csQueue;

	bool						m_bIsConnected;
	bool                        m_bIsInitialConnection;
	bool						m_bIsInitialized;
	bool						m_bThreadStarted;
	long						m_lReqCounter;	
	CMyTrackLoginInfo			m_tagLoginInfo;
	CClientList					m_lstClients;
	CResponceMap				*m_pmapResponse;
	static const CMyTrackError	m_MyTrackErrors[];
	CMsgBuffer					*m_pMsgBuffer;
	

	CComAutoCriticalSection		m_csRequestBuffer;
	CRequestBuffer				m_RequestBuffer;
	
	int							m_iDisconnectionMessage;	

	PriceProviderTypeEnum		m_enPPMode;

	static char  m_IsAllExchanges; 


};

#endif // !defined(AFX_MYTRACKCORE_H__C00C4BAD_C048_4068_B44F_4A2B6A4BF730__INCLUDED_)
