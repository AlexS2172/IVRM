VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Quote Entry System for ISE"
   ClientHeight    =   7320
   ClientLeft      =   1350
   ClientTop       =   1860
   ClientWidth     =   12960
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7320
   ScaleWidth      =   12960
   Begin VB.Timer timerOnline 
      Interval        =   10000
      Left            =   10080
      Top             =   0
   End
   Begin VB.CheckBox chkRealtime 
      Caption         =   "Realtime"
      Height          =   255
      Left            =   7680
      TabIndex        =   15
      Top             =   120
      Width           =   1095
   End
   Begin VB.CommandButton cmdDeleteAll 
      Caption         =   "Delete All"
      Height          =   375
      Left            =   11040
      TabIndex        =   14
      Top             =   5040
      Width           =   1815
   End
   Begin VB.CommandButton cmdEnterOrder 
      Caption         =   "Submit order..."
      Height          =   375
      Left            =   11040
      TabIndex        =   13
      Top             =   1920
      Width           =   1815
   End
   Begin VB.CommandButton cmdAddSeries 
      Caption         =   "Add series"
      Height          =   315
      Left            =   5760
      TabIndex        =   12
      Top             =   60
      Width           =   1575
   End
   Begin VB.CommandButton cmdStressSubmit 
      Caption         =   "Stress submit quote(s)"
      Height          =   375
      Left            =   11040
      TabIndex        =   11
      Top             =   4080
      Width           =   1815
   End
   Begin VB.CommandButton cmdDelete 
      Caption         =   "Delete quote(s)"
      Height          =   375
      Left            =   11040
      TabIndex        =   10
      Top             =   4560
      Width           =   1815
   End
   Begin VB.TextBox edLog 
      Height          =   1575
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   9
      Top             =   5640
      Width           =   12735
   End
   Begin VB.CommandButton cmdSubmit 
      Caption         =   "Submit quote(s)"
      Height          =   1575
      Left            =   11040
      TabIndex        =   7
      Top             =   2400
      Width           =   1815
   End
   Begin MSComctlLib.ListView lstSeries 
      CausesValidation=   0   'False
      Height          =   4935
      Left            =   120
      TabIndex        =   6
      Top             =   480
      Width           =   10815
      _ExtentX        =   19076
      _ExtentY        =   8705
      View            =   3
      LabelEdit       =   1
      LabelWrap       =   -1  'True
      HideSelection   =   -1  'True
      Checkboxes      =   -1  'True
      FullRowSelect   =   -1  'True
      GridLines       =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   0
      NumItems        =   0
   End
   Begin VB.CommandButton cmdListSeries 
      Appearance      =   0  'Flat
      Caption         =   "List series"
      Height          =   315
      Left            =   4080
      MaskColor       =   &H00E0E0E0&
      TabIndex        =   5
      Top             =   60
      Width           =   1455
   End
   Begin VB.ComboBox cboUnderlyings 
      Height          =   315
      ItemData        =   "QES.frx":0000
      Left            =   1560
      List            =   "QES.frx":0002
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   75
      Width           =   2415
   End
   Begin VB.CommandButton cmdSetMMParameters 
      Caption         =   "Set MM parameters..."
      Height          =   375
      Left            =   11040
      TabIndex        =   2
      Top             =   1440
      Width           =   1815
   End
   Begin VB.CommandButton cmdDisconnect 
      Caption         =   "Disconnect"
      Enabled         =   0   'False
      Height          =   375
      Left            =   11040
      TabIndex        =   1
      Top             =   600
      Width           =   1815
   End
   Begin VB.CommandButton cmdConnect 
      Caption         =   "Connect"
      Height          =   375
      Left            =   11040
      TabIndex        =   0
      Top             =   120
      Width           =   1815
   End
   Begin VB.Label lblOrderbook 
      Alignment       =   2  'Center
      BackStyle       =   0  'Transparent
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   9700
      TabIndex        =   17
      Top             =   150
      Width           =   255
   End
   Begin VB.Label lblOnLine 
      Caption         =   "On line"
      Height          =   255
      Left            =   9120
      TabIndex        =   16
      Top             =   120
      Width           =   495
   End
   Begin VB.Shape shpOnline 
      BorderColor     =   &H8000000F&
      FillColor       =   &H00808080&
      FillStyle       =   0  'Solid
      Height          =   255
      Left            =   9720
      Shape           =   3  'Circle
      Top             =   120
      Width           =   255
   End
   Begin VB.Label Label2 
      Caption         =   "Log window :"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   5400
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Underlying symbol :"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   1575
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public objMMParams As New ISEPROVIDERLib.MMParameters
Public WithEvents objMMManager As ISEPROVIDERLib.ISEMMManager
Attribute objMMManager.VB_VarHelpID = -1
Public WithEvents objMarketStructureProv As ISEPROVIDERLib.ISEStructureProvider
Attribute objMarketStructureProv.VB_VarHelpID = -1
Public WithEvents objPriceProv As ISEPROVIDERLib.ISEPriceProvider
Attribute objPriceProv.VB_VarHelpID = -1
Public objGroupPrice As ISEPROVIDERLib.IGroupPrice
Public WithEvents objISETracer As ISEPROVIDERLib.ISETracer
Attribute objISETracer.VB_VarHelpID = -1

