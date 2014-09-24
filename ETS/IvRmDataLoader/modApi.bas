Attribute VB_Name = "modApi"
' Reg Key Security Options...
Public Const READ_CONTROL As Long = &H20000
Public Const KEY_QUERY_VALUE As Long = &H1
Public Const KEY_SET_VALUE As Long = &H2
Public Const KEY_CREATE_SUB_KEY As Long = &H4
Public Const KEY_ENUMERATE_SUB_KEYS As Long = &H8
Public Const KEY_NOTIFY As Long = &H10
Public Const KEY_CREATE_LINK As Long = &H20
Public Const KEY_ALL_ACCESS As Long = KEY_QUERY_VALUE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
Public Const KEY_READ As Long = READ_CONTROL + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY

Public Const HKEY_LOCAL_MACHINE As Long = &H80000002
Public Const HKEY_CURRENT_USER As Long = &H80000001
Public Const HKEY_CLASSES_ROOT As Long = &H80000000

Public Const TIME_ZONE_ID_INVALID As Long = &HFFFFFFFF
Public Const TIME_ZONE_ID_UNKNOWN As Long = 0
Public Const TIME_ZONE_ID_STANDARD As Long = 1
Public Const TIME_ZONE_ID_DAYLIGHT As Long = 2

Public Const ERROR_SUCCESS As Long = 0
Public Const REG_SZ As Long = 1                         ' Unicode nul terminated string
Public Const REG_DWORD As Long = 4                      ' 32-bit number

Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Any, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByRef lpData As Any, ByRef lpcbData As Long) As Long
Declare Function RegSetValueExA Lib "advapi32.dll" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long                                ' Note that if you declare the lpData parameter as String, you must pass it By Value.
Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long

Public Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long

Public Declare Function GetComputerName Lib "kernel32.dll" Alias "GetComputerNameA" (Name As Byte, ByRef Size As Long) As Boolean
Public Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpBuffer As String, nSize As Long) As Long
Public Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long

Public Type SystemTime
    wYear As Integer
    wMonth As Integer
    wDayOfWeek As Integer
    wDay As Integer
    wHour As Integer
    wMinute As Integer
    wSecond As Integer
    wMilliseconds As Integer
End Type

Public Type TIME_ZONE_INFORMATION
    Bias As Long
    StandardName(32) As Integer
    StandardDate As SystemTime
    StandardBias As Long
    DaylightName(32) As Integer
    DaylightDate As SystemTime
    DaylightBias As Long
End Type

Public Declare Function GetTimeZoneInformation Lib "kernel32" (lpTimeZoneInformation As TIME_ZONE_INFORMATION) As Long
