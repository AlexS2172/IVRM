
#ifndef __MGUIDE_H
#define __MGUIDE_H

#ifndef INCL_NOPACKING
	#pragma pack(1)
#endif

// Defines for the various Reference database Key Numbers.
 
#define REFKN_TICKER 	0
#define REFKN_CUSIP		1
#define REFKN_NAME		2
#define REFKN_REPNO		3

// Reference database record.

typedef struct _REF_TICKER 
{
	SHORT 		type;
	UCHAR 		symbol[12];
	UCHAR 		countryCode[2];
	UCHAR 		exchangeCode[2];
	UCHAR 		currencyCode[3];
	UCHAR 		reserved;
} REF_TICKER;

typedef struct _REFERENCE_RECORD 
{
	REF_TICKER	ticker;
	UCHAR 		cusip[12];
	UCHAR 		name[36];
	UCHAR 		REPNO[5];
	UCHAR 		reserved[29];
} REFERENCE_RECORD;

#define DBA_RECORD_57 REFERENCE_RECORD

// Defines for the various Market Guide database Record Types.

#define MGRT_ID					0
#define MGRT_FINSUMMARY 		1
#define MGRT_PERSHAREHISTORY	2
#define MGRT_INCSELECT			3
#define MGRT_INCSUMMARY 		4
#define MGRT_BSSUMMARY			5
#define MGRT_BRIEF1				6
#define MGRT_BRIEF2				7
#define MGRT_BRIEF3				8
#define MGRT_CAPITALSTRUCT1		9
#define MGRT_CAPITALSTRUCT2		10
#define MGRT_CAPITALSTRUCT3		11
#define MGRT_ANALYST 			12
#define MGRT_RESTATEMENT		13
#define MGRT_OFFICERS1			14
#define MGRT_OFFICERS2			15
#define MGRT_INCDETAIL1 		16
#define MGRT_INCDETAIL2 		17
#define MGRT_INCDETAIL3 		18
#define MGRT_INCDETAIL4 		19
#define MGRT_BSDETAIL1			20
#define MGRT_BSDETAIL2			21
#define MGRT_BSDETAIL3			22
#define MGRT_BSDETAIL4			23
#define MGRT_BSDETAIL5			24
#define MGRT_CASHFLOW			25
#define MGRT_INCQUARTERLY		26
#define MGRT_BSQUARTERLY		27
#define MGRT_INDUSTRY1			28
#define MGRT_INDUSTRY2			29
#define MGRT_PRODUCT1			30
#define MGRT_PRODUCT2			31
#define MGRT_ADDRESS 			32

#define MGRT_MAX				32 	// Last valid record type.

// Market Guide database records.

typedef struct _MGUIDE_DATE 
{
	UCHAR 		month;
	UCHAR 		day;
	USHORT		year;
} MGUIDE_DATE;

typedef struct _MGUIDE_KEY 
{
	UCHAR 		REPNO[5];
	CHAR		type;
} MGUIDE_KEY;	  // 6 bytes

typedef struct _MGUIDE_ID_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		CUSIP[12];			// CUSIP number as assigned by S&P
	UCHAR 		TICKER[6];			// Ticker symbol
	UCHAR 		EXCH[4]; 			// Exchange
	MGUIDE_DATE DTUPD;				// Date analyst updated information
	UCHAR 		CONAME[30]; 		// Company name
	CHAR		COSTATUS;			// Company status
	CHAR		NEWCO;				// New company, added in the previous week
	SHORT 		YEAR; 				// Fiscal year, latest updated
	CHAR		FYEM; 				// Fiscal year ending month
	CHAR		ASOURCE; 			// Source, year end
	CHAR		QTR;				// Latest quarter updated in current fiscal year (1,2,3 or 4)
	CHAR		QSOURCE; 			// Source, quarter end
	CHAR		UNITS;				// Units financial statements are reported in (U)
	UCHAR 		CURRENCY[18];		// Currency financial statements are reported in
	LONG		CURRATE; 			// Currency exchange rate, most recent (C)
	LONG		SHSPERADR;			// Shares per ADR
	LONG		CURRATEFYAVG;		// Currency exchange rate, fiscal year average
	LONG		CURRATEFYEND;		// Currency exchange rate, fiscal year end
	MGUIDE_DATE DTPRICE; 			// Pricing date
	LONG		PRICE;				// Price, closing or last bid
	LONG		PRICEH;				// Price, 12 month high
	LONG		PRICEL;				// Price, 12 month low
	LONG		PR4WCHG; 			// Price, 4 week price percent change
	LONG		PR13WCHG;			// Price, 13 week percent change
	LONG		PR26WCHG;			// Price, 26 week price percent change
	LONG		PR52WCHG;			// Price, 52 week price percent change
	LONG		VOL3MAVG;			// Volume, average monthly trading volume for the last 3 months
	LONG		INSNETTRANS;		// Insider net trades
	LONG		INSBUYTRANS;		// Insider buy transactions
	LONG		INSSELTRANS;		// Insider sell transactions
	LONG		INSNETPURCH;		// Insider net shares bought
	LONG		INSSHRPURCH;		// Insider shares purchased
	LONG		INSHRSOLD;			// Insider shares sold
	LONG		INSTHLD; 			// Institutional percent held
	LONG		INSTITUTES; 		// Institutional number of holders
	LONG		INSTNOSHSHLD;		// Institutional number of shares owned
	LONG		INSTNETPURCH;		// Institutional net shares purchased
	LONG		INSTSHSPURCH;		// Institutional shares bought
	LONG		INSTSHSSOLD;		// Institutional shares sold
	UCHAR 		MM[6][4];			// Market Maker NASD symbol
	LONG		EMPL; 				// Number of employees
} MGUIDE_ID_RECORD;


