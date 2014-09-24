// Vickers.h
// Vickers Header File 

#if !defined(_VICKERS_H)
#define	_VICKERS_H

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

// vickers server errors returned in status field of response
#define VICKERR_SUCCESS				0		// successful call
#define VICKERR_DB_OPEN				1		// vickers diskDB open error
#define VICKERR_INVALID_KEY			2		// invalid key number specified
#define VICKERR_INVALID_FUNC		3		// invalid function specified
#define VICKERR_INVALID_MSG_TYPE	4		// invalid message type ('v' '?')
#define VICKERR_KEY_NOT_FOUND		5		// key not found in database
#define VICKERR_SERVER_BUSY			6		// server could not process request at this time
#define VICKERR_SERVER_SHUTDOWN		7		// server could not process request, shutting down
#define VICKERR_NOT_LOGGED_IN		8		// server could not process request, workstation not logged in
#define VICKERR_SUBCOUNT			9		// invalid sub count specified

// vickers key types
#define VKT_SYM_INSTNUM			0
#define VKT_SYM_CURHOLD			1
#define VKT_INST_CURHOLD		2
#define VKT_CUSIP_CURHOLD		3
#define VKT_SYM_NETHOLD			4
#define VKT_INST_NETHOLD		5
#define VKT_CUSIP_NETHOLD		6
#define VKT_MANG_CURHOLD		7
#define VKT_MANG_NETHOLD		8
#define VKT_INST_NAME_NUM		9
#define VKT_INST_NETAMOUNT		10
#define VKT_INST_CURAMOUNT		11
#define VKT_MAX					VKT_INST_CURAMOUNT

// returned record types
#define VRT_DB					0
#define VRT_TICKER_INSTS		1
#define VRT_INST_TICKERS		2
#define VRT_INST_AMOUNT			3

// summary search directions
#define VICK_SRCH_FORWARD		0
#define VICK_SRCH_BACKWARD		1

// maximum vickers summaries
#define MAX_VICK_SUMS			8

// maximum value stored in the value field (28 bits)
#define MAX_VALUE_EXP	268435456
#define MIN_VALUE_EXP	-268435456
// ===========================================================================
// value field with exponent of 10
// structure to compress up to 15 digit positive and 14 digit negative numbers
typedef	struct tagVALUE_EXP
{
	DWORD	value:  28,		// Lower  28 bits = value
			exponent:3,		// Middle 3 bits = exponent of 10 (ie. how many zeros should be tagged to the end)
			sign:    1;		// Upper 1 bit = sign ( 0 == positive or zero, 1 == negative

} VALUE_EXP, *LPVALUE_EXP;

// maximum value stored in the value field (27 bits)
#define MAX_VALUE_DEC	134217728

// ===========================================================================
// value field with implied decimals
// structure to compress up to 8 floating point digits
typedef	struct tagVALUE_DEC
{
	DWORD	value:	27,		// Lower  27 bits = value
			dec:	4,		// Middle 4 bits = decimal places
			sign:	1;		// Upper 1 bit = sign ( 0 == positive or zero, 1 == negative
} VALUE_DEC, *LPVALUE_DEC;

// ===========================================================================
// compressed date field -- Day, Month, Year
typedef	struct _COMP_DATE
{
	WORD	day:  5,		// Lower  5 bits = day   (1 - 31)
			month:4,		// Middle 4 bits = month (1 - 12)
			year: 7;		// Upper  7 bits = year	 (0 - 99) NOTE: Offset from 1970
} COMP_DATE, *LPCOMP_DATE;

typedef struct tagVK_SYM_INSTNUM	// unique key
{
	char		sTickerSymbol[6];	// ticker symbol (2)
	DWORD		dwInstNum;			// institution number (3)
} VK_SYM_INSTNUM, *LPVK_SYM_INSTNUM;

typedef struct tagVK_SYM_HOLDINGS	// dup key
{
	char		sTickerSymbol[6];	// ticker symbol (2)
	VALUE_EXP	veHoldings;			// current or net holdings (3)
} VK_SYM_HOLDINGS, *LPVK_SYM_HOLDINGS;

typedef struct tagVK_INST_HOLDINGS	// dup key
{
	DWORD		dwInstNum;			// institution number (3)
	VALUE_EXP	veHoldings;			// current or net holdings (3)
} VK_INST_HOLDINGS, *LPVK_INST_HOLDINGS;

typedef struct tagVK_INST_AMOUNT
{
	DWORD		dwInstNum;
	VALUE_EXP	veAmount;			// current or net amount
} VK_INST_AMOUNT, *LPVK_INST_AMOUNT;

typedef struct tagVK_CUSIP_HOLDINGS	// dup key
{
	char		sCUSIP[9];			// CUSIP (2)
	VALUE_EXP	veHoldings;			// current or net holdings (3)
} VK_CUSIP_HOLDINGS, *LPVK_CUSIP_HOLDINGS;

// this key is only used for institution name/number summary messages
typedef struct tagVK_INSTNAME_INSTNUM	// unique key in ihead.ddb
{
	char		sInstName[58];		// institution name (1)
	DWORD		dwInstNum;			// institution number (3)
} VK_INSTNAME_INSTNUM, *LPVK_INSTNAME_INSTNUM;

typedef struct tagVK_MANG_HOLDINGS	// dup key
{
	char		sTickerSymbol[6];	// ticker symbol (2)
	DWORD		dwMangNum;			// management number (1)
	VALUE_EXP	veHoldings;			// current or net holdings (3)
} VK_MANG_HOLDINGS, *LPVK_MANG_HOLDINGS;

// for accessing the Insider Holdings database
typedef struct tagVK_INSIDER_HOLDINGS
{
	char		sTickerSymbol[6];
	char		sFilersName[22];
	char		cTransType;				// 0 = Buy, 1 = Sell, 2 = Private Buy, 3 = Private Sell, 4 = Non Open Market Trade Buy, 5 = Non Open Market Trade Sell
	VALUE_EXP	veSharesTraded;
	DWORD		dwUniqueField;			// filled in by disk DB
} VK_INSIDER_HOLDINGS, *LPVK_INSIDER_HOLDINGS;

typedef struct tagVK_INSIDER_CUSIP
{
	char		sCusip[9];
	DWORD		dwUniqueField;			// filled in by disk DB
} VK_INSIDER_CUSIP, *LPVK_INSIDER_CUSIP;

typedef struct tagVK_INSIDER_ISSUER
{
	char		sIssuersName[28];
	DWORD		dwUniqueField;			// filled in by disk DB
} VK_INSIDER_ISSUER, *LPVK_INSIDER_ISSUER;

// for accessing the 5percent Holdings database
typedef struct tagVK_FIVEPERCENT_HOLDINGS
{
	char		sTickerSymbol[6];
	char		sFilersName[40];
	char		cTransType;			
	char		sPercentSharesOut[4];	
	VALUE_EXP	veSharesTraded;
	DWORD		dwUniqueField;			
} VK_FIVEPERCENT_HOLDINGS, *LPVK_FIVEPERCENT_HOLDINGS;

typedef struct tagVK_FIVEMORE_HOLDINGS	// unique key
{
	char		sTickerSymbol[6];	
	char        sFilersName[40];			
} VK_FIVEMORE_HOLDINGS, *LPVK_FIVEMORE_HOLDINGS;


typedef struct tagVK_FIVEPERCENT_CUSIP
{
	char		sCUSIP[9];
	DWORD		dwUniqueField;		
} VK_FIVEPERCENT_CUSIP, *LPVK_FIVEPERCENT_CUSIP;

typedef struct tagVK_FIVEPERCENT_COMPANY
{
	char		sCompanyName[40];
	DWORD		dwUniqueField;			
} VK_FIVEPERCENT_COMPANY, *LPVK_FIVEPERCENT_COMPANY;

typedef union tagVICK_KEYS
{
	VK_SYM_INSTNUM			symInstnum;		// unique key
	VK_SYM_HOLDINGS			symHoldings;	// symbol - holdings (cur or net)
	VK_INST_HOLDINGS		instHoldings;	// inst num - holdings (cur or net)
	VK_INST_AMOUNT			instAmount;		// inst num - amount(cur or net)
	VK_CUSIP_HOLDINGS		cusipHoldings;	// cusip - holdings (cur or net)
	VK_INSTNAME_INSTNUM		instNameNumber; // this key is for accessing a seperate inst name / inst num database
	VK_MANG_HOLDINGS		mangHoldings;	// management number - holdings (cur or net)
	VK_INSIDER_HOLDINGS		insiderHoldings;// insider holdings key
	VK_INSIDER_CUSIP		insiderCusip;	// insider cusip key
	VK_INSIDER_ISSUER		insiderIssuer;	// insider issuer key
	VK_FIVEPERCENT_HOLDINGS	fivepercentHoldings;// fivepercent holdings key
	VK_FIVEPERCENT_CUSIP	fivepercentCusip;	// fivepercent cusip key
	VK_FIVEPERCENT_COMPANY	fivepercentCompany;	// fivepercent company key
	VK_FIVEMORE_HOLDINGS	fivemoreHoldings;// fivemore holdings key
	UCHAR					key[128];		// general purpose member to access this in a generic way	
} VICK_KEYS, *LPVICK_KEYS;					// NOTE: key must be at least 4 bytes larger than biggest key

// definition for the final diskDB record
// comment codes indicate which file the data came from:
//	(1) = Inst Header File, (2) = Security File, (3) Inst Holdings File
typedef struct tagVICK_DB_RECORD
{
	// Security File Data
	char		sTickerSymbol[6];	// ticker symbol (2)
	char		sCUSIP[9];			// CUSIP (2)
	char		sSecurityName[60];	// security name (2)
	char		cStockExchange;		// stock exchange (see "stock exchange codes document" ) (2)
	WORD		wIndustryCode;		// industry code (SIC?) (2)
	char		cSecurityType;		// security type 1 Common Stock, 2 Preferred, 3 Warrants, 4 Convertible Bonds, 7 Open End Mutual Funds, 8 Bonds (2)
	char		cSecurityLocal;		// (D)omestic, (C)andian, (F)oreign (2)		**80
	VALUE_DEC	vdPrice;			// security price (2)						**84
	
	DWORD		dwInstNum;			// institution number (3)					** 88
	VALUE_EXP	veCurHoldings;		// current holdings (3)
	VALUE_EXP	vePrevHoldings;		// previous holdings (3)
	VALUE_EXP	veDiffHoldings;		// difference in holdings (we made this up)
	VALUE_EXP	veCurAmount;		// current holdings amount
	VALUE_EXP	vePrevAmount;		// previous holdings amount
	VALUE_EXP	veDiffAmount;		// difference in holdings amount (we made this up)
	COMP_DATE	cdCurReportDate;	// current report date (3)
	COMP_DATE	cdPrevReportDate;	// privious report date (3)					**100
	
	DWORD		dwMangNum;			// management number (1)	
	COMP_DATE	cdMaturityDate;		// maturiy date (bonds only) (2)			**106
	char		sInstName[58];		// institution name (1)						**164
	char		cInstType;			// 1 Other, 2 Pension, 3 Foundation, 4 Investment Co, 5 Insurance, 7 College, 8 Bank (1)
	char		cInstLocal;			// (D)omestic, (C)anadian, (F)oreign (1)
}VICK_DB_RECORD, *LPVICK_DB_RECORD;

typedef struct tagVICK_HEADER_TICKER_FIELDS
{
	char		sTickerSymbol[6];	// ticker symbol (2)
	char		sCUSIP[9];			// CUSIP (2)
	char		sSecurityName[60];	// security name (2)
	char		cStockExchange;		// stock exchange (see "stock exchange codes document" ) (2)
	WORD		wIndustryCode;		// industry code (SIC?) (2)
	char		cSecurityType;		// security type 1 Common Stock, 2 Preferred, 3 Warrants, 4 Convertible Bonds, 7 Open End Mutual Funds, 8 Bonds (2)
	char		cSecurityLocal;		// (D)omestic, (C)andian, (F)oreign (2)
	VALUE_DEC	vdPrice;			// security price (2)
	COMP_DATE	cdMaturityDate;		// maturiy date (bonds only) (2)
} VICK_HEADER_TICKER_FIELDS, *LPVICK_HEADER_TICKER_FIELDS; // 86 bytes

typedef struct tagVICK_LIST_INST_FIELDS
{
	DWORD		dwInstNum;			// institution number (3)
	VALUE_EXP	veCurHoldings;		// current holdings (3)
	VALUE_EXP	vePrevHoldings;		// previous holdings (3)
	VALUE_EXP	veDiffHoldings;		// difference in holdings (we made this up)
	COMP_DATE	cdCurReportDate;	// current report date (3)
	COMP_DATE	cdPrevReportDate;	// privious report date (3)
	DWORD		dwMangNum;			// management number (1)	
	char		sInstName[58];		// institution name (1)
	char		cInstType;			// 1 Other, 2 Pension, 3 Foundation, 4 Investment Co, 5 Insurance, 7 College, 8 Bank (1)
	char		cInstLocal;			// (D)omestic, (C)anadian, (F)oreign (1)
} VICK_LIST_INST_FIELDS, *LPVICK_LIST_INST_FIELDS;  // 84 bytes

typedef struct tagVICK_HEADER_INST_FIELDS
{
	DWORD		dwInstNum;			// institution number (3)
	DWORD		dwMangNum;			// management number (1)	
	char		sInstName[58];		// institution name (1)
	char		cInstType;			// 1 Other, 2 Pension, 3 Foundation, 4 Investment Co, 5 Insurance, 7 College, 8 Bank (1)
	char		cInstLocal;			// (D)omestic, (C)anadian, (F)oreign (1)
} VICK_HEADER_INST_FIELDS, *LPVICK_HEADER_INST_FIELDS;  // 64 bytes

typedef struct tagVICK_LIST_AMOUNT_FIELDS
{
	VALUE_EXP	veCurAmount;		// current holdings amount
	VALUE_EXP	vePrevAmount;		// previous holdings amount
	VALUE_EXP	veDiffAmount;		// difference in holdings amount(we made this up)
	COMP_DATE	cdCurReportDate;	// current report date (3)
	COMP_DATE	cdPrevReportDate;	// privious report date (3)
	char		sTickerSymbol[6];	// ticker symbol (2)
	char		sSecurityName[60];	// security name (2)
} VICK_LIST_AMOUNT_FIELDS, *LPVICK_LIST_AMOUNT_FIELDS; //82 bytes


typedef struct tagVICK_LIST_TICKER_FIELDS
{
	VALUE_EXP	veCurHoldings;		// current holdings (3)
	VALUE_EXP	vePrevHoldings;		// previous holdings (3)
	VALUE_EXP	veDiffHoldings;		// difference in holdings (we made this up)
	COMP_DATE	cdCurReportDate;	// current report date (3)
	COMP_DATE	cdPrevReportDate;	// privious report date (3)
	char		sTickerSymbol[6];	// ticker symbol (2)
	char		sSecurityName[60];	// security name (2)
} VICK_LIST_TICKER_FIELDS, *LPVICK_LIST_TICKER_FIELDS; // 82 bytes

#define MAX_VICK_SUBS	5

typedef struct tagVICK_TICKER_INSTS_RECORD
{
	DWORD						dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	DWORD						dwLastDupKeyField;		// this is the hidden segment that makes every dup key unique... we use it here for quick access
	VICK_HEADER_TICKER_FIELDS	vickHeaderTickerFields;
	VICK_LIST_INST_FIELDS		vickListInstFields[MAX_VICK_SUBS];
} VICK_TICKER_INSTS_RECORD, *LPVICK_TICKER_INSTS_RECORD;// 492 bytes

typedef struct tagVICK_INST_TICKERS_RECORD
{
	DWORD						dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	DWORD						dwLastDupKeyField;		// this is the hidden segment that makes every dup key unique... we use it here for quick access
	VICK_HEADER_INST_FIELDS		vickHeaderInstFields;
	VICK_LIST_TICKER_FIELDS		vickListTickerFields[MAX_VICK_SUBS];
} VICK_INST_TICKERS_RECORD, *LPVICK_INST_TICKERS_RECORD; // 474 bytes

typedef struct tagVICK_INST_AMOUNT_RECORD
{
	DWORD						dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	DWORD						dwLastDupKeyField;		// this is the hidden segment that makes every dup key unique... we use it here for quick access
	VICK_HEADER_INST_FIELDS		vickHeaderInstFields;
	VICK_LIST_AMOUNT_FIELDS		vickListAmountFields[MAX_VICK_SUBS];
} VICK_INST_AMOUNT_RECORD, *LPVICK_INST_AMOUNT_RECORD;   

typedef union tagVICK_RECORD
{
	VICK_DB_RECORD				vickDdbRecord;			// one whole diskDB record 
	VICK_TICKER_INSTS_RECORD	vickTickerInsts;		// one ticker and n insts
	VICK_INST_TICKERS_RECORD	vickInstTickers;		// one inst and n tickers, show by share
	VICK_INST_AMOUNT_RECORD		vickInstAmount;			// one inst and n tickers, show by dollar amount
} VICK_RECORD, *LPVICK_RECORD;


// this is only used for institution cusip/symbol summary messages
typedef struct tagCUSIP_SYM
{
	char		sCUSIP[9];			// CUSIP (2)
	char		sTickerSymbol[6];	// ticker symbol (2)
} CUSIP_SYM, *LPCUSIP_SYM;

typedef struct tagISSUE_SYM
{
	char		sIssuersName[28];
	char		sTickerSymbol[6];
} ISSUE_SYM, *LPISSUE_SYM;

typedef struct tagCOMPANY_SYM
{
	char		sCompanyName[40];
	char		sTickerSymbol[6];
} COMPANY_SYM, *LPCOMPANY_SYM;


typedef union tagVICK_SUMMARY_RECORD
{
	char					vsTickerSymbols[MAX_VICK_SUMS][6];
	VK_INSTNAME_INSTNUM		vsInsts[MAX_VICK_SUMS];
	CUSIP_SYM				vsCusipSyms[MAX_VICK_SUMS];
	ISSUE_SYM				vsIssueSyms[MAX_VICK_SUMS];
	COMPANY_SYM				vsCompanySyms[MAX_VICK_SUMS];
}VICK_SUMMARY_RECORD, *LPVICK_SUMMARY_RECORD;

// definition for the final diskDB record for ihead.ddb
typedef struct tagIHEAD_DB_RECORD
{
	DWORD				dwInstNum;
	char				sInstName[58];
	DWORD				dwMangNum;
	char				cInstType;	// 1 Other, 2 Pension, 3 Foundation, 4 Investment Co, 5 Insurance, 7 College, 8 Bank
	char				cInstLocal;	// (D)omestic, (C)anadian, (F)oreign
}IHEAD_DB_RECORD, *LPIHEAD_DB_RECORD;

// definition for the final diskDB record for scount.ddb and icount.ddb
typedef struct tagVICK_STOCK_COUNT_RECORD
{
	char	sTickerSymbol[6];
	DWORD	dwTotalCount;			// total number of records for this stock (parent and children)
	DWORD	dwParentCount;			// only parent company record count
}VICK_STOCK_COUNT_RECORD, *LPVICK_STOCK_COUNT_RECORD;

typedef struct tagVICK_INST_COUNT_RECORD
{
	DWORD	dwInstNum;
	DWORD	dwTotalCount;			// total number of records for this inst
}VICK_INST_COUNT_RECORD, *LPVICK_INST_COUNT_RECORD;

////////////////////////////////////////////////
// FORM 4 & 5 INSIDER HOLDINGS DATA ACCESS BELOW
////////////////////////////////////////////////

// definition for the disk db output file record
typedef struct tagVICK_INSIDER_DB_RECORD
{
	DWORD		dwUniqueField;			// internal unique ID for the database	
	DWORD		dwFilersID;
	VALUE_EXP	veSharesTraded;
	VALUE_EXP	veSharesHeldAfterTrans;
	VALUE_DEC	vdShareFromPrice;		// share price (from field)
	VALUE_DEC	vdShareToPrice;			// share price (to field)
	COMP_DATE	cdDateEntered;
	COMP_DATE	cdTransFromDate;		// transaction date (from)
	COMP_DATE	cdTransToDate;			// transaction date (to)
	char		sTickerSymbol[6];
	char		sIssuersName[28];
	char		sCUSIP[9];
	char		sFilersName[22];
	char		sFilersRelationship[6];
	char		sIssueType[20];
	char		cTransType;				// 0 = Buy, 1 = Sell, 2 = Private Buy, 3 = Private Sell, 4 = Non Open Market Trade Buy, 5 = Non-Open Market Trade Sell
	char		cSharesOwnedType;		// 0 = Direct, 1 = Indirect, 2 = Combined Direct and Indirect
	char		cStockExchange;			// 1 = NYSE, 2 = AMEX, 99 = NASDAQ
}VICK_INSIDER_DB_RECORD, *LPVICK_INSIDER_DB_RECORD;	// *** 128 bytes (can fit 4 in one packet)

typedef struct tagFIVE_PERCENT_DB_RECORD
{
	DWORD   	dwUniqueField;          //Unique key
	char		sTickerSymbol[6];
	COMP_DATE	cdTransFromDate;		// YYMMDD
	COMP_DATE	cdTransToDate;			// YYMMDD
	char		sFilersName[40];
	char		sCompanyName[40];
	VALUE_EXP	veSharesTraded;	
	char		cTransType;				// 'B' = BUY, 'S' = SELL
	VALUE_EXP	veSharesAfterTrade;	// shares held after trade
	char		sPercentSharesOut[4];	// percent of shares outstanding (two decimal places)
	char		sCUSIP[9];

}FIVE_PERCENT_DB_RECORD, *LPFIVE_PERCENT_DB_RECORD;

typedef struct tagFIVE_MORE_DB_RECORD
{
	char    sTickerSymbol[6];
	char	sFilersName[40];
	char	sFilersAttName[40];		// filers attention name
	char	sFilersTitle[30];
	char	sFilersAddress[40];
	char	sFilersCity[25];
	char	sFilersState[15];
	char	sFilersZipCode[9];
	char	sFilersTeleNum[10];

}FIVE_MORE_DB_RECORD, *LPFIVE_MORE_DB_RECORD;

typedef struct tagVICK_INSIDERS_RECORD
{
	DWORD					dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	DWORD					dwLastDupKeyField;		// this is the hidden segment that makes every dup key unique... we use it here for quick access
	VICK_INSIDER_DB_RECORD	vickInsidersDBRecord[4];
} VICK_INSIDERS_RECORD, *LPVICK_INSIDERS_RECORD;

typedef struct tagVICK_INSIDER_COUNT_RECORD
{
	char	sTickerSymbol[6];
	DWORD	dwTotalCount;
} VICK_INSIDER_COUNT_RECORD, *LPVICK_INSIDER_COUNT_RECORD;

typedef struct tagVICK_FIVEPERCENT_RECORD
{
	DWORD							dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	DWORD							dwLastDupKeyField;		// this is the hidden segment that makes every dup key unique... we use it here for quick access
	FIVE_PERCENT_DB_RECORD		vickFivePercentDBRecord[4];
} VICK_FIVEPERCENT_RECORD, *LPVICK_FIVEPERCENT_RECORD;

typedef struct tagVICK_FIVEMORE_RECORD
{
	DWORD					dwTotalSubCount;		// filled in on first request (block 0) with total number of subrecords
	FIVE_MORE_DB_RECORD		vickFiveMoreDBRecord;
} VICK_FIVEMORE_RECORD, *LPVICK_FIVEMORE_RECORD;



typedef struct tagVICK_FIVEPERCENT_COUNT_RECORD
{
	char	sTickerSymbol[6];
	DWORD	dwTotalCount;
} VICK_FIVEPERCENT_COUNT_RECORD, *LPVICK_FIVEPERCENT_COUNT_RECORD;
#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif


