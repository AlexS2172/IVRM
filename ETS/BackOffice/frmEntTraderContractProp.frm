VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEntTraderContractProp 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Trader/Contract strategies"
   ClientHeight    =   3675
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4380
   Icon            =   "frmEntTraderContractProp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3675
   ScaleWidth      =   4380
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fg 
      Height          =   3435
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2715
      _cx             =   4789
      _cy             =   6059
      _ConvInfo       =   1
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
      ForeColorSel    =   0
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   0
      SelectionMode   =   1
      GridLines       =   1
      GridLinesFixed  =   1
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   2
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmEntTraderContractProp.frx":000C
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
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   2940
      TabIndex        =   1
      Top             =   120
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Close"
   End
End
Attribute VB_Name = "frmEntTraderContractProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum GridColumns
    TC_STRATEGY
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_nTraderID&
Private m_sTraderName$
Private m_nUnderlyingID&
Private m_sUnderlyingSymbol$

Public Sub Execute(ByVal nTraderId&, ByVal sTraderName$, _
                        ByVal nUnderlyingID&, ByVal sUnderlyingSymbol$)
    On Error GoTo EH
    Screen.MousePointer = vbHourglass
    m_nTraderID = nTraderId
    m_sTraderName = sTraderName
    m_nUnderlyingID = nUnderlyingID
    m_sUnderlyingSymbol = sUnderlyingSymbol

    Load Me
    
    Me.Caption = m_sTraderName & "/" & m_sUnderlyingSymbol & " strategies"
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    Me.Hide
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
End Sub

Private Sub InitGrid()
    Dim i&
    On Error Resume Next
    
    With fg
        .Rows = 0
        .Cols = COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 1
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDNone
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        i = TC_STRATEGY
        .TextMatrix(0, i) = "Strategy"
        .ColDataType(i) = flexDTString
    End With
End Sub

Private Function LoadData() As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    LoadData = False
                
    With fg
        .Redraw = flexRDNone
        
        Set rs = gDBW.usp_BoTraderContractStrategy_Get(m_nTraderID, m_nUnderlyingID)
        While Not rs.EOF
            .AddItem ""
            .Cell(flexcpText, .Rows - 1, TC_STRATEGY) = gCmn.ReadStr(rs!vcStrategyName)
            rs.MoveNext
        Wend
        
        If .Rows > 1 Then
            .Row = 1
            .Col = 0
        End If
        .Redraw = flexRDBuffered
    End With
    
    LoadData = True
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    ShowError
End Function
