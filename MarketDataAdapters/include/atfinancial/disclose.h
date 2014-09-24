/*
 *		disclose.h
 *
 *		Disclosure / Btrieve Database
 *
 *		Written 03/22/96	Steve Pingry
 *
 *		Copyright (c) A-T Financial Information, Inc., 1996.
 *		All rights reserved.
 *
 */

#if !defined(_DISCLOSE_H)
#define	_DISCLOSE_H

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

// disclosure key types
#define DKT_DCN						0
#define DKT_TICKER					1
#define DKT_CUSIP					2
#define DKT_COMPANY_NAME			3
#define DKT_POSITION				4

// Disclosure record types
#define DRT_RESUME						0
#define DRT_BUSINESS_DESC				1
#define DRT_FILINGS_TABLE				2
#define DRT_AUDITORS_RPT				3
#define DRT_BS_ASSETS					4
#define DRT_BS_LIABILITIES				5
#define DRT_INCOME_STATEMENT			6	
#define DRT_CASHFLOW_STATEMENT			7
#define DRT_RATIO_ANALYSIS				8
#define DRT_RATIO_ANALYSIS_SEG_DATA		9
#define DRT_5YEAR_SUMMARY				10
#define DRT_FIN_DATA_COMMENTS			11
#define DRT_MULLER_PRICING				12
#define DRT_MULLER_OFFICERS				13
#define DRT_MULLER_DIRECTORS			14
#define DRT_MULLER_OWNERSHIP			15
#define DRT_MULLER_SUBSIDIARIES			16
#define DRT_MULLER_EXHIBITS				17
#define DRT_MULLER_OTHER_EVENTS			18
#define DRT_MULLER_PRES_LETTER			19
#define DRT_MULLER_MGMT_DISCUSSION		20
#define DRT_MULLER_FOOTNOTES			21
#define DRT_MULLER_ASSETS_TABLE			22
#define DRT_MULLER_LIAB_TABLE			23
#define DRT_MULLER_INCOME_STMT_TABLE	24
#define DRT_COMPRESSION_SCALE			25	// special record that keeps the compresion scale
#define DRT_MAX							DRT_COMPRESSION_SCALE

// disclosure server errors returned in status field of response
#define DISCERR_SUCCESS				0		// successful call
#define DISCERR_BTR_OPEN			1		// disclosure btrieve open error
#define DISCERR_INVALID_KEY			2		// invalid key number specified
#define DISCERR_INVALID_FUNC		3		// invalid function specified
#define DISCERR_INVALID_MSG_TYPE	4		// invalid message type ('d' '?')
#define DISCERR_KEY_NOT_FOUND		5		// key not found in database
#define DISCERR_SERVER_BUSY			6		// server could not process request at this time
#define DISCERR_SERVER_SHUTDOWN		7		// server could not process request, shutting down
#define DISCERR_NOT_LOGGED_IN		8		// server could not process request, workstation not logged in

// disclosure function defines for 'd' 'r' messages
#define DISC_GET_EQUAL	0			// regular old get equal
#define DISC_GET_LEQUAL	1			// get less than or equal
#define DISC_GET_GEQUAL	2			// get greater than or equal
#define DISC_MAX_FUNC  DISC_GET_GEQUAL

// disclosure direction defines for 'd' 's' messages
#define DISC_SRCH_FORWARD	0		// forward/greater-than or equal
#define DISC_SRCH_BACKWARD	1		// backward/less-than or equal

// disclosure misc defines
#define MAX_UPDATE_CHUNK	64000			// max btrieve chunk size
#define MAX_DISC_RSP_DATA	512				// max data response size (block)

// ===========================================================================
// disclosure date field -- Day, Month, Year
typedef	struct tagDISC_DATE
{
	WORD	day:  5,		// Lower  5 bits = day   (1 - 31)
			month:4,		// Middle 4 bits = month (1 - 12)
			year: 7;		// Upper  7 bits = year	 (0 - 99) NOTE: Offset from 1970
} DISC_DATE, *LPDISC_DATE;

// maximum value stored in the value field (28 bits)
#define MAX_DISC_VALUE_EXP	268435456
// ===========================================================================
// disclosure value field with exponent of 10
// structure to compress up to 15 digit positive and 14 digit negative numbers
typedef	struct tagDISC_VALUE_EXP
{
	DWORD	value:  28,		// Lower  28 bits = value
			sign:    1,		// Middle 1 bit = sign ( 0 == positive or zero, 1 == negative
			exponent:3;		// Upper 3 bits = exponent of 10 (ie. how many zeros should be tagged to the end)
} DISC_VALUE_EXP, *LPDISC_VALUE_EXP;

// maximum value stored in the value field (27 bits)
#define MAX_DISC_VALUE_DEC	134217728
// ===========================================================================
// disclosure value field with implied decimals
// structure to compress up to 8 floating point digits
typedef	struct tagDISC_VALUE_DEC
{
	DWORD	value:	27,		// Lower  27 bits = value
			sign:	1,		// Middle 1 bit = sign ( 0 == positive or zero, 1 == negative
			dec:	4;		// Upper 4 bits = decimal places
} DISC_VALUE_DEC, *LPDISC_VALUE_DEC;

// ===========================================================================
// disclosure company number
typedef struct tagDK_DCN
{
	CHAR	sDisclosureCompanyNumber[10];	// unique disclosure company number
	WORD	wDisclosureRecordType;			// Disclosure record type
	CHAR	sPeriodCode[6];					// (e.g. "     " = NA, "A 1996" = Annual for 1996, "1 1996" = first quarter 1996
} DK_DCN, *LPDK_DCN;

// ===========================================================================
// ticker key
typedef struct tagDK_TICKER
{
	CHAR	sTicker[6];				// ticker symbol
	WORD	wDisclosureRecordType;	// Disclosure record type
	CHAR	sPeriodCode[6];			// (e.g. "     " = NA, "A 1996" = Annual for 1996, "1 1996" = first quarter 1996
} DK_TICKER, *LPDK_TICKER;

// ===========================================================================
// CUSIP key
typedef struct tagDK_CUSIP
{
	CHAR	sCusip[10];				// CUSIP
	WORD	wDisclosureRecordType;	// Disclosure record type
	CHAR	sPeriodCode[6];			// (e.g. "     " = NA, "A 1996" = Annual for 1996, "1 1996" = first quarter 1996
} DK_CUSIP, *LPDK_CUSIP;

