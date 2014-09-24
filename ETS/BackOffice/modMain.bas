Attribute VB_Name = "modMain"
Option Explicit

Public Const DB_VERSION_COMPATIBLE_MAJOR& = 4
Public Const DB_VERSION_COMPATIBLE_MINOR& = 46
Public Const DB_VERSION_COMPATIBLE_BUILD& = 158
Global g_frmOwner As Form

' option variables
Public Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hWnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long
Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Public Const STANDARD_RIGHTS_REQUIRED As Long = &HF0000
Public Const MUTANT_QUERY_STATE As Long = &H1
Public Const SYNCHRONIZE As Long = &H100000
Public Const MUTEX_ALL_ACCESS As Long = STANDARD_RIGHTS_REQUIRED + SYNCHRONIZE + MUTANT_QUERY_STATE

Public Declare Function OpenMutex Lib "kernel32" Alias "OpenMutexA" (ByVal dwDesiredAccess As Long, ByVal bInheritHandle As Long, ByVal lpName As String) As Long
Public Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" (lpMutexAttributes As Long, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Public Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Public Declare Function GetComputerName Lib "kernel32.dll" Alias "GetComputerNameA" (Name As Byte, ByRef Size As Long) As Boolean

Public Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long
Public Const SM_CXVSCROLL As Long = 2
Public Const SM_CYHSCROLL As Long = 3

Private Const APP_BO_TITLE = "IVRmBO"
Private Const APP_IVRM_TITLE = "IVRm"
Private Const APP_BO_LONGTITLE = "IV Risk Management BO"
Private Const APP_COPYRIGHT As String = "Copyright © 2001-2006, EGAR Technology Inc."

Public Const APP_REG_KEY = "SOFTWARE\Egar\ETS\Back Office\"
Public Const PRODUCT_REG_KEY As String = "SOFTWARE\Egar\ETS\"

Public Const APP_XML_KEY As String = "ETS\BackOffice\"
Public Const PRODUCT_XML_KEY As String = "ETS\"
Public Const GROUP_XML_KEY As String = "ETS\Asp\Groups\"
Public Const APP_MM_XML_KEY As String = "ETS\MarketMaker\"

Public Const GENERAL_SETTINGS As String = "ETS\Asp\GeneralSettings\"


Public Const GCOLOR_BUTTON_BACK = &HAD9A94
Public Const GCOLOR_BUTTON_FORE = &H0
Public Const GCOLOR_BUTTON_HOVER_BACK = &HCEBAB5
Public Const GCOLOR_BUTTON_HOVER_FORE = &H0
Public Const GCOLOR_BUTTON_PRESS_BACK = &H8CCFFF
Public Const GCOLOR_BUTTON_PRESS_FORE = &H0
Public Const GCOLOR_BUTTON_DISABLED_BACK = &HAD9A94
Public Const GCOLOR_BUTTON_DISABLED_FORE = &HCCCCCC

Public Const GCOLOR_RED_ACTIVE_LABEL_FORE = &H66CCFF
Public Const GCOLOR_RED_ACTIVE_LABEL_HOVERFORE = &HBCE7FF
Public Const GCOLOR_RED_ACTIVE_LABEL_PRESSFORE = &H0

Public Const GCOLOR_DIALOGUE_BACK = &HE7E3E7
Public Const GCOLOR_DIALOGUE_BACK2 = &HF7F3F7

Public Const GCOLOR_TABLE_CAPTION_BACK = GCOLOR_BUTTON_BACK
Public Const GCOLOR_TABLE_CAPTION_FORE = GCOLOR_BUTTON_FORE
Public Const GCOLOR_TABLE_WHITE_BACK = &HFFFFFF
Public Const GCOLOR_TABLE_WHITE_FORE = &H0
Public Const GCOLOR_TABLE_SELECT_BACK = &H9CD7F7
Public Const GCOLOR_TABLE_SELECT_FORE = &H0
Public Const GCOLOR_TABLE_SELECT2_BACK = &H66CCFF
Public Const GCOLOR_TABLE_SELECT2_FORE = &H0
Public Const GCOLOR_TABLE_CHANGED_FORE = vbRed
Public Const GCOLOR_TABLE_NEW_FORE = vbBlue
Public Const GCOLOR_TABLE_DELETED_FORE = &HC0C0C0
Public Const GCOLOR_TABLE_ODD_ROW = &HFFFFFF
Public Const GCOLOR_TABLE_EVEN_ROW = &HEEEEEE


