VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxQuotesViewOut"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_Aux As clsAuxQuotesView

Public gdUnd As clsGridDef
Public gdDiv As clsGridDef
Public gdVol As clsGridDef
Public gdOpt As clsGridDef

Public fgUnd As VSFlex7Ctl.VSFlexGrid
Public fgDiv As VSFlex7Ctl.VSFlexGrid
Public fgVol As VSFlex7Ctl.VSFlexGrid
Public fgOpt As VSFlex7Ctl.VSFlexGrid

Public imgBadPrice As VB.Image
Public ExpiryColorSelection As Boolean

Public SelectedRowKey As String

Private m_bShutDown As Boolean

Public Sub Init(ByRef aAux As clsAuxQuotesView)
    On Error Resume Next
    Set m_Aux = aAux
End Sub

Public Sub Term()
    On Error Resume Next
    m_bShutDown = True
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_Aux = Nothing
End Sub

Public Function UnderlyingExchUpdate() As Long
    On Error Resume Next
    Dim nRow&
    Dim nUnds&: nUnds = 0
    
    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        For nRow = 1 To .Rows - 1
            If UnderlyingExchUpdateQuote(nRow) Then
                nUnds = nUnds + 1
                                                Else
                Exit For
            End If
        Next
        
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
    
    UnderlyingExchUpdate = nUnds
End Function

Public Function UnderlyingExchUpdateQuote(ByVal nRow As Long) As Boolean
    On Error Resume Next
    Dim aExch As EtsGeneralLib.ExchAtom, nCol&
    Dim aRowData As MmQvRowData, aUnd As EtsMmQuotesLib.MmQvUndAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom, aFutRoot As EtsMmQuotesLib.MmQvFutRootAtom
    Dim aFirstRowData As MmQvRowData
    Dim aFirstRowQuote As EtsMmQuotesLib.MmQvQuoteAtom
    
    UnderlyingExchUpdateQuote = True
    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw

        Set aRowData = .RowData(nRow)
        
        Set aUnd = aRowData.Und
        Set aQuote = aRowData.UndQuote
        Set aFut = aRowData.Fut
        Set aFutRoot = aRowData.FutRoot

        Set aFirstRowData = .RowData(1)
        Set aFirstRowQuote = aFirstRowData.UndQuote
                            

        UnderlyingExchUpdateQuote = (Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged)

        If Not aUnd Is Nothing And Not aQuote Is Nothing _
            And (Not m_Aux.Grp.IsStockOrIndex And Not aFut Is Nothing And Not aFutRoot Is Nothing) Then
        
            If Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged Then
                 If (nRow > 1) And (aQuote.Exch.Code = "G") Then
                   If (aFirstRowQuote.PriceBid <= BAD_DOUBLE_VALUE) And _
                     (aFirstRowQuote.PriceAsk <= BAD_DOUBLE_VALUE) And _
                        (aFirstRowQuote.PriceLast <= BAD_DOUBLE_VALUE) And _
                          (aQuote.PriceBid > BAD_DOUBLE_VALUE) Then
                             .TextMatrix(1, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA)
                                 aFut.PosExchID = aQuote.Exch.ID
                                                                  
                                 aFirstRowQuote.Exch.Code = aQuote.Exch.Code
                                 aFirstRowQuote.Exch.Name = aQuote.Exch.Name
                                 aFirstRowQuote.Exch.ID = aQuote.Exch.ID
                                 
                                 aFirstRowQuote.PriceBid = aQuote.PriceBid
                                 aFirstRowQuote.PriceAsk = aQuote.PriceAsk
                                 aFirstRowQuote.PriceOpen = aQuote.PriceOpen
                                 aFirstRowQuote.PriceClose = aQuote.PriceClose
                                 aFirstRowQuote.PriceHigh = aQuote.PriceHigh
                                 aFirstRowQuote.PriceLow = aQuote.PriceLow
                                 aFirstRowQuote.PriceLast = aQuote.PriceLast
                                 aFirstRowQuote.Volume = aQuote.Volume
                                 aFirstRowQuote.AskExchange = aQuote.AskExchange
                                 aFirstRowQuote.BidExchange = aQuote.BidExchange
                                 aFirstRowQuote.NetChange = aQuote.NetChange
                                 aFirstRowQuote.SizeBid = aQuote.SizeBid
                                 aFirstRowQuote.SizeAsk = aQuote.SizeAsk
                                 aFirstRowQuote.UpdateTime = aQuote.UpdateTime

                    End If
                End If
           End If
        Else
           UnderlyingExchUpdateQuote = False
        End If
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
                    
    aQuote.PriceUpdateStatus = enMmQvPusNone
    Set aUnd = Nothing
    Set aQuote = Nothing
    Set aRowData = Nothing
    Set aExch = Nothing
    Set aFut = Nothing
    Set aFutRoot = Nothing
End Function


Public Function UnderlyingUpdate(ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, Optional ManualEdit As Boolean = True) As Long
    On Error Resume Next
    Dim nRow&
    Dim nUnds&: nUnds = 0
    
    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        For nRow = 1 To .Rows - 1
            If UnderlyingUpdateQuote(nRow, bSymbol, bUpdateColors, ManualEdit) Then nUnds = nUnds + 1
        Next
        
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
    
    UnderlyingUpdate = nUnds
End Function

Public Function UnderlyingUpdateQuote(ByVal nRow As Long, ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, Optional ManualEdit As Boolean = True) As Boolean
    On Error Resume Next
    Dim aExch As EtsGeneralLib.ExchAtom, nCol&
    Dim aRowData As MmQvRowData, aUnd As EtsMmQuotesLib.MmQvUndAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom, aFutRoot As EtsMmQuotesLib.MmQvFutRootAtom
    Dim bIsPriceUsed As Boolean: bIsPriceUsed = True

    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw

        Set aRowData = .RowData(nRow)
        Set aUnd = aRowData.Und
        Set aQuote = aRowData.UndQuote
        Set aFut = aRowData.Fut
        Set aFutRoot = aRowData.FutRoot

        UnderlyingUpdateQuote = (Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged)

        If Not aUnd Is Nothing And Not aQuote Is Nothing _
            And (m_Aux.Grp.IsStockOrIndex Or Not m_Aux.Grp.IsStockOrIndex And Not aFut Is Nothing And Not aFutRoot Is Nothing) Then
            
            If Not aUnd.UndPriceProfile Is Nothing Then _
                bIsPriceUsed = Not aUnd.UndPriceProfile.AlwaysUseLast
            
            Dim i&, nIdx&
            i = 0
            nIdx = gdUnd.Idx(0)
            While nIdx >= 0 And i <= QUC_LAST_COLUMN

                nCol = i + 1
                If bSymbol Then
                    Select Case nIdx
                        Case QUC_SYMBOL
                            If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aUnd.Symbol, "")
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFut.Symbol, "")
                            End If

                        Case QUC_EXCHANGE
                            .TextMatrix(nRow, nCol) = aQuote.Exch.Code
                        
                        Case QUC_SYMBOL_NAME
                            If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aUnd.SymbolName, "")
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFut.ContractName, "")
                            End If
                   
                        Case QUC_DPC
                            If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aQuote.LotSize, "")
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFutRoot.FutLotSize, "")
                            End If
                            
                        Case QUC_FUT_ROOT
                            If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFutRoot.Symbol, "")
                            End If
                    End Select
                End If

                Select Case nIdx
                Case QUC_BA_EXCHANGE
                           .TextMatrix(nRow, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE And aQuote.BidExchange <> "", aQuote.BidExchange, STR_NA) + "/" + IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE And aQuote.AskExchange <> "", aQuote.AskExchange, STR_NA)

                    Case QUC_BID
                        If Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged Then _
                            .TextMatrix(nRow, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA)

                        If bUpdateColors Then
                            If aQuote.PriceBidTick = 0# Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColor Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColor

                            ElseIf aQuote.PriceBidTick < 0# Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColorAlt2 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColorAlt2

                            Else
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColorAlt1 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColorAlt1
                            End If
                        End If
                        
