VERSION 5.00
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Object = "{C932BA88-4374-101B-A56C-00AA003668DC}#1.1#0"; "MSMASK32.OCX"
Begin VB.Form frmSpreadCalculator 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Spread Calculator"
   ClientHeight    =   4305
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   11100
   Icon            =   "frmSpreadCalculator.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4305
   ScaleWidth      =   11100
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrRecalc 
      Enabled         =   0   'False
      Interval        =   20
      Left            =   10680
      Top             =   3360
   End
   Begin VSFlex8Ctl.VSFlexGrid fgSpread 
      Height          =   3255
      Left            =   120
      TabIndex        =   8
      Top             =   120
      Width           =   10815
      _cx             =   19076
      _cy             =   5741
      Appearance      =   2
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   1
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   3
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   -1  'True
      FormatString    =   $"frmSpreadCalculator.frx":6852
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   2
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   1
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   12582911
      ForeColorFrozen =   0
      WallPaperAlignment=   9
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
   End
   Begin VB.CheckBox chkClearSpreadData 
      Caption         =   "Clear Spread Data"
      Height          =   255
      Left            =   6720
      TabIndex        =   4
      Top             =   3480
      Width           =   4215
   End
   Begin VB.Frame Frame1 
      Height          =   855
      Left            =   120
      TabIndex        =   3
      Top             =   3360
      Width           =   6495
      Begin MSMask.MaskEdBox txtQuantity 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "0"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1033
            SubFormatType   =   1
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   9
         Top             =   360
         Width           =   1575
         _ExtentX        =   2778
         _ExtentY        =   556
         _Version        =   393216
         PromptInclude   =   0   'False
         MaxLength       =   5
         Format          =   "#,##0"
         Mask            =   "#####"
         PromptChar      =   " "
      End
      Begin VB.ComboBox cbTrader 
         Height          =   315
         Left            =   1800
         TabIndex        =   7
         Text            =   "Combo1"
         Top             =   360
         Width           =   4575
      End
      Begin VB.Label Label2 
         Caption         =   "Trader"
         Height          =   255
         Left            =   1800
         TabIndex        =   6
         Top             =   120
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "Quantity"
         Height          =   255
         Left            =   120
         TabIndex        =   5
         Top             =   120
         Width           =   975
      End
   End
   Begin VB.CommandButton btnOrders 
      Cancel          =   -1  'True
      Caption         =   "Orders"
      Enabled         =   0   'False
      Height          =   375
      Left            =   6720
      TabIndex        =   2
      Top             =   3840
      Width           =   1335
   End
   Begin VB.CommandButton btnTrades 
      Caption         =   "Trades"
      Height          =   375
      Left            =   8160
      TabIndex        =   1
      Top             =   3840
      Width           =   1335
   End
   Begin VB.CommandButton btnClose 
      Caption         =   "Close"
      Height          =   375
      Left            =   9600
      TabIndex        =   0
      Top             =   3840
      Width           =   1335
   End
End
Attribute VB_Name = "frmSpreadCalculator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private m_nUnderlyingID As Long
Private m_aSpreadColl As EtsMmQuotesLib.MmQvSpreadColl
Private m_aUndQuote As EtsMmQuotesLib.MmQvQuoteAtom


Public Sub Init(aSpreadColl As EtsMmQuotesLib.MmQvSpreadColl, aUndQuote As EtsMmQuotesLib.MmQvQuoteAtom, iUnderlyingID As Long)
 On Error Resume Next
 txtQuantity.Text = CStr(1)
 m_nUnderlyingID = iUnderlyingID
 Set m_aSpreadColl = aSpreadColl
 Set m_aUndQuote = aUndQuote
 FillTraders
 FillSpreadGreed aSpreadColl
 
End Sub

