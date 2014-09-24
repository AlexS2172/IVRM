/*
**** ATPBS.H - Include file for ATFI Price Block structures
****
**** This file contains all price block structures
*/

#ifndef __ATPBS_H__
#define __ATPBS_H__

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

// Structure to define a full time or date.

typedef struct _ATLTIME
{
	union
	{
		DWORD		datetime;
		struct
		{
			BYTE	bDate	:1;		// Date or time flag
			BYTE	hour	:7;			
			BYTE	minute;				
			BYTE	second;			
			BYTE	millisecond;
		};
		struct
		{
			BYTE	bDateLong	:1;	// Date or time flag
			BYTE	month	:7;
			BYTE	day;
			WORD	year;	
		};
	};
} ATLTIME, *LPATLTIME;

// Common symbol shared by all databases and messages representing
// the ticker symbol.

typedef struct _ATSYMBOL
{
	BYTE	type;					// Symbol type
	UCHAR   symbol[16];				// Symbol
	UCHAR   country[2];				// Country code
	UCHAR	exchange;				// Exchange code
} ATSYMBOL, *LPATSYMBOL;

typedef struct _ATMMSYMBOL
{
	BYTE	type;					// Symbol type
	UCHAR   symbol[10];				// Symbol
	UCHAR	mmid[6];				// Market Maker ID
	UCHAR   country[2];				// Country code
	UCHAR	exchange;				// Exchange code
} ATMMSYMBOL, *LPATMMSYMBOL;

		/* News Info Block: Story Info */

typedef struct _ATSTORYINFO
{
	DWORD			hour:	5;
	DWORD			minute: 6;
	DWORD			source: 10;
	DWORD			bDesktopSource: 1;
	DWORD			resv:	10;
	BYTE			reference[16];
} ATSTORYINFO, *LPATSTORYINFO;

// Translate CUSIP to symbol.

#define CUSIP_ENTRY_CHAR		'#'

// Use closing bid for ySettle and bid for last.

#define CLOSE_BID_CHAR			';'

// Prices are delayed.

#define DELAYED_QUOTE_CHAR		'>'

// Display index.

#define DISPLAY_INDEX_CHAR		'.'

// trends

#define TRENDS_LAST_UP			0x01
#define TRENDS_LAST_DOWN		0x02
#define TRENDS_LAST				0x03

#define TRENDS_BID_UP			0x04
#define TRENDS_BID_DOWN			0x08
#define TRENDS_BID				0x0C

#define TRENDS_ASK_UP			0x10
#define TRENDS_ASK_DOWN			0x20
#define TRENDS_ASK				0x30

// news

#define NEWS_AVAILABLE			0x0001

// alerts - Stocks

#define ALERTS_52_LOW			0x0001
#define ALERTS_52_HIGH			0x0002
#define ALERTS_LOW				0x0004
#define ALERTS_HIGH				0x0008
#define ALERTS_VOLUME			0x0010

// restricted

#define RESTRICTED_UPC			0x4000
#define RESTRICTED_NOTES		0x8000

// flags - DWORD all AT price blocks

#define FLAGS_OPEN				0x00000001
#define FLAGS_XDIV				0x00000002
#define FLAGS_SPLIT				0x00000004
#define FLAGS_BOND_CALLED		0x00000008
#define FLAGS_TRADE_FILTERED	0x00000010

/*-------------------------------------------------------------------------*/

		/* AT Generic price block header */

typedef struct _AT_HEADER_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		BYTE		reserved[96];
		DWORD		flags;			// Flags
		}
		AT_HEADER_PB, *LPAT_HEADER_PB;

		/* AT Bonds: PBT_AT_BONDS */

typedef	struct _AT_BOND_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		ATLTIME		matdate;		// maturity date
		ATLTIME		calldate;		// called date 
		long		high52week;		// 52 week high price 
		long		low52week;		// 52 week low price 
		long		bondrate;		// bond rate 
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE	    extra[2];		// Unused
		BYTE		reserved[4];
		DWORD		flags;			// Flags
		}
		AT_BOND_PB, *LPAT_BOND_PB;

		/* AT Mutual Funds: PBT_AT_MUTUAL */

typedef	struct _AT_MUTUAL_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		Close;			// Close
		long		Dividend;		// Dividend
		long		StockSplit;		// Stock Split
		long		NetAssetVal;	// Net Asset Valuation
		long		OfferPrice;		// Offering Price
		long		CapitalGains;	// Capital Gains
		long		Assets;			// Total Assets
		BYTE		AssetMult;		// Total Assets multiplier (exponent sld)
		char		Category;		// Category
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		ATLTIME		lasttime;		// Last time
		BYTE		reserved[60];
		DWORD		flags;			// Flags
		}
		AT_MUTUAL_PB, *LPAT_MUTUAL_PB;

		/* AT Money Market Funds: PBT_AT_MMFUND */

typedef	struct _AT_MMFUND_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		Yield7Day;		//   7 Day  Yield
		long		Yield14Day;		//  14 Day  Yield
		long		Yield30Day;		//  30 Day  Yield
		long		AvgMaturity;	// Average Maturity
		long		Yield90Day;		//  90 Day  Yield
		long		Yield120Day;	// 120 Day  Yield
		long		Yield180Day;	// 180 Day  Yield
		long		Yield1Year;		//   1 Year Yield
		long		Yield7DayAnnual;//   7 Day  Yield Annualized
		long		NetAssets;		// Actual value in k$
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE	    extra[2];		// Unused
		ATLTIME		lasttime;		// Last time
		BYTE		reserved[48];
		DWORD		flags;			// Flags
		}
		AT_MMFUND_PB, *LPAT_MMFUND_PB;

		/* AT Futures: PBT_AT_FUTURE */

typedef	struct _AT_FUTURE_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		ATLTIME		expiredate;		// Expiration date
		long		openint;		// Open Interest
		long		open1;			// Open 1
		long		open2;			// Open 2
		long		close1;			// Close 1
		long		close2;			// Close 2
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		char		trbate;			// Trade bate
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE	    extra;			// Unused
		DWORD		flags;			// Flags
		}
		AT_FUTURE_PB, *LPAT_FUTURE_PB;

		/* AT Futures Option: PBT_AT_FUTOPT */

typedef	struct _AT_FUTOPT_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		ATLTIME		lastradate;		// Last trade date
		long		strike;			// Strike
		long		openint;		// Open Interest
		long		close1;			// Close 1
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		char		trbate;			// Trade bate
		BYTE		pricrcode; 		// Price Representation code
		BYTE		pricuncode;		// Price Unit Code
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		char		putCall;		// Put/Call
		BYTE		optMonth;		// Option month (0 - 11)
		BYTE		strikeDecType;	// Strike decimal type
		BYTE		reserved[4];
		DWORD		flags;			// Flags
		}
		AT_FUTOPT_PB, *LPAT_FUTOPT_PB;

		/* AT Index: PBT_AT_INDEX */

typedef	struct _AT_INDEX_PB
		{
		BYTE		updType; 		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		char		trbate;			// Trade bate
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE	    extra;			// Unused
		BYTE		reserved[24];
		DWORD		flags;			// Flags
		}
		AT_INDEX_PB, *LPAT_INDEX_PB;

		/* AT Stock: PBT_AT_STOCK */

typedef	struct	_AT_STOCK_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low  price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		long		rgnlClose;		// Regional Close
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		WORD		news;			// News
		WORD		alerts;			// Alerts
		union
		{
			WORD	restricted;		// Restricted/Notes see note* below
			struct
			{
			WORD	restCode:	14;	// Restriction code
			WORD	restUPC:	1;	// UPC Restriction
			WORD	restNotes:	1;	// Notes available
			};
		};
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		char		lastExch;		// Composite last  exchange
		char		openExch;		// Composite open  exchange
		char		bidExch;		// Composite bid   exchange
		char		askExch;		// Composite ask   exchange
		char		highExch;		// Composite high  exchange
		char		lowExch;		// Composite low   exchange
		BYTE		extra[2];		// Reserved.
		BYTE		reserved[8];
		DWORD		flags;			// Flags
		}
		AT_STOCK_PB, *LPAT_STOCK_PB;

		/* AT NASDAQ Stock: PBT_AT_NASDAQ */

