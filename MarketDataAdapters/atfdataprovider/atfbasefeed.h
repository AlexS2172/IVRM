// ATFBaseFeed.h: interface for the CATFBaseFeed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ATFBASEFEED_H__B3D777A7_B2A1_420A_A886_8E44CC6B5D4C__INCLUDED_)
#define AFX_ATFBASEFEED_H__B3D777A7_B2A1_420A_A886_8E44CC6B5D4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Common.h"
#include <Process.h>
#include <float.h>

extern const double  decTable[21];
#define MAX_OPTROOTS 32

enum ClientType
{
	_enClientStructureProvider,
	_enClientStructureInfo,
	_enClientPriceProvider,
	_enClientPriceInfo
};
enum RequestType
{
	_enRequestNone,
    _enRequestStock,
	_enRequestOption,
	_enRequestLast,
	_enRequestRealtime,
	_enRequestRealtimeNotification,
	_enRequestGroup,
	_enRequestGrpOptions,
	_enRequestGrpStocks,
	_enRequestError

};

class CEventsData
{
public:
	CComVariant m_vtRequest;
	CComVariant m_vtResponse;
	RequestType m_type;
	bool        m_bValue;
	_bstr_t     m_bsKey;

	// error handling
	ErrorNumberEnum  m_Error;
	_bstr_t m_bstrDescription;	
	RequestsTypeEnum  m_RequestType;

	CEventsData()
	{
		m_RequestType = enNoRequest;
		m_type = _enRequestNone;
		m_bValue = false;
		m_Error = enNoError;
		m_bValue = true;
	}
	CEventsData(const CEventsData& ed){Copy(ed);}
	CEventsData& operator=(const CEventsData& ed){Copy(ed); return *this;}

private:
	void Copy(const CEventsData& ed)
	{
		m_vtRequest = ed.m_vtRequest;
		m_vtResponse =ed.m_vtResponse;
		m_type = ed.m_type;
		m_bValue= ed.m_bValue;
		m_Error = ed.m_Error;
		m_RequestType= ed.m_RequestType;
		m_bstrDescription = ed.m_bstrDescription;
		m_bsKey = ed.m_bsKey;
		m_bValue = ed.m_bValue;
	}
};

class CSubscription
{
	typedef map<_bstr_t, CComVariant> REQUESTS;
public:

	CSubscription(){ZeroMemory(&m_key, sizeof(PB)); m_nCount = 0; m_dwTick = GetTickCount();}
	CSubscription(const CSubscription& c){Copy(c);}
	CSubscription& operator=(const CSubscription& c){Copy(c); return *this;}
	void InsertKey(const PB& Key){memcpy(&m_key, &Key, sizeof(PB));}

	void RemoveRequest(const CComVariant& vtRequest)
	{
		if (vtRequest == vtMissing)
			return;

		_QuoteUpdateParams param(vtRequest);
		_bstr_t bsKey = param->Symbol;
		bsKey += L".US";
		bsKey += param->Exchange;

		REQUESTS::iterator iter = m_Request.find(bsKey);
		if (iter != m_Request.end())
			m_Request.erase(iter);
	}

	void AddRequest(const CComVariant& request)
	{
		_QuoteUpdateParams param(request);
		_bstr_t bsKey = param->Symbol;
		bsKey += L".US";
		bsKey += param->Exchange;
		if(m_Request.find(bsKey)==m_Request.end())
			m_Request[bsKey] = request;
	}

	PB			m_key;
	REQUESTS	m_Request;
	long        m_nCount;
	DWORD       m_dwTick;

private:
	void Copy(const CSubscription& c)
	{ 
		m_dwTick = c.m_dwTick;
		m_nCount = c.m_nCount;
		InsertKey(c.m_key);
		for(REQUESTS::const_iterator iter = c.m_Request.begin(); iter!=c.m_Request.end(); iter++)
		{
			m_Request[iter->first] = iter->second;
		}
	}

};

class CGroupRequest
{
public:
	typedef set<_bstr_t> GROUP;

	long  m_lID;
	GROUP m_keys;

	CGroupRequest(){m_lID = 0; m_keys.clear();}
	CGroupRequest(const CGroupRequest& req){Copy(req);}

	CGroupRequest& operator=(const CGroupRequest& req){Copy(req); return *this;}

private:
	void Copy(const CGroupRequest& req)
	{
		m_lID = req.m_lID;
		m_keys.clear();
		for(GROUP::const_iterator iter = req.m_keys.begin(); iter != req.m_keys.end(); iter++)
		{
			m_keys.insert(*iter);
		}
	}
};

class CRequest
{
public:
	CComVariant m_vtRequest;
	RequestType m_enType;
	_bstr_t     m_bsExtendedSymbol;
	long        m_lGroupRequest;
	GroupRequestType m_enGroupRequestType;
	CRequest():m_enType(_enRequestNone), m_lGroupRequest(-1){};
	CRequest(const CRequest& c){Copy(c);};
	CRequest& operator=(const CRequest& c){Copy(c); return *this;};

private:

	void Copy(const CRequest& c)
	{
		m_lGroupRequest = c.m_lGroupRequest;
		m_enType = c.m_enType;
		m_vtRequest = c.m_vtRequest;
		m_bsExtendedSymbol = c.m_bsExtendedSymbol;
		m_enGroupRequestType = c.m_enGroupRequestType;
	}

};

struct COptionRequest
{
	COptionRequest():m_bEnabled(false),m_bATFSpec(false),m_iLotSize(0){};

	_bstr_t					m_bsKey;
	bool                    m_bATFSpec;
	CComVariant				m_vtRequest;
	CComAutoCriticalSection m_csOptionRequest;
	volatile bool           m_bEnabled;
	long                    m_lGroupRequest;
	int                     m_iLotSize;
	volatile GroupRequestType m_enType;
	//       ATExch  UserExch          
	std::map<char, set<char> >  m_mapExchanges;
};

template< class T >
class CATFBaseFeed
{
	typedef vector<CRequest> REQUEST;
	typedef map<_bstr_t, CSubscription> SUBSCRIPTION;
	typedef map<DWORD, _bstr_t> SUBSCRKEY;

	typedef map<long, _bstr_t>  GROUPREQUESTID;
	typedef map<_bstr_t, CGroupRequest> GROUPREQUEST;

