// MnStar.h
// Morning Star Header File 
// By Wade and Jane, 7/97
#if !defined(_MNSTAR_H)
#define	_MNSTAR_H

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

#define MNSTAR_SRCH_FORWARD		0
#define MNSTAR_SRCH_BACKWARD	1

#define MNSTAR_MAX_ID_PACKAGE	125

// MorningStar Record Type
#define MNSTAR_RDTYPE_GENERAL				0
#define MNSTAR_RDTYPE_SHORTPERF				1
#define MNSTAR_RDTYPE_LONGPERF				2
#define MNSTAR_RDTYPE_10YRPERF				3
#define MNSTAR_RDTYPE_COMPPERF				4
#define MNSTAR_RDTYPE_RISK					5
#define MNSTAR_RDTYPE_RATIO					6
#define MNSTAR_RDTYPE_OPERFEE				7
#define MNSTAR_RDTYPE_EQPORTST				8
#define MNSTAR_RDTYPE_FIPORTST				9
#define MNSTAR_RDTYPE_COMP				    10
#define MNSTAR_RDTYPE_EXCHANGE			    11
#define MNSTAR_RDTYPE_BASICPERF		        12
#define MNSTAR_RDTYPE_BASICRISK		        13
#define MNSTAR_RDTYPE_BASICRATIO	        14
#define MNSTAR_RDTYPE_POPUPPERF		   	    15
#define MNSTAR_RDTYPE_POPUPRISKRATIO	    16
#define MNSTAR_RDTYPE_POPUPOPERFEE			17
#define MNSTAR_RDTYPE_POPUPPORTST			18
#define MNSTAR_RDTYPE_POPUPBASICPERF		19
#define MNSTAR_RDTYPE_POPUPBASICRISKRATIO	20

// maximum mnstar summaries
#define MAX_MNSTAR_SUMS			8
#define MAX_MNSTAR_MSG  	  520      // 520 bytes for package

//key type
#define MSKT_INTERNALID			0
#define MSKT_FUNDNAME			1
#define MSKT_CATEGORY			2
#define MSKT_TICKER				3
#define MSKT_FAMILY				1

// maximum value stored in the value field (31 bits)
#define MAX_MNSTAR_WD_DEC  	 32767  // 2**15 - 1

#define MNSTAR_CLOSE         65535  //send message app closed

// morning star server errors returned in status field of response
#define MNSTARERR_SUCCESS			0		// successful call
#define MNSTARERR_DB_OPEN			1		// Morning Star diskDB open error
#define MNSTARERR_INVALID_KEY		2		// invalid key number specified
#define MNSTARERR_INVALID_FUNC		3		// invalid function specified
#define MNSTARERR_INVALID_MSG_TYPE	4		// invalid message type ('z' '?')
#define MNSTARERR_KEY_NOT_FOUND		5		// key not found in database
#define MNSTARERR_SERVER_BUSY		6		// server could not process request at this time
#define MNSTARERR_SERVER_SHUTDOWN	7		// server could not process request, shutting donw
#define MNSTARERR_NOT_LOGGED_IN		8		// server could not process request, workstation not logged in
#define MNSTARERR_SUBCOUNT			9		// invalid sub count specified

// ===========================================================================
//value filed with exponecial
typedef struct tagMNSTAR_DW_EXP
{
	DWORD	value:	28,
			exp:	3,
			sign:	1;
}MNSTAR_DW_EXP, *LPMNSTAR_DW_EXP;

// value field with implied decimals
typedef	struct tagMNSTAR_DW_DEC
{
	DWORD	value:	31,		// Lower  31 bits = value with implied 2 decimal digits
			sign:	1;		// Upper 1 bit = sign ( 0 == positive or zero, 1 == negative
} MNSTAR_DW_DEC, *LPMNSTAR_DW_DEC;

typedef struct tagMNSTAR_WD_DEC	  // implied 2 decimal digits
{
	WORD	value : 15,
			sign : 1;			  // 0 positive, 1 negative
} MNSTAR_WD_DEC, *LPMNSTAR_WD_DEC;

typedef struct tagMNSTAR_WD_INT
{
	WORD	value : 15,			   // no implied decimal digits
			sign : 1;			   // 0 positive, 1 negative
} MNSTAR_WD_INT, *LPMNSTAR_WD_INT;

// compressed date field -- Day, Month, Year
typedef	struct _MNSTAR_COMP_DATE
{
	WORD	day:  5,		// Lower  5 bits = day   (1 - 31)
			month:4,		// Middle 4 bits = month (1 - 12)
			year: 7;		// Upper  7 bits = year	 (0 - 99) NOTE: Offset from 1970
} MNSTAR_COMP_DATE, *LPMNSTAR_COMP_DATE;

