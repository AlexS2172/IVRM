///////////////////////////////////
// EgLibDB.inl
// inline functions definitions of the eGAR data access library
//
///////////////////////////////////

#ifndef __EGLIB_DB_ACCESS_INL
#define __EGLIB_DB_ACCESS_INL__
DWORD __declspec(selectany) g_dwCurrentThreadId = ::GetCurrentThreadId();

//extern EgLib::CEvent g_EventStop;
namespace binary_utils
{
	inline void var_to_binary (const _variant_t & vtVal, void * pData, size_t nSize)
	{
		ATLASSERT (pData);
		if (! utils::HasValue (vtVal) ) return;
		if ( (vtVal.vt && (VT_ARRAY | VT_I1 )) ==0 )
			_com_util::CheckError (DISP_E_TYPEMISMATCH);

		HRESULT hr = ::SafeArrayLock (vtVal.parray);
		_com_util::CheckError (hr);

		size_t nDataSize = vtVal.parray->rgsabound[0].cElements;
		nSize = nSize > nDataSize ? nDataSize : nSize;

		memcpy (pData, vtVal.parray->pvData , nSize);

		hr = ::SafeArrayUnlock (vtVal.parray);
		ATLASSERT (SUCCEEDED(hr));
	}
	inline void binary_to_var (const void * pData, size_t nSize, _variant_t & rvt )
	{
		ATLASSERT (pData);
		SAFEARRAYBOUND rgBounds [1] = {{long(nSize), 0}};

		SAFEARRAY * pArray = ::SafeArrayCreate (VT_I1, 1, rgBounds);
		if (!pArray)
			_com_util::CheckError (E_OUTOFMEMORY);

		HRESULT hr = SafeArrayLock (pArray);
		ATLASSERT (SUCCEEDED (hr));

		memcpy (pArray->pvData, pData, nSize);

		hr = SafeArrayUnlock (pArray);
		ATLASSERT (SUCCEEDED (hr));

		rvt.vt = VT_ARRAY | VT_I1;
		rvt.parray = pArray;
	}
};


#ifndef EGLIB_DB_CLIENT_ONLY

//////////////////////////////////////////////////////
// CDBConnection methods

inline void CDBConnection::InitConnection (const _bstr_t & bstrConn, long lConnectionTimeout, long lCommandTimeout)
{
	m_bInTran = false;

	if(!m_bsConnectionString.length())
	{
		m_bsConnectionString = bstrConn;
		m_lConnectionTimeout = lConnectionTimeout;
		m_lCommandTimeout    = lCommandTimeout;
	}

	HRESULT hr = m_spConn.CreateInstance (__uuidof (Connection));
	_com_util::CheckError(hr);
	m_spConn->ConnectionString = bstrConn;		

	hr = m_spCheckConn.CreateInstance (__uuidof (Connection));
	_com_util::CheckError(hr);
	m_spCheckConn->ConnectionString = bstrConn;

	if(lConnectionTimeout)
	{
		if(!lCommandTimeout) 
			lCommandTimeout = lConnectionTimeout;
		SetExecuteCommandTimeout(lConnectionTimeout);

		m_spConn->ConnectionTimeout = lConnectionTimeout;
		m_spConn->CommandTimeout = lCommandTimeout;

		m_spCheckConn->ConnectionTimeout = lConnectionTimeout;
		m_spCheckConn->CommandTimeout = lCommandTimeout;
	}
	else
	{
		if(lCommandTimeout)
		{
			m_spConn->CommandTimeout = lCommandTimeout;
			m_spCheckConn->CommandTimeout = lCommandTimeout;
		}
	}

}