	typedef CDoubleBuffer<CEventsData, 100L>  EVENTS;
	typedef map<_bstr_t, char>   OPTROOT;
public:
	CATFBaseFeed()
	{
		m_bEtiInitialized = false;
		m_lGroupRequest = 0;
		m_pHAT = NULL;
		m_bConnected = false;

		m_QSN = 0;

		m_hThreadRealtime = INVALID_HANDLE_VALUE;
		m_hThreadSender   = INVALID_HANDLE_VALUE;
		m_hThreadRequest  = INVALID_HANDLE_VALUE;


		m_hStop = CreateEvent(NULL,TRUE, FALSE,NULL);
		m_hRequest = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hRealtime = CreateEvent(NULL,FALSE,FALSE,NULL);
		m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);

	}
	virtual ~CATFBaseFeed(){
		try
		{
			DisconnectProvider();
			CloseHandle(m_hRequest);
			CloseHandle(m_hRealtime);
			CloseHandle(m_hEvent);
		}catch(...){}
	};

public:
	void ConnectProvider()
	{
		if(m_bConnected) return;
		HMODULE hLib = LoadLibrary(_T("atfi32.dll"));
		if(hLib==NULL)
		{
			_bstr_t bsMessage = L"Unable to initialize to AT Financial Price Provider. A-T Attitude is not installed";
			CCommonException::ThrowError (E_FAIL,bsMessage);
		}

		FreeLibrary(hLib);
		LoadProviderInfo();

		if(!IsAtServerRunning())
		{
			_bstr_t bsMessage = L"Unable to initialize to AT Financial Price Provider. A-T Attitude is not runned";
			CCommonException::ThrowError (E_FAIL,bsMessage);
		}

		long rc = AtcAttach(&m_pHAT, AtProcessMsg, reinterpret_cast<DWORD>(this));
		if ( rc )
		{
			TCHAR buffer[50];
			_bstr_t bsMessage = L"Unable to connect to AT Financial price provider (";
			itoa(rc,buffer,10);
			bsMessage +=buffer;			
			bsMessage +=_T(")");

			CCommonException::ThrowError (E_FAIL,bsMessage);
		}	

		ResetEvent(m_hStop);
		unsigned ID = 0; 
		m_hThreadSender = (HANDLE)_beginthreadex(NULL,0,_EventSender,this,0,&ID);
		m_hThreadRequest =(HANDLE)_beginthreadex(NULL,0,_Request,this,0,&ID);
		m_bConnected = true;
	};
	
	void DisconnectProvider()
	{
		SetEvent(m_hStop);
		if(m_bConnected)
		{
			m_bConnected = false;

			if(m_pHAT)
			{
				AtcDetach(&m_pHAT);
				m_pHAT = NULL;
			}		
		}

		try
		{
			RemoveRequest(vtMissing, _enRequestRealtime);
		}
		catch (_com_error&) { }

		if(INVALID_HANDLE_VALUE != m_hThreadSender)
		{
			WaitWithEvents(1, &m_hThreadSender);
			CloseHandle(m_hThreadSender);
			m_hThreadSender = INVALID_HANDLE_VALUE;
		}
		if(INVALID_HANDLE_VALUE != m_hThreadRequest)
		{
			WaitWithEvents(1, &m_hThreadRequest);
			CloseHandle(m_hThreadRequest);
			m_hThreadRequest = INVALID_HANDLE_VALUE;
		}
	};

	bool IsStopped()
	{
		return WaitForSingleObject(m_hStop,0)!=WAIT_TIMEOUT;
	}

	// bMode - true - trim LAST spaces in string
	// bMode - false - trim till first space in string
	_bstr_t Convert(const UCHAR* buffer, INT len, bool bMode = true)
	{
		int iRealLen = len +1;
		_bstr_t bsRet;

		if(bMode)
		{
			UCHAR* copy = (UCHAR*)_alloca(iRealLen); 
			ZeroMemory(copy, iRealLen);

			memcpy(copy, buffer,len);
			for(int i = len-1; i>=0; i--)
			{
				if(copy[i]==' ') copy[i] = 0;
				if(copy[i])
					break;
			}
			bsRet = (LPCSTR)copy;
		}
		else
		{
			UCHAR* conv = (UCHAR*)_alloca(len+1);
			ZeroMemory(conv, len+1);
			for(int i = 0; i<len; i++)
			{
				if(buffer[i] && buffer[i]!=' ')
				{
					conv[i] = toupper(buffer[i]);
				} else break;
			}
			bsRet = (LPCSTR)conv;
		}
		return bsRet;
	}

	void Stop(){SetEvent(m_hStop);};

	bool CheckConnection()
	{
		QS_INFO info = {0};
		const int	qsn = m_pHAT->rspMessage.qsInfo.qsn;
		return m_pHAT != 0 && !AtcGetQSInfo(m_pHAT, qsn, & info) && info.connection > 0;
	}

	_bstr_t GetATExchange(BSTR Exchange, RequestType type)
	{
		_bstr_t bsRet;
		if (Exchange == 0)
			return bsRet;
		
		USES_CONVERSION;
		LPCSTR szExchange = W2A(Exchange);
		if (*szExchange && *szExchange != ' ')
		{
			switch (type)
			{
			case _enRequestStock:
				bsRet = GetATStockExchange(*szExchange);
				break;
			case _enRequestOption:
				bsRet = GetATOptionExchange(*szExchange);
				break;
			}
		}

		return bsRet;
	}

	_bstr_t CreateFullName(const CComVariant& vtRequest, RequestType type)
	{
		_bstr_t bsRet =L"";
		if(vtRequest == vtMissing) return bsRet;
		USES_CONVERSION;
		switch(type)
		{
		case _enRequestGrpStocks:
		case _enRequestGrpOptions:
			{
				_StockParams param(vtRequest);
				bsRet = param->Stock;
				bsRet += _T(".US");
				if(param->Exchange)
				{
					_bstr_t pszExchange = GetATExchange(param->Exchange,_enRequestOption); 
					if(pszExchange.length() && *((LPCTSTR)pszExchange)!=_T(' '))
					{
						bsRet += pszExchange;
					}
				}
			}break;

		case _enRequestGroup:
			{
				_QuoteUpdateParams param(vtRequest);
				bsRet = param->Symbol;
				bsRet += _T(".US");
				if(param->Exchange)
				{
					_bstr_t pszExchange = GetATExchange(param->Exchange,_enRequestOption); 
					if(pszExchange.length() && *((LPCTSTR)pszExchange)!=_T(' '))
					{
						bsRet += pszExchange;
					}
				}

			}break;
		case _enRequestStock:
			{
				_StockParams param(vtRequest);
				bsRet = param->Stock;
				bsRet += _T(".US");
				if(param->Exchange)
				{
					LPCSTR pszExchange = W2A(param->Exchange);
					if(*pszExchange && *pszExchange!=_T(' '))
					{
						bsRet += GetATExchange(param->Exchange,_enRequestStock);
					}
				}
			}break;
		case _enRequestOption:
			{
				_OptionParams param(vtRequest);
				bsRet = param->UnderlyingSymbol;
				bsRet += _T(".US");

				if(param->Exchange && *W2A(param->Exchange)!=' ')
					bsRet += GetATExchange(param->Exchange,_enRequestOption);
			}break;
		case _enRequestLast:
		case _enRequestRealtime:
		case _enRequestRealtimeNotification:
			{
				_QuoteUpdateParams param(vtRequest);
				if(param->Type == enOPT)
				{
					_bstr_t bsOption;
					if(!CreateOptionName(Convert((UCHAR*)(LPCSTR)_bstr_t(param->Symbol), 5),  bsOption))
						return _T("");
					
					bsRet = bsOption;
					bsRet += _T(".US");
					bsRet += GetATExchange(param->Exchange,_enRequestOption);

				}
				else
				{
					bsRet = param->Symbol;
					bsRet += _T(".US");
					bsRet += GetATExchange(param->Exchange,_enRequestStock);
				}
			}break;
		default:
			_ASSERTE(false);
			break;
		}
		return bsRet;
	}

	bool CreateOptionName(_bstr_t bsSymbol, _bstr_t& bsOptionRet)
	{
		bool bRet = true;
		int  len = bsSymbol.length();
		bsOptionRet = _T("");
		if( len < 3 || len >5) 
		{
			return false;
		}

		CHAR   buffer[6];
		ZeroMemory(buffer, sizeof(CHAR)*6);

		LPCSTR pszBuffer = (LPCSTR)bsSymbol;
		switch(len)
		{
			case 3:
			{
				buffer[0] = toupper(pszBuffer[0]);
				buffer[1] = ' ';
				buffer[2] = ' ';
				buffer[3] = toupper(pszBuffer[1]);
				buffer[4] = toupper(pszBuffer[2]);
			}
			break;
			case 4:
				{
					buffer[0] = toupper(pszBuffer[0]);
					buffer[1] = toupper(pszBuffer[1]);
					buffer[2] = ' ';
					buffer[3] = toupper(pszBuffer[2]);
					buffer[4] = toupper(pszBuffer[3]);					
				}
				break;
			case 5:
				{
					buffer[0] = toupper(pszBuffer[0]);
					buffer[1] = toupper(pszBuffer[1]);
					buffer[2] = toupper(pszBuffer[2]);
					buffer[3] = toupper(pszBuffer[3]);
					buffer[4] = toupper(pszBuffer[4]);					
				}
				break;
			default:
				return false;
			}

		bsOptionRet = buffer;
		return bRet;
	}

	void RemoveRequest(const CComVariant& vtRequest, RequestType type)
	{
		T* pThis = static_cast<T*>(this);
		if (vtRequest != vtMissing)
		{
			_QuoteUpdateParams param(vtRequest);
			_bstr_t bsRet = param->Symbol;
			if (!bsRet.length())
				utils::ThrowErrorNoSetErrorInfo(E_INVALIDARG, L"Invalid symbol name(empty).");
		}
		else
		{
			m_EventsData.Reset();
			CCriticalSectionWrapper cs(m_csRequest);
			m_Request.clear();
		}

		CRequest req;
		_bstr_t bsFullName = CreateFullName(vtRequest, type);
		switch(type)
		{
		case _enRequestRealtimeNotification:
		case _enRequestRealtime:
			{
			
				CCriticalSectionWrapper s(m_csSubsription);
				CCriticalSectionWrapper cs(m_csRequest);
				if(vtRequest != vtMissing)
				{
					_QuoteUpdateParams param(vtRequest);
					if(param->Type == enOPT && (!param->Exchange || !*param->Exchange || *param->Exchange==L' '))
					{
						char szDefaultExchange[2];
						_bstr_t bsOption;
						if(CreateOptionName(_bstr_t(param->Symbol), bsOption))
						{
							szDefaultExchange[0] = GetOptionDefaultExchange(bsOption);
							szDefaultExchange[1] = 0;
							bsFullName += szDefaultExchange;
						}
					}
				}
				if(bsFullName.length())
				{
					if(m_Subscription.find(bsFullName)!=m_Subscription.end())
					{
						Unsubscribe(vtRequest, bsFullName)	;
					}
				}
				else
				{
					SUBSCRIPTION::iterator it = m_Subscription.begin();	
					while(it != m_Subscription.end())
					{
						Unsubscribe(vtRequest, it->first);
						it = m_Subscription.begin();					
					}
				}
			}break;
		case _enRequestGrpOptions:
		case _enRequestGroup:
			{
				{
					CCriticalSectionWrapper cs(m_GrpRequest.m_csOptionRequest);
					if( !bsFullName.length() || m_GrpRequest.m_bsKey == bsFullName)
					{
						m_GrpRequest.m_bEnabled = false;
					}
				}

				CCriticalSectionWrapper s(m_csSubsription);
				CCriticalSectionWrapper cs(m_csRequest);
				GROUPREQUEST::iterator it = m_GroupRequest.find(bsFullName);
				if (m_GroupRequest.end() != it)
				{
					CGroupRequest::GROUP::iterator  itKey = it->second.m_keys.begin();
					while (itKey != it->second.m_keys.end())
					{
						Unsubscribe(vtMissing, *itKey);
						itKey++;
					}

					m_GroupRequest.erase(it);
				}
				else
				{
					GROUPREQUEST::iterator its =m_GroupRequest.begin();
					while(its !=m_GroupRequest.end())
					{
						CGroupRequest::GROUP::iterator  itKey = its->second.m_keys.begin();
						while(itKey!=its->second.m_keys.end())
						{
							Unsubscribe(vtMissing, *itKey);
							itKey++;
						}
						m_GroupRequest.erase(its);
						its =m_GroupRequest.begin();
					}
				}

			}break;

		case _enRequestOption:
			{
				_bstr_t bs = CreateFullName(vtRequest,_enRequestOption);
				{
					CCriticalSectionWrapper cs(m_OptRequest.m_csOptionRequest);
					if(!bs.length() || m_OptRequest.m_bsKey == bs)
						m_OptRequest.m_bEnabled = false;
				} //  No BREAK here
			}
		default:
			{
				CCriticalSectionWrapper cs(m_csRequest);
				REQUEST::iterator iter = m_Request.begin();
				while(iter!=m_Request.end())
				{
					if(!bsFullName.length())
					{
						m_Request.clear();
						break;
					}
					else
					if(iter->m_enType == type && (iter->m_bsExtendedSymbol == bsFullName))
					{
						m_Request.erase(iter);
						iter = m_Request.begin();
						continue;
					}
					iter++;
				}
			} break;
		}

	};

	void AddRequest(const CComVariant& vtRequest, RequestType type,  GroupRequestType enOperationType = enGroupRequestLastQuote);
	
	SUBSCRIPTION             m_Subscription;
	SUBSCRKEY                m_SubsKey; 
	EVENTS					 m_EventsData;
