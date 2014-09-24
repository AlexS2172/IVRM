// InterpolationEngine.h: interface for the CInterpolationEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERPOLATIONENGINE_H__EF1F0211_9EE0_4DDC_9449_D4DD9A2B0ABD__INCLUDED_)
#define AFX_INTERPOLATIONENGINE_H__EF1F0211_9EE0_4DDC_9449_D4DD9A2B0ABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MIN_SPLINE_POINTS 3

class CInterpolationException
{
};

#ifndef _INC_MATH
#error <math.h> must be included
#endif
#ifndef _VECTOR_
#error <vector> must be included
#endif
#ifndef _ALGORITHM_
#error <algorithm> must be included
#endif
#ifndef _FUNCTIONAL_
#error <functional> must be included
#endif

class CInterpolationEngine  
{
public:
// Type definitions
	struct Point
	{
		double x;
		double f;

		Point( double x, double f ): x(x), f(f) {}

		bool operator<( const Point& pt ) const
		{
			return x < pt.x;
		}

		bool operator<=( const Point& pt ) const
		{
			return x <= pt.x;
		}
	};

	typedef std::vector<Point>  PointsVector;
	typedef std::vector<double>	DoubleVector;

// Properties
private:
	double m_dIFMin;
	double m_dIFMax;
	double m_dIFCur;
	double m_dIFVal;

	double m_dr;
	double m_dR;

	PointsVector m_pts;

	DoubleVector m_h;
	DoubleVector m_A;
	DoubleVector m_B;
	DoubleVector m_C;
	DoubleVector m_D;

// Construction/Destruction
public:
	 CInterpolationEngine();
	~CInterpolationEngine();

// Interface
public:
	// IF min poisition
	double GetIFMin( void )	const { return m_dIFMin; }
	void   SetIFMin( double dVal )	
	{ 
		m_dIFMin = dVal; 

		CalculateIFVal();
	}

	// IF max poisition
	double GetIFMax( void )	const { return m_dIFMin; }
	void   SetIFMax( double dVal )	
	{ 
		m_dIFMax = dVal; 

		CalculateIFVal();
	}

	// IF poisition
	double GetIFCur( void )	const { return m_dIFMin; }
	void   SetIFCur( double dVal )	
	{ 
		m_dIFCur = dVal; 

		CalculateIFVal();
	}

	// Returns points vector
	PointsVector& GetPointVector()	{ return m_pts;	}

	// Returns interpolation factors vectors
	DoubleVector& GetAVector()	{ return m_A; }
	DoubleVector& GetBVector()	{ return m_B; }
	DoubleVector& GetCVector()	{ return m_C; }
	DoubleVector& GetDVector()	{ return m_D; }

	// Recalculates interpolation factors
	void RecalculateVector() throw(CInterpolationException);

	// Returns interpolated value
	double GetInterpolatedValue( double y ) const throw(CInterpolationException);

// Implementation
protected:

	// Calculates slider value and contomotant factors
	void CalculateIFVal() throw(CInterpolationException)
	{
		if( m_dIFMin != m_dIFMax )
			m_dIFVal = ( m_dIFCur - m_dIFMin )/( m_dIFMax - m_dIFMin );
		else
			throw CInterpolationException();

		m_dr = 1.25 * ( pow( (double) 81, (double)m_dIFVal ) - 1 );
		m_dR = ( 3 + 3 * m_dr + m_dr * m_dr )/( 3 + 4 * m_dr + m_dr * m_dr );
	}

	// Performs linear interpolation for two points
	double LinearInterpolation( double y ) const throw(CInterpolationException);

	// Performs spline interpolation for more than two points
	double SplineInterpolation( double y ) const throw(CInterpolationException);
};

#endif // !defined(AFX_INTERPOLATIONENGINE_H__EF1F0211_9EE0_4DDC_9449_D4DD9A2B0ABD__INCLUDED_)
