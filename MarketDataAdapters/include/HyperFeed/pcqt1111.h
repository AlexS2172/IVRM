////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//                  "PCQT1111"  OPTIONS CLASS DATA                            //
//                                                                            //
//                        preliminary specs                                   //
//                                                                            //
//                    October 31, 1996        R.WAGNER                        //
//                                                                            //
//    revision history :                                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef __PCQT1111__
#define __PCQT1111__

#ifndef INCL_NOPACKING
	#pragma pack(2)
#endif

#ifndef __STRUCS_1111__
#define __STRUCS_1111__

#ifndef _DATE_1111
typedef struct _DATE_1111
{
	BYTE	century;
	BYTE	year;
	BYTE	month;
	BYTE	day;
} DATE_1111;
#endif

#ifndef _DBA_ITEM_1111 
#define _DBA_ITEM_1111 
typedef struct _DBA_ITEM_1111
{
	SHORT	type;
	CHAR	symbol[12];
	CHAR	countryCode[2];
	CHAR	exchangeCode[2];
} DBA_ITEM_1111;
#endif

#endif

//
//		data types
//

#define DATA_TYPE_OPTION_CLASS					0
#define DATA_TYPE_OPTION_STRIKE_EXCEPT  		1
#define DATA_TYPE_ADDITIONAL_UND      			2 

#define DATA_TYPE_UNDERLYING_DATA				10
#define DATA_TYPE_ALTERNATE_UND_DATA			11    

#define DATA_TYPE_INTEREST_RATE					100 
#define DATA_TYPE_CALENDAR						101

//		record categories

#define REC_CAT_CLASS_HEADER					0

#define REC_CAT_PRM_OPT_TKR						10
#define REC_CAT_SEC_OPT_TKR						11
#define REC_CAT_SEC_OPT_TKR_SPLIT				17
#define REC_CAT_SEC_OPT_TKR_MAX					19

#define REC_CAT_PRM_OPT_0_YR_LEAP				20
#define REC_CAT_SEC_OPT_0_YR_LEAP				21
#define REC_CAT_SEC_OPT_0_YR_LEAP_MAX			29

#define REC_CAT_PRM_OPT_1_YR_LEAP				30
#define REC_CAT_SEC_OPT_1_YR_LEAP				31
#define REC_CAT_SEC_OPT_1_YR_LEAP_MAX			39

#define REC_CAT_PRM_OPT_2_YR_LEAP				40
#define REC_CAT_SEC_OPT_2_YR_LEAP				41
#define REC_CAT_SEC_OPT_2_YR_LEAP_MAX			49

#define REC_CAT_PRM_OPT_3_YR_LEAP				50
#define REC_CAT_SEC_OPT_3_YR_LEAP				51
#define REC_CAT_SEC_OPT_3_YR_LEAP_MAX			59

//				class types

#define CLASS_TYPE_EQUITY						0
#define CLASS_TYPE_INDEX 						1
#define CLASS_TYPE_INDEX_OPEN_SETTLED			2
#define CLASS_TYPE_CURRENCY						3
#define CLASS_TYPE_END_OF_MONTH_CURR    		4
#define CLASS_TYPE_END_OF_QUARTER_IND   		5

#define CLASS_TYPE_3D_CURRENCY					10
#define CLASS_TYPE_3D_CURRENCY_MON   			10
#define CLASS_TYPE_3D_CURRENCY_TUE   			11
#define CLASS_TYPE_3D_CURRENCY_WED      		12
#define CLASS_TYPE_3D_CURRENCY_THU				13
#define CLASS_TYPE_3D_CURRENCY_FRI				14
#define CLASS_TYPE_3D_CURRENCY_MAX				14

//        INTEREST RATES

#define   INT_CD_RATES                          0
#define   INT_COST_TO_CARRY_RATES               0

//        CALENDAR  TYPES ,  CATEGORIES, AND SEQUENCES

#define   CLNDR_TYPE_MONTHLY_EXP                0
#define   CLNDR_TYPE_WEEKLY_EXP                 1
#define   CLNDR_TYPE_BANK_HOLIDAY               10
#define   CLNDR_TYPE_EXCHANGE_HOLIDAY           11

