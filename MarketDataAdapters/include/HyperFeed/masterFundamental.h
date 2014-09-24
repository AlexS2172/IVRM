////////////////////////////////////////////////////////
//
//    Fundamental data
//      PCQT3060
//
////////////////////////////////////////////////////////

#ifndef __PCQT3060_INCL__
#define __PCQT3060_INCL__

#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

#define MAX_MF_DIVHIST			16
#define MAX_MF_SPDIV			16
#define MAX_MF_DIVHIST_RECS		4
#define MAX_MF_SPDIV_RECS		4
#define MAX_MF_ISSUE_ID_RECS	32

#define FUND_BASIC				0
#define FUND_PRICE_HIST			1
#define FUND_EARNINGS			2
#define FUND_DIVIDEND			3
#define FUND_ADD_SPC_DIV		4
#define FUND_HIST_DIV			5
#define FUND_BOND				6
#define FUND_MM_FUND			8
#define FUND_ISSUE_ID			9

#define MAX_MF_TYPES			32

typedef struct _F_DATE
{
	BYTE	century;
	BYTE	year;
	BYTE	month;
	BYTE	day;
} F_DATE, *lpF_DATE;

typedef union _FUND_DATE
{
	F_DATE	date;
	ULONG	lDate;
} FUND_DATE;

// Dividend types

#define DV_TYPE_NONE			0
#define DV_TYPE_CASH			1
#define DV_TYPE_STOCK			2  
#define DV_TYPE_SPLIT			3
#define DV_TYPE_OTHER_STOCK		4
#define DV_TYPE_RIGHTS			5
#define DV_TYPE_WARRANTS		6
#define DV_TYPE_OTHER_CURR		7
#define DV_TYPE_ISSUE_CALLED	8
#define DV_OMITTED				9
#define DV_STOCKHOLDER_MEETING	10
#define DV_TYPE_STOCK_EXCH		11  

// Supplemental dividend types

// Reinvestment codes

// Tax staus

// Tax base 

typedef struct _FUND_DIV_DATA
{
	SHORT		type;					// dividend type
	SHORT		fmt;					// format ( number of decimal places )
	LONG		amount;					// amount/rate
	FUND_DATE	exDate;					// exdividend date
	FUND_DATE	payDate;				// payment date	
	FUND_DATE	recDate;				// record date
	FUND_DATE	decDate;				// declaration date
	SHORT		suppTypeCode[2];
	CHAR		currCode[3];			// currency of cash dividends
	BYTE		taxStatus;				// tax status code
	BYTE		taxBaseCode;
	BYTE		DTC_ReInvest;			// DTC reinvestment eligibility 0 no, 1 yes
	BYTE		reInvest;				// reinvestment code    
	BYTE		reserved;
} FUND_DIV_DATA, *lpFUND_DIVD_DATA;

#define FUND_KEYNUM_PRIMARY	0
#define FUND_KEYNUM_TYPE	1

typedef struct _FUND_KEY_0
{
	TICKER_KEY	item;
	SHORT		type;  
	SHORT		seq;             
} FUND_KEY_0, *lpFUND_KEY_0;

typedef struct _FUND_KEY_1
{
	SHORT		type;
	TICKER_KEY	item;
	SHORT		seq;
} FUND_KEY_1, *lpFUND_KEY_1;

typedef union _MASTER_FUNDAMENTAL_KEY
{
	FUND_KEY_0	primaryKey;
	FUND_KEY_1	typeKey;
} MASTER_FUNDAMENTAL_KEY, *lpMASTER_FUNDAMENTAL_KEY;

typedef struct _MASTER_FUND_UPD_BITS_ALL
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	deleted				:1;
	BIT_FIELD	changed				:1;
	BIT_FIELD	split				:1;
	BIT_FIELD	divHist				:1;
	BIT_FIELD	reserved			:12;
	BIT_FIELD	reserved2			:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved2			:16;
	BIT_FIELD	reserved			:12;
	BIT_FIELD	divHist				:1;
	BIT_FIELD	split				:1;
	BIT_FIELD	changed				:1;
	BIT_FIELD	deleted				:1;
#endif
} MASTER_FUND_UPD_BITS_ALL, *lpMASTER_FUND_UPD_BITS_ALL;

typedef struct _MASTER_FUND_UPD_BITS_0
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	extUpd				:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	ftUpd				:1;
	BIT_FIELD	telekursUpd			:1;
	BIT_FIELD	reserved2			:3;
	BIT_FIELD	ignoreDate			:1;
	BIT_FIELD	ignoreDesc			:1;
	BIT_FIELD	ignoreName			:1;
	BIT_FIELD	ignoreSic			:1;
	BIT_FIELD	ignoreCat			:1;
	BIT_FIELD	ignoreStat			:1;
	BIT_FIELD	reserved3			:9;
	BIT_FIELD	isNotEquity			:1;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	isNotEquity			:1;
	BIT_FIELD	reserved3			:9;
	BIT_FIELD	ignoreStat			:1;
	BIT_FIELD	ignoreCat			:1;
	BIT_FIELD	ignoreSic			:1;
	BIT_FIELD	ignoreName			:1;
	BIT_FIELD	ignoreDesc			:1;
	BIT_FIELD	ignoreDate			:1;
	BIT_FIELD	reserved2			:3;
	BIT_FIELD	telekursUpd			:1;
	BIT_FIELD	ftUpd				:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	extUpd				:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_0, *lpMASTER_FUND_UPD_BITS_0;

