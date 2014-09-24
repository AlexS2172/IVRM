#ifndef INCL_PCQT1400H						/* Just once!	*/
#define INCL_PCQT1400H

/* $Header:   G:/LOG/INCLUDE/pcqt1400.h_v   1.10   Jan 30 1997 13:30:10   unknown  $ */
/*****************************************************************************
 *
 * $Workfile::   pcqt1400.h                                                  $
 *
 * $Log::   G:/LOG/INCLUDE/pcqt1400.h_v                                      $
 * 
 *    Rev 1.10   Jan 30 1997 13:30:10   unknown
 * realigned snp_data structure for Market Arts
 * 
 *    Rev 1.9   Oct 25 1996 13:29:38   unknown
 * moved fundata to end ot dba_record_55
 * 
 *    Rev 1.8   Oct 23 1996 14:59:32   unknown
 * added new union members for 2 byte alignment purposes
 * 
 *    Rev 1.7   04 Jan 1996 09:22:08   bac
 * The item type now matches etix.h and edbx.h
 * 
 *    Rev 1.6   08 Jun 1995 12:10:04   BAC
 * repaired the conflicts with wdb.h due to the renaming
 * of the PCQT1400_RECORD to PCQT1400.
 * 
 *    Rev 1.5   05 Aug 1994 17:15:38   BPM
 * Moved a field from the 1400 portion out to the main structure.
 * 
 *    Rev 1.4   21 Apr 1994 16:11:18   BPM
 * Added updateDays to the PCQT1400_RECORD, and shortened the
 * PCQT1400_REC to make sure that the record remained the same length.
 * 
 *    Rev 1.3   23 Feb 1994 14:27:52   BPM
 * Tweaked header and other PVCS keywords.
 * 
 *    Rev 1.2   23 Feb 1994 14:24:58   BPM
 * Added PVCS keywords and added deleted key types (1401 and 1501).
 *
 *****************************************************************************/

#ifndef UNIX
#pragma pack(1) /* Byte aligned (necessary for 1400 file).	*/
#endif

typedef struct _CUSIP_REC  {
   CHAR    issueDesc[34];
   CHAR     primaryExchange;
   CHAR    resv4;
   LONG    cusipFileDate;
/*   UCHAR           recordState;	*/
/*   CHAR    resv5[3];					*/
     CHAR    resv5[4];           /* This was changed because the recordState */
                                 /* field has been moved to the              */
                                 /* PCQT1400_RECORD.                         */
        } CUSIP_REC;  /* 44 bytes		*/


typedef struct _SNP_STOCK {
        CHAR            tickerSymbol[6];                                /* S&Ps ticker symbol                */
        CHAR            exchanges[3];                                   /* Exchanges traded                  */
        CHAR            resv1;
        SHORT           industryCode;                                   /* SIC Codes                         */
        CHAR            sp500Category;                                  /* See S&P catagory codes            */
        CHAR            optionIndicator;                                /* Optionable Security               */
        CHAR            rank[4];                                        /* S&P rank AAA+ -> D                */
        ULONG           currentAssets;                                  /* Assets in 000's                   */
        ULONG           bookValue;                                      /* Book value in 000's               */
        ULONG           commonShares;                                   /* Common shares outstanding         */
        ULONG           pfdShares;                                      /* Preferred shares outstanding      */
        LONG            divYield;                                       /* Yield in % .00 implied            */
        CHAR            divFreq[4];                                     
        LONG            latestDivPmt;                                   /* Dollars per shares dec .00 implied*/
        LONG            dtLastDiv;                                      /* Date Last dividend payed          */
        SHORT           dtExDiv;                                        /* Date (mo/yr) stock is ex-div      */
        LONG            ttlIndicRate;                                   /* Total Indicated Rate              */
        USHORT  			yrDivMissed;                             			/* Year last dividend was missed     */
        LONG            epsLast12Mo;                                    /* self explanitory                  */
        CHAR            fn_eps[3];                 							/* footnote eps                      */
        SHORT           interim[2];                                     /* Interim[0] = #months              */
        CHAR            resv2;                                          /* Interim[1] = month code 1-12      */
        LONG            epsLastIntrm;                                   /* EPS .00                           */
        LONG            epsCurrentIntrm;                                /* EPS .00                           */
        LONG            epsGrowthRate;                                  /* EPS Growth rate .00               */
        ULONG           instStockHolders;                               /* Number of instit. owners          */
        ULONG           instSharesHeld;                         			/* Number of shares held by instit.  */
        LONG            currentYrHigh;                                  /* Price Type 15 in ETI spec         */
        LONG            currentYrLow;                                   /* ''                                */
        LONG            betaWODiv;                                      /* beta w/o dividend .0000           */
        LONG            betaWDiv;                                       /* beta w/Dividend .0000             */
        ULONG           annualSales;                                    /* Annual Sales 000s                 */
        LONG            annualIncome;                                   /* Annual Income 000s                */
        SHORT           fiscalPeriod;                                   /* 1-12                              */
        ULONG           adrShares;                 							/* Shares per ADR unit ??            */
        CHAR            issueType[3];                                   /* See issue type defines            */
        CHAR     			resv3;      
        LONG            snpFileDate;
        } SNP_STOCK;  /*	 128 bytes	*/

