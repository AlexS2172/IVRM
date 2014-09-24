#ifndef __FUTUREMODELS_H__
#define __FUTUREMODELS_H__


double CFO_BlackScholes(double	R,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks = NULL
                        );

double CFO_VskLog(
						double	R,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double	Skew,
						double	Kurtosis,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks = NULL);


double CFO_CoxRossWithBlackScholes(	
						double	R,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						long	Steps,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks = NULL
                        );



double CFO_BlackScholes2(double	R,
						double	S,
						bool bSpotPrice,
						double	K,
						double	V,
						double	Yte, 
						bool	Call,
						bool	American,
						double*	pDivAmnts,
						double*	pDivYears,
						long	nCount,
						GREEKS *pGreeks = NULL
						);

double CFO_VskLog2(
				  double	R,
				  double	S,
				  bool bSpotPrice,
				  double	K,
				  double	V,
				  double	Yte, 
				  bool	Call,
				  bool	American,						 
				  double	Skew,
				  double	Kurtosis,
				  double*	pDivAmnts,
				  double*	pDivYears,
				  long	nCount,
				  GREEKS *pGreeks = NULL);


double CFO_CoxRossWithBlackScholes2(	
								   double	R,
								   double	S,
								   bool bSpotPrice,
								   double	K,
								   double	V,
								   double	Yte, 
								   bool	Call,
								   bool	American,
								   long	Steps,
								   double*	pDivAmnts,
								   double*	pDivYears,
								   long	nCount,
								   GREEKS *pGreeks = NULL
								   );

double CFO_BlackScholes3(double	R,
						 double	dYield,
						 bool bSpotPrice,
						 double	S,
						 double	K,
						 double	V,
						 double	Yte, 
						 bool	Call,
						 bool	American,
						 double*	pDivAmnts,
						 double*	pDivYears,
						 long	nCount,
						 GREEKS *pGreeks = NULL
						 );

double CFO_VskLog3(
				   double	R,
				   double	dYield,
				   bool bSpotPrice,
				   double	S,
				   double	K,
				   double	V,
				   double	Yte, 
				   bool	Call,
				   bool	American,						 
				   double	Skew,
				   double	Kurtosis,
				   double*	pDivAmnts,
				   double*	pDivYears,
				   long	nCount,
				   GREEKS *pGreeks = NULL);


double CFO_CoxRossWithBlackScholes3(	
									double	R,
									double	dYield,
									bool bSpotPrice,
									double	S,
									double	K,
									double	V,
									double	Yte, 
									bool	Call,
									bool	American,
									long	Steps,
									double*	pDivAmnts,
									double*	pDivYears,
									long	nCount,
									GREEKS *pGreeks = NULL
									);

#endif	//__FUTUREMODELS_H__