// ===========================================================================
// company name key
typedef struct tagDK_COMPANY_NAME
{
	CHAR	sCompanyName[40];		// company name
	WORD	wDisclosureRecordType;	// Disclosure record type
	CHAR	sPeriodCode[6];			// (e.g. "     " = NA, "A 1996" = Annual for 1996, "1 1996" = first quarter 1996
} DK_COMPANY_NAME, *LPDK_COMPANY_NAME;

// ===========================================================================
// position key (btrive)
typedef struct tagDK_POSITION
{
	DWORD	dwPosition;
} DK_POSITION, *LPDK_POSITION;

typedef union tagDISC_KEYS
{
	UCHAR			key[1];			// general purpose member to access this in a generic way
	DK_DCN			dcn;
	DK_TICKER		ticker;
	DK_CUSIP		cusip;
	DK_COMPANY_NAME	companyName;
	DK_POSITION		position;
} DISC_KEYS, *LPDISC_KEYS;

typedef struct tagRECORD_HEADER
{
	DWORD	dwRecordLength;				// total record length (including variable)
	WORD	wType;						// Disclosure record type
	WORD	wCompressionFlag:		1,	// flag for indicating if record is compressed
			wCompressionStartBlock: 7,	// indicates which block the compression starts (0 - n)
			resv:					8;	// Upper 15 bits = reserved
} RECORD_HEADER, *LPRECORD_HEADER;		// 8 bytes

// ===========================================================================
// disclosure header 
typedef struct tagDISCLOSURE_HEADER
{
	RECORD_HEADER	recordHeader;
	CHAR			sPeriodCode[6];					// (e.g. "     " = NA, "A 1996" = Annual for 1996, "1 1996" = first quarter 1996
	CHAR			sTicker[6];						// ticker symbol
	CHAR			sCusip[10];						// CUSIP
	CHAR			sCompanyName[40];				// company name
	CHAR			sDisclosureCompanyNumber[10];	// unique disclosure company number
} DISCLOSURE_HEADER, *LPDISCLOSURE_HEADER;			// 80 bytes

// ===========================================================================
// disclosure record resume fields (compressed portion of the DR_RESUME)
typedef struct tagDR_RESUME_FIELDS
{
	CHAR	sCrossReference[51];
	CHAR	sCompanyStatus[154];
	CHAR	sStreetAddress1[40];
	CHAR	sStreetAddress2[40];
	CHAR	sStreetAddress3[40];
	CHAR	sCity[50];
	CHAR	sState[2];
	CHAR	sZipCode[9];
	CHAR	sTelephone[12];
	CHAR	sIncorporation[22];
	CHAR	sExchange[3];
	CHAR	sFortuneNumber[4];
	CHAR	sForbesNumbers[23];
	CHAR	sDUNSNumber[11];
	CHAR	sSICCodes[34];
	CHAR	sPrimarySICCode[4];
	CHAR	sCurrentSharesOutstanding[42];
	CHAR	sSharesHeldByOffsAndDirs[25];
	CHAR	sShareholders[25];
	CHAR	sEmployees[25];
	CHAR	sLegalCounsel[77];
	CHAR	sStockTransferAgent[77];
	CHAR	sAuditorChange[154];
	CHAR	sAuditor[77];				
} DR_RESUME_FIELDS, *LPDR_RESUME_FIELDS;

// ===========================================================================
// disclosure record resume
typedef struct tagDR_RESUME
{
	DISC_DATE	fiscalYearEnd;			// Note: the year field in this date should be ignored
	DISC_DATE	latestAnnualData;
	DISC_DATE	latestQuarterlyData;
	union
	{
		CHAR			 sVarLenField[1];	// generic name for ease of use (e.g. offsetof)
		DR_RESUME_FIELDS drResumeFields;	// descriptive name for whats actually in here
	};										// the second block is compressed so we can get it into two packets
} DR_RESUME, *LPDR_RESUME;

// ===========================================================================
// disclosure record business description
typedef struct tagDR_BUSINESS_DESC
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sBusinessDescription[1];	// descriptive name for whats actually in here
	};
} DR_BUSINESS_DESC, *LPDR_BUSINESS_DESC;

// ===========================================================================
// disclosure record filings table
typedef struct tagDR_FILINGS_TABLE
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sFilingsTable[1];			// descriptive name for whats actually in here
	};
} DR_FILINGS_TABLE, *LPDR_FILINGS_TABLE;

// ===========================================================================
// disclosure record auditors report
typedef struct tagDR_AUDITORS_RPT
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sAuditorsReport[1];			// descriptive name for whats actually in here
	};
} DR_AUDITORS_RPT, *LPDR_AUDITORS_RPT;

// ===========================================================================
// disclosure record balance sheet - assets
typedef struct tagDR_BS_ASSETS
{
	DISC_VALUE_EXP  dveCash;
	DISC_VALUE_EXP  dveMarketableSecurities;
	DISC_VALUE_EXP  dveReceivables;
	DISC_VALUE_EXP  dveInventories;
	DISC_VALUE_EXP  dveRawMaterials;
	DISC_VALUE_EXP  dveWorkInProgress;
	DISC_VALUE_EXP  dveFinishedGoods;
	DISC_VALUE_EXP  dveNotesReceivable;
	DISC_VALUE_EXP  dveOtherCurrentAssets;
	DISC_VALUE_EXP  dveCurrentAssets;
	DISC_VALUE_EXP  dvePropertyPlantEquipment;
	DISC_VALUE_EXP  dveAccumulatedDepreciation;
	DISC_VALUE_EXP  dveNetPropertyPlantEquipment;
	DISC_VALUE_EXP  dveInvestmentAdvancesToSubs;
	DISC_VALUE_EXP  dveOtherNonCurrentAssets;
	DISC_VALUE_EXP  dveDeferredCharges;
	DISC_VALUE_EXP  dveIntangibles;
	DISC_VALUE_EXP  dveDepositsAndOtherAssets;
	DISC_VALUE_EXP  dveTotalAssets;
} DR_BS_ASSETS, *LPDR_BS_ASSETS;

