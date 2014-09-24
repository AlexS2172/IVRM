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
Public gdFut As clsGridDef
Public gdOpt As clsGridDef

Public fgUnd As VSFlex7Ctl.VSFlexGrid
Public fgDiv As VSFlex7Ctl.VSFlexGrid
Public fgVol As VSFlex7Ctl.VSFlexGrid
Public fgFut As VSFlex7Ctl.VSFlexGrid
Public fgOpt As VSFlex7Ctl.VSFlexGrid

Public imgBadPrice          As VB.Image
Public imgInSpread          As VB.Image
Public imgInSpreadBadPrice  As VB.Image

Public ExpiryColorSelection As Boolean

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


Public Function UnderlyingUpdate(ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, Optional ManualEdit As Boolean = False) As Long
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

Public Function UnderlyingUpdateQuote(ByVal nRow As Long, ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, Optional ManualEdit As Boolean = False) As Boolean
    On Error Resume Next
    Dim aExch As EtsGeneralLib.ExchAtom, nCol&
    Dim aRowData As MmQvRowData, aUnd As EtsMmQuotesLib.MmQvUndAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom, aFutRoot As EtsMmQuotesLib.MmQvFutRootAtom
    Dim bInSpread As Boolean
    Dim sSpread As MmQvSpreadAtom

    With fgUnd
        m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw

        Set aRowData = .RowData(nRow)
        Set aUnd = aRowData.Und
        Set aQuote = aRowData.UndQuote
        Set aFut = aRowData.Fut
        Set aFutRoot = aRowData.FutRoot
        Set sSpread = m_Aux.Grp.Spread(aUnd.Symbol)

        If Not sSpread Is Nothing Then
            If sSpread.Quote.Exch.ID = aQuote.Exch.ID Then
                bInSpread = True
            Else
                bInSpread = False
            End If
        End If

        UnderlyingUpdateQuote = (Not m_Aux.RealTime Or aQuote.PriceUpdateStatus = enMmQvPusChanged)

        'fokiny aFut => aUnd
'        If Not aUnd Is Nothing And Not aQuote Is Nothing _
'            And (m_Aux.Grp.IsStockOrIndex Or Not m_Aux.Grp.IsStockOrIndex And Not aFutRoot Is Nothing) Then
        If Not aUnd Is Nothing And Not aQuote Is Nothing Then
            Dim i&, nIdx&
            i = 0
            nIdx = gdUnd.Idx(0)
            While nIdx >= 0 And i <= QUC_LAST_COLUMN

                nCol = i + 1
                If bSymbol Then
                    Select Case nIdx
                        Case QUC_SYMBOL
                            'If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aUnd.Symbol, "")
                            'Else
                            '    .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFut.Symbol, "")
                            'End If

                        Case QUC_EXCHANGE
                            .TextMatrix(nRow, nCol) = aQuote.Exch.Code
                        
                        Case QUC_SYMBOL_NAME
                            'If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aUnd.SymbolName, "")
                            'Else
                            '    .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFut.ContractName, "")
                            'End If
                   
                        Case QUC_DPC
                            'If m_Aux.Grp.IsStockOrIndex Then
                            If aUnd.UndType = enCtIndex Or aUnd.UndType = enCtFutUnd Then
                                .TextMatrix(nRow, nCol) = ""
                            Else
                                .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aUnd.PosExchID, aQuote.LotSize, "")
                            End If
                                
                            'Else
                            '    .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFutRoot.FutLotSize, "")
                            'End If
                            
                        Case QUC_FUT_ROOT
                            'If m_Aux.Grp.IsStockOrIndex Then
                                .TextMatrix(nRow, nCol) = ""
                            'Else
                            '    .TextMatrix(nRow, nCol) = IIf(aQuote.Exch.ID = aFut.PosExchID, aFutRoot.Symbol, "")
                            'End If
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
                        
                        If Not bInSpread Then
                            If ManualEdit Then
                                Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            Else
                                If aQuote.ReplacePriceStatus And enRpsBid Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                    .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                Else
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                        Else
                            If sSpread.SpreadType = SPT_SELL Then
                                If ManualEdit Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                                Else
                                    If aQuote.ReplacePriceStatus And enRpsBid Then
                                        Set .Cell(flexcpPicture, nRow, nCol) = imgInSpreadBadPrice
                                        .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                    Else
                                        Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                                    End If
                                End If
                            Else
                                If ManualEdit Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                Else
                                    If aQuote.ReplacePriceStatus And enRpsBid Then
                                        Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                        .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                    Else
                                        Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                    End If
                                End If
                            End If
                        End If

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
                        If Not bInSpread Then
                            If ManualEdit Then
                                Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            Else
                                If aQuote.ReplacePriceStatus And enRpsAsk Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                    .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                Else
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                        Else
                            If sSpread.SpreadType = SPT_BUY Then
                                If ManualEdit Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                                Else
                                    If aQuote.ReplacePriceStatus And enRpsAsk Then
                                        Set .Cell(flexcpPicture, nRow, nCol) = imgInSpreadBadPrice
                                        .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                    Else
                                        Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                                    End If
                                End If
                           Else
                            If ManualEdit Then
                                Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            Else
                                If aQuote.ReplacePriceStatus And enRpsAsk Then
                                    Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                                    .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                                Else
                                    Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                                End If
                            End If
                           End If
                        End If

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
                            If aQuote.Exch.ID = aUnd.PosExchID Then
                                .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = ""
                            End If
                        End If

                    Case QUC_FUT_QTY
                        If m_Aux.Grp.IsStockOrIndex Then
                            .TextMatrix(nRow, nCol) = ""
                        Else
                            If aQuote.Exch.ID = aUnd.PosExchID Then
                                .TextMatrix(nRow, nCol) = IIf(aUnd.Qty > BAD_LONG_VALUE, aUnd.Qty, STR_NA)
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
                    
                    Case QUC_BASIS
                        If m_Aux.Grp.ContractType = enCtIndex Then
                            If Not aUnd.ActiveFuture Is Nothing Then
                                .TextMatrix(nRow, nCol) = IIf(aUnd.ActiveFuture.bAsIs > BAD_DOUBLE_VALUE And aUnd.ActiveFuture.bAsIs <> 0, aUnd.ActiveFuture.bAsIs, STR_NA)
                            Else
                                .TextMatrix(nRow, nCol) = STR_NA
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If
                    Case QUC_INDEXCALCPRICE
                        Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                        .TextMatrix(nRow, nCol) = STR_NA
                        
                        If m_Aux.Grp.ContractType = enCtIndex Or m_Aux.Grp.ContractType = enCtStock Then
                            .TextMatrix(nRow, nCol) = IIf(aUnd.ActivePrice > 0, aUnd.ActivePrice, STR_NA)
                        End If
                        
                        If (aUnd.IsHead) Then
                            If (aUnd.ActiveFuture Is Nothing And aUnd.UseManualActivePrice) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                            End If
                        ElseIf (Not aUnd.PriceByHead) Then
                            If (aUnd.ActiveFuture Is Nothing And aUnd.UseManualActivePrice) Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                            End If
                        End If
                    Case QUC_ACTIVEFUTURE
                        If aUnd.ActiveFuture Is Nothing Then
                            .TextMatrix(nRow, nCol) = 0
                        Else
                            .TextMatrix(nRow, nCol) = aUnd.ActiveFuture.ID
                        End If
                    Case QUC_ACTIVEFUTUREPRICE
                        Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                    
                        If Not aUnd.ActiveFuture Is Nothing Then
                            If aUnd.ActiveFuture.IsUseManualActivePrice Then
                                  Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread
                            End If
                        End If
                        If m_Aux.Grp.ContractType <> enCtIndex Or aUnd.ActiveFuture Is Nothing Or aUnd.ActiveFuture.ActivePrice = BAD_DOUBLE_VALUE Then
                            .TextMatrix(nRow, nCol) = STR_NA
                        Else
                            .TextMatrix(nRow, nCol) = aUnd.ActiveFuture.ActivePrice
                        End If
                    Case QUC_HEAD_COMP
                        .TextMatrix(nRow, nCol) = STR_NA
                                               
                        If (Not m_Aux.Grp.Und.HeadComponent Is Nothing) Then
                            .TextMatrix(nRow, nCol) = m_Aux.Grp.Und.HeadComponent.Symbol
                        ElseIf (m_Aux.Grp.Und.IsHead) Then
                            .TextMatrix(nRow, nCol) = m_Aux.Grp.Und.Symbol
                        End If
                        
                    Case QUC_COEFF
                        .TextMatrix(nRow, nCol) = STR_NA
                        If (Not m_Aux.Grp.Und.HeadComponent Is Nothing Or m_Aux.Grp.Und.IsHead) Then
                            .TextMatrix(nRow, nCol) = m_Aux.Grp.Und.Coeff
                        End If

                        
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
'    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    m_Aux.GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    On Error GoTo Ex

    With fgUnd
        For nRow = 1 To .Rows - 1
    
            Set aRowData = .RowData(nRow)
            Set aUnd = aRowData.Und
            Set aQuote = aRowData.UndQuote
