Attribute VB_Name = "basHtmlHelp"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Commands to pass to HtmlHelp()
'
Public Enum HH_COMMANDS
    HH_DISPLAY_TOPIC = 0
    HH_DISPLAY_TOC = 1
    HH_DISPLAY_INDEX = 2
    HH_DISPLAY_SEARCH = 3
    HH_SET_WIN_TYPE = 4
    HH_GET_WIN_TYPE = 5
    HH_GET_WIN_HANDLE = 6
    HH_ENUM_INFO_TYPE = 7           ' Get Info type name, call repeatedly to enumerate, -1 at end
    HH_SET_INFO_TYPE = 8            ' Add Info type to filter.
    HH_SYNC = 9
    HH_RESERVED1 = &HA
    HH_RESERVED2 = &HB
    HH_RESERVED3 = &HC
    HH_KEYWORD_LOOKUP = &HD
    HH_DISPLAY_TEXT_POPUP = &HE     ' display string resource id or text in a popup window
    HH_HELP_CONTEXT = &HF           ' display mapped numeric value in dwData
    HH_TP_HELP_CONTEXTMENU = &H10   ' text popup help, same as WinHelp HELP_CONTEXTMENU
    HH_TP_HELP_WM_HELP = &H11       ' text popup help, same as WinHelp HELP_WM_HELP
    HH_CLOSE_ALL = &H12             ' close all windows opened directly or indirectly by the caller
    HH_ALINK_LOOKUP = &H13          ' ALink version of HH_KEYWORD_LOOKUP
    HH_GET_LAST_ERROR = &H14        ' not currently implemented // See HHERROR.h
    HH_ENUM_CATEGORY = &H15         ' Get category name, call repeatedly to enumerate, -1 at end
    HH_ENUM_CATEGORY_IT = &H16      ' Get category info type members, call repeatedly to enumerate, -1 at end
    HH_RESET_IT_FILTER = &H17       ' Clear the info type filter of all info types.
    HH_SET_INCLUSIVE_FILTER = &H18  ' set inclusive filtering method for untyped topics to be included in display
    HH_SET_EXCLUSIVE_FILTER = &H19  ' set exclusive filtering method for untyped topics to be excluded from display
    HH_INITIALIZE = &H1C            ' Initializes the help system.
    HH_UNINITIALIZE = &H1D          ' Uninitializes the help system.
    HH_PRETRANSLATEMESSAGE = &HFD   ' Pumps messages. (NULL, NULL, MSG*).
    HH_SET_GLOBAL_PROPERTY = &HFC   ' Set a global property. (NULL, NULL, HH_GPROP)
End Enum

'HWND HtmlHelp(
'              HWND    hwndCaller,
'              LPCSTR  pszFile,
'              UINT    uCommand,
'              DWORD   dwData) ;

Public Declare Function HtmlHelp Lib "hhctrl.ocx" Alias "HtmlHelpA" _
( _
    ByVal hwndCaller As Long, _
    ByVal pszFile As String, _
    ByVal uCommand As HH_COMMANDS, _
    ByVal dwData As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayHelpContents()
Dim FileName As String
    FileName = App.Path & "\" & FILE_CHM
    If Dir(FileName) <> "" Then
        HtmlHelp frmMain.hwnd, FileName, HH_DISPLAY_TOC, 0
    End If
End Sub

