/*
 * PCQ1CALL.H
 * 
 * Record layout for the PC Quote Earnings Consensus record, compiled from
 * data received from the First Call Corporation, as well as the key
 * layouts for the Btrieve database.
 * 
 * For detailed information on the field definitions, refer to documents
 * prepared by Scott Clyde.
 * 
 * S. Feldman,   August 1994
 * 
 */
 




typedef struct _FC_DATE {    // Corresponds to SQLDATETIME structure

   LONG dtdays;              // # days since 1/1/1900
   ULONG dttime;             // 1/300ths of a second since midnight
   
   } FC_DATE;


/*
 * Whether for actual reported earnings, estimates, or standard deviations,
 * the FC_EARNINGS structure contains the annual composite and four quarterly
 * values:
 * 
 */

typedef struct _FC_EARNINGS {
   
   LONG annual;
   LONG q1;
   LONG q2;
   LONG q3;
   LONG q4;
   
   } FC_EARNINGS;


typedef struct _DBA_RECORD_69 {
   
   DBA_KEY     item;
   USHORT      fiscalYear;
   UCHAR       fiscalMonth;
   USHORT      cntBrokers;        // Number of brokers following issue
   UCHAR       industry[6];       // not currently used
   UCHAR       cusip[8];          // not currently used
   SHORT       xptdDate;          // not currently used
   LONG        FC;                // not currently used
   SHORT       curPeriod;         // See HSC's docs.
   FC_DATE     dateConfirm;       // Most recent confirm or change by any broker of their annual estimate
   FC_DATE     dateRevised;       // Most recent change of any annual estimate
   FC_DATE     dateReported;      // Most recent reporting of actual earnings
   FC_DATE     timeReceived;      // When an estimate record was most recently received from First Call
   LONG        surprise;          // % diff of most recent quarter from current estimate
   SHORT       surpriseAlert;     // See HSC's docs.
   SHORT       alert;             // See HSC's docs.
   
   FC_EARNINGS earningsCurrent;   // Current earnings, in cents; see docs
   FC_EARNINGS earningsPrevious;  // Mean of earnings estimates prior to dateRvsd
   FC_EARNINGS stdDeviation;      // See docs
   
   // The following pairs of values are for annual and for current quarter,
   // as per the curPeriod indicator:
   
   LONG        lastAnnualEst;        // Most recent estimate (from any broker)
   LONG        lastQuarterEst;
   
   LONG        lastAnnualPctChg;     // % that the preceding field changed 
   LONG        lastQuarterPctChg;    //    from that broker's previous est.
   
   LONG        lastAnnualStdDev;     // # of std dev'ns that last(Annual!Quarter)Est
   LONG        lastQuarterStdDev;    // is above/below earningsPrevious
   
   LONG        highAnnualEst;        // Highest current estimate (any broker)
   LONG        highQuarterEst;
   
   LONG        lowAnnualEst;         // Lowest current estimate (any broker)
   LONG        lowQuarterEst;

   LONG        todayAnnualEstChg;    // Mean chnges in brkr estimates today
   LONG        todayQuarterEstChg;
   
   LONG        weekAnnualEstChg;     // Mean chnges in brkr estimates this week
   LONG        weekQuarterEstChg;
   
   SHORT       todayAnnualCntRaised; // Number of estimates revised up today
   SHORT       todayQuarterCntRaised;
   
   SHORT       weekAnnualCntRaised;  // Number of estimates revised up this week
   SHORT       weekQuarterCntRaised;
   
   SHORT       todayAnnualCntLowered;// Number of estimates revised down today
   SHORT       todayQuarterCntLowered;
   
   SHORT       weekAnnualCntLowered; // Number of estimates revised down this week
   SHORT       weekQuarterCntLowered;
   
   SHORT       todayAnnualNewEst;    // Number of new estimates today
   SHORT       todayQuarterNewEst;
   
   SHORT       todayAnnualConfirmed; // Number of estimates confirmed (staying the same) today
   SHORT       todayQuarterConfirmed;
   
   } DBA_RECORD_69;
   
   



/*
 * Btrieve Key Structures
 * 
 */


#define FCALL_NDX_SYMB_FISYEAR    0
#define FCALL_NDX_SYMB            1
#define FCALL_NDX_CURPERIOD       2
#define FCALL_NDX_RVSDDATE        3
#define FCALL_NDX_ALERT           4
#define FCALL_NDX_RPTDDATE        5


typedef struct _FCALL_KEY_SYMB_FISYEAR {
   
   DBA_KEY item;
   USHORT  fiscalYear;
   
   } FCALL_KEY_SYMB_FISYEAR;

typedef struct _FCALL_KEY_SYMB {

   DBA_KEY item;
   
   } FCALL_KEY_SYMB;

typedef struct _FCALL_KEY_CURPERIOD {

   SHORT curPeriod;
   
   } FCALL_KEY_CURPERIOD;

typedef struct _FCALL_KEY_RVSDDATE {

   FC_DATE dateRevised;
   
   } FCALL_KEY_RVSDDATE;

typedef struct _FCALL_KEY_ALERT {
   
   SHORT alert;

   } FCALL_KEY_ALERT;

typedef struct _FCALL_KEY_RPTDDATE {
                                                    
   FC_DATE dateReported;   
   
   } FCALL_KEY_RPTDDATE;
