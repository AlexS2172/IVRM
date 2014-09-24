/*
 *		etix.h		Date=931209
 *
 *		ETI messages and defines
 *
 *
 *		modified 5/12/93
 *			added support for IBM C Set/2 compiler.
 *
 *		modified 8/19/93
 *			added anonymous structures and unions to trades and quotes for the
 *				flags.
 *
 *    modified 12/09/93	rmh
 *			added the FT_CONTROL structure for file transfers
 *
 */

/*=====================================================================*/

#include <pcqdefs.h>
#include <edbx.h>
#include <indicatr.h>

#ifndef INCL_ETI
#define INCL_ETI

#ifdef __cplusplus			// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

#define ETI_READ			0
#define ETI_ATTACH			1
#define ETI_ATTACH_ASYNC	1
#define ETI_DETACH			2
#define ETI_PERIODIC	 	3
#define ETI_QUERY	 		4
#define ETI_UNLOAD			5
#define ETI_HOLD			6
#define ETI_WRITE			7
//#define ETI_DETACH_ALL		8
#define ETI_ATTACH_SYNC 	9
#define ETI_REGISTER		10
#define ETI_REGISTER_ASYNC	10
#define ETI_UNREGISTER		11
#define ETI_REGISTER_SYNC	12

#define ETI_INSERT_BEFORE	15
#define ETI_INSERT_AFTER	16
#define ETI_DELETE_CURRENT 	17

#define RC_ETI_NOERROR		0
#define RC_ETI_PERIODIC		3
#define RC_ETI_UNLOAD		5
#define RC_ETI_NOMESSAGE	8
#define RC_ETI_INIT			9				// indication to callback to initialize
#define RC_ETI_END			10				// indication to callback to end

#define ERR_ETI_NOT_ATTACHED	0x8000
#define ERR_ETI_QUEUE_FULL		0x8001
#define ERR_ETI_WRITE	 		0x8002
#define ERR_ETI_UNKNOWN			0x8003
#define ERR_ETI_IS_ATTACHED		0x8004
#define ERR_ETI_BUSY			0x8005
#define ERR_ETI_MAX				0x8006
#define ERR_ETI_INV_FUNC_CODE	0x8007
#define ERR_ETI_BAD_PBLK		0x8008

#define HYPER_VER_TRADE_BITS	2           // trade and quote flags fixed
#define HYPER_VER_OHL_FLAGS		3			// open/high/low bit turned on


/*		Define packet Types		 */
/* 	CAUTION:  PCQETIX.EXE must be updated if you add a new message type!! */

#define MT_QUOTE			0			/* QUOTE 			*/
#define MT_TRADE			1			/* TRADE 			*/
#define MT_RESERVED2		2			/* Reserved			*/
#define MT_TOPTEN 			3			/* TOPTEN			*/
#define MT_NEWS_ALERT		4			/* NEWS_ALERT		*/
#define MT_MM_QUOTE			5			/* MMQUOTE			*/
#define MT_BASELINE4 		6			/* REFRESH_SP(dep)	*/
#define MT_BASELINE5 		7			/* REFRESH_SP(ind)	*/
#define MT_LS_REFRESH		8			/* REFRESH			*/
#define MT_LS_CORRECTION	9			/* REFRESH			*/
#define MT_LS_SUMMARY		10 			/* REFRESH			*/
#define MT_LS_DELETE 		11 			/* DELETE			*/
#define MT_ALERT_LINE		12 			/* ALERT_LINE		*/
#define MT_FILE_TRANSFER	13 			/* FILE_TRANSFER	*/
#define MT_HOTBOX 			14 			/* HOTBOX			*/
#define MT_TRADE_CORRECTION	15 			// TRADE CORRECTION
#define MT_MONDO_QUOTE		16 			/* MONDO_QUOTE 		*/

#define MT_MM_REFRESH		18 			/* REFRESH_MM		*/
#define MT_MM_CORRECTION	19 			/* REFRESH_MM		*/
#define MT_MM_SUMMARY		20 			/* REFRESH_MM		*/
#define MT_MM_DELETE 		21 			/* DELETE_MM		*/

#define MT_SP_REFRESH		22 			/* REFRESH_SP		*/
#define MT_SP_CORRECTION	23 			/* REFRESH_SP		*/
#define MT_SP_SUMMARY		24 			/* REFRESH_SP		*/
#define MT_SP_DELETE 		25 			/* DELETE_SP		*/

#define MT_NF_REFRESH		26 			/* REFRESH_NF		*/
#define MT_NF_CORRECTION	27 			/* REFRESH_NF		*/
#define MT_NF_SUMMARY		28 			/* REFRESH_NF		*/
#define MT_NF_DELETE 		29 			/* DELETE_NF		*/

#define MT_EX_SUMMARY		30 			/* EX_SUMMARY		*/
#define MT_SHORT_SUMMARY	31 			// SHORT_SUMMARY
#define MT_EXA_SUMMARY		32 			/* EXA_SUMMARY 		*/
#define MT_TRANSPORT		33			// TRANSPORT
#define MT_RANGE			34 			/* RANGE 			 */

#define MT_VERSION			35 			/* HYPER_VERSION	 */
#define MT_BBO_QUOTE		36 			// BBO QUOTE
#define MT_ORDER			37 			// ORDER
#define	MT_NEWS_STORY		38			/* Full unencrypted News story*/
#define MT_PARTIAL			39			// Partial update

#define MT_TS_CORRECT		40			// Time and sales correction
#define MT_RESERVED41		41
#define MT_RESERVED42		42
#define MT_RESERVED43		43
#define MT_RESERVED44		44
#define MT_RESERVED45		45
#define MT_RESERVED46		46
#define MT_RESERVED47		47
#define MT_RESERVED48		48
#define MT_MARKET_QUOTE		49		// Used for NYSE Liquidity Quote and NASDAQ Inside Quote
#define MT_HFT_BAR			50		// Bar Message currently used for bar corrections
									// we have other programs using Bar so I had to name it HFT_BAR

#define MT_MAX 				51 			// Last valid message

// 							254 Reserved
#define MT_UNDEFINED 		255 		// UNDEFINED_ETI


/*		Define item types		 */

#define IT_EQUITY				0
#define IT_FOREIGN_EXCHG		1
#define IT_FUTURE				2
#define IT_BOND					3
#define IT_EQUITY_OPTION		4
#define IT_FUTURE_OPTION		5
#define IT_INDEX				6
#define IT_DEPOSIT_RATE 		7
#define IT_MUNI_BOND 			8
#define IT_MAXIMUM				IT_MUNI_BOND
#define IT_ITEM_ERROR			0xffff

/*		price types		*/

#define PT_LCD_256				0
#define PT_LCD_64				1
#define PT_FUTURE_256			5
#define PT_FUTURE_64			6
#define PT_FUTURE_32			7
#define PT_FUTURE_8				8
#define PT_DECIMAL_0			9
#define PT_LEADING				10
#define PT_DECIMAL_1			11
#define PT_DECIMAL_2			12
#define PT_DECIMAL_3			13
#define PT_DECIMAL_4			14
#define PT_DECIMAL_5			15
#define PT_DECIMAL_6 			16
#define PT_DECIMAL_7 			17
#define PT_DECIMAL_8 			18
#define PT_DECIMAL_9 			19
#define PT_UNDEFINED 			0xffff	// Do not use.

/*	Define File Transfer Function Bits for all downloads */

#define FT_CONTINUE				0x20000000L
#define FT_PROCEED_ON_ERROR		0x00000001L
#define FT_BEGIN				0x00000004L
#define FT_END 					0x00000008L

// File Transfer Transparent Image Download Messages

#define FT_OPEN					0x00000010L
#define FT_WRITE				0x00000020L
#define FT_READ					0x00000040L
#define FT_CLOSE				0x00000080L
#define FT_VERIFY				0x00000100L
#define FT_SET_FILE_INFO		0x00000200L
#define FT_DELETE				0x00000400L
#define FT_RENAME				0x00000800L
#define FT_MARK_PASS			0x00001000L
#define FT_RESET				0x00002000L

// File Transfer Btrieve Messages Download Messages

#define FT_BTR_ABORT			0x00000100L	
#define FT_BTR_BEGIN			0x00000200L
#define FT_BTR_END				0x00000400L
#define FT_BTR_CREATE			0x00000800L
#define FT_BTR_OPEN				0x00001000L
#define FT_BTR_CLOSE			0x00002000L
#define FT_BTR_INSERT			0x00004000L
#define FT_BTR_UPDATE			0x00008000L
#define FT_BTR_DELETE			0x00010000L
#define FT_BTR_GET_EQ			0x00020000L
#define FT_BTR_GET_GE			0x00040000L
#define FT_BTR_GET_LE			0x00080000L
#define FT_BTR_GET_HI			0x00100000L
#define FT_BTR_GET_LO			0x00200000L
#define FT_BTR_GET_NEXT			0x00400000L
#define FT_BTR_GET_PREV			0x00800000L
#define FT_BTR_REPEAT_UPDATE	0x01000000L
#define FT_BTR_REPEAT_DELETE	0x02000000L
#define FT_BTR_PRESERVE			0x04000000L
#define FT_BTR_USE_PRESERVE		0x08000000L
#define FT_BTR_VARIABLE_LENGTH	0x10000000L
#define FT_BTR_UPDATE_AT 		0x20000000L

// File Transfer DBA Messages Download Messages

#define FT_DBA_ABORT				0x00000100L	
#define FT_DBA_ABORT_TRANSACTION	0x00000100L	
#define FT_DBA_BEGIN				0x00000200L
#define FT_DBA_BEGIN_TRANSACTION	0x00000200L
#define FT_DBA_END					0x00000400L
#define FT_DBA_END_TRANSACTION		0x00000400L
#define FT_DBA_CREATE				0x00000800L
#define FT_DBA_ATTACH				0x00001000L
#define FT_DBA_CLOSE				0x00002000L	// Same as/use FT_DBA_DETACH instead.
#define FT_DBA_DETACH				0x00002000L
#define FT_DBA_INSERT				0x00004000L
#define FT_DBA_WRITE_RECORD			0x00008000L
#define FT_DBA_DELETE_KEY			0x00010000L
#define FT_DBA_GET_EQUAL			0x00020000L
#define FT_DBA_GET_APPROX			0x00040000L
#define FT_DBA_GET_LE				0x00080000L
#define FT_DBA_GET_HIGHER			0x00100000L
#define FT_DBA_GET_LOWER			0x00200000L
#define FT_DBA_GET_NEXT				0x00400000L
#define FT_DBA_GET_PREV				0x00800000L
#define FT_DBA_REPEAT_WRITE			0x01000000L
#define FT_DBA_REPEAT_DELETE		0x02000000L
#define FT_DBA_PRESERVE				0x04000000L
#define FT_DBA_USE_PRESERVE			0x08000000L
#define FT_DBA_VARIABLE_LENGTH		0x10000000L
#define FT_DBA_UPDATE_AT 			0x20000000L
#define FT_DBA_GET_FIRST			0x40000000L
#define FT_DBA_GET_LAST 			0x80000000L

// File Transfer DBA type TS Messages Download Messages

#define FT_DBA_TS_GET_FIRST			0x00000010L
#define FT_DBA_TS_GET_LAST			0x00000020L
#define FT_DBA_TS_GET_NEXT			0x00000040L
#define FT_DBA_TS_GET_PREV			0x00000080L
#define FT_DBA_TS_ADD				0x00000100L
#define FT_DBA_TS_DELETE			0x00000200L
#define FT_DBA_TS_INSERT			0x00000400L
#define FT_DBA_TS_GET_EQUAL			0x00000800L
#define FT_DBA_TS_ATTACH			0x00001000L
#define FT_DBA_TS_DETACH			0x00002000L
#define FT_DBA_TS_GET_APPROX		0x00004000L
#define FT_DBA_TS_WRITE				0x00008000L
#define FT_DBA_TS_GET_FIRST_BLK		0x00010000L
#define FT_DBA_TS_GET_LAST_BLK		0x00020000L
#define FT_DBA_TS_GET_NEXT_BLK		0x00040000L
#define FT_DBA_TS_GET_PREV_BLK		0x00080000L
#define FT_DBA_TS_CANCEL			0x00100000L
#define FT_DBA_TS_CORRECT			0x00200000L	
#define FT_DBA_TS_REPEAT_DELETE		0x02000000L
#define FT_DBA_TS_PRESERVE			0x04000000L
#define FT_DBA_TS_USE_PRESERVE		0x08000000L