typedef struct _SNP_DATA {
        CHAR            tickerSymbol[6];                                /* S&Ps ticker symbol                */
        CHAR            exchanges[3];                                   /* Exchanges traded                  */
        CHAR            resv1;
        SHORT           industryCode;                                   /* SIC Codes                         */
        CHAR            sp500Category;                                  /* See S&P catagory codes            */
        CHAR            optionIndicator;                                /* Optionable Security               */
        CHAR            rank[4];                                        /* S&P rank AAA+ -> D                */
        SHORT           fiscalPeriod;                                   /* 1-12                              */
        ULONG           currentAssets;                                  /* Assets in 000's                   */
        ULONG           bookValue;                                      /* Book value in 000's               */
        ULONG           commonShares;                                   /* Common shares outstanding         */
        ULONG           pfdShares;                                      /* Preferred shares outstanding      */
        LONG            divYield;                                       /* Yield in % .00 implied            */
        CHAR            divFreq[4];                                     
        LONG            latestDivPmt;                                   /* Dollars per shares dec .00 implied*/
        LONG            dtLastDiv;                                      /* Date Last dividend payed          */
        SHORT           dtExDiv;                                        /* Date (mo/yr) stock is ex-div      */
        USHORT  			yrDivMissed;                             			/* Year last dividend was missed     */
        LONG            ttlIndicRate;                                   /* Total Indicated Rate              */
        LONG            epsLast12Mo;                                    /* self explanitory                  */
        CHAR            fn_eps[3];                 							/* footnote eps                      */
        CHAR            resv2;                                          /* Interim[1] = month code 1-12      */
        SHORT           interim[2];                                     /* Interim[0] = #months              */
        LONG            epsLastIntrm;                                   /* EPS .00                           */
        LONG            epsCurrentIntrm;                                /* EPS .00                           */
        LONG            epsGrowthRate;                                  /* EPS Growth rate .00               */
        ULONG           instStockHolders;                               /* Number of instit. owners          */
        ULONG           instSharesHeld;                         			/* Number of shares held by instit.  */
        LONG            currentYrHigh;                                  /* Price Type 15 in ETI spec         */
        LONG            currentYrLow;                                   /* ''                                */
        LONG            betaWODiv;                                      /* beta w/o dividend .0000           */
        LONG            betaWDiv;                                       /* beta w/Dividend .0000             */
        ULONG           annualSales;                                    /* Annual Sales 000s                 */
        LONG            annualIncome;                                   /* Annual Income 000s                */
        ULONG           adrShares;                 							/* Shares per ADR unit ??            */
        CHAR            issueType[3];                                   /* See issue type defines            */
        CHAR     			resv3;      
        LONG            snpFileDate;
        } SNP_DATA;  /*	 128 bytes	*/