typedef struct _MGUIDE_FINSUMMARY_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	CHAR		REVGRYRS;			// Growth years, ann. revenue
	CHAR		EPSGRYRS;			// Growth years, EPS
	LONG		FLOATSHRS;			// Shares outstanding less insider control, R144, +5% owners
	LONG		EARNC;				// EPS for the trailing 12 months, current value
	LONG		EARNF;				// EPS excluding extraordinary items, latest fiscal year
	LONG		PEEXCLXOR;			// P/E excluding extraordinary items, TTM
	LONG		QBVPS;				// Book value/share, latest quarter
	LONG		PRICE2BK;			// Price/Book, latest quarter
	LONG		ASHSOUTAVG; 		// Average shares outstanding latest fiscal year
	LONG		SHSOUT;				// Shares currently outstanding
	LONG		TTMREVPS;			// Revenue/share, trailing 12 month
	LONG		QCSHPS;				// Cash per share, latest quarter
	LONG		IAD;				// Dividend rate, indicated annual
	LONG		DIVLQ;				// Dividend, last quarterly declared amount
	MGUIDE_DATE DIVLQXDT;			// Dividend, last quarterly ex-date
	MGUIDE_DATE DIVLQPDT;			// Dividend, last quarterly pay-date
	LONG		YIELD;				// Yield, indicated annual
	LONG		QCURRATIO;			// Current ratio, latest quarter
	LONG		TTMROE;				// Return on average common equity, trailing 12 month
	LONG		TTMROA;				// Return on average assets, trailing 12 month
	LONG		QLTD2EQ; 			// LT debt/total equity, latest quarter
	LONG		TTMINTEXP;			// Interest expense, trailing 12 month
	LONG		MKTCAP;				// Market capitalization
	LONG		TTMTAXPD;			// Taxes paid, trailing 12 month
	LONG		TTMDEPSCF;			// Depreciation expense, (SCF) trailing 12 month
	LONG		TTMEBIT; 			// EBIT, trailing 12 month
	LONG		REVGR;				// Growth rate%, ann. revenue
	LONG		EPSGR;				// Growth rate%, EPS
	LONG		DIVGR;				// Growth rate%, dividend
	LONG		IACGR;				// Growth rate%, income available to common
	LONG		WORKCAPITAL;		// Working capital, latest quarter
	LONG		BETA; 				// Beta, five year monthly
	LONG		QTANBVPS;			// Tangible book value/share, latest quarter
	CHAR		DIVGRYRS;			// Growth years, dividend
	CHAR		IACGRYRS;			// Growth years, income available to common
	UCHAR 		reserved[2];
} MGUIDE_FINSUMMARY_RECORD;

typedef struct _MGUIDE_PERSHAREHISTORY_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		APEPSXCLXORT[20];	// EPS primary excl. ex-ord items title
	LONG		APEPSXCLXOR[5];		// EPS primary excl. ex-ord items for 5 historical fiscal years
	UCHAR 		APEPSINCXORT[20];	// EPS primary incl. ex-ord items title
	LONG		APEPSINCXOR[5];		// EPS primary incl. ex-ord items for 5 historical fiscal years
	UCHAR 		AAVGSHSOUTT[20];	// Avg shares outstanding title
	LONG		AAVGSHSOUT[5]; 		// Avg shares outstanding entry for 5 historical fiscal years
	UCHAR 		ADIVPSTITL[20];		// Dividend paid per share title
	LONG		ADIVPS[5];			// Dividends paid per share for 5 historical fiscal years
} MGUIDE_PERSHAREHISTORY_RECORD;

