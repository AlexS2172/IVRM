Attribute VB_Name = "basSettings"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Const FILE_CHM = "EgarHedger.chm"

Public Const APP_NAME = "Egar Hedger SQL"
Public Const APP_TITLE = "Egar Hedger"
Public Const APP_KEY = "EgarHedger"
Public Const APP_SETTINGS = "Settings"

Public Const APP_REG_KEY = "SOFTWARE\Egar\ETS\Hedger\"

Public Enum enmStocksFilter
    STOCKS_PORTFOLIO = 0
    STOCKS_SUPPORTED
End Enum

Public g_lMainTop As Long
Public g_lMainLeft As Long
Public g_lMainWidth As Long
Public g_lMainHeight As Long
Public g_lMainState As Long

Public g_VolaSource As VMELib.VolatilitySource

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub InitVolaSources()
    On Error GoTo EH
    Dim vmDB As New VADBLAYOUTLib.DBLayout
    
    Set g_VolaSource = New VMELib.VolatilitySource

    vmDB.ConnectionString = g_Params.DbConnection

    Set g_VolaSource.DataSource = vmDB
    g_VolaSource.EnableEvents = True
    g_VolaSource.EnableEditing = True

    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to init volatility surfaces."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadSettings()
    LoadMainWindowPos

    g_CalcModel = MyGetSettingLong("CalcModel", ETS_BINOMIAL)
    If g_CalcModel < ETS_BINOMIAL Then
        g_CalcModel = ETS_BINOMIAL
    ElseIf g_CalcModel > ETS_BINOMIAL_OPT Then
        g_CalcModel = ETS_BINOMIAL_OPT
    End If
    
    g_ReportPricesFeed = False 'MyGetSettingBool("ReportPricesFeed", False)

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SaveSettings()
    On Error Resume Next
    SaveMainWindowPos
    
    MySaveSettingBool "ReportPricesFeed", g_ReportPricesFeed

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadMainWindowPos()
    On Error Resume Next
    g_lMainTop = MyGetSettingLong("MainTop", 0)
    g_lMainLeft = MyGetSettingLong("MainLeft", 0)
    g_lMainWidth = MyGetSettingLong("MainWidth", 0)
    g_lMainHeight = MyGetSettingLong("MainHeight", 0)
    g_lMainState = MyGetSettingLong("MainState", vbNormal)
    If g_lMainState <> vbMaximized Then
        g_lMainState = vbNormal
    End If
    g_bShowHelpTags = MyGetSettingBool("ShowHelpTags", True)
End Sub
    
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SaveMainWindowPos()
    On Error Resume Next
    MySaveSettingLong "MainTop", g_lMainTop
    MySaveSettingLong "MainLeft", g_lMainLeft
    MySaveSettingLong "MainWidth", g_lMainWidth
    MySaveSettingLong "MainHeight", g_lMainHeight
    MySaveSettingLong "MainState", g_lMainState
    MySaveSettingBool "ShowHelpTags", g_bShowHelpTags
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MyGetSetting(ByVal Key As String, Optional ByVal DefaultValue As String) As String
    MyGetSetting = GetStrKeyValueEx(APP_REG_KEY & "Settings", Key, DefaultValue)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MyGetSettingLong(ByVal Key As String, Optional ByVal DefaultValue As Long) As Long
    MyGetSettingLong = GetLongKeyValueEx(APP_REG_KEY & "Settings", Key, DefaultValue)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MyGetSettingBool(ByVal Key As String, Optional ByVal DefaultValue As Boolean) As Boolean
    MyGetSettingBool = 0 <> MyGetSettingLong(Key, IIf(DefaultValue, 1&, 0&))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MyGetSettingDouble(ByVal Key As String, Optional ByVal DefaultValue As Double) As Double
    MyGetSettingDouble = MyGetSetting(Key, DefaultValue)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub MySaveSetting(ByVal Key As String, ByVal Value As String)
    SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & APP_SETTINGS, Key, Value
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub MySaveSettingLong(ByVal Key As String, ByVal Value As Long)
    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & APP_SETTINGS, Key, Value
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub MySaveSettingBool(ByVal Key As String, ByVal Value As Boolean)
    MySaveSettingLong Key, IIf(Value, 1&, 0&)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub MySaveSettingDouble(ByVal Key As String, ByVal Value As Double)
    MySaveSetting Key, Value
End Sub

Public Function CheckInternetConnection() As Boolean
    On Error GoTo EH
    Dim IvSettings As ETSMANAGERLib.IvSettings

    CheckInternetConnection = False
    Set IvSettings = New ETSMANAGERLib.IvSettings
    
    CheckInternetConnection = (IvSettings.VolLogin <> "" And IvSettings.VolPassword <> "")
    
    Set IvSettings = Nothing
    Exit Function
EH:
    gCmn.ErrorMsgBox "Fail to check IV connection settings."
    Set IvSettings = Nothing
End Function

