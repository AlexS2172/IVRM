Attribute VB_Name = "modMain"
Option Explicit

Public Const APP_LIC_NAME As String = "EGAR ETS 85A70EF2-7BFD-446F-BBB9-D9BE82063CE4"
Public Const APP_REG_KEY As String = "SOFTWARE\Egar\ETS\Market Maker\"
Public Const PRODUCT_REG_KEY As String = "SOFTWARE\Egar\ETS\"

Public Const APP_XML_KEY As String = "ETS\MarketMaker\"
Public Const PRODUCT_XML_KEY As String = "ETS\"
Public Const GROUP_XML_KEY As String = "ETS\Asp\Groups\"
Public Const GENERAL_SETTINGS As String = "ETS\Asp\GeneralSettings\"

Public Const DATA_SHAPSHOT_FILENAME$ = "EtsMM_DataShapshot"
Public Const DB_VERSION_COMPATIBLE_MAJOR& = 4
Public Const DB_VERSION_COMPATIBLE_MINOR& = 46
Public Const DB_VERSION_COMPATIBLE_BUILD& = 123

Public Const GCOLOR_LABEL_VALID As Long = &H80000008
Public Const GCOLOR_LABEL_INVALID As Long = &HC0&

Private Const APP_TITLE As String = "IVRm"
Private Const APP_LONG_TITLE As String = "IV Risk Management"
Private Const APP_COPYRIGHT As String = "Copyright © 2001-2009, EGAR Technology Inc."

Global g_lMainTop As Long
Global g_lMainLeft As Long
Global g_lMainWidth As Long
Global g_lMainHeight As Long
Global g_lMainState As Long
Global g_sComputerName As String
Global g_lSizeComputerName As Long

Global g_LastActiveWindow As Long

Global g_lMinLogLevel As Long
Global g_lLogLifetime As Long

Global g_lLogTop As Long
Global g_lLogLeft As Long
Global g_lLogWidth As Long
Global g_lLogHeight As Long
Global g_frmLog As frmLog

Global gDBW As clsDBWork
Global gCmn As clsCommon
Global g_TradeChannel As clsTradeChannel
Global g_OrdersProcessor As clsOrdersProcessor

Global g_ViewFrm As clsFormColl
Global g_ViewFrmID As Long

Global g_Params As New clsParams
Global g_aUserXMLParams As ETSXMLParamsLib.XMLParams
Global g_aMainXMLParams As ETSXMLParamsLib.XMLParams
Global g_nGraphicalPnLUntitledIdx As Long

Public Enum EntityTypeEnum
    ET_TRADER = &H1&
    ET_TRADER_GROUP = &H2&
    ET_STRATEGY = &H4&
    ET_BROKER = &H8&
    ET_UNDERLYING = &H10&
    ET_UNDERLYING_GROUP = &H20&
    ET_RM_SCENARIO = &H40&
    ET_EXP_CALENDAR = &H80&
    ET_INDEX = &H100&
    ET_EXCHANGE = &H200&
    ET_PRICE_PROFILE = &H400&
    ET_ORDER_EXEC_DEST = &H800&
    ET_TNT_CNT_PTY = &H1000&
    ET_ALL = &HFFFFFFFF
End Enum

Global g_CurTraderID As Long

Global g_Main As EtsGeneralLib.EtsMain

Global g_Trader As EtsGeneralLib.EtsTraderColl
Global g_TraderGroup As EtsGeneralLib.EtsTraderGroupColl
Global g_Strategy As EtsGeneralLib.EtsStrategyColl
Global g_Broker As EtsGeneralLib.EtsBrokerColl
Global g_ClearingBroker As EtsGeneralLib.EtsBrokerColl
Global g_Underlying As EtsGeneralLib.UndColl
Global g_UnderlyingAll As EtsGeneralLib.UndColl
Global g_UnderlyingGroup As EtsGeneralLib.EtsUndGroupColl
Global g_RmScenario As clsRmScenarioColl
Global g_ExpCalendar As EtsGeneralLib.EtsExpCalColl
Global g_Index As EtsGeneralLib.IndexColl
Global g_HedgeSymbols As EtsGeneralLib.IndexColl
Global g_BasketIndex As EtsGeneralLib.IndexColl
Global g_Exch As EtsGeneralLib.ExchColl
Global g_PriceProfile As EtsGeneralLib.EtsPriceProfileColl
Global g_CancelledOrderExecs As EtsGeneralLib.MmTradeInfoColl
Global g_OrderExecDest As EtsMmGeneralLib.MmOrderExecDestColl
Global g_OptRootByName As EtsGeneralLib.EtsOptRootByNameColl
Global g_ContractAll As EtsGeneralLib.EtsContractColl
Global g_Contract As EtsGeneralLib.EtsContractColl
Global g_Stock As EtsGeneralLib.UndColl

Global g_VolaSource As VolatilitySourcesLib.VolatilitySource

Global g_nQuotesUntitledIdx As Long
Global g_nRisksUntitledIdx As Long
Global g_nRiskMatrixUntitledIdx As Long
Global g_nTradesUntitledIdx As Long
Global g_nVolaUntitledIdx As Long
Global g_nQuotationUntitledIdx As Long
Global g_nVolaAnalysisUntitledIdx As Long
Global g_nGreeksUntitledIdx As Long
Global g_nHedgeSummaryUntitledIdx As Long
Global g_nReportsUntitledIdx As Long
Global g_nOrdersUntitledIdx As Long
Global g_nTntCardUntitledIdx As Long
Global g_nStockHedgeUntitledIdx As Long
Global g_nIndexHedge1UntitledIdx As Long
Global g_nIndexHedge2UntitledIdx As Long

Global g_frmWtdVegaSettings As frmWtdVegaSettings
Global g_nHedgeSummaryFileIdx As Long

Private m_hSingleInstanceMutex As Long
Private Const SINGLE_INSTANCE_MUTEX_NAME As String = "EGAR ETS MM EABA61FA-32DE-41D2-AE2B-F31A84FF9A47"

Global g_frmOwner As Form

Private m_bShutDownNow As Boolean

Global g_sLicenseKey As String
Global g_lcLicense As LicenseType
Global g_sLicenseCompany As String
Global g_sLicenseUser As String

Global g_bIseEnable As Boolean
Global g_bIsScannersEnable As Boolean

Global g_nDefaultTerm As Long

Global g_ClipMgr As clsClipMgr
Global g_ptDefWindowSize As POINTAPI

Global g_frmMatchedTrades As frmMatchedTrades

Global g_frmUnderlyings As clsFormAtom
Global g_frmEarlyExer As clsFormAtom

Private m_bFirstRun As Boolean
Global g_PerformanceLog As clsPerformanceLog

Private m_PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo

Global g_RTQuantity As Long
Global g_MaxRTQuantity As Long
Global g_RTNumber As Long

Global g_MaxRvRTQuantity As Long
Global g_RvRTQuantity As Long
Global g_RvRTNumber As Long

Global g_pOptionInfo As PROCESS_INFORMATION
Global g_pStrategyInfo As PROCESS_INFORMATION
Global g_ScannersLogin As String
Global g_bOldData As Boolean

Global g_ScreenExport As clsScreenExport

Global g_frmProjections As frmBatchCapability

'CV Functionality
Global g_cvContextInfo As clsCVInfo


Sub Main()
    On Error GoTo EH
    
    Dim bNewLicense As Boolean
    Dim iTimeRest As Long

    SetCrashHandler
    
    g_RTQuantity = 0
    g_MaxRTQuantity = 4
    g_RTNumber = 0
    
    g_LastActiveWindow = -1
    
    g_RvRTQuantity = 0
    g_MaxRvRTQuantity = 1
    g_RvRTNumber = 0
    
    g_pOptionInfo.dwProcessId = 0
    g_pOptionInfo.dwThreadId = 0
    g_pOptionInfo.hProcess = 0
    g_pOptionInfo.hThread = 0
    
    g_pStrategyInfo.dwProcessId = 0
    g_pStrategyInfo.dwThreadId = 0
    g_pStrategyInfo.hProcess = 0
    g_pStrategyInfo.hThread = 0
    g_nGraphicalPnLUntitledIdx = 0
    
    App.OleRequestPendingTimeout = 2000000
    App.OleServerBusyTimeout = 2000000
    
    g_nQuotesUntitledIdx = 0
    g_nRisksUntitledIdx = 0
    g_nRiskMatrixUntitledIdx = 0
    g_nTradesUntitledIdx = 0
    g_nVolaUntitledIdx = 0
    g_nQuotationUntitledIdx = 0
    g_nVolaAnalysisUntitledIdx = 0
    g_nGreeksUntitledIdx = 0
    g_nHedgeSummaryUntitledIdx = 0
    g_nReportsUntitledIdx = 0
    g_nOrdersUntitledIdx = 0
    g_nTntCardUntitledIdx = 0
    g_nStockHedgeUntitledIdx = 0
    g_nIndexHedge1UntitledIdx = 0
    g_nIndexHedge2UntitledIdx = 0
    
    m_bShutDownNow = False
    
    g_nHedgeSummaryFileIdx = 0
    
    g_nDefaultTerm = 252
    
    If Not g_Params.IsDebug Then
        m_hSingleInstanceMutex = OpenMutex(MUTEX_ALL_ACCESS, 0, SINGLE_INSTANCE_MUTEX_NAME)
        If m_hSingleInstanceMutex <> 0 Then Exit Sub
            
        m_hSingleInstanceMutex = CreateMutex(0, 0, SINGLE_INSTANCE_MUTEX_NAME)
    End If
    
    'InstallExceptionHandler
      
    Set gCmn = New clsCommon
    
    CreateGlobalCollections
    LoadSettings frmSplash
    g_Params.LoadSettings
    
    If (Not g_Main Is Nothing) Then
        g_Main.SetLogDirectory g_Params.LogDirectory
        g_Main.SetLogLevel g_lMinLogLevel
        g_Main.SetLogLifetime g_lLogLifetime
        
         g_Main.SetWorkingMode enWmClient
        
        g_Main.CalculationSettings.CurveExtrapolationType = g_Params.CurveExtrapolationType
        g_Main.CalculationSettings.CurveInterpolationType = g_Params.CurveInterpolationType
    End If
    
    g_Params.SetIcon WND_MAIN, frmSplash.hWnd, True
    
    InitEventLog
    Load frmSplash
    frmSplash.Show
    frmSplash.Refresh

    Set g_frmProjections = New frmBatchCapability
    Set g_frmOwner = frmSplash
        
    frmSplash.SetStatus "Loading application settings..."
    DoEvents
   
    frmSplash.SetStatus "Checking license ..."
    DoEvents
    
