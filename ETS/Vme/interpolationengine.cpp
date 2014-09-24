// InterpolationEngine.cpp: implementation of the CInterpolationEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "InterpolationEngine.h"
#include <limits>

//////////////////////////////////////////////////////////////////////
// CInterpolationEngine Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterpolationEngine::CInterpolationEngine(): 
		m_dIFMin(0.0), 
		m_dIFMax(1.0),
		m_dIFCur(1.0), 
		m_dIFVal(0.0),
		m_dr(0.0),
		m_dR(0.0)
{
	CalculateIFVal();
}


CInterpolationEngine::~CInterpolationEngine()
{
}


//////////////////////////////////////////////////////////////////////
// CInterpolationEngine Interface
//////////////////////////////////////////////////////////////////////
void CInterpolationEngine::RecalculateVector() throw(CInterpolationException)
{
	// Sort point vector
	std::sort( m_pts.begin(), m_pts.end() );

	// Spline interpolation is used for more than 2 points
	long i, size = m_pts.size();
	if( size < MIN_SPLINE_POINTS )
		return;

	// Calculate points offsets
	m_h.resize( size );
	for( i = 0; i < size - 1; i++ )
		m_h[i] = m_pts[i+1].x - m_pts[i].x;

	// Fill auxiliary arrays
	DoubleVector lam, mu, d;
	lam.resize( size );
	mu.resize( size );
	d.resize( size );
	for( i = 1; i < size - 1; i++ )
	{
		lam[i] = m_h[i]/( m_h[i-1] + m_h[i] );
		mu[i]  = 1 - lam[i];
		d[i]   = 2 * (3 + m_dr) * (lam[i] * (m_pts[i].f - m_pts[i-1].f)/m_h[i-1] +
			     mu[i] * (m_pts[i+1].f - m_pts[i].f)/m_h[i]);
	}
	d[0] = (3 + m_dr) * (m_pts[1].f - m_pts[0].f)/m_h[0] -
		   m_h[0]/m_dR * (m_pts[0].f/m_h[1]/(m_h[1]+m_h[0]) - m_pts[1].f/m_h[1]/m_h[0] + m_pts[2].f/m_h[0]/(m_h[1] + m_h[0]));
	d[size-1] = (3 + m_dr) * (m_pts[size-1].f - m_pts[size-2].f)/m_h[size-2] + 
				m_h[size-2]/m_dR * (m_pts[size-1].f/m_h[size-3]/(m_h[size-2] + m_h[size-3]) - m_pts[size-2].f/m_h[size-2]/m_h[size-3] +
				m_pts[size-3].f/m_h[size-2]/(m_h[size-2] + m_h[size-3]));

	// Forward
	DoubleVector P, Q;
	P.resize( size );
	Q.resize( size );
	P[0] = -1/( 2 + m_dr );
	Q[0] = d[0]/( 2 + m_dr );
	for( i = 1; i < size - 1; i++ )
	{
		P[i] = -mu[i]/(lam[i] * P[i-1] + 2 + m_dr);
		Q[i] = P[i] * (2 * lam[i] * Q[i-1] - d[i]);
	}

	// Backward
	DoubleVector m;
	m.resize( size );
	m[size-1] = (d[size-1] - Q[size-2])/(2 + m_dr + P[size-2]);
	for( i = size - 2; i >= 0; i-- )
		m[i] = P[i] * m[i+1] + Q[i];

	// Factor arrays
	m_A.resize( size - 1 );
	m_B.resize( size - 1 );
	m_C.resize( size - 1 );
	m_D.resize( size - 1 );
	for( i = 0; i < size - 1; i++ )
	{
		m_C[i] = (-(3 + m_dr) * (m_pts[i+1].f - m_pts[i].f) + m_h[i] * m[i]   + (2 + m_dr) * m_h[i] * m[i+1])/(3 + 4 * m_dr + m_dr * m_dr);
		m_D[i] = ( (3 + m_dr) * (m_pts[i+1].f - m_pts[i].f) - m_h[i] * m[i+1] - (2 + m_dr) * m_h[i] * m[i])/(3 + 4 * m_dr + m_dr * m_dr);
		m_A[i] = m_pts[i+1].f - m_C[i];
		m_B[i] = m_pts[i].f - m_D[i];
	}
}


