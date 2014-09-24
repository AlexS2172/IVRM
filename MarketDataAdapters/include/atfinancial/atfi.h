/*
**** ATFI.H - Include file for ATFI Opening Bell apps
****
**** This file contains system wide constants, special types
**** and function prototypes
*/

#if !defined(_ATFI_H)
#define	_ATFI_H

// Define the INCL_ALL flag to include all *optional* include files
#if defined(INCL_ALL)
 #define INCLUDE_EDBX
 #define INCLUDE_RXF
 #define INCLUDE_DISCLOSE
 #define INCLUDE_VICKERS
 #define INCLUDE_ZACKS
 #define INCLUDE_MNSTAR
#endif

#if !defined(_WINSOCKAPI_)
#include <winsock.h>
#endif

#if !defined(INCL_DBWIN)
#include "dbwin.h"
#endif

#if !defined(__ATFIFUND_DEFINES)
#include "atfifund.h"
#endif

#if defined(INCLUDE_EDBX)
 #if !defined(INCL_DBAX)
  #include "edbx.h"
 #endif
#endif

#if defined(INCLUDE_RXF)
 #if !defined(INCL_RXF)
  #include "rxf.h"
 #endif
#endif

#include <atfinews.h>

#ifdef __cplusplus
#include <ringbufr.h>
#endif

#if !defined(_ATFD_H_)
#include "atfd.h"
#endif

#if !defined(_ATAPINFO_H_)
#include "atapinfo.h"
#endif

#if !defined(_CURRENCY_H_)
#include "currency.h"
#endif

#if defined(INCLUDE_DISCLOSE)
 #if !defined(_DISCLOSE_H)
  #include "disclose.h"
 #endif
#endif

#if defined(INCLUDE_VICKERS)
 #if !defined(_VICKERS_H)
  #include "vickers.h"
 #endif
#endif

#if defined(INCLUDE_ZACKS)
 #if !defined(_ZACKS_H)
  #include "zacks.h"
 #endif
#endif

#if defined(INCLUDE_MNSTAR)
 #if !defined(_MNSTAR_H_)
  #include "mnstar.h"
 #endif
#endif

#if !defined(_ATHALT_H_)
#include "athalt.h"
#endif

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifdef	__cplusplus
extern	"C" {
#endif

	// price types

#define PTYP_WHOLE			0
#define PTYP_DECIMAL_1		1
#define PTYP_DECIMAL_2		2
#define PTYP_DECIMAL_3		3
#define PTYP_DECIMAL_4		4
#define PTYP_DECIMAL_5		5
#define PTYP_DECIMAL_6		6
#define PTYP_DECIMAL_7		7

#define PTYP_2				8
#define PTYP_4				9
#define PTYP_8				10
#define PTYP_16				11
#define PTYP_32				12
#define PTYP_64				13
#define PTYP_128			14
#define PTYP_256			15

#define PTYP_DECIMAL_8		16
#define PTYP_DECIMAL_9		17

	// Display price types.

#define PTYP_STOCKS			20		// Stocks reduce fraction.
#define PTYP_QUARTER_32		21		// CBOT crap for bond futures
#define PTYP_HALF_32		22		// CBOT crap for bond futures
#define PTYP_HALF_64		23		// CBOT crap for bond futures
#define PTYP_FUTURE_64		24		// price type 64ths (futures)
#define PTYP_BOND_64		24		// price type 64ths (bonds)
#define PTYP_FUTURE_32		25		// price type 32nds (futures)
#define PTYP_BOND_32		25		// price type 32nds (bonds)
#define PTYP_2_HALF			26		// price type 2 1/2ths (futures)

#define PTYP_UNASSIGNED		31		// No price type established.

	// Price Block Types (Unique from server datatypes)

#define	PBT_AT_BONDS		100
#define	PBT_AT_MUTUAL		101
#define	PBT_AT_MMFUND		102
#define	PBT_AT_FUTURE		103
#define	PBT_AT_FUTOPTS		104
#define	PBT_AT_INDEX		105
#define	PBT_AT_STOCKS		106
#define	PBT_AT_NASDAQ		107
#define	PBT_AT_STKOPTS		108
#define	PBT_AT_FOREX		109
#define	PBT_AT_FIXED_INCOME	110
#define	PBT_AT_MARKET_MAKER	111
#define	PBT_AT_NEWS_INFO	112

	// Data types - Server Data Block Types

#define	DT_BONDS			0
#define	DT_FUTOPTS			1
#define	DT_FUTINDX			2
#define	DT_STOCKS			3
#define	DT_CANADA			4
#define	DT_EUROPE			5

#define	DT_STKOPT_AE		6		//             DT_STKOPT_AG
#define	DT_STKOPT_FM		7		// DT_STKOPT_A DT_STKOPT_HQ
#define	DT_STKOPT_NR		8		// DT_STKOPT_N DT_STKOPT_RZ
#define	DT_STKOPT_SZ		9		// Stock Option Split - 4.60

#define	DT_PQ_US_BONDS		10		// US Markets
#define	DT_PQ_US_FUTOPTS	11
#define	DT_PQ_US_FUTINDX	12
#define	DT_PQ_US_STOCKS		13

#define	DT_FC_EARNINGS		14		// First Call earnings

#define	DT_PQ_EU_STOCKS		15		// European Markets
#define	DT_PQ_EU_STKOPTS	16
#define	DT_PQ_EU_BONDS		17
#define	DT_PQ_EU_FUTOPTS	18
#define	DT_PQ_EU_FUTINDX	19

#define	DT_PQ_AM_BONDS		20		// North/Central/South America
#define	DT_PQ_AM_FUTOPTS	21		//     Non-US Markets
#define	DT_PQ_AM_FUTINDX	22
#define	DT_PQ_AM_STOCKS		23
#define	DT_PQ_AM_STKOPTS	24

#define	DT_PQ_AS_BONDS		25		// Asian Markets
#define	DT_PQ_AS_FUTOPTS	26
#define	DT_PQ_AS_FUTINDX	27
#define	DT_PQ_AS_STOCKS		28
#define	DT_PQ_AS_STKOPTS	29

#define	DT_PQ_US_STKOPT_A	30
#define	DT_PQ_US_STKOPT_N	31

#define	DT_FOREX			32
#define	DT_FIXED_INCOME		33

#define DT_SP_LEVEL2_A		34
#define DT_SP_LEVEL2_N		35
#define DT_MONEY_RATE		36
#define DT_MARKET_MAKER		37
#define DT_ALL_LEVEL2		37

#define DT_MONEY_MARKET		38
#define DT_MUTUAL_FUND		39

#define	DT_KRCASH			40
#define	DT_KRFUTURE			41
#define	DT_KRFFMT			42

#define	DT_JJMUNIBOND		45
#define	DT_JJBLUELIST		46
#define	DT_JJKDPAGE			47

#define	DT_RSFQUOTE			50
#define	DT_RSFPAGE			51
#define	DT_RSFSYNTH			52

#define	DT_TRTFFMT			55
#define	DT_CMFFMT			56

#define	DT_GPXCUSIP			57
#define	DT_GPXQUOTE			58
#define	DT_GPXFFMT			59

#define	DT_NQDS_A			60
#define	DT_NQDS_M			61

#define	DT_PQ_US_STKOPT_AG	65
#define	DT_PQ_US_STKOPT_HQ	66
#define DT_PQ_US_STKOPT_RZ	67
#define DT_PQ_US_STKOPTS	68

#define DT_INDEX			69

#define	DT_CM_FOREX			70
#define	DT_CM_TREASURY		71
#define	DT_CM_MONEYMARKET	72
#define	DT_CM_METAL			73

#define DT_PQ_US_STKOPT_AE	75
#define DT_PQ_US_STKOPT_FM	76
#define DT_PQ_US_STKOPT_NR	77
#define DT_PQ_US_STKOPT_SZ	78
	
#define DT_NEWS_INFO		80

//	Data types 100 to 127 reserved for Price block types

//	Data types 150 to 180 reserved for delayed Price block types

#define	DT_US_BONDS_DLY		150		// US Markets
#define	DT_US_FUTOPTS_DLY	151
#define	DT_US_FUTURE_DLY	152
#define	DT_US_STOCKS_DLY	153
#define	DT_US_STKOPTS_DLY	154

#define	DT_EU_BONDS_DLY		155		// European Markets
#define	DT_EU_FUTOPTS_DLY	156
#define	DT_EU_FUTURE_DLY	157
#define	DT_EU_STOCKS_DLY	158
#define	DT_EU_STKOPTS_DLY	159
		   
#define	DT_AM_BONDS_DLY		160		// North/Central/South America
#define	DT_AM_FUTOPTS_DLY	161		//     Non-US Markets
#define	DT_AM_FUTURE_DLY	162
#define	DT_AM_STOCKS_DLY	163
#define	DT_AM_STKOPTS_DLY	164
		   
#define	DT_AS_BONDS_DLY		165		// Asian Markets
#define	DT_AS_FUTOPTS_DLY	166
#define	DT_AS_FUTURE_DLY	167
#define	DT_AS_STOCKS_DLY	168
#define	DT_AS_STKOPTS_DLY	169

#define DT_PORTFOLIO		200

#define DT_FUTURE_INFO		0xf0	// the next 6 data types live in NT servers only,
#define DT_TOPTEN			0xf1	// and are not visible to workstations... at least
#define DT_ADVISE			0xf2	// not yet.
#define DT_LOGON			0xf3
#define DT_ADVISE_QSDTID	0xf4
#define DT_STATISTICS		0xf5

#define	DT_ADVISE_SYMS		0xf6	// the next 4 data types live in NT Advanced 
#define	DT_ADVISE_CATS		0xf7	// NEWS Servers only
#define	DT_ADVISE_KEYWD		0xf8
#define	DT_ADVISE_GCATS		0xf9	

#define DT_L2_MMQUOTES		0xfc

#define	DT_ATPRESSNEWS		0xfd
#define DT_TOPSTORYNEWS		0xfe

#define	DT_SPECIAL			0xff
#define	DT_DEFAULT			0xff

	/*-----------------------------------*/
	/* These are the values the HIWORD() */
	/* portion of an WM_AT_DATA message  */
	/*-----------------------------------*/

#define	ATD_PRICE			0
#define	ATD_SCROLLINGNEWS	1
#define	ATD_NEWS			2
#define	ATD_HEARTBEAT		3
#define	ATD_TICKS			4
#define	ATD_FFMT			5
#define	ATD_KRBLOCK			6
#define	ATD_READVISE		7
#define	ATD_RSFSTATUS		8
#define	ATD_RSFQUOTE		9
#define	ATD_L2UPDATE		10		// NASDAQ Level 2 Update
#define	ATD_MMUPDATE		11		// Market Maker Update
#define	ATD_NEWSEVENT		12		// News Event
#define	ATD_NEWSSCROLL		13		// News Scroll
#define	ATD_QSALARM			14
#define	ATD_RSFPNAC			15		// Reuters Select Feed PNAC information
#define	ATD_PROFILEEVENT	16		// News Headline
#define	ATD_QSCHANGED		17		// QuoteServer config has changed
#define	ATD_TERMINATE		18		// ATSERVER is terminating
#define	ATD_GPXQUOTE		19
#define	ATD_QTUPDATE		20		// Short updates
#define	ATD_QSTIMEDATE		21		// QuoteServer Time/Date message
#define	ATD_ORDERFILL		22		// Order filled
#define	ATD_CDUPDATE		23		// Condition update
#define	ATD_LOSTPACKET		24		// We lost 1+ packets (QSN in LOWORD)
#define	ATD_BBUPDATE		25		// Big Block Trade updates
#define	ATD_SEARCHEVENT		26		// NT News Server Async Search Results
#define	ATD_INDEXUPD		27		// NT News Server Async Index Update Notification
#define	ATD_HALTUPDATE		28		// Halt Data updates
#define	ATD_QSMESSAGE		29		// QuoteServer Messaging update.

#define	ATD_ATPRICE			50		// Advanced AT updates.

	/*----------------------------------------------------*/
	/* ATD_HEARTBEAT messages are sent every two seconds  */
	/* These "time" values can be used for comparisons	  */	
	/*----------------------------------------------------*/

#define	HB_1_MINUTE			30
#define	HB_1_HOUR			(60 * HB_1_MINUTE)

	/*-------------------------------------------------*/
	/* QS Alarm values: LOBYTE(LOWORD(lParam)) = QSN   */
	/*					HIBYTE(LOWORD(lParam)) = alarm */
	/*-------------------------------------------------*/

#define	QS_DATALOSS			1
#define	QS_DATARESUME		2
#define	QS_ADVISELIMITNEAR	3
#define	QS_ADVISELIMITHIT	4
#define QS_ATSCONNERROR		5	// ATServer connection has failed, see hat->iStatus

	/*-------------------------------------------------*/
	/* These values are used to ADVISE on Special item */
	/*-------------------------------------------------*/

#define	ID_DJS_NEWSCHG		0x0001
#define	ID_DJS_SCROLL		0x0011
		
#define	ID_DJB_NEWSCHG		0x0002
#define	ID_DJB_SCROLL		0x0012
		
#define	ID_FWN_NEWSCHG		0x0003
#define	ID_FWN_SCROLL		0x0013
		
#define	ID_SPM_NEWSCHG		0x0004
#define	ID_SPM_SCROLL		0x0014
		
#define	ID_KRN_NEWSCHG		0x0005
#define	ID_KRN_SCROLL		0x0015
		
#define	ID_DJF_NEWSCHG		0x0006
#define	ID_DJF_SCROLL		0x0016
		
#define	ID_ICV_NEWSCHG		0x0007
#define	ID_ICV_SCROLL		0x0017
		
#define	ID_SST_NEWSCHG		0x0008		// No scroll avail
		
#define	ID_RTR_NEWSCHG		0x0009		// No scroll avail
		
#define	ID_MMS_NEWSCHG		0x000a		// No scroll avail
		
#define	ID_TICKDATA			0x0021
		
#define	ID_RSF_STATUS		0x0031
#define	ID_RSF_PNAC			0x0032
		
#define	ID_JJ_OFFERINGS		0x003a
		
#define	ID_PQ_MMQUOTES		0x0041
		
#define	ID_ANY_NEWSCHG		0x004e
#define	ID_ANY_NEWSCROLL	0x004f
		
#define	ID_NEWS_EVENT		0x0050
#define	ID_NEWS_SCROLL		0x0051
		
#define	ID_PROFILE_EVENT	0x0060
#define	ID_SEARCH_EVENT		0x0061

#define ID_BIG_BLOCK		0x0070
#define ID_HALTDATA			0x0071

#define ID_QS_MESSAGE		0x0080

	/*-------------------------------------*/
	/* QuoteServer New Source definitions  */
	/*-------------------------------------*/

#define		ID_QSNS_ICV			6

	/*------------------------------------------*/
	/* Key types used with GetFundamentalData() */
	/*------------------------------------------*/

#define		KEY_CUSIP			0
#define		KEY_STOCKSYMBOL		1
#define		KEY_STOCKNAME		2
#define		KEY_FUTURESYMBOL	3
#define		KEY_EUROPESYMBOL	4
#define		KEY_EUROPELNAME		5
#define		KEY_EUROPEIDNUM		6
#define		KEY_SNAPSHOTSYMBOL	7
#define		KEY_SNAPSHOTNAME	8

#define		MAX_BTREE_KEYSIZE	40

// defines for login messages

#define	STATUS_OK			0x4b4f
#define	STATUS_USERDATA		(WORD) -1
#define	STATUS_GROUPDATA	(WORD) -2
#define	STATUS_GROUP_NF		(WORD) -3
#define	STATUS_USER_NF		(WORD) -4
#define	STATUS_MULTI_WS		(WORD) -5
#define	STATUS_CONN_NF		(WORD) -6
#define	STATUS_FULL			(WORD) -7


/*
**** Messages To/From ATSERVER/Apps Messages
****          have the QSM_* prefix
*/

/*-------- ATFI Special types and record layouts ----------*/

#ifndef __DTDP_STRUCTURE
#define __DTDP_STRUCTURE

typedef struct _DTDP
{
	union
	{
		DWORD  dtdp;
		struct
		{
			DWORD  dataPtr: 24;			// lower 24 bits is data pointer
			DWORD  dataType: 8;			// upper 8 bits is the data type
		};
	};

#ifdef __cplusplus

	void Init( DWORD dt, DWORD dp )
	{
		dataPtr = dp;
		dataType = dt;
	}

	void SetDataType( DWORD dt )
	{
		dataType = dt;
	}

	void SetDataPtr( DWORD dp )
	{
		dataPtr = dp;
	}

	DWORD GetDataType( void )
	{
		return dataType;
	}

	DWORD GetDataPtr( void )
	{
		return dataPtr;
	}
#endif
} DTDP, *LPDTDP, *PDTDP;

#endif

#if 0
//////////// Taken over by Ken 9/2/98
typedef struct {
	DWORD  dp: 16;				// lower 16 bits of dataptr
	DWORD  dt: 8;				// data type
	DWORD  qs: 5;				// quote server
	DWORD  dh: 3;				// upper 3 bits of dataptr
} FQSDTDP, *PFQSDTDP, *LPFQSDTDP;

typedef union {
	FQSDTDP  fields;
	DWORD    value;

#ifdef __cplusplus

	void SetQS( DWORD qs )
	{
		fields.qs = qs & 0x1f;
	}

	void SetDataType( DWORD dataType )
	{
		fields.dt = dataType & 0xff;
	}

	void SetDataPtr( DWORD dataPtr )
	{
		fields.dh = ( dataPtr >> 16 ) & 0x07;
		fields.dp = dataPtr & 0xffff;
	}

	BYTE GetQS()
	{
		return (BYTE)fields.qs;
	}

	BYTE GetDataType()
	{
		return (BYTE)fields.dt;
	}

	DWORD GetDataPtr()
	{
		return ( fields.dh << 16 ) | fields.dp;
	}

#endif
} QSDTDP;

#define QS(a)	((BYTE) a.fields.qs )
#define DT(a)	((BYTE) a.fields.dt )
#define DP(a)	(((DWORD) a.fields.dh << 16 ) | (DWORD) a.fields.dp )

#else
//////////// This is what Ken did to it!

typedef	struct _QSDTDP
{
	DWORD	qs;			// QS
	DTDP	dtdp;		// Data Type and Data Pointer
}
QSDTDP,*PQSDTDP,*LPQSDTDP;

#endif

#if !defined(_ATPBS_H_)
#include "atpbs.h"
#endif

#if !defined(__ADV_SERVER_REQUESTS)
#include <atadvreq.h>
#endif

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

		/*----------------------------------------*/
		/* Define the Clipboard Price Block Link. */
		/*----------------------------------------*/

#define CF_TYPE_PB				0
#define CF_TYPE_PB_NEWS			1
#define CF_TYPE_PROG			2
#define CF_TYPE_DROP			3
#define CF_TYPE_DROP_NEWS		4
#define CF_TYPE_DROP_NEWS_INFO	5

#define CF_TYPE_CPAGE			10

#define CF_CHANNEL_1			0x01
#define CF_CHANNEL_2			0x02
#define CF_CHANNEL_3			0x04
#define CF_CHANNEL_4			0x08
#define CF_CHANNEL_5			0x10
#define CF_CHANNEL_6			0x20
#define CF_CHANNEL_7			0x40
#define CF_CHANNEL_8			0x80
#define CF_CHANNEL_ALL			0xFF

#define SIZEOF_CFHEADER			4 * sizeof(DWORD)

typedef	struct
		{
		DWORD	dwType;					// Type of link block.
		DWORD	dwLength;				// Total length of link block.
		DWORD	curQSN;					// Current QuoteServer number
		DWORD	channel;				// Link channels.
		PB		pb;						// Price block.
		}
		CF_LINK;

typedef	CF_LINK *LPCF_LINK;

typedef	struct
		{
		DWORD	dwType;					// Type of link block.
		DWORD	dwLength;				// Total length of link block.
		DWORD	curQSN;					// Current QuoteServer number
		DWORD	channel;				// Link channels.
		char	szString[64];			// News search.
		}
		CF_NEWS;

typedef	struct
		{
		DWORD		dwType;		 		// Type of link block.
		DWORD		dwLength;	 		// Total length of link block.
		DWORD		curQSN;		 		// Current QuoteServer number
		DWORD		channel;	 		// Link channels.
		ATSTORYINFO	storyInfo;			// Story Info.
		}
		CF_NEWS_INFO;

typedef	struct
		{
		DWORD		dwType;	 			// Type of link block.
		DWORD		dwLength;			// Total length of link block.
		BYTE		TypeID[2];			// Page Type ID.
		int			xBias;	 			// X Bias for page display.
		int			yBias;	 			// Y Bias for page display.
		int			width;	 			// Width  for window.
		int			height;	 			// Height for window.
		COLORREF	crColor[12];		// Colors for this window.
		LOGFONT		lf;					// Selected font.
		char		szPage[80];			// User entry page/comment.
		}
		CF_CPAGE;

/*------------------------------------------------------------------------*/

		/* NASDAQ Level 2 QuoteBlocks */

#define BATRSZ_TREND_UNCH	0
#define BATRSZ_TREND_UP		1
#define BATRSZ_TREND_DOWN	2
			
typedef	struct
		{
		WORD	size  : 14,
				trend : 2;
		}
		BATRSZ;
		
typedef	union
		{
		BATRSZ	ts;
		WORD	v;
		}
		BATSV;

typedef	struct	_SPQB			// Data Types DT_SP_LEVEL2_A & DT_SP_LEVEL2_N
		{
		char	Symbol[5];		// Stock symbol
		char	MMid[3];		// Market Maker ID (3 of 4 characters)
		WORD	dataptr;
		char	MMid2;			// The fourth character of the MMid
		char    space;			// Always blank
		char	Exchange;		// 'Q'=nasdaq, 'S'=small cap
		BYTE	OriginMKT;		// Origin market
		BYTE	QuoteType;		// Quote Type
		BYTE	QuoteCondition;	// Quote Condition
		BYTE	BidPrice[3];	// Bid price in 64ths
		BYTE	AskPrice[3];	// Ask price in 64ths
		BATSV	BidTSV;			// Bid Size/Trend Value
		BATSV	AskTSV;			// Bid Size/Trend Value
		WORD	Time;			// Hours * 100 + minutes (high bit == NotToday!)
		BYTE	extra;			// ** not used
		}
		SPQB, FAR *LPSPQB;

		/* Statistics - synthetic blocks */

typedef	struct
		{
		BYTE	updType;			// Will always be 'R'
		BYTE	datatype;			// Statistics is TYPE 5
		char	symbol[8];
		BYTE	group;
		BYTE	type;
		DWORD	up;
		DWORD	down;
		DWORD	unch;
		BYTE	exponent;
		BYTE	GMC;
		long	net;
		BYTE	name[20];
		}
		STATBLOCK;

/*
**** QSM_PBLIST (Price Block List) structures
*/

typedef	struct
		{
		BYTE	a;					// lower case 'a'
		BYTE	datatype;			// data type
		BYTE	sym[8];				// symbol
		WORD	dataptr;			// data pointer
		}
		PBID;						// Price Block ID


typedef	struct
		{
		BYTE	A;					// Message type 'A'
		BYTE	datatype;			// data type
		WORD	nSrchLen;			// Zero for first buffer, then set to 
									// number of "const" chars in symbol
		BYTE	sym[16];				// Symbol to search for
		}
		QSM_PBLISTS;				// Message sent to QuoteServer
		
typedef	struct
		{
		BYTE		A;				// Returned Message type
		BYTE		dtype;			// Returned data type
		WORD		nList;			// Number of items in list
		PBID		list[44];		// List of PBID''s WADE CHAGED FROM 48
		}
		QSM_PBLISTR;

	/*-----------------------------*/
	/* PC Quote (Big A equivalent) */
	/*-----------------------------*/

typedef	struct
		{
		char	sym[11];	// PCQuote Symbols are 11 bytes long
		WORD	dataptr;	// Data pointer associated with this
		}
		PQID;

typedef	struct
		{
		BYTE	J;			// Message type is 'J'
		BYTE	datatype;	// Datatype: (DT_PQ_US_BONDS..DT_PQ_AS_STKOPTS)
		WORD	nSearch;	// Zero for first rq, then set to # const chars
		char	sym[16];	// Symbol to look for
		}
		QSM_PQLISTS;

typedef	struct
		{
		BYTE	J;			// Message type is 'J'
		BYTE	datatype;	// Datatype: (DT_PQ_US_BONDS..DT_PQ_AS_STKOPTS)
		WORD	nList;		// Number of items in list
		PQID	list[40];	// List of PQID''s
		}
		QSM_PQLISTR;

#define BFLG_LBLOCK_LPRICES		0		// long block, long prices
#define BFLG_SBLOCK_LPRICES		1		// short block, long prices
#define BFLG_LBLOCK_SPRICES		2		// long block, short prices
#define BFLG_SBLOCK_SPRICES		3		// short block, short prices
#define BFLG_SHORT				4		// shortened block
#define BFLG_MMSHORT			5		// full block
#define BFLG_OLDVALUES			0x80	// old option values have been returned

#define BMSG_OLDVALUES			0x01	// request old options values

typedef	struct
		{
		BYTE	B;			// 'B'={get block, go live} 'b'={just get block}
		BYTE	datatype;	// Data type    of block request
		WORD	glBits;		// Go Live bits: bit 0=dptr[0].Only 14 bits used
		BYTE	count;		// Number of data pointers in dptrs[]
		BYTE	flag;		// Option bits (alternate formats,etc)
		WORD	dptrs[16];	// Data pointer of block request
		}					// Non-stock options only use twelve,MktMkrs use all sixteen.
		QSM_RQB, *LPQSM_RQB;

		// Note: Apps should always use 'B', ATSERVER will change the
		//       B --> b if this WS is already advised on the QSDTID

typedef	struct
		{
		BYTE	B;
		BYTE	datatype;	// Data type of blocks requested
		BYTE	count;		// Number of data pointers in dptrs[]
		BYTE	data[516];	// Max size is 12 block image records.
		}
		QSM_RSB, *LPQSM_RSB;

// These are the "possible" types of records in the QSM_RSB.data[] 

typedef	struct	_B0_LBLP			// Long Block Long Prices (like reg.pb)
		{
		BYTE	flag;				// Flag = 0
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		BYTE	ysettle[3];
		short	last;				// offset from ysettle
		short	high;				// offset from ysettle
		short	low;				// offset from ysettle
		short	bid;				// offset from ysettle
		short	ask;				// offset from ysettle
		short	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		}
		B0_LBLP, *LPB0_LBLP;

typedef	struct	_B1_SBLP			// Short Block Long Prices
		{							//   No Last/High/Low/Volume/Last Size
		BYTE	flag;				// Flag = 1
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		BYTE	ysettle[3];
		short	bid;				// offset from ysettle
		short	ask;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	openint[3];			// Open Interest
		WORD	timequote;			// Time of last Quote
		}
		B1_SBLP, *LPB1_SBLP;

typedef	struct	_B2_LBSP			// Long Block Short Prices
		{
		BYTE	flag;				// Flag = 2
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		WORD	ysettle;			// Note: 2 byte ysettle & 1 byte offs.
		char	last;				// offset from ysettle
		char	high;				// offset from ysettle
		char	low;				// offset from ysettle
		char	bid;				// offset from ysettle
		char	ask;				// offset from ysettle
		char	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		WORD	volume;				// Note: 2 byte Volume
		WORD	openint;			// Note: 2 byte Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		}
		B2_LBSP, *LPB2_LBSP;

typedef	struct	_B3_SBSP			// Short Block Short Prices
		{
		BYTE	flag;				// Flag = 3
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		WORD	ysettle;			// Note: 2 byte ysettle & 1 byte offs.
		char	bid;				// offset from ysettle
		char	ask;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		WORD	openint;			// Note: 2 byte Open Interest
		WORD	timequote;			// Time of last Quote
		}
		B3_SBSP, *LPB3_SBSP;

typedef	struct	_B4_BLKI			// Block Image - Non Stock Option.
		{
		BYTE	flag;				// Flag = 4
		WORD	dataptr;			// used to make qsdtid
		BYTE	data[40];			// Block image, 40 bytes from ysettle.
		}
		B4_BLKI, *LPB4_BLKI;

typedef	struct	_B5_MMKR			// Market Makers
		{
		BYTE	flag;				// Flag = 5
		WORD	dataIndex;			// used to verify MMID
		BYTE	data[15];			// Market Maker Block (15 bytes)
		}
		B5_MMKR, *LPB5_MMKR;

typedef	struct	_B6_LBLPOV			// Long Block Long Prices w/ Old values
		{
		BYTE	flag;				// Flag = 0x80
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		BYTE	ysettle[3];
		short	last;				// offset from ysettle
		short	high;				// offset from ysettle
		short	low;				// offset from ysettle
		short	bid;				// offset from ysettle
		short	ask;				// offset from ysettle
		short	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	volume[3];			// Actual value
		BYTE	openint[3];			// Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		WORD	yestVol;			// Yesterday volume
		BYTE	oldDecimal;			// Decimal type for old stuff
		WORD	oldBid;				// Previous day
		WORD	oldAsk;				// Previous day
		}
		B6_LBLPOV;

typedef	struct	_B7_SBLPOV			// Short Block Long Prices w/ Old values
		{							//   No Last/High/Low/Volume/Last Size
		BYTE	flag;				// Flag = 0x81
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		BYTE	ysettle[3];
		short	bid;				// offset from ysettle
		short	ask;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		BYTE	openint[3];			// Open Interest
		WORD	timequote;			// Time of last Quote
		WORD	yestVol;			// Yesterday volume
		BYTE	oldDecimal;			// Decimal type for old stuff
		WORD	oldBid;				// Previous day
		WORD	oldAsk;				// Previous day
		}
		B7_SBLPOV;

typedef	struct	_B8_LBSPOV			// Long Block Short Prices w/ old values
		{
		BYTE	flag;				// Flag = 0x82
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		WORD	ysettle;			// Note: 2 byte ysettle & 1 byte offs.
		char	last;				// offset from ysettle
		char	high;				// offset from ysettle
		char	low;				// offset from ysettle
		char	bid;				// offset from ysettle
		char	ask;				// offset from ysettle
		char	open;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		WORD	volume;				// Note: 2 byte Volume
		WORD	openint;			// Note: 2 byte Open Interest
		WORD	timelast;			// Time of last Trade
		WORD	timequote;			// Time of last Quote
		WORD	lastsize;			// Size of last
		WORD	yestVol;			// Yesterday volume
		BYTE	oldDecimal;			// Decimal type for old stuff
		WORD	oldBid;				// Previous day
		WORD	oldAsk;				// Previous day
		}
		B8_LBSPOV;

typedef	struct	_B9_SBSPOV			// Short Block Short Prices w/ Old values
		{
		BYTE	flag;				// Flag = 0x83
		BYTE    quoteCondition;
		BYTE    tradeCondition;
		WORD	ysettle;			// Note: 2 byte ysettle & 1 byte offs.
		char	bid;				// offset from ysettle
		char	ask;				// offset from ysettle
		BYTE	dectype;			// Decimal type
		BYTE	dectypeSHL;			// Decimal type of Strike,cont(h/l)
		WORD	ostrike;			// Strike
		WORD	oexprdate;			// Expiration date
		WORD	openint;			// Note: 2 byte Open Interest
		WORD	timequote;			// Time of last Quote
		WORD	yestVol;			// Yesterday volume
		BYTE	oldDecimal;			// Decimal type for old stuff
		WORD	oldBid;				// Previous day
		WORD	oldAsk;				// Previous day
		}
		B9_SBSPOV;

typedef	union
		{
		B0_LBLP		lblp;
		B1_SBLP		sblp;
		B2_LBSP		lbsp;
		B3_SBSP		sbsp;
		B4_BLKI		blki;
		B5_MMKR		mmkr;
		B6_LBLPOV	lblpov;
		B7_SBLPOV	sblpov;
		B8_LBSPOV	lbspov;
		B9_SBSPOV	sbspov;
		}
		B_BLOCK, *LPB_BLOCK;

// Combo Price and Fundamental data messages

#define PFI_QUICK1_FP			0x8000		// Quick1 for front page data
#define PFI_QUICK1_EB			0x8001		// Quick1 for earnings/balance data (S&P)
#define PFI_QUICK1_HS			0x8002		// Quick1 for history data (S&P)

#define PFI_NEWS_INFO			0x4000		// Full News Info request

typedef	struct
		{
		BYTE	i;				// Price & Fund info request
		BYTE	reqType;		// Request Type: CTFX...etc
		WORD	bmFields;		// Bitmap of "additional" Fundamental fields
		BYTE	datatype;		// Datatype of pb (if known, else 0xff)
		char	sSymbol[11];	// Formatted or User entered symbol 
		char	sCCE[3];		// Country Code/Exchange 
		}
		QSM_PFI_RQ;

typedef	struct
		{
		BYTE	i;				// Price & Fund info request
		BYTE	reqType;		// Request Type: CTFX...etc
		BYTE	nFundType;		// Fundamental data type(See Table)
		BYTE	nExtra;			// Extra
		BYTE	datatype;		// Datatype of pb
		char	sSymbol[11];	// Symbol (found or same as given)
		WORD	dataptr;		// Data pointer
		BYTE	pData[52];		// Price data (starting at ysettle)
		BYTE	fData[466];		// Fundamental data
		}
		QSM_PFI_RS;

typedef struct 
		{
		BYTE  i;
		BYTE  K;
		WORD  bmFields;
		BYTE  fData[530];
		}
		QSM_PFK_RS;

	/*-----------------------------*/
	/* Request symbol lists by CCE */
	/*-----------------------------*/

typedef	struct
		{
		BYTE	j;			// Message type is 'j'.
		BYTE	datatype;	// Datatype: (DT_PQ_US_BONDS..DT_PQ_AS_STKOPTS)
		WORD	nSearch;	// Zero for first req. then # of const chars.
		char	sym[8];		// Symbol to begin or greater.
		char	country[2];	// Country Code to filter on.
		BYTE	exchange;	// Optional exchange. (Blank or Exchange Code).
		}
		QSM_CCELISTS;

	/*---------------------------------------*/
	/* Response from symbol list req. by CCE */
	/*---------------------------------------*/

typedef	struct
		{
		char	sym[8];		// PCQuote style Symbol.
		char	country[2];	// Country Code.
		BYTE	exchange;	// Exchange.
		WORD	dataptr;	// Data pointer.
		}
		LISTID;

typedef	struct
		{
		BYTE	j;			// Message type is 'j'
		BYTE	datatype;	// Datatype: (DT_PQ_US_BONDS..DT_PQ_AS_STKOPTS)
		WORD	nList;		// Number of items in list
		LISTID	list[36];	// List of PQID''s
		}
		QSM_CCELISTR;

	/*-----------------------*/
	/* Get a retrans message */
	/*-----------------------*/

typedef	struct
		{
		BYTE	X;						// Retrans Message "Send"
		BYTE	SeqNumber;
		}
		QSM_RTS;

typedef	struct
		{
		BYTE	X;						// Retrans Message "Response"
		BYTE	SeqNumber;
		BYTE	MsgType,MsgSubType;
		WORD	PacketLength;
		BYTE	Seq,Ses;
		WORD	Extra;
		WORD	MsgLength;
		BYTE	Message[246];
		}
		QSM_RTR;

typedef	struct
		{
		BYTE	E;						// Eudaemonic message
		BYTE	Source;					// R=Reuters Select Feed
		BYTE	Type;					// 'P'age or 'Q'uote
		BYTE	Status;					// 'L'ive or 'S'napshot
		WORD	id;						// ID number (returned)
		char	szID[20];				// Page name or Symbol
		}
		QSM_GRQ;

typedef	struct
		{
		BYTE	H;						// Haul in a Reuters page
		BYTE	Source;					// R = Reuters
		BYTE	Status;					// (returned = 'E' means done)
		BYTE	Action;					// L = Live,S=Snap,K=Kill it
		WORD	Page;					// Page number (base at 1)
		BYTE	StartLine;				// Based at 1
		BYTE	EndLine;				// (returned)
		BYTE	text[480];
		}
		QSM_RPS;

typedef	struct
		{
		BYTE	h;						// haul in a Reuters block
		BYTE	Source;					// R = Reuters
		BYTE	Type;					// not used
		BYTE	Action;					// L = Live,S=Snap,K=Kill it
		WORD 	dataptr;				// block number
		BYTE	data[400];				// RSF LB  (excl. Sym & dataptr)
		}
		QSM_RBQ;

// Reuters Synthetic Block (C)reate/(U)pdate/(K)ill message

typedef	struct
		{
		BYTE	Y;				// 'Y' - because we like you
		BYTE	command;		// (C)reate,(U)pdate,(K)ill
		BYTE	source;			// 'R' = Reuters
		BYTE	status;			// 0 on send, status on recv
		char	Symbol[8];
		char	Page[4];		// Reuters "page name", example: WRLD
		BYTE	DecType;
		BYTE	PriceType;
		BYTE	Line;
		BYTE	ColStart;
		BYTE	Flags;
		BYTE	ColEnd;
		char	Name[20];
		char	Key[12];
		char	PageExt[6];		// Reuters "page name", extension
		}
		QSM_SSE;

typedef	struct
		{
		BYTE	E;						// Eudaemonic message 'E'
		BYTE	Source;					// 'T'=Telerate Digital Page Feed
		BYTE	Type;					// 'P'age 'F'or. 'B'ack or 'T'ime
		BYTE	Status;					// 'L'ive
		WORD	id;						// Page number or time.
		WORD	HPage;					// Headline page number.
		}
		QSM_TRQ;

typedef	struct
		{
		BYTE	E;						// Eudaemonic message 'E'
		BYTE	Source;					// 'T'=Telerate Digital Page Feed
		BYTE	Type;					// 'E'xtended Service
		BYTE	Status;					// 'L'ive
		WORD	ByteCount;				// Number of characters.
		char	Text[80];				// Message Text.
		}
		QSM_ERQ;

typedef	struct
		{
		BYTE	E;						// Eudaemonic message
		BYTE	Source;					// C=Crossmar Page Viewer
		BYTE	Type;					// 'P'age 'F'or. 'B'ack
		BYTE	Status;					// 'L'ive
		WORD	id;						// Page id for 'F' and 'B'.
		char	szPageNumber[16];		// Page number.
		}
		QSM_CRQ;

	/***** This is the layout of a change record (asynchronous) *****/

typedef	struct
		{
		char	updType;			// Can be R,b,e,f,j,o,p,s,t,v
		BYTE	datatype;			// Data type
		BYTE	sym[8];
		WORD	dataptr;			// Data pointer
		BYTE	ysettle[3];			// Yesterday''s settle
		SHORT	last;				// offset from ysettle
		SHORT	high;				// offset from ysettle
		SHORT	low;				// offset from ysettle
		SHORT	bid;				// offset from ysettle
		SHORT	ask;				// offset from ysettle
		SHORT	open;				// offset from ysettle
		WORD	bidsize;			// Actual value
		WORD	asksize;			// Actual value
		WORD	lastsize;			// Actual value  20
		BYTE	volume[3];			// Actual value
		WORD	timelast;			// time of last trade
		WORD	timequote;
		BYTE	QtCond;				// Quote Condition
		BYTE	status;
		SHORT	compptr;			// ignore this
		SHORT	fundptr;			// ignore this
		BYTE	gmclsop;
		BYTE	gmchilo;
		BYTE	gmcbidask;			// global mkt center bid/ask
		BYTE	trendflag;
		WORD	tosptr;
		BYTE	filler[12];
		}
		UPDATE_PB;

// The new "Shorter" update records

typedef	struct
		{
		char	Type;				// 'T'= short Trade, 'Q' = short Quote
		BYTE	datatype;			// Data Type
		WORD	dataptr;			// Data Pointer
		BYTE	ohllTbac;			// OHLL = High nibble: Open/High/Low/Last
									// Tbac = Low  nibble: TRBATE action code
		BYTE	dectype;			// Decimal type
		}
		QSA_T, *LPQSA_T;

#define	QSAT_OPEN	0x80
#define	QSAT_HIGH	0x40
#define	QSAT_LOW	0x20
#define	QSAT_LAST	0x10

#define	TBAC(a)	(a & 0x0f)

#define TBAC_NONE	0		// No special action, use default T or Q
#define	TBAC_BATE	1		// Record contains new bate code
#define	TBAC_BTSC	2		// Record contains new bate + status & condition	

		/*------ Trades ------*/

typedef	struct
		{
		QSA_T	qsat;
		WORD	last;				// Offset from ysettle
		WORD	lastsize;			// last size
		BYTE	volume[3];			// total volume
		}
		QSA_TBOND, *LPQSA_TBOND;

typedef	struct
		{
		QSA_T	qsat;
		WORD	last;				// Offset from ysettle
		WORD	ticount;			// Tick Count
		char	trbate;				// Present if Type == 1
		}
		QSA_TFUTOPT, *LPQSA_TFUTOPT;

typedef	struct
		{
		QSA_T	qsat;
		WORD	last;				// Offset from ysettle
		WORD	ticount;			// Tick Count
		WORD	timetrade;			// Time of last trade
		BYTE	trendflag;			// 0 = unch,1=up,2=down
		char	trbate;				// Present if Type == 1
		}
		QSA_TFUTURE, *LPQSA_TFUTURE;

typedef	struct
		{
		QSA_T	qsat;
		WORD	last;				// Offset from ysettle
		WORD	lastsize;			// last size
		WORD	timelast;			// Time of last trade
		DWORD	dwVolume;			// Total Volume -- need to split it up
		BYTE	trendflag;			// 0 = unch,1=up,2=down, alerts (S&P)
		BYTE	gmclsop;			// {Tbac == 1 or 2}
		BYTE	status;				// {Tbac == 2}
		BYTE	TrCond;				// {Tbac == 2} Trade Condition
		}
		QSA_TSTOCK, *LPQSA_TSTOCK;

typedef struct
		{
		QSA_T	qsat;
		BYTE    price[3];			// price (Intel order)
		BYTE    size[3];			// size (Intel order)
		WORD    timecond;			// time of trade (hour*100+minute)
		BYTE    condition;			// condition, see below
		}
		QSA_CONDITION, *LPQSA_CONDITION;

//
// conditions:
//
//		33		insert
//		10		cancel
//		'G'		open/reopen trade detail (non-NASDAQ)
//		'G'		bunched sold (NASDAQ)
//		'H'		intraday trade detail
//		'I'		basket index on close
//		'K'		rule 155
//		'T'		form 'T'
//		'Z'		sold out of sequence
//

typedef	struct
		{
		QSA_T	qsat;
		WORD	last;				// Offset from ysettle
		WORD	lastsize;			// last size
		WORD	timelast;			// Time of last trade
		BYTE	volume[3];			// Volume
		}
		QSA_TSTKOPT, *LPQSA_TSTKOPT;

		/*------ Quotes ------*/

typedef	struct
		{
		QSA_T	qsat;
		SHORT	bid;				// Offset from ysettle
		SHORT	ask;				// Offset from ysettle
		WORD	bidsize;			// Size of bid
		WORD	asksize;			// Size of ask
		}
		QSA_QBOND, *LPQSA_QBOND;

typedef	struct
		{
		QSA_T	qsat;
		SHORT	bid;				// Offset from ysettle
		SHORT	ask;				// Offset from ysettle
		}
		QSA_QFUTOPT, *LPQSA_QFUTOPT;// Note: Set trbate = 'Q'

typedef	struct
		{
		QSA_T	qsat;
		SHORT	bid;				// Offset from ysettle
		SHORT	ask;				// Offset from ysettle
		}
		QSA_QFUTURE, *LPQSA_QFUTURE;// Note: Set trbate = 'Q'

typedef	struct
		{
		QSA_T	qsat;
		SHORT	bid;				// Offset from ysettle
		SHORT	ask;				// Offset from ysettle
		WORD	bidsize;			// Size of bid
		WORD	asksize;			// Size of ask
		WORD	timequote;
		BYTE	gmcbidask;			// {Tbac == 1 or 2}
		BYTE	status;				// {Tbac == 2}
		BYTE	QtCond;				// {Tbac == 2} Quote Condition		
		}
		QSA_QSTOCK, *LPQSA_QSTOCK;

typedef	struct
		{
		QSA_T	qsat;
		SHORT	bid;				// Offset from ysettle
		SHORT	ask;				// Offset from ysettle
		WORD	timequote;			// Time of last Quote
		}
		QSA_QSTKOPT, *LPQSA_QSTKOPT;

typedef struct
		{
		char	Type;				// 'T'= News Info Update
		BYTE	datatype;			// Data Type - DT_NEWS_INFO
		WORD	dataptr;			// Data Pointer
		ATSTORYINFO	storyInfo;		// Latest Story Info.
		}
		QSA_NEWSINFO, *LPQSA_NEWSINFO;

		/*------ Trades, Quotes and Conditions Union ------*/

typedef	union
		{
		QSA_T			qsat;
		QSA_TBOND		qsatBond;
		QSA_TFUTOPT		qsatFutopt;
		QSA_TFUTURE		qsatFuture;
		QSA_TSTOCK		qsatStock;
		QSA_TSTKOPT		qsatStkopt;
		QSA_QBOND		qsaqBond;
		QSA_QFUTOPT		qsaqFutopt;
		QSA_QFUTURE		qsaqFuture;
		QSA_QSTOCK		qsaqStock;
		QSA_QSTKOPT		qsaqStkopt;
		QSA_CONDITION	qsacStock;
		QSA_NEWSINFO	qsatNewsInfo;
		}
		QSA_UPDATE, *LPQSA_UPDATE;

typedef	struct
		{
		BYTE	G;					// Message type 
		BYTE	OnOff;				// 'L'ive or 'D'ead
		BYTE	DataType;			// DataType
		WORD	DataPtr;			// DataPtr
		}
		QSM_ADVISE;

#define	MAX_GLD	168

typedef	struct
		{
		BYTE	dt;					// datatype.
		WORD	id;					// dataptr.
		}
		DTID;

typedef	struct
		{
		BYTE	g;					// Message type 
		BYTE	LD;					// 'L'ive or 'D'ead
		BYTE	count;				// dtid count.
		DTID	dtid[MAX_GLD];		// dtid array.
		}
		QSM_GLD;

//	Go Live/Dead on Market Makers For given symbol.

typedef	struct
		{
		BYTE	g;					// Message type 'g' 
		BYTE	OnOff;				// 'l'ive or 'd'ead
		BYTE	Flag;				// 'M'=MarketMakers
		BYTE	DataType;			// DataType
		char	Symbol[12];			// Underlying Symbol
		DWORD	DataPtr;			// DataPtr.
		}
		QSM_ENUM;

#define MAX_MMU						264

typedef struct 
		{
		BYTE    g, b;				// Message type
		BYTE    action;				// 'L' - Live, 'D' - Dead
		BYTE    datatype;			// Data type of all symbols
		WORD    count;				// # of symbols in list
		WORD    dataptrs[MAX_MMU];	// List of NASDAQ symbols
		}
		QSM_RQ_GBLD, *LPQSM_RQ_GBLD, *PQSM_RQ_GBLD;

typedef struct 
		{
		BYTE  g, b;					// Message type
		BYTE  action;				// 'L' - Live, 'D' - Dead
		BYTE  datatype;				// Data type of all symbols
		WORD  count;				// # of symbols in list
		}
		QSM_RS_GBLD, *LPQSM_RS_GBLD, *PQSM_RS_GBLD;

/*---------------------------------------------------------------------*/
// Big Block Trades Go Live/Dead.

typedef	struct
		{
		BYTE		g,B;			// Message Type.
		BYTE		action;			// 'L' - live, 'D' - dead, 'T' - test.
		WORD		lastsize;		// Minimum trade size.
		}
		QSM_BB_GLD, *LPQSM_BB_GLD;

typedef	struct
		{
		char			Z;
		char			S;
		WORD			Version;
		char			VersionExtension;
		unsigned char	Session;
		unsigned long	MessageCount;
		unsigned long	ErrorCount;
		char			DataSource;
		}
		QSM_INFO;

// Message used to do DBAX() functions on a PC Quote type QuoteServer

typedef	struct
		{
		BYTE	Q,X;				// Message type and sub-type
		WORD	status;				// Returned status
		WORD	function;			// DBAX function
		WORD	dbn;				// Data base number (used by function 0 only)
		char	pblk[256];			// Btrieve parameter block
		char	key[24];			// Key
		char	buffer[242];		// Returned record
		}
		QSM_QX;

typedef	struct {
		char	Z,D;				// Message ZD
		char	type;				// Type of items desired
		WORD	count;				// Returned count of items
		WORD	size;				// Size of items
		char	items[256];			// Items are different size
} QSM_ZDINFO;


// Items returned from a ZDD message (DEVCAPS:Databases)
// Max 11 of these structures returned in the items area

typedef	struct
		{
		WORD	dbn;
		char	szName[20];
		}
		ZD_DB;

#ifndef _FIND_T_DEFINED

#ifndef RC_INVOKED
#pragma pack(2)
#endif /* RC_INVOKED */

/* _dos_findfirst structure */

struct _find_t {
		char            reserved[21];
		char            attrib;
		unsigned short  wr_time;
		unsigned short  wr_date;
		long            size;
		char            name[13];
};

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#ifndef __STDC__
/* Non-ANSI name for compatibility */
#define find_t _find_t
#endif

#define _FIND_T_DEFINED
#endif

typedef	struct
		{
		BYTE	Q,r;
		WORD	wLen;
		DWORD	dwOffset;
		char	szFN[64];
		}
		Qr_REQ;

typedef	struct
		{
		BYTE	Q,r;
		WORD	wLen;			// Actual read length
		char	data[512];
		}
		Qr_RSP;

typedef	struct
		{
		BYTE	Q,One;				// 'Q1' - Find first command
		WORD	Attribute;			// Attributes of file to find
		char	szFilename[80];		// Path and filename with Wild-cards
		}
		Q1_REQ;

typedef	struct
		{
		BYTE			Q,One;		// 'Q1' - Find first command
		WORD			Status;		// status of find first function
		struct _find_t	Info;		// Info returned from call
		}
		Q1_RSP;

typedef	struct
		{
		BYTE			Q,Two;		// 'Q2' - Find next command
		WORD			reserved;	// ** not used **
		struct _find_t	Info;		// Info passed to call
		}
		Q2_REQ;

typedef	struct
		{
		BYTE			Q,Two;		// 'Q2' - Find next command
		WORD			Status;		// status of find first function
		struct _find_t	Info;		// Info returned from call
		}
		Q2_RSP;

typedef	struct
		{
		unsigned	day  :5,
					month:4,
					year :7;		// Add 1980
		}
		DOSDATE;

typedef	union
		{
		unsigned	value;
		DOSDATE		date;
		}
		DOSFILEDATE;

typedef	struct
		{
		unsigned	second:5,		// Multiply by two
					minute:6,
					hour:  5;
		}
		DOSTIME;

typedef	union
		{
		unsigned	value;
		DOSTIME		time;
		}
		DOSFILETIME;

typedef	struct
		{
		char	Q;
		char	O;
		char	szFilename[64];
		}
		QSOPENS;

typedef	struct
		{
		char	Q;
		char	O;
		SHORT	status;
		SHORT	fh;
		}
		QSOPENR;

typedef	struct
		{
		char	Q;
		char	D;
		DWORD	dwOffset;
		SHORT	fh;
		}
		QSREADS;

typedef	struct
		{
		char	Q;
		char	D;
		SHORT	actual;
		SHORT	fh;
		char	data[512];
		}
		QSREADR;

typedef	struct
		{
		char	Q;
		char	S;
		SHORT	fh;
		DWORD	offset;
		BYTE	origin;
		}
		QSSEEKS;

typedef	struct
		{
		char	Q;
		char	S;
		SHORT	status;
		DWORD	offset;
		}
		QSSEEKR;

typedef	struct
		{
		char	Q;
		char	C;
		SHORT	fh;
		}
		QSCLOSE;

typedef	struct
		{
		char	Q;
		char	W;
		SHORT	fh;
		SHORT	nbytes;
		char	data[256];
		}
		QSWRITES;

typedef	struct
		{
		char	Q;
		char	W;
		SHORT	status;
		}
		QSWRITER;

		/* BTREE Access Structures */

typedef	struct
		{
		char	Q;
		char	B;
		char	O;
		char	szIdxName[64];
		}
		QSOPENBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	O;
		SHORT	tree;
		}
		QSOPENBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	C;
		SHORT	tree;
		}
		QSCLOSEBT;

