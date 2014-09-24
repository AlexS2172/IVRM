#if !defined(AFX_DATAPROCESSOR_H__C4FB1C8B_2A0D_47FA_BB20_518316DDAF4A__INCLUDED_)
#define AFX_DATAPROCESSOR_H__C4FB1C8B_2A0D_47FA_BB20_518316DDAF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DataProcessor.h : header file
//
#include "DBLayout.h"

/////////////////////////////////////////////////////////////////////////////
// CDataProcessor thread

#define WM_STATUS_NOTIFICATION	WM_USER+1000
#define WM_READY				WM_USER+1002
#define WM_DATA_READY			WM_USER+1003
//#define WM_VOLA_CHANGED			WM_USER+4
#define WM_STOCK_CHANGED		WM_USER+1005
//#define WM_UNDERLAYING_CHANGED  WM_USER+6
#define WM_ERROR				WM_USER+1007
#define WM_ONLINE				WM_USER+1008 

#ifdef _VOLA_MANAGER

#define WM_CUSTOM_VOLA_CHANGED			WM_USER+1009 
#define WM_CUSTOM_VOLA_SAVED			WM_USER+1010 
#define WM_CUSTOM_VOLA_RESTORED			WM_USER+1011
#define WM_CUSTOM_VOLA_UPDATE_VIEW		WM_USER+1012
#define WM_CUSTOM_VOLA_PRICE_CHANGED	WM_USER+1013

#define WM_CUSTOM_VOLA_DEFAULTSURFACE_CHANGED	WM_USER+1014

#define WM_RULE_LIST_CHANGED			WM_USER+1015
#define WM_NEXTDAY_SURFACE_LIST_CHANGED	WM_USER+1016 
#define WM_NEXTDAY_SURFACE_OPEN			WM_USER+1017
#define WM_SETTINGS						WM_USER+1018


 
#endif //_VOLA_MANAGER

#include "DataDef.h"
#include "IVConnection.h"


class months_container
{
public:
	months_container() {}
	virtual void addMonth( DATE dt ) = 0;
	virtual std::vector<DATE> getMonth() const = 0;
};


class CDataProcessor;

typedef IDispEventImpl <1, CDataProcessor,  & __uuidof (PP::_IBatchPriceInfoEvents)> _IPriceInfoEventsImpl;
typedef IDispEventImpl <2, CDataProcessor,  & __uuidof (IVM::_IIVControlEvents)> _IIVControlImpl;

#ifdef _VOLA_MANAGER
typedef IDispEventImpl <4, CDataProcessor,  & __uuidof (VME::_IVolatilityManagementEvents)> _IVolatilityManagementEventsImpl;
typedef IDispEventImpl <5, CDataProcessor,  & __uuidof (Stg::_ISettingsEvents)> _IDBSettingsEventsImpl;
typedef IDispEventImpl <6, CDataProcessor,  & __uuidof (Stg::_IIVSettingsEvents)> _IIVSettingsEventsImpl;
typedef IDispEventImpl <7, CDataProcessor,  & MMW::DIID__IOnMessages> _IMMWOnMessagesEventImpl;
#endif //_VOLA_MANAGER

// This class performs all data processing - download and calculations
class CDataProcessor: 
	public CWinThread,
	public _IPriceInfoEventsImpl,
	public _IIVControlImpl
#ifdef _VOLA_MANAGER
	,public _IVolatilityManagementEventsImpl
	,public _IDBSettingsEventsImpl
	,public _IIVSettingsEventsImpl
	,public _IMMWOnMessagesEventImpl
#endif _VOLA_MANAGER
{
	DECLARE_DYNCREATE(CDataProcessor)
	friend class CVolaMgmtInstance;
public:

	CDataProcessor();
	virtual ~CDataProcessor();
	
	bool GetData (LPCTSTR szSymbol, int nID, months_container & mnths, bool bInitial = false);
	void CancelData ();

	// Stops data processing
	void Stop ()
	{
		
		CancelData();
		PostThreadMessage (WM_QUIT, 0, 0);
#ifdef _VOLA_MANAGER
		m_spVolaMgmtExternal = NULL;
		m_spProviderData = NULL;
		m_spProviders = NULL;
#endif _VOLA_MANAGER
	}
	
	// Waits while InitInstance is complete (or error occurred)
	bool WaitReady()
	{
		HANDLE arHandles [] = {m_evtReady, m_hThread};
//		DWORD dwResult = WaitForMultipleObjects (2, arHandles, FALSE, INFINITE );
//		return (dwResult == WAIT_OBJECT_0);
		return (EgLib::WaitWithEvents(2, arHandles) == WAIT_OBJECT_0);

	}

// Sink map
BEGIN_SINK_MAP (CDataProcessor)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IBatchPriceInfoEvents), 1, OnLastQuote, &m_OnLastQuoteInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IBatchPriceInfoEvents), 2, OnQuoteUpdate, &m_OnQuoteUpdateInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IBatchPriceInfoEvents), 3, OnError, &m_OnErrorInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IBatchPriceInfoEvents), 4, OnSubscribed, &m_OnSubscribedInfo)
	SINK_ENTRY_INFO(1, __uuidof (PP::_IBatchPriceInfoEvents), 5, OnUnsubscribed, &m_OnUnsubscribedInfo)


	SINK_ENTRY_INFO(2, __uuidof (IVM::_IIVControlEvents), 1, OnControllingEvent, &m_OnControllingEventInfo)

#ifdef _VOLA_MANAGER
	SINK_ENTRY_INFO(4, __uuidof (VME::_IVolatilityManagementEvents), 1, OnDefaultSurfaceChanged, &m_OnDefaultSurfaceChangedInfo)
	SINK_ENTRY_INFO(5, __uuidof (Stg::_ISettingsEvents), 1, OnDatabaseConnectionChanged, &m_OnDatabaseConnectionChangedInfo)
	SINK_ENTRY_INFO(5, __uuidof (Stg::_ISettingsEvents), 2, OnPriceProviderChanged, &m_OnPriceProviderChangedInfo)
	SINK_ENTRY_INFO(6, __uuidof (Stg::_IIVSettingsEvents), 1, OnIVSettingsChanged, &m_OnIVSettingsChangedInfo)
	SINK_ENTRY_INFO(7, MMW::DIID__IOnMessages, MMW::DISPID_OnTradeUpdate, OnTradeUpdate, &m_OnTradeUpdateInfo)
#endif // _VOLA_MANAGER
END_SINK_MAP()


// Attributes
public:

// Operations
public:

	void FinishProcessing();
	
	void ReloadPricesConnection(bool bReloadProvider);
	void ReloadRanges();
	void ReloadRates();
	
	void CalcOptPrice( COptionData& opt );
	void CalcOptIVola( COptionData& opt );

	// Returns symbol that's processing currently
	CString GetContractSymbol() const 
	{
		m_csSymbol.Lock();
		CString strResult = m_strSymbol;
		m_csSymbol.Unlock();
		
		return strResult;
	}

	CString GetStatusText();
	CString GetErrorText();
	void SetErrorState()
	{
		SetStatusText (_T("Error occured!"));
		m_evtError.SetEvent();
	}
	bool IsErrorState()
	{
		return m_evtError.Lock (0) == TRUE ;
	}
	
	CURVE_MODE  GetCurveMode() { return m_opts.m_enCurveMode;} 
	CHART_MODE  GetChartMode() { return m_opts.m_enChartMode;}
	void GetChartRange(double& dFrom, double& dTo)
	{
		dFrom = m_opts.m_fChartRangeFrom;
		dTo = m_opts.m_fChartRangeTo;
	}
	bool GetFlatOutRange(){return m_opts.m_bFlatOutRange;}
	bool GetFlatNoBid(){return m_opts.m_bFlatNoBid;}

	double GetContractPrice() const
	{
		m_csContractData.Lock();	
		double dPrice = m_contractData.m_dPrice;
		m_csContractData.Unlock();	
		return dPrice;
	}

	long GetContractID() const
	{
		return m_contractData.nID;
	}

	const vola_bands_map & GetVolaBands () const
	{
		return m_volaBandsData;
	}

	const time_skew_map &  GetTimeSkewData () const
	{
		return m_time_skewData;
	}
	
	const options_map & GetOptions () const
	{
		return m_optsData;
	}
	void ReloadSettings()
	{
		theApp.GetOptions(m_opts);
	}

	DATE GetNewYorkTime()
	{
		DOUBLE	dNYDateTime;
		GetNYDateTimeAsDATE(&dNYDateTime);
		return static_cast<DATE>(dNYDateTime);
	}

	bool GetRangeInfo( double& dStartStrike, double& dEndStrike );
	long GetOptionsDataByEpiration( const COleDateTime& dtMonth, COleDateTime& dtExpiration, strikes_map& strikesData,  bool bUseRange = false );

	void SetStockDividends(UINT iSymbolID, IEtsIndexDivAtomPtr& spDividends)
	{
		m_StockDividends[iSymbolID] = spDividends;
	}


	const sUndPrice&  GetUndPrice(long lUndID = -1) { return lUndID == -1 ? m_PriceData[m_contractData.nID] : m_PriceData[lUndID]; }
#ifdef _VOLA_MANAGER
	VME::IVolatilityManagementPtr GetVolaManager() 
	{ 
		if( AfxGetThread()->m_nThreadID != m_nThreadID )
		{
			if( m_spVolaMgmtExternal == NULL && m_spVolaMgmtStream != NULL )
			{
				ASSERT( m_spVolaMgmtStream != NULL );

				IStream *pStream = m_spVolaMgmtStream.Detach();

				VME::IVolatilityManagement *pVM = NULL;
				HRESULT hr = ::CoGetInterfaceAndReleaseStream( pStream, VME::IID_IVolatilityManagement, (LPVOID*)&pVM );
				ASSERT( SUCCEEDED(hr) );

				m_spVolaMgmtExternal = pVM;
			}

			return m_spVolaMgmtExternal; 
		}
		else
			return m_spVolaManagement;
	}

	void ReleaseVolaManager()
	{
		ASSERT( AfxGetThread()->m_nThreadID != m_nThreadID );

		m_spVolaMgmtExternal = NULL;
	}
#endif //_VOLA_MANAGER


	// Locks updating volatility 
	class CLockVola
	{
	public:
		CLockVola();
		~CLockVola();
	};

	friend class CLockVola;
	
	// Locks updating time skew data;
	class CLockTimeSkew
	{
	public:
		CLockTimeSkew();
		~CLockTimeSkew();
	};
	friend class CLockTimeSkew;

	// Locks reload;
	class CLockReload
	{
	public:
		CLockReload();
		~CLockReload();
	};
	friend class CLockReload;

	class CLockVolaBands
	{
	public:
		CLockVolaBands();
		~CLockVolaBands();
	};
	friend class CLockVolaBands;
//********************* UTILS *******************************/
public:
	static double GetStrikeInterpolation(double Strike, double StrikeB, double VolaB, double StrikeA, double VolaA) throw( ... )
	{
		if(StrikeB > StrikeA)
			_com_issue_error(E_INVALIDARG);
		else if(StrikeA == StrikeB && VolaB != VolaA)
			_com_issue_error(E_INVALIDARG);
		else if(StrikeA == StrikeB && VolaB == VolaA)
			return VolaA;
		else // if StrikeB < StrikeA
		{
			if(StrikeA <= Strike)
				return VolaA;
			else if(Strike <= StrikeB)
				return VolaB;
			else
			{
				double A = (VolaA - VolaB)/(StrikeA - StrikeB);
				double B = VolaB - (A * StrikeB);
				return (A * Strike) + B;
			}
		}
		return 0;
	}

	static double GetTimeInterpolation(long ExpPeriod, long ExpPeriodB, double VolaB,long ExpPeriodA,double VolaA) throw( ... )
	{
		if(ExpPeriodB > ExpPeriodA)
			_com_issue_error(E_INVALIDARG);
		else if(ExpPeriodA == ExpPeriodB && VolaB != VolaA)
			_com_issue_error(E_INVALIDARG);
		else if(ExpPeriodA == ExpPeriodB && VolaB == VolaA)
			return VolaA;
		else // if ExpPeriodB < ExpPeriodA
		{
			if(ExpPeriodA <= ExpPeriod)
				return VolaA;
			else if(ExpPeriod <= ExpPeriodB)
				return VolaB;
			else
			{
				double A = (VolaA - VolaB)/(sqrt(ExpPeriodA/365.) - sqrt(ExpPeriodB/365.));
				double B = VolaB - (A * sqrt(ExpPeriodB/365.));
				return (A * sqrt(ExpPeriod/365.)) + B;
			}
		}
		return 0;
	}
//***********************************************************/

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataProcessor)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Important! This method is called in the beginning of any data processing operation
	bool BeginProcessing()
	{
		if (! m_evtStopData.Lock(0) )
		{
			m_evtWaiting.ResetEvent();
			return true;
		}

		return false;
	}

	// Important! This method is called in the end of any data processing operation
	void EndProcessing ()
	{
		m_evtWaiting.SetEvent ();
	}

	bool CheckStop (bool bThrow=true);

	static _ATL_FUNC_INFO m_OnLastQuoteInfo;
	static _ATL_FUNC_INFO m_OnQuoteUpdateInfo;
	static _ATL_FUNC_INFO m_OnErrorInfo;
	static _ATL_FUNC_INFO m_OnSubscribedInfo;
	static _ATL_FUNC_INFO m_OnUnsubscribedInfo;


	static _ATL_FUNC_INFO m_OnControllingEventInfo;

	static _ATL_FUNC_INFO m_OnSOAPErrorInfo;
	static _ATL_FUNC_INFO m_OnSOAPResultInfo;
	
	static _ATL_FUNC_INFO m_OnIVAuthErrorInfo;
	static _ATL_FUNC_INFO m_OnIVAuthResultInfo;

#ifdef _VOLA_MANAGER
	static _ATL_FUNC_INFO m_OnDefaultSurfaceChangedInfo;
	static _ATL_FUNC_INFO m_OnDatabaseConnectionChangedInfo;
	static _ATL_FUNC_INFO m_OnPriceProviderChangedInfo;
	static _ATL_FUNC_INFO m_OnIVSettingsChangedInfo;
	static _ATL_FUNC_INFO m_OnTradeUpdateInfo;

#endif //_VOLA_MANAGER

	STDMETHOD (OnLastQuote)(PP::QuoteUpdateParams *Params, PP::QuoteUpdateInfo *Results);
	STDMETHOD (OnQuoteUpdate)();
	STDMETHOD (OnError) (PP::ErrorNumberEnum ErrorNumber, BSTR Description, PP::RequestsTypeEnum ReqType, VARIANT Request);
	STDMETHOD (OnSubscribed)(PP::QuoteUpdateParams *Params);
	STDMETHOD (OnUnsubscribed)();

	STDMETHOD (OnControllingEvent)(IVM::IVMCommandTypeEnum, VARIANT vt);
	
#ifdef _VOLA_MANAGER
	STDMETHOD (OnDefaultSurfaceChanged)(/*[in]*/long SurfaceID, /*[in]*/EOptType OptType, /*in*/VARIANT_BOOL NewDefault);
	STDMETHOD (OnDatabaseConnectionChanged)(/*[in]*/BSTR bsConnectionString);
	STDMETHOD (OnPriceProviderChanged)(/*[in]*/ long lProviderType);
	STDMETHOD (OnIVSettingsChanged)(/*[in]*/ Stg::IVSettingsType enType);
	STDMETHOD (OnTradeUpdate)(/*[in]*/ ITradeUpdate* Params);

	void UpdateVola( );
	
	void ReLoadContractPos();

#endif //_VOLA_MANAGER

	void SetStatusText( LPCTSTR szStatus )
	{
		m_csStatus.Lock();
		m_strStatus = szStatus;
		m_csStatus.Unlock();
	}

	void GotQuoteResponse( _bstr_t& szSymbol, bool bSubscribe );
	
	void GotLastQuoteResponse();

	void SetReady ();
	
	void ErrorNotification( _com_error & e, bool bFatal = true );
	void ErrorNotification( LPCTSTR szError, bool bFatal = true, bool bTerminate = false );

	void VolaChangedNotification( const COptionData & opt );
	void VolaChangedNotification();
	
	void UnderlyingChangedNotification( int ID, double fBidPrice, double fAskPrice, double fLastPrice );

	double CalcRates(DOUBLE dYTE);

	bool CalcIV( COptionData& opt );
	void LoadStructure( int nID );
	void LoadRates();
	void LoadCustomVola( int nID, const std::vector<DATE>& mnths );
	void LoadVolaManagementData( int nID );
	void LoadSyntheticRoot( int nRootID , CSyntheticOptionRootData* pSynthRoot );
	void LoadBasketIndexDividends( int nID, IEtsIndexDivCollPtr& spDiv,double dCoeff = 1.0f );
	void LoadSynthRootDividends( CSyntheticOptionRootData& synthRoot );

	void InitialRequests( bool bPrices = true, bool bRanges  = true );

	bool RecalcOptions(CString strSymbol = _T(""));
	
	void UpdateVolaCache();
	void UpdateMidMarket();
	void UpdateVolaCache( const std::wstring & strOption, const COptionData & opt );

	void UpdatePrice( PP::QuoteUpdateParams *Params, PP::QuoteUpdateInfo *Results );

	double GetMidMarket( double dBid, double dAsk, double dLast );

	mutable CCriticalSection m_csSymbol;
	CString m_strSymbol;
	
	CIVConnection m_ivc;
	PP::IPriceProviderPtr m_spPriceProvider;
	IVM::IIVControlPtr 	m_spControl;
	HANDLE    m_hStartMutex;

	
#ifdef _VOLA_MANAGER	
	VME::IVolatilityManagementPtr m_spVolaManagement;
	VME::IVolatilityManagementPtr m_spVolaMgmtExternal;
	MMW::ISubManagerPtr	m_spTradeSubscriber;
	Stg::ISettingsPtr m_spDBManagement;
	Stg::IIVSettingsPtr m_spIVManagement;
	bool InitializeVolaManager( LPCTSTR szConnection );

	IStreamPtr	m_spVolaMgmtStream; 
	
