Attribute VB_Name = "modAPI"
' **************************************************************
' Module            modAPI
'
' Filename          modAPI.bas
'
' Author            Oleg Sharkevich
'                   eGar Technology Inc.
'
' Description
'
' Win32 API declarations module for ETS
'
' **************************************************************

Option Explicit

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
                     
' Reg Key ROOT Types...
Public Const HKEY_LOCAL_MACHINE As Long = &H80000002
Public Const HKEY_CURRENT_USER As Long = &H80000001
Public Const HKEY_CLASSES_ROOT As Long = &H80000000

Public Const ERROR_SUCCESS As Long = 0
Public Const REG_SZ As Long = 1                         ' Unicode nul terminated string
Public Const REG_DWORD As Long = 4                      ' 32-bit number

Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Any, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByRef lpData As Any, ByRef lpcbData As Long) As Long
Declare Function RegSetValueExA Lib "advapi32.dll" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long                                ' Note that if you declare the lpData parameter as String, you must pass it By Value.
Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
'Public Declare Function GetTickCount Lib "kernel32" () As Long
Public Declare Function timeGetTime Lib "winmm.dll" () As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Public Declare Function GetForegroundWindow Lib "user32" () As Long
Public Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long

Public Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
Public Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

Public Const SM_CXBORDER As Long = 5
Public Const SM_CYBORDER As Long = 6
Public Const SM_CYFRAME As Long = 33
Public Const SM_CXFRAME As Long = 32
Public Const SM_CXSIZEFRAME As Long = SM_CXFRAME
Public Const SM_CYSIZEFRAME As Long = SM_CYFRAME
Public Const SM_CYCAPTION As Long = 4
Public Const SM_CYMENU As Long = 15
Public Const SM_CXFULLSCREEN As Long = 16
Public Const SM_CYFULLSCREEN As Long = 17
Public Const SM_CYHSCROLL As Long = 3
Public Const SM_CXVSCROLL As Long = 2

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, Source As Any, ByVal numBytes As Long)

Public Type POINTAPI
    X As Long
    Y As Long
End Type

Public Type RECT
    Left As Long
    Top As Long
    Right As Long
    Bottom As Long
End Type

Public Type MINMAXINFO
    ptReserved As POINTAPI
    ptMaxSize As POINTAPI
    ptMaxPosition As POINTAPI
    ptMinTrackSize As POINTAPI
    ptMaxTrackSize As POINTAPI
End Type

