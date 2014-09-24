/*
 *		tosfread.h		Date=970131
 *
 *		A-T TOS Filtered Read defines and structures.
 *
 */

/*===========================================================================*/

#ifndef __TOSFREAD_DEFINES
#define __TOSFREAD_DEFINES

#ifndef RC_INVOKED
#pragma pack(1)
#endif /* RC_INVOKED */

#define	TOSBUFFER_SIZE	(64 * sizeof(TOS))
#define	TPBBUFFER_SIZE	(51 * sizeof(ETPB))

typedef union
		{
		WORD	value;
		TOSTIME	tosTime;
		}
		PK_TIME;

// Expanded Time Period Bar

typedef	struct
		{
		PK_TIME	pkTime;
		BYTE	dec;
		BYTE	extra;
		DWORD	dwLow;
		DWORD	dwHigh;
		DWORD	dwOpen;
		DWORD	dwClose;
		DWORD	dwVolume;
		}
		ETPB,*LPETPB;

typedef	struct
		{
		WORD	wFilterFlags;		// TFR_*
		BYTE	day;				// 0=today, DOS:1=Monday
		BYTE	exch;

		DWORD	dwSize;

		WORD	wTimeStart;
		WORD	wTimeEnd;

		BYTE	period;

	/*----- User sets vars above this line only ----*/

		BYTE	nHiDec;					// Highest decimal type
		WORD	wLastError;

		WORD	wTradeEnd;				// Latest   time in file
		WORD	wTradeStart;			// Earliest time in file

		char	szFN[16];

		DWORD	dwCurrOffset;
		DWORD	dwFileSize;
		DWORD	dwTotalRecords;
		DWORD	dwRecsReadSF;			// Number records read so far
		}
		TFRBLOCK;
typedef	TFRBLOCK FAR *LPTFRBLOCK;

#define	TFR_SIZE		0x0001
#define TFR_EXCHANGE	0x0002
#define TFR_TIME		0x0004
#define TFR_X_TRADES	0x0008
#define TFR_X_BIDS		0x0010
#define TFR_X_ASKS		0x0020
#define TFR_X_SPECIAL	0x0040

#define TFR_TPB			0x4000			// Reserved for API use only
#define TFR_TEST		0x8000			// Reserved for API use only

/*=========================================================================*/

#pragma pack()

/*=========================================================================*/

#endif

/*=========================================================================*/

