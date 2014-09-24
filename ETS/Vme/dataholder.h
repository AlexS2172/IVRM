#ifndef __DATAHOLDER_H__
#define __DATAHOLDER_H__

#include "InterpolationEngine.h"
#include "..\include\DataProtector.h"
#include "boost\shared_ptr.hpp"

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
	class ExpDate{
	public:
		double date_;
	public:
		ExpDate(): date_(0.) {}
		ExpDate(const double &r): date_(r) {}
		bool operator==( const ExpDate& r) const {
			return fabs( r.date_ - date_) < (1./(24.*60.));
		}
		bool operator<( const ExpDate& r) const {
			return  !(*this==r) && ( (date_ - r.date_) < 0. );
		}
		ExpDate& operator=(const double& r)
		{
			date_	=	r;
			return *this;
		}
	};
public:
	//typedef long ExpDate;	// Represent exp date as long to insure map search ability
	typedef long PointID;

	// Strikes values
	class CSkew;
	class CPoint
	{
		PointID				nID;
		double				dStrike;
		double				dVola;
		EVolaItemStatus	enStatus;
		bool					bIsBasePoint;
	public:
		CPoint(): dStrike(0.0f), dVola(0.0f), enStatus(Untouched), bIsBasePoint(false) {}
		double	GetStrike()	const		{	return dStrike;	}
		double	GetVola()	const		{	return dVola;	}
		EVolaItemStatus	GetStatus()	const	{	return enStatus;	}
		PointID	GetPointID()	const	{	return nID;	}
		bool		GetIsBase()	const		{ return		bIsBasePoint;	}
		void		SetIsBase(bool isBase)	{	bIsBasePoint = isBase;}
		void		SetVola(double vola)		{	dVola = vola;}
		//friend CDataHolder;
		friend CSkew;
	};
	class CVolaValuesMap;
	class CSkew: std::vector<CPoint> 
	{
		CInterpolationEngine	m_ie;
		bool						m_bBasePointsInterpolation;
		bool						m_bInterpolationVectorUpdated;
		unsigned				usefulPtCount;			// CSkew will keep number of useful points
																// here
		DWORD m_dwUpdateTime;

		CSkew( double dIF ): 
			m_bBasePointsInterpolation(false), 
			m_bInterpolationVectorUpdated(false),
			usefulPtCount(0)
		{ 
			m_ie.SetIFCur( dIF ); 
			
			m_dwUpdateTime = GetTickCount();
		}

		double	GetBasePointsCount() const;

		void   SetVolaDelta( double dVolaDelta );
		void   SetVola( double dVola );
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
		unsigned GetUsefulPtCount()	{	return usefulPtCount;	}
		friend CVolaValuesMap;
		friend CDataHolder;

		void InitPoint(CPoint&	pt, long nID, double dStrike, const _variant_t& vtVola, const _variant_t& vtIsBasePoint){
			pt.nID			= nID;
			pt.dStrike		= dStrike;
			if( vtVola.vt == VT_R4 )
				pt.dVola = vtVola.fltVal;
			else if( vtVola.vt == VT_R8 )
				pt.dVola = vtVola.dblVal;
			else
				pt.dVola = -1;
			pt.bIsBasePoint = (bool)vtIsBasePoint;
		}
		void InitPoint( CPoint&	 p, double dStrike, double dVola, bool bIsBasePoint){
			p.nID			= -1;
			p.dStrike		= dStrike;
			p.dVola		= dVola;
			p.enStatus		= Created;
			p.bIsBasePoint = bIsBasePoint;
		}

		void	RemoveAllPoints(){
			for ( size_t	i = 0; i < size(); i++ ) 
				at(i).enStatus = Deleted;
			usefulPtCount = 0;
		}

		void	RemovePoint(double strike){
			GetPoint(strike).enStatus = Deleted;
			if (usefulPtCount > 0 ) usefulPtCount--;
		}
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
	double	ExpirationInterpolate( DATE dtExpDate, double dStrike, double UnderlyingPrice );
	void CopyTo(CDataHolder* pDest);
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

	// Set time skew volatility with provided value
	void SetTimeSkewVolatility( DATE dtExpDate, double dVola );

	// Set all time skew volatility with provided value
	void SetTimeSkewVolatilityAll( double dVola );

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
	double GetPointVolatility( DATE dtExpDate, double Strike, double UnderlinePrice = 0);

	// Returns volatility value for particular strike
	/*double GetPointVolatility( DATE dtExpDate, double Strike );*/

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

//--------------------------------------------------------------------//
// Synthetic Volatility Surface Support								  //
//--------------------------------------------------------------------//
class CSubSurface
{
public:
	//_bstr_t m_bsRootSymbol;
	long	m_lSurfaceID;
	long	m_lRootID;

	CSubSurface(): /*m_bsRootSymbol(""),*/ m_lSurfaceID(0), m_lRootID(0)
	{};

	CSubSurface(/*_bstr_t bsRootSymbol,*/ long lRootID, long lSurfaceID)
	{
		/*m_bsRootSymbol	= _bstr_t(bsRootSymbol);*/
		m_lSurfaceID	= lSurfaceID;
		m_lRootID		= lRootID;
	};
};
typedef std::map<long, CSubSurface> CRootSurfaceMap;
typedef CRootSurfaceMap::iterator it_srm;
typedef boost::shared_ptr<CDataHolder> CDataHolderPtr;

class CSurfaceGroup: public std::map<long, CDataHolderPtr>
{
public:

	CDataHolder& GetDataHolderBySurfaceID(long lSurfaceID)
	{
		iterator it = find(lSurfaceID);
		if (it == end())
		{
			it = insert(value_type(lSurfaceID, CDataHolderPtr(new CDataHolder()))).first;
		}
		return *(it->second);
	};

	long GetSurfaceByRoot(long lRootID)
	{
		it_srm it = m_SurfaceMap.find( lRootID );
		if (it == m_SurfaceMap.end())
			it = m_SurfaceMap.find(-1);

		return it->second.m_lSurfaceID;
	};

	CRootSurfaceMap m_SurfaceMap;

	void AddSurface(_bstr_t bsRootSymbol, long lRootID, long lSurfaceID)
	{
		it_srm it = m_SurfaceMap.find(lRootID);
		if ( it == m_SurfaceMap.end() )
			m_SurfaceMap.insert( CRootSurfaceMap::value_type(lRootID, CSubSurface(/*bsRootSymbol,*/ lRootID, lSurfaceID)) );

		GetDataHolderBySurfaceID( lSurfaceID );
	};

	void ProcessSurface(const _variant_t& vcRootSymbol, long lRootID, long nSurfaceID)
	{
		AddSurface( _bstr_t(vcRootSymbol), lRootID, nSurfaceID );
	};


	void ClearAll()
	{
		iterator it = begin();
		for(it; it != end(); it++)
		{
			it->second->ClearAll();
		}
		clear();
	};
	//friend CDataHolder;
};

#endif //__DATAHOLDER_H__