// Returns interpolated value
double CInterpolationEngine::GetInterpolatedValue( double y ) const throw(CInterpolationException)
{
	if( m_pts.size() <= 0 || 
		( m_pts.size() >= MIN_SPLINE_POINTS && m_pts.size() != m_A.size() + 1 ) )
		throw CInterpolationException();

	if( m_pts.size() == 1 )
	{
		// Flat interpolation
		return m_pts[0].f;
	}
	else 
	{
		// Linear interpolation
		return LinearInterpolation( y );
	}


//	else if( m_pts.size() == 2 )
//	{
//		// Linear interpolation
//		return LinearInterpolation( y );
//	}
//	else
//	{
//		// Spline interpolation
//		return SplineInterpolation( y );
//	}

}


//////////////////////////////////////////////////////////////////////
// CInterpolationEngine Implementation
//////////////////////////////////////////////////////////////////////

// Performs linear interpolation for two points
double CInterpolationEngine::LinearInterpolation( double y ) const throw(CInterpolationException)
{
	if( m_pts.size() < 2 )
		throw CInterpolationException();

	if( y <= m_pts.begin()->x )
		return m_pts.begin()->f;

	if( y >= (m_pts.end()-1)->x )
		return (m_pts.end()-1)->f;

	PointsVector::const_iterator itEnd ;
	PointsVector::const_iterator itBegin;

	if (m_pts.size() == 2)
	{
		itBegin = m_pts.begin();
		itEnd	= itBegin+1;
	}
	else
	{	
		PointsVector::const_iterator itFind = std::lower_bound(m_pts.begin(), m_pts.end(), CInterpolationEngine::Point( y, 0 ));

		if( itFind == m_pts.end() || itFind == m_pts.begin())
			throw CInterpolationException();

		if(fabs(itFind->x - y)<= std::numeric_limits<double>::epsilon())
			return itFind->f;

		itEnd = itFind;
		itBegin = itEnd-1;
	}

	double dP = y - itBegin->x;
	double dN = itEnd->x - itBegin->x; 

	double dChange = itEnd->f - itBegin->f;
	double dRatio  = dP/dN;

	return itBegin->f + dChange * dRatio;
}


// Performs spline interpolation for more than two points
double CInterpolationEngine::SplineInterpolation( double y ) const throw(CInterpolationException)
{
	long size = m_pts.size();

	if( size < MIN_SPLINE_POINTS )
		throw CInterpolationException();

	if( y <= m_pts[0].x )
	{
	
		return m_pts[0].f;
		/*
		Need to Correct Extrapolation Algorithm
		double ai = m_D[0] * (m_dr * m_dr + 3 * m_dr + 3)/(m_h[0] * m_h[0]);
		double bi = (-m_D[0] * (m_dr + 3) + m_A[0] - m_B[0])/m_h[0] - 2 * ai * m_pts[0].x;
		double ci = m_B[0] + m_D[0] - ai * m_pts[0].x * m_pts[0].x - bi * m_pts[0].x;
		double gi = (m_pts[0].f - m_pts[1].f)/(m_pts[0].x - m_pts[1].x);
		double hi = m_pts[0].f - gi * m_pts[0].x;

		return (ai * y * y + bi * y + ci) * (1 - m_dr/100) + (gi * y + hi) * m_dr/100;
		*/
	}
	else if( y >= m_pts[size - 1].x )
	{
		return m_pts[size - 1].f;
		/*
		Need to Correct Extrapolation Algorithm
		double af = m_C[size-2] * (m_dr * m_dr + 3 * m_dr + 3)/(m_h[size-2] * m_h[size-2]);
		double bf = (m_C[size-2] * (m_dr + 3) + m_A[size-2] - m_B[size-2])/m_h[size-2] - 2 * af * m_pts[size-1].x;
		double cf = m_A[size-2] + m_C[size-2] - af * m_pts[size-1].x * m_pts[size-1].x - bf * m_pts[size-1].x;
		double gf = (m_pts[size-2].f - m_pts[size-1].f)/(m_pts[size-2].x - m_pts[size-1].x);
		double hf = m_pts[size-2].f - gf * m_pts[size-2].x;

		return (af * y * y + bf * y + cf) * (1 - m_dr/100) + (gf * y + hf) * m_dr/100;
		*/
	}
	else
	{
		PointsVector::const_iterator it = std::find_if( m_pts.begin(), m_pts.end(), std::bind1st( std::less_equal<CInterpolationEngine::Point>(), CInterpolationEngine::Point( y, 0 ) ) );
		if( it == m_pts.end() )
			throw CInterpolationException();
		long i = it - m_pts.begin() - 1;

		double z = (y - m_pts[i].x)/m_h[i];

		return m_A[i] * z + m_B[i] * (1-z) + m_C[i] * z * z * z /(1 + m_dr * (1-z)) + m_D[i] * (1-z) * (1-z) * (1-z)/(1 + m_dr * z);
	}

	return 0;
}
