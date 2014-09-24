////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//                  "PCQT3066"  Single Stock Futures master database          //
//                                                                            //
//                        preliminary specs                                   //
//                                                                            //
//                    September 26, 2003      R.WAGNER                        //
//                                                                            //
//    revision history :                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __SINGLE_STOCK_FUTURES_INCL__
#define __SINGLE_STOCK_FUTURES_INCL__

#ifndef INCL_NOPACKING
	#pragma pack(2)
#endif



#define	SSF_MAX_CYCLE			16
#define	SSF_MAX_UND_PER_RECORD	8
#define	SSF_MAX_CALENDAR_DATES	12
#define	SSF_MAX_CLASSES			4

#define	SSF_MAX_SPC_DIV			4

typedef union _SSF_DATE
{
	struct
	{
		UCHAR	century;
		UCHAR	year;
		UCHAR	month;
		UCHAR	day;
	}	date;
	ULONG	lDate;

}	SSF_DATE;



//
//		data types
//

#define DATA_TYPE_SSF_CLASS						0
#define DATA_TYPE_SSF_FUTURE					1
#define DATA_TYPE_SSF_OPTION					2
#define DATA_TYPE_SSF_ADDITIONAL_UND   			3 

#define DATA_TYPE_SSF_UND_DATA					10
#define DATA_TYPE_SSF_ALT_UND_DATA				11    

#define DATA_TYPE_SSF_INTEREST_RATE				100 
#define DATA_TYPE_SSF_CALENDAR					101

//		record categories

#define SSF_CAT_CLASS_HEADER					0

#define SSF_CAT_PRM_SSF_TKR						1
#define SSF_CAT_MODIFIED_SSF_TKR				2

#define SSF_CAT_PRM_SSF_OPT						10
#define SSF_CAT_MODIFIED_SSF_OPT				11


//				class types

#define SSF_CLASS_TYPE_EQUITY						0
#define SSF_CLASS_TYPE_INDEX 						1
#define SSF_CLASS_TYPE_INDEX_OPEN_SETTLED			2


//        INTEREST RATES

#define   INT_CD_RATES						0
#define   INT_COST_TO_CARRY_RATES			0

//        CALENDAR  TYPES ,  CATEGORIES, AND SEQUENCES

#define	SSF_CLNDR_TYPE_MONTHLY_EXP			0

#define	SSF_CLNDR_CAT_EQUITY				1
#define	SSF_CLNDR_CAT_INDEX					1
#define	SSF_CLNDR_CAT_INDEX_OPEN_SETTLED	5


#define	CLNDR_SEQ_YEAR_0					0
#define	CLNDR_SEQ_YEAR_1					1
#define	CLNDR_SEQ_YEAR_2					2
#define	CLNDR_SEQ_YEAR_3					3

//        MISC

#define	EXERC_STYLE_AMERICAN				0
#define	EXERC_STYLE_EUROPEAN				1


#define	DIV_TYPE_CASH						0	
#define	MAX_SPC_DIV_1111					4

//
//      Key Numbers
//

#define   SSF_PRIMARY_KEY_NUM					0
#define   SSF_SYMBOL_KEY_NUM					1
#define   SSF_IOPTION_KEY_NUM					2
#define   SSF_UNDERLYING_KEY_NUM				3
#define   SSF_CLASS_TYPE_KEY_NUM				4



////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//       KEY LAYOUTS                                                          //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//
//    Primary Key 
//

