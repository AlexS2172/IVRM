Attribute VB_Name = "basMain"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Public gRuntime As Boolean
Public gDebugtime As Boolean
Public gDBW As clsDataSource
Public gCmn As clsCommon
Public gPortfolioHedge As PortfolioHedger

Public g_Providers As PRICEPROVIDERSLib.Providers
Public g_PriceProvider As PRICEPROVIDERSLib.PriceInfo
Public g_StructureProvider As PRICEPROVIDERSLib.StructureInfo
Public g_ReportPricesFeed As Boolean

Public Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Declare Function GetModuleFileName Lib "kernel32" Alias "GetModuleFileNameA" (ByVal hModule As Long, ByVal lpFileName As String, ByVal nSize As Long) As Long

Public Enum EventTypeEnum
    EVENT_INFO = 1
    EVENT_WARNING = 2
    EVENT_ERROR = 3
End Enum

Global g_Params As New clsParams

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Main()
    On Error GoTo Herr
    gRuntime = True
    gDebugtime = False
    If InStr(Command, "debugfile") > 0 Then
        gDebugtime = True
    End If
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ' Avoid 'Switch to' message
    '
    App.OleRequestPendingTimeout = 2000000000 ' in msec
    App.OleServerBusyTimeout = 2000000000 ' in msec
    
    Set gCmn = New clsCommon
    
    Load frmSplash
    frmSplash.Show
    frmSplash.Refresh

#If LICENSE Then
    frmSplash.SetStatus "Checking license ..."
    
    Dim bNewLicense As Boolean
    bNewLicense = False
    If Not CheckLicense(frmSplash, bNewLicense) Then
        Shutdown
        Exit Sub
    End If
#End If

    frmSplash.SetStatus "Loading ..."
    LoadSettings
    g_Params.LoadSettings

    Set gPortfolioHedge = New PortfolioHedger

    frmSplash.SetStatus "Connecting to database ..."
    
    If Not OpenDataBase(frmSplash.hWnd) Then
        gCmn.MyMsgBox "Fail to connect to database. Application will be closed.", vbCritical
        Shutdown
        Exit Sub
    End If
    
    GetDBVersion
    
    ' enNoPriceProvider
    If g_ReportPricesFeed Then
        frmSplash.SetStatus "Connecting to real-time feed..."
        On Error GoTo FailedPriceProvider
        On Error Resume Next
        
        Set g_Providers = New PRICEPROVIDERSLib.Providers
        g_Providers.Initialize
        
        Set g_PriceProvider = New PRICEPROVIDERSLib.PriceInfo
        Dim Provider1 As PRICEPROVIDERSLib.IProvider
        Set Provider1 = g_PriceProvider
        If Not Provider1 Is Nothing Then
            Provider1.type = g_Params.PriceProviderType
        End If
        
    '    Set g_StructureProvider = New PRICEPROVIDERSLib.StructureInfo
    '    Dim Provider2 As PRICEPROVIDERSLib.IProvider
    '    Set Provider2 = g_StructureProvider
    '    If Not Provider2 Is Nothing Then
    '        Provider2.type = g_PriceProviderType
    '    End If
        
        g_PriceProvider.Connect
    '    g_StructureProvider.Connect
    
        On Error Resume Next
        gPortfolioHedge.fNeedReconnectMT = False
        gPortfolioHedge.ConnectMT
    
    End If
    On Error GoTo Herr
    Load frmMain
    
    If g_lMainWidth > 0 And g_lMainHeight > 0 Then
        frmMain.Left = g_lMainLeft
        frmMain.Top = g_lMainTop
        frmMain.Width = g_lMainWidth
        frmMain.Height = g_lMainHeight
    End If
    
    InitVolaSources
    
    frmMain.WindowState = g_lMainState
    frmMain.Show
    
    Dim sParameters() As String, sCommand$
    
    sCommand = Trim(Command)
    While InStr(1, sCommand, "  ") <> 0
        sCommand = Replace(sCommand, "  ", " ")
    Wend
    
    sParameters = Split(sCommand, , 1)
    If UBound(sParameters) >= 0 Then
        sParameters(0) = Trim(sParameters(0))
        If sParameters(0) <> "" Then
            frmSplash.SetStatus "Opening from file ..."
            frmMain.InitializePortfolioFromFile (sParameters(0))
            frmMain.AfterLoadPortfolio
        End If
    End If
    
    Unload frmSplash
    
    Exit Sub
Herr:
    If Err.Number <> 32755 Then
        ShowError "Failed to start Egar Hedger."
        Shutdown
    End If
    Exit Sub
FailedPriceProvider:
    ShowError "Failed to initialize price provider."
    Shutdown
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Shutdown()
    On Error Resume Next
    gRuntime = False
    g_Params.SaveSettings
    g_VolaSource.EnableEvents = False
    Set g_VolaSource = Nothing
    SaveSettings
'    frmMain.Hide
    Dim f As Form
    For Each f In Forms
        If Not f Is frmMain Then
            Unload f
            Set f = Nothing
        End If
    Next
    
    Set gCmn = Nothing
    Set gDBW = Nothing
'    Set gPortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Restart()
    Shutdown
    Main
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    gPortfolioHedge.LogSave sMessage
    If Screen.ActiveForm.Name = "frmMain" Or Screen.ActiveForm.MDIChild = True Then
        frmDataLog.ShowLog
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogSave(ByVal Message As String)
Dim f As Long
    On Error Resume Next
    f = FreeFile
    Open LogFile For Append Access Write As #f
    Print #f, Message
    Close #f
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogClear()
    On Error Resume Next
    If Len(Dir(LogFile)) > 0 Then
        Kill LogFile
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogFile() As String
    On Error Resume Next
    LogFile = App.Path & "\Data\Feed.log"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowStocWindowStatic(ByVal StockSymbol As String)
    On Error GoTo Herr
    With frmStockInfo
        .Execute StockSymbol
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub HideStocWindowStatic()
    On Error Resume Next
    With frmStockInfo
        .CloseMe
    End With
End Sub

#If DEBUG_REPLY Then
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DebugFile() As String
    On Error Resume Next
    DebugFile = App.Path & "\Data\Debug.log"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DebugClear()
    On Error Resume Next
    If Len(Dir(DebugFile)) > 0 Then
        Kill DebugFile
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DebugSave(ByVal Message As String)
Dim f As Long
    On Error Resume Next
    If gDebugtime Then
        f = FreeFile
        Open DebugFile For Append Access Write As #f
        Print #f, Message
        Close #f
    Else
        Debug.Print Message
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DebugSaveEx(ParamArray d())
Dim f As Long
Dim v
    On Error Resume Next
    If gDebugtime Then
        f = FreeFile
        Open DebugFile For Append Access Write As #f
        For Each v In d
            Print #f, v,
        Next
            Print #f,
        Close #f
    Else
        For Each v In d
            Debug.Print v,
        Next
        Debug.Print
    End If
End Sub

#End If

