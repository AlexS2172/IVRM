//**************************************************************************************************************//
// logic guardians
//**************************************************************************************************************//
#if !defined(__EGLIBMISC_H__)
	#error 'EgLibDb.h requires EgLibMisc.h to be included first'
#endif


//**************************************************************************************************************//
// header identifier
//**************************************************************************************************************//
#define __EGLIBDB_H__

///////////////////////////////////////////////
// EgLibDB.h
// Definitions of the eGAR data access library
//
///////////////////////////////////////////////

#ifndef __EGLIB_DB_ACCESS_H__
#define __EGLIB_DB_ACCESS_H__

#pragma warning(disable: 4786)

#pragma warning	(push)
#pragma warning (disable: 4146)

#ifndef EGLIB_ADO_IMPORT_SPECIFIER 
	#define EGLIB_ADO_IMPORT_SPECIFIER rename_namespace("omlib_ado"), named_guids
#endif

#ifndef EGLIB_ADO_USING_NAMESPACE
	#define EGLIB_ADO_USING_NAMESPACE  using namespace omlib_ado;
#endif

// To be sure that it will be compatible for ADO 2.5 and later
//#define EGLIB_DB_BACKWARD_COMPATIBLE

#ifdef EGLIB_DB_BACKWARD_COMPATIBLE
	#undef EGLIB_DB_CLIENT_ONLY
	#import <msado25.tlb> rename ("EOF", "AdoEof")  rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
#else
	#ifndef EGLIB_DB_CLIENT_ONLY
		#import <msado15.dll> rename ("EOF", "AdoEof")  rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
	#else
		#import <msador15.dll> rename ("EOF", "AdoEof") rename ("BOF", "AdoBof") EGLIB_ADO_IMPORT_SPECIFIER
	#endif //EGLIB_DB_CLIENT_ONLY
#endif

#pragma warning(pop)

//#define _WIN32_WINNT 0x0500
#include "EgLibSync.h"

#include <string>
#include <map>
#include <list>
#include <sstream>
#include <vector>
#include <atlstr.h>


#pragma warning(push)
#pragma warning(disable : 4290)