//	IPubManagerPtr		m_spPubManager;
//	ISubManagerPtr		m_spSubManager;
//	ISymbolObjectPtr	m_spSymbolObject;
//	IVMESurfacePtr		m_spVMESurface;
#endif //_VOLA_MANAGER
	
	CDBLayout m_dbCache;

	// The working copy of app's options. It's updated only from the main thread, and isn't accessed
	// from the DataProcessor's thread at the same time. So we don't have to synchronize it 
	CIVAnalyzerOptions m_opts; 

	CCriticalSection m_csVola;
	CCriticalSection m_csTimeSkew;
	CCriticalSection m_csVolaBands;
	CCriticalSection m_csReload;

	mutable  CCriticalSection m_csStatus;
	mutable  CCriticalSection m_csError;
	mutable  CCriticalSection m_csContractData;
	mutable  CCriticalSection m_csTradeData;
	mutable  CCriticalSection m_csSynthUndList;

	CString m_strStatus;
	CString m_strError;

	::CEvent m_evtStopData;  // data processing has stopped 
	::CEvent m_evtWaiting;   // waiting for the COM events from PriceProvider or SOAP client
	::CEvent m_evtHandlingStopped; // all requests from the price provider and SOAP client canceled
	::CEvent m_evtReady;		// InitInstance successfully completed
	::CEvent m_evtError;	// Fatal error occurred

	long m_lGotUndCounter;
	long m_lTotalUndCounter;
	long m_lGotOptCounter;
	long m_lTotalOptCounter;
	long m_ErrorCount;


	bool m_bConnected;
	bool m_bInitial;
	bool m_bGotPrices;
	bool m_bGotRanges;
	
	underl_price_map   m_PriceData;
	CPriceProviderOptions m_oldPriceOpts;

	PP::IProvidersPtr     m_spProviders;
	PP::IProviderDataPtr  m_spProviderData;

	CContractData m_contractData;
	long		  m_PriceProviderType;
	

	long			m_lTradeSeqNum;
	options_map		m_optsData;
	time_skew_map	m_time_skewData;
	vola_bands_map  m_volaBandsData;

	std::map<UINT, IEtsIndexDivAtomPtr> m_StockDividends;
	synth_underl_map	m_SynthUndList;

	CIRCurveData	m_IRCurveData;
	rates_vec m_ratesBid;
	rates_vec m_ratesAsk;

	bool m_bGettingContract;
	bool m_bUpdateCache;
	bool m_bReload;
	long m_lState;
	static const double m_dPriceDelta;
protected:
	void SetRequestsStatusText (bool bPrices = true, bool bRanges  = true);
	void SubscribeOption (options_map::iterator it);
	void CalcTimeSkew();
	void LoadContractParams(int nID);
	CString GetErrorMessage (HRESULT hr, BSTR bstrDesc);
	void ContractChangedNotification(CString strContract);
	HRESULT CreatePriceProvider(long type);

	afx_msg void OnGetData	(WPARAM wParam, LPARAM lParam);
	afx_msg void OnStopHandling (WPARAM wParam, LPARAM lParam);
	afx_msg void OnReload (WPARAM wParam, LPARAM lParam);

	long m_nSleepDuration;
	long m_nSleepFrequency;
	long m_nUpdatesCount;

	// Generated message map functions
	//{{AFX_MSG(CDataProcessor)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

extern CDataProcessor g_DataProcessor;

inline CDataProcessor::CLockVola::CLockVola ()
	{ g_DataProcessor.m_csVola.Lock(); }
inline CDataProcessor::CLockVola::~CLockVola ()  
	{ g_DataProcessor.m_csVola.Unlock(); }

inline CDataProcessor::CLockTimeSkew::CLockTimeSkew ()
	{ g_DataProcessor.m_csTimeSkew.Lock(); }
inline CDataProcessor::CLockTimeSkew::~CLockTimeSkew ()  
	{ g_DataProcessor.m_csTimeSkew.Unlock(); }

inline CDataProcessor::CLockReload::CLockReload()
	{ g_DataProcessor.m_csReload.Lock(); }
inline CDataProcessor::CLockReload::~CLockReload()  
	{ g_DataProcessor.m_csReload.Unlock(); }

inline CDataProcessor::CLockVolaBands::CLockVolaBands()
	{ g_DataProcessor.m_csVolaBands.Lock(); }
inline CDataProcessor::CLockVolaBands::~CLockVolaBands()  
	{ g_DataProcessor.m_csVolaBands.Unlock(); }


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAPROCESSOR_H__C4FB1C8B_2A0D_47FA_BB20_518316DDAF4A__INCLUDED_)