typedef	struct	_AT_NASDAQ_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low  price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		long		closingBid;		// Closing Bid price
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		WORD		news;			// News
		WORD		alerts;			// Alerts
		union
		{
			WORD	restricted;		// Restricted/Notes see note* below
			struct
			{
			WORD	restCode:	14;	// Restriction code
			WORD	restUPC:	1;	// UPC Restriction
			WORD	restNotes:	1;	// Notes available
			};
		};
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE		reserved[16];
		DWORD		flags;			// Flags
		}
		AT_NASDAQ_PB, *LPAT_NASDAQ_PB;

		/* AT Stock Options: PBT_AT_STKOPT */

typedef	struct _AT_STKOPT_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low  price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		ATLTIME		exprdate;		// Expiration date
		long		strike;			// Strike
		long		openint;		// Open Interest
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE		strikeDecType;	// Strike decimal type
		BYTE	    ydecType;		// Yesterday''s decimal type
		long	    yVolume;		// Yesterday''s volume
		long	    yBid;			// Yesterday''s bid
		long	    yAsk;			// Yesterday''s ask
		DWORD		flags;			// Flags
		}
		AT_STKOPT_PB, *LPAT_STKOPT_PB;

		/* AT Forex: PBT_AT_FOREX */

typedef	struct	_AT_FOREX_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low  price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		char		bank[4];		// Bank Code
		char		city[4];		// City Code
		char		region[4];		// Region Code
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE		reserved[14];
		DWORD		flags;			// Flags
		}
		AT_FOREX_PB, *LPAT_FOREX_PB;

		/* AT Fixed Income: PBT_AT_FIXED_INCOME */

typedef	struct	_AT_FIXED_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		long		last;			// Last price
		long		lastsize;		// Last size
		ATLTIME		lasttime;		// Last time
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		long		high;			// High price
		long		low;			// Low  price
		long		open;			// Open price
		long		close;			// Previous day close
		long		volume;			// Volume
		DWORD		tradeCount;		// Trade count
		DWORD		quoteCount;		// Quote count
		long		yield;			// Yield
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		tradeCond;		// Trade condition
		BYTE		quoteCond;		// Quote condition
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		long		lastYield;		// Last yield
		long		askYield;		// Ask yield
		long		bidYield;		// Bid yield
		BYTE		yieldDecType;	// Yield decimal type
		BYTE		reserved[9];
		DWORD		flags;			// Flags
		}
		AT_FIXED_PB, *LPAT_FIXED_PB;

		/* AT Market Maker: PBT_AT_MARKET_MAKER */

typedef	struct	_AT_MKTMKR_PB
		{
		BYTE		updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr
		ATMMSYMBOL	sym;			// Symbol type/symbol/mmid/CCE
		long		bid;			// Bid  price
		long		bidsize;		// Bid  size
		ATLTIME		bidtime;		// Bid  time
		long		ask;			// Ask  price
		long		asksize;		// Ask  size
		ATLTIME		asktime;		// Ask  time
		union
		{
			WORD	trends;			// Trends
			struct
			{
			WORD	lastTrend:	2;	// Last trend
			WORD	bidTrend:	2;	// Bid trend
			WORD	askTrend:	2;	// Ask trend
			WORD	resvTrend:	10;
			};
		};
		BYTE		quoteType;		// Quote type
		BYTE		quoteCond;		// Quote condition
		DWORD		quoteCount;		// Quote count
		BYTE		originMkt;		// Origin market
		BYTE	    currType;		// Currency type
		BYTE		displayDecType;	// Display decimal type
		BYTE		extra;			// Not Used
		BYTE		reserved[60];
		DWORD		flags;			// Flags
		}
		AT_MKTMKR_PB, *LPAT_MKTMKR_PB;

		/* News Info Block Long Block - All news info. */

typedef	struct _AT_NEWSINFO_LB
		{
		DWORD	   	totalCount;		// Total stories all sources
		BYTE	   	sourceMask[128];// Story flag per source, 512 AT - 512 Other 
		ATSTORYINFO	storyInfo[16];	// Story Info - 20 bytes each.
		}
		AT_NEWSINFO_LB, *LPAT_NEWSINFO_LB;

		/* AT News Info : PBT_AT_NEWS_INFO */

typedef struct _AT_NEWSINFO_NB
		{
		BYTE		updType; 		// Can be R,e,t
		BYTE		pbType;			// Price block type.
		BYTE		decType;		// Decimal type
		BYTE		qsn;			// Workstation server number.
		DTDP		dtdp;			// Server datatype/dataptr.
		ATSYMBOL	sym;			// Symbol type/symbol/CCE
		ATSTORYINFO	storyInfo;		// Latest Story Info.
		LPAT_NEWSINFO_LB lpBlock;	// Memory holding actual block (NEWS_INFO_LB)
		BYTE		reserved[72];
		DWORD		flags;			// Flags
		}
		AT_NEWSINFO_NB, *LPAT_NEWSINFO_NB;

/*-------------------------------------------------------------------------*/

typedef	struct _EXP_HEADER_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE    resv1;
		BYTE	data[112];			// Make same size as PB
		}
		EXP_HEADER_PB, *LPEXP_HEADER_PB;

		/* expanded future/future option price block */

typedef	struct	_EXP_FUTURE_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE    resv1;
		LONG	ysettle;
		LONG    last;
		LONG    high;
		LONG	low;
		LONG	bid;
		LONG	ask;
		LONG	open;
		LONG	open1;
		LONG	open2;
		LONG	close1;
		LONG	close2;
		DWORD   volume;
		DWORD   openInterest;
		WORD	bidSize;			// Actual value
		WORD	askSize;			// Actual value
		WORD	lastSize;			// Actual value  20
		WORD	timeTrade;			// time of last trade
		WORD	timeQuote;
		BYTE	bateCode;
		BYTE	priceType;			// decimal type (if not zero)
		BYTE    displayPriceType;	// display price type
		BYTE	trendFlag;			// 0 = unch,1=up,2=down, alerts
		WORD    expireDate;			// expiration date
		WORD    tickCount;
		BYTE	strikePriceType;	// price encoding for the strike price
		BYTE	resv2;
		LONG    lifeHigh;
		WORD    lifeHighDate;
		WORD    lifeLowDate;
		LONG    lifeLow;
		LONG    lowLimit;
		LONG    highLimit;
		BYTE	resv3[8];
		}
		EXP_FUTURE_PB, *LPEXP_FUTURE_PB;

		/* Expanded stock price block header */

typedef	struct	_EXP_STOCK_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE    resv1;
		LONG	ysettle;
		LONG    last;
		LONG    high;
		LONG	low;
		LONG	bid;
		LONG	ask;
		LONG	open;
		DWORD   volume;
		WORD	bidsize;			// Actual value
		WORD	asksize;			// Actual value
		WORD	lastsize;			// Actual value  20
		WORD	timelast;			// time of last trade
		WORD	timequote;
		BYTE	quoteCondition;		// Quote Condition
		BYTE	extra;
		BYTE	tradeCondition;		// Trade Condition
		BYTE	dectype;			// Decimal type (if not zero)
		BYTE	flags;				// Restricted/Notes see note* below
		BYTE	trendflag;			// 0 = unch,1=up,2=down, alerts (S&P)
		union
		{
			LONG	rgnlClose;		// Regional Close (NYSE and AMEX regionals)
			LONG	closingBid;		// Closing Bid price (NASDAQ)
			struct					// market center of open/close/high/low/bid/ask
			{						//  (NYSE and AMEX composites)
				BYTE	gmclsop;
				BYTE	gmchilo;
				BYTE	gmcbidask;
				BYTE    resv3;
			};
		};
		union
		{
			WORD	tosptr;			// tick data offset in TIC.DAT
			WORD	tickCount;
		};
		BYTE	resv2[6];
		}
		EXP_STOCK_PB, *LPEXP_STOCK_PB;

/*-------------------------------------------------------------------------*/

		/* PC Quote generic price block header */

typedef	struct _PQ_HEADER_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	data[2];
		}
		PQ_HEADER_PB, *LPPQ_HEADER_PB;

		/* PC Quote Bonds: DT_PQ_xx_BONDS*/

typedef	struct _PQ_BOND_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		WORD	bidsize;			// size of bid
		WORD	asksize;			// size of ask
		WORD	lastsize;			// size of last trade
		BYTE	volume[3];			// Actual value
		WORD	matdate;			// maturity date
		BYTE	bondrate[3];		// bond rate 
		BYTE	decimal;			// decimal (non-US only)
		BYTE	flags;				// Flags: msb = called
		BYTE	extra;				//  ** extra **
		WORD	calldate;			// called date 
		BYTE	high52week[3];		// 52 week high price 
		BYTE	low52week[3];		// 52 week low price 
		BYTE	reserved[12];
		}
		PQ_BOND_PB, *LPPQ_BOND_PB;

		/*------------------------------------------*/
		/* Mutual funds: Same datatype as PQ_BONDS  */
		/* Note: sym[8] == 'T' and sym[4] == 'X'	*/
		/*------------------------------------------*/

typedef	struct _PQ_MUTUAL_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];				// Note: 'ABCDX  T' 
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		WORD	Close;				// Close
		BYTE	extra1;
		BYTE	extra2[2];			// Not Used
		WORD	Dividend;			// Dividend
		WORD	StockSplit;			// Stock Split
		WORD	NetAssetVal;		// Net Asset Valuation
		WORD	OfferPrice;			// Offering Price
		WORD	CapitalGains;		// Capital Gains
		BYTE	extra3[6];			// Not Used
		BYTE	Assets[3];			// Total Assets
		BYTE	AssetMult;			// Total Assets multiplier (exponent sld)
		char	Category;			// Category
		WORD	time;				// last update time/date
		BYTE	extra4;
		BYTE	decimal;			// decimal code
		BYTE	extra5[10];			// Not Used
		BYTE	reserved[12];
		}
		PQ_MUTUAL_PB, *LPPQ_MUTUAL_PB;

		/*-------------------------------------------------*/
		/* Money Market Funds: Same datatype as PQ_BONDS   */
		/* Note: sym[8] == 'T', sym[3] == sym[4] == 'T'	   */
		/*-------------------------------------------------*/

typedef	struct _PQ_MMFUND_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	extra1[3];
		WORD	Yield7Day;			//   7 Day  Yield
		WORD	Yield14Day;			//  14 Day  Yield
		WORD	Yield30Day;			//  30 Day  Yield
		WORD	AvgMaturity;		// Average Maturity
		WORD	Yield90Day;			//  90 Day  Yield
		WORD	Yield120Day;		// 120 Day  Yield
		WORD	Yield180Day;		// 180 Day  Yield
		WORD	Yield1Year;			//   1 Year Yield
		WORD	Yield7DayAnnual;	//   7 Day  Yield Annualized
		BYTE	NetAssets[3];		// Actual value in k$
		BYTE	extra3[2];			// Not Used
		WORD	time;				// last update time/date
		BYTE	extra4;				// Not Used
		BYTE	decimal;			// decimal code
		BYTE	extra5[10];			// Not Used
		BYTE	reserved[12];
		}
		PQ_MMFUND_PB, *LPPQ_MMFUND_PB;

	// Note: AvgMaturity & 7 Day yield swapped positon 10/8/93 per Steve

		/* PC Quote Futures Options */

typedef	struct _PQ_FUTOPT_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[4];				// Symbol
		char	PutCall;			// Put call indicator
		BYTE	strike[2];			// strike price backwards
		BYTE	exch;				// exchange
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		SHORT	close;				// offset from ysettle
		BYTE	extra1[2];			// Not used
		WORD    tosptr;				// tos pointer
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		BYTE	decimal;			// Decimal type 
		char	trbate;				// Trade bate
		SHORT	ticount;			// Trade count
		BYTE	displayPriceType;	// Display price type
		BYTE	bidsize;			// Bid Size
		BYTE	asksize;			// Ask Size
		BYTE	strikePriceType;	// price encoding for the strike price
		WORD	lastradate;			// Last trade date
		BYTE	pricrcode;			// Price Representation code
		BYTE	pricuncode;			// Price Unit Code
		BYTE	QtCond;				// Quote conditon
		WORD	timeTrade;			// time of last trade
		WORD	timeQuote;
		BYTE	reserved[8];
		}
		PQ_FUTOPT_PB, *LPPQ_FUTOPT_PB;

		/* PC Quote Futures or Index */

typedef	struct _PQ_FUTINDX_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open1;				// offset from ysettle
		SHORT	open2;				// offset from ysettle
		SHORT	close1;				// offset from ysettle
		SHORT	close2;				// offset from ysettle
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		BYTE	decimal;			// Decimal type 
		char	trbate;				// Trade bate
		SHORT	ticount;			// Trade count
		BYTE	displayPriceType;	// Display price type
		BYTE	bidsize;			// Bid Size;
		BYTE	asksize;			// Ask Size;
		WORD	timetrade;			// Time of last trade
		BYTE	QtCond;				// Quote Condtion
		BYTE	trendflag;			// 0 = unch,1=up,2=down
		SHORT	open;				// offset from ysettle
		WORD    expireDate;			// expiration date
		BYTE	reserved[10];
		}
		PQ_FUTINDX_PB, *LPPQ_FUTINDX_PB;

		/* PC Quote Stock */

typedef	struct	_PQ_STOCK_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		WORD	bidsize;			// Actual value
		WORD	asksize;			// Actual value
		WORD	lastsize;			// Actual value  20
		BYTE	volume[3];			// Actual value
		WORD	timelast;			// time of last trade
		WORD	timequote;
		BYTE	QtCond;				// Quote Condition
		BYTE	status;
		BYTE	volumeHi;			// The highest byte of volume DWORD
		BYTE	TrCond;				// Trade Condition
		BYTE	dectype;			// Decimal type (if not zero)
		BYTE	flags;				// Restricted/Notes see note* below
		union
		{
			BYTE	rgnlClose[3];	// Regional Close (NYSE and AMEX regionals)
			BYTE	closingBid[3];	// Closing Bid price (NASDAQ)
			struct					// market center of open/close/high/low/bid/ask
			{						//  (NYSE and AMEX composites)
				BYTE	gmclsop;
				BYTE	gmchilo;
				BYTE	gmcbidask;
			};
		};
		BYTE	trendflag;			// 0 = unch,1=up,2=down, alerts (S&P)
		union
		{
			WORD	tosptr;			// tick data offset in TIC.DAT
			WORD	tickCount;
		};
		BYTE	reserved[12];
		}
		PQ_STOCK_PB, *LPPQ_STOCK_PB;

// trendflag

#define TRENDFLG_UNCH			0x00
#define TRENDFLG_UP				0x01
#define TRENDFLG_DOWN			0x02
#define TRENDFLG_NEWS			0x04
#define TRENDFLG_ALERT_L		0x08
#define TRENDFLG_ALERT_H		0x10
#define TRENDFLG_ALERT_p		0x20
#define TRENDFLG_ALERT_P		0x40
#define TRENDFLG_ALERT_V		0x80

// flags
// bits: 0,1,2 Restrictions:
//
//		R/H				Lehman
//		0 = nothing		nothing
//		1 = 'Q'			'Q'  Securities Offering has been filed w/ SEC
//		2 = 'R'			'R1' For Restriction Due to Investment Banking
//		3 = 'E'			'E'  Employee 1 Day Restriction due to Research change opinion.
//		4 = 'P'			'S'	 Syndicate Penalty 
//		5 = '?'			'R2' Securities Offerings
//		6 = '?'			'R3' Affiliated Companies
//		7 = '?'			'R4' CEBA Restrictions
//
// bit: 3 Notes present
// bit: 4 Split
// bit: 5 X-Dividend
// bit: 6 1 = Bid up tick, 0 = Bid down tick

#define STKFLG_REST				0x07
#define STKFLG_NOTES			0x08

#define STKFLG_XDIV				0x10
#define STKFLG_SPLIT			0x20
#define STKFLG_BID_UP			0x40
#define STKFLG_UPC_RESTRICTED	0x80

		/* PC Quote Stock Options */

