VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxRiskViewCalc"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_bShutDown As Boolean
Private m_bInCalc As Boolean

Public UndUpdated&
Public OptUpdated&
Public FutUpdated&
Private m_nGreeksMask&

Private m_ProcessDelay As EtsGeneralLib.EtsProcessDelay

Public UndCount&
Public OptCount&
Public FutCount&

Public QuoteReqsAll As New EtsMmRisksLib.MmRvReqColl ' all quote requests
Public QuoteReqsGrp As New EtsMmRisksLib.MmRvReqColl ' referencies to group requests
Public QuoteReqsNonGrp As New EtsMmRisksLib.MmRvReqColl ' referencies to non-group requests

Private m_Aux As clsAuxRiskView
Private m_bUndDataProviderInit As Boolean
Private m_bPosDataProviderInit As Boolean

Public Sub Init(aAux As clsAuxRiskView)
    On Error Resume Next
    m_bShutDown = False
    UndUpdated = 0
    OptUpdated = 0
    FutUpdated = 0
    UndCount = 0
    OptCount = 0
    FutCount = 0
    m_nGreeksMask = GM_ALL
    Set m_Aux = aAux
    Set m_ProcessDelay = New EtsGeneralLib.EtsProcessDelay
    m_bUndDataProviderInit = False
    m_bPosDataProviderInit = False
End Sub

Public Sub Term()
    On Error Resume Next
    m_bShutDown = True
    m_ProcessDelay.Interrupt
    Set m_ProcessDelay = Nothing
    
    QuoteReqsNonGrp.Clear
    QuoteReqsGrp.Clear
    QuoteReqsAll.Clear
    Set m_Aux = Nothing
End Sub

Public Function GetBeta(ByRef Asset As EtsMmRisksLib.MmRvUndAtom) As Double
On Error GoTo exception_handler
'------------------------------'
    Dim aHedgeIndex As EtsGeneralLib.IndexAtom
    Dim aIndexComponent As EtsGeneralLib.IndexCompAtom
    Dim lHedgeIndexID As Long
    
    GetBeta = BAD_DOUBLE_VALUE
    
    If (Not m_Aux Is Nothing) Then
        If (Not Asset Is Nothing) Then
            
            If (Not m_Aux.Idx Is Nothing) Then
                lHedgeIndexID = m_Aux.Idx.ID
            End If
        
            Set aHedgeIndex = g_HedgeSymbols(lHedgeIndexID)
            
            If (Not aHedgeIndex Is Nothing) Then
                Set aIndexComponent = aHedgeIndex.Components(Asset.ID)
                
                If (Not aIndexComponent Is Nothing) Then
                    GetBeta = aIndexComponent.Beta
                    Set aIndexComponent = Nothing
                End If
            End If
            
        End If
    End If
    
Exit Function
exception_handler:
    Debug.Print "Error while calling GetBeta() Description: " & Err.Description & " Number: "; CStr(Err.Number)
End Function

Public Function CantStartRealTime() As Boolean
    CantStartRealTime = CBool(OptCount > g_Params.MaxRTOptCount)
    If CantStartRealTime Then
        MsgBox "Can't start Real Time Mode because Options number is " & OptCount & " and it must be less or equal " & g_Params.MaxRTOptCount & ". Please use filters to decrease Options number.", vbExclamation
    End If
End Function

Public Sub UnderlyingsCalc(ByVal bCalcGreeks As Boolean, ByVal bUpdateVola As Boolean, Optional ByVal bRealtimeCalc As Boolean = False, Optional ByVal bTotalsOnly As Boolean = False, _
                                                                                       Optional ByVal bCalcOnlyDirty As Boolean = False)
On Error GoTo error_handler
'--------------------------

    If m_bShutDown Or m_bInCalc Then Exit Sub
    m_bInCalc = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Grp Calculation Enter.", "clsAuxRiskViewCalc"
    
    If g_Params.UseCalcSleep Then
        m_ProcessDelay.Reset
        m_ProcessDelay.Frequency = g_Params.CalcSleepFreq
        m_ProcessDelay.Duration = g_Params.CalcSleepAmt
    End If
    
    Err.Clear
    m_Aux.Grp.Calc m_Aux.Und, m_Aux.Idx, bTotalsOnly, m_nGreeksMask, bCalcGreeks, bUpdateVola, bRealtimeCalc, _
                IIf(g_Params.UseCalcSleep, m_ProcessDelay, Nothing), _
                (g_Params.PnLCalcType = PNLCT_LTD), g_Params.RiskFitToImpFlatNoBid, g_Params.RiskMIVCalcType, _
                g_Params.CalcModel, g_Params.UseTheoVolatility, _
                g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
                g_Params.PriceRoundingRule, m_Aux.SimUnd, False, m_Aux.Simulation, bCalcOnlyDirty, g_Main.CalculationParametrs, UndUpdated, FutUpdated

    If Err.Number <> 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Grp Calculation Exit with Error: " & CStr(Err.Number) & " " & Err.Description, "clsAuxRiskViewCalc"
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Grp Calculation Exit.", "clsAuxRiskViewCalc"
    End If

    m_bInCalc = False
    
    Exit Sub
    
error_handler:

    m_bInCalc = False
    
    If Not g_PerformanceLog Is Nothing Then
            g_PerformanceLog.LogMmInfo enLogFaults, "Error while calculating group: " & _
                                                    Err.Description & "(" & Err.Number & ")", "clsAuxRiskViewCalc"
    End If
End Sub

Public Sub UnderlyingCalc(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal bCalcGreeks As Boolean, ByVal bUpdateVola As Boolean, _
                        Optional ByVal bRealtimeCalc As Boolean = False, Optional ByVal bClearSynthGreeks As Boolean = True)
On Error GoTo error_handler
'--------------------------
    
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    aUnd.Calc m_Aux.Und, m_nGreeksMask, bCalcGreeks, bUpdateVola, bRealtimeCalc, bClearSynthGreeks, Nothing, _
                (g_Params.PnLCalcType = PNLCT_LTD), g_Params.RiskFitToImpFlatNoBid, g_Params.RiskMIVCalcType, _
                g_Params.CalcModel, g_Params.UseTheoVolatility, _
                g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
                g_Params.PriceRoundingRule, m_Aux.SimUnd, False, m_Aux.Simulation, False, g_Main.CalculationParametrs
                    
    aUnd.CalcCV
    
    Exit Sub
    
error_handler:
    If Not g_PerformanceLog Is Nothing Then
            g_PerformanceLog.LogMmInfo enLogFaults, "Error while calculating asset(" & _
                                                    aUnd.Symbol & ") " & _
                                                    Err.Description & "(" & Err.Number & ")", "clsAuxRiskViewCalc"
    End If
End Sub

Public Sub UnderlyingsCalcWtdVega()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    m_Aux.Grp.CalcWtdVega m_Aux.Und, g_ExpCalendar
    If m_Aux.Simulation = True And Not m_Aux.Grp.SimulationGrp Is Nothing And Not m_Aux.SimUnd Is Nothing Then
        m_Aux.Grp.SimulationGrp.CalcWtdVega m_Aux.SimUnd, g_ExpCalendar
    End If
    Debug.Assert Err.Number = 0
End Sub