// ===========================================================================
// disclosure record balance sheet - liabilities
typedef struct tagDR_BS_LIABILITIES
{
	DISC_VALUE_EXP  dveNotesPayable;
	DISC_VALUE_EXP  dveAccountsPayable;
	DISC_VALUE_EXP  dveCurrentLongTermDebt;
	DISC_VALUE_EXP  dveCurrentPortionOfCapLeases;
	DISC_VALUE_EXP  dveAccruedExpenses;
	DISC_VALUE_EXP  dveIncomeTaxes;
	DISC_VALUE_EXP  dveOtherCurrentLiabilities;
	DISC_VALUE_EXP  dveTotalCurrentLiabilities;
	DISC_VALUE_EXP  dveMortgages;
	DISC_VALUE_EXP  dveDeferredChargesIncome;
	DISC_VALUE_EXP  dveConvertibleDebt;
	DISC_VALUE_EXP  dveLongTermDebt;
	DISC_VALUE_EXP  dveNonCurrentCapitalLeases;
	DISC_VALUE_EXP  dveOtherLongTermLiabilities;
	DISC_VALUE_EXP  dveTotalLiabilities;
	DISC_VALUE_EXP  dveMinorityInterest;
	DISC_VALUE_EXP  dvePreferredStock;
	DISC_VALUE_EXP  dveCommonStockNet;
	DISC_VALUE_EXP  dveCapitalSurplus;
	DISC_VALUE_EXP  dveRetainedEarnings;
	DISC_VALUE_EXP  dveTreasuryStock;
	DISC_VALUE_EXP  dveOtherLiabilities;
	DISC_VALUE_EXP  dveShareholdersEquity;
	DISC_VALUE_EXP  dveTotalLiabilitiesNetWorth;
} DR_BS_LIABILITIES, *LPDR_BS_LIABILITIES;

// ===========================================================================
// disclosure record income statement
typedef struct tagDR_INCOME_STATEMENT
{
	DISC_VALUE_EXP  dveNetSales;
	DISC_VALUE_EXP  dveCostOfGoods;
	DISC_VALUE_EXP  dveGrossProfit;
	DISC_VALUE_EXP  dveRDExpenditures;
	DISC_VALUE_EXP  dveSellGenAdminExpenses;
	DISC_VALUE_EXP  dveIncomeBeforeDeprecAmort;
	DISC_VALUE_EXP  dveDepreciationAmortization;
	DISC_VALUE_EXP  dveNonOperatingIncome;
	DISC_VALUE_EXP  dveInterestExpense;
	DISC_VALUE_EXP  dveIncomeBeforeTax;
	DISC_VALUE_EXP  dveProvisionForIncomeTaxes;
	DISC_VALUE_EXP  dveMinorityInterest;
	DISC_VALUE_EXP  dveInvestmentGainsLosses;
	DISC_VALUE_EXP  dveOtherIncome;
	DISC_VALUE_EXP  dveNetIncomeBeforeExtraItems;
	DISC_VALUE_EXP  dveExtraItemsAndDiscontinuedOps;
	DISC_VALUE_EXP  dveNetIncome;
	DISC_VALUE_EXP  dveOutstandingShares;
} DR_INCOME_STATEMENT, *LPDR_INCOME_STATEMENT;

// ===========================================================================
// disclosure record cash flow statement
typedef struct tagDR_CASHFLOW_STATEMENT
{
	DISC_VALUE_EXP  dveNetIncome;
	DISC_VALUE_EXP  dveDepreciationAmortization;
	DISC_VALUE_EXP  dveNetIncInAssetsLiabs;
	DISC_VALUE_EXP  dveCashProvidedByOps;
	DISC_VALUE_EXP  dveOtherAdjustmentsNet;
	DISC_VALUE_EXP  dveNetCashProvidedByOps;
	DISC_VALUE_EXP  dveDecPropertyAndPlant;
	DISC_VALUE_EXP  dveAcqDispSubsidOrOthBusiness;
	DISC_VALUE_EXP  dveIncInInvestments;
	DISC_VALUE_EXP  dveOtherCashInflow;
	DISC_VALUE_EXP  dveNetCashProvByInvestment;
	DISC_VALUE_EXP  dveIssuancesEquitySecurity;
	DISC_VALUE_EXP  dveIssuancesDebtSecurity;
	DISC_VALUE_EXP  dveIncBankOtherBorrowings;
	DISC_VALUE_EXP  dveDividendsOtherDistributions;
	DISC_VALUE_EXP  dveOtherCashInflow2;
	DISC_VALUE_EXP  dveNetCashProvByFinancing;
	DISC_VALUE_EXP  dveEffectOfExchRatesOnCash;
	DISC_VALUE_EXP  dveNetChangesCashNCashEquiv;
	DISC_VALUE_EXP  dveCashNCashEquivStartOfYear;
	DISC_VALUE_EXP  dveCashNCashEquivEndOfYear;
} DR_CASHFLOW_STATEMENT, *LPDR_CASHFLOW_STATEMENT;

// ===========================================================================
// disclosure record ratio analysis
typedef struct tagDR_RATIO_ANALYSIS
{
	DISC_VALUE_DEC	dvdQuickRatio;
	DISC_VALUE_DEC	dvdCurrentRatio;
	DISC_VALUE_DEC	dvdSalesCash;
	DISC_VALUE_DEC	dvdSGAndASales;
	DISC_VALUE_DEC	dvdReceivablesTurnover;
	DISC_VALUE_DEC	dvdReceivablesDaysSales;
	DISC_VALUE_DEC	dvdInventoriesTurnover;
	DISC_VALUE_DEC	dvdInventoriesDaysSales;
	DISC_VALUE_DEC	dvdNetSalesWorkingCapital;
	DISC_VALUE_DEC	dvdNetSalesNetPlantEquipment;
	DISC_VALUE_DEC	dvdNetSalesCurrentAssets;
	DISC_VALUE_DEC	dvdNetSalesTotalAssets;
	DISC_VALUE_DEC	dvdNetSalesEmployees;
	DISC_VALUE_DEC	dvdTotalLiabilitiesTotalAssets;
	DISC_VALUE_DEC	dvdTotalLiabilitiesInvestedCapital;
	DISC_VALUE_DEC	dvdTotalLiabilitiesCommonEquity;
	DISC_VALUE_DEC	dvdTimesInterestEarned;
	DISC_VALUE_DEC	dvdCurrentDebtEquity;
	DISC_VALUE_DEC	dvdLongTermDebtEquity;
	DISC_VALUE_DEC	dvdTotalDebtEquity;
	DISC_VALUE_DEC	dvdTotalAssetsEquity;
	DISC_VALUE_DEC	dvdPretaxIncomeNetSales;
	DISC_VALUE_DEC	dvdPretaxIncomeTotalAssets;
	DISC_VALUE_DEC	dvdPretaxIncomeInvestedCapital;
	DISC_VALUE_DEC	dvdPretaxIncomeCommonEquity;
	DISC_VALUE_DEC	dvdNetIncomeNetSales;
	DISC_VALUE_DEC	dvdNetIncomeTotalAssets;
	DISC_VALUE_DEC	dvdNetIncomeInvestedCapital;
	DISC_VALUE_DEC	dvdNetIncomeCommonEquity;
	DISC_VALUE_DEC	dvdRDExpendituresNetSales;
	DISC_VALUE_DEC	dvdRDExpendituresNetIncome;
	DISC_VALUE_DEC	dvdRDExpendituresEmployees;
} DR_RATIO_ANALYSIS, *LPDR_RATIO_ANALYSIS;

