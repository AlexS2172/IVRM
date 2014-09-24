#include "Common.h"

namespace OPM
{

double __stdcall NormalC(double X)
{
	double dX = X / sqrt(2.);	
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
        dY = (1. / sqrt(Pi) -  dY) / dW;
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

double NormalDensity(double X)
{
	return exp(-X*X / 2.) / sqrt(2. * OPM::Pi);
}

}