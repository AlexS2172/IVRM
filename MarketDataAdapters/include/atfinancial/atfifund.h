/*
 $Header: /include/atfifund.h 59    10/11/00 9:21a Richh $
 *
 *		ATFI Server fundamental data structures
 *
 $Log: /include/atfifund.h $
 * 
 * 59    10/11/00 9:21a Richh
 * added field IDs for DJ sector codes
 * removed Market Guide Fields
 * added date of percent held
 * 
 * 58    10/05/00 2:06p Brian
 * Added lock bits for the primark numbers in the company and security
 * records.
 * 
 * 57    10/04/00 10:42a Richh
 * we don't need 2 country keys in the company database
 * 
 * 56    10/04/00 10:40a Margas
 * Extra key for company database
 * 
 * 55    10/04/00 10:30a Brian
 * Added primark number to company and security DB's and added keys for
 * them.
 * 
 * 54    9/26/00 10:30a Brian
 * Added lock bits to the company and security records.
 * 
 * 53    9/19/00 9:13a Richh
 * added vendor, industry key to industry code and industry sector records
 * 
 * 52    9/08/00 11:39a Richh
 * cleanups
 * 
 * 51    9/08/00 11:13a Richh
 * fixed reserved fields in updated security and company records
 * 
 * 50    9/08/00 9:53a Brian
 * Modified the security and company db's.
 * 
 * 49    9/07/00 1:08p Richh
 * added ATISTYPE_ECONOMIC_SECTOR
 * 
 * 48    9/07/00 12:05p Richh
 * added ATFI_SECTOR_KEY2
 * 
 * 47    9/05/00 4:26a Richh
 * removed ATFI_INDUCODE_KEY0::resv as this key is unique.
 * 
 * 46    8/25/00 10:20a Richh
 * more DJ sector settings in the equity basic fundamental
 * 
 * 45    8/22/00 12:22p Richh
 * added Dow Jones sector, cap weight, and style
 * 
 * 44    7/25/00 10:46a Richh
 * added ATFI_SECURITY::globalTopicTicker
 * 
 * 43    7/25/00 10:39a Richh
 * ATFI_COMPANY::baselineID changed to datastreamID
 * 
 * 42    7/21/00 5:39p Richh
 * added new field IDs for bonds, mutual funds, and money market funds
 * 
 * 41    7/19/00 4:27p Margaret
 * Adding fields to store symbol/exch change info into Bond, Mutual Fund
 * and Money Market Fundamental records.
 * 
 * 40    7/18/00 4:59p Richh
 * key 2 of the event database is now type and time not just type
 * 
 * 39    7/17/00 4:17p Richh
 * added price type and coupon to the ATFI_FUNDAMENTAL_BOND structure.
 * 
 * 38    7/17/00 1:58p Brian
 * Corrected the comments on a few of the new fundamental fields, and
 * modified Company and Security databases to use Primark number as their
 * primary key, and to have a CFI code.
 * 
 * 37    6/23/00 3:01p Richh
 * fixed company database key structures
 * 
 * 36    6/21/00 2:48p Brian
 * Added isoCountry field to the ADDRESS record.
 * 
 * 35    6/15/00 2:31p Brian
 * Corrected some #defines, and increased the size of the segment field.
 * 
 * 34    6/08/00 9:06a Brian
 * In the _FINANCIALS record, operatingExpenses had been misspelled.
 * 
 * 33    6/06/00 10:31a Craigb
 * added Calendar High and Low fields to Fundamental Calculations
 * 
 * 32    6/05/00 3:05p Richh
 * merged new earnings fields from IBES
 * 
 * 31    6/05/00 2:16p Brian
 * Corrected a few errors in the new WS records.
 * 
 * 30    5/24/00 12:02p Richh
 * expanded some fields to 4 bytes instead of 2
 * 
 *       5/23/00 Richh
 * added permission to the fundamental record.
 * 
 * 29    5/23/00 4:35p Richh
 * base2 to equity2
 * expanded indexraw to 16 bytes
 * 
 * 28    5/23/00 4:24p Brian
 * Corrected the indexRaw size from 4 to 8 bytes.
 * 
 * 27    5/23/00 2:24p Brian
 * Adjusted some field sizes in the new records, and changed the comments
 * to indicate the number of decimals represented.
 * 
 * 26    5/23/00 9:48a Brian
 * Added 5 new records to supported expanded Worldscope data.
 * 
 * 25    5/22/00 10:29a Richh
 * added the post and zone record for NYSE stocks
 * added last quarter date to earnings record.
 * 
 *       5/20/00 Richh
 * added the post and zone record for NYSE stocks
 * added last quarter date to earnings record.
 * 
 * 24    5/16/00 5:52p Richh
 * added the description record
 * 
 * 23    5/09/00 3:20p Richh
 * fixed ATFI_SECURITY keys
 * 
 * 22    5/09/00 1:11p Richh
 * added ATFI_COMPANY, ATFI_SECURITY, and ATFI_INDUSTRY
 * 
 * 21    5/01/00 11:03a Brian
 * Added ADDRESS to the union.
 * 
 * 20    5/01/00 10:18a Richh
 * moved field IDs to record locations
 * 
 * ------------- Old style history below --------------
 *
 *		Written 05/14/98	R. Holm
 *
 *		Modified 05/28/98	R. Holm
 *			changed to sub-record format (much better layout).
 *
 *		Modified 05/29/98	R. Holm
 *			added comment field to dividend and split sub-records.
 *
 *		Modified 06/24/98	S. McNeany
 *			more fields and record types.
 *
 *		Modified 08/08/98	R. Holm
 *			no more DB_KEY's.
 *
 *		Modified 08/10/98	R. Holm
 *			added ATFI_FUNDAMENTAL_KEY.
 *
 *		Modified 08/24/98	R. Holm
 *			all dates are AT_DATETIME.
 *
 *		Modified 08/27/98	R. Holm
 *			fixed paymentDateFootnote.
 *
 *		Modified 09/10/98	R. Holm
 *			changed dividend prices to AT_PRICE.
 *
 *		Modified 09/11/98	R. Holm and Steve Xi
 *			added oldest fields and lastUpdate fields to calculated records.
 *
 *		Modified 09/16/98	R. Holm
 *			fixed base equity and income to reflect Market Guide data.
 *
 *		Modified 09/21/98	R. Holm
 *			added currency to dividend record.
 *			changed annual dividend to AT_PRICE.
 *
 *		Modified 10/20/98	R. Holm
 *			fixed primaryExchange from array to single character.
 *
 *		Modified 11/04/98	R. Holm
 *			added new AtOption record.
 *
 *		Modified 11/05/98	R. Holm
 *			fixed declaration of forex record.
 *
 *		Modified 11/09/98	R. Holm
 *			added cash to ATFI_OPTION record.
 *			added ATFI_NEWS record.
 *
 *		Modified 11/11/98	R. Holm
 *			ATFI_OPTION record is now packed correctly.
 *
 *		Modified 11/30/98	R. Holm
 *			added balance sheet record.
 *
 *		Modified 12/06/98	R. Holm
 *			added fundamental delete flag.
 *			added flags to ATFI_OPTION record.
 *
 *		Modified 12/17/98	R. Holm
 *			removed ATFI_FUNDAMENTAL_EQUITY::dateExchangeChange.
 *			removed ATFI_FUNDAMENTAL_EQUITY::dateSymbolChange to dateChange.
 *
 *		Modified 01/05/99	R. Holm
 *			added ATFI_FUNDAMENTAL_KEY1 as key 1 for ATFI fundamental databases.
 *
 *		Modified 01/12/99	R. Holm
 *			added footnotes to dividend record.
 *
 *		Modified 01/16/99	R. Holm
 *			added new index chain record.
 *
 *		Modified 02/01/99	R. Holm
 *			cleaned up the index chain database.
 *
 *		Modified 02/02/99	R. Holm
 *			added future and future option fields
 *			added fixed income type
 *
 *		Modified 02/03/99	R. Holm
 *			added index keys.
 *
 *		Modified 02/08/99	R. Holm
 *			fiscal year is now a string.
 *
 *		Modified 02/09/99	R. Holm
 *			ATFI_FUNDTYPE_INCOME_BALANCE is now ATFI_FUNDTYPE_INCOME
 *			ATFI_FUNDTYPE_BALANCE_SHEET is now ATFI_FUNDTYPE_BALANCE
 *
 *		Modified 02/10/99	R. Holm
 *			added duplicate padding to AtIndex keys.
 *
 *		Modified 02/15/99	R. Holm
 *			added symbol guide databases.
 *
 *		Modified 02/17/99	R. Holm
 *			added structures for the option chain database keys.
 *			added ATFID_*'s.
 *			added ATFI_EQUITY_QUICK1.
 *
 *		Modified 02/18/99	S. Aharrah
 *			added ATFID_EQUITY_FP*'s.
 *			changed ATFI_EQUITY_QUICK1 to ATFI_EQUITY_FP.
 *
 *		Modified 02/24/99	S. Aharrah
 *			added ATFI_EQUITY_FP to the union.
 *
 *		Modified 02/26/99  Q. Liu
 *			added ATFI_FUNDFLAG_ADDBYCHANGE to mark fundamental added by symbol change
 *
 *		Modified 03/08/99  R. Holm
 *			added bond fundamental fields.
 *
 *		Modified 03/10/99  R. Holm
 *			added ATFI_FUNDAMENTAL_EARNINGS::growthRate5Year.
 *
 *		Modified 03/17/99  C. Baker
 *			added comments to ATFI_FUNDAMENTAL_CALCULATED about volatility pricetypes
 *			added comments to ATFI_SESSION about the data
 *
 *		Modified 04/09/99  R. Holm
 *			added sources for IBES, Primark, and Disclosure data.
 *
 *		Modified 04/19/99  R. Holm
 *			added lastQuarter and quarterCount to ATFI_FUNDAMENTAL_BALANCE_SHEET.
 *
 *		Modified 04/23/99  S. Aharrah
 *			added last12MonthEPS to ATFI_EQUITY_FP.
 *
 *		Modified 04/26/99  R. Holm
 *			added ATFI_FUNDAMENTAL_EQUITY::sharesDate.
 *
 *		Modified 05/10/99  R. Holm
 *			added ATFI_INDEX::flags.
 *			added ATFI_SYMBOL::flags.
 *
 *		Modified 05/12/99  R. Holm
 *			added ATFI_OPTION::primaryExchange.
 *
 *		Modified 06/25/99  S. Aharrah
 *			added contractType to ATFI_OPTION::flags.
 *
 *		Modified 06/29/99  R. Holm and C. Baker
 *			added ATFI_FUTURE record and keys.
 *
 *		Modified 07/04/99  R. Holm
 *			added FCTABLE_DATA.
 *
 *		Modified 08/06/99  C. Baker
 *			removed oldestMovingAverage* fields from ATFI_FUNDAMENTAL_CALCULATED
 *			added implied volatility fields to ATFI_FUNDAMENTAL_HISTORICAL
 *
 *		Modified 08/17/99  S. Aharrah
 *			added sharesPerContractMult to ATFI_OPTION::flags.
 *
 *		Modified 10/07/99  C. Baker
 *			added Future Unit of Measure Constants.
 *
 *		Modified 10/11/99  C. Baker
 *			fix Future Unit of Measure Constants values to be less than 256.
 *
 *		Modified 10/13/99  K. Lea and R. Holm
 *			added fixed income fields
 *
 *		Modified 10/22/99  R. Holm
 *			added new fixed income field IDs.
 *
 *		Modified 12/07/99  C. Baker
 *			added avgDailyVolume250Day to ATFI_FUNDAMENTAL_CALCULATED.
 *			added ATFID_CALCULATED_AVGDAILYVOLUME250DAY
 *
 *		Modified 01/05/00  C. Baker
 *			added beta to ATFI_FUNDAMENTAL_CALCULATED.
 *
 *		Modified 01/07/00  R. Holm
 *			moved AT_EVENT_DATA here.
 *			added AT_EVENT database keys - AT_EVENT_KEY0 - AT_EVENT_KEY3.
 *
 *		Modified 01/12/00  S. Aharrah
 *			added contractMultiplier to ATFI_OPTION.
 *
 *		Modified 01/25/00  C. Baker
 *			added YTD and 12 Month Rate of Return to ATFI_FUNDAMENTAL_CALCULATED.
 *
 *		Modified 02/28/00  R. Holm
 *			added the record and keys for the account usage database.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-2000.
 *		All rignts reserved.
 *
 */

/*=============================================================*/

#ifndef __ATFIFUND_DEFINES
#define __ATFIFUND_DEFINES

/*=============================================================*/

//#pragma comment (exestr, "vssID $Header: /include/atfifund.h 59    10/11/00 9:21a Richh $")

/*=============================================================*/

#include <atfidefs.h>

/*=============================================================*/

#pragma pack(1)

/*=============================================================*/

#define ATFI_FUNDTYPE_EQUITY					0
#define ATFI_FUNDTYPE_DIVIDEND					1
#define ATFI_FUNDTYPE_SPLIT						2
#define ATFI_FUNDTYPE_CALCULATED				3
#define ATFI_FUNDTYPE_CALCULATED_HISTORICAL		4
#define ATFI_FUNDTYPE_EARNINGS					5
#define ATFI_FUNDTYPE_INCOME					6
#define ATFI_FUNDTYPE_CHANGE					7
#define ATFI_FUNDTYPE_FUTURE					8
#define ATFI_FUNDTYPE_BOND						9
#define ATFI_FUNDTYPE_INDEX						10
#define ATFI_FUNDTYPE_OPTION					11
#define ATFI_FUNDTYPE_MUTUALFUND				12
#define ATFI_FUNDTYPE_MMFUND					13
#define ATFI_FUNDTYPE_FUTUREOPTION				14
#define ATFI_FUNDTYPE_FOREX						15
#define ATFI_FUNDTYPE_BALANCE					16
#define ATFI_FUNDTYPE_FIXEDINCOME				17
#define ATFI_FUNDTYPE_ADDRESS					18
#define ATFI_FUNDTYPE_DESCRIPTION				19
#define ATFI_FUNDTYPE_POSTPANEL					20
#define ATFI_FUNDTYPE_EQUITY2					21
#define ATFI_FUNDTYPE_DIVIDEND2					22
#define ATFI_FUNDTYPE_EARNINGS2					23
#define ATFI_FUNDTYPE_SEGMENTS					24
#define ATFI_FUNDTYPE_FINANCIALS				25