'            Set aFut = aRowData.Fut
        
            'fokiny aFut => aUnd
            If Not aUnd Is Nothing And Not aQuote Is Nothing _
                And (m_Aux.Grp.IsStockOrIndex Or Not m_Aux.Grp.IsStockOrIndex) Then
                
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
        
                            Case QUC_NET_DELTA_EQ
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.NetDeltaEq > BAD_DOUBLE_VALUE, aUnd.NetDeltaEq, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            
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
'                                If aQuote.Exch.ID = aUnd.PosExchID Then
'                                    .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
'                                Else
                                    .TextMatrix(nRow, nCol) = ""
'                                End If
        
                            Case QUC_FUT_QTY
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.Qty > BAD_LONG_VALUE, aUnd.Qty, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
                            Case QUC_NET_DELTA_EQ
                                If aQuote.Exch.ID = aUnd.PosExchID Then
                                    .TextMatrix(nRow, nCol) = IIf(aUnd.NetDeltaEq > BAD_DOUBLE_VALUE, aUnd.NetDeltaEq, STR_NA)
                                Else
                                    .TextMatrix(nRow, nCol) = ""
                                End If
        
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
                    End If
    
                    i = i + 1
                    nIdx = gdUnd.Idx(i)
                Wend
    
            End If
    
        Next
    End With
Ex:
    m_Aux.GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw

    Set aUnd = Nothing
    Set aQuote = Nothing
'    Set aFut = Nothing
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

Public Function FuturesUpdate(ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, ByVal bForceUpdate As Boolean) As Long
    On Error Resume Next
    Dim nRow&
    Dim nQuotes&: nQuotes = fgFut.Rows
    Dim nUndID&, nOptID&

    With fgFut
        m_Aux.GridLock(GT_QUOTES_FUTURES).LockRedraw

        For nRow = 1 To .Rows - 1
            If FutureUpdateQuote(nRow, bSymbol, bUpdateColors, bForceUpdate) Then nQuotes = nQuotes + 1
        Next
        
        m_Aux.GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With

    FuturesUpdate = nQuotes
End Function