'                        If Not ManualEdit Then
'                            Set .Cell(flexcpPicture, nRow, nCol) = Nothing
'                        Else
                            If ((aQuote.ReplacePriceStatus And enRpsBid) And aQuote.PriceBid = BAD_DOUBLE_VALUE) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            End If
'                        End If

                    Case QUC_ASK
                        If Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged Then _
                            .TextMatrix(nRow, nCol) = IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA)

                        If bUpdateColors Then
                            If aQuote.PriceAskTick = 0# Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColor Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColor

                            ElseIf aQuote.PriceAskTick < 0# Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColorAlt2 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColorAlt2

                            Else
                                If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColorAlt1 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColorAlt1
                            End If
                        End If
                        
'                        If Not ManualEdit Then
'                            Set .Cell(flexcpPicture, nRow, nCol) = Nothing
'                        Else
                            If ((aQuote.ReplacePriceStatus And enRpsAsk) And aQuote.PriceAsk) Or _
                               (Not bIsPriceUsed) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                            Else
                                Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            End If
'                        End If

                    Case QUC_OPEN
                        .TextMatrix(nRow, nCol) = IIf(aQuote.PriceOpen > BAD_DOUBLE_VALUE, aQuote.PriceOpen, STR_NA)

                    Case QUC_CLOSE
                        .TextMatrix(nRow, nCol) = IIf(aQuote.PriceClose > BAD_DOUBLE_VALUE, aQuote.PriceClose, STR_NA)

                    Case QUC_HIGH
                        .TextMatrix(nRow, nCol) = IIf(aQuote.PriceHigh > BAD_DOUBLE_VALUE, aQuote.PriceHigh, STR_NA)

                    Case QUC_LOW
                        .TextMatrix(nRow, nCol) = IIf(aQuote.PriceLow > BAD_DOUBLE_VALUE, aQuote.PriceLow, STR_NA)

'                    Case QUC_SU_PRICE
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.SU_Price > BAD_DOUBLE_VALUE, aUnd.SU_Price, STR_NA)
                    Case QUC_LAST
                        .TextMatrix(nRow, nCol) = IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA)

                    Case QUC_VOLUME
                        .TextMatrix(nRow, nCol) = IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA)

                    Case QUC_POS
                        If m_Aux.Grp.IsStockOrIndex Then
                            If aQuote.Exch.ID = aUnd.PosExchID And aUnd.ID = m_Aux.Grp.Und.ID Then
                                .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = ""
                            End If
                        Else
                            If aQuote.Exch.ID = aFut.PosExchID Then
                                .TextMatrix(nRow, nCol) = IIf(aFut.QtyInShares > BAD_LONG_VALUE, aFut.QtyInShares, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = ""
                            End If
                        End If

                    Case QUC_FUT_QTY
                        If m_Aux.Grp.IsStockOrIndex Then
                            .TextMatrix(nRow, nCol) = ""
                        Else
                            If aQuote.Exch.ID = aFut.PosExchID Then
                                .TextMatrix(nRow, nCol) = IIf(aFut.Qty > BAD_LONG_VALUE, aFut.Qty, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = ""
                            End If
                        End If

                    Case QUC_NET_CHANGE
                        .TextMatrix(nRow, nCol) = IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA)

                    Case QUC_BID_SIZE
                        .TextMatrix(nRow, nCol) = IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA)

                    Case QUC_ASK_SIZE
                        .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)

                    Case QUC_UPDATE_TIME
                        .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
                    
                End Select

                i = i + 1
                nIdx = gdUnd.Idx(i)
            Wend

            aQuote.PriceUpdateStatus = enMmQvPusNone
        End If

        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With

    Set aUnd = Nothing
    Set aQuote = Nothing
    Set aRowData = Nothing
    Set aExch = Nothing
    Set aFut = Nothing
    Set aFutRoot = Nothing
End Function


Public Sub UnderlyingUpdateTotals()
    On Error Resume Next
    Dim nCol&, nRow&
    Dim aRowData As MmQvRowData, aUnd As EtsMmQuotesLib.MmQvUndAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom

    With fgUnd
        For nRow = 1 To .Rows
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
            Set aRowData = .RowData(nRow)
            Set aUnd = aRowData.Und
            Set aQuote = aRowData.UndQuote
            Set aFut = aRowData.Fut
        
            If Not aUnd Is Nothing And Not aQuote Is Nothing _
                And (m_Aux.Grp.IsStockOrIndex Or Not m_Aux.Grp.IsStockOrIndex And Not aFut Is Nothing) Then
                
                Dim i&, nIdx&
                i = 0
                nIdx = gdUnd.Idx(0)
                While nIdx >= 0 And i <= QUC_LAST_COLUMN
    
                    nCol = i + 1
                    If m_Aux.Grp.IsStockOrIndex Then
                        Select Case nIdx
                            Case QUC_POS
                                If aQuote.Exch.ID = aUnd.PosExchID And aUnd.ID = m_Aux.Grp.Und.ID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
                            Case QUC_FUT_QTY
                                .TextMatrix(nRow, nCol) = ""
        
                            Case QUC_NET_DELTA
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.NetDelta > BAD_DOUBLE_VALUE, aUnd.NetDelta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_OPT_DELTA
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.TotalDelta > BAD_DOUBLE_VALUE, aUnd.TotalDelta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_GAMMA_USD
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.TotalGamma > BAD_DOUBLE_VALUE, aUnd.TotalGamma, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_VEGA
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.TotalVega > BAD_DOUBLE_VALUE, aUnd.TotalVega, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_THETA
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.TotalTheta > BAD_DOUBLE_VALUE, aUnd.TotalTheta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                                
                            Case QUC_RHO
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.TotalRho > BAD_DOUBLE_VALUE, aUnd.TotalRho, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
                        End Select
                    Else
                        Select Case nIdx
                            Case QUC_POS
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.QtyInShares > BAD_LONG_VALUE, aFut.QtyInShares, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
                            Case QUC_FUT_QTY
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.Qty > BAD_LONG_VALUE, aFut.Qty, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
                            Case QUC_NET_DELTA
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.NetDelta > BAD_DOUBLE_VALUE, aFut.NetDelta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_OPT_DELTA
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.TotalDelta > BAD_DOUBLE_VALUE, aFut.TotalDelta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_GAMMA_USD
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.TotalGamma > BAD_DOUBLE_VALUE, aFut.TotalGamma, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_VEGA
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.TotalVega > BAD_DOUBLE_VALUE, aFut.TotalVega, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
                            Case QUC_THETA
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.TotalTheta > BAD_DOUBLE_VALUE, aFut.TotalTheta, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                                
                            Case QUC_RHO
                                If aQuote.Exch.ID = aFut.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aFut.TotalRho > BAD_DOUBLE_VALUE, aFut.TotalRho, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
                        End Select
                    End If
    
                    i = i + 1
                    nIdx = gdUnd.Idx(i)
                Wend
    
            End If
    
            m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
        Next
    End With

    Set aUnd = Nothing
    Set aQuote = Nothing
    Set aFut = Nothing
    Set aRowData = Nothing
End Sub

Public Sub UnderlyingUpdateColors()
    On Error Resume Next
    Dim nRow&
    
    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        For nRow = 1 To .Rows - 1
            UnderlyingUpdateQuoteColors nRow, Nothing
        Next
        
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
End Sub

Public Sub UnderlyingUpdateQuoteColors(ByVal nRow As Long, ByRef aRowQuote As EtsMmQuotesLib.MmQvQuoteAtom, Optional ByVal bShowUpdateTime As Boolean = False)
    On Error Resume Next
    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nCol&

    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        If aRowQuote Is Nothing Then
            Set aQuote = .RowData(nRow).UndQuote
        Else
            Set aQuote = aRowQuote
        End If
        
        If Not aQuote Is Nothing Then
            nCol = .ColIndex(QUC_BID)
            If nCol >= 0 Then
                If aQuote.PriceBidTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColor
                    
                ElseIf aQuote.PriceBidTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColorAlt2
                    
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_BID).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_BID).ForeColorAlt1
                End If
            End If
            
            nCol = .ColIndex(QUC_ASK)
            If nCol >= 0 Then
                If aQuote.PriceAskTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColor
                    
                ElseIf aQuote.PriceAskTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColorAlt2
                    
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdUnd.Col(QUC_ASK).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdUnd.Col(QUC_ASK).ForeColorAlt1
                End If
            End If
            
            If bShowUpdateTime Then
                nCol = .ColIndex(QUC_UPDATE_TIME)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
            End If
        End If
        
        Set aQuote = Nothing

        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
End Sub

Public Function OptionsUpdate(ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, ByVal bForceUpdate As Boolean) As Long
    On Error Resume Next
    Dim nRow&
    Dim nQuotes&: nQuotes = m_Aux.QV.OptResponses
    
    fgOpt.Refresh

'    With fgOpt
'        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
'
'        For nRow = 1 To .Rows - 1
'            If OptionUpdateQuote(nRow, enOtCall, bSymbol, bUpdateColors, bForceUpdate) Then nQuotes = nQuotes + 1
'            If OptionUpdateQuote(nRow, enOtPut, bSymbol, bUpdateColors, bForceUpdate) Then nQuotes = nQuotes + 1
'        Next
'
'        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
'    End With

    OptionsUpdate = nQuotes
End Function
'
'Public Function OptionUpdateQuote(ByVal nRow As Long, ByVal nOptType As Long, ByVal bSymbol As Boolean, _
'                                    ByVal bUpdateColors As Boolean, ByVal bForceUpdate As Boolean) As Boolean
'    On Error Resume Next
'    OptionUpdateQuote = True
'
''    Dim aRowData As MmQvRowData, aOpt As EtsMmQuotesLib.MmQvOptAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
''    Dim nCol&, nShift&, aRoot As EtsMmQuotesLib.MmQvOptRootAtom, sTemp$, aSyntComp As EtsGeneralLib.SynthRootCompAtom
''    Dim bUpdOpt: bUpdOpt = False
''    Dim aUndQuote As EtsMmQuotesLib.MmQvQuoteAtom
''    Dim aFut As EtsMmQuotesLib.MmQvFutAtom, aFutRoot As EtsMmQuotesLib.MmQvFutRootAtom
''
''    With fgOpt
''        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
''
''        Set aRowData = .RowData(nRow)
''        Set aOpt = aRowData.Opt(nOptType)
''        Set aQuote = aRowData.OptQuote(nOptType)
''        Set aRoot = aRowData.OptRoot
''        Set aFut = aRowData.Fut
''        Set aFutRoot = aRowData.FutRoot
''
''        If nOptType = enOtPut Then
''            nShift = QOC_P_SYMBOL - 2
''        Else
''            nShift = 0
''        End If
''
''        Dim i&, nIdx&, nColIdx&
''        i = 0
''        nIdx = gdOpt.Idx(0)
''
''        If Not (aOpt Is Nothing Or aQuote Is Nothing) Then
''
''            bUpdOpt = (bForceUpdate Or Not m_Aux.RealTime Or aQuote.PriceUpdateStatus <> enMmQvPusNone)
''
''            If Not bUpdOpt Then
''                If m_Aux.Grp.IsStockOrIndex Then
''                    If Not aRoot Is Nothing Then
''                        If aRoot.Synthetic Then
''                            If Not aRoot.SynthOptRoot.SynthRootComponents Is Nothing Then
''                                For Each aSyntComp In aRoot.SynthOptRoot.SynthRootComponents
''                                    Set aUndQuote = m_Aux.Grp.Und.SynthUnd(aSyntComp.UndID).Quote(0)
''                                    If Not aUndQuote Is Nothing Then
''                                        If aUndQuote.PriceUpdateStatus = enMmQvPusChanged Then
''                                            bUpdOpt = True
''                                            Exit For
''                                        End If
''                                        Set aUndQuote = Nothing
''                                    End If
''                                Next
''                            End If
''                        Else
''                            Set aUndQuote = m_Aux.Grp.Und.Quote(0)
''                            If aUndQuote.PriceUpdateStatus = enMmQvPusChanged Then bUpdOpt = True
''                        End If
''
''                        Set aUndQuote = Nothing
''                    End If
''                Else
''                    Set aUndQuote = m_Aux.Grp.Und.Quote(0)
''                    If aUndQuote.PriceUpdateStatus = enMmQvPusChanged Then bUpdOpt = True
''                    Set aUndQuote = Nothing
''
''                    If Not bUpdOpt Then
''                        Set aUndQuote = m_Aux.Grp.Fut.Quote(0)
''                        If aUndQuote.PriceUpdateStatus = enMmQvPusChanged Then bUpdOpt = True
''                        Set aUndQuote = Nothing
''                    End If
''                End If
''            End If
''
''            If bUpdOpt Then
''
''                    While nIdx >= 0 And i <= QOC_LAST_COLUMN
''
''                        nCol = i + 2
''                        nColIdx = nIdx
''
''                        If nIdx >= nShift + QOC_C_SYMBOL And nIdx <= nShift + QOC_C_UPDATE_TIME Then
''                            nIdx = nIdx - nShift
''                            If bSymbol Then
''                                Select Case nIdx
''                                    Case QOC_C_SYMBOL
''                                        .TextMatrix(nRow, nCol) = aOpt.Symbol
''
''                                    Case QOC_C_EXCHANGE
''                                        .TextMatrix(nRow, nCol) = aQuote.Exch.Code
''
''                                    Case QOC_C_SERIES
''                                        .TextMatrix(nRow, nCol) = aQuote.Series
''
''                                    Case QOC_C_STRIKE
''                                        .TextMatrix(nRow, nCol) = aOpt.Strike
''
''                                    Case QOC_C_EXPIRY
''                                        .TextMatrix(nRow, nCol) = aOpt.Expiry
''                                End Select
''                            End If
''
''                            Select Case nIdx
''
''                                Case QOC_C_BA_EXCHANGE
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE And aQuote.BidExchange <> "", aQuote.BidExchange, STR_NA) + "/" + IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE And aQuote.AskExchange <> "", aQuote.AskExchange, STR_NA)
''
''                                Case QOC_C_BID
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA)
''
''                                    If bUpdateColors Then
''                                        If aQuote.PriceBidTick = 0# Then
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColor Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColor
''
''                                        ElseIf aQuote.PriceBidTick < 0# Then
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt2 Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt2
''
''                                        Else
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt1 Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt1
''                                        End If
''                                    End If
''
''                                Case QOC_C_ASK
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA)
''
''                                    If bUpdateColors Then
''                                        If aQuote.PriceAskTick = 0# Then
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColor Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColor
''
''                                        ElseIf aQuote.PriceAskTick < 0# Then
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt2 Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt2
''
''                                        Else
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt1 Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt1
''                                        End If
''                                    End If
''
''                                Case QOC_C_LAST
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA)
''
''                                Case QOC_C_VOLUME
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA)
''
''                                Case QOC_C_QTY
''                                    If m_Aux.Grp.IsStockOrIndex Then
''                                        If aQuote.Exch.ID = m_Aux.Grp.Und.OptPosExchID Then
''                                            .TextMatrix(nRow, nCol) = IIf(aOpt.Qty > BAD_LONG_VALUE, aOpt.Qty, STR_NA)
''                                        Else
''                                            .TextMatrix(nRow, nCol) = STR_NA
''                                        End If
''                                    Else
''                                        If aQuote.Exch.ID = m_Aux.Grp.Fut.OptPosExchID Then
''                                            .TextMatrix(nRow, nCol) = IIf(aOpt.Qty > BAD_LONG_VALUE, aOpt.Qty, STR_NA)
''                                        Else
''                                            .TextMatrix(nRow, nCol) = STR_NA
''                                        End If
''                                    End If
''
''                                Case QOC_C_OPEN_INTEREST
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.OpenInterest > BAD_LONG_VALUE, aQuote.OpenInterest, STR_NA)
''
''                                Case QOC_C_THEO_PRICE
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.PriceTheo > BAD_DOUBLE_VALUE, aQuote.PriceTheo, STR_NA)
''
''                                    If bUpdateColors Then
''                                        If aQuote.PriceTheo >= 0# Then
''                                            If aQuote.PriceTheo < aQuote.PriceBid Then
''                                                If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt1 Then _
''                                                    .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt1
''
''                                            ElseIf aQuote.PriceAsk >= 0# And aQuote.PriceTheo > aQuote.PriceAsk Then
''                                                If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColorAlt2 Then _
''                                                    .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColorAlt2
''
''                                            Else
''                                                If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColor Then _
''                                                    .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColor
''                                            End If
''                                        Else
''                                            If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nColIdx).ForeColor Then _
''                                                .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nColIdx).ForeColor
''                                        End If
''                                    End If
''
''                                Case QOC_C_DELTA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Delta > BAD_DOUBLE_VALUE, aQuote.Delta, STR_NA)
''
''                                Case QOC_C_GAMMA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Gamma > BAD_DOUBLE_VALUE, aQuote.Gamma, STR_NA)
''
''                                Case QOC_C_VEGA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Vega > BAD_DOUBLE_VALUE, aQuote.Vega, STR_NA)
''
''                                Case QOC_C_THETA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Theta > BAD_DOUBLE_VALUE, aQuote.Theta, STR_NA)
''
''                                Case QOC_C_RHO
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.Rho > BAD_DOUBLE_VALUE, aQuote.Rho, STR_NA)
''
''                                Case QOC_C_VEGA_DELTA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.VegaDelta > BAD_DOUBLE_VALUE, aQuote.VegaDelta, STR_NA)
''
''                                Case QOC_C_VEGA_GAMMA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.VegaGamma > BAD_DOUBLE_VALUE, aQuote.VegaGamma, STR_NA)
''
''                                Case QOC_C_THETA_DELTA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.ThetaDelta > BAD_DOUBLE_VALUE, aQuote.ThetaDelta, STR_NA)
''
''                                Case QOC_C_THETA_GAMMA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.ThetaGamma > BAD_DOUBLE_VALUE, aQuote.ThetaGamma, STR_NA)
''
''                                Case QOC_C_GAMMA_PER_THETA
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.GammaPerTheta > BAD_DOUBLE_VALUE, aQuote.GammaPerTheta, STR_NA)
''
''                                Case QOC_C_VOLA
''                                    If aOpt.Vola > BAD_DOUBLE_VALUE Then
''                                        .TextMatrix(nRow, nCol) = aOpt.Vola * 100#
''                                    Else
''                                        .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''
''                                Case QOC_C_IV
''                                    If aQuote.IV > BAD_DOUBLE_VALUE Then
''                                        .TextMatrix(nRow, nCol) = aQuote.IV * 100#
''                                    Else
''                                        .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''
''                                Case QOC_C_IV_BID
''                                    If aQuote.IVBid > BAD_DOUBLE_VALUE Then
''                                        .TextMatrix(nRow, nCol) = aQuote.IVBid * 100#
''                                    Else
''                                        .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''
''                                Case QOC_C_IV_ASK
''                                    If aQuote.IVAsk > BAD_DOUBLE_VALUE Then
''                                        .TextMatrix(nRow, nCol) = aQuote.IVAsk * 100#
''                                    Else
''                                        .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''
''                                Case QOC_C_NET_CHANGE
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA)
''
''                                Case QOC_C_BID_SIZE
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA)
''
''                                Case QOC_C_ASK_SIZE
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)
''
''                                Case QOC_C_IV_SPREAD
''                                    If aQuote.IVSpread > BAD_DOUBLE_VALUE Then
''                                        .TextMatrix(nRow, nCol) = aQuote.IVSpread * 100#
''                                    Else
''                                        .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''                                Case QOC_C_UPDATE_TIME
''                                    .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
''                            End Select
''
''                        ElseIf bSymbol Then
''                            Select Case nIdx
''                                Case QOC_ROOT
''                                    If m_Aux.Grp.IsStockOrIndex Then
''                                        .TextMatrix(nRow, nCol) = aRoot.Name
''                                    Else
''                                        .TextMatrix(nRow, nCol) = aFut.Symbol
''                                    End If
''                                Case QOC_IS_SYNTH
''                                    .TextMatrix(nRow, nCol) = aRoot.Synthetic
''                                Case QOC_SU_PRICE
''                                    If aRowData.OptRoot.SU_Price > BAD_DOUBLE_VALUE Then
''                                       .TextMatrix(nRow, nCol) = aRowData.OptRoot.SU_Price
''                                    Else
''                                       .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''
''                                Case QOC_DPC
''                                    If m_Aux.Grp.IsStockOrIndex Then
''                                        .TextMatrix(nRow, nCol) = aRoot.DPC
''                                    Else
''                                        .TextMatrix(nRow, nCol) = Trim$(Str$(aFutRoot.FutLotSize * aFutRoot.OptLotSize)) & " " & m_Aux.Grp.Und.Symbol
''                                    End If
''
''                            End Select
''                       Else
''                            Select Case nIdx
''                                Case QOC_SU_PRICE
''                                    If aRowData.OptRoot.SU_Price > BAD_DOUBLE_VALUE Then
''                                       .TextMatrix(nRow, nCol) = aRowData.OptRoot.SU_Price
''                                    Else
''                                       .TextMatrix(nRow, nCol) = STR_NA
''                                    End If
''                            End Select
''                       End If
''                        i = i + 1
''                        nIdx = gdOpt.Idx(i)
''                    Wend
''            End If
''
''            aQuote.PriceUpdateStatus = enMmQvPusNone
''        Else
''            While nIdx >= 0 And i <= QOC_LAST_COLUMN
''                If nIdx >= nShift + QOC_C_SYMBOL And nIdx <= nShift + QOC_C_UPDATE_TIME Then
''                    .TextMatrix(nRow, i + 2) = ""
''                End If
''                i = i + 1
''                nIdx = gdOpt.Idx(i)
''            Wend
''        End If
''
''        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
''    End With
''
''    OptionUpdateQuote = bUpdOpt
''
''    Set aOpt = Nothing
''    Set aQuote = Nothing
''    Set aRoot = Nothing
''    Set aRowData = Nothing
''    Set aFut = Nothing
''    Set aFutRoot = Nothing
'End Function
                             
