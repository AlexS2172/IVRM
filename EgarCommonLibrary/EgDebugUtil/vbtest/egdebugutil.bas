Attribute VB_Name = "EgDebugUtil"

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Win32 constants and definitions needed by the EgDebugUtil code.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Type EXCEPTION_POINTERS
    ExceptionRecord As Long
    ContextRecord As Long
End Type

Public Const EXCEPTION_EXECUTE_HANDLER = 1
Public Const EXCEPTION_CONTINUE_SEARCH = 0
Public Const EXCEPTION_CONTINUE_EXECUTION = -1

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' The EgDebugUtil.dll functions.
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Public Declare Function SetUnhandledExceptionFilter Lib "Kernel32" _
    (ByVal pFn As Long) As Long

Public Enum MINIDUMP_TYPE
    MiniDumpNormal = &H0
    MiniDumpWithDataSegs = &H1
    MiniDumpWithFullMemory = &H2
    MiniDumpWithHandleData = &H4
End Enum

Public Enum EDUMDRET
    '// Everything worked.
    eDUMP_SUCCEEDED = 0
    '// DBGHELP.DLL could not be found at all in the path.
    eDBGHELP_NOT_FOUND = 1
    '// The mini dump exports are not in the version of DBGHELP.DLL
    '// in memory.
    eDBGHELP_MISSING_EXPORTS = 2
    '// A parameter was bad.
    eBAD_PARAM = 3
    '// Unable to open the dump file requested.
    eOPEN_DUMP_FAILED = 4
    '// MiniDumpWriteDump failed.  Call GetLastError to see why.
    eMINIDUMPWRITEDUMP_FAILED = 5
    '// Death error.  Thread failed to crank up.
    eDEATH_ERROR = 6
    '// The invalid error value.
    eINVALID_ERROR = 7
End Enum

Public Declare Function CreateCurrentProcessMiniDump Lib "EgDebugUtil" Alias "CreateCurrentProcessMiniDumpA" _
    (ByVal enType As MINIDUMP_TYPE, _
     ByVal szDumpFile As String, _
     ByVal hThreadID As Long, _
     ByRef pExPtrs As EXCEPTION_POINTERS) As EDUMDRET
  
Public Declare Function SetMiniDumpDefaultCrashHandler Lib "EgDebugUtil" () As Boolean
 

