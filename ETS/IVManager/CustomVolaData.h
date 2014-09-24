#ifndef __CUSTOM_VOLA_DATA_H__
#define __CUSTOM_VOLA_DATA_H__

#pragma warning(disable: 4786)

/////////////////////////////////////////////////////////////////////////////////////
// Custom strike skew data structures
/////////////////////////////////////////////////////////////////////////////////////

// Strike skew point key
class CCustomStrikePoint
{
public:
	double	 m_dStrike;
	double	 m_dVola;
	EOptType m_enOptType;
	bool	 m_bATM;
	bool	 m_bIsBasePoint;
	long	 m_nBasePointNumber;

	CCustomStrikePoint() 
	{
		ZeroMemory( this, sizeof(CCustomStrikePoint) );
		m_nBasePointNumber = -1;
	}
};


inline bool operator<( const CCustomStrikePoint& pt1, const CCustomStrikePoint& pt2 )
{
	return pt1.m_dStrike < pt2.m_dStrike;
} 


// Vector of particular user defined strike skew points.
class CStrikeSkewValues: public std::vector<CCustomStrikePoint>
{
};


/////////////////////////////////////////////////////////////////////////////////////
// Custom time skew data structures
/////////////////////////////////////////////////////////////////////////////////////

// Time skew point key
class CCustomTimePoint
{
public:
	DATE	 m_dtMonth;
	DATE	 m_dtExpDate;
	double   m_dStrike;
	double	 m_dVola;
	long	 m_nIndex;
};


inline bool operator<( const CCustomTimePoint& pt1, const CCustomTimePoint& pt2 )
{
	return pt1.m_dtMonth < pt2.m_dtMonth;
} 


// Map for user defined time skew points.
class CTimeSkewValues: public std::vector<CCustomTimePoint>
{
};

#endif //__CUSTOM_VOLA_DATA_H__