/*=============================================================*/

#define ATFI_SOURCE_INTERNAL					0
#define ATFI_SOURCE_PRIMARK						1
#define ATFI_SOURCE_DISCLOSURE					2
#define ATFI_SOURCE_IBES						3
#define ATFI_SOURCE_NYSE						4

#define ATFI_FUNDFLAG_DELETE					0x00008000
#define ATFI_FUNDFLAG_VERSION					0xff000000
#define ATFI_FUNDFLAG_ADDBYCHANGE				0x00000001

/*=============================================================*/
// Unit of Measure Constants for Future Contracts

#define ATFI_FUT_UNIT_BAGS				00
#define ATFI_FUT_UNIT_BUSHELS			01
#define ATFI_FUT_UNIT_DOZEN				02
#define ATFI_FUT_UNIT_BARRELS			03
#define ATFI_FUT_UNIT_FINE_TROY_OZ		04
#define ATFI_FUT_UNIT_GALLONS			05
#define ATFI_FUT_UNIT_100_OZ			06
#define ATFI_FUT_UNIT_112_OZ			07
#define ATFI_FUT_UNIT_KILOS				08
#define ATFI_FUT_UNIT_LONG_TONS			09
#define ATFI_FUT_UNIT_METRIC_TONS		10
#define ATFI_FUT_UNIT_OUNCES			11
#define ATFI_FUT_UNIT_POUNDS			12
#define ATFI_FUT_UNIT_SQUARE_FEET		13
#define ATFI_FUT_UNIT_SHORT_TONS		14
#define ATFI_FUT_UNIT_TROY_OUNCES		15
#define ATFI_FUT_UNIT_BRIT_POUND		16
#define ATFI_FUT_UNIT_CAN_DOLLAR		17
#define ATFI_FUT_UNIT_DEUT_MARK			18
#define ATFI_FUT_UNIT_DUT_GUILDER		19
#define ATFI_FUT_UNIT_FRENCH_FRANC		20
#define ATFI_FUT_UNIT_JAPAN_YEN			21
#define ATFI_FUT_UNIT_MEXICAN_PESO		22
#define ATFI_FUT_UNIT_SWISS_FRANC		23
#define ATFI_FUT_UNIT_US_DOLLAR			24
#define ATFI_FUT_UNIT_BOARD_FEET		25
#define ATFI_FUT_UNIT_COINS				26
#define ATFI_FUT_UNIT_INDEX				27
#define ATFI_FUT_UNIT_NONE1				28
#define ATFI_FUT_UNIT_NONE2				29
#define ATFI_FUT_UNIT_NOMINAL			30
#define ATFI_FUT_UNIT_BALE				31
#define ATFI_FUT_UNIT_BOX_CASE			32
#define ATFI_FUT_UNIT_BOTTLE_FLASK		33
#define ATFI_FUT_UNIT_PERCENT			34
#define ATFI_FUT_UNIT_PUNCE				35
#define ATFI_FUT_UNIT_H_W_100LBS		36	// Hundred Weight (Rough Rice)
#define ATFI_FUT_UNIT_H_W_112LBS		37	// Hundred Weight (Rough Rice)
#define ATFI_FUT_UNIT_GRAM				38
#define ATFI_FUT_UNIT_PIECE				39
#define ATFI_FUT_UNIT_10_PIECES			40
#define ATFI_FUT_UNIT_100_PIECES		41
#define ATFI_FUT_UNIT_1000_PIECES		42
#define ATFI_FUT_UNIT_10000_PIECES		43
#define ATFI_FUT_UNIT_HUNDRED			44
#define ATFI_FUT_UNIT_THOUSAND			45
#define ATFI_FUT_UNIT_MILLION			46
#define ATFI_FUT_UNIT_BTU_MILLIONS		47
#define ATFI_FUT_UNIT_MEG_WATT_HOUR		48	// MWH
#define ATFI_FUT_UNIT_AUSTRALIA_DOL		49
#define ATFI_FUT_UNIT_EU_CUR_UNIT		50	// European Currency Unit - ECU
#define ATFI_FUT_UNIT_SOUTH_AFRICA_RAND	51
#define ATFI_FUT_UNIT_RUSSIA_RUBLE		52
#define ATFI_FUT_UNIT_NEW_ZEAL_DOL		53
#define ATFI_FUT_UNIT_INDON_RUPIAH		54	// Indonesia Rupiah
#define ATFI_FUT_UNIT_MALY_RINGGIT		55	// Malaysia Ringgit
#define ATFI_FUT_UNIT_THAI_BAHT			56
#define ATFI_FUT_UNIT_BRAZIL_REAL		57
#define ATFI_FUT_UNIT_FUT_CONTRACT		58
#define ATFI_FUT_UNIT_NDM_WEST			59	// Non-Dry Milk - West Coast

/*=============================================================*/

#define ATFID_RECORD							0xff

/*=============================================================*/

// This is the composite fundamental record for equities
// returned with initial PB request for front page displays.

#define ATFID_EQUITY_FP_NAME					0
#define ATFID_EQUITY_FP_SHARESOUTSTANDING		1	
#define ATFID_EQUITY_FP_CUSIP					2
#define ATFID_EQUITY_FP_PRIMARKNUMBER			3
#define ATFID_EQUITY_FP_PRIMARYSIC				4
#define ATFID_EQUITY_FP_CURRENCY				5
#define ATFID_EQUITY_FP_DIVPRICETYPE			6
#define ATFID_EQUITY_FP_PREVCASHDIVDATE			7
#define ATFID_EQUITY_FP_PREVCASHDIVAMOUNT		8
#define ATFID_EQUITY_FP_NEXTDIVDATE1			9
#define ATFID_EQUITY_FP_NEXTDIVAMOUNT1			10
#define ATFID_EQUITY_FP_NEXTDIVDATE2			11
#define ATFID_EQUITY_FP_NEXTDIVDATE3			12
#define ATFID_EQUITY_FP_NEXTDIVDATE4			13
#define ATFID_EQUITY_FP_DIVFREQUENCYCODE		14
#define ATFID_EQUITY_FP_SPLITPRICETYPE			15
#define ATFID_EQUITY_FP_SPLITFACTOR				16
#define ATFID_EQUITY_FP_SPLITDATE				17
#define ATFID_EQUITY_FP_VOLATILITY6MONTH		18
#define ATFID_EQUITY_FP_VOLATILITY20DAY			19
#define ATFID_EQUITY_FP_HLPRICETYPE				20
#define ATFID_EQUITY_FP_HIGHPRICE				21
#define ATFID_EQUITY_FP_LOWPRICE				22
#define ATFID_EQUITY_FP_HIGHDATE				23
#define ATFID_EQUITY_FP_LOWDATE					24
#define ATFID_EQUITY_FP_LAST12MONTHEPS			25

typedef struct _ATFI_EQUITY_FP {
	char   name[40];
	ULONG  sharesOutstanding;			
	char   cusip[16];
	char   primarkNumber[16];
	WORD   primarySIC;
	char   currency[3];
	UCHAR  dividendPriceType;
	WORD   prevCashDivDate;
	LONG   prevCashDivAmount;
	WORD   nextDivDate1;
	LONG   nextDivAmount1;
	WORD   nextDivDate2;
	WORD   nextDivDate3;
	WORD   nextDivDate4;
	UCHAR  dividendFrequencyCode;
	UCHAR  splitPriceType;
	LONG   splitFactor;
	WORD   splitDate;
	ULONG  volatility6Month;
	ULONG  volatility20Day;
	BYTE   highLowPriceType;
	LONG   highPrice;
	LONG   lowPrice;
	WORD   highDate;
	WORD   lowDate;
	LONG   last12MonthEPS;
} ATFI_EQUITY_FP, *LPATFI_EQUITY_FP, *PATFI_EQUITY_FP;

/*=============================================================*/

typedef struct _ATFI_FUNDAMENTAL_KEY {
	ULONG   dataType;
	AT_KEY  key;
	ULONG   recordType;
} ATFI_FUNDAMENTAL_KEY, *LPATFI_FUNDAMENTAL_KEY, *PATFI_FUNDAMENTAL_KEY;

typedef struct _ATFI_FUNDAMENTAL_KEY1 {
	ULONG   recordType;
	ULONG   dataType;
	AT_KEY  key;
} ATFI_FUNDAMENTAL_KEY1, *LPATFI_FUNDAMENTAL_KEY1, *PATFI_FUNDAMENTAL_KEY1;

typedef struct _ATFI_FUNDAMENTAL_BASE {
	ULONG  flags;
	BYTE   resv[252];
} ATFI_FUNDAMENTAL_BASE, *LPATFI_FUNDAMENTAL_BASE, *PATFI_FUNDAMENTAL_BASE;

#define ATFID_EQUITY_FLAGS						0
#define ATFID_EQUITY_CUSIP						1
#define ATFID_EQUITY_SEDOL						1
#define ATFID_EQUITY_ISIN						1
#define ATFID_EQUITY_NAME						2
#define ATFID_EQUITY_DESCRIPTION				3
#define ATFID_EQUITY_CURRENCY					4
#define ATFID_EQUITY_PRIMARYEXCHANGE			5
#define ATFID_EQUITY_FISCALYEAR					6
#define ATFID_EQUITY_FISCALQUARTEREND			7
#define ATFID_EQUITY_INDUSTRYSIC1				8
#define ATFID_EQUITY_INDUSTRYSIC2				9
#define ATFID_EQUITY_INDUSTRYSIC3				10
#define ATFID_EQUITY_INDUSTRYSIC4				11
#define ATFID_EQUITY_SECTORMARKGUIDE			12
#define ATFID_EQUITY_INDUSTRYMARKGUIDE			13
#define ATFID_EQUITY_INDUSTRYMAPMARKGUIDE		14
#define ATFID_EQUITY_NUMINSTHOLDINGSHARES		15
#define ATFID_EQUITY_PERCENTHELDINSTITUTIONS	16
#define ATFID_EQUITY_SHARESOUTSTANDING			17	
#define ATFID_EQUITY_DATECHANGE					18
#define ATFID_EQUITY_PREVIOUSSYMBOL				19
#define ATFID_EQUITY_PREVIOUSEXCHANGE			20
#define ATFID_EQUITY_FORTUNENUMBER				21
#define ATFID_EQUITY_SHAREHOLDERS				22
#define ATFID_EQUITY_EMPLOYEES					23
#define ATFID_EQUITY_PRIMARKNUMBER				24
#define ATFID_EQUITY_SHARESDATE					25
#define ATFID_EQUITY_DJECONOMIC					26
#define ATFID_EQUITY_DJMARKET					27
#define ATFID_EQUITY_DJINDUSTRY					28
#define ATFID_EQUITY_DJSUBINDUSTRY				29
#define ATFID_EQUITY_DJCAPWEIGHT				30
#define ATFID_EQUITY_DJSTYLE					31
#define ATFID_EQUITY_DATEPERCENTHELD			32

typedef struct _ATFI_FUNDAMENTAL_EQUITY {
	ULONG        flags;
	union {	  
		char     cusip[16];
		char     sedol[16];
		char     isin[16];
	};
	char         name[40];
	char         description[52];
	char         currency[3];
	char         primaryExchange;
	char         fiscalYear[4];
	AT_DATETIME	 fiscalQuarterEnd;
	USHORT		 industrySIC1;
	USHORT		 industrySIC2;
	USHORT		 industrySIC3;
	USHORT		 industrySIC4;
	UCHAR        resv0[6];
	USHORT		 numInstHoldingShares;
	ULONG		 percentHeldInstitutions;
	ULONG		 sharesOutstanding;			
	AT_DATETIME	 dateChange;
	AT_DATETIME  datePercentHeld;
	AT_KEY		 previousSymbol;
	char		 previousExchange;		
	UCHAR        resv2;
	USHORT       fortuneNumber;
	ULONG        shareholders;
	ULONG        employees;
	AT_DATETIME  sharesDate;
	char         djEconomic[4];
	char         djMarket[4];
	char         djIndustry[4];
	char         djSubIndustry[4];
	char         djCapWeight[4];
	char         djStyle[4];
	UCHAR		 resv3[24];
	char         primarkNumber[16];
} ATFI_FUNDAMENTAL_EQUITY, *LPATFI_FUNDAMENTAL_EQUITY, *PATFI_FUNDAMENTAL_EQUITY;

#define ATFID_FUTURE_FLAGS						0
#define ATFID_FUTURE_CUSIP						1
#define ATFID_FUTURE_SEDOL						1
#define ATFID_FUTURE_ISIN1						1
#define ATFID_FUTURE_NAME						2
#define ATFID_FUTURE_DESCRIPTION				3
#define ATFID_FUTURE_OPTIONS					4
#define ATFID_FUTURE_PRICETYPE					5
#define ATFID_FUTURE_UNITOFMEASURE				6
#define ATFID_FUTURE_DAY						7
#define ATFID_FUTURE_SESSION1					8
#define ATFID_FUTURE_SESSION2					9
#define ATFID_FUTURE_LASTDAY					10
#define ATFID_FUTURE_LASTTRADINGDATE			11
#define ATFID_FUTURE_NOTICEDATE					12
#define ATFID_FUTURE_FIRSTDELIVERYDATE			13
#define ATFID_FUTURE_LASTDELIVERYDATE			14
#define ATFID_FUTURE_CONTRACTSIZE				15
#define ATFID_FUTURE_MINIMUMTICK				16
#define ATFID_FUTURE_PRIMARKNUMBER				17

typedef struct _ATFI_SESSION {
	WORD  startTime;	// hour = startTime / 100; minute = startTime % 100
	WORD  endTime;
	BYTE  dayMask;		// Sunday = bit 0, Monday = bit 1, Tuesday = bit 2 ...
	BYTE  resv[3];
} ATFI_SESSION, *LPATFI_SESSION;

typedef struct _ATFI_FUNDAMENTAL_FUTURE {
	ULONG   flags;
	union {	  
		char      cusip[16];
		char      sedol[16];
		char      isin[16];
	};		  
	char          name[40];
	char          description[52];
	BYTE          options;
	BYTE          priceType;
	BYTE          unitOfMeasure;
	BYTE          resv1;
	ATFI_SESSION  day;
	ATFI_SESSION  session1;
	ATFI_SESSION  session2;
	ATFI_SESSION  lastDay;
	AT_DATETIME   lastTradingDate;
	AT_DATETIME   noticeDate;
	AT_DATETIME   firstDeliveryDate;
	AT_DATETIME   lastDeliveryDate;
	ULONG         contractSize;
	AT_PRICE      minimumTick;
	UCHAR         resv[68];
	char          primarkNumber[16];
} ATFI_FUNDAMENTAL_FUTURE, *LPATFI_FUNDAMENTAL_FUTURE, *PATFI_FUNDAMENTAL_FUTURE;

#define ATFID_BOND_FLAGS						0
#define ATFID_BOND_CUSIP						1
#define ATFID_BOND_SEDOL						1
#define ATFID_BOND_ISIN							1
#define ATFID_BOND_NAME							2
#define ATFID_BOND_DESCRIPTION					3
#define ATFID_BOND_PRIMARKNUMBER				4
#define ATFID_BOND_CALLDATE						5
#define ATFID_BOND_MATURITYDATE					6
#define ATFID_BOND_YIELD						7
#define ATFID_BOND_COUPON						8
#define ATFID_BOND_PRICETYPE					9
#define ATFID_BOND_CHANGEDATE					10
#define ATFID_BOND_PREVIOUSSYMBOL				11
#define ATFID_BOND_PREVIOUSEXCHANGE				12

typedef struct _ATFI_FUNDAMENTAL_BOND {
	ULONG        flags;
	union {	  
		char     cusip[16];
		char     sedol[16];
		char     isin[16];
	};		  
	char         name[40];
	char         description[52];
	char		 resv[3];
	char		 primaryExchange;
	AT_DATETIME  callDate;
	AT_DATETIME  maturityDate;
	AT_PRICE     yield;
	AT_PRICE     coupon;
	UCHAR        priceType;
	UCHAR		 resv1[15];
	AT_DATETIME	 dateChange;
	UCHAR		 resv2[4];
	AT_KEY		 previousSymbol;
	char		 previousExchange;		
	UCHAR        resv3[63];
	char         primarkNumber[16];
} ATFI_FUNDAMENTAL_BOND, *LPATFI_FUNDAMENTAL_BOND, *PATFI_FUNDAMENTAL_BOND;

#define ATFID_OPTION_FLAGS						0
#define ATFID_OPTION_CUSIP						1
#define ATFID_OPTION_SEDOL						1
#define ATFID_OPTION_ISIN						1
#define ATFID_OPTION_NAME						2
#define ATFID_OPTION_DESCRIPTION				3
#define ATFID_OPTION_PRIMARKNUMBER				4

typedef struct _ATFI_FUNDAMENTAL_OPTION {
	ULONG   flags;
	union {	  
		char  cusip[16];
		char  sedol[16];
		char  isin[16];
	};		  
	char    name[40];
	char    description[52];
	UCHAR   resv[128];
	char    primarkNumber[16];
} ATFI_FUNDAMENTAL_OPTION, *LPATFI_FUNDAMENTAL_OPTION, *PATFI_FUNDAMENTAL_OPTION;

#define ATFID_INDEX_FLAGS						0
#define ATFID_INDEX_CUSIP						1
#define ATFID_INDEX_SEDOL						1
#define ATFID_INDEX_ISIN						1
#define ATFID_INDEX_NAME						2
#define ATFID_INDEX_DESCRIPTION					3
#define ATFID_INDEX_PRIMARKNUMBER				4

typedef struct _ATFI_FUNDAMENTAL_INDEX {
	ULONG   flags;
	union {	  
		char  cusip[16];
		char  sedol[16];
		char  isin[16];
	};		  
	char    name[40];
	char    description[52];
	UCHAR   resv[128];
	char    primarkNumber[16];
} ATFI_FUNDAMENTAL_INDEX, *LPATFI_FUNDAMENTAL_INDEX, *PATFI_FUNDAMENTAL_INDEX;

#define ATFID_FUTUREOPTION_FLAGS				0
#define ATFID_FUTUREOPTION_CUSIP				1
#define ATFID_FUTUREOPTION_SEDOL				1
#define ATFID_FUTUREOPTION_ISIN					1
#define ATFID_FUTUREOPTION_NAME					2
#define ATFID_FUTUREOPTION_DESCRIPTION			3
#define ATFID_FUTUREOPTION_OPTIONS				4
#define ATFID_FUTUREOPTION_PRICETYPE			5
#define ATFID_FUTUREOPTION_UNITOFMEASURE		6
#define ATFID_FUTUREOPTION_DAY					7
#define ATFID_FUTUREOPTION_SESSION1				8
#define ATFID_FUTUREOPTION_SESSION2				9
#define ATFID_FUTUREOPTION_LASTDAY				10
#define ATFID_FUTUREOPTION_LASTTRADINGDATE		11
#define ATFID_FUTUREOPTION_NOTICEDATE			12
#define ATFID_FUTUREOPTION_FIRSTDELIVERYDATE	13
#define ATFID_FUTUREOPTION_LASTDELIVERYDATE		14
#define ATFID_FUTUREOPTION_CONTRACTSIZE			15
#define ATFID_FUTUREOPTION_MINIMUMTICK			16
#define ATFID_FUTUREOPTION_UNDERLYINGDATATYPE	17
#define ATFID_FUTUREOPTION_UNDERLYINGSYMBOL		18
#define ATFID_FUTUREOPTION_EXCHANGESYMBOL		19
#define ATFID_FUTUREOPTION_PRIMARKNUMBER		20

typedef struct _ATFI_FUNDAMENTAL_FUTUREOPTION {
	ULONG   flags;
	union {	  
		char      cusip[16];
		char      sedol[16];
		char      isin[16];
	};		  
	char          name[40];
	char          description[52];
	BYTE          options;
	BYTE          priceType;
	BYTE          unitOfMeasure;
	BYTE          resv1;
	ATFI_SESSION  day;
	ATFI_SESSION  session1;
	ATFI_SESSION  session2;
	ATFI_SESSION  lastDay;
	AT_DATETIME   lastTradingDate;
	AT_DATETIME   noticeDate;
	AT_DATETIME   firstDeliveryDate;
	AT_DATETIME   lastDeliveryDate;
	ULONG         contractSize;
	AT_PRICE      minimumTick;
	BYTE          resv2[3];
	BYTE          underlyingDataType;
	AT_KEY        underlyingSymbol;
	AT_KEY        exchangeSymbol;
	UCHAR         resv3[24];
	char          primarkNumber[16];
} ATFI_FUNDAMENTAL_FUTUREOPTION, *LPATFI_FUNDAMENTAL_FUTUREOPTION, *PATFI_FUNDAMENTAL_FUTUREOPTION;

#define ATFID_MUTUALFUND_FLAGS					0
#define ATFID_MUTUALFUND_CUSIP					1
#define ATFID_MUTUALFUND_SEDOL					1
#define ATFID_MUTUALFUND_ISIN					1
#define ATFID_MUTUALFUND_NAME					2
#define ATFID_MUTUALFUND_DESCRIPTION			3
#define ATFID_MUTUALFUND_PRIMARKNUMBER			4
#define ATFID_MUTUALFUND_CHANGEDATE				5
#define ATFID_MUTUALFUND_PREVIOUSSYMBOL			6

typedef struct _ATFI_FUNDAMENTAL_MUTUALFUND {
	ULONG        flags;
	union {	  
		char     cusip[16];
		char     sedol[16];
		char     isin[16];
	};		  
	char         name[40];
	char         description[52];
	UCHAR        resv[36];
	AT_DATETIME	 dateChange;
	UCHAR		 resv2[4];
	AT_KEY		 previousSymbol;
	UCHAR        resv3[64];
	char         primarkNumber[16];
} ATFI_FUNDAMENTAL_MUTUALFUND, *LPATFI_FUNDAMENTAL_MUTUALFUND, *PATFI_FUNDAMENTAL_MUTUALFUND;

#define ATFID_MONEYMARKET_FLAGS					0
#define ATFID_MONEYMARKET_CUSIP					1
#define ATFID_MONEYMARKET_SEDOL					1
#define ATFID_MONEYMARKET_ISIN					1
#define ATFID_MONEYMARKET_NAME					2
#define ATFID_MONEYMARKET_DESCRIPTION			3
#define ATFID_MONEYMARKET_PRIMARKNUMBER			4
#define ATFID_MONEYMARKET_CHANGEDATE			5
#define ATFID_MONEYMARKET_PREVIOUSSYMBOL		6

typedef struct _ATFI_FUNDAMENTAL_MMFUND {
	ULONG        flags;
	union {	  
		char     cusip[16];
		char     sedol[16];
		char     isin[16];
	};		  
	char         name[40];
	char         description[52];
	UCHAR        resv[36];
	AT_DATETIME	 dateChange;
	UCHAR		 resv2[4];
	AT_KEY		 previousSymbol;
	UCHAR        resv3[64];
	char         primarkNumber[16];
} ATFI_FUNDAMENTAL_MMFUND, *LPATFI_FUNDAMENTAL_MMFUND, *PATFI_FUNDAMENTAL_MMFUND;

#define ATFID_FOREX_FLAGS						0
#define ATFID_FOREX_CUSIP						1
#define ATFID_FOREX_SEDOL						1
#define ATFID_FOREX_ISIN						1
#define ATFID_FOREX_NAME						2
#define ATFID_FOREX_DESCRIPTION					3
#define ATFID_FOREX_PRIMARKNUMBER				4

typedef struct _ATFI_FUNDAMENTAL_FOREX {
	ULONG   flags;
	union {	  
		char  cusip[16];
		char  sedol[16];
		char  isin[16];
	};		  
	char    name[40];
	char    description[52];
	UCHAR   resv[128];
	char    primarkNumber[16];
} ATFI_FUNDAMENTAL_FOREX, *LPATFI_FUNDAMENTAL_FOREX, *PATFI_FUNDAMENTAL_FOREX;

#define ATFID_FIXEDINCOME_FLAGS					0
#define ATFID_FIXEDINCOME_CUSIP					1
#define ATFID_FIXEDINCOME_SEDOL					1
#define ATFID_FIXEDINCOME_ISIN					1
#define ATFID_FIXEDINCOME_NAME					2
#define ATFID_FIXEDINCOME_DESCRIPTION			3
#define ATFID_FIXEDINCOME_PRIMARKNUMBER			4
#define ATFID_FIXEDINCOME_MATURITYDATE			5
#define ATFID_FIXEDINCOME_ISSUEDATE				6
#define ATFID_FIXEDINCOME_COUPONRATE			7
#define ATFID_FIXEDINCOME_COUPONPRICETYPE		8

typedef struct _ATFI_FUNDAMENTAL_FIXEDINCOME {
	ULONG        flags;
	union {	  
		char     cusip[16];
		char     sedol[16];
		char     isin[16];
	};		  
	char         name[40];
	char         description[52];
	AT_DATETIME  maturityDate;
	AT_DATETIME  issueDate;
	AT_PRICE     couponRate;
	UCHAR        couponPriceType;
	UCHAR        resv[115];
	char         primarkNumber[16];
} ATFI_FUNDAMENTAL_FIXEDINCOME, *LPATFI_FUNDAMENTAL_FIXEDINCOME, *PATFI_FUNDAMENTAL_FIXEDINCOME;

#define ATFID_DIVIDEND_FLAGS					0
#define ATFID_DIVIDEND_PRICETYPE				1
#define ATFID_DIVIDEND_FREQUENCYCODE			2
#define ATFID_DIVIDEND_SOURCECODE				3
#define ATFID_DIVIDEND_INDICATEDANNUALDIV		4
#define ATFID_DIVIDEND_INDICATEDANNUALDIVNOTES	5
#define ATFID_DIVIDEND_PREVCASHDIVDATE			6
#define ATFID_DIVIDEND_NEXTDIVDATE1				7
#define ATFID_DIVIDEND_NEXTDIVDATE2				8
#define ATFID_DIVIDEND_NEXTDIVDATE3				9
#define ATFID_DIVIDEND_NEXTDIVDATE4				10
#define ATFID_DIVIDEND_SPECIALDIVDATE			11
#define ATFID_DIVIDEND_PREVCASHDIVAMOUNT		12
#define ATFID_DIVIDEND_NEXTDIVAMOUNT1			13
#define ATFID_DIVIDEND_NEXTDIVAMOUNT2			14
#define ATFID_DIVIDEND_NEXTDIVAMOUNT3			15
#define ATFID_DIVIDEND_NEXTDIVAMOUNT4			16
#define ATFID_DIVIDEND_SPECIALDIVAMOUNT			17
#define ATFID_DIVIDEND_SPECIALDIVNOTES			18
#define ATFID_DIVIDEND_CASHDIVPAYDATE			19
#define ATFID_DIVIDEND_CASHDIVPAYDATENOTES		20
#define ATFID_DIVIDEND_CASHDIVRECORDDATE		21
#define ATFID_DIVIDEND_CASHDIVRECORDDATENOTES	22
#define ATFID_DIVIDEND_CASHDIVDECLAREDATE		23
#define ATFID_DIVIDEND_CASHDIVDECLAREDATENOTES	24
#define ATFID_DIVIDEND_COMMENT					25
#define ATFID_DIVIDEND_CURRENCY					26
#define ATFID_DIVIDEND_PREVDIVFOOTNOTE			27
#define ATFID_DIVIDEND_SPECIALDIVFOOTNOTE		28

typedef struct _ATFI_FUNDAMENTAL_DIVIDEND {
	ULONG        flags;
	UCHAR        priceType;
	UCHAR        frequencyCode;
	UCHAR        sourceCode;
	UCHAR        resv1;
	AT_PRICE     indicatedAnnualDiv;
	UCHAR        indicatedAnnualDivNotes[4];
	AT_DATETIME  prevCashDivDate;
	AT_DATETIME  nextDivDate1;
	AT_DATETIME  nextDivDate2;
	AT_DATETIME  nextDivDate3;
	AT_DATETIME  nextDivDate4;
	AT_DATETIME  specialDivDate;
	AT_PRICE     prevCashDivAmount;
	AT_PRICE     nextDivAmount1;
	AT_PRICE     nextDivAmount2;
	AT_PRICE     nextDivAmount3;
	AT_PRICE     nextDivAmount4;
	AT_PRICE     specialDivAmount;
	UCHAR        specialDivNotes[4];
	AT_DATETIME  cashDivPayDate;
	UCHAR        cashDivPayDateNotes[4];
	AT_DATETIME  cashDivRecordDate;
	UCHAR        cashDivRecordDateNotes[4];
	AT_DATETIME  cashDivDeclareDate;
	UCHAR        cashDivDeclareDateNotes[4];
	char         comment[80];
	char         currency[3];
	UCHAR        resv2;
	UCHAR        prevDivFootnote[4];
	UCHAR        specialDivFootnote[4];
	UCHAR        resv3[72];
} ATFI_FUNDAMENTAL_DIVIDEND, *LPATFI_FUNDAMENTAL_DIVIDEND, *PATFI_FUNDAMENTAL_DIVIDEND;