typedef struct _MGUIDE_INCSELECT_RECORD 
{
	MGUIDE_KEY	key;					// Record key
	UCHAR 		PQFPLEN[2]; 		// Fiscal period length for IS/CF (Prior Year "quarter")
	UCHAR 		PQFPTYP; 			// Fiscal period type for IS/CF (Prior Year "quarter")
	UCHAR 		CQFPLEN[2]; 		// Fiscal period length for IS/CF (Current "quarter")
	UCHAR 		CQFPTYP; 			// Fiscal period type for IS/CF (Current "quarter")
	UCHAR 		PTFPLEN[2]; 		// Fiscal period length for IS/CF (Prior year TTM period)
	UCHAR 		PTFPTYP; 			// Fiscal period type for IS/CF (Prior year TTM period)
	UCHAR 		CTFPLEN[2]; 		// Fiscal period length for IS/CF (Current TTM period)
	UCHAR 		CTPFTYP; 			// Fiscal period type for IS/CF (Current TTM period)
	CHAR		FLASH;				// Indicates whether "current" value comes from a flash report
	UCHAR 		reserved;			// Indicates whether "current" value comes from a flash report
	MGUIDE_DATE PQDTFPEND;			// Ending Date for IS/CF (Prior Year "quarter")
	MGUIDE_DATE CQDTFPEND;			// Ending Date for IS/CF (Current "quarter")
	MGUIDE_DATE PTDTFPEND;			// Ending Date for IS/CF (Prior year TTM period)
	MGUIDE_DATE CTDTFPEND;			// Ending Date for IS/CF (Current TTM period)
	LONG		PQTOTREV;			// Value for Total Revenue Prior Year "quarter"
	LONG		CQTOTREV;			// Value for Total Revenue Current "quarter"
	LONG		PTTOTREV;			// Value for Total Revenue Prior year TTM period
	LONG		CTTOTREV;			// Value for Total Revenue Current TTM period
	LONG		PQTOTEXP;			// Value for Total Expenses Prior Year "quarter"
	LONG		CQTOTEXP;			// Value for Total Expenses Current "quarter"
	LONG		PTTOTEXP;			// Value for Total Expenses Prior year TTM period
	LONG		CTTOTEXP;			// Value for Total Expenses Current TTM period
	LONG		PQTAX;				// Value for Taxes(-credits) Prior Year "quarter"
	LONG		CQTAX;				// Value for Taxes(-credits) Current "quarter"
	LONG		PTTAX;				// Value for Taxes(-credits) Prior year TTM period
	LONG		CTTAX;				// Value for Taxes(-credits) Current TTM period
	LONG		PQAFTTAX;			// Value for After Tax Earnings Prior Year "quarter"
	LONG		CQAFTTAX;			// Value for After Tax Earnings Current "quarter"
	LONG		PTFTTAX; 			// Value for After Tax Earnings Prior year TTM period
	LONG		CTAFTTAX;			// Value for After Tax Earnings Current TTM period
	LONG		PQEPSXCLXOR;		// Value for EPS Excl Ex-Ord Item Prior Year "quarter"
	LONG		CQEPSXCLXOR;		// Value for EPS Excl Ex-Ord Item Current "quarter"
	LONG		PTEPSXCLXOR;		// Value for EPS Excl Ex-Ord Item Prior year TTM period
	LONG		CTEPSXCLXOR;		// Value for EPS Excl Ex-Ord Item Current TTM period
	LONG		PQEPSINCXOR;		// Value for EPS Incl Ex-Ord Item Prior Year "quarter"
	LONG		CQEPSINCXOR;		// Value for EPS Incl Ex-Ord Item Current "quarter"
	LONG		PTEPSINCXOR;		// Value for EPS Incl Ex-Ord Item Prior year TTM period
	LONG		CTEPSINCXOR;		// Value for EPS Incl Ex-Ord Item Current TTM period
	MGUIDE_DATE DTFLASH; 			// Date of "current" period
	UCHAR 		QFN[2][60]; 		// Earnings flash footnote
} MGUIDE_INCSELECT_RECORD;

typedef struct _MGUIDE_INCSUMMARY_RECORD 
{
	MGUIDE_KEY	key;					// Record key
	UCHAR 		reserved[2];
	UCHAR 		ATOTREVTITL[20];	// Total revenue title
	LONG		ATOTREV[5]; 		// Total revenue for 5 historical fiscal years
	UCHAR 		ATOTEXPTITL[20];	// Total expenses title
	LONG		ATOTEXP[5]; 		// Total expenses for 5 historical fiscal years
	UCHAR 		AINCBTAXTITL[20];	// Income before taxes title
	LONG		AINCBTAX[5];		// Income before taxes for 5 historical fiscal years
	UCHAR 		ATAXTITL[20];		// Income taxes (tax credits) title
	LONG		ATAX[5]; 			// Income taxes (tax credits) for 5 historical fiscal years
	UCHAR 		AAFTTAXTITL[20];	// Income after taxes title
	LONG		AAFTTAX[5]; 		// Income after taxes for 5 historical fiscal years
} MGUIDE_INCSUMMARY_RECORD;