Public Function FutureUpdateQuote(ByVal nRow As Long, ByVal bSymbol As Boolean, ByVal bUpdateColors As Boolean, ByVal bForceUpdate As Boolean) As Boolean
    On Error Resume Next
    
    Dim aRowData As MmQvRowData, aQuote As EtsMmQuotesLib.MmQvQuoteAtom
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    'Dim bUpdFut As Boolean
    Dim i&, nIdx&, nColIdx&, nCol&, nShift&, sTemp$
    i = 0
    nIdx = gdFut.Idx(0)
    'bUpdFut = False
    
    With fgFut
        m_Aux.GridLock(GT_QUOTES_FUTURES).LockRedraw

        Set aRowData = .RowData(nRow)
        Set aFut = aRowData.Fut
        Set aQuote = aRowData.FutQuote 'Set aQuote = aFut.Quote(0)
        
        If Not m_Aux.QV.Grp.Und.ActiveFuture Is Nothing Then
            If m_Aux.QV.Grp.Und.ActiveFuture.ID = aFut.ID Then
                .Cell(flexcpFontBold, nRow, 1, nRow, .Cols - 1) = True
            Else
                .Cell(flexcpFontBold, nRow, 1, nRow, .Cols - 1) = False
            End If
        Else
            .Cell(flexcpFontBold, nRow, 1, nRow, .Cols - 1) = False
        End If
        
            'bUpdFut = (bForceUpdate Or Not m_Aux.RealTime Or aQuote.PriceUpdateStatus <> enMmQvPusNone)
            
            'If bUpdFut Then
            While nIdx >= 0 And i <= QOC_LAST_COLUMN

                nCol = i + 1
                If bSymbol Then
                    Select Case nIdx
                        Case QOF_SYMBOL
                            .TextMatrix(nRow, nCol) = aFut.Symbol

                        Case QOF_MONTH
                            .TextMatrix(nRow, nCol) = aFut.MaturityDate
                        
                        Case QOF_FUTCONTRACTSIZE
                            .TextMatrix(nRow, nCol) = aFut.LotSize
                        
                        Case QOF_PRICEUNIT
                            .TextMatrix(nRow, nCol) = aFut.QuotationUnit

                    End Select
                End If
                
                Select Case nIdx
                     Case QOF_ACTIVEPRICE
                            Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                            
                            .TextMatrix(nRow, nCol) = IIf(aFut.ActivePrice > 0, aFut.ActivePrice, STR_NA)
                            If aFut.IsUseManualActivePrice Then
                                Set .Cell(flexcpPicture, nRow, nCol) = imgInSpread.Picture
                            End If
                     Case QOF_BID
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA)
                            If bUpdateColors Then
                                If aQuote.PriceBidTick = 0# Then
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColor Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColor

                                ElseIf aQuote.PriceBidTick < 0# Then
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColorAlt2 Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColorAlt2

                                Else
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColorAlt1 Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColorAlt1
                                End If
                            End If
                            
                        If aQuote.ReplacePriceStatus = enRpsBid Or aQuote.ReplacePriceStatus = enRpsBoth Then
                            .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                        End If
                     Case QOF_ASK
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA)
                            If bUpdateColors Then
                                If aQuote.PriceAskTick = 0# Then
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColor Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColor

                                ElseIf aQuote.PriceAskTick < 0# Then
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColorAlt2 Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColorAlt2

                                Else
                                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(nColIdx).ForeColorAlt1 Then _
                                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(nColIdx).ForeColorAlt1
                                End If
                            End If
                        If aQuote.ReplacePriceStatus = enRpsAsk Or aQuote.ReplacePriceStatus = enRpsBoth Then
                            .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                        End If
                     Case QOF_LAST
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA)
                
                     Case QOF_CHANGE
                         .TextMatrix(nRow, nCol) = IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA)
    
                     Case QOF_CLOSE
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceClose > BAD_DOUBLE_VALUE, aQuote.PriceClose, STR_NA)
                     
                     Case QOF_OPEN
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceOpen > BAD_DOUBLE_VALUE, aQuote.PriceOpen, STR_NA)
                                         
                     Case QOF_HI
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceHigh > BAD_DOUBLE_VALUE, aQuote.PriceHigh, STR_NA)
                         
                     Case QOF_LOW
                         .TextMatrix(nRow, nCol) = IIf(aQuote.PriceLow > BAD_DOUBLE_VALUE, aQuote.PriceLow, STR_NA)
                         
                     Case QOF_ASK_SIZE
                         .TextMatrix(nRow, nCol) = IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA)
                             
                     Case QOF_BID_SIZE
                         .TextMatrix(nRow, nCol) = IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA)
                     
                     Case QOF_VOLUME
                         .TextMatrix(nRow, nCol) = IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA)
                         
                     Case QOF_OPENINTEREST
                         .TextMatrix(nRow, nCol) = IIf(aQuote.OpenInterest > BAD_LONG_VALUE, aQuote.OpenInterest, STR_NA)
                                         
                     Case QOF_UPDTIME
                         .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
                                                  
                     Case QOF_FUTQTY
                         .TextMatrix(nRow, nCol) = IIf(aFut.Qty > BAD_LONG_VALUE, aFut.Qty, STR_NA)
                                                  
                     Case QOF_PRICEFORMAT
                       Select Case aRowData.FutRoot.QuoteFormat
                        Case 2:
                         .TextMatrix(nRow, nCol) = "Pts"   'don't have this value now -- fokiny
                        Case 1:
                         .TextMatrix(nRow, nCol) = "c"   'don't have this value now -- fokiny
                        Case Else
                         .TextMatrix(nRow, nCol) = "$"   'don't have this value now -- fokiny
                       End Select
                         
                     Case QOF_OPT_DELTA
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalDelta > BAD_DOUBLE_VALUE, aFut.TotalDelta, STR_NA)
                     Case QOF_NET_DELTA
                         .TextMatrix(nRow, nCol) = IIf(aFut.NetDelta > BAD_DOUBLE_VALUE, aFut.NetDelta, STR_NA)
                     Case QOF_GAMMA
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalGamma > BAD_DOUBLE_VALUE, aFut.TotalGamma, STR_NA)
                     Case QOF_THETA
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalTheta > BAD_DOUBLE_VALUE, aFut.TotalTheta, STR_NA)
                     Case QOF_VEGA
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalVega > BAD_DOUBLE_VALUE, aFut.TotalVega, STR_NA)
                     Case QOF_RHO
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalRho > BAD_DOUBLE_VALUE, aFut.TotalRho, STR_NA)
                     Case QOF_NET_DELTA_EQ
                         .TextMatrix(nRow, nCol) = IIf(aFut.NetDeltaEq > BAD_DOUBLE_VALUE, aFut.NetDeltaEq, STR_NA)
                     Case QOF_GAMMA_EQ
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalGammaEq > BAD_DOUBLE_VALUE, aFut.TotalGammaEq, STR_NA)
                     Case QOF_OPT_DELTA_EQ
                         .TextMatrix(nRow, nCol) = IIf(aFut.TotalDeltaEq > BAD_DOUBLE_VALUE, aFut.TotalDeltaEq, STR_NA)
                     Case QOF_UND_PROFILE
                         .TextMatrix(nRow, nCol) = aFut.UndPriceProfile.ID
                     Case QOF_OPT_PROFILE
                         .TextMatrix(nRow, nCol) = aFut.OptPriceProfile.ID
                     Case QOF_RATIO
                         If Not m_Aux.QV.Grp.Und.ActiveFuture Is Nothing Then
                              If aFut.Ratio > 0 Then
                                 .TextMatrix(nRow, nCol) = IIf(aFut.Ratio <> 0, aFut.Ratio, STR_NA)
                                 .Cell(flexcpFontItalic, nRow, nCol, nRow, nCol) = True
                              Else
                                ' Calculate ratio on the fly
                                Dim dFutClosePrice As Double
                                Dim dActiveFutclosePrice As Double
                                
                                dFutClosePrice = aFut.Quote(0).PriceClose
                                dActiveFutclosePrice = m_Aux.QV.Grp.Und.ActiveFuture.Quote(0).PriceClose
                                If dFutClosePrice > 0 And dActiveFutclosePrice > 0 Then
                                     .TextMatrix(nRow, nCol) = dFutClosePrice / dActiveFutclosePrice
                                Else
                                     .TextMatrix(nRow, nCol) = 1#
                                End If
                                 .Cell(flexcpFontItalic, nRow, nCol, nRow, nCol) = False
                              End If
                         Else
                             .TextMatrix(nRow, nCol) = STR_NA
                         End If
                     Case QOF_BASIS
                        If m_Aux.QV.Grp.Und.UndType <> enCtFutUnd And Not m_Aux.QV.Grp.Und.ActiveFuture Is Nothing Then
                          .TextMatrix(nRow, nCol) = IIf(aFut.bAsIs <> 0 And aFut.bAsIs > BAD_DOUBLE_VALUE, aFut.bAsIs, STR_NA)
                        Else
                             .TextMatrix(nRow, nCol) = STR_NA
                        End If
                     Case QOF_ACTIVE
                        If Not m_Aux.QV.Grp.Und.ActiveFuture Is Nothing Then
                         .TextMatrix(nRow, nCol) = m_Aux.QV.Grp.Und.ActiveFuture.ID
                        Else
                         .TextMatrix(nRow, nCol) = 0
                        End If
                End Select

                i = i + 1
                nIdx = gdFut.Idx(i)
            Wend
                        
            aQuote.PriceUpdateStatus = enMmQvPusNone

        m_Aux.GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With

    FutureUpdateQuote = True
    
    'fgFut.Refresh 'fokiny

    Set aRowData = Nothing
    Set aFut = Nothing
    Set aQuote = Nothing
    
