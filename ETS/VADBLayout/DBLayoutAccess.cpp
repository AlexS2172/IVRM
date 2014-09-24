// DBLayoutAccess.cpp : Implementation of CDBLayout
#include "stdafx.h"
#include "VADBLayout.h"
#include "DBLayoutAccess.h"

/////////////////////////////////////////////////////////////////////////////
// IObjectControl implementation
/////////////////////////////////////////////////////////////////////////////

HRESULT CDBLayoutAccess::Activate()
{
	HRESULT hr = GetObjectContext(&m_spObjectContext);
	if (SUCCEEDED(hr))
		return S_OK;
	return hr;
} 


BOOL CDBLayoutAccess::CanBePooled()
{
	return FALSE;
} 


void CDBLayoutAccess::Deactivate()
{
	m_spObjectContext.Release();
} 


/////////////////////////////////////////////////////////////////////////////
// IDBLayout implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDBLayoutAccess::get_ConnectionString(BSTR *pVal)
{
	*pVal = m_bsConnectionString.copy();

	return S_OK;
}

STDMETHODIMP CDBLayoutAccess::put_ConnectionString(BSTR newVal)
{
	m_bsConnectionString = newVal;
	
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IVAManagement implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDBLayoutAccess::GetDefaultSurfaceID(BSTR Symbol, long *pSurfaceID)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;
	_RecordsetPtr  spRecordset;

	try
		{
		// Creating objects
		__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
		__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

		// Connect
		spConnection->CursorLocation = adUseClient;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText =
					L" select vsd.iVolaSurfaceDataID as iVolaSurfaceDataID"
					L" from VolaSurfaceDataView AS vsd"
					L" inner join ContractView AS c on vsd.iContractID = c.iContractID"
					L" where c.vcSymbol = IN_vcSymbol" 
					L" and vsd.tiIsDefaultSurface = 1";

		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_vcSymbol", adVarChar, adParamInput, 20, Symbol ) );
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		if( SUCCEEDED( spRecordset->raw_MoveFirst() ) )
			*pSurfaceID = spRecordset->Fields->GetItem(L"iVolaSurfaceDataID")->GetValue();
		else
			return Error( _T("No active surface found."), IID_IVAManagement, E_FAIL );
		}
	catch( const _com_error& e )
		{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


STDMETHODIMP CDBLayoutAccess::SaveDefaultSurfaceID(/*[in]*/BSTR Symbol, /*[in]*/long SurfaceID)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;

	try
		{
		// Creating objects
		__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
		__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

		// Connect
		spConnection->CursorLocation = adUseClient;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Start transaction
		if( !m_spObjectContext.GetInterfacePtr() )
			spConnection->BeginTrans();

		// Clean marks

		// Create command with parameters
		spCommand->CommandText =
			L" update VolaSurfaceData"
				L" set tiIsDefaultSurface = 0"
				L" from VolaSurfaceDataView AS vsd "
						L" inner join ContractView AS c on vsd.iContractID = c.iContractID"
				L" where c.vcSymbol = IN_vcSymbol";

		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_vcSymbol", adVarChar, adParamInput, 20, Symbol ) );
		spCommand->ActiveConnection = spConnection;
		
		// Execute command
		_variant_t vtRowsAfected;
		spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		// Set new mark
		spCommand->CommandText =
			L" update VolaSurfaceData"
				L" set tiIsDefaultSurface = 1"
				L" from VolaSurfaceData AS vsd"
					L" inner join ContractView AS c on vsd.iContractID = c.iContractID"
				L" where c.vcSymbol = IN_vcSymbol and vsd.iVolaSurfaceDataID = IN_iVolaSurfaceDataID";

		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iVolaSurfaceDataID", adInteger, adParamInput, 4,  SurfaceID ) );
		spCommand->ActiveConnection = spConnection;
		
		// Execute command
		spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		// Commit transaction
		if( !m_spObjectContext.GetInterfacePtr() )
			spConnection->CommitTrans();
		else
			m_spObjectContext->SetComplete();
		}
	catch( const _com_error& e )
		{
		// Abort transaction
		if( !m_spObjectContext.GetInterfacePtr() )
			spConnection->RollbackTrans();
		else
			m_spObjectContext->SetAbort();

		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


STDMETHODIMP CDBLayoutAccess::GetSurfaceVolatilities(long SurfaceID, EOptType OptType, _Recordset **ppRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;
	_RecordsetPtr  spRecordset;

	try
		{
		// Creating objects
		__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
		__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

		// Connect
		spConnection->CursorLocation = adUseClient;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = 
			L" select"
				L" cssp.iCustomStrikeSkewPointID AS iCustomStrikeSkewPointID,"
				L" cssp.iVolaSurfaceDataID AS iVolaSurfaceDataID,"
				L" cssp.fStrike AS fStrike,"
				L" cssp.iType AS iType,"
				L" cssp.fVolatility AS fVolatility,"
				L" DateSerial(Year(e.dtExpiryDate), Month(e.dtExpiryDate), 1) AS dtExpDate,"
				L" cssp.iIsBasePoint AS iIsBasePoint"
				L" from ((CustomStrikeSkewPointView AS cssp"
					L" inner join VolaSurfaceDataView vsd on cssp.iVolaSurfaceDataID = vsd.iVolaSurfaceDataID)"
					L" inner join ContractView AS c on vsd.iContractID = c.iContractID)"
					L" inner join ExpiryInCalendarView AS e on E.iExpCalendarID = c.iExpCalendarID and cssp.iExpiryID = e.iExpiryID"
				L" where cssp.iVolaSurfaceDataID = IN_iVolaSurfaceDataID and cssp.iType = IN_iType";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iVolaSurfaceDataID", adInteger, adParamInput, 4, SurfaceID ) );
		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iType",			  adInteger, adParamInput, 4, (long)OptType ) );
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		// Return recordset in output parameter
		*ppRecordset = spRecordset.Detach();
		}
	catch( const _com_error& e )
		{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


STDMETHODIMP CDBLayoutAccess::SaveSurfaceVolatilities(long SurfaceID, EOptType OptType, _Recordset *pRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommandIns;
	_CommandPtr	   spCommandSave;
	_CommandPtr	   spCommandDel;

	try
		{
		if( SUCCEEDED( pRecordset->raw_MoveFirst() ) )
			{
			// Creating objects
			__CHECK_HRESULT( spConnection. CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
			__CHECK_HRESULT( spCommandIns. CreateInstance( CLSID_Command ),	   _T("Error creating command instance.") );
			__CHECK_HRESULT( spCommandSave.CreateInstance( CLSID_Command ),	   _T("Error creating command instance.") );
			__CHECK_HRESULT( spCommandDel. CreateInstance( CLSID_Command ),    _T("Error creating command instance.") );

			// Connect
			spConnection->CursorLocation = adUseClient;
			__CHECK_HRESULT( spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified ), _T("Error open connection.") );

			// Create insert command with parameters
			spCommandIns->CommandText =
				L" insert into CustomStrikeSkewPoint ("
					L" iVolaSurfaceDataID,"
					L" iType,"
					L" fStrike,"
					L" fVolatility,"
					L" iExpiryID,"
					L" iIsBasePoint)"
				L" values (IN_iVolaSurfaceDataID, IN_iType, IN_fStrike, IN_fVolatility, IN_iExpiryID, IN_iIsBasePoint)";

			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_iVolaSurfaceDataID", adInteger, adParamInput, 4, SurfaceID ) );
			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_iType",				adInteger, adParamInput, 4, (long)OptType ) );
			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_fStrike",			adDouble,  adParamInput, 8 ) );
			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_fVolatility",		adDouble,  adParamInput, 8 ) );
			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_iExpiryID",			adInteger, adParamInput, 4 ) );
			spCommandIns->Parameters->Append( spCommandSave->CreateParameter( L"IN_iIsBasePoint",		adInteger, adParamInput, 4 ) );
			spCommandIns->ActiveConnection = spConnection;

			// Create save command with parameters
			spCommandSave->CommandText =
				L" update CustomStrikeSkewPoint set"
					L" iVolaSurfaceDataID = IN_iVolaSurfaceDataID,"
					L" iType = IN_iType,"
					L" fStrike = IN_fStrike,"
					L" fVolatility = IN_fVolatility,"
					L" iExpiryID = IN_iExpiryID,"
					L" iIsBasePoint = IN_iIsBasePoint"
					L" where iCustomStrikeSkewPointID = IN_iCustomStrikeSkewPointID";
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_iVolaSurfaceDataID",	adInteger, adParamInput, 4, SurfaceID ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_iType",					adInteger, adParamInput, 4, (long)OptType ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_fStrike",				adDouble,  adParamInput, 8 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_fVolatility",			adDouble,  adParamInput, 8 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_iExpiryID",				adInteger, adParamInput, 4 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_iIsBasePoint",			adInteger, adParamInput, 4 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"IN_iCustomStrikeSkewPointID", adInteger, adParamInput, 4 ) );
			spCommandSave->ActiveConnection = spConnection;

			// Create delete command with parameters
			spCommandDel->CommandText =
				L" delete CustomStrikeSkewPoint"
					L" where iCustomStrikeSkewPointID = IN_iCustomStrikeSkewPointID";
			spCommandDel->Parameters->Append( spCommandDel->CreateParameter( L"IN_iCustomStrikeSkewPointID", adInteger, adParamInput, 4 ) );
			spCommandDel->ActiveConnection = spConnection;

			// Start transaction
			if( !m_spObjectContext.GetInterfacePtr() )
				spConnection->BeginTrans();

			// For each record in provided data
			_variant_t vtRowsAfected;
			for( ; !pRecordset->Eof; pRecordset->MoveNext() )
				{
				long nPointID = pRecordset->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value;

				// Select action
				if( pRecordset->Fields->GetItem(L"iStatus")->Value.lVal == Deleted )
					{
					// Set parameters values
					spCommandDel->Parameters->GetItem(L"IN_iCustomStrikeSkewPointID")->PutValue( nPointID );
					
					// Execute command
					spCommandDel->Execute( &vtRowsAfected, &vtMissing, adCmdText );
					}
				else
					{
					vt_date dtExpiry;
					dtExpiry = (vt_date)pRecordset->Fields->GetItem(L"dtExpDate")->Value;
					long nExpiryID = (dtExpiry.get_year() - 2000) * 12 + dtExpiry.get_month();

					// Set parameters values
					if( nPointID <= 0 )
						{
						spCommandIns->Parameters->GetItem(L"IN_fStrike")->PutValue( pRecordset->Fields->GetItem(L"fStrike")->Value );
						spCommandIns->Parameters->GetItem(L"IN_fVolatility")->PutValue( pRecordset->Fields->GetItem(L"fVolatility")->Value );
						spCommandIns->Parameters->GetItem(L"IN_iExpiryID")->PutValue( nExpiryID );
						spCommandIns->Parameters->GetItem(L"IN_iIsBasePoint")->PutValue( pRecordset->Fields->GetItem(L"iIsBasePoint")->Value );

						// Execute command
						spCommandIns->Execute( &vtRowsAfected, &vtMissing, adCmdText );
						}
					else
						{
						spCommandSave->Parameters->GetItem(L"IN_fStrike")->PutValue( pRecordset->Fields->GetItem(L"fStrike")->Value );
						spCommandSave->Parameters->GetItem(L"IN_fVolatility")->PutValue( pRecordset->Fields->GetItem(L"fVolatility")->Value );
						spCommandSave->Parameters->GetItem(L"IN_iExpiryID")->PutValue( nExpiryID );
						spCommandSave->Parameters->GetItem(L"IN_iIsBasePoint")->PutValue( pRecordset->Fields->GetItem(L"iIsBasePoint")->Value );
						spCommandSave->Parameters->GetItem(L"IN_iCustomStrikeSkewPointID")->PutValue( nPointID );

						// Execute command
						spCommandSave->Execute( &vtRowsAfected, &vtMissing, adCmdText );
						}
					}
				}

			// Commit transaction
			if( !m_spObjectContext.GetInterfacePtr() )
				spConnection->CommitTrans();
			else
				m_spObjectContext->SetComplete();
			}
		}
	catch( const _com_error& e )
		{
		// Abort transaction
		if( !m_spObjectContext.GetInterfacePtr() )
			spConnection->RollbackTrans();
		else
			m_spObjectContext->SetAbort();

		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


STDMETHODIMP CDBLayoutAccess::GetVolatilitySurfaceProps(long SurfaceID, _Recordset **ppRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;
	_RecordsetPtr  spRecordset;

	try
		{
		// Creating objects
		__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
		__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

		// Connect
		spConnection->CursorLocation = adUseClient;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText =
			L" select vsd.iVolaSurfaceDataID as iVolaSurfaceDataID,"
					L" vsd.iContractID as iContractID,"
					L" vsd.fBaseUnderlinePrice as fBaseUnderlinePrice,"
					L" vsd.fSmileAccelerator as fSmileAccelerator,"
					L" vsd.fInterpolationFactor as fInterpolationFactor,"
					L" vsd.fStrikeMoneynessHi as fStrikeMoneynessHi,"
					L" vsd.fStrikeMoneynessLo as fStrikeMoneynessLo,"
					L" vsd.vcSurfaceName as vcSurfaceName,"
					L" vsd.tiIsPriceOverride AS bIsPriceOverride,"
					L" vsd.tiIsDiscreteAcceleration AS bIsDiscreteAcceleration,"
					L" vsd.tiIsDefaultSurface AS bIsDefaultSurface,"
					L" vsd.fATMVolatility as fATMVolatility,"
					L" c.vcSymbol AS vcSymbolName"
					L" from VolaSurfaceDataView as vsd"
							L" inner join ContractView as c on vsd.iContractID = c.iContractID"
					L" where vsd.iVolaSurfaceDataID = IN_iVolaSurfaceDataID";

		spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iVolaSurfaceDataID", adInteger, adParamInput, 4, SurfaceID ) );
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		// Return recordset in output parameter
		*ppRecordset = spRecordset.Detach();
		}
	catch( const _com_error& e )
		{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


STDMETHODIMP CDBLayoutAccess::SaveVolatilitySurfaceProps(long SurfaceID, _Recordset *pRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;
	_CommandPtr	   spCommandSymbol;

	_variant_t vtRowsAfected;

	try
		{
		if( SUCCEEDED( pRecordset->raw_MoveFirst() ) )
			{
			// Creating objects
			__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
			__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );
			__CHECK_HRESULT( spCommandSymbol.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

			// Connect
			spConnection->CursorLocation = adUseClient;
			spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

			// Start transaction
			if( !m_spObjectContext.GetInterfacePtr() )
				spConnection->BeginTrans();

			// Get Symbol ID
			spCommandSymbol->CommandText = L"select iContractID from ContractView where vcSymbol = IN_vcSymbol";
			spCommandSymbol->Parameters->Append( spCommand->CreateParameter( L"IN_vcSymbol", adVarChar, adParamInput, 20, pRecordset->Fields->GetItem(L"vcSymbolName")->Value ) );
			spCommandSymbol->ActiveConnection = spConnection;

			_RecordsetPtr spSymbolData = spCommandSymbol->Execute( &vtRowsAfected, &vtMissing, adCmdText );

			spSymbolData->MoveFirst();
			long nSymbolID = spSymbolData->Fields->GetItem(L"iContractID")->Value.lVal;

			// Update default status
			spCommandSymbol->CommandText = L"update VolaSurfaceData set tiIsDefaultSurface = 0 where iContractID = IN_iContractID";
			spCommandSymbol->Parameters->Delete( L"IN_vcSymbol" );
			spCommandSymbol->Parameters->Append( spCommand->CreateParameter( L"IN_iContractID", adInteger, adParamInput, 4, nSymbolID ) );
			spCommandSymbol->Execute( &vtRowsAfected, &vtMissing, adCmdText );

			// Create command with parameters
			spCommand->CommandText =
				L" update VolaSurfaceData set"
					L" fBaseUnderlinePrice = IN_fBaseUnderlinePrice,"
					L" fSmileAccelerator = IN_fSmileAccelerator,"
					L" fInterpolationFactor = IN_fInterpolationFactor,"
					L" tiIsPriceOverride = IN_tiIsPriceOverride,"
					L" tiIsDiscreteAcceleration = IN_tiIsDiscreteAcceleration,"
					L" tiIsDefaultSurface = IN_tiIsDefaultSurface"
				L" where iVolaSurfaceDataID = IN_iVolaSurfaceDataID";
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_fBaseUnderlinePrice",    adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fBaseUnderlinePrice")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_fSmileAccelerator",      adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fSmileAccelerator")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_fInterpolationFactor",   adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fInterpolationFactor")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_tiPriceOverride",        adInteger, adParamInput, 4, pRecordset->Fields->GetItem(L"bPriceOverride")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_tiDiscreteAcceleration", adInteger, adParamInput, 4, pRecordset->Fields->GetItem(L"bDiscreteAcceleration")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_tiIsDefaultSurface",     adInteger, adParamInput, 4, 1L ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iVolaSurfaceDataID",     adInteger, adParamInput, 4, SurfaceID ) );
			spCommand->ActiveConnection = spConnection;

			// Execute command
			spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );
			if( vtRowsAfected.lVal == 0L )
			{
				spCommand->CommandText =
					L" insert into VolaSurfaceData ("
						L" fBaseUnderlinePrice,"
						L" fSmileAccelerator,"
						L" fInterpolationFactor,"
						L" tiIsPriceOverride,"
						L" tiIsDiscreteAcceleration,"
						L" tiIsDefaultSurface,"
						L" iVolaSurfaceDataID,"
						L" iContractID,"
						L" vcSurfaceName )"
					L" values (IN_fBaseUnderlinePrice, IN_fSmileAccelerator, IN_fInterpolationFactor, IN_tiIsPriceOverride, IN_tiIsDiscreteAcceleration, IN_tiIsDefaultSurface, IN_iVolaSurfaceDataID, IN_iContractID, IN_vcSurfaceName )";
				spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_iContractID", adInteger,  adParamInput, 4, nSymbolID ) );
				spCommand->Parameters->Append( spCommand->CreateParameter( L"IN_vcSurfaceName",  adVarChar,  adParamInput, 250, L"<Default>" ) );

				spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );
			}

			// Commit transaction
			if( !m_spObjectContext.GetInterfacePtr() )
				spConnection->CommitTrans();
			else
				m_spObjectContext->SetComplete();
			}
		}
	catch( const _com_error& e )
		{
		// Abort transaction
		if( !m_spObjectContext.GetInterfacePtr() )
			spConnection->RollbackTrans();
		else
			m_spObjectContext->SetAbort();
		
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
		}

	return S_OK;
}


/*
STDMETHODIMP CDBLayoutAccess::GenerateDefaultVolatilities(BSTR Symbol, EOptType OptType, double dDefVola)
{
	return E_NOTIMPL;
}
*/

STDMETHODIMP CDBLayoutAccess::GetDualQuadraticProps(BSTR Symbol, long ContractType, DATE ExpDate, _Recordset** pRS)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDBLayoutAccess::SaveDualQuadraticProps(BSTR Symbol, long ContractType, DATE ExpDate, _Recordset* pRS)
{
	return E_NOTIMPL;
}

STDMETHODIMP CDBLayoutAccess::GetVolaSurfaceGroup(BSTR Symbol, _Recordset** ppRecordset)
{
	return E_NOTIMPL;
}
