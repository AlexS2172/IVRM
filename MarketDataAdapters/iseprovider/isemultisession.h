#ifndef ISE_MULTISESSION_H
#define ISE_MULTISESSION_H

#include "ISESession.h"
#include "ISESettingsManager.h"

struct CSubmitQuoteRequest
{
	CISESubmitQuote							m_Data;
	vector<CQuoteParams*>					m_Refs;
};

struct CDeleteQuoteRequest
{
	CISEDeleteQuote							m_Data;
	CQuoteParams*							m_Ref;
};

struct CBookRequests
{
	vector<CSubmitQuoteRequest>				m_SubmitRequests;
	vector<CDeleteQuoteRequest>				m_DeleteRequests;
};



typedef enum
{
	enArtSumbit,
	enArtDelete
}	EnumAsyncRequestType;

class CAsyncISESession : public CISESession
{
	HANDLE m_hAsyncThread;
	HANDLE m_hAsyncStopEvent;

protected:

	HANDLE m_hAsyncEvent;
	HANDLE m_hReadyEvent;

	void*					m_pRequestData;
	EnumAsyncRequestType	m_RequestType;

private:

	static UINT __stdcall AsyncThread( void *pThisClass);

	UINT  virtual AsyncProc();

public:

	CAsyncISESession();

	void Logon( const bool bPrimary,
				const char* szAddr, const char* szPort,
				const char* szUser, const char* szPassword,
				const char* szProgID = NULL,
				const char* szNewPassword = NULL)  throw (CISEException);

	void Logoff() throw (CISEException);
};

class CISESession0 : public CAsyncISESession
{
};

class CISESession1 : public CAsyncISESession
{
};

class CISESession2 : public CAsyncISESession
{
};

class CISEMultiSession : 
	public CISESession0, 
	public CISESession1,
	public CISESession2
{
private:

	int		m_nSessions;
	DWORD	m_dwSessionID;

private:

	void CallAsync(EnumAsyncRequestType Type, void* pParam);
	void WaitAsyncComplete();
	void InitAsync();

	CLock	m_MultiSessionLock;
	bool	m_bMultiSessionLoggedOn;

public:

	CISEMultiSession():m_dwSessionID(0), m_bMultiSessionLoggedOn(false){}

public:

	// Logon & Logoff
	void Logon( const char* szAddr, const char* szPort,
				vector<CAccount>& vecAccounts,
				const char* szProgID = NULL,
				const char* szNewPassword = NULL)  throw (CISEException);

	void Logoff() throw (CISEException);

	// Subscription & unsubscription
	bool SubscribeOption(const CSymbolParams& Req, bool bGroup) throw (CISEException);
	bool SubscribeUnderlying(const CSymbolParams& Req) throw (CISEException);

	void UnsubscribeOption(const CSymbolParams& Req, bool bGroup) throw (CISEException);
	void UnsubscribeUnderlying(const CSymbolParams& Req) throw (CISEException);

	// Ready-to-trade status
	void ReadyToTrade() throw (CISEException);

	// Submit quotes
	bool SendQuotes(vector<CQuoteParams>& vecQuotes);

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

	void EnterOrder( const string& sSeries, 
					 const unsigned int uiQuantity, 
					 const double dPremium,
					 const BidAskEnum BidOrAsk,
					 string& sOrderID);

};

#endif