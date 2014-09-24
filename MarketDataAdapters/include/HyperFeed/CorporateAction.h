///////////////////////////////////////////
//
//  Dividend, splits, symbol change database
//
//
///////////////////////////////////////////

#ifndef __CORPORATE_ACTION_INCL__
#define __CORPORATE_ACTION_INCL__


#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif


#define   CORPORATE_ACTION_TYPE_EXDIV			1
#define   CORPORATE_ACTION_TYPE_SPLIT			2
#define   CORPORATE_ACTION_TYPE_CHANGE			3
#define   CORPORATE_ACTION_TYPE_DELIST			4
#define   CORPORATE_ACTION_TYPE_IPO				5
#define   CORPORATE_ACTION_MAX_TYPE				5

typedef struct  _CORPORATE_ACTION_KEY
{
	LONG        applyDate;
	TICKER_KEY  item;
	USHORT		recordType;
} CORPORATE_ACTION_KEY;                             // 30 bytes

typedef struct  CORPORATE_ACTION_BITS {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD   deleted :1;
	BIT_FIELD   changed :1;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD   changed :1;
	BIT_FIELD   deleted :1;
#endif
} CORPORATE_ACTION_BITS;                            // 4 bytes

typedef union _CORPORATE_ACTION_FLAGS {
	CORPORATE_ACTION_BITS   bits;                   
	ULONG           mask;
} CORPORATE_ACTION_FLAGS;                           // 4 bytes

typedef struct  CORPORATE_ACTION_SPLIT {
	LONG        toShares;
	LONG        fromShares;
	BOOL        stockDist;
} CORPORATE_ACTION_SPLIT;                           // 10 bytes

typedef struct  CORPORATE_ACTION_SYMBOL_CHANGE{
    TICKER_KEY  newItem;
} CORPORATE_ACTION_SYMBOL_CHANGE;                   // 22 bytes

typedef struct  CORPORATE_ACTION_IPO{
    LONG    price;
	USHORT  priceType;
} CORPORATE_ACTION_IPO;                             // 6 bytes

typedef struct  CORPORATE_ACTION_DIVIDEND{
    SHORT   dividendCount;
	USHORT  priceType;
    LONG    dividend[4];
} CORPORATE_ACTION_DIVIDEND;                        // 20 bytes

typedef   struct  _CORPORATE_ACTION_RECORD
{
	CORPORATE_ACTION_KEY    key;
    CORPORATE_ACTION_FLAGS  flags;
	LONG                    actionDate;         // no bits CreateDate, changed bit ChangeDate, delete bit DeleteDate

    union
	{
		CORPORATE_ACTION_SPLIT         splits;
		CORPORATE_ACTION_SYMBOL_CHANGE newName;
        CORPORATE_ACTION_IPO           ipo;
        CORPORATE_ACTION_DIVIDEND      divs;
		CHAR          data[92];
	} action;

} CORPORATE_ACTION_RECORD;                          // 128 bytes

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif



#endif