Public Sub OptionsUpdateBackColor(ByVal bSeparateExpiries As Boolean)
    On Error Resume Next
    Dim i&, nIdx&, nRows&, nCol&, clrBackColor As OLE_COLOR, nRow&, nStartRow&, nEndRow&, aRowData As MmQvRowData
    Dim dtExpiryMonth As Date, dtNextExpiryMonth As Date, aExp As EtsMmQuotesLib.MmQvExpAtom, bUseAltColor As Boolean
    
    If bSeparateExpiries Then
       ' If Not ExpiryColorSelection Then
            'ExpiryColorSelection = False
                    
            With fgOpt
                nStartRow = 1&
                dtExpiryMonth = 0
                bUseAltColor = True
                nRows = .Rows - 1
                                
                For nRow = 1 To nRows
                    Set aRowData = m_Aux.QV.OptsRowData(nRow)
                    Set aExp = aRowData.Exp
                    
                    If Not aExp Is Nothing Then
                        dtNextExpiryMonth = aExp.ExpiryMonth
                    Else
                        dtNextExpiryMonth = -1
                    End If
                                        
                    If dtExpiryMonth = 0 Then dtExpiryMonth = dtNextExpiryMonth
                                        
                    If nRow = nRows Or dtNextExpiryMonth <> dtExpiryMonth Then
                        nEndRow = IIf(nRow <> nRows, nRow - 1, nRow)
                        dtExpiryMonth = dtNextExpiryMonth
                        bUseAltColor = Not bUseAltColor
                        
                        i = 0
                        nIdx = gdOpt.Idx(0)
                        
                        While nIdx >= 0 And i <= QOC_LAST_COLUMN
                            nCol = i + 2
                            If nIdx <> QOC_NONE Then
                                clrBackColor = gdOpt.Col(nIdx).BackColor
                                If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
                                
                                .Cell(flexcpBackColor, nStartRow, nCol, nEndRow, nCol) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
                            End If
                            i = i + 1
                            nIdx = gdOpt.Idx(i)
                        Wend
                        
                        nStartRow = nEndRow + 1
                    End If
                    
                    Set aExp = Nothing
                    Set aRowData = Nothing
                Next
            End With
       ' End If
    Else
        'If ExpiryColorSelection Then
            ExpiryColorSelection = False
            With fgOpt
                i = 0
                nIdx = gdOpt.Idx(0)
                nRows = .Rows - 1
                
                While nIdx >= 0 And i <= QOC_LAST_COLUMN
                    nCol = i + 2
                    If nIdx <> QOC_NONE Then
                        .Cell(flexcpBackColor, 1, nCol, nRows, nCol) = IIf(gdOpt.Col(nIdx).BackColor <> 0, gdOpt.Col(nIdx).BackColor, RGB(1, 1, 1))
                    End If
                    i = i + 1
                    nIdx = gdOpt.Idx(i)
                Wend
            End With
       ' End If
    End If
    
    nRow = fgOpt.FindRow(SelectedRowKey, , QOC_C_KEY)
    If nRow > 0 Then
        fgOpt.IsSelected(nRow) = True
        fgOpt.Row = nRow
    Else
        fgOpt.IsSelected(1) = True
        fgOpt.Row = 1
        fgOpt.Col = QOC_C_SYMBOL
    End If
End Sub

Public Sub ShowSelectedRow()
    On Error Resume Next
    Dim nDummy As Long
    nDummy = fgOpt.CellTop
End Sub

Public Sub OptionsUpdateColors()
    On Error Resume Next
    Dim i&, nRows&
    
    With fgOpt
        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
    
        nRows = .Rows - 1
        For i = 1 To nRows
            OptionUpdateColors i, enOtCall
            OptionUpdateColors i, enOtPut
        Next
        
        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    End With
End Sub

Public Sub OptionUpdateColors(ByVal nRow As Long, ByVal nOptType As Long, Optional ByVal bShowUpdateTime As Boolean = False)
    On Error Resume Next
    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nCol&, aRowData As MmQvRowData, nShift&
    
    With fgOpt
        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
        
        Set aRowData = m_Aux.QV.OptsRowData(nRow)
        
        Set aOpt = aRowData.Opt(nOptType)
        Set aQuote = aRowData.OptQuote(nOptType)
        
        If Not (aOpt Is Nothing Or aQuote Is Nothing) Then
            If nOptType = enOtPut Then
                nShift = QOC_P_SYMBOL - 2
            Else
                nShift = 0
            End If
        
            nCol = .ColIndex(nShift + QOC_C_BID)
            If nCol >= 0 Then
                If aQuote.PriceBidTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_BID).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_BID).ForeColor
                        
                ElseIf aQuote.PriceBidTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_BID).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_BID).ForeColorAlt2
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_BID).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_BID).ForeColorAlt1
                End If
            End If
            
            nCol = .ColIndex(nShift + QOC_C_ASK)
            If nCol >= 0 Then
                If aQuote.PriceAskTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_ASK).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_ASK).ForeColor
                ElseIf aQuote.PriceAskTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_ASK).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_ASK).ForeColorAlt2
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_ASK).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_ASK).ForeColorAlt1
                End If
            End If
        
            nCol = .ColIndex(nShift + QOC_C_THEO_PRICE)
            If nCol >= 0 Then
                If aQuote.PriceTheo >= 0# Then
                    If aQuote.PriceTheo < aQuote.PriceBid Then
                        If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColorAlt1 Then _
                            .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColorAlt1
                    ElseIf aQuote.PriceAsk >= 0# And aQuote.PriceTheo > aQuote.PriceAsk Then
                        If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColorAlt2 Then _
                            .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColorAlt2
                    Else
                        If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColor Then _
                            .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColor
                    End If
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdOpt.Col(nShift + QOC_C_THEO_PRICE).ForeColor
                End If
            End If
            
            If bShowUpdateTime Then
                nCol = .ColIndex(nShift + QOC_C_UPDATE_TIME)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
            End If
        End If
        
        Set aRowData = Nothing
        Set aOpt = Nothing
        Set aQuote = Nothing

        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    End With
End Sub