typedef	struct
		{
		char	Q;
		char	B;
		char	N;
		SHORT	tree;
		}
		QSNEXTBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	N;
		DWORD	ptr;
		}
		QSNEXTBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	K;
		SHORT	tree;
		}
		QSKEYVBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	K;
		char	szKeyValue[64];
		}
		QSKEYVBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	L;
		SHORT	tree;
		char	szKeyValue[64];
		}
		QSLOCTBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	L;
		DWORD	ptr;
		}
		QSLOCTBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	W;
		SHORT	tree;
		}
		QSWILDBT;

typedef	struct
		{
		char	Q;
		char	B;
		char	P;
		SHORT	tree;
		}
		QSPREVBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	P;
		DWORD	ptr;
		}
		QSPREVBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	F;
		SHORT	tree;
		}
		QSFIRSTBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	F;
		DWORD	ptr;
		}
		QSFIRSTBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	E;
		SHORT	tree;
		}
		QSLASTBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	E;
		DWORD	ptr;
		}
		QSLASTBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	S;
		SHORT	tree;
		}
		QSKSIZEBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	S;
		WORD	kSize;
		}
		QSKSIZEBTR;

typedef	struct
		{
		char	Q;
		char	B;
		char	R;
		SHORT	tree;
		}
		QSCURRBTS;

typedef	struct
		{
		char	Q;
		char	B;
		char	R;
		DWORD	ptr;
		}
		QSCURRBTR;

typedef	struct
		{
		char	szFundKey[6];
		char	szFundName[36];
		}
		MMFREC;


#define MAX_QSNUM			32
#define MAX_IP_PACKET		1492	// Largest UDP/TCP packet sent/recv'd

#define QST_NONE			0
#define QST_IPXSPX			2
#define QST_TCPIP			3
#define QST_UDP				4
#define	QST_VIRTUAL			5

#define NO_QS_BACKUP	0xFFFFFFFF	// no backup for this QS (idMirror in QS_INFO)

	// These values are used to indicate what kind of MM database a
	// NASDAQ Level II QS -or- a S&P ComStock QS is supporting in nMMSmartQS

#define	QSIQ_UNKNOWN		0
#define QSIQ_DUMB			1
#define	QSIQ_SMART_EM		2
#define	QSIQ_SMART_EN		3

	// These values are used to indicate what kind of ATOPTION file
	// a server is supporting in nATOption

#define	QSATOPT_UNKNOWN		0
#define	QSATOPT_NONE		1
#define	QSATOPT_ATOPTNUS	2
#define	QSATOPT_ATOPTION	3

	// These values are used to indicate what kind of US stock option
	// database a server is supporting in nStkOptQS