//	File Transfer Control Command Messages

#define FT_CTRL_NONE					0		//	no state implied
#define FT_CTRL_MARKET_OPEN				1		//	market open
#define FT_CTRL_MARKET_CLOSED			2		//	market closed
#define FT_CTRL_ETI_SESS_DOWN			4		//	Eti Session down
#define FT_CTRL_ETI_SESS_RESET			8		//	Eti Session reset or restored
#define FT_CTRL_EXECUTE					16		//	process launcher ( .exe ) data is command line
#define	FT_CTRL_START_THREAD			17		//	process launcher ( dll )      
#define FT_CTRL_EXECUTE_EX				18		//	process launcher w/target capability ( .exe ) data is command line
#define FT_CTRL_HEART_BEAT				32		//	heart beat

#define	FT_CTRL_START_ROLL				33		//  roll your own ( command data is ROLL_ENTRY struct )
#define	FT_CTRL_RESTART_ROLL			34		//  rerun scheduled roll ( command data is schedule time )
#define	FT_CTRL_CANCEL_ROLL				35		//  cancel scheduled roll ( command data is schedule time )
#define	FT_CTRL_REACTIVATE_ROLL			36		//  cancel scheduled roll ( command data is schedule time )

#define FT_CTRL_DUMP_ROLL_SCHEDULE		38		//	dump roll schedule to text log
#define	FT_CTRL_RESET_ROLL_SCHEDULE		39		//  reset roll schedule (no data)

#define FT_CTRL_RESET_CALENDARS			40		//  reset calendars	{no data)

#define	FT_CTRL_START_EVENT				41		//	start ( or delay start ) non-scheduled event
#define	FT_CTRL_RESTART_EVENT			42		//  re start scheduled event
#define	FT_CTRL_CANCEL_EVENT			43		//	cancel active or scheduled or delay scheduled event
#define	FT_CTRL_REACTIVATE_EVENT		44		//	reactivate canceled scheduled event
#define FT_CTRL_DUMP_EVENT_SCHEDULE		48		//	dump event schedule to text log
#define	FT_CTRL_RESET_EVENT_SCHEDULE	49		//	reset scheduled events
#define	FT_CTRL_RESET_INDICES			50		//	recalculate Indices
#define	FT_CTRL_RESET_CONTROL_SETTINGS  51      //  reset control settings from ini file / defaults
#define	FT_CTRL_RESET_INDICES_CLOSE		52		//	reset close value in index record
#define	FT_CTRL_RESET_MARKET_STAT		53      //  reset market stat value
#define	FT_CTRL_REMOVE_TOPTEN			54		//	remove item fron topten list
#define FT_CTRL_LOG_MESSAGE				64		// logmessage
#define FT_CTRL_STATS_COMMAND			65		// Stats command

#define FT_CTRL_SET_LOGGING				255		// set control logger mode



#define	FT_MAX_HTI_COMMAND				255		//	maximum Hyperfeed defined command
#define	FT_USER_COMMAND					256		//	first value allowed for user defined commands



#define NEWS_NRS				0x0010	/* Dow Jones News Retrieval	*/
#define NEWS_CMN				0x0004	/* Capital Market News 			*/

/* Define File Transfer Mask Bits */

#define FILE_ADMIN				0x0001	/* Originating source messages*/
#define FILE_DB					0x0002	/* Dbax database records.(bpm)*/
#define FILE_BTRNEWS			0x0004	// Btrieve Style News.(alp)
#define FILE_DML				0x0008	// DML type messages
#define FILE_FTS				0x0010	/* File Transfer format			*/
#define FILE_BTRIEVE			0x0010	/* Btrieve Transfer format		*/
#define FILE_DBA_TS				0x0020	// DBA Transfer format TS functions
#define FILE_DBA				0x0080	/* DBA Transfer format			*/
#define FILE_HFDK				0x0100	/* */
#define FILE_PASSWORD			0x0200	/* Townsend passwords			*/
#define FILE_SESSION 			0x0400	/* Session (roll) events		*/
#define FILE_CONTROL 			0x0800	/* Admin control message		*/
#define FILE_FILTERED			0x1000	/* Filtered File Download		*/
#define FILE_IMAGE				0x2000	/* Image file download			*/
#define FILE_NEWS				0x4000	/* News download				*/
#define FILE_INDEXED 			0x8000	/* Indexed file download		*/

/*		tick values		*/

#define TICK_LS_UP		0x4000
#define TICK_LS_DOWN		0x8000

/*		Define security catagories */

#define SC_UNDEFINED 	0xffff	// (-1)
#define SC_ALPHA		0
#define SC_BETA			1
#define SC_GAMMA		2
#define SC_DELTA		3
#define SC_EPSILON		4
#define SC_ZETA			5
#define SC_ETA			6		// Underlying of currency option
#define SC_THETA		7		// Money market
#define SC_IOTA			8		// Mutual Funds
#define SC_KAPPA		9		// Emerging Companies
#define SC_LAMBDA 		10		// Exchange Indices
#define SC_MU 			11		// Level1 OTCBB (Pink Sheets)

#define PCQUOTE_SOURCE 			100
#define PCQUOTE_1400_SOURCE		120
#define PCQUOTE_1500_SOURCE		121
#define PCQUOTE_QUOTE_SERVER	200
#define PCQUOTE_LOCAL_HOST		0xffff

#define HYPERFEED_SOURCE 			100
#define HYPERFEED_1400_SOURCE		120
#define HYPERFEED_1500_SOURCE		121
#define HYPERFEED_QUOTE_SERVER		200
#define HYPERFEED_LOCAL_HOST		0xffff

#define FEED_RAW				0		// Data Feed is raw
#define FEED_PARTIAL			1		// Partially Data Based
#define FEED_PROCESSED			2		// Fully Data Based, i.e. HyperFeed.

typedef struct _ETI_HEADER_BITS {
	USHORT	exchRexmission :	1; // Retransmission from a exchange.
	USHORT	processOnly		:	1; // Used to tell the server to process the message then drop it and not allow it downstream
	USHORT	dropped			:	1; // Used to drop messages.
	USHORT	passThrough		:	1; // Instructs the server to ignore processing on this message. 
	USHORT	partTwo			:	1;
	USHORT	databased		:	2; // 0 = RAW, 1 = Telekurs, 2 = HyperFeed
	USHORT	debug			:	1; // Internal HyperFeed use.  Used for debugging 
	USHORT	unused2			:	8;
	} ETI_HEADER_BITS;

typedef union _ETI_HEADER_FLAGS {
	ETI_HEADER_BITS	bits;
	USHORT				mask;
	} ETI_HEADER_FLAGS;						// 2 bytes

typedef struct _ETI_PACKET_HEADER {
	ULONG				sequence;			/* HyperFeed Sequence number 				*/
	ULONG				time;				/* Packet time 							*/
	ULONG				dataSequence; 		/* Sequence number of orginating source	*/
//	USHORT				dataModifier;		/* Modifier of data sequence			*/
	ETI_HEADER_FLAGS	flags;				// Flag bits.
//	USHORT				source;				/* HyperFeed Source of data				*/
	UCHAR				source;				// HyperFeed source of data
	UCHAR				version;			// Hyperfeed feed version
	USHORT				dataSource;			/* Orginating Source of data			*/
//	USHORT				dataGap;       		/* Known gaps from this source			*/
	USHORT				packetLength;		// Packet length
	USHORT				type;				/* Packet type							*/
	USHORT				row;				/* Row of entitlement					*/
	USHORT				column;				/* Column of entitlement				*/
} ETI_PACKET_HEADER; 	/* 26 bytes */

typedef struct _ETI_KEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	reserved;
} ETI_KEY;	 /* 22 bytes */

typedef struct _ETI_MMKEY {
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
	UCHAR 	currencyCode[3];
	UCHAR 	mmid[4];
	UCHAR 	reserved;
} ETI_MMKEY;

/* Common transaction flags for multiple message types. */

#define REGIONAL_LAST			0x00000001L
#define REGIONAL_OPEN			0x00000002L
#define REGIONAL_HIGH			0x00000004L
#define REGIONAL_LOW 			0x00000008L
#define REGIONAL_CLOSE			0x00000010L
#define REGIONAL_VOLUME 		0x00000020L
#define REGIONAL_ASK 			0x00000040L
#define REGIONAL_BID 			0x00000080L
#define COMPOSITE_LAST			0x00000100L
#define COMPOSITE_OPEN			0x00000200L
#define COMPOSITE_HIGH			0x00000400L
#define COMPOSITE_LOW			0x00000800L
#define COMPOSITE_CLOSE 		0x00001000L
#define COMPOSITE_VOLUME		0x00002000L
#define COMPOSITE_ASK			0x00004000L
#define COMPOSITE_BID			0x00008000L
#define IS_BBO 					0x00010000L // Quote is the new BBO
#define NO_BBO 					0x00020000L // Issue has no national BBO.
#define FILTERED				0x00040000L // Last Sale price has been filtered.
#define BANKRUPTCY				0x00080000L
#define XDIVIDEND 				0x00100000L
#define INTRADAY				0x00200000L
#define ZERO_BID_ASK			0x00400000L
#define TICK_PRESENT 			0x00800000L
#define REGIONAL_DOWN_TICK 		0x01000000L
#define COMPOSITE_DOWN_TICK		0x02000000L

typedef struct _ETI_TRANSACT_BITS {
	BIT_FIELD	regionalLast		: 1;
	BIT_FIELD	regionalOpen		: 1;
	BIT_FIELD	regionalHigh		: 1;
	BIT_FIELD	regionalLow 		: 1;
	BIT_FIELD	regionalClose		: 1;
	BIT_FIELD	regionalVolume 		: 1;
	BIT_FIELD	regionalAsk 		: 1;
	BIT_FIELD	regionalBid 		: 1;
	BIT_FIELD	compositeLast		: 1;
	BIT_FIELD	compositeOpen		: 1;
	BIT_FIELD	compositeHigh		: 1;
	BIT_FIELD	compositeLow		: 1;
	BIT_FIELD	compositeClose 		: 1;
	BIT_FIELD	compositeVolume		: 1;
	BIT_FIELD	compositeAsk		: 1;
	BIT_FIELD	compositeBid		: 1;
	BIT_FIELD	isBbo 				: 1;	// BBO fields are the new BBO (Composite only)
	BIT_FIELD	noBbo 				: 1;	// Issue has no national BBO.
	BIT_FIELD	filtered 			: 1;	// Last Sale price has been filtered.
	BIT_FIELD	bankruptcy			: 1;
	BIT_FIELD	exDividend			: 1;
	BIT_FIELD	intraDay 			: 1;
	BIT_FIELD	zeroBidAsk			: 1;
	BIT_FIELD	tickPresent 		: 1;	// Tick fields are valid
	BIT_FIELD	regionalDownTick	: 1;	// Regional is down tick
	BIT_FIELD	compositeDownTick	: 1;	// Composite is down tick
	BIT_FIELD	reserved 			: 6;
	} ETI_TRANSACT_BITS; 					// 4 bytes

// Note:  ETI_FUN_TRANSACT_BITS are aligned with ETI_TRANSACT_BITS and are
// used only in the PC Quote Production environment.

#define DO_LAST					0x00000001L
#define DO_OPEN					0x00000002L
#define DO_HIGH					0x00000004L
#define DO_LOW 					0x00000008L
#define DO_CLOSE				0x00000010L
#define DO_VOLUME 				0x00000020L
#define DO_ASK 					0x00000040L
#define DO_BID 					0x00000080L
#define IS_LAST					0x00000100L
#define IS_OPEN					0x00000200L
#define IS_HIGH					0x00000400L
#define IS_LOW 					0x00000800L
#define IS_CLOSE				0x00001000L
#define IS_VOLUME 				0x00002000L
#define IS_ASK 					0x00004000L
#define IS_BID 					0x00008000L