Public Const GSTR_NA = "N/A"

Public Const GF_MAX_NUMBER = 100000000000#
Public Const GINT_MAX_NUMBER_TEXT_SIZE = 20
Public Const GINT_MAX_TEXT_SIZE = 255


Public g_lLogTop As Long
Public g_lLogLeft As Long
Public g_lLogWidth As Long
Public g_lLogHeight As Long
Public g_frmLog As frmLog

Public gDBW As clsDBWork
Public gCmn As clsCommon

Public Const GINT_NONEID = 0
Public ShowForm As Boolean  'to navigate between child forms

Public g_Providers As PRICEPROVIDERSLib.Providers
Public g_StructureProvider As PRICEPROVIDERSLib.StructureProviderEx
Public g_PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo

Public g_ReportMutex As Boolean
Public g_VolaSource As VolatilitySourcesLib.VolatilitySource
Public g_FolderForPositionsFile As String
Public g_FolderForExportFile As String
Public g_collHolidays As clsHolidayColl

'global array for exchanges definitions
Public garrExchanges() As Variant

Global g_sComputerName As String
Global g_lSizeComputerName As Long

Global g_Params As New clsParams
Global g_aUserXMLParams As ETSXMLParamsLib.XMLParams
Global g_aMainXMLParams As ETSXMLParamsLib.XMLParams

Global MmwMsgManager As MMWLib.MsgManager

Private m_hSingleInstanceMutex As Long
Private Const SINGLE_INSTANCE_MUTEX_NAME As String = "EGAR ETS BO 5124AB3B-AD2A-4324-8850-D8F8432BC6B4"

Private m_bShutDownNow As Boolean

Private m_bFirstRun As Boolean

Sub Main()
    On Error GoTo EH
    
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    ' Avoid 'Switch to' message
    '
    App.OleRequestPendingTimeout = 2000000000 ' in msec
    App.OleServerBusyTimeout = 2000000000 ' in msec
    m_bShutDownNow = False
    LoadSettings
    Set gCmn = New clsCommon
    
#If SIMULATE Then
    MsgBox "Simulation mode! For development only!", vbExclamation
#End If
      
    If Not g_Params.IsDebug Then
        m_hSingleInstanceMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, SINGLE_INSTANCE_MUTEX_NAME)
        If m_hSingleInstanceMutex <> 0 Then Exit Sub
            
        m_hSingleInstanceMutex = CreateMutex(0, 0, SINGLE_INSTANCE_MUTEX_NAME)
    End If
    
    ShowForm = 0
    
    g_Params.LoadSettings
    
    Load frmSplash
    g_Params.SetIcon WND_MAIN, frmSplash.hWnd, True

    frmSplash.Show
    frmSplash.Refresh
    
    Set g_frmOwner = frmSplash
    
    frmSplash.lblStatus.Caption = "Connecting to database..."
    DoEvents
    
    
    Set gDBW = New clsDBWork
    
    'gDBW.usp_RegUserAction_Save Now, 2, CurrentNtUserName, g_sComputerName, g_Params.UserGroup
    DoEvents
    If Not gDBW.CheckDBConnection(frmSplash.hWnd, m_bFirstRun) Then
        gCmn.MyMsgBox "Fail to connect to database. Application will be closed.", vbCritical
        ShutDown True
        Exit Sub
    End If
    
    LoadDBVersion (True)