Public Sub ModelUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        .TextMatrix(1, QDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If m_Aux.Grp.IsStockOrIndex Then
            .TextMatrix(1, QDC_STYLE) = IIf(m_Aux.Grp.Und.IsAmerican, 1, 0)
        Else
            .TextMatrix(1, QDC_STYLE) = IIf(m_Aux.Grp.Fut.IsAmerican, 1, 0)
        End If
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub TradesFilterUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        .TextMatrix(1, QDC_TRADES) = m_Aux.QV.TradeFilter.Data(enFtTrades)
        .TextMatrix(1, QDC_FLT_TRADER) = m_Aux.QV.TradeFilter.Data(enFtTrader)
        .TextMatrix(1, QDC_FLT_STRATEGY) = m_Aux.QV.TradeFilter.Data(enFtStrategy)
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub


Public Sub CalendarUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
'        If m_Aux.Grp.IsStockOrIndex Then
'            .TextMatrix(1, QDC_CALENDAR) = g_ExpCalendar(m_Aux.Grp.Und.ExpCalendarID).Name
'        Else
'            .TextMatrix(1, QDC_CALENDAR) = g_ExpCalendar(m_Aux.Grp.Fut.ExpCalendarID).Name
'        End If
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub ProfilesUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim nUndID&, nOptID&

    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            nUndID = m_Aux.Grp.Und.UndPriceProfile.ID
            nOptID = m_Aux.Grp.Und.OptPriceProfile.ID
        Else
            nUndID = m_Aux.Grp.Fut.UndPriceProfile.ID
            nOptID = m_Aux.Grp.Fut.OptPriceProfile.ID
        End If
            
        .TextMatrix(1, QDC_UNDPROFILE) = nUndID
        .TextMatrix(1, QDC_OPTPROFILE) = nOptID
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub DivsUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim nDivFreq&, dDivAmt#, dtDivDate As Date
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim dAmount() As Double
    Dim dDate() As Double
    ReDim dAmount(1)
    ReDim dDate(1)
    Dim dtNYTime As Date
        
    dtNYTime = GetNewYorkTime

    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        Set aDiv = m_Aux.Grp.Und.Dividend
        
        If m_Aux.Grp.ContractType = enCtIndex Then
            .ColComboList(QDC_DIV) = "#0;Market|#1;Custom Periodical|#4;Yield %|#3;Stock Basket Div|#2;Custom Stream"
            .TextMatrix(1, QDC_DIV) = "4"
        Else
            .ColComboList(QDC_DIV) = "#0;Market|#1;Custom Periodical|#2;Custom Stream"
            .TextMatrix(1, QDC_DIV) = "0"
        End If
        
        
        If aDiv Is Nothing Then
'            .TextMatrix(1, QDC_DIV) = "0"
        Else
            Select Case aDiv.DivType
                Case enDivCustomPeriodical
                    .TextMatrix(1, QDC_DIV) = "1"
                    dtDivDate = aDiv.DivDateCust
                    nDivFreq = aDiv.DivFreqCust
                    dDivAmt = aDiv.DivAmtCust
            
                Case enDivMarket
                    .TextMatrix(1, QDC_DIV) = "0"
                    dtDivDate = aDiv.DivDate
                    nDivFreq = aDiv.DivFreq
                    dDivAmt = aDiv.DivAmt
            
                Case enDivCustomStream
                    .TextMatrix(1, QDC_DIV) = "2"
                    aDiv.GetNearest2 dtNYTime, CDate("01/01/2999"), CDate("00:00:00"), dAmount(0), dDate(0)
                    nDivFreq = 0
                    dDivAmt = dAmount(0)
                    dtDivDate = dDate(0)
                    
                Case enDivStockBasket
                    .TextMatrix(1, QDC_DIV) = "3"
                    m_Aux.Grp.Und.BasketIndex.BasketDivs.GetNearest2 dtNYTime, CDate("01/01/2999"), CDate("00:00:00"), dAmount(0), dDate(0)
                    nDivFreq = 0
                    dDivAmt = dAmount(0)
                    dtDivDate = dDate(0)
                
                Case enDivIndexYield
                    .TextMatrix(1, QDC_DIV) = "4"
                    nDivFreq = 0
                    dDivAmt = m_Aux.Grp.Und.Yield * 100#
                    dtDivDate = 0
                    
            End Select
        End If
    
        .TextMatrix(1, QDC_FREQ) = nDivFreq
        .TextMatrix(1, QDC_DATE) = Format$(dtDivDate, gdDiv.Col(QDC_DATE).Format)
        .TextMatrix(1, QDC_AMT) = IIf(dDivAmt > BAD_DOUBLE_VALUE, dDivAmt, "")
        
        If m_Aux.DividendsVisible And (m_Aux.Grp.ContractType = enCtStock Or m_Aux.Grp.ContractType = enCtIndex) Then
            If aDiv.DivType = enDivCustomStream Then
                If dtDivDate = 0 Then
                    .ColHidden(QDC_DATE) = True
                    .ColHidden(QDC_AMT) = True
                    .ColHidden(QDC_FREQ) = False
                Else
                    .ColHidden(QDC_DATE) = False
                    .ColHidden(QDC_AMT) = False
                    .ColHidden(QDC_FREQ) = True
                End If
                    gdDiv.Col(QDC_DATE).CanEdit = False
                    gdDiv.Col(QDC_AMT).CanEdit = False
                    gdDiv.Col(QDC_FREQ).CanEdit = False
            End If
                                                
            If aDiv.DivType = enDivCustomPeriodical Then
                If m_Aux.IsValidDivFreq(nDivFreq) Then
                    .ColHidden(QDC_DATE) = False
                    .ColHidden(QDC_AMT) = False
                    .ColHidden(QDC_FREQ) = False
                Else
                    .ColHidden(QDC_DATE) = True
                    .ColHidden(QDC_AMT) = True
                    .ColHidden(QDC_FREQ) = False
                End If
                    gdDiv.Col(QDC_DATE).CanEdit = True
                    gdDiv.Col(QDC_AMT).CanEdit = True
                    gdDiv.Col(QDC_FREQ).CanEdit = True
            End If
                                                
            If aDiv.DivType = enDivMarket Then
                If m_Aux.IsValidDivFreq(nDivFreq) Then
                    .ColHidden(QDC_DATE) = False
                    .ColHidden(QDC_AMT) = False
                    .ColHidden(QDC_FREQ) = False
                Else
                    .ColHidden(QDC_DATE) = True
                    .ColHidden(QDC_AMT) = True
                    .ColHidden(QDC_FREQ) = True
                End If
                    gdDiv.Col(QDC_DATE).CanEdit = False
                    gdDiv.Col(QDC_AMT).CanEdit = False
                    gdDiv.Col(QDC_FREQ).CanEdit = False
            End If
            
            
            If aDiv.DivType = enDivStockBasket Then
                If dtDivDate = 0 Then
                    .ColHidden(QDC_DATE) = True
                    .ColHidden(QDC_AMT) = True
                    .ColHidden(QDC_FREQ) = True
                Else
                    .ColHidden(QDC_DATE) = False
                    .ColHidden(QDC_AMT) = False
                    .ColHidden(QDC_FREQ) = True
                End If
                    gdDiv.Col(QDC_DATE).CanEdit = False
                    gdDiv.Col(QDC_AMT).CanEdit = False
                    gdDiv.Col(QDC_FREQ).CanEdit = False
            End If
            
            If aDiv.DivType = enDivIndexYield Then
                    .ColHidden(QDC_DATE) = True
                    .ColHidden(QDC_AMT) = False
                    .ColHidden(QDC_FREQ) = True
                
                    gdDiv.Col(QDC_DATE).CanEdit = False
                    gdDiv.Col(QDC_AMT).CanEdit = True
                    gdDiv.Col(QDC_FREQ).CanEdit = False
            End If
            
            
        End If

        .AutoSize 0, .Cols - 1

        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub RatesUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim i&, aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl

    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            Set aExpColl = m_Aux.Grp.Und.Exp
        Else
            Set aExpColl = m_Aux.Grp.Fut.Exp
        End If
        
        .Cols = QDC_LAST_COLUMN + aExpColl.Count
        
        .TextMatrix(1, QDC_RATE) = IIf(m_Aux.Grp.UseCustRates, "1", "0")
        
        i = 0
        For Each aExp In aExpColl
            .ColData(i + QDC_RATE_VAL) = aExp
            .ColFormat(i + QDC_RATE_VAL) = gdDiv.Col(QDC_RATE_VAL).Format
            .ColDataType(i + QDC_RATE_VAL) = gdDiv.Col(QDC_RATE_VAL).DataType
            .TextMatrix(0, i + QDC_RATE_VAL) = Format$(aExp.ExpiryMonth, "MMM YY")
            .TextMatrix(1, i + QDC_RATE_VAL) = IIf(m_Aux.Grp.UseCustRates, aExp.RateCust * 100#, aExp.Rate * 100#)
            
            .Cell(flexcpBackColor, 1, i + QDC_RATE_VAL) = IIf(gdDiv.Col(QDC_RATE_VAL).BackColor <> 0, gdDiv.Col(QDC_RATE_VAL).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i + QDC_RATE_VAL) = IIf(gdDiv.Col(QDC_RATE_VAL).ForeColor <> 0, gdDiv.Col(QDC_RATE_VAL).ForeColor, RGB(1, 1, 1))
            i = i + 1
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub VolaUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim i&, aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim dAtmVola As Double
    Dim lSurfaceID As Long

    lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(0)
    With fgVol
        m_Aux.GridLock(GT_QUOTES_VOLA).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            Set aExpColl = m_Aux.Grp.Und.Exp
        Else
            Set aExpColl = m_Aux.Grp.Fut.Exp
        End If
        
        .Cols = QVC_LAST_COLUMN + aExpColl.Count
        
        i = 0
        For Each aExp In aExpColl
            .ColData(i + QVC_VOLA_VAL) = aExp
            .ColFormat(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_VAL).Format
            .ColDataType(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_VAL).DataType
            .TextMatrix(0, i + QVC_VOLA_VAL) = Format$(aExp.ExpiryMonth, "MMM YY")
            dAtmVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, False, lSurfaceID)

            If dAtmVola > BAD_DOUBLE_VALUE Then
                .TextMatrix(1, i + QVC_VOLA_VAL) = dAtmVola * 100#
            Else
                .TextMatrix(1, i + QVC_VOLA_VAL) = STR_NA
            End If
            
            .Cell(flexcpBackColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_VAL).BackColor <> 0, gdVol.Col(QVC_VOLA_VAL).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_VAL).ForeColor <> 0, gdVol.Col(QVC_VOLA_VAL).ForeColor, RGB(1, 1, 1))
            i = i + 1
        Next
        
        .TextMatrix(0, i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_DATECALC).Caption
        .ColFormat(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_DATECALC).Format
        .ColDataType(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_DATECALC).DataType
        
        .Cell(flexcpBackColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_DATECALC).BackColor <> 0, gdVol.Col(QVC_VOLA_DATECALC).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_DATECALC).ForeColor <> 0, gdVol.Col(QVC_VOLA_DATECALC).ForeColor, RGB(1, 1, 1))
        
        If Len(.TextMatrix(1, i + QVC_VOLA_VAL)) = 0 Then .TextMatrix(1, i + QVC_VOLA_VAL) = CStr(Now)
        
        i = i + 1
        
        .TextMatrix(0, i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_SIM).Caption
        .ColFormat(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_SIM).Format
        .ColDataType(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_SIM).DataType
        .ColData(i + QVC_VOLA_VAL) = QVC_VOLA_SIM
        
        .TextMatrix(1, i + QVC_VOLA_VAL) = m_Aux.Grp.Und.VolaSrv.SimulatedVol
        
        .Cell(flexcpBackColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_SIM).BackColor <> 0, gdVol.Col(QVC_VOLA_SIM).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_SIM).ForeColor <> 0, gdVol.Col(QVC_VOLA_SIM).ForeColor, RGB(1, 1, 1))
        
        i = i + 1
        
        .TextMatrix(0, i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_FLAT).Caption
        .ColFormat(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_FLAT).Format
        .ColDataType(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_FLAT).DataType
        .ColData(i + QVC_VOLA_VAL) = QVC_VOLA_FLAT
        
        .TextMatrix(1, i + QVC_VOLA_VAL) = m_Aux.IsSimulatedFlat
        .ColHidden(i + QVC_VOLA_VAL) = Not m_Aux.Grp.Und.VolaSrv.SimulatedVol
        
        .Cell(flexcpBackColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_FLAT).BackColor <> 0, gdVol.Col(QVC_VOLA_FLAT).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_FLAT).ForeColor <> 0, gdVol.Col(QVC_VOLA_FLAT).ForeColor, RGB(1, 1, 1))
        
        Set aExpColl = Nothing
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        .Col = 1
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Sub VolaUpdateValues(Optional ManualEdit As Boolean = True)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim i&, aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim dAtmVola As Double
    Dim lSurfaceID As Long
    
    lSurfaceID = m_Aux.Grp.Und.VolaSrv.GetSurfaceByRoot(0)
    With fgVol
        m_Aux.GridLock(GT_QUOTES_VOLA).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            Set aExpColl = m_Aux.Grp.Und.Exp
        Else
            Set aExpColl = m_Aux.Grp.Fut.Exp
        End If
        
        .Cols = QVC_LAST_COLUMN + aExpColl.Count
        
        i = 0
        For Each aExp In aExpColl
            .ColData(i + QVC_VOLA_VAL) = aExp
             dAtmVola = m_Aux.Grp.Und.AtmVola(aExp, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, ManualEdit, lSurfaceID)

            If dAtmVola > BAD_DOUBLE_VALUE Then
                .TextMatrix(1, i + QVC_VOLA_VAL) = dAtmVola * 100#
            Else
                .TextMatrix(1, i + QVC_VOLA_VAL) = STR_NA
            End If
            i = i + 1
        Next
        Set aExpColl = Nothing
        
        i = i + 1
        .TextMatrix(1, i + QVC_VOLA_VAL) = m_Aux.Grp.Und.VolaSrv.SimulatedVol
        
        i = i + 1
        .TextMatrix(1, i + QVC_VOLA_VAL) = m_Aux.IsSimulatedFlat
        .ColHidden(i + QVC_VOLA_VAL) = Not m_Aux.Grp.Und.VolaSrv.SimulatedVol
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Function GetTotalsString$()
    On Error Resume Next
    
    If m_Aux.Grp.ID <> 0 Then
        If m_Aux.Grp.IsStockOrIndex Then
            With m_Aux.Grp.Und
            
                Dim dNetDelta As Double: dNetDelta = .NetDelta
                Dim dTotalDelta As Double: dTotalDelta = .TotalDelta
                Dim dTotalGamma As Double: dTotalGamma = .TotalGamma
                Dim dTotalVega As Double: dTotalVega = .TotalVega
                Dim dTotalTheta As Double: dTotalTheta = .TotalTheta
                Dim dTotalRho As Double: dTotalRho = .TotalRho
                
                If Not .SynthUnd Is Nothing Then
                    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom
                    
                    For Each aUnd In .SynthUnd
                        If aUnd.NetDelta > BAD_DOUBLE_VALUE Then
                            dNetDelta = IIf(dNetDelta > BAD_DOUBLE_VALUE, dNetDelta, 0) + aUnd.NetDelta
                        End If
                        If aUnd.TotalDelta > BAD_DOUBLE_VALUE Then
                            dTotalDelta = IIf(dTotalDelta > BAD_DOUBLE_VALUE, dTotalDelta, 0) + aUnd.TotalDelta
                        End If
                        If aUnd.TotalGamma > BAD_DOUBLE_VALUE Then
                            dTotalGamma = IIf(dTotalGamma > BAD_DOUBLE_VALUE, dTotalGamma, 0) + aUnd.TotalGamma
                        End If
                        If aUnd.TotalVega > BAD_DOUBLE_VALUE Then
                            dTotalVega = IIf(dTotalVega > BAD_DOUBLE_VALUE, dTotalVega, 0) + aUnd.TotalVega
                        End If
                        If aUnd.TotalTheta > BAD_DOUBLE_VALUE Then
                            dTotalTheta = IIf(dTotalTheta > BAD_DOUBLE_VALUE, dTotalTheta, 0) + aUnd.TotalTheta
                        End If
                        If aUnd.TotalRho > BAD_DOUBLE_VALUE Then
                            dTotalRho = IIf(dTotalRho > BAD_DOUBLE_VALUE, dTotalRho, 0) + aUnd.TotalRho
                        End If
                    Next
                End If
            
            
                GetTotalsString = "'" & .Symbol & "' Totals - " & _
                        "NetDlt: " & Format$(IIf(dNetDelta > BAD_DOUBLE_VALUE, dNetDelta, "N/A"), "#,##0") & "    " & _
                        "OptDlt: " & Format$(IIf(dTotalDelta > BAD_DOUBLE_VALUE, dTotalDelta, "N/A"), "#,##0") & "    " & _
                        "Gma$1: " & Format$(IIf(dTotalGamma > BAD_DOUBLE_VALUE, dTotalGamma, "N/A"), "#,##0") & "    " & _
                        "Vga: " & Format$(IIf(dTotalVega > BAD_DOUBLE_VALUE, dTotalVega, "N/A"), "#,##0") & "    " & _
                        "Tht: " & Format$(IIf(dTotalTheta > BAD_DOUBLE_VALUE, dTotalTheta, "N/A"), "#,##0") & "    " & _
                        "Rho: " & Format$(IIf(dTotalRho > BAD_DOUBLE_VALUE, dTotalRho, "N/A"), "#,##0") & "    "
            End With
        Else
            With m_Aux.Grp.Fut
                GetTotalsString = "'" & .Symbol & "' Totals - " & _
                        "NetDlt: " & Format$(IIf(.NetDelta > BAD_DOUBLE_VALUE, .NetDelta, "N/A"), "#,##0") & "    " & _
                        "OptDlt: " & Format$(IIf(.TotalDelta > BAD_DOUBLE_VALUE, .TotalDelta, "N/A"), "#,##0") & "    " & _
                        "Gma$1: " & Format$(IIf(.TotalGamma > BAD_DOUBLE_VALUE, .TotalGamma, "N/A"), "#,##0") & "    " & _
                        "Vga: " & Format$(IIf(.TotalVega > BAD_DOUBLE_VALUE, .TotalVega, "N/A"), "#,##0") & "    " & _
                        "Tht: " & Format$(IIf(.TotalTheta > BAD_DOUBLE_VALUE, .TotalTheta, "N/A"), "#,##0") & "    " & _
                        "Rho: " & Format$(IIf(.TotalRho > BAD_DOUBLE_VALUE, .TotalRho, "N/A"), "#,##0") & "    "
            End With
        End If
    Else
        GetTotalsString = "Totals - N/A"
    End If
End Function

'Public Sub ShowOptionRows(ByVal bUpdateQuote As Boolean)
'    On Error Resume Next
'    Dim nRow&, nRows&
'    Dim aRowData As MmQvRowData, nOldRow&, bHidden As Boolean, nOptPosExchID&
'
'    With fgOpt
'        m_Aux.GridLock(GT_QUOTES_OPTIONS).LockRedraw
'        nOldRow = .Row
'
'        nRows = .Rows - 1
'        For nRow = 1 To nRows
'            Set aRowData = m_Aux.QV.OptsRowData(nRow)
'
'            If m_Aux.Grp.IsStockOrIndex Then
'                Debug.Assert Not aRowData Is Nothing
'                Debug.Assert Not aRowData.Exp Is Nothing
'                Debug.Assert Not aRowData.Exch Is Nothing
'                Debug.Assert Not aRowData.OptRoot Is Nothing
'                Debug.Assert Not aRowData.Strike Is Nothing
'
'                bHidden = Not aRowData.Exp.Visible Or Not aRowData.Exch.Visible _
'                            Or Not aRowData.OptRoot.Visible Or Not aRowData.Strike.Visible
'
'                nOptPosExchID = m_Aux.Grp.Und.OptPosExchID
'            Else
'                Debug.Assert Not aRowData Is Nothing
'                Debug.Assert Not aRowData.Exp Is Nothing
'                Debug.Assert Not aRowData.Exch Is Nothing
'                Debug.Assert Not aRowData.Strike Is Nothing
'
'                bHidden = Not aRowData.Exp.Visible Or Not aRowData.Exch.Visible Or Not aRowData.Strike.Visible
'                nOptPosExchID = m_Aux.Grp.Fut.OptPosExchID
'            End If
'
'            If bHidden Then
'                If Not aRowData.Opt(enOtCall) Is Nothing Then
'                    bHidden = Not (aRowData.Exch.ID = nOptPosExchID And aRowData.Opt(enOtCall).Qty > BAD_LONG_VALUE)
'                End If
'
'                If bHidden And Not aRowData.Opt(enOtPut) Is Nothing Then
'                    bHidden = Not (aRowData.Exch.ID = nOptPosExchID And aRowData.Opt(enOtPut).Qty > BAD_LONG_VALUE)
'                End If
'            End If
'
'            .RowHidden(nRow) = bHidden
'            If bUpdateQuote And Not bHidden Then
'                OptionUpdateQuote nRow, enOtCall, False, False, True
'                OptionUpdateQuote nRow, enOtPut, False, False, True
'            End If
'
'            Set aRowData = Nothing
'        Next
'
'        If nOldRow > 0 Then
'            If Not .RowHidden(nOldRow) Then
'                .Row = nOldRow
'            Else
'                .Row = .TopRow
'            End If
'        End If
'
'        m_Aux.GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
'    End With
'End Sub