typedef struct _MGUIDE_BSSUMMARY_RECORD 
{
	MGUIDE_KEY	key;					// Record key
	UCHAR 		reserved[2];
	LONG		BSPRESCODE;
	// Balance Sheet presentation code, 1 = differentiates; 0 = does not
	UCHAR 		ATOTCURASTT[20];	// Total current assets title
	LONG		ATOTCURAST[2]; 		// Total current assets for 2 historical fiscal years
	UCHAR 		ATOTASTTITL[20];	// Total assets title
	LONG		ATOTAST[2]; 		// Total assets for 2 historical fiscal years
	UCHAR 		ATOTCURLIABT[20];	// Total current liabilities title
	LONG		ATOTCURLIAB[2];		// Total current liabilities for 2 historical fiscal years
	UCHAR 		ATOTLTDTITL[20];	// Total long term debt title
	LONG		ATOTLTD[2]; 		// Total long term debt for 2 historical fiscal years
	UCHAR 		ATOTLIABTITL[20];	// Total liabilities title
	LONG		ATOTLIAB[2];		// Total liabilities for 2 historical fiscal years
	UCHAR 		ATOTEQTITL[20];		// Total equity title
	LONG		ATOTEQ[2];			// Total equity for 2 historical fiscal years
} MGUIDE_BSSUMMARY_RECORD;

typedef struct _MGUIDE_BRIEF1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		BRIEF1[4][60]; 		// Brief part 1 (Currently only 8 lines of the brief are in use.
	UCHAR 		reserved[2];
} MGUIDE_BRIEF1_RECORD;

typedef struct _MGUIDE_BRIEF2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		BRIEF2[4][60]; 		// Brief part 2 (Currently only 8 lines of the brief are in use.
	UCHAR 		reserved[2];
} MGUIDE_BRIEF2_RECORD;

typedef struct _MGUIDE_BRIEF3_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		BRIEFRESVD[4][60];	// Brief part 3 (Currently only 8 lines of the brief are in use.
	UCHAR 		reserved[2];
} MGUIDE_BRIEF3_RECORD;

typedef struct _MGUIDE_CAPITALSTRUCT1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		EQDBT1[4][60]; 		// Equity and debt composition part 1
	UCHAR 		reserved[2];
} MGUIDE_CAPITALSTRUCT1_RECORD;

typedef struct _MGUIDE_CAPITALSTRUCT2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		EQDBT2[2][60]; 		// Equity and debt composition part 2
	UCHAR 		EQDBTRESVD1[2][60];	// Equity and debt composition (Reserved for future expansion) part 1
	UCHAR 		reserved[2];
} MGUIDE_CAPITALSTRUCT2_RECORD;

typedef struct _MGUIDE_CAPITALSTRUCT3_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		EQDBTRESVD2[60];	// Equity and debt composition (Reserved for future expansion) part 2
	UCHAR 		reserved[2];
} MGUIDE_CAPITALSTRUCT3_RECORD;

typedef struct _MGUIDE_ANALYST_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		ANALYST[40];		// Responsible analyst name
	UCHAR 		FN[3][60];			// Analyst's footnotes
	UCHAR 		reserved[2];
} MGUIDE_ANALYST_RECORD;

typedef struct _MGUIDE_RESTATEMENT_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	CHAR		AFPLEN[5];			// Annual fiscal period length
	UCHAR 		AFPTYP[5];			// Annual fiscal period type
	MGUIDE_DATE AFPENDDT[5];		// Annual fiscal period ending date
	UCHAR 		AUPDSTATIS[5]; 		// Annual fiscal period update status, income statement
	UCHAR 		AUPDSTATBS[2]; 		// Annual fiscal period update status, balance statement
	UCHAR 		reserved[1];
	MGUIDE_DATE ARESTDTIS[5];		// Annual restatement date, income statement
	MGUIDE_DATE ARESTDTBS[2];		// Annual restatement date, balance statement
} MGUIDE_RESTATEMENT_RECORD;

typedef struct _MGUIDE_OFFICERS1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		OFFICERS1[4][60];	// Corporate officers names and titles part 1
	UCHAR 		reserved[2];
} MGUIDE_OFFICERS1_RECORD;

typedef struct _MGUIDE_OFFICERS2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		OFFICERS2[60]; 		// Corporate officers names and titles part 2
	UCHAR 		reserved[2];
} MGUIDE_OFFICERS2_RECORD;

