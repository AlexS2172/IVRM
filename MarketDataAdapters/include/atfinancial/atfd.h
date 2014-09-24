
// include the new corporate action records for NT servers.

#include <corpactn.h>

#if !defined(_ATFD_H_)
#define _ATFD_H_

#ifndef RC_INVOKED
#pragma pack(1)						 /* Word aligned. */
#endif /* RC_INVOKED */

/*------------ Packed Time and Date formats ---------------*/

typedef	struct
		{
		WORD		day   : 5,		// Lower  5 bits = day 
					month : 4,		// Middle 4 bits = month
					year  : 7;		// Upper  7 bits = year (add 1970)
		}
		S_DATE;						// Used by Stocks/Stock options

typedef	struct
		{
		WORD		day   : 5,		// Lower  5 bits = day 
					month : 4,		// Middle 4 bits = month
					year  : 7;		// Upper  7 bits = year (add 1970)
		}
		H_DATE;						// Used by History Files

typedef	struct
		{
		WORD		year  : 6,		// Lower  6 bits = year (add 1970)
					day   : 5,		// Middle 5 bits = day
					month : 4,		// Upper  4 bits = month
					flag  : 1;		// MSB    1 bit  = Date (not time)
		}
		F_DATE;						// Used by futures

typedef	struct
		{
		WORD		seconds:4,		// seconds/4
				 	minute :6,
					hour   :5,
					flag   :1;		// 0 = time
		}
		F_TIME;						// Used by futures

typedef	union
		{
		WORD		value;
		S_DATE		sDate;
		H_DATE		hDate;
		F_DATE		fDate;
		F_TIME		fTime;
		}
		PKDATE;

// "Known" filename values.

#define	BFN_TK_STOCK_US				1
#define	BFN_TK_STKOPT_US			2
#define	BFN_TK_FUTURE_US			3
#define	BFN_TK_ATOPTN_US			4

#define	BFN_TK_STOCK_EU				5
#define	BFN_TK_STKOPT_EU			6
#define	BFN_TK_FUTURE_EU			7
#define	BFN_TK_EXTRA_EU				8

#define	BFN_TK_STOCK_AS				9
#define	BFN_TK_STKOPT_AS			10
#define	BFN_TK_FUTURE_AS			11
#define	BFN_TK_EXTRA_AS				12

#define	BFN_TK_STOCK_AM				13
#define	BFN_TK_STKOPT_AM			14
#define	BFN_TK_FUTURE_AM			15
#define	BFN_TK_EXTRA_AM				16

#define	BFN_TK_STOCK_CA				17
#define	BFN_TK_FUND					18

#define	BFN_AT_OPTION				19

#define	BFN_FDR_STOCK				20
#define	BFN_FDR_FUTURE				21
#define	BFN_FDR_EUROPE				22

#define	BFN_AT_INDEX				25
#define	BFN_AT_FUTOPTION			26

#define	BFN_CORP_ACTION				32
#define	BFN_SP_EARNINGS				33
#define	BFN_SP_SPMUSA				34
#define	BFN_SP_SPMEUR				35
#define	BFN_SP_MMS					36

#define	BFN_CA_CHANGE				37
#define	BFN_CA_IPO					38
#define	BFN_CA_DIV					39
#define	BFN_CA_SPLIT				40

#define	BFN_BR_FUNDDATA				41
#define	BFN_FC_SNAPSHOTDATA			42
#define	BFN_FC_BROKER				43

#define BFN_PQ_TOPTEN				44
#define BFN_PQ_FUNDAMENTAL			45
#define BFN_PQ_SECPROFILE			46
#define BFN_PQ_OPTION				47
#define BFN_PQ_FUTURE				48
#define BFN_PQ_SNPCUSIP				49

#define	BFN_TCA_CHANGE				50
#define	BFN_TCA_IPO					51
#define	BFN_TCA_DIV					52
#define	BFN_TCA_SPLIT				53

#define	BFN_CA_DIVIDENDS			54
#define	BFN_CA_STATUS				55
#define	BFN_CA_CHANGES				56
#define	BFN_CA_SPLITS				57
#define	BFN_NEWS_TYPE				58

#define BFN_ENTITLE					62
#define BFN_EVENT					63

// Key path values

#define	BKP_TK_STOCK_US_SYMBOL		0
#define	BKP_TK_STOCK_US_CUSIP		1
#define	BKP_TK_STOCK_US_NAME		2
#define	BKP_TK_STOCK_US_SIC			3

#define	BKP_TK_STKOPT_US_SYMBOL		0
#define	BKP_TK_STKOPT_US_ROOTSYM	1

#define	BKP_TK_FUTURE_US_SYMBOL		0
#define	BKP_TK_FUTURE_US_NAME		1

#define	BKP_BR_SYMBOL				0
#define	BKP_BR_CUSIP				1
#define	BKP_BR_NAME					2
#define	BKP_BR_BINARY_SYM			3

#define	BKP_FC_SYMBOL				0
#define	BKP_FC_NAME					1

	// Record types returned in for fundamental data type

#define	FDT_TK_STOCK	  			0
#define	FDT_TK_OPTION 				1
#define	FDT_TK_FUTURE 				2
#define	FDT_CS_STOCK  				3
#define	FDT_CS_FUTURE 				4
#define	FDT_CS_EUROPE 				5
#define	FDT_PQ_DB	  				6
#define	FDT_PQ_1500	  				7
#define	FDT_PQ_SECPROF				8

#define	FDT_AT_OPTNUS				10

#define FDT_ATFI_EQUITY_FP			99
#define FDT_ATFI_EQUITY				100
#define FDT_ATFI_DIVIDEND			101
#define FDT_ATFI_SPLIT				102
#define FDT_ATFI_CALCULATED			103
#define FDT_ATFI_CALCULATED_HIST	104
#define FDT_ATFI_EARNINGS			105
#define FDT_ATFI_INCOME				106
#define FDT_ATFI_CHANGE				107
#define FDT_ATFI_FUTURE				108
#define FDT_ATFI_BOND				109
#define FDT_ATFI_INDEX				110
#define FDT_ATFI_OPTION				111
#define FDT_ATFI_MUTUALFUND			112
#define FDT_ATFI_MMFUND				113
#define FDT_ATFI_FUTUREOPTION		114
#define FDT_ATFI_FOREX				115
#define FDT_ATFI_BALANCE			116
#define FDT_ATFI_FIXEDINCOME		117
#define FDT_ATFI_DESCRIPTION		118
#define FDT_ATFI_POSTPANEL			119

#define	FDT_ATFI_OPTIONS			120

#define FDT_ATFI_ADDRESS			130
#define FDT_ATFI_EQUITY2			131
#define FDT_ATFI_DIVIDEND2			132
#define FDT_ATFI_EARNINGS2			133
#define FDT_ATFI_SEGMENTS			134
#define FDT_ATFI_FINANCIALS			135

#define	FDT_TK_US_STOCK				8001
#define	FDT_TK_US_STKOPT			8002
#define	FDT_TK_US_FUTURE			8003

#define	FDT_TK_EU_STOCK				8005

#define	FDT_TK_AM_STOCK				8009

#define	FDT_TK_AS_STOCK				8013

#define	FDT_TK_CA_STOCK				8017

#define	FDT_SP_US_STOCK				8020
#define	FDT_SP_US_FUTURE			8021

#define	FDT_PQ_DBA_51				8030
#define	FDT_PQ_SECURITYPROFILE		8031
#define	FDT_PQ_FUTURE_56_1			8032

#define	FDT_BR_FUNDDATA				8040

#define	FDT_FC_SNAPSHOTDATA			8050

#define	FDT_WS_MUTUALFUND			8400

/*
**** Fundamental Data Record layouts
****
**** Contains these "types":	AMD_STOCK
****							AMD_STKOPT
****							AMD_FUTURE
****							FDR_STOCK
****							FDR_FUTURE
****							FDR_EUROPE
****							DBA_RECORD
****
****    and composite type:		FUNDDATA
*/

/*---------------- AMD Record Types -------------------*/

#define	AMDRT_STOCK		1
#define	AMDRT_STKOPT	2
#define	AMDRT_FUTURE	3
#define	AMDRT_FUTOPT	4
#define	AMDRT_CASH		5
#define	AMDRT_INDEX		10

/*---------------- Stock AMD Record -------------------*/

typedef	struct
		{
		BYTE	d[1];
		BYTE	AMD;
		BYTE	extra;
		BYTE	SecurityType;			/* This will be 1 (AMD_STOCK) */
		char	CUSIP[8];
		char	CUSIPExt[2];
		char	CUSIPCheck;
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	OutputUnderSymbol[6];
		BYTE	MarketFlags[2];
		BYTE	SecuritySubtype;
		BYTE	SIC[2];
		BYTE	IndustryCode[2];
		BYTE	TradingPriceCurrencyCode;
		BYTE	PriceRepCode;
		BYTE	MinFluctuation;
		WORD	CreationChgDate;
		BYTE	StatusFlags[2];
		BYTE	ChangeCode;
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		WORD	ConversionDate;
		BYTE	ConversionRate[4];
		BYTE	DowJonesIndicator;
		BYTE	SharesPerRndLot[2];
		BYTE	MarginIndicator;
		BYTE	SPRank[4];
		BYTE	ComnShrsOutstanding[4];
		BYTE	PerfShrsOutstanding[4];
		BYTE	InstitutionShares[4];
		BYTE	NumInstHoldingShrs[2];
		BYTE	VolatilityCM12[2];
		BYTE	VolatilityCM11[2];
		BYTE	VolatilityCM10[2];
		BYTE	VolatilityCM9[2];
		BYTE	VolatilityCM8[2];
		BYTE	VolatilityCM7[2];
		BYTE	VolatilityCM6[2];
		BYTE	VolatilityCM5[2];
		BYTE	VolatilityCM4[2];
		BYTE	VolatilityCM3[2];
		BYTE	VolatilityCM2[2];
		BYTE	VolatilityCM1[2];
		BYTE	Volatility6Mon[2];
		BYTE	Volatility20Day[2];
		BYTE	VolatilityImpliedCall[2];
		BYTE	VolatilityImpliedPut[2];
		BYTE	Reserved1[8];
		BYTE	EarningsFrgnExchMark;
		BYTE	InterimPeriod[9];
		BYTE	PriorIntermEPSfoot[3];
		BYTE	PriorIntermEPS[2];
		BYTE	CurrIntermEPSfoot[3];
		BYTE	CurrIntermEPS[2];
		BYTE	FiscalYearMonthEnd[2];
		BYTE	EPS12MonthFoot[3];
		BYTE	EPS12Month[4];
		BYTE	Reserved2[4];
		WORD	DateEPS;
		BYTE	EstEPSFoot[3];
		BYTE	EstEPS[4];
		BYTE	DivFrgnExchMark;
		BYTE	IndicatedAnnualDiv[4];
		BYTE	IndicatedAnnualDivFoot[3];
		WORD	PrevCashXDivDate;
		BYTE	PrevCashXDivAmount[4];
		WORD	CashXDivDate1;
		BYTE	CashXDivAmount1[4];
		WORD	CashXDivDate2;
		WORD	CashXDivDate3;
		WORD	CashXDivDate4;
		WORD	SpecialExDivDate;
		BYTE	SpecialExDivAmount[4];
		BYTE	Reserved3[3];
		BYTE	FrequencyCode;
		BYTE	DividendSourceCode;
		BYTE	Reserved4[14];
		BYTE	SP500FTSE100IndCode;
		BYTE	SP500GroupCode;
		BYTE	Reserved5[8];
		BYTE	LatestYrCollect[2];
		BYTE	LastFiscalYearEPS[4];
		BYTE	LastFsclYrEPSFoot[3];
		BYTE	StockDivRate[4];
		BYTE	StockDivRateFoot[3];
		WORD	StockXDivDate;
		BYTE	StockXDivRateFoot[3];
		WORD	StockDivPayDate;
		BYTE	StockDivPayDateFoot[3];
		WORD	StockDivRecordDate;
		BYTE	StockDivRecordDateFoot[3];
		WORD	StockDivDeclareDate;
		BYTE	StockDivDeclareDateFoot[3];
		WORD	CashDivPayDate;
		BYTE	CashDivPayDateFoot[3];
		WORD	CashDivRecordDate;
		BYTE	CashDivRecordDateFoot[3];
		WORD	CashDivDeclareDate;
		BYTE	CashDivDeclareDateFoot[3];
		BYTE	CommonSharesFootCodes[3];
		BYTE	PreferredSharesFootCodes[3];
		BYTE	FiscalYearMonthEndFoot[3];
		BYTE	DividendYield[2];
		BYTE	DividendYieldFoot[3];
		BYTE	Reserved6[2];
		BYTE	PriceToEarningsRatio[2];
		BYTE	CashXDivAmount2[4];
		BYTE	CashXDivAmount3[4];
		BYTE	CashXDivAmount4[4];
		char	SpinoffOptionRootSymbol[15];
		BYTE	Reserved7[7];
		BYTE	GMCBitmap[16];
		BYTE	High52WeekPrice[4];
		WORD	High52WeekDate;
		BYTE	Low52WeekPrice[4];
		WORD	Low52WeekDate;
		BYTE	StockDividendIndicator;
		BYTE	OldCusipNumber[10];
		BYTE	OldOutputTickerSymbol[16];
		BYTE	OldTickerSymbol[16];
		BYTE	Reserved8[27];				/* Pad to end */
		}
		AMD_STOCK;

