 		/************  WARNING *********************** 
		THIS MATERIAL IS TRACK DATA COMPANY PRIVATE!
		IT MAY NOT BE DISTRIBUTED OR SHARED WITHOUT PERMISSION
		*************************************************/
#ifndef MYTRACK_H
#define MYTRACK_H

/*************************************************
* Note the following changes (as of version 2.41):
*	function prototypes have been removed and placed in a new file: myTrack_func.h
*		This file is implicitly included by mytrack.h
*	If your code defines: USING_MYTRACK_DLL, Mytrack.h will implicitly include mytrac_funcPt.h
**************************************************/


//		record types in the real time pricefile data base

//		0=stock 1=index 2=option  3=regional stock	4=market stats 
//		5=future 11=mutual fund


//	------------------------------------------------------------------------------


//		MYTRACK_MSG structure

//		Below are the structures and the overall union of the various messages
//		that will be sent back from the server in response to a request
//		or an update.

#if defined __cplusplus && ! defined SOLARIS_REV
extern "C" {
#endif

#define SDK_VERSION  417
	
/***********************************/
/* SDK Version IDS for Unix Flavors
/***********************************/
#if defined Linux
#define SDK_ID 6
#elif defined SOLARIS_REV
#define SDK_ID 7
#elif defined X86_REV
#define SDK_ID 8
#else // Windows
#define SDK_ID 5
#endif

#define	MSG_ERROR_REPORT		0
#define	MSG_LOGON_RESPONSE		1				// response to a mtRequestLogon 
#define MSG_LOGOFF_RESPONSE		2
#define MSG_HEART_BEAT			3				// will be handled internally by API
#define MSG_HISTORICAL_DATA		4
#define MSG_INTRADAY_UPDATE		6
#define MSG_BACKGROUND_RESPONSE	7
#define MSG_MARKET_PULSE		8
#define MSG_QUOTE_RESPONSE		9
#define MSG_NEWS_HEADLINE		10				// news headline message
#define MSG_NEWS_RESPONSE		11				// either news headlines or actual story
#define MSG_FROM_TRACK		14
#define MSG_DATA_WAREHOUSE      15
#define MSG_TRANSACTION_LOG     17
#define MSG_YESTERDAYS_LOGS		18
#define MSG_OPTION_DATA         20
#define MSG_CHAT                22
#define MSG_FUTURE_FAMILY_DATA  25
#define MSG_COMPOSITE_FAMILY_DATA	26
#define MSG_SYMBOL_LOOKUP	    27          // full name of ticker symbol
#define MSG_COMPOSITE_VALUE     28          // occasional response from intraday update request
#define MSG_INTRADAY_DATA       29
#define MSG_HISTORICAL_NEWS     30
#define MSG_TICKER_CLOSED		32			// No more intraday updates for symbol *** DEPRECATED ***
#define MSG_ZACKS_EARNINGS		34			// Zack's Earnings DB
#define MSG_NASDAQ_LEVEL_2      36
#define MSG_AUTO_REG_RESPONSE   39
#define MSG_NEWS_VENDORS		43			// news vendors specs.
#define MSG_OPTION_LINKS        53
#define MSG_EDGAR_ONLINE		55			// Edgar Online DB
#define MSG_VOLATILITY_DISPLAY	56
#define MSG_HEMINGTON_SCOTT		57			// Hemington Scott DB
#define MSG_ORDER_BOOK        61
#define MSG_MARKET_MAKERS		63			

//		brokerage related responses

#define MSG_BROKER_ACCT_IDS				      300				// returns acct id's
#define MSG_BROKER_ACCT_SUMMARY			    301				// returns an acct summary
#define MSG_BROKER_ACK_ORDER_BY_MYTRACK	302				// order ack'ed by myTrack
#define	MSG_BROKER_ACK_ORDER_BY_BROKER	303				// order ack'ed by broker
#define MSG_BROKER_ORDER_EXEC			      304				// order has executed
#define MSG_BROKER_ACCT_POSITION		    305				// returns a position
#define MSG_BROKER_ACK_CANCEL_BY_MYTRACK 306			// cancel ack'ed by myTrack
#define	MSG_BROKER_ACK_CANCEL_BY_BROKER	307				// cancel ack'ed by broker
#define	MSG_BROKER_REJ_CANCEL_BY_BROKER	308				// cancel rejected by broker
#define MSG_BROKER_ORDER_CANCELLED		  309				// order has been cancelled
#define MSG_BROKER_ERROR_MESSAGE		    310				// error message
#define MSG_BROKER_TRANSACTION          311             // Transaction data
#define MSG_BROKER_ORDER_DATA           312             // Record of orders
#define MSG_BROKER_CASH_TRANSACTION     313             // Record of Cash Transaction

// ----------- new broker messages (ver# 400) --------------------------------

#define MSG_BROKER_ACCT_SUMMARY_LINE    314		// acct summary - new line format

#define MSG_BROKER_TABLE_MESSAGE		315		// Generic BrokerTable message (transaction, orders, positions, acctSummary)

#define MSG_BROKER_MESSAGE              316     // just a message 


//	-----------------------------------------------------------------------------------

// MSG_ERROR_REPORT - server reported error condition

	typedef	struct {					
		char	error;					// see server side errors below
	} ERROR_REPORT;

//	-------------------------------------------------------------------------------------

// MSG_LOGON_RESPONSE - response from mtRequestLogon

	typedef	struct {					
		char	error;					// 0=ok, otherwise see server side errors below
		char	spare1[5];
		char	host[8];				// myTrack host name				
		char	features[16];			// to be defined
		char	spare[200];
	} LOGON_RESPONSE;


//	-------------------------------------------------------------------------------------

// AUTO_REG_RESPONSE - response from 39

	typedef	struct {					
		char	error;					// 0=ok, otherwise see server side errors below
		char	username[16];
		char	password[16];
		char	errortext[80];
	} AUTO_REG_RESPONSE;

//	-----------------------------------------------------------------------------------

// MSG_LOGOFF_RESPONSE - response from mtRequestLogoff

	typedef	struct {					
		char	error;					// 0=ok, otherwise see server side errors below
		char	spare1;
	} LOGOFF_RESPONSE;

//	-----------------------------------------------------------------------------------

// MSG_HEART_BEAT

	typedef	struct 
  {					
		short	numClientMsgsLost;
		char	timeOfDay[3];
		char	spare[1];		
		short	numMsgsDropped;
	} HEART_BEAT;

//	-----------------------------------------------------------------------------------


// MSG_HISTORICAL_DATA - response from mtRequestHistoricalData

#define MAX_HISTORICAL_PKTS_PER_SEND 50

//		The server will send back multiple data sub-packets one for each period

//		below is the sub-packet not all fields are valid for all record types - see below

	typedef struct {					
		short	date[3];
		UCHAR	spare;
		UCHAR	format;					// see below
		long	open;
		long	high;
		long	low;
		long	close;
		long	volume;
		long	misc;					// see below
	} HIS_PACKET;
				

	
	typedef	struct {					
		char	error;					
		char	recordType;				// 0=stock 1=index 2=option4=mkt stat 
										// 5=future 11=mutual fund

		char	packetFlag;				// 0=more packets to follow
										// 1=last transmission for this request
										// 2=no more data for this ticker

		UCHAR	numPackets;				// returned
		int		generalSplitFactor;		// see below
		HIS_PACKET	pkt[MAX_HISTORICAL_PKTS_PER_SEND];
	} HISTORICAL_DATA;

/*		the # of packets sent back does not always correspond to the 
		# requested.  the server may send back less than requested.
		be guided by the packet flag and re-issue another request if
		needed.  max # of packets sent by the server at one time is 50



		daily_split_factor: is in 10000, divide by 10000 to get real factor
		i.e. 15000 is 1.5
		the actual value is (after factoring out the 10000):
		value = (value * general_split_factor) / daily_split_factor

		 see the utility routine mtuGetSplitAdjustedPrice in mytrack_func.h



		format: the denominator you divide the value by to get the real price.
		if (format < 10)
			denominator = pow (10, format);		// 10 ** format
		else
			denominator = 1 << (format-10);		//  2 ** (format - 10)

		example:  value = 40160    format = 18   -> real value = 156.875
			value = 663010   format = 2    -> real value = 6630.10

		see the utility routine mtuGetPrice in mytrack_func.h



		date:   year month day

		----------------------------------------------------------------------------

		the following are the valid fields per type of record

		type		date	format	open	high	low		close	volume		misc
		----		-----	------	-----	-----	----	------	-------		------

  0		stock		Yes		Yes		Yes		Yes		Yes		Yes		Yes			daily split factor
  1		index		Yes		Yes		Yes		Yes		Yes		Yes		No			No
  2		option		Yes		Yes		Yes		Yes		Yes		Yes		Yes			No
  4		mkt stat	Yes		Yes		Yes		Yes		Yes		Yes		Yes			No
  5		future		Yes		Yes		Yes		Yes		Yes		Yes		Yes			open interest
  11	mutual fund	Yes		Yes		NAV		Offer



*/
/*		record types in the historical data base


	char * HDB_type_desc[19]

     =											//  symbol type descriptions     
{												//*******************************
	"stock",                                    // 0 
	"index",                                    // 1 
	"option",                                   // 2 
	"regional",									// 3 
	"market statistic",                         // 4 
	"future",                                   // 5 
	"future option",							// 6
	"money rates",								// 7
	"misc option",								// 8
	"bond",                                     // 9 
	"S&P",                                      // 10
	"mutual fund",								// 11
	"money market fund",						// 12
	"technical data",                           // 13
	"volatility data",							// 14
	"otc bond data",                            // 15
	"dividend",									// 16
	"eps and sos",								// 17
	"splits",									// 18
} ;										   
 */
//	-----------------------------------------------------------------------------------

// MSG_QUOTE_RESPONSE - response from mtRequestQuoteData

	//		There are two types of pkts returned:  options and everything else.
	//		Non-options are returned in the stock structure.

	typedef	struct {					
		char	error;					
		UCHAR	recordType;
		UCHAR	rtFlag;					// 0=real time   1=delayed
		char	spare[2];
		UCHAR	format;
		UCHAR	tradeHour;
		UCHAR	tradeMinute;
		int		last;
		int		yestClose;
		int		volume;
		int		bid;
		int		ask;
		int		bidSize;
		int		askSize;
		int		high;
		int		low;
		int		open;
		int		lastTradeVolume;
		int		annualHigh;
		int		annualLow;
		UCHAR	tick;
		char	exchange[2];			// mkt letter (i.e. N for new york) up to 2 letters
		UCHAR	volatility;				// a number between 0 and 150
		UCHAR	divFrequency;
		UCHAR	divDate[3];
		int		divAmt;
		int		eps;
		short	blockFactor;
		short	shortableQuantity;
		int		implied;
		int		yestVolume;
		short	newsValue;
		short	spare3;
		int		compositeVolume;
		int		numTrades;
		int		afterMktLast;
		UCHAR	afterMktHour;
		UCHAR	afterMktMinute;
	} QUOTE_RESPONSE_STOCK;

	typedef	struct {					
		char	error;					
		UCHAR	recordType;
		UCHAR	rtFlag;					// 0=real time   1=delayed
		char	spare[2];
		UCHAR	format;
		UCHAR	tradeHour;
		UCHAR	tradeMinute;
		int		last;
		int		yestClose;
		int		volume;
		int		bid;
		int		ask;
		int		high;
		int		low;
		int		openInterest;
		char	exchangeCode[2];
		char	optionDesignation[2];
		int		strikePrice;			// use format code to interpert this
		short	daysToExpiration;
		UCHAR	seriesNumber;
		UCHAR	numDividends;
		UCHAR	callPutFlag;			// 0=call 1=put
		char	spare1[3];
		int		yesterdayVolume;
		char	exchangeRoot;
		int		open;
		int		numTrades;
	} QUOTE_RESPONSE_OPTION;


	typedef	struct {							// the union of stock & option pkts					
		union {
			QUOTE_RESPONSE_STOCK	stock;
			QUOTE_RESPONSE_OPTION	option;
		} u;
	} QUOTE_RESPONSE;


//	-----------------------------------------------------------------------------------

// MSG_BACKGROUND_RESPONSE - response from mtRequestBackgroundData


	typedef	struct {					
		char	error;					
		UCHAR	recordType;
		char	description[32];		// name of company
		char	cusip[12];				// standard industry id for this security
		char	industryGroup[6];
		char	startDate[3];			// history start date
		char	endDate[3];				// history end date	(will be 0 0 0 for active stocks)
		UCHAR	optionFlag;				// 0=no options, 1=has options
		char	primaryMarketCode;
		int		sharesOutstanding;		// in thousands
		int		sharesInFloat;			// in thousands
		int		averageDailyVolume;		// in units
		int		shortInterest;			// in units
		char	underlyerSymbol[8] ;	// option underlier ticker symbol
      BYTE  lotSize;             // usually 100 can be 10 or 1
	} BACKGROUND_RESPONSE;


//		if you are using the end date for a subsequent mtRequestHistoricalData
//		and it is zero - set the end date
//		to be the current date or tomorrow's date.

//	-----------------------------------------------------------------------------------

// MSG_INTRADAY_UPDATE - response from mtRequestIntradayUpdate


	typedef struct {					// intraday update - sale message
		char	time[3];
		char	format;
		long	seq;					// unique seq # of this trade
		long	value;
		long	volume;
		char	saleCode;				// a code defining the type of sale
	} SALE_UPDATE;
	
	typedef struct {					// intraday update - quote message					
		char	time[3];
		char	format;
		long	seq;					// unique seq # of this quote
		long	bid;
		long	ask;
		USHORT	bidSize;
		USHORT	askSize;
	} QUOTE_UPDATE;
	
	typedef	struct {					
		UCHAR	error;					
		UCHAR	recordType;
		char	flag;					// see below
		char	msgType;				// 0=trade  1=quote
		union	{							
			SALE_UPDATE		sale;
			QUOTE_UPDATE	quote;
		} m;
	} INTRADAY_UPDATE;


/*		the flag will denote the type of data.  there are 3 possibilities:
		0 is for a regular msg, 1 is for a cancellation of a previous msg.
		the msg to be cancelled is the one with the same sequence # that is in
		this msg.  2 is for a correction - replace the msg with the seq #
		that is sent with this msg with the data in this msg (new volume & last)

		not all fields are defined for each record type:
		the following are the valid fields per type of record

		for Sale (a trade):

		type		time	format	seq		value	volume	saleCode
		----		-----	------	-----	-----	----	--------

  0		stock		Yes		Yes		Yes		Yes		Yes		Yes		
  1		index		Yes		Yes		Yes		Yes		No		No	
  2		option		Yes		Yes		Yes		Yes		Yes		No	
  3		reg stock	Yes		Yes		Yes		Yes		Yes		Yes	
  4		mkt stat	Yes		Yes		Yes		Yes		No		No	
  5		future		Yes		Yes		Yes		Yes		Yes		No
  

		for Quotes (a bid/ask quote):

		type		time	format	seq		bid		ask		bid size	ask size
		----		-----	------	-----	-----	----	--------	--------

  0		stock		Yes		Yes		Yes		Yes		Yes		Yes			Yes	
  1		index		Yes		Yes		Yes		Yes		No		No			No	
  2		option		Yes		Yes		Yes		Yes		Yes		No			No	
  3		reg stock	Yes		Yes		Yes		Yes		Yes		Yes			Yes	
  4		mkt stat	n/a
  5		future		Yes		Yes		Yes		Yes		Yes		No			No
  

	time = <hour> <minute> <second>
	saleCode = trade code - usually a blank  - see file stockCodes.stcd
	
*/

//	-----------------------------------------------------------------------------------

// MSG_NEWS_HEADLINE - a new  - news headline just arrived

	typedef	struct {					
		UCHAR	length;					// length of headline
		char	headline[160];
	} NEWS_HEADLINE;

/*		presently only up to 80 characters will be sent - but allow for a double headline
		of 160 characters sometimes down the road.

		The headline will have the following format:
		story #	  vendorCode  time    text
				i.e.:  01235  R   6.06 	Judge rules against Microsoft
		where:
		01235   is the unique story # - use this # to retrieve the story
		R		is the vendor code (Reuters)  - see below MSG_NEWS_VENDORS
		6.06    is the time - 6:06 am     6:06 would be 6:06 pm
*/
//	-----------------------------------------------------------------------------------

// MSG_NEWS_RESPONSE - a response to a request for headlines for a ticker or vendor
//						and also for a specific story

	typedef	struct {					
		UCHAR	error;
		UCHAR	flag;			// 0=more to come  1=last msg
		UCHAR	length;			// # of bytes in line
		char	line[160];
	} NEWS_RESPONSE;

//		headlines and stories are sent 1 line at a time.

//	-----------------------------------------------------------------------------------

// MSG_FROM_TRACK

	typedef	struct {					
		UCHAR	flag;			// 1=more to come  2=last msg
		UCHAR	length;			// # of bytes in line
		char	line[160];
	} FROM_TRACK;

//		headlines and stories are sent 1 line at a time.

//	-----------------------------------------------------------------------------------

// MSG_NEWS_VENDORS - response from mtRequestNewsVendors

	typedef	struct {					
		char	flag;				// 0=more to come  1=last one 				
		char	enabled;			// 0=you are not enabled   1=you are enabled
		char	vendorCode[3];		// 1 to 3 chars - vendor code 
		char	vendorSubCode[2];	// 1 ro 2 chars - vendor sub code (see myTrack.txt)
		char	vendorName[14];		// use this name in your display
	} NEWS_VENDORS;

//	-----------------------------------------------------------------------------------

// MSG_SYMBOL_LOOKUP - response from mtRequestSymbolLookup()
	typedef struct {
		char error;
		char type;
		char eom_flag;
		char nob;        // # of bytes in ticker
		char ticker[80]; // Full Name 
	} SYMBOL_LOOKUP;

//	-----------------------------------------------------------------------------------

// MSG_OPTION_DATA - response from mtRequestOptionData()
// Version 2.1 - exchangeRoot field changed from int to char[4]
	typedef struct {
		char  error;
		char  recordType;
		char  rtFlag;
		char  eomFlag;
		char  spare;
		UCHAR	format;
		UCHAR	tradeHour;
		UCHAR	tradeMinute;
		int last;
		int yest_close; // yesterday's close
		int   volume;
		int bid;
		int ask;
		int high;
		int low;
		int open_interest;
		char  exchange[2];
		char  designation[2];
		int strikePrice; // use format code to interpret this
		short daysToExp;   // days to expiration
		char  series_num;
		char  numDivPayments;
		char  callOrPut;   // 0=call 1 =put
		char  spare1[2];
		int   yest_volume;
		char  exchangeRoot[4];
		int open;
		int   numTrades;
	} OPTION_PACKET;

//	-----------------------------------------------------------------------------------

// MSG_CHAT - response from mtRequestSymbolLookup()
	typedef struct {
		char  error;        // 0 = ok
		char  type;         // 0=public window 1=private window
		short nob;          // # of bytes in text
		char  text[120];    // maximum text = 120 bytes
	} CHAT;

//	-----------------------------------------------------------------------------------

// MSG_OPTION_LINKS - response from mtRequestOptionLinks()
	typedef struct {
		char error;
		char num;	// # of tickers ( max 20 )
		char ticker[20][4];
	} OPTION_LINKS;

//	-----------------------------------------------------------------------------------

// MSG_FUTURE_FAMILY_DATA - response from mtRequestFutureFamilyData()
	typedef struct {
		char error;
		char recordType; 
		char rtFlag;  // 0=real time 1 = delayed
		char eomFlag; // 0 = more packets will be sent 1=last packet
		UCHAR	format;
		UCHAR	tradeHour;
		UCHAR	tradeMinute;
		int  last;
		int	 yestClose; // yesterday's close
		int  volume;
		int  bid;
		int  ask;
		int  high;
		int  low;
		int  openHi;
		int  openLo;
		int  annualHi;
		int  annualLo;
		char tick;      //>0 uptick, <0 downtick
		char exchange[2];
		BYTE volatility; // between 0 and 150
		char firstDeliveryDate[3];
		char lastDeliveryDate[3];
		char lastTradeDate[3];
		char hasOptions; // 0=no 1 = yes
		char spare;
		int  impliedPrice;
		int  yestVolume;
		int  openInterest;
		char ticker[6];
	} FUTURE_FAMILY_DATA;

// MSG_INTRADAY_DATA - response from mtRequestIntradayData()
	typedef struct {
		char error;
		char recordType;
		char spare[2];
		char date[3];
		char packetFlag;
		UCHAR	Hour;
		UCHAR	Minute;
		UCHAR   Second;
		UCHAR	format;
		int     endSeq;
		int		open;
		int		high;
		int		low;
		int		close;
		int		volume;
	} INTRADAY_DATA;


//	-----------------------------------------------------------------------------------
// New Response Structures for version 2.1
    //MSG_COMPOSITE_VALUE - occasional response from mtRequestIntradayUpdate() (intraday update request)
	typedef struct {
		int value;
    } COMPOSITE_VALUE;

// MSG_DATA_WAREHOUSE - response from mtRequestDataWarehouse()
	typedef struct {	
		char error;
		char actionType; // request type
		short key;       // request id  
		char data[100];  // only valid if actionType = 2
	} DATA_WAREHOUSE;

// MSG_TRANSACTION_LOG - response from mtRequestTransactionLog()
	typedef struct {
		char error;
		char flag;    // 0=more lines to follow, 1 = last line
		char spare;
		char nob;     // number of bytes in line
		char line[160]; // Data - fractions need to be converted see ConvertFractions() at end of file!!
	} TRANSACTION_LOG;

// MSG_HISTORICAL_NEWS - responses from mtRequestHistoricalNews_Story() 
// 	                     and mtRequestHistoricalNews_Ticker()
	typedef struct {
		char error;
		char flag;    // 0=more lines to follow, 1 = last line
		char nob;     // number of bytes in line
		char line[160];
	} HISTORICAL_NEWS;

 // the data will be sent back in lines. each line may have
 // up to 160 bytes of text but presently will not exceed 80 bytes.
 // stories and headlines will be sent one line at a time.


// MSG_NASDAQ_LEVEL_2 - response from mtRequestNasdaqLevel_2()
	typedef struct {
		char     stMmid[4];      // market maker id
		BYTE     bAction; // 1=valid mm record  2=delete this mm record
		char     cQc;     // quote condition 'C' = closed
		char     cPriceFormat;  // 0: bid is in 256ths,  ask is in 256ths
                       // 1: bid is in 256ths,  ask is in 1000ds
                       // 2: bid is in 1000ds,  ask is in 256ths
                       // 3: bid is in 1000ds,  ask is in 1000ds
                       // 4: bid is in 256ths,  ask is in 10,000ds
                       // 5: bid is in 10000ds,  ask is in 256ths
                       // 6: bid is in 10000ds,  ask is in 10000ds
		char     cTrend;     // trend indicator
                       //  bit 0x01 for bid: off = bid down  on = bid up
                       //  bit 0x02 for ask: off = ask down  on = ask up
		long     lMmbid;     // bid price
		long     lMmbidsz;   // bid size
		long     lMmask;     // ask price
		long     lMmasksz;   // ask size
		long     lTime;      // time of quote (sfm)
		USHORT   sNumBid;    // # of times at the inside bid
		USHORT   sNumAsk;    // # of times at the inside ask  
	} NQDSDATA;

	typedef struct {
		char     error;
		char     flag;   // 0=more data to follow, 1 = last data transmission
		char     nop;    // number of packets in this msg
		char     spare;
		NQDSDATA packet; // list of nop NQDSDATA packets
	} NASDAQ_LEVEL_2;
//	-----------------------------------------------------------------------------------

//	-----------------------------------------------------------------------------------
// New Response Structures for version 2.2
	typedef struct {
		char error;
		char space[3];
		char data[24][80]; // 24 by 80 page data
	} MARKET_PULSE;

	typedef struct {
		char spare[4];
	} TICKER_CLOSED;  // For MSG_TICKER_CLOSED *** DEPRECATED ***



//	-----------------------------------------------------------------------------------
// New Response Structures for version 2.3
	typedef struct {
		char error;
		char flag;  // 0 - more pages to follow, 1 - last page
		short nob;  // # of bytes in page
		char data[24][80]; // 24 by 80 page data
	} ZACKS_EARNINGS; // for MSG_ZACKS_EARNINGS

	
	typedef struct {
		char  error;
		char  eomflag;  // 1=more to come 2=last data
		char  spare[2];
		char  dateText[8];
		char  headline[96];
		char  url[128]; 
	} EDGAR_ONLINE; // for MSG_EDGAR_ONLINE

	typedef struct {
		char  error;
		char  spare;
		char  flag;  // 0=more text to follow, 1=this is the last line
		char  nob;
		char  text[80]; 
	} HEMINGTON_SCOTT; // for MSG_HEMINGTON_SCOTT

	typedef struct {
		char error;
		char space[3];
		char data[24][80]; // 24 by 80 page data
	} VOLATILITY_DISPLAY;


	typedef struct {
		char	error;
		char	recordType; 
		char	rtFlag;     // 0=real time ticker 1=delayed ticker
		char	eomflag;    // 0=more packets will be sent 1=last packet
		char	spare;
		UCHAR	format;
		UCHAR	tradeHour;
		UCHAR	tradeMinute;
		int		last;
		int		yestClose;
		int		volume;
		int		bid;
		int		ask;
		int		bidSize;
		int		askSize;
		int		high;
		int		low;
		int		open;
		int		lastTradeVolume;
		int		annualHigh;
		int		annualLow;
		UCHAR	tick;
		char	exchange[2];			// mkt letter (i.e. N for new york) up to 2 letters
		UCHAR	volatility;				// a number between 0 and 150
		UCHAR	divFrequency;
		UCHAR	divDate[3];
		int		divAmt;
		int		eps;
		short	blockFactor;
		short	shortableQuantity;
		int		implied;
		int		yestVolume;
		short	newsValue;
		short	spare3;
		int		compositeVolume;
		int		numTrades;
		int		afterMktLast;
		UCHAR	afterMktHour;
		UCHAR	afterMktMinute;
		short   spare4;
		int		afterMktVolume;
	} COMPOSITE_FAMILY_DATA;

	typedef struct {
		char error;
		char flag;
		char spare;
		char nob;
		char line[80];
	} YESTERDAYS_LOGS;

/////////////////////////////////////////////////////////////////

//		myTrack STATS structure (used by mtGetStats() )

	typedef struct
  {
		int		numMsgsReceived;				// # of msgs received

		int		maxInputMsgsOnQueue;    // deprecated

		int		inputQueueCurrent;        // current # of input msgs
		int		inputQueueHigh;           // high water mark of input msgs
		int		inputQueueMaxCapacity;    // max # of input msgs allowed

		int		numInputFaults;					// # of msgs dropped cause there was no
												            // room to store them on queue.

    int   numMsgsDroppedByHost;     // # of messages dropped by TDS

	} STATS_PKT;


//	-----------------------------------------------------------------------------------
// New Response Structures for version 2.4
   typedef struct {
      char error;
      char flag;        // which market is being sent same as request
      char eod;         // end of data: 1 more to come, 2 last entry
      char code;      // value to set to corrosponding field when placing order to use this mm
      char name[20];    // name of market maker
      short fee;        // 10's of cents i.e. 7 = $.007
   } MARKET_MAKER;

// MSG_ORDER_BOOK - response from mtRequestOrderBook()
	typedef struct {
		long     orderNum;      // order number, vendor specific (key)
      char     stMmid[4];     // market maker id (key)
		BYTE     bAction;       // 1=valid mm record  2=delete this mm record
		char     cSide;         // 'B' = buy, 'S' = sell
		BYTE     cPriceFormat;  

		char     spare;     
		long     price;      // price
		long     size;       // size
		long     lTime;      // time of quote (sfm)
	} OB_DATA;

	typedef struct {
		char     error;
		char     flag;   // 0=more data to follow, 1 = last data transmission
		char     nop;    // number of packets in this msg
		char     spare;
		OB_DATA  packet; // list of nop OB_DATA packets
	} ORDER_BOOK;
///	-----------------------------------------------------------------------------------
//		BROKERAGE RELATED REQUESTS
//		===========================


//	STRUCTURES
//	===========


//	below is the order entry structure for submitting an order

typedef struct {
								// below are fields needed at order entry time

	char	turnaround[4];		// use to identify order, returned in order
	char	acct[16];			// account id
	char	ticker[10];			// ticker - i.e. "IBM", for options "IBM AT"
	char	actionCode;			// 1=buy  2=sell  3=buy to cover 4=sell short
	char	instrumentType;		// 1=stock   2=option
	char	fillKill;			// 1=fill or kill
	char	AllNone;			// 1=all or none
	char	orderType;			// 1=market   2=limit   3=stop limit  4=stop market
	char	dayGTC;				// 0=day order 1=good till cancel
	char	MarketMaker[4];		// if prefrencing a specific mkt maker
	int	orderQty;			// # of shares or # of option contracts
	float	limitPrice;			// limit price, if market order set it to 0
	float	stopPrice;			// if stop order
	char	spare1[12];			// set to nulls
               
   // fields for routing
	USHORT	ecnMap;			
	USHORT	excludeMap;			
	char	mmListed;
	char	mmOption;
	char	editOverride;
	char	origin;
	char	optionMarket;
	char	condInd;
	char	mmNasdaq;			// mkt maker for Nasd 0=system default 1=herzog 2=knight
	char	mmOtc;				// mkt maker for Otc BB 0=system default 1=herzog 2=knight

								// below are fields that are returned by the broker
	char	orderId[16];		// unique order id for this order
	short	orderDate[3];		// order date month day year
	short	spare2;
	int	orderTime;			// 10ths of seconds from midnight
	int	filledQty;			// quantity executed (will increase as order executes)
	char	spare3[12];			// will be set to nulls

} BROKER_ORDER;


// Structure for response to order status requests
typedef struct {
	char		turnaround[4];
	char		acct[16];
	char		ticker[10];		
	BYTE		actionCode;					// see action code defines
	BYTE		instType;					// see instType defines above
	char		orderID[16];			
 	short		orderDate[3];		
 	short		closedDate[3];	
 	short		settleDate[3];	
	int		orderTime;			        // milliseconds past midnight 
	int		closedTime;			        // milliseconds past midnight 
	int		orderQty;	
	int		filledQty;	
	float		limitPrice;	
	float		stopPrice;	
	float		execPrice;					// execution price
	BYTE		fillKill;					// 1 = FillOrKill else = 0 
	BYTE		AllNone;					// 1 = AllOrNone else = 0 
	BYTE		dayGTC;						// 0 = day order, 1 = GTC  
	BYTE		orderType;					// see order types
	UCHAR		orderStatus;
	BYTE		bRejectionCode;
	float       commissions;
	float       fees;
	float       price;
	char		brokerConfirm[16];
} BROKER_ORDER_RECORD;

//	below is the transaction packet

typedef struct {
	char	orderId[16];			
	char	acct[16];
	char	instrumentType;		// 1=stock   2=option
	char	transType;			// see below
	char	ticker[10];		
	short	transDate[3];
	short	spare1;
 	int		transTime;		
	int		qty;	
	float	tradePrice;	
	float	commissions;	
	float	fees;	
	char	actionCode;			// 1=buy  2=sell  3=buy to cover 4=sell short
	char	spare2[23];
 } BROKER_TRANSACTION;

 typedef struct {				// cash transactions
		char	acct[16];
		short	date[3];
		double	amount;
		char	description[60];
 } BROKER_CASH_TRANSACTION;

//	transaction types

#define		ACKD_BY_BKR				1			  
#define		EXECUTED_BY_BKR			2		 
#define		CANCELLED_BY_USER		3 
#define		REJECTED_BY_BKR			4 
#define		CANCELLED_BY_BKR		5
#define		CREDIT_CASH				6 
#define		DEBIT_CASH				7
#define		REJECTED_BY_TDC			8

#define		CANCEL_ACKD_BY_TDC		10
#define		CANCEL_ACKD_BY_BKR		11
#define		CANCEL_REJECTED_BY_BKR	12
#define		NOTHING_DONE			13
#define		CREDIT_MMKT				14 
#define		DEBIT_MMKT				15
#define		ACAT_IN					16
#define		ACAT_OUT				17
#define		TRADE_REVERSED			18
#define		REVERSE					19

//	below is the position structure 

typedef struct {
	char	acct[16];			// account id
	char	ticker[10];			// ticker - i.e. "IBM", for options "IBM AT"
	char	instrumentType;		// 1=stock   2=option
	char	longShort;					// flag: 1=long 2=short
	char	spare1;				// spare
	int		qty;				// # of shares or # of option contracts
	float	avgPrice;			// average price
	float	unrealPL;			// unrealized p&l
	float	realPL;				// realized p&l
	char	spare2[12];			// will be set to nulls

} BROKER_POSITION;

//	----------------------------------------------------------------------------

//	MESSAGE TYPES
//	==============



// MSG_BROKER_ACCT_IDS - response from mtBrokerRequestAcctIds

	typedef	struct {
		short	error;
		short	num;
		char	acct[20][16];		// account id's (up to 20 max)
	} BROKER_ACCT_IDS;


//	-----------------------------------------------------------------------------------

// MSG_BROKER_ACCT_SUMMARY - response from mtBrokerRequestAcctSummary

	typedef	struct {
		short	error;
		short	spare;
		double	cost;
		double	marketValue;
		double	realizedPL;					   
		double	unrealizedPL;
		double	cash;
		double	buyingPower;
		double	BPwithOpenOrders;
		char	cAccount[16];
	} BROKER_ACCT_SUMMARY;

// MSG_BROKER_ACCT_SUMMARY_LINE - response from mtBrokerRequestAcctSummary
//                                    replaces  MSG_BROKER_ACCT_SUMMARY     


	typedef struct				// acct summary - new line format
  {
		char	flag;				    // 1=intial line, 2=subsequest line 3=last line
		char	description[30];
		char	value[20];
		char	cAccount[16];
	} BROKER_ACCT_SUMMARY_LINE;

  
  
//	-----------------------------------------------------------------------------------
// MSG_BROKER_TABLE_MESSAGE - General Purpose table message

	typedef struct 		// general text display
	{
		char  type;		// H=heading B=body E=end (end will not have any fields)
								      // A=append R=remove U=update
		int	  source;		// see below
		int   fieldCount;   // # of fields in message
	    char **fieldTable; // pointers to field text
		char  internalBuf[400];   // 
	} BROKER_TABLE_MESSAGE;
	
	// source types

	#define	OPEN_ORDERS_SOURCE				32			  
	#define	CLOSED_ORDERS_SOURCE			35
	#define	OTHER_ORDERS_SOURCE			36
	#define	ALL_ORDERS_SOURCE				41 

	#define	OPEN_POSITIONS_SOURCE			34			  
	#define	CLOSED_POSITIONS_SOURCE		37
	#define	ALL_POSITIONS_SOURCE			38

	#define	OTHER_TRANSACTTIONS_SOURCE		30
	#define	SECURITY_TRANSACTTIONS_SOURCE	33

//	-----------------------------------------------------------------------------------
// MSG_BROKER_MESSAGE - General Purpose message


	typedef struct 	  // general text display
  {
		BYTE		colorCode;		// See below
		BYTE		numBytes;
		char	 	text[160];		
	} BROKER_MESSAGE;
	
	// colorCodes
	#define	BLACK	1			  
	#define	BLUE	2
	#define	RED		3
	#define	GREEN	4
	#define YELLOW	5

  
//	-----------------------------------------------------------------------------------

// MSG_BROKER_ACK_ORDER_BY_MYTRACK - response from mtBrokerEnterOrder
// MSG_BROKER_ACK_ORDER_BY_BROKER  - unsolicited msg from broker

	typedef	struct {
		BROKER_ORDER	order;
		char	errorCode;				// 0 if ok,  otherwise order was rejected
		char	numBytesText;			// # of bytes in misc text
		char	miscText[80];			// will contain error/rejection text if
										// order is not accepted or just routing misc text
	} BROKER_ACK_ORDER;

//	-----------------------------------------------------------------------------------

// MSG_BROKER_ACK_CANCEL_BY_MYTRACK - cancel ack'ed by myTrack
// MSG_BROKER_ACK_CANCEL_BY_BROKER	- cancel ack'ed by broker
// MSG_BROKER_REJ_CANCEL_BY_BROKER	- cancel rejected by broker

	typedef	struct {
		BROKER_ORDER	order;
		char	errorCode;				// 0 if ok,  otherwise error code
		char	numBytesText;			// # of bytes in misc text
		char	miscText[80];			// will contain error/rejection text if
										// cancel is not accepted or just routing misc text
	} BROKER_ACK_CANCEL;



//	-----------------------------------------------------------------------------------

// MSG_BROKER_ORDER_CANCEL	- order cancelled by broker

	typedef	struct {
		BROKER_ORDER	order;
		char	errorCode;				// 0 if ok,  otherwise error code
		char	numBytesText;			// # of bytes in misc text
		char	miscText[80];			// will contain error/rejection text

	} BROKER_ORDER_CANCEL;



 //	-----------------------------------------------------------------------------------
// MSG_BROKER_ORDER_EXEC - unsolicited message when an order executes

	typedef	struct {
		BROKER_ORDER		order;		// the original order
		BROKER_TRANSACTION	trans;		// the transaction for this execution
		char	numBytesText;			// # of bytes in misc text
		char	miscText[80];			// may contain misc text 
	} BROKER_ORDER_EXEC;


//	-----------------------------------------------------------------------------------

// MSG_BROKER_ACCT_POSITION - response to position request

	typedef	struct {
		short	error;
		char	eof;					// 1=valid position 2=no more positions
										//	2 does not contain a position
		char	spare;
		BROKER_POSITION		position;	// a position record
	} BROKER_ACCT_POSITION;

//	-----------------------------------------------------------------------------------

// MSG_BROKER_ERROR_MESSAGE - error message from broker

	typedef	struct {
		char		numBytes;
		char	 	errorText[80];
	} BROKER_ERROR_MESSAGE;


	typedef	struct {
		short	error;
		char	eof;					// 1=valid transaction 2=no more transaction										
		char	spare;
		BROKER_TRANSACTION		trans;
	} BROKER_ACCT_TRANSACTION;

	typedef	struct {
		short	error;
		char	flag;					// 1=open, 2=no more order, 3 = Completed order										
		char	spare;
		BROKER_ORDER_RECORD		order;
	} BROKER_ACCT_ORDER;

	typedef struct {
		short error;
		char  eof;                   // 1 = valid transaction, 2 = no more transactions
		char  spare;
        BROKER_CASH_TRANSACTION  trans;
    } BROKER_ACCT_CASH_TRANSACTION;

//	======================================================================================
//	======================================================================================


//		now define the overall myTrack msg that will come back from the server


	typedef struct {
		short	messageCode;			// defines what type of message this is
		short	spare;
		int		rqn;					// the turn around data 
		union	{
			ERROR_REPORT        errorReport;
			LOGON_RESPONSE      logonResponse;
			LOGOFF_RESPONSE     logoffResponse;
			HEART_BEAT          heartBeat;
			HISTORICAL_DATA     historicalData;
			QUOTE_RESPONSE      quoteResponse;
			BACKGROUND_RESPONSE backgroundResponse;
			INTRADAY_UPDATE     tick;
			NEWS_HEADLINE       newsHeadline;
			NEWS_RESPONSE       newsResponse;
			FROM_TRACK          from_track;
			NEWS_VENDORS        newsVendors;
			SYMBOL_LOOKUP       symbolLookup;
			OPTION_PACKET       optionPacket;
      CHAT                chat;
			FUTURE_FAMILY_DATA  futData;
			OPTION_LINKS		          optionLinks;
			INTRADAY_DATA		          intradayData;
			HISTORICAL_NEWS           historicalNews;
			DATA_WAREHOUSE            dataWarehouse;
			TRANSACTION_LOG           transactionLog;
			NASDAQ_LEVEL_2            nasdaqLevel_2;
			AUTO_REG_RESPONSE         auto_reg_response;
			MARKET_PULSE		          marketPulse;
			TICKER_CLOSED		          tickerClosed;     // MSG_TICKER_CLOSED (no more intraday updates for today) *** DEPRECATED ***
			ZACKS_EARNINGS		        zacksEarnings;
			EDGAR_ONLINE		          edgarOnline;
			HEMINGTON_SCOTT		        hemingtonScott;
			VOLATILITY_DISPLAY	      volatilityDisplay;
			COMPOSITE_FAMILY_DATA     compositeFamilyData; // deprecated: use QUOTE_RESPONSE
			YESTERDAYS_LOGS		        yesterdaysLogs;
			COMPOSITE_VALUE           compositeValue;
      ORDER_BOOK                orderBook;
      MARKET_MAKER              marketMaker;
			BROKER_ACCT_IDS		        brokerAcctIds;
			BROKER_ACCT_SUMMARY	      brokerAcctSummary;    // no longer used, replaced by BROKER_ACCT_SUMMARY_LINE
			BROKER_ACCT_SUMMARY_LINE	brokerAcctSummaryLine;

			BROKER_ACK_ORDER	      brokerAckOrder;		// MSG_BROKER_ACK_ORDER_BY_MYTRACK
													// and MSG_BROKER_ACK_ORDER_BY_BROKER

			BROKER_ACK_CANCEL	      brokerAckCancel;	// MSG_BROKER_ACK_CANCEL_BY_MYTRACK 
													// and MSG_BROKER_ACK_CANCEL_BY_BROKER
													// and MSG_BROKER_REJ_CANCEL_BY_BROKER

			BROKER_ORDER_CANCEL	    brokerOrderCancel;	// MSG_BROKER_ORDER_CANCELLED

			BROKER_ORDER_EXEC	      brokerOrderExec;
			BROKER_ACCT_POSITION	  brokerPosition;
	    BROKER_ACCT_TRANSACTION brokerTransaction; // MSG_BROKER_TRANSACTION
			BROKER_ACCT_ORDER       brokerOrder;
      BROKER_ACCT_CASH_TRANSACTION brokerCashTransaction; // MSG_BROKER_CASH_TRANSACTION 
			BROKER_ERROR_MESSAGE	  errorMessage;	// MSG_BROKER_ERROR_MESSAGE
			// new broker messages
			BROKER_TABLE_MESSAGE    brokerTable;
			BROKER_MESSAGE          brokerMessage;


		} u;
	} MYTRACK_MSG;




//	*********************************************************************************

		// client side error codes - will be returned on a API call

#define	MT_ERR_NO_ERROR					 0		// no error
#define MT_ERR_WSA						-1		// error on WSAStartup
#define	MT_ERR_NO_Q						-2		// could not init queues
#define	MT_ERR_NO_MALLOC				-3		// could not malloc queue elements
#define MT_ERR_NO_SOCKET				-4		// could not get a socket
#define MT_ERR_NO_FIND_IP_ADDR			-5		// cannot find sdk.tdc.com ip address
#define	MT_ERR_NO_CONNECT				-6		// could not connect to mytrack
#define	MT_ERR_INPUT_THREAD				-7		// could not start input thread
#define	MT_ERR_OUTPUT_THREAD			-8		// could not start output thread
#define	MT_ERR_TIMER_THREAD				-9		// could not start timer thread
#define MT_ERR_INIT_NOT_CALLED			-10		// you did not call mtInit
#define MT_ERR_NOT_CONNECTED			-11		// you are not connected to a myTrack server
#define MT_ERR_NO_MESSAGE				-12		// there is no message right now
#define MT_ERR_SPARE2					-13		// you are already logged on
#define	MT_ERR_ARG_LENGTH				-14		// # of bytes of an argument is invalid
#define MT_ERR_NO_OUTPUT_Q_LINKS		-15		// could not send a msg - ran out of queue links
#define MT_ERR_OUT_MSG_TOO_LARGE		-16		// TDC problem - contact Support
#define MT_ERR_IN_MSG_TOO_LARGE			-17		// TDC problem - contact Support
#define MT_ERR_DISCONNECT				-18		// connection was disconnected
#define MT_ERR_NO_HB_FROM_SERVER		-19		// no heart beat from server
#define MT_ERR_INVALID_ARGUMENT			-20		// passed to the api routine
#define MT_ERR_INVALID_ACCT_ID			-21		// passed to a brokerage api routine
#define MT_ERR_DEINIT_CALLED			-22		// cannot call any api function
#define MT_ERR_ALREADY_CALLED			-23		// One-time operation has alread been done
#define MT_ERR_INVALID_PRODUCTNAME		-24		// change product name in mtRequestLogon
#define MT_ERR_TT_STARTUP				-25  	// error in Tunnel Startup
#define MT_ERR_TT_SHUTDOWN				-26  	// error in Tunnel Shutdown
#define MT_ERR_TT_CLOSE				    -27  	// error in Tunnel Close
#define MT_ERR_TT_DESTROY			    -28  	// error in Tunnel Destroy

#define MT_ERR_UNDEFINED				-33		// Undefined error
//	*********************************************************************************

		// server side error codes - will be returned as "error" in the data message


#define MTS_ERR_NO_ERROR				0		// ok
#define MTS_ERR_NO_ROOM					1		// no more room for connection
												// this error will be sent back (mc=0)
												// upon the tcp/ip connection.
												// even before you send the
												// "request to connect"
#define MTS_ERR_UNKNOWN_CONNECTION		2		// unknown connection
#define MTS_ERR_TOO_MANY_CONNECTIONS	3		// too many connections already opened
												// presently will accept up to 4
												// connections from the same ip address
#define MTS_ERR_NO_HEART_BEAT			4		// no heartbeat received - disconnecting (mc=2)
#define MTS_ERR_INVALID_ACCT			5		// invalid account-password

#define MTS_ERR_NO_SUCH_TICKER			7		// ticker not on database
#define MTS_ERR_NOT_LOGON				8		// you did not logon
#define MTS_ERR_LOGON_ALREADY			9		// you logged on already
#define MTS_ERR_TOO_MANY_REQUESTS		10		// too many requests outstanding
#define MTS_ERR_DB_NOT_AVAILABLE		11		// database requested is temporarily not available
											    // if condition persists - call tdc computer operations
#define MTS_ERR_NO_ACTIVITY				12		// no activity in a while - connection will be closed
												// reconnect when you need more data.
#define MTS_ERR_DB_ERROR				13		// database error, with your request,
												// database is probably ok
#define MTS_ERR_NO_DATA					14		// no data available for the ticker requested
#define MTS_ERR_NO_PRIOR_REQUEST		15		// no prior request
#define MTS_ERR_INVALID_REQUEST			16		// invalid request (check your parameters)
#define MTS_ERR_UNKNOWN_REQUEST			17		// no such request (invalid messageCode)
#define MTS_ERR_TOO_MANY_LINES			18		// # of lines of data exceeds system max
#define MTS_ERR_UNKNOWN_RQN				19		// unknown request # (unknown rqn)
#define MTS_ERR_INVALID_RQN				20		// rqn of 0 is invalid
#define MTS_ERR_TOO_MANY_CONCURRENT		21		// too many concurrent requests
#define MTS_ERR_INVALID_VERSION			22		// your version # is no longer supported
#define MTS_ERR_WAREHOUSE_NOT_AVAILABLE	23		// data warehouse system not available
#define MTS_ERR_TDS_NOT_AVAILABLE		25		// tds data base not available
#define MTS_ERR_WAREHOUSE_GET			26		// data request for key specified not found (get)
#define MTS_ERR_WAREHOUSE				27		// data warehouse error on access to data base
#define MTS_ERR_WAREHOUSE_PUT			28		// data warehouse error on put to data base
#define MTS_ERR_PROFILE					30		// profile data base error
#define MTS_ERR_NO_OPTIONS				31		// security has no options
#define MTS_ERR_NO_FUTURES				32		// security has no futures
#define MTS_ERR_NO_COMPOSITE			33		// security has no composite markets
#define MTS_ERR_NO_OTC_BB				34		// security not on otc bb
#define MTS_ERR_NO_TRADE				35		// could not forward trade request to db server
#define MTS_ERR_FILE					39		// file does not exist
#define MTS_ERR_NO_SDK_FEATURE			42		// SDK feature not enabled
#define MTS_ERR_NO_SDK_REGISTRATION		43		// you did not register for the SDK - i cross
												//  check your username if it's on file
/////////////////////////////////////////////////////////////////
// the following defines are used by the logging functions

// MTLOG LOG LEVELS
#define MTLOG_OFF      0
#define MTLOG_ERRORS   1
#define MTLOG_MESSAGES 3
#define MTLOG_INFO     7

// MTLOG RETURN VALUES
#define MTLOG_SUCCESS  0
#define MTLOG_INIT    -1 // mtLog system not inited 
#define MTLOG_REQUEST -2 // Invalid Request Parameter
#define MTLOG_SYSTEM  -3 // System call failed
/////////////////////////////////////////////////////////////////

#define MP_FORMAT_TEXT 1 // Format response  with all ascii characters and aligned columns - each line may be up to 160 chars


/////////////////////////////////////////////////////////////////

enum ORDER_CANCEL_TYPE	// to specify cancellation type in mtBrokerCancelOrderEx()
{
	OCT_SPECIFIC_ORDER  = 1,        // cancel a specific order
                                  //     set text = order id 
	OCT_LAST_ORDER      = 2,			  // cancel last order entered 
	OCT_ALL_ORDERS      = 3,		    // cancel all open orders 
	OCT_SPECIFIC_SYMBOL = 4,			  // cancel all orders for a specific ticker
                                	//      set text = ticker symbol
                                  //	    will cancel all orders for that ticker 
                                  //      AND all option orders that has that
                                  //      ticker as an underlyer.
  
	OCT_TOP_SYMBOL      = 5,		    // cancel the closest order to the market 
                                  //   for a specific ticker.
                                  //      set text = ticker symbol
};


/////////////////////////////////////////////////////////////////

#ifdef USING_MYTRACK_DLL
#include "myTrack_funcPtr.h"
#else // using myTrack.lib
#include "myTrack_func.h"
#endif

#if defined __cplusplus && ! defined SOLARIS_REV
}
#endif

