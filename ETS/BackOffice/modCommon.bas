Attribute VB_Name = "modCommon"
Option Explicit

Public Enum eType
    TYPE_INDEX = 1
    TYPE_STOCK = 2
    TYPE_OPTION = 3
End Enum

Public Enum ContractTypes
    CONTRACT_TYPE_INDEX = 1
    CONTRACT_TYPE_STOCK = 2
    CONTRACT_TYPE_OPTION = 3
End Enum

Public Enum eOptionsStyle
    OPTIONS_EUROPIAN = 0
    OPTIONS_AMERICAN = 1
    OPTIONS_DEFAULT_STOCK = OPTIONS_AMERICAN
    OPTIONS_DEFAULT_INDEX = OPTIONS_EUROPIAN
End Enum

Public Enum eCalendarType
    CALENDAR_THIRD_SATURDAY = 1
    CALENDAR_THIRD_FRIDAY = 2
    CALENDAR_THIRD_THURSDAY = 3
    CALENDAR_DEFAULT_STOCK = CALENDAR_THIRD_SATURDAY
    CALENDAR_DEFAULT_INDEX = CALENDAR_THIRD_FRIDAY
End Enum

Public Enum CollectionElementState
    ELEM_UNCHANGED
    ELEM_INSERTED
    ELEM_UPDATED
    ELEM_DELETED
End Enum

Private Type IconType
    cbSize As Long
    picType As PictureTypeConstants
    hIcon As Long
    hPal As Long
    Reserved As Long
End Type


Private Const INVALID_TICKER_CHARS$ = "01234567890"
Public Const BAD_DOUBLE_VALUE As Double = -1E+308
Public Const BAD_LONG_VALUE As Long = -2147483647
Public Const DBL_EPSILON As Double = 2.22044604925031E-16
 
' Reg Key Security Options...
Public Const READ_CONTROL = &H20000
Public Const KEY_QUERY_VALUE = &H1
Public Const KEY_SET_VALUE = &H2
Public Const KEY_CREATE_SUB_KEY = &H4
Public Const KEY_ENUMERATE_SUB_KEYS = &H8
Public Const KEY_NOTIFY = &H10
Public Const KEY_CREATE_LINK = &H20
Public Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
Public Const KEY_READ = READ_CONTROL + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY
                     
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

' Reg Key ROOT Types...
Public Const HKEY_LOCAL_MACHINE = &H80000002
Public Const HKEY_CURRENT_USER = &H80000001
Public Const HKEY_CLASSES_ROOT As Long = &H80000000

Public Const ERROR_SUCCESS = 0
Public Const REG_SZ = 1                         ' Unicode nul terminated string
Public Const REG_DWORD = 4                      ' 32-bit number

Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Any, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByRef lpData As Any, ByRef lpcbData As Long) As Long
Declare Function RegSetValueExA Lib "advapi32.dll" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long                                ' Note that if you declare the lpData parameter as String, you must pass it By Value.
Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
Public Declare Function LockWindowUpdate Lib "user32" (ByVal hwndLock As Long) As Long
Public Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long
Public Declare Function FindWindow Lib "user32" Alias "FindWindowA" (lpClassName As Any, ByVal lpWindowName As String) As Long
Public Declare Function ReleaseCapture Lib "user32" () As Long
Public Declare Function DeleteObject Lib "gdi32" (ByVal hObject As Long) As Long
Private Declare Sub GetSystemTime Lib "kernel32" (lpSystemTime As SystemTime)


Type STARTUPINFO
    cb As Long
    lpReserved As String
    lpDesktop As String
    lpTitle As String
    dwX As Long
    dwY As Long
    dwXSize As Long
    dwYSize As Long
    dwXCountChars As Long
    dwYCountChars As Long
    dwFillAttribute As Long
    dwFlags As Long
    wShowWindow As Integer
    cbReserved2 As Integer
    lpReserved2 As Long
    hStdInput As Long
    hStdOutput As Long
    hStdError As Long
End Type

Type PROCESS_INFORMATION
    hProcess As Long
    hThread As Long
    dwProcessId As Long
    dwThreadId As Long
End Type


Public Const CREATE_ALWAYS As Long = 2
Public Const OPEN_EXISTING As Long = 3

Public Const GENERIC_WRITE As Long = &H40000000
Public Const GENERIC_READ As Long = &H80000000

Public Const FILE_SHARE_READ As Long = &H1
Public Const FILE_SHARE_WRITE As Long = &H2

Public Const FILE_ATTRIBUTE_NOT_CONTENT_INDEXED As Long = &H2000
Public Const FILE_ATTRIBUTE_TEMPORARY As Long = &H100
Public Const FILE_FLAG_DELETE_ON_CLOSE As Long = &H4000000
Public Const FILE_ATTRIBUTE_NORMAL As Long = &H80

Public Const INVALID_HANDLE_VALUE As Long = -1
Public Const CREATE_NO_WINDOW As Long = &H8000000