typedef struct tagMNSTAR_DB_RECORD																								     
{
	DWORD				dwID;				// Internal id by Ms, use only last six digits
	char				sFundName[37];		// Fund Name
	char				sInvCat[2];			// Investment category
	BYTE				byRating;			// Over all star rating, default -999 to 255
	MNSTAR_DW_DEC	  	ddPreMonNAV;		// Previous month-end NAV
	MNSTAR_DW_EXP	  	deCurAsset;			// Current net assets for fund(from $ to k$)
	char				sEInvStyle[2];		// Equity investment style
	char				sFInvStyle[2];		// Fixed income investment style
											// Above from general.txt
	MNSTAR_WD_DEC  		wd12MonDisYield;	// 12 month distribution yield
	MNSTAR_WD_DEC  		wdExpRatio;			// Expense ratio
	MNSTAR_WD_DEC  		wd30DaySecYield;	// 30 day SEC yield
	MNSTAR_WD_DEC  		wdIncomeRatio;		// Income Ratio
	MNSTAR_DW_DEC  		ddTurnoverRatio;	// Turnover ratio
											// Above from ratio.txt
	char				sTicker[5];			// Ticker
	char				sFreePhone[12];		// Toll free Phone number
	char				sLocalPhone[12];	// Local Phone number
	MNSTAR_COMP_DATE	cdInceptionDate;	// Inception date
	char				sFundManager[40];	// Fund Manager
	MNSTAR_WD_INT		wiTenure;			// Fund manager tenue
	char				sMinPurchase[10];	// Minimum Initial Purchase
	char				sMinIRAPur[10];		// Minimum Inittial IRA Purchase
	MNSTAR_WD_DEC  		wdFrontLoad;		// Front load
	MNSTAR_WD_DEC		wdDeferedLoad;		// Defered load
	MNSTAR_WD_DEC  		wdRedemptionFee;	// Redemption Fee
	MNSTAR_WD_DEC  		wd12B_1Fee;			// 12B-1 fee
	MNSTAR_WD_DEC		wdMaxManFee;
	MNSTAR_WD_DEC		wdMinManFee;
	//char				sMaxManFee[15];		// Maximum management fee
	//char				sMinManFee[15];		// Minimum management fee
	char				sOtherManFee[25];	// Other management fee
											// Above from oper_fee.txt
	MNSTAR_DW_DEC  		ddBeta_3yr;			// Beta(3 year)
	MNSTAR_DW_DEC  		ddR_Sqr_3yr;		// R_squared(3 year)
	MNSTAR_DW_DEC  		ddStdev_3yr;		// Standard deviation(3 year)
	MNSTAR_DW_DEC  		ddStdev_5yr;		// Standard deviation(5 year)
	MNSTAR_DW_DEC  		ddMean_3yr;			// Mean(3 year)
	MNSTAR_DW_DEC  		ddMean_5yr;			// Mean(5 year)
	MNSTAR_DW_DEC  		ddRisk;				// Moningstar risk(3 year)
											// Above from risk.txt
	MNSTAR_DW_DEC  		ddMnRetn_3Yr;		// Morningstar return(3 year)
	MNSTAR_DW_DEC  		dd1MonReturn;		// 1 month total return
	MNSTAR_WD_INT  		wi1MonRankAll;		// 1 month percentile rank - all funds -999
	MNSTAR_WD_INT  		wi1MonRankCat;		// 1 month percentile rank - funds category
	MNSTAR_DW_DEC  		dd3MonReturn;		// 3 month total return
	MNSTAR_WD_INT  		wi3MonRankAll;		// 3 month percentile rank - all funds 
	MNSTAR_WD_INT  		wi3MonRankCat;		// 3 month percentile rank - funds category
	MNSTAR_DW_DEC  		ddToDateReturn;		// Year-to-date total return
	MNSTAR_WD_INT  		wiToDtRankAll;		// Year-to-date percentile rank - all funds
	MNSTAR_WD_INT  		wiToDtRankCat;		// Year-to-date percentile rank - funds category
	MNSTAR_DW_DEC  		dd1YrReturn;		// 1 year average annualized total return
	MNSTAR_WD_INT  		wi1YrRankAll;		// 1 year percentile rank - all funds
	MNSTAR_WD_INT  		wi1YrRankCat;		// 1 year percentile rank - fund category
	MNSTAR_DW_DEC  		dd3YrReturn;		// 3 year average annualized total return
	MNSTAR_WD_INT  		wi3YrRankAll;		// 3 year percentile rank - all funds
	MNSTAR_WD_INT  		wi3YrRankCat;		// 3 year percentile rank - fund category
	MNSTAR_DW_DEC  		dd5YrReturn;		// 5 year average annualized toatl return
	MNSTAR_WD_INT  		wi5YrRankAll;		// 5 year percentile rank - all funds
	MNSTAR_WD_INT  		wi5YrRankCat;		// 5 year percentile rank - fund category
	MNSTAR_DW_DEC  		dd10YrReturn;		// 10 year average annualized total return
	MNSTAR_WD_INT		wi10YrRankAll;		// 10 year percentile rank - all funds
	MNSTAR_WD_INT		wi10YrRankCat;		// 10 year percentile rank - fund category
	MNSTAR_DW_DEC  		dd15YrReturn;		// 15 year average annualized total return
	MNSTAR_WD_INT		wi15YrRankAll;		// 15 year percentile rank - all funds
	MNSTAR_WD_INT		wi15YrRankCat;		// 15 year percentile rank - fund category
	MNSTAR_DW_DEC		dd10YrAnlRetn;		// Annual total return(10 years ago)
	MNSTAR_DW_DEC  		dd9YrAnlRetn;		// Annual total return(9 years ago)
	MNSTAR_DW_DEC		dd8YrAnlRetn;		// Annual total return(8 years ago)
	MNSTAR_DW_DEC		dd7YrAnlRetn;		// Annual total return(7 years ago)
	MNSTAR_DW_DEC		dd6YrAnlRetn;		// Annual total return(6 years ago)
	MNSTAR_DW_DEC		dd5YrAnlRetn;		// Annual total return(5 years ago)
	MNSTAR_DW_DEC		dd4YrAnlRetn;		// Annual total return(4 years ago)
	MNSTAR_DW_DEC		dd3YrAnlRetn;		// Annual total return(3 years ago)
	MNSTAR_DW_DEC  		dd2YrAnlRetn;		// Annual total return(2 years ago)
	MNSTAR_DW_DEC  		ddPreYrReturn;		// Annual total return(previous year)
											// Above from perf.txt
	MNSTAR_WD_DEC  		wdCash;				// Cash % -999.00
	MNSTAR_WD_DEC  		wdStock;			// Stock(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdBond;				// Bond(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdForeign;			// Foreign(% - stocks and bonds)
											// Above from comp.txt
}MNSTAR_DB_RECORD, *LPMNSTAR_DB_RECORD;

