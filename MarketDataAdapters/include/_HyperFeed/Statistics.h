
#ifndef	__STAT_RECORD_INCL__
#define	__STAT_RECORD_INCL__




#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

//		stats indexs

#define	NUM_MKT_STATS		3

#define	MKT_STATS_COMP		0
#define	MKT_STATS_NASDAQ	1
#define	MKT_STATS_CALLS		1
#define	MKT_STATS_PUTS		2






#define	NUM_TOP15_LISTS	6

#define	TOP15_COMP		0
#define	TOP15_REGN		1
#define	TOP15_COMP_CALL	2
#define	TOP15_REGN_CALL	3
#define	TOP15_COMP_PUT	4
#define	TOP15_REGN_PUT	5

#define	NUM_TOP15_LISTS_NAMES	7

#define	TOP15_ACTIVES		0
#define	TOP15_GAINERS		1
#define	TOP15_LOSERS		2
#define	TOP15_NET_GAINERS	3
#define	TOP15_NET_LOSERS	4
#define	TOP15_LATE_ACTIVE	5
#define	TOP15_LATE_TICKS	6


//		market stats types

#define SSTYPE_VOLUME		0
#define SSTYPE_ADVANCES		1
#define SSTYPE_UPVOLUME 	2
#define SSTYPE_DECLINES 	3
#define SSTYPE_DOWNVOLUME	4
#define SSTYPE_ISSUES		5
#define SSTYPE_TICK			6
#define SSTYPE_TRIN			7
#define SSTYPE_NONE			-1



/*
typedef struct _STATISTICS_BITS {
	BIT_FIELD		volumeChanged		:1;
	BIT_FIELD		advancesChanged		:1;
	BIT_FIELD		upVolumeChanged		:1;
	BIT_FIELD		declinesChanged		:1;
	BIT_FIELD		downVolumeChanged	:1;
	BIT_FIELD		issuesChanged		:1;
	BIT_FIELD		tickChanged 		:1;
	BIT_FIELD		trinChanged 		:1;

	BIT_FIELD		activesChanged 		:1;
	BIT_FIELD		gainersChanged 		:1;
	BIT_FIELD		losersChanged		:1;
	BIT_FIELD		netGainersChanged   :1;
	BIT_FIELD		netLosersChanged	:1;
	BIT_FIELD		regActivesChanged   :1;
	BIT_FIELD		regionalsValid 	    :1;	// Regionals stats configured.
	BIT_FIELD		lateActivesChanged  :1;
	BIT_FIELD		lateTicksChanged	:1;
	BIT_FIELD		lateValid			:1;
	BIT_FIELD		reserved2			:14;
} DBA_STATISTICS_BITS;
*/

typedef struct _DBA_MKT_BITS 
{
	BIT_FIELD		isValid				:1;
	BIT_FIELD		volumeChanged		:1;
	BIT_FIELD		advancesChanged		:1;
	BIT_FIELD		upVolumeChanged		:1;
	BIT_FIELD		declinesChanged		:1;
	BIT_FIELD		downVolumeChanged	:1;
	BIT_FIELD		issuesChanged		:1;
	BIT_FIELD		tickChanged 		:1;
	BIT_FIELD		trinChanged 		:1;

}	DBA_MKT_BITS;

typedef struct _DBA_TOP15_BITS 
{
	BIT_FIELD		isValid				:1;
	BIT_FIELD		isLateValid			:1;
	BIT_FIELD		activesChanged 		:1;
	BIT_FIELD		gainersChanged 		:1;
	BIT_FIELD		losersChanged		:1;
	BIT_FIELD		netGainersChanged   :1;
	BIT_FIELD		netLosersChanged	:1;
	BIT_FIELD		regActivesChanged   :1;
	BIT_FIELD		lateActivesChanged  :1;
	BIT_FIELD		lateTicksChanged	:1;


}	DBA_TOP15_BITS;


typedef union _DBA_MKT_FLAGS {

	DBA_MKT_BITS		bits;
	USHORT 				mask;
} DBA_MKT_FLAGS;

typedef union _DBA_TOP15_FLAGS {

	DBA_TOP15_BITS		bits;
	USHORT 				mask;
} DBA_TOP15_FLAGS;

typedef	struct	_STATISTICS_FLAGS_DBA
{
	union
	{
		struct
		{
			BIT_FIELD	open	:1;
			BIT_FIELD	deleted	:1;
		}		bits;
		USHORT	mask;
	}	recordFlags;

	

	DBA_MKT_FLAGS	mktFlags[NUM_MKT_STATS];
	DBA_TOP15_FLAGS	top15Flags[NUM_TOP15_LISTS];

}	STATISTICS_FLAGS_DBA;

typedef	union	_STATS_DATE
{
	struct	
	{
		CHAR	century;
		CHAR	year;
		CHAR	month;
		CHAR	day;
	}	date;
	LONG	lDate;
}	STATS_DATE;
		



typedef struct _TOP15_ITEM_DBA {
	TICKER_KEY				item;
	LONG					value;
} TOP15_ITEM_DBA, *lpTOP15_ITEM_DBA;

typedef struct _TOP15NET_ITEM_DBA {
	TICKER_KEY			item;
	LONG				value;
	SHORT				priceType;
} TOP15NET_ITEM_DBA, *lpTOP15NET_ITEM_DBA;


typedef	struct	_TOP15_LISTS
{
	USHORT				activeIndex;				// index of lowest active
	USHORT				gainersIndex;				// index of lowest gainer
	USHORT				losersIndex;				// index of highest loser
	USHORT				netGainersIndex;			// index of lowest gainer
	USHORT				netLosersIndex;				// index of highest loser
	USHORT				lateActiveIndex;			// index of lowest late traded active
	USHORT				lateTicksIndex;				// index of lowest late traded active

	TOP15_ITEM_DBA		actives[15];				// 15 most active instruments
	TOP15_ITEM_DBA		gainers[15];				// 15 biggest gainers
	TOP15_ITEM_DBA		losers[15]; 				// 15 biggest losers
	TOP15NET_ITEM_DBA	netGainers[15];				// 15 biggest net change gainers
	TOP15NET_ITEM_DBA	netLosers[15]; 				// 15 biggest net change losers
	TOP15_ITEM_DBA		lateActives[15];			// 15 most active late traded instruments
	TOP15_ITEM_DBA		lateTicks[15];				// 15 most active late traded instruments

}	TOP15_LISTS, *lpTOP15_LISTS;



typedef	struct	_MARKET_STATS
{
	LONGLONG		volume;						// exchange volume
	ULONG			advances;					// exchange advances
	LONGLONG		upVolume;					// exchange up volume
	ULONG			declines;					// exchange declines
	LONGLONG		downVolume; 				// exchange down volume
	ULONG			issues;						// issues traded
	LONG			tick; 						// exchange tick
	LONG			trin; 						// exchange trin
}	MARKET_STATS, *lpMARKET_STATS;			



//	statistics record

typedef struct _STATISTICS_RECORD_DBA 
{ 		
	TICKER_KEY				item;				//  record key
	STATISTICS_FLAGS_DBA	flags;					// bit field flags
	USHORT					session;
	USHORT					row;
	USHORT					column;
	STATS_DATE				updateDate;
	MARKET_STATS			mktStats[NUM_MKT_STATS];
	TOP15_LISTS				top15Lists[NUM_TOP15_LISTS];
	UCHAR 					reserved2[32];

} STATISTICS_RECORD_DBA;



#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif


#endif