typedef struct _MASTER_FUND_UPD_BITS_1
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	histUpd				:1;
	BIT_FIELD	idsiHiLoUpd			:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:5;
	BIT_FIELD	ignoreHist			:1;
	BIT_FIELD	ignoreBeta			:1;
	BIT_FIELD	ignoreVol			:1;
	BIT_FIELD	reserved3			:13;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:13;
	BIT_FIELD	ignoreVol			:1;
	BIT_FIELD	ignoreBeta			:1;
	BIT_FIELD	ignoreHist			:1;
	BIT_FIELD	reserved2			:5;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiHiLoUpd			:1;
	BIT_FIELD	histUpd				:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_1, *lpMASTER_FUND_UPD_BITS_1;

typedef struct _MASTER_FUND_UPD_BITS_2
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	splitsUpd			:1;
	BIT_FIELD	reserved2			:5;
	BIT_FIELD	ignoreEarn			:1;
	BIT_FIELD	ignoreShares		:1;
	BIT_FIELD	ignoreEarn2			:1;
	BIT_FIELD	sharesAdjusted		:1;
	BIT_FIELD	reserved3			:12;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:12;
	BIT_FIELD	sharesAdjusted		:1;
	BIT_FIELD	ignoreEarn2			:1;
	BIT_FIELD	ignoreShares		:1;
	BIT_FIELD	ignoreEarn			:1;
	BIT_FIELD	reserved2			:5;
	BIT_FIELD	splitsUpd			:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_2, *lpMASTER_FUND_UPD_BITS_2;

typedef struct _MASTER_FUND_UPD_BITS_3
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	ignoreDiv			:1;
	BIT_FIELD	ignoreSpc			:1;
	BIT_FIELD	ignoreIad			:1;
	BIT_FIELD	reserved3			:13;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:13;
	BIT_FIELD	ignoreIad			:1;
	BIT_FIELD	ignoreSpc			:1;
	BIT_FIELD	ignoreDiv			:1;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_3, *lpMASTER_FUND_UPD_BITS_3;

typedef struct _MASTER_FUND_UPD_BITS_4
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	ignoreSpc			:1;
	BIT_FIELD	reserved3			:15;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:15;
	BIT_FIELD	ignoreSpc			:1;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_4, *lpMASTER_FUND_UPD_BITS_4;

typedef struct _MASTER_FUND_UPD_BITS_5
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	corpActUpd			:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	reserved3			:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:16;
	BIT_FIELD	reserved2			:6;  
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	corpActUpd			:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_5, *lpMASTER_FUND_UPD_BITS_5;

typedef struct _MASTER_FUND_UPD_BITS_6
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	ignoreType			:1;
	BIT_FIELD	ignoreSType			:1;
	BIT_FIELD	ignoreSP			:1;
	BIT_FIELD	ignoreMoody			:1;
	BIT_FIELD	ignoreCoupon		:1;
	BIT_FIELD	ignorePar			:1;
	BIT_FIELD	ignoreDebt			:1;
	BIT_FIELD	ignoreInf			:1;
	BIT_FIELD	ignoreCallDate		:1;
	BIT_FIELD	ignoreCallAmt		:1;
	BIT_FIELD	ignoreCallPrc		:1;
	BIT_FIELD	ignoreCallIty		:1;
	BIT_FIELD	ignoreCallTy		:1;
	BIT_FIELD	ignoreCallRed		:1;
	BIT_FIELD	ignoreCallPrem		:1;
	BIT_FIELD	ignoreIPay			:1;
	BIT_FIELD	ignoreIRd			:1;
	BIT_FIELD	ignoreIPd			:1;
	BIT_FIELD	ignoreIPTy			:1;
	BIT_FIELD	ignoreIPFr			:1;
	BIT_FIELD	ignoreTax			:1;
	BIT_FIELD	ignoreForm			:1;

#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	ignoreForm			:1;
	BIT_FIELD	ignoreTax			:1;
	BIT_FIELD	ignoreIPFr			:1;
	BIT_FIELD	ignoreIPTy			:1;
	BIT_FIELD	ignoreIPd			:1;
	BIT_FIELD	ignoreIRd			:1;
	BIT_FIELD	ignoreIPay			:1;
	BIT_FIELD	ignoreCallPrem		:1;
	BIT_FIELD	ignoreCallRed		:1;
	BIT_FIELD	ignoreCallTy		:1;
	BIT_FIELD	ignoreCallIty		:1;
	BIT_FIELD	ignoreCallPrc		:1;
	BIT_FIELD	ignoreCallAmt		:1;
	BIT_FIELD	ignoreCallDate		:1;
	BIT_FIELD	ignoreInf			:1;
	BIT_FIELD	ignoreDebt			:1;
	BIT_FIELD	ignorePar			:1;
	BIT_FIELD	ignoreCoupon		:1;
	BIT_FIELD	ignoreMoody			:1;
	BIT_FIELD	ignoreSP			:1;
	BIT_FIELD	ignoreSType			:1;
	BIT_FIELD	ignoreType			:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_6, *lpMASTER_FUND_UPD_BITS_6;

