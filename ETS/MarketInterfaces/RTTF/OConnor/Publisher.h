#ifndef _PUBLISHER_H
#define _PUBLISHER_H

#include "ThreadWrapper.h"
#include "DataQueue.h"
#include "OConst.h"

typedef set<string> TDBFailsStore;

class CPublisher //: 
//	public CStrategyT<CPublisher, CTradePtr>
{

public:
    typedef set<_bstr_t>     TAccounts;

	CPublisher();
	virtual ~CPublisher();

	DWORD __stdcall Start(void* pParam);	
	void __stdcall Stop();
    const TAccounts& GetAccounts();

// ThreadWrapper methods
private:

    HRESULT FlushTrade(const CTradePtr& pTrade);

protected:

public:
	
	DWORD m_dwDBDeleted;
	DWORD m_dwDBStored;
	DWORD m_dwPublished;
	DWORD m_dwReceived;
	//static DWORD m_dwSent;
	//static CHandle m_hFinishHandle;
    
    CDBConnection m_Connection;
    _bstr_t	      m_bsConnectionString;

	DWORD StoreInDatabase(CTradePtr& Trade);
	DWORD StoreInFile(const CTradePtr& Trade);	
	DWORD Publish(const CTradePtr& Trade);
	bool  Reconnect();
	LONG  GetConnectionString();
	HRESULT FillTradeUpdate(CClientRecordset& rs, CTradePtr& Trade);
    HRESULT BroadcastError(HRESULT hrErr, _bstr_t& bsDesc);
	HRESULT BroadcastInformation(_bstr_t& bsDesc);
    void PublishBroadcastMessage();
    DWORD ReProcessTrade(CTradePtr& Trade,long lSeqNum);

	HRESULT OnStrategyInitialize(){return CoInitializeEx(NULL, COINIT_MULTITHREADED);}
	void    OnStrategyUninitialize(){ CoUninitialize(); }
	void	OnData(CTradePtr& pTrade);

private:	
    
    TDBFailsStore           m_DBFails;
    //long                    m_lStructureMissed;

	fstream			    	m_fStructFile;
	fstream				    m_fTradesFile;
    fstream			    	m_fFailesFile;

	IPubManagerPtr		    m_spPubManager;
	ITradeUpdatePtr		    m_spTradeUpdate;
    IBroadcastMessagePtr    m_spBroadcastMessage;
	ISettingsPtr		    m_pEtsSettings;
    TAccounts               m_Accounts;
};

inline const CPublisher::TAccounts& CPublisher::GetAccounts()
{ return m_Accounts; }


#endif