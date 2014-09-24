/*
 *		atfinews.h		Date=980701
 *
 *		A-T News defines and structures.
 *
 *		Written 03/05/96	R. Holm
 *
 *		Modified 04/16/96	R. Holm
 *			cleaned up and hopefully made it more consistent.
 *
 *		Modified 04/30/96	R. Holm
 *			moved NEWS_RECORD to atfinews.h where it can be used by applications.
 *
 *		Modified 05/20/96	R. Holm
 *			added QSM_REQ_OLDRECORD and QSM_RSP_OLDRECORD.
 *
 *		Modified 05/22/96	R. Holm
 *			added comments to better describe NT Server output.
 *
 *		Modified 05/23/96	R. Holm
 *			moved Comtex to 31.
 *			moved Reuters Financial to 25.
 *
 *		Modified 05/24/96	R. Holm
 *			changed the data size of QSU_PROFILEEVENT and ATN_PROFILEEVENT messages 
 *				from 494 to 482.
 *
 *		Modified 06/05/96	R. Holm
 *			added QSM_REQ_CATCODES and QSM_RSP_CATCODES.
 *
 *		Modified 06/13/96	R. Holm
 *			moved NWSSRC_DV_BROAD_TAPE to 26.
 *
 *		Modified 07/03/96	R. Holm
 *			NWSSRC_MARKET_NEWS is 20.
 *
 *		Modified 07/05/96	R. Holm
 *			NWSSRC_SPORTS_TICKER is 33.
 *			sizeof STORY::text changed to MAX_STORYLENGTH.
 *
 *		Modified 07/11/96	R. Holm
 *			support for extended Comtex news sources.
 *
 *		Modified 07/12/96	R. Holm
 *			added Dow Jones International sources.
 *
 *		Modified 09/25/96	R. Holm
 *			added misc *LP types.
 *
 *		Modified 12/13/96	R. Holm
 *			added QSM_RQ_CSPL, QSM_RS_CSPL, QSM_RQ_TSR, and QSM_RS_TSR messages.
 *
 *		Modified 02/05/97	R. Holm
 *			increased the size of symbol directories to 65536
 *
 *		Modified 02/11/97	LM Loh
 *			added search event updates - results of qx search request message
 *				but delivered asynchronously
 *
 *		Modified 04/21/97	LM Loh
 *			added new search type - true boolean searching with
 *				and, or, butnot valid operations 
 *
 *		Modified 05/23/97	R. Holm
 *			added NWSSRC_DV_ONLINE.
 *
 *		Modified 6/18/97	LM Loh
 *			increased story text size to 65000 bytes instead of 32K
 *				several news sources will be increasing their story
 *				size very soon.
 *
 *		Modified 06/19/97	R. Holm
 *			added new SIG_* values for STORY records.
 *
 *		Modified 06/20/97	R. Holm
 *			NWSSRC_SPE changed to NWSSRC_SPN.
 *
 *		Modified 06/26/97	R. Holm
 *			NWSSRC_FUE changed to NWSSRC_DJN.
 *
 *		Modified 06/27/97	R. Holm
 *			added STORY_FRAGMENT.
 *
 *		Modified 9/17/97	LM Loh
 *			added Z search record - for extended or additional search information because too
 *				large to fit within one qx request
 *
 *		Modified 4/16/98	LM Loh
 *			added qg messages - handle transfer of generic category code information
 *
 *		Modified 06/04/98	R. Holm
 *			NWSSRC_DV_CAPITAL_MARKETS changed to NWSSRC_DJH.
 *
 *		Modified 07/01/98	R. Holm
 *			NEWS_RECORD changed to AT_NEWS_RECORD.
 *
 *		Modified 2/22/99 LM Loh
 *			Added some DowJones news sources - those received from DowVision direct feed.
 *
 *		Modified 2/23/99 LM Loh
 *			Added AT_NEWS_MESSAGE - this structure is sent from parsers through MDS distribution
 *			mechanism.  The original AT_NEWS_RECORD is still used by NewsServer as structure in
 *			actual news data files *.nwx.
 *			Also, removed signature from new message structure - don't need.
 *			Moved around WORD contents of structure to force better packing on even boundary.
 *
 *		Modified 4/21/99 LM Loh
 *			Modified list of news sources.
 *			Added:
 *				#define	NWSSRC_DV_ENERGY			57
 *				#define	NWSSRC_DISC_FEDFILE			63 
 *
 *			Removed:
 *				#define	NWSSRC_COMTEX_SPORTS        7
 *				#define	NWSSRC_REUTERS_NEWS         22
 *				#define NWSSRC_USA_TODAY			32
 *				#define NWSSRC_SPORTS_TICKER		33
 *				#define NWSSRC_JOURNAL_GRAPHICS		37
 *				#define NWSSRC_NEWSNET				39
 *				#define NWSSRC_DV_TEST 				46
 *
 *		Modified 4/21/99 LM Loh
 *			Added Comtex Sports back in - Internet folks don't
 *		redistribute news to paying customers, but they do use as demo feed when
 *		at software show over weekends.
 *
 *		Modified 5/5/99 LM Loh
 *			Added 22 PRN PR News
 *
 *
 *		Modified 7/7/99 LM Loh
 *		Added 0x20 PME and 0x21 PMP Primark Edgar Filings and Image Filings
 *
 *		Modified 7/22/99 LM Loh
 *		Added 37 PMI Primark Insider
 *
 *		Modified 12/14/99 LMLoh
 *			Added 2 new Comtex news sources and refine Comtex MarketAlert
 *			Now there are a total of 6 Comtex sources.
 *			Changed 27 from NWSSRC_COMTEX_MARKET TO NWSSRC_COMTEX_GENERAL
 *			Added #define	NWSSRC_COMTEX_WEATHER		39
 *			Added #define	NWSSRC_COMTEX_PUBLIC		46
 *
 *		Modified 5/31/00 LM Loh
 *			Added #define	NWSSRC_FLYONTHEWALL		63
 *
 *		Copyright (c) A-T Financial Information, Inc., 1996-2000
 *		All rights reserved.
 *
 */

/*===========================================================================*/

#ifndef __NEWS_DEFINES
#define __NEWS_DEFINES

/*===========================================================================*/

// Current quote server news structures and defines

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

// Minimum server version for ATPRESS.

#define	ATPRESS_MIN_NT_VERS			209


// News sources

#define	NWSSRC_UNK		            0
#define	NWSSRC_DJB		            1
#define	NWSSRC_FUTURE_WORLD_NEWS    2
#define	NWSSRC_SPM		            3
#define	NWSSRC_KNIGHT_RIDDER        4
#define	NWSSRC_SPI		            5
#define	NWSSRC_DV_ONLINE            6
#define	NWSSRC_COMTEX_SPORTS        7
#define	NWSSRC_SPN		            8
#define	NWSSRC_REUTERS	            9
#define	NWSSRC_DV_PROF_INVESTOR     10
#define	NWSSRC_DV_PRESS_RELEASE     11
#define	NWSSRC_DJH					12
#define	NWSSRC_DV_FED_FILINGS       13
#define	NWSSRC_DV_WSJ	            14
#define	NWSSRC_DV_EMERGING_MARKETS	15
#define	NWSSRC_DV_ECONOMIC_WIRE		16
#define	NWSSRC_DV_FINANCIAL_WIRE	17
#define	NWSSRC_DV_FOREIGN_EXCHANGE	18
#define	NWSSRC_DV_EUROPE          	19
#define	NWSSRC_MARKET_NEWS          20
#define	NWSSRC_COMTEX_INTL          21
#define	NWSSRC_PRNEWS				22
#define	NWSSRC_REU		            23
#define NWSSRC_DJF		            24
#define NWSSRC_REUTERS_FINANCIAL	25
#define NWSSRC_DV_BROAD_TAPE		26
#define NWSSRC_COMTEX_GENERAL       27
#define	NWSSRC_FIRSTCALL_US			28
#define	NWSSRC_FIRSTCALL_INTL		29
#define	NWSSRC_DJN					30
#define NWSSRC_COMTEX_WALL_STREET	31
#define NWSSRC_COMTEX				31

#define NWSSRC_BASE_NT				32

#define	NWSSRC_PMK_EDGAR			32		
#define	NWSSRC_PMK_IMAGE			33
#define NWSSRC_BUSINESS_WIRE		34
#define NWSSRC_FEDERAL_NEWS			35
#define	NWSSRC_DV_COMMSERV			36
#define NWSSRC_PMK_INSIDER			37
#define NWSSRC_AMERICAN_BANKER		38
#define	NWSSRC_COMTEX_WEATHER		39
#define NWSSRC_ZIFF					40
#define NWSSRC_AFP					41
#define NWSSRC_CNN					42
#define NWSSRC_AM_BEST				43
#define	NWSSRC_DV_CAPMARKET			44
#define NWSSRC_DV_INVESTEXT			45
#define	NWSSRC_COMTEX_PUBLIC		46
#define NWSSRC_DV_PROF_INVESTOR_D	47
#define NWSSRC_DV_JAPAN_ECONOMIC	48
#define NWSSRC_DV_ABSTRACT			49
#define NWSSRC_DV_NYT  				50
#define NWSSRC_DV_INTERNATIONAL		51
#define NWSSRC_DV_WORLD_EQUITY		52
#define NWSSRC_DV_PETROLEUM_WIRE	53
#define NWSSRC_DV_BANKERS_REPORT	54
#define	NWSSRC_DV_LATIMES			55
#define	NWSSRC_DV_ASIANEQ			56
#define	NWSSRC_DV_ENERGY			57
#define	NWSSRC_DV_BARRONS			58
#define	NWSSRC_DV_FAREAST			59
#define	NWSSRC_DV_WSEUROPE			60
#define	NWSSRC_DV_ASIANWS			61
#define	NWSSRC_DV_EXTEL				62
#define	NWSSRC_FLYONTHEWALL			63

#define	NWSSRC_MAX					64		// Number of valid News Types
#define NWSSRC_MASKSIZE				8

// error codes

#define NWSSTAT_NO_ERROR			0
#define NWSSTAT_BAD_NEWSSOURCE		1
#define NWSSTAT_BAD_DATA			2
#define NWSSTAT_GENERAL_ERROR		0xff

// Sizes of Directories and Blocks

#define	DAY_DIRSIZE		4096
#define	DAY_BLKSIZE		1024

#define	CAT_DIRSIZE		8192
#define	CAT_BLKSIZE		1024

#define	SYM_DIRSIZEEXT	65280
#define	SYM_DIRSIZE		49152
#define	SYM_BLKSIZE		256

#define	DAY_DIRSIZE		4096
#define	DAY_BLKSIZE		1024

#define	REF_DIRSIZE		49152
#define	REF_BLKSIZE		128

#define	MAX_DIRSIZE		49152
#define	MAX_BLKSIZE		1024


#ifndef __STORYPTR_TYPE
#define __STORYPTR_TYPE
typedef	struct _STORYPTR {
	WORD  wOffset;							// Offset of story (divided by 256)
	WORD  minute :  6,						// Lower  6 bits of 2nd WORD
		  hour   :  5,						// Middle 5 bits of 2nd WORD
		  type   :  5;						// Upper  5 bits of 2nd WORD
	WORD  date   : 12,						// Lower 12 bits of 3rd WORD
 		  flag0  : 1,						// Upper  4 bits of 3rd WORD are flags
		  flag1  : 1,						// flag0 = highlight bkgnd,profile hit!
		  flag2  : 1,						// flag1 = gray text, story was read
		  flag3  : 1;						// flag2 = not used
} STORYPTR, *LPSTORYPTR;
#endif

#ifndef __STORYHND_TYPE
#define __STORYHND_TYPE
typedef	struct _STORYHND {
	WORD   wOffset;							// offset of story (divided by 256) or index
	DWORD  minute :  6,						// bits 0-5
		   hour   :  5,						// bits 6-10
	       date   : 13,						// bits 11-23
		   type   :  6,						// bits 24-29
		   resv   :  1,						// bit 30
		   flag   :  1;						// bit 31
} STORYHND, *LPSTORYHND;
#endif

typedef	struct _STORYREF {
	STORYHND	shnd;
	STORYPTR	sptr;
} STORYREF, FAR *LPSTORYREF;

#define	KEY_SIZE		6

typedef	struct _KEY_ENTRY {
	char  key[KEY_SIZE];					// actual key
	WORD  startBlock;						// offset modulo block size to first block for key
	WORD  endBlock;							// offset modulo block size to last block for key
} KEY_ENTRY, *LPKEY_ENTRY;

typedef	struct _NEWS_DIR {
	WORD       nDirSize;					// directory size
	WORD       nBlkSize;					// data block size
	WORD       nKeys;						// number of keys in the directory
	KEY_ENTRY  entry[1];					// Variable size depending on type
} NEWS_DIR, *LPNEWS_DIR;

// A Block consists of some header info and then as many
// STORYPTR's as will fit in the xxx_BLKSIZE.
// The STORYPTR's are 6 bytes each

typedef	struct _NEWS_BLOCK {
	WORD      nEntries;						// number of records in data
	WORD      link;							// link to next block - modulo block size
	STORYPTR  data[1];						// Variable size depending on type
} NEWS_BLOCK, *LPNEWS_BLOCK;

#define SIG_STORY		0xd4c1
#define SIG_FRAGMENT	0xd5c1
#define SIG_CONTINUE	0xd6c1
#define SIG_DELETE		0x00c1

#define MAX_STORYLENGTH	65000				// leaves 536 bytes for overhead from 64K

typedef struct _STORY {
	WORD  AT;								// Special flag = SIG_STORY
	WORD  sLength;							// Story length  (nc AT)
	WORD  date;								// Coded date
	WORD  time;								// Coded time
	char  text[MAX_STORYLENGTH];			// story text (unparsed) *see example
} STORY, *LPSTORY, *PSTORY;

typedef struct _STORY_FRAGMENT {
	WORD  AT;								// Special flag = STG_FRAGMENT
	WORD  sLength;							// Story length  (nc AT)
	WORD  date;								// Coded date
	WORD  next;								// next block... time is now in continuation
	char  text[MAX_STORYLENGTH];			// story text (unparsed) *see example
} STORY_FRAGMENT, *LPSTORY_FRAGMENT, *PSTORY_FRAGMENT;

// Example of STORY::text
//
// DJB4604 NY40128,DIV,DVR(null)Headline here\r\nFirst line of story\r\n...

// This structure is used to dispatch "News" Updates to the MDI windows
// It is created in TSFRAME::ProcessATDDEdata().


// NT News Server record

#define SIZEOF_NEWS_RECORD_HEADER	66

typedef struct _AT_NEWS_RECORD {
	UCHAR  sig[2];							// must be 0xC2,0xD4 (0xC2,0xD5 if continuation)
	WORD   length;							// length of record
	WORD   date;							// date of story
	WORD   time;							// time of story
	UCHAR  seqNumber[16];					// story sequence number
	UCHAR  source[4];						// source
	UCHAR  hot[2];							// hot news indicator
	UCHAR  accession[12];					// accession number
	WORD   retention;						// retention time
	UCHAR  origSource[4];					// original source
	WORD   sourceNum;						// source number
	WORD   headlineOffset;					// offset of the headline
	WORD   headlineLength;					// length of the headline
	WORD   storyOffset;						// offset of the story
	WORD   storyLength;						// length of the story
	WORD   offset;							// offset in this news file
	WORD   next;							// if continuation needed
	WORD   totalLength;						// total length of this record space
	WORD   resv;							// reserved space
	UCHAR  text[MAX_STORYLENGTH];			// data area
} AT_NEWS_RECORD, *LPAT_NEWS_RECORD;

//
// The text portion of the AT_NEWS_RECORD structure consists of a null terminated string
// of symbols and category codes followed by the headline and story.  Symbols and 
// category codes are formatted as separate fields terminated by field separators (0x1c).
// Category codes are further separated in the following different groups-Industry, 
// Government, Page, Subject, Market Sector, Product/Service, Geographic Region, 
// Statistical Table, and Journal.  Each of these groups is separated by field separators.
// Please note that the maximum size of *any* key is 16 bytes.
//
// Example- (Dow Jones story)
//		AN MOB OXY SUN TX\x1cI/OIL I/OIS\x1c\x1c\x1cN/HIY N/PET\x1cM/ENE\x1c\x1c
//		R/CA R/IL R/NY R/PA R/PRM R/US R/VA\x1c\x1c\x1c
//

/**************************************************************************************/
//	AT_NEWS_MESSAGE structure
//
//	new structure used to transmit information about a specific news story from the
//	parser through the MDS distribution channel.  Previously, AT_NEWS_RECORD was used
//	between parsers and NewsServer sent via ServInput object.

#define	MAX_STORYCHARS	65535

typedef struct _AT_NEWS_MESSAGE {
	DWORD  length;							// length of entire record (inc dword and text chars)
	WORD   date;							// date of story
	WORD   time;							// time of story
	WORD   sourceNum;						// source number
	WORD   resv;							// reserved space
	UCHAR  seqNumber[16];					// story sequence number
	UCHAR  source[4];						// source
	UCHAR  hot[2];							// hot news indicator
	UCHAR  accession[12];					// accession number
	WORD   retention;						// retention time
	UCHAR  origSource[4];					// original source
	DWORD  headlineOffset;					// offset of the headline
	DWORD  headlineLength;					// length of the headline
	DWORD  storyOffset;						// offset of the story
	DWORD  storyLength;						// length of the story
	UCHAR  text[MAX_STORYCHARS];			// data area
} AT_NEWS_MESSAGE, *LPAT_NEWS_MESSAGE;

/**************************************************************************************/

// request/response type messages

// SPL Compression messages
typedef	struct {
	BYTE		Q,a;						// Message type, subtype
	BYTE		flags;						// 0x00 = File contains 6 byte storyptrs
											// 0x80 = Message test
	WORD		nFirstSPL;					// Start index into file
	WORD		nMaxSPL;					// Max number of SPL''s wanted
	char		szFN[64];
} QSM_RQ_CSPL, *LPQSM_RQ_CSPL;

typedef	struct {
	BYTE		Q,a;
	SHORT		status;
	WORD		nSPTR;
	STORYPTR	sp;							// First story pointer non-compressed
	BYTE		byStream[522];
} QSM_RS_CSPL, *LPQSM_RS_CSPL;

// Status Values
// 0	OK: More data yet in file
// 1	OK: Reached EOF
// 2	OK: QS can do this message
// -1	NG: File not found
// -2 	NG: Offset invalid
// -3	NG: Read error
// -4	NG: Invalid data in file (if QS can tell?)
// -5	NG:	Bad flags
		
// Read Story Record 
typedef	struct {
	BYTE		Q,b;						// Message type, subtype
	BYTE		flags;						// 0x00 = Read up to wLength bytes from sp+wOffset
											// 0x80 = Test, QS will return status of 2 if can do message
	STORYPTR	sp;							// Storyptr of story to read
	WORD		wOffset;					// Offset into story
	WORD		wLength;					// Number of bytes to read
} QSM_RQ_TSR, *LPQSM_RQ_TSR;
		
typedef	struct {
	BYTE		Q,b;						// Message type, subtype
	SHORT		status;
	WORD		wLength;					// Number of bytes to follow
	BYTE		data[528];					// Story data (story structure)
} QSM_RS_TSR, *LPQSM_RS_TSR;

typedef	struct {
	char	Z;
	char	N;
	char	DJBEXT;							// 0 = None,1 = DJB,2 = EXT
	char	FWN;							// 1 = Futures World News
	char	SPM;							// 1 = S&P Market Scope
	char	extra1[5];
	char	NewsDays;						// Number of DJB or EXT news days kept
	char	DJF;							// Dow Jones Financial Wire
	char	ICV;							// ICV News
	char	SST;							// Street Software Technology news
	char	extra2[4];
} QSM_NEWSINFO, *LPQSM_NEWSINFO;			// S&P and "mutant" TK QuoteServers only

typedef	struct {
	BYTE	q;								// Message type q.
	BYTE	y;								// Inquire.
	WORD	status;							// Zero means OK.
	char	szFileName[32];					// File name server wants.
} QSM_INQRNF, *LPQSM_INQRNF;

typedef	struct {
	BYTE	q;								// Message type q.
	BYTE	Y;								// Porcess.
	WORD	status;							// Zero means OK.
	char	szFileName[32];					// File name server wants.
} QSM_PROCRNF, *LPQSM_PROCRNF;
	
// Status values
// 0	OK: More data yet in file
// 1	OK: Reached EOF
// 2 	OK: QS can do this message
// -1	NG: File not found
// -2	NG: Offset invalid
// -3	NG:	Read error
// -4	NG: Invalid flags

typedef struct _QSM_REQ_NEWSCAPS {			// request for count of days for each news source
	BYTE  q, c;								// ASCII 'q' and 'c'
} QSM_REQ_NEWSCAPS, *LPQSM_REQ_NEWSCAPS;

typedef struct _QSM_RSP_NEWSCAPS {			// response to the "qc" message
	BYTE  q, c;								// ASCII 'q' and 'c'
	BYTE  servType; 						// server type: 0=Quote Server/News, 1=NT News Server
	BYTE  reqType;							// request type ability: 0=simple cat,sym, or date
											//                       1=simple+advanced
	WORD  resv;								// breathing room... we may have forgotten something
	WORD  numSources;						// number of news sources following
	BYTE  sourceMap[NWSSRC_MAX];			// source mask... variable
} QSM_RSP_NEWSCAPS, *LPQSM_RSP_NEWSCAPS;

#define NSERVTYPE_QUOTESERV		0			// Quote Server/News
#define NSERVTYPE_NEWSSERV		1			// NT News Server

// Search Types
// Use Original Category Codes
#define REQTYPE_TODAY    		0			// today only
#define REQTYPE_CATEGORY 		1			// category codes search
#define REQTYPE_SYMBOL   		2 			// symbol search
#define REQTYPE_DATE     		3			// use the date range
#define REQTYPE_REFERENCE		4			// search on a reference number
#define REQTYPE_BOOLEAN  		5 			// a boolean equation is in the data field
#define	REQTYPE_DATERANGE		5			// On older MSDOS Servers, 5 is for DATE RANGE search

#define REQTYPE_ACCESSION		6 			// an accession number is in the data field
#define	REQTYPE_REUSF			7			// Reuters Select Feed only "AND" Search (concatenated cats)
#define	REQTYPE_TRUEBOOL		8			// RPN or postfix boolean expression, &|~ valid operators
#define	REQTYPE_KEYWORD			9			// Strictly keyword search

// Extended Search Types
// Use Generic Category Codes
#define REQTYPE_GCATEGORY 		10			// category codes search
#define REQTYPE_GBOOLEAN  		11 			// a boolean equation is in the data field
#define	REQTYPE_GTRUEBOOL		12			// RPN or postfix boolean expression, &|~ valid operators

// Delete stories.

typedef	struct _QSM_RQ_DELETE {
	BYTE		q,d;						// Message "qd"
	char		logName[20];				// User login name.
	WORD		nPtrs;						// Number of Story Pointers.			
	STORYPTR	ptrs[80];					// Story Pointer array.
} QSM_RQ_DELETE, *LPQSM_RQ_DELETE;

typedef	struct _QSM_RS_DELETE {
	BYTE		q,d;						// Message "qd"
	WORD		count;						// Success count.
} QSM_RS_DELETE, *LPQSM_RS_DELETE;

typedef struct _QSM_REQ_SEARCH {			// perform a search on the server
	BYTE  q, f;					   			// ASCII 'q' and 'f'
	BYTE  reqType;				   			// request type: 0=today list,  1=by category
											//               2=by seaymbol, 3=by date
	BYTE  source;				   			// news source
	WORD  startDate;			   			// start date (0=beginning) (in DSNB format)
	WORD  endDate;				   			// end date (0=today) (in DSNB format)
	char  szSymCat[504];		   			// variable
} QSM_REQ_SEARCH, *LPQSM_REQ_SEARCH;

typedef struct _QSM_RSP_SEARCH {			// response of the search
	BYTE  q, f;								// ASCII 'q' and 'f'
	BYTE  status;							// status
	BYTE  recSize;							// size of the "story pointers" (6 or 8)
	WORD  totalPtrs;						// total number of stories (pointers) found
	BYTE  szFilename[14];					// filename containing whole list
	WORD  nPtrs;							// number of pointers returned in this message
	BYTE  data[498];						// "last n" pointers that will fit in this message
} QSM_RSP_SEARCH, *LPQSM_RSP_SEARCH;

typedef struct _QSM_REQ_GETHEADLINES {
	BYTE      q, h;							// ASCII 'q' and 'h'
	BYTE      compLevel;					// Maximum return Compression level supported
	BYTE      resv;							// padding
	BYTE      flags;						// flags - misc.
											//    Bit 0:  Storyhands are compressed in dec date order
											//    Bit 1:  Storyhands are compressed in inc date order
	BYTE      num;							// number of headlines being requested
	union {
		STORYPTR  storyPtrs[6];				// story pointer/handle array
		STORYHND  storyHnds[6];
		BYTE      compressHnds[120];		// If input is compressed, use this (max is 20)
	};
} QSM_REQ_GETHEADLINES, *LPQSM_REQ_GETHEADLINES;

typedef struct _QSM_RSP_GETHEADLINES {
	BYTE  q, h;								// ASCII 'q' and 'h'
	BYTE  compLevel;						// compression level used
											//     0:  No compression - NULL terminated strings
											//	   1:  Huffman compression - each headline compressed separately
	BYTE  resv;								// padding
	BYTE  flags;							// flags - misc.
	BYTE  num;								// number of headlines returned
	BYTE  szHeadlines[506];					// data area - variable length null terminated strings (was 486)
} QSM_RSP_GETHEADLINES, *LPQSM_RSP_GETHEADLINES;

typedef struct _QSM_REQ_CLRINST {
	BYTE  q, j;								// ASCII 'q' and 'j'
	WORD  inst;								// instance to clear
} QSM_REQ_CLRINST, *LPQSM_REQ_CLRINST;

typedef struct _QSM_RSP_CLRINST {
	BYTE  q, j;								// ASCII 'q' and 'j'
	WORD  status;							// status from request
} QSM_RSP_CLRINST, *LPQSM_RSP_CLRINST;

typedef struct _QSM_REQ_CATCODES {
	BYTE   q, k;							// ASCII 'q' and 'k'
	BYTE   compLevel;						// compression level requested
	BYTE   newsSource;						// news source
	DWORD  index;							// starting index
} QSM_REQ_CATCODES, *LPQSM_REQ_CATCODES;

typedef struct _QSM_RSP_CATCODES {
	BYTE   q, k;							// ASCII 'q' and 'k'
	BYTE   compLevel;						// compression level used
	BYTE   status;							// status from request
	WORD   resv;							// padding
	WORD   count;							// count in this packet
	DWORD  total;							// total count
	BYTE   data[522];						// category codes in NUL terminated strings
} QSM_RSP_CATCODES, *LPQSM_RSP_CATCODES;

#define	CATSVERS	1
#define	CATSGROUP	2
#define	CATSCODES	3

#define	CODEMAXLEN	16
#define	LDMAXLEN	64

typedef struct _QSM_REQ_GENCATCODES {
	BYTE   q, g;							// ASCII 'q' and 'g'
	BYTE   flags;							// Flags 0: Request Version # only
											//       1: Category Group Names
											//       2: Category Codes and Long Descriptions
	BYTE   compLevel;						// compression level requested
	BYTE   compNS;							// compression ns huff codes to use
	DWORD  index;							// starting index
} QSM_REQ_GENCATCODES, *LPQSM_REQ_GENCATCODES;

typedef struct _QSM_RSP_GENCATCODES {
	BYTE   q, g;							// ASCII 'q' and 'g'
	BYTE   status;							// status from request
	BYTE   compLevel;						// compression level used
	WORD   version;							// version
	WORD   count;							// count in this packet
	DWORD  total;							// total count
	WORD   dataLen;							// number of bytes of compressed data
	BYTE   data[520];						// category codes in NUL terminated strings
} QSM_RSP_GENCATCODES, *LPQSM_RSP_GENCATCODES;

typedef struct _QSM_REQ_OLDRECORD {
	BYTE      q, o;							// ASCII 'q' and 'o'
	BYTE      compLevel;					// maximum compression level supported
	BYTE      resv;							// padding
	WORD      inst;							// instance handle (only needed if asking for more),
											// otherwise it should be 0xffff
	STORYHND  storyHnd;						// story handle to use in search
	WORD      offset;						// offset to start reading at
} QSM_REQ_OLDRECORD, *LPQSM_REQ_OLDRECORD;

typedef struct _QSM_RSP_OLDRECORD {
	BYTE      q, o;							// ASCII 'q' and 'o'
	BYTE      compLevel;					// compression level used
	BYTE      status;						// status
	WORD      inst;							// returned instance handle
	WORD      totalLen;						// total length of the story
	WORD      offset;						// offset of the block returned here
	WORD      length;						// length of the block returned
	BYTE      data[500];					// data area - a portion of a STORY structure
} QSM_RSP_OLDRECORD, *LPQSM_RSP_OLDRECORD;

typedef struct _QSM_REQ_RECORD {
	BYTE      q, r;							// ASCII 'q' and 'r'
	BYTE      compLevel;					// maximum compression level supported
	BYTE      resv;							// padding
	WORD      inst;							// instance handle (only needed if asking for more),
											// otherwise it should be 0xffff
	STORYHND  storyHnd;						// story handle to use in search
	WORD      offset;						// if continuation request 
											//		then offset to start reading at
											//		else bit 0 - 0 use ns cat codes
											//           bit 0 - 1 use generic cat codes
} QSM_REQ_RECORD, *LPQSM_REQ_RECORD;

typedef struct _QSM_RSP_RECORD {
	BYTE      q, r;							// ASCII 'q' and 'r'
	BYTE      compLevel;					// compression level used
	BYTE      status;						// status
	WORD      inst;							// returned instance handle
	WORD      totalLen;						// total length of the story
	WORD      offset;						// offset of the block returned here
	WORD      length;						// length of the block returned
	BYTE      data[500];					// data area - a portion of a NEW_RECORD structure
} QSM_RSP_RECORD, *LPQSM_RSP_RECORD;

typedef struct _CMP_BITS {
	WORD  sequence : 4;
	WORD  source : 2;
	WORD  hot : 1;
	WORD  accession : 4;
	WORD  origSource : 2;
	WORD  sourceEqual : 1;
	WORD  resv : 2;
} CMP_BITS;

typedef struct _CMP_NEWS_HEADER {			// Header used during Huffman compression
	union {									// of story records
		WORD      flags;
		CMP_BITS  bits;
	};
	WORD  date;
	WORD  time;
	WORD  headlineOffset;
	WORD  headlineLength;
	WORD  storyOffset;
	WORD  storyLength;
	BYTE  data[1];
} CMP_NEWS_HEADER, *LPCMP_NEWS_HEADER;

typedef struct _QSM_REQ_STORY {
	BYTE      q, s;							// ASCII 'q' and 's'
	BYTE      compLevel;					// maximum compression level supported
	BYTE      resv;							// padding
	WORD      inst;							// instance handle (only needed if asking for more),
											// otherwise it should be 0xffff
	STORYHND  storyHnd;						// story handle to use in search
	WORD      offset;						// offset to start reading at
} QSM_REQ_STORY, *LPQSM_REQ_STORY;

typedef struct _QSM_RSP_STORY {
	BYTE  q, s;							    // ASCII 'q' and 's'
	BYTE  compLevel;					    // compression level used
	BYTE  status;							// status
	WORD  inst;							    // returned instance handle
	WORD  totalLen;						    // total length of the story
	WORD  offset;						    // offset of the block returned here
	WORD  length;						    // length of the block returned
	BYTE  data[500];					    // data area
} QSM_RSP_STORY, *LPQSM_RSP_STORY;

typedef struct _QSM_REQ_XSEARCH {
	BYTE  q, x;	  							// ASCII 'q' and 'x'
	BYTE  compLevel;						// maximum compression level supported
											// high-order bit set - means there is more
	BYTE  reqType;  						// request type: 0=today list,  1=by category
		  									//               2=by seaymbol, 3=by date
											// high-order bit set - perform asynchronous search
	BYTE  sourceMap[NWSSRC_MASKSIZE];		// news source bit map
	WORD  startDate;						// start date (0=beginning)
	WORD  endDate;		   					// end date (0=today)
	WORD  maxStories;						// maximum story handles for this search
	char  szSymCat[494];	   				// variable depending on search type... see below
} QSM_REQ_XSEARCH, *LPQSM_REQ_XSEARCH;

//
// The szSymCat structure member should be in the following format for each search type-
//
// 		REQTYPE_TODAY    	nothing.
//
//		REQTYPE_CATEGORY 	category codes separated by \t''s
//							startDate and endDate are checked
//							Ex. N/IND\tR/IL\tR/US
//
//		REQTYPE_SYMBOL   	symbols separated by \t''s
//							startDate and endDate are checked
//							Ex. MSFT\tIBM\tDEC
//
//		REQTYPE_DATE     	nothing
//
//		REQTYPE_REFERENCE	reference number (sequence number)
//
//		REQTYPE_BOOLEAN  	a boolean expression of symbol and category codes.
//							category codes are marked with an '*' as the first 
//							character.  Currently, the expression only supports 
//							logical or (+).
//							Ex. MSFT+IBM+DEC+*N/IND+*R/US+F+*N/CMP
//
//		REQTYPE_ACCESSION	accession number
//
//		REQTYPE_TRUEBOOL	true boolean expression of symbol and category codes
//							- string is in valid postfix or RPN notation
//							- Valid operations consist of  & | ~ where:
//								& is AND or INTERSECT
//								| is OR or UNION
//								~ is BUT NOT or DISJOINT
//							- Tokens are separated by blanks
//							- Category codes start with '*' as first character
//							Ex. IBM MSFT | *HIGHTECH &
//

typedef struct _QSM_RSP_XSEARCH {
	BYTE  q, x;  							// ASCII 'q' and 'x'
	BYTE  compLevel;						// compression level used
	BYTE  status;							// status
	WORD  inst;								// "instance" handle for the search result
	WORD  totalPtrs;						// total number of stories (handles) found
	WORD  num;								// number of pointers returned in this message
	BYTE  data[502]; 						// "last n" pointers that will fit in this message
} QSM_RSP_XSEARCH, *LPQSM_RSP_XSEARCH;

// Additional search information when it all doesn't fit within qx
#define	LASTREC		0x01
#define	ACTIONBIT	0x02

typedef struct _QSM_REQ_ZSEARCH {
	BYTE  q, z;	  							// ASCII 'q' and 'z'
	WORD  inst;
	WORD  flag;                             // bit 0: last record in set
											// bit 1: set means OR, clear means AND
	WORD  offset;
	WORD  totalLen;
	WORD  resv;
	char  szSymCat[500];
} QSM_REQ_ZSEARCH, *LPQSM_REQ_ZSEARCH;

typedef struct _QSM_RSP_ZSEARCH {
	BYTE  q, z;	  							// ASCII 'q' and 'z'
	WORD  inst;
	BYTE  status;
} QSM_RSP_ZSEARCH, *LPQSM_RSP_ZSEARCH;

typedef struct _QSM_REQ_RSEARCH {
	BYTE  q, t;  							// ASCII 'q' and 't'
	BYTE  compLevel;						// maximum compression level supported
	BYTE  resv;								// padding
	WORD  inst;								// instance handle
	WORD  index;							// index for first story pointer
	WORD  num;								// number requested
} QSM_REQ_RSEARCH, *LPQSM_REQ_RSEARCH;

typedef struct _QSM_RSP_RSEARCH {
	BYTE  q, t;  							// ASCII 'q' and 't'
	BYTE  compLevel;						// compression level used
	BYTE  status;							// status
	WORD  total;							// total story handles for this instance handle
	WORD  index;							// index for the first story handle in the buffer
	WORD  length;							// length of the buffer
	WORD  num;								// number of pointers returned in this message
	BYTE  data[500];						// data area
} QSM_RSP_RSEARCH, *LPQSM_RSP_RSEARCH;

typedef struct _QSM_REQ_COMPDATA {
	BYTE  q, m;								// ASCII 'q' and 'm'
	BYTE  compLevel;						// Compression type		1:  Huffman, others undefined
	BYTE  resv;								// padding
	BYTE  dataType;							// 0: CRCs; 1: Compression Counts/Data
	BYTE  newsSource;						// News Source Number currently 0-63
} QSM_REQ_COMPDATA, *LPQSM_REQ_COMPDATA;

typedef struct _QSM_RSP_COMPDATA {
	BYTE  q, m;								// ASCII 'q' and 'm'
	BYTE  compLevel;						// Compression type		1:  Huffman, others undefined
	BYTE  status;							// status value 0: OK, -1: NOT supported
	WORD  resv;                             // padding
	WORD  count;                            // Number of elements in data array (specific for each compression type)
	WORD  length;							// Length of data
	BYTE  data[502];						// data area
} QSM_RSP_COMPDATA, *LPQSM_RSP_COMPDATA;

//--------------------------------------------------------------------------------
// Reuters SF Plus messages

typedef	struct _QSM_RPNAC {
	BYTE	O,P;			                // This message is used to get Story text by PNAC
	WORD	Status;			                // Status (Sent = 0)
	char	PNAC[14];		                // The PNAC.. obtained from the story
	BYTE	flag;
} QSM_RPNAC, *LPQSM_RPNAC;

typedef	struct _QSM_PNACID {
	BYTE	O,P;		                	// This message is used to get Story text by PNAC
	WORD	Status;		                	// Status (Sent = 0)
	char	szFN[14];	                	// The name of the file for the story
} QSM_PNACID, *LPQSM_PNACID;

//--------------------------------------------------------------------------------
// network messages

typedef struct _QSU_NEWSEVENT {
	BYTE      W;							// ASCII 'W'                            
	BYTE      source;						// NT_xxx                               
	STORYPTR  sp;							// Story Pointer (needs QSN to be real) 
	WORD      nTime;						// Time of story                        
	WORD      nDate;						// Date of story packed                 
	BYTE      nSymCats;						// Number of SymCats that follow        
	char      symCatTitle[499];				// Symbols,Cats and The title           
} QSU_NEWSEVENT, *LPQSU_NEWSEVENT;

typedef struct _QSU_PROFILEEVENT {
	BYTE      m;							// ASCII 'm'                               
	BYTE      flags;						// old padding - NOW Flags
											// bit 0: Update contains Generic Cats
	STORYHND  storyHnd;						// story handle of this story              
	WORD	  sequence;						// packet sequence number                  
	WORD	  total;						// total packets                           
	DWORD	  totalLength;					// total length of all packets             
	WORD      dataLength;					// length of the data area of this message 
	char      data[482];					// data area - variable                    
} QSU_PROFILEEVENT, *LPQSU_PROFILEEVENT;

//
// The data portion of the QSU_PROFILEEVENT message consists of a null terminated string
// of symbols and category codes followed by the headline, which is also null terminated.  
// Symbols and category codes are formatted as separate fields terminated by field 
// separators (0x1c). Category codes are further separated in the following different 
// groups-Industry, Government, Page, Subject, Market Sector, Product/Service, Geographic 
// Region, Statistical Table, and Journal.  Each of these groups is separated by field 
// separators. Please note that the maximum size of *any* key is 16 bytes.
//
// Example- (Dow Jones story)
//		AN MOB OXY SUN TX\x1cI/OIL I/OIS\x1c\x1c\x1cN/HIY N/PET\x1cM/ENE\x1c\x1c
//		R/CA R/IL R/NY R/PA R/PRM R/US R/VA\x1c\x1c\x1c
//

typedef struct _QSU_SCROLL {
	BYTE	msg;							// Message type 'n' or 'w'
	BYTE	dataLen;						// Length of data
	BYTE	source;							// Source: 1= DJB, etc.
	BYTE	flag;							// First/Middle/Last line flag
	char	text[1];						// Text of story
} QSU_SCROLL, *LPQSU_SCROLL;

typedef struct _QSU_NEWSUPDATE {
	BYTE	N;								// Message type 'N'          
	BYTE	source;							// Source of data            
	DWORD	storyPtr;						// Story pointer             
	BYTE	nCatSyms;						// Number of Cats or Syms    
	char	catSymHead[1];					// {SsymCat...\0}{Headline\0}
} QSU_NEWSUPDATE, *LPQSU_NEWSUPDATE;

// AT Server messages

typedef struct _ATN_NEWS {
	DWORD	type;
	DWORD	qsdtID;				
	DWORD	length;
	BYTE	N;								// Message type 'N'
	BYTE	source;
	DWORD	StoryPtr;
	WORD	nTime;							// Time (added 3/1/93)
	BYTE	nCatSyms;
	BYTE	CatSymHead[1];					// {SsymCat...\0}{Headline\0}
} ATN_NEWS, *LPATN_NEWS;

typedef struct _ATN_SCROLL {				// Scrolling news message
	DWORD	type;
	DWORD	qsdtID;				
	DWORD	length;
	char	n;
	char	linelength;
	char	source;
	char	flag;
	char	news[128];
} ATN_SCROLL, *LPATN_SCROLL;

typedef struct _ATN_NEWSEVENT {
	DWORD	  type;							// packet type - ATD_PROFILEEVENT
	DWORD	  qsdtID;                       // QSDTID of this packet      
	DWORD	  length;                       // total length of this packet
	BYTE      W;							// ASCII 'W'                            
	BYTE	  source;						// NT_xxx                               
	STORYPTR  sp;							// Story Pointer (needs QSN to be real) 
	WORD	  nTime;						// Time of story                        
	WORD	  nDate;						// Date of story packed                 
	BYTE	  nSymCats;						// Number of SymCats that follow        
	char	  SymCatTitle[499];				// Symbols,Cats and The title           
} ATN_NEWSEVENT, *LPATN_NEWSEVENT;

#define	GENCATS		0x01

typedef struct _ATN_PROFILEEVENT {
	DWORD	  type;							// packet type - ATD_PROFILEEVENT
	DWORD	  qsdtID;                       // QSDTID of this packet      
	DWORD	  length;                       // total length of this packet
	BYTE      m;							// ASCII 'm'                               
	BYTE      flags;						// old padding - NOW Flags
											// bit 0: Update contains Generic Cats
	STORYHND  storyHnd;						// story handle of this story              
	WORD	  sequence;						// packet sequence number                  
	WORD	  total;						// total packets                           
	DWORD	  totalLength;					// total length of all packets             
	WORD      dataLength;					// length of the data area of this message 
	char      data[482];					// data area - variable                    
} ATN_PROFILEEVENT, *LPATN_PROFILEEVENT;

//
// The data portion of the ATN_PROFILEEVENT message consists of a null terminated string
// of symbols and category codes followed by the headline, which is also null terminated.  
// Symbols and category codes are formatted as separate fields terminated by field 
// separators (0x1c). Category codes are further separated in the following different 
// groups-Industry, Government, Page, Subject, Market Sector, Product/Service, Geographic 
// Region, Statistical Table, and Journal.  Each of these groups is separated by field 
// separators. Please note that the maximum size of *any* key is 16 bytes.
//
// Example- (Dow Jones story)
//		AN MOB OXY SUN TX\x1cI/OIL I/OIS\x1c\x1c\x1cN/HIY N/PET\x1cM/ENE\x1c\x1c
//		R/CA R/IL R/NY R/PA R/PRM R/US R/VA\x1c\x1c\x1c
//
// The full update event can consist of multiple packets that must be reassembled
// at the workstation.
//


typedef struct _QSU_SEARCHEVENT {
	BYTE  S;							// ASCII 'S'                               
	BYTE  resv;							// padding                                 
	BYTE  compLevel;					// compression level used
	BYTE  status;						// status
	WORD  inst;							// "instance" handle for the search result
	WORD  totalPtrs;					// total number of stories (handles) found
	WORD  num;							// number of pointers returned in this message
	BYTE  data[502]; 					// "last n" pointers that will fit in this message
} QSU_SEARCHEVENT, *LPQSU_SEARCHEVENT;

typedef struct _ATN_SEARCHEVENT {
	DWORD type;							// packet type - ATD_SEARCHEVENT
	DWORD qsdtID;                       // QSDTID of this packet      
	DWORD length;                       // total length of this packet
	BYTE  S;							// ASCII 'S'                               
	BYTE  resv;							// padding                                 
	BYTE  compLevel;					// compression level used
	BYTE  status;						// status
	WORD  inst;							// "instance" handle for the search result
	WORD  totalPtrs;					// total number of stories (handles) found
	WORD  num;							// number of pointers returned in this message
	BYTE  data[502]; 					// "last n" pointers that will fit in this message
} ATN_SEARCHEVENT, *LPATN_SEARCHEVENT;


typedef struct _QSU_INDEXUPD {
	BYTE	R;
	BYTE	resv;
	BYTE	data[510];					// Filter items that had index modifications
} QSU_INDEXUPD, *LPQSU_INDEXUPD;

typedef struct _ATN_INDEXUPD {
	DWORD type;							// packet type - ATD_INDEXUPDATE
	DWORD qsdtID;                       // QSDTID of this packet      
	DWORD length;                       // total length of this packet
	BYTE	R;
	BYTE	resv;
	BYTE	data[510];					// Filter items that had index modifications
} ATN_INDEXUPD, *LPATN_INDEXUPD;

#pragma pack()

/*===========================================================================*/

#endif

/*===========================================================================*/