typedef	struct _PQ_STKOPT_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		SHORT	oconhigh;			// Contract High 
		SHORT	oconlow;			// Contract Low 
		BYTE	oundlsym[5];		// Underlying instr. Symbol 
		BYTE	oundlex;			// Underlying Exchange 
		BYTE	QtCond;				// Quote Condition
		WORD	tosptr;				// Time of sales ptr (TK only)
		BYTE    reserved[12];
		}
		PQ_STKOPT_PB, *LPPQ_STKOPT_PB;

		/*---------------------------------------------*/
		/* New style PC Quotes US Stock Options Block  */
		/* Note: DT_STKOPT_A and DT_STKOPT_N datatypes */
		/* 		 share this block layout.	 		   */
		/*---------------------------------------------*/

typedef	struct	_PQ_STKOPTAN_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		WORD    yestVolume;			// yesterday''s volume
		BYTE	reserved1[2];		// not used
		BYTE	QtCond;				// Quote Condition
		WORD	tosptr;				// Time of sales ptr (TK only)
		BYTE	reserved2[2];
		BYTE    oldDecType;			// old decimal type
		BYTE    reserved3;
		WORD    yestBid;			// yesterday''s bid
		WORD    yestAsk;			// yesterday''s ask
		BYTE	reserved4[4];
		}
		PQ_STKOPTAN_PB, *LPPQ_STKOPTAN_PB;

		/* PC Quote Foreign Futures Options */

typedef	struct _PQ_EUFUTOPT_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		SHORT	close;				// offset from ysettle
		BYTE	extra1[4];			// Not used
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		BYTE	decimal;			// Decimal type 
		char	trbate;				// Trade bate
		SHORT	ticount;			// Trade count
		BYTE	extra2;				// Not used
		BYTE	bidsize;			// Bid Size
		BYTE	asksize;			// Ask Size
		BYTE	extra3;				// Not used
		WORD	lastradate;			// Last trade date
		BYTE	pricrcode;			// Price Representation code
		BYTE	pricuncode;			// Price Unit Code
		BYTE	QtCond;				// Quote conditon
		BYTE	reserved[12];
		}
		PQ_EUFUTOPT_PB;

/*-------------------------------------------------------------------------*/

		/* Forex Price Block */

typedef	struct	_FOREX_PB
		{
		BYTE	updType;			// Can be R...
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	YSettle[3];
		BYTE	Last[3];
		BYTE	Bid[3];
		BYTE	Ask[3];
		BYTE	High[3];
		BYTE	Low[3];
		BYTE	Open[3];
		WORD	notused;
		BYTE	decimal;
		WORD	TimeTrade;
		WORD	ticount;
		char	Bank[4];			// Bank Code
		char	City[3];			// City Code
		BYTE	extra[3];
		WORD	tosptr;				// tick data offset in TIC.DAT
		BYTE	extra2[3];
		BYTE	flags;
		BYTE	reserved[8];
		}
		FOREX_PB, *LPFOREX_PB;

		/* Reuse FOREX_PB for MONEYRATE_PB */

#ifdef __cplusplus
typedef _FOREX_PB	_MONEYRATE_PB;
#endif
typedef FOREX_PB	MONEYRATE_PB;	
typedef	LPFOREX_PB	LPMONEYRATE_PB;

		/* Fixed Income Price Block */

typedef	struct	_FIXED_PB
		{
		BYTE	updType;			// Can be R...
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	country[2];
		BYTE	exchange;
		BYTE	YSettle[3];
		BYTE	Last[3];
		BYTE	Bid[3];
		BYTE	Ask[3];
		BYTE	High[3];
		BYTE	Low[3];
		BYTE	Open[3];
		WORD	Yield;
		BYTE	decimal;
		WORD	TimeTrade;
		WORD	ticount;
		LONG    askYield;
		LONG    bidYield;
		WORD    quoteTime;
		WORD	tosptr;				// tick data offset in TIC.DAT
		WORD    lastSize;
		BYTE	yieldPriceType;
		BYTE	flags;
		LONG    lastYield;
		BYTE	reserved[4];
		}
		FIXED_PB, *LPFIXED_PB;

/*-------------------------------------------------------------------------*/

		/* generic price block header */

typedef struct _HEADER_PB {
	BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
	BYTE	datatype;
	char	sym[8];
	WORD	dataptr;			// used to make qsdtid
} HEADER_PB;

		/* CORPORATE BONDS */

typedef	struct _BOND_PB {
	BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
	BYTE	datatype;
	char	sym[8];
	WORD	dataptr;			// used to make qsdtid
	BYTE	ysettle[3];
	SHORT	last;				// offset from ysettle
	SHORT	high;				// offset from ysettle
	SHORT	low;				// offset from ysettle
	SHORT	bid;				// offset from ysettle
	SHORT	ask;				// offset from ysettle
	SHORT	open;				// offset from ysettle
	WORD	bidsize;			// size of bid
	WORD	asksize;			// size of ask
	WORD	lastsize;			// size of last trade
	BYTE	volume[3];			// Actual value
	WORD	matdate;			// maturity date
	BYTE	bondrate[3];		// bond rate 
	BYTE	bondform;			// bond form 
	BYTE	trdunit[2];			// unit of trade 
	WORD	calldate;			// called date 
	BYTE	high52week[3];		// 52 week high price 
	BYTE	low52week[3];		// 52 week low price 
	BYTE	reserved[12];
} BOND_PB;

		/*---------------------------------------*/
		/* Mutual funds: Same datatype as BONDS  */
		/* Note: sym[8] == 'Q' and sym[4] == 'X' */
		/*---------------------------------------*/

typedef	struct	_MUTUAL_PB
	{
	BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
	BYTE	datatype;
	char	sym[8];				// Note: 'ABCDX  Q' 
	WORD	dataptr;			// used to make qsdtid
	WORD	Close;				// Close
	BYTE	extra1;
	BYTE	extra2[2];
	WORD	Dividend;			// Dividend
	WORD	StockSplit;			// Stock Split
	WORD	NetAssetVal;		// Net Asset Valuation
	WORD	OfferPrice;			// Offering Price
	WORD	CapitalGains;		// Capital Gains
	BYTE	extra3[6];			// Not Used
	BYTE	Assets[3];			// Total Assets
	BYTE	AssetMult;			// Total Assets multiplier (exponent sld)
	char	Category;			// Category
	BYTE	extra4[3];
	BYTE	decimal;			// decimal code
	BYTE	extra5[2];			// Not used
	BYTE	flags;				// MSB = CorpAction (S&P only)
	BYTE	extra6[7];			// Not Used
	BYTE	reserved[12];
} MUTUAL_PB, *LPMUTUAL_PB;

		/*----------------------------------------------*/
		/* Money Market Funds: Same datatype as BONDS   */
		/* Note: sym[8] == 'Q', sym[3] == sym[4] == 'X' */
		/*----------------------------------------------*/

typedef	struct	_MMFUND_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		BYTE	extra1[3];
		WORD	Yield7Day;			//   7 Day  Yield
		WORD	Yield14Day;			//  14 Day  Yield
		WORD	Yield30Day;			//  30 Day  Yield
		WORD	AvgMaturity;		// Average Maturity
		WORD	Yield90Day;			//  90 Day  Yield
		WORD	Yield120Day;		// 120 Day  Yield
		WORD	Yield180Day;		// 180 Day  Yield
		WORD	Yield1Year;			//   1 Year Yield
		WORD	Yield7DayAnnual;	//   7 Day  Yield Annualized
		BYTE	NetAssets[3];		// Actual value in k$
		BYTE	extra3[5];			// Not Used
		BYTE	decimal;			// decimal code
		BYTE	extra4[10];			// Not Used
		BYTE	reserved[12];
		}
		MMFUND_PB, *LPMMFUND_PB;

		/* Futures Options */

typedef	struct	_FUTOPT_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[4];				// Symbol
		char	PutCall;			// Put call indicator
		BYTE	strike[2];			// strike price backwards
		BYTE	exch;				// exchange
		WORD	dataptr;			// used to make qsdtid
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		SHORT	close;				// offset from ysettle
		BYTE	extra1[4];			// Not used
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		BYTE	decimal;			// Decimal type 
		char	trbate;				// Trade bate
		SHORT	ticount;			// Trade count
		BYTE	extra2;				// Not used
		BYTE	bidsize;			// Bid Size
		BYTE	asksize;			// Ask Size
		BYTE	extra3;				// Not used
		WORD	lastradate;			// Last trade date
		BYTE	pricrcode;			// Price Representation code
		BYTE	pricuncode;			// Price Unit Code
		BYTE	QtCond;				// Quote conditon
		BYTE	reserved[12];
		}
		FUTOPT_PB;

		/* Futures - Price Block */

typedef	struct	_FUTINDX_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		SHORT	open2;				// offset from ysettle
		SHORT	close1;				// offset from ysettle
		SHORT	close2;				// offset from ysettle
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		BYTE	decimal;			// Decimal type 
		char	trbate;				// Trade bate
		SHORT	ticount;			// Trade count
		BYTE	extra1;				// Not used
		BYTE	bidsize;			// Bid Size;
		BYTE	asksize;			// Ask Size;
		WORD	timetrade;			// Time of last trade
		BYTE	QtCond;				// Quote Condtion
		BYTE	trendflag;			// 0 = unch,1=up,2=down
		WORD	tosptr;				// Time of sales ptr (TK=not used)
		BYTE	reserved[12];
		}
		FUTINDX_PB;

		/* Stock - Price Block */

typedef	struct _STOCK_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		WORD	bidsize;			// Actual value
		WORD	asksize;			// Actual value
		WORD	lastsize;			// Actual value  20
		BYTE	volume[3];			// Actual value
		WORD	timelast;			// time of last trade
		WORD	timequote;
		BYTE	QtCond;				// Quote Condition
		BYTE	status;
		WORD	rgnlClose;			// Regional Close
		BYTE	dectype;			// Decimal type (if not zero)
		BYTE	flags;				// Restricted/Notes see note*
		BYTE	gmclsop;
		BYTE	gmchilo;
		BYTE	gmcbidask;			// global mkt center bid/ask
		BYTE	trendflag;			// 0 = unch,1=up,2=down, alerts (S&P)
		WORD	tosptr;				// tick data offset in TIC.DAT
		BYTE	extra[2];
		BYTE	volumeHi;			// The highest byte of volume DWORD
		BYTE	reserved[9];
		}
		STOCK_PB;

// 					Telesphere Version < 5.00
// trendflag
// bits: 0,1, Trend:
//
//		0 = unchanged
//		1 = up
//		2 = down
//
// bit: 2 News
//
// bit: 5 Split
// bit: 6 X-Dividend
// bit: 7 Bid Trend

#define TREND_UNCH			0x00
#define TREND_UP			0x01
#define TREND_DOWN			0x02
#define TREND_NEWS			0x04

#define TREND_XDIV			0x20
#define TREND_SPLIT			0x40
#define TREND_BID_UP		0x80

// flags
// bits: 0,1,2 Restrictions:
//
//		R/H				Lehman
//		0 = nothing		nothing
//		1 = 'Q'			'Q'  Securities Offering has been filed w/ SEC
//		2 = 'R'			'R1' For Restriction Due to Investment Banking
//		3 = 'E'			'E'  Employee 1 Day Restriction due to Research change opinion.
//		4 = 'P'			'S'	 Syndicate Penalty 
//		5 = '?'			'R2' Securities Offerings
//		6 = '?'			'R3' Affiliated Companies
//		7 = '?'			'R4' CEBA Restrictions
//
//  bit: 3 Notes present

		/* US STOCK OPTIONS */

		/*-----------------------------------*/
		/* New style US Stock Options Block  */
		/* Note: DT_STKOPT_A and DT_STKOPT_N */
		/* 		 share this block layout.	 */
		/*-----------------------------------*/

typedef	struct	_STKOPTAN_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		WORD	yestVol;			// Yesterday''s volume
		BYTE	extra1[2];			// not used
		BYTE	QtCond;				// Quote Condition
		WORD	tosptr;				// Time of sales ptr (TK only)
		BYTE	extra2[2];
		BYTE	oldDecimal;			// Decimal type for old stuff
		BYTE	extra3;
		WORD	oldBid;				// Previous day
		WORD	oldAsk;				// Previous day
		BYTE	reserved[4];
		}
		STKOPTAN_PB;

		/* European Stock */

typedef	struct	_EUROPE_PB
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		BYTE	ysettle[3];
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		WORD	bidsize;			// Actual value
		WORD	asksize;			// Actual value
		WORD	lastsize;			// Actual value  20
		BYTE	volume[3];			// Actual value
		WORD	timelast;			// time of last trade
		WORD	timequote;
		BYTE	QtCond;				// Quote Conditon
		BYTE	status;
		BYTE	extra1[2];			// Not used
		BYTE	dectype;			// Decimal type
		BYTE	extra2;				// Not used
		char	currency[3];		// Currency like 'USD' 
		BYTE	trendflag;			// 0 = unch,1=up,2=down
		WORD	tosptr;				// tick data offset in TIC.DAT
		BYTE	reserved[12];
		}
		EUROPE_PB;

/*-------------------------------------------------------------------------*/

		/* CrossMar Treasury Price Block */

typedef	struct	_CM_TREASURY_PB
		{
		BYTE	updType;			// Can be R...
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	term[3];
		BYTE	Bid[3];
		BYTE	Ask[3];
		BYTE	High[3];
		BYTE	Low[3];
		WORD	BidYield;			// Note: decimal type = 3
		WORD	AskYield;			// Note: decimal type = 3
		char	ID[2];
		char	IssueDate[5];
		BYTE	decimal;			// decimal of HLBA
		WORD	TimeTrade;
		WORD	ticount;
		char	Region[3];
		char	City[3];
		char	Contributor[4];
		WORD	tosptr;				// tick data offset in TIC.DAT
		BYTE	extra1;
		BYTE	Live;
		BYTE	Link[10];
		}
		CM_TREASURY_PB;

		/* CrossMar Forex,Money Market, and Metals */

typedef	struct	_CM_FOREXMMM_PB
		{
		BYTE	updType;			// Can be R...
		BYTE	datatype;
		char	sym[8];
		WORD	dataptr;			// used to make qsdtid
		char	term[3];
		BYTE	YSettle[3];
		BYTE	Last[3];
		BYTE	Bid[3];
		BYTE	Ask[3];
		BYTE	High[3];
		BYTE	Low[3];
		BYTE	Open[3];
		WORD	Yield;
		BYTE	decimal;			// decimal of YLBAHLO
		WORD	TimeTrade;
		WORD	ticount;
		char	Region[3];
		char	City[3];
		char	Contributor[4];
		WORD	tosptr;				// tick data offset in TIC.DAT
		BYTE	extra1;
		BYTE	Live;
		BYTE	Link[10];
		}
		CM_FOREXMMM_PB;

/*-------------------------------------------------------------------------*/

typedef	struct
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;			// Data type is DT_KRCASH or DT_KRFUTURE
		BYTE	sym[8];				// Symbol 
		WORD	dataptr;			// Data pointer
		BYTE	DecType;			// Decimal type
		BYTE	Exch;				// Exchange # (K.R.)
		WORD	TosPtr;				// Time of sales pointer
		BYTE	Flag;				// Misc. Flags
		BYTE	Bate;				// Bate code
		WORD	TimeTrade;			// Time of trade or fake trade
		BYTE	YClose[3];			// Yesterday''s close
		BYTE	Last[3];			// Last Price
		BYTE	Bid[3];				// Bid
		BYTE	Ask[3];				// Ask
		BYTE	High[3];			// High
		BYTE	Low[3];				// Low
		BYTE	Open[3];			// Open
		BYTE	Volume[3];			// Volume
		BYTE	OpenInt[3];			// Open Interest
		BYTE	Open2[3];			// Open (2nd value)
		BYTE	Close[3];			// Close
		BYTE	Close2[3];			// Close (2nd value)
		BYTE	Week1Bid[3];		// 1 Week Bid
		BYTE	Week1Ask[3];		// 1 Week Ask
		BYTE	Week2Bid[3];		// 2 Week Bid
		BYTE	Week2Ask[3];		// 2 Week Ask
		BYTE	Month1Bid[3];		// 1 Month Bid
		BYTE	Month1Ask[3];		// 1 Month Ask
		BYTE	Month2Bid[3];		// 2 Month Bid
		BYTE	Month2Ask[3];		// 2 Month Ask
		BYTE	Month3Bid[3];		// 3 Month Bid
		BYTE	Month3Ask[3];		// 3 Month Ask
		BYTE	Month6Bid[3];		// 6 Month Bid
		BYTE	Month6Ask[3];		// 6 Month Ask
		BYTE	Year1Bid[3];		// 1 Year Bid
		BYTE	Year1Ask[3];		// 1 Year Ask
		BYTE	YSettle[3];			// Yesterday Settlement (not used)
		BYTE	Name[32];			// Instrument name
		BYTE	Type;				// type of record (f=forex, e=not
		BYTE	Live;				// Live update byte
		BYTE	extra[5];			//   **** Not used ****
		}
		KR_LB;