/*------------ Stock Option AMD Record -------------------*/

typedef	struct
		{
		char	d[1];							// Signature
		BYTE	AMD;							// Add/Modify/Delete flag
		BYTE	extra1;							// Not used
		BYTE	SecurityType;					// AMD_STKOPT = 2
		char	CUSIP[10];
		char	CUSIPCheck;
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	OutputUnderRootSymbol[6];
		BYTE	MarketFlags[2];
		BYTE	SecuritySubtype;
		BYTE	SIC[2];
		BYTE	IndustryCode[2];
		BYTE	TradingPriceCurrencyCode;
		BYTE	PriceRepCode;
		BYTE	MinFluctuation;
		BYTE	CreationChgDate[2];
		BYTE	StatusFlags[2];
		BYTE	ChangeCode;
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		BYTE	ExpirationDate[2];				// Strike date
		BYTE	ExercisePrice[4];				// Strike price
		BYTE	UnderSecurityGMC;
		BYTE	SharesPerContract[2];
		char	SpinoffOptionRootSymbol[15];
		BYTE	extra2[7];
		BYTE	GMCBitmap[16];
		BYTE	High52WeekPrice[4];
		BYTE	High52WeekDate[2];
		BYTE	Low52WeekPrice[4];
		BYTE	Low52WeekDate[2];
		BYTE	StockDividendIndicator;
		BYTE	OldCusipNumber[10];
		BYTE	OldOutputTickerSymbol[16];
		BYTE	OldTickerSymbol[16];
		BYTE	extra3[14];				/* Pad to end */
		}
		AMD_STKOPT;

/*---------- Future,Future Option,Cash AMD Record -------------*/

typedef	struct
		{
		char	d[1];					// Record type
		BYTE	AMD;					// Add, Modify, Delete record
		BYTE	extra1;
		BYTE	SecurityType;			// This will be 3,4 or 5
		BYTE	extra2[11];
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	IssuerRootSymbol[6];
		char	extra3[2];
		BYTE	SecuritySubtype;
		BYTE	TradingPriceCurrencyCode[3];
		BYTE	FutureSecuritySubtypeCode;
		char	extra4[6];
		BYTE	UnderSecurityGMC;
		WORD	CreationChangeDate;
		char	extra5;
		BYTE	PriceUnitCode;
		BYTE	UnitPricingSize[2];
		BYTE	UnitMeasureCode[2];
		char	extra6[4];
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		BYTE	ExerciseStyle;
		BYTE	OptionTypeIndicator;
		BYTE	ContractSizePRC;
		char	OutputUnderSymbol[16];
		char	extra7[4];
		BYTE	ExercisePRC;
		char	SecurityShortName[20];
		char	extra8[5];
		BYTE	Volatility6Month[2];
		BYTE	Volatility20Day[2];
		BYTE	VolatilityImpCall[2];
		BYTE	VolatilityImpPut[2];
		BYTE	ContractSize[4];
		BYTE	ExercisePrice[4];
		BYTE	ChangeCode;
		char	extra9[39];
		BYTE	PriceRepCode;
		BYTE	extra10;
		BYTE	TolerancePriceFlux[2];
		BYTE	MaxDailyPriceFlux[2];
		BYTE	MinPriceFluxInt[2];
		BYTE	MinPriceFluxCode;
		char	extra11[27];
		BYTE	SessionIdentifier;
		BYTE	TickValue[2];
		BYTE	TickValuePRC;
		char	CUSIP[9];
		char	PrevCINS[10];
		BYTE	ExchangeSymbol[25];
		BYTE	IssuerPart2[25];
		WORD	OpenTimeRegDay;
		WORD	CloseTimeRegDay;
		WORD	CloseTimeLastDay;
		WORD	LastTradeDate;
		WORD	FirstNoticeDate;
		WORD	FirstDeliveryDate;
		WORD	LastDeliveryDate;
		WORD	ExerciseTimeOnLDD;	// LDD = Last Delivery Date
		BYTE	SourceIdentifierCode[2];
		WORD	ExpirationStrikeDate;
		BYTE	extra12[14];
		BYTE	extra13[15];
		BYTE	GMCBitmap[16];
		BYTE	LifeHighPrice[4];
		WORD	LifeHighDate;
		BYTE	LifeLowPrice[4];
		WORD	LifeLowDate;
		BYTE	FutureSecurityType;
		char	PrevNSIN[10];
		char	PrevOutputTickerSymbol[16];
		char	PrevTickerInternalSymbol[16];
		BYTE	extra14[3];
		BYTE	extra15[3];
		char	ISIN[12];
		BYTE	PrevSecType;
		BYTE	SecType;
		BYTE	SecTypeSupplement;
		BYTE	DatabaseType;
		BYTE	GlobalCenter;
		}
		AMD_FUTURE;

/*
**** S&P ComStock Fundamental data record (A-T Format)
**** Contain FD1 and FD2 in a 768 byte record.
*/

typedef	struct
		{
		char	Symbol[6];
		char	PE12Month[4];
		char	AvgDailyVol[5];
		char	HighLow52Wk[15];
		char	HighLowCal[15];
		char	DivYield[5];
		char	LatestDivAmt[9];
		char	IndDivRate[7];
		char	PayDate[8];
		char	ExDivDate[5];
		char	InterimEarnPeriod[11];
		char	LatestInterimEarn[21];
		char	Last12MonthEPS[10];
		char	RefYearEarnings[2];
		char	CurrentYearEPS[10];
		char	NextYearEPSEst[10];
		char	EarningsComment[40];
		char	InterimEarnComment[40];
		char	GrowthPct5Year[6];
		char	FiscalYearEnd[2];
		char	CUSIP_SPInd[9];
		char	CompanyName[30];
		char	SPRankCommon[3];
		char	OptionSymbol[7];
		char	PctHeldByInst[3];
		char	SPStarsRating[1];
		char	Beta[4];
		char	CurrentAssets[9];
		char	CurrentLiabilities[9];
		char	BalanceSheetDate[8];
		char	LongTermDebt[9];
		char	CommonSharesOut[11];
		char	PreferedSharesOut[11];
		char	FinancialComment[40];
		char	RefYearHighLow[2];
		char	Year1HighLow[15];
		char	Year2HighLow[15];
		char	Year3HighLow[15];
		char	Year4HighLow[15];
		char	RefYearDiv[2];
		char	Year1Div[9];
		char	Year2Div[9];
		char	Year3Div[9];
		char	Year4Div[9];
		char	RefYearEPS[2];
		char	Year1EPS[10];
		char	Year2EPS[10];
		char	Year3EPS[10];
		char	Year4EPS[10];
		char	Year1EQPS[7];
		char	Year2EQPS[7];
		char	Year3EQPS[7];
		char	Year4EQPS[7];
		char	Year1Revenue[8];
		char	Year2Revenue[8];
		char	Year3Revenue[8];
		char	Year4Revenue[8];
		char	Year1NetIncome[8];
		char	Year2NetIncome[8];
		char	Year3NetIncome[8];
		char	Year4NetIncome[8];
		char	SplitFactor1_Date[14];
		char	SplitFactor2_Date[14];
		char	SplitFactor3_Date[14];
		char	Leaps[20];
		char    SP500Flag;
		char	filler[86];
		}
		FDR_STOCK, *PFDR_STOCK, *LPFDR_STOCK;

typedef	struct
		{
		char	Name[32];
		char	MinPriceFlux[16];
		char	ContractSize[16];
		char	TradingTime[16];
		char	unused1[16];
		char	Symbol[8];
		char	unused2[24];
		}
		FDR_FUTURE, *PFDR_FUTURE, *LPFDR_FUTURE;

typedef	struct
		{
		BYTE	d[1];
		BYTE	reserved1;
		BYTE	Exchange;				// 'l' or 'f'
		BYTE	reserved2[13];
		char	reserved3[8];
		BYTE	Symbol[8];
		char	OutputSymbol[16];
		char	reserved4[16];
		char	OutputUnderSymbol[8];
		char	IDNUM[9];
		BYTE	Type;
		BYTE	reserved5[3];
		BYTE	CompanyName[40];
		BYTE	reserved6[3];
		}
		FDR_EUROPE, *PFDR_EUROPE, *LPFDR_EUROPE;