// disclosure record ratio analysis
typedef struct tagDR_RATIO_ANALYSIS_SEG_DATA
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sSegmentData[1];			// descriptive name for whats actually in here
	};
} DR_RATIO_ANALYSIS_SEG_DATA, *LPDR_RATIO_ANALYSIS_SEG_DATA;

// ===========================================================================
// disclosure record 5 year summary
typedef struct tagDR_SUMMARY_DATA
{
	DWORD			dwYear;
	DISC_VALUE_EXP  dveSales;
	DISC_VALUE_EXP  dveNetIncome;
	DISC_VALUE_DEC	dvdEPS;
} DR_SUMMARY_DATA, *LPDR_SUMMARY_DATA;

// ===========================================================================
// disclosure record 5 year summary
typedef struct tagDR_5YEAR_SUMMARY
{
	DR_SUMMARY_DATA	firstYear;
	DR_SUMMARY_DATA	secondYear;
	DR_SUMMARY_DATA	thirdYear;
	DR_SUMMARY_DATA	forthYear;
	DR_SUMMARY_DATA	fifthYear;
	DISC_VALUE_DEC	dvd5YearSalesGrowthRate;
	DISC_VALUE_DEC	dvd5YearNetIncomeGrowthRate;
	DISC_VALUE_DEC	dvd5YearEPSGrowthRate;
} DR_5YEAR_SUMMARY, *LPDR_5YEAR_SUMMARY;

// ===========================================================================
// disclosure record financial data comments
typedef struct tagDR_FIN_DATA_COMMENTS
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sFinDataComments[1];		// descriptive name for whats actually in here
	};
} DR_FIN_DATA_COMMENTS, *LPDR_FIN_DATA_COMMENTS;

// ===========================================================================
// disclosure record muller pricing
typedef struct tagDR_MULLER_PRICING
{
	DWORD			dwOutstandingShares;
	DWORD			dwVolume;
	DISC_VALUE_DEC	dvdHigh;
	DISC_VALUE_DEC	dvdLow;
	DISC_VALUE_DEC	dvdClose;
	DISC_VALUE_DEC	dvdPriceEarningsRatio;
	DISC_VALUE_DEC	dvdEarningsPerShare;
	DISC_VALUE_DEC	dvdIndicatedAnnualDividend;
	DISC_VALUE_DEC	dvdFirstDividend;
	DISC_VALUE_DEC	dvdSecondDividend;
	DISC_DATE		lastTradeDate;
	DISC_DATE		pricingInfoForWeekEnding;
	DISC_DATE		epsDateforYearEnding;
	DISC_DATE		firstExDividendDate;
	DISC_DATE		firstRecordDate;
	DISC_DATE		firstPaymentDate;
	DISC_DATE		secondExDividendDate;
	DISC_DATE		secondRecordDate;
	DISC_DATE		secondPaymentDate;
	CHAR			cFirstPaymentMethod;
	CHAR			cFirstReserved[3];
	CHAR			cSecondPaymentMethod;
	CHAR			cSecondReserved[3];
} DR_MULLER_PRICING, *LPDR_MULLER_PRICING;

// ===========================================================================
// disclosure record muller officers
typedef struct tagDR_MULLER_OFFICERS
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sOfficers[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_OFFICERS, *LPDR_MULLER_OFFICERS;

// ===========================================================================
// disclosure record muller directors
typedef struct tagDR_MULLER_DIRECTORS
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sDirectors[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_DIRECTORS, *LPDR_MULLER_DIRECTORS;

// ===========================================================================
// disclosure record muller ownership
typedef struct tagDR_MULLER_OWNERSHIP
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sOwnership[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_OWNERSHIP, *LPDR_MULLER_OWNERSHIP;

// ===========================================================================
// disclosure record muller subsidiaries
typedef struct tagDR_MULLER_SUBSIDIARIES
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sSubsidiaries[1];			// descriptive name for whats actually in here
	};
} DR_MULLER_SUBSIDIARIES, *LPDR_MULLER_SUBSIDIARIES;

// ===========================================================================
// disclosure record muller exhibits
typedef struct tagDR_MULLER_EXHIBITS
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sExhibits[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_EXHIBITS, *LPDR_MULLER_EXHIBITS;

// ===========================================================================
// disclosure record muller other corporate events
typedef struct tagDR_MULLER_OTHER_EVENTS
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sOtherEvents[1];			// descriptive name for whats actually in here
	};
} DR_MULLER_OTHER_EVENTS, *LPDR_MULLER_OTHER_EVENTS;

// ===========================================================================
// disclosure record muller presidents letter
typedef struct tagDR_MULLER_PRES_LETTER
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sPresidentsLetter[1];		// descriptive name for whats actually in here
	};
} DR_MULLER_PRES_LETTER, *LPDR_MULLER_PRES_LETTER;

// ===========================================================================
// disclosure record muller management discussion
typedef struct tagDR_MULLER_MGMT_DISCUSSION
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sMgmtDiscussion[1];			// descriptive name for whats actually in here
	};
} DR_MULLER_MGMT_DISCUSSION, *LPDR_MULLER_MGMT_DISCUSSION;

// ===========================================================================
// disclosure record muller financial footnotes
typedef struct tagDR_MULLER_FOOTNOTES
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sFootnotes[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_FOOTNOTES, *LPDR_MULLER_FOOTNOTES;

// ===========================================================================
// disclosure record muller assets table full text
typedef struct tagDR_MULLER_ASSETS_TABLE
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sAssetsTable[1];			// descriptive name for whats actually in here
	};
} DR_MULLER_ASSETS_TABLE, *LPDR_MULLER_ASSETS_TABLE;

// ===========================================================================
// disclosure record muller liabilities table full text
typedef struct tagDR_MULLER_LIAB_TABLE
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sLiabTable[1];				// descriptive name for whats actually in here
	};
} DR_MULLER_LIAB_TABLE, *LPDR_MULLER_LIAB_TABLE;

