VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxRiskView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_bShutDown As Boolean

Private m_Idx As EtsMmRisksLib.MmRvUndAtom

Private m_Grp As EtsMmRisksLib.MmRvGrpAtom
Private m_SimGrp As EtsMmRisksLib.MmRvGrpAtom


Private m_Und As New EtsMmRisksLib.MmRvUndColl
Private m_SimUnd As New EtsMmRisksLib.MmRvUndColl
Private m_Exp As New EtsGeneralLib.EtsMmEntityAtomColl

Public VolaSource As VolatilitySourcesLib.VolatilitySource
Public gePos As clsGridEx

Public gdFlt As clsGridDef
Public gdTot As clsGridDef
Public gdPos As clsGridDef

Public fgFlt As VSFlex7Ctl.VSFlexGrid
Public fgPos As VSFlex7Ctl.VSFlexGrid
Public fgTot As VSFlex7Ctl.VSFlexGrid

Private m_nFilter(RFC_GROUP To RFC_INDEX) As Long

Private m_GridLock(GT_RISKS_FILTER To GT_RISKS_POSITIONS) As New clsGridLock

Public RealTime As Boolean
Public RealTimeConnected As Boolean

Public SortColOrder As VSFlex7Ctl.SortSettings
Public SortColKey As Long

Public m_aContext As CVLib.cvRTContext
Public m_nContextID As Long

Private m_SimulationParams As EtsMmRisksLib.MmRvSimulationParams

Public m_nGrpLevelID As Long '= 1
Public m_nUndLevelID As Long '= 2
Public m_nOptLevelID As Long '= 3

'Public m_nGrpPoolID As Long
'Public m_nUndPoolID As Long
'Public m_nOptPoolID As Long

Private m_bSimulation As Boolean

Public m_bWeightedVolaShift As Boolean

Public m_cvOptIDs As Collection
Public m_cvGrpIDs As Collection


'Stock specific shifts
Public m_nShiftID As Long

Public Enum SpotShiftTypeEnum
    enCommon = 0
    enStockSpecific = 1
    enCorrelated
End Enum

Public m_enSpotShift As SpotShiftTypeEnum

Public m_nStockShiftIndex As Long
Public m_StockShifts As Collection
Public m_UniqueStockShiftIDs As Collection


'
Public Property Let SimulationDate(SimDate As Date)
    m_SimulationParams.m_dDate = SimDate
End Property
'
Public Property Get SimulationDate() As Date
    SimulationDate = m_SimulationParams.m_dDate
End Property
'
Public Property Let SimulationVola(Vola As Double)
    m_SimulationParams.m_dVolaChangePercent = Vola
End Property
'
Public Property Get SimulationVola() As Double
    SimulationVola = m_SimulationParams.m_dVolaChangePercent
End Property
'
Public Property Let SimulationSpot(Spot As Long)
    m_SimulationParams.m_dSpotChangePercent = CDbl(Spot)
End Property
'
Public Property Get SimulationSpot() As Long
    SimulationSpot = CLng(m_SimulationParams.m_dSpotChangePercent)
End Property
'
Public Property Get SimulationParams() As EtsMmRisksLib.MmRvSimulationParams
    SimulationParams = m_SimulationParams
End Property
'
Public Property Get Simulation() As Boolean
    Simulation = m_bSimulation
End Property
'
Public Property Let Simulation(Simulation As Boolean)
    m_bSimulation = Simulation
End Property

Private Sub Class_Initialize()
    On Error Resume Next
    m_bShutDown = False
    
    Set m_Grp = New EtsMmRisksLib.MmRvGrpAtom
    
    Simulation = False
    m_SimulationParams.m_dDate = GetNewYorkTime
    m_SimulationParams.m_dSpotChangePercent = 0#
    m_SimulationParams.m_dVolaChangePercent = 0#
    Set m_SimUnd = Nothing
End Sub

Public Sub Term()
    On Error Resume Next
    m_bShutDown = True
    
    Set m_StockShifts = Nothing
    Set m_UniqueStockShiftIDs = Nothing
    Set gePos = Nothing
    
    Set gdFlt = Nothing
    Set gdTot = Nothing
    Set gdPos = Nothing
    
    m_cvOptIDs.Clear
    m_cvGrpIDs.Clear
End Sub

Public Property Get GridLock(nIdx&) As clsGridLock
    Set GridLock = m_GridLock(nIdx)
End Property

Public Property Get Filter&(nIdx&)
    Filter = m_nFilter(nIdx)
End Property

Public Property Let Filter(nIdx&, nFilter&)
    m_nFilter(nIdx) = nFilter
End Property

Public Property Get Idx() As EtsMmRisksLib.MmRvUndAtom
    Set Idx = m_Idx
End Property

Public Property Let Idx(aIdx As EtsMmRisksLib.MmRvUndAtom)
    Set m_Idx = aIdx
End Property

Public Property Get Grp() As EtsMmRisksLib.MmRvGrpAtom
    Set Grp = m_Grp
End Property

Public Property Let Grp(aGrp As EtsMmRisksLib.MmRvGrpAtom)
    Set m_Grp = aGrp
End Property

Public Property Get SimGrp() As EtsMmRisksLib.MmRvGrpAtom
    Set SimGrp = m_SimGrp
End Property

Public Property Get Und() As EtsMmRisksLib.MmRvUndColl
    Set Und = m_Und
End Property

Public Property Let Und(aUnd As EtsMmRisksLib.MmRvUndColl)
    Set m_Und = aUnd
End Property

Public Property Get SimUnd() As EtsMmRisksLib.MmRvUndColl
    Set SimUnd = m_SimUnd
End Property

Public Property Let SimUnd(aSimUnd As EtsMmRisksLib.MmRvUndColl)
    Set m_SimUnd = aSimUnd
End Property

Public Property Get Exp() As EtsGeneralLib.EtsMmEntityAtomColl
    Set Exp = m_Exp
End Property

Public Property Let Exp(aExp As EtsGeneralLib.EtsMmEntityAtomColl)
    Set m_Exp = aExp
End Property

Public Sub PosGridCompare(ByVal Row1&, ByVal Row2&, ByRef Cmp%)
    On Error Resume Next
    Dim aRowData1 As clsRvRowData, aRowData2 As clsRvRowData
    Dim aUnd1 As EtsMmRisksLib.MmRvUndAtom, aUnd2 As EtsMmRisksLib.MmRvUndAtom
    Dim aSynthUnd1 As EtsMmRisksLib.MmRvUndAtom, aSynthUnd2 As EtsMmRisksLib.MmRvUndAtom
    Dim aPos1 As EtsMmRisksLib.MmRvPosAtom, aPos2 As EtsMmRisksLib.MmRvPosAtom
    Dim aSynthGreeks1 As EtsMmRisksLib.MmRvSynthGreeksAtom, aSynthGreeks2 As EtsMmRisksLib.MmRvSynthGreeksAtom, i&
    Dim dValue1#, dValue2#, nValue1&, nValue2&, sValue1$, sValue2$
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim nIdx&
    
    Cmp = 0
        
    Set aRowData1 = fgPos.RowData(Row1)
    Set aRowData2 = fgPos.RowData(Row2)
        
    If Not aRowData1 Is Nothing And Not aRowData2 Is Nothing Then
        
        Set aUnd1 = aRowData1.Und
        Set aUnd2 = aRowData2.Und
        Set aPos1 = aRowData1.Pos
        Set aPos2 = aRowData2.Pos
        Set aSynthGreeks1 = aRowData1.SynthGreeks
        If Not aSynthGreeks1 Is Nothing Then Set aSynthUnd1 = m_Und(aSynthGreeks1.SynthUndID)
        Set aSynthGreeks2 = aRowData2.SynthGreeks
        If Not aSynthGreeks2 Is Nothing Then Set aSynthUnd2 = m_Und(aSynthGreeks2.SynthUndID)
        
        If Not aUnd1 Is Nothing And Not aUnd2 Is Nothing Then
        
            If aPos1 Is Nothing And Not aPos2 Is Nothing Then
                Cmp = -1
                GoTo Ex
            ElseIf Not aPos1 Is Nothing And aPos2 Is Nothing Then
                Cmp = 1
                GoTo Ex
            End If
            
            Select Case SortColKey
                Case RPC_SYMBOL
                    If aUnd1.Symbol > aUnd2.Symbol Then
                        Cmp = 1
                    ElseIf aUnd1.Symbol < aUnd2.Symbol Then
                        Cmp = -1
                    Else
                        If Not aPos1 Is Nothing And Not aPos2 Is Nothing Then
                            If aPos1.Symbol > aPos2.Symbol Then
                                Cmp = 1
                            ElseIf aPos1.Symbol < aPos2.Symbol Then
                                Cmp = -1
                            Else
                                If Not aSynthGreeks1 Is Nothing Then
                                    sValue1 = aSynthUnd1.Symbol
                                Else
                                    sValue1 = ""
                                End If
                                
                                If Not aSynthGreeks2 Is Nothing Then
                                    sValue2 = aSynthUnd2.Symbol
                                Else
                                    sValue2 = ""
                                End If
                                
                                If sValue1 > sValue2 Then
                                    Cmp = 1
                                ElseIf sValue1 < sValue2 Then
                                    Cmp = -1
                                End If
                            
                                If sValue1 = "" Xor sValue2 = "" Then
                                    If SortColOrder <> flexSortGenericAscending Then Cmp = -Cmp
                                End If
                            End If
                        ElseIf Not aSynthGreeks1 Is Nothing And Not aSynthGreeks2 Is Nothing Then
                            If aSynthGreeks1.Symbol > aSynthGreeks2.Symbol Then
                                Cmp = 1
                            ElseIf aSynthGreeks1.Symbol < aSynthGreeks2.Symbol Then
                                Cmp = -1
                            End If
                        Else
                            If Not aSynthGreeks1 Is Nothing Then
                                sValue1 = aSynthUnd1.Symbol
                            Else
                                sValue1 = ""
                            End If
                            
                            If Not aSynthGreeks2 Is Nothing Then
                                sValue2 = aSynthUnd1.Symbol
                            Else
                                sValue2 = ""
                            End If
                            
                            If sValue1 > sValue2 Then
                                Cmp = 1
                            ElseIf sValue1 < sValue2 Then
                                Cmp = -1
                            End If
                        
                            If sValue1 = "" Xor sValue2 = "" Then
                                If SortColOrder <> flexSortGenericAscending Then Cmp = -Cmp
                            End If
                        End If
                    End If
                    
                Case RPC_OPT_TYPE
                    If Not aPos1 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos1.OptType > aPos2.OptType Then
                            Cmp = 1
                        ElseIf aPos1.OptType < aPos2.OptType Then
                            Cmp = -1
                        End If
                    End If
                    
                Case RPC_EXPIRY
                    If Not aPos1 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos1.Expiry > aPos2.Expiry Then
                            Cmp = 1
                        ElseIf aPos1.Expiry < aPos2.Expiry Then
                            Cmp = -1
                        End If
                    End If
                    
                Case RPC_STRIKE
                    If Not aPos1 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos1.Strike > aPos2.Strike Then
                            Cmp = 1
                        ElseIf aPos1.Strike < aPos2.Strike Then
                            Cmp = -1
                        End If
                    End If
                    
                Case RPC_BID
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    If Not aSynthGreeks1 Is Nothing Then
                        If aSynthGreeks1.IsTotal Then dValue1 = aSynthUnd1.PriceBid
                    ElseIf Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PriceBid
                    Else
                        dValue1 = aUnd1.PriceBid
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        If aSynthGreeks2.IsTotal Then dValue2 = aSynthUnd2.PriceBid
                    ElseIf Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PriceBid
                    Else
                        dValue2 = aUnd2.PriceBid
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                    
                Case RPC_ASK
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    If Not aSynthGreeks1 Is Nothing Then
                        If aSynthGreeks1.IsTotal Then dValue1 = aSynthUnd1.PriceAsk
                    ElseIf Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PriceAsk
                    Else
                        dValue1 = aUnd1.PriceAsk
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        If aSynthGreeks2.IsTotal Then dValue2 = aSynthUnd2.PriceAsk
                    ElseIf Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PriceAsk
                    Else
                        dValue2 = aUnd2.PriceAsk
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                    
                Case RPC_LAST
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    If Not aSynthGreeks1 Is Nothing Then
                        If aSynthGreeks1.IsTotal Then dValue1 = aSynthUnd1.PriceLast
                    ElseIf Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PriceLast
                    Else
                        dValue1 = aUnd1.PriceLast
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        If aSynthGreeks2.IsTotal Then dValue2 = aSynthUnd2.PriceLast
                    ElseIf Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PriceLast
                    Else
                        dValue2 = aUnd2.PriceLast
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                    
                Case RPC_CLOSE
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    If Not aSynthGreeks1 Is Nothing Then
                        If aSynthGreeks1.IsTotal Then dValue1 = aSynthUnd1.PriceClose
                    ElseIf Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PriceClose
                    Else
                        dValue1 = aUnd1.PriceClose
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        If aSynthGreeks2.IsTotal Then dValue2 = aSynthUnd2.PriceClose
                    ElseIf Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PriceClose
                    Else
                        dValue2 = aUnd2.PriceClose
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                    
                Case RPC_UND_POS
                    nValue1 = BAD_LONG_VALUE
                    nValue2 = BAD_LONG_VALUE
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtStock Or aPos1.ContractType = enCtIndex Then nValue1 = aPos1.QtyInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtStock Or aPos2.ContractType = enCtIndex Then nValue2 = aPos2.QtyInShares
                    End If
                    
                    If nValue1 > nValue2 Then
                        Cmp = 1
                    ElseIf nValue1 < nValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_OPT_QTY
                    nValue1 = BAD_LONG_VALUE
                    nValue2 = BAD_LONG_VALUE
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Then nValue1 = aPos1.Qty
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Then nValue2 = aPos2.Qty
                    End If
                    
                    If nValue1 > nValue2 Then
                        Cmp = 1
                    ElseIf nValue1 < nValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_FUT_QTY
                    nValue1 = BAD_LONG_VALUE
                    nValue2 = BAD_LONG_VALUE
                    If Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtFuture Then nValue1 = aPos1.Qty
                    End If
                    
                    If Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtFuture Then nValue2 = aPos2.Qty
                    End If
                    
                    If nValue1 > nValue2 Then
                        Cmp = 1
                    ElseIf nValue1 < nValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_PNL_MTM
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PnlMtm
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PnlMtm
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_PNL_THEO
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        dValue1 = aPos1.PnlTheo
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        dValue2 = aPos2.PnlTheo
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_PNL_EDGE
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.PnlTheo > BAD_DOUBLE_VALUE And aPos1.PnlMtm > BAD_DOUBLE_VALUE Then
                            dValue1 = aPos1.PnlTheo - aPos1.PnlMtm
                        End If
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.PnlTheo > BAD_DOUBLE_VALUE And aPos2.PnlMtm > BAD_DOUBLE_VALUE Then
                            dValue2 = aPos2.PnlTheo - aPos2.PnlMtm
                        End If
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_NET_DELTA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And aPos1 Is Nothing Then
                        dValue1 = aUnd1.NetDelta
                    End If
                    
                    If aSynthGreeks2 Is Nothing And aPos2 Is Nothing Then
                        dValue2 = aUnd2.NetDelta
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_NET_DELTA_USD
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And aPos1 Is Nothing Then
                        dValue1 = aUnd1.DeltaEq
                    End If
                    
                    If aSynthGreeks2 Is Nothing And aPos2 Is Nothing Then
                        dValue2 = aUnd2.DeltaEq
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_OPT_DELTA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If Not aSynthGreeks1 Is Nothing Then
                        dValue1 = aSynthGreeks1.DeltaInShares
                    ElseIf Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.DeltaInShares
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        dValue2 = aSynthGreeks2.DeltaInShares
                    ElseIf Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.DeltaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_GAMMA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If Not aSynthGreeks1 Is Nothing Then
                        dValue1 = aSynthGreeks1.GammaInSharesPerc
                    ElseIf Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.GammaInSharesPerc
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        dValue2 = aSynthGreeks2.GammaInSharesPerc
                    ElseIf Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.GammaInSharesPerc
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_NET_GAMMA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If Not aSynthGreeks1 Is Nothing Then
                        dValue1 = aSynthGreeks1.NetGamma
                    ElseIf Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.NetGamma
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        dValue2 = aSynthGreeks2.NetGamma
                    ElseIf Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.NetGamma
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_GAMMA_SHARES
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If Not aSynthGreeks1 Is Nothing Then
                        dValue1 = aSynthGreeks1.GammaInShares
                    ElseIf Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.GammaInShares
                    End If
                    
                    If Not aSynthGreeks2 Is Nothing Then
                        dValue2 = aSynthGreeks2.GammaInShares
                    ElseIf Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.GammaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_VEGA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.VegaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.VegaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_WTD_VEGA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.WtdVega
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.WtdVega
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_THETA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.ThetaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.ThetaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_RHO
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.RhoInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.RhoInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_VEGA_DELTA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.VegaDeltaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.VegaDeltaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_VEGA_GAMMA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.VegaGammaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.VegaGammaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_THETA_DELTA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.ThetaDeltaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.ThetaDeltaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_THETA_GAMMA
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.ThetaGammaInShares
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.ThetaGammaInShares
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_UND
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        sValue1 = aUnd1.Symbol
                    ElseIf Not aSynthGreeks1 Is Nothing Then
                        If Not aSynthGreeks1.IsTotal Then sValue1 = aSynthGreeks1.SynthUndSymbol
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        sValue2 = aUnd2.Symbol
                    ElseIf Not aSynthGreeks2 Is Nothing Then
                        If Not aSynthGreeks2.IsTotal Then sValue2 = aSynthGreeks2.SynthUndSymbol
                    End If
                    
                    If sValue1 > sValue2 Then
                        Cmp = 1
                    ElseIf sValue1 < sValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_DEL_UNIT
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then

                        If Not aPos1.IsSynthetic Then
                            dValue1 = aPos1.LotSize
                        Else
                            Set aSynthRoot = aUnd1.SynthRoots(aPos1.OptionRootID)
                            If Not aSynthRoot Is Nothing Then _
                                If Not aSynthRoot.SynthRootComponents(aUnd1.ID) Is Nothing Then _
                                    dValue1 = aSynthRoot.SynthRootComponents(aUnd1.ID).Weight * aPos1.LotSize
                            Set aSynthRoot = Nothing
                        End If
                    ElseIf Not aSynthGreeks1 Is Nothing Then
                        If Not aSynthGreeks1.IsTotal Then
                            Set aSynthRoot = aUnd1.SynthRoots(aPos1.OptionRootID)
                            If Not aSynthRoot Is Nothing Then
                                If Not aSynthRoot.SynthRootComponents(aSynthGreeks1.SynthUndID) Is Nothing Then
                                    dValue1 = aSynthRoot.SynthRootComponents(aSynthGreeks1.SynthUndID).Weight * aPos1.LotSize
                                ElseIf aSynthGreeks1.SynthUndID = USD_ID Then
                                    dValue1 = aSynthRoot.CashValue * aPos1.LotSize
                                End If
                            End If
                            Set aSynthRoot = Nothing
                        End If
                    End If

                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then

                        If Not aPos2.IsSynthetic Then
                            dValue2 = aPos2.LotSize
                        Else
                            Set aSynthRoot = aUnd2.SynthRoots(aPos2.OptionRootID)
                            If Not aSynthRoot Is Nothing Then _
                                If Not aSynthRoot.SynthRootComponents(aUnd2.ID) Is Nothing Then _
                                    dValue2 = aSynthRoot.SynthRootComponents(aUnd2.ID).Weight * aPos2.LotSize
                            Set aSynthRoot = Nothing
                        End If
                    ElseIf Not aSynthGreeks2 Is Nothing Then
                        If Not aSynthGreeks2.IsTotal Then
                            Set aSynthRoot = aUnd2.SynthRoots(aPos2.OptionRootID)
                            If Not aSynthRoot Is Nothing Then
                                If Not aSynthRoot.SynthRootComponents(aSynthGreeks2.SynthUndID) Is Nothing Then
                                    dValue2 = aSynthRoot.SynthRootComponents(aSynthGreeks2.SynthUndID).Weight * aPos2.LotSize
                                ElseIf aSynthGreeks2.SynthUndID = USD_ID Then
                                    dValue2 = aSynthRoot.CashValue * aPos2.LotSize
                                End If
                            End If
                            Set aSynthRoot = Nothing
                        End If
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_TIME_VALUE
                    dValue1 = BAD_DOUBLE_VALUE
                    dValue2 = BAD_DOUBLE_VALUE
                    
                    If aSynthGreeks1 Is Nothing And Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtOption Or aPos1.ContractType = enCtFutOption Then dValue1 = aPos1.TimeValue
                    End If
                    
                    If aSynthGreeks2 Is Nothing And Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtOption Or aPos2.ContractType = enCtFutOption Then dValue2 = aPos2.TimeValue
                    End If
                    
                    If dValue1 > dValue2 Then
                        Cmp = 1
                    ElseIf dValue1 < dValue2 Then
                        Cmp = -1
                    End If
                    
                Case RPC_FUT_ROOT
                    sValue1 = ""
                    sValue2 = ""
                    
                    If Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtFuture Or aPos1.ContractType = enCtFutOption Then sValue1 = aPos1.Fut.FutRootSymbol
                    End If
                    
                    If Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtFuture Or aPos2.ContractType = enCtFutOption Then sValue2 = aPos2.Fut.FutRootSymbol
                    End If
                    
                    If sValue1 > sValue2 Then
                        Cmp = 1
                    ElseIf sValue1 < sValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_FUTURES
                    sValue1 = ""
                    sValue2 = ""
                    
                    If Not aPos1 Is Nothing Then
                        If aPos1.ContractType = enCtFuture Or aPos1.ContractType = enCtFutOption Then sValue1 = aPos1.Fut.Symbol
                    End If
                    
                    If Not aPos2 Is Nothing Then
                        If aPos2.ContractType = enCtFuture Or aPos2.ContractType = enCtFutOption Then sValue2 = aPos2.Fut.Symbol
                    End If
                    
                    If sValue1 > sValue2 Then
                        Cmp = 1
                    ElseIf sValue1 < sValue2 Then
                        Cmp = -1
                    End If
                
                Case RPC_FUT_MATURITY
                    If Not aPos1 Is Nothing And Not aPos2 Is Nothing Then
                        If Not aPos1.Fut Is Nothing And Not aPos2.Fut Is Nothing Then
                            If aPos1.Fut.Maturity > aPos2.Fut.Maturity Then
                                Cmp = 1
                            ElseIf aPos1.Fut.Maturity < aPos2.Fut.Maturity Then
                                Cmp = -1
                            End If
                        End If
                    End If
                    
            End Select
        End If
    End If
    
    If SortColOrder <> flexSortGenericAscending Then
        Cmp = -Cmp
    End If
    
Ex:
    Set aPos1 = Nothing
    Set aPos2 = Nothing
    Set aUnd1 = Nothing
    Set aUnd2 = Nothing
    Set aSynthGreeks1 = Nothing
    Set aSynthGreeks2 = Nothing
    Set aRowData1 = Nothing
    Set aRowData2 = Nothing
    Set aSynthUnd1 = Nothing
    Set aSynthUnd2 = Nothing
End Sub

Public Sub InitFltData()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        .ColComboList(RFC_GROUP) = "#" & CStr(TYPE_UNDERLYING) & ";Underlying" & _
                                    "|#" & CStr(TYPE_GROUP) & ";Group" & _
                                    "|#" & CStr(TYPE_TRADER_GROUP) & ";TraderGroup" & _
                                    "|#" & CStr(TYPE_TRADER) & ";Trader" & _
                                    "|#" & CStr(TYPE_STRATEGY) & ";Strategy" & _
                                    "|#" & CStr(TYPE_ALL) & ";<All>"
        .TextMatrix(1, RFC_GROUP) = CStr(TYPE_UNDERLYING)
        m_nFilter(RFC_GROUP) = TYPE_UNDERLYING
        
        .ColComboList(RFC_VALUE) = "#0;<None>"
        .TextMatrix(1, RFC_VALUE) = "0"
        m_nFilter(RFC_VALUE) = 0
    
        .ColComboList(RFC_TYPE) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, RFC_TYPE) = "0"
    
        .ColComboList(RFC_EXPIRY) = "#0;<All>"
        .TextMatrix(1, RFC_EXPIRY) = "0"
        m_nFilter(RFC_EXPIRY) = 0
    
        .ColComboList(RFC_INDEX) = "#0;<None>"
        .TextMatrix(1, RFC_INDEX) = "0"
        m_nFilter(RFC_INDEX) = 0
    
        .TextMatrix(1, RFC_SIM) = Simulation
        .TextMatrix(1, RFC_SIM_DATE) = SimulationParams.m_dDate
        .TextMatrix(1, RFC_SIM_VOLA) = CLng(SimulationParams.m_dVolaChangePercent)
        .TextMatrix(1, RFC_SIM_SPOT) = CLng(SimulationParams.m_dSpotChangePercent)
        
        .ColComboList(RFC_SIM_VOLA_SHIFT_TYPE) = "#0;Common|#1;Weighted"
        .TextMatrix(1, RFC_SIM_VOLA_SHIFT_TYPE) = IIf(m_bWeightedVolaShift, 1, 0)
        
        
        .ColComboList(RFC_SIM_TYPE) = "#0;Common|#1;Stock specific|#2;Correlated"
        .TextMatrix(1, RFC_SIM_TYPE) = m_enSpotShift
        
        GetUniqueStockShiftIDs
        
        .ColComboList(RFC_SHIFT_ID) = "#-1;<All>|#0;<None>|"
        
        Dim i As Variant
        For Each i In m_UniqueStockShiftIDs
            .ColComboList(RFC_SHIFT_ID) = .ColComboList(RFC_SHIFT_ID) & "#" & CStr(i) & ";" & CStr(i) & "|"
        Next
        
        .TextMatrix(1, RFC_SHIFT_ID) = m_nShiftID
        
        .TextMatrix(1, RFC_SHIFT_FILE) = Mid(g_Params.StockShiftFile, InStrRev(g_Params.StockShiftFile, "\") + 1)
        .ColComboList(RFC_SHIFT_FILE) = "..."
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub UpdateFltData()
    On Error Resume Next
    
    fgFlt.TextMatrix(1, RFC_SHIFT_ID) = m_nShiftID
End Sub

Public Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = RFC_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarNone
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        
        .HighLight = flexHighlightWithFocus
    End With
    
    With fgTot
        .Rows = 2
        .Cols = 10
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExMove
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarNone
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        
        .HighLight = flexHighlightWithFocus
    End With

    With fgPos
        .Rows = 1
        .Cols = 10
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortAndMove
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusHeavy
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .OutlineCol = 1
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
    End With
End Sub

Public Sub FormatFltGrid()
    On Error Resume Next
    Dim i&
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        For i = RFC_GROUP To RFC_LAST_COLUMN
            .TextMatrix(0, i) = gdFlt.Col(i).Caption
            .ColFormat(i) = gdFlt.Col(i).Format
            .ColDataType(i) = gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        aFont.Name = gdFlt.Font.Name
        aFont.Size = gdFlt.Font.Size
        aFont.Bold = gdFlt.Font.Bold
        aFont.Italic = gdFlt.Font.Italic
        aFont.Strikethrough = gdFlt.Font.Strikethrough
        aFont.Underline = gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = RFC_GROUP To RFC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdFlt.Col(i).BackColor <> 0, gdFlt.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdFlt.Col(i).ForeColor <> 0, gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If gdFlt.HorizLinesVisible Then
            If gdFlt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdFlt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdFlt.GridLinesColor
        
        .BackColorSel = gdFlt.BackColorSel
        .ForeColorSel = gdFlt.ForeColorSel
        
        .AutoSize 0, .Cols - 1, , 100
        
        If Simulation = False Then
            .ColHidden(RFC_SIM_DATE) = True
            .ColHidden(RFC_SIM_VOLA) = True
            .ColHidden(RFC_SIM_SPOT) = True
            
            .ColHidden(RFC_SIM_VOLA_SHIFT_TYPE) = True
            
            .ColHidden(RFC_SIM_TYPE) = True
            .ColHidden(RFC_SHIFT_ID) = True
            .ColHidden(RFC_SHIFT_FILE) = True
        Else
            .ColHidden(RFC_SHIFT_ID) = IIf(m_enSpotShift = enStockSpecific, False, True)
            .ColHidden(RFC_SHIFT_FILE) = IIf(m_enSpotShift = enStockSpecific, False, True)
            
            .ColHidden(RFC_SIM_DATE) = False
            .ColHidden(RFC_SIM_VOLA) = False
            .ColHidden(RFC_SIM_SPOT) = IIf(m_enSpotShift <> enStockSpecific, False, True)
        End If
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FormatTotGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgTot
        m_GridLock(GT_RISKS_TOTALS).LockRedraw
    
        nCols = 0
        For i = 0 To g_RTC_LAST_COLUMN
            If gdTot.Idx(i) <> RTC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            gdTot.Idx(0) = RTC_PNL_MTM
            nCols = 1
        End If
        
        .Cols = nCols
        
        For i = 0 To g_RTC_LAST_COLUMN
            nIdx = gdTot.Idx(i)
            If nIdx <> RTC_NONE Then
                .ColDataType(i) = gdTot.Col(nIdx).DataType
                .ColFormat(i) = gdTot.Col(nIdx).Format
                .ColKey(i) = nIdx
                .TextMatrix(0, i) = gdTot.Col(nIdx).Caption
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub

Public Sub FormatTotColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgTot
        m_GridLock(GT_RISKS_TOTALS).LockRedraw

        aFont.Name = gdTot.Font.Name
        aFont.Size = gdTot.Font.Size
        aFont.Bold = gdTot.Font.Bold
        aFont.Italic = gdTot.Font.Italic
        aFont.Strikethrough = gdTot.Font.Strikethrough
        aFont.Underline = gdTot.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> RTC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdTot.Col(nIdx).BackColor <> 0, gdTot.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdTot.Col(nIdx).ForeColor <> 0, gdTot.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next

        If gdTot.HorizLinesVisible Then
            If gdTot.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdTot.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdTot.GridLinesColor

        .BackColorSel = gdTot.BackColorSel
        .ForeColorSel = gdTot.ForeColorSel

        .AutoSize 0, .Cols - 1

        m_GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub

Public Sub FormatPosGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgPos
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        gdPos.Idx(0) = RPC_SYMBOL
        nCols = 1
        For i = 1 To g_RPC_LAST_COLUMN
            If gdPos.Idx(i) <> RPC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColHidden(RPC_KEY) = True
        .ColKey(RPC_KEY) = RPC_NONE
        .ColDataType(RPC_KEY) = flexDTString
        
        Set .Cell(flexcpPicture, 1, 0, .Rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        For i = 0 To g_RPC_LAST_COLUMN
            nIdx = gdPos.Idx(i)
            If nIdx <> RPC_NONE Then
                .TextMatrix(0, i + 1) = gdPos.Col(nIdx).Caption
                .ColFormat(i + 1) = gdPos.Col(nIdx).Format
                .ColDataType(i + 1) = gdPos.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(gdPos.Col(nIdx).Width > 0, gdPos.Col(nIdx).Width, -1)
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        gePos.AdjustSortImages
        
        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub FormatPosColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont, aRowData As clsRvRowData

    With fgPos
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw

        aFont.Name = gdPos.Font.Name
        aFont.Size = gdPos.Font.Size
        aFont.Bold = gdPos.Font.Bold
        aFont.Italic = gdPos.Font.Italic
        aFont.Strikethrough = gdPos.Font.Strikethrough
        aFont.Underline = gdPos.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> RPC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdPos.Col(nIdx).BackColor <> 0, gdPos.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdPos.Col(nIdx).ForeColor <> 0, gdPos.Col(nIdx).ForeColor, RGB(1, 1, 1))
                
                If nRows > 1 Then
                    If nIdx = RPC_OPT_TYPE Then
                        .ColAlignment(i) = flexAlignCenterCenter
                    Else
                        .ColAlignment(i) = flexAlignGeneral
                    End If
                End If
            End If
        Next
        
        For i = 1 To nRows
            Set aRowData = .RowData(i)
            If aRowData.Pos Is Nothing Then
                .Cell(flexcpFontBold, i, 0, i, nCols) = True
            End If
        Next

        If gdPos.HorizLinesVisible Then
            If gdPos.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdPos.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdPos.GridLinesColor
        
        .BackColorSel = gdPos.BackColorSel
        .ForeColorSel = gdPos.ForeColorSel

        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub PosBeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim nKey As Long
    If m_bShutDown Then Exit Sub
    
    SortColKey = fgPos.ColKey(Col)
    SortColOrder = IIf(SortColOrder = flexSortGenericDescending, flexSortGenericAscending, flexSortGenericDescending)
        
    nKey = fgPos.ColKey(Col)
    If nKey <> RPC_EXPIRY And nKey <> RPC_STRIKE Then
        Order = flexSortCustom
    End If
End Sub

Public Sub PosAfterSort(ByVal Col&, ByRef Order%)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim bAsc As Boolean, nKey&
    
    nKey = fgPos.ColKey(Col)
    If SortColKey = nKey Then
        Order = SortColOrder
        SortColKey = -2
    End If
    
    If nKey <> RPC_OPT_TYPE And nKey <> RPC_EXPIRY And nKey <> RPC_STRIKE Then
        Select Case SortColOrder
            Case flexSortStringNoCaseAscending, flexSortStringAscending, flexSortNumericAscending, flexSortGenericAscending
                bAsc = True
            Case Else
                bAsc = False
        End Select
        
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw
        SortPosNodes Col, bAsc
        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End If
End Sub

Public Sub SortPosNodes(ByVal nCol As Long, ByVal bAsc As Boolean)
    On Error Resume Next
    Dim nRow&, arrVals() As NodeSortItemType, nUBound&, i&, bByStrKey As Boolean
    
    With fgPos
        If .Rows <= 1 Then Exit Sub
            
        Select Case .ColDataType(nCol)
            Case flexDTShort, flexDTLong, flexDTSingle, flexDTDouble, flexDTCurrency, flexDTDate, flexDTBoolean, flexDTDecimal
                bByStrKey = False
            Case flexDTString
                bByStrKey = True
            Case Else
                Exit Sub
        End Select
        
        nUBound = 0
        nRow = .GetNodeRow(1, flexNTFirstSibling)
        While nRow > 0
            If nUBound = 0 Then ReDim arrVals(1 To 1) Else ReDim Preserve arrVals(1 To nUBound + 1)
            nUBound = nUBound + 1
            
            Set arrVals(nUBound).Node = .GetNode(nRow)
            If bByStrKey Then
                arrVals(nUBound).StrKey = .TextMatrix(nRow, nCol)
            Else
                arrVals(nUBound).NumKey = .TextMatrix(nRow, nCol)
            End If
            
            nRow = .GetNodeRow(nRow, flexNTNextSibling)
        Wend
             
        If nUBound > 1 Then
            SortNodeArray arrVals, 1, nUBound, bByStrKey
            
            For i = 1 To nUBound
                If bAsc Then
                    arrVals(i).Node.Move flexNMLast
                Else
                    arrVals(i).Node.Move flexNMFirst
                End If
            Next
        End If
        
        Erase arrVals
    End With
End Sub

Public Function CheckPosFilter(ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    nValue = m_nFilter(RFC_EXPIRY)
    bMatched = (nValue = 0)
    If Not bMatched Then
        If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
            bMatched = (nValue = aPos.ExpiryMonth)
        Else
            bMatched = False
        End If
    End If

    CheckPosFilter = bMatched
End Function

Public Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    bMatched = False
    
    If Not aTrd Is Nothing Then
'        If aTrd.ContractTypeID = enCtOption Then
'            If aTrd.Expiry <= Date Then Exit Sub
'        End If
    
        nValue = m_nFilter(RFC_VALUE)
        
        If nValue <> 0 Then
            Select Case m_nFilter(RFC_GROUP)
                Case TYPE_UNDERLYING
                    bMatched = (aTrd.UndID = nValue)
                    
                Case TYPE_GROUP
                    bMatched = Not g_UnderlyingGroup(nValue).Und(aTrd.UndID) Is Nothing
                    
                Case TYPE_TRADER
                    bMatched = (aTrd.Trader.ID = nValue)
                    
                Case TYPE_STRATEGY
                    bMatched = (aTrd.StrategyID = IIf(nValue > 0, nValue, 0))
                    
                Case TYPE_ALL
                    bMatched = True
                    
            End Select
        
            If bMatched Then
                nValue = m_nFilter(RFC_TYPE)
                bMatched = (nValue = 0)
                If Not bMatched Then
                    Select Case nValue
                        Case 1 ' Today's
                            bMatched = (Int(aTrd.TradeDate) = Int(Date) And aTrd.Status <> enTsSimulated)
                            
                        Case 2 ' OpenPos
                            bMatched = (Int(aTrd.TradeDate) < Int(Date) Or aTrd.IsPosition <> 0)
                            
                        Case 3 ' NetPos
                            bMatched = (aTrd.Status <> enTsSimulated)
                            
                        Case 4 ' Simulated
                            bMatched = (aTrd.Status = enTsSimulated)
                            
                        Case 5 ' Today's & Sim
                            bMatched = (Int(aTrd.TradeDate) = Int(Date) Or aTrd.Status = enTsSimulated)
                    
                        Case 6 ' OpenPos & Sim
                            bMatched = (Int(aTrd.TradeDate) < Int(Date) Or aTrd.IsPosition <> 0 Or aTrd.Status = enTsSimulated)
                            
                        Case 7 ' Manual
                            bMatched = (aTrd.Status = enTsManual)
                    
                        Case 8 ' Manual & Sim
                            bMatched = (aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated)
                            
                    End Select
                End If
            End If
        End If
    End If

    CheckTradeFilter = bMatched
End Function

Public Sub FilterUpdateAll()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        If m_nFilter(RFC_GROUP) <> TYPE_UNDERLYING _
            And m_nFilter(RFC_GROUP) <> TYPE_GROUP _
            And m_nFilter(RFC_GROUP) <> TYPE_TRADER_GROUP _
            And m_nFilter(RFC_GROUP) <> TYPE_TRADER _
            And m_nFilter(RFC_GROUP) <> TYPE_STRATEGY _
            And m_nFilter(RFC_GROUP) <> TYPE_ALL Then
            
            m_nFilter(RFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(RFC_VALUE) = 0
        End If
        
        If m_nFilter(RFC_GROUP) = TYPE_ALL Then m_nFilter(RFC_VALUE) = -1
        
        .TextMatrix(1, RFC_GROUP) = m_nFilter(RFC_GROUP)
        
        FilterUpdateValue False

        .TextMatrix(1, RFC_TYPE) = m_nFilter(RFC_TYPE)
        
        FilterUpdateIndex False
        FilterUpdateExpiry False
        FilterUpdateModel False

        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateValue(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim nValue&, aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    Dim aTraderGroup As EtsGeneralLib.EtsTraderGroupAtom
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = ""
        
        Select Case m_nFilter(RFC_GROUP)
            Case TYPE_ALL
                sValue = "-1"
                nValue = -1
                sComboList = "#-1;<All>"
                
            Case TYPE_UNDERLYING
                sComboList = g_Params.UnderlyingComboList
                
                Set aUnd = g_Underlying(m_nFilter(RFC_VALUE))
                If Not aUnd Is Nothing Then
                    nValue = aUnd.ID
                    sValue = Trim$(Str$(nValue))
                    Set aUnd = Nothing
                End If
                
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
                
            Case TYPE_TRADER_GROUP
                For Each aTraderGroup In g_TraderGroup
                    If aTraderGroup.ID <> 0 Then
                        sComboList = sComboList & "|#" & Trim$(Str$(aTraderGroup.ID)) & ";" & aTraderGroup.Name
                    End If
                    If aTraderGroup.ID = m_nFilter(RFC_VALUE) Then
                        nValue = aTraderGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
                
            Case TYPE_TRADER
                For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = m_nFilter(RFC_VALUE) Then
                        nValue = aTrader.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
                
            Case TYPE_GROUP
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(RFC_VALUE) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
            
            Case TYPE_STRATEGY
                sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
                If m_nFilter(RFC_VALUE) = NO_STRATEGY_ID Then
                    nValue = NO_STRATEGY_ID
                    sValue = NO_STRATEGY_NAME
                End If
                
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_nFilter(RFC_VALUE) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
              
        End Select
        
        If Len(sComboList) > 0 Then
            If nValue = 0 Then
                sComboList = "#0;<Select>" & sComboList
            Else
                If InStr(1, sComboList, "|") = 1 Then
                    sComboList = Mid$(sComboList, InStr(1, sComboList, "|") + 1)
                End If
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(RFC_VALUE) = sComboList
        .TextMatrix(1, RFC_VALUE) = sValue
        m_nFilter(RFC_VALUE) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateIndex(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sComboList$, sValue$, m_Idx As EtsGeneralLib.IndexAtom
    Dim nValue&, nFirstValue&
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        nFirstValue = 0
        sComboList = ""
'        For Each m_Idx In g_Index
        For Each m_Idx In g_HedgeSymbols
            If m_Idx.HaveComponentBetas Then
                sComboList = sComboList & "|#" & Trim$(Str$(m_Idx.ID)) & ";" & m_Idx.Symbol
                If m_Idx.ID = m_nFilter(RFC_INDEX) Then
                    nValue = m_Idx.ID
                    sValue = Trim$(Str$(m_Idx.ID))
                End If
                If nFirstValue = 0 Then
                    If (Len(g_Params.HedgeSymbol) > 0) Then
                        If (g_Params.HedgeSymbol = m_Idx.Symbol) Then
                            nFirstValue = m_Idx.ID
                        End If
                    Else
                        nFirstValue = m_Idx.ID
                    End If
                End If
            End If
        Next
        
        If Len(sComboList) > 0 Then
            sComboList = Mid$(sComboList, 2)
            If nValue = 0 Then
                nValue = nFirstValue
                sValue = Trim$(Str$(nValue))
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(RFC_INDEX) = sComboList
        .TextMatrix(1, RFC_INDEX) = sValue
        m_nFilter(RFC_INDEX) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateExpiry(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nValue&, aExp As EtsGeneralLib.EtsMmEntityAtom, sValue$, sComboList$
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aExp In m_Exp
            sComboList = sComboList & "|#" & Trim$(Str$(aExp.ID)) & ";" & aExp.Name
            If aExp.ID = m_nFilter(RFC_EXPIRY) Then
                nValue = aExp.ID
                sValue = Trim$(Str$(aExp.ID))
            End If
        Next
        .ColComboList(RFC_EXPIRY) = sComboList
        .TextMatrix(1, RFC_EXPIRY) = sValue
        m_nFilter(RFC_EXPIRY) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        .TextMatrix(1, RFC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub GridAfterMoveColumn(ByRef gd As clsGridDef, ByVal Col&, ByRef Position&, ByVal nFirstCol&)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - nFirstCol
    nNewPos = Position - nFirstCol
    nIdx = gd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then gd.Idx(i) = gd.Idx(i - 1)
        Next
        gd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then gd.Idx(i) = gd.Idx(i + 1)
        Next
        gd.Idx(nNewPos) = nIdx
    End If
End Sub

Public Sub GridBeforeMoveColumn(ByRef fg As VSFlex7Ctl.VSFlexGrid, ByRef gd As clsGridDef, _
                                ByVal Col&, ByRef Position&, ByVal nNoneCol&)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fg.ColKey(Col)
    nNewIdx = fg.ColKey(Position)
    
    If nOldIdx = nNoneCol Or nNewIdx = nNoneCol Then
        Position = Col
    ElseIf Not gd.Col(nOldIdx).CanChangeOrder Or Not gd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Public Function InitCVContext() As Boolean
    On Error Resume Next
        
    
    Set m_aContext = Nothing
    Set m_aContext = New CVLib.cvRTContext
    On Error GoTo EH
    m_nContextID = gDBW.usp_cvContextID_Get("RiskView")
    If m_nContextID = 0 Then
        gCmn.ErrorMsgBox Nothing, "RiskView CV context definitions not found. Custom variables functionality disabled"
    Else
        m_aContext.ConnectionString = g_Params.DbConnection
        m_aContext.InitContext m_nContextID
        m_nGrpLevelID = m_aContext.GetLevelIDByName("Totals")
        m_nUndLevelID = m_aContext.GetLevelIDByName("Underlyings")
        m_nOptLevelID = m_aContext.GetLevelIDByName("Positions")
        If m_nGrpLevelID = 0 Or m_nUndLevelID = 0 Or m_nOptLevelID = 0 Then
            m_aContext.Halt
            gCmn.ErrorMsgBox Nothing, "Some required levels definitions within RiskView not found. Custom variables functionality disabled"
        End If
    End If
    
    Exit Function
EH:
    m_nGrpLevelID = m_aContext.GetLevelIDByName("Totals")
    m_nUndLevelID = m_aContext.GetLevelIDByName("Underlyings")
    m_nOptLevelID = m_aContext.GetLevelIDByName("Positions")
    If m_nGrpLevelID = 0 Or m_nUndLevelID = 0 Or m_nOptLevelID = 0 Then
        m_aContext.Halt
        gCmn.ErrorMsgBox Nothing, "Some required levels definitions within RiskView not found. Custom variables functionality disabled"
    End If
    
    Dim sErrDesc$
    sErrDesc = Err.Description
    On Error Resume Next
    Screen.MousePointer = vbDefault
    If Not m_aContext Is Nothing Then m_aContext.Halt
    gCmn.MyMsgBox Nothing, "Fail to initialize custom variable engine" & vbCrLf & sErrDesc, vbCritical
End Function

Public Sub InitRisksGridsWithCV(Optional ByVal nNewVarID As Long = BAD_LONG_VALUE)
    On Error Resume Next
    Dim Info As CVLib.cvInfo
    Dim nOptCount As Long
    Dim nUndCount As Long
    Dim nGrpCount As Long
    
    Dim nNewOptColID As Long: nNewOptColID = BAD_LONG_VALUE
    Dim nNewGrpColID As Long: nNewGrpColID = BAD_LONG_VALUE
   
    Dim OptIDs() As Long
    Dim UndIDs() As Long
    Dim GrpIDs() As Long
   
    Set m_cvGrpIDs = Nothing
    Set m_cvOptIDs = Nothing
   
    Set m_cvGrpIDs = New Collection
    Set m_cvOptIDs = New Collection
   
    OptIDs = m_aContext.GetCVIDs(m_nOptLevelID)
    nOptCount = UBound(OptIDs) + 1
   
    UndIDs = m_aContext.GetCVIDs(m_nUndLevelID)
    nUndCount = UBound(UndIDs) + 1
   
    GrpIDs = m_aContext.GetCVIDs(m_nGrpLevelID)
    nGrpCount = UBound(GrpIDs) + 1
   
    InitRisksGrids nGrpCount, nOptCount + nUndCount
   
    Dim sGridID As String
    Dim i As Long
    For i = 0 To nOptCount - 1
        Info = m_aContext.GetCVInfo(m_nOptLevelID, OptIDs(i))
        sGridID = CStr(AddRisksColumn(Info, GT_RISKS_POSITIONS))
        m_cvOptIDs.Add OptIDs(i), sGridID
        
        If nNewVarID = OptIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewOptColID = sGridID
    Next
   
    For i = 0 To nUndCount - 1
        If Not OptColumnExist(UndIDs(i)) Then
            Info = m_aContext.GetCVInfo(m_nUndLevelID, UndIDs(i))
            sGridID = CStr(AddRisksColumn(Info, GT_RISKS_POSITIONS))
            m_cvOptIDs.Add UndIDs(i), sGridID
            
            If nNewVarID = UndIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewOptColID = sGridID
        End If
    Next
   
    For i = 0 To nGrpCount - 1
        Info = m_aContext.GetCVInfo(m_nGrpLevelID, GrpIDs(i))
        sGridID = CStr(AddRisksColumn(Info, GT_RISKS_TOTALS))
        m_cvGrpIDs.Add GrpIDs(i), sGridID
        
        If nNewVarID = GrpIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewGrpColID = sGridID
    Next
   
    LoadRisksDefaults
    
    If nNewOptColID <> BAD_LONG_VALUE Then DisplayNewCol GT_RISKS_POSITIONS, nNewOptColID
    If nNewGrpColID <> BAD_LONG_VALUE Then DisplayNewCol GT_RISKS_TOTALS, nNewGrpColID
    
End Sub

Private Function OptColumnExist(ByVal Idx As Long) As Boolean
    On Error Resume Next
    Dim vtID As Variant
    
    OptColumnExist = False
    
    For Each vtID In m_cvOptIDs
        If CLng(vtID) = Idx Then OptColumnExist = True
    Next
End Function

Public Function PrepareSimulation()
    On Error Resume Next
    
End Function

Public Sub GetUniqueStockShiftIDs()
    On Error Resume Next
    Dim aStockShift As clsStockShift
    
    Set m_UniqueStockShiftIDs = Nothing
    Set m_UniqueStockShiftIDs = New Collection
    
    For Each aStockShift In m_StockShifts
        If m_UniqueStockShiftIDs(CStr(aStockShift.ShiftID)) <> aStockShift.ShiftID Then
            m_UniqueStockShiftIDs.Add aStockShift.ShiftID, CStr(aStockShift.ShiftID)
        End If
    Next
End Sub