typedef struct _MGUIDE_INCDETAIL1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AREV1TITL[20]; 		// Revenue item 1 title
	LONG		AREV1[5];			// Revenue entry for 5 historical fiscal years
	UCHAR 		AREV2TITL[20]; 		// Revenue item 2 title
	LONG		AREV2[5];			// Revenue entry for 5 historical fiscal years
	UCHAR 		AREV3TITL[20]; 		// Revenue item 3 title
	LONG		AREV3[5];			// Revenue entry for 5 historical fiscal years
	UCHAR 		AREV4TITL[20]; 		// Revenue item 4 title
	LONG		AREV4[5];			// Revenue entry for 5 historical fiscal years
	UCHAR 		AREV5TITL[20]; 		// Revenue item 5 title
	LONG		AREV5[5];			// Revenue entry for 5 historical fiscal years
	UCHAR 		AREV6TITL[20]; 		// Revenue item 6 title
	LONG		AREV6[5];			// Revenue entry for 5 historical fiscal years
} MGUIDE_INCDETAIL1_RECORD;

typedef struct _MGUIDE_INCDETAIL2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AEXP1TITL[20]; 		// Expense item 1 title
	LONG		AEXP1[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		AEXP2TITL[20];		// Expense item 2 title
	LONG		AEXP2[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		AEXP3TITL[20]; 		// Expense item 3 title
	LONG		AEXP3[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		AEXP4TITL[20];		// Expense item 4 title
	LONG		AEXP4[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		AEXP5TITL[20]; 		// Expense item 5 title
	LONG		AEXP5[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		AEXP6TITL[20]; 		// Expense item 6 title
	LONG		AEXP6[5];			// Expense entry for 5 historical fiscal years
} MGUIDE_INCDETAIL2_RECORD;

typedef struct _MGUIDE_INCDETAIL3_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AEXP7TITL[20]; 		// Expense item 7 title
	LONG		AEXP7[5];			// Expense entry for 5 historical fiscal years
	UCHAR 		ANOPINC1TITL[20];	// Non-operating income item 1 title
	LONG		ANOPINC1[5];		// Non-operating income entry for 5 historical fiscal years
	UCHAR 		ANOPINC2TITL[20];	// Non-operating income item 2 title
	LONG		ANOPINC2[5];		// Non-operating income entry for 5 historical fiscal years
	UCHAR 		AINCADJ1TITL[20];	// Adjustments to income 1 title
	LONG		AINCADJ1[5];		// Adjustments to income for 5 historical fiscal years
	UCHAR 		AINCADJ2TITL[20];	// Adjustments to income 2 title
	LONG		AINCADJ2[5];		// Adjustments to income for 5 historical fiscal years
	UCHAR 		AINCADJ3TITL[20];	// Adjustments to income 3 title
	LONG		AINCADJ3[5];		// Adjustments to income for 5 historical fiscal years
} MGUIDE_INCDETAIL3_RECORD;

typedef struct _MGUIDE_INCDETAIL4_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AXORD1TITL[20];		// Ex-ord itm & disc opns item 1 title
	LONG		AXORD1[5];			// Ex-ord itm & disc opns entry for 5 historical fiscal years
	UCHAR 		AXORD2TITL[20];		// Ex-ord itm & disc opns item 2 title
	LONG		AXORD2[5];			// Ex-ord itm & disc opns entry for 5 historical fiscal years
	UCHAR 		AXORD3TITL[20];		// Ex-ord itm & disc opns item 3 title
	UCHAR 		AXORD3[5];			// Ex-ord itm & disc opns entry for 5 historical fiscal years
} MGUIDE_INCDETAIL4_RECORD;


typedef struct _MGUIDE_BSDETAIL1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		ACURAST1TITL[20];	// Current asset item 1 title
	LONG		ACURAST1[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST2TITL[20];	// Current asset item 2 title
	LONG		ACURAST2[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST3TITL[20];	// Current asset item 3 title
	LONG		ACURAST3[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST4TITL[20];	// Current asset item 4 title
	LONG		ACURAST4[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST5TITL[20];	// Current asset item 5 title
	LONG		ACURAST5[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST6TITL[20];	// Current asset item 6 title
	LONG		ACURAST6[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST7TITL[20];	// Current asset item 7 title
	LONG		ACURAST7[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST8TITL[20];	// Current asset item 8 title
	LONG		ACURAST8[2];		// Current asset entry for 2 historical fiscal years
} MGUIDE_BSDETAIL1_RECORD;

typedef struct _MGUIDE_BSDETAIL2_RECORD 
{
	MGUIDE_KEY	key;					// Record key
	UCHAR 		reserved[2];
	UCHAR 		ACURAST9TITL[20];	// Current asset item 9 title
	LONG		ACURAST9[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ACURAST10TITL[20];	// Current asset item 10 title
	LONG		ACURAST10[2];		// Current asset entry for 2 historical fiscal years
	UCHAR 		ALTAST1TITL[20];	// Long term asset item 1 title
	LONG		ALTAST1[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST2TITL[20];	// Long term asset item 2 title
	LONG		ALTAST2[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST3TITL[20];	// Long term asset item 3 title
	LONG		ALTAST3[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST4TITL[20];	// Long term asset item 4 title
	LONG		ALTAST4[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST5TITL[20];	// Long term asset item 5 title
	LONG		ALTAST5[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST6TITL[20];	// Long term asset item 6 title
	LONG		ALTAST6[2]; 		// Long term asset for 2 historical fiscal years
} MGUIDE_BSDETAIL2_RECORD;

typedef struct _MGUIDE_BSDETAIL3_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		ALTAST7TITL[20];	// Long term asset item 7 title
	LONG		ALTAST7[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST8TITL[20];	// Long term asset item 8 title
	LONG		ALTAST8[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ALTAST9TITL[20];	// Long term asset item 9 title
	LONG		ALTAST9[2]; 		// Long term asset for 2 historical fiscal years
	UCHAR 		ACURLIAB1T[20];		// Current liability item 1 title
	LONG		ACURLIAB1[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB2T[20];		// Current liability item 2 title
	LONG		ACURLIAB2[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB3T[20];		// Current liability item 3 title
	LONG		ACURLIAB3[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB4T[20];		// Current liability item 4 title
	LONG		ACURLIAB4[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB5T[20];		// Current liability item 5 title
	LONG		ACURLIAB5[2];		// Current liability entry for 2 historical fiscal years
} MGUIDE_BSDETAIL3_RECORD;

typedef struct _MGUIDE_BSDETAIL4_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		ACURLIAB6T[20];		// Current liability item 6 title
	LONG		ACURLIAB6[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB7T[20];		// Current liability item 7 title
	LONG		ACURLIAB7[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB8T[20];		// Current liability item 8 title
	LONG		ACURLIAB8[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB9T[20];		// Current liability item 9 title
	LONG		ACURLIAB9[2];		// Current liability entry for 2 historical fiscal years
	UCHAR 		ACURLIAB10T[20];	// Current liability item 10 title
	LONG		ACURLIAB10[2]; 		// Current liability entry for 2 historical fiscal years
	UCHAR 		ALTD1TITL[20]; 		// Long term debt item 1 title
	LONG		ALTD1[2];			// Long term debt entry for 2 historical fiscal years
	UCHAR 		ALTD2TITL[20]; 		// Long term debt item 2 title
	LONG		ALTD2[2];			// Long term debt entry for 2 historical fiscal years
	UCHAR 		ALTD3TITL[20]; 		// Long term debt item 3 title
	LONG		ALTD3[2];			// Long term debt entry for 2 historical fiscal years
} MGUIDE_BSDETAIL4_RECORD;

typedef struct _MGUIDE_BSDETAIL5_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AOLTLIAB1T[20];		// Other long term liability item 1 title
	LONG		AOLTLIAB1[2];		// Other long term liability entry for 2 historical fiscal years
	UCHAR 		AOLTLIAB2T[20];		// Other long term liability item 2 title
	LONG		AOLTLIAB2[2];		// Other long term liability entry for 2 historical fiscal years
	UCHAR 		AOLTLIAB3T[20];		// Other long term liability item 3 title
	LONG		AOLTLIAB3[2];		// Other long term liability entry for 2 historical fiscal years
	UCHAR 		AEQ1TITL[20];		// Equity item 1 title
	LONG		AEQ1[2]; 			// Equity entry for 2 historical fiscal years
	UCHAR 		AEQ2TITL[20];		// Equity item 2 title
	LONG		AEQ2[2]; 			// Equity entry for 2 historical fiscal years
	UCHAR 		AEQ3TITL[20];		// Equity item 3 title
	LONG		AEQ3[2]; 			// Equity entry for 2 historical fiscal years
	UCHAR 		AEQ4TITL[20];		// Equity item 4 title
	LONG		AEQ4[2]; 			// Equity entry for 2 historical fiscal years
	UCHAR 		AEQ5TITL[20];		// Equity item 5 title
	LONG		AEQ5[2]; 			// Equity entry for 2 historical fiscal years
} MGUIDE_BSDETAIL5_RECORD;

typedef struct _MGUIDE_CASHFLOW_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	UCHAR 		AEQ6TITL[20];		// Equity item 6 title
	LONG		AEQ6[2]; 			// Equity entry for 2 historical fiscal years
	LONG		PQNI; 				// Value for Net Income Prior Year "quarter"
	LONG		CQNI; 				// Value for Net Income Current "quarter"
	LONG		PTNI; 				// Value for Net Income Prior year TTM period
	LONG		CTNI; 				// Value for Net Income Current TTM period
	LONG		PQDEPR;				// Value for Depreciation Prior Year "quarter"
	LONG		CQDEPR;				// Value for Depreciation Current "quarter"
	LONG		PTDEPR;				// Value for Depreciation Prior year TTM period
	LONG		CTDEPR;				// Value for Depreciation Current TTM period
	LONG		PQCASHOP;			// Value for Cash From Operations Prior Year "quarter"
	LONG		CQCASHOP;			// Value for Cash From Operations Current "quarter"
	LONG		PTCASHOP;			// Value for Cash From operations Prior year TTM period
	LONG		CTCASHOP;			// Value for Cash From Operations Current TTM period
	LONG		PQCAPEXP;			// Value for Capital Expenditures Paid Prior Year "quarter"
	LONG		CQCAPEXP;			// Value for Capital Expenditures Paid Current "quarter"
	LONG		PTCAPEXP;			// Value for Capital Expenditures Prior Paid year TTM period
	LONG		CTCAPEXP;			// Value for Capital Expenditures Paid Current TTM period
	LONG		PQCASHINV;			// Value for Cash From Investing Prior Year "quarter"
	LONG		CQCASHINV;			// Value for Cash From Investing Current "quarter"
	LONG		PTCASHINV;			// Value for Cash From Investing Prior year TTM period
	LONG		CTCASHINV;			// Value for Cash From Investing Current TTM period
	LONG		PQDIV;				// Value for Dividends Prior Year "quarter"
	LONG		CQDIV;				// Value for Dividends Current "quarter"
	LONG		PTDIV;				// Value for Dividends Prior year TTM period
	LONG		CTDIV;				// Value for Dividends Current TTM period
	LONG		PQCASHFIN;			// Value for Cash From Financing Prior Year "quarter"
	LONG		CQCASHFIN;			// Value for Cash From Financing Current "quarter"
	LONG		PTCASHFIN;			// Value for Cash From Financing Prior year TTM period
	LONG		CTCASHFIN;			// Value for Cash From Financing Current TTM period
	LONG		PQFXEFF; 			// Value for Foreign Exchange Effects Prior Year "quarter"
	LONG		CQFXEFF; 			// Value for Foreign Exchange Effects Current "quarter"
	LONG		PTFXEFF; 			// Value for Foreign Exchange Effects Prior year TTM period
	LONG		CTFXEFF; 			// Value for Foreign Exchange Effects Current TTM period
} MGUIDE_CASHFLOW_RECORD;

typedef struct _MGUIDE_INCQUARTERLY_RECORD 
{
	MGUIDE_KEY	key;					// Record key
	SHORT 		CURRYR;				// Current fiscal year for quarterly data
	CHAR		MON[4];				// Month quarter ends
	LONG		QSREV4[4];			// Quarterly revenue for qtrs 1 to 4 of the 4th hist. fiscal year
	LONG		QSREV3[4];			// Quarterly revenue for qtrs 1 to 4 of the 3rd hist. fiscal year
	LONG		QSREV2[4];			// Quarterly revenue for qtrs 1 to 4 of the 2nd hist. fiscal year
	LONG		QSREV1[4];			// Quarterly revenue for qtrs 1 to 4 of the 1st hist. fiscal year
	LONG		QSREV0[4];			// Quarterly revenue for qtrs 1 to 4 of the current fiscal year
	LONG		QSEPS4[4];			// Quarterly EPS for qtrs 1 to 4 of the 4th historical fiscal year
	LONG		QSEPS3[4];			// Quarterly EPS for qtrs 1 to 4 of the 3rd historical fiscal year
	LONG		QSEPS2[4];			// Quarterly EPS for qtrs 1 to 4 of the 2nd historical fiscal year
	LONG		QSEPS1[4];			// Quarterly EPS for qtrs 1 to 4 of the 1st historical fiscal year
	LONG		QSEPS0[4];			// Quarterly EPS for qtrs 1 to 4 of the current fiscal year
} MGUIDE_INCQUARTERLY_RECORD;

typedef struct _MGUIDE_BSQUARTERLY_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		reserved[2];
	MGUIDE_DATE BSDATE[4];			// As of Date code for Balance Sheet for 4 hist. quarterly periods
	LONG		BSCASHSTINV[4];		// Value for cash/st invest. for 4 hist. quarterly periods
	LONG		BSTOTCURAST[4];		// Value for total current assets for 4 hist. quarterly periods
	LONG		BSTOTAST[4];		// Value for total assets for 4 hist. quarterly periods
	LONG		BSTOTCURLIAB[4];	// Value for total current liab. for 4 hist. quarterly periods
	LONG		BSTOTSTLTDBT[4];	// Value for total ST+LT debt for 4 hist. quarterly periods
	LONG		BSTOTLIAB[4];		// Value for total liab. for 4 hist. quarterly periods
	LONG		BSSHRHLDEQ[4]; 		// Value for shareholder equity for 4 hist. quarterly periods
	LONG		BSSHSOUT[4];		// Value for shares outstanding for 4 hist. quarterly periods
} MGUIDE_BSQUARTERLY_RECORD;

typedef struct _MGUIDE_INDUSTRY1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		SIC[4][4];			// SIC code
	UCHAR 		SICNAME[4][35];		// SIC Name
	UCHAR 		INDNAME1[2][34];	// Industry Name part 1
	UCHAR 		reserved[2];
} MGUIDE_INDUSTRY1_RECORD;

typedef struct _MGUIDE_INDUSTRY2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		INDNAME2[2][34];	// Industry Name part 2
	UCHAR 		SUBINDNAME[4][34];	// Sub-Industry Name
	UCHAR 		reserved[2];
} MGUIDE_INDUSTRY2_RECORD;

typedef struct _MGUIDE_PRODUCT1_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		PRODUCT1[5][42];	// Product Name part 1
	UCHAR 		reserved[2];
} MGUIDE_PRODUCT1_RECORD;

typedef struct _MGUIDE_PRODUCT2_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		PRODUCT2[5][42];	// Product Name part 2
	UCHAR 		reserved[2];
} MGUIDE_PRODUCT2_RECORD;

typedef struct _MGUIDE_ADDRESS_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	UCHAR 		ADDR1[30];			// Company address line 1
	UCHAR 		ADDR2[30];			// Company address line 2
	UCHAR 		ADDR3[30];			// Company address line 3
	UCHAR 		CITY[30];			// City
	UCHAR 		STATE[2];			// State
	UCHAR 		ZIP[10]; 			// Zip
	UCHAR 		COUNTRY[20];		// Country name
	UCHAR 		PHONE[14];			// Phone number with area code
	UCHAR 		CONTACTFNM[20];		// Contact first name
	UCHAR 		CONTACTLNM[20];		// Contact last name
	UCHAR 		CONTACTTITL[30];	// Contact title
	UCHAR 		reserved[2];
} MGUIDE_ADDRESS_RECORD;

typedef struct _MGUIDE_UNDEFINED_RECORD 
{
	MGUIDE_KEY	key;				// Record key
	CHAR		data[250];
} MGUIDE_UNDEFINED_RECORD;

typedef union _MGUIDE_RECORD 
{
	MGUIDE_ID_RECORD				id;
	MGUIDE_FINSUMMARY_RECORD		finSummary;
	MGUIDE_PERSHAREHISTORY_RECORD	perShareHistory;
	MGUIDE_INCSELECT_RECORD 		incSelect;
	MGUIDE_INCSUMMARY_RECORD		incSummary;
	MGUIDE_BSSUMMARY_RECORD 		bsSummary;
	MGUIDE_BRIEF1_RECORD 			brief1;
	MGUIDE_BRIEF2_RECORD 			brief2;
	MGUIDE_BRIEF3_RECORD 			brief3;
	MGUIDE_CAPITALSTRUCT1_RECORD	capitalStruct1;
	MGUIDE_CAPITALSTRUCT2_RECORD	capitalStruct2;
	MGUIDE_CAPITALSTRUCT3_RECORD	capitalStruct3;
	MGUIDE_ANALYST_RECORD			analyst;
	MGUIDE_RESTATEMENT_RECORD		restatement;
	MGUIDE_OFFICERS1_RECORD 		officers1;
	MGUIDE_OFFICERS2_RECORD 		officers2;
	MGUIDE_INCDETAIL1_RECORD		incDetail1;
	MGUIDE_INCDETAIL2_RECORD		incDetail2;
	MGUIDE_INCDETAIL3_RECORD		incDetail3;
	MGUIDE_INCDETAIL4_RECORD		incDetail4;
	MGUIDE_BSDETAIL1_RECORD 		bsDetail1;
	MGUIDE_BSDETAIL2_RECORD 		bsDetail2;
	MGUIDE_BSDETAIL3_RECORD 		bsDetail3;
	MGUIDE_BSDETAIL4_RECORD 		bsDetail4;
	MGUIDE_BSDETAIL5_RECORD 		bsDetail5;
	MGUIDE_CASHFLOW_RECORD			cashFlow;
	MGUIDE_INCQUARTERLY_RECORD 		incQuarterly;
	MGUIDE_BSQUARTERLY_RECORD		bsQuarterly;
	MGUIDE_INDUSTRY1_RECORD 		industry1;
	MGUIDE_INDUSTRY2_RECORD 		industry2;
	MGUIDE_PRODUCT1_RECORD			product1;
	MGUIDE_PRODUCT2_RECORD			product2;
	MGUIDE_ADDRESS_RECORD			address;
	MGUIDE_UNDEFINED_RECORD 		undefined;
	CHAR							filler[256];
} MGUIDE_RECORD;		// 256 bytes.

#define DBA_RECORD_58 MGUIDE_RECORD

#endif

#ifndef INCL_NOPACKING
	#pragma pack()
#endif

