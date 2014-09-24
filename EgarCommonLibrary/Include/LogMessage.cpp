#include "StdAfx.h"
#include ".\logmessage.h"

CLogMessage::CLogMessage(void)
:m_pILogs(NULL)
{

}

void CLogMessage::Init ()
{
	if ( m_pILogs == NULL )
	{
		
		HRESULT hr ;

		hr = ::CoCreateInstance
		(
			EtsLogs::CLSID_CETSLog,
			NULL,
			CLSCTX_INPROC_SERVER  ,
			EtsLogs::IID_IETSLog,
			(void**)&m_pILogs
		) ;

		if ( FAILED (hr) )
		{
			m_pILogs = NULL ;
			return ;
		}

		char cBuff[128]  ;
		GetCurrentDirectory ( 128, cBuff );
		char stBuffer[128] ;
		CString sPath ( cBuff ) ;
		sPath += "\\" ;
		sPath += C_ProfileName ;
		memset ( stBuffer , 0 , sizeof(stBuffer) ) ;

		DWORD dwRes = GetPrivateProfileString ( "Service"  , "LogPath" , "d:\\" , stBuffer ,sizeof(stBuffer) , sPath  ) ;
		BOOL bRes = WritePrivateProfileString ( "Service"  , "LogPath" ,  stBuffer , sPath  ) ;

		m_pILogs->FilePath = _bstr_t ( stBuffer ) ;

		UINT UI = GetPrivateProfileInt( "Service"  , "LogLifeTime" , 15 , sPath  ) ;
		sprintf ( stBuffer , "%i" , UI ) ;
		bRes = WritePrivateProfileString ( "Service"  , "LogLifeTime" ,  stBuffer , sPath  ) ;

		hr = m_pILogs->put_LogLifetime ( UI ) ;

		UI = GetPrivateProfileInt( "Service"  , "LogMaxSize" , 0 , sPath  ) ;
		sprintf ( stBuffer , "%i" , UI ) ;
		bRes = WritePrivateProfileString ( "Service"  , "LogMaxSize" ,  stBuffer , sPath  ) ;

		m_pILogs->PutMinLogLevel ( EtsLogs::enLogAll ) ;
		m_pILogs->RemoveOldFiles() ;
	}
}

CLogMessage::~CLogMessage(void)
{
}

void CLogMessage::TraceToFile ( EtsLogs::LogLevelEnum  _enLogLevel, const char* _vLine, ...)
{

	TCHAR  szBuffer[1024] = {0};
	LPTSTR pszBuffer = szBuffer;

	va_list ArgList;
	va_start ( ArgList, _vLine );

	::_vsntprintf ( pszBuffer , ARRAY_SIZE(szBuffer), _vLine, ArgList);

	m_pILogs->LogMmInfo ( _enLogLevel , pszBuffer , "" ) ;
}
