VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmMarketStructure 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Market Structure Import"
   ClientHeight    =   2430
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6015
   Icon            =   "frmMarketStructure.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2430
   ScaleWidth      =   6015
   StartUpPosition =   1  'CenterOwner
   Begin VB.Timer tmrControl 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   1320
      Top             =   0
   End
   Begin VB.CommandButton btnControl 
      Cancel          =   -1  'True
      Caption         =   "Control"
      Height          =   315
      Left            =   4800
      TabIndex        =   0
      Top             =   120
      Width           =   1095
   End
   Begin VB.TextBox txtLog 
      Height          =   1455
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   2
      Top             =   840
      Width           =   5775
   End
   Begin MSComctlLib.ProgressBar pbProgress 
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   480
      Width           =   5775
      _ExtentX        =   10186
      _ExtentY        =   556
      _Version        =   393216
      Appearance      =   1
      Scrolling       =   1
   End
   Begin VB.Label lblPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "Prompt"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   150
      UseMnemonic     =   0   'False
      Width           =   4575
   End
End
Attribute VB_Name = "frmMarketStructure"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const MS_MAX_OPTIONS& = 300&
Private Const OTM_IVTYPE& = 2&

Private Enum MsStateEnum
    enMsStart
    enMsRequestStock
    enMsRequestOptions
    enMsRequestDefVola
    enMsRequestVolaBands
    enMsRequestSkewKurt
    enMsRequestIdxComps
    enMsSaveData
    enMsError
    enMsComplete
End Enum

Private m_enState As MsStateEnum


Private m_mbResult As VbMsgBoxResult
Private m_hComplete As Long

Private WithEvents MsProvider As PRICEPROVIDERSLib.StructureProviderEx
Attribute MsProvider.VB_VarHelpID = -1
Private WithEvents IvDataLoader As DHIVFeeder5.IvDataLoader
Attribute IvDataLoader.VB_VarHelpID = -1

Private m_Und As clsMsUndAtom
Private m_Opt As clsMsOptColl
Private m_Vola As clsMsVolaColl
Private m_VolaBand As clsMsVolaBandColl
Private m_bWithIvData As Boolean

Public Function Execute(ByVal sNewSymbol$, ByRef frmOwner As Form) As Long
    On Error GoTo EH
    Dim bRestoreEnabled As Boolean
    Dim sPosition As String
    m_mbResult = vbCancel
    Execute = 0
    
    m_enState = enMsStart
    m_bWithIvData = True
    sPosition = "1"
    
    Set m_Und = New clsMsUndAtom
    sPosition = "2"
    Set m_Und.Dividend = New EtsGeneralLib.EtsIndexDivAtom
    sPosition = "3"
    
    m_Und.Symbol = sNewSymbol
    sPosition = "4"
    
    
    Set m_Opt = New clsMsOptColl
    sPosition = "5"
    Set m_Vola = New clsMsVolaColl
    sPosition = "6"
    Set m_VolaBand = New clsMsVolaBandColl
    sPosition = "7"
    
    Load Me
    sPosition = "8"
    txtLog.Text = ""
    UpdateControls
    sPosition = "9"

    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set MsProvider = New PRICEPROVIDERSLib.StructureProviderEx
    sPosition = "10"
    
    Set aPT = MsProvider
    sPosition = "11"
    
    aPT.Type = g_Params.PriceProviderType
    sPosition = "12" & CStr(g_Params.PriceProviderType)
    
    Set aPT = Nothing
    sPosition = "14"
    
    MsProvider.Connect
    sPosition = "15"
    
    Set IvDataLoader = New DHIVFeeder5.IvDataLoader
    sPosition = "16"

    If Not frmOwner Is Nothing Then
        sPosition = "17"
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            sPosition = "18"
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            sPosition = "19"
            
            Me.Show vbModeless, frmOwner
        
            btnControl_Click
        
            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLEVENTS) <> WAIT_OBJECT_0
                Sleep 0
                DoEvents
            Wend
            
            If m_hComplete <> 0 Then
                CloseHandle m_hComplete
                m_hComplete = 0
            End If
            
            If bRestoreEnabled Then frmOwner.Enabled = True
        Else
            btnControl_Click
            Me.Show vbModal
        End If
    Else
        btnControl_Click
        Me.Show vbModal
    End If
    
    Set MsProvider = Nothing
    IvDataLoader.CancelTask
    Set IvDataLoader = Nothing
    
    If m_mbResult = vbOK Then Execute = m_Und.ID
    
    Unload Me
    Set m_Und = Nothing
    Set m_Opt = Nothing
    Set m_Vola = Nothing
    Set m_VolaBand = Nothing
    Exit Function
EH:
    gCmn.ErrorMsgBox Me, "Fail to open market structure import window: " & sPosition
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Set MsProvider = Nothing
    Set IvDataLoader = Nothing
    Set m_Und = Nothing
    Set m_Opt = Nothing
    Set m_Vola = Nothing
    Set m_VolaBand = Nothing
    Unload Me
