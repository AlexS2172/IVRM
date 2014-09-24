
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the OPTIONCALC_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// OPTIONCALC_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#if !defined(AFX_OPTIONCALC_H__385A6A19_B778_4162_94AD_57F1BA2592F9__INCLUDED_)
#define AFX_OPTIONCALC_H__385A6A19_B778_4162_94AD_57F1BA2592F9__INCLUDED_


#ifdef OPTIONCALC_EXPORTS
#define OPTIONCALC_API WINAPI
//#define OPTIONCALC_API __declspec(dllexport)
#else
#define OPTIONCALC_API WINAPI
//#define OPTIONCALC_API __declspec(dllimport)
#endif


extern "C" {

#include "DataTypes.h"
#include "DateConverter.h"

double OPTIONCALC_API NormalC(double X);

// returns difference between 2 dates in years
double OPTIONCALC_API GetDateDiff(time_t begDate, time_t endDate);
bool OPTIONCALC_API OleDateToUnixDate(double dateIn, time_t* uxDate);
bool OPTIONCALC_API TmToDATE( struct tm* pTm, double *pDateOut );
bool OPTIONCALC_API TmToDateEx( struct tm* pTm, double *pDateOut );

bool OPTIONCALC_API GetNYDateTimeAsDATE(double *pdtDate);

bool OPTIONCALC_API GetCalculationParams(double		dtNow,
										 double		dtExpiryOV,
										 double		dtCloseTime,
										 bool		bUseTime,
										 double*	dtDateCalc,
										 double*	dtExpiryCalc,
										 double*	dtCloseTimeCalc,
										 double*	dtYte);

double OPTIONCALC_API  InterpolateRates(	long nCount, 
											const RATE *pRates, 
											double dYte);

double OPTIONCALC_API  InterpolateRates2(	long nCount, 
											double* pRates, 
											double* pdYte, 
											double dYte);

long OPTIONCALC_API  GetDividendsCount(		time_t nToday, 
											double	dYte, 
											time_t	nLastDivDate, 
											long nFrequency ); 

long OPTIONCALC_API  GetBasketDividendsCount(	time_t	nToday, 
												double	dYte, 
												REGULAR_DIVIDENDS*	pDividends,
												long				nCount); 

// Fills array of dividends using given last dividend amount, last dividend day and 
// dividend frequency. Returns 0 if failed, or value greater than zero otherwise.
// pnCount contains number of dividends available prior expiry date.
long OPTIONCALC_API GetDividends(		time_t nToday, 
										double dYte, 
										time_t nLastDivDate, 
										long nFrequency, 
										double dAmount, 
										long nCount, 
										DIVIDEND *pDividends, 
										long *pnCount );

long OPTIONCALC_API  GetDividends2(		 time_t nToday, 
										 double	dYte, 
										 time_t nLastDivDate, 
										 long nFrequency, 
										 double dAmount, 
										 long nCount, 
										 double* pDivAmnts, 
										 double* pDivDays,
										 long *pnCount );

long OPTIONCALC_API  GetBasketDividends(	  time_t				nToday, 
											  double				dYte, 
											  REGULAR_DIVIDENDS*	pDividends,
											  unsigned				nCount,
											  double*				pDivAmnts,
											  double*				pDivDays,
											  long					nInCount,
											  long*					pnOutCount);

////////////////////////////////////////////////////////////
// Rates
////////////////////////////////////////////////////////////
// Interpolates rates. Returns 0 if failed, or value greater than 0 if succeeded
// nDTE must be greater that zero. Input rates array assumed to be unsorted.
//double OPTIONCALC_API InterpolateRates( long nCount, const RATE *pRates, long nDTE );
//double OPTIONCALC_API InterpolateRates2( long nCount, double* pRates, long* pnDTEs, long nDTE );

////////////////////////////////////////////////////////////
// Dividends
// 
// All dates in functions and structures are 
// number of days since 30 December 1899, midnight.
////////////////////////////////////////////////////////////

// Returns number of dividends or -1 if error occured.
/*long OPTIONCALC_API GetDividendsCount( long nToday, long nDTE, 
									   long nLastDivDate, long nFrequency ); 

long OPTIONCALC_API GetBasketDividendsCount(long				nToday, 
											long				nDTE, 
											REGULAR_DIVIDENDS*	pDividends,
											long				nCount);*/ 

// Fills array of dividends using given last dividend amount, last dividend day and 
// dividend frequency. Returns 0 if failed, or value greater than zero otherwise.
// pnCount contains number of dividends available prior expiry date.
//long OPTIONCALC_API GetDividends( long nToday, long nDTE, 
//								  long nLastDivDate, long nFrequency, double dAmount, 
//								  long nCount, DIVIDEND *pDividends, long *pnCount );
//
//long OPTIONCALC_API GetDividends2( long nToday, long nDTE, 
//								  long nLastDivDate, long nFrequency, double dAmount, 
//								  long nCount, double* pDivAmnts, double* pDivDays,
//								  long *pnCount );
//
//long OPTIONCALC_API GetBasketDividends(	long				nToday, 
//										long				nDTE, 
//										REGULAR_DIVIDENDS*	pDividends,
//										unsigned			nCount,
//										double*				pDivAmnts,
//										double*				pDivDays,
//										long				nInCount,
//										long*				pnOutCount);


void PrepareDivData(const DIVIDEND *pDividends,std::vector<double> *divytes,std::vector<double> *divamts, int nCount);

////////////////////////////////////////////////////////////
// Implied volatility
//
// All dates in functions and structures are 
// number of days since 30 December 1899, midnight.
////////////////////////////////////////////////////////////

// Calculates volatility. Returns -1 if error occured.
double OPTIONCALC_API CalcVolatilityCustDivs( double dDomesticRate, double dForeignRate,
									  double dSpotPrice, double dOptionPrice, double dStrike, 
	 								  double dYTE, long nIsCall, long nIsAmerican,
									  long nCount,  double*	pDivAmnts, double* pDivYears, long nSteps );


// Calculates volatility. Returns -1 if error occured.
double OPTIONCALC_API CalcVolatility( double dDomesticRate, double dForeignRate,
									  double dSpotPrice, double dOptionPrice, double dStrike, 
	 								  double dYTE, long nIsCall, long nIsAmerican,
									  long nCount, const DIVIDEND *pDividends, long nSteps );

////////////////////////////////////////////////////////////
// Greeks
//
// All dates in functions and structures are 
// number of days since 30 December 1899, midnight.
////////////////////////////////////////////////////////////
// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
long OPTIONCALC_API CalcGreeksCustDivs( double dDomesticRate, double dForeignRate, 
									double dSpotPrice, double dStrike, double dVolatility, 
									double dYTE, long nIsCall, long nIsAmerican, 
									long nCount, double*	pDivAmnts, double* pDivYears, long nSteps, 
									/*out*/GREEKS *pGreeks );


// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
long OPTIONCALC_API CalcGreeks( double dDomesticRate, double dForeignRate, 
								double dSpotPrice, double dStrike, double dVolatility, 
								double dYTE, long nIsCall, long nIsAmerican, 
								long nCount, const DIVIDEND *pDividends, long nSteps, 
								/*out*/GREEKS *pGreeks );

// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
// Uses alternative algorithm to calculate Delta and Gamma.
long OPTIONCALC_API CalcGreeksEx( double dDomesticRate, double dForeignRate, 
								  double dSpotPrice, double dStrike, double dVolatility, 
								  double dYTE, long nIsCall, long nIsAmerican, 
								  long nCount, const DIVIDEND *pDividends, long nSteps, 
								  /*out*/GREEKS *pGreeks );


// Calculates greeks. Returns 0 if failed, or value greater than zero otherwise.
// Uses alternative algorithm to calculate Delta and Gamma.
long OPTIONCALC_API CalcGreeksExCustDivs( double dDomesticRate, double dForeignRate, 
								  double dSpotPrice, double dStrike, double dVolatility, 
								  double dYTE, long nIsCall, long nIsAmerican, 
								  long nCount, double*	pDivAmnts, double* pDivYears, long nSteps, 
								  /*out*/GREEKS *pGreeks );

								  
double OPTIONCALC_API CalcVolatilityMM2(double	dDomesticRate,	
										double	dForeignRate,
										double	dHTBRate,
										double	dSpotPrice,
										double	dOptionPrice,
										double	dStrike,
										double	dYTE,
										long	nIsCall,
										long	nIsAmerican,
										long	nCount, 
										double*	pDivAmnts,
										double*	pDivYears,
										long	nSteps,
										double	dSkew,
										double	dKurtosis,
										long	nModel);

double OPTIONCALC_API CalcVolatilityMM3(double	dDomesticRate,	
										double	dForeignRate, 
										double	dHTBRate,
										double	dSpotPrice,
										double	dOptionPrice,
										double	dStrike,
										double	dYTE,
										long	nIsCall,
										long	nIsAmerican,
										long	nCount, 
										double*	pDivAmnts,
										double*	pDivYears,
										long	nSteps,
										double	dSkew,
										double	dKurtosis,
										long	nModel,
                                        long*   pnFlag);

long OPTIONCALC_API CalcGreeksMM2(	double	dDomesticRate,
									double	dForeignRate,		
									double	dHTBRate,
									double	dSpotPrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									long	nIsCall,
									long	nIsAmerican,		
									long	nCount, 
									double*	pDivAmnts,			
									double*	pDivYears,
									long	nSteps,				
									double	dSkew,
									double	dKurtosis,
									long 	nModel,
									GREEKS*	pGreeks);
								  
double OPTIONCALC_API CalcForwardPrice( double dSpotPrice, 
	                                    double dYTE, /*date of option expiration*/
	                                    long nDivCount, /* size of dividends array*/
	                                    double* pDivAmnts, /* array of dividends amounts*/
	                                    double* pDivYears, /* array of time to dividends payments*/
	                                    double dForeignRate, /*yield for index option */
	                                    long nRateCount, /*size of rates array*/
	                                    const RATE *pRates /*array of rates*/ );

double OPTIONCALC_API CalcForwardPrice2( double dSpotPrice, 
	                                     double dYTE, /*date of option expiration*/
	                                     long nDivCount, /* size of dividends array*/
	                                     double* pDivAmnts, /* array of dividends amounts*/
	                                     double* pDivYears, /* array of time to dividends payments*/
	                                     double dForeignRate, /*yield for index option */
	                                     long nRateCount, /*size of rates array*/
	                                     double* pRates, /*array of rates*/ 
                                         double* pdYTEs /*array of time to rates*/ );

double OPTIONCALC_API CalcFutureOptionVolatility(double	dDomesticRate,	
										double	dFuturePrice,
										double	dOptionPrice,
										double	dStrike,
										double	dYTE,
										long	nIsCall,
										long	nIsAmerican,
										long	nSteps,
										double	dSkew,
										double	dKurtosis,
										long	nModel,
										long*   pnFlag);

long OPTIONCALC_API CalcFutureOptionGreeks(	double	dDomesticRate,
										   double	dFuturePrice,
										   double	dStrike,
										   double	dVolatility, 
										   double	dYTE,
										   long	nIsCall,
										   long	nIsAmerican,
										   long	nSteps,
										   double	dSkew,
										   double	dKurtosis,
										   long	nModel,
										  // const DIVIDEND *pDividends,
										   GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeksCustDivs(	double	dDomesticRate,
									double	dFuturePrice,
									double	dStrike,
									double	dVolatility, 
									double	dYTE,
									long	nIsCall,
									long	nIsAmerican,
									long	nSteps,				
									double	dSkew,
									double	dKurtosis,
									long 	nModel,
									long	nCount,
									double *pDivAmnts,
									double *pDivYears,
									GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeks2(	double	dDomesticRate,
										   double	dFuturePrice,
										   bool		bSpotPrice,
										   double	dStrike,
										   double	dVolatility, 
										   double	dYTE,
										   long	nIsCall,
										   long	nIsAmerican,
										   long	nSteps,
										   double	dSkew,
										   double	dKurtosis,
										   long	nModel,
										   // const DIVIDEND *pDividends,
										   GREEKS*	pGreeks);


long OPTIONCALC_API CalcFutureOptionGreeksCustDivs2(	double	dDomesticRate,
												   double	dFuturePrice,
												   bool		bSpotPrice,
												   double	dStrike,
												   double	dVolatility, 
												   double	dYTE,
												   long	nIsCall,
												   long	nIsAmerican,
												   long	nSteps,				
												   double	dSkew,
												   double	dKurtosis,
												   long 	nModel,
												   long	nCount,
												   double *pDivAmnts,
												   double *pDivYears,
												   GREEKS*	pGreeks);


// a version with [custom] dividends, with Spot/Forward switching 
// for commodities and with Yield using...

long OPTIONCALC_API CalcFutureOptionGreeks3(	double	dDomesticRate,
												double	dYield,
												double	dFuturePrice,
												bool	bSpotPrice,
												double	dStrike,
												double	dVolatility, 
												double	dYTE,
												long	nIsCall,
												long	nIsAmerican,
												long	nSteps,				
												double	dSkew,
												double	dKurtosis,
												long 	nModel,
												long	nCount,
												double *pDivAmnts,
												double *pDivYears,
												GREEKS*	pGreeks);


////////////////////////////////////////////////////////////
// Helpers
////////////////////////////////////////////////////////////

// Check is symbol a call. Returns -1 if error occured and SetLast Error - ERROR_INVALID_PARAMETER
int OPTIONCALC_API IsCall( LPCTSTR cSymbol );

// Check is two symbols make a pair. Returns -1 if error occured and SetLast Error - ERROR_INVALID_PARAMETER
int OPTIONCALC_API IsSymbolsPair( LPCTSTR cSymbol1, LPCTSTR cSymbol2 );

}	// extern "C"


// Please include "EgCommon\lib" path to "Tools->Options->VC++ Directories->Library Files" and section (VC7)
// or in "Tools->Options->Directories->Library Files" section (VC6)
//
// Please include "C:\Program Files\Common Files\Egar" path to "Tools->Options->VC++ Directories->Executable Files" and section (VC7)
// or in "Tools->Options->Directories->Executable Files" section (VC6)
#ifndef OPTIONCALC_EXPORTS
	#ifdef _DEBUG
		#ifdef _UNICODE 
			#pragma comment(lib, "OptionCalcUD.lib") 
			#pragma message("Automatically linking with OptionCalcUD.dll")
		#else 
			#pragma comment(lib, "OptionCalcD.lib") 
			#pragma message("Automatically linking with OptionCalcD.dll")
		#endif //_UNICODE 

	#else //_DEBUG 

		#ifdef _UNICODE 
			#pragma comment(lib, "OptionCalcU.lib") 
			#pragma message("Automatically linking with OptionCalcU.dll")
		#else 
			#pragma comment(lib, "OptionCalc.lib") 
			#pragma message("Automatically linking with OptionCalc.dll")
		#endif //_UNICODE 
	#endif //_DEBUG
#endif

#endif //!defined(AFX_OPTIONCALC_H__385A6A19_B778_4162_94AD_57F1BA2592F9__INCLUDED_)
