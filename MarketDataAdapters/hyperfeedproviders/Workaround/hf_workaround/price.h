// Price.h : Declaration of the CPrice

#ifndef __PRICE_H__
#define __PRICE_H__

#include "Extended.h"
#include "HFServer.h"
#include "HyperFeedBase.h"
#include <queue>
#include <Boost/noncopyable.hpp>
/////////////////////////////////////////////////////////////////////////////
// CPrice
typedef boost::shared_ptr<DBA_RECORD_3> CDBARecordPtr;
typedef boost::shared_ptr<DBA_KEY>		CKeysPtr;

template<class T>
class CPtr_Compare
{
public:
	enum
	{	// parameters for hash table
		bucket_size = 2,	// 0 < bucket_size
		min_buckets = 2048};	// min_buckets = 2 ^^ N, 0 < N

		CPtr_Compare(){}

		size_t operator()(const T& _Keyval) const
		{	// hash _Keyval to size_t value
			return _Keyval->GetHashCode();
		}

		bool operator()(const T& _Keyval1, const T& _Keyval2) const
		{	// test if _Keyval1 ordered before _Keyval2
			return _Keyval1->GetKey() < _Keyval2->GetKey();
		}
};


//struct CHahedKeyString : public boost::noncopyable
//{
//public:
//	enum KeyType {enKeyComposite, enKeyRegional} ;
//	CHahedKeyString(DBA_KEY* pKey)
//	{
//		if(pKey)
//		{
//			char szKey[MAX_TICKER*2+1];
//			szKey[MAX_TICKER*2] = 0;
//			CUtilities::KeyToString(pKey, szKey, MAX_TICKER*2);
//
//			m_sKeyString = szKey;
//			m_lHashValue= CalcHashCode(m_sKeyString); 
//			memcpy(&m_rawKey, pKey, sizeof(m_rawKey));
//		}
//	}
//
//	CHahedKeyString(std::string& sKey): m_sKeyString(sKey)
//	{ m_lHashValue= CalcHashCode(m_sKeyString); }
//
//	void  				GetRawKey(DBA_KEY *key) const 
//	{
//		if(key)
//			memcpy(key,&m_rawKey, sizeof(DBA_KEY));
//	}
//
//	const std::string&	GetKeyString() const {return m_sKeyString;}
//	const size_t		GetHashCode() const {return m_lHashValue;}
//private:
//	size_t			m_lHashValue;
//	std::string		m_sKeyString;
//	KeyType         m_enKeyType;
//	DBA_KEY         m_rawKey;
//
//	size_t CalcHashCode(std::string& _Keyval)
//	{
//		UINT x1=0x14f2affc;
//		UINT x2=0xfac5bd8f;
//		string::const_iterator itr = _Keyval.begin();
//		string::const_iterator itrEnd = _Keyval.end();
//		for(; itr!=itrEnd; ++itr)
//		{
//			x1+=8*x2 + 0xe5fc82a9;
//			x2+=8*x1 + *itr;
//		}
//		return x1+x2;
//	}
//
//private:
//	CHahedKeyString():m_lHashValue(0){}
//};
//
//typedef boost::shared_ptr<CHahedKeyString> CHahedKeyStringPtr;

struct CSubscriptionInfo: public boost::noncopyable
{
	CSubscriptionInfo(CHahedKeyStringPtr ptrKey, InstrumentTypeEnum enType = enSTK)
		:m_lClosePrice(0)
		,m_enReferences(0)
		,m_ptrString(ptrKey)
		,m_bInitialized(false)
		,m_enInstrumentType(enType)
	{
		m_pData = CDBARecordPtr(new DBA_RECORD_3);
		ZeroMemory(m_pData.get(), sizeof(DBA_RECORD_3));
		m_perrorMessage = CErrorResponsePtr();

	}

	void AssignData(DBA_RECORD_3* pData)
	{
		if(pData)
		{

			CAutoLock lock(m_csSubscription);
			memcpy(m_pData.get(), pData, sizeof(DBA_RECORD_3));
			m_bInitialized = true;
		}
	}
	const bool     IsValid() const { return m_bInitialized; }
public:

	const InstrumentTypeEnum GetType()		 const {return m_enInstrumentType;}
	const std::string&       GetKey()		 const {return m_ptrString->GetKeyString();}
	const size_t			 GetHashCode()   const {return m_ptrString->GetHashCode();}
	const long               GetClosePrice() const {return m_lClosePrice; }
	CHahedKeyStringPtr       GetHashedKey() {return m_ptrString;}