/*
**** A-T Fundamental Data record descriptions
*/
typedef	struct _FD_TK_STOCK_US
		{
		char	CUSIP[8];
		char	CUSIPExt[2];
		char	CUSIPCheck;
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	OutputUnderSymbol[6];
		BYTE	MarketFlags[2];
		BYTE	SecuritySubtype;
		BYTE	SIC[2];
		BYTE	IndustryCode[2];
		BYTE	TradingPriceCurrencyCode;
		BYTE	PriceRepCode;
		BYTE	MinFluctuation;
		WORD	CreationChgDate;
		BYTE	StatusFlags[2];
		BYTE	ChangeCode1;
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		WORD	ConversionDate;
		BYTE	ConversionRate[4];
		BYTE	DowJonesIndicator;
		BYTE	SharesPerRndLot[2];
		BYTE	MarginIndicator;
		BYTE	SPRank[4];
		BYTE	ComnShrsOutstanding[4];
		BYTE	PerfShrsOutstanding[4];
		BYTE	InstitutionShares[4];
		BYTE	NumInstHoldingShrs[2];
		BYTE	VolatilityCM12[2];
		BYTE	VolatilityCM11[2];
		BYTE	VolatilityCM10[2];
		BYTE	VolatilityCM9[2];
		BYTE	VolatilityCM8[2];
		BYTE	VolatilityCM7[2];
		BYTE	VolatilityCM6[2];
		BYTE	VolatilityCM5[2];
		BYTE	VolatilityCM4[2];
		BYTE	VolatilityCM3[2];
		BYTE	VolatilityCM2[2];
		BYTE	VolatilityCM1[2];
		BYTE	Volatility6Mon[2];
		BYTE	Volatility20Day[2];
		BYTE	VolatilityImpliedCall[2];
		BYTE	VolatilityImpliedPut[2];
		BYTE	ExchangeSymbol[9];
		BYTE	InterimPeriod[9];
		BYTE	PriorIntermEPSfoot[3];
		BYTE	PriorIntermEPS[2];
		BYTE	CurrIntermEPSfoot[3];
		BYTE	CurrIntermEPS[2];
		BYTE	FiscalYearMonthEnd[2];
		BYTE	EPS12MonthFoot[3];
		BYTE	EPS12Month[4];
		BYTE	EarningsCurrencyCode[3];
		BYTE    PreviousPrimaryGMCCode;
		WORD	DateEPS;
		BYTE	EstEPSFoot[3];
		BYTE	EstEPS[4];
		BYTE	Reserved2;
		BYTE	IndicatedAnnualDiv[4];
		BYTE	IndicatedAnnualDivFoot[3];
		WORD	PrevCashXDivDate;
		BYTE	PrevCashXDivAmount[4];
		WORD	CashXDivDate1;
		BYTE	CashXDivAmount1[4];
		WORD	CashXDivDate2;
		WORD	CashXDivDate3;
		WORD	CashXDivDate4;
		WORD	SpecialExDivDate;
		BYTE	SpecialExDivAmount[4];
		BYTE	Reserved3[3];
		BYTE	FrequencyCode;
		BYTE	DividendSourceCode;
		BYTE	Reserved4[14];
		BYTE	SP500FTSE100IndCode;
		BYTE	SP500GroupCode;
		BYTE	Reserved5[8];
		BYTE	LatestYrCollect[2];
		BYTE	LastFiscalYearEPS[4];
		BYTE	LastFsclYrEPSFoot[3];
		BYTE	StockDivRate[4];
		BYTE	StockDivRateFoot[3];
		WORD	StockXDivDate;
		BYTE	StockXDivRateFoot[3];
		WORD	StockDivPayDate;
		BYTE	StockDivPayDateFoot[3];
		WORD	StockDivRecordDate;
		BYTE	StockDivRecordDateFoot[3];
		WORD	StockDivDeclareDate;
		BYTE	StockDivDeclareDateFoot[3];
		WORD	CashDivPayDate;
		BYTE	CashDivPayDateFoot[3];
		WORD	CashDivRecordDate;
		BYTE	CashDivRecordDateFoot[3];
		WORD	CashDivDeclareDate;
		BYTE	CashDivDeclareDateFoot[3];
		BYTE	CommonSharesFootCodes[3];
		BYTE	PreferredSharesFootCodes[3];
		BYTE	FiscalYearMonthEndFoot[3];
		BYTE	DividendYield[2];
		BYTE	DividendYieldFoot[3];
		BYTE	Reserved6[2];
		BYTE	PriceToEarningsRatio[2];
		BYTE	CashXDivAmount2[4];
		BYTE	CashXDivAmount3[4];
		BYTE	CashXDivAmount4[4];
		char	SpinoffOptionRootSymbol[15];
		BYTE    TradingPriceCurrency[3];
        WORD    ChangeCode2;
        WORD    SourceIdentifier;
		BYTE	GMCBitmap[16];
		BYTE	High52WeekPrice[4];
		WORD	High52WeekDate;
		BYTE	Low52WeekPrice[4];
		WORD	Low52WeekDate;
		BYTE	StockDividendIndicator;
		BYTE	OldCusipNumber[10];
		BYTE	OldOutputTickerSymbol[16];
		BYTE	OldTickerSymbol[16];
		BYTE	Reserved8[27];
		}
		FD_TK_STOCK_US, *PFD_TK_STOCK_US, *LPFD_TK_STOCK_US;

typedef	struct _FD_TK_FUTURE_US
		{
		BYTE	extra2[11];
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	IssuerRootSymbol[6];
		char	extra3[2];
		BYTE	SecuritySubtype;
		BYTE	TradingPriceCurrencyCode[3];
		BYTE	FutureSecuritySubtypeCode;
		char	extra4[6];
		BYTE	UnderSecurityGMC;
		WORD	CreationChangeDate;
		char	extra5;
		BYTE	PriceUnitCode;
		BYTE	UnitPricingSize[2];
		BYTE	UnitMeasureCode[2];
		char	extra6[4];
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		BYTE	ExerciseStyle;
		BYTE	OptionTypeIndicator;
		BYTE	ContractSizePRC;
		char	OutputUnderSymbol[16];
		char	extra7[4];
		BYTE	ExercisePRC;
		char	SecurityShortName[20];
		char	extra8[5];
		BYTE	Volatility6Month[2];
		BYTE	Volatility20Day[2];
		BYTE	VolatilityImpCall[2];
		BYTE	VolatilityImpPut[2];
		BYTE	ContractSize[4];
		BYTE	ExercisePrice[4];
		BYTE	ChangeCode;
		char	extra9[39];
		BYTE	PriceRepCode;
		BYTE	extra10;
		BYTE	TolerancePriceFlux[2];
		BYTE	MaxDailyPriceFlux[2];
		BYTE	MinPriceFluxInt[2];
		BYTE	MinPriceFluxCode;
		char	extra11[27];
		BYTE	SessionIdentifier;
		BYTE	TickValue[2];
		BYTE	TickValuePRC;
		char	CUSIP[9];
		char	PrevCINS[10];
		BYTE	ExchangeSymbol[25];
		BYTE	IssuerPart2[25];
		WORD	OpenTimeRegDay;
		WORD	CloseTimeRegDay;
		WORD	CloseTimeLastDay;
		WORD	LastTradeDate;
		WORD	FirstNoticeDate;
		WORD	FirstDeliveryDate;
		WORD	LastDeliveryDate;
		WORD	ExerciseTimeOnLDD;	// LDD = Last Delivery Date
		BYTE	SourceIdentifierCode[2];
		WORD	ExpirationStrikeDate;
		BYTE	extra12[14];
		BYTE	extra13[15];
		BYTE	GMCBitmap[16];
		BYTE	LifeHighPrice[4];
		WORD	LifeHighDate;
		BYTE	LifeLowPrice[4];
		WORD	LifeLowDate;
		BYTE	FutureSecurityType;
		char	PrevNSIN[10];
		char	PrevOutputTickerSymbol[16];
		char	PrevTickerInternalSymbol[16];
		BYTE	extra14[3];
		BYTE	extra15[3];
		char	ISIN[12];
		BYTE	PrevSecType;
		BYTE	SecType;
		BYTE	SecTypeSupplement;
		BYTE	DatabaseType;
		BYTE	GlobalCenter;
		}
		FD_TK_FUTURE_US, *PFD_TK_FUTURE_US, *LPFD_TK_FUTURE_US;

typedef	struct _FD_TK_STKOPT_US
		{
		char	CUSIP[10];
		char	CUSIPCheck;
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];
		char	ErgoSymbol[16];
		char	OutputUnderRootSymbol[6];
		BYTE	MarketFlags[2];
		BYTE	SecuritySubtype;
		BYTE	SIC[2];
		BYTE	IndustryCode[2];
		BYTE	TradingPriceCurrencyCode;
		BYTE	PriceRepCode;
		BYTE	MinFluctuation;
		BYTE	CreationChgDate[2];
		BYTE	StatusFlags[2];
		BYTE	ChangeCode;
		BYTE	Issuer[25];
		BYTE	IssueDesc[30];
		BYTE	ExpirationDate[2];				// Strike date
		BYTE	ExercisePrice[4];				// Strike price
		BYTE	UnderSecurityGMC;
		BYTE	SharesPerContract[2];
		BYTE    LastTradingDate[2];
		BYTE    ExerciseStyle;
		BYTE    OptionType;
		BYTE	SharesPerContractNew[4];		//new 8/28/95
		BYTE	SharePerContPriceRepCode;		//new 8/28/95
		BYTE    Reserved1[141];
		BYTE    OutputTickerSymbol[16];
		BYTE    ExchangeSymbol[25];
		BYTE    Issuer2[25];
		BYTE    ExercisePriceNew[4];
		BYTE	SpinoffOptionRootSymbol[15];
		BYTE    Currency[3];
		BYTE    ChangeCode2[2];
		BYTE    SourceIdentifier[2];
		BYTE	GMCBitmap[16];
		BYTE	High52WeekPrice[4];
		BYTE	High52WeekDate[2];
		BYTE	Low52WeekPrice[4];
		BYTE	Low52WeekDate[2];
		BYTE	StockDividendIndicator;
		BYTE	OldCusipNumber[10];
		BYTE	OldOutputTickerSymbol[16];
		BYTE	OldTickerSymbol[16];
		BYTE    Reserved2[6];
		BYTE    ISIN[12];
		BYTE    SecurityTypeOld;
		BYTE    SecurityType;
		BYTE    SecuritySubType;
		BYTE    DatabaseType;
		BYTE    GlobalCenter;
		}
		FD_TK_STKOPT_US, *PFD_TK_STKOPT_US, *LPFD_TK_STKOPT_US;


typedef	struct _FD_TK_STOCK_EU 	// And Asian Stocks
		{
		char	SEDOL[10];				// Btrieve key #2
		char	Check;
		BYTE	PrimaryGMC;
		char	TickerSymbol[16];
		char	OutputSymbol[16];		// Btrieve key #1
		char	ErgoSymbol[16];
		char	OutputUnderSymbol[6];
		BYTE	Language;
		BYTE	CalcMethod;
		BYTE	SecuritySubtype;
		BYTE	IssuerDomicile[3];
		BYTE	FutureSecuritySubtype;
		BYTE	CategoryCode;
		BYTE	PriceRepCode;
		BYTE	MinFluctuation;
		WORD	CreationChgDate;
		BYTE	StatusFlags[2];
		BYTE	ChangeCode;
		BYTE	Issuer[25];				// Btrieve key #3
		BYTE	IssueDesc[30];
		BYTE	reserved2[28];
		BYTE	VolatilityCM12[2];
		BYTE	VolatilityCM11[2];
		BYTE	VolatilityCM10[2];
		BYTE	VolatilityCM9[2];
		BYTE	VolatilityCM8[2];
		BYTE	VolatilityCM7[2];
		BYTE	VolatilityCM6[2];
		BYTE	VolatilityCM5[2];
		BYTE	VolatilityCM4[2];
		BYTE	VolatilityCM3[2];
		BYTE	VolatilityCM2[2];
		BYTE	VolatilityCM1[2];
		BYTE	Volatility6Mon[2];
		BYTE	Volatility20Day[2];
		BYTE	VolatilityImpliedCall[2];
		BYTE	VolatilityImpliedPut[2];
		BYTE	ContributorCode[5];
		BYTE	SourceIdCode[2];
		BYTE	reserved3[7];
		BYTE	ChangeCode2[2];
		BYTE	reserved4;
		BYTE	ValorenNumber[7];
		BYTE	reserved5[63];
		BYTE	CUSIPIntlNumber[9];
		BYTE	ParValue[4];
		BYTE	ParValuePRC;
		BYTE	SP500FTSE100IndCode;
		BYTE	PrevCINS[10];
		BYTE	Sedol[9];
		BYTE	ExchangeSymbol[25];
		BYTE	reserved6;
		BYTE	IssuerII[25];
		BYTE	ShortName[20];
		char	SpinoffOptionRootSymbol[15];
		BYTE	PrevValorenNumber[7];
		BYTE	GMCBitmap[16];
		BYTE	Currency[3];
		BYTE	CRSCoverage;
		BYTE	FutureSecurityType;
		BYTE	reserved7[7];
		BYTE	StockDividendIndicator;
		BYTE	OldCusipNumber[10];
		BYTE	OldOutputTickerSymbol[16];
		BYTE	OldTickerSymbol[16];
		BYTE	NISNDomicile[3];
		BYTE	NISNPrevDomicile[3];
		BYTE	ISIN[12];
		BYTE	PrevSecurityType;
		BYTE	SecurityType;
		BYTE	SecuritySubtypeSup;
		BYTE	DatabaseType;
		BYTE	GlobalCenterCode;
		}
		FD_TK_STOCK_EU, *PFD_TK_STOCK_EU, *LPFD_TK_STOCK_EU;

/*
**** Bridge Fundamental Data record descriptions
*/

/*------------ Union of Bridge vechile history ---------*/

typedef	struct
		{
		BYTE	type;					// Type.
		BYTE	unused;					// One byte pad.
		WORD	beta;					// Beta.
		DWORD	high;					// Historic high.
		DWORD	low;					// Historic low.
		}
		BRVHIST;