#define	QSOPT_UNKNOWN		0
#define QSOPT_SINGLE		1	// Very old and very new
#define	QSOPT_TWO			2	// _A and _N
#define	QSOPT_THREE			3	// _AG, _HQ, and _RZ
#define	QSOPT_FOUR			4	// _AE, _FM, _NR, and _SZ
#define	QSOPT_THREEA		5	// _AG + H, _HQ, and QQ +_RZ

	// These values are used to indicate what kind of services are
	// supported in cmpNameFlags.

#define	QSCN_UNKNOWN		0	// Old login/No compression.
#define QSCN_NEW_LOGIN		1	// Supports twenty character login ids.
#define QSCN_SMART_QS		2	// Supports compression messages. (ie Smart)
#define	QSCN_LONG_LOGIN		4	// Has twenty character login ids.

typedef struct _QS_INFO
		{
		BOOL   connection;			// true if initialized
		DWORD  qsn;					// quote server number (app side)
		DWORD  connectType;			// QS connection type
		BYTE   nodeAddr[10];		// QS Net/Node number
		union
		{
			BYTE   update[2];	 	// QS Update Socket number
			WORD   wUpdate;
		};
		union
		{
			BYTE   ticker[2];	 	// QS Tick   Socket number
			WORD   wTicker;
		};
		union
		{
			BYTE   request[2]; 		// QS SPX Connection number
			WORD   wRequest;
		};
		BYTE   extra1[97];			//   ** not used
		DWORD  groupAddress;		// Multicast group addr (used for updates)
		BYTE   bBigRequests;		// QS supports big requests
		BYTE   compVersion;			// compression version
		BYTE   bHaltResume;			// QS supports Halt/Resume Messaging
		BYTE   bAlertMsg;			// QS supports Alert Messaging
		BYTE   nATOption;			// Type of ATOPTION file, values = QSATOPT_xx
		WORD   wWSVersionNum;		// Latest workstation version number
		DWORD  type;				// QS Types (0xff is no-type)
		BYTE   status;				// bitmap status byte *see below
		BYTE   restType;			// QS Restricted type?
		WORD   versionNum;			// QS Version number
		BYTE   versionLet;			// QS Version letter
		BYTE   lastSeq;				// last Sequence byte recv''d
		WORD   nConnects;			// number of times connect/reconnect
		DWORD  maxConnects;			// maximum connections supported
		DWORD  idMirror;			// QS# that this QSLink backs up
		short  iLogStatus;			// Value of last "Login" request
		short  iExtra;				// Unused 16-bits
		BYTE   nMMSmartQS;			// Type of MM database, values = QSIQ_xxx
		BYTE   cmpNameFlag;			// Compression/Name length flag
		BYTE   nStkOptQS;			// Type of US StkOpt db, values = QSOPT_xx
		BYTE   bNewsInfo;			// QS has news info db
		BYTE   name[16];			// QS Name
		BYTE   hour;				// time of last time message
		BYTE   minute;
		BYTE   month;				// date of last message
		BYTE   day;
		WORD   year;
		WORD   session;				// current session number
		DWORD  packets;				// total packets received
		DWORD  lostPackets;			// total lost packets 
		DWORD  recoveredPackets;	// total recovered packets
		DWORD  duplicatePackets;	// total duplicate packets
		DWORD  ipAddr;
		DWORD  portAddr;
		WORD   lastHistDay;
		DWORD  fBlockGLD   :1,		// QS can handle Block Go Live/Dead msgs
			   f24BidDP	   :1;		// QS can handle 24 bit datapointers
		WORD   resv;
} QS_INFO;

typedef	struct
		{
		WORD	command;
		DWORD	qsdtID;
		BYTE	data[32];
		}
		ADVISE_X;

// TOS structure.

#define TOSC_REGULAR		0
#define TOSC_CANCEL			1
#define TOSC_DELETE			2
#define TOSC_INSERT			3
#define	TOSC_REPLACE		4
#define TOSC_SOLD_SALE		6
#define TOSC_OPEN			7
#define TOSC_USER_DELETE	10
#define TOSC_NOT_LOADED		11
#define TOSC_MISC			13
#define TOSC_BID			14
#define TOSC_ASK			15

typedef	struct
		{
		WORD	min :6,				// Lower  6 bits = minute
				hour:5,				// Middle 5 bits = hour
				day :5;				// Upper  5 bits = day
		BYTE	exchdec; 			// Hi nibble = exch, Lo nibble = decimal
		BYTE	price[3];
		WORD	size:14,			// Lower 14 bits = size
				exp:2;				// Upper  2 bits = exponent
		}
		TOS, *LPTOS;


// New format of TOSLINK with count and RTOS
//     (Note: link is still in the same place

typedef	struct
		{
		WORD	rtos;			// Reverse TOS link sector (prev sector)
		BYTE	exchdec;		//  ** not used
		WORD	link;			// Forward TOS link sector (next sector)
		BYTE	zero;			//  ** not used
		WORD	nSectors;		// Total number of sectors in chain
		}
		TOSLINK, *LPTOSLINK;

// Retrieve the most current TOS information.

typedef struct
		{
		char	S;
		BYTE	dataType;
		WORD	sector;
		BYTE    day;			// 0 = today, 1 = yesterday
		BYTE    extra[3];		// set to zero for now
		DTDP    dtdp;			// data type/data pointer
		}
		QSM_MSRQ;

typedef struct
		{
		char	S;
		BYTE	dataType;
		WORD	endLink;		// Sector of "end" TOS block
		TOS		tos[32];		// Ver 6.06 NT QS's can		
		}						//	   more shards!
		QSM_MSRP;

typedef struct _QSM_HEADER
		{
		BYTE  Z;
		BYTE  type;
		}
		QSM_HEADER;

#define USERNAME_10		0
#define USERNAME_20		1
#define USERNAME_IS_20	2

#define CMP_NONE		0
#define CMP_BMSG		1
#define CMP_ALL			7

typedef struct _QSM_ZQ {
		BYTE  Z, Q;
		BYTE  access;
		BYTE  types[4];
		BYTE  updSocket[2];
		BYTE  ticSocket[2];
		BYTE  name[16];
		BYTE  contention;
		BYTE  nlogged;
		BYTE  nmax;
		BYTE  restType;
		BYTE  nmFlags;
		BYTE  cmpFlags;
		WORD  lastDay;			// Last Deep History Day
		BYTE  extra[13];
} QSM_ZQ;

typedef struct _QSM_ZP {
		BYTE  Z, P;
		BYTE  access;
		BYTE  types[4];
		BYTE  updPipe[40];
		BYTE  ticPipe[40];
		BYTE  name[16];
} QSM_ZP;

typedef struct _QSM_ZS {
		BYTE   Z, S;
		WORD   versionNum;
		BYTE   versionLet;
		BYTE   session;
		DWORD  msgCount;
		DWORD  errCount;
		BYTE   dataSource;
} QSM_ZS;

typedef struct _QSM_VIDEO {
		BYTE   Z, V;
		BYTE   line;
		BYTE   text[400];
} QSM_VIDEO;

// Zi msg: used to get QS Information & Capabilities

typedef struct _QSM_RQ_QSICAP
{
	BYTE	Z,i;			// Message type,subtype 'Zi'
	WORD	cmd;			// Command (0 = Basic info)
	WORD	wWSVersionNum;	// Workstation version number
	BYTE	bWSVersionLet;	// Not used.
}
QSM_RQ_QSICAP;

typedef struct
{
	BYTE	datatype;
	BYTE	nPeriod;		// Time period in minutes
	WORD	nDays;			// Number of days of data avail
}
TPBINFO;

typedef struct
{
	BOOL	bTOSTPB;
	WORD	nTpbInfo;
	TPBINFO	tpbInfo[100];
}
TPBINFOBLOCK;

// This is the structure of the data returned when using the
//      AtcGetQSInfoEx with dwInfoType set to 1.

typedef struct _QSEX_ADVISEINFO
{
	DWORD	dwMaxTotalAdvises;
	DWORD	dwCurrTotalAdvises;
	DWORD	dwHighTotalAdvises;
	DWORD	dwClientAdvises;
	BOOL	bLimitHit;
	union
	{
		WORD	wBchFlags;
		struct
		{
			WORD	bInhibitBunched : 1;
			WORD	bSendsBunched   : 1;
			WORD	bSendsAdvUpdates: 1;
			WORD	bInhibitBigMsgs : 1;
		};
	};
	DWORD	dwLostMsgs;
	DWORD	dwRecoveredMsgs;
	DWORD	dwGoodMsgs;
	DWORD	dwDatalossDelay;
}
QSEX_ADVISEINFO;

typedef struct _QSM_RS_QSICAP
{
	BYTE	Z,i;		   		// Message type,subtype 'Zi'
	WORD	status;		   		// Status: 0 = ok
	BYTE	QSType;		   		// QS Type
	BYTE	QSVersionLet;  		// QS Version Letter
	WORD	QSVersionNum;  		// QS Version Number
	WORD	wUpdSocket;	   		// Update socket/port
	WORD	wTicSocket;	   		// Tick socket/port
	char	szQSName[16];  		// QS Name (Null terminated)
	BYTE	nRestType;	   		// Restriction type
	BYTE	nSession;	   		// Session number (optional)
	WORD	wLastDay;	   		// Deep history last day
	DWORD	fUserIDMax20:1,		// User ID can be 20 char
			fUserIDIs20 :1,		// User ID's are 20 chars
			fCompression:1,		// Compresssion: iF msgs,etc.
			fBlockGLD   :1,		// QS does block GLD message
			fHistSearch :1,		// QS does history search
			fTOSTPB,	:1,		// QS does Create TOS/TPB
			nMMDBSize   :2,		// MarketMaker DB size. 00 = none,
								// 01 = split small, 10 = single big
			f24BitDP    :1,		// QS does 24 bit data pointers
			fNewHistory :1,		// 6.10 history format
			fOptionDatabase: 2,	// option database count 00 = 3 in 6.10 configuration
								// 01 = 4 (A-E, F-M, N-R, and S-Z), 10 = 1 large database
			fNewsInfo	:1,		// QS has news info
			fATOption	:2,		// AT Option DB type-0=Maybe,1=None,2=ATOPTNUS,3=ATOPTION
			fAlertMsg	:1,		// QS supports Alert Message
			fHalt: 1,			// QS supports Halts
			fBigRequests: 1;	// QS supports big requests
	DWORD   resv: 14;
	DWORD	dwBigBlkStockSze;	// Big block stock threshold
	DWORD	dwBigBlkStkOptSze;	// Big block stock option threshold
	WORD	wWSVersionNum;		// Latest workstation version number
	BYTE    compVersion;		// compression version
	BYTE	reserved;			// Unused
	DWORD   groupAddress;		// IP Multicast Group Address
	DWORD	dwExtra[6];			// Unused
	WORD	nTpbInfo;			// Number of TPB Info pairs 
	TPBINFO	tpbInfo[1];			// Variable length array
}
QSM_RS_QSICAP;

// login SPX packets

typedef struct _REQ_LOGIN {
		BYTE  L, I;
		WORD  option;
		BYTE  id[20];
} REQ_LOGIN;

typedef struct _RSP_LOGIN {
		BYTE  L, I;
		WORD  status;
		BYTE  data[256];
} RSP_LOGIN;

typedef struct _REQ_LOGOUT {
		BYTE  L, O;
		WORD  option;
} REQ_LOGOUT;

typedef struct _RSP_LOGOUT {
		BYTE  L, O;
		WORD  status;
} RSP_LOGOUT;

typedef struct _REQ_LOGSTAT {
		BYTE  L, S;
		WORD  option;
} REQ_LOGSTAT;

typedef struct _RSP_LOGSTAT {
		BYTE   L, S;
		WORD   status;
		BYTE   userID[10];
		BYTE   data[32];
		WORD   time;
		BYTE   userIDExp[10];
		DWORD  maxAdvises;
		WORD   rpsFriend;
		WORD   rpsUnknown;
} RSP_LOGSTAT;

typedef struct _REQ_LOGPERM {
		BYTE  L, P;
		WORD  option;
		BYTE  userID[20];
} REQ_LOGPERM;

typedef struct _RSP_LOGPERM {
		BYTE  L, P;
		WORD  status;
		BYTE  data[32];
} RSP_LOGPERM;

typedef struct _REQ_LOGCON {
		BYTE  L, C;
		WORD  option;
		BYTE  userID[20];
} REQ_LOGCON;

typedef struct _RSP_LOGCON {
		BYTE  L, C;
		WORD  status;
		BYTE  bContention;
		BYTE  bBackup;
		BYTE  nMax;
		BYTE  nLoggedIn;
		BYTE  bAlready;
} RSP_LOGCON;

typedef	struct _REQ_LOGEXT {
		BYTE	L,e;			// Extended Log Permission command 'Le'
		BYTE	partNum;		// Desired chunk (0/1 start @ 0 or 512)
		BYTE	compression;	// Compression mode desired
} REQ_LOGEXT;

typedef	struct _RSP_LOGEXT {
		BYTE	L,e;			// Extended Log Permission command 'Le'
		SHORT	length;			// Return length -or- error code (in data)
		BYTE	data[512];		// Return data
} RSP_LOGEXT;					// Errors: 1=user not log in, 2 = no ext perm

/*
**** Special types and structures used in ATFIDISK
*/

typedef struct {								// create a file
		BYTE  Q;
		BYTE  M;
		WORD  attribute;						// file attribute
		BYTE  szFilename[64];
} QSCREATES;

typedef	struct {
		char	Q;
		char	M;
		WORD	status;
		WORD	fh;
} QSCREATER;

typedef struct {								// request to a delete file
		BYTE	 Q;
		BYTE	 K;
		char	szFilename[64];
} QSKILLQ;

typedef struct {								// response to a kill file
		BYTE	 Q;
		BYTE	 K;
		SHORT	 status;
} QSKILLR;

typedef struct {
		char R;  								// Message Type = 'R'
		BYTE datatype;							// (Symbol[0]<'M') ? DT_SP_LEVEL2_A : DT_SP_LEVEL2_N
		char Symbol[5];							// Symbol 
		char MMid[4];							// MMid
		char space;								// = ' '
		char Exchange;							// 'Q'=nasdaq, 'S'=small cap
} SPQB_REQUEST;

typedef struct {
		char R;  								// Message Type = 'R'
		BYTE datatype;							// (Symbol[0]<'M') ? DT_SP_LEVEL2_A : DT_SP_LEVEL2_N
		SPQB data;
} SPQB_RESPONSE;

typedef	struct					// Message "QP", gimme your Alt Path Info
		{
		BYTE	Q,P;
		char	cAltPath;		// AltPath: '0'...'9'
		}
		QSM_REQ_ALTPATH, *LPQSM_REQ_ALTPATH;
		
typedef	struct
		{
		BYTE	Q,P;
		WORD	bPathWriteable;	// True if path is writeable
		char	szPath[128];	// Alt path
		}
		QSM_RSP_ALTPATH, *LPQSM_RSP_ALTPATH;

typedef struct
		{
		BYTE    r;
		BYTE    dataType;
		BYTE    symbol[8];
		BYTE    country[2];
		BYTE    exchange;
		}
		QSM_REQ_BLOCK, *LPQSM_REQ_BLOCK;

/*
**** Structure of Btrieve messages sent TO the QS
*/

#define BTRTYPE_MASK			0xff00
#define BTRTYPE_FUNDAMENTAL		0x0500
#define BTRTYPE_SYMBOL			0x0600

typedef	struct
		{
		BYTE	Q,V;			// Message type and sub-type
		WORD	opCode;			// Operation code (BTRIEVE op code)
		WORD	handle;			// handle (OPEN_BTV index)
		WORD	keyPath;		// Key path
		char	key[30];		// Key or other data
		}
		QSM_QVS, *LPQSM_QVS;

typedef	struct					// Used by MAINTAINENCE FUNCTIONS
		{
		BYTE	Q,V;			// Message type and sub-type
		WORD	opCode;			// Operation code (BTRIEVE op code)
		WORD	handle;			// handle (OPEN_BTV index)
		WORD	keyPath;		// Key path
		WORD	datalen;		// Length of key
		char	data[498];		// Data returned
		}
		QSM_QVMS, *LPQSM_QVMS;

/*
**** Structure of Btrieve messages returned FROM the QS
*/

typedef	struct
		{
		BYTE	Q,V;			// Message type and sub-type
		WORD	status;			// Status of function performed
		char	key[26];		// Returned key
		WORD	dataLen;		// Returned data length
		char	data[498];		// Data returned
		}
		QSM_QVR, *LPQSM_QVR;

// new symbol guide messages

// 	Op-codes for QSM_RQSG.

#define	LIST_FROM_KEY_FORWARD		0
#define	LIST_FROM_POS_FORWARD		1
#define	LIST_FROM_KEY_BACKWARD		2
#define	LIST_FROM_POS_BACKWARD		3
#define	GET_RECORD_FROM_POS			4

#define SGERR_SUCCESS				0
#define SGERR_INVALID_OPCODE		1
#define SGERR_INVALID_KEYPATH		2
#define SGERR_INVALID_FILENUM		3
#define SGERR_FILE_OPEN				4

typedef	struct
		{
		BYTE		Q,v;					// Message type and sub-type.
		BYTE		btvNum;					// Btrieve file number.
		BYTE		keyPath;				// Btrieve key path.
		BYTE		opCode;					// Operation.
		BYTE		keyPosition[42];		// Key or DWORD position.
		}
		QSM_RQSG, *LPQSM_RQSG;

//	Response message.

typedef	struct
		{
		BYTE		Q,v;					// Message type and sub-type.
		WORD		count;					// Number of PLCT''s or -1.
		char		data[530];				// Packed data.
		}
		QSM_RSSG, *LPQSM_RSSG;

typedef struct 
		{
		DWORD  dwPosition;
		char   data[1];						// ticker 1..7 + NULL, Long name 0..25 + NULL, 
											// CUSIP 0..11 + NULL
		}
		PLCT, *LPPLCT;

// *New* History search messages

#define	YF_CURYR_STOCKS		100
#define	YF_CURYR_FUTURES	101
#define	YF_CURYR_NYSE_HST	102
#define	YF_CURYR_AMEX_HST	103
#define	YF_CURYR_NMS_HST	104
#define	YF_FUND_HST			105
#define	YF_MONTHLY_HST		110
#define	YF_WEEKLY1_HST		111		// First  4K
#define	YF_WEEKLY2_HST		112		// Second 4K
#define	YF_WEEKLY3_HST		113		// Third  4K

typedef	struct
		{
		char	s;				// Search
		char	H;				//  History message
		BYTE	datatype;
		BYTE	yrFlag;			// 0-99:NYSE.95.. 100:history.xxx,101=fut
		char	sym[8];
		char	country[2];
		BYTE	exchange;
		BYTE	extra;
		}
		QSM_HSRCHRQ, *LPQSM_HSRCHRQ;

#define HSR_OK				0
#define HSR_NO_FILE			1
#define HSR_NOT_FOUND		2

typedef	struct
		{
		char	s;				// Search
		char	H;				//  History message
		BYTE	datatype;
		BYTE	status;			// 0=ok,1=no such file,2=not found
		DWORD	dwOffset;		// Offset of data
		char	szFN[128];		// Null terminated filename
		}
		QSM_HSRCHRS, *LPQSM_HSRCHRS;

#define HCMPTYP_RAW1024		0
#define HCMPTYP_FIXED1024	1
#define HCMPTYP_RAW512		100
#define HCMPTYP_FIXED512	101

typedef struct
		{
		char    s,R;            // Compressed read message
		BYTE    datatype;       // Datatype not used
		BYTE    kindFlag;       // Kind of compression expected
		DWORD   dwOffset;       // Offset in to file
		char    szFN[64];       // Filename
		}
		QSM_CMPHISTRQ, *LPQSM_CMPHISTRQ;

typedef struct
		{
		char    s,R;            // Compressed read message
		BYTE    datatype;       // Datatype not used
		BYTE    status;         // Flag
		DWORD   dwOffset;       // Offset in to file
		BYTE    data[526];      // Filename or data WADE CHANGED FROM 530
		}
		QSM_CMPHISTRS, *LPQSM_CMPHISTRS;

typedef struct
		{
		short	lowOffset;
		BYTE    highOffset;
		BYTE    lastOffset;
		WORD    volValue: 14,
				volMult : 2;
		}
		COMP6, *LPCOMP6;

typedef struct
		{
		short	lowOffset;
		WORD    highOffset;
		WORD    lastOffset;
		WORD    volValue: 14,
				volMult : 2;
		}
		COMP8, *LPCOMP8;

/*------------- TOS Compression Messages --------------*/

#define QTFLGS_TODAY		0x00	// today
#define QTFLGS_FILEHANDLE	0x01	// file handle present
#define QTFLGS_FORWARD		0x00	// return the forward sector link
#define QTFLGS_BACKWARD		0x02	// return the backward sector link
#define QTFLGS_DATATYPE		0x04	// data type and day present

typedef	struct
		{
		BYTE	Q,t;				// Message and subtype
		BYTE	flags;				// Flag bits, described below
		WORD	wSector;			// Start Sector to Read and Compress
		WORD	fh;					// Optional File handle
		BYTE    dataType;			// data type... needed by NT Servers
		BYTE    day;				// Days back - NT Servers: 1 = Yesterday, 2 = two days ago, etc...
		DTDP    dtdp;				// data type/data pointer
		}
		QSM_RQQT, *LPQSM_RQQT;

// Flags: Bit 0:  0 = Use today''s TIC.DAT file
//				  1 = Use File handle in fh
//		  Bit 1:  0 = Return forward  sector link
//				  1 = Return backward sector link
//		  Bit 2:  0 = no data type present
//				  1 = data type present

typedef	struct
		{
		BYTE	nRecs;				// Number of records returned (0..63)
		WORD	wLink;				// Next/Prev sector #
		WORD	nCount;				// # of sectors "before" this one
		BYTE	rFirst[8];			// First TOS recor. (Not compressed)
		BYTE	btStream[517];		// Bit stream, compressed data
		}
		CMPTOS, *LPCMPTOS;

typedef	struct
		{
		BYTE	Q,t;				// Message and subtype
		BYTE	nSectors;			// Number of cmpTos''s returned
		BYTE	cmpTos[530];		// One or more compressed TOS
		}
		QSM_RSQT, *LPQSM_RSQT;

// Note: If nSectors == -1 and error has occured.
//       first byte of cmpTos contains reason
//		 0 = TIC.DAT file is not open
//		 1 = Seek/Read error

/*----------------------------------------------------------*/
//
// New Search&Read Previous Day TOS Pointers and Cache data
//

typedef struct		// Request to QS
	{
	BYTE	s,T;			// Search and Read Previous Day TOS
	BYTE	datatype;		// datatype of symbol to search for
	char	cExt;			// File Extension ('1' = Monday, etc..)
							// Days back - NT Servers: 1 = Yesterday, 2 = two days ago, etc...
	char	szSym[12];		// Symbol to search for
	DTDP    dtdp;			// data type, data pointer
	}
	QSM_PTOSRQ, *LPQSM_PTOSRQ;

typedef struct		// Reply from QS
	{
	BYTE	s,T;			// Search and Read Previous Day TOS
	BYTE	datatype;		// datatype of symbol to search for
	BYTE	flag;			// ?
	WORD	tosPtr;			// Forward TOS pointer
	WORD	rtosPtr;		// Reverse TOS pointer
	BYTE	data[ 128 ];	// Prev day cached TOS data
	}
	QSM_PTOSRS, *LPQSM_PTOSRS;

/*----------------------------------------------------------*/

// TOS/OMB Filtered Read

#define TOSFR_SIZE				0x0001				// filter by size
#define TOSFR_EXCHANGE			0x0002				// filter by exchange
#define TOSFR_TIME				0x0004				// filter by time
#define TOSFR_XTRADES			0x0008				// exclude trades
#define TOSFR_XBIDS				0x0010				// exclude bids
#define TOSFR_XASKS				0x0020				// exclude asks
#define TOSFR_XSPECIAL			0x0040				// special rcds
#define TOSFR_BARS				0x4000				// make time period bars
#define TOSFR_TEST				0x8000				// message test

typedef	struct
		{
		char	Q,f;			// Qf Message
		WORD	flag;			// See below
		BYTE	datatype;		// Datatype
		BYTE	day;			// Day (0=today,
								//      DOS:1=Monday,etc.
								//		NT :1=yesterday
		WORD	endTosPtr;		// TOS Pointer
		DWORD	dwSize;			// Min size
		WORD	wTimeStart;		// Start time
		WORD	wTimeEnd;		// End time
		BYTE	exch;			// Exchange
		BYTE	period;			// Time period in minutes
		WORD	nCtos;			// Number of "Cached" TOS records (shards)
		TOS		cTos[16];		// Cached TOS records (shards)
		DTDP    dtdp;			// data type/data pointer
		}
		QSM_RQ_TOSFR;

typedef	struct
		{
		char	Q,f;			// Qf Message
		DWORD	nRecsStatus;	// Number of records or status (see table)
								// HiByte is highest dec type
		DWORD	nFileSize;		// FileSize
		char	szFN[12];		// Output filename
		WORD	nDataSize;		// Number of bytes in byData[]
		WORD	wTradeEnd;		// Latest time in file
		WORD	wTradeStart;	// Earliest time in file
		BYTE	byData[504];	// Upto first 504 bytes after header
		}
		QSM_RS_TOSFR;

#define PACKEDSIZE_MAX		0x3fff*1000

typedef	struct
		{
		WORD	size:14,		// Lower 14 bits = size
				exp : 2;		// Upper  2 bits = exponent
		}
		TOSSIZE;

typedef	union
		{
		WORD	value;
		TOSSIZE	tosSize;
		}
		PK_SIZE;

typedef	struct
		{
		WORD	min: 6,			// minute
				hour : 5,		// hour
			    day: 5;			// day
		}
		TOSTIME;

#define CTPB_SHORT_BASE		0x00	// short base price (low)
#define CTPB_LONG_BASE		0x10	// long base price (low)
#define CTPB_SHORT_OFFSETS	0x00	// short offsets
#define CTPB_LONG_OFFSETS	0x20	// long offsets
#define CTPB_PACKED_VOLUME	0x00	// packed volume
#define CTPB_FULL_VOLUME	0x40	// full volume

typedef struct
		{
		TOSTIME	tosTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		}
		CTPB_HEADER;

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		WORD    low;	    // Low price for the time period
		BYTE    high;	    // High  as positive offset from low
		BYTE    open;	    // Open  as positive offset from low
		BYTE    close;	    // Close as positive offset from low
		TOSSIZE	pkVol;		// Volume for the time period (less than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB0;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		BYTE    low[3];	    // Low price for the time period
		BYTE    high;	    // High  as positive offset from low
		BYTE    open;	    // Open  as positive offset from low
		BYTE    close;	    // Close as positive offset from low
		TOSSIZE	pkVol;		// Volume for the time period (less than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB1;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		WORD    low;	    // Low price for the time period
		WORD    high;	    // High  as positive offset from low
		WORD    open;	    // Open  as positive offset from low
		WORD    close;	    // Close as positive offset from low
		TOSSIZE	pkVol;		// Volume for the time period (less than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB2;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		BYTE    low[3];	    // Low price for the time period
		WORD    high;	    // High  as positive offset from low
		WORD    open;	    // Open  as positive offset from low
		WORD    close;	    // Close as positive offset from low
		TOSSIZE	pkVol;		// Volume for the time period (less than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB3;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		WORD    low;	    // Low price for the time period
		BYTE    high;	    // High  as positive offset from low
		BYTE    open;	    // Open  as positive offset from low
		BYTE    close;	    // Close as positive offset from low
		DWORD	volume;		// Volume for the time period (more than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB4;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		BYTE    low[3];	    // Low price for the time period
		BYTE    high;	    // High  as positive offset from low
		BYTE    open;	    // Open  as positive offset from low
		BYTE    close;	    // Close as positive offset from low
		DWORD	volume;		// Volume for the time period (more than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB5;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		WORD    low;	    // Low price for the time period
		WORD    high;	    // High  as positive offset from low
		WORD    open;	    // Open  as positive offset from low
		WORD    close;	    // Close as positive offset from low
		DWORD	volume;		// Volume for the time period (more than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB6;			    //        volume this is the tic count.

typedef struct
		{
		WORD	wTime;
		BYTE	compDec;	// Upper nibble=compress type,lower=decimal type
		BYTE    low[3];	    // Low price for the time period
		WORD    high;	    // High  as positive offset from low
		WORD    open;	    // Open  as positive offset from low
		WORD    close;	    // Close as positive offset from low
		DWORD	volume;		// Volume for the time period (more than 16MM)
		}				    //  Note: if records (INDU,etc) have ZERO
		CTPB7;			    //        volume this is the tic count.

typedef	union
		{
		CTPB_HEADER  header;
		CTPB0	     t0;
		CTPB1	     t1;
		CTPB2	     t2;
		CTPB3	     t3;
		CTPB4	     t4;
		CTPB5	     t5;
		CTPB6	     t6;
		CTPB7	     t7;
		}
		CTPB, *PCTPB, *LPCTPB;

typedef	struct
		{
		char	Q,e;			// Compress Time Period Bar Read message
		WORD	options;
		char	szFN[16];
		DWORD	dwOffset;
		WORD	nRecs;			// Number of records wanted
		}
		QSM_RQ_CTREAD;

typedef	struct
		{
		char	Q,e;			// Compress Time Period Bar Read message
		WORD	nDataSize;		// Number of bytes returned (MSB=QS Deleted file)
		BYTE	byData[530];	// Compressed data up to 53 records
		}
		QSM_RS_CTREAD;

/*------------- ATOPTNUS Compression Messages --------------*/

typedef	struct
		{
		BYTE	Q,A;				// Message and subtype
		char	sKey[6];			// Underlying Symbol
		}
		QSM_RQQA, *LPQSM_RQQA;

typedef	struct
		{
		BYTE	Q,A;				// Message and subtype
		WORD	nBytes;				// Number of bytes (see below)
		BYTE	data[530];			// Compressed data stream (or else)
		}
		QSM_RSQA, *LPQSM_RSQA;

typedef	struct
		{
		WORD	nTotalBytes;		// Length of whole stream
		WORD	nReturnedBytes;		// Length of partial stream returned
		char	szFN[16];			// Name of temp file with whole stream
		BYTE	data[510];			// First portion of stream
		}
		QA_ESP, *LPQA_ESP;

// Note: nBytes = -1 and length of returned message = 5 means error.
//		 First byte of data[] details the error:
//				0 = Symbol not found
//				1 = error opening/reading btrieve file.
//
//		 nBytes = 0 and length of returned message > 5 means the
//		 data is in QA_ESP format.

typedef struct _QSM_DBREQ {
	BYTE   type;
	BYTE   func;
	BYTE   dataType;
	BYTE   status;
	BYTE   key[12];
	DWORD  dataPtr;
	WORD   recordLen;
	BYTE   record[1];
} QSM_DBREQ, *LPQSM_DBREQ;

typedef struct _QSM_DBRSP {
	BYTE   type;
	BYTE   func;
	BYTE   dataType;
	BYTE   status;
	BYTE   key[12];
	DWORD  dataPtr;
	WORD   recordLen;
	BYTE   record[1];
} QSM_DBRSP, *LPQSM_DBRSP;

#define RANKGMC_NYSE			14
#define RANKGMC_AMEX			1
#define RANKGMC_NASD			19

#define RANKST_ACTIVE			0
#define RANKST_ADVANCERS		1
#define RANKST_DECLINERS		2
#define RANKST_ADVANCERS_PCT	3
#define RANKST_DECLINERS_PCT	4

typedef struct _QSM_RANKREQ {
	BYTE   type;
	BYTE   gmc;
	BYTE   subType;
} QSM_RANKREQ, *LPQSM_RANKREQ;

typedef struct _QSM_RANKRSP {
	BYTE  type;
	BYTE  gmc;
	BYTE  subType;
	BYTE  num;
	BYTE  symbols[15*8];
} QSM_RANKRSP, *LPQSM_RANKRSP;

#define FCALLFLG_CONTINUE		0x01

typedef struct _QSM_FCALLSYMREQ {
	BYTE  R;
	BYTE  dataType;
	BYTE  s;
	BYTE  flags;
	BYTE  symbol[9];
	BYTE  broker[2];
	BYTE  exchange;
} QSM_FCALLSYMREQ, *LPQSM_FCALLSYMREQ;

typedef struct _FCALL_SYM {
	BYTE   year[2];
	BYTE   broker[2];
	DWORD  idPtr;
} FCALL_SYM, *LPFCALL_SYM;

typedef struct _QSM_FCALLSYMRSP {
	BYTE       R;
	BYTE       dataType;
	BYTE       s;
	BYTE       flags;
	BYTE       symbol[9];
	BYTE       broker[2];
	BYTE       exchange;
	BYTE       num;
	BYTE       resv;
	WORD       total;
	FCALL_SYM  symBuf[60];
} QSM_FCALLSYMRSP, *LPQSM_FCALLSYMRSP;

typedef struct _QSM_FCALLBLOCKREQ {
	BYTE   R;
	BYTE   dataType;
	BYTE   b;
	BYTE   num;
	BYTE   flags;
	BYTE   resv[3];
	DWORD  idPtr[6];
} QSM_FCALLBLOCKREQ, *LPQSM_FCALLBLOCKREQ;

typedef union _FCALL_BLOCK {
	FCEARN_PB  earn;
	FCCONS_PB  cons;
} FCALL_BLOCK, *LPFCALL_BLOCK;

typedef struct _QSM_FCALLBLOCKRSP {
	BYTE         R;
	BYTE         dataType;
	BYTE         b;
	BYTE         num;
	BYTE         flags;
	BYTE         resv[3];
	FCALL_BLOCK  blocks[6];
} QSM_FCALLBLOCKRSP, *LPQSM_FCALLBLOCKRSP;

// Messages supported by QS Versions 5.01 and above

typedef	struct	_QSM_KRPAGE_RQ	{
	BYTE		K,p;			// Message
	WORD		pageNum;		// Page number 
	BYTE		part;			// Part 0..6
	BYTE		compression;	// Compression mode
} QSM_KRPAGE_RQ;

typedef	struct	_QSM_KRPAGE_RS	{
	BYTE		K,p;			// Message
	WORD		length;			// Number of bytes ya got
	BYTE		data[512];		// Page data
} QSM_KRPAGE_RS;

typedef	struct	_QSM_KNP	{
	BYTE		K;				// Message
	BYTE		np;				// '+' means Next, '-' means previous
	WORD		pageNum;		// Page number 
} QSM_KNP;

//	Disclosure messages.
#if defined(_DISCLOSE_H)

typedef struct _DISC_KEYSUM {
	WORD	recordType:5,		// disclosure record type (0-31)
			periodCode:3,		// period code (0 = NA, 1 = 1Q, 2 = 2Q, 3 = 3Q, 4 = 4Q, 5 = Annual )
			year:7,				// year - NOTE: Offset from 1970 (0-127)
			resv:1;				// reserved for future use
} DISC_KEYSUM, *LPDISC_KEYSUM;

typedef struct _QSM_DISC_KEYSUM_REQ {
	BYTE		d, k;							// Message and subtype 'd(isclosure)' 'k(ey)' summary
	BYTE        resv;							// reserved field for packing
	BYTE		nKeyType;						// disclosure key type that follows (defined in disclose.h)
	DISC_KEYS	discKey;						// disclosure key
} QSM_DISC_KEYSUM_REQ, *LPQSM_DISC_KEYSUM_REQ;

#define MAX_DISC_KEY_SUMS 200
typedef struct _QSM_DISC_KEYSUM_RSP {
	BYTE				d, k;							// Message and subtype 'd(isclosure)' 'k(ey)' summary
	BYTE				nStatus;						// status of the request
	BYTE				nCount;							// count of how many key summarys came back
	DISCLOSURE_HEADER	discHeader;						// disclosure header
	DISC_KEYSUM			discKeySum[MAX_DISC_KEY_SUMS];	// array of key summary structures
} QSM_DISC_KEYSUM_RSP, *LPQSM_DISC_KEYSUM_RSP;

#define MAX_DISC_SYM_SUMS 10
typedef union _DISC_SYMSUM {
	CHAR sData[1];								 // generic access to the symbol summary response
	CHAR sDCNSums[MAX_DISC_SYM_SUMS][10];		 // array of DCN summary strings
	CHAR sTickerSums[MAX_DISC_SYM_SUMS][6];		 // array of Ticker summary strings
	CHAR sCUSIPSums[MAX_DISC_SYM_SUMS][10];		 // array of CUSIP summary strings
	CHAR sCompanyNameSums[MAX_DISC_SYM_SUMS][40];// array of Company Name summary strings
} DISC_SYMSUM, *LPDISC_SYMSUM;

typedef struct _QSM_DISC_SYMSUM_REQ {
	BYTE		d, s;							// Message and subtype 'd(isclosure)' 's(ymbol)' summary
	BYTE        nSearchDirection;				// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
	BYTE		nKeyType;						// disclosure key type that follows (defined in disclose.h)
	DISC_KEYS	discKey;						// disclosure key
} QSM_DISC_SYMSUM_REQ, *LPQSM_DISC_SYMSUM_REQ;

typedef struct _QSM_DISC_SYMSUM_RSP {
	BYTE		d, s;								// Message and subtype 'd(isclosure)' 's(ymbol)' summary
	BYTE		nStatus;							// status of the request
	BYTE		nCount;								// count of how many key summarys came back
	DISC_SYMSUM discSymSum;							// union of summary messages
} QSM_DISC_SYMSUM_RSP, *LPQSM_DISC_SYMSUM_RSP;

typedef struct _QSM_DISC_REQ {
	BYTE		d, r;							// Message and subtype 'd(isclosure)' 'r(equest)'
	WORD		wBlockNum;						// Block number ( 0 - 124 ) * 512 = 64k
	BYTE        nFunction;						// function number (look in disclose.h)
	BYTE		nKeyType;						// disclosure key type that follows (defined in disclose.h)
	BYTE		resv[2];						// reserved
	DISC_KEYS	discKey;						// disclosure key
} QSM_DISC_REQ, *LPQSM_DISC_REQ;

typedef struct _QSM_DISC_RSP {
	BYTE		d, r;							// Message and subtype 'd(isclosure)' 'r(equest)'
	BYTE		nStatus;						// status of the request	
	BYTE		resv;							// reserved	
	DWORD		dwPosition;						// position in btrieve file	(can be used as key for fast access)
	WORD		wDataLen;						// length of the "data" -- up to 512
	BYTE		resv1[2];						// reserved
	CHAR		data[MAX_DISC_RSP_DATA];		// max data is 512 bytes per block
} QSM_DISC_RSP, *LPQSM_DISC_RSP;

#endif

#if defined(_VICKERS_H)

typedef struct _QSM_VICK_REQ {					// Request for Institutional Holdings Data
	BYTE		v, r;							// Message and subtype 'v(ickers)' 'r(equest)'
	BYTE		nSubCount;						// number of subs requested (up to 5) (offset 1)
	BYTE		nKeyType;						// vickers key type that follows (defined in vickers.h)
	WORD		nWantTotal:1,					// True if you want server to fill in total number of subs in the vickrecord
				nSearchDir:1,					// FALSE is descending, TRUE is ascending
				nWantChildren:1,				// true if children are to be counted and returned
				nList:1,						// true if for list
				resv:12;
	VICK_KEYS	vickKey;						// vickers key
	HWND		hWnd;
} QSM_VICK_REQ, *LPQSM_VICK_REQ;

typedef struct _QSM_VICK_RSP {					// Response for Institutional Holdings Data
	BYTE		v, r;							// Message and subtype 'v(ickers)' 'r(equest)'
	WORD		nStatus:5,						// status of the request (0-31)
				nRecordType:3,					// type of record returned (0-7)
				nSubCount:4,					// subcount in array in vickRecord (0-5)
				resv:4;							// reserved
	VICK_RECORD	vickRecord;						// vickers record ( vickers.h )
} QSM_VICK_RSP, *LPQSM_VICK_RSP;

typedef struct _QSM_VICK_SUM_REQ {				// Request for Institutional Holdings Data  Summary
	BYTE		v, s;							// Message and subtype 'v(ickers)' 's(ummary)'
	BYTE        nSearchDirection;				// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
	BYTE		nKeyType;						// vickers key type that follows (defined in vickers.h)
	VICK_KEYS	vickKey;						// vickers key
} QSM_VICK_SUM_REQ, *LPQSM_VICK_SUM_REQ;

typedef struct _QSM_VICK_SUM_RSP {				// Response for Institutional Holdings Data Summary
	BYTE				v, s;					// Message and subtype 'v(ickers)' 's(ummary)'
	BYTE				nStatus;				// status of the request
	BYTE				nSubCount;				// subcount in array in vick summary Record
	VICK_SUMMARY_RECORD	vickSummaryRecord;		// vickers summary record ( vickers.h )
} QSM_VICK_SUM_RSP, *LPQSM_VICK_SUM_RSP;

typedef struct _QSM_VICK_INSIDER_REQ {			// Request for Form 4 & 5 Insider Holdings Data
	BYTE				v, i;					// Message and subtype 'v(ickers)' 'i(nsider)'
	WORD				nSubCount:3,			// number of subs requested (up to 4) (offset 1)
						nWantTotal:1,			// True if you want server to fill in total number of subs in the vickrecord
						resv:12;
	VK_INSIDER_HOLDINGS	vkInsiderHoldings;		// key
} QSM_VICK_INSIDER_REQ, *LPQSM_VICK_INSIDER_REQ;

typedef struct _QSM_VICK_INSIDER_RSP {			// Response for Form 4 & 5 Insider Holdings Data
	BYTE					v, i;				// Message and subtype 'v(ickers)' 'i(nsider)'
	WORD					nStatus:5,			// status of the request (0-31)
							nSubCount:4,		// subcount in array in vickRecord (0-4)
							resv:7;				// reserved
	VICK_INSIDERS_RECORD	vickInsidersRecord;	// vickers insiders record ( vickers.h )
} QSM_VICK_INSIDER_RSP, *LPQSM_VICK_INSIDER_RSP;

typedef struct _QSM_VICK_INSIDER_SUM_REQ {		// Request for Institutional Holdings Insider Data Summary
	BYTE		v, j;							// Message and subtype
	BYTE        nSearchDirection;				// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
	BYTE		nKeyType;						// vickers key type that follows (defined in vickers.h)
	VICK_KEYS	vickKey;						// vickers key
} QSM_VICK_INSIDER_SUM_REQ, *LPQSM_VICK_INSIDER_SUM_REQ;

typedef struct _QSM_VICK_INSIDER_SUM_RSP {		// Response for Institutional Holdings Insider Data Summary
	BYTE				v, j;					// Message and subtype
	BYTE				nStatus;				// status of the request
	BYTE				nSubCount;				// subcount in array in vick summary Record
	VICK_SUMMARY_RECORD	vickSummaryRecord;		// vickers summary record ( vickers.h )
} QSM_VICK_INSIDER_SUM_RSP, *LPQSM_VICK_INSIDER_SUM_RSP;

typedef struct _QSM_VICK_FIVEPERCENT_REQ {			// Request for Form 13D & 14-1D 5% Holdings Data
	BYTE				v, f;						// Message and subtype 'v(ickers)' 'f(ivepercent)'
	WORD				nSubCount:3,				// number of subs requested (up to 4) (offset 1)
						nWantTotal:1,				// True if you want server to fill in total number of subs in the vickrecord
						resv:12;
	VK_FIVEPERCENT_HOLDINGS	vkFivePercentHoldings;	// key
} QSM_VICK_FIVEPERCENT_REQ, *LPQSM_VICK_FIVEPERCENT_REQ;

typedef struct _QSM_VICK_FIVEPERCENT_RSP {			// Response for Form 13D & 14-1D 5% Holdings Data
	BYTE					v, f;					// Message and subtype 'v(ickers)' 'f(ivepercent)'
	WORD					nStatus:5,				// status of the request (0-31)
							nSubCount:4,			// subcount in array in vickRecord (0-4)
							resv:7;					// reserved
	VICK_FIVEPERCENT_RECORD	vickFivePercentRecord;	// vickers fivepercent record ( vickers.h )
} QSM_VICK_FIVEPERCENT_RSP, *LPQSM_VICK_FIVEPERCENT_RSP;


typedef struct _QSM_VICK_FIVEPERCENT_SUM_REQ {		// Request for Institutional Holdings Fivepercent Data Summary
	BYTE		v, k;								// Message and subtype
	BYTE		nSearchDirection;					// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
	BYTE		nKeyType;							// vickers key type that follows (defined in vickers.h)
	VICK_KEYS	vickKey;							// vickers key
}QSM_VICK_FIVEPERCENT_SUM_REQ, *LPQSM_VICK_FIVEPERCENT_SUM_REQ;

typedef struct _QSM_VICK_FIVEPERCENT_SUM_RSP {		// Response for Institutional Holdings Fivepercent Data Summary
	BYTE				v, k;						// Message and subtype
	BYTE				nStatus;					// status of the request
	BYTE				nSubCount;					// subcount in array in vick summary Record
	VICK_SUMMARY_RECORD vickSummaryRecord;			// vickers summary record ( vickers.h )	
} QSM_VICK_FIVEPERCENT_SUM_RSP, *LPQSM_VICK_FIVEPERCENT_SUM_RSP;

typedef struct _QSM_VICK_FIVEMORE_REQ {
	BYTE				v, m;						// Message and subtype 'v(ickers)' 'm(ore_fivepercent)'
	VK_FIVEMORE_HOLDINGS	vkFiveMoreHoldings;	// key
} QSM_VICK_FIVEMORE_REQ, *LPQSM_VICKFIVE_MORE_REQ;

typedef struct _QSM_VICK_FIVEMORE_RSP {			// Response for Form 13D & 14-1D 5% Holdings Data
	BYTE					v, m;					// Message and subtype 'v(ickers)' 'm(ore_fivepercent)'
	VICK_FIVEMORE_RECORD	vickFiveMoreRecord;	// vickers fivepercent more record ( vickers.h )
} QSM_VICK_FIVEMORE_RSP, *LPQSM_VICK_FIVEMORE_RSP;

typedef struct _QSM_VICK_INSTNUM_SEND {
	BYTE	v, n;
	HWND	hWnd;
	BYTE	byTot;
	DWORD	dwInstNum[130];
} QSM_VICK_INSTNUM_SEND, *LPQSM_VICK_INSTNUM_SEND;

typedef struct _QSM_VICK_INSTNUM_RSP {
	BYTE	v, n;
	BYTE	nStatus;
} QSM_VICK_INSTNUM_RSP, *LPQSM_VICK_INSTNUM_RSP;

#endif

#if defined(_ZACKS_H)

//Zack message
 typedef struct _QSM_ZACK_SUM_REQ {					// Request for Zacks Data Summary
	BYTE		z, s;								// Message and subtype
	BYTE		nSearchDirection;					// 0 = forward/greater-than or equal, 1 = backward/less-than or equal
	BYTE		nKeyType;							// zacks key type that follows (defined in zacks.h)
	ZACK_KEYS	zackKey;							// zacks key
}QSM_ZACK_SUM_REQ, *LPQSM_ZACK_SUM_REQ;

typedef struct _QSM_ZACK_SUM_RSP {					// Response for Zacks Data Summary
	BYTE				z, s;						// Message and subtype
	BYTE				nStatus;					// status of the request
	BYTE				nSubCount;					// subcount in array in zacks summary Record
	ZACK_SUMMARY_RECORD zackSummaryRecord;			// zacks summary record ( zacks.h )	
} QSM_ZACK_SUM_RSP, *LPQSM_ZACK_SUM_RSP;

typedef struct _QSM_ZACK_DTL_REQ {					// Request for zacks Data
	BYTE		z, d;								// Message and subtype 'z(acks)' 'd(etail)'					
	BYTE		nKeyType;							// zacks key type that follows (defined in zacks.h)
	BYTE		nTier:1,							// 1 = TierOne, 2 = TierTwo 
				nSearchDir:1,						// for future consideration
				resv:6;								//reserved
	ZACK_KEYS	zackKey;							// zacks key
} QSM_ZACK_DTL_REQ, *LPQSM_ZACK_DTL_REQ;

typedef struct _QSM_ZACK_DTL_RSP {					// Response for zacks Data
	BYTE		z, d;								// Message and subtype 'z(acks)' 'd(etail)'
	BYTE		nStatus:5,							// status of the request (0-31)
				resv:3;								// reserved
	ZACK_RECORD	zackRecord;							// zacks record ( zacks.h )
} QSM_ZACK_DTL_RSP, *LPQSM_ZACK_DTL_RSP;

typedef struct _QSM_ZACK_IREQ
{
	BYTE	z, i;
	BYTE	Sortfield:4,
			PageType:4;
	WORD	Industry;
    HWND   hwnd;
	BYTE	Command;
}QSM_ZACK_IREQ, *LPQSM_ZACK_IREQ;

typedef struct _QSM_ZACK_IRSP
{
	BYTE		z, i;
	WORD		nStatus:5,
				nSubCount:4,
				resv:7;
	WORD		wTotalCount;
	ZACK_RECORD	ZackRecord;
}QSM_ZACK_IRSP, *LPQSM_ZACK_IRSP;

typedef struct _QSM_ZACK_PPGREQ		  //for PPG Import request
{
	BYTE	z, p;
	BYTE	Sortfield : 4,
			PageType  : 4;
	WORD	wTotalTicker;
	HWND    hwnd;
	BYTE	Command;
	char	sTicker[500];
}QSM_ZACK_PPGREQ, *LPQSM_ZACK_PPGREQ;

typedef struct _QSM_ZACK_PPGRSP
{
	BYTE		z, p;
	WORD		nStatus:5,
				nSubCount:4,
				resv:7;
	WORD		wTotalCount;
	ZACK_RECORD	ZackRecord;
}QSM_ZACK_PPGRSP, *LPQSM_ZACK_PPGRSP;

#endif

// The New 'r' message: Get multiple blocks

#define	MAX_DEF					12
typedef	struct
		{
		BYTE	DataType;			// DataType
		char	Symbol[11];			// Symbol
		}
		DTSYM, *LPDTSYM;

typedef	struct
		{
		BYTE	r;					// Small r - just like before 
		BYTE	dtFlag;				// 0xFF - new 'r' flag
		BYTE	count;				// Number of symbols.
		BYTE	flag;				// Unused at this time.
		DTSYM	dtSyms[MAX_DEF];	// (Datatype/Symbol)s
		}
		QSM_RQNEWR, *LPQSM_RQNEWR;

typedef	struct
		{
		BYTE	r;					// Small r - just like before 
		BYTE	dtFlag;				// 0xFF - new 'r' flag
		BYTE	count;				// Number of symbols.
		BYTE	flag;				// Unused at this time.
		BYTE	cmpBlks[516];		// (Datatype/DataPtr/Data[40])s
		}
		QSM_RSNEWR, *LPQSM_RSNEWR;

/*
**** Special Types and structures used by ATAOEORD
*/

typedef struct
		{
		BYTE	SellBuy;			// S-Sell or B-Buy.
		BYTE	Flags;				// Bit 0 - MktMaker pref.
									//     1 - Price Override 
									//     2 - Use 20% load 
		char	Symbol[12];			// Symbol.
		char	MktMaker[6];		// Market Maker.
		BYTE	OrderPrice[3];		// Order price.
		BYTE	decimal;			// Order price decimal type.
		DWORD	Qty;				// Order quantity.
		BYTE	nMarket;			// Market.
		BYTE	nExecSystem;		// Execution system.
		char	Account[16];		// Account Number.
		WORD	wFillDate;			// FillDate.
		WORD	wFillTime;			// FillTime.
		DWORD   OrderID;			// QS Order ID
		}
		ORDER, FAR *LP_ORDER;

/*************************** Server Messages ****************************/

typedef struct
		{
		BYTE	O,A;				// Message type 'O'rder 'A'ccounts
		}
		QSM_OA_REQ;					// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	O,A;				// Message type 'O'rder 'A'ccounts
		BYTE	nAccounts;			// Total accounts.
		char	Accounts[16][16];   // Account numbers.			
		}
		QSM_OA_RSP;					// Message received from QuoteServer

typedef struct
		{
		BYTE	O,C;				// Message type 'O'rder 'C'ancel
		DWORD   OrderID;			// QS Order ID
		}
		QSM_OC_REQ;					// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	O,C;				// Message type 'O'rder 'C'ancel
		BYTE	Status;				// Order Rejected/Accepted.
		}
		QSM_OC_RSP;					// Message received from QuoteServer
 
typedef struct
		{
		BYTE	O,E;				// Message type 'O'rder 'E'xecute
		ORDER   order;				// Order
		}
		QSM_OE_REQ;					// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	O,E;				// Message type 'O'rder 'E'xecute
		BYTE	Status;				// Order Rejected/Accepted.
		DWORD   OrderID;			// QS Order ID
		}
		QSM_OE_RSP;					// Message received from QuoteServer
 
typedef struct
		{
		BYTE	O,F;				// Message type 'O'rder 'F'illed
		WORD	count;				// Zero first time, Start number next.
		char    Account[16];		// Account Number
		}
		QSM_OF_REQ;					// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	O,F;				// Message type 'O'rder 'F'illed
		WORD	nOrders;			// Total orders.
		ORDER	order[9];			// Up to NINE Order WADE CHANGED TO 9
		}
		QSM_OF_RSP;					// Message received from QuoteServer

typedef struct
		{
		BYTE	O,P;				// Message type 'O'rder 'P'ending
		WORD	count;				// Zero first time, Start number next.
		char    Account[16];		// Account Number
		}
		QSM_OP_REQ;					// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	O,P;				// Message type 'O'rder 'P'ending
		WORD	nOrders;			// Total orders.
		ORDER	order[9];			// Up to NINE Order WADE CHANGED TO 9
		}
		QSM_OP_RSP;					// Message received from QuoteServer

// First Call messages

typedef struct
		{
		BYTE	f,B;				// Message type 'fB'
		WORD	count;				// count (max 10)
		char    brokers[30];		// broker id''s 
		}
		QSM_FC_BROKER_REQ;			// Message sent to QuoteServer
	
typedef struct
		{
		BYTE	f,B;				// Message type 'fB'
		char    brokers[480];		// Up to ten brokers
		}
		QSM_FC_BROKER_RSP;			// Message received from QuoteServer

typedef struct _QSM_REQ_STAT {
	BYTE       C;
	BYTE       nGMC;
	BYTE       nGroup;
	BYTE       zero;
} QSM_REQ_STAT, *PQSM_REQ_STAT, *LPQSM_REQ_STAT;

typedef struct _STATISTIC {
	BYTE	group;
	BYTE	type;
	DWORD	up;
	DWORD	down;
	DWORD	unch;
	BYTE	exponent;
	BYTE	GMC;
} STATISTIC, *PSTATISTIC, *LPSTATISTIC;

typedef struct _QSM_RSP_STAT {
	BYTE       C;
	BYTE       nGMC;
	BYTE       nGroup;
	BYTE       zero;
	STATISTIC  stats[4];
} QSM_RSP_STAT, *PQSM_RSP_STAT, *LPQSM_RSP_STAT;

#if defined(INCL_RXF)
typedef	union
		{
		char		key[16];
		IERX_REC	record;
		}
		IX_BLK;

typedef	struct
		{
		BYTE	Q,I;
		WORD	status;
		IX_BLK	blk;
		}
		QSM_QI;
#endif

// These structures are used by the Market Marker messages that
//       can combine a request for compressed MM info with and
//       advise on associated MM''s... cool, eh?

typedef struct {
	WORD  decType: 4;				// decimal type
	WORD  bDate: 1;					// bDate - ONE means date
	WORD  extra: 2;					// unused
	WORD  month: 4;					// month
	WORD  day: 5;					// day
} EMC_DECDATE, *PEMC_DECDATE, *LPEMC_DECDATE;

typedef struct {
	WORD  decType: 4;				// decimal type
	WORD  bDate: 1;					// bDate - ZERO means time
	WORD  hour: 5;					// hour
	WORD  minute: 6;				// minute
} EMC_DECTIMEDATE, *PEMC_DECTIMEDATE, *LPEMC_DECTIMEDATE;

typedef union {
	EMC_DECDATE      date;
	EMC_DECTIMEDATE  time;
} EMC_MISC, *PEMC_MISC, *LPEMC_MISC;

typedef struct {
	DWORD     compType: 2;			// compression type
	DWORD     mmid0: 5;				// MMID - ASCII 
	DWORD     mmid1: 5;				//   0 = <space>
	DWORD     mmid2: 5;				//   1 = 'A'
	DWORD     mmid3: 5;				//   2 = 'B'
	DWORD     qType: 5;				// quote type - ASCII
	DWORD     qCond: 5;				// quote condition - ASCII
	EMC_MISC  misc;					// time/date/dectype 
} EMC_HEADER, *PEMC_HEADER, *LPEMC_HEADER;

typedef struct {
	WORD  bidPrice;					// full 16 bit bid value
	BYTE  askOffset;				// positive offset of Ask from Bid
	BATSV bidTSV;
	BATSV askTSV;
} EMC_COMP0, *PEMC_COMP0, *LPEMC_COMP0;

typedef struct {
	BYTE  bidPrice[3];				// full 24-bit bid value
	WORD  askOffset;				// positive offset of Ask from Bid
	BATSV bidTSV;
	BATSV askTSV;
} EMC_COMP1, *PEMC_COMP1, *LPEMC_COMP1;

typedef struct {
	BYTE  askFlag: 1;
	BYTE  valueHi: 7;
	WORD  valueLo;
	BATSV baTSV;
} EMC_COMP2, *PEMC_COMP2, *LPEMC_COMP2;

//	For single database servers (version >= 605) , use 'eN'

typedef struct {
	BYTE    e, M;					// 'eM': message type
	BYTE    command;				// bit 7 = advise,
									// bit 2 = no data
									// bit 1 = next
	DTDP	dtdp;					// dataptr of underlying	
	BYTE    symbol[9];				// symbol of underlying
} QSM_RQ_EMAD;

typedef struct {
	BYTE  e, M;						// 'eM': message type
	BYTE  count;					// # of MM''s, bit 7 = done
	BYTE  data[531];				// compressed MM records
} QSM_RS_EMAD;

// Arbiter Messages

#define AT_LIST_FIRST	0	 // Used to start getting QS List
#define	AT_LIST_NEXT	0x80 // OR this with QSn to continue list

typedef struct
{
	BYTE	a,T;			// The 'aT' arbiter transaction msg
	BYTE	command;		// Command byte
	char	sLogin[20];		// User ID (req's if cmd byte is AT_LIST_FIRST)
}
QSM_AT_REQ;

typedef	struct
{
	BYTE	a,T;			// The 'aT' arbiter transaction msg
	BYTE	status;			// Status: 0 = unknown user, else # of QS's in list
							//         if bit 7 set means more list to follow.
	BYTE	qsList[530];	// QS List Data (variable)
}
QSM_AT_RSP;

// News Source ID's translation table

// Misc Flag - bit breakdown
#define	NSTYPE_FLAGMASK		0x000f
#define	NSTYPE_ATFI			0
#define	NSTYPE_DESKDATA		1

#define	NSDATA_FLAGMASK		0x0030
#define	NSDATA_SHORT		0
#define	NSDATA_LONGV		1

#define	NSDATA_MAX_SHORT	89
#define	NSDATA_MAX_LONG		6

typedef struct
		{
		BYTE	f,N;				// Message type 'fN'
		WORD	flag;				// Misc flag
		WORD	source;				// Starting source number
} QSM_NEWS_SOURCE_REQ;		// Message sent to QuoteServer
	
typedef struct
		{
		WORD	source;				// Source number
		char	newsType[4];		// News Type
} NEWS_SRC_TYPE;

typedef struct
		{
		WORD	source;				// Source number
		char	newsType[4];		// News Type (short ID)
		char	longDescription[80];// Long News Source Description
} NEWS_LONGSRC_TYPE;

typedef struct
		{
		BYTE	f,N;				// Message type 'fN'
		WORD	flag;				// Misc flag
		WORD	count;				// count (max 89 or 6)
		union						// Array of known news sources.
		{
			NEWS_SRC_TYPE		newsSrcType[NSDATA_MAX_SHORT];
			NEWS_LONGSRC_TYPE	newsLongType[NSDATA_MAX_LONG];
		};
} QSM_NEWS_SOURCE_RSP;		// Message received from QuoteServer

/*-------------- The Union of all QuoteServer Messages ----------------------*/

typedef union _QSM_MSGS {
	UCHAR							data[540];
	QSM_ZP							msgZP;
	QSM_ZQ							msgZQ;
	QSM_ZS							msgZS;
	QSM_VIDEO						msgZV;
	QSM_RQ_QSICAP                   reqICap;
	QSM_RS_QSICAP                   rspICap;
	QSM_ADVISE						advise;
	QSM_GLD							msgGLD;
	QSM_ENUM						msgEnum;
	REQ_LOGIN						reqLogin;
	RSP_LOGIN						rspLogin;
	REQ_LOGOUT						reqLogout;
	RSP_LOGOUT						rspLogout;
	REQ_LOGSTAT						reqLogStat;
	RSP_LOGSTAT						rspLogStat;
	REQ_LOGPERM						reqLogPerm;
	RSP_LOGPERM						rspLogPerm;
	REQ_LOGCON						reqLogCon;
	RSP_LOGCON						rspLogCon;
	REQ_LOGEXT						reqLogExt;
	RSP_LOGEXT						rspLogExt;
	Q1_REQ							reqQ1;
	Q1_RSP							rspQ1;
	Q2_REQ							reqQ2;
	Q2_RSP							rspQ2;
	QSCREATES						reqCreate;
	QSCREATER						rspCreate;
	QSOPENS							reqOpen;
	QSOPENR							rspOpen;
	QSKILLQ							reqKill;
	QSKILLR							rspKill;
	Qr_REQ							reqReadFN;
	Qr_RSP							rspReadFN;
	QSREADS							reqRead;
	QSREADR							rspRead;
	QSSEEKS							reqSeek;
	QSSEEKR							rspSeek;
	QSCLOSE							reqClose;
	QSWRITES						reqWrite;
	QSWRITER						rspWrite;
#ifdef __NEWS_DEFINES
	QSM_REQ_NEWSCAPS				reqNewsCaps;
	QSM_RSP_NEWSCAPS				rspNewsCaps;
	QSM_REQ_SEARCH					reqSearch;
	QSM_RSP_SEARCH					rspSearch;
	QSM_REQ_GETHEADLINES			reqHeadlines;
	QSM_RSP_GETHEADLINES			rspHeadlines;
	QSM_REQ_CLRINST					reqClearInst;
	QSM_RSP_CLRINST					rspClearInst;
	QSM_REQ_CATCODES				reqCatCodes;
	QSM_RSP_CATCODES				rspCatCodes;
	QSM_REQ_GENCATCODES				reqGenCats;
	QSM_RSP_GENCATCODES				rspGenCats;
	QSM_REQ_OLDRECORD				reqOldRecord;
	QSM_RSP_OLDRECORD				rspOldRecord;
	QSM_REQ_RECORD					reqRecord;
	QSM_RSP_RECORD					rspRecord;
	QSM_REQ_STORY					reqStory;
	QSM_RSP_STORY					rspStory;
	QSM_REQ_XSEARCH					reqXSearch;
	QSM_RSP_XSEARCH					rspXSearch;
	QSM_REQ_ZSEARCH					reqZSearch;
	QSM_RSP_ZSEARCH					rspZSearch;
	QSM_RQ_CSPL						reqCompr;
	QSM_RS_CSPL						rspCompr;
	QSM_RQ_TSR						reqOldStory;
	QSM_RS_TSR						rspOldStory;
	QSM_NEWSINFO					msgNewsInfo;	// S&P and "mutant" TK QuoteServers only
	QSM_INQRNF						msgInqRNF;
	QSM_PROCRNF						msgProcRNF;
	QSM_REQ_COMPDATA				reqCompData;
	QSM_RSP_COMPDATA				rspCompData;
#endif
	QSM_GRQ							msgGRQ;
	QSM_RPS							msgRPS;
	QSM_RBQ							msgRBQ;
	QSM_INFO						msgInfo;
	QSM_QX							msgQX;
	QSM_ZDINFO						msgZDInfo;
	QSM_MSRQ						reqTOS;
	QSM_MSRP						rspTOS;			// Changed 1/28/93: Steve chg''d PQ QS''s
	QSM_REQ_RSEARCH					reqRetSearch;
	QSM_RSP_RSEARCH					rspRetSearch;
	QSM_RTS							reqRetran;
	QSM_RTR							rspRetran;
	QSM_REQ_ALTPATH					reqAltPath;
	QSM_RSP_ALTPATH					rspAltPath;
	QSM_REQ_BLOCK					reqBlock;
	QSM_PBLISTS						reqListA;
	QSM_PBLISTR						rspListA;
	QSM_PQLISTS						reqList;
	QSM_PQLISTR						rspList;
	QSM_QVS							reqBtr;
	QSM_QVMS						reqBtrM;
	QSM_QVR							rspBtr;
	QSM_HSRCHRQ						reqHistChr;
	QSM_HSRCHRS						rspHistChr;
	QSM_CMPHISTRQ					reqHistCmp;
	QSM_CMPHISTRS					rspHistCmp;
	QSM_DBREQ						reqDB;
	QSM_DBRSP						rspDB;
	QSM_RANKREQ						reqRank;
	QSM_RANKRSP						rspRank;
	QSM_FCALLSYMREQ					reqFCSym;
	QSM_FCALLSYMRSP					rspFCSym;
	QSM_FCALLBLOCKREQ				reqFCBlock;
	QSM_FCALLBLOCKRSP				rspFCBlock;
	QSM_RQB							reqBlk;
	QSM_RSB							rspBlk;
	QSM_PFI_RQ						reqPInfo;
	QSM_PFI_RS						rspPInfo;
	QSM_PFK_RS						rspPInfoK;
	QSM_RQNEWR						reqNewr;
	QSM_RSNEWR						rspNewr;
	QSM_RQQT						reqCmpTOS;
	QSM_RSQT						rspCmpTOS;
	QSM_PTOSRQ						reqPrvTOS;
	QSM_PTOSRS						rspPrvTOS;
	QSM_RQQA						reqCmpOpt;
	QSM_RSQA						rspCmpOpt;
	QSM_KRPAGE_RQ					reqKRPage;
	QSM_KRPAGE_RS					rspKRPage;
	QSM_KNP							reqKRNP;

#if defined (_DISCLOSE_H)
	QSM_DISC_REQ					reqDisc;
	QSM_DISC_RSP					rspDisc;
	QSM_DISC_KEYSUM_REQ				reqDiscKeySum;
	QSM_DISC_KEYSUM_RSP				rspDiscKeySum;
	QSM_DISC_SYMSUM_REQ				reqDiscSymSum;
	QSM_DISC_SYMSUM_RSP				rspDiscSymSum;
#endif

	QSM_RQSG						reqSymGuide;
	QSM_RSSG						rspSymGuide;

#if defined(_VICKERS_H)
	QSM_VICK_REQ					reqVick;
	QSM_VICK_RSP					rspVick;
	QSM_VICK_SUM_REQ				reqVickSum;
	QSM_VICK_SUM_RSP				rspVickSum;
	QSM_VICK_INSIDER_REQ			reqVickInsider;
	QSM_VICK_INSIDER_RSP			rspVickInsider;
	QSM_VICK_INSIDER_SUM_REQ		reqVickInsiderSum;
	QSM_VICK_INSIDER_SUM_RSP		rspVickInsiderSum;
	QSM_VICK_FIVEPERCENT_REQ		reqVickFivePercent;
	QSM_VICK_FIVEPERCENT_RSP		rspVickFivePercent;
	QSM_VICK_FIVEMORE_REQ			reqVickFiveMore;
	QSM_VICK_FIVEMORE_RSP			rspVickFiveMore;
	QSM_VICK_FIVEPERCENT_SUM_REQ	reqVickFivePercentSum;
	QSM_VICK_FIVEPERCENT_SUM_RSP	rspVickFivePercentSum;
	QSM_VICK_INSTNUM_SEND			outVickInstNum;
	QSM_VICK_INSTNUM_RSP			rspVickInstNum;
#endif
	
	QSM_OA_REQ						reqOEAcc;
	QSM_OA_RSP						rspOEAcc;
	QSM_OC_REQ						reqOECancel;
	QSM_OC_RSP						rspOECancel;
	QSM_OE_REQ						reqOEOrder;
	QSM_OE_RSP						rspOEOrder;
	QSM_OF_REQ						reqOEFill;
	QSM_OF_RSP						rspOEFill;
	QSM_OP_REQ						reqOEPend;
	QSM_OP_RSP						rspOEPend;
	QSM_FC_BROKER_REQ				reqFCBroker;
	QSM_FC_BROKER_RSP				rspFCBroker;
	QSM_REQ_STAT                    reqStat;
	QSM_RSP_STAT                    rspStat;

#if defined(INCL_RXF)
	QSM_QI                          reqQI;
	QSM_QI                          rspQI;
#endif

#if defined(_ZACKS_H)
	// for Zacks database
	QSM_ZACK_SUM_REQ                reqZackSum;  // SUMMARY REQUEST
	QSM_ZACK_SUM_RSP                rspZackSum;	 // SUMMARY RSPONSE
	QSM_ZACK_DTL_REQ                reqZackDtl;
	QSM_ZACK_DTL_RSP                rspZackDtl;
	QSM_ZACK_IREQ					reqZackInd;
	QSM_ZACK_IRSP					rspZackInd;
	QSM_ZACK_PPGREQ					reqZackPPG;
	QSM_ZACK_PPGRSP					rspZackPPG;
#endif

	QSM_RQ_GBLD                     reqGBLD;
	QSM_RS_GBLD                     rspGBLD;
	QSM_BB_GLD						msgBBGLD;
	QSM_RQ_EMAD                     reqEnumMM;
	QSM_RS_EMAD                     rspEnumMM;

	QSM_RQ_TOSFR                    reqTOSFiltRead;
	QSM_RS_TOSFR                    rspTOSFiltRead;

	QSM_RQ_CTREAD                   reqTOSCompRead;
	QSM_RS_CTREAD                   rspTOSCompRead;

#if defined(_MNSTAR_H_)
	// for Morning Star database
	QSM_MNSTAR_SUM_REQ				reqMnstarSum;
	QSM_MNSTAR_SUM_RSP				rspMnstarSum;
	QSM_MNSTAR_SINGLE_REQ			reqMnstarSingle;
	QSM_MNSTAR_SINGLE_RSP			rspMnstarSingle;
	QSM_MNSTAR_LIST_REQ				reqMnstarList;
	QSM_MNSTAR_FILE_REQ				reqMnstarFile;
	QSM_MNSTAR_LIST_RSP				rspMnstarList;
	QSM_MNSTAR_FILE_RSP				rspMnstarFile;
#endif

	// for Halt Data
	QSM_RQ_HALT						reqHalt;
	QSM_RS_HALT						rspHalt;

#ifdef __ADV_SERVER_REQUESTS
	QSMX_HEADER                     header;
	QSMX_REQUEST                    dbReq;
	QSMX_REQUEST_KEY                dbKey;
	QSMX_REQUEST_DTDP               dbDtdpReq;
	QSMX_RESPONSE                   dbResp;
	QSMX_ADVISE_REQ                 advReq;
	QSMX_ADVISE_RESP                advResp;
	QSMX_WILDCARD_ADVISE_REQ        wildAdvReq;
	QSMX_WILDCARD_ADVISE_RESP       wildAdvResp;
	QSMX_WILDCARD_REQ               wildReq;
	QSMX_WILDCARD_RESP              wildResp;
	QSMX_BUNCHED_REQ                bunchReq;
	QSMX_SYMBOL_REQ                 symbolReq;
	QSMX_BUNCHED_RESP               bunchResp;
	QSMX_HIST_REQ                   histReq;
	QSMX_HIST_RESP                  histResp;
	QSMX_OPTION_REQ                 optionReq;
	QSMX_OPTION_RESP                optionResp;
	QSMX_INDEX_REQ                  indexReq;
	QSMX_INDEX_RESP                 indexResp;
	QSMX_HALT_REQ                   haltReq;
	QSMX_HALT_RESP                  haltResp;
#endif

	QSM_AT_REQ						arbReq;
	QSM_AT_RSP						arbRsp;

	// for News Source Translation
	QSM_NEWS_SOURCE_REQ				reqNewsSource;
	QSM_NEWS_SOURCE_RSP				rspNewsSource;
} QSM_MSGS, *LPQSM_MSGS;


	/*---------------------------------------------------*/
	/* broadcast IPX packets                             */
	/*---------------------------------------------------*/

typedef struct _QSU_TIME {
	BYTE	z;
	WORD	resv1;
	BYTE	text[11];
	WORD	resv2;
	WORD	time;
	WORD	resv3;
	BYTE	day;
	BYTE	month;
	WORD	year;
	BYTE	resv4[8];
} QSU_TIME;

typedef struct _QSU_UPDATE {
	BYTE	updType;						// Update type (befjopstv)
	BYTE	datatype;						// Data Type
	CHAR	sym[8];							// Symbol
	WORD	dataptr;						// Data pointer
	BYTE	data[1];						// update data
} QSU_UPDATE;

typedef struct _QSU_GOVPX {
	BYTE	updType;						// Update type (befjopstv)
	BYTE	datatype;						// Data Type
	WORD	dataptr;						// Data pointer
	BYTE	data[1];						// update data
} QSU_GOVPX;

typedef struct _QSU_QTUPDATE {
	BYTE	updType;						// Update type (QT)
	BYTE	datatype;						// Data Type
	WORD	dataptr;						// Data Pointer
	BYTE	ohllTbac;						// OHLL = High nibble: Open/High/Low/Last
											// Tbac = Low  nibble: TRBATE action code
	BYTE	dectype;						// Decimal type
} QSU_QTUPDATE;

typedef struct _QSU_JJKUPDATE {
		BYTE	K;								// Message type
		BYTE	type;							// 'F'	means Fixed format
		WORD	page;							// Intel order, based at 1
		BYTE	line;							// Based at 1
		BYTE	column;							// Based at 1
		WORD	length;							// Length of TextAttr data
		BYTE	textAttr[1];					// Text and Attribute data
} QSU_JJKUPDATE;

typedef struct _QSU_KRUPDATE {
		BYTE	K;								// Message type
		BYTE	type;							// 'F'	means Fixed format
		WORD	page;							// Intel order, based at 1
		BYTE	line;							// Based at 1
		BYTE	column;							// Based at 1
		WORD	length;							// Length of TextAttr data
		BYTE	textAttr[1];					// Text and Attribute data
} QSU_KRUPDATE;

typedef struct _QSU_FFUPDATE {
		BYTE	F;								// Message type
		BYTE	source;							// 'K' = Knight/Ridder, 'R'	= Reuters SF
		WORD	page;							// Intel order, based at 1
		BYTE	line;							// Based at 1
		BYTE	column;							// Based at 1
		WORD	length;							// Length of TextAttr data
		BYTE	textAttr[1];					// Text and Attribute data
} QSU_FFUPDATE;

typedef struct _QSU_RSFSTATUS {
		BYTE	E;								// Message type
		BYTE	Source;							// 'R' = Reuters
		BYTE	Type;							// 'P' = Page
		BYTE	Status;							// 'F' = Found, etc.
		WORD	id;								// Page number, etc. (if found)
		CHAR	Name[20];						// ASCII Name of Page, etc.
} QSU_RSFSTATUS;

typedef struct _QSU_RSFUPDATE {
		BYTE	h;								// Message type
		BYTE	datatype;						// Data type
		CHAR	Symbol[20];						// Symbol
		WORD	dataptr;						// Data pointer
		BYTE	data[1];						// update data (actual is about 64)
} QSU_RSFUPDATE;

typedef struct _QSU_MMUPDATE {
		BYTE	M;								// Message type
		BYTE	datatype;						// Data Type
		BYTE	filler;							// don''t know what this is!
		BYTE	Symbol[15];						// Symbol
		WORD	dataptr;						// Data pointer
		CHAR	data[1];						// update data (actual is 24 bytes)
} QSU_MMUPDATE;					

typedef struct _QSU_SPL2UPDATE {
		BYTE	updType;						// Update type (befjopstv)
		BYTE	datatype;						// Data Type
		SPQB  	spqb;
		WORD	undlDataptr;					// Underlying dataptr
} QSU_SPL2UPDATE;

typedef struct _QSU_MAIL {
		BYTE	Z;								// Message type 'Z'
		BYTE	type;							// Type of Mail
		BYTE	text[246];						// Text of Mail
} QSU_MAIL;

typedef	struct _QSU_BROKEN {
		BYTE	B;								// Message type 		= 'B'
		BYTE	R;	 							// Message Sub-type	    = 'R'
		BYTE	NodeAddr[10];					// Node Address
} QSU_BROKEN;

typedef struct	_QSU_YIKES {
		BYTE	Y;	 							// Message type			= 'Y'
		BYTE	L;	 							// Message Sub-type	    = 'L'
		BYTE	target[10];						// Who its for
		BYTE	source[10];						// Who its from
} QSU_YIKES;

typedef struct _QSU_RSFPNAC {
		BYTE	O;								// Message type	= 'O'
		BYTE	flag;
		CHAR	sPNAC[14];
		BYTE	dataLen;
		CHAR	data[232];
} QSU_RSFPNAC;

typedef struct
		{
		BYTE	V;				// Message type	= 'V'
		char	Symbol[11];		// Symbol/country/exchange.
		BYTE	decimal;		// Decimal type.
		BYTE	last[3];		// Last price.
		WORD	lastsize;		// Last size w/exponent.
		}
		QSU_BBUPDATE;

typedef	struct
		{
		BYTE	A,F;				// Message type 'A'uto Order 'F'illed
		BYTE	Status;				// Order Rejected/Filled.
		BYTE	Extra;				// Unused.
		ORDER   order;
		}
		QSU_ORDERFILL;				// Message received from QuoteServer

typedef struct
		{
		BYTE		H;				// Message type	= 'H'
		HALTDATA	haltData;		// Halt data.
		}
		QSU_HALTUPDATE;

typedef struct _QSU_MAXMSG {
		CHAR  buffer[512];
} QSU_MAXMSG;

typedef union _UPDATE_MSG {
		BYTE              type;
		BYTE              data[512];
		QSU_TIME          time;
#ifdef __NEWS_DEFINES
		QSU_NEWSEVENT     newsEvent;
		QSU_PROFILEEVENT  profileEvent;
		QSU_NEWSUPDATE    newsUpdate;
		QSU_SCROLL        scroll;
		QSU_SEARCHEVENT   searchEvent;
		QSU_INDEXUPD	  indexUpdate;
#endif
		QSU_UPDATE        update;
		QSU_GOVPX         gpxUpdate;
		QSU_QTUPDATE      qtUpdate;
		QSU_KRUPDATE      krUpdate;
		QSU_FFUPDATE      ffUpdate;
		QSU_RSFSTATUS     rsfStatus;
		QSU_RSFUPDATE     rsfUpdate;
		QSU_MMUPDATE      mmUpdate;	
		QSU_SPL2UPDATE    spl2Update;	
		QSU_MAIL          mail;
		QSU_BROKEN		  broken;
		QSU_YIKES         yikes;
		QSU_RSFPNAC       rsfPnac;
		QSA_UPDATE        shortMsg;
		QSU_BBUPDATE      bbUpdate;
		QSU_HALTUPDATE    haltUpdate;
		QSU_ORDERFILL     orderMsg;
		QSU_MAXMSG        undefined;
} UPDATE_MSG, *LPUPDATE_MSG;

typedef struct _BUNCHED_PAK {
		USHORT      length;
		UPDATE_MSG  data;
} BUNCHED_PAK, *LPBUNCHED_PAK;

typedef struct _BUNCHED_UPDATE {
		UCHAR        at;
		UCHAR        count;
		BUNCHED_PAK  pak;
} BUNCHED_UPDATE, *LPBUNCHED_UPDATE;

typedef struct _BCAST_MSG {
		BYTE        tag[2];
		WORD        totalLen;
		BYTE        seqNum;
		BYTE        sessionNum;
		WORD        unused;
		WORD        msgLen;
		UPDATE_MSG  data;
} BCAST_MSG, *LPBCAST_MSG;

typedef	struct _QSU_TICKS {
		BYTE	ticks[475];						// TICKs or PQ_TICKs
} QSU_TICKS, *LPQSU_TICKS;

typedef	struct _TICK_MSG {
		BYTE	tag[2];							// SB {01,00}
		WORD	totLen;							// Total len of {msgLen+data}
		BYTE	seqNum;							// Sequence Number
		BYTE	sesNum;							// Session  Number
		BYTE	rcdSize;						// Record Size	(Note: 0 means 19)
		BYTE	unused;							// Not used
		WORD	msgLen;							// Length of message that follows
		BYTE	data[502];		 				// Message data
} TICK_MSG, *LPTICK_MSG;

		
	/*---------------------------------------------------*/
	/* Special datatypes sent with an WM_AT_DATA message */
	/*---------------------------------------------------*/

#define SIZEOF_UPDHEADER	3*sizeof( DWORD )

typedef struct _UPDATE_PAK {
		DWORD  type;
		DWORD  qsdtID;
		DWORD  length;
		BYTE   data[1024];
} UPDATE_PAK, *LPUPDATE_PAK;

typedef struct {
		DWORD		type;
		DWORD		qsdtID;		
		DWORD		length;
		UPDATE_PB	updBlock;
} ATN_UPDATE, *LPATN_UPDATE;

typedef struct {
		DWORD	type;
		DWORD	qsdtID;				
		DWORD	length;
		KR_LB	krBlock;
} ATN_KRBLOCK, *LPATN_KRBLOCK;

typedef	struct
		{
		DWORD	type;
		DWORD	qsdtID;				
		DWORD	length;
		GPX_LB	gpxBlock;
} ATN_GPXUPDATE, *LPATN_GPXUPDATE;

typedef	struct
		{
		DWORD	type;
		DWORD	qsdtID;				
		DWORD	length;
		QSA_UPDATE	update;
} ATN_QTUPDATE, *LPATN_QTUPDATE;

typedef struct
{
	DWORD			type;
	DWORD			qsdtID;				
	DWORD			length;
	QSA_CONDITION	update;
}
ATN_CDUPDATE, *LPATN_CDUPDATE;

typedef struct	{				/* Fixed-Format (FF) Update */
	DWORD	type;
	DWORD	qsdtID;				
	DWORD	length;
	WORD	page;				// Intel order, based at 1
	BYTE	line;				// Based at 1
	BYTE	column;				// Based at 1
	WORD	lenTA;				// Length of TextAttr data
	BYTE	TextAttr[160];		// Text and Attribute data
} ATN_FFMT, *LPATN_FFMT;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// Reuters Select Feed Update
	DWORD	length;
	BYTE	Source;				// R = Reuters
	BYTE	Type;				// P = Page
	BYTE	Status;				// F = Found, etc. see table below
	WORD	Id;					// Reference number, etc.
	char	Name[20];			// ASCII Name of Page,etc.
} ATN_RSFSTATUS, *LPATN_RSFSTATUS;				// Values for ATN_RSFSTATUS (Status)
								// 'E' - Error
								// 'F' - Found, Use QSM_RPS to get text
								// 'N' - Not found
								// 'P' - Not permissioned
								// 'R' - Too many outstanding requests,later...
								// 'T' - Throttle limit hit
								// 'W' - Watch list full, try as later
								// 'X' - Max pages exceeded
								// 'Z' - Transmit buffer full, try later ?

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// Reuters Select Feed Update
	DWORD	length;
	char	Symbol[20];			// Symbol
	WORD	dataptr;			// Data ptr
	char	data[64];			// FieldList...Currency
} ATN_RSFUPDATE, *LPATN_RSFUPDATE;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// Reuters Select Feed PNAC info
	DWORD	length;
	BYTE	flag;
	char	sPNAC[14];
	BYTE	datalen;
	char	data[232];
} ATN_RSFPNAC, *LPATN_RSFPNAC;

typedef	struct {
	DWORD	type;
	DWORD	qsdtID;				// Telerate status
	DWORD	length;
	BYTE	Source;				// R = Reuters
	BYTE	Type;				// P = Page
	BYTE	Status;				// F = Found, etc.
	WORD	Id;					// QS Internal number
	WORD	Tag;				// Tag number
	WORD	PageNumber;			// Telerate Page Number
} ATN_TRTPAGE, *LPATN_TRTPAGE;

typedef	struct {
	DWORD	type;
 	DWORD	qsdtID;				// Telerate status
	DWORD	length;
 	BYTE	Source;				// T = Telerate
 	BYTE	Type;				// S = Status,T = TimeStamp,B = Broadcast
 	BYTE	Status;				// F = Found, etc.
 	WORD	MsgNum;				// Network Message Number
 	WORD	Tag;				// Tag number
 	char	Text[80];			// Message Text.
} ATN_TRTSTATUS, *LPATN_TRTSTATUS;

typedef	struct {
	DWORD	type;
	DWORD	qsdtID;				// Crossmar status
	DWORD	length;
	BYTE	Source;				// C = Crossmar
	BYTE	Type;				// P = Page
	BYTE	Status;				// F = Found, N = Not Found.
	WORD	Id;					// QS Page Number
} ATN_CPAGE, *LPATN_CPAGE;

typedef	struct {
	DWORD	type;
  	DWORD	qsdtID;				// Crossmar status
	DWORD	length;
  	BYTE	Source;				// C = Crossmar
  	BYTE	Type;				// S = Status,T = TimeStamp,B = Broadcast
  	BYTE	Status;				// F = Found, etc.
  	WORD	MsgNum;				// Network Message Number
  	WORD	Tag;				// Tag number
  	char	Text[80];			// Message Text.
} ATN_CSTATUS, *LPATN_CSTATUS;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// Market Maker Update
	DWORD	length;
	BYTE	datatype;			// Data type
	BYTE	filler;				// Steve says "filler"...not used.
	char	Symbol[8];			// Stock symbol
	char	Country[2];			// Country code
	char	Exchange;			// Exchange
	char	MMid[4];			// Market Maker ID
	WORD	dbaMask5;			// DBAX type 5 flag mask
	WORD	priceType;			// Price Type ( fraction )
	WORD	securityType;		// Security Type
	WORD	quoteIndicator;		// Quote Indicator
	LONG	BidPrice;			// Bid Price
	LONG	BidSize;			// Bid Size
	LONG	AskPrice;			// Ask Price
	LONG	AskSize;			// Ask size
	char	PacketTime[4];		// Some strange time storage area we want to decode
	WORD	undlDataptr;		// Underlying dataptr
} ATN_MMUPDATE, *LPATN_MMUPDATE;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// unused in this case!
	DWORD	length;
	char  	updType;
	BYTE  	datatype;
	SPQB  	spqb;
	WORD	undlDataptr;		// Underlying dataptr
} ATN_SPL2UPDATE, *LPATN_SPL2UPDATE;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// unused in this case!
	DWORD	length;
	DWORD	qsn;
} ATN_READVISE, *LPATN_READVISE;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// unused in this case!
	DWORD	length;
	DWORD	qsn;
} ATN_LOSTPACKET, *LPATN_LOSTPACKET;

typedef struct {
	DWORD	type;
	DWORD	qsdtID;				// unused in this case!
	DWORD	length;
	DWORD	qsn;
	DWORD	status;
} ATN_QSALARM, *LPATN_QSALARM;

typedef	struct
		{
		DWORD		type;
		DWORD		qsdtID;				// unused in this case!
		DWORD		length;
		DWORD		qsn;
		QSU_TIME	qsuTime;
		}
		ATN_QSTIME, *LPATN_QSTIME;

typedef struct
		{
		DWORD		type;
		DWORD		qsdtID;			// Adviesd.
		DWORD		length;
		char		Symbol[11];		// Symbol/country/exchange.
		BYTE		decimal;		// Decimal type.
		BYTE		last[3];		// Last price.
		WORD		lastsize;		// Last size w/exponent.
		}
		ATN_BBUPDATE, *LPATN_BBUPDATE;

typedef struct
		{
		DWORD		type;
		DWORD		qsdtID;			// Adviesd.
		DWORD		length;
		HALTDATA	haltData;
		}
		ATN_HALTUPDATE, *LPATN_HALTUPDATE;

typedef	struct
		{
		DWORD	type;
		DWORD	qsdtID;				// Adviesd.
		DWORD	length;
		BYTE	O,F;				// Message type 'O'rder 'F'illed
		BYTE	Status;				// Order Rejected/Filled.
		BYTE	Extra;				// Unused.
		ORDER   order;				// Order
		}
		ATN_ORDERFILL;				// Message received from QuoteServer
 
typedef	struct
		{
		DWORD			type;
		DWORD			qsdtID;		// Adviesd.
		DWORD			length;
		PORTFOLIO_PB	updBlock;
		}
		ATN_PORTFOLIO, *LPATN_PORTFOLIO;

typedef struct {
	BYTE	msg;
	BYTE	dataType;
	BYTE	symbol[8];
	BYTE	flag;
	WORD	time;
	BYTE	decimal;
	BYTE	price[3];
	WORD	size;
} TICK;							// Size is 19 bytes

typedef	struct {
	BYTE	msg;
	BYTE	dataType;
	BYTE	symbol[8];
	BYTE	flag;
	WORD	time;
	BYTE	decimal;
	BYTE	price[3];
	WORD	size;
	BYTE	country[2];
	BYTE	exchange;
} PQ_TICK;						// Size is 22 bytes

typedef	struct {
	DWORD	type;
	DWORD	qsdtID;
	DWORD	length;
	DWORD	nRecords;			// Number of TICK or PQ_TICK records
	DWORD	recSize;			// Will be 19 or 22
	BYTE	Ticks[475];			// TICKs or PQ_TICKs
} ATN_TICKS, *LPATN_TICKS;

typedef struct {
	DWORD		type;			// Type ATD_AT_PRICE
	DWORD		dtdp;			// Datatype/dataptr.		
	DWORD		length;			// Total length of update packet.
	DWORD		qsn;			// Quote server number.		
	QSUX_UPDATE updBlock;
} ATN_ATUPDATE, *LPATN_ATUPDATE;

typedef union _UPDATE {
	UPDATE_PAK		  data;
	ATN_UPDATE		  update;
	ATN_KRBLOCK		  krUpdate;
	ATN_GPXUPDATE	  gpxUpdate;
	ATN_QTUPDATE	  qtUpdate;
	ATN_CDUPDATE	  cdUpdate;
#ifdef __NEWS_DEFINES
	ATN_NEWS		  news;
	ATN_SCROLL		  scroll;
	ATN_NEWSEVENT	  newsEvent;
	ATN_PROFILEEVENT  profileEvent;
#endif
	ATN_FFMT		  ffmt;
	ATN_RSFSTATUS	  rsfStatus;
	ATN_RSFUPDATE	  rsfUpdate;
	ATN_RSFPNAC		  rsfPNAC;
	ATN_TRTPAGE		  trtPage;
	ATN_TRTSTATUS	  trtStatus;
	ATN_CPAGE		  cPage;
	ATN_CSTATUS		  cStatus;
	ATN_MMUPDATE	  mmUpdate;
	ATN_SPL2UPDATE	  spl2Update;
	ATN_READVISE      reAdvise;
	ATN_LOSTPACKET	  lostPacket;
	ATN_QSALARM       qsAlarm;
	ATN_QSTIME        qsTime;
	ATN_BBUPDATE      bbUpdate;
	ATN_ORDERFILL     order;
	ATN_PORTFOLIO     portUpdate;
	ATN_TICKS         ticks;
	ATN_HALTUPDATE    haltUpdate;
	ATN_ATUPDATE      atUpdate;
} UPDATE, *LPUPDATE;

		/* Quote Server short name translation. */

#define	QSN_TELEKURS			"TS"
#define	QSN_COMSTOCK			"SP"
#define	QSN_KNIGHTRIDDER		"KR"
#define	QSN_REUTERS_SF			"RS"
#define	QSN_NASDAQLEVEL2		"N2"
#define	QSN_REUTERS_MF			"RM"
#define	QSN_TELERATE			"TR"
#define	QSN_PCQUOTE				"PQ"
#define QSN_GTISMARKETDATA		"GM"
#define	QSN_JJKENNY				"JJ"
#define	QSN_GOVPX				"GP"
#define QSN_NTNEWSSERVER		"NS"
#define QSN_BRIDGE				"BR"
#define QSN_BLOOMBERG			"BB"
#define QSN_DISCLOSURE			"DI"
#define QSN_STREET_SOFTWARE		"SS"
#define QSN_VICKERS				"VI"
#define QSN_ZACKS				"ZK"
#define QSN_MNSTAR				"MN"
#define QSN_FIRSTCALL			"FC"
#define QSN_RESEARCH_NEWS		"RN"
#define	QSN_PCQUOTE2			"P2"
#define QSN_ATFI				"AT"

		/* Virtual Quote Server short name translation. */

#define QSN_VS_PORTFOLIO		"pt"

		/* Quote Server character type translation. */

#define	QSC_TELEKURS			'T'
#define	QSC_COMSTOCK			'S'
#define	QSC_KNIGHTRIDDER		'K'
#define	QSC_REUTERS_SF			'R'
#define	QSC_NASDAQLEVEL2		'N'
#define	QSC_REUTERS_MF			'M'
#define	QSC_TELERATE			'F'
#define	QSC_PCQUOTE				'P'
#define QSC_GTISMARKETDATA		'C'
#define	QSC_JJKENNY				'J'
#define	QSC_GOVPX				'G'
#define QSC_NTNEWSSERVER		'E'
#define QSC_BRIDGE				'I'
#define QSC_BLOOMBERG			'B'
#define QSC_DISCLOSURE			'D'
#define QSC_STREET_SOFTWARE		'O'
#define QSC_VICKERS				'V'
#define QSC_ZACKS				'Z'
#define QSC_MNSTAR				'*'
#define QSC_FIRSTCALL			'L'
#define QSC_RESEARCH_NEWS		'A'
#define	QSC_PCQUOTE2			'2'
#define QSC_ATFI				'H'

		/* Virtual Quote Server character type translation. */

#define QSC_VS_PORTFOLIO		'p'

		/* Values returned in QSSTATUS.Types field */

#define QST_TELEKURS			0			// 'T'
#define QST_COMSTOCK			1			// 'S'
#define QST_KNIGHTRIDDER		2			// 'K'
#define QST_REUTERS_SF			3			// 'R'
#define QST_NASDAQLEVEL2		4			// 'N'
#define QST_REUTERS_MF			5			// 'M'
#define QST_TELERATE			6			// 'F'
#define QST_PCQUOTE				7			// 'P'
#define QST_GTISMARKETDATA		8			// 'C'
#define QST_JJKENNY				9			// 'J'
#define	QST_GOVPX				10			// 'G'
#define QST_NTNEWSSERVER		11			// 'E'
#define QST_BRIDGE				12			// 'I'
#define QST_BLOOMBERG			13			// 'B'
#define QST_DISCLOSURE			14			// 'D'
#define QST_STREET_SOFTWARE		15			// 'O'
#define QST_VICKERS				16			// 'V'
#define QST_ORDER_EXECUTION		17			// 'X'
#define	QST_ZACKS				18          // 'Z'
#define QST_MNSTAR				19			// '*'
#define QST_FIRSTCALL			20			// 'P'
#define QST_RESEARCH_NEWS		21			// 'A'
#define QST_PCQUOTE2			22			// '2'
#define QST_ATFI				23			// 'H'

		/* Virtual Servers - Values returned in QSSTATUS.Types field */

#define QST_VS_PORTFOLIO		31			// 'p'

		/* Quote Server bitmap type translation. */

#define B_QST_TELEKURS			0x00000001
#define B_QST_COMSTOCK			0x00000002
#define B_QST_KNIGHTRIDDER		0x00000004
#define B_QST_REUTERS_SF		0x00000008
#define B_QST_NASDAQLEVEL2		0x00000010
#define B_QST_REUTERS_MF		0x00000020
#define B_QST_TELERATE			0x00000040
#define B_QST_PCQUOTE			0x00000080
#define B_QST_GTISMARKETDATA	0x00000100
#define B_QST_JJKENNY			0x00000200
#define	B_QST_GOVPX				0x00000400
#define B_QST_NTNEWSSERVER		0x00000800
#define B_QST_BRIDGE			0x00001000
#define B_QST_BLOOMBERG			0x00002000
#define B_QST_DISCLOSURE		0x00004000
#define B_QST_STREET_SOFTWARE	0x00008000
#define B_QST_VICKERS			0x00010000
#define B_QST_ORDER_EXECUTION	0x00020000
#define B_QST_ZACKS             0x00040000
#define B_QST_MNSTAR			0x00080000
#define B_QST_FIRSTCALL			0x00100000
#define B_QST_RESEARCH_NEWS		0x00200000
#define B_QST_PCQUOTE2			0x00400000
#define B_QST_ATFI				0x00800000

#define B_QST_VS_PORTFOLIO      0x80000000
#define B_QST_ALL				0x80ffffff

#define	MAX_QS					MAX_QSNUM	// Max number of QS''s per node

	/*------------------------------------------------------------*/
	/* This is the Structure returned by a GetSystemConfig() call */
	/*------------------------------------------------------------*/

typedef	struct
		{
		WORD			Version;			// DataSource Software Version
		char			VersionExtension;	// DataSource Software Version(x)
		unsigned char	Session;			// DataSource Session number
		unsigned long	MessageCount;		// Data feed message count
		unsigned long	ErrorCount;			// Data feed error count
		char			DataFeed;			// Data feed identifier
		}
		QSCONFIG;

typedef	struct	_tagExec
		{
		WORD	wSize;
		WORD	wCommand;
		WORD	wStatus;
		BYTE	byData[1];
		}
		EXEC;

typedef	EXEC *LPEXEC;

#define	EXEC_ATTACH_CFG		0			// Attach to QS''s in [NETRESW3] or CFG
#define	EXEC_ATTACH_LIST	1			// Attach to QS''s in byData list
#define	EXEC_ATTACH_BACKUP	2			// Attach to Backup QS
#define	EXEC_ATTACH_TEMP	3			// Attach to Temp QS
#define	EXEC_DETACH_ALL		4			// Detach from all QS''s
#define	EXEC_DETACH_TEMP	5			// Detach from Temp QS
#define	EXEC_DEFINE_BACKUP	6			// Define backup list for a QuoteServer
#define	EXEC_CLOSE_SERVER	7			// Close ATSERVER

// S&P Market Scope Data strutures and defines

typedef	struct _SPMSCOPE_DATA
		{
		char	symbol[8];				// Symbol (part 1 of key 1)
		char    country[2];				// Country Code (part 2, key1)
		char    exchange;				// Exchange Code (part 3, key1)
		char	type[3];				// Type of record (part 4, key1)
		char	companyName[26];		// Company name (key 2)
		char	szText[1994];			// Actual data
		}
		SPMSCOPE_DATA, *LPSPMSCOPE_DATA;

typedef struct _MSCOPE_DIR 
		{
		WORD  nextFree;					// next free
		BYTE  pageFlag[200];			// page present flag
		} MSCOPE_DIR, *LPMSCOPE_DIR;


///////////////////////////////////////////////////////////////////////////
// New-style history data access typedefs,constants and functions
///////////////////////////////////////////////////////////////////////////

typedef	struct
		{
		WORD		day:  5,		// Lower  5 bits = day
					month:4,		// Middle 4 bits = month
					year: 7;		// Upper  7 bits = year
		}
		HFDATE;						// History File Date

typedef	struct tagHFDR
		{
		BYTE	flag;		// Not currently used
		BYTE	decimal;	// Decimal code
		HFDATE	date;		// record date
		DWORD	close;		// close price
		DWORD	high;		// high
		DWORD	low;		// low
		DWORD	open;		// open
		DWORD	paClose;	// previous adjusted close
		DWORD	volume;		// volume
		DWORD	openint;	// open interest
		}
		HFDR;				// History File Daily Record

typedef	HFDR *LPHFDR;

/*---------- Native history formats -----------*/

typedef	struct
		{
		HFDATE	date;
		WORD	close;
		WORD	high;
		WORD	low;
		WORD	open;
		WORD	prevAdj;
		DWORD	volume;
		}
		DailyStock;

typedef	struct
		{
		HFDATE	date;
		BYTE	fdectype;
		BYTE	fclose[3];
		SHORT	fhigh;
		SHORT	flow;
		SHORT	fopen;
		WORD	fvolume;
		WORD	fopenint;
		}
		DailyFuture;

typedef struct
		{
		HFDATE	date;
		BYTE	decimal;
		BYTE	last[3];	// longint format 
		SHORT	high;		// offset from last
		SHORT	low;		// offset from last
		SHORT	open;		// offset from last
		BYTE	volume[3];	// longint format
		BYTE	extra;
		}
		DailyEurope;

typedef struct
		{
		HFDATE	date;
		BYTE	decimal;
		BYTE	last[3];
		BYTE	high[3];
		BYTE	low[3];
		BYTE	open[3];
		BYTE	extra;
		}
		DailyKRCash;			// Knight-Ridder Cash

typedef struct
		{
		HFDATE	date;
		BYTE	decimal;
		BYTE	last[3];
		BYTE	high[3];
		BYTE	low[3];
		DWORD	volume;
		}
		DailyArchive;			// MetroMedia History

typedef	struct
		{
		HFDATE	date;
		WORD	dividend;		   // Single $ in 100ths ( 161   = $           1.61 )
		LONG	netIncome;		   // Million $ in 10ths ( 123   = $  12,300,000.00 )
		DWORD	revenue;		   // Million $ in 10ths ( 1849  = $ 184,900,000.00 )
		DWORD	sharesOutstanding; // Thousands of Shares( 21483 =    21,483,000    )
		}
		QuarterlyFundamental;

typedef struct
		{
		HFDATE	date;
		BYTE	decimal;
		BYTE	last[3];	// longint format 
		short	high;		// offset from last
		short	low;		// offset from last
		short	open;		// offset from last
		DWORD	volume;		// longint format
		}
		DailyCCE;

typedef	union
		{
		DailyStock		stock;
		DailyFuture		future;
		DailyEurope		europe;
		DailyKRCash		krcash;
		DailyArchive	archive;
		DailyCCE		cce;
		}
		DailyChart;


typedef DailyEurope HISTORY_RAW;
typedef HISTORY_RAW* LPHISTORY_RAW;

typedef DailyFuture HISTORYFUT_RAW;
typedef HISTORYFUT_RAW* LPHISTORYFUT_RAW;

typedef DailyArchive HISTORYARCH_RAW;
typedef HISTORYARCH_RAW* LPHISTORYARCH_RAW;

/*------------------     Basic 32-bit API      -------------------*/
#define ATLMSG_ATTACH			0x0000
#define ATLMSG_SRVRREQ			0x0001
#define ATLMSG_CONNECT			0x0002
#define ATLMSG_DISCONNECT 		0x0003
#define ATLMSG_ADVISE 			0x0004
#define ATLMSG_UNADVISE			0x0005
#define ATLMSG_STATUS			0x0006
#define ATLMSG_QSINFO			0x0007
#define ATLMSG_QSOPEN			0x0008
#define ATLMSG_QSCLOSE			0x0009
#define ATLMSG_QSINIT			0x000a
#define ATLMSG_QSEXIT			0x000b
#define ATLMSG_SRVRREQ_ASYNC	0x000c
#define ATLMSG_GETNETADDR		0x000d
#define ATLMSG_DETACH			0x000e
#define	ATLMSG_UPDATE			0x000f
#define	ATLMSG_OPEN_VQS			0x0010
#define	ATLMSG_CLOSE_VQS		0x0011
#define	ATLMSG_UPDATE_VQS		0x0012
#define ATLMSG_VQS_DATA			0x0013
#define ATLMSG_QSINFO_EX		0x0014
#define ATLMSG_QSACTION			0x0015
#define ATLMSG_ADVISE_TEST		0x0016	// Uses ATLOCAL_ADVISE msg structure
#define ATLMSG_GROUP_ADVISE		0x0017
#define	ATLMSG_LAST_VALID_MSG	0x0017	// Last valid message

typedef struct _ATLOCAL_HEADER
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
}
ATLOCAL_HEADER;

typedef struct _ATLOCAL_ATTACH
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;	
	DWORD		pid;
	DWORD		update;
	UCHAR		name[256];
}
ATLOCAL_ATTACH;

typedef struct _ATLOCAL_DETACH
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
}
ATLOCAL_DETACH;

typedef struct _ATLOCAL_SRVRREQ
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qsn;
	union
	{
		QSM_MSGS	qsmMsg;
		BYTE		bigMsg[MAX_IP_PACKET];	// This accomodates the Big Messages
	};
}
ATLOCAL_SRVRREQ;

typedef struct _ATLOCAL_CONNECT
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
}
ATLOCAL_CONNECT;

typedef struct _ATLOCAL_ADVISE
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qs;
	DTDP		dtdp;
}
ATLOCAL_ADVISE;

typedef struct _ATLOCAL_GROUP_ADVISE
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	WORD		bAdvise;		// TRUE means Advise, FALSE means Unadvise
	WORD		nAdvises;		// Number of dtdp in array
	DWORD		qs;				// QuoteServer number
	DWORD		dtdp[1];		// Array of dtdp's
}
ATLOCAL_GROUP_ADVISE;

#if 0
typedef struct _ATLOCAL_ATADVISE
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qsn;
	DWORD		dtdp;
	}
ATLOCAL_ATADVISE;
#endif

typedef struct _ATLOCAL_QSINFO		// This msg structure is used to: obtain
{							// 	obtain QS Info
	DWORD		type;			//  connect to "temporary" QS's
	DWORD		length;			//  connect to "virtual"   QS's
	DWORD		status;
	DWORD		dwID;
	DWORD		qsn;
	QS_INFO		data;
}
ATLOCAL_QSINFO;

#define QSI_TPBDATA	0

typedef struct _ATLOCAL_QSINFO_EX	// This msg structure is used to: obtain
{									// 	obtain Extended QS Info
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qsn;
	DWORD		iType;			// Type of info requested/returned (QSI_*)
	BYTE		data[540];		// Variable format data returned
}
ATLOCAL_QSINFO_EX;

#define NAT_IPXSPX		0
#define NAT_TCPIP		1

typedef struct _ATLOCAL_GETNETADDR
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;	
	DWORD		qsn;
	DWORD		netType;
	BYTE		address[16];
}
ATLOCAL_GETNETADDR;

typedef struct _ATLOCAL_UPDATE
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	UPDATE_PAK	updatePak;
}
ATLOCAL_UPDATE;

typedef struct _ATLOCAL_VQSDATA
{
	DWORD		type;
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qsmLen;
	QSM_MSGS	qsm;
}
ATLOCAL_VQSDATA;

typedef struct _ATLOCAL_QSACTION	// This msg structure is used to:
{							// 	Force a reconnect on a QS
	DWORD		type;			//  or other such violent thing!
	DWORD		length;
	DWORD		status;
	DWORD		dwID;
	DWORD		qsn;			// QS to operate on
	DWORD		dwAction;		// What to do to it
	BYTE		data[1];		// Any optional data required
}
ATLOCAL_QSACTION;


typedef union _ATLOCAL_MESSAGE
{
	ATLOCAL_HEADER			header;
	ATLOCAL_ATTACH			attach;
	ATLOCAL_SRVRREQ			qsReq;
	ATLOCAL_CONNECT			connect;
	ATLOCAL_ADVISE			advise;
	ATLOCAL_QSINFO			qsInfo;
	ATLOCAL_QSINFO_EX		qsInfoEx;
	ATLOCAL_GETNETADDR		getNetAddr;
	ATLOCAL_UPDATE			update;
	ATLOCAL_VQSDATA			vqsData;
//	ATLOCAL_ATADVISE		atadvise;
	ATLOCAL_QSACTION		qsAction;
	ATLOCAL_GROUP_ADVISE	groupAdvise;
}
ATLOCAL_MESSAGE;


#define QSDTID(qsn,datatype,dataptr) (DWORD) ((qsn << 24) |					\
										((datatype << 16) & 0x00ff0000) |	\
										  dataptr & 0x0000ffff)

typedef DWORD (CALLBACK * GREQUESTPROC)(LPBYTE,UINT,LPBYTE);
typedef DWORD (CALLBACK * ATMESSAGEPROC)(VOID *,DWORD,DWORD);

	// New TCP IPC HAT

typedef struct _HAT
		{
		SOCKET			  socket;
		int				  iStatus;
		HANDLE			  hRecvReady;
		HANDLE			  hReadThread;
		HANDLE			  hUpdateThread;
		ATLOCAL_MESSAGE	  rspMessage;
#ifdef __cplusplus
		RingBuffer		  *updateRingBuffer;
#else
		void              *updateRingBuffer;
#endif
		volatile BOOL	  bRingBufferValid;
		DWORD			  (WINAPI *callbackProc)(void *buffer,
											   DWORD length,DWORD param);
		DWORD			  dwParam;
		GREQUESTPROC	  vqsCallback;
		UINT			  vQSN;
		CRITICAL_SECTION  lock;
		WORD			  wSrcSeq;		// Sequence number info (updates only)
		}
		HAT;

typedef struct _ATCOPTIONS
{
	DWORD	dwVersion;		// Version of this structure (currently 1 is the max defined)
	char	szAtsIP[32];	// Alternate local IP addr (of ATServer) [0] == 0 : use default
	WORD	wAtsPort;		// Alternate local IP port (of ATServer)  0  == 0 : use default
	WORD	wUpdRingSize;	// Update Ring size 0: default (32k), else * 1k max is 256K
}
ATCOPTIONS,*LPATCOPTIONS;

// AtcQSAction definitions

#define QSACT_FORCERECONNECT	0
#define QSACT_PURGEMSGCACHE		1

