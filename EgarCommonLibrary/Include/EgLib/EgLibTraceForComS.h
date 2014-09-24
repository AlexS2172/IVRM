//**************************************************************************************************************//
// abstract:	interface & implementation of strategy classes
//
// content:		-
// copyright:	(c) 2005 EGAR Technology Inc.
//**************************************************************************************************************//
#pragma once

//#include <EgStdLib/Trace.h>
#include <shlobj.h>
#include <EgLib/EgLibStrategy.h>
#include <EgLib/EgLibTimeZone.h>
#include <boost/shared_ptr.hpp>

namespace EgLib 
{	
	typedef enum  enLogLevel
	{
		LogNone		= 0,
		LogSystem	= 1,
		LogFaults	= 2,
		LogError	= 2,
		LogWarning	= 3,
		LogInfoExt	= 4,
		LogInfo		= 5,
		LogSubsExt	= 6,
		LogSubs		= 7,
		LogCustom	= 8,
		LogDebug	= 9,
		LogAll		= LogDebug,
	} enLogLevel;


	namespace EgLibTrace
	{
		struct SLogLine
		{
		public:

			SLogLine() 
				:m_bEasternTimeInitalized(false)
			{ 
				GetLocalTime(&m_st);
				::ZeroMemory(&m_stEastern, sizeof(m_stEastern));
				m_uiThreadID = GetCurrentThreadId();
			}

			std::string		m_strInform;
			std::string		m_strContext;

			long			m_lLogLevel;

		private:
			SYSTEMTIME		m_st;
			SYSTEMTIME      m_stEastern;
			std::string	    m_strBuffer;
			UINT            m_uiThreadID; 

			void			InitializeTime()
			{
				if(!m_bEasternTimeInitalized)
				{
					m_EasternTimeZone.LocalTimeToDestinationTime(m_st, m_stEastern);
					m_bEasternTimeInitalized = true;
				}
			}

		public:
			static EgLib::CEasternTimeZone  m_EasternTimeZone;
			bool							m_bEasternTimeInitalized;

		public:
			const SYSTEMTIME& GetTime()
			{
				InitializeTime();
				return     m_stEastern;
			}

			std::string& GetLogLine()
			{
				InitializeTime();
				char strLog[1024] ;
				sprintf( strLog , "%-2d[0x%04x] %-2.2i:%-2.2i:%-2.2i.%03i %-30s\t\t\t\t%s", m_lLogLevel, m_uiThreadID, m_stEastern.wHour, m_stEastern.wMinute, m_stEastern.wSecond, m_stEastern.wMilliseconds, m_strInform.c_str(),m_strContext.c_str() ) ;
				m_strBuffer = std::string(strLog) ;
				return m_strBuffer;
			}
		};

		__declspec(selectany) EgLib::CEasternTimeZone SLogLine::m_EasternTimeZone; 
		typedef  boost::shared_ptr<SLogLine> SLogLinePtr;
	}

	class CEgLibTraceManager:
		public EgLib::CStrategyT<CEgLibTraceManager,  EgLibTrace::SLogLinePtr >
	{
	private:
		CEgLibTraceManager(){};
	public:

		typedef enum 
		{
			enAbsolute,
			enRelativeCurrent,
			enRelativeMyDocuments
			
		} FilderPathEnum;

		virtual ~CEgLibTraceManager()
		{
			Stop();
		}

		CEgLibTraceManager(FilderPathEnum enPath, _bstr_t sPath)
			:m_lLogLifetime(15) 
			,m_dwCurrentFileSize(0L)
			,m_lMaxSize(100*1024*1024)
			,m_lMinLogLevel(LogAll)
			,m_enFilePathType(enPath)
			,m_bWithTime(true)
		{
			m_EndlineBuf[0] = 0xD;
			m_EndlineBuf[1] = 0xA;
			m_EndlineBuf[2] = 0;

			m_EasternTimeZone.InitializeTimeZone();
			ZeroMemory(&m_stLogLifetime, sizeof(m_stLogLifetime));
			ChangeFilePath(sPath);
			Start();
		}

		//---------------------------------------------------------------------
		//Interface members

		void LogInfo(long LogLevel, LPCSTR bsContext, LPCTSTR szInformation, ...)
		{
			if(LogLevel<=m_lMinLogLevel)
			{
				va_list ptr;
				va_start(ptr, bsContext);
				LogInfo2(LogLevel, bsContext, szInformation, ptr);
				va_end(ptr);
			}
		}

		void LogInfo2(long LogLevel, LPCSTR bsContext, LPCTSTR szInformation, va_list pList)
		{
			if(LogLevel<=m_lMinLogLevel)
			{
				std::string strFormat;
				char strbuff[1024] ;
				sprintf(strbuff,szInformation, pList);
				strFormat = std::string(strbuff) ;
				//strFormat.FormatV(szInformation, pList);
				AddLogInfoQueue(LogLevel, bsContext, strFormat.c_str() );
			}
		}

		__declspec(property(get=GetMaxSize,		put=SetMaxSize))		long	FileMaxSize;
		__declspec(property(get=GetLogLifeTime, put=SetLogLifeTime))	long	LogLifetime;
		__declspec(property(get=GetMinLogLevel, put=SetMinLogLevel))	long	MinLogLevel;
		__declspec(property(get=GetFileName,	put=SetFileName))		_bstr_t FileName;
		__declspec(property(get=GetFilePath,	put=SetFilePath))		_bstr_t FilePath;

	public:

		//---------------------------------------------------------------------
		void OnData(EgLibTrace::SLogLinePtr& data)
		{
			try
			{ 
				TraceToFile(data->GetTime(), data->GetLogLine());
			}
			catch (...)
			{
			}
		}
		//---------------------------------------------------------------------
		void	SetMaxSize(long lMaxSize)		  {m_lMaxSize = lMaxSize;}
		long    GetMaxSize() const				  {return m_lMaxSize;}
		//---------------------------------------------------------------------
		void	SetWithTime(bool _bWithTime)	  { m_bWithTime = _bWithTime ;}
		//---------------------------------------------------------------------
		void    SetLogLifeTime(long lLifeTime)	  {m_lLogLifetime = lLifeTime;}
		long    GetLogLifeTime() const			  {return m_lLogLifetime;}
		//---------------------------------------------------------------------
		void    SetMinLogLevel(long lMinLogLevel) {m_lMinLogLevel = lMinLogLevel;}
		long    GetMinLogLevel() const            {return m_lMinLogLevel;}
		//---------------------------------------------------------------------
		void    SetFileName(_bstr_t bsfileName)   {
													m_bsFileName = bsfileName;
													m_hFileHandle.Close();
													m_dwCurrentFileSize = 0;
												  }
		_bstr_t GetFileName() const				  {return m_bsFileName;}

		//---------------------------------------------------------------------
		void    SetFilePath(_bstr_t bsfilePath)   {
													ChangeFilePath(bsfilePath);
													//m_bsFilePath = bsfilePath;
													m_hFileHandle.Close();
											  	  }
		_bstr_t GetFilePath() const				  {return m_bsFileName;}
		//---------------------------------------------------------------------
		void RemoveOldFiles (void)
		{
	
			_bstr_t bsFileNameMask = EgLib::CEgLibTraceManager::FilePath ;
			bsFileNameMask += _T("*");
			bsFileNameMask += _T(".log");

			EgLib::CEgLibTraceManager::RemoveOldLogFiles ( EgLib::CEgLibTraceManager::FilePath, EgLib::CEgLibTraceManager::FileName, bsFileNameMask ) ;
		}


	
		void TraceToFile(const SYSTEMTIME& st, std::string& szLine);
		void RemoveOldLogFiles(const _bstr_t rsDirName, const _bstr_t rsFileName, const _bstr_t& bsFileNameMask);
		void CreateNewFileName(const SYSTEMTIME& st);


	private:
		void AddLogInfoQueue(long LogLevel, LPCTSTR bsContext, LPCTSTR sInformation)
		{
			EgLibTrace::SLogLinePtr pData = EgLibTrace::SLogLinePtr(new EgLibTrace::SLogLine);

			pData->m_lLogLevel  = LogLevel;
			pData->m_strInform  = sInformation;
			pData->m_strContext  = bsContext;
			if(!IsRunning() && !IsStopping())
				Start();
			//
			PushData(pData);

		}
		void ChangeFilePath(const _bstr_t& sPath)
		{
			switch(m_enFilePathType)
			{
			case enAbsolute:
				m_bsFilePath = sPath;
				m_bsFilePath +="\\";
				break;
			case enRelativeCurrent:
				{
					TCHAR szFilename[MAX_PATH];
					::GetModuleFileName(NULL, szFilename, ARRAY_SIZE(szFilename));
					TCHAR szDrive[_MAX_DRIVE];
					TCHAR szDir[_MAX_DIR];
					TCHAR szFile[_MAX_FNAME];

					_tsplitpath((LPCTSTR)szFilename, szDrive, szDir, szFile, NULL);
					m_bsFilePath = szDrive;
					m_bsFilePath += szDir;
					m_bsFilePath += sPath;
				}
				break;
			case enRelativeMyDocuments:
				{
					TCHAR buffer[MAX_PATH*20];
					ZeroMemory(buffer, sizeof(buffer));
					if( SHGetSpecialFolderPath(NULL, buffer, CSIDL_PERSONAL, false) )
					{   
						DWORD dwError = 0;
						m_bsFilePath  = buffer;
						m_bsFilePath +="\\";
						m_bsFilePath += sPath;
						m_bsFilePath +="\\";
					}
				}break;
			}

			_bstr_t bsPath(L"\\\\?\\");
			bsPath += m_bsFilePath;
			CreateDirectoryW((LPCWSTR)bsPath, NULL);
		}

	private:

		long			m_lMaxSize;
		long			m_lLogLifetime;
		long			m_lMinLogLevel; 
     
		_bstr_t			m_bsFileName;
		_bstr_t			m_bsFilePath;

		SYSTEMTIME		m_stLogLifetime;

		_bstr_t         m_bstrFileNameMask;
		_bstr_t			m_bstrFullFilePath;
		bool			m_bWithTime ;

		CHandle         m_hFileHandle;
		DWORD			m_dwCurrentFileSize;
		char			m_EndlineBuf[4];
		FilderPathEnum	m_enFilePathType;
		static EgLib::CEasternTimeZone  m_EasternTimeZone;


	public:
		// Static Members
		static boost::shared_ptr<CEgLibTraceManager> g_spTrace;
		static void Trace(enLogLevel enLevel, const TCHAR* lpszContext, const TCHAR* lpszMessage, ...)
		{
			if(!lpszMessage || !g_spTrace)
				return;

			va_list arglist;
			va_start(arglist, lpszMessage);
			try
			{
				if(g_spTrace != NULL)
					g_spTrace->LogInfo2(static_cast<long>(enLevel), lpszContext, lpszMessage, arglist);
			}
			catch (...) 
			{
				EGLIBASSERT(false);
			}
		}
		static long GetMinLoggingLevel()
		{
			long lLogLecel = 0;
			try {
				if(g_spTrace != NULL)
					lLogLecel =  g_spTrace->MinLogLevel;
			}catch (...)
			{
				EGLIBASSERT(false);
			}

			return lLogLecel;
		}
		static void SetMinLoggingLevel(long lLogLevel)
		{
			try
			{
				if(g_spTrace != NULL)
					g_spTrace->MinLogLevel = lLogLevel;
			}catch (...)
			{
				EGLIBASSERT(false);
			}

		}


	};  
	typedef boost::shared_ptr<CEgLibTraceManager> CEgLibTraceManagerPtr;

	__declspec(selectany) CEgLibTraceManagerPtr CEgLibTraceManager::g_spTrace;

	inline void CEgLibTraceManager::RemoveOldLogFiles(const _bstr_t rsDirName, const _bstr_t rsFileName, const _bstr_t& rsFileNameMask)
	{
		SYSTEMTIME stLocal = {0};
		SYSTEMTIME st = {0};
		::GetLocalTime(&stLocal);			
		m_EasternTimeZone.LocalTimeToDestinationTime(stLocal, st);

		// we need call this function only once at day!
		if (m_stLogLifetime.wYear != st.wYear   ||
			m_stLogLifetime.wMonth != st.wMonth ||
			m_stLogLifetime.wDay != st.wDay)	// first time?!
			memcpy(&m_stLogLifetime, &st, sizeof(SYSTEMTIME));
		else
			return; // today already clean log-files...

		FILETIME ft;
		::SystemTimeToFileTime(&st, &ft);

		PLARGE_INTEGER pli = reinterpret_cast<PLARGE_INTEGER>(&ft);
		pli->QuadPart -= (DWORD64) m_lLogLifetime* 24 * 60 * 60 * 10000000;

		_bstr_t sFileMask(rsDirName);
		sFileMask = sFileMask + L"\\" + rsFileNameMask;

		WIN32_FIND_DATAW fd;
		HANDLE hFind = ::FindFirstFileW(sFileMask, &fd);
		while (INVALID_HANDLE_VALUE != hFind)
		{
			_bstr_t sFile(rsDirName);
			sFile += fd.cFileName;
			if (::CompareFileTime(&fd.ftCreationTime, &ft) < 0  )
				::DeleteFileW(sFile);

			if (0 == ::FindNextFileW(hFind, &fd))
			{
				::FindClose(hFind);
				hFind = INVALID_HANDLE_VALUE;
			}
		}
	}


	inline void CEgLibTraceManager::CreateNewFileName(const SYSTEMTIME& st)
	{
		if(!m_bsFileName.length())
		{
			TCHAR szFilename[MAX_PATH];
			::GetModuleFileName(NULL, szFilename, ARRAY_SIZE(szFilename));
			TCHAR szDrive[_MAX_DRIVE];
			TCHAR szDir[_MAX_DIR];
			TCHAR szFile[_MAX_FNAME];

			_tsplitpath((LPCTSTR)szFilename, szDrive, szDir, szFile, NULL);
			m_bsFileName  = szFile;
		}

		m_bstrFileNameMask  = m_bsFileName;
		m_bstrFileNameMask += _T("*.Log");

		std::string strDateSuffix;
		// Egar style: MODNAME-YEARMMDD.log

		char strbuff[1024] ;
		sprintf(strbuff,"-%-4.4i%-2.2i%-2.2i-%-2.2i%-2.2i.Log", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);
		strDateSuffix = std::string(strbuff) ;
		//strDateSuffix.Format(_T("-%-4.4i%-2.2i%-2.2i-%-2.2i%-2.2i.Log"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute);

		m_bstrFullFilePath = L"\\\\?\\";
		m_bstrFullFilePath += m_bsFilePath;
		m_bstrFullFilePath += m_bsFileName;
		m_bstrFullFilePath += _bstr_t ( strDateSuffix.c_str() ) ; 
	}

	inline void CEgLibTraceManager::TraceToFile(const SYSTEMTIME& st, std::string& strLine)
	{

		if (0 == m_bstrFullFilePath.length() ||   m_dwCurrentFileSize > (DWORD)m_lMaxSize ||
			(	m_stLogLifetime.wYear  != st.wYear   ||
			m_stLogLifetime.wMonth != st.wMonth  ||
			m_stLogLifetime.wDay   != st.wDay)
			)
		{
			if(m_dwCurrentFileSize > (DWORD)m_lMaxSize && m_hFileHandle !=NULL)
			{
				DWORD dwWriten = 0;
				std::string strFinishMessage;
				strFinishMessage = "The file size exceeds maximum limit. Continue will be on a next volume" ;
				WriteFile(m_hFileHandle, strFinishMessage.c_str(), strFinishMessage.length(), &dwWriten, NULL);
			}
			CreateNewFileName(st);
			RemoveOldLogFiles(m_bsFilePath, m_bsFileName, m_bstrFileNameMask);
			m_hFileHandle.Close();
			m_dwCurrentFileSize = 0;
		}

		if(m_bstrFullFilePath.length() && NULL == (HANDLE)m_hFileHandle)
		{
			m_hFileHandle.Attach(::CreateFileW(
				m_bstrFullFilePath, 
				GENERIC_WRITE , 
				FILE_SHARE_READ, 
				NULL, 
				OPEN_EXISTING, 
				FILE_ATTRIBUTE_NORMAL, 
				NULL));

			if (INVALID_HANDLE_VALUE == (HANDLE)m_hFileHandle || NULL == (HANDLE)m_hFileHandle)
			{
				m_hFileHandle.Detach();
				m_hFileHandle.Attach(::CreateFileW(
					m_bstrFullFilePath, 
					GENERIC_WRITE, 
					FILE_SHARE_READ, 
					NULL, 
					OPEN_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, 
					NULL));
			}
		}
		if (INVALID_HANDLE_VALUE != m_hFileHandle && NULL != (HANDLE)m_hFileHandle)        
		{
			if ( !m_bWithTime ) 
				SetFilePointer ( m_hFileHandle , 0 , 0 , FILE_END )	;
			DWORD dwWriten = 0;
			//::SetFilePointer(hFile, 0, NULL, FILE_END);

			WriteFile(m_hFileHandle, strLine.c_str(), strLine.length(), &dwWriten, NULL);
			m_dwCurrentFileSize+=dwWriten;

			WriteFile(m_hFileHandle, m_EndlineBuf, 2, &dwWriten, NULL);
			m_dwCurrentFileSize+=dwWriten;
		}
	}
	__declspec(selectany) EgLib::CEasternTimeZone CEgLibTraceManager::m_EasternTimeZone; 

};