Declare Function CreateProcess Lib "kernel32" Alias "CreateProcessA" (ByVal lpApplicationName As String, ByVal lpCommandLine As String, lpProcessAttributes As SECURITY_ATTRIBUTES, lpThreadAttributes As Long, ByVal bInheritHandles As Boolean, ByVal dwCreationFlags As Long, lpEnvironment As Long, ByVal lpCurrentDirectory As String, lpStartupInfo As STARTUPINFO, lpProcessInformation As PROCESS_INFORMATION) As Long
Declare Function WaitForSingleObject Lib "kernel32" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" (ByVal lpFileName As String, ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, lpSecurityAttributes As SECURITY_ATTRIBUTES, ByVal dwCreationDisposition As Long, ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) As Long
Declare Function WriteFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToWrite As Long, lpNumberOfBytesWritten As Long, lpOverlapped As Long) As Boolean



Public Const HWND_TOP As Long = 0
Public Const HWND_TOPMOST As Long = -1
Public Const HWND_NOTOPMOST As Long = -2
Public Const SWP_NOMOVE As Long = &H2
Public Const SWP_NOSIZE As Long = &H1
Public Const SWP_NOACTIVATE As Long = &H10
Public Const SW_SHOWNOACTIVATE As Long = 4
Public Const SW_SHOWNA As Long = 4
Public Const SW_SHOWNORMAL As Long = 1
Public Const SW_SHOWMINNOACTIVE As Long = 7
Public Const SW_SHOWMAXIMIZED As Long = 3
Public Const SW_SHOWMINIMIZED As Long = 2
Public Const SW_MAXIMIZE As Long = 3
Public Const SW_MINIMIZE As Long = 6

Public Declare Function SetWindowPos Lib "user32" (ByVal hWnd As Long, ByVal hWndInsertAfter As Long, ByVal X As Long, ByVal y As Long, ByVal cx As Long, ByVal cy As Long, ByVal wFlags As Long) As Long

Public Const TIME_ZONE_ID_INVALID As Long = &HFFFFFFFF
Public Const TIME_ZONE_ID_UNKNOWN As Long = 0
Public Const TIME_ZONE_ID_STANDARD As Long = 1
Public Const TIME_ZONE_ID_DAYLIGHT As Long = 2

Public Const USD_ID As Long = -1
Public Const USD_KEY As String = "-1"
Public Const USD_SYMBOL As String = "$"

Public Const USA_REGION As String = "USD"
Public Const CAN_REGION As String = "CAD"

Public Type SECURITY_ATTRIBUTES
    nLength As Long
    lpSecurityDescriptor As Long
    bInheritHandle As Boolean
End Type

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

Public Const CSIDL_PERSONAL& = &H5
Public Declare Function SHGetSpecialFolderPath Lib "shell32.dll" Alias "SHGetSpecialFolderPathA" (ByVal hWnd As Long, ByVal pszPath As String, ByVal csidl As Long, ByVal fCreate As Long) As Long
Public Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpBuffer As String, nSize As Long) As Long

Public Enum EventTypeEnum
    EVENT_INFO = 1
    EVENT_WARNING = 2
    EVENT_ERROR = 3
End Enum

Public Const ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_TRIAL& = 1&
Public Const ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_WITH_PAYMENT& = 0&
Public Const ASP_SUBSCRIPTION_STATUS_EXPIRED& = -1
Public Const ASP_SUBSCRIPTION_STATUS_CANCELED_BY_USER& = -2
Public Const ASP_SUBSCRIPTION_STATUS_NOT_SUBSCRIBED& = -3
Public Const ASP_SUBSCRIPTION_STATUS_CANCELED_BY_ADMIN& = -4
Public Const ASP_SUBSCRIPTION_STATUS_SUSPENDED& = -5
Public Const ASP_SUBSCRIPTION_STATUS_CARD_INVALID& = -6&


Public Declare Function GetForegroundWindow Lib "user32" () As Long

Public Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
Public Declare Function GetComputerName Lib "kernel32.dll" Alias "GetComputerNameA" (Name As Byte, ByRef Size As Long) As Boolean
Public Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long


Public Const ERROR_CANCELLED As Long = &H800704C7

Public Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, Source As Any, ByVal numBytes As Long)

Private Type CLSIdType
    ID(16) As Byte
End Type
Public Type POINTAPI
    X As Long
    y As Long
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

Public Type PictDesc
    cbSizeofStruct As Long
    picType As Long
    hImage As Long
    xExt As Long
    yExt As Long
End Type

Public Type Guid
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
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


Declare Function GetUpdateRect Lib "user32" (ByVal hWnd As Long, lpRect As RECT, ByVal bErase As Long) As Long



Private Const SM_CXICON = 11
Private Const SM_CYICON = 12

Private Const SM_CXSMICON = 49
Private Const SM_CYSMICON = 50
   
Private Declare Function LoadImageAsString Lib "user32" Alias "LoadImageA" ( _
      ByVal hInst As Long, _
      ByVal lpsz As String, _
      ByVal uType As Long, _
      ByVal cxDesired As Long, _
      ByVal cyDesired As Long, _
      ByVal fuLoad As Long _
   ) As Long
   