Private Declare Function LockWindowUpdate Lib "user32" (ByVal hwndLock As Long) As Long
Private Declare Function GdiSetBatchLimit Lib "gdi32" (ByVal dwLimit As Long) As Long
Private Declare Function GdiGetBatchLimit Lib "gdi32" () As Long

Private colUnderlyings As New Collection
Public colQuoteInfo As New Collection
Private sCurrentUnderlying As String

Private Sub UpdateOnConnect()
    cmdDisconnect.Enabled = True
    cmdConnect.Enabled = False
End Sub

Private Sub GetAllUnderlyings()
    Dim Stocks As ISEPROVIDERLib.StockParams

    objMarketStructureProv.RequestAllStocks
End Sub

Private Sub UpdateOnDisconnect()
    cmdDisconnect.Enabled = False
    cmdConnect.Enabled = True
End Sub



Private Sub cmdAddSeries_Click()
    ListSeries False
End Sub

Private Sub cmdConnect_Click()
On Error GoTo ErrHandler

Dim Stocks As ISEPROVIDERLib.StockParams
    
    objMMManager.Connect
    objMarketStructureProv.Connect
    objPriceProv.Connect
    
    UpdateOnConnect
    cboUnderlyings.Clear
    Set colUnderlyings = Nothing
    Set colUnderlyings = New Collection
    
'    GetAllUnderlyings
    Exit Sub
    
ErrHandler:
    frmError.labText = "Failed to establish connection to ISE : " & Error
    frmError.Show vbModal
End Sub

Private Sub cmdDelete_Click()
    SendQuotes False
End Sub

Private Sub cmdDeleteAll_Click()
    For iIndex = 1 To lstSeries.ListItems.Count
        lstSeries.ListItems(iIndex).SubItems(3) = 0
        lstSeries.ListItems(iIndex).SubItems(4) = 0
        lstSeries.ListItems(iIndex).SubItems(5) = 0
        lstSeries.ListItems(iIndex).SubItems(10) = 0
        lstSeries.ListItems(iIndex).SubItems(11) = 0
        lstSeries.ListItems(iIndex).SubItems(12) = 0
        lstSeries.ListItems(iIndex).Checked = True
    Next
    
    SendQuotes True
End Sub

Private Sub cmdDisconnect_Click()
On Error GoTo ErrHandler
    
    objMMManager.Disconnect
    objMarketStructureProv.Disconnect
    objPriceProv.Disconnect
    
    UpdateOnDisconnect
    Exit Sub
ErrHandler:
    frmError.labText = Error
    frmError.Show vbModal
End Sub

Private Sub cmdEnterOrder_Click()
    frmEnterOrder.Show vbModal
End Sub

Private Sub cmdListSeries_Click()
    ListSeries True
End Sub

Private Sub ListSeries(bClear As Boolean)
Dim Quote As QuoteUpdateParams
Dim iItem As Integer
Dim lstItem As ListItem

