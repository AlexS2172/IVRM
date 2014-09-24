Attribute VB_Name = "modMain"
Option Explicit

Public Const DB_VERSION_COMPATIBLE_MAJOR& = 3
Public Const DB_VERSION_COMPATIBLE_MINOR& = 46
Public Const DB_VERSION_COMPATIBLE_BUILD& = 123

Public Const PRODUCT_REG_KEY As String = "SOFTWARE\Egar\ETS\"
Private Const APP_BO_TITLE = "IVRmDataLoader"
Private Const APP_IVRM_TITLE = "IVRm"
Private Const APP_BO_LONGTITLE = "IV Risk Management"
Private Const APP_COPYRIGHT As String = "Copyright © 2001-2006, EGAR Technology Inc."

Public Const APP_XML_KEY As String = "ETS\MarketMaker\"
Public Const PRODUCT_XML_KEY As String = "ETS\"
Public Const GROUP_XML_KEY As String = "ETS\Asp\Groups\"

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

Global g_aMainXMLParams As ETSXMLParamsLib.XMLParams
Global g_aUserXMLParams As ETSXMLParamsLib.XMLParams

Global g_sFtpFolder As String
Global g_sVolaFile As String
Global g_sIndexWeightFile As String
Global g_sDividendsFile As String
Global g_sInterestRatesFile As String
Global g_sUserGroup As String
Global g_sImportLogPath As String
Global g_sAppLogPath As String
Global g_sDataHistoryPath As String
Global g_lStartTime As Long
Global g_lStopTime As Long
Global g_lFrequency As Long

Global g_lLogLeft As Long
Global g_lLogTop As Long
Global g_lLogWidth As Long
Global g_lLogHeight As Long

Global g_lMinLogLevel As Long

Global g_PerformanceLog As clsPerformanceLog

Global g_StartTimeInMinute As Long
Global g_StopTimeInMinute As Long

Global g_sComputerName As String
Global g_lSizeComputerName As Long
Global g_frmMain As frmMain
Global g_frmOwner As Form
Global g_Params As clsParams
Public gDBW As clsDBWork
Public gCmn As clsCommon
Public g_frmLog As frmLog
Global MmwMsgManager As MMWLib.MsgManager

Public Enum eContractType
    GINT_INDEXES = 1
    GINT_STOCKS = 2
    GINT_FUTURES = 4
    GINT_FUTUREOPTIONS = 5
    GINT_COMMODITIES = 6
End Enum

Public Enum LOAD_STRING_FROM_FILE
    LSFF_OK = 0
    LSFF_EMPTY = 1
    LSFF_NOT_EXISTS = 3
End Enum


Public Sub InitMessaging()
    On Error GoTo EH
    
    If MmwMsgManager Is Nothing Then
        Dim aPubManager As MMWLib.IPubManager
        
        Set MmwMsgManager = New MMWLib.MsgManager
            
        Set aPubManager = MmwMsgManager
        aPubManager.RegPublisher enMtBroadcastMessage, Nothing
        Set aPubManager = Nothing
        
        MmwMsgManager.SubBroadcastMessage
    End If
        
    Exit Sub
EH:
    Set aPubManager = Nothing
    AddImportLog "Fail to initialize messaging service."
End Sub

Private Sub DeleteMessageing()
    On Error GoTo EH
    Dim aPubManager As MMWLib.IPubManager
    
    MmwMsgManager.UnsubBroadcastMessage
    
    Set aPubManager = MmwMsgManager
    aPubManager.UnregPublisher enMtBroadcastMessage, Nothing
    Set aPubManager = Nothing
    
    Set MmwMsgManager = Nothing
EH:
    AddImportLog "Fail to unsubscribe messaging service."
End Sub