namespace EgLib
{

//namespace db_access
//{

class CDBCommand;

//};

#ifndef EGLIB_DB_CLIENT_ONLY

namespace db_manip
{

//using namespace db_access;


class setsize
{
	friend class CDBCommand;
public:
	setsize (int nSize): m_nSize (nSize) {}
private:
	int m_nSize;
};
class setname 
{
	friend class CDBCommand;
public:
	setname(_bstr_t Name): m_bsName (Name) {}
private:
	_bstr_t m_bsName;

};

};

#endif // OMLIB_DB_CLIENT_ONLY


//namespace db_access
//{
	
EGLIB_ADO_USING_NAMESPACE;
	
class CDBNull
{
public:
	CDBNull () {}
};

class CBinaryParam
{
	friend class CDBCommand;
public:
	CBinaryParam (void * pData, size_t nSize): m_pData (pData), m_nSize (nSize)
		{ ATLASSERT (pData && nSize >0 );	}
protected:
	void * m_pData;
	size_t m_nSize;
};


//namespace binary_utils
//{
	void var_to_binary (const _variant_t & vtVal, void * pData, size_t nSize) throw(_com_error);
	void binary_to_var (const void * pData, size_t nSize, _variant_t & rvt ) throw(_com_error);
//};

#ifndef EGLIB_DB_CLIENT_ONLY

class CDBConnection 
	: public EgLib::CLock
{
	friend class CDBCommand;

protected:
	void CheckShutdown(long lTimeout) throw(_com_error);

public:

	CDBConnection ():
	  m_lExecuteCommandTimeout(0) 
	{
		m_ShutDownEvent.CreateEvent(NULL, TRUE, FALSE);
	}

	CDBConnection (const _bstr_t & bstrConn, long lReconnectAttempts = 0, long lReconnectTimeout=0, long lConnectionTimeout = 0, long lCommandTimeout = 0) throw (_com_error)
		:	m_lReconnectAttempts(lReconnectAttempts)
		,	m_lReconnectTimeout(lReconnectTimeout)
		,	m_lExecuteCommandTimeout(lCommandTimeout)
		,	m_bsConnectionString(bstrConn)
	    ,   m_lConnectionTimeout(lConnectionTimeout)
 	    ,   m_lCommandTimeout(lCommandTimeout)
		,	m_bInfiniteReconnect(true)
		,	m_bFilterConnection(false)
	{
		InitConnection(bstrConn, lConnectionTimeout, lCommandTimeout); 
		m_ShutDownEvent.CreateEvent(NULL, TRUE, FALSE);
	}

	void Stop()
	{
		m_ShutDownEvent.SetEvent();
	}

	void SetFilterConnection()
	{
		m_bFilterConnection = true;
	}

	void Open(const _bstr_t & bstrConn = _bstr_t (),  long lReconnectAttempts = 0, long lReconnectTimeout=0, long lConnectionTimeout = 0, long lCommandTimeout=0)  throw (_com_error) ;
	void Reconnect(const _bstr_t & bstrConn, const _com_error& ExtError = _com_error(S_OK)) throw(_com_error);
	void Close()  throw (_com_error);
	bool IsOpened() const  throw (_com_error);

	_bstr_t GetConnectionString() const throw (_com_error);
	void SetConnectionString(const _bstr_t bsConnection) const throw (_com_error);

	void BeginTran() throw (_com_error);
	void Commit() throw (_com_error);
	void Rollback() throw (_com_error);
	bool InTransaction() const { return m_bInTran; }

	CDBConnection& operator=(_ConnectionPtr spConn)
	{
		m_spConn = NULL;
		m_spConn = spConn;
		return *this;
	}

	long GetCommandTimeout() 
	{
		if(m_spConn!=NULL)
			return m_spConn->CommandTimeout;
		else
			return 0;
	}

	_ConnectionPtr& GetAdoConnectionPtr()
	{
		return m_spConn;
	}

	_ConnectionPtr& GetAdoBackupConnectionPtr()
	{
		return m_spCheckConn;
	}

    long GetReconnectTimeout() 
    {
		return m_lReconnectTimeout;
    }

	long GetConnectionTimeout() 
	{
		if(m_spConn!=NULL)
			return m_spConn->ConnectionTimeout;
		else
			return 0;
	}

	void SetExecuteCommandTimeout(long lExecuteCommandTimeout)
	{
		m_lExecuteCommandTimeout = lExecuteCommandTimeout;
	}

	long GetExecuteCommandTimeout() const
	{
		return m_lExecuteCommandTimeout;
	}

	void SetReconnectTimeout(long lReconnectTimeout)
	{
		m_lReconnectTimeout = lReconnectTimeout;
	}

	long GetReconnectAttempts()
	{
		return m_lReconnectAttempts;
	}

	void SetReconnectAttepmts(long lReconnectAttemts)
	{
		m_lReconnectAttempts = lReconnectAttemts;
	}

	bool GetInfiniteReconnect()
	{
		return m_bInfiniteReconnect;
	}

	void SetInfiniteReconnect(bool bInfiniteReconnect)
	{
		m_bInfiniteReconnect = bInfiniteReconnect;
	}

	_bstr_t GetErrorMessage() const;
	long GetErrorCode() const;

private:
	void InitConnection (const _bstr_t & bstrConn, long lConnectionTimeout, long lCommandTimeout)  throw (_com_error);
	_ConnectionPtr m_spConn;
	_ConnectionPtr m_spCheckConn;
	bool m_bInTran;
	long m_lReconnectAttempts;
	long m_lReconnectTimeout;
	long m_lExecuteCommandTimeout;
	bool m_bInfiniteReconnect;
	EgLib::CEvent m_ShutDownEvent;

	_bstr_t m_bsConnectionString;
	long    m_lConnectionTimeout;
	long    m_lCommandTimeout;

	bool m_bFilterConnection;
};

//----------------------------------------------------------------------------------------------
class _COutParamBase
{
	friend class CDBCommand;
protected:
	_COutParamBase (void * pData): m_pData(pData)  { ATLASSERT (m_pData); }
	virtual void SetValue ( _Parameter * pParam ) throw (_com_error) =0;
	bool operator == (void * pVoid) const { return m_pData == pVoid; }
	void * m_pData;
};

//----------------------------------------------------------------------------------------------
template <class T>
class _COutParam: public _COutParamBase
{
	friend class CDBCommand;
protected:
	_COutParam (T * pData): _COutParamBase ( pData) {}	
	void SetValue ( _Parameter * pParam) throw (_com_error)
	{
		_variant_t vtVal =  pParam->GetValue();
		if (utils::HasValue (vtVal) )
			*(static_cast <T*> (m_pData))  = vtVal;
	}
};

//----------------------------------------------------------------------------------------------
template <>
class _COutParam <vt_date> : public _COutParamBase
{
	friend class CDBCommand;
protected:
	_COutParam (vt_date * pData): _COutParamBase ( pData) {}	
	void SetValue ( _Parameter * pParam) throw (_com_error)
	{
		_variant_t vtVal =  pParam->GetValue();
		if (utils::HasValue (vtVal) )
			*(static_cast <vt_date*> (m_pData)) = static_cast <DATE>  (vtVal);
	}
};

//----------------------------------------------------------------------------------------------
template <>
class _COutParam <void> : public _COutParamBase
{
	friend class CDBCommand;
protected:
	_COutParam (void * pData, size_t nSize): _COutParamBase(pData), m_nSize (nSize)
	{
	}
	void SetValue ( _Parameter * pParam) throw (_com_error)
	{
		_variant_t vtVal =  pParam->GetValue();
		binary_utils::var_to_binary (vtVal, m_pData, m_nSize);
	}
	size_t m_nSize;		
};

//----------------------------------------------------------------------------------------------
class CFieldBase;
//----------------------------------------------------------------------------------------------
class ATL_NO_VTABLE CDBCommand
{
	friend class CRecordsetBase;
private:
	CDBCommand (const CDBCommand & cmd);
public:

