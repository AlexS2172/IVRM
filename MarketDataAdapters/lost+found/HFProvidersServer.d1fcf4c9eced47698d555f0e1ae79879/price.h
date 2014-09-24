// Price.h : Declaration of the CPrice

#ifndef __PRICE_H__
#define __PRICE_H__

#include "Extended.h"
#include "HFServer.h"
#include "HyperFeedBase.h"
#include <queue>

/////////////////////////////////////////////////////////////////////////////
// CPrice


class CPrice : 
	virtual public CExtended,
	public CHyperFeedResponserBase,
	public EgStd::CEgTracingClass
{
	typedef std::map<std::string, CQuoteRequestPtr>  CMuttipleRequests;
public:

	virtual void RealtimeMessage(ETI_TABLE* etiPtr);
	virtual void NotificationMessage(ErrorNumberEnum enType, const std::string& sMessage);

	void OnRemoveAll()
	{
		m_bCancelLastQuote  = true;
		m_bUnsubscribeAll   = true;
	}
protected:
	virtual ~CPrice() ;
protected:
	virtual long Attach();
	virtual long Detach();

	long GetOneClosePrice(const CRequestBasePtr& pParams, double& dClosePrice);
	long GetPrice(const CRequestBasePtr& Params, bool bFireError = true, bool bInternal = false);
	void Subscribe(const CRequestBasePtr& Params, bool bNotify = true, bool bInternal = false);
	void Unsubscribe(const CComVariant& Params);
	
	virtual void UnsubscribeAll();
	virtual void ProcessMessage(const char* symb, DBA_RECORD* etiPtr);

	long GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);
	long AddInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);
	long DecInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);

	virtual long AddInterest(bool bNotify = true);
	virtual long DecInterest();

	// special futures handling
	void AddFutureExchangeSymbol(DBA_KEY& key, char cExchange);
	void ParamFromKey(const DBA_KEY& key, _QuoteUpdateParams& Params);

	virtual void PublicLastQuote(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec) = 0;
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams& Params, const DBA_RECORD_3& rec) = 0;

	virtual void CopyExtendedToInfo(const DBA_RECORD_3& rec, _QuoteUpdateInfo& Result);
	virtual void CopyExtendedToInfo(const DBA_RECORD_3& rec, QuoteUpdateInfo* result);
	virtual void CopyExtendedToResult(const DBA_RECORD_3& rec, _QuoteUpdateResult& Result);
	virtual void CopyExtendedToIvResult(const DBA_RECORD_3& rec, _StockForIvResult& Result);

private:
	void _SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY& Key);
	// special options handling
	void _ConvertOptionNameInKey(DBA_KEY& key);
	// special futures options handling
	void _AddFutureOptionExchangeSymbol(DBA_KEY& key, char cExchange);

	bool _IsSubscribed(const string& strSymbol);

	void _GetStockExchangeFromKey(const DBA_KEY& key, BSTR* Exchange);
	void _GetOptionExchangeFromKey(const DBA_KEY& key, BSTR* Exchange);
	void _GetFutureExchangeFromName(char* szSymbol, BSTR* Exchange);
	void _GetFutureOptionExchangeFromName(char* szSymbol, BSTR* Exchange);

	bool _GetMultipleRecords(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey, bool bFireError);
	bool _SubscribeMultipleRecords(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey, bool bNotify);

	static long CALLBACK _MultipleDecInterest(VOID* pblk,	// pointer to param block
									ULONG			ulFunc,	// function
									VOID*			pKey,	// pointer to key
									INT				nKeyLen,// length of key struct
									DBA_RECORD*		pRec,	// pointer to return record
									INT				nRecLen,// length of return record
									ULONG			ulKey,	// key number
									DWORD			dwStatus,//Status of the DBA callback
									VOID*			pVoid);

	static long CALLBACK _MultipleRequests(VOID* pblk,	// pointer to param block
		ULONG			ulFunc,	// function
		VOID*			pKey,	// pointer to key
		INT				nKeyLen,// length of key struct
		DBA_RECORD*		pRec,	// pointer to return record
		INT				nRecLen,// length of return record
		ULONG			ulKey,	// key number
		DWORD			dwStatus,//Status of the DBA callback
		VOID*			pVoid);//ptr to Stream Filter struct

	static long CALLBACK _MultipleSubscriptions(VOID* pblk,	// pointer to param block
		ULONG			ulFunc,	// function
		VOID*			pKey,	// pointer to key
		INT				nKeyLen,// length of key struct
		DBA_RECORD*		pRec,	// pointer to return record
		INT				nRecLen,// length of return record
		ULONG			ulKey,	// key number
		DWORD			dwStatus,//Status of the DBA callback
		VOID*			pVoid);//ptr to Stream Filter struct


	CHandle             m_hDataReady;

	volatile bool       m_bUnsubscribeAll;
	volatile bool       m_bCancelLastQuote;

	QuoteStatusType     GetQuoteStatus(USHORT Indicator);

private:
	EgLib::CLock             m_csMultipleRequests;
	CMuttipleRequests        m_MultipleRequests;

	EgLib::CLock             m_csMultipleSubscriptions;
	CMuttipleRequests        m_MultipleSubscriptions;

	bool                     m_bMultipleRequests;
	bool                     m_bMultipleSubscription;

	bool                     m_bWasDisconnected;
	bool					 m_bWasConnectedOnce;

	boost::shared_array<DBA_MAXKEY> m_pUnsubscribeKey;
	boost::shared_array<DBA_MAXKEY> m_pLastQuoteKey;
	boost::shared_array<DBA_MAXKEY> m_pKeySubscriptionKey;
protected:
	typedef std::map<string, InstrumentTypeEnum> subscriptionmap;
	typedef std::map<string, long> pricemap;
	
	EgLib::CLock    m_csSubscript;
	subscriptionmap m_mapSubscript;
	
	EgLib::CLock m_csClosePrice;
	pricemap m_mapClosePrices;

	static CHFServer m_hfServer;
};

inline bool CPrice::_IsSubscribed(const string& strSymbol)
{
	m_csSubscript.Lock();
	bool bRes = m_mapSubscript.find(strSymbol) != m_mapSubscript.end();
	m_csSubscript.Unlock();
	return bRes;
}


#endif //__PRICE_H__
