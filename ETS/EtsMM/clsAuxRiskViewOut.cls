VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxRiskViewOut"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_Aux As clsAuxRiskView

Public gdFlt As clsGridDef
Public gdTot As clsGridDef
Public gdPos As clsGridDef

Public fgPos As VSFlex7Ctl.VSFlexGrid
Public fgTot As VSFlex7Ctl.VSFlexGrid
Public imgBadPrice As vB.Image

Public Sub Init(ByRef aAux As clsAuxRiskView)
    On Error Resume Next
    Set m_Aux = aAux
End Sub

Private Sub Class_Terminate()
On Error Resume Next
    
    Set m_Aux = Nothing
    
    Set gdFlt = Nothing
    Set gdTot = Nothing
    Set gdPos = Nothing
    
    Set fgPos = Nothing
    Set fgTot = Nothing
    
End Sub

Public Sub Term()
On Error Resume Next
    
    Set m_Aux = Nothing
    
    Set gdFlt = Nothing
    Set gdTot = Nothing
    Set gdPos = Nothing
    
    Set fgPos = Nothing
    Set fgTot = Nothing
    
End Sub

Public Sub UnderlyingsUpdate(ByVal bSymbol As Boolean, Optional ByVal bRealTimeUpdate As Boolean = False)
    On Error Resume Next
    Dim nRow&
    Dim aRowData As EtsMmRisksLib.MmRvRowData
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aFut As EtsMmRisksLib.MmRvFutAtom

    With fgPos
        If bRealTimeUpdate Then
            m_Aux.Idx.CalcGreeks = False
            m_Aux.Idx.CalcTotals = False
            
            ' clear update status
            For Each aUnd In m_Aux.Und
                aUnd.CalcGreeks = False
                aUnd.CalcTotals = False
                
                For Each aPos In aUnd.Pos
                    aPos.CalcGreeks = False
                Next
            
                If Not aUnd.Fut Is Nothing Then
                    For Each aFut In aUnd.Fut
                        aFut.CalcGreeks = False
                    Next
                End If
            Next
        End If
    End With
End Sub