On Error GoTo ErrHandler
    If cboUnderlyings.Text = "" Then
        Exit Sub
    End If
    
    Dim Options As ISEPROVIDERLib.OptionParams
    
    If bClear Then
        ' if was given market structure for prev underlying symbol then clear it
        If sCurrentUnderlying <> "" Then
            ' cancel und
            Quote.Exchange = ""
            Quote.Type = enSTK
            Quote.Symbol = sCurrentUnderlying
            objPriceProv.CancelLastQuote Quote
            objPriceProv.UnSubscribeQuote Quote ''
            
            ' cancel group
            objGroupPrice.CancelGroup
            objMMManager.CancelLastMmQuote
            
            ' cancel ms
            Options.Exchange = ""
            Options.UnderlyingSymbol = sCurrentUnderlying
            objMarketStructureProv.CancelOptions Options
        End If
    
        lstSeries.ListItems.Clear
    End If
    
    ' get ms
    sCurrentUnderlying = cboUnderlyings.Text
    Options.Exchange = ""
    Options.UnderlyingSymbol = sCurrentUnderlying
    objMarketStructureProv.RequestOptions Options
    
    ' sub on und
    Quote.Exchange = ""
    Quote.Type = enSTK
    Quote.Symbol = sCurrentUnderlying
    objPriceProv.RequestLastQuote Quote
    If chkRealtime = 1 Then
        objPriceProv.SubscribeQuote Quote ''
    End If
    
    Exit Sub
    
ErrHandler:
    frmError.labText = "Failed to list series for underlying symbol '" & sCurrentUnderlying & "' : " & Error
    frmError.Show vbModal
End Sub

Private Sub cmdSetMMParameters_Click()
    frmMMParameters.Show vbModal
End Sub

Private Sub cmdStartTrading_Click()
On Error GoTo ErrHandler
    
    objMMManager.StartTrading
    Exit Sub
    
ErrHandler:
    frmError.labText = "Failed to start trading : " & Error
    frmError.Show vbModal
End Sub

Private Sub cmdStressSubmit_Click()
    
    For iIndex = 1 To lstSeries.ListItems.Count
        lstSeries.ListItems(iIndex).SubItems(4) = lstSeries.ListItems(iIndex).SubItems(4) + 10
        lstSeries.ListItems(iIndex).SubItems(5) = lstSeries.ListItems(iIndex).SubItems(7)
        lstSeries.ListItems(iIndex).SubItems(10) = lstSeries.ListItems(iIndex).SubItems(8)
        lstSeries.ListItems(iIndex).SubItems(11) = lstSeries.ListItems(iIndex).SubItems(11) + 10
        lstSeries.ListItems(iIndex).Checked = True
    Next
    
    SendQuotes True
End Sub

Private Sub cmdSubmit_Click()
    SendQuotes True
End Sub
    
Private Sub SendQuotes(bSubmit As Boolean)
On Error GoTo ErrHandler
Dim iIndex As Integer
Dim nCount As Integer
Dim Quotes() As ISEPROVIDERLib.ISEQuote

    nCount = 0
    For iIndex = 1 To lstSeries.ListItems.Count
        If lstSeries.ListItems(iIndex).Checked Then
            nCount = nCount + 2
        End If
    Next
    
    ReDim Quotes(nCount - 1)
    nCount = -1

    If nCount <> 0 Then
        For iIndex = 1 To lstSeries.ListItems.Count
            If lstSeries.ListItems(iIndex).Checked Then
                ' bid
                nCount = nCount + 1
                Quotes(nCount).Series = lstSeries.ListItems(iIndex).SubItems(1)
                Quotes(nCount).BidOrAsk = enBid
                Quotes(nCount).OpenQuantity = 10
                
                lstSeries.ListItems(iIndex).SubItems(3) = 0
                If bSubmit Then
                    Quotes(nCount).Premium = lstSeries.ListItems(iIndex).SubItems(5)
                Else
                    lstSeries.ListItems(iIndex).SubItems(5) = 0
                    Quotes(nCount).Premium = 0
                End If
                    
                Quotes(nCount).Quantity = lstSeries.ListItems(iIndex).SubItems(4)
                Quotes(nCount).StepUpBuffer = 10
                
                ' ask
                nCount = nCount + 1
                Quotes(nCount).Series = lstSeries.ListItems(iIndex).SubItems(1)
                Quotes(nCount).BidOrAsk = enAsk
                Quotes(nCount).OpenQuantity = 10
                
                If bSubmit Then
                    Quotes(nCount).Premium = lstSeries.ListItems(iIndex).SubItems(10)
                Else
                    Quotes(nCount).Premium = 0
                    lstSeries.ListItems(iIndex).SubItems(10) = 0
                End If
                
                Quotes(nCount).Quantity = lstSeries.ListItems(iIndex).SubItems(11)
                lstSeries.ListItems(iIndex).SubItems(12) = 0
                Quotes(nCount).StepUpBuffer = 10
                lstSeries.ListItems(iIndex).Checked = False
            End If
        Next

        objMMManager.SubmitQuotes Quotes
    End If
    
    Exit Sub
    