typedef union _PCQFUNDDATA  {
        CUSIP_REC       cusip;
        SNP_STOCK       stkGuide;
        SNP_DATA			snpData;
        CHAR            resv[184];           /* This was changed to 184 by   */
                                             /* BPM to allow for an extra    */
                                             /* USHORT and UCHAR in the      */
                                             /* PCQT1400_RECORD (below).     */
        } PCQFUNDDATA;

typedef struct _PCQT1400  {
        DBA_KEY         symbol;
        CHAR            cusip[9];
        CHAR            desc[36];
        USHORT          recType;
        PCQFUNDDATA        data;
        UCHAR           recordState;
        USHORT          updateDays;          /* This field will be set to a  */ 
                                             /* number which represents the  */
                                             /* times that this record will  */ 
                                             /* be transmitted in an update. */ 
        } PCQT1400;

typedef struct _DBA_RECORD_55  {
        DBA_KEY         symbol;
        UCHAR           recordState;
        CHAR            cusip[9];
        CHAR            desc[36];
        USHORT          recType;
        USHORT          updateDays;          /* This field will be set to a  */ 
                                             /* number which represents the  */
                                             /* times that this record will  */ 
                                             /* be transmitted in an update. */ 
        PCQFUNDDATA        data;
        } DBA_RECORD_55;


#ifndef INCL_1400RECORD 		/* To avoid conflicts with wdb.h	*/

/*
 * Following are values that recType may take:
 *
 */

#define RECTYPE_1400_CUSIP      0x1400
#define RECTYPE_1400_CUSIP_DEL  0x1401
#define RECTYPE_1400_STKGDE     0x1500
#define RECTYPE_1400_STKGDE_DEL 0x1501


/*
 * Possible Keys for lookup into the Cusip Database:
 *
 */

#define KEY1400_TYPE_SYM        0
#define KEY1400_SYM_TYPE        1

#define KEY1400_TYPE_CUSIP      2
#define KEY1400_CUSIP           3

#define KEY1400_TYPE_DESC       4
#define KEY1400_DESC            5


/*
 * Key Segment Structures
 *
 */

typedef struct _PCQT1400_KEY0_SEG {

   USHORT       recType;
   DBA_KEY      symbol;

  } PCQT1400_KEY0_SEG;

typedef struct _PCQT1400_KEY1_SEG {

		  USHORT   type;
        UCHAR symbol[12];
        UCHAR countryCode[2];
        UCHAR exchangeCode[2];
   USHORT       recType;

  } PCQT1400_KEY1_SEG;


typedef struct _PCQT1400_KEY2_SEG {

   USHORT       recType;
   CHAR                 cusip[9];

  } PCQT1400_KEY2_SEG;


typedef struct _PCQT1400_KEY3_SEG {

   CHAR                 cusip[9];

  } PCQT1400_KEY3_SEG;


typedef struct _PCQT1400_KEY4_SEG {
   USHORT       recType;
   CHAR                 desc[20];

  } PCQT1400_KEY4_SEG;


typedef struct _PCQT1400_KEY5_SEG {

   CHAR                 desc[22];

  } PCQT1400_KEY5_SEG;


typedef union _PCQT1400_KEY_PATH {

   PCQT1400_KEY0_SEG key0;
   PCQT1400_KEY1_SEG key1;
   PCQT1400_KEY2_SEG key2;
   PCQT1400_KEY3_SEG key3;
   PCQT1400_KEY4_SEG key4;
        PCQT1400_KEY5_SEG key5;

  } PCQT1400_KEY_PATH;

/* Defines for record state field in cusip rec		*/

#define CUSIP_REC_UPDATED               0x01
#define CUSIP_REC_NEW                   0x08
#define CUSIP_REC_SOFTDEL               0x02
#define CUSIP_REC_IN_USE                0x04

#endif

#ifndef UNIX
#pragma pack()  /* End of PCQT1400, restore previous packing.	*/
#endif

#endif