Private Const LR_DEFAULTCOLOR = &H0
Private Const LR_MONOCHROME = &H1
Private Const LR_COLOR = &H2
Private Const LR_COPYRETURNORG = &H4
Private Const LR_COPYDELETEORG = &H8
Private Const LR_LOADFROMFILE = &H10
Private Const LR_LOADTRANSPARENT = &H20
Private Const LR_DEFAULTSIZE = &H40
Private Const LR_VGACOLOR = &H80
Private Const LR_LOADMAP3DCOLORS = &H1000
Private Const LR_CREATEDIBSECTION = &H2000
Private Const LR_COPYFROMRESOURCE = &H4000
Private Const LR_SHARED = &H8000&


Private Declare Function SendMessageLong Lib "user32" Alias "SendMessageA" ( _
      ByVal hWnd As Long, ByVal wMsg As Long, _
      ByVal wParam As Long, ByVal lParam As Long _
   ) As Long

Private Const WM_SETICON = &H80

Private Const ICON_SMALL = 0
Private Const ICON_BIG = 1

Private Declare Function GetWindow Lib "user32" ( _
   ByVal hWnd As Long, ByVal wCmd As Long) As Long
Private Const GW_OWNER = 4

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

Public Declare Function GetWindowPlacement Lib "user32" (ByVal hWnd As Long, lpwndpl As WINDOWPLACEMENT) As Long
Public Declare Function SetWindowPlacement Lib "user32" (ByVal hWnd As Long, lpwndpl As WINDOWPLACEMENT) As Long

Public Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long) As Long
Public Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
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

Public Declare Function GetMenu Lib "user32" (ByVal hWnd As Long) As Long
Public Declare Function GetMenuBarInfo Lib "user32" (ByVal hWnd As Long, ByVal idObject As Long, ByVal idItem As Long, ByRef pmbi As MENUBARINFO) As Long
Public Declare Function GetLastError Lib "kernel32" () As Long
Public Declare Function GetMenuItemCount Lib "user32" (ByVal hMenu As Long) As Long

Public Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long
Public Declare Function GetVolumeInformation Lib "kernel32" Alias "GetVolumeInformationA" (ByVal lpRootPathName As String, lpVolumeNameBuffer As Any, ByVal nVolumeNameSize As Long, lpVolumeSerialNumber As Long, lpMaximumComponentLength As Any, lpFileSystemFlags As Any, lpFileSystemNameBuffer As Any, ByVal nFileSystemNameSize As Long) As Long

Public Const EM_SETTABSTOPS As Long = &HCB
Public Declare Function SendMessage Lib "user32" Alias "SendMessageA" (ByVal hWnd As Long, ByVal wMsg As Long, ByVal wParam As Long, lParam As Any) As Long

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

'Public Type Guid
'    Data1 As Long
'    Data2 As Integer
'    Data3 As Integer
'    Data4(0 To 7) As Byte
'End Type

Public Declare Function CoCreateGuid Lib "ole32" (ByRef pguid As Guid) As Long
Public Declare Function CLSIDFromProgID Lib "ole32.dll" (ByVal pOLESTR As Long, ByRef pCLSID As Guid) As Long
Public Declare Function FlashWindow Lib "user32" (ByVal hWnd As Long, ByVal bInvert As Long) As Long
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
    y As Long
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

Public Declare Function CreateWindowEx Lib "user32" Alias "CreateWindowExA" (ByVal dwExStyle As Long, ByVal lpClassName As String, ByVal lpWindowName As String, ByVal dwStyle As Long, ByVal X As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal hWndParent As Long, ByVal hMenu As Long, ByVal hInstance As Long, lpParam As Any) As Long
Public Declare Function DestroyWindow Lib "user32" (ByVal hWnd As Long) As Long
Public Declare Function MoveWindow Lib "user32" (ByVal hWnd As Long, ByVal X As Long, ByVal y As Long, ByVal nWidth As Long, ByVal nHeight As Long, ByVal bRepaint As Long) As Long

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

' iType options:
Public Const IMAGE_BITMAP = 0
Public Const IMAGE_ICON = 1
Public Const IMAGE_CURSOR = 2


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

Public Const SND_APPLICATION = &H80
Public Const SND_ALIAS = &H10000
Public Const SND_ALIAS_ID = &H110000
Public Const SND_ASYNC = &H1
Public Const SND_FILENAME = &H20000
Public Const SND_LOOP = &H8
Public Const SND_MEMORY = &H4
Public Const SND_NODEFAULT = &H2
Public Const SND_NOSTOP = &H10
Public Const SND_NOWAIT = &H2000
Public Const SND_PURGE = &H40
Public Const SND_RESOURCE = &H40004
Public Const SND_SYNC = &H0


Private Declare Function OleCreatePictureIndirect Lib "oleaut32.dll" _
        (pDicDesc As IconType, riid As CLSIdType, ByVal fown As Long, _
        lpUnk As Object) As Long
Private Declare Function OleCreatePictureIndirect2 Lib "oleaut32.dll" _
        (pDicDesc As IconType, riid As Guid, ByVal fown As Long, _
        lpUnk As Object) As Long


Public Declare Function LoadBitmap Lib "user32" Alias "LoadBitmapA" (ByVal hInstance As Long, _
        ByVal BitmapID As String) As Long

Public Declare Function LoadCursor Lib "user32" Alias "LoadCursorA" (ByVal hLib As Long, _
        ByVal lngCursorID As Long) As Long