' Win32 API consts for internal using
Public Const WM_ACTIVATEAPP As Long = &H1C
Public Const WM_CAPTURECHANGED As Long = &H215
Public Const WM_CHAR As Long = &H102
Public Const WM_CLOSE As Long = &H10
Public Const WM_COMMAND As Long = &H111
Public Const WM_COMPACTING As Long = &H41
Public Const WM_CONTEXTMENU As Long = &H7B
Public Const WM_COPYDATA As Long = &H4A
Public Const WM_DEVMODECHANGE As Long = &H1B
Public Const WM_DEVICECHANGE As Long = &H219
Public Const WM_DISPLAYCHANGE As Long = &H7E
Public Const WM_DROPFILES As Long = &H233
Public Const WM_ENDSESSION As Long = &H16
Public Const WM_ENTERMENULOOP As Long = &H211
Public Const WM_ERASEBKGND As Long = &H14
Public Const WM_EXITMENULOOP As Long = &H212
Public Const WM_FONTCHANGE As Long = &H1D
Public Const WM_GETMINMAXINFO As Long = &H24
Public Const WM_HOTKEY As Long = &H312
Public Const WM_HSCROLL As Long = &H114
Public Const WM_KEYDOWN As Long = &H100
Public Const WM_KEYUP As Long = &H101
Public Const WM_KILLFOCUS As Long = &H8
Public Const WM_LBUTTONDBLCLK As Long = &H203
Public Const WM_LBUTTONDOWN As Long = &H201
Public Const WM_LBUTTONUP As Long = &H202
Public Const WM_MBUTTONDBLCLK As Long = &H209
Public Const WM_MBUTTONDOWN As Long = &H207
Public Const WM_MBUTTONUP As Long = &H208
Public Const WM_MENUCHAR As Long = &H120
Public Const WM_MENUSELECT As Long = &H11F
Public Const WM_MOUSEACTIVATE As Long = &H21
Public Const WM_MOUSEMOVE As Long = &H200
Public Const WM_MOVE As Long = &H3
Public Const WM_MOVING As Long = &H216
Public Const WM_NCACTIVATE As Long = &H86
Public Const WM_NCHITTEST As Long = &H84
Public Const WM_NCLBUTTONDBLCLK As Long = &HA3
Public Const WM_NCLBUTTONDOWN As Long = &HA1
Public Const WM_NCLBUTTONUP As Long = &HA2
Public Const WM_NCMBUTTONDBLCLK As Long = &HA9
Public Const WM_NCMBUTTONDOWN As Long = &HA7
Public Const WM_NCMBUTTONUP As Long = &HA8
Public Const WM_NCMOUSEMOVE As Long = &HA0
Public Const WM_NCPAINT As Long = &H85
Public Const WM_NCRBUTTONDBLCLK As Long = &HA6
Public Const WM_NCRBUTTONDOWN As Long = &HA4
Public Const WM_NCRBUTTONUP As Long = &HA5
Public Const WM_NOTIFY As Long = &H4E
Public Const WM_OTHERWINDOWCREATED As Long = &H42
Public Const WM_OTHERWINDOWDESTROYED As Long = &H43
Public Const WM_PAINT As Long = &HF
Public Const WM_PALETTECHANGED As Long = &H311
Public Const WM_PALETTEISCHANGING As Long = &H310
Public Const WM_POWER As Long = &H48
Public Const WM_POWERBROADCAST As Long = &H218
Public Const WM_QUERYENDSESSION As Long = &H11
Public Const WM_QUERYNEWPALETTE As Long = &H30F
Public Const WM_QUERYOPEN As Long = &H13
Public Const WM_RBUTTONDBLCLK As Long = &H206
Public Const WM_RBUTTONDOWN As Long = &H204
Public Const WM_RBUTTONUP As Long = &H205
Public Const WM_SETCURSOR As Long = &H20
Public Const WM_SETFOCUS As Long = &H7
Public Const WM_SETTINGCHANGE As Long = &H1A
Public Const WM_SIZE As Long = &H5
Public Const WM_SIZING As Long = &H214
Public Const WM_SPOOLERSTATUS As Long = &H2A
Public Const WM_SYSCOLORCHANGE As Long = &H15
Public Const WM_SYSCOMMAND As Long = &H112
Public Const WM_TIMECHANGE As Long = &H1E
Public Const WM_USERCHANGED As Long = &H54
Public Const WM_VSCROLL As Long = &H115
Public Const WM_WININICHANGE As Long = &H1A


Declare Function GetUpdateRect Lib "user32" (ByVal hwnd As Long, lpRect As RECT, ByVal bErase As Long) As Long

Public Const HWND_TOP As Long = 0
Public Const HWND_TOPMOST As Long = -1
Public Const HWND_NOTOPMOST As Long = -2
Public Const SWP_NOMOVE As Long = &H2
Public Const SWP_NOSIZE As Long = &H1
Public Const SWP_NOACTIVATE As Long = &H10
Public Const SW_SHOWNOACTIVATE As Long = 4
Public Const SW_SHOWNA As Long = 4
Public Const SW_SHOWNORMAL As Long = 1
Public Const SW_SHOW As Long = 5
Public Const SW_RESTORE As Long = 9
Public Const SW_SHOWMINNOACTIVE As Long = 7
Public Const SW_SHOWMAXIMIZED As Long = 3
Public Const SW_SHOWMINIMIZED As Long = 2
Public Const SW_MAXIMIZE As Long = 3
Public Const SW_MINIMIZE As Long = 6