#define ATFID_SPLIT_FLAGS						0
#define ATFID_SPLIT_CURRENT						1
#define ATFID_SPLIT_PREVIOUS					2
#define ATFID_SPLIT_PRICETYPE					3
#define ATFID_SPLIT_SOURCECODE					4
#define ATFID_SPLIT_RATIO						5
#define ATFID_SPLIT_INDICATOR					6
#define ATFID_SPLIT_EXDATE						7
#define ATFID_SPLIT_FOOTNOTE					8
#define ATFID_SPLIT_SHARESAFTER					9
#define ATFID_SPLIT_SHARESBEFORE				10
#define ATFID_SPLIT_DECLAREDATE					11
#define ATFID_SPLIT_DECLAREDATEFOOTNOTE			12
#define ATFID_SPLIT_RECORDDATE					13
#define ATFID_SPLIT_RECORDDATEFOOTNOTE			14
#define ATFID_SPLIT_PAYMENTDATE					15
#define ATFID_SPLIT_PAYMENTDATEFOOTNOTE			16
#define ATFID_SPLIT_COMMENT						17
#define ATFID_SPLIT_PRICETYPE_PREV				18
#define ATFID_SPLIT_SOURCECODE_PREV				19
#define ATFID_SPLIT_RATIO_PREV					20
#define ATFID_SPLIT_INDICATOR_PREV				21
#define ATFID_SPLIT_EXDATE_PREV					22
#define ATFID_SPLIT_FOOTNOTE_PREV				23
#define ATFID_SPLIT_SHARESAFTER_PREV			24
#define ATFID_SPLIT_SHARESBEFORE_PREV			25
#define ATFID_SPLIT_DECLAREDATE_PREV			26
#define ATFID_SPLIT_DECLAREDATEFOOTNOTE_PREV	27
#define ATFID_SPLIT_RECORDDATE_PREV				28
#define ATFID_SPLIT_RECORDDATEFOOTNOTE_PREV		29
#define ATFID_SPLIT_PAYMENTDATE_PREV			30
#define ATFID_SPLIT_PAYMENTDATEFOOTNOTE_PREV	31
#define ATFID_SPLIT_COMMENT_PREV				32

typedef struct _ATFI_SPLIT {
	UCHAR        priceType;
	UCHAR        sourceCode;
	UCHAR        resv1[2];
	ULONG	     ratio;				/*  x 100,000  */
	UCHAR	     indicator;
	UCHAR	     resv2[3];
	AT_DATETIME	 exDate;
	UCHAR	     footnote[4];
	ULONG        sharesAfter;
	ULONG        sharesBefore;
	AT_DATETIME  declareDate;
	UCHAR	     declareDateFootnote[4];
	AT_DATETIME  recordDate;
	UCHAR	     recordDateFootnote[4];
	AT_DATETIME  paymentDate;
	UCHAR        paymentDateFootnote[4];
	char         comment[60];
} ATFI_SPLIT, *LPATFI_SPLIT, *PATFI_SPLIT;

typedef struct _ATFI_FUNDAMENTAL_SPLIT {
	ULONG       flags;
	ATFI_SPLIT  current;
	ATFI_SPLIT  previous;
	UCHAR       resv[28];
} ATFI_FUNDAMENTAL_SPLIT, *LPATFI_FUNDAMENTAL_SPLIT, *PATFI_FUNDAMENTAL_SPLIT;

#define ATFID_CALCULATED_FLAGS					0
#define ATFID_CALCULATED_PRICETYPE				1
#define ATFID_CALCULATED_HIGHDATE				2
#define ATFID_CALCULATED_HIGHPRICE52WEEK		3
#define ATFID_CALCULATED_LOWDATE				4
#define ATFID_CALCULATED_LOWPRICE52WEEK			5
#define ATFID_CALCULATED_MOVINGAVERAGE9DAY		6
#define ATFID_CALCULATED_MOVINGAVERAGE14DAY		7
#define ATFID_CALCULATED_MOVINGAVERAGE21DAY		8
#define ATFID_CALCULATED_MOVINGAVERAGE50DAY		9
#define ATFID_CALCULATED_MOVINGAVERAGE100DAY	10
#define ATFID_CALCULATED_MOVINGAVERAGE200DAY	11
#define ATFID_CALCULATED_CLOSEPRICEWEEK			12
#define ATFID_CALCULATED_CLOSEPRICEMONTH		13
#define ATFID_CALCULATED_CLOSEPRICEQUARTER		14
#define ATFID_CALCULATED_CLOSEPRICEYEAR			15
#define ATFID_CALCULATED_AVGDAILYVOLUME22DAY	16
#define ATFID_CALCULATED_AVGDAILYVOLUME100DAY	17
#define ATFID_CALCULATED_VOLATILITY20DAY		18
#define ATFID_CALCULATED_VOLATILITY6MONTH		19
#define ATFID_CALCULATED_LASTUPDATE				20
#define ATFID_CALCULATED_AVGDAILYVOLUME250DAY	21
#define ATFID_CALCULATED_BETA					22
#define ATFID_CALCULATED_YTDRATEOFRETURN		23
#define ATFID_CALCULATED_12MORATEOFRETURN		24
#define ATFID_CALCULATED_HIGHDATECALENDAR		25
#define ATFID_CALCULATED_HIGHPRICECALENDAR		26
#define ATFID_CALCULATED_LOWDATECALENDAR		27
#define ATFID_CALCULATED_LOWPRICECALENDAR		28

typedef struct _ATFI_FUNDAMENTAL_CALCULATED {
	ULONG        flags;
	UCHAR        priceType;
	UCHAR        resv1[3];
	AT_DATETIME  highDate;
	LONG         highPrice52Week;
	AT_DATETIME  lowDate;
	LONG         lowPrice52Week;
	LONG	     movingAverage9Day;
	LONG	     movingAverage14Day;
	LONG	     movingAverage21Day;
	LONG	     movingAverage50Day;
	LONG	     movingAverage100Day;
	LONG	     movingAverage200Day;
	LONG	     closePriceWeek;
	LONG	     closePriceMonth;
	LONG	     closePriceQuarter;
	LONG	     closePriceYear;
	ULONG	     avgDailyVolume22Day;	// priceType = 0
	ULONG	     avgDailyVolume100Day;	// priceType = 0
	ULONG	     volatility20Day;		// priceType = 4
	ULONG	     volatility6Month;		// priceType = 4
	AT_DATETIME  lastUpdate;
	ULONG	     avgDailyVolume250Day;	// priceType = 0
	LONG	     beta;					// priceType = 4
	LONG	     ytdRateOfReturn;		// priceType = 4
	LONG	     mo12RateOfReturn;		// priceType = 4
	AT_DATETIME  highDateCalendar;
	LONG         highPriceCalendar;
	AT_DATETIME  lowDateCalendar;
	LONG         lowPriceCalendar;
	UCHAR        resv[140];
} ATFI_FUNDAMENTAL_CALCULATED, *LPATFI_FUNDAMENTAL_CALCULATED, *PATFI_FUNDAMENTAL_CALCULATED;

#define ATFID_HISTORICAL_FLAGS					0
#define ATFID_HISTORICAL_PRICETYPE				1
#define ATFID_HISTORICAL_SOURCECODE				2
#define ATFID_HISTORICAL_VOLATILITY1MONTH		3
#define ATFID_HISTORICAL_VOLATILITY2MONTH		4
#define ATFID_HISTORICAL_VOLATILITY3MONTH		5
#define ATFID_HISTORICAL_VOLATILITY4MONTH		6
#define ATFID_HISTORICAL_VOLATILITY5MONTH		7
#define ATFID_HISTORICAL_VOLATILITY6MONTH		8
#define ATFID_HISTORICAL_VOLATILITY7MONTH		9
#define ATFID_HISTORICAL_VOLATILITY8MONTH		10
#define ATFID_HISTORICAL_VOLATILITY9MONTH		11
#define ATFID_HISTORICAL_VOLATILITY10MONTH		12
#define ATFID_HISTORICAL_VOLATILITY11MONTH		13
#define ATFID_HISTORICAL_VOLATILITY12MONTH		14
#define ATFID_HISTORICAL_LASTUPDATE				15
#define ATFID_HISTORICAL_CALLIMPVOLATILITY		16
#define ATFID_HISTORICAL_PUTIMPVOLATILITY		17

typedef struct _ATFI_FUNDAMENTAL_HISTORICAL {
	ULONG        flags;
	UCHAR        priceType;
	UCHAR        sourceCode;
	UCHAR        resv1[2];
	LONG         volatility1Month;
	LONG         volatility2Month;
	LONG         volatility3Month;
	LONG         volatility4Month;
	LONG         volatility5Month;
	LONG         volatility6Month;
	LONG         volatility7Month;
	LONG         volatility8Month;
	LONG         volatility9Month;
	LONG         volatility10Month;
	LONG         volatility11Month;
	LONG         volatility12Month;
	AT_DATETIME  lastUpdate;
	LONG         callImpVolatility;
	LONG         putImpVolatility;
	UCHAR        resv[188];
} ATFI_FUNDAMENTAL_HISTORICAL, *LPATFI_FUNDAMENTAL_HISTORICAL, *PATFI_FUNDAMENTAL_HISTORICAL;

#define ATFID_EARNINGS_FLAGS					0
#define ATFID_EARNINGS_SOURCECODE				1
#define ATFID_EARNINGS_FISCALYEAREND			2
#define ATFID_EARNINGS_LAST12MONTHEPSNOTE		3
#define ATFID_EARNINGS_LASTFISCALYEAREPSNOTE	4
#define ATFID_EARNINGS_LAST12MONTHEPS			5
#define ATFID_EARNINGS_PREVIOUS12MONTHEPS		6
#define ATFID_EARNINGS_LASTFISCALYEAREPS		7
#define ATFID_EARNINGS_PREVIOUSFISCALYEAREPS	8
#define ATFID_EARNINGS_LASTQUARTEREPS			9
#define ATFID_EARNINGS_PREVIOUSQUARTEREPS		10
#define ATFID_EARNINGS_ESTIMATEREPORTDATE		11
#define ATFID_EARNINGS_NUMBERREPORTINGBROKERS	12
#define ATFID_EARNINGS_CURQUARTERESTIMATEDEPS	13
#define ATFID_EARNINGS_NEXTQUARTERESTIMATEDEPS	14
#define ATFID_EARNINGS_CURRENTYEARESTIMATEDEPS	15
#define ATFID_EARNINGS_NEXTYEARESTIMATEDEPS		16
#define ATFID_EARNINGS_GROWTHRATE5YEAR			17
#define ATFID_EARNINGS_LASTQUARTERDATE			18
#define ATFID_EARNINGS_NEXTQUARTERDATE			19
#define ATFID_EARNINGS_QUARTER2EPS				20
#define ATFID_EARNINGS_QUARTER3EPS				21
#define ATFID_EARNINGS_QUARTER4EPS				22
#define ATFID_EARNINGS_QUARTER5EPS				23
#define ATFID_EARNINGS_QUARTER6EPS				24
#define ATFID_EARNINGS_QUARTER7EPS				25
#define ATFID_EARNINGS_NEXTREPORTDATE			26

typedef struct _ATFI_FUNDAMENTAL_EARNINGS {
	ULONG        flags;
	UCHAR	     sourceCode;
	UCHAR	     resv1[3];
	UCHAR        fiscalYearEnd[2];
	UCHAR	     last12MonthEPSFootnote;
	UCHAR	     lastFiscalYearEPSFootnote;
	LONG	     last12MonthEPS;
	LONG	     previous12MonthEPS;
	LONG	     lastFiscalYearEPS;
	LONG	     previousFiscalYearEPS;
	LONG	     lastQuarterEPS;
	LONG	     previousQuarterEPS;
	AT_DATETIME  estimateReportDate;
	UCHAR        resv2[2];
	USHORT	     numberReportingBrokers;
	LONG	     currentQuarterEstimatedEPS;
	LONG	     nextQuarterEstimatedEPS;
	LONG	     currentYearEstimatedEPS;
	LONG	     nextYearEstimatedEPS;
	LONG         growthRate5Year;
	AT_DATETIME  lastQuarterDate;
	AT_DATETIME  nextQuarterDate;
	LONG         quarter2EPS;
	LONG         quarter3EPS;
	LONG         quarter4EPS;
	LONG         quarter5EPS;
	LONG         quarter6EPS;
	LONG         quarter7EPS;
	AT_DATETIME  nextReportDate;
	UCHAR        resv3[156];
} ATFI_FUNDAMENTAL_EARNINGS, *LPATFI_FUNDAMENTAL_EARNINGS, *PATFI_FUNDAMENTAL_EARNINGS;

#define ATFID_INCOME_FLAGS					0
#define ATFID_INCOME_SOURCECODE				1
#define ATFID_INCOME_FLASHCURRENTVALUES		2
#define ATFID_INCOME_DATECURRPERIODEND		3
#define ATFID_INCOME_DATEMONTHEND			4
#define ATFID_INCOME_FREECASHFLOW			5
#define ATFID_INCOME_DEPRECIATIONEXPENSES	6
#define ATFID_INCOME_REVENUECURRENT			7
#define ATFID_INCOME_INCOMEBEFORETAX		8
#define ATFID_INCOME_INCOMEAVAILTOCOMMON	9
#define ATFID_INCOME_EPSBASIC				10
#define ATFID_INCOME_TOTCURRASSETSLASTQ		11
#define ATFID_INCOME_TOTCURRLIABSLASTQ		12
#define ATFID_INCOME_CURRENTRATIO			13
#define ATFID_INCOME_QUICKRATIO				14
#define ATFID_INCOME_LTDEBTEQLASTQ			15
#define ATFID_INCOME_TOTDEBTEQLASTQ			16
#define ATFID_INCOME_BOOKVALUEPERSHARE		17