'    Do While Not LoadDBVersion(True)
'        'If Not gDBW.ChangeDatabase(frmSplash.hWnd) Then
'            gCmn.MyMsgBox "Fail to connect to database. Application will be closed.", vbCritical
'            ShutDown True
'            Exit Sub
'        'End If
'    Loop
                    
'    If Not CheckAspLicense(frmSplash) Then
'        ShutDown True
'        Exit Sub
'    End If
    
    frmSplash.lblStatus.Caption = "Initializing volatility source..."
    DoEvents
    InitVolaSources m_bFirstRun
        
    frmSplash.lblStatus.Caption = "Connecting to real-time feed..."
    DoEvents
    
    InitPriceProviders m_bFirstRun
    
    frmSplash.lblStatus.Caption = "Loading..."
    Set g_collHolidays = New clsHolidayColl
    ReloadHolidays
    g_ReportMutex = m_bFirstRun
    DoEvents
    Load frmMain
    frmMain.Show
    
    Set g_frmOwner = frmMain
    
    Unload frmSplash
    
    LoadInterestRates
    LoadWtdVegaSettings
    
    InitMessaging
    
    Exit Sub
EH:
    ShowError "Failed to start " & AppTitle()
    ShutDown True
    Exit Sub
End Sub

Public Sub InitPriceProviders(ByVal bFirstRun As Boolean)
    On Error GoTo EH
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Dim aBatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
    
    Dim bRetry As Boolean
    
    bRetry = False
    
    Set g_Providers = New PRICEPROVIDERSLib.Providers
    g_Providers.Initialize
    
    bRetry = True

Retry:
    On Error GoTo EH
           
    If g_StructureProvider Is Nothing Then Set g_StructureProvider = New PRICEPROVIDERSLib.StructureProviderEx
    If aPT Is Nothing Then Set aPT = g_StructureProvider
        
    aPT.Type = g_Params.PriceProviderType
    
    g_StructureProvider.Connect
    
    Set aPT = Nothing
    
    If g_PriceProvider Is Nothing Then Set g_PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    Set aPT = g_PriceProvider
        
    aPT.Type = g_Params.PriceProviderType
    
    g_PriceProvider.Connect
    
    'Set CanadianRegion USD/CAD rate
    Set aBatchPriceProvider = g_PriceProvider
    If (Not aBatchPriceProvider Is Nothing) Then
        Dim dRate As Double
        dRate = g_Params.ExchangeRate
        aBatchPriceProvider.SetRegionRate CAN_REGION, CDbl(dRate)
        Set aBatchPriceProvider = Nothing
    End If
    
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to initialize price provider."
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source
        
'    If bRetry Then
'        If Not bFirstRun Then gCmn.ErrorMsgBox sDescription
'
'        On Error Resume Next
'        Dim frmConnect As frmConnections
'        Set frmConnect = New frmConnections
'
'        If frmConnect.Execute(True, enCsoPriceProviderOnly, True, Nothing) = vbOK Then
'            Set frmConnect = Nothing
'            Resume Retry
'        Else
'            On Error GoTo 0
'
'            Set frmConnect = Nothing
'            Set aPT = Nothing
'            Set g_StructureProvider = Nothing
'
'            Err.Clear
'            Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'        End If
'    End If

    Set aPT = Nothing
    Set g_StructureProvider = Nothing
    Set g_PriceProvider = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
    
End Sub

Private Sub InitVolaSources(ByVal bFirstRun As Boolean)
    On Error GoTo EH
    Dim bRetry As Boolean, sName$, sFileName$, sVersion$, nId&, sProgID$
    Dim vmDB As VADBLAYOUTLib.DBLayout
    Dim aVolaSources As VolatilitySourcesLib.VolatilitySources
    
    bRetry = False
    
    Set vmDB = New VADBLAYOUTLib.DBLayout
    Set aVolaSources = New VolatilitySourcesLib.VolatilitySources
    
    On Error Resume Next
    aVolaSources.GetVolatilitySourceInfoByID g_Params.VolatilitySourceType, sName, sFileName, sVersion, sProgID
    g_Params.VolatilitySourceProgID = sProgID

    bRetry = True

Retry:
    On Error GoTo EH
    Set g_VolaSource = aVolaSources.VolatilitySource(g_Params.VolatilitySourceType)
    vmDB.ConnectionString = g_Params.DbConnection

    Set g_VolaSource.DataSource = vmDB
    g_VolaSource.EnableEvents = True
    g_VolaSource.EnableEditing = True
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to initialize volatility source."
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source
    
'    If bRetry Then
'        If Not bFirstRun Then gCmn.ErrorMsgBox sDescription
'
'        On Error Resume Next
'        Dim frmConnect As frmConnections
'        Set frmConnect = New frmConnections
'
'        If frmConnect.Execute(True, enCsoVolatilitySourceOnly, True, Nothing) = vbOK Then
'            Set g_VolaSource = Nothing
'            Set frmConnect = Nothing
'            Resume Retry
'        Else
'            On Error GoTo 0
'            Set frmConnect = Nothing
'            Set vmDB = Nothing
'            Set aVolaSources = Nothing
'            Set g_VolaSource = Nothing
'            Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'        End If
'    End If

    g_VolaSource.UnregisterPublisher
    
    Set vmDB = Nothing
    Set aVolaSources = Nothing
    Set g_VolaSource = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Public Sub ShutDown(ByVal bEndExecution As Boolean)
    On Error Resume Next
    'gDBW.usp_RegUserAction_Save Now, 3, CurrentNtUserName, g_sComputerName, g_Params.UserGroup
    If m_bShutDownNow Then Exit Sub
    
    Dim frm As Object, i&
    
    DeleteMessageing
    
    g_VolaSource.EnableEvents = False
    g_VolaSource.UnregisterPublisher
    
    m_bShutDownNow = True
    SaveSettings
    g_Params.SaveSettings
    ClearInterestRates
    ClearWtdVegaSettings
    
    If Not g_frmLog Is Nothing Then
        Set g_frmLog = Nothing
    End If
    
    For Each frm In Forms
        Unload frm
    Next frm
    
    Set g_StructureProvider = Nothing
    Set g_PriceProvider = Nothing
    Set g_Providers = Nothing
        
    Set g_VolaSource = Nothing
    Set gDBW = Nothing
    Set gCmn = Nothing
    Set g_collHolidays = Nothing

    If Not g_Params.IsDebug Then
        If m_hSingleInstanceMutex <> 0 Then CloseHandle m_hSingleInstanceMutex
    End If

    Set g_frmOwner = Nothing
    If bEndExecution Then
        m_bShutDownNow = False
        End
    End If
    m_bShutDownNow = False
End Sub

Public Sub Restart()
    ShutDown False
    Main
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    Set g_aMainXMLParams = aXMLLoader.Load(MainXMLFilePath)
    Set g_aUserXMLParams = aXMLLoader.Load(CurrentUserXMLFilePath)
    
    g_lSizeComputerName = 50
    g_sComputerName = ""
    Dim helpComputerName(0 To 50) As Byte
    If GetComputerName(helpComputerName(0), g_lSizeComputerName) = 0 Then
        gCmn.ErrorMsgBox "Fail to Get Computer Name. "
    End If
    
    Dim Counter As Integer
    Counter = 0
    
    Do While helpComputerName(Counter) > 0
        g_sComputerName = g_sComputerName + Chr(helpComputerName(Counter))
        Counter = Counter + 1
    Loop
    
    If g_aMainXMLParams Is Nothing Then
        Set g_aMainXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    If g_aUserXMLParams Is Nothing Then
        Set g_aUserXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    
    g_lLogTop = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogTop", 0)
    g_lLogLeft = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogLeft", 0)
    g_lLogWidth = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogWidth", 0)
    g_lLogHeight = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogHeight", 0)
    
    'g_FolderForPositionsFile = GetStrKeyValueEx(APP_REG_KEY & "Settings", "FolderForPositionsFile", "")
    'g_FolderForExportFile = GetStrKeyValueEx(APP_REG_KEY & "Settings", "FolderForExportFile", "")
    
    m_bFirstRun = False '(GetLongKeyValueEx(APP_REG_KEY & "Settings", "FirstRun", 1) <> 0)
    
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogTop", g_lLogTop
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogLeft", g_lLogLeft
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogWidth", g_lLogWidth
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogHeight", g_lLogHeight
    
    'SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "FolderForPositionsFile", g_FolderForPositionsFile
    'SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "FolderForExportFile", g_FolderForExportFile
    
    If m_bFirstRun Then g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "FirstRun", 0
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    aXMLLoader.Save CurrentUserXMLFilePath, g_aUserXMLParams
End Sub