End Function

Public Sub FuturesUpdateColors()
    On Error Resume Next
    Dim i&, nRows&
    
    With fgFut
        m_Aux.GridLock(GT_QUOTES_FUTURES).LockRedraw
    
        nRows = .Rows - 1
        For i = 1 To nRows
            FutureUpdateColors i
        Next
        
        m_Aux.GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With
End Sub

Public Sub FutureUpdateColors(ByVal nRow As Long, Optional ByVal bShowUpdateTime As Boolean = False)
    On Error Resume Next
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom, aQuote As EtsMmQuotesLib.MmQvQuoteAtom, nCol&, aRowData As MmQvRowData, nShift&
    
    With fgFut
        m_Aux.GridLock(GT_QUOTES_FUTURES).LockRedraw
        
        Set aRowData = .RowData(nRow)
        
        Set aFut = aRowData.Fut
        Set aQuote = aRowData.FutQuote
        
        If Not (aFut Is Nothing Or aQuote Is Nothing) Then
        
            nCol = .ColIndex(QOF_BID)
            If nCol >= 0 Then
                If aQuote.PriceBidTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_BID).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_BID).ForeColor
                        
                ElseIf aQuote.PriceBidTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_BID).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_BID).ForeColorAlt2
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_BID).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_BID).ForeColorAlt1
                End If
            End If
            
            nCol = .ColIndex(QOF_ASK)
            If nCol >= 0 Then
                If aQuote.PriceAskTick = 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_ASK).ForeColor Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_ASK).ForeColor
                ElseIf aQuote.PriceAskTick < 0# Then
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_ASK).ForeColorAlt2 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_ASK).ForeColorAlt2
                Else
                    If .Cell(flexcpForeColor, nRow, nCol) <> gdFut.Col(QOF_ASK).ForeColorAlt1 Then _
                        .Cell(flexcpForeColor, nRow, nCol) = gdFut.Col(QOF_ASK).ForeColorAlt1
                End If
            End If
                                
            If bShowUpdateTime Then
                nCol = .ColIndex(QOF_UPDTIME)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA)
            End If
        End If
        
        Set aRowData = Nothing
        Set aFut = Nothing
        Set aQuote = Nothing

        m_Aux.GridLock(GT_QUOTES_FUTURES).UnlockRedraw
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
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        .TextMatrix(1, QDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If m_Aux.Grp.IsStockOrIndex Then
            .TextMatrix(1, QDC_STYLE) = IIf(m_Aux.Grp.Und.IsAmerican, 1, 0)
        Else
            For Each aFut In m_Aux.Grp.Und.Fut
                .TextMatrix(1, QDC_STYLE) = IIf(aFut.IsAmerican, 1, 0)
                Exit For
            Next
        End If
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub OptionsFilterUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        .TextMatrix(1, QDC_OPTIONS) = m_Aux.OptionsFilter
        
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
'    On Error Resume Next
'    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
'
'    With fgDiv
'        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
'
'        If m_Aux.Grp.IsStockOrIndex Then
'            .TextMatrix(1, QDC_CALENDAR) = g_ExpCalendar(m_Aux.Grp.Und.ExpCalendarID).Name
'        Else
'            .TextMatrix(1, QDC_CALENDAR) = g_ExpCalendar(m_Aux.Grp.Und.ExpCalendarID).Name 'fokiny
'        End If
'
'        .AutoSize 0, .Cols - 1
'
'        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
'    End With
End Sub

Public Sub ProfilesUpdate()
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim nUndID&, nOptID&
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom

    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            nUndID = m_Aux.Grp.Und.UndPriceProfile.ID
            nOptID = m_Aux.Grp.Und.OptPriceProfile.ID
        Else
            nUndID = m_Aux.Grp.Und.UndPriceProfile.ID 'For Each aFut In m_Aux.Grp.Und.Fut fokiny
            nOptID = m_Aux.Grp.Und.OptPriceProfile.ID
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
    
    Dim i&, iCols&, aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom

    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        If m_Aux.Grp.IsStockOrIndex Then
            Set aExpColl = m_Aux.Grp.Und.Exp
            
            .Cols = QDC_LAST_COLUMN + aExpColl.Count
            .TextMatrix(1, QDC_RATE) = IIf(m_Aux.Grp.UseCustRates, "1", "0")
            
            i = 1
            For Each aExp In aExpColl
                .ColData(i + QDC_RATE) = aExp
                .ColFormat(i + QDC_RATE) = gdDiv.Col(QDC_RATE_VAL).Format
                .ColDataType(i + QDC_RATE) = gdDiv.Col(QDC_RATE_VAL).DataType
                .TextMatrix(0, i + QDC_RATE) = Format$(aExp.ExpiryOV + 1#, "MMM,DD YY")
                .TextMatrix(1, i + QDC_RATE) = IIf(m_Aux.Grp.UseCustRates, aExp.RateCust * 100#, aExp.Rate * 100#)
                
                .Cell(flexcpBackColor, 1, i + QDC_RATE) = IIf(gdDiv.Col(QDC_RATE_VAL).BackColor <> 0, gdDiv.Col(QDC_RATE_VAL).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i + QDC_RATE) = IIf(gdDiv.Col(QDC_RATE_VAL).ForeColor <> 0, gdDiv.Col(QDC_RATE_VAL).ForeColor, RGB(1, 1, 1))
                i = i + 1
            Next
            
            .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
            .AutoSize 0, .Cols - 1
        Else
          
            .Cols = QDC_LAST_COLUMN + m_Aux.Grp.ExpAll.Count
            .TextMatrix(1, QDC_RATE) = IIf(m_Aux.Grp.UseCustRates, "1", "0")

            i = 1
            For Each aExp In m_Aux.Grp.ExpAll 'For Each aFut In m_Aux.Grp.Und.Fut
                    .ColData(i + QDC_RATE) = aExp
                    .ColFormat(i + QDC_RATE) = gdDiv.Col(QDC_RATE_VAL).Format
                    .ColDataType(i + QDC_RATE) = gdDiv.Col(QDC_RATE_VAL).DataType
                    .TextMatrix(0, i + QDC_RATE) = Format$(aExp.ExpiryOV + 1#, "MMM,DD YY")
                    .TextMatrix(1, i + QDC_RATE) = IIf(m_Aux.Grp.UseCustRates, aExp.RateCust * 100#, aExp.Rate * 100#)
                    
                    .Cell(flexcpBackColor, 1, i + QDC_RATE) = IIf(gdDiv.Col(QDC_RATE_VAL).BackColor <> 0, gdDiv.Col(QDC_RATE_VAL).BackColor, RGB(1, 1, 1))
                    .Cell(flexcpForeColor, 1, i + QDC_RATE) = IIf(gdDiv.Col(QDC_RATE_VAL).ForeColor <> 0, gdDiv.Col(QDC_RATE_VAL).ForeColor, RGB(1, 1, 1))
                    i = i + 1
            Next
            
            .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
            .AutoSize 0, .Cols - 1
        End If
                
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
    
    UserParamsUpdate
    
End Sub

Public Sub UserParamsUpdate()
On Error Resume Next
    
    Dim lColID As Long
    With fgDiv
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
        
        For lColID = QDC_DATECALC To QDC_RATE
        
            .ColFormat(lColID) = gdDiv.Col(lColID).Format
            .ColDataType(lColID) = gdDiv.Col(lColID).DataType
            .TextMatrix(0, lColID) = gdDiv.Col(lColID).Caption
            'Coloring
            .Cell(flexcpBackColor, 1, lColID) = IIf(gdDiv.Col(lColID).BackColor <> 0, gdDiv.Col(lColID).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, lColID) = IIf(gdDiv.Col(lColID).ForeColor <> 0, gdDiv.Col(lColID).ForeColor, RGB(1, 1, 1))
            
            'Default value
            If (lColID = QDC_DATECALC) Then
                .TextMatrix(1, lColID) = Format(Now)
            ElseIf (lColID = QDC_VOLA_IS_MANUAL) Then
                If Not m_Aux.Grp.Und Is Nothing Then
                    .TextMatrix(1, lColID) = IIf(m_Aux.Grp.Und.IsManualVol = True, "-1", "0")
                End If
            ElseIf (lColID = QDC_VOLA_SIM) Then
                .TextMatrix(1, lColID) = m_Aux.IsVolaSimulated
                .ColData(lColID) = lColID
            ElseIf (lColID = QDC_VOLA_FLAT) Then
                .ColData(lColID) = lColID
                .TextMatrix(1, lColID) = m_Aux.IsSimulatedFlat
                .ColHidden(lColID) = Not m_Aux.IsVolaSimulated
            End If
            
        Next
        m_Aux.GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub VolaUpdate()

    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
       
    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom
    
    Dim aGrp As EtsMmQuotesLib.MmQvGrpAtom
    Dim iColCount&, iExpCount&
    
    Dim i As Long, j As Long
    Dim enPriceStatus As EtsReplacePriceStatusEnum
    Dim bDriverUsed As Boolean

    With fgVol
        m_Aux.GridLock(GT_QUOTES_VOLA).LockRedraw
        
        Set aGrp = m_Aux.Grp
        Set aUnd = aGrp.Und
        
        If (aGrp.IsStockOrIndex) Then
            iExpCount = aUnd.Exp.Count
            iColCount = QVC_LAST_COLUMN + iExpCount * 2 'x2 because of 2 volatility type used
            Set aExpColl = aUnd.Exp
        Else
            iExpCount = aGrp.ExpAll.Count
            iColCount = QVC_LAST_COLUMN + iExpCount * 2 'x2 because of 2 volatility type used
            Set aExpColl = aGrp.ExpAll
        End If
        
        .Cols = iColCount
        
        For i = QVC_LAST_COLUMN To iColCount
            .TextMatrix(0, i) = IIf(i < iExpCount + QVC_LAST_COLUMN, "Implied Volatility", "Theoretical Volatility")
        Next
        
        i = 0
        For Each aExp In aExpColl
                
                'Implied Volatility Coloring and formating
                .ColData(i + QVC_VOLA_VAL) = aExp
                .ColFormat(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_VAL).Format
                .ColDataType(i + QVC_VOLA_VAL) = gdVol.Col(QVC_VOLA_VAL).DataType
                .TextMatrix(1, i + QVC_VOLA_VAL) = Format$(aExp.ExpiryMonth, "MMM,DD YY")
                                               
                'Theoretical Volatility Coloring and formating
                .ColData(i + QVC_VOLA_VAL + iExpCount) = aExp
                .ColFormat(i + QVC_VOLA_VAL + iExpCount) = gdVol.Col(QVC_VOLA_VAL).Format
                .ColDataType(i + QVC_VOLA_VAL + iExpCount) = gdVol.Col(QVC_VOLA_VAL).DataType
                .TextMatrix(1, i + QVC_VOLA_VAL + iExpCount) = Format$(aExp.ExpiryMonth, "MMM,DD YY")
                
                For j = 2 To 7
                    .Cell(flexcpBackColor, j, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_VAL).BackColor <> 0, gdVol.Col(QVC_VOLA_VAL).BackColor, RGB(1, 1, 1))
                    .Cell(flexcpForeColor, j, i + QVC_VOLA_VAL) = IIf(gdVol.Col(QVC_VOLA_VAL).ForeColor <> 0, gdVol.Col(QVC_VOLA_VAL).ForeColor, RGB(1, 1, 1))
                    .Cell(flexcpBackColor, j, i + QVC_VOLA_VAL + iExpCount) = IIf(gdVol.Col(QVC_VOLA_VAL).BackColor <> 0, gdVol.Col(QVC_VOLA_VAL).BackColor, RGB(1, 1, 1))
                    .Cell(flexcpForeColor, j, i + QVC_VOLA_VAL + iExpCount) = IIf(gdVol.Col(QVC_VOLA_VAL).ForeColor <> 0, gdVol.Col(QVC_VOLA_VAL).ForeColor, RGB(1, 1, 1))
                Next
                
                i = i + 1
        Next
                       
        Set aExpColl = Nothing
        Set aExp = Nothing
        Set aGrp = Nothing
        Set aUnd = Nothing
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        .Col = 1
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
    VolaUpdateValues
End Sub

Public Sub VolaUpdateValues(Optional ManualEdit As Boolean = False)
    On Error Resume Next
    If m_bShutDown Or m_Aux.Grp Is Nothing Then Exit Sub
    
    Dim iExp&, aExp As EtsMmQuotesLib.MmQvExpAtom, aExpColl As EtsMmQuotesLib.MmQvExpColl
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim aFutCalc As EtsMmQuotesLib.MmQvFutAtom, aExpOpt As EtsMmQuotesLib.MmQvExpAtom
    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom, aFutExp As EtsMmQuotesLib.MmQvExpAtom
    Dim aSearchExp As EtsMmQuotesLib.MmQvExpAtom
    
    Dim aGrp As EtsMmQuotesLib.MmQvGrpAtom
    Dim iColCount&, iExpCount&
    
    Dim dUndPrice As Double, dStrike As Double, dVola As Double
    Dim iCol As Long, iRow As Long, i As Long
    Dim enPriceStatus As EtsReplacePriceStatusEnum
    Dim bDriverUsed As Boolean
    Dim sCaption As String
    Dim lSurfaceID As Long
    
    Dim dMinLeft As Double, dMinRight As Double, dBuffVola As Double
    Dim aLExp As EtsMmQuotesLib.MmQvExpAtom, aRExp As EtsMmQuotesLib.MmQvExpAtom
    
    With fgVol
        m_Aux.GridLock(GT_QUOTES_VOLA).LockRedraw
        
        
        Set aGrp = m_Aux.Grp
        Set aUnd = aGrp.Und
        
        If (aGrp.IsStockOrIndex) Then
            iExpCount = aUnd.Exp.Count
            iColCount = QVC_LAST_COLUMN + iExpCount * 2 'x2 because of 2 volatility type used
            Set aExpColl = aUnd.Exp
        Else
            iExpCount = aGrp.ExpAll.Count
            iColCount = QVC_LAST_COLUMN + iExpCount * 2 'x2 because of 2 volatility type used
            Set aExpColl = aGrp.ExpAll
        End If
        
        .Cols = iColCount
            
        dUndPrice = aUnd.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enPriceStatus, bDriverUsed)
        
        If m_Aux.Grp.IsStockOrIndex Then
                                      
            'Get ATM strike
            'dUndPrice = aUnd.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enPriceStatus, bDriverUsed)
            'Calc volatility
            '-20% -10% 0% 10% 20%
            iRow = 2
            For i = -2 To 2
                iCol = 1
                dStrike = dUndPrice * (1# - (CDbl(i) * 0.1))
                
                If (dStrike > 0#) Then
                    If (i <> 0) Then
                        sCaption = IIf(i < 0, "+", "") + CStr((-i) * 10) + "%"
                    Else
                        sCaption = "ATM"
                    End If
                    sCaption = sCaption + CStr("(") + Format(dStrike, "####.##") + CStr(")")
                    .TextMatrix(iRow, 0) = sCaption
                End If
                
                For Each aExp In aExpColl
                
                    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(0)
                    'Get Theoretical Vola
                    dVola = aUnd.VolaSrv.OptionVola(aExp.ExpiryOV, dStrike, lSurfaceID)
                    .TextMatrix(iRow, iCol + iExpCount) = IIf(dStrike > 0#, dVola * 100#, STR_NA)
                    
                    'Get Implied Vola
                    dVola = aExp.Vola(dStrike, dUndPrice)
                    
                    If (dVola < 0) Then 'Interpolate
                    
                        dMinLeft = 10000000#
                        dMinRight = 10000000#
                        Dim dDiff As Double
                        dDiff = 0#
                        Set aLExp = aExp
                        Set aRExp = aExp
                        'Search boundary expiry's
                        For Each aSearchExp In aExpColl
                            If (aSearchExp.ExpiryOV <> aExp.ExpiryOV) Then
                                dDiff = CDbl(aSearchExp.ExpiryOV - aExp.ExpiryOV)
                                If (dDiff < 0#) Then
                                    If (dMinLeft > Abs(dDiff)) Then
                                        dMinLeft = Abs(dDiff)
                                        Set aLExp = aSearchExp
                                    End If
                                Else
                                    If (dMinRight > dDiff) Then
                                        dMinRight = dDiff
                                        Set aRExp = aSearchExp
                                    End If
                                End If
                            End If
                        Next
                        
                        'Calc vola
                        Dim dLeftVola As Double, dRightVola As Double
                        If (aLExp.ExpiryOV <> aExp.ExpiryOV And aRExp.ExpiryOV <> aExp.ExpiryOV) Then
                            dLeftVola = aLExp.Vola(dStrike, dUndPrice)
                            dRightVola = aRExp.Vola(dStrike, dUndPrice)
                            dVola = ((Sqr(aExp.ExpiryOV) - Sqr(aLExp.ExpiryOV)) / (Sqr(aRExp.ExpiryOV) - Sqr(aLExp.ExpiryOV))) * (dRightVola - dLeftVola) + dLeftVola
                        ElseIf (aLExp.ExpiryOV = aExp.ExpiryOV) Then
                            dVola = aRExp.Vola(dStrike, dUndPrice)
                        ElseIf (aRExp.ExpiryOV = aExp.ExpiryOV) Then
                            dVola = aLExp.Vola(dStrike, dUndPrice)
                        End If
                        
                    End If
                    .TextMatrix(iRow, iCol) = IIf(dStrike > 0#, IIf(dVola > 0#, dVola * 100#, STR_NA), STR_NA)
                    
                    iCol = iCol + 1
                Next
                iRow = iRow + 1
            Next

        Else
            iCol = 1
            For Each aExpOpt In aExpColl
            
                'get Nearest future and expiration for vola calculation
                iExp = 0
                For Each aFut In m_Aux.Grp.Und.Fut
                    For Each aExp In aFut.Exp
                        If aExp.ExpiryMonth = aExpOpt.ExpiryMonth Then
                            iExp = iExp + 1
                            If iExp >= 2 Then
                                If aFut.MaturityDate < aFutCalc.MaturityDate Then
                                    Set aFutCalc = aFut 'aFutCalc - future for vola calculation
                                    'Set aFutExp = aExp
                                End If
                            End If
                            If iExp = 1 Then
                                Set aFutCalc = aFut
                                Set aFutExp = aExp
                            End If
                        End If
                    Next
                Next
                              
                'Get ATM Strike
                'dUndPrice = aFutCalc.GetFuturePrice(aUnd, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enPriceStatus, bDriverUsed)
                              
                'Calc volatility -20% -10% 0% 10% 20%
                iRow = 2
                For i = -2 To 2
                    dStrike = dUndPrice * (1# - (CDbl(i) * 0.1)) 'calc Strike
                    
                    If (dStrike > 0#) Then
                        If (i <> 0) Then
                            sCaption = IIf(i < 0, "+", "") + CStr((-i) * 10) + "%"
                        Else
                            sCaption = "ATM"
                        End If
                        sCaption = sCaption + CStr("(") + Format(dStrike, "####.##") + CStr(")")
                        .TextMatrix(iRow, 0) = sCaption
                    End If
                    
                    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(0)
                    If (Not aFutCalc Is Nothing) Then
                        lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aFutCalc.FutRootID)
                    End If
                    'Get Theoretical Vola
                    dVola = aUnd.VolaSrv.OptionVola(aExpOpt.ExpiryOV, dStrike, lSurfaceID)
                    .TextMatrix(iRow, iCol + iExpCount) = IIf(dStrike > 0#, dVola * 100#, STR_NA)
                    dBuffVola = dVola
                    
                    'Get Implied Vola
                    dVola = aFutExp.Vola(dStrike, dUndPrice)
                    
                    If (dVola < 0#) Then dVola = dBuffVola
                    .TextMatrix(iRow, iCol) = IIf(dStrike > 0#, IIf(dVola > 0#, dVola * 100#, STR_NA), STR_NA)
                                        
                    iRow = iRow + 1
                Next
                iCol = iCol + 1
            Next
            '--------------------------------------------------------------------
        End If
        
        
        Set aExpColl = Nothing
        Set aFut = Nothing
        Set aFutCalc = Nothing
        Set aFutExp = Nothing
        
        .AutoSize 0, .Cols - 1
        
        m_Aux.GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Function GetTotalsString$()
    On Error Resume Next
    Dim sSpreadData As String
    Dim aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim NetDeltaAll#, TotalDeltaAll#, TotalGammaAll#, TotalVegaAll#, TotalThetaAll#, TotalRhoAll#
    NetDeltaAll = 0#: TotalDeltaAll = 0#: TotalGammaAll = 0#: TotalVegaAll = 0#: TotalThetaAll = 0#: TotalRhoAll# = 0#
    
    If m_Aux.Grp.Spread.Count > 0 Then
        sSpreadData = "Spread [$" & Format$(IIf(m_Aux.Grp.Spread.TotalPrice > BAD_DOUBLE_VALUE, m_Aux.Grp.Spread.TotalPrice, "N/A"), "#,##0.##") & "   " & _
        "Dlt:" & Format$(IIf(m_Aux.Grp.Spread.TotalDelta > BAD_DOUBLE_VALUE, m_Aux.Grp.Spread.TotalDelta, "N/A"), "#,##0.##") & "   " & _
        "Vga:" & Format$(IIf(m_Aux.Grp.Spread.TotalVega > BAD_DOUBLE_VALUE, m_Aux.Grp.Spread.TotalVega, "N/A"), "#,##0.##") & "   " & _
        "Leg:" & m_Aux.Grp.Spread.Count & _
        "]"

    Else
        sSpreadData = "Spread [N/A]"
    End If
    
    If m_Aux.Grp.ID <> 0 Then
        If m_Aux.Grp.IsStockOrIndex Then
            With m_Aux.Grp.Und
                GetTotalsString = "'" & .Symbol & "' Totals - " & _
                        "NetDlt: " & Format$(IIf(.NetDelta > BAD_DOUBLE_VALUE, .NetDelta, "N/A"), "#,##0") & "    " & _
                        "OptDlt: " & Format$(IIf(.TotalDelta > BAD_DOUBLE_VALUE, .TotalDelta, "N/A"), "#,##0") & "    " & _
                        "Gma$1: " & Format$(IIf(.TotalGamma > BAD_DOUBLE_VALUE, .TotalGamma, "N/A"), "#,##0") & "    " & _
                        "Vga: " & Format$(IIf(.TotalVega > BAD_DOUBLE_VALUE, .TotalVega, "N/A"), "#,##0") & "    " & _
                        "Tht: " & Format$(IIf(.TotalTheta > BAD_DOUBLE_VALUE, .TotalTheta, "N/A"), "#,##0") & "    " & _
                        "Rho: " & Format$(IIf(.TotalRho > BAD_DOUBLE_VALUE, .TotalRho, "N/A"), "#,##0") & "    " & _
                        sSpreadData & "    " & _
                        "Opt: " & (m_Aux.QV.OptQuotesCount + m_Aux.QV.FutQuotesCount) & "    "
            End With
        Else
            For Each aFut In m_Aux.Grp.Und.Fut
                With aFut
                    NetDeltaAll = NetDeltaAll + IIf(.NetDelta > BAD_DOUBLE_VALUE, .NetDelta, 0#)
                    TotalDeltaAll = TotalDeltaAll + IIf(.TotalDelta > BAD_DOUBLE_VALUE, .TotalDelta, 0#)
                    TotalGammaAll = TotalGammaAll + IIf(.TotalGamma > BAD_DOUBLE_VALUE, .TotalGamma, 0#)
                    TotalVegaAll = TotalVegaAll + IIf(.TotalVega > BAD_DOUBLE_VALUE, .TotalVega, 0#)
                    TotalThetaAll = TotalThetaAll + IIf(.TotalTheta > BAD_DOUBLE_VALUE, .TotalTheta, 0#)
                    TotalRhoAll = TotalRhoAll + IIf(.TotalRho > BAD_DOUBLE_VALUE, .TotalRho, 0#)
                End With
            Next
            GetTotalsString = "'" & m_Aux.Grp.Und.Symbol & "' Totals - " & _
                    "NetDlt: " & Format$(IIf(NetDeltaAll > BAD_DOUBLE_VALUE, NetDeltaAll, "N/A"), "#,##0") & "    " & _
                    "OptDlt: " & Format$(IIf(TotalDeltaAll > BAD_DOUBLE_VALUE, TotalDeltaAll, "N/A"), "#,##0") & "    " & _
                    "Gma$1: " & Format$(IIf(TotalGammaAll > BAD_DOUBLE_VALUE, TotalGammaAll, "N/A"), "#,##0") & "    " & _
                    "Vga: " & Format$(IIf(TotalVegaAll > BAD_DOUBLE_VALUE, TotalVegaAll, "N/A"), "#,##0") & "    " & _
                    "Tht: " & Format$(IIf(TotalThetaAll > BAD_DOUBLE_VALUE, TotalThetaAll, "N/A"), "#,##0") & "    " & _
                    "Rho: " & Format$(IIf(TotalRhoAll > BAD_DOUBLE_VALUE, TotalRhoAll, "N/A"), "#,##0") & "    " & _
                    sSpreadData & "    " & _
                    "Opt: " & (m_Aux.QV.OptQuotesCount + m_Aux.QV.FutQuotesCount) & "    "
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

Public Sub UpdateOptionsSpread(ByRef aImg As Image)
Dim pSpread As MmQvSpreadAtom
On Error Resume Next
    For Each pSpread In m_Aux.Grp.Spread
        UpdateOptionsSpreadInfo pSpread, aImg
    Next
End Sub
Public Sub UpdateFuturesSpreadInfo(ByRef pSpread As MmQvSpreadAtom, ByRef aImg As Image, ByVal dmRow As Long)
    On Error Resume Next
    Dim dRow As Long
    Dim iCol As Long
    
    dRow = dmRow
    If dRow <> BAD_LONG_VALUE Then
        Select Case pSpread.SpreadType
            Case SPT_BUY
                For iCol = 0 To QOF_LAST_COLUMN
                    If gdFut.Idx(iCol) = QOF_ASK Then
                       Set fgFut.Cell(flexcpPicture, dRow, iCol + 1) = aImg
                    End If
                Next
            Case SPT_SELL
                For iCol = 0 To QOF_LAST_COLUMN
                    If gdFut.Idx(iCol) = QOF_BID Then
                       Set fgFut.Cell(flexcpPicture, dRow, iCol + 1) = aImg
                    End If
                Next
        End Select
    End If
    
End Sub
Public Sub ClearFutGrid()
    On Error Resume Next
        Dim nColBid As Long
        Dim nColAsk As Long
        Dim iCol As Long
        Dim iRow As Long
        
        For iCol = 0 To QOF_LAST_COLUMN
            
            If gdFut.Idx(iCol) = QOF_ASK Then
                nColAsk = iCol
            End If
            If gdFut.Idx(iCol) = QOF_BID Then
                nColBid = iCol
            End If
        Next
        For iRow = 0 To fgFut.Rows - 1
            Set fgFut.Cell(flexcpPicture, iRow, nColBid + 1) = Nothing
            Set fgFut.Cell(flexcpPicture, iRow, nColAsk + 1) = Nothing
        Next
End Sub
Public Sub UpdateOptionsSpreadInfo(ByRef pSpread As MmQvSpreadAtom, ByRef aImg As Image)
    On Error Resume Next
    Dim dRow As Long
    Dim iCol As Long
    
    dRow = pSpread.OptionItemPositiion
    If dRow <> BAD_LONG_VALUE Then
    ' options
    Select Case pSpread.SpreadType
        Case SPT_BUY
            If pSpread.Opt.OptType = enOtCall Then
                For iCol = 0 To QOC_LAST_COLUMN
                    If gdOpt.Idx(iCol) = QOC_C_ASK Then
                           Set fgOpt.Cell(flexcpPicture, dRow, iCol + 2) = aImg
                    End If
                Next
                
            Else
                For iCol = 0 To QOC_LAST_COLUMN
                    If gdOpt.Idx(iCol) = QOC_P_ASK Then
                           Set fgOpt.Cell(flexcpPicture, dRow, iCol + 2) = aImg
                    End If
                Next
            End If
        Case SPT_SELL
            If pSpread.Opt.OptType = enOtCall Then
                For iCol = 0 To QOC_LAST_COLUMN
                    If gdOpt.Idx(iCol) = QOC_C_BID Then
                           Set fgOpt.Cell(flexcpPicture, dRow, iCol + 2) = aImg
                    End If
                Next
                
            Else
                For iCol = 0 To QOC_LAST_COLUMN
                    If gdOpt.Idx(iCol) = QOC_P_BID Then
                           Set fgOpt.Cell(flexcpPicture, dRow, iCol + 2) = aImg
                    End If
                Next
            End If
    End Select
    
    End If

End Sub