End Function

Private Sub btnControl_Click()
    On Error Resume Next
    Select Case m_enState
        Case enMsStart
            tmrControl.Enabled = True
            AddLogMsg "Start market structure import."
            
        Case enMsRequestStock, enMsRequestOptions, enMsRequestDefVola, enMsRequestVolaBands, _
            enMsRequestSkewKurt, enMsRequestIdxComps, enMsSaveData
            
            m_enState = enMsError
            AddLogMsg "Market structure import was cancelled."
            
        Case enMsError, enMsComplete
            m_mbResult = IIf(m_enState = enMsComplete, vbOK, vbCancel)
            
            If m_hComplete <> 0 Then
                SetEvent m_hComplete
            Else
                Me.Hide
            End If
    End Select
    
    UpdateControls
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_hComplete <> 0 Then SetEvent m_hComplete
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If m_hComplete <> 0 Then
        CloseHandle m_hComplete
        m_hComplete = 0
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Public Sub AddLogMsg(ByVal sMsg As String)
    On Error Resume Next
    If Len(txtLog.Text) > 64000 Then txtLog.Text = Right$(txtLog.Text, 32000)
    txtLog.SelStart = Len(txtLog.Text)
    txtLog.SelText = sMsg & vbCrLf
    txtLog.SelStart = Len(txtLog.Text)
End Sub

Private Sub MsProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    If m_enState <> enMsRequestOptions And m_enState <> enMsRequestStock Then Exit Sub
    
    If ReqType = enRequestOptions Or ReqType = enRequestStock Then
        If ReqType = enRequestOptions Then
            Select Case ErrorNumber
                Case enNoAccessForData, enNoDataAvailableForSymbol, enSymbolNotSupported
                    pbProgress.Value = pbProgress.Max
                    AddLogMsg "Options info request succeeded. " & Description
                    tmrControl.Enabled = True
                    Exit Sub
                    
                Case Else
                    AddLogMsg "Fail to request options info. " & Description
            End Select
        
        Else
            AddLogMsg "Fail to request stock info. " & Description
        End If
    Else
        AddLogMsg "General market structure request error: " & Description
    End If
    
    m_enState = enMsError
    UpdateControls
    tmrControl.Enabled = True
End Sub


Private Sub MsProvider_OnOption(Params As PRICEPROVIDERSLib.OptionParams, Results As PRICEPROVIDERSLib.OptionResultsEx, ByVal IsLastOption As Boolean)
    On Error Resume Next
    If m_enState <> enMsRequestOptions Then Exit Sub
    If Params.UnderlyingSymbol <> m_Und.Symbol Then Exit Sub
    Dim aOpt As clsMsOptAtom
    
    If Not IsLastOption Then
        Set aOpt = m_Opt(Results.Series)
        If aOpt Is Nothing Then
            Set aOpt = m_Opt.Add(Results.Series)
            aOpt.ID = 0
            aOpt.Symbol = Results.Series
            aOpt.Expiry = Results.ExpirationDate
            aOpt.OptType = IIf(Results.OptionType = enCall, enOtCall, enOtPut)
            aOpt.Strike = Round(Results.StrikePrice, STRIKE_DECIMALS_COUNT)
            aOpt.PriceAsk = Results.Ask
            aOpt.PriceBid = Results.Bid
            aOpt.Volume = Results.Volume
            aOpt.OpenInterest = Results.OpenInterest
            aOpt.LotSize = IIf(Results.LotSize > 0, Results.LotSize, 100)
        
            If pbProgress.Value < pbProgress.Max Then IncProgress pbProgress
        End If
        Set aOpt = Nothing
    Else
        pbProgress.Value = pbProgress.Max
        AddLogMsg "Options info request succeeded. " & m_Opt.Count & " options was received."
        tmrControl.Enabled = True
    End If

End Sub


Private Sub MsProvider_OnStock(Params As PRICEPROVIDERSLib.StockParams, Results As PRICEPROVIDERSLib.StockResultsEx)
    On Error Resume Next
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    
    If m_enState <> enMsRequestStock Then Exit Sub
    If Params.Stock <> m_Und.Symbol Then Exit Sub
    
    m_Und.UndType = IIf(Results.StockType = enStock, enCtStock, enCtIndex)
    m_Und.Description = Left$(Trim$(Results.Description), 255)
    If m_Und.ID = 0 Then
        m_Und.IsAmerican = (Results.StockType = enStock)
        m_Und.IsHTB = False
        m_Und.IsTraderContract = (g_CurTraderID = 0)
        
        If m_Und.UndType = enCtStock Then
            Set m_Und.UndPriceProfile = g_Main.DefStkPriceProfile
            
        Else
            Set m_Und.UndPriceProfile = g_Main.DefIdxPriceProfile
        End If
    
        Set m_Und.OptPriceProfile = g_Main.DefOptPriceProfile
    End If

    'm_Und.Yield = Results.

    Set aDiv = m_Und.Dividend
    aDiv.DivFreq = Results.DivFreq
    If aDiv.DivFreq <> 0 Then
        aDiv.DivDate = Results.DivLastDate
        aDiv.DivAmt = Results.DivAmount
    End If
    Set aDiv = Nothing

    m_Und.PriceBid = Results.Bid
    m_Und.PriceAsk = Results.Ask
    m_Und.PriceOpen = Results.OpenPrice
    m_Und.PriceClose = Results.ClosePrice
    m_Und.PriceLow = Results.Low
    m_Und.PriceHigh = Results.High
    m_Und.Volume = Results.Volume
    m_Und.LotSize = 1 'IIf(Results.LotSize > 0, Results.LotSize, 1)

    If pbProgress.Value < pbProgress.Max Then IncProgress pbProgress

    AddLogMsg "Stock info request succeeded."
    tmrControl.Enabled = True

End Sub

Private Sub tmrControl_Timer()
    On Error Resume Next
    tmrControl.Enabled = False

    Select Case m_enState
        Case enMsStart
            RequestStockInfo
            
        Case enMsRequestStock
            RequestOptionsInfo
        
        Case enMsRequestOptions
            If m_bWithIvData Then
                RequestDefVola
            Else
                SaveData
            End If
            
        Case enMsRequestDefVola
            If m_bWithIvData Then
                RequestVolaBands
            Else
                SaveData
            End If
                
        Case enMsRequestVolaBands
            If m_bWithIvData Then
                RequestSkewKurt
            Else
                SaveData
            End If
        
        Case enMsRequestSkewKurt
            If m_bWithIvData Then
                RequestIdxComps
            Else
                SaveData
            End If
        
        Case enMsRequestIdxComps
            SaveData
        
        Case enMsSaveData
            m_enState = enMsComplete
            UpdateControls
            tmrControl.Enabled = True
        
        Case enMsError
            lblPrompt.Caption = "Failed."
            AddLogMsg "Market structure import failed."
        
        Case enMsComplete
            lblPrompt.Caption = "Completed."
            AddLogMsg "Market structure import completed."
            btnControl_Click
            
    End Select
End Sub

Private Sub RequestStockInfo()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.StockParams
    
    m_enState = enMsRequestStock
    UpdateControls
    
    lblPrompt.Caption = "Request stock info for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    pbProgress.Min = 0
    pbProgress.Max = MS_MAX_OPTIONS + 1
    pbProgress.Value = 0
    
    aParam.Exchange = ""
    aParam.Stock = m_Und.Symbol
    
    MsProvider.RequestStock aParam
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request stock info. " & Err.Description
    m_enState = enMsError
    UpdateControls
    tmrControl.Enabled = True
End Sub

Private Sub RequestOptionsInfo()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.OptionParams
    
    m_enState = enMsRequestOptions
    UpdateControls
    
    lblPrompt.Caption = "Request options info for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    
    aParam.Exchange = ""
    aParam.UnderlyingSymbol = m_Und.Symbol
    
    MsProvider.RequestOptions aParam
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request stock info. " & Err.Description
    m_enState = enMsError
    UpdateControls
    tmrControl.Enabled = True
End Sub

Private Sub RequestDefVola()
    On Error GoTo EH
    
    m_enState = enMsRequestDefVola
    lblPrompt.Caption = "Request default volatility for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    UpdateControls
    pbProgress.Min = 0
    pbProgress.Max = MS_MAX_OPTIONS / 2
    pbProgress.Value = 0
    
    With IvDataLoader
        .IndexListClear
        .StocksListClear
        
        If m_Und.UndType = enCtStock Then
            .StocksListAddSymbol m_Und.Symbol
        Else
            .IndexListAddSymbol m_Und.Symbol
        End If
        
        .LoadSymbolDefaultIVSurface
        
        pbProgress.Value = pbProgress.Max
        
        AddLogMsg "Request default volatility succeeded."
        
        tmrControl.Enabled = True
    End With
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request default volatility. " & Err.Description
    
    If gCmn.MyMsgBox(Me, "Do you wish to continue without default volatility?", vbQuestion + vbYesNo) <> vbYes Then
        m_enState = enMsError
        UpdateControls
    End If
    tmrControl.Enabled = True
End Sub

Private Sub RequestVolaBands()
    On Error GoTo EH
    
    m_enState = enMsRequestVolaBands
    lblPrompt.Caption = "Request IV bands for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    UpdateControls
    pbProgress.Min = 0
    pbProgress.Max = MS_MAX_OPTIONS / 2
    pbProgress.Value = 0
    
    With IvDataLoader
        .Tenor = g_nDefaultTerm
        .IndexListClear
        .StocksListClear
        
        If m_Und.UndType = enCtStock Then
            .StocksListAddSymbol m_Und.Symbol
        Else
            .IndexListAddSymbol m_Und.Symbol
        End If
        
        .LoadSymbolIVBands
        
        pbProgress.Value = pbProgress.Max
        
        AddLogMsg "Request IV bands succeeded."
        
        tmrControl.Enabled = True
    End With
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request IV bands . " & Err.Description
    
    If gCmn.MyMsgBox(Me, "Do you wish to continue without IV bands?", vbQuestion + vbYesNo) <> vbYes Then
        m_enState = enMsError
        UpdateControls
    End If
    tmrControl.Enabled = True
End Sub

Private Sub RequestSkewKurt()
    On Error GoTo EH
    
    m_enState = enMsRequestSkewKurt
    lblPrompt.Caption = "Request skew & kurtosis info for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    UpdateControls
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    With IvDataLoader
        .Tenor = 180
        .IndexListClear
        .StocksListClear
        
        If m_Und.UndType = enCtStock Then
            .StocksListAddSymbol m_Und.Symbol
        Else
            .IndexListAddSymbol m_Und.Symbol
        End If
        
        .LoadSymbolSkewKurtsis
        
        pbProgress.Value = pbProgress.Max
        
        AddLogMsg "Request skew & kurtosis info succeeded."
        
        tmrControl.Enabled = True
    End With
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request skew & kurtosis info. " & Err.Description
    
    If gCmn.MyMsgBox(Me, "Do you wish to continue without skew & kurtosis info?", vbQuestion + vbYesNo) <> vbYes Then
        m_enState = enMsError
        UpdateControls
    End If
    tmrControl.Enabled = True
End Sub

Private Sub RequestIdxComps()
    On Error GoTo EH
    Dim aIdx As EtsGeneralLib.IndexAtom
    
    m_enState = enMsRequestIdxComps
    If m_Und.UndType = enCtStock Then
        lblPrompt.Caption = "Request beta info for " & m_Und.Symbol & "..."
        lblPrompt.Refresh
        UpdateControls
        pbProgress.Min = 0
        pbProgress.Max = 100
        pbProgress.Value = 0
        
        m_Und.IdxComp.Clear
        
        With IvDataLoader
            .CancelTask
            
            .Tenor = 180
            .IndexListClear
            .StocksListClear
            
            If g_Index.Count > 0 Then
                For Each aIdx In g_Index
                    If aIdx.Symbol = "DJX" _
                        Or aIdx.Symbol = "OEX" _
                        Or aIdx.Symbol = "SPX" _
                        Or aIdx.Symbol = "NDX" Then
                        
                        .IndexListAddSymbol aIdx.Symbol
                    End If
                Next
                
                .StocksListAddSymbol m_Und.Symbol
                .LoadStockIndexBeta
            
                pbProgress.Value = pbProgress.Max
                
                AddLogMsg "Request beta info succeeded."
            Else
                AddLogMsg "No indices for request beta info."
            End If
        End With
    End If
    
    tmrControl.Enabled = True
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to request beta info. " & Err.Description
    
    If gCmn.MyMsgBox(Me, "Do you wish to continue without beta info?", vbQuestion + vbYesNo) <> vbYes Then
        m_enState = enMsError
        UpdateControls
    End If
    tmrControl.Enabled = True
End Sub

Private Sub IvDataLoader_Progress(ByVal ProgressPercent As Long)
    On Error Resume Next
    pbProgress.Value = ProgressPercent
End Sub

Private Sub IvDataLoader_ProgressStart()
    On Error Resume Next
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
End Sub

Private Sub IvDataLoader_SymbolIVBandPoint(ByVal Symbol As String, ByVal StrikeMoneyness As Long, ByVal ExpPeriod As Long, ByVal IvBandLow As Double, ByVal IVBandHigh As Double)
    On Error Resume Next
    If m_enState <> enMsRequestVolaBands Then Exit Sub
    If Symbol <> m_Und.Symbol Then Exit Sub
    
    Dim sKey$, aVolaBand As clsMsVolaBandAtom
    sKey = CStr(ExpPeriod) & "_" & CStr(StrikeMoneyness)
    
    Set aVolaBand = m_VolaBand(sKey)
    If aVolaBand Is Nothing Then
        Set aVolaBand = m_VolaBand.Add(sKey)
    End If
    
    aVolaBand.ExpiryPeriod = ExpPeriod
    aVolaBand.StrikeMoneyness = (100 + StrikeMoneyness) / 100
    aVolaBand.IvBandLow = IvBandLow
    aVolaBand.IvBandHi = IVBandHigh
    If pbProgress.Value < pbProgress.Max Then IncProgress pbProgress
    
    Set aVolaBand = Nothing
End Sub

Private Sub IvDataLoader_SymbolIVPoint(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double)
    On Error Resume Next
    If m_enState <> enMsRequestDefVola Then Exit Sub
    If Symbol <> m_Und.Symbol Then Exit Sub
    
    Dim sKey$, aVola As clsMsVolaAtom
    sKey = UCase$(Format$(Expiration, "YYMMM")) & Trim$(CStr(Strike))
    
    Set aVola = m_Vola(sKey)
    If aVola Is Nothing Then
        Set aVola = m_Vola.Add(sKey)
    End If
    
    aVola.Expiry = Expiration
    aVola.Strike = Round(Strike, STRIKE_DECIMALS_COUNT)
    aVola.Vola = Vola * 100#
    If pbProgress.Value < pbProgress.Max Then IncProgress pbProgress
    
    Set aVola = Nothing
End Sub

Private Sub IvDataLoader_SymbolSkewKurtsis(ByVal Symbol As String, ByVal Skew As Double, ByVal Kurt As Double)
    On Error Resume Next
    If m_enState <> enMsRequestSkewKurt Then Exit Sub
    If Symbol <> m_Und.Symbol Then Exit Sub
    
    m_Und.Skew = Skew
    m_Und.Kurt = Kurt
End Sub

Private Sub IvDataLoader_StockIndexBeta(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Beta As Double)
    On Error Resume Next
    If m_enState <> enMsRequestIdxComps Then Exit Sub
    If StockSymbol <> m_Und.Symbol Then Exit Sub
    
    Dim aIdxComp As EtsGeneralLib.IndexCompAtom, aIdx As EtsGeneralLib.UndAtom
    
    Set aIdx = g_UnderlyingAll.BySortKey(IndexSymbol)
    If Not aIdx Is Nothing Then
        
        Set aIdxComp = m_Und.IdxComp(aIdx.ID)
        If aIdxComp Is Nothing Then
            Set aIdxComp = m_Und.IdxComp.Add(aIdx.ID)
            aIdxComp.ID = aIdx.ID
        End If
        
        aIdxComp.Beta = Beta
        
        If pbProgress.Value < pbProgress.Max Then IncProgress pbProgress
        
        Set aIdxComp = Nothing
        Set aIdx = Nothing
    End If
End Sub

Private Sub SaveData()
    On Error GoTo EH
    m_enState = enMsSaveData
    UpdateControls
    
    lblPrompt.Caption = "Save market structure info for " & m_Und.Symbol & "..."
    lblPrompt.Refresh
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = m_Opt.Count + m_Und.IdxComp.Count + m_Vola.Count + m_VolaBand.Count + 1
    
    SaveUnderlyingData
    If m_enState <> enMsSaveData Then Exit Sub
    
    SaveOptionsData
    If m_enState <> enMsSaveData Then Exit Sub
    
    SaveIvData
    If m_enState <> enMsSaveData Then Exit Sub
    
    AdjustGlobalDataStorage
    
    AddLogMsg "Save market structure info succeeded."
    tmrControl.Enabled = True
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to save market structure info. " & Err.Description
    
    m_enState = enMsError
    UpdateControls
    tmrControl.Enabled = True
End Sub

Private Sub SaveUnderlyingData()
    On Error GoTo EH
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Set aDiv = m_Und.Dividend

    If m_Und.UndType = enCtStock Then
        m_Und.ID = gDBW.usp_Stock_Save(IIf(m_Und.ID = 0, Null, m_Und.ID), Null, m_Und.Symbol, _
                                    m_Und.Description, IIf(m_Und.ID = 0, 0, Null), Null, Null, Null, _
                                    IIf(m_Und.ID = 0, 0, Null), Null, Null, Null, IIf(m_Und.ID = 0, IIf(m_Und.IsAmerican, 1, 0), Null), Null, 0, Null, _
                                    IIf(m_bWithIvData, m_Und.Skew, Null), IIf(m_bWithIvData, m_Und.Kurt, Null), Null, Null, _
                                    1, m_Und.UndPriceProfile.ID, m_Und.OptPriceProfile.ID)
                                    
    Else
        m_Und.ID = gDBW.usp_Index_Save(IIf(m_Und.ID = 0, Null, m_Und.ID), m_Und.Symbol, _
                                    m_Und.Description, Null, IIf(m_Und.ID = 0, 0, Null), _
                                    IIf(m_Und.ID = 0, IIf(m_Und.IsAmerican, 1, 0), Null), Null, 0, Null, Null, _
                                    IIf(m_bWithIvData, m_Und.Skew, Null), IIf(m_bWithIvData, m_Und.Kurt, Null), _
                                    1, Null, Null, Null, m_Und.UndPriceProfile.ID, m_Und.OptPriceProfile.ID)
    End If
    
    gDBW.usp_Stock_Import m_Und.ID, IIf(m_Und.UndType = enCtStock, 1, 0), m_Und.Description, aDiv.DivFreq, aDiv.DivAmt, _
                                    IIf(aDiv.DivDate >= #1/1/1990#, aDiv.DivDate, Null), m_Und.PriceAsk, m_Und.PriceBid, _
                            m_Und.PriceOpen, m_Und.PriceClose, m_Und.PriceLow, m_Und.PriceHigh, m_Und.Volume, Null
    
    IncProgress pbProgress
    DoEvents
    If m_enState <> enMsSaveData Then Exit Sub
    
    gDBW.usp_EodMsUnderlyingCache_Save m_Und.ID
    
    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to save market structure info. " & Err.Description
    
    m_enState = enMsError
    UpdateControls
    tmrControl.Enabled = True
End Sub

Private Sub SaveOptionsData()
    On Error Resume Next
    Dim aOpt As clsMsOptAtom
    
    If m_Opt.Count > 0 Then
        For Each aOpt In m_Opt
            Err.Clear
            gDBW.usp_Option_Import m_Und.ID, aOpt.Symbol, (aOpt.OptType <> enOtPut), _
                                    aOpt.Strike, aOpt.Expiry, aOpt.LotSize
                                    
            If Err.Number <> 0 Then
                AddLogMsg aOpt.Symbol & ": Fail to save option. " & Err.Description
                
                Err.Clear
                gDBW.usp_EodMsOptionCache_Save m_Und.ID, aOpt.Symbol, aOpt.Expiry, aOpt.Strike, (aOpt.OptType <> enOtPut), aOpt.LotSize
                If Err.Number <> 0 Then
                    AddLogMsg m_Und.Symbol & ": Fail to save market structure info. " & Err.Description
                    
                    m_enState = enMsError
                    UpdateControls
                    tmrControl.Enabled = True
                    Exit Sub
                End If
            End If
            
            IncProgress pbProgress
            DoEvents
            If m_enState <> enMsSaveData Then Exit Sub
        Next
    
    End If

    Err.Clear
    gDBW.usp_EodMsUnderlyingCache_Check m_Und.ID
    If Err.Number <> 0 Then
        AddLogMsg m_Und.Symbol & ": Fail to save market structure info. " & Err.Description
        
        m_enState = enMsError
        UpdateControls
        tmrControl.Enabled = True
        Exit Sub
    End If
End Sub

Private Sub SaveIvData()
    On Error GoTo EH
    Dim aIdxComp As EtsGeneralLib.IndexCompAtom, aVola As clsMsVolaAtom, aVolaBand As clsMsVolaBandAtom
    
    If m_bWithIvData Then
        If m_Und.UndType = enCtStock Then
            For Each aIdxComp In m_Und.IdxComp
                gDBW.usp_IndexUnderlyingBeta_Save aIdxComp.ID, m_Und.ID, aIdxComp.Beta
                
                IncProgress pbProgress
                DoEvents
                If m_enState <> enMsSaveData Then Exit Sub
            Next
        End If
        
        For Each aVola In m_Vola
            gDBW.usp_CustomStrikeSkewDefaultPointFromIV_Save m_Und.ID, aVola.Strike, aVola.Expiry, OTM_IVTYPE, aVola.Vola
            
            IncProgress pbProgress
            DoEvents
            If m_enState <> enMsSaveData Then Exit Sub
        Next
        
        If m_VolaBand.Count > 0 Then
            gDBW.usp_VolaBandsEOD_Init_Save
        
            For Each aVolaBand In m_VolaBand
                gDBW.usp_VolaBandsEOD_Save m_Und.Symbol, aVolaBand.ExpiryPeriod, aVolaBand.StrikeMoneyness, aVolaBand.IvBandLow, aVolaBand.IvBandHi
                
                IncProgress pbProgress
                DoEvents
                If m_enState <> enMsSaveData Then Exit Sub
            Next
        
            gDBW.usp_VolaBandsEOD_Complete_Save
        End If
    End If

    Exit Sub
EH:
    AddLogMsg m_Und.Symbol & ": Fail to save IV data. " & Err.Description
End Sub

Private Sub AdjustGlobalDataStorage()
    On Error Resume Next
    Dim aGUnd As EtsGeneralLib.UndAtom, aContract As EtsGeneralLib.EtsContractAtom
    Dim aIdx As EtsGeneralLib.IndexAtom, aIdxComp As EtsGeneralLib.IndexCompAtom, aNewIdxComp As EtsGeneralLib.IndexCompAtom
    
    Dim aFutRoot As EtsGeneralLib.EtsFutRootAtom
    Dim nID As Long
    Dim nFutRootID As Long
    Dim nFutID As Long
    Dim aFut As EtsGeneralLib.EtsFutAtom
    Dim rs As ADODB.Recordset
    Dim nOptRootID As Long
    Dim aRoot As EtsGeneralLib.EtsOptRootAtom
    Dim aSyntRoot As EtsGeneralLib.SynthRootAtom
    Dim nSynthUndID As Long
    Dim aSyntRootComp As EtsGeneralLib.SynthRootCompAtom
    
    If m_enState <> enMsSaveData Then Exit Sub
    
    
    Set aGUnd = g_UnderlyingAll(m_Und.ID)
    If aGUnd Is Nothing Then
        Set aGUnd = New EtsGeneralLib.UndAtom
        aGUnd.ID = m_Und.ID
        aGUnd.UndType = m_Und.UndType
        aGUnd.Symbol = m_Und.Symbol
        aGUnd.ContractName = m_Und.Description
        
        aGUnd.IsAmerican = m_Und.IsAmerican
        aGUnd.IsHTB = m_Und.IsHTB

        aGUnd.IsTraderContract = m_Und.IsTraderContract
        
        Set aGUnd.UndPriceProfile = m_Und.UndPriceProfile
        Set aGUnd.OptPriceProfile = m_Und.OptPriceProfile

        g_UnderlyingAll.Add aGUnd.ID, aGUnd.Symbol, aGUnd
        'g_UnderlyingAllBySym.Add aGUnd.Symbol, aGUnd
                
        If aGUnd.IsTraderContract Then
            g_Underlying.Add aGUnd.ID, aGUnd.Symbol, aGUnd
            'g_UnderlyingBySym.Add aGUnd.Symbol, aGUnd
            
            If aGUnd.UndType = enCtStock Then
                g_Stock.Add aGUnd.ID, aGUnd.Symbol, aGUnd
                'g_StockBySym.Add aGUnd.Symbol, aGUnd
            End If
        End If
        
        If g_ContractAll(m_Und.ID) Is Nothing Then
            Set aContract = New EtsGeneralLib.EtsContractAtom
            Set aContract.Und = aGUnd
            
            g_ContractAll.Add aContract.Und.ID, aContract.Und.Symbol, aContract
            'g_ContractAllBySym.Add aContract.Und.Symbol, aContract
            
            If aGUnd.IsTraderContract Then _
                g_Contract.Add aContract.Und.ID, aContract.Und.Symbol, aContract
                        
            Set aContract = Nothing
        End If
        
    End If
    
    aGUnd.PriceClose = m_Und.PriceClose
    aGUnd.LotSize = m_Und.LotSize
    
    Set aGUnd.Dividend = m_Und.Dividend
    If aGUnd.UndType = enCtIndex Then
        aGUnd.Yield = m_Und.Yield
    End If
    
    If m_bWithIvData Then
        aGUnd.Skew = m_Und.Skew
        aGUnd.Kurt = m_Und.Kurt
        
        If m_Und.UndType = enCtStock Then
            For Each aIdxComp In m_Und.IdxComp
                Set aIdx = g_Index(aIdxComp.ID)
                
                Set aNewIdxComp = aIdx.Components(m_Und.ID)
                If aNewIdxComp Is Nothing Then
                    Set aNewIdxComp = aIdx.Components.Add(m_Und.ID)
                    aNewIdxComp.ID = m_Und.ID
                
                    If aIdx.IsBasket Then
                        aIdx.InitBasketDivs g_UnderlyingAll
                    End If
                End If
                aNewIdxComp.Beta = aIdxComp.Beta
                
                Set aIdx = Nothing
                Set aNewIdxComp = Nothing
            Next
        End If
    End If
    
    
        Set rs = gDBW.usp_MmOptionRoot_Get(aGUnd.ID)
        While Not rs.EOF
                nOptRootID = ReadLng(rs!iRootID)
                If aGUnd.Roots(nOptRootID) Is Nothing Then
                    Set aRoot = New EtsGeneralLib.EtsOptRootAtom
                    aRoot.ID = nOptRootID
                    aRoot.UndID = m_Und.ID
                    aRoot.Name = ReadStr(rs!vcSymbol)
                    aRoot.LotSize = ReadLng(rs!iLotSize)
                    aRoot.IsSynth = IIf(ReadByte(rs!tiIsSynthetic) <> 0, True, False)

                    If Not aGUnd.HaveOptions Then aGUnd.HaveOptions = True

                    If aRoot.IsSynth Then
                        If aGUnd.SyntheticRoots Is Nothing Then
                            aGUnd.HaveSyntheticRoots = True
                            Set aGUnd.SyntheticRoots = New EtsGeneralLib.SynthRootColl
                        End If

                        Set aSyntRoot = New EtsGeneralLib.SynthRootAtom
                        aSyntRoot.Basket = (ReadByte(rs!tiSyntheticIsBasket) <> 0)
                        aSyntRoot.CashValue = ReadDbl(rs!fCash)
                        aSyntRoot.Skew = ReadDbl(rs!fSyntheticSkew)
                        aSyntRoot.Kurt = ReadDbl(rs!fSyntheticKurt)
                        aSyntRoot.Yield = ReadDbl(rs!fSyntheticYield)
                        aSyntRoot.OptRootID = nOptRootID

                        aGUnd.SyntheticRoots.Add aSyntRoot.OptRootID, aSyntRoot

                        Set aSyntRoot = Nothing
                    End If

                    aGUnd.Roots.Add aRoot.ID, aRoot.Name, aRoot

'                    If g_OptRootByName(aRoot.Name) Is Nothing Then _
                        'g_OptRootByName.Add aRoot.Name, aRoot

                    Set aRoot = Nothing
                End If
            rs.MoveNext
        Wend
        Set rs = Nothing

'''''''''''''''''''''''''''''''''''''''''''''''''

        Set rs = gDBW.usp_MmSyntheticRootParams_Get(aGUnd.ID)
        While Not rs.EOF
                nOptRootID = ReadLng(rs!iOptionRootID)

                Set aSyntRoot = aGUnd.SyntheticRoots(nOptRootID)
                If Not aSyntRoot Is Nothing Then
                    nSynthUndID = ReadLng(rs!iComponentUnderlyingID)
                    Set aSyntRootComp = aGUnd.SyntheticRoots(nSynthUndID)

                    If aSyntRootComp Is Nothing Then
                        Set aSyntRootComp = New EtsGeneralLib.SynthRootCompAtom

                        aSyntRootComp.UndID = nSynthUndID
                        aSyntRootComp.Weight = ReadDbl(rs!fWeight)

                        aSyntRoot.SynthRootComponents.Add aSyntRootComp.UndID, aSyntRootComp

                        Set aSyntRootComp = Nothing
                    End If

                    Set aSyntRoot = Nothing
                End If
        Wend
        Set rs = Nothing

        Set rs = gDBW.usp_MmFutureRoot_Get(aGUnd.ID)
        While Not rs.EOF
                nID = ReadLng(rs!iFutureRootID)

                If aGUnd.FutRoots(nID) Is Nothing Then
                    Set aFutRoot = New EtsGeneralLib.EtsFutRootAtom

                    aFutRoot.ID = nID
                    aFutRoot.Symbol = ReadStr(rs!vcFutureRootSymbol)
                    aFutRoot.Name = ReadStr(rs!vcFutureRootName)
                    aFutRoot.UndID = aGUnd.ID
                    aFutRoot.FutLotSize = ReadLng(rs!iFutureLotSize)
                    aFutRoot.OptLotSize = ReadLng(rs!iOptionLotSize)

                    aGUnd.FutRoots.Add aFutRoot.ID, aFutRoot.Symbol, aFutRoot
                    Set aFutRoot = Nothing
                End If

            rs.MoveNext
        Wend
        Set rs = Nothing

        Set rs = gDBW.usp_MmFuture_Get(aGUnd.ID)
        While Not rs.EOF
                nFutRootID = ReadLng(rs!iFutureRootID)
                Set aFutRoot = aGUnd.FutRoots(nFutRootID)

                If Not aFutRoot Is Nothing Then
                    nFutID = ReadLng(rs!iFutureID)

                    If aFutRoot.Futures(nFutID) Is Nothing Then
                        Set aFut = New EtsGeneralLib.EtsFutAtom

                        aFut.ID = nFutID
                        aFut.Symbol = ReadStr(rs!vcFutureSymbol)
                        aFut.ContractName = ReadStr(rs!vcFutureName)
                        aFut.FutRootID = nFutRootID
                        aFut.MaturityDate = ReadDate(rs!dtMaturityDate)
                        aFut.IsAmerican = (ReadByte(rs!tiCalcOptionType) <> 0)
                        aFut.IsActive = (ReadByte(rs!tiIsActive) <> 0)
                        aFut.PriceClose = ReadDbl(rs!fPriceClose)
                        aFut.HaveOptions = (ReadLng(rs!iHaveOptions) <> 0)

                        Set aFut.UndPriceProfile = g_PriceProfile(ReadLng(rs!iUndPriceProfileID))
                        Set aFut.OptPriceProfile = g_PriceProfile(ReadLng(rs!iOptPriceProfileID))

                        If aFut.UndPriceProfile Is Nothing Then Set aFut.UndPriceProfile = g_Main.DefStkPriceProfile
                        If aFut.OptPriceProfile Is Nothing Then Set aFut.OptPriceProfile = g_Main.DefOptPriceProfile

                        Debug.Assert Not aFut.UndPriceProfile Is Nothing And Not aFut.OptPriceProfile Is Nothing

                        If Not aGUnd.HaveFutures Then aGUnd.HaveFutures = True

                        aFutRoot.Futures.Add aFut.ID, aFut.Symbol, aFut

                        If g_ContractAll(aFut.ID) Is Nothing Then
                            Set aContract = New EtsGeneralLib.EtsContractAtom
                            Set aContract.Und = aGUnd
                            Set aContract.Fut = aFut
                            Set aContract.FutRoot = aFutRoot

                            g_ContractAll.Add aContract.Fut.ID, aContract.Fut.Symbol, aContract

                            If aGUnd.IsTraderContract Then g_Contract.Add aContract.Fut.ID, aContract.Fut.Symbol, aContract

                            Set aContract = Nothing
                        End If

                        Set aFut = Nothing
                    End If

                    Set aFutRoot = Nothing
                End If

            rs.MoveNext
        Wend
        Set rs = Nothing
        
    Set aGUnd = Nothing
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    Select Case m_enState
        Case enMsStart
            btnControl.Caption = "Start"
            
        Case enMsRequestStock, enMsRequestDefVola, enMsRequestVolaBands, enMsRequestOptions, enMsRequestSkewKurt, enMsRequestIdxComps
            btnControl.Caption = "Cancel"
            
        Case enMsError, enMsComplete
            btnControl.Caption = "Close"
    End Select
End Sub