Public Sub UnderlyingsUpdateBadStatus()
    On Error Resume Next
    Dim nRow&

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        nRow = .GetNodeRow(1, flexNTFirstSibling)
        While nRow > 0
            UnderlyingUpdateBadStatus nRow
            nRow = .GetNodeRow(nRow, flexNTNextSibling)
        Wend
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub UnderlyingUpdateBadStatus(ByVal nRow&)
    On Error Resume Next
    Dim nCol&, aUnd As EtsMmRisksLib.MmRvUndAtom, aRowData As EtsMmRisksLib.MmRvRowData

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        Set aRowData = m_Aux.RiskView.PosRowData(nRow)
        If Not aRowData.Und Is Nothing Then
            Set aUnd = aRowData.Und
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= RPC_LAST_COLUMN
                nCol = i + 1
            
                Select Case nIdx
                    Case RPC_PNL_MTM
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlMtm, gdPos.Col(RPC_PNL_MTM).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_PNL_MTM).ForeColor)
                    
                    Case RPC_PNL_THEO
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlTheo, gdPos.Col(RPC_PNL_THEO).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_PNL_THEO).ForeColor)
            
                    Case RPC_PNL_EDGE
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlMtm Or aUnd.BadPnlTheo, gdPos.Col(RPC_PNL_EDGE).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_PNL_EDGE).ForeColor)
            
                    Case RPC_NET_DELTA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_NET_DELTA).ForeColor)
            
                    Case RPC_NET_DELTA_USD
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadDeltaEq, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                    Case RPC_NET_EXPOSURE
                        Dim aAgg As IMmRvAggregationDataAtom
                        Set aAgg = aUnd
                        
                       .Cell(flexcpForeColor, nRow, nCol) = IIf(aAgg.BadNetExposure, gdPos.Col(RPC_NET_EXPOSURE).ForeColorAlt1, _
                                                                              gdPos.Col(RPC_NET_EXPOSURE).ForeColor)
            
                    Case RPC_OPT_DELTA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadOptDelta, gdPos.Col(RPC_OPT_DELTA).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_OPT_DELTA).ForeColor)
            
                    Case RPC_GAMMA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGammaPerc, gdPos.Col(RPC_GAMMA).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_GAMMA).ForeColor)
                    
                    Case RPC_NET_GAMMA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetGamma, gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_NET_GAMMA).ForeColor)
            
            
                    Case RPC_GAMMA_SHARES
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGamma, gdPos.Col(RPC_GAMMA_SHARES).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_GAMMA_SHARES).ForeColor)
            
                    Case RPC_VEGA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVega, gdPos.Col(RPC_VEGA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_VEGA).ForeColor)
        
                    Case RPC_WTD_VEGA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadWtdVega, gdPos.Col(RPC_WTD_VEGA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_WTD_VEGA).ForeColor)
            
                    Case RPC_THETA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTheta, gdPos.Col(RPC_THETA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_THETA).ForeColor)
            
                    Case RPC_RHO
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadRho, gdPos.Col(RPC_RHO).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_RHO).ForeColor)
            
                    Case RPC_THETA_DELTA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadThetaDelta, gdPos.Col(RPC_THETA_DELTA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_THETA_DELTA).ForeColor)
            
                    Case RPC_THETA_GAMMA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadThetaGamma, gdPos.Col(RPC_THETA_GAMMA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_THETA_GAMMA).ForeColor)
            
                    Case RPC_VEGA_DELTA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVegaDelta, gdPos.Col(RPC_VEGA_DELTA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_VEGA_DELTA).ForeColor)
            
                    Case RPC_VEGA_GAMMA
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVegaGamma, gdPos.Col(RPC_VEGA_GAMMA).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_VEGA_GAMMA).ForeColor)
            
                    Case RPC_TIME_VALUE
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTimeValue, gdPos.Col(RPC_TIME_VALUE).ForeColorAlt1, _
                                                                            gdPos.Col(RPC_TIME_VALUE).ForeColor)
            
                    Case RPC_BETA_WTD_DELTA_USD
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadBetaWtdDeltaEq, gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                                                                gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                End Select
    
                i = i + 1
                nIdx = gdPos.Idx(i)
            Wend
            
            Set aUnd = Nothing
        End If
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub SyntheticGreeksUpdate(ByVal nRow&, ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim nCol&, bUpdateOptSymbol As Boolean: bUpdateOptSymbol = False
    Dim bUpdateSynth As Boolean: bUpdateSynth = False
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aRowData As EtsMmRisksLib.MmRvRowData, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom

    With fgPos
        Set aRowData = .RowData(nRow) 'm_Aux.RiskView.PosRowData(nRow)
        
        If Not aRowData Is Nothing And Not aRowData.SynthGreeks Is Nothing And Not aRowData.Und Is Nothing Then
                
            Set aSynthGreek = aRowData.SynthGreeks
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
            
            If bSymbol Then bUpdateOptSymbol = (aPos.ContractType = enCtOption)
            
            Set aSynthUnd = m_Aux.Und(aSynthGreek.SynthUndID)
            bUpdateSynth = aSynthGreek.IsTotal And Not aSynthUnd Is Nothing
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= RPC_LAST_COLUMN
                nCol = i + 1
            
                If bSymbol Then
                    If bUpdateOptSymbol Then
                        Select Case nIdx
                            Case RPC_SYMBOL
                                .TextMatrix(nRow, nCol) = aSynthGreek.Symbol
                            
                            Case RPC_OPT_TYPE
                                .TextMatrix(nRow, nCol) = IIf(aPos.OptType = enOtCall, "C", "P")
                            
                            Case RPC_EXPIRY
                                .TextMatrix(nRow, nCol) = aPos.Expiry
                            
                            Case RPC_STRIKE
                                .TextMatrix(nRow, nCol) = IIf(aPos.Strike > BAD_DOUBLE_VALUE, aPos.Strike, STR_NA)
                            
                            Case RPC_UND
                                .TextMatrix(nRow, nCol) = aSynthGreek.SynthUndSymbol
                            
                            Case RPC_DEL_UNIT
                                .TextMatrix(nRow, nCol) = aSynthGreek.DeliveryUnits
                                
                            Case RPC_FUT_ROOT, RPC_FUTURES, RPC_FUT_MATURITY
                                .TextMatrix(nRow, nCol) = ""
                        End Select
                    Else
                        Select Case nIdx
                            Case RPC_SYMBOL
                                .TextMatrix(nRow, nCol) = aSynthGreek.Symbol
                            
                            Case RPC_OPT_TYPE, RPC_EXPIRY, RPC_STRIKE, RPC_UND, RPC_DEL_UNIT, _
                                RPC_FUT_ROOT, RPC_FUTURES, RPC_FUT_MATURITY
                                .TextMatrix(nRow, nCol) = ""
                        End Select
                    End If
                End If
                
                If bUpdateSynth Then
                    Select Case nIdx
                        Case RPC_NETCHANGE
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.price.NetChange <> BAD_DOUBLE_VALUE, aSynthUnd.price.NetChange, STR_NA)
                        Case RPC_BID
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.price.Bid > BAD_DOUBLE_VALUE, aSynthUnd.price.Bid, STR_NA)
                            
                            If aSynthUnd.ReplacePriceStatus And enRpsBid Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                    
                        Case RPC_ASK
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.price.Ask > BAD_DOUBLE_VALUE, aSynthUnd.price.Ask, STR_NA)
                            
                            If aSynthUnd.ReplacePriceStatus And enRpsAsk Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                    
                        Case RPC_LAST
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.price.Last > BAD_DOUBLE_VALUE, aSynthUnd.price.Last, STR_NA)
                    
'                        Case RPC_SYNTHETIC_PRICE
'                            .TextMatrix(nRow, nCol) = IIf(aSynthGreek.SuPrice > BAD_DOUBLE_VALUE, aSynthGreek.SuPrice, STR_NA)
                    
                        Case RPC_CLOSE
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.price.Close > 0#, aSynthUnd.price.Close, STR_NA)
                    End Select
                Else
                    Select Case nIdx
                        Case RPC_NETCHANGE, RPC_BID, RPC_ASK, RPC_LAST, RPC_CLOSE
                            .TextMatrix(nRow, nCol) = ""
'                        Case RPC_SYNTHETIC_PRICE
'                            .TextMatrix(nRow, nCol) = IIf(aSynthGreek.SuPrice > BAD_DOUBLE_VALUE, aSynthGreek.SuPrice, STR_NA)
                    End Select
                End If
                
                Select Case nIdx
                    Case RPC_PNL_MTM, RPC_PNL_THEO, RPC_PNL_EDGE, RPC_UND_POS, RPC_OPT_QTY, _
                        RPC_VEGA, RPC_WTD_VEGA, RPC_THETA, RPC_RHO, RPC_THETA_DELTA, RPC_THETA_GAMMA, _
                        RPC_VEGA_DELTA, RPC_VEGA_GAMMA, RPC_TIME_VALUE, RPC_FUT_QTY
                        .TextMatrix(nRow, nCol) = ""
                                                
                    Case RPC_NET_DELTA
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                    gdPos.Col(RPC_NET_DELTA).ForeColor)
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                            If aSynthGreek.IsTotal Then _
                                .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                 gdPos.Col(RPC_NET_DELTA).ForeColor)
                        End If
                            
                    Case RPC_NET_DELTA_USD
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInMoney, STR_NA)
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInMoney, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                    gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInMoney, STR_NA)
                            If aSynthGreek.IsTotal Then _
                                .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInMoney, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                 gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                        End If
                            
                    Case RPC_OPT_DELTA
                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_OPT_DELTA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_OPT_DELTA).ForeColor)
                
                    Case RPC_GAMMA
                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaInSharesPerc > BAD_DOUBLE_VALUE, aSynthGreek.GammaInSharesPerc, STR_NA)
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGammaPerc, gdPos.Col(RPC_GAMMA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_GAMMA).ForeColor)
    
                    Case RPC_NET_GAMMA
                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.NetGamma > BAD_DOUBLE_VALUE, aSynthGreek.NetGamma, STR_NA)
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadNetGamma, gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_NET_GAMMA).ForeColor)
                    
                    Case RPC_GAMMA_SHARES
                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaInShares > BAD_DOUBLE_VALUE, aSynthGreek.GammaInShares, STR_NA)
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGamma, gdPos.Col(RPC_GAMMA_SHARES).ForeColorAlt1, _
                                                        gdPos.Col(RPC_GAMMA_SHARES).ForeColor)
                
                    Case RPC_BETA_WTD_DELTA_USD
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aSynthGreek.BetaWtdDeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.BetaWtdDeltaInMoney, STR_NA)
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadBetaWtdDeltaInMoney, gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                    gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = IIf(aSynthGreek.BetaWtdDeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.BetaWtdDeltaInMoney, STR_NA)
                            If aSynthGreek.IsTotal Then _
                                .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadBetaWtdDeltaInMoney, gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                 gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                        End If
                End Select
    
                i = i + 1
                nIdx = gdPos.Idx(i)
            Wend
                        
            Set aSynthUnd = Nothing
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        End If
        Set aUnd = Nothing
        Set aPos = Nothing
        Set aRowData = Nothing
    End With
End Sub

Public Sub TotalsUpdate()
    On Error Resume Next
    Dim nCol&

    With fgTot
        m_Aux.GridLock(GT_RISKS_TOTALS).LockRedraw
        
        Dim i&, nIdx&
        i = 0
        nIdx = gdTot.Idx(0)
        While nIdx >= 0 And i <= g_RTC_LAST_COLUMN
            nCol = i
            
            '--------------------------------------------------------------------------
            Dim cvID As Long
            Dim dCVRealVal As Variant
            
            cvID = BAD_LONG_VALUE
            cvID = m_Aux.RiskView.getCvIDbyColumn(CVLEVEL_GRP, nIdx)
            If cvID <> 0 Then
                dCVRealVal = m_Aux.RiskView.cvRTContext.GetCVValue(m_Aux.RiskView.Grp.cvPoolID, cvID)
                .TextMatrix(1, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE, dCVRealVal, STR_NA)
            End If
            
            '--------------------------------------------------------------------------
            
            Select Case nIdx
                Case RTC_PNL_MTM
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.PnlMtm > BAD_DOUBLE_VALUE, m_Aux.Grp.PnlMtm, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm, gdTot.Col(RTC_PNL_MTM).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_MTM).ForeColor)
        
                Case RTC_PNL_THEO
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.PnlTheo > BAD_DOUBLE_VALUE, m_Aux.Grp.PnlTheo, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_THEO).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_THEO).ForeColor)
        
                Case RTC_PNL_EDGE
                    If m_Aux.Grp.PnlTheo > BAD_DOUBLE_VALUE And m_Aux.Grp.PnlMtm > BAD_DOUBLE_VALUE Then
                        .TextMatrix(1, nCol) = m_Aux.Grp.PnlTheo - m_Aux.Grp.PnlMtm
                    Else
                        .TextMatrix(1, nCol) = STR_NA
                    End If
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm Or m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_EDGE).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_EDGE).ForeColor)
            
                Case RTC_VEGA
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Vega > BAD_DOUBLE_VALUE, m_Aux.Grp.Vega, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadVega, gdTot.Col(RTC_VEGA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_VEGA).ForeColor)
        
                Case RTC_WTD_VEGA
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.WtdVega > BAD_DOUBLE_VALUE, m_Aux.Grp.WtdVega, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadWtdVega, gdTot.Col(RTC_WTD_VEGA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_WTD_VEGA).ForeColor)
        
                Case RTC_THETA
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Theta > BAD_DOUBLE_VALUE, m_Aux.Grp.Theta, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadTheta, gdTot.Col(RTC_THETA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_THETA).ForeColor)
        
                Case RTC_DELTA_EQ
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.DeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.DeltaEq, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadDeltaEq, gdTot.Col(RTC_DELTA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_DELTA_EQ).ForeColor)
        
                Case RTC_GAMMA_EQ
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetGammaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.NetGammaEq, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGammaEq, gdTot.Col(RTC_GAMMA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_GAMMA_EQ).ForeColor)
        
                Case RTC_RHO
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Rho > BAD_DOUBLE_VALUE, m_Aux.Grp.Rho, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadRho, gdTot.Col(RTC_RHO).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_RHO).ForeColor)
        
                Case RTC_INDEX_DELTA_EQ
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.IdxDeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.IdxDeltaEq, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadIdxDeltaEq, gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColor)
        
                Case RTC_BETA_WTD_DELTA
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.BetaWtdDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.BetaWtdDelta, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDelta, gdTot.Col(RTC_BETA_WTD_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_BETA_WTD_DELTA).ForeColor)
        
                Case RTC_OPT_DELTA
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.OptDeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.OptDeltaEq, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadOptDeltaEq, gdTot.Col(RTC_OPT_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_OPT_DELTA).ForeColor)
        
'                Case RTC_NET_DELTA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.NetDelta, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetDelta, gdTot.Col(RTC_NET_DELTA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_DELTA).ForeColor)
'
'                Case RTC_NET_GAMMA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetGamma > BAD_DOUBLE_VALUE, m_Aux.Grp.NetGamma, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGamma, gdTot.Col(RTC_NET_GAMMA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_GAMMA).ForeColor)
        
                Case RTC_BETA_WTD_DELTA_USD
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.BetaWtdDeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.BetaWtdDeltaEq, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDeltaEq, gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColor)
                                                                            
            Case RTC_NET_EXPOSURE_LONG
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetExposureLong > BAD_DOUBLE_VALUE, m_Aux.Grp.NetExposureLong, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetExposure, gdTot.Col(RTC_NET_EXPOSURE_LONG).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_NET_EXPOSURE_LONG).ForeColor)
            Case RTC_NET_EXPOSURE_SHORT
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetExposureShort > BAD_DOUBLE_VALUE, m_Aux.Grp.NetExposureShort, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetExposure, gdTot.Col(RTC_NET_EXPOSURE_SHORT).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_NET_EXPOSURE_SHORT).ForeColor)
            Case RTC_NET_EXPOSURE
                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetExposure > BAD_DOUBLE_VALUE, m_Aux.Grp.NetExposure, STR_NA)
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetExposure, gdTot.Col(RTC_NET_EXPOSURE).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_NET_EXPOSURE).ForeColor)
                                                                            
            End Select

            i = i + 1
            nIdx = gdTot.Idx(i)
        Wend
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub

Public Sub TotalsUpdateBadStatus()
    On Error Resume Next
    Dim nCol&

    With fgTot
        m_Aux.GridLock(GT_RISKS_TOTALS).LockRedraw
        
        Dim i&, nIdx&
        i = 0
        nIdx = gdTot.Idx(0)
        While nIdx >= 0 And i <= g_RTC_LAST_COLUMN
            nCol = i
            Select Case nIdx
                Case RTC_PNL_MTM
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm, gdTot.Col(RTC_PNL_MTM).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_MTM).ForeColor)
        
                Case RTC_PNL_THEO
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_THEO).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_THEO).ForeColor)
        
                Case RTC_PNL_EDGE
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm Or m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_EDGE).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_EDGE).ForeColor)
            
                Case RTC_VEGA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadVega, gdTot.Col(RTC_VEGA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_VEGA).ForeColor)
        
                Case RTC_WTD_VEGA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadWtdVega, gdTot.Col(RTC_WTD_VEGA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_WTD_VEGA).ForeColor)
        
                Case RTC_THETA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadTheta, gdTot.Col(RTC_THETA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_THETA).ForeColor)
        
                Case RTC_DELTA_EQ
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadDeltaEq, gdTot.Col(RTC_DELTA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_DELTA_EQ).ForeColor)
        
                Case RTC_GAMMA_EQ
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGammaEq, gdTot.Col(RTC_GAMMA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_GAMMA_EQ).ForeColor)
        
                Case RTC_RHO
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadRho, gdTot.Col(RTC_RHO).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_RHO).ForeColor)
        
                Case RTC_INDEX_DELTA_EQ
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadIdxDeltaEq, gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColor)
        
                Case RTC_BETA_WTD_DELTA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDelta, gdTot.Col(RTC_BETA_WTD_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_BETA_WTD_DELTA).ForeColor)
        
                Case RTC_OPT_DELTA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadOptDeltaEq, gdTot.Col(RTC_OPT_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_OPT_DELTA).ForeColor)
        
'                Case RTC_NET_DELTA
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetDelta, gdTot.Col(RTC_NET_DELTA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_DELTA).ForeColor)
'
'                Case RTC_NET_GAMMA
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGamma, gdTot.Col(RTC_NET_GAMMA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_GAMMA).ForeColor)
        
                Case RTC_BETA_WTD_DELTA_USD
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDeltaEq, gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColor)
            End Select

            i = i + 1
            nIdx = gdTot.Idx(i)
        Wend
        
        m_Aux.GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub



