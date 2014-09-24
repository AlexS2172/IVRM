#pragma once

using namespace Activ;
using namespace ContentPlatform;
using namespace Feed;
using namespace FeedApi;

#define BAD_DOUBLE_VALUE -1E+308
#define BAD_LONG_VALUE   -2147483647

enum PriceTickTypeEnum
{	
	enLastSaleDnTick	= 0x8000,
	enLastSaleUpTick	= 0x4000,
	enOpenDnTick	= 0x2000,
	enOpenUpTick	= 0x1000,
	enCloseDnTick	= 0x800,
	enCloseUpTick	= 0x400,
	enBidDnTick	= 0x200,
	enBidUpTick	= 0x100,
	enAskDnTick	= 0x80,
	enAskUpTick	= 0x40
};

enum QuoteStatusTypeEnum
{	
	enQuoteStatus_OK	= 0,
	enQuoteStatus_Halted	= 1,
	enQuoteStatus_Unreliable	= 2
};

enum  InstrumentTypeEnum
{
	enNone = -1,
	enSTK	= 0,
	enFUT	= 1,
	enIDX	= 3,
	enOPT	= 4,
	enFOP	= 5,
	enFX	= 6
};

struct QuoteRequest
{
	std::string			Symbol;
	InstrumentTypeEnum	Type;
	std::string			Exchange;

	bool operator < (const QuoteRequest& aKey) const
	{
		return Type < aKey.Type
			|| Type == aKey.Type && Symbol < aKey.Symbol
			|| Type == aKey.Type && Symbol == aKey.Symbol && Exchange < aKey.Exchange;
	}

	bool operator == (const QuoteRequest& aKey) const
	{
		return	Type == aKey.Type &&
				Symbol == aKey.Symbol && 
				Exchange == aKey.Exchange;
	}
};

struct QuoteInfo
{
	DATE				UpdateDateTime;
	DATE				BidDateTime;
	DATE				AskDateTime;
	DATE				LastDateTime;
	long				SeqNum;
	double				BidPrice;
	double				AskPrice;
	long				BidSize;
	long				AskSize;
	std::string			Currency;
	long				LotSize;
	double				LastPrice;
	double				OpenInterest;
	long				Volume;
	std::string			Exchange;
	double				OpenPrice;
	double				ClosePrice;
	double				HighPrice;
	double				LowPrice;
	long				Skipped;
	long				TotalRequests;
	long				WaitTime;
	double				NetChange;
	QuoteStatusTypeEnum Status;
	std::string			BidExchange;
	std::string			AskExchange;
	std::string			BaseContract;
	LONG				PriceTick;

	QuoteInfo():
	BidExchange(""), 
	AskExchange(""), 
	Exchange(""), 
	Currency(""),
	BaseContract("")
	{
		BidPrice	= AskPrice= LastPrice= OpenInterest= OpenPrice= ClosePrice= HighPrice= LowPrice= NetChange= BAD_DOUBLE_VALUE;
		SeqNum		= BidSize= AskSize= LotSize= Volume= Skipped= TotalRequests= WaitTime= PriceTick= BAD_LONG_VALUE;
		Status		= enQuoteStatus_OK;
		UpdateDateTime = AskDateTime = BidDateTime = LastDateTime = 0.;
	}

};

typedef boost::shared_ptr<QuoteInfo>	QuoteInfoPtr;
typedef boost::shared_ptr<QuoteRequest>	QuoteRequestPtr;

struct QuoteAtom
{
	QuoteInfoPtr			m_spResponse;
	QuoteRequestPtr			m_spRequest;

	SubscriptionCookie		m_Cookie;
	bool					m_bUpdate;
	bool					m_Subscribed;

	CLock					m_lock;
};
typedef boost::shared_ptr<QuoteAtom>			QuoteAtomPtr;
typedef std::vector<QuoteAtomPtr>				QuoteList;

typedef boost::shared_ptr<Activ::RequestId>		RequestIdPtr;

struct RequestIdPtr_less
	: public binary_function<RequestIdPtr, RequestIdPtr, bool>
{	// functor for operator<
	bool operator()(const RequestIdPtr& _Left, const RequestIdPtr& _Right) const
	{	// apply operator< to operands
		return (*_Left < *_Right);
	}
};

struct QuoteRequestPtr_less
	: public binary_function<QuoteRequestPtr, QuoteRequestPtr, bool>
{	// functor for operator<
	bool operator()(const QuoteRequestPtr& _Left, const QuoteRequestPtr& _Right) const
	{	// apply operator< to operands
		return (*_Left < *_Right);
	}
};

typedef std::map<std::string, QuoteAtomPtr>	CResponseColl;
typedef std::map<RequestIdPtr, CResponseColl, RequestIdPtr_less >		CRequestMap;

typedef std::map<QuoteRequestPtr, QuoteAtomPtr, QuoteRequestPtr_less>	CRequestQuoteMap;
typedef	std::map<QuoteRequestPtr, RequestIdPtr, QuoteRequestPtr_less>	CSubscribtionMap;

typedef	std::vector<QuoteAtomPtr>		CQuoteVector;
typedef std::vector<QuoteRequestPtr>	CRequestVector;

typedef std::map<QuoteRequestPtr, long, QuoteRequestPtr_less>	CSubsQuoteMap;