#ifndef _PUBLISHER_H
#define _PUBLISHER_H

#include "FixTag.h"
#include "ThreadWrapper.h"
#include "DataQueue.h"

struct CTrade
{
	string			sAccount;
	string			sExecID;
	char			cExecTransType;
	float			fLastPrice;
	int				iLastShares;
//	string			sOrderID;
	char			cOrderStatus; 
	char			cSide;
	string			sSymbol;
	CFixTimeValue	ftTransactTime;
	CFixDateValue	fdTradeDate;
	string			sSecurityType;
					//cOpenClose;
					//ReportToExch
	string			sText;
    string          sRoot;
	
    // for options
	CFixMonthYearValue	fmyMaturityMonthYear;
	int					iPutOrCall;			//0=Put, 1=Call
	float				fStrikePrice;
	CFixDayOfMonthValue	fmdMaturityDay;
	string				sUnderlying;
	string				sStatus;
    

	void MakeOptionSymbol();
};

typedef set<string> TDBFailsStore;

class CTracer;
class CPublisher : public CThreadWrapper, public CDataQueueBuffered<CTrade>
{

public:
	DWORD m_dwDBDeleted;
	DWORD m_dwDBStored;
	DWORD m_dwPublished;
	DWORD m_dwReceived;

	DWORD __stdcall Start(void* pParam);
	DWORD __stdcall Stop();

// ThreadWrapper methods
private:

	UINT  virtual __stdcall Thread(void* pParam);
	
	double ConvertDateTime(const CFixTimeValue* tValue);
	double ConvertDateTime(const CFixDateValue* tValue);
	double ConvertDateTime(const CFixMonthYearValue* tValue, const CFixDayOfMonthValue* dValue);

    HRESULT FlushTrade(const CTrade& Trade);

public:
    
    CDBConnection   m_Connection;
    static _bstr_t	m_bsConnectionString;
	static _bstr_t	m_bsConnStringLabel;

	PDWORD	GetCounterReceived();	

	DWORD StoreInDatabase(const CTrade& Trade);	
	DWORD StoreInFile(const CTrade& Trade);	
	DWORD Publish(const CTrade& Trade);
	bool  Reconnect();
	LONG  GetConnectionString();
	HRESULT FillTradeUpdate(CClientRecordset& rs, const CTrade& Trade);
    HRESULT BroadcastError(HRESULT hrErr, _bstr_t& bsDesc);
    void PublishBroadcastMessage() throw(_com_error);
    DWORD ReProcessTrade(const CTrade& Trade,long lSeqNum);

	CPublisher();
    virtual ~CPublisher();

private:

	TDBFailsStore		m_DBFails;
	
	fstream				m_fStructFile;
	fstream				m_fTradesFile;
    fstream	    		m_fFailesFile;

	IPubManagerPtr		    m_spPubManager;
	ITradeUpdatePtr		    m_spTradeUpdate;
    IBroadcastMessagePtr    m_spBroadcastMessage;
};

#endif