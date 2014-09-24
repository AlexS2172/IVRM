/*//////////////////////////////////////////////////////////////////////
                                Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "EgDebugUtil.h"
// The project internal header file.
#include <crtdbg.h>
/*//////////////////////////////////////////////////////////////////////
          File Specific Defines, Typdefs, Constants, and Structs
//////////////////////////////////////////////////////////////////////*/
// The typedef for the MiniDumpWriteDump function.
typedef BOOL
(WINAPI * PFNMINIDUMPWRITEDUMP)(
    IN HANDLE hProcess,
    IN DWORD ProcessId,
    IN HANDLE hFile,
    IN MINIDUMP_TYPE DumpType,
    IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
    IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
    IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL   ) ;

// The structure I can package the data necessary to the dump writer
// thread.
typedef struct tag_DUMPTHREADPARAMS
{
    MINIDUMP_TYPE        eType                        ;
    wchar_t *            szFileName                   ;
    DWORD                dwThreadID                   ;
    EXCEPTION_POINTERS * pExceptInfo                  ;
    EDUMDRET             eReturnValue                 ;
    DWORD                dwMiniDumpWriteDumpLastError ;
} DUMPTHREADPARAMS , * LPDUMPTHREADPARAMS ;

// Ye ol' DBGHELP.DLL name.
static const TCHAR * k_DBGHELPDLLNAME = _T ( "DBGHELP.DLL" ) ;
// The function name for MiniDumpWriteDump.  Note, this is ANSI as
// that's what GetProcAddress wants.
static const char * k_MINIDUMPWRITEDUMP = "MiniDumpWriteDump" ;

/*//////////////////////////////////////////////////////////////////////
                        File Specific Prototypes
//////////////////////////////////////////////////////////////////////*/
// The dumper function.
unsigned WINAPI DumpThread ( LPVOID pData ) ;

/*//////////////////////////////////////////////////////////////////////
                          File Specific Globals
//////////////////////////////////////////////////////////////////////*/
// The MiniDumpWriteDump function.  I don't want to link to this
// directly as it requires a version if DBGHELP.DLL that most people
// don't have on their machines.
static PFNMINIDUMPWRITEDUMP g_pfnMDWD = NULL ;

// The last error value for IsMiniDumpFunctionAvailible so I don't have
// to go through doing lookup operations all over again.
static EDUMDRET g_eIMDALastError = eINVALID_ERROR ;

/*//////////////////////////////////////////////////////////////////////
                             Implementation
//////////////////////////////////////////////////////////////////////*/

BOOL EGDEBGUTIL_DLLINTERFACE __stdcall IsMiniDumpFunctionAvailable ( void)
{
    // If this is the first time through, always do it.
    if ( NULL == g_pfnMDWD )
    {
        // Find DBGHELP.DLL in memory.
        HINSTANCE hInstDBGHELP = GetModuleHandle ( k_DBGHELPDLLNAME ) ;
        // EgDebugUtil.DLL has it in memory, but someone might excise
        // this file for their own purposes so try and load it.
        if ( NULL == hInstDBGHELP )
        {

#if _MSC_VER <= 1200
			HINSTANCE hMod = GetModuleHandle ( _T ( "EGDEBUGUTIL6.DLL" ) );
#else
			HINSTANCE hMod = GetModuleHandle ( _T ( "EGDEBUGUTIL.DLL" ) );
#endif

			TCHAR szBuf[_MAX_PATH];
			DWORD dwErr = GetModuleFileName(hMod, szBuf, sizeof(szBuf));
			if(dwErr == 0)
				return FALSE;

			TCHAR szDrive[_MAX_DRIVE]= {0};
			TCHAR szDir[_MAX_DIR]={0};
			_tsplitpath_s( szBuf, szDrive,sizeof(szDrive)/sizeof(TCHAR), szDir, sizeof(szDir)/sizeof(TCHAR), NULL,0, NULL,0 );
			TCHAR szFile[_MAX_PATH];
			_stprintf_s(szFile, _T("%s%s%s") , szDrive, szDir, k_DBGHELPDLLNAME);


            hInstDBGHELP = LoadLibrary ( szFile ) ;
        }
        if ( NULL != hInstDBGHELP )
        {
            // At least I have DBGHELP.DLL's handle.  Get the exported
            // function.
            g_pfnMDWD = (PFNMINIDUMPWRITEDUMP)
                 GetProcAddress ( hInstDBGHELP , k_MINIDUMPWRITEDUMP ) ;


            if ( NULL != g_pfnMDWD )
            {
                // It's good so set the last error for this function.
                g_eIMDALastError = eDUMP_SUCCEEDED ;
            }
            else
            {
                // Ain't got the export.
                g_eIMDALastError = eDBGHELP_MISSING_EXPORTS ;
            }

        }
        else
        {
            // Can't find DBGHELP.DLL!  Save this for the
            // CreateCurrentProcessMiniDump function to return.
            g_eIMDALastError = eDBGHELP_NOT_FOUND ;
        }
    }
    // If g_pfnMDWD is not NULL, I found it.
    return ( NULL != g_pfnMDWD ) ;
}

EDUMDRET EGDEBGUTIL_DLLINTERFACE __stdcall
    CreateCurrentProcessMiniDumpA ( MINIDUMP_TYPE        eType      ,
                                    char *               szFileName ,
                                    DWORD                dwThread   ,
                                    EXCEPTION_POINTERS * pExceptInfo )
{
    // Check the string parameter because I am paranoid.
    _ASSERT ( FALSE == IsBadStringPtrA ( szFileName , MAX_PATH ) ) ;
    if ( TRUE == IsBadStringPtrA ( szFileName , MAX_PATH ) )
    {
        return ( eBAD_PARAM ) ;
    }

    // The return value.
    EDUMDRET eRetVal = eDUMP_SUCCEEDED ;

    // Allocate enough space to hold the converted string.
    int iLen = ( lstrlenA ( szFileName ) + 1 ) * sizeof ( wchar_t ) ;
    wchar_t * pWideFileName = (wchar_t*)
                               HeapAlloc ( GetProcessHeap ( )         ,
                                           HEAP_GENERATE_EXCEPTIONS |
                                             HEAP_ZERO_MEMORY         ,
                                           iLen                       );

    int iRet = MultiByteToWideChar ( CP_ACP          ,
                                     MB_PRECOMPOSED  ,
                                     szFileName      ,
                                     -1              ,
                                     pWideFileName   ,
                                     iLen             ) ;
    _ASSERT ( iRet != 0 ) ;
    if ( iRet != 0 )
    {
        // The conversion worked, call the wide function.
        eRetVal = CreateCurrentProcessMiniDumpW ( eType         ,
                                                  pWideFileName ,
                                                  dwThread      ,
                                                  pExceptInfo    ) ;
    }
    else
    {
        eRetVal = eBAD_PARAM ;
    }

    if ( NULL != pWideFileName )
    {
        HeapFree ( GetProcessHeap ( ) , 0 , pWideFileName ) ;
    }

    return ( eRetVal ) ;
}