Public Sub CalcOptionGreeks(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, ByVal nMask As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    aPos.CalcOptionGreeks m_Aux.Und, aUnd, _
            aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule), _
            m_nGreeksMask, (g_Params.PnLCalcType = PNLCT_LTD), g_Params.RiskFitToImpFlatNoBid, g_Params.RiskMIVCalcType, _
            g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, m_Aux.SimUnd, False, m_Aux.Simulation, g_Main.CalculationParametrs
    Debug.Assert Err.Number = 0
    
    aPos.CalcCV
    
End Sub

Public Sub CalcFutOptionGreeks(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, ByVal nMask As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    aPos.CalcFutOptionGreeks aUnd, _
            aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule), _
            m_nGreeksMask, (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, False, m_Aux.Simulation, g_Main.CalculationParametrs

    Debug.Assert Err.Number = 0
    
    aPos.CalcCV
    
End Sub

Public Sub SyntheticUnderlyingCalc(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    aUnd.CalcSyntheticUnderlying m_Aux.Und, m_nGreeksMask, _
            (g_Params.PnLCalcType = PNLCT_LTD), g_Params.RiskFitToImpFlatNoBid, g_Params.RiskMIVCalcType, _
            g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, g_Main.CalculationParametrs
    Debug.Assert Err.Number = 0
    
    aUnd.CalcCV
    
End Sub

Public Sub InitVola(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim nLoopCounter As Long: nLoopCounter = 1
    
    If Not aUnd.VolaSrv.IsInitialized Then
        Err.Clear
        aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), m_Aux.VolaSource
        
        While Err.Number <> 0 And nLoopCounter < 10
            MMSleep 20
            Err.Clear
            aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), m_Aux.VolaSource

            nLoopCounter = nLoopCounter + 1
        Wend
        
        If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
        Else
            If Not aUnd.SimulationUnd Is Nothing Then
                aUnd.SimulationUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), m_Aux.VolaSource 'simulation stuff
                
                nLoopCounter = 1
                While Err.Number <> 0 And nLoopCounter < 10
                    MMSleep 20
                    Err.Clear
                    aUnd.SimulationUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), m_Aux.VolaSource 'simulation stuff

                    nLoopCounter = nLoopCounter + 1
                Wend
                
                If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
            End If
    End If
End Sub

Public Function AddNewUnderlying(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom, _
                                Optional ByRef aNewReqsAll As EtsMmRisksLib.MmRvReqColl = Nothing, _
                                Optional ByRef aNewReqsNonGrp As EtsMmRisksLib.MmRvReqColl = Nothing) As EtsMmRisksLib.MmRvUndAtom
    On Error Resume Next
    Dim sKey$, aUnd As EtsMmRisksLib.MmRvUndAtom, aReq As EtsMmRisksLib.MmRvReqAtom, aSimUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aUndDataProvider As CVLib.IcvDataProvider
    Dim aGrpDataProvider As CVLib.IcvDataProvider
    Debug.Assert m_Aux.Und(aTrd.UndID) Is Nothing
    Dim infSysVars() As CVLib.cvInfo
    Dim infSysFuncs() As CVLib.cvInfo
    Dim lUnsupported As Long
    
    Set aUnd = m_Aux.Und.Add(aTrd.UndID, aTrd.Und.Symbol)
    If m_Aux.Simulation = True Then
        Set aSimUnd = m_Aux.SimUnd.Add(aTrd.UndID, aTrd.Und.Symbol)     'Simulation stuff
        Set aUnd.SimulationUnd = aSimUnd                                'Simulation stuff
    End If
    
    aUnd.ID = aTrd.Und.ID
    aUnd.ContractType = aTrd.Und.UndType
    aUnd.Symbol = aTrd.Und.Symbol
    
    If Not aSimUnd Is Nothing Then
        aSimUnd.Symbol = aUnd.Symbol
        AddSimParamsToUnd aSimUnd, aSimUnd                       'Simulation stuff
    End If
    
    Set aUnd.Dividend = aTrd.Und.Dividend
    If m_Aux.Simulation = True Then Set aSimUnd.Dividend = aTrd.Und.Dividend    'Simulation stuff
    
    If aUnd.ContractType <> enCtStock Then
        Set aUnd.BasketIndex = g_Index(aUnd.ID)
        If m_Aux.Simulation = True Then Set aSimUnd.BasketIndex = g_Index(aUnd.ID)  'Simulation stuff
        If Not aUnd.BasketIndex Is Nothing Then
            If Not aUnd.BasketIndex.IsBasket Then
                Set aUnd.BasketIndex = Nothing
                If m_Aux.Simulation = True Then Set aSimUnd.BasketIndex = Nothing   'Simulation stuff
            End If
        End If
        aUnd.Yield = aTrd.Und.Yield
    End If
    
    aUnd.IsAmerican = aTrd.Und.IsAmerican
    aUnd.IsHTB = aTrd.Und.IsHTB
    aUnd.Skew = aTrd.Und.Skew
    aUnd.Kurt = aTrd.Und.Kurt
    aUnd.HasSynthetic = aTrd.Und.HaveSyntheticRoots
    Set aUnd.SynthRoots = aTrd.Und.SyntheticRoots
    Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
    Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
    If m_Aux.Simulation = True Then
        Set aSimUnd.SynthRoots = aTrd.Und.SyntheticRoots                    'Simulation stuff
        Set aSimUnd.UndPriceProfile = aTrd.Und.UndPriceProfile              'Simulation stuff
        Set aSimUnd.OptPriceProfile = aTrd.Und.OptPriceProfile              'Simulation stuff
    End If
    aUnd.PriceClose = aTrd.Und.PriceClose
    aUnd.LotSize = aTrd.Und.LotSize
    
    aUnd.Qty = BAD_LONG_VALUE
    aUnd.OptQty = BAD_LONG_VALUE
    aUnd.FutQty = BAD_LONG_VALUE
    aUnd.UndPosForRates = 0
    
    ' InitVola aUnd
    
    sKey = CStr(aUnd.ContractType) & "_" & aUnd.Symbol
    Set aReq = QuoteReqsAll(sKey)
    If aReq Is Nothing Then
        Set aReq = QuoteReqsAll.Add(sKey)
        Set aReq.Und = aUnd
        
        QuoteReqsNonGrp.Add sKey, aReq
        
        If Not aNewReqsAll Is Nothing Then aNewReqsAll.Add sKey, aReq
        If Not aNewReqsNonGrp Is Nothing Then aNewReqsNonGrp.Add sKey, aReq
    Else
        aReq.IndexOnly = False
    End If
    Set aReq = Nothing
    
    UndCount = UndCount + 1
    
    'cv stuff
    Set aUnd.cvRTContext = m_Aux.m_aContext
    Set aGrpDataProvider = m_Aux.Grp
    Set aUndDataProvider = aUnd

    If m_bUndDataProviderInit = False And m_Aux.m_aContext.Halted = False Then
        m_Aux.m_aContext.GetSystemEnvironment m_Aux.m_nUndLevelID, infSysVars, infSysFuncs
        lUnsupported = aUndDataProvider.Check(infSysVars, infSysFuncs)
        If lUnsupported > 0 Then
            m_Aux.m_aContext.Halt
            gCmn.ErrorMsgBox Nothing, "Custom variables functionality disabled due incorrect version of data provider"
        End If
        m_bUndDataProviderInit = True
    End If
    
    If m_Aux.m_aContext.Halted = False Then
        aUnd.cvPoolID = m_Aux.m_aContext.CreateCVPool(aUndDataProvider, m_Aux.m_nUndLevelID)
        m_Aux.m_aContext.RegisterPoolDataProvider aUnd.cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider
        m_Aux.m_aContext.BindAggregations aUnd.cvPoolID, m_Aux.Grp.cvPoolID
        If m_Aux.Simulation = True Then
            Set aSimUnd.cvRTContext = m_Aux.m_aContext
            Set aGrpDataProvider = m_Aux.Grp.SimulationGrp                                             'Simulation stuff
            Set aUndDataProvider = aSimUnd                                                  'Simulation stuff
            aSimUnd.cvPoolID = m_Aux.m_aContext.CreateCVPool(aUndDataProvider, m_Aux.m_nUndLevelID) 'Simulation stuff
            m_Aux.m_aContext.RegisterPoolDataProvider aSimUnd.cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider   'Simulation stuff
            m_Aux.m_aContext.BindAggregations aSimUnd.cvPoolID, m_Aux.Grp.SimulationGrp.cvPoolID       'Simulation stuff
        End If
    End If
    
    Set AddNewUnderlying = aUnd
End Function

Public Function AddNewPosition(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom, _
                            ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                            Optional ByRef aNewReqsAll As EtsMmRisksLib.MmRvReqColl = Nothing, _
                            Optional ByRef aNewReqsNonGrp As EtsMmRisksLib.MmRvReqColl = Nothing, _
                            Optional ByRef aNewReqsGrp As EtsMmRisksLib.MmRvReqColl = Nothing) As EtsMmRisksLib.MmRvPosAtom
    On Error Resume Next
    Dim sKey$, aPos As EtsMmRisksLib.MmRvPosAtom, aReq As EtsMmRisksLib.MmRvReqAtom, sExpiryKey$, aSimPos As EtsMmRisksLib.MmRvPosAtom
    Dim aExp As EtsGeneralLib.EtsMmEntityAtom, aFutReq As EtsMmRisksLib.MmRvReqAtom
    Dim aGrpDataProvider As CVLib.IcvDataProvider
    Dim aUndDataProvider As CVLib.IcvDataProvider
    Dim aPosDataProvider As CVLib.IcvDataProvider
    Dim infSysVars() As CVLib.cvInfo
    Dim infSysFuncs() As CVLib.cvInfo
    Dim lUnsupported As Long

    Debug.Assert Not aUnd Is Nothing And aUnd.Pos(aTrd.ContractID) Is Nothing
    
    Set aSimPos = Nothing
    Set aPos = aUnd.Pos.Add(aTrd.ContractID, aTrd.Symbol)
    If Not aUnd.SimulationUnd Is Nothing Then
        Set aSimPos = aUnd.SimulationUnd.Pos.Add(aTrd.ContractID, aTrd.Symbol)  ' simulation stuff
        Set aPos.SimulationPos = aSimPos                                        ' simulation stuff
    End If
    aPos.ID = aTrd.ContractID
    aPos.ContractType = aTrd.ContractType
    aPos.Symbol = aTrd.Symbol
    
    sKey = CStr(aPos.ContractType) & "_" & aPos.Symbol
    
    If aPos.ContractType = enCtOption Then
        If aTrd.Opt.Flex = 1 Then GoTo DoNotAskQuote
    End If
    Set aReq = QuoteReqsAll(sKey)
    If aReq Is Nothing Then
        Set aReq = QuoteReqsAll.Add(sKey)
        Set aReq.Und = aUnd
        Set aReq.Pos = aPos
    
        If Not aNewReqsAll Is Nothing Then aNewReqsAll.Add sKey, aReq
    Else
        If aReq.Pos Is Nothing Then Set aReq.Pos = aPos
        
        If Not aReq.Pos Is Nothing And Not aReq.Pos.OppositePos Is Nothing Then
            Set aPos.OppositePos = aReq.Pos.OppositePos     ' volatility management
            Set aPos.OppositePos.OppositePos = aPos         ' volatility management
            Set aReq.Pos = aPos                             ' volatility management
        End If
    End If
    
    If aPos.ContractType = enCtOption Then
        If QuoteReqsGrp(sKey) Is Nothing Then
            QuoteReqsGrp.Add sKey, aReq
            If Not aNewReqsGrp Is Nothing Then aNewReqsGrp.Add sKey, aReq
        End If
    Else
        If QuoteReqsNonGrp(sKey) Is Nothing Then
            QuoteReqsNonGrp.Add sKey, aReq
            If Not aNewReqsNonGrp Is Nothing Then aNewReqsNonGrp.Add sKey, aReq
        End If
    End If
    
    ' ---------- Volatility management ----------
    If (aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption) And aPos.OppositePos Is Nothing Then
        Dim aOppositePos As New EtsMmRisksLib.MmRvPosAtom
        Dim aOppositeReq As EtsMmRisksLib.MmRvReqAtom
        Dim sOppositeKey As String
        
        Dim aRS As ADODB.Recordset
        Set aRS = gDBW.usp_MmOppositeOptionByOption_Get(aPos.ID)
    
        If Not aRS.EOF Then
            aOppositePos.ID = ReadLng(aRS!ContractID)
            aOppositePos.Symbol = ReadStr(aRS!Symbol)
            aOppositePos.ContractType = aPos.ContractType
        
            sOppositeKey = CStr(aOppositePos.ContractType) & "_" & aOppositePos.Symbol
        
            Set aOppositeReq = QuoteReqsAll(sOppositeKey)
            If aOppositeReq Is Nothing Then
                Set aOppositeReq = QuoteReqsAll.Add(sOppositeKey)
                Set aOppositeReq.Und = aUnd
                Set aOppositeReq.Pos = aOppositePos
    
                If Not aNewReqsAll Is Nothing Then aNewReqsAll.Add sKey, aReq
            End If
        
            Set aPos.OppositePos = aOppositePos
            Set aOppositePos.OppositePos = aPos
        End If
    End If
    ' -------------------------------------------
    
DoNotAskQuote:
    aPos.LotSize = aTrd.LotSize
    
    Select Case aPos.ContractType
        Case enCtOption
            InitVola aUnd
            If Not aUnd.SimulationUnd Is Nothing Then
                aUnd.SimulationUnd.Symbol = aUnd.Symbol
                InitVola aUnd.SimulationUnd
            End If
          
            aPos.PriceClose = aTrd.Opt.PriceClose
            aPos.OptType = aTrd.Opt.OptType
            aPos.Expiry = aTrd.Opt.Expiry
            aPos.ExpiryOV = aTrd.Opt.ExpiryOV
            aPos.TradingClose = aTrd.Opt.TradingClose
            aPos.ExpiryMonth = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry))
            aPos.Strike = aTrd.Opt.Strike
            aPos.OptionRootID = aTrd.OptRoot.ID
            aPos.UndID = aUnd.ID
            aPos.ImportID = aTrd.Opt.ImportID
            aPos.IsFlex = IIf(aTrd.Opt.Flex = 1, True, False)
            
            If aTrd.Opt.Flex = 1 Then
                aPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aPos.ExpiryOV)
            Else
                aPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aPos.ExpiryOV)
            End If
            
            If (Not aSimPos Is Nothing) Then
                aSimPos.IsFlex = IIf(aTrd.Opt.Flex = 1, True, False)
            End If
            
            If Not aSimPos Is Nothing And m_Aux.SimulationDate <= aPos.Expiry Then
                If aTrd.Opt.Flex = 1 Then
                    aSimPos.VegaWeight = g_ExpCalendar.GetVegaWeight(DateAdd("d", DateDiff("d", m_Aux.SimulationDate, GetNewYorkTime), aPos.ExpiryOV))
                Else
                    aSimPos.VegaWeight = g_ExpCalendar.GetVegaWeight(DateAdd("d", DateDiff("d", m_Aux.SimulationDate, GetNewYorkTime), aPos.ExpiryOV))
                End If
            End If
            
            sExpiryKey = CStr(aPos.ExpiryMonth)
            Set aExp = m_Aux.Exp(sExpiryKey)
            If aExp Is Nothing Then
                Set aExp = m_Aux.Exp.Add(sExpiryKey)
                aExp.ID = aPos.ExpiryMonth
                aExp.Name = Format$(aPos.Expiry, "MMM YY")
                Set aExp = Nothing
            End If
            
            OptCount = OptCount + 1
            
            If aUnd.HasSynthetic And Not aUnd.SynthRoots Is Nothing Then
                InitPositionSynthetics aUnd, aPos, aNewReqsAll, aNewReqsNonGrp
'                If Not aSimPos Is Nothing Then
'                    Set aSimPos.SynthGreeks = aPos.SynthGreeks          'simulation stuf
'                End If
            End If
            
        Case enCtFuture, enCtFutOption
            InitVola aUnd
            If Not aUnd.SimulationUnd Is Nothing Then
                aUnd.SimulationUnd.Symbol = aUnd.Symbol
                InitVola aUnd.SimulationUnd
            End If
            
            Dim aFut As EtsMmRisksLib.MmRvFutAtom
            
            If aUnd.Fut Is Nothing Then Set aUnd.Fut = New EtsMmRisksLib.MmRvFutColl
            
            Set aFut = aUnd.Fut(aTrd.FutID)
            If aFut Is Nothing Then
                Set aFut = aUnd.Fut.Add(aTrd.FutID, aTrd.Fut.Symbol)
                aFut.ID = aTrd.FutID
                
                aFut.Symbol = aTrd.Fut.Symbol
                aFut.Maturity = aTrd.Fut.MaturityDate
                Set aFut.UndPriceProfile = aTrd.Fut.UndPriceProfile
                Set aFut.OptPriceProfile = aTrd.Fut.OptPriceProfile
                
                aFut.FutRootID = aTrd.FutRootID
                aFut.FutRootSymbol = aTrd.FutRoot.Symbol
                aFut.ContractName = aTrd.Fut.ContractName
                
                aFut.UndID = aUnd.ID
                
                aFut.OptLotSize = aTrd.FutRoot.OptLotSize
                aFut.FutLotSize = aTrd.FutRoot.FutLotSize
                aFut.PriceClose = aTrd.Fut.PriceClose
                
                aFut.IsAmerican = aTrd.Fut.IsAmerican
                
                FutCount = FutCount + 1
            
                sKey = CStr(enCtFuture) & "_" & aFut.Symbol
                Set aFutReq = QuoteReqsAll(sKey)
                If aFutReq Is Nothing Then
                    Set aFutReq = QuoteReqsAll.Add(sKey)
                    Set aFutReq.Und = aUnd
                
                    If Not aNewReqsAll Is Nothing Then aNewReqsAll.Add sKey, aFutReq
                
                    If QuoteReqsNonGrp(sKey) Is Nothing Then
                        QuoteReqsNonGrp.Add sKey, aFutReq
                        If Not aNewReqsNonGrp Is Nothing Then aNewReqsNonGrp.Add sKey, aFutReq
                    End If
                End If
                
                Set aFutReq.Fut = aFut
                
                Set aFutReq = Nothing
            Else
                Set aReq.Fut = aFut
            End If
            
            Set aPos.Fut = aFut
            aPos.UndID = aUnd.ID
            If Not aPos.SimulationPos Is Nothing Then                       'simulation stuff
                Set aPos.SimulationPos.Fut = aFut                               'simulation stuff
                aPos.SimulationPos.UndID = aUnd.SimulationUnd.ID            'simulation stuff
            End If                                                          'simulation stuff
            
            If aPos.ContractType = enCtFutOption Then
                InitVola aUnd
                
                aPos.PriceClose = aTrd.FutOpt.PriceClose
                aPos.OptType = aTrd.FutOpt.OptType
                aPos.Expiry = aTrd.FutOpt.Expiry
                aPos.ExpiryOV = aTrd.FutOpt.ExpiryOV
                aPos.TradingClose = aTrd.FutOpt.TradingClose
                aPos.ExpiryMonth = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry))
                aPos.Strike = aTrd.FutOpt.Strike
                
                If Not aSimPos Is Nothing And m_Aux.SimulationDate <= aPos.Expiry Then
'                    If aTrd.Opt.Flex = 1 Then
'                        aSimPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aTrd.Opt.FlexExpCalID, DateAdd("d", -DateDiff("d", m_Aux.SimulationDate, Date), aPos.Expiry))
'                    Else
                        aSimPos.VegaWeight = g_ExpCalendar.GetVegaWeight(DateAdd("d", -DateDiff("d", m_Aux.SimulationDate, GetNewYorkTime), aPos.ExpiryOV))
'                    End If
                End If
                
                'aPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aUnd.ExpCalendarID, aPos.Expiry)
                aPos.ImportID = aTrd.FutOpt.ImportID
                
                sExpiryKey = CStr(aPos.ExpiryMonth)
                Set aExp = m_Aux.Exp(sExpiryKey)
                If aExp Is Nothing Then
                    Set aExp = m_Aux.Exp.Add(sExpiryKey)
                    aExp.ID = aPos.ExpiryMonth
                    aExp.Name = Format$(aPos.Expiry, "MMM YY")
                    Set aExp = Nothing
                End If
                
                OptCount = OptCount + 1
            Else
                aPos.PriceClose = aTrd.Fut.PriceClose
            End If
            
            Set aFut = Nothing
        
        Case Else ' stock/index
            aPos.PriceClose = aUnd.PriceClose
    End Select

    Set aReq = Nothing

    aPos.Qty = 0
    aPos.QtyInShares = 0
    
    aPos.QtyLTDBuy = BAD_LONG_VALUE
    aPos.PosLTDBuy = BAD_DOUBLE_VALUE
    aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE
    aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE
    aPos.QtyDailyTodayBuy = BAD_LONG_VALUE
    aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE
    
    aPos.QtyLTDSell = BAD_LONG_VALUE
    aPos.PosLTDSell = BAD_DOUBLE_VALUE
    aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE
    aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE
    aPos.QtyDailyTodaySell = BAD_LONG_VALUE
    aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE
    
    If Not aSimPos Is Nothing Then
        aSimPos.Symbol = aPos.Symbol
        AddSimParamsToPos aSimPos, aUnd                                      'Simulation stuff
    End If
    
    'cv stuff
    Set aPosDataProvider = aPos
    Set aGrpDataProvider = m_Aux.Grp
    Set aUndDataProvider = aUnd
    
    If m_bPosDataProviderInit = False And m_Aux.m_aContext.Halted = False Then
        m_Aux.m_aContext.GetSystemEnvironment m_Aux.m_nOptLevelID, infSysVars, infSysFuncs
        lUnsupported = aPosDataProvider.Check(infSysVars, infSysFuncs)
        If lUnsupported > 0 Then
            m_Aux.m_aContext.Halt
            gCmn.ErrorMsgBox Nothing, "Custom variables functionality disabled due incorrect version of data provider"
        End If
        m_bPosDataProviderInit = True
    End If
   
    If m_Aux.m_aContext.Halted = False Then
        Set aPos.cvRTContext = m_Aux.m_aContext
        aPos.cvPoolID = m_Aux.m_aContext.CreateCVPool(aPosDataProvider, m_Aux.m_nOptLevelID)
        m_Aux.m_aContext.BindAggregations aPos.cvPoolID, aUnd.cvPoolID
        m_Aux.m_aContext.BindAggregations aPos.cvPoolID, m_Aux.Grp.cvPoolID
        m_Aux.m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider
        m_Aux.m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_Aux.m_nUndLevelID, aUndDataProvider
        If Not aPos.SimulationPos Is Nothing Then                                                           'simulation stuff
            Set aPosDataProvider = aPos.SimulationPos                                                       'simulation stuff
            Set aGrpDataProvider = m_Aux.Grp.SimulationGrp                                                             'simulation stuff
            Set aUndDataProvider = aUnd.SimulationUnd                                                       'simulation stuff
            With aPos.SimulationPos                                                                         'simulation stuff
                Set .cvRTContext = m_Aux.m_aContext                                                         'simulation stuff
                .cvPoolID = m_Aux.m_aContext.CreateCVPool(aPosDataProvider, m_Aux.m_nOptLevelID)            'simulation stuff
                m_Aux.m_aContext.BindAggregations .cvPoolID, aUnd.SimulationUnd.cvPoolID                    'simulation stuff
                m_Aux.m_aContext.BindAggregations .cvPoolID, m_Aux.Grp.SimulationGrp.cvPoolID                          'simulation stuff
                m_Aux.m_aContext.RegisterPoolDataProvider .cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider  'simulation stuff
                m_Aux.m_aContext.RegisterPoolDataProvider .cvPoolID, m_Aux.m_nUndLevelID, aUndDataProvider  'simulation stuff
            End With                                                                                        'simulation stuff
        End If
        
    End If
    
    Set AddNewPosition = aPos
    
End Function

Private Sub InitPositionSynthetics(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                Optional ByRef aNewReqsAll As EtsMmRisksLib.MmRvReqColl = Nothing, _
                                Optional ByRef aNewReqsNonGrp As EtsMmRisksLib.MmRvReqColl = Nothing)
    On Error Resume Next
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom, aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim aSynthUnd As EtsMmRisksLib.MmRvUndAtom, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aSimSynthUnd As EtsMmRisksLib.MmRvUndAtom, aSimSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aSimUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aGUnd As EtsGeneralLib.UndAtom, sKey$, aReq As EtsMmRisksLib.MmRvReqAtom
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aBasketDivs As EtsGeneralLib.EtsIndexDivColl

    Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
    
    If Not aSynthRoot Is Nothing Then
        
        aPos.IsSynthetic = True
        If aPos.SynthGreeks Is Nothing Then
            Set aPos.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl
        End If
        If aUnd.SynthGreeks Is Nothing Then
            Set aUnd.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl
        End If
        
        If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing Then
            If aPos.SimulationPos.SynthGreeks Is Nothing Then
                Set aPos.SimulationPos.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl  'Simulation stuff
            End If
        End If
        If m_Aux.Simulation = True And Not aUnd.SimulationUnd Is Nothing Then
            If aUnd.SimulationUnd.SynthGreeks Is Nothing Then
                Set aUnd.SimulationUnd.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl  'Simulation stuff
            End If
        End If
        
        
        If aSynthRoot.CashValue > 0# Then
            Set aSynthUnd = m_Aux.Und(USD_KEY)
            If aSynthUnd Is Nothing Then
                Set aSynthUnd = m_Aux.Und.Add(USD_KEY, USD_SYMBOL)
                aSynthUnd.ID = USD_ID
                aSynthUnd.Symbol = USD_SYMBOL
                Set aSynthUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                
                
                If m_Aux.Simulation = True Then
                    Set aSimSynthUnd = m_Aux.SimUnd.Add(USD_KEY, USD_SYMBOL)                'Simulation stuff
                    Set aSynthUnd.SimulationUnd = aSimSynthUnd                              'Simulation stuff
                    Set aSynthUnd.SimulationUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl    'Simulation stuff
                    
                    aSimSynthUnd.Symbol = aSynthUnd.Symbol
                    AddSimParamsToUnd aSimSynthUnd, aUnd                             'Simulation stuff
                End If
                
                aSynthUnd.ContractType = enCtStock
                aSynthUnd.LotSize = 1
                
                aSynthUnd.Qty = BAD_LONG_VALUE
                aSynthUnd.OptQty = BAD_LONG_VALUE
                aSynthUnd.FutQty = BAD_LONG_VALUE
                aSynthUnd.UndPosForRates = 0
                
                aSynthUnd.PriceAsk = 1#
                aSynthUnd.PriceBid = 1#
                aSynthUnd.PriceLast = 1#
                
                Set aSynthUnd.UndPriceProfile = aUnd.UndPriceProfile
                Set aSynthUnd.OptPriceProfile = aUnd.OptPriceProfile
                
                If m_Aux.Simulation = True Then
                    Set aSynthUnd.SimulationUnd.UndPriceProfile = aUnd.UndPriceProfile     'Simulation stuff
                    Set aSynthUnd.SimulationUnd.OptPriceProfile = aUnd.OptPriceProfile     'Simulation stuff
                End If
                
            End If
                                                                                                                                                                            
            Set aSynthGreek = aPos.SynthGreeks.Add(USD_KEY, USD_SYMBOL)
            aSynthGreek.ContractID = aPos.ID
            aSynthGreek.ContractType = aPos.ContractType
            aSynthGreek.DeliveryUnits = aSynthRoot.CashValue * aPos.LotSize
            aSynthGreek.Symbol = aPos.Symbol
            aSynthGreek.SynthUndID = USD_ID
            aSynthGreek.SynthUndSymbol = USD_SYMBOL
            aSynthGreek.Visible = aPos.Visible
            aSynthGreek.IsTotal = False
            
            If m_Aux.Simulation = True Then
                Set aSimSynthGreek = aPos.SimulationPos.SynthGreeks.Add(USD_KEY, USD_SYMBOL)    'Simulation stuff
                aSimSynthGreek.ContractID = aPos.ID
                aSimSynthGreek.ContractType = aPos.ContractType
                aSimSynthGreek.DeliveryUnits = aSynthRoot.CashValue * aPos.LotSize
                'aSimSynthGreek.Symbol = aPos.Symbol
                aSimSynthGreek.SynthUndID = USD_ID
                'aSimSynthGreek.SynthUndSymbol = USD_SYMBOL
                'aSimSynthGreek.Visible = aPos.Visible
                'aSimSynthGreek.IsTotal = False
            End If
            
            If m_Aux.Simulation = True Then
                Set aSimSynthGreek = aUnd.SimulationUnd.SynthGreeks(USD_KEY)
            End If
            
            Set aSynthGreek = aUnd.SynthGreeks(USD_KEY)
            If aSynthGreek Is Nothing Then
                Set aSynthGreek = aUnd.SynthGreeks.Add(USD_KEY, USD_SYMBOL)
                aSynthGreek.ContractID = aUnd.ID
                aSynthGreek.ContractType = aUnd.ContractType
                aSynthGreek.DeliveryUnits = BAD_DOUBLE_VALUE
                aSynthGreek.Symbol = USD_SYMBOL
                aSynthGreek.SynthUndID = USD_ID
                aSynthGreek.SynthUndSymbol = USD_SYMBOL
                aSynthGreek.Visible = True
                aSynthGreek.IsTotal = True
                
                If m_Aux.Simulation = True Then
                    Set aSimSynthGreek = aUnd.SimulationUnd.SynthGreeks.Add(USD_KEY, USD_SYMBOL)    'Simulation stuff
                    aSimSynthGreek.ContractID = aUnd.ID
                    aSimSynthGreek.ContractType = aUnd.ContractType
                    aSimSynthGreek.DeliveryUnits = BAD_DOUBLE_VALUE
                    'aSimSynthGreek.Symbol = USD_SYMBOL
                    aSimSynthGreek.SynthUndID = USD_ID
                    'aSimSynthGreek.SynthUndSymbol = USD_SYMBOL
                    'aSimSynthGreek.Visible = True
                    'aSimSynthGreek.IsTotal = True
                End If
                
            End If
        End If
        
        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
            
            If aSynthRootComp.UndID <> aUnd.ID Then
            
                Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)
                
                If Not aGUnd Is Nothing Then
                    Set aSynthUnd = m_Aux.Und(aSynthRootComp.UndID)
                    
                    If aSynthUnd Is Nothing Then
                        Set aSynthUnd = m_Aux.Und.Add(aSynthRootComp.UndID, aGUnd.Symbol)
                        aSynthUnd.ID = aSynthRootComp.UndID
                        Set aSynthUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                        
                        If m_Aux.Simulation = True Then
                            Set aSimUnd = m_Aux.SimUnd.Add(aSynthRootComp.UndID, aGUnd.Symbol)     'Simulation stuff
                            Set aSynthUnd.SimulationUnd = aSimUnd                                'Simulation stuff
                            Set aSynthUnd.SimulationUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                            
                            aSimUnd.Symbol = aSynthUnd.Symbol
                            AddSimParamsToUnd aSimUnd, aUnd                                  'Simulation stuff
                        End If
                        
                        aSynthUnd.ContractType = aGUnd.UndType
                        aSynthUnd.Symbol = aGUnd.Symbol
                        aSynthUnd.IsAmerican = aGUnd.IsAmerican
                        aSynthUnd.IsHTB = aGUnd.IsHTB
                        aSynthUnd.Skew = aGUnd.Skew
                        aSynthUnd.Kurt = aGUnd.Kurt
                        aSynthUnd.HasSynthetic = aGUnd.HaveSyntheticRoots
                        aSynthUnd.PriceClose = aGUnd.PriceClose
                        aSynthUnd.LotSize = aGUnd.LotSize
                        Set aSynthUnd.SynthRoots = aGUnd.SyntheticRoots
                        
                        Set aSynthUnd.Dividend = aGUnd.Dividend
                        If m_Aux.Simulation = True Then Set aSimUnd.Dividend = aGUnd.Dividend    'Simulation stuff
                        If aSynthUnd.ContractType <> enCtStock Then
                            Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
                            If m_Aux.Simulation = True Then Set aSimUnd.BasketIndex = g_Index(aSynthUnd.ID)  'Simulation stuff
                            If Not aSynthUnd.BasketIndex Is Nothing Then
                                If Not aSynthUnd.BasketIndex.IsBasket Then
                                    Set aSynthUnd.BasketIndex = Nothing
                                    If m_Aux.Simulation = True Then Set aSimUnd.BasketIndex = Nothing   'Simulation stuff
                                End If
                            End If
                            
                            aSynthUnd.Yield = aGUnd.Yield
                        End If
                        Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
                        Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile
                        If m_Aux.Simulation = True Then
                            Set aSimUnd.SynthRoots = aGUnd.SyntheticRoots                    'Simulation stuff
                            Set aSimUnd.UndPriceProfile = aGUnd.UndPriceProfile              'Simulation stuff
                            Set aSimUnd.OptPriceProfile = aGUnd.OptPriceProfile              'Simulation stuff
                        End If
                        
                        'cv stuff
                        Dim aGrpDataProvider As CVLib.IcvDataProvider
                        Dim aUndDataProvider As CVLib.IcvDataProvider
                        Dim infSysVars() As CVLib.cvInfo
                        Dim infSysFuncs() As CVLib.cvInfo
                        Dim lUnsupported As Long
                        
                        Set aSynthUnd.cvRTContext = m_Aux.m_aContext
                        Set aGrpDataProvider = m_Aux.Grp
                        Set aUndDataProvider = aSynthUnd

                        If m_bUndDataProviderInit = False And m_Aux.m_aContext.Halted = False Then
                            m_Aux.m_aContext.GetSystemEnvironment m_Aux.m_nUndLevelID, infSysVars, infSysFuncs
                            lUnsupported = aUndDataProvider.Check(infSysVars, infSysFuncs)
                            If lUnsupported > 0 Then
                                m_Aux.m_aContext.Halt
                                gCmn.ErrorMsgBox Nothing, "Custom variables functionality disabled due incorrect version of data provider"
                            End If
                            m_bUndDataProviderInit = True
                        End If
    
                        If m_Aux.m_aContext.Halted = False Then
                            aSynthUnd.cvPoolID = m_Aux.m_aContext.CreateCVPool(aUndDataProvider, m_Aux.m_nUndLevelID)
                            m_Aux.m_aContext.RegisterPoolDataProvider aSynthUnd.cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider
                            m_Aux.m_aContext.BindAggregations aSynthUnd.cvPoolID, m_Aux.Grp.cvPoolID
                            If m_Aux.Simulation = True Then
                                Set aSimUnd.cvRTContext = m_Aux.m_aContext
                                Set aGrpDataProvider = m_Aux.Grp.SimulationGrp                                             'Simulation stuff
                                Set aUndDataProvider = aSimUnd                                                  'Simulation stuff
                                aSimUnd.cvPoolID = m_Aux.m_aContext.CreateCVPool(aUndDataProvider, m_Aux.m_nUndLevelID) 'Simulation stuff
                                m_Aux.m_aContext.RegisterPoolDataProvider aSimUnd.cvPoolID, m_Aux.m_nGrpLevelID, aGrpDataProvider   'Simulation stuff
                                m_Aux.m_aContext.BindAggregations aSimUnd.cvPoolID, m_Aux.Grp.SimulationGrp.cvPoolID       'Simulation stuff
                            End If
                        End If
                        
                        UndCount = UndCount + 1
                                                                                                                                                                                                    
                        aSynthUnd.Qty = BAD_LONG_VALUE
                        aSynthUnd.OptQty = BAD_LONG_VALUE
                        aSynthUnd.FutQty = BAD_LONG_VALUE
                        aSynthUnd.UndPosForRates = 0
                        
                        InitVola aSynthUnd
                    
                        sKey = CStr(aSynthUnd.ContractType) & "_" & aSynthUnd.Symbol
                        Set aReq = QuoteReqsAll(sKey)
                        If aReq Is Nothing Then
                            Set aReq = QuoteReqsAll.Add(sKey)
                            Set aReq.Und = aSynthUnd
                            
                            QuoteReqsNonGrp.Add sKey, aReq
                            
                            If Not aNewReqsAll Is Nothing Then aNewReqsAll.Add sKey, aReq
                            If Not aNewReqsNonGrp Is Nothing Then aNewReqsNonGrp.Add sKey, aReq
                        Else
                            aReq.IndexOnly = False
                        End If
                        Set aReq = Nothing
                        
                    End If
                                                        
                    If aSynthUnd.SynthPos Is Nothing Then
                        Set aSynthUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                        
                        If m_Aux.Simulation = True Then
                            Set aSynthUnd.SimulationUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl        'Simulation stuff
                        End If
                    End If
                    
                    aSynthUnd.SynthPos.Add aPos.ID, sKey, aPos
                    
                    Set aSynthGreek = aPos.SynthGreeks.Add(aSynthRootComp.UndID, aPos.Symbol)
                    aSynthGreek.ContractID = aPos.ID
                    aSynthGreek.ContractType = aPos.ContractType
                    aSynthGreek.DeliveryUnits = aSynthRootComp.Weight * aPos.LotSize
                    aSynthGreek.Symbol = aPos.Symbol
                    aSynthGreek.SynthUndID = aSynthRootComp.UndID
                    aSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                    aSynthGreek.Visible = aPos.Visible
                    aSynthGreek.IsTotal = False
                    
                    If m_Aux.Simulation = True Then
                        aSynthUnd.SimulationUnd.SynthPos.Add aPos.ID, sKey, aPos
                        
                        Set aSimSynthGreek = aPos.SimulationPos.SynthGreeks.Add(aSynthRootComp.UndID, aPos.Symbol)
                        aSimSynthGreek.ContractID = aPos.ID
                        aSimSynthGreek.ContractType = aPos.ContractType
                        aSimSynthGreek.DeliveryUnits = aSynthRootComp.Weight * aPos.LotSize
                        'aSimSynthGreek.Symbol = aPos.Symbol
                        aSimSynthGreek.SynthUndID = aSynthRootComp.UndID
                        'aSimSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                        'aSimSynthGreek.Visible = aPos.Visible
                        'aSimSynthGreek.IsTotal = False
                    End If
                    
                    If m_Aux.Simulation = True Then
                        Set aSimSynthGreek = aUnd.SimulationUnd.SynthGreeks(aSynthRootComp.UndID)
                    End If
                    
                    Set aSynthGreek = aUnd.SynthGreeks(aSynthRootComp.UndID)
                    If aSynthGreek Is Nothing Then
                        
                        Set aSynthGreek = aUnd.SynthGreeks.Add(aSynthRootComp.UndID, aSynthUnd.Symbol)
                        aSynthGreek.ContractID = aUnd.ID
                        aSynthGreek.ContractType = aUnd.ContractType
                        aSynthGreek.DeliveryUnits = BAD_DOUBLE_VALUE
                        aSynthGreek.Symbol = aSynthUnd.Symbol
                        aSynthGreek.SynthUndID = aSynthRootComp.UndID
                        aSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                        aSynthGreek.Visible = True
                        aSynthGreek.IsTotal = True
                        
                        If m_Aux.Simulation = True Then
                            Set aSimSynthGreek = aUnd.SimulationUnd.SynthGreeks.Add(aSynthRootComp.UndID, aSynthUnd.Symbol)
                            aSimSynthGreek.ContractID = aUnd.ID
                            aSimSynthGreek.ContractType = aUnd.ContractType
                            aSimSynthGreek.DeliveryUnits = BAD_DOUBLE_VALUE
                            'aSimSynthGreek.Symbol = aSynthUnd.Symbol
                            aSimSynthGreek.SynthUndID = aSynthRootComp.UndID
                            'aSimSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                            'aSimSynthGreek.Visible = True
                            'aSimSynthGreek.IsTotal = True
                        End If
                    End If
                    
                    Set aSynthUnd = Nothing
                    Set aSimSynthUnd = Nothing
                End If
                Set aGUnd = Nothing
            End If
        Next
    End If
End Sub

Public Sub AddNewTradeToPosition(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                                ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nQty&, nQtyInShares&
    
    nQty = aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
    nQtyInShares = nQty * aPos.LotSize
    
    If aTrd.IsBuy Then
        If aPos.QtyLTDBuy = BAD_LONG_VALUE Then aPos.QtyLTDBuy = 0
        aPos.QtyLTDBuy = aPos.QtyLTDBuy + nQtyInShares
        If aTrd.IsPosition Then
            If aPos.PriceClose >= 0 Then
                If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
                aPos.PosLTDBuy = aPos.PosLTDBuy + aPos.PriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
            aPos.PosLTDBuy = aPos.PosLTDBuy + aTrd.Price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateBuy = 0
            aPos.QtyDailyPrevDateBuy = aPos.QtyDailyPrevDateBuy + nQtyInShares
            If aPos.PriceClose >= 0 Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aPos.PriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aTrd.Price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodayBuy = BAD_LONG_VALUE Then aPos.QtyDailyTodayBuy = 0
            aPos.QtyDailyTodayBuy = aPos.QtyDailyTodayBuy + nQtyInShares
            If aTrd.IsPosition Then
                If aPos.PriceClose >= 0 Then
                    If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                    aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aPos.PriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aTrd.Price * nQtyInShares
            End If
        End If
    Else
        If aPos.QtyLTDSell = BAD_LONG_VALUE Then aPos.QtyLTDSell = 0
        aPos.QtyLTDSell = aPos.QtyLTDSell + nQtyInShares
        If aTrd.IsPosition Then
            If aPos.PriceClose >= 0 Then
                If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
                aPos.PosLTDSell = aPos.PosLTDSell + aPos.PriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
            aPos.PosLTDSell = aPos.PosLTDSell + aTrd.Price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateSell = 0
            aPos.QtyDailyPrevDateSell = aPos.QtyDailyPrevDateSell + nQtyInShares
            If aPos.PriceClose >= 0 Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aPos.PriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aTrd.Price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodaySell = BAD_LONG_VALUE Then aPos.QtyDailyTodaySell = 0
            aPos.QtyDailyTodaySell = aPos.QtyDailyTodaySell + nQtyInShares
            If aTrd.IsPosition Then
                If aPos.PriceClose >= 0 Then
                    If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                    aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aPos.PriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aTrd.Price * nQtyInShares
            End If
        End If
    End If
    
    aPos.Qty = aPos.Qty + nQty
    aPos.QtyInShares = aPos.QtyInShares + nQtyInShares
    
    If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
        If aUnd.OptQty = BAD_LONG_VALUE Then aUnd.OptQty = 0
        aUnd.OptQty = aUnd.OptQty + nQty
        
    ElseIf aPos.ContractType = enCtFuture Then
        If aUnd.FutQty = BAD_LONG_VALUE Then aUnd.FutQty = 0
        aUnd.FutQty = aUnd.FutQty + nQty
        
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty + nQtyInShares
        
    Else
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty + nQtyInShares
    End If
End Sub

Public Sub RemoveOldTradeFromPosition(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                                ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nQty&, nQtyInShares&
    
    nQty = aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
    nQtyInShares = nQty * aPos.LotSize
    
    If aTrd.IsBuy Then
        aPos.QtyLTDBuy = aPos.QtyLTDBuy - nQtyInShares
        aPos.PosLTDBuy = aPos.PosLTDBuy - aTrd.Price * nQtyInShares
        
        If aTrd.TradeDate < Date Then
            aPos.QtyDailyPrevDateBuy = aPos.QtyDailyPrevDateBuy - nQtyInShares
            aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy - nQtyInShares * _
                IIf(aPos.PriceClose > DBL_EPSILON, aPos.PriceClose, aTrd.Price)
        Else
            aPos.QtyDailyTodayBuy = aPos.QtyDailyTodayBuy - nQtyInShares
            aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy - aTrd.Price * nQtyInShares
        End If
    Else
        aPos.QtyLTDSell = aPos.QtyLTDSell - nQtyInShares
        aPos.PosLTDSell = aPos.PosLTDSell - aTrd.Price * nQtyInShares
        
        If aTrd.TradeDate < Date Then
            aPos.QtyDailyPrevDateSell = aPos.QtyDailyPrevDateSell - nQtyInShares
            aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell - nQtyInShares * _
                IIf(aPos.PriceClose > DBL_EPSILON, aPos.PriceClose, aTrd.Price)
        Else
            aPos.QtyDailyTodaySell = aPos.QtyDailyTodaySell - nQtyInShares
            aPos.PosDailyTodaySell = aPos.PosDailyTodaySell - aTrd.Price * nQtyInShares
        End If
    End If
    
    aPos.Qty = aPos.Qty - nQty
    aPos.QtyInShares = aPos.QtyInShares - nQtyInShares
    
    If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
        If aUnd.OptQty = BAD_LONG_VALUE Then aUnd.OptQty = 0
        aUnd.OptQty = aUnd.OptQty - nQty
        
    ElseIf aPos.ContractType = enCtFuture Then
        If aUnd.FutQty = BAD_LONG_VALUE Then aUnd.FutQty = 0
        aUnd.FutQty = aUnd.FutQty - nQty
        
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty - nQtyInShares
        
    Else
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty - nQtyInShares
    End If
    
    '--------------------------------------------------------------------
    'aPos.cvPoolID = BAD_LONG_VALUE
    'If Not aUnd Is Nothing Then
        'If aUnd.Pos.Count = 1 Then
            'aUnd.cvPoolID = BAD_LONG_VALUE
        'End If
    'End If
    '--------------------------------------------------------------------
    
End Sub

Public Sub UnderlyingsAdjustRates(ByVal bForceRateUpdate As Boolean)
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    For Each aUnd In m_Aux.Und
        UnderlyingAdjustRates aUnd, bForceRateUpdate
    Next
End Sub

Public Function UnderlyingAdjustRates(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal bForceRateUpdate As Boolean) As Boolean
    On Error Resume Next
    UnderlyingAdjustRates = False
    If aUnd Is Nothing Then Exit Function

    Dim aPos As EtsMmRisksLib.MmRvPosAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    Dim dtToday As Date: dtToday = GetNewYorkTime

    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates

    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
        If Not bForceRateUpdate And Not bUseMidRates Then _
            bForceRateUpdate = (Sgn(dPos) * Sgn(aUnd.UndPosForRates) < 0)
    Else
        bUseMidRates = True
    End If

    If bForceRateUpdate Or aUnd.UseMidRates <> bUseMidRates Then
        aUnd.UseMidRates = bUseMidRates
        aUnd.UndPosForRates = dPos
        
        Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)

        For Each aPos In aUnd.Pos
            If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
                If bUseMidRates Then
                    aPos.Rate = GetNeutralRate(dtToday, aPos.ExpiryOV)
                Else
                    aPos.Rate = IIf(dPos < 0, GetShortRate(dtToday, aPos.ExpiryOV), GetLongRate(dtToday, aPos.ExpiryOV))
                End If
                
                If bIsHTBRatesExist Then
                    aPos.HTBRate = GetHTBRate(aUnd.ID, dtToday, aPos.ExpiryOV)
                Else
                    aPos.HTBRate = BAD_DOUBLE_VALUE
                End If
            End If
        Next

        UnderlyingAdjustRates = True
    End If
End Function

Private Sub AddSimParamsToPos(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aFrom As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim aStockShift As clsStockShift
    Dim sKey As String
    Dim dPercentShift As Double
    Dim dRatio As Double
    Dim dBeta As Double
    
    aPos.SimulationDate = m_Aux.SimulationParams.m_dDate
    aPos.VolaChange = m_Aux.SimulationParams.m_dVolaChangePercent * IIf(m_Aux.m_bWeightedVolaShift And aPos.VegaWeight > BAD_DOUBLE_VALUE, aPos.VegaWeight, 1)
    
    If m_Aux.m_enSpotShift = enCommon Then
        aPos.SpotChange = m_Aux.SimulationParams.m_dSpotChangePercent
    ElseIf (m_Aux.m_enSpotShift = enCorrelated) Then
    
        dPercentShift = 1# + CDbl(m_Aux.SimulationParams.m_dSpotChangePercent) / 100#
        
        dBeta = GetBeta(aFrom)
        
        If (dPercentShift > 0) Then
            dRatio = (Exp(Log(dPercentShift) * dBeta) - 1#)
            aPos.SpotChange = CLng(dRatio * 100#)
        Else
            aPos.SpotChange = 0
        End If
        
    Else
        sKey = aFrom.Symbol & CStr(m_Aux.m_nShiftID)
        Set aStockShift = m_Aux.m_StockShifts(sKey)

        If Not aStockShift Is Nothing Then
            aPos.SpotChange = aStockShift.Shift
        Else
            aPos.SpotChange = 0
        End If
    End If
    
End Sub

Private Sub AddSimParamsToUnd(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aFrom As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim aStockShift As clsStockShift
    Dim sKey As String
    Dim dPercentShift As Double
    Dim dRatio As Double
    Dim dBeta As Double
    
    If m_Aux.m_enSpotShift = enCommon Then
        aUnd.SpotChange = m_Aux.SimulationParams.m_dSpotChangePercent
    ElseIf (m_Aux.m_enSpotShift = enCorrelated) Then
    
        dPercentShift = 1# + CDbl(m_Aux.SimulationParams.m_dSpotChangePercent) / 100#
        
        dBeta = GetBeta(aFrom)
        
        If (dPercentShift > 0) Then
            dRatio = (Exp(Log(dPercentShift) * dBeta) - 1#)
            aUnd.SpotChange = dRatio * 100#
        Else
            aUnd.SpotChange = 0
        End If
        
    Else
        sKey = aFrom.Symbol & CStr(m_Aux.m_nShiftID)
        Set aStockShift = m_Aux.m_StockShifts(sKey)
        
        If Not aStockShift Is Nothing Then
            aUnd.SpotChange = aStockShift.Shift
        Else
            aUnd.SpotChange = 0
        End If
    End If
    
    aUnd.SimulationDate = m_Aux.SimulationParams.m_dDate
    aUnd.VolaChange = m_Aux.SimulationParams.m_dVolaChangePercent
    
    Set aStockShift = Nothing
End Sub