	//const LARGE_INTEGER&	 GetPerfData()   const {return m_PerfCounter; }	
	DBA_RECORD_3*			 GetData() {return m_pData.get();}

	void					 SetClosePrice(long lclosePrice){m_lClosePrice  = lclosePrice; }
	CLock&					 GetSubscriptionLock(){return m_csSubscription;}

	long                     AddReff(){return ::InterlockedIncrement(&m_enReferences);}
	long                     Release(){return ::InterlockedDecrement(&m_enReferences);}



	CErrorResponsePtr        GetErrorMessage(){return m_perrorMessage;}
	void                     SetErrorMessage(CErrorResponsePtr& pMessage){m_perrorMessage = pMessage; }

private:

	CLock							m_csSubscription;
	long							m_lClosePrice;
	InstrumentTypeEnum				m_enInstrumentType;
	CDBARecordPtr					m_pData;
	CHahedKeyStringPtr	            m_ptrString;

	bool                            m_bInitialized;

	volatile long                   m_enReferences;
	CErrorResponsePtr               m_perrorMessage;

};

typedef boost::shared_ptr<CSubscriptionInfo> CSubscriptionInfoPtr;


class CClosePriceInfo: public boost::noncopyable
{
	LONG				m_lClosePrice;
	CHahedKeyStringPtr  m_pKey;
public:
	CClosePriceInfo(CHahedKeyStringPtr pKey, LONG lClosePrice = 0)
		:m_pKey(pKey)
		,m_lClosePrice(lClosePrice)
	{
	}

	const LONG  GetClosePrice() const {return m_lClosePrice;}
	const size_t  GetHashCode() const {return m_pKey->GetHashCode();}
	const std::string& GetKey() const {return m_pKey->GetKeyString();}
};

class CHahedKeyStringPtr_Compare
{
public:
	bool operator()(const CHahedKeyStringPtr& _Left, const CHahedKeyStringPtr& _Right) const
	{	// apply operator< to operands
		return (_Left->GetKeyString() < _Right->GetKeyString());
	}
};
typedef boost::shared_ptr<CClosePriceInfo> CClosePriceInfoPtr;



class CPrice : 
	virtual public CExtended,
	public CHyperFeedResponserBase,
	public EgStd::CEgTracingClass
{
	typedef std::map<CHahedKeyStringPtr, CQuoteRequestPtr, CHahedKeyStringPtr_Compare>  CMuttipleRequests;
public:
	typedef std::deque<CHahedKeyStringPtr> CVectorKeys;

	static  void RealtimeUpdate(CHahedKeyStringPtr pKey, DBA_RECORD_3* etiPtr);
	virtual void RealtimeMessage(CHahedKeyStringPtr pKey, DBA_RECORD_3* etiPtr);
	virtual void NotificationMessage(ErrorNumberEnum enType, const std::string& sMessage);
	virtual void ErrorMessage(long lErrorMessage);

	void OnRemoveAll()
	{
		m_bCancelLastQuote  = true;
		m_bUnsubscribeAll   = true;
	}
	CPrice():
	m_DBAStrategy(this)
		,m_lRTResponses(0)
		,m_pPerfMon(NULL)

	{
	}

protected:
	virtual ~CPrice();
	PerfMon::IPPBatchInfoPtr  m_spGlobalPerfCounter;
private:
	volatile        long m_lRTResponses;
protected:
	virtual long Attach();
	virtual long Detach();

	long GetOneClosePrice(const CRequestBasePtr& pParams, double& dClosePrice);
	long GetPrice(const CRequestBasePtr& Params, bool bFireError = true, bool bInternal = false);
	void Subscribe(const CRequestBasePtr& Params, bool bNotify = true, bool bInternal = false);
	void Unsubscribe(const CComVariant& Params);
	
	virtual void UnsubscribeAll(CVectorKeys& keys);
	virtual void UnsubscribeAll();
	virtual void ProcessMessage(CSubscriptionInfoPtr& pExtRec);

	long GetMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);
	long AddInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);
	long DecInterstMultiple(MULTIGE_FILTER* pFilrer, DBA_MAXKEY* pKey);

	virtual long AddInterest(bool bNotify = true);
	virtual long DecInterest(DBA_KEYS* pKeys);

	// special futures handling
	void AddFutureExchangeSymbol(DBA_KEY& key, char cExchange);
	void ParamFromKey(const DBA_KEY& key, _QuoteUpdateParams& Params)
	{
		Params.Init();
		ParamFromKey(key, (QuoteUpdateParams*)Params);
	}

	void ParamFromKey(const DBA_KEY& key, QuoteUpdateParams* pParams);

	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData) = 0;
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pData) = 0;

	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, _QuoteUpdateInfo& Result);
	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3& rec, QuoteUpdateInfo* result);
	virtual void CopyExtendedToResult(const DBA_RECORD_3& rec, _QuoteUpdateResult& Result);
	virtual void CopyExtendedToIvResult(const DBA_RECORD_3& rec, _StockForIvResult& Result);

private:
	void _SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY& Key);
	// special options handling
	void _ConvertOptionNameInKey(DBA_KEY& key);
	// special futures options handling
	void _AddFutureOptionExchangeSymbol(DBA_KEY& key, char cExchange);

	CSubscriptionInfoPtr  GetSubscribed(CHahedKeyStringPtr& pKey);

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
public:
	void RegisterSubsctiption()
	{
		if(m_pPerfMon)
			m_pPerfMon->AddSubscription();
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->AddSubscription();
	}
	void UnregisterSubsctiption(UINT iNumber=1)
	{
		if(m_pPerfMon)
			m_pPerfMon->DeleteSubscription(iNumber);
		if(m_spGlobalPerfCounter)
			m_spGlobalPerfCounter->DeleteSubscription(iNumber);
	}

protected:

	void StoreClosePrice(CHahedKeyStringPtr pKey, LONG price);
	long GetClosePrice(CHahedKeyStringPtr pKey);

public:
	CSubscriptionInfoPtr	AddSubscription(CHahedKeyStringPtr pKey, InstrumentTypeEnum Type);
	void				    DeleteSubscription(CVectorKeys& vecKey);

	HRESULT OnStrategyInitialize();
	void    OnStrategyUninitialize();

	virtual void OnData(CSubscriptionInfoPtr& Data);

	CVectorKeys				 m_NeedToUnsubscribe;
	PerfMon::IPPBatchInfoPtr m_pPerfMon;


private:
	EgLib::CLock						m_csMultipleRequests;
	CMuttipleRequests					m_MultipleRequests;
	std::deque<CSubscriptionInfoPtr>	m_AllreadySubscribedInMultipleRequest;

	EgLib::CLock             m_csMultipleSubscriptions;
	CMuttipleRequests        m_MultipleSubscriptions;

	bool                     m_bWasDisconnected;
	bool					 m_bWasConnectedOnce;

	bool                     m_bMultipleRequests;
	bool                     m_bMultipleSubscription;

//	bool                     m_bWasDisconnected;
//	bool					 m_bWasConnectedOnce;

	boost::shared_array<DBA_MAXKEY> m_pUnsubscribeKey;
	boost::shared_array<DBA_MAXKEY> m_pLastQuoteKey;
	boost::shared_array<DBA_MAXKEY> m_pKeySubscriptionKey;

	CStrategy<CSubscriptionInfoPtr,  CPrice>  m_DBAStrategy;
	CVectorKeys						   m_SubsctiptionVector;

	static bool UDgreater ( CKeysPtr elem1, CKeysPtr elem2 );

	CLock					m_cUnsubscribeAll;
public:
	static CHFServer		m_hfServer;

	std::string            m_strUserName; 

protected:
	typedef hash_set<CSubscriptionInfoPtr,	CPtr_Compare<CSubscriptionInfoPtr> > subscriptionSet;
	typedef hash_set<CClosePriceInfoPtr,	CPtr_Compare<CClosePriceInfoPtr>   > closePricesSet;

	EgLib::CLock		m_csSubscript;
	subscriptionSet		m_Subscriptions;

	EgLib::CLock		m_csClosePrice;
	closePricesSet		m_mapClosePrices;

	static EgLib::CLock     m_csSubscriptionsGlobal;
	static subscriptionSet	m_SubscriptionsGlobal;

	static inline CSubscriptionInfoPtr CPrice::GetGlobalSubscribed(CHahedKeyStringPtr& pKey);


};

__declspec(selectany)
	EgLib::CLock   CPrice::m_csSubscriptionsGlobal;
__declspec(selectany)
	CPrice::subscriptionSet	CPrice::m_SubscriptionsGlobal;


inline CSubscriptionInfoPtr CPrice::GetGlobalSubscribed(CHahedKeyStringPtr& pKey)
{
	CSubscriptionInfoPtr pSubsctiption;
	CSubscriptionInfoPtr pSubsctiptionFind = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey));

	CAutoLock lock(m_csSubscriptionsGlobal);
	subscriptionSet::iterator itr = m_SubscriptionsGlobal.find(pSubsctiptionFind);
	if(itr!=m_SubscriptionsGlobal.end())
	{
		pSubsctiption = *itr;
		pSubsctiption->AddReff();
	}

	return pSubsctiption;
}


inline CSubscriptionInfoPtr CPrice::GetSubscribed(CHahedKeyStringPtr& pKey)
{
	CSubscriptionInfoPtr pSubsctiption;
	CSubscriptionInfoPtr pSubsctiptionFind = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey));

	CAutoLock lock(m_csSubscript);
	subscriptionSet::iterator itr = m_Subscriptions.find(pSubsctiptionFind);
	if(itr!=m_Subscriptions.end())
		pSubsctiption = *itr;

	return pSubsctiption;
}

inline CSubscriptionInfoPtr CPrice::AddSubscription(CHahedKeyStringPtr pKey, InstrumentTypeEnum Type)
{
	CSubscriptionInfoPtr pSubscript;
	bool bSubscribe = false;

	{
		EgLib::CAutoLock lock(m_csSubscriptionsGlobal);
		CSubscriptionInfoPtr pSubscriptGlobal = GetGlobalSubscribed(pKey);
		if(pSubscriptGlobal == NULL)
		{
			pSubscript = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey, Type));
			m_SubscriptionsGlobal.insert(pSubscript);
			pSubscript->AddReff();
		}
		else
			pSubscript = pSubscriptGlobal;
	}


	{
		EgLib::CAutoLock lock(m_csSubscript);
		if(m_Subscriptions.empty())
			bSubscribe = true;

		CSubscriptionInfoPtr pSubscriptLocal =  GetSubscribed(pKey);
		if(pSubscriptLocal == NULL && pSubscript != NULL)
		{
			if (pSubscript != NULL)
			{
				m_Subscriptions.insert(pSubscript);
				RegisterSubsctiption();
			}
		}
	}

	if(bSubscribe)
		m_hfServer.Subscribe(this);
	return pSubscript;
}

inline void	CPrice::DeleteSubscription(CVectorKeys& vecKey)
{
	{
		EgLib::CAutoLock lock(m_csSubscript);
		if(m_Subscriptions.empty())
			return;

		if(vecKey.empty())
		{
			if(!m_Subscriptions.empty())
			{
				UnregisterSubsctiption(m_Subscriptions.size());

				subscriptionSet::iterator itr =  m_Subscriptions.begin();
				while(itr != m_Subscriptions.end())
				{
					CSubscriptionInfoPtr Subscription = *itr;
					if(!Subscription->Release())
					{
						//Delete from global scope
						EgLib::CAutoLock lock(m_csSubscriptionsGlobal);
						vecKey.push_back(Subscription->GetHashedKey());
						m_SubscriptionsGlobal.erase(Subscription);
					}
					++itr;
				}
				m_Subscriptions.clear(); 
			}
		}
		else
		{
			CVectorKeys::iterator itVec = vecKey.begin();
			while(itVec != vecKey.end())
			{
				//Delete from global scope
				CSubscriptionInfoPtr pSubscript =  GetSubscribed(*itVec);
				if(pSubscript!=NULL)
				{
					CSubscriptionInfoPtr pSubscriptLocal =  GetGlobalSubscribed(*itVec);
					pSubscriptLocal->Release();
					if(pSubscriptLocal->Release())
						itVec = vecKey.erase(itVec);
					else
					{
						m_SubscriptionsGlobal.erase(pSubscriptLocal);
						++itVec;
					}
					m_Subscriptions.erase(pSubscript);
				}
			}
			UnregisterSubsctiption();
		}
	}
	if(m_Subscriptions.empty())
		m_hfServer.Unsubscribe(this);
}

inline void CPrice::StoreClosePrice(CHahedKeyStringPtr pKey, LONG price)
{
	CClosePriceInfoPtr pClosePrice = CClosePriceInfoPtr(new CClosePriceInfo(pKey, price));
	if(pClosePrice)
	{
		EgLib::CAutoLock lock(m_csClosePrice);
		std::pair<closePricesSet::iterator, bool> result = m_mapClosePrices.insert(pClosePrice);
		if(!result.second)
			*result.first = pClosePrice;
	}
}


inline long CPrice::GetClosePrice(CHahedKeyStringPtr pKey)
{
	long lClosePrice = 0;
	CClosePriceInfoPtr pClosePrice = CClosePriceInfoPtr(new CClosePriceInfo(pKey));
	EgLib::CAutoLock lock(m_csClosePrice);
	closePricesSet::iterator itr = m_mapClosePrices.find(pClosePrice);
	if(itr!=m_mapClosePrices.end())
		lClosePrice =(*itr)->GetClosePrice();
	return lClosePrice;

}

#endif //__PRICE_H__