typedef union tagMNSTAR_SUMMARY_RECORD
{
	char	sTickerSymbol[MAX_MNSTAR_SUMS][5];
	char	sFundName[MAX_MNSTAR_SUMS][37];
}MNSTAR_SUMMARY_RECORD, *LPMNSTAR_SUMMARY_RECORD;


typedef union tagsMNSTAR_KEYS
{
	char    sCategory[2];
	char	sTickerSymbol[5];
	char	sFundName[37];
	char    szFamily[20];
	UCHAR	key[50];			// general purpose member to access this in a generic way	
} MNSTAR_KEYS, *LPMNSTAR_KEYS;	// NOTE: key must be at least 4 bytes larger than biggest key

typedef struct tagsMNSTAR_GENERAL_DBRD		//  56 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	char				sInvCat[2];			// Investment category
	BYTE				byRating;			// Over all star rating, default -999 to 255
	MNSTAR_DW_DEC	  	ddPreMonNAV;		// Previous month-end NAV
	MNSTAR_DW_EXP	  	deCurAsset;			// Current net assets for fund(from $ to k$)
	char				sEInvStyle[2];		// Equity investment style
	char				sFInvStyle[2];		// Fixed income investment style
} MNSTAR_GENERAL_DBRD, *LPMNSTAR_GENERAL_DBRD;

typedef struct tagsMNSTAR_RATIO_DBRD		// 53 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_WD_DEC  		wd12MonDisYield;	// 12 month distribution yield
	MNSTAR_WD_DEC  		wdExpRatio;			// Expense ratio
	MNSTAR_WD_DEC  		wd30DaySecYield;	// 30 day SEC yield
	MNSTAR_WD_DEC  		wdIncomeRatio;		// Income Ratio
	MNSTAR_DW_DEC  		ddTurnoverRatio;	// Turnover ratio
} MNSTAR_RATIO_DBRD, *LPMNSTAR_RATIO_DBRD;

typedef struct tagsMNSTAR_OPERFEE_DBRD		// 197 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	char				sTicker[5];			// Ticker
	char				sFreePhone[12];		// Toll free Phone number
	char				sLocalPhone[12];	// Local Phone number
	MNSTAR_COMP_DATE	cdInceptionDate;	// Inception date
	char				sFundManager[40];	// Fund Manager
	MNSTAR_WD_INT		wiTenure;			// Fund manager tenue
	char				sMinPurchase[10];	// Minimum Initial Purchase
	char				sMinIRAPur[10];		// Minimum Initial IRA Purchase
	MNSTAR_WD_DEC  		wdFrontLoad;		// Front load
	MNSTAR_WD_DEC		wdDeferedLoad;		// Defered load
	MNSTAR_WD_DEC  		wdRedemptionFee;	// Redemption Fee
	MNSTAR_WD_DEC  		wd12B_1Fee;			// 12B-1 fee
	MNSTAR_WD_DEC		wdMaxManFee;
	MNSTAR_WD_DEC		wdMinManFee;
	//char				sMaxManFee[15];		// Maximum management fee
	//char				sMinManFee[15];		// Minimum management fee
	char				sOtherManFee[25];	// Other management fee
} MNSTAR_OPERFEE_DBRD, *LPMNSTAR_OPERFEE_DBRD;

typedef struct tagsMNSTAR_RISK_DBRD			// 69 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddBeta_3yr;			// Beta(3 year)
	MNSTAR_DW_DEC  		ddR_Sqr_3yr;		// R_squared(3 year)
	MNSTAR_DW_DEC  		ddStdev_3yr;		// Standard deviation(3 year)
	MNSTAR_DW_DEC  		ddStdev_5yr;		// Standard deviation(5 year)
	MNSTAR_DW_DEC  		ddMean_3yr;			// Mean(3 year)
	MNSTAR_DW_DEC  		ddMean_5yr;			// Mean(5 year)
	MNSTAR_DW_DEC  		ddRisk;				// Moningstar risk(3 year)
} MNSTAR_RISK_DBRD, *LPMNSTAR_RISK_DBRD;