private:

	HAT*                      m_pHAT;  
	DWORD					  m_QSN;	

	void Unsubscribe(const CComVariant& vtRequest, _bstr_t bsSymbol)
	{
		ATFPROVTRACE(__FUNCTION__, _T("Start unsibscribe. symbol %s"), (LPCTSTR)bsSymbol);
		SUBSCRIPTION::iterator it = m_Subscription.find(bsSymbol);
		if(it!=m_Subscription.end())
		{
			CSubscription& s = it->second;
			if(vtRequest == vtMissing)
			{
				DWORD dwKey = GetPriceBlockDtdp(&s.m_key);
				AtcAtUnadvise(m_pHAT, m_QSN, dwKey);
				m_SubsKey.erase(dwKey);
				m_Subscription.erase(it);
			}
			else
			{
				s.RemoveRequest(vtRequest);
				if(s.m_Request.empty())
				{
					DWORD dwKey = GetPriceBlockDtdp(&s.m_key);
					AtcAtUnadvise(m_pHAT, m_QSN, dwKey);
					m_SubsKey.erase(dwKey);
					m_Subscription.erase(it);
				}
			}
		}
		ATFPROVTRACE(__FUNCTION__, _T("Finish unsibscribe. symbol %s"), (LPCTSTR)bsSymbol);
	}

	void Subscribe(const CComVariant& vtRequest, bool bUseNotification)
	{
		ATFPROVTRACE(__FUNCTION__, _T("Start"));

		PB	buffer;
		memset(&buffer, 0, sizeof(buffer));

		_bstr_t bsKey;

		for (int i = 0; ;i++)
		{
			if (AskPrice(vtRequest, buffer, true, &bsKey))
			{
				T* pThis = static_cast<T*>(this);
				if(bUseNotification)
				{
					ATFPROVTRACE(__FUNCTION__, _T("key = %s"), (LPCTSTR)bsKey);
					pThis->PublicSubscriptionNotification(vtRequest);
				}
				pThis->PublicRealtime(bsKey, &buffer);
				break;
			}
			else if ( i == 4 )
			{
				PublicError(enSubscribeQuote, vtRequest, L"Unable to subscribe symbol", enNoAccessForData);
				break;
			}
		}
		ATFPROVTRACE(__FUNCTION__, _T("End"));
	}

	static DWORD WaitWithEvents(DWORD nCount, CONST HANDLE *pHandles, DWORD dwMilliseconds = INFINITE)
	{
		DWORD dwStatus = -1;
		ATLASSERT(nCount > 0);
		DWORD dwStart = GetTickCount();

		for(;;)
		{
			dwStatus = ::MsgWaitForMultipleObjects(nCount, pHandles, FALSE, dwMilliseconds, QS_ALLINPUT);
			
			if(dwStatus != WAIT_OBJECT_0 + nCount) break;
			
			MSG msg;
			dwStatus = WAIT_TIMEOUT;
			while(dwStatus == WAIT_TIMEOUT && PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				dwStatus = ::WaitForMultipleObjects(nCount, pHandles, FALSE, 0);
			}

			if (dwStatus != WAIT_TIMEOUT)
				break;

			if (dwMilliseconds != INFINITE)
			{
				dwMilliseconds -= (GetTickCount() - dwStart);
				dwStart = GetTickCount();
				if (dwMilliseconds < 0)
					dwMilliseconds = 0;
			}

		}
		return dwStatus;		
	}
	static UINT WINAPI  _EventSender(LPVOID pData)
	{
		CATFBaseFeed<T>* pT = reinterpret_cast< CATFBaseFeed<T>* >(pData);
		T* pThis = static_cast<T*>(pT);
		if(pT)
		{
			HANDLE hEvents[] =
			{
				pT->m_hEvent,
				pT->m_hStop
			};

			ATFPROVTRACE(__FUNCTION__, _T("Begin event thread"));
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			while(1)
			{
				if(WaitWithEvents(2,hEvents)!=WAIT_OBJECT_0)
					break;

				long lSize;
				pT->m_EventsData.LockOut();
				pT->m_EventsData.LockIn();
				pT->m_EventsData.Swap();
				pT->m_EventsData.UnlockIn();
				lSize = pT->m_EventsData.OutSize();
				pT->m_EventsData.UnlockOut();
				ATFPROVTRACE4(__FUNCTION__, _T("Events out buffer size = %d"), lSize);

				try 
				{
					for(long nIdx = 0L; nIdx < lSize; nIdx++)
					{
						if(WaitForSingleObject(pT->m_hStop, 0) == WAIT_OBJECT_0)
							throw -1L; // stop thread

						pT->m_EventsData.LockOut();
						lSize = pT->m_EventsData.OutSize();
						if(nIdx >= lSize)
							break;
						const CEventsData& data = pT->m_EventsData.GetOut(nIdx);
						pT->m_EventsData.UnlockOut();

						ATFPROVTRACE(__FUNCTION__, _T("Begin send event = %d"), data.m_type);
						switch(data.m_type)
						{
						case _enRequestStock:
							{
								pThis->OnStock(data.m_vtRequest, data.m_vtResponse);
							} break;
						case _enRequestGrpStocks:
							{
								pThis->OnStock(data.m_vtRequest, data.m_vtResponse);

							}break;
						case _enRequestGrpOptions:
							{
								pThis->OnOption(data.m_vtRequest, data.m_vtResponse, CComVariant(data.m_bValue));
							}break;
						case _enRequestOption:
							{
								pThis->OnOption(data.m_vtRequest, data.m_vtResponse, CComVariant(data.m_bValue));

							} break;
						case _enRequestLast:
							{
								if (nIdx && !(nIdx % 20))
									ATFPROVTRACE4(__FUNCTION__, _T("OnLastQuote, events index = %d"), nIdx);
								pThis->OnLastQuote(data.m_vtRequest, data.m_vtResponse);

							} break;
						case _enRequestRealtime:
							{
								try
								{
									if(pT->m_Subscription.find(data.m_bsKey)!=pT->m_Subscription.end())
									{								
										if (nIdx && !(nIdx % 20))
											ATFPROVTRACE4(__FUNCTION__, _T("OnQuoteUpdate, events index = %d"), nIdx);
										pThis->OnQuoteUpdate(data.m_vtRequest, data.m_vtResponse);
									}
								}
								catch(...){}
							}break;
						case _enRequestError:
							{
								if (nIdx && !(nIdx % 20))
									ATFPROVTRACE4(__FUNCTION__, _T("OnError, events index = %d,  error %s, request type %d"), nIdx, (LPCTSTR)data.m_bstrDescription, data.m_RequestType);
								pThis->OnError(data.m_Error, data.m_bstrDescription, data.m_RequestType, data.m_vtRequest);
							}break;
						case _enRequestRealtimeNotification:
							{
								if (nIdx && !(nIdx % 20))
									ATFPROVTRACE4(__FUNCTION__, _T("OnSubscribed, events index = %d"), nIdx);
								pThis->OnSubscribed(data.m_vtRequest);
							}break;
						}
						ATFPROVTRACE(__FUNCTION__, _T("End send event = %d"), data.m_type);
					}
					pT->m_EventsData.ClearOut();
				} 
				catch(...)
				{
				}
			}

			CoUninitialize();
			ATFPROVTRACE(__FUNCTION__, _T("End event thread"));
		}
		return 0;
	}
	
