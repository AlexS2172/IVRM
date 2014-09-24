#define DEF_PROVIDER_ID		10L
#define DEF_PROVIDER_NAME	L"Data Feed Provider" 
#define PROVIDER_SETTINGS_ROOT	_T("SOFTWARE\\Egar\\Providers\\{34A0BAE2-87B1-4BDF-A6E8-E8C5B8405B6F}")
#define PROVIDER_NODE		"Provider"
#define REMOTEOBJECT_NODE	"RemoteObjName"
#define USERNAME_NODE		"UserName"
#define PASSWORD_NODE		"Password"
#define COMPRESSION_NODE	"Compression"
#define SERVERTIMEOUT_NODE	"ServerTimeout"
#define SERVERENDPOINT_NODE "ORBendPoint"
#define ORBTRACELEVEL_NODE	"ORBtraceLevel"
#define ATTEMPTS_NODE		"AttemptsToConnect"
#define LOG_LIFETIME		"LogLifetime"
#define LOG_LEVEL			"LogLevel"
#define PULL_TIMEOUT		"PullTimeout"
#define LOG_DIRECTORY		"LogDirectory"
#define TRACEINFO(a,b)		//EgLib::CEgLibTraceManager::Trace(EgLib::LogInfoExt,a,b)
#define TRACEERROR(a,b)		//EgLib::CEgLibTraceManager::Trace(EgLib::LogError,a,b)
#define TRACEWARNING(a,b)	//EgLib::CEgLibTraceManager::Trace(EgLib::LogWarning,a,b)
#define TRACEDEBUG(a,b)		//EgLib::CEgLibTraceManager::Trace(EgLib::LogDebug,a,b)
#define PULL_THREAD_WAIT	1000
#define BAD_DOUBLE_VALUE	-1E+308
#define BAD_LONG_VALUE		-2147483647

class CTicker
{	
public:
	CTicker() 
	{
	};

	CTicker(const CTicker& ticker)
	{
		m_sSymbol.Append(ticker.m_sSymbol);
		m_sExchange.Append(ticker.m_sExchange);
		m_enType = ticker.m_enType;
	};

	CTicker(const QuoteUpdateParams& aKey)
	{
		m_sSymbol = (LPCTSTR)_bstr_t(aKey.Symbol);
		m_enType = aKey.Type;
		m_sExchange = (LPCTSTR)_bstr_t(aKey.Exchange);
	}

	CTicker(const ITicker& aKey)
	{
		m_sSymbol = (LPCTSTR)_bstr_t(aKey.Symbol);
		m_enType = aKey.Type;
		m_sExchange = (LPCTSTR)_bstr_t(aKey.Exchange);
	}

	virtual ~CTicker(void)
	{
	}

	void CopyTo(QuoteUpdateParams& aKey) const
	{
		aKey.Symbol = _bstr_t(m_sSymbol).Detach();
		aKey.Type = m_enType;
		aKey.Exchange = _bstr_t(m_sExchange).Detach();
	}

	void CopyTo(ITicker& aKey) const
	{
		aKey.Symbol = _bstr_t(m_sSymbol).Detach();
		aKey.Type = m_enType;
		aKey.Exchange = _bstr_t(m_sExchange).Detach();
	}

	bool operator < (const CTicker& aKey) const
	{
		return m_enType < aKey.m_enType
			|| m_enType == aKey.m_enType && m_sSymbol < aKey.m_sSymbol
			|| m_enType == aKey.m_enType && m_sSymbol == aKey.m_sSymbol && m_sExchange < aKey.m_sExchange;
	}

	bool operator == (const CTicker& aKey) const
	{
		return	m_enType == aKey.m_enType &&
			m_sSymbol == aKey.m_sSymbol && 
			m_sExchange == aKey.m_sExchange;
	}	

public:
	CString m_sSymbol;
	
	InstrumentTypeEnum m_enType;
	CString m_sExchange;
};

typedef boost::shared_ptr<CTicker>	CTickerPtr;

typedef CComRecPtr<QuoteUpdateParams, &LIBID_PRICEPROVIDERSLib >	_QuoteUpdateParams;
typedef CComRecPtr<ITicker, &LIBID_PRICEPROVIDERSLib >				_Ticker;
typedef CComRecPtr<QuoteUpdateInfo, &LIBID_PRICEPROVIDERSLib >		_QuoteUpdateInfo;
typedef CComRecPtr<IRisks, &LIBID_PRICEPROVIDERSLib >				_RisksUpdateInfo;
typedef CComRecPtr<OptionResultsEx, &LIBID_PRICEPROVIDERSLib >		_OptionUpdateInfo;
typedef CComRecPtr<StockResultsEx, &LIBID_PRICEPROVIDERSLib >		_StockUpdateInfo;
typedef CComRecPtr<OptionParams, &LIBID_PRICEPROVIDERSLib >			_OptionUpdateParams;
typedef CComRecPtr<StockParams, &LIBID_PRICEPROVIDERSLib >			_StockUpdateParams;

typedef CComRecPtr<FutureResultsEx, &LIBID_PRICEPROVIDERSLib >		_FutureUpdateInfo;
typedef CComRecPtr<FutureParams, &LIBID_PRICEPROVIDERSLib >			_FutureUpdateParams;
typedef CComRecPtr<FutureRootParams, &LIBID_PRICEPROVIDERSLib >		_FutureRootUpdateParams;

typedef CComRecPtr<FuturesOptionResultsEx, &LIBID_PRICEPROVIDERSLib >	_FutureOptionUpdateInfo;
typedef CComRecPtr<FuturesOptionParams, &LIBID_PRICEPROVIDERSLib >		_FutureOptionUpdateParams;

typedef CComRecPtr<IRatesFullInfo, &LIBID_PRICEPROVIDERSLib>		_RatesUpdateFullInfo;
typedef CComRecPtr<IRate, &LIBID_PRICEPROVIDERSLib>					_Rates;

typedef CComRecPtr<DividendsResults, &LIBID_PRICEPROVIDERSLib >		_DividendsUpdateInfo;

struct EquiteBaseInfo
{
	std::string		StockSymbol;
	double			DIV_Date;
	short			DIV_Freq;
	float			DIV_AMT_YIELD;
	std::string		currency;
	char			StockType;
	float			PriceClose;
	float			PriceHigh52WK;
	float			PriceLow52WK;
	float			SOD_IVX30;
	float			SOD_IVX30_YEST;
	float			SOD_IVX30_High52WK;
	float			SOD_IVX30_Low52WK;
	float			SOD_HV20;
	float			SOD_HV20_YEST;
	float			SOD_HV20_High52WK;
	float			SOD_HV20_LOW52WK;
	int				SOD_OPT_VOLUME_CALL;
	int				SOD_OPT_VOLUME_PUT;
	float			SOD_OPT_VOLUME_AVG;
	float			OutstandingShares;
};
struct OptionBaseInfo
{
	std::string		OptionSymbol;
	std::string		StockSymbol;
	double			Expiration;
	double			Strike;
	char			OptionType;
	char			OptionStyle;
	int				OpenInterest;
};

struct EquiteQuoteInfo
{
	std::string		StockSymbol;
	double			Bid;
	std::string		BidExchange;
	double			BidDate;
	int				BibSize;
	double			Ask;
	std::string		AskExchange;
	double			AskDate;
	int				AskSize;
	double			Last;
	std::string		LastExchange;
	double			LastDate;
	int				LastSize;
	__int64			Volume;
};

struct OptionQuoteInfo
{
	std::string		OptionSymbol;
	std::string		StockSymbol;
	double			IV;
	float			PreIV;
	double			Delta;
	double			Gamma;
	double			Vega;
	double			Theta;
	float			Rho;
	double			Bid;
	double			BidDate;
	std::string		BidExchange;
	int				BidSize;
	double			Ask;
	double			AskDate;
	std::string		AskExchange;
	int				AskSize;
	double			Last;
	double			LastDate;
	std::string		LastExchange;
	int				LastSize;
	__int64			Volume;
};

enum RequestType
{
	enQuoteUpdate = 0x0,
	enStockRequest = 0x1,
	enOptionRequest = 0x2,
	enSubscribeRequest = 0x3,
	enDividendsRequest = 0x4
};

struct StockResultsExFullInfo
{
	StockResultsEx	Info;
	StockParams		Params;
};

struct OptionResultsExFullInfo
{
	OptionResultsEx	Info;
	StockParams		Params;
};

typedef boost::shared_ptr<IContractsFullInfo>			ContractsFullInfoPtr;
typedef boost::shared_ptr<EquiteBaseInfo>				EquiteBaseInfoPtr;
typedef boost::shared_ptr<EquiteQuoteInfo>				EquiteQuoteInfoPtr;
typedef boost::shared_ptr<QuoteUpdateParams>			QuoteUpdateParamsPtr;
typedef boost::shared_ptr<ITicker>						TickerPtr;
typedef boost::shared_ptr<StockResultsEx>				StockResultsExPtr;
typedef boost::shared_ptr<OptionResultsEx>				OptionResultsExPtr;
typedef boost::shared_ptr<IRisksFullInfo>				RisksFullInfoPtr;
typedef boost::shared_ptr<QuoteUpdateFullInfo>			QuoteUpdateFullInfoPtr;
typedef boost::shared_ptr<StockParams>					RequestParamsPtr;
typedef boost::shared_ptr<OptionParams>					RequestOpParamsPtr;

typedef boost::shared_ptr<StockResultsExFullInfo>		StockResultsExFullInfoPtr;
typedef boost::shared_ptr<OptionResultsExFullInfo>		OptionResultsExFullInfoPtr;

typedef boost::shared_ptr<OptionResultsEx>				OptionResultsExPtr;
typedef boost::shared_ptr<StockResultsEx>				StockResultsExPtr;

typedef boost::shared_ptr<OptionBaseInfo>				OptionBaseInfoPtr;
typedef boost::shared_ptr<OptionQuoteInfo>				OptionQuoteInfoPtr;
typedef boost::shared_ptr<DividendsFullInfo>			DividendsFullInfoPtr;
typedef boost::shared_ptr<IRate>						IRatePtr;
typedef boost::shared_ptr<IRatesFullInfo>				IRatesFullInfoPtr;

typedef std::map<ITicker, ContractsFullInfoPtr>			ContractsUpdateMap;
typedef std::map<CTicker, CTickerPtr>					QuoteUpdateMap;
typedef std::map<CTicker, CTickerPtr>					RisksUpdateInfoMap;
typedef std::map<std::string, RequestParamsPtr>			RequestMap;
typedef std::map<std::string, QuoteUpdateFullInfoPtr>	QuoteUpdateInfoMap;
typedef std::map<std::string, StockResultsExFullInfoPtr>		StockInfoMap;
//typedef std::map<CTicker, DividendsFullInfoPtr>				DividendsInfoMap;
typedef std::map<std::string, OptionResultsExFullInfoPtr>		OptionInfoMap;
typedef std::map<CTicker, RisksFullInfoPtr>					RisksUpdateMap;
typedef CComRecPtr<QuoteUpdateFullInfo, &LIBID_PRICEPROVIDERSLib>	_QuoteUpdateFullInfo;
typedef CComRecPtr<IRisksFullInfo, &LIBID_PRICEPROVIDERSLib>	_RisksUpdateFullInfo;

namespace DFFields
{
	static const CORBA::Short T_EQUITY_QUOTE = DDS::T_EQUITY_QUOTE;
	static const CORBA::Short T_OPTION_QUOTE = DDS::T_OPTION_QUOTE;	
	static const CORBA::Short T_EQUITY_BASE = DDS::T_EQUITY_BASE;
	static const CORBA::Short T_OPTION_BASE = DDS::T_OPTION_BASE;
	static const CORBA::Short T_FUTURE_QUOTE = DDS::T_FUTURE_QUOTE;
	static const CORBA::Short T_FUTURE_OPTION_QUOTE = DDS::T_DIRECT_FUTURE_OPTION_QUOTE;

	static const CORBA::Short T_IVRM_RISKS = 35;

	static const CORBA::Short FIELD_SYMBOL				= 291;
	static const CORBA::Short FIELD_BASE_SYMBOL			= 292;
	static const CORBA::Short FIELD_SYMBOL_TYPE			= 293;
	static const CORBA::Short FIELD_BASE_SYMBOL_TYPE	= 294;
	static const CORBA::Short FIELD_IV					= DDS::FIELD_IV;
	static const CORBA::Short FIELD_DELTA				= DDS::FIELD_DELTA;
	static const CORBA::Short FIELD_VEGA				= DDS::FIELD_VEGA;
	static const CORBA::Short FIELD_GAMMA				= DDS::FIELD_GAMMA;
	static const CORBA::Short FIELD_THETA				= DDS::FIELD_THETA;
	static const CORBA::Short FIELD_RHO					= DDS::FIELD_RHO;
	static const CORBA::Short FIELD_THEO_PRICE			= 282;
	static const CORBA::Short FIELD_CONTRACT_PRICE		= 283;
	static const CORBA::Short FIELD_BASECONTRACT_PRICE	= 284;
	static const CORBA::Short FIELD_VOLGA				= 285;
	static const CORBA::Short FIELD_DELTA_VEGA			= 286;
	static const CORBA::Short FIELD_DELTA_THETA			= 287;
	static const CORBA::Short FIELD_GAMMA_VEGA			= 288;
	static const CORBA::Short FIELD_GAMMA_THETA			= 289;
	static const CORBA::Short FIELD_THEO_VOLA			= 290;
	static const CORBA::Short FIELD_UPDATE_DATE			= 295;
	static const CORBA::Short FIELD_OPEN_PRICE			= 296;
	static const CORBA::Short FIELD_NET_CHANGE_1		= 297;
	static const CORBA::Short FIELD_PRICE_TICK			= 298;
	static const CORBA::Short FIELD_OPEN_INTEREST		= DDS::FIELD_OPEN_INTEREST;
	static const CORBA::Short FIELD_PRICE_HIGH			= DDS::FIELD_PRICE_HIGH;
	static const CORBA::Short FIELD_PRICE_LOW			= DDS::FIELD_PRICE_LOW;
	static const CORBA::Short FIELD_THEO_PRICE_BID			= 299;
	static const CORBA::Short FIELD_THEO_PRICE_ASK			= 300;
	static const CORBA::Short FIELD_CONTRACT_PRICE_BID		= 301;
	static const CORBA::Short FIELD_CONTRACT_PRICE_ASK		= 302;
	static const CORBA::Short FIELD_BASECONTRACT_PRICE_BID	= 303;
	static const CORBA::Short FIELD_BASECONTRACT_PRICE_ASK	= 304;
	static const CORBA::Short FIELD_INTRINSIC_VALUE			= 305;
}