	// Func just call usp_check_connection
	// It's empty Storep Procedured, 
	// but if we cannot successfully execute, it means that connection is lost
	// true  - ok,
	// false - error, connection is lost
	bool IsConnected();

	void Execute () throw (_com_error);

	////////////////////////////////////////////////////
	// For using manipulators
	CDBCommand & operator << ( CDBCommand & (*_f)( CDBCommand &)) throw (_com_error)
		{ _f (*this); return *this; }
	CDBCommand & operator << ( const db_manip::setsize & sz) throw (_com_error)
		{ SetSize (sz.m_nSize); return *this;}
	CDBCommand & operator << ( const db_manip::setname & sz) throw (_com_error)
		{ SetName (sz.m_bsName); return *this;}

	////////////////////////////////////////////////////
	CDBCommand & operator << ( const CDBNull &) throw (_com_error);
	
	CDBCommand & operator << ( int nVal) throw (_com_error);
	CDBCommand & operator << ( double dVal) throw (_com_error);
	
	CDBCommand & operator << ( const std::wstring &  strVal) throw (_com_error);
	CDBCommand & operator << ( const _bstr_t &  bstrVal) throw (_com_error);
	CDBCommand & operator << ( LPCOLESTR szVal) throw (_com_error);
	
	CDBCommand & operator << ( const vt_date &  dateVal) throw (_com_error);
	CDBCommand & operator << ( const CBinaryParam & binVal) throw (_com_error);
	//CDBCommand & operator << ( const LONGLONG& llVal) throw (_com_error);
	CDBCommand & operator << ( BYTE bVal) throw (_com_error);
	CDBCommand & operator << ( float fVal) throw (_com_error);
	CDBCommand & operator << ( long lVal) throw (_com_error);
	CDBCommand & operator << ( short sVal) throw (_com_error);
	CDBCommand & operator << ( CFieldBase & fld) throw (_com_error);
	CDBCommand & operator << ( const std::string & fld) throw (_com_error);

	CDBCommand & operator << ( int * pnVal) throw (_com_error);
	CDBCommand & operator << ( long * pnVal) throw (_com_error);
	CDBCommand & operator << ( short * pnVal) throw (_com_error);
	CDBCommand & operator << ( double * pdVal) throw (_com_error);
	CDBCommand & operator << ( float * pfVal) throw (_com_error);
	CDBCommand & operator << ( _bstr_t *  pbstrVal) throw (_com_error);
	CDBCommand & operator << ( vt_date *  pdateVal) throw (_com_error);
	CDBCommand & operator << ( CBinaryParam * pbinVal) throw (_com_error);
	//CDBCommand & operator << ( LONGLONG* pllVal) throw (_com_error);



	void ClearParams () throw (_com_error);
	void ResetParams ();
	void SetSize ( int nSize )  {m_nVarCharSize = nSize;}
	void SetName(_bstr_t bsName){m_bsParamName = bsName;}
	bool IsNull (void * pParam);
	long GetRowsAffected()
	{
		return m_nRows;
	}

	long GetConnectionTimeout() const;
	void SetConnectionTimeout(long nTimeout);

	//long GetExecuteCommandTimeout() const;
	//void SetExecuteCommandTimeout(long lExecuteCommandTimeout);
	
    int GetResult () throw (_com_error);  

protected:
	CDBCommand ( CDBConnection & db, const _bstr_t & bstrText, CommandTypeEnum cmd_type) throw (_com_error) 
		:	m_pConnection(&db)
		{
			InitCommand(db, bstrText, cmd_type);
		}	
	
	void Prepare () throw (_com_error);
	void Unprepare () throw (_com_error);
	bool IsPrepared ();
private:
	typedef CSmartPtr <_COutParamBase> COpPtr;
	typedef std::map <_Parameter*, COpPtr> prm_map;

	void InitCommand (CDBConnection & db, const _bstr_t & bstrText, CommandTypeEnum cmd_type) throw (_com_error);
	void ReInitConnection(CDBConnection & db) throw(_com_error);
	CDBCommand& AddInParam ( DataTypeEnum dt, const _variant_t & vtVal = vtMissing, size_t nSize = 0, bool bSelType = false) throw (_com_error);
	CDBCommand& AddOutParam ( DataTypeEnum dt, COpPtr spOp, size_t nSize=0) throw (_com_error);
	_ParameterPtr AddParam (DataTypeEnum dt, ParameterDirectionEnum paramDir, size_t nSize, 
		const _variant_t & vtVal = vtMissing) throw (_com_error);
	void SetOutParams() throw (_com_error);
	
	prm_map m_out_prms;
	long    m_nVarCharSize;
	_bstr_t m_bsParamName;
	bool	m_bParamAppended;
	long	m_nNewParamIdx;
	long	m_nParamCnt;
	long	m_nRows;
protected:
	CDBConnection*	m_pConnection;
	_CommandPtr	m_spCommand;
	_ParameterPtr	m_spResParam;
};

class CNoRecordset
{
protected:
	void SetDBCommand (CDBCommand *) {};
};

template <class R = CNoRecordset>
class CStoredProc: public CDBCommand, public R
{
public:
	CStoredProc (CDBConnection & db, const _bstr_t & bstrName) throw (_com_error) : 
	  CDBCommand (db, bstrName, adCmdStoredProc) { SetDBCommand (this);  }

	int GetResult() throw (_com_error) 
		{ return CDBCommand::GetResult(); } ;
};

template <class R = CNoRecordset>
class CSQLQuery: public CDBCommand, public R
{
public:
	CSQLQuery (CDBConnection & db, const _bstr_t & bstrText) throw (_com_error): 
		CDBCommand (db, bstrText, adCmdText) 
		{ SetDBCommand (this); }
	void Prepare ()	throw (_com_error)		{ CDBCommand::Prepare();  }
	void Unprepare () throw (_com_error)	{ CDBCommand::Unprepare();  };
	bool IsPrepared () 	{ CDBCommand::IsPrepared();  };
};

#endif // EGLIB_DB_CLIENT_ONLY


class ATL_NO_VTABLE CFieldBase 
{
	friend class CDBCommand;
public:
	CFieldBase ( const FieldPtr & spFld): m_spField (spFld) { ATLASSERT (m_spField != NULL); }
public:
	operator int () const throw (_com_error); 
	operator double () const throw (_com_error); 
	operator float () const throw (_com_error); 
	operator _bstr_t () const throw (_com_error); 
	operator vt_date () const throw (_com_error); 
	operator long () const throw (_com_error); 
	operator short () const throw (_com_error); 
	operator BYTE () const throw (_com_error); 
	//operator LONGLONG() const throw (_com_error); 
	operator bool () const throw (_com_error); 
	operator CURRENCY () const throw (_com_error); 
	
	void GetBinaryValue(void * pData, size_t nSize) const throw (_com_error); 
	bool IsNull ()  const  throw (_com_error);
	bool IsEmpty ()  const throw (_com_error);
	bool HasValue () const throw (_com_error);
	_variant_t GetValue ()  const throw (_com_error);
	_bstr_t GetName () const throw (_com_error);
protected:
	FieldPtr m_spField;
};

class ATL_NO_VTABLE CRecordsetBase 
{
public:
	int GetFieldCount () const throw (_com_error);
	void Open () throw (_com_error);
	void Close () throw (_com_error);
	bool IsOpened () const throw (_com_error);

	bool IsEOF() const throw (_com_error);
	bool IsBOF() const throw (_com_error);

#ifndef EGLIB_DB_CLIENT_ONLY
	void NextRecordset () throw (_com_error);
#endif
protected:
	CRecordsetBase () 
#ifndef EGLIB_DB_CLIENT_ONLY
		: m_pDBCmd (NULL) 
#endif
	{};
	CRecordsetBase (CursorLocationEnum cl, CursorTypeEnum ct, LockTypeEnum lt) throw (_com_error)
#ifndef EGLIB_DB_CLIENT_ONLY
		: m_pDBCmd (NULL) 
#endif
		{ InitRecordset (cl, ct, lt); } 
		
	virtual ~CRecordsetBase()
	{
		Close();
	}

#ifndef EGLIB_DB_CLIENT_ONLY
	void SetDBCommand (CDBCommand * pDBCmd) {m_pDBCmd = pDBCmd;} ;
#endif

	//FieldPtr GetField (int nIndex ) const throw (_com_error);
	FieldPtr GetField (const _variant_t& vtName ) const throw (_com_error);
private:
	void InitRecordset (CursorLocationEnum cl, CursorTypeEnum ct, LockTypeEnum lt) throw (_com_error);
protected:
	_RecordsetPtr m_spRs;
private:
	typedef std::map < int, FieldPtr > fld_by_idx;
	typedef std::map < _bstr_t, FieldPtr > fld_by_name;

#ifndef EGLIB_DB_CLIENT_ONLY
	CDBCommand * m_pDBCmd;
#endif

	mutable fld_by_idx m_idx_map;
	mutable fld_by_name m_name_map;
};

class CReadOnlyField: public CFieldBase
{
	friend class CDefaultRecordset;
	friend class CClientRecordset;
protected:
	CReadOnlyField (const FieldPtr & spFld) throw (_com_error)
		: CFieldBase (spFld) {}
public:
};

class CDefaultRecordset: public CRecordsetBase
{
protected:
	CDefaultRecordset () throw (_com_error); 
public:
	void MoveNext () throw (_com_error);
	CDefaultRecordset & operator ++ () throw (_com_error) { MoveNext(); return *this; }

	CReadOnlyField operator []  ( const _variant_t& vtName) const throw (_com_error);
	//CReadOnlyField operator []  ( int nIndex) const throw (_com_error);
};


class CClientRecordset: public CRecordsetBase
{
public:
	CClientRecordset () throw (_com_error)
		: CRecordsetBase (adUseClient, adOpenStatic, adLockReadOnly ) {}
	CClientRecordset (IDispatch * pRs) throw (_com_error);
	CClientRecordset (_Recordset * pRs) throw (_com_error);
	CClientRecordset (const CClientRecordset & src) throw (_com_error);
	CClientRecordset (CFieldBase & fld) throw (_com_error);

	CClientRecordset & operator = (const CClientRecordset & src) throw (_com_error);

	void GetAdoRecordset (IDispatch ** ppRs) throw (_com_error);
	void GetAdoRecordset (_Recordset ** ppRs) throw (_com_error);
	
	void Open () throw (_com_error) { CRecordsetBase::Open (); }
	
#ifndef EGLIB_DB_CLIENT_ONLY	
	void Open ( CSQLQuery <> & query ) throw (_com_error) {InternalOpen (query); }
	void Open ( CStoredProc <> & sp ) throw (_com_error) {InternalOpen (sp); }    
#endif
	void MoveLast () throw (_com_error);
	void MoveFirst () throw (_com_error);
	void MovePrev () throw (_com_error);
	void MoveNext () throw (_com_error);
	void SetFilter(_bstr_t bsFilter) throw (_com_error);
	
	CClientRecordset & operator ++ () throw (_com_error) { MoveNext(); return *this; }
	CClientRecordset & operator -- () throw (_com_error) { MovePrev(); return *this; }

	CReadOnlyField operator []  ( const _variant_t& vtName) const throw (_com_error);
	//CReadOnlyField operator []  ( int nIndex) const throw (_com_error);

	int GetRecordCount () throw (_com_error);
protected:
#ifndef EGLIB_DB_CLIENT_ONLY	
	 void InternalOpen ( CDBCommand & cmd ) throw (_com_error);
#endif
};

#include "EgLibDB.inl"

//}; // namespace db_access

#ifndef EGLIB_DB_CLIENT_ONLY

namespace db_manip
{

//using namespace db_access;

inline CDBCommand& clear (CDBCommand& cmd) throw (_com_error)
{
	cmd.ClearParams();
	return cmd;
}
inline CDBCommand& reset (CDBCommand& cmd)
{
	cmd.ResetParams();
	return cmd;
}

}; //namespace db_manip

#endif // EGLIB_DB_CLIENT_ONLY


#ifndef EGLIB_DB_CLIENT_ONLY

//using db_access::CDBConnection;
//using db_access::CStoredProc;
//using db_access::CSQLQuery;
//using db_access::CDefaultRecordset;

#endif // EGLIB_DB_CLIENT_ONLY


#pragma warning(pop)

}; // namespace EgLib

#endif //__EGLIB_DB_ACCESS_H__
