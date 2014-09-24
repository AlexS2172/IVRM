#include "stdafx.h"

#include <math.h>
#include <memory.h>

#include "opt.h"
#include <OptionCalc\OptionCalc.h>

// Normal Distribution
double RateToCnt(double R, int Dte) 
{
	double Yte	= Dte / 360.;
	double Yte_	= Dte / DaysPerYear;
	double DF = Yte > 1	? pow(1.0 + R, Yte) : 1.0 + R * Yte;		
	return log(DF) / Yte_;
}

double NormalC(double s) 
{
	double dX = s / sqrt(2.);	
	double dW = fabs(dX);
    double dY = 0.;

// evaluate  erf  for  |x| <= 0.46875
	const double dXBreak = 0.46875;

	if (dW <= dXBreak)
	{
        const double dA[5] = {3.16112374387056560e00, 1.13864154151050156e02,
					          3.77485237685302021e02, 3.20937758913846947e03,
             				  1.85777706184603153e-1};
        const double dB[4] = {2.36012909523441209e01, 2.44024637934444173e02,
							  1.28261652607737228e03, 2.84423683343917062e03};

// a and b are arrays of double; arrays indexation is from zero, so that
// array a of size contains elements a[0], a[1], a[2], a[3], a[4].
        const double dZ = dW * dW;
        double dXNum = dA[4] * dZ;
        double dXDen = dZ;

        for(int i = 0; i < 3; i++)
		{
			dXNum = (dXNum + dA[i]) * dZ;
	        dXDen = (dXDen + dB[i]) * dZ;
        }

        dY = dX* (dXNum + dA[3]) / (dXDen + dB[3]);
	}
    else if (dW <= 4.) // evaluate  erfc  for 0.46875 <= |x| <= 4.0
	{    
		const double dC[9] = {5.64188496988670089e-1, 8.88314979438837594e00,
							  6.61191906371416295e01, 2.98635138197400131e02,
							  8.81952221241769090e02, 1.71204761263407058e03,
							  2.05107837782607147e03, 1.23033935479799725e03,
							  2.15311535474403846e-8};
	    
		const double dD[8] = {1.57449261107098347e01, 1.17693950891312499e02,
							  5.37181101862009858e02, 1.62138957456669019e03,
							  3.29079923573345963e03, 4.36261909014324716e03,
							  3.43936767414372164e03, 1.23033935480374942e03};
            
        double dXNum = dC[8] * dW;
        double dXDen = dW;
        
		for(int i = 0; i < 7; i++)
	    {
			dXNum = (dXNum + dC[i]) * dW;
            dXDen = (dXDen + dD[i]) * dW;
        }
        
		dY = (dXNum + dC[7]) / (dXDen + dD[7]);
           
        double dZ = dW > 0 ? floor(dW * 16.) / 16. : ceil(dW * 16.) / 16.;
        double dDel = (dW - dZ) * (dW + dZ);

        dY = exp(-dZ * dZ) * exp(-dDel) * dY;
	}
    else // evaluate  erfc  for |x| > 4.0
	{
        const double dP[6] = {3.05326634961232344e-1, 3.60344899949804439e-1,
							  1.25781726111229246e-1, 1.60837851487422766e-2,
							  6.58749161529837803e-4, 1.63153871373020978e-2};
        const double dQ[5] = {2.56852019228982242e00, 1.87295284992346047e00,
							  5.27905102951428412e-1, 6.05183413124413191e-2,
							  2.33520497626869185e-3};
        
        double dZ = 1. / (dW * dW);
        double dXNum = dP[5] * dZ;
        double dXDen = dZ;

        for(int i = 0; i < 4; i++)
	    {
            dXNum = (dXNum + dP[i]) * dZ;
            dXDen = (dXDen + dQ[i]) * dZ;
		}

        dY = dZ * (dXNum + dP[4]) / (dXDen + dQ[4]);
        dY = (1. / sqrt(OPM::Pi) -  dY) / dW;
        dZ = dW > 0 ? floor(dW * 16.) / 16. : ceil(dW * 16.) / 16.;

        double dDel = (dW - dZ) * (dW + dZ);

        dY = exp(-dZ * dZ) * exp(-dDel) * dY;
// check that result is finite, in other case set it to zero
        if ( _finite(dY) == 0 ) dY = 0; 
	}

// fix up for negative argument, erf, etc.
	if (dX > dXBreak) 
        dY = 1. - dY;
	else if (dX < -dXBreak) 
        dY = dY - 1.;

// fix up for NormalC()
    return dY = (dY + 1.) / 2.;
}