typedef struct  _SSF_KEY_0
{
	ULONG		dataType;			//	0 =  class data/ future definitions
								//  1 =  future definitions
								//  2 =  option definitions
								//	10 =  future underlyings 
								//	11 =  future alt underlyings for open settled indexes
								//  100 =  system interest rates
								//  101 =  system calendars

	TICKER_KEY	classKey;			//	for data type 0 - 11 -- key of primary intrument underlying class
								//	for other data types -- NULL/Space filled; 

//	future definitions
  

	CHAR		countryCode[2];		//	for data type 0 - 11 -- country code of SSF ticker symbol
								//	for calendars		 -- country of calendar ( or blank for default )
								//	for intetrest		 -- country of interest rates ( or blank for default ) ??
								//	for other data types -- blank / reserved; 
	CHAR		exchangeCode[2];	//	for data type 0 - 11 -- exchange code of SSF ticker symbol
								//	for calendars		 -- exchange of calendar ( or blank for default )
								//	for intetrest		 -- exchange of interest rates ( or blank for default ) ??
								//	for other data types -- blank / reserved; 
   
	UCHAR		futureCategory;	
								//	for data type 0 - 11:
								//		0		=   class header
								//		1		=   primary ticker symbol ( future )
								//		2+		=   secondary ticker symbol ( future )
								
								//	for calendar records:
								//		type of expiration calendar
								//			0	= monthly expiration 
								//			1 +	= (reserved)	
								//	for other data types:
								//		0    


	CHAR		tickerSymbol[7];//	root portion of SSF ticker symbol

//	Option Definitions

	UCHAR		futureYear;		//	underlying future settlement year ( 0 for class header or future definition )	
	UCHAR		futureMonth;	//	underlying future settlement month ( 0 for class header or future definition )
	UCHAR		optionCategory;	//	for data type 0 - 11:
								//		0		=   class header or future definition
								//		10		=   primary symbol ( option )
								//		11 +	=   secondary symbol ( option )
								//	for calendar records:
								//		category of expiration calendar
								//			1	= equity/ index 
								//			5	= open settled index 
							

   CHAR			optTickerSymbol[7];	//	root portion of SSF ticker symbol ( blank for class header or future definition )


   //  symnbol definition, strike exceptions,
   //  additional underlyings
   //
   //  blanks for class header, underlying data
   
   LONG			recordSequence;		//  for data type 0 - 11 used if multiple records required 
									//		such as additional underlying records,
									//
									//	for calendar records used for year offset from current dates
									//
									//	for other data types ( reserved )
   
   
} SSF_KEY_0, * lpSSF_KEY_0;


//                                   
//    secondary keys                 
//                                   

                                  
typedef struct _SSF_KEY_1		//  acess data by future root symbol
{
	ULONG	dataType;			//  data type = 0
	CHAR    tickerSymbol[7];	//  root ticker symbol
	CHAR    countryCode[2];     //  country code
	CHAR    exchangeCode[2];	//  exchange code
	LONG	recordSequence;
}  SSF_KEY_1;
                                  
typedef struct _SSF_KEY_2		//  acess data by option root symbol
{
	ULONG	dataType;			//  data type = 0
	CHAR    optTickerSymbol[7];//  ticker symbol
	CHAR    countryCode[2];		//  country code
	CHAR    exchangeCode[2];	//  exchange code
	LONG	recordSequence;
}  SSF_KEY_2;
                                  
                                    
typedef struct _SSF_KEY_3			//  access  underlying security data by 
{                               
	ULONG		dataType;			//  data type = 10 for for underlying securities
									//            = 11 for alternate underlying
	TICKER_KEY	key;				//	underlying  key;
	LONG		recordSequence;
}	SSF_KEY_3;
                                  
typedef struct _SSF_KEY_4			//  access by class type
{                               
	ULONG		dataType;			//  data type = 0
	ULONG		classType;			//  class type
	TICKER_KEY	key;				//	key of primary underlying		
	LONG		recordSequence;
}  SSF_KEY_4;



//	update data



//			update flag mask


#define     SSF_DELETED			0x00000001
#define		SSF_ADDED			0x00000002
#define     SSF_CHANGED			0x00000004
#define     SSF_UINDELETED		0x00000008 
#define		SSF_CALENDAR_LOCK0	0xff000000


typedef	union
{
	struct
	{
		ULONG	fDeleted		:1;
		ULONG	fAdded			:1;
		ULONG	fChanged		:1;
		ULONG	fUndeleted		:1;
	}	bits;
	ULONG	mask;
}	SSF_FLAGS;

typedef	union
{
	struct
	{
		ULONG	lName			:1;
		ULONG	lDesc			:1;
		ULONG	lVolatility		:1;
		ULONG	lDividend		:1;
		ULONG	lCalendar		:12;


	}	lockBits;
	ULONG	lockMask[2];
}	SSF_LOCKS;


//			lock flags for calendar records date entries ( bit/month )
//			added to support leap/open settled calendar maintenace processing
//			for Hyperfeed internal user ONLY



typedef	struct	_SSF_UPDATE_DATA
{
	SSF_FLAGS	flags;
	SSF_DATE	addDate;
	SSF_DATE	updateDate;
	SSF_DATE	deleteDate;
	SSF_LOCKS	locks;
}	SSF_UPDATE_DATA, *lpSSF_UPDATE_DATA;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//       DATA RECORD LAYOUTS                                                  //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//       OPTION DATA RECORD LAYOUTS  (  data type 0 )                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//                                
//      CLASS HEADER    ( item type 0 -- record category 0 ) 
//                                
                                  
typedef struct	_SSF_CLASS_HEADER   
{                               
	  
	  
	CHAR		issuerName[48];	//  issuer name
	CHAR		issueDesc[32];	//  issuer description
	ULONG		numSymbols;		//	number of futures symbols
}	SSF_CLASS_HEADER, *lpSSF_CLASS_HEADER;


//
//      Furture Definition  ( item type 0 -- record category 10 - 59 )
//


typedef struct     _FUTURE_SYMBOL
{
	UCHAR		expCalendarType;		//  expiration calendar type
	UCHAR		expCalendarCat;			//  expiraation calendar category											//  calendar
	  
	USHORT		settlementStyle;		//  0 = American
												//  1 = European
												//  all other values reserved for
												//  future requirements
  
	ULONG       contractSize;			//  number of underlying units for future
	  
	  
	  
	LONG		cashSettlement;			//  total cash amount for settlement of future
	SHORT		priceType;				//  cash price type
	SHORT		reserved;
	
	  
	TICKER_KEY		primaryUnd;		//  primary underlying security this future contract
	  
	TICKER_KEY		altUnd;			//  underlying security this future used to evaluate 
								//	the open settled indexes disemminated by the exchanges with the
								//  "open settlement" value 
	  
	  
	ULONG		primaryShares;	//  shares per contract of primary ( or altternate ) underlying
  	BOOL		hasAddUnd;		//  future has additional underlying records
	BOOL		hasOptions;		//	number of option definition records

//	current trading	cycle  
	  
	struct						//  ordered table of future last trade dates
	{                               
		UCHAR	expYearOffset;	//  year offset
								//  represents years from current
								//  cycle year ( first 12 expiration
								//  months
								//  i.e futures expiring within year
								//  would have 0, one year out would
								//  have 1, etc.
		  
		UCHAR	expMonth;		//  last trade month
	}	cycleTable[SSF_MAX_CYCLE];					//  cycle dates

}	SSF_FUTURE_SYMBOL;


typedef struct	_OPTION_SYMBOL
{
	UCHAR		expCalendarType;		//  expiration calendar type
	UCHAR		expCalendarcat;			//  expiraation calendar category											//  calendar
	  
	USHORT		expStyle;				//  0 = American
										//  1 = European
										//  all other values reserved for
										//  future requirements
  
	ULONG       contractSize;			//  number of underlying units for future
	  
	  
//	current trading	cycle  
	  
	struct						//  ordered table of future last trade dates
	{                               
		UCHAR	expYearOffset;	//  year offset
								//  represents years from current
								//  cycle year ( first 12 expiration
								//  months
								//  i.e futures expiring within year
								//  would have 0, one year out would
								//  have 1, etc.
		  
		BYTE	expMonth;		//  last trade month
	}	cycleTable[SSF_MAX_CYCLE];					//  cycle dates
	  

}	SSF_OPTION_SYMBOL;


typedef struct	_SSF_ADDITIONAL_UND
{
	ULONG			numUnd;						//  number of underlyings listed in
	struct  
	{
		TICKER_KEY	undKey;						//	underlying key
		ULONG		undShares;					//  shares per contract 
	}	undTable	[SSF_MAX_UND_PER_RECORD];	
	BOOL			more;						//	hass more records	

}	SSF_ADDITIONAL_UND, *lpSSF_ADDITIONAL_UND;

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//       UNDERLYING SECURITY DATA RECORD LAYOUTS  (  data type 10, 11 )       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////


//
//      UNDERLYING DATA 
//      data structure is the same for underlying securities ( data type 10 )
//            and alternate underlying securities ( data type 11 )
//       
//

typedef struct	_SSF_UNDERLYING_SEC
{
	CHAR			issuerName[48];	//  name of issuer
	CHAR			issueDesc[32];	//  security description

	CHAR			primaryExch[2];	//  HTI standard exchange code for primary trading exchange of underlying
	CHAR			currency[3];	//	HTI standard currency code

	USHORT			numClasses;		// number of options classes this
									// this security underlies

	TICKER_KEY		primaryClass;	// class for which this is primary 
	TICKER_KEY		secondaryClass[SSF_MAX_CLASSES -1];


	USHORT			divFreq;		//  dividend payment frequency in times per year
									//  i.e  2 = semiannual, 4 = quarterly, 12 = monthly
	USHORT			divFmt;			//  format of dividend amounts using HTI standard price types
	SSF_DATE		exDivDate;		//  nest ex-dividend date if announced, or last
									//  if not announced ( zero if no dividends )
	LONG			divAmt;			//  amount of dividend
	  
	struct
	{
		SSF_DATE	spDivExDate;	//  ex-date of special (irregular) dividend
		USHORT		divType;		//  type of dividend;
									//    0  = cash
									//    1-   reserved for future use
		USHORT		spDivFmt;		//  format of dividend amount
		LONG		spDivAmt;		//  amount of dividend
	}	specialDiv[SSF_MAX_SPC_DIV];
									//  table of up to 4 special dividends
  
	LONG			volatility;		//  52 week historical volatility
	LONG			interestAdj;	//  ( reserved ) optional interest rate adjsutment factor for Theorecal value calcs
									//  ie dividend yield for indexes  

}	SSF_UNDERLYING_SEC, *lpSSF_UNDERLYING_SEC;


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//      RELATED SYSTEM DATA    ( data type 100, 101 )                        //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

 

//
//       INTEREST RATE RECORD  
//      ( data type 100 -- item type 0 -- record category 0
//

typedef struct	_SSF_INTEREST_RATE
{	  
	USHORT				rateFmt;					//  data format using PCQUOTE standard price type
	USHORT				filler;						//  reserved = 0
	  
	struct  
	{
		LONG			cdRate;						//  interest rates on $100,000 cd
	}					cdRateTbl[5];				//  5 table entries for 30, 60, 90, 180, 360 days
	  
	LONG				brokerCall;					//  broker call rate
	LONG				shortStock;					//  short stock rebate rate
}	SSF_INTEREST_RATE, *lpSSF_INTEREST_RATE;


//
//      Calendar   (  data type 101 )
//  

typedef struct	_SSF_CALENDAR
{
	SSF_DATE    calendarDate[SSF_MAX_CALENDAR_DATES];	//  12 date entries
}	SSF_CALENDAR;


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//      RECORD AND KEY BUFFER UNIONS                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


typedef  union _SSF_KEY

{
    SSF_KEY_0	primaryKey;					//
    SSF_KEY_1	futureKey;					//
    SSF_KEY_2	optionKey;					//
    SSF_KEY_3	underlyingKey;				//
    SSF_KEY_4	classTypeKey;				//
}	SSF_KEY, * lpSSF_KEY;					//




typedef  struct _SSF_RECORD
{  
	SSF_KEY_0			key;
	CHAR				classType;		//  class type: ( for data type zero records )
										//                                
										//    0     = equity
										//    1     = index normal
										//    2     = index open settled
										//    3-     reserved for future
										//            requirements
										//

	CHAR				reserved[31];	
	SSF_UPDATE_DATA		updateData;
	union
	{
		SSF_CLASS_HEADER		classDef;				//    class definition
		SSF_FUTURE_SYMBOL		futDef;					//    symbol definition
		SSF_OPTION_SYMBOL		optDef;					//    option definition
		SSF_ADDITIONAL_UND		additionalUnd;			//    additional underlying keys
		SSF_UNDERLYING_SEC		undSecurity;			//    underlying security data
		SSF_INTEREST_RATE		intRate;				//    interest rates
		SSF_CALENDAR			calendar;				//    calendars
	}	data;

}	SSF_RECORD, * lpSSF_RECORD;


#ifndef INCL_NOPACKING
	#pragma pack()
#endif

#endif
