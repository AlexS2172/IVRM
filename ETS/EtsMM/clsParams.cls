VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsParams"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Enum PnLCalcTypeEnum
    PNLCT_LTD = 0
    PNLCT_DAILY = 1
End Enum


Public Event ActiveRealTimeChange()
Public Event EventLogVisibleChange()
Public Event CalcModelChange()
Public Event RmScenariosChange()
Public Event SystemDbConnectionChange()
Public Event SystemPriceProviderChange()
Public Event NewUnderlyingAdded(ByVal nNewUndID As Long)
Public Event NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
Public Event QuoteOptExpirySeparationChange()
Public Event SystemVolatilitySourceChange()
Public Event InternalVolatilityChanged(ByVal sSymbol As String)
Public Event PriceProfilesChange()
Public Event UndPriceToleranceChange()
Public Event PriceRoundingRuleChange()
Public Event UnderlyingsParamsChange(ByRef collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
Public Event NewTntCntPtyAdded(ByVal nNewCntPtyID As Long)
Public Event UseTntMessagingChanged()
Public Event NewStrategyAdded(ByVal nStrategyID As Long)
Public Event NetExposureAUMChange(ByVal nNewVal As Double)

Private m_ActiveRealTime As Boolean
Private m_EventLogVisible As Boolean
Private m_hResource As Long

Public Enum WindowCode
        WND_MAIN
        WND_TRADES
        WND_QUOTES
        WND_RISK
        WND_RISKMATRIX
        WND_EXERCISE
        WND_EXERCISE_TRADES
        WND_GREEKS
        WND_HEDGESUMARY
        WND_ORDERS
        WND_PNLPROJECTIONS
        WND_QUOTATION
        WND_REPORTS
        WND_UNDERLYING
        WND_VOLAANALYCIS
        WND_VOLAMONITOR
        WND_INDEXHEDGE
        WND_LAST = WND_INDEXHEDGE
End Enum

Public PnLCalcType As PnLCalcTypeEnum
Public CalcIvInRuntime As Boolean
Public UseCalcSleep As Boolean
Public CalcSleepFreq As Long

Public EAOptWithPosition As Boolean
Public EAUpcomingDivs As Boolean
Public EADaysToDiv As Long

Public CheckExpiredOptions  As Boolean
Public EADaysToDivDays As Long

Public SOQ_CheckExpiredOptions  As Boolean
Public SOQ_EADaysToDivDays As Long

Public HedgeUseExtPosition As Boolean

Public CalcSleepAmt As Long
Private m_CalcModel As EtsGeneralLib.EtsCalcModelTypeEnum
Public ShowEventLog As Boolean
Public IsDebug As Boolean
Public DBVersion As String
Public DBPath As String

Public CalcDefUndExchID As Long
Public CalcDefOptExchID As Long

Public ShowWindowsInTaskbar As Boolean
Public MainWinAlwaysOnTop As Boolean
Private m_OsVersion As OSVERSIONINFOEX

Public QuoteBidColBuy As Byte
Public QuoteOtherColBuy As Byte
Public QuoteDirectlyToRealtime As Boolean
Public QuoteUndExchAll As Boolean
Public QuoteOptExchAll As Boolean
Public QuoteDefUndExchID As Long
Public QuoteDefOptExchID As Long
Private m_QuoteOptExpirySeparation As Boolean
Public QuoteFitToImpFlatNoBid As Boolean

Public MaxRTOptCount As Long

Public Enum FitToImpCurveTypeEnum
    enFitToImpCurveCalls = 0
    enFitToImpCurvePuts
    enFitToImpCurveOTM
End Enum

Public QuoteFitToImpCurveType As FitToImpCurveTypeEnum
Public QuoteReqTypeAlwaysNonGroup As Boolean

Public RiskBidColBuy As Byte
Public RiskOtherColBuy As Byte
Public RiskDirectlyToRealtime As Boolean
Public RiskReqTypeAlwaysNonGroup As Boolean

Public MatrixExerciseOptions As Boolean
Public MatrixCalcModel As EtsGeneralLib.EtsCalcModelTypeEnum
Public MatrixReqTypeAlwaysNonGroup As Boolean

Public UseTheoVolatility As Boolean
Public UseTheoNoBid As Boolean
Public UseTheoBadMarketVola As Boolean

'EtsMM needs this data for notify user of new data from IVLoader
Public LastStopDate As String
Public LastStopTime As String
Public LastDataDate As String
Public LastDataTime As String
Public LastIVFile As String
Public LastIRFile As String
Public LastIWFile As String
Public LastDVFile As String
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Public ExchangeRate As Double
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'Private WithEvents EtsSettings As ETSMANAGERLib.Settings
Private m_sDbConnection As String
Private m_sIvDbConnection As String

Private m_sIvFeedUser As String
Private m_sIvFeedPassword As String
    
Private m_sUserGroup As String
'Private m_sDbSupportConnection As String
Private m_bChangingDbConnection As Boolean
Private m_sSystemDbConnection As String

Private m_nPriceProviderType As Long
Private m_sPriceProviderProgID As String
Public PriceProviderIsGroupRequestSupported As Boolean
Private m_bChangingPriceProvider As Boolean
Private m_nSystemPriceProviderType As Long

Private m_nVolatilitySourceType As Long
Private m_sVolatilitySourceProgID As String
Private m_bChangingVolatilitySource As Boolean
Private m_nSystemVolatilitySourceType As Long

Private m_sAppTitle As String
Private m_sAppLongTitle As String
Private m_sAppDescription As String
Private m_sAppCopyright As String


Private m_nTimeZoneBias As Long

Public UseDefTrader As Boolean
Public DefTraderID As Long
Private m_nOrdersSupport As Long

Private m_sUnderlyingComboListAllWhtFutUnd As String ' all stocks/indices (without future underlyings)
Private m_sUnderlyingComboListAll As String ' all stocks/indices/future underlyings
Private m_sUnderlyingComboList As String ' current trader stocks/indices
Private m_sStockComboList$ ' all stocks
Private m_sBasketIndexComboList$ ' all basket indices
Private m_VSCalcEnabled As Boolean
Private m_CVEnabled As Boolean
Private m_dNetExposureAUM As Double

Private m_sUnderlyingWithFuturesComboListAll As String ' all stocks/indices/futures
Private m_sUnderlyingWithFuturesComboList As String ' current trader stocks/indices/futures

Private m_bActualTradesEditable As Boolean

Private m_enPriceRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
Private m_dUndPriceToleranceValue As Double


Public DefTradeType As TRADE_STATUS

Public LogAdvancedInfo As Boolean
Public LogPerformanceCounters As Boolean
Public LogPerfCountInterval As Long
Public ProcessorsLoad As Long

Public DoNotCheckDbVersion As Boolean

Public OrdDefStkQtyIncrement As Long
Public OrdDefOptQtyIncrement As Long
Public OrdDefStkPrcIncrement As Double
Public OrdDefOptPrcIncrementCheap As Double
Public OrdDefOptPrcIncrementExpensive As Double

Public CrdDefQtyIncrement As Long
Public CrdDefPrcIncrementCheap As Double
Public CrdDefPrcIncrementExpensive As Double

Public FTPExportPath As String

Private m_dCommissionLowPremiumLimit#

Public MarkedTradesRuleOn As Boolean

Public StockHedgeSlice#
Public StockHedgeLotSize&
Public StockHedgeMaxSize&
Public StockHedgeExportPath$

Public IndexHedgeSlice#
Public IndexHedgeLotSize&
Public IndexHedgeMaxSize&

Public UserName As String
Public CurrentUserPrivateFolder$
Public ScannersLogin As String
Public ScanStrategyName As String
Public ScanStrategyPath As String
Public ScanOptionsName As String
Public ScanOptionsPath As String
Public ScanStrategyWindClass As String
Public ScanOptionsWindClass As String
Public ShowMessageUnableToEdit As Boolean
Public UseTheoCloseForPNL As Boolean

Public SMTPServer As String
Public MailAddress As String

Private m_BatchReports As Collection
Private m_BatchReportingShedules As Collection

Public BR_Email As String
Public BR_SendEmail As Boolean
Public BR_FilterDisplay As Boolean
Public BR_IncludeTime As Boolean
Public BR_IncludeDate As Boolean
Public BR_IncludeFilters As Boolean

Public ExecuteBatchReporting As Boolean
Public AllReportsAvalable As Boolean
Public UseTTE As Boolean
Private m_hedgeSymbol As String
Private m_realtimeCalculateCV As Boolean
Private m_calculateCVCount As Long

Public GroupSpecificRatioFile As String
Public StockShiftFile As String
Public CurveInterpolationType As Long
Public CurveExtrapolationType As Long
Public LogDirectory As String

Public Property Get RealtimeCalculateCV() As Boolean
On Error Resume Next
    RealtimeCalculateCV = m_realtimeCalculateCV
End Property

Public Property Let RealtimeCalculateCV(Flag As Boolean)
On Error Resume Next
    m_realtimeCalculateCV = Flag
End Property

Public Property Get CalculateCVCount() As Long
On Error Resume Next
    CalculateCVCount = m_calculateCVCount
End Property

Public Property Let CalculateCVCount(Count As Long)
On Error Resume Next
    m_calculateCVCount = Count
End Property

Public Property Get IsVSCalcEnabled() As Boolean
    IsVSCalcEnabled = m_VSCalcEnabled
End Property
Public Property Get IsCVEnabled() As Boolean
    IsCVEnabled = m_CVEnabled
End Property
Public Property Get AppTitle() As String
    AppTitle = m_sAppTitle
End Property
Public Property Get AppLongTitle() As String
    AppLongTitle = m_sAppLongTitle
End Property
Public Property Get AppDescription() As String
    AppDescription = m_sAppDescription
End Property
Public Property Get AppCopyright() As String
    AppCopyright = m_sAppCopyright
End Property

Public Property Get HedgeSymbol() As String
On Error Resume Next
    HedgeSymbol = m_hedgeSymbol
End Property

Public Property Let HedgeSymbol(Symbol As String)
On Error Resume Next
    m_hedgeSymbol = Symbol
End Property
Public Property Get CalcModel() As EtsGeneralLib.EtsCalcModelTypeEnum
    CalcModel = m_CalcModel
End Property

Public Property Let CalcModel(NewValue As EtsGeneralLib.EtsCalcModelTypeEnum)
    On Error Resume Next
    If m_CalcModel <> NewValue Then
        m_CalcModel = NewValue
        RaiseEvent CalcModelChange
    End If
End Property


Public Function CalcModelName(ByVal enCalcModel As EtsGeneralLib.EtsCalcModelTypeEnum) As String
    On Error Resume Next
    Select Case enCalcModel
        Case enCmBinomial
            CalcModelName = "Standard"
        Case enCmBinomialOpt
            CalcModelName = "Optimized"
        Case enCmVskLog
            CalcModelName = "VSK Log"
    End Select
End Function

Public Property Get NetExposureAUM() As Double
    NetExposureAUM = m_dNetExposureAUM
End Property
Public Property Let NetExposureAUM(dAUM As Double)
    If m_dNetExposureAUM <> dAUM Then
        m_dNetExposureAUM = dAUM
        FireNetExposureAUMChange dAUM
    Else
         m_dNetExposureAUM = dAUM
    End If
End Property

Public Property Get OrdersEnabled() As Boolean
    OrdersEnabled = m_nOrdersSupport > 1
End Property
Public Property Get OrdersVisible() As Boolean
    OrdersVisible = m_nOrdersSupport > 0
End Property

Public Property Get ActiveRealTime() As Boolean
    ActiveRealTime = m_ActiveRealTime
End Property

Public Property Let ActiveRealTime(NewValue As Boolean)
    On Error Resume Next
    m_ActiveRealTime = NewValue
    RaiseEvent ActiveRealTimeChange
End Property

Public Property Get EventLogVisible() As Boolean
    EventLogVisible = m_EventLogVisible
End Property

Public Property Let EventLogVisible(NewValue As Boolean)
    On Error Resume Next
    m_EventLogVisible = NewValue
    RaiseEvent EventLogVisibleChange
End Property

Public Property Get QuoteOptExpirySeparation() As Boolean
    QuoteOptExpirySeparation = m_QuoteOptExpirySeparation
End Property

Public Property Let QuoteOptExpirySeparation(NewValue As Boolean)
    On Error Resume Next
    m_QuoteOptExpirySeparation = NewValue
    RaiseEvent QuoteOptExpirySeparationChange
End Property

Public Sub ReloadCustomParams()
On Error GoTo err_exception
    
    Dim aXMLLoader As ETSXMLParamsLib.XMLLoader
    Set aXMLLoader = New ETSXMLParamsLib.XMLLoader
    
    Dim aUserXMLParams As ETSXMLParamsLib.XMLParams
    
    Set aUserXMLParams = aXMLLoader.Load(CurrentUserXMLFilePath)
    If Not aUserXMLParams Is Nothing Then
    
            'reload fit2impl params settings
            QuoteFitToImpFlatNoBid = (aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteFitToImpFlatNoBid", 1) <> 0)
            QuoteFitToImpCurveType = aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteFitToImpCurveType", enFitToImpCurveOTM)
            
            If QuoteFitToImpCurveType <> enFitToImpCurveCalls _
                And QuoteFitToImpCurveType <> enFitToImpCurvePuts _
                And QuoteFitToImpCurveType <> enFitToImpCurveOTM Then
                QuoteFitToImpCurveType = enFitToImpCurveOTM
            End If
            
            'reload PnL Settings
            UseTheoCloseForPNL = (aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTheoCloseForPNL", 1) <> 0)

            PnLCalcType = aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "PnLCalcType", PNLCT_DAILY)
            If PnLCalcType < PNLCT_LTD Then
                PnLCalcType = PNLCT_LTD
            ElseIf PnLCalcType > PNLCT_DAILY Then
                PnLCalcType = PNLCT_DAILY
            End If
            
            'Model reload
            m_CalcModel = aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcModel", enCmBinomialOpt)
            If m_CalcModel <> enCmBinomial And m_CalcModel <> enCmBinomialOpt And m_CalcModel <> enCmVskLog Then
                m_CalcModel = enCmBinomialOpt
            End If
            
            'Matrix calc type
            MatrixExerciseOptions = (aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MatrixExerciseOptions", 0) <> 0)
            MatrixCalcModel = aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MatrixCalcModel", enCmBinomialOpt)
            If MatrixCalcModel <> enCmBinomial And MatrixCalcModel <> enCmBinomialOpt And MatrixCalcModel <> enCmVskLog Then
                MatrixCalcModel = enCmBinomialOpt
            End If
            
            'Calculation Precision Mode
            UseTTE = aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTTE", 0) <> 0
            g_Main.CalculationParametrs.UseTimePrecision = UseTTE
    End If
    
Exit Sub
err_exception:
    Debug.Print "Error while reloading CustomParams()"
End Sub

Public Sub LoadSettings()
    On Error GoTo EH
    
    'If EtsSettings Is Nothing Then Set EtsSettings = New ETSMANAGERLib.Settings
    Dim sUserName$, sUserGroup$
    sUserName = CurrentNtUserName
    
    If Len(sUserName) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user name."
    End If
    
    sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", "")
    If Len(sUserGroup) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user ETS group."
    End If
    
    m_sUserGroup = sUserGroup
    FTPExportPath = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\FTP", sUserGroup, "")
    m_sDbConnection = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Database", sUserGroup, "")
    m_sIvDbConnection = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\IvDatabase", sUserGroup, "")
    m_sIvFeedUser = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "IVFeed", "IVSDKLogin", "")
    m_sIvFeedPassword = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "IVFeed", "IVSDKPassword", "")
    
    LastDVFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastDivFile", "")
    LastIVFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastVolaFile", "")
    LastIRFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastIRFile", "")
    LastIWFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastIWFile", "")
    
    HedgeSymbol = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup, "HedgeSymbol", "")
    RealtimeCalculateCV = IIf(g_aMainXMLParams.GetLongValue(GROUP_XML_KEY & sUserGroup, "RealtimeCalculateCV", 0) = 0, False, True)
    CalculateCVCount = g_aMainXMLParams.GetLongValue(GROUP_XML_KEY & sUserGroup, "CalculateCVCount", 0)
    
    ExchangeRate = (CDbl(g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\ExchangeRates", CAN_REGION, "1")))
    
    'this GetString must to be the first
    LastDataDate = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastDataDate", Format(Date))
    LastDataTime = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & sUserGroup & "\DataLoader", "LastDataTime", Format(Time))
    Sleep 1000
    'this GetString must to be the second
    LastStopDate = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "LastStopDate", Format(Date))
    LastStopTime = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "LastStopTime", Format(Time))

    If Len(m_sDbConnection) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user database connection params."
    End If
    
    m_sSystemDbConnection = m_sDbConnection
    
    m_nPriceProviderType = 6 ' Hyperfeed ' EtsSettings.PriceProvider
    m_sPriceProviderProgID = ""
    m_nSystemPriceProviderType = m_nPriceProviderType
    
    m_nVolatilitySourceType = 0 ' VME ' EtsSettings.VolatilitySource
    m_sVolatilitySourceProgID = ""
    m_nSystemVolatilitySourceType = m_nVolatilitySourceType
    
    On Error Resume Next
    LoadCurrentUserPrivateFolderPath
    
    DBVersion = "N/A"
    DBPath = "N/A"
    m_OsVersion.dwOSVersionInfoSize = Len(m_OsVersion)
    GetVersionEx m_OsVersion
    
    m_CalcModel = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcModel", enCmBinomialOpt)
    If m_CalcModel <> enCmBinomial And m_CalcModel <> enCmBinomialOpt And m_CalcModel <> enCmVskLog Then
        m_CalcModel = enCmBinomialOpt
    End If
    