// ===========================================================================
// disclosure record muller income statement full text
typedef struct tagDR_MULLER_INCOME_STMT_TABLE
{
	DWORD	dwTotalNumberOfLines;		// total number of lines in the variable length data
	union
	{
	CHAR	sVarLenField[1];			// generic name for ease of use (e.g. offsetof)
	CHAR	sIncomeStatementTable[1];	// descriptive name for whats actually in here
	};
} DR_MULLER_INCOME_STMT_TABLE, *LPDR_MULLER_INCOME_STMT_TABLE;

// ===========================================================================
// disclosure record compressoin scale for huffman compression
typedef struct tagDR_COMPRESSION_SCALE
{
	CHAR	sCompressionScale[256];		// huffman compression scale
} DR_COMPRESSION_SCALE, *LPDR_COMPRESSION_SCALE;

// ===========================================================================
// structure for financial info header 
typedef struct tagDR_FINANCIAL_INFO_HEADER
{
	DISC_DATE	financialInformationDate;
	CHAR		sReserved[2];				// for packing
} DR_FINANCIAL_INFO_HEADER, *LPDR_FINANCIAL_INFO_HEADER;

// ===========================================================================
// union of disclosure financial info statements
typedef struct tagDR_FINANCIAL_INFO_STMTS
{
	DR_FINANCIAL_INFO_HEADER header;
	union
	{
		DR_BS_ASSETS				drBSAssets;
		DR_BS_LIABILITIES			drBSLiabilities;
		DR_INCOME_STATEMENT			drIncomeStatement;
		DR_CASHFLOW_STATEMENT		drCashFlowStatement;
		DR_RATIO_ANALYSIS			drRatioAnalysis;
		DR_RATIO_ANALYSIS_SEG_DATA	drRatioAnalysisSegData;
	};
} DR_FINANCIAL_INFO_STMTS, *LPDR_FINANCIAL_INFO_STMTS;

// ===========================================================================
// disclosure record
typedef struct tagDISCLOSURE_RECORD
{
	DISCLOSURE_HEADER disclosureHeader;
	union
	{
		UCHAR						data;				// general purpose member to access this in a generic way
		DR_RESUME					drResume;
		DR_BUSINESS_DESC			drBusinessDesc;
		DR_FILINGS_TABLE			drFilingsTable;
		DR_AUDITORS_RPT				drAuditorsRpt;
		DR_FINANCIAL_INFO_STMTS		drFinancialInfoStmts;	// union of financial statements that are tied together by a date
		DR_5YEAR_SUMMARY			dr5YearSummary;
		DR_FIN_DATA_COMMENTS		drFinDataComments;
		DR_MULLER_PRICING			drMullerPricing;
		DR_MULLER_OFFICERS			drMullerOfficers;
		DR_MULLER_DIRECTORS			drMullerDirectors;
		DR_MULLER_OWNERSHIP			drMullerOwnership;
		DR_MULLER_SUBSIDIARIES		drMullerSubsidiaries;
		DR_MULLER_EXHIBITS			drMullerExhibits;
		DR_MULLER_OTHER_EVENTS		drMullerOtherEvents;
		DR_MULLER_PRES_LETTER		drMullerPresidentsLetter;
		DR_MULLER_MGMT_DISCUSSION	drMullerMgmtDiscussion;
		DR_MULLER_FOOTNOTES			drMullerFootnotes;
		DR_MULLER_ASSETS_TABLE		drMullerAssetsTable;
		DR_MULLER_LIAB_TABLE		drMullerLiabTable;
		DR_MULLER_INCOME_STMT_TABLE	drMullerIncomeStatement;
		DR_COMPRESSION_SCALE		drCompressionScale;
	};
}DISCLOSURE_RECORD, *LPDISCLOSURE_RECORD;

// ===========================================================================
// ======== defines for reading of the discloure magnetic tape below =========
// ===========================================================================
// defines for reading in tape
#define DISCLOSURE_LINE_SIZE	83			// tape line size
#define MAX_FL_FIELD		25				// tape max fl field size
#define MAX_TEXT_FIELD		83				// tape max text field
#define MAX_READ_LINES		4096			// max lines to read at a time from tape