static UINT WINAPI  _Request(LPVOID pData);
static DWORD WINAPI AtProcessMsg(void *buffer,DWORD length,DWORD param)
{
	UPDATE* update  = (UPDATE*) buffer;
	if( !param ) return 0;

	CATFBaseFeed<T>* pT = reinterpret_cast< CATFBaseFeed<T>* >(param);


	switch (update->data.type)
	{
	default:

		 break;

	case ATD_QSTIMEDATE:	// QS Time message

		break;

	case ATD_HEARTBEAT:		// ATServer send us this every two seconds
							//  we can use this if we want to do "periodic"
							//  actions but don't want to bother setting
							//  up own timer
		break;

	case ATD_QSALARM:		// See if we care

		if (update->qsAlarm.qsn == pT->m_QSN)		// Yep, that's us
		{
			if (update->qsAlarm.status == QS_DATALOSS)
				ATLASSERT("Warning QS is in DataLoss, values may be inaccurate");
			else
			if (update->qsAlarm.status == QS_DATARESUME)
				ATLASSERT("QS is out of DataLoss, values will be accurate after next update");
			else
				ATLASSERT("Unknown QS Alarm");
		}

		break;

	case ATD_READVISE:
	
			// ATServer is telling us that the QS has
			// changed session and does not know what we're
			// watching. You should re-get price blocks (yep, enumerate them again)
			// and readvise on them if the qsn in the message
			// is the QuoteServer you're using.

		if (GetPBUpdateQSN(update) == pT->m_QSN)
		{
			CCriticalSectionWrapper sc(pT->m_csSubsription);
			for(SUBSCRKEY::iterator iter = pT->m_SubsKey.begin();iter != pT->m_SubsKey.end();iter++)
			{
				ATFPROVTRACE4(__FUNCTION__, _T("Process readvise for key = %s"), (LPCTSTR)iter->second);
				AtcAtAdvise(pT->m_pHAT, pT->m_QSN, iter->first);
			}			
			ATLASSERT("Processed READVISE");
		}

		break;

	case ATD_TERMINATE:		// ATServer is letting us know that someone
							// has closed ATServer or ATExec. You're done!
		try
		{
			ATFPROVTRACE4(__FUNCTION__, _T("Process terminate server"));
			pT->PublicError(enNoRequest, vtMissing, L"ATServer was closed", enProviderInternalError);
			pT->DisconnectProvider();
			pT->m_bConnected = false;
		}
		catch(...)
		{}
		break;

	case ATD_PRICE:			// These messages are sent when something you're
	case ATD_KRBLOCK:		// advised on changes. You can tell what changed
	case ATD_RSFQUOTE:		// by comparing the QSDTID value
	case ATD_GPXQUOTE:
	case ATD_QTUPDATE:
	case ATD_ATPRICE:	// Watch Out!! OLD SAMPLE CODE didn't have this
		{
			DWORD dw = GetPBUpdateDtdp(update);

			CCriticalSectionWrapper cs(pT->m_csSubsription);
			SUBSCRKEY::iterator iter = pT->m_SubsKey.find(dw);
			if(iter!=pT->m_SubsKey.end())
			{
				CSubscription& subs = pT->m_Subscription[iter->second];
				if (CheckPriceBlockUpdate(update, &subs.m_key, pT->m_QSN, CPBU_AUTOUPDATE))
				{
					T* pThis = static_cast<T*>(pT);
					if(pThis)
					{
						PB& pb = subs.m_key;
						const _bstr_t& bsKey = iter->second;
						ATFPROVTRACE(__FUNCTION__, _T("QuoteUpdate %s: bid = %.2f, ask = %.2f, last = %.2f"), 
							(LPCTSTR)bsKey, pb.atstkopt.bid /100., pb.atstkopt.ask / 100., pb.atstkopt.last / 100.);
						pThis->PublicRealtime(bsKey, &pb);
					}
					break;
				}				
			}
			break;
		}
	}

	return 0;
}

	bool GetPrice(const CComVariant vtRequest)
	{
		PB buffer;
		DWORD dwTick = GetTickCount();
		if(AskPrice(vtRequest, buffer, false))
		{
			DWORD len = GetTickCount()-dwTick;
			T* pThis = static_cast<T*>(this);
			   pThis->PublicQuoteData(vtRequest, &buffer);
			   return true;             
		}
		PublicError(enRequestLastQuote, vtRequest, L"Could not get the last quote data", enNoDataAvailableForSymbol);
		return false;
	}

	bool GetStock(const CComVariant vtRequest);
	bool GetGroup(const CComVariant& vtRequest, GroupRequestType enType);
	bool GetOptionsGroup(const CComVariant& vtRequest, GroupRequestType enType);
	bool GetOptions(const CComVariant& vtRequest);
static int WINAPI
	OptionFunc(LPPB pbPtr,LPARAM pData)
	{
		CATFBaseFeed<T>* pT = reinterpret_cast<CATFBaseFeed<T>* >(pData);
		try
		{
			if(pT)
			{
				if(pT->IsStopped())
					return FALSE;
				
				CCriticalSectionWrapper cs(pT->m_OptRequest.m_csOptionRequest);

				_bstr_t bsRequest = pT->CreateFullName(pT->m_OptRequest.m_vtRequest, _enRequestOption);
				if(!pT->m_OptRequest.m_bEnabled || bsRequest!=pT->m_OptRequest.m_bsKey)
					return FALSE;

				if(!pT->m_OptRequest.m_mapExchanges.size() || pT->m_OptRequest.m_mapExchanges.find(pbPtr->atheader.sym.exchange)==pT->m_OptRequest.m_mapExchanges.end())
					return TRUE;


				_bstr_t bs = pT->ExtractOptionSymbol(pbPtr->atstkopt.sym);			
				if(bs.length())
				{
					T* pThis = static_cast<T*>(pT);
					pThis->PublicOptionData(pT->m_OptRequest.m_vtRequest, pbPtr, pT->m_OptRequest.m_iLotSize, false);
				}
				return TRUE;
			}


		}
		catch(...)
		{
			return FALSE;
		}
	return FALSE;
}

static int WINAPI
	GroupSubscribeFunc(LPPB pbPtr,LPARAM pData)
	{
		CATFBaseFeed<T>* pT = reinterpret_cast<CATFBaseFeed<T>* >(pData);
		if(pT)
		{
			if(pT->IsStopped())
				return FALSE;
			
			CCriticalSectionWrapper cs(pT->m_GrpRequest.m_csOptionRequest);
			_bstr_t bsRequest = pT->CreateFullName(pT->m_GrpRequest.m_vtRequest,pT->m_GrpRequest.m_bATFSpec?_enRequestGrpOptions:_enRequestGroup);
			

			if(!pT->m_GrpRequest.m_bEnabled || bsRequest!=pT->m_GrpRequest.m_bsKey)
			{
				ATFPROVTRACE(__FUNCTION__, "Group request canceled");
				return FALSE;
			}

			bool  bAllExchanges = false;
			if(!pT->m_GrpRequest.m_mapExchanges.size())
			{
				return TRUE;
			}

			
			if(!bAllExchanges && pT->m_GrpRequest.m_mapExchanges.find(pbPtr->atheader.sym.exchange)==pT->m_GrpRequest.m_mapExchanges.end())
				return TRUE;


			set<char>& setData = pT->m_GrpRequest.m_mapExchanges.find(pbPtr->atheader.sym.exchange)->second;

			_bstr_t bs = pT->ExtractOptionSymbol(pbPtr->atstkopt.sym);			
			
			char exchBuffer[5];
			exchBuffer[0] = '.';
			exchBuffer[1] = 'U';
			exchBuffer[2] = 'S';
			exchBuffer[3] = pbPtr->atstkopt.sym.exchange;
			exchBuffer[4] = 0;
			bs += exchBuffer;
			
			GROUPREQUESTID::iterator itID = pT->m_GroupRequestID.find(pT->m_GrpRequest.m_lGroupRequest);
			if(itID != pT->m_GroupRequestID.end())
			{
				CSubscription subs;
				bool bMode = true;
				if(pT->m_GrpRequest.m_enType!=enGroupRequestLastQuote)
				{
					ATFPROVTRACE(__FUNCTION__, _T("Subscribe: %s"),(LPCTSTR)bs);
					CCriticalSectionWrapper s(pT->m_csSubsription);
					bMode = pT->m_Subscription.find(bs)==pT->m_Subscription.end();
				}
				if(bMode)
				{
					if(pT->m_GrpRequest.m_enType!=enGroupRequestLastQuote)
					{
						subs.InsertKey(*pbPtr);
					}

					if(!bAllExchanges)
					{
						for(set<char>::iterator itSet = setData.begin(); itSet!=setData.end(); itSet++)
						{
							_QuoteUpdateParams param;
							char szExch[2];
							szExch[0] = *itSet;
							szExch[1] = 0;

							param.Init();
							param->Symbol = pT->ExtractOptionSymbol(pbPtr->atstkopt.sym).copy();
							param->Exchange = _bstr_t(szExch).copy();

							param->Type = enOPT;
							CComVariant vtRequest;
							param.CopyTo(vtRequest);
							if(pT->m_GrpRequest.m_enType!=enGroupRequestLastQuote)
							{
								subs.AddRequest(vtRequest);
								pT->m_SubsKey[GetPriceBlockDtdp(pbPtr)]=bs;
								pT->m_GroupRequest[itID->second].m_keys.insert(bs);

								CCriticalSectionWrapper s(pT->m_csSubsription);
								pT->m_Subscription[bs] = subs;
							}

							T* pThis = static_cast<T*>(pT);
							if (pT->m_GrpRequest.m_enType != enGroupRequestSubscribe && pT->m_GrpRequest.m_enType != enGroupRequestAllNotify)
								pThis->PublicGroupData(pT->m_GrpRequest.m_bATFSpec?pT->m_GrpRequest.m_vtRequest:vtRequest, pbPtr, pT->m_GrpRequest.m_iLotSize, false);
							else if (pT->m_GrpRequest.m_enType == enGroupRequestAllNotify)
							{
								ATFPROVTRACE4(__FUNCTION__, _T("PublicSubscriptionNotification, key = %s"), (LPCTSTR)bs);
								pThis->PublicSubscriptionNotification(vtRequest);
								pThis->PublicRealtime(bs, pbPtr);
							}
						}
					}

					if (pT->m_GrpRequest.m_enType != enGroupRequestLastQuote)
						AtcAtAdvise(pT->m_pHAT, pT->m_QSN, GetPriceBlockDtdp(pbPtr));
				}				
				else
				{
					if(pT->m_GrpRequest.m_enType!=enGroupRequestLastQuote)
					{
						for(set<char>::iterator itSet = setData.begin(); itSet!=setData.end(); itSet++)
						{
							char szExch[2];
							szExch[0] = *itSet;
							szExch[1] = 0;

							_QuoteUpdateParams param;
							param.Init();
							param->Symbol = pT->ExtractOptionSymbol(pbPtr->atstkopt.sym).copy();
							param->Exchange = _bstr_t(szExch).copy();

							param->Type = enOPT;
							CComVariant vtRequest;
							param.CopyTo(vtRequest);

							CCriticalSectionWrapper s(pT->m_csSubsription);
							pT->m_Subscription[bs].AddRequest(vtRequest);
						}
					}
				}			
			}
			return TRUE;
		}
	return FALSE;
}

