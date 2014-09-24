/****************************************************************************
*
*	Market message structure definition file.
*
*	Eugene Karnygin (EVK) 14 jan 2002
*
*	Copyright (C) 2002 EGAR technology incorporation. All right reserved.
*
*****************************************************************************/

/****************************************************************************/

#define u_short		unsigned short
#define u_long		unsigned long

/****************************************************************************/

enum MsgClass
{
	mcQuote,
	mcTrade,
	mcVolatility,
	mcMarketStructure,
	mcSystem,
	mcInterestRate,
	mcWeightedVega,
};

enum MsgType
{
	mtRequest,
	mtUpdate,
};

struct HeaderData
{
	MsgClass	Class;
	MsgType		Type;
	u_short		nSize;
};

/****************************************************************************/

#define MAX_SYMBOL_NAME		8
#define MAX_EXHCANGE_NAME	3
#define MAX_CURRENCY_NAME	4

struct SymbolData
{
	long	Type;
	char	Symbol[MAX_SYMBOL_NAME];
	char	Exchange[MAX_EXHCANGE_NAME];
	char	Currency[MAX_CURRENCY_NAME];
};

/****************************************************************************/

struct QuoteData :
	public HeaderData, 
	public SymbolData
{
	float	fBidPrice;
	float	fAskPrice;

	long	nBidSize;
	long	nAskSize;
};

/****************************************************************************/

struct TradeRequestMsg :
	public HeaderData
{
	u_long	SeqFrom;	// (Can be 0)
	u_long	SeqTo;		// (Can be 0)
	u_long	SeqLast;	// Used to request new trades (Can be 0)
}

struct TradeUpdateMsg :
	public HeaderData, 
	public SymbolData
{
	u_long	nSeqNum;

	u_long	nTradeID;
	u_long	nOwnerID;

	float	fPrice;
	long	nVolume;
	DATE	ExecDate;
};

/****************************************************************************/

	struct DividendData
	{
		u_short		nFreq;
		DATE		LastDate;
		float		fAmount;
	};

	struct OptionData
	{
		char	OptionSymbol[8];
		float	fStrike;
		DATE	ExpDate;
		
		float	fTheoPrice;
		float	fClosePrice;
		long	lPosition;
	};

	struct MarketStructureData
	{
		float				fClosePrice;
		DividendData		DivInfo;
		
		u_short				nOptionsCount;	
		OptionData*			pOptions;	
	};

/****************************************************************************/

struct MarketStructureRequestMsg :
	public HeaderData,
	public SymbolData
{
	DATE	LastUpdate;
};

struct MarketStructureUpdateMsg :
	public HeaderData,
	public SymbolData
{
	DATE					Date;		// = 0 if no updates
	MarketStructureData*	pData;		// = NULL if Date = 0
};

/****************************************************************************/

	struct VolaPointData
	{	
		u_long		PointID;
		u_long		ExpDate;
		float		fStrike;
		float		fVola;
		byte		bStatus;
		bool		IsBasePoint;
	};

struct VolaRequestMsg :
	public HeaderData,
	public SymbolData
{
};

struct VolaUpdateMsg :
	public HeaderData,
	public SymbolData
{
	float			fUndPrice;
	float			fSmileAccel;
	float			fInterpFactor;
	bool			DiscreteAccel;
	bool			PriceOverride;
	u_long			PointsCount;

	VolaPointData*	pPoints;
};

/****************************************************************************/

struct InterestRateData
{
	u_long	nDays;
	float	fLong;
	float	fShort;
	float	fHTB;	// Hard to borrow
};

struct InterestRateRequestMsg :
	public HeaderData
{
};

struct InterestRateUpdateMsg :
	public HeaderData
{
	u_long				nRatesCount;
	InterestRateData*	pRates;
};

/****************************************************************************/

struct WeightedVegaItemData
{
	DATE	Date;
	float	fValue;
};

struct WeightedVegaRequestMsg :
	public HeaderData,
	public SymbolData
{
};

struct WeightedVegaUpdateMsg :
	public HeaderData,
	public SymbolData
{
	u_long					nRefMatDays;
	u_long					nItemsCount;
	WeightedVegaItemData*	pItems;
};

/****************************************************************************/

struct SystemUpdateMsg :
	public HeaderData
{
};

/****************************************************************************/
