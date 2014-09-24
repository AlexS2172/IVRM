#ifndef ISESESSION_H
#define ISESESSION_H

#include "ThreadWrapper.h"
#include "ISEConst.h"
#include "ISEMessage.h"
#include "ISEStatus.h"
#include "ISEUser.h"
#include "ISEData.h"

#pragma pack(push)
#pragma pack(1)

struct CSubscriptionItem
{
	DWORD			m_dwRefCount;
	subscr_item_t	m_Data;

	CSubscriptionItem():m_dwRefCount(0)
	{
		memset(&m_Data, 0, sizeof(m_Data));
	}

	bool operator < (const CSubscriptionItem& Origin) const
	{
		return memcmp(&m_Data.infobj_x, &Origin.m_Data.infobj_x, sizeof(m_Data.infobj_x)) < 0;
	}
};

#pragma pack(pop)

struct CSubscribedSymbol
{
	CSymbolParams			m_SymbolParams;
	InstrumentTypeEnum		m_Type;
	bool					m_bGroup;

	bool operator <(const CSubscribedSymbol& Origin) const
	{
		if(m_Type != Origin.m_Type)
			return m_Type < Origin.m_Type;

		if(m_bGroup != Origin.m_bGroup)
			return m_bGroup < Origin.m_bGroup;

		return m_SymbolParams < Origin.m_SymbolParams;
	}
};

typedef map<CSubscribedSymbol, DWORD>	mapSubscribedSymbols;

class CISESessionBase
{
private:

	HANDLE		m_hEnableLogonEvent;

	DWORD		m_dwSessionID;

public:

	CISESessionBase();
	~CISESessionBase();

public:

	void EnableLogon(bool bEnable = true);

	bool LogonSleep(const DWORD dwMillisecs) const;

	bool IsLogonEnabled() const;

	DWORD GetID() const {return m_dwSessionID;}

	void SetID(const DWORD dwSessionID) {m_dwSessionID = dwSessionID;}
};

#define ISE_API_TIMEOUT		60

class CISEApiTimeout
{
	HANDLE m_hTimer;

public:

	CISEApiTimeout(HANDLE hTimer, long lTimer, PTIMERAPCROUTINE pfnCompletionRoutine, void* pArg)
	{
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = ((ULONGLONG)lTimer) * 1000 * 10000; // 100 ns discontinuity
		liDueTime.QuadPart *= -1;

		m_hTimer = hTimer;

		SetWaitableTimer(m_hTimer, &liDueTime, 0, 
			pfnCompletionRoutine, pArg, 0);
	}

	~CISEApiTimeout()
	{
		CancelWaitableTimer(m_hTimer);
	}
};

class CISESession : virtual public CISESessionBase, public CThreadWrapper, virtual protected CISEData
{
	friend class CISEData;
	friend class CCountryMarket;

private:

	set<CSubscriptionItem>	m_Subscriptions;
	CLock					m_SubscriptionsLock;

	mapSubscribedSymbols	m_SubscribedSymbols;
	bool					m_bPrimary;

	uint32					m_uiBaseFacility;

	CLock					m_LogonLock;
	omniapi_session_handle	m_hSession;

	bool					m_bLoggedOn;

public:

	CLock					m_OmniApiLock;
	CISEUser				m_User;

	omniapi_session_handle	GetHandle() const {return m_hSession;}

private:

// Api timeout
	VOID static CALLBACK ApiTimeoutProc(LPVOID pArg, DWORD, DWORD);
	HANDLE				 m_hApiTimeoutEvent;

// broadcasts thread
private:

	UINT  virtual __stdcall Thread(void* pParam);

// OMNIAPI tx & query
public:

	void SendTx(const void* pMsgData,
				const uint32 uiFacility,
				uint64_t& uiTransactionID,
				uint64_t& uiOrderID) throw (CISEException);

	void SendTx(const CISEMessage& Msg, 
				const uint32 uiFacility,
				uint64_t& uiTransactionID,
				uint64_t& uiOrderID) throw (CISEException);

	void SendQuery( const void* pQueryData,
					const uint32 uiFacility,
					void* pAnswerData,
					uint32& uiAnswerLen,
					uint64_t& uiTransactionID,
					uint64_t& uiOrderID) throw (CISEException);

	void SendQuery( const CISEMessage& Query,
					const uint32 uiFacility,
					CISEAnswer& Answer,
					uint64_t& uiTransactionID,
					uint64_t& uiOrderID) throw (CISEException);

// helpers
private:

	void Subscribe(set_event_list_t* pSubsList) throw (CISEException);

	void Unsubscribe(uint32* pSubsHandles) throw (CISEException);

	void QueryUserInfo(const char* szUser, const char* szPassword) throw (CISEException);

	void QueryUserType() throw (CISEException);

	void SubscribeGeneralBroadcasts() throw (CISEException);

	void RestoreSubscriptions() throw (CISEException);

	bool ReadIseEvents();

	void SubscribeAll() throw (CISEException);

	void SetMMParameters(	// for underlying or class
							const unsigned short uiCommodity,
							const unsigned char  uiInstrumentGroup,

							const unsigned char  uiExpirationGroup,
							const unsigned char  uiStrikePriceGroup,

							// CMM parameters
							const unsigned short *puiTickWorseVolume,

							const unsigned short uiStepUpAfterRegenBuffer,

							const unsigned short uiIseMMTradeLimitAbsolute,
 							const unsigned short uiFirmTradeLimitAbsolute,
							const unsigned short uiFarMMTradeLimitAbsolute,
							
							const unsigned char  uiIseMmTradeLimitFraction,
							const unsigned char  uiFirmTradeLimitFraction,
							const unsigned char  uiFarMmTradeLimitFraction,

							// PMM parameters
							const unsigned short *puiDerivedOrderMaxSize,
							const unsigned short *puiMatchAwayMarketMaxSize,

							//array
							vector<set_pmm_parameters_ui102_item_t>& vecParams
						);

protected:

