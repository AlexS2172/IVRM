// SymbolVolatility.cpp : Implementation of CSymbolVolatility
#include "stdafx.h"
#include "VME.h"
#include "SymbolVolatility.h"

#include "VolatilitySource.h"

_ATL_FUNC_INFO CSymbolVolatility::m_OnVMESurfaceInfo = 
	{CC_STDCALL, VT_EMPTY, 2, {VT_DISPATCH, VT_DISPATCH}};

/////////////////////////////////////////////////////////////////////////////
// Construction/destruction/initialization
/////////////////////////////////////////////////////////////////////////////
CSymbolVolatility::CSymbolVolatility(): 
	m_bInitialized(false),
	m_bEnableEvents(VARIANT_TRUE),
	m_bEnableCache(VARIANT_FALSE),
	m_bEnableEditing(VARIANT_FALSE),
	m_pSource(NULL), 
	//m_nSurfaceID(0), 
	m_enOptType(OTM),
	m_bIsSimulated(VARIANT_FALSE)
{
	m_SurfaceGroup.ClearAll();
}


HRESULT CSymbolVolatility::FinalConstruct()
{
	try
	{
		__CHECK_HRESULT( m_spSymbolObject.CreateInstance( CLSID_SymbolObject ), _T("Error creating publisher symbol object.") );
		__CHECK_HRESULT( m_spVMESurface.CreateInstance( CLSID_VMESurface ), _T("Error creating publisher data object.") );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
	}

	return S_OK;
}


void CSymbolVolatility::FinalRelease()
{
	//UnregisterPublisher();

	m_spSymbolObject = NULL;
	m_spVMESurface   = NULL;
}


STDMETHODIMP CSymbolVolatility::Init( CVolatilitySource* pSource, const _bstr_t& bsSymbolName, SYMBOL_TYPE enSymbolType )
{
	__CHECK_POINTER( pSource );

	try
	{
		ObjectLock lock(this);

		if( m_bInitialized )
			Deinit();

		m_pSource = pSource;
		m_spDS	  = pSource->m_spDS;

		m_bsSymbolName = bsSymbolName;
		m_enSymbolType = enSymbolType == 0 ? m_enSymbolType : enSymbolType;

		m_bInitialized = true;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::Deinit()
{
	try
	{
		ObjectLock lock(this);

		// Deinit() intentionaly skips symbol name and type clearing!!!
		m_SurfaceGroup.ClearAll();
		m_pSource = NULL;
		m_spDS	  = NULL;

		m_bInitialized = false;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


/*STDMETHODIMP CSymbolVolatility::RegisterPublisher()
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
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}*/


/*STDMETHODIMP CSymbolVolatility::UnregisterPublisher()
{
	try
	{
		ObjectLock lock(this);

		ATLASSERT( m_spSymbolObject != NULL );
		if( m_spPubManager != NULL )
			__CHECK_HRESULT( m_spPubManager->UnregPublisher( enMtVola, m_spSymbolObject ), _T("Error unregistering publisher.") );
		//m_spPubManager = NULL;

		if( m_spSubManager != NULL )
		{
			__CHECK_HRESULT( DispEventUnadvise( m_spSubManager ), _T("Error unadvising messaging source interface") );
			__CHECK_HRESULT( m_spSubManager->UnsubVola( m_spSymbolObject ), _T("Error unsubscribing from volatility messages.") );
		}
		//m_spSubManager = NULL;
	}
	catch( const _com_error& e )
	{
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}
	
	return S_OK;
}*/


STDMETHODIMP CSymbolVolatility::EnableEvents( VARIANT_BOOL bEnable )
{
	ObjectLock lock(this);

	m_bEnableEvents = bEnable;

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::EnableCache( VARIANT_BOOL bEnable )
{
	ObjectLock lock(this);

	m_bEnableCache = bEnable;

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::EnableEditing( VARIANT_BOOL bEnable )
{
	ObjectLock lock(this);

	m_bEnableEditing = bEnable;

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::DefaultVola( double dVola )
{
	ObjectLock lock(this);

	m_dDefaultVola = dVola;

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::LoadData()
{
	try
	{
		ObjectLock lock(this);

		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error loading volatility surface - no data source provided.") );

		HRESULT hr;
		/*hr = m_spDS->raw_GenerateDefaultVolatilities( m_bsSymbolName, OTM, m_dDefaultVola );
		if( FAILED(hr) && hr != E_NOTIMPL )
			CComErrorWrapper::ThrowError( hr, _T("Error generating default volatilities.") );*/

		long nSurfaceID = -1;
		hr = m_spDS->raw_GetDefaultSurfaceID( m_bsSymbolName, &nSurfaceID );

		if( SUCCEEDED(hr) )
		{
			// Prepare buffer
			m_SurfaceGroup.AddSurface(_bstr_t("DEFAULT"), -1, nSurfaceID);
			CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(nSurfaceID);
			dhData.ClearAll();

			__CHECK_HRESULT4( LoadSurfaceGroups(), _T("Error loading surface group data.")); 

			//load volatilities for all surfaces in current base asset
			CSurfaceGroup::iterator it = m_SurfaceGroup.begin();
			for (; it != m_SurfaceGroup.end(); it++)
			{
				long	lSurfaceID = it->first;

			// Get vola surface for specified mode
			_RecordsetPtr spRS;
			_Recordset* pRs = NULL;

				__CHECK_HRESULT4(m_spDS->raw_GetSurfaceVolatilities( lSurfaceID, m_enOptType, &pRs), _T("Error getting surface data."));
			 spRS =	 _RecordsetPtr(pRs, false);

				__CHECK_HRESULT4( SetSurfaceData( spRS, lSurfaceID ), _T("Error setting surface data.") );
			spRS->Close();
			pRs = NULL;

				__CHECK_HRESULT4(m_spDS->raw_GetVolatilitySurfaceProps( lSurfaceID, &pRs ), _T("Error getting surface properties."));
			spRS =	 _RecordsetPtr(pRs, false);

				__CHECK_HRESULT4( SetSurfaceProps( spRS, lSurfaceID ), _T("Error setting surface properties.") );
			spRS->Close();
		}

		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__, __FUNCDNAME__,__LINE__);
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::SetSurfaceData( _RecordsetPtr& spData, long lSurfaceID)
{
	try
	{
		ObjectLock lock(this);

		// For each record
		if( SUCCEEDED( spData->raw_MoveFirst() ) )
		{
			for( ; !spData->BOF && !spData->Eof; spData->MoveNext() )
			{
				lSurfaceID = spData->Fields->GetItem(L"iVolaSurfaceDataID")->Value;
				m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID).SetPoint( 
					spData->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value,
								 spData->Fields->GetItem(L"fStrike")->Value,
								 spData->Fields->GetItem(L"dtExpDate")->Value,
								 spData->Fields->GetItem(L"fVolatility")->Value,
								 spData->Fields->GetItem(L"iIsBasePoint")->Value
							   );
			}

			m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID).UpdateSurfaceStatus();
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::SetSurfaceProps( _RecordsetPtr& spProps, long lSurfaceID )
{
	_variant_t vtValue;
	
	try
	{
		ObjectLock lock(this);
		if((!spProps->Eof )&&( SUCCEEDED( spProps->MoveFirst() ) ))
		{
			CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

			vtValue = spProps->Fields->GetItem(L"fBaseUnderlinePrice")->Value;
			if( vtValue.vt != VT_NULL )
				dhData.SetUnderlinePrice( vtValue );
			else
			dhData.SetUnderlinePrice( 0.0f );

			vtValue = spProps->Fields->GetItem(L"fSmileAccelerator")->Value;
			if( vtValue.vt != VT_NULL )
				dhData.SetSmileAccelerator( vtValue );
			else
				dhData.SetSmileAccelerator( 0.0f );

			vtValue = spProps->Fields->GetItem(L"fInterpolationFactor")->Value;
			if( vtValue.vt != VT_NULL )
				dhData.SetInterpolationFactor( 1.0f - vtValue.dblVal );
			else
				dhData.SetInterpolationFactor( 0.0f );

			vtValue = spProps->Fields->GetItem(L"bIsDiscreteAcceleration")->Value;
			if( vtValue.vt != VT_NULL )
				dhData.SetDiscreteAcceleration( vtValue );
			else
				dhData.SetDiscreteAcceleration( false );

			vtValue = spProps->Fields->GetItem(L"bIsPriceOverride")->Value;
			if( vtValue.vt != VT_NULL )
				dhData.SetPriceOverride( vtValue );
			else
				dhData.SetPriceOverride( false );
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::GetSurfaceData( _RecordsetPtr& spData, long lSurfaceID )
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


		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);
		// For each expiration
		for( long j = 0; j < dhData.GetExpirationsCount(); j++ )
		{
			DATE dtExpDate = dhData.GetExpirationDate( j );

			// For each strike
			for( long i = 0; i < dhData.GetPointsCount( dtExpDate ); i++ )
			{
				EVolaItemStatus enStatus = dhData.GetPointStatus( dtExpDate, i );
				if( enStatus == Untouched )
					continue;

				spData->AddNew();
				spData->Fields->GetItem(L"iCustomStrikeSkewPointID")->PutValue( dhData.GetPointID( dtExpDate, i ) );
				spData->Fields->GetItem(L"fStrike")->PutValue( dhData.GetPointStrike( dtExpDate, i ) );
				spData->Fields->GetItem(L"fVolatility")->PutValue( dhData.GetPointVolatility( dtExpDate, i ) );
				spData->Fields->GetItem(L"dtExpDate")->PutValue( dtExpDate );
				spData->Fields->GetItem(L"iIsBasePoint")->PutValue( dhData.GetIsBasePoint( dtExpDate, i ) );
				spData->Fields->GetItem(L"iStatus")->PutValue( (long)enStatus );
				spData->Update();
			}
		}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
//		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::GetSurfaceProps( _RecordsetPtr& spProps, long lSurfaceID )
{
	try
	{
		ObjectLock lock(this);

		// Create recordset instance
		if( !spProps.GetInterfacePtr() )
			__CHECK_HRESULT( spProps.CreateInstance( CLSID_Recordset ), _T("Error creating recordset instance.") );

		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);
		// Open disconnected recordset
		spProps->CursorLocation = adUseClient;
		spProps->LockType = adLockOptimistic;
		spProps->Fields->Append( L"fBaseUnderlinePrice",   adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"fSmileAccelerator",     adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"fInterpolationFactor",  adDouble,  8, adFldUnspecified );
		spProps->Fields->Append( L"bDiscreteAcceleration", adInteger, 4, adFldUnspecified );
		spProps->Fields->Append( L"bPriceOverride",		   adInteger, 4, adFldUnspecified );
		spProps->Fields->Append( L"vcSymbolName",		   adVarChar, 32, adFldUnspecified );
		__CHECK_HRESULT( spProps->Open( L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified ), _T("Error opening recordset.") );

		// Put data into recordset
		spProps->AddNew();
		spProps->Fields->GetItem(L"fBaseUnderlinePrice")->PutValue( dhData.GetUnderlinePrice() );
		spProps->Fields->GetItem(L"fSmileAccelerator")->PutValue( dhData.GetSmileAccelerator() );
		spProps->Fields->GetItem(L"fInterpolationFactor")->PutValue( 1.0 - dhData.GetInterpolationFactor() );
		spProps->Fields->GetItem(L"bDiscreteAcceleration")->PutValue( dhData.GetDiscreteAcceleration() );
		spProps->Fields->GetItem(L"bPriceOverride")->PutValue( dhData.GetPriceOverride() );
		spProps->Fields->GetItem(L"vcSymbolName")->PutValue( m_bsSymbolName );
		spProps->Update();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::Save()
{
	try
	{
		ObjectLock lock(this);

		//if( m_nSurfaceID == -1 )
		//	return S_OK;

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		//save data from all surfaces
		CSurfaceGroup::iterator it = m_SurfaceGroup.begin();
		for (; it != m_SurfaceGroup.end(); it++)
		{
			long	lSurfaceID = it->first;

		// Save properties
		_RecordsetPtr spRS;
			__CHECK_HRESULT4( GetSurfaceProps( spRS, lSurfaceID), _T("Error collecting volatility surface properties.") );

			__CHECK_HRESULT4( m_spDS->raw_SaveVolatilitySurfaceProps( lSurfaceID, spRS ), _T("Error saving volatility surface properties.") );
		spRS->Close();

		// Save data
			__CHECK_HRESULT4( GetSurfaceData( spRS, lSurfaceID ), _T("Error collecting volatility surface data.") );
			__CHECK_HRESULT4( m_spDS->raw_SaveSurfaceVolatilities( lSurfaceID, m_enOptType, spRS ), _T("Error saving volatility surface data.") );

		spRS->Close();
	}
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}

	
STDMETHODIMP CSymbolVolatility::PublishChanges( _RecordsetPtr& spRS, long lSurfaceID )
{
	try
	{
		ObjectLock  lock(this);

		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

		m_spVMESurface->SurfaceID = lSurfaceID;
		m_spVMESurface->OptType   = m_enOptType;
		m_spVMESurface->UnderlinePrice   = dhData.GetUnderlinePrice();
		m_spVMESurface->SmileAccelerator = dhData.GetSmileAccelerator();
		m_spVMESurface->InterpolationFactor = dhData.GetInterpolationFactor();
		m_spVMESurface->PriceOverride = dhData.GetPriceOverride() ? VARIANT_TRUE : VARIANT_FALSE;
		m_spVMESurface->DiscreteAcceleration = dhData.GetDiscreteAcceleration() ? VARIANT_TRUE : VARIANT_FALSE;
		m_spVMESurface->Symbol = m_bsSymbolName;
		m_spVMESurface->Points = spRS->Clone( adLockReadOnly );
		m_pSource->PublishChanges(spRS, m_spVMESurface );
		//m_spPubManager->PubVola( m_spSymbolObject, m_spVMESurface );
	}
	catch( const _com_error& e )
	{
		_bstr_t message(L"Error calling publishing manager. ");
		message += CComErrorWrapper::ErrorDescription( e );
		REPORT_ERR_TO_MAIL (e)
		//return Error( (PTCHAR)message, IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", message, __FILE__,__FUNCDNAME__,__LINE__);

	}

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// ISupportErrorInfo implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CSymbolVolatility::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVSSymbolVolatility,
		&IID_IVSTargetVolatility
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// ISymbolVolatility implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CSymbolVolatility::get_Volatility(double UnderlinePrice, double Strike, DATE ExpDate, LONG SurfaceID, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		long lSurfaceID = SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);
		*pVal = dhData.GetPointVolatility( ExpDate, Strike, UnderlinePrice );
	}
	catch( const _com_error& )
	{
		*pVal = -1;
		return S_FALSE;
	}

	return S_OK;
}


STDMETHODIMP CSymbolVolatility::put_Volatility(double UnderlinePrice, double Strike, DATE ExpDate, LONG SurfaceID, double newVal)
{
	try
	{
		ObjectLock lock(this);

		long lSurfaceID = SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

		if( m_bEnableEditing == VARIANT_FALSE )
			dhData.AddPoint( ExpDate, Strike, newVal, false, true );
		else
		{
			bool bPriceChanged = false;

			dhData.AddPoint( ExpDate, Strike, newVal, true, false );

			double dUnderlinePrice = dhData.GetUnderlinePrice();
			if( 0.0 == dUnderlinePrice )
			{
				dUnderlinePrice = UnderlinePrice;
				dhData.SetUnderlinePrice( dUnderlinePrice );

				bPriceChanged = true;
			}
			
			if(m_bIsSimulated)
				return S_OK;
			
			if( lSurfaceID == -1 )
				return S_OK;

			// Check data source
			if( !m_spDS.GetInterfacePtr() )
				CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

			_RecordsetPtr rsData;
			if( bPriceChanged )
			{
				__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
				__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
				rsData->Close();
			}
			
			// Save data
			__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
			__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

			// Publish changes
			__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
			
			rsData->Close();
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


/////////////////////////////////////////////////////////////////////////////
// _IOnMessages implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatility::OnVMESurface(/*[in]*/IDispatch* Symbol, /*[in]*/IDispatch* Data)
{
	if(m_bIsSimulated)
		return S_OK;

	try
	{
		ObjectLock lock(this);

		IVMESurfacePtr spData = Data;
		
		long lSurfaceID = spData->SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

		// Set properties
		dhData.SetUnderlinePrice( spData->UnderlinePrice );
		dhData.SetInterpolationFactor( spData->InterpolationFactor );
		dhData.SetSmileAccelerator( spData->SmileAccelerator );
		dhData.SetPriceOverride( spData->PriceOverride == VARIANT_TRUE ? true : false );
		dhData.SetDiscreteAcceleration( spData->DiscreteAcceleration == VARIANT_TRUE ? true : false );

		// Import points
		_RecordsetPtr spPoints = spData->Points;
		if( spPoints != NULL && SUCCEEDED( spPoints->raw_MoveFirst() ) )
		{
			//CObjectLock dataLock( &dhData );

			for( ; !spPoints->BOF && !spPoints->Eof; spPoints->MoveNext() )
				dhData.ImportPoint( spPoints->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value,
									spPoints->Fields->GetItem(L"fStrike")->Value,
									spPoints->Fields->GetItem(L"dtExpDate")->Value,
									spPoints->Fields->GetItem(L"fVolatility")->Value,
									spPoints->Fields->GetItem(L"iIsBasePoint")->Value,
									spPoints->Fields->GetItem(L"iStatus")->Value
								  );
		}

		if( m_bEnableCache == VARIANT_TRUE )
			Save();

		dhData.UpdateSurfaceStatus();
		if( m_bEnableEvents == VARIANT_TRUE )
			m_pSource->OnSurfaceChanged( m_bsSymbolName );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		ATLTRACE( (PTCHAR)CComErrorWrapper::ErrorDescription( e ) );
		//return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

STDMETHODIMP CSymbolVolatility::get_TargetVolatility(double UnderlinePrice, double Strike, DATE expDate, double *pVal)
{
	if(!pVal)
		return E_POINTER;

	double dA1 = 5.;
	double dA2 = 4.;
	double dB1 = -3.;
	double dB2 = 10.;

	double dUndPriceSQ = UnderlinePrice*UnderlinePrice;
	
	double dC1= 30.-( dA1*dUndPriceSQ+dB1*UnderlinePrice);
	double dC2= 30.-( dA2*dUndPriceSQ+dB2*UnderlinePrice);

	if(Strike<=UnderlinePrice)
		*pVal = dA1*Strike*Strike+dB1*Strike+dC1;
	else
		*pVal = dA2*Strike*Strike+dB2*Strike+dC2;

	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilityShift(DATE expDate, double fShift, LONG SurfaceID)
{
	try
	{
		long	lSurfaceID = SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

		dhData.SetTimeSkewVolatilityDelta(expDate, fShift);

		if(m_bIsSimulated)
			return S_OK;

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		_RecordsetPtr rsData;
		__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
		__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
		rsData->Close();
		
		// Save data
		__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
		
		rsData->Close();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
	//	return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}
	
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilitySet(DATE expDate, double newValue, LONG SurfaceID)
{
	try
	{
		long lSurfaceID = SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);
		dhData.SetTimeSkewVolatility(expDate, newValue);

		if(m_bIsSimulated)
			return S_OK;

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		_RecordsetPtr rsData;
		__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
		__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
		rsData->Close();
		
		// Save data
		__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
		
		rsData->Close();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
	//	return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}
	
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilitySetAll(double newValue, LONG SurfaceID)
{
	try
	{
		long lSurfaceID = SurfaceID;
		CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

		dhData.SetTimeSkewVolatilityAll(newValue);

		if(m_bIsSimulated)
			return S_OK;

		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		_RecordsetPtr rsData;
		__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
		__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
		rsData->Close();
		
		// Save data
		__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
		__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
		
		rsData->Close();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL (e)
//		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}
	
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilitySetAllByExp(LPSAFEARRAY* saData, LONG SurfaceID)
{
	if(saData == NULL || *saData == NULL)
		return E_POINTER;

	ExpiryVolaData* pData;

	UINT uiDim = SafeArrayGetDim(*saData);
	if(uiDim == 1)
	{
		HRESULT hr = SafeArrayAccessData(*saData, (PVOID*)&pData);
		if(FAILED(hr)) return hr;

		try
		{
			long lSurfaceID = SurfaceID;
			CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);

			for(ULONG iIndex = 0; iIndex < (*saData)->rgsabound[0].cElements; iIndex++)
			{
				const ExpiryVolaData& evData = pData[iIndex];
				dhData.SetTimeSkewVolatility(evData.Expiry, evData.Vola);
			}

			if(m_bIsSimulated)
                        {
                                SafeArrayUnaccessData(*saData);
				return S_OK;
                        }

			// Check data source
			if( !m_spDS.GetInterfacePtr() )
				CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

			_RecordsetPtr rsData;
			__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
			__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
			rsData->Close();
			
			// Save data
			__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
			__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

			// Publish changes
			__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
			
			rsData->Close();
		}
		catch( const _com_error& e )
		{
			REPORT_ERR_TO_MAIL(e)
			SafeArrayUnaccessData(*saData);
			return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		}

		SafeArrayUnaccessData(*saData);
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilitySetAllByExpAndStrike(LPSAFEARRAY* saData, LONG SurfaceID)
{
	if(saData == NULL || *saData == NULL)
		return E_POINTER;

	ExpiryStrikeVolaData* pData;

	UINT uiDim = SafeArrayGetDim(*saData);
	if(uiDim == 1)
	{
		HRESULT hr = SafeArrayAccessData(*saData, (PVOID*)&pData);
		if(FAILED(hr)) return hr;

		try
		{
			long lSurfaceID = SurfaceID;
			CDataHolder& dhData = m_SurfaceGroup.GetDataHolderBySurfaceID(lSurfaceID);
			for(ULONG iIndex = 0; iIndex < (*saData)->rgsabound[0].cElements; iIndex++)
			{
				const ExpiryStrikeVolaData& evData = pData[iIndex];
				dhData.AddPoint(evData.Expiry, evData.Strike, evData.Vola, true, false);
			}

			if(m_bIsSimulated)
                        {
                                SafeArrayUnaccessData(*saData);
				return S_OK;
                        }

			// Check data source
			if( !m_spDS.GetInterfacePtr() )
				CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

			_RecordsetPtr rsData;
			__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
			__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
			rsData->Close();
			
			// Save data
			__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
			__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

			// Publish changes
			__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );
			
			rsData->Close();
		}
		catch( const _com_error& e )
		{
			SafeArrayUnaccessData(*saData);
			REPORT_ERR_TO_MAIL (e)
			return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVSSymbolVolatility, e.Error() );
		}

		SafeArrayUnaccessData(*saData);
	}
	else
	{
		return E_INVALIDARG;
	}
	
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
/*STDMETHODIMP CSymbolVolatility::SetPubManager(IPubManagerPtr spPubManger)
{
	if (m_spPubManager == NULL)
		m_spPubManager = spPubManger;
	return S_OK;
}
//////////////////////////////////////////////////////////////////////////
STDMETHODIMP CSymbolVolatility::SetSubManager(ISubManagerPtr spSubManger)
{
	if (m_spSubManager == NULL)
		m_spSubManager = spSubManger;
	return S_OK;
}*/
STDMETHODIMP CSymbolVolatility::put_SimulatedVol(VARIANT_BOOL newVal)
{
	m_bIsSimulated = newVal;
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::get_SimulatedVol(VARIANT_BOOL *pVal)
{
	if(!pVal)
		return E_POINTER;

	*pVal = m_bIsSimulated;
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::VolatilitySave(void)
{
	try
	{
		// Check data source
		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error saving volatility surface - no data source provided.") );

		CSurfaceGroup::iterator it = m_SurfaceGroup.begin();
		for (; it != m_SurfaceGroup.end(); it++)
		{
			long	lSurfaceID = it->first;

		_RecordsetPtr rsData;
			__CHECK_HRESULT( GetSurfaceProps( rsData, lSurfaceID ), _T("Error collecting volatility surface properties.") );
			__CHECK_HRESULT( m_spDS->SaveVolatilitySurfaceProps( lSurfaceID, rsData ), _T("Error saving volatility surface properties.") );
		rsData->Close();

		// Save data
			__CHECK_HRESULT( GetSurfaceData( rsData, lSurfaceID ), _T("Error collecting volatility surface data.") );
			__CHECK_HRESULT( m_spDS->SaveSurfaceVolatilities( lSurfaceID, m_enOptType, rsData ), _T("Error saving volatility surface data.") );

		// Publish changes
			__CHECK_HRESULT( PublishChanges( rsData, lSurfaceID ), _T("Error publishing changes") );

		rsData->Close();

		m_pSource->OnSimulatedSave( m_bsSymbolName );
	}
	}
	catch(const _com_error& e)
	{
		return CComErrorWrapper::SetError(e, L"VolaControl", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}
	

	return S_OK;
}

STDMETHODIMP CSymbolVolatility::GetInternalBuffer(CSurfaceGroup** ppData)
{
	if(!ppData)
		return E_POINTER;

	if(*ppData)
		return E_POINTER;

	CSurfaceGroup& dhData = m_SurfaceGroup;
	*ppData = &dhData;

	return S_OK;
}

STDMETHODIMP CSymbolVolatility::SetInternalBuffer(const CSurfaceGroup* pData)
{
	__CHECK_POINTER(pData);

	CSurfaceGroup::const_iterator it = pData->begin();
	for ( ; it!=pData->end(); it++)
	{
		CDataHolder *dhNew = new CDataHolder();
		it->second->CopyTo(dhNew);
		m_SurfaceGroup[it->first] = CDataHolderPtr(dhNew);
	}

	m_SurfaceGroup.m_SurfaceMap = pData->m_SurfaceMap;

	return S_OK;
}

STDMETHODIMP CSymbolVolatility::LoadSurfaceGroups(void)
{

	try
	{
		ObjectLock lock(this);

		if( !m_spDS.GetInterfacePtr() )
			CComErrorWrapper::ThrowError( E_PENDING, _T("Error loading volatility surface groupd - no data source provided.") );

		HRESULT hr = S_OK;
	
		if( SUCCEEDED(hr) )
		{
			// Get vola surface for specified mode
			_RecordsetPtr spRS;
			_Recordset* pRs = NULL;

			__CHECK_HRESULT4(m_spDS->raw_GetVolaSurfaceGroup( m_bsSymbolName, &pRs), _T("Error getting surface group data."));
			spRS =	 _RecordsetPtr(pRs, false);

			__CHECK_HRESULT4( SetSurfaceGroupData( spRS ), _T("Error setting surface group data.") );
			//spRS->Close();
			pRs = NULL;
		}
	}
	catch( const _com_error& e )
	{
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__, __FUNCDNAME__,__LINE__);
	}
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::SetSurfaceGroupData( _RecordsetPtr& spData )
{

	try
	{
		ObjectLock lock(this);
		//iGroupID, iBaseAsset, iRootId, vcSurfaceName, iSurfaceID, vcRootSymbol         

		// For each record
		if( SUCCEEDED( spData->raw_MoveFirst() ) )
		{
			for( ; !spData->BOF && !spData->Eof; spData->MoveNext() )

				m_SurfaceGroup.ProcessSurface(	spData->Fields->GetItem(L"vcRootSymbol")->Value,
												spData->Fields->GetItem(L"iRootID")->Value,												
												spData->Fields->GetItem(L"iSurfaceID")->Value
											 );
		}
	}
	catch( const _com_error& e )
	{
		return CComErrorWrapper::SetError(e, L"SymbolVolatility", L"", __FILE__,__FUNCDNAME__,__LINE__);
	}
	return S_OK;
}

STDMETHODIMP CSymbolVolatility::GetSurfaceByRoot(LONG RootID, LONG *pVal)
{
	if(!pVal)
		return E_POINTER;

	*pVal = m_SurfaceGroup.GetSurfaceByRoot(RootID);
	return S_OK;
}
