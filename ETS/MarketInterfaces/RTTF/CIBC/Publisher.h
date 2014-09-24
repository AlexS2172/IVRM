  #ifndef _PUBLISHER_H
#define _PUBLISHER_H

#include "trade.h"

typedef set<int> TDBFailsStore;

class CPublisher : public EgLib::CThread
{
public:
	CPublisher();
	virtual ~CPublisher();

    void SetMode(UINT nMode);
    UINT GetMode() { return m_nMode;}
    
    DWORD Start(void* pParam);	
	DWORD Stop(DWORD dwWait = INFINITE);

// ThreadWrapper methods
private:

	UINT Run(void);
    
	UINT  ThreadFuncForIntradayMode(void* pParam);
	UINT  ThreadFuncForEndofdayMode(void* pParam);

    DWORD  PutSnapShotTrades(CClientRecordset &rsSnapShotTrades);
    UINT  SelectSnapShotTrades(CClientRecordset &rs);
    UINT  ClearTradeFeed();  

protected:
    
    UINT m_nMode;
public:
	
	DWORD m_dwDBDeleted;
	DWORD m_dwDBStored;
	DWORD m_dwPublished;
	DWORD m_dwNotPublished;
	DWORD m_dwReceived;
    
    CDBConnection m_ConnectionSource;
    _bstr_t	      m_bsConnectionStringSource;

	CDBConnection m_ConnectionDest;
    _bstr_t	      m_bsConnectionStringDest;

	DWORD StoreInDatabase(CTrade& Trade);
	DWORD Publish(const CTrade& Trade);
	bool  Reconnect();
	//CString GetConnectionString(LPCTSTR key);
	HRESULT FillTradeUpdate(CClientRecordset& rs, CTrade& Trade);
    HRESULT BroadcastError(HRESULT hrErr, _bstr_t& bsDesc);
    void PublishBroadcastMessage() throw(...);
    DWORD ReProcessTrade(CTrade& Trade,long lSeqNum);

    
private:
    
    void Clear();
	DWORD GetTrades(CClientRecordset &rs, CTrade& trade);

    enum TradeStatusesEnum
    {
        tsNotProcessed = 0, 
        tsProcessed,
        tsIgnored
    };
	void  SetTradeStatus(const CTrade& trade, TradeStatusesEnum eStat);
    
    TDBFailsStore           m_DBFails;

	IPubManagerPtr		    m_spPubManager;
	ITradeUpdatePtr		    m_spTradeUpdate;
    IBroadcastMessagePtr    m_spBroadcastMessage;
	//ISettingsPtr		    m_pEtsSettings;
};


#endif