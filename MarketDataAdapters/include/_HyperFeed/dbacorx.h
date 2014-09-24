#if !defined(DBA_CORX_INCL)
#define DBA_CORX_INCL
#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif
#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

#define CORX_MAX_KEYS	1

#define DBA_CORX_TYPE_CANCEL		0x8000
#define DBA_CORX_TYPE_MODIFIED		0x4000
#define DBA_CORX_TYPE_INSERTED		0x2000
#define DBA_CORX_TYPE_CORRECTED		0x1000
#define DBA_CORX_TYPE_COMP_OPEN		0x0080
#define DBA_CORX_TYPE_REGN_OPEN		0x0040

#define DBA_CORX_TYPE_COMP_ONLY		0x0800
#define DBA_CORX_TYPE_RETRAN		0x0400

#define CORX_CLOSE_COMP				0x0200
#define CORX_CLOSE_REGN				0x0100

#define CORX_NEW_COMP				0x08f0
#define CORX_NEW_COMP_HIGH			0x0080
#define CORX_NEW_COMP_LOW			0x0040
#define CORX_NEW_COMP_OPEN			0x0020
#define CORX_NEW_COMP_LAST			0x0010
#define CORX_NEW_COMP_IND			0x0800

#define CORX_NEW_REGN				0x040f
#define CORX_NEW_REGN_HIGH			0x0008
#define CORX_NEW_REGN_LOW			0x0004
#define CORX_NEW_REGN_OPEN			0x0002
#define CORX_NEW_REGN_LAST			0x0001
#define CORX_NEW_REGN_IND			0x0400

//		trade indicator flags

#define	CORX_UPD_LAST				0x0001
#define	CORX_UPD_HIGH				0x0002
#define	CORX_UPD_LOW				0x0004 
#define	CORX_UPD_OPEN				0x0008
#define	CORX_UPD_LAST_REGN			0x0010
#define	CORX_UPD_HIGH_REGN			0x0020
#define	CORX_UPD_LOW_REGN			0x0040
#define	CORX_UPD_OPEN_REGN			0x0080

#define DBA_CORX_MAX_ENTRY			32
#define MAX_CORX_BLOCKS				25
#define MAX_CORX_AUX				25

/*
** This is the same as a WTI_TIME.
*/
typedef struct _CORX_ITEM 
{
	USHORT	type;
	UCHAR 	symbol[12];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} CORX_ITEM;					// 18 bytes

typedef struct _CORX_TIME {
	BYTE		hour;
	BYTE		minute;
	BYTE		seconds;
	BYTE		hundredths;
} CORX_TIME;

#ifndef __DBA_PRICE_DEFINED
#define __DBA_PRICE_DEFINED
typedef struct _DBA_PRICE {	
	LONG		price;
	ULONG		size;
	CHAR		exchangeCode[2];
	CHAR		hour;
	CHAR		minute;
} DBA_PRICE;
#endif

//typedef struct _CORX_PRICE {
//	LONG		price;
//	ULONG 		size;
//	UCHAR 		exchangeCode[2];
//	CHAR		hour;
//	CHAR		minute;
//} CORX_PRICE;            // 12 bytes

typedef struct DBA_CORX_ITEM {
	ULONG			sequence;
	ULONG			corxSequence;
	DBA_PRICE 		regnPrice;
	DBA_PRICE		regnLow;
	DBA_PRICE		regnHigh;
	DBA_PRICE		regnLast;
	DBA_PRICE		compLow;
	DBA_PRICE		compHigh;
	DBA_PRICE		compLast;
	SHORT			cPriceType;
	SHORT			pPriceType;
	USHORT		    type;
	CHAR			exchange[2];
	SHORT			indicator;
	SHORT			updCode;
	CORX_TIME		time;
} DBA_CORX_ITEM;

typedef struct _DBA_CORX_REC {
	SHORT       usEntry;
	DBA_CORX_ITEM	corxData[DBA_CORX_MAX_ENTRY];
} DBA_CORX_REC;

typedef struct _DBA_CORXF_SAV {		// current key save structure
	ULONG	ulPrevEntry;
	ULONG	ulNextEntry;
	ULONG	ulCurrentEntry;
} DBA_CORXF_SAV;

typedef struct _DBA_CORXF_REC 
{		// File structure
	ULONG			ulPrevEntry;
	ULONG			ulNextEntry;
	DBA_CORX_REC	corxRec;
} DBA_CORXF_REC;

typedef struct _CORX_FILE_PTRS 
{
	ULONG			ulFirstEntry;
	ULONG			ulLastEntry;
} CORX_FILE_PTRS;

typedef struct _DBA_CORXM_REC 
{
	CORX_ITEM		item;
	ULONG		    usCount;
	ULONG			usBlks;
	
	CORX_FILE_PTRS	ulEntryPtrs[CORX_MAX_KEYS];
	DBA_CORXF_REC		corxfData;
} DBA_CORXM_REC;

#if (_MSC_VER > 800)
  
typedef struct DBA_CORX {
	USHORT		    updateMask;
	USHORT			compInd;
	DBA_PRICE		regnLast;
	DBA_PRICE		regnOpen;
	DBA_PRICE		compLast;
	DBA_PRICE		compOpen;
	DBA_CORX_ITEM item;  
} DBA_CORX;

#endif
#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif
#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif
#endif

