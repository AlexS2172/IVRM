#ifndef __BLACK_AND_SCHOLES_H__
#define __BLACK_AND_SCHOLES_H__

namespace OPM
{

double BlackAndScholes(	double	Rd,
						double	Rf,
						double	S,
						double	K,
						double	T,
						double	V,
						bool	IsCall);

}

#endif	__BLACK_AND_SCHOLES_H__