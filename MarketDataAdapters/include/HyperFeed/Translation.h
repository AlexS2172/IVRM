///////////////////////////////////////////
//
//		Multi purpose reference to TICKER_KEY translation
//
///////////////////////////////////////////

#ifndef __TRANSLATION_INCL__
#define __TRANSLATION_INCL__

#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif

//  input/output source/type
#define		TRANSLATION_TYPE_TICKER				0
#define		TRANSLATION_TYPE_CUSIP				1
#define		TRANSLATION_TYPE_VALOREN			2
#define		TRANSLATION_TYPE_SEDOL				3
#define		TRANSLATION_TYPE_ISIN				4
#define		TRANSLATION_TYPE_CEDEL				5
#define		TRANSLATION_TYPE_GERMAN				6
#define		TRANSLATION_TYPE_BELGIAN			7
#define		TRANSLATION_TYPE_STRING				8
#define		TRANSLATION_TYPE_PAGE				9
#define		TRANSLATION_TYPE_BRIDGE				10
#define		TRANSLATION_TYPE_SRC_CBOT			11		
#define		TRANSLATION_TYPE_SRC_CME			12	
#define		TRANSLATION_TYPE_SRC_CANADA			13		
#define		TRANSLATION_TYPE_SRC_CEC			14	
#define		TRANSLATION_TYPE_SRC_OPRA			15		
#define		TRANSLATION_TYPE_SRC_ITC21			16		
#define		TRANSLATION_TYPE_SRC_HYPERFEED		17
#define		TRANSLATION_TYPE_SRC_TELEKURS_XLATE	18			
#define		TRANSLATION_TYPE_SRC_COMSTOCK		19
				

typedef struct _TRANSLATION_BITS_DBA {
#if defined( PQTENDIAN_LITTLE )
	BIT_FIELD		fDeleted 		:1;		//  record deleted
	BIT_FIELD		fNoAutoDelete	:1;		//  do NOT age
	BIT_FIELD		fLocked			:1;		//	data locked from updates
	BIT_FIELD		fChanged		:1;		//  data has changed
	BIT_FIELD		fInactive		:1;		//  record is inactive
	BIT_FIELD		reserved 		:11;
#elif defined( PQTENDIAN_BIG )
	BIT_FIELD		reserved 		:11;
	BIT_FIELD		fInactive		:1;
	BIT_FIELD		fChanged		:1;
	BIT_FIELD		fLocked			:1;
	BIT_FIELD		fNoAutoDelete	:1;
	BIT_FIELD		fDeleted 		:1; 

#endif

} TRANSLATION_BITS_DBA;

typedef union	_TRANSLATION_FLAGS {
	TRANSLATION_BITS_DBA	bits;
	USHORT 					mask;
} TRANSLATION_FLAGS;

typedef	union _TRANSLATION_REFERENCE
{
	TICKER_KEY		key;
	CHAR			text[64];
}	TRANSLATION_REFERENCE;

typedef struct _TRANSLATION_KEY_0
{													
	USHORT					inSource;			//  input source/type
	USHORT					outSource;			//  output source/type
	TRANSLATION_REFERENCE	inReference;		//  input symbol Translation key
} TRANSLATION_KEY_0;

typedef struct _TRANSLATION_KEY_1
{													
	USHORT					outSource;			//  output source/type
	USHORT					inSource;			//  input source/type
	TRANSLATION_REFERENCE	outReference;		//  input symbol Translation key
} TRANSLATION_KEY_1;

typedef struct _TRANSLATION_KEY_2
{													
	USHORT					inSource;			//  input source/type
	TRANSLATION_REFERENCE	inReference;		//  input symbol Translation key
	USHORT					outSource;			//  output source/type
} TRANSLATION_KEY_2;

typedef	union	_TRANSLATION_KEY
{
	TRANSLATION_KEY_0			inKey;
	TRANSLATION_KEY_1			outKey;
	TRANSLATION_KEY_2			searchKey;
}	TRANSLATION_KEY, *lpTRANSLATION_KEY;

typedef	struct	_TRANSLATION_FUTURES_DATA
{
	USHORT				priceType;
	SHORT				strikeScale; 
	SHORT				session; 
}	TRANSLATION_FUTURES_DATA;

typedef	union	_TRANSLATION_DATA
{
	TRANSLATION_FUTURES_DATA	futuresData;
	CHAR						miscData[16];
}	TRANSLATION_DATA;

typedef struct _TRANSLATION_RECORD
{
	USHORT							inSource;			//  input source/type
	TRANSLATION_REFERENCE			inReference;
	USHORT							outSource;			
	TRANSLATION_REFERENCE			outReference;
	USHORT							row;
	USHORT							column;
	TRANSLATION_FLAGS				flags;
	UCHAR							reserved[2];
	ULONG	                        date;
	CHAR							description[48];
	TRANSLATION_DATA				data;				
} TRANSLATION_RECORD, *lpTRANSLATION_RECORD;

#define DBA_RECORD_79   TRANSLATION_RECORD
#define DBA_KEY_79		TRANSLATION_KEY

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif

#endif