typedef struct _ETI_FUN_TRANSACT_BITS {
	BIT_FIELD	doLast			: 1;		// Do bits indicate that normal
	BIT_FIELD	doOpen			: 1;		// processing should occur and that
	BIT_FIELD	doHigh			: 1;		// these fields may or may not be
	BIT_FIELD	doLow 			: 1;		// affected.
	BIT_FIELD	doClose			: 1;
	BIT_FIELD	doVolume 		: 1;
	BIT_FIELD	doAsk 			: 1;
	BIT_FIELD	doBid 			: 1;
	BIT_FIELD	isLast			: 1;		// Is bits indicate that this IS a
	BIT_FIELD	isOpen			: 1;		//	new value for this field which
	BIT_FIELD	isHigh			: 1;		// should just be stored.
	BIT_FIELD	isLow 			: 1;
	BIT_FIELD	isClose			: 1;
	BIT_FIELD	isVolume 		: 1;
	BIT_FIELD	isAsk 			: 1;
	BIT_FIELD	isBid 			: 1;
	BIT_FIELD	isBbo 			: 1;		// BBO fields are the new BBO (Composite only)
	BIT_FIELD	noBbo 			: 1;		//	Issue has no national BBO.
	BIT_FIELD	filtered 		: 1;		// Last Sale price has been filtered.
	BIT_FIELD	bankruptcy		: 1;
	BIT_FIELD	exDividend		: 1;
	BIT_FIELD	intraDay 		: 1;
	BIT_FIELD	zeroBidAsk		: 1;		// Zero bid/ask prices should be stored.
	BIT_FIELD	reserved 		: 9;
	} ETI_FUN_TRANSACT_BITS;				// 4 bytes

typedef union _ETI_TRANSACT_FLAGS {
	ETI_TRANSACT_BITS 		bits;
	ETI_FUN_TRANSACT_BITS	funBits;
	ULONG 					mask;
	} ETI_TRANSACT_FLAGS;					// 4 bytes
/* packet Type MT_MM_QUOTE */

typedef struct _MMQUOTE {
	ETI_PACKET_HEADER	header;
	ETI_MMKEY			item;
	DBA_MASK_5			dbaMask5;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				quoteIndicator;
	LONG				bidPrice;
	LONG				bidSize;
	LONG				askPrice;
	LONG				askSize;
} MMQUOTE;

/* packet Type MT_MM_REFRESH */
/* packet Type MT_MM_CORRECTION	*/
/* packet Type MT_MM_SUMMARY */

typedef struct _REFRESH_MM {
	ETI_PACKET_HEADER	header;
	DBA_RECORD_5		recMM;
/*
	DBA_MMKEY	item;
	DBA_MASK_5	dbaMask5;
	TICK		tick;
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		quoteIndicator;
	DBA_PRICE	bid;
	DBA_PRICE	ask;
	BYTE		bidTimeHundredths;
	BYTE		bidTimeSeconds;
	BYTE		askTimeHundredths;
	BYTE		askTimeSeconds;
	USHORT		row;
	USHORT		column;
*/
} REFRESH_MM;

/* packet Type MT_MM_DELETE */

typedef struct _DELETE_MM {
	ETI_PACKET_HEADER	header;
	ETI_MMKEY			item;
	USHORT				flags;
} DELETE_MM;

#define	TR_TRADE_TICK				0x0001
#define TR_BANKRUPTCY				0x0002
#define	TR_DOWN_TICK_REGIONAL		0x0004
#define	TR_DOWN_TICK_COMPOSITE		0x0008
#define TR_REGIONAL_CLOSE			0x0010
#define TR_COMPOSITE_CLOSE			0x0020
#define TR_COMPOSITE_ASK			0x0040
#define TR_COMPOSITE_BID			0x0080
#define TR_REGIONAL_ASK				0x0100
#define TR_REGIONAL_BID				0x0200
#define TR_COMPOSITE_LAST			0x0400
#define TR_COMPOSITE_VOLUME			0x0800
#define TR_EX_DIVIDEND				0x1000
#define TR_FILL_ZERO_LAST			0x2000
#define TR_FILL_ZERO_SIZE			0x4000


typedef struct _ETI_TRADE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	tradeTick		: 1;		// trade tick is present
	BIT_FIELD	bankruptcy		: 1;
	BIT_FIELD	downTickReg		: 1;		// regional is down tick
	BIT_FIELD	downTickComp	: 1;        // composite is down tick
	BIT_FIELD	regionalClose	: 1;
	BIT_FIELD	compositeClose	: 1;
	BIT_FIELD	compositeAsk 	: 1;
	BIT_FIELD	compositeBid 	: 1;
	BIT_FIELD	regionalAsk 	: 1;
	BIT_FIELD	regionalBid 	: 1;
	BIT_FIELD	compositeLast	: 1;
	BIT_FIELD	compositeVolume	: 1;
	BIT_FIELD	exDividend		: 1;
	BIT_FIELD	fillZeroLast	: 1; //The fill flags will instruct the database to fill the field
	BIT_FIELD	fillZeroSize	: 1; //in the message with the last value if the field is 0.
	BIT_FIELD	undefined1		: 1; 
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	undefined1		: 1;
	BIT_FIELD	fillZeroLast	: 1;
	BIT_FIELD	fillZeroSize	: 1;
	BIT_FIELD	exDividend		: 1;
	BIT_FIELD	compositeVolume	: 1;
	BIT_FIELD	compositeLast	: 1;
	BIT_FIELD	regionalBid 	: 1;
	BIT_FIELD	regionalAsk 	: 1;
	BIT_FIELD	compositeBid 	: 1;
	BIT_FIELD	compositeAsk 	: 1;
	BIT_FIELD	compositeClose	: 1;
	BIT_FIELD	regionalClose	: 1;
	BIT_FIELD	downTickComp	: 1;        
	BIT_FIELD	downTickReg		: 1;		
	BIT_FIELD	bankruptcy		: 1;
	BIT_FIELD	tradeTick		: 1;		
#endif
} ETI_TRADE_BITS;

/* Flags for the Quote message */

typedef union _ETI_TRADE_FLAGS {
	ETI_TRADE_BITS		bits;
	USHORT				mask;
} ETI_TRADE_FLAGS;				// 2 bytes

/* Trade Flags	*/

#define TF_INTRADAY			0x0001
#define TF_REGIONAL_OPEN	0x0002
#define TF_REGIONAL_LOW		0x0004
#define TF_REGIONAL_HIGH	0x0008
#define TF_FILTERED			0x0010
#define TF_COMPOSITE_OPEN	0x0020
#define TF_COMPOSITE_LOW 	0x0040
#define TF_COMPOSITE_HIGH	0x0080
#define TF_UPDATE_VOLUME	0x0100
#define TF_UPDATE_OHL		0x0200
#define TF_UPDATE_LAST		0x0400
#define TF_CANCEL 			0x0800
#define TF_BBO_NO_CHANGE	0x2000
#define TF_REGIONAL_ONLY	0x2000

/* packet Type MT_TRADE */

typedef struct _TRADE {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	union {
		struct {
		#if defined( PQTENDIAN_LITTLE )
			BIT_FIELD			intraDay 		:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD		 	compositeOpen	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD 			updateVolume	:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			cancel			:1;
			BIT_FIELD			bboNoBbo 		:1;
			BIT_FIELD		 	regionalOnly	:1;
			BIT_FIELD	    	bboIsBbo 		:1;
			BIT_FIELD   		bboAppendage	:1;
		#elif defined( PQTENDIAN_BIG ) 
			BIT_FIELD   		bboAppendage	:1;
			BIT_FIELD	    	bboIsBbo 		:1;
			BIT_FIELD		 	regionalOnly 	:1;
			BIT_FIELD			bboNoBbo 		:1;
			BIT_FIELD			cancel			:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD 			updateVolume	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD		 	compositeOpen	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			intraDay 		:1;
		#endif
		#if defined (__IBMC__) || defined (__IBMCPP__) ||  defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined (__unix)
			} bits;
		#else
			};
		#endif

		USHORT  mask;
		
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined(__unix)
			} flag1;
		#else
			};
		#endif

	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				tradeIndicator;
	LONG				price;
	LONG				size;
	LONG				volume;
	USHORT				sellersDays;
	USHORT				buyersDays;
	ETI_TRADE_FLAGS		flags;
	USHORT				flagsReserved;
	LONG				regionalVolume;
} TRADE;

#define	QT_QUOTE_TICK				0x0001
#define QT_BANKRUPTCY				0x0002
#define	QT_DOWN_TICK_REGIONAL		0x0004
#define	QT_DOWN_TICK_COMPOSITE		0x0008
#define QT_REGIONAL_CLOSE			0x0010
#define QT_COMPOSITE_CLOSE			0x0020
#define QT_COMPOSITE_ASK			0x0040
#define QT_COMPOSITE_BID			0x0080
#define QT_REGIONAL_ASK				0x0100
#define QT_REGIONAL_BID				0x0200
#define QT_COMPOSITE_LAST			0x0400
#define QT_COMPOSITE_VOLUME			0x0800
#define QT_EX_DIVIDEND				0x1000
#define QT_COMPOSITE_TICK_PRESENT	0x2000 // quote tick that affects composite record is present


typedef struct _ETI_QUOTE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	quoteTick				: 1;		// quote tick is present
	BIT_FIELD	bankruptcy				: 1;
	BIT_FIELD	downTickReg				: 1;		// regional is down tick
	BIT_FIELD	downTickComp			: 1;        // composite is down tick
	BIT_FIELD	regionalClose			: 1;
	BIT_FIELD	compositeClose			: 1;
	BIT_FIELD	compositeAsk 			: 1;
	BIT_FIELD	compositeBid 			: 1;
	BIT_FIELD	regionalAsk 			: 1;
	BIT_FIELD	regionalBid 			: 1;
	BIT_FIELD	compositeLast			: 1;
	BIT_FIELD	compositeVolume			: 1;
	BIT_FIELD	exDividend				: 1;
	BIT_FIELD	compositeTickPresent	: 1;		// quote tick that affects composite record is present
	BIT_FIELD	undefined1				: 2;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	undefined1				: 2;
	BIT_FIELD	compositeTickPresent	: 1;
	BIT_FIELD	exDividend				: 1;
	BIT_FIELD	compositeVolume			: 1;
	BIT_FIELD	compositeLast			: 1;
	BIT_FIELD	regionalBid 			: 1;
	BIT_FIELD	regionalAsk 			: 1;
	BIT_FIELD	compositeBid 			: 1;
	BIT_FIELD	compositeAsk 			: 1;
	BIT_FIELD	compositeClose			: 1;
	BIT_FIELD	regionalClose			: 1;
	BIT_FIELD	downTickComp			: 1;        
	BIT_FIELD	downTickReg				: 1;		
	BIT_FIELD	bankruptcy				: 1;
	BIT_FIELD	quoteTick				: 1;		
#endif
} ETI_QUOTE_BITS;

/* Flags for the Quote message */

typedef union _ETI_QUOTE_FLAGS {
	ETI_QUOTE_BITS		bits;
	USHORT				mask;
} ETI_QUOTE_FLAGS;	// 2 bytes

/*   m Quote Flags	 */

#define QF_INTRADAY			0x0001
#define QF_REGIONAL_OPEN	0x0002
#define QF_REGIONAL_LOW		0x0004
#define QF_REGIONAL_HIGH	0x0008
#define QF_FILTERED			0x0010
#define QF_COMPOSITE_OPEN	0x0020
#define QF_COMPOSITE_LOW 	0x0040
#define QF_COMPOSITE_HIGH	0x0080
#define QF_UPDATE_VOLUME	0x0100
#define QF_UPDATE_OHL		0x0200
#define QF_UPDATE_LAST		0x0400
#define QF_ZERO_BID_ASK		0x0800
#define QF_BBO_NO_BBO		0x1000
#define QF_BBO_NO_CHANGE	0x2000
#define QF_BBO_IS_BBO		0x4000
#define QF_BBO_APPENDAGE	0x8000

typedef struct _ETI_QUOTE_BITS1 {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			intraDay 		:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD			compositeOpen	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD			updateVolume	:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			bboNoBbo 		:1;	/* item has no bbo		 */
	BIT_FIELD			bboNoChange 	:1;	/* quote does not affect the BBO  */
	BIT_FIELD			bboIsBbo 		:1;	/* quote is the BBO (no appendage) */
	BIT_FIELD			bboAppendage	:1;	/* Valid BBO appendage		 */
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD			bboAppendage	:1;
	BIT_FIELD			bboIsBbo 		:1;
	BIT_FIELD			bboNoChange 	:1;
	BIT_FIELD			bboNoBbo 		:1;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD			updateVolume	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD			compositeOpen	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			intraDay 		:1;
#endif
} ETI_QUOTE_BITS1;