typedef	struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_KRCASH or DT_KRFUTURE (40,41)
		char	sym[8];				// Part the symbol
		WORD	dataptr;			// data pointer
		KR_LB	*lpBlock;			// Memory holding actual block (KR_LB)
		}
		KR_EB;						// Knight-Ridder Extended block

//// Reuters 
typedef	struct
		{
		BYTE	value[3];			// 24 Bit signed value
		BYTE	dectype;			// Lower nibble is index
		}							// into standard dectable[]
		RSFV;

typedef	struct
		{
		char	Symbol[20];			// Symbol
		WORD	dataptr;			// QuoteServer index number
		BYTE	FieldList;			// Record sub-type
		BYTE	volumeHi;			// The highest byte of volume DWORD
		BYTE	Exch[3];			// Exchange # (RSF)
		BYTE	Flags;				// Flags see map below
		char	Bate;				// Bate code, see map below
		WORD	TimeTrade;			// Time of trade or fake trade
		RSFV	YSettle;			// Yesterday''s close
		RSFV	Last;				// Last price
		RSFV	Bid;				// Bid
		RSFV	Ask;				// Ask
		RSFV	High;				// High  (daily)
		RSFV	Low;				// Low   (daily)
		RSFV	Open;				// Open
		BYTE	Volume[3];			// Volume
		BYTE	OpenInt[3];			// Open Interest
		RSFV	Open2;				// Open 2  (split open)
		RSFV	Close;				// Close
		RSFV	Close2;				// Close 2 (split close)
		WORD	BidSize;			// Bid size
		WORD	AskSize;			// Ask size
		WORD	TosPtr;				// Time of Sales pointer
		char	InstName[32];		// ASCII Instrument name
		char	BackPage[4];		// ASCII Background info page
		char	unused1[3];			// ASCII *Obsolete* ConLocation[3]
		char	ConDeal[6];			// ASCII Contributor Deal
		RSFV	YearHigh;			// High for the Year
		RSFV	YearLow;			// Low  for the Year
		RSFV	LifeHigh;			// Life time high
		RSFV	LifeLow;			// Life time low
		char	Currency[3];		// ASCII Currency code
		BYTE	Live;				// Live update byte (ignore)
		char	DivYield[8];		// ASCII Dividend yield
		char	PERatio[8];			// ASCII P/E Ratio
		char	DivType[3];			// ASCII Dividend type
		char	DivPayDate[12];		// ASCII Dividend Pay date
		char	DivXDate[12];		// ASCII Dividend X Date
		char	Earnings[8];		// ASCII Earnings
		char	SplitDiv[12];		// ASCII Split dividend
		char	Capitalization[15];	// ASCII Capitalization
		char	OrigIssuePrice[12];	// ASCII Original Issue Price
		char	DivAmount[8];		// ASCII Dividend Amount
		char	StrikePrice[8];		// ASCII Strike price
		char	ExprDate[12];		// ASCII Expriation Date
		char	MatDate[12];		// ASCII Maturity Date
		char	CouponRate[8];		// ASCII Coupon Rate
		char	CouponDate[12];		// ASCII Coupon Date
		char	BondType[4];		// ASCII Bond Type
		char	Rating[4];			// ASCII Rating
		char	IssueDate[12];		// ASCII Issue Date
		char	CallDate[12];		// ASCII Call Date
		char	RatingID[4];		// ASCII Rating ID
		char	NewsPage[4];		// ASCII News page
		char	unused2[4];			// ASCII Background info page
		char	OfficialCode[12];	// ASCII CUSIP,VOlAR,SEDOL...
		char	OfficialCodeID[3];	// ASCII What Offical Code is
		char	LastTradeDate[12];	// ASCII Last Trade Date
		}
		RSF_LB;

		// Flags:			Bate  Codes
		// 0=trade		'b' = Bid	'q' = Quote (bid/ask)
		// 1=settle		'a' = Ask	't' = Trade (last price)
		// 2=nominal				'r' = refresh

typedef	struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_RSFQUOTE (50)
		char	sym[8];				// Part 1 of the symbol
		WORD	dataptr;			// data pointer
		RSF_LB	*lpBlock;			// Memory holding actual: RSF_LB
		}
		RSF_EB;						// RSF Extended block

// Reuters Synthetic Block(s)

typedef	struct
		{
		BYTE	updType;		// Usually 'R'
		BYTE	datatype;		// 52 (DT_RSFSYNTH)
		char	Symbol[8];
		WORD	dataptr;		// FAKE (dataptr)
		char	Page[4];		// Reuters "page name", example: WRLD
		BYTE	DecType;
		BYTE	PriceType;
		BYTE	Line;
		BYTE	ColStart;
		BYTE	Flags;			// Bit 0/1 = trend, bit 7 = BOGUS
		BYTE	ColEnd;
		BYTE	Last[3];		// 24 bit Intel-order signed numbers
		BYTE	Prev[3];		//		.
		BYTE	High[3];		//		.
		BYTE	Low[3];			//		.
		BYTE	Bid[3];			//		.
		BYTE	Ask[3];			//		.
		WORD	TicCount;
		WORD	TosPtr;
		char	Name[20];
		BYTE	Live;
		char	Key[12];		// Null term string if < 12 chars
		char	PageExt[6];		// Reuters "page name", extension
		}
		RSFSYNTH_LB;

typedef	struct
		{
		BYTE		updType;	// Update type
		BYTE		datatype;	// DT_RSFQUOTE (50)
		char		sym[8];		// Part 1 of the symbol
		WORD		dataptr;	// data pointer
		RSFSYNTH_LB	*lpBlock;	// Memory holding actual: RSFSYNTH_LB
		}
		RSFSYNTH_EB;				// RSFSYNTH Extended block


// JJ Kenny Drake Municipal Bond price blocks

typedef	struct
		{
		char	FracCode;			// Note: non-standard codes
		char	Value[6];			// Value
		}
		JJPRICE;

