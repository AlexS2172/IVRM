/*
 *		atfidefs.cpp		Date=990920
 *
 *		ATFI Server structures and defines.
 *
 *		Written 06/16/98	R. Holm
 *
 *		Modified 06/23/98	R. Holm
 *			added data source includes.
 *
 *		Modified 07/02/98	R. Holm
 *			ATDT_LOGON changed back to 243.
 *			removed type from AT_KEY.
 *
 *		Modified 07/07/98	R. Holm
 *			fixed AT_DATETIME::GetValue().
 *
 *		Modified 07/23/98	R. Holm
 *			fixed GetDifference() to return DIFF_ZERO.
 *			fixed year in SetDate().
 *			added GetDate() and GetTime().
 *
 *		Modified 08/22/98	R. Holm
 *			added AT_PRICE::Get().
 *
 *		Modified 08/24/98	R. Holm
 *			ATDT_INDEX moved to 69.
 *
 *		Modified 08/26/98	R. Holm
 *			fixed year problem in AT_DATETIME.
 *			added ATDT_EXCHANGE.
 *
 *		Modified 08/27/98	R. Holm
 *			added ATDT_GENERIC.
 *
 *		Modified 08/31/98	R. Holm
 *			if a price is invalid a conversion is not performed.
 *
 *		Modified 09/09/98	R. Holm
 *			added defines for 4 option databases.
 *
 *		Modified 09/15/98	R. Holm
 *			added ATDT_NEWSFLAG.
 *
 *		Modified 09/16/98	R. Holm
 *			moved ATDT_NEWSFLAG to 80.
 *
 *		Modified 10/19/98	R. Holm
 *			added ATDT_SPREAD to 81.
 *			added AT_SPREAD_KEY.
 *
 *		Modified 10/20/98	R. Holm
 *			added spread type to spread key.
 *
 *		Modified 11/12/98	R. Holm
 *			fixed PRICE:Copy().
 *			added '!=' to PRICE.
 *
 *		Modified 12/03/98	R. Holm
 *			added AT_DATETIME::GetTsValue().
 *
 *		Modified 12/31/98	R. Holm and T. Tian
 *			added +,-,*,/ operators to AT_PRICE.
 *
 *		Modified 03/13/99	R. Holm
 *			fix of operator presidence in AT_PRICE::Convert().
 *
 *		Modified 03/22/99	R. Holm
 *			added ATDT_MISC.
 *
 *		Modified 03/25/99	R. Holm
 *			added ATDT_SPECIAL.
 *
 *		Modified 03/29/99	R. Holm
 *			added delayed data type range 150-180.
 *
 *		Modified 04/19/99	R. Holm
 *			added easy out for AT_PRICE::Convert().
 *			added a few sanity checks to AT_PRICE::Convert().
 *
 *		Modified 05/03/99	R. Holm
 *			added Linux support.
 *
 *		Modified 06/07/99	R. Holm
 *			AT_PRICE::Convert() now uses 10^-3 not 10^-5.
 *
 *		Modified 06/08/99	R. Holm
 *			fixed bug AT_PRICE::Convert()'s use of 10^-5.
 *
 *		Modified 08/04/99	S. Aharrah
 *			added ATPT_STOCKS display price type for reduced fraction.
 *
 *		Modified 09/01/99	R. Holm
 *			added ATDT_ADVISE_MDS.
 *
 *		Modified 09/20/99	R. Holm
 *			handled C/C++ issues.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-1999.
 *		All rignts reserved.
 *
 */

/*=============================================================*/

#ifndef __ATFI_DEFINES
#define __ATFI_DEFINES

/*=============================================================*/

#include <atfisrc.h>

/*=============================================================*/

#pragma pack(1)

/*=============================================================*/

// data types

#define ATDT_MISC			0		// miscellaneous data (data transfers/database updates)

#define	ATDT_US_BONDS		10		// US Markets
#define	ATDT_US_FUTOPTS		11
#define	ATDT_US_FUTINDX		12
#define	ATDT_US_FUTURE		12
#define	ATDT_US_STOCKS		13

#define	ATDT_FC_EARNINGS	14		// First Call earnings

#define	ATDT_EU_STOCKS		15		// European Markets
#define	ATDT_EU_STKOPTS		16
#define	ATDT_EU_BONDS		17
#define	ATDT_EU_FUTOPTS		18
#define	ATDT_EU_FUTINDX		19
#define	ATDT_EU_FUTURE		19
		   
#define	ATDT_AM_BONDS		20		// North/Central/South America
#define	ATDT_AM_FUTOPTS		21		//     Non-US Markets
#define	ATDT_AM_FUTINDX		22
#define	ATDT_AM_FUTURE		22
#define	ATDT_AM_STOCKS		23
#define	ATDT_AM_STKOPTS		24
		   
#define	ATDT_AS_BONDS		25		// Asian Markets
#define	ATDT_AS_FUTOPTS		26
#define	ATDT_AS_FUTINDX		27
#define	ATDT_AS_FUTURE		27
#define	ATDT_AS_STOCKS		28
#define	ATDT_AS_STKOPTS		29

#define	ATDT_FOREX			32
#define	ATDT_FIXED_INCOME	33
#define ATDT_SP_LEVEL2_A	34
#define ATDT_SP_LEVEL2_N	35
#define ATDT_MONEY_RATE		36
#define ATDT_MARKET_MAKER	37

#define ATDT_MONEY_MARKET	38
#define ATDT_MUTUAL_FUND	39

#define	ATDT_US_STKOPT_AG	65		// Stock Option Split - 4.60
#define	ATDT_US_STKOPT_HQ	66		// DT_STKOPT_A
#define	ATDT_US_STKOPT_RZ	67		// DT_STKOPT_N
#define ATDT_US_STKOPTS		68

#define ATDT_INDEX			69

#define ATDT_US_STKOPT_AE	75
#define ATDT_US_STKOPT_FM	76
#define ATDT_US_STKOPT_NR	77
#define ATDT_US_STKOPT_SZ	78

#define ATDT_NEWSFLAG		80		// news flag database
#define ATDT_SPREAD			81		// spread database

// delayed data types 150-180

#define	ATDT_US_BONDS_DLY	150		// US Markets
#define	ATDT_US_FUTOPTS_DLY	151
#define	ATDT_US_FUTURE_DLY	152
#define	ATDT_US_STOCKS_DLY	153
#define	ATDT_US_STKOPTS_DLY	154

#define	ATDT_EU_BONDS_DLY	155		// European Markets
#define	ATDT_EU_FUTOPTS_DLY	156
#define	ATDT_EU_FUTURE_DLY	157
#define	ATDT_EU_STOCKS_DLY	158
#define	ATDT_EU_STKOPTS_DLY	159
		   
#define	ATDT_AM_BONDS_DLY	160		// North/Central/South America
#define	ATDT_AM_FUTOPTS_DLY	161		//     Non-US Markets
#define	ATDT_AM_FUTURE_DLY	162
#define	ATDT_AM_STOCKS_DLY	163
#define	ATDT_AM_STKOPTS_DLY	164
		   
#define	ATDT_AS_BONDS_DLY	165		// Asian Markets
#define	ATDT_AS_FUTOPTS_DLY	166
#define	ATDT_AS_FUTURE_DLY	167
#define	ATDT_AS_STOCKS_DLY	168
#define	ATDT_AS_STKOPTS_DLY	169

#define ATDT_FUTURE_INFO	240		// the next 6 data types live in NT servers only,
#define ATDT_TOPTEN			241		// and are not visible to workstations... at least
#define ATDT_ADVISE			242		// not yet.
#define ATDT_LOGON			243
#define ATDT_ADVISE_QSDTID	244
#define ATDT_STATISTICS		245

#define	ATDT_ADVISE_SYMS	246		// the next 4 data types live in NT Advanced 
#define	ATDT_ADVISE_CATS	247		// NEWS Servers only
#define	ATDT_ADVISE_KEYWD	248
#define	ATDT_ADVISE_GCATS	249	

#define ATDT_EXCHANGE		250		// exchange database
#define ATDT_GENERIC		251		// generic database settings holder

#define ATDT_ADVISE_MDS		252		// MDS advise database

#define ATDT_SPECIAL		255		// special advise types (halts, alerts, and big blocks).

/*=============================================================*/

// quote server types

#define ATQST_TELEKURS			0			// 'T'
#define ATQST_COMSTOCK			1			// 'S'
#define ATQST_KNIGHTRIDDER		2			// 'K'
#define ATQST_REUTERS_SF		3			// 'R'
#define ATQST_NASDAQLEVEL2		4			// 'N'
#define ATQST_REUTERS_MF		5			// 'M'
#define ATQST_TELERATE			6			// 'F'
#define ATQST_PCQUOTE			7			// 'P'
#define ATQST_GTISMARKETDATA	8			// 'C'
#define ATQST_JJKENNY			9			// 'J'
#define	ATQST_GOVPX				10			// 'G'
#define ATQST_NTNEWSSERVER		11			// 'E'
#define ATQST_BRIDGE			12			// 'I'
#define ATQST_BLOOMBERG			13			// 'B'
#define ATQST_DISCLOSURE		14			// 'D'
#define ATQST_STREET_SOFTWARE	15			// 'O'
#define ATQST_VICKERS			16			// 'V'
#define ATQST_ORDER_EXECUTION	17			// 'X'
#define	ATQST_ZACKS				18          // 'Z'
#define ATQST_MNSTAR			19			// '*'
#define ATQST_FIRSTCALL			20			// 'P'
#define ATQST_RESEARCH_NEWS		21			// 'A'
#define ATQST_PCQUOTE2			22			// '2'
#define ATQST_ATFI				23			// 'H'

/*=============================================================*/

// basic database key

typedef struct _AT_KEY {
	BYTE  symbol[17];				// symbol
	BYTE  country[2];				// country code
	BYTE  exchange;					// exchange code
} AT_KEY, *LPAT_KEY;

// spread database key

typedef struct _AT_SPREAD_KEY {
	char  spreadType[2];
	BYTE  dataType1;
	char  symbol1[17];
	BYTE  dataType2;
	char  symbol2[17];
	BYTE  dataType3;
	char  symbol3[17];
	char  country[2];
	char  exchange;
	BYTE  resv;
} AT_SPREAD_KEY, *LPAT_SPREAD_KEY;

/*=============================================================*/

// prices

#define ATPT_UNDEFINED		    0x1f	// undefined
#define ATPT_DETERMINE		    0x1e	// to be determined...
							    
#define ATPT_WHOLE			    0		// whole number price
#define ATPT_DECIMAL_1		    1		// decimal price types
#define ATPT_DECIMAL_2		    2		
#define ATPT_DECIMAL_3		    3		
#define ATPT_DECIMAL_4		    4		
#define ATPT_DECIMAL_5		    5		
#define ATPT_DECIMAL_6		    6		
#define ATPT_DECIMAL_7		    7		
							    
#define ATPT_2				    8		// price type halves
#define ATPT_4				    9		// price type 4ths
#define ATPT_8				    10		// price type 8ths
#define ATPT_16				    11		// price type 16ths
#define ATPT_32				    12		// price type 32nds
#define ATPT_64				    13		// price type 64ths
#define ATPT_128			    14		// price type 128ths
#define ATPT_256			    15		// price type 256ths

#define ATPT_STOCKS				20		// Stocks reduce fraction.
#define ATPT_QUARTER_32		    21		// CBOT crap for bond futures
#define ATPT_HALF_32			22		// CBOT crap for bond futures
#define ATPT_HALF_64			23		// CBOT crap for bond futures
#define ATPT_FUTURE_64		    24		// price type 64ths (futures)
#define ATPT_BOND_64			24		// price type 64ths (bonds)
#define ATPT_FUTURE_32		    25		// price type 32nds (futures)
#define ATPT_BOND_32			25		// price type 32nds (bonds)
#define ATPT_2_HALF			    26		// price type 2 1/2ths (futures)

#define DIFF_ZERO				0x8000

typedef struct _AT_PRICE {
	LONG   price : 31;				// price value
	DWORD  bIsValid: 1;

#ifdef __cplusplus

	BOOL IsValid( void )
	{
		return bIsValid;
	}

	BOOL operator==( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			return price == pr.price;
		else
			return FALSE;
	}
	
	BOOL operator!=( const _AT_PRICE &pr )
	{
		return !( price == pr.price );
	}
	
	BOOL operator>( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			return price > pr.price;
		else if ( !bIsValid )
			return FALSE;
		else if ( !pr.bIsValid )
			return TRUE;
		else
			return FALSE;
	}
	
	BOOL operator>=( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			return price >= pr.price;
		else if ( !bIsValid )
			return FALSE;
		else if ( !pr.bIsValid )
			return TRUE;
		else
			return FALSE;
	}
	
	BOOL operator<( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			return price < pr.price;
		else if ( !bIsValid )
			return FALSE;
		else if ( !pr.bIsValid )
			return TRUE;
		else
			return FALSE;
	}

	BOOL operator<=( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			return price <= pr.price;
		else if ( !bIsValid )
			return FALSE;
		else if ( !pr.bIsValid )
			return TRUE;
		else
			return FALSE;
	}
	
	_AT_PRICE operator*( const int factor )
	{
		_AT_PRICE  temp;

		if ( bIsValid )
			temp.Set( price*factor );
		else if ( !bIsValid )
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator*=( const int factor )
	{
		if ( bIsValid )
			Set( price*factor );
		else if ( !bIsValid )
			Clear();

		return *this;
	}
	
	_AT_PRICE operator/( const int factor )
	{
		_AT_PRICE  temp;

		if ( bIsValid || factor == 0 )
			temp.Set( price/factor );
		else if ( !bIsValid )
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator/=( const int factor )
	{
		if ( bIsValid || factor == 0 )
			Set( price/factor );
		else if ( !bIsValid )
			Clear();

		return *this;
	}
	
	_AT_PRICE operator*( const double factor )
	{
		_AT_PRICE  temp;

		if ( bIsValid )
			temp.Set((LONG)( factor*price ));
		else if ( !bIsValid )
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator*=( const double factor )
	{
		if ( bIsValid )
			Set((LONG)( factor*price ));
		else if ( !bIsValid )
			Clear();

		return *this;
	}
	
	_AT_PRICE operator/( const double factor )
	{
		_AT_PRICE  temp;

		if ( bIsValid || factor == 0 )
			temp.Set((LONG)((double)price/factor ));
		else if ( !bIsValid )
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator/=( const double factor )
	{
		if ( bIsValid || factor == 0 )
			Set((LONG)((double) price/factor ));
		else if ( !bIsValid )
			Clear();

		return *this;
	}
	
	_AT_PRICE operator+( const _AT_PRICE &pr )
	{
		_AT_PRICE  temp;

		if ( bIsValid && pr.bIsValid )
			temp.Set( price+pr.price );
		else if ( !bIsValid )
			temp.Set( pr.price );
		else if ( !pr.bIsValid )
			temp.Set( price );
		else
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator+=( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			Set( price+pr.price );
		else if ( !bIsValid )
			Set( pr.price );
		else if ( !pr.bIsValid )
			Set( price );
		else
			Clear();

		return *this;
	}
	
	_AT_PRICE operator-( const _AT_PRICE &pr )
	{
		_AT_PRICE  temp;

		if ( bIsValid && pr.bIsValid )
			temp.Set( price-pr.price );
		else if ( !pr.bIsValid )
			temp.Set( -pr.price );
		else if ( !pr.bIsValid )
			temp.Set( pr.price );
		else
			temp.Clear();

		return temp;
	}
	
	_AT_PRICE operator-=( const _AT_PRICE &pr )
	{
		if ( bIsValid && pr.bIsValid )
			Set( price-pr.price );
		else if ( !pr.bIsValid )
			Set( -pr.price );
		else if ( !pr.bIsValid )
			Set( pr.price );
		else
			Clear();

		return *this;
	}
	
	_AT_PRICE operator-()
	{
		_AT_PRICE  temp;

		if ( bIsValid )
			temp.Set( -price );
		else
			temp.Clear();

		return temp;
	}
	
	BOOL Copy( const _AT_PRICE &pr, const UINT recordPriceType, const UINT newPriceType )
	{
		if ( !pr.bIsValid )
			return FALSE;
		else if ( newPriceType == ATPT_DETERMINE ) {
			switch ( recordPriceType ) {
				case ATPT_UNDEFINED:
					price = pr.price;
					break;

				case ATPT_2:
					price = (( pr.price/10 ) << 1 )+( pr.price%10 );
					break;

				case ATPT_4:
					price = (( pr.price/10 ) << 2 )+( pr.price%10 );
					break;

				case ATPT_8:
					price = (( pr.price/10 ) << 3 )+( pr.price%10 );
					break;

				case ATPT_16:
					price = (( pr.price/100 ) << 4 )+( pr.price%100 );
					break;

				case ATPT_32:
					price = (( pr.price/100 ) << 5 )+( pr.price%100 );
					break;

				case ATPT_64:
					price = (( pr.price/100 ) << 6 )+( pr.price%100 );
					break;

				case ATPT_128:
					price = (( pr.price/1000 ) << 7 )+( pr.price%1000 );
					break;

				case ATPT_256:
					price = (( pr.price/1000 ) << 8 )+( pr.price%1000 );
					break;

				default:
					price = pr.price;
					break;
			}
		} else if ( newPriceType == recordPriceType ) {
			price = pr.price;
		} else if ( newPriceType == ATPT_WHOLE && recordPriceType != ATPT_UNDEFINED ) {
			_AT_PRICE  temp;

			temp.price = pr.price;
			temp.Convert( recordPriceType, newPriceType );
			price = temp.price;
		} else if ( newPriceType != ATPT_UNDEFINED ) {
			price = pr.price;
		} else
		    return FALSE;

		bIsValid = TRUE;
		return TRUE;
	}

	UINT ChoosePriceType( const UINT priceType1, const UINT priceType2 )
	{
		static UINT  div[] = { 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 
							   2, 4, 8, 16, 32, 64, 128, 256, 
							   0, 0, 0, 0, 0, 128, 64, 128, 
							   64, 32, 2, 0, 0, 0, 0, 0
		};

		if ( priceType1 == priceType2 )
			return priceType1;
		else if ( priceType1 == ATPT_UNDEFINED && priceType2 == ATPT_UNDEFINED )
			return priceType1;
		else if ( priceType1 == ATPT_DETERMINE && priceType2 == ATPT_DETERMINE )
			return priceType1;
		else if ( priceType1 == ATPT_DETERMINE )
			return priceType2;
		else if ( priceType2 == ATPT_DETERMINE )
			return priceType1;
		else
			return ( div[priceType1] > div[priceType2] ) ? priceType1 : priceType2;
	}

	WORD Difference( const _AT_PRICE &pr, const UINT priceType )
	{
		if ( priceType == ATPT_UNDEFINED || !((_AT_PRICE)pr).IsValid())
		    return (WORD)DIFF_ZERO;
		else
		    return (WORD)((int)price-(int)pr.price );
	}

	void Clear( void )
	{
		price = 0;
		bIsValid = FALSE;
	}

	void Set( LONG value )
	{
		price = value;
		bIsValid = TRUE;
	}

	LONG Get( void )
	{
		if ( IsValid())
			return price;
		else
			return (LONG)0x80000000;
	}

	void Convert( UINT priceType, UINT newPriceType )
	{
		static UINT  shift[] = { 
			0, 0, 0, 0, 0, 0, 0, 0,
			1, 2, 3, 4, 5, 6, 7, 8, 
			0, 0, 0, 0, 0, 7, 6, 7, 
			6, 5, 1, 0, 0, 0, 0, 0
		};
		static UINT  div[] = { 
			1, 10, 100, 1000, 10000, 100000, 1000000, 10000000,
			2, 4, 8, 16, 32, 64, 128, 256, 
			0, 0, 0, 0, 0, 128, 64, 128, 
			64, 32, 2, 0, 0, 0, 0, 0
		};

		// sanity checks
		if ( !IsValid())
			return;
		else if ( priceType > sizeof( shift )/sizeof( shift[0] ))
			return;
		else if ( newPriceType > sizeof( shift )/sizeof( shift[0] ))
			return;

		if ( priceType == newPriceType )					// do we need to do anything?
			return;
		else if ( div[priceType] == div[newPriceType] ) {	// same encoding?
			priceType = newPriceType;
			return;
		} else if ( priceType == ATPT_UNDEFINED || priceType == ATPT_DETERMINE )
			return;
		else if (( priceType >= ATPT_WHOLE && priceType <= ATPT_DECIMAL_7 ) &&
				 ( newPriceType >= ATPT_WHOLE && newPriceType <= ATPT_DECIMAL_7 )) {
			LONG  value = price & 0x07ffffff;

			if ( newPriceType > priceType ) {
				while ( priceType < newPriceType ) {
					value *= 10;
					priceType++;
				}
			} else {
				while ( priceType > newPriceType ) {
					value /= 10;
					priceType--;
				}
			}

			price = value;
		} else if ( priceType >= ATPT_WHOLE && priceType <= ATPT_DECIMAL_7 ) {
			UINT  mod, numerator, whole;

			if ( div[newPriceType] == 0 )
				return;

			if ( div[priceType] == 0 )
				return;

			mod = div[priceType];

			whole = price/mod;
			numerator = ( price%mod )*div[newPriceType]/div[priceType];
			price = ( whole << shift[newPriceType] )+numerator;
			priceType = newPriceType;
		} else if ( newPriceType >= ATPT_WHOLE && newPriceType <= ATPT_DECIMAL_7 ) {
			UINT  whole, fraction;

			if ( div[priceType] == 0 )
				return;

			whole = price >> shift[priceType];

			if ( newPriceType != ATPT_WHOLE ) {
				fraction = (( price & ( 0xff >> ( 8-shift[priceType] )))*100000 )/div[priceType];
				price = ( whole*100000 )+fraction;

				if ( newPriceType != ATPT_DECIMAL_5 ) {
					UINT  i;

					if ( newPriceType < ATPT_DECIMAL_5 ) {
						for ( i = newPriceType; i < ATPT_DECIMAL_5; i++ )
							price /= 10;
					} else if ( newPriceType > ATPT_DECIMAL_5 ) {
						for ( i = ATPT_DECIMAL_5; i < newPriceType; i++ )
							price *= 10;
					}
				}
			} else
				price = whole;

			priceType = newPriceType;
		} else if ( shift[priceType] > shift[newPriceType] ) {
			price >>= shift[priceType]-shift[newPriceType];
			priceType = newPriceType;			
		} else if ( shift[newPriceType] > shift[priceType] ) {
			price <<= shift[newPriceType]-shift[priceType];
			priceType = newPriceType;
		}
	}

#endif
} AT_PRICE, *LPAT_PRICE;

/*=============================================================*/

#define HOUR_INVALID		31

typedef union _AT_DATETIME {
	struct {
		DWORD  bValid: 1;
		DWORD  bTime: 1;
		DWORD  resv: 30;
	} dateTime;
	struct {
		DWORD  resv: 15;
		DWORD  hour: 5;
		DWORD  minute: 6;
		DWORD  second: 6;
	} time;
	struct {
		DWORD  resv: 16;
		DWORD  year: 7;
		DWORD  month: 4;
		DWORD  day: 5;
	} date;

#ifdef __cplusplus

	void Clear( void )
	{
		dateTime.bValid = FALSE;
		dateTime.resv = 0;
	}

	void SetTime( UINT hour, UINT minute, UINT second )
	{
		dateTime.bValid = TRUE;
		dateTime.bTime = TRUE;
		time.hour = hour;
		time.minute = minute;
		time.second = second;
	}

	BOOL GetTime( UINT *pHour, UINT *pMinute, UINT *pSecond )
	{
		if ( IsTime()) {
			*pHour = time.hour;
			*pMinute = time.minute;
			*pSecond = time.second;
			return TRUE;
		} else
			return FALSE;
	}

	void SetDate( UINT year, UINT month, UINT day )
	{
		dateTime.bValid = TRUE;
		dateTime.bTime = FALSE;
		if ( year > 127 )
			date.year = year-1970;
		else
			date.year = year-70;
		date.month = month;
		date.day = day;
	}

	BOOL GetDate( UINT *pYear, UINT *pMonth, UINT *pDay )
	{
		if ( IsDate()) {
			*pYear = date.year+1970;
			*pMonth = date.month;
			*pDay = date.day;
			return TRUE;
		} else
			return FALSE;
	}

	BOOL IsValid( void )
	{
		return dateTime.bValid;
	}

	BOOL IsDate( void )
	{
		if ( dateTime.bValid )
			return !dateTime.bTime;
		else
			return FALSE;
	}

	BOOL IsTime( void )
	{
		if ( dateTime.bValid )
			return dateTime.bTime;
		else
			return FALSE;
	}

	BOOL operator==( const _AT_DATETIME &tm )
	{
		if ( dateTime.bValid && tm.dateTime.bValid ) {
			if ( dateTime.bTime != tm.dateTime.bTime )
				return FALSE;
			else if ( dateTime.bTime )
				return time.hour == tm.time.hour && time.minute == tm.time.minute && 
						time.second == tm.time.second;
			else if ( !dateTime.bTime )
				return date.year == tm.date.year && date.month == tm.date.month && 
						date.day == tm.date.day;
			else
				return FALSE;
		} else
			return FALSE;
	}

	BOOL operator!=( const _AT_DATETIME &tm )
	{
		return !( *this == tm );
	}
	
	BOOL operator>( const _AT_DATETIME &tm )
	{
		if ( dateTime.bValid && tm.dateTime.bValid ) {
			if ( dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return TRUE;
			else if ( !dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return FALSE;
			else if ( dateTime.bTime ) {
				if ( time.hour == tm.time.hour ) {
					if ( time.minute == tm.time.minute )
						return time.second > tm.time.second;
					else
						return time.minute > tm.time.minute;
				} else
					return time.hour > tm.time.hour;
			} else if ( !dateTime.bTime ) {
				if ( date.year == tm.date.year ) {
					if ( date.month == tm.date.month )
						return date.day > tm.date.day;
					else
						return date.month > tm.date.month;
				} else
					return date.year > tm.date.year;
			} else
				return TRUE;
		} else if ( !dateTime.bValid )
			return FALSE;
		else
			return TRUE;
	}
	
	BOOL operator>=( const _AT_DATETIME &tm )
	{
		if ( dateTime.bValid && tm.dateTime.bValid ) {
			if ( dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return TRUE;
			else if ( !dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return FALSE;
			else if ( dateTime.bTime ) {
				if ( time.hour == tm.time.hour ) {
					if ( time.minute == tm.time.minute )
						return time.second >= tm.time.second;
					else
						return time.minute >= tm.time.minute;
				} else
					return time.hour >= tm.time.hour;
			} else if ( !dateTime.bTime ) {
				if ( date.year == tm.date.year ) {
					if ( date.month == tm.date.month )
						return date.day >= tm.date.day;
					else
						return date.month >= tm.date.month;
				} else
					return date.year >= tm.date.year;
			} else
				return TRUE;
		} else if ( !dateTime.bValid )
			return FALSE;
		else
			return TRUE;
	}
	
	BOOL operator<( const _AT_DATETIME &tm )
	{
		if ( dateTime.bValid && tm.dateTime.bValid ) {
			if ( dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return TRUE;
			else if ( !dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return FALSE;
			else if ( dateTime.bTime ) {
				if ( time.hour == tm.time.hour ) {
					if ( time.minute == tm.time.minute )
						return time.second < tm.time.second;
					else
						return time.minute < tm.time.minute;
				} else
					return time.hour < tm.time.hour;
			} else if ( !dateTime.bTime ) {
				if ( date.year == tm.date.year ) {
					if ( date.month == tm.date.month )
						return date.day < tm.date.day;
					else
						return date.month < tm.date.month;
				} else
					return date.year < tm.date.year;
			} else
				return TRUE;
		} else if ( !dateTime.bValid )
			return FALSE;
		else
			return TRUE;
	}

	BOOL operator<=( const _AT_DATETIME &tm )
	{
		if ( dateTime.bValid && tm.dateTime.bValid ) {
			if ( dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return TRUE;
			else if ( !dateTime.bTime && dateTime.bTime != tm.dateTime.bTime )
				return FALSE;
			else if ( dateTime.bTime ) {
				if ( time.hour == tm.time.hour ) {
					if ( time.minute == tm.time.minute )
						return time.second <= tm.time.second;
					else
						return time.minute <= tm.time.minute;
				} else
					return time.hour <= tm.time.hour;
			} else if ( !dateTime.bTime ) {
				if ( date.year == tm.date.year ) {
					if ( date.month == tm.date.month )
						return date.day <= tm.date.day;
					else
						return date.month <= tm.date.month;
				} else
					return date.year <= tm.date.year;
			} else
				return TRUE;
		} else if ( !dateTime.bValid )
			return FALSE;
		else
			return TRUE;
	}

	UINT GetValue( void )
	{
		if ( !dateTime.bValid )
			return 0;
		else {
			if ( dateTime.bTime )
				return ( time.hour*100 )+time.minute;
			else
				return 0x8000 | (( date.month*100 )+date.day );
		}
	}

	UINT GetTsValue( void )
	{
		if ( !dateTime.bValid )
			return 0;
		else {
			if ( dateTime.bTime )
				return 0;
			else
				return ( date.year << 9 ) | ( date.month << 5 ) | ( date.day );
		}
	}

#endif
} AT_DATETIME, *LPAT_DATETIME;

/*=============================================================*/

#pragma pack()

/*=============================================================*/

#endif

/*=============================================================*/