double OP_CoxRub( double R, double S, double K, double SIG, double T, double RF, bool CALL,
				  bool AMERICAN, int STEPS, int noofdiv,  double * DTime,double * DDIV,
                  C_QMATRIX* pQMatrix, double* pSpotAjustDiv)
{
	int STEPS_T_2;
	int TOP_BOUNDRY, BOTTOM_BOUNDRY, stop;
	int OLD_TOP;
	int OLD_BOTTOM;

	double 
		U, SQRT_T,
		e_v,
		e_u,
		p,
		r_temp,
		a, int_val,
		p_div_r,
		one_m_p_div_r,
		one_m_p, last_value;

	int z, finish, i, j, passed_eeb, divstep, startstep, noofdivl, nextdivoff;

    // clear
    if (pQMatrix)
		memset(pQMatrix, 0, sizeof(C_QMATRIX));

	STEPS_T_2 = STEPS * 2;
	SQRT_T = sqrt(T);
	
	if (noofdiv > 0)
		RF=0.0;   //GI

	a = exp( (R - RF)*T/STEPS);
	
	noofdivl = 0;
	nextdivoff = 0;
	
	for ( j = 0; j < noofdiv; j++) 
	{
	   if ((DTime[j] >= 0.0) && (DTime[j] <= T) && (DDIV[j] != 0.0)) 
	   {
			nextdivoff = j;

			S -= DDIV[j];
			
			if (S <= 0.0) 
				return  ERR_VALUE;
			
			divstep = (int) (DTime[j] / T  * STEPS);
			noofdivl++;
	   }
	}

    // fill output spot value
    if (pSpotAjustDiv)
        *pSpotAjustDiv = S;

	noofdiv = noofdivl;

	U   = SIG * SQRT_T / sqrt( (double)STEPS);
	
	e_u = exp( U );
	e_v = 1 / e_u ;

	if (e_u == e_v) 
		return ERR_VALUE;

	p = ( a - e_v ) / ( e_u - e_v );

	double int_val_array[MAX_STEPS + MAX_STEPS + 1];
	double array[MAX_STEPS + 1];

	r_temp = exp(R * T / STEPS);

	one_m_p = 1-p;
	p_div_r = p / r_temp;
	one_m_p_div_r = one_m_p / r_temp;

	if (noofdiv > 0) 
	{
		if (divstep < 0) divstep = 0 ;
		
		stop = divstep;
	}
	else
		stop = 0;
	
	startstep = STEPS;

	do 
	{
		passed_eeb = 0;
		
		memset(int_val_array, 0, sizeof(double) * (MAX_STEPS + MAX_STEPS + 1));
		
		if (CALL) 
		{
			last_value = S * exp(U * STEPS);
			
			for (z = STEPS_T_2; z >= 0; z--) 
			{
				if ((int_val_array[z] = last_value - K) >= 0.0) 
                {
					last_value *= e_v;
                }
				else
                {
            		int_val_array[z] = 0.0;
					break;
                }
			};
		
			BOTTOM_BOUNDRY = z/2;
	   }
	   else 
	   { 
			last_value = S / exp(U * STEPS);
		
			for(z = 0; z <= STEPS_T_2; z++) 
			{
				if ((int_val_array[z] = K - last_value) >= 0.0 ) 
                {
					last_value *= e_u;
                }
				else 
                {
        			int_val_array[z] = 0.0;
					break;
                }
			};

			TOP_BOUNDRY = z/2;
	   };

	   if (noofdivl == noofdiv) 
	   {
			for (z = 0; z <= STEPS; z++) 
				array[z] = int_val_array[z+z];
	   }
	   else 
	   {
			if ( TOP_BOUNDRY < OLD_TOP )
				TOP_BOUNDRY = OLD_TOP;

			if ( BOTTOM_BOUNDRY > OLD_BOTTOM )
				BOTTOM_BOUNDRY = OLD_BOTTOM;

			for( z = 0; z <= startstep; z++ ) 
			{
				int_val = int_val_array[STEPS + z + z - startstep];

				if (int_val >= array[z]) 
					array[z] = int_val;
				else 
					int a = 0;
			}
		}

        if (startstep < QSIZE && pQMatrix)
        {
    		if (CALL) 
            {
                C_QVECTOR& vec = (*pQMatrix)[startstep];
                for (z = 0; z <= startstep; z++)
                    vec[z] = array[z];
            }
            else
            {
    		    finish = (startstep < TOP_BOUNDRY) ?  startstep : TOP_BOUNDRY;
                C_QVECTOR& vec = (*pQMatrix)[startstep];
                for (z = 0; z <= finish; z++)
                    vec[z] = array[z];
            }
        }

		startstep--;

		if (CALL) 
		{
            if (AMERICAN)
            {
    			for (i = startstep; i >= stop; i--)
	    		{
		    		if (BOTTOM_BOUNDRY > 0) BOTTOM_BOUNDRY --;

			    	for(z = BOTTOM_BOUNDRY; z <= i; z++)
				    {
					    int_val	= int_val_array[STEPS + z + z - i];
					    array[z] = p_div_r * array[z+1] + one_m_p_div_r * array[z];

					    if (int_val > array[z])
						    array[z] = int_val;
				    }

                    if (i < QSIZE && pQMatrix)
                    {
                        C_QVECTOR& vec = (*pQMatrix)[i];
                        for (z = 0; z <= i; z++)
                            vec[z] = array[z];
                    }
                }
			}
            else
            {
    			for (i = startstep; i >= stop; i--) 
	    		{
		    		if (BOTTOM_BOUNDRY > 0) BOTTOM_BOUNDRY --;

			    	for(z = BOTTOM_BOUNDRY; z <= i; z++) 
					    array[z] = p_div_r * array[z+1] + one_m_p_div_r * array[z];

                    if (i < QSIZE && pQMatrix)
                    {
                        C_QVECTOR& vec = (*pQMatrix)[i];
                        for (z = 0; z <= i; z++)
                            vec[z] = array[z];
                    }
                }
            }
	    }
        else 
	    {
            if (AMERICAN)
            {
                for (i = startstep; i >= stop; i--) 
			    {
				    finish = (i < TOP_BOUNDRY) ?  i : TOP_BOUNDRY;
				
				    for (z = 0; z <= finish; z++) 
				    {
					    int_val	= int_val_array[STEPS+z+z-i];
					    array[z]= p_div_r * array[z+1] + one_m_p_div_r * array[z];

    					if (int_val > array[z]) 
	    					array[z] = int_val;
		    		}

                    if (i < QSIZE && pQMatrix)
                    {
                        C_QVECTOR& vec = (*pQMatrix)[i];
                        for (z = 0; z <= finish; z++)
                            vec[z] = array[z];
                    }
                }
            }
            else
            {
                for (i = startstep; i >= stop; i--) 
			    {
				    finish = (i < TOP_BOUNDRY) ?  i : TOP_BOUNDRY;
				
				    for (z = 0; z <= finish; z++) 
					    array[z] = p_div_r * array[z+1] + one_m_p_div_r * array[z];

                    if (i < QSIZE && pQMatrix)
                    {
                        C_QVECTOR& vec = (*pQMatrix)[i];
                        for (z = 0; z <= finish; z++)
                            vec[z] = array[z];
                    }
                }
            }
	   }

	   if (noofdivl > 0) 
	   {
			i++;
			
			startstep = i;
			
			S += DDIV[nextdivoff];
			
			noofdivl--;

			if (noofdivl != 0)
			{
				do 
				{
					nextdivoff--;
				
					if (nextdivoff < 0) 
						return ERR_VALUE;
				}
				while (!((DTime[nextdivoff] >= 0.0) && (DTime[nextdivoff] <= T) && (DDIV[nextdivoff] != 0.0)));

				divstep = (int) (DTime[nextdivoff] / T  * STEPS);
				
				if (divstep < 0) 
					divstep = 0;

				if (divstep > i) 
					return ERR_VALUE;
				
				stop = divstep;
			}
			else
				stop = 0;
			
			OLD_BOTTOM = BOTTOM_BOUNDRY;
			OLD_TOP = TOP_BOUNDRY;
		}
	} 
	while (i >= 0);
	
	return array[0];
}

inline double CRValue( double	R,
					   double	S,
					   double	K,
					   double	V,
					   double	T,
					   double	RF,
					   bool	CALL,
					   bool	AMERICAN,
					   int		BSteps,
					   int		NoOfDivs,
					   double	*YearsToDate,
    				   double	*dividends,
                       C_QMATRIX* pQMatrix_Up = NULL,
                       C_QMATRIX* pQMatrix_Down = NULL,
                       double* pSpotAjustDiv = NULL)
{
	// binomial , didvidents should be discounted by now
	double dPrice1 = OP_CoxRub( R, S, K, V, T, RF, CALL, AMERICAN, BSteps, NoOfDivs, YearsToDate, dividends, pQMatrix_Up, pSpotAjustDiv);
  	double dPrice2 = OP_CoxRub( R, S, K, V, T, RF, CALL, AMERICAN, BSteps-1, NoOfDivs, YearsToDate, dividends, pQMatrix_Down, pSpotAjustDiv);
  	
	return (dPrice1 + dPrice2) * 0.5;
}

/*******************************************************************************
** OP_Black																							**
********************************************************************************/
double OP_Black( 
				double	R,
				double	S,
				double	K,
				double	Vol,
				double	T,
				double	RF,
				bool	Call
                )
{
	double	A, Ln1, TSqR, x1, x2, x3, x4, x5, x6, X, Xs, NsX, NsXs;

	Ln1 = R - RF;
	A	= Ln1 / (Vol * Vol) + .5;

	TSqR = sqrt(T);
		
	x1 = Vol * TSqR;
	x2 = x1 * A;
	x3 = 1 / exp(R * T);
	x4 = 1 / exp(RF * T);
	x5 = x4 * S;
	x6 = x3 * K;

	X	= log(S / K) / x1 + x2;
	Xs	= X - x1;

	NsX		= NormalC(X) - (Call ? 0 : 1);
	NsXs	= NormalC(Xs) - (Call ? 0 : 1);

	return (x5 * NsX - x6 * NsXs);
}

/*void OptionOnFutures(LP_C530004 C530004) 
{
	// WhichGreeks - Bit 1 On if gamma, Bit2On if kappa, Bit3On if thetta

	double	vol=C530004->OptVal.AnyOptVal.Vol;			// volatility, i.e. .2 for20%
	double	t=C530004->OptVal.AnyOptVal.TimeToExpir;	// time in years
	double	f=C530004->OptVal.AnyOptVal.UnderPrice;		// futures price
	double	sp=C530004->OptVal.AnyOptVal.StrikePrice;	// strike price
	double	DFactDom=C530004->OptVal.AnyOptVal.DomDiscFact; //discount factor 
	double	DomCntRate=C530004->OptVal.AnyOptVal.DomCntRate; // continuous interest rate,
	
	int pu = (C530004->OptVal.AnyOptVal.MiscFlags & Bit4On ) ? 1 : 0; // put = 1, call = 0

	double d1,d2, nd2,v;
	double d1a, d1b;
	
	//x5 = DFactDom * f;
	//x6 = DFactDom * sp;
	
	TSqR	= sqrt(T)
	x1		= V * TSqR;

	x3 = 1 / exp(R * T);
	x4 = 1 / exp(RF * T);
	
	x5 = x4 * F;
	x6 = x3 * K;
	
	//v = vol * sqrt(t);

	d1a = log(f / sp);
	d1b = 0.5 * vol * vol * t;

	X   = (d1a + d1b) / x1;
	
	//NsX  = NormalC(d1);
	//NsX -= pu;

	Xs   = X - x1;
	
	//nd2  = NormalC(d2);
	//nd2 -= pu;

	NsX		= NormalC(X) - (Call ? 0 : 1);
	NsXs	= NormalC(Xs) - (Call ? 0 : 1);

	dValue =  x5 * NsX  - x6 * NsXs;

	pGreeks->dDelta = exp(-FT) * (N1 - (nIsCall == 0 ? 1 : 0));
	dCntrDelta = - NsXs * DFactDom;
	dDelta = NsX * DFactDom;

	C530004->OptVal.ROD1 = - t * C530004->OptVal.OptValue / 100.;    //per .1%
	C530004->OptVal.ROD2 = t * t * C530004->OptVal.OptValue / 10000.;

	// Gamma
	dGamma	= exp(-d1*d1*.5) * PiF  * DFactDom /(f*v);

	dValue = A * dD1dVol * n1 - B * dD2dVol * n2;
	pGreeks->dVega = dValue * dV;
			
	// Vega 
	dVega	= x5*sqrt(t)*exp(-d1*d1*.5)*PiF ;

	// Theta
	DerR	= DerCntDiscFact(DomCntRate, t);
	dTheta	= - DerR * f * NsX + DerR * sp* nd2 - x6 * v / (2*t) * exp(-d1*d1*.5) * PiF;
}
*/

