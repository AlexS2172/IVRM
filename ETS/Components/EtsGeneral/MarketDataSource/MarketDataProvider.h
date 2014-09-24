#pragma once

#include "..\stdafx.h"


typedef TRecord<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateParams;

const unsigned int SOURCEID_BatchPriceInfo	= 0x1;
const DISPID DISPID_OnLastQuote			= 0x1;
const DISPID DISPID_OnQuoteUpdate		= 0x2;
const DISPID DISPID_OnError				= 0x3;
const DISPID DISPID_OnSubscribed		= 0x4;
const DISPID DISPID_OnUnsubscribed		= 0x5;

const unsigned int SOURCEID_RisksInfo	= 0x2;
const DISPID DISPID_OnRisksUpdate		= 0x6;
const DISPID DISPID_OnSubscribedRisks	= 0x7;
/////////////////////////////////////////////////////////////////////////////
//
class CMarketDataProvider;
typedef IDispEventImpl<SOURCEID_RisksInfo, CMarketDataProvider, &DIID__IRisksInfoEvents> DispRisks;
typedef IDispEventImpl<SOURCEID_BatchPriceInfo, CMarketDataProvider, &DIID__IBatchPriceInfoEvents> DispQuotes;

class CMarketDataProvider: 
	public DispQuotes,
	public DispRisks

{
public:
	CMarketDataProvider(void);
	virtual ~CMarketDataProvider(void);

	virtual void Connect();
	virtual void Disconnect();

	void Subscribe(RequestVector& request);
	void RequestLastQuote(const CTicker& ticker);
	void SubscribeQuotesSingle(const CTicker& ticker);

	void CancelLastQuote();
	void UnSubscribeQuote();
	virtual	void GetQuoteUpdate();

	virtual void AcceptQuote(const CTicker& ticker, const CQuote& quote) = 0;
	virtual void OnLastQuote(const CTicker& pTicker, const  CQuote& pQuote) = 0;
	virtual void OnLastQuoteDone() = 0;
	virtual void OnLastQuoteError(	ErrorNumberEnum enErrorNumber,
									BSTR bsDescription,
									const CTicker& pTicker  ) = 0;

	virtual void OnError( ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;
	virtual void OnDisconnect( ErrorNumberEnum enErrorNumber, BSTR bsDescription ) = 0;

	virtual void OnSubscribed(const CTicker& pTicker) = 0;
	virtual void OnUnsubscribed() = 0;
	virtual void OnQuoteUpdate() = 0;
	
BEGIN_SINK_MAP(CMarketDataProvider)
	SINK_ENTRY_INFO(SOURCEID_BatchPriceInfo, DIID__IBatchPriceInfoEvents, DISPID_OnLastQuote, _OnLastQuote, &m_fiOnLastQuoteInfo)
	SINK_ENTRY_INFO(SOURCEID_BatchPriceInfo, DIID__IBatchPriceInfoEvents, DISPID_OnError, _OnError, &m_fiOnErrorInfo)
	SINK_ENTRY_INFO(SOURCEID_BatchPriceInfo, DIID__IBatchPriceInfoEvents, DISPID_OnQuoteUpdate, _OnQuoteUpdate, &m_fiOnQuoteUpdateInfo)
	SINK_ENTRY_INFO(SOURCEID_BatchPriceInfo, DIID__IBatchPriceInfoEvents, DISPID_OnSubscribed, _OnSubscribed, &m_fiOnSubscribedInfo)
	SINK_ENTRY_INFO(SOURCEID_BatchPriceInfo, DIID__IBatchPriceInfoEvents, DISPID_OnUnsubscribed, _OnUnsubscribed, &m_fiOnUnsubscribedInfo)
	SINK_ENTRY_INFO(SOURCEID_RisksInfo, DIID__IRisksInfoEvents, DISPID_OnRisksUpdate, _OnRisksUpdate, &m_fiOnRisksUpdateInfo)
	SINK_ENTRY_INFO(SOURCEID_RisksInfo, DIID__IRisksInfoEvents, DISPID_OnSubscribedRisks, _OnSubscribedRisks, &m_fiOnSubscribedRisksInfo)
END_SINK_MAP()

	static _ATL_FUNC_INFO		m_fiOnErrorInfo;
	static _ATL_FUNC_INFO		m_fiOnLastQuoteInfo;
	static _ATL_FUNC_INFO		m_fiOnSubscribedInfo;
	static _ATL_FUNC_INFO		m_fiOnUnsubscribedInfo;
	static _ATL_FUNC_INFO		m_fiOnQuoteUpdateInfo;
	static _ATL_FUNC_INFO		m_fiOnRisksUpdateInfo;
	static _ATL_FUNC_INFO		m_fiOnSubscribedRisksInfo;

private:
	STDMETHOD(_OnLastQuote)(struct QuoteUpdateParams* pParams,
							struct QuoteUpdateInfo* pResults);

	STDMETHOD(_OnError)(ErrorNumberEnum enErrorNumber,
						BSTR bsDescription,
						RequestsTypeEnum enReqType,
						VARIANT varRequest);

	STDMETHOD(_OnQuoteUpdate)();
	STDMETHOD(_OnSubscribed)(struct QuoteUpdateParams* pParams);
	STDMETHOD(_OnUnsubscribed)();

	STDMETHOD(_AcceptQuote)(const QuoteUpdateParams& Params, QuoteUpdateInfo& Info);

	IPriceProviderPtr		GetPriceProvider()
	{
		CAutoLock lock(m_providerLock);
		
		if (m_spPriceProvider == NULL) return NULL;

		IPriceProvider* pRet = NULL;
		CComGITPtr<IPriceProvider> pProvider(m_dwCooke);	
		pProvider.CopyTo(&pRet);
		pProvider.Detach();

		IPriceProviderPtr spRet;
		spRet.Attach(pRet, true);
		return spRet;
		
	};

	IBatchPriceProviderPtr	GetBatchPriceProvider()
	{
		CAutoLock lock(m_providerLock);

		if (m_spPriceProvider == NULL) return NULL;

		IBatchPriceProviderPtr spRet = NULL;
		CComGITPtr<IBatchPriceProvider> pProvider(m_dwCooke);	
		pProvider.CopyTo(&spRet);
		pProvider.Detach();
		return spRet;
	};

	void	SetPriceProvider(IPriceProviderPtr pPriceProvider)
	{
		CAutoLock lock(m_providerLock);
		m_spPriceProvider = pPriceProvider;
	};

public:
	//IRisksProvider
	STDMETHOD(_OnRisksUpdate)();
	STDMETHOD(_OnSubscribedRisks)(struct ITicker* pParams);
	STDMETHOD(_AcceptRisks)(const ITicker& Params, IRisks& Info);

	IRisksProviderPtr GetRisksProvider()
	{
		CAutoLock lock(m_providerLock);

		if (m_spPriceProvider == NULL) return NULL;

		IRisksProviderPtr spRet = NULL;
		CComGITPtr<IRisksProvider> pProvider(m_dwCooke);	
		pProvider.CopyTo(&spRet);
		pProvider.Detach();
		return spRet;
	};

	virtual void	OnRisksUpdate() = 0;
	virtual void	OnSubscribedRisks(const CTicker& pTicker) = 0;
	void			SubscribeRisksSingle(const CTicker& ticker);
	virtual	void	GetRisksUpdate();
	virtual void	AcceptRisks(const CTicker& ticker, const CRisks& risks) = 0;

private:
	IPriceProviderPtr		m_spPriceProvider;

	bool					m_bInRequestMode;
	bool					m_bInSubscribeMode;

	CLock					m_providerLock;

	DWORD	m_dwCooke;
};