typedef struct _ATFI_FUNDAMENTAL_INCOME {
	ULONG		 flags;
	UCHAR		 sourceCode;
	UCHAR		 flashCurrentValues;
	UCHAR		 resv1[2];
	AT_DATETIME	 dateCurrentPeriodEnd;
	AT_DATETIME	 dateMonthFlash;
	ULONG		 freeCashFlowPerShareTTM;
	ULONG		 depreciationExpenseTTM;
	ULONG		 revenueCurrentTTM;
	ULONG		 incomeBeforeTaxTTM;
	ULONG		 incomeAvailToCommonTTM;
	LONG		 epsBasicTTM;
	ULONG		 totalCurrentAssetsLatestQuarter;
	ULONG		 totalCurrentLiabilitiesLatestQuarter;
	ULONG		 currentRatioLatestQuarter;
	ULONG		 quickRatioLatestQuarter;
	ULONG		 LTdebtEquityLatestQuarter;
	ULONG		 totalDebtEquityLatestQuarter;
	ULONG		 bookValuePerShareLatestQuarter;
	UCHAR		 resv2[188];
} ATFI_FUNDAMENTAL_INCOME, *LPATFI_FUNDAMENTAL_INCOME, *PATFI_FUNDAMENTAL_INCOME;

#define ATFID_BALANCE_FLAGS						0
#define ATFID_BALANCE_SOURCECODE				1
#define ATFID_BALANCE_REPORTDATEINCOME			2
#define ATFID_BALANCE_REPORTDATEBALANCE			3
#define ATFID_BALANCE_REPORTDATECASH			4
#define ATFID_BALANCE_REVENUESALES				5
#define ATFID_BALANCE_GROSSPROFIT				6
#define ATFID_BALANCE_GENERALEXPENSES			7
#define ATFID_BALANCE_DEPRECIATIONDEPLETION		8
#define ATFID_BALANCE_INTERESTINCOME			9
#define ATFID_BALANCE_OTHERINCOME				10
#define ATFID_BALANCE_INCOMEBEFORETAX			11
#define ATFID_BALANCE_NETINCOME					12
#define ATFID_BALANCE_CURRENTASSETS				13
#define ATFID_BALANCE_LONGTERMINVESTMENTS		14
#define ATFID_BALANCE_PROPERTYPLANTEQUIPMENT	15
#define ATFID_BALANCE_TOTALASSETS				16
#define ATFID_BALANCE_CURRENTLIABILITIES		17
#define ATFID_BALANCE_LONGTERMDEBT				18
#define ATFID_BALANCE_CAPITALSTOCK				19
#define ATFID_BALANCE_ADDITIONALPAIDINCAPITAL	20
#define ATFID_BALANCE_RETAINEDEARNINGS			21
#define ATFID_BALANCE_TOTALCASHFROMOPERATING	22
#define ATFID_BALANCE_TOTALCASHFROMINVESTING	23
#define ATFID_BALANCE_TOTALCASHFROMFINANCING	24
#define ATFID_BALANCE_NETCHANGEINCASH			25
#define ATFID_BALANCE_LASTQUARTER				26
#define ATFID_BALANCE_QUARTERCOUNT				27

typedef struct _ATFI_FUNDAMENTAL_BALANCE_SHEET {
	ULONG		 flags;
	UCHAR		 sourceCode;
	UCHAR		 resv1[3];
	AT_DATETIME	 reportDateIncome;
	AT_DATETIME	 reportDateBalance;
	AT_DATETIME	 reportDateCash;
	LONG		 revenueSales;
	LONG		 grossProfit;
	LONG		 generalExpenses;
	LONG		 depreciationDepletion;
	LONG		 interestIncome;
	LONG		 otherIncome;
	LONG		 incomeBeforeTax;
	LONG		 netIncome;
	LONG		 currentAssets;
	LONG		 longTermInvestments;
	LONG		 propertyPlantEquipment;
	LONG		 totalAssets;
	LONG		 currentLiabilities;
	LONG		 longTermDebt;
	LONG		 capitalStock;
	LONG		 additionalPaidinCapital;
	LONG		 retainedEarnings;
	LONG		 totalCashFromOperating;
	LONG		 totalCashFromInvesting;
	LONG		 totalCashFromFinancing;
	LONG		 netChangeInCash;
	char         lastQuarter[4];
	UCHAR        quarterCount;
	UCHAR		 resv2[147];
} ATFI_FUNDAMENTAL_BALANCE_SHEET, *LPATFI_FUNDAMENTAL_BALANCE_SHEET, *PATFI_FUNDAMENTAL_BALANCE_SHEET;

#define ATFID_CHANGE_FLAGS						0
#define ATFID_CHANGE_OLDCUSIPNUMBER				1
#define ATFID_CHANGE_OLDKEY						2
#define ATFID_CHANGE_OLDNAME					3

typedef struct _ATFI_FUNDAMENTAL_CHANGE {
	ULONG   flags;
	char    oldCusipNumber[16];				// previous CUSIP
	AT_KEY  oldKey;							// previous symbol
	char    oldName[40];
	UCHAR   resv2[176];
} ATFI_FUNDAMENTAL_CHANGE, *LPATFI_FUNDAMENTAL_CHANGE, *PATFI_FUNDAMENTAL_CHANGE;

#define ATFID_ADDRESS_FLAGS			0
#define ATFID_ADDRESS_SOURCECODE	1
#define ATFID_ADDRESS_ADDRESS1		2
#define ATFID_ADDRESS_ADDRESS2		3
#define ATFID_ADDRESS_CITY			4
#define ATFID_ADDRESS_STATE			5
#define ATFID_ADDRESS_ZIPCODE		6
#define ATFID_ADDRESS_PHONENUMBER	7
#define ATFID_ADDRESS_FAXNUMBER		8
#define ATFID_ADDRESS_INETADDR		9
#define ATFID_ADDRESS_IRNAME		10
#define ATFID_ADDRESS_IRADDRESS		11
#define ATFID_ADDRESS_IRPHONE		12
#define ATFID_ADDRESS_ISOCOUNTRY	13

typedef struct _ATFI_FUNDAMENTAL_ADDRESS {
	ULONG	flags;
	UCHAR	sourceCode;
	UCHAR	resv1;
	UCHAR	isoCountry[2];
	char	address1[32];
	char	address2[32];
	char	city[16];
	char	state[16];
	char	zipCode[8];
	char	phoneNumber[16];
	char	faxNumber[16];
	char	internetAddress[32];
	char	irName[32];
	char	irAddress[32];
	char	irPhone[16];
} ATFI_FUNDAMENTAL_ADDRESS, *LPATFI_FUNDAMENTAL_ADDRESS, *PATFI_FUNDAMENTAL_ADDRESS;

#define ATFID_DESCRIPTION_FLAGS			0
#define ATFID_DESCRIPTION_SOURCECODE	1
#define ATFID_DESCRIPTION_TEXT			2

typedef struct _ATFI_FUNDAMENTAL_DESCRIPTION {
	ULONG	flags;
	UCHAR	sourceCode;
	UCHAR	resv1[3];
	char	text[248];
} ATFI_FUNDAMENTAL_DESCRIPTION, *LPATFI_FUNDAMENTAL_DESCRIPTION, *PATFI_FUNDAMENTAL_DESCRIPTION;

#define ATFID_POSTPANEL_FLAGS				0
#define ATFID_POSTPANEL_SOURCECODE			1
#define ATFID_POSTPANEL_POST				2
#define ATFID_POSTPANEL_PANEL				3
#define ATFID_POSTPANEL_SPECIALIST			4
#define ATFID_POSTPANEL_SPECIALISTNUMBER	5
#define ATFID_POSTPANEL_CLEARING			6
#define ATFID_POSTPANEL_TRADINGUNIT			7

typedef struct _ATFI_FUNDAMENTAL_POSTPANEL {
	ULONG	flags;
	UCHAR	sourceCode;
	UCHAR	resv1[3];
	char    post[4];
	char    panel[4];
	char    specialist[64];
	char    specialistNumber[8];
	char    clearing[8];
	ULONG   tradingUnit;
	char	resv2[156];
} ATFI_FUNDAMENTAL_POSTPANEL, *LPATFI_FUNDAMENTAL_POSTPANEL, *PATFI_FUNDAMENTAL_POSTPANEL;

#define ATFID_EQUITY2_FLAGS				0
#define ATFID_EQUITY2_SOURCECODE		1
#define ATFID_EQUITY2_FULLCOVERAGE		2
#define ATFID_EQUITY2_COVERAGEDATE		3
#define ATFID_EQUITY2_EPSDATEQ1			4
#define ATFID_EQUITY2_EPSDATEQ2			5
#define ATFID_EQUITY2_EPSDATEQ3			6
#define ATFID_EQUITY2_EPSDATEQ4			7
#define ATFID_EQUITY2_VALOR				8
#define ATFID_EQUITY2_WSNUMBER			9
#define ATFID_EQUITY2_DJTICKER			10
#define ATFID_EQUITY2_IBESNUMBER		11
#define ATFID_EQUITY2_INACTIVEDATE		12
#define ATFID_EQUITY2_DISCLOSURENUM		13
#define ATFID_EQUITY2_SEGMENT			14
#define ATFID_EQUITY2_INDEXRAW			15
#define ATFID_EQUITY2_CURRENCYNAME		16
#define ATFID_EQUITY2_ADR				17

typedef struct _ATFI_FUNDAMENTAL_EQUITY2 {
	ULONG	        flags;
	UCHAR	        sourceCode;
	char	        fullCoverageFlag;	// Indicates that WS has full coverage of this company (07033)
	char			isADR;				// 'X' means yes.
	char	        resv;
	AT_DATETIME		coverageDate;		// Date of first coverage (11516)
	AT_DATETIME		epsDateQ1;			// EPS Reporting date for Q1 (05901)
	AT_DATETIME		epsDateQ2;			// EPS Reporting date for Q2 (05902)
	AT_DATETIME		epsDateQ3;			// EPS Reporting date for Q3 (05903)
	AT_DATETIME		epsDateQ4;			// EPS Reporting date for Q4 (05904)
	char			valor[16];			// Valor number (06007)
	char			wsNumber[16];		// WorldScope Company Number (06035)
	char			djTicker[8];		// Dow Jones Ticker Symbol (06040)
	char			ibesNumber[16];		// I/B/E/S Company number (06038)
	AT_DATETIME		inactiveDate;		// Date of cessation of activity (if inactive) (07015)
	char			disclosureNum[16];	// Disclosure Company Number (06038)
	char			industrySegment[32];// Worldscope Industry Group (06011)
	union	{
		BYTE		indexRaw[16];		// Index membership (05661)
		struct {
			BYTE		sp500:1;			//	S & P 500 - US
			BYTE		djComposite:1;		// Dow Jones Composite - US
			BYTE		spWorld:1;			// FT/ S & P World
			BYTE		resv:1;				// Unused
			BYTE		ftse:1;				// FTSE 100 - UK
			BYTE		nikkei:1;			// Nikkei 225 - Japan
			BYTE		tse:1;				// TSE 300 - Canada
			BYTE		allOrdinaries:1;	// All Ordinaries - Australia
			BYTE		creditanStalt:1;	// CreditanStalt - Austria
			BYTE		bse:1;				// Brussels Stock Exchange - Belgium
			BYTE		kfx:1;				// KFX Index - Denmark
			BYTE		hex:1;				// HEX Index - Finland
			BYTE		cac40:1;			// CAC 40 - France
			BYTE		fazAktien:1;		// Faz Aktien - Germany
			BYTE		hangSeng:1;			// Hang Seng - Hong Kong
			BYTE		iseq:1;				// ISEQ Overall Index - Ireland
			BYTE		bancaCommerical:1;	// Banca Commercial Italiana - Italy
			BYTE		newStraitsTimes:1;	// New Straits Times Index - Malaysia
			BYTE		cbs:1;				// CBS All Share - Netherlands
			BYTE		nzse:1;				// NZSE 40 - New Zealand
			BYTE		totalShareComp:1;	// Total Shares Composite Index - Norway
			BYTE		straitTimes:1;		// Strait Times Industrial - Singapore
			BYTE		jse:1;				// JSE Overall Index - South Africa
			BYTE		madrid:1;			// Madrid Stock Exchange - Spain
			BYTE		affarsvarlden:1;	// Affarsvarlden - Sweden
			BYTE		sbc:1;				// SBC Index - Switzerland
			BYTE		allShare:1;			// All Share Index - UK
			BYTE		merval:1;			// MERVAL - Argentina
			BYTE		bovespa:1;			// BOVESPA - Brazil
			BYTE		ipsa:1;				// IPSA - Chile
			BYTE		ibb:1;				// IBB - Colombia
			BYTE		aseIndex:1;			// ASE Index - Greece
			BYTE		bseSensitive:1;		// BSE Sensitive Index - India
			BYTE		ihsg:1;				// IHSG Index - Indonesia
			BYTE		topix:1;			// TOPIX - Japan
			BYTE		luxembourgPrice:1;	// Luxembourg Price Index - Luxembourg
			BYTE		ipcIndex:1;			// IPC Index - Mexico
			BYTE		kse100:1;			// KSE 100 Index - Pakistan
			BYTE		igbvlIndex:1;		// I.G.B.V.L. Index - Peru
			BYTE		philippineSECI:1;	// Philippine Stock Exchange Composite Index - Philippine
			BYTE		btaIndex:1;			// BT&A Index - Portugal
			BYTE		kcspiIndex:1;		// KCSPI Index - South Korea
			BYTE		taiwanWeighted:1;	// Taiwan Weighted Stock Index - Taiwan
			BYTE		bangkokSET:1;		// Bangkok SET Index - Thailand
			BYTE		IstanbulSEI:1;		// Istanbul Stock Exchange Index
			BYTE		ftseMid250:1;		// FTSE Mid 250 - UK
			BYTE		ftseMid350:1;		// FTSE Mid 350 - UK
			BYTE		merinvest:1;		// Merinvest - Venezuela
			BYTE		dax:1;				// DAX - Germany
			BYTE		spSmallCap:1;		// S&P Small Cap - US
			BYTE		ifcComposite:1;		// IFC Composite Index
			BYTE		ifcInvestable:1;	// IFC Investable Index
			BYTE		sbf250:1;			// SBF 250 - France
			BYTE		djGlobalIndex:1;	// DJ Global Index
			BYTE		djSTOXX:1;			// DJ STOXX
			BYTE		djEuroSTOXX:1;		// DJ Euro STOXX
			BYTE		djSTOXX50:1;		// DJ STOXX 50
			BYTE		djEuroSTOXX50:1;	// DJ Euro STOXX 50
			BYTE		dax100:1;			// DAX 100 - Germany
			BYTE		sbf120:1;			// SBF 120 - France
			BYTE		resv1:3;
		} bits;
	};
	char			currencyName[32];	// Currency Name (06099)
	char			resv1[72];
} ATFI_FUNDAMENTAL_EQUITY2, *LPATFI_FUNDAMENTAL_EQUITY2, *PATFI_FUNDAMENTAL_EQUITY2;