typedef	union
		{
		BRVHIST	vHist;					// Vehicle history.
		DWORD	reserved[16];			// Max. vehicle history.
		}
		BRVH;

/*------------ Union of Bridge vechile data -----------*/

typedef struct 
		{
    	BYTE	uType;			/* tag of variable portion of record */
    	BYTE	divType;		/* dividend period - type */
    	WORD	divDate;		/* latest dividend date */
    	DWORD	EPS12Month;		/* previous 12 month earnings */
    	DWORD	dividend;		/* latest dividend */
    	DWORD	annualDiv;		/* annualized dividend */
    	DWORD	volatility;		/* intermediate period volatility */
		}
		BR_VD_COMMON;

/* union member for Preferreds */

typedef struct 
		{
    	BYTE	uType;			/* tag of variable portion of record */
    	BYTE	divType;		/* dividend period - type */
    	WORD	divDate;		/* latest dividend date */
    	DWORD	pad;			/* padding */
    	DWORD	cvtRate;		/* conversion rate */
    	DWORD	dividend;		/* latest dividend */
    	DWORD	divFace;		/* face dividend */
		}
		BR_VD_PREFERRED;

/* union member for corporate bonds */

typedef struct 
		{
		BYTE	uType;			/* tag of variable portion of record */
        BYTE	divType;		/* dividend period - type */
        WORD	maturityDate;	/* maturity date */
        DWORD	pad;			/* padding */
        DWORD	cvtRate;		/* conversion rate */
        DWORD	divFace;		/* face dividend */
        WORD    qual[2];		/* quality rating block */
		}
		BR_VD_BOND;

/* union member for warrants */

typedef struct 
		{
		BYTE	uType;			/* tag of variable portion of record */
        BYTE	pad1;			/* padding */
        WORD	maturityDate;	/* maturity date */
        DWORD	pad2;			/* padding */
        DWORD	cvtRate;		/* conversion rate */
        DWORD	strike;			/* strike price */
        DWORD	pad3;			/* padding */
		}
		BR_VD_WARRANT;

typedef struct 
		{
    	BYTE	uType;			/* tag of variable portion of record */
    	BYTE	optionType;		/* type of option */
    	WORD	maturityDate;	/* maturity date */
    	DWORD	pad1;			/* padding */
    	DWORD	cvtRate;		/* conversion rate */
    	DWORD	strike;			/* strike price */
    	DWORD	pad2;			/* padding */
		}
		BR_VD_OPTION;

/* union member for corporate futures */

typedef struct 
		{
        BYTE	uType;			/* tag of variable portion of record */
        BYTE	pad1;			/* padding */
        WORD	maturityDate;	/* maturity date */
        WORD	contractSize;	/* contract size */
        WORD	pad2;			/* padding */
        DWORD	pad3;			/* padding */
        DWORD	cvtRate;		/* conversion rate */
        DWORD	pad4;			/* padding */
		}
		BR_VD_FUTURE;

/* union of all of the above */

typedef union 
		{
    	BYTE             type;
    	BYTE             data[20];
    	BR_VD_COMMON     common;
    	BR_VD_PREFERRED  preferred;
    	BR_VD_BOND       bond;
    	BR_VD_WARRANT    warrant;
    	BR_VD_OPTION     option;
    	BR_VD_FUTURE     future;
		}
		BR_VD_DATA;

typedef	struct
		{
		DWORD	    outstanding;		// Shares outstanding (thousands).
		WORD	    dataDate;			// Data date.
		WORD	    dataTime;			// Data time.
		WORD	    exchSessCnt;		// Exchange/Session record count.
		BYTE	    accessType;			// Access type.
		BYTE	    vehicleFlags;		// Vehicle type.
		BYTE	    priceFmt;			// Price storage format.
		BYTE	    priceDsp;			// Price display format.
		BYTE	    bidExch;			// Exchange of bid.
		BYTE	    askExch;			// Exchange of ask.
		BYTE	    priExch;			// Primary exchange.
		BYTE	    lotSize;			// Index to price/vol table.
		BYTE	    currency[3];		// Currency for vechile price.
		BYTE	    dayType;			// Day type.
		WORD	    mmCnt;				// Number of Market Makers.
		BR_VD_DATA  data;
		}
		BR_VD_EQUITY;

typedef	union
		{
		BR_VD_EQUITY  equity;			// Vehicle data.
		DWORD	      reserved[16];		// Max. vehicle data.
		}
		BRVD;

/*----------- Bridge Fundamental Data record -----------*/

typedef	struct _FD_BR_FUNDDATA
		{
		DWORD	rType;					// Record Type
		char	symbol[12];				// Trading symbol
		char	BSymbol[12];			// Bridge binary trading symbol
		char	ASymbol[16];			// Bridge ASCII trading symbol
		char	Issuer[40];				// Company or Commodity name.
		char	vName[40];				// Vehicle name.
		char	regionID[12];			// Region ID. (US=CUSIP AN9)
		BYTE	priceFmt;				// Price storage format.
		BYTE	priceDsp;				// Price display format.
		BYTE	unused[2];				// 2 byte pad.
		BRVH	vehHist;				// Union of vehicle history.
		BRVD	vehData;				// Union of vehicle data.
		}
		FD_BR_FUNDDATA;

/*----------- First Call Fundamental Data record -----------*/

typedef struct _FC_SNAPSHOTDATA
		{
		BYTE    symbol[12];				// symbol
		BYTE    companyName[30];		// company name
		BYTE    curQtr[4];				// current quarter
		BYTE    fiscalYear[7];			// fiscal year
		BYTE    recommendation[8];		// recommmendation
		BYTE    resv1[7];
		SHORT   cqMean;					// current quarter mean estimate                
		SHORT   cqHigh;					// current quarter high estimate                
		SHORT   cqLow;					// current quarter low estimate                 
		USHORT  cqNum;					// current quarter num estimates                
		SHORT   cqYaActual;				// current quarter year ago actual              
		SHORT   cqUp30;					// current quarter up adjustments last 30 days  
		SHORT   cqDown30;				// current quarter down adjustments last 30 days
		USHORT  cqERD;					// current quarter expected report date         
		SHORT   nqMean;					// next quarter mean estimate                
		SHORT   nqHigh;					// next quarter high estimate                
		SHORT   nqLow;					// next quarter low estimate                 
		USHORT  nqNum;					// next quarter num estimates                
		SHORT   nqYaActual;				// next quarter year ago actual              
		SHORT   nqUp30;					// next quarter up adjustments last 30 days  
		SHORT   nqDown30;				// next quarter down adjustments last 30 days
		USHORT  nqERD;					// next quarter expected report date
		SHORT   cyMean;					// current year mean estimate
		SHORT   cyHigh;					// current year high estimate
		SHORT   cyLow;					// current year low estimate
		SHORT   pyMean;					// previous year mean estimate
		SHORT   cyPERatio;				// current year PE ratio
		SHORT   cyNum;					// current year num estimates
		SHORT   cyUp30;					// current year up adjustments last 30 days    
		SHORT   cyDown30;				// current year down adjustments last 30 days  
		SHORT   cyERD;					// current year expected report date
		SHORT   nyMean;					// next year mean estimate
		SHORT   nyHigh;					// next year high estimate
		SHORT   nyLow;					// next year low estimate
		SHORT   nyPERatio;				// next year PE ratio
		SHORT   nyNum;					// next year num estimates
		SHORT   nyUp30;					// next year up adjustments last 30 days    
		SHORT   nyDown30;				// next year down adjustments last 30 days  
		SHORT   nyERD;					// next year expected report date
		SHORT   fiveYrGrowthRate;		// 5 year growth rate
		SHORT   pqYaActual;				// previous quarter year ago actual
		SHORT   pqMean;					// previous quarter mean estimate
		SHORT   pqActual;				// previous quarter actual
		SHORT   pqDeltaMeanActual;		// previous quarter delta mean estimate and actual 
		SHORT   pqNum;					// previous quarter num estimates
		USHORT  pqARD;					// previous quarter actual report date
		SHORT   pyMeanDup;				// previous year mean (duplicate)
		SHORT   pyActual;				// previous year actual
		SHORT   pyDeltaMeanActual;		// previous year delta mean estimate and actual 
		SHORT   pyNum;					// previous year num estimates
		SHORT   pyYaActual;				// 2 years ago actual
		USHORT  pyARD;					// previous year actual report date
		BYTE    resv2[10];
		}
		FC_SNAPSHOTDATA, *LPFC_SNAPSHOTDATA;

/*------- Special "Union" of Fundamental Data records ------*/

typedef	union
		{
		AMD_STOCK			AmdStock;
		AMD_FUTURE			AmdFuture;
		FDR_STOCK			FdrStock;
		FDR_FUTURE			FdrFuture;
		FDR_EUROPE			FdrEurope;

#if defined(INCL_DBAX)
		DBA_RECORD			Dba;
#endif

		FD_TK_STOCK_US		TkUSStock;
		FD_TK_STKOPT_US		TkUSStkOpt;
		FD_TK_STOCK_EU		TkEUStock;
		FD_TK_FUTURE_US 	TkUSFuture;
		FD_BR_FUNDDATA		BrData;
		FC_SNAPSHOTDATA		FcData;
		}
		FUNDDATA, *PFUNDDATA, *LPFUNDDATA;

/*
**** Format Fundamental Data Field width flags
*/

#define	FF_LJUST					0x8000
#define	FF_SHORT					0x4000
#define	FF_WIDTH_MASK				0x0FFF

// Generic Fundamental Data Field Identifiers

#define FFD_GF_FIRST				1000
#define FFD_GF_LONGNAME				1001
#define FFD_GF_52WHIGH				1002
#define FFD_GF_52WLOW				1003
#define FFD_GF_DIVAMOUNT			1004
#define FFD_GF_DIVDATE				1005
#define FFD_GF_DIVFREQ				1006
#define FFD_GF_CURRENCY				1007
#define FFD_GF_PE_RATIO				1008
#define FFD_GF_EPS					1009
#define FFD_GF_YIELD				1010
#define FFD_GF_SHARES				1011
#define FFD_GF_ANNUALDIV			1012
#define FFD_GF_CUSIP				1013
#define FFD_GF_PRIMARKNUMBER		1014
#define FFD_GF_LAST					1014

		/*==================================*/
		/* Telekurs Fundamental Data Fields */
		/*     for AMD_STOCK records 		*/
		/*==================================*/

#define	FFD_TS_CUSIP				0
#define	FFD_TS_CUSIPEXT				1
#define	FFD_TS_CUSIPCHECK			2
#define	FFD_TS_PRIMARYGMC			3
#define	FFD_TS_TICKERSYMBOL			4
#define	FFD_TS_OUTPUTSYMBOL			5
#define	FFD_TS_ERGOSYMBOL			6
#define	FFD_TS_OUTUNDERSYMBOL		7
#define	FFD_TS_MARKETFLAGS			8
#define	FFD_TS_SECSUBTYPE			9

#define	FFD_TS_SI_CODE				10
#define	FFD_TS_SI_STRING			11
#define	FFD_TS_I_CODE				12
#define	FFD_TS_I_STRING				13
#define	FFD_TS_CURRENCY				14
#define	FFD_TS_PRICEREP				15
#define	FFD_TS_MINFLUX				16
#define	FFD_TS_CREATECHGDATE		17
#define	FFD_TS_STATUS				18
#define	FFD_TS_CHANGECODE			19

#define	FFD_TS_ISSUER				20
#define	FFD_TS_ISSUEDESC			21
#define	FFD_TS_CONVERSIONDATE		22
#define	FFD_TS_CONVERSIONRATE		23
#define	FFD_TS_DOWJONESIND			24
#define	FFD_TS_SHARERNDLOT			25
#define	FFD_TS_MARGININD			26
#define	FFD_TS_SPRANK				27
#define	FFD_TS_COMNSHARESOUT		28
#define	FFD_TS_PERFSHARESOUT		29

