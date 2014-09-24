//**************************************************************************************************************//
// abstract:	interface & implementation of strategy classes
//
// content:		-
// copyright:	(c) 2005 EGAR Technology Inc.
// Created by Artem Shilyaev
//**************************************************************************************************************//
#pragma once
#include <EgLib/EgLibTrace.h>
#include <boost/smart_ptr.hpp>

namespace EgLib 
{	

class CEgLogsMessangerForService
{

	//typedef boost::shared_ptr<CEgLibTraceManager> typeLog ;
	CEgLibTraceManagerPtr m_pLogs ;
	std::string	m_sProfileName ;
public:
	CEgLogsMessangerForService ( LPCTSTR vIniFileName )
		:m_sProfileName(vIniFileName)
	{
	}
	void InitializeService()
	{
		char cBuff[128]  ;
		GetCurrentDirectory ( 128, cBuff );
		char stBuffer[128] ;
		std::string sPath ( cBuff ) ;
		sPath += "\\" ;
		sPath += m_sProfileName.c_str() ;
		memset ( stBuffer , 0 , sizeof(stBuffer) ) ;

		DWORD dwRes = GetPrivateProfileString ( "Service"  , "LogPath" , "c:\\" , stBuffer ,sizeof(stBuffer) , sPath.c_str() ) ;
		BOOL bRes = WritePrivateProfileString ( "Service"  , "LogPath" ,  stBuffer , sPath.c_str() ) ;
		
		m_pLogs = CEgLibTraceManagerPtr  ( new CEgLibTraceManager ( CEgLibTraceManager::enAbsolute , stBuffer ) ) ;
		//m_pLogs->g_spTrace = boost::shared_ptr<CEgLibTraceManager> ( new CEgLibTraceManager( CEgLibTraceManager::enAbsolute , stBuffer ) ) ;
		m_pLogs->g_spTrace  = m_pLogs ;
		
		m_pLogs->SetFilePath ( _bstr_t ( stBuffer ) ) ;

		UINT UI = GetPrivateProfileInt( "Service"  , "LogLifeTime" , 15 , sPath.c_str()	) ;
		sprintf ( stBuffer , "%i" , UI ) ;
		bRes = WritePrivateProfileString ( "Service"  , "LogLifeTime" ,  stBuffer , sPath.c_str()  ) ;

		m_pLogs->SetLogLifeTime ( UI ) ;

		UI = GetPrivateProfileInt( "Service"  , "LogMaxSize" , 0 , sPath.c_str()  ) ;
		sprintf ( stBuffer , "%i" , UI ) ;
		bRes = WritePrivateProfileString ( "Service"  , "LogMaxSize" ,  stBuffer , sPath.c_str() ) ;
		m_pLogs->SetMaxSize( UI * 1024 ) ;

		UI = GetPrivateProfileInt( "Service"  , "MinLogLevel" , 9 , sPath.c_str()  ) ;
		sprintf ( stBuffer , "%i" , UI ) ;
		bRes = WritePrivateProfileString ( "Service"  , "MinLogLevel" ,  stBuffer , sPath.c_str() ) ;

		m_pLogs->SetMinLogLevel ( UI ) ;
		m_pLogs->SetWithTime ( false ) ;
//		m_pLogs->RemoveOldFiles() ;
	}
	void TraceToFile ( enLogLevel _enLogLevel, const char* _vLine, ...)
	{

		TCHAR  szBuffer[1024] = {0};
		LPTSTR pszBuffer = szBuffer;

		va_list ArgList;
		va_start ( ArgList, _vLine );

		::_vsntprintf ( pszBuffer , ARRAY_SIZE(szBuffer), _vLine, ArgList);
		if ( m_pLogs )
		//	m_pLogs->Trace ( _enLogLevel , "" , pszBuffer , pszBuffer ) ;
			m_pLogs->LogInfo2 ( _enLogLevel , "" , pszBuffer , pszBuffer ) ;
	}

};

} ;