Public Declare Function LoadIcon Lib "user32" Alias "LoadIconA" (ByVal hLib As Long, _
        ByVal lngIconID As String) As Long

Public Declare Function LoadString Lib "user32" Alias "LoadStringA" (ByVal hLib As Long, _
        ByVal ResourceID As Long, ByVal lpBuffer As String, ByVal nBufferSize As Long) As Long

Public Function ClipDays(ByVal dtDate As Date) As Date
    On Error Resume Next
    ClipDays = TimeSerial(Hour(dtDate), Minute(dtDate), 0)
End Function

Public Sub LogEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    ShowEventLog True
    g_frmLog.LogEvent enType, sMessage
End Sub

Public Sub ShowEventLog(ByVal bShow As Boolean)
    On Error Resume Next
    If g_frmLog Is Nothing Then
        Set g_frmLog = New frmLog
        Load g_frmLog
        If g_lLogWidth > 0 And g_lLogHeight > 0 Then
            If g_lLogLeft < 0 Then g_lLogLeft = 0
            If g_lLogTop < 0 Then g_lLogTop = 0
            If g_lLogLeft > Screen.Width - 300 Then g_lLogLeft = Screen.Width - 300
            If g_lLogTop > Screen.Height - 300 Then g_lLogTop = Screen.Height - 300
            g_frmLog.Left = g_lLogLeft
            g_frmLog.Top = g_lLogTop
            g_frmLog.Width = g_lLogWidth
            g_frmLog.Height = g_lLogHeight
        Else
            g_frmLog.Left = (Screen.Width - g_frmLog.Width) / 2
            g_frmLog.Top = (Screen.Height - g_frmLog.Height) / 2
        End If
    End If
    If bShow Then
        g_frmLog.Show vbModeless, frmMain
        frmMain.mnuOptionsEventLog.Checked = True
    Else
        g_frmLog.Hide
        frmMain.mnuOptionsEventLog.Checked = False
    End If
End Sub

Public Function MakeAppComponentsDescription(Optional ByVal bAddAppVersion As Boolean = False, Optional ByVal bReloadDBVersion As Boolean = True) As String
    On Error Resume Next
    Dim cVI As clsVersionInfo, sVersion$, sFile$, sPath$, sGUID$
    MakeAppComponentsDescription = " Components :" & vbTab & "Version :" & vbTab & "Path :" & vbCrLf

    
    If bAddAppVersion Then
        MakeAppComponentsDescription = MakeAppComponentsDescription & " " & AppTitle() & vbTab & App.Major & "." & App.Minor & "." & App.Revision & vbCrLf
    End If
    
    If bReloadDBVersion Then
        LoadDBVersion
    End If
    
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & "Database" & _
                                    vbTab & IIf(g_Params.DBVersion <> "", g_Params.DBVersion, "N/A") & _
                                    vbTab & IIf(g_Params.DBPath <> "", g_Params.DBPath, "N/A") & vbCrLf
    
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & "Login" & _
                                    vbTab & CurrentNtUserName() & vbCrLf
                                    
    Set cVI = New clsVersionInfo
    
'    sPath = App.Path
'    If Right$(sPath, 1) <> "\" And Right$(sPath, 1) <> "/" Then
'        sPath = sPath & "\"
'    End If
    
    sFile = "ElladaFlatControls.ocx"
    sGUID = "{DD579E5D-E367-4D5C-858B-9B34BEC79C8D}" ' FlatButton
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "OptionCalc.dll"
    If cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf

    sFile = "MMW.dll"
    sGUID = "{D9614A17-5E4D-47D9-8EDB-8A6209CF7A1A}" ' MsgManager
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "MsgStruct.dll"
    sGUID = "{5B7B0165-1126-4C59-B85C-C0C6B4E8642F}" ' SymbolObject
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "Transport.exe"
    sGUID = "{3372D3D9-953E-4A68-B2DC-34DD4AC2C9D4}" ' Router
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "VSDBLayout.dll"
    sGUID = "{A7166386-06A9-4D6A-8276-3A7CF0E0235E}" ' DBLayout
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "VME.exe"
    sGUID = "{079EBA0B-CAC8-4210-BD39-36F52BC5C01D}" ' VolatilitySource
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sFile = "PriceProviders.dll"
    sGUID = "{FA41723C-C387-4448-9245-FC2875C2EE3C}" ' PriceInfo
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sPath & sFile) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    
    sGUID = ""
    Select Case g_Params.PriceProviderType
        Case 2 ' enTrackData
            sFile = "TrackDataProviders.exe"
            sGUID = "{6B0F8362-4C54-4602-99A1-246089E287C8}" ' TrackDataPriceInfo
            
        Case 0 'enMyTrack
            sFile = "MyTrackProviders.exe"
            sGUID = "{266203B3-D1DC-4808-A5C7-265F7C312629}" ' MyTrackPriceInfo
            
        Case 1 'enHyperFeed
            sFile = "HyperFeedDataProviders.exe"
            sGUID = "{15F78302-5620-4AAE-822D-99C15DCFBFB8}" ' HyperFeedPriceInfo
            
        Case 3 ' enATFinancial
            sFile = "ATFDataProvider.exe"
            sGUID = "{9BEA9053-D943-45B6-8D0D-2700617CED39}" ' TrackDataPriceInfo
            
    End Select
    
    If Len(sGUID) <> 0 Then
        If cVI.SetObjectGUID(sGUID) Then
            sFile = cVI.FileName
            sVersion = cVI.FixedFileVersion
        ElseIf cVI.SetFilePath(sPath & sFile) Then
            sVersion = cVI.FixedFileVersion
        ElseIf cVI.SetFilePath(sFile) Then
            sVersion = cVI.FixedFileVersion
        Else
            sVersion = ""
        End If
        MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf
    End If
    Set cVI = Nothing
End Function

' get string value from HKEY_LOCAL_MACHINE
Public Function GetStrKeyValueHKLM(strKeyName As String, strValueName As String, Optional ByVal strDefault As String = "") As String
    On Error Resume Next
    Dim strValue$
    
    GetStrKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue, strDefault
        
    GetStrKeyValueHKLM = strValue
End Function

' get string value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
Public Function GetStrKeyValueEx(strKeyName As String, strValueName As String, Optional ByVal strDefault As String = "") As String
    Dim strValue$
    
    If Not GetStrKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, strValue, strDefault) Then _
        GetStrKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue, strDefault
        
    GetStrKeyValueEx = strValue
End Function

' get long value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
Public Function GetLongKeyValueEx(strKeyName As String, strValueName As String, Optional ByVal lngDefault As Long = 0) As Long
    Dim nValue&
    
    If Not GetLongKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, nValue, lngDefault) Then _
        GetLongKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, nValue, lngDefault
        
    GetLongKeyValueEx = nValue
End Function

' get string value from registry
Public Function GetStrKeyValue(lngKeyRoot As Long, strKeyName As String, strValueName As String, strValue As String, Optional ByVal strDefault As String = "") As Boolean
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    Dim KeyValType As Long                                  ' Data Type Of A Registry Key
    Dim tmpVal As String                                    ' Tempory Storage For A Registry Key Value
    Dim KeyValSize As Long                                  ' Size Of Registry Key Variable
    '------------------------------------------------------------
    ' Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey) ' Open Registry Key
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    
    tmpVal = String$(16384, 0)                              ' Allocate Variable Space
    KeyValSize = 16384                                      ' Mark Variable Size
    
    '------------------------------------------------------------
    ' Retrieve Registry Key Value...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, strValueName, 0, _
                         KeyValType, ByVal tmpVal, KeyValSize)    ' Get/Create Key Value
                        
    If rc <> ERROR_SUCCESS Or KeyValType <> REG_SZ Then GoTo GetKeyError          ' Handle Errors
    
    If KeyValSize <= 0 Then
        tmpVal = ""
    ElseIf (Asc(Mid$(tmpVal, KeyValSize, 1)) = 0) Then           ' Win95 Adds Null Terminated String...
        tmpVal = Left$(tmpVal, KeyValSize - 1)               ' Null Found, Extract From String
    Else                                                    ' WinNT Does NOT Null Terminate String...
        tmpVal = Left$(tmpVal, KeyValSize)                   ' Null Not Found, Extract String Only
    End If
    
    strValue = tmpVal                                       ' Return Success
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
    GetStrKeyValue = True
    Exit Function                                           ' Exit
    
GetKeyError:      ' Cleanup After An Error Has Occured...
    GetStrKeyValue = False
    strValue = strDefault                                   ' Set Return Val To Default String
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Function

' get long value from registry
Public Function GetLongKeyValue(lngKeyRoot As Long, strKeyName As String, strValueName As String, lngValue As Long, Optional ByVal lngDefault As Long = 0) As Boolean
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    Dim KeyValType As Long                                  ' Data Type Of A Registry Key
    Dim tmpVal As Long                                      ' Tempory Storage For A Registry Key Value
    Dim KeyValSize As Long                                  ' Size Of Registry Key Variable
    '------------------------------------------------------------
    ' Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey) ' Open Registry Key
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    
    KeyValSize = 4                                          ' Mark Variable Size
    
    '------------------------------------------------------------
    ' Retrieve Registry Key Value...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, strValueName, 0, _
                         KeyValType, tmpVal, KeyValSize)    ' Get/Create Key Value
                        
    If rc <> ERROR_SUCCESS Or KeyValType <> REG_DWORD Then GoTo GetKeyError            ' Handle Errors
    
    lngValue = tmpVal                                ' Return Value
    GetLongKeyValue = True
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
    Exit Function                                           ' Exit
    
GetKeyError:      ' Cleanup After An Error Has Occured...
    GetLongKeyValue = False
    lngValue = lngDefault                            ' Set Return Val To Default value
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Function


' write string value from registry
Public Sub SetStrKeyValue(ByVal lngKeyRoot As Long, ByVal strKeyName As String, ByVal strValueName As String, ByVal strValue As String)
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    '------------------------------------------------------------
    ' Create or Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    '------------------------------------------------------------
    ' Set Registry Key Value...
    '------------------------------------------------------------
    If Len(strValue) > 0 Then
        'rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal strValue, CLng(Len(strValue) + 1))
        rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal strValue, CLng(Len(strValue)))
    Else
        rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal 0, 0)
    End If
                        
GetKeyError:      ' Cleanup After An Error Has Occured...
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Sub

' write string value from registry
Public Sub SetLongKeyValue(ByVal lngKeyRoot As Long, ByVal strKeyName As String, ByVal strValueName As String, ByVal lngValue As Long)
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    '------------------------------------------------------------
    ' Create or Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    '------------------------------------------------------------
    ' Set Registry Key Value...
    '------------------------------------------------------------
    rc = RegSetValueExA(hKey, strValueName, 0, REG_DWORD, lngValue, 4)
                        
GetKeyError:      ' Cleanup After An Error Has Occured...
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Sub


Public Function ContractTypeName(ByVal TypeID) As String
    If TypeID = 3 Then
        ContractTypeName = "Option"
    ElseIf TypeID = 2 Then
        ContractTypeName = "Stock"
    Else
        ContractTypeName = "Index"
    End If
End Function

Public Sub CopyGridToClipboard(grd As VSFlexGrid)
Dim Buffer As String
    With Clipboard
        .Clear
        If SaveGridToText(grd, Buffer) Then
            .SetText Buffer
        End If
    End With
End Sub

Public Function SaveGridToText(grd As VSFlexGrid, Buffer As String) As Boolean
Dim i As Long
Dim j As Long
Dim k As Long
Dim s As String
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
    
    Buffer = ""
    With grd
        
        If .Rows > 1 And .Cols > 0 Then
            .GetSelection r1, c1, r2, c2
            If (r2 - r1 + 1) * (c2 - c1 + 1) > 1 Then
                Debug.Print "Selection"
            Else
                r1 = 1
                r2 = .Rows - 1
                c1 = 0
                c2 = .Cols - 1
            End If
            For i = r1 To r2
                If Not .RowHidden(i) Then
                    s = ""
                    k = 0
                    For j = c1 To c2
                        If Not .ColHidden(j) Then
                            If k > 0 Then
                                s = s & vbTab
                            End If
                            k = k + 1
                            If .Cell(flexcpChecked, i, j) = flexNoCheckbox Then
                                s = s & .TextMatrix(i, j)
                            Else
                                If .Cell(flexcpChecked, i, j) = flexChecked Then
                                    s = s & "1"
                                ElseIf .Cell(flexcpChecked, i, j) = flexUnchecked Then
                                    s = s & "0"
                                Else
                                    s = s & ""
                                End If
                            End If
                        End If
                    Next
                    If Len(s) > 0 Then
                        Buffer = Buffer & s & vbCrLf
                    End If
                End If
            Next
        End If
    End With
    SaveGridToText = Len(Buffer) > 0
End Function

Public Function LoadDBVersion(Optional ByVal bCheckVersion As Boolean = False) As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    
    LoadDBVersion = False
 
    Set rs = gDBW.usp_DataInfo_Get(, "version")
    If Not rs Is Nothing Then
        If Not rs.EOF Then
            g_Params.DBVersion = gCmn.ReadStr(rs!vcKeyValue)
            
            If bCheckVersion Then
                LoadDBVersion = CheckDBVersion
            Else
                LoadDBVersion = True
            End If
        End If
        Set rs = Nothing
    End If
    
    Set rs = Nothing
    
    On Error Resume Next
    g_Params.DBPath = gDBW.GetDbPath
    Exit Function
EH:
    Set rs = Nothing
    gCmn.ErrorMsgBox "Fail to get DB version."
End Function

Private Function CheckDBVersion() As Boolean
    On Error Resume Next
    Dim sVersion() As String
    Dim nLBound&: nLBound = -1
    Dim nUBound&: nUBound = -1
    Dim nMajor&, nMinor&, nBuild&
    
    CheckDBVersion = False

    If Not g_Params.DoNotCheckDbVersion And DB_VERSION_COMPATIBLE_MAJOR > 0 And DB_VERSION_COMPATIBLE_MINOR > 0 Then
        sVersion = Split(g_Params.DBVersion, ".")
        nLBound = LBound(sVersion)
        nUBound = UBound(sVersion)
        If nUBound >= 0 Then
            If nUBound = 2 Then
                nMajor = CLng(sVersion(0))
                nMinor = CLng(sVersion(1))
                nBuild = CLng(sVersion(2))
            End If
                
            If nMajor > 0 And nMinor > 0 _
                And (DB_VERSION_COMPATIBLE_BUILD = 0 Or DB_VERSION_COMPATIBLE_BUILD > 0 And nBuild > 0) Then
                
                If DB_VERSION_COMPATIBLE_MAJOR = nMajor _
                    And DB_VERSION_COMPATIBLE_MINOR = nMinor _
                    And (DB_VERSION_COMPATIBLE_BUILD = 0 _
                        Or DB_VERSION_COMPATIBLE_BUILD > 0 And DB_VERSION_COMPATIBLE_BUILD <= nBuild) Then
                    
                    CheckDBVersion = True
                Else
                    gCmn.MyMsgBox "Fail to check database version." & vbCrLf & _
                                            "Incompatible version '" & g_Params.DBVersion & "'. Expected '" & _
                                            Trim$(Str$(DB_VERSION_COMPATIBLE_MAJOR)) & "." & _
                                            Trim$(Str$(DB_VERSION_COMPATIBLE_MINOR)) & "." & _
                                            IIf(DB_VERSION_COMPATIBLE_BUILD > 0, _
                                                Trim$(Str$(DB_VERSION_COMPATIBLE_BUILD)), "x") & "' at least." & vbCrLf & _
                                            "Please check your database connection.", vbCritical
                End If
            Else
                gCmn.MyMsgBox "Fail to check database version." & vbCrLf & _
                                        "Invalid version number format '" & g_Params.DBVersion & "'." & vbCrLf & _
                                        "Please check your database connection.", vbCritical
            End If
        Else
            gCmn.MyMsgBox "Fail to check database version. Empty version number." & vbCrLf & _
                                    "Please check your database connection.", vbCritical
        End If
    Else
        CheckDBVersion = True
    End If
End Function


Public Function FixDbString(ByVal sString As String) As String
    On Error Resume Next
    FixDbString = Replace(sString, "'", "''")
End Function

Public Function CalendarExpiry(ByVal Expiry As Date, Optional ByVal CalendarType As eCalendarType = CALENDAR_THIRD_SATURDAY) As Date
Dim y As Integer
Dim m As Integer
Dim d1 As Date
Dim D As Integer
    y = Year(Expiry)
    m = Month(Expiry)
    Select Case CalendarType
    Case CALENDAR_THIRD_SATURDAY '1
        D = 22
    Case CALENDAR_THIRD_FRIDAY   '2
        D = 21
    Case CALENDAR_THIRD_THURSDAY '3
        D = 20
    Case Else
        D = 22
    End Select
    d1 = DateSerial(y, m, 1)
    CalendarExpiry = d1 + D - Weekday(d1, vbSaturday)
End Function


Public Function DivFreqName(ByVal DivFreqID As Long) As String
    Select Case DivFreqID
    Case 0
        DivFreqName = "<NONE>"
    Case 1
        DivFreqName = "Annually"
    Case 2
        DivFreqName = "Semiannually"
    Case 4
        DivFreqName = "Quarterly"
    Case 12
        DivFreqName = "Monthly"
    Case Else
        DivFreqName = ""
        Debug.Assert False
    End Select
End Function

Public Function GmtToLocal(ByVal dtGmtTime As Date) As Date
    On Error Resume Next
    GmtToLocal = DateAdd("n", -g_Params.TimeZoneBias, dtGmtTime)
End Function

Public Function LocalToGmt(ByVal dtLocalTime As Date) As Date
    On Error Resume Next
    LocalToGmt = DateAdd("n", g_Params.TimeZoneBias, dtLocalTime)
End Function

Public Function GetNextBussinessDay(ByVal dtDate As Date, ByVal nDays As Long) As Date
    Dim dtTemp As Date, i&, sKey$, nWeekDay&
    On Error Resume Next
    i = 0
    dtTemp = dtDate
    Do
        dtTemp = DateAdd("d", 1, dtTemp)
        nWeekDay = DatePart("w", dtTemp, vbMonday)
        If nWeekDay < 6 Then ' from monday till friday
            sKey = Format$(dtTemp, "yyyymmdd")
            If g_collHolidays(sKey) Is Nothing Then i = i + 1
        End If
    Loop While i < nDays
    GetNextBussinessDay = dtTemp
End Function

Public Function IsValidTicker(ByVal sTicker$) As Boolean
    On Error Resume Next
    Dim i&, sChar$
    IsValidTicker = True
    
    For i = 1 To Len(sTicker)
        If InStr(1, INVALID_TICKER_CHARS, Mid$(sTicker, i, 1)) Then
            IsValidTicker = False
            Exit Function
        End If
    Next
End Function

Public Function MakeCommonFilePath$(ByVal sSubPath$, _
                                    ByRef sFileName$, _
                                    ByVal bPrivatePathCheckWriteAccess As Boolean, _
                                    Optional ByVal for_logs As Boolean = False)
    On Error Resume Next
    Dim sPath$, nFile#, bUseDefault As Boolean
    Dim log_directory As String
    
    bUseDefault = True
    
    sSubPath = "\" & IIf(Len(sSubPath) > 0, sSubPath & "\", "")
    
    If Len(g_Params.CurrentUserPrivateFolder) > 0 Then
        
        If (for_logs And Len(g_Params.LogDirectory) > 0) Then
            CheckAndCreateFolder g_Params.LogDirectory & sSubPath
            sPath = g_Params.LogDirectory & sSubPath & sFileName
        Else
            CheckAndCreateFolder g_Params.CurrentUserPrivateFolder & sSubPath
            sPath = g_Params.CurrentUserPrivateFolder & sSubPath & sFileName
        End If
        
        If Len(sFileName) > 0 Then
            ' check current user file availability
            Err.Clear
            nFile = FreeFile
            If bPrivatePathCheckWriteAccess Then
                Open sPath For Output Access Write Lock Read Write As #nFile
            Else
                Open sPath For Input Access Read Lock Write As #nFile
            End If
            
            bUseDefault = (Err.Number <> 0)
            
            Close #nFile
        Else
            bUseDefault = False
        End If
    End If
    
    If bUseDefault Then
        CheckAndCreateFolder App.Path & sSubPath
        sPath = App.Path & sSubPath & sFileName
    End If
    
    MakeCommonFilePath = sPath
