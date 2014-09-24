VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlIndexHedge2View 
   ClientHeight    =   7905
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   10530
   KeyPreview      =   -1  'True
   ScaleHeight     =   7905
   ScaleWidth      =   10530
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   5040
      Top             =   6060
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnd1 
      Height          =   2055
      Left            =   0
      TabIndex        =   3
      Top             =   1320
      Width           =   4395
      _cx             =   7752
      _cy             =   3625
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   -1  'True
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgFlt 
      Height          =   495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   9075
      _cx             =   16007
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTot1 
      Height          =   495
      Left            =   0
      TabIndex        =   1
      Top             =   660
      Width           =   4395
      _cx             =   7752
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnd2 
      Height          =   2055
      Left            =   4680
      TabIndex        =   4
      Top             =   1320
      Width           =   4395
      _cx             =   7752
      _cy             =   3625
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   -1  'True
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTot2 
      Height          =   495
      Left            =   4680
      TabIndex        =   2
      Top             =   660
      Width           =   4395
      _cx             =   7752
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgRes 
      Height          =   2055
      Left            =   0
      TabIndex        =   5
      Top             =   3480
      Width           =   9075
      _cx             =   16007
      _cy             =   3625
      _ConvInfo       =   1
      Appearance      =   1
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
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   -1  'True
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
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxPositionTransfer 
         Caption         =   "Position Transfer..."
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator8 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlIndexHedge2View"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public m_frmOwner As Form
'
'Public Event OnSetCaption()
'Public Event OnStateChange()
'
'Private m_ge(GT_INDEXHEDGE2_UNDERLYINGS_1 To GT_INDEXHEDGE2_RESULTS) As New clsGridEx
'Private WithEvents geRes As clsGridEx
'
'Private WithEvents aParams As clsParams
'Private WithEvents PriceProvider As PRICEPROVIDERSLib.PriceInfo
'Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
'Private VolaSource As VolatilitySourcesLib.VolatilitySource
'Private TradeChannel As clsTradeChannel
'
'Private WithEvents frmLayout As frmGridLayout
'
'Public pbProgress As MSComctlLib.ProgressBar
'Public lblProcess As VB.Label
'Public lblStatus As VB.Label
'Public WithEvents imgStop As VB.Image
'Public imgStopDis As VB.Image
'
'Private m_bInProc As Boolean
'Private m_bDataLoad As Boolean
'Private m_bLastQuoteReqNow As Boolean
'Private m_bRecalc As Boolean
'
'Private m_QuoteRequests As New clsIhRequestColl
'Private m_nQuoteReqCount As Long
'Private m_nQuoteReqDone As Long
'Private m_nQuoteGroupReqCount As Long
'Private m_nQuoteGroupReqDone As Long
'
'Private m_sCurrentOriginalText As String
'
'Private m_enMenuGrid As GridTypeEnum
'Private m_nMenuGridCol As Long
'Private m_nMenuGridRow As Long
'Private m_nMenuGridCols As Long
'Private m_nMenuGridRows As Long
'
'Private m_nFilter(JFC_TRADER To JFC_INDEX2) As Long
'Private m_nFilterPrev(JFC_TRADER To JFC_INDEX2) As Long
'
'Private m_bKeyDown(GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS) As Boolean
'Private m_GridLock(GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS) As New clsGridLock
'
'Private m_gd(GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS) As clsGridDef
'Private m_fg(GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS) As VSFlex7Ctl.VSFlexGrid
'
'Private m_bGroupRequest As Boolean
'Private m_bGroupRequestSupported As Boolean
'Private m_bShutDown As Boolean
'
'Private m_Idx(1 To 2) As EtsMmIndexHedgeLib.MmIhIndexAtom
'Private m_Res As EtsMmIndexHedgeLib.MmIhUndColl
'
'Private m_dSlice#
'Private m_nLotSize&
'Private m_nMaxSize&
'
'Private m_dExposure#(1 To 2)
'Private m_nContracts&(1 To 2)
'Private m_dDelta#(1 To 2)
'
'Public Function Init() As Boolean
'    On Error GoTo EH
'
'    m_dSlice = g_Params.IndexHedgeSlice
'    m_nLotSize = g_Params.IndexHedgeLotSize
'    m_nMaxSize = g_Params.IndexHedgeMaxSize
'
'    m_dExposure(1) = BAD_DOUBLE_VALUE
'    m_nContracts(1) = 0&
'    m_dDelta(1) = 0#
'
'    m_dExposure(2) = BAD_DOUBLE_VALUE
'    m_nContracts(2) = 0&
'    m_dDelta(2) = 0#
'
'    m_bShutDown = False
'
'    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
'    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
'    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
'
'    ResetMenuData
'    m_sCurrentOriginalText = ""
'
'    InitColumns
'    InitGrids
'
'    m_ge(GT_INDEXHEDGE2_UNDERLYINGS_1).Init m_fg(GT_INDEXHEDGE2_UNDERLYINGS_1)
'    m_ge(GT_INDEXHEDGE2_UNDERLYINGS_2).Init m_fg(GT_INDEXHEDGE2_UNDERLYINGS_2)
'    m_ge(GT_INDEXHEDGE2_RESULTS).Init m_fg(GT_INDEXHEDGE2_RESULTS)
'
'    FormatFltGrid
'    FormatFltColumns
'
'    FormatTotGrid GT_INDEXHEDGE2_TOTALS_1
'    FormatTotColumns GT_INDEXHEDGE2_TOTALS_1
'
'    FormatTotGrid GT_INDEXHEDGE2_TOTALS_2
'    FormatTotColumns GT_INDEXHEDGE2_TOTALS_2
'
'    FormatUndGrid GT_INDEXHEDGE2_UNDERLYINGS_1
'    FormatUndColumns GT_INDEXHEDGE2_UNDERLYINGS_1
'
'    FormatUndGrid GT_INDEXHEDGE2_UNDERLYINGS_2
'    FormatUndColumns GT_INDEXHEDGE2_UNDERLYINGS_2
'
'    FormatResGrid
'    FormatResColumns
'
'    InitFltData
'
'    Set VolaSource = g_VolaSource
'    Set TradeChannel = g_TradeChannel
'
'    Dim aPT As PRICEPROVIDERSLib.IProvider
'    Set PriceProvider = New PRICEPROVIDERSLib.PriceInfo
'
'    Set aPT = PriceProvider
'    aPT.Type = g_Params.PriceProviderType
'    Set aPT = Nothing
'
'    PriceProvider.Connect
'
'    m_bGroupRequest = False
'
'    m_bGroupRequestSupported = g_Params.PriceProviderIsGroupRequestSupported
'    If m_bGroupRequestSupported Then
'        Set m_GroupPriceProvider = PriceProvider
'        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
'    Else
'        Set m_GroupPriceProvider = Nothing
'    End If
'
'    fgFlt.Col = JFC_TRADER
'    fgFlt.Row = 1
'
'    Set frmLayout = New frmGridLayout
'    Set aParams = g_Params
'
'    Init = True
'
'    UpdateTotalsAll
'    AdjustCaption
'    AdjustState
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize index hedge."
'End Function
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    Dim i&
'
'    Select Case nType
'        Case TYPE_TRADER
'            m_nFilter(JFC_TRADER) = nValue
'        Case TYPE_STRATEGY
'            m_nFilter(JFC_STRATEGY) = nValue
'        Case TYPE_UNDERLYING
'            m_nFilter(JFC_INDEX1) = nValue
'    End Select
'
'    For i = JFC_TRADER To JFC_LAST_COLUMN
'        m_nFilterPrev(i) = 0
'    Next
'
'    FilterUpdate
'    FilterUpdateChangedStatus
'    AdjustState
'End Sub
'
'Private Sub fgUnd1_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    fgUnd_AfterUserResize GT_INDEXHEDGE2_UNDERLYINGS_1, Row, Col
'End Sub
'
'Private Sub fgUnd1_DblClick()
'    On Error Resume Next
'    fgUnd_DblClick GT_INDEXHEDGE2_UNDERLYINGS_1
'End Sub
'
'Private Sub fgUnd1_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgUnd_KeyDown GT_INDEXHEDGE2_UNDERLYINGS_1, KeyCode, Shift
'End Sub
'
'Private Sub fgUnd1_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgUnd_KeyUp GT_INDEXHEDGE2_UNDERLYINGS_1, KeyCode, Shift
'End Sub
'
'Private Sub fgUnd1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    fgUnd_MouseUp GT_INDEXHEDGE2_UNDERLYINGS_1, Button, Shift, X, Y
'End Sub
'
'Private Sub fgUnd2_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    fgUnd_AfterUserResize GT_INDEXHEDGE2_UNDERLYINGS_1, Row, Col
'End Sub
'
'Private Sub fgUnd2_DblClick()
'    On Error Resume Next
'    fgUnd_DblClick GT_INDEXHEDGE2_UNDERLYINGS_2
'End Sub
'
'Private Sub fgUnd2_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgUnd_KeyDown GT_INDEXHEDGE2_UNDERLYINGS_2, KeyCode, Shift
'End Sub
'
'Private Sub fgUnd2_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgUnd_KeyUp GT_INDEXHEDGE2_UNDERLYINGS_2, KeyCode, Shift
'End Sub
'
'Private Sub fgUnd2_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    fgUnd_MouseUp GT_INDEXHEDGE2_UNDERLYINGS_2, Button, Shift, X, Y
'End Sub
'
'Private Sub fgUnd_AfterUserResize(nIdx&, ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim i&
'
'    If Col >= 0 And Row < 0 Then
'        With m_fg(nIdx)
'            i = .ColKey(Col)
'            If i > JUC_KEY And i <= JUC_LAST_COLUMN Then
'                m_gd(nIdx).Col(i).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgUnd_DblClick(nIdx&)
'    On Error Resume Next
'    With m_fg(nIdx)
'        m_enMenuGrid = nIdx
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick True
'    End With
'End Sub
'
'Private Sub fgUnd_KeyDown(nIdx&, KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridCommonOrTrade(KeyCode, Shift) Then
'        m_bKeyDown(nIdx) = True
'    End If
'End Sub
'
'Private Sub fgUnd_KeyUp(nIdx&, KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(nIdx) Then
'        m_bKeyDown(nIdx) = False
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyInsert And Shift = 0
'                mnuCtxTradeNew_Click
'
'            Case KeyCode = vbKeyG And Shift = vbCtrlMask
'                mnuCtxAutosizeGrid_Click
'
'            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
'                mnuCtxAutosizeCol_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgUnd_MouseUp(nIdx&, Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgTot1_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    fgTot_AfterEdit GT_INDEXHEDGE2_TOTALS_1, Row, Col
'End Sub
'
'Private Sub fgTot1_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    fgTot_StartEdit GT_INDEXHEDGE2_TOTALS_1, Row, Col, Cancel
'End Sub
'
'Private Sub fgTot1_DblClick()
'    On Error Resume Next
'    fgTot_DblClick GT_INDEXHEDGE2_TOTALS_1
'End Sub
'
'Private Sub fgTot1_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgTot_KeyDown GT_INDEXHEDGE2_TOTALS_1, KeyCode, Shift
'End Sub
'
'Private Sub fgTot1_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgTot_KeyUp GT_INDEXHEDGE2_TOTALS_1, KeyCode, Shift
'End Sub
'
'Private Sub fgTot1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    fgTot_MouseUp GT_INDEXHEDGE2_TOTALS_1, Button, Shift, X, Y
'End Sub
'
'Private Sub fgTot2_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    fgTot_AfterEdit GT_INDEXHEDGE2_TOTALS_2, Row, Col
'End Sub
'
'Private Sub fgTot2_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    fgTot_StartEdit GT_INDEXHEDGE2_TOTALS_2, Row, Col, Cancel
'End Sub
'
'Private Sub fgTot2_DblClick()
'    On Error Resume Next
'    fgTot_DblClick GT_INDEXHEDGE2_TOTALS_2
'End Sub
'
'Private Sub fgTot2_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgTot_KeyDown GT_INDEXHEDGE2_TOTALS_2, KeyCode, Shift
'End Sub
'
'Private Sub fgTot2_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    fgTot_KeyUp GT_INDEXHEDGE2_TOTALS_2, KeyCode, Shift
'End Sub
'
'Private Sub fgTot2_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    fgTot_MouseUp GT_INDEXHEDGE2_TOTALS_2, Button, Shift, X, Y
'End Sub
'
'Private Sub fgTot_AfterEdit(nIdx&, ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    If m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
'    Dim sValue$, dValue#, nValue&, bRecalcTotals As Boolean: bRecalcTotals = False
'    Dim nIndexIdx&
'
'    nIndexIdx = nIdx - GT_INDEXHEDGE2_TOTALS_1 + 1
'
'    With m_fg(nIdx)
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            Select Case Col
'                Case JTC_SLICE
'                    dValue = Abs(ReadDbl(sValue)) / 100#
'                    If dValue > 0.01 And dValue <= 1# And m_dSlice <> dValue Then
'                        m_dSlice = dValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeSlice = m_dSlice
'                    End If
'                    m_fg(GT_INDEXHEDGE2_TOTALS_1).TextMatrix(Row, Col) = m_dSlice * 100#
'                    m_fg(GT_INDEXHEDGE2_TOTALS_2).TextMatrix(Row, Col) = m_dSlice * 100#
'
'                Case JTC_LOT_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nLotSize <> nValue Then
'                        m_nLotSize = nValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeLotSize = m_nLotSize
'                    End If
'                    m_fg(GT_INDEXHEDGE2_TOTALS_1).TextMatrix(Row, Col) = m_nLotSize
'                    m_fg(GT_INDEXHEDGE2_TOTALS_2).TextMatrix(Row, Col) = m_nLotSize
'
'                Case JTC_MAX_SIZE
'                    nValue = Abs(ReadLng(sValue))
'                    If nValue > 0# And m_nMaxSize <> nValue Then
'                        m_nMaxSize = nValue
'                        bRecalcTotals = True
'                        g_Params.IndexHedgeMaxSize = m_nMaxSize
'                    End If
'                    m_fg(GT_INDEXHEDGE2_TOTALS_1).TextMatrix(Row, Col) = m_nMaxSize
'                    m_fg(GT_INDEXHEDGE2_TOTALS_2).TextMatrix(Row, Col) = m_nMaxSize
'
'                Case JTC_CONTRACTS
'                    nValue = ReadLng(sValue)
'                    If m_nContracts(nIndexIdx) <> nValue Then
'                        m_nContracts(nIndexIdx) = nValue
'                        bRecalcTotals = True
'                    End If
'                    .TextMatrix(Row, Col) = IIf(m_nContracts(nIndexIdx) <> 0&, m_nContracts(nIndexIdx), "")
'
'                Case JTC_DELTA
'                    dValue = ReadDbl(sValue) / 100#
'                    If m_dDelta(nIndexIdx) <> dValue Then
'                        m_dDelta(nIndexIdx) = dValue
'                        bRecalcTotals = True
'                    End If
'                    .TextMatrix(Row, Col) = IIf(m_dDelta(nIndexIdx) <> 0#, m_dDelta(nIndexIdx) * 100#, 0#)
'
'            End Select
'
'            If bRecalcTotals Then
'                CalcTotalsAll False
'                UnderlyingsUpdateAll False
'                ResultsUpdate False
'                UpdateTotalsAll
'            End If
'        End If
'    End With
'End Sub
'
'Private Sub fgTot_StartEdit(nIdx&, ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = (Col >= JTC_NET_DELTA) Or IsDblClickHandled Or m_bShutDown Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(m_fg(nIdx).TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgTot_DblClick(nIdx&)
'    On Error Resume Next
'    With m_fg(nIdx)
'        m_enMenuGrid = nIdx
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick False
'    End With
'End Sub
'
'Private Sub fgTot_KeyDown(nIdx&, KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridClip(KeyCode, Shift) Then m_bKeyDown(nIdx) = True
'End Sub
'
'Private Sub fgTot_KeyUp(nIdx&, KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(nIdx) Then
'        m_bKeyDown(nIdx) = False
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgTot_MouseUp(nIdx&, Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub fgRes_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim i&, nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'
'    If Col >= 0 And Row < 0 Then
'        With m_fg(nIdx)
'            i = .ColKey(Col)
'            If i > JRC_KEY And i <= JRC_LAST_COLUMN Then
'                m_gd(nIdx).Col(i).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
'            End If
'        End With
'    End If
'End Sub
'
'Private Sub fgRes_DblClick()
'    On Error Resume Next
'    Dim nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'    With m_fg(nIdx)
'        m_enMenuGrid = nIdx
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick True
'    End With
'End Sub
'
'Private Sub fgRes_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    Dim nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'    If IsHotKeyGridCommonOrTrade(KeyCode, Shift) Then
'        m_bKeyDown(nIdx) = True
'    End If
'End Sub
'
'Private Sub fgRes_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    Dim nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'    If m_bKeyDown(nIdx) Then
'        m_bKeyDown(nIdx) = False
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyInsert And Shift = 0
'                mnuCtxTradeNew_Click
'
'            Case KeyCode = vbKeyG And Shift = vbCtrlMask
'                mnuCtxAutosizeGrid_Click
'
'            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
'                mnuCtxAutosizeCol_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgRes_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    Dim nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'    If Button = vbRightButton Then
'        With m_fg(nIdx)
'            m_enMenuGrid = nIdx
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub mnuCtxAutosizeCol_Click()
'    On Error Resume Next
'    Dim nIdx&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1, GT_INDEXHEDGE2_UNDERLYINGS_2
'            With m_fg(m_enMenuGrid)
'                m_GridLock(m_enMenuGrid).LockRedraw
'                .AutoSize m_nMenuGridCol, m_nMenuGridCol
'
'                nIdx = .ColKey(m_nMenuGridCol)
'                If nIdx > JUC_KEY And nIdx <= JUC_LAST_COLUMN Then
'                    m_gd(m_enMenuGrid).Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
'                End If
'
'                m_GridLock(m_enMenuGrid).UnlockRedraw
'            End With
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxAutosizeGrid_Click()
'    On Error Resume Next
'    Dim nIdx&, i&, nCount&
'
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1, GT_INDEXHEDGE2_UNDERLYINGS_2
'            With m_fg(m_enMenuGrid)
'                m_GridLock(m_enMenuGrid).LockRedraw
'                .AutoSize 0, .Cols - 1
'
'                nCount = .Cols - 1
'                For i = 0 To nCount
'                    nIdx = .ColKey(i)
'                    If nIdx > JUC_KEY And nIdx <= JUC_LAST_COLUMN Then
'                        m_gd(m_enMenuGrid).Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
'                    End If
'                Next
'
'                m_GridLock(m_enMenuGrid).UnlockRedraw
'            End With
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxPositionTransfer_Click()
'    On Error Resume Next
'    If m_bShutDown Or m_bInProc Then Exit Sub
'    Dim nUndID&, nTrdID&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim sContractDesc$, aGUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    nUndID = 0
'    nTrdID = m_nFilter(JFC_TRADER)
'
'    If m_enMenuGrid = GT_INDEXHEDGE2_UNDERLYINGS_1 Or m_enMenuGrid = GT_INDEXHEDGE2_UNDERLYINGS_2 Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then _
'            nUndID = m_fg(m_enMenuGrid).RowData(m_nMenuGridRow).ID
'
'    ElseIf m_enMenuGrid = GT_INDEXHEDGE2_FILTER And (m_nMenuGridCol = JFC_INDEX1 Or m_nMenuGridCol = JFC_INDEX2) Then
'        nUndID = m_nFilter(m_nMenuGridCol)
'    End If
'
'    m_bInProc = False
'    If nUndID <> 0 Then
'        Set aGUnd = g_Underlying(nUndID)
'        If Not aGUnd Is Nothing Then
'            sContractDesc = aGUnd.Symbol & " (" & aGUnd.ContractName & ")"
'            Set aGUnd = Nothing
'        End If
'
'        On Error GoTo EH
'        frmPositionTransfer.TransferPosition nUndID, nUndID, nTrdID, sContractDesc, m_frmOwner
'    End If
'    Exit Sub
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to transfer position."
'    ResetMenuData
'End Sub
'
'Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE2_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Two Indices Hedge Filter", "Two Indices Hedge Filter", fgFlt
'
'        Case GT_INDEXHEDGE2_TOTALS_1
'            frmPrintPreview.Execute m_frmOwner, "1st Index Hedge Parameters/Totals", "Two Indices Hedge Parameters/Totals", fgTot1
'
'        Case GT_INDEXHEDGE2_TOTALS_2
'            frmPrintPreview.Execute m_frmOwner, "2nd Index Hedge Parameters/Totals", "Two Indices Hedge Parameters/Totals", fgTot1
'
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1
'            frmPrintPreview.Execute m_frmOwner, "1st Index Hedge Stocks", "Two Indices Hedge Stocks", fgUnd1
'
'        Case GT_INDEXHEDGE2_UNDERLYINGS_2
'            frmPrintPreview.Execute m_frmOwner, "2nd Index Hedge Stocks", "Two Indices Hedge Stocks", fgUnd1
'
'        Case GT_INDEXHEDGE2_RESULTS
'            frmPrintPreview.Execute m_frmOwner, "Two Indices Hedge Results", "Two Indices Hedge Results", fgFlt
'
'    End Select
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub tmrShow_Timer()
'    On Error Resume Next
'    tmrShow.Enabled = False
'
'    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
'
'    ClearViewAndData
'    If Not IsValidFilter Then Exit Sub
'
'    SetRefreshHint False
'    CopyFilters True
'    FilterUpdateChangedStatus
'
'    If Not PriceProvider Is Nothing Then
'        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'        m_bLastQuoteReqNow = False
'    End If
'
'    AdjustCaption
'
'    If UnderlyingsLoadAll Then
'        UpdateTotalsAll
'        UnderlyingsUpdateAll True
'        ResultsUpdate True
'        AdjustCaption
'
'        lblProcess.Caption = "Last quotes request..."
'        lblProcess.Refresh
'
'        RequestLastQuotes
'    Else
'        ClearViewAndData
'        AdjustCaption
'        AdjustState
'
'        pbProgress.Visible = False
'        lblProcess.Visible = False
'        lblStatus.Visible = True
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'    End If
'End Sub
'
'Private Function UnderlyingsLoadAll() As Boolean
'    On Error GoTo EH
'    UnderlyingsLoadAll = False
'    If m_bInProc Then Exit Function
'    If Not IsValidFilter Then Exit Function
'
'    Dim aUndByTrd As clsTradeByUndAtom
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aIdx As EtsMmIndexHedgeLib.MmIhIndexAtom
'    Dim aResUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim i&
'
'    m_bInProc = True
'    m_bDataLoad = True
'    AdjustState
'
'    If m_bDataLoad Then
'        If Not g_BasketIndex(m_nFilter(JFC_INDEX1)) Is Nothing And Not g_Underlying(m_nFilter(JFC_INDEX1)) Is Nothing _
'            And Not g_BasketIndex(m_nFilter(JFC_INDEX2)) Is Nothing And Not g_Underlying(m_nFilter(JFC_INDEX2)) Is Nothing Then
'
'            lblStatus.Visible = False
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = g_TradeChannel.Trades.Count * 2
'            pbProgress.Visible = True
'            lblProcess.Caption = "Data loading..."
'            lblProcess.Visible = True
'            lblProcess.Refresh
'
'            Set aUndByTrd = TradeChannel.TradesByUnd(CStr(m_nFilter(JFC_INDEX1)))
'            If Not aUndByTrd Is Nothing Then pbProgress.Max = pbProgress.Max + aUndByTrd.Trd.Count
'
'            Set aUndByTrd = TradeChannel.TradesByUnd(CStr(m_nFilter(JFC_INDEX2)))
'            If Not aUndByTrd Is Nothing Then pbProgress.Max = pbProgress.Max + aUndByTrd.Trd.Count
'
'            Set aUndByTrd = Nothing
'
'            If UnderlyingsLoad(JFC_INDEX1) Then
'                If UnderlyingsLoad(JFC_INDEX2) Then
'                    Set m_Res = New EtsMmIndexHedgeLib.MmIhUndColl
'
'                    For i = 1 To 2
'                        Set aIdx = m_Idx(i)
'                        For Each aUnd In aIdx.Und
'                            Set aResUnd = m_Res(aUnd.ID)
'                            If aResUnd Is Nothing Then
'                                Set aResUnd = m_Res.Add(aUnd.ID, aUnd.Symbol)
'                                aResUnd.ID = aUnd.ID
'                                aResUnd.Symbol = aUnd.Symbol
'                                aResUnd.TotalQtyInShares = aUnd.TotalQtyInShares
'                                aResUnd.CurShares = aUnd.CurShares
'                                Set aResUnd = Nothing
'                            End If
'                        Next
'                        Set aIdx = Nothing
'                    Next
'
'                    UnderlyingsLoadAll = m_bDataLoad
'                End If
'            End If
'
'        End If
'    End If
'
'    m_bInProc = False
'EX:
'    On Error Resume Next
'    Set aUndByTrd = Nothing
'    m_bDataLoad = False
'    AdjustState
'    Exit Function
'EH:
'    m_bInProc = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load stocks."
'    GoTo EX
'End Function
'
'Private Function UnderlyingsLoad(nIdx&) As Boolean
'    On Error GoTo EH
'    UnderlyingsLoad = False
'    If Not m_bDataLoad Then Exit Function
'
'    Dim aGIdx As EtsGeneralLib.IndexAtom
'    Dim aGUnd As EtsGeneralLib.UndAtom
'    Dim aIdxComp As EtsGeneralLib.IndexCompAtom
'    Dim aUndByTrd As clsTradeByUndAtom
'    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim aReq As clsIhRequestAtom
'    Dim sKey$, nIndexIdx&
'
'    nIndexIdx = nIdx - JFC_INDEX1 + 1
'
'    Set aGIdx = g_BasketIndex(m_nFilter(nIdx))
'    Set aGUnd = g_Underlying(m_nFilter(nIdx))
'    If Not aGIdx Is Nothing And Not aGUnd Is Nothing Then
'        ' index
'        Set m_Idx(nIndexIdx) = New EtsMmIndexHedgeLib.MmIhIndexAtom
'        m_Idx(nIndexIdx).ID = aGUnd.ID
'        m_Idx(nIndexIdx).Symbol = aGUnd.Symbol
'        m_Idx(nIndexIdx).Yield = aGUnd.Yield
'        m_Idx(nIndexIdx).IsHTB = aGUnd.IsHTB
'        m_Idx(nIndexIdx).IsAmerican = aGUnd.IsAmerican
'        m_Idx(nIndexIdx).Skew = aGUnd.Skew
'        m_Idx(nIndexIdx).Kurt = aGUnd.Kurt
'        Set m_Idx(nIndexIdx).BasketIndex = aGIdx
'
'        Set m_Idx(nIndexIdx).UndPriceProfile = aGUnd.UndPriceProfile
'        Set m_Idx(nIndexIdx).OptPriceProfile = aGUnd.OptPriceProfile
'
'        InitIndexVola nIndexIdx
'
'        sKey = CStr(enCtIndex) & "_" & m_Idx(nIndexIdx).Symbol
'        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, m_Idx(nIndexIdx), Nothing
'
'        ' underlyings
'        For Each aTrd In g_TradeChannel.Trades
'            If CheckUndTradeFilter(aTrd) Then
'                Set aUnd = m_Idx(nIndexIdx).Und(aTrd.UndID)
'                If aUnd Is Nothing Then
'                    Set aUnd = m_Idx(nIndexIdx).Und.Add(aTrd.UndID, aTrd.Und.Symbol)
'                    aUnd.ID = aTrd.Und.ID
'                    aUnd.Symbol = aTrd.Und.Symbol
'                    aUnd.TotalQtyInShares = aTrd.Und.TotalQtyInShares
'                    Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
'
'                    If aUnd.CurShares = BAD_LONG_VALUE Then aUnd.CurShares = 0
'                    aUnd.CurShares = aUnd.CurShares + IIf(aTrd.IsBuy, 1, -1) * aTrd.Quantity * aTrd.LotSize
'
'                    Set aIdxComp = m_Idx(nIndexIdx).BasketIndex.Components(aUnd.ID)
'                    If Not aIdxComp Is Nothing Then
'                        aUnd.Weight = aIdxComp.Weight
'                        Set aIdxComp = Nothing
'                    End If
'
'                    sKey = CStr(enCtStock) & "_" & aUnd.Symbol
'                    Set aReq = m_QuoteRequests(sKey)
'                    If aReq Is Nothing Then Set aReq = m_QuoteRequests.Add(sKey, Nothing, Nothing, Nothing)
'                    Set aReq.Und2(nIndexIdx) = aUnd
'                End If
'            End If
'
'            DoEvents
'            If Not m_bDataLoad Then Exit For
'            IncProgress pbProgress
'        Next
'
'        ' index option trades
'        Set aUndByTrd = TradeChannel.TradesByUnd(CStr(m_Idx(nIndexIdx).ID))
'        If Not aUndByTrd Is Nothing Then
'            For Each aTrd In aUndByTrd.Trd
'                If CheckIdxTradeFilter(aTrd, nIdx) Then
'                    Set aOpt = m_Idx(nIndexIdx).Opt(aTrd.ContractID)
'                    If aOpt Is Nothing Then
'                        Set aOpt = m_Idx(nIndexIdx).Opt.Add(aTrd.ContractID)
'                        aOpt.ID = aTrd.ContractID
'                        aOpt.Symbol = aTrd.Opt.Symbol
'                        aOpt.Expiry = aTrd.Opt.Expiry
'                        aOpt.OptType = aTrd.Opt.OptType
'                        aOpt.Strike = aTrd.Opt.Strike
'
'                        aOpt.Vola = m_Idx(nIndexIdx).VolaSrv.OptionVola(aOpt.Expiry, aOpt.Strike)
'
'                        sKey = CStr(enCtOption) & "_" & aOpt.Symbol
'                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aOpt
'                    End If
'
'                    If aOpt.Qty = BAD_LONG_VALUE Then aOpt.Qty = 0
'                    aOpt.Qty = aOpt.Qty + IIf(aTrd.IsBuy, 1, -1) * aTrd.Quantity * aTrd.LotSize
'                    Set aOpt = Nothing
'                End If
'
'                DoEvents
'                If Not m_bDataLoad Then Exit For
'                IncProgress pbProgress
'            Next
'        End If
'
'        AdjustIndexRates nIndexIdx
'        UnderlyingsLoad = m_bDataLoad
'    End If
'EX:
'    On Error Resume Next
'    Set aOpt = Nothing
'    Set aIdxComp = Nothing
'    Set aGIdx = Nothing
'    Set aGUnd = Nothing
'    Set aUndByTrd = Nothing
'    Exit Function
'EH:
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load stocks."
'    GoTo EX
'End Function
'
'Private Sub UnderlyingsUpdateAll(ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nRow&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    UnderlyingsUpdate GT_INDEXHEDGE2_UNDERLYINGS_1, bReload
'    UnderlyingsUpdate GT_INDEXHEDGE2_UNDERLYINGS_2, bReload
'End Sub
'
'Private Sub UnderlyingsUpdate(nIdx&, ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nRow&, nIndexIdx&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    nIndexIdx = nIdx - GT_INDEXHEDGE2_UNDERLYINGS_1 + 1
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        If bReload Then
'            .Rows = 1
'            If Not m_Idx(nIndexIdx) Is Nothing Then
'                For Each aUnd In m_Idx(nIndexIdx).Und
'                    .AddItem ""
'                    nRow = .Rows - 1
'
'                    .RowData(nRow) = aUnd
'                    .TextMatrix(nRow, JUC_KEY) = 0
'                    UnderlyingUpdate nIdx, nRow, True, aUnd
'                Next
'
'                If .Rows > 1 Then .Row = 1
'
'                .Col = JUC_SYMBOL
'                .Row = 1
'                .Sort = flexSortGenericAscending
'                m_ge(nIdx).ShowSortImage JUC_SYMBOL, 1
'
'                FormatUndColumns nIdx
'            End If
'        Else
'            If Not m_Idx(nIndexIdx) Is Nothing Then
'                For nRow = 1 To .Rows - 1
'                    Set aUnd = .RowData(nRow)
'                    UnderlyingUpdate nIdx, nRow, False, aUnd
'                    Set aUnd = Nothing
'                Next
'            End If
'        End If
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub UnderlyingUpdate(nIdx&, ByVal nRow As Long, ByVal bSymbol As Boolean, ByRef aUnd As EtsMmIndexHedgeLib.MmIhUndAtom)
'    On Error Resume Next
'    Dim nCol&, sValue$
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        If aUnd Is Nothing Then Set aUnd = .RowData(nRow)
'
'        If Not aUnd Is Nothing Then
'            Dim i&, nColIdx&
'            i = 0
'            nColIdx = m_gd(nIdx).Idx(0)
'            While nColIdx >= 0 And i <= JUC_LAST_COLUMN
'                nCol = i + 1
'                If bSymbol Then
'                    Select Case nColIdx
'                        Case JUC_SYMBOL
'                            .TextMatrix(nRow, nCol) = aUnd.Symbol
'
'                        Case JUC_WEIGHT
'                            If aUnd.Weight > BAD_DOUBLE_VALUE Then
'                                .TextMatrix(nRow, nCol) = aUnd.Weight * 100#
'                            Else
'                                .TextMatrix(nRow, nCol) = STR_NA
'                            End If
'
'                    End Select
'                End If
'
'                Select Case nColIdx
'                    Case JUC_PRICE
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceLast > BAD_DOUBLE_VALUE, aUnd.PriceLast, STR_NA)
'
'                    Case JUC_IDX_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexShares1 > BAD_LONG_VALUE, aUnd.IndexShares1, STR_NA)
'
'                End Select
'
'                i = i + 1
'                nColIdx = m_gd(nIdx).Idx(i)
'            Wend
'        End If
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub ResultsUpdate(ByVal bReload As Boolean)
'    On Error Resume Next
'    Dim i&, nRow&, nIdx&
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    nIdx = GT_INDEXHEDGE2_RESULTS
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        If bReload Then
'            .Rows = 1
'            If Not m_Res Is Nothing Then
'                For Each aUnd In m_Res
'                    .AddItem ""
'                    nRow = .Rows - 1
'
'                    .RowData(nRow) = aUnd
'                    .TextMatrix(nRow, JRC_KEY) = 0
'                    ResultUpdate nRow, True, aUnd
'                Next
'
'                If .Rows > 1 Then .Row = 1
'
'                .Col = JRC_SYMBOL
'                .Row = 1
'                .Sort = flexSortGenericAscending
'                m_ge(nIdx).ShowSortImage JRC_SYMBOL, 1
'
'                FormatResColumns
'            End If
'        Else
'            If Not m_Res Is Nothing Then
'                For nRow = 1 To .Rows - 1
'                    Set aUnd = .RowData(nRow)
'                    ResultUpdate nRow, False, aUnd
'                    Set aUnd = Nothing
'                Next
'            End If
'        End If
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub ResultUpdate(ByVal nRow As Long, ByVal bSymbol As Boolean, ByRef aUnd As EtsMmIndexHedgeLib.MmIhUndAtom)
'    On Error Resume Next
'    Dim nCol&, sValue$, nIdx&
'
'    nIdx = GT_INDEXHEDGE2_RESULTS
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        If aUnd Is Nothing Then Set aUnd = .RowData(nRow)
'
'        If Not aUnd Is Nothing Then
'            Dim i&, nColIdx&
'            i = 0
'            nColIdx = m_gd(nIdx).Idx(0)
'            While nColIdx >= 0 And i <= JRC_LAST_COLUMN
'                nCol = i + 1
'                If bSymbol Then
'                    Select Case nColIdx
'                        Case JRC_SYMBOL
'                            .TextMatrix(nRow, nCol) = aUnd.Symbol
'
'                        Case JRC_CUR_SHARES
'                            .TextMatrix(nRow, nCol) = IIf(aUnd.CurShares > BAD_LONG_VALUE, aUnd.CurShares, STR_NA)
'
'                    End Select
'                End If
'
'                Select Case nColIdx
'                    Case JRC_IDX1_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexShares1 > BAD_LONG_VALUE, aUnd.IndexShares1, STR_NA)
'
'                    Case JRC_IDX2_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexShares2 > BAD_LONG_VALUE, aUnd.IndexShares2, STR_NA)
'
'                    Case JRC_TXN_SHARES
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.IndexTxn > BAD_LONG_VALUE, aUnd.IndexTxn, STR_NA)
'
'                    Case JRC_TRANSACT
'                        .TextMatrix(nRow, nCol) = IIf(aUnd.Transact > BAD_LONG_VALUE, aUnd.Transact, STR_NA)
'
'                End Select
'
'                i = i + 1
'                nColIdx = m_gd(nIdx).Idx(i)
'            Wend
'        End If
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub AdjustIndexRates(nIndexIdx&)
'    On Error Resume Next
'    If m_Idx(nIndexIdx) Is Nothing Then Exit Sub
'
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
'    Dim aUnd As EtsGeneralLib.UndAtom, bForceRateUpdate As Boolean
'
'    Set aUnd = g_UnderlyingAll(m_Idx(nIndexIdx).ID)
'    If aUnd Is Nothing Then Exit Sub
'
'    dPos = aUnd.UndPosForRates
'
'    If GetIrRuleType = enRateBasedOnPosition Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'    Else
'        bUseMidRates = True
'    End If
'
'    For Each aOpt In m_Idx(nIndexIdx).Opt
'        If bUseMidRates Then
'            If Not m_Idx(nIndexIdx).IsHTB Then
'                aOpt.Rate = GetNeutralRate(Date, aOpt.Expiry)
'            Else
'                aOpt.Rate = GetNeutralHTBRate(Date, aOpt.Expiry)
'            End If
'        Else
'            If Not m_Idx(nIndexIdx).IsHTB Then
'                aOpt.Rate = IIf(dPos < 0#, GetShortRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'            Else
'                aOpt.Rate = IIf(dPos < 0#, GetHTBRate(Date, aOpt.Expiry), GetLongRate(Date, aOpt.Expiry))
'            End If
'        End If
'    Next
'
'    Set aUnd = Nothing
'End Sub
'
'Private Sub RequestLastQuotes()
'    On Error GoTo EH
'    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aReq As clsIhRequestAtom
'    Dim nCount&
'
'    If Not PriceProvider Is Nothing Then
'        Debug.Assert Not m_bLastQuoteReqNow
'
'        nCount = m_QuoteRequests.Count
'        If nCount > 0 Then
'            m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.RiskReqTypeAlwaysNonGroup
'
'            m_bInProc = True
'            m_bLastQuoteReqNow = True
'            AdjustState
'
'            m_nQuoteReqCount = nCount
'            m_nQuoteReqDone = 0&
'
'            pbProgress.Min = 0
'            pbProgress.Value = 0
'            pbProgress.Max = m_nQuoteReqCount
'            pbProgress.Visible = True
'
'            lblStatus.Visible = False
'            lblProcess.Visible = True
'            lblProcess.Caption = "Last quotes request..."
'            imgStop.Visible = True
'            imgStopDis.Visible = False
'            DoEvents
'
'            If Not m_bGroupRequest Then
'                m_nQuoteGroupReqCount = 0&
'                m_nQuoteGroupReqDone = 0&
'
'                aParam.Type = enMStart
'                PriceProvider.RequestLastQuote aParam
'
'                For Each aReq In m_QuoteRequests
'                    If Not m_bLastQuoteReqNow Then Exit For
'                    PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
'                    DoEvents
'                Next
'
'                aParam.Type = enMStop
'                PriceProvider.RequestLastQuote aParam
'            Else
'                m_nQuoteGroupReqCount = 0&
'                m_nQuoteGroupReqDone = 0&
'
'                For Each aReq In m_QuoteRequests
'                    If Not m_bLastQuoteReqNow Then Exit For
'
'                    aParam = aReq.GetQuoteUpdateParam
'                    If aParam.Type = enSTK Or aParam.Type = enIDX Then
'                        If aReq.Idx Is Nothing And aReq.Opt Is Nothing Then
'                            If Not m_bLastQuoteReqNow Then Exit For
'                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                            PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
'                            DoEvents
'                        Else
'                            PriceProvider.RequestLastQuote aParam
'                            m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                            DoEvents
'
'                            If Not m_bLastQuoteReqNow Then Exit For
'
'                            If aReq.Idx.Opt.Count > 0 Then
'                                aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
'                                m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
'                                m_nQuoteGroupReqCount = m_nQuoteGroupReqCount + 1
'                                DoEvents
'                            End If
'                        End If
'                    End If
'                Next
'            End If
'
'            m_bInProc = False
'            AdjustState
'        Else
'            pbProgress.Visible = False
'            lblStatus.Visible = True
'            lblProcess.Visible = False
'            lblProcess.Caption = ""
'            imgStop.Visible = False
'            imgStopDis.Visible = False
'        End If
'    Else
'        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
'    End If
'    Exit Sub
'EH:
'    m_bLastQuoteReqNow = False
'    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
'    On Error Resume Next
'    pbProgress.Visible = False
'    lblStatus.Visible = True
'    lblProcess.Visible = False
'    lblProcess.Caption = ""
'    imgStop.Visible = False
'    imgStopDis.Visible = False
'    m_bInProc = False
'    AdjustState
'
'    PriceProvider.CancelLastQuote
'    If m_bGroupRequest Then
'        m_GroupPriceProvider.CancelGroup
'    End If
'End Sub
'
'Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'    Dim aReq As clsIhRequestAtom, bFinished As Boolean
'    Dim aRequest As PRICEPROVIDERSLib.QuoteUpdateParams, sKey$
'
'    aRequest = Request
'    bFinished = False
'
'    If Not Request Is Nothing And ReqType = enRequestLastQuote Then
'        If m_bLastQuoteReqNow Then
'            If PpIsNonGroupReqType(Request.Type) Then
'
'                sKey = CStr(IIf(aRequest.Type = enOPT, enCtOption, IIf(aRequest.Type = enSTK, enCtStock, enCtIndex))) & "_" & aRequest.Symbol
'
'                Set aReq = m_QuoteRequests(sKey)
'                If Not aReq Is Nothing Then
'                    LogEvent EVENT_WARNING, aRequest.Symbol & ": " & Description
'
'                    m_nQuoteReqDone = m_nQuoteReqDone + 1
'                    IncProgress pbProgress
'
'                    If m_bGroupRequest And aRequest.Type <> enOPT Then
'                        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'                    End If
'                    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
'                                Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'                End If
'
'            ElseIf PpIsGroupReqType(Request.Type) Then
'                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
'                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'                bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'            End If
'
'            If bFinished Then
'                m_bLastQuoteReqNow = False
'
'                PriceProvider.CancelLastQuote
'                If m_bGroupRequest Then
'                    m_GroupPriceProvider.CancelGroup
'                End If
'
'                CalcIndices
'                UnderlyingsUpdateAll False
'                ResultsUpdate False
'                UpdateTotalsAll
'
'                pbProgress.Visible = False
'                lblStatus.Visible = True
'                lblProcess.Visible = False
'                imgStop.Visible = False
'                imgStopDis.Visible = False
'
'                AdjustState
'            End If
'        Else
' '           Debug.Assert False
'        End If
'
'    Else
'        LogEvent EVENT_ERROR, Description
'    End If
'End Sub
'
'Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
'    On Error Resume Next
'    If Not m_bLastQuoteReqNow Then Exit Sub
'
'    Dim aReq As clsIhRequestAtom, sKey$
'    Dim aUnd1 As EtsMmIndexHedgeLib.MmIhUndAtom, aUnd2 As EtsMmIndexHedgeLib.MmIhUndAtom, aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim dPriceBid#, dPriceAsk#, dPriceLast#, aIdx As EtsMmIndexHedgeLib.MmIhIndexAtom
'    Dim bFinished As Boolean
'    bFinished = False
'
'    If PpIsNonGroupReqType(Params.Type) Then
'        sKey = CStr(IIf(Params.Type = enOPT, enCtOption, IIf(Params.Type = enSTK, enCtStock, enCtIndex))) & "_" & Params.Symbol
'        Set aReq = m_QuoteRequests(sKey)
'
'        If Not aReq Is Nothing Then
'            dPriceBid = Results.BidPrice
'            dPriceAsk = Results.AskPrice
'            dPriceLast = Results.LastPrice
'
'            Set aOpt = aReq.Opt
'            Set aUnd1 = aReq.Und2(1)
'            Set aUnd2 = aReq.Und2(2)
'            Set aIdx = aReq.Idx
'
'            If Not aOpt Is Nothing Then
'                aOpt.PriceBid = dPriceBid
'                aOpt.PriceAsk = dPriceAsk
'                aOpt.PriceLast = dPriceLast
'
'            ElseIf Not aIdx Is Nothing Then
'                aIdx.PriceBid = dPriceBid
'                aIdx.PriceAsk = dPriceAsk
'                aIdx.PriceLast = dPriceLast
'
'            Else
'                If Not aUnd1 Is Nothing Then
'                    aUnd1.PriceBid = dPriceBid
'                    aUnd1.PriceAsk = dPriceAsk
'                    aUnd1.PriceLast = dPriceLast
'                End If
'
'                If Not aUnd2 Is Nothing Then
'                    aUnd2.PriceBid = dPriceBid
'                    aUnd2.PriceAsk = dPriceAsk
'                    aUnd2.PriceLast = dPriceLast
'                End If
'            End If
'
'            Set aOpt = Nothing
'            Set aUnd1 = Nothing
'            Set aUnd2 = Nothing
'            Set aIdx = Nothing
'
'            m_nQuoteReqDone = m_nQuoteReqDone + 1
'            IncProgress pbProgress
'            Set aReq = Nothing
'
'            If m_bGroupRequest And Params.Type <> enOPT Then
'                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'            End If
'            bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
'                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'        End If
'    ElseIf PpIsGroupReqType(Params.Type) Then
'        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
'        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
'    End If
'
'    If bFinished Then
'        m_bLastQuoteReqNow = False
'
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'
'        CalcIndices
'        UnderlyingsUpdateAll False
'        ResultsUpdate False
'        UpdateTotalsAll
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'
'        AdjustState
'    End If
'End Sub
'
'Private Function CheckIdxTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom, nFilterIdx&) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    bMatched = (m_nFilter(nFilterIdx) = aTrd.UndID And aTrd.ContractType = enCtOption)
'
'    If bMatched Then
'        nValue = m_nFilter(JFC_TRADER)
'        bMatched = (aTrd.Trader.ID = nValue)
'    End If
'
'    If bMatched Then
'        nValue = m_nFilter(JFC_STRATEGY)
'        bMatched = (aTrd.StrategyID = nValue)
'    End If
'
'    CheckIdxTradeFilter = bMatched
'End Function
'
'Private Function CheckUndTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    bMatched = (aTrd.Und.UndType = enCtStock And aTrd.FutRootID = 0)
'
'    If bMatched Then
'        nValue = m_nFilter(JFC_TRADER)
'        bMatched = (aTrd.Trader.ID = nValue)
'    End If
'
'    If bMatched Then
'        nValue = m_nFilter(JFC_STRATEGY)
'        bMatched = (aTrd.StrategyID = nValue)
'    End If
'
'    CheckUndTradeFilter = bMatched
'End Function
'
'Private Sub FilterUpdate()
'    On Error Resume Next
'    Dim nValue&
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE2_FILTER).LockRedraw
'
'        nValue = m_nFilter(JFC_TRADER)
'        If g_Trader(nValue) Is Nothing Then nValue = 0
'        m_nFilter(JFC_TRADER) = nValue
'        .TextMatrix(1, JFC_TRADER) = nValue
'
'        nValue = m_nFilter(JFC_STRATEGY)
'        If nValue > 0 Then _
'            If g_Strategy(nValue) Is Nothing Then nValue = 0
'        m_nFilter(JFC_STRATEGY) = nValue
'        .TextMatrix(1, JFC_STRATEGY) = nValue
'
'        nValue = m_nFilter(JFC_INDEX1)
'        If g_BasketIndex(nValue) Is Nothing Then nValue = 0
'        m_nFilter(JFC_INDEX1) = nValue
'        .TextMatrix(1, JFC_INDEX1) = nValue
'
'        nValue = m_nFilter(JFC_INDEX2)
'        If g_BasketIndex(nValue) Is Nothing Then nValue = 0
'        m_nFilter(JFC_INDEX2) = nValue
'        .TextMatrix(1, JFC_INDEX2) = nValue
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE2_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitFltData()
'    On Error Resume Next
'    Dim i&, nCount&, sComboList$, sValue$, aIdx As EtsGeneralLib.IndexAtom, nValue&
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE2_FILTER).LockRedraw
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<Select>"
'        For Each aTrader In g_Trader
'            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
'            If aTrader.ID = m_nFilter(JFC_TRADER) Then
'                nValue = aTrader.ID
'                sValue = Trim$(Str$(nValue))
'            End If
'        Next
'        .ColComboList(JFC_TRADER) = sComboList
'        .TextMatrix(1, JFC_TRADER) = sValue
'        m_nFilter(JFC_TRADER) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = "#0;<Select>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'        If m_nFilter(JFC_STRATEGY) = NO_STRATEGY_ID Then
'            nValue = NO_STRATEGY_ID
'            sValue = NO_STRATEGY_NAME
'        End If
'
'        For Each aStrategy In g_Strategy
'            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'            If aStrategy.ID = m_nFilter(JFC_STRATEGY) Then
'                nValue = aStrategy.ID
'                sValue = Trim$(Str$(aStrategy.ID))
'            End If
'        Next
'        .ColComboList(JFC_STRATEGY) = sComboList
'        .TextMatrix(1, JFC_STRATEGY) = sValue
'        m_nFilter(JFC_STRATEGY) = nValue
'
'        nValue = 0
'        sValue = CStr(nValue)
'        sComboList = g_Params.BasketIndexComboList
'        If Len(sComboList) > 0 Then
'            sComboList = "#0;<Select>|" & sComboList
'        Else
'            sComboList = "#0;<Select>"
'        End If
'
'        Set aIdx = g_BasketIndex(m_nFilter(JFC_INDEX1))
'        If Not aIdx Is Nothing Then
'            nValue = aIdx.ID
'            sValue = Trim$(Str$(aIdx.ID))
'            Set aIdx = Nothing
'        End If
'        .ColComboList(JFC_INDEX1) = sComboList
'        .TextMatrix(1, JFC_INDEX1) = sValue
'        m_nFilter(JFC_INDEX1) = nValue
'        Set aIdx = Nothing
'
'        nValue = 0
'        sValue = CStr(nValue)
'        Set aIdx = g_BasketIndex(m_nFilter(JFC_INDEX2))
'        If Not aIdx Is Nothing Then
'            nValue = aIdx.ID
'            sValue = Trim$(Str$(aIdx.ID))
'            Set aIdx = Nothing
'        End If
'        .ColComboList(JFC_INDEX2) = sComboList
'        .TextMatrix(1, JFC_INDEX2) = sValue
'        m_nFilter(JFC_INDEX2) = nValue
'        Set aIdx = Nothing
'
'        m_GridLock(GT_INDEXHEDGE2_FILTER).UnlockRedraw
'    End With
'End Sub
'
'Private Sub InitColumns()
'    On Error Resume Next
'    Dim i&
'    Set m_gd(GT_INDEXHEDGE2_FILTER) = New clsGridDef
'    Set m_gd(GT_INDEXHEDGE2_TOTALS_1) = New clsGridDef
'    Set m_gd(GT_INDEXHEDGE2_UNDERLYINGS_1) = New clsGridDef
'    Set m_gd(GT_INDEXHEDGE2_TOTALS_2) = New clsGridDef
'    Set m_gd(GT_INDEXHEDGE2_UNDERLYINGS_2) = New clsGridDef
'    Set m_gd(GT_INDEXHEDGE2_RESULTS) = New clsGridDef
'
'    For i = GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS
'        g_DefGrids(i).CopyTo m_gd(i)
'    Next
'
'    'g_DefGrids(GT_INDEXHEDGE2_FILTER).CopyTo m_gdFlt
'    'g_DefGrids(GT_INDEXHEDGE2_TOTALS_1).CopyTo m_gdTot1
'    'g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_1).CopyTo m_gdUnd1
'End Sub
'
'Private Sub InitGrids()
'    On Error Resume Next
'    Dim i&
'
'    Set m_fg(GT_INDEXHEDGE2_FILTER) = fgFlt
'    Set m_fg(GT_INDEXHEDGE2_TOTALS_1) = fgTot1
'    Set m_fg(GT_INDEXHEDGE2_UNDERLYINGS_1) = fgUnd1
'    Set m_fg(GT_INDEXHEDGE2_TOTALS_2) = fgTot2
'    Set m_fg(GT_INDEXHEDGE2_UNDERLYINGS_2) = fgUnd2
'    Set m_fg(GT_INDEXHEDGE2_RESULTS) = fgRes
'
'    For i = GT_INDEXHEDGE2_FILTER To GT_INDEXHEDGE2_RESULTS
'        m_bKeyDown(i) = False
'        m_GridLock(i).Init m_fg(i)
'    Next
'
'    With fgFlt
'        .Rows = 2
'        .Cols = JFC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .AutoSizeMode = flexAutoSizeColWidth
'        .Appearance = flex3DLight
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbdMouse
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgTot1
'        .Rows = 2
'        .Cols = JTC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbdMouse
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgTot2
'        .Rows = 2
'        .Cols = JTC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDKbdMouse
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .ScrollBars = flexScrollBarNone
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'
'        .HighLight = flexHighlightWithFocus
'    End With
'
'    With fgUnd1
'        .Rows = 1
'        .Cols = JUC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSort
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .OutlineCol = 1
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionByRow
'        .TabBehavior = flexTabControls
'    End With
'
'    With fgUnd2
'        .Rows = 1
'        .Cols = JUC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSort
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .OutlineCol = 1
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionByRow
'        .TabBehavior = flexTabControls
'    End With
'
'    With fgRes
'        .Rows = 1
'        .Cols = JRC_COLUMN_COUNT
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeColumns
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = True
'        .Editable = flexEDNone
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExSort
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusHeavy
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarNone
'        .OutlineCol = 1
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionByRow
'        .TabBehavior = flexTabControls
'    End With
'End Sub
'
'Private Sub FormatFltGrid()
'    On Error Resume Next
'    Dim i&, nCols&, nIdx&
'    nIdx = GT_INDEXHEDGE2_FILTER
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        nCols = JFC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            .TextMatrix(0, i) = m_gd(nIdx).Col(i).Caption
'            .ColFormat(i) = m_gd(nIdx).Col(i).Format
'            .ColDataType(i) = m_gd(nIdx).Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatFltColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont, nIdx&
'    nIdx = GT_INDEXHEDGE2_FILTER
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        aFont.Name = m_gd(nIdx).Font.Name
'        aFont.Size = m_gd(nIdx).Font.Size
'        aFont.Bold = m_gd(nIdx).Font.Bold
'        aFont.Italic = m_gd(nIdx).Font.Italic
'        aFont.Strikethrough = m_gd(nIdx).Font.Strikethrough
'        aFont.Underline = m_gd(nIdx).Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gd(nIdx).Col(i).BackColor <> 0, m_gd(nIdx).Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gd(nIdx).Col(i).ForeColor <> 0, m_gd(nIdx).Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If m_gd(nIdx).HorizLinesVisible Then
'            If m_gd(nIdx).VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf m_gd(nIdx).VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = m_gd(nIdx).GridLinesColor
'
'        .BackColorSel = m_gd(nIdx).BackColorSel
'        .ForeColorSel = m_gd(nIdx).ForeColorSel
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotGrid(ByVal nIdx&)
'    On Error Resume Next
'    Dim i&, nCols&, gd As clsGridDef
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        nCols = JTC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = JTC_INDEX To JTC_LAST_COLUMN
'            .TextMatrix(0, i) = gd.Col(i).Caption
'            .ColFormat(i) = gd.Col(i).Format
'            .ColDataType(i) = gd.Col(i).DataType
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatTotColumns(ByVal nIdx&)
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont, gd As clsGridDef
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        aFont.Name = gd.Font.Name
'        aFont.Size = gd.Font.Size
'        aFont.Bold = gd.Font.Bold
'        aFont.Italic = gd.Font.Italic
'        aFont.Strikethrough = gd.Font.Strikethrough
'        aFont.Underline = gd.Font.Underline
'
'        Set .Font = aFont
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = JTC_INDEX To JTC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gd.Col(i).BackColor <> 0, gd.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gd.Col(i).ForeColor <> 0, gd.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If gd.HorizLinesVisible Then
'            If gd.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf gd.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = gd.GridLinesColor
'
'        .BackColorSel = gd.BackColorSel
'        .ForeColorSel = gd.ForeColorSel
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatUndGrid(ByVal nIdx&)
'    On Error Resume Next
'    Dim i&, nCols&, gd As clsGridDef
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        nCols = JUC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = JUC_SYMBOL To JUC_LAST_COLUMN
'            .TextMatrix(0, i) = gd.Col(i).Caption
'            .ColFormat(i) = gd.Col(i).Format
'            .ColDataType(i) = gd.Col(i).DataType
'            .ColWidth(i) = IIf(gd.Col(i).Width > 0, gd.Col(i).Width, -1)
'            .ColKey(i) = i
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        m_ge(nIdx).AdjustSortImages
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatUndColumns(ByVal nIdx&)
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont, gd As clsGridDef
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        aFont.Name = gd.Font.Name
'        aFont.Size = gd.Font.Size
'        aFont.Bold = gd.Font.Bold
'        aFont.Italic = gd.Font.Italic
'        aFont.Strikethrough = gd.Font.Strikethrough
'        aFont.Underline = gd.Font.Underline
'
'        Set .Font = aFont
'
'        .ColHidden(JUC_KEY) = True
'        .ColDataType(JUC_KEY) = flexDTString
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = JUC_SYMBOL To JUC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gd.Col(i).BackColor <> 0, gd.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gd.Col(i).ForeColor <> 0, gd.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If gd.HorizLinesVisible Then
'            If gd.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf gd.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = gd.GridLinesColor
'
'        .BackColorSel = gd.BackColorSel
'        .ForeColorSel = gd.ForeColorSel
'
'        '.AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatResGrid()
'    On Error Resume Next
'    Dim i&, nCols&, gd As clsGridDef, nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        nCols = JRC_COLUMN_COUNT
'        .Cols = nCols
'
'        For i = JRC_SYMBOL To JRC_LAST_COLUMN
'            .TextMatrix(0, i) = gd.Col(i).Caption
'            .ColFormat(i) = gd.Col(i).Format
'            .ColDataType(i) = gd.Col(i).DataType
'            .ColWidth(i) = IIf(gd.Col(i).Width > 0, gd.Col(i).Width, -1)
'            .ColKey(i) = i
'        Next
'
'        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
'        m_ge(nIdx).AdjustSortImages
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub FormatResColumns()
'    On Error Resume Next
'    Dim i&, nCols&, nRows&, aFont As New StdFont, gd As clsGridDef, nIdx&: nIdx = GT_INDEXHEDGE2_RESULTS
'
'    Set gd = m_gd(nIdx)
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        aFont.Name = gd.Font.Name
'        aFont.Size = gd.Font.Size
'        aFont.Bold = gd.Font.Bold
'        aFont.Italic = gd.Font.Italic
'        aFont.Strikethrough = gd.Font.Strikethrough
'        aFont.Underline = gd.Font.Underline
'
'        Set .Font = aFont
'
'        .ColHidden(JRC_KEY) = True
'        .ColDataType(JRC_KEY) = flexDTString
'
'        nCols = .Cols - 1
'        nRows = .Rows - 1
'        For i = JRC_SYMBOL To JRC_LAST_COLUMN
'            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gd.Col(i).BackColor <> 0, gd.Col(i).BackColor, RGB(1, 1, 1))
'            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gd.Col(i).ForeColor <> 0, gd.Col(i).ForeColor, RGB(1, 1, 1))
'        Next
'
'        If gd.HorizLinesVisible Then
'            If gd.VertLinesVisible Then
'                .GridLines = flexGridFlat
'            Else
'                .GridLines = flexGridFlatHorz
'            End If
'        ElseIf gd.VertLinesVisible Then
'            .GridLines = flexGridFlatVert
'        Else
'            .GridLines = flexGridNone
'        End If
'        .GridColor = gd.GridLinesColor
'
'        .BackColorSel = gd.BackColorSel
'        .ForeColorSel = gd.ForeColorSel
'
'        '.AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub aParams_PriceProfilesChange()
'    SetRefreshHint True
'End Sub
'
'Private Sub aParams_PriceRoundingRuleChange()
'    SetRefreshHint True
'End Sub
'
'Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As clsFilterAtomColl)
'    On Error Resume Next
'    If m_Idx(1) Is Nothing Or m_Idx(2) Is Nothing Then Exit Sub
'    Dim aFilterAtom As clsFilterAtom
'
'    For Each aFilterAtom In collUpdUnd
'        If m_Idx(1).ID = aFilterAtom.ID Or m_Idx(2).ID = aFilterAtom.ID Then
'            SetRefreshHint True
'            Exit For
'        End If
'    Next
'End Sub
'
'Private Sub aParams_UndPriceToleranceChange()
'    SetRefreshHint True
'End Sub
'
'Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
'    On Error Resume Next
'    Dim aIdx As EtsGeneralLib.IndexAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(JFC_INDEX1) <> nNewUndID Or m_nFilter(JFC_INDEX2) <> nNewUndID Then
'        Set aIdx = g_BasketIndex(nNewUndID)
'        If Not aIdx Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_INDEXHEDGE2_FILTER).LockRedraw
'
'                nValue = 0
'                sValue = CStr(nValue)
'                sComboList = g_Params.BasketIndexComboList
'                If Len(sComboList) > 0 Then
'                    sComboList = "#0;<Select>|" & sComboList
'                Else
'                    sComboList = "#0;<Select>"
'                End If
'
'                Set aIdx = g_BasketIndex(m_nFilter(JFC_INDEX1))
'                If Not aIdx Is Nothing Then
'                    nValue = aIdx.ID
'                    sValue = Trim$(Str$(aIdx.ID))
'                    Set aIdx = Nothing
'                End If
'                .ColComboList(JFC_INDEX1) = sComboList
'                .TextMatrix(1, JFC_INDEX1) = sValue
'                m_nFilter(JFC_INDEX1) = nValue
'                Set aIdx = Nothing
'
'                nValue = 0
'                sValue = CStr(nValue)
'                Set aIdx = g_BasketIndex(m_nFilter(JFC_INDEX2))
'                If Not aIdx Is Nothing Then
'                    nValue = aIdx.ID
'                    sValue = Trim$(Str$(aIdx.ID))
'                    Set aIdx = Nothing
'                End If
'                .ColComboList(JFC_INDEX2) = sComboList
'                .TextMatrix(1, JFC_INDEX2) = sValue
'                m_nFilter(JFC_INDEX2) = nValue
'                Set aIdx = Nothing
'
'                m_GridLock(GT_INDEXHEDGE2_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
'    On Error Resume Next
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, sComboList$, sValue$, nValue&
'    If m_nFilter(JFC_STRATEGY) <> nStrategyID Then
'        Set aStrategy = g_Strategy(nStrategyID)
'        If Not aStrategy Is Nothing Then
'            With fgFlt
'                m_GridLock(GT_INDEXHEDGE2_FILTER).LockRedraw
'
'                sValue = "0"
'                nValue = 0
'                sComboList = "#0;<Select>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
'                If m_nFilter(JFC_STRATEGY) = NO_STRATEGY_ID Then
'                    nValue = NO_STRATEGY_ID
'                    sValue = NO_STRATEGY_NAME
'                End If
'
'                For Each aStrategy In g_Strategy
'                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
'                    If aStrategy.ID = m_nFilter(JFC_STRATEGY) Then
'                        nValue = aStrategy.ID
'                        sValue = Trim$(Str$(aStrategy.ID))
'                    End If
'                Next
'                .ColComboList(JFC_STRATEGY) = sComboList
'                .TextMatrix(1, JFC_STRATEGY) = sValue
'                m_nFilter(JFC_STRATEGY) = nValue
'
'                m_GridLock(GT_INDEXHEDGE2_FILTER).UnlockRedraw
'            End With
'        End If
'    End If
'End Sub
'
'Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    Dim sValue$, nValue&
'
'    With fgFlt
'        sValue = Trim$(.TextMatrix(Row, Col))
'        If m_sCurrentOriginalText <> sValue Then
'            nValue = CLng(sValue)
'            If Err.Number <> 0 Then nValue = Val(sValue)
'            If Err.Number = 0 Then
'                m_nFilter(Col) = nValue
'                .AutoSize 0, .Cols - 1, , 100
'            End If
'
'            FilterUpdate
'            FilterUpdateChangedStatus
'            AdjustCaption
'            AdjustState
'        End If
'    End With
'End Sub
'
'Private Sub fgFlt_DblClick()
'    On Error Resume Next
'    With fgFlt
'        m_enMenuGrid = GT_INDEXHEDGE2_FILTER
'        m_nMenuGridCol = .MouseCol
'        m_nMenuGridRow = .MouseRow
'        m_nMenuGridCols = .Cols
'        m_nMenuGridRows = .Rows
'
'        HandleGridDblClick False
'    End With
'End Sub
'
'Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If IsHotKeyGridClipOrTrade(KeyCode, Shift) Then m_bKeyDown(GT_INDEXHEDGE2_FILTER) = True
'End Sub
'
'Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown(GT_INDEXHEDGE2_FILTER) Then
'        m_bKeyDown(GT_INDEXHEDGE2_FILTER) = False
'        With fgFlt
'            m_enMenuGrid = GT_INDEXHEDGE2_FILTER
'            m_nMenuGridCol = .Col
'            m_nMenuGridRow = .Row
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'        End With
'
'        Select Case True
'            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
'                mnuCtxCopy_Click
'
'            Case KeyCode = vbKeyInsert And Shift = 0
'                mnuCtxTradeNew_Click
'        End Select
'    End If
'End Sub
'
'Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = IsDblClickHandled Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
'    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgFlt.TextMatrix(Row, Col))
'End Sub
'
'Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        With fgFlt
'            m_enMenuGrid = GT_INDEXHEDGE2_FILTER
'            m_nMenuGridCol = .MouseCol
'            m_nMenuGridRow = .MouseRow
'            m_nMenuGridCols = .Cols
'            m_nMenuGridRows = .Rows
'            ShowPopup
'        End With
'    End If
'End Sub
'
'Private Sub ShowPopup()
'    On Error Resume Next
'    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
'
'    mnuCtxCopy.Enabled = Not m_bInProc
'
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE2_FILTER
'            mnuCtxTradeNew.Enabled = (Not m_bInProc And (m_nFilter(JFC_INDEX1) <> 0 Or m_nFilter(JFC_INDEX2) <> 0) _
'                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_INDEXHEDGE2_TOTALS_1, GT_INDEXHEDGE2_TOTALS_2
'            mnuCtxTradeNew.Enabled = False
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = False
'            mnuCtxAutosizeGrid.Enabled = False
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1, GT_INDEXHEDGE2_UNDERLYINGS_2, GT_INDEXHEDGE2_RESULTS
'            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxPositionTransfer.Enabled = mnuCtxTradeNew.Enabled
'
'            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols)
'            mnuCtxAutosizeGrid.Enabled = True
'
'            PopupMenu mnuCtx, , , , mnuCtxTradeNew
'
'    End Select
'End Sub
'
'Private Sub ResetMenuData()
'    On Error Resume Next
'    m_enMenuGrid = -1
'    m_nMenuGridCol = -1
'    m_nMenuGridRow = -1
'    m_nMenuGridCols = 0
'    m_nMenuGridRows = 0
'End Sub
'
'Private Sub frmLayout_OnApply(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    SetGridLayout enGridType, gdGrid
'End Sub
'
'Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    SetGridLayout enGridType, gdGrid
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    Select Case enGridType
'        Case GT_INDEXHEDGE2_FILTER
'            gdGrid.CopyTo m_gd(enGridType)
'
'            m_GridLock(enGridType).LockRedraw
'            FormatFltGrid
'            FormatFltColumns
'            m_GridLock(enGridType).UnlockRedraw
'
'        Case GT_INDEXHEDGE2_TOTALS_1, GT_INDEXHEDGE2_TOTALS_2
'            gdGrid.CopyTo m_gd(enGridType)
'
'            m_GridLock(enGridType).LockRedraw
'            FormatTotGrid enGridType
'            FormatTotColumns enGridType
'            m_GridLock(enGridType).UnlockRedraw
'
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1, GT_INDEXHEDGE2_UNDERLYINGS_2
'            gdGrid.CopyTo m_gd(enGridType)
'
'            m_GridLock(enGridType).LockRedraw
'            FormatUndGrid enGridType
'            FormatUndColumns enGridType
'            m_GridLock(enGridType).UnlockRedraw
'
'        Case GT_INDEXHEDGE2_RESULTS
'            gdGrid.CopyTo m_gd(enGridType)
'
'            m_GridLock(enGridType).LockRedraw
'            FormatResGrid
'            FormatResColumns
'            m_GridLock(enGridType).UnlockRedraw
'
'    End Select
'
'    UserControl_Resize
'End Sub
'
'Private Sub imgStop_Click()
'    On Error Resume Next
'    If m_bLastQuoteReqNow Then
'        m_bLastQuoteReqNow = False
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then m_GroupPriceProvider.CancelGroup
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bDataLoad Then
'        m_bDataLoad = False
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bRecalc Then
'        m_bRecalc = False
'
'        pbProgress.Visible = False
'        lblStatus.Visible = True
'        lblProcess.Visible = False
'        imgStop.Visible = False
'        imgStopDis.Visible = False
'        AdjustState
'
'    ElseIf m_bInProc Then
'        m_bInProc = False
'        AdjustState
'    End If
'End Sub
'
'Private Sub mnuCtxCopy_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'
'    Screen.MousePointer = vbHourglass
'    g_ClipMgr.CopyGridToClipboard m_fg(m_enMenuGrid)
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuCtxGridLayout_Click()
'    On Error Resume Next
'    Select Case m_enMenuGrid
'        Case GT_INDEXHEDGE2_FILTER
'            CustomizeFltGridLayout
'
'        Case GT_INDEXHEDGE2_TOTALS_1, GT_INDEXHEDGE2_TOTALS_2
'            CustomizeTotGridLayout m_enMenuGrid
'
'        Case GT_INDEXHEDGE2_UNDERLYINGS_1, GT_INDEXHEDGE2_UNDERLYINGS_2
'            CustomizeUndGridLayout m_enMenuGrid
'
'        Case GT_INDEXHEDGE2_RESULTS
'            CustomizeResGridLayout
'    End Select
'End Sub
'
'Private Sub mnuCtxRefresh_Click()
'    Refresh
'End Sub
'
'Private Sub UserControl_Hide()
'    On Error Resume Next
'    m_bDataLoad = False
'    m_bLastQuoteReqNow = False
'End Sub
'
'Private Sub UserControl_Resize()
'    On Error Resume Next
'    If m_bShutDown Or gCmn Is Nothing Then Exit Sub
'
'    With fgFlt
'        .Top = 0
'        .Left = 0
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = ScaleWidth
'    End With
'
'    With fgTot1
'        .Top = fgFlt.Height + GRID_VERT_SPACE
'        .Left = 0
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = (ScaleWidth - GRID_VERT_SPACE) / 2
'    End With
'
'    With fgTot2
'        .Top = fgFlt.Height + GRID_VERT_SPACE
'        .Left = fgTot1.Width + GRID_VERT_SPACE
'        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
'        .Width = (ScaleWidth - GRID_VERT_SPACE) / 2
'    End With
'
'    With fgUnd1
'        .Left = 0
'        .Top = fgFlt.Height + fgTot1.Height + GRID_VERT_SPACE * 2
'        .Height = (ScaleHeight - .Top - GRID_VERT_SPACE) / 2
'        .Width = (ScaleWidth - GRID_VERT_SPACE) / 2
'    End With
'
'    With fgUnd2
'        .Left = fgUnd1.Width + GRID_VERT_SPACE
'        .Top = fgUnd1.Top
'        .Height = fgUnd1.Height
'        .Width = fgUnd1.Width
'    End With
'
'    With fgRes
'        .Left = 0
'        .Top = fgFlt.Height + fgTot1.Height + fgUnd1.Height + GRID_VERT_SPACE * 3
'        .Width = ScaleWidth
'        .Height = ScaleHeight - .Top
'    End With
'End Sub
'
'Private Sub UserControl_Terminate()
'    On Error Resume Next
'    Term
'End Sub
'
'Public Sub Term()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'
'    m_bShutDown = True
'
'    Set pbProgress = Nothing
'    Set lblProcess = Nothing
'    Set lblStatus = Nothing
'
'    If Not PriceProvider Is Nothing Then
'        PriceProvider.CancelLastQuote
'        If m_bGroupRequest Then
'            m_GroupPriceProvider.CancelGroup
'        End If
'        Set PriceProvider = Nothing
'    End If
'
'    ClearViewAndData
'
'    Set m_GroupPriceProvider = Nothing
'    Set m_Idx(1) = Nothing
'    Set m_Idx(2) = Nothing
'    Set m_Res = Nothing
'
'    Set imgStop = Nothing
'    Set imgStopDis = Nothing
'    Set frmLayout = Nothing
'    Set aParams = Nothing
'End Sub
'
'Private Sub AdjustCaption()
'    On Error Resume Next
'    RaiseEvent OnSetCaption
'End Sub
'
'Private Sub AdjustState()
'    On Error Resume Next
'    UpdateMenu
'    RaiseEvent OnStateChange
'End Sub
'
'Private Sub UpdateMenu()
'    On Error Resume Next
'    mnuCtxRefresh.Enabled = Not (m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc)
'End Sub
'
'Public Function GetCaption() As String
'    On Error Resume Next
'    Dim sCaption$
'    GetCaption = "Two Indices Hedge"
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    If Not m_Idx(1) Is Nothing And Not m_Idx(2) Is Nothing Then
'        GetShortCaption = m_Idx(1).Symbol & " | " & m_Idx(2).Symbol
'    Else
'        GetShortCaption = "<Empty>"
'    End If
'End Function
'
'Public Property Get InProc() As Boolean
'    InProc = m_bInProc
'End Property
'
'Public Property Get LastQuoteReqNow() As Boolean
'    LastQuoteReqNow = m_bLastQuoteReqNow
'End Property
'
'Private Sub CopyFilters(ByVal bToPrev As Boolean)
'    On Error Resume Next
'    Dim i&
'
'    If bToPrev Then
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            m_nFilterPrev(i) = m_nFilter(i)
'        Next
'    Else
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            m_nFilter(i) = m_nFilterPrev(i)
'       Next
'    End If
'End Sub
'
'Private Sub FilterUpdateChangedStatus()
'    On Error Resume Next
'    Dim i&, bSetHint As Boolean
'
'    bSetHint = False
'
'    With fgFlt
'        m_GridLock(GT_INDEXHEDGE2_FILTER).LockRedraw
'
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            If m_nFilter(i) <> m_nFilterPrev(i) Then
'                .Cell(flexcpFontBold, 1, i) = True
'                bSetHint = True
'            Else
'                .Cell(flexcpFontBold, 1, i) = False
'            End If
'        Next
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(GT_INDEXHEDGE2_FILTER).UnlockRedraw
'        SetRefreshHint bSetHint
'    End With
'End Sub
'
'Public Sub FilterRevert()
'    On Error Resume Next
'    CopyFilters False
'    FilterUpdate
'    FilterUpdateChangedStatus
'    AdjustState
'End Sub
'
'Public Function FilterRevertEnabled() As Boolean
'    On Error Resume Next
'    Dim i&, bCanRevert As Boolean: bCanRevert = False
'
'    If Not m_bInProc Then
'        bCanRevert = False
'        For i = JFC_TRADER To JFC_LAST_COLUMN
'            If m_nFilter(i) <> m_nFilterPrev(i) Then
'                bCanRevert = True
'                Exit For
'            End If
'        Next
'    End If
'
'    FilterRevertEnabled = bCanRevert
'End Function
'
'Private Sub ClearViewAndData()
'    On Error Resume Next
'    m_fg(GT_INDEXHEDGE2_UNDERLYINGS_1).Rows = 1
'    m_fg(GT_INDEXHEDGE2_UNDERLYINGS_2).Rows = 1
'    m_fg(GT_INDEXHEDGE2_RESULTS).Rows = 1
'
'    Set m_Idx(1) = Nothing
'    Set m_Idx(2) = Nothing
'    Set m_Res = Nothing
'    m_QuoteRequests.Clear
'End Sub
'
'Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, sSection$
'
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = JFC_TRADER To JFC_LAST_COLUMN
'        aStorage.SetLongValue "TwoIndicesHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
'    Next
'
'    aStorage.SetDoubleValue "TwoIndicesHedgeTot" & sKey, "Slice", m_dSlice
'    aStorage.SetLongValue "TwoIndicesHedgeTot" & sKey, "LotSize", m_nLotSize
'    aStorage.SetLongValue "TwoIndicesHedgeTot" & sKey, "MaxSize", m_nMaxSize
'
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
'    On Error GoTo EH
'    Dim i&, sSection$
'
'    If Len(sKey) > 0 Then sKey = "." & sKey
'
'    ' common info
'    For i = JFC_TRADER To JFC_LAST_COLUMN
'        m_nFilter(i) = aStorage.GetLongValue("TwoIndicesHedgeFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
'    Next
'
'    m_dSlice = aStorage.GetDoubleValue("TwoIndicesHedgeTot" & sKey, "Slice", m_dSlice)
'    If m_dSlice < 0.01 Then
'        m_dSlice = 0.01
'    ElseIf m_dSlice > 1# Then
'        m_dSlice = 1#
'    End If
'
'    m_nLotSize = aStorage.GetLongValue("TwoIndicesHedgeTot" & sKey, "LotSize", m_nLotSize)
'    If m_nLotSize <= 0 Then _
'        m_nLotSize = 100
'
'    m_nMaxSize = aStorage.GetLongValue("TwoIndicesHedgeTot" & sKey, "MaxSize", m_nMaxSize)
'    If m_nMaxSize <= 0 Then _
'        m_nMaxSize = 10000
'
'    fgTot1.TextMatrix(1, JTC_SLICE) = m_dSlice * 100#
'    fgTot1.TextMatrix(1, JTC_LOT_SIZE) = m_nLotSize
'    fgTot1.TextMatrix(1, JTC_MAX_SIZE) = m_nMaxSize
'
'    FilterUpdate
'
'    tmrShow.Enabled = True
'    Exit Sub
'EH:
'    gCmn.ErrorHandler ""
'End Sub
'
'Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, "Two Indices Hedge", "Two Indices Hedge", fgFlt, fgTot1, fgUnd1, fgTot2, fgUnd2, fgRes
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Function CurUnderlyingID() As Long
'    On Error Resume Next
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    CurUnderlyingID = 0
'
'    If ActiveControl Is fgUnd1 Then
'        With fgUnd1
'            If .Row > 0 Then
'                Set aUnd = .RowData(.Row)
'                If Not aUnd Is Nothing Then
'                    CurUnderlyingID = aUnd.ID
'                    Set aUnd = Nothing
'                End If
'            End If
'        End With
'
'    ElseIf ActiveControl Is fgUnd2 Then
'        With fgUnd2
'            If .Row > 0 Then
'                Set aUnd = .RowData(.Row)
'                If Not aUnd Is Nothing Then
'                    CurUnderlyingID = aUnd.ID
'                    Set aUnd = Nothing
'                End If
'            End If
'        End With
'
'    ElseIf ActiveControl Is fgRes Then
'        With fgRes
'            If .Row > 0 Then
'                Set aUnd = .RowData(.Row)
'                If Not aUnd Is Nothing Then
'                    CurUnderlyingID = aUnd.ID
'                    Set aUnd = Nothing
'                End If
'            End If
'        End With
'    Else
'        CurUnderlyingID = m_nFilter(JFC_INDEX1)
'        If CurUnderlyingID = 0 Then CurUnderlyingID = m_nFilter(JFC_INDEX2)
'    End If
'End Function
'
'Public Sub Refresh()
'    On Error Resume Next
'    With fgFlt
'        If .EditWindow <> 0 Then .Select .Row, .Col
'    End With
'
'    tmrShow.Enabled = True
'End Sub
'
'Public Sub CustomizeFltGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_INDEXHEDGE2_FILTER, JFC_TRADER, JFC_LAST_COLUMN, m_gd(GT_INDEXHEDGE2_FILTER), m_frmOwner
'End Sub
'
'Public Sub CustomizeTotGridLayout(nIdx&)
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute nIdx, JTC_INDEX, JTC_LAST_COLUMN, m_gd(nIdx), m_frmOwner
'End Sub
'
'Public Sub CustomizeUndGridLayout(nIdx&)
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute nIdx, JUC_SYMBOL, JUC_LAST_COLUMN, m_gd(nIdx), m_frmOwner
'End Sub
'
'Public Sub CustomizeResGridLayout()
'    On Error Resume Next
'    If m_bShutDown Or frmLayout Is Nothing Then Exit Sub
'    frmLayout.Execute GT_INDEXHEDGE2_RESULTS, JRC_SYMBOL, JRC_LAST_COLUMN, m_gd(GT_INDEXHEDGE2_RESULTS), m_frmOwner
'End Sub
'
'Private Sub SetRefreshHint(ByVal bSet As Boolean)
'    On Error Resume Next
'    lblStatus.Visible = True
'    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
'End Sub
'
'Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
'    On Error Resume Next
'    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
'        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'
'        If bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
'            mnuCtxTradeNew_Click
'            FixDblClickHandled
'        End If
'    End If
'End Sub
'
'Private Sub mnuCtxTradeNew_Click()
'    On Error GoTo EH
'    If m_bInProc Then Exit Sub
'    Dim nUndID&, nID&, bBuy As Boolean
'
'    m_bInProc = True
'    nUndID = 0
'    bBuy = True
'
'    If m_enMenuGrid = GT_INDEXHEDGE2_FILTER Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then nUndID = m_nFilter(JFC_INDEX2)
'        bBuy = (g_Params.QuoteOtherColBuy <> 0)
'
'    ElseIf m_enMenuGrid = GT_INDEXHEDGE2_UNDERLYINGS_1 Or m_enMenuGrid = GT_INDEXHEDGE2_UNDERLYINGS_2 Or m_enMenuGrid = GT_INDEXHEDGE2_RESULTS Then
'        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
'            On Error Resume Next
'            nUndID = m_fg(m_enMenuGrid).RowData(m_nMenuGridRow).ID
'            On Error GoTo EH
'        End If
'        bBuy = (g_Params.QuoteOtherColBuy <> 0)
'    End If
'
'    m_bInProc = False
'    If nUndID <> 0 Then frmTradeEditor.NewTrade nUndID, nUndID, bBuy, m_frmOwner
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
'    ResetMenuData
'End Sub
'
'Private Sub InitIndexVola(nIndexIdx&)
'    On Error GoTo EH
'    If Not m_Idx(nIndexIdx) Is Nothing Then m_Idx(nIndexIdx).VolaSrv.Init m_Idx(nIndexIdx).Symbol, enCtIndex, g_VolaSource
'    Exit Sub
'EH:
'    LogEvent EVENT_ERROR, "Fail to init volatility for '" & m_Idx(nIndexIdx).Symbol & "': " & Err.Description
'End Sub
'
'Public Sub SaveOrders(ByVal sPath As String)
'    On Error GoTo EH
'    If m_bInProc Or m_Idx(1) Is Nothing Or m_Idx(2) Is Nothing Or Not IsValidFilter Then Exit Sub
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim nFileB&, nFileS&, nFileSS&, sPrefix$, nTotalQty&, nExportCount&
'    Dim nQtyB&, nQtyS&, nQtySS&, nOrdRemainder&, nOrdCount&, i&, nUndTotalQty&
'
'    m_bInProc = True
'    nExportCount = 0
'
'    nFileB = 0
'    nFileS = 0
'    nFileSS = 0
'
'    For Each aUnd In m_Res
'        If aUnd.TotalQtyInShares > BAD_LONG_VALUE And aUnd.CurShares > BAD_LONG_VALUE _
'            And aUnd.Transact > BAD_LONG_VALUE And aUnd.Transact <> 0& Then
'
'            sPrefix = aUnd.Symbol
'            If Len(sPrefix) < 8 Then sPrefix = sPrefix & Space$(8 - Len(sPrefix))
'            nQtyB = 0&
'            nQtyS = 0&
'            nQtySS = 0&
'
'            nUndTotalQty = aUnd.TotalQtyInShares
'
'            If aUnd.Transact > 0 Then
'                nQtyB = aUnd.Transact
'
'            Else ' aUnd.Transact < 0
'                If nUndTotalQty <= 0 Then
'                    nQtySS = Abs(aUnd.Transact)
'
'                ElseIf Abs(aUnd.Transact) > nUndTotalQty And nUndTotalQty > 0 Then
'                    nQtyS = Abs(-Sgn(nUndTotalQty) * Int(Abs(nUndTotalQty) / m_nLotSize + 0.5) * m_nLotSize)
'                    nQtySS = Abs(Abs(aUnd.Transact) - nQtyS)
'
'                ElseIf Abs(aUnd.Transact) <= nUndTotalQty Then
'                    nQtyS = Abs(aUnd.Transact)
'                End If
'            End If
'
'            If nQtyB <> 0 Or nQtyS <> 0 Or nQtySS <> 0 Then
'                If nQtyB <> 0 Then
'                    nOrdCount = Int(nQtyB / m_nMaxSize)
'                    nOrdRemainder = nQtyB - nOrdCount * m_nMaxSize
'
'                    If nFileB = 0 Then
'                        nFileB = FreeFile
'                        Open sPath & "bsktBuy.txt" For Output Access Write Lock Read Write As #nFileB
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileB, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileB, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'
'                If nQtyS <> 0 Then
'                    nOrdCount = Int(nQtyS / m_nMaxSize)
'                    nOrdRemainder = nQtyS - nOrdCount * m_nMaxSize
'
'                    If nFileS = 0 Then
'                        nFileS = FreeFile
'                        Open sPath & "bsktSell.txt" For Output Access Write Lock Read Write As #nFileS
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileS, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileS, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'
'                If nQtySS <> 0 Then
'                    nOrdCount = Int(nQtySS / m_nMaxSize)
'                    nOrdRemainder = nQtySS - nOrdCount * m_nMaxSize
'
'                    If nFileSS = 0 Then
'                        nFileSS = FreeFile
'                        Open sPath & "bsktSht.txt" For Output Access Write Lock Read Write As #nFileSS
'                    End If
'
'                    For i = 1 To nOrdCount
'                        Print #nFileSS, sPrefix & Trim$(Str$(m_nMaxSize))
'                        nExportCount = nExportCount + 1
'                    Next
'
'                    If nOrdRemainder > 0 Then
'                        Print #nFileSS, sPrefix & Trim$(Str$(nOrdRemainder))
'                        nExportCount = nExportCount + 1
'                    End If
'                End If
'            End If
'        End If
'    Next
'
'    If nFileB <> 0 Then Close #nFileB
'    If nFileS <> 0 Then Close #nFileS
'    If nFileSS <> 0 Then Close #nFileSS
'
'    gCmn.MyMsgBox m_frmOwner, CStr(nExportCount) & " order(s) saved.", vbInformation
'EX:
'    On Error Resume Next
'    Close #nFileB
'    Close #nFileS
'    Close #nFileSS
'
'    m_bInProc = False
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox m_frmOwner, "Fail to save orders."
'    ResetMenuData
'    GoTo EX
'End Sub
'
'Private Sub UpdateTotalsAll()
'    On Error Resume Next
'    UpdateTotals GT_INDEXHEDGE2_TOTALS_1
'    UpdateTotals GT_INDEXHEDGE2_TOTALS_2
'End Sub
'
'Private Sub UpdateTotals(nIdx&)
'    On Error Resume Next
'    Dim nIndexIdx&
'    nIndexIdx = nIdx - GT_INDEXHEDGE2_TOTALS_1 + 1
'
'    With m_fg(nIdx)
'        m_GridLock(nIdx).LockRedraw
'
'        If Not m_Idx(nIndexIdx) Is Nothing Then
'            .TextMatrix(1, JTC_INDEX) = m_Idx(nIndexIdx).Symbol
'            .TextMatrix(1, JTC_NET_DELTA) = IIf(m_Idx(nIndexIdx).NetDelta > BAD_DOUBLE_VALUE, m_Idx(nIndexIdx).NetDelta, STR_NA)
'            .TextMatrix(1, JTC_PRICE) = IIf(m_Idx(nIndexIdx).PriceLast > BAD_DOUBLE_VALUE, m_Idx(nIndexIdx).PriceLast, STR_NA)
'        Else
'            .TextMatrix(1, JTC_INDEX) = "<Not Selected>"
'            .TextMatrix(1, JTC_NET_DELTA) = STR_NA
'            .TextMatrix(1, JTC_PRICE) = STR_NA
'        End If
'
'        .TextMatrix(1, JTC_SLICE) = m_dSlice * 100#
'        .TextMatrix(1, JTC_LOT_SIZE) = m_nLotSize
'        .TextMatrix(1, JTC_MAX_SIZE) = m_nMaxSize
'        .TextMatrix(1, JTC_CONTRACTS) = IIf(m_nContracts(nIndexIdx) <> 0&, m_nContracts(nIndexIdx), "")
'        .TextMatrix(1, JTC_DELTA) = IIf(m_dDelta(nIndexIdx) <> 0#, m_dDelta(nIndexIdx) * 100#, "")
'        .TextMatrix(1, JTC_EXPOSURE) = IIf(m_dExposure(nIndexIdx) > BAD_DOUBLE_VALUE, m_dExposure(nIndexIdx), STR_NA)
'
'        .AutoSize 0, .Cols - 1, , 100
'
'        m_GridLock(nIdx).UnlockRedraw
'    End With
'End Sub
'
'Private Sub CalcTotalsAll(Optional ByVal bCalcNetDelta As Boolean = True)
'    On Error Resume Next
'    If m_Res Is Nothing Then Exit Sub
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aUnd1 As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aUnd2 As EtsMmIndexHedgeLib.MmIhUndAtom
'
'    CalcTotals 1, bCalcNetDelta
'    CalcTotals 2, bCalcNetDelta
'
'    For Each aUnd In m_Res
'        Set aUnd1 = m_Idx(1).Und(aUnd.ID)
'        Set aUnd2 = m_Idx(2).Und(aUnd.ID)
'
'        aUnd.IndexTxn = BAD_LONG_VALUE
'        aUnd.Transact = BAD_LONG_VALUE
'
'        aUnd.IndexShares1 = BAD_LONG_VALUE
'        If Not aUnd1 Is Nothing Then aUnd.IndexShares1 = aUnd1.IndexShares1
'        If aUnd.IndexShares1 > BAD_LONG_VALUE Then
'            aUnd.IndexTxn = aUnd.IndexShares1
'        End If
'
'        aUnd.IndexShares2 = BAD_LONG_VALUE
'        If Not aUnd2 Is Nothing Then aUnd.IndexShares2 = aUnd2.IndexShares1
'        If aUnd.IndexShares2 > BAD_LONG_VALUE Then
'            If aUnd.IndexTxn = BAD_LONG_VALUE Then aUnd.IndexTxn = 0
'            aUnd.IndexTxn = aUnd.IndexTxn + aUnd.IndexShares2
'        End If
'
'        If aUnd.CurShares > BAD_LONG_VALUE And aUnd.IndexTxn > BAD_LONG_VALUE Then
'            aUnd.IndexTxn = -aUnd.IndexTxn
'            aUnd.Transact = (aUnd.IndexTxn - aUnd.CurShares) * m_dSlice
'            aUnd.Transact = Sgn(aUnd.Transact) * Int(Abs(aUnd.Transact) / m_nLotSize + 0.5) * m_nLotSize
'        End If
'
'        Set aUnd1 = Nothing
'        Set aUnd2 = Nothing
'    Next
'End Sub
'
'Private Sub CalcTotals(nIndexIdx&, ByVal bCalcNetDelta As Boolean)
'    On Error Resume Next
'    Dim aUnd As EtsMmIndexHedgeLib.MmIhUndAtom
'    Dim aOpt As EtsMmIndexHedgeLib.MmIhOptAtom
'    Dim dUndMidPrice#
'
'    m_dExposure(nIndexIdx) = BAD_DOUBLE_VALUE
'
'    If Not m_Idx(nIndexIdx) Is Nothing Then
'        If bCalcNetDelta Then
'            m_Idx(nIndexIdx).NetDelta = BAD_DOUBLE_VALUE
'            For Each aOpt In m_Idx(nIndexIdx).Opt
'                If aOpt.Delta > BAD_DOUBLE_VALUE Then
'                    If m_Idx(nIndexIdx).NetDelta = BAD_DOUBLE_VALUE Then m_Idx(nIndexIdx).NetDelta = 0&
'                    m_Idx(nIndexIdx).NetDelta = m_Idx(nIndexIdx).NetDelta + aOpt.Delta * aOpt.Qty
'                End If
'            Next
'        End If
'
'        If m_Idx(nIndexIdx).PriceLast > 0# Then
'            If m_nContracts(nIndexIdx) <> 0 And m_dDelta(nIndexIdx) <> 0# Then
'                m_dExposure(nIndexIdx) = m_Idx(nIndexIdx).PriceLast * m_nContracts(nIndexIdx) * m_dDelta(nIndexIdx) * 100#
'            Else
'                If m_Idx(nIndexIdx).NetDelta > BAD_DOUBLE_VALUE Then _
'                    m_dExposure(nIndexIdx) = m_Idx(nIndexIdx).PriceLast * m_Idx(nIndexIdx).NetDelta
'            End If
'        End If
'
'        For Each aUnd In m_Idx(nIndexIdx).Und
'            dUndMidPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, _
'                                                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'
'            If m_dExposure(nIndexIdx) > BAD_DOUBLE_VALUE And dUndMidPrice > 0# And aUnd.Weight > 0# And aUnd.CurShares > BAD_LONG_VALUE Then
'                aUnd.IndexShares1 = m_dExposure(nIndexIdx) * aUnd.Weight / aUnd.PriceLast
'                aUnd.IndexShares1 = Sgn(aUnd.IndexShares1) * Int(Abs(aUnd.IndexShares1) / m_nLotSize + 0.5) * m_nLotSize
'                aUnd.IndexTxn = -(aUnd.IndexShares1 + aUnd.CurShares) * m_dSlice
'                aUnd.IndexTxn = Sgn(aUnd.IndexTxn) * Int(Abs(aUnd.IndexTxn) / m_nLotSize + 0.5) * m_nLotSize
'            Else
'                aUnd.IndexShares1 = BAD_LONG_VALUE
'                aUnd.IndexTxn = BAD_LONG_VALUE
'            End If
'        Next
'    End If
'End Sub
'
'Private Sub CalcIndices()
'    On Error Resume Next
'    If m_bInProc Or m_Idx(1) Is Nothing Or m_Idx(2) Is Nothing Then Exit Sub
'
'    m_bInProc = True
'    m_bRecalc = True
'    AdjustState
'
'    If m_bRecalc Then
'        lblStatus.Visible = False
'        imgStop.Visible = True
'        imgStopDis.Visible = False
'        pbProgress.Min = 0
'        pbProgress.Value = 0
'        pbProgress.Max = 100
'        pbProgress.Visible = True
'        lblProcess.Caption = "Calculation..."
'        lblProcess.Visible = True
'        lblProcess.Refresh
'
'        Dim nCalcSleepFreq&: nCalcSleepFreq = 0&
'        Dim nCalcSleepAmt&: nCalcSleepAmt = 0&
'
'        If g_Params.UseCalcSleep And g_Params.CalcSleepAmt <> 0 Then
'            nCalcSleepFreq = g_Params.CalcSleepFreq
'            nCalcSleepAmt = g_Params.CalcSleepAmt
'        End If
'
'        m_Idx(1).CalcAllOptions g_Params.CalcModel, _
'                        g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
'                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
'                        nCalcSleepFreq, nCalcSleepAmt
'
'        m_Idx(2).CalcAllOptions g_Params.CalcModel, _
'                        g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, _
'                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
'                        nCalcSleepFreq, nCalcSleepAmt
'        CalcTotalsAll
'    End If
'    m_bRecalc = False
'    m_bInProc = False
'    AdjustState
'End Sub
'
'Public Function IsValidFilter() As Boolean
'    On Error Resume Next
'    IsValidFilter = (m_nFilter(JFC_TRADER) <> 0 _
'                    And m_nFilter(JFC_STRATEGY) <> 0 _
'                    And m_nFilter(JFC_INDEX1) <> 0 _
'                    And m_nFilter(JFC_INDEX2) <> 0 _
'                    And m_nFilter(JFC_INDEX1) <> m_nFilter(JFC_INDEX2))
'End Function
'