//	AT Client API functions

DWORD WINAPI  AtcAttach(HAT **hAt,ATMESSAGEPROC,DWORD param);

DWORD WINAPI  AtcAttachEx(HAT **hAt,ATMESSAGEPROC,DWORD param,LPATCOPTIONS atcOptions);

DWORD WINAPI  AtcDetach(HAT **hAt);

DWORD WINAPI  AtcRequest(HAT *hAt,UINT qsn,VOID *msgIn, DWORD lenIn,
						 				   VOID *msgOut,DWORD *lenOut);

DWORD WINAPI  AtcAsyncRequest(HAT *hAt,UINT qsn,VOID *msgIn,DWORD lenIn,
									   VOID *msgOut,DWORD *lenOut);

DWORD WINAPI  AtcConnect(HAT *hAt);
DWORD WINAPI  AtcDisconnect(HAT *hAt);
DWORD WINAPI  AtcOpenQS(HAT *hAt,QS_INFO *qsConfig,UINT *qsn);
DWORD WINAPI  AtcCloseQS(HAT *hAt,UINT qsn);
DWORD WINAPI  AtcGetQSInfo(HAT *hAt,UINT qsn,QS_INFO *info);
DWORD WINAPI  AtcGetQSInfoEx(HAT *hAt,UINT qsn,DWORD dwInfoType,void *info,DWORD *lenOut);
DWORD WINAPI  AtcAdvise(HAT *hAt,DWORD qsdtid);
DWORD WINAPI  AtcUnadvise(HAT *hAt,DWORD qsdtid);
DWORD WINAPI  AtcGetNetAddr(HAT *hAt,DWORD type,PBYTE buffer,DWORD bufLen);
DWORD WINAPI  AtcQSAction(HAT *hAt,UINT qsn,DWORD dwAction,DWORD dwDataLen,void *data);

DWORD WINAPI  AtcAtAdvise(HAT *hAt,UINT qsn,DWORD dtdp);
DWORD WINAPI  AtcAtUnadvise(HAT *hAt,UINT qsn,DWORD dtdp);
DWORD WINAPI  AtcAtGroupAdvise(HAT *hAt,BOOL bAdvise,WORD nAdvises,UINT qsn,DWORD *adviseList);
DWORD WINAPI  AtcAtAdviseTest(HAT *hAt,UINT qsn,DWORD qsdtid);

/*--------------------- Virtual Server Functions -------------------------*/

DWORD WINAPI  AtcInitVQS(HAT *hAt,GREQUESTPROC lpfnFunc,
								 QS_INFO *qsConfig,UINT *qsn);

DWORD WINAPI  AtcRemoveVQS(HAT *hAt,UINT qsn);
DWORD WINAPI  AtcUpdateVQS(HAT *hAt,LPUPDATE update,UINT updateSize);


/*----------------------- Function declarations ------------------------*/

#if !defined(_QSDSKAPI_H_)
#include "qsdskapi.h"
#endif

#if !defined(_POPINFO_H_)
#include "popinfo.h"
#endif

/*----------------------- Price Block functions ------------------------*/

UINT WINAPI	GetPriceBlock(HAT *hAt,UINT qsn,LPSTR szSymbol,UINT dataType,
		  									LPVOID lpPB,DWORD blockSize);

#define	GPBE_PRIMARYGMC		0x00000001
#define	GPBE_NEXTBLOCK		0x00000002
#define	GPBE_PREVBLOCK		0x00000004
#define	GPBE_REFRESH		0x00000008
#define	GPBE_COMPOSITEGMC	0x00000010
#define	GPBE_CUSIPLOOKUP	0x00000020
#define	GPBE_REUTERS		0x00000040
#define	GPBE_REUTERSREFRESH	(GPBE_REUTERS | GPBE_REFRESH)
#define	GPBE_REUTERSPRIC	0x00000080
#define	GPBE_FUNDONLY		0x00000100
#define	GPBE_FUNDCOPY		0x00000200
#define	GPBE_EXACT_EXCH		0x00004000
#define	GPBE_LISTBOX		0x00008000

#define	GPBE_EXPANDEDRTN	0x00200000
#define	GPBE_EXPANDEDBLK	0x00400000
#define	GPBE_DATATYPE		0x00800000

typedef union _GPBE_FLAGS {
	DWORD  flags;
	struct {
		UINT  bPrimaryGMC	:1,		// Primary market.
			  bNextBlock	:1,		// Next price block.
			  bPrevBlock	:1,		// Previous price block.
			  bRefresh		:1,		// Refresh price block.
			  bCompositeGMC :1,		// Composite market.
			  bCUSIPLookup	:1,		// Do CUSIP lookup.
			  bReuters		:1,		// Reuters price block.
			  bReutersPric	:1,		// Reuters permanent RIC.
			  bFundOnly		:1,		// Fundamental data only.
			  bFundCopy		:1,		// Copy fundamental fields.
			  unused2		:4,
			  bExactExch	:1,		// Get exact exchange.
			  bListBox		:1,		// Use symbol search listbox.
			  unused3		:5,
			  bExpandedRtn	:1,		// Return Expanded Block
			  bExpandedBlk	:1,		// Get Expanded Block
			  bDataType		:1,		// Datatype supplied
			  datatype		:8;		// Datatype
	};
} GPBE_FLAGS;

UINT WINAPI	GetPriceBlockEx(HAT *hAt,UINT qsn,LPSTR szSymbol,UINT dataType,
				   				   				LPPB lpPB,DWORD dwFlags);

// Fundamental Data Type

#define	PFI_AMD_STOCK		0x01
#define	PFI_AMD_FUTURE		0x02
#define	PFI_FDR_STOCK		0x03
#define	PFI_PQ_SEC_PROFILE	0x04

// Bitmap definitions

#define	PFI_COMPANYNAME		0x0001
#define	PFI_DIVIDENDS		0x0002
#define	PFI_VOLATILITIES	0x0004
#define	PFI_OPTIONROOTS		0x0008

#define	PFI_SIC_CURRENCY	0x0010
#define	PFI_CUSIP			0x0020
#define	PFI_SHARES			0x0040
#define	PFI_LONGROOTS		0x0080

// Full record definitions

#define	PFI_FIXEDPAGE		0x8000
#define	PFI_EARNINGS		0x8001
#define	PFI_HISTORY			0x8002
#define	PFI_DIVIDEND		0x8003
#define	PFI_SPLIT			0x8004
#define	PFI_CALCULATED		0x8005
#define	PFI_INCOME			0x8006
#define	PFI_CHANGE			0x8007
#define	PFI_BALANCE			0x8008
#define	PFI_EQUITY			0x8009
#define	PFI_DESCRIPTION		0x800A
#define	PFI_POSTPANEL		0x800B
#define	PFI_ADDRESS			0x800C
#define	PFI_EQUITY2			0x800D
#define	PFI_DIVIDEND2		0x800E
#define	PFI_EARNINGS2		0x800F
#define	PFI_SEGMENTS		0x8010
#define	PFI_FINANCIALS		0x8011

int		WINAPI CopyFDFields(LPSTR lpFundData,DWORD wFundType,
							LPSTR lpBlockData,DWORD bmFields);

DWORD	WINAPI GetFDDataType(DWORD QST,LPPB lpPB);

DWORD	WINAPI GetPBInfo(HAT *hAt,DWORD qsn,BYTE reqType,LPSTR szSymbol,LPPB lpPB,
						LPDWORD wFundType,LPSTR lpData,DWORD bmFields,DWORD dwFlags);

//	Returns 0 if PB not found
//	else LOWORD == (len of lpPB), HIWORD == (len of lpData) 

/*----------------------------------------------------------------------*/

DWORD WINAPI AtGetPriceBlock(HAT *hAt,DWORD qsn,LPSTR szSymbol,LPPB lpPB,
			LPDWORD wFundType,LPSTR lpFundData,DWORD bmFields,DWORD dwFlags);

//	Returns 0 if PB not found
//	else LOWORD == (len of lpPB), HIWORD == (len of lpData) 

/*----------------------------------------------------------------------*/

// UpdatePriceBlock request flags.

#define	UPBF_TRENDLAST		0x00000001
#define	UPBF_TRENDBID		0x00000002
#define	UPBF_TRENDASK		0x00000004
#define	UPBF_TRENDALL		0x00000007

#define	UPBF_FORCELAST		0x00010000
#define	UPBF_FORCEQUOTE		0x00020000
#define	UPBF_FORCEALL		0x00030000

// UpdatePriceBlock returns flags.

typedef union _UPB_FLAGS
{
	DWORD  flags;
	struct
		{
		UINT  bUpdateApplied	:1,
			  bNewLast			:1,
			  nTrendLast		:2,
			  bNewBid			:1,
			  nTrendBid			:2,
			  bNewAsk			:1,
			  nTrendAsk			:2,
			  unused			:22;
		};
} UPB_FLAGS;

UINT WINAPI 	UpdatePriceBlock(UPDATE_PAK *pUpdate,LPPB pb,DWORD dwFlags);

#define	CPBU_AUTOUPDATE		0x80000000	// Call UpdatePriceBlock on match.

UINT WINAPI 	CheckPriceBlockUpdate(UPDATE *update,LPPB pb,DWORD qsn,DWORD dwFlags);

/*----------------------------------------------------------------------*/

/*
**** ATFI Get Bunched Price Blocks functions
*/

#define MAX_BUNCHREQ		25

// lpPBFunc(LPPB lpPB,LPARAM lP);

typedef (CALLBACK* GBLOCKENUMPBPROC)(LPPB,LPARAM);

typedef	struct
		{
		ATREQ_KEY			key;
		LPARAM				lP;
		}
		BUNCH_REQINFO, *LPBUNCH_REQINFO;

typedef	struct
		{
		UINT				nRequests;
		BUNCH_REQINFO		reqInfo[MAX_BUNCHREQ];
		}
		BUNCH_REQUESTS, *LPBUNCH_REQUESTS;

typedef	struct
		{
	   	GBLOCKENUMPBPROC	lpfnProc;
		BUNCH_REQUESTS		requests[MAX_QSNUM];
		}
		BUNCH_SYMS, *LPBUNCH_SYMS;

BOOL WINAPI	AddBunchPriceBlockSymbols(HAT *hAt,LPBUNCH_SYMS lpBunchSymbols,
							UINT qsn,UINT datatype,LPSTR szSymbol,LPARAM lP);

BOOL WINAPI	FlushBunchPriceBlockSymbols(HAT *hAt,LPBUNCH_SYMS lpBunchSymbols);

/*----------------------------------------------------------------------*/

/*
**** ATFI Load Deferred Price Blocks functions
*/

#define MAX_LOADDEFERRED	32

typedef	struct
		{
		LPPB				lpPB;
		LPARAM				lP;
		}
		LOAD_DEFERPB, *LPLOAD_DEFERPB;

typedef	struct
		{
	   	GBLOCKENUMPBPROC	lpfnProc;
		BOOL				bInhibitAdvise;	// Don't Auto advice on PB's
		BOOL				bStkOptions;
		UINT				nRequests;
		LOAD_DEFERPB		reqInfo[MAX_LOADDEFERRED];
		}
		LOAD_DEFERPBS, *LPLOAD_DEFERPBS;

BOOL WINAPI	LoadDeferredPriceBlocks(HAT *hAt,UINT qsn,
							LPLOAD_DEFERPBS lpDeferPBs,LPPB lpPB,LPARAM lP);

BOOL WINAPI	FlushDeferredPriceBlocks(HAT *hAt,UINT qsn,LPLOAD_DEFERPBS lpDeferPBs);

/*----------------------------------------------------------------------*/

/*
**** ATFI Enumeration functions
*/

// Enumerate Price Block Symbols

typedef struct _ENUM_DATATYPE
{
	union
	{
		UINT	dwDatatype;
		struct
		{
			UINT	datatype: 8;		// lower 8 bits is the data type
			UINT	reserved: 22;	 	// Unused
			UINT	bInhibitAdvise:  1;	// Don't Auto advice on PB's
			UINT	bContinueSearch: 1;	// Use last symbol search length.
		};
	};
} ENUM_DATATYPE;

// lpSymFunc(LPBYTE sym,int symLen,BYTE datatype,WORD dataptr,LPARAM lP);

typedef (CALLBACK* GBLOCKENUMPROC)(LPBYTE,int,BYTE,WORD,LPARAM);

int WINAPI	EnumSymbols(HAT *hAt,UINT qst,UINT qsn,UINT enumDatatype,
		   				LPSTR szSymbol,GBLOCKENUMPROC lpfnFunc,LPARAM lP);

// Enumerate AT Price Block Symbols

// lpSymFunc(LPBYTE sym,int symLen,DWORD dtdp,LPARAM lP);

typedef (CALLBACK* GBLOCKATENUMPROC)(LPBYTE,int,DWORD,LPARAM);

int WINAPI	AtEnumSymbols(HAT *hAt,UINT qsn,UINT enumDatatype,
		   				LPSTR szSymbol,GBLOCKATENUMPROC lpfnFunc,LPARAM lP);

// Enumerate Price Blocks

// lpPBFunc(LPPB lpPB,LPARAM lP);

int WINAPI	EnumPriceBlocks(HAT *hAt,UINT qsn,UINT enumDatatype,
		   				LPSTR szSymbol,GBLOCKENUMPBPROC lpfnFunc,LPARAM lP);

// Enumerate Market Marker Price Blocks

// lpPBFunc(LPSPQB lpSpqb,LPARAM lP);

typedef (CALLBACK* GMMBLOCKENUMPROC)(LPSPQB,LPARAM);

int	WINAPI 	EnumMMBlocks(HAT *hAt,UINT qsn,LPPB lpPB,
										GMMBLOCKENUMPROC lpfn,LPARAM lP);

// Enumerate AT Market Marker Price Blocks

// lpPBFunc(LPPB lpPB,LPARAM lP);

int	WINAPI 	AtEnumMMBlocks(HAT *hAt,UINT qsn,LPPB lpPB,
		   							GBLOCKENUMPBPROC lpfnFunc,LPARAM lP);

/*----------------------------------------------------------------------*/

#define	GBC_TKSPSTOCK		0x00000001
#define	GBC_PQSTOCK			0x00000002
#define	GBC_ATSTOCK			0x00000004
#define	GBC_NASDAQ			0x00000008

#define	GBC_USSTOCK			0x00000010
#define	GBC_NUSTOCK			0x00000020
#define	GBC_ANYSTOCK		(GBC_USSTOCK | GBC_NUSTOCK)

#define	GBC_BONDS			0x00000100
#define	GBC_MUTUAL			0x00000200
#define	GBC_MMFUND			0x00000400

#define	GBC_FUTURE			0x00001000
#define	GBC_INDEX			0x00002000

#define	GBC_USFUTOPT		0x00010000
#define	GBC_NUFUTOPT		0x00020000
#define	GBC_ANYFUTOPT		(GBC_USFUTOPT | GBC_NUFUTOPT)

#define	GBC_USSTKOPT		0x00100000
#define	GBC_NUSTKOPT		0x00200000
#define	GBC_ANYSTKOPT		(GBC_USSTKOPT | GBC_NUSTKOPT)

#define GBC_EXTENDED		0x01000000
#define	GBC_COMPOSITE		0x02000000
#define	GBC_DELAYED			0x04000000

#define	GBC_HAS_FAULT		0x08000000

#define	GBC_HAS_SYMBOL		0x10000000
#define	GBC_HAS_CCE			0x20000000
#define	GBC_HAS_DTPTR		0x40000000
#define	GBC_HAS_TOSPTR		0x80000000

typedef union _GPBC_FLAGS
{
	DWORD  flags;
	struct {
		UINT  bTKSPStock	:1,
			  bPQStock		:1,
			  bATStock		:1,
			  bNASDAQ		:1,
			  bUSStock		:1,
			  bNUStock		:1,
			  Unused1		:2,
			  bBonds		:1,
			  bMutualFund	:1,
			  bMMFund		:1,
			  Unused2		:1,
			  bFuture		:1,
			  bIndex		:1,
			  Unused3		:2,
			  bUSFutOpt		:1,
			  bNUFutOpt		:1,
			  Unused4		:2,
			  bUSStkOpt		:1,
			  bNUStkOpt		:1,
			  Unused5		:2,
			  bExtended		:1,
			  bComposite	:1,
			  bDelayed		:1,
			  bHasFault		:1,
			  bHasSymbol	:1,
			  bHasCCE		:1,
			  bHasDtPtr		:1,
			  bHasTOSPtr	:1;
	};
} GPBC_FLAGS;

DWORD WINAPI 	GetPriceBlockClass(LPPB lpPB);

DWORD WINAPI 	GetPriceBlockDtdp(LPPB lpPB);
DWORD WINAPI 	GetPriceBlockDatatype(LPPB lpPB);
DWORD WINAPI 	GetPriceBlockDataptr(LPPB lpPB);

DWORD WINAPI	TranslateDelayedDatatype(DWORD dataType);

DWORD WINAPI 	GetPBUpdateQSN(UPDATE *update);
DWORD WINAPI 	GetPBUpdateDtdp(UPDATE *update);
DWORD WINAPI 	GetPBUpdateDatatype(UPDATE *update);
DWORD WINAPI 	GetPBUpdateDataptr(UPDATE *update);

#define	GBF_RESTRICTION		0x0000001F
#define	GBF_NOTES			0x00000020
#define	GBF_RTYPE			0x000000C0

#define	GBF_NEWS			0x00000100
#define	GBF_UPC_RESTRICT	0x00004000
#define	GBF_HALTED			0x00008000

#define	GBF_OPEN			0x00010000
#define	GBF_SPLIT			0x00020000
#define	GBF_XDIV			0x00040000
#define	GBF_BOND_CALLED		0x00080000

#define	GBF_TRENDLAST		0x00300000
#define	GBF_TRENDBID		0x00C00000

#define	GBF_ALERT_L			0x01000000
#define	GBF_ALERT_H			0x02000000
#define	GBF_ALERT_p			0x04000000
#define	GBF_ALERT_P			0x08000000
#define	GBF_ALERT_V			0x10000000

#define	GBF_RST_NOTES		(GBF_RESTRICTION | GBF_NOTES)
#define	GBF_SPLIT_XDIV		(GBF_SPLIT | GBF_XDIV)
#define	GBF_SPLIT_XDIV_UPC	(GBF_SPLIT | GBF_XDIV | GBF_UPC_RESTRICT)
#define	GBF_ALERTS			(GBF_ALERT_L | GBF_ALERT_H | GBF_ALERT_p | GBF_ALERT_P |  GBF_ALERT_V)

typedef union _GPBF_FLAGS {
	DWORD  flags;
	struct {
		UINT  nRestriction	:5,
			  bNotes		:1,
			  nRType		:2,
			  bNews			:1,
			  unused1		:5,
			  bUPCRestrict	:1,
			  bHalted		:1,
			  bOpen			:1,
			  bSplit		:1,
			  bXDiv			:1,
			  bBondCalled	:1,
			  nTrendLast	:2,
			  nTrendBid		:2,
			  bAlert_L		:1,
			  bAlert_H		:1,
			  bAlert_p		:1,
			  bAlert_P		:1,
			  bAlert_V		:1,
			  unused2		:3;
	};
} GPBF_FLAGS;

DWORD WINAPI 	GetPriceBlockFlags(LPPB lpPB,DWORD dwFlags);

INT WINAPI 		FormatEntrySymbol(UINT qst,LPPB pb,LPSTR lpSymbol);

UINT WINAPI 	GetChartData(HAT *hAt,UINT iQSN,DWORD iQST,LPSTR szSymbol,
								 char exchange,LPVOID lpData,LPDWORD dwID);

UINT WINAPI 	GetChartDirectory(HAT *hAt,UINT iQSN,DWORD iQST,char Exchange,
										  LPVOID lpDirectory);

BOOL WINAPI 	GetChartName(UINT iQSN,DWORD iQST,DWORD dwID,LPSTR lpSymbol);

BOOL WINAPI 	SaveChartData(HAT *hAt,UINT iQSN,DWORD iQST,DWORD dwID,LPVOID lpData);

DWORD WINAPI 	GetHistoryFile(HAT *hAt,UINT iQSN,WORD year,
								LPPB lpPB,HFDR *lpHFDR,DWORD fFilter);

BOOL WINAPI 	GetHFInfo(UINT hft,LPSTR lpszFilename,LPWORD lpwSymSize,
							LPDWORD lpdwDirSize,BOOL *bDirHasCount,
							BOOL *bCanCode,BOOL *bClassEncoded);

#define	HFT_NYSE		0
#define	HFT_AMEX		1
#define	HFT_NMS			2
#define	HFT_FUTURE		3
#define	HFT_LONDON		4
#define	HFT_SEAQ		5
#define	HFT_FRANKF		6
#define	HFT_PARIS		7
#define	HFT_AMSTER		8
#define	HFT_CASH		9
#define	HFT_HONGKONG	10
#define	HFT_EUROFUT		11
#define	HFT_ZURICH		12
#define	HFT_RSYN		13
#define	HFT_RIC			14
#define	HFT_CCE			15	/* history.cce */
#define	HFT_CCE_ARC		16 /* this history.cce is history */
#define	HFT_NYSE_ARC	17 /* this history.cce is history */
#define	HFT_AMEX_ARC	18 /* this history.cce is history */
#define	HFT_NASD_ARC	19 /* this history.cce is history */
#define	HFT_INDEX_ARC	20 /* this history.cce is history */
#define HFT_NYSE_FN		21 /* fundamental history */
#define HFT_AMEX_FN		22 /* fundamental history */
#define HFT_NASD_FN		23 /* fundamental history */
#define	HFT_MONTHLY_CMP	24 /* Deep Compressed Monthly */
#define	HFT_WEEKLY1_CMP	25 /* Deep Compressed Weekly */
#define	HFT_WEEKLY2_CMP	26 /* Deep Compressed Weekly */
#define	HFT_WEEKLY3_CMP	27 /* Deep Compressed Weekly */
#define	HFT_MAXVALUE	27 /*!!! Must equal to last member of HFT_list !!!*/

#define	HFF_ZERO_CLOSE	0x0001	/* Remove days that have ZERO close */
#define	HFF_ZERO_VOLUME	0x0002	/* Remove days that have ZERO volume */
#define	HFF_RJUST		0x0004	/* Fill buffer from end towards beginning */
#define	HFF_HOLIDAYS	0x0008	/* Remove holiday days */
#define	HFF_FIXUP		0x0010	/* Fixup fractions,zero Hi/Lo/Cl..etc. */
#define	HFF_NO_CMP_READ	0x0020  /* Do not use compression, need OPEN data */
#define  HFF_CMPDAILY2	0x0100	/* Which Chunk of Shallow Daily to start with (second K)   */
#define  HFF_CMPDAILY3	0x0200	/* Which Chunk of Shallow Daily to start with (third K)    */
#define  HFF_CMPDAILY4	0x0300	/* Which Chunk of Shallow Daily to start with (last chunk) */
#define	HFF_CMPWEEK1	0x0400	/* Compressed Weekly (first 4 years) */
#define	HFF_CMPWEEK2	0x0800	/* Compressed Weekly (middle 4 years) */
#define	HFF_CMPWEEK3	0x1000	/* Compressed Weekly (last 4 years) */
#define	HFF_CMPMONTH	0x2000	/* Compressed Monthly */
#define	HFF_FUNDDATA	0x4000	/* Return Fundamental Data */
#define	HFF_RAWDATA		0x8000	/* Don''t filter or convert data at all */

#define	HFE_HFT_INVALID	1		/* History file type is out of range */
#define	HFE_BAD_FN		2		/* History filename is bad (not exist) */
#define	HFE_BAD_DIR		3		/* History directory is bad (short) */
#define HFE_SYMBOL_NF	4		/* Symbol not found */
#define HFE_BAD_DATA	5		/* History data is bad (short) */

///////////////////////////////////////////////////////////////////////////

int	WINAPI 		GetLatestStory(LPSTR lpText);
BOOL WINAPI 	GetStory(int index,LPSTR lpText);
BOOL WINAPI 	GetHeadlines(LPSTR lpText,LPINT lpLatest);
BOOL WINAPI 	GetNewsAlerts(LPSTR lpBuffer);

BOOL WINAPI 	LookupFundamentalData(HAT *hAt,UINT iQSN,DWORD iQST,
									  LPSTR szKey,int keyType,
									  LPVOID lpFundData,int nFundSize);

int WINAPI 		GetFundamentalData(HAT *hAt,UINT iQSN,DWORD iQST,
										   LONG lPtr,int keyType,
										   LPVOID lpFundData,int nFundSize);

int WINAPI 		GetFundamentalKeys(HAT *hAt,UINT iQSN,DWORD iQST,
			   					   LPSTR szKey,int keyType,
			   					   BOOL bWild,LPSTR lpKeys,
			   					   LPLONG lpPtrs,int maxKeys,
			   					   LPINT lpKeySize);

int WINAPI 		LookupFundName(LPSTR lpFundKey,LPSTR lpFundName);

int WINAPI 		GetInstrumentName(HAT *hAt,UINT QSN,LPPB lpPB,LPSTR lpName,DWORD dwFlags);

#define	GIN_MAXLEN	48
#define	GIN_SYMBOL	1
#define	GIN_PBTYPE	2

		// PC Quote DBAX Access functions

