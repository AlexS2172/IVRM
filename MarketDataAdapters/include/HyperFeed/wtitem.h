
#ifndef __WT_ITEM_INCL__
#define __WT_ITEM_INCL__


typedef struct _WT_ITEM 
{
	USHORT	type;
	UCHAR 	symbol[14];
	UCHAR 	countryCode[2];
	UCHAR 	exchangeCode[2];
} WT_ITEM;	
				// 20 bytes

#endif