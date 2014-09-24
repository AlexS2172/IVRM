// StrikeSkewVolatilities.cpp : Implementation of CStrikeSkewVolatilities
#include "stdafx.h"
#include "VME.h"
#include "StrikeSkewVolatilities.h"

/////////////////////////////////////////////////////////////////////////////
// ISupportsErrorInfo implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CStrikeSkewVolatilities::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IVMStrikeSkewVolatilities
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

HRESULT CStrikeSkewVolatilities::FinalConstruct()
{
	return S_OK;
}


void CStrikeSkewVolatilities::FinalRelease()
{
}


STDMETHODIMP CStrikeSkewVolatilities::Init( CSymbolVolatilitySurface* pSurface, 
										   	BSTR bsSymbolName,SYMBOL_TYPE enSymbolType,
											long nSurfaceID, EOptType enOptType, DATE dtExpDate )
{
	__CHECK_POINTER( pSurface );

	ObjectLock lock(this);
	
	m_pSurface	= pSurface;
	m_spSurface = pSurface;
	
	m_nSurfaceID  = nSurfaceID;
	m_enOptType   = enOptType;
	m_dtExpDate	  = dtExpDate;

	m_pData = &(pSurface->m_data);

	m_bsSymbolName = bsSymbolName;
	m_enSymbolType = enSymbolType;

	return S_OK;
}


/////////////////////////////////////////////////////////////////////////////
// IVMStrikeSkewVolatilities implementation
/////////////////////////////////////////////////////////////////////////////

STDMETHODIMP CStrikeSkewVolatilities::get_Count(long *pVal)
{
	__CHECK_POINTER( pVal );

	*pVal = m_pData->GetPointsCount( m_dtExpDate );

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_Strike(long index, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetPointStrike( m_dtExpDate, index );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_Status(double Strike, EVolaItemStatus *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetPointStatus( m_dtExpDate, Strike );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_Volatility(double Strike, double *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetPointVolatility( m_dtExpDate, Strike );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::put_Volatility(double Strike, double newVal)
{
	try
	{
		m_pData->SetPointVolatility( m_dtExpDate, Strike, newVal );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_IsATM(double Strike, VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		double dATMStrike = m_pData->GetATMStrike( m_dtExpDate );
		*pVal = dATMStrike == Strike ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_IsBasePoint(double Strike, VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetIsBasePoint( m_dtExpDate, Strike ) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::put_IsBasePoint(double Strike, VARIANT_BOOL newVal)
{
	try
	{
		m_pData->SetIsBasePoint( m_dtExpDate, Strike, newVal == VARIANT_TRUE ? true : false );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}



STDMETHODIMP CStrikeSkewVolatilities::get_BasePointsCount(long *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetBasePointsCount( m_dtExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;

	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_BasePointsInterpolation(VARIANT_BOOL *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetBasePointsInterpolation( m_dtExpDate ) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::put_BasePointsInterpolation(VARIANT_BOOL newVal)
{
	try
	{
		m_pData->SetBasePointsInterpolation( m_dtExpDate, newVal == VARIANT_TRUE ? true : false );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::get_UpdateMark(long *pVal)
{
	__CHECK_POINTER( pVal );

	try
	{
		*pVal = m_pData->GetSkewUpdateMark( m_dtExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::Reload()
{
	try
	{
		ObjectLock lock(this);

		// Get data source
		IVAManagementPtr spDS;
		m_spSurface->get_DataSource( &spDS );

		// Prepare buffer
		m_pData->ClearSkew( m_dtExpDate );

		_RecordsetPtr spRS;

		// Get vola surface for specified mode
		spRS = spDS->GetSurfaceVolatilities( m_nSurfaceID, m_enOptType );

		// For each record
		if( SUCCEEDED( spRS->raw_MoveFirst() ) )
			for( ; !spRS->BOF && !spRS->Eof; spRS->MoveNext() )
			{
				if( spRS->Fields->GetItem(L"dtExpDate")->GetValue().date != m_dtExpDate )
					continue;

				m_pData->SetPoint( spRS->Fields->GetItem(L"iCustomStrikeSkewPointID")->Value,
								   spRS->Fields->GetItem(L"fStrike")->Value,
								   spRS->Fields->GetItem(L"dtExpDate")->Value,
								   spRS->Fields->GetItem(L"fVolatility")->Value,
								   spRS->Fields->GetItem(L"iIsBasePoint")->Value
							     );
			}
		}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::Save()
{
	try
	{
		ObjectLock lock(this);

		// Create recordset instance
		_RecordsetPtr spRS;
		__CHECK_HRESULT( spRS.CreateInstance( CLSID_Recordset ), _T("Error creating recordset instance.") );

		// Open diconnected recordset
		spRS->CursorLocation = adUseClient;
		spRS->LockType = adLockOptimistic;
		spRS->Fields->Append( L"iCustomStrikeSkewPointID", adInteger, 4, adFldUnspecified );
		spRS->Fields->Append( L"fStrike",				   adDouble,  8, adFldUnspecified );
		spRS->Fields->Append( L"fVolatility",			   adDouble,  8, adFldUnspecified );
		spRS->Fields->Append( L"dtExpDate",				   adDate,    4, adFldUnspecified );
		spRS->Fields->Append( L"iIsBasePoint",			   adInteger, 4, adFldUnspecified );
		spRS->Fields->Append( L"iStatus",				   adInteger, 4, adFldUnspecified );
		__CHECK_HRESULT( spRS->Open( L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified ), _T("Error opening recordset.") );

		for( long i = 0; i < m_pData->GetPointsCount( m_dtExpDate ); i++ )
		{
			EVolaItemStatus enStatus = m_pData->GetPointStatus( m_dtExpDate, i );
			if( enStatus == Untouched )
				continue;

			// Put data into recordset
			spRS->AddNew();
			spRS->Fields->GetItem(L"iCustomStrikeSkewPointID")->PutValue( m_pData->GetPointID( m_dtExpDate, i ) );
			spRS->Fields->GetItem(L"fStrike")->PutValue( m_pData->GetPointStrike( m_dtExpDate, i ) );
			spRS->Fields->GetItem(L"dtExpDate")->PutValue( m_dtExpDate );
			spRS->Fields->GetItem(L"iIsBasePoint")->PutValue( m_pData->GetIsBasePoint( m_dtExpDate, i ) );
			spRS->Fields->GetItem(L"iStatus")->PutValue( (long)enStatus );

			if( enStatus != Deleted )
			{
				spRS->Fields->GetItem(L"fVolatility")->PutValue( m_pData->GetPointVolatility( m_dtExpDate, i ) );
			}
			spRS->Update();
		}

		// Get data source
		IVAManagementPtr spDS;
		m_spSurface->get_DataSource( &spDS );

		// Save data
		__CHECK_HRESULT( spDS->SaveSurfaceVolatilities( m_nSurfaceID, m_enOptType, spRS ), _T("Error saving volatility surface data.") );

		// Publish changes
		__CHECK_HRESULT( m_pSurface->PublishChanges( spRS ), _T("Error publishing changes") );

		// Close recordset
		spRS->Close();

		// Update points status
		m_pData->UpdateSkewStatus( m_dtExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::Add(double Strike, double Volatility, VARIANT_BOOL IsBasePoint, long *pnIndex)
{
	try
	{
		*pnIndex = m_pData->AddPoint( m_dtExpDate, Strike, Volatility, IsBasePoint == VARIANT_TRUE ? true : false );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::Remove(double Strike)
{
	try
	{
		m_pData->RemovePoint( m_dtExpDate, Strike );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::RemoveAll()
{
	try
	{
		m_pData->RemoveAllPoints( m_dtExpDate );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}


STDMETHODIMP CStrikeSkewVolatilities::ProportionalShift(double Strike, double Vola)
{
	try
	{
		m_pData->ProportionalShift( m_dtExpDate, Strike, Vola );
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

_COM_SMARTPTR_TYPEDEF(IDualQuadraticData, __uuidof(IDualQuadraticData));

STDMETHODIMP CStrikeSkewVolatilities::get_DualQuadraticParams(IDualQuadraticData** pVal)
{
	__CHECK_POINTER( pVal );

	ObjectLock lock(this);
	
	IDualQuadraticDataPtr pData;
	try
	{
		__CHECK_HRESULT( pData.CreateInstance( CLSID_DualQuadraticData ), _T("Error creating dual quadratic params.") );

		// Get data source
		IVAManagementPtr spDS;
		_RecordsetPtr spRS;

		__CHECK_HRESULT( m_spSurface->get_DataSource( &spDS ), _T("Error getting data source."));
		
		long lSymbolType;
		switch( m_enSymbolType )
		{
			case enStIndex:
				lSymbolType = 1;
				break;
			case enStStock:
				lSymbolType = 2;
				break;
			case enStOption:
				lSymbolType = 3;
				break;
			default:
				lSymbolType = 2;
		}
		
		spRS = spDS->GetDualQuadraticProps(m_bsSymbolName,lSymbolType,m_dtExpDate);

		if(!spRS->RecordCount)
		{
			__CHECK_HRESULT(pData->put_ATMVola(m_pData->GetATMVola(m_dtExpDate)) ,_T("Error saving ATM Vola."));
			__CHECK_HRESULT(pData->put_CoefA1(0.02),_T("Error saving A1 Coeff."));		
			__CHECK_HRESULT(pData->put_CoefA2(0.02),_T("Error saving A2 Coeff."));
			__CHECK_HRESULT(pData->put_CoefB1(0.),_T("Error saving B1 Coeff."));
			__CHECK_HRESULT(pData->put_CoefB2(0.),_T("Error saving B2 Coeff."));
		}
		else
		{
			__CHECK_HRESULT( spRS->raw_MoveFirst() ,_T("Error while Recordset MoveFirst.")) ;		

			__CHECK_HRESULT(pData->put_ATMVola(m_pData->GetATMVola(m_dtExpDate)),_T("Error saving ATM Vola."));
			__CHECK_HRESULT(pData->put_CoefA1(spRS->Fields->GetItem(L"fCoefficientA1")->GetValue().dblVal),_T("Error saving A1 Coeff."));		
			__CHECK_HRESULT(pData->put_CoefA2(spRS->Fields->GetItem(L"fCoefficientA2")->GetValue().dblVal),_T("Error saving A2 Coeff."));
			__CHECK_HRESULT(pData->put_CoefB1(spRS->Fields->GetItem(L"fCoefficientB1")->GetValue().dblVal),_T("Error saving B1 Coeff."));
			__CHECK_HRESULT(pData->put_CoefB2(spRS->Fields->GetItem(L"fCoefficientB2")->GetValue().dblVal),_T("Error saving B2 Coeff."));
		}

		*pVal = pData.GetInterfacePtr();

		if( *pVal )
			(*pVal)->AddRef();
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}

	return S_OK;
}

STDMETHODIMP CStrikeSkewVolatilities::putref_DualQuadraticParams(IDualQuadraticData* newVal)
{
	__CHECK_POINTER( newVal );
	ObjectLock lock(this);

	try
	{
		_RecordsetPtr spRS;
		__CHECK_HRESULT( spRS.CreateInstance( CLSID_Recordset ), _T("Error creating recordset instance.") );
		// Get data source
		IVAManagementPtr spDS;
		__CHECK_HRESULT( m_spSurface->get_DataSource( &spDS ), _T("Error getting data source."));


		double dATMVola,dCoefA1,dCoefA2,dCoefB1,dCoefB2;
		newVal->get_ATMVola(&dATMVola);
		newVal->get_CoefA1(&dCoefA1);
		newVal->get_CoefA2(&dCoefA2);
		newVal->get_CoefB1(&dCoefB1);
		newVal->get_CoefB2(&dCoefB2);

		// Open diconnected recordset
		spRS->CursorLocation = adUseClient;
		spRS->LockType = adLockOptimistic;
		spRS->Fields->Append( L"dCoefA1", adDouble, 8, adFldUnspecified );
		spRS->Fields->Append( L"dCoefB1", adDouble, 8, adFldUnspecified );
		spRS->Fields->Append( L"dCoefA2", adDouble, 8, adFldUnspecified );
		spRS->Fields->Append( L"dCoefB2", adDouble, 8, adFldUnspecified );
			
		__CHECK_HRESULT( spRS->Open( L"data source=none", vtMissing, adOpenUnspecified, adLockUnspecified, adCmdUnspecified ), _T("Error opening recordset.") );

		// Put data into recordset
		__CHECK_HRESULT(spRS->AddNew(), _T("Error adding new row in recordset."));
		spRS->Fields->GetItem(L"dCoefA1")->PutValue( dCoefA1 );
		spRS->Fields->GetItem(L"dCoefA2")->PutValue( dCoefA2 );
		spRS->Fields->GetItem(L"dCoefB1")->PutValue( dCoefB1 );
		spRS->Fields->GetItem(L"dCoefB2")->PutValue( dCoefB2 );

		__CHECK_HRESULT(spRS->Update(), _T("Error updating recordset."));

		long lSymbolType;
		switch( m_enSymbolType )
		{
			case enStIndex:
				lSymbolType = 1;
				break;
			case enStStock:
				lSymbolType = 2;
				break;
			case enStOption:
				lSymbolType = 3;
				break;
			default:
				lSymbolType = 2;
		}

		__CHECK_HRESULT(spDS->SaveDualQuadraticProps(m_bsSymbolName,lSymbolType,m_dtExpDate,spRS),_T("Error saving dual quadratic params in DB."));
		
		// Close recordset
		__CHECK_HRESULT(spRS->Close(),_T("Error closing recordset."));
	}
	catch( const _com_error& e )
	{
		REPORT_ERR_TO_MAIL(e)
		//return Error( (PTCHAR)CComErrorWrapper::ErrorDescription( e ), IID_IVMStrikeSkewVolatilities, e.Error() );
		return CComErrorWrapper::SetError(e, L"StrikeSkewVolatilities", L"", __FILE__,__FUNCDNAME__,__LINE__);;
	}
		
	return S_OK;
}
