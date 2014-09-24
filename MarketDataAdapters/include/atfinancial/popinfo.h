#ifndef	_POPINFO_H_
#define	_POPINFO_H_

typedef struct _POPINFO
{
	DWORD	lStructSize;	// Size of this struct (used in future versions)
	HWND	hParentWnd;
	LPSTR	lpCaption;		// Caption or NULL if no caption desired
	DWORD	qsn;			// QSN to use to get info
	LPPB	lpPB;
	POINT	ptLocation;		// relative to upper left of hParentWnd
	POINT	ptSize;			// Set if user wants to specify size
	DWORD	dwInfoType;		// Type of info to display *see below
	DWORD	dwFlags;		// Other stuff	*see below
	HWND	hPopInfoWnd;	// Returned to caller
} POPINFO;

typedef POPINFO FAR* LPPOPINFO;
		
BOOL WINAPI	PopInfo(HAT *hAt,LPPOPINFO lpPopInfo);

	// Values used for restriction types.

#define	RESTRICT_TYPE_NONE		0		// No restriction type
#define	RESTRICT_TYPE_LEHMAN	1		// Lehman restriction type
#define	RESTRICT_TYPE_RH_BTV	2		// R.H. BTV (not used)
#define	RESTRICT_TYPE_RH		3		// R.H. restriction type

	// Values used for wInfoType

#define	PIT_AT_TEST				0		// Used for testing
#define	PIT_SP_ALERTS			1		// S&P Alerts (52 Wk Hi/Low, etc..
#define	PIT_IERX				2		// R/H Notes and Restrictions
#define	PIT_CORP_ACTION			3		// Corporate Action records
#define	PIT_STOCK_OPTION		4		// Symbol decode US stock option
#define	PIT_CONDITIONS			5		// Condition decode

	// Values used in wFlags

#define	PIF_NOTIFY_DESTROY		1	// Notify parent of Destroy (WM_USER+0x6f)
#define	PIF_USER_DESTROY		2	// Don't capture mouse, let user destroy 

	// Definitions of Window Extra Words 

#define	GWL_PI_FLAGS			0
#define	GWL_PI_HTEXT			4

	// This message is sent to the popup window after the handle to the
	// memory holding the '/n' separated text has been set in the
	// GWW_PI_HTEXT word.

#define	WM_PROCESS_TEXT			WM_USER

#endif
