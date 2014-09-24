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
Public m_bHaveFlexOptions As Boolean     ' true if riskview contains flex options - need it it to enforce recalc even when
                                        ' there are not any quotes from feed
Public UndUpdated&
Public OptUpdated&
Public FutUpdated&
Private m_nGreeksMask&

Private m_ProcessDelay As EtsGeneralLib.EtsProcessDelay
Private m_dtCalcDateShift As Date

Public UndCount&
Public OptCount&
Public FutCount&

Private m_Aux As clsAuxRiskView

Public Property Let CalcDate(ShiftDate As Date)
    m_dtCalcDateShift = ShiftDate
End Property

Public Property Get CalcDate() As Date
    CalcDate = m_dtCalcDateShift
End Property

Public Property Get QuoteReqsAll() As EtsMmRisksLib.MmRvReqColl
    Set QuoteReqsAll = m_Aux.RiskView.QuoteReqsAll
End Property

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
    m_dtCalcDateShift = GetNewYorkTime
    Set m_Aux = aAux
    Set m_ProcessDelay = New EtsGeneralLib.EtsProcessDelay
End Sub

Public Sub Term()
    On Error Resume Next
    
    m_bShutDown = True
    m_ProcessDelay.Interrupt
    
    Set m_ProcessDelay = Nothing
    Set m_Aux = Nothing
End Sub

Public Function CantStartRealTime() As Boolean
    CantStartRealTime = CBool(OptCount > g_Params.MaxRTOptCount)
    If CantStartRealTime Then
        MsgBox "Can't start Real Time Mode because Options number is " & OptCount & " and it must be less or equal " & g_Params.MaxRTOptCount & ". Please use filters to decrease Options number.", vbExclamation
    End If
End Function

