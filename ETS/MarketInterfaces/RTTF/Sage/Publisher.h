#ifndef _PUBLISHER_H
#define _PUBLISHER_H

#include "FixTag.h"

typedef boost::shared_ptr<CDBConnection> CDBConnectionPtr;
struct CTrade
{
	CTrade()
		:sAccount("")
		,sExecID("")
		,cExecTransType(0)
		,fLastPrice(0.)
		,iLastShares(0)
		,cOrderStatus(0)
		,cSide(0)
		,sSymbol("")
		,sSecurityType("")
		,sText("")
		,sRoot("")
		,iPutOrCall(0)
		,fStrikePrice(0.)
		,sUnderlying("")
		,sStatus("")
		,sBroker("")
		,sStrategy("")
		,fStockRefPrice(0.0)
	{
		ZeroMemory(&ftTransactTime,			sizeof(ftTransactTime));
		ZeroMemory(&fdTradeDate,			sizeof(fdTradeDate));
		ZeroMemory(&fmyMaturityMonthYear,	sizeof(fmyMaturityMonthYear));
		ZeroMemory(&fmdMaturityDay,			sizeof(fmdMaturityDay));
	}
	CTrade(const CTrade* pTrade)
		:sAccount(pTrade->sAccount)
		,sExecID(pTrade->sExecID)
		,cExecTransType(pTrade->cExecTransType)
		,fLastPrice(pTrade->fLastPrice)
		,iLastShares(pTrade->iLastShares)
		,cOrderStatus(pTrade->cOrderStatus)
		,cSide(pTrade->cSide)
		,sSymbol(pTrade->sSymbol)
		,sSecurityType(pTrade->sSecurityType)
		,sText(pTrade->sText)
		,sRoot(pTrade->sRoot)
		,iPutOrCall(pTrade->iPutOrCall)
		,fStrikePrice(pTrade->fStrikePrice)
		,sUnderlying(pTrade->sUnderlying)
		,sStatus(pTrade->sStatus)
		,sBroker(pTrade->sBroker)
		,fStockRefPrice( pTrade->fStockRefPrice)
		,sStrategy(pTrade->sStrategy)

	{
		_ASSERT(pTrade!=NULL);
		memcpy(&ftTransactTime,			&pTrade->ftTransactTime,		sizeof(ftTransactTime));
		memcpy(&fdTradeDate,			&pTrade->fdTradeDate,			sizeof(fdTradeDate));
		memcpy(&fmyMaturityMonthYear,	&pTrade->fmyMaturityMonthYear,	sizeof(fmyMaturityMonthYear));
		memcpy(&fmdMaturityDay,			&pTrade->fmdMaturityDay,		sizeof(fmdMaturityDay));
	}

	string			sAccount;
	string			sExecID;
	string          sBroker;
	string			sStrategy;
	char			cExecTransType;
	float			fLastPrice;
	int				iLastShares;
	char			cOrderStatus; 
	char			cSide;
	string			sSymbol;
	string			sSecurityType;
	string			sText;
    string          sRoot;
	CFixTimeValue	ftTransactTime;
	CFixDateValue	fdTradeDate;
	
    // for options
	CFixMonthYearValue	fmyMaturityMonthYear;
	int					iPutOrCall;			//0=Put, 1=Call
	float				fStrikePrice;
	CFixDayOfMonthValue	fmdMaturityDay;
	string				sUnderlying;
	string				sStatus;
    float				fStockRefPrice;

	void MakeOptionSymbol();
	void MakeFuturesSymbolRoot();
};

typedef boost::shared_ptr<CTrade> CTradePtr;


typedef set<string> TDBFailsStore;

class CTracer;

class CPublisher : public EgLib::CStrategyT<CPublisher, CTradePtr>
{

public:
	DWORD m_dwDBDeleted;
	DWORD m_dwDBStored;
	DWORD m_dwPublished;
	DWORD m_dwReceived;


// ThreadWrapper methods
	DWORD Start();
	DWORD Stop();
	virtual void OnData(CTradePtr& Data);


	HRESULT OnStrategyInitialize();
	void    OnStrategyUninitialize();

private:
	
	double ConvertDateTime(const CFixTimeValue* tValue);
	double ConvertDateTime(const CFixDateValue* tValue);
	double ConvertDateTime(const CFixMonthYearValue* tValue, const CFixDayOfMonthValue* dValue);

    HRESULT FlushTrade(const CTradePtr& Trade);

public:
    
	CDBConnectionPtr m_pConnection;
    _bstr_t	m_bsConnectionString;
	_bstr_t	m_bsConnStringLabel;
	_bstr_t	m_bsUserGroup;

    HRESULT	SetUserGroup(_bstr_t bsUserGroup = L"");
	HRESULT	InitInfromationStream();

	PDWORD	GetCounterReceived();	

	DWORD StoreInDatabase(const CTradePtr& Trade);	
	DWORD StoreInFile(const CTradePtr& Trade);	
	DWORD Publish(const CTradePtr& Trade);
	bool  Reconnect();

	LONG  GetConnectionString();
	HRESULT FillTradeUpdate(CClientRecordset& rs, const CTradePtr& Trade);
    HRESULT BroadcastError(HRESULT hrErr, _bstr_t bsDesc);
    void PublishBroadcastMessage() throw(_com_error);
    DWORD ReProcessTrade(const CTradePtr& Trade,long lSeqNum);

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