Public Sub ChangeExchanges()
    On Error GoTo EH
    Dim rst As ADODB.Recordset
    Dim i As Long
    
    Set rst = gDBW.usp_Exchange_Get(Null)
    rst.MoveFirst
    i = 0
    Do Until rst.EOF
        ReDim Preserve garrExchanges(2, i)
        garrExchanges(0, i) = rst("iExchangeID")
        garrExchanges(1, i) = rst("vcExchangeName")
        rst.MoveNext
        i = i + 1
    Loop
    garrExchanges(2, 0) = True
        
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Fail to load exchanges."
End Sub

Public Function GetExchangesIDs() As String
    Dim sParams As String
    Dim i As Long, nUBound As Long: nUBound = -1
    
    nUBound = UBound(garrExchanges, 2)
    For i = 0 To nUBound
        sParams = sParams & garrExchanges(0, i) & "|"
    Next
    GetExchangesIDs = sParams
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub EditInternetConnections()
    On Error Resume Next
    frmEditInternetConnections.Execute
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

Public Sub ReloadHolidays()
    Dim rs As ADODB.Recordset, sKey$, aHoliday As clsHolidayAtom
    On Error GoTo EH
    Set rs = gDBW.usp_Holiday_Get(Null, Null)
    If Not rs Is Nothing Then
        g_collHolidays.Clear
        With rs
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    sKey = Format$(gCmn.ReadDate(!dtHolidayDate), "yyyymmdd")
                    Set aHoliday = g_collHolidays(UCase(sKey))
                    If aHoliday Is Nothing Then
                        Set aHoliday = g_collHolidays.Add(UCase(sKey))
                        aHoliday.HolidayDate = gCmn.ReadDate(!dtHolidayDate)
                        aHoliday.HolidayId = gCmn.ReadLng(!iHolidayID)
                        Set aHoliday = Nothing
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    Set rs = Nothing
        
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Fail to load holidays."
End Sub

Private Function CheckAspLicense(ByRef frmOwner As Form) As Boolean
    On Error Resume Next
    Dim sUserName$, nSize&, nStatus&, sBadStatusDesc$, bSucceeded As Boolean
    
    bSucceeded = False
    sUserName = CurrentNtUserName
    
    If Len(sUserName) > 0 Then
        Err.Clear
        nStatus = gDBW.usp_AspUserStatus_Get(sUserName)
        If Err.Number = 0 Then
            Select Case nStatus
                Case ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_TRIAL, _
                    ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_WITH_PAYMENT
                    bSucceeded = True
                    
'                    Case ASP_SUBSCRIPTION_STATUS_EXPIRED
'                        sBadStatusDesc = "Your subscription period is expired."
'
'                    Case ASP_SUBSCRIPTION_STATUS_CANCELED_BY_USER
'                        sBadStatusDesc = "Your subscription is cancelled by yourself."
'
'                    Case ASP_SUBSCRIPTION_STATUS_CANCELED_BY_ADMIN
'                        sBadStatusDesc = "Your subscription is cancelled by administrator."
'
'                    Case ASP_SUBSCRIPTION_STATUS_SUSPENDED
'                        sBadStatusDesc = "Your subscription is suspended."
'
'                    Case ASP_SUBSCRIPTION_STATUS_CARD_INVALID
'                        sBadStatusDesc = "Your credit card is invalid."
                
                Case Else ' or ASP_SUBSCRIPTION_STATUS_NOT_SUBSCRIBED
                    sBadStatusDesc = "You are not subscribed to a service."
            End Select
        Else
            sBadStatusDesc = "Fail to finish checking procedure: " & Err.Description
        End If
    Else
        sBadStatusDesc = "Fail to get current user name."
    End If
    
    If Not bSucceeded Then
        gCmn.MyMsgBox frmOwner, "Can not validate " & AppTitle() & " subscription status!" & vbCrLf & sBadStatusDesc & vbCrLf & _
                            "Please contact IVolatility.com support team.", vbCritical
    End If
    
    CheckAspLicense = bSucceeded