End Function

Public Function CheckAndCreateFolder(ByRef sFolderPath$) As Boolean
    On Error Resume Next
    Dim aFSO As Scripting.FileSystemObject
    
    CheckAndCreateFolder = False
    Set aFSO = New Scripting.FileSystemObject
    If Not aFSO Is Nothing Then
        If Not aFSO.FolderExists(sFolderPath) Then
            Err.Clear
            aFSO.CreateFolder sFolderPath
            CheckAndCreateFolder = (Err.Number = 0)
        Else
            CheckAndCreateFolder = True
        End If
        Set aFSO = Nothing
    End If
End Function

Public Function CurrentNtUserName$()
    On Error Resume Next
    Dim sUserName$, nSize&
    sUserName = String$(512, 0)
    nSize = 512
    
    If GetUserName(sUserName, nSize) <> 0 Then
        CurrentNtUserName = Left$(sUserName, nSize - 1)
    Else
        CurrentNtUserName = ""
    End If
End Function

Public Function MainXMLFilePath$()
    On Error Resume Next
    MainXMLFilePath = GetStrKeyValueEx(PRODUCT_REG_KEY & "Asp" & "\Settings", "PathToMainXML", "")
End Function

Public Function CurrentUserXMLFilePath$()
    On Error Resume Next
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    Dim aXMLParams As ETSXMLParamsLib.XMLParams
    Set aXMLParams = aXMLLoader.Load(MainXMLFilePath)
    
    CurrentUserXMLFilePath = aXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp" & "\Users\" & CurrentNtUserName, "PathToXML", "")
End Function

Public Function LoadBitmapFromResource(hBmp As Long) As Picture
    Dim result As Long
    Dim Icon As IconType
    Dim IID_IDispatch As Guid
    Dim IPic As IPicture
    Dim CLSID As CLSIdType
    On Error Resume Next
     
    If hBmp <> 0 Then
        CLSID.ID(8) = &HC0
        CLSID.ID(15) = &H46
          
      Icon.cbSize = Len(Icon)
      Icon.picType = vbPicTypeBitmap
      Icon.hIcon = hBmp
      Icon.hPal = 0
      
      result = OleCreatePictureIndirect(Icon, CLSID, 1, IPic)
      If result = 0 Then
          Set LoadBitmapFromResource = IPic
              Set IPic = Nothing
          Else
              Call DeleteObject(hBmp)
          End If
      End If
    
    
End Function
Public Function LoadIconFromResource(hRes As Long) As IPictureDisp
    Dim result As Long
    Dim Unknown As IUnknown
    Dim Icon As IconType
    Dim CLSID As CLSIdType
    
    Icon.cbSize = Len(Icon)
    Icon.picType = vbPicTypeIcon
    Icon.hIcon = hRes
    CLSID.ID(8) = &HC0
    CLSID.ID(15) = &H46
    result = OleCreatePictureIndirect(Icon, CLSID, 1, Unknown)
    
    Set LoadIconFromResource = Unknown
End Function

' Return current time as UTC
Public Function SystemTime() As Date
    Dim t As SystemTime
    
    GetSystemTime t
    SystemTime = DateSerial(t.wYear, t.wMonth, t.wDay) + TimeSerial(t.wHour, t.wMinute, t.wSecond) + t.wMilliseconds / 86400000#
End Function

Public Sub Set_Icon( _
      ByVal hWnd As Long, _
      ByVal sIconResName As String, _
      ByVal hLibrary As Long, _
      Optional ByVal bSetAsAppIcon As Boolean = True _
   )
Dim lhWndTop As Long
Dim lhWnd As Long
Dim cx As Long
Dim cy As Long
Dim hIconLarge As Long
Dim hIconSmall As Long
      
   If (bSetAsAppIcon) Then
      ' Find VB's hidden parent window:
      lhWnd = hWnd
      lhWndTop = lhWnd
      Do While Not (lhWnd = 0)
         lhWnd = GetWindow(lhWnd, GW_OWNER)
         If Not (lhWnd = 0) Then
            lhWndTop = lhWnd
         End If
      Loop
   End If
   
   cx = GetSystemMetrics(SM_CXICON)
   cy = GetSystemMetrics(SM_CYICON)
   hIconLarge = LoadImageAsString( _
         hLibrary, sIconResName, _
         IMAGE_ICON, _
         cx, cy, _
         LR_SHARED)
   If (bSetAsAppIcon) Then
      SendMessageLong lhWndTop, WM_SETICON, ICON_BIG, hIconLarge
   End If
   SendMessageLong hWnd, WM_SETICON, ICON_BIG, hIconLarge
   
   cx = GetSystemMetrics(SM_CXSMICON)
   cy = GetSystemMetrics(SM_CYSMICON)
   hIconSmall = LoadImageAsString( _
         hLibrary, sIconResName, _
         IMAGE_ICON, _
         cx, cy, _
         LR_SHARED)
   If (bSetAsAppIcon) Then
      SendMessageLong lhWndTop, WM_SETICON, ICON_SMALL, hIconSmall
   End If
   SendMessageLong hWnd, WM_SETICON, ICON_SMALL, hIconSmall
End Sub


