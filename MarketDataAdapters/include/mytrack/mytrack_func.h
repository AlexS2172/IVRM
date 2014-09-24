		/************  WARNING *********************** 
		THIS MATERIAL IS TRACK DATA COMPANY PRIVATE!
		IT MAY NOT BE DISTRIBUTED OR SHARED WITHOUT PERMISSION
		*************************************************/
		
// this file contains the myTrack function prototypes

#ifndef MYTRACK_FUNC_H
#define MYTRACK_FUNC_H


#if defined __cplusplus && ! defined SOLARIS_REV
extern "C" {
#endif



//	------------------------------------------------------------------------------------

//		Init the myTrack Api environment
											
int		mtInit (void);

//	------------------------------------------------------------------------------------

//		DeInit the myTrack Api environment
//		use this function when you are shutting down your program
//		cannot call mtInit after this
											
int		mtDeInit (void);

//	------------------------------------------------------------------------------------

//		Supply host name/ip address


int   mtSetHost(char * host, int port);

//	------------------------------------------------------------------------------------

//		Set host & port of Proxy server
 
int   mtSetProxy(char * host, int port);

//	------------------------------------------------------------------------------------

//		Set to use tunnel or connect directly
 
int   mtSetUsingTunnel(BOOL flag);
			// flag = TRUE  : use Tunnel		(default setting)
			// flag = FALSE : connect directly

//	------------------------------------------------------------------------------------

//		Connect to a myTrack server				

											
int		mtConnectToServer (void);
int     mtDisconnect(void);
//	------------------------------------------------------------------------------------


											
int		mtGetStats (STATS_PKT *statPkt);			// pointer to a STATS_PKT structure

//	------------------------------------------------------------------------------------


//		get a MYTRACK_MSG - a message from the myTrack server

int		mtGetMessage (MYTRACK_MSG * message, int flag);	// returns the message

//			This is the routine you will use to retrieve the various messages
//			comming back from the server.

//			flag:  0= do not pend for a message - return MT_ERR_NO_MESSAGE if no message
//						if there is a msg it will return it.
//				  -1= pend forever until a message comes
//			       otherwise, pend for that many milliseconds and either return 
//						MT_ERR_NO_MESSAGE at the end of that time interval
//						or return a message if it comes in before the time expires. 


//	------------------------------------------------------------------------------------


//		Request to logon
											
int		mtRequestLogon (int rqn,				// request # - turn around data 
						char * username,		// 16 bytes max
						char * password,		// 16 bytes max
						char * productName,		// your product name - 12 bytes max
						UCHAR revHO,			// your product rev # high order
						UCHAR revLO);			// your product rev # low order

//	-----------------------------------------------------------------------------------


//		Request to logoff
											
int		mtRequestLogoff (int rqn);				// request # - turn around data 


//	-----------------------------------------------------------------------------------

//		Request for historical data
											
int		mtRequestHistoricalData (int rqn,		// request # - turn around data
								 char *ticker,	// ticker symbol - up to 16 bytes
								 short date[3],	// starting date: year, month, day
								 short numPkts,	// # of packets requested
								 short period);	// 1=daily 2=weekly 3=monthly

//			The server will send you the data backwards in time from the most recent day
//			going backwards. Hence the starting date is really the end date on a chronological
//			basis.  The # of packets requested has to be between 1 and 255. If you need more
//			data, make additional requests.


//	-----------------------------------------------------------------------------------

//		Request for quote data
											
int		mtRequestQuoteData (int rqn,			// request # - turn around data
							char *ticker,		// ticker symbol - up to 14 bytes
							char *optionCode);	// option code if requesting an option
												// 2 bytes, or null string if not an option



//	-----------------------------------------------------------------------------------


//		Request for background data
											
int		mtRequestBackgroundData (int rqn,		// request # - turn around data
							char *ticker,		// ticker symbol - up to 14 bytes
							char *optionCode);	// option code if requesting an option
												// 2 bytes, or null string if not an option



//	-----------------------------------------------------------------------------------


//		Request for intraday update
											
int		mtRequestIntradayUpdate (int rqn,		// request # - turn around data
							char *ticker,		// ticker symbol - up to 14 bytes
							char *optionCode,	// option code if requesting an option
												// 2 bytes, or null string if not an option
							UCHAR	flag);		// 1=start 2=stop

//		This request will start/stop tick info updates



//	-----------------------------------------------------------------------------------


//		Request for dynamic news headlines msgs
											
int		mtRequestNewsHeadlines (int rqn,		// request # - turn around data
							UCHAR	flag);		// 1=start 2=stop


//		This request will start/stop news headlines from coming in
//		If you do not call this routine, you will not get any news.
//		Hence, call it once after a successful logon.


//	-----------------------------------------------------------------------------------


//		Request for news headlines for a specific ticker symbol
											
int		mtRequestNewsByTicker (int rqn,			// request # - turn around data
							   char *ticker,		// ticker symbol - up to 16 bytes
							   char y_flag);   // 0=today's news,1=yesterday's news 

//		requests all the news headlines for a specific ticker.
//		message returned will be of type MSG_NEWS_RESPONSE.


//	-----------------------------------------------------------------------------------


//		Request for a specific news story
											
int		mtRequestNewsStory (int rqn,				// request # - turn around data
							char *storyNumber);		// story number - up to 8 bytes
		
//		request for a specific story. 
//		message returned will also be of type MSG_NEWS_RESPONSE.

//	-----------------------------------------------------------------------------------

//		Request news vendors definition
											
int		mtRequestNewsVendors (int rqn);				// request # - turn around data
							

//		request for all the news vendors codes and descriptions

// request for symbol lookup
int mtRequestSymbolLookup(int rqn, 
						  char *symbol, // ticker or wildcard pattern 
						  char type,    // 0=ticker, 1= wild card pattern
						  char next,    // (for type 1 only) 0=initial request 1=more
						  char nol);    // (for type 1 only) number of matching company names to return

// request option data
int mtRequestOptionData(int rqn, 
						char *ticker, 
						char RequestType,  // 0=calls 1=puts 2=both 
						char composite,    // 0=one market 1=all markets 
						short pct_out,     // % out of the money filter
						short pct_in,      // % in the money filter
						BYTE num_lines,    // 
						int flag);         // 0 = initial request 1=more 

int mtRequestOptionLinks(int rqn, 
						 char *symbol);

int mtRequestFutureFamilyData(int rqn, 
							  char *symbol);

// request for intraday 1 minute interval data
int mtRequestIntradayData(int rqn, 
						  char *symbol, 
						  BYTE dateFlag);  // 0 = most current date, 1 = 1 day back, 2 = 2 days back, etc. 

int mtRequestIntradayDataTime(int rqn, 
						  char *symbol, 
						  BYTE dateFlag,   // 0 = most current date, 1 = 1 day back, 2 = 2 days back, etc.
                    short minFromMidnight);   

//	------------------------------------------------------------------------------------
//  Version 2.1  
//  ------------------------------------------------------------------------------------

//  Changes To Previous Function Definitions and Structures
//  -------------------------------------------------------
//  OPTION_PACKET structure: exchangeRoot changed from int to char[4]


//  New Functions for Version 2.1
//  -----------------------------
//  mtRequestDataWarehouse()
//  mtRequestTransactionLog()
//  mtRequestNasdaqLevel_2()
//  mtRequestNewsByVendor()
//  mtRequestHistoricalNews_Ticker()
//  mtRequestHistoricalNews_Story()
//	------------------------------------------------------------------------------------

//  request for Data Warehouse

    int mtRequestDataWarehouse( int rqn,
							char actionType, // 1 = get data, 2 = send data, 3 = delete data
							short key, // specifies what record we're dealing with
							char *data); // only valid if actionType = 2

//	message returned will be of type MSG_DATA_WAREHOUSE (15).
						  
//	------------------------------------------------------------------------------------

// request for transaction log

	int mtRequestTransactionLog(int rqn, 
								char flag, // 0=initial request 1=page down 2=page up 
								char nol,  // # of lines in window
								char noc,  // # of columns in window
								char *text,// request text (max 10 chars). typically a ticker symbol
								int type); // 0= both trades and quotes, 1 = quotes only, 2 = trades only 

//	message returned will be of type MSG_TRANSACTION_LOG (17).

//	------------------------------------------------------------------------------------
 
// request for Nasdaq Level 2 data

	int mtRequestNasdaqLevel_2(int rqn, 
		                       char flag, //1=initiate dynamic monitoring 2=terminate monitoring 
							   char nol, //# of lines: in the window for display max = 24 minimum = 8
							   char show_closed, // mm: 1=do not display closed mm, 2=display closed mm
							   char *text); // request text (max 12 chars). typically a ticker symbol

//  Note: the server will stop dynamic updating after 15 minutes
//  hence if the window is still monitoring, you should refresh this
//  request every 10 minutes. In this case, rqn must be same as in 
//	the previous request(s).

//	message returned will be of type MSG_NASDAQ_LEVEL_2 (36).

//	-----------------------------------------------------------------------------------

// request for news by vendor 
	
	int mtRequestNewsByVendor(int rqn,
							  char *vendor,  // vendor (ex: D for Dow Jones) 
							  int headline); // earliest headline # - set to 1 for all of today's news

// Up to 24 lines will be sent back. To get more re-issue request 
// with a headline value equal to 1 greater than the last returned
 
//	message returned will be of type MSG_NEWS_RESPONSE (11).

//	-----------------------------------------------------------------------------------
// requests for historical news

	// Get headlines for ticker  
	int mtRequestHistoricalNews_Ticker(int rqn, 
									   char nol,    // number of headlines to return
									   char more,   // 0=initial request, 1=more headlines
									   char *ticker);  // ticker symbol i.e 'IBM'

	// Get matching story
	int mtRequestHistoricalNews_Story(int rqn, 
									  int storynumber, // ex: 12345 
									  char *mmddyy);   // ex: 01/24/00   

//      the data will be sent back in lines. each line may have
//      up to 160 bytes of text but presently will not exceed 80 bytes.
//      stories and headlines will be sent one line at a time.

//	message returned will be of type MSG_HISTORICAL_NEWS (30).
//	-----------------------------------------------------------------------------------

//	------------------------------------------------------------------------------------
//  Version 2.2  
//  ------------------------------------------------------------------------------------

//  Changes To Previous Function Definitions and Structures
//  -------------------------------------------------------
//  SYMBOL_LOOKUP structure: 'spare' member changed to 'nob' (type remains as char)   


//  New Functions for Version 2.2
//  -----------------------------
//  int mtDisconnect() - close tds socket connection (declaration on line 1059 (app.) of this file) 
//  int mt3rdParty_MarketPulse()
//	int mtMPFormatData(char *from[80], void *to,int type)
//	int mtBrokerUseRealAcct (void) (declaration on line 1380 (app.) of this file)

//-----------------------------------------------------
// Request Market Pulse Data.

int mt3rdParty_MarketPulse(int rqn, char *text); 

// Text is typically a page number.
// 101	- get me page 101
// 102.1	- get me yesterday page 102
// 102.2   - get me after hours for page 102
// message returned will be of type MSG_MARKET_PULSE (8)

int mtMPFormatData(char *from, void *to,int type);
	
//int mtMPFormatData(char from[24][80], void *to,int type);
// Format Incoming Market Pulse Data Based On type


//  New Functions for Version 2.3
//  -----------------------------
int mt3rdParty_ZacksEarnings(int rqn, char *text); 
// message returned will be of type MSG_ZACKS_EARNINGS (34)

int mt3rdParty_EdgarOnline(int rqn, char *text);
// message returned will be of type MSG_EDGAR_ONLINE (55)

int mt3rdParty_HemingtonScott(int rqn, char *text);
// message returned will be of type MSG_HEMINGTON_SCOTT (57)
		
int mtRequestVolatilityDisplay(int rqn, char *text);
// message returned will be of type MSG_VOLATILITY_DISPLAY (56)

int mtRequestCompositeFamilyData(int rqn, char *text);
// message returned will be of type MSG_COMPOSITE_FAMILY_DATA (26)

int mtRequestYesterdaysLogs(int rqn,
							char flag, //0=initial request 1=page down 2=page up 
							char nol,  //# of lines in window 
							char noc,  //# of columns in window
							char *text, // typically a ticker symbol. max 10 chars
							int type); // 0= both trades and quotes, 1 = quotes only, 2 = trades only 

// message returned will be of type MSG_YESTERDAYS_LOGS (18)

// New Functions for Version 2.4
// -----------------------------
int mtRequestMarketMakers(int rqn, char flag);
/* flag: 1 give me Nasdaq, 2 give me OTC BB
   3 give me listed, 4 give me options, 5 give me ECN's
*/
// message returned will be of type MSG_MARKET_MAKERS (63)

int mtRequestOrderBook(int rqn, 
		                 char flag, //1=initiate dynamic monitoring 2=terminate monitoring 
							  char nol, //# of lines: in the window for display max = 24 minimum = 8
							  char *MMid, // either specify a market maker or leave blank for all mm's
							  char *text); // request text (max 12 chars). typically a ticker symbol

//  Note: the server will stop dynamic updating after 15 minutes
//  hence if the window is still monitoring, you should refresh this
//  request every 10 minutes. In this case, rqn must be same as in 
//	the previous request(s).

// message returned will be of type MSG_ORDER_BOOK (61)

// New Function for Version 2.41
// -----------------------------
int getVersion(void); // deprecated, conflicts with Clarion
// returns the SDK version #

// New Function for Version 2.42
// -----------------------------
int mtGetVersion(void); // returns the SDK version #
BOOL mtIsConnected(void); // returns current connection status
//-------------------------------------------------------

//		BROKERAGE RELATED REQUESTS
//		---------------------------


//		will tell the system that you want to use only the contest accounts
//		this is very helpful when you are developing your application and do
//		not want to test on a real account.

int		mtBrokerUseContest(void);
int		mtBrokerUseRealAcct(void);

//	-----------------------------------------------------------------------------------



//		Request all account id's associated with the myTrack id 
//		that you logged in under.


int		mtBrokerRequestAcctIds (short brqn);			// broker request #
								

//		message returned will also be of type MSG_BROKER_ACCT_IDS


//	------------------------------------------------------------------------------------

//		Request an account summary 


int		mtBrokerRequestAcctSummary (short brqn,		// broker request #
									char *acctId);	// account id
								

//		message returned will  be of type MSG_BROKER_ACCT_SUMMARY


//	------------------------------------------------------------------------------------

//		Request account positions

//		will return open, closed, or all positions for a specified account
//		a long and short position for a specific ticker will be returned
//		as 2 position records. 


int		mtBrokerRequestAcctPositions (short brqn,		// broker request #
									  char *acctId,		// account id
									  char	flag);		// 1=open 2=closed 3=all

//		message returned will  be of type MSG_BROKER_ACCT_POSITION


//	----------------------------------------------------------------------------------

//		Enter an order 

//		you need to submit a BROKER_ORDER structure - see above



int		mtBrokerEnterOrder (short brqn,					// broker request #
							BROKER_ORDER *Order);		// passed structure

//	----------------------------------------------------------------------------------
// use these for condFlag field

int	mtBrokerEnterCondOrder (short brqn, 	         // broker request #
                              BROKER_ORDER * order1,  // first order
                              BROKER_ORDER * order2,  // second order
                              int condFlag);          // type of conditional order
//	----------------------------------------------------------------------------------

//		Enter a mutual fund order 

int		mtBrokerEnterMutualFundOrder(	short  brqn,
										char   *acctId, 
										char   *buySymbol, 
										char   *sellSymbol,
										BYTE   actionCode,           // 1=buy 2=sell 3=exchange
										BYTE   specialInstructions,  // 1=reinvest dividends & capital gains
											                                        // 2=pay dividends & reinvest cap.gains
											                                        // 3=pay dividned & capital gains
										BYTE   entireBalance,				// 1=if selling/exchanging entire balance
											                                        // 2=if by dollar amount
										float  dollarAmount 				  // if entireBalance is  set to 2
                                 );

//	----------------------------------------------------------------------------------

//		Enter an option spread order

int		mtBrokerEnterOptionSpreadOrder(	short  brqn,
										BROKER_ORDER * order1, 
                    BROKER_ORDER * order2);

//	----------------------------------------------------------------------------------
//		Cancel an order 


int		mtBrokerCancelOrder (short brqn,				// broker request #
							char *acctId,				// account id
							char *OrderId);				// passed order id

//	=======================================================================================

int	mtBrokerCancelOrderEx (short brqn, 
                         char *acctId, 
                         ORDER_CANCEL_TYPE type, 
                         char *text);

//	=======================================================================================

int mtBrokerRequestTransactionData(short brqn,          // broker request #
								   char *acctId,        // account id
								   char *symbol,        // Optional ticker symbol
								   char *sdate,         // Start date for Transactions mm/dd/yy   
								   char *edate);        // End date for transactions   mm/dd/yy

// you get back 1 or more messages of type MSG_BROKER_TRANSACTION


int mtBrokerRequestOrder(short brqn,                    // broker request #
						 char *acctId,                  // account id
						 char *sdate,                   // Start date for Orders mm/dd/yy
						 char *edate,                   // End date for Orders  mm/dd/yy
						 BYTE RequestType);              // 1 = open orders, 2 = completed orders, 3 = other orders
// you get back 1 or more messages of type MSG_BROKER_ORDER_DATA


int	mtBrokerCashTransactions (short brqn,               // broker request #
							  char *acctId,             // account id
							  char *sdate,              // Start date mm/dd/yy
							  char *edate);             // end date   mm/dd/yy
// you get back 1 or more messages of type MSG_BROKER_CASH_TRANSACTION








//	*********************** utility routines **************************************

//		routine to convert an integer price and format code to a double

double mtuGetPrice(int price, int format_code);


//	-------------------------------------------------------------------------------


//		routine to convert an integer price and format code with
//		the associated daily split factor and general split factor to a double

double mtuGetSplitAdjustedPrice (int price,
								 int format_code,
								 int generalSplitFactor,
								 int dailySplitFactor);


//	-------------------------------------------------------------------------------

//		routine to retrieve the last Socket error code

int		mtuGetLastSocketError (void);


// Byte swapping routine for Solaris platform
void mtUnixMsgConv(MYTRACK_MSG *pMsg);



#define MT_LOGFILE "mtapi.log"

// Logging Functions
//   ( see MTLOG RETURN VALUES for return values)
int  mtLogInit(BYTE Level,   // logging level (see MTLOG LOG LEVELS)
			   char *logfile, // Logfile name
			   char *errmsg,   // buffer for description of error
			   size_t  errmsg_size
			   ); 

int  mtSetLogLevel(BYTE LogLevel);  // (see MTLOG LOG LEVELS)
int  mtGetLogLevel(BYTE *pLevel);   // (see MTLOG LOG LEVELS) 

#if defined __cplusplus && ! defined SOLARIS_REV
}
#endif

#endif

