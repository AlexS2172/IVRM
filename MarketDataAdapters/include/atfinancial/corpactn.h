/*
 *		corpactn.h		Date=990217
 *
 *		Corporate Action Records.
 *
 *		Written 09/25/97	R. Holm
 *
 *		Modified 09/29/97	R. Holm
 *			added security type.
 *
 *		Modified 12/17/97	R. Holm
 *			added CARSPTYPE_DISTRIBUTION and CARSPTYPE_OTHER.
 *			fixed the packing!
 *
 *		Modified 02/17/99	R. Holm
 *			added structures for the keys.
 *
 *
 *		Copyright (c) A-T Financial Information, Inc., 1995-1999.
 *		All rignts reserved.
 *
 */

/*=============================================================*/

#ifndef __CORPORATE_ACTION_DEFINES
#define __CORPORATE_ACTION_DEFINES

/*=============================================================*/

#ifndef RC_INVOKED
#pragma pack(1)						// byte packing
#endif

/*=============================================================*/

// corporate action symbol

#define SECTYPE_UNDEFINED			0xff
#define SECTYPE_EQUITY				0
#define SECTYPE_MONEY_MARKET		1
#define SECTYPE_MUTUAL_FUND			2
#define SECTYPE_CORP_BOND			3
#define SECTYPE_GOV_BOND			4

typedef struct _CAR_SECURITY {
	BYTE  type;
    char  symbol[16];
    char  country[2];
    char  exchange;
} CAR_SECURITY, *LPCAR_SECURITY, *PCAR_SECURITY;

// corporate action record header

typedef struct _CAR_HEADER {
    CAR_SECURITY  security;
    char          date[8];
    char          name[32];
    char          cusip[12];
} CAR_HEADER, *LPCAR_HEADER, *PCAR_HEADER;

// dividend flags

#define CARDVTYPE_CASH				0
#define CARDVTYPE_STOCK				1
#define CARDVTYPE_BONUS				2

#define CARDVFLG_AMOUNT				0x0001
#define CARDVFLG_RATIO				0x0002
#define CARDVFLG_FACTOR				0x0004

// dividend record

typedef struct _CAR_DIVIDEND {
    CAR_HEADER  header;
    WORD        flags;
    BYTE        fractionType;
    BYTE        type;
    DWORD       amount;
    DWORD       percent;
    DWORD       factor;
    char        notes[80];
    BYTE        resv[8];
} CAR_DIVIDEND, *LPCAR_DIVIDEND, *PCAR_DIVIDEND;

// split flags

#define CARSPTYPE_SPLIT				0
#define CARSPTYPE_REVERSE_SPLIT		1
#define CARSPTYPE_VALUE_REDUCTION	2
#define CARSPTYPE_DISTRIBUTION		3
#define CARSPTYPE_UNKNOWN			4

#define CARSPFLG_FACTOR				0x0001
#define CARSPFLG_RATIO				0x0002
#define CARSPFLG_ADJUSTMENT			0x0004

// split record

typedef struct _CAR_SPLIT {
    CAR_HEADER  header;
    WORD        flags;
    BYTE        fractionType;
    BYTE        type;
    DWORD       factor;
    DWORD       ratio;
    DWORD       adjustment;
    char        notes[80];
    BYTE        resv[8];
} CAR_SPLIT, *LPCAR_SPLIT, *PCAR_SPLIT;

// status flags

#define CARSTTYPE_IPO				0
#define CARSTTYPE_DELIST			1
#define CARSTTYPE_SPINOFF			2
#define CARSTTYPE_RIGHTS			3
#define CARSTTYPE_WARRENTS			4
#define CARSTTYPE_BANKRUPTCY		5
#define CARSTTYPE_MERGER			6
#define CARSTTYPE_EXPIRE			7

// status record

typedef struct _CAR_STATUS {
    CAR_HEADER  header;
    WORD        flags;
    BYTE        type;
    BYTE        resv1;
    char        notes[80];
    BYTE        resv2[8];
} CAR_STATUS, *LPCAR_STATUS, *PCAR_STATUS;

// change flags

#define CARCHFLG_NEW_SYMBOL			0x0001
#define CARCHFLG_NEW_NAME			0x0002
#define CARCHFLG_NEW_CUSIP			0x0004
#define CARCHFLG_NEW_EXCHANGE		0x0008
#define CARCHFLG_NEW_SEDOL			0x0010
#define CARCHFLG_NEW_ISIN			0x0020
#define CARCHFLG_NEW_NSIN			0x0040

// change record

typedef struct _CAR_CHANGE {
    CAR_HEADER    header;
    WORD          flags;
    BYTE          resv1[2];
    CAR_SECURITY  prevSymbol;
    char          prevName[32];
    char          prevCusip[12];
    char          prevExchange[4];
    char          notes[80];
    BYTE          resv2[8];
} CAR_CHANGE, *LPCAR_CHANGE, *PCAR_CHANGE;

typedef union _CAR_RECORD {
    CAR_HEADER    header;
    CAR_DIVIDEND  dividend;
    CAR_SPLIT     split;
    CAR_STATUS    status;
    CAR_CHANGE    change;
} CAR_RECORD, *LPCAR_RECORD, *PCAR_RECORD;

// car keys :)

typedef struct _CAR_KEY0 {
    CAR_SECURITY  security;
	DWORD         resv;
} CAR_KEY0, *LPCAR_KEY0, *PCAR_KEY0;

typedef struct _CAR_KEY1 {
    char   date[8];	
	DWORD  resv;
} CAR_KEY1, *LPCAR_KEY1, *PCAR_KEY1;

typedef struct _CAR_KEY2 {
    CAR_SECURITY  security;
    char          date[8];	
} CAR_KEY2, *LPCAR_KEY2, *PCAR_KEY2;

/*=============================================================*/

#ifndef RC_INVOKED
#pragma pack()
#endif

/*=============================================================*/

#endif

/*=============================================================*/