Private Sub FillSpreadGreed(aSpreadColl As EtsMmQuotesLib.MmQvSpreadColl)
 On Error Resume Next
 Dim aSpreadAtom As EtsMmQuotesLib.MmQvSpreadAtom
 Dim aSpreadData As EtsMmQuotesLib.MmQvSpreadData
 Dim iRow As Long
 
 iRow = 2
 fgSpread.Rows = iRow
 fgSpread.Cell(flexcpFontBold, 1, QSV_SERIES, 1, QSV_RHO) = True

 For Each aSpreadAtom In aSpreadColl
    Set aSpreadData = aSpreadAtom.SpreadData
    fgSpread.AddItem "", iRow
     fgSpread.RowData(iRow) = aSpreadData
     FillRow iRow
    iRow = iRow + 1
 Next
 FillSpreadTotals
End Sub

Private Sub FillRow(iRow As Long)
    Dim iColumn As Integer
    Dim aSpreadData As EtsMmQuotesLib.MmQvSpreadData
    
    Set aSpreadData = fgSpread.RowData(iRow)
    If Not aSpreadData Is Nothing Then
        For iColumn = 0 To fgSpread.Cols
           Select Case iColumn
            Case QSV_SERIES
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Series
            Case QSV_QUANTITY
               If aSpreadData.Qty <> BAD_LONG_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = IIf(g_Params.QuoteBidColBuy = 0, aSpreadData.Qty, -aSpreadData.Qty)
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
               If iRow > 1 Then fgSpread.Cell(flexcpBackColor, iRow, iColumn, iRow, iColumn) = RGB(240, 251, 239)
            Case QSV_BID
            
               If aSpreadData.Bid <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Bid
               Else
                    If aSpreadData.Qty < 0 Then
                        fgSpread.TextMatrix(iRow, iColumn) = IIf(aSpreadData.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadData.TheoPrice, "--")
                    Else
                        fgSpread.TextMatrix(iRow, iColumn) = "--"
                    End If
               End If
               
               fgSpread.Cell(flexcpFontBold, iRow, iColumn) = IIf(aSpreadData.Qty < 0, True, False)
               
               If iRow > 1 Then fgSpread.Cell(flexcpBackColor, iRow, iColumn, iRow, iColumn) = RGB(240, 251, 239)
            Case QSV_THEO
               If aSpreadData.TheoPrice <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.TheoPrice
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
            Case QSV_ASK
            
               If aSpreadData.Ask <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Ask
               Else
                    If aSpreadData.Qty > 0 Then
                        fgSpread.TextMatrix(iRow, iColumn) = IIf(aSpreadData.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadData.TheoPrice, "--")
                    Else
                        fgSpread.TextMatrix(iRow, iColumn) = "--"
                    End If
               End If
               
               fgSpread.Cell(flexcpFontBold, iRow, iColumn) = IIf(aSpreadData.Qty > 0, True, False)

               If iRow > 1 Then fgSpread.Cell(flexcpBackColor, iRow, iColumn, iRow, iColumn) = RGB(240, 251, 239)
            Case QSV_DELTA
               If aSpreadData.Delta <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Delta
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
            Case QSV_GAMMA
               If aSpreadData.Gamma <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Gamma
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
            Case QSV_VEGA
               If aSpreadData.Vega <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Vega
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
            Case QSV_THETA
               If aSpreadData.Theta <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Theta
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
            Case QSV_RHO
               If aSpreadData.Rho <> BAD_DOUBLE_VALUE Then
                    fgSpread.TextMatrix(iRow, iColumn) = aSpreadData.Rho
               Else
                    fgSpread.TextMatrix(iRow, iColumn) = "--"
               End If
           End Select
        Next
    End If