#define   CLNDR_CAT_EQUITY                      1
#define   CLNDR_CAT_INDEX                       1
#define   CLNDR_CAT_CURRENCY                    2
#define   CLNDR_CAT_EOM_CURRENCY                3
#define   CLNDR_CAT_EOQ_INDEX                   4
#define   CLNDR_CAT_INDEX_OPEN_SETTLED          5

#define   CLNDR_CAT_WEEKLY_MONDAY               1
#define   CLNDR_CAT_WEEKLY_TUESDAY              2
#define   CLNDR_CAT_WEEKLY_WEDNESDAY            3
#define   CLNDR_CAT_WEEKLY_THURSDAY             4
#define   CLNDR_CAT_WEEKLY_FRIDAY               5

#define   CLNDR_CAT_HOLIDAY                     0

#define   CLNDR_SEQ_WEEK_1                      1
#define   CLNDR_SEQ_WEEK_2                      2
#define   CLNDR_SEQ_WEEK_3                      3
#define   CLNDR_SEQ_WEEK_4                      4
#define   CLNDR_SEQ_WEEK_5                      5

#define	CLNDR_SEQ_YEAR_0                      0
#define	CLNDR_SEQ_YEAR_1                      1
#define	CLNDR_SEQ_YEAR_2                      2
#define	CLNDR_SEQ_YEAR_3                      3

//        MISC

#define	EXERC_STYLE_AMERICAN                  0
#define	EXERC_STYLE_EUROPEAN                  1


#define	DIV_TYPE_CASH							0
#define	MAX_SPC_DIV_1111						4
//
//      Key Numbers
//

#define   PRIMARY_KEY_NUM                       0
#define   CLASS_KEY_NUM                         0
#define   SYSTEM_DATA_KEY_NUM                   0

#define   SYMBOL_KEY_NUM                        1
#define   UNDERLYING_KEY_NUM                    2
#define   CLASS_TYPE_KEY_NUM                    3

//			update flags


#define		ufmAdded      0x00000001
#define     ufmChanged    0x00000002
#define     ufmDeleted    0x00000004
#define     ufmUndeleted  0x00000008 


//			lock flags for calendar records date entries ( bit/month )
//			added to support leap/open settled calendar maintenace processing
//			for Hyperfeed internal user ONLY

#define		ufmCalendarLocks	0x0000fff0

//			limits

#define		MAX_LEAP_YEAR		3



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//       KEY LAYOUTS                                                          //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


//
//    PRIMARY KEY FOR OPTION CLASS DATA RECORDS
//

   typedef struct  _PCQT1111_KEY_DATA

{
   USHORT   dataType;							//  data type    0 =  options class data
   //               1 =  options strike exceptions
   //               2 =  option additional underlyings
   //              10 =  option underlyings
   //              11 =  option alternate
   //                    underlyings
   //                    for open settled indexes
   //             100 =  system interest rates
   //             101 =  system calendars
   //
   
   CHAR    countryCode[2];			          //  PCQUOTE standard code
   
   USHORT  itemType;						          //  item type:
   //     = 0  for class header
   //        (record  category 0 )
   //     = 0  for option symbols
   //        (record categories 10 - 255)
   //     = PCQUOTE standard item
   //        type for underlying securites
   //        ( record categories 1 - 9 )
   
   CHAR    symbol[12];		          // for options data records --
   //   class symbol -- normally
   //   ticker symbol of primary
   //   underlying stock, but can be
   //   bastardized to create 8
   //   character symbol ( usually
   //   by omitting special characters
   //   such as "/" or "p" )
   //
   // for underlying records --
   //   ticker symbol of underlying
   //
   //
   
   
   USHORT  recordCategory;			          //  indicates category of data:
   //     0    =  class header
   //    10    =  primary option ticker
   //    11-19 =  secondary options tickers
   //    20    =  primary options zero year leap
   //    21-29 =  secondary option zero year leap
   //    30    =  primary options one year leap
   //    31-39 =  secondary option one year leap
   //    40    =  primary options two year leap
   //    41-49 =  secondary option two year leap
   //    50    =  primary options three year leap
   //    51-59 =  secondary option three year leap
   //    60-255 reserved
   
   CHAR    tickerSymbol[12];		          //  class of options ticker symbol for option
   //  symnbol definition, strike exceptions,
   //  additional underlyings
   //
   //  blanks for class header, underlying data
   
   USHORT  recordSequence;			          //  used if multiple records required 
   //    such as additional underlying records,
   //    strike price exception records,
   //    3D expiration calendars, etc.
   
   
   USHORT  classType;					          //  class type: ( for data type zero records )
   //                                
   //    0     = equity
   //    1     = index normal
   //    2     = index open settled
   //    3     = currency
   //    4     = end of month currency
   //    5     = end of quarter index
   //    6 -9    reserved for future
   //            requirements
   //
   //  3D currency expiration day :
   //    ( for expiration calendars - data type 101 )
   //
   //    10-14 =  monday-friday exp respectively
   //    15-255  reserved
} PCQT1111_KEY_DATA, * lpPCQT1111_KEY_DATA;




//
//  PRIMARY KEY
//


typedef struct  _PCQT1111_KEY_0        //  access options by class 
                                       //  symbol  
{
	USHORT	dataType;                  //  data type
    CHAR    countryCode[2];            //  country code
    USHORT  itemType;                  //  item type:
    CHAR    symbol[12];                //  class symbol 
    USHORT  recordCategory;            //  indicates category of data:
    CHAR    tickerSymbol[12];          //  ticker symbol
    USHORT  recordSequence;            //
} PCQT1111_KEY_0;


//                                   
//    SECONDARY KEYS                 
//                                   

                                  
  typedef struct _PCQT1111_KEY_1          //  acess options data by
                                          //  class portion of
                                          //  options ticker symbol
  {
	  USHORT  dataType;                     //  data type = 0
	  
	  CHAR    countryCode[2];               //  country code
	  CHAR    tickerSymbol[12];             //  ticker symbol
	  USHORT  recordCategory;               //  record category
	  USHORT  recordSequence;               //  record sequence
  }  PCQT1111_KEY_1;
                                  
                                    
  typedef struct _PCQT1111_KEY_2          //  access  underlying security data by 
                                          //    security key  
  {                               
	  USHORT  dataType;                     //  data type = 10 for for underlying securities
	  //            = 11 for alternate underlying
	  USHORT  itemType;                     //  item type of underlying
	  CHAR    symbol[12];                   //  ticker symbol
	  CHAR    countryCode[2];               //  country code of option class
  }  PCQT1111_KEY_2;
                                  
  typedef struct _PCQT1111_KEY_3          //  access options data by
                                          //  class type
  {                               
	  USHORT  dataType;                     //  data type
	  CHAR    countryCode[2];               //  country code
	  USHORT  classType;                    //  class type
	  USHORT  itemType;                     //  item type = 0
	  CHAR    symbol[12];                   //  class symbol
	  USHORT  recordCategory;                    //  record category
	  CHAR    tickerSymbol[12];             //  ticker symbol
	  USHORT  recordSequence;                    //  record sequence = 0
  }  PCQT1111_KEY_3;



//
//    KEY STRUCTURES FOR INTEREST AND CALENDAR DATA
//      (  key 0  redefinitions )
// 

  typedef struct  _PCQT1111_KEY_0_100     //  interest rate records
  {
	  USHORT  dataType;                     //  data type = 100 for
	  //  interest rate records
	  CHAR    countryCode[2];               //  PCQUOTE standard code
	  USHORT  unused1;                      //  = 0
	  CHAR    unused2[12];                  //  blanks
	  USHORT  interestType;                 //  indicates category of data:
	  //     0  =  cd rates
	  //     1-255 reserved
	  CHAR    unused3[12];                  //  blanks
	  //  portion of ticker symbol for options
	  //  blanks for class header
	  USHORT  unused4;                      //  = 0
  }  PCQT1111_KEY_0_100;


 
   typedef struct  _PCQT1111_KEY_0_101

   {
	   USHORT  dataType;                     //  data type = 101 for
	   //  calendars
	   
	   CHAR    countryCode[2];               //  PCQUOTE standard code
	   
	   USHORT  calendarType;                 //  indicates type of calendar
	   //     0  =  option monthly expirations
	   //     1  =  option weekly expirations
	   //    10  =  bank holiday table
	   CHAR    unused1[12];                  //  blanks
	   
	   
	   
		USHORT  calendarCategory;             //  indicates type of calendar                
		//     for monthly expirations                
		//      1  =  equities and indexes            
		//      2  =  currencies                      
		//      3  =  end of month currencies         
		//      4  =  end of quarter indexes
		//		5  =  open settled indexes
		//  6 - 8     reserved for future             
		//                                            
		//     for weekly expiration                  
		//                                            
		//      1  =  monday expirations              
		//  2 - 5  =  tuesday - friday                
		//                                            
		//      for holiday calendars                 
		//                                            
		//         = 0                                
	   
		CHAR    unused2[12];                  //   blanks
	   
	   
		USHORT  calendarSequence;			
											//	for weekly expiration indecates week of the month
											//		1 = first week of month, etc
											//	for monthly expiration, indicates year offset
	}	PCQT1111_KEY_0_101;



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//       DATA RECORD LAYOUTS                                                  //
//                                                                            //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//
//      GENERIC RECORD LAYOUT

  typedef struct     _PCQT1111_RECORD
  {
	  PCQT1111_KEY_DATA	key;						//  record key data
	  CHAR              data[212];					//  data fields
	  DATE_1111			updateDate;					//  date of last update
	  ULONG				updateFlag;					//  update flags
													//  note: these last two fields  
													//  are used internally by
													//  PCQUOTE and should not be 
													//  modified    
}	PCQT1111_RECORD;   




////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//       OPTION DATA RECORD LAYOUTS  (  data type 0 )                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

//                                
//      CLASS HEADER    ( item type 0 -- record category 0 ) 
//                                
                                  
typedef struct	_CLASS_HEADER   
{                               
	PCQT1111_KEY_DATA	key;						//  record key
	  
	  
	CHAR				desc[36];					//  description
	CHAR				currCode[3];				//  PCQUOTE standard
													//  currency code
	BYTE				session;					//  PCQUOTE standard trading session number
	LONG				intrAdj;					//  risk free interest
													//  adjustment factor
													//  normally dividend yield for
													//  indexes used to simulate
													//  continuous diviend effect
	USHORT				intrFormat;					//  format of interest using
													//  PCQUOTE standard price types
	USHORT				numSymbols;					//	number of symbol definition record used 
													//	(record categories 10 - 59 )
}	CLASS_HEADER;


//
//      SYMBOL DEFINITION   ( item type 0 -- record category 10 - 59 )
//


typedef struct     _SYMBOL_DEF
{
	PCQT1111_KEY_DATA key;							//  record key
	  
	USHORT				expCalendarCat;				//  record category of expiration
													//  calander appropriate to class
	USHORT				expCalendarType;				//  record item type of expiration
													//  calendar
	  
	USHORT				exerciseStyle;				//  0 = American
													//  1 = European
													//  all other values reserved for
													//  future requirements
	  
	  
	USHORT				strikePrcType;				//  PCQUOTE standrd price type code
													//  for entries in strike price table
	  
	ULONG				strikeTable[26];			//  table of strike prices corresponding
													//  to 26 letter codes in option ticker
													//  symbol
	  
	  ULONG             contractSize;				//  number of underlying units in option
													//  contract (note: for US equities and
													//  indexes this value is normally 100
													//  and the underlying shares is also 100.
													//  This means each contract represents 100
													//  units with each unit being 1 share of
													//  stock or 1 times the index.  Option
													//  prices are quoted per unit, therefore
													//  contract price = options price x units.
													//  The same is true for exercise price
													//  and underlying price.
													//  The units ( and underlying shares ) are
													//  often adjusted as the result of stock 
													//  splits, distributions, mergers,
													//  aquisitions, etc.
	  
	  
	union											//  table of PCQUOTE standard exchange codes
	{                     
		CHAR            primaryExch[2];			//  exchange code of
													//  primary exchange
		CHAR			exchCode[8][2];				//  exchange code of
													//	each exchange on
													//  which option trades ( up to 8 )
	}  exchangeTable;
	  
	  
	struct											//  ordered table of option expirations for symbol
	{                               
		BYTE            expYear;					//  expiration year offset
													//  represents years from current
													//  cycle year ( first 12 expiration
													//  months
													//  i.e options expiring within year
													//  would have 0, one year leaps would
													//  have 1, etc.
		  
		BYTE           expMonth;					//  expiration month
	}	cycleTable[12];								//  12 expirations in cycle
	  
	ULONG				cashSettlement;				//  cash amount for settlement of
													//  exercise or assignment.  amount
													//  is per entire contract, therefore
													//  to use as component of underlying
													//  price, this amount must be divided
													//  by contract units (contractSize)
													//  (note: value is stored as same
													//  price type as strike prices)
	  
	DBA_ITEM_1111		primaryUnd;					//  PCQUOTE standard DBA key for primary
													//  underlying security this option
													//  symbol
	  
	  
	  
	DBA_ITEM_1111		altUnd;						//  PCQUOTE standard DBA key for alternate
													//  underlying security this option
													//  symbol.  The alternate underlying is
													//  used to evaluate the open settled indexes
													//  when they cease trading on the day prior to
													//  expiration.  The symbol is the ticker symbol
													//  disemminated by the exchanges with the
													//  "open settlement" value
	  
	  
	ULONG				primaryShares;				//  shares per contract of primary 
													//  underlying
	  
	ULONG				altShares;					//  shares per contract of alternate
													//  underlying( usually the same as
													//  primary but who knows ????
	  
	USHORT				numAddUnd;					//  number of additional underlying records
													//  if non-zero additioal underlying record(s)
													//  exists as data type 2
	  
	  
	USHORT				numStrikeExcpt;				//  number of strike exception records
													//  if non-zero strike exceprtion record(s)
													//  exists as data type 1
}	SYMBOL_DEF;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//                                                                            //
//       ADDITIONAL OPTION DATA RECORD LAYOUTS  (  data type 1, 2 )           //
//         ( optional records used in classes where additional                //
//                       data is required )                                   //
//                                                                            //                                                                           
////////////////////////////////////////////////////////////////////////////////


//
//      STRIKE EXCEPTION RECORD   
//      ( data type 1 -- item type 0 -- record category 10 - 59)
//
//      this optional record(s) defines individual option
//      ticker symbols whose strike prices have
//      been redined in a way that differs form the normal
//      values difined in the strike price table in
//      the symbol definition record.
//
//      It (they) will have the same key values as the
//      symbol definition record to which ite applies, 
//      except data  type will equal 1
//


typedef struct      _STRIKE_EXCPT
{
	PCQT1111_KEY_DATA	key;						// record key
	USHORT				numExcpt;					//  number of entries in table this record
	USHORT				filler;						//  reserved = 0
	  
	struct  
	{
		CHAR			expCode;					//  expiration code in ticker symbol
		CHAR			strikeCode;					//  strike price code in ticker symbol
		CHAR			reserved[2];				//  reserved blank
		  
		LONG			strikeExcpt;				//  exceptional strike price ( format
													//  conforms to price type in symbol
													//  defition record
	}					ExcptTbl[26];				//  26 table entries per record
}	STRIKE_EXCPT;


//
//      ADDITIONAL UNDERLYING RECORD
//      ( data type 2 -- item type 0 -- record category 10 - 59)
//
//      this is an optional record(s) which will be created
//      for option symbols which settle for multiple underlying
//      securites.  
//
//      It (they) will have the same key values as the
//      symbol definition record to which the apply, excepty data 
//      type will equal 2
//


typedef struct	_ADDITIONAL_UND
{
	PCQT1111_KEY_DATA	key;						//  record key
	USHORT				numUnd;						//  number of underlyings listed in
													//  in this record
	USHORT				filler;						//  reserved = 0
	  
	struct  
	{
		DBA_ITEM_1111	undItem;					//  PCQUOTE standard DBA key for
													//  underlying security
		USHORT			filler;						//  reserved = 0
		ULONG           undShares;					//  shares per contract of this
													//  security
	}	undTable[8];								//  allow eight per record
}	ADDITIONAL_UND;

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

typedef struct     _UNDERLYING_SEC
{
	PCQT1111_KEY_DATA	key;						//  record key
	CHAR				undDes[36];					//  security description
	USHORT				divFreq;					//  dividend payment frequency in times per year
													//  i.e  2 = semiannual, 4 = quarterly, 12 = monthly
	USHORT            divFmt;						//  format of dividend amounts using PCQUOTE
													//  standard price types
	DATE_1111         exDivDate;					//  nest ex-dividend date if announced, or last
													//  if not announced ( zero if no dividends )
	LONG              divAmt;						//  amount of dividend
	  
	struct
	{
		DATE_1111		spDivExDate;				//  ex-date of special (irregular) dividend
		USHORT			divType;					//  type of dividend;
													//    0  = cash
													//    1-   reserved for future use
		USHORT			spDivFmt;					//  format of dividend amount
		LONG			spDivAmt;					//  amount of dividend
	}					specialDiv[MAX_SPC_DIV_1111];				//  table of up to 4 special dividends
	  
	LONG				volatility;					//  52 week historical volatility
	CHAR				primaryExch[2];				//  PCQUOTE standard exchange code for
													//  primary trading exchange of underlying
	CHAR				prmOptClsCCD[2];			// class for which this is primary 
	CHAR				prmOptCls[12];				// underlying
	USHORT				numClasses;					// number of additional options classes this
													// this security underlies
	struct 
	{
		CHAR			optClsCCD[2];				// country code of class
		CHAR			optCls[12];					//  class symbol
	}					optClsTbl[3];				// table of additional classes ( up to 3 )
}						UNDERLYING_SEC;


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
//

typedef struct	_INTEREST_RATE
{	  
	PCQT1111_KEY_DATA key;							// record key
	USHORT				rateFmt;					//  data format using PCQUOTE standard price type
	USHORT				filler;						//  reserved = 0
	  
	struct  
	{
		LONG			cdRate;						//  interest rates on $100,000 cd
	}					cdRateTbl[5];				//  5 table entries for 30, 60, 90, 180, 360 days
	  
	LONG				brokerCall;					//  broker call rate
	LONG				shortStock;					//  short stock rebate rate
}	INTEREST_RATE;


//
//      CALENDARS     (  data type 101 )
//  
//

typedef struct	_CALENDAR
{
	PCQT1111_KEY_DATA key;							// record key
	struct
	{
		DATE_1111	    calendarDate[24];			//  24 date entries
	}	calendar;									//  
}	CALENDAR;


///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//      RECORD AND KEY BUFFER UNIONS                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////


typedef  union _PCQT1111_KEY

{
    PCQT1111_KEY_DATA   keyData;					//
    PCQT1111_KEY_0      primaryKey;					//
    PCQT1111_KEY_1      symbolKey;					//
    PCQT1111_KEY_2      underlyingKey;				//
    PCQT1111_KEY_3      classTypeKey;				//
    PCQT1111_KEY_0_100  interestKey;				//
    PCQT1111_KEY_0_101  calendarKey;				//
}	PCQT1111_KEY, * lpPCQT1111_KEY;					//



typedef  union   _PCQT1111
{  
	PCQT1111_RECORD		rec1111;				//    generic PCQT1111 record
	CLASS_HEADER		classHd;				//    class header
	SYMBOL_DEF			symDef;					//    symbol definition
	STRIKE_EXCPT		strkExcpt;				//    strike price exceptions
	ADDITIONAL_UND		addUnd;					//    additional underlying items`
	UNDERLYING_SEC		undSec;					//    underlying security data
	INTEREST_RATE		intRate;				//    interest rates
	CALENDAR			calendar;				//    calendars
}	PCQT1111, * lpPCQT1111;


#ifndef INCL_NOPACKING
	#pragma pack()
#endif

#endif
