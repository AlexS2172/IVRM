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

#ifndef INCL_ETI
#define INCL_ETI

#ifdef __cplusplus									// C++ compiler is being used
	extern "C" {
#endif

#include <pcqdefs.h>
#include <edbx.h>
#include <indicatr.h>

#pragma pack(2)						 /* Word aligned. */

#define ETI_READ				0
#define ETI_ATTACH			1
#define ETI_DETACH			2
#define ETI_PERIODIC	 		3
#define ETI_QUERY	 			4
#define ETI_UNLOAD			5
#define ETI_HOLD				6
#define ETI_WRITE				7

#define ETI_REGISTER		10
#define ETI_UNREGISTER	11

#define RC_ETI_NOERROR		0
#define RC_ETI_PERIODIC		3
#define RC_ETI_UNLOAD		5
#define RC_ETI_NOMESSAGE	8
#define RC_ETI_INIT			9

#define ERR_ETI_NOT_ATTACHED	0x8000
#define ERR_ETI_QUEUE_FULL		0x8001
#define ERR_ETI_WRITE	 		0x8002
#define ERR_ETI_UNKNOWN			0x8003
#define ERR_ETI_IS_ATTACHED	0x8004
#define ERR_ETI_BUSY				0x8005
#define ERR_ETI_MAX				0x8006

#define HYPER_VER_TRADE_BITS		2           // trade and quote flags fixed
#define HYPER_VER_OHL_FLAGS		3				// open/high/low bit turned on


/*		Define packet Types		 */
/* 	CAUTION:  PCQETIX.EXE must be updated if you add a new message type!! */

#define MT_QUOTE				0				/* QUOTE 			 */
#define MT_TRADE				1				/* TRADE 			 */
#define MT_RESERVED2			2				/* Reserved			 */
#define MT_TOPTEN 			3				/* TOPTEN			 */
#define MT_NEWS_ALERT		4				/* NEWS_ALERT		 */
#define MT_MM_QUOTE			5				/* MMQUOTE			 */
#define MT_BASELINE4 		6				/* REFRESH_SP(dep) */
#define MT_BASELINE5 		7				/* REFRESH_SP(ind) */
#define MT_LS_REFRESH		8				/* REFRESH			 */
#define MT_LS_CORRECTION	9				/* REFRESH			 */
#define MT_LS_SUMMARY		10 			/* REFRESH			 */
#define MT_LS_DELETE 		11 			/* DELETE			 */
#define MT_ALERT_LINE		12 			/* ALERT_LINE		 */
#define MT_FILE_TRANSFER	13 			/* FILE_TRANSFER	 */
#define MT_HOTBOX 			14 			/* HOTBOX			 */
// 								15 Reserved (trade correction)
#define MT_MONDO_QUOTE		16 			/* MONDO_QUOTE 	 */

#define MT_MM_REFRESH		18 			/* REFRESH_MM		 */
#define MT_MM_CORRECTION	19 			/* REFRESH_MM		 */
#define MT_MM_SUMMARY		20 			/* REFRESH_MM		 */
#define MT_MM_DELETE 		21 			/* DELETE_MM		 */

#define MT_SP_REFRESH		22 			/* REFRESH_SP		 */
#define MT_SP_CORRECTION	23 			/* REFRESH_SP		 */
#define MT_SP_SUMMARY		24 			/* REFRESH_SP		 */
#define MT_SP_DELETE 		25 			/* DELETE_SP		 */

#define MT_NF_REFRESH		26 			/* REFRESH_NF		 */
#define MT_NF_CORRECTION	27 			/* REFRESH_NF		 */
#define MT_NF_SUMMARY		28 			/* REFRESH_NF		 */
#define MT_NF_DELETE 		29 			/* DELETE_NF		 */

#define MT_EX_SUMMARY		30 			/* EX_SUMMARY		 */
// 								31 Reserved
#define MT_EXA_SUMMARY		32 			/* EXA_SUMMARY 	 */
#define MT_RANGE				34 			/* RANGE 			 */

#define MT_VERSION			35 			/* HYPER_VERSION	 */
// 								36 Reserved
#define MT_ORDER           37 			// ORDER

#define MT_RESERVED40		40
#define MT_RESERVED41		41
#define MT_RESERVED42		42
#define MT_RESERVED43		43
#define MT_RESERVED44		44
#define MT_RESERVED45		44
#define MT_RESERVED46		46
#define MT_RESERVED47		47
#define MT_RESERVED48		48
#define MT_RESERVED49		49

#define MT_MAX 				36 			// Last valid message

// 							  254 Reserved
#define MT_UNDEFINED 	  255 			/* UNDEFINED_ETI	 */


/*		Define item types		 */

#define IT_EQUITY					0
#define IT_FOREIGN_EXCHG		1
#define IT_FUTURE					2
#define IT_BOND					3
#define IT_EQUITY_OPTION		4
#define IT_FUTURE_OPTION		5
#define IT_INDEX					6
#define IT_DEPOSIT_RATE 		7
#define IT_MUNI_BOND 			8
#define IT_ITEM_ERROR			0xffff

/*		price types		*/

#define PT_LCD_256				0
#define PT_LCD_64					1
#define PT_FUTURE_256			5
#define PT_FUTURE_64				6
#define PT_FUTURE_32				7
#define PT_FUTURE_8				8
#define PT_DECIMAL_0				9
#define PT_LEADING				10
#define PT_DECIMAL_1				11
#define PT_DECIMAL_2				12
#define PT_DECIMAL_3				13
#define PT_DECIMAL_4				14
#define PT_DECIMAL_5				15
#define PT_DECIMAL_6 			16
#define PT_DECIMAL_7 			17
#define PT_DECIMAL_8 			18
#define PT_DECIMAL_9 			19
#define PT_UNDEFINED 			0xffff

/*		Define File Transfer Function Bits	*/

#define FT_CONTINUE					0x20000000L
#define FT_PROCEED_ON_ERROR		0x00000001L
#define FT_BEGIN						0x00000004L
#define FT_END 						0x00000008L

#define FT_OPEN						0x00000010L
#define FT_WRITE						0x00000020L
#define FT_READ						0x00000040L
#define FT_CLOSE						0x00000080L
#define FT_VERIFY						0x00000100L
#define FT_SET_FILE_INFO			0x00000200L
#define FT_DELETE						0x00000400L
#define FT_RENAME						0x00000800L
#define FT_MARK_PASS					0x00001000L
#define FT_RESET						0x00002000L

#define FT_BTR_ABORT					0x00000100L
#define FT_BTR_BEGIN					0x00000200L
#define FT_BTR_END					0x00000400L
#define FT_BTR_CREATE				0x00000800L
#define FT_BTR_OPEN					0x00001000L
#define FT_BTR_CLOSE					0x00002000L
#define FT_BTR_INSERT				0x00004000L
#define FT_BTR_UPDATE				0x00008000L
#define FT_BTR_DELETE				0x00010000L
#define FT_BTR_GET_EQ				0x00020000L
#define FT_BTR_GET_GE				0x00040000L
#define FT_BTR_GET_LE				0x00080000L
#define FT_BTR_GET_HI				0x00100000L
#define FT_BTR_GET_LO				0x00200000L
#define FT_BTR_GET_NEXT				0x00400000L
#define FT_BTR_GET_PREV				0x00800000L
#define FT_BTR_REPEAT_UPDATE		0x01000000L
#define FT_BTR_REPEAT_DELETE		0x02000000L
#define FT_BTR_PRESERVE				0x04000000L
#define FT_BTR_USE_PRESERVE		0x08000000L
#define FT_BTR_VARIABLE_LENGTH	0x10000000L
#define FT_BTR_UPDATE_AT 			0x20000000L

#define FT_CTRL_NONE					0x0000	// no state implied
#define FT_CTRL_MARKET_OPEN		0x0001	// market open
#define FT_CTRL_MARKET_CLOSED		0x0002	// market closed
#define FT_CTRL_ETI_SESS_DOWN		0x0004	// Eti Session down
#define FT_CTRL_ETI_SESS_RESET	0x0008	// Eti Session reset or restored

#define NEWS_NRS						0x0010	/* Dow Jones News Retrieval	*/
#define NEWS_CMN						0x0004	/* Capital Market News 			*/

/* Define File Transfer Mask Bits */

#define FILE_ADMIN					0x0001	/* Originating source messages*/
#define FILE_DB						0x0002	/* Dbax database records.(bpm)*/
#define FILE_FTS						0x0010	/* File Transfer format			*/
#define FILE_BTRIEVE					0x0010	/* Btrieve Transfer format		*/
#define FILE_PASSWORD				0x0200	/* Townsend passwords			*/
#define FILE_SESSION 				0x0400	/* Session (roll) events		*/
#define FILE_CONTROL 				0x0800	/* Admin control message		*/
#define FILE_FILTERED				0x1000	/* Filtered File Download		*/
#define FILE_IMAGE					0x2000	/* Image file download			*/
#define FILE_NEWS						0x4000	/* News download					*/
#define FILE_INDEXED 				0x8000	/* Indexed file download		*/

/*		tick values		*/

#define TICK_LS_UP		0x4000
#define TICK_LS_DOWN		0x8000

/*		Define security catagories */

#define SC_UNDEFINED 	0xffff	// (-1)
#define SC_ALPHA			0
#define SC_BETA			1
#define SC_GAMMA			2
#define SC_DELTA			3
#define SC_EPSILON		4
#define SC_ZETA			5
#define SC_ETA				6	// Underlying of currency option
#define SC_THETA			7	// Money market
#define SC_IOTA			8	// Mutual Funds
#define SC_KAPPA			9	// Emerging Companies
#define SC_LAMBDA 		10 // Exchange Indices
#define SC_MU 				11 // Level1 OTCBB (Pink Sheets)

// "Standard" entitlement masks

#define ETI_TICKER			0x80
#define ETI_RECALL			0x40

// "Simple" entitlement masks

#define ETI_SIMPLE_ENABLED 	0x80	// Feature is enabled
#define ETI_SIMPLE_RESERVED	0x40	// Reserved for backward compatibility.

// "News" entitlement masks

#define ETI_NEWS_ENABLED		0x80	// Feature is enabled
#define ETI_NEWS_RECALL 		0x40	// Reserved for backward compatibility.

#define PCQUOTE_SOURCE 			100
#define PCQUOTE_1400_SOURCE	120
#define PCQUOTE_1500_SOURCE	121
#define PCQUOTE_LOCAL_HOST    0xffff

typedef struct _ETI_PACKET_HEADER {
	ULONG  sequence;				/* PcQuote Sequence number 					*/
	ULONG  time;					/* Packet time 									*/
	ULONG  dataSequence; 		/* Sequence number of orginating source	*/
	USHORT dataModifier;			/* Modifier of data sequence					*/
	USHORT source;					/* Pcquote Source of data						*/
	USHORT dataSource;			/* Orginating Source of data					*/
	USHORT dataGap;       		/* Known gaps from this source				*/
	USHORT type;					/* Packet type										*/
	USHORT row;						/* Row of entitlement							*/
	USHORT column;					/* Column of entitlement						*/
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

/* packet Type MT_MM_QUOTE */

typedef struct _MMQUOTE {
	ETI_PACKET_HEADER	header;
	ETI_MMKEY	item;
	DBA_MASK_5	dbaMask5;
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		quoteIndicator;
	LONG			bidPrice;
	LONG			bidSize;
	LONG			askPrice;
	LONG			askSize;
	} MMQUOTE;

/* packet Type MT_MM_REFRESH		*/
/* packet Type MT_MM_CORRECTION	*/
/* packet Type MT_MM_SUMMARY		*/

typedef struct _REFRESH_MM {
	ETI_PACKET_HEADER	header;
	DBA_RECORD_5		recMM;
	/*
	DBA_MMKEY	item;
	DBA_MASK_5	dbaMask5;
	PCQ_TICK	tick;
	USHORT		priceType;
	USHORT		securityCatagory;
	USHORT		quoteIndicator;
	DBA_PRICE	bid;
	DBA_PRICE	ask;
	BYTE			bidTimeHundredths;
	BYTE			bidTimeSeconds;
	BYTE			askTimeHundredths;
	BYTE			askTimeSeconds;
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

#define	TR_TRADE_TICK					0x0001
#define	TR_DOWN_TICK_REGIONAL		0x0004
#define	TR_DOWN_TICK_COMPOSITE		0x0008

typedef struct _ETI_TRADE_BITS {
	BIT_FIELD  tradeTick		: 1;		   // trade tick is present
	BIT_FIELD  undefined	 	: 1;
	BIT_FIELD  downTickReg	: 1;			// regional is down tick
	BIT_FIELD  downTickComp : 1;        // composite is down tick
	BIT_FIELD  undefined1	:12;
	} ETI_TRADE_BITS;

/* Flags for the Quote message */

typedef union _ETI_TRADE_FLAGS {
	ETI_TRADE_BITS		bits;
	USHORT				mask;
	} ETI_TRADE_FLAGS;				// 2 bytes

/*		Trade Flags		*/

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

/* packet Type MT_TRADE */

typedef struct _TRADE {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	union {
		struct {
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
			BIT_FIELD			bboNoBbo 		:1;	/* item has no bbo		 */
			BIT_FIELD		 	bboNoChange 	:1;	/* quote does not affect the BBO  */
			BIT_FIELD	    	bboIsBbo 		:1;	/* quote is the BBO (no appendage) */
			BIT_FIELD   		bboAppendage	:1;   /* Valid BBO appendage		 */
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || (_MSC_VER > 800) 
			} bits;
		#else
			};
		#endif

		USHORT  mask;
		
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || (_MSC_VER > 800) 
			} flag1;
		#else
			};
		#endif

	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				tradeIndicator;
	LONG					price;
	LONG					size;
	LONG					volume;
	USHORT				sellersDays;
	USHORT				buyersDays;
	ETI_TRADE_FLAGS	flags;
	USHORT reserved;
	LONG					regionalVolume;
	} TRADE;

#define	QT_QUOTE_TICK					0x0001
#define	QT_DOWN_TICK_REGIONAL		0x0004
#define	QT_DOWN_TICK_COMPOSITE		0x0008

typedef struct _ETI_QUOTE_BITS {
	BIT_FIELD  quoteTick		: 1;		   // quote tick is present
	BIT_FIELD  undefined	  	: 1;
	BIT_FIELD  downTickReg	: 1;			// regional bid is down tick
	BIT_FIELD  downTickComp : 1;			// composite bid is down tick
	BIT_FIELD  undefined1	:12;
	} ETI_QUOTE_BITS;

/* Flags for the Quote message */

typedef union _ETI_QUOTE_FLAGS {
	ETI_QUOTE_BITS		bits;
	USHORT				mask;
	} ETI_QUOTE_FLAGS;				// 2 bytes

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
	} ETI_QUOTE_BITS1;