End Sub
Private Sub FillSpreadTotals()
 On Error Resume Next
 Dim aSpreadData As EtsMmQuotesLib.MmQvSpreadData
 Dim aSpreadPos As EtsMmQuotesLib.MmQvSpreadData
 Dim iRow As Long
 Dim iSign As Double
  
 dSign = IIf(g_Params.QuoteBidColBuy = 0, 1, -1)

  
 Set aSpreadData = New EtsMmQuotesLib.MmQvSpreadData
 aSpreadData.Series = "Totals"
 If fgSpread.Rows > 2 Then
    For iRow = 3 To fgSpread.Rows
        Set aSpreadPos = fgSpread.RowData(iRow - 1)
        If Not aSpreadPos Is Nothing Then
           
            If aSpreadPos.Qty <> BAD_LONG_VALUE Then
                If aSpreadPos.TheoPrice <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.TheoPrice = BAD_DOUBLE_VALUE Then aSpreadData.TheoPrice = 0
                    aSpreadData.TheoPrice = aSpreadData.TheoPrice + dSign * aSpreadPos.Qty * aSpreadPos.TheoPrice
                End If
                If aSpreadPos.Delta <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.Delta = BAD_DOUBLE_VALUE Then aSpreadData.Delta = 0
                    aSpreadData.Delta = aSpreadData.Delta + dSign * aSpreadPos.Qty * aSpreadPos.Delta
                End If
                If aSpreadPos.Gamma <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.Gamma = BAD_DOUBLE_VALUE Then aSpreadData.Gamma = 0
                    aSpreadData.Gamma = aSpreadData.Gamma + dSign * aSpreadPos.Qty * aSpreadPos.Gamma
                End If
                If aSpreadPos.Vega <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.Vega = BAD_DOUBLE_VALUE Then aSpreadData.Vega = 0
                    aSpreadData.Vega = aSpreadData.Vega + dSign * aSpreadPos.Qty * aSpreadPos.Vega
                End If
                If aSpreadPos.Theta <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.Theta = BAD_DOUBLE_VALUE Then aSpreadData.Theta = 0
                    aSpreadData.Theta = aSpreadData.Theta + dSign * aSpreadPos.Qty * aSpreadPos.Theta
                End If
                If aSpreadPos.Rho <> BAD_DOUBLE_VALUE Then
                    If aSpreadData.Rho = BAD_DOUBLE_VALUE Then aSpreadData.Rho = 0
                    aSpreadData.Rho = aSpreadData.Rho + dSign * aSpreadPos.Qty * aSpreadPos.Rho
                End If
                               
                If aSpreadPos.Qty > 0 Then
                
                    If aSpreadData.Bid = BAD_DOUBLE_VALUE Then aSpreadData.Bid = 0
                    If aSpreadPos.Ask <> BAD_DOUBLE_VALUE Then
                        aSpreadData.Bid = aSpreadData.Bid + dSign * aSpreadPos.Qty * aSpreadPos.Ask
                    Else
                        aSpreadData.Bid = aSpreadData.Bid + dSign * aSpreadPos.Qty * IIf(aSpreadPos.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadPos.TheoPrice, 0)
                    End If
                        
                 End If
                
                If aSpreadPos.Qty < 0 Then
                
                    If aSpreadData.Bid = BAD_DOUBLE_VALUE Then aSpreadData.Bid = 0
                    If aSpreadPos.Bid <> BAD_DOUBLE_VALUE Then
                        aSpreadData.Bid = aSpreadData.Bid + dSign * aSpreadPos.Qty * aSpreadPos.Bid
                    Else
                        aSpreadData.Bid = aSpreadData.Bid + dSign * aSpreadPos.Qty * IIf(aSpreadPos.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadPos.TheoPrice, 0)
                    End If
                    
                End If
            End If
        End If
    Next
 End If
 fgSpread.RowData(1) = aSpreadData
 FillRow 1
End Sub

Private Sub FillTraders()
  On Error Resume Next
  Dim aTrader As EtsGeneralLib.EtsTraderAtom
  Dim i As Integer
  cbTrader.Clear
  i = 0
  If g_Trader.Count > 0 Then
    For Each aTrader In g_Trader
          cbTrader.AddItem aTrader.Name, i
          cbTrader.ItemData(i) = aTrader.ID
          i = i + 1
    Next
    cbTrader.ListIndex = 0
 End If
End Sub
Private Sub btnClose_Click()
On Error Resume Next
    If chkClearSpreadData.Value <> 0 Then
        m_aSpreadColl.Clear
    End If
    Unload Me
End Sub

