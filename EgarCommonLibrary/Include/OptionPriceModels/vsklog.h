#ifndef __VSK_LOG_H__
#define __VSK_LOG_H__

namespace OPM
{

double VSKLog(	double	S,
				double	K,
				double	Rd,
				double	Rf,
				double	V,
				double	T,
				bool	IsCall,
				double	Skew,
				double	Kurtosis);

}

#endif // __VSK_LOG_H__