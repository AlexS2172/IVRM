VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Flex options close prices"
   ClientHeight    =   8805
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4470
   BeginProperty Font 
      Name            =   "Courier"
      Size            =   9.75
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8805
   ScaleWidth      =   4470
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox tbLog 
      Height          =   7455
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Top             =   480
      Width           =   4455
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Close"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   1200
      TabIndex        =   0
      Top             =   8160
      Width           =   1935
   End
   Begin VB.Label lblUndTotal 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   3120
      TabIndex        =   2
      Top             =   120
      Width           =   1335
   End
   Begin VB.Label lblUnd 
      Caption         =   "Underlyings with flex options found"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   2775
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private m_nNewGrpID&
Public Grp As EtsMmQuotesLib.MmQvGrpAtom
Public QuoteReqsAll As clsQvRequestColl
Public CustRates As New clsEntityColl
Public m_bDataLoad As Boolean
Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
Attribute VolaSource.VB_VarHelpID = -1
Private WithEvents MsProvider As PRICEPROVIDERSLib.StructureProviderEx
Attribute MsProvider.VB_VarHelpID = -1
Private m_hComplete As Long
Private m_bCorrectClosePrice As Boolean
Private m_dPriceClose As Double
Private m_nUndQuotesCount As Long

Private Sub Command1_Click()
    ShutDown True
    End
End Sub
Private Function InitGroup(ByVal nGroupID As Long) As Boolean
    On Error GoTo EH
    Dim aContract As EtsGeneralLib.EtsContractAtom

    InitGroup = False
    
    If m_bInProc Then Exit Function
    
    Set aContract = g_ContractAll(nGroupID)
    If aContract Is Nothing Then Exit Function
    
    m_bDataLoad = True
    m_bInProc = True
    
    m_nUndQuotesCount = 0
    m_nOptQuotesCount = 0
    m_nFutQuotesCount = 0

    DoEvents
    
    Grp.ID = aContract.ID
    Grp.ContractType = aContract.ContractType
    Grp.Symbol = aContract.Symbol

    
    InitUnderlying aContract
    DoEvents
    If Not m_bDataLoad Then GoTo EX

    InitContracts aContract
    DoEvents
    If Not m_bDataLoad Then GoTo EX

    
    InitContractsQuotes aContract

    Select Case aContract.ContractType
        Case enCtIndex, enCtStock
            If Not InitUnderlyingOptions(aContract) Then GoTo EX
        Case Else
            GoTo EX
    End Select

    UnderlyingAdjustRates True
    
    m_dPriceClose = 0#
    m_bCorrectClosePrice = True
    
    
    RequestStock aContract.Symbol, g_Main.Exch(aContract.Und.PrimaryExchangeID).Code

    InitGroup = True
EX:
    m_bInProc = False
    m_bDataLoad = False
    Set aContract = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox Me, "Fail to load underlying."
    GoTo EX
End Function

