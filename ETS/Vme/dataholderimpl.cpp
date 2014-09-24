#include "stdafx.h"
#include "DataHolder.h"

#include "InterpolationEngine.h"

// CDataHolder internal classes implementation

/////////////////////////////////////////////////////////////////////////////
// CDataHolder::CVolaValuesMap class
/////////////////////////////////////////////////////////////////////////////
DATE CDataHolder::CVolaValuesMap::GetExpirationDate( long index )
{
	long i = 0;
	for( iterator it = begin(); it != end(); it++ )
		if( i == index )
			return it->first.date_;
		else
			i++;

	CComErrorWrapper::ThrowError( E_INVALIDARG, _T("GetExpirationDate() error - Index value is out of bounds.") );

	return 0;
}


CDataHolder::CSkew& CDataHolder::CVolaValuesMap::GetSkew( ExpDate date, double dIF )
{
	//double	dInt, dFrac;
	//dFrac = modf(date.date_, &dInt);

	//iterator it = end();
	//if (fabs(dFrac) < 0.0000001) // expiry date without time info
	//{
	//	double dDelta = 1., dComputeDlt;
	//	for(iterator it_s = begin(); it_s != end(); it_s++)
	//	{
	//		dComputeDlt = fabs(it_s->first.date_ - date.date_);
	//		if (dComputeDlt < dDelta){
	//			dDelta = dComputeDlt;
	//			it = it_s;
	//		}
	//	}
	//}
	/*else
		it = find( date );*/

	//TODO: Uncomment this code when one version of this service will be used for all IVRM versions
	iterator it = find( date ); 
	if( it == end() )
		it = insert( value_type(date, CSkew( dIF ) ) ).first;

	return it->second;
}


/////////////////////////////////////////////////////////////////////////////
// CDataHolder::CSkew class
/////////////////////////////////////////////////////////////////////////////
double CDataHolder::CSkew::GetBasePointsCount() const
{
	long nCount = 0;

	for( const_iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted || !it->bIsBasePoint )
			continue;

		nCount++;
	}

	return nCount;
}

		
CDataHolder::CPoint& CDataHolder::CSkew::GetPoint( long nIndex )
{
	if( nIndex < 0 || static_cast<size_t>(nIndex) > (size() - 1) )
		CComErrorWrapper::ThrowError( E_INVALIDARG, _T("GetPoint() error - Invalid point index value.") );

	return at( nIndex );
}


CDataHolder::CPoint& CDataHolder::CSkew::GetPoint( double dStrike, bool bSetErrorInfo )
{
	if( size() <= 0 )
		CComErrorWrapper::ThrowError( E_INVALIDARG, _T("GetPoint() error - Skew is empty.") );

	for( iterator it = begin(); it != end(); it++ )
		if( it->dStrike == dStrike )
			return *it;

	// If we are here it == end()
	if( bSetErrorInfo )
		CComErrorWrapper::ThrowError( E_INVALIDARG, _T("GetPoint() error - No explicitly defined point found.") );
	else
		throw _com_error( E_INVALIDARG );

	// Safe because of exception thrown a line above
	return at( 0 );
}


CDataHolder::CPoint& CDataHolder::CSkew::GetATMPoint( double dUnderlinePrice, bool bSetErrorInfo )
{
	iterator n = end();
	long   nPointsWatched  = 0;
	double delta = 0, temp = 0;

	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted )
			continue;

		temp = fabs( dUnderlinePrice - it->dStrike );
		if( nPointsWatched == 0 || temp < delta )
		{
			n     = it;
			delta = temp;
		}

		nPointsWatched++;
	}

	if( n == end() )
	{
		if( bSetErrorInfo )
			CComErrorWrapper::ThrowError( E_PENDING, _T("GetATMPoint() error - No points in skew.") );
		else
			throw _com_error( E_PENDING );
	}

	return *n;
}


double CDataHolder::CSkew::GetPointVolatility( double dStrike )
{
	try
	{
		if( !m_bInterpolationVectorUpdated )
		{
			CInterpolationEngine::PointsVector& pts = m_ie.GetPointVector();
			pts.clear();

			for( iterator it = begin(); it != end(); it++ )
			{
				if( it->enStatus == Deleted )
					continue;

				if( m_bBasePointsInterpolation )
				{
					if( it->bIsBasePoint )
						pts.push_back( CInterpolationEngine::Point( it->dStrike, it->dVola ) );
				}
				else
					pts.push_back( CInterpolationEngine::Point( it->dStrike, it->dVola ) );
			}

			m_ie.RecalculateVector();
			m_bInterpolationVectorUpdated = true;
		}

		return m_ie.GetInterpolatedValue( dStrike );
	}
	catch( const CInterpolationException& )
	{
		CComErrorWrapper::ThrowError( E_PENDING, _T("GetPointVolatility() error - Interpolation error.") );
	}

	return 0;
}


void CDataHolder::CSkew::SetBasePointsInterpolation( bool bValue )
{
	if( m_bBasePointsInterpolation == bValue )
		return;

	for( iterator it = begin(); it != end(); it++ )
		if( it->enStatus != Deleted )
			it->enStatus = Changed;

	m_bBasePointsInterpolation    = bValue;
	m_bInterpolationVectorUpdated = false;
	m_dwUpdateTime = GetTickCount();
}


// Inserts (returns true) or modifys (returns false) popint in skew
bool CDataHolder::CSkew::PutPoint( CDataHolder::CPoint& pt, long *pnIndex, bool bForceUntouched )
{
	if( begin() == end() )
	{
		pt.enStatus = bForceUntouched ? Untouched : Created;
		push_back( pt );

		if( pnIndex != NULL )
			*pnIndex = 0;
		usefulPtCount++;
		return true;
	}

	// Try to find point with same strike
    iterator it = begin();
	for(; it != end(); it++ )
	{
		if( it->dStrike == pt.dStrike )
		{
			it->dVola		 = pt.dVola;
			it->enStatus	 = bForceUntouched ? Untouched : Changed;
			it->bIsBasePoint = pt.bIsBasePoint;

			return false;
		}
		else if( it->dStrike > pt.dStrike)
		{
			pt.enStatus = bForceUntouched ? Untouched : Created;
			it = insert( it, pt );

			if( pnIndex != NULL )
				*pnIndex = it - begin();
			usefulPtCount++;
			return true;
		}
	}

	// If we are here it == end()
	pt.enStatus = bForceUntouched ? Untouched : Created;
	it = insert( it, pt );

	if( pnIndex != NULL )
		*pnIndex = it - begin();
	usefulPtCount++;
	return true;
}


void CDataHolder::CSkew::SetStrikeDelta( double dStrikeDelta )
{
	if( dStrikeDelta == 0 )
		return;

	std::vector<CPoint>			  newpts;
	std::vector<CPoint>::iterator newpt; 

	// Update points vola
	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted )
			continue;

		newpt = newpts.insert( newpts.end(), *it );
		newpt->dVola = GetPointVolatility( it->dStrike + dStrikeDelta );
		if( newpt->dVola < 0 )
			newpt->dVola = 0;
		newpt->enStatus = Changed;
	}

	clear();
	assign( newpts.begin(), newpts.end() );
}


void CDataHolder::CSkew::SetVolaDelta( double dVolaDelta )
{
	if( dVolaDelta == 0 )
		return;

	// Update points vola
	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted )
			continue;
		
		it->dVola += dVolaDelta;
		if( it->dVola < 0 )
			it->dVola = 0;
		it->enStatus = Changed;
	}
}

void CDataHolder::CSkew::SetVola( double dVola )
{
	if( dVola <= 0 )
		return;

	// Update points vola
	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted )
			continue;
		
		it->dVola = dVola;
		if( it->dVola < 0 )
			it->dVola = 0;
		it->enStatus = Changed;
	}
}


void CDataHolder::CSkew::ProportionalShift( double dRatio )
{
	if( dRatio == 1 )
		return;

	for( iterator it = begin(); it != end(); it++ )
	{
		if( it->enStatus == Deleted )
			continue;
		
		it->dVola *= dRatio;
		if( it->dVola < 0 )
			it->dVola = 0;
		it->enStatus = Changed;
	}
}


// Removes points marked as deleted and sets other points status to unchanged
void CDataHolder::CSkew::UpdateStatus()
{
	for( long i = 0; static_cast<size_t>(i) < size(); i++ )
	{
		CPoint& pt = GetPoint( i );

		if( pt.enStatus == Deleted )
			erase( begin() + i );
		else
			pt.enStatus = Untouched;
	}

	m_bInterpolationVectorUpdated = false;
	m_dwUpdateTime = GetTickCount();
}



