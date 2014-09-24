// DBLayout.cpp : Implementation of CDBLayout
#include "stdafx.h"
#include "VADBLayout.h"
#include "DBLayout.h"

/////////////////////////////////////////////////////////////////////////////
// IObjectControl implementation
/////////////////////////////////////////////////////////////////////////////

HRESULT CDBLayout::Activate()
{
	HRESULT hr = GetObjectContext(&m_spObjectContext);
	if (SUCCEEDED(hr))
		return S_OK;
	return hr;
} 


BOOL CDBLayout::CanBePooled()
{
	return FALSE;
} 


void CDBLayout::Deactivate()
{
	m_spObjectContext.Release();
} 


/////////////////////////////////////////////////////////////////////////////
// IDBLayout implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CDBLayout::get_ConnectionString(BSTR *pVal)
{
	*pVal = m_bsConnectionString.copy();

	return S_OK;
}

STDMETHODIMP CDBLayout::put_ConnectionString(BSTR newVal)
{
	m_bsConnectionString = newVal;
	
	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IVAManagement implementation
/////////////////////////////////////////////////////////////////////////////
STDMETHODIMP CDBLayout::GetDefaultSurfaceID(BSTR Symbol, long *pSurfaceID)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_ActiveSurface_Get @vcSymbolName = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbolName", adVarChar, adParamInput, 20, Symbol ) );
		spCommand->CommandTimeout = 120;
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


STDMETHODIMP CDBLayout::SaveDefaultSurfaceID(/*[in]*/BSTR Symbol, /*[in]*/long SurfaceID)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_ActiveSurface_Save @vcSymbolName = ?, @iVolaSurfaceDataID = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbolName",		  adVarChar, adParamInput, 20, Symbol ) );
 		spCommand->Parameters->Append( spCommand->CreateParameter( L"iVolaSurfaceDataID", adInteger, adParamInput, 4,  SurfaceID ) );
		spCommand->CommandTimeout = 120;
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );
	}
	catch( const _com_error& e )
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
	}

	return S_OK;
}


STDMETHODIMP CDBLayout::GetSurfaceVolatilities(long SurfaceID, EOptType OptType, _Recordset **ppRecordset)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_CustomStrikeSkewPoint_Get @iVolaSurfaceDataID = ?, @iType = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"iVolaSurfaceDataID", adInteger, adParamInput, 4, SurfaceID ) );
		spCommand->Parameters->Append( spCommand->CreateParameter( L"iType",			  adInteger, adParamInput, 4, (long)OptType ) );
		spCommand->CommandTimeout = 120;
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


STDMETHODIMP CDBLayout::SaveSurfaceVolatilities(long SurfaceID, EOptType OptType, _Recordset *pRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommandSave;
	_CommandPtr	   spCommandDel;

	try
	{
		if( SUCCEEDED( pRecordset->raw_MoveFirst() ) )
		{
			// Creating objects
			__CHECK_HRESULT( spConnection. CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
			__CHECK_HRESULT( spCommandSave.CreateInstance( CLSID_Command ),	   _T("Error creating command instance.") );
			__CHECK_HRESULT( spCommandDel. CreateInstance( CLSID_Command ),    _T("Error creating command instance.") );

			// Connect
			spConnection->CursorLocation = adUseClient;
			spConnection->ConnectionTimeout = 120;
			spConnection->CommandTimeout = 120;
			__CHECK_HRESULT( spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified ), _T("Error open connection.") );

			// Create save command with parameters
			spCommandSave->CommandText = L"usp_CustomStrikeSkewPoint_Save @iCustomStrikeSkewPointID = ?, @iVolaSurfaceDataID = ?, @iType = ?, @fStrike = ?, @fVolatility = ?, @dtExpDate = ?, @iIsBasePoint = ?";
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"iCustomStrikeSkewPointID", adInteger, adParamInput, 4 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"iVolaSurfaceDataID",		adInteger, adParamInput, 4, SurfaceID ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"iType",					adInteger, adParamInput, 4, (long)OptType ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"fStrike",				    adDouble,  adParamInput, 8 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"fVolatility",				adDouble,  adParamInput, 8 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"dtExpDate",				adDate,	   adParamInput, 4 ) );
			spCommandSave->Parameters->Append( spCommandSave->CreateParameter( L"iIsBasePoint",				adInteger, adParamInput, 4 ) );
			spCommandSave->ActiveConnection = spConnection;

			// Create delete command with parameters
			spCommandDel->CommandText = L"usp_CustomStrikeSkewPoint_Del @iCustomStrikeSkewPointID = ?";
			spCommandDel->Parameters->Append( spCommandDel->CreateParameter( L"iCustomStrikeSkewPointID", adInteger, adParamInput, 4 ) );
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
					spCommandDel->Parameters->GetItem(L"iCustomStrikeSkewPointID")->PutValue( nPointID );
					
					// Execute command
					spCommandDel->Execute( &vtRowsAfected, &vtMissing, adCmdText );
				}
				else
				{
					// Set parameters values
					if( nPointID <= 0 )
					{
						_variant_t vtNull;
						vtNull.ChangeType( VT_NULL );

						spCommandSave->Parameters->GetItem(L"iCustomStrikeSkewPointID")->PutValue( vtNull );
					}
					else
						spCommandSave->Parameters->GetItem(L"iCustomStrikeSkewPointID")->PutValue( nPointID );
					spCommandSave->Parameters->GetItem(L"fStrike")->PutValue( pRecordset->Fields->GetItem(L"fStrike")->Value );
					spCommandSave->Parameters->GetItem(L"fVolatility")->PutValue( pRecordset->Fields->GetItem(L"fVolatility")->Value );
					spCommandSave->Parameters->GetItem(L"dtExpDate")->PutValue( pRecordset->Fields->GetItem(L"dtExpDate")->Value );
					spCommandSave->Parameters->GetItem(L"iIsBasePoint")->PutValue( pRecordset->Fields->GetItem(L"iIsBasePoint")->Value );
					
					// Execute command
					spCommandSave->Execute( &vtRowsAfected, &vtMissing, adCmdText );
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

		return CComErrorWrapper::SetError(e,L"DBLayout",L"", __FILE__,__FUNCTION__, __LINE__);

	}

	return S_OK;
}


STDMETHODIMP CDBLayout::GetVolatilitySurfaceProps(long SurfaceID, _Recordset **ppRecordset)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_VolaSurfaceData_Get @iContractID = null, @iVolaSurfaceDataID = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"@iVolaSurfaceDataID", adInteger, adParamInput, 4, SurfaceID ) );
		spCommand->CommandTimeout = 120;
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		// Return recordset in output parameter
		*ppRecordset = spRecordset.Detach();
	}
	catch( const _com_error& e )
	{
		return CComErrorWrapper::SetError(e,L"DBLayout",L"", __FILE__, __FUNCTION__, __LINE__);
	}

	return S_OK;
}


STDMETHODIMP CDBLayout::SaveVolatilitySurfaceProps(long SurfaceID, _Recordset *pRecordset)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;

	try
	{
		if( SUCCEEDED( pRecordset->raw_MoveFirst() ) )
		{
			// Creating objects
			__CHECK_HRESULT( spConnection.CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
			__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );

			// Connect
			spConnection->CursorLocation = adUseClient;
			spConnection->ConnectionTimeout = 120;
			spConnection->CommandTimeout = 120;
			spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

			// Create command with parameters
			spCommand->CommandText = L"usp_VolaSurfaceData_Save @iVolaSurfaceDataID = ?, @fBaseUnderlinePrice = ?, @fSmileAccelerator = ?, @fInterpolationFactor = ?, @bIsDiscreteAcceleration = ?, @bIsPriceOverride = ?";
			spCommand->Parameters->Append( spCommand->CreateParameter( L"iVolaSurfaceDataID",     adInteger, adParamInput, 4, SurfaceID ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fBaseUnderlinePrice",    adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fBaseUnderlinePrice")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fSmileAccelerator",      adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fSmileAccelerator")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fInterpolationFactor",   adDouble,  adParamInput, 8, pRecordset->Fields->GetItem(L"fInterpolationFactor")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"bDiscreteAcceleration",  adInteger, adParamInput, 4, pRecordset->Fields->GetItem(L"bDiscreteAcceleration")->Value ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"bPriceOverride",         adInteger, adParamInput, 4, pRecordset->Fields->GetItem(L"bPriceOverride")->Value ) );
			spCommand->CommandTimeout = 120;
			spCommand->ActiveConnection = spConnection;

			// Start transaction
			if( !m_spObjectContext.GetInterfacePtr() )
				spConnection->BeginTrans();

			// Execute command
			_variant_t vtRowsAfected;
			spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

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
STDMETHODIMP CDBLayout::GenerateDefaultVolatilities(BSTR Symbol, EOptType OptType, double dDefVola)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Check whether default points generation required
		spCommand->CommandText = L"usp_CustomVolatilityPointIsExist2_Get @vcSymbolName = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbolName", adVarChar, adParamInput, 20, Symbol ) );
		spCommand->CommandTimeout = 120;
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		if( SUCCEEDED(spRecordset->raw_MoveFirst()) )
		{
			_CommandPtr	spGenerateCommand;

			__CHECK_HRESULT( spGenerateCommand.CreateInstance( CLSID_Command ), _T("Error creating command instance.") );
			spGenerateCommand->CommandText = L"usp_CustomStrikeSkewDefaultPoint2_Save @iContractID = ?, @iExpiryID = ?, @iType = ?, @fVolatility = ?";
			spGenerateCommand->Parameters->Append( spCommand->CreateParameter( L"iContractID", adInteger, adParamInput, 4 ) );
			spGenerateCommand->Parameters->Append( spCommand->CreateParameter( L"iExpiryID",   adInteger, adParamInput, 4 ) );
			spGenerateCommand->Parameters->Append( spCommand->CreateParameter( L"iType",       adInteger, adParamInput, 4 ) );
			spGenerateCommand->Parameters->Append( spCommand->CreateParameter( L"fVolatility", adDouble,  adParamInput, 8 ) );
			spGenerateCommand->ActiveConnection = spConnection;

			// For every found expiration
			while( !spRecordset->Eof )
			{
				spGenerateCommand->Parameters->GetItem(L"iContractID")->PutValue( spRecordset->Fields->GetItem(L"iContractID")->Value );
				spGenerateCommand->Parameters->GetItem(L"iExpiryID")->PutValue( spRecordset->Fields->GetItem(L"iExpiryID")->Value );
				spGenerateCommand->Parameters->GetItem(L"iType")->PutValue( (long)OptType );
				spGenerateCommand->Parameters->GetItem(L"fVolatility")->PutValue( dDefVola );

				spGenerateCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

				spRecordset->MoveNext();
			}
		}
		spRecordset->Close();
	}
	catch( const _com_error& e )
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
	}

	return S_OK;
}*/


STDMETHODIMP CDBLayout::GetDualQuadraticProps(BSTR Symbol, long ContractType,DATE ExpDate, _Recordset** ppRS)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_DualQuadraticParams_Get";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbol", adVarChar, adParamInput, 20, Symbol ) );
		spCommand->Parameters->Append( spCommand->CreateParameter( L"dtExpiry", adDate, adParamInput, 4, ExpDate ) );
		spCommand->Parameters->Append( spCommand->CreateParameter( L"iContractTypeID", adInteger, adParamInput, 4, (long)ContractType ) );
		spCommand->CommandTimeout = 120;
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdStoredProc );

		// Return recordset in output parameter
		*ppRS = spRecordset.Detach();
	}
	catch( const _com_error& e )
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
	}

	return S_OK;
}

STDMETHODIMP CDBLayout::SaveDualQuadraticProps(BSTR Symbol,long ContractType, DATE ExpDate, _Recordset* pRS)
{
	_ConnectionPtr spConnection;
	_CommandPtr	   spCommand;
	
	try
	{
		if( SUCCEEDED( pRS->raw_MoveFirst() ) )
		{
			// Creating objects
			__CHECK_HRESULT( spConnection. CreateInstance( CLSID_Connection ), _T("Error creating connection instance.") );
			__CHECK_HRESULT( spCommand.CreateInstance( CLSID_Command ),	   _T("Error creating command instance.") );

			// Connect
			spConnection->CursorLocation = adUseClient;
			spConnection->ConnectionTimeout = 120;
			spConnection->CommandTimeout = 120;
			__CHECK_HRESULT( spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified ), _T("Error open connection.") );

			// Create save command with parameters
			spCommand->CommandText = L"usp_DualQuadraticParams_Save";
			spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbol", adVarChar, adParamInput, 20, Symbol ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"dtExpiry", adDate, adParamInput, 4, ExpDate ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"iContractTypeID", adInteger, adParamInput, 4, (long)ContractType ) );
			spCommand->CommandTimeout = 120;
			spCommand->ActiveConnection = spConnection;


			double dCoefA1 = pRS->Fields->GetItem(L"dCoefA1")->Value;
			double dCoefB1 = pRS->Fields->GetItem(L"dCoefB1")->Value;
			double dCoefA2 = pRS->Fields->GetItem(L"dCoefA2")->Value;
			double dCoefB2 = pRS->Fields->GetItem(L"dCoefB2")->Value;
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fCoefficientA1 ", adDouble, adParamInput, 8, dCoefA1 ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fCoefficientA2", adDouble, adParamInput, 8, dCoefA2 ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fCoefficientB1", adDouble, adParamInput, 8, dCoefB1 ) );
			spCommand->Parameters->Append( spCommand->CreateParameter( L"fCoefficientB2", adDouble, adParamInput, 8, dCoefB2 ) );

			_variant_t vtRowsAfected;
			// Execute command
			spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdStoredProc );
		}
	}
	catch( const _com_error& e )
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
	}

	return S_OK;
}
//****************************************************************************//
// return recordeset on VolaSurfaceGroup //
//****************************************************************************//
STDMETHODIMP CDBLayout::GetVolaSurfaceGroup(BSTR Symbol, _Recordset **ppRecordset)
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
		spConnection->ConnectionTimeout = 120;
		spConnection->CommandTimeout = 120;
		spConnection->Open( m_bsConnectionString, L"", L"", adConnectUnspecified );

		// Create command with parameters
		spCommand->CommandText = L"usp_SubSurface_Get null, null, null, null, @vcSymbolName = ?";
		spCommand->Parameters->Append( spCommand->CreateParameter( L"vcSymbolName", adVarChar, adParamInput, 32, Symbol ) );
		spCommand->CommandTimeout = 120;
		spCommand->ActiveConnection = spConnection;

		// Execute command
		_variant_t vtRowsAfected;
		spRecordset = spCommand->Execute( &vtRowsAfected, &vtMissing, adCmdText );

		*ppRecordset = spRecordset.Detach();
	}
	catch( const _com_error& e )
	{
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVAManagement, e.Error() );
	}

	return S_OK;
}