'    PnLCalcType = PNLCT_DAILY

    PnLCalcType = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "PnLCalcType", PNLCT_DAILY)
    If PnLCalcType < PNLCT_LTD Then
        PnLCalcType = PNLCT_LTD
    ElseIf PnLCalcType > PNLCT_DAILY Then
        PnLCalcType = PNLCT_DAILY
    End If

    m_ActiveRealTime = True ' (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "ActiveRealTime", 1) <> 0)
    CalcIvInRuntime = True '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcIvInRuntime", 1) <> 0)
    ShowEventLog = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "ShowEventLog", 1) <> 0)
    
    CalcDefUndExchID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcDefUndExchID", 0)
    CalcDefOptExchID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcDefOptExchID", 0)
    AllReportsAvalable = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "AllReportsAvalable", 0) <> 0
    UseTTE = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTTE", 0) <> 0
        
    m_VSCalcEnabled = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "VSCalcEnabled", 0) > 0, True, False)
    m_CVEnabled = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CVEnabled", 0) > 0, True, False)
    
    UseCalcSleep = True '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseCalcSleep", 1) <> 0)
    CalcSleepFreq = 10 'g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcSleepFreq", 10)
    CalcSleepAmt = 1 ' g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CalcSleepAmt", 1)
    
    Dim longBuff As Long
    longBuff = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "EAOptWithPosition", 0)
    If longBuff <> 0 Then
        EAOptWithPosition = True
    Else
        EAOptWithPosition = False
    End If
    
    longBuff = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CheckExpiredOptions", 0)
    If longBuff <> 0 Then
        SOQ_CheckExpiredOptions = True
    Else
        SOQ_CheckExpiredOptions = False
    End If
        
    longBuff = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "EADaysToDivDays", 0)
    SOQ_EADaysToDivDays = longBuff
       
    longBuff = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "EAUpcomingDivs", 0)
    If longBuff <> 0 Then
        EAUpcomingDivs = True
    Else
        EAUpcomingDivs = False
    End If
    
    EADaysToDiv = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "EADaysToDiv", 7)
    
    longBuff = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "HedgeUseExtPosition", 0)
    If longBuff <> 0 Then
        HedgeUseExtPosition = True
    Else
        HedgeUseExtPosition = False
    End If
     
    If CalcSleepFreq <= 0 Then
        CalcSleepFreq = 1
    ElseIf CalcSleepFreq > 1000 Then
        CalcSleepFreq = 1000
    End If

    If CalcSleepAmt <= 0 Then
        CalcSleepAmt = 1
    ElseIf CalcSleepAmt > 1000 Then
        CalcSleepAmt = 1000
    End If

    MainWinAlwaysOnTop = False ' (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MainWinAlwaysOnTop", 1) <> 0)
    ShowWindowsInTaskbar = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "ShowWindowsInTaskbar", 1) <> 0)
    
    QuoteBidColBuy = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteBidColBuy", 0) <> 0, 1, 0)
    QuoteOtherColBuy = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteOtherColBuy", 0) <> 0, 1, 0)
    QuoteDirectlyToRealtime = False ' (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteDirectlyToRealtime", 0) <> 0)
    QuoteUndExchAll = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteUndExchAll", 1) <> 0)
    QuoteOptExchAll = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteOptExchAll", 1) <> 0)
    QuoteDefUndExchID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteDefUndExchID", 0)
    QuoteDefOptExchID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteDefOptExchID", 0)
    m_QuoteOptExpirySeparation = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteOptExpirySeparation", 1) <> 0)
    
    QuoteFitToImpFlatNoBid = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteFitToImpFlatNoBid", 1) <> 0)
    QuoteFitToImpCurveType = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteFitToImpCurveType", enFitToImpCurveOTM)
    QuoteDirectlyToRealtime = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteDirectlyToRealtime", 0) <> 0)
    If QuoteFitToImpCurveType <> enFitToImpCurveCalls _
        And QuoteFitToImpCurveType <> enFitToImpCurvePuts _
        And QuoteFitToImpCurveType <> enFitToImpCurveOTM Then
        QuoteFitToImpCurveType = enFitToImpCurveOTM
    End If
    
    m_dNetExposureAUM = CDbl(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "NetExposureAUM", "1.0"))
    
    QuoteReqTypeAlwaysNonGroup = False '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "QuoteReqTypeAlwaysNonGroup", 0) <> 0)
    
    RiskBidColBuy = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "RiskBidColBuy", 0) <> 0, 1, 0)
    RiskOtherColBuy = IIf(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "RiskOtherColBuy", 0) <> 0, 1, 0)
    RiskDirectlyToRealtime = False '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "RiskDirectlyToRealtime", 0) <> 0)
    RiskReqTypeAlwaysNonGroup = True '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "RiskReqTypeAlwaysNonGroup", 1) <> 0)
    ShowMessageUnableToEdit = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "ShowMessageUnableToEdit", 1) <> 0)
    UseTheoCloseForPNL = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTheoCloseForPNL", 1) <> 0)
    
    If (Not g_Main Is Nothing) Then
        g_Main.UseTheoCloseForPNL = UseTheoCloseForPNL
    End If
    
    MatrixExerciseOptions = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MatrixExerciseOptions", 0) <> 0)
    MatrixCalcModel = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MatrixCalcModel", enCmBinomialOpt)
    If MatrixCalcModel <> enCmBinomial And MatrixCalcModel <> enCmBinomialOpt And MatrixCalcModel <> enCmVskLog Then
        MatrixCalcModel = enCmBinomialOpt
    End If
    MatrixReqTypeAlwaysNonGroup = True '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MatrixReqTypeAlwaysNonGroup", 0) <> 0)
    
    UseTheoVolatility = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTheoVolatility", 1) <> 0)
    UseTheoNoBid = False '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTheoNoBid", 1) <> 0)
    UseTheoBadMarketVola = False '(g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "UseTheoBadMarketVola", 1) <> 0)
    
    Dim tzi(0 To 0) As TIME_ZONE_INFORMATION, nResult&
    Erase tzi
    Dim sTxtGroup$, sTxtSettings$

    nResult = GetTimeZoneInformation(tzi(0))
    If nResult <> TIME_ZONE_ID_INVALID Then
        m_nTimeZoneBias = tzi(0).Bias
        If nResult = TIME_ZONE_ID_DAYLIGHT Then
            m_nTimeZoneBias = m_nTimeZoneBias + tzi(0).DaylightDate.wMinute
        End If
    Else
        m_nTimeZoneBias = 0
    End If
    
    sTxtGroup = GROUP_XML_KEY & sUserGroup
    
    m_nOrdersSupport = g_aMainXMLParams.GetLongValue(sTxtGroup, "Orders", 0)
    If m_nOrdersSupport > 0 Then
        m_nOrdersSupport = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "Orders", m_nOrdersSupport)
    End If
    
    
    
    LoadResource g_aMainXMLParams.GetStringValue(sTxtGroup, "Appearance", "")
    If m_hResource <> 0 Then
        Dim strString$, lngStringLen As Long
        
        strString = String(512, Chr(0))
        lngStringLen = LoadString(m_hResource, 1001, strString, Len(strString))
        If lngStringLen <> 0 Then m_sAppTitle = Left(strString, lngStringLen)

        strString = String(512, Chr(0))
        lngStringLen = LoadString(m_hResource, 1000, strString, Len(strString))
        If lngStringLen <> 0 Then m_sAppLongTitle = Left(strString, lngStringLen)

        strString = String(512, Chr(0))
        lngStringLen = LoadString(m_hResource, 1002, strString, Len(strString))
        If lngStringLen <> 0 Then m_sAppDescription = Left(strString, lngStringLen)

        strString = String(512, Chr(0))
        lngStringLen = LoadString(m_hResource, 1003, strString, Len(strString))
        If lngStringLen <> 0 Then m_sAppCopyright = Left(strString, lngStringLen)

    End If

   
    sTxtSettings = APP_XML_KEY & "Settings"
    
    UseDefTrader = (g_aUserXMLParams.GetLongValue(sTxtSettings, "UseDefTrader", 0) <> 0)
    DefTraderID = g_aUserXMLParams.GetLongValue(sTxtSettings, "DefTraderID", 0)
    DefTradeType = g_aUserXMLParams.GetLongValue(sTxtSettings, "DefTradeType", -1)
    
    LogAdvancedInfo = CBool(g_aUserXMLParams.GetLongValue(sTxtSettings, "LogAdvancedInfo", 1) <> 0)
    ProcessorsLoad = g_aUserXMLParams.GetLongValue(sTxtSettings, "ProcessorsLoad", 2)
  
    LogPerformanceCounters = CBool(g_aUserXMLParams.GetLongValue(sTxtSettings, "LogPerformanceCounters", 0) <> 0)
    LogPerfCountInterval = g_aUserXMLParams.GetLongValue(sTxtSettings, "LogPerfCountInterval", 1)
    
    DoNotCheckDbVersion = (g_aUserXMLParams.GetLongValue(sTxtSettings, "DoNotCheckDbVersion", 0) <> 0)

    OrdDefStkQtyIncrement = g_aUserXMLParams.GetLongValue(sTxtSettings, "OrdDefStkQtyIncrement", 100)
    If OrdDefStkQtyIncrement <= 0& Then
        OrdDefStkQtyIncrement = 100
    ElseIf OrdDefStkQtyIncrement > 10000 Then
        OrdDefStkQtyIncrement = 10000
    End If
    
    OrdDefOptQtyIncrement = g_aUserXMLParams.GetLongValue(sTxtSettings, "OrdDefOptQtyIncrement", 10)
    If OrdDefOptQtyIncrement <= 0& Then
        OrdDefOptQtyIncrement = 10
    ElseIf OrdDefOptQtyIncrement > 10000 Then
        OrdDefOptQtyIncrement = 10000
    End If
    
    OrdDefStkPrcIncrement = Round(g_aUserXMLParams.GetLongValue(sTxtSettings, "OrdDefStkPrcIncrement", 1) / 100#, 2)
    If OrdDefStkPrcIncrement < 0.01 Then
        OrdDefStkPrcIncrement = 0.01
    ElseIf OrdDefStkPrcIncrement > 100# Then
        OrdDefStkPrcIncrement = 100#
    End If
    
    OrdDefOptPrcIncrementCheap = Round(g_aUserXMLParams.GetLongValue(sTxtSettings, "OrdDefOptPrcIncrementCheap", 5) / 100#, 2)
    If OrdDefOptPrcIncrementCheap < 0.01 Then
        OrdDefOptPrcIncrementCheap = 0.05
    ElseIf OrdDefOptPrcIncrementCheap > 100# Then
        OrdDefOptPrcIncrementCheap = 100#
    End If
    
    OrdDefOptPrcIncrementExpensive = Round(g_aUserXMLParams.GetLongValue(sTxtSettings, "OrdDefOptPrcIncrementExpensive", 10) / 100#, 2)
    If OrdDefOptPrcIncrementExpensive < 0.01 Then
        OrdDefOptPrcIncrementExpensive = 0.1
    ElseIf OrdDefOptPrcIncrementExpensive > 100# Then
        OrdDefOptPrcIncrementExpensive = 100#
    End If
    
    CrdDefQtyIncrement = g_aUserXMLParams.GetLongValue(sTxtSettings, "CrdDefQtyIncrement", 10)
    If CrdDefQtyIncrement <= 0& Then
        CrdDefQtyIncrement = 10
    ElseIf CrdDefQtyIncrement > 10000 Then
        CrdDefQtyIncrement = 10000
    End If
    
    CrdDefPrcIncrementCheap = Round(g_aUserXMLParams.GetLongValue(sTxtSettings, "CrdDefPrcIncrementCheap", 5) / 100#, 2)
    If CrdDefPrcIncrementCheap < 0.01 Then
        CrdDefPrcIncrementCheap = 0.05
    ElseIf CrdDefPrcIncrementCheap > 100# Then
        CrdDefPrcIncrementCheap = 100#
    End If
    
    CrdDefPrcIncrementExpensive = Round(g_aUserXMLParams.GetLongValue(sTxtSettings, "CrdDefPrcIncrementExpensive", 10) / 100#, 2)
    If CrdDefPrcIncrementExpensive < 0.01 Then
        CrdDefPrcIncrementExpensive = 0.1
    ElseIf CrdDefPrcIncrementExpensive > 100# Then
        CrdDefPrcIncrementExpensive = 100#
    End If
    
    MarkedTradesRuleOn = (g_aUserXMLParams.GetLongValue(sTxtSettings, "MarkedTradesRuleOn", 0) <> 0)
    
    StockHedgeSlice = g_aUserXMLParams.GetLongValue(sTxtSettings, "StockHedgeSlice", 100&) / 100#
    If StockHedgeSlice < 0.01 Then
        StockHedgeSlice = 0.01
    ElseIf StockHedgeSlice > 1# Then
        StockHedgeSlice = 1#
    End If

    StockHedgeLotSize = g_aUserXMLParams.GetLongValue(sTxtSettings, "StockHedgeLotSize", 100&)
    If StockHedgeLotSize <= 0 Then _
        StockHedgeLotSize = 100
        
    StockHedgeMaxSize = g_aUserXMLParams.GetLongValue(sTxtSettings, "StockHedgeMaxSize", 10000&)
    If StockHedgeMaxSize <= 0 Then _
        StockHedgeMaxSize = 10000
        
    StockHedgeExportPath = g_aUserXMLParams.GetStringValue(sTxtSettings, "StockHedgeExportPath", App.Path & "\")
    
    IndexHedgeSlice = g_aUserXMLParams.GetLongValue(sTxtSettings, "IndexHedgeSlice", 100&) / 100#
    If IndexHedgeSlice < 0.01 Then
        IndexHedgeSlice = 0.01
    ElseIf IndexHedgeSlice > 1# Then
        IndexHedgeSlice = 1#
    End If

    IndexHedgeLotSize = g_aUserXMLParams.GetLongValue(sTxtSettings, "IndexHedgeLotSize", 100&)
    If IndexHedgeLotSize <= 0 Then _
        IndexHedgeLotSize = 100
        
    IndexHedgeMaxSize = g_aUserXMLParams.GetLongValue(sTxtSettings, "IndexHedgeMaxSize", 10000&)
    If IndexHedgeMaxSize <= 0 Then _
        IndexHedgeMaxSize = 10000
    
    
    MaxRTOptCount = g_aMainXMLParams.GetLongValue(PRODUCT_XML_KEY & "Asp\Calculation", sUserGroup, 500)
    
    If (MaxRTOptCount <= 0) Or (MaxRTOptCount > 10000) Then
        MaxRTOptCount = 500
    End If
    
    g_lMinLogLevel = (g_aMainXMLParams.GetLongValue(PRODUCT_XML_KEY & "Asp\MMLogLevel", sUserGroup, CLng(enLogWarning)))
    g_lLogLifetime = (g_aMainXMLParams.GetLongValue(PRODUCT_XML_KEY & "Asp\LogLifetime", sUserGroup, 2))
    
    g_MaxRTQuantity = (g_aMainXMLParams.GetLongValue(PRODUCT_XML_KEY & "Asp\MaxQuoteRTNumber", sUserGroup, 4))
    If (g_MaxRTQuantity <= 0) Or (g_MaxRTQuantity > 300) Then
        g_MaxRTQuantity = 1
    End If
    
    g_MaxRvRTQuantity = (g_aMainXMLParams.GetLongValue(PRODUCT_XML_KEY & "Asp\MaxRiskRTNumber", sUserGroup, 4))
    If (g_MaxRvRTQuantity <= 0) Or (g_MaxRvRTQuantity > 300) Then
        g_MaxRvRTQuantity = 1
    End If
    
    ScannersLogin = g_aUserXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners\Settings", "Login", "")
    If Len(ScannersLogin) <= 0 Then
        g_bIsScannersEnable = False
    Else
        g_bIsScannersEnable = True
    End If
    
        ScanStrategyName = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanStrategyName", "")
        ScanStrategyPath = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanStrategyPath", "")
        ScanOptionsName = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanOptionsName", "")
        ScanOptionsPath = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanOptionsPath", "")
        ScanOptionsWindClass = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanOptionsWindClass", "")
        ScanStrategyWindClass = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Scanners", "ScanStrategyWindClass", "")
        
        
    BR_FilterDisplay = (g_aUserXMLParams.GetLongValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "FilterDisplay", 0) <> 0)
    BR_SendEmail = (g_aUserXMLParams.GetLongValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "SendEmail", 0) <> 0)
    BR_Email = g_aUserXMLParams.GetStringValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "Email", "")
    'Show time stemp in file name of generated report
    BR_IncludeTime = (g_aUserXMLParams.GetLongValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeTime", 0) <> 0)
    'Show date stemp in file name of generated report
    BR_IncludeDate = (g_aUserXMLParams.GetLongValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeDate", 0) <> 0)
    'Show used filters in file name of generated report
    BR_IncludeFilters = (g_aUserXMLParams.GetLongValue(PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeFilters", 0) <> 0)
    
    'Get group Specific ratio file for risk matrix specific simulation
    GroupSpecificRatioFile = g_aUserXMLParams.GetStringValue(PRODUCT_XML_KEY & "MarketMaker\RiskMatrix", "GroupSpecificRatioFile", "")
    
    SMTPServer = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Groups\" & sUserGroup & "\SMTPSettings", "SMTPServer", "")
    MailAddress = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Groups\" & sUserGroup & "\SMTPSettings", "SenderAddress", "")
    
    ExecuteBatchReporting = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "ExecuteBatchReporting", 0) <> 0)
    
    StockShiftFile = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "StockShiftFile", STR_NA)
        
    CurveInterpolationType = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CurveInterpolationType", 1)
    CurveExtrapolationType = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "CurveExtrapolationType", 1)
    
    UserName = CurrentNtUserName
    
    LogDirectory = g_aMainXMLParams.GetStringValue(GENERAL_SETTINGS, "LogDirectory", "")
    If (Len(LogDirectory) > 0) Then
        LogDirectory = LogDirectory & "\" & UserName
    End If
    
EH:
    gCmn.ErrorHandler "Fail to load application settings."
End Sub

Public Sub SaveSettings()
    On Error Resume Next
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseTTE", IIf(UseTTE, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcModel", m_CalcModel
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "PnLCalcType", PnLCalcType

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcDefUndExchID", CalcDefUndExchID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcDefOptExchID", CalcDefOptExchID

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "ActiveRealTime", IIf(m_ActiveRealTime, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcIvInRuntime", IIf(CalcIvInRuntime, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "ShowEventLog", IIf(ShowEventLog, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseCalcSleep", IIf(UseCalcSleep, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcSleepFreq", CalcSleepFreq
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CalcSleepAmt", CalcSleepAmt
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "EAOptWithPosition", EAOptWithPosition
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "EAUpcomingDivs", EAUpcomingDivs
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "EADaysToDiv", EADaysToDiv
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CheckExpiredOptions", SOQ_CheckExpiredOptions
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "EADaysToDivDays", SOQ_EADaysToDivDays
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "HedgeUseExtPosition", HedgeUseExtPosition
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MainWinAlwaysOnTop", IIf(MainWinAlwaysOnTop, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "ShowWindowsInTaskbar", IIf(ShowWindowsInTaskbar, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteBidColBuy", QuoteBidColBuy
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteOtherColBuy", QuoteOtherColBuy
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteDirectlyToRealtime", IIf(QuoteDirectlyToRealtime, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteUndExchAll", IIf(QuoteUndExchAll, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteOptExchAll", IIf(QuoteOptExchAll, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteDefUndExchID", QuoteDefUndExchID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteDefOptExchID", QuoteDefOptExchID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteOptExpirySeparation", m_QuoteOptExpirySeparation
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteFitToImpFlatNoBid", IIf(QuoteFitToImpFlatNoBid, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteFitToImpCurveType", QuoteFitToImpCurveType
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "QuoteReqTypeAlwaysNonGroup", IIf(QuoteReqTypeAlwaysNonGroup, 1, 0)
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "RiskBidColBuy", RiskBidColBuy
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "RiskOtherColBuy", RiskOtherColBuy
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "RiskDirectlyToRealtime", IIf(RiskDirectlyToRealtime, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "RiskReqTypeAlwaysNonGroup", IIf(RiskReqTypeAlwaysNonGroup, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "ShowMessageUnableToEdit", IIf(ShowMessageUnableToEdit, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseTheoCloseForPNL", IIf(UseTheoCloseForPNL, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MatrixExerciseOptions", IIf(MatrixExerciseOptions, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MatrixCalcModel", MatrixCalcModel
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MatrixReqTypeAlwaysNonGroup", IIf(MatrixReqTypeAlwaysNonGroup, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseTheoVolatility", IIf(UseTheoVolatility, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseTheoNoBid", IIf(UseTheoNoBid, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseTheoBadMarketVola", IIf(UseTheoBadMarketVola, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "UseDefTrader", IIf(UseDefTrader, 1, 0)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "DefTraderID", DefTraderID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "DefTradeType", DefTradeType

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "OrdDefStkQtyIncrement", OrdDefStkQtyIncrement
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "OrdDefOptQtyIncrement", OrdDefOptQtyIncrement
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "OrdDefStkPrcIncrement", CLng(OrdDefStkPrcIncrement * 100#)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "OrdDefOptPrcIncrementCheap", CLng(OrdDefOptPrcIncrementCheap * 100#)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "OrdDefOptPrcIncrementExpensive", CLng(OrdDefOptPrcIncrementExpensive * 100#)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CrdDefQtyIncrement", CrdDefQtyIncrement
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CrdDefPrcIncrementCheap", CLng(CrdDefPrcIncrementCheap * 100#)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CrdDefPrcIncrementExpensive", CLng(CrdDefPrcIncrementExpensive * 100#)
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MarkedTradesRuleOn", IIf(MarkedTradesRuleOn, 1, 0)

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "StockHedgeSlice", Int(StockHedgeSlice * 100)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "StockHedgeLotSize", StockHedgeLotSize
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "StockHedgeMaxSize", StockHedgeMaxSize

    g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "StockHedgeExportPath", StockHedgeExportPath

    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "IndexHedgeSlice", Int(IndexHedgeSlice * 100)
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "IndexHedgeLotSize", IndexHedgeLotSize
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "IndexHedgeMaxSize", IndexHedgeMaxSize
    
    g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "NetExposureAUM", CStr(m_dNetExposureAUM)
    g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "LastStopDate", Format(Date)
    g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "LastStopTime", Format(Time)
    
    'Save USD/CAD Rate
    g_aMainXMLParams.SetStringValue PRODUCT_XML_KEY & "Asp\ExchangeRates", CAN_REGION, CStr(ExchangeRate)
    g_aMainXMLParams.SetStringValue GROUP_XML_KEY & UserGroup, "HedgeSymbol", HedgeSymbol
    
    g_aUserXMLParams.SetLongValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "FilterDisplay", BR_FilterDisplay
    g_aUserXMLParams.SetLongValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "SendEmail", BR_SendEmail
    g_aUserXMLParams.SetStringValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "Email", BR_Email
    g_aUserXMLParams.SetLongValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeDate", BR_IncludeDate
    g_aUserXMLParams.SetLongValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeTime", BR_IncludeTime
    g_aUserXMLParams.SetLongValue PRODUCT_XML_KEY & "MarketMaker\BatchReporting", "IncludeFilters", BR_IncludeFilters
    
    g_aUserXMLParams.SetStringValue PRODUCT_XML_KEY & "MarketMaker\RiskMatrix", "GroupSpecificRatioFile", GroupSpecificRatioFile
    g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "StockShiftFile", StockShiftFile
    
    'g_aMainXMLParams.SetStringValue PRODUCT_XML_KEY & "Asp\Groups\" & m_sUserGroup & "\SMTPSettings", "SMTPServer", SMTPServer
    'g_aMainXMLParams.SetStringValue PRODUCT_XML_KEY & "Asp\Groups\" & m_sUserGroup & "\SMTPSettings", "SenderAddress", MailAddress
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "ExecuteBatchReporting", ExecuteBatchReporting
    
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CurveInterpolationType", CurveInterpolationType
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "CurveExtrapolationType", CurveExtrapolationType
    
End Sub

Public Function IsMultiScreenSupported() As Boolean
    On Error Resume Next
    ' need Win98 and older or Win2000 and older
    IsMultiScreenSupported = m_OsVersion.dwPlatformId = VER_PLATFORM_WIN32_WINDOWS _
                            And m_OsVersion.dwMajorVersion >= 4 _
                            And m_OsVersion.dwMinorVersion >= 10 _
                            Or m_OsVersion.dwPlatformId = VER_PLATFORM_WIN32_NT _
                            And m_OsVersion.dwMajorVersion >= 5 _
                            And m_OsVersion.dwMinorVersion >= 0
End Function

Private Function GetIsDebug() As Boolean
    On Error Resume Next
    Dim sFileName$, nCount&
    
    sFileName = String$(1024, 0)
    nCount = GetModuleFileName(App.hInstance, sFileName, 1024)
    sFileName = Left$(sFileName, nCount)
    
    GetIsDebug = (UCase$(Right$(sFileName, 7)) = "VB6.EXE")
End Function

Public Sub FireRmScenariosChange()
    On Error Resume Next
    RaiseEvent RmScenariosChange
End Sub

Private Sub Class_Initialize()
    On Error Resume Next
    IsDebug = GetIsDebug()
    m_sDbConnection = ""
    m_sUserGroup = ""
    'm_sDbSupportConnection = ""
    m_sSystemDbConnection = ""
    m_nPriceProviderType = -1
    m_sPriceProviderProgID = ""
    m_nSystemPriceProviderType = -1
    m_nVolatilitySourceType = 0
    m_sVolatilitySourceProgID = ""
    m_nSystemVolatilitySourceType = 0
    m_bChangingDbConnection = False
    m_bChangingPriceProvider = False
    m_bChangingVolatilitySource = False
    PriceProviderIsGroupRequestSupported = False
    m_sUnderlyingComboListAll = ""
    m_sUnderlyingComboList = ""
    m_sStockComboList = ""
    m_sBasketIndexComboList = ""
    m_sUnderlyingWithFuturesComboListAll = ""
    m_sUnderlyingWithFuturesComboList = ""

    m_nOrdersSupport = 0

    m_nTimeZoneBias = 0
    DefTradeType = -1
    
    StockHedgeSlice = 1#
    StockHedgeLotSize = 100
    StockHedgeMaxSize = 10000

    IndexHedgeSlice = 1#
    IndexHedgeLotSize = 100
    IndexHedgeMaxSize = 10000
    
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    'Set EtsSettings = Nothing
'    Set m_ProjectionsShedule = Nothing
    Set m_BatchReports = Nothing
    Set m_BatchReportingShedules = Nothing
End Sub

Public Property Get DbConnection() As String
    On Error Resume Next
    DbConnection = m_sDbConnection
End Property

Public Property Let DbConnection(NewValue As String)
    On Error Resume Next
    m_sDbConnection = NewValue
End Property
Public Property Get IvDbConnection() As String
    On Error Resume Next
    IvDbConnection = m_sIvDbConnection
End Property
Public Property Let IvDbConnection(NewValue As String)
    On Error Resume Next
    m_sIvDbConnection = NewValue
End Property
Public Property Get IvFeedUser() As String
    On Error Resume Next
    IvFeedUser = m_sIvFeedUser
End Property
Public Property Get IvFeedPassword() As String
    On Error Resume Next
    IvFeedPassword = m_sIvFeedPassword
End Property
Public Property Let IvFeedPassword(NewValue As String)
    On Error Resume Next
    m_sIvFeedPassword = NewValue
End Property
Public Property Let IvFeedUser(NewValue As String)
    On Error Resume Next
    m_sIvFeedUser = NewValue
End Property
Public Property Get UserGroup() As String
    On Error Resume Next
    UserGroup = m_sUserGroup
End Property

Public Property Let UserGroup(NewValue As String)
    On Error Resume Next
    m_sUserGroup = NewValue
End Property

'Public Property Get DbSupportConnection() As String
'    On Error Resume Next
'    DbSupportConnection = m_sDbSupportConnection
'End Property

'Public Property Let DbSupportConnection(NewValue As String)
'    On Error Resume Next
'    m_sDbSupportConnection = NewValue
'End Property

'Private Sub EtsSettings_DatabaseConnectionChanged(ByVal bsConnectionString As String)
'    On Error Resume Next
'    If Not m_bChangingDbConnection Then
'        m_sSystemDbConnection = bsConnectionString
'        RaiseEvent SystemDbConnectionChange
'    End If
'End Sub

Public Property Get SystemDbConnection() As String
    On Error Resume Next
    SystemDbConnection = m_sSystemDbConnection
End Property

Public Property Let SystemDbConnection(ByVal NewValue As String)
'    On Error GoTo EH
'    m_bChangingDbConnection = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.DatabaseConnection = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_sSystemDbConnection = NewValue
'
'    m_bChangingDbConnection = False
'    Exit Property
'EH:
'    m_bChangingDbConnection = False
'    gCmn.ErrorHandler "Fail to save DB connection settings."
End Property

Public Property Get PriceProviderType() As Long
    On Error Resume Next
    PriceProviderType = m_nPriceProviderType
End Property

Public Property Let PriceProviderType(ByVal NewValue As Long)
    On Error Resume Next
    m_nPriceProviderType = NewValue
End Property

Public Property Get PriceProviderProgID() As String
    On Error Resume Next
    PriceProviderProgID = m_sPriceProviderProgID
End Property

Public Property Let PriceProviderProgID(ByVal NewValue As String)
    On Error Resume Next
    m_sPriceProviderProgID = NewValue
End Property

'Private Sub EtsSettings_PriceProviderChanged(ByVal lProviderType As Long)
'    On Error Resume Next
'    If Not m_bChangingPriceProvider Then
'        m_nSystemPriceProviderType = lProviderType
'        RaiseEvent SystemPriceProviderChange
'    End If
'End Sub

Public Property Get SystemPriceProviderType() As Long
    On Error Resume Next
    SystemPriceProviderType = m_nSystemPriceProviderType
End Property

Public Property Let SystemPriceProviderType(ByVal NewValue As Long)
'    On Error GoTo EH
'    m_bChangingPriceProvider = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.PriceProvider = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_nSystemPriceProviderType = NewValue
'
'    m_bChangingPriceProvider = False
'    Exit Property
'EH:
'    m_bChangingPriceProvider = False
'    gCmn.ErrorHandler "Fail to save price provider settings."
End Property

Public Property Get TimeZoneBias() As Long
    On Error Resume Next
    TimeZoneBias = m_nTimeZoneBias
End Property

Public Property Get UnderlyingComboListAllWhtFutUnd() As String
    On Error Resume Next
    UnderlyingComboListAllWhtFutUnd = m_sUnderlyingComboListAllWhtFutUnd
End Property

Public Property Get UnderlyingComboListAll() As String
    On Error Resume Next
    UnderlyingComboListAll = m_sUnderlyingComboListAll
End Property

Public Property Get UnderlyingComboList() As String
    On Error Resume Next
    UnderlyingComboList = m_sUnderlyingComboList
End Property

Public Property Get StockComboList() As String
    On Error Resume Next
    StockComboList = m_sStockComboList
End Property

Public Property Get BasketIndexComboList() As String
    On Error Resume Next
    BasketIndexComboList = m_sBasketIndexComboList
End Property

Public Property Get UnderlyingWithFuturesComboListAll() As String
    On Error Resume Next
    UnderlyingWithFuturesComboListAll = m_sUnderlyingWithFuturesComboListAll
End Property

Public Property Get UnderlyingWithFuturesComboList() As String
    On Error Resume Next
    UnderlyingWithFuturesComboList = m_sUnderlyingWithFuturesComboList
End Property

Public Sub FireNewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    RaiseEvent NewUnderlyingAdded(nNewUndID)
End Sub

Public Sub FireNewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    RaiseEvent NewUnderlyingGroupAdded(nNewUndGroupID)
End Sub

Public Sub FireNewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    RaiseEvent NewStrategyAdded(nStrategyID)
End Sub

Public Sub MakeUnderlingComboLists()
    On Error Resume Next
    m_sUnderlyingComboListAll = ""
    m_sUnderlyingComboList = ""
    m_sStockComboList = ""
    m_sUnderlyingWithFuturesComboListAll = ""
    m_sUnderlyingWithFuturesComboList = ""

    MakeUnderlingComboListWhtFutUnd g_UnderlyingAll, m_sUnderlyingComboListAllWhtFutUnd
    MakeUnderlingComboList g_UnderlyingAll, m_sUnderlyingComboListAll
    MakeUnderlingComboList g_Stock, m_sStockComboList
    MakeUnderlingComboList g_BasketIndex, m_sBasketIndexComboList
    MakeUnderlingWithFuturesComboList g_ContractAll, m_sUnderlyingWithFuturesComboListAll
    
    If g_CurTraderID <> 0 Then
        MakeUnderlingComboList g_Underlying, m_sUnderlyingComboList
        MakeUnderlingWithFuturesComboList g_Contract, m_sUnderlyingWithFuturesComboList
    Else
        m_sUnderlyingComboList = m_sUnderlyingComboListAll
        m_sUnderlyingWithFuturesComboList = m_sUnderlyingWithFuturesComboListAll
    End If
End Sub

Private Sub MakeUnderlingComboListWhtFutUnd(ByRef aUndColl As EtsGeneralLib.UndColl, ByRef sUndComboList$)
    On Error Resume Next
    Dim arrUndList() As String
    Dim aUnd As EtsGeneralLib.UndAtom, nIdx&
    
    sUndComboList = ""
    
    If aUndColl.Count > 0 Then
        ReDim arrUndList(0 To aUndColl.Count - 1)
        nIdx = 0&
        
        For Each aUnd In aUndColl
            If aUnd.UndType <> enCtFutUnd Then
                arrUndList(nIdx) = "#" & Trim$(Str$(aUnd.ID)) & ";" & aUnd.Symbol & _
                        vbTab & IIf(aUnd.UndType = enCtStock, "S", "I") & _
                        vbTab & aUnd.ContractName
                nIdx = nIdx + 1
            End If
        Next
        
        sUndComboList = Join$(arrUndList, "|")
        Erase arrUndList
    End If
End Sub

Private Sub MakeUnderlingComboList(ByRef aUndColl As EtsGeneralLib.UndColl, ByRef sUndComboList$)
    On Error Resume Next
    Dim arrUndList() As String
    Dim aUnd As EtsGeneralLib.UndAtom, nIdx&
    
    sUndComboList = ""
    
    If aUndColl.Count > 0 Then
        ReDim arrUndList(0 To aUndColl.Count - 1)
        nIdx = 0&
        
        For Each aUnd In aUndColl
            arrUndList(nIdx) = "#" & Trim$(Str$(aUnd.ID)) & ";" & aUnd.Symbol & _
                        vbTab & IIf(aUnd.UndType = enCtStock, "S", "I") & _
                        vbTab & aUnd.ContractName
            nIdx = nIdx + 1
        Next
        
        sUndComboList = Join$(arrUndList, "|")
        Erase arrUndList
    End If
End Sub
Private Sub MakeIndexComboList(ByRef aIndexColl As EtsGeneralLib.IndexColl, ByRef sUndComboList$)
    On Error Resume Next
    Dim arrUndList() As String
    Dim aUnd As EtsGeneralLib.IndexAtom, nIdx&
    
    sUndComboList = ""
    
    If aIndexColl.Count > 0 Then
        ReDim arrUndList(0 To aIndexColl.Count - 1)
        nIdx = 0&
        
        For Each aUnd In aIndexColl
            arrUndList(nIdx) = "#" & Trim$(Str$(aUnd.ID)) & ";" & aUnd.Symbol
            nIdx = nIdx + 1
        Next
        
        sUndComboList = Join$(arrUndList, "|")
        Erase arrUndList
    End If
End Sub


Private Sub MakeUnderlingWithFuturesComboList(ByRef aContractColl As EtsGeneralLib.EtsContractColl, _
                                                ByRef sUndWithFutComboList$)
    On Error Resume Next
    Dim arrUndWithFutList() As String
    Dim aContract As EtsGeneralLib.EtsContractAtom, nIdx&
    
    sUndWithFutComboList = ""
    
    If aContractColl.Count > 0 Then
        ReDim arrUndWithFutList(0 To aContractColl.Count - 1)
        nIdx = 0&
        
        For Each aContract In aContractColl
            arrUndWithFutList(nIdx) = "#" & Trim$(Str$(aContract.ID)) & ";" & aContract.Symbol & _
                        vbTab & IIf(aContract.ContractType = enCtStock, "S", _
                                IIf(aContract.ContractType = enCtIndex, "I", "F")) & _
                        vbTab & aContract.ContractName
            nIdx = nIdx + 1
        Next
        
        sUndWithFutComboList = Join$(arrUndWithFutList, "|")
        Erase arrUndWithFutList
    End If
End Sub

Public Property Get VolatilitySourceType() As Long
    On Error Resume Next
    VolatilitySourceType = m_nVolatilitySourceType
End Property

Public Property Let VolatilitySourceType(NewValue As Long)
    On Error Resume Next
    m_nVolatilitySourceType = NewValue
End Property

Public Property Get VolatilitySourceProgID() As String
    On Error Resume Next
    VolatilitySourceProgID = m_sVolatilitySourceProgID
End Property

Public Property Let VolatilitySourceProgID(ByVal NewValue As String)
    On Error Resume Next
    m_sVolatilitySourceProgID = NewValue
End Property

'Private Sub EtsSettings_VolatilitySourceChanged(ByVal lVolatilitySourceType As Long)
'    On Error Resume Next
'    If Not m_bChangingVolatilitySource Then
'        m_nSystemVolatilitySourceType = lVolatilitySourceType
'        RaiseEvent SystemVolatilitySourceChange
'    End If
'End Sub

Public Property Get SystemVolatilitySourceType() As Long
    On Error Resume Next
    SystemVolatilitySourceType = m_nSystemVolatilitySourceType
End Property

Public Property Let SystemVolatilitySourceType(NewValue As Long)
'    On Error GoTo EH
'    m_bChangingVolatilitySource = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.VolatilitySource = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_nSystemVolatilitySourceType = NewValue
'
'    m_bChangingVolatilitySource = False
'    Exit Property
'EH:
'    m_bChangingVolatilitySource = False
'    gCmn.ErrorHandler "Fail to save volatility source settings."
End Property

Public Sub FireInternalVolatilityChanged(ByVal sSymbol As String)
    On Error Resume Next
    RaiseEvent InternalVolatilityChanged(sSymbol)
End Sub

Public Property Get ActualTradesEditable() As Boolean
    On Error Resume Next
    ActualTradesEditable = m_bActualTradesEditable
End Property

Public Sub SaveActualTradesEditable(ByVal bActualTradesEditable As Boolean)
    On Error GoTo EH
    
    If m_bActualTradesEditable <> bActualTradesEditable Then
        gDBW.usp_ActTrdEditable_Save IIf(bActualTradesEditable, 1, 0)
        m_bActualTradesEditable = bActualTradesEditable
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to save actual trade editing setting."
End Sub

Public Sub LoadActualTradesEditable()
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    
    m_bActualTradesEditable = False
 
    Set rs = gDBW.usp_ActTrdEditable_Get
    If Not rs.EOF Then
        m_bActualTradesEditable = (ReadLng(rs!iIsActTrdEditable) <> 0)
    End If
    
    Set rs = Nothing
    Exit Sub
EH:
    Set rs = Nothing
    gCmn.ErrorMsgBox g_frmOwner, "Fail to load actual trade editing setting."
End Sub

Public Property Get PriceRoundingRule() As EtsGeneralLib.EtsPriceRoundingRuleEnum
    On Error Resume Next
    PriceRoundingRule = m_enPriceRoundingRule
End Property

Public Sub SavePriceRoundingRule(ByVal enPriceRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum)
    On Error GoTo EH
    
    If m_enPriceRoundingRule <> enPriceRoundingRule Then
        gDBW.usp_PriceRoundingType_Save enPriceRoundingRule
        m_enPriceRoundingRule = enPriceRoundingRule
        RaiseEvent PriceRoundingRuleChange
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to save price rounding type setting."
End Sub

Public Sub LoadPriceRoundingRule()
    On Error GoTo EH
    
    m_enPriceRoundingRule = enPrrRoundNone
 
    m_enPriceRoundingRule = gDBW.usp_PriceRoundingType_Get
    If m_enPriceRoundingRule <> enPrrRoundNone _
        And m_enPriceRoundingRule <> enPrrRoundUp _
        And m_enPriceRoundingRule <> enPrrRoundDown Then
        
        m_enPriceRoundingRule = enPrrRoundNone
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to load price rounding type setting."
End Sub

Public Property Get UndPriceToleranceValue() As Double
    On Error Resume Next
    UndPriceToleranceValue = m_dUndPriceToleranceValue
End Property

Public Sub SaveUndPriceToleranceValue(ByVal dUndPriceToleranceValue As Double)
    On Error GoTo EH
    
    If dUndPriceToleranceValue <= 0# Then
        dUndPriceToleranceValue = 0.01
    ElseIf dUndPriceToleranceValue > 100# Then
        dUndPriceToleranceValue = 100#
    End If
    
    If m_dUndPriceToleranceValue <> dUndPriceToleranceValue Then
        gDBW.usp_UndPriceToleranceValue_Save dUndPriceToleranceValue
        m_dUndPriceToleranceValue = dUndPriceToleranceValue
        RaiseEvent UndPriceToleranceChange
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to save underlying price tolerance setting."
End Sub

Public Sub LoadUndPriceToleranceValue()
    On Error GoTo EH
    
    m_dUndPriceToleranceValue = 5#
 
    m_dUndPriceToleranceValue = gDBW.usp_UndPriceToleranceValue_Get
    If m_dUndPriceToleranceValue <= 0# Then
        m_dUndPriceToleranceValue = 5#
    ElseIf m_dUndPriceToleranceValue > 100# Then
        m_dUndPriceToleranceValue = 100#
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to load underlying price tolerance setting."
End Sub

Public Sub FireNetExposureAUMChange(dVal As Double)
 On Error Resume Next
    RaiseEvent NetExposureAUMChange(dVal)
End Sub

Public Sub FirePriceProfilesChange()
    On Error Resume Next
    RaiseEvent PriceProfilesChange
End Sub

Public Sub FireUnderlyingsParamsChange(ByRef collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    On Error Resume Next
    RaiseEvent UnderlyingsParamsChange(collUpdUnd)
End Sub

Public Sub FireNewTntCntPtyAdded(ByVal nNewCntPtyID As Long)
    On Error Resume Next
    RaiseEvent NewTntCntPtyAdded(nNewCntPtyID)
End Sub

Public Sub FireUseTntMessagingChanged()
    On Error Resume Next
    RaiseEvent UseTntMessagingChanged
End Sub

Public Sub LoadCommissionLowPremiumLimit()
    On Error GoTo EH
    
    m_dCommissionLowPremiumLimit = 0#
    m_dCommissionLowPremiumLimit = gDBW.usp_CommissionLowPremiumLimit_Get
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox g_frmOwner, "Fail to load low premium commission limit setting."
End Sub

Public Property Get CommissionLowPremiumLimit#()
    On Error Resume Next
    CommissionLowPremiumLimit = m_dCommissionLowPremiumLimit
End Property

Public Sub MakeAllParamsDescription(ByRef sDesc$)
    On Error Resume Next
    sDesc = ""

    ' Connections Params
    sDesc = sDesc & "Connections Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "Price Provider: "
    Dim aProviders As PRICEPROVIDERSLib.Providers, aProvider As IProviderData
    Set aProviders = New PRICEPROVIDERSLib.Providers
    aProviders.Initialize
    aProviders.GetProvider m_nPriceProviderType, aProvider
    If Not aProvider Is Nothing Then
        sDesc = sDesc & aProvider.Description & vbCrLf
        Set aProvider = Nothing
    Else
        sDesc = sDesc & "<fail to retrieve info>" & vbCrLf
    End If
    Set aProviders = Nothing

    
    sDesc = sDesc & "Volatility Provider: "
    Dim aVolaSources As VolatilitySourcesLib.VolatilitySources, sName$, sFileName$, sVersion$, sProgID$
    Set aVolaSources = New VolatilitySourcesLib.VolatilitySources
    Err.Clear
    aVolaSources.GetVolatilitySourceInfoByID m_nVolatilitySourceType, sName$, sFileName$, sVersion$, sProgID$
    If Err.Number = 0 Then
        sDesc = sDesc & sName & vbCrLf
    Else
        sDesc = sDesc & "<fail to retrieve info>" & vbCrLf
    End If
    Set aVolaSources = Nothing
    
    ' Calculation Params
    sDesc = sDesc & vbCrLf & "Calculation Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "P&L Calculation Type: " & IIf(PnLCalcType = PNLCT_LTD, "LTD", "Daily") & vbCrLf
    sDesc = sDesc & "Underlying Price Tolerance, %: " & m_dUndPriceToleranceValue * 100 & vbCrLf
    sDesc = sDesc & "Price Rounding: " & IIf(m_enPriceRoundingRule = enPrrRoundNone, "None", IIf(m_enPriceRoundingRule = enPrrRoundDown, "Down", "Up")) & vbCrLf
    sDesc = sDesc & "Use Market Volatility: " & IIf(UseTheoVolatility, "No", "Yes") & vbCrLf
    sDesc = sDesc & "Use Theo Volatility if No Bid: " & IIf(UseTheoNoBid, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Use Theo Volatility if Bad Market Vola: " & IIf(UseTheoNoBid, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Calculation Model: " & CalcModelName(m_CalcModel) & vbCrLf
    
    ' Quotes Params
    sDesc = sDesc & vbCrLf & "Quotes View Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "Bid/Ask Column Click: " & IIf(QuoteBidColBuy <> 0, "Buy/Sell Trade", "Sell/Buy Trade") & vbCrLf
    sDesc = sDesc & "Other Column Click: " & IIf(QuoteOtherColBuy <> 0, "Buy Trade", "Sell Trade") & vbCrLf
    sDesc = sDesc & "Use Colors for Expiries Separation: " & IIf(m_QuoteOptExpirySeparation, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Directly to Realtime Mode: " & IIf(QuoteDirectlyToRealtime, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Market Data Request Type: " & IIf(QuoteReqTypeAlwaysNonGroup, "Always Non-group", "Depends on Price Provider") & vbCrLf
    
    Dim aExch As EtsGeneralLib.ExchAtom
    sDesc = sDesc & "Initial Underlying Visible Exchange: "
    If QuoteUndExchAll Then
        sDesc = sDesc & "<All>" & vbCrLf
    Else
        Set aExch = g_Exch(QuoteDefUndExchID)
        If Not aExch Then
            sDesc = sDesc & aExch.Name & vbCrLf
            Set aExch = Nothing
        Else
            sDesc = sDesc & "<fail to retrieve info>" & vbCrLf
        End If
    End If
    sDesc = sDesc & "Initial Options Visible Exchange: "
    If QuoteOptExchAll Then
        sDesc = sDesc & "<All>" & vbCrLf
    Else
        Set aExch = g_Exch(QuoteDefOptExchID)
        If Not aExch Then
            sDesc = sDesc & aExch.Name & vbCrLf
            Set aExch = Nothing
        Else
            sDesc = sDesc & "<fail to retrieve info>" & vbCrLf
        End If
    End If
    sDesc = sDesc & "Fit to Implied - Use Flat Vola if No Bid: " & IIf(QuoteFitToImpFlatNoBid, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Fit to Implied - Curve Type: " & IIf(QuoteFitToImpCurveType = enFitToImpCurveCalls, "Calls", IIf(QuoteFitToImpCurveType = enFitToImpCurvePuts, "Puts", "OTM")) & vbCrLf
    
    ' Risks Params
    sDesc = sDesc & vbCrLf & "Risks View Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "Bid/Ask Column Click: " & IIf(RiskBidColBuy <> 0, "Buy/Sell Trade", "Sell/Buy Trade") & vbCrLf
    sDesc = sDesc & "Other Column Click: " & IIf(RiskOtherColBuy <> 0, "Buy Trade", "Sell Trade") & vbCrLf
    sDesc = sDesc & "Directly to Realtime Mode: " & IIf(RiskDirectlyToRealtime, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Market Data Request Type: " & IIf(RiskReqTypeAlwaysNonGroup, "Always Non-group", "Depends on Price Provider") & vbCrLf
    
    ' Risk Matrix Params
    sDesc = sDesc & vbCrLf & "Risk Matrix View Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "Time Scenario: " & IIf(MatrixExerciseOptions, "Exercise Options", "Ignore Options With Expiration Passed") & vbCrLf
    sDesc = sDesc & "Market Data Request Type: " & IIf(MatrixReqTypeAlwaysNonGroup, "Always Non-group", "Depends on Price Provider") & vbCrLf
    sDesc = sDesc & "Calculation Model: " & CalcModelName(MatrixCalcModel) & vbCrLf
    
    ' Advanced Params
    sDesc = sDesc & vbCrLf & "Advanced Params" & vbCrLf & String$(70, "-") & vbCrLf
    sDesc = sDesc & "Active Window Realtime: " & IIf(m_ActiveRealTime, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Use Calculation Delay: " & IIf(UseCalcSleep, "Yes", "No") & vbCrLf
    sDesc = sDesc & "Calculation Delay Frequency, steps: " & CalcSleepFreq & vbCrLf
    sDesc = sDesc & "Calculation Delay Time, ms: " & CalcSleepAmt & vbCrLf
    sDesc = sDesc & "Time Zone Bias: " & m_nTimeZoneBias & vbCrLf
    sDesc = sDesc & "Current Trader: "
    Dim aTrader As EtsGeneralLib.EtsMmEntityAtom
    If g_CurTraderID <> 0 Then
        Set aTrader = g_Trader(CStr(g_CurTraderID))
        If Not aTrader Is Nothing Then
            sDesc = sDesc & aTrader.Name & vbCrLf
            Set aTrader = Nothing
        Else
            sDesc = sDesc & "<fail to retrieve info>" & vbCrLf
        End If
    Else
        sDesc = sDesc & "<All Traders>" & vbCrLf
    End If


    ' Price Profiles
    sDesc = sDesc & vbCrLf & "Price Profiles" & vbCrLf & String$(70, "-") & vbCrLf
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    
    For Each aPrProf In g_PriceProfile
        sDesc = sDesc & "Name: " & aPrProf.Name & vbCrLf
        sDesc = sDesc & "Description: " & aPrProf.Description & vbCrLf
        If aPrProf.IsOptionProfile Then
            sDesc = sDesc & "Type: Option" & vbCrLf
            sDesc = sDesc & "Use Bid/Ask for Long/Short Positions for P&L: " & IIf(aPrProf.UseMidMarketForPnL, "No", "Yes") & vbCrLf
            sDesc = sDesc & "If No Bid or Ask then: " & IIf(aPrProf.BadOptSinglePriceRule = enObsprReplaceWithLast, "Replace missing Bid or Ask with Last", _
                                                        IIf(aPrProf.BadOptSinglePriceRule = enObsprReplaceWithOpposite, "Replace missing Bid/Ask with Ask/Bid", _
                                                        IIf(aPrProf.BadOptSinglePriceRule = enObsprReplaceWithZero, "Replace missing Bid/Ask with Zero", _
                                                        "Replace missing Bid/Ask with last Theo"))) & vbCrLf
            sDesc = sDesc & "If No Bid and Ask then: " & IIf(aPrProf.BadOptBothPriceRule = enObbprUseLast, "Use Last", "Use last Theo") & vbCrLf
        Else
            sDesc = sDesc & "Type: Stock" & vbCrLf
            If aPrProf.AlwaysUseLast Then
                sDesc = sDesc & "Always Use Last: Yes" & vbCrLf
            Else
                sDesc = sDesc & "Always Use Last: No" & vbCrLf
                sDesc = sDesc & "Use Bid/Ask for Long/Short Positions for P&L: " & IIf(aPrProf.UseMidMarketForPnL, "No", "Yes") & vbCrLf
                sDesc = sDesc & "Use Last Price instead Bid and Ask if Bid or Ask is not within Tolerance range (or no Bid or Ask): " & _
                                IIf(aPrProf.BadUndPriceRule = enUbprCheckTolerance, "Yes", "No") & vbCrLf
            End If
        End If
        sDesc = sDesc & vbCrLf
    Next
End Sub

Private Sub LoadCurrentUserPrivateFolderPath()
    On Error Resume Next
    Dim sPath$, nLen&: nLen = -1
    Dim aFSO As Scripting.FileSystemObject
    
    Set aFSO = New Scripting.FileSystemObject
    If Not aFSO Is Nothing Then
        sPath = String$(2048, 0)
        
        If SHGetSpecialFolderPath(0, sPath, CSIDL_PERSONAL, 0) Then
            nLen = InStr(1, sPath, Chr$(0), vbBinaryCompare) - 1
            If nLen >= 0 Then
                sPath = Left$(sPath, nLen) & "\ETS"
                If Not aFSO.FolderExists(sPath) Then
                    Err.Clear
                    aFSO.CreateFolder sPath
                    If Err.Number = 0 Then CurrentUserPrivateFolder = sPath
                Else
                    CurrentUserPrivateFolder = sPath
                End If
            End If
        End If
        
        Set aFSO = Nothing
    End If
End Sub


Private Sub LoadResource(sLibrary As String)
   On Error Resume Next
   
    m_hResource = LoadLibrary(sLibrary)
    If m_hResource = 0 Then
        m_hResource = LoadLibrary("IVRmRes.dll")
    End If
End Sub

Private Sub FreeResource()
   On Error Resume Next
   If m_hResource <> 0 Then FreeLibrary m_hResource
End Sub

Public Property Get Splash() As Picture
    On Error Resume Next
    If m_hResource <> 0 Then
        Dim hBitmap  As Long
        hBitmap = LoadBitmap(m_hResource, "Splash_IVRm")
        If hBitmap <> 0 Then
            Set Splash = LoadBitmapFromResource(hBitmap)
        End If
    End If
End Property

Private Function GetWindowText(enWindow As WindowCode) As String
    Dim sResourceCode$
    On Error Resume Next
    
        Select Case enWindow
            Case WND_MAIN:
                sResourceCode = "Main"
            Case WND_TRADES
                sResourceCode = "Trades"
            Case WND_QUOTES
                sResourceCode = "Quotes"
            Case WND_EXERCISE
                sResourceCode = "Manage"
            Case WND_GREEKS
                sResourceCode = "Greeks"
            Case WND_HEDGESUMARY
                sResourceCode = "HedgeSummary"
            Case WND_INDEXHEDGE
                sResourceCode = "IndexHedge"
            Case WND_ORDERS
                sResourceCode = "Orders"
            Case WND_PNLPROJECTIONS
                sResourceCode = "PnLProjections"
            Case WND_QUOTATION
                sResourceCode = "Quotation"
            Case WND_REPORTS
                sResourceCode = "Reports"
            Case WND_RISKMATRIX
                sResourceCode = "Matrix"
            Case WND_RISK
                sResourceCode = "Risks"
            Case WND_UNDERLYING
                sResourceCode = "Underlyings"
            Case WND_VOLAANALYCIS
                sResourceCode = "VolaAnalysis"
            Case WND_VOLAMONITOR
                sResourceCode = "VolaMonitor"
        End Select
GetWindowText = sResourceCode
End Function

Public Property Get Icon(enWindow As WindowCode) As IPictureDisp
  On Error Resume Next
    Dim sResourceCode As String
    
    If m_hResource <> 0 Then _
        sResourceCode = GetWindowText(enWindow)
    
    If Len(sResourceCode) > 0 Then
        Set Icon = LoadIconFromResource(LoadIcon(m_hResource, sResourceCode))
    End If
End Property

Public Sub SetIcon(enWindow As WindowCode, hWnd As Long, Optional ByVal bSetAsAppIcon As Boolean = True)
  On Error Resume Next
    Dim sResourceCode As String
    
    If m_hResource <> 0 Then _
        sResourceCode = GetWindowText(enWindow)
    
    If Len(sResourceCode) > 0 Then
        Set_Icon hWnd, sResourceCode, m_hResource, bSetAsAppIcon
    End If
End Sub


Public Sub Terminate()
    FreeResource
End Sub

Public Property Get BatchReports() As Collection
    Set BatchReports = m_BatchReports
End Property

Public Property Let BatchReports(aSrc As Collection)
    Set m_BatchReports = aSrc
End Property

Public Property Get BatchReportingShedules() As Collection
    Set BatchReportingShedules = m_BatchReportingShedules
End Property

Public Property Let BatchReportingShedules(aSrc As Collection)
    Set m_BatchReportingShedules = aSrc
End Property

Public Sub LoadBatchShedule()
    On Error Resume Next
    
    Set m_BatchReportingShedules = Nothing
    Set m_BatchReportingShedules = New Collection
    
    Dim rs As ADODB.Recordset
    Set rs = gDBW.usp_BR_Shedule_Get()
    
    If rs Is Nothing Then Exit Sub
    
    Dim aShedule As clsBatchReportingShedule
    While Not rs.EOF
        Set aShedule = New clsBatchReportingShedule
        
        aShedule.ID = ReadLng(rs!iID)
        aShedule.ExecTime = CDate(rs!dtTime)
        aShedule.Enabled = CBool(ReadByte(rs!tiStatus))
        aShedule.LastRun = CDate(rs!dtLastRun)
        
        m_BatchReportingShedules.Add aShedule, Trim(CStr(aShedule.ID))
        Set aShedule = Nothing
    
        rs.MoveNext
    Wend
    
End Sub

Public Sub SaveBatchShedule()
    On Error Resume Next

    Dim aShedule As clsBatchReportingShedule

    For Each aShedule In m_BatchReportingShedules
        gDBW.usp_BR_Shedule_Save aShedule.ExecTime, aShedule.Enabled, aShedule.ID
        gDBW.usp_BR_Shedule_Complete aShedule.ID, aShedule.LastRun
    Next
End Sub

Public Sub LoadBatchReports()
    On Error Resume Next
    
    Set m_BatchReports = Nothing
    Set m_BatchReports = New Collection
    
    Dim rs As ADODB.Recordset
    Set rs = gDBW.usp_BR_Reports_Get()
    
    If rs Is Nothing Then Exit Sub
    
    Dim aReport As clsBatchReport
    While Not rs.EOF
        Set aReport = New clsBatchReport
        
        aReport.ID = ReadLng(rs!iID)
        aReport.UserName = ReadStr(rs!vcUserName)
        aReport.IsActive = CBool(ReadByte(rs!tiStatus))
        aReport.LayoutFilePath = ReadStr(rs!vcLayoutFile)
        aReport.GroupID = ReadLng(rs!iGroupID)
        aReport.TraderID = ReadLng(rs!iTraderID)
        aReport.strategyID = ReadLng(rs!iStrategyID)
        aReport.TraderGroupID = ReadLng(rs!iTraderGroupID)
        aReport.IsPosExpanded = CBool(ReadByte(rs!tiPositions))
        aReport.LastRun = CDate(rs!dtLastRun)
        aReport.ResultFile = ReadStr(rs!vcResultFile)
        aReport.ResultFilePath = ReadStr(rs!vcFilePath)
        aReport.FileNameHeader = ReadStr(rs!vcFileNameHeader)
        aReport.ReportFormat = ReadLng(rs!iReportFormat)
        
        aReport.UseDefaultLayout = CBool(aReport.LayoutFilePath = "DefaultRiskView")
        
        aReport.Init
        
        m_BatchReports.Add aReport, CStr(aReport.ID)
        Set aReport = Nothing
    
        rs.MoveNext
    Wend
    
End Sub

Public Sub SaveBatchReports()
    On Error Resume Next

    Dim aReport As clsBatchReport
    Dim rs As ADODB.Recordset

    For Each aReport In m_BatchReports
        Select Case aReport.Status
            Case SS_NEW
                gDBW.usp_BR_Report_Save aReport.UserName, _
                                        aReport.IsActive, _
                                        aReport.LayoutFilePath + aReport.LayoutFile, _
                                        aReport.GroupID, _
                                        aReport.TraderID, _
                                        aReport.strategyID, _
                                        aReport.TraderGroupID, _
                                        aReport.IsPosExpanded, _
                                        aReport.ResultFilePath, _
                                        aReport.LastRun, _
                                        aReport.ResultFile, _
                                        aReport.FileNameHeader, _
                                        aReport.ReportFormat
                aReport.Status = SS_COMMON
            Case SS_UPDATED
                gDBW.usp_BR_Report_Save aReport.UserName, _
                                        aReport.IsActive, _
                                        aReport.LayoutFilePath + aReport.LayoutFile, _
                                        aReport.GroupID, _
                                        aReport.TraderID, _
                                        aReport.strategyID, _
                                        aReport.TraderGroupID, _
                                        aReport.IsPosExpanded, _
                                        aReport.ResultFilePath, _
                                        aReport.LastRun, _
                                        aReport.ResultFile, _
                                        aReport.FileNameHeader, _
                                        aReport.ReportFormat, _
                                        aReport.ID
                aReport.Status = SS_COMMON
            Case SS_DELETED
                gDBW.usp_BR_Report_Del aReport.ID
        End Select
    Next

End Sub