Private Sub btnTrades_Click()
 On Error Resume Next
 Dim aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom
 Dim iRows As Long
 Dim aSpreadPos As EtsMmQuotesLib.MmQvSpreadData
 Dim bValidContract As Boolean
 Dim nQtyMmultiplier As Long
 Dim cType As Integer
 
 nQtyMmultiplier = CLng(txtQuantity.Text)
 If nQtyMmultiplier <= 0 Then nQtyMmultiplier = 1
 
 If fgSpread.Rows > 2 Then
     For iRow = 3 To fgSpread.Rows
       bValidContract = False
        Set aSpreadPos = fgSpread.RowData(iRow - 1)
        If Not aSpreadPos Is Nothing Then
             Set aNewTrdInfo = New EtsGeneralLib.MmTradeInfoAtom
             If Not aNewTrdInfo Is Nothing Then
                Set aNewTrdInfo.Und = g_UnderlyingAll(m_nUnderlyingID)
                If Not aNewTrdInfo.Und Is Nothing Then
                    aNewTrdInfo.Quantity = Abs(aSpreadPos.Qty) * nQtyMmultiplier
                    aNewTrdInfo.IsBuy = IIf(aSpreadPos.Qty > 0, True, False)
                    aNewTrdInfo.Mark = True
                    aNewTrdInfo.TradeDate = Now
                    aNewTrdInfo.Status = enTrsRealtime
                    
                    'aNewTrdInfo.Price = IIf(aNewTrdInfo.IsBuy, aSpreadPos.Ask, aSpreadPos.Bid)
                    'if there not BID or ASK price we need to use a THEOPRICE
                    If aNewTrdInfo.IsBuy Then
                        If aSpreadPos.Ask <> BAD_DOUBLE_VALUE Then
                            aNewTrdInfo.price = aSpreadPos.Ask
                        Else
                            aNewTrdInfo.price = IIf(aSpreadPos.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadPos.TheoPrice, 0)
                        End If
                    Else
                        If aSpreadPos.Bid <> BAD_DOUBLE_VALUE Then
                            aNewTrdInfo.price = aSpreadPos.Bid
                        Else
                            aNewTrdInfo.price = IIf(aSpreadPos.TheoPrice <> BAD_DOUBLE_VALUE, aSpreadPos.TheoPrice, 0)
                        End If
                    End If
                                      
                    
                    
                    If g_Params.QuoteBidColBuy = 1 Then aNewTrdInfo.IsBuy = Not aNewTrdInfo.IsBuy
                    
                    Set aNewTrdInfo.Trader = g_Trader(cbTrader.ItemData(cbTrader.ListIndex))
                    
                    If aSpreadPos.ContractType = enCtIndex Or aSpreadPos.ContractType = enCtStock Then bValidContract = True
                    If aSpreadPos.ContractType = enCtOption Then

                        Set aNewTrdInfo.OptRoot = aNewTrdInfo.Und.Roots(aSpreadPos.Opt.RootID)
                        If Not aTrd.OptRoot Is Nothing Then
                            bValidContract = True

                            Set aNewTrdInfo.Opt = New EtsGeneralLib.EtsOptAtom
                            
                            aNewTrdInfo.Opt.ID = aSpreadPos.Opt.ID
                            aNewTrdInfo.Opt.Symbol = aSpreadPos.Opt.Symbol
                            aNewTrdInfo.Opt.RootID = aSpreadPos.Opt.RootID
                            aNewTrdInfo.Opt.OptType = aSpreadPos.Opt.OptType
                            aNewTrdInfo.Opt.Expiry = aSpreadPos.Opt.Expiry
                            aNewTrdInfo.Opt.Strike = aSpreadPos.Opt.Strike
                            aNewTrdInfo.Opt.PriceClose = aSpreadPos.Opt.Quote(0).PriceClose
                            aNewTrdInfo.Opt.PriceTheoclose = aSpreadPos.Opt.Quote(0).PriceTheo
                            aNewTrdInfo.Opt.Series = aNewTrdInfo.OptRoot.Name + UCase$(Format$(aNewTrdInfo.Opt.Expiry, "YYMMM")) & Trim$(Str$(Int(aNewTrdInfo.Opt.Strike)))
                            aNewTrdInfo.SpotReference = m_aUndQuote.PriceLast

                            If aSpreadPos.FutID <> BAD_LONG_VALUE Then
                                Set aNewTrdInfo.FutRoot = aNewTrdInfo.Und.FutRoots(aSpreadPos.FutRootID)

                                If Not aNewTrdInfo.FutRoot Is Nothing Then
                                    Set aNewTrdInfo.Fut = aNewTrdInfo.FutRoot.Futures(aSpreadPos.FutID)

'                                    If Not aNewTrdInfo.Fut Is Nothing Then
'                                         Set aTrd.FutOpt = New EtsGeneralLib.EtsFutOptAtom
''                                        aNewTrdInfo.FutOpt.ID = aSpreadPos.Opt.ID
''                                        aNewTrdInfo.FutOpt.Symbol = aSpreadPos.Opt.Symbol
''                                        aNewTrdInfo.FutOpt.FutID = aSpreadPos.FutID
''                                        aNewTrdInfo.FutOpt.OptType = aSpreadPos.Opt.OptType
''                                        aNewTrdInfo.FutOpt.Expiry = aSpreadPos.Opt.Expiry
''                                        aNewTrdInfo.FutOpt.Strike = aSpreadPos.Opt.Strike
''                                        aNewTrdInfo.FutOpt.PriceClose = aSpreadPos.Opt.Quote(0).PriceClose
''                                        aNewTrdInfo.FutOpt.PriceTheoClose = aSpreadPos.Opt.Quote(0).PriceTheo
''                                        aNewTrdInfo.FutOpt.Series = aNewTrdInfo.OptRoot.Name + UCase$(Format$(aNewTrdInfo.Opt.Expiry, "YYMMM")) & Trim$(Str$(Int(aNewTrdInfo.Opt.Strike)))
'                                    End If
                                    
                                End If
                                
                            End If

                        End If
                        If aNewTrdInfo.SpotReference <> BAD_DOUBLE_VALUE Then
                           g_TradeChannel.CalcTradedIV aNewTrdInfo
                        End If
                       
                    ElseIf aSpreadPos.ContractType = enCtFuture Then

                        Set aNewTrdInfo.FutRoot = aNewTrdInfo.Und.FutRoots((aSpreadPos.FutRootID))
                        If Not aTrd.FutRoot Is Nothing Then
                            Set aNewTrdInfo.Fut = aNewTrdInfo.FutRoot.Futures((aSpreadPos.FutID))
                            If Not aNewTrdInfo.Fut Is Nothing Then
                                bValidContract = True
                            End If
                        End If
                        
                    End If
                    If bValidContract Then g_TradeChannel.NewTrade aNewTrdInfo
                
                End If
             End If
             Set aSpreadPos = Nothing
        End If
    Next
 End If
 If chkClearSpreadData.Value <> 0 Then
    m_aSpreadColl.Clear
 End If
 Unload Me
End Sub

Private Sub fgSpread_AfterEdit(ByVal Row As Long, ByVal Col As Long)
 Dim aSpreadData As EtsMmQuotesLib.MmQvSpreadData
 Dim sData As String
    sData = Trim$(fgSpread.TextMatrix(Row, Col))
    Set aSpreadData = fgSpread.RowData(Row)
    If Not aSpreadData Is Nothing Then
        Select Case Col
          Case QSV_QUANTITY
            If aSpreadData.Qty <> CLng(sData) Then
                aSpreadData.Qty = CLng(sData)
                tmrRecalc.Enabled = True
            End If
          Case QSV_BID
            If aSpreadData.Bid <> CDbl(sData) Then
                aSpreadData.Bid = CDbl(sData)
                tmrRecalc.Enabled = True
            End If
          Case QSV_ASK
            If aSpreadData.Ask <> CDbl(sData) Then
                aSpreadData.Ask = CDbl(sData)
                tmrRecalc.Enabled = True
            End If
        End Select
    End If
End Sub

Private Sub fgSpread_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
 On Error Resume Next
 Cancel = True
 If Row > 1 Then
    If Col = QSV_QUANTITY Or Col = QSV_BID Or Col = QSV_ASK Then Cancel = False
 End If

End Sub


Private Sub Form_Initialize()
     chkClearSpreadData.Value = 1

End Sub

Private Sub tmrRecalc_Timer()
    On Error Resume Next
    tmrRecalc.Enabled = False
    FillSpreadTotals
End Sub
