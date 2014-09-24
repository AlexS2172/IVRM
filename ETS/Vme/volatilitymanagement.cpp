// VolatilityManagement.cpp : Implementation of CVolatilityManagement
#include "stdafx.h"
#include "VME.h"

#include "VolatilityManagement.h"

_ATL_FUNC_INFO CVolatilityManagement::m_OnVMESurfaceInfo = 
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_DISPATCH}};
/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CVolatilityManagement::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVolatilityManagement
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Construction/destruction
/////////////////////////////////////////////////////////////////////////////
CVolatilityManagement::CVolatilityManagement()
{
}


HRESULT CVolatilityManagement::FinalConstruct()
{
	__CHECK_HRESULT( m_spSymbolObject.CreateInstance( CLSID_SymbolObject ), _T("Error creating publisher symbol object.") );

	RegisterPublisher();

	return S_OK;
}


void CVolatilityManagement::FinalRelease()
{
	ClearMaps();
	//UnregisterPublisher();
}


/////////////////////////////////////////////////////////////////////////////
// Notification entries
/////////////////////////////////////////////////////////////////////////////
void CVolatilityManagement::OnDefaultSurfaceChanged( const _bstr_t& SymbolName, long SurfaceID, EOptType OptType, bool bNewDefault )
{
	try
	{
		ObjectLock lock(this);

		if( bNewDefault )
		{
			for( CSurfaceMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
			{
				if( it->first.m_bsSymbol != SymbolName )
					continue;

				if( it->first.m_nSurfaceID == SurfaceID )
				{
					it->second->SetDefault( true );
				}
				else
					it->second->SetDefault( false );
			}
		}

		Fire_OnDefaultSurfaceChanged( SurfaceID, OptType, bNewDefault ? VARIANT_TRUE : VARIANT_FALSE );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		ATLTRACE( (PTCHAR)CComErrorWrapper::ErrorDescription( e ) );
	}
}


/////////////////////////////////////////////////////////////////////////////
// IVolatilityManagement implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CVolatilityManagement::get_DataSource(IVAManagement **pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_spDS.GetInterfacePtr();
	if( *pVal )
		(*pVal)->AddRef();

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::put_DataSource(IVAManagement *newVal)
{
	return putref_DataSource( newVal );
}


STDMETHODIMP CVolatilityManagement::putref_DataSource(IVAManagement *newVal)
{
	__CHECK_POINTER( newVal );

	ObjectLock lock(this);

	// Drop all surface refereneces.
	// It's up to users to reget surfaces.
	ClearMaps();

	// Store new data source reference
	m_spDS = newVal;

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::put_UnderlinePrice(BSTR Symbol, double newVal)
{
	try
	{
		ObjectLock lock(this);

		for( CSurfaceMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
			if( it->first.m_bsSymbol == _bstr_t(Symbol) )
			{
				VARIANT_BOOL bPriceOverride = VARIANT_FALSE;
				it->second->get_PriceOverride( &bPriceOverride );

				if( bPriceOverride != VARIANT_TRUE )
					it->second->put_UnderlinePrice( newVal );
			}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::get_Surface(long SurfaceID, EOptType OptType, IVMSymbolVolatilitySurface **pVal)
{
	__CHECK_POINTER( pVal );

	CComObject<CSymbolVolatilitySurface> *pObject = NULL;

	try
	{
		ObjectLock lock(this);

		// Try to find object
		CSurfaceKey key( SurfaceID, OptType );
		CSurfaceMap::iterator it = m_surfaces.find( key );
		
		// Ig not found - create new
		_bstr_t Symbol;
		if( it == m_surfaces.end() )
		{
			__CHECK_HRESULT( CComObject<CSymbolVolatilitySurface>::CreateInstance( &pObject ), _T("Error creating volatility surface object instance.") );
			__CHECK_HRESULT( pObject->Init( this ), _T("Error setting volatility surface data source.") );
			__CHECK_HRESULT( pObject->LoadData( SurfaceID, OptType, Symbol ), _T("Error loading volatility surface data.") );

			//pObject->SetPubManager(m_spPubManager);
			//pObject->SetSubManager(m_spSubManager);

			//if( FAILED( pObject->RegisterPublisher() ) )
			//	ATLTRACE( _T("Error registering volatility surface object as publisher.\n") );
			
			key.m_bsSymbol = Symbol;

			it = m_surfaces.insert( CSurfaceMap::value_type( key, pObject ) ).first;
			it->second->AddRef();
		}

		// Return surface pointer
		pObject = it->second;
		__CHECK_HRESULT( pObject->QueryInterface( IID_IVMSymbolVolatilitySurface, (void**)pVal ), NULL );
	}
	catch( const _com_error& e )
	{
		if( pObject )
			pObject->Release();
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::get_ActiveSurfaceID(BSTR Symbol, long *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		ObjectLock lock(this);

		// Try to find object
		CActiveSurfaceIDMap::iterator it = m_activeSurfaces.find( Symbol );
		
		// Ig not found - create new
		if( it == m_activeSurfaces.end() )
		{
			long nSurfaceID = m_spDS->GetDefaultSurfaceID( Symbol );
			
			it = m_activeSurfaces.insert( CActiveSurfaceIDMap::value_type( Symbol, nSurfaceID ) ).first;
		}

		*pVal = it->second;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::put_ActiveSurfaceID(BSTR Symbol, long newVal)
{
	try
	{
		ObjectLock lock(this);

		// Try to find object
		CActiveSurfaceIDMap::iterator it = m_activeSurfaces.find( Symbol );
		
		// Ig not found - create new
		if( it == m_activeSurfaces.end() )
			it = m_activeSurfaces.insert( CActiveSurfaceIDMap::value_type( Symbol, -1 ) ).first;

		it->second = newVal;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::get_DefaultSurfaceID(BSTR Symbol, long *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		ObjectLock lock(this);

		*pVal = m_spDS->GetDefaultSurfaceID( Symbol );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::put_DefaultSurfaceID(/*[in]*/BSTR Symbol, /*[in]*/ long newVal)
{
	try
	{
		ObjectLock lock(this);

		m_spDS->SaveDefaultSurfaceID( Symbol, newVal );

		for( CSurfaceMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		{
			if( it->first.m_nSurfaceID == newVal )
			{
				it->second->SetDefault( true );

				_RecordsetPtr spData;
				__CHECK_HRESULT( it->second->GetSurfaceData( spData, true, false ), _T("Error collecting surface data.") )
				__CHECK_HRESULT( it->second->PublishChanges( spData ), _T("Error publishing surface data.") )
				spData->Close();
			}
			else
				it->second->SetDefault( false );
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVolatilityManagement, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolatilityManagement", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CVolatilityManagement::RegisterPublisher()
{
	try
	{
		ObjectLock lock(this);

		__CHECK_HRESULT( m_spSymbolObject->Set( enStStock, _T("ALL_VOLA")/*m_bsSymbolName*/, _T(""), _T("") ), _T("Error setting publisher symbol object properties") );

		if( m_spPubManager == NULL )
		{
			__CHECK_HRESULT( m_spPubManager.CreateInstance( CLSID_MsgManager ), _T("Error creating publisher instance.") );
			__CHECK_HRESULT( m_spPubManager->RegPublisher( enMtVola, m_spSymbolObject ), _T("Error registering publisher.") );
		}

		if( m_spSubManager == NULL )
		{
			m_spSubManager = m_spPubManager;
			__CHECK_INTERFACE_POINTER( m_spSubManager, _T("Error quering subscriber interface.") );

			// Advise for events
			__CHECK_HRESULT( DispEventAdvise( m_spSubManager ), _T("Error advising messaging source interface") );
			__CHECK_HRESULT( m_spSubManager->SubVola( m_spSymbolObject ), _T("Error subscribing for vola messages.") );
		}

	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

STDMETHODIMP CVolatilityManagement::UnregisterPublisher()
{
	try
	{
		ObjectLock lock(this);

		ATLASSERT( m_spSymbolObject != NULL );
		if( m_spPubManager != NULL )
			__CHECK_HRESULT( m_spPubManager->UnregPublisher( enMtVola, m_spSymbolObject ), _T("Error unregistering publisher.") );
		m_spPubManager = NULL;

		if( m_spSubManager != NULL )
		{
			__CHECK_HRESULT( DispEventUnadvise( m_spSubManager ), _T("Error unadvising messaging source interface") );
			__CHECK_HRESULT( m_spSubManager->UnsubVola( m_spSymbolObject ), _T("Error unsubscribing from volatility messages.") );
		}
		m_spSubManager = NULL;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

STDMETHODIMP CVolatilityManagement::OnVMESurface(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data)
{
	ObjectLock lock(this);

	IVMESurfacePtr spData = Data;
	if (spData) {
		CSurfaceKey searchKey(spData->SurfaceID, static_cast<EOptType>(spData->OptType));
		CSurfaceMap::iterator itrSurface = m_surfaces.find(searchKey);
		if ( itrSurface != m_surfaces.end() ) {
			itrSurface->second->OnVMESurface(Symbol, Data );
		}
	}
	return S_OK;
}
/////////////////////////////////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CVolatilityManagement::ClearMaps()
{
	ObjectLock lock(this);

	for( CSurfaceMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
	{
		//it->second->UnregisterPublisher();
		it->second->Deinit();
		it->second->Release();
	}
	m_surfaces.clear();
	m_activeSurfaces.clear();

	return S_OK;
}
