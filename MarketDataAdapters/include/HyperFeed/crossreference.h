/////////////////////////////////////////
//
//
//
/////////////////////////////////////////
#ifndef __CROSS_REFERENCE_INCL__
#define __CROSS_REFERENCE_INCL__

#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif
#include  <wtitem.h>


#define		ID_ITEM_KEY			0
#define		ID_ISSUE_KEY		1

#define		MAX_ID_TYPES		16
#define		MAX_ID_SIZE			24

#define		ID_TYPE_TICKER		0
#define		ID_TYPE_CUSIP		1
#define		ID_TYPE_VALOREN		2
#define		ID_TYPE_SEDOL		3
#define		ID_TYPE_ISIN		4
#define		ID_TYPE_CEDEL		5
#define		ID_TYPE_GERMAN		6
#define		ID_TYPE_BELGIAN		7
#define		ID_TYPE_MLT         8

typedef struct
{
	BYTE    century;
	BYTE    year;
	BYTE    month;
	BYTE    day;
}	I_DATE, *lpI_DATE;

typedef union
{
	I_DATE  date;
	ULONG   lDate;

}	CROSS_REFERENCE_DATE;

typedef	union
{
	TICKER_KEY			key;
	CHAR				id[MAX_ID_SIZE];
}	XREF, *lpXREF;



typedef struct
{
	TICKER_KEY		item;
	SHORT			idType;		
}	CROSS_REFERENCE_KEY_0, *lpCROSS_REFERENCE_KEY_0;


typedef struct
{
	XREF            xref;
	SHORT           idType;
}	CROSS_REFERENCE_KEY_1, *lpCROSS_REFERENCE_KEY_1;


typedef struct
{

	union
	{
		CROSS_REFERENCE_KEY_0  itemKey;
		CROSS_REFERENCE_KEY_1  xrefKey;
	};
}	CROSS_REFERENCE_KEY, *lpCROSS_REFERENCE_KEY;

typedef struct _CROSS_REFERENCE_BITS {
#if defined( PQTENDIAN_LITTLE )
	ULONG		deleted		:1;
	ULONG		changed		:1;
    ULONG       cusipUpd	:1;
	ULONG		nyseUpd		:1;	
	ULONG		telekUpd	:1;
	ULONG		ftUpd		:1;
	ULONG		mfUpd		:1;
	ULONG		partUpd		:1;
	ULONG		fill		:7;
	ULONG		fill1		:16;
	ULONG		locked		:1;
#elif defined( PQTENDIAN_BIG )
	ULONG		locked		:1;
	ULONG		fill1		:16;
	ULONG		fill		:7;
	ULONG		partUpd		:1;
	ULONG		mfUpd		:1;
	ULONG		ftUpd		:1;
	ULONG		telekUpd	:1;
	ULONG		nyseUpd		:1;	
    ULONG       cusipUpd	:1;
	ULONG		changed		:1;
	ULONG		deleted		:1;
#endif
} CROSS_REFERENCE_BITS;

typedef union _CROSS_REFERENCE_FLAGS {
  CROSS_REFERENCE_BITS bits;
  ULONG        mask;
} CROSS_REFERENCE_FLAGS;


typedef struct
{
	CROSS_REFERENCE_KEY_0		key;
	XREF						xref;
	SHORT						row;
	SHORT						column;
	
	CHAR						name[48];
	CHAR						description[32];
	CHAR						fill[16];
	CROSS_REFERENCE_FLAGS		flags;
	CROSS_REFERENCE_DATE		addDate;         
	CROSS_REFERENCE_DATE		updDate;
	CROSS_REFERENCE_DATE		delDate;
  
} CROSS_REFERENCE_RECORD, *lpCROSS_REFERENCE_RECORD;


#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif

#endif
