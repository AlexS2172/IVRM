/*
**** Special types and structures used in ATAPINFO
*/
#ifndef __ATAPINFO_H__
#define __ATAPINFO_H__

#ifdef	__cplusplus
extern	"C" {
#endif

#define	APP_ATSERVER				0
#define	APP_ATEXEC					1
#define	APP_PPAGE					2
#define	APP_OPTAN					3
#define	APP_CPAGE					4
#define	APP_HCHART					5
#define	APP_TCHART					6
#define	APP_NCHART					7
#define	APP_QUICK1					8
#define	APP_QUICK1_POP1				9
#define	APP_QUICK1_POP2				10
#define	APP_QUICK2					11
#define	APP_RANK					12
#define	APP_STAT					13
#define	APP_QVIEW					14
#define	APP_TICKER					15
#define	APP_SYMBOL					16
#define	APP_TOS						17
#define	APP_NEWS					18
#define	APP_NEWS90					19
#define	APP_TOPSTORY				20
#define	APP_NEWSKR					21
#define	APP_NEWSDJF					22
#define	APP_NEWSICV					23
#define	APP_KRVIEW					24
#define	APP_ATRSF					25
#define	APP_ATSST					26
#define	APP_SCOPE					27
#define	APP_CARVIEW					28
#define	APP_SPMVIEW					29
#define	APP_EARNVIEW				30
#define	APP_MMS						31
#define	APP_TRATE					32
#define	APP_CROSS					33
#define	APP_ATNQDS					34
#define	APP_ATMMPQL					35
#define	APP_DDESRV					36
#define	APP_TPA						37
#define	APP_MICROHEDGE				38
#define	APP_ACCTEXEC				39
#define	APP_ENVVIEW					40
#define	APP_JJKENNY					41
#define	APP_JJKENNY_POPBLK			42
#define	APP_JJKENNY_POPFLS			43
#define	APP_PPAGE_TICKER			44
#define	APP_PPAGE_BASKET			45
#define	APP_TRATE_POPUP				46
#define	APP_ATMM_MMTICKER			47
#define	APP_ATMM_MMCHART			48
#define	APP_OPT_TICKER				49
#define	APP_NEWS_TICKER				50
#define	APP_OPT_UNDER				51
#define	APP_QVIEW_TICKER			52
#define	APP_ATGOVPX					53
#define	APP_ATCOMP					54
#define	APP_ATFCALL					55
#define	APP_LQUOTE					56
#define	APP_SQUOTE					57
#define	APP_LOADLAYOUT				58
#define	APP_NEWSEDGE				59
#define	APP_ATMMNT					60
#define	APP_ATDISCLS				61
#define	APP_ATPRESS					62
#define	APP_ATPORT					63
#define	APP_ATPORT_SYMBOL			64
#define	APP_ATVICK					65
#define	APP_LQUOTE_TICKER			66
#define	APP_SQUOTE_TICKER			67
#define	APP_CARVIEW_POPUP			68
#define	APP_SYMBOL_POPUP			69
#define	APP_ATZACK					70
#define	APP_MKT_TICKER				71
#define	APP_MKT_SYMBOL				72
#define	APP_ATMNSTAR				73
#define	APP_CHARTH					74
#define	APP_ATHALT					75
#define	APP_ATLAS					76
#define	APP_ATDATAMAN				77
#define	APP_ATHIST					78
#define	APP_ATLINK					79
#define	APP_NEWSEDGE_NMDI			80
#define	APP_NEWSEDGE_MDI			81
#define	APP_ATBV					82
#define	APP_LEHQUOTE				83
#define	APP_ATRHBS					84
#define	APP_ATALERT					85
#define	APP_MICROHEDGE4				86
#define	APP_NYQUOTES				87

#define MAX_APPS					88

typedef	struct
		{
		char	szAppClass[16];			// App. Class name.
		char	szAppExec[16];			// App. Executable name.
		char	szAppName[32];			// App. name.
		DWORD	bATServer:1,			// The AT server app.
				bATExec:1,				// The AT exec app.
				bATApp:1,				// Not a foreign app.
				bATChild:1,				// Not main app.
				bUsesDataman:1,			// App. uses DataManager.
				bATLayout:1,			// App. response to WM_AT_RECON.
				bIconActivate:1,		// Activate app. from ICON.
				bLinkMenu:1,			// Included in link menu.
				bLinkChannel:1,			// App. supports link channels.
				bNewsInfo:1,			// App. accepts news info blocks.
				reserver:22;			// Spare flags.
		}
		APP_INFO;

			// ATAPINFO.C

BOOL	WINAPI	GetAppInfo(int iAppIndex,APP_INFO *AppInfo);
BOOL	WINAPI	GetATAppName(int iAppIndex,LPSTR szString,DWORD dwFlags);
int		WINAPI	FindAppInfo(APP_INFO *AppInfo);
BOOL	WINAPI 	IsAtServerRunning(void);

			// Get App. Name flags.

#define	GAN_SHORTNAME			0x0001
#define	GAN_STRIPACC			0x0002

#ifdef __cplusplus
}
#endif

#endif