typedef struct _MASTER_FUND_UPD_BITS_8
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:6;
	BIT_FIELD	ignoreCat			:1;
	BIT_FIELD	ignoreMaturity		:1;
	BIT_FIELD	ignoreAssets		:1;
	BIT_FIELD	ignoreAnnualYield	:1;
	BIT_FIELD	ignoreCurrYield		:1;
	BIT_FIELD	ignorePrevYield		:1;
	BIT_FIELD	reserved3			:10;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved3			:10;
	BIT_FIELD	ignorePrevYield		:1;
	BIT_FIELD	ignoreCurrYield		:1;
	BIT_FIELD	ignoreAnnualYield	:1;
	BIT_FIELD	ignoreAssets		:1;
	BIT_FIELD	ignoreMaturity		:1;
	BIT_FIELD	ignoreCat			:1;
	BIT_FIELD	reserved2			:6;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_8, *lpMASTER_FUND_UPD_BITS_8;

typedef struct _MASTER_FUND_UPD_BITS_9
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	reserved			:8;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	cusipUpd			:1;
	BIT_FIELD	ftUpd				:1;
	BIT_FIELD	telekursUpd			:1;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	reserved2			:3;
	BIT_FIELD	reserved3			:1;
	BIT_FIELD	ignoreID			:1;
	BIT_FIELD	reserved4			:14;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	reserved4			:14;
	BIT_FIELD	ignoreID			:1;
	BIT_FIELD	reserved3			:1;
	BIT_FIELD	reserved2			:3;
	BIT_FIELD	partialUpd			:1;
	BIT_FIELD	telekursUpd			:1;
	BIT_FIELD	ftUpd				:1;
	BIT_FIELD	cusipUpd			:1;
	BIT_FIELD	idsiPrcDivUpd		:1;
	BIT_FIELD	reserved			:8;
#endif
} MASTER_FUND_UPD_BITS_9, *lpMASTER_FUND_UPD_BITS_9;

typedef union _MASTER_FUND_UPD_FLAGS
{
	MASTER_FUND_UPD_BITS_ALL	all;
	MASTER_FUND_UPD_BITS_0		upf0;
	MASTER_FUND_UPD_BITS_1		upf1;
	MASTER_FUND_UPD_BITS_2		upf2;
	MASTER_FUND_UPD_BITS_3		upf3;
	MASTER_FUND_UPD_BITS_4		upf4;
	MASTER_FUND_UPD_BITS_5		upf5;
	MASTER_FUND_UPD_BITS_6		upf6;
	MASTER_FUND_UPD_BITS_8		upf8;
	MASTER_FUND_UPD_BITS_9		upf9;
    ULONG						mask;
} MASTER_FUND_UPD_FLAGS, *lpMASTER_FUND_UPD_FLAGS;

typedef struct _MASTER_FUND_REC_BITS_0
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		hasPriceHist	:1;
	BIT_FIELD		hasEarnings		:1;
	BIT_FIELD		hasDividend		:1;
	BIT_FIELD		hasAddSpcDiv	:1;
	BIT_FIELD		hasDivHist		:1;
	BIT_FIELD		hasBond			:1;
	BIT_FIELD		reserved		:1;
	BIT_FIELD		hasMMFund		:1;
	BIT_FIELD		hasIssueId		:1;
	BIT_FIELD		reserved2		:7;
	BIT_FIELD		reserved3		:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		reserved3		:16;
	BIT_FIELD		reserved2		:7;
	BIT_FIELD		hasIssueId		:1;
	BIT_FIELD		hasMMFund		:1;
	BIT_FIELD		reserved		:1;
	BIT_FIELD		hasBond			:1;
	BIT_FIELD		hasDivHist		:1;
	BIT_FIELD		hasAddSpcDiv	:1;
	BIT_FIELD		hasDividend		:1;
	BIT_FIELD		hasEarnings		:1;
	BIT_FIELD		hasPriceHist	:1;
#endif
} MASTER_FUND_REC_BITS_0, *lpMASTER_FUND_REC_BITS_0;

typedef struct _MASTER_FUND_REC_BITS_1
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		reserved		:16;
	BIT_FIELD		reserved2		:16;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		reserved2		:16;
	BIT_FIELD		reserved		:16;
#endif
} MASTER_FUND_REC_BITS_1, *lpMASTER_FUND_REC_BITS_1;

typedef union _MASTER_FUND_REC_FLAGS_0
{
	MASTER_FUND_REC_BITS_0	bits;
	ULONG					mask;
} MASTER_FUND_REC_FLAGS_0, *lp_MASTER_FUND_REC_FLAGS_0;

typedef union _MASTER_FUND_REC_FLAGS_1
{
	MASTER_FUND_REC_BITS_1	bits;
	ULONG					mask;
} MASTER_FUND_REC_FLAGS_1, *lp_MASTER_FUND_REC_FLAGS_1;

typedef struct _MASTER_FUND_REC_FLAGS
{
	MASTER_FUND_REC_FLAGS_0	flag0;
	MASTER_FUND_REC_FLAGS_1	flag1;
} MASTER_FUND_REC_FLAGS;

typedef struct _MASTER_FUND_UPDATE_DATA
{
	MASTER_FUND_UPD_FLAGS	flags;
	FUND_DATE				lastUpdate;
	FUND_DATE				added;
	FUND_DATE				deleted;
	SHORT					row;
	SHORT					column;
	SHORT					session;
	CHAR					reserved[10];
} MASTER_FUND_UPDATE_DATA;

typedef struct _FUNDAMENTAL_0
{
	SHORT					securityCategory;	// hyperfeed stand security category
	SHORT					issueStatus;		// when issued, delisted, suspended, backruptcy, etc 
	CHAR					name[48];			// issuer 
	CHAR					description[32];	// issue description
	FUND_DATE				maturityDate;		// maturity/expiration date where applicable
	SHORT					sicCode;			// SIC code;  
	CHAR					reserved[2];
	MASTER_FUND_REC_FLAGS	flags;   
	FUND_DATE				effectiveDate;
} FUNDAMENTAL_0, *lpFUNDAMENTAL_0;

typedef struct _FUNDAMENTAL_1
{
	LONG					high;				// 52 week high
	LONG					low;				// 52 week low
	FUND_DATE				highDate;
	FUND_DATE				lowDate;
	SHORT					format;				// price format ( deciaml precision )
	CHAR					reserved[2];
	LONG					beta;				// calculated beta;      
	LONG					volatility;
	FUND_DATE				volDate;			// date of last vol calc;  
	SHORT					betaFormat;
	SHORT					volFormat;
} FUNDAMENTAL_1, *lpFUNDAMENTAL_1;

typedef struct _FUNDAMENTAL_2
{
	ULONG					shares;
	LONG					earnings;			// eps
	LONG					earnings2;			// undiluted;
	SHORT					epsFormat;
	SHORT					eps2Format;  
	SHORT					earningsYear;		//  
	SHORT					earningsMonth;		//  1 - 12
	SHORT					earnings2Year;		//  
	SHORT					earnings2Month;		//  1 - 12
	FUND_DATE				lastSplit;
	LONG					lastSplitToShares;	//  
	LONG					lastSplitFromShares;
} FUNDAMENTAL_2, *lpFUNDAMENTAL_2;

// Dividend data
// current dividend data

// iad codes

#define IAD_NA				0	// does not apply
#define IAD_FREQ			1	// iad = cash rate * payment frequency
#define IAD_FREQ_PLUS		2	// iad = cash rate * payment frequency + special
#define IAD_TOTAL			3	// iad = cash payments for past year
#define IAD_TOTAL_PLUS		4	// iad = cash payments last year plus special       
#define IAD_EST				5	// iad is estimated long-term return on UIT by fund sponsor   
#define IAD_SPLIT_ONLY		6	// NO iad, stock splits only
#define IAD_STOCK_ONLY		7	// NO iad, stock dividend only
#define IAD_INITIAL			8	// initial dividend, iad = initial
#define IAD_LESS_YEAR_HIST	9	// less than a full year sum 
#define IAD_PREFERRED		10	// preferred issue with fixed annaul rate
#define IAD_RESUMED			11	// dividends resumed
#define IAD_ADJ				12	// adjusted for split of stock dividend
#define IAD_LESS_YEAR_EST	13	// iad = (dividends paid / num paid) * frequency
#define IAD_SPONSOR			14	// sponsor provided current yield used to calcualte for UIT
#define IAD_UIT_UA			99	// UIT temporarily unavailable

// dividend types

#define DV_TYPE_NONE			0
#define DV_TYPE_CASH			1
#define DV_TYPE_STOCK			2
#define DV_TYPE_SPLIT			3
#define DV_TYPE_OTHER_STOCK		4
#define DV_TYPE_RIGHTS			5
#define DV_TYPE_WARRANTS		6
#define DV_TYPE_OTHER_CURR		7
#define DV_TYPE_ISSUE_CALLED	8
#define DV_OMITTED				9
#define DV_STOCKHOLDER_MEETING	10
#define DV_TYPE_STOCK_EXCH		11

// supplemental dividend types

// reinvestment codes

// tax status codes

#define TSC_NONE								0	// not available
#define TSC_FULL								1	// fully taxable
#define TSC_US_NO_NRA							2	// no US tax withholding for non-resident aliens			 
#define TSC_US_DIV_NRA							3	// dividend withholding for non-resident aliens			 
#define TSC_US_NO_DIV_NRA						4	// no dividend withholding for non-resident aliens			 
#define TSC_NO_FOREIGN							5	// tax free distribution not subject to foreign withholding
#define TSC_RECLAIM								6	// reclaim entitlement
#define TSC_FTC									7	// rate includes foreign tax credit
#define TSC_NO_FTC								8	// rate excludes foreign tax credit
#define TSC_UNDIS_CG							9	// undistributed capital gains/to be determined
#define TSC_RC									10	// return of capital
#define TSC_RCI									11	// return of capital and income ( partially taxable )
#define TSC_NO_TAX								12	// tax exempt
#define TSC_ROYALITY							13	// royality income distribution
#define TSC_FCG									14	// includes foreign currency gains
#define TSC_PART_NO_TAX							15	// partially tax exempt
#define TSC_NON_TAX_EVENT						16	// non taxable event
												
#define TSC_FULL_US_NO_NRA						22	// fully taxable, no US tax withholding for non-resident aliens			 
#define TSC_FULL_US_DIV_NRA						23	// fully taxable, dividend withholding for non-resident aliens			 
#define TSC_FULL_US_NO_DIV_NRA					24	// fully taxable, no dividend withholding for non-resident aliens			 
#define TSC_FULL_NO_FOREIGN						25	// fully taxable, tax free distribution not subject to foreign withholding
#define TSC_FULL_RECLAIM						26	// fully taxable, reclaim entitlement
#define TSC_FULL_FTC							27	// fully taxable, rate includes foreign tax credit
#define TSC_FULL_NO_FTC							28	// fully taxable, rate excludes foreign tax credit
#define TSC_FULL_UNDIS_CG						29	// fully taxable, undistributed capital gains/to be determined
#define TSC_FULL_ROYALITY						33	// fully taxable, royality inclome distribution
#define TSC_FULL_FCG							34	// fully taxable, includes foreign currency gains
												
#define TSC_NA									255	// does not apply

// tax base codes

#define TBC_UNKNOWN_CODE_NA						0	// undefined code or not applicable
#define TBC_NORMAL								1	// no special base applies
#define TBC_NR_CANADA_10						2	// less 10% Canadian non-resident
#define TBC_NR_CANADA_15						3	// less 15% Canadian non-resident
#define TBC_UNKNOWN_VARIABLE					4	// unknown or variable tax
#define TBC_US_NO_NRA							5	// US tax not withheld from non-resident aliens
#define TBC_INCOME_CAPITALGAINS					6	// income and capital gains
#define TBC_PART_ALL_EXEMPT						7	// partially or completely tax exempt
#define TBC_INCOME_CAPITALGAINS_PART_ALL_EXEMPT	8	// both income and capital gains AND partially or completely tax exempt
#define TBC_CAPITALGAINS_ONLY					9	// capital gains distribution only
#define TBC_NET_BEFORE_US_RES					11	// net before taxes to US residents ( ADRS )
#define TBC_NET_AFTER_US_RES					12	// net after taxes to US residents ( ADRS )
#define TBC_EX_AT_CLOSE							24	// Ex-distribution/dividend at close instead of opening  of business 

typedef struct _FUNDAMENTAL_3
{
	LONG			iad;			
	SHORT			iadFormat;			// number of deciamls
	SHORT			iadCode;			// calculation method footnote
	BYTE			regularFrequency;	// dividends paid per year ( if regular )
	BYTE			resv1[3];			// reserved
	FUND_DIV_DATA	regularDiv;
	FUND_DIV_DATA	specialDiv;
} FUNDAMENTAL_3, *lpFUNDAMENTAL_3;

// additional special dividends

typedef struct _FUNDAMENTAL_4
{
	FUND_DIV_DATA	addSpecialDiv[4];
	BOOL			more;
} FUNDAMENTAL_4, *lpFUNDAMENTAL_4;

// dividend history

typedef struct _FUNDAMENTAL_5
{
	FUND_DIV_DATA	historicalDiv[4];
	BOOL			more;
} FUNDAMENTAL_5, *lpFUNDAMENTAL_5;

// Bonds

// type codes

#define BOND_TYPE_NONE			0			// no type specified
#define BOND_TYPE_CONV			1			// convertible
#define BOND_TYPE_MUNI_DOL_TAX	2			// municiple dollar or taxable
#define BOND_TYPE_CORP_GOV		3			// government or corporate
#define BOND_TYPE_TNOTE			4			// treasury note
#define BOND_TYPE_TBOND			5			// treasury bond
#define BOND_TYPE_TBILL			6			// treasury bill
#define BOND_TYPE_FHLB			7			// Federal Home Loan Bank
#define BOND_TYPE_FHLMC			8			// FHLMC debenture
#define BOND_TYPE_FLB			9			// Federal Land Bank
#define BOND_TYPE_FFNMA			10			// FFNMA
#define BOND_TYPE_FNMA			11			// Federal National Mortgage Assioc Partnership
#define BOND_TYPE_GNMA			12			// GNMA pass through
#define BOND_TYPE_FHLA			13			// Federal Home Loan Administration
#define BOND_TYPE_GSA			14			// General Service Administration
#define BOND_TYPE_TFLB			15			// Twelve Federal Land Bank
#define BOND_TYPE_TFICB			16			// Twelve Federal International Credit bank
#define BOND_TYPE_TBC			17			// Thirteen Banks for Cooperative
#define BOND_TYPE_STRIPPED		18			// stripped issue
#define BOND_TYPE_ZERO			19			// zero coupon  issue
#define BOND_TYPE_EXP_INP		20			// Export Import
#define BOND_TYPE_OTHER_GOV		21			// other government ( US Postal, TVA, Student Loan, etc )