inline void CDBConnection::Open(const _bstr_t & bstrConn,  long lReconnectAttempts, long lReconnectTimeout, long lConnectionTimeout, long lCommandTimeout)
{
	m_lReconnectAttempts =lReconnectAttempts;
	m_lReconnectTimeout  =lReconnectTimeout;

	if (m_spConn == NULL || m_spCheckConn == NULL)
		InitConnection (bstrConn, lConnectionTimeout, lCommandTimeout);
	try
	{
		m_spConn->Open(bstrConn, _bstr_t(), _bstr_t(), 0);
		m_spCheckConn->Open(bstrConn, _bstr_t(), _bstr_t(), 0);
	}
	catch (_com_error& err)
	{
		// Do not reconnect at startup
		throw err;
//		Reconnect(bstrConn, err);
	}
#ifndef IGNORE_MSSQL_SPECIFIC
	// it is necessary for MTS
	m_spConn->Execute( L"SET TRANSACTION ISOLATION LEVEL READ COMMITTED", NULL, adExecuteNoRecords );
	m_spCheckConn->Execute( L"SET TRANSACTION ISOLATION LEVEL READ COMMITTED", NULL, adExecuteNoRecords );
#endif
}

inline void CDBConnection::CheckShutdown(long lTimeout) throw(_com_error)
{
	DWORD dwResult = ::WaitForSingleObject(m_ShutDownEvent, lTimeout);
	if (dwResult == WAIT_OBJECT_0)
		throw _com_error(E_FAIL);
	else
	if (dwResult != WAIT_TIMEOUT)
		throw _com_error(E_FAIL);

	//dwResult = ::WaitForSingleObject(g_EventStop, 0L);
	//if (dwResult == WAIT_OBJECT_0)
	//{
	//	EGLIB_TRACE_FMT("Thread 'DbInterface::Reconnect' was stopped!");
	//	throw _com_error(E_FAIL);
	//}
	//else
	//if (dwResult != WAIT_TIMEOUT)
	//{
	//	EGLIB_TRACE_FMT("Thread 'DbInterface::Reconnect' fail to stop");
	//	throw _com_error(E_FAIL);
	//}
}

inline void CDBConnection::Reconnect(const _bstr_t& bstrConn, const _com_error& ExtError) throw(_com_error)
{
	if (TryToLock() != TRUE)
	{
		// already reconnecting...
		EgLib::CAutoLock Lock(*this);

		// check shutdown state!
		CheckShutdown(0L);

		return;
	}
	
	EgLib::CAutoLock Lock(*this);
	Unlock();	// for TryLock

	_com_error error(ExtError);
	try
	{
		if (IsOpened())
		{
			m_spConn->Close();
			m_spCheckConn->Close();
		}
	}
	catch(...)
	{
	}
	for (long i = 0 ; i < m_lReconnectAttempts; (m_bInfiniteReconnect) ? i : ++i)
	{

		CheckShutdown(m_lReconnectTimeout * 1000);
		InitConnection(m_bsConnectionString, m_lConnectionTimeout, m_lCommandTimeout);
		try
		{
			//InitConnection (bstrConn, m_lReconnectTimeout, m_lReconnectTimeout);
			m_spConn->Open ( bstrConn, _bstr_t(), _bstr_t(), 0 );
			m_spCheckConn->Open ( bstrConn, _bstr_t(), _bstr_t(), 0 );
			//Open(bstrConn, m_lReconnectTimeout, m_lReconnectTimeout);
			error = _com_error(S_OK);
			break;
		}
		catch (_com_error& err)
		{
			error = err;
		}
	}
	if (error.Error() != S_OK || m_lReconnectAttempts == 0)
	{
		throw error;
	}
}

inline void CDBConnection::Close()
{
	if (IsOpened())
	{
		m_spConn->Close();
		m_spCheckConn->Close();
	}
	
	m_spConn = NULL;
	m_spCheckConn = NULL;
	m_bInTran = false;
}

inline bool CDBConnection::IsOpened() const
{
	if (m_spConn == NULL && m_spCheckConn == NULL)
		return false;

	long nState = m_spConn->GetState();
	long nCheckState = m_spCheckConn->GetState();
	return  (nState & adStateOpen) && (nCheckState & adStateOpen);
}

inline _bstr_t CDBConnection::GetConnectionString() const
{
	_bstr_t bstrConn;
	if (m_spConn)
		bstrConn = m_spConn->ConnectionString;
	return bstrConn;
}

inline void CDBConnection::SetConnectionString(const _bstr_t bsConnection) const
{
	if (m_spConn)
	{
		m_spConn->ConnectionString = bsConnection;
		m_spCheckConn->ConnectionString = bsConnection;
	}
}

inline void CDBConnection::BeginTran ()
{
	m_spConn->BeginTrans ();
	m_bInTran = true;
}

inline void CDBConnection::Commit ()
{
	m_spConn->CommitTrans();
	m_bInTran = false;
}

inline void CDBConnection::Rollback()
{
	m_spConn->RollbackTrans();
	m_bInTran = false;
}

inline _bstr_t CDBConnection::GetErrorMessage() const
{
	_bstr_t bstrRet("");

	try
	{
		ErrorPtr pErr = NULL;
		WCHAR szBuffer[1024];
		if((m_spConn->Errors->Count) > 0)
		{
			long nCount = m_spConn->Errors->Count;
			bstrRet = L"Errors: ";
			for(long i = 0; i < nCount; i++)
			{
				pErr = m_spConn->Errors->GetItem(i);
				swprintf_s (szBuffer, 1023, L"(0x%X) - %s ", pErr->Number, (LPCWSTR)pErr->Description);
				bstrRet += szBuffer;
			}
		}	
	}
	catch(...)
	{
		bstrRet = L"";
	}
	return bstrRet;
}

inline long CDBConnection::GetErrorCode() const
{
	return m_spConn->Errors->GetItem(0)->GetNativeError();
}

/////////////////////////////////////////////////////
// CDBCommand methods

inline void CDBCommand::InitCommand (CDBConnection & db, const _bstr_t & bstrText, CommandTypeEnum cmd_type)
{
	//// set sql command

	m_nVarCharSize = 0;
	m_bsParamName = L"";
	m_nRows = 0L;
	m_bParamAppended = false;
	m_nNewParamIdx = 0;
	m_nParamCnt = 0;
	HRESULT hr = m_spCommand.CreateInstance (__uuidof (Command) );
	_com_util::CheckError (hr);

	while (true)
	{
		try
		{
			m_spCommand->ActiveConnection = db.m_spConn;
			break;
		}
		catch(_com_error& err)
		{
			if (!m_pConnection->IsOpened() || !IsConnected()/*IsConnectError(m_pConnection->GetErrorCode())*/) // cannot connect to DB!
			{
				m_pConnection->Reconnect(m_pConnection->GetConnectionString(), err);
				ReInitConnection(*m_pConnection);
			}
			else
			{
				// something other errors, just return to upper level!
				throw err;
			}
		}
	}

	m_spCommand->CommandText = bstrText;
	m_spCommand->CommandType = cmd_type;

	long lTm = db.GetExecuteCommandTimeout();
	if (lTm!=0)
		m_spCommand->CommandTimeout = lTm;

#ifndef IGNORE_MSSQL_SPECIFIC	
	if ( cmd_type == adCmdStoredProc )
	{
		_ParameterPtr spRes = AddParam (adInteger, adParamReturnValue, 0, 0L );
		m_spResParam = spRes;
	}
#endif
}

inline void CDBCommand::ReInitConnection(CDBConnection & db) throw(_com_error)
{
	EgLib::CAutoLock Lock(db);
	m_spCommand->ActiveConnection = db.m_spConn;
	long lTm = db.GetExecuteCommandTimeout();
	if (lTm!=0)
		m_spCommand->CommandTimeout = lTm;
}


inline _ParameterPtr CDBCommand::AddParam ( DataTypeEnum dt, ParameterDirectionEnum paramDir, 
								size_t nSize, const _variant_t & vtVal )
{
	_ParameterPtr spParam;
	if (m_nNewParamIdx < m_nParamCnt )	
	{
		spParam = m_spCommand->Parameters->GetItem(m_nNewParamIdx);
		if (spParam->GetDirection() != paramDir )
		{
			ATLASSERT (FALSE);
			_com_util::CheckError (E_INVALIDARG);
		}
		spParam->Type = dt;
		spParam->Size = static_cast<long>(nSize);
		if (vtVal != vtMissing)
			spParam->Value = vtVal;
	}
	else
	{
		spParam = m_spCommand->CreateParameter (m_bsParamName, dt, paramDir, static_cast<long>(nSize), vtVal );
		m_spCommand->Parameters->Append (spParam);
		m_bsParamName = L"";
	}

	m_nNewParamIdx++;
	m_nParamCnt++;
	
	m_bParamAppended = true;
	return spParam;
}

inline	CDBCommand& CDBCommand::AddInParam ( DataTypeEnum dt,  const _variant_t & vtVal, size_t nSize, bool bSelType)
{
	if ((dt == adVarChar || dt == adVarWChar ) && vtVal.vt == VT_BSTR)
	{
		if(!nSize) 
			nSize = ::SysStringLen (vtVal.bstrVal);

		if (nSize ==0) nSize = 1;
		if(!bSelType && nSize > 4000) dt = adVarChar;
	}
	else
	if ( dt == adVarBinary )
		ATLASSERT ( nSize > 0);

	AddParam (dt,adParamInput,nSize,  vtVal);

	return *this;
}

inline 	CDBCommand& CDBCommand::AddOutParam ( DataTypeEnum dt, COpPtr spOp, size_t nSize)
{
	if (nSize ==0)
		nSize = (dt == adVarChar || dt == adVarWChar ) ? m_nVarCharSize : 0;
	
	_ParameterPtr spParam =	AddParam (dt,adParamOutput,nSize);
	m_out_prms [ spParam] = spOp;
	m_nVarCharSize = 0;
	
	return *this;
}

// operators << definitions

inline	CDBCommand & CDBCommand::operator << ( const CDBNull &)
{
	_variant_t vtVal;
	vtVal.vt = VT_NULL;
	return AddInParam (adInteger, vtVal );
}

inline CDBCommand & CDBCommand::operator << ( int nVal)
{
	return AddInParam (adInteger, (long)nVal );
}

inline CDBCommand & CDBCommand::operator << ( double dVal)
{
	return AddInParam (adDouble, dVal );
}

inline CDBCommand & CDBCommand::operator << (LPCOLESTR szVal)
{
	return AddInParam (adVarWChar, szVal );
}

inline CDBCommand & CDBCommand::operator << ( const _bstr_t &  bstrVal)
{
	return AddInParam (adVarWChar, bstrVal );
}
inline CDBCommand & CDBCommand::operator << ( const std::string&  lsVal)
{
	return AddInParam (adVarWChar, lsVal.c_str(), lsVal.length());
}

inline CDBCommand & CDBCommand::operator << ( const std::wstring &  strVal)
{
	return AddInParam (adVarWChar, strVal.c_str() );
}

inline	CDBCommand & CDBCommand::operator << ( const vt_date &  dateVal)
{
	_variant_t vtDate;
	vtDate.vt = VT_DATE;
	vtDate.date = dateVal;

	return AddInParam (adDate, vtDate );
}

inline	CDBCommand & CDBCommand::operator << ( const CBinaryParam & binVal)
{
	_variant_t vtVal;
	binary_utils::binary_to_var (binVal.m_pData, binVal.m_nSize, vtVal);
	return AddInParam (adVarBinary, vtVal, binVal.m_nSize);
}

//inline	CDBCommand & CDBCommand::operator << ( const LONGLONG & llVal)
//{
//#if (_WIN32_WINNT >= 0x0501)
//	_variant_t vtVal(static_cast<__int64>(llVal));	
//	return AddInParam (adBigInt, vtVal);
//#else
//	_variant_t vtVal(llVal);	
//	return AddInParam (adBigInt, vtVal);
//#endif
//}