/* Flags for the Quote message */

typedef union _ETI_QUOTE_FLAGS1 {
	ETI_QUOTE_BITS1	bits;
	USHORT				mask;
	} ETI_QUOTE_FLAGS1;				 // 2 bytes

/* packet Type MT_QUOTE */

typedef struct _QUOTE {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	union {
		struct {
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

		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || (_MSC_VER > 800) 
			} bits;
		#else
			};
		#endif
		USHORT  mask;
		#if defined (__IBMC__) || defined (__IBMCPP__) || defined (__BORLANDC__) || (_MSC_VER > 800) 
			} flag1;
		#else
			};
		#endif

	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				quoteIndicator;
	LONG					bidPrice;
	LONG					bidSize;
	LONG					askPrice;
	LONG					askSize;
	LONG					bboBidPrice;
	LONG					bboBidSize;
	LONG					bboAskPrice;
	LONG					bboAskSize;
	UCHAR 				askExchangeCode[2];
	UCHAR 				bboBidExchangeCode[2];
	UCHAR 				bboAskExchangeCode[2];
	ETI_QUOTE_FLAGS	flags;
	} QUOTE;

/* packet Type MT_TOPTEN */

typedef struct _TOPTEN {
	ETI_PACKET_HEADER	header;
	ETI_KEY item;
	ETI_KEY data[10];
	} TOPTEN;

/* packet Type MT_HOTBOX */
 
#define HB_STATS		0x0800
#define HB_DUTIL		0x1000
#define HB_DTRAN		0x2000
#define HB_DJIA		0x8000

typedef struct _HOTBOX {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
	USHORT				priceType;
	USHORT				securityCatagory;
	USHORT				reserved;
	LONG					djia;
	LONG					djiac;
	LONG					dtran;
	LONG					dutil;
	LONG					advn;
	LONG					decn;
	LONG					volm;
	LONG					tick;
	LONG					trin;
	} HOTBOX;

/* Packet Type MT_MONDO_QUOTE */

#define	MQ_FILTERED 					0x01

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
	UCHAR 				expiration; // Expiration Month Code
	UCHAR 				strike;		// Strike Price
	UCHAR 				exchangeCode;
	UCHAR 				flags;
	LONG					bidPrice;
	LONG					askPrice;
	USHORT				bidSize;
	USHORT				askSize;
	} MONDO_ITEM;		// 16 bytes

typedef struct _MONDO_QUOTE {
	ETI_PACKET_HEADER 	header;				// 26 bytes
	ETI_KEY					item; 				// 22 bytes
	ETI_QUOTE_FLAGS1		flag1;				// 2 bytes
	USHORT					priceType;
	USHORT					quoteIndicator;
	USHORT					securityCategory;
	USHORT					count;
	MONDO_ITEM				mondoItem[27]; 	// 27 * 16 bytes
	} MONDO_QUOTE; 	// 490 bytes