/* Flags for the Quote message */

typedef union _ETI_QUOTE_FLAGS1 {
	ETI_QUOTE_BITS1		bits;
	USHORT				mask;
} ETI_QUOTE_FLAGS1;				 // 2 bytes


#define QF_FILL_ZERO_BID		0x0001
#define QF_FILL_ZERO_BID_SIZE	0x0002
#define QF_FILL_ZERO_ASK		0x0004
#define QF_FILL_ZERO_ASK_SIZE	0x0008

typedef struct _ETI_QUOTE_BITS2 {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			fillZeroBid			:1;  //The fill flags will instruct the
	BIT_FIELD			fillZeroBidSize		:1;	 //database to fill the field in the message 	
	BIT_FIELD			fillZeroAsk			:1;	 //with the last value if the field is 0.
	BIT_FIELD			fillZeroAskSize		:1;
	BIT_FIELD			fillZeroBBOBid		:1;  //The fill flags will instruct the
	BIT_FIELD			fillZeroBBOBidSize	:1;	 //database to fill the field in the message 	
	BIT_FIELD			fillZeroBBOAsk		:1;	 //with the last value if the field is 0.
	BIT_FIELD			fillZeroBBOAskSize	:1;
	BIT_FIELD			reserved			:8;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD			reserved			:8;
	BIT_FIELD			fillZeroBBOAskSize	:1;
	BIT_FIELD			fillZeroBBOAsk		:1;	 //with the last value if the field is 0.
	BIT_FIELD			fillZeroBBOBidSize	:1;	 //database to fill the field in the message 	
	BIT_FIELD			fillZeroBBOBid		:1;	 //database to fill the field in the message 
	BIT_FIELD			fillZeroAskSize		:1;  //The fill flags will instruct the
	BIT_FIELD			fillZeroAsk			:1;	 //database to fill the field in the message 	
	BIT_FIELD			fillZeroBidSize		:1;	 //with the last value if the field is 0.
	BIT_FIELD			fillZeroBid			:1;
	
#endif
} ETI_QUOTE_BITS2;

typedef union _ETI_QUOTE_FLAGS2 {
	ETI_QUOTE_BITS2		bits;
	USHORT				mask;
} ETI_QUOTE_FLAGS2;				 // 2 bytes

/* packet Type MT_QUOTE */

typedef struct _QUOTE {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	union {
		struct {
		#if defined( PQTENDIAN_LITTLE )
			BIT_FIELD			intraDay 		:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD			compositeOpen	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD			updateVolume	:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			zeroBidAsk		:1;
			BIT_FIELD			bboNoBbo 		:1;	/* item has no bbo		 */
			BIT_FIELD			bboNoChange 	:1;	/* quote does not affect the BBO  */
			BIT_FIELD			bboIsBbo 		:1;	/* quote is the BBO (no appendage) */
			BIT_FIELD			bboAppendage	:1;   /* Valid BBO appendage		 */
		#elif defined( PQTENDIAN_BIG )
			BIT_FIELD			bboAppendage	:1; 
			BIT_FIELD			bboIsBbo 		:1;	
			BIT_FIELD			bboNoChange 	:1;	
			BIT_FIELD			bboNoBbo 		:1;	
			BIT_FIELD			zeroBidAsk		:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD			updateVolume	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD			compositeOpen	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			intraDay 		:1;
		#endif
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined (__unix)
			} bits;
		#else
			};
		#endif
		USHORT  mask;
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined (__unix)
			} flag1;
		#else
			};
		#endif

	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				quoteIndicator;
	LONG				bidPrice;
	LONG				bidSize;
	LONG				askPrice;
	LONG				askSize;
	LONG				bboBidPrice;
	LONG				bboBidSize;
	LONG				bboAskPrice;
	LONG				bboAskSize;
	UCHAR 				askExchangeCode[2];
	UCHAR 				bboBidExchangeCode[2];
	UCHAR 				bboAskExchangeCode[2];
	ETI_QUOTE_FLAGS		flags;
	ETI_QUOTE_FLAGS2	flags2; // Not currently on feed (7/31/02)
} QUOTE;

/* packet Type MT_TOPTEN */

typedef struct _TOPTEN {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	ETI_KEY				data[10];
} TOPTEN;

/* packet Type MT_HOTBOX */
 
#define HB_STATS	0x0800
#define HB_DUTIL	0x1000
#define HB_DTRAN	0x2000
#define HB_DJIA		0x8000

typedef struct _HOTBOX {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				reserved;
	LONG				djia;
	LONG				djiac;
	LONG				dtran;
	LONG				dutil;
	LONG				advn;
	LONG				decn;
	LONG				volm;
	LONG				tick;
	LONG				trin;
} HOTBOX;

/* Packet Type MT_MONDO_QUOTE */

#define	MQ_FILTERED 	0x01

//typedef struct _MONDO_QUOTE_BITS {
//   BIT_FIELD 		  filtered		  : 1;  // filtered by open interest
//   BIT_FIELD 		  unused 		  : 7;
//   } MONDO_QUOTE_BITS;
//
//typedef union _MONDO_QUOTE_FLAGS {
//   MONDO_QUOTE_BITS  bits;
//   UCHAR				  mask;
//   } MONDO_QUOTE_FLAGS;

typedef struct _MONDO_ITEM {
	UCHAR 		expiration; // Expiration Month Code
	UCHAR 		strike;		// Strike Price
	UCHAR 		exchangeCode;
	UCHAR 		flags;
	LONG		bidPrice;
	LONG		askPrice;
	USHORT		bidSize;
	USHORT		askSize;
} MONDO_ITEM;	// 16 bytes

typedef struct _MONDO_QUOTE {
	ETI_PACKET_HEADER 	header;				// 26 bytes
	ETI_KEY				item; 				// 22 bytes
	ETI_QUOTE_FLAGS1	flag1;				// 2 bytes
	USHORT				priceType;
	USHORT				quoteIndicator;
	USHORT				securityCategory;
	USHORT				count;
	MONDO_ITEM			mondoItem[27]; 		// 27 * 16 bytes
} MONDO_QUOTE; 	// 490 bytes

/* Packet Type MT_TRADE_CORRECTION */

#define CORRX_CANCEL 			0x0001
#define CORRX_REGUPTREND		0x0002
#define CORRX_REGDOWNTREND 		0x0004
#define CORRX_COMPUPTREND		0x0008
#define CORRX_COMPDOWNTREND		0x0010
#define CORRX_REGUPTICK 		0x0020
#define CORRX_REGDOWNTICK		0x0040
#define CORRX_COMPUPTICK		0x0080
#define CORRX_COMPDOWNTICK 		0x0100
#define CORRX_INSERTTRADE 		0x0200

typedef struct _ETI_CORRECTION_BITS {
	BIT_FIELD	cancel			: 1;
	BIT_FIELD	regUpTrend		: 1;
	BIT_FIELD	regDownTrend	: 1;
	BIT_FIELD	compUpTrend 	: 1;
	BIT_FIELD	compDownTrend	: 1;
	BIT_FIELD	regUpTick		: 1;
	BIT_FIELD	regDownTick 	: 1;
	BIT_FIELD	compUpTick		: 1;
	BIT_FIELD	compDownTick	: 1;
	BIT_FIELD	insertTrade		: 1;
	BIT_FIELD	reserved 		: 6;
} ETI_CORRECTION_BITS;				// 2 bytes

typedef union _ETI_CORRECTION_FLAGS 
{
	ETI_CORRECTION_BITS	bits;
	USHORT					mask;
} ETI_CORRECTION_FLAGS; 			// 2 bytes

typedef struct _ETI_TRADE_CORRECTION 
{
	ETI_PACKET_HEADER 		header;		// 26 bytes
	ETI_KEY					item; 		// 18 bytes
	ETI_CORRECTION_FLAGS	corxFlags;	// 2 bytes
	USHORT					securityCategory;
	USHORT					priceType;
	USHORT					displayPriceType;
	USHORT					tradeIndicator;
	SHORT 					seller;
	ETI_TRANSACT_FLAGS		price1Flags;	// 4 bytes
	DBA_PRICE				price1;			// 12 bytes
	ETI_TRANSACT_FLAGS		price2Flags;	// 4 bytes
	DBA_PRICE				price2;			// 12 bytes
	ETI_TRANSACT_FLAGS		newPriceFlags; // 4 bytes
	DBA_PRICE				newPrice;		// 12 bytes
	LONG					oldPrice;
	ULONG 					oldSize;
	ULONG 					originalSequence;
	LONG					printsBack;
	LONG					volume;
	SHORT 					buyer;
	USHORT 					reserved;
} ETI_TRADE_CORRECTION;

/* generic packet with a header and item */

typedef struct _ETI_HEADER_ITEM {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
} ETI_HEADER_ITEM;

/* packet Type MT_LS_REFRESH	  */
/* packet Type MT_LS_CORRECTION */
/* packet Type MT_LS_SUMMARY	  */

typedef struct _SUM_PRICE {
	LONG	price;
	LONG	size;
	UCHAR	exchangeCode[2];
	UCHAR	hour;
	UCHAR	minute;
} SUM_PRICE;

typedef struct _REFRESH {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	DBA_MASK_3			dbaMask3;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				tick;
	USHORT				newsSource[3];
	UCHAR 				primaryExchangeCode[2];
	USHORT				quoteIndicator;
	USHORT				tradeIndicator;
	SUM_PRICE			lastSale;
	SUM_PRICE			open;
	SUM_PRICE			high;
	SUM_PRICE			low;
	SUM_PRICE			close;
	SUM_PRICE			bid;
	SUM_PRICE			ask;
	SUM_PRICE			mid;
	SUM_PRICE			midClose;
	ULONG 				openInterest;
	ULONG 				volume;
	ULONG				openRange;
	ULONG				closeRanges[2];
	USHORT				session;
	UCHAR				prevBidAskMonth;		// Previous Bid Ask Month
	UCHAR				prevBidAskDay;			// Previous Bid Ask Day
	LONG				prevBidPrice;			// Previous closing bid price.
	LONG				prevAskPrice;			// Previous closing ask price.
	ULONG				bidVolume;				// Total volume of trades at the bid price.
	ULONG				askVolume;				// Total volume of trades at the ask price.
	LONGLONG			moneyFlow;				// Total value of ask side sales minus bid side sales.
	DBA_EXTENDED_FLAGS	flags;
	SUM_PRICE			settlement;
	SUM_PRICE			yestSettle;
	ULONG 				yestVolume;
	ULONG 				lateVolume;
	ULONG				lateTicks;
	ULONG				lastTradeSrcSeq;
	ULONG				lastQuoteSrcSeq;
	UCHAR				lastQuoteExch[2];
	SHORT				tickTrend;
} REFRESH;

/* packet Type MT_SP_REFRESH  */
/* packet Type MT_SP_CORRECTION */
/* packet Type MT_SP_SUMMARY  */

typedef struct _REFRESH_SP {
	ETI_PACKET_HEADER	header;
	DBA_RECORD_52		recSP;
} REFRESH_SP;

/* packet type MT_FILE_TRANSFER */

typedef struct _NEWS_HEADLINE {
	SHORT	storyNumber;
	SHORT	catagoryNumber;
	UCHAR	serviceLevel;
	UCHAR	priority;
	UCHAR	catagory;
	UCHAR	reserved;
	UCHAR	text[80];
} NEWS_HEADLINE;

typedef struct _DV_INDEX {
	UCHAR   product[2];
	UCHAR   date[8];
	UCHAR   sequence[4];
	UCHAR   type;
	UCHAR   key[13];
} DV_INDEX;

typedef struct _DV_ACCESSION {	// used for chaining, base 36 maximum value = zzzzzz = 2,176,782,335 or year = 217,678.
	UCHAR	date[6];				// Need to convert to base 10 to match orgional sequence number.
	UCHAR	sequence[4];			// sequence base 36, same base as orgional sequence number
} DV_ACCESSION;

typedef struct _DV_TEXT {
	UCHAR			product[2];
	UCHAR			date[8];
	UCHAR			sequenceNumber[4];
	UCHAR			newsSource[4];
	UCHAR			origSource[4];
	UCHAR			tempPerm;
	UCHAR			retention;
	UCHAR			displayDate[6];
	UCHAR			displayTime[4];
	DV_ACCESSION	accession;
	USHORT			headlineLength;
	USHORT			hot;
	USHORT			row;
	USHORT			column;
} DV_TEXT;

