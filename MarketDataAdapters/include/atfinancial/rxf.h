/* 
 * Robinson-Humpfrey ( Atlanta Development Center)
 *
 * record definition for the IERX.dta btrieve data base
 *
 * The number of records in this file will change infrequently;
 * most of the activity will affect acFlags.
 *
 * Rev 1.2 Geoff Chalmers 5/31/93:
 *
 *   1. I have added a field for szType to indicate the R7/D1/R2
 *      that comes in from ADP.  
 *
 *   2. The fields szTerm and szSystem were too short by one byte
 *      (the trailing NULL) as they were defined sz.  
 * 
 */

#if !defined(INCL_RXF)
#define INCL_RXF

#ifndef RC_INVOKED
#pragma pack(1)						 /* Word aligned. */
#endif /* RC_INVOKED */

#define IERX_FLAGS_MAX                                6
#define IERX_ANALYST_LASTNAME_LEN                     15
#define IERX_ANALYST_NAME_LEN                         30
#define IERX_TICKER_LEN                               8
#define IERX_DATE_LEN                                 6
#define IERX_TRANS_NUMB_LEN                           4
#define IERX_TERM_LEN                                 6
#define IERX_SYSTEM_LEN                               8

	// Constants used for nExchange

#define IERX_NYSE                   1
#define IERX_AMEX                   10
#define IERX_PACIFIC                15
#define IERX_MIDWEST                20
#define IERX_BOSTON                 25
#define IERX_PHILADELPHIA           27
#define IERX_OTC                    36
#define IERX_OTHER                  -1    /* just in case */

	// Constants used for szFlags

#define IERX_RESTRICTED_CODES       0
#define IERX_MORNING_NOTES          1
#define IERX_SHORT_OPINION          2
#define IERX_RISK_INDICATOR         3
#define IERX_RECOMMENDATION         4
#define IERX_OTC_MARKETM            5

typedef struct IERX_REC_struct
		{
		char	szTicker[IERX_TICKER_LEN + 1];				// primary key
		short	nExchange;
		char	szFlags[IERX_FLAGS_MAX + 1];
		char	cRatingOld;
		char	cRiskOld;
		char	szAnalystLastName[IERX_ANALYST_LASTNAME_LEN + 1];
		char	szAnalystName[IERX_ANALYST_NAME_LEN + 1];
		char	szRatingDate[IERX_DATE_LEN + 1];			// yymmdd
		char	szUpdateDate[IERX_DATE_LEN + 1];			// yymmdd
		char	szUpdateTime[IERX_DATE_LEN + 1];			// hhmmss
		char	szUpdateTransNo[IERX_TRANS_NUMB_LEN + 1];
		char	szTerm[IERX_TERM_LEN];
		char	szSystem[IERX_SYSTEM_LEN];
		char	sRFU[20];						// reserved for future use
		}
		IERX_REC;

/*      This is the record that is passed to us on the IERX screen, one
 *      to a line.  It is the same as the record passed to ADP
 *
 */

#define IERX_ADP_ID_LEN                   4
#define IERX_ADP_NUMB_LEN                 4
#define IERX_ADP_DATE_LEN                 6
#define IERX_ADP_TIME_LEN                 6
#define IERX_ADP_TYPE_LEN                 2
#define IERX_ADP_SYMBOL_LEN               8
#define IERX_ADP_QUOTEALERT_LEN           8

#define IERX_ADP_ANALYST_LEN              30
#define IERX_ADP_EXCHANGE_ID_LEN          2

#define IERX_ADP_ADD_REQ                  "R7"
#define IERX_ADP_DEL_REQ                  "D1"
#define IERX_ADP_CLEAR_NOTES              "R2"

#define IERX_ADP_RESTRICTED_CODES    IERX_RESTRICTED_CODES
#define IERX_ADP_MORNING_NOTES       IERX_MORNING_NOTES
#define IERX_ADP_SHORT_OPINION       IERX_SHORT_OPINION
#define IERX_ADP_RISK_INDICATOR      IERX_RISK_INDICATOR
#define IERX_ADP_RECOMMENDATION      IERX_RECOMMENDATION
#define IERX_ADP_OTC_MARKETM         IERX_OTC_MARKETM

typedef struct IERX_ADP_struct
		{
		char	sID[IERX_ADP_ID_LEN];
		char	sNumb[IERX_ADP_NUMB_LEN];
		char	sDate[IERX_ADP_DATE_LEN];    /* ddmmyy */
		char	sTime[IERX_ADP_TIME_LEN];    /* hhmmss */
		char	sType[IERX_ADP_TYPE_LEN];
		char	sSymbol[IERX_ADP_SYMBOL_LEN];
		char	acQuoteAlert[IERX_ADP_QUOTEALERT_LEN];
		char	sAnalystName[IERX_ADP_ANALYST_LEN];
		char	sExchangeID[IERX_ADP_EXCHANGE_ID_LEN];
		char	sFiller[2];
		}
		IERX_ADP;


/* this is the data to be passed via SPX to AT Financial */

typedef struct
		{
		char	cMsgType;                         // 'I' ASCII
		char	cSubtype;                         // 'X' ASCII
		char	szTicker[IERX_TICKER_LEN + 1];
		int		nExchange;
		char	szFlags[IERX_FLAGS_MAX + 1];
		char	cRatingOld;
		char	cRiskOld;
		char	szAnalystName[IERX_ANALYST_NAME_LEN + 1];
		char	szRatingDate[IERX_DATE_LEN + 1];
		char	szUpdateDate[IERX_DATE_LEN + 1];
		char	szUpdateTime[IERX_DATE_LEN + 1];
		char	szUpdateTransNo[IERX_TRANS_NUMB_LEN + 1];
		char	szTerm[IERX_TERM_LEN + 1];
		char	szSystem[IERX_SYSTEM_LEN + 1];
		char	szType[IERX_ADP_TYPE_LEN + 1];
		char	sRFU[20];
		WORD	wTransID;
		}
		IERX_QSM;

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif	