int WINAPI		DBAX(HAT *hAt,UINT iQSN,LPSTR pblk,int function,void *key,void *buffer,int dbn);
int WINAPI		DBAXAttach(HAT *hAt,UINT qsn,LPSTR pblk,int dbn);
int WINAPI		DBAXDetach(HAT *hAt,UINT qsn,LPSTR pblk);
int WINAPI		DBAXGetEqual(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetApprox(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetFirst(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetLast(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetNext(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetHigher(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);
int WINAPI		DBAXGetNextSub(HAT *hAt,UINT qsn,LPSTR pblk,LPSTR key,LPSTR data,int keyPath);

		// GetATOptionInfo() dwFlags.

#define	GOPT_RETRIEVE_ALL_DATA	1

/*
**** GetATOptionInfo() function prototype, return number of Option Roots.
*/
int WINAPI	 	GetATOptionInfo(HAT *hAt,UINT iQSN,LPPB pbPtr,
						LPAT_OPTION lpATOptions,int maxRoots,DWORD dwFlags);

/*
**** GetATFIOptionInfo() function prototype, return number of Option Roots.
*/
int WINAPI	 	GetATFIOptionInfo(HAT *hAt,UINT iQSN,LPPB pbPtr,
						LPATFI_OPTION lpATOptions,int maxRoots,DWORD dwFlags);

/*
**** GetFDRecord() function prototype, flags and return status defs'
**** For ATFI equity subrecords set dwFlags to PFI_ values defined above. 
*/
int	WINAPI 		GetFDRecord(HAT *hAt,UINT iQSN,LPPB pbPtr,DWORD dwFlags,
									PUINT lpType,LPSTR lpData);

	// Status returned in lpType if return size is ZERO
	// File data types for lpType defined in ATFD.H

#define	GFR_INVALID_QSN			1
#define	GFR_INVALID_QST			2
#define	GFR_INVALID_DATATYPE	3
#define	GFR_FILE_NOT_FOUND		4
#define	GFR_RECORD_NOT_FOUND	5

/*
**** ATFI Common functions
*/

int WINAPI		GetATFIProfileString(HANDLE hAppInst,DWORD idsString,
												LPSTR szString,int nSize);

BOOL WINAPI		WriteATFIProfileString(HANDLE hAppInst,DWORD idsString,
												LPSTR szString);

DWORD WINAPI	ATGetPrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName,
								LPCTSTR lpDefault, LPTSTR lpReturnedString,
								DWORD  nSize, LPCTSTR  lpFileName);

BOOL WINAPI		ATWritePrivateProfileString( LPCTSTR lpAppName, LPCTSTR lpKeyName, 
								LPCTSTR lpString, LPCTSTR lpFileName);

UINT WINAPI		ATGetPrivateProfileInt( LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, 
								LPCTSTR lpFileName );

ULONG WINAPI	nlongint(LPVOID lpData,UINT nBytes);
LONG WINAPI		longint(LPVOID lpData,UINT nBytes);
ULONG WINAPI	longdword(LPVOID lpData,UINT nBytes);

DWORD WINAPI	AsciiToCan(LPSTR lpString);
void WINAPI		CanToAscii(DWORD dwCan,LPSTR lpString);

/*-------------------------------------------------------------------------*/

/*
**** Special types and values used by ATFI
*/

#define	WM_AT_INITIATE		(WM_USER + 0x70)
#define	WM_AT_TERMINATE		(WM_USER + 0x71)
#define	WM_AT_REQUEST		(WM_USER + 0x72)
#define	WM_AT_ADVISE		(WM_USER + 0x73)
#define	WM_AT_DATA			(WM_USER + 0x74)
#define	WM_AT_UNADVISE		(WM_USER + 0x75)
#define	WM_AT_STATUS		(WM_USER + 0x76)
#define	WM_AT_RECON			(WM_USER + 0x77)
#define	WM_AT_BRIDGE		(WM_USER + 0x78)
#define	WM_AT_ADVISE_X		(WM_USER + 0x79)
#define	WM_AT_EXEC			(WM_USER + 0x7a)
#define	WM_AT_WNDPOSCHANGE  (WM_USER + 0x7b)

#define WM_AT_TARGET		(WM_USER + 0x7e)
#define WM_AT_DROP			(WM_USER + 0x7f)
#define WM_AT_REPORT		(WM_USER + 0x80)
#define WM_AT_SYSPARMCHG	(WM_USER + 0x81)
#define WM_AT_ACCELERATOR	(WM_USER + 0x82)

typedef	struct
		{
		BYTE	QSN;				// Quote Server number
		WORD	length;				// Length  of data to/from QS
		char	data[576];			// Content of data to/from QS
		}
		REQUEST;

		// AtFormat() fields

#define	AT_SHORTSYM		 			1
#define	AT_YSETTLE					2
#define	AT_CLOSE					2
#define	AT_LAST						3
#define	AT_NETASSETVAL				3
#define	AT_7DAYYIELD				3
#define	AT_HIGH						4
#define AT_LOW						5
#define	AT_BID						6
#define	AT_AVGMATURITY				6
#define	AT_ASK						7
#define	AT_OFFERPRICE				7
#define	AT_OPEN						8
#define	AT_BIDSIZE					9

#define	AT_ASKSIZE					10
#define	AT_LASTSIZE					11
#define	AT_VOLUME					12
#define	AT_NETASSETS				12
#define	AT_TIMELAST					13
#define	AT_TIMEQUOTE				14
#define	AT_CONDITION				15
#define	AT_STATUS					16
#define	AT_GMCLSOP					17
#define	AT_GMCHILO					18
#define	AT_GMCBIDASK				19

#define	AT_LONGSYM					20
#define	AT_RANK						21
#define	AT_NCOMSHARES				22
#define	AT_NPREFSHARES				23
#define	AT_HALFYRVOL				24
#define	AT_VOLATILE20				25
#define	AT_IMPLDCALLVOL				26
#define	AT_IMPLDPUTVOL				27
#define	AT_PINTRMEPS				28
#define	AT_CINTRMEPS				29

#define	AT_EARNINGS					30
#define	AT_EARNINGS12MO				30
#define	AT_ESTEARNINGS				31
#define	AT_PREVXDIVDATE				32
#define	AT_PREVXDIVAMOUNT			33
#define	AT_FREQUENCYCODE			34
#define	AT_XDIVINDICATOR			35
#define	AT_52WEEKHIGH				36
#define	AT_52WEEKLOW				37
#define	AT_LASTYEAREPS				38
#define	AT_LASTSTKDIV				39

#define	AT_STKDIVXDATE				40
#define	AT_DIVYIELD					41
#define	AT_YIELD					41
#define	AT_PERATIO					42
#define AT_VALUATION       			43
#define	AT_XDIVDATE1				44
#define	AT_XDIVAMOUNT1				45
#define	AT_DIVAMOUNT				45
#define	AT_XDIVDATE2				46
#define	AT_XDIVDATE3				47
#define	AT_STOCKFLAG				48
#define AT_AT_STKDIVIND				49
#define AT_DIVTYPE					49

#define	AT_DIVPAYDATE				50
#define	AT_UNDLPRICE				50
#define	AT_STRIKE					51
#define	AT_EXPIREDATE				52
#define	AT_OPENINTEREST				53
#define	AT_CONTRACTHIGH				54
#define	AT_CONTRACTLOW				55
#define	AT_UNDLSYMBOL				56
#define	AT_UNDLEXCHG				57

#define	AT_AUCTIONDATE				59
#define	AT_MATURITYDATE				60
#define	AT_BONDRATE					61
#define	AT_BONDFORM					62
#define	AT_TRADEUNIT				63
#define	AT_CALLDATE					64

#define	AT_OPEN1					70
#define	AT_OPEN2					71
#define	AT_CLOSE1					72
#define	AT_CLOSE2					73
#define	AT_DECIMAL					74
#define	AT_TRBATE					75
#define	AT_TICKCOUNT				76
#define	AT_TIMETRADE				77

#define	AT_SPLITDIV					78
#define	AT_UNITPRICESIZE			78
#define AT_COUPONRATE				79
#define	AT_UNITMEASURE				79

#define	AT_COUPONDATE				80
#define	AT_CONTRACTSIZE				80
#define	AT_RATING					81
#define	AT_POSITIONLIMIT			81
#define	AT_ISSUEDATE				82
#define	AT_LIFEHIGHDATE				82
#define	AT_LIFELOWDATE				83
#define	AT_RATINGID					84
#define	AT_VOL6MO					84
#define AT_ORIGISSUEPRICE			85
#define	AT_PRICEREPCODE				85
#define	AT_MMID						86	/* Old RSF */
#define	AT_MAXPRICEFLUX				86
#define	AT_BANK						86	/* FOREX */

#define	AT_MMSOURCE					87	/* Old RSF */
#define	AT_BACKPAGE					87	/* New RSF */
#define	AT_MINPRICEFLUX				87

#define	AT_MMNAME					88	/* Old RSF */
#define	AT_CONDEAL					88	/* New RSF */
#define	AT_TIMEOPEN					88

#define	AT_TIMECLOSE				89
#define	AT_CAPITALIZATION			89

#define	AT_NEWSPAGE					90
#define	AT_LASTTRADEDAY				90

#define	AT_NOTICEDATE				91
#define	AT_DELIVERYDATE				92
#define	AT_OPTIONMONTH				93
#define	AT_COUNTRY					94

#define	AT_OFFICIALCODE				96

#define	AT_OFFICIALCODEID			97
#define	AT_TRID						97

#define	AT_LASTTRADEDATE			98
#define	AT_TRADECONDITION			99

#define	AT_PUTCALL					100
#define AT_LASTRADATE				101
#define	AT_TRENDFLAG				102
#define	AT_CURRENCY					103

#define	AT_YCLOSE					104
#define	AT_1WEEKBID					105
#define	AT_1WEEKASK					106
#define	AT_2WEEKBID					107
#define	AT_2WEEKASK					108
#define	AT_1MONTHBID				109
#define	AT_1MONTHASK				110
#define	AT_2MONTHBID				111
#define	AT_2MONTHASK				112
#define	AT_3MONTHBID				113
#define	AT_3MONTHASK				114
#define	AT_6MONTHBID				115
#define	AT_6MONTHASK				116
#define	AT_1YEARBID					117
#define	AT_1YEARASK					118
#define	AT_ALERTS					119

#define	AT_SOURCEPAGE				120
#define	AT_REGION					120

#define	AT_EXCHANGE					121

#define	AT_14DAYYIELD				122
#define	AT_CITY						122

#define	AT_30DAYYIELD				123
#define	AT_LASTYIELD				123
#define	AT_CONTRIBUTOR				123

#define	AT_90DAYYIELD				124
#define	AT_BIDYIELD					124

#define	AT_120DAYYIELD				125
#define	AT_ASKYIELD					125

#define	AT_180DAYYIELD				126
#define	AT_INDPRICE					126

#define	AT_1YEARYIELD				127
#define	AT_INDYIELD					127

#define AT_7DAYANNUALYIELD			128

#define	AT_DIVIDEND					129
#define	AT_STOCKSPLIT				130
#define	AT_CAPITALGAINS				131
#define	AT_TOTALASSETS				132
#define	AT_CATEGORY					133

#define	AT_LREGION					134
#define	AT_LCITY					135
#define	AT_LCONTRIBUTOR				136

#define AT_MIDPRICE					137
#define AT_MIDPRICECLOSE			138

#define	AT_TIMEBID					140
#define	AT_TIMEASK					141
#define	AT_CUSIPID					142
#define	AT_ALIASCODE				143
#define	AT_ACTIONCODE				144
#define	AT_INSTRTYPE				145
#define	AT_HITPRICE					146
#define	AT_HITYIELD					147
#define	AT_HITVOLUME				148
#define	AT_TAKEPRICE				149
#define	AT_TAKEYIELD				150
#define	AT_TAKEVOLUME				151

#define AT_INDUCODE1				152
#define AT_INDUCODE2				153
#define AT_FISCALYEAR				154
#define AT_YREARNINGS				155
#define AT_YREARNINGSFLAG			156
#define AT_Q1EARNINGS				157
#define AT_Q1EARNINGSFLAG			158
#define AT_Q2EARNINGS				159
#define AT_Q2EARNINGSFLAG			160
#define AT_Q3EARNINGS				161
#define AT_Q3EARNINGSFLAG			162
#define AT_Q4EARNINGS				163
#define AT_Q4EARNINGSFLAG			164
#define AT_EARNINGSDATE				165
#define AT_EARNINGSTIME				166
#define AT_PREVYREARNINGS			167
#define AT_PREVQ1EARNINGS			168
#define AT_PREVQ2EARNINGS			169
#define AT_PREVQ3EARNINGS			170
#define AT_PREVQ4EARNINGS			171
#define AT_PREVDATE					172
#define AT_NUMBROKERS				173
#define AT_YRSTDDEV					174
#define AT_Q1STDDEV					175
#define AT_Q2STDDEV					176
#define AT_Q3STDDEV					177
#define AT_Q4STDDEV					178
#define AT_BROKERID					179

#define AT_STORYINFO				180
#define AT_RGNLCLOSE				180
#define	AT_TOTAL_PL		 			181

#define	AT_LIFEHIGH					182
#define	AT_LIFELOW					183
#define	AT_LOWLIMIT					184
#define	AT_HIGHLIMIT				185

#define	AT_DISPLAYDECIMAL			188
#define	AT_STRIKEDECIMAL			189
#define	AT_YESTDECIMAL				190
#define	AT_YESTBID					191
#define	AT_YESTASK					192
#define	AT_YESTVOLUME				193

		// Calculated fields...

#define	AT_NETCHANGE				200
#define	AT_PCTCHANGE				201
#define AT_BIDASK					202
#define	AT_SPACES					203
#define	AT_ACHAR					204
#define AT_BIDASKSIZE				205
#define	AT_NETCHGOPEN				206
#define	AT_PCTCHGOPEN				207

		// AtFormat special formatting flags

#define ATF_FIELDMASK				0x0000FFFF	// Field mask.

#define ATF_SHOWSYMBOL				0x00010000	// Show symbol. 
#define ATF_SHOWCOUNTRY				0x00020000	// Show country.
#define ATF_SHOWEXCHANGE			0x00040000	// Show exchange.
#define ATF_HIDECLOSEBID			0x00080000	// Hide closing bid character.

#define ATF_HIDEDELAYED				0x00100000	// Hide delayed quote character.
#define ATF_HIDEINDEX				0x00200000	// Hide display index character.

#define ATF_SHOWALL					0x00070000	// Show symbol,country and exchange.
#define ATF_HIDEALL					0x00380000	// Hide all special characters.

		// ATReal special formatting flags

#define ATF_LONGVALUE				0x01000000	// Return long value.

typedef union _ATF_FLAGS {
	DWORD  value;
	struct {
		UINT  field			:16,
			  bShowSymbol	:1,
			  bShowCountry	:1,
			  bShowExchange	:1,
			  bHideCloseBid	:1,
			  bHideDelayed	:1,
			  bHideIndex	:1,
			  reserved1		:2,
			  bLongValue	:1,
			  reserved2		:7;
	};
} ATF_FLAGS;

/*************************************************************************************/

BOOL WINAPI		ATTranslateSymbol(HAT *hAt,DWORD qsn,LPSTR szSymbol,int *dataType);

BOOL WINAPI		ATParseSymbol(LPSTR szSymbol, int dataType);
void WINAPI		ChangeDecimalMode(BOOL mode);

int WINAPI		AtFormat(char *szOutput,PB *pPB,void *altData,UINT field,UINT length);

void WINAPI		AtFormatValue(LPSTR string,long value,int decType,int displayDecType);

void WINAPI		ATFormatValue(LPSTR string,long value,BYTE dcode);

void WINAPI 	ATFormatValueField(LPSTR string,long value,BYTE dcode,int strlength);

/*************************************************************************************/

#define ATFKEY_ALL					0x0000	// Show all.
#define ATFKEY_SYMBOL				0x0001	// Show symbol. 
#define ATFKEY_COUNTRY				0x0002	// Include country.
#define ATFKEY_EXCHANGE				0x0004	// Include exchange.

int  WINAPI		ATFormatATKey(LPSTR szSymbol,LPAT_KEY lpKey,DWORD dwFormat);

int  WINAPI		ATFormatATPrice(LPSTR szPrice,AT_PRICE atPrice,
													int decType,int displayDecType);

#define ATFDATE_SHOWYEAR			0x0001	// Show four digit year. 
#define ATFDATE_SHOWSECONDS			0x0002	// Show seconds. 
#define ATFDATE_HIDEDAY				0x0004	// No day. 

int  WINAPI		ATFormatATDateTime(LPSTR szDateTime,AT_DATETIME atDateTime,DWORD dwFormat);

/*************************************************************************************/

char	WINAPI TrendFlag2Char(int flag);

int 	WINAPI CommaFormat(char *sz,DWORD value);

double 	WINAPI ATRealPrice(PB *pbPtr,UINT field,int bAllowSubstitutes);

void  	WINAPI CrunchSymbol(LPSTR lpszSymbol);

void  	WINAPI DrawSymbol(HDC hDC,LPPB pbPtr,int x,int y,
										int width,int height,DWORD dwSpecial);

void  	WINAPI DrawSymbolEx(HDC hDC,LPPB pbPtr,LPRECT lpRect,DWORD dwSpecial);

		// Caller supplies symbol pre-crunched.
 
void 	WINAPI ATDrawSymbol(HDC hDC,LPPB pbPtr,LPSTR szSymbol,
												LPRECT lpRect,DWORD dwSpecial);

		// Caller supplies symbol pre-crunched and
		// flags as returned by GetPriceBlockFlags.

void 	WINAPI ATDrawFancySymbol(HDC hDC,DWORD dwFlags,LPSTR szSymbol,
												LPRECT lpRect,DWORD dwSpecial);

		// Values for DrawSymbol(), DrawSymbolEx(),
		// ATDrawSymbol() and ATDrawFancySymbol() dwSpecial.

#define	DSF_NEWSUNDERLINE	0x00000001

		// Values for DrawSymbol() and DrawSymbolEx() dwSpecial

#define DSF_SHOWCCE			0x00000002	/* Include country code and exchange */

		// Price Block predefined headers. */

#define HDR_MUTUALFUND		0x0001
#define HDR_MONEYMARKET		0x0002

LPSTR	WINAPI	ATHeaderPB(LPSTR szString,
						UINT datatype, int fieldId, int width,int special);

		// Values for Currency Conversion

typedef	struct
		{
		short	nAltDataType;		// Type 0 = CVT_PRICE
		BOOL	bConvertPrice;
		char	szBaseCurrency[4];
		double	fToUSD;
		BOOL	bMultUSD;
		char	szDispCurrency[4];
		double	fToXXX;
		BOOL	bMultXXX;
		}
		CVT_PRICE;

#define	CVT_GETCURRENCY		0x0001
#define	CVT_SETCURRENCY		0x0002
#define	CVT_RESETCURRENCY	0x0003

		// Values for DrawSymbol() wFlags

long WINAPI		ATParseValue(LPSTR string,BYTE dcode);

AT_DATETIME WINAPI	ATParseDate(LPSTR string);
AT_DATETIME WINAPI	ATParseTime(LPSTR string);

INT WINAPI		GetFundFieldName(int fundType,int fField,
									LPSTR lpString,int widthFlags);

INT WINAPI		FormatFundData(LPVOID lpFundData,DWORD fundType,
								int fField,LPSTR lpString,int widthFlags);

UINT WINAPI		ListQuoteServers(HAT *hAt,QS_INFO *qsList);

void WINAPI 	DrawBitmap(HDC,int xStart,int yStart,int wide,int tall,HBITMAP,DWORD);
int  WINAPI 	PaintSmallCaption(HWND hWnd,HDC hDC,DWORD dwFlags);

LPSTR WINAPI 	StringResize(LPSTR lpString,int size);
LPSTR WINAPI 	rtrim(LPSTR);
LPSTR WINAPI 	ljust(LPSTR);
LPSTR WINAPI 	rjust(LPSTR,int);
BOOL  WINAPI 	IsMixedCase(LPSTR);

/*************************************************************************************/

UINT	WINAPI	GetQSVersion(HAT *hAt,UINT QSN);
int		WINAPI	GetDataTypeName(int dataType,LPSTR szString,int nBufferMax);
int		WINAPI	GetQSTypeName(DWORD iQST,LPSTR szString,int nBufferMax);
int		WINAPI	GetQSTypeSN(DWORD iQST,LPSTR szString);
DWORD	WINAPI 	QSCtoQST(CHAR qsc);
CHAR	WINAPI 	QSTtoQSC(DWORD iQST);
BOOL	WINAPI	GetQSUserID(HAT *hAt,UINT QSN,LPSTR szUserID);
BOOL	WINAPI	GetQSExchPermission(HAT *hAt,UINT QSN,int nExchBit);
int		WINAPI	GetQSUserExt(HAT *hAt,UINT QSN,LPSTR szString,int nBufferMax);

	// Exchange Permission Bits.
#define EPB_ZACK_TIER2      0            //Tier TWO service

#define EPB_VICK_INST		0			 //Vickers Institutional
#define EPB_VICK_FPER       1			 //Vickers Five Percent
#define EPB_VICK_INSIDER	2			 //Vickers Insider

#define EPB_MNSTAR_BASIC    0			 //MorningStar Basic service
#define EPB_MNSTAR_PREMIER  1			 //MorningStar Premier service

#define	EPB_JJ_JJMKT		0
#define	EPB_JJ_MUNMKT		1
#define	EPB_JJ_MUNFUT		2
#define	EPB_JJ_FNDSRV		3
#define	EPB_JJ_SHTSRV		4
#define	EPB_JJ_BLUELIST		5

#define	EPB_NASDAQ_L2		4
#define	EPB_SSTNEWS			232
#define	EPB_CARVIEW			233
#define	EPB_SCOPE			234
#define	EPB_SPMVIEW			235
#define	EPB_EARNVIEW		236
#define	EPB_MMS				237
#define EPB_ANR_BIDASK		238

#define EPB_ATPRESS			239
#define EPB_NEWS_LEVEL_II	239

#define EPB_TOPSTORY		240
#define EPB_NEWS_LEVEL_I	240

#define EPB_MAJORATTITUDE	241

#define EPB_CHARTS_LEVEL_I	242
#define EPB_CHARTS_LEVEL_II	243
#define EPB_CHARTH			243
#define EPB_TIME_OF_SALES	244
#define EPB_TICKERS			245
#define EPB_CUSTOM_PAGE		246
#define EPB_OPTION_ANALYTIC	247
#define EPB_HALT_RESUME		248
#define EPB_PORTFOLIO		249

	// Private Label Types.

#define	PLT_ATFI			0
#define	PLT_COMSTOCK		1
#define	PLT_PCQUOTE			2
#define	PLT_ATTITUDE		3

#define	PLI_CORPNAME		0
#define	PLI_PRODUCTNAME		1
#define	PLI_VERSION			2
#define	PLI_LOGINMODE		3
#define	PLI_COPYRIGHT_YEAR	4

int WINAPI 		GetPLInfo(unsigned iType,LPSTR lpCorpName);

// Generic app. about box.
// if szUserMsg is not NULL, it will be display in about box user line.
// LOWORD(dwFlags) == Copyright start year if none zero.

void WINAPI AtDisplayAbout(HWND hWnd,int iAppIndex,LPSTR szUserMsg,DWORD dwFlags);

/*-------- Get/Set Internal values functions and definitions ----------*/

DWORD WINAPI 	GetATLong(unsigned index);
DWORD WINAPI 	SetATLong(unsigned index,DWORD value);

#define	GATL_TRENDARROWS		0
#define	GATL_LCD				1
#define	GATL_SHOWBCASTMAIL		2
#define	GATL_ALLOWNRPOPUP		3
#define	GATL_DEFAULTCC			4
#define	GATL_NETCHANGE			5
#define	GATL_BIDASKALLOWED		6
#define	GATL_UNCHANGED			7
#define	GATL_SHOW16THS			8
#define	GATL_UNOPENED			9
#define	GATL_ATTITUDEMENU		10
#define	GATL_PRIMARYEXCH		11

DWORD WINAPI 	InitEnvironment(HAT *hAt,DWORD patsy);

/*-------------- Encode/Decode local/public directory ----------------*/

BOOL 	WINAPI ATEncodeFilename(LPSTR szFilename);
BOOL 	WINAPI ATDecodeFilename(LPSTR szFilename);

/*------------------- ATX Report App. Activity -----------------------*/

#define	ATX_READY			0
#define	ATX_TERMINATE		1
#define	ATX_LINK_MENU		2
#define	ATX_LAUNCH_DM		3

BOOL	WINAPI		ATXReport(HWND hWnd,DWORD wReportType,DWORD wFlags);

/*------------------- Drag and Drop/Popup Menu -----------------------*/

#define	IDM_LINK_FIRST			32000

#define	DD_ONTARGET				0x4F4B

BOOL	WINAPI		CopyLinkToClipboard(HWND hWnd,LPCF_LINK lpCFLink,DWORD length);
BOOL	WINAPI		CopyLinkFromClipboard(HWND hWnd,LPCF_LINK lpCFLink,DWORD maxLength);

HMENU	WINAPI		CreateLinkMenu(HAT *hAt,int iCurrApp,UINT qsn,LPPB pbPtr);
BOOL	WINAPI		ProcessLinkCommand(HWND hWnd,WPARAM wP,UINT qsn,LPPB pbPtr);
VOID	WINAPI		SendDropMessage(int iAppIndex,BOOL bOpenIcon);

BOOL	WINAPI		CopyPBToClipboard(HWND hWnd,DWORD dwType,UINT qsn,
										UINT channel,LPPB pbPtr,int PBSize);
int		WINAPI		CopyPBFromClipboard(LPCF_LINK lpCFLink,LPPB pbPtr);

BOOL	WINAPI		ClearLinkMenuList(void);
BOOL	WINAPI		AddToLinkMenuList(int iAppIndex);
BOOL	WINAPI		CloseLinkMenuList(void);

#define	IDM_NEWS_FIRST			33000

BOOL	WINAPI		CopyStoryInfoToClipboard(HWND hWnd,DWORD dwType,UINT qsn,
														int iNewsIndex,LPPB lpPB);

int		WINAPI		CheckNewsTarget(HWND hWnd,LPPB pbPtr);
HMENU	WINAPI		CreateNewsMenu(HAT *hAt,UINT qsn,LPPB pbPtr);
BOOL	WINAPI		ProcessNewsCommand(HWND hWnd,WPARAM wP,UINT qsn,LPPB pbPtr);

/*------------------ TOS Filtered Read Functions --------------------*/

#include "tosfread.h"

BOOL  WINAPI TestTosFilteredRead(HAT *hat,DWORD qsn);

WORD  WINAPI CreateTosFile(HAT *hAt,DWORD qsn,LPPB lpPB,
							LPTFRBLOCK pBlock,TOS *tosBuffer);

DWORD WINAPI GetTosFileData(HAT *hAt,DWORD qsn,LPTFRBLOCK pBlock,
							DWORD dwRec,TOS *buffer,DWORD nTos);

void  WINAPI ReleaseTosFile(HAT *hAt,DWORD qsn,LPTFRBLOCK pBlock);

WORD  WINAPI CreateTpbFile(HAT *hAt,DWORD qsn,LPPB lpPB,
							LPTFRBLOCK pBlock,LPETPB etpb);

DWORD WINAPI GetNextTpbData(HAT *hAt,DWORD qsn,LPTFRBLOCK pBlock,
							LPETPB etpb,DWORD nRec);

#define	ReleaseTpbFile(a,b,c) ReleaseTosFile(a,b,c)

//
// TPA - Third Party Application, a method to transmit "orders" to
//		 Non-AT apps via shared memory

// TPA action codes

#define	TPA_BUY			0		// Preferred Tech Code 'B'
#define	TPA_SELL		1		// Preferred Tech Code 'L'
#define	TPA_SHORTSELL	2		// Preferred Tech Code 'S'
#define	TPA_CANCEL		3
#define TPA_BUYOPEN		4		// Preferred Tech Code 'BO'
#define TPA_BUYCLOSE	5		// Preferred Tech Code 'BC'
#define TPA_SELLOPEN    6		// Preferred Tech Code 'SO'
#define TPA_SELLCLOSE	7		// Preferred Tech Code 'SC'

// TPA format codes

#define TPAF_PREFERRED  3		// Preferred Tech
#define TPAF_ORDERSETUP 4		// Detail info from order setup toolbar

	// Note: dwAppID is one of the values defined in atapinfo.h
	// The RegisterTPA function will return non-zero if we are
	//	   "allowed" to send TPA messages. Also returns the
	//     TPA Format number defined in the registry

BOOL WINAPI RegisterTPA(int iAppID,LPWORD lpwFormat);

	// This function will format and copy the TPA message into the
	// shared memory and then post the TPA message to all top-level
	// windows. If TPA Format is not zero, the strings in sz1 and
	// sz2 will be concatenated to the TPA message.
	//
	// Returns Non-Zero if the function worked.

DWORD WINAPI BroadcastTPA(WORD tpaAction,PB *pbPtr,LPSTR sz1,LPSTR sz2);

// Note: These function are exported for use by the QSCSCS

void WINAPI DecompressPQtoAT(DWORD qsn,LPPB lpPB,LPQSMX_MSGS rspMsgPtr,BOOL bATFI);
void WINAPI DecompressExptoAT(DWORD qsn,LPPB lpPB,LPQSMX_MSGS rspMsgPtr,DWORD dwFlags);
void WINAPI TranslatePQtoAT(DWORD qsn,LPPB lpATPB,LPPB lpPQPB,BOOL bSymbol);
void WINAPI TranslateExptoAT(DWORD qsn,LPPB lpATPB,LPEXP_PB lpExpPB,BOOL bSymbol);

#ifdef __cplusplus
}
#endif

#ifndef RC_INVOKED
#pragma pack()
#endif /* RC_INVOKED */

#endif