// ===========================================================================
// disclosure field tags read from tape
// resume
#define DFT_DISCLOSURE_COMPANY_NUMBER			MAKEWORD( 'P', 'G' )
#define DFT_COMPANY_NAME						MAKEWORD( 'C', 'O' )
#define DFT_CROSS_REFERENCE						MAKEWORD( 'X', 'R' )
#define DFT_COMPANY_STATUS						MAKEWORD( 'C', 'S' )
#define DFT_STREET_ADDRESS1						MAKEWORD( 'A', '1' )
#define DFT_STREET_ADDRESS2						MAKEWORD( 'A', '2' )
#define DFT_STREET_ADDRESS3						MAKEWORD( 'A', '3' )
#define DFT_CITY								MAKEWORD( 'C', '1' )
#define DFT_STATE								MAKEWORD( 'C', '2' )
#define DFT_ZIP_CODE							MAKEWORD( 'C', '3' )
#define DFT_TELEPHONE							MAKEWORD( 'T', 'E' )
#define DFT_INCORPORATION						MAKEWORD( 'I', 'E' )
#define DFT_EXCHANGE							MAKEWORD( 'E', 'Z' )
#define DFT_TICKER_SYMBOL						MAKEWORD( 'T', 'K' )
#define DFT_FORTUNE_NUMBER						MAKEWORD( 'F', 'D' )
#define DFT_FORBES_NUMBERS						MAKEWORD( 'F', 'B' )
#define DFT_CUSIP_NUMBER						MAKEWORD( 'C', 'U' )
#define DFT_DUNS_NUMBER							MAKEWORD( 'D', 'N' )
#define DFT_SIC_CODES							MAKEWORD( 'S', 'I' )
#define DFT_PRIMARY_SIC_CODE					MAKEWORD( 'S', 'P' )
#define DFT_BUSINESS_DESCRIPTION				MAKEWORD( 'D', 'P' )
#define DFT_CURRENT_SHARES_OUTSTANDING			MAKEWORD( 'O', 'S' )
#define DFT_SHARES_HELD_BY_OFFS_AND_DIRS		MAKEWORD( 'H', 'S' )
#define DFT_SHAREHOLDERS						MAKEWORD( 'S', 'H' )
#define DFT_EMPLOYEES							MAKEWORD( 'E', 'M' )
#define DFT_FISCAL_YEAR_END						MAKEWORD( 'F', 'Y' )
#define DFT_FILINGS_TABLE						MAKEWORD( 'F', 'L' )
#define DFT_LATEST_ANNUAL_DATA					MAKEWORD( 'L', 'F' )
#define DFT_LATEST_QUARTERLY_DATA				MAKEWORD( 'L', 'Q' )
#define DFT_LEGAL_COUNSEL						MAKEWORD( 'L', 'G' )
#define DFT_STOCK_TRANSFER_AGENT				MAKEWORD( 'S', 'T' )
#define DFT_AUDITOR_CHANGE						MAKEWORD( 'A', 'C' )
#define DFT_AUDITOR								MAKEWORD( 'A', 'U' )
#define DFT_AUDITORS_REPORT						MAKEWORD( 'A', 'R' )
// financial data
// balance sheet - assets
#define DFT_FINANCIAL_HEADER_DATE				MAKEWORD( 'C', 'F' )
#define DFT_PERIOD_OF_DATA						MAKEWORD( 'Q', 'N' )
#define DFT_BS_ASSETS_HEADER					MAKEWORD( 'B', 'S' )
#define DFT_CASH								MAKEWORD( 'C', 'H' )
#define DFT_MARKETABLE_SECURITIES				MAKEWORD( 'M', 'S' )
#define DFT_RECEIVABLES							MAKEWORD( 'R', 'E' )
#define DFT_INVENTORIES							MAKEWORD( 'I', 'V' )
#define DFT_RAW_MATERIALS						MAKEWORD( 'R', 'M' )
#define DFT_WORK_IN_PROGRESS					MAKEWORD( 'W', 'P' )
#define DFT_FINISHED_GOODS						MAKEWORD( 'F', 'G' )
#define DFT_NOTES_RECEIVABLE					MAKEWORD( 'N', 'O' )
#define DFT_OTHER_CURRENT_ASSETS				MAKEWORD( 'O', 'C' )
#define DFT_CURRENT_ASSETS						MAKEWORD( 'C', 'A' )
#define DFT_PROPERTY_PLANT_EQUIPMENT			MAKEWORD( 'P', 'R' )
#define DFT_ACCUMULATED_DEPRECIATION			MAKEWORD( 'D', 'R' )
#define DFT_NET_PROPERTY_PLANT_EQUIPMENT		MAKEWORD( 'P', 'N' )
#define DFT_INVESTMENT_ADVANCES_TO_SUBS			MAKEWORD( 'V', 'V' )
#define DFT_OTHER_NON_CURRENT_ASSETS			MAKEWORD( 'T', 'H' )
#define DFT_DEFERRED_CHARGES					MAKEWORD( 'C', 'D' )
#define DFT_INTANGIBLES							MAKEWORD( 'I', 'G' )
#define DFT_DEPOSITS_AND_OTHER_ASSETS			MAKEWORD( 'D', 'A' )
#define DFT_TOTAL_ASSETS						MAKEWORD( 'A', 'S' )
// balance sheet - liabilities
#define DFT_BS_LIABILITIES_HEADER				MAKEWORD( 'B', 'L' )
#define DFT_NOTES_PAYABLE						MAKEWORD( 'N', 'T' )
#define DFT_ACCOUNTS_PAYABLE					MAKEWORD( 'A', 'P' )
#define DFT_CURRENT_LONG_TERM_DEBT				MAKEWORD( 'L', '2' )
#define DFT_CURRENT_PORTION_OF_CAP_LEASES		MAKEWORD( 'L', '3' )
#define DFT_ACCRUED_EXPENSES					MAKEWORD( 'L', '4' )
#define DFT_INCOME_TAXES						MAKEWORD( 'T', 'X' )
#define DFT_OTHER_CURRENT_LIABILITIES			MAKEWORD( 'L', 'L' )
#define DFT_TOTAL_CURRENT_LIABILITIES			MAKEWORD( 'T', 'C' )
#define DFT_MORTGAGES							MAKEWORD( 'L', '5' )
#define DFT_DEFERRED_CHARGES_INCOME				MAKEWORD( 'L', '6' )
#define DFT_CONVERTIBLE_DEBT					MAKEWORD( 'L', '7' )
#define DFT_LONG_TERM_DEBT						MAKEWORD( 'L', 'D' )
#define DFT_NON_CURRENT_CAPITAL_LEASES			MAKEWORD( 'L', '8' )
#define DFT_OTHER_LONG_TERM_LIABILITIES			MAKEWORD( 'O', 'L' )
#define DFT_TOTAL_LIABILITIES					MAKEWORD( 'L', '9' )
#define DFT_BS_MINORITY_INTEREST				MAKEWORD( 'M', 'I' )
#define DFT_PREFERRED_STOCK						MAKEWORD( 'L', 'C' )
#define DFT_COMMON_STOCK_NET					MAKEWORD( 'L', 'A' )
#define DFT_CAPITAL_SURPLUS						MAKEWORD( 'L', 'B' )
#define DFT_RETAINED_EARNINGS					MAKEWORD( 'O', 'E' )
#define DFT_TREASURY_STOCK						MAKEWORD( 'T', 'S' )
#define DFT_OTHER_LIABILITIES					MAKEWORD( 'L', 'O' )
#define DFT_SHAREHOLDERS_EQUITY					MAKEWORD( 'N', 'W' )
#define DFT_TOTAL_LIABILITIES_NET_WORTH			MAKEWORD( 'L', 'E' )
// income statement
#define DFT_INCOME_STATEMENT_HEADER				MAKEWORD( 'I', 'S' )
#define DFT_NET_SALES							MAKEWORD( 'S', 'A' )
#define DFT_COST_OF_GOODS						MAKEWORD( 'C', 'G' )
#define DFT_GROSS_PROFIT						MAKEWORD( 'G', 'P' )
#define DFT_RD_EXPENDITURES						MAKEWORD( 'I', '1' )
#define DFT_SELL_GEN_ADMIN_EXPENSES				MAKEWORD( 'S', 'G' )
#define DFT_INCOME_BEFORE_DEPREC_AMORT			MAKEWORD( 'I', '2' )
#define DFT_IS_DEPRECIATION_AMORTIZATION		MAKEWORD( 'I', '3' )
#define DFT_NON_OPERATING_INCOME				MAKEWORD( 'I', '4' )
#define DFT_INTEREST_EXPENSE					MAKEWORD( 'I', '5' )
#define DFT_INCOME_BEFORE_TAX					MAKEWORD( 'P', 'I' )
#define DFT_PROVISION_FOR_INCOME_TAXES			MAKEWORD( 'T', 'T' )
#define DFT_IS_MINORITY_INTEREST				MAKEWORD( 'I', '6' )
#define DFT_INVESTMENT_GAINS_LOSSES				MAKEWORD( 'G', 'L' )
#define DFT_OTHER_INCOME						MAKEWORD( 'I', 'O' )
#define DFT_NET_INCOME_BEFORE_EXTRA_ITEMS		MAKEWORD( 'I', '7' )
#define DFT_EXTRA_ITEMS_AND_DISCONTINUED_OPS	MAKEWORD( 'E', 'I' )
#define DFT_IS_NET_INCOME						MAKEWORD( 'I', 'N' )
#define DFT_OUTSTANDING_SHARES					MAKEWORD( 'D', 'S' )
// cash flow analysis
#define DFT_CASH_FLOW_STATEMENT_HEADER			MAKEWORD( 'F', '0' )
#define DFT_CF_NET_INCOME						MAKEWORD( 'F', '1' )
#define DFT_CF_DEPRECIATION_AMORTIZATION		MAKEWORD( 'F', '2' )
#define DFT_NET_INC_IN_ASSETS_LIABS				MAKEWORD( 'F', '3' )
#define DFT_CASH_PROVIDED_BY_OPS				MAKEWORD( 'F', '4' )
#define DFT_OTHER_ADJUSTMENTS_NET				MAKEWORD( 'F', '5' )
#define DFT_NET_CASH_PROVIDED_BY_OPS			MAKEWORD( 'F', '6' )
#define DFT_DEC_PROPERTY_AND_PLANT				MAKEWORD( 'F', '7' )
#define DFT_ACQ_DISP_SUBSID_OR_OTH_BUSINESS		MAKEWORD( 'F', '8' )
#define DFT_INC_IN_INVESTMENTS					MAKEWORD( 'F', '9' )
#define DFT_OTHER_CASH_IN_FLOW					MAKEWORD( 'F', 'A' )
#define DFT_NET_CASH_PROV_BY_INVESTMENT			MAKEWORD( 'F', 'C' )
#define DFT_ISSUANCES_EQUITY_SECURITY			MAKEWORD( 'F', 'E' )
#define DFT_CF_BLANK_FIELD						MAKEWORD( 'F', 'F' )
#define DFT_ISSUANCES_DEBT_SECURITY				MAKEWORD( 'F', 'H' )
#define DFT_INC_BANK_OTHER_BORROWINGS			MAKEWORD( 'F', 'I' )
#define DFT_DIVIDENDS_OTHER_DISTRIBUTIONS		MAKEWORD( 'F', 'J' )
#define DFT_OTHER_CASH_IN_FLOW2					MAKEWORD( 'F', 'K' )
#define DFT_NET_CASH_PROV_BY_FINANCING			MAKEWORD( 'F', 'M' )
#define DFT_EFFECT_OF_EXCH_RATES_ON_CASH		MAKEWORD( 'F', 'N' )
#define DFT_NET_CHANGES_CASH_N_CASH_EQUIV		MAKEWORD( 'F', 'Q' )
#define DFT_CASH_N_CASH_EQUIV_START_OF_YEAR		MAKEWORD( 'F', 'R' )
#define DFT_CASH_N_CASH_EQUIV_END_OF_YEAR		MAKEWORD( 'F', 'V' )
// ratio analysis
#define DFT_RATIO_ANALYSIS_HEADER				MAKEWORD( 'R', 'D' )
#define DFT_QUICK_RATIO							MAKEWORD( 'R', 'Q' )
#define DFT_CURRENT_RATIO						MAKEWORD( 'R', 'C' )
#define DFT_SALES_CASH							MAKEWORD( 'R', 'S' )
#define DFT_SG_AND_A_SALES						MAKEWORD( 'R', 'G' )
#define DFT_RECEIVABLES_TURNOVER				MAKEWORD( 'R', 'I' )
#define DFT_RECEIVABLES_DAYS_SALES				MAKEWORD( 'R', 'A' )
#define DFT_INVENTORIES_TURNOVER				MAKEWORD( 'R', 'B' )
#define DFT_INVENTORIES_DAYS_SALES				MAKEWORD( 'R', 'F' )
#define DFT_NET_SALES_WORKING_CAPITAL			MAKEWORD( 'R', 'H' )
#define DFT_NET_SALES_NET_PLANT_EQUIPMENT		MAKEWORD( 'R', 'R' )
#define DFT_NET_SALES_CURRENT_ASSETS			MAKEWORD( 'R', 'J' )
#define DFT_NET_SALES_TOTAL_ASSETS				MAKEWORD( 'R', 'K' )
#define DFT_NET_SALES_EMPLOYEES					MAKEWORD( 'R', 'L' )
#define DFT_TOTAL_LIABILITIES_TOTAL_ASSETS		MAKEWORD( 'R', 'N' )
#define DFT_TOTAL_LIABILITIES_INVESTED_CAPITAL	MAKEWORD( 'R', 'O' )
#define DFT_TOTAL_LIABILITIES_COMMONEQUITY		MAKEWORD( 'R', 'P' )
#define DFT_TIMES_INTEREST_EARNED				MAKEWORD( 'R', 'U' )
#define DFT_CURRENT_DEBT_EQUITY					MAKEWORD( 'R', 'V' )
#define DFT_LONG_TERM_DEBT_EQUITY				MAKEWORD( 'R', 'W' )
#define DFT_TOTAL_DEBT_EQUITY					MAKEWORD( 'R', 'X' )
#define DFT_TOTAL_ASSETS_EQUITY					MAKEWORD( 'R', 'Y' )
#define DFT_PRETAX_INCOME_NET_SALES				MAKEWORD( 'R', '1' )
#define DFT_PRETAX_INCOME_TOTAL_ASSETS			MAKEWORD( 'R', '2' )
#define DFT_PRETAX_INCOME_INVESTED_CAPITAL		MAKEWORD( 'R', '3' )
#define DFT_PRETAX_INCOME_COMMON_EQUITY			MAKEWORD( 'R', '4' )
#define DFT_NET_INCOME_NET_SALES				MAKEWORD( 'R', '5' )
#define DFT_NET_INCOME_TOTAL_ASSETS				MAKEWORD( 'R', '6' )
#define DFT_NET_INCOME_INVESTED_CAPITAL			MAKEWORD( 'R', '7' )
#define DFT_NET_INCOME_COMMON_EQUITY			MAKEWORD( 'R', '8' )
#define DFT_RD_EXPENDITURES_NET_SALES			MAKEWORD( 'R', '9' )
#define DFT_RD_EXPENDITURES_NET_INCOME			MAKEWORD( 'N', 'R' )
#define DFT_RD_EXPENDITURES_EMPLOYEES			MAKEWORD( 'E', 'R' )
#define DFT_SEGMENT_DATA						MAKEWORD( 'S', 'L' )
// 5 year summary
#define DFT_FIVE_YEAR_SUMMARY_HEADER			MAKEWORD( '5', 'D' )
#define DFT_FIRST_YEAR_SUMMARY					MAKEWORD( '5', '1' )
#define DFT_SECOND_YEAR_SUMMARY					MAKEWORD( '5', '2' )
#define DFT_THIRD_YEAR_SUMMARY					MAKEWORD( '5', '3' )
#define DFT_FORTH_YEAR_SUMMARY					MAKEWORD( '5', '4' )
#define DFT_FIFTH_YEAR_SUMMARY					MAKEWORD( '5', '5' )
#define DFT_FIVE_YEAR_GROWTH_RATE				MAKEWORD( '5', '6' )
#define DFT_COMMENTS							MAKEWORD( 'C', 'M' )
// Muller pricing/earnins/dividend data
#define DFT_MULLER_PRICING_DATA_HEADER			MAKEWORD( 'P', 'D' )
#define DFT_MULLER_OUTSTANDING_SHARES			MAKEWORD( 'M', '4' )
#define DFT_MULLER_LAST_TRADE_DATE				MAKEWORD( 'M', '5' )
#define DFT_MULLER_PRICING_INFO_FOR_WEEK_ENDING	MAKEWORD( 'M', 'T' )
#define DFT_MULLER_VOLUME						MAKEWORD( 'M', '6' )
#define DFT_MULLER_HIGH							MAKEWORD( 'M', '7' )
#define DFT_MULLER_LOW							MAKEWORD( 'M', '8' )
#define DFT_MULLER_CLOSE						MAKEWORD( 'M', '1' )
#define DFT_MULLER_PRICE_EARNINGS_RATIO			MAKEWORD( 'M', 'W' )
#define DFT_MULLER_EARNINGS_INFO_HEADER			MAKEWORD( 'M', 'A' )
#define DFT_MULLER_EPS_DATE_FOR_YEAR_ENDING		MAKEWORD( 'M', '9' )
#define DFT_MULLER_EARNINGS_PER_SHARE			MAKEWORD( 'M', '2' )
#define DFT_MULLER_DIVIDEND_INFO_HEADER			MAKEWORD( 'M', 'R' )
#define DFT_MULLER_INDICATED_ANNUAL_DIVIDEND	MAKEWORD( 'M', '3' )
#define DFT_MULLER_FIRST_DIVIDEND				MAKEWORD( 'M', 'B' )
#define DFT_MULLER_FIRST_EXDIVIDEND_DATE		MAKEWORD( 'M', 'C' )
#define DFT_MULLER_FIRST_RECORD_DATE			MAKEWORD( 'M', 'D' )
#define DFT_MULLER_FIRST_PAYMENT_DATE			MAKEWORD( 'M', 'E' )
#define DFT_MULLER_FIRST_PAYMENT_METHOD			MAKEWORD( 'M', 'Q' )
#define DFT_MULLER_SECOND_DIVIDEND				MAKEWORD( 'M', 'K' )
#define DFT_MULLER_SECOND_EXDIVIDEND_DATE		MAKEWORD( 'M', 'M' )
#define DFT_MULLER_SECOND_RECORD_DATE			MAKEWORD( 'M', 'N' )
#define DFT_MULLER_SECOND_PAYMENT_DATE			MAKEWORD( 'M', 'P' )
#define DFT_MULLER_SECOND_PAYMENT_METHOD		MAKEWORD( 'M', 'J' )
// Muller textual information
#define DFT_MULLER_OFFICERS						MAKEWORD( 'O', 'F' )
#define DFT_MULLER_DIRECTORS					MAKEWORD( 'D', 'I' )
#define DFT_MULLER_OWNERSHIP					MAKEWORD( 'O', 'W' )
#define DFT_MULLER_SUBSIDIARIES					MAKEWORD( 'S', 'U' )
#define DFT_MULLER_EXHIBITS						MAKEWORD( 'E', 'X' )
#define DFT_MULLER_OTHEREVENTS					MAKEWORD( 'O', 'T' )
#define DFT_MULLER_PRESIDENTS_LETTER			MAKEWORD( 'P', 'L' )
#define DFT_MULLER_MGMT_DISCUSSION				MAKEWORD( 'M', 'G' )
#define DFT_MULLER_FOOTNOTES					MAKEWORD( 'F', 'O' )
#define DFT_MULLER_ASSETS_TABLE					MAKEWORD( 'A', 'T' )
#define DFT_MULLER_LIAB_TABLE					MAKEWORD( 'L', 'T' )
#define DFT_MULLER_INCOME_STMT_TABLE			MAKEWORD( 'I', 'F' )
// misc fields
#define DFT_REPEAT_FIELD1						MAKEWORD( ' ', ' ' )
#define DFT_REPEAT_FIELD2						MAKEWORD( 0x0D, 0x0A )