#define	FFD_TS_INSTITUTESHARES		30
#define	FFD_TS_NUMINSTHOLDSHRS		31
#define	FFD_TS_VOLCM12				32
#define	FFD_TS_VOLCM11				33
#define	FFD_TS_VOLCM10				34
#define	FFD_TS_VOLCM9				35
#define	FFD_TS_VOLCM8				36
#define	FFD_TS_VOLCM7				37
#define	FFD_TS_VOLCM6				38
#define	FFD_TS_VOLCM5				39

#define	FFD_TS_VOLCM4				40
#define	FFD_TS_VOLCM3				41
#define	FFD_TS_VOLCM2				42
#define	FFD_TS_VOLCM1				43
#define	FFD_TS_VOL6MONTH			44
#define	FFD_TS_VOL20DAY				45
#define	FFD_TS_VOLIMPLIEDCALL		46
#define	FFD_TS_VOLIMPLIEDPUT		47
#define	FFD_TS_EARNFRGNEXCHMARK		48
#define	FFD_TS_INTERIMPERIOD		49

#define	FFD_TS_PRIORINTEPSFOOT		50
#define	FFD_TS_PRIORINTEPS			51
#define	FFD_TS_CURRINTEPSFOOT		52
#define	FFD_TS_CURRINTEPS			53
#define	FFD_TS_FISCALYRMONTHEND		54
#define	FFD_TS_EPS12MONTHFOOT		55
#define	FFD_TS_EPS12MONTH			56
#define	FFD_TS_DATEEPS				57
#define	FFD_TS_ESTEPSFOOT			58
#define	FFD_TS_ESTEPS				59

#define	FFD_TS_DIVFRGNEXCHMARK		60
#define	FFD_TS_INDANNUALDIV			61
#define	FFD_TS_INDANNUALDIVFOOT		62
#define	FFD_TS_PREVCASHXDIVDATE		63
#define	FFD_TS_PREVCASHXDIVAMT		64
#define	FFD_TS_CASHXDIVDATE1		65
#define	FFD_TS_CASHXDIVAMOUNT1		66
#define	FFD_TS_CASHXDIVDATE2		67
#define	FFD_TS_CASHXDIVDATE3		68
#define	FFD_TS_CASHXDIVDATE4		69

#define	FFD_TS_SPECIALXDIVDATE		70
#define	FFD_TS_SPECIALXDIVAMT		71
#define	FFD_TS_FREQUENCY			72
#define	FFD_TS_DIVIDENDSOURCE		73
#define	FFD_TS_SP500FTSE100IND		74
#define	FFD_TS_SP500GROUP			75
#define	FFD_TS_LATESTYRCOLLECT		76
#define	FFD_TS_LASTFISCALYREPS		77
#define	FFD_TS_LSTFSCLYREPSFOOT		78
#define	FFD_TS_STOCKDIVRATE			79

#define	FFD_TS_STOCKDIVRATEFOOT		80
#define	FFD_TS_STOCKXDIVDATE		81
#define	FFD_TS_STOCKXDIVRATEFOOT	82
#define	FFD_TS_STOCKDIVPAYDATE		83
#define	FFD_TS_STOCKDIVPAYDATEFOOT	84
#define	FFD_TS_STOCKDIVRECDATE		85
#define	FFD_TS_STOCKDIVRECDATEFOOT	86
#define	FFD_TS_STOCKDIVDCLRDATE		87
#define	FFD_TS_STOCKDIVDCLRDATEFOOT	88
#define	FFD_TS_CASHDIVPAYDATE		89

#define	FFD_TS_CASHDIVPAYDATEFOOT	90
#define	FFD_TS_CASHDIVRECDATE		91
#define	FFD_TS_CASHDIVRECDATEFOOT	92
#define	FFD_TS_CASHDIVDCLRDATE		93
#define	FFD_TS_CASHDIVDCLRDATEFOOT	94
#define	FFD_TS_COMNSHARESFOOT		95
#define	FFD_TS_PERFSHARESFOOT		96
#define	FFD_TS_FISCALYRMONTHENDFOOT	97
#define	FFD_TS_DIVIDENDYIELD		98
#define	FFD_TS_DIVIDENDYIELDFOOT	99

#define	FFD_TS_PE_RATIO				100
#define	FFD_TS_CASHXDIVAMOUNT2		101
#define	FFD_TS_CASHXDIVAMOUNT3		102
#define	FFD_TS_CASHXDIVAMOUNT4		103
#define	FFD_TS_SPINOPTROOTSYMBOL	104
#define	FFD_TS_GMCLIST				105
#define	FFD_TS_52WEEKHIGHPRICE		106
#define	FFD_TS_52WEEKHIGHDATE		107
#define	FFD_TS_52WEEKLOWPRICE		108
#define	FFD_TS_52WEEKLOWDATE		109

#define	FFD_TS_STOCKDIVIDENDIND		110

#define FFD_TS_LAST					110

		/*==================================*/
		/* Telekurs Fundamental Data Fields */
		/*    for AMD_FUTURE records 		*/
		/*==================================*/

#define	FFD_TF_PRIMARYGMC			0				// 8
#define	FFD_TF_TICKERSYMBOL			1				// 16
#define	FFD_TF_OUTPUTSYMBOL			2				// 16
#define	FFD_TF_ERGOSYMBOL			3				// 16
#define	FFD_TF_OUTUNDERSYMBOL		4				// 6
#define	FFD_TF_SECURITYSUBTYPE		5				// 4
#define	FFD_TF_TRADINGCURRENCY1		6				// 8
#define	FFD_TF_UNDERSECURITYGMC		7				// 8
#define	FFD_TF_CREATECHANGEDATE		8				// 8
#define	FFD_TF_TRADINGCURRENCY2		9				// 8
#define	FFD_TF_PRICEUNIT			10				// 16
#define	FFD_TF_UNITPRICINGSIZE		11				// 8
#define	FFD_TF_UNITMEASURE			12				// 16
#define	FFD_TF_CONTRACTSIZE			13				// 8
#define	FFD_TF_ISSUER				14				// 25
#define	FFD_TF_ISSUEDESC			15				// 30

#define FFD_TF_STRIKEPRICE			16				// 8
#define FFD_TF_VOL6MONTH			17				// 4
#define FFD_TF_VOL20DAY				18				// 4
#define FFD_TF_VOLIMPCALL			19				// 4
#define FFD_TF_VOLIMPPUT			20				// 4
#define FFD_TF_CHANGECODE			21				// 3
#define FFD_TF_TOLPRICEFLUX			22				// 8
#define FFD_TF_MINPRICEFLUX			23				// 8
#define FFD_TF_OPENTIMEREG			24				// 5
#define FFD_TF_CLOSETIMEREG			25				// 5
#define FFD_TF_CLOSETIMELAST		26				// 5
#define FFD_TF_LASTTRADEDATE		27				// 8
#define FFD_TF_FIRSTNOTICEDATE		28				// 8
#define FFD_TF_FIRSTDLVYDATE		29				// 8
#define FFD_TF_LASTDLVYDATE			30				// 8
#define FFD_TF_EXERCISETIMELDD		31				// 5
#define FFD_TF_EXPRSTRIKEDATE		32				// 8
#define FFD_TF_EXCHANGESYMBOL		33				// 15
#define FFD_TF_GMCBITMAP			34				// 20
#define FFD_TF_LIFEHIGHPRICE		35				// 8
#define FFD_TF_LIFEHIGHDATE			36				// 8
#define FFD_TF_LIFELOWPRICE			37				// 8
#define FFD_TF_LIFELOWDATE			38				// 8

#define FFD_TF_LAST					38

		/*==================================*/
		/* Telekurs Fundamental Data Fields */
		/*     for FD_TK_EU_STOCK records   */
		/*==================================*/

#define	FFD_TE_SEDOL				0
#define	FFD_TE_LANGUAGE				1
#define	FFD_TE_CHECK				2
#define	FFD_TE_PRIMARYGMC			3
#define	FFD_TE_TICKERSYMBOL			4
#define	FFD_TE_OUTPUTSYMBOL			5
#define	FFD_TE_ERGOSYMBOL			6
#define	FFD_TE_OUTUNDERSYMBOL		7
#define	FFD_TE_ISSUERDOMICILE		8
#define	FFD_TE_SECSUBTYPE			9

#define	FFD_TE_CUSIP_INTL_NUMBER	10
#define	FFD_TE_CRS_COVERAGE			11
#define	FFD_TE_NISN_DOMICILE		12
#define	FFD_TE_NISN_PREV_DOMICILE	13
#define	FFD_TE_CURRENCY				14
#define	FFD_TE_PRICEREP				15
#define	FFD_TE_MINFLUX				16
#define	FFD_TE_CREATECHGDATE		17
#define	FFD_TE_STATUS				18
#define	FFD_TE_CHANGECODE			19

#define	FFD_TE_ISSUER				20
#define	FFD_TE_ISSUEDESC			21
#define	FFD_TE_CONTRIBUTORCODE		22
#define	FFD_TE_SOURCEIDCODE			23
#define	FFD_TE_CHANGECODE2			24
#define	FFD_TE_VALORENNUMBER		25
#define	FFD_TE_PARVALUE				26
#define	FFD_TE_CALCMETHOD			27
#define	FFD_TE_CATEGORYCODE			28
#define	FFD_TE_ISIN					29

#define	FFD_TE_STOCKDIVIND			30
#define	FFD_TE_SP500FTSE100IND		31
#define	FFD_TE_VOLCM12				32
#define	FFD_TE_VOLCM11				33
#define	FFD_TE_VOLCM10				34
#define	FFD_TE_VOLCM9				35
#define	FFD_TE_VOLCM8				36
#define	FFD_TE_VOLCM7				37
#define	FFD_TE_VOLCM6				38
#define	FFD_TE_VOLCM5				39

#define	FFD_TE_VOLCM4				40
#define	FFD_TE_VOLCM3				41
#define	FFD_TE_VOLCM2				42
#define	FFD_TE_VOLCM1				43
#define	FFD_TE_VOL6MONTH			44
#define	FFD_TE_VOL20DAY				45
#define	FFD_TE_VOLIMPLIEDCALL		46
#define	FFD_TE_VOLIMPLIEDPUT		47
#define	FFD_TE_SPINOPTROOTSYMBOL	48
#define	FFD_TE_EXCHANGESYMBOL		49

#define FFD_TE_LAST					49

		/*==================================*/
		/* ComStock Fundamental Data Fields */
		/*       for StockFdr records       */
		/*==================================*/

#define	FFD_CS_SYMBOL				0
#define	FFD_CS_12MONTHPE			1
#define	FFD_CS_AVGDAILYVOL			2
#define	FFD_CS_HIGHLOW52WK			3
#define	FFD_CS_HIGHLOWCAL			4
#define	FFD_CS_DIVYIELD				5
#define	FFD_CS_LATESTDIVAMT			6
#define	FFD_CS_INDDIVRATE			7
#define	FFD_CS_PAYDATE				8
#define	FFD_CS_EXDIVDATE			9

#define	FFD_CS_INTERIMEARNPERIOD	10
#define	FFD_CS_LATESTINTERIMEARN	11
#define	FFD_CS_LAST12MONTHEPS		12
#define	FFD_CS_REFYEAREARNINGS		13
#define	FFD_CS_CURRENTYEAREPS		14
#define	FFD_CS_NEXTYEAREPSEST		15
#define	FFD_CS_EARNINGSCOMMENT		16
#define	FFD_CS_INTERIMEARNCOMMENT	17
#define	FFD_CS_GROWTHPCT5YEAR		18
#define	FFD_CS_FISCALYEAREND		19

#define	FFD_CS_CUSIP_SPIND			20
#define	FFD_CS_COMPANYNAME			21
#define	FFD_CS_SPRANKCOMMON			22
#define	FFD_CS_OPTIONSYMBOL			23
#define	FFD_CS_PCTHELDBYINST		24
#define	FFD_CS_SPSTARSRATING		25
#define	FFD_CS_BETA					26
#define	FFD_CS_CURRENTASSETS		27
#define	FFD_CS_CURRENTLIABILITIES	28
#define	FFD_CS_BALANCESHEETDATE		29

#define	FFD_CS_LONGTERMDEBT			30
#define	FFD_CS_COMMONSHARESOUT		31
#define	FFD_CS_PREFEREDSHARESOUT	32
#define	FFD_CS_FINANCIALCOMMENT		33
#define	FFD_CS_REFYEARHIGHLOW		34
#define	FFD_CS_YEAR1HIGHLOW			35
#define	FFD_CS_YEAR2HIGHLOW			36
#define	FFD_CS_YEAR3HIGHLOW			37
#define	FFD_CS_YEAR4HIGHLOW			38
#define	FFD_CS_REFYEARDIV			39

#define	FFD_CS_YEAR1DIV				40
#define	FFD_CS_YEAR2DIV				41
#define	FFD_CS_YEAR3DIV				42
#define	FFD_CS_YEAR4DIV				43
#define	FFD_CS_REFYEAREPS			44
#define	FFD_CS_YEAR1EPS				45
#define	FFD_CS_YEAR2EPS				46
#define	FFD_CS_YEAR3EPS				47
#define	FFD_CS_YEAR4EPS				48
#define	FFD_CS_YEAR1EQPS			49

#define	FFD_CS_YEAR2EQPS			50
#define	FFD_CS_YEAR3EQPS			51
#define	FFD_CS_YEAR4EQPS			52
#define	FFD_CS_YEAR1REVENUE			53
#define	FFD_CS_YEAR2REVENUE			54
#define	FFD_CS_YEAR3REVENUE			55
#define	FFD_CS_YEAR4REVENUE			56
#define	FFD_CS_YEAR1NETINCOME		57
#define	FFD_CS_YEAR2NETINCOME		58
#define	FFD_CS_YEAR3NETINCOME		59

#define	FFD_CS_YEAR4NETINCOME		60
#define	FFD_CS_SPLITFACTOR1_DATE	61
#define	FFD_CS_SPLITFACTOR2_DATE	62
#define	FFD_CS_SPLITFACTOR3_DATE	63

#define	FFD_CS_HIGH52WK				64
#define	FFD_CS_LOW52WK				65

#define FFD_CS_LAST					65

		/*==================================*/
		/* ComStock Fundamental Data Fields */
		/*       for Future FDR records     */
		/*==================================*/

#define	FFD_CF_SYMBOL				0
#define	FFD_CF_NAME					1
#define	FFD_CF_MINPRICEFLUX			2
#define	FFD_CF_CONTRACTSIZE			3
#define	FFD_CF_TRADINGTIMES			4

#define FFD_CF_LAST					4

		/*==================================*/
		/* ComStock Fundamental Data Fields */
		/*       for Europe FDR records     */
		/*==================================*/

#define	FFD_CE_SYMBOL				0
#define	FFD_CE_COMPANYNAME			1

#define FFD_CE_LAST					1

		/*==========================================*/
		/* PC Quote Fundamental Data Fields dbn = 51*/
		/*==========================================*/

#define	FFD_PQ_SYMBOL			1			// 12
#define	FFD_PQ_COUNTRYCODE		2			//  2
#define	FFD_PQ_EXCHANGECODE		3			//  2
#define	FFD_PQ_CURRENCYCODE		4			//  2
#define	FFD_PQ_52WEEKHIGH		5			//  8 
#define	FFD_PQ_52WEEKLOW		6			//  8
#define	FFD_PQ_XDIVDATE			7			//  8
#define	FFD_PQ_DIVIDEND			8			// 12
#define	FFD_PQ_FREQUENCY		9			// 11
#define	FFD_PQ_EARNINGS			10			//  8
#define	FFD_PQ_SHARES			11			// 15
#define	FFD_PQ_VOLATILITY		12			//  5
#define	FFD_PQ_VOLUME			13			//  5
#define	FFD_PQ_NAME				14			// 36

#define FFD_PQ_LAST				14

		/*=====================================*/
		/* PC Quote CUSIP (0x1500) Data Fields */
		/*=====================================*/

#define	FFD_PQ1500_SYMBOL			1			// 12
#define	FFD_PQ1500_COUNTRYCODE		2			//  2
#define	FFD_PQ1500_EXCHANGECODE		3			//  2
#define	FFD_PQ1500_CURRENCYCODE		4			//  2

#define	FFD_PQ1500_CUSIP			5			//  9
#define	FFD_PQ1500_DESCRIPTION		6			// 36
#define	FFD_PQ1500_TICKERSYMBOL		7			//  6
#define	FFD_PQ1500_EXCHANGES		8			//  3

#define	FFD_PQ1500_SICCODE			9			//  4
#define	FFD_PQ1500_SP500CAT			10			//  1
#define	FFD_PQ1500_OPTIONIND		11			//  1
#define	FFD_PQ1500_RANK				12			//  4

#define	FFD_PQ1500_CURRENTASSETS	13			// 15
#define	FFD_PQ1500_BOOKVALUE		14			// 15
#define	FFD_PQ1500_COMMONSHARES		15			// 15
#define	FFD_PQ1500_PFDSHARES		16			// 15

#define	FFD_PQ1500_DIVYIELD			17			// 12
#define	FFD_PQ1500_DIVFREQ			18			// 13
#define	FFD_PQ1500_LASTDIVPAYMENT	19			//  8
#define	FFD_PQ1500_DATELASTDIV		20			//  8

#define	FFD_PQ1500_DATEXDIV			21			//  8
#define	FFD_PQ1500_TOTALINDRATE		22			//  8
#define	FFD_PQ1500_YEARDIVMISSED	23			//  2
#define	FFD_PQ1500_EPSLAST12MONTHS	24			// 15
#define FFD_PQ1500_EPSFOOTNOTE      25          //  6
#define FFD_PQ1500_INTERIMMONTHS    26          //  4
#define FFD_PQ1500_INTERIMCODE      27          //  2
#define FFD_PQ1500_EPSLASTINTERIM   28          // 15
#define FFD_PQ1500_EPSCURRINTERIM   29          // 15
#define FFD_PQ1500_EPSGROWTHRATE    30          // 15 
#define FFD_PQ1500_INSTSTKHOLDERS   31          // 15
#define FFD_PQ1500_INSTSHRSHELD     32          // 15
#define FFD_PQ1500_CURRYEARHIGH     33          // 15
#define FFD_PQ1500_CURRYEARLOW      34          // 15
#define FFD_PQ1500_BETAWODIV        35          // 15
#define FFD_PQ1500_BETAWDIV         36          // 15
#define FFD_PQ1500_ANNUALSALES      37          // 15
#define FFD_PQ1500_ANNUALINCOME     38          // 15
#define FFD_PQ1500_FISCALPERIOD     39          //  8
#define FFD_PQ1500_ADRSHARES        40          // 15
#define FFD_PQ1500_ISSUETYPE        41          //  6
#define FFD_PQ1500_SNPFILEDATE      42          // 15

#define FFD_PQ1500_LAST				42

		// PCQUOTE - DBA_RECORD_52 fields

#define	FFD_PQDB52_SYMBOL			1			// 12
#define	FFD_PQDB52_COUNTRYCODE		2			//  2
#define	FFD_PQDB52_EXCHANGECODE		3			//  2
#define	FFD_PQDB52_CURRENCYCODE		4			//  2
#define	FFD_PQDB52_NAME				5			// 36
#define	FFD_PQDB52_DIVFREQ			6			// 11
#define FFD_PQDB52_VOLATILITY		7			//  5
#define	FFD_PQDB52_EXD_DIV_AMT		8			//  5
#define	FFD_PQDB52_SPC_DIV_AMT		9			//  5
#define	FFD_PQDB52_MAT_DIV_AMT		10			//  5
#define	FFD_PQDB52_EXD_DIV_DATE		11			//  8
#define	FFD_PQDB52_SPC_DIV_DATE		12			//  8
#define	FFD_PQDB52_MAT_DIV_DATE		13			//  8
#define	FFD_PQDB52_LAST_SALE_DATE	14			//  8
#define	FFD_PQDB52_CLOSE_PRICE		15			//  8
#define	FFD_PQDB52_52_WEEK_HIGH		16			//  8
#define	FFD_PQDB52_52_WEEK_LOW		17			//  8
#define	FFD_PQDB52_SHARESOUT		18			//  9
#define	FFD_PQDB52_PREVDAYVOL		19			//  9
#define	FFD_PQDB52_EPS				20			//  5
#define	FFD_PQDB52_CNTR_SIZE		21			//	8

#define FFD_PQDB52_LAST				21

		// PCQUOTE - DBA_RECORD_56_1 fields

#define	FFD_PQ561_GROUP				1
#define	FFD_PQ561_CNTR_NAME			2
#define	FFD_PQ561_MARKET			3
#define	FFD_PQ561_PRICETYPE			4
#define	FFD_PQ561_EXPIREDATE		5
#define	FFD_PQ561_CNTR_SIZE			6
#define	FFD_PQ561_CNTR_DESC			7
#define	FFD_PQ561_INTEREST			8
#define	FFD_PQ561_VOLATILITY		9
#define	FFD_PQ561_PRICEUNITS		10
#define	FFD_PQ561_CNTR_SIZE_TYPE	11
#define	FFD_PQ561_PRICE_TO_DOLLARS	12

#define FFD_PQ561_LAST				12

		/*================================*/
		/* Bridge Fundamental Data Fields */
		/*================================*/

#define	FFD_BR_SYMBOL			1			// 12
#define	FFD_BR_BSYMBOL			2			// 12
#define	FFD_BR_ASYMBOL			3			// 16
#define	FFD_BR_NAME				4			// 40
#define	FFD_BR_VEHICLENAME		5			// 40
#define	FFD_BR_CUSIP			6			// 12

#define	FFD_BR_BETA				10			//  8
#define	FFD_BR_52WEEKHIGH		11			//  8 
#define	FFD_BR_52WEEKLOW		12			//  8

#define	FFD_BR_BIDEXCHANGE		20			//  1
#define	FFD_BR_ASKEXCHANGE		21			//  1
#define	FFD_BR_PRIEXCHANGE		22			//  1
#define	FFD_BR_LOTSIZE			23			//  4
#define	FFD_BR_CURRENCYCODE		24			//  3
#define	FFD_BR_SHARES			25			// 12
#define	FFD_BR_DIVFREQ			26			// 10
#define	FFD_BR_XDIVDATE			27			//  8
#define	FFD_BR_EPS12MONTH		28			//  8
#define	FFD_BR_DIVIDEND			29			//  8
#define	FFD_BR_ANNUALDIVIDEND	30			//  8
#define	FFD_BR_VOLATILITY		31			//  8

#define FFD_BR_LAST				31

		/*================================*/
		/* FirstCall Snapshot Data Fields */
		/*================================*/

#define	FFD_FC_SYMBOL			1			// 12
#define	FFD_FC_NAME				2			// 30
#define	FFD_FC_CURQTR			3			// 4
#define	FFD_FC_FISCALYEAR		4			// 7
#define	FFD_FC_RECOMMENDATION	5			// 8
#define	FFD_FC_CQMEAN			6			// 8
#define	FFD_FC_CQHIGH			7			// 8
#define	FFD_FC_CQLOW			8			// 8
#define	FFD_FC_CQNUMBER			9			// 6
#define	FFD_FC_CQYAACTUAL		10			// 8
#define	FFD_FC_CQUP30			11			// 6
#define	FFD_FC_CQDOWN30			12			// 6
#define	FFD_FC_CQERD			13			// 10
#define	FFD_FC_NQMEAN			14			// 8
#define	FFD_FC_NQHIGH			15			// 8
#define	FFD_FC_NQLOW			16			// 8
#define	FFD_FC_NQNUMBER			17			// 6
#define	FFD_FC_NQYAACTUAL		18			// 8
#define	FFD_FC_NQUP30			19			// 6
#define	FFD_FC_NQDOWN30			20			// 6
#define	FFD_FC_NQERD			21			// 10
#define	FFD_FC_PYMEAN			22			// 8
#define	FFD_FC_CYMEAN			23			// 8
#define	FFD_FC_CYHIGH			24			// 8
#define	FFD_FC_CYLOW			25			// 8
#define	FFD_FC_CYNUMBER			26			// 6
#define	FFD_FC_CYPERATIO		27			// 8
#define	FFD_FC_CYUP30			28			// 6
#define	FFD_FC_CYDOWN30			29			// 6
#define	FFD_FC_CYERD			30			// 10
#define	FFD_FC_NYMEAN			31			// 8
#define	FFD_FC_NYHIGH			32			// 8
#define	FFD_FC_NYLOW			33			// 8
#define	FFD_FC_NYNUMBER			34			// 6
#define	FFD_FC_NYPERATIO		35			// 8
#define	FFD_FC_NYUP30			36			// 6
#define	FFD_FC_NYDOWN30			37			// 6
#define	FFD_FC_NYERD			38			// 10
#define	FFD_FC_5YRGROWTHRATE	39			// 8
#define	FFD_FC_PQYAACTUAL		40			// 8
#define	FFD_FC_PQMEAN			41			// 8
#define	FFD_FC_PQACTUAL			42			// 8
#define	FFD_FC_PQDMACTUAL		43			// 8
#define	FFD_FC_PQNUMBER			44			// 6
#define	FFD_FC_PQARD			45			// 10
#define	FFD_FC_PYMEANDUP		46			// 8
#define	FFD_FC_PYACTUAL			47			// 8
#define	FFD_FC_PYDMACTUAL		48			// 8
#define	FFD_FC_PYNUMBER			49			// 6
#define	FFD_FC_PYYAACTUAL		50			// 8
#define	FFD_FC_PYARD			51			// 10

#define	FFD_FC_THISQTR			52			// 6
#define	FFD_FC_NEXTQTR			53			// 6
#define	FFD_FC_PREVQTR			54			// 6

#define	FFD_FC_THISFISCAL		55			// 7
#define	FFD_FC_NEXTFISCAL		56			// 7
#define	FFD_FC_PREVFISCAL		57			// 7

#define FFD_FC_LAST				57

		/*=========================*/
		/* ATFI Option Data Fields */
		/*=========================*/

#define ATFID_ATFI_OPTION_FLAGS					0
#define ATFID_ATFI_OPTION_OPTIONSYMBOL			1
#define ATFID_ATFI_OPTION_UNDERLYINGSYMBOL		2
#define ATFID_ATFI_OPTION_OPTIONTYPE			3
#define ATFID_ATFI_OPTION_EXPIRESTYLE			4
#define ATFID_ATFI_OPTION_EXPIRETYPE			5
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_00		6
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_01		7
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_02		8
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_03		9
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_04		10
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_05		11
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_06		12
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_07		13
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_08		14
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_09		15
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_10		16
#define ATFID_ATFI_OPTION_EXPIRATIONDATE_11		17
#define ATFID_ATFI_OPTION_STRIKEPRICE_00		18
#define ATFID_ATFI_OPTION_STRIKEPRICE_01		19
#define ATFID_ATFI_OPTION_STRIKEPRICE_02		20
#define ATFID_ATFI_OPTION_STRIKEPRICE_03		21
#define ATFID_ATFI_OPTION_STRIKEPRICE_04		22
#define ATFID_ATFI_OPTION_STRIKEPRICE_05		23
#define ATFID_ATFI_OPTION_STRIKEPRICE_06		24
#define ATFID_ATFI_OPTION_STRIKEPRICE_07		25
#define ATFID_ATFI_OPTION_STRIKEPRICE_08		26
#define ATFID_ATFI_OPTION_STRIKEPRICE_09		27
#define ATFID_ATFI_OPTION_STRIKEPRICE_10		28
#define ATFID_ATFI_OPTION_STRIKEPRICE_11		29
#define ATFID_ATFI_OPTION_STRIKEPRICE_12		30
#define ATFID_ATFI_OPTION_STRIKEPRICE_13		31
#define ATFID_ATFI_OPTION_STRIKEPRICE_14		32
#define ATFID_ATFI_OPTION_STRIKEPRICE_15		33
#define ATFID_ATFI_OPTION_STRIKEPRICE_16		34
#define ATFID_ATFI_OPTION_STRIKEPRICE_17		35
#define ATFID_ATFI_OPTION_STRIKEPRICE_18		36
#define ATFID_ATFI_OPTION_STRIKEPRICE_19		37
#define ATFID_ATFI_OPTION_STRIKEPRICE_20		38
#define ATFID_ATFI_OPTION_STRIKEPRICE_21		39
#define ATFID_ATFI_OPTION_STRIKEPRICE_22		40
#define ATFID_ATFI_OPTION_STRIKEPRICE_23		41
#define ATFID_ATFI_OPTION_STRIKEPRICE_24		42
#define ATFID_ATFI_OPTION_STRIKEPRICE_25		43
#define ATFID_ATFI_OPTION_SHARESPERCONTRACT		44
#define ATFID_ATFI_OPTION_PRIMARYEXCHANGE		45
#define ATFID_ATFI_OPTION_UNDERLYINGSYMBOL_1	46
#define ATFID_ATFI_OPTION_SHARESPERCONTRACT_1	47
#define ATFID_ATFI_OPTION_UNDERLYINGSYMBOL_2	48
#define ATFID_ATFI_OPTION_SHARESPERCONTRACT_2	49
#define ATFID_ATFI_OPTION_UNDERLYINGSYMBOL_3	50
#define ATFID_ATFI_OPTION_SHARESPERCONTRACT_3	51
#define ATFID_ATFI_OPTION_CASH					52
#define ATFID_ATFI_OPTION_CONTRACT_TYPE			53
#define ATFID_ATFI_OPTION_CONTRACT_MULTIPLIER	54

/*------------------------------------------------------------------*/

// Corporate Action Records (ComStock) 
//
// Note: Dates in YYYYMMDD format
		
/*------------------------------------------------------------------*/

// Corporation Action Record - Original flavor, full sized

typedef	struct	_FD_CAR
		{
		char	Symbol[11];
		char	Date[8];
		char	Exchange[4];
		char	Name[30];			// Company Name
		char	CUSIP[9];
		char	DtPrevSymbol[19];
		char	DtPrevName[38];
		char	DtPrevCUSIP[17];
		char	DtIPOSymbol[19];
		char	DtDelistedSymbol[19];
		char	DtDivAmount[16];
		char	DtDivPercent[13];
		char	DtSplitFactor[16];
		char	DtSplitRatio[15];
		char	DtAdjFactor[16];
		char	DtPrevExchange[12];
		char	DtComment[88];
		}
		FD_CAR, *PFD_CAR, *LPFD_CAR;

// These records are derived from the original CAR and 
// will eventually replace it. There are 4 "new" records.
// Changes,IPO/Delistings,Dividends and Splits.

// Corporate Action Records - Changes

typedef	struct	_FD_CACHANGE
		{
		char	Symbol[11];
		char	Date[8];
		char	Exchange[4];
		char	Name[30];			// Company Name
		char	CUSIP[9];
		char	DtPrevSymbol[19];
		char	DtPrevName[38];
		char	DtPrevCUSIP[17];
		char	DtPrevExchange[12];
		char	DtComment[88];
		char	DtNewMarketCenter;
		char	DtOldMarketCenter;
		}
		FD_CACHANGE, *PFD_CACHANGE, *LPFD_CACHANGE;

// Corporate Action Records - IPOs and Delistings

typedef	struct	_FD_CAIPO
		{
		char	Symbol[11];
		char	Date[8];
		char	Exchange[4];
		char	Name[30];			// Company Name
		char	CUSIP[9];
		char	DtIPOSymbol[19];
		char	DtDelistedSymbol[19];
		char	DtNewMarketCenter;
		}
		FD_CAIPO, *PFD_CAIPO, *LPFD_CAIPO;

// Corporate Action Records - Dividends

typedef	struct	_FD_CADIV
		{
		char	Symbol[11];
		char	Date[8];
		char	Exchange[4];
		char	Name[30];			// Company Name
		char	CUSIP[9];
		char	DtDivAmount[16];
		char	DtDivPercent[13];
		char	DtAdjFactor[16];
		char	DtComment[88];
		char	DtNewMarketCenter;
		}
		FD_CADIV, *PFD_CADIV, *LPFD_CADIV;

// Corporate Action Records - Splits

typedef	struct	_FD_CASPLIT
		{
		char	Symbol[11];
		char	Date[8];
		char	Exchange[4];
		char	Name[30];			// Company Name
		char	CUSIP[9];
		char	DtSplitFactor[16];
		char	DtSplitRatio[15];
		char	DtAdjFactor[16];
		char	DtComment[88];
		char	DtNewMarketCenter;
		}
		FD_CASPLIT, *PFD_CASPLIT, *LPFD_CASPLIT;

typedef	union
		{
		FD_CACHANGE	change;
		FD_CAIPO	ipo;
		FD_CADIV	div;
		FD_CASPLIT	split;
		}
		FD_CA, *PFD_CA, *LPFD_CA;

/*------------------------------------------------------------------*/

// Corporation Action Record - Telesphere flavor

// Corporate Action Records - Changes

typedef	struct	_FD_TCACHANGE
		{
		char	Symbol[8];
		char	Date[8];
		char	Name[25];			// Company Name
		char	PrevSymbol[8];
		}
		FD_TCACHANGE;

// Corporate Action Records - Dividends

typedef	struct	_FD_TCADIV
		{
		char	Symbol[8];
		char	Date[8];
		char	Name[25];			// Company Name
		BYTE	DivAmount[4];
		}
		FD_TCADIV;

// Corporate Action Records - Splits

typedef	struct	_FD_TCASPLIT
		{
		char	Symbol[8];
		char	Date[8];
		char	Name[25];			// Company Name
		BYTE	SplitFactor[4];
		BYTE	SplitFoot[2];
		BYTE	SplitType;
		WORD	SplitDate;
		}
		FD_TCASPLIT;

typedef	union
		{
		FD_TCACHANGE	change;
		FD_TCADIV		div;
		FD_TCASPLIT		split;
		}
		FD_TCA, *PFD_TCA, *LPFD_TCA;

/*------------------------------------------------------------------*/

		// Earnings Estimates Records (ComStock)
		//
		// Note: Dates in YYYYMMDD format

typedef	struct	_FD_EER
		{
		char	Symbol[11];						// TE0000 + 5 bytes
		char	CUSIP[8];						// TE0001
		char	FsclPeriodOrMonthYrEnds[3];		// TE2000
		char	EPSPrevFsclPeriod[7];			// TE2001

		char	CurYrEPSEstMean[7];				// TE2002
		char	CurYrEPSEstMedian[7];			// TE2003
		char	CurYrEPSEstStdDev[7];			// TE2004
		char	CurYrNumEstRevisedUp[3];		// TE2005
		char	CurYrNumEstRevisedDown[3];		// TE2006
		char	CurYrEPSEstNumOfEst[8];			// TE2007

		char	Year1EPSEstMean[7];				// TE2008
		char	Year1EPSEstMedian[7];			// TE2009
		char	Year1EPSEstStdDev[7];			// TE2010
		char	Year1NumEstRevisedUp[3];		// TE2011
		char	Year1NumEstRevisedDown[3];		// TE2012
		char	Year1EPSEstNumOfEst[8];			// TE2013

		char	Year5ProjGrowthRateMean[7];		// TE2014
		char	Year5ProjGrowthRateMedian[7];	// TE2015
		char	Year5ProjGrowthRateStdDev[7];	// TE2016
		char	Year5ProjGrowthRateNumOfEst[8];	// TE2017

		char	CurYrEPSEstMax[7];				// TE2018
		char	Year1EPSEstMax[7];				// TE2019
		char	CurYrEPSEstMin[7];				// TE2020
		char	Year1EPSEstMin[7];				// TE2021
		char	CurYrEPSEstS_P[7];				// TE2022
		char	Year1EPSEstS_P[7];				// TE2023
		char	CurYrEPSEstAGE[7];				// TE2024
		char	Year1EPSEstAGE[7];				// TE2025

		char	DateDateCreated[6];				// TE2026

		char	CurYrEPSEstYear[4];				// TE2027
		char	Year1EPSEstYear[4];				// TE2028
		char	Year2EPSEstYear[4];				// TE2029
		char	Year2EPSEstMean[7];				// TE2030
		char	Year2EPSEstMedian[7];			// TE2031
		char	Year2EPSEstStdDev[7];			// TE2032

		char	Year2NumEstRevisedUp[3];		// TE2033
		char	Year2NumEstRevisedDown[3];		// TE2034
		char	Year2EPSEstNumOfEst[8];			// TE2035
		char	Year2EPSEstMax[7];				// TE2036
		char	Year2EPSEstMin[7];				// TE2037
		char	Year2EPSEstS_P[7];				// TE2038
		char	Year2EPSEstAGE[7];				// TE2039

		char	DateOfNextReport[6];			// TE2040

		char	CurYrChgFlag30Day;				// TE2041
		char	CurYrChgFlag90Day;				// TE2042
		char	Year1ChgFlag30Day;				// TE2043
		char	Year1ChgFlag90Day;				// TE2044
		char	Year2ChgFlag30Day;				// TE2045
		char	Year2ChgFlag90Day;				// TE2046

		char	BuySellHoldFlag;				// TE2047

		char	TE2048;							// TE2048

		char	NumOfBuyRecmds[3];				// TE2049
		char	NumOfBuyHoldRecmds[3];			// TE2050
		char	NumOfHoldRecmds[3];				// TE2051
		char	NumOfHoldSellRecmds[3];			// TE2052
		char	NumOfSellRecmds[3];				// TE2053
		char	NumOfRecmdsInBSHInd[3];			// TE2054

		char	CurYrQ1EPSEstMean[7];			// TE2055
		char	CurYrQ1EPSEstMedian[7];			// TE2056
		char	CurYrQ1EPSEstStdDev[7];			// TE2057
		char	CurYrQ1EPSEstMax[7];			// TE2058
		char	CurYrQ1EPSEstMin[7];			// TE2059
		char	CurYrQ1NumEstRevisedUp[3];		// TE2060
		char	CurYrQ1NumEstRevisedDown[3];	// TE2061
		char	CurYrQ1EstNumOfEst[3];			// TE2062

		char	CurYrQ2EPSEstMean[7];			// TE2063
		char	CurYrQ2EPSEstMedian[7];			// TE2064
		char	CurYrQ2EPSEstStdDev[7];			// TE2065
		char	CurYrQ2EPSEstMax[7];			// TE2066
		char	CurYrQ2EPSEstMin[7];			// TE2067
		char	CurYrQ2NumEstRevisedUp[3];		// TE2068
		char	CurYrQ2NumEstRevisedDown[3];	// TE2069
		char	CurYrQ2EstNumOfEst[3];			// TE2070

		char	CurYrQ3EPSEstMean[7];			// TE2071
		char	CurYrQ3EPSEstMedian[7];			// TE2072
		char	CurYrQ3EPSEstStdDev[7];			// TE2073
		char	CurYrQ3EPSEstMax[7];			// TE2074
		char	CurYrQ3EPSEstMin[7];			// TE2075
		char	CurYrQ3NumEstRevisedUp[3];		// TE2076
		char	CurYrQ3NumEstRevisedDown[3];	// TE2077
		char	CurYrQ3EstNumOfEst[3];			// TE2078

		char	CurYrQ4EPSEstMean[7];			// TE2079
		char	CurYrQ4EPSEstMedian[7];			// TE2080
		char	CurYrQ4EPSEstStdDev[7];			// TE2081
		char	CurYrQ4EPSEstMax[7];			// TE2082
		char	CurYrQ4EPSEstMin[7];			// TE2083
		char	CurYrQ4NumEstRevisedUp[3];		// TE2084
		char	CurYrQ4NumEstRevisedDown[3];	// TE2085
		char	CurYrQ4EstNumOfEst[3];			// TE2086

		char	Year1Q1EPSEstMean[7];			// TE2087
		char	Year1Q1EPSEstMedian[7];			// TE2088
		char	Year1Q1EPSEstStdDev[7];			// TE2089
		char	Year1Q1EPSEstMax[7];			// TE2090
		char	Year1Q1EPSEstMin[7];			// TE2091
		char	Year1Q1NumEstRevisedUp[3];		// TE2092
		char	Year1Q1NumEstRevisedDown[3];	// TE2093
		char	Year1Q1EstNumOfEst[3];			// TE2094

		char	Year1Q2EPSEstMean[7];			// TE2095
		char	Year1Q2EPSEstMedian[7];			// TE2096
		char	Year1Q2EPSEstStdDev[7];			// TE2097
		char	Year1Q2EPSEstMax[7];			// TE2098
		char	Year1Q2EPSEstMin[7];			// TE2099
		char	Year1Q2NumEstRevisedUp[3];		// TE2100
		char	Year1Q2NumEstRevisedDown[3];	// TE2101
		char	Year1Q2EstNumOfEst[3];			// TE2102

		char	Year1Q3EPSEstMean[7];			// TE2103
		char	Year1Q3EPSEstMedian[7];			// TE2104
		char	Year1Q3EPSEstStdDev[7];			// TE2105
		char	Year1Q3EPSEstMax[7];			// TE2106
		char	Year1Q3EPSEstMin[7];			// TE2107
		char	Year1Q3NumEstRevisedUp[3];		// TE2108
		char	Year1Q3NumEstRevisedDown[3];	// TE2109
		char	Year1Q3EstNumOfEst[3];			// TE2110

		char	Year1Q4EPSEstMean[7];			// TE2111
		char	Year1Q4EPSEstMedian[7];			// TE2112
		char	Year1Q4EPSEstStdDev[7];			// TE2113
		char	Year1Q4EPSEstMax[7];			// TE2114
		char	Year1Q4EPSEstMin[7];			// TE2115
		char	Year1Q4NumEstRevisedUp[3];		// TE2116
		char	Year1Q4NumEstRevisedDown[3];	// TE2117
		char	Year1Q4EstNumOfEst[3];			// TE2118

		char	Year2Q1EPSEstMean[7];			// TE2119
		char	Year2Q1EPSEstMedian[7];			// TE2120
		char	Year2Q1EPSEstStdDev[7];			// TE2121
		char	Year2Q1EPSEstMax[7];			// TE2122
		char	Year2Q1EPSEstMin[7];			// TE2123
		char	Year2Q1NumEstRevisedUp[3];		// TE2124
		char	Year2Q1NumEstRevisedDown[3];	// TE2125
		char	Year2Q1EstNumOfEst[3];			// TE2126

		char	Year2Q2EPSEstMean[7];			// TE2127
		char	Year2Q2EPSEstMedian[7];			// TE2128
		char	Year2Q2EPSEstStdDev[7];			// TE2129
		char	Year2Q2EPSEstMax[7];			// TE2130
		char	Year2Q2EPSEstMin[7];			// TE2131
		char	Year2Q2NumEstRevisedUp[3];		// TE2132
		char	Year2Q2NumEstRevisedDown[3];	// TE2133
		char	Year2Q2EstNumOfEst[3];			// TE2134

		char	Year2Q3EPSEstMean[7];			// TE2135
		char	Year2Q3EPSEstMedian[7];			// TE2136
		char	Year2Q3EPSEstStdDev[7];			// TE2137
		char	Year2Q3EPSEstMax[7];			// TE2138
		char	Year2Q3EPSEstMin[7];			// TE2139
		char	Year2Q3NumEstRevisedUp[3];		// TE2140
		char	Year2Q3NumEstRevisedDown[3];	// TE2141
		char	Year2Q3EstNumOfEst[3];			// TE2142

		char	Year2Q4EPSEstMean[7];			// TE2143
		char	Year2Q4EPSEstMedian[7];			// TE2144
		char	Year2Q4EPSEstStdDev[7];			// TE2145
		char	Year2Q4EPSEstMax[7];			// TE2146
		char	Year2Q4EPSEstMin[7];			// TE2147
		char	Year2Q4NumEstRevisedUp[3];		// TE2148
		char	Year2Q4NumEstRevisedDown[3];	// TE2149
		char	Year2Q4EstNumOfEst[3];			// TE2150

		char	CurYrQ1ChgFlag30Days;			// TE2151
		char	CurYrQ1ChgFlag90Days;			// TE2152
		char	CurYrQ2ChgFlag30Days;			// TE2153
		char	CurYrQ2ChgFlag90Days;			// TE2154
		char	CurYrQ3ChgFlag30Days;			// TE2155
		char	CurYrQ3ChgFlag90Days;			// TE2156
		char	CurYrQ4ChgFlag30Days;			// TE2157
		char	CurYrQ4ChgFlag90Days;			// TE2158

		char	Year1Q1ChgFlag30Days;			// TE2159
		char	Year1Q1ChgFlag90Days;			// TE2160
		char	Year1Q2ChgFlag30Days;			// TE2161
		char	Year1Q2ChgFlag90Days;			// TE2162
		char	Year1Q3ChgFlag30Days;			// TE2163
		char	Year1Q3ChgFlag90Days;			// TE2164
		char	Year1Q4ChgFlag30Days;			// TE2165
		char	Year1Q4ChgFlag90Days;			// TE2166

		char	Year2Q1ChgFlag30Days;			// TE2167
		char	Year2Q1ChgFlag90Days;			// TE2168
		char	Year2Q2ChgFlag30Days;			// TE2169
		char	Year2Q2ChgFlag90Days;			// TE2170
		char	Year2Q3ChgFlag30Days;			// TE2171
		char	Year2Q3ChgFlag90Days;			// TE2172
		char	Year2Q4ChgFlag30Days;			// TE2173
		char	Year2Q4ChgFlag90Days;			// TE2174

		char    Padding;
		}
		FD_EER, *PFD_EER, FAR *LPFD_EER;

// This file is synthesized from AMD_STKOPT records. It may eventually be
// present on other types of QuoteServers. It is a BTRIEVE file which
// goes by the Btrieve File Name: BFN_TK_ATOPTN_US

typedef	struct	_AT_OPTION
		{
		char	OptionRootSymbol[4];
		char	UnderRootSymbol[6];
		PKDATE	ExpirationDate[12];
		DWORD	StrikePrice[26];
		WORD	ShrsPerContract;
		char	SpinoffOptRoot[6];
		BYTE	StockDivIndicator;
		DWORD	ShrsPerContNew;
		char	ShrsPerContRepCode;
		BYTE    ExerciseStyle;
		BYTE	extra[15];
		}
		AT_OPTION, *PAT_OPTION, FAR *LPAT_OPTION;

// From the old ('92) edbx.h file for compatibility purposes.  Not supported
// by PC Quote anymore.
#if defined(INCL_DBAX)
typedef struct _PCQT1500
		{
		DBA_KEY		item;
		char		cusip[9];
		char		description[36];
		SHORT		rectype;			// 0x1500
		SNP_STOCK	stkGuide;
		char		resv[57]; 
		}
		PCQT1500;
#endif

#define DBA_CUSIP	DBA_1400

#define	DBA_KT_STOCK			0
#define	DBA_KT_FORIEGNEXCHANGE	1
#define	DBA_KT_FUTURE			2
#define	DBA_KT_GILT				3
#define	DBA_KT_BOND				3
#define	DBA_KT_STOCKOPTION		4
#define	DBA_KT_FUTUREOPTION		5
#define	DBA_KT_MARKETINDICATOR	6
#define	DBA_KT_DEPOSITRATE		7
// End old edbx.h file.

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif
