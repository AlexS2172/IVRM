#ifndef __MODELS_H__
#define __MODELS_H__


double CO_BlackScholes(	double	R,
						double	RF,
						double	dHTBRate,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						double* DivAmnt,
						double* DivYte,
						int		DivCount,
						GREEKS *pGreeks = NULL
                        );

double CO_StandardBinomial(
						double	R,
					    double	RF,
						double	dHTBRate,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,						    
						long	Steps,							
						GREEKS *pGreeks = NULL);

const long cnOptimizedBinomialSteps = 20;

double CO_OptimizedBinomial(
						double	R,
					    double	RF,
						double	dHTBrate,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,						    
						GREEKS *pGreeks = NULL);

double CO_VskLog(
						double	R,
					    double	RF,
					    double	S,
					    double	K,
					    double	V,
					    double	Yte, 
                        bool	Call,
					    bool	American,						 
						double* DivAmnt,						   
						double* DivYte,						    
						int		DivCount,
						double	Skew,
						double	Kurtosis,
						GREEKS *pGreeks = NULL);


double CO_CoxRossWithBlackScholes(	
						double	R,
						double	RF,
						double	dHTBrate,
						double	S,
						double	K,
						double	V,
						double	Yte, 
                        bool	Call,
						bool	American,
						double* DivAmnt,
						double* DivYte,
						int		DivCount,
						long	Steps,
						GREEKS *pGreeks = NULL
                        );

#endif	//__MODELS_H__