#define ATFID_DIVIDEND2_FLAGS			0
#define ATFID_DIVIDEND2_SOURCECODE		1
#define ATFID_DIVIDEND2_DIVDATEQ1		2	
#define ATFID_DIVIDEND2_DIVDATEQ2		3
#define ATFID_DIVIDEND2_DIVDATEQ3		4
#define ATFID_DIVIDEND2_DIVDATEQ4		5
#define ATFID_DIVIDEND2_EXDIVQ1			6
#define ATFID_DIVIDEND2_EXDIVQ2			7
#define ATFID_DIVIDEND2_EXDIVQ3			8
#define ATFID_DIVIDEND2_EXDIVQ4			9
#define ATFID_DIVIDEND2_DIVIDENDQ1		10
#define ATFID_DIVIDEND2_DIVIDENDQ2		11
#define ATFID_DIVIDEND2_DIVIDENDQ3		12
#define ATFID_DIVIDEND2_DIVIDENDQ4		13
#define ATFID_DIVIDEND2_SPECDIVQ1		14
#define ATFID_DIVIDEND2_SPECDIVQ2		15
#define ATFID_DIVIDEND2_SPECDIVQ3		16
#define ATFID_DIVIDEND2_SPECDIVQ4		17
#define ATFID_DIVIDEND2_PREFDIV			18

typedef struct _ATFI_FUNDAMENTAL_DIVIDEND2 {
	ULONG			flags;
	UCHAR			sourceCode;
	char			resv[3];
	AT_DATETIME		divDateQ1;			// Div. Declare date for Q1 (05910)
	AT_DATETIME		divDateQ2;			// Div. Declare date for Q2 (05911)
	AT_DATETIME		divDateQ3;			// Div. Declare date for Q2 (05912)
	AT_DATETIME		divDateQ4;			// Div. Declare date for Q4 (05913)
	AT_DATETIME		exDivDateQ1;		// Ex. Div. date for Q1 (05923)
	AT_DATETIME		exDivDateQ2;		// Ex. Div. date for Q2 (05924)
	AT_DATETIME		exDivDateQ3;		// Ex. Div. date for Q3 (05925)
	AT_DATETIME		exDivDateQ4;		// Ex. Div. date for Q4 (05926)
	AT_PRICE		dividendQ1;			// Dividend amount for Q1, 2 decimals (05121)
	AT_PRICE		dividendQ2;			// Dividend amount for Q2, 2 decimals (05122)
	AT_PRICE		dividendQ3;			// Dividend amount for Q3, 2 decimals (05123)
	AT_PRICE		dividendQ4;			// Dividend amount for Q4, 2 decimals (05124)
	AT_PRICE		specDivQ1;			// Special Dividend for Q1, 2 decimals (05155)
	AT_PRICE		specDivQ2;			// Special Dividend for Q2, 2 decimals (05156)
	AT_PRICE		specDivQ3;			// Special Dividend for Q3, 2 decimals (05157)
	AT_PRICE		specDivQ4;			// Special Dividend for Q4, 2 decimals (05158)
	AT_PRICE		preferredDividend;	// (01701)
	char			resv1[180];
} ATFI_FUNDAMENTAL_DIVIDEND2, *LPATFI_FUNDAMENTAL_DIVIDEND2, *PATFI_FUNDAMENTAL_DIVIDEND2;


#define ATFID_SEGMENTS_FLAGS			0
#define ATFID_SEGMENTS_SOURCECODE		1
#define ATFID_SEGMENTS_PRODUCT1			2
#define ATFID_SEGMENTS_PRODUCT2			3
#define ATFID_SEGMENTS_PRODUCT3			4
#define ATFID_SEGMENTS_PRODUCT4			5
#define ATFID_SEGMENTS_PRODUCT5			6
#define ATFID_SEGMENTS_PRODUCT6			7
#define ATFID_SEGMENTS_PRODUCT7			8
#define ATFID_SEGMENTS_PRODUCT8			9
#define ATFID_SEGMENTS_PRODUCT9			10
#define ATFID_SEGMENTS_PRODUCT10		11
#define ATFID_SEGMENTS_GEOGRAPHIC1		12
#define ATFID_SEGMENTS_GEOGRAPHIC2		13
#define ATFID_SEGMENTS_GEOGRAPHIC3		14
#define ATFID_SEGMENTS_GEOGRAPHIC4		15
#define ATFID_SEGMENTS_GEOGRAPHIC5		16
#define ATFID_SEGMENTS_GEOGRAPHIC6		17
#define ATFID_SEGMENTS_GEOGRAPHIC7		18
#define ATFID_SEGMENTS_GEOGRAPHIC8		19
#define ATFID_SEGMENTS_GEOGRAPHIC9		20
#define ATFID_SEGMENTS_GEOGRAPHIC10		21

typedef struct _ATFI_FUNDAMENTAL_SEGMENTS {
	ULONG	flags;
	UCHAR	sourceCode;
	char	resv[3];
	WORD	productSegment[10];				// Product Segment SIC's (19506, 19516... 19596)
	char	geographicSegment[10][16];		// Product Geographic Desc. (19600, 19610...19690)
	char	resv1[68];
} ATFI_FUNDAMENTAL_SEGMENTS, *LPATFI_FUNDAMENTAL_SEGMENTS, *PATFI_FUNDAMENTAL_SEGMENTS;


#define ATFID_EARNINGS2_FLAGS			0
#define ATFID_EARNINGS2_SOURCECODE		1
#define ATFID_EARNINGS2_ONEMONTHRET		2
#define ATFID_EARNINGS2_THREEMONTHRET	3
#define ATFID_EARNINGS2_ONEYEARRET		4
#define ATFID_EARNINGS2_THREEYEARRET	5
#define ATFID_EARNINGS2_MARKETCAP		6
#define ATFID_EARNINGS2_BETAVINDEX		7
#define ATFID_EARNINGS2_PERATIO			8
#define ATFID_EARNINGS2_AVGANNUALPE		9
#define ATFID_EARNINGS2_EARNINGSYIELD	10
#define ATFID_EARNINGS2_DIVYIELD		11
#define ATFID_EARNINGS2_PRICETOBOOK		12
#define ATFID_EARNINGS2_PRICETOSALES	13
#define ATFID_EARNINGS2_PRICETOCASH		14
#define ATFID_EARNINGS2_DIVPERSHARE		15
#define ATFID_EARNINGS2_EPSBEFOREEXTRA	16
#define ATFID_EARNINGS2_EPSAFTEREXTRA	17
#define ATFID_EARNINGS2_SALESPERSHARE	18
#define ATFID_EARNINGS2_CASHPERSHARE	19
#define ATFID_EARNINGS2_MARKETCAPCHANGE	20
#define ATFID_EARNINGS2_TYMARKETCAP		21
#define ATFID_EARNINGS2_SALESCHANGE		22
#define ATFID_EARNINGS2_TYSALES			23
#define ATFID_EARNINGS2_NETINCOMECHANGE	24
#define ATFID_EARNINGS2_TYNETINCOME		25
#define ATFID_EARNINGS2_EMPLOYEECHANGE	26
#define ATFID_EARNINGS2_ASSETCHANGE		27
#define ATFID_EARNINGS2_EPSCHANGE		28
#define ATFID_EARNINGS2_TYEPSCHANGE		29
#define ATFID_EARNINGS2_DPSCHANGE		30
#define ATFID_EARNINGS2_TYDPSCHANGE		31
#define ATFID_EARNINGS2_OPERMARGIN		32
#define ATFID_EARNINGS2_PRETAXMARGIN	33
#define ATFID_EARNINGS2_RETURNONASSETS	34
#define ATFID_EARNINGS2_RETURNONEQUITY	35
#define ATFID_EARNINGS2_RETURNONCAPITAL	36
#define ATFID_EARNINGS2_ASSETTURNOVER	37
#define ATFID_EARNINGS2_FIXEDCOSTCHANGE	38
#define ATFID_EARNINGS2_FIXEDTOWORTH	39
#define ATFID_EARNINGS2_REVPEREMPLOYEE	40
#define ATFID_EARNINGS2_DEBTTOWORTH		41
#define ATFID_EARNINGS2_EBITTOTIE		42

typedef struct _ATFI_FUNDAMENTAL_EARNINGS2 {
	ULONG		flags;
	UCHAR		sourceCode;
	char		resv[3];
	LONG		oneMonTotRetPercent;		// 2 implied decimal places (08807)
	LONG		threeMonTotRetPercent;		// 2 implied decimal places (08808)
	LONG		oneYearTotRetPercent;		// 2 implied decimal places (08801)
	LONG		threeYearTotRetPercent;		// 2 implied decimal places (O8803)
	ULONG		marketCap;					// (08001)
	LONG		betaVsIndex;				// Beta vs. Local Index (2 places) (09802)
	LONG		peRatio;					// P/E Ratio (2 places) (09102)
	LONG		avgAnnualPE;				// Average Annual P/E Ratio (2 places) (09106)
	LONG		earningsYield;				// (09202)
	LONG		divYield;					// (09402)
	LONG		priceToBook;				// Price/Book ratio (2 places) (09302)
	LONG		priceToSales;				// Price/Sales ratio (2 places) (09904)
	LONG		priceToCashFlow;			// Price/Cash Flow ratio (2 places) (09602)
	LONG		divPerShare;				// Dividends per share (05101)
	DWORD		epsBeforeExtra;				// EPS Before Extraordinary Items (2 places) (05201)
	DWORD		epsAfterExtra;				// EPS After Extraordinary Items (2 places) (05230)
	LONG		salesPerShare;				// Sales per share (05508)
	LONG		cashFlowPerShare;			// Cash flow per share (2 decimals) (05501)
	LONG		marketCapChangePerYear;		// (08579)
	LONG		threeYearMarketCapGrowth;	// Three year % market cap growth (08581)
	LONG		salesChangePerYear;			// (08631)
	LONG		threeYearSalesGrowth;		// Three year % sales growth (08633)
	LONG		netIncomeChangePerYear;		// (08636)
	LONG		threeYearNetIncomeGrowth;	// 3 year % net income growth (08638)
	LONG		employeeChangePerYear;		// (08626)
	LONG		assetChangePerYear;			// (08621)
	LONG		epsChangePerYear;			// (08601)
	LONG		threeYearEPSGrowth;			// Three year % EPS growth (08604)
	LONG		dpsChangePerYear;			// (08611)
	LONG		threeYearDPSGrowth;			// Three year % DPS growth (08614)
	LONG		operatingMargin;			// (08316)
	LONG		pretaxMargin;				// (08321)
	LONG		returnOnAssets;				// (08326)
	LONG		returnOnEquity;				// (08371)
	LONG		returnOnCapital;			// (08376)
	LONG		assetTurnover;				// (08401)
	LONG		fixedCostChange;			// Fixed cost change coverage (08251)
	LONG		fixedToWorth;				// Fixed/Worth (08266)
	DWORD		revenuePerEmployee;			// (in 000's) (08351)
	LONG		debtToWorth;				// Debt/Worth (08236)
	LONG		ebitToTotalIntExpense;		// Earnings Before Income Tax / Total Interest Expense (08291)
	char		resv1[84];
} ATFI_FUNDAMENTAL_EARNINGS2, *LPATFI_FUNDAMENTAL_EARNINGS2, *PATFI_FUNDAMENTAL_EARNINGS2;

#define ATFID_FINANCIALS_FLAGS			0
#define ATFID_FINANCIALS_SOURCECODE		1
#define ATFID_FINANCIALS_COSTOFGOODS	2
#define ATFID_FINANCIALS_OPERATINGEXP	3
#define ATFID_FINANCIALS_INTERESTEXP	4
#define ATFID_FINANCIALS_INCTAXEXP		5
#define ATFID_FINANCIALS_INCBEFOREEXTRA	6
#define ATFID_FINANCIALS_GROSSFIXED		7
#define ATFID_FINANCIALS_ACCUMDEPREC	8
#define ATFID_FINANCIALS_LTRECEIVABLES	9
#define ATFID_FINANCIALS_INVENTORY		10
#define ATFID_FINANCIALS_CASHANDEQUIV	11
#define ATFID_FINANCIALS_PREFEQUITY		12
#define ATFID_FINANCIALS_MINORITYINT	13
#define ATFID_FINANCIALS_DVFAEPS		14
#define ATFID_FINANCIALS_OVFAEPS		15
#define ATFID_FINANCIALS_WSHEPS			16
#define ATFID_FINANCIALS_PROVFORLOSS	17
#define ATFID_FINANCIALS_STLIAB			18
#define ATFID_FINANCIALS_ACCOUNTSPAY	19
#define ATFID_FINANCIALS_TOTLIAB		20
#define ATFID_FINANCIALS_TOTLIABEQUITY	21
#define ATFID_FINANCIALS_PREMEARNED		22
#define ATFID_FINANCIALS_INTERESTINC	23
#define ATFID_FINANCIALS_OPERINCOME		24
#define ATFID_FINANCIALS_ACCOUNTSREC	25

