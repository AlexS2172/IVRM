// Price.h : Declaration of the CPrice

#ifndef __PRICE_H__
#define __PRICE_H__

#include "Extended.h"
#include "HFServer.h"
#include "HyperFeedBase.h"
#include <queue>
/////////////////////////////////////////////////////////////////////////////
// CPrice
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



struct CSubscriptionInfo: public boost::noncopyable
{
	CSubscriptionInfo(CHahedKeyStringPtr ptrKey, InstrumentTypeEnum enType = enSTK)
		:m_lClosePrice(0)
		,m_lFailedOwnershipCounter(0)
		,m_ptrString(ptrKey)
		,m_enInstrumentType(enType)
		,m_dwCurrentUpdateTime(0)
	{
		m_pData = CDBARecordPtr(new DBA_RECORD_3);
		ZeroMemory(m_pData.get(), sizeof(DBA_RECORD_3));
	}

	bool AssignData(CDBARecordPtr& pData)
	{
		bool bRet = false;
		if(pData!=NULL)
		{
			long lCount = 0;
			if(m_csSubscription.TryToLock())
			{	
				DWORD dwCurrentTick = GetTickCount();
				if( (dwCurrentTick - m_dwCurrentUpdateTime) >=100)
				{
					m_pData				  = pData; 
					m_dwCurrentUpdateTime = dwCurrentTick;
					m_dtCurrentUpdateTime = vt_date::GetCurrentDate();
					bRet				  = true;
				}
				m_csSubscription.Unlock();
				InterlockedExchange(&m_lFailedOwnershipCounter, 0L);
			}
			else
				lCount = InterlockedIncrement(&m_lFailedOwnershipCounter);

			if(lCount > 5000)
			{
				m_dwCurrentUpdateTime  = GetTickCount();
				EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("Price record for %s is locked too long. Possobly DeadLock "), GetKey().c_str());	
			}
		}
		return bRet;
	}
public:

	const InstrumentTypeEnum GetType()		 const {return m_enInstrumentType;}
	const std::string&       GetKey()		 const {return m_ptrString->GetKeyString();}
	const size_t			 GetHashCode()   const {return m_ptrString->GetHashCode();}
	const long               GetClosePrice() const {return m_lClosePrice; }
	CHahedKeyStringPtr       GetHashedKey() {return m_ptrString;}

	CDBARecordPtr			 GetData() 
	{	
		CAutoLock lock(m_csSubscription);
		InterlockedExchange(&m_lFailedOwnershipCounter, 0L);
		return m_pData;
	}
	bool IsFreshData(vt_date& dtLast) 
	{	
		CAutoLock lock(m_csSubscription);
		InterlockedExchange(&m_lFailedOwnershipCounter, 0L);
		return (m_dtCurrentUpdateTime>=dtLast);
	}

	void					 SetClosePrice(long lclosePrice){m_lClosePrice  = lclosePrice; }

private:

	CLock							m_csSubscription;
	long							m_lClosePrice;
	InstrumentTypeEnum				m_enInstrumentType;
	CDBARecordPtr					m_pData;
	CHahedKeyStringPtr	            m_ptrString;
	volatile long                   m_lFailedOwnershipCounter;
	DWORD							m_dwCurrentUpdateTime;
	vt_date							m_dtCurrentUpdateTime;
};
typedef boost::shared_ptr<CSubscriptionInfo> CSubscriptionInfoPtr;
typedef std::vector<CSubscriptionInfoPtr>			 CUpdatesList;
/*

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
typedef boost::shared_ptr<CClosePriceInfo> CClosePriceInfoPtr;

*/
class CHahedKeyStringPtr_Compare
{
public:
	bool operator()(const CHahedKeyStringPtr& _Left, const CHahedKeyStringPtr& _Right) const
	{	// apply operator< to operands
		return (_Left->GetKeyString() < _Right->GetKeyString());
	}
};




class CPrice : 
	virtual public CExtended,
	public CHyperFeedResponserBase
{
	typedef std::map<CHahedKeyStringPtr, CQuoteRequestPtr, CHahedKeyStringPtr_Compare>  CMuttipleRequests;
public:
	typedef std::deque<CHahedKeyStringPtr> CVectorKeys;


#ifdef __PERFTEST
	virtual void PerfTestMessage()
	{
		try
		{
			CSubscriptionInfoPtr  pSubsctiption;
			GetRandomSubscribed(pSubsctiption);
			if(pSubsctiption != NULL)
			{
				//Subscribed
				CDBARecordPtr etiPtr = pSubsctiption->GetData();
				double dValue = Price2Dbl( etiPtr->priceType, etiPtr->lastSale.price);
				if(dValue >0.01)
				{
					 dValue += (dValue * (rand()&1)?-.01:0.01);

					 etiPtr->lastSale.price = Dbl2Price(etiPtr->priceType, dValue);
					 etiPtr->ask.price = Dbl2Price(etiPtr->priceType, dValue-0.05);
					 etiPtr->bid.price = Dbl2Price(etiPtr->priceType, dValue+0.1);
				}

				if(pSubsctiption->AssignData(etiPtr))
					PostIntoRealtimeMessageProcessor(pSubsctiption);
			}
		}
		catch(...)
		{
			EgLib::CEgLibTraceManager::Trace(LogFaults, __FUNCTION__ , _T("RealTime Message - Unknown exception"));	
		}

	}
#endif
	virtual void RealtimeMessage(CHahedKeyStringPtr pKey, CDBARecordPtr etiPtr);
	virtual void ErrorMessage(long lErrorMessage);
	virtual void NotificationMessage(ErrorNumberEnum enType, const std::string& sMessage);
	virtual bool HasSubscriptions()
	{
		//CAutoLock lock(m_csSubscript);
		return !m_Subscriptions.empty();
	}

	void OnRemoveAll()
	{
		m_bCancelLastQuote  = true;
		m_bUnsubscribeAll   = true;
	}
	CPrice():
		m_DBAStrategy(this)
		,m_lProcessDenied(0L)
		,m_bAtteched(false)
		,m_lRTResponses(0)
#ifdef __PERFMON
		,m_pPerfMon(NULL)
#endif // __PERFMON

	{
	}

protected:
	virtual ~CPrice();

#ifdef __PERFMON
	PerfMon::IPPBatchInfoPtr  m_spGlobalPerfCounter;
#endif // _DEBUG

private:
	volatile        long m_lRTResponses;
	bool			m_bAtteched;
protected:
	virtual long Attach();
	virtual long Detach();

	long GetOneClosePrice(const CRequestBasePtr& pParams, double& dClosePrice);
	long GetPrice(const CRequestBasePtr& Params, bool bFireError = true, bool bInternal = false);
	void Subscribe(const CRequestBasePtr& Params, bool bNotify = true, bool bInternal = false);
	void Unsubscribe(const _variant_t& Params);
	
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
	void ParamFromKey(const DBA_KEY& key, InstrumentTypeEnum enType, _QuoteUpdateParams& Params)
	{
		Params.Init();
		ParamFromKey(key, enType, (QuoteUpdateParams*)Params);
	}

	void ParamFromKey(const DBA_KEY& key, InstrumentTypeEnum enType, QuoteUpdateParams* pParams);

	virtual void PublicLastQuote(const _QuoteUpdateParams* pParams, const CDBARecordPtr& pData) = 0;
	virtual void PublicQuoteUpdate(const _QuoteUpdateParams* pParams, const CSubscriptionInfoPtr& pData) = 0;

	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3* pRec, _QuoteUpdateInfo& Result);
	virtual void CopyExtendedToInfo(InstrumentTypeEnum enType, const DBA_RECORD_3* pRec, QuoteUpdateInfo* result);
	virtual void CopyExtendedToResult(const DBA_RECORD_3* pRec, _QuoteUpdateResult& Result);
	virtual void CopyExtendedToIvResult(const DBA_RECORD_3* pRec, _StockForIvResult& Result);

	virtual void ClearRealtimeData(){};

	virtual void InitializeRealtimeMessageProcessor();
	virtual void UninitializeRealtimeMessageProcessor();
	virtual void PostIntoRealtimeMessageProcessor(CSubscriptionInfoPtr& pSubsctiption);
	virtual bool ResendQuoteRequest(CQuoteRequestPtr spQuoteRequest){return false;}

	void GetUpdatesList(vt_date& dtLast, CUpdatesList& list);

private:
	void _SetDbKey(const _QuoteUpdateParams& Params, DBA_KEY& Key);
	// special options handling
	void _ConvertOptionNameInKey(DBA_KEY& key);
	// special futures options handling
	void _AddFutureOptionExchangeSymbol(DBA_KEY& key, char cExchange);

	 bool GetSubscribed(CHahedKeyStringPtr& pKey, CSubscriptionInfoPtr&pSubsctiption, bool bNeedLock);

#ifdef __PERFTEST
    void  GetRandomSubscribed(CSubscriptionInfoPtr& pData);
#endif // __PERFTEST
	

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

#ifdef __PERFMON
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
#endif // __PERFMON

	//std::string     m_strUserName; 

//protected:
//
//	void StoreClosePrice(CHahedKeyStringPtr pKey, LONG price);
//	long GetClosePrice(CHahedKeyStringPtr pKey);
//
public:
	CSubscriptionInfoPtr	AddSubscription(CHahedKeyStringPtr pKey, InstrumentTypeEnum Type);
	void				    DeleteSubscription(CHahedKeyStringPtr pKey, bool bUnadvice = true);

	HRESULT OnStrategyInitialize();
	void    OnStrategyUninitialize();

	virtual void OnData(CSubscriptionInfoPtr& Data);

	CVectorKeys				 m_NeedToUnsubscribe;
#ifdef __PERFMON
	PerfMon::IPPBatchInfoPtr m_pPerfMon;
#endif // __PERFMON


private:
	EgLib::CLock             m_csMultipleRequests;
	CMuttipleRequests        m_MultipleRequests;

	EgLib::CLock             m_csMultipleSubscriptions;
	CMuttipleRequests        m_MultipleSubscriptions;

	bool                     m_bWasDisconnected;
	bool					 m_bWasConnectedOnce;

	bool                     m_bMultipleRequests;
	bool                     m_bMultipleSubscription;
	volatile long            m_lProcessDenied;

//	bool                     m_bWasDisconnected;
//	bool					 m_bWasConnectedOnce;

	boost::shared_array<DBA_MAXKEY> m_pUnsubscribeKey;
	boost::shared_array<DBA_MAXKEY> m_pLastQuoteKey;
	boost::shared_array<DBA_MAXKEY> m_pKeySubscriptionKey;

	CStrategy<CSubscriptionInfoPtr,  CPrice>  m_DBAStrategy;
	CVectorKeys						   m_SubsctiptionVector;

	//static bool UDgreater ( CKeysPtr elem1, CKeysPtr elem2 );

	CLock					m_cUnsubscribeAll;
public:
	static CHFServer		m_hfServer;

protected:
	typedef hash_set<CSubscriptionInfoPtr,	CPtr_Compare<CSubscriptionInfoPtr> > subscriptionSet;
	//typedef hash_set<CClosePriceInfoPtr,	CPtr_Compare<CClosePriceInfoPtr>   > closePricesSet;

	CFairSingleWriterMultipleReaderGuard			m_csSubscript;
	subscriptionSet									m_Subscriptions;

	//EgLib::CLock		m_csClosePrice;
	//closePricesSet	m_mapClosePrices;

};

#ifdef __PERFTEST
inline void CPrice::GetRandomSubscribed(CSubscriptionInfoPtr& pData)
{
	pData = CSubscriptionInfoPtr();
	if(m_Subscriptions.size())
	{

		srand(GetTickCount());
		double dRand = ((double)rand()) /RAND_MAX;
		CReaderAutoTryLock lock(m_csSubscript);
		if(lock.IsLocked())
		{
			size_t lRandPos = (size_t)(dRand * (m_Subscriptions.size()-1));
			if(lRandPos <= 0 || lRandPos >= m_Subscriptions.size())
				pData = *m_Subscriptions.begin();
			else
			{
				size_t lHalfPos = m_Subscriptions.size()/2;
				if(lRandPos <= lHalfPos)
				{
					subscriptionSet::iterator itr = m_Subscriptions.begin();
					int i= 0;
					while(i!=lRandPos && itr != m_Subscriptions.end())
					{
						++itr;
						++i;
					}
					if(itr == m_Subscriptions.end())
						--itr;
					pData = *itr;
				}
				else
				{
					subscriptionSet::reverse_iterator itr = m_Subscriptions.rbegin();
					int i= 0;
					lRandPos = m_Subscriptions.size() - lRandPos;

					while(i!=lRandPos && itr != m_Subscriptions.rend())
					{
						++itr;
						++i;
					}
					if(itr == m_Subscriptions.rend())
						--itr;
					pData = *itr;
				}
			}
		}
	}
}

#endif
inline void CPrice::GetUpdatesList(vt_date& dtLast, CUpdatesList& listData)
{
	CReaderAutoLock lock(m_csSubscript);
	listData.clear();
	listData.reserve(m_Subscriptions.size());

	for(subscriptionSet::iterator itr = m_Subscriptions.begin(); itr != m_Subscriptions.end(); ++itr)
	{
		if((*itr)->IsFreshData(dtLast))
			listData.insert(listData.end(), *itr);
	}
}

inline bool CPrice::GetSubscribed(CHahedKeyStringPtr& pKey, CSubscriptionInfoPtr& pSubsctiption, bool bNeedLock)
{
	pSubsctiption                          = CSubscriptionInfoPtr();
	CSubscriptionInfoPtr pSubsctiptionFind = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey));
	bool bRet = false;
	if(bNeedLock)
	{
		CReaderAutoLock lock(m_csSubscript);
		subscriptionSet::iterator itr = m_Subscriptions.find(pSubsctiptionFind);
		if(itr!=m_Subscriptions.end())
			pSubsctiption = *itr;
		bRet = true;
	}
	else
	{
		CReaderAutoTryLock lock(m_csSubscript);
		if(lock.IsLocked())
		{
			subscriptionSet::iterator itr = m_Subscriptions.find(pSubsctiptionFind);
			if(itr!=m_Subscriptions.end())
				pSubsctiption = *itr;
			bRet = true;
		}   
	}
	return  bRet;
}


inline CSubscriptionInfoPtr CPrice::AddSubscription(CHahedKeyStringPtr pKey, InstrumentTypeEnum Type)
{
	CSubscriptionInfoPtr pSubscript;
	bool bSubscribe = false;
	{
		{
			CWriterAutoLock lock(m_csSubscript);
			bSubscribe = m_Subscriptions.empty();
		}

		CSubscriptionInfoPtr pSubscription;
		GetSubscribed(pKey, pSubscription, true);
		if(pSubscription == NULL)
		{
			pSubscript = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey, Type));
			if (pSubscript!=NULL)
			{
				CWriterAutoLock lock(m_csSubscript);
				m_Subscriptions.insert(pSubscript);
			}
#ifdef __PERFMON
			RegisterSubsctiption();
#endif // __PERFMON

		}
		else
			pSubscript = pSubscription;
	}
	if(bSubscribe)
		m_hfServer.Subscribe(this);
	return pSubscript;
}

inline void	CPrice::DeleteSubscription(CHahedKeyStringPtr pKey, bool bUnadvice)
{
	bool bNeedToUnregister = false;
	if(pKey == NULL)
	{
		if(bUnadvice)
			m_hfServer.Unsubscribe(this);

		ClearRealtimeData();
		{
			CWriterAutoLock lock(m_csSubscript);
			if(m_Subscriptions.empty())
				return;
#ifdef __PERFMON
				UnregisterSubsctiption(m_Subscriptions.size());
#endif // __PERFMON
			m_Subscriptions.clear(); 
		}
	}
	else
	{
		{
			CSubscriptionInfoPtr pSubs = CSubscriptionInfoPtr(new CSubscriptionInfo(pKey));
			CWriterAutoLock lock(m_csSubscript);
			if(m_Subscriptions.empty())
				return;
			m_Subscriptions.erase(pSubs);
			bNeedToUnregister = m_Subscriptions.empty();
		}
#ifdef __PERFMON
		UnregisterSubsctiption();
#endif // __PERFMON
		if(bNeedToUnregister)
		{
			m_hfServer.Unsubscribe(this);
			ClearRealtimeData();
		}
	}
}

//inline void CPrice::StoreClosePrice(CHahedKeyStringPtr pKey, LONG price)
//{
//	CClosePriceInfoPtr pClosePrice = CClosePriceInfoPtr(new CClosePriceInfo(pKey, price));
//	if(pClosePrice)
//	{
//		EgLib::CAutoLock lock(m_csClosePrice);
//		std::pair<closePricesSet::iterator, bool> result = m_mapClosePrices.insert(pClosePrice);
//		if(!result.second)
//			*result.first = pClosePrice;
//	}
//}
//
//
//inline long CPrice::GetClosePrice(CHahedKeyStringPtr pKey)
//{
//	long lClosePrice = 0;
//	CClosePriceInfoPtr pClosePrice = CClosePriceInfoPtr(new CClosePriceInfo(pKey));
//	EgLib::CAutoLock lock(m_csClosePrice);
//	closePricesSet::iterator itr = m_mapClosePrices.find(pClosePrice);
//	if(itr!=m_mapClosePrices.end())
//		lClosePrice =(*itr)->GetClosePrice();
//	return lClosePrice;
//
//}

#endif //__PRICE_H__