End Function

Public Function AppIvRmTitle() As String
 On Error Resume Next
 If Len(g_Params.AppIvRmTitle) Then
    AppIvRmTitle = g_Params.AppIvRmTitle
 Else
    AppIvRmTitle = APP_IVRM_TITLE
 End If
End Function


Public Function AppTitle() As String
 On Error Resume Next
 If Len(g_Params.AppTitle) Then
    AppTitle = g_Params.AppTitle
 Else
    AppTitle = APP_BO_TITLE
 End If
End Function

Public Function AppLongTitle() As String
 On Error Resume Next
 If Len(g_Params.AppLongTitle) Then
    AppLongTitle = g_Params.AppLongTitle
 Else
    AppLongTitle = APP_BO_LONGTITLE
 End If
End Function
Public Function AppDescription() As String
 On Error Resume Next
 If Len(g_Params.AppDescription) Then
    AppDescription = g_Params.AppDescription
 Else
    AppDescription = App.FileDescription
 End If
End Function

Public Function AppCopyright() As String
 On Error Resume Next
 If Len(g_Params.AppCopyright) Then
    AppCopyright = g_Params.AppCopyright
 Else
    AppCopyright = APP_COPYRIGHT
 End If
End Function

Private Sub InitMessaging()
    On Error GoTo EH
    
    If MmwMsgManager Is Nothing Then
        Dim aPubManager As MMWLib.IPubManager
        
        Set MmwMsgManager = New MMWLib.MsgManager
            
        Set aPubManager = MmwMsgManager
            aPubManager.RegPublisher enMtFlexOption, Nothing
        Set aPubManager = Nothing
        
        MmwMsgManager.SubFlexOption
        
    End If
        
    Exit Sub
EH:
    Set aPubManager = Nothing
    Debug.Print "Fail to initialize messaging service."
End Sub

Private Sub DeleteMessageing()
    On Error GoTo EH
    Dim aPubManager As MMWLib.IPubManager
    
    MmwMsgManager.UnsubFlexOption
    
    Set aPubManager = MmwMsgManager
        aPubManager.UnregPublisher enMtFlexOption, Nothing
    Set aPubManager = Nothing
    
    Set MmwMsgManager = Nothing
    Exit Sub
EH:
    Debug.Print "Fail to unsubscribe messaging service."
End Sub


Public Sub PubNewFlexOptionRoot(ByVal RootName As String, ByVal RootID As Long, ByVal UndId As Long, ByVal LotSize As Long)
    On Error GoTo EH
    
    Dim lRetCode As Long
            
    Dim aBroadcastMsg As MSGSTRUCTLib.FlexOption
    Set aBroadcastMsg = New MSGSTRUCTLib.FlexOption
    
    Dim aPubManager As MMWLib.IPubManager
    Set aPubManager = MmwMsgManager
    
        aBroadcastMsg.LotSize = LotSize
        aBroadcastMsg.RootName = RootName
        aBroadcastMsg.RootID = RootID
        aBroadcastMsg.UndId = UndId
    
        lRetCode = aPubManager.PubFlexOption(aBroadcastMsg)
            
    Set aPubManager = Nothing
    Exit Sub
    
EH:
    Debug.Print "Fail to pub NewFlexOptionRoot"
End Sub