Private Sub InitUnderlying(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    'On Error Resume Next
    Dim aIdx As EtsGeneralLib.IndexAtom, sKey$
    
    If Grp.ID = 0 Then Exit Sub
    
    Grp.Und.ID = aContract.Und.ID
    Grp.Und.Symbol = aContract.Und.Symbol
    Grp.Und.SymbolName = aContract.Und.ContractName
    Grp.Und.UndType = aContract.Und.UndType
    Grp.Und.IsTraderContract = aContract.Und.IsTraderContract
    Grp.Und.PrimaryExchangeID = 0 'aContract.Und.PrimaryExchangeID
    
    InitVola Grp.Und
    
    Grp.Und.IsHTB = aContract.Und.IsHTB
    Grp.Und.Skew = aContract.Und.Skew
    Grp.Und.Kurt = aContract.Und.Kurt
    
    Set Grp.Und.UndPriceProfile = aContract.Und.UndPriceProfile
    Set Grp.Und.OptPriceProfile = aContract.Und.OptPriceProfile
    
    Grp.Und.UndPosForRates = 0#
     
    Grp.Und.IsAmerican = aContract.Und.IsAmerican
        
    Grp.UseCustRates = UseCustRates
    
    If (Not aContract.Und.Dividend Is Nothing) Then
        Set Grp.Und.Dividend = aContract.Und.Dividend
        If Grp.Und.Dividend.DivDate = 0 Then Grp.Und.Dividend.DivDate = Date
        If Grp.Und.Dividend.DivDateCust = 0 Then Grp.Und.Dividend.DivDateCust = Date
    End If
        
    If aContract.Und.UndType = enCtIndex Then
        Grp.Und.Yield = aContract.Und.Yield
        Set Grp.Und.BasketIndex = Nothing
        
        Set aIdx = g_Index(Grp.Und.ID)
        If Not aIdx Is Nothing Then
            Set Grp.Und.BasketIndex = aIdx
            Set aIdx = Nothing
        End If
    End If
    
    Grp.Und.Quote.Clear
    Grp.Und.Exp.Clear
    Grp.Und.OptRoot.Clear

    Set aIdx = Nothing
End Sub

Private Sub InitContracts(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    On Error Resume Next
    Dim sKey$
    
    If Grp.ID = 0 Then Exit Sub
    
End Sub

Private Sub InitContractsQuotes(ByRef aContract As EtsGeneralLib.EtsContractAtom)
    'On Error Resume Next
    Dim sKey$, aExch As EtsGeneralLib.ExchAtom
    Dim aReq As clsQvRequestAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    
    If Grp.ID = 0 Then Exit Sub
    
    For Each aExch In Grp.Und.UndExch

        Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
        Set aQuote.Exch = aExch
        aQuote.LotSize = aContract.Und.LotSize

        sKey = Grp.Und.Symbol
        If aExch.ID <> 0 Then
            sKey = sKey & "." & aExch.Code
        End If

        Set aReq = QuoteReqsAll(sKey)
        If aReq Is Nothing Then
            Set aReq = QuoteReqsAll.Add(sKey)
            Set aReq.Exch = aExch
            Set aReq.Und = Grp.Und
        End If
        Set aReq = Nothing

         Grp.Und.Quote.Add aExch.ID, aExch.Code, aQuote
        Set aQuote = Nothing
    Next
    
    m_nUndQuotesCount = Grp.Und.UndExch.Count

End Sub

Private Function InitUnderlyingOptions(ByRef aContract As EtsGeneralLib.EtsContractAtom) As Boolean
    On Error GoTo EH
    Dim rsOpt As ADODB.Recordset, aExch As EtsGeneralLib.ExchAtom, rsExch As ADODB.Recordset
    Dim dtExpiry As Date, dStrike#, dtExpiryMonth As Date, sRoot$, aRoot As EtsMmQuotesLib.MmQvOptRootAtom, aPair As EtsMmQuotesLib.MmQvOptPairAtom
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom, enOptType As EtsGeneralLib.EtsOptionTypeEnum, sKey$, aEnt As clsEntityAtom
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom, aOpt As EtsMmQuotesLib.MmQvOptAtom
    Dim nID&, nOptRootID&, aGUnd As EtsGeneralLib.UndAtom, aSyntRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom, aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aStrAll As EtsMmQuotesLib.MmQvStrikeAtom, aReq As clsQvRequestAtom, aExpAll As EtsMmQuotesLib.MmQvExpAtom
    Dim dtExpiryOV As Date, dtTradingClose As Date
    Dim lSurfaceID As Long
    
    InitUnderlyingOptions = False
    If Grp.ID = 0 Then Exit Function
            
    Set rsOpt = gDBW.usp_FlexOptionByUnderlying_Get(Grp.ID, Date)
    DoEvents
    If Not m_bDataLoad Then GoTo EX
    
    Do While Not rsOpt.EOF
        
        dtExpiryOV = ReadDate(rsOpt!dtExpiryOV)
        dtTradingClose = ReadDate(rsOpt!dtTradingClose)
        'Clip days from trading close time
        dtTradingClose = TimeSerial(Hour(dtTradingClose), Minute(dtTradingClose), 0)
        
        dtExpiry = ReadDate(rsOpt!dtExpiry)
        dtExpiryMonth = DateSerial(Year(dtExpiry), Month(dtExpiry), Day(dtExpiry))

        Set aExp = Grp.Und.Exp(dtExpiryMonth)
        If aExp Is Nothing Then
            Set aExp = Grp.Und.Exp.Add(dtExpiryMonth)
            aExp.Expiry = dtExpiry
            aExp.ExpiryMonth = dtExpiryMonth
            aExp.ExpiryOV = dtExpiryOV
            aExp.TradingClose = dtTradingClose
            aExp.Visible = True
        End If

        nOptRootID = ReadLng(rsOpt!iOptionRootID)
        
        Set aRoot = Grp.Und.OptRoot(nOptRootID)
        If aRoot Is Nothing Then
            Set aRoot = New EtsMmQuotesLib.MmQvOptRootAtom
            aRoot.ID = nOptRootID
            
            aRoot.Name = ReadStr(rsOpt!vcOptionRootSymbol)
            aRoot.LotSize = ReadLng(rsOpt!iLotSize)
            aRoot.IsFit = IIf(ReadLng(rsOpt!tiIsFitEnabled) <> 0, True, False)
            ' Sharky: unnecessary code. just for ensure not zero lot
            If aRoot.LotSize <= 0 Then aRoot.LotSize = 100
            
            If aContract.Und.HaveSyntheticRoots Then
                Set aSynthRoot = aContract.Und.SyntheticRoots(nOptRootID)
            End If
            
            aRoot.Synthetic = Not aSynthRoot Is Nothing
                
            If aRoot.Synthetic Then
                Set aRoot.SynthOptRoot = aSynthRoot
                Dim dDPC As Double
                dDPC = aRoot.LotSize * aSynthRoot.CashValue
                If dDPC <> 0# Then _
                    aRoot.DPC = Trim$(Str$(aRoot.LotSize * aSynthRoot.CashValue)) & " USD"
                                                           
                If aSynthRoot.CashValue > 0# Then
                    If Grp.Und.SynthUnd Is Nothing Then Set Grp.Und.SynthUnd = New EtsMmQuotesLib.MmQvUndColl
                
                    If Grp.Und.SynthUnd(USD_ID) Is Nothing Then
                        Set aUnd = New EtsMmQuotesLib.MmQvUndAtom
                        aUnd.ID = USD_ID
                        aUnd.Symbol = USD_SYMBOL
                        aUnd.SymbolName = USD_SYMBOL
                        aUnd.UndType = enCtStock
                        
                        aUnd.OptRoot.Add aRoot.ID, aRoot.Name, aRoot
                        
                        Set aExch = AddExch(aUnd.UndExch, 0, "", "<Primary>")
                        
                        Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
                        Set aQuote.Exch = aExch
                        aQuote.LotSize = aRoot.LotSize
                        aQuote.PriceAsk = 1#
                        aQuote.PriceBid = 1#
                        aQuote.PriceLast = 1#
                                        
                        aUnd.Quote.Add aExch.ID, aExch.Code, aQuote
                        Set aQuote = Nothing
                        Set aExch = Nothing
                                                    
                         Grp.Und.SynthUnd.Add USD_ID, USD_SYMBOL, aUnd
                        Set aUnd = Nothing
                    End If
                End If
                                  
                For Each aSyntRootComp In aSynthRoot.SynthRootComponents
                    Set aGUnd = g_UnderlyingAll(aSyntRootComp.UndID)
                    
                    If Not aGUnd Is Nothing Then
                        aRoot.DPC = aRoot.DPC & IIf(Len(aRoot.DPC) > 0, " + ", "") & Trim$(Str$(aRoot.LotSize * aSyntRootComp.Weight)) & " " & aGUnd.Symbol
                                            
                        If aSyntRootComp.UndID <> Grp.Und.ID Then
                        
                            If Grp.Und.SynthUnd Is Nothing Then Set Grp.Und.SynthUnd = New EtsMmQuotesLib.MmQvUndColl
                            
                            If Grp.Und.SynthUnd(aSyntRootComp.UndID) Is Nothing Then
                                Set aUnd = New EtsMmQuotesLib.MmQvUndAtom
                                aUnd.ID = aSyntRootComp.UndID
                                aUnd.Symbol = aGUnd.Symbol
                                aUnd.SymbolName = aGUnd.ContractName
                                aUnd.UndType = aGUnd.UndType
                                
                                aUnd.OptRoot.Add aRoot.ID, aRoot.Name, aRoot
                                
                                AddExch aUnd.UndExch, 0, "", "<Primary>"
                                
'                                Set rsExch = gDBW.usp_MmUnderlyingExchanges_Get(aSyntRootComp.UndID)
'                                Do While Not rsExch.EOF
'                                    nID = ReadLng(rsExch!iExchangeId)
'                                    Set aExch = g_Exch(nID)
'
'                                    If Not aExch Is Nothing Then
'                                        If ReadByte(rsExch!tiIsUnderlying) <> 0 Then
'                                            If Grp.UndExchAll(nID) Is Nothing Then AddExch Grp.UndExchAll, nID, aExch.Code, aExch.Name
'                                            If aUnd.UndExch(nID) Is Nothing Then AddExch aUnd.UndExch, nID, aExch.Code, aExch.Name
'                                        End If
'                                        Set aExch = Nothing
'                                    End If
'
'                                    DoEvents
'                                    If Not m_bDataLoad Then GoTo Ex
'                                    rsExch.MoveNext
'                                Loop
'                                Set rsExch = Nothing
                                
                                For Each aExch In aUnd.UndExch
                        
                                    Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
                                    Set aQuote.Exch = aExch
                                    aQuote.LotSize = aRoot.LotSize
                        
                                    sKey = aGUnd.Symbol
                                    If aExch.ID <> 0 Then
                                        sKey = sKey & "." & aExch.Code
                                    End If
                        
                                    Set aReq = QuoteReqsAll.Item(sKey)
                                    If aReq Is Nothing Then
                                        Set aReq = QuoteReqsAll.Add(sKey)
                                        Set aReq.Exch = aExch
                                        Set aReq.Und = aUnd
                                    End If
                                    Set aReq = Nothing
                        
                                    aUnd.Quote.Add aExch.ID, aExch.Code, aQuote
                                    Set aQuote = Nothing
                                Next
                                
                                m_nUndQuotesCount = m_nUndQuotesCount + aUnd.UndExch.Count
                                
                                 Grp.Und.SynthUnd.Add aSyntRootComp.UndID, aGUnd.Symbol, aUnd
                                Set aUnd = Nothing
                            End If
                        End If
                        Set aGUnd = Nothing
                    End If
                Next
            Else
                aRoot.DPC = Trim$(Str$(aRoot.LotSize)) + " " + Grp.Und.Symbol
            End If
        
            aRoot.Visible = True
             Grp.Und.OptRoot.Add aRoot.ID, aRoot.Name, aRoot
        End If
        Set aSynthRoot = Nothing

        Set aExpAll = Grp.ExpAll(dtExpiryMonth)
        If aExpAll Is Nothing Then
            Set aExpAll = Grp.ExpAll.Add(dtExpiryMonth)
            aExpAll.Expiry = dtExpiry
            aExpAll.ExpiryMonth = dtExpiryMonth
            aExpAll.Visible = True
            aExpAll.ExpiryOV = dtExpiryOV
            aExpAll.TradingClose = dtTradingClose
        End If
        
        If InStr(aExpAll.RootNames, aRoot.Name) = 0 Then
            If Len(aExpAll.RootNames) > 0 Then
                aExpAll.RootNames = aExpAll.RootNames & "," & aRoot.Name
            Else
                aExpAll.RootNames = aRoot.Name
            End If
        End If

        Set aExpAll = Nothing

        dStrike = Round(ReadDbl(rsOpt!fStrike), STRIKE_DECIMALS_COUNT)
        
        Set aStr = aExp.Strike(dStrike)
        If aStr Is Nothing Then
            Set aStr = aExp.Strike.Add(dStrike)
            aStr.Strike = dStrike
            aStr.Visible = True
        End If
        
        Set aStrAll = Grp.StrikeAll(dStrike)
        If aStrAll Is Nothing Then
            Set aStrAll = Grp.StrikeAll.Add(dStrike)
            aStrAll.Strike = dStrike
            aStrAll.Visible = True
        End If
        Set aStrAll = Nothing
        
        Set aPair = aStr.OptPair(aRoot.ID)
        If aPair Is Nothing Then
            Set aPair = New EtsMmQuotesLib.MmQvOptPairAtom
            aPair.RootID = aRoot.ID
            
            aStr.OptPair.Add aRoot.ID, aRoot.Name, aPair
        End If

        enOptType = IIf(ReadLng(rsOpt!tiIsCall) <> 0, enOtCall, enOtPut)
        
        Set aOpt = aPair.Opt(enOptType)
        If aOpt.ID = 0 Then
            aOpt.ID = ReadLng(rsOpt!iContractID)
            aOpt.Symbol = ReadStr(rsOpt!vcSymbol)
            aOpt.OptType = enOptType
            aOpt.Expiry = dtExpiry
            aOpt.ExpiryOV = dtExpiryOV
            aOpt.TradingClose = dtTradingClose
            aOpt.Strike = dStrike
            aOpt.RootID = aPair.RootID
            
            lSurfaceID = Grp.Und.VolaSrv.GetSurfaceByRoot(aPair.RootID)
            aOpt.Vola = Grp.Und.VolaSrv.OptionVola(dtExpiryOV, dStrike, lSurfaceID)

            If aOpt.Vola < 0 Then
                aOpt.Vola = BAD_DOUBLE_VALUE
            End If

             Grp.Und.Opt.Add aOpt.ID, aOpt
        End If

        Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
        Set aExch = g_Exch(nID)
        Set aQuote.Exch = aExch
        aQuote.LotSize = aRoot.LotSize
            
        If Int(aOpt.Strike) = aOpt.Strike Then
            aQuote.Series = UCase$(Format$(aOpt.Expiry, "YYMMM")) & Trim$(Str$(Int(aOpt.Strike)))
        Else
            aQuote.Series = UCase$(Format$(aOpt.Expiry, "YYMMM")) & Trim$(Format$(aOpt.Strike, "##,##.#0"))
        End If
            
            
        Set aOpt.DefQuote = aQuote
        aQuote.Exch.Visible = True
        aOpt.Quote.Add 0, "", aQuote
        Set aQuote = Nothing

        DoEvents
        If Not m_bDataLoad Then GoTo EX

        Set aOpt = Nothing
        Set aPair = Nothing
        Set aRoot = Nothing
        Set aUnd = Nothing
        Set aSyntRootComp = Nothing
        
        rsOpt.MoveNext
    Loop
    
    m_nOptQuotesCount = Grp.Und.Opt.Count * Grp.Und.OptExch.Count
    
    InitUnderlyingOptions = True
EX:
    Set aGUnd = Nothing
    Set aPair = Nothing
    Set aRoot = Nothing
    Set aExp = Nothing
    Set aQuote = Nothing
    Set rsOpt = Nothing
    Set rsExch = Nothing
    Set aExch = Nothing
    Set aSynthRoot = Nothing
    Set aSyntRootComp = Nothing
    Exit Function
EH:
    'If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlying."
    Debug.Print "Error in InitUnderlyingOptions"
    GoTo EX
End Function


Private Function UnderlyingAdjustRates(ByVal bForceUpdateCustom As Boolean) As Boolean
    'On Error Resume Next
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    Dim bForceRateUpdate As Boolean, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim dtNow As Date
    
    UnderlyingAdjustRates = False
    If Grp.ID = 0 Then Exit Function
    
    dtNow = GetNewYorkTime
    
    bForceRateUpdate = False
    
    dPos = g_UnderlyingAll(Grp.Und.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    
        If Not bUseMidRates Then _
            bForceRateUpdate = (Sgn(dPos) * Sgn(Grp.Und.UndPosForRates) < 0)
    
    Else
        bUseMidRates = True
    End If
    
    If bForceUpdateCustom Or Grp.Und.UseMidRates <> bUseMidRates Or bForceRateUpdate Then
        Grp.Und.UseMidRates = bUseMidRates
        Grp.Und.UndPosForRates = dPos
        
        
        If Grp.IsStockOrIndex Then
            Set aExpColl = Grp.Und.Exp
        Else
            Set aExpColl = Grp.ExpAll
        End If
                
        Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(Grp.Und.ID)

        For Each aExp In aExpColl
                    If bUseMidRates Then
                            aExp.Rate = GetNeutralRate(dtNow, aExp.ExpiryOV)
                        Else
                            aExp.Rate = IIf(dPos < 0#, GetShortRate(dtNow, aExp.ExpiryOV), GetLongRate(dtNow, aExp.ExpiryOV))
                    End If
                    
            If bIsHTBRatesExist Then
                aExp.HTBRate = GetHTBRate(Grp.Und.ID, dtNow, aExp.ExpiryOV)
            Else
                aExp.HTBRate = BAD_DOUBLE_VALUE
            End If
            
'            If bForceUpdateCustom Or Not Grp.UseCustRates Then
'                If QV.CustRates(CStr(CLng(aExp.ExpiryMonth))) Is Nothing Then
'                    aExp.RateCust = aExp.Rate
'                End If
'            End If
            Next
        
        If Grp.IsStockOrIndex = False Then
            For Each aExpAll In Grp.ExpAll
                For Each aFut In Grp.Und.Fut
                    For Each aExp In aFut.Exp
                        If aExpAll.ExpiryMonth = aExp.ExpiryMonth Then
                            aExp.RootNames = aExpAll.RootNames
                            aExp.Rate = aExpAll.Rate
                            aExp.RateCust = aExpAll.RateCust
                        End If
                    Next
                Next
            Next
        End If
                
        UnderlyingAdjustRates = True
    End If

End Function


Private Sub InitVola(ByRef aUnd As EtsMmQuotesLib.MmQvUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, aUnd.UndType, VolaSource
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub
Private Function AddExch(ByRef aExchColl As EtsGeneralLib.ExchColl, ByVal nID As Long, ByVal sCode As String, ByVal sName As String) As EtsGeneralLib.ExchAtom
    On Error GoTo EH
    Dim aExch As EtsGeneralLib.ExchAtom
    Set AddExch = Nothing
    
    Set aExch = New EtsGeneralLib.ExchAtom
    aExch.ID = nID
    aExch.Code = sCode
    aExch.Name = sName
    aExch.Visible = True
    aExchColl.Add aExch.ID, aExch.Code, aExch
    Set AddExch = aExch
EX:
    Set aExch = Nothing
    Exit Function
EH:
    Set AddExch = Nothing
    gCmn.ErrorHandler "Fail to add exchange."
    GoTo EX
End Function

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
    Set VolaSource = aVolaSources.VolatilitySource(g_Params.VolatilitySourceType)
    vmDB.ConnectionString = g_Params.DbConnection

    Set VolaSource.DataSource = vmDB
    VolaSource.EnableEvents = True
    VolaSource.EnableEditing = True

    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to initialize volatility source."
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source

    Set vmDB = Nothing
    Set aVolaSources = Nothing
    Set g_VolaSource = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Private Sub Form_Load()
    Dim Und(), Clp()
    Dim rsOpt As ADODB.Recordset
    Dim uc&, ucb&
    Dim sKey$
    Dim aReq As clsQvRequestAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aExch As EtsGeneralLib.ExchAtom
    On Error GoTo EH
    uc& = 0
    ucb& = 0
    
    Connect
    
    Set rsOpt = gDBW.usp_UnderlyingsWithFlexOptions_Get()
    lblUndTotal.Caption = Str(rsOpt.RecordCount)
    
    If Not rsOpt.RecordCount > 0 Then Exit Sub
    
    ReDim Und(0 To rsOpt.RecordCount - 1)
    ReDim Clp(0 To rsOpt.RecordCount - 1)
    
    Do While Not rsOpt.EOF
         dbl = ReadDbl(rsOpt!PriceClose)
         If dbl <> 0# Then
            Clp(uc) = dbl
            Und(uc) = ReadStr(rsOpt!ContractID)
            uc = uc + 1
         Else
            ucb = ucb + 1
         End If
         rsOpt.MoveNext
    Loop
    lblUndTotal.Caption = Str(rsOpt.RecordCount) + " (" + Str(ucb) + " bad)"
    
    Set Grp = New EtsMmQuotesLib.MmQvGrpAtom
    Set QuoteReqsAll = New clsQvRequestColl
    
    InitVolaSources True
    
    m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
    If m_hComplete <> 0 Then ResetEvent m_hComplete
    Dim dtCalcDate As Date
    dtCalcDate = GetNewYorkTime
    
    
    uc = 0
    For Each u In Und
        
        InitGroup (u)
        
        'wait for response from PriceProvider
        While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLEVENTS) <> WAIT_OBJECT_0
            Sleep 0: DoEvents
        Wend
        If m_hComplete <> 0 Then ResetEvent m_hComplete
        
        If (Grp.Und.Symbol <> "" And m_bCorrectClosePrice = True) Then
                     
            tbLog.Text = tbLog.Text + Grp.Und.Symbol + " (" + Trim(Str(Grp.Und.Opt.Count)) + " flex options found)" + vbNewLine
            tbLog.Text = tbLog.Text + "---------------------------------" + vbNewLine
            
            Set aQuote = New EtsMmQuotesLib.MmQvQuoteAtom
            aQuote.PriceClose = m_dPriceClose 'Clp(uc)
            aQuote.PriceBid = m_dPriceClose 'Clp(uc)
            aQuote.PriceAsk = m_dPriceClose 'Clp(uc)
            aQuote.PriceLast = m_dPriceClose 'Clp(uc)
            aQuote.LotSize = 100
            
            Set aExch = New EtsGeneralLib.ExchAtom
            aExch.Code = ""
            aExch.IsUnderlying = True
            aExch.Visible = True
            
            Set aQuote.Exch = aExch
            
            Grp.Und.Quote.Add 0, "", aQuote
            Grp.Und.SetDirty
            
            Grp.Und.CalcAllOptions GM_THEOPRICE, GM_THEOPRICE, _
                                    enMmQvCalcIvNone, enMmQvCalcIvNone, _
                                    enCmBinomial, _
                                    True, False, False, _
                                    True, Nothing, 0.05, 0, _
                                    False, 0, 0, dtCalcDate, False, True, g_Main.CalculationParametrs
            
            'save theo price as close price
            o = 0
            For Each aOpt In Grp.Und.Opt
                If aOpt.Quote(0).PriceTheo > 0 Then
                    gDBW.usp_ContractPrice_Save Null, aOpt.ID, Null, Null, Null, Null, Null, Null, aOpt.Quote(0).PriceTheo, Null, Null, Null, Null, Null, 3, Null
                    o = o + 1
                    tbLog.Text = tbLog.Text + aOpt.Symbol + vbTab + vbTab + Trim$(Format$(aOpt.Quote(0).PriceTheo, "##,##.####0")) + vbNewLine
                End If
            Next
            tbLog.Text = tbLog.Text + vbNewLine
            
        ElseIf (m_bCorrectClosePrice = False) Then
            tbLog.Text = tbLog.Text + "Wrong close price for: " + Grp.Und.Symbol + vbNewLine
        End If
        
        Grp.Und.Opt.Clear
        Grp.Und.CleanUp
        Grp.CleanUp (True)
        uc = uc + 1
    Next
    
    If m_hComplete <> 0 Then
        CloseHandle m_hComplete
        m_hComplete = 0
    End If
    
    Disconnect
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Operation failed."

End Sub

Private Sub RequestStock(ByVal StockName As String, ByVal StockExchange As String)
On Error GoTo ErrHandler
    Dim sParams As PRICEPROVIDERSLib.StockParams
          
    sParams.Stock = StockName
    sParams.Exchange = StockExchange
                
    MsProvider.RequestStock sParams
    
    
    DoEvents
    
    Exit Sub
ErrHandler:
    Debug.Print "Error while trying to request Stock"
End Sub

Private Sub SetPPRegionRate(sRegion As String, dRate As Double)
On Error GoTo ErrHandler
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Dim aPriceProvider As New PRICEPROVIDERSLib.BatchPriceInfo
    Dim aBatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
    
    Set aPT = aPriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    aPriceProvider.Connect
    Set aBatchPriceProvider = aPriceProvider
    aBatchPriceProvider.SetRegionRate sRegion, dRate
    aPriceProvider.Disconnect
    Set aPriceProvider = Nothing
    Exit Sub
ErrHandler:
    Debug.Print "Error ocures while trying to setup region rate"
End Sub

Private Sub Connect()
On Error GoTo ErrHandler
    Dim aPT As PRICEPROVIDERSLib.IProvider
    
    Set MsProvider = New PRICEPROVIDERSLib.StructureProviderEx
    Dim aBatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
    
    Set aPT = MsProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    MsProvider.Connect

    SetPPRegionRate "CAD", g_Params.ExchangeRate
       
    Exit Sub
ErrHandler:
    Debug.Print "Error on MsProvider initialization: " & Err.Description
End Sub

Private Sub Disconnect()
On Error GoTo ErrHandler
    MsProvider.Disconnect
    Set MsProvider = Nothing
    Exit Sub
ErrHandler:
    Debug.Print "Error on Disconnect: " & Err.Description
End Sub

Private Sub MsProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
On Error Resume Next
    tbLog.Text = tbLog.Text + "Unable to request symbol: " + Description + vbNewLine
    m_bCorrectClosePrice = False
    SetEvent m_hComplete
End Sub

Private Sub MsProvider_OnStock(Params As PRICEPROVIDERSLib.StockParams, Results As PRICEPROVIDERSLib.StockResultsEx)
    tbLog.Text = tbLog.Text + Params.Stock + "." + Params.Exchange + " - price for calc " + Format$(Results.ClosePrice, "##,##.####0") + vbNewLine
    m_dPriceClose = Results.ClosePrice
    SetEvent m_hComplete
End Sub
