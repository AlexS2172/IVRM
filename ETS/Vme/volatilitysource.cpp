// VolatilitySource.cpp : Implementation of CVolatilitySource
#include "stdafx.h"
#include "VME.h"

#include "VolatilitySource.h"


_ATL_FUNC_INFO CVolatilitySource::m_OnVMESurfaceInfo = 
{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_DISPATCH}};

/////////////////////////////////////////////////////////////////////////////
// Construction/destruction
/////////////////////////////////////////////////////////////////////////////
CVolatilitySource::CVolatilitySource():
	m_bEnableEvents(VARIANT_FALSE),
	m_bEnableCache(VARIANT_FALSE),
	m_bEnableEditing(VARIANT_FALSE),
	m_dDefaultVola(20.)
{
}


HRESULT CVolatilitySource::FinalConstruct()
{
	__CHECK_HRESULT( m_spSymbolObject.CreateInstance( CLSID_SymbolObject ), _T("Error creating publisher symbol object.") );

	RegisterPublisher();

	return S_OK;
}


void CVolatilitySource::FinalRelease()
{
	ClearMap();
	//UnregisterPublisher();
	m_spSymbolObject = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// Notification entries
/////////////////////////////////////////////////////////////////////////////
void CVolatilitySource::OnSurfaceChanged( const _bstr_t& SymbolName )
{
	Fire_VolatilityChanged( SymbolName );
}


/////////////////////////////////////////////////////////////////////////////
// IVolatilitySource implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CVolatilitySource::get_DataSource(IVAManagement **pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_spDS.GetInterfacePtr();
	if( *pVal )
		(*pVal)->AddRef();

	return S_OK;
}


STDMETHODIMP CVolatilitySource::put_DataSource(IVAManagement *newVal)
{
	putref_DataSource( newVal );

	return S_OK;
}


STDMETHODIMP CVolatilitySource::putref_DataSource(IVAManagement *newVal)
{
	__CHECK_POINTER( newVal );
	
	ObjectLock lock(this);

	// Drop all surface refereneces.
	// It's up to users to reget surfaces.
	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
	{
		CComObject<CSymbolVolatility> *pObject = it->second;
		
		ObjectLock lock( pObject );

		pObject->Deinit();
		pObject->Init( this, it->first, SYMBOL_TYPE(0) );
		pObject->LoadData();
	}

	// Store new data source reference
	m_spDS = newVal;

	return S_OK;
}


STDMETHODIMP CVolatilitySource::get_SymbolVolatility(BSTR Symbol, SYMBOL_TYPE SymbolType, IVSSymbolVolatility **pVal)
{
	__CHECK_POINTER( pVal );

	CComObject<CSymbolVolatility> *pObject = NULL;
	bool bAddedToCollection = false;

	try
	{
		ObjectLock lock(this);

		// Try to find object
		_bstr_t bsSymbol(Symbol);

		CSymbolsMap::iterator it = m_surfaces.find( bsSymbol );
		IVSSymbolVolatilityPtr spSymbolVolatility;

		// If not found - create new
		if( it == m_surfaces.end() )
		{
			__CHECK_HRESULT( CComObject<CSymbolVolatility>::CreateInstance( &pObject ), _T("Error creating symbol volatility object instance.") );
			spSymbolVolatility.Attach(pObject, TRUE);

			pObject->DefaultVola( m_dDefaultVola );
			
			__CHECK_HRESULT( pObject->Init( this, bsSymbol, SymbolType ), _T("Error setting volatility surface data source.") );
			__CHECK_HRESULT( pObject->LoadData(), _T("Error loading volatility surface data.") );

			it = m_surfaces.insert( CSymbolsMap::value_type( bsSymbol, pObject ) ).first;
			it->second->AddRef();
			bAddedToCollection = true;
		}
		else
		{
			pObject = it->second;
			spSymbolVolatility.Attach(pObject, TRUE);
		}

		pObject->EnableCache  ( m_bEnableCache   );
		pObject->EnableEditing( m_bEnableEditing );
		pObject->EnableEvents ( m_bEnableEvents  );
		pObject->DefaultVola  ( m_dDefaultVola   );

		// Return surface pointer
		*pVal = spSymbolVolatility.Detach();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"VolatilitySource", L"", __FILE__,__FUNCDNAME__,__LINE__);
	}
	catch(...)
	{
		REPORT_UNHANDLED_EXCEPTION
		return E_FAIL;
	}
	return S_OK;
}


STDMETHODIMP CVolatilitySource::get_EnableEvents(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	ObjectLock lock(this);

	*pVal = m_bEnableEvents;

	return S_OK;
}


STDMETHODIMP CVolatilitySource::put_EnableEvents(VARIANT_BOOL newVal)
{
	ObjectLock lock(this);

	m_bEnableEvents = newVal;
	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		it->second->EnableEvents( m_bEnableEvents );

	return S_OK;
}


STDMETHODIMP CVolatilitySource::get_EnableCache(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	ObjectLock lock(this);

	*pVal = m_bEnableCache;

	return S_OK;
}


STDMETHODIMP CVolatilitySource::put_EnableCache(VARIANT_BOOL newVal)
{
	ObjectLock lock(this);

	m_bEnableCache = newVal;
	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		it->second->EnableCache( m_bEnableCache );

	return S_OK;
}


STDMETHODIMP CVolatilitySource::get_EnableEditing(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	ObjectLock lock(this);

	*pVal = m_bEnableEditing;

	return S_OK;
}


STDMETHODIMP CVolatilitySource::put_EnableEditing(VARIANT_BOOL newVal)
{
	ObjectLock lock(this);

	m_bEnableEditing = newVal;
	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		it->second->EnableEditing( m_bEnableEditing );

	return S_OK;
}


STDMETHODIMP CVolatilitySource::get_DefaultVolatility(double *pVal)
{
	__CHECK_POINTER( pVal );

	ObjectLock lock(this);

	*pVal = m_dDefaultVola;

	return S_OK;
}


STDMETHODIMP CVolatilitySource::put_DefaultVolatility(double newVal)
{
	ObjectLock lock(this);

	m_dDefaultVola = newVal;
	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		it->second->DefaultVola( m_dDefaultVola );

	return S_OK;
}


STDMETHODIMP CVolatilitySource::Reload()
{
	ObjectLock lock(this);

	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
		it->second->LoadData();

	return S_OK;
}

STDMETHODIMP CVolatilitySource::RegisterPublisher()
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

STDMETHODIMP CVolatilitySource::UnregisterPublisher()
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
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

STDMETHODIMP CVolatilitySource::OnVMESurfaceSource(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data)
{
	ObjectLock lock(this);

	IVMESurfacePtr spData = Data;
	if (spData) {
		_bstr_t bSymbol = spData->Symbol;
		CSymbolsMap::iterator itrSurface = m_surfaces.find(bSymbol);
		if ( itrSurface != m_surfaces.end() ) {
			itrSurface->second->OnVMESurface(Symbol, Data );
		}
	}
	return S_OK;
}

void CVolatilitySource::OnSimulatedSave(const _bstr_t& Symbol)
{
	ObjectLock lock(this);

	CSymbolsMap::iterator it = m_surfaces.find( Symbol );

	if( it != m_surfaces.end() )
		it->second->LoadData();

	return;
}

STDMETHODIMP CVolatilitySource::get_SimulatedSymbolVol(BSTR Symbol, SYMBOL_TYPE SymbolType, IVSSymbolVolatility **pVal)
{
	__CHECK_POINTER( pVal );

	IVSSymbolVolatilityPtr spSymbolVolatility;
	CComObject<CSymbolVolatility> *pObject = NULL;

	try
	{
		ObjectLock lock(this);

		__CHECK_HRESULT( CComObject<CSymbolVolatility>::CreateInstance( &pObject ), _T("Error creating symbol volatility object instance.") );
		spSymbolVolatility.Attach(pObject, TRUE);

		pObject->DefaultVola( m_dDefaultVola );

		__CHECK_HRESULT( pObject->Init( this, Symbol, SymbolType ), _T("Error setting volatility surface data source.") );

		CSymbolsMap::iterator it = m_surfaces.find( Symbol );
		if( it != m_surfaces.end() )
		{
			CSurfaceGroup* pData = NULL;
			__CHECK_HRESULT( it->second->GetInternalBuffer(&pData), _T("Error loading volatility surface data.") );
			__CHECK_HRESULT( pObject->SetInternalBuffer(pData), _T("Error saving volatility surface data.") );
		}
		else
		{
			__CHECK_HRESULT( pObject->LoadData(), _T("Error loading volatility surface data.") );
		}

		//if( FAILED( pObject->RegisterPublisher() ) )
		//	ATLTRACE( _T("Error registering volatility surface object as publisher.\n") );

		pObject->EnableCache  ( m_bEnableCache   );
		pObject->EnableEditing( m_bEnableEditing );
		pObject->EnableEvents ( m_bEnableEvents  );
		pObject->DefaultVola  ( m_dDefaultVola   );
		pObject->put_SimulatedVol(VARIANT_TRUE);

		// Return surface pointer
		*pVal = spSymbolVolatility.Detach();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"VolatilitySource", L"", __FILE__,__FUNCDNAME__,__LINE__);
	}
	catch(...)
	{
                REPORT_UNHANDLED_EXCEPTION
		return E_FAIL;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Helpers
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CVolatilitySource::ClearMap()
{
	ObjectLock lock(this);

	for( CSymbolsMap::iterator it = m_surfaces.begin(); it != m_surfaces.end(); it++ )
	{
		//it->second->UnregisterPublisher();
		it->second->Deinit();
		it->second->Release();
	}
	m_surfaces.clear();

	return S_OK;
}