// Define special flags.

#define DFF_TITLELINE				0x0001
#define DFF_IN_MILLIONS				0x0002

// Define special fields.

#define DFT_PAGETYPE				1
#define DFT_COMPOSITE_ADDRESS		2

#define DFT_YEAR					10
#define DFT_SALES					11
#define DFT_NET_INCOME				12
#define DFT_EPS						13

// Define five year summary break out fields.

#define DFT_FIRST_YEAR				100
#define DFT_FIRST_YEAR_SALES		101
#define DFT_FIRST_YEAR_NETINCOME	102
#define DFT_FIRST_YEAR_EPS			103

#define DFT_SECOND_YEAR				105
#define DFT_SECOND_YEAR_SALES		106
#define DFT_SECOND_YEAR_NETINCOME	107
#define DFT_SECOND_YEAR_EPS			108

#define DFT_THIRD_YEAR				110
#define DFT_THIRD_YEAR_SALES		111
#define DFT_THIRD_YEAR_NETINCOME	112
#define DFT_THIRD_YEAR_EPS			113

#define DFT_FOURTH_YEAR				115
#define DFT_FOURTH_YEAR_SALES		116
#define DFT_FOURTH_YEAR_NETINCOME	117
#define DFT_FOURTH_YEAR_EPS			118

#define DFT_FIFTH_YEAR				120
#define DFT_FIFTH_YEAR_SALES		121
#define DFT_FIFTH_YEAR_NETINCOME	122
#define DFT_FIFTH_YEAR_EPS			123

#define DFT_FIVE_YEAR_SALES			125
#define DFT_FIVE_YEAR_NETINCOME		126
#define DFT_FIVE_YEAR_EPS			127

#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif


