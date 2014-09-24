#ifndef _PUBLISHER_H
#define _PUBLISHER_H

#include "clearingbase.h"
#include "portfolio.h"

#import "../../../../tlb/msgstruct.tlb" no_namespace named_guids
#import "../../../../tlb/mmw.tlb" no_namespace named_guids
typedef set<string> TDBFailsStore;

#define TRADES_FILENAME					_T("Logs\\FOCTrades.log")
#define TRADES_FILENAME_BEGIN			_T("Logs\\FOCTrades")
#define TRADES_FILENAME_END				_T(".log")
#define FAILES_FILENAME_BEGIN			_T("Logs\\FOCFailures")
#define FAILES_FILENAME_END				_T(".log")

#define STRUCTURE_FILENAME				_T("Logs\\FOCStructure.log")

#define FLUSH_TIME	1000 * 30
#define CACHE_SIZE	1000	// 1000 * sizeof(foc_rttf_trade) = 384,000

const char BUY_FLAG = 'B';
const char SELL_FLAG = 'S';
const char PUT_FLAG = 'P';
const char CALL_FLAG = 'C';
const char NEW_FLAG = ' ';
const char UPD_FLAG = 'N';
const char DEL_FLAG = 'Y';

typedef int			cache_id_t;
typedef map<string, foc_rttf_trade_ex> TDBFailsMap;

//##ModelId=3C1DD431038E
struct trade_cache_t 
{
	//##ModelId=3C1DD43103A4
	foc_rttf_trade_ex	m_Trade;
	//##ModelId=3C1DD431039A
	time_t			m_AccessTime;
	//##ModelId=3C1DD4310399
	bool			m_bFlushed;
};

typedef map<cache_id_t, trade_cache_t>	trades_cache_t;

//##ModelId=3C1DD43202AA
class CPublisher : public CLock
{
public:
	// ----------------- STATISTICS -----------------
	DWORD m_dwNewTotal;
	DWORD m_dwDeleteTotal;
	DWORD m_dwPublished;
	// -----------------------------------------------

	//##ModelId=3C1DD4320356
	CPublisher() : 
		m_pLastTradeTimeCache(NULL),	
		m_CacheId(0),		
		m_dwPublished(0),
		m_dwDeleteTotal(0),
		m_dwNewTotal(0)
	{
		USES_CONVERSION;
		
		SYSTEMTIME st;
		::GetLocalTime(&st);

		TCHAR pTradesFileName[100] = { 0 };
        TCHAR pFailsFileName[100] = { 0 };
		TCHAR pFileDate[100] = { 0 };

        _stprintf(pFileDate, _T("_%2.2i_%2.2i_%4.4i"), st.wDay, st.wMonth, st.wYear);
				
		_tcscpy(pTradesFileName, TRADES_FILENAME_BEGIN);
		_tcscat(pTradesFileName, pFileDate);			
		_tcscat(pTradesFileName, TRADES_FILENAME_END);	

        _tcscpy(pFailsFileName, FAILES_FILENAME_BEGIN);
		_tcscat(pFailsFileName, pFileDate);			
		_tcscat(pFailsFileName, FAILES_FILENAME_END);	

		char Path[MAX_PATH] = {0};		
		::GetModuleFileName(NULL, Path, MAX_PATH);

		for (int Counter = strlen(Path); Counter != 0 && Path[Counter] != '\\'; Counter--);
		
		memset(Path + Counter, 0, strlen(Path) - Counter);

		std::string TradesLogPath(Path);
		std::string FailuresLogPath(Path);
		std::string StructuresLogPath(Path);

		std::string Slash("\\");

		TradesLogPath += Slash + T2A(pTradesFileName);
		FailuresLogPath += Slash + T2A(pFailsFileName);
		StructuresLogPath += Slash + T2A(STRUCTURE_FILENAME);

		m_fTradesFile.open( TradesLogPath.c_str(), ios::app | ios::out );
        m_fFailsFile.open( FailuresLogPath.c_str(), ios::app | ios::out );
		m_fStructFile.open( StructuresLogPath.c_str(), ios::app | ios::out);
	}
	//##ModelId=3C1DD4320355
	~CPublisher()
	{
		Clear();
	}

	void Clear()
	{
		if(m_pLastTradeTimeCache)
		{
			delete m_pLastTradeTimeCache;
			m_pLastTradeTimeCache = NULL;
		}

		m_TradesCache.clear();
	}

	//##ModelId=3C1DD4320354
	DWORD Start();
	//##ModelId=3C1DD432034B
	DWORD Stop();

public:
	HRESULT Publish(const foc_rttf_trade& Trade, bool bIsTradeUpdate = true);
	DWORD ReProcessTrade(const foc_rttf_trade_ex&  Trade);
	//##ModelId=3C1DD4320342
	DWORD GetLastTradeTime(FOC_DATETIME & Time);
	//##ModelId=3C1DD4320340
	DWORD SetLastTradeTime(const FOC_DATETIME & Time);

	//##ModelId=3C1DD432032D
	DWORD InsertTrade(const foc_rttf_trade_ex & Trade, cache_id_t & Id);
	//##ModelId=3C1DD4320324
	DWORD GetTrade(const cache_id_t & Id, foc_rttf_trade_ex & Trade);


private:

	DWORD StoreInDatabase(const foc_rttf_trade_ex& Trade);

	//##ModelId=3C1DD4320323
	DWORD Reconnect();

	//##ModelId=3C1DD43202F2
	FOC_DATETIME * m_pLastTradeTimeCache;
	//##ModelId=3C1DD43202E8
	trades_cache_t m_TradesCache;
	//##ModelId=3C1DD43202DC
	cache_id_t	   m_CacheId;

	//##ModelId=3C1DD4320322
	DWORD LoadLastTradeTime();
	//##ModelId=3C1DD4320319
	DWORD FlushLastTradeTime();

	//##ModelId=3C1DD432030F
	DWORD LoadTradeById(const cache_id_t & Id, foc_rttf_trade_ex & Trade);

	DWORD FlushTrade(foc_rttf_trade_ex& Trade);
	//##ModelId=3C1DD432030E
	DWORD FlushTrades();

	//##ModelId=3C1DD4320306
	DWORD Flush();

	//##ModelId=3C1DD43202FC
	static unsigned __stdcall ThreadFunc(void *pThisClass);

	//##ModelId=3C1DD43202FB
	BOOL IsStopping();

	//##ModelId=3C1DD43202FA
	void NormalizeCacheSize();

private:
	ITradeUpdatePtr		    m_spTradeUpdate;
	IBroadcastMessagePtr	m_pBroadcast;
    TDBFailsStore           m_DBFails;
	IPubManagerPtr		    m_pPubManager;

	fstream			    	m_fStructFile;
	fstream				    m_fTradesFile;
    fstream			    	m_fFailsFile;
	fstream					m_fSameAsScreen;

	//##ModelId=3C1DD43202D2
	HANDLE m_hShutdownEvent;
	//##ModelId=3C1DD43202C8
	HANDLE m_hThread;
	//##ModelId=3C1DD43202C0
	_bstr_t m_bstrConnectionString;
	//##ModelId=3C1DD43202B6
	CDBConnection m_Connection;
	
	HRESULT FillTradeUpdate(CClientRecordset& rs, const foc_rttf_trade_ex& Trade);
	HRESULT FillBroadcast(HRESULT hrErrorCode, _bstr_t& bsDescription);

	DWORD StoreInFile(const std::string& Status, const foc_rttf_trade_ex& Trade);
};

HRESULT GetConnectionString(_bstr_t & bstrConnectionString);

#endif