typedef struct tagsMNSTAR_SHORTPERF_DBRD	// 65 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddMnRetn_3Yr;		// Morningstar return(3 year)
	MNSTAR_DW_DEC  		dd1MonReturn;		// 1 month total return
	MNSTAR_DW_DEC  		dd3MonReturn;		// 3 month total return
	MNSTAR_DW_DEC  		ddToDateReturn;		// Year-to-date total return
	MNSTAR_DW_DEC  		dd1YrReturn;		// 1 year average annualized total return
	MNSTAR_DW_DEC  		dd3YrReturn;		// 3 year average annualized total return
} MNSTAR_SHORTPERF_DBRD, *LPMNSTAR_SHORTPERF_DBRD;

typedef struct tagsMNSTAR_LONGPERF_DBRD		// 65 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddMnRetn_3Yr;		// Morningstar return(3 year)
	MNSTAR_DW_DEC  		dd1YrReturn;		// 1 year average annualized total return
	MNSTAR_DW_DEC  		dd3YrReturn;		// 3 year average annualized total return
	MNSTAR_DW_DEC  		dd5YrReturn;		// 5 year average annualized toatl return
	MNSTAR_DW_DEC  		dd10YrReturn;		// 10 year average annualized total return
	MNSTAR_DW_DEC  		dd15YrReturn;		// 15 year average annualized total return
} MNSTAR_LONGPERF_DBRD, *LPMNSTAR_LONGPERF_DBRD;

typedef struct tagsMNSTAR_10YRPERF_DBRD		// 81 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC		dd10YrAnlRetn;		// Annual total return(10 years ago)
	MNSTAR_DW_DEC  		dd9YrAnlRetn;		// Annual total return(9 years ago)
	MNSTAR_DW_DEC		dd8YrAnlRetn;		// Annual total return(8 years ago)
	MNSTAR_DW_DEC		dd7YrAnlRetn;		// Annual total return(7 years ago)
	MNSTAR_DW_DEC		dd6YrAnlRetn;		// Annual total return(6 years ago)
	MNSTAR_DW_DEC		dd5YrAnlRetn;		// Annual total return(5 years ago)
	MNSTAR_DW_DEC		dd4YrAnlRetn;		// Annual total return(4 years ago)
	MNSTAR_DW_DEC		dd3YrAnlRetn;		// Annual total return(3 years ago)
	MNSTAR_DW_DEC  		dd2YrAnlRetn;		// Annual total return(2 years ago)
	MNSTAR_DW_DEC  		ddPreYrReturn;		// Annual total return(previous year)
} MNSTAR_10YRPERF_DBRD, *LPMNSTAR_10YRPERF_DBRD;

typedef struct tagsMNSTAR_COMPPERF_DBRD		// 73 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_WD_INT  		wi1MonRankAll;		// 1 month percentile rank - all funds -999
	MNSTAR_WD_INT  		wi1MonRankCat;		// 1 month percentile rank - funds category
	MNSTAR_WD_INT  		wi3MonRankAll;		// 3 month percentile rank - all funds 
	MNSTAR_WD_INT  		wi3MonRankCat;		// 3 month percentile rank - funds category
	MNSTAR_WD_INT  		wiToDtRankAll;		// Year-to-date percentile rank - all funds
	MNSTAR_WD_INT  		wiToDtRankCat;		// Year-to-date percentile rank - funds category
	MNSTAR_WD_INT  		wi1YrRankAll;		// 1 year percentile rank - all funds
	MNSTAR_WD_INT  		wi1YrRankCat;		// 1 year percentile rank - fund category
	MNSTAR_WD_INT  		wi3YrRankAll;		// 3 year percentile rank - all funds
	MNSTAR_WD_INT  		wi3YrRankCat;		// 3 year percentile rank - fund category
	MNSTAR_WD_INT  		wi5YrRankAll;		// 5 year percentile rank - all funds
	MNSTAR_WD_INT  		wi5YrRankCat;		// 5 year percentile rank - fund category
	MNSTAR_WD_INT		wi10YrRankAll;		// 10 year percentile rank - all funds
	MNSTAR_WD_INT		wi10YrRankCat;		// 10 year percentile rank - fund category
	MNSTAR_WD_INT		wi15YrRankAll;		// 15 year percentile rank - all funds
	MNSTAR_WD_INT		wi15YrRankCat;		// 15 year percentile rank - fund category
} MNSTAR_COMPPERF_DBRD, *LPMNSTAR_COMPPERF_DBRD;

typedef struct tagsMNSTAR_EQPORTST_DBRD		// 76 bytes
{
	DWORD				dwID;				// Internal id by Ms, use only last six digits
	char				sFundName[37];		// Fund Name
	char				sInvCat[2];			// Investment category
	char				sTicker[5];			// Ticker
	MNSTAR_WD_DEC  		wdPERatio;			// Price to earnings ratio
	MNSTAR_WD_DEC  		wdPBRatio;			// Price to book ratio
	MNSTAR_DW_DEC  		ddMedMakCap;		// Median market cpaitalization(millions of $)
	MNSTAR_WD_DEC		wd5YrGrRate;		// 5 year eearnings growth rate
	MNSTAR_WD_DEC  		wdUtilities;		// Sector weightings(%) Utilities -999.00
	MNSTAR_WD_DEC  		wdEnergy;			// Sector weightings(%) Energy -999.00
	MNSTAR_WD_DEC  		wdFinancials;		// Sector weightings(%) Financials -999.00
	MNSTAR_WD_DEC  		wdIndCyc;			// Sector weightings(%) Industrial cyclicals -999.00
	MNSTAR_WD_DEC  		wdConDurables;		// Sector weightings(%) Consumer durables -999.00
	MNSTAR_WD_DEC  		wdConStaples;		// Sector weightings(%) Consumer staples -999.00
	MNSTAR_WD_DEC  		wdServices;			// Sector weightings(%) Services -999.00
	MNSTAR_WD_DEC  		wdRetail;			// Sector weightings(%) Retail -999.00
	MNSTAR_WD_DEC  		wdHealth;			// Sector weightings(%) Health -999.00
	MNSTAR_WD_DEC  		wdTechnology;		// Sector weightings(%) Technology -999.00
} MNSTAR_EQPORTST_DBRD, *LPMNSTAR_EQPORTST_DBRD;

typedef struct tagsMNSTAR_FIPORTST_DBRD		// 62 bytes
{
	DWORD				dwID;				// Internal id by Ms, use only last six digits
	char				sFundName[37];		// Fund Name
	char				sInvCat[2];			// Investment category
	char				sTicker[5];			// Ticker
	MNSTAR_DW_DEC  		ddAveMaturity;		// Average weithged meaturity(years)
	MNSTAR_DW_DEC  		ddAveCoupon;		// Average weighted coupon
	MNSTAR_DW_DEC  		ddAvePrice;			// Average weighted price
	char		  		sAveCdQuality[10];	// Average credit quality
} MNSTAR_FIPORTST_DBRD, *LPMNSTAR_FIPORTST_DBRD;

typedef struct tagsMNSTAR_COMP_DBRD			// 45 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_WD_DEC  		wdCash;				// Cash % -999.00
	MNSTAR_WD_DEC  		wdStock;			// Stock(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdBond;				// Bond(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdForeign;			// Foreign(% - stocks and bonds)
} MNSTAR_COMP_DBRD, *LPMNSTAR_COMP_DBRD;

typedef struct tagsMNSTAR_EXCHANGE_DBRD		// 62 bytes
{
	DWORD				dwID;				// Internal id by Ms, use only last six digits
	char				sFundName[37];		// Fund Name
	char				sInvCat[2];			// Investment category
	char				sTicker[5];			// Ticker
	MNSTAR_WD_DEC  		wdNYSE;				// New York stock exchange
	MNSTAR_WD_DEC  		wdAMEX;				// American stock exchange
	MNSTAR_WD_DEC  		wdNASDAQ;			// NASDAQ
	MNSTAR_WD_DEC  		wdExForeign;		// Foreign
	MNSTAR_WD_DEC  		wdDowJones;			// Dow Jones 30
	MNSTAR_WD_DEC  		wdSP500;			// S & P 500
	MNSTAR_WD_DEC  		wdSPMid400;			// S & P Mid_cap 400
	MNSTAR_WD_DEC  		wdSmallCap;			// U.S. Small cap
} MNSTAR_EXCHANGE_DBRD, *LPMNSTAR_EXCHANGE_DBRD;

typedef struct tagsMNSTAR_BASIC_PERF_DBRD	// 69 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		dd3MonReturn;		// 3 month total return
	MNSTAR_DW_DEC  		ddToDateReturn;		// Year-to-date total return
	MNSTAR_DW_DEC  		dd1YrReturn;		// 1 year average annualized total return
	MNSTAR_DW_DEC  		dd3YrReturn;		// 3 year average annualized total return
	MNSTAR_DW_DEC  		dd5YrReturn;		// 5 year average annualized toatl return
	MNSTAR_DW_DEC  		dd10YrReturn;		// 10 year average annualized total return
	MNSTAR_DW_DEC  		ddPreYrReturn;		// Annual total return(previous year)
} MNSTAR_BASIC_PERF_DBRD, *LPMNSTAR_BASIC_PERF_DBRD;

typedef struct tagsMNSTAR_BASIC_RISK_DBRD	// 61 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddBeta_3yr;			// Beta(3 year)
	MNSTAR_DW_DEC  		ddR_Sqr_3yr;		// R_squared(3 year)
	MNSTAR_DW_DEC  		ddStdev_3yr;		// Standard deviation(3 year)
	MNSTAR_DW_DEC  		ddMean_3yr;			// Mean(3 year)
	MNSTAR_DW_DEC  		ddRisk;				// Moningstar risk(3 year)
} MNSTAR_BASIC_RISK_DBRD, *LPMNSTAR_BASIC_RISK_DBRD;

typedef struct tagsMNSTAR_BASIC_RATIO_DBRD	// 43 bytes
{
	DWORD				dwID;
	char				sFundName[37];		// Fund Name
	MNSTAR_WD_DEC  		wdExpRatio;			// Expense ratio
} MNSTAR_BASIC_RATIO_DBRD, *LPMNSTAR_BASIC_RATIO_DBRD;

typedef	struct tagsPOPUP_PERF_DBRD
{
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddMnRetn_3Yr;		// Morningstar return(3 year)
	MNSTAR_DW_DEC  		dd1MonReturn;		// 1 month total return
	MNSTAR_DW_DEC  		dd3MonReturn;		// 3 month total return
	MNSTAR_DW_DEC  		ddToDateReturn;		// Year-to-date total return
	MNSTAR_DW_DEC  		dd1YrReturn;		// 1 year average annualized total return
	MNSTAR_DW_DEC  		dd3YrReturn;		// 3 year average annualized total return

	MNSTAR_DW_DEC  		dd5YrReturn;		// 5 year average annualized toatl return
	MNSTAR_DW_DEC  		dd10YrReturn;		// 10 year average annualized total return
	MNSTAR_DW_DEC  		dd15YrReturn;		// 15 year average annualized total return

	MNSTAR_DW_DEC		dd10YrAnlRetn;		// Annual total return(10 years ago)
	MNSTAR_DW_DEC  		dd9YrAnlRetn;		// Annual total return(9 years ago)
	MNSTAR_DW_DEC		dd8YrAnlRetn;		// Annual total return(8 years ago)
	MNSTAR_DW_DEC		dd7YrAnlRetn;		// Annual total return(7 years ago)
	MNSTAR_DW_DEC		dd6YrAnlRetn;		// Annual total return(6 years ago)
	MNSTAR_DW_DEC		dd5YrAnlRetn;		// Annual total return(5 years ago)
	MNSTAR_DW_DEC		dd4YrAnlRetn;		// Annual total return(4 years ago)
	MNSTAR_DW_DEC		dd3YrAnlRetn;		// Annual total return(3 years ago)
	MNSTAR_DW_DEC  		dd2YrAnlRetn;		// Annual total return(2 years ago)
	MNSTAR_DW_DEC  		ddPreYrReturn;		// Annual total return(previous year)

	MNSTAR_WD_INT  		wi1MonRankAll;		// 1 month percentile rank - all funds -999
	MNSTAR_WD_INT  		wi1MonRankCat;		// 1 month percentile rank - funds category
	MNSTAR_WD_INT  		wi3MonRankAll;		// 3 month percentile rank - all funds 
	MNSTAR_WD_INT  		wi3MonRankCat;		// 3 month percentile rank - funds category
	MNSTAR_WD_INT  		wiToDtRankAll;		// Year-to-date percentile rank - all funds
	MNSTAR_WD_INT  		wiToDtRankCat;		// Year-to-date percentile rank - funds category
	MNSTAR_WD_INT  		wi1YrRankAll;		// 1 year percentile rank - all funds
	MNSTAR_WD_INT  		wi1YrRankCat;		// 1 year percentile rank - fund category
	MNSTAR_WD_INT  		wi3YrRankAll;		// 3 year percentile rank - all funds
	MNSTAR_WD_INT  		wi3YrRankCat;		// 3 year percentile rank - fund category
	MNSTAR_WD_INT  		wi5YrRankAll;		// 5 year percentile rank - all funds
	MNSTAR_WD_INT  		wi5YrRankCat;		// 5 year percentile rank - fund category
	MNSTAR_WD_INT		wi10YrRankAll;		// 10 year percentile rank - all funds
	MNSTAR_WD_INT		wi10YrRankCat;		// 10 year percentile rank - fund category
	MNSTAR_WD_INT		wi15YrRankAll;		// 15 year percentile rank - all funds
	MNSTAR_WD_INT		wi15YrRankCat;		// 15 year percentile rank - fund category
}POPUP_PERF_DBRD, *LPPOPUP_PERF_DBRD;

typedef struct tagsPOPUP_RISKRATIO_DBRD
{
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddBeta_3yr;			// Beta(3 year)
	MNSTAR_DW_DEC  		ddR_Sqr_3yr;		// R_squared(3 year)
	MNSTAR_DW_DEC  		ddStdev_3yr;		// Standard deviation(3 year)
	MNSTAR_DW_DEC  		ddStdev_5yr;		// Standard deviation(5 year)
	MNSTAR_DW_DEC  		ddMean_3yr;			// Mean(3 year)
	MNSTAR_DW_DEC  		ddMean_5yr;			// Mean(5 year)
	MNSTAR_DW_DEC  		ddRisk;				// Moningstar risk(3 year)

	MNSTAR_WD_DEC  		wd12MonDisYield;	// 12 month distribution yield
	MNSTAR_WD_DEC  		wdExpRatio;			// Expense ratio
	MNSTAR_WD_DEC  		wd30DaySecYield;	// 30 day SEC yield
	MNSTAR_WD_DEC  		wdIncomeRatio;		// Income Ratio
	MNSTAR_DW_DEC  		ddTurnoverRatio;	// Turnover ratio
}POPUP_RISKRATIO_DBRD, *LPPOPUP_RISKRATIO_DBRD;

typedef struct tagsPOPUP_PORTST_DBRD
{
	char				sFundName[37];		// Fund Name
	BYTE				bEquity : 1,		// 1 if found equity
						bFixed  : 1,		// 1 if found fixed
						resv :    6;
	MNSTAR_WD_DEC  		wdPERatio;			// Price to earnings ratio
	MNSTAR_WD_DEC  		wdPBRatio;			// Price to book ratio
	MNSTAR_DW_DEC  		ddMedMakCap;		// Median market cpaitalization(millions of $)
	MNSTAR_WD_DEC		wd5YrGrRate;		// 5 year eearnings growth rate
	MNSTAR_WD_DEC  		wdUtilities;		// Sector weightings(%) Utilities -999.00
	MNSTAR_WD_DEC  		wdEnergy;			// Sector weightings(%) Energy -999.00
	MNSTAR_WD_DEC  		wdFinancials;		// Sector weightings(%) Financials -999.00
	MNSTAR_WD_DEC  		wdIndCyc;			// Sector weightings(%) Industrial cyclicals -999.00
	MNSTAR_WD_DEC  		wdConDurables;		// Sector weightings(%) Consumer durables -999.00
	MNSTAR_WD_DEC  		wdConStaples;		// Sector weightings(%) Consumer staples -999.00
	MNSTAR_WD_DEC  		wdServices;			// Sector weightings(%) Services -999.00
	MNSTAR_WD_DEC  		wdRetail;			// Sector weightings(%) Retail -999.00
	MNSTAR_WD_DEC  		wdHealth;			// Sector weightings(%) Health -999.00
	MNSTAR_WD_DEC  		wdTechnology;		// Sector weightings(%) Technology -999.00

	MNSTAR_DW_DEC  		ddAveMaturity;		// Average weithged meaturity(years)
	MNSTAR_DW_DEC  		ddAveCoupon;		// Average weighted coupon
	MNSTAR_DW_DEC  		ddAvePrice;			// Average weighted price
	char		  		sAveCdQuality[10];	// Average credit quality

	MNSTAR_WD_DEC  		wdCash;				// Cash % -999.00
	MNSTAR_WD_DEC  		wdStock;			// Stock(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdBond;				// Bond(% - domestic plus foreign)
	MNSTAR_WD_DEC  		wdForeign;			// Foreign(% - stocks and bonds)
}POPUP_PORTST_DBRD, *LPPOPUP_PORTST_DBRD;

typedef struct tagsPOPUP_BASIC_RISKRATIO_DBRD
{
	char				sFundName[37];		// Fund Name
	MNSTAR_DW_DEC  		ddBeta_3yr;			// Beta(3 year)
	MNSTAR_DW_DEC  		ddR_Sqr_3yr;		// R_squared(3 year)
	MNSTAR_DW_DEC  		ddStdev_3yr;		// Standard deviation(3 year)
	MNSTAR_DW_DEC  		ddMean_3yr;			// Mean(3 year)
	MNSTAR_DW_DEC  		ddRisk;				// Moningstar risk(3 year)

	MNSTAR_WD_DEC  		wdExpRatio;			// Expense ratio
}POPUP_BASIC_RISKRATIO_DBRD, *LPPOPUP_BASIC_RISKRATIO_DBRD;

typedef union tagMNSTAR_RECORD
{
	MNSTAR_GENERAL_DBRD			MnStarGeneralRd		   [MAX_MNSTAR_MSG / sizeof(MNSTAR_GENERAL_DBRD)];
	MNSTAR_RATIO_DBRD			MnStarRatioRd		   [MAX_MNSTAR_MSG / sizeof(MNSTAR_RATIO_DBRD)];
	MNSTAR_OPERFEE_DBRD			MnStarOperFeeRd		   [MAX_MNSTAR_MSG / sizeof(MNSTAR_OPERFEE_DBRD)];
	MNSTAR_RISK_DBRD			MnStarRiskRd		   [MAX_MNSTAR_MSG / sizeof(MNSTAR_RISK_DBRD)];
	MNSTAR_SHORTPERF_DBRD		MnStarShortPerfRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_SHORTPERF_DBRD)];
	MNSTAR_LONGPERF_DBRD		MnStarLongPerfRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_LONGPERF_DBRD)];
	MNSTAR_10YRPERF_DBRD		MnStar10yrPerfRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_10YRPERF_DBRD)];
	MNSTAR_COMPPERF_DBRD		MnStarCompPerfRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_COMPPERF_DBRD)];
	MNSTAR_EQPORTST_DBRD		MnStarEqportstRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_EQPORTST_DBRD)];
	MNSTAR_FIPORTST_DBRD		MnStarFiportstRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_FIPORTST_DBRD)];
	MNSTAR_COMP_DBRD			MnStarCompRd		   [MAX_MNSTAR_MSG / sizeof(MNSTAR_COMP_DBRD)];
	MNSTAR_EXCHANGE_DBRD		MnStarExchangeRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_EXCHANGE_DBRD)];
	MNSTAR_BASIC_PERF_DBRD	    MnStarBasicPerfRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_BASIC_PERF_DBRD)];
	MNSTAR_BASIC_RISK_DBRD		MnStarBasicRiskRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_BASIC_RISK_DBRD)];
	MNSTAR_BASIC_RATIO_DBRD		MnStarBasicRatioRd	   [MAX_MNSTAR_MSG / sizeof(MNSTAR_BASIC_RATIO_DBRD)];
	POPUP_PERF_DBRD				PopupPerfRd;
	POPUP_RISKRATIO_DBRD		PopupRiskRatioRd;
	POPUP_PORTST_DBRD			PopupPortstRd;
	POPUP_BASIC_RISKRATIO_DBRD	PopupBasicRiskRatioRd;
}MNSTAR_RECORD, *LPMNSTAR_RECORD;

//MESSAGES
 typedef struct _QSM_MNSTAR_SUM_REQ {					// Request for MnStar Data Summary
	BYTE			cStar, s;							// Message and subtype
	BYTE			nSearchDirection:2,					// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
					nKeyType:6;							// mnstar key type that follows (defined in mnstar.h)
	MNSTAR_KEYS		MnstarKey;							// mnstar key
}QSM_MNSTAR_SUM_REQ, *LPQSM_MNSTAR_SUM_REQ;

typedef struct _QSM_MNSTAR_SUM_RSP {					// Response for MnStar Data Summary
	BYTE				  cStar, s;						// Message and subtype
	BYTE				  nStatus;						// status of the request
	BYTE				  nSubCount;					// subcount in array in mnstar summary Record
	MNSTAR_SUMMARY_RECORD MnstarSummaryRecord;			// Mnstar summary record (Mnstar.h)	
} QSM_MNSTAR_SUM_RSP, *LPQSM_MNSTAR_SUM_RSP;

typedef struct _QSM_MNSTAR_SINGLE_REQ {					// Request for ONE Record
	BYTE			cStar, u;							// Message and subtype '*' 'u(nique)'					
	BYTE			nRecordType : 5,					// What kind of record we need
					nKeyType : 3;						// mnstar key type that follows (defined in mnstar.h)
	MNSTAR_KEYS		MnstarKey;							// mnstar key
} QSM_MNSTAR_SINGLE_REQ, *LPQSM_MNSTAR_SINGLE_REQ;

typedef struct _QSM_MNSTAR_SINGLE_RSP {					// Response for mnstar Data
	BYTE			cStar, u;							// Message and subtype '*' 'u(nique)'
	BYTE			nStatus:5,							// status of the request (0-31)
					resv:3;								// reserved
	MNSTAR_RECORD	MnstarRecord;						// mnstar record (mnstar.h)
} QSM_MNSTAR_SINGLE_RSP, *LPQSM_MNSTAR_SINGLE_RSP;

typedef struct _QSM_MNSTAR_LIST_REQ {					// Request for ONE Record
	BYTE			cStar, l;							// Message and subtype '*' 'l(ist)'					
	HWND			hwnd;								// window handle 
	BYTE			nRecordType : 5,					// What kind of record we need
					nKeyType : 3;						// mnstar key type that follows (defined in mnstar.h)
	BYTE			nSortby;							// For sortable record
	WORD			wNthRecord;                         // Request the wNthRecord
	MNSTAR_KEYS		MnstarKey;							// mnstar key
} QSM_MNSTAR_LIST_REQ, *LPQSM_MNSTAR_LIST_REQ;

typedef struct _QSM_MNSTAR_LIST_RSP {					// Response for mnstar Data
	BYTE			cStar, l;							// Message and subtype '*' 'l(ist)'
	BYTE			nStatus:5,							// status of the request (0-31)
					resv:3;								// reserved
	BYTE			nSubCount;
	WORD			wTotalCount;
	MNSTAR_RECORD	MnstarRecord;						// mnstar record (mnstar.h)
} QSM_MNSTAR_LIST_RSP, *LPQSM_MNSTAR_LIST_RSP;

typedef struct _QSM_MNSTAR_FILE_REQ {
	BYTE			cStar, f;
	HWND			hwnd;
	BYTE			nRecordType;
	BYTE			nSortby;
	BYTE			byPackageNum;
	WORD			wTotalRecords;
	DWORD			dwIDarray[MNSTAR_MAX_ID_PACKAGE];
} QSM_MNSTAR_FILE_REQ, *LPQSM_MNSTAR_FILE_REQ;

typedef struct _QSM_MNSTAR_FILE_RSP {					// Response for mnstar Data
	BYTE			cStar, f;							// Message and subtype '*' 'f(ile)'
	BYTE			nStatus:5,							// status of the request (0-31)
					resv:3;								// reserved
	BYTE			nSubCount;
	WORD			wTotalCount;
	MNSTAR_RECORD	MnstarRecord;						// mnstar record (mnstar.h)
} QSM_MNSTAR_FILE_RSP, *LPQSM_MNSTAR_FILE_RSP;

#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif 
#endif