// supplemental types

#define BOND_STYPE_NONE			0			// no supplemental type
#define BOND_STYPE_EQUIP		1			// equipment trust
#define BOND_STYPE_COLL			2			// collateral trust
#define BOND_STYPE_NOTE			3			// note
#define BOND_STYPE_MORT			4			// mortgage ( general, consolidated, refunding )
#define BOND_STYPE_REFUND		5			// refunding
#define BOND_STYPE_DEB			6			// debenture
#define BOND_STYPE_SUB			7			// subordinate debenture
#define BOND_STYPE_INCOME		8			// income
#define BOND_STYPE_LOAN			9			// loan certificate
#define BOND_STYPE_BILL			10			// bill
#define BOND_STYPE_CD			11			// certicate of deposit
#define BOND_STYPE_FD			12			// foreign debt
#define BOND_STYPE_E_H			13			// series E or H US government
#define BOND_STYPE_CMO			14			// asset backed security or CMO
#define BOND_STYPE_PRIVATE		15			// private placement
#define BOND_STYPE_MTNF			16			// medium term fixed rate
#define BOND_STYPE_MTNV			17			// medium term variable rate
#define BOND_STYPE_MARINE		18			// Merchant Marine, US guaranty
#define BOND_STYPE_COMM			19			// commercial paper
#define BOND_STYPE_MGO			20			// municipal general obligation
#define BOND_STYPE_MR			21			// municpal revenue
#define BOND_STYPE_VARI_STEP	22			// varible rate step bond
#define BOND_STYPE_EXCH			23			// exchangeable
#define BOND_STYPE_UNIT			24			// unit ( debt issue )
#define BOND_STYPE_VARI			25			// variable rate ( not MTN )
#define BOND_STYPE_STRIPPED		26			// stripped issue
#define BOND_STYPE_ZERO			27			// zero coupon
#define BOND_STYPE_MORT_BACK	28			// mortgage backed pass-through number
#define BOND_STYPE_ADJ			29			// adjustable rate

#define BOND_STYPE_OTHER		255

// Standard and Poor's ratings

#define BOND_SR_NONE			0			// no rating available
#define BOND_SR_AAA				1			// AAA
#define BOND_SR_AA_P			2			// AA+
#define BOND_SR_AA				3			// AA
#define BOND_SR_AA_M			4			// AA-
#define BOND_SR_A_P				5			// A+
#define BOND_SR_A				6			// A
#define BOND_SR_A_M				7			// A-
#define BOND_SR_BBB_P			8			// BBB+
#define BOND_SR_BBB				9			// BBB
#define BOND_SR_BBB_M			10			// BBB-
#define BOND_SR_BB_P			11			// BB+
#define BOND_SR_BB				12			// BB
#define BOND_SR_BB_M			13			// BB-
#define BOND_SR_B_P				14			// B+
#define BOND_SR_B				15			// B
#define BOND_SR_B_M				16			// B-
#define BOND_SR_CCC_P			17			// CCC+		
#define BOND_SR_CCC				18			// CCC
#define BOND_SR_CCC_M			19			// CCC-
#define BOND_SR_CC				20			// CC
#define BOND_SR_C				21			// C
#define BOND_SR_CI				22			// CI ( corporate income )		
#define BOND_SR_DDD				23			// DDD
#define BOND_SR_DD				24			// DD
#define BOND_SR_D				25			// D
#define BOND_SR_A_1				26			// A-1+	( short term debt and US muni bonds )

#define	BOND_SR_NR				255			// not rated	

// Moody"s ratings

#define BOND_MR_NONE			0			// no rating available
#define BOND_MR_Aaa				1			// Aaa
#define BOND_MR_Aa1				2			// Aa1
#define BOND_MR_Aa2				3			// Aa2
#define BOND_MR_Aa3				4			// Aa3
#define BOND_MR_Aa				5			// Aa
#define BOND_MR_A1				6			// A1
#define BOND_MR_A2				7			// A2
#define BOND_MR_A3				8			// A3
#define BOND_MR_A				9			// A
#define BOND_MR_Baa1			10			// Baa1
#define BOND_MR_Baa2			11			// Baa2
#define BOND_MR_Baa3			12			// Baa3
#define BOND_MR_Baa				13			// Baa
#define BOND_MR_Ba1				14			// Ba1
#define BOND_MR_Ba2				15			// Ba2
#define BOND_MR_Ba3				16			// Ba3
#define BOND_MR_Ba				17			// Ba
#define BOND_MR_B1				18			// B1
#define BOND_MR_B2				19			// B2
#define BOND_MR_B3				20			// B3
#define BOND_MR_B				21			// B
#define BOND_MR_Caa				22			// Caa
#define BOND_MR_Ca				23			// Ca
#define BOND_MR_C				24			// C
#define BOND_MR_PRIME_1_MIG1	25			// prime-1, MIG1 ( muni bonds )
#define BOND_MR_PRIME_2_MIG2	26			// prime-2, MIG2 ( muni bonds )
#define BOND_MR_PRIME_3_MIG3	27			// prime-3, MIG3 ( muni bonds )
#define BOND_MR_NO_PRIME_MIG4	28			// not prime, MIG4 ( muni bonds )
#define BOND_MR_Caa1_VMIG1		29			// Caaa1, VMIG1 ( muni bonds )
#define BOND_MR_Caa2_VMIG2		30			// Caaa2, VMIG2 ( muni bonds )
#define BOND_MR_Caa3_VMIG3		31			// Caaa3, VMIG3 ( muni bonds )
#define BOND_MR_P1				32			// P-1	( muni only )
#define BOND_MR_P2				32			// P-2	( muni only )
#define BOND_MR_P3				32			// P-3	( muni only )

#define BOND_MR_NR				255			// not rated

// bond interest payment type codes

#define BOND_INT_PAY_NONE		0			// not available/nat applicable
#define BOND_INT_PAY_US			1			// US currency
#define BOND_INT_PAY_CAN		2			// Canadian currency
#define BOND_INT_PAY_FC			3			// foreign currency ( non Canadian )	
#define BOND_INT_PAY_UNKNOWN	255			// unknown or not available

// bond interest payment frequency code

#define BOND_INT_FRQ_NONE		0			// not available
#define BOND_INT_FRQ_ANNUAL		1			// annual
#define BOND_INT_FRQ_SEMI		2			// semi annual
#define BOND_INT_FRQ_TRI		3			// three payments/year
#define BOND_INT_FRQ_QUART		4			// quarterly
#define BOND_INT_FRQ_BI_MONTH	6			// bi monthly
#define BOND_INT_FRQ_MONTH		12			// monthly

#define BOND_INT_FRQ_DISC_365	13			// discound basis, 365 day calendar
#define BOND_INT_FRQ_DISC_360	14			// discount basis, 360 day calendar
#define BOND_INT_FRQ_SEMI_365	15			// semi annual, 365 day calendar
#define BOND_INT_FRQ_ANNUAL_365	16			// annual, 365 day calendar

#define BOND_INT_FRQ_MATURITY	18			// payable at maturity
#define BOND_INT_FRQ_VARI		19			// variable 

#define BOND_INT_FRQ_ANNUAL_A	21			// annual - in arrears
#define BOND_INT_FRQ_SEMI_A		22			// semi annual - in arrears
#define BOND_INT_FRQ_TRI_A		23			// three payments/year - in arrears
#define BOND_INT_FRQ_QUART_A	24			// quarterly - in arrears
#define BOND_INT_FRQ_BI_MONTH_A	26			// bi monthly - in arrears
#define BOND_INT_FRQ_MONTH_A	32			// monthly - in arrears

#define BOND_INT_FRQ_DEFAULT	99			// in default 

// bond form code

#define BOND_FORM_NONE				0		// not available
#define BOND_FORM_REGISTERED		1		// registered
#define BOND_FORM_BEARER			2		// coupon or bearer
#define BOND_FORM_R_DTC_CNS			3		// registered, DTC, CNS
#define BOND_FORM_R_DTC				4		// registered, DTC
#define BOND_FORM_R_NO_DTC_CNS		5		// registered, not DTC, CNS elligible	
#define BOND_FORM_B_DTC_CNS			6		// coupon or bearer, DTC, CNS
#define BOND_FORM_B_DTC				7		// coupon or bearer, DTC
#define BOND_FORM_B_NO_DTC_CNS		8		// coupon or bearer, not DTC, CNS elligible
#define BOND_FORM_RC_DTC_CNS		9		// registered and coupon, DTC, CNS
#define BOND_FORM_RC_DTC			10		// registered and coupon, DTC
#define BOND_FORM_RC_NO_DTC_CNS		11		// registered and coupon, not DTC, CNS elligible

// called bond issue types

#define BOND_CALL_I_TYPE_NONE		0		// unavailable ( or not called )
#define BOND_CALL_I_TYPE_CORP		1		// corpoarte
#define BOND_CALL_I_TYPE_MUNI		2		// municipal
#define BOND_CALL_I_TYPE_FOREIGN	3		// foreign
#define BOND_CALL_I_TYPE_CHURCH		4		// church
#define BOND_CALL_I_TYPE_TAX		5		// tax anticipation warrant
#define BOND_CALL_I_TYPE_DIST		6		// district improvement bond

// call types

#define BOND_CALL_TYPE_NONE			0		// not specified ( or not called )
#define BOND_CALL_TYPE_ALL			1		// entire issue
#define BOND_CALL_TYPE_PART_DOL		2		// partial  call in dollars
#define BOND_CALL_TYPE_BOND			3		// partial call in bonds
#define BOND_CALL_TYPE_VARI			4		// varaious maturity, denomination, etc

// call redemption options

#define BOND_REDEMP_NONE			0		// no options (or not called)
#define BOND_REDEMP_PREPAY			1		// prepayment before call date
#define BOND_REDEMP_CONV			2		// convertable to stock at speicified price before call date
#define BOND_REDEMP_PURCHASE		3		// offer to purchase at spcified price before call date
#define BOND_REDEMP_CONV_PURCHASE	4		// convertaible and offer to purchase

// call premium codes

#define BOND_PREMIUM_NONE			0		// no premium or does not apply
#define BOND_PREMIUM_INCL			1		// redemption includes premium, usually % of call price

typedef struct _FUNDAMENTAL_6
{
	BYTE		bondType;					// bond type code
	BYTE		suppBondType;				// supplimental bond code
	BYTE		spRating;					// S&P quality rating
	BYTE		moodyRating;				// Moody's quality rating
	LONG		couponRate;
	LONG		parValue;
	LONG		debtOutstanding;
	LONG		inflationRatio;				// ( for TIPS, current day, unadjusted CPI )
	BYTE		couponFmt;					// number of decimal palces
	BYTE		parFmt;						// number of decimal palces
	BYTE		debtFmt;					// number of decimal palces
	BYTE		irFmt;						// number of decimal palces
	FUND_DATE	callDate;					// date of scheduled redemption
	LONG		callAmount;					// bonds or percent dollars for partial call
	LONG		callPrice;					// for regular or coupon, % of par: for zero coupon or deep discount, % of accrued value
	SHORT		callPriceFmt;				// number of decimal palces
	BYTE		callIssueTypeCode;
	BYTE		callTypeCode;
	BYTE		callRedemptionOptionCode;
	BYTE		callPremiumCode; 	
	CHAR		reserved[2];
	LONG		intPayment;					// payment per $1000 par value 
	FUND_DATE	intRecordDate;				// record date
	FUND_DATE	intPayDate;					// payment date
	BYTE		payTypeCode;				// payment type code
	BYTE		payFreqCode;				// interest payment frequency
	BYTE		taxBaseCode;				// tax base code
	BYTE		intFmt;
	BYTE		formCode;					// bond form
	BYTE		callReserved[3];			// == NULL
} FUNDAMENTAL_6, *lpFUNDAMENTAL_6;

//  money market mutual find data

#define FUND_TYPE_NA		0		// not available
#define FUND_TYPE_GP		1		// general purpose
#define FUND_TYPE_GOV		2		// goverment security
#define FUND_TYPE_TAX_EX	3		// tax exempt

typedef struct _FUNDAMENTAL_8
{
	SHORT		fundType;
	CHAR		reserved[2];
	LONG		totalAssets;		// millions of dollars
	LONG		avgMaturity;
	LONG		annualYield;		// 3 devimal
	LONG		currentYield;		// 3 deciaml current 7 day average yield           
	LONG		prevCurrYield;		// 3 deciaml previous 7 days average yield
	FUND_DATE	currDate;			// evaluation date current average
	FUND_DATE	prevDate;			// evaluation date provious average
	SHORT		annualFmt;
	SHORT		currFmt;
	SHORT		prevFmt;
	CHAR		reserved2[2];
} FUNDAMENTAL_8, *lpFUNDAMENTAL_8;

#define ID_TYPE_NA			0
#define ID_TYPE_CUSIP		1
#define ID_TYPE_VELOREN		2
#define	ID_TYPE_SEDOL		3
#define	ID_TYPE_ISIN		4
#define	ID_TYPE_CEDEL		5
#define	ID_TYPE_GERMAN		6
#define	ID_TYPE_BELGIAN		7



typedef struct _FUNDAMENTAL_9
{
	SHORT		idType;
	CHAR		issueId[32];
	CHAR		reserved[2];
} FUNDAMENTAL_9, *lpFUNDAMENTAL_9;

typedef union _MASTER_FUNDAMENTAL_RECORD_DATA
{
	FUNDAMENTAL_0		basic;
	FUNDAMENTAL_0		f0;
	FUNDAMENTAL_1		priceHist;
	FUNDAMENTAL_1		f1;
	FUNDAMENTAL_2		earningsShares;
	FUNDAMENTAL_2		f2;
	FUNDAMENTAL_3		dividends;
	FUNDAMENTAL_3		f3;
	FUNDAMENTAL_4		addSpcDiv;
	FUNDAMENTAL_4		f4;
	FUNDAMENTAL_5		divHistory;
	FUNDAMENTAL_5		f5;
	FUNDAMENTAL_6		bond;
	FUNDAMENTAL_6		f6;
	FUNDAMENTAL_8		moneyMkt;
	FUNDAMENTAL_8		f8;
	FUNDAMENTAL_9		issueId;
	FUNDAMENTAL_9		f9;
} MASTER_FUNDAMENTAL_RECORD_DATA, *lpMASTER_FUNDAMENTAL_RECORD_DATA;

typedef struct _MASTER_FUNDAMENTAL_RECORD
{
	FUND_KEY_0						key;
	SHORT							row;
	SHORT							column;  
	MASTER_FUND_UPDATE_DATA			ud;
	MASTER_FUNDAMENTAL_RECORD_DATA	recordData;
	CHAR							reserved[44];		// Pad to 256 total.
} MASTER_FUNDAMENTAL_RECORD, *lpMASTER_FUNDAMENTAL_RECORD;

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif

#endif
