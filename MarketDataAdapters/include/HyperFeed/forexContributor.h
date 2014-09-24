/////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////

#ifndef __FOREXCONTRIB_INCL__
#define __FOREXCONTRIB_INCL__


#ifdef __cplusplus				// C++ compiler is being used
	#ifndef __unix //unix sdk requires c++
		extern "C" {
	#endif
#endif

#ifndef INCL_NOPACKING
	#pragma pack(2)						 /* Word aligned. */
#endif



typedef struct
{
	USHORT			contributorCode;
}	FOREX_CONTRIBUTOR_KEY,  *lpFOREX_CONTRIBUTOR_KEY;

typedef struct
{
	CHAR		countryCountryCode[2];
	CHAR		exchangeCode[2];
}	FOREX_CONTRIBUTOR_KEY_1,  *lpFOREX_CONTRIBUTOR_KEY_1;

typedef	union _FOREX_CONTRIBUTOR_FLAGS
{
	struct
	{
		ULONG		enabled				:1;
		ULONG		deleted				:1;

	}	bits;
	ULONG				mask;
}	FOREX_CONTRIBUTOR_FLAGS;

typedef union	_FOREX_CONTRIBUTOR_DATE
{
	struct
	{
		BYTE			century;			// century
		BYTE			year;				// year
		BYTE			month;				// month
		BYTE			day;				// day
	}	date;
	ULONG	ldate;
}	FOREX_CONTRIBUTOR_DATE;
 


typedef struct
{
	FOREX_CONTRIBUTOR_KEY	key;
	USHORT					usReserved;
	FOREX_CONTRIBUTOR_FLAGS	flags;
	FOREX_CONTRIBUTOR_DATE	deleteDate;
	CHAR					countryCode[2];
	CHAR					exchangeCode[2];
	SHORT					session;
	CHAR  					description[30];			//  session title

}	FOREX_CONTRIBUTOR_RECORD, * lpFOREX_CONTRIBUTOR_RECORD;

#ifndef INCL_NOPACKING
	#pragma pack() 						 /* Word aligned. */
#endif

#ifdef __cplusplus	// C++ compiler is being used
	#ifndef __unix
		   }
	#endif
#endif

#endif

 