/*******************************************************************************
** CalculateOption															  **
********************************************************************************/
/*
process dividents before calling GKValue
need NoOfDivs,
Rate- continuous
Dividents
will change stock price

Divs[]; zero based
*/

double CalculateOption(	
						double	R,
						double	RF,
						double	S,
						double	K,
						double	V,
						int		Dte, 
                        bool	Call,
						bool	American,
						double* DivAmnt,
						double* DivYte,
						int		DivCount,
                        long    nSteps,
                        double* pSpotAjustDiv
						)
{
	if (Dte < 1) 
		return max(Call ? S - K : K - S, 0.);

	if (DivCount > 0)
		RF = 0.;

	DivCount = min(DivCount, 30);

	double	daDivAmnt[30];						// Dividends amount on continios rate 
	double	dSWOdiv	= S;						// Stock price w/o dividends
	double	dYte	= Dte / 365.0;				// Expiration date in years
	double	dRx		= 0;						// Continios short rate
	double	dRFx	= 0;						// Continios long rate

	memset( daDivAmnt, 0, 30 * sizeof(double) );
	
	dRx		= RateToCnt(R, Dte);
	dRFx	= RateToCnt(RF, Dte);
		
	for (int j = 0; j < DivCount; j++) 
	{
		bool	bGoodDiv = (DivYte[j] >= 0. && DivYte[j] <= dYte && DivAmnt[j] > 0.);
		
		if (bGoodDiv) 
		{
			daDivAmnt[j] = DivAmnt[j] * exp(-dRx * DivYte[j]);
			
			if ((dSWOdiv -= daDivAmnt[j]) <= 0)
				return ERR_VALUE;
       	}
	}
    
	double	dResultBS = OP_Black(dRx, dSWOdiv, K, V, dYte, dRFx, Call);
    if (pSpotAjustDiv) 
        *pSpotAjustDiv = dSWOdiv;
	
	return max(dResultBS, 0.);
} 

/*******************************************************************************
** CalculateOption_CRR														  **
********************************************************************************/
/*
process dividents before calling OP_CoxRub
need NoOfDivs,
Rate- continuous
Dividents
will change stock price

Divs[]; zero based
*/

double CalculateOption_CRR(	double	R,
						    double	RF,
						    double	S,
						    double	K,
						    double	V,
						    int		Dte, 
                            bool	Call,
						    bool	American,
						    double* DivAmnt,
						    double* DivYte,
						    int		DivCount,
						    long	Steps,
                            C_QMATRIX* pQMatrix_Up,
                            C_QMATRIX* pQMatrix_Down,
                            double*    pSpotAjustDiv,
                            long*      pnModel)
{
	if (Dte < 1)
		return max(Call ? S - K : K - S, 0.);

	if (DivCount > 0)
		RF = 0.;

	DivCount = min(DivCount, 30);

	double	daDivAmnt[30];						// Dividends amount on continios rate 
	double	dSWOdiv	= S;						// Stock price w/o dividends
	double	dYte	= Dte / 365.0;				// Expiration date in years
	double	dRx		= 0;						// Continios short rate
	double	dRFx	= 0;						// Continios long rate

	memset( daDivAmnt, 0, 30 * sizeof(double) );
	
	dRx		= RateToCnt(R, Dte);
	dRFx	= RateToCnt(RF, Dte);
		
	for (int j = 0; j < DivCount; j++) 
	{
		bool	bGoodDiv = (DivYte[j] >= 0. && DivYte[j] <= dYte && DivAmnt[j] > 0.);
		
		if (bGoodDiv) 
		{
			daDivAmnt[j] = DivAmnt[j] * exp(-dRx * DivYte[j]);
			
			if ((dSWOdiv -= daDivAmnt[j]) <= 0)
				return ERR_VALUE;
       	}
	}
    
	double	dResultBS = OP_Black(dRx, dSWOdiv, K, V, dYte, dRFx, Call);
	
	if(!American ||
	   (American && Call && DivCount == 0)
	  )
    {
        if (pnModel) *pnModel = MM_EGAR_BS;
        if (pSpotAjustDiv) *pSpotAjustDiv = dSWOdiv;
		return dResultBS;
    }
	
	double dResultCR = CRValue(dRx, S, K, V, dYte, dRFx, Call, American, Steps, DivCount, DivYte, daDivAmnt, pQMatrix_Up, pQMatrix_Down, pSpotAjustDiv);

    if (dResultBS > dResultCR)
    {
        if (pnModel) *pnModel = MM_EGAR_BS;
        if (pSpotAjustDiv) *pSpotAjustDiv = dSWOdiv;
        return max(dResultBS, 0.);
    }
    else
    {
        if (pnModel) *pnModel = MM_EGAR_BINOMIAL;
        return max(dResultCR, 0.);
    }
} 

/*******************************************************************************
** lp - Laplace function																		**
********************************************************************************/
double lp(double x) 
{
	double z, p, b1, b2, b3, b4, b5, tt;

	if (x > 30) 
		return 1;
	if (x < -30)
		return 0;

	if (x >= 0) {
		z = exp(-x * x * 0.5) / sqrt(8 * atan(1));
		p = 0.2316419;
		b1 = 0.31938153;
		b2 = -0.356563782;
		b3 = 1.781477937;
		b4 = -1.821255978;
		b5 = 1.330274429;
		tt = 1.0 / (1.0 + p * x);
		return 1 - z * (
			b1 * tt + 
			b2 * tt * tt + 
			b3 * tt * tt * tt + 
			b4 * tt * tt * tt * tt + 
			b5 * tt * tt * tt * tt * tt);
	}
	else
		return 1 - lp(-x);
}

/*******************************************************************************
** nordis																							**
********************************************************************************/
double nordis(double s) 
{
	//nordis is a function density of normalized gauss distribution
	return 1 / sqrt(atan(1) * 8) * exp(-0.5 * s * s);
}

double nordis1(double s) 
{
	return -s / sqrt(atan(1) * 8) * exp(-0.5 * s * s);
}

double nordis2(double s) 
{
	return (s*s-1) / sqrt(atan(1) * 8) * exp(-0.5 * s * s);
}

/*******************************************************************************
** nordisgen																						**
********************************************************************************/
double nordisgen(double s, double mu, double sigma) 
{
	//nordis is a function density of  gauss distribution
	return 1 / sigma * nordis((s - mu) / sigma);
}

double nordis1gen(double s, double mu, double sigma) 
{
	//nordis1gen is a derivative of function density of   distribution
	return 1 / (sigma * sigma) * nordis1((s - mu) / sigma);
}

double nordis2gen(double s, double mu, double sigma) 
{
	//nordis2 is a second derivative of function density of  gauss  distribution
	return 1 / (sigma * sigma * sigma) * nordis2((s - mu) / sigma);
}

/*******************************************************************************
** i3 and i4																						**
********************************************************************************/
double i3_1(double s, double k, double sigma, double rd, double rf, double t) 
{
	double mu, sigmat;
	mu = log(s) + (rd - rf - 0.5 * sigma * sigma) * t;
	sigmat = sigma * sqrt(t);

	return - k * exp(-rd * t) * (lp(-(log(k) - mu) / sigmat) +
			nordisgen(log(k), mu, sigmat) - nordis1gen(log(k), mu, sigmat));
}

double i4_1(double s, double k, double sigma, double rd, double rf, double t) 
{
	double mu, sigmat;
	mu = log(s) + (rd - rf - 0.5 * sigma * sigma) * t;
	sigmat = sigma * sqrt(t);

	return -i3_1(s, k, sigma, rd, rf, t) + k * exp(-rd * t) * nordis2gen(log(k), mu, sigmat);
}

/*******************************************************************************
** VSKLog model																					**
********************************************************************************/
double adjustVSKLog(double s, double k, double rd, double rf, 
				  double sigma, double t, double lam3, double lam4,
				  double OptVal)
//lam3=3 moment
//lam4=4 moment-3
{
	double tmp=sigma * sqrt(t);
	double tmp3=tmp*tmp*tmp;

	return OptVal - 
		lam3 * tmp3 / 6.0 * (-OptVal + i3_1(s, k, sigma, rd, rf, t)) + 
		lam4 * tmp3 * tmp / 24.0 * (OptVal + i4_1(s, k, sigma, rd, rf, t));
}

/*******************************************************************************
** VSK model																						**
********************************************************************************/
double sigmaVSK( double rd, double rf, double k2) 
{
	double r = rd - rf;
	double k1 = exp(r / 365);//t=1/365
	return sqrt(365 * log((k2 / (k1 * k1) ) + 1)); //t=1/365
}

double adjustVSK( double S0, double k, double rd, double rf, double t,
				double k2, double k3, double k4, double OptVal)
{
	//k2,k3,k4-cumulants of stock price
	double k1, sigma, d1, d2, /*ca,*/ ak, dak, d2ak, gam1f;
	double gam2f, gam1a, gam2a, lam1, lam2, q, q3, q4;
	double pi, r, sigmat;
	r = rd - rf;
	pi = 4 * atan(1);
	k1 = /*S0 * */exp(r / 365);//t=1/365
	sigma = sigmaVSK(rd,rf,k2); //sqrt(365 * log((k2 / (k1 * k1) ) + 1)); //t=1/365
	sigmat = sigma * sqrt(t);
	d1 = (log(S0 / k) + (r + sigma * sigma * 0.5) * t) / sigmat;
	d2 = d1 - sigmat;
	ak = 1 / (k * sigma * sqrt(2 * pi * t)) * exp(-(d2 * d2) * 0.5);
	dak = ak * (d2 - sigmat) / k / sigmat;
	d2ak = ak / ((k * sigmat)*(k * sigmat)) * ((d2 - sigmat)*(d2 - sigmat) - sigmat * (d2 - sigmat) - 1);
	q = sqrt(exp(sigma * sigma / 365) - 1);//sigmat=sigma*sqr(1/365)
	gam1a = 3 * q + q * q * q;
	gam2a = 16 * q * q + 15 * q * q * q *q + 6 * q * q * q * q * q * q + q * q * q * q * q * q * q * q;
	gam1f = k3 / exp(1.5 * log(k2));//(k2 ^ (3 / 2))
	gam2f = k4 / (k2 * k2);
	lam1 = gam1f - gam1a;
	lam2 = gam2f - gam2a;
	q3 = -(k1 * k1 * k1) * q * q * q * exp(-r * t) / 6 * dak;
	q4 = k1 * k1 * k1 * k1 * q * q * q * q * exp(-r * t) / 24 * d2ak;
	return  OptVal+lam1 * q3 + lam2 * q4; //+ca
}