ErrHandler:
    frmError.labText = "Failed to send " & nCount & " quotes : " & Error
    frmError.Show vbModal
End Sub


Private Sub Form_Load()
Dim Li As ListItem
Dim test As Long
    Set objMarketStructureProv = New ISEPROVIDERLib.ISEStructureProvider
    Set objPriceProv = New ISEPROVIDERLib.ISEPriceProvider
    Set objMMManager = New ISEPROVIDERLib.ISEMMManager
    Set objISETracer = New ISEPROVIDERLib.ISETracer
    Set objGroupPrice = objPriceProv
    
    With lstSeries
        .View = lvwReport
        .FullRowSelect = True
        .ColumnHeaders.Add , , , 300
        .ColumnHeaders.Add , , "Symbol", 1500, lvwColumnCenter '1
        .ColumnHeaders.Add , , "St", 400, lvwColumnCenter '2
        .ColumnHeaders.Add , , "mmLB", 820, lvwColumnCenter '3
        .ColumnHeaders.Add , , "mmSB", 820, lvwColumnCenter '4
        .ColumnHeaders.Add , , "mmB", 820, lvwColumnCenter '5
        .ColumnHeaders.Add , , "SB", 820, lvwColumnCenter '6
        .ColumnHeaders.Add , , "B", 820, lvwColumnCenter '7
        .ColumnHeaders.Add , , "A", 820, lvwColumnCenter '8
        .ColumnHeaders.Add , , "SA", 820, lvwColumnCenter '9
        .ColumnHeaders.Add , , "mmA", 820, lvwColumnCenter '10
        .ColumnHeaders.Add , , "mmSA", 820, lvwColumnCenter '11
        .ColumnHeaders.Add , , "mmLA", 820, lvwColumnCenter '12
    End With
    
    objISETracer.Bind
   
End Sub

Public Sub LogMsg(ByVal sMsg As String)
    On Error Resume Next
    'If Len(edLog.Text) > 64000 Then edLog.Text = Right$(txtLog.Text, 32000)
    edLog.SelStart = Len(edLog.Text)
    edLog.SelText = sMsg & vbCrLf
    edLog.SelStart = Len(edLog.Text)
End Sub

Private Sub Form_Unload(Cancel As Integer)
On Error GoTo ErrHandler
    Set objMarketStructureProv = Nothing
    Set objMMManager = Nothing
    Set objISETracer = Nothing
    Set objGroupPrice = Nothing
    Set objPriceProv = Nothing
    Exit Sub
ErrHandler:
End Sub

Private Sub lstSeries_DblClick()
    Dim iIndex As Integer
    
    If lstSeries.SelectedItem Is Nothing Then
        Exit Sub
    End If
    
    iIndex = lstSeries.SelectedItem.Index
    QuoteEntry lstSeries.ListItems(iIndex)
End Sub

Private Sub QuoteEntry(ByVal Item As MSComctlLib.ListItem)
On Error Resume Next

    frmQuoteEntry.edSeries.Text = Item.SubItems(1)
    
    frmQuoteEntry.edBidQuantity.Text = Item.SubItems(4)
    If frmQuoteEntry.edBidQuantity.Text = 0 Then frmQuoteEntry.edBidQuantity.Text = 10
    frmQuoteEntry.edBidPremium.Text = Item.SubItems(5)
    
    frmQuoteEntry.edAskPremium.Text = Item.SubItems(10)
    frmQuoteEntry.edAskQuantity.Text = Item.SubItems(11)
    If frmQuoteEntry.edAskQuantity.Text = 0 Then frmQuoteEntry.edAskQuantity.Text = 10
    
    frmQuoteEntry.edAskOpenQuantity = 10
    frmQuoteEntry.edBidOpenQuantity = 10
    frmQuoteEntry.edAskStepUpBuffer = 10
    frmQuoteEntry.edBidStepUpBuffer = 10
    
    frmQuoteEntry.Show vbModal
    Item.Checked = frmQuoteEntry.bOk
    If frmQuoteEntry.bOk Then
        Item.SubItems(4) = frmQuoteEntry.strBidQuantity
        Item.SubItems(5) = frmQuoteEntry.strBidPremium
        Item.SubItems(10) = frmQuoteEntry.strAskPremium
        Item.SubItems(11) = frmQuoteEntry.strAskQuantity
    End If
End Sub

Private Sub lstSeries_ItemCheck(ByVal Item As MSComctlLib.ListItem)
    If Item.Checked = True Then
        Item.Checked = False
        QuoteEntry Item
    End If