typedef struct _ATFI_FUNDAMENTAL_FINANCIALS {
	ULONG	flags;
	UCHAR	sourceCode;
	char	resv[3];
	DWORD	costOfGoodsSold;				// (in 000's) (01051)
	DWORD	operatingExpenses;				// (in 000's) (01249)
	DWORD	interestExpense;				// (01251)
	DWORD	incomeTaxExpense;				// (01451)
	DWORD	incomeBeforeExtra;				// Income before extraordinary items (in 000's) (01551)
	DWORD	grossFixedAssets;				// (in 000's) (02301)
	DWORD	accumulatedDepreciation;		// (in 000's) (02401)
	DWORD	ltreceivables;					// Long term receivables and investments (in 000's) (02258)
	DWORD	inventory;						// (in 000's) (02101)
	DWORD	cashAndEquiv;					// (in 000's) (02001)
	DWORD	preferredEquity;				// (in 000's) (03451)
	DWORD	minorityInterest;				// (in 000's) (03426)
	DWORD	dvfaEPS;						// DVFA EPS (Germany) (05240)
	DWORD	ovfaEPS;						// OVFA EPS (Austria) (05203)
	DWORD	wsHeadlineEPS;					// Worldscope Headline EPS (UK) (05725)
	DWORD	provisionForLoanLoss;			// Note:  Banks only (in 000's) (01271)
	DWORD	stLiabilities;					// Short Term Liabilities (in 000's) (03051)
	DWORD	accountsPayable;				// (in 000's) (03040)
	DWORD	totalLiabilities;				// (in 000's) (03351)
	DWORD	totalLiabilitesAndEquity;		// (in 000's) (03999)
	DWORD	premiumsEarned;					// (in 000's) (01002)
	DWORD	interestIncome;					// Banks only.  (In 000's) (01016)
	DWORD	operatingIncome;					// (in 000's) (01250)
	DWORD	accountsReceivable;				// (in 000's) (02051)
	char	resv1[156];						
} ATFI_FUNDAMENTAL_FINANCIALS, *LPATFI_FUNDAMENTAL_FINANCIALS, *PATFI_FUNDAMENTAL_FINANCIALS;


typedef struct _ATFI_FUNDAMENTAL {
	ULONG   dataType;
	AT_KEY  key;
	ULONG   recordType;
	WORD    permission;
	BYTE    resv[10];
	union {
		UCHAR                           data[256];
		ATFI_FUNDAMENTAL_BASE           base;
		ATFI_FUNDAMENTAL_EQUITY         equity;
		ATFI_FUNDAMENTAL_EQUITY2        equity2;
		ATFI_FUNDAMENTAL_DIVIDEND       dividend;
		ATFI_FUNDAMENTAL_DIVIDEND2		dividend2;
		ATFI_FUNDAMENTAL_SPLIT          split;
		ATFI_FUNDAMENTAL_CALCULATED     calculated;
		ATFI_FUNDAMENTAL_HISTORICAL     historical;
		ATFI_FUNDAMENTAL_EARNINGS       earnings;
		ATFI_FUNDAMENTAL_EARNINGS2		earnings2;
		ATFI_FUNDAMENTAL_INCOME         income;
		ATFI_FUNDAMENTAL_BALANCE_SHEET  balanceSheet;
		ATFI_FUNDAMENTAL_CHANGE         change;
		ATFI_FUNDAMENTAL_FUTURE         future;
		ATFI_FUNDAMENTAL_BOND           bond;
		ATFI_FUNDAMENTAL_INDEX          index;
		ATFI_FUNDAMENTAL_MUTUALFUND     mutualFund;
		ATFI_FUNDAMENTAL_MMFUND         mmFund;
		ATFI_FUNDAMENTAL_OPTION         option;
		ATFI_FUNDAMENTAL_FUTUREOPTION   futureOption;
		ATFI_FUNDAMENTAL_FOREX          forex;
		ATFI_FUNDAMENTAL_FIXEDINCOME    fixedIncome;
		ATFI_EQUITY_FP                  equityFP;
		ATFI_FUNDAMENTAL_ADDRESS	    address;
		ATFI_FUNDAMENTAL_DESCRIPTION    description;
		ATFI_FUNDAMENTAL_POSTPANEL      postPanel;
		ATFI_FUNDAMENTAL_SEGMENTS		segments;
		ATFI_FUNDAMENTAL_FINANCIALS		financials;
	};
} ATFI_FUNDAMENTAL, *PATFI_FUNDAMENTAL, *LPATFI_FUNDAMENTAL;

// A-T Option databases

#define ATFI_OPTION_CNTTYPE_AMERICAN	0
#define ATFI_OPTION_CNTTYPE_EUROPEAN	1
#define ATFI_OPTION_CNTTYPE_ADJUSTMENT	2
#define ATFI_OPTION_CNTTYPE_SPINOFF		3
#define ATFI_OPTION_CNTTYPE_MERGER		4
#define ATFI_OPTION_CNTTYPE_PERCENTAGE	5

typedef struct _ATFI_OPTION_KEY0 {
	char  optionSymbol[4];
} ATFI_OPTION_KEY0, *PATFI_OPTION_KEY0, *LPATFI_OPTION_KEY0;

typedef struct _ATFI_OPTION_KEY1 {
	char   underlyingSymbol[6];
	DWORD  resv1;
	BYTE   resv2[2];
} ATFI_OPTION_KEY1, *PATFI_OPTION_KEY1, *LPATFI_OPTION_KEY1;

typedef struct _ATFI_OPTION_KEY2 {
	char  optionSymbol[4];
	char  underlyingSymbol[6];
	BYTE  resv2[2];
} ATFI_OPTION_KEY2, *PATFI_OPTION_KEY2, *LPATFI_OPTION_KEY2;

typedef struct _ATFI_OPTION {
	char         optionSymbol[4];
	char         underlyingSymbol[6];
	BYTE         optionType;
	BYTE         strikePriceType;
	union {
		DWORD        flags;
		struct {
			DWORD	addByChange		 		:1;
			DWORD	reserved1				:3;
			DWORD	contractType			:4;
			DWORD	sharesPerContractMult	:3;	// Power of 10.
			DWORD	reserved2				:4;
			DWORD	deleteFlag				:1;
			DWORD	reserved3				:8;
			DWORD	version					:8;
		};
	};
	AT_DATETIME  expirationDates[12];
	AT_PRICE     strikePrice[26];
	WORD         sharesPerContract;
	char         primaryExchange;
	BYTE         resv2;
	char         underlyingSymbol1[6];
	WORD         sharesPerContract1;
	char         underlyingSymbol2[6];
	WORD         sharesPerContract2;
	char         underlyingSymbol3[6];
	WORD         sharesPerContract3;
	AT_PRICE     cash;
	WORD         contractMultiplier;
	BYTE         resv3[2];
} ATFI_OPTION, *PATFI_OPTION, *LPATFI_OPTION;

// A-T Index databases

typedef struct _ATFI_INDEX_KEY0 {
	AT_KEY  indexSymbol;
	DWORD   resv;
} ATFI_INDEX_KEY0, *LPATFI_INDEX_KEY0, *PATFI_INDEX_KEY0;

typedef struct _ATFI_INDEX_KEY1 {
	DWORD   dataType;
	AT_KEY  underlyingSymbol;
	DWORD   resv;
} ATFI_INDEX_KEY1, *LPATFI_INDEX_KEY1, *PATFI_INDEX_KEY1;

typedef struct _ATFI_INDEX_KEY2 {
	AT_KEY  indexSymbol;
	DWORD   dataType;
	AT_KEY  underlyingSymbol;
} ATFI_INDEX_KEY2, *LPATFI_INDEX_KEY2, *PATFI_INDEX_KEY2;

typedef struct _ATFI_INDEX {
	AT_KEY       indexSymbol;
	DWORD        dataType;
	AT_KEY       underlyingSymbol;
	double       factor;
	AT_DATETIME  updateTime;
	DWORD        flags;
	BYTE         resv[12];
} ATFI_INDEX, *LPATFI_INDEX, *PATFI_INDEX;

// A-T News Source database

// Upper 4 bits of sourceNum used to specify origin

#define	NSTYPE_MASK		0xf0000000			// ATFI:0, DeskTop:1 
											// (see type flags in atfi.h for QSM_NEWS_SOURCE)

typedef struct _ATFI_NEWS {
	DWORD  sourceNum;
	char   source[4];
	char   description[80];
	BYTE   resv[16];
} ATFI_NEWS, *PATFI_NEWS, *LPATFI_NEWS;

// A-T Symbol Guide databases

typedef struct _ATFI_SYMBOL_KEY0 {
	AT_KEY  ticker;
} ATFI_SYMBOL_KEY0, *LPATFI_SYMBOL_KEY0, *PATFI_SYMBOL_KEY0;

typedef struct _ATFI_SYMBOL_KEY1 {
	char    companyName[40];
	DWORD   resv;
} ATFI_SYMBOL_KEY1, *LPATFI_SYMBOL_KEY1, *PATFI_SYMBOL_KEY1;

typedef struct _ATFI_SYMBOL_KEY2 {
	char    cusip[16];
	DWORD   resv;
} ATFI_SYMBOL_KEY2, *LPATFI_SYMBOL_KEY2, *PATFI_SYMBOL_KEY2;

typedef struct _ATFI_SYMBOL_KEY3 {
	char    primarkNumber[16];
	DWORD   resv;
} ATFI_SYMBOL_KEY3, *LPATFI_SYMBOL_KEY3, *PATFI_SYMBOL_KEY3;

typedef struct _ATFI_SYMBOL {
	AT_KEY  ticker;
	char    companyName[40];
	char    cusip[16];
	char    primarkNumber[16];
	DWORD   flags;
	BYTE    resv[12];
} ATFI_SYMBOL, *LPATFI_SYMBOL, *PATFI_SYMBOL;

// A-T Future database

typedef struct _ATFI_FUTURE_KEY0 {
	DWORD   optionDataType;
	AT_KEY  optionSymbol;
} ATFI_FUTURE_KEY0, *PATFI_FUTURE_KEY0, *LPATFI_FUTURE_KEY0;

typedef struct _ATFI_FUTURE_KEY1 {
	DWORD   underlyingDataType;
	AT_KEY  underlyingSymbol;
	DWORD   resv1;
} ATFI_FUTURE_KEY1, *PATFI_FUTURE_KEY1, *LPATFI_FUTURE_KEY1;

typedef struct _ATFI_FUTURE_KEY2 {
	DWORD   optionDataType;
	AT_KEY  optionSymbol;
	DWORD   underlyingDataType;
	AT_KEY  underlyingSymbol;
} ATFI_FUTURE_KEY2, *PATFI_FUTURE_KEY2, *LPATFI_FUTURE_KEY2;

typedef struct _ATFI_FUTURE {
	DWORD        optionDataType;
	AT_KEY       optionSymbol;
	DWORD        underlyingDataType;
	AT_KEY       underlyingSymbol;
	BYTE         strikePriceType;
	BYTE         resv1[3];
	DWORD        flags;
	AT_DATETIME  expirationDate;
	AT_PRICE     baseStrikePrice;
	AT_PRICE     strikeIncrement;
	WORD         count;
	BYTE         resv2[34];
} ATFI_FUTURE, *PATFI_FUTURE, *LPATFI_FUTURE;

// first call lookup tables

typedef struct _FCTABLE_DATA {
	char  code[16];
	char  description[80];
} FCTABLE_DATA, *PFCTABLE_DATA, *LPFCTABLE_DATA;

// event database

typedef struct _AT_EVENT_TIME {
	BYTE  day;						// day
	BYTE  month;					// month
	WORD  year;						// year
	WORD  seconds;					// hundredths of seconds
	BYTE  minute;					// minute
	BYTE  hour;						// hour
} AT_EVENT_TIME, *LPAT_EVENT_TIME;

#define EVENTTYPE_HALT			0
#define EVENTTYPE_HIGH_52		1
#define EVENTTYPE_LOW_52		2
#define EVENTTYPE_VOLUME		3
#define EVENTTYPE_PRICE_UP		4
#define EVENTTYPE_PRICE_DOWN	5

typedef struct _AT_EVENT_KEY0 {
	AT_EVENT_TIME  time;			// event time
	DWORD          type;			// event type
	DWORD          dataType;		// data type
	AT_KEY         ticker;			// ticker symbol
} AT_EVENT_KEY0, *LPAT_EVENT_KEY0, *PAT_EVENT_KEY0;

typedef struct _AT_EVENT_KEY1 {
	AT_EVENT_TIME  time;			// event time
	DWORD          resv;
} AT_EVENT_KEY1, *LPAT_EVENT_KEY1, *PAT_EVENT_KEY1;

typedef struct _AT_EVENT_KEY2 {
	DWORD          type;			// event type
	AT_EVENT_TIME  time;			// event time
	DWORD          resv;
} AT_EVENT_KEY2, *LPAT_EVENT_KEY2, *PAT_EVENT_KEY2;

typedef struct _AT_EVENT_KEY3 {
	DWORD   dataType;				// data type
	AT_KEY  ticker;					// ticker symbol
	DWORD   resv;
} AT_EVENT_KEY3, *LPAT_EVENT_KEY3, *PAT_EVENT_KEY3;

typedef struct _AT_EVENT_HALT {
	DWORD   condition: 8;			// halt condition, see atficond.h
	DWORD   conditionType: 2;		// condition type, see above
	DWORD   resv: 17;
	DWORD   priceType: 5;
	LONG    lastSale;				// last sale at halt
	LONG    high;					// high price at halt
	LONG    low;					// low price at halt
	DWORD   totalVolume;			// total volume at halt
	DWORD   sellVolume;				// sell volume
	DWORD   buyVolume;				// buy volume
} AT_EVENT_HALT, *LPAT_EVENT_HALT, *PAT_EVENT_HALT;

typedef struct _AT_EVENT_PRICE {
	BYTE         priceType;
	BYTE         resv[3];
	AT_PRICE     newPrice;
	AT_PRICE     oldPrice;
	AT_DATETIME  oldDate;
} AT_EVENT_PRICE, *LPAT_EVENT_PRICE, *PAT_EVENT_PRICE;

typedef struct _AT_EVENT_VOLUME {
	DWORD  volume;
	DWORD  averageVolume;
} AT_EVENT_VOLUME, *LPAT_EVENT_VOLUME, *PAT_EVENT_VOLUME;

typedef struct _AT_EVENT_DATA {
	AT_EVENT_TIME  time;			// event time
	DWORD          type;			// event type
	DWORD          dataType;		// data type
	AT_KEY         ticker;			// ticker symbol
	DWORD          flags;			// flags
	union {
		BYTE             data[128];
		AT_EVENT_HALT    halt;		// halt event
		AT_EVENT_PRICE   price;		// 52 week highs/lows
		AT_EVENT_VOLUME  volume;	// average daily volume
	};
} AT_EVENT_DATA, *LPAT_EVENT_DATA;

