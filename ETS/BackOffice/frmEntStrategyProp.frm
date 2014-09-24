VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEntStrategyProp 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Strategy Properties"
   ClientHeight    =   3645
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   6000
   Icon            =   "frmEntStrategyProp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3645
   ScaleWidth      =   6000
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fg 
      Height          =   3435
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4335
      _cx             =   7646
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
      FormatString    =   $"frmEntStrategyProp.frx":000C
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
      Left            =   4560
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
Attribute VB_Name = "frmEntStrategyProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum GridColumns
    TC_TRADER
    TC_UNDSYMBOL
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_nStrategyID As Long
Private m_sStrategyName As String

Public Sub Execute(ByVal nStrategyId As Long, ByVal sStrategyName As String)
    
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    
    m_nStrategyID = nStrategyId
    m_sStrategyName = sStrategyName

    Load Me
    
    Me.Caption = m_sStrategyName & "'s traders & contracts"
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
        
    Exit Sub
Herr:
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim I As Long
    On Error Resume Next
    
    With fg
        .Rows = 0
        .Cols = COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDNone
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 2
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(TC_TRADER) = True
        .MergeCol(TC_UNDSYMBOL) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        I = TC_TRADER
        .TextMatrix(0, I) = "Trader"
        .TextMatrix(1, I) = "Trader"
        .ColDataType(I) = flexDTString
        
        I = TC_UNDSYMBOL
        .TextMatrix(0, I) = "Underlying" & vbCrLf & "symbol"
        .TextMatrix(1, I) = "Underlying" & vbCrLf & "symbol"
        .ColDataType(I) = flexDTString
                
                                   
    End With
End Sub

Private Function LoadData() As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    LoadData = False
                
    With fg
        .Redraw = flexRDNone
        
        Set rs = gDBW.usp_BoStrategyTraderContract_Get(m_nStrategyID)
        While Not rs.EOF
            .AddItem ""
            .Cell(flexcpText, .Rows - 1, TC_TRADER) = gCmn.ReadStr(rs!vcAcronym)
            .Cell(flexcpText, .Rows - 1, TC_UNDSYMBOL) = gCmn.ReadStr(rs!vcUnderlyingSymbol)
            rs.MoveNext
        Wend
        
        If .Rows > 2 Then
            .Row = 2
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

