/*
 *		atadvreq.h		Date=990730
 *
 *		new advanced QS request/response messages
 *
 *		Written 10/23/97	R. Holm
 *
 *		Modified 11/11/97	R. Holm
 *			advise support.
 *
 *		Modified 11/14/97	R. Holm
 *			more TOS support.
 *
 *		Modified 11/17/97	R. Holm
 *			added structure DTDP.
 *
 *		Modified 12/15/97	R. Holm
 *			added adjustments and defines.
 *
 *		Modified 12/17/97	R. Holm
 *			added next key to QSMX_WILDCARD_RESPONSE.
 *
 *		Modified 12/23/97	R. Holm
 *			added fundType to QSMX_RESPONSE.
 *
 *		Modified 01/05/98	R. Holm
 *			fixed up update messages.
 *			repacked and reorganized request messages.
 *
 *		Modified 01/06/98	R. Holm
 *			compression into updates.
 *
 *		Modified 01/07/98	R. Holm
 *			alright these are the updates.
 *
 *		Modified 01/08/98	R. Holm
 *			added QSUX_COMP_DEFAULT.
 *
 *		Modified 01/12/98	R. Holm
 *			boolean flags in ATHIST_KEYFLAGS now have a 'b' to start the message.
 *			HISTKEY_LAST_SECTOR now 0x007fffff.
 *
 *		Modified 01/14/98	R. Holm
 *			new 12 bit tick compression.
 *
 *		Modified 01/16/98	R. Holm
 *			date removed from history response and sector number added.
 *
 *		Modified 01/28/98	R. Holm
 *			'?C' is now GetEqual by CUSIP (not implemented yet!).
 *
 *		Modified 02/18/98	R. Holm
 *			bunched symbol requests now return the DTDP.
 *
 *		Modified 03/30/98	R. Holm
 *			message lengths adjusted to 536.
 *
 *		Modified 04/08/98	R. Holm
 *			added historical database defines, etc.
 *			added ATXREQ_PFI_LONGROOTS.
 *
 *		Modified 04/15/98	R. Holm
 *			QSUX_HEADER changed.
 *
 *		Modified 04/16/98	R. Holm
 *			added source to alert.
 *
 *		Modified 05/28/98	R. Holm
 *			compression levels bumped up 1 to make room for existing compression.
 *
 *		Modified 06/08/98	R. Holm
 *			why is openInterest in a trade?
 *
 *		Modified 06/22/98	R. Holm
 *			added a define for 1 minute bars.
 *			added equity option refresh.
 *
 *		Modified 06/23/98	R. Holm
 *			tradeCount changed to tickCount
 *
 *		Modified 07/01/98	R. Holm
 *			added month day year to history requests.
 *
 *		Modified 07/02/98	R. Holm
 *			added bReverse to history requests.
 *			added a define for 15 minute bars.
 *
 *		Modified 09/11/98	R. Holm
 *			QSUX_UPDATE was a structure not a union!
 *
 *		Modified 09/17/98	R. Holm
 *			QSUX_REFDATA was a structure not a union!
 *
 *		Modified 10/01/98	R. Holm
 *			added ATXREQ_WILD_ROOT.
 *
 *		Modified 10/22/98	R. Holm
 *			maximum TOS/history request data is now 496.
 *
 *		Modified 10/27/98	R. Holm
 *			added new news alert update.
 *
 *		Modified 11/17/98	R. Holm
 *			removed QSUX_COMP_LZO.
 *
 *		Modified 01/10/99	R. Holm
 *			added compressed option database request "?O".
 *
 *		Modified 01/22/99	R. Holm
 *			added "store last" and filtered trade bits.
 *
 *		Modified 02/03/99	R. Holm
 *			added index chain request "?X".
 *
 *		Modified 02/17/99	R. Holm
 *			added ATXREQ_PFI_FIELD_LIST.
 *			added the QSMX_FUND_FIELD structure.
 *
 *		Modified 03/25/99	R. Holm
 *			added halt support.
 *
 *		Modified 03/27/99	R. Holm
 *			halt updates now contain a short halt.
 *
 *		Modified 04/09/99	R. Holm
 *			added HISTKEY_FIRST_SECTOR.
 *
 *		Modified 04/27/99	R. Holm
 *			added QSUX_REFDATA_OPTION::strikePriceType.
 *
 *		Modified 04/28/99	R. Holm
 *			added QSUX_REFDATA_FUTURE::displayPriceType.
 *
 *		Modified 05/19/99	R. Holm
 *			added QSUX_REFDATA_EQUITY::bRestricted.
 *
 *		Modified 06/28/99	R. Holm
 *			added OPTIONCOMP_AT_PRIMARY.
 *
 *		Modified 07/27/99	R. Holm
 *			added ATXREQ_LARGE_BUFFER.
 *			added QSMX_REQUEST_KEY.
 *
 *		Modified 07/30/99	R. Holm
 *			added HISTDBTYPE_VOLATILITY.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-1999.
 *		All rights reserved.
 *
 */

/*=======================================================================*/

#ifndef __ADV_SERVER_REQUESTS
#define __ADV_SERVER_REQUESTS

/*=======================================================================*/

#pragma pack(1)

/*=======================================================================*/

#define ATXREQ_SUCCESS				0		// the request was successful
#define ATXREQ_NOT_FOUND			1		// key not found
#define ATXREQ_INVALID_DATA			2		// invalid data in request
#define ATXREQ_NO_DATABASE			3		// no database
#define ATXREQ_UNKNOWN_REQUEST		4		// unknown request
#define ATXREQ_COMPRESS_ERROR		5		// compression problem
#define ATXREQ_MORE					6		// there is more data
#define ATXREQ_ACCESS_DENIED		7		// access denied

#define ATXREQ_COMP_PB				0		// standard price block format
#define ATXREQ_COMP_BMSG			1		// 'B' message compression
#define ATXREQ_COMP_BMSG_OLD		2		// 'B' message compression with old values
#define ATXREQ_COMP_EXPANDED		3		// expanded price block format
#define ATXREQ_COMP_AT				10		// new A-T price block format
#define ATXREQ_COMP_MASK			0x3f	// type mask
#define ATXREQ_LARGE_BUFFER			0x40	// use the large buffer to reply
#define ATXREQ_COMP_ONLY			0x80	// standard price block format, but price block only

#define ATXREQ_WILD_SYMBOLMATCH		0x0001	// match only the symbol (country exchange can vary)
#define ATXREQ_WILD_NOCOUNTRY		0x0002	// don't return the country code for wildcard requests
#define ATXREQ_WILD_ROOT			0x0004	// use symbol as a root (p,w,r,/ are ok)

#define ATXREQ_SRCH_EQUAL			0		// exact match
#define ATXREQ_SRCH_APPROX			1		// find one...
#define ATXREQ_SRCH_PRIMARY			2		// primary market
#define ATXREQ_SRCH_COMPOSITE		3		// composite market
#define ATXREQ_SRCH_FUNDAMENTAL		4		// fundamental data only

// Fundamental field bitmap definitions

#define	ATXREQ_PFI_COMPANYNAME		0x0001
#define	ATXREQ_PFI_DIVIDENDS		0x0002
#define	ATXREQ_PFI_VOLATILITIES		0x0004
#define	ATXREQ_PFI_OPTIONROOTS		0x0008

#define	ATXREQ_PFI_SIC_CURRENCY		0x0010
#define	ATXREQ_PFI_CUSIP			0x0020
#define	ATXREQ_PFI_SHARES			0x0040
#define	ATXREQ_PFI_LONGROOTS		0x0080

#define	ATXREQ_PFI_FIXEDPAGE		0x8000
#define	ATXREQ_PFI_EARNINGS			0x8001
#define	ATXREQ_PFI_HISTORY			0x8002
#define	ATXREQ_PFI_FIELD_LIST		0x8fff

/*=======================================================================*/

#ifndef __DTDP_STRUCTURE
#define __DTDP_STRUCTURE

typedef struct _DTDP {
	DWORD  dataPtr: 24;			// lower 24 bits is data pointer
	DWORD  dataType: 8;			// upper 8 bits is the data type

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
} DTDP, *LPDTDP, *PDTDP;

#endif

/*=======================================================================*/

// advanced request key

typedef struct _ATREQ_KEY {
	BYTE  dataType;
	char  symbol[16];
	char  country[2];
	char  exchange;
} ATREQ_KEY, *LPATREQ_KEY, *PATREQ_KEY;

/*=======================================================================*/

// advanced requests

typedef struct _QSMX_HEADER {
	char       msgType;
	char       msgSubType;
	BYTE       status;					// request status
	BYTE       compType;				// compression type
} QSMX_HEADER, *LPQSMX_HEADER, *PQSMX_HEADER;

typedef struct _QSMX_FUND_FIELD {
	BYTE  recordType;
	BYTE  fieldID;
} QSMX_FUND_FIELD, *LPQSMX_FUND_FIELD, *PQSMX_FUND_FIELD;

// message '?', 'F' :  Get First
// message '?', 'L' :  Get Last
// message '?', 'N' :  Get Next
// message '?', 'P' :  Get Prev
// message '?', 'Q' :  Get Equal
// message '?', 'A' :  Get Approx
// message '?', 'I' :  Perform intelligent search
// message '?', 'D' :  Get Equal by dataType/dataPtr
// message '?', 'C' :  Get Equal by CUSIP (or NSIN/SEDOL) number

typedef struct _QSMX_REQUEST {
	QSMX_HEADER      header;
	WORD             flags;				// fundamental field flags
	BYTE             searchType;		// search type for 'I' messages
	BYTE             fundCount;			// fundamental field count
	ATREQ_KEY        key;				// key for search
	BYTE             resv2[4];
	QSMX_FUND_FIELD  fundFields[1];
} QSMX_REQUEST, *LPQSMX_REQUEST, *PQSMX_REQUEST;

typedef struct _QSMX_REQUEST_KEY {
	QSMX_HEADER      header;
	WORD             flags;				// fundamental field flags
	BYTE             searchType;		// search type for 'I' messages
	BYTE             fundCount;			// fundamental field count
	BYTE             dataType;			// data type
	BYTE             key[19];			// key for search
	BYTE             resv2[4];
	QSMX_FUND_FIELD  fundFields[1];
} QSMX_REQUEST_KEY, *LPQSMX_REQUEST_KEY, *PQSMX_REQUEST_KEY;

typedef struct _QSMX_REQUEST_DTDP {
	QSMX_HEADER      header;
	WORD             flags;				// fundamental field flags
	BYTE             resv1;
	BYTE             fundCount;			// fundamental field count
	DTDP             dtdp;				// DTDP for search
	BYTE             resv2[20];
	QSMX_FUND_FIELD  fundFields[1];
} QSMX_REQUEST_DTDP, *LPQSMX_REQUEST_DTDP, *PQSMX_REQUEST_DTDP;

typedef struct _QSMX_RESPONSE {
	QSMX_HEADER  header;
	DTDP         dtdp;					// DTDP of record
	ATREQ_KEY    key;					// key of record
	BYTE         fundType;				// fundamental type
	BYTE         resv[3];
	WORD         dataLength;			// total length in data
	WORD         pbLength;				// length of price block in data
	BYTE         data[500];				// price block, fundamental data, etc.
} QSMX_RESPONSE, *LPQSMX_RESPONSE, *PQSMX_RESPONSE;

// message '?', 'a' : Advise by DTDP

typedef struct _QSMX_ADVISE_REQ {
	QSMX_HEADER  header;
	BYTE         count;					// request count
	BYTE         resv[3];
	BYTE         flags[16];				// advise flags for each DTDP (set - advise, clear - unadvise)
	DTDP         dtdp[128];				// DTDPs to advise on
} QSMX_ADVISE_REQ, *LPQSMX_ADVISE_REQ, *PQSMX_ADVISE_REQ;

typedef struct _QSMX_ADVISE_RESP {
	QSMX_HEADER  header;
	BYTE         count;					// count
	BYTE         resv[3];				// status
	BYTE         flags[16];				// status flags, (set - success, clear - failure)
} QSMX_ADVISE_RESP, *LPQSMX_ADVISE_RESP, *PQSMX_ADVISE_RESP;

// message '?', 'w' : Wildcard advise by symbol

typedef struct _QSMX_WILDCARD_ADVISE_REQ {
	QSMX_HEADER  header;
	BYTE         count;					// request count
	BYTE         resv1[3];
	WORD         flags;					// advise flags (set - advise, clear - unadvise)
	BYTE         resv2[2];
	ATREQ_KEY    keys[16];				// keys to advise on
} QSMX_WILDCARD_ADVISE_REQ, *LPQSMX_WILDCARD_ADVISE_REQ, *PQSMX_WILDCARD_ADVISE_REQ;

typedef struct _QSMX_WILDCARD_ADVISE_RESP {
	QSMX_HEADER  header;
	BYTE         count;					// count
	BYTE         resv1[3];
	WORD         flags;					// status flags, (set - success, clear - failure)
	BYTE         resv2[2];
} QSMX_WILDCARD_ADVISE_RESP, *LPQSMX_WILDCARD_ADVISE_RESP, *PQSMX_WILDCARD_ADVISE_RESP;

// message '?', 'J' : Wildcard search

typedef struct _QSMX_WILDCARD_REQ {
	QSMX_HEADER  header;
	BYTE         searchLength;
	BYTE         resv1;
	WORD         flags;
	ATREQ_KEY    key;
	BYTE         resv2[4];
} QSMX_WILDCARD_REQ, *LPQSMX_WILDCARD_REQ, *PQSMX_WILDCARD_REQ;

// data is returned as keySize length structures that consist of a dword for dtdp and
// the remaining space is the key as in the following structure:
//			
//			struct {
//				DTDP  dtdp;
//				char  symbol[8];
//			}
//
// here keySize is 12.

typedef struct _QSMX_WILDCARD_RESP {
	QSMX_HEADER  header;
	BYTE         searchLength;			// symbol search length
	BYTE         count;					// count of keys in data
	BYTE         keySize;				// key size
	BYTE         resv;
	ATREQ_KEY    nextKey;				// next key to search on
	BYTE         data[508];				// returned keys
} QSMX_WILDCARD_RESP, *LPQSMX_WILDCARD_RESP, *PQSMX_WILDCARD_RESP;

// message '?', 'B' :  Bunched request with dataPtr
// message '?', 'S' :  Bunched request with symbol

typedef struct _QSMX_BUNCHED_REQ {
	QSMX_HEADER  header;
	BYTE         count;					// request count
	BYTE         resv[3];
	DWORD        flags;					// upper 16 bits - go live bits for each DTDP
	DTDP         dtdp[16];				// DTDP of each record requested
} QSMX_BUNCHED_REQ, *LPQSMX_BUNCHED_REQ, *PQSMX_BUNCHED_REQ;

typedef struct _QSMX_SYMBOL_REQ {
	QSMX_HEADER  header;
	BYTE         count;					// request count
	BYTE         resv[3];
	DWORD        flags;					// upper 16 bits - go live bits for each DTDP
	ATREQ_KEY    key[16];				// symbol of each record requested
} QSMX_SYMBOL_REQ, *LPQSMX_SYMBOL_REQ, *PQSMX_SYMBOL_REQ;

typedef struct _QSMX_BUNCHED_RESP {
	QSMX_HEADER  header;
	BYTE         count;
	BYTE         resv;
	WORD         dataLength;
	BYTE         data[528];
} QSMX_BUNCHED_RESP, *LPQSMX_BUNCHED_RESP, *PQSMX_BUNCHED_RESP;

// data is returned in the following format for request by DTDP:
//			
//			struct {
//				WORD  pbLength;
//				BYTE  data[pbLength];
//			}
//
// and in this format for request by symbol
//			
//			struct {
//				WORD  pbLength;
//              DTDP  dtdp;
//				BYTE  data[pbLength];
//			}
//

// message '?', 'O' :  compressed option database record request

#define OPTIONCOMP_NONE	    	0		// no compression for option records
#define OPTIONCOMP_AT       	1		// AtOption compression
#define OPTIONCOMP_AT_PRIMARY	2		// AtOption compression w/primary exchange

#define OPTIONREQ_DONE		0x0001		// that's all folks!

typedef struct _QSMX_OPTION_REQ {
	QSMX_HEADER  header;
	BYTE         searchType;			// search type 'F' first, 'N' all others
	BYTE         resv1[3];
	ATREQ_KEY    underlyingKey;			// underlying symbol
	ATREQ_KEY    optionKey;				// option symbol (for 'N' requests)
	BYTE         resv2[4];
} QSMX_OPTION_REQ, *LPQSMX_OPTION_REQ, *PQSMX_OPTION_REQ;

typedef struct _QSMX_OPTION_RESP {
	QSMX_HEADER  header;
	ATREQ_KEY    nextKey;				// next key
	WORD         flags;
	BYTE         count;					// compressed option record count
	BYTE         resv1;
	WORD         dataLength;			// data length
	BYTE         resv2[2];				// compressed option record count
	BYTE         data[504];				// compressed option records
} QSMX_OPTION_RESP, *LPQSMX_OPTION_RESP, *PQSMX_OPTION_RESP;

// message '?', 'X' :  compressed option database record request

#define INDEXCOMP_NONE	    0			// no compression for option records
#define INDEXCOMP_AT       	1			// AtIndex compression

#define INDEXREQ_DONE		0x0001		// that's all folks!

typedef struct _QSMX_INDEX_REQ {
	QSMX_HEADER  header;
	BYTE         searchType;			// search type 'F' first, 'N' all others
	BYTE         resv1[3];
	ATREQ_KEY    indexKey;				// option symbol 
	ATREQ_KEY    underlyingKey;			// underlying symbol (for 'N' requests)
	BYTE         resv2[4];
} QSMX_INDEX_REQ, *LPQSMX_INDEX_REQ, *PQSMX_INDEX_REQ;

typedef struct _QSMX_INDEX_RESP {
	QSMX_HEADER  header;
	ATREQ_KEY    nextKey;				// next key
	WORD         flags;
	BYTE         count;					// compressed index record count
	BYTE         resv1;
	WORD         dataLength;			// data length
	BYTE         resv2[2];				// compressed index record count
	BYTE         data[504];				// compressed index records
} QSMX_INDEX_RESP, *LPQSMX_INDEX_RESP, *PQSMX_INDEX_RESP;

// message '?', 'T' :  TOS request with symbol
// message '?', 'H' :  historical request with symbol

#define TOSCOMP_NONE			0
#define TOSCOMP_UNCOMP_8		1
#define TOSCOMP_COMP_8			2
#define TOSCOMP_COMP_12			3
#define TOSCOMP_COMP_12_SEC6	4
#define TOSCOMP_COMP_12_SEC10	5

#define HISTCOMP_NONE			0
#define HISTCOMP_COMP			1

#define HISTKEY_TOS_SECTOR		0
#define HISTKEY_HIST_SECTOR		0

#define HISTKEYFLGS_SECTOR		0x00000001
#define HISTKEYFLGS_TIME		0x00000002
#define HISTKEYFLGS_DATE		0x00000004
#define HISTKEYFLGS_REVERSE		0x00000008

#define HISTKEY_LAST_SECTOR		0x007fffff		// newest sector for this symbol
#define HISTKEY_FIRST_SECTOR	0				// oldest sector for this symbol

#define HISTDBTYPE_TICKS		0				// tick database

#define HISTDBTYPE_TICKS_1		9				// 1 minute bars
#define HISTDBTYPE_TICKS_5		10				// 5 minute bars
#define HISTDBTYPE_TICKS_10		11				// 10 minute bars
#define HISTDBTYPE_TICKS_60		12				// 1 hour bars
#define HISTDBTYPE_TICKS_15		13				// 15 minute bars

#define HISTDBTYPE_NEAR			20				// near history
#define HISTDBTYPE_WEEKLY		21				// weekly history
#define HISTDBTYPE_MONTHLY		22				// monthly history

#define HISTDBTYPE_VOLATILITY	30				// volatility

#define HISTDBTYPE_DEEP			0x80			// deep history

typedef union _ATHIST_KEYFLAGS {
	struct {
		DWORD  bSector: 1;						// sector field is present
		DWORD  bTime: 1;						// time field is present
		DWORD  bDate: 1;						// date field is present
		DWORD  bReverse: 1;						// reverse the bars/ticks
		DWORD  resv: 12;						// unused
		DWORD  day: 8;							// day
		DWORD  tickOffset: 8;					// reserverd for server use
	};
	DWORD  mask;
} ATHIST_KEYFLAGS, *LPATHIST_KEYFLAGS, *PATHIST_KEYFLAGS;

#define HISTREQFLGS_BACKWARD	0x00000000
#define HISTREQFLGS_FORWARD		0x00000001

typedef union _ATHIST_REQFLAGS {
	struct {
		WORD  direction: 1;						// 0 backward, 1 forward
		WORD  resv: 15;
	};
	WORD  mask;
} ATHIST_REQFLAGS, *LPATHIST_REQFLAGS, *PATHIST_REQFLAGS;

typedef struct _ATHIST_KEY {
	ATREQ_KEY  ticker;
	union {
		DWORD  date;							// julian date (used by history requests only!)
		struct {
			DWORD  day: 8;
			DWORD  month: 8;
			DWORD  year: 16;
		};
		DWORD  time;							// time of tick (TOS use only)
		DWORD  sector;							// sector
	};
	ATHIST_KEYFLAGS  flags;
} ATHIST_KEY, *LPATHIST_KEY, *PATHIST_KEY;

typedef struct _QSMX_HIST_REQ {
	QSMX_HEADER      header;
	ATHIST_REQFLAGS  flags;						// flags (see above)
	BYTE             keyNumber;					// key number (see above HISTKEY_*)
	BYTE             databaseType;						
	ATHIST_KEY       key;						// key
	BYTE             resv2[4];
} QSMX_HIST_REQ, *LPQSMX_HIST_REQ, *PQSMX_HIST_REQ;

typedef struct _QSMX_HIST_RESP {
	QSMX_HEADER      header;
	BYTE             keyNumber;					// key number (see above HISTKEY_*)
	BYTE             resv;
	union {
		WORD         highestSector;				// highest sector touched by this request
		WORD         baseDate;					// base date of file (used by history requests).
	};
	ATHIST_REQFLAGS  flags;						// flags (see above)
	WORD             count;						// number of ticks in 'data'
	ATHIST_KEY       nextKey;					// next key... place holder
	BYTE             data[496];					// tick data
} QSMX_HIST_RESP, *LPQSMX_HIST_RESP, *PQSMX_HIST_RESP;

#define HALTCOMP_NONE		0
#define HALTCOMP_SHORT		1

#define HALTCOND_QUOTE		0
#define HALTCOND_TRADE		1
#define HALTCOND_BATE		2

typedef struct _AT_HALT_SHORT {
	ATREQ_KEY  key;								// datatype+symbol+cce
	DWORD      condition: 8;					// halt condition, see atficond.h
	DWORD      conditionType: 2;				// condition type, see above
	DWORD      resv1: 17;
	DWORD      priceType: 5;
	WORD       time;							// 100*hour + minute
	BYTE       resv2[2];
} AT_HALT_SHORT, *LPAT_HALT_SHORT, *PAT_HALT_SHORT;

typedef struct _AT_HALT {
	ATREQ_KEY  key;								// datatype+symbol+cce
	DWORD      condition: 8;					// halt condition, see atficond.h
	DWORD      conditionType: 2;				// condition type, see above
	DWORD      resv1: 17;
	DWORD      priceType: 5;					// price type for prices
	WORD       time;							// 100*hour + minute
	BYTE       resv2[2];
	LONG       lastSale;						// last sale at halt
	LONG       high;							// high price at halt
	LONG       low;								// low price at halt
	DWORD      totalVolume;						// total volume at halt
	DWORD      sellVolume;						// sell volume
	DWORD      buyVolume;						// buy volume
} AT_HALT, *LPAT_HALT, *PAT_HALT;

typedef struct _QSMX_HALT_REQ {
	QSMX_HEADER  header;
	DWORD        recordNumber;
	BYTE         day;
	BYTE         count;
	BYTE         resv[6];
} QSMX_HALT_REQ, *LPQSMX_HALT_REQ, *PQSMX_HALT_REQ;

typedef struct _QSMX_HALT_RESP {
	QSMX_HEADER  header;
	DWORD        recordNumber;
	BYTE         count;
	BYTE         resv[11];
	BYTE         data[516];
} QSMX_HALT_RESP, *LPQSMX_HALT_RESP, *PQSMX_HALT_RESP;

typedef union _QSMX_MSGS {
	BYTE                        data[536];
	QSMX_HEADER                 header;
	QSMX_REQUEST                dbReq;
	QSMX_REQUEST_KEY            dbKey;
	QSMX_REQUEST_DTDP           dbDtdpReq;
	QSMX_RESPONSE               dbResp;
	QSMX_ADVISE_REQ             advReq;
	QSMX_ADVISE_RESP            advResp;
	QSMX_WILDCARD_ADVISE_REQ    wildAdvReq;
	QSMX_WILDCARD_ADVISE_RESP   wildAdvResp;
	QSMX_WILDCARD_REQ           wildReq;
	QSMX_WILDCARD_RESP          wildResp;
	QSMX_BUNCHED_REQ            bunchReq;
	QSMX_SYMBOL_REQ             symbolReq;
	QSMX_BUNCHED_RESP           bunchResp;
	QSMX_OPTION_REQ             optionReq;
	QSMX_OPTION_RESP            optionResp;
	QSMX_INDEX_REQ              indexReq;
	QSMX_INDEX_RESP             indexResp;
	QSMX_HIST_REQ               histReq;
	QSMX_HIST_RESP              histResp;
	QSMX_HALT_REQ               haltReq;
	QSMX_HALT_RESP              haltResp;
} QSMX_MSGS, *LPQSMX_MSGS, *PQSMX_MSGS;

/*=======================================================================*/

// advanced updates

#define QSUX_COMP_NONE				1			// no compression
#define QSUX_COMP_ULTRA				2			// "Ultra" compression (data specific)
#define QSUX_COMP_RDC				3			// RDC compression
#define QSUX_COMP_DEFAULT			0xff		// default compression

typedef struct _QSUX_HEADER {
	DWORD  msgType: 8;
	DWORD  msgSubType: 8;
	DWORD  length: 11;							// length of update
	DWORD  compType: 5;							// compression type
} QSUX_HEADER, *LPQSUX_HEADER, *PQSUX_HEADER;

#define QSUX_TICK_UNCH				0
#define QSUX_TICK_UP				1
#define QSUX_TICK_DOWN				2

// message '!', 't' : Trade

#define QSUX_TFLGS_TICK_MASK		0x0000003
#define QSUX_TFLGS_TICK_UNCH		0x0000000
#define QSUX_TFLGS_TICK_UP			0x0000001
#define QSUX_TFLGS_TICK_DOWN		0x0000002
#define QSUX_TFLGS_UPDATE_OPEN		0x0000004
#define QSUX_TFLGS_UPDATE_HIGH		0x0000008
#define QSUX_TFLGS_UPDATE_LOW		0x0000010
#define QSUX_TFLGS_UPDATE_LAST		0x0000020
#define QSUX_TFLGS_STORE_LAST		0x0000040
#define QSUX_TFLGS_FILTERED			0x0000080

typedef struct _QSUX_TRADE {
	QSUX_HEADER  header;						// '!', 't'
	DTDP         dtdp;							// data type, data pointer
	DTDP         dtdpBase;						// base symbol, data type, data pointer
	BYTE         mmID[4];						// market maker id
	BYTE         priceType;						// price type
	BYTE         condition;						// trade condition
	WORD         time;							// time of trade
	BYTE         seconds;
	BYTE         bateCode;						// bate code
	BYTE         exchange;						// exchange of trade
	BYTE         resv1;
	union {
		DWORD    flags;							// update flags, trend, etc.
		struct {
			DWORD  tick: 2;						// tick
			DWORD  bOpen: 1;					// update open
			DWORD  bHigh: 1;					// update high
			DWORD  bLow: 1;						// update low
			DWORD  bLast: 1;					// update last
			DWORD  bStoreLast: 1;				// update last only
			DWORD  bFiltered: 1;				// trade is suspect
			DWORD  resvFlags: 24;
		};
	};
	LONG         last;							// last sale
	DWORD        size;							// trade size
	DWORD        volume;						// volume
	BYTE         resv2[12];
} QSUX_TRADE, *LPQSUX_TRADE, *PQSUX_TRADE;

// message '!', 'q' : Quote

#define QSUX_QFLGS_BID_TICK_MASK	0x0000003
#define QSUX_QFLGS_BID_TICK_UNCH	0x0000000
#define QSUX_QFLGS_BID_TICK_UP		0x0000001
#define QSUX_QFLGS_BID_TICK_DOWN	0x0000002
#define QSUX_QFLGS_ASK_TICK_MASK	0x000000c
#define QSUX_QFLGS_ASK_TICK_UNCH	0x0000000
#define QSUX_QFLGS_ASK_TICK_UP		0x0000004
#define QSUX_QFLGS_ASK_TICK_DOWN	0x0000008
#define QSUX_QFLGS_MID_TICK_MASK	0x0000030
#define QSUX_QFLGS_MID_TICK_UNCH	0x0000000
#define QSUX_QFLGS_MID_TICK_UP		0x0000010
#define QSUX_QFLGS_MID_TICK_DOWN	0x0000020
#define QSUX_QFLGS_RESTRICTED		0x0000040

typedef struct _QSUX_QUOTE {
	QSUX_HEADER  header;						// '!', 'q'
	DTDP         dtdp;							// data type, data pointer
	DTDP         dtdpBase;						// base symbol, data type, data pointer
	BYTE         mmID[4];						// market maker id
	BYTE         priceType;						// price type
	BYTE         condition;						// quote condition
	WORD         time;							// time of quote
	BYTE         seconds;						// seconds of quote
	BYTE         bateCode;						// bate code
	BYTE         bidExchange;					// bid exchange
	BYTE         askExchange;					// ask exchange
	union {
		DWORD    flags;							// update flags, trend, etc.
		struct {
			DWORD  bidTick: 2;
			DWORD  askTick: 2;
			DWORD  midTick: 2;
			DWORD  bRestricted: 1;
			DWORD  resvFlags: 25;
		};
	};
	LONG         bid;							// bid
	DWORD        bidSize;						// bid size
	LONG         ask;							// ask sale
	DWORD        askSize;						// ask size
	LONG         mid;							// mid
	BYTE         resv1[12];
} QSUX_QUOTE, *LPQSUX_QUOTE, *PQSUX_QUOTE;

// message '!', 'r' : Range

#define QSUX_RANGE_OPEN				0x00		// opening range
#define QSUX_RANGE_CLOSE			0x01		// closing range

typedef struct _QSUX_RANGE {
	QSUX_HEADER  header;						// '!', 'r'
	DTDP         dtdp;							// data type, data pointer
	DTDP         dtdpBase;						// base symbol, data type, data pointer
	LONG         price1;						// price 1
	LONG         price2;						// price 2
	DWORD        volume;						// volume
	WORD         time;							// time of message
	union {
		BYTE       flags;						// type, etc.
		struct {
			BYTE     openClose: 1;				// range type (open/close)
			BYTE     resvFlags: 7;
		};
	};
	BYTE         priceType;						// price type
	BYTE         resv1[8];
} QSUX_RANGE, *LPQSUX_RANGE, *PQSUX_RANGE;

// message '!', 'S' : Summary
// message '!', 'T' : Trade send as a summary (price type change)
// message '!', 'Q' : Quote send as a summary (price type change)
// message '!', 'V' : Volume summary

#define QSUX_REFTYPE_EQUITY			0
#define QSUX_REFTYPE_OPTION			1
#define QSUX_REFTYPE_FUTURE			2
#define QSUX_REFTYPE_MMAKER			3
#define QSUX_REFTYPE_BOND			4
#define QSUX_REFTYPE_MMFUND			5
#define QSUX_REFTYPE_MUTUAL			6
#define QSUX_REFTYPE_FOREX			7
#define QSUX_REFTYPE_FIXED			8
#define QSUX_REFTYPE_VOLUME			9

typedef union _QSUX_REFDATA_FLAGS {
	DWORD  mask;							// update flags, trend, etc.
	struct {
		DWORD  lastTick: 2;					// last tick
		DWORD  bidTick: 2;					// bid tick
		DWORD  askTick: 2;					// ask tick
		DWORD  resvFlags: 18;
		DWORD  bRestricted: 1;				// UPC restricted
		DWORD  bLow52: 1;					// 52 week low alert
		DWORD  bHigh52: 1;					// 52 week high alert
		DWORD  bLow: 1;						// low alert
		DWORD  bHigh: 1;					// high alert
		DWORD  bVolume: 1;					// volume alert
		DWORD  bSplit: 1;					// split flag
		DWORD  bDividend: 1;				// X Dividend flag
	};
} QSUX_REFDATA_FLAGS, *LPQSUX_REFDATA_FLAGS, *PQSUX_REFDATA_FLAGS;


typedef struct _QSUX_REFDATA_EQUITY {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                close;					// yesterday
	LONG                last;					// last sale
	LONG                bid;					// bid
	LONG                ask;					// ask
	LONG                mid;					// mid
	LONG                open;					// open
	LONG                high;					// high
	LONG                low;					// low
	union {
		LONG            rgnlClose;				// regional close
		LONG            closingBid;				// closing bid
	};
	DWORD               lastSize;				// trade size
	DWORD               askSize;				// ask size
	DWORD               bidSize;				// bid size
	DWORD               volume;					// volume
	WORD                tradeTime;				// time of trade
	WORD                quoteTime;				// time of quote
	WORD                restriction;			// restriction
	BYTE                priceType;				// price type
	BYTE                tradeCondition;			// trade condition
	BYTE                quoteCondition;			// quote condition
	BYTE                quoteSeconds;			// quote seconds
	BYTE                closeExchange;			// close exchange
	BYTE                openExchange;			// open exchange
	BYTE                highExchange;			// high exchange
	BYTE                lowExchange;			// low exchange
	BYTE                lastExchange;			// last exchange
	BYTE                bidExchange;			// bid exchange
	BYTE                askExchange;			// ask exchange
	BYTE                newsFlag;				// news flags
	BYTE                resv2[22];
} QSUX_REFDATA_EQUITY, *LPQSUX_REFDATA_EQUITY, *PQSUX_REFDATA_EQUITY;

typedef struct _QSUX_REFDATA_OPTION {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                close;					// yesterday
	LONG                last;					// last sale
	LONG                bid;					// bid
	LONG                ask;					// ask
	LONG                open;					// open
	LONG                high;					// high
	LONG                low;					// low
	DWORD               lastSize;				// trade size
	DWORD               askSize;				// ask size
	DWORD               bidSize;				// bid size
	DWORD               volume;					// volume
	LONG                yestBid;				// yesterday's bid
	LONG                yestAsk;				// yesterday's ask
	DWORD               yestVolume;				// yesterday's volume
	DWORD               openInterest;			// open interest
	DWORD               strikePrice;			// strike price
	WORD                tradeTime;				// time of trade
	WORD                quoteTime;				// time of quote
	WORD                expirationDate;			// expiration date
	BYTE                priceType;				// price type
	BYTE                tradeCondition;			// trade condition
	BYTE                quoteCondition;			// quote condition
	BYTE                strikePriceType;		// strike price type
	BYTE                resv2[26];
} QSUX_REFDATA_OPTION, *LPQSUX_REFDATA_OPTION, *PQSUX_REFDATA_OPTION;

typedef struct _QSUX_REFDATA_MMAKER {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                bid;					// bid
	LONG                ask;					// ask
	DWORD               askSize;				// ask size
	DWORD               bidSize;				// bid size
	WORD                seconds;				// quote seconds
	WORD                time;					// time of quote
	BYTE                priceType;				// price type
	BYTE                condition;				// quote condition
	BYTE                bidExchange;			// bid exchange
	BYTE                askExchange;			// ask exchange
	BYTE                mmType;					// market maker type
	BYTE                mmClass;				// market maker class
	BYTE                mmID[4];				// market maker id
	BYTE                resv2[26];
} QSUX_REFDATA_MMAKER, *LPQSUX_REFDATA_MMAKER, *PQSUX_REFDATA_MMAKER;

typedef struct _QSUX_REFDATA_FUTURE {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                close;					// yesterday
	LONG                last;					// last sale
	LONG                bid;					// bid
	LONG                ask;					// ask
	LONG                high;					// high
	LONG                low;					// low
	LONG                open1;					// opening range 1
	LONG                open2;					// opening range 2
	LONG                close1;					// closing range 1
	LONG                close2;					// closing range 2
	DWORD               lastSize;				// trade size
	DWORD               askSize;				// ask size
	DWORD               bidSize;				// bid size
	DWORD               volume;					// volume
	DWORD               openInterest;			// open interest
	DWORD               strikePrice;			// strike price
	DWORD               tickCount;				// trade count
	WORD                expirationDate;			// expiration date
	WORD                tradeTime;				// time of trade
	WORD                quoteTime;				// time of quote
	BYTE                priceType;				// price type
	BYTE                tradeCondition;			// trade condition
	BYTE                quoteCondition;			// quote condition
	BYTE                bateCode;				// bate code
	BYTE                displayPriceType;		// display price type
	BYTE                resv2[23];
} QSUX_REFDATA_FUTURE, *LPQSUX_REFDATA_FUTURE, *PQSUX_REFDATA_FUTURE;

typedef struct _QSUX_REFDATA_BOND {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                last;					// last
	LONG                open;					// open
	LONG                high;					// high
	LONG                low;					// low
	LONG                close;					// close
	LONG                bid;					// bid
	LONG                ask;					// ask
	DWORD               volume;					// volume
	DWORD               lastSize;				// last size
	DWORD               bidSize;				// bid size
	DWORD               askSize;				// ask size
	WORD                yield;					// yield
	WORD                tradeTime;				// trade time/date
	WORD                quoteTime;				// quote time/date
	BYTE                tradeCondition;			// trade condition
	BYTE                quoteCondition;			// quote condition
	BYTE                priceType;				// price type
	BYTE                resv[11];
} QSUX_REFDATA_BOND, *LPQSUX_REFDATA_BOND, *PQSUX_REFDATA_BOND;

typedef struct _QSUX_REFDATA_FIXED {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                last;					// last
	LONG                open;					// open
	LONG                high;					// high
	LONG                low;					// low
	LONG                close;					// close
	LONG                bid;					// bid
	LONG                ask;					// ask
	DWORD               tradeCount;				// update count
	WORD                yield;					// yield
	WORD                lastTime;				// last update time/date
	BYTE                priceType;				// price type
	BYTE                yieldPriceType;
	WORD                lastSize;
	LONG                lastYield;
	LONG                bidYield;
	LONG                askYield;
	WORD                quoteTime;
	BYTE                resv[6];
} QSUX_REFDATA_FIXED, *LPQSUX_REFDATA_FIXED, *PQSUX_REFDATA_FIXED;

typedef struct _QSUX_REFDATA_MMFUND {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                close;					// yesterday
	LONG                nav;					// net asset value
	LONG                assets;					// assets
	LONG                yield7Day;				// 7 day yield
	LONG                yield14Day;				// 14 day yield
	LONG                yield30Day;				// 30 day yield
	LONG                yield60Day;				// 60 day yield
	LONG                yield90Day;				// 120 day yield
	LONG                yield120Day;			// 120 day yield
	LONG                yield180Day;			// 180 day yield
	LONG                yield1Year;				// 1 year yield
	LONG                yield7DayAve;			// effective 7 day yield
	WORD                aveMaturity;			// average maturity
	WORD                time;					// last update time/date
	BYTE                priceType;				// price type
	BYTE                resv2[7];
} QSUX_REFDATA_MMFUND, *LPQSUX_REFDATA_MMFUND, *PQSUX_REFDATA_MMFUND;

typedef struct _QSUX_REFDATA_MUTUAL {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG                close;					// yesterday
	LONG                nav;					// net asset value
	LONG                offer;					// offer
	LONG                capGains;				// capital gains
	LONG                dividend;				// dividend
	LONG                stockSplit;				// stock split
	LONG                assets;					// assets
	LONG                correction;				// correction for yesterday
	LONG                netChange;				// net change
	LONG                special;				// special price
	WORD                time;					// last update time/date
	BYTE                footnotes[8];			// footnote codes
	BYTE                priceType;				// price type
	BYTE                resv[11];
} QSUX_REFDATA_MUTUAL, *LPQSUX_REFDATA_MUTUAL, *PQSUX_REFDATA_MUTUAL;

typedef struct _QSUX_REFDATA_FOREX {
	QSUX_REFDATA_FLAGS  flags;					// update flags, trend, etc.
	LONG	            last;					// last sale
	LONG	            open;					// open
	LONG	            high;					// high
	LONG	            low;					// low
	LONG	            close;					// close
	LONG	            bid;					// bid
	LONG	            ask;					// ask
	WORD	            yield;					// yield
	WORD	            time;					// last update time/date
	BYTE	            contributor[4];			// last contributor code
	BYTE                priceType;				// price type
	BYTE	            resv[11];
} QSUX_REFDATA_FOREX, *LPQSUX_REFDATA_FOREX, *PQSUX_REFDATA_FOREX;

typedef struct _QSUX_REFDATA_VOLUME {
	DWORD  flags;								// misc flags
	DWORD  volume;								// volume
	DWORD  openInterest;						// open interest
	WORD   time;								// time of refresh
	BYTE   resv2[6];
} QSUX_REFDATA_VOLUME, *LPQSUX_REFDATA_VOLUME, *PQSUX_REFDATA_VOLUME;

typedef union _QSUX_REFRESH_DATA {
	BYTE                 refData[240];
	QSUX_REFDATA_EQUITY  equity;
	QSUX_REFDATA_OPTION  option;
	QSUX_REFDATA_FUTURE  future;
	QSUX_REFDATA_MMAKER  marketMaker;
	QSUX_REFDATA_BOND    bond;
	QSUX_REFDATA_FIXED   fixed;
	QSUX_REFDATA_MMFUND  moneyMarket;
	QSUX_REFDATA_MUTUAL  mutualFund;
	QSUX_REFDATA_FOREX   forex;
	QSUX_REFDATA_VOLUME  volume;
} QSUX_REFRESH_DATA, *LPQSUX_REFRESH_DATA, *PQSUX_REFRESH_DATA;

typedef struct _QSUX_REFRESH {
	QSUX_HEADER        header;					// '!', 'S'
	DTDP               dtdp;					// data type, data pointer
	DTDP               dtdpBase;				// base symbol, data type, data pointer
	BYTE               type;					// refresh type
	BYTE               resv[3];
	QSUX_REFRESH_DATA  data;
} QSUX_REFRESH, *LPQSUX_REFRESH, *PQSUX_REFRESH;

// message '!', 'a' : Alert
// message '!', 'n' : News Alert

typedef struct _QSUX_ALERT {
	QSUX_HEADER  header;						// '!', 'a'
	DTDP         dtdp;							// data type, data pointer
	DTDP         dtdpBase;						// base symbol, data type, data pointer
	BYTE         type;							// alert type
	BYTE         resv;
	WORD         source;
	WORD         time;
	WORD         flags;							// variable (based on alert type)
	union {
		BYTE     data[16];
		BYTE     reference[16];
	};
} QSUX_ALERT, *LPQSUX_ALERT, *PQSUX_ALERT;

// message '!', 'h' : Alert

typedef struct _QSUX_HALT {
	QSUX_HEADER    header;						// '!', 'h'
	AT_HALT_SHORT  haltData;
} QSUX_HALT, *LPQSUX_HALT, *PQSUX_HALT;

typedef union _QSUX_UPDATE {
	QSUX_HEADER   header;
	BYTE          message[256];
	QSUX_TRADE    trade;
	QSUX_QUOTE    quote;
	QSUX_RANGE    range;
	QSUX_REFRESH  refresh;
	QSUX_ALERT    alert;
	QSUX_HALT     halt;
} QSUX_UPDATE, *LPQSUX_UPDATE, *PQSUX_UPDATE;

/*=======================================================================*/

#pragma pack()

/*=======================================================================*/

#endif

/*=======================================================================*/