EDUMDRET EGDEBGUTIL_DLLINTERFACE __stdcall
    CreateCurrentProcessMiniDumpW ( MINIDUMP_TYPE        eType      ,
                                    wchar_t *            szFileName ,
                                    DWORD                dwThread   ,
                                    EXCEPTION_POINTERS * pExceptInfo )
{
    // Check the string parameter because I am paranoid.  I can't check
    // the eType as that might change in the future.
    _ASSERT ( FALSE == IsBadStringPtrW ( szFileName , MAX_PATH ) ) ;
    if ( TRUE == IsBadStringPtrW ( szFileName , MAX_PATH ) )
    {
        return ( eBAD_PARAM ) ;
    }
    // If an exception pointer blob was passed in.
    if ( NULL != pExceptInfo )
    {
        _ASSERT ( FALSE ==
           IsBadReadPtr ( pExceptInfo , sizeof ( EXCEPTION_POINTERS)));
        if ( TRUE ==
            IsBadReadPtr ( pExceptInfo , sizeof ( EXCEPTION_POINTERS)))
        {
            return ( eBAD_PARAM ) ;
        }
    }

    // Have I even tried to get the exported MiniDumpWriteDump function
    // yet?
    if ( ( NULL == g_pfnMDWD ) && ( eINVALID_ERROR == g_eIMDALastError))
    {
        if ( FALSE == IsMiniDumpFunctionAvailable ( ) )
        {
            return ( g_eIMDALastError ) ;
        }
    }
    // If the MiniDumpWriteDump function pointer is NULL, return
    // whatever was in g_eIMDALastError.
    if ( NULL == g_pfnMDWD )
    {
        return ( g_eIMDALastError ) ;
    }

    // Package up the data for the dump writer thread.
    DUMPTHREADPARAMS stParams ;
    stParams.eReturnValue = eINVALID_ERROR  ;
    stParams.eType        = eType           ;
    stParams.pExceptInfo  = pExceptInfo     ;
    stParams.dwThreadID   = dwThread        ;
    stParams.szFileName   = szFileName      ;
    stParams.dwMiniDumpWriteDumpLastError = ERROR_SUCCESS ;

    // Crank the writer thread.
    unsigned dwTID ;
    HANDLE hThread = (HANDLE)_beginthreadex ( NULL        ,
                                              0           ,
                                              DumpThread  ,
                                              &stParams   ,
                                              0           ,
                                              &dwTID       ) ;
    _ASSERT ( (HANDLE)-1 != hThread ) ;
    if ( (HANDLE)-1 != hThread )
    {
        // The thread is running.  Block until the thread ends.
        WaitForSingleObject ( hThread , INFINITE ) ;

        // Close the handle.
         CloseHandle ( hThread ) ;

    }
    else
    {
        stParams.dwMiniDumpWriteDumpLastError = GetLastError ( ) ;
        stParams.eReturnValue = eDEATH_ERROR ;
    }

    // Set the last error code based so it looks like this thread made
    // the call to MiniDumpWriteDump.
    SetLastError ( stParams.dwMiniDumpWriteDumpLastError ) ;

    return ( stParams.eReturnValue ) ;
}


/*----------------------------------------------------------------------
FUNCTION        :   DumpThread
DISCUSSION      :
    The function that does all the work.
PARAMETERS      :
    pData - A pointer to a DUMPTHREADPARAMS structure.
RETURNS         :
    FALSE - There was a problem dumping the data.  The DUMPTHREADPARAMS
            struct contains the problem.
    TRUE  - All OK, Jumpmaster!
----------------------------------------------------------------------*/
unsigned WINAPI DumpThread ( LPVOID pData )
{
    LPDUMPTHREADPARAMS pParams = (LPDUMPTHREADPARAMS)pData ;

    // Create the file first.
    HANDLE hFile = CreateFileW ( pParams->szFileName             ,
                                 GENERIC_READ | GENERIC_WRITE    ,
                                  0                              ,
                                  NULL                           ,
                                  CREATE_ALWAYS                  ,
                                  FILE_ATTRIBUTE_NORMAL          ,
                                  NULL                            ) ;
    _ASSERT ( INVALID_HANDLE_VALUE != hFile ) ;
    if ( INVALID_HANDLE_VALUE != hFile )
    {
        MINIDUMP_EXCEPTION_INFORMATION   stMDEI ;
        MINIDUMP_EXCEPTION_INFORMATION * pMDEI = NULL ;

        if ( NULL != pParams->pExceptInfo )
        {
            stMDEI.ThreadId = pParams->dwThreadID ;
            stMDEI.ExceptionPointers = pParams->pExceptInfo ;
            stMDEI.ClientPointers = TRUE ;
            pMDEI = &stMDEI ;
        }

        // Got the file open.  Write it.
        BOOL bRet = g_pfnMDWD ( GetCurrentProcess ( )   ,
                                GetCurrentProcessId ( ) ,
                                hFile                   ,
                                pParams->eType          ,
                                pMDEI                   ,
                                NULL                    ,
                                NULL                     ) ;
        _ASSERT ( TRUE == bRet ) ;
        if ( TRUE == bRet )
        {
            pParams->eReturnValue = eDUMP_SUCCEEDED ;
        }
        else
        {
            // Oops.
            pParams->eReturnValue = eMINIDUMPWRITEDUMP_FAILED ;
        }

        // Close the open file.
         CloseHandle ( hFile )  ;
    }
    else
    {
        // Could not open the file!
        pParams->eReturnValue = eOPEN_DUMP_FAILED ;
    }
    // Always save the last error value so I can set it in the original
    // thread.
    pParams->dwMiniDumpWriteDumpLastError = GetLastError ( ) ;
    return ( eDUMP_SUCCEEDED == pParams->eReturnValue ) ;
}

LONG __stdcall MiniDumpDefaultCrashHandler( EXCEPTION_POINTERS * pExPtrs )
{
	TCHAR szBuf[_MAX_PATH];
	DWORD dwErr = GetModuleFileName(NULL, szBuf, sizeof(szBuf));
	if(dwErr == 0)
		EXCEPTION_CONTINUE_SEARCH;

	TCHAR szDrive[_MAX_DRIVE]= {0};
	TCHAR szDir[_MAX_DIR]    = {0};
	TCHAR szFname[_MAX_FNAME]= {0};
	TCHAR szExt[_MAX_EXT]    = {0};
	_tsplitpath_s( szBuf, szDrive, sizeof(szDrive)/sizeof(TCHAR), szDir, sizeof(szDir)/sizeof(TCHAR), szFname, sizeof(szFname)/sizeof(TCHAR), szExt, sizeof(szExt)/sizeof(TCHAR) );

	TCHAR szMiniDumpFile[_MAX_PATH];
	_stprintf_s(szMiniDumpFile, _T("%s%s%s.DMP"), szDrive, szDir, szFname);

    EDUMDRET eCCPMD ;
    eCCPMD = CreateCurrentProcessMiniDump ( MiniDumpWithHandleData    ,
                                             szMiniDumpFile         ,
                                             GetCurrentThreadId ( )    ,
                                             pExPtrs              ) ;
    _ASSERT ( eDUMP_SUCCEEDED == eCCPMD ) ;


	TCHAR szError[1024];
	_stprintf_s(szError, _T("The %s%s has encountered with a problem and will be closed.\n" 
		"Some information you were working on might be not saved.\n"
		"We are sorry for the caused inconvenience.\n\n"
		"Please send the error report located in \n"
		"%s \n"
		"to EGAR support team.\n\n"
		"We will analyze and solve this problem ASAP. This report is threated \n"
		"as confidential and anonymous."),
		szFname, szExt, szMiniDumpFile);

	TCHAR szFile[_MAX_PATH];
	_stprintf_s(szFile, _T("%s%s") , szFname, szExt);

	MessageBox(NULL, szError, szFile, MB_SERVICE_NOTIFICATION | MB_OK | MB_ICONERROR);

   return ( EXCEPTION_EXECUTE_HANDLER ) ;
};

BOOL EGDEBGUTIL_DLLINTERFACE __stdcall SetMiniDumpDefaultCrashHandler ( void)
{
	if(!IsMiniDumpFunctionAvailable())
		return FALSE;
	else
	{
		SetUnhandledExceptionFilter(MiniDumpDefaultCrashHandler);
		return TRUE;
	}
}