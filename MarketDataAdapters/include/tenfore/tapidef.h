/*  TAPIDEF.H
 *
 *  Tenfore Ticker --> 3rd parties
 *
 *  Conversion of Tenfore Ticker messages (from the feed server)
 *  into general format usable by 3rd parties
 *
 *  (C) 1997-03 EuroAmericanGroup Inc. all rights reserved
 *  Author: M.H.Gruener
 *
 */
#ifndef __TAPIDEF_H
#define __TAPIDEF_H

#ifndef ArraySize
#define ArraySize(x) (sizeof(x)/sizeof(*x))
#endif

/*
 * definitions for the different value types
 */
#define TF_VAL_TYPE_NULL         0     // undefined type
#define TF_VAL_TYPE_CHAR         1     // signed character
#define TF_VAL_TYPE_BYTE         2     // unsigned character
#define TF_VAL_TYPE_SHORT        3     // signed short (2 bytes)
#define TF_VAL_TYPE_WORD         4     // unsigned short (2 bytes)
#define TF_VAL_TYPE_INT          5     // signed integer (4 bytes)
#define TF_VAL_TYPE_UINT         6     // unsigned integer (4 bytes)
#define TF_VAL_TYPE_LONG         7     // signed long
#define TF_VAL_TYPE_ULONG        8     // unsigned long
#define TF_VAL_TYPE_FLOAT        9     // float
#define TF_VAL_TYPE_DOUBLE      10     // double
#define TF_VAL_TYPE_LPSTR       11     // pointer to zero terminated string
#define TF_VAL_TYPE_FRAC        12     // EAG fractional representation (see below)
#define TF_VAL_TYPE_BINARY      13     // binary data block
#define TF_VAL_TYPE_JULIAN      14     // julian date (use TApiConvJulianDate())

/*
 * The EAG fractional representation has the following format
 * (in a unsigned long)
 *
 *   tfff ffnn nnnn nnnn nnnn nnnn nnnn nnnn
 *
 *   where: t =  1 bit reserved
 *          f =  5 bit fraction table index for the denominator
 *                 value range: 0 .. 17
 *          n = 26 bit numerator
 *
 *   use the TApiFracTo...() functions to retrieve the different parts
 *
 *   TApiFracToDouble (TfValue *v)       - converts the fractional value
 *                                        into a double value
 *   TApiFracToDenom (TfValue *v)        - returns the denominator
 *   TApiFracToNumerator (TfValue *v)    - returns the numerator
 */


/*
 * definitions for the different field types
 * NOTE:
 *   they should be translated into the tenfore field definitions
 */
#define TF_FIELD_SYMBOL             0     // n symbol name
#define TF_FIELD_MIN_OF_DAY         1     // 2 time in minutes of the day (0..1440)
#define TF_FIELD_LAST               2     // 4 last price
#define TF_FIELD_VOLUME             3     // 4 last volume
#define TF_FIELD_BID                4     // 4 bid price
#define TF_FIELD_BID_SIZE           5     // 4 bid size
#define TF_FIELD_ASK                6     // 4 ask price
#define TF_FIELD_ASK_SIZE           7     // 4 ask size
#define TF_FIELD_BID_MARKET         8     // 1 bid market
#define TF_FIELD_BID_TIME           9     // 2 best bid time (in minutes of the day)
#define TF_FIELD_BEST_BID          10     // 4 best bid price
#define TF_FIELD_BEST_BID_SIZE     11     // 4 best bid size
#define TF_FIELD_ASK_MARKET        12     // 1 ask market
#define TF_FIELD_ASK_TIME          13     // 2 best ask time (in minutes of the day)
#define TF_FIELD_BEST_ASK          14     // 4 best ask price
#define TF_FIELD_BEST_ASK_SIZE     15     // 4 best ask size
#define TF_FIELD_CUM_VOLUME        16     // 4 cumulative volume
#define TF_FIELD_OPEN              17     // 4 open price
#define TF_FIELD_HIGH              18     // 4 high price
#define TF_FIELD_LOW               19     // 4 low price
#define TF_FIELD_CLOSE             20     // 4 close price
#define TF_FIELD_OP_INT            21     // 4 open interest

//
// field definitions for time and date record (eastern day time)
//
#define TF_FIELD_TIME_DAY          22     // 1 system time - day
#define TF_FIELD_TIME_MONTH        23     // 1 system time - month
#define TF_FIELD_TIME_YEAR         24     // 1 system time - year
#define TF_FIELD_TIME_HOURS        25     // 1 system time - hour
#define TF_FIELD_TIME_MINUTES      26     // 1 system time - minute
#define TF_FIELD_TIME_SECONDS      27     // 1 system time - second

//
// field definitions for error messages from the TIC card
// must not be transmitted to the end user
//
#define TF_FIELD_ERROR_CODE        28     // 4 TIC error code
#define TF_FIELD_ERROR_TEXT        29     // n TIC error text

//
// field definitions added 05.03.97
//
#define TF_FIELD_TRADE_COUNT       30     // 4 # of trades since market opening
#define TF_FIELD_FFM_SPOT          31     // 4 German Spot price (Kassakurs)

//
// field types for options, futures and future options
//
#define TF_FIELD_PREV_VOLUME       32     /* 4 previous days volume */
#define TF_FIELD_EST_VOLUME        33     /* 4 estimated volume (= cumulative volume for futures) */
#define TF_FIELD_SETTL             34     /* 4 settlement price */
#define TF_FIELD_PREV_SETTL        35     /* 4 previous days settlement */
#define TF_FIELD_OP_RANGE1         36     /* 4 closing range 1 */
#define TF_FIELD_OP_RANGE2         37     /* 4 closing range 2 */
#define TF_FIELD_CL_RANGE1         38     /* 4 opening range 1 */
#define TF_FIELD_CL_RANGE2         39     /* 4 opening range 2 */
#define TF_FIELD_SP_RANGE1         40     /* 4 special range 1 */
#define TF_FIELD_SP_RANGE2         41     /* 4 special range 1 */
#define TF_FIELD_LIMIT_HIGH        42     /* 4 limit high */
#define TF_FIELD_LIMIT_LOW         43     /* 4 limit low */

//
// LME specific field types
//
#define TF_FIELD_LME_CF_MONTH      44     /* 1 LME cash/forward month */
#define TF_FIELD_LME_SPRD_MONTH    45     /* 1 LME spread month */
#define TF_FIELD_LME_PR_QUAL       46     /* 1 LME price qualifier */
#define TF_FIELD_LME_DEALER        47     /* n LME dealer */

//
// Ticker Plant Frankfurt (TPF) specific field types
//
#define TF_FIELD_TPF_CQUAL         48     /* 1 close qualifier (for IBIS(213): broker, for FFM(200): qualifier) */
#define TF_FIELD_TPF_SQUAL         49     /* 1 spot qualifier (for IBIS(213): broker, for FFM(200): qualifier) */
#define TF_FIELD_TPF_LQUAL         50     /* 1 last qualifier (for IBIS(213): broker, for FFM(200): qualifier) */
#define TF_FIELD_TPF_BQUAL         51     /* 1 bid qualifier (for IBIS(213): broker, for FFM(200): qualifier) */
#define TF_FIELD_TPF_AQUAL         52     /* 1 ask qualifier (for IBIS(213): broker, for FFM(200): qualifier) */
#define TF_FIELD_TPF_SPOT          53     /* 4 German Spot price (Kassakurs) */

//
// Swisse Market Feed (SMF) specific field types
//
#define TF_FIELD_SMF_FORWARD       54     /* 4 SMF forward price */
#define TF_FIELD_SMF_FWD_MONTH     55     /* 1 SMF forward month */
#define TF_FIELD_SMF_PREMIUM       56     /* 4 SMF premium price */

//
// LSE specific field definitions
//
#define TF_FIELD_MID_PRICE         57     /* LSE mid price */

#define TF_FIELD_CMP_HIGH          58     /* 0x3a 4 composite high price */
#define TF_FIELD_CMP_LOW           59     /* 0x3b 4 composite low price */

/*
 *  additional LME field definitions
 */
#define TF_FIELD_LME_CF_DAY        76     /* LME cash/forward day */
#define TF_FIELD_LME_CF_YEAR       77     /* LME cash/forward year */
#define TF_FIELD_LME_SPRD_DAY      78     /* LME spread month day */
#define TF_FIELD_LME_SPRD_YEAR     79     /* LME spread month year */
#define TF_FIELD_LME_PRICE         80     /* LME price value */
#define TF_FIELD_LME_BATE_CODE     81     /* LME bate code */
#define TF_FIELD_LME_CONTANGO      82     /* LME contango (to indicate neg. prices) */
#define TF_FIELD_LME_VOLUME        83     /* LME volume */

/*
 *  field definitions for statistic messages
 */
#define TF_FIELD_STAT_FACTOR       84     /* 4 multiplicator for the following values */
#define TF_FIELD_STAT_VALUE        85     /* 4 value */

/*
 *  additional field definitions for LIFFE/LTOM AutoQuote
 */
#define TF_FIELD_AUTO_BID          86     /* 1 autoquote bid */
#define TF_FIELD_AUTO_ASK          87     /* 1 autoquote ask */

/*
 *  additional field definitions for FOREX symbols
 *  quotes without quote qualifier are spot prices
 */
#define TF_FIELD_FRX_BANK          88     /* n bank name */
#define TF_FIELD_FRX_CITY          89     /* n city */
#define TF_FIELD_FRX_BID           90     /* 4 forex bid */
#define TF_FIELD_FRX_ASK           91     /* 4 forex ask */
#define TF_FIELD_FRX_QUAL          92     /* 1 quote qualifier (London am, pm, ...) */

/*
 *  additional field definitions for NASDAQ fund
 */
#define TF_FIELD_FOOTNOTE          93     /* n footnote */
#define TF_FIELD_AVG_MAT           94     /* 4 average maturity */
#define TF_FIELD_7DY_YIELD         95     /* 4 7 days yield */
#define TF_FIELD_EFF_7DY_YIELD     96     /* 4 effective 7 days yield */
#define TF_FIELD_ASSET             97     /* 4 asset */
#define TF_FIELD_NAV               98     /* 4 net asset value */
#define TF_FIELD_OFFER             99     /* 4 offer */
#define TF_FIELD_CAP_GAIN         100     /* 4 capital gain */
#define TF_FIELD_DIVIDEND         101     /* 4 dividend */
#define TF_FIELD_SPL_DIV          102     /* 4 spilt dividend */
#define TF_FIELD_NET_NAV          103     /* 4 NAV net change */
#define TF_FIELD_PREV_NAV         104     /* 4 previous days NAV */
#define TF_FIELD_OPEN_NAV         105     /* 4 beginning NAV */
#define TF_FIELD_CLOSE_NAV        106     /* 4 closing NAV */
/*
 * definition to force change/change(%) recalculation see also TF_FIELD_RECALC_CHANGE
 */
#define TF_FIELD_RECALC_CHANGE    107     /* 0x6b 1 recalculate net change fields () */
#define TF_FIELD_TRADE_COND       108     /* bit flag to updated last/high/low/open/vol/composite */

#define TF_FIELD_BEST_LIMITS      109     // best bid/ask values packet in a binary data block
#define TF_FIELD_CHART_UPDATE     110     // chart update packet in a binary data block

#define TF_FIELD_OPEN_IND         111     /* n open indicator flag */
#define TF_FIELD_HIGH_IND         112     /* n high indicator flag */
#define TF_FIELD_LOW_IND          113     /* n low indicator flag */
#define TF_FIELD_BID_IND          114     /* n bid indicator flag */
#define TF_FIELD_ASK_IND          115     /* n ask indicator flag */

#define TF_FIELD_SUB_MARKET       116     /* 4 sub market */

#define TF_FIELD_CHART_INTERVAL   117     /* 4 chart tick resolution in minutes */

//
// static data specific field definitions
//
#define TF_FIELD_STATIC_AMD          1     /* 1 add (3) / modify (2) / delete (1) flag (see TF_FIELD_AMD_... below) */
#define TF_FIELD_STATIC_ROOT         2     /* n root symbol (for options,...) */
#define TF_FIELD_STATIC_DTB_VER      3     /* 1 DTB version code */
#define TF_FIELD_STATIC_EXP_DAY      4     /* 1 expiration day */
#define TF_FIELD_STATIC_EXP_MONTH    5     /* 1 expiration month */
#define TF_FIELD_STATIC_EXP_YEAR     6     /* 1 expiration year (modulo 100) */
#define TF_FIELD_STATIC_STRIKE       7     /* 4 strike price */
#define TF_FIELD_STATIC_CONSZ        8     /* 4 contract size (shares per contract) */
#define TF_FIELD_STATIC_CURR         9     /* n currency code */
#define TF_FIELD_STATIC_UNITSZ      10     /* 4 unit size */
#define TF_FIELD_STATIC_UNITTP      11     /* n unit type */
#define TF_FIELD_STATIC_ISSNM       12     /* n issuer name */
#define TF_FIELD_STATIC_NAME        TF_FIELD_STATIC_ISSNM     /* company name */
#define TF_FIELD_STATIC_ISSDSC      13     /* n issuer description */
#define TF_FIELD_STATIC_NAME2       TF_FIELD_STATIC_ISSDSC     /* additional company name infoemation */
#define TF_FIELD_STATIC_LTD_DAY     14     /* 1 last trading date day */
#define TF_FIELD_STATIC_LTD_MONTH   15     /* 1 last trading date month */
#define TF_FIELD_STATIC_LTD_YEAR    16     /* 1 last trading date year */
#define TF_FIELD_STATIC_HIGH        17     /* 4 high */
#define TF_FIELD_STATIC_HIGH52      TF_FIELD_STATIC_HIGH  /* 4 52 weeks high */
#define TF_FIELD_STATIC_LOW         18     /* 4 low */
#define TF_FIELD_STATIC_LOW52       TF_FIELD_STATIC_LOW   /* 4 52 weeks low */
#define TF_FIELD_STATIC_ISIN        19     /* n ISIN code */
#define TF_FIELD_STATIC_INSTR_TYPE  20     /* 4 market specific instrument type */
#define TF_FIELD_STATIC_IND_SEC     21     /* n market specific industry sector */
#define TF_FIELD_STATIC_TICK_SZ     30     /* 4 minimum tick size */
#define TF_FIELD_STATIC_CON_NAME    31     /* n contract name */
#define TF_FIELD_STATIC_SNAME       32     /* n short name */
#define TF_FIELD_STATIC_SEDOL       33     /* n sedol code */
#define TF_FIELD_STATIC_ISO         TF_FIELD_STATIC_SEDOL     /* n local instrument code (like sedol,sicovam,wpk,...) */
#define TF_FIELD_STATIC_COUNTRY     34     /* n country */
#define TF_FIELD_STATIC_FTSE        35     /* 0x23 1 FTSE flag 0=no,1=100,2=250,... */
#define TF_FIELD_STATIC_SEC_FLAG    TF_FIELD_STATIC_FTSE    /* market specific security flag (index constituents, special sec sub type) */
#define TF_FIELD_STATIC_MKT_SEG     36     /* 0x24 n market specific segment code (like LSE market segment) */
#define TF_FIELD_STATIC_MKT_SEC     37     /* 0x25 n market specific sector code (like LSE market sector) */
#define TF_FIELD_STATIC_LONGNAME2   38     /* 0x26 n additional companies long name (e.g. for greek name) */
#define TF_FIELD_STATIC_SYMBOL2     39     /* 0x27 n 2nd symbol name (e.g. for greek name) */
#define TF_FIELD_STATIC_SOURCE      40     /* 0x28 n contract source information */
#define TF_FIELD_STATIC_PREV_CURR   41     /* 0x29 n previous currency code (after EURO conversion) */

/*
 * field IDs for the new dynmaic message RTP_TF_DYN_NEW
 * should be all above 128
 */

/*
 * AEX feed specific definitions
 */
#define TF_FIELD_ASSET_BID          128     /* 0x0080  4 asset bid */
#define TF_FIELD_ASSET_BID_SIZE     129     /* 0x0081  4 asset bid size */
#define TF_FIELD_ASSET_BID_MEM      130     /* 0x0082  n asset bid member */
#define TF_FIELD_ASSET_ASK          131     /* 0x0083  4 asset ask */
#define TF_FIELD_ASSET_ASK_SIZE     132     /* 0x0084  4 asset ask size */
#define TF_FIELD_ASSET_ASK_MEM      133     /* 0x0085  n asset ask member */
#define TF_FIELD_LIMBO_BID          134     /* 0x0086  4 limbo bid */
#define TF_FIELD_LIMBO_BID_SIZE     135     /* 0x0087  4 limbo bid size */
#define TF_FIELD_LIMBO_ASK          136     /* 0x0088  4 limbo ask */
#define TF_FIELD_LIMBO_ASK_SIZE     137     /* 0x0089  4 limbo ask size */
#define TF_FIELD_TRADE_IND          138     /* 0x008a  n trade indicator flag */
#define TF_FIELD_EXDIV              139     /* 0x008b  1 ex divident flag */
#define TF_FIELD_THPRICE            140     /* 0x008c  4 theoretical price */

#define TF_FIELD_WVOL               141     /* 0x008d  4 wholesale volume */
#define TF_FIELD_WTRADE_COUNT       142     /* 0x008e  4 # of wholesale trades */
#define TF_FIELD_RVOL               143     /* 0x008f  4 retail volume */
#define TF_FIELD_RTRADE_COUNT       144     /* 0x0090  4 # of retail trades */

#define TF_FIELD_BOOK_BID           145     /* 0x0091  4 AEO book bid */
#define TF_FIELD_BOOK_BID_SIZE      146     /* 0x0092  4 AEO book bid size */
#define TF_FIELD_BOOK_ASK           147     /* 0x0093  4 AEO book ask */
#define TF_FIELD_BOOK_ASK_SIZE      148     /* 0x0094  4 AEO book ask size */

#define TF_FIELD_BEST_BID_COMP      149     /* 0x0095  1 asset bid joint flag */
#define TF_FIELD_BEST_ASK_COMP      150     /* 0x0096  1 asset ask joint flag */
#define TF_FIELD_CLOSE_IND          151     /* 0x0097  n close indicator flag */
#define TF_FIELD_HIST_UV_PRICE      152     /* 0x0098  4 historlical underlining value */
#define TF_FIELD_DAYSTOEXP          153     /* 0x0099  4 days to expiration */
#define TF_FIELD_TRANS_COUNT        154     /* 0x009a  4 total # of transactions */

/*
 * FIM feed specific definitions
 */
#define TF_FIELD_SUSPENDEDTIME      155     /* 0x009b  4 053 0x0035 C 004 Suspended time */
#define TF_FIELD_ASKCOMP_FIXCOURS   156     /* 0x009c  4 060 0x003c F 004 Compensation price  (== FNO_ASKCOMP) */
#define TF_FIELD_ASKREP_FIXASK      157     /* 0x009d  4 061 0x003d F 004 Contango price      (== FNO_ASKREP) */
#define TF_FIELD_RATEREP            158     /* 0x009e  4 062 0x003e T 012 Contango rate */
#define TF_FIELD_INTEREST_RATE      159     /* 0x009f  4 063 0x003f F 004 Interest rate    (== FNO_RATEINT) */
#define TF_FIELD_DEALINCVOL         160     /* 0x00a0  4 083 0x0053 U 004 Trade inc.vol */
#define TF_FIELD_DEALCUMVOL         161     /* 0x00a1  4 084 0x0054 U 004 Trade cum.vol */
#define TF_FIELD_SUSPENDED          162     /* 0x00a2  1 088 0x0058 H 001 Suspended */
#define TF_FIELD_INXCHANGE3         163     /* 0x00a3  4 130 0x0082 F 004 Change 1 year */
#define TF_FIELD_INXCHANGE          164     /* 0x00a4  4 131 0x0083 F 004 Change (opening level) */
#define TF_FIELD_INXSETTLEMENT      165     /* 0x00a5  4 132 0x0084 F 004 Change - settlement */
#define TF_FIELD_INXQUOTESAVAIL     166     /* 0x00a6  4 133 0x0085 I 002 Quoted shares */
#define TF_FIELD_BIDNO1             167     /* 0x00a7  4 134 0x0086 I 002 Bid no. 1 */
#define TF_FIELD_BIDNO2             168     /* 0x00a8  4 135 0x0087 I 002 Bid no. 2 */
#define TF_FIELD_BIDNO3             169     /* 0x00a9  4 136 0x0088 I 002 Bid no. 3 */
#define TF_FIELD_BIDNO4             170     /* 0x00aa  4 137 0x0089 I 002 Bid no. 4 */
#define TF_FIELD_BIDNO5             171     /* 0x00ab  4 138 0x008a I 002 Bid no. 5 */
#define TF_FIELD_BIDNO6             172     /* 0x00ac  4 139 0x008b I 002 Bid no. 6 */
#define TF_FIELD_BIDVOL1            173     /* 0x00ad  4 140 0x008c U 004 Bid vol. 1 */
#define TF_FIELD_BIDVOL2            174     /* 0x00ae  4 141 0x008d U 004 Bid vol. 2 */
#define TF_FIELD_BIDVOL3            175     /* 0x00af  4 142 0x008e U 004 Bid vol. 3 */
#define TF_FIELD_BIDVOL4            176     /* 0x00b0  4 143 0x008f U 004 Bid vol. 4 */
#define TF_FIELD_BIDVOL5            177     /* 0x00b1  4 144 0x0090 U 004 Bid vol. 5 */
#define TF_FIELD_BIDVOL6            178     /* 0x00b2  4 145 0x0091 U 004 Bid vol. 6 */
#define TF_FIELD_BID1               179     /* 0x00b3  4 146 0x0092 F 004 Bid 1 */
#define TF_FIELD_BID2               180     /* 0x00b4  4 147 0x0093 F 004 Bid 2 */
#define TF_FIELD_BID3               181     /* 0x00b5  4 148 0x0094 F 004 Bid 3 */
#define TF_FIELD_BID4               182     /* 0x00b6  4 149 0x0095 F 004 Bid 4 */
#define TF_FIELD_BID5               183     /* 0x00b7  4 150 0x0096 F 004 Bid 5 */
#define TF_FIELD_BID6               184     /* 0x00b8  4 151 0x0097 F 004 Bid 6 */
#define TF_FIELD_ASKNO1             185     /* 0x00b9  4 152 0x0098 I 002 Ask no. 1 */
#define TF_FIELD_ASKNO2             186     /* 0x00ba  4 153 0x0099 I 002 Ask no. 2 */
#define TF_FIELD_ASKNO3             187     /* 0x00bb  4 154 0x009a I 002 Ask no. 3 */
#define TF_FIELD_ASKNO4             188     /* 0x00bc  4 155 0x009b I 002 Ask no. 4 */
#define TF_FIELD_ASKNO5             189     /* 0x00bd  4 156 0x009c I 002 Ask no. 5 */
#define TF_FIELD_ASKNO6             190     /* 0x00be  4 157 0x009d I 002 Ask no. 6 */
#define TF_FIELD_ASKVOL1            191     /* 0x00bf  4 158 0x009e U 004 Ask vol. 1 */
#define TF_FIELD_ASKVOL2            192     /* 0x00c0  4 159 0x009f U 004 Ask vol. 2 */
#define TF_FIELD_ASKVOL3            193     /* 0x00c1  4 160 0x00a0 U 004 Ask vol. 3 */
#define TF_FIELD_ASKVOL4            194     /* 0x00c2  4 161 0x00a1 U 004 Ask vol. 4 */
#define TF_FIELD_ASKVOL5            195     /* 0x00c3  4 162 0x00a2 U 004 Ask vol. 5 */
#define TF_FIELD_ASKVOL6            196     /* 0x00c4  4 163 0x00a3 U 004 Ask vol. 6 */
#define TF_FIELD_ASK1               197     /* 0x00c5  4 164 0x00a4 F 004 Ask 1 */
#define TF_FIELD_ASK2               198     /* 0x00c6  4 165 0x00a5 F 004 Ask 2 */
#define TF_FIELD_ASK3               199     /* 0x00c7  4 166 0x00a6 F 004 Ask 3 */
#define TF_FIELD_ASK4               200     /* 0x00c8  4 167 0x00a7 F 004 Ask 4 */
#define TF_FIELD_ASK5               201     /* 0x00c9  4 168 0x00a8 F 004 Ask 5 */
#define TF_FIELD_ASK6               202     /* 0x00ca  4 169 0x00a9 F 004 Ask 6 */
#define TF_FIELD_ORIGIN             203     /* 0x00cb  1 206 0x00ce H 004 origin market */
#define TF_FIELD_CURRENCYCODE       204     /* 0x00cc  4 323 0x0143 T 004 Currency Code */
#define TF_FIELD_BLOCK_TRADE_TIME   205     /* 0x00cd  4 935 0x03a7 C 004 Block trade time */
#define TF_FIELD_BLOCK_PRICE        206     /* 0x00ce  4 936 0x03a8 F 004 Block price */
#define TF_FIELD_BLOCK_VOLUME       207     /* 0x00cf  4 937 0x03a9 U 004 Block volume */
#define TF_FIELD_FIX_PRICE          208     /* 0x00d0  4 938 0x03aa F 004 Fix price */
#define TF_FIELD_FIX_BID            209     /* 0x00d1  4 939 0x03ab F 004 Fix bid */
#define TF_FIELD_INXVALUE           210     /* 0x00d2  4 101 0x0065 F 004 index value */
#define TF_FIELD_FIX_ASK            211     /* 0x00d3  4 940 0x03ac F 004 Fix ask */
#define TF_FIELD_FOURCHBID          212     /* 0x00d4  4 065 0x0041 F 004 fourchette bid */
#define TF_FIELD_FOURCHASK          213     /* 0x00d5  4 064 0x0040 F 004 fourchette ask */
#define TF_FIELD_LAST_MARKET        214     // 4 origination market of last trade for composite updates

/*
 * SIA feed specific definitions
 */
#define TF_FIELD_OFF_PRICE          216     // 0xD8     official price
#define TF_FIELD_REF_PRICE          217     // 0xD9     reference price (== close for net change calc)

// additional market depth 7-10
#define TF_FIELD_BIDNO7             218     // 0x00DA    Bid no. 7
#define TF_FIELD_BIDNO8             219     // 0x00DB    Bid no. 8
#define TF_FIELD_BIDNO9             220     // 0x00DC    Bid no. 9
#define TF_FIELD_BIDNO10            221     // 0x00DD    Bid no. 10
#define TF_FIELD_BIDVOL7            222     // 0x00DE    Bid vol. 7
#define TF_FIELD_BIDVOL8            223     // 0x00DF    Bid vol. 8
#define TF_FIELD_BIDVOL9            224     // 0x00E0    Bid vol. 9
#define TF_FIELD_BIDVOL10           225     // 0x00E1    Bid vol. 10
#define TF_FIELD_BID7               226     // 0x00E2    Bid 7
#define TF_FIELD_BID8               227     // 0x00E3    Bid 8
#define TF_FIELD_BID9               228     // 0x00E4    Bid 9
#define TF_FIELD_BID10              229     // 0x00E5    Bid 10
#define TF_FIELD_ASKNO7             230     // 0x00E6    Ask no. 7
#define TF_FIELD_ASKNO8             231     // 0x00E7    Ask no. 8
#define TF_FIELD_ASKNO9             232     // 0x00E8    Ask no. 9
#define TF_FIELD_ASKNO10            233     // 0x00E9    Ask no. 10
#define TF_FIELD_ASKVOL7            234     // 0x00EA    Ask vol. 7
#define TF_FIELD_ASKVOL8            235     // 0x00EB    Ask vol. 8
#define TF_FIELD_ASKVOL9            236     // 0x00EC    Ask vol. 9
#define TF_FIELD_ASKVOL10           237     // 0x00ED    Ask vol. 10
#define TF_FIELD_ASK7               238     // 0x00EE    Ask 7
#define TF_FIELD_ASK8               239     // 0x00EF    Ask 8
#define TF_FIELD_ASK9               240     // 0x00F0    Ask 9
#define TF_FIELD_ASK10              241     // 0x00F1    Ask 10

//
// VWAP fields
//
#define TF_FIELD_VWAP_VOL           242     // 0x00F2    VWAP volume (ulong)
#define TF_FIELD_VWAP_PRICE         243     // 0x00F3    VWAP price (frac)

/*
 * reserved field IDs
 */
#define TF_FIELD_LINKSYMBOL        0xfa     /* 250 link symbol (mkt,sec,symbol) */
#define TF_FIELD_SUBSDONGLE        0xfb     /* 251 new subscription data packet in a binary data block */
#define TF_FIELD_DELAYSUBSC        0xfc     /* 252 subscription for delayed data */
#define TF_FIELD_COMMAND           0xfd     /* 253 server command packet in a binary data block */
#define TF_FIELD_FILETX            0xfe     /* 254 file transmission data packet in a binary data block */
#define TF_FIELD_SUBSCRIPTION      0xff     /* 255 subscription data packet in a binary data block */

/*
 *  additional field definitions for new FOREX feed
 */
#define TF_FIELD_FRX_BASE           0x00000100                   // 256 FOREX field id base value
#define TF_FIELD_FRX_BID_ON         (TF_FIELD_FRX_BASE + 0x00)   // 256 Overnight Bid
#define TF_FIELD_FRX_ASK_ON         (TF_FIELD_FRX_BASE + 0x01)   // 257 Overnight Ask
#define TF_FIELD_FRX_BID_SN         (TF_FIELD_FRX_BASE + 0x02)   // 258 Spot/Next Bid
#define TF_FIELD_FRX_ASK_SN         (TF_FIELD_FRX_BASE + 0x03)   // 259 Spot/Next Ask
#define TF_FIELD_FRX_BID_TN         (TF_FIELD_FRX_BASE + 0x04)   // 260 Tommorow/Next Bid
#define TF_FIELD_FRX_ASK_TN         (TF_FIELD_FRX_BASE + 0x05)   // 261 Tommorow/Next Ask
#define TF_FIELD_FRX_BID_1W         (TF_FIELD_FRX_BASE + 0x06)   // 262 1 Week Bid
#define TF_FIELD_FRX_ASK_1W         (TF_FIELD_FRX_BASE + 0x07)   // 263 1 Week Ask
#define TF_FIELD_FRX_BID_2W         (TF_FIELD_FRX_BASE + 0x08)   // 264 2 Weeks Bid
#define TF_FIELD_FRX_ASK_2W         (TF_FIELD_FRX_BASE + 0x09)   // 265 2 Weeks Ask
#define TF_FIELD_FRX_BID_3W         (TF_FIELD_FRX_BASE + 0x0A)   // 266 3 Weeks Bid
#define TF_FIELD_FRX_ASK_3W         (TF_FIELD_FRX_BASE + 0x0B)   // 267 3 Weeks Ask
#define TF_FIELD_FRX_BID_1M         (TF_FIELD_FRX_BASE + 0x0C)   // 268 1 Month Bid
#define TF_FIELD_FRX_ASK_1M         (TF_FIELD_FRX_BASE + 0x0D)   // 269 1 Month Ask
#define TF_FIELD_FRX_BID_2M         (TF_FIELD_FRX_BASE + 0x0E)   // 270 2 Months Bid
#define TF_FIELD_FRX_ASK_2M         (TF_FIELD_FRX_BASE + 0x0F)   // 271 2 Months Ask
#define TF_FIELD_FRX_BID_3M         (TF_FIELD_FRX_BASE + 0x10)   // 272 3 Months Bid
#define TF_FIELD_FRX_ASK_3M         (TF_FIELD_FRX_BASE + 0x11)   // 273 3 Months Ask
#define TF_FIELD_FRX_BID_4M         (TF_FIELD_FRX_BASE + 0x12)   // 274 4 Months Bid
#define TF_FIELD_FRX_ASK_4M         (TF_FIELD_FRX_BASE + 0x13)   // 275 4 Months Ask
#define TF_FIELD_FRX_BID_5M         (TF_FIELD_FRX_BASE + 0x14)   // 276 5 Months Bid
#define TF_FIELD_FRX_ASK_5M         (TF_FIELD_FRX_BASE + 0x15)   // 277 5 Months Ask
#define TF_FIELD_FRX_BID_6M         (TF_FIELD_FRX_BASE + 0x16)   // 278 6 Months Bid
#define TF_FIELD_FRX_ASK_6M         (TF_FIELD_FRX_BASE + 0x17)   // 279 6 Months Ask
#define TF_FIELD_FRX_BID_9M         (TF_FIELD_FRX_BASE + 0x18)   // 280 9 Months Bid
#define TF_FIELD_FRX_ASK_9M         (TF_FIELD_FRX_BASE + 0x19)   // 281 9 Months Ask
#define TF_FIELD_FRX_BID_1Y         (TF_FIELD_FRX_BASE + 0x1A)   // 282 1 Year Bid
#define TF_FIELD_FRX_ASK_1Y         (TF_FIELD_FRX_BASE + 0x1B)   // 283 1 Year Ask
#define TF_FIELD_FRX_BID_2Y         (TF_FIELD_FRX_BASE + 0x1C)   // 284 2 Years Bid
#define TF_FIELD_FRX_ASK_2Y         (TF_FIELD_FRX_BASE + 0x1D)   // 285 2 Years Ask
#define TF_FIELD_FRX_BID_3Y         (TF_FIELD_FRX_BASE + 0x1E)   // 286 3 Years Bid
#define TF_FIELD_FRX_ASK_3Y         (TF_FIELD_FRX_BASE + 0x1F)   // 287 3 Years Ask
#define TF_FIELD_FRX_BID_4Y         (TF_FIELD_FRX_BASE + 0x20)   // 288 4 Years Bid
#define TF_FIELD_FRX_ASK_4Y         (TF_FIELD_FRX_BASE + 0x21)   // 289 4 Years Ask
#define TF_FIELD_FRX_BID_5Y         (TF_FIELD_FRX_BASE + 0x22)   // 290 5 Years Bid
#define TF_FIELD_FRX_ASK_5Y         (TF_FIELD_FRX_BASE + 0x23)   // 291 5 Years Ask
#define TF_FIELD_FRX_BID_7M         (TF_FIELD_FRX_BASE + 0x24)   // 292 7 Months Bid
#define TF_FIELD_FRX_ASK_7M         (TF_FIELD_FRX_BASE + 0x25)   // 293 7 Months Ask
#define TF_FIELD_FRX_BID_8M         (TF_FIELD_FRX_BASE + 0x26)   // 294 8 Months Bid
#define TF_FIELD_FRX_ASK_8M         (TF_FIELD_FRX_BASE + 0x27)   // 295 8 Months Ask
#define TF_FIELD_FRX_BID_TM         (TF_FIELD_FRX_BASE + 0x28)   // 296 10 Months Bid
#define TF_FIELD_FRX_ASK_TM         (TF_FIELD_FRX_BASE + 0x29)   // 297 10 Months Ask
#define TF_FIELD_FRX_BID_EM         (TF_FIELD_FRX_BASE + 0x2A)   // 298 11 Months Bid
#define TF_FIELD_FRX_ASK_EM         (TF_FIELD_FRX_BASE + 0x2B)   // 299 11 Months Ask
#define TF_FIELD_FRX_BID_6Y         (TF_FIELD_FRX_BASE + 0x2C)   // 300 6 Years Bid
#define TF_FIELD_FRX_ASK_6Y         (TF_FIELD_FRX_BASE + 0x2D)   // 301 6 Years Ask
#define TF_FIELD_FRX_BID_7Y         (TF_FIELD_FRX_BASE + 0x2E)   // 302 7 Years Bid
#define TF_FIELD_FRX_ASK_7Y         (TF_FIELD_FRX_BASE + 0x2F)   // 303 7 Years Ask
#define TF_FIELD_FRX_BID_TY         (TF_FIELD_FRX_BASE + 0x30)   // 304 10 Years Bid
#define TF_FIELD_FRX_ASK_TY         (TF_FIELD_FRX_BASE + 0x31)   // 305 10 Years Ask

#define TF_FIELD_MET_BID_LA         (TF_FIELD_FRX_BASE + 0x32)   // 306 London AM Fixing
#define TF_FIELD_MET_ASK_LA         (TF_FIELD_FRX_BASE + 0x33)   // 307 London AM Fixing
#define TF_FIELD_MET_BID_1M         (TF_FIELD_FRX_BASE + 0x34)   // 308 1 Month Forward
#define TF_FIELD_MET_ASK_1M         (TF_FIELD_FRX_BASE + 0x35)   // 309 1 Month Forward
#define TF_FIELD_MET_BID_LP         (TF_FIELD_FRX_BASE + 0x36)   // 310 London PM Fixing
#define TF_FIELD_MET_ASK_LP         (TF_FIELD_FRX_BASE + 0x37)   // 311 London PM Fixing
#define TF_FIELD_MET_BID_2M         (TF_FIELD_FRX_BASE + 0x38)   // 312 2 Month Forward
#define TF_FIELD_MET_ASK_2M         (TF_FIELD_FRX_BASE + 0x39)   // 313 2 Month Forward
#define TF_FIELD_MET_BID_LC         (TF_FIELD_FRX_BASE + 0x3A)   // 314 London Close
#define TF_FIELD_MET_ASK_LC         (TF_FIELD_FRX_BASE + 0x3B)   // 315 London Close
#define TF_FIELD_MET_BID_3M         (TF_FIELD_FRX_BASE + 0x3C)   // 316 3 Month Forward
#define TF_FIELD_MET_ASK_3M         (TF_FIELD_FRX_BASE + 0x3D)   // 317 3 Month Forward
#define TF_FIELD_MET_BID_LF         (TF_FIELD_FRX_BASE + 0x3E)   // 318 London Fixing
#define TF_FIELD_MET_ASK_LF         (TF_FIELD_FRX_BASE + 0x3F)   // 319 London Fixing
#define TF_FIELD_MET_BID_6M         (TF_FIELD_FRX_BASE + 0x40)   // 320 6 Month Forward
#define TF_FIELD_MET_ASK_6M         (TF_FIELD_FRX_BASE + 0x41)   // 321 6 Month Forward
#define TF_FIELD_MET_BID_LO         (TF_FIELD_FRX_BASE + 0x42)   // 322 London Open
#define TF_FIELD_MET_ASK_LO         (TF_FIELD_FRX_BASE + 0x43)   // 323 London Open
#define TF_FIELD_MET_BID_1Y         (TF_FIELD_FRX_BASE + 0x44)   // 324 1 Year Forward
#define TF_FIELD_MET_ASK_1Y         (TF_FIELD_FRX_BASE + 0x45)   // 325 1 Year Forward
#define TF_FIELD_MET_BID_NC         (TF_FIELD_FRX_BASE + 0x46)   // 326 New York Close
#define TF_FIELD_MET_ASK_NC         (TF_FIELD_FRX_BASE + 0x47)   // 327 New York Close
#define TF_FIELD_MET_BID_3X         (TF_FIELD_FRX_BASE + 0x48)   // 328 3 Month Forward Fixing
#define TF_FIELD_MET_ASK_3X         (TF_FIELD_FRX_BASE + 0x49)   // 329 3 Month Forward Fixing
#define TF_FIELD_MET_BID_SC         (TF_FIELD_FRX_BASE + 0x4A)   // 330 Sydney Close
#define TF_FIELD_MET_ASK_SC         (TF_FIELD_FRX_BASE + 0x4C)   // 331 Sydney Close
#define TF_FIELD_MET_BID_6X         (TF_FIELD_FRX_BASE + 0x4B)   // 332 6 Month Forward Fixing
#define TF_FIELD_MET_ASK_6X         (TF_FIELD_FRX_BASE + 0x4D)   // 333 6 Month Forward Fixing
#define TF_FIELD_MET_BID_SO         (TF_FIELD_FRX_BASE + 0x4E)   // 334 Sydney Open
#define TF_FIELD_MET_ASK_SO         (TF_FIELD_FRX_BASE + 0x4F)   // 335 Sydney Open
#define TF_FIELD_MET_BID_YX         (TF_FIELD_FRX_BASE + 0x50)   // 336 1 Year Forward Fixing
#define TF_FIELD_MET_ASK_YX         (TF_FIELD_FRX_BASE + 0x51)   // 337 1 Year Forward Fixing
#define TF_FIELD_MET_BID_ZC         (TF_FIELD_FRX_BASE + 0x52)   // 338 Zurich Close
#define TF_FIELD_MET_ASK_ZC         (TF_FIELD_FRX_BASE + 0x53)   // 349 Zurich Close
#define TF_FIELD_MET_BID_FX         (TF_FIELD_FRX_BASE + 0x54)   // 340 Fixing
#define TF_FIELD_MET_ASK_FX         (TF_FIELD_FRX_BASE + 0x55)   // 341 Fixing
#define TF_FIELD_MET_BID_ZF         (TF_FIELD_FRX_BASE + 0x56)   // 342 Zurich Fixing
#define TF_FIELD_MET_ASK_ZF         (TF_FIELD_FRX_BASE + 0x57)   // 343 Zurich Fixing
#define TF_FIELD_MET_BID_FF         (TF_FIELD_FRX_BASE + 0x58)   // 344 Frankfurt Fixing
#define TF_FIELD_MET_ASK_FF         (TF_FIELD_FRX_BASE + 0x59)   // 345 Frankfurt Fixing
#define TF_FIELD_MET_BID_ZP         (TF_FIELD_FRX_BASE + 0x5A)   // 346 Zurich Metal Pool
#define TF_FIELD_MET_ASK_ZP         (TF_FIELD_FRX_BASE + 0x5B)   // 347 Zurich Metal Pool
#define TF_FIELD_MET_BID_HC         (TF_FIELD_FRX_BASE + 0x5C)   // 348 Hong Kong Close
#define TF_FIELD_MET_ASK_HC         (TF_FIELD_FRX_BASE + 0x5D)   // 349 Hong Kong Close

#define TF_FIELD_FRX_ASK_HIGH       (TF_FIELD_FRX_BASE + 0x5E)   // 350 ask high
#define TF_FIELD_FRX_ASK_LOW        (TF_FIELD_FRX_BASE + 0x5F)   // 351 ask low
#define TF_FIELD_FRX_ASK_CLOSE      (TF_FIELD_FRX_BASE + 0x60)   // 352 ask close
#define TF_FIELD_FRX_MID_HIGH       (TF_FIELD_FRX_BASE + 0x61)   // 353 mid high
#define TF_FIELD_FRX_MID_LOW        (TF_FIELD_FRX_BASE + 0x62)   // 354 mid low

/*
 *  additional field definitions for new LSE Level 2 feed
 */
#define TF_FIELD_LSE_BASE           0x00000180                   // 384 LSE Level 2 field id base value
#define TF_FIELD_LSE_BID_NO         (TF_FIELD_LSE_BASE + 0x00)   // 384 u number of orders at best bid
#define TF_FIELD_LSE_ASK_NO         (TF_FIELD_LSE_BASE + 0x01)   // 385 u number of orders at best ask
#define TF_FIELD_LSE_BID_VNE        (TF_FIELD_LSE_BASE + 0x02)   // 386 u Value Not Executed Buy Side (div. by 100)
#define TF_FIELD_LSE_ASK_VNE        (TF_FIELD_LSE_BASE + 0x03)   // 387 u Value Not Executed Sell Side (div. by 100)
#define TF_FIELD_LSE_MFLOW          (TF_FIELD_LSE_BASE + 0x04)   // 388 u Money Flow Value Executed (div. by 100)
#define TF_FIELD_LSE_OB_MFLOW       (TF_FIELD_LSE_BASE + 0x05)   // 389 u Off Book Money Flow Value Executed (div. by 100)
#define TF_FIELD_LSE_BID_PERC       (TF_FIELD_LSE_BASE + 0x06)   // 390 f Buy Percentage
#define TF_FIELD_LSE_BID_VOL        (TF_FIELD_LSE_BASE + 0x07)   // 391 u Buy Volume (total)
#define TF_FIELD_LSE_ASK_PERC       (TF_FIELD_LSE_BASE + 0x08)   // 392 f Sell Percentage
#define TF_FIELD_LSE_ASK_VOL        (TF_FIELD_LSE_BASE + 0x09)   // 393 u Sell Volume (total)
#define TF_FIELD_LSE_MID_HIGH       (TF_FIELD_LSE_BASE + 0x0A)   // 394 f Mid High
#define TF_FIELD_LSE_MID_HIGH_TM    (TF_FIELD_LSE_BASE + 0x0B)   // 395 u Mid High Time (min of day)
#define TF_FIELD_LSE_MID_LOW        (TF_FIELD_LSE_BASE + 0x0C)   // 396 f Mid Low
#define TF_FIELD_LSE_MID_LOW_TM     (TF_FIELD_LSE_BASE + 0x0D)   // 397 u Mid Low Time (min of day)
#define TF_FIELD_LSE_UNC_SIZE       (TF_FIELD_LSE_BASE + 0x0E)   // 398 u Uncrossing volume
#define TF_FIELD_LSE_VWAP           (TF_FIELD_LSE_BASE + 0x0F)   // 399 f VWAP from trades
#define TF_FIELD_LSE_AUTO_VWAP      (TF_FIELD_LSE_BASE + 0x10)   // 400 f VWAP from automatic trades
#define TF_FIELD_LSE_DEPTH_AMD      (TF_FIELD_LSE_BASE + 0x11)   // 401 u add/mod/del flag (see TF_FIELD_AMD... definitions)
#define TF_FIELD_LSE_DEPTH_SIDE     (TF_FIELD_LSE_BASE + 0x12)   // 402 u B=bid, A=ask
#define TF_FIELD_LSE_DEPTH_POS      (TF_FIELD_LSE_BASE + 0x13)   // 403 u position
#define TF_FIELD_LSE_DEPTH_PRICE    (TF_FIELD_LSE_BASE + 0x14)   // 404 f price
#define TF_FIELD_LSE_DEPTH_SIZE     (TF_FIELD_LSE_BASE + 0x15)   // 405 u size
#define TF_FIELD_LSE_DEPTH_NO       (TF_FIELD_LSE_BASE + 0x16)   // 406 u # of quotes at given price
#define TF_FIELD_LSE_DEPTH_MM       (TF_FIELD_LSE_BASE + 0x17)   // 407 s market maker code
#define TF_FIELD_LSE_PERIOD         (TF_FIELD_LSE_BASE + 0x18)   // 408 s trading period name
#define TF_FIELD_LSE_DEPTH_ID       (TF_FIELD_LSE_BASE + 0x19)   // 409 u depth entry id (unique per symbol and side)
#define TF_FIELD_LSE_AVOL           (TF_FIELD_LSE_BASE + 0x1a)   // 410 u cum volume of automatic trades
#define TF_FIELD_LSE_NVOL           (TF_FIELD_LSE_BASE + 0x1b)   // 411 u cum volume of non-automatic trades
#define TF_FIELD_LSE_TWAS           (TF_FIELD_LSE_BASE + 0x1c)   // 412 f time weighted average spread
#define TF_FIELD_LSE_PERIOD_LEN     (TF_FIELD_LSE_BASE + 0x1d)   // 413 u period length of TWAS
#define TF_FIELD_LSE_DEPTH_TIME     (TF_FIELD_LSE_BASE + 0x1e)   // 414 u creation timestamp of depth entry (min of day)
#define TF_FIELD_LSE_DEPTH_DATE     (TF_FIELD_LSE_BASE + 0x1f)   // 415 u creation date of depth entry (julian format)

/*
 * additional static fields
 */
#define TF_FIELD_STATIC_BASE         0x00000200                      // 512 static field id base value
#define TF_FIELD_STATIC_SHARES       (TF_FIELD_STATIC_BASE + 0x00)   // 512 common shares
#define TF_FIELD_STATIC_PFSHARES     (TF_FIELD_STATIC_BASE + 0x01)   // 513 prefered shares
#define TF_FIELD_STATIC_RANK         (TF_FIELD_STATIC_BASE + 0x02)   // 514 ranking (S&P ranking)
#define TF_FIELD_STATIC_FYEM         (TF_FIELD_STATIC_BASE + 0x03)   // 515 fiscal year end month (1=jan,...)
#define TF_FIELD_STATIC_52HIGH       (TF_FIELD_STATIC_BASE + 0x04)   // 516 52 week high price
#define TF_FIELD_STATIC_52HDATE      (TF_FIELD_STATIC_BASE + 0x05)   // 517 52 week high price date (julian)
#define TF_FIELD_STATIC_52LOW        (TF_FIELD_STATIC_BASE + 0x06)   // 518 52 week low price
#define TF_FIELD_STATIC_52LDATE      (TF_FIELD_STATIC_BASE + 0x07)   // 519 52 week low price date (julian)
#define TF_FIELD_STATIC_PE           (TF_FIELD_STATIC_BASE + 0x08)   // 520 P/E (== KGV)
#define TF_FIELD_STATIC_DIV          (TF_FIELD_STATIC_BASE + 0x09)   // 521 last dividend (annual)
#define TF_FIELD_STATIC_DIVDATE      (TF_FIELD_STATIC_BASE + 0x0A)   // 522 last dividend date
#define TF_FIELD_STATIC_NDIV         (TF_FIELD_STATIC_BASE + 0x0B)   // 523 next dividend
#define TF_FIELD_STATIC_NDIVDATE     (TF_FIELD_STATIC_BASE + 0x0C)   // 524 next dividend date
#define TF_FIELD_STATIC_DIV_CHEX     (TF_FIELD_STATIC_BASE + 0x0D)   // 525 cash ex dividend
#define TF_FIELD_STATIC_DIV_SP       (TF_FIELD_STATIC_BASE + 0x0E)   // 526 special dividend
#define TF_FIELD_STATIC_SPLIT        (TF_FIELD_STATIC_BASE + 0x0F)   // 527 split ratio
#define TF_FIELD_STATIC_SPLITDATE    (TF_FIELD_STATIC_BASE + 0x10)   // 528 last split date
#define TF_FIELD_STATIC_EPS          (TF_FIELD_STATIC_BASE + 0x11)   // 529 earnings per share (current)
#define TF_FIELD_STATIC_EPS_ESTIM    (TF_FIELD_STATIC_BASE + 0x12)   // 530 earnings per share (estim.)
#define TF_FIELD_STATIC_EPS_INTER    (TF_FIELD_STATIC_BASE + 0x13)   // 531 earnings per share interim
#define TF_FIELD_STATIC_EPS_5YRGR    (TF_FIELD_STATIC_BASE + 0x14)   // 532 earnings per share 5 years growth
#define TF_FIELD_STATIC_EPS_LASTFY   (TF_FIELD_STATIC_BASE + 0x15)   // 533 earnings per share last fiscal year
#define TF_FIELD_STATIC_DEBT         (TF_FIELD_STATIC_BASE + 0x16)   // 534 debt to equity ratio
#define TF_FIELD_STATIC_YIELD        (TF_FIELD_STATIC_BASE + 0x17)   // 535 yield
#define TF_FIELD_STATIC_BETA         (TF_FIELD_STATIC_BASE + 0x18)   // 536 volatility beta
#define TF_FIELD_STATIC_V30          (TF_FIELD_STATIC_BASE + 0x19)   // 537 volatility V30 (TPF only)
#define TF_FIELD_STATIC_V250         (TF_FIELD_STATIC_BASE + 0x1A)   // 538 volatility V250 (TPF only)
#define TF_FIELD_STATIC_K30          (TF_FIELD_STATIC_BASE + 0x1B)   // 539 volatility K30 (TPF only)
#define TF_FIELD_STATIC_K250         (TF_FIELD_STATIC_BASE + 0x1C)   // 540 volatility K250 (TPF only)
#define TF_FIELD_STATIC_DIP          (TF_FIELD_STATIC_BASE + 0x1D)   // 541 DIP
#define TF_FIELD_STATIC_SRIGHT       (TF_FIELD_STATIC_BASE + 0x1E)   // 542 last subscription right
#define TF_FIELD_STATIC_SRIGHT_DATE  (TF_FIELD_STATIC_BASE + 0x1F)   // 543 last subscription right date
#define TF_FIELD_STATIC_SRIGHT_RATIO (TF_FIELD_STATIC_BASE + 0x20)   // 544 subscription ratio
#define TF_FIELD_STATIC_MAT_FROM     (TF_FIELD_STATIC_BASE + 0x21)   // 545 maturity start date
#define TF_FIELD_STATIC_MAT_TO       (TF_FIELD_STATIC_BASE + 0x22)   // 546 maturity end date
#define TF_FIELD_STATIC_INT_RATE     (TF_FIELD_STATIC_BASE + 0x23)   // 547 interest rate
#define TF_FIELD_STATIC_INT_DUE      (TF_FIELD_STATIC_BASE + 0x24)   // 548 interest due date
#define TF_FIELD_STATIC_CAPITAL      (TF_FIELD_STATIC_BASE + 0x25)   // 549 capitalization
#define TF_FIELD_STATIC_IPRICE       (TF_FIELD_STATIC_BASE + 0x26)   // 550 issue price
#define TF_FIELD_STATIC_INT_PERIOD   (TF_FIELD_STATIC_BASE + 0x27)   // 551 interest rate period
#define TF_FIELD_STATIC_SPRICE       (TF_FIELD_STATIC_BASE + 0x28)   // 552 next subscription price
#define TF_FIELD_STATIC_SPRICE_DATE  (TF_FIELD_STATIC_BASE + 0x29)   // 553 next subscription price date
#define TF_FIELD_STATIC_RPRICE       (TF_FIELD_STATIC_BASE + 0x2A)   // 554 repayment price
#define TF_FIELD_STATIC_START_DATE   (TF_FIELD_STATIC_BASE + 0x2B)   // 555 listing start date
#define TF_FIELD_STATIC_END_DATE     (TF_FIELD_STATIC_BASE + 0x2C)   // 556 listing end date
#define TF_FIELD_STATIC_NOM_PRICE    (TF_FIELD_STATIC_BASE + 0x2D)   // 557 nominal price
#define TF_FIELD_STATIC_ADJ_PRICE    (TF_FIELD_STATIC_BASE + 0x2E)   // 558 adjusted price
#define TF_FIELD_STATIC_COUPON_NO    (TF_FIELD_STATIC_BASE + 0x2F)   // 559 coupon number
#define TF_FIELD_STATIC_LIMIT_HI     (TF_FIELD_STATIC_BASE + 0x30)   // 560 high limit
#define TF_FIELD_STATIC_LIMIT_LO     (TF_FIELD_STATIC_BASE + 0x31)   // 561 low limit
#define TF_FIELD_STATIC_PERFORMANCE  (TF_FIELD_STATIC_BASE + 0x32)   // 562 fund performance in percent

#define TF_FIELD_STATIC_CLEAR_ISIN   (TF_FIELD_STATIC_BASE + 0x33)   // 563 s clearing ISIN (Euronext)

#define TF_FIELD_STATIC_NMS          (TF_FIELD_STATIC_BASE + 0x34)   // 564 u LSE Normal Market Size
#define TF_FIELD_STATIC_EX_CODE      (TF_FIELD_STATIC_BASE + 0x35)   // 565 s LSE ex-market code
#define TF_FIELD_STATIC_EX_START     (TF_FIELD_STATIC_BASE + 0x36)   // 566 s LSE ex-market start date
#define TF_FIELD_STATIC_EX_END       (TF_FIELD_STATIC_BASE + 0x37)   // 567 s LSE ex-market end date


/*
 * fields reserved for Tenfore internal usage
 */
#define TF_FIELD_STATIC_META_ID            0x0f00 /* 3840 n meta ID (used for meta futures) */
#define TF_FIELD_STATIC_UNDERLYING_PAPER   0x0f01 /* 3841 n underlying paper name  */
#define TF_FIELD_STATIC_COMPARE_FIELD      0x0f02 /* 3842 n compare field */
#define TF_FIELD_STATIC_BOX_ID             0x0f03 /* 3843 n BoxId in keytree */
#define TF_FIELD_STATIC_AXXESS_KEY         0x0f04 /* 3844 n AxxessKey of instrument */
#define TF_FIELD_STATIC_FACTOR             0x0f05 /* 3845 n multiplicator added to system info field */
#define TF_FIELD_STATIC_UI_TYPE            0x0f06 /* 3846 n underl. instr. type */
#define TF_FIELD_STATIC_META_ID_GROUP      0x0f07 /* 3847 n met id group */

/*
 *  return code field ids for ClientApi dbase request
 *
 *	LOWORD - running number for dbase records
 *  HIWORD - additional flags (see CAPI_MSG_...)
 */
#define CAPI_MSG_LAST_RECORD    0x00010000        /* identifies the last record */
#define CAPI_MSG_NO_DATA        0x00020000        /* no records available */

#define TF_FIELD_CAPI_MSG                  0x0f08 /* 3848 4 ClientApi dbase request return value

/*
 * Forex quote qualifiers
 * (they come together with a forex bid/ask pair to specify which quotes to update)
 */
#define TF_FRX_QUAL_LDN_AM    'o'     /* London am */
#define TF_FRX_QUAL_LDN_PM    'T'     /* London PM */
#define TF_FRX_QUAL_LDN_FIX   'U'     /* London FIX */
#define TF_FRX_QUAL_ZUR_PL    'W'     /* Zurich pool  */
#define TF_FRX_QUAL_1MNTH     'A'     /* 1 month */
#define TF_FRX_QUAL_2MNTH     'B'     /* 2 month */
#define TF_FRX_QUAL_3MNTH     'C'     /* 3 month */
#define TF_FRX_QUAL_6MNTH     'F'     /* 6 month */
#define TF_FRX_QUAL_ZUR_CL    'L'     /* Zurich close */
#define TF_FRX_QUAL_HK_CL     'M'     /* Honkong close */
#define TF_FRX_QUAL_NY_CL     'N'     /* New York close */
#define TF_FRX_QUAL_ZUR_FIX   'P'     /* Zurich fix */

//
// field definitions for the TIC card status record
// must not be transmitted to the end user
//
#define TF_FIELD_TOTALS_ERROR_COUNT      100   // total # of bad blocks (including CRCs and invalid)
#define TF_FIELD_TOTALS_BLOCK_COUNT      101   // # of received blocks
#define TF_FIELD_TOTALS_MAX_BUFF_USED    102   // # of overflows
#define TF_FIELD_TOTALS_MAX_SCOUNT       103   // reserved
#define TF_FIELD_TOTALS_MAX_RCOUNT       104   // max # of used TIC - PC buffers (on TIC card)
#define TF_FIELD_TOTALS_HASHS            105   // stock symbols in TIC hash tables
#define TF_FIELD_TOTALS_HASHO            106   // options in TIC hash tables
#define TF_FIELD_TOTALS_HASHC            107   // futures in TIC hash tables
#define TF_FIELD_TOTALS_HASHX            108   // forex symbols in TIC hash tables
#define TF_FIELD_TOTALS_MEM              109   // free memory on TIC card
#define TF_FIELD_TOTALS_FRAME_ERRORS     110   // # of received CRC errors
#define TF_FIELD_TOTALS_UPTIME           111   // runtime of the TICDEC program in minutes
#define TF_FIELD_TOTALS_MAX_REC_SIZE     112   // # of available TIC - PC buffers
#define TF_FIELD_TOTALS_CUR_REC_SIZE     113   // # of TIC - PC buffers currently in use
#define TF_FIELD_TOTALS_VERSION          114   // version number of the TICDEC program (display it in hex format)
#define TF_FIELD_TOTALS_DROP_REC         115   // # of dropped records in the TIC (buffer overflow)

#define TF_FIELD_TEXT                    TF_FIELD_ERROR_TEXT     /* free format text */

/*
 * definitions for the message types
 */
#define TF_MSG_TYPE_UNDEF        0        // undefined
#define TF_MSG_TYPE_TRADE        1        // trades
#define TF_MSG_TYPE_QUOTE        2        // bid/ask
#define TF_MSG_TYPE_RECAP        3        // refresh
#define TF_MSG_TYPE_ADMIN        4        // administration (in text format)
#define TF_MSG_TYPE_CONTROL      5        // control messages (reserved)
#define TF_MSG_TYPE_STATIC       6        // fundamental data
#define TF_MSG_TYPE_DYNAMIC      7        // dynamic size messages
#define TF_MSG_TYPE_OTHERS       8        // other messages
#define TF_MSG_TYPE_CLOSE        9        // closing message
#define TF_MSG_TYPE_NEWS        10        // news message
#define TF_MSG_TYPE_CHART		13		  // chart update (msg 11 and 12 are used internal)

/*
 * news messages sub types (nSecurityType field contains value)
 */
#define TF_MSG_NEWS_MOS          0        // mid of story (story line)
#define TF_MSG_NEWS_SOS          1        // start of story (header)
#define TF_MSG_NEWS_EOS          2        // end of story

/*
 * administration messages sub types (nSecurityType field contains value)
 */
#define TF_MSG_ADMIN_UNDEF        0        // undefined admin message
#define TF_MSG_ADMIN_ERROR        1        // error from TIC card (see TF_FIELD_ERROR_.. fields)
#define TF_MSG_ADMIN_TIME         2        // time and date message (see TF_FIELD_TIME_.. fields)
#define TF_MSG_ADMIN_TOTALS       3        // TIC status message (see TF_FIELD_TOTALS_.. fields)

/*
 * definitions for the security types
 */
#define TF_SEC_TYPE_UNDEF       0         // undefined security type
#define TF_SEC_TYPE_STOCK       1         // stocks
#define TF_SEC_TYPE_OPTION      2         // options
#define TF_SEC_TYPE_FUTURE      3         // commodities/futures
#define TF_SEC_TYPE_FUT_OPT     4         // commodity options
#define TF_SEC_TYPE_SPOT        5         // spots
#define TF_SEC_TYPE_SPECS       6         // contract specs (was reserved)
#define TF_SEC_TYPE_CORP_BOND   7         // corporate bonds
#define TF_SEC_TYPE_MUT_FUND    8         // mutual funds
#define TF_SEC_TYPE_GOV_BOND    9         // government bonds
#define TF_SEC_TYPE_INDEX      10         // indices
#define TF_SEC_TYPE_MUN_BOND   11         // municipial bonds
#define TF_SEC_TYPE_NEWS       12         // news
#define TF_SEC_TYPE_RES2       13         // reserved
#define TF_SEC_TYPE_STAT       14         // statistic symbols
#define TF_SEC_TYPE_MON_FUND   15         // monetary funds
#define TF_SEC_TYPE_UNSP_BOND  16         // unspecified bonds
#define TF_SEC_TYPE_UNSP_FUND  17         // unspecified funds
#define TF_SEC_TYPE_MISC       18         // miscelaneous securities
#define TF_SEC_TYPE_MON_MKT    19         // money market
#define TF_SEC_TYPE_RES3       19         // for compatibility
#define TF_SEC_TYPE_FOREX      20         // forex symbols

/*
 * definitions for the exchange IDs (and also market centers)
 */
#define TF_EXCHG_ID_CTS             0  /* CTS (consolidated trade system) high speed line */
#define TF_EXCHG_ID_CQS             0  /* CQS (consolidated quote system) high speed line */
#define TF_EXCHG_ID_AMEX            1  /* AMEX, american stock exchange */
#define TF_EXCHG_ID_AMEX_N          4  /* AMEX (listed on NASDAQ) */
#define TF_EXCHG_ID_BSE             2  /* BSE, boston stock exchange */
#define TF_EXCHG_ID_BSE_N           5  /* BSE (listed on NASDAQ) */
#define TF_EXCHG_ID_OPRA            3  /* OPRA */
#define TF_EXCHG_ID_CBOE            3  /* chicago board option exchange */
#define TF_EXCHG_ID_EAG             7  /* symbols generated by EAG (like INDU,UTIL,...) */
#define TF_EXCHG_ID_TENFORE        TF_EXCHG_ID_EAG /* for compatibility */
#define TF_EXCHG_ID_ASIA_IND       11  /* Asia/Pacific Indices */
#define TF_EXCHG_ID_MSE            13  /* MSE, midwest (chicago) stock exchange */
#define TF_EXCHG_ID_MSE_N           6  /* MSE, (listed on NASDAQ) */
#define TF_EXCHG_ID_NYSE           14  /* NYSE, new york stock exchange */
#define TF_EXCHG_ID_NYSE_N          8  /* NYSE, (listed on NASDAQ) */
#define TF_EXCHG_ID_INST           15  /* Island ECN */
#define TF_EXCHG_ID_INST_N         10  /* Island ECN (listed on NASDAQ) */
#define TF_EXCHG_ID_PSE            16  /* PSE, pacific stock exchange */
#define TF_EXCHG_ID_PSE_N          12  /* PSE, (listed on NASDAQ) */
#define TF_EXCHG_ID_SCAP           17  /* NTDS NASDAQ small cap stock market */
#define TF_EXCHG_ID_ADF            18  /* NASDAQ ADF (Alternative Display Facility) */
#define TF_EXCHG_ID_NASDAQ         19  /* NTDS NASDAQ stock market */
#define TF_EXCHG_ID_NASDAQ_C       29  /* NASDAQ on CTS/CQS */
#define TF_EXCHG_ID_NASD           20  /* for compatibility */
#define TF_EXCHG_ID_CTS_OTC        20  /* CTS OTC equities */
#define TF_EXCHG_ID_NASD_FUNDS     21  /* NASDAQ funds */
#define TF_EXCHG_ID_NONOTCBBEQ     22  /* NASDAQ non OTCBB equities */
#define TF_EXCHG_ID_THIRD          23  /* NTDS NASDAQ listed third market */
#define TF_EXCHG_ID_PHL            24  /* PHX, philadelphia stock exchange */
#define TF_EXCHG_ID_PHL_N          27  /* PHX, (listed on NASDAQ) */
#define TF_EXCHG_ID_OTCBB          25  /* OTCBB over the counter bulletin board equities */
#define TF_EXCHG_ID_CSE            26  /* CSE, cincinnati stock exchange */
#define TF_EXCHG_ID_CSE_N          28  /* CSE, (listed on NASDAQ) */
#define TF_EXCHG_ID_US_COMP       126  /* US composite */

// markets 100-125 are reserved for market depth of various markets (only as subscription markets)

#define TF_EXCHG_ID_SEAQ_NAT      151  /* LSE SEAQ national */
#define TF_EXCHG_ID_SEAQ_INT      152  /* LSE SEAQ international*/
#define TF_EXCHG_ID_LSE_CWTS      112  /* LSE Covered Warrants */
#define TF_EXCHG_ID_BIFFEX_ROUTES 153  /* BIFFEX specific routes */
#define TF_EXCHG_ID_BIFFEX        154  /* BIFFEX headline service */
#define TF_EXCHG_ID_LCE           154  /* london commodities exchange (old feed, n.u. since 07/03) */
#define TF_EXCHG_ID_LME           155  /* london metal exchange (old feed, n.u. since 07/03) */
#define TF_EXCHG_ID_LME_NEW       157  /* london metal exchange (new feed) */
#define TF_EXCHG_ID_IPE           156  /* IPE, international petroleum exchange, London */
#define TF_EXCHG_ID_LIFFE         159  /* LIFFE */
#define TF_EXCHG_ID_LIFFEMD       104  /* LIFFE, reserved for market depth */
#define TF_EXCHG_ID_LSE_LVL2N     110  /* LSE Level 2 national */
#define TF_EXCHG_ID_LSE_LVL2I     111  /* LSE Level 2 international */
#define TF_EXCHG_ID_LSE_LVL1PN    112  /* LSE Level 1plus national */
#define TF_EXCHG_ID_LSE_LVL1PI    113  /* LSE Level 1plus international */

#define TF_EXCHG_ID_SBF           160  /* SBF - Paris Stock Exchange */
#define TF_EXCHG_ID_MONEP         161  /* MONEP - Paris options */
#define TF_EXCHG_ID_SBFMD         162  /* SBF - market depth */
#define TF_EXCHG_ID_MATIF         168  /* MATIF */

#define TF_EXCHG_ID_SMF_SOFFEX    181  /* Eurex Swiss (SMF: soffex) */
#define TF_EXCHG_ID_SMF_EBS       182  /* SMF: EBS (electronic boerse suisse) */
#define TF_EXCHG_ID_SWX           TF_EXCHG_ID_SMF_EBS  /* SWX replaced EBS */
#define TF_EXCHG_ID_SWXMD         105  /* SWX, reserved for market depth */
#define TF_EXCHG_ID_VIRTX         185  /* VIRT-X (european blue chips) */
#define TF_EXCHG_ID_VIRTXMD       106  /* VIRT-X, reserved for market depth */

#define TF_EXCHG_ID_DUB           190  /* Dublin, Irish stock exchange */
#define TF_EXCHG_ID_DUBMD         100  /* Dublin, reserved for market depth */
#define TF_EXCHG_ID_DUBRES        191  /* Dublin, reserved for future use */

#define TF_EXCHG_ID_EGYPT         193  /* reserved for Egypt stock exchange */

#define TF_EXCHG_ID_VSE           194  /* reserved for Vienna stock exchange */

#define TF_EXCHG_ID_LISBON        195  /* Euronext Lisbon (former BVLP - Bolsa de Valores de Lisboa e Porto */
#define TF_EXCHG_ID_LISBON_DER    196  /* Euronext Lisbon Derivatives (reserved) */

#define TF_EXCHG_ID_MADRID        199  /* reserved for Madrid stock exchange */

#define TF_EXCHG_ID_FFM           200  /* Frankfurter Börse (TPF general) */
#define TF_EXCHG_ID_DTB           201  /* EUREX (was DTB, Deutsch Termin Börse) */
#define TF_EXCHG_ID_EUREX         TF_EXCHG_ID_DTB
#define TF_EXCHG_ID_EUREXMD       101  /* EUREX, reserved for market depth */

#define TF_EXCHG_ID_AEX           202  /* AEX stocks */
#define TF_EXCHG_ID_EOE           203  /* AEX options */
#define TF_EXCHG_ID_AEXMD         204  /* AEX market depth */

#define TF_EXCHG_ID_ADEX          206  /* adex, athens derivative exchange */
#define TF_EXCHG_ID_ADEXMD        108  /* adex, reserved for market depth */

#define TF_EXCHG_ID_BEL1          207  /* Euronext Brussels, Belgium */
#define TF_EXCHG_ID_BEL2          209  /* Euronext Brussels Derivatives */
#define TF_EXCHG_ID_BELMD         109  /* Euronext Brussels Derivatives Market Depht */

#define TF_EXCHG_ID_ATHENS        212  /* athens stock exchange */
#define TF_EXCHG_ID_ATHENSMD      107  /* ASE, reserved for market depth */

#define TF_EXCHG_ID_XETRA         213  /* XETRA Frankfurt */
#define TF_EXCHG_ID_IBIS          TF_EXCHG_ID_XETRA  /* for compatibility */
#define TF_EXCHG_ID_XETRAMD       102  /* XETRA, reserved for market depth */
#define TF_EXCHG_ID_MUNICH        214  /* München */
#define TF_EXCHG_ID_STUTT         215  /* Stuttgart */
#define TF_EXCHG_ID_BERLIN        216  /* Berlin */
#define TF_EXCHG_ID_BREMEN        217  /* Bremen */
#define TF_EXCHG_ID_DDORF         218  /* Düsseldorf */
#define TF_EXCHG_ID_HAMBG         219  /* Hamburg */
#define TF_EXCHG_ID_HANOVER       220  /* Hanover */
#define TF_EXCHG_ID_NM            221  /* Neuer Markt */
#define TF_EXCHG_ID_NMMD          103  /* Neuer Markt, reserved for market depth */

#define TF_EXCHG_ID_STOXX         222  /* STOXX symbols */

#define TF_EXCHG_ID_CED           223  /* reserved for CED Borsa Milan */
#define TF_EXCHG_ID_CEDMD         224  /* CED Borsa market depth */
#define TF_EXCHG_ID_SIA           226  /* reserved for SIA Milan */

#define TF_EXCHG_ID_MOSCOW        230  /* Moscow stock exchange */
#define TF_EXCHG_ID_MOSCOW_FUT    231  /* Moscow future exchange */

#define TF_EXCHG_ID_CYPSE         239  /* Cyprus stock exchange */

// reserve markets 240 - 249 for forex markets
#define TF_EXCHG_ID_FOREX         240  /* FOREX cross rates data */
#define TF_EXCHG_ID_METALS        241  /* FOREX prec. metals data */
#define TF_EXCHG_ID_MON_MKT       242  /* FOREX monery market data */
#define TF_EXCHG_ID_TUTO          243  /* FOREX data from Tullet & Tokyo */
#define TF_EXCHG_ID_HBI           244  /* FOREX data from HBI (Garban) */
#define TF_EXCHG_ID_LITE          245  /* TenForeX Lite */

/*
 * future and future option exchanges
 */
#define TF_EXCHG_ID_KCBT           43  /* Kansas city board of trade */
#define TF_EXCHG_ID_MACE           56  /* mid american commodity exchange */

#define TF_EXCHG_ID_CBOT           64  /* CBOT */
#define TF_EXCHG_ID_CFFE           65  /* Cantor Financial Futures Exchange */
#define TF_EXCHG_ID_CEC_COMEX      66  /* NY Commodity Exchange */
#define TF_EXCHG_ID_CEC_NYMEX      67  /* NY Mercantile Exchange */
#define TF_EXCHG_ID_CEC_CSCE       68  /* Coffe, Sugar, and Cocoa Exchange */
#define TF_EXCHG_ID_CME            75  /* chicago mercantile exchange */
#define TF_EXCHG_ID_CME1           77  /* additional CME market */
#define TF_EXCHG_ID_CEC_NYFE       76  /* NY Futures Exchange */
#define TF_EXCHG_ID_CEC_NYCE       80  /* NY Cotton Exchange */
#define TF_EXCHG_ID_CEC_NYCT       TF_EXCHG_ID_CEC_NYCE  /* for compatibility */
#define TF_EXCHG_ID_CEC_FINEX      84  /* NY Cotton, Financial Associates */


//
// exchange IDs used for the news sources
//
#define TF_EXCHG_ID_NEWS_EAG          90 /* EAG internal news */
#define TF_EXCHG_ID_NEWS_MNS          91 /* market news */
#define TF_EXCHG_ID_NEWS_AFX          92 /* AFX news */
#define TF_EXCHG_ID_NEWS_FWN          93 /* OsterDowJones Overview (old future world news) */
#define TF_EXCHG_ID_NEWS_PRN          94 /* PR news stories (15 min delay) */
#define TF_EXCHG_ID_NEWS_PRHDR        95 /* PR news realtime headers (not used) */
#define TF_EXCHG_ID_NEWS_ADN          96 /* german ADN news (not used) */
#define TF_EXCHG_ID_NEWS_ADN1         97 /* local german ADN news (not used) */
#define TF_EXCHG_ID_NEWS_RMS          98 /* greek news */

#define TF_EXCHG_ID_NEWS_AEX          99 /* AEX feed news */

#define TF_EXCHG_ID_NEWS_VWDS        100 /* VWD special news */
#define TF_EXCHG_ID_NEWS_VWDI        101 /* VWD international news */
#define TF_EXCHG_ID_NEWS_VWDD        102 /* VWD deutschland news */

#define TF_EXCHG_ID_NEWS_HMERISIA    103 /* Hmerisia news */

#define TF_EXCHG_ID_NEWS_VWD_NM      104 /* VWD Neuer Markt news */
#define TF_EXCHG_ID_NEWS_VWD_IPO     105 /* VWD IPO news */
                                         /* reserve 106-109 for other vwd news */
#define TF_EXCHG_ID_NEWS_VWD_NASD    106 /* VWD NASDAQ news */

#define TF_EXCHG_ID_NEWS_AFX_UK      110 /* AFX UK news */
#define TF_EXCHG_ID_NEWS_AFX_ASIA    111 /* AFX Asia news */
#define TF_EXCHG_ID_NEWS_AFX_NL      112 /* AFX Dutch news */
#define TF_EXCHG_ID_NEWS_AFX_DE      113 /* AFX Deutschland news */
#define TF_EXCHG_ID_NEWS_AFX_IT      114 /* AFX Italy news */
#define TF_EXCHG_ID_NEWS_AFX_CH      115 /* AFX Swiss news */
#define TF_EXCHG_ID_NEWS_AFX_DL      116 /* dpa-AFX Internet news */

#define TF_EXCHG_ID_NEWS_INVEST      117 /* Swiss INVEST.CH news */
#define TF_EXCHG_ID_NEWS_TENTI       118 /* Swiss TENTI news */

#define TF_EXCHG_ID_NEWS_CYPRUS      119 /* Cyprus news (name to be defined) */

#define TF_EXCHG_ID_NEWS_SWX         120 /* SWX feed news */
#define TF_EXCHG_ID_NEWS_SBF         121 /* SBF feed news */
#define TF_EXCHG_ID_NEWS_RNS         122 /* RNS regulatory news service */

#define TF_EXCHG_ID_NEWS_LBBWA       123 /* LaBa BaWü A P1 TMV/Weekly */
#define TF_EXCHG_ID_NEWS_LBBWB       124 /* LaBa BaWü B P2 Spots */
#define TF_EXCHG_ID_NEWS_LBBWC       125 /* LaBa BaWü C P3 Intraday */
#define TF_EXCHG_ID_NEWS_LBBWD       127 /* LaBa BaWü D P4 Handelssignale */
#define TF_EXCHG_ID_NEWS_LBBWE       128 /* LaBa BaWü E P5 Intraday Handelssignale */

#define TF_EXCHG_ID_NEWS_SIDEX       129 /* SIDEX Bulgarian news */
#define TF_EXCHG_ID_NEWS_IBB         129 /* IBB News (Dutch) */

#define TF_EXCHG_ID_NEWS_RFM         130 /* Russian Financial Market news */
#define TF_EXCHG_ID_NEWS_AKME        131 /* AK&M (English) news (not used any more) */
#define TF_EXCHG_ID_NEWS_AKMR        132 /* AK&M (Russian) news (not used any more) */

#define TF_EXCHG_ID_NEWS_BBNB        131 /* BeursMedia Betten news B-Feed */
#define TF_EXCHG_ID_NEWS_BBNC        132 /* BeursMedia Betten news C-Feed */

#define TF_EXCHG_ID_NEWS_DISTR       133 /* Distributor news */

#define TF_EXCHG_ID_NEWS_EVN         134 /* Ekonomske Vesti (Jugosl.) news */

#define TF_EXCHG_ID_NEWS_ANAF        135 /* Athens News Agency Financial news */

#define TF_EXCHG_ID_NEWS_LBBWF       136 /* LaBa BaWü F P6 Aktienzusatzpaket */
#define TF_EXCHG_ID_NEWS_LBBWG       137 /* LaBa BaWü G P7 Rentenzusatzpaket */

#define TF_EXCHG_ID_NEWS_AFP         138 /* AFP news */

#define TF_EXCHG_ID_NEWS_BBNS        139 /* BeursMedia SCE news */

#define TF_EXCHG_ID_NEWS_DJNL        140 /* Dow Jones NL (dutch) news */
#define TF_EXCHG_ID_NEWS_DJNS        141 /* Dow Jones news general */

#define TF_EXCHG_ID_NEWS_ANSA        145 /* ANSA (italian) news */
#define TF_EXCHG_ID_NEWS_PQT         146 /* ProfQuotes (professional quotes via new service) */

#define TF_EXCHG_ID_NEWS_FINPRESS_N  147 /* FinPress N (case economics bulletin) */
#define TF_EXCHG_ID_NEWS_FINPRESS_R  148 /* FinPress R (case economics review issues) */

#define TF_EXCHG_ID_NEWS_SELEUKOS    149 /* Seleukos news (IT) */

#define TF_EXCHG_ID_NEWS_ODJ_BASE    150 /* OsterDowJones basic news (use the following definitions) */
#define TF_EXCHG_ID_NEWS_ODJ_A       TF_EXCHG_ID_NEWS_ODJ_BASE+0 /* 150 ODJ Agricult. news */
#define TF_EXCHG_ID_NEWS_ODJ_C       TF_EXCHG_ID_NEWS_ODJ_BASE+1 /* 151 ODJ Commodity wire */
#define TF_EXCHG_ID_NEWS_ODJ_E       TF_EXCHG_ID_NEWS_ODJ_BASE+2 /* 152 ODJ Energy news */
#define TF_EXCHG_ID_NEWS_ODJ_F       TF_EXCHG_ID_NEWS_ODJ_BASE+3 /* 153 ODJ Financial news */
#define TF_EXCHG_ID_NEWS_ODJ_M       TF_EXCHG_ID_NEWS_ODJ_BASE+4 /* 154 ODJ Metals news */
#define TF_EXCHG_ID_NEWS_ODJ_T       TF_EXCHG_ID_NEWS_ODJ_BASE+5 /* 155 ODJ Trader news */

#define TF_EXCHG_ID_NEWS_FINANZA     158 /* Finanza Online news (IT) */

/*
 * values for the TF_FIELD_STATIC_AMD field
 */
#define TF_FIELD_AMD_DELETE            1 /* delete static data for the symbol */
#define TF_FIELD_AMD_MODIFY            2 /* modify static data for the symbol */
#define TF_FIELD_AMD_ADD               3 /* add static data for the symbol */
#define TF_FIELD_AMD_UNCHG             4 /* static data for the symbol unchanged */

/*
 * values for the TF_FIELD_TRADE_COND field
 * (combinations possible)
 */
#define TF_FIELD_TRADE_COND_LAST     0x01 /* update last price */
#define TF_FIELD_TRADE_COND_HIGHLOW  0x02 /* update high/low values from last price */
#define TF_FIELD_TRADE_COND_CMP      0x04 /* update composite symbol also */
#define TF_FIELD_TRADE_COND_OPEN     0x08 /* update open value from last price */
#define TF_FIELD_TRADE_COND_VOL      0x10 /* update only volume */
#define TF_FIELD_TRADE_COND_DEFAULT  (TF_FIELD_TRADE_COND_LAST | TF_FIELD_TRADE_COND_HIGHLOW | TF_FIELD_TRADE_COND_VOL)



#pragma pack(1)
/*
 * union to hold a value of several data type
 */
typedef union _TfValue {
    signed char    c;      // signed character
    unsigned char  b;      // unsigned character
    signed short   s;      // signed short
    unsigned short w;      // unsigned short
    signed long    l;      // signed long
    unsigned long  u;      // unsigned long
    float          f;      // float
    double        *d;      // pointer to double value
    char          *p;      // pointer to string (do not try to free this pointer)
} TfValue;

/*
 * data field descriptor
 */
typedef struct _TfField {
	WORD    nFieldId;      // field ID defined by tenfore
	BYTE    bValueType;    // value type
	TfValue Value;         // union to hold the actual value
} TfField;

/*
 * structure for a TENFORE data packet
 */
typedef struct _TfPacket {
	WORD    nMessageType;  // message type
	WORD    nSecurityType; // security type
	BYTE    bExchangeId;   // exchange ID
	BYTE    bMarketCenter; // market center if different from exchange id
	int     iFieldCount;   // # of following fields
	char    cSymbol[20];   // symbol name
    TfField FieldArr[1];   // start of the field array (use TApiGetNextField() to loop through this array)
} TfPacket;

/*
 * Client API Section
 */

/*
 *	structure and values for ClientApi symbol request
 */

#define TF_CAPI_ACTIVATE_REALTIME    0x00000001  // send realtime values once and activate updating
#define TF_CAPI_REQUEST_REALTIME     0x00000002  // send realtime information once
#define TF_CAPI_REQUEST_FUNDAMENTAL  0x00000004  // send fundamental information once
#define TF_CAPI_DEACTIVATE_REALTIME  0x00000008  // stop sending realtime updates
#define TF_CAPI_REQUEST_RECAP		 0x00000010	 // send recap for allready activated symbol

// additional flags, which control the information send with a trade message
// only last price, last volume, time and trade qualifiers/indicators are send by default

#define TF_CAPI_SEND_CUM_VOLUME     0x00010000  // cumulative volume
#define TF_CAPI_SEND_TRADE_COUNT    0x00020000  // trade count
#define TF_CAPI_SEND_HIGH_LOW       0x00040000  // high and low
#define TF_CAPI_SEND_OPEN_CLOSE     0x00080000  // open and close (close is usualy updated by a recap)
#define TF_CAPI_SEND_QUOTE_FIELDS   0x00100000  // all quote related fields

#define TF_CAPI_SEND_ALL            0x00800000  // send a complete recap with each trade message

typedef struct _TfCApiSymbolRequest {
    WORD    nSecurityType; // security type
    BYTE    bExchangeId;   // exchange ID
    BYTE    bMarketCenter; // market center if different from exchange id
    char    cSymbol[20];   // symbol name
    DWORD   dwFlags;       // type of request
} TfCApiSymbolRequest;


/*
 *  structure and values for ClientApi database request
 */

// possible values for bGeoRegion are
#define CAPI_GRG_USA           126    /* (grg < 128) */
#define CAPI_GRG_CANADA        128    /* (grg < 139) */
#define CAPI_GRG_GERMANY       200    /* (grg == 200 || grg == 201 || grg == 213) */
#define CAPI_GRG_SWISSE        180    /* (grg > 178 && grg < 184 && grg != 182) */
#define CAPI_GRG_NETHERLAND    202    /* (grg > 201 && grg < 206 && grg != 204) */
#define CAPI_GRG_ENGLAND       150    /* (grg == 150) */
#define CAPI_GRG_FRANCE        160    /* (grg > 159 && grg < 170 */
#define CAPI_GRG_AUSTRIA       194    /* (grg == 194) */
#define CAPI_GRG_ITALY         223    /* (grg == 223 || grg == 226) */
#define CAPI_GRG_SPAIN         199    /* (grg == 199) */
#define CAPI_GRG_BELGIUM       207    /* (grg == 207 || grg == 209) */
#define CAPI_GRG_ATHENS        212    /* (grg == 212) */
#define CAPI_GRG_VIRTX         185
#define CAPI_GRG_IRELAND       190
#define CAPI_GRG_EGYPT         193
#define CAPI_GRG_CYPRUS        239
#define CAPI_GRG_RUSSIA        230
#define CAPI_GRG_FOREX         240    /* (grg > 239 && grg <250) || grg == 155 */
#define CAPI_GRG_UK            150

// possible values for bCategory

#define CAPI_SEC_STOCKS         1     // search for stocks, indices, funds and warrents
#define CAPI_SEC_OPTIONS        2     // search for options (set symbol field to root symbol)
#define CAPI_SEC_FUTURES        3     // search for futures and comodities
#define CAPI_SEC_BONDS          4     // search for bonds
#define CAPI_SEC_FUTUREOPTIONS  6     // search for future options (set symbol field to future root symbol)


typedef struct _TfCApiDbaseRequest {
    BYTE    bGeoRegion;    // search database for country bGeoRegion
    BYTE    bCategory;     // search database for bCategory
    char    cSymbol[20];   // the seach will start after symbol cSymbol
    DWORD   dwNumber;      // number of symbols requested
} TfCApiDbaseRequest;



#pragma pack()


#endif