inline	CDBCommand & CDBCommand::operator << ( BYTE bVal) throw (_com_error)
{
	return AddInParam (adTinyInt, bVal );
}

inline	CDBCommand & CDBCommand::operator << ( float fVal) throw (_com_error)
{
	return AddInParam (adSingle, fVal );
}

inline	CDBCommand & CDBCommand::operator << ( long lVal) throw (_com_error)
{
	return AddInParam (adInteger, lVal );
}

inline	CDBCommand & CDBCommand::operator << ( short sVal) throw (_com_error)
{
	return AddInParam (adSmallInt, sVal );
}

inline CDBCommand & CDBCommand::operator << ( CFieldBase & fld) throw (_com_error)
{
	return AddInParam (fld.m_spField->Type, fld.m_spField->Value); 
}


inline 	CDBCommand & CDBCommand:: operator << ( int * pnVal)
{
	return AddOutParam (adInteger, new _COutParam <long>  ( (long*) pnVal)  );
}

inline CDBCommand & CDBCommand::operator << ( long * pnVal)
{
	return AddOutParam (adInteger, new _COutParam <long> (pnVal) );
}

inline 	CDBCommand & CDBCommand:: operator << ( double * pdVal)
{
	return AddOutParam (adDouble, new _COutParam <double> (pdVal) );
}

inline 	CDBCommand & CDBCommand:: operator << (_bstr_t *  pbstrVal)
{
	return AddOutParam (adVarWChar, new _COutParam <_bstr_t> (pbstrVal) );
}

inline CDBCommand & CDBCommand::operator << ( short * pnVal)
{
	return AddOutParam (adSmallInt, new _COutParam <short> (pnVal) );
}


inline CDBCommand & CDBCommand::operator << ( float * pfVal)
{
	return AddOutParam (adSingle, new _COutParam <float> (pfVal) );
}

inline CDBCommand & CDBCommand::operator << ( vt_date *  pdateVal)
{
	return AddOutParam (adDate, new _COutParam <vt_date> (pdateVal) );
}


//inline CDBCommand & CDBCommand::operator << ( LONGLONG*  pllVal)
//{
//	return AddOutParam (adBigInt, new _COutParam <LONGLONG> (pllVal) );
//}

inline CDBCommand & CDBCommand::operator << ( CBinaryParam * pbinVal) 
{
	ATLASSERT (pbinVal);
	return AddOutParam (adVarBinary, new _COutParam <void> (pbinVal->m_pData, pbinVal->m_nSize), pbinVal->m_nSize);
}

//////////////////////////////////////////////////

inline void TESTHR(HRESULT x) {if FAILED(x) _com_issue_error(x);};
inline bool CDBCommand::IsConnected()
{
	bool bStatus = true;
	try
	{
		m_pConnection->GetAdoBackupConnectionPtr()->Execute(_bstr_t("SELECT 1"), NULL, adExecuteNoRecords);
	} 
	catch (_com_error&)
	{
		bStatus = false;
	}

	return bStatus;
}

inline void CDBCommand::ClearParams()
{
	m_nRows = 0;
	m_out_prms.clear();
	if (m_bParamAppended)
	{
		while ((m_nParamCnt = m_spCommand->Parameters->GetCount ()) > 
			(m_spResParam != NULL ? 1:0) )
		{
			m_spCommand->Parameters->Delete (m_nParamCnt-1L);
		}
		m_nNewParamIdx = m_nParamCnt;
		if ( m_nParamCnt == 0 ) m_bParamAppended = false;
	}
}

inline void CDBCommand::ResetParams ()
{
	m_nNewParamIdx = m_spResParam != NULL ? 1 : 0;
}

inline bool CDBCommand::IsNull (void * pParam)
{
	for (prm_map::iterator it = m_out_prms.begin(); it != m_out_prms.end(); it++ )
	{
		if (* (it->second) ==  pParam )
		{
			VARTYPE vt = it->first->GetValue().vt;
			return (vt == VT_NULL);
		}
	}

	return false;
}

inline void CDBCommand::SetOutParams()
{
	for (prm_map::iterator it = m_out_prms.begin(); it != m_out_prms.end(); it++ )
	{
		_ParameterPtr param = it->first;
		it->second->SetValue ( param );	
	}
}

inline void CDBCommand::Execute ()
{
	_variant_t vtAffected;

	while (true)
	{
		try
		{
			m_spCommand->Execute (&vtAffected, NULL, adExecuteNoRecords);
			m_nRows =vtAffected;
			SetOutParams();
			return;
		}
		catch (_com_error& err)
		{
			if (!m_pConnection->IsOpened() || 
				!IsConnected())
				/*IsConnectError(m_pConnection->GetErrorCode()))*/ // cannot connect to DB!
			{
				m_pConnection->Reconnect(m_pConnection->GetConnectionString(), err);
				ReInitConnection(*m_pConnection);
			}
			else
			{
				// something other errors, just return to upper level!
				throw err;
			}
		}
		//m_spCommand->Execute (&vtAffected, NULL, adExecuteNoRecords);
		//m_nRows =vtAffected;
		//SetOutParams();
	}
}

inline int CDBCommand::GetResult ()
{
	if ( m_spResParam != NULL )	
	{
		_variant_t vtVal = m_spResParam->GetValue();
		return static_cast <long> (vtVal);
	}
	return 0;
}

inline void CDBCommand::Prepare ()
{
	m_spCommand->PutPrepared (true);
}

inline void CDBCommand::Unprepare ()
{
	m_spCommand->PutPrepared (false);
}

inline bool CDBCommand::IsPrepared ()
{
	return m_spCommand->GetPrepared () == TRUE;
}


inline long CDBCommand::GetConnectionTimeout() const
{
	try
	{
		return m_spCommand->CommandTimeout;
	}
	catch(...)
	{
        return 0;
	}
}


inline void CDBCommand::SetConnectionTimeout(long nTimeout)
{
	try
	{
		m_spCommand->CommandTimeout = nTimeout;
	}
	catch(...)
	{
	}
}

#endif //EGLIB_DB_CLIENT_ONLY

///////////////////////
// CFieldBase 

inline CFieldBase::operator int ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? static_cast <long> (vtVal) : 0 ;
}

inline CFieldBase::operator CURRENCY ()  const
{
	CURRENCY Def;
	memset(&Def, 0, sizeof(Def));
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? static_cast <CURRENCY> (vtVal) : Def ;
}

inline CFieldBase::operator double ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : 0.0 ;
}


inline CFieldBase::operator _bstr_t ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : L"" ;
}
inline	CFieldBase::operator vt_date () const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ?  static_cast<DATE> (vtVal) : vt_date () ;
}

inline CFieldBase::operator long ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : 0L ;
}

inline CFieldBase::operator short ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : (short)0 ;
}
inline CFieldBase::operator float ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : 0.0f ;
}

inline CFieldBase::operator BYTE ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : (BYTE)0 ;
}

//inline CFieldBase::operator LONGLONG ()  const
//{
//	_variant_t vtVal = GetValue();
//	return utils::HasValue(vtVal) ? vtVal : (LONGLONG)0 ;
//}

inline CFieldBase::operator bool ()  const
{
	_variant_t vtVal = GetValue();
	return utils::HasValue(vtVal) ? vtVal : false;
}

inline	bool CFieldBase::HasValue () const
{
	return utils::HasValue ( GetValue());
}

inline bool CFieldBase::IsNull () const
{
	return GetValue().vt == VT_NULL;
}

inline bool CFieldBase::IsEmpty () const
{
	return GetValue().vt == VT_EMPTY;
}

inline _variant_t CFieldBase::GetValue () const
{
	return m_spField->GetValue();
}
inline	_bstr_t CFieldBase::GetName () const
{
	return m_spField->GetName();
}

inline void CFieldBase::GetBinaryValue(void * pData, size_t nSize) const
{
	ATLASSERT (pData && nSize >0 );

	_variant_t vtVal = GetValue();
	binary_utils::var_to_binary (vtVal, pData, nSize);
}
//////////////////////////////////////////////////
// CRecordsetBase
inline void CRecordsetBase::InitRecordset (CursorLocationEnum cl, CursorTypeEnum ct, LockTypeEnum lt)
{
	HRESULT hr=m_spRs.CreateInstance (__uuidof (Recordset) );
	_com_util::CheckError (hr);

	m_spRs->CursorLocation = cl;
	m_spRs->CursorType = ct;
	m_spRs->LockType  = lt;

}

inline void CRecordsetBase::Open ()
{
	while(true)
	{
#ifndef EGLIB_DB_CLIENT_ONLY
		try
		{
			m_spRs->Open (m_pDBCmd ? m_pDBCmd->m_spCommand.GetInterfacePtr() : vtMissing , 
				vtMissing, adOpenUnspecified, adLockUnspecified, 0 );
			if (m_pDBCmd)
				m_pDBCmd->SetOutParams ();
			break;
		}
		catch (_com_error& err)
		{
			if (!m_pDBCmd->m_pConnection->IsOpened() || 
				!m_pDBCmd->IsConnected())
				/*m_pDBCmd->IsConnectError(m_pDBCmd->m_pConnection->GetErrorCode()))*/ // cannot connect to DB!
			{
				m_pDBCmd->m_pConnection->Reconnect(m_pDBCmd->m_pConnection->GetConnectionString(), err);
				m_pDBCmd->ReInitConnection(*(m_pDBCmd->m_pConnection));
			}
			else
				throw err;
		}
#else
		m_spRs->Open ( vtMissing, vtMissing, adOpenUnspecified, adLockUnspecified, 0 );
#endif //EGLIB_DB_CLIENT_ONLY
	}
}

inline void CRecordsetBase::Close ()
{
	if (IsOpened())
		m_spRs->Close();

	m_idx_map.clear();
	m_name_map.clear();
}

inline bool CRecordsetBase::IsOpened () const
{
	long nState = m_spRs->GetState ();
	return nState & adStateOpen;
}


inline int CRecordsetBase::GetFieldCount ()  const
{
	return m_spRs->Fields->GetCount();
}

inline bool CRecordsetBase::IsBOF ()  const
{
	return m_spRs->GetAdoBof() != 0;
}

inline bool CRecordsetBase::IsEOF () const
{
	return m_spRs->GetAdoEof() != 0;
}


//inline FieldPtr CRecordsetBase::GetField ( int nIndex) const
//{
//	FieldPtr spFld;
//	fld_by_idx::iterator it = m_idx_map.find (nIndex);
//	
//	if (it == m_idx_map.end() )
//	{
//		spFld = m_spRs->Fields->GetItem (static_cast <long> (nIndex) );
//		m_idx_map [nIndex] = spFld;
//	}
//	else
//		spFld = it->second;
//	
//	return spFld;
//}

inline FieldPtr CRecordsetBase::GetField ( const _variant_t& bstrName) const
{
//	FieldPtr spFld;
//	BSTR bstrCopy = bstrName.copy();
//	_wcslwr (bstrCopy);
//
//	_bstr_t bstrKey  (bstrCopy, false);
//	fld_by_name::iterator it = m_name_map.find (bstrKey);
//	
//	if (it == m_name_map.end() )
//	{
//		spFld = m_spRs->Fields->GetItem (bstrKey);
//		m_name_map [bstrKey] = spFld;
//	}
//	else
//		spFld = it->second;
//
//	return spFld;
	return m_spRs->Fields->GetItem(bstrName);
}

#ifndef EGLIB_DB_CLIENT_ONLY

inline void CRecordsetBase::NextRecordset ()
{
	_RecordsetPtr spRsNew = m_spRs->NextRecordset (NULL);
	if (spRsNew != NULL)
	{
		m_idx_map.clear();
		m_name_map.clear();
		m_spRs = spRsNew;
	}
}

#endif

#ifndef EGLIB_DB_CLIENT_ONLY
//////////////////////////////////////
// CDefaultRecordset
inline 	CDefaultRecordset::CDefaultRecordset():
	CRecordsetBase (adUseServer, adOpenForwardOnly, adLockReadOnly ) 
{
	
}

inline void CDefaultRecordset::MoveNext ()
{
	m_spRs->MoveNext();
}

inline CReadOnlyField CDefaultRecordset::operator [] ( const _variant_t& vtName) const
{
	return GetField (vtName);
}

//inline  CReadOnlyField CDefaultRecordset::operator [] ( int nIndex) const
//{
//	return GetField (nIndex);
//}

#endif //EGLIB_DB_CLIENT_ONLY

//////////////////////////////////////////////
// CClientRecordset

inline CClientRecordset::CClientRecordset (const CClientRecordset & src)
{
	m_spRs = src.m_spRs->Clone (adLockUnspecified);
}

inline	CClientRecordset::CClientRecordset (IDispatch * pRs)
{
	ATLASSERT (pRs);
	_RecordsetPtr spRs = pRs;

	ATLASSERT ( spRs->CursorLocation == adUseClient);
	ATLASSERT ( spRs->CursorType == adOpenStatic); 
	m_spRs = spRs;
}

inline	CClientRecordset::CClientRecordset (_Recordset * pRs)
{
	ATLASSERT (pRs);
	_RecordsetPtr spRs = pRs;

	ATLASSERT ( spRs->CursorLocation == adUseClient);
	ATLASSERT ( spRs->CursorType == adOpenStatic); 
	m_spRs = spRs;
}

inline	CClientRecordset::CClientRecordset (CFieldBase & fld)
{
	_variant_t vtValue = fld.GetValue();
	m_spRs = (IDispatch *) vtValue;
}	


inline void CClientRecordset::GetAdoRecordset (IDispatch ** ppRs)
{
	ATLASSERT (ppRs);
	_RecordsetPtr spRs = m_spRs;
	spRs->PutRefActiveConnection (NULL);
	spRs->QueryInterface (ppRs);
}

inline void CClientRecordset::GetAdoRecordset (_Recordset ** ppRs)
{
	ATLASSERT (ppRs);
	_RecordsetPtr spRs = m_spRs;
	spRs->PutRefActiveConnection (NULL);
	spRs->QueryInterface (ppRs);
}

inline void CClientRecordset::MoveLast ()
{
	m_spRs->MoveLast();
}

inline void CClientRecordset::MoveFirst ()
{
	m_spRs->MoveFirst ();
}

inline void CClientRecordset::MovePrev ()
{
	m_spRs->MovePrevious();
}

inline void CClientRecordset::MoveNext ()
{
	m_spRs->MoveNext();
}

inline void CClientRecordset::SetFilter(_bstr_t bsFilter)
{
	m_spRs->Filter = bsFilter;
}

inline CReadOnlyField CClientRecordset::operator [] ( const _variant_t& vtName)  const
{
	return GetField (vtName);
}

//inline  CReadOnlyField CClientRecordset::operator [] ( int nIndex) const
//{
//	return GetField (nIndex);
//}

inline int CClientRecordset::GetRecordCount ()
{
	return m_spRs->GetRecordCount ();
}

inline 	CClientRecordset & CClientRecordset::operator = (const CClientRecordset & src)
{
	m_spRs = src.m_spRs->Clone (adLockUnspecified);
	return *this;
}

#ifndef EGLIB_DB_CLIENT_ONLY	

inline void CClientRecordset::InternalOpen ( CDBCommand & cmd )
{
	SetDBCommand (&cmd);
	try
	{
		CRecordsetBase::Open ();
	}
	catch (...)
	{
		SetDBCommand (NULL);
		throw;
	}
	SetDBCommand (NULL);
}

#endif
#endif //__EGLIB_DB_ACCESS_INL__