/* packet Type MT_LS_REFRESH	  */
/* packet Type MT_LS_CORRECTION */
/* packet Type MT_LS_SUMMARY	  */

typedef struct _SUM_PRICE {
	LONG price;
	LONG size;
	UCHAR exchangeCode[2];
	UCHAR hour;
	UCHAR minute;
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
	ULONG					openRange;
	ULONG					closeRanges[2];
	USHORT				session;
	} REFRESH;

/* packet Type MT_SP_REFRESH	  */
/* packet Type MT_SP_CORRECTION */
/* packet Type MT_SP_SUMMARY	  */

typedef struct _REFRESH_SP {
	ETI_PACKET_HEADER header;
	DBA_RECORD_52		recSP;
	} REFRESH_SP;

/* packet type MT_FILE_TRANSFER */

typedef struct _NEWS_HEADLINE {
	SHORT	storyNumber;
	SHORT	catagoryNumber;
	UCHAR serviceLevel;
	UCHAR priority;
	UCHAR catagory;
	UCHAR reserved;
	UCHAR text[80];
	} NEWS_HEADLINE;

typedef struct _DV_INDEX {
	UCHAR   product[2];
	UCHAR   date[8];
	UCHAR   sequence[4];
	UCHAR   type;
	UCHAR   key[13];
	} DV_INDEX;

typedef struct _DV_TEXT {
	UCHAR   product[2];
	UCHAR   date[8];
	UCHAR   sequenceNumber[4];
	UCHAR   newsSource[4];
	UCHAR   origSource[4];
	UCHAR   tempPerm;
	UCHAR   retention;
	UCHAR   displayDate[6];
	UCHAR   displayTime[4];
	UCHAR   accession[10];
	USHORT  headlineLength;
	USHORT  hot;
	UCHAR   resv[4];
	} DV_TEXT;

typedef struct _FT_CONTROL {
	USHORT  source;
	USHORT  command;
	ULONG   mask;
	USHORT  textLength;
	USHORT  reserved;
	UCHAR   text[168];
	} FT_CONTROL; 							// 180 bytes

#define FTSESSEVENT_BOD1	0		// Beginning of day.
											// 1-7 reserved.
#define FTSESSEVENT_EOD1	8		// End of day.
											// 9-15 reserved.

typedef struct _FTSESS_BITS {
	BIT_FIELD			disabled : 1;	// Session disabled.
	BIT_FIELD			deleted	: 1;	// Session deleted.
	BIT_FIELD			refresh	: 1;	// Database refresh (not live).
	BIT_FIELD			unused	: 13;
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
	UCHAR 			sessionBODs[7];	// # of session starts per day of week.
	UCHAR 			sessionEODs[7];	// # of session ends per day of week.
	UCHAR 			countryCode[2];	// Exchange info (used by tower).
	UCHAR 			exchangeCode[2];
	} FT_SESSION;

typedef struct _FT_PASSWORD {
	UCHAR 			password[32];
	} FT_PASSWORD;

/* Note: File Transfer data size is limited by a maximum total 19.2 */
/* packet size of 256 bytes in the user Dcp drivers _inpxx buffer */
/* (as of 8/19/92). */

typedef union _FT_DATA {
	NEWS_HEADLINE	headline;
	UCHAR 			storyText[180];
	UCHAR 			nrs[180];
	UCHAR 			fts[180];
	DV_TEXT        dvText;
	DV_INDEX       dvIndex;
	UCHAR 			dvData[180];
	FT_CONTROL     control;				// admin control messages
	FT_SESSION		session;
	FT_PASSWORD 	password;			// Townsend passwords
	} FT_DATA;

typedef struct _FILE_TRANSFER {
	ETI_PACKET_HEADER	header;
	USHORT	dataLength;
	ULONG 	function;
	ULONG 	filePointer;
	USHORT	sequenceNumber;
	USHORT	keyPosition;
	USHORT	keyLength;
	USHORT	keyNumber;
	USHORT	namePosition;
	USHORT	nameLength;
	USHORT	address[4];
	USHORT	subAddress[4];
	USHORT	transferMask[4];
	FT_DATA	data;
	} FILE_TRANSFER;

#define OLD_ETI_MAX  (sizeof(FILE_TRANSFER) - sizeof(FT_DATA)) + 250

/* packet Type MT_NEWS_ALERT */

#define	NA_FLAG_BASE_ITEM 		 0x8000

/* Service category News Source defines */

#define	NS_DOWJONES 				 0 	 // (DJ) 	 T
#define	NS_WALLSTREETJOURNAL 	 1 	 // (WSJ)	 J
#define	NS_BARRONS					 2 	 // (BAR)	 B
#define	NS_NEWSRETRIEVAL			 3 	 // (DNR)	 N
#define	NS_DOWJONESWALLSTREET	 4 	 // (DWS)	 W
#define	NS_CAPITALMARKETNEWS 	 5 	 // (CMR)	 C
#define	NS_BUSINESSWIRE			 6 	 // (BW )	 S
#define	NS_COMTEXSCIENTIFIC		 7 	 // (CTX)	 X
#define	NS_AFPDPA					 8 	 // (AFP)	 D
#define	NS_ASSOCIATEDPRESS		 9 	 // (AP )	 E
#define	NS_GARVINGUYBUTLER		10 	 // (GGB)	 G
#define	NS_KYODDNEWSINT			11 	 // (KYO)	 K
#define	NS_PRNEWSWIRE				12 	 // (PR )	 R
#define	NS_UNITEDPRESSINT 		13 	 // (UPI)	 U
#define	NS_SECURITYTRADERS		14 	 // (ST )	 Y
#define	NS_PRNEWSWIREINC			15 	 //			 P
#define	NS_DPA						16 	 // (DPA)	 H
#define	NS_FEDERALNEWS 			17 	 // (FNS)	 V
#define	NS_BUSINESSWIRENEWS		18 	 // (BW )	 Z
#define	NS_INTLDATAGROUP			19 	 // (not implemented)
#define	NS_MUNIFACTS				20 	 // (MFX)	 M
#define	NS_REUTERSDIRECT			21 	 // (RTR)	 A
#define	NS_PROFINVESTOR			22 	 // (PIR)	 I
#define	NS_FIRSTCALLUS 			23 	 // (FC )	 F
#define	NS_FEDERALFILINGS 		24 	 // (FED)	 L
#define	NS_FUTUREWORLD 			25 	 // (FWN)	 1

#define	NS_DJNDV_BT					26		 // DV Broad Tape
#define	NS_DJNDV_CM					27		 // DV Capital Markets
#define	NS_DJNDV_PI					28		 // DV Professional Investor Report
#define	NS_DJNDV_PR					29		 // DV Press Release Wires
#define	NS_DJNDV_DI					30		 // DV International News Wire
#define	NS_DJNDV_FF					31		 // DV Federal Filings
#define	NS_DJNDV_WSJ				32		 // DV Wall Street Journal
#define	NS_DJNDV_FF10				33		 // DV Federal Filings 10K's & 10Q's
#define	NS_DJNDV_ON 				34		 // DV Online News

#define	NS_EXTEL 					64 	 // (EXT)	 E
#define	NS_EXTELWORLDECONEWS 	65 	 // (GEX)	 W
#define	NS_EXTELAFP 				66 	 // (AFX)	 A
#define	NS_VWDNEWS					67 	 // (VWD)	 V
#define	NS_DJIECONEWSWIRE 		68 	 // (DJE)	 I
#define	NS_DJIFINNEWSWIRE 		69 	 // (DJF)	 F
#define	NS_DJIEUROCORPREPT		70 	 // (DJC)	 C
#define	NS_FIRSTCALLEUROPE		71 	 // (FCE)	 B

#define	NS_NIKKEI					96 	 // (NEN)	 N
#define	NS_AUSTRFINRPT 			97 	 // (AFR)	 A
#define	NS_AUSTRAP					98 	 // (AAP)	 P
#define	NS_KYODONEWS				99 	 // (KYO)	 K
#define	NS_FIRSTCALLPACIFIC	  100 	 // (FCP)	 F


#define DELETE_NF  DELETE_EX

typedef struct _NEWS_ALERT {
	ETI_PACKET_HEADER	header;
	ETI_KEY				item;
	USHORT				flags;
	USHORT				serviceCatagory;
	} NEWS_ALERT;

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
#define EXA_OPEN				0x00000002L
#define EXA_HIGH				0x00000004L
#define EXA_LOW				0x00000008L
#define EXA_CLOSE				0x00000010L
#define EXA_BID				0x00000020L
#define EXA_ASK				0x00000040L
#define EXA_OPEN_INTEREST	0x00000080L
#define EXA_VOLUME			0x00000100L
#define EXA_COMPOSITE		0x00000200L
#define EXA_REGIONAL			0x00000400L
#define EXA_RESTRICTED		0x40000000L
#define EXA_BANKRUPTCY		0x80000000L

typedef struct _ETI_SUMMARY_BITS {
	BIT_FIELD  lastSale	  		: 1;		// packet has a last sale
	BIT_FIELD  open 		  		: 1;		// packet has an open
	BIT_FIELD  high 		  		: 1;		// packet has a high
	BIT_FIELD  low			 		: 1;		// packet has a low
	BIT_FIELD  close		  		: 1;		// packet has a close
	BIT_FIELD  bid			 		: 1;		// packet has a bid
	BIT_FIELD  ask			 		: 1;		// packet has an ask
	BIT_FIELD  openInterest 	: 1;		// packet has an open interest
	BIT_FIELD  volume		 		: 1;		// packet has a volume
	BIT_FIELD  updComposite 	: 1;		// update Composite record
	BIT_FIELD  updRegional  	: 1;		// update Regional record
	BIT_FIELD  reserved0	 		: 5;		// reserved
	BIT_FIELD  reserved1	 		: 14;		// reserved
	BIT_FIELD  restricted   	: 1;		// restricted (options)
	BIT_FIELD  bankruptcy   	: 1;		// bankruptcy
	} ETI_SUMMARY_BITS;

/* Flags for the Exchange Summary message */

typedef union _ETI_SUMMARY_FLAGS {
	ETI_SUMMARY_BITS	bits;
	ULONG 				mask;
	} ETI_SUMMARY_FLAGS; 				// 4 bytes

typedef struct _EXA_PRICE {
	LONG price;
	ULONG size;
	} EXA_PRICE;

/* Packet Type MT_EXA_SUMMARY */

typedef struct _EXA_SUMMARY {
	ETI_PACKET_HEADER header;
	ETI_KEY				item;  
	ETI_SUMMARY_FLAGS flags; 
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
	} EXA_SUMMARY; 

/* Packet Type MT_RANGE */

#define ETI_OPEN_RANGE			0x0001
#define ETI_CLOSE_RANGE			0x0002

typedef struct _ETI_RANGE_BITS {
	BIT_FIELD  opening		: 1;
	BIT_FIELD  closing		: 1;
	BIT_FIELD  reserved 		:14;
	} ETI_RANGE_BITS; 					// 2 bytes

typedef union _ETI_RANGE_FLAGS {
	ETI_RANGE_BITS bits;
	USHORT			mask;
	} ETI_RANGE_FLAGS;					// 2 bytes

typedef struct _ETI_RANGE {
	ETI_PACKET_HEADER 	header;
	ETI_KEY					item;
	ETI_RANGE_FLAGS		rangeFlags;
	USHORT					priceType;
	USHORT					securityCategory;
	USHORT					rangeIndicator;
	LONG						price1;
	LONG						price2;
	LONG						size;
	LONG						volume;
	} ETI_RANGE;

/* packet Type MT_LS_DELETE */
/* packet Type MT_SP_DELETE */
/* packet Type MT_NF_DELETE */

typedef struct _DELETE_EX {
	ETI_PACKET_HEADER	header;
	ETI_KEY item;
	USHORT flags;
	} DELETE_EX;

#define DELETE_SP  DELETE_EX
#define DELETE_NF  DELETE_EX

/* packet Type MT_ALERT_LINE */

typedef struct _ALERT_LINE {
	ETI_PACKET_HEADER	header;
	USHORT	address[4];
	USHORT	subAddress[4];
	USHORT	beepCount;
	USHORT	textLength;
	UCHAR text[80];
	} ALERT_LINE;

/* packet Type MT_VERSION */

typedef struct _HYPER_VERSION {
	ETI_PACKET_HEADER header;
	USHORT 				number;
	} HYPER_VERSION;

/* packet Type MT_UNDEFINED */

typedef struct _UNDEFINED_ETI {
	ETI_PACKET_HEADER	header;
	USHORT				dataLength;
	UCHAR 				data[996];
	} UNDEFINED_ETI;		/* 1024 bytes total */

/* ETI_DATA Union of all packet Types */

typedef union _ETI_DATA {
	TRADE 			trade;
	ETI_RANGE		etiRange;
	QUOTE 			quote;
	TOPTEN			topten;
	MMQUOTE			mmquote;
	REFRESH			refresh;
	REFRESH_MM		refreshMM;
	REFRESH_SP		refreshSP;
	REFRESH_NF		refreshNF;
	EXA_SUMMARY		exaSummary;
	FILE_TRANSFER	fileTransfer;
	NEWS_ALERT		newsAlert;
	ALERT_LINE		alertLine;
	HOTBOX			hotbox;
	MONDO_QUOTE 	mondoQuote;
	DELETE_EX		deleteEX;
	DELETE_MM		deleteMM;
	DELETE_SP		deleteSP;
	DELETE_NF		deleteNF;
	HYPER_VERSION	version;
	UNDEFINED_ETI	undefined;
	} ETI_DATA;

/*	Define Header	*/

typedef struct _ETI_HEADER {
	SHORT	systemYear;					/* current system year							*/
	SHORT systemMonth;				/* current system month							*/
	SHORT systemDay;					/* current system day							*/
	SHORT systemHour;					/*	current system hour							*/
	SHORT systemMinute;		 		/* current system minute						*/
	SHORT systemSecond;			 	/* current system second 						*/
	SHORT systemRemainder;		 	/* current system seconds remainder			*/
	SHORT startYear;				 	/* year system was started	 					*/
	SHORT startMonth;					/* month system was started 					*/
	SHORT startDay;				 	/* day system was started 						*/
	SHORT startHour;					/* hour system was started	 					*/
	SHORT startMinute;				/* minute system was started					*/
	SHORT startSecond;				/* second system was started					*/
	SHORT packetRate;					/* current packets per second					*/
	SHORT packetMax;					/* maximum packet rate today					*/
	SHORT packetCount;				/*	packet count this second					*/
	LONG	packetTotal;			 	/* packets processed today						*/
	ETI_KEY	refreshSymbol;			/* current refresh symbol						*/
	USHORT	refreshCap;				/* times refresh has caped today 			*/
	USHORT	sequenceGaps;		   /* sequence gaps									*/
	USHORT	lineErrors[10];		/* communications errors of primary line	*/
	USHORT	line1Errors[10];    	/* communications errors of line a			*/
	USHORT	line2Errors[10];    	/* communications errors of line b			*/
	USHORT	lineSelect;				/* line selected A = 1, B = 2					*/
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

#if ( _MSC_VER > 800 )
//#ifdef __NT__

#define ETI

typedef struct _ETI_TABLE {
	ETI_HEADER	*headerPtr;
	ETI_DATA 	*packetPtr;
	DBA_RECORD	*compositePtr;
	DBA_RECORD	*regionalPtr;
	} ETI_TABLE;

typedef LONG PASCAL FN_ETICALLBACK(LONG status,ETI_TABLE *etiTable, VOID *userPtr);
typedef FN_ETICALLBACK *PFN_ETICALLBACK;

typedef struct _ETIREGISTER {
	PFN_ETICALLBACK lpfnEtiProc;
	VOID *userPtr;
	} ETIREGISTER;

USHORT  PASCAL eti( VOID *pblk,USHORT func, ETI_TABLE **etiTable );;
USHORT  PASCAL edi( VOID *pblk,USHORT func, VOID *pVoid );;

#endif

#ifndef ETI

typedef struct _ETI_TABLE {
	ETI_HEADER	SEGPTR * SEG16 headerPtr;
	ETI_DATA 	SEGPTR * SEG16 packetPtr;
	DBA_RECORD	SEGPTR * SEG16 compositePtr;
	DBA_RECORD	SEGPTR * SEG16 regionalPtr;
	} ETI_TABLE;

#ifndef __BORLANDC__

typedef LONG SEGPTR PASCAL FN_ETICALLBACK(LONG status,ETI_TABLE SEGPTR * SEG16 etiTable, VOID SEGPTR *userPtr);
typedef FN_ETICALLBACK *PFN_ETICALLBACK;

typedef struct _ETIREGISTER {
	PFN_ETICALLBACK lpfnEtiProc;
	VOID SEGPTR *userPtr;
	} ETIREGISTER;

#endif

USHORT  _PCQENTRY eti( VOID SEGPTR *pblk,
							  USHORT func, ETI_TABLE SEGPTR * SEGPTR * SEG16 etiTable );
USHORT  _PCQENTRY edi( VOID SEGPTR *pblk,
							  USHORT func, VOID SEGPTR * SEG16 pVoid );
#endif							  

#pragma pack()

#ifdef __cplusplus									// C++ compiler is being used
		}
#endif

#endif

/*=====================================================================*/