'    bNewLicense = False
'    If Not CheckLicense(frmSplash, bNewLicense) Then
'        ShutDown True
'        Exit Sub
'    End If
    
    Set gDBW = New clsDBWork
    gDBW.Init g_Params.DbConnection, g_Params.IvDbConnection
    
    On Error Resume Next
    gDBW.usp_RegUserAction_Save 0, CurrentNtUserName, g_sComputerName, g_Params.UserGroup
    On Error GoTo EH
    
    frmSplash.SetStatus "Connecting to database..."
    DoEvents
        
    If Not gDBW.CheckDBConnection(frmSplash, m_bFirstRun) Then
        gCmn.MyMsgBox frmSplash, "Fail to connect to database. Application will be closed.", vbCritical
        ShutDown True
        Exit Sub
    End If
    
    If Not CheckEODUpdate(iTimeRest, True) Then
        ShutDown True
        Exit Sub
    End If

    Do While Not LoadDBVersion(True)
        Set gDBW = Nothing
        Set gDBW = New clsDBWork
        gDBW.Init (g_Params.DbConnection)
        'If Not gDBW.ChangeDatabase(frmSplash, Nothing, True) Then
            gCmn.MyMsgBox frmSplash, "Fail to connect to database. Application will be closed.", vbCritical
            ShutDown True
            Exit Sub
        'End If
    Loop
    
    If Not CheckAspLicense(frmSplash) Then
        ShutDown True
        Exit Sub
    End If
    
    g_Params.LoadActualTradesEditable
    g_Params.LoadPriceRoundingRule
    g_Params.LoadUndPriceToleranceValue
    g_Params.LoadCommissionLowPremiumLimit
    
    Set g_TradeChannel = New clsTradeChannel
    Set g_OrdersProcessor = New clsOrdersProcessor
    
    frmSplash.SetStatus "Connecting to price provider..."
    DoEvents
    'InitPriceProvider

    If g_Params.LogAdvancedInfo Then
        Set g_PerformanceLog = New clsPerformanceLog
        
        If Not g_PerformanceLog.Init("ivrm_std", g_Params.LogDirectory) Then
            Set g_PerformanceLog = Nothing
        End If
        
        frmSplash.tmrPerformance.Interval = g_Params.LogPerfCountInterval
        frmSplash.tmrPerformance.Enabled = g_Params.LogPerformanceCounters
    End If
    
    frmSplash.SetStatus "Initializing volatility source..."
    DoEvents
    InitVolaSources m_bFirstRun
    
    frmSplash.SetStatus "Loading common data..."
    DoEvents
    g_CurTraderID = g_Params.DefTraderID
    
    If frmTrader.Execute(g_CurTraderID, frmSplash) <> vbOK Then
        gCmn.MyMsgBox frmSplash, "No active trader selected! Application will be closed.", vbCritical
        ShutDown True
        Exit Sub
    End If
    DoEvents
    
    Dim nOperation&
    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
        
    
    LoadEntities ET_ALL, frmSplash
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADINITDATA, "EtsMM"
        
    g_Params.LoadBatchShedule
    g_Params.LoadBatchReports
        
    frmSplash.SetStatus "Loading interest rates data..."
    DoEvents
    LoadInterestRates
    
    frmSplash.SetStatus "Loading default settings..."
    DoEvents
    
    g_Params.MakeUnderlingComboLists
    InitDefGrids
    
    Set g_ViewFrm = New clsFormColl
    Set g_frmWtdVegaSettings = New frmWtdVegaSettings
    Set g_frmMatchedTrades = New frmMatchedTrades
    
    frmSplash.SetStatus "Connecting to real-time trades feed..."
    DoEvents
    
    g_TradeChannel.InitMessaging

    If (iTimeRest <> 0) Then LogEvent EVENT_WARNING, "Database has not been updated."

    g_OrdersProcessor.InitMessaging

    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
    
    g_TradeChannel.LoadTrades frmSplash
    g_OrdersProcessor.LoadOrders frmSplash
    
    Load frmMain
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_LOADTRADESDATA, "EtsMM"
    
    g_TradeChannel.Start
    g_OrdersProcessor.Start
    
    Set g_ClipMgr = New clsClipMgr
    Set g_ScreenExport = New clsScreenExport
    
    If g_Params.LogAdvancedInfo Then
        frmMain.tmrPerformance.Interval = g_Params.LogPerfCountInterval
        frmMain.tmrPerformance.Enabled = g_Params.LogPerformanceCounters
    End If
    GetDefWindowSize g_ptDefWindowSize
    g_ptDefWindowSize.X = frmMain.ScaleX(g_ptDefWindowSize.X, vbPixels, vbTwips)
    g_ptDefWindowSize.Y = frmMain.ScaleY(g_ptDefWindowSize.Y, vbPixels, vbTwips)
    
    If g_lMainWidth > 0 Then
        frmMain.Left = g_lMainLeft
        frmMain.Top = g_lMainTop
        frmMain.Width = g_lMainWidth
    End If
    
    frmMain.Show
    Set g_frmOwner = frmMain
    Unload frmSplash
    
    g_frmMatchedTrades.StartMonitoring
    g_frmProjections.Init
    
    If g_Params.SOQ_CheckExpiredOptions Then
        Dim rs As ADODB.Recordset
        Set rs = gDBW.usp_MmExpiredPositions_Check_Simple(g_Params.SOQ_EADaysToDivDays)
        Dim strExp As String
        Dim dtExpary As Date
        dtExpary = DateSerial(1900, 0, 0)
        
        While Not rs.EOF
            If (dtExpary <> rs!expiryDate) Then
            
                dtExpary = rs!expiryDate
                If strExp <> "" Then
                    strExp = strExp + " , "
                End If
                Dim stdDt As String
                stdDt = Format$(dtExpary, "DD MMMM YYYY")
    
                strExp = strExp + stdDt
            End If
            rs.MoveNext
        Wend
        If strExp <> "" Then
            'MsgBox "You have Options with expirations passed in the system", vbSystemModal, "Warning"
            Dim Frm As New frmWarning
            Frm.lblWarning = "you have option positions in the system with past / upcoming expiration dates:  " + strExp
            Frm.Show
        End If
    End If
    
    'Check for new data from ivloader in period of ets is don't lunched
    If CLng(CDate(g_Params.LastStopDate)) <= CLng(CDate(g_Params.LastDataDate)) Then
        If TimeValue(g_Params.LastStopTime) <= TimeValue(g_Params.LastDataTime) Then
            LogEvent EVENT_INFO, "Dividends’ file has been uploaded. " & g_Params.LastDVFile
            LogEvent EVENT_INFO, "Interest file has been uploaded." & g_Params.LastIRFile
            LogEvent EVENT_INFO, "Volatilities’ file has been uploaded. " & g_Params.LastIVFile
            LogEvent EVENT_INFO, "Index components’ file has been uploaded. " & g_Params.LastIWFile
        End If
    End If
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'    CheckMarketStructureInCache frmMain
    
    'CV functionality
    Set g_cvContextInfo = New clsCVInfo
    g_cvContextInfo.Init
    If Not g_cvContextInfo.IsInit Then
        gCmn.MyMsgBox frmSplash, "Fail to initialize CV context! Application will be closed.", vbCritical
        ShutDown True
        Exit Sub
    End If
    
    If Len(Command$) > 0 Then frmMain.OpenFromFile Replace$(Command$, """", "")
        
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to initialize ETS. Application will be closed."
    ShutDown True
End Sub

Public Sub ShutDown(ByVal bEndExecution As Boolean)
    On Error Resume Next
    If m_bShutDownNow Then Exit Sub
    
    gDBW.usp_RegUserAction_Save 1, CurrentNtUserName, g_sComputerName, g_Params.UserGroup
    
    Dim aFrm As Form
    m_bShutDownNow = True
    
    For Each aFrm In Forms
        Unload aFrm
    Next
    Set g_ViewFrm = Nothing
    
    Set g_frmMatchedTrades = Nothing
    Set g_TradeChannel = Nothing
    Set g_OrdersProcessor = Nothing
    g_VolaSource.EnableEvents = False
    
    
    m_PriceProvider.Disconnect
    Set m_PriceProvider = Nothing
    
    Set g_frmLog = Nothing
    Set g_frmUnderlyings = Nothing
    Set g_frmEarlyExer = Nothing
    
    g_frmProjections.Term
    Set g_frmProjections = Nothing
    
    Set g_frmOwner = Nothing
    
    g_Params.SaveSettings
    g_Params.Terminate
    SaveSettings
    ClearInterestRates
    
    Set g_frmWtdVegaSettings = Nothing
    g_VolaSource.UnregisterPublisher
    Set g_VolaSource = Nothing
      
    g_Main.Clear
    
    g_RmScenario.Clear
    g_OrderExecDest.Clear
    g_CancelledOrderExecs.Clear
    
    ClearDefGrids
    
    Set g_cvContextInfo = Nothing
    
    Set gDBW = Nothing
    Set gCmn = Nothing
    Set g_ClipMgr = Nothing
    Set g_ScreenExport = Nothing
    Set g_PerformanceLog = Nothing
    Set g_Main = Nothing
    
   
    If Not g_Params.IsDebug And m_hSingleInstanceMutex <> 0 Then CloseHandle m_hSingleInstanceMutex
    'UninstallExceptionHandler
    
    If bEndExecution Then
        m_bShutDownNow = False
        End
    End If
    m_bShutDownNow = False
End Sub

Public Sub Restart()
    On Error Resume Next
    ShutDown False
    Main
End Sub

Private Sub InitEventLog()
    On Error Resume Next
    Set g_frmLog = Nothing
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
    g_frmLog.AdjustGridColWidths
End Sub

Private Sub LoadSettings(ByRef frmOwner As Form)
    On Error Resume Next
    Dim sUID$, sTmp$, nTmp&
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    Set g_aMainXMLParams = aXMLLoader.Load(MainXMLFilePath)
    If g_aMainXMLParams Is Nothing Then
        Set g_aMainXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    
    g_lcLicense.Modules = &H7FFFFFFF
    g_sLicenseKey = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "License", "Key", "")
    g_sLicenseCompany = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "License", "Company", "")
    g_sLicenseUser = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "License", "User", "")
    
    g_lSizeComputerName = 50
    g_sComputerName = ""
    Dim helpComputerName(0 To 50) As Byte
    If GetComputerName(helpComputerName(0), g_lSizeComputerName) = 0 Then
        gCmn.ErrorMsgBox g_frmOwner, "Fail to Get Computer Name. "
    End If
    
    Dim Counter As Integer
    Counter = 0
    
    Do While helpComputerName(Counter) > 0
        g_sComputerName = g_sComputerName + Chr(helpComputerName(Counter))
        Counter = Counter + 1
    Loop
          
    sTmp = String$(1024, 0)
    If GetWindowsDirectory(sTmp, 1024) <> 0 Then
        sTmp = Left$(sTmp, 3)
    Else
        sTmp = "C:\"
    End If
    
    nTmp = GetVolumeSerialNo(sTmp)
    If nTmp < 0 Then nTmp = -nTmp
    sUID = Hex$(nTmp)
    sUID = APP_LIC_NAME & " " & sUID & " " & g_sLicenseCompany & " " & g_sLicenseUser
    
    'g_lcLicense.UID = CalcCRC(sUID, frmOwner)
    
    Set g_aUserXMLParams = aXMLLoader.Load(CurrentUserXMLFilePath)
    If g_aUserXMLParams Is Nothing Then
        Set g_aUserXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    
    g_lMainTop = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainTop", 0)
    g_lMainLeft = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainLeft", 0)
    g_lMainWidth = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainWidth", 0)
    g_lMainHeight = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainHeight", 0)
    g_lMainState = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainState", vbNormal)
    
    If g_lMainTop < 0 Then g_lMainTop = 0
    If g_lMainLeft < 0 Then g_lMainLeft = 0
    
    nTmp = GetSystemMetrics(SM_CXFULLSCREEN) * 15
    If g_lMainWidth < 0 Then g_lMainWidth = 0
    If g_lMainWidth > nTmp Then g_lMainWidth = nTmp
    If g_lMainLeft > nTmp Then g_lMainLeft = nTmp - g_lMainWidth
    nTmp = GetSystemMetrics(SM_CYFULLSCREEN) * 15
    If g_lMainTop > nTmp Then g_lMainTop = nTmp - 1500
    
    g_lLogTop = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogTop", 0)
    g_lLogLeft = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogLeft", 0)
    g_lLogWidth = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogWidth", 0)
    g_lLogHeight = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "LogHeight", 0)
    
    m_bFirstRun = False ' (GetLongKeyValueEx(APP_REG_KEY & "Settings", "FirstRun", 1) <> 0)
    
    'g_CurTraderID = GetLongKeyValueEx(APP_REG_KEY & "Settings", "CurTrader")
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainTop", g_lMainTop
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainLeft", g_lMainLeft
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainWidth", g_lMainWidth
    'g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainHeight", g_lMainHeight
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainState", g_lMainState
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogTop", g_lLogTop
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogLeft", g_lLogLeft
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogWidth", g_lLogWidth
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "LogHeight", g_lLogHeight
    
    If m_bFirstRun Then g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "FirstRun", 0
    'SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "CurTrader", g_CurTraderID
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    aXMLLoader.Save CurrentUserXMLFilePath, g_aUserXMLParams
    aXMLLoader.Save MainXMLFilePath, g_aMainXMLParams
End Sub

Public Sub LoadEntities(Optional ByVal enType As EntityTypeEnum = ET_ALL, Optional ByRef Progress As frmSplash = Nothing)
    On Error GoTo EH
    Dim dKOptDeltaEq As Double
    dKOptDeltaEq = 1#
    Dim rs As ADODB.Recordset, rs2 As ADODB.Recordset, aScn As clsRmScenarioAtom, bAdd As Boolean
    Dim aExpMonth As EtsGeneralLib.EtsExpMonthAtom, nExpiryMonth&
    Dim aUnd As EtsGeneralLib.UndAtom, aIdx As EtsGeneralLib.IndexAtom, aIdxComp As EtsGeneralLib.IndexCompAtom, aExch As EtsGeneralLib.ExchAtom
    Dim nID&, dtExpiry As Date, sExpiryKey$, nIdxID&, sStep$, nUndID&, nFutRootID&, nFutID&, nSynthUndID&
    Dim aSyntRoot As EtsGeneralLib.SynthRootAtom
    Dim aSyntRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim dWeight#, dBeta#, aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Dim aRoot As EtsGeneralLib.EtsOptRootAtom, nOptRootID&
    Dim aExecDest As EtsMmGeneralLib.MmOrderExecDestAtom
    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    Dim aTrdUnd As EtsGeneralLib.EtsTraderUndAtom
    Dim aFutRoot As EtsGeneralLib.EtsFutRootAtom
    Dim aFut As EtsGeneralLib.EtsFutAtom
    Dim aContract As EtsGeneralLib.EtsContractAtom
    
    Dim rsCust As ADODB.Recordset
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aCustDivColl As EtsGeneralLib.EtsDivColl
    Dim aUndDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim dDate As Date
    Dim dAmount As Double

    g_Main.DatabaseString = g_Params.DbConnection
    
    If (enType And ET_STRATEGY) = ET_STRATEGY Then
        sStep = "strategies data."
        g_Main.LoadStrategy
    End If
       
    
    If (enType And ET_BROKER) = ET_BROKER Then
        sStep = "brokers data."
        g_Main.LoadBroker
    End If
    
    If (enType And ET_PRICE_PROFILE) = ET_PRICE_PROFILE Then
        sStep = "price profiles."
        g_PriceProfile.Clear
        gDBW.usp_PriceProfile_CheckDefaults
        g_Main.LoadPriceProfile
    End If
    
    If (enType And ET_EXP_CALENDAR) = ET_EXP_CALENDAR Then
    
        Dim dtNow As Date
        dtNow = GetNewYorkTime
        
        sStep = "expiry calendars data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading Vega Weight data..."
        g_ExpCalendar.Clear
        
        Set rs = gDBW.usp_WtdVegaMaturity_Get()
        If Not rs.EOF Then
            g_ExpCalendar.Maturity = ReadLng(rs!iWtdVegaMaturity)
            If g_ExpCalendar.Maturity <= 0 Then g_ExpCalendar.Maturity = 30
        Else
            g_ExpCalendar.Maturity = 30
        End If
        
        g_Main.CalculationSettings.Normal = g_ExpCalendar.Maturity
        
        Set rs = Nothing
        
        Set rs = gDBW.usp_MmVegaWeight_Get
        While Not rs.EOF
            dtExpiry = ReadDate(rs!dtExpiryOV)
            
            If dtExpiry > Date Then
                If g_ExpCalendar(CLng(dtExpiry)) Is Nothing Then
                
                    Set aExpMonth = New EtsGeneralLib.EtsExpMonthAtom
                    aExpMonth.ID = nID
                    aExpMonth.Expiry = dtExpiry
                    aExpMonth.VegaWeight = ReadDbl(rs!fVegaWeight)
                    aExpMonth.IsDirty = False
                    
                    If aExpMonth.VegaWeight <= 0# Then
                        aExpMonth.VegaWeight = Sqr(g_ExpCalendar.Maturity / Abs(DateDiff("d", dtExpiry, dtNow)))
                    End If
                    
                    g_ExpCalendar.Add CLng(dtExpiry), aExpMonth
                    Set aExpMonth = Nothing
                End If
            End If
            
            rs.MoveNext
        Wend
        Set rs = Nothing
    End If
    
    If (enType And ET_UNDERLYING) = ET_UNDERLYING Then
        sStep = "underlyings data."
        g_Main.LoadUnderlying g_CurTraderID

        sStep = "option roots data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading option roots data..."
        g_Main.LoadOptionRoots
    
        
        Set rs = gDBW.usp_MmSyntheticRootParams_Get(0)
        While Not rs.EOF
            nUndID = ReadLng(rs!iUnderlyingID)
            Set aUnd = g_UnderlyingAll(nUndID)
            If Not aUnd Is Nothing Then
                nOptRootID = ReadLng(rs!iOptionRootID)
                
                Set aSyntRoot = aUnd.SyntheticRoots(nOptRootID)
                If Not aSyntRoot Is Nothing Then
                    nSynthUndID = ReadLng(rs!iComponentUnderlyingID)
                    Set aSyntRootComp = aUnd.SyntheticRoots(nSynthUndID)
                    
                    If aSyntRootComp Is Nothing Then
                        Set aSyntRootComp = New EtsGeneralLib.SynthRootCompAtom
                    
                        aSyntRootComp.UndID = nSynthUndID
                        aSyntRootComp.Weight = ReadDbl(rs!fWeight)
                        aSyntRootComp.Settlement = ReadDbl(rs!fStrikeSettlement)
                        
                        aSyntRoot.SynthRootComponents.Add aSyntRootComp.UndID, aSyntRootComp
                        
                        Set aSyntRootComp = Nothing
                    End If
                    
                    Set aSyntRoot = Nothing
                End If
                
                Set aUnd = Nothing
            End If
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
    
        sStep = "futures roots data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading futures roots data..."
    
        Set rs = gDBW.usp_MmFutureRoot_Get(0)
        While Not rs.EOF
            nUndID = ReadLng(rs!iUnderlyingID)
            Set aUnd = g_UnderlyingAll(nUndID)
            If Not aUnd Is Nothing Then
                nID = ReadLng(rs!iFutureRootID)
                
                If aUnd.FutRoots(nID) Is Nothing Then
                    Set aFutRoot = New EtsGeneralLib.EtsFutRootAtom
                    
                    aFutRoot.ID = nID
                    aFutRoot.Symbol = ReadStr(rs!vcFutureRootSymbol)
                    aFutRoot.Name = ReadStr(rs!vcFutureRootName)
                    aFutRoot.UndID = nUndID
                    aFutRoot.QuotationUnitID = ReadLng(rs!iQuotationUnitID)
                    aFutRoot.TickSize = ReadDbl(rs!fTickSize)
                    aFutRoot.TickValue = ReadDbl(rs!fTickValue)
                    aFutRoot.PriceQuotationUnit = ReadLng(rs!iQuotationUnitAmmount)
                    aFutRoot.QuoteFormat = ReadLng(rs!tiQuoteFormat)
                    
                    If aFutRoot.QuotationUnitID = 40 Then   ' VK: '40' - whtat da heck this means?
                        aFutRoot.QuotationUnitName = ReadStr(rs!vcQuotationUnitName)
                    Else
                        aFutRoot.QuotationUnitName = GetQuotationNameByID(aFutRoot.QuotationUnitID)
                    End If
                    
                    aFutRoot.FutLotSize = ReadLng(rs!iFutureLotSize)
                    aFutRoot.OptLotSize = ReadLng(rs!iOptionLotSize)
                    
                    aUnd.FutRoots.Add aFutRoot.ID, aFutRoot.Symbol, aFutRoot
                    Set aFutRoot = Nothing
                End If
                
                Set aUnd = Nothing
            End If
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
    
        sStep = "futures data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading futures data..."
        
        Dim future_contract As IContract
        
        Set rs = gDBW.usp_MmFuture_Get(0)
        While Not rs.EOF
            nUndID = ReadLng(rs!iUnderlyingID)
            Set aUnd = g_UnderlyingAll(nUndID)
            If Not aUnd Is Nothing Then
                nFutRootID = ReadLng(rs!iFutureRootID)
                Set aFutRoot = aUnd.FutRoots(nFutRootID)

                If Not aFutRoot Is Nothing Then
                    nFutID = ReadLng(rs!iFutureID)

                    If aFutRoot.Futures(nFutID) Is Nothing Then
                        Set aFut = New EtsGeneralLib.EtsFutAtom
                                        
                        Set future_contract = g_Main.GetContract(nFutID)
                        
                        aFut.ID = nFutID
                        aFut.Symbol = ReadStr(rs!vcFutureSymbol)
                        aFut.ContractName = ReadStr(rs!vcFutureName)
                        aFut.FutRootID = nFutRootID
                        aFut.FutureBasis = ReadDbl(rs!dBasis)
                        aFut.UseInCalc = IIf(rs!bUseInCalculation, True, False)
                        aFut.ActiveFutureRatio = ReadDbl(rs!dActiveRatio)
                        aFut.ShowInFutureContractEq = IIf(ReadLng(rs!iShowInFutureContractEq), True, False)
                        
                        aFut.FutureBasis = ReadDbl(rs!dBasis)
                        aFut.UseInCalc = IIf(rs!bUseInCalculation, True, False)
                        aFut.ActiveFutureRatio = ReadDbl(rs!dActiveRatio)
                        
                        If aFut.UseInCalc Then
                            Set aUnd.ActiveFuture = aFut
                        End If
                        
                        If aFutRoot.PriceQuotationUnit <> 0 And aFutRoot.TickSize <> 0 Then
                            dKOptDeltaEq = aFutRoot.TickValue / aFutRoot.TickSize * (1 / (aFutRoot.PriceQuotationUnit * IIf(aFutRoot.QuoteFormat = 1, 100, 1)))
                        Else
                            dKOptDeltaEq = 1#
                        End If
                        aFut.KEq = dKOptDeltaEq
                        
                        If Not aFut.ShowInFutureContractEq And aFutRoot.PriceQuotationUnit <> 0 And aFutRoot.TickSize <> 0 Then
                           aFut.K = dKOptDeltaEq
                           aFut.MultOptDltEq = False
                        Else
                           aFut.MultOptDltEq = True
                        End If
                        
                        If aFut.UseInCalc Then
                            Set aUnd.ActiveFuture = aFut
                        End If
                        
                        aFut.MaturityDate = ReadDate(rs!dtMaturityDate)
                        aFut.IsAmerican = (ReadByte(rs!tiCalcOptionType) <> 0)
                        aFut.IsActive = (ReadByte(rs!tiIsActive) <> 0)
                        aFut.PriceClose = ReadDbl(rs!fPriceClose)
                        aFut.PriceTheoclose = ReadDbl(rs!fPriceTheoClose)
                        aFut.HaveOptions = (ReadLng(rs!iHaveOptions) <> 0)
                        aFut.manualActivePrice = ReadDbl(rs!fManualActivePrice)
                        
                        If aFut.manualActivePrice <> 0 Then
                            aFut.manualActivePrice = aFut.manualActivePrice
                        End If

                        Set aFut.UndPriceProfile = g_PriceProfile(ReadLng(rs!iUndPriceProfileID))
                        Set aFut.OptPriceProfile = g_PriceProfile(ReadLng(rs!iOptPriceProfileID))

                        If aFut.UndPriceProfile Is Nothing Then Set aFut.UndPriceProfile = g_Main.DefStkPriceProfile
                        If aFut.OptPriceProfile Is Nothing Then Set aFut.OptPriceProfile = g_Main.DefOptPriceProfile

                        Debug.Assert Not aFut.UndPriceProfile Is Nothing And Not aFut.OptPriceProfile Is Nothing

                        If Not aUnd.HaveFutures Then aUnd.HaveFutures = True

                        aFutRoot.Futures.Add aFut.ID, aFut.Symbol, aFut

                        If g_ContractAll(aFut.ID) Is Nothing Then
                            Set aContract = New EtsGeneralLib.EtsContractAtom
                            Set aContract.Und = aUnd
                            Set aContract.Fut = aFut
                            Set aContract.FutRoot = aFutRoot
                                                                                  
                            'don't load future contracts fokiny
                            g_ContractAll.Add aContract.Fut.ID, aContract.Fut.Symbol, aContract
                            'g_ContractAllBySym.Add aContract.Fut.Symbol, aContract

                            If aUnd.IsTraderContract Then g_Contract.Add aContract.Fut.ID, aContract.Fut.Symbol, aContract

                            Set aContract = Nothing
                        End If

                        Set aFut = Nothing
                    End If

                    Set aFutRoot = Nothing
                End If

                Set aUnd = Nothing
            End If
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
    End If
    
    If (enType And ET_TRADER_GROUP) = ET_TRADER_GROUP Then
        sStep = "trader groups data."
        'If Not Progress Is Nothing Then Progress.SetStatus "Loading trader groups data..."
        g_Main.LoadTraderGroup
    End If
    
    If (enType And ET_TRADER) = ET_TRADER Then
        sStep = "traders data."
        'If Not Progress Is Nothing Then Progress.SetStatus "Loading traders data..."
        g_Main.LoadTrader g_CurTraderID
    End If
    
    If (enType And ET_UNDERLYING_GROUP) = ET_UNDERLYING_GROUP Then
        g_Main.LoadGroups g_CurTraderID
    End If
    
    If (enType And ET_RM_SCENARIO) = ET_RM_SCENARIO Then
        sStep = "risk matrix scenarios data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading risk matrix scenarios data..."
        LoadScenariosFromDB
    End If
    
    If (enType And ET_INDEX) = ET_INDEX Then
        sStep = "indices data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading indices data..."
        g_Index.Clear
        g_BasketIndex.Clear
        g_HedgeSymbols.Clear
        Set rs = gDBW.usp_MmIndex_Get()
        While Not rs.EOF
            nID = ReadLng(rs!iContractID)
            If g_Index(nID) Is Nothing Then
                Set aIdx = New EtsGeneralLib.IndexAtom
                aIdx.ID = nID
                aIdx.Symbol = ReadStr(rs!vcSymbol)
                aIdx.IsBasket = True '(ReadByte(rs!tiIsBasket) <> 0)
                Set aIdx.UndPriceProfile = g_UnderlyingAll(nID).UndPriceProfile
                Set aIdx.OptPriceProfile = g_UnderlyingAll(nID).OptPriceProfile
                g_Index.Add aIdx.ID, aIdx.Symbol, aIdx
                Set aIdx = Nothing
            End If
            rs.MoveNext
        Wend
        Set rs = Nothing
    
        sStep = "index components data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading index components data..."
        Set rs = gDBW.usp_MmIndexDefinition_Get()
        While Not rs.EOF
            dWeight = ReadDbl(rs!fWeight)
            
            If dWeight > 0# Then
                nIdxID = ReadLng(rs!iIndexID)
                nID = ReadLng(rs!iStockID)
                Set aIdx = g_Index(nIdxID)
                If Not aIdx Is Nothing Then
                
                    Set aIdxComp = aIdx.Components(nID)
                    If aIdxComp Is Nothing Then
                        Set aIdxComp = aIdx.Components.Add(nID)
                        aIdxComp.ID = nID
                        aIdxComp.Weight = dWeight
                        aIdxComp.IsBasketComponent = True
                    End If
                    
                    If aIdx.IsBasket And g_BasketIndex(aIdx.ID) Is Nothing Then
                        g_BasketIndex.Add aIdx.ID, aIdx.Symbol, aIdx
                        Set aUnd = Nothing
                    End If
                        
                    Set aIdxComp = Nothing
                    Set aIdx = Nothing
                End If
            End If
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
        
        sStep = "index underlyings beta."
        g_Main.LoadBetas
                
        For Each aIdx In g_Index
            If (aIdx.IsBasket) Then
                aIdx.InitBasketDivs g_UnderlyingAll
            End If
        Next
    End If
    
    ' init synthetic roots dividends
    If (enType And ET_UNDERLYING) = ET_UNDERLYING Then
        sStep = "synthetic roots dividends "
        If Not Progress Is Nothing Then Progress.SetStatus "Initialize synthetic roots dividends..."
        For Each aUnd In g_UnderlyingAll
            If aUnd.HaveSyntheticRoots Then
                If Not aUnd.SyntheticRoots Is Nothing Then
                    For Each aSyntRoot In aUnd.SyntheticRoots
                        aSyntRoot.InitBasketDivs g_UnderlyingAll, g_Index
                        DoEvents
                    Next
                End If
            End If
            DoEvents
        Next
    End If
    
    If (enType And ET_EXCHANGE) = ET_EXCHANGE Then
        sStep = "exchanges data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading exchanges data..."
        g_Exch.Clear
        Set aExch = New EtsGeneralLib.ExchAtom
        aExch.ID = 0
        aExch.Code = ""
        aExch.Name = "<NBBO>"
        aExch.IsUnderlying = 1
        aExch.IsOption = 1
        g_Exch.Add aExch.ID, aExch.Code, aExch
        Set aExch = Nothing
        
        Set rs = gDBW.usp_Exchange_Get(Null)
        While Not rs.EOF
            nID = ReadLng(rs!iExchangeId)
            Set aExch = g_Exch(nID)
            If aExch Is Nothing Then
                Set aExch = New EtsGeneralLib.ExchAtom
                aExch.ID = nID
                aExch.Name = ReadStr(rs!vcExchangeName)
                aExch.IsUnderlying = ReadByte(rs!tiIsUnderlying)
                aExch.IsOption = ReadByte(rs!tiIsOption)
                aExch.Code = ReadStr(rs!vcExchangeCode)
                g_Exch.Add aExch.ID, aExch.Code, aExch
            End If
            Set aExch = Nothing
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
    End If
    
    If (enType And ET_ORDER_EXEC_DEST) = ET_ORDER_EXEC_DEST Then
        sStep = "execution destinations data."
        If Not Progress Is Nothing Then Progress.SetStatus "Loading execution destinations data..."
        g_OrderExecDest.Clear

        ' add default destination
        Set aExecDest = New EtsMmGeneralLib.MmOrderExecDestAtom
        aExecDest.ID = 0
        aExecDest.Code = ""
        aExecDest.Name = "<Default>"
        aExecDest.IsOption = True
        aExecDest.IsStock = True
        g_OrderExecDest.Add aExecDest.ID, aExecDest.Code, aExecDest
        Set aExecDest = Nothing

        ' load destinations
        Set rs = gDBW.usp_OrderExecDest_Get(0)
        While Not rs.EOF
            nID = ReadLng(rs!iDestID)
            Set aExecDest = g_OrderExecDest(nID)
            If aExecDest Is Nothing Then
                Set aExecDest = New EtsMmGeneralLib.MmOrderExecDestAtom
                aExecDest.ID = nID
                aExecDest.Code = ReadStr(rs!vcCode)
                aExecDest.Name = ReadStr(rs!vcName)
                aExecDest.IsOption = ReadByte(rs!tiIsOption)
                aExecDest.IsStock = ReadByte(rs!tiIsStock)
                g_OrderExecDest.Add aExecDest.ID, aExecDest.Code, aExecDest
            End If
            Set aExecDest = Nothing
            rs.MoveNext
            DoEvents
        Wend
        Set rs = Nothing
    End If
    
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    If sStep = "" Then sStep = "common data."
    sSource = "Fail to load " & sStep & vbCrLf & Err.Source
    
    Set aScn = Nothing
    Set aExpMonth = Nothing
    Set aUnd = Nothing
    Set aIdx = Nothing
    Set aIdxComp = Nothing
    Set aExch = Nothing
    Set aSyntRoot = Nothing
    Set aSyntRootComp = Nothing
    Set aPrProf = Nothing
    Set aRoot = Nothing
    Set aExecDest = Nothing
    Set aUndGroup = Nothing
    Set aTrdUnd = Nothing
    Set aFutRoot = Nothing
    Set aFut = Nothing
    Set aContract = Nothing
    
    If (enType And ET_TRADER) = ET_TRADER Then
        g_Trader.Clear
    End If
    
    If (enType And ET_TRADER_GROUP) = ET_TRADER_GROUP Then
        g_TraderGroup.Clear
    End If
    
    If (enType And ET_STRATEGY) = ET_STRATEGY Then
        g_Strategy.Clear
    End If
    
    If (enType And ET_BROKER) = ET_BROKER Then
        g_Broker.Clear
        g_ClearingBroker.Clear
    End If
    
    If (enType And ET_UNDERLYING) = ET_UNDERLYING Then
        g_Underlying.Clear
        g_UnderlyingAll.Clear
        g_Contract.Clear
        g_ContractAll.Clear
        g_OptRootByName.Clear
        g_Stock.Clear
    End If
    
    If (enType And ET_UNDERLYING_GROUP) = ET_UNDERLYING_GROUP Then
        g_UnderlyingGroup.Clear
    End If
    
    If (enType And ET_RM_SCENARIO) = ET_RM_SCENARIO Then
        g_RmScenario.Clear
    End If
    
    If (enType And ET_EXP_CALENDAR) = ET_EXP_CALENDAR Then
        g_ExpCalendar.Clear
    End If

    If (enType And ET_INDEX) = ET_INDEX Then
        g_Index.Clear
        g_BasketIndex.Clear
    End If
    
    If (enType And ET_EXCHANGE) = ET_EXCHANGE Then
        g_Exch.Clear
    End If
    
    If (enType And ET_PRICE_PROFILE) = ET_PRICE_PROFILE Then
        g_PriceProfile.Clear
    End If
    
    If (enType And ET_ORDER_EXEC_DEST) = ET_ORDER_EXEC_DEST Then
        g_OrderExecDest.Clear
    End If
   
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Private Sub CheckPriceProfileDefaults(ByRef aPrProf As EtsGeneralLib.EtsPriceProfileAtom)
    On Error Resume Next
    If aPrProf.IsOptionProfile Then
        Select Case aPrProf.DefaultID
            Case 1
                aPrProf.UseMidMarketForPnL = True
                
            Case 2
                aPrProf.UseMidMarketForPnL = False
                
        End Select
    Else
        Select Case aPrProf.DefaultID
            Case 1
                aPrProf.AlwaysUseLast = False
                aPrProf.UseMidMarketForPnL = True
            
            Case 2
                aPrProf.AlwaysUseLast = False
                aPrProf.UseMidMarketForPnL = False
            
            Case 3
                aPrProf.AlwaysUseLast = True
                aPrProf.UseMidMarketForPnL = False
                aPrProf.BadUndPriceRule = enUbprDoNotCheckTolerance
        End Select
    End If
End Sub

Private Sub InitVolaSources(ByVal bFirstRun As Boolean)
    On Error GoTo EH
    Dim bRetry As Boolean, sName$, sFileName$, sVersion$, nID&, sProgID$
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
    
    Set g_Main.VolatilitySource = g_VolaSource
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to initialize volatility source."
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source
    
'    If bRetry Then
'        If Not bFirstRun Then gCmn.ErrorMsgBox g_frmOwner, sDescription
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
    Set vmDB = Nothing
    Set aVolaSources = Nothing
    Set g_VolaSource = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

'Private Function CheckLicense(ByRef frmOwner As Form, ByRef bNewLicense As Boolean) As Boolean
'    On Error Resume Next
'    Dim bSucceed As Boolean: bSucceed = False
'    Dim bExpired As Boolean: bExpired = False
'    Dim bEnterNewLicense As Boolean: bEnterNewLicense = False
'    Dim lcLicense As LicenseType, sKey$, nDays
'    sKey = g_sLicenseKey
'    bNewLicense = False
'    Do While True
'        bExpired = False
'        If sKey <> "" Then
'            If GetLicenseByKey(sKey, lcLicense) Then
'                If lcLicense.UID = g_lcLicense.UID _
'                    And lcLicense.Modules = g_lcLicense.Modules Then
'
'                    nDays = DateDiff("d", Date, DateSerial(lcLicense.Year, lcLicense.Month, lcLicense.Day))
'
'                    If nDays > 0 Then
'                        bSucceed = True
'                        g_lcLicense.Year = lcLicense.Year
'                        g_lcLicense.Month = lcLicense.Month
'                        g_lcLicense.Day = lcLicense.Day
'                    End If
'
'                    If bSucceed Then
'                        If nDays <= 7 Then
'                            If gCmn.MyMsgBox(frmOwner, "Your ETS license is expired in " & nDays & " day(s). " & vbCrLf & _
'                                                "Do you wish to enter new license key?", vbQuestion + vbYesNo) = vbYes Then
'                                bEnterNewLicense = True
'                            Else
'                                Exit Do
'                            End If
'                        Else
'                            Exit Do
'                        End If
'                    Else
'                        bExpired = True
'                    End If
'
'                End If
'            End If
'        End If
'
'        If bExpired Then
'            gCmn.MyMsgBox frmOwner, "Your ETS license is expired! Please contact technical support at 1-212-223-3552.", vbCritical
'
'        ElseIf Not bEnterNewLicense Then
'            gCmn.MyMsgBox frmOwner, "ETS is not licensed, or you have invalid license key! Please contact technical support at 1-212-223-3552.", vbCritical
'        End If
'
'        bNewLicense = True
'        lcLicense = g_lcLicense
'        If frmLicense.Execute(sKey, g_lcLicense.UID, frmOwner) <> vbOK Then
'            Exit Do
'        End If
'    Loop
'
'    If bSucceed Then
'        If g_sLicenseKey <> sKey Then
'            g_sLicenseKey = sKey
'            g_lcLicense = lcLicense
'            SetStrKeyValue HKEY_CURRENT_USER, PRODUCT_REG_KEY & "License", "Key", g_sLicenseKey
'        End If
'    Else
'        gCmn.MyMsgBox frmOwner, "ETS is not licensed! Please contact technical support at 1-212-223-3552.", vbCritical
'    End If
'    CheckLicense = bSucceed
'End Function
'
'Private Function IsIseProviderPresent() As Boolean
'    Dim MmManager As ISEPROVIDERLib.ISEMMManager
'    On Error GoTo EH
'
'    Set MmManager = New ISEPROVIDERLib.ISEMMManager
'    MmManager.Connect
'    MmManager.Disconnect
'    Set MmManager = Nothing
'    IsIseProviderPresent = True
'    Exit Function
'EH:
'    Set MmManager = Nothing
'    IsIseProviderPresent = False
'End Function
'
'Private Sub CheckMarketStructureInCache(frmOwner As VB.Form)
'    On Error GoTo EH
'    Dim rs As ADODB.Recordset, sMsg$, nCount&
'
'    Set rs = gDBW.usp_EodMsUnderlyingCache_Get
'    If Not rs.EOF Then
'        sMsg = "You need to finish market structure update for some underlyings." & vbCrLf & _
'               "Please do it in Back Office after reconciling positions or erasing deleted trades." & vbCrLf & _
'               "(Symbol(s): "
'
'        nCount = 0
'        While Not rs.EOF And nCount < 99
'            sMsg = sMsg & " " & ReadStr(rs!vcUndSymbol) & ","
'            nCount = nCount + 1
'            If nCount Mod 10 = 0 Then sMsg = sMsg & vbCrLf
'            rs.MoveNext
'        Wend
'
'        If rs.EOF Then
'            sMsg = Left$(sMsg, Len(sMsg) - 1) & ")"
'        Else
'            sMsg = Left$(sMsg, Len(sMsg) - 1) & "...)"
'        End If
'
'        gCmn.MyMsgBox frmOwner, sMsg
'    End If
'
'    Set rs = Nothing
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox frmOwner, "Fail to check market structure updates."
'    Set rs = Nothing
'End Sub
'
'Private Sub DeleteOldOrders()
'    On Error GoTo EH
'    gDBW.usp_FixOldOrder_Del
'    Exit Sub
'EH:
'    LogEvent EVENT_ERROR, "Fail to delete old orders." & Err.Description
'End Sub

Private Sub SetCrashHandler()
    On Error Resume Next
    SetMiniDumpDefaultCrashHandler
End Sub

Public Sub InitPriceProvider()
    On Error GoTo EH
    Dim aPT As PRICEPROVIDERSLib.IProvider, bRetry As Boolean
    Dim aProviderData As PRICEPROVIDERSLib.IProviderData
    Dim aProviders As PRICEPROVIDERSLib.Providers
    Dim aBatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
    
    bRetry = False
    
    Set aProviders = New PRICEPROVIDERSLib.Providers
    aProviders.Initialize
    
    On Error Resume Next
    aProviders.GetProvider g_Params.PriceProviderType, aProviderData
    g_Params.PriceProviderProgID = aProviderData.ProgId(enPriceInfo)
    
    bRetry = True

Retry:
    On Error GoTo EH
    
    If m_PriceProvider Is Nothing Then Set m_PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    If aPT Is Nothing Then Set aPT = m_PriceProvider
    
    aPT.Type = g_Params.PriceProviderType
    m_PriceProvider.Connect
    
    'Set CanadianRegion USD/CAD rate
    Set aBatchPriceProvider = m_PriceProvider
    If (Not aBatchPriceProvider Is Nothing) Then
        Dim dRate As Double
        dRate = g_Params.ExchangeRate
        aBatchPriceProvider.SetRegionRate CAN_REGION, CDbl(dRate)
        Set aBatchPriceProvider = Nothing
    End If
    
    Set aPT = Nothing
    
    Set aProviderData = Nothing
    aProviders.GetProvider g_Params.PriceProviderType, aProviderData
    If Not aProviderData Is Nothing Then
        g_Params.PriceProviderIsGroupRequestSupported = aProviderData.IsGroupRequestSupported
    Else
        g_Params.PriceProviderIsGroupRequestSupported = False
    End If
    
    Set aProviderData = Nothing
    Set aProviders = Nothing
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to initialize price provider. " & Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source
    
'    If bRetry Then
'        If Not m_bFirstRun Then gCmn.ErrorMsgBox g_frmOwner, sDescription
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
'            Set frmConnect = Nothing
'            Set aPT = Nothing
'            Set m_PriceProvider = Nothing
'            Set aProviderData = Nothing
'            Set aProviders = Nothing
'            Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'        End If
'    End If
    
    Set aPT = Nothing
    Set m_PriceProvider = Nothing
    Set aProviderData = Nothing
    Set aProviders = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Private Sub CreateGlobalCollections()
    On Error GoTo EH
    Set g_Main = New EtsMain
        
    Set g_Trader = g_Main.Trader
    Set g_TraderGroup = g_Main.TraderGroup
    Set g_Strategy = g_Main.Strategy
    Set g_Broker = g_Main.Broker
    Set g_ClearingBroker = g_Main.ClearingBroker
    Set g_Underlying = g_Main.Underlying
    Set g_UnderlyingAll = g_Main.UnderlyingAll
    Set g_UnderlyingGroup = g_Main.UnderlyingGroup
    Set g_Stock = g_Main.Stock
    Set g_PriceProfile = g_Main.PriceProfile
    Set g_ContractAll = g_Main.ContractAll
    Set g_Contract = g_Main.Contract
    Set g_ExpCalendar = g_Main.ExpCalendar
    Set g_OptRootByName = g_Main.OptRootByName
    Set g_Exch = g_Main.Exch
    Set g_Index = g_Main.Index
    
    Set g_RmScenario = New clsRmScenarioColl
    Set g_BasketIndex = g_Main.BasketIndex
    Set g_HedgeSymbols = g_Main.HedgeSymbols
    
'    Set g_Exch = New EtsGeneralLib.ExchColl
    Set g_OrderExecDest = New EtsMmGeneralLib.MmOrderExecDestColl
    Set g_CancelledOrderExecs = New EtsGeneralLib.MmTradeInfoColl
'    Set g_TntCntPty = New EtsMmGeneralLib.MmTntCounterPartyColl
'    Set g_TntCntPtyByName = New EtsMmGeneralLib.MmTntCounterPartyByNameColl
    
    'Set g_ContractAllBySym = New EtsGeneralLib.EtsContractBySymColl
    'Set g_StockBySym = New EtsGeneralLib.UndBySymColl
    'Set g_BasketIndex = New EtsGeneralLib.IndexColl
    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to init internal storage"
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
        gCmn.MyMsgBox frmOwner, "Can not validate ETS subscription status!" & vbCrLf & sBadStatusDesc & vbCrLf & _
                            "Please contact IVolatility.com support team.", vbCritical
    End If
    
    CheckAspLicense = bSucceeded
End Function

Public Function CheckEODUpdate(ByRef iTimeRest As Long, ByVal bNeedToAsk As Boolean) As Boolean
    Dim EODProcess As String, sDate As String, EODProcessTime As String, sTime As String
    Dim rsEOD As ADODB.Recordset
    Dim Answer As VbMsgBoxStyle
    Dim iDaysFromUpdate As Long

    On Error Resume Next

    iTimeRest = 0
    CheckEODUpdate = True
    Set rsEOD = gDBW.usp_DataInfo_Get(, "EODLastDate") 'Field is updated by EOD Job
    If Not rsEOD Is Nothing Then
        If Not rsEOD.EOF Then
            EODProcess = Format$(ReadStr(rsEOD!vcKeyValue), "YYYY-MM-DD")
            sDate = Format$(Date, "YYYY-MM-DD")
            iDaysFromUpdate = 60 - DateDiff("s", EODProcessTime, sTime) / 60
            If Weekday(sDate) <> 1 And Weekday(sDate) <> 7 And _
               StrComp(sDate, EODProcess, vbTextCompare) <> 0 Then
                'An average time to make database update is 30-40 minutes.
                EODProcessTime = Format$(ReadStr(rsEOD!vcKeyValue), "Long Time")
                sTime = Format$(Now, "Long Time")
                iTimeRest = 60 - DateDiff("s", EODProcessTime, sTime) / 60
                g_bOldData = True
                
                iDaysFromUpdate = DateDiff("d", EODProcess, sDate)
                If bNeedToAsk Then
                    If iDaysFromUpdate > 1 Or iTimeRest < 0 Or iTimeRest > 60 Then 'Last update time is yeterday or earlier.
'                        iTimeRest = 2 'Hope to finish it in a couple of minutes...
                        EODProcess = "The database was not updated with the latest market and analytical information." & vbCrLf & _
                                     "Please contact support." & vbCrLf & vbCrLf & _
                                     "Press 'Yes' to continue without update or press 'No' to exit. "
                        Answer = gCmn.MyMsgBox(frmMsgBox, EODProcess, vbYesNo, "Database has not been updated.")
                    Else
                        EODProcess = "Application performance might be decreased. " & vbCrLf & vbCrLf & _
                            "Estimated time for update - " + Str(iTimeRest) + " minutes. " & vbCrLf & vbCrLf & _
                            "Would you like to continue? "
                        Answer = gCmn.MyMsgBox(frmMsgBox, EODProcess, vbYesNo, "Database is being updated...")
                    End If

                    If Answer = vbNo Then
                        CheckEODUpdate = False
                        Set rsEOD = Nothing
                    End If
                End If
            Else: CheckEODUpdate = True
            End If
        End If
        Set rsEOD = Nothing
    End If
End Function

Public Sub LoadScenariosFromDB()
    Dim rs As ADODB.Recordset, nID As Long
    Dim aScn As clsRmScenarioAtom
    g_RmScenario.Clear
    Set aScn = Nothing
    Set rs = gDBW.usp_RMScenario_Get(Null)
    While Not rs.EOF
        nID = ReadLng(rs!iRMScenarioID)
        Set aScn = g_RmScenario(CStr(nID))
        If aScn Is Nothing Then
            Set aScn = g_RmScenario.Add(CStr(nID))
            aScn.ID = nID
            
            aScn.ScenarioName = ReadStr(rs!vcScenarioName)
            aScn.Description = ReadStr(rs!vcDescription)

            aScn.Axis(RMA_HORZ) = ReadByte(rs!tiType1)
            aScn.Points(RMA_HORZ) = ReadLng(rs!iPoints1)
            aScn.Step(RMA_HORZ) = ReadDbl(rs!fStep1)
            aScn.Units(RMA_HORZ) = ReadByte(rs!tiIsAbs1)
            aScn.Hour(RMA_HORZ) = ReadLng(rs!iHours)
            aScn.Minute(RMA_HORZ) = ReadLng(rs!iMinutes)

            aScn.Axis(RMA_VERT) = ReadByte(rs!tiType2)
            aScn.Points(RMA_VERT) = ReadLng(rs!iPoints2)
            aScn.Step(RMA_VERT) = ReadDbl(rs!fStep2)
            aScn.Units(RMA_VERT) = ReadByte(rs!tiIsAbs2)
            aScn.Hour(RMA_VERT) = ReadLng(rs!iHours)
            aScn.Minute(RMA_VERT) = ReadLng(rs!iMinutes)
            
            aScn.ShowChange = IIf(ReadLng(rs!iShowChange) <> 0, True, False)
            
            aScn.SpotShiftType = ReadLng(rs!iSpotShift)
            aScn.SpotSpecificShiftType = ReadLng(rs!iSpotSpecificShift)
            
            aScn.VolaShiftType = ReadByte(rs!tiVolaShift)
            aScn.VolaSpecificShiftType = ReadLng(rs!iVolaSpecificShift)
            
            aScn.CalcModel = ReadLng(rs!iCalcModel)
            
            If (aScn.CalcModel = 0) Then
                aScn.CalcModel = g_Params.MatrixCalcModel
            End If
            
            aScn.CorrIndex = ReadLng(rs!iCorrIndex)
            aScn.SpecificParams = ReadStr(rs!vcSpecificShiftParams)
            
            aScn.FixValues
            
            Set aScn = Nothing
        End If
        rs.MoveNext
        DoEvents
    Wend
    Set rs = Nothing
    
    'g_RmScenario.LoadScenarioTimes
End Sub

Private Function GetQuotationNameByID(ByVal ID As Long) As String
    Dim Name As String
    Select Case ID
        Case 0
            Name = "None"
        Case 1
            Name = "Nominal"
        Case 2
            Name = "Bags"
        Case 3
            Name = "Bushels"
        Case 4
            Name = "Barrels"
        Case 5
            Name = "Gallons"
        Case 6
            Name = "Crams"
        Case 7
            Name = "Kilos"
        Case 8
            Name = "ShortTones"
        Case 9
            Name = "LongTones"
        Case 10
            Name = "MetricTones"
        Case 11
            Name = "Ounces"
        Case 12
            Name = "Pounds"
        Case 13
            Name = "SquareFeet"
        Case 14
            Name = "FineTroyOunces"
        Case 15
            Name = "Troy Ounces"
        Case 16
            Name = "BoardFeet"
        Case 17
            Name = "Coins"
        Case 18
            Name = "Bales"
        Case 19
            Name = "BoxCases"
        Case 20
            Name = "BottleFlaskes"
        Case 21
            Name = "Percent"
        Case 22
            Name = "NonFatDryMilk"
        Case 23
            Name = "BtuMillions"
        Case 24
            Name = "MegaWattHours"
        Case 25
            Name = "HundredWeight"
        Case 26
            Name = "HundredWeight112LBS"
        Case 27
            Name = "Ones"
        Case 28
            Name = "Tens"
        Case 29
            Name = "Dozens"
        Case 30
            Name = "Hundreds"
        Case 31
            Name = "Thousands"
        Case 32
            Name = "Millions"
        Case 33
            Name = "Pieces"
        Case 34
            Name = "TenPieces"
        Case 35
            Name = "HundredPieces"
        Case 36
            Name = "ThousandPieces"
        Case 37
            Name = "TenThousandPieces"
        Case 38
            Name = "Indices"
        Case 39
            Name = "FuturesContracts"
        Case 40
            Name = "Currency"
        Case 41
            Name = "Shares"
    End Select
    GetQuotationNameByID = Name
End Function
'

Public Function AppTitle() As String
 On Error Resume Next
 If Len(g_Params.AppTitle) Then
    AppTitle = g_Params.AppTitle
 Else
    AppTitle = APP_TITLE
 End If
End Function

Public Function AppLongTitle() As String
 On Error Resume Next
 If Len(g_Params.AppLongTitle) Then
    AppLongTitle = g_Params.AppLongTitle
 Else
    AppLongTitle = APP_LONG_TITLE
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