typedef struct _FT_CONTROL {
	USHORT  source;
	USHORT  command;
	ULONG   mask;
	USHORT  textLength;
	USHORT  reserved;
	UCHAR   text[168];
} FT_CONTROL;	// 180 bytes

typedef struct _FT_DML {
	CHAR	szHost[32];
	ULONG	instance;
	UCHAR   payLoad[912];
} FT_DML;			// 948 bytes

#define FTSESSEVENT_BOD1	0		// Beginning of day.
									// 1-7 reserved.
#define FTSESSEVENT_EOD1	8		// End of day.
									// 9-15 reserved.

typedef struct _FTSESS_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		disabled	: 1;	// Session disabled.
	BIT_FIELD		deleted		: 1;	// Session deleted.
	BIT_FIELD		refresh		: 1;	// Database refresh (not live).
	BIT_FIELD		unused		: 13;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		unused		: 13;
	BIT_FIELD		refresh		: 1;
	BIT_FIELD		deleted		: 1;
	BIT_FIELD		disabled	: 1;
#endif
} FTSESS_BITS;

typedef union _FTSESS_FLAGS {
	FTSESS_BITS 	bits;
	USHORT			mask;
} FTSESS_FLAGS;

typedef struct _FT_SESSION {
	USHORT			session;
	USHORT			eventType;
	FTSESS_FLAGS	flags;
	PCQ_TIME 		timeGMT; 			// GMT time of roll.
	PCQ_DATE 		dateLocal;			// Exchange local date of roll.
	UCHAR 			sessionBODs[7];		// # of session starts per day of week.
	UCHAR 			sessionEODs[7];		// # of session ends per day of week.
	UCHAR 			countryCode[2];		// Exchange info (used by tower).
	UCHAR 			exchangeCode[2];
} FT_SESSION;

typedef struct _FT_PASSWORD {
	UCHAR 			password[32];
} FT_PASSWORD;

typedef struct _FT_HFDK {
	UCHAR 			data[1];
} FT_HFDK;

/* Note: File Transfer data size is limited by a maximum total 19.2 */
/* packet size of 256 bytes in the user Dcp drivers _inpxx buffer */
/* (as of 8/19/92). */

typedef union _FT_DATA {
	NEWS_HEADLINE	headline;
	UCHAR 			storyText[180];
	UCHAR 			nrs[180];
	UCHAR 			fts[180];
	DV_TEXT			dvText;
	DV_INDEX		dvIndex;
	UCHAR 			dvData[180];
	FT_CONTROL		control;				// admin control messages
	FT_SESSION		session;
	FT_PASSWORD 	password;			// Townsend passwords
	FT_HFDK			hfdk;
	FT_DML			dml;
} FT_DATA;

typedef struct _FILE_TRANSFER {
	ETI_PACKET_HEADER	header;
	USHORT				dataLength;
	ULONG 				function;
	ULONG 				filePointer;
	USHORT				sequenceNumber;
	USHORT				keyPosition;
	USHORT				keyLength;
	USHORT				keyNumber;
	USHORT				namePosition;
	USHORT				nameLength;
	USHORT				address[4];
	USHORT				subAddress[4];
	USHORT				transferMask[4];
	FT_DATA				data;
} FILE_TRANSFER;

#define FILE_TRANSFER_MIN_SIZE	( sizeof( FILE_TRANSFER ) - sizeof((( FILE_TRANSFER*)0)->data ))

#define OLD_ETI_MAX  (sizeof(FILE_TRANSFER) - sizeof(FT_DATA)) + 250

/* packet Type MT_NEWS_ALERT */

#define	NA_FLAG_BASE_ITEM 		 0x8000

/* Service category News Source defines */

#define	NS_DOWJONES 			 0 	// (DJ) 	 T
#define	NS_WALLSTREETJOURNAL 	 1 	// (WSJ)	 J
#define	NS_BARRONS				 2 	// (BAR)	 B
#define	NS_NEWSRETRIEVAL		 3 	// (DNR)	 N
#define	NS_DOWJONESWALLSTREET	 4 	// (DWS)	 W
#define	NS_CAPITALMARKETNEWS 	 5 	// (CMR)	 C
#define	NS_BUSINESSWIRE			 6 	// (BW )	 S
#define	NS_COMTEXSCIENTIFIC		 7 	// (CTX)	 X
#define	NS_AFPDPA				 8 	// (AFP)	 D
#define	NS_ASSOCIATEDPRESS		 9 	// (AP )	 E
#define	NS_GARVINGUYBUTLER		10 	// (GGB)	 G
#define	NS_KYODDNEWSINT			11 	// (KYO)	 K
#define	NS_PRNEWSWIRE			12 	// (PR )	 R
#define	NS_UNITEDPRESSINT 		13 	// (UPI)	 U
#define	NS_SECURITYTRADERS		14 	// (ST )	 Y
#define	NS_PRNEWSWIREINC		15 	//			 P
#define	NS_DPA					16 	// (DPA)	 H
#define	NS_FEDERALNEWS 			17 	// (FNS)	 V
#define	NS_BUSINESSWIRENEWS		18 	// (BW )	 Z
#define	NS_INTLDATAGROUP		19 	// (not implemented)
#define	NS_MUNIFACTS			20 	// (MFX)	 M
#define	NS_REUTERSDIRECT		21 	// (RTR)	 A
#define	NS_PROFINVESTOR			22 	// (PIR)	 I
#define	NS_FIRSTCALLUS 			23 	// (FC )	 F
#define	NS_FEDERALFILINGS 		24 	// (FED)	 L
#define	NS_FUTUREWORLD 			25 	// (FWN)	 1

#define	NS_DJNDV_BT				26	// DV Broad Tape
#define	NS_DJNDV_CM				27	// DV Capital Markets
#define	NS_DJNDV_PI				28	// DV Professional Investor Report
#define	NS_DJNDV_PR				29	// DV Press Release Wires
#define	NS_DJNDV_DI				30	// DV International News Wire
#define	NS_DJNDV_FF				31	// DV Federal Filings
#define	NS_DJNDV_WSJ			32	// DV Wall Street Journal
#define	NS_DJNDV_FF10			33	// DV Federal Filings 10K's & 10Q's
#define	NS_DJNDV_ON 			34	// DV Online News

#define	NS_EXTEL 				64 	 // (EXT)	 E
#define	NS_EXTELWORLDECONEWS 	65 	 // (GEX)	 W
#define	NS_EXTELAFP 			66 	 // (AFX)	 A
#define	NS_VWDNEWS				67 	 // (VWD)	 V
#define	NS_DJIECONEWSWIRE 		68 	 // (DJE)	 I
#define	NS_DJIFINNEWSWIRE 		69 	 // (DJF)	 F
#define	NS_DJIEUROCORPREPT		70 	 // (DJC)	 C
#define	NS_FIRSTCALLEUROPE		71 	 // (FCE)	 B

#define	NS_NIKKEI				96 	 // (NEN)	 N
#define	NS_AUSTRFINRPT 			97 	 // (AFR)	 A
#define	NS_AUSTRAP				98 	 // (AAP)	 P
#define	NS_KYODONEWS			99 	 // (KYO)	 K
#define	NS_FIRSTCALLPACIFIC		100  // (FCP)	 F


#define DELETE_NF  DELETE_EX

typedef struct _NEWS_ALERT {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
	USHORT				serviceCatagory;
} NEWS_ALERT;

/* packet Type MT_NEWS_STORY	  */

#define NSF_NEWS				0x0001
#define NSF_ADDINDEX			0x0002
#define NSF_CHANGEHEADLINE		0x0004
#define NSF_CHANGESTORY			0x0008
#define NSF_DELETEINDEX			0x0010
#define NSF_DELETESTORY			0x0020
#define NSF_LONGSTORY			0x0040

typedef struct _NS_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		news			: 1;
	BIT_FIELD		addIndex		: 1;
	BIT_FIELD		changeHeadline	: 1;
	BIT_FIELD		changeStory		: 1;
	BIT_FIELD		deleteIndex		: 1;
	BIT_FIELD		deleteStory		: 1;
	BIT_FIELD		longStory		: 1;
	BIT_FIELD		unused			: 9;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		unused			: 9;
	BIT_FIELD		longStory		: 1;
	BIT_FIELD		deleteStory		: 1;
	BIT_FIELD		deleteIndex		: 1;
	BIT_FIELD		changeStory		: 1;
	BIT_FIELD		changeHeadline	: 1;
	BIT_FIELD		addIndex		: 1;
	BIT_FIELD		news			: 1;
#endif
} NS_BITS;

typedef union _NS_FLAGS {
	NS_BITS 	bits;
	USHORT		mask;
} NS_FLAGS;

typedef struct _NEWS_STORY {
	ETI_PACKET_HEADER	header;			// 26 bytes
	NS_FLAGS			flags;
	USHORT				reserved;
	USHORT				sequenceNumber;	 
	USHORT				entitleCount;	//to be used for multi-row entitlement will state the 
										// number of row-column pairs at the begining of data.
	UCHAR				sourceCode[4];	// Text code representing the data source.
	DV_TEXT				dvText;			// 52 bytes
	ULONG				dataLength;
	UCHAR				data[1];		// Warning: Can be up to 64KB!
} NEWS_STORY;						// ?? bytes

#define NEWS_STORY_MIN_SIZE	( sizeof( NEWS_STORY ) - sizeof((( NEWS_STORY*)0)->data ))

/* packet Type MT_NF_REFRESH	  */
/* packet Type MT_NF_CORRECTION */
/* packet Type MT_NF_SUMMARY	  */

typedef struct _REFRESH_NF {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				zeroCount;
	NEWS_FLAGS			flags;
} REFRESH_NF;

/* Flag bits in the exchange summary message */

#define EXA_LAST_SALE		0x00000001L
#define EXA_OPEN			0x00000002L
#define EXA_HIGH			0x00000004L
#define EXA_LOW				0x00000008L
#define EXA_CLOSE			0x00000010L
#define EXA_BID				0x00000020L
#define EXA_ASK				0x00000040L
#define EXA_OPEN_INTEREST	0x00000080L
#define EXA_VOLUME			0x00000100L
#define EXA_COMPOSITE		0x00000200L 
#define EXA_REGIONAL		0x00000400L 
#define EXA_OPENRANGE		0x00000800L
#define EXA_CLOSERANGE		0x00001000L
#define EXA_PREVBID			0x00002000L
#define EXA_PREVASK			0x00004000L
#define EXA_ISBBO			0x00008000L
#define EXE_COMPOSITE_OPEN  0x00010000L
#define EXE_COMPOSITE_HIGH  0x00020000L
#define EXE_COMPOSITE_LOW   0x00040000L
#define EXA_SETTLEMENT		0x00080000L 
#define EXA_EXPLICIT_STRIKE	0x00100000L 
#define EXA_UPD_FULL_PRICE	0x00200000L 
#define EXA_MID_PRICE		0x08000000L
#define EXA_QUOTE_INDICATOR	0x10000000L
#define EXA_TRADE_INDICATOR	0x20000000L
#define EXA_RESTRICTED		0x40000000L
#define EXA_BANKRUPTCY		0x80000000L