End Sub

Private Sub objISETracer_OnHeartbeat(ByVal Instance As Long)
    timerOnline.Enabled = False
    shpOnline.FillColor = &HFF
    timerOnline.Enabled = True
    lblOrderbook.Caption = Str(Instance)
    'LogMsg "Heartbeat from orderbook #" + lblOrderbook.Caption
End Sub

Private Sub objISETracer_OnMessage(ByVal EventType As ISEPROVIDERLib.ISEInfoTypeEnum, ByVal Text As String)
    LogMsg Text
End Sub

Private Sub objMarketStructureProv_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
    LogMsg "Market structure provider error"
End Sub

Private Sub objMarketStructureProv_OnEvent(ByVal ErrorNumber As ISEPROVIDERLib.EventNumberEnum, ByVal Description As String)
    'LogMsg "Market structure provider event :" & Description
    '
    GetAllUnderlyings
End Sub

Private Sub objMarketStructureProv_OnOption(Params As ISEPROVIDERLib.OptionParams, _
    Results As ISEPROVIDERLib.OptionResults, ByVal IsLastOption As Boolean)
    
Dim Li As ListItem
Dim Quote As QuoteUpdateParams
Dim MMQuote As ISEPROVIDERLib.ISEMMQuoteParams

    If IsLastOption = False Then
        LockWindowUpdate lstSeries.hWnd
    Else
        LockWindowUpdate 0
            
        Quote.Exchange = ""
        Quote.Symbol = sCurrentUnderlying
        Quote.Type = enGrSTK

        objGroupPrice.RequestGroup Quote, enGroupRequestLastQuote
        If chkRealtime = 1 Then
            objGroupPrice.RequestGroup Quote, enGroupRequestSubscribe
        End If
        MMQuote.Symbol = sCurrentUnderlying
        MMQuote.Type = enSTK
        MMQuote.BidOrAsk = enBoth
        objMMManager.RequestLastMmQuote MMQuote
    End If
   
    If IsLastOption = True Then
        LogMsg "Number of series for " & sCurrentUnderlying & " = " & lstSeries.ListItems.Count
    
        Exit Sub
    End If
    
    If Params.UnderlyingSymbol = sCurrentUnderlying Then
         Set Li = lstSeries.ListItems.Add(, Results.Series)
        lstSeries.ListItems(Results.Series).SubItems(1) = Results.Series
    Else
        LogMsg "Not current underlying"
    End If

End Sub

Private Sub objMarketStructureProv_OnStock(Params As ISEPROVIDERLib.StockParams, Results As ISEPROVIDERLib.ISEStockResults, ByVal IsLastStock As Boolean)
    If IsLastStock = False Then
        LogMsg "Stock = " & Params.Stock & ", Bin Num = " & Results.BinNum & ", Trader Role = " & Results.TraderRole
        'If Results.TraderRole <> enTrEAM Then
            colUnderlyings.Add Results, Params.Stock
            cboUnderlyings.AddItem Params.Stock
        'End If
    Else
        LogMsg "All stocks received."
    End If
    
End Sub