typedef	struct _JJMUNI_LB
		{
		BYTE	updType;			// Can be R.. or something else
		BYTE	datatype;			// Data type is DT_JJMUNIBOND
		BYTE	sym[8];				// Symbol 
		WORD	dataptr;			// Data pointer
		BYTE	Live;				// Live update byte
		char	ContribID[2];		// Contributor Identifier
		char	ItemType;			// Item Type: B-bid,O-offering,A-Active
		char	CUSIP[9];			// CUSIP including check digit
		char	Desc[150];			// Issuer and Issue information
		char	OfferComment[6];	// AMT or Escrow Indicator
		char	BidWantComment[16];	// Time Limit,status for bid wanteds
		JJPRICE	Coupon;				// Coupon Rate
		char	Bid[14];			// Bid   par value amt in $1k par mults.
		char	Offer[14];			// Offer par value amt in $1k par mults.
		char	PutFlag[2];			// Put & pre-refunded flag:	"MP","OP","PR"
		JJPRICE	DollarBid;			// Dollar bid for actives
		JJPRICE	BasisBid;			// Basis bid for actives
		char	BidConcSign;		// Bid Concession sign: +/-
		JJPRICE	BidConc;			// Bid Concession for actives
		JJPRICE	DollarOffer;		// Dollar Offer for offerings and actives
		JJPRICE	BasisOffer;			// Basis Offer for offerings and actives
		char	OfferConcSign;		// Offer Conc sign: +/-
		JJPRICE	OfferConc;			// Offer Conc for offerings and actives
		char	SP_Rating[4];		// S&P rating for the security
		char	MoodiesRating[4];	// Moodies rating for the security
		char	Insurer[7];			// Insurer name,if any
		char	State[2];			// State Code (IL,NY,etc)
		char	DatedDate[8];		// Dated Date,or official issue date
		char	Maturity[8];		// Maturity date
		char	FirstCall[8];		// Nearest call date
		char	SecondCall[8];		// Next Nearest call date
		char	FirstPut[8];		// Nearest put date
		JJPRICE	FirstCallPrice;		// Price of first call, per $1000 par
		JJPRICE	SecondCallPrice;	// Price of second call,per $1000 par
		JJPRICE	FirstPutPrice;		// Price of first put,  per $1000 par
		char	FirstCoupon[8];		// Date of first coupon
		char	InterestFrom[8];	// Interest computed from date
		char	CouponsPerYear[2];	// Number of interest payments per year
		char	SinkingFundStart[8];// Start of redemptions from sinking fund
		char	MandatorySink;		// "M" if mandatory sinker, else " "
		char	BondType;			// U-unknown,R-registered,
									// B-bearer, E-book entry
		char	DtcEligibility;		// U-unknown,E-eligible,N-not eligible
		char	MstcEligibility;	// U-unknown,E-eligible,N-not eligible
		char	CallOptions;		// A-Other calls may exist,
									// B-Original calls may exist
									// C-Calls may exist
									// D-No optional calls
									// E-Not callable
		char	CalculatorCode;		// Monroe Bond Calculator Code:
									//   0 -  30/360
									//   1 - act/act
									//   2 -  30/360
									//   3 - act/360
									//   4 - act/365
		char	SettleDate[8];		// Settlement date
		char	reserved[20];		//  **reserved
		char	PurposeType[4];		// "DB  " - Double Barrel
									// "GO  " - General Obligation
									// "REV " - Revenue 
		char	PurposeClass[4];	// "TAX " - Tax Revenue
									// "BLDG" - Building Revenue
									// "HOSP" - Hospital Revenue
		char	PurposeSubClass[4];	// "ALLC" - Tax Allocation
									// "FRAN" - Franchise Tax
									// "GEN " - Lease Renewal Revenue
									// "HLTH" - Health Care
									// "LIFE" - Life Care
		JJPRICE	CloseDollarBid;		// Yesterdays closing dollar bid
		JJPRICE	CloseBasisBid;		// Yesterdays closeing basis bid
		char	CloseConcSign;		// Concession sign for yesterdays bid
		JJPRICE	CloseBidConc;		// Concession for yesterdays closing bid
		JJPRICE	CloseDollarOffer;	// Yesterdays closing dollar offer
		JJPRICE	CloseBasisOffer;	// Yesterdays closing basis  offer
		char	CloseOfferConcSign;	// Conc sign for yesterdays close offer
		JJPRICE	CloseOfferConc;		// Conc for yesterdays closing offer
		char	CloseDate[8];		// Yesterdays Closing Date
		}
		JJMUNI_LB;

typedef	struct _JJBLUELIST_LB
		{
		BYTE	updType;			// Can be R.. or something else
		BYTE	datatype;			// Data type is DT_JJBLUELISTBOND
		BYTE	sym[8];				// Symbol 
		WORD	dataptr;			// Data pointer
		BYTE	Live;				// Live update byte.
		BYTE	TransType;			// Transaction type.
		BYTE	SecType[2];			// Security type (appendix F).
		char	Purpose[11];		// Purpose.
		BYTE	ItemID[6];			// Item ID.
		char	Advertiser[10];		// Advertiser.
		char	PhoneNumber[10];	// Phone Number.
		BYTE	MoodyRating[4];		// Moody rating.
		BYTE	SPRating[4];		// S&P rating.
		char	Insurer[7];			// Insurer name,if any
		char	CUSIP[9];			// CUSIP including check digit
		char	TimeRcvd[6];		// Time received.
		char	Desc[60];			// Issuer and Issue information
		JJPRICE	Coupon;				// Coupon Rate
		char	Maturity[8];		// Maturity date
		BYTE	StateCode[2];		// State Code.
		JJPRICE	PriceYield;			// Price Yield.
		BYTE	PriceType;			// Price type.
		BYTE	PriceFlag;			// Price flag.
		char	SizeOffer[7];		// Size of offer in thousands.
		BYTE	SizeFlag;			// Size flag.
		}
		JJBLUELIST_LB;

typedef	struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_JJMUNIBOND or DT_JJBLUELISTBOND
		char	sym[8];				// Symbol
		WORD	dataptr;			// data pointer
		JJMUNI_LB	*lpBlock;		// Memory holding actual block (JJMUNI_LB)
		}
		JJ_EB;						// JJ Kenny Drake Extended block

typedef	struct
		{
		BYTE	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;			// Data type - DT_GPXCUSIP or DT_GPXQUOTE
		WORD	dataptr;			// Data pointer
		BYTE	dectype;			// Decimal type
		BYTE	ysettle[3];			// Yesterday''s closing price
		BYTE	last[3];			// last price
		BYTE	bid[3];				// bid
		BYTE	ask[3];				// ask
		BYTE	high[3];			// high
		BYTE	low[3];				// low
		BYTE	open[3];			// open
		BYTE	lastsize[3];		// last size
		BYTE	bidsize[3];			// bid size
		BYTE	asksize[3];			// ask size
		DWORD	volume;				// volume
		WORD	timelast;			// time of last trade
		WORD	timebid;			// time of last bid
		WORD	timeask;			// time of last ask
		BYTE	couponRate[3];		// Coupon rate		(.9999)
		BYTE	lastYield[3];		// Last price yield	(.9999)
		BYTE	bidYield[3];		// Bid yield		(.9999)
		BYTE	askYield[3];		// Ask yield		(.9999)
		BYTE	indPrice[3];		// Indicative price
		BYTE	indYield[3];		// Indicative yield	(.9999)
		char	auctionDate[6];		// Auction date
		char	issueDate[6];		// Issue date
		char	maturDate[6];		// Maturity date
		char	CUSIP[11];			// CUSIP number
		char	Symbol[11];			// Symbol
		BYTE	exch;				// exchange
		char	alias[3];			// Alias code (3MO,1YR,etc)
		BYTE	actCode;			// Activity code
		BYTE	instrType;			// Instrument type
		BYTE	hitPrice[3];		// Hit work up price.
		BYTE	hitYield[3];		// Hit work up yield.	(.9999)
		BYTE	hitVolume[3];		// Hit work up volume.
		BYTE	takePrice[3];		// Take work up price.
		BYTE	takeYield[3];		// Take work up yield.	(.9999)
		BYTE	takeVolume[3];		// Take work up volume.
		WORD	TosPtr;				// TOS pointer.
		BYTE	live;				// Live byte.
		BYTE	filler[2];
		}
		GPX_LB;

typedef	struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_GPXCUSIP/DT_GPXQUOTE
		char	sym[8];				// Symbol
		WORD	dataptr;			// data pointer
		GPX_LB	*lpBlock;			// Memory holding actual block (GPX_LB)
		}
		GPX_EB;						// GovPx Extended block

