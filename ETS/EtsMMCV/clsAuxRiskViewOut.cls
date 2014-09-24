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
Public imgBadPrice As VB.Image

Public Sub Init(ByRef aAux As clsAuxRiskView)
    On Error Resume Next
    Set m_Aux = aAux
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_Aux = Nothing
End Sub

Public Sub UnderlyingsUpdate(ByVal bSymbol As Boolean, Optional ByVal bRealTimeUpdate As Boolean = False)
    On Error Resume Next
    Dim nRow&, aRowData As clsRvRowData
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aFut As EtsMmRisksLib.MmRvFutAtom

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        For nRow = 1 To .Rows - 1
            Set aRowData = .RowData(nRow)
            If Not aRowData Is Nothing Then
                If .RowOutlineLevel(nRow) = ROL_POS Then
                    If aRowData.Pos.IsRecalculated Or aRowData.Pos.IsSynthetic Or _
                       aRowData.Pos.ContractType <> enCtOption Then
                        PositionUpdate nRow, bSymbol, bRealTimeUpdate
                        aRowData.Pos.IsRecalculated = False
                        
                        DoEvents
                    End If
                Else
                    If aRowData.Und.IsRecalculated Then
                        UnderlyingUpdate nRow, bSymbol, bRealTimeUpdate
                        aRowData.Und.IsRecalculated = False
                        
                        DoEvents
                    End If
                End If
            End If
        Next

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

        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
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

Public Sub PositionsUpdate(ByVal nUndRow&, ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim nRow&, nFirstRow&, nLastRow&

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        nFirstRow = .GetNodeRow(nUndRow, flexNTFirstChild)
        nLastRow = .GetNodeRow(nUndRow, flexNTLastChild)
        If nFirstRow > 0 And nLastRow > 0 Then
            For nRow = nFirstRow To nLastRow
                PositionUpdate nRow, bSymbol
                DoEvents
            Next
        End If
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub SyntheticPositionsUpdate(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim aUnd2 As EtsMmRisksLib.MmRvUndAtom, sKey$, nRow&

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        For Each aUnd2 In m_Aux.Und
            sKey = CStr(aUnd2.ID) & "_" & CStr(aUnd.ID)
            nRow = .FindRow(sKey, 1, RPC_KEY, , True)
            
            If nRow > 0 Then
                sKey = CStr(aUnd2.ID) & "_" & CStr(aUnd2.ID)
                nRow = fgPos.FindRow(sKey, 1, RPC_KEY, , True)
                If nRow > 0 Then
                    UnderlyingUpdate nRow, False
                    PositionsUpdate nRow, False
                End If
            End If
        Next
    
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub SyntheticPositionGreeksUpdate(ByVal nUndID&, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim sKey$, nRow&, aSynthGreek  As EtsMmRisksLib.MmRvSynthGreeksAtom
    
    If aPos.SynthGreeks Is Nothing Then Exit Sub
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        For Each aSynthGreek In aPos.SynthGreeks
            sKey = CStr(nUndID) & "_" & CStr(aSynthGreek.SynthUndID) & "_" & CStr(aSynthGreek.ContractID)
            
            nRow = .FindRow(sKey, 1, RPC_KEY, , True)
            
            If nRow > 0 Then
                SyntheticGreeksUpdate nRow, bSymbol
            Else
                Debug.Assert False
            End If
        Next
    
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub SyntheticUnderlyingGreeksUpdate(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim sKey$, nRow&, aSynthGreek  As EtsMmRisksLib.MmRvSynthGreeksAtom
    
    If aUnd.SynthGreeks Is Nothing Then Exit Sub
    
    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
        
        For Each aSynthGreek In aUnd.SynthGreeks
            sKey = CStr(aUnd.ID) & "_" & CStr(aSynthGreek.SynthUndID)
            
            nRow = .FindRow(sKey, 1, RPC_KEY, , True)
            
            If nRow > 0 Then
                SyntheticGreeksUpdate nRow, bSymbol
            Else
                Debug.Assert False
            End If
        Next
        
        m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub
Private Sub PositionUpdateWithSimulation(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal nRow As Long, ByVal nCol As Long, _
                                          ByVal realValue As Double, ByVal simulatedValue As Double, ByVal nColID As Long, Optional ByVal dMultiplier As Double = 1)
    With fgPos
        If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing And gdPos.Col(nColID&).SupportSimulation = True _
                            And gdPos.Col(nColID&).UseInSimulation = True Then
            If gdPos.Col(nColID&).ShowDeltaInSimulation Then
                .TextMatrix(nRow, nCol) = IIf(realValue > BAD_DOUBLE_VALUE And simulatedValue > BAD_DOUBLE_VALUE, simulatedValue * dMultiplier - realValue * dMultiplier, STR_NA)
            Else
                .TextMatrix(nRow, nCol) = IIf(simulatedValue > BAD_DOUBLE_VALUE, simulatedValue * dMultiplier, STR_NA)
            End If
        Else
            .TextMatrix(nRow, nCol) = IIf(realValue > BAD_DOUBLE_VALUE, realValue * dMultiplier, STR_NA)
        End If
        
'        If (nColID = RPC_PNL_THEO) Then
'            If (realValue < 0# And simulatedValue < 0#) Then
'                If (aPos.PriceLast > 0) Then
'                    If Not g_PerformanceLog Is Nothing Then
'
'                        Dim lAssetID As Long
'                        Dim dPriceLast As Double
'                        lAssetID = aPos.UndID
'                        If (Not m_Aux.Und Is Nothing) Then
'                            If (Not m_Aux.Und(lAssetID) Is Nothing) Then
'                                dPriceLast = m_Aux.Und(lAssetID).PriceLast
'                            End If
'                        End If
'
'                        g_PerformanceLog.LogMmInfo enLogFaults, "Symbol: " & CStr(aPos.Symbol) & _
'                                                                " PriceTheo: " & CStr(aPos.PriceTheo) & " PriceLast: " & CStr(aPos.PriceLast) & _
'                                                                " Vola: " & CStr(aPos.Vola) & " MIV: " & CStr(aPos.MIV) & _
'                                                                " SimPriceTheo: " & IIf(Not aPos.SimulationPos Is Nothing, CStr(aPos.SimulationPos.PriceTheo), -1#) & _
'                                                                " SimVola: " & IIf(Not aPos.SimulationPos Is Nothing, CStr(aPos.SimulationPos.Vola), -1#) & _
'                                                                " SimMIV: " & IIf(Not aPos.SimulationPos Is Nothing, CStr(aPos.SimulationPos.MIV), -1#) & _
'                                                                " SpotPriceLast: " & CStr(dPriceLast), "PositionUpdateWithSimulation"
'
'                    End If
'                End If
'            End If
'        End If
    End With
End Sub

Public Sub PositionUpdate(ByVal nRow&, ByVal bSymbol As Boolean, Optional ByVal bRealTimeUpdate As Boolean = False)
    On Error Resume Next
    Dim nCol&, aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aRowData As clsRvRowData
    Dim dSimVal As Double
    Dim bIsPriceUsed As Boolean: bIsPriceUsed = True
                        
    With fgPos
        Set aRowData = .RowData(nRow)
        Set aPos = aRowData.Pos
        Set aUnd = aRowData.Und
        
        If bRealTimeUpdate Then
            Dim bNeedUpdate As Boolean
            
            bNeedUpdate = aUnd.CalcGreeks Or aUnd.CalcTotals Or m_Aux.Idx.CalcTotals
            
            If Not aPos Is Nothing Then
                bNeedUpdate = bNeedUpdate Or aPos.CalcGreeks
                
                If aPos.ContractType = enCtFutOption Then
                    bNeedUpdate = bNeedUpdate Or aPos.Fut.CalcGreeks
                End If
            End If
            
            If Not bNeedUpdate Then
                GoTo Ex
            End If
        End If
        
        If Not aRowData.SynthGreeks Is Nothing Then
            SyntheticGreeksUpdate nRow, bSymbol
            GoTo Ex
        End If
        
        If Not aUnd Is Nothing And Not aPos Is Nothing Then
            Dim enPriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum
                
            If aUnd.ID = aPos.ID Then
                enPriceStatus = aUnd.ReplacePriceStatus
            Else
                enPriceStatus = aPos.ReplacePriceStatus
            End If
            
            If Not aUnd.UndPriceProfile Is Nothing Then _
                bIsPriceUsed = Not aUnd.UndPriceProfile.AlwaysUseLast
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= g_RPC_LAST_COLUMN
                nCol = i + 1
                dSimVal = BAD_DOUBLE_VALUE
                '-------------------------------------------------------------
                Select Case nIdx
                    Case RPC_NET_DELTA, RPC_NET_DELTA_USD, RPC_BETA_WTD_DELTA_USD
                        ' Clear table cell
                        .TextMatrix(nRow, nCol) = ""
                End Select

                '-----------------------------------------------------------
            
                '--------------------------------------------------------------------------
                Dim cvID As Long
                Dim CVRealVal As Variant
                
                cvID = BAD_LONG_VALUE
                cvID = m_Aux.m_cvOptIDs(CStr(nIdx))
                If cvID <> BAD_LONG_VALUE Then
                    .TextMatrix(nRow, nCol) = ""
                    '.TextMatrix(nRow, nCol) = m_Aux.m_aContext.GetCVValue(aPos.cvPoolID, cvID)
                    'If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.PnlMtm
                    'PositionUpdateWithSimulation aPos, nRow, nCol, aPos.PnlMtm, dSimVal, n
                    CVRealVal = m_Aux.m_aContext.GetCVValue(aPos.cvPoolID, cvID)
                    With fgPos
                        If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing And gdPos.Col(nIdx).SupportSimulation = True _
                                                                                            And gdPos.Col(nIdx).UseInSimulation = True Then
                            dSimVal = m_Aux.m_aContext.GetCVValue(aPos.SimulationPos.cvPoolID, cvID)
                            If gdPos.Col(nIdx).ShowDeltaInSimulation And Not IsEmpty(CVRealVal) Then
                                .TextMatrix(nRow, nCol) = IIf(CVRealVal > BAD_DOUBLE_VALUE And dSimVal > BAD_DOUBLE_VALUE, dSimVal - CVRealVal, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = IIf(CVRealVal > BAD_DOUBLE_VALUE, dSimVal, STR_NA)
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = IIf(CVRealVal > BAD_DOUBLE_VALUE, CVRealVal, STR_NA)
                        End If
                    End With
                    
                End If
                '--------------------------------------------------------------------------
            
                If bSymbol Then
                    Select Case aPos.ContractType
                        Case enCtOption
                            Select Case nIdx
                                Case RPC_SYMBOL
                                    .TextMatrix(nRow, nCol) = aPos.Symbol
                                
                                Case RPC_OPT_TYPE
                                    .TextMatrix(nRow, nCol) = IIf(aPos.OptType = enOtCall, "C", "P")
                            
                                Case RPC_EXPIRY
                                    .TextMatrix(nRow, nCol) = aPos.Expiry
                                Case RPC_DTE
                                    '.TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                    '                                DateSerial(Year(Now), Month(Now), Day(Now))
                                    If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing And gdPos.Col(nIdx).SupportSimulation = True _
                                                                                            And gdPos.Col(nIdx).UseInSimulation = True Then
                                        dSimVal = m_Aux.SimulationParams.m_dDate
                                        If gdPos.Col(nIdx).ShowDeltaInSimulation Then
                                            .TextMatrix(nRow, nCol) = DateSerial(Year(dSimVal), Month(dSimVal), Day(dSimVal)) - _
                                                                    DateSerial(Year(Now), Month(Now), Day(Now))
                                        Else
                                            .TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                                                    DateSerial(Year(dSimVal), Month(dSimVal), Day(dSimVal))
                                        End If
                                    Else
                                        .TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                                                    DateSerial(Year(Now), Month(Now), Day(Now))
                                    End If
                                    
                                Case RPC_STRIKE
                                    .TextMatrix(nRow, nCol) = IIf(aPos.Strike > BAD_DOUBLE_VALUE, aPos.Strike, STR_NA)
                            
                                Case RPC_UND
                                    .TextMatrix(nRow, nCol) = aUnd.Symbol
                            
                                Case RPC_DEL_UNIT
                                    If Not aPos.IsSynthetic Then
                                        .TextMatrix(nRow, nCol) = aPos.LotSize
                                    Else
                                        Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
                                        Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
                                        
                                        .TextMatrix(nRow, nCol) = aSynthRoot.SynthRootComponents(aUnd.ID).Weight * aPos.LotSize
                                    End If
                            
                                Case RPC_FUT_ROOT, RPC_FUTURES, RPC_FUT_MATURITY
                                    .TextMatrix(nRow, nCol) = ""
                                    
                
                                Case RPC_IMPORT_ID
                                    .TextMatrix(nRow, nCol) = aPos.ImportID
                                    
                            End Select
                            
                        Case enCtFuture
                            Select Case nIdx
                                Case RPC_SYMBOL
                                    .TextMatrix(nRow, nCol) = aPos.Symbol
                            
                                Case RPC_OPT_TYPE, RPC_EXPIRY, RPC_STRIKE
                                    .TextMatrix(nRow, nCol) = ""
                            
                                Case RPC_UND
                                    .TextMatrix(nRow, nCol) = aUnd.Symbol
                            
                                Case RPC_DEL_UNIT
                                    .TextMatrix(nRow, nCol) = aPos.LotSize
                            
                                Case RPC_FUT_ROOT
                                    .TextMatrix(nRow, nCol) = aPos.Fut.FutRootSymbol
                            
                                Case RPC_FUTURES
                                    .TextMatrix(nRow, nCol) = aPos.Fut.Symbol
                            
                                Case RPC_FUT_MATURITY
                                    .TextMatrix(nRow, nCol) = aPos.Fut.Maturity
                            End Select
                            
                        Case enCtFutOption
                            Select Case nIdx
                                Case RPC_SYMBOL
                                    .TextMatrix(nRow, nCol) = aPos.Symbol
                            
                                Case RPC_OPT_TYPE
                                    .TextMatrix(nRow, nCol) = IIf(aPos.OptType = enOtCall, "C", "P")
                            
                                Case RPC_EXPIRY
                                    .TextMatrix(nRow, nCol) = aPos.Expiry
                                    
                                Case RPC_DTE
                                   '.TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                   '                                 DateSerial(Year(Now), Month(Now), Day(Now))
                                   If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing And gdPos.Col(nIdx).SupportSimulation = True _
                                                                                            And gdPos.Col(nIdx).UseInSimulation = True Then
                                        dSimVal = m_Aux.SimulationParams.m_dDate
                                        If gdPos.Col(nIdx).ShowDeltaInSimulation Then
                                            .TextMatrix(nRow, nCol) = DateSerial(Year(dSimVal), Month(dSimVal), Day(dSimVal)) - _
                                                                    DateSerial(Year(Now), Month(Now), Day(Now))
                                        Else
                                            .TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                                                    DateSerial(Year(dSimVal), Month(dSimVal), Day(dSimVal))
                                        End If
                                    Else
                                        .TextMatrix(nRow, nCol) = DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)) - _
                                                                    DateSerial(Year(Now), Month(Now), Day(Now))
                                    End If
                                    
                                Case RPC_STRIKE
                                    .TextMatrix(nRow, nCol) = IIf(aPos.Strike > BAD_DOUBLE_VALUE, aPos.Strike, STR_NA)
                            
                                Case RPC_UND
                                    .TextMatrix(nRow, nCol) = aUnd.Symbol
                            
                                Case RPC_DEL_UNIT
                                    .TextMatrix(nRow, nCol) = aPos.LotSize
                            
                                Case RPC_FUT_ROOT
                                    .TextMatrix(nRow, nCol) = aPos.Fut.FutRootSymbol
                            
                                Case RPC_FUTURES
                                    .TextMatrix(nRow, nCol) = aPos.Fut.Symbol
                            
                                Case RPC_FUT_MATURITY
                                    .TextMatrix(nRow, nCol) = aPos.Fut.Maturity
                            
                                Case RPC_IMPORT_ID
                                    .TextMatrix(nRow, nCol) = aPos.ImportID
                                    
                            End Select
                            
                        Case Else 'underlying
                            Select Case nIdx
                                Case RPC_SYMBOL
                                    .TextMatrix(nRow, nCol) = aPos.Symbol
                                    
                                Case RPC_OPT_TYPE, RPC_EXPIRY, RPC_STRIKE, RPC_FUT_ROOT, RPC_FUTURES, RPC_FUT_MATURITY, RPC_VOLGA
                                    .TextMatrix(nRow, nCol) = ""
                            
                                Case RPC_UND
                                    .TextMatrix(nRow, nCol) = aUnd.Symbol
                            
                                Case RPC_DEL_UNIT
                                    .TextMatrix(nRow, nCol) = aUnd.LotSize
                            End Select
                    End Select
                End If
                
                Select Case nIdx
                    Case RPC_NETCHANGE
                        .TextMatrix(nRow, nCol) = IIf(aPos.NetChange <> BAD_DOUBLE_VALUE, aPos.NetChange, STR_NA)
                    Case RPC_BID
                        .TextMatrix(nRow, nCol) = IIf(aPos.PriceBid > BAD_DOUBLE_VALUE, aPos.PriceBid, STR_NA)
                        
                        If aPos.ContractType <> enCtOption And aPos.ID <> USD_ID Then
                            If ((enPriceStatus And enRpsBid)) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                        End If
                        
                    Case RPC_ASK
                        .TextMatrix(nRow, nCol) = IIf(aPos.PriceAsk > BAD_DOUBLE_VALUE, aPos.PriceAsk, STR_NA)
                        
                        If aPos.ContractType <> enCtOption And aPos.ID <> USD_ID Then
                            If ((enPriceStatus And enRpsAsk)) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                        End If
                    
                    Case RPC_LAST
                        .TextMatrix(nRow, nCol) = IIf(aPos.PriceLast > BAD_DOUBLE_VALUE, aPos.PriceLast, STR_NA)
                    
                    Case RPC_CLOSE
                        .TextMatrix(nRow, nCol) = IIf(aPos.PriceClose > 0#, aPos.PriceClose, STR_ZERO)
                    
                    Case RPC_PNL_MTM
'                        If m_Aux.Simulation = True And Not aPos.SimulationPos Is Nothing And gdPos.Col(RPC_PNL_MTM).SupportSimulation = True _
'                            And gdPos.Col(RPC_PNL_MTM).UseInSimulation = True Then
'                            If gdPos.Col(RPC_PNL_MTM).ShowDeltaInSimulation Then
'                                .TextMatrix(nRow, nCol) = IIf(aPos.PnlMtm > BAD_DOUBLE_VALUE And _
'                                    aPos.SimulationPos.PnlMtm > BAD_DOUBLE_VALUE, aPos.SimulationPos.PnlMtm - aPos.PnlMtm, STR_NA)
'                            Else
'                                    .TextMatrix(nRow, nCol) = IIf(aPos.SimulationPos.PnlMtm > BAD_DOUBLE_VALUE, aPos.SimulationPos.PnlMtm, STR_NA)
'                            End If
'                        Else
'                            .TextMatrix(nRow, nCol) = IIf(aPos.PnlMtm > BAD_DOUBLE_VALUE, aPos.PnlMtm, STR_NA)
'                        End If
                        
                        If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.PnlMtm
                        PositionUpdateWithSimulation aPos, nRow, nCol, aPos.PnlMtm, dSimVal, RPC_PNL_MTM
                    Case RPC_PNL_THEO
'                        .TextMatrix(nRow, nCol) = IIf(aPos.PnlTheo > BAD_DOUBLE_VALUE, aPos.PnlTheo, STR_NA)
                        If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.PnlTheo
                        PositionUpdateWithSimulation aPos, nRow, nCol, aPos.PnlTheo, dSimVal, RPC_PNL_THEO
                    Case RPC_PNL_EDGE
                        If Not m_Aux.Simulation Then
                            If aPos.PnlTheo > BAD_DOUBLE_VALUE And aPos.PnlMtm > BAD_DOUBLE_VALUE Then
                                .TextMatrix(nRow, nCol) = aPos.PnlTheo - aPos.PnlMtm
                            Else
                                .TextMatrix(nRow, nCol) = STR_NA
                            End If
                        Else
                            If Not aPos.SimulationPos Is Nothing Then
                                If aPos.PnlTheo = BAD_DOUBLE_VALUE Or aPos.PnlMtm = BAD_DOUBLE_VALUE Or _
                                    aPos.SimulationPos.PnlTheo = BAD_DOUBLE_VALUE Or aPos.SimulationPos.PnlMtm = BAD_DOUBLE_VALUE _
                                    Then
                                    .TextMatrix(nRow, nCol) = STR_NA
                                Else
                                    dSimVal = aPos.SimulationPos.PnlTheo - aPos.SimulationPos.PnlMtm
                                    PositionUpdateWithSimulation aPos, nRow, nCol, aPos.PnlTheo - aPos.PnlMtm, dSimVal, RPC_PNL_EDGE
                                End If
                            Else
                                .TextMatrix(nRow, nCol) = STR_NA
                            End If
                        End If
                End Select
        
                Select Case aPos.ContractType
                    Case enCtOption
                        Select Case nIdx
                            Case RPC_UND_POS, RPC_FUT_QTY
                                .TextMatrix(nRow, nCol) = ""
                            
                            Case RPC_OPT_QTY
                                .TextMatrix(nRow, nCol) = IIf(aPos.Qty > BAD_LONG_VALUE, aPos.Qty, STR_NA)
                            
                            Case RPC_OPT_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.DeltaInShares > BAD_DOUBLE_VALUE, aPos.DeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.DeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.DeltaInShares, dSimVal, RPC_OPT_DELTA
                            Case RPC_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.GammaInSharesPerc > BAD_DOUBLE_VALUE, aPos.GammaInSharesPerc, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.GammaInSharesPerc
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.GammaInSharesPerc, dSimVal, RPC_GAMMA
                            
                            Case RPC_NET_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.NetGamma > BAD_DOUBLE_VALUE, aPos.NetGamma, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.NetGamma
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.NetGamma, dSimVal, RPC_NET_GAMMA
                    
                            Case RPC_GAMMA_SHARES
'                                .TextMatrix(nRow, nCol) = IIf(aPos.GammaInShares > BAD_DOUBLE_VALUE, aPos.GammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.GammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.GammaInShares, dSimVal, RPC_GAMMA_SHARES
                    
                            Case RPC_VEGA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaInShares > BAD_DOUBLE_VALUE, aPos.VegaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.VegaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.VegaInShares, dSimVal, RPC_VEGA
                    
                            Case RPC_WTD_VEGA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.WtdVega > BAD_DOUBLE_VALUE, aPos.WtdVega, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.WtdVega
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.WtdVega, dSimVal, RPC_WTD_VEGA
                    
                            Case RPC_THETA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaInShares > BAD_DOUBLE_VALUE, aPos.ThetaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaInShares, dSimVal, RPC_THETA
                    
                            Case RPC_RHO
'                                .TextMatrix(nRow, nCol) = IIf(aPos.RhoInShares > BAD_DOUBLE_VALUE, aPos.RhoInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.RhoInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.RhoInShares, dSimVal, RPC_RHO
                    
                            Case RPC_THETA_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaDeltaInShares > BAD_DOUBLE_VALUE, aPos.ThetaDeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaDeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaDeltaInShares, dSimVal, RPC_THETA_DELTA
                    
                            Case RPC_THETA_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaGammaInShares > BAD_DOUBLE_VALUE, aPos.ThetaGammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaGammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaGammaInShares, dSimVal, RPC_THETA_GAMMA
                    
                            Case RPC_VEGA_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaDeltaInShares > BAD_DOUBLE_VALUE, aPos.VegaDeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.VegaDeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.VegaDeltaInShares, dSimVal, RPC_VEGA_DELTA
                    
                            Case RPC_VEGA_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaGammaInShares > BAD_DOUBLE_VALUE, aPos.VegaGammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.VegaGammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.VegaGammaInShares, dSimVal, RPC_VEGA_GAMMA
                    
                            Case RPC_TIME_VALUE
'                                .TextMatrix(nRow, nCol) = IIf(aPos.TimeValue > BAD_DOUBLE_VALUE, aPos.TimeValue, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.TimeValue
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.TimeValue, dSimVal, RPC_TIME_VALUE
                                
                            Case RPC_DELTAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Delta > BAD_DOUBLE_VALUE, aPos.Delta * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Delta
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Delta, dSimVal, RPC_DELTAP, 100
        
                            Case RPC_GAMMAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Gamma > BAD_DOUBLE_VALUE, aPos.Gamma * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Gamma
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Gamma, dSimVal, RPC_GAMMAP, 100
    
                            Case RPC_VEGAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Vega > BAD_DOUBLE_VALUE, aPos.Vega * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Vega
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Vega, dSimVal, RPC_VEGAP, 100
    
                            Case RPC_THETAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Theta > BAD_DOUBLE_VALUE, aPos.Theta * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Theta
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Theta, dSimVal, RPC_THETAP, 100
    
                            Case RPC_RHOP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Rho > BAD_DOUBLE_VALUE, aPos.Rho * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Rho
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Rho, dSimVal, RPC_RHOP, 100
                                
                            Case RPC_THEO_VOL
                                '.TextMatrix(nRow, nCol) = IIf(aPos.Vola > BAD_DOUBLE_VALUE, aPos.Vola * 100#, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Vola
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Vola, dSimVal, RPC_THEO_VOL, 100
                                
                            Case RPC_MIV
                                '.TextMatrix(nRow, nCol) = +IIf(aPos.MIV > BAD_DOUBLE_VALUE, aPos.MIV * 100#, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.MIV
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.MIV, dSimVal, RPC_MIV, 100
                                
                            Case RPC_VOLGA
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Volga
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Volga, dSimVal, RPC_VOLGA, 100

                        End Select
                        
                    Case enCtFuture
                        Select Case nIdx
                            Case RPC_OPT_QTY, RPC_OPT_DELTA, RPC_GAMMA, RPC_NET_GAMMA, RPC_GAMMA_SHARES, _
                                RPC_VEGA, RPC_WTD_VEGA, RPC_THETA, RPC_RHO, RPC_THETA_DELTA, RPC_THETA_GAMMA, _
                                RPC_VEGA_DELTA, RPC_VEGA_GAMMA, RPC_TIME_VALUE, RPC_DELTAP, RPC_GAMMAP, RPC_VEGAP, RPC_THETAP, RPC_RHOP
                                .TextMatrix(nRow, nCol) = ""
                            
                            Case RPC_UND_POS
                                .TextMatrix(nRow, nCol) = IIf(aPos.QtyInShares > BAD_LONG_VALUE, aPos.QtyInShares, STR_NA)
                                
                            Case RPC_FUT_QTY
                                .TextMatrix(nRow, nCol) = IIf(aPos.Qty > BAD_LONG_VALUE, aPos.Qty, STR_NA)
                            
                        End Select
                    
                    Case enCtFutOption
                        Select Case nIdx
                            Case RPC_UND_POS, RPC_FUT_QTY
                                .TextMatrix(nRow, nCol) = ""
                            
                            Case RPC_OPT_QTY
                                .TextMatrix(nRow, nCol) = IIf(aPos.Qty > BAD_LONG_VALUE, aPos.Qty, STR_NA)
                            
                            Case RPC_OPT_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.DeltaInShares > BAD_DOUBLE_VALUE, aPos.DeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.DeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.DeltaInShares, dSimVal, RPC_OPT_DELTA

                            Case RPC_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.GammaInSharesPerc > BAD_DOUBLE_VALUE, aPos.GammaInSharesPerc, STR_NA)
                    
                            Case RPC_NET_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.NetGamma > BAD_DOUBLE_VALUE, aPos.NetGamma, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.NetGamma
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.NetGamma, dSimVal, RPC_NET_GAMMA
                                
                            Case RPC_GAMMA_SHARES
'                                .TextMatrix(nRow, nCol) = IIf(aPos.GammaInShares > BAD_DOUBLE_VALUE, aPos.GammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.DeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.DeltaInShares, dSimVal, RPC_GAMMA_SHARES
                                
                            Case RPC_VEGA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaInShares > BAD_DOUBLE_VALUE, aPos.VegaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.GammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.GammaInShares, dSimVal, RPC_VEGA
                                
                            Case RPC_WTD_VEGA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.WtdVega > BAD_DOUBLE_VALUE, aPos.WtdVega, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.WtdVega
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.WtdVega, dSimVal, RPC_WTD_VEGA
                                
                            Case RPC_THETA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaInShares > BAD_DOUBLE_VALUE, aPos.ThetaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaInShares, dSimVal, RPC_THETA
                                
                            Case RPC_RHO
'                                .TextMatrix(nRow, nCol) = IIf(aPos.RhoInShares > BAD_DOUBLE_VALUE, aPos.RhoInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.RhoInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.RhoInShares, dSimVal, RPC_RHO
                                
                            Case RPC_THETA_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaDeltaInShares > BAD_DOUBLE_VALUE, aPos.ThetaDeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaDeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaDeltaInShares, dSimVal, RPC_THETA_DELTA
                                
                            Case RPC_THETA_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.ThetaGammaInShares > BAD_DOUBLE_VALUE, aPos.ThetaGammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.ThetaGammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.ThetaGammaInShares, dSimVal, RPC_THETA_GAMMA
                                
                            Case RPC_VEGA_DELTA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaDeltaInShares > BAD_DOUBLE_VALUE, aPos.VegaDeltaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.VegaDeltaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.VegaDeltaInShares, dSimVal, RPC_VEGA_DELTA
                                
                            Case RPC_VEGA_GAMMA
'                                .TextMatrix(nRow, nCol) = IIf(aPos.VegaGammaInShares > BAD_DOUBLE_VALUE, aPos.VegaGammaInShares, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.VegaGammaInShares
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.VegaGammaInShares, dSimVal, RPC_VEGA_GAMMA
                                
                            Case RPC_TIME_VALUE
'                                .TextMatrix(nRow, nCol) = IIf(aPos.TimeValue > BAD_DOUBLE_VALUE, aPos.TimeValue, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.TimeValue
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.TimeValue, dSimVal, RPC_TIME_VALUE
                                
                            Case RPC_DELTAP
'                                    .TextMatrix(nRow, nCol) = IIf(aPos.Delta > BAD_DOUBLE_VALUE, aPos.Delta * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Delta
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Delta, dSimVal, RPC_DELTAP, 100
                                
                            Case RPC_GAMMAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Gamma > BAD_DOUBLE_VALUE, aPos.Gamma * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Gamma
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Gamma, dSimVal, RPC_GAMMAP, 100
                                
                            Case RPC_VEGAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Vega > BAD_DOUBLE_VALUE, aPos.Vega * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Vega
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Vega, dSimVal, RPC_VEGAP, 100
                                
                            Case RPC_THETAP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Theta > BAD_DOUBLE_VALUE, aPos.Theta * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Theta
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Theta, dSimVal, RPC_THETAP, 100
                                
                            Case RPC_RHOP
'                                .TextMatrix(nRow, nCol) = IIf(aPos.Rho > BAD_DOUBLE_VALUE, aPos.Rho * 100, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Rho
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Rho, dSimVal, RPC_RHOP, 100
                                
                            Case RPC_THEO_VOL
                                '.TextMatrix(nRow, nCol) = IIf(aPos.Vola > BAD_DOUBLE_VALUE, aPos.Vola * 100#, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Vola
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Vola, dSimVal, RPC_THEO_VOL, 100
                                
                            Case RPC_MIV
                                '.TextMatrix(nRow, nCol) = IIf(aPos.MIV > BAD_DOUBLE_VALUE, aPos.MIV * 100#, STR_NA)
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.MIV
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.MIV, dSimVal, RPC_MIV, 100
                                
                            Case RPC_VOLGA
                                If Not aPos.SimulationPos Is Nothing Then dSimVal = aPos.SimulationPos.Volga
                                PositionUpdateWithSimulation aPos, nRow, nCol, aPos.Volga, dSimVal, RPC_VOLGA, 100

                        End Select
                    
                    Case Else 'underlying
                        Select Case nIdx
                            Case RPC_UND_POS
                                .TextMatrix(nRow, nCol) = IIf(aPos.QtyInShares > BAD_LONG_VALUE, aPos.QtyInShares, STR_NA)
        
                            Case RPC_OPT_QTY, RPC_OPT_DELTA, RPC_GAMMA, RPC_VEGA, RPC_WTD_VEGA, _
                                RPC_THETA, RPC_RHO, RPC_THETA_DELTA, RPC_THETA_GAMMA, RPC_VEGA_DELTA, _
                                RPC_VEGA_GAMMA, RPC_TIME_VALUE, RPC_FUT_QTY, RPC_DELTAP, RPC_GAMMAP, RPC_VEGAP, RPC_THETAP, RPC_RHOP
                                .TextMatrix(nRow, nCol) = ""
                        End Select
                    
                End Select
                
                i = i + 1
                nIdx = gdPos.Idx(i)
            Wend
    
            m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        End If
Ex:
        Set aUnd = Nothing
        Set aPos = Nothing
    End With
End Sub
Private Sub UnderlyingUpdateWithSimulation(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, realBadValue As Boolean, simulatedBadValue As Boolean, _
                                       realValue As Double, simulatedValue As Double, lColID As Long, nCol As Long, nRow As Long)
    With fgPos
        If m_Aux.Simulation = True And Not aUnd.SimulationUnd Is Nothing And _
                                                                gdPos.Col(lColID).UseInSimulation = True Then
            If realValue = BAD_DOUBLE_VALUE Or simulatedValue = BAD_DOUBLE_VALUE Then
                .TextMatrix(nRow, nCol) = STR_NA
            Else
                If gdPos.Col(lColID).ShowDeltaInSimulation = True Then
                    .TextMatrix(nRow, nCol) = simulatedValue - realValue
                Else
                    .TextMatrix(nRow, nCol) = simulatedValue
                End If
            End If
            .Cell(flexcpForeColor, nRow, nCol) = IIf(realBadValue Or simulatedBadValue, _
                                                gdPos.Col(lColID).ForeColorAlt1, gdPos.Col(lColID).ForeColor)
        Else
            .TextMatrix(nRow, nCol) = IIf(realValue > BAD_DOUBLE_VALUE, realValue, STR_NA)
            .Cell(flexcpForeColor, nRow, nCol) = IIf(realBadValue, gdPos.Col(lColID).ForeColorAlt1, gdPos.Col(lColID).ForeColor)
        End If
    End With
End Sub

Public Sub UnderlyingUpdate(ByVal nRow&, ByVal bSymbol As Boolean, Optional ByVal bRealTimeUpdate As Boolean = False)
    On Error Resume Next
    Dim nCol&, aUnd As EtsMmRisksLib.MmRvUndAtom, aRowData As clsRvRowData
    Dim bIsPriceUsed As Boolean: bIsPriceUsed = True

    With fgPos
        Set aRowData = .RowData(nRow)
        Set aUnd = aRowData.Und
        If Not aUnd Is Nothing Then
            
            If bRealTimeUpdate Then
                If Not aUnd.CalcGreeks And Not aUnd.CalcTotals And Not m_Aux.Idx.CalcTotals Then GoTo Ex
            End If
            
            If Not aUnd.UndPriceProfile Is Nothing Then _
                bIsPriceUsed = Not aUnd.UndPriceProfile.AlwaysUseLast
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
            
            Dim i&, nIdx&
            Dim dSimVal#
            Dim bSimBadVal As Boolean
            
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= g_RPC_LAST_COLUMN
                nCol = i + 1
                dSimVal = BAD_DOUBLE_VALUE
                bSimBadVal = True
                
                If bSymbol Then
                    Select Case nIdx
                        Case RPC_SYMBOL
                            .TextMatrix(nRow, nCol) = aUnd.Symbol
                        
                        Case RPC_OPT_TYPE, RPC_EXPIRY, RPC_STRIKE, RPC_UND, _
                            RPC_DEL_UNIT, RPC_FUT_ROOT, RPC_FUTURES, RPC_FUT_MATURITY, RPC_VOLGA
                            .TextMatrix(nRow, nCol) = ""
                    End Select
                End If
                
                '--------------------------------------------------------------------------
                Dim cvID As Long
                Dim dCVRealVal As Variant
                
                cvID = BAD_LONG_VALUE
                cvID = m_Aux.m_cvOptIDs(CStr(nIdx))
                If cvID <> BAD_LONG_VALUE Then
                    .TextMatrix(nRow, nCol) = ""
                    dCVRealVal = m_Aux.m_aContext.GetCVValue(aUnd.cvPoolID, cvID)
                    With fgPos
                        If m_Aux.Simulation = True And Not aUnd.SimulationUnd Is Nothing And gdPos.Col(nIdx).SupportSimulation = True _
                                                                                            And gdPos.Col(nIdx).UseInSimulation = True Then
                            dSimVal = m_Aux.m_aContext.GetCVValue(aUnd.SimulationUnd.cvPoolID, cvID)
                            If gdPos.Col(nIdx).ShowDeltaInSimulation And Not IsEmpty(dCVRealVal) Then
                                .TextMatrix(nRow, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE And dSimVal > BAD_DOUBLE_VALUE, dSimVal - dCVRealVal, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = IIf(dSimVal > BAD_DOUBLE_VALUE, dSimVal, STR_NA)
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE, dCVRealVal, STR_NA)
                        End If
                    End With
                    '.TextMatrix(nRow, nCol) = m_Aux.m_aContext.GetCVValue(aUnd.cvPoolID, cvID)
                End If
                
                '--------------------------------------------------------------------------
                
                Select Case nIdx
                    Case RPC_NETCHANGE
                        .TextMatrix(nRow, nCol) = IIf(aUnd.NetChange <> BAD_DOUBLE_VALUE, aUnd.NetChange, STR_NA)
                    Case RPC_BID
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceBid > BAD_DOUBLE_VALUE, aUnd.PriceBid, STR_NA)
                    
                        If ((aUnd.ReplacePriceStatus And enRpsBid)) Or _
                           (Not bIsPriceUsed) Then
                            Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                            .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                        Else
                            Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                        End If
                    
                    Case RPC_ASK
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceAsk > BAD_DOUBLE_VALUE, aUnd.PriceAsk, STR_NA)
                    
                        If ((aUnd.ReplacePriceStatus And enRpsAsk)) Or _
                           (Not bIsPriceUsed) Then
                            Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                            .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                        Else
                            Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                        End If
                    
                    Case RPC_LAST
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceLast > BAD_DOUBLE_VALUE, aUnd.PriceLast, STR_NA)
                    
                    Case RPC_CLOSE
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceClose > 0#, aUnd.PriceClose, STR_NA)
                    
                    Case RPC_UND_POS
                        .TextMatrix(nRow, nCol) = IIf(aUnd.Qty > BAD_LONG_VALUE, aUnd.Qty, STR_NA)
                    
                    Case RPC_OPT_QTY
                        .TextMatrix(nRow, nCol) = IIf(aUnd.OptQty > BAD_LONG_VALUE, aUnd.OptQty, STR_NA)
                    
                    Case RPC_PNL_MTM
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.PnlMtm > BAD_DOUBLE_VALUE, aUnd.PnlMtm, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlMtm, gdPos.Col(RPC_PNL_MTM).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_PNL_MTM).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.PnlMtm
                            bSimBadVal = aUnd.SimulationUnd.BadPnlMtm
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadPnlMtm, bSimBadVal, aUnd.PnlMtm, dSimVal, RPC_PNL_MTM, nCol, nRow
                    
                    Case RPC_PNL_THEO
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.PnlTheo > BAD_DOUBLE_VALUE, aUnd.PnlTheo, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlTheo, gdPos.Col(RPC_PNL_THEO).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_PNL_THEO).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.PnlTheo
                            bSimBadVal = aUnd.SimulationUnd.BadPnlTheo
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadPnlTheo, bSimBadVal, aUnd.PnlTheo, dSimVal, RPC_PNL_THEO, nCol, nRow
                                
                    Case RPC_PNL_EDGE
'                        If aUnd.PnlTheo > BAD_DOUBLE_VALUE And aUnd.PnlMtm > BAD_DOUBLE_VALUE Then
'                            .TextMatrix(nRow, nCol) = aUnd.PnlTheo - aUnd.PnlMtm
'                        Else
'                            .TextMatrix(nRow, nCol) = STR_NA
'                        End If
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadPnlMtm Or aUnd.BadPnlTheo, gdPos.Col(RPC_PNL_EDGE).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_PNL_EDGE).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.PnlTheo - IIf(aUnd.SimulationUnd.PnlMtm <= BAD_DOUBLE_VALUE, 0, aUnd.SimulationUnd.PnlMtm)
                            bSimBadVal = aUnd.SimulationUnd.BadPnlMtm Or aUnd.SimulationUnd.BadPnlTheo
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadPnlMtm Or aUnd.BadPnlTheo, bSimBadVal, aUnd.PnlTheo - aUnd.PnlMtm, dSimVal, RPC_PNL_EDGE, nCol, nRow
            
                    Case RPC_NET_DELTA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.NetDelta > BAD_DOUBLE_VALUE, aUnd.NetDelta, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_NET_DELTA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.NetDelta
                            bSimBadVal = aUnd.SimulationUnd.BadNetDelta
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadNetDelta, bSimBadVal, aUnd.NetDelta, dSimVal, RPC_NET_DELTA, nCol, nRow
                        
                    Case RPC_NET_DELTA_USD
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.DeltaEq > BAD_DOUBLE_VALUE, aUnd.DeltaEq, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadDeltaEq, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.DeltaEq
                            bSimBadVal = aUnd.SimulationUnd.BadDeltaEq
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadDeltaEq, bSimBadVal, aUnd.DeltaEq, dSimVal, RPC_NET_DELTA_USD, nCol, nRow
                        
                    Case RPC_OPT_DELTA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.OptDelta > BAD_DOUBLE_VALUE, aUnd.OptDelta, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadOptDelta, gdPos.Col(RPC_OPT_DELTA).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_OPT_DELTA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.OptDelta
                            bSimBadVal = aUnd.SimulationUnd.BadOptDelta
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadOptDelta, bSimBadVal, aUnd.OptDelta, dSimVal, RPC_OPT_DELTA, nCol, nRow
                        
                    Case RPC_GAMMA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.GammaPerc > BAD_DOUBLE_VALUE, aUnd.GammaPerc, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGammaPerc, gdPos.Col(RPC_GAMMA).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_GAMMA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.GammaPerc
                            bSimBadVal = aUnd.SimulationUnd.BadGammaPerc
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadGammaPerc, bSimBadVal, aUnd.GammaPerc, dSimVal, RPC_GAMMA, nCol, nRow
                        
                    Case RPC_NET_GAMMA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.NetGamma > BAD_DOUBLE_VALUE, aUnd.NetGamma, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetGamma, gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_NET_GAMMA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.NetGamma
                            bSimBadVal = aUnd.SimulationUnd.BadNetGamma
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadNetGamma, bSimBadVal, aUnd.NetGamma, dSimVal, RPC_NET_GAMMA, nCol, nRow
                        
                    Case RPC_GAMMA_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.Gamma > BAD_DOUBLE_VALUE, aUnd.Gamma, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGamma, gdPos.Col(RPC_GAMMA_SHARES).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_GAMMA_SHARES).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.Gamma
                            bSimBadVal = aUnd.SimulationUnd.BadGamma
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadGamma, bSimBadVal, aUnd.Gamma, dSimVal, RPC_GAMMA_SHARES, nCol, nRow
                        
                    Case RPC_VEGA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.Vega > BAD_DOUBLE_VALUE, aUnd.Vega, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVega, gdPos.Col(RPC_VEGA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_VEGA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.Vega
                            bSimBadVal = aUnd.SimulationUnd.BadVega
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadVega, bSimBadVal, aUnd.Vega, dSimVal, RPC_VEGA, nCol, nRow
                        
                    Case RPC_WTD_VEGA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.WtdVega > BAD_DOUBLE_VALUE, aUnd.WtdVega, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadWtdVega, gdPos.Col(RPC_WTD_VEGA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_WTD_VEGA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.WtdVega
                            bSimBadVal = aUnd.SimulationUnd.BadWtdVega
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadWtdVega, bSimBadVal, aUnd.WtdVega, dSimVal, RPC_WTD_VEGA, nCol, nRow
                        
                    Case RPC_THETA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.Theta > BAD_DOUBLE_VALUE, aUnd.Theta, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTheta, gdPos.Col(RPC_THETA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_THETA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.Theta
                            bSimBadVal = aUnd.SimulationUnd.BadTheta
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadTheta, bSimBadVal, aUnd.Theta, dSimVal, RPC_THETA, nCol, nRow
                        
                    Case RPC_RHO
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.Rho > BAD_DOUBLE_VALUE, aUnd.Rho, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadRho, gdPos.Col(RPC_RHO).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_RHO).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.Rho
                            bSimBadVal = aUnd.SimulationUnd.BadRho
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadRho, bSimBadVal, aUnd.Rho, dSimVal, RPC_RHO, nCol, nRow
                        
                    Case RPC_THETA_DELTA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.ThetaDelta > BAD_DOUBLE_VALUE, aUnd.ThetaDelta, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadThetaDelta, gdPos.Col(RPC_THETA_DELTA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_THETA_DELTA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.ThetaDelta
                            bSimBadVal = aUnd.SimulationUnd.BadThetaDelta
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadThetaDelta, bSimBadVal, aUnd.ThetaDelta, dSimVal, RPC_THETA_DELTA, nCol, nRow
            
                    Case RPC_THETA_GAMMA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.ThetaGamma > BAD_DOUBLE_VALUE, aUnd.ThetaGamma, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadThetaGamma, gdPos.Col(RPC_THETA_GAMMA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_THETA_GAMMA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.ThetaGamma
                            bSimBadVal = aUnd.SimulationUnd.BadThetaGamma
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadThetaGamma, bSimBadVal, aUnd.ThetaGamma, dSimVal, RPC_THETA_GAMMA, nCol, nRow

                    Case RPC_VEGA_DELTA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.VegaDelta > BAD_DOUBLE_VALUE, aUnd.VegaDelta, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVegaDelta, gdPos.Col(RPC_VEGA_DELTA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_VEGA_DELTA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.VegaDelta
                            bSimBadVal = aUnd.SimulationUnd.BadVegaDelta
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadVegaDelta, bSimBadVal, aUnd.VegaDelta, dSimVal, RPC_VEGA_DELTA, nCol, nRow

                    Case RPC_VEGA_GAMMA
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.VegaGamma > BAD_DOUBLE_VALUE, aUnd.VegaGamma, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVegaGamma, gdPos.Col(RPC_VEGA_GAMMA).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_VEGA_GAMMA).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.VegaGamma
                            bSimBadVal = aUnd.SimulationUnd.BadVegaGamma
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadVegaGamma, bSimBadVal, aUnd.VegaGamma, dSimVal, RPC_VEGA_GAMMA, nCol, nRow
                        
                    Case RPC_TIME_VALUE
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.TimeValue > BAD_DOUBLE_VALUE, aUnd.TimeValue, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTimeValue, gdPos.Col(RPC_TIME_VALUE).ForeColorAlt1, _
'                                                                            gdPos.Col(RPC_TIME_VALUE).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.TimeValue
                            bSimBadVal = aUnd.SimulationUnd.BadTimeValue
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadTimeValue, bSimBadVal, aUnd.TimeValue, dSimVal, RPC_TIME_VALUE, nCol, nRow
                        
                    Case RPC_BETA_WTD_DELTA_USD
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.BetaWtdDeltaEq > BAD_DOUBLE_VALUE, aUnd.BetaWtdDeltaEq, STR_NA)
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadBetaWtdDeltaEq, gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
'                                                                                gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                        If Not aUnd.SimulationUnd Is Nothing Then
                            dSimVal = aUnd.SimulationUnd.BetaWtdDeltaEq
                            bSimBadVal = aUnd.SimulationUnd.BadBetaWtdDeltaEq
                        End If
                        UnderlyingUpdateWithSimulation aUnd, aUnd.BadBetaWtdDeltaEq, bSimBadVal, aUnd.BetaWtdDeltaEq, dSimVal, RPC_BETA_WTD_DELTA_USD, nCol, nRow
                
                    Case RPC_FUT_QTY
                        .TextMatrix(nRow, nCol) = IIf(aUnd.FutQty > BAD_LONG_VALUE, aUnd.FutQty, STR_NA)
                End Select
    
                i = i + 1
                nIdx = gdPos.Idx(i)
            Wend
            
            m_Aux.GridLock(GT_RISKS_POSITIONS).UnlockRedraw
        End If
        
Ex:
        Set aUnd = Nothing
        Set aRowData = Nothing
    End With
End Sub

Public Sub UnderlyingUpdateBadStatus(ByVal nRow&)
    On Error Resume Next
    Dim nCol&, aUnd As EtsMmRisksLib.MmRvUndAtom, aRowData As clsRvRowData

    With fgPos
        m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        Set aRowData = .RowData(nRow)
        If Not aRowData.Und Is Nothing Then
            Set aUnd = aRowData.Und
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= g_RPC_LAST_COLUMN
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

Private Sub SyntheticGreeksUpdateWithSimulation(ByRef aSimSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom, ByVal nRow As Long, ByVal nCol As Long, _
                                                ByVal realValue As Double, ByVal simulatedValue As Double, ByVal nColID As Long, Optional ByVal dMultiplier As Double = 1)
    With fgPos
        If m_Aux.Simulation = True And Not aSimSynthGreek Is Nothing And gdPos.Col(nColID&).SupportSimulation = True _
                            And gdPos.Col(nColID&).UseInSimulation = True Then
            If gdPos.Col(nColID&).ShowDeltaInSimulation Then
                .TextMatrix(nRow, nCol) = IIf(realValue > BAD_DOUBLE_VALUE And simulatedValue > BAD_DOUBLE_VALUE, simulatedValue * dMultiplier - realValue * dMultiplier, STR_NA)
            Else
                .TextMatrix(nRow, nCol) = IIf(simulatedValue > BAD_DOUBLE_VALUE, simulatedValue * dMultiplier, STR_NA)
            End If
        Else
            .TextMatrix(nRow, nCol) = IIf(realValue > BAD_DOUBLE_VALUE, realValue * dMultiplier, STR_NA)
        End If
    End With
End Sub

Public Sub SyntheticGreeksUpdate(ByVal nRow&, ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim nCol&, bUpdateOptSymbol As Boolean: bUpdateOptSymbol = False
    Dim bUpdateSynth As Boolean: bUpdateSynth = False
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aRowData As clsRvRowData, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aSimSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim dSimValue As Double
    Dim bIsPriceUsed As Boolean: bIsPriceUsed = True

    With fgPos
        Set aRowData = .RowData(nRow)
        
        If Not aRowData Is Nothing And Not aRowData.SynthGreeks Is Nothing And Not aRowData.Und Is Nothing Then
                
            Set aSynthGreek = aRowData.SynthGreeks
            Set aSimSynthGreek = aRowData.SimSynthGreeks
            Set aUnd = aRowData.Und
            Set aPos = aRowData.Pos
            m_Aux.GridLock(GT_RISKS_POSITIONS).LockRedraw
            
            If bSymbol Then bUpdateOptSymbol = (aPos.ContractType = enCtOption)
            
            Set aSynthUnd = m_Aux.Und(aSynthGreek.SynthUndID)
            bUpdateSynth = aSynthGreek.IsTotal And Not aSynthUnd Is Nothing
            
            If Not aSynthUnd.UndPriceProfile Is Nothing Then _
                bIsPriceUsed = Not aSynthUnd.UndPriceProfile.AlwaysUseLast
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdPos.Idx(0)
            While nIdx >= 0 And i <= g_RPC_LAST_COLUMN
                nCol = i + 1
                dSimValue = BAD_DOUBLE_VALUE
            
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
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.NetChange <> BAD_DOUBLE_VALUE, aSynthUnd.NetChange, STR_NA)
                        Case RPC_BID
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.PriceBid > BAD_DOUBLE_VALUE, aSynthUnd.PriceBid, STR_NA)
                            
                            If ((aSynthUnd.ReplacePriceStatus And enRpsBid) And aSynthUnd.PriceBid = BAD_DOUBLE_VALUE) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                    
                        Case RPC_ASK
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.PriceAsk > BAD_DOUBLE_VALUE, aSynthUnd.PriceAsk, STR_NA)
                            
                            If ((aSynthUnd.ReplacePriceStatus And enRpsAsk) And aSynthUnd.PriceAsk = BAD_DOUBLE_VALUE) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                    
                        Case RPC_LAST
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.PriceLast > BAD_DOUBLE_VALUE, aSynthUnd.PriceLast, STR_NA)
                    
                        Case RPC_CLOSE
                            .TextMatrix(nRow, nCol) = IIf(aSynthUnd.PriceClose > 0#, aSynthUnd.PriceClose, STR_NA)
                    End Select
                Else
                    Select Case nIdx
                        Case RPC_NETCHANGE, RPC_BID, RPC_ASK, RPC_LAST, RPC_CLOSE
                            .TextMatrix(nRow, nCol) = ""
                    End Select
                End If
                
                Select Case nIdx
                    Case RPC_PNL_MTM, RPC_PNL_THEO, RPC_PNL_EDGE, RPC_UND_POS, RPC_OPT_QTY, _
                        RPC_VEGA, RPC_WTD_VEGA, RPC_THETA, RPC_RHO, RPC_THETA_DELTA, RPC_THETA_GAMMA, _
                        RPC_VEGA_DELTA, RPC_VEGA_GAMMA, RPC_TIME_VALUE, RPC_FUT_QTY
                        .TextMatrix(nRow, nCol) = ""
                                                
                    Case RPC_NET_DELTA
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.DeltaInShares
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                                SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.DeltaInShares, dSimValue, RPC_NET_DELTA
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                    gdPos.Col(RPC_NET_DELTA).ForeColor)
                            End If
                        Else
                            '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                            SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.DeltaInShares, dSimValue, RPC_NET_DELTA
                            If aSynthGreek.IsTotal Then _
                                .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_NET_DELTA).ForeColorAlt1, _
                                 gdPos.Col(RPC_NET_DELTA).ForeColor)
                        End If
                            
                    Case RPC_NET_DELTA_USD
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.DeltaInMoney
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInMoney, STR_NA)
                                SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.DeltaInMoney, dSimValue, RPC_NET_DELTA_USD
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInMoney, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                    gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                            End If
                        Else
                            '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInMoney, STR_NA)
                            SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.DeltaInMoney, dSimValue, RPC_NET_DELTA_USD
                            If aSynthGreek.IsTotal Then _
                                .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInMoney, gdPos.Col(RPC_NET_DELTA_USD).ForeColorAlt1, _
                                 gdPos.Col(RPC_NET_DELTA_USD).ForeColor)
                        End If
                            
                    Case RPC_OPT_DELTA
                        '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.DeltaInShares
                        SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.DeltaInShares, dSimValue, RPC_OPT_DELTA
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDelta, gdPos.Col(RPC_OPT_DELTA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_OPT_DELTA).ForeColor)
                
                    Case RPC_GAMMA
                        '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaInSharesPerc > BAD_DOUBLE_VALUE, aSynthGreek.GammaInSharesPerc, STR_NA)
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.GammaInSharesPerc
                        SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.GammaInSharesPerc, dSimValue, RPC_GAMMA
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGammaPerc, gdPos.Col(RPC_GAMMA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_GAMMA).ForeColor)
    
                    Case RPC_NET_GAMMA
                        '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.NetGamma > BAD_DOUBLE_VALUE, aSynthGreek.NetGamma, STR_NA)
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.NetGamma
                        SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.NetGamma, dSimValue, RPC_NET_GAMMA
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadNetGamma, gdPos.Col(RPC_NET_GAMMA).ForeColorAlt1, _
                                                        gdPos.Col(RPC_NET_GAMMA).ForeColor)
                    
                    Case RPC_GAMMA_SHARES
                        '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaInShares > BAD_DOUBLE_VALUE, aSynthGreek.GammaInShares, STR_NA)
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.GammaInShares
                        SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.GammaInShares, dSimValue, RPC_GAMMA_SHARES
                        If aSynthGreek.IsTotal Then _
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGamma, gdPos.Col(RPC_GAMMA_SHARES).ForeColorAlt1, _
                                                        gdPos.Col(RPC_GAMMA_SHARES).ForeColor)
                
                    Case RPC_BETA_WTD_DELTA_USD
                        If Not aSimSynthGreek Is Nothing Then dSimValue = aSimSynthGreek.BetaWtdDeltaInMoney
                        
                        If Not aPos Is Nothing Then
                            If aPos.ContractType = enCtOption Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.BetaWtdDeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.BetaWtdDeltaInMoney, STR_NA)
                                SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.BetaWtdDeltaInMoney, dSimValue, RPC_BETA_WTD_DELTA_USD
                                If aSynthGreek.IsTotal Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadBetaWtdDeltaInMoney, gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
                                    gdPos.Col(RPC_BETA_WTD_DELTA_USD).ForeColor)
                            End If
                        Else
                            '.TextMatrix(nRow, nCol) = IIf(aSynthGreek.BetaWtdDeltaInMoney > BAD_DOUBLE_VALUE, aSynthGreek.BetaWtdDeltaInMoney, STR_NA)
                            SyntheticGreeksUpdateWithSimulation aSimSynthGreek, nRow, nCol, aSynthGreek.BetaWtdDeltaInMoney, dSimValue, RPC_BETA_WTD_DELTA_USD
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
Private Sub UpdateTotalsWithSimulation(realBadValue As Boolean, simulatedBadValue As Boolean, _
                                       realValue As Double, simulatedValue As Double, lColID As Long, nCol As Long)
    With fgTot
        If m_Aux.Simulation = True And Not m_Aux.Grp.SimulationGrp Is Nothing And _
                                                                gdTot.Col(lColID).UseInSimulation = True Then
            If realValue = BAD_DOUBLE_VALUE Or simulatedValue = BAD_DOUBLE_VALUE Then
                .TextMatrix(1, nCol) = STR_NA
            Else
                If gdTot.Col(lColID).ShowDeltaInSimulation = True Then
                    .TextMatrix(1, nCol) = simulatedValue - realValue
                Else
                    .TextMatrix(1, nCol) = simulatedValue
                End If
            End If
            .Cell(flexcpForeColor, 1, nCol) = IIf(realBadValue Or simulatedBadValue, _
                                                gdTot.Col(lColID).ForeColorAlt1, gdTot.Col(lColID).ForeColor)
        Else
            .TextMatrix(1, nCol) = IIf(realValue > BAD_DOUBLE_VALUE, realValue, STR_NA)
            .Cell(flexcpForeColor, 1, nCol) = IIf(realBadValue, gdTot.Col(lColID).ForeColorAlt1, gdTot.Col(lColID).ForeColor)
        End If
    End With
End Sub

Public Sub TotalsUpdate()
    On Error Resume Next
    Dim nCol&

    With fgTot
        m_Aux.GridLock(GT_RISKS_TOTALS).LockRedraw
        
        Dim i&, nIdx&
        Dim dSimVal#
        Dim bSimBadVal As Boolean
        
        i = 0
        nIdx = gdTot.Idx(0)
        While nIdx >= 0 And i <= g_RTC_LAST_COLUMN
            nCol = i
            dSimVal = BAD_DOUBLE_VALUE
            bSimBadVal = True
            
            '--------------------------------------------------------------------------
            Dim cvID As Long
            Dim dCVRealVal As Variant
            
            cvID = BAD_LONG_VALUE
            cvID = m_Aux.m_cvGrpIDs(CStr(nIdx))
            If cvID <> BAD_LONG_VALUE Then
                .TextMatrix(1, nCol) = ""
                dCVRealVal = m_Aux.m_aContext.GetCVValue(m_Aux.Grp.cvPoolID, cvID)
                With fgTot
                    If m_Aux.Simulation = True And Not m_Aux.Grp.SimulationGrp Is Nothing And gdTot.Col(nIdx).SupportSimulation = True _
                                                                                            And gdTot.Col(nIdx).UseInSimulation = True Then
                        dSimVal = m_Aux.m_aContext.GetCVValue(m_Aux.Grp.SimulationGrp.cvPoolID, cvID)
                        If gdTot.Col(nIdx).ShowDeltaInSimulation Then
                            .TextMatrix(1, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE And dSimVal > BAD_DOUBLE_VALUE, dSimVal - dCVRealVal, STR_NA)
                        Else
                            .TextMatrix(1, nCol) = IIf(dSimVal > BAD_DOUBLE_VALUE, dSimVal, STR_NA)
                        End If
                    Else
                        .TextMatrix(1, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE, dCVRealVal, STR_NA)
                        '.TextMatrix(1, nCol) = IIf(dCVRealVal > BAD_DOUBLE_VALUE Or Not dCVRealVal Is Empty, dCVRealVal, STR_NA)
                    End If
                End With
            End If
            
            '--------------------------------------------------------------------------
            
            Select Case nIdx
                Case RTC_PNL_MTM
                    '.TextMatrix(1, nCol) = IIf(m_Aux.Grp.PnlMtm > BAD_DOUBLE_VALUE, m_Aux.Grp.PnlMtm, STR_NA)
                    '.Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm, gdTot.Col(RTC_PNL_MTM).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_PNL_MTM).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.PnlMtm
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadPnlMtm
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadPnlMtm, bSimBadVal, m_Aux.Grp.PnlMtm, dSimVal, RTC_PNL_MTM, nCol
                Case RTC_PNL_THEO
                    'If m_Aux.Simulation = True And Not m_Aux.Grp.SimulationGrp Is Nothing And _
                    '                                            gdTot.Col(RTC_PNL_THEO).UseInSimulation = True Then
                    '    If m_Aux.Grp.PnlTheo = BAD_DOUBLE_VALUE Or m_Aux.Grp.SimulationGrp.PnlTheo = BAD_DOUBLE_VALUE Then
                    '        .TextMatrix(1, nCol) = STR_NA
                    '    Else
                    '        If gdTot.Col(RTC_PNL_THEO).ShowDeltaInSimulation = True Then
                    '            .TextMatrix(1, nCol) = m_Aux.Grp.SimulationGrp.PnlTheo - m_Aux.Grp.PnlTheo
                    '        Else
                    '            .TextMatrix(1, nCol) = m_Aux.Grp.SimulationGrp.PnlTheo
                    '        End If
                    '    End If
                    '    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlTheo Or m_Aux.Grp.SimulationGrp.BadPnlTheo, gdTot.Col(RTC_PNL_THEO).ForeColorAlt1, _
                    '                                                        gdTot.Col(RTC_PNL_THEO).ForeColor)
                    'Else
                    '    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.PnlTheo > BAD_DOUBLE_VALUE, m_Aux.Grp.PnlTheo, STR_NA)
                    '    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_THEO).ForeColorAlt1, _
                    '                                                        gdTot.Col(RTC_PNL_THEO).ForeColor)
                    'End If
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.PnlTheo
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadPnlTheo
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadPnlTheo, bSimBadVal, m_Aux.Grp.PnlTheo, dSimVal, RTC_PNL_THEO, nCol
                Case RTC_PNL_EDGE
                    'If m_Aux.Grp.PnlTheo > BAD_DOUBLE_VALUE And m_Aux.Grp.PnlMtm > BAD_DOUBLE_VALUE Then
                    '    .TextMatrix(1, nCol) = m_Aux.Grp.PnlTheo - m_Aux.Grp.PnlMtm
                    'Else
                    '    .TextMatrix(1, nCol) = STR_NA
                    'End If
                    '.Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadPnlMtm Or m_Aux.Grp.BadPnlTheo, gdTot.Col(RTC_PNL_EDGE).ForeColorAlt1, _
                    '                                                        gdTot.Col(RTC_PNL_EDGE).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.PnlTheo - IIf(m_Aux.Grp.SimulationGrp.PnlMtm = BAD_DOUBLE_VALUE, 0, m_Aux.Grp.SimulationGrp.PnlMtm)
                        bSimBadVal = m_Aux.Grp.BadPnlMtm Or m_Aux.Grp.SimulationGrp.BadPnlTheo
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadPnlMtm Or m_Aux.Grp.BadPnlTheo, bSimBadVal, m_Aux.Grp.PnlTheo - m_Aux.Grp.PnlMtm, dSimVal, RTC_PNL_EDGE, nCol
            
                Case RTC_VEGA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Vega > BAD_DOUBLE_VALUE, m_Aux.Grp.Vega, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadVega, gdTot.Col(RTC_VEGA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_VEGA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.Vega
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadVega
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadVega, bSimBadVal, m_Aux.Grp.Vega, dSimVal, RTC_VEGA, nCol
                Case RTC_WTD_VEGA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.WtdVega > BAD_DOUBLE_VALUE, m_Aux.Grp.WtdVega, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadWtdVega, gdTot.Col(RTC_WTD_VEGA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_WTD_VEGA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.WtdVega
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadWtdVega
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadWtdVega, bSimBadVal, m_Aux.Grp.WtdVega, dSimVal, RTC_WTD_VEGA, nCol
                Case RTC_THETA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Theta > BAD_DOUBLE_VALUE, m_Aux.Grp.Theta, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadTheta, gdTot.Col(RTC_THETA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_THETA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.Theta
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadTheta
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadTheta, bSimBadVal, m_Aux.Grp.Theta, dSimVal, RTC_THETA, nCol
                Case RTC_DELTA_EQ
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.DeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.DeltaEq, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadDeltaEq, gdTot.Col(RTC_DELTA_EQ).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_DELTA_EQ).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.DeltaEq
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadDeltaEq
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadDeltaEq, bSimBadVal, m_Aux.Grp.DeltaEq, dSimVal, RTC_DELTA_EQ, nCol
                Case RTC_GAMMA_EQ
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.GammaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.GammaEq, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadGammaEq, gdTot.Col(RTC_GAMMA_EQ).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_GAMMA_EQ).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.GammaEq
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadGammaEq
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadGammaEq, bSimBadVal, m_Aux.Grp.GammaEq, dSimVal, RTC_GAMMA_EQ, nCol
                Case RTC_RHO
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.Rho > BAD_DOUBLE_VALUE, m_Aux.Grp.Rho, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadRho, gdTot.Col(RTC_RHO).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_RHO).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.Rho
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadRho
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadRho, bSimBadVal, m_Aux.Grp.Rho, dSimVal, RTC_RHO, nCol
                Case RTC_INDEX_DELTA_EQ
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.IdxDeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.IdxDeltaEq, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadIdxDeltaEq, gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_INDEX_DELTA_EQ).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.IdxDeltaEq
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadIdxDeltaEq
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadIdxDeltaEq, bSimBadVal, m_Aux.Grp.IdxDeltaEq, dSimVal, RTC_INDEX_DELTA_EQ, nCol
                Case RTC_BETA_WTD_DELTA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.BetaWtdDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.BetaWtdDelta, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDelta, gdTot.Col(RTC_BETA_WTD_DELTA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_BETA_WTD_DELTA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.BetaWtdDelta
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadBetaWtdDelta
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadBetaWtdDelta, bSimBadVal, m_Aux.Grp.BetaWtdDelta, dSimVal, RTC_BETA_WTD_DELTA, nCol
                Case RTC_OPT_DELTA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.OptDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.OptDelta, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadOptDelta, gdTot.Col(RTC_OPT_DELTA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_OPT_DELTA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.OptDelta
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadOptDelta
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadOptDelta, bSimBadVal, m_Aux.Grp.OptDelta, dSimVal, RTC_OPT_DELTA, nCol
                Case RTC_NET_DELTA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.NetDelta, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetDelta, gdTot.Col(RTC_NET_DELTA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_DELTA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.NetDelta
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadNetDelta
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadNetDelta, bSimBadVal, m_Aux.Grp.NetDelta, dSimVal, RTC_NET_DELTA, nCol
                Case RTC_NET_GAMMA
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.NetGamma > BAD_DOUBLE_VALUE, m_Aux.Grp.NetGamma, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGamma, gdTot.Col(RTC_NET_GAMMA).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_NET_GAMMA).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.NetGamma
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadNetGamma
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadNetGamma, bSimBadVal, m_Aux.Grp.NetGamma, dSimVal, RTC_NET_GAMMA, nCol
                Case RTC_BETA_WTD_DELTA_USD
'                    .TextMatrix(1, nCol) = IIf(m_Aux.Grp.BetaWtdDeltaEq > BAD_DOUBLE_VALUE, m_Aux.Grp.BetaWtdDeltaEq, STR_NA)
'                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadBetaWtdDeltaEq, gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColorAlt1, _
'                                                                            gdTot.Col(RTC_BETA_WTD_DELTA_USD).ForeColor)
                    If Not m_Aux.Grp.SimulationGrp Is Nothing Then
                        dSimVal = m_Aux.Grp.SimulationGrp.BetaWtdDeltaEq
                        bSimBadVal = m_Aux.Grp.SimulationGrp.BadBetaWtdDeltaEq
                    End If
                    UpdateTotalsWithSimulation m_Aux.Grp.BadBetaWtdDeltaEq, bSimBadVal, m_Aux.Grp.BetaWtdDeltaEq, dSimVal, RTC_BETA_WTD_DELTA_USD, nCol
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
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadGammaEq, gdTot.Col(RTC_GAMMA_EQ).ForeColorAlt1, _
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
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadOptDelta, gdTot.Col(RTC_OPT_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_OPT_DELTA).ForeColor)
        
                Case RTC_NET_DELTA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetDelta, gdTot.Col(RTC_NET_DELTA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_NET_DELTA).ForeColor)
        
                Case RTC_NET_GAMMA
                    .Cell(flexcpForeColor, 1, nCol) = IIf(m_Aux.Grp.BadNetGamma, gdTot.Col(RTC_NET_GAMMA).ForeColorAlt1, _
                                                                            gdTot.Col(RTC_NET_GAMMA).ForeColor)
        
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

