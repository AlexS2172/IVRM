#pragma once

//-----------------------------------------------------------------------------------------//
class CRequestKey
{
public:
	std::string	Symbol;
	std::string	Exchange;
	InstrumentTypeEnum	Type;

	bool operator < (const CRequestKey& aKey) const
	{
		return Type < aKey.Type
			|| Type == aKey.Type && Symbol < aKey.Symbol
			|| Type == aKey.Type && Symbol == aKey.Symbol && Exchange < aKey.Exchange;
	}

	bool operator == (const CRequestKey& aKey) const
	{
		return	Type == aKey.Type &&
			Symbol == aKey.Symbol && 
			Exchange == aKey.Exchange;
	}
};
typedef boost::shared_ptr<CRequestKey>	CRequestKeyPtr;
//-----------------------------------------------------------------------------------------//
struct RequestKeyPtr_less
	: public binary_function<CRequestKeyPtr, CRequestKeyPtr, bool>
{	// functor for operator<
	bool operator()(const CRequestKeyPtr& _Left, const CRequestKeyPtr& _Right) const
	{	// apply operator< to operands
		return (*_Left < *_Right);
	}
};
//-----------------------------------------------------------------------------------------//
enum ResponseTypeEnum
{
	enNoneResponse = 0,
	enQuotesResponse = 1,
	enRisksResponse = 2
};
//-----------------------------------------------------------------------------------------//
class CResponse
{
public:
	CResponse(ResponseTypeEnum enType, CRequestKeyPtr spRequestKey):
	m_enType(enType),
	m_bUpdated(false),
	m_spRequestKey(spRequestKey),
	m_nUpdatesCount(0)
	{
	};

	virtual ~CResponse() {}
public:
	ResponseTypeEnum	m_enType;
	bool				m_bUpdated;
	CRequestKeyPtr		m_spRequestKey;
	long				m_nUpdatesCount;				

	inline long GetUpdatesCount(){
		return	m_nUpdatesCount;
	};

	inline ResponseTypeEnum GetType(){
		return m_enType;
	};

	inline void	SetUpdated(){ 
		++m_nUpdatesCount;
		m_bUpdated = true; 
	};

	inline bool& IsUpdated(){
		return m_bUpdated;
	};

	inline void ResetUpdated(){
		m_bUpdated = false;
	};

	virtual void Clear() = 0;
private:
	CResponse(){};
};

typedef boost::shared_ptr<CResponse> CResponsePtr;
//-----------------------------------------------------------------------------------------//
class CRisksResponse: public CResponse
{
public:
	CRisksResponse(ResponseTypeEnum enType, CRequestKeyPtr spRequestKey):
	CResponse(enType, spRequestKey)
	{
		Clear();
	};

	virtual ~CRisksResponse() {}
public:
	double	TheoPrice;	
	double	TheoPriceBid;
	double	TheoPriceAsk;
	double	Alpha;
	double	Delta;
	double	Gamma;
	double	Vega;
	double	Theta;
	double	Rho;
	double	Volga;

	double	DeltaVega;
	double	DeltaTheta;
	double	GammaVega;
	double	GammaTheta;

	double	IVola;
	double	IVolaBid;
	double	IVolaAsk;
	double	TheoVola;

	double	BaseContractPrice;
	double	BaseContractPriceBid;
	double	BaseContractPriceAsk;
	double	ContractPrice;
	double	ContractPriceBid;
	double	ContractPriceAsk;

	double	IntrinsicValue;

	double	BidPrice;
	DATE	BidDate;
	long	BidSize;
	double	AskPrice;
	DATE	AskDate;
	long	AskSize;
	double	LastPrice;
	DATE	LastDate;
	long	Volume;

	DATE	UpdateDate;

public:
	void	Clear()
	{
		TheoPrice = Alpha = Delta = Gamma = Vega = Theta = Rho = Volga = BAD_DOUBLE_VALUE;
		TheoPriceAsk = TheoPriceBid = IntrinsicValue = BAD_DOUBLE_VALUE;
		DeltaVega = DeltaTheta = GammaVega = GammaTheta = BAD_DOUBLE_VALUE;
		IVola = IVolaBid = IVolaAsk = TheoVola = BAD_DOUBLE_VALUE;
		BaseContractPrice = ContractPrice = BAD_DOUBLE_VALUE;
		BaseContractPriceBid = ContractPriceBid = BAD_DOUBLE_VALUE;
		BaseContractPriceAsk = ContractPriceAsk = BAD_DOUBLE_VALUE;
		BidPrice = AskPrice = LastPrice = BAD_DOUBLE_VALUE;
		BidSize = AskSize = Volume = BAD_LONG_VALUE;
		BidDate = AskDate = LastDate = UpdateDate = 0.;
	};
};
//-----------------------------------------------------------------------------------------//
class CQuoteResponse: public CResponse
{
public:
	CQuoteResponse(ResponseTypeEnum enType, CRequestKeyPtr spRequestKey):
	CResponse(enType, spRequestKey)
	{
		Clear();
	};

public:
	DATE			UpdateDateTime;
	DATE			BidDateTime;
	DATE			AskDateTime;
	DATE			LastDateTime;
	long			SeqNum;
	double			BidPrice;
	double			AskPrice;
	long			BidSize;
	long			AskSize;
	long			LotSize;
	double			LastPrice;
	double			OpenInterest;
	long			Volume;
	std::string		Exchange;
	std::string		Currency;

	double			OpenPrice;
	double			ClosePrice;
	double			HighPrice;
	double			LowPrice;

	long			Skipped;
	long			TotalRequests;
	long			WaitTime;
	double			NetChange;
	QuoteStatusType Status;

	std::string		BidExchange;
	std::string		AskExchange;
	long			PriceTick; 

public:
	void Clear()
	{
		UpdateDateTime = BidDateTime = AskDateTime = LastDateTime = 0.;
		SeqNum = BidSize = AskSize = LotSize = Volume = Skipped = 
			TotalRequests = WaitTime = PriceTick = BAD_LONG_VALUE;
		BidPrice = AskPrice = LastPrice = OpenInterest = OpenPrice =
			ClosePrice = HighPrice = LowPrice = NetChange = BAD_DOUBLE_VALUE;
	};
};
//-----------------------------------------------------------------------------------------//
typedef std::map<ResponseTypeEnum, CResponsePtr>	CResponseColl;
typedef boost::shared_ptr<CResponseColl>			CResponseCollPtr;
typedef std::map<CRequestKeyPtr, CResponseCollPtr, RequestKeyPtr_less>	CDataColl;
typedef boost::shared_ptr<CDataColl>	CDataCollPtr;
typedef std::vector<CRequestKeyPtr>		CRequestColl;
typedef boost::shared_ptr<CRequestColl>	CRequestCollPtr;
typedef std::vector<CResponsePtr>		CResponseVector;
//-----------------------------------------------------------------------------------------//
class CGatewayNotify
{
public:
	virtual void	OnQuoteUpdate() = 0;
	virtual void	OnLastQuote() = 0;
	virtual void	OnSubscribed(CRequestKeyPtr& request) = 0;
	virtual void	OnUnsubscribed(CRequestKeyPtr& request) = 0;

	virtual void	OnSubscribedRisks(CRequestKeyPtr& request) = 0;
	virtual void	OnUnsubscribedRisks(CRequestKeyPtr& request) = 0;
	virtual void	OnRiskUpdate() = 0;

	virtual void	OnError(CRequestKeyPtr& request, std::string& error, long code, long requestType) = 0;
};
//-----------------------------------------------------------------------------------------//
class CGateway
{
public:
	CGateway(CGatewayNotify* client);
	virtual ~CGateway();

	virtual long	Subscribe(CRequestKeyPtr& request, ResponseTypeEnum type) = 0;
	virtual long	SubscribeMultiple(CRequestColl& request, ResponseTypeEnum type) = 0;
	virtual long	Request(CRequestKeyPtr& request, ResponseTypeEnum type, CResponsePtr& response) = 0;
	virtual long	RequestMultiple(CRequestColl& request, ResponseTypeEnum type, CResponseVector& response) = 0;

	virtual long	Connect() = 0;
	virtual long	Disconnect() = 0;

	bool GetResponse(CRequestKeyPtr key,
					ResponseTypeEnum type,
					CResponsePtr& response);

	bool FindResponse(CRequestKeyPtr key,
					  ResponseTypeEnum type);
private:
	CGateway(){};

public:
	CDataCollPtr		m_spData;
	CGatewayNotify*		m_client;
	CLock				m_dataLock;
};
//-----------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------//
class CTaskInterface
{
public:
	virtual void DoTask() = 0;
	virtual ~CTaskInterface(){};
	typedef CTaskInterface*	pointer;
};
//-----------------------------------------------------------------------------------------//
class CTaskExecutor
{
public:
	typedef CTaskInterface::pointer RequestType;

	CTaskExecutor(void){};
	virtual ~CTaskExecutor(void){};

	virtual BOOL Initialize(void*)
	{
		return SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED))?TRUE:FALSE;
	}

	virtual void Terminate(void*)
	{
		::CoUninitialize();
	}

	void Execute(RequestType dw, void*, OVERLAPPED*)
	{
		CTaskInterface::pointer pTask = reinterpret_cast<CTaskInterface::pointer>(dw);
		
		try	{
			if (pTask)
				pTask->DoTask();
				
			else throw std::runtime_error("invalid pointer of task.");
			
		}
		catch (std::runtime_error err) {
			TRACE_ERROR(_T("runtime error: [%s]"), _T(err.what()));
			throw err;
		}
		catch (...) {
			TRACE_UNKNOWN_ERROR();
		}
		delete pTask;
	};
};
//-----------------------------------------------------------------------------------------//
class CSubscribeSingle: public CTaskInterface
{
public:
	CSubscribeSingle(CGateway* gateway, CRequestKeyPtr request, ResponseTypeEnum type):
	m_spRequest(request),
	m_gateway(gateway),
	m_enType(type)
	{};
public:
	CGateway*			m_gateway;
	CRequestKeyPtr		m_spRequest;
	ResponseTypeEnum	m_enType;

	virtual void DoTask();
};
//-----------------------------------------------------------------------------------------//
class CSubscribeMultiple: public CTaskInterface
{
public:
	CSubscribeMultiple(CGateway* gateway, CRequestCollPtr request, ResponseTypeEnum type):
	m_spRequest(request),
	m_gateway(gateway),
	m_enType(type)
	{};
public:
	CGateway*			m_gateway;
	CRequestCollPtr		m_spRequest;
	ResponseTypeEnum	m_enType;

	virtual void DoTask();
};
//-----------------------------------------------------------------------------------------//
class CRequestSingle: public CTaskInterface
{
public:
	CRequestSingle(CGateway* gateway, CRequestKeyPtr request, ResponseTypeEnum type):
	m_spRequest(request),
	m_gateway(gateway),
	m_enType(type)
	{};
public:
	CGateway*			m_gateway;
	CRequestKeyPtr		m_spRequest;
	ResponseTypeEnum	m_enType;

	virtual void DoTask();
};
//-----------------------------------------------------------------------------------------//
class CRequestMultiple: public CTaskInterface
{
public:
	CRequestMultiple(CGateway* gateway, CRequestCollPtr request, ResponseTypeEnum type):
	m_spRequest(request),
	m_gateway(gateway),
	m_enType(type)
	{};
public:
	CGateway*			m_gateway;
	CRequestCollPtr		m_spRequest;
	ResponseTypeEnum	m_enType;

	virtual void DoTask();
};
//------------------------------------------------------------------------------------------//
class CStopEvent
{
public:
	static bool CreateEvent()
	{
		if (!hEvent)
		{
			hEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
			if (hEvent)
				return true;
		}
		return false;
	}

	static void CloseEvent()
	{
		if (hEvent)
		{
			CloseHandle(hEvent);
			hEvent = NULL;
		}
	}
	static BOOL SetEvent()
	{
		return ::SetEvent(hEvent);
	}
	static bool HasSignaledState()
	{
		return (::WaitForSingleObject(hEvent, 0) == WAIT_OBJECT_0);
	}
	static HANDLE GetHandle() { return hEvent; }


	static HANDLE hEvent;
};
//------------------------------------------------------------------------------------------//