typedef struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_FC_EARNINGS
		CHAR	sym[8];				// Symbol
		WORD	dataptr;			// data pointer
		CHAR    brokerID[2];		// broker ID
		BYTE    type;
		BYTE    extra;				// not used
	    BYTE    induCode1[6];       // industry code 1
	    BYTE    induCode2[6];       // industry code 2
		BYTE    fiscalYear[5];      // fiscal year
		BYTE    yrActual;           // flags
		BYTE    q1Actual;
		BYTE    q2Actual;
		BYTE    q3Actual;
		BYTE    q4Actual;
	    SHORT   yrEstEarnings;      // estimated annual earnings (all earnings are in 10^-2)
		SHORT   q1EstEarnings;      // quarter 1 estimated earnings
		SHORT   q2EstEarnings;      // quarter 2 estimated earnings
		SHORT   q3EstEarnings;      // quarter 3 estimated earnings
		SHORT   q4EstEarnings;      // quarter 4 estimated earnings
		WORD    estimateDate;       // date of estimate
		WORD    estimateTime;       // time of estimate
		SHORT   prevEarnings;       // previous year earnings
		SHORT   q1PrevEarnings;     // previous quarter 1 earnings
		SHORT   q2PrevEarnings;     // previous quarter 2 earnings
		SHORT   q3PrevEarnings;     // previous quarter 3 earnings
		SHORT   q4PrevEarnings;     // previous quarter 4 earnings
		WORD    prevEstimateDate;   // previous date of estimate
		}
		FCEARN_PB, *LPFCEARN_PB;

typedef struct
		{
		BYTE	updType;			// Update type
		BYTE	datatype;			// DT_FC_EARNINGS
		CHAR	sym[8];				// Symbol
		WORD	dataptr;			// data pointer
		CHAR    brokerID[2];		// broker ID ( this will be '  ' )
		BYTE    type;
		BYTE    extra;				// not used
		BYTE    fiscalYear[6];      // fiscal year
		BYTE    numBrokers;         // number of brokers
		BYTE    annualActual;       // actual or estimated
		BYTE    q1Actual;
		BYTE    q2Actual;
		BYTE    q3Actual;
		BYTE    q4Actual;
		SHORT   annualMean;         // annual mean
		SHORT   q1MeanEarnings;     // quarter 1 mean earnings
		SHORT   q2MeanEarnings;     // quarter 2 mean earnings
		SHORT   q3MeanEarnings;     // quarter 3 mean earnings
		SHORT   q4MeanEarnings;     // quarter 4 mean earnings
		SHORT   annualStdDev;       // annual standard deviation
		SHORT   q1AnnualStdDev;     // quarter 1 standard deviation
		SHORT   q2AnnualStdDev;     // quarter 2 standard deviation
		SHORT   q3AnnualStdDev;     // quarter 3 standard deviation
		SHORT   q4AnnualStdDev;     // quarter 4 standard deviation
	    WORD    meanDate;           // date
		SHORT   prevMean;           // previous year earnings
		SHORT   q1PrevMean;         // previous quarter 1 earnings
		SHORT   q2PrevMean;         // previous quarter 2 earnings
		SHORT   q3PrevMean;         // previous quarter 3 earnings
		SHORT   q4PrevMean;         // previous quarter 4 earnings
		WORD    prevMeanDate;       // previous date of mean
		}
		FCCONS_PB, *LPFCCONS_PB;

// Portfolio Price Block(s)

typedef	struct	_PORTFOLIO_PB
		{
		BYTE	updType;		// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;		// DT_PORTFOLIO
		char	Symbol[8];
		WORD	dataptr;		// Portfolio Index (dataptr)
		DWORD	Last;			// 32 bit Intel-order	- last
		DWORD	High;			//		.				- high
		DWORD	Low;			//		.				- low
		DWORD	Prev;			//		.				- close
		long	TotalPL;		//		.				- profit/loss (signed)
		BYTE	DecType;
		BYTE	Live;
		BYTE	flags;			// See note* below
		BYTE	trendflag;		// 0 = unch,1=up,2=down, alerts (S&P)
		char	Name[20];
		}
		PORTFOLIO_PB;

// flags

#define PORTFLG_NET_PL			0x80

		/* NASDAQ Level 2 QuoteBlocks */

#define MMAKERPB_TREND_UNCH	0
#define MMAKERPB_TREND_UP	1
#define MMAKERPB_TREND_DOWN	2

typedef	struct	_MMAKER_PB		// Data Types DT_SP_LEVEL2_A & DT_SP_LEVEL2_N
		{
		BYTE    updType;
		BYTE	datatype;		
		char	Symbol[5];		// Stock symbol
		char	MMid[3];		// Market Maker ID (3 of 4 characters)
		WORD	dataptr;
		char	MMid2;			// The fourth character of the MMid
		char    space;			// Always blank
		char	Exchange;		// 'Q'=nasdaq, 'S'=small cap
		BYTE	OriginMKT;		// Origin market
		BYTE	QuoteType;		// Quote Type
		BYTE	QuoteCondition;	// Quote Condition
		BYTE	BidPrice[3];	// Bid price in 64ths
		BYTE	AskPrice[3];	// Ask price in 64ths
		WORD	BidSize:14,		// Bid size
				BidTrend:2;		// Bid Trend - 0 = unch,1=up,2=down
		WORD	AskSize:14,		// Ask size
				AskTrend:2;		// Ask Trend - 0 = unch,1=up,2=down
		WORD	Time;			// Hours * 100 + minutes (high bit == NotToday!)
		BYTE	Live;			// Live bits, etc.
		}
		MMAKER_PB, *LPMMAKER_PB;

// Top ten and statistic blocks. Not in PB union due to size.

typedef struct _TT_KEY
		{
		char	symbol[17];
		char	country[2];
		char	exchange;
		}
		TT_KEY, *LPTT_KEY;

typedef struct _TOPTEN_PB
		{
		WORD    time;
		BYTE    resv[2];
		TT_KEY  symList[15];
		}
		TOPTEN_PB, *LPTOPTEN_PB;

typedef struct _STATISTICS_PB
		{
		DWORD  up;
		DWORD  down;
		DWORD  unchanged;
		BYTE   exponent;
		BYTE   resv;
		WORD   time;
		}
		STATISTICS_PB, *LPSTATISTICS_PB;

/*--------------- Blocks Union (local 597) ----------------------------*/

#define	PB_MINSIZE	(sizeof(BOND_PB)-12)
#define	PB_MAXSIZE	sizeof(KR_PB)

typedef	union
		{
		AT_HEADER_PB	atheader;
		AT_BOND_PB		atbond;
		AT_MUTUAL_PB	atmutual;
		AT_MMFUND_PB	atmmfund;
		AT_FUTURE_PB	atfuture;
		AT_FUTOPT_PB	atfutopt;
		AT_INDEX_PB		atindex;
		AT_STOCK_PB		atstock;
		AT_NASDAQ_PB	atnasdaq;
		AT_STKOPT_PB	atstkopt;
		AT_FOREX_PB		atforex;
		AT_FIXED_PB		atfixed;
		AT_MKTMKR_PB	atmktmkr;
		AT_NEWSINFO_NB	atnewsinfo;
		PQ_HEADER_PB	pqheader;
		PQ_BOND_PB		pqbond;
		PQ_MUTUAL_PB	pqmutual;
		PQ_MMFUND_PB	pqmmfund;
		PQ_FUTOPT_PB	pqfutopt;
		PQ_FUTINDX_PB	pqfutindx;
		PQ_STOCK_PB		pqstock;
		PQ_STKOPT_PB	pqstkopt;
		PQ_STKOPTAN_PB	pqstkoptan;
		PQ_EUFUTOPT_PB	pqeufutopt;
		FOREX_PB		forex;
		FIXED_PB		fixed;
		HEADER_PB		header;
		BOND_PB			bond;
		MUTUAL_PB		mutual;
		MMFUND_PB		mmfund;
		FUTOPT_PB		futopt;
		FUTINDX_PB		futindx;
		STOCK_PB		stock;
		STKOPTAN_PB		stkoptan;
		EUROPE_PB		europe;
		CM_FOREXMMM_PB	cmforexmmm;
		CM_TREASURY_PB	cmtreasury;
		KR_EB			kr;
		JJ_EB			jj;
		RSF_EB			rsf;
		RSFSYNTH_EB		rsfsynth;
		GPX_EB			govpx;
		FCEARN_PB       fcEarn;
		FCCONS_PB       fcCons;
		PORTFOLIO_PB	portfolio;
		MMAKER_PB       mmaker;
		}
		PB, *LPPB;

typedef	union
		{
		EXP_HEADER_PB	expheader;
		EXP_STOCK_PB	expstock;
		EXP_FUTURE_PB   expfuture;
		}
		EXP_PB, *LPEXP_PB;

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif

