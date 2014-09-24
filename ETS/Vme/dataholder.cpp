#include "stdafx.h"
#include "DataHolder.h"
#include "OptionCalc\optioncalc.h"


/////////////////////////////////////////////////////////////////////////////
// 
// CDataHolder implementation
//
/////////////////////////////////////////////////////////////////////////////

void CDataHolder::CopyTo(CDataHolder* pDest)
{
	CObjectLock(this);
	if (pDest)
	{
		pDest->m_bDiscreteAcceleration	=	m_bDiscreteAcceleration;
		pDest->m_bPriceOverride		= m_bPriceOverride;
		pDest->m_dSmileAccelerator	= m_dSmileAccelerator;
		pDest->m_dUnderlinePrice	= m_dUnderlinePrice;
		pDest->m_dInterpolationFactor = m_dInterpolationFactor;
		pDest->m_vola = m_vola;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CDataHolder::SetUnderlinePrice( double dUP ) 
{ 
	CObjectLock(this);

	if( m_dUnderlinePrice == dUP )
		return;
	
	double dStrikeDelta = 0.0f;
	if( !m_bDiscreteAcceleration )
		dStrikeDelta = ( m_dUnderlinePrice - dUP );

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		if( !m_bPriceOverride /*&& it->second.m_bBasePointsInterpolation*/ )
			continue;

		try
		{	
			if( m_bDiscreteAcceleration )
				dStrikeDelta = it->second.GetATMPoint( m_dUnderlinePrice, false ).GetStrike() -
							   it->second.GetATMPoint( dUP, false ).GetStrike();
		}
		catch( ... )
		{
			continue;
		}

		if( m_dUnderlinePrice != -1.0f )
		{
			it->second.SetStrikeDelta( dStrikeDelta * m_dSmileAccelerator );
			it->second.m_bInterpolationVectorUpdated = false;
			it->second.m_dwUpdateTime = GetTickCount();
		}
	}

	m_dUnderlinePrice = dUP;  
}


double CDataHolder::GetUnderlinePrice( void )	   
{ 
	CObjectLock(this);

	return m_dUnderlinePrice; 
}


void CDataHolder::SetSmileAccelerator( double dSF ) 
{ 
	CObjectLock(this);

	m_dSmileAccelerator	= dSF;  

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		it->second.m_bInterpolationVectorUpdated = false;
		it->second.m_dwUpdateTime = GetTickCount();
	}
}


double CDataHolder::GetSmileAccelerator( void ) 
{ 
	CObjectLock(this);

	return m_dSmileAccelerator; 
}


void CDataHolder::SetInterpolationFactor( double dIF ) 
{ 
	CObjectLock(this);

	m_dInterpolationFactor = dIF;  

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		it->second.m_ie.SetIFCur( dIF );
		it->second.m_bInterpolationVectorUpdated = false;
		it->second.m_dwUpdateTime = GetTickCount();
	}
}


double CDataHolder::GetInterpolationFactor( void ) 
{ 
	CObjectLock(this);

	return m_dInterpolationFactor; 
}

	
void CDataHolder::SetDiscreteAcceleration( bool bDA )
{
	CObjectLock(this);

	m_bDiscreteAcceleration = bDA;  

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		it->second.m_bInterpolationVectorUpdated = false;
		it->second.m_dwUpdateTime = GetTickCount();
	}
}


bool CDataHolder::GetDiscreteAcceleration( void ) 
{ 
	CObjectLock(this);

	return m_bDiscreteAcceleration; 
}

	
void CDataHolder::SetPriceOverride( bool bPO )
{
	CObjectLock(this);

	m_bPriceOverride = bPO;  

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		it->second.m_bInterpolationVectorUpdated = false;
		it->second.m_dwUpdateTime = GetTickCount();
	}
}

	
bool CDataHolder::GetPriceOverride( void ) 
{ 
	CObjectLock(this);

	return m_bPriceOverride; 
}


// Clears all data
void CDataHolder::ClearAll()
{
	CObjectLock(this);

	m_dUnderlinePrice		= -1.0f;
	m_dSmileAccelerator		= 0.0f;	   
	m_dInterpolationFactor	= 1.0f;   
	m_bDiscreteAcceleration = false;
	m_bPriceOverride		= false;

	m_vola.clear();
}


// Clears particular skew data
void CDataHolder::ClearSkew( DATE dtExpDate )
{
	CObjectLock(this);

	m_vola.erase( ExpDate(dtExpDate) );
}


DWORD CDataHolder::GetSkewUpdateMark( DATE dtExpDate )
{
	CObjectLock(this);

	return m_vola.GetSkew(ExpDate(dtExpDate), m_dInterpolationFactor ).m_dwUpdateTime;
}


// Sets surface point. Primary used for data load
void CDataHolder::SetPoint( long nID, double dStrike, 
						    const _variant_t& vtExpDate, const _variant_t& vtVola, const _variant_t& vtIsBasePoint )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew( ExpDate(vtExpDate.date), m_dInterpolationFactor );

	CPoint pt;
	skew.InitPoint( pt, nID,dStrike, vtVola, vtIsBasePoint);													

	skew.PutPoint( pt, NULL );
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}


// Imports surface point update.
void CDataHolder::ImportPoint( long nID, double dStrike, const _variant_t& vtExpDate, const _variant_t& vtVola, const _variant_t& vtIsBasePoint, const _variant_t& vtStatus )
{
	CObjectLock(this);

	try
	{
		CSkew& skew = m_vola.GetSkew( ExpDate(vtExpDate.date), m_dInterpolationFactor );

		EVolaItemStatus enStatus = (EVolaItemStatus)vtStatus.lVal;
		switch( enStatus )
		{
			case Deleted:
				skew.RemovePoint(dStrike);
				//skew.GetPoint( dStrike ).enStatus = Deleted;
				break;
			case Changed:
			case Created:
			case Untouched:
			{
				CPoint pt;
				skew.InitPoint( pt, nID,dStrike, vtVola, vtIsBasePoint);
				skew.PutPoint( pt, NULL );
				break;
			}
		}
	}
	catch( const _com_error& )
	{
	}
}


// Returns number of holded expiration dates
long CDataHolder::GetExpirationsCount()
{
	CObjectLock(this);

	return m_vola.size();
}


// Returns expiration date
DATE CDataHolder::GetExpirationDate( long index )
{
	CObjectLock(this);

	return m_vola.GetExpirationDate( index );
}


// Returns volatility value for particular time skew point
double CDataHolder::GetTimeSkewVolatility( DATE dtExpDate )
{
	CObjectLock(this);

	try
	{
		return GetATMVola( dtExpDate );
	}
	catch( _com_error )
	{
		return 0;
	}
}


// Returns strike value for particular time skew point
double CDataHolder::GetTimeSkewStrike( DATE dtExpDate )
{
	CObjectLock(this);

	try
	{
		return GetATMStrike( dtExpDate );
	}
	catch( _com_error )
	{
		return 0;
	}
}


// Adjusts time skew volatility with provided delta
void CDataHolder::SetTimeSkewVolatilityDelta( DATE dtExpDate, double dVolaDelta )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew( ExpDate(dtExpDate), m_dInterpolationFactor );

	skew.SetVolaDelta( dVolaDelta );
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}


// Set time skew volatility with provided value
void CDataHolder::SetTimeSkewVolatility( DATE dtExpDate, double dVola )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew( ExpDate(dtExpDate), m_dInterpolationFactor );

	skew.SetVola( dVola );
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}

// Set all time skew volatility with provided value
void CDataHolder::SetTimeSkewVolatilityAll( double dVola )
{
	CObjectLock(this);

	for(CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++)
	{
		CSkew& skew = it->second;

		skew.SetVola( dVola );
		skew.m_bInterpolationVectorUpdated = false;
		skew.m_dwUpdateTime = GetTickCount();
	}
}


// Return number of strike points for particular skew mode
long CDataHolder::GetPointsCount( DATE dtExpDate )
{
	CObjectLock(this);

	return m_vola.GetSkew( ExpDate(dtExpDate), m_dInterpolationFactor ).size();
}


// Returns BasePointsInterpolation property value
bool CDataHolder::GetBasePointsInterpolation( DATE dtExpDate )
{
	CObjectLock(this);

	return m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).m_bBasePointsInterpolation;
}


// Sets BasePointsInterpolation property value
void CDataHolder::SetBasePointsInterpolation( DATE dtExpDate, bool bValue )
{
	CObjectLock(this);

	m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).SetBasePointsInterpolation( bValue );
}

	
// Returns ID of particular point
CDataHolder::PointID CDataHolder::GetPointID( DATE dtExpDate, long nIndex )
{
	CObjectLock(this);

	return m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( nIndex ).GetPointID();
}


// Returns strike value of particular point
double CDataHolder::GetPointStrike( DATE dtExpDate, long nIndex )
{
	CObjectLock(this);

	return m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( nIndex ).GetStrike();
}


// Returns status of particular point
EVolaItemStatus CDataHolder::GetPointStatus( DATE dtExpDate, double Strike )
{
	CObjectLock(this);

	return m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( Strike ).GetStatus();
}


// Returns status of particular point
EVolaItemStatus CDataHolder::GetPointStatus( DATE dtExpDate, long nIndex )
{
	CObjectLock(this);

	return m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( nIndex ).GetStatus();
}


// Returns base points count
long CDataHolder::GetBasePointsCount( DATE dtExpDate )
{
	CObjectLock(this);

	return (long)m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetBasePointsCount();
}

	
// Returns base point status of particular point
bool CDataHolder::GetIsBasePoint( DATE dtExpDate, double Strike )
{
	CObjectLock(this);

	const CDataHolder::CPoint& pt = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( Strike );

	return pt.GetStatus() != Deleted && pt.GetIsBase();
}


// Returns base point status of particular point
bool CDataHolder::GetIsBasePoint( DATE dtExpDate, long nIndex )
{
	CObjectLock(this);

	const CDataHolder::CPoint& pt = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( nIndex );

	if( pt.GetStatus() == Deleted )
		return false;

	return pt.GetIsBase();
}


// Sets base point status of particular point
void CDataHolder::SetIsBasePoint( DATE dtExpDate, double Strike, bool bIsBasePoint )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	skew.GetPoint( Strike ).SetIsBase(bIsBasePoint);
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}

double	CDataHolder::ExpirationInterpolate( DATE dtExpDate, double dStrike, double UnderlyingPrice ){
	double dV1 = 0., dV2 = 0., dV = 0., dVol = 0.0;

	// get neighbors
	CVolaValuesMap::iterator itPrev = m_vola.lower_bound( ExpDate(dtExpDate) ); 
	CVolaValuesMap::iterator itEnd = m_vola.end();
	CVolaValuesMap::reverse_iterator ritPrev( itPrev );
	CVolaValuesMap::reverse_iterator ritEnd = m_vola.rend();
	CVolaValuesMap::iterator itNext = m_vola.upper_bound( ExpDate(dtExpDate) );
	
	/*CVolaValuesMap::iterator itPos;
	boudary_find<ExpDate, CSkew>(m_vola, itPrev, itNext, itPos, ExpDate(dtExpDate));
	CVolaValuesMap::reverse_iterator ritPrev( itPrev );
	CVolaValuesMap::reverse_iterator ritEnd = m_vola.rend();
	CVolaValuesMap::iterator itEnd = m_vola.end();*/

	// find previous useful expiration
	bool bGoodSkew = false;
	if ( itPrev != itEnd ) {
		while( ritPrev != ritEnd ){
			CSkew& skwP = ritPrev->second;
			if ( !skwP.GetUsefulPtCount() )
				ritPrev++;
			else
				break;
		}
	}

	// find next useful expiration
	if ( itNext != itEnd ) {
		while( itNext != itEnd ){
			CSkew& skwN = itNext->second;
			if ( !skwN.GetUsefulPtCount()  )
				itNext++;
			else
				break;
		}
	}

	if ( ritPrev != ritEnd && itNext != itEnd ) {
		// if both neighbors exist
		CSkew& skw1 = ritPrev->second;
		CSkew& skw2 = itNext->second;

		// get volatility for previous expiration with requested strike
		// ( may cause interpolation )
		double dStrikeDelta = 0.0f;
		if( m_dUnderlinePrice != UnderlyingPrice || m_dSmileAccelerator == 0.0 ) {
			if( !m_bDiscreteAcceleration )
				dStrikeDelta = ( m_dUnderlinePrice - UnderlyingPrice);
			else
				dStrikeDelta = skw1.GetATMPoint( UnderlyingPrice, false ).GetStrike() -
				skw1.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
		}

		dV1 = skw1.GetPointVolatility( dStrike + dStrikeDelta * m_dSmileAccelerator );
		dV1 = dV1 > 0 ? dV1 : 0.0f;

		// get volatility for next expiration with requested strike
		// ( may cause interpolation )
		if( m_dUnderlinePrice != UnderlyingPrice || m_dSmileAccelerator == 0.0 ) {
			if( !m_bDiscreteAcceleration )
				dStrikeDelta = ( m_dUnderlinePrice - UnderlyingPrice);
			else
				dStrikeDelta = skw2.GetATMPoint( UnderlyingPrice, false ).GetStrike() -
							skw2.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
		}

		dV2 = skw2.GetPointVolatility( dStrike + dStrikeDelta * m_dSmileAccelerator );
		dV2 = dV2 > 0 ? dV2 : 0.0f;

		double dtNYNow;
		::GetNYDateTimeAsDATE(&dtNYNow);
		// interpolate
		/*double dE = static_cast<double>(static_cast<long>(itNext->first) - static_cast<long>(ritPrev->first));
		double dE1 = static_cast<double>(static_cast<long>(dtExpDate) - static_cast<long>(ritPrev->first));*/

		double dE  = sqrt(itNext->first.date_ - dtNYNow) - sqrt(ritPrev->first.date_ - dtNYNow);
		double dE1 = sqrt(dtExpDate - dtNYNow) - sqrt(ritPrev->first.date_ - dtNYNow);

		if ( dE1 == 0.0 ) 
			dE1 = 1.0;
		dV = (dV2 - dV1);
		dVol = dV1 + dV*dE1/dE;
		dVol = dVol > 0 ? dVol : 0.0f;
	}
	else if ( ritPrev != ritEnd ) {
		// if only previous expiration exists
		// get volatility for previous expiration with requested strike
		// ( may cause interpolation )

		CSkew& skw1 = ritPrev->second;
		double dStrikeDelta = 0.0f;
		if( m_dUnderlinePrice != UnderlyingPrice || m_dSmileAccelerator == 0.0 ) {
			if( !m_bDiscreteAcceleration )
				dStrikeDelta = ( m_dUnderlinePrice - UnderlyingPrice);
			else
				dStrikeDelta = skw1.GetATMPoint( UnderlyingPrice, false ).GetStrike() -
				skw1.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
		}
		// return volatility equal to volatility of previous expiration with requested strike
		dV1 = skw1.GetPointVolatility( dStrike + dStrikeDelta * m_dSmileAccelerator );
		dVol = dV1 > 0 ? dV1 : 0.0f;
	}
	else{
		// if there are not any volatility points
		// return 20 as default
		if ( ritPrev == ritEnd && itNext == itEnd ) 
			return 20.;

		// if only next expiration exists
		// get volatility for next expiration with requested strike
		// ( may cause interpolation )
		CSkew& skw1 = itNext->second;
		double dStrikeDelta = 0.0f;
		if( m_dUnderlinePrice != UnderlyingPrice || m_dSmileAccelerator == 0.0 ) {
			if( !m_bDiscreteAcceleration )
				dStrikeDelta = ( m_dUnderlinePrice - UnderlyingPrice);
			else
				dStrikeDelta = skw1.GetATMPoint( UnderlyingPrice, false ).GetStrike() -
				skw1.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
		}

		// return volatility equal to volatility of next expiration with requested strike
		dV1 = skw1.GetPointVolatility( dStrike + dStrikeDelta * m_dSmileAccelerator );
		dVol = dV1 > 0 ? dV1 : 0.0f;
	}

	return dVol;
}

// Returns volatility value for particular strike and underline price
double CDataHolder::GetPointVolatility( DATE dtExpDate, double Strike, double UnderlinePrice)
{
	CObjectLock(this);
	
	double	dVola = 0.;

	// this part is for further realization of vola interpolation for fop
	//if (m_vola.find(ExpDate(dtExpDate)) == m_vola.end())
	//{
	//	// no volatilities for this expiration
	//	// perform interpolation between expirations
	//	dVola = ExpirationInterpolate( dtExpDate, Strike, UnderlinePrice);
	//	if( (dtExpDate > 32874) && ( Strike>(0.1)) ) 
	//		AddPoint(dtExpDate, Strike, dVola, false, true);
	//	return dVola;
	//}
	//else
	//{
	//	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );
	//	double dStrikeDelta = 0.0f;
	//	if( m_dUnderlinePrice != UnderlinePrice || m_dSmileAccelerator == 0.0 )
	//	{
	//		if( !m_bDiscreteAcceleration )
	//			dStrikeDelta = ( m_dUnderlinePrice - UnderlinePrice);
	//		else
	//			dStrikeDelta = skew.GetATMPoint( UnderlinePrice, false ).GetStrike() -
	//			skew.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
	//	}

	//	dVola = skew.GetPointVolatility( Strike + dStrikeDelta * m_dSmileAccelerator );
	//	dVola = dVola > 0 ? dVola : 0.0f;
	//}

	CSkew& skew = m_vola.GetSkew(ExpDate(dtExpDate), m_dInterpolationFactor);

	if (!skew.GetBasePointsCount()) 
	{
		// no volatilities for this expiration
		// perform interpolation between expirations
		dVola = ExpirationInterpolate( dtExpDate, Strike, UnderlinePrice);
		
		//if(Strike > 0.1) 
		//	AddPoint(dtExpDate, Strike, dVola, false, true);
		
		return dVola;
	}

	double dStrikeDelta = 0.0f;
	
	if (UnderlinePrice > 0 && (m_dUnderlinePrice != UnderlinePrice || m_dSmileAccelerator == 0.0))
	{
		if( !m_bDiscreteAcceleration )
			dStrikeDelta = ( m_dUnderlinePrice - UnderlinePrice);
		else
			dStrikeDelta = skew.GetATMPoint( UnderlinePrice, false ).GetStrike() -
						   skew.GetATMPoint( m_dUnderlinePrice, false ).GetStrike();
	}

	//skew.SetBasePointsInterpolation(true);
	dVola = skew.GetPointVolatility( Strike + dStrikeDelta * m_dSmileAccelerator );
	dVola = dVola > 0 ? dVola : 0.0f;

	return dVola;
}


// Returns volatility value for particular strike
/*double CDataHolder::GetPointVolatility( DATE dtExpDate, double Strike )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	double dVola = skew.GetPointVolatility( Strike );
	dVola = dVola > 0 ? dVola : 0.0f;

	try
	{
		skew.GetPoint( Strike, false ).SetVola(dVola);
	}
	catch( const _com_error& )
	{
	}

	return dVola;
}  */


// Returns volatility value of particular point
double CDataHolder::GetPointVolatility( DATE dtExpDate, long nIndex )
{
	CObjectLock(this);

	double dStrike = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).GetPoint( nIndex ).GetStrike();

	return GetPointVolatility( dtExpDate, dStrike );
}


// Sets point volatility
void CDataHolder::SetPointVolatility( DATE dtExpDate, double Strike, double dVola )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	skew.GetPoint( Strike).SetVola( dVola );
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}

// Shifts all skew proportional selected point shift
void CDataHolder::ProportionalShift( DATE dtExpDate, double Strike, double dVola )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	try
	{
		double dRatio = dVola/skew.GetPoint( Strike, false ).GetVola();

		skew.ProportionalShift( dRatio );
		skew.m_bInterpolationVectorUpdated = false;
		skew.m_dwUpdateTime = GetTickCount();
	}
	catch( const _com_error& )
	{
	}
}


// Adds point to skew
long CDataHolder::AddPoint( DATE dtExpDate, double dStrike, double dVola, bool bIsBasePoint, bool bForceUntouched )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	CPoint pt;
	skew.InitPoint( pt, dStrike, dVola, bIsBasePoint);
	//pt.nID			= -1;
	//pt.dStrike		= dStrike;
	//pt.dVola		= dVola;
	//pt.enStatus		= Created;
	//pt.bIsBasePoint = bIsBasePoint;

	long index = 0;
	skew.PutPoint( pt, &index, bForceUntouched );

	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
	
	return index;
}


// Removes point to skew
void CDataHolder::RemovePoint( DATE dtExpDate, double dStrike )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	skew.RemovePoint( dStrike );
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}


// Removes all points from particular skew
void CDataHolder::RemoveAllPoints( DATE dtExpDate )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	//for( long i = 0; i < skew.size(); i++ )
	//	skew.GetPoint( i ).enStatus = Deleted;
	skew.RemoveAllPoints();
	skew.m_bInterpolationVectorUpdated = false;
	skew.m_dwUpdateTime = GetTickCount();
}


// Gets ATM point strike
double CDataHolder::GetATMStrike( DATE dtExpDate )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	try
	{
		return skew.GetATMPoint( m_dUnderlinePrice ).GetStrike();
	}
	catch( const _com_error& )
	{
		return 0;
	}
}

// Gets ATM point vola
double CDataHolder::GetATMVola( DATE dtExpDate )
{
	CObjectLock(this);

	CSkew& skew = m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor );

	try
	{
		return skew.GetATMPoint( m_dUnderlinePrice ).GetVola();
	}
	catch( const _com_error& )
	{
		return 0;
	}
}


// Removes points marked as deleted and sets other points status to unchanged
void CDataHolder::UpdateSurfaceStatus()
{
	CObjectLock(this);

	for( CVolaValuesMap::iterator it = m_vola.begin(); it != m_vola.end(); it++ )
	{
		it->second.UpdateStatus();
	}
}


// Removes points marked as deleted and sets other points status to unchanged
void CDataHolder::UpdateSkewStatus( DATE dtExpDate )
{
	CObjectLock(this);

	m_vola.GetSkew(  ExpDate(dtExpDate), m_dInterpolationFactor ).UpdateStatus();
}
