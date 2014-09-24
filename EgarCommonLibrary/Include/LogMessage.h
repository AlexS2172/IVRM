#pragma once
// Created 11 - 04 - 2005
// Created by Artem Shilaev
// Egar T

#import "..\\..\\ETS\\ETSLogsManager\\_ETSLogsManager.tlb " named_guids , rename_namespace ("EtsLogs")
#include <string>

class CLogMessage
{
	EtsLogs::IETSLog*		m_pILogs ;
	std::string				m_sProfileName ;
public:
	CLogMessage(const std::string& _Profile)
		:m_pILogs(NULL)
		,m_sProfileName(_Profile)
	{
	}
	virtual ~CLogMessage(void)
	{
	}
	void TraceToFile ( EtsLogs::LogLevelEnum  _enLogLevel, const char* _vLine, ...)
	{
	
		TCHAR  szBuffer[1024] = {0};
		LPTSTR pszBuffer = szBuffer;

		va_list ArgList;
		va_start ( ArgList, _vLine );

		::_vsntprintf ( pszBuffer , ARRAY_SIZE(szBuffer), _vLine, ArgList);

		m_pILogs->LogMmInfo ( _enLogLevel , pszBuffer , "" ) ;
	}


	void Init ( ) 
	{
		if ( m_pILogs == NULL )
		{
				
			HRESULT hr ;//= ::CoInitializeEx(NULL, COINIT_MULTITHREADED);

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
			std::string sPath ( cBuff ) ;
			sPath += "\\" ;
			sPath += m_sProfileName.c_str() ;
			memset ( stBuffer , 0 , sizeof(stBuffer) ) ;

			DWORD dwRes = GetPrivateProfileString ( "Service"  , "LogPath" , "d:\\ets_logs" , stBuffer ,sizeof(stBuffer) , sPath.c_str() ) ;
			BOOL bRes = WritePrivateProfileString ( "Service"  , "LogPath" ,  stBuffer , sPath.c_str() ) ;

			m_pILogs->FilePath = _bstr_t ( stBuffer ) ;

			UINT UI = GetPrivateProfileInt( "Service"  , "LogLifeTime" , 15 , sPath.c_str()	) ;
			sprintf ( stBuffer , "%i" , UI ) ;
			bRes = WritePrivateProfileString ( "Service"  , "LogLifeTime" ,  stBuffer , sPath.c_str()  ) ;

			hr = m_pILogs->put_LogLifetime ( UI ) ;

			UI = GetPrivateProfileInt( "Service"  , "LogMaxSize" , 0 , sPath.c_str()  ) ;
			sprintf ( stBuffer , "%i" , UI ) ;
			bRes = WritePrivateProfileString ( "Service"  , "LogMaxSize" ,  stBuffer , sPath.c_str() ) ;

			m_pILogs->PutMinLogLevel ( EtsLogs::enLogAll ) ;
			m_pILogs->RemoveOldFiles() ;
		}

	}
};
