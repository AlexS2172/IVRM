// TimeSkewVolatilities.cpp : Implementation of CTimeSkewVolatilities
#include "stdafx.h"
#include "VME.h"

#include "TimeSkewVolatilities.h"

/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CTimeSkewVolatilities::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVMTimeSkewVolatilities
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Construction/initialization/destruction
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CTimeSkewVolatilities::Init( CSymbolVolatilitySurface* pSurface, 
										  long nSurfaceID, EOptType enOptType )
{
	__CHECK_POINTER( pSurface );

	ObjectLock lock(this);

	m_pSurface	= pSurface;
	m_spSurface = pSurface;

	m_nSurfaceID  = nSurfaceID;
	m_enOptType   = enOptType;

	m_pData = &(pSurface->m_data);

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IVMTimeSkewVolatilities implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CTimeSkewVolatilities::get_Count(long *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_pData->GetExpirationsCount();

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::get_ExpDate(long index, DATE *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetExpirationDate( index );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::get_Volatility(DATE ExpDate, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetTimeSkewVolatility( ExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
	}

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::get_Strike(DATE ExpDate, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetTimeSkewStrike( ExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;

		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
	}

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::put_VolatilityDelta(DATE ExpDate, double newVal)
{
	try
	{
		m_pData->SetTimeSkewVolatilityDelta( ExpDate, newVal );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::Save()
{
	try
	{
		ObjectLock lock(this);

		// Get data source
		IVAManagementPtr spDS;
		__CHECK_HRESULT( m_spSurface->get_DataSource( &spDS ), _T("Error getting data source.") );

		// Create recordset instance
		_RecordsetPtr spRS;
		__CHECK_HRESULT( m_pSurface->GetSurfaceData( spRS, false, false ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( spDS->SaveSurfaceVolatilities( m_nSurfaceID, m_enOptType, spRS ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( m_pSurface->PublishChanges( spRS ), _T("Error publishing changes.") );

		// Close recordset
		spRS->Close();

		m_spSurface->Reload();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
	//	return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
    }

	return S_OK;
}


STDMETHODIMP CTimeSkewVolatilities::Reload()
{
	try
	{
		m_spSurface->Reload();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMTimeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"TimeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}