// account report database

#define ATUSAGE_QUOTES			0

typedef struct _ATFI_ACCOUNT_USAGE_KEY0 {
	char   userName[20];
	DWORD  type: 16;
	DWORD  day: 5;
	DWORD  month: 4;
	DWORD  year: 7;					// year-1970
} ATFI_ACCOUNT_USAGE_KEY0, *LPATFI_ACCOUNT_USAGE_KEY0, *PATFI_ACCOUNT_USAGE_KEY0;

typedef struct _ATFI_ACCOUNT_USAGE_KEY1 {
	char   userName[20];
	DWORD  resv;					// padding for duplicate keys
} ATFI_ACCOUNT_USAGE_KEY1, *LPATFI_ACCOUNT_USAGE_KEY1, *PATFI_ACCOUNT_USAGE_KEY1;

typedef struct _ATFI_ACCOUNT_USAGE_KEY2 {
	DWORD  type: 16;
	DWORD  day: 5;
	DWORD  month: 4;
	DWORD  year: 7;					// year-1970
	DWORD  resv;					// padding for duplicate keys
} ATFI_ACCOUNT_USAGE_KEY2, *LPATFI_ACCOUNT_USAGE_KEY2, *PATFI_ACCOUNT_USAGE_KEY2;

typedef struct _ATFI_ACCOUNT_USAGE_QUOTES {
	DWORD  usage[512];
} ATFI_ACCOUNT_USAGE_QUOTES, *LPATFI_ACCOUNT_USAGE_QUOTES, *PATFI_ACCOUNT_USAGE_QUOTES;

typedef struct _ATFI_ACCOUNT_USAGE {
	char   userName[20];
	union {
		struct {
			DWORD  type: 16;
			DWORD  day: 5;
			DWORD  month: 4;
			DWORD  year: 7;			// year-1970
		};
		DWORD  typeDate;
	};
	DWORD  flags;
	BYTE   resv[16];
	union {
		BYTE                       data[2048];
		ATFI_ACCOUNT_USAGE_QUOTES  quotes;
	};
} ATFI_ACCOUNT_USAGE, *LPATFI_ACCOUNT_USAGE, *PATFI_ACCOUNT_USAGE;

// company database

typedef struct _ATFI_COMPANY_KEY0 {
	char  companyID[12];
} ATFI_COMPANY_KEY0, *LPATFI_COMPANY_KEY0, *PATFI_COMPANY_KEY0;

typedef struct _ATFI_COMPANY_KEY1 {
	char   dsCompanyID[12];
	DWORD  resv;
} ATFI_COMPANY_KEY1, *LPATFI_COMPANY_KEY1, *PATFI_COMPANY_KEY1;

typedef struct _ATFI_COMPANY_KEY2 {
	char   ibesNumber[16];
	DWORD  resv;
} ATFI_COMPANY_KEY2, *LPATFI_COMPANY_KEY2, *PATFI_COMPANY_KEY2;

typedef struct _ATFI_COMPANY_KEY3 {
	char   disclosureNumber[16];
	DWORD  resv;
} ATFI_COMPANY_KEY3, *LPATFI_COMPANY_KEY3, *PATFI_COMPANY_KEY3;

typedef struct _ATFI_COMPANY_KEY5 {
	char   worldscopeNumber[16];
	DWORD  resv;
} ATFI_COMPANY_KEY5, *LPATFI_COMPANY_KEY5, *PATFI_COMPANY_KEY5;

typedef struct _ATFI_COMPANY_KEY6 {
	char   isin[16];
	DWORD  resv;
} ATFI_COMPANY_KEY6, *LPATFI_COMPANY_KEY6, *PATFI_COMPANY_KEY6;

typedef struct _ATFI_COMPANY_KEY7 {
	char   name[64];
	DWORD  resv;
} ATFI_COMPANY_KEY7, *LPATFI_COMPANY_KEY7, *PATFI_COMPANY_KEY7;

typedef struct _ATFI_COMPANY_KEY8 {
	DWORD   dataType;
	AT_KEY  ticker;
	DWORD   resv;
} ATFI_COMPANY_KEY8, *LPATFI_COMPANY_KEY8, *PATFI_COMPANY_KEY8;

typedef struct _ATFI_COMPANY_KEY9 {
	char   country[12];
	char   name[64];
	DWORD  resv;
} ATFI_COMPANY_KEY9, *LPATFI_COMPANY_KEY9, *PATFI_COMPANY_KEY9;

typedef struct _ATFI_COMPANY_KEY10 {
	char   primarySecurityID[12];
	DWORD  resv;
} ATFI_COMPANY_KEY10, *LPATFI_COMPANY_KEY10, *PATFI_COMPANY_KEY10;

typedef struct _ATFI_COMPANY_KEY11 {
	char   primarkNumber[12];
	DWORD  resv;
} ATFI_COMPANY_KEY11, *LPATFI_COMPANY_KEY11, *PATFI_COMPANY_KEY11;

typedef struct _ATFI_COMPANY {
	char         companyID[12];
	DWORD        flags;
	char		 resv1[4];
	char         dsCompanyID[12];
	char         ibesNumber[16];
	char         disclosureNumber1[16];
	char         disclosureNumber2[16];
	char         worldscopeNumber[16];
	char		 resv2[12];
	char         isin[16];
	char         resv3[32];
	char         name[64];
	char         primarySecurityID[12];
	DWORD        primaryDataType;
	AT_KEY       primaryTicker;
	DWORD        primarySource;
	char         country[12];
	char         url[100];
	AT_DATETIME  updateTime;
	union {
		BYTE lockCompanyID:1;
		BYTE lockFlags:1;
		BYTE lockDsCompanyID:1;
		BYTE lockIbesNumber:1;
		BYTE lockDisclosureNumber1:1;
		BYTE lockDisclosureNumber2:1;
		BYTE lockWorldscopeNumber:1;
		BYTE lockIsin:1;
		BYTE lockName:1;
		BYTE lockPrimarySecurityID:1;
		BYTE lockPrimaryDataType:1;
		BYTE lockPrimaryTicker:1;
		BYTE lockPrimarySource:1;
		BYTE lockCountry:1;
		BYTE lockUrl:1;
		BYTE lockPrimarkNumber:1;
		DWORD lockResv:16;
	};
	char         primarkNumber[12];
	BYTE         resv4[48];
} ATFI_COMPANY, *LPATFI_COMPANY, *PATFI_COMPANY;

// security database

typedef struct _ATFI_SECURITY_KEY0 {
	char   securityID[12];
	DWORD  resv;
} ATFI_SECURITY_KEY0, *LPATFI_SECURITY_KEY0, *PATFI_SECURITY_KEY0;

typedef struct _ATFI_SECURITY_KEY1 {
	char   companyID[12];
	DWORD  resv;
} ATFI_SECURITY_KEY1, *LPATFI_SECURITY_KEY1, *PATFI_SECURITY_KEY1;

typedef struct _ATFI_SECURITY_KEY2 {
	char   ricCode[20];
	DWORD  resv;
} ATFI_SECURITY_KEY2, *LPATFI_SECURITY_KEY2, *PATFI_SECURITY_KEY2;

typedef struct _ATFI_SECURITY_KEY3 {
	char   isin[16];
	DWORD  resv;
} ATFI_SECURITY_KEY3, *LPATFI_SECURITY_KEY3, *PATFI_SECURITY_KEY3;

typedef struct _ATFI_SECURITY_KEY4 {
	char   name[64];
	DWORD  resv;
} ATFI_SECURITY_KEY4, *LPATFI_SECURITY_KEY4, *PATFI_SECURITY_KEY4;

typedef struct _ATFI_SECURITY_KEY5 {
	DWORD   dataType;
	AT_KEY  ticker;
	DWORD   resv;
} ATFI_SECURITY_KEY5, *LPATFI_SECURITY_KEY5, *PATFI_SECURITY_KEY5;

typedef struct _ATFI_SECURITY_KEY6 {
	char    securityID[12];
	DWORD   dataType;
	AT_KEY  ticker;
} ATFI_SECURITY_KEY6, *LPATFI_SECURITY_KEY6, *PATFI_SECURITY_KEY6;

typedef struct _ATFI_SECURITY_KEY7 {
	char   dsSecurityID[12];
	DWORD  resv;
} ATFI_SECURITY_KEY7, *LPATFI_SECURITY_KEY7, *PATFI_SECURITY_KEY7;

typedef struct _ATFI_SECURITY_KEY8 {
	char   primarkNumber[12];
	DWORD  resv;
} ATFI_SECURITY_KEY8, *LPATFI_SECURITY_KEY8, *PATFI_SECURITY_KEY8;

typedef struct _ATFI_SECURITY {
	char         securityID[12];
	DWORD        flags;
	char		 resv1[4];
	char         companyID[12];
	char		 cfiCode[6];
	char		 ricCode[20];
	char		 dsSecurityID[12];
	char         djTicker[8];
	BYTE         resv2[30];
	char         isin[16];
	char         name[64];
	DWORD        securityType;
	DWORD        dataType;
	AT_KEY       ticker;
	DWORD        source;
	AT_DATETIME  updateTime;
	char         currency[3];
	BYTE         resv3;
	char         globalTopicTicker[32];
	union {
		BYTE lockSecurityID:1;
		BYTE lockFlags:1;
		BYTE lockCompanyID:1;
		BYTE lockCfiCode:1;
		BYTE lockRicCode:1;
		BYTE lockDsSecurityID:1;
		BYTE lockDjTicker:1;
		BYTE lockIsin:1;
		BYTE lockName:1;
		BYTE lockSecurityType:1;
		BYTE lockDataType:1;
		BYTE lockTicker:1;
		BYTE lockSource:1;
		BYTE lockCurrency:1;
		BYTE lockGlobalTopicTicker:1;
		BYTE lockPrimarkNumber:1;
		DWORD lockResv:16;
	};
	char         primarkNumber[12];
	BYTE         resv4[12];
} ATFI_SECURITY, *LPATFI_SECURITY, *PATFI_SECURITY;

// industry sector database

typedef struct _ATFI_SECTOR_KEY0 {
	char  vendor[4];
	char  parent[20];
	char  industry[20];
} ATFI_SECTOR_KEY0, *LPATFI_SECTOR_KEY0, *PATFI_SECTOR_KEY0;

typedef struct _ATFI_SECTOR_KEY1 {
	char   vendor[4];
	char   industry[20];
	DWORD  resv;
} ATFI_SECTOR_KEY1, *LPATFI_SECTOR_KEY1, *PATFI_SECTOR_KEY1;

typedef struct _ATFI_SECTOR_KEY2 {
	char   vendor[4];
	char   parent[20];
	DWORD  resv;
} ATFI_SECTOR_KEY2, *LPATFI_SECTOR_KEY2, *PATFI_SECTOR_KEY2;

#define ATISTYPE_MARKET_SECTOR		0
#define ATISTYPE_ECONOMIC_SECTOR	1
#define ATISTYPE_INDUSTRY			2
#define ATISTYPE_SUB_INDUSTRY		3
#define ATISTYPE_UNKNOWN			0xff

typedef struct _ATFI_INDUSTRY_SECTOR {
	char         vendor[4];
	char         parent[20];
	char         industry[20];
	AT_DATETIME  updateTime;
	DWORD        flags;
	char         name[64];
	BYTE         type;
	BYTE         level;
	BYTE         resv[30];
} ATFI_INDUSTRY_SECTOR, *LPATFI_INDUSTRY_SECTOR, *PATFI_INDUSTRY_SECTOR;

// industry code database

typedef struct _ATFI_INDUCODE_KEY0 {
	char   vendor[4];
	char   region[4];
	char   industry[20];
} ATFI_INDUCODE_KEY0, *LPATFI_INDUCODE_KEY0, *PATFI_INDUCODE_KEY0;

typedef struct _ATFI_INDUCODE_KEY1 {
	DWORD   dataType;
	AT_KEY  symbol;
	DWORD   resv;
} ATFI_INDUCODE_KEY1, *LPATFI_INDUCODE_KEY1, *PATFI_INDUCODE_KEY1;

typedef struct _ATFI_INDUCODE_KEY2 {	
	char   vendor[4];
	char   industry[20];
	DWORD  resv;
} ATFI_INDUCODE_KEY2, *LPATFI_INDUCODE_KEY2, *PATFI_INDUCODE_KEY2;

#define ATINDUCODE_INDEX		0x00000001

typedef struct _ATFI_INDUSTRY_CODE {
	char         vendor[4];
	char         region[4];
	char         industry[20];
	DWORD        dataType;
	AT_KEY       symbol;
	AT_DATETIME  updateTime;
	DWORD        flags;
	char         name[64];
	BYTE         resv[16];
} ATFI_INDUSTRY_CODE, *LPATFI_INDUSTRY_CODE, *PATFI_INDUSTRY_CODE;

// industry chain database

typedef struct _ATFI_INDUSTRY_KEY0 {
	char    vendor[4];
	char    region[4];
	char    industry[20];
	DWORD   dataType;
	AT_KEY  symbol;
} ATFI_INDUSTRY_KEY0, *LPATFI_INDUSTRY_KEY0, *PATFI_INDUSTRY_KEY0;

typedef struct _ATFI_INDUSTRY_KEY1 {
	DWORD   dataType;
	AT_KEY  symbol;
	DWORD   resv;
} ATFI_INDUSTRY_KEY1, *LPATFI_INDUSTRY_KEY1, *PATFI_INDUSTRY_KEY1;

typedef struct _ATFI_INDUSTRY_KEY2 {
	char   vendor[4];
	char   industry[20];
	DWORD  resv;
} ATFI_INDUSTRY_KEY2, *LPATFI_INDUSTRY_KEY2, *PATFI_INDUSTRY_KEY2;

typedef struct _ATFI_INDUSTRY {
	char         vendor[4];
	char         region[4];
	char         industry[20];
	DWORD        dataType;
	AT_KEY       symbol;
	AT_DATETIME  updateTime;
	DWORD        flags;
	BYTE         resv[16];
} ATFI_INDUSTRY, *LPATFI_INDUSTRY, *PATFI_INDUSTRY;

/*=============================================================*/

#pragma pack()

/*=============================================================*/

#endif

/*=============================================================*/