public:
_bstr_t ExtractOptionSymbol(ATSYMBOL& sym)
{
	_bstr_t bsRet;
	char buffer[16];
	ZeroMemory(buffer,16);

	int j = 0;
	for(int i = 0; i<16; i++)
	{
		if(!sym.symbol[i])
			break;

		if(sym.symbol[i]!=' ')
		{
			buffer[j] += sym.symbol[i];
			j++;
		}
	}
	bsRet = buffer;
	return bsRet;
}

	char GetOptionDefaultExchange(_bstr_t bsOption)
	{
		char cRet = 0;
		DWORD		BtvSize;		// Size of current BTV record
		char  szKey[64];
		ZeroMemory(szKey, 64); 
		strncpy(szKey,(LPCSTR)bsOption, sizeof(szKey));
		StringResize(szKey,3);
		strupr(szKey);
		rtrim(szKey);
		_bstr_t bsRoot = szKey;
		// Check in cache
		OPTROOT::iterator it =  m_OptRoots.find(bsRoot);
		if(it != m_OptRoots.end())
		{
			cRet = it->second;
			return cRet;
		}			

		AT_OPTION	*atOption = (AT_OPTION *) _alloca(8192);
		if(atOption)
		{
			int	  keyPath;
			char  szKey[64];
			DWORD   rc;
			ZeroMemory(szKey, 64); 

			int hBtvFile = QSBtvOpen(m_pHAT, m_QSN, BFN_AT_OPTION, &rc);
			if (hBtvFile < 0)
				return cRet;

			strncpy(szKey,(LPCSTR)bsRoot, sizeof(szKey));
			StringResize(szKey,4);
			strupr(szKey);

			keyPath = 0;
			BtvSize = 8192;

			rc = QSBtvGetEqual(m_pHAT, m_QSN, hBtvFile,szKey,keyPath,&BtvSize, (LPSTR)atOption);
		
			QSBtvClose(m_pHAT, m_QSN, hBtvFile);

			if (rc == 0)
			{
				_bstr_t bsUndSymbol = Convert((UCHAR*)atOption->UnderRootSymbol,sizeof(atOption->UnderRootSymbol));
				bsUndSymbol += L".US";
				
				PB		pb;
				DWORD  	rc, dwFType;
				
				ATFI_OPTION atfiOptionRecords[MAX_OPTROOTS];				
	
				rc = AtGetPriceBlock(m_pHAT, m_QSN, bsUndSymbol, &pb, &dwFType, NULL, 0, 0);

				if (rc < PB_MINSIZE)
					return cRet;
			
				rc = GetATFIOptionInfo(m_pHAT, m_QSN, &pb, atfiOptionRecords, MAX_OPTROOTS,0);

				if (rc == 0)
					return cRet;

				for (DWORD i = 0; i < rc; ++i)
				{
					char	szRootSym[32];
					if(IsStopped())
						return cRet;

					memcpy(szRootSym, atfiOptionRecords[i].optionSymbol,4);
					szRootSym[4] = 0;
					rtrim(szRootSym);
					_strupr(szRootSym);
					_bstr_t bs = szRootSym;
					m_OptRoots[bs] = atfiOptionRecords[i].primaryExchange;
					
					if(bs == bsRoot)
						cRet = atfiOptionRecords[i].primaryExchange;
				}
			}
		}
		return cRet;
	}

	bool AskPrice(const CComVariant& vtRequest, PB& buffer, bool bSubscribe, _bstr_t* pbsKey = 0)
	{

		_QuoteUpdateParams param(vtRequest) ;
		_bstr_t bsKey = CreateFullName(vtRequest, _enRequestLast);

		if(param->Type == enOPT && (!param->Exchange || !*param->Exchange || *param->Exchange==L' '))
		{
			char szDefaultExchange[2];
			_bstr_t bsOption;
			if(CreateOptionName(_bstr_t(param->Symbol), bsOption))
			{
				szDefaultExchange[0] = GetOptionDefaultExchange(bsOption);
				szDefaultExchange[1] = 0;
				bsKey += szDefaultExchange;
			}
		}
		DWORD dwFType;
		DWORD rc = AtGetPriceBlock(m_pHAT, m_QSN, bsKey,
						 &buffer,				// The price block will go here
						 &dwFType,			// The 'type' of fundamental data here
						 NULL,	// The fundamental data goes here
						 0,		// This is what we want
						 0);

		if (LOBYTE(rc) < PB_MINSIZE)
		{
			// Error
			return false;
		}

		switch(param->Type)
		{
		case enSTK:
			{
				if(dwFType != FDT_ATFI_EQUITY_FP)
					return false;
			}break;
		case enOPT:
			{
				if(dwFType != FDT_ATFI_OPTION)
					return false;
			}break;
		case enIDX:
			{
				if(dwFType != FDT_ATFI_INDEX && dwFType != FDT_ATFI_EQUITY_FP)
					return false;
			}break;
		}
		if(bSubscribe)
		{
			CCriticalSectionWrapper s(m_csSubsription);
			DWORD DW = GetPriceBlockDtdp(&buffer);
			if(m_SubsKey.find(DW)== m_SubsKey.end())
			{
				m_SubsKey[DW] = bsKey;
				CSubscription subs;
				subs.AddRequest(vtRequest);
				subs.InsertKey(buffer);
				m_Subscription[bsKey] = subs;
				DWORD d = AtcAtAdvise(m_pHAT, m_QSN, DW);
			}
			else
			{
				m_Subscription[m_SubsKey[DW]].AddRequest(vtRequest);				
			}
		}

		if (pbsKey)
			*pbsKey = bsKey;

		return true;
	}

	void PublicError(RequestsTypeEnum type, const CComVariant& request, _bstr_t description, ErrorNumberEnum Error);
	void PublicResponce(RequestType type, const CComVariant& request, const CComVariant& response, bool bValue);
	void PublicResponce(RequestType type, const CComVariant& request, const CComVariant& response);
	void PublicResponce(RequestType type, const CComVariant& request);
	void PublicResponce(_bstr_t bsKey, const CComVariant& response, bool bSkip)
	{
		try
		{
			USES_CONVERSION;

			CEventsData data;
			data.m_type = _enRequestRealtime;
			data.m_vtResponse = response;
			data.m_bsKey = bsKey;

			CCriticalSectionWrapper s(m_csSubsription);
			CSubscription& subs = m_Subscription[bsKey];
			CSubscription::REQUESTS::iterator iter = subs.m_Request.begin();

			m_EventsData.LockIn();
			while(iter != subs.m_Request.end())
			{
				data.m_vtRequest = iter->second;
				m_EventsData.AddIn(data);
				iter++;
			}
			if (m_EventsData.InSize() && !(m_EventsData.InSize() % 20))
				ATFPROVTRACE4(__FUNCTION__, _T("PublicResponce, key = %s, Events in buffer size = %d"), (LPCTSTR)bsKey, m_EventsData.InSize());

			SetEvent(m_hEvent);
			m_EventsData.UnlockIn();

		}
		catch(...)	
		{
			ATLASSERT(FALSE);
		}
	}

	bool LoadProviderInfo()
	{
		DWORD dwValue;
		if(GetRegValue(HKEY_CURRENT_USER, DEF_PATH, _T("QuoteServerNumber"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
			if(GetRegValue(HKEY_LOCAL_MACHINE, DEF_PATH, _T("QuoteServerNumber"), dwValue, KEY_QUERY_VALUE) != ERROR_SUCCESS)
				return false;

        m_QSN = dwValue;
		return true;
	}

	long GetRegValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, std::string& rValue, REGSAM samDesired = KEY_QUERY_VALUE)
	{
		rValue.erase();
		CRegKey keySoftware;
		long lResult = keySoftware.Open(hKeyParent, lpszKeyName, samDesired);
		if(lResult != ERROR_SUCCESS)
			return lResult;

		USES_CONVERSION;
		DWORD dwSize = 0;
		lResult = keySoftware.QueryValue(NULL, lpszValueName, &dwSize);
		int nLen = dwSize / sizeof(TCHAR);
		if(lResult == ERROR_SUCCESS && nLen > 0)
		{
			LPTSTR szBuff = new TCHAR[nLen + 1];
			lResult = keySoftware.QueryValue(szBuff, lpszValueName, &dwSize);
			rValue = T2A(szBuff);
			delete [] szBuff;
		}
		return lResult;
	}

	long GetRegValue(HKEY hKeyParent, LPCTSTR lpszKeyName, LPCTSTR lpszValueName, DWORD& dwValue, REGSAM samDesired = KEY_QUERY_VALUE)
	{
		CEgRegKey keySoftware;
		long lResult = keySoftware.Open(hKeyParent, lpszKeyName, samDesired);
		if(lResult != ERROR_SUCCESS)
			return lResult;
		return keySoftware.QueryValue(dwValue, lpszValueName);
	}


	void PublicRealtime(_bstr_t key, PB* pData){};
	void PublicQuoteData(const CComVariant& request, PB* pData){}; 
	void PublicOptionData(const CComVariant& request, PB* pData, int iLotSize, bool bLast){};
	void PublicGroupData(const CComVariant& request, PB* pData, int iLotSize, bool bLast){};
	void PublicStockData(const CComVariant& request, PB* pbData, ATFI_FUNDAMENTAL *pFD){}; 
	void PublicSubscriptionNotification(const CComVariant& request){};

	HRESULT OnStock(const CComVariant &varParams, const CComVariant &varResults){return S_OK;}
	HRESULT OnOption(const CComVariant &varParams, const CComVariant &varResults, const CComVariant &varIsLastOption){return S_OK;}
	HRESULT OnLastQuote(const CComVariant &varParams, const CComVariant &varResults){ return S_OK;}
	HRESULT OnQuoteUpdate(const CComVariant &varParams, const CComVariant &varResults){ return S_OK;}
	HRESULT OnError(ErrorNumberEnum enumError, _bstr_t bstrDescription,	RequestsTypeEnum enumRequest, const CComVariant &varRequest){return S_OK;}
	HRESULT OnSubscribed(const CComVariant &varParams){return S_OK;}

private:

	HANDLE  m_hThreadRealtime;
	HANDLE	m_hThreadSender;
	HANDLE	m_hThreadRequest;

	CComAutoCriticalSection m_csResponse;

private: 

	REQUEST                  m_Request;
	OPTROOT					 m_OptRoots;

	GROUPREQUESTID           m_GroupRequestID;
	GROUPREQUEST             m_GroupRequest;
	long                     m_lGroupRequest;

	CComAutoCriticalSection  m_csClients;
	CComAutoCriticalSection  m_csRequest;
	CComAutoCriticalSection  m_csSubsription;
	bool                     m_bConnected;

private:

	HANDLE                   m_hStop;
	HANDLE                   m_hRequest;
	HANDLE                   m_hRealtime;
	HANDLE                   m_hEvent;

	bool					 m_bEtiInitialized;

	CComVariant						m_optionRequest;
	CComVariant						m_vtGroupRequest;

	COptionRequest                  m_OptRequest;
	COptionRequest                  m_GrpRequest;
};

static inline bool fEQZero (double dVal)
{
	return fabs(dVal) < DBL_EPSILON;		
}

static inline bool fNEQZero (double dVal)
{
	return fabs(dVal) > DBL_EPSILON;		
}

static inline bool fEQ (double dVal1 , double dVal2 )
{
	return fEQZero (dVal1 - dVal2);
}

static inline bool IsSubRequest(RequestType	enRType, GroupRequestType enGRType)
{
	return _enRequestRealtime == enRType || _enRequestRealtimeNotification == enRType ||
		_enRequestGroup == enRType &&
		( enGroupRequestSubscribe == enGRType || enGroupRequestAll == enGRType || enGroupRequestAllNotify == enGRType);
}

#include "ATFBaseFeed.inl"
#endif // !defined(AFX_ATFBASEFEED_H__B3D777A7_B2A1_420A_A886_8E44CC6B5D4C__INCLUDED_)