	// Submit quotes
	void SubmitQuotes(CISESubmitQuote& Request, string& sOrderID) throw (CISEException);
	void DeleteQuote(CISEDeleteQuote& Request, string& sOrderID) throw (CISEException);

// broadcasts
private:

	void OnBroadcast(const broadcast_type_t* const pEvent, const uint16 uiLen);

	void OnBroadcastBD(const broadcast_type_t* const pEvent, const uint16 uiLen);
	void OnBroadcastBN(const broadcast_type_t* const pEvent, const uint16 uiLen);
	void OnBroadcastBI(const broadcast_type_t* const pEvent, const uint16 uiLen);
	void OnBroadcastBO(const broadcast_type_t* const pEvent, const uint16 uiLen);

	// Structure
	void OnBroadcastBI8(const general_message_t* pData, const uint16 uiLen);
	void OnBroadcastBI9(const info_heartbeat_t* pData, const uint16 uiLen);
	void OnBroadcastBI41(const instrument_status_info_t* pData, const uint16 uiLen);
	void OnBroadcastBI103(const series_status_change_bi103_t* pData, const uint16 uiLen);
	void OnBroadcastBI132(const underlying_trading_status_bi132_t* pData, const uint16 uiLen);
	void OnBroadcastBI202(const series_update_bi202_t* pData, const uint16 uiLen);
	void OnBroadcastBI204(const underlying_update_bi204_t* pData, const uint16 uiLen);
	void OnBroadcastBI210(const instrument_class_update_bi210_t* pData, const uint16 uiLen);

	// BBO
	void OnBroadcastBO101(const best_bid_offer_bo101_t* pData, const uint16 uiLen);
	void OnBroadcastBO102(const best_bid_offer_extended_bo102_t* pData, const uint16 uiLen);
	void OnBroadcastBO103(const opra_best_bid_offer_bo103_t * pData, const uint16 uiLen);
	void OnBroadcastBO104(const opra_all_best_bid_offer_bo104_t* pData, const uint16 uiLen);
	void OnBroadcastBO105(const underlying_best_bid_offer_bo105_t* pData, const uint16 uiLen);

	// trades & deals
	void OnBroadcastBD5(const directed_trade_t* pData, const uint16 uiLen);
	void OnBroadcastBD101(const trade_info_bd101_t* pData, const uint16 uiLen);
	void OnBroadcastBD102(const opra_trade_info_bd102_t* pData, const uint16 uiLen);
	void OnBroadcastBD103(const underlying_trade_info_bd103_t* pData, const uint16 uiLen);
	void OnBroadcastBD111(const deal_user_bd111_t* pData, const uint16 uiLen);
	
	// Submit quote 
	void OnBroadcastBO108(const quote_rejection_reason_bo108_t* pData, const uint16 uiLen);
	void OnBroadcastBO109(const quote_stepped_up_bo109_t* pData, const uint16 uiLen);
	void OnBroadcastBO110(const quote_tick_worse_bo110_t* pData, const uint16 uiLen);

	// network
	void OnBroadcastBN1(const omni_broadcast* pData, const uint16 uiLen);

// public operations
public:

	CISESession();

	~CISESession();

	// Logon & Logoff
	void Logon( const bool bPrimary,
				const char* szAddr, const char* szPort,
				const char* szUser, const char* szPassword,
				const char* szProgID = NULL,
				const char* szNewPassword = NULL)  throw (CISEException);

	void Logoff() throw (CISEException);

	// Subscription & unsubscription
	bool SubscribeOption(const CSymbolParams& Req, bool bGroup, bool bAddSubscription = true) throw (CISEException);
	bool SubscribeUnderlying(const CSymbolParams& Req, bool bAddSubscription = true) throw (CISEException);

	void UnsubscribeOption(const CSymbolParams& Req, bool bGroup) throw (CISEException);
	void UnsubscribeUnderlying(const CSymbolParams& Req) throw (CISEException);

	// Ready-to-trade status
	void ReadyToTrade() throw (CISEException);

	// setting MM patameters
	void SetMMParameters(	// role
							bool bPmm,
							// for underlying or class
							const char* const	 szUnd,
							const unsigned char  uiInstrumentGroup,

							const unsigned char  uiExpirationGroup,
							const unsigned char  uiStrikePriceGroup,

							// CMM parameters
							const unsigned short* puiTickWorseVolume,

							const unsigned short uiStepUpAfterRegenBuffer,

							const unsigned short uiIseMMTradeLimitAbsolute,
 						    const unsigned short uiFirmTradeLimitAbsolute,
						    const unsigned short uiFarMMTradeLimitAbsolute,
						    
							const unsigned char  uiIseMmTradeLimitFraction,
							const unsigned char  uiFirmTradeLimitFraction,
							const unsigned char  uiFarMmTradeLimitFraction,

							// PMM parameters
							const unsigned short* puiDerivedOrderMaxSize,
							const unsigned short* puiMatchAwayMarketMaxSize
						 ) throw (CISEException);

	// order management
	void EnterOrder( const string& sSeries, 
					 const unsigned int uiQuantity, 
					 const double dPremium,
					 const BidAskEnum BidOrAsk,
					 string& sOrderID);
};

#endif