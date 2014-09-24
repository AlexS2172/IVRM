/*
 *		Struct.h		Date=981201
 *
 *		Common Structures and Flags for ATlas, ATDataManager, and Technical Studies Library
 *
 *		Written 02/03/98	S. Xia
 *
 *		Modified 07/09/98	C. Baker
 *			Moved CHART_ENTRY_DATA from AtheDlg.h
 *			Deleted ATDM_PRICE_VOLUME
 *
 *		Modified 11/27/98	C. Baker
 *			Added Union to ATDM_TIME
 *
 *		Modified 12/01/98	C. Baker
 *			Fixed Union to ATDM_TIME
 *
 *		Modified 12/18/98   Xia
 *         Added Study scales enum
 *
 *		Copyright (c) A-T Financial Information, Inc., 1998.
 *		All rignts reserved.
 *
 */

/*=============================================================*/

#ifndef _STRUCT_H_
#define _STRUCT_H_

#define ATDM_NUM_TICK_FIELDS    3
#define ATDM_NUM_BAR_FIELDS		6
#define ATDM_NUM_PF_FIELDS		3


typedef struct _ATDM_TIME {
	union {
		struct {
			DWORD min: 6,
				  hour: 5,
				  day: 5,
				  month: 4,
				  year: 7,
				  unused: 5;
		};
		DWORD value;
	};
} ATDM_TIME; //time structure defined as bits

//This structure is defined in data manager's .h
typedef struct _ATDM_BAR_FIELDS
{
	double dLast;
	double dHigh;
	double dLow;
	double dOpen;
	double dVolume;
	double dOpenInt;
} ATDM_BAR_FIELDS;

typedef struct _ATDM_TICK_FIELDS
{
	double dPrice;
	double dSize;
	double dType; //trade = TOSC_REGULAR, bid = TOSC_BID, ask = TOSC_ASK
} ATDM_TICK_FIELDS;

//This structure is defined for point & figure display
typedef struct _ATDM_POINT_FIGURE
{
	double dPriceAtReversal; //This is not exactly the price
	double dNumOfMarks;
	double dXOFlag; //1 = X, -1 = O
} ATDM_POINT_FIGURE;

// These are data types.

#define	ATDM_TYPE_BAR			 0x0001
#define	ATDM_TYPE_TICK			 0x0002
#define	ATDM_TYPE_STUDY			 0x0004
#define ATDM_TYPE_PRICE_VOLUME	 0x0008
#define	ATDM_TYPE_POINT_FIGURE	 0x0010
#define ATDM_TYPE_ALL			 0xffff

// Time Period ID definition
enum PERIOD {TP_TICK, TP_MINUTE, TP_VARMINUTE, TP_DAILY, TP_WEEKLY,
			 TP_MONTHLY, TP_EQU_TICK, TP_UNDEFINED};

//line styles
enum ATC_BSTYLE  {LINE_BS_LINE = 101, LINE_BS_BAR, 
LINE_BS_CANDLE, LINE_BS_HISTOGRAM, LINE_BS_HISTOLINE, LINE_BS_PT_AND_FIG,
LINE_BS_STUDY, LINE_BS_PRICE_VOLUME, LINE_BS_TICK};

//study scales
enum ATC_STUDY_SCALES {STUDY_SCALE_ANY = 201, STUDY_SCALE_NON_NEGATIVE, STUDY_SCALE_0_100,
STUDY_SCALE_NEG100_100}; 

// Base Data Dialog Box structure - defined here because DMTCPMSG uses it
typedef struct _CHART_ENTRY_DATA
{
	DWORD splitAdjust: 2, //BST_CHECKED, BST_UNCHECKED
		  omitZeros: 2,
		  trades: 2,
		  bids: 2,
		  asks: 2,
		  quotes: 2,
		  filter: 2,
		  timeLimits: 2,
		  unused: 16;
} CHART_ENTRY_DATA; //to be inserted in the base_data structure

#define AT_YEAR_OFFSET 1980  // Dates from ATServer are all offset by this year

#endif