Public Declare Function SetWindowPos Lib "user32" (ByVal hwnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal Y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long
Public Declare Function ShowWindow Lib "user32" (ByVal hwnd As Long, ByVal nCmdShow As Long) As Long
Public Declare Function IsIconic Lib "user32" (ByVal hwnd As Long) As Long


'Public Type MONITORINFO
'    cbSize As Long
'    rcMonitor As RECT
'    rcWork As RECT
'    dwFlags As Long
'End Type

Type OSVERSIONINFOEX
    dwOSVersionInfoSize As Long
    dwMajorVersion As Long
    dwMinorVersion As Long
    dwBuildNumber As Long
    dwPlatformId As Long
    szCSDVersion As String * 128
    wServicePackMajor As Integer
    wServicePackMinor As Integer
    wSuiteMask As Integer
    wProductType As Byte
    wReserved As Byte
End Type

Public Const VER_PLATFORM_WIN32_WINDOWS As Long = 1
Public Const VER_PLATFORM_WIN32_NT As Long = 2
Public Declare Function GetVersionEx Lib "kernel32" Alias "GetVersionExA" (LpVersionInformation As OSVERSIONINFOEX) As Long

Public Const SPI_GETWORKAREA = 48
Public Declare Function SystemParametersInfo Lib "user32" Alias "SystemParametersInfoA" (ByVal uAction As Long, ByVal uParam As Long, ByRef lpvParam As Any, ByVal fuWinIni As Long) As Long

Public Type WINDOWPLACEMENT
    Length As Long
    Flags As Long
    showCmd As Long
    ptMinPosition As POINTAPI
    ptMaxPosition As POINTAPI
    rcNormalPosition As RECT
End Type

Public Declare Function GetWindowPlacement Lib "user32" (ByVal hwnd As Long, lpwndpl As WINDOWPLACEMENT) As Long
Public Declare Function SetWindowPlacement Lib "user32" (ByVal hwnd As Long, lpwndpl As WINDOWPLACEMENT) As Long
Public Declare Function LockWindowUpdate Lib "user32" (ByVal hwndLock As Long) As Long

Public Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Public Const SWP_NOZORDER As Long = &H4&
Public Const SWP_HIDEWINDOW As Long = &H80
Public Const SWP_SHOWWINDOW As Long = &H40
Public Const WS_EX_TOOLWINDOW As Long = &H80&
Public Const WS_EX_APPWINDOW As Long = &H40000
Public Const GWL_EXSTYLE As Long = (-20)
Public Const GWL_STYLE As Long = (-16)
Public Const WS_MINIMIZEBOX As Long = &H20000
Public Const WS_HSCROLL As Long = &H100000

Public Const STANDARD_RIGHTS_REQUIRED As Long = &HF0000
Public Const MUTANT_QUERY_STATE As Long = &H1
Public Const SYNCHRONIZE As Long = &H100000
Public Const MUTEX_ALL_ACCESS As Long = STANDARD_RIGHTS_REQUIRED + SYNCHRONIZE + MUTANT_QUERY_STATE

Public Declare Function OpenMutex Lib "kernel32" Alias "OpenMutexA" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal lpName As String) As Long
Public Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As Long, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Public Declare Function GetFocus Lib "user32" () As Long
Public Declare Function GetCursorPos Lib "user32" (lpPoint As POINTAPI) As Long
Public Declare Function WindowFromPoint Lib "user32" (ByVal xPoint As Long, ByVal yPoint As Long) As Long
Public Declare Function GetCapture Lib "user32" () As Long
Public Declare Function ReleaseCapture Lib "user32" () As Long
Public Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long

Public Declare Function CreateEvent Lib "kernel32" Alias "CreateEventA" (lpEventAttributes As Any, ByVal bManualReset As Long, ByVal bInitialState As Long, lpName As Any) As Long
Public Declare Function SetEvent Lib "kernel32" (ByVal hEvent As Long) As Long
Public Declare Function ResetEvent Lib "kernel32" (ByVal hEvent As Long) As Long
Public Declare Function MsgWaitForMultipleObjects Lib "user32" (ByVal nCount As Long, pHandles As Long, ByVal fWaitAll As Long, ByVal dwMilliseconds As Long, ByVal dwWakeMask As Long) As Long

Public Const QS_HOTKEY& = &H80
Public Const QS_KEY& = &H1
Public Const QS_MOUSEBUTTON& = &H4
Public Const QS_MOUSEMOVE& = &H2
Public Const QS_PAINT& = &H20
Public Const QS_POSTMESSAGE& = &H8
Public Const QS_SENDMESSAGE& = &H40
Public Const QS_TIMER& = &H10
Public Const QS_MOUSE& = (QS_MOUSEMOVE Or QS_MOUSEBUTTON)
Public Const QS_INPUT& = (QS_MOUSE Or QS_KEY)
Public Const QS_ALLEVENTS& = (QS_INPUT Or QS_POSTMESSAGE Or QS_TIMER Or QS_PAINT Or QS_HOTKEY)
Public Const QS_ALLINPUT& = (QS_SENDMESSAGE Or QS_PAINT Or QS_TIMER Or QS_POSTMESSAGE Or QS_MOUSEBUTTON Or QS_MOUSEMOVE Or QS_HOTKEY Or QS_KEY)
                            
Public Const INFINITE As Long = &HFFFF      '  Infinite timeout
Public Const WAIT_OBJECT_0 As Long = 0

Public Type LARGE_INTEGER
    LowPart As Long
    HighPart As Long
End Type

Public Declare Function CreateWaitableTimer Lib "kernel32" Alias "CreateWaitableTimerA" (lpTimerAttributes As Any, ByVal bManualReset As Long, lpTimerName As Any) As Long
Public Declare Function SetWaitableTimer Lib "kernel32" (ByVal hTimer As Long, lpDueTime As LARGE_INTEGER, ByVal lPeriod As Long, ByVal pfnCompletionRoutine As Long, ByVal lpArgToCompletionRoutine As Long, ByVal fResume As Long) As Long

Public Const OBJID_MENU As Long = &HFFFFFFFD
Public Const OBJID_CLIENT As Long = &HFFFFFFFC
Public Const OBJID_SYSMENU As Long = &HFFFFFFFF

Public Type MENUBARINFO
    cbSize As Long
    rcBar As RECT
    hMenu As Long
    hwndMenu As Long
    fBarFocused_fFocused As Long
End Type

Public Declare Function GetMenu Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function GetMenuBarInfo Lib "user32" (ByVal hwnd As Long, ByVal idObject As Long, ByVal idItem As Long, ByRef pmbi As MENUBARINFO) As Long
Public Declare Function GetLastError Lib "kernel32" () As Long
Public Declare Function GetMenuItemCount Lib "user32" (ByVal hMenu As Long) As Long

Public Const TIME_ZONE_ID_INVALID As Long = &HFFFFFFFF
Public Const TIME_ZONE_ID_UNKNOWN As Long = 0
Public Const TIME_ZONE_ID_STANDARD As Long = 1
Public Const TIME_ZONE_ID_DAYLIGHT As Long = 2

Public Type SYSTEMTIME
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
    StandardDate As SYSTEMTIME
    StandardBias As Long
    DaylightName(32) As Integer
    DaylightDate As SYSTEMTIME
    DaylightBias As Long
End Type

Public Declare Function GetTimeZoneInformation Lib "kernel32" (lpTimeZoneInformation As TIME_ZONE_INFORMATION) As Long

Public Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
Public Declare Function GetVolumeInformation Lib "kernel32" Alias "GetVolumeInformationA" (ByVal lpRootPathName As String, lpVolumeNameBuffer As Any, ByVal nVolumeNameSize As Long, lpVolumeSerialNumber As Long, lpMaximumComponentLength As Any, lpFileSystemFlags As Any, lpFileSystemNameBuffer As Any, ByVal nFileSystemNameSize As Long) As Long

Public Const EM_SETTABSTOPS As Long = &HCB
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hwnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long

Public Declare Function GetLongPathName Lib "kernel32" Alias "GetLongPathNameA" (ByVal lpszShortPath As String, ByVal lpszLongPath As String, ByVal cchBuffer As Long) As Long

Public Const ES_UPPERCASE& = &H8&
Public Const CBS_UPPERCASE& = &H2000&

Public Const COLOR_WINDOW& = 5&
Public Const COLOR_WINDOWTEXT& = 8&
Public Declare Function GetSysColor Lib "user32" (ByVal nIndex As Long) As Long

Public Declare Function GetCurrentThreadId Lib "kernel32" () As Long

Public Declare Function LoadLibrary Lib "kernel32" Alias "LoadLibraryA" (ByVal lpLibFileName As String) As Long
Public Declare Function FreeLibrary Lib "kernel32" (ByVal hLibModule As Long) As Long
Public Declare Function GetModuleHandle Lib "kernel32" Alias "GetModuleHandleA" (ByVal lpModuleName As String) As Long

Public Const EM_LIMITTEXT As Long = &HC5
Public Const CB_LIMITTEXT As Long = &H141

Public Declare Function FindWindow Lib "user32" Alias "FindWindowA" (lpClassName As Any, ByVal lpWindowName As String) As Long

Public Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
End Type

Public Declare Function CoCreateGuid Lib "ole32" (ByRef pguid As GUID) As Long
Public Declare Function CLSIDFromProgID Lib "ole32.dll" (ByVal pOLESTR As Long, ByRef pCLSID As GUID) As Long
Public Declare Function FlashWindow Lib "user32" (ByVal hwnd As Long, ByVal bInvert As Long) As Long
Public Declare Function GetTempPath Lib "kernel32" Alias "GetTempPathA" (ByVal nBufferLength As Long, ByVal lpBuffer As String) As Long

Public Const VK_LSHIFT& = &HA0
Public Const VK_LMENU& = &HA4
Public Const VK_LCONTROL& = &HA2
Public Const VK_RSHIFT& = &HA1
Public Const VK_RMENU& = &HA5
Public Const VK_RCONTROL& = &HA3
Public Const VK_SHIFT& = &H10
Public Const VK_CONTROL& = &H11
Public Const VK_MENU& = &H12
Public Declare Function GetKeyState Lib "user32" (ByVal nVirtKey As Long) As Integer

Public Const WM_ENABLE& = &HA
Public Const WM_SETFONT& = &H30&
Public Const WM_GETFONT& = &H31&
Public Const WS_EX_STATICEDGE& = &H20000
Public Const WS_EX_TRANSPARENT& = &H20&
Public Const WS_CHILD& = &H40000000

Public Type CREATESTRUCT
    lpCreateParams As Long
    hInstance As Long
    hMenu As Long
    hWndParent As Long
    cy As Long
    cx As Long
    Y As Long
    X As Long
    style As Long
    lpszName As String
    lpszClass As String
    ExStyle As Long
End Type

Public Const ICC_INTERNET_CLASSES& = &H800

Public Type InitCommonControlsExType
    dwSize As Long
    dwICC As Long
End Type

Public Declare Sub InitCommonControls Lib "comctl32" ()
Public Declare Function InitCommonControlsEx Lib "comctl32" (Init As InitCommonControlsExType) As Boolean

Public Declare Function CreateWindowEx Lib "user32" Alias "CreateWindowExA" (ByVal dwExStyle As Long, ByVal lpClassName As String, ByVal lpWindowName As String, ByVal dwStyle As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hWndParent As Long, ByVal hMenu As Long, ByVal hInstance As Long, lpParam As Any) As Long
Public Declare Function DestroyWindow Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function MoveWindow Lib "user32" (ByVal hwnd As Long, ByVal X As Long, ByVal Y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal bRepaint As Long) As Long

Public Const WM_USER& = &H400
Public Const IPM_CLEARADDRESS& = (WM_USER + 100)
Public Const IPM_GETADDRESS& = (WM_USER + 102)
Public Const IPM_ISBLANK& = (WM_USER + 105)
Public Const IPM_SETADDRESS& = (WM_USER + 101)
Public Const IPM_SETFOCUS& = (WM_USER + 104)
Public Const IPM_SETRANGE& = (WM_USER + 103)

Public Const LB_FINDSTRING& = &H18F&
Public Const LB_FINDSTRINGEXACT& = &H1A2&
Public Const LB_ERR& = -1&

Public Const SB_BOTTOM& = 7
Public Const SB_LINEDOWN& = 1
Public Const VK_DOWN& = &H28

Type OemId
    wProcessorArchitecture As Integer
    wReserved As Integer
End Type

Type SYSTEM_INFO
    dwOemId As OemId
    dwPageSize As Long
    lpMinimumApplicationAddress As Long
    lpMaximumApplicationAddress As Long
    dwActiveProcessorMask As Long
    dwNumberOfProcessors As Long
    dwProcessorType As Long
    dwAllocationGranularity As Long
    wProcessorLevel As Integer
    wProcessorRevision As Integer
End Type

Type MEMORYSTATUS
    dwLength As Long
    dwMemoryLoad As Long
    dwTotalPhys As Long
    dwAvailPhys As Long
    dwTotalPageFile As Long
    dwAvailPageFile As Long
    dwTotalVirtual As Long
    dwAvailVirtual As Long
End Type

Public Declare Sub GlobalMemoryStatus Lib "kernel32" (lpBuffer As MEMORYSTATUS)
Public Declare Sub GetSystemInfo Lib "kernel32" (lpSystemInfo As SYSTEM_INFO)

Public Const VER_NT_WORKSTATION As Long = &H1
Public Const VER_NT_DOMAIN_CONTROLLER As Long = &H2
Public Const VER_NT_SERVER As Long = &H3
Public Const VER_WORKSTATION_NT As Long = &H40000000
' Microsoft Small Business Server
Public Const VER_SUITE_SMALLBUSINESS As Long = &H1&
' Win2k Adv Server or .Net Enterprise Server
Public Const VER_SUITE_ENTERPRISE As Long = &H2&
' Terminal Services is installed.
Public Const VER_SUITE_TERMINAL As Long = &H10&
' Win2k Datacenter
Public Const VER_SUITE_DATACENTER As Long = &H80&
' Terminal server in remote admin mode
Public Const VER_SUITE_SINGLEUSERTS As Long = &H100&
Public Const VER_SUITE_PERSONAL As Long = &H200&
' Microsoft .Net webserver installed
Public Const VER_SUITE_BLADE As Long = &H400

'Performance monitor functions for Visual Basic from PDH.DLL
Declare Function PdhVbOpenQuery Lib "pdh.dll" (ByRef QueryHandle As Long) As Long
Declare Function PdhCloseQuery Lib "pdh.dll" (ByVal QueryHandle As Long) As Long
Declare Function PdhVbAddCounter Lib "pdh.dll" (ByVal QueryHandle As Long, ByVal CounterPath As String, ByRef CounterHandle As Long) As Long
Declare Function PdhRemoveCounter Lib "pdh.dll" (ByVal CounterHandle As Long) As Long
Declare Function PdhCollectQueryData Lib "pdh.dll" (ByVal QueryHandle As Long) As Long
Declare Function PdhVbGetDoubleCounterValue Lib "pdh.dll" (ByVal CounterHandle As Long, ByRef CounterStatus As Long) As Double
Declare Function PdhVbIsGoodStatus Lib "pdh.dll" (ByVal StatusValue As Long) As Long

Public Type MapiMessage
    Reserved As Long
    Subject As String
    NoteText As String
    MessageType As String
    DateReceived As String
    ConversiondID As String
    Flags As Long
    Originator As Long
    RecipCount As Long
    FileCount As Long
End Type

Public Type MapiRecip
    Reserved As Long
    RecipClass As Long
    Name As String
    address As String
    EIDSize As Long
    EntryID As String
End Type

Public Type Mapifile
    Reserved As Long
    Flags As Long
    Position As Long
    PathName As String
    FileName As String
    FileType As String
End Type

Public Const MAPI_TO& = 1&
Public Const MAPI_CC& = 2&
Public Const MAPI_BCC& = 3&
Public Const MAPI_DIALOG& = &H8&

Declare Function MAPISendMail Lib "MAPI32.DLL" Alias "BMAPISendMail" (ByVal Session&, ByVal UIParam&, message As MapiMessage, Recipient() As MapiRecip, File() As Mapifile, ByVal Flags&, ByVal Reserved&) As Long

Declare Function MAPIResolveName Lib "MAPI32.DLL" Alias "BMAPIResolveName" (ByVal Session&, ByVal UIParam&, ByVal UserName$, ByVal Flags&, ByVal Reserved&, Recipient As MapiRecip) As Long

Public Declare Function GetDesktopWindow Lib "user32" () As Long

Public Declare Function SetMiniDumpDefaultCrashHandler Lib "EgDebugUtil" () As Boolean

Public Const BIF_RETURNONLYFSDIRS& = 1
Public Const BIF_DONTGOBELOWDOMAIN& = 2
Public Const BIF_STATUSTEXT& = &H4&
Public Const BIF_NEWDIALOGSTYLE& = &H40&
Public Const MAX_PATH& = 260

Public Type BrowseInfo
   hWndOwner As Long
   pIDLRoot As Long
   pszDisplayName As Long
   lpszTitle As Long
   ulFlags As Long
   lpfnCallback As Long
   lParam As Long
   iImage As Long
End Type

Public Declare Function SHBrowseForFolder Lib "shell32" (lpbi As BrowseInfo) As Long
Public Declare Function SHGetPathFromIDList Lib "shell32" (ByVal pidList As Long, ByVal lpBuffer As String) As Long
Public Declare Function lstrcat Lib "kernel32" Alias "lstrcatA" (ByVal lpString1 As String, ByVal lpString2 As String) As Long

Public Const BFFM_INITIALIZED = 1
Public Const BFFM_SELCHANGED = 2
Public Const BFFM_SETSTATUSTEXT& = (WM_USER + 100)
Public Const BFFM_SETSELECTION& = (WM_USER + 102)

Public Const CSIDL_PERSONAL& = &H5
Public Declare Function SHGetSpecialFolderPath Lib "shell32.dll" Alias "SHGetSpecialFolderPathA" (ByVal hwnd As Long, ByVal pszPath As String, ByVal csidl As Long, ByVal fCreate As Long) As Long
Public Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpBuffer As String, nSize As Long) As Long