Private Sub UpdateQuote(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
Dim Li As ListItem
On Error GoTo ErrHandler

    Set Li = lstSeries.ListItems(Params.Symbol)
    
    Li.SubItems(6) = Results.BidSize
    Li.SubItems(7) = Results.BidPrice
    Li.SubItems(8) = Results.AskPrice
    Li.SubItems(9) = Results.AskSize
    Li.SubItems(2) = Results.TradingStatus
    
    Exit Sub
  
ErrHandler:
    LogMsg "Failed to update price for '" & Params.Symbol & "'"
End Sub


Private Sub objMMManager_OnDeal(Deal As ISEPROVIDERLib.ISEDeal)
    LogMsg "Deal on series '" & Deal.Series & "' , Quantity = " & Deal.Quantity & " , Premium = " & Deal.Price & ", OrderID = " & Deal.OrderID
End Sub

Private Sub objMMManager_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
    LogMsg "MMManager error"
End Sub

Private Sub UpdateMmQuote(Quote As ISEPROVIDERLib.ISEQuote, Optional ByVal TickWorseLevel As Long)
Dim Li As ListItem
On Error GoTo ErrHandler
    
    Set Li = lstSeries.ListItems(Quote.Series)
    
    If Quote.BidOrAsk = enAsk Then
        Li.SubItems(10) = Quote.Premium
        Li.SubItems(11) = Quote.Quantity
        If Not IsMissing(TickWorseLevel) Then
            Li.SubItems(12) = TickWorseLevel
        End If
    Else
        Li.SubItems(5) = Quote.Premium
        Li.SubItems(4) = Quote.Quantity
        If Not IsMissing(TickWorseLevel) Then
            Li.SubItems(3) = TickWorseLevel
        End If
    End If
    
    Exit Sub
  
ErrHandler:
    LogMsg "Failed to update MM quote for '" & Quote.Series & "'"
End Sub

Private Sub objMMManager_OnEvent(ByVal ErrorNumber As ISEPROVIDERLib.EventNumberEnum, ByVal Description As String)
    LogMsg "Market manager provider event :" & Description
End Sub

Private Sub objMMManager_OnLastMmQuote(Request As ISEPROVIDERLib.ISEMMQuoteParams, Results As ISEPROVIDERLib.ISEQuote, ByVal TickWorseLevel As Long)
    UpdateMmQuote Results, TickWorseLevel
End Sub

Private Sub objMMManager_OnQuoteRejection(Quote As ISEPROVIDERLib.ISEQuote, ByVal Reason As String)
    LogMsg "Quote '" & Quote.Series & "' rejected, Reason = '" & Reason & "'"
End Sub

Private Sub objMMManager_OnQuotesSent(Quotes() As ISEPROVIDERLib.ISEQuote, ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal OrderID As String)
    Dim n As Integer
    Dim Err As String
    
    If ErrorNumber = enNoError Then
        Err = "OK"
    Else
        If ErrorNumber = enNotConnected Then
            Err = "Not connected"
        Else
            Err = "Rejected"
        End If
    End If
    
    
    n = UBound(Quotes) + 1
    LogMsg n & " quotes sent, Error = " & Err & ", OrderID = " & OrderID
End Sub

Private Sub objMMManager_OnQuoteStepUp(Quote As ISEPROVIDERLib.ISEQuote)
    LogMsg "Quote " & Quote.Series & " stepped up."
    UpdateMmQuote Quote
End Sub

Private Sub objMMManager_OnQuoteTickWorse(Quote As ISEPROVIDERLib.ISEQuote, ByVal TickWorseLevel As Long, ByVal Reason As String)
    LogMsg "Quote '" & Quote.Series & "' ticked worse, Reason = '" & Reason & "', Level = " & TickWorseLevel
    UpdateMmQuote Quote, TickWorseLevel
End Sub

Private Sub objMMManager_OnTrade(Trade As ISEPROVIDERLib.ISETrade)
    LogMsg "Trade '" & Trade.Series & "', Price = " & Trade.Price & ", Quantity = " & Trade.Quantity & ", Time = " & Trade.TradeDate & ", Bought = " & Trade.IsBought & ", New = " & Trade.IsNew & ", OrderID =" & Trade.OrderID
End Sub

Private Sub objPriceProv_OnError(ByVal ErrorNumber As ISEPROVIDERLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As ISEPROVIDERLib.RequestsTypeEnum, ByVal Request As Variant)
    LogMsg "Price provider error : " & Description
End Sub

Private Sub objPriceProv_OnEvent(ByVal ErrorNumber As ISEPROVIDERLib.EventNumberEnum, ByVal Description As String)
    LogMsg "Price provider event :" & Description
End Sub

Private Sub objPriceProv_OnLastQuote(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
    If Params.Type = enSTK Then
        LogMsg "Stock " & Params.Symbol & " Bid = " & Results.BidPrice & " Ask = " & Results.AskPrice & " , Status = " & Results.TradingStatus
    Else
        If Params.Type = enOPT Then
            UpdateQuote Params, Results
        Else
            LogMsg "OnLastQuote Type = " & Params.Type
        End If
    End If
End Sub

Private Sub objPriceProv_OnQuoteUpdate(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
    If Params.Type = enSTK Then
        LogMsg "Stock Update " & Params.Symbol & " Bid = " & Results.BidPrice & " Ask = " & Results.AskPrice & ", Status = " & Results.TradingStatus
    Else
        LogMsg "Option Update " & Params.Symbol & " Bid = " & Results.BidPrice & " Ask = " & Results.AskPrice
        UpdateQuote Params, Results
    End If
End Sub

Private Sub timerOnline_Timer()
    shpOnline.FillColor = &H808080
    lblOrderbook.Caption = ""
End Sub