typedef struct _ETI_SUMMARY_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	lastSale	  	: 1;		// packet has a last sale
	BIT_FIELD	open 		  	: 1;		// packet has an open
	BIT_FIELD	high 		  	: 1;		// packet has a high
	BIT_FIELD	low			 	: 1;		// packet has a low
	BIT_FIELD	close		  	: 1;		// packet has a close
	BIT_FIELD	bid			 	: 1;		// packet has a bid
	BIT_FIELD	ask			 	: 1;		// packet has an ask
	BIT_FIELD	openInterest	: 1;		// packet has an open interest
	BIT_FIELD	volume		 	: 1;		// packet has a volume
	BIT_FIELD	updComposite 	: 1;		// update composite record (used?)
	BIT_FIELD	updRegional  	: 1;		// update regional record (used?)
	BIT_FIELD	openRange		: 1;		// packet has an open range (EX_SUMMARY only)
	BIT_FIELD	closeRange		: 1;		// packet has an closing range (EX_SUMMARY only)
	BIT_FIELD	prevBid			: 1;		// packet has a prevBid
	BIT_FIELD	prevAsk			: 1;		// packet has an prevAsk
	BIT_FIELD	isBBO			: 1;
	BIT_FIELD	compositeOpen	: 1;
	BIT_FIELD	compositeHigh	: 1;
	BIT_FIELD	compositeLow	: 1;
	BIT_FIELD	settlement 		: 1;	
	BIT_FIELD	explicitStrike	: 1;		// packet has an explicit strike
	BIT_FIELD	updFullPrice	: 1;		// Update the entire price Structure for high,low,close,open
	BIT_FIELD	reserved2	 	: 5;	
	BIT_FIELD   midPrice		: 1;
	BIT_FIELD	quoteIndicator  : 1;		// packet has a quote indicator
	BIT_FIELD	tradeIndicator  : 1;		// packet has a trade indicator
	BIT_FIELD	restricted   	: 1;		// restricted (options)
	BIT_FIELD	bankruptcy   	: 1;		// bankruptcy
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	bankruptcy   	: 1;		
	BIT_FIELD	restricted   	: 1;		
	BIT_FIELD	tradeIndicator  : 1;		// packet has a trade indicator
	BIT_FIELD	quoteIndicator  : 1;		// packet has a quote indicator
	BIT_FIELD   midPrice		: 1;
	BIT_FIELD	reserved2	 	: 5;	
	BIT_FIELD	updFullPrice	: 1;		// Update the entire price Structure for high,low,close,open
	BIT_FIELD	explicitStrike	: 1;		// packet has an explicit strike
	BIT_FIELD	settlement 		: 1;	
	BIT_FIELD	compositeLow	: 1;
	BIT_FIELD	compositeHigh	: 1;
	BIT_FIELD	compositeOpen	: 1;
	BIT_FIELD	isBBO			: 1;
	BIT_FIELD	prevAsk			: 1;
	BIT_FIELD	prevBid			: 1;		
	BIT_FIELD	closeRange		: 1;		// packet has an closing range (EX_SUMMARY only)
	BIT_FIELD	openRange		: 1;		// packet has an open range (EX_SUMMARY only)
	BIT_FIELD	updRegional  	: 1;		
	BIT_FIELD	updComposite 	: 1;		
	BIT_FIELD	volume		 	: 1;		
	BIT_FIELD	openInterest	: 1;		
	BIT_FIELD	ask			 	: 1;		
	BIT_FIELD	bid			 	: 1;		
	BIT_FIELD	close		  	: 1;		
	BIT_FIELD	low			 	: 1;		
	BIT_FIELD	high 		  	: 1;		
	BIT_FIELD	open 		  	: 1;		
	BIT_FIELD	lastSale	  	: 1;		
#endif
} ETI_SUMMARY_BITS;

/* Flags for the Exchange Summary message */

typedef union _ETI_SUMMARY_FLAGS {
	ETI_SUMMARY_BITS	bits;
	ULONG 				mask;
} ETI_SUMMARY_FLAGS;	// 4 bytes

typedef struct _EXA_PRICE {
	LONG	price;
	ULONG	size;
} EXA_PRICE;

/* Packet Type MT_EXA_SUMMARY */

typedef struct _EXA_SUMMARY {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;  
	ETI_SUMMARY_FLAGS	flags; 
	USHORT				priceType;
	USHORT				securityCategory;
	EXA_PRICE			lastSale;
	EXA_PRICE			open;    
	EXA_PRICE			high;    
	EXA_PRICE			low;	   
	EXA_PRICE			close;   
	EXA_PRICE			bid;	   
	EXA_PRICE			ask;	   
	ULONG 				openInterest;
	ULONG 				volume;
	USHORT				quoteIndicator;
	USHORT				tradeIndicator;
} EXA_SUMMARY; 

/* Packet Type ETI_MT_EX_SUMMARY */

typedef struct _ETI_EX_SUMMARY {
	ETI_PACKET_HEADER	header;			// 26 bytes
	ETI_KEY				item; 			// 22 bytes
	ETI_SUMMARY_FLAGS	flags;			//  4 bytes
	USHORT				priceType;
	USHORT				displayPriceType;
	DBA_PRICE			lastSale;		// 12 bytes
	DBA_PRICE			open; 			// 12 bytes
	DBA_PRICE			high; 			// 12 bytes
	DBA_PRICE			low;			// 12 bytes
	DBA_PRICE			close;			// 12 bytes
	DBA_PRICE			bid;			// 12 bytes
	DBA_PRICE			ask;			// 12 bytes
	ULONG 				openInterest;
	ULONG 				volume;
	USHORT				securityCategory;
	USHORT				reserved;
	ULONG				openRange;
	ULONG				closeRanges[2];
	ULONG				explicitStrike;
	USHORT				strikePriceType;
} ETI_EX_SUMMARY; 					// 164 bytes

/* Packet Type ETI_MT_SHORT_SUMMARY */ 

typedef struct _ETI_SHORT_SUMMARY { // "Short" summary.
	ETI_PACKET_HEADER	header;			// 26 bytes
	ETI_KEY				item; 			// 20 bytes
	ETI_SUMMARY_FLAGS	flags;			// 4 bytes
	USHORT				priceType;		// 2 bytes
	USHORT				displayPriceType;
	LONG				value;			// 4 bytes
	USHORT				securityCategory;
	USHORT				reserved;
} ETI_SHORT_SUMMARY;   				// 62 bytes

/* Packet Type ETI_TRANSPORT */ 					 		// Transparent transport message

#define TRANSPORT_ETI		0
#define TRANSPORT_WTI		1

typedef struct _ETI_TRANSPORT {
	ETI_PACKET_HEADER header;			// 26 bytes
	USHORT            type;
	USHORT            reserved;
	UCHAR 			  data[994];
} ETI_TRANSPORT;						// 1024 bytes

/* Packet Type MT_RANGE */

#define ETI_OPEN_RANGE		0x0001
#define ETI_CLOSE_RANGE		0x0002

typedef struct _ETI_RANGE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD  opening		: 1;
	BIT_FIELD  closing		: 1;
	BIT_FIELD  filtered		: 1;
	BIT_FIELD  reserved		:13;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD  reserved		:13;
	BIT_FIELD  filtered		: 1;
	BIT_FIELD  closing		: 1;
	BIT_FIELD  opening		: 1;
#endif
} ETI_RANGE_BITS;	// 2 bytes

typedef union _ETI_RANGE_FLAGS {
	ETI_RANGE_BITS	bits;
	USHORT			mask;
} ETI_RANGE_FLAGS;	// 2 bytes

typedef struct _ETI_RANGE {
	ETI_PACKET_HEADER 	header;
	ETI_KEY				item;
	ETI_RANGE_FLAGS		rangeFlags;
	USHORT				priceType;
	USHORT				securityCategory;
	USHORT				rangeIndicator;
	LONG				price1;
	LONG				price2;
	LONG				size;
	LONG				volume;
} ETI_RANGE;

/* packet Type MT_LS_DELETE */
/* packet Type MT_SP_DELETE */
/* packet Type MT_NF_DELETE */

typedef struct _DELETE_EX {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
} DELETE_EX;

#define DELETE_SP  DELETE_EX
#define DELETE_NF  DELETE_EX

/* packet Type MT_ALERT_LINE */

typedef struct _ALERT_LINE {
	ETI_PACKET_HEADER	header;
	USHORT				address[4];
	USHORT				subAddress[4];
	USHORT				beepCount;
	USHORT				textLength;
	UCHAR				text[80];
} ALERT_LINE;

/* packet Type MT_VERSION */

typedef struct _HYPER_VERSION {
	ETI_PACKET_HEADER	header;
	USHORT 				number;
} HYPER_VERSION;

/* Packet Type MT_BBO_QUOTE */

typedef struct _ETI_BBO_QUOTE {
	ETI_PACKET_HEADER 	header;
	ETI_KEY				item;
	union {
		struct {
		#if defined( PQTENDIAN_LITTLE )
			BIT_FIELD			intraDay 		:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD			compositeOpen	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD			updateVolume	:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			zeroBidAsk		:1;
			BIT_FIELD			bboNoBbo 		:1;	/* item has no bbo		 */
			BIT_FIELD			bboNoChange 	:1;	/* quote does not affect the BBO  */
			BIT_FIELD			bboIsBbo 		:1;	/* quote is the BBO (no appendage) */
			BIT_FIELD			bboAppendage	:1;   /* Valid BBO appendage		 */
		#elif defined( PQTENDIAN_BIG )
			BIT_FIELD			bboAppendage	:1; 
			BIT_FIELD			bboIsBbo 		:1;	
			BIT_FIELD			bboNoChange 	:1;	
			BIT_FIELD			bboNoBbo 		:1;	
			BIT_FIELD			zeroBidAsk		:1;
			BIT_FIELD			updateLast		:1;
			BIT_FIELD			updateOhl		:1;
			BIT_FIELD			updateVolume	:1;
			BIT_FIELD			compositeHigh	:1;
			BIT_FIELD			compositeLow	:1;
			BIT_FIELD			compositeOpen	:1;
			BIT_FIELD			filtered 		:1;
			BIT_FIELD			regionalHigh	:1;
			BIT_FIELD			regionalLow 	:1;
			BIT_FIELD			regionalOpen	:1;
			BIT_FIELD			intraDay 		:1;
		#endif
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined (__unix)
			} bits;
		#else
			};
		#endif
		USHORT  mask;
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (_IBMR2) || defined (__BORLANDC__) || (_MSC_VER > 800) || defined (__unix)
			} flag1;
		#else
			};
		#endif

	USHORT				priceType;
	USHORT				quoteIndicator;
	USHORT				securityCatagory;
	LONG				bidPrice;
	LONG				bidSize;
	LONG				askPrice;
	LONG				askSize;
	LONG				bboBidPrice;
	LONG				bboBidSize;
	LONG				bboAskPrice;
	LONG				bboAskSize;
	UCHAR 				askExchangeCode[2];
	UCHAR 				bboBidExchangeCode[2];
	UCHAR 				bboAskExchangeCode[2];
	ETI_QUOTE_FLAGS		flags;
	ETI_QUOTE_FLAGS2	flags2; // Not currently on feed (7/31/02)
	ETI_TRANSACT_FLAGS	tflags;
} ETI_BBO_QUOTE;

/* packet Type MT_TS_CORRECT */

#define TS_CORRECT_FLAGS_START	1
#define TS_CORRECT_FLAGS_END	2


/*
** The DBA_TS_REC structure isn't byte aligned, and we'd like our record to be aligned.
*/

typedef struct ETI_TS_ITEM {
	ULONG	sequence;
	LONG 	price;
	ULONG 	size;
	UCHAR	type;
	UCHAR	indicator;
	UCHAR	priceType;
	UCHAR 	exchangeCode[2][4];
	UCHAR 	hour;
	UCHAR 	minute;
	UCHAR	second;
	USHORT	reserved;
} ETI_TS_ITEM;  /* 24 bytes */

typedef struct _TS_CORRECT {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
	USHORT				sequence;
	USHORT				reserved;
	USHORT				count;
	ETI_TS_ITEM			data[DBA_TS_MAX_ENTRY];
} TS_CORRECT;

#define TS_CORRECT_MIN_SIZE	( sizeof( TS_CORRECT ) - sizeof((( TS_CORRECT*)0)->data ))

/* packet Type MT_HFT_BAR */

#define BAR_RECTYPE_PRICE		0	// Price record

#define BAR_RECTYPE_TRADE		0	// Trade record
#define BAR_RECTYPE_QUOTE		1	// Quote record
#define BAR_RECTYPE_MMQUOTE		3	// Level II Quote record
#define BAR_RECTYPE_TRADE_ID	5	// Buyer / Seller ID in Exchange Field

#define BAR_RECTYPE_SPLIT		16	// Split Record
#define BAR_RECTYPE_SYMBOL		17	// Symbol Change
#define BAR_RECTYPE_DIVIDEND	18	// Dividend 

#define BAR_RECTYPE_DELETED		0x7f	// Deleted

typedef struct _BAR_RECS 
{										// Record Type == BAR_RECTYPE_PRICE
	CHAR		type; 					// = BAR_RECTYPE_PRICE
	UCHAR		priceType;				// HyperFeed price type
	ULONG		time; 					// Bar begin time
	LONG		start; 					// Bar Start price, by priceType
	LONG		high; 					// Bar High price price, by priceType
	LONG		low;					// Bar Low price, by priceType
	LONG		last; 					// Bar Close price, by priceType
	LONG		volume;					// Session volume in 100s shares
	CHAR		unused[6];				// Fill out to 32 bytes
} BAR_RECS;

typedef struct _BAR_SPLIT
{										// Record Type == BAR_RECTYPE_SPLIT
	CHAR		type; 					// = BAR_RECTYPE_SPLIT
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	LONG		numerator;				// Numerator of Split ratio.
	LONG		denominator; 			// Demoninator of Split ratio
	ULONG		timeOfLastSplit;		// time last split was recorded
	ULONG		timeOfLastDividend;		// time last dividend was recorded
	CHAR		padding[10];			// Fill out to 32 bytes
} BAR_SPLIT;

typedef struct _BAR_DIVIDEND
{										// Record Type == BAR_RECTYPE_DIVIDEND
	CHAR		type; 					// = BAR_RECTYPE_SPLIT
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	ULONG		reserved;
	LONG		dividend;				// Divicend amount.
	ULONG		timeOfLastSplit;		// time last split was recorded
	ULONG		timeOfLastDividend;		// time last dividend was recorded
	CHAR		padding[10];			// Fill out to 32 bytes
} BAR_DIVIDEND;

typedef struct _BAR_SYMBOL
{										// Record Type == BAR_RECTYPE_SYMBOL
	CHAR		type; 					// = BAR_RECTYPE_SYMBOL
	UCHAR 		priceType;				// HyperFeed price type
	ULONG		time; 					// End of session time
	DBA_KEY		newKey;
	CHAR		padding[4];				// Fill out to 32 bytes
} BAR_SYMBOL;

typedef struct _BAR_TOS_TRADE_BITS 
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			intraDay 		:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD		 	compositeOpen	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD 			updateVolume	:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			unused			:4;
#elif defined( PQTENDIAN_BIG ) 
	BIT_FIELD			unused			:4;
	BIT_FIELD			cancel			:1;
	BIT_FIELD			updateLast		:1;
	BIT_FIELD			updateOhl		:1;
	BIT_FIELD 			updateVolume	:1;
	BIT_FIELD			compositeHigh	:1;
	BIT_FIELD			compositeLow	:1;
	BIT_FIELD		 	compositeOpen	:1;
	BIT_FIELD			filtered 		:1;
	BIT_FIELD			regionalHigh	:1;
	BIT_FIELD			regionalLow 	:1;
	BIT_FIELD			regionalOpen	:1;
	BIT_FIELD			intraDay 		:1;
#endif
} BAR_TOS_TRADE_BITS;

typedef union _BAR_TOS_TRADE_FLAGS 
{
	BAR_TOS_TRADE_BITS	bits;
	USHORT				mask;
} BAR_TOS_TRADE_FLAGS;

typedef struct _BAR_TOS_TRADE 
{
	CHAR				type;					// 0 = trade
	UCHAR				priceType;				// HyperFeed price type
	ULONG				time;					// time
	LONG				price;					// last price
	LONG				size;					// last size
	UCHAR				exchangeCode[2];		// exchange code
	BAR_TOS_TRADE_FLAGS	flags;					// trade flags
	USHORT				buyerID;				// buyer ID
	USHORT				sellerID;				// seller ID
	UCHAR				reserved[6];
	UCHAR				indicator;				// sale or quote indicator;
	UCHAR				sequence[3];
} BAR_TOS_TRADE;

typedef struct _BAR_TOS_QUOTE_BITS 
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD			zeroBidAsk		:1;
	BIT_FIELD			bboNoBbo 		:1;	
	BIT_FIELD			bboNoChange 	:1;	
	BIT_FIELD			bboIsBbo 		:1;	
	BIT_FIELD			unused			:4;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD			unused			:4;
	BIT_FIELD			bboIsBbo 		:1;
	BIT_FIELD			bboNoChange 	:1;
	BIT_FIELD			bboNoBbo 		:1;
	BIT_FIELD			zeroBidAsk		:1;
#endif
} BAR_TOS_QUOTE_BITS;

typedef union _BAR_TOS_QUOTE_FLAGS 
{
	BAR_TOS_QUOTE_BITS	bits;
	UCHAR				mask;
} BAR_TOS_QUOTE_FLAGS;

typedef struct _BAR_TOS_QUOTE 
{
	CHAR					type;					// 1 = quote
	UCHAR					priceType;				// HyperFeed price type
	ULONG					time;					// time
	LONG					bidPrice;				// bid price
	LONG					bidSize;				// bid size
	LONG					askPrice;				// ask price
	LONG					askSize;				// ask size
	UCHAR					bidExchangeCode[2];		// bid exchange code
	UCHAR					askExchangeCode[2];		// ask exchange
	BAR_TOS_QUOTE_FLAGS		flags;
	UCHAR					indicator;				// sale or quote indicator;
	UCHAR					sequence[3];
} BAR_TOS_QUOTE;

typedef struct _BAR_TOS_MMQUOTE 
{
	CHAR					type;					// 2 = level 2 quote
	UCHAR					priceType;				// HyperFeed price type
	ULONG					time;					// time
	LONG					bidPrice;				// bid price
	LONG					bidSize;				// bid size
	LONG					askPrice;				// ask price
	LONG					askSize;				// ask size
	UCHAR					mmid[4];				// market maker id
	UCHAR					unused[2];				// reserved for mmquote flags.
	UCHAR					indicator;				// sale or quote indicator.
	UCHAR					sequence[3];
} BAR_TOS_MMQUOTE;

typedef union _BAR_DATA 
{
	BAR_RECS			bar;
	BAR_TOS_TRADE		trade;
	BAR_TOS_QUOTE		quote;
	BAR_TOS_MMQUOTE		mmquote;
	BAR_SPLIT 			split;
	BAR_SYMBOL			symbol;
	BAR_DIVIDEND		dividend;
} BAR_DATA;

#define BAR_UPDATE				0x0000001
#define BAR_INSERT				0x0000002
#define BAR_DELETE				0x0000004

 
typedef struct _BAR_BITS 
{
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		update			: 1;
	BIT_FIELD		insert			: 1;
	BIT_FIELD		barDelete		: 1;
	BIT_FIELD		unused			: 14;
	BIT_FIELD		unused2			: 15;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		unused2			: 15;
	BIT_FIELD		unused			: 14;
	BIT_FIELD		barDelete		: 1;
	BIT_FIELD		insert			: 1;
	BIT_FIELD		update			: 1;
#endif
} BAR_BITS;

typedef union _BAR_FLAGS {
	BAR_BITS 	bits;
	ULONG		mask;
} BAR_FLAGS;


#define BAR_TYPE_BAR		0
#define BAR_TYPE_TOS		1
#define BAR_TYPE_HISTORY	2
 
typedef struct _HFT_BAR  {
	ETI_PACKET_HEADER	header;  //26 bytes
	ETI_KEY				item;	//22 bytes
	BAR_FLAGS			flags;  //4 bytes
	ULONG				nRecs;	//4 bytes
	ULONG				barType; //4 bytes
	BAR_DATA			barData;  //32 bytes
} HFT_BAR;


#define HTI_ORDER_SOURCE_ID_ISLAND  "ISLAND"
#define HTI_ORDER_SOURCE_ID_REDIB  "REDIB"
#define HTI_ORDER_SOURCE_ID_ADAP "NASDAQ"
#define HTI_ORDER_SOURCE_ID_NYSE "NYSE"
#define HTI_ORDER_SOURCE_ID_DEPTH "DEPTH"
#define HTI_ORDER_SOURCE_ID_ARCHA "ARCHA"

// For Prime the source ID becomes the Market Maker.

#define HTI_ORDER_PRODUCT_ID_ECN "EC"
#define HTI_ORDER_PRODUCT_ID_ADAP "AD"
#define HTI_ORDER_PRODUCT_ID_PRIME "PR"
#define HTI_ORDER_PRODUCT_ID_OPEN_BOOK "NY"
#define HTI_ORDER_PRODUCT_ID_DEPTH "DB"


#define	ORDER_TYPE_NEW               0
#define	ORDER_TYPE_PART              1
#define	ORDER_TYPE_FULL              2
#define	ORDER_TYPE_CANCEL            3
#define	ORDER_TYPE_TIMED_OUT         4
#define	ORDER_TYPE_TOP_OF_BOOK       5
#define	ORDER_TYPE_SECURITY_HALT     6
#define	ORDER_TYPE_SECURITY_RESUME   7
#define	ORDER_TYPE_UPDATE			 8
#define	ORDER_TYPE_DELETE_BOOK		 9
#define	ORDER_TYPE_REFRESH			10
#define	ORDER_TYPE_REFRESH_DELETE	11
#define	ORDER_TYPE_DELETE_BOOK_BY_SYMBOL 12

#define	ORDER_TYPE_SELL  0
#define	ORDER_TYPE_BUY   1
#define	OF_ORDER_UPDATE_BUY_SELL		0x000001L
#define	OF_ORDER_UPDATE_PRICE			0x000004L
#define	OF_ORDER_UPDATE_SIZE			0x000008L
#define	OF_ORDER_REDUCE_SIZE			0x000010L
#define OF_ORDER_UPDATE_NUMORDERS		0x000020L
#define OF_ORDER_UPDATE_OPEN_INDICATOR	0x000040L

#define ORDER_TIME_IN_FORCE_DAY					0
#define ORDER_TIME_IN_FORCE_GTC					1
#define ORDER_TIME_IN_FORCE_OPENING				2
#define ORDER_TIME_IN_FORCE_IMMEDIATE_OR_CANCEL	3
#define ORDER_TIME_IN_FORCE_FILL_OR_KILL		4
#define ORDER_TIME_IN_FORCE_GOOD_TILL_CROSSING	5
#define ORDER_TIME_IN_FORCE_TILL_DATE			6

typedef struct _ORDER_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		buySell			: 1;  //0 Sell 1=Buy
	BIT_FIELD		negotiable		: 1;
	BIT_FIELD		updatePrice		: 1;
	BIT_FIELD		updateSize		: 1;    // replace existing size with this size
	BIT_FIELD		reduceSize		: 1;	// subtract size from current size
	BIT_FIELD		updateNumOrders : 1;
	BIT_FIELD		openIndicator	: 1;
	BIT_FIELD		unused          : 9;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		unused          : 9;
	BIT_FIELD		openIndicator	: 1;
	BIT_FIELD		updateNumOrders : 1;
	BIT_FIELD		reduceSize		: 1;
	BIT_FIELD		updateSize		: 1;
	BIT_FIELD		updatePrice		: 1;
	BIT_FIELD		negotiable		: 1;
	BIT_FIELD		buySell			: 1;  //0 Sell 1=Buy
#endif
} ORDER_BITS;

typedef union _ORDER_FLAGS {
	ORDER_BITS		bits;
	USHORT          mask;
} ORDER_FLAGS;            // 2 bytes


typedef struct _ORDER_OPEN {  // use for new, update and refresh
   ULONG    orderTime;        //  4 bytes
   LONG     price;            //  4 bytes
   LONG     size;             //  4 bytes
   LONG		numOrders;        //  4 bytes
   ULONG    expireTime;       //  4 bytes
   ULONG	timeInForce;	  //  4 bytes
   USHORT   priceType;        //  2 bytes
   UCHAR	indicator;		 //  1 byte
   UCHAR	reserved;		//  1 byte
} ORDER_OPEN;				  // 28 bytes

typedef struct _ORDER_CANCEL {
   ULONG    orderTime;        //  4 bytes
	} ORDER_CANCEL;				  // 4 bytes

typedef struct _ORDER_TOP_OF_LIMIT {
   LONG     buyPrice;               //  4 bytes
   LONG     buySize;                //  4 bytes
   LONG     sellPrice;              //  4 bytes
   LONG     sellSize;               //  4 bytes
   USHORT   buyPriceType;           //  2 bytes
   USHORT   sellPriceType;          //  2 bytes
   UCHAR	reserved[4];			//  4 bytes
} ORDER_TOP_OF_LIMIT;		        // 24 bytes

typedef union _ORDER_MSG {
	ORDER_OPEN          open;
	ORDER_CANCEL        cancel;
	ORDER_TOP_OF_LIMIT  topOfLimit;
} ORDER_MSG; 			

// Old order structure
typedef struct _ORDER_OLD {
	ETI_PACKET_HEADER 	header;			// 26 bytes
	ETI_KEY				item;			// 22 bytes
	CHAR				ecnID[8];		// 8 bytes 	
	CHAR				entryID[16];	// 16 bytes
	ORDER_FLAGS			flags;			//  2 bytes
	USHORT				msgType;		//  2 bytes
	ORDER_MSG			msg;			// 28 bytes
} ORDER_OLD; 							// 104 bytes