Public Sub PubMessage(ByVal strMsg As String)
    On Error Resume Next
    
    Dim lRetCode As Long
            
    Dim aBroadcastMsg As MSGSTRUCTLib.BroadcastMessage
    Dim aPubManager As MMWLib.IPubManager
    
    Set aPubManager = MmwMsgManager
    Set aBroadcastMsg = New MSGSTRUCTLib.BroadcastMessage
    
    aBroadcastMsg.Code = CLng(777)
    aBroadcastMsg.Facility = enFacIVRMDataLoader
    aBroadcastMsg.Description = strMsg
    aBroadcastMsg.Priority = enPrNormal
    aBroadcastMsg.Severity = enSevInfo
    
    lRetCode = aPubManager.PubBroadcastMessage(aBroadcastMsg)
            
    Set aPubManager = Nothing

End Sub


Public Sub InitGlobals()

    Set g_frmMain = New frmMain
    Set g_frmOwner = g_frmMain
    Set g_frmLog = New frmLog
    Set gCmn = New clsCommon
    Set gDBW = New clsDBWork
    Set g_Params = New clsParams
    InitMessaging
    
End Sub

Public Sub DeleteGlobals()

    SaveSettings
    Set g_frmMain = Nothing
    Set g_frmOwner = Nothing
    Set g_frmLog = Nothing
    Set gCmn = Nothing
    Set gDBW = Nothing
    Set g_Params = Nothing
    Set g_PerformanceLog = Nothing
    DeleteMessageing
    
End Sub

Sub Main()
    
    InitGlobals
    
    LoadSettings
    
    InitLogger
    
    g_frmMain.Show
    
End Sub

Private Sub InitLogPos()
    g_lLogLeft = 20
    g_lLogTop = 20
    g_lLogWidth = 300
    g_lLogHeight = 100
End Sub

Private Sub InitLogger()

        Set g_PerformanceLog = New clsPerformanceLog
        
        g_lMinLogLevel = 9
        
        If Not g_PerformanceLog.Init("IVDL_PerfLog", g_sAppLogPath) Then
            Set g_PerformanceLog = Nothing
        End If
        
        Dim nOperation&
        If Not g_PerformanceLog Is Nothing Then _
            nOperation = g_PerformanceLog.BeginLogMmOperation
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADINITDATA, "IVDL"
            
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Init logger", "IVDL"
        
End Sub

Public Function MainXMLFilePath$()
    On Error Resume Next
    MainXMLFilePath = GetStrKeyValueEx(PRODUCT_REG_KEY & "Asp" & "\Settings", "PathToMainXML", "")
End Function


Public Sub LoadSettings()
    On Error Resume Next
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    
    Set g_aMainXMLParams = aXMLLoader.Load(MainXMLFilePath)
    If g_aMainXMLParams Is Nothing Then
        Set g_aMainXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    
    g_Params.LoadSettings
    
End Sub

Public Sub SaveSettings()
    On Error Resume Next
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    If Not aXMLLoader Is Nothing And Not g_aMainXMLParams Is Nothing And Len(MainXMLFilePath) <> 0 Then
        aXMLLoader.Save MainXMLFilePath, g_aMainXMLParams
    End If
End Sub

Public Function AppIVRmTitle() As String
 On Error Resume Next
 If Len(g_Params.AppIVRmTitle) Then
    AppIVRmTitle = g_Params.AppIVRmTitle
 Else
    AppIVRmTitle = APP_IVRM_TITLE
 End If
End Function

Public Function GetTimeFromLongToMinute(ByVal lTime As Long) As Long
    
    If lTime > 0 Then
        If (lTime / 100) < 25 And (lTime Mod 100) < 60 Then
            GetTimeFromLongToMinute = ((lTime - (lTime Mod 100)) / 100) * 60 + (lTime Mod 100)
        End If
    End If
    
End Function

Public Function GetCurrentTimeInMinutes&()
    GetCurrentTimeInMinutes = Hour(Time) * 60 + Minute(Time)
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

Public Sub AddImportLog(ByVal sInfo As String)
    If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, sInfo, "IVDL"
End Sub

Public Function AppCopyright() As String
 On Error Resume Next
 If Len(g_Params.AppCopyright) Then
    AppCopyright = g_Params.AppCopyright
 Else
    AppCopyright = APP_COPYRIGHT
 End If
End Function