Public Sub UnderlyingsCalc(ByVal bCalcGreeks As Boolean, ByVal bUpdateVola As Boolean, Optional ByVal bRealtimeCalc As Boolean = False, Optional ByVal bTotalsOnly As Boolean = False)
    On Error Resume Next
    If m_bShutDown Or m_bInCalc Then Exit Sub
    m_bInCalc = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogSystem, "UnderlyingsCalc Enter", "clsAuxRiskViewCalc"
    
    If g_Params.UseCalcSleep Then
        m_ProcessDelay.Reset
        m_ProcessDelay.Frequency = g_Params.CalcSleepFreq
        m_ProcessDelay.Duration = g_Params.CalcSleepAmt
    End If
    
    Err.Clear
    
    
    m_Aux.RiskView.Calc bTotalsOnly, m_nGreeksMask, bCalcGreeks, bUpdateVola, bRealtimeCalc, _
                IIf(g_Params.UseCalcSleep, m_ProcessDelay, Nothing), _
                (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
                g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
                g_Params.PriceRoundingRule, OptUpdated, UndUpdated, FutUpdated, m_dtCalcDateShift, g_Main.CalculationParametrs
                
    Debug.Assert Err.Number = 0
    m_bInCalc = False
    
    If Err.Number = 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogSystem, "UnderlyingsCalc Exit", "clsAuxRiskViewCalc"
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogFaults, "FAIL: UnderlyingsCalc Exit with error " & CStr(Err.Number) & " " & Err.Description, "clsAuxRiskViewCalc"
    End If
    
End Sub

Public Sub UnderlyingCalc(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal bCalcGreeks As Boolean, ByVal bUpdateVola As Boolean, _
                        Optional ByVal bRealtimeCalc As Boolean = False, Optional ByVal bClearSynthGreeks As Boolean = True)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
       
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogSystem, "UnderlyingCalc Enter", "clsAuxRiskViewCalc"
    
    Err.Clear
    aUnd.Calc m_Aux.Und, m_nGreeksMask, bCalcGreeks, bUpdateVola, bRealtimeCalc, bClearSynthGreeks, Nothing, _
                (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
                g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
                g_Params.PriceRoundingRule, 0, 0, 0, m_dtCalcDateShift, g_Main.CalculationParametrs
                
    Debug.Assert Err.Number = 0
    
    If Err.Number = 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogSystem, "UnderlyingCalc Exit", "clsAuxRiskViewCalc"
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogFaults, "FAIL: UnderlyingCalc Exit with error " & CStr(Err.Number) & " " & Err.Description, "clsAuxRiskViewCalc"
    End If
        
End Sub

Public Sub UnderlyingsCalcWtdVega()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    m_Aux.Grp.CalcWtdVega m_Aux.Und, g_ExpCalendar
    Debug.Assert Err.Number = 0
End Sub
Public Sub ChangeQtyDailyLTD(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                             ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                             ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
                             
    On Error Resume Next
    Dim nQty&, nQtyInShares&
        
    nQty = aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
    nQtyInShares = nQty * aPos.Quote.LotSize
    
    Dim dPriceClose As Double
    dPriceClose = aPos.Quote.price.Close
    If (Not g_Main Is Nothing) Then
        If (g_Main.UseTheoCloseForPNL = True And aPos.Quote.price.TheoClose > 0#) Then
            dPriceClose = aPos.Quote.price.TheoClose
        End If
    End If
            
    If aTrd.IsBuy Then
        If aPos.QtyLTDBuy = BAD_LONG_VALUE Then aPos.QtyLTDBuy = 0
        aPos.QtyLTDBuy = aPos.QtyLTDBuy + nQtyInShares
        If aTrd.IsPosition Then
            If dPriceClose >= 0# Then
                If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
                aPos.PosLTDBuy = aPos.PosLTDBuy + dPriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
            aPos.PosLTDBuy = aPos.PosLTDBuy + aTrd.price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateBuy = 0
            aPos.QtyDailyPrevDateBuy = aPos.QtyDailyPrevDateBuy + nQtyInShares
            If dPriceClose >= 0# Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + dPriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aTrd.price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodayBuy = BAD_LONG_VALUE Then aPos.QtyDailyTodayBuy = 0
            aPos.QtyDailyTodayBuy = aPos.QtyDailyTodayBuy + nQtyInShares
            If aTrd.IsPosition Then
                If dPriceClose >= 0# Then
                    If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                    aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + dPriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aTrd.price * nQtyInShares
            End If
        End If
    Else
        If aPos.QtyLTDSell = BAD_LONG_VALUE Then aPos.QtyLTDSell = 0
        aPos.QtyLTDSell = aPos.QtyLTDSell + nQtyInShares
        If aTrd.IsPosition Then
            If dPriceClose >= 0# Then
                If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
                aPos.PosLTDSell = aPos.PosLTDSell + dPriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
            aPos.PosLTDSell = aPos.PosLTDSell + aTrd.price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateSell = 0
            aPos.QtyDailyPrevDateSell = aPos.QtyDailyPrevDateSell + nQtyInShares
            If dPriceClose >= 0# Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + dPriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aTrd.price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodaySell = BAD_LONG_VALUE Then aPos.QtyDailyTodaySell = 0
            aPos.QtyDailyTodaySell = aPos.QtyDailyTodaySell + nQtyInShares
            If aTrd.IsPosition Then
                If dPriceClose >= 0# Then
                    If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                    aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + dPriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aTrd.price * nQtyInShares
            End If
        End If
    End If
    
    aPos.Qty = aPos.Qty + nQty
    aPos.QtyInShares = aPos.QtyInShares + nQtyInShares
    
End Sub

Public Sub CalcOptionGreeks(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, ByVal nMask As Long)
     On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogSystem, "CalcOptionGreeks Enter", "clsAuxRiskViewCalc"
    
    Err.Clear
    aPos.CalcOptionGreeks m_Aux.Und, aUnd, _
            aUnd.UndPriceProfile.GetUndPriceMid(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule), _
            m_nGreeksMask, (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, m_dtCalcDateShift, g_Main.CalculationParametrs

    Debug.Assert Err.Number = 0
    
    If Err.Number = 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogSystem, "CalcOptionGreeks Exit", "clsAuxRiskViewCalc"
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogFaults, "FAIL: CalcOptionGreeks Exit with error " & CStr(Err.Number) & " " & Err.Description, "clsAuxRiskViewCalc"
    End If
    
End Sub

Public Sub CalcFutOptionGreeks(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, ByVal nMask As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogSystem, "CalcFutOptionGreeks Enter", "clsAuxRiskViewCalc"
    
    Err.Clear
    aPos.CalcFutOptionGreeks aUnd, _
            aUnd.UndPriceProfile.GetUndPriceMid(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule), _
            m_nGreeksMask, (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, m_dtCalcDateShift, g_Main.CalculationParametrs

    Debug.Assert Err.Number = 0
    
    If Err.Number = 0 Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogSystem, "CalcFutOptionGreeks Exit", "clsAuxRiskViewCalc"
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogFaults, "FAIL: CalcFutOptionGreeks Exit with error " & CStr(Err.Number) & " " & Err.Description, "clsAuxRiskViewCalc"
    End If
    
End Sub

Public Sub SyntheticUnderlyingCalc(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    Err.Clear
    aUnd.CalcSyntheticUnderlying m_Aux.Und, m_nGreeksMask, _
            (g_Params.PnLCalcType = PNLCT_LTD), g_Params.CalcModel, g_Params.UseTheoVolatility, _
            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
            g_Params.PriceRoundingRule, m_dtCalcDateShift, g_Main.CalculationParametrs
    Debug.Assert Err.Number = 0
End Sub

Public Sub InitVola(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    If Not aUnd.VolaSrv.IsInitialized Then
        Err.Clear
        aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), m_Aux.VolaSource
        If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
    End If
End Sub

Public Sub UnderlyingsAdjustRates(ByVal bForceRateUpdate As Boolean)
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    For Each aUnd In m_Aux.Und
        UnderlyingAdjustRates aUnd, bForceRateUpdate
    Next
End Sub

Public Function UnderlyingAdjustRates(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal bForceRateUpdate As Boolean) As Boolean
On Error GoTo error_handler
    
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
    Exit Function
error_handler:
    MsgBox "UnderlyingAdjustRates failed with error (" & Err.Number & ", " & Err.Description & ")"
End Function

Public Sub ClearPosQty(ByRef aPos As EtsMmRisksLib.MmRvPosAtom)
    On Error Resume Next
    
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
End Sub