#endif

/*
Function to ConvertFractions in 'line' variable of TRANSACTION_LOG responses
int ConvertFractions(unsigned char token,char *sToken) // token = character value from 'line', sToken = ptr to store ascii string  
{
	int ret = 1;
	switch ((int)token)
	{
	case 224:
	case 226:
	case 230:
	case 238:
		strcpy(sToken," 1/");
		break;
	case 225:
	case 229:
	case 233:
	case 237:
	case 241:
	case 245:
	case 249:
	case 253:
		strcpy(sToken,"s");
		break;
	case 227:
	case 235:
	case 243:
	case 251:
		strcpy(sToken,"8");
		break;
	case 231:
	case 247:
		strcpy(sToken,"4");
		break;
	case 239:
		strcpy(sToken,"2");
		break;
	case 228:
	case 234:
	case 246:
		strcpy(sToken," 3/");
		break;
	case 232:
	case 242:
		strcpy(sToken," 5/");
		break;
	case 236:
	case 250:
		strcpy(sToken," 7/");
		break;
	case 240:
		strcpy(sToken," 9/");
		break;
	case 244:
		strcpy(sToken," 11/");
		break;
	case 248:
		strcpy(sToken," 13/");
		break;
	case 252:
		strcpy(sToken," 15/");
		break;
	default:
		sToken[0] = token;
		sToken[1] = 0;
		ret = 0;
		break;
	}

	return ret;
}
*/
