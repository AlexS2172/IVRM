/*
**** Special types and structures used in CURRENCY
*/
#ifndef __CURRENCY_H__
#define __CURRENCY_H__

#ifdef	__cplusplus
extern	"C" {
#endif

#define CUR_ARP						0
#define CUR_ATS						1
#define CUR_AUS						2
#define CUR_BEF						3
#define CUR_BRC						4
#define CUR_CAD						5
#define CUR_CHF						6
#define CUR_CLP						7
#define CUR_DEM						8
#define CUR_DKK						9
#define CUR_ESP						10
#define CUR_FIM						11
#define CUR_FRF						12
#define CUR_GBP						13
#define CUR_GRD						14
#define CUR_HKD						15
#define CUR_IDR						16
#define CUR_IEP						17
#define CUR_ILS						18
#define CUR_INR						19
#define CUR_ITL						20
#define CUR_JPY						21
#define CUR_KRW						22
#define CUR_MXP						23
#define CUR_MYR						24
#define CUR_NLG						25
#define CUR_NOK						26
#define CUR_NZD						27
#define CUR_PHP						28
#define CUR_PKR						29
#define CUR_PTE						30
#define CUR_SEK						31
#define CUR_SGD						32
#define CUR_THB						33
#define CUR_TRL						34
#define CUR_TWD						35
#define CUR_USD						36
#define CUR_VEB						37
#define CUR_ZAR						38

#define MAX_CURRENCYS				39

typedef	struct
		{
		char	szCurrCode[4];
		char	szCurrName[24];
		int		exchQST;			// Exchange rate quote server type 
		char	szExchSym[16];		// Exchange rate symbol 
		DWORD	bMultiply :1,		// Multiply rate.
				bHundreds :1,		// Rate is times one hundred.
				bUSDollar :1,		// US Dollar (No conversion).
				reserved  :30;		// Spare flags.
		}
		CURR_INFO;

			// CURRENCY.CPP

BOOL	WINAPI	GetCurrencyInfo(int iCurIndex,CURR_INFO *CurrInfo);
int		WINAPI	FindCurrencyInfo(CURR_INFO *CurrInfo);
int		WINAPI	GetCurrencyCode(int iCurrIndex,LPSTR szString);

#ifdef __cplusplus
}
#endif

#endif