Public Sub ShowUndExchanges()
    On Error Resume Next
    Dim nRow&, nRows&, nOldRow&
    Dim aRowData As MmQvRowData, bHidden As Boolean, nSymCol&
    Dim nNameCol&: nNameCol = -1
    Dim nDpcCol&: nDpcCol = -1
    Dim nFutRootCol&: nFutRootCol = -1
    
    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
        
        nSymCol = .ColIndex(QUC_SYMBOL)
        nNameCol = .ColIndex(QUC_SYMBOL_NAME)
        nDpcCol = .ColIndex(QUC_DPC)
        nFutRootCol = .ColIndex(QUC_FUT_ROOT)
        nOldRow = .Row
        
        nRows = .Rows - 1
        .Cell(flexcpBackColor, 1, nSymCol, nRows, nSymCol) = IIf(gdUnd.Col(QUC_SYMBOL).BackColor <> 0, gdUnd.Col(QUC_SYMBOL).BackColor, RGB(1, 1, 1))
        
        For nRow = 1 To nRows
            Set aRowData = .RowData(nRow)
            
            Debug.Assert Not aRowData Is Nothing
            Debug.Assert Not aRowData.Exch Is Nothing
            
            bHidden = Not aRowData.Exch.Visible
            .RowHidden(nRow) = bHidden
            
            If m_Aux.Grp.IsStockOrIndex Then
                If aRowData.Exch.ID <> aRowData.Und.PosExchID Then _
                    .Cell(flexcpBackColor, nRow, nSymCol, nRow, nSymCol) = .GridColor
                
                .TextMatrix(nRow, nSymCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Und.PosExchID, aRowData.Und.Symbol, "")
                
                If nNameCol >= 0 Then _
                    .TextMatrix(nRow, nNameCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Und.PosExchID, aRowData.Und.SymbolName, "")
            
                If nDpcCol >= 0 Then _
                    .TextMatrix(nRow, nDpcCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Und.PosExchID, aRowData.UndQuote.LotSize, "")
            
                If nFutRootCol >= 0 Then _
                    .TextMatrix(nRow, nFutRootCol) = ""
            
            Else
                If aRowData.Exch.ID <> aRowData.Fut.PosExchID Then _
                    .Cell(flexcpBackColor, nRow, nSymCol, nRow, nSymCol) = .GridColor
                
                .TextMatrix(nRow, nSymCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Fut.PosExchID, aRowData.Fut.Symbol, "")
                
                If nNameCol >= 0 Then _
                    .TextMatrix(nRow, nNameCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Fut.PosExchID, aRowData.Fut.ContractName, "")
            
                If nDpcCol >= 0 Then _
                    .TextMatrix(nRow, nDpcCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Fut.PosExchID, aRowData.FutRoot.FutLotSize, "")
            
                If nFutRootCol >= 0 Then _
                    .TextMatrix(nRow, nFutRootCol) = IIf(aRowData.UndQuote.Exch.ID = aRowData.Fut.PosExchID, aRowData.FutRoot.Symbol, "")
            End If
            
            Set aRowData = Nothing
        Next
        
        If nOldRow > 0 Then
            If Not .RowHidden(nOldRow) Then
                .Row = nOldRow
            Else
                .Row = .TopRow
            End If
        End If
        
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
End Sub




