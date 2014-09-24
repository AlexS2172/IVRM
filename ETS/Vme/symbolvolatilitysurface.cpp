// SymbolVolatilitySurface.cpp : Implementation of CSymbolVolatilitySurface
#include "stdafx.h"
#include "VME.h"

#include "SymbolVolatilitySurface.h"
#include "TimeSkewVolatilities.h"
#include "StrikeSkewVolatilities.h"

#include "VolatilityManagement.h"

_ATL_FUNC_INFO CSymbolVolatilitySurface::m_OnVMESurfaceInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_DISPATCH}};

/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatilitySurface::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVMSymbolVolatilitySurface
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// Construction/destruction/initialization
/////////////////////////////////////////////////////////////////////////////
CSymbolVolatilitySurface::CSymbolVolatilitySurface(): 
	m_pManager(NULL), 
	m_nSurfaceID(0), 
	m_enOptType(Call),
	m_bIsDefault(false),
	m_bEnableEvents(VARIANT_TRUE)
{
	m_spPubManager = NULL;
	m_spSubManager = NULL;
}


HRESULT CSymbolVolatilitySurface::FinalConstruct()
{
	try
	{
		__CHECK_HRESULT( m_spSymbolObject.CreateInstance( CLSID_SymbolObject ), _T("Error creating publisher symbol object.") );
		__CHECK_HRESULT( m_spVMESurface.CreateInstance( CLSID_VMESurface ), _T("Error creating publisher data object.") );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


void CSymbolVolatilitySurface::FinalRelease()
{
	//UnregisterPublisher();

	m_spSymbolObject = NULL;
	m_spVMESurface   = NULL;
}


STDMETHODIMP CSymbolVolatilitySurface::Init( CVolatilityManagement* pManager )
{
	__CHECK_POINTER( pManager );

	ObjectLock lock(this);

	m_pManager = pManager;
	m_spDS	   = pManager->m_spDS;

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::Deinit()
{
	ObjectLock lock(this);

	m_pManager = NULL;

	return S_OK;
}


/*STDMETHODIMP CSymbolVolatilitySurface::RegisterPublisher()
{
	try
	{
		ObjectLock lock(this);

		__CHECK_HRESULT( m_spSymbolObject->Set( m_enSymbolType, m_bsSymbolName, _T(""), _T("") ), _T("Error setting publisher symbol object properties") );

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
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}*/


/*STDMETHODIMP CSymbolVolatilitySurface::UnregisterPublisher()
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
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}*/


STDMETHODIMP CSymbolVolatilitySurface::LoadData( long SurfaceID, EOptType OptType, _bstr_t& Symbol )
{
	try
	{
		ObjectLock lock(this);

		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error loading volatility surface - no data source provided.") );

		// Store surface properties
		m_nSurfaceID  = SurfaceID;
		m_enOptType	  = OptType;

		// Prepare buffer
		m_data.ClearAll();

		_RecordsetPtr spRS;

		// Get vola surface for specified mode
		spRS = m_spDS->GetSurfaceVolatilities( m_nSurfaceID, m_enOptType );
		__CHECK_HRESULT( SetSurfaceData( spRS ), _T("Error setting surface data.") );
		spRS->Close();

		spRS = m_spDS->GetVolatilitySurfaceProps( SurfaceID );
		__CHECK_HRESULT( SetSurfaceProps( spRS ), _T("Error setting surface properties.") );
		spRS->Close();

		Symbol = m_bsSymbolName;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::SetDefault( bool bDefault )
{
	try
	{
		m_bIsDefault = bDefault;
	}
	catch( const _com_error& e )
	{
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::SetSurfaceData( _RecordsetPtr& spData )
{
	try
	{
		ObjectLock lock(this);

		// For each record
		if( SUCCEEDED( spData->raw_MoveFirst() ) )
		{
			for( ; !spData->BOF && !spData->Eof; spData->MoveNext() )
				m_data.SetPoint( spData->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value,
								 spData->Fields->GetItem(L"fStrike")->Value,
								 spData->Fields->GetItem(L"dtExpDate")->Value,
								 spData->Fields->GetItem(L"fVolatility")->Value,
								 spData->Fields->GetItem(L"iIsBasePoint")->Value
							   );

			m_data.UpdateSurfaceStatus();
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::SetSurfaceProps( _RecordsetPtr& spProps )
{
	_variant_t vtValue;
	
	try
	{
		ObjectLock lock(this);

		spProps->MoveFirst();

		vtValue = spProps->Fields->GetItem(L"fBaseUnderlinePrice")->Value;
		if( vtValue.vt != VT_NULL )
			m_data.SetUnderlinePrice( vtValue );
		else
		m_data.SetUnderlinePrice( 0.0f );

		vtValue = spProps->Fields->GetItem(L"fSmileAccelerator")->Value;
		if( vtValue.vt != VT_NULL )
			m_data.SetSmileAccelerator( vtValue );
		else
			m_data.SetSmileAccelerator( 0.0f );

		vtValue = spProps->Fields->GetItem(L"fInterpolationFactor")->Value;
		if( vtValue.vt != VT_NULL )
			m_data.SetInterpolationFactor( 1.0f - vtValue.dblVal );
		else
			m_data.SetInterpolationFactor( 0.0f );

		vtValue = spProps->Fields->GetItem(L"bIsDiscreteAcceleration")->Value;
		if( vtValue.vt != VT_NULL )
			m_data.SetDiscreteAcceleration( vtValue );
		else
			m_data.SetDiscreteAcceleration( false );

		vtValue = spProps->Fields->GetItem(L"bIsPriceOverride")->Value;
		if( vtValue.vt != VT_NULL )
			m_data.SetPriceOverride( vtValue );
		else
			m_data.SetPriceOverride( false );

		m_bsSymbolName = spProps->Fields->GetItem(L"vcSymbolName")->Value;
		switch( spProps->Fields->GetItem(L"iContractTypeID")->Value.lVal )
		{
			case 1:
				m_enSymbolType = enStIndex;
				break;
			case 2:
				m_enSymbolType = enStStock;
				break;
			case 3:
				m_enSymbolType = enStOption;
				break;
			default:
				ATLASSERT( FALSE );
		}

		m_bIsDefault = spProps->Fields->GetItem(L"bIsDefaultSurface")->Value;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::GetSurfaceData( _RecordsetPtr& spData, bool bAllRecords, bool bClone )
{
	try
	{
		ObjectLock lock(this);

		// Create recordset instance
		if( !spData.GetInterfacePtr() )
			__CHECK_HRESULT( spData.CreateInstance( CLSID_Recordset ), _T("Error creating recordset instance.") );

		// Specify RS fields and properties
		spData->CursorLocation = adUseClient;
		spData->LockType	   = adLockOptimistic;
		spData->Fields->Append( L"iCustomStrikeSkewPointID", adInteger, 4, adFldUnspecified );
		spData->Fields->Append( L"fStrike",			         adDouble,  8, adFldUnspecified );
		spData->Fields->Append( L"fVolatility",			     adDouble,  8, adFldUnspecified );
		spData->Fields->Append( L"dtExpDate",				 adDate,    4, adFldUnspecified );
		spData->Fields->Append( L"iIsBasePoint",			 adInteger, 4, adFldUnspecified );
		spData->Fields->Append( L"iStatus",				     adInteger, 4, adFldUnspecified );

		// Open disconnected RS
		__CHECK_HRESULT( spData->Open( L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified ), _T("Error opening recordset.") );

		// For each expiration
		for( long j = 0; j < m_data.GetExpirationsCount(); j++ )
		{
			DATE dtExpDate = m_data.GetExpirationDate( j );

			// For each strike
			if( bClone )
			{
				for( long i = 0; i < m_data.GetPointsCount( dtExpDate ); i++ )
				{
					EVolaItemStatus enStatus = m_data.GetPointStatus( dtExpDate, i );
					if( enStatus == Deleted )
						continue;

					spData->AddNew();
					spData->Fields->GetItem(L"iCustomStrikeSkewPointID")->PutValue( -1L );
					spData->Fields->GetItem(L"iStatus")->PutValue( (long)Created );
					spData->Fields->GetItem(L"fStrike")->PutValue( m_data.GetPointStrike( dtExpDate, i ) );
					spData->Fields->GetItem(L"fVolatility")->PutValue( m_data.GetPointVolatility( dtExpDate, i ) );
					spData->Fields->GetItem(L"dtExpDate")->PutValue( dtExpDate );
					spData->Fields->GetItem(L"iIsBasePoint")->PutValue( m_data.GetIsBasePoint( dtExpDate, i ) );
					spData->Update();
				}
			}
			else
			{
				for( long i = 0; i < m_data.GetPointsCount( dtExpDate ); i++ )
				{
					EVolaItemStatus enStatus = m_data.GetPointStatus( dtExpDate, i );
					if( !bAllRecords && enStatus == Untouched )
						continue;

					spData->AddNew();
					spData->Fields->GetItem(L"iCustomStrikeSkewPointID")->PutValue( m_data.GetPointID( dtExpDate, i ) );
					spData->Fields->GetItem(L"iStatus")->PutValue( (long)enStatus );
					spData->Fields->GetItem(L"fStrike")->PutValue( m_data.GetPointStrike( dtExpDate, i ) );
					spData->Fields->GetItem(L"dtExpDate")->PutValue( dtExpDate );
					spData->Fields->GetItem(L"iIsBasePoint")->PutValue( m_data.GetIsBasePoint( dtExpDate, i ) );

					if( enStatus != Deleted )
					{
						spData->Fields->GetItem(L"fVolatility")->PutValue( m_data.GetPointVolatility( dtExpDate, i ) );
					}
					spData->Update();
				}
			}
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::GetSurfaceProps( _RecordsetPtr& spProps )
{
	try
	{
		ObjectLock lock(this);

		// Create recordset instance
		if( !spProps.GetInterfacePtr() )
			__CHECK_HRESULT( spProps.CreateInstance( CLSID_Recordset ), _T("Error creating recordset instance.") );

		// Open diconnected recordset
		spProps->CursorLocation = adUseClient;
		spProps->LockType = adLockOptimistic;
		spProps->Fields->Append( L"fBaseUnderlinePrice",   adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"fSmileAccelerator",     adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"fInterpolationFactor",  adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"bDiscreteAcceleration", adInteger, 4, adFldUnspecified );
		spProps->Fields->Append( L"bPriceOverride",		   adInteger, 4, adFldUnspecified );
		spProps->Fields->Append( L"vcSymbolName",		   adVarChar, 8, adFldUnspecified );
		__CHECK_HRESULT( spProps->Open( L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified ), _T("Error opening recordset.") );

		// Put data into recordset
		spProps->AddNew();
		spProps->Fields->GetItem(L"fBaseUnderlinePrice")->PutValue( m_data.GetUnderlinePrice() );
		spProps->Fields->GetItem(L"fSmileAccelerator")->PutValue( m_data.GetSmileAccelerator() );
		spProps->Fields->GetItem(L"fInterpolationFactor")->PutValue( 1.0 - m_data.GetInterpolationFactor() );
		spProps->Fields->GetItem(L"bDiscreteAcceleration")->PutValue( m_data.GetDiscreteAcceleration() );
		spProps->Fields->GetItem(L"bPriceOverride")->PutValue( m_data.GetPriceOverride() );
		spProps->Fields->GetItem(L"vcSymbolName")->PutValue( m_bsSymbolName );
		spProps->Update();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::PublishChanges( _RecordsetPtr& spRS )
{
	try
	{
		ObjectLock  lock(this);
		if( m_bIsDefault )
		{
			CObjectLock dataLock( &m_data );

			m_spVMESurface->SurfaceID = m_nSurfaceID;
			m_spVMESurface->OptType   = m_enOptType;
			m_spVMESurface->UnderlinePrice = m_data.GetUnderlinePrice();
			m_spVMESurface->SmileAccelerator = m_data.GetSmileAccelerator();
			m_spVMESurface->InterpolationFactor = m_data.GetInterpolationFactor();
			m_spVMESurface->PriceOverride = m_data.GetPriceOverride() ? VARIANT_TRUE : VARIANT_FALSE;
			m_spVMESurface->DiscreteAcceleration = m_data.GetDiscreteAcceleration() ? VARIANT_TRUE : VARIANT_FALSE;
			m_spVMESurface->Symbol = m_bsSymbolName;
			m_spVMESurface->Points = spRS->Clone( adLockReadOnly );

			m_pManager->PublishChanges(spRS, m_spVMESurface );
			//m_spPubManager->PubVola( m_spSymbolObject, m_spVMESurface );
		}
	}
	catch( const _com_error& e )
	{
		_bstr_t message(L"Error calling publishing manager. ");
		message += CComErrorWrapper::ErrorDescription( e );
		REPORT_ERR_TO_MAIL(e)
//		return Error( (PTCHAR)message, IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", message, __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IVMSymbolVolatilitySurface implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatilitySurface::get_SmileAccelerator(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_data.GetSmileAccelerator();
	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::put_SmileAccelerator(double newVal)
{
	m_data.SetSmileAccelerator( newVal );

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_UnderlinePrice(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_data.GetUnderlinePrice();
	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::put_UnderlinePrice(double newVal)
{
	m_data.SetUnderlinePrice( newVal );

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_PriceOverride(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_data.GetPriceOverride() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CSymbolVolatilitySurface::put_PriceOverride(VARIANT_BOOL newVal)
{
	m_data.SetPriceOverride( newVal == VARIANT_TRUE ? true : false );

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_DiscreteAcceleration(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_data.GetDiscreteAcceleration() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::put_DiscreteAcceleration(VARIANT_BOOL newVal)
{
	m_data.SetDiscreteAcceleration( newVal == VARIANT_TRUE ? true : false );

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_InterpolationFactor(double *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_data.GetInterpolationFactor();
	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::put_InterpolationFactor(double newVal)
{
	m_data.SetInterpolationFactor( newVal );

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_DataSource(IVAManagement **pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_spDS.GetInterfacePtr();
	(*pVal)->AddRef();

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_TimeSkew(IVMTimeSkewVolatilities **pVal)
{
	__CHECK_POINTER( pVal );

	CComObject<CTimeSkewVolatilities> *pObject = NULL;

	try
	{
		ObjectLock lock(this);

		// Create object instance
		__CHECK_HRESULT( CComObject<CTimeSkewVolatilities>::CreateInstance(&pObject), _T("Error creating TimeSkewVolatilities instance.") );

		// Init object
		__CHECK_HRESULT( pObject->Init( this, m_nSurfaceID, m_enOptType ), _T("Error initializing TimeSkewVolatilities object.") );

		// Return object reference
		__CHECK_HRESULT( pObject->QueryInterface(IID_IVMTimeSkewVolatilities, (void**)pVal), NULL );
	}
	catch( const _com_error& e )
	{
		if( pObject )
			pObject->Release();
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_Volatility(double UnderlinePrice, double Strike, DATE ExpDate, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_data.GetPointVolatility( ExpDate, Strike, UnderlinePrice );
	}
	catch( const _com_error& )
	{
		*pVal = -1;
		return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_StrikeSkew(DATE ExpDate, IVMStrikeSkewVolatilities **pVal)
{
	__CHECK_POINTER( pVal );

	CComObject<CStrikeSkewVolatilities> *pObject = NULL;

	try
	{
		ObjectLock lock(this);

		// Create object instance
		__CHECK_HRESULT( CComObject<CStrikeSkewVolatilities>::CreateInstance(&pObject), _T("Error creating StrikeSkewVolatilities instance.") );

		// Init object
		__CHECK_HRESULT( pObject->Init( this, m_bsSymbolName, m_enSymbolType, m_nSurfaceID, m_enOptType, ExpDate ), _T("Error initializing StrikeSkewVolatilities object.") );

		// Return object reference
		__CHECK_HRESULT( pObject->QueryInterface(IID_IVMStrikeSkewVolatilities, (void**)pVal), NULL );
	}
	catch( const _com_error& e )
	{
		if( pObject )
			pObject->Release();
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::get_EnableEvents(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		ObjectLock lock(this);

		*pVal = m_bEnableEvents;
	}
	catch( const _com_error& e )
	{
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::put_EnableEvents(VARIANT_BOOL newVal)
{
	try
	{
		ObjectLock lock(this);

		m_bEnableEvents = newVal;

		/*if( m_bEnableEvents == VARIANT_TRUE )
			RegisterPublisher();
		else
			UnregisterPublisher();*/
	}
	catch( const _com_error& e )
	{
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::Save()
{
	try
	{
		ObjectLock lock(this);

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		// Save properties
		_RecordsetPtr spRS;
		__CHECK_HRESULT( GetSurfaceProps( spRS ), _T("Error collecting volatility surface properties.") );
		__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( m_nSurfaceID, spRS ), _T("Error saving volatility surface properties.") );
		spRS->Close();

		// Save data
		__CHECK_HRESULT( GetSurfaceData( spRS, false, false ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( m_nSurfaceID, m_enOptType, spRS ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( PublishChanges( spRS ), _T("Error publishing changes") );

		spRS->Close();

		// Reload data
		Reload();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::SaveAs(long SurfaceID, VARIANT_BOOL NewDefault)
{
	try
	{
		ObjectLock lock(this);

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		// Save properties
		_RecordsetPtr spRS;
		__CHECK_HRESULT( GetSurfaceProps( spRS ), _T("Error collecting volatility surface properties.") );
		__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( SurfaceID, spRS ), _T("Error saving volatility surface properties.") );
		spRS->Close();

		// Save data
		__CHECK_HRESULT( GetSurfaceData( spRS, true, true ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( SurfaceID, m_enOptType, spRS ), _T("Error saving volatility surface data.") );

		// Publish changes
		if( NewDefault == VARIANT_TRUE )
			__CHECK_HRESULT( PublishChanges( spRS ), _T("Error publishing changes") );

		spRS->Close();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatilitySurface::Reload()
{
	try
	{
		_bstr_t Symbol;
		LoadData( m_nSurfaceID, m_enOptType, Symbol );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMSymbolVolatilitySurface, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatilitySurface", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// _IOnMessages implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatilitySurface::OnVMESurface(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data)
{
	try
	{
		ObjectLock lock(this);

		IVMESurfacePtr spData = Data;
		if(	m_nSurfaceID != spData->SurfaceID || m_enOptType != spData->OptType )
		{
			// Provide new default surface ID and opt type for clients.
			// Do not import data as them are from different surface.
			m_pManager->OnDefaultSurfaceChanged( m_bsSymbolName, spData->SurfaceID, (EOptType)spData->OptType, true );
			return S_OK;
		}
		else if( m_bIsDefault )
		{
			// Set properties
			m_data.SetUnderlinePrice( spData->UnderlinePrice );
			m_data.SetInterpolationFactor( spData->InterpolationFactor );
			m_data.SetSmileAccelerator( spData->SmileAccelerator );
			m_data.SetPriceOverride( spData->PriceOverride == VARIANT_TRUE ? true : false );
			m_data.SetDiscreteAcceleration( spData->DiscreteAcceleration == VARIANT_TRUE ? true : false );

			// Import points
			_RecordsetPtr spPoints = spData->Points;
			if( spPoints != NULL && SUCCEEDED( spPoints->raw_MoveFirst() ) )
			{
				CObjectLock dataLock( &m_data );

				for( ; !spPoints->BOF && !spPoints->Eof; spPoints->MoveNext() )
					m_data.ImportPoint( spPoints->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value,
										spPoints->Fields->GetItem(L"fStrike")->Value,
										spPoints->Fields->GetItem(L"dtExpDate")->Value,
										spPoints->Fields->GetItem(L"fVolatility")->Value,
										spPoints->Fields->GetItem(L"iIsBasePoint")->Value,
										spPoints->Fields->GetItem(L"iStatus")->Value
									  );

				m_data.UpdateSurfaceStatus();
			}

			m_pManager->OnDefaultSurfaceChanged( m_bsSymbolName, m_nSurfaceID, m_enOptType, !m_bIsDefault );
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		ATLTRACE( (PTCHAR)CComErrorWrapper::ErrorDescription( e ) );
	}

	return S_OK;
}
/*STDMETHODIMP CSymbolVolatilitySurface::SetPubManager(IPubManagerPtr& spPubManger)
{
	if (m_spPubManager == NULL)
		m_spPubManager = spPubManger;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatilitySurface::SetSubManager(ISubManagerPtr& spSubManger)
{
	if (m_spSubManager == NULL)
		m_spSubManager = spSubManger;
	return S_OK;
}*/