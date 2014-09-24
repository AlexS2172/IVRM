VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Begin VB.Form frmMarketDepth 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "MarketDepth"
   ClientHeight    =   3900
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   3285
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3900
   ScaleWidth      =   3285
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VSFlex8UCtl.VSFlexGrid fgMktDepth 
      Align           =   1  'Align Top
      Height          =   3750
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3285
      _cx             =   5794
      _cy             =   6615
      Appearance      =   0
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
      ForeColor       =   0
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   8388608
      ForeColorSel    =   65535
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   3
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   1
      SelectionMode   =   1
      GridLines       =   1
      GridLinesFixed  =   1
      GridLineWidth   =   1
      Rows            =   20
      Cols            =   3
      FixedRows       =   0
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmMarketDepth.frx":0000
      ScrollTrack     =   0   'False
      ScrollBars      =   2
      ScrollTips      =   0   'False
      MergeCells      =   1
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   0   'False
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   2
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   0
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
   End
End
Attribute VB_Name = "frmMarketDepth"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents ctlSource As ctlQuotesViewSingle
Attribute ctlSource.VB_VarHelpID = -1
Private nRows As Long
Private mOptColl As MmQvOptColl
Private Enum QVMarketDepthColumn
    BID_COL = 0
    ASK_COL = 1
    OPT_ID = 2
End Enum

Public Sub ShowContractsMarketDepth(ByRef ctlView As ctlQuotesViewSingle)
    'On Error Resume Next
    
    Set mOptColl = Nothing
    Set ctlSource = Nothing
    
    If Not ctlView Is Nothing Then
        Set ctlSource = ctlView
        Set mOptColl = ctlSource.MarketDepthOptColl
        Load Me
        Me.Show
        RefreshAll
    End If
    
End Sub
Private Sub ctlSource_OnScreenClose()
    'On Error Resume Next
    Unload Me
End Sub
Private Sub ctlSource_OnScreenRefresh()
    'On Error Resume Next
    RefreshAll
End Sub

Private Sub ctlSource_OnScreenRefreshAndShow()
    'On Error Resume Next
    RefreshAll
    Show
End Sub

Private Sub fgMktDepth_KeyDown(KeyCode As Integer, Shift As Integer)
    'On Error Resume Next
    Dim Key As Long
    If fgMktDepth.Row >= 0 Then
        Key = CLng(fgMktDepth.TextMatrix(fgMktDepth.Row, OPT_ID))
        If KeyCode = vbKeyDelete Then
            mOptColl.Remove Key
            RefreshAll
        End If
    End If
End Sub
Private Sub Form_Resize()
    'On Error Resume Next
    fgMktDepth.Height = Height - 400
End Sub

Private Sub Form_Unload(Cancel As Integer)
    'On Error Resume Next
    
    ctlSource.MarketDepthVisible = False
    mOptColl.Clear
    Set ctlSource = Nothing
        
End Sub
Private Sub AddOptionContractToGrid(ByRef aOpt As EtsMmQuotesLib.MmQvOptAtom)
    On Error Resume Next

    Dim aBidMarketData() As MmQvMarketDepthAtom
    Dim aAskMarketData() As MmQvMarketDepthAtom
    Dim nMaxRows As Long
    Dim j As Long
    Dim nBidCount As Long, nAskCount As Long
    nBidCount = 0
    nAskCount = 0
    
    If Not aOpt Is Nothing Then
        
        AddRow
        AddContractHeader aOpt, nRows - 1
        aOpt.CalcMarketDepth aBidMarketData, aAskMarketData
        
        nBidCount = UBound(aBidMarketData) - LBound(aBidMarketData) + 1
        nAskCount = UBound(aAskMarketData) - LBound(aAskMarketData) + 1
        nMaxRows = IIf(nBidCount > nAskCount, nBidCount, nAskCount)
        
        If nMaxRows > 0 Then
         For j = 0 To nMaxRows - 1
             AddRow
             If j < nBidCount Then AddMarketDepthData nRows - 1, BID_COL, aBidMarketData(j)
             If j < nAskCount Then AddMarketDepthData nRows - 1, ASK_COL, aAskMarketData(j)
             SetOptIdForRow nRows - 1, aOpt
         Next
        End If
    End If
    
End Sub
Private Sub RefreshAll()
    'On Error Resume Next
    
    Dim aOpt As MmQvOptAtom
    Dim j As Long
    
    If Not ctlSource.Group Is Nothing Then
        Caption = ctlSource.Group.Symbol & " Market Depth"
    End If
      
    nRows = 0
    fgMktDepth.Rows = 0
    
    AddRow
    AddMainHeader
      
    If Not mOptColl Is Nothing Then
        For Each aOpt In mOptColl
            AddOptionContractToGrid aOpt
        Next
    End If
    
End Sub
Private Sub AddMarketDepthData(ByVal nRow As Long, ByVal nCol As Long, ByRef aData As MmQvMarketDepthAtom)
    'On Error Resume Next
        fgMktDepth.TextMatrix(nRow, nCol) = IIf(aData.Price <> BAD_DOUBLE_VALUE, Str(aData.Price), "--") & " / " & _
                                            IIf(aData.Depth <> BAD_LONG_VALUE, Str(aData.Depth), "--")
End Sub
Private Sub AddContractHeader(ByRef aOpt As EtsMmQuotesLib.MmQvOptAtom, ByVal Row As Long)
    'On Error Resume Next
    
    fgMktDepth.MergeRow(Row) = True
    fgMktDepth.TextMatrix(Row, BID_COL) = aOpt.Symbol & Str(aOpt.Strike) & " " & Str(aOpt.Expiry) & " " & IIf(aOpt.OptType = enOtCall, "C", "P")
    fgMktDepth.TextMatrix(Row, ASK_COL) = fgMktDepth.TextMatrix(Row, BID_COL)
    SetOptIdForRow Row, aOpt
    fgMktDepth.Select Row, 0
    fgMktDepth.CellBackColor = RGB(125, 125, 125)
    fgMktDepth.CellForeColor = RGB(255, 255, 255)
    
End Sub

Private Sub AddMainHeader()
    'On Error Resume Next
    fgMktDepth.TextMatrix(0, BID_COL) = "Bid / Size"
    fgMktDepth.TextMatrix(0, ASK_COL) = "Ask / Size"
End Sub

Private Sub AddRow()
    'On Error Resume Next
    fgMktDepth.Rows = fgMktDepth.Rows + 1
    nRows = nRows + 1
End Sub
Private Sub SetOptIdForRow(ByVal Row As Long, ByRef Opt As MmQvOptAtom)
    'On Error Resume Next
    fgMktDepth.TextMatrix(Row, OPT_ID) = Str(Opt.ID)
End Sub