// Order Structure changed for NASDAQ Super Montage 
// The size remains the same.
typedef struct _ORDER {
	ETI_PACKET_HEADER 	header;			// 26 bytes
	ETI_KEY				item;			// 22 bytes
	CHAR				productCode[2];	// 2 bytes 	
	CHAR				sourceID[6];	// 6 bytes 	
	HTI_ENTRY_ID		entryID;		// 16 bytes
	ORDER_FLAGS			flags;			//  2 bytes
	USHORT				msgType;		//  2 bytes
	ORDER_MSG			msg;			// 28 bytes
} ORDER;								// 104 bytes

/* packet Type MT_MARKET_QUOTE */

#define	MARKET_QUOTE_BID_PRICE        0x0001
#define	MARKET_QUOTE_ASK_PRICE        0x0002
#define	MARKET_QUOTE_CANCEL           0x0004
#define	MARKET_QUOTE_OPEN_INDICATOR   0x0008
#define	MARKET_QUOTE_RESTRICTED       0x0010
#define	MARKET_QUOTE_BID_TICK_PRESENT 0x0020
#define	MARKET_QUOTE_BID_DOWN_TICK    0x0040
#define	MARKET_QUOTE_NO_TICK		  0x0080
#define	MARKET_QUOTE_SOFT_DELETE	  0x0100
#define	MARKET_QUOTE_HARD_DELETE	  0x0200

typedef struct _ETI_MARKET_QUOTE_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD	bidPrice       		: 1;
	BIT_FIELD	askPrice       		: 1;
	BIT_FIELD	cancel       		: 1;
	BIT_FIELD	openIndicator		: 1;
	BIT_FIELD	restricted			: 1;
	BIT_FIELD	bidTickPresent		: 1;
	BIT_FIELD	bidDownTick			: 1;
	BIT_FIELD	noTick				: 1;
	BIT_FIELD	softDelete			: 1;
	BIT_FIELD	hardDelete			: 1;
	BIT_FIELD	undefined			: 6;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD	undefined			: 6;
	BIT_FIELD	hardDelete			: 1;
	BIT_FIELD	softDelete			: 1;
	BIT_FIELD	noTick				: 1;
	BIT_FIELD	bidDownTick			: 1;
	BIT_FIELD	bidTickPresent		: 1;
	BIT_FIELD	restricted			: 1;
	BIT_FIELD	openIndicator		: 1;
	BIT_FIELD	cancel       		: 1;
	BIT_FIELD	askPrice       		: 1;
	BIT_FIELD	bidPrice       		: 1;

#endif
} ETI_MARKET_QUOTE_BITS;

typedef union _ETI_MARKET_QUOTE_FLAGS {
	ETI_MARKET_QUOTE_BITS	bits;
	USHORT				mask;
} ETI_MARKET_QUOTE_FLAGS;	// 2 bytes


typedef struct _MARKET_QUOTE {
	ETI_PACKET_HEADER	header;			//26 Bytes
	ETI_KEY			item;				//22 Bytes
	ETI_MARKET_QUOTE_FLAGS	flags;			// 2 Bytes
	USHORT			securityCategory;	// 2 Bytes
	UCHAR			bidQuoteIndicator;	// 1 Byte
	UCHAR			bidXpressIndicator;	// 1 Byte
	DBA_EXT_PRICE	bid;				// 16 bytes
	DBA_EXT_PRICE	ask;				// 16 bytes
	UCHAR			askQuoteIndicator;	// 1 Byte
	UCHAR			askXpressIndicator;	// 1 Byte
} MARKET_QUOTE;							// 88 bytes


/* packet Type MT_UNDEFINED */

typedef struct _UNDEFINED_ETI {
	ETI_PACKET_HEADER	header;
	USHORT				dataLength;
	UCHAR 				data[996];
} UNDEFINED_ETI;	/* 1024 bytes total */

//      flags for ETI Partial Processes

#define F52_NAME_CHG       0x0010
#define F52_DIV_CHG        0x0020
#define F52_HIST_HIGH_CHG  0x0040
#define F52_HIST_LOW_CHG   0x0080
#define F52_ID_NUMBER_CHG  0x0100

#define F52_UPD_MASK        0xfff0
typedef struct _ETI_PARTIAL_ENTRY {
	USHORT	type;
	USHORT	offset;						// The offset from the beginning of the NEXT element;
	USHORT	length;						// The length of the data in this element;
	CHAR	data[1];					// The data itself;
} ETI_PARTIAL_ENTRY;

/*
** The list of all defined type values for a ETI_PARTIAL_ENTRY:
*/
#define PARTIAL_INVALID			0
#define PARTIAL_52WEEK_HIGH		1
#define PARTIAL_52WEEK_LOW		2
#define PARTIAL_VEHICLE_NAME	3
#define PARTIAL_DIVIDEND		4
#define PARTIAL_ID_NUMBER		5 // Cusip, ISIN, sedol, etc.

/*
** The ETI_PARTIAL message type gives the funnel the ability to send individual
** data items to the database or further downstream.  The data portion is a stream of 
** ETI_PARTIAL_ENTRY's.  The ETI_PARTIAL_ENTRY type is any of the PT_ values, the length
** is the length of the data of this entry, and the offset is the location of the 
** NEXT ETI_PARTIAL_ENTRY as an offset from the beginning of the data element
**(i.e. &ETI_PARTIAL.data).
*/
typedef union _ETI_PARTIAL_PAYLOAD {
	ETI_PARTIAL_ENTRY	entry;
	UCHAR				data[974];
} ETI_PARTIAL_PAYLOAD;

typedef struct _ETI_PARTIAL {
	ETI_PACKET_HEADER	header;			// 26 bytes
	ETI_KEY				item;			// 22 bytes
	CHAR				flags[2];		// 2 bytes;
	ETI_PARTIAL_PAYLOAD	payLoad;
} ETI_PARTIAL;							// 1024 bytes;

typedef union _ETI_DATA {
	TRADE 					trade;
	ETI_RANGE				etiRange;
	QUOTE 					quote;
	TOPTEN					topten;
	MMQUOTE					mmquote;
	REFRESH					refresh;
	REFRESH_MM				refreshMM;
	REFRESH_SP				refreshSP;
	REFRESH_NF				refreshNF;
	EXA_SUMMARY				exaSummary;
	ETI_SHORT_SUMMARY		shortSummary;
	ETI_EX_SUMMARY			exSummary;
	FILE_TRANSFER			fileTransfer;
	NEWS_ALERT				newsAlert;
	NEWS_STORY				newsStory;
	ALERT_LINE				alertLine;
	HOTBOX					hotbox;
	ETI_TRADE_CORRECTION	tradeCorrection;
	MONDO_QUOTE 			mondoQuote;
	DELETE_EX				deleteEX;
	DELETE_MM				deleteMM;
	DELETE_SP				deleteSP;
	DELETE_NF				deleteNF;
	TS_CORRECT				tsCorrect;
	HFT_BAR					bar;
	HYPER_VERSION			version;
	ORDER					order;
	ETI_PARTIAL				partial;
	ETI_HEADER_ITEM			info;
	UNDEFINED_ETI			undefined;
	ETI_BBO_QUOTE			bboQuote;
	MARKET_QUOTE			marketQuote;
} ETI_DATA;

/*	Define Header	*/

typedef struct _ETI_HEADER {
	SHORT	systemYear;				/* current system year						*/
	SHORT systemMonth;				/* current system month						*/
	SHORT systemDay;				/* current system day						*/
	SHORT systemHour;				/*	current system hour						*/
	SHORT systemMinute;		 		/* current system minute					*/
	SHORT systemSecond;			 	/* current system second 					*/
	SHORT systemRemainder;		 	/* current system seconds remainder			*/
	SHORT startYear;				/* year system was started	 				*/
	SHORT startMonth;				/* month system was started 				*/
	SHORT startDay;				 	/* day system was started 					*/
	SHORT startHour;				/* hour system was started	 				*/
	SHORT startMinute;				/* minute system was started				*/
	SHORT startSecond;				/* second system was started				*/
	SHORT packetRate;				/* current packets per second				*/
	SHORT packetMax;				/* maximum packet rate today				*/
	SHORT packetCount;				/*	packet count this second				*/
	LONG	packetTotal;			/* packets processed today					*/
	ETI_KEY	refreshSymbol;			/* current refresh symbol					*/
	USHORT	refreshCap;				/* times refresh has caped today 			*/
	USHORT	sequenceGaps;			/* sequence gaps							*/
	USHORT	lineErrors[10];			/* communications errors of primary line	*/
	USHORT	line1Errors[10];    	/* communications errors of line a			*/
	USHORT	line2Errors[10];    	/* communications errors of line b			*/
	USHORT	lineSelect;				/* line selected A = 1, B = 2				*/
} ETI_HEADER;	

/* Note: compositePtr and regionalPtr point to buffers that are only */
/* 256 bytes (DBA_USER) in size. 												*/

#if defined (__IBMC__) || defined (__IBMCPP__) 

#define ETI

typedef struct _ETI_TABLE {
	ETI_HEADER	*headerPtr;
	ETI_DATA 	*packetPtr;
	DBA_RECORD	*compositePtr;
	DBA_RECORD	*regionalPtr;
} ETI_TABLE;

//typedef LONG SEGPTR PASCAL FN_ETICALLBACK(LONG status,ETI_TABLE *etiTable VOID *userPtr);
//typedef FN_ETICALLBACK *PFN_ETICALLBACK;

//typedef struct _ETIREGISTER {
//	PFN_ETICALLBACK lpfnEtiProc;
//	VOID *userPtr;
//	} ETIREGISTER;

USHORT  _Pascal eti( VOID *pblk,USHORT func, ETI_TABLE **etiTable );;
//USHORT  _Pascal edi( VOID *pblk,USHORT func, VOID *pVoid );;

#endif

#if( _MSC_VER > 800 ) || defined(__unix)
//#ifdef __NT__

#define ETI

typedef struct _ETI_TABLE {
	ETI_HEADER	*headerPtr;
	ETI_DATA 	*packetPtr;
	DBA_RECORD	*compositePtr;
	DBA_RECORD	*regionalPtr;
	DBA_RECORD	*orgCompositePtr;
	DBA_RECORD	*orgRegionalPtr;
	DBA_RECORD	*countryPtr;
	DBA_RECORD	*exchangePtr;
	DBA_RECORD	*primExchPtr;
} ETI_TABLE;

typedef LONG PASCAL FN_ETICALLBACK(LONG status,ETI_TABLE *etiTable, VOID *userPtr);
typedef FN_ETICALLBACK *PFN_ETICALLBACK;

typedef struct _ETIREGISTER {
	PFN_ETICALLBACK lpfnEtiProc;
	VOID			*userPtr;
} ETIREGISTER;

USHORT  PASCAL eti( VOID *pblk,USHORT func, ETI_TABLE **etiTable );
USHORT  PASCAL edi( VOID *pblk,USHORT func, VOID *pVoid );

#endif

#ifndef ETI

typedef struct _ETI_TABLE {
	ETI_HEADER	SEGPTR * SEG16	headerPtr;
	ETI_DATA SEGPTR * SEG16		packetPtr;
	DBA_RECORD	SEGPTR * SEG16	compositePtr;
	DBA_RECORD	SEGPTR * SEG16	regionalPtr;
} ETI_TABLE;

#ifndef __BORLANDC__
typedef LONG SEGPTR PASCAL FN_ETICALLBACK(LONG status,ETI_TABLE SEGPTR* SEG16 etiTable, VOID SEGPTR *userPtr);

typedef FN_ETICALLBACK *PFN_ETICALLBACK;

typedef struct _ETIREGISTER {
	PFN_ETICALLBACK lpfnEtiProc;
	VOID SEGPTR		*userPtr;
} ETIREGISTER;

#endif

USHORT  _PCQENTRY eti( VOID SEGPTR *pblk, USHORT func, ETI_TABLE SEGPTR * SEGPTR * SEG16 etiTable );
USHORT  _PCQENTRY edi( VOID SEGPTR *pblk, USHORT func, VOID SEGPTR * SEG16 pVoid );
#endif							  

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus		// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		}
	#endif
#endif

#endif

/*=====================================================================*/
