#ifndef __DATAHOLDER_H__
#define __DATAHOLDER_H__

#include "InterpolationEngine.h"
#include "..\include\DataProtector.h"

/////////////////////////////////////////////////////////////////////////////
//
// CDataHolder class
//
// Holds all volatility surface data for particular contract and option type
// and provides interface for data access and modification
/////////////////////////////////////////////////////////////////////////////
class CDataHolder: public CDataProtector
{
// Internal types
public:
	typedef long ExpDate;	// Represent exp date as long to insure map search ability
	typedef long PointID;

	// Strikes values
	class CSkew;
	class CPoint
	{
		PointID			nID;
		double			dStrike;
		double			dVola;
		EVolaItemStatus enStatus;
		bool			bIsBasePoint;

		CPoint(): dStrike(0.0f), dVola(0.0f), enStatus(Untouched), bIsBasePoint(false) {}

		friend CDataHolder;
		friend CSkew;
	};
	class CVolaValuesMap;
	class CSkew: std::vector<CPoint> 
	{
		CInterpolationEngine m_ie;
		bool m_bBasePointsInterpolation;
		bool m_bInterpolationVectorUpdated;
		
		DWORD m_dwUpdateTime;

		CSkew( double dIF ): 
			m_bBasePointsInterpolation(false), 
			m_bInterpolationVectorUpdated(false)	
		{ 
			m_ie.SetIFCur( dIF ); 
			
			m_dwUpdateTime = GetTickCount();
		}

		double	GetBasePointsCount() const;

		void   SetVolaDelta( double dVolaDelta );
		void   ProportionalShift( double dRatio );
		void   SetStrikeDelta( double dStrikeDelta );
		double GetPointVolatility( double dStrike );
		void   SetBasePointsInterpolation( bool bValue );

		CPoint& GetPoint( long nIndex );
		CPoint& GetPoint( double dStrike, bool bSetErrorInfo = true );
		CPoint& GetATMPoint( double dUnderlinePrice, bool bSetErrorInfo = true );
		bool	PutPoint( CPoint& pt, long *pnIndex, bool bForceUntouched = false );

		// Removes points marked as deleted and sets other points status to unchanged
		void UpdateStatus();

		friend CVolaValuesMap;
		friend CDataHolder;
	};


	// Volatility values
	class CVolaValuesMap: public std::map<ExpDate, CSkew> 
	{
		DATE   GetExpirationDate( long index );
		CSkew& GetSkew( ExpDate date, double dIF );

		friend CDataHolder;
	};

// Data
private:
	double			m_dUnderlinePrice;
	double			m_dSmileAccelerator,
					m_dInterpolationFactor;
	bool			m_bDiscreteAcceleration,
					m_bPriceOverride;
	
	CVolaValuesMap m_vola;

// Construction/destruction
public:
	CDataHolder(): 
	  m_dUnderlinePrice(-1.0f),
	  m_dSmileAccelerator(0.0f),	   
	  m_dInterpolationFactor(1.0f),   
	  m_bDiscreteAcceleration(false),
	  m_bPriceOverride(false)
	{
	}

// Interface
public:

	// Underlying price accessors
	void   SetUnderlinePrice( double dUP );
	double GetUnderlinePrice( void );

	// Shift factor accessors
	void   SetSmileAccelerator( double dSF );
	double GetSmileAccelerator( void );

	void   SetInterpolationFactor( double dIF );
	double GetInterpolationFactor( void );

	void SetDiscreteAcceleration( bool bDA );
	bool GetDiscreteAcceleration( void );

	void SetPriceOverride( bool bPO );
	bool GetPriceOverride( void );

	// Clears all data
	void ClearAll();

	// Clears particular skew data
	void ClearSkew( DATE dtExpDate );

	// Returns particular skew update mark
	DWORD GetSkewUpdateMark( DATE dtExpDate );

	// Sets surface point. Primary useed for data load
	void SetPoint( long nID, double dStrike, const _variant_t& vtExpDate, const _variant_t& vtVola, const _variant_t& vtIsBasePoint );

	// Imports surface point update.
	void ImportPoint( long nID, double dStrike, const _variant_t& vtExpDate, const _variant_t& vtVola, const _variant_t& vtIsBasePoint, const _variant_t& vtStatus );

	//
	// Time skew related methods
	//

	// Returns number of holded expiration dates
	long GetExpirationsCount();

	// Returns expiration date
	DATE GetExpirationDate( long index );

	// Returns volatility value for particular time skew point
	double GetTimeSkewVolatility( DATE dtExpDate );

	// Returns strike value for particular time skew point
	double GetTimeSkewStrike( DATE dtExpDate );

	// Adjusts time skew volatility with provided delta
	void SetTimeSkewVolatilityDelta( DATE dtExpDate, double dVolaDelta );

	// 
	//	Strike skew related methods
	//

	// Return number of strike points for particular skew
	long GetPointsCount( DATE dtExpDate );

	// Returns base points count
	long GetBasePointsCount( DATE dtExpDate );

	// Returns BasePointsInterpolation property value
	bool GetBasePointsInterpolation( DATE dtExpDate );

	// Sets BasePointsInterpolation property value
	void SetBasePointsInterpolation( DATE dtExpDate, bool bValue );

	// Returns ID of particular point
	PointID GetPointID( DATE dtExpDate, long nIndex );

	// Returns strike value of particular point
	double GetPointStrike( DATE dtExpDate, long nIndex );

	// Returns status of particular point
	EVolaItemStatus GetPointStatus( DATE dtExpDate, double Strike );
	EVolaItemStatus GetPointStatus( DATE dtExpDate, long nIndex );

	// Returns base point status of particular point
	bool GetIsBasePoint( DATE dtExpDate, double Strike );
	bool GetIsBasePoint( DATE dtExpDate, long nIndex );

	// Sets base point status of particular point
	void SetIsBasePoint( DATE dtExpDate, double Strike, bool bIsBasePoint );

	// Returns volatility value for particular strike and underline price
	double GetPointVolatility( DATE dtExpDate, double Strike, double UnderlinePrice );

	// Returns volatility value for particular strike
	double GetPointVolatility( DATE dtExpDate, double Strike );

	// Returns volatility value of particular point
	double GetPointVolatility( DATE dtExpDate, long nIndex );

	// Sets point volatility
	void SetPointVolatility( DATE dtExpDate, double Strike, double dVola );

	// Shifts all skew propotional selected point shift
	void ProportionalShift( DATE dtExpDate, double Strike, double dVola );

	// Adds point to skew
	long AddPoint( DATE dtExpDate, double dStrike, double dVola, bool bIsBasePoint, bool bForceUntouched = false );

	// Removes point from skew
	void RemovePoint( DATE dtExpDate, double dStrike );

	// Removes all points from particular skew
	void RemoveAllPoints( DATE dtExpDate );

	// Gets ATM point strike
	double GetATMStrike( DATE dtExpDate );

	// Gets ATM point vola
	double GetATMVola( DATE dtExpDate );

	// Removes points marked as deleted and sets other points status to unchanged
	void UpdateSurfaceStatus();

	// Removes points marked as deleted and sets other points status to unchanged
	void UpdateSkewStatus( DATE dtExpDate );
};

#endif //__DATAHOLDER_H__