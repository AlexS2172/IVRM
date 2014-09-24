
#ifndef INCL_PCQW32
#define INCL_PCQW32

#include <edbx.h>
#include <dbpblk.h>

#ifdef __cplusplus									// C++ compiler is being used
   extern "C" {
#endif

//---------------------------//
// Get Message definitions   //
//---------------------------//

#define ETI_ITEM_TYPE 			0
#define ETI_SOURCE 				1

#define DBA_DATABASE 			100
#define DBA_FUNCTION 			101
#define DBA_ERROR 				102
#define DBANAME					103
#define DBAFILE					104

#define ETI_MESSAGE_TYPE		200

VOID	 EtiCount(INT);
BOOL   dballoc(DBHEADER*);
LONG   dbsrch(DBHEADER*,PVOID,SHORT,PVOID *);
BOOL   dbadd(DBHEADER*,PVOID,ULONG);
BOOL   dbdel(DBHEADER*,ULONG);
BOOL   dbsave(DBHEADER*);
SHORT  dbload(DBHEADER*);
SHORT  dbfree(DBHEADER*);
LONG   fbsrch(HANDLE,VOID *,USHORT, VOID *,USHORT);
VOID   *db_get_ptr(DBHEADER*,ULONG);

BOOL   dbAllocW32(DBAPBLK*);
BOOL   dbCreateW32(DBAPBLK*,DBHEADER *);
BOOL   dbOpenW32(DBAPBLK*,DBHEADER *);
LONG   dbSrchW32(DBAPBLK*,PVOID,SHORT,PVOID *);
BOOL   dbAddW32(DBAPBLK*,PVOID,ULONG);
BOOL   dbDelW32(DBAPBLK*,ULONG);
BOOL   dbAllocI(DBAPBLK *, DBAKEYPARMS *, UINT);
BOOL   dbLoadI(DBAPBLK *);
LONG   dbSrchI(DBAPBLK*,PVOID, UINT, PVOID *, INT);
BOOL   dbAddI(DBAPBLK*,PVOID,LONG);
BOOL   dbDelI(DBAPBLK*,ULONG);
BOOL   dbFreeI(DBAPBLK *);
BOOL   dbSaveW32(DBAPBLK*);
BOOL   dbLoadW32(DBAPBLK*);
BOOL   dbFreeW32(DBAPBLK*);
VOID   *dbGetPtrW32(DBAPBLK*,ULONG);
VOID   *dbGetPtrI(DBAPBLK*,ULONG, INT);
LONG   fbSrchW32(HANDLE, VOID *,ULONG,VOID *,ULONG);

BOOL keyinA(CHAR * , DBA_KEY *);
BOOL keyoutA(DBA_KEY *, CHAR * , USHORT );

BOOL keyinW(TCHAR * , DBA_KEY *);
BOOL keyoutW(DBA_KEY *, TCHAR * , USHORT );

DLLEXPORT TCHAR *TickerKeyToString( TICKER_KEY *pTickerKey, TCHAR *string, USHORT stringLen );
DLLEXPORT CHAR *TickerKeyToStringA( TICKER_KEY *pTickerKey, CHAR *string, USHORT stringLen );
DLLEXPORT TCHAR *TickerKeyToStringW( TICKER_KEY *pTickerKey, TCHAR *pszOutput, ULONG cchOutputLen);

DLLEXPORT BOOL StringToTickerKey(TCHAR *pszInput, TICKER_KEY *pTickerKey);
DLLEXPORT BOOL StringToTickerKeyA(CHAR *pszInput, TICKER_KEY *pTickerKey);
DLLEXPORT BOOL StringToTickerKeyW(TCHAR *pszInput, TICKER_KEY *pTickerKey);
	
INT DataExpand (UCHAR *compressString, INT compressLenIn, UCHAR *expandString, INT expandLenIn);
INT DataCompress (UCHAR *compressString, INT compressLenIn, UCHAR *expandString, INT expandLenIn);
BOOL DllName32(TCHAR *);
TCHAR *DllGetName32(VOID);

INT fmtpriceW(TCHAR *, LONG, USHORT);
INT	FormatPrice64W( TCHAR *pOutput, UINT outputSize, LONGLONG price, USHORT priceType );

INT fmtpriceA(CHAR *, LONG, USHORT);
INT	FormatPrice64A( CHAR *pOutput, UINT outputSize, LONGLONG price, USHORT priceType );

CHAR	*EtGetMessageA(ULONG function, ULONG number);
TCHAR	*EtGetMessageW(ULONG function, ULONG number);

VOID GetProcessNameA( CHAR *pszName, INT length );
VOID GetProcessNameW( TCHAR *pszName, INT length );

//
//		STIC  conversions
//

//		generic

DLLEXPORT CHAR *SticToStringA( STIC *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(STIC *pStic, TCHAR *pszOutput, INT cchOutputLen); 

DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, STIC *pStic); 
DLLEXPORT BOOL StringToSticA( CHAR *string, STIC *pStic); 


#ifndef PCQW32_EXPORTS
#ifdef UNICODE
		#define FormatPrice64 FormatPrice64W
		#define fmtprice fmtpriceW
		#define keyin keyinW
		#define keyout keyoutW
		#define EtGetMessage EtGetMessageW
		#define GetProcessName GetProcessNameW
		#define SticToString SticToStringW
		#define StringToStic StringToSticW
#else
		#define FormatPrice64 FormatPrice64A
		#define fmtprice fmtpriceA
		#define keyin keyinA
		#define keyout keyoutA
		#define EtGetMessage EtGetMessageA
		#define GetProcessName GetProcessNameA
		#define SticToString SticToStringA
		#define StringToStic StringToSticA

#endif // !UNICODE
#endif // !PCQW32

TCHAR *getIniName(VOID);
TCHAR *StartDBADll(VOID);
SHORT cKeyLenIn(USHORT usFunction ,USHORT usKeyNum, USHORT usDbNum);
SHORT cKeyLenOut(USHORT usFunction ,USHORT usKeyNum, USHORT usDbNum);
SHORT cMinKeyIn(USHORT usFunction ,USHORT usKeyNum, USHORT usDbNum);
SHORT cMinKeyOut(USHORT usFunction ,USHORT usKeyNum, USHORT usDbNum);
SHORT cXtraKeyIn(USHORT usFunction,USHORT usKeyNum, USHORT usDbNum);
SHORT cXtraKeyOut(USHORT usFunction,USHORT usKeyNum, USHORT usDbNum);
SHORT KeyLenValue( USHORT usKeyNum, USHORT usDbNum );
SHORT cBufrLenIn(USHORT usFunction, USHORT usDbNum );
SHORT cBufrLenOut(USHORT usFunction, USHORT usDbNum );
BOOL checkDbaRecord(INT function);

INT	_btwncpy(TCHAR *dst, CHAR *src, UINT count);
INT	_btwcpy(TCHAR *dst, CHAR *src);
INT	_wtbncpy(CHAR *dst, TCHAR *src, UINT count);
INT	_wtbcpy(CHAR *dst, TCHAR *src);
INT _btwncat(TCHAR *dst, CHAR *src, UINT count);

#ifdef _WIN32_WCE
#include <htipda.h>
#endif

#ifdef __cplusplus									// C++ compiler is being used
	}
#endif

#ifdef __cplusplus									// C++ compiler is being used

//	TICKER_KEY

DLLEXPORT CHAR *SticToStringA( TICKER_KEY *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_KEY *pStic, TCHAR *pszOutput, INT cchOutputLen); 


DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_KEY *pStic); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_KEY *pStic); 

//	TICKER_MM_KEY

DLLEXPORT CHAR *SticToStringA( TICKER_MM_KEY *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_MM_KEY *pStic, TCHAR *pszOutput, INT cchOutputLen); 

DLLEXPORT CHAR *SticToStringA( TICKER_MM_KEY *pStic, CHAR *string, INT stringLen, BOOL mmid ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_MM_KEY *pStic, TCHAR *pszOutput, INT cchOutputLen, BOOL mmid); 

DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_MM_KEY *pStic, CHAR *mmID); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_MM_KEY *pStic, TCHAR *mmID); 

//	TICKER_OPTION_KEY

DLLEXPORT CHAR *SticToStringA( TICKER_OPTION_KEY *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_OPTION_KEY *pStic, TCHAR *pszOutput, INT cchOutputLen); 


DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_OPTION_KEY *pStic); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_OPTION_KEY *pStic); 

//	TICKER_ITEM

DLLEXPORT CHAR *SticToStringA( TICKER_ITEM *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_ITEM *pStic, TCHAR *pszOutput, INT cchOutputLen); 

DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_ITEM *pStic); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_ITEM *pStic); 

//	TICKER_MM_ITEM

DLLEXPORT CHAR *SticToStringA( TICKER_MM_ITEM *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_MM_ITEM *pStic, TCHAR *pszOutput, INT cchOutputLen); 

DLLEXPORT CHAR *SticToStringA( TICKER_MM_ITEM *pStic, CHAR *string, INT stringLen, BOOL mmid ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_MM_ITEM *pStic, TCHAR *pszOutput, INT cchOutputLen, BOOL mmid); 


DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_MM_ITEM *pStic, CHAR *mmID); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_MM_ITEM *pStic, TCHAR *mmID); 

//	TICKER_OPTION_ITEM

DLLEXPORT CHAR *SticToStringA( TICKER_OPTION_ITEM *pStic, CHAR *string, INT stringLen ); 
DLLEXPORT TCHAR *SticToStringW(TICKER_OPTION_ITEM *pStic, TCHAR *pszOutput, INT cchOutputLen); 

DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_OPTION_ITEM *pStic); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_OPTION_ITEM *pStic); 

//	TICKER_ITEM explict "has currency"

DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_ITEM *pStic, BOOL hasCurrency); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_ITEM *pStic, BOOL hasCurrency); 


//	TICKER_OPTION_ITEM explict "has currency"

DLLEXPORT BOOL StringToSticA( CHAR *string, TICKER_OPTION_ITEM *pStic, BOOL hasCurrency); 
DLLEXPORT BOOL StringToSticW( TCHAR *pszOutput, TICKER_OPTION_ITEM *pStic, BOOL basCurrency); 



//	StringToStic			primary function ( all overloads call this ultimately )
//
//	arguments:
//		string				NULL terminated input string
//		pStic				output STIC element ( per overload )
//		isKey				if TRUE, specifies "KEY" type element, and will fill in default currency code
//								else specifies "ITEM" type element and currency code and reserved will not be filled in
//		hasCurrency			if TRUE, will initialize currency key and reserved fields to spaces and NULL repectively, regardless
//								of whether KEY or ITEM	type element
//		isMM				if TRUE, will initialize "mmid" field of MM type elements to spaces;
//		mmID				if not NULL, specifies NULL terminated market maker ID, and will be moved to 
//								"mmid" fieled of "MM" type elements
//

DLLEXPORT BOOL StringToSticW(TCHAR *string, STIC *pStic, BOOL isKey, BOOL hasCurrency, BOOL isMM, TCHAR * mmID );
DLLEXPORT BOOL StringToSticA(CHAR *string, STIC *pStic, BOOL isKey, BOOL hasCurrency, BOOL isMM, CHAR * mmID );

//	SticToString			primary function ( all overloads call this ultimately )
//
//	arguments:
//		pStic				input STIC element ( per overload )
//		string				NULL terminated output string
//		isMM				if TRUE, mmid of "MM" type element will be appended to output string ( preceeded by space ); 
//
DLLEXPORT CHAR *SticToStringA( STIC *pStic, CHAR *string, INT stringLen, BOOL isMM ); 
DLLEXPORT TCHAR *SticToStringW( STIC *pStic, TCHAR *string, INT stringLen, BOOL isMM ); 
#endif

#endif

