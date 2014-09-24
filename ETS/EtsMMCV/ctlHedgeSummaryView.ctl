VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlHedgeSummaryView 
   ClientHeight    =   8460
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9675
   KeyPreview      =   -1  'True
   ScaleHeight     =   8460
   ScaleMode       =   0  'User
   ScaleWidth      =   9675
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   8040
      Top             =   4920
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTot 
      Height          =   495
      Left            =   0
      TabIndex        =   2
      Top             =   600
      Width           =   7575
      _cx             =   13361
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
   Begin VSFlex7Ctl.VSFlexGrid fgInst 
      Height          =   495
      Left            =   0
      TabIndex        =   1
      Top             =   1200
      Width           =   7575
      _cx             =   13361
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
      Height          =   495
      Left            =   0
      TabIndex        =   3
      Top             =   1800
      Width           =   7575
      _cx             =   13361
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
   Begin VSFlex7Ctl.VSFlexGrid fgFlt 
      Height          =   495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   7575
      _cx             =   13361
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
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
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlHedgeSummaryView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "RVB_UniqueId" ,"3E478C2300BB"
Attribute VB_Ext_KEY = "RVB_ModelStereotype" ,"User Control"
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()
Public Event OnRunHedger(sFileName As String)

Private m_gdFlt As New clsGridDef
Private m_gdInst As New clsGridDef
Private m_gdTot As New clsGridDef
Private m_gdRes As New clsGridDef

Private WithEvents geInst As clsGridEx
Attribute geInst.VB_VarHelpID = -1
Private WithEvents geRes As clsGridEx
Attribute geRes.VB_VarHelpID = -1

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
Private WithEvents DhIvLoader As DHIVFeeder5.DhIvLoader
Attribute DhIvLoader.VB_VarHelpID = -1
Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label
Public lblValue As VB.Label
Public WithEvents imgStop As VB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As VB.Image

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bRecalc As Boolean
Private m_bLastQuoteReqNow As Boolean
Private m_bInited As Boolean
Private m_bRunHedger As Boolean

Private m_QuoteRequests As New clsHsRequestColl
Private m_IvComRequests As New clsHsRequestColl

Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Private m_iEnableInstr As Long

Private m_Grp As New clsHsGrpAtom

Private HedgeCalc As New EtsMmHedgeLib.MmHsCalc

Public m_frmOwner As Form

Private m_sCurrentOriginalText As String
Private m_nFilter(HFC_GROUP To HFC_COLUMN_COUNT) As Long

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_HEDGESUMMARY_FILTER To GT_HEDGESUMMARY_TOTAL) As Boolean
Private m_GridLock(GT_HEDGESUMMARY_FILTER To GT_HEDGESUMMARY_TOTAL) As New clsGridLock

Private m_bGroupRequestSupported As Boolean
Private m_bShutDown As Boolean

Private m_fGain As Double
Private m_NewInsts As Collection

Private Const DHIVCORR_NORM As Long = 10000&
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Public Function Init() As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    
    m_iEnableInstr = 0
    
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    ResetMenuData
    m_bKeyDown(GT_HEDGESUMMARY_FILTER) = False
    m_bKeyDown(GT_HEDGESUMMARY_INSTRUMENTS) = False
    m_bKeyDown(GT_HEDGESUMMARY_RESULTS) = False
    m_bKeyDown(GT_HEDGESUMMARY_TOTAL) = False

    m_GridLock(GT_HEDGESUMMARY_FILTER).Init fgFlt
    m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).Init fgInst
    m_GridLock(GT_HEDGESUMMARY_RESULTS).Init fgRes
    m_GridLock(GT_HEDGESUMMARY_TOTAL).Init fgTot

    InitColumns
    InitGrids

    Set geInst = New clsGridEx
    geInst.Init fgInst
    Set geRes = New clsGridEx
    geRes.Init fgRes
    
    FormatFltGrid
    FormatFltColumns True

    FormatInstGrid
    FormatInstColumns True

    FormatResGrid
    FormatResColumns True
    
    FormatTotGrid
    FormatTotColumns True
    
    InitFltData
'    InitInstData
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequestSupported = False 'g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    fgFlt.Col = HFC_GROUP
    fgFlt.Row = 1
    
    Set DhIvLoader = New DHIVFeeder5.DhIvLoader
    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    
    Init = True
    
    AdjustCaption
    AdjustState
    
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize hedge summary."
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    m_nFilter(HFC_GROUP) = nType
    m_nFilter(HFC_VALUE) = nValue
    FilterUpdateAll
    
    ClearResGrid True
    ClearTotGrid True
    
    tmrShow.Enabled = True
End Sub

Private Sub ClearResGrid(ByVal bAutosize As Boolean)
    On Error Resume Next
    fgRes.Cell(flexcpText, 1, 1, fgRes.Rows - 1, fgRes.Cols - 1) = ""
End Sub

Private Sub ClearTotGrid(ByVal bAutosize As Boolean)
    On Error Resume Next
    fgTot.Cell(flexcpText, 1, 0, fgTot.Rows - 1, fgTot.Cols - 1) = ""
End Sub

Private Sub FilterUpdateAll()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw

        If m_nFilter(HFC_GROUP) <> TYPE_UNDERLYING _
            And m_nFilter(HFC_GROUP) <> TYPE_GROUP _
            And m_nFilter(HFC_GROUP) <> TYPE_TRADER _
            And m_nFilter(HFC_GROUP) <> TYPE_STRATEGY _
            And m_nFilter(HFC_GROUP) <> TYPE_ALL Then
            
            m_nFilter(HFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(HFC_VALUE) = 0
        End If

        If m_nFilter(HFC_GROUP) = TYPE_ALL Then
            m_nFilter(HFC_VALUE) = -1
        End If

        .TextMatrix(1, HFC_GROUP) = m_nFilter(HFC_GROUP)

        FilterUpdateValue True

        .TextMatrix(1, HFC_TYPE) = m_nFilter(HFC_TYPE)

        .TextMatrix(1, HFC_WEIGHTED) = CBool(m_nFilter(HFC_WEIGHTED))
        .ColHidden(HFC_WEIGHTED) = Not CBool(m_nFilter(HFC_TYPE))
        
        .AutoSize 0, .Cols - 1, , 100

        m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
    End With
    
    With fgRes
        m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
        
        .ColHidden(HRC_VEGAEQ) = Not CBool(m_nFilter(HFC_TYPE))
        .ColHidden(HRC_POS) = CBool(m_nFilter(HFC_TYPE))
        .ColHidden(HRC_CONTRACTS) = CBool(m_nFilter(HFC_TYPE))
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateValue(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom
    Dim nValue&
    
    With fgFlt
        m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = ""
        
        Select Case m_nFilter(HFC_GROUP)
            Case TYPE_ALL
                sValue = "-1"
                nValue = -1
                sComboList = "#-1;<All>"
                
            Case TYPE_UNDERLYING
                sComboList = g_Params.UnderlyingComboList
                
                Set aUnd = g_Underlying(m_nFilter(HFC_VALUE))
                If Not aUnd Is Nothing Then
                    nValue = aUnd.ID
                    sValue = Trim$(Str$(nValue))
                    Set aUnd = Nothing
                End If
                
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
            
            Case TYPE_TRADER
                Dim aTrader As EtsGeneralLib.EtsTraderAtom
                
                For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = m_nFilter(HFC_VALUE) Then
                        nValue = aTrader.ID
                        sValue = Trim$(nValue)
                    End If
                Next
            
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
                
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(HFC_VALUE) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
            
            Case TYPE_STRATEGY
                Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
                
                sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
                If m_nFilter(HFC_VALUE) = NO_STRATEGY_ID Then
                    nValue = NO_STRATEGY_ID
                    sValue = NO_STRATEGY_NAME
                End If
                
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_nFilter(HFC_VALUE) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
            
        End Select
        
        If Len(sComboList) > 0 Then
            If nValue = 0 Then
                sComboList = "#0;<Select>" & sComboList
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(HFC_VALUE) = sComboList
        .TextMatrix(1, HFC_VALUE) = sValue
        m_nFilter(HFC_VALUE) = nValue
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If
        
        m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
    End With
End Sub

Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    bMatched = False
    
    If Not aTrd Is Nothing Then
        ' futures & futures options trades are not supported yet
        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then Exit Function
        
        If aTrd.ContractType = enCtOption Then
            If aTrd.Opt.Expiry < Date Then Exit Function
        End If
        nValue = m_nFilter(HFC_VALUE)
        
        If nValue <> 0 Then
            Select Case m_nFilter(HFC_GROUP)
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
        End If
    End If

    CheckTradeFilter = bMatched
End Function

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_HEDGESUMMARY_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_HEDGESUMMARY_INSTRUMENTS).CopyTo m_gdInst
    g_DefGrids(GT_HEDGESUMMARY_RESULTS).CopyTo m_gdRes
    g_DefGrids(GT_HEDGESUMMARY_TOTAL).CopyTo m_gdTot
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = HFC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .ColWidthMin = 150
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

    With fgInst
        .Rows = 1
        .Cols = HIC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchFromTop
        .AutoSizeMouse = True
        .ColWidthMin = 150
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSort
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls

        .HighLight = flexHighlightWithFocus
    End With

    With fgRes
        .Rows = 1
        .Cols = HRC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchFromTop
        .AutoSizeMouse = True
        .ColWidthMin = 150
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSort
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .MergeCells = flexMergeFree
        .MergeCompare = flexMCExact
        .TabBehavior = flexTabControls

        .HighLight = flexHighlightWithFocus
    End With

    With fgTot
        .Rows = 2
        .Cols = HTC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .ColWidthMin = 150
        .Editable = flexEDNone
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
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls

        .HighLight = flexHighlightWithFocus
    End With

End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim i&
    
    With fgFlt
        m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
        
        .ColComboList(HFC_GROUP) = "#" & CStr(TYPE_UNDERLYING) & ";Underlying" & _
                                    "|#" & CStr(TYPE_GROUP) & ";Group" & _
                                    "|#" & CStr(TYPE_TRADER) & ";Trader" & _
                                    "|#" & CStr(TYPE_STRATEGY) & ";Strategy" & _
                                    "|#" & CStr(TYPE_ALL) & ";<All>"
                                    
        .TextMatrix(1, HFC_GROUP) = CStr(TYPE_UNDERLYING)
        m_nFilter(HFC_GROUP) = TYPE_UNDERLYING
        
        .ColComboList(HFC_VALUE) = "#0;<None>"
        .TextMatrix(1, HFC_VALUE) = "0"
        m_nFilter(HFC_VALUE) = 0
    
        .ColComboList(HFC_TYPE) = "#0;Delta|#1;Vega"
        .TextMatrix(1, HFC_TYPE) = "0"
        m_nFilter(HFC_TYPE) = 0
    
        m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
    
        nCols = HFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = HFC_GROUP To HFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = HFC_GROUP To HFC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).BackColor <> 0, m_gdFlt.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).ForeColor <> 0, m_gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If m_gdFlt.HorizLinesVisible Then
            If m_gdFlt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdFlt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdFlt.GridLinesColor
        
        .BackColorSel = m_gdFlt.BackColorSel
        .ForeColorSel = m_gdFlt.ForeColorSel
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitInstData()
    On Error Resume Next
    Dim i&, sRow
    
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim aHsAtom As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim nRow&
    
    Screen.MousePointer = vbArrow
    lblStatus.Caption = "Initializing ..."
    lblStatus.Visible = True
    
    
    Set aStocks = HedgeCalc.Instruments
    
    For Each aUnd In g_UnderlyingAll
        Set aHsAtom = aStocks.Add(aUnd.ID)
        Set aHsAtom.Dividend = aUnd.Dividend
                
        aHsAtom.ID = aUnd.ID
        aHsAtom.Enabled = False
        aHsAtom.IsAmerican = aUnd.IsAmerican
        aHsAtom.LotSize = aUnd.LotSize
        aHsAtom.Symbol = aUnd.Symbol
        aHsAtom.UndType = aUnd.UndType
        aHsAtom.Yield = aUnd.Yield
        Set aHsAtom.UndPriceProfile = aUnd.UndPriceProfile
        Set aHsAtom.OptPriceProfile = aUnd.OptPriceProfile
        
        Set aHsAtom = Nothing
        Set aUnd = Nothing
           
        If m_bShutDown Then Exit Sub
        
        DoEvents
    Next
        
    With fgInst
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
        
        For Each aHsAtom In aStocks
            sRow = CStr(aHsAtom.Enabled)
            sRow = sRow & vbTab & aHsAtom.Symbol
            sRow = sRow & vbTab & IIf(aHsAtom.UndType = enCtIndex, "Index", "Stock")
            
            .AddItem sRow
            .RowData(.Rows - 1) = aHsAtom.ID
            
            DoEvents
            Set aHsAtom = Nothing
        Next
        
        .Select 0, 1
        .Sort = flexSortGenericAscending
        .Row = 1
        .Col = 1
                
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
    End With
    
    Set aStocks = Nothing
    
    lblStatus.Caption = ""
    Screen.MousePointer = vbDefault
End Sub

Private Sub FormatInstGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgInst
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
    
        nCols = HIC_COLUMN_COUNT
        .Cols = nCols
        
        For i = HIC_USE To HIC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdInst.Col(i).Caption
            .ColFormat(i) = m_gdInst.Col(i).Format
            .ColDataType(i) = m_gdInst.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geInst.AdjustSortImages
        
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
    End With
End Sub

Private Sub FormatInstColumns(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgInst
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
    
        aFont.Name = m_gdInst.Font.Name
        aFont.Size = m_gdInst.Font.Size
        aFont.Bold = m_gdInst.Font.Bold
        aFont.Italic = m_gdInst.Font.Italic
        aFont.Strikethrough = m_gdInst.Font.Strikethrough
        aFont.Underline = m_gdInst.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = HIC_USE To HIC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdInst.Col(i).BackColor <> 0, m_gdInst.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdInst.Col(i).ForeColor <> 0, m_gdInst.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If m_gdInst.HorizLinesVisible Then
            If m_gdInst.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdInst.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdInst.GridLinesColor
        
        .BackColorSel = m_gdInst.BackColorSel
        .ForeColorSel = m_gdInst.ForeColorSel
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
            UserControl_Resize
        End If
        
        m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
    End With
End Sub

Private Sub FormatResGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgRes
        m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
    
        nCols = HRC_COLUMN_COUNT
        .Cols = nCols
        
        For i = HRC_SYMBOL To HRC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdRes.Col(i).Caption
            .ColFormat(i) = m_gdRes.Col(i).Format
            .ColDataType(i) = m_gdRes.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geRes.AdjustSortImages
        
        m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
    End With
End Sub

Private Sub FormatResColumns(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgRes
        m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
    
        aFont.Name = m_gdRes.Font.Name
        aFont.Size = m_gdRes.Font.Size
        aFont.Bold = m_gdRes.Font.Bold
        aFont.Italic = m_gdRes.Font.Italic
        aFont.Strikethrough = m_gdRes.Font.Strikethrough
        aFont.Underline = m_gdRes.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = HRC_SYMBOL To HRC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdRes.Col(i).BackColor <> 0, m_gdRes.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdRes.Col(i).ForeColor <> 0, m_gdRes.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If m_gdRes.HorizLinesVisible Then
            If m_gdRes.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdRes.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdRes.GridLinesColor
        
        .BackColorSel = m_gdRes.BackColorSel
        .ForeColorSel = m_gdRes.ForeColorSel
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
    End With
End Sub

Private Sub FormatTotGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgTot
        m_GridLock(GT_HEDGESUMMARY_TOTAL).LockRedraw
    
        nCols = HTC_COLUMN_COUNT
        .Cols = nCols
        
        For i = HTC_NET To HTC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdTot.Col(i).Caption
            .ColFormat(i) = m_gdTot.Col(i).Format
            .ColDataType(i) = m_gdTot.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_HEDGESUMMARY_TOTAL).UnlockRedraw
    End With
End Sub

Private Sub FormatTotColumns(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgTot
        m_GridLock(GT_HEDGESUMMARY_TOTAL).LockRedraw
    
        aFont.Name = m_gdTot.Font.Name
        aFont.Size = m_gdTot.Font.Size
        aFont.Bold = m_gdTot.Font.Bold
        aFont.Italic = m_gdTot.Font.Italic
        aFont.Strikethrough = m_gdTot.Font.Strikethrough
        aFont.Underline = m_gdTot.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = HTC_NET To HTC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdTot.Col(i).BackColor <> 0, m_gdTot.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdTot.Col(i).ForeColor <> 0, m_gdTot.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If m_gdTot.HorizLinesVisible Then
            If m_gdTot.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdTot.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdTot.GridLinesColor
        
        .BackColorSel = m_gdTot.BackColorSel
        .ForeColorSel = m_gdTot.ForeColorSel
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_HEDGESUMMARY_TOTAL).UnlockRedraw
    End With
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = ""
    If m_Grp.ID <> 0 Then sCaption = sCaption & m_Grp.Name & " - "
    sCaption = sCaption & "Hedge Summary"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    If m_Grp.ID <> 0 Then
        GetShortCaption = m_Grp.Name
    Else
        GetShortCaption = "<Empty>"
    End If
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Property Get DataLoad() As Boolean
    DataLoad = m_bDataLoad
End Property

Public Property Get LastQuoteReqNow() As Boolean
    LastQuoteReqNow = m_bLastQuoteReqNow
End Property

Public Property Get Inited() As Boolean
    Inited = m_bInited
End Property

Public Property Get InCalc() As Boolean
    InCalc = m_bRecalc
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_HEDGESUMMARY_FILTER, HFC_GROUP, HFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeInstGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_HEDGESUMMARY_INSTRUMENTS, HIC_USE, HIC_LAST_COLUMN, m_gdInst, m_frmOwner
    End If
End Sub

Public Sub CustomizeResGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_HEDGESUMMARY_RESULTS, HRC_SYMBOL, HRC_LAST_COLUMN, m_gdRes, m_frmOwner
    End If
End Sub

Public Sub CustomizeTotGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_HEDGESUMMARY_TOTAL, HTC_NET, HTC_LAST_COLUMN, m_gdTot, m_frmOwner
    End If
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    
    If Len(sKey) > 0 Then sKey = "." & sKey

    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height
    
    ' common info
    For i = HFC_GROUP To HFC_COLUMN_COUNT
        aStorage.SetLongValue "HedgeSummaryFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next
    
    Set aStocks = HedgeCalc.Instruments
    
    i = 0
    For Each aUnd In aStocks
        If aUnd.Enabled Then
            i = i + 1
            aStorage.SetLongValue "HedgeSummaryInst" & sKey, "Inst" & CStr(i), CStr(aUnd.ID)
        End If
    Next
    aStorage.SetLongValue "HedgeSummaryInst" & sKey, "InstCount", CStr(i)
    
    Set aStocks = Nothing
    
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&, nCount&, nID&, nRow&
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aColl As EtsMmHedgeLib.MmHsUndColl
    
    If Len(sKey) > 0 Then sKey = "." & sKey
        
    Set m_NewInsts = Nothing
    Set m_NewInsts = New Collection
    
        
    m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
    m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
    m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
    m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
        
    ' set new instruments info
    nCount = aStorage.GetLongValue("HedgeSummaryInst" & sKey, "InstCount", 0)
    For i = 1 To nCount
        nID = aStorage.GetLongValue("HedgeSummaryInst" & sKey, "Inst" & CStr(i), 0)
        If nID > 0 Then m_NewInsts.Add nID
    Next
    
    ' common info
    For i = HFC_GROUP To HFC_COLUMN_COUNT
        m_nFilter(i) = aStorage.GetLongValue("HedgeSummaryFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next
    
    If m_nFilter(HFC_TYPE) < 0 Or m_nFilter(HFC_TYPE) > 1 Then m_nFilter(HFC_TYPE) = 0
    
    FilterUpdateAll
    tmrShow.Enabled = True
    
    Set aColl = Nothing
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Function Group() As clsHsGrpAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Sub Refresh()
    On Error Resume Next
    tmrShow.Enabled = True
End Sub

Private Sub ClearViewAndData()
    On Error Resume Next
    
    ClearResGrid False
    ClearTotGrid False
    
    lblValue.Caption = ""

    m_Grp.Clear
    HedgeCalc.Stocks.Clear
    m_QuoteRequests.Clear
    m_IvComRequests.Clear
    HedgeCalc.Correlations.Clear
End Sub

Private Sub aParams_PriceProfilesChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom
    
    If m_Grp.GroupType = 0 Or m_Grp.ID = 0 Then Exit Sub
    
    bUpdate = False
    On Error Resume Next
    
    For Each aFilterAtom In collUpdUnd
        If Not HedgeCalc.Stocks(aFilterAtom.ID) Is Nothing Then
            bUpdate = True
            Set aFilterAtom = Nothing
            Exit For
        End If
        Set aUnd = HedgeCalc.Instruments(aFilterAtom.ID)
        If Not aUnd Is Nothing Then
            If aUnd.Enabled Then
                bUpdate = True
                Exit For
            End If
        End If
        Set aFilterAtom = Nothing
    Next
    If bUpdate Then SetRefreshHint True
End Sub

Private Sub aParams_UndPriceToleranceChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub DhIvLoader_GotIndexHVData4(ByVal IndexSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
    On Error Resume Next
    Dim aReq As PRICEPROVIDERSLib.QuoteUpdateParams
    aReq.Symbol = IndexSymbol
    aReq.Type = enIDX
    HvIvSave aReq, HV, IV, VolOfVol
End Sub

Private Sub DhIvLoader_GotStockHVData4(ByVal StockSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
    On Error Resume Next
    Dim aReq As PRICEPROVIDERSLib.QuoteUpdateParams
    aReq.Symbol = StockSymbol
    aReq.Type = enSTK
    HvIvSave aReq, HV, IV, VolOfVol
End Sub

Private Sub DhIvLoader_GotIndexIndexCorrData4(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error Resume Next
    Dim aReq1 As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aReq2 As PRICEPROVIDERSLib.QuoteUpdateParams
    
    aReq1.Symbol = IndexSymbol1
    aReq1.Type = enIDX
    aReq2.Symbol = IndexSymbol2
    aReq2.Type = enIDX
    CorrelationSave aReq1, aReq2, Corr / DHIVCORR_NORM, VCorr / DHIVCORR_NORM
End Sub

Private Sub DhIvLoader_GotStockIndexCorrData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error Resume Next
    Dim aReq1 As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aReq2 As PRICEPROVIDERSLib.QuoteUpdateParams
    
    aReq1.Symbol = StockSymbol
    aReq1.Type = enSTK
    aReq2.Symbol = IndexSymbol
    aReq2.Type = enIDX
    CorrelationSave aReq1, aReq2, Corr / DHIVCORR_NORM, VCorr / DHIVCORR_NORM
End Sub

Private Sub DhIvLoader_GotStockStockCorrData4(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error Resume Next
    Dim aReq1 As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aReq2 As PRICEPROVIDERSLib.QuoteUpdateParams
    
    aReq1.Symbol = StockSymbol1
    aReq1.Type = enSTK
    aReq2.Symbol = StockSymbol2
    aReq2.Type = enSTK
    CorrelationSave aReq1, aReq2, Corr / DHIVCORR_NORM, VCorr / DHIVCORR_NORM
End Sub

Private Sub DhIvLoader_LogMessage(ByVal message As String)
    LogEvent EVENT_INFO, message
End Sub

Private Sub DhIvLoader_Progress(ByVal ProgressPercent As Long)
    On Error Resume Next
    pbProgress.Value = ProgressPercent
    DoEvents
End Sub

Private Sub DhIvLoader_ProgressStart()
    On Error Resume Next
    pbProgress.Value = 0
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = .ComboData
            Select Case Col
                Case HFC_GROUP
                    m_nFilter(HFC_GROUP) = nValue
                    If m_nFilter(HFC_GROUP) <> TYPE_ALL Then
                        m_nFilter(HFC_VALUE) = 0
                        ClearViewAndData
                        FilterUpdateValue True
                        AdjustCaption
                    Else
                        m_nFilter(HFC_VALUE) = -1
                        FilterUpdateValue True
                        tmrShow.Enabled = True
                    End If

                Case HFC_VALUE
                    If m_nFilter(HFC_VALUE) = 0 Then
                        sValue = .ColComboList(Col)
                        sValue = Mid$(sValue, InStr(1, sValue, "|") + 1)
                        .ColComboList(Col) = sValue
                        .TextMatrix(Row, Col) = CStr(nValue)
                    End If
                    m_nFilter(HFC_VALUE) = nValue
                    .AutoSize 0, .Cols - 1, , 100

                    tmrShow.Enabled = True

                Case HFC_TYPE
                    
                    m_nFilter(HFC_TYPE) = nValue
                    .ColHidden(HFC_WEIGHTED) = Not CBool(nValue)
                    
                    .AutoSize 0, .Cols - 1, , 100
                    
                    m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
        
                    fgRes.ColHidden(HRC_VEGAEQ) = Not CBool(m_nFilter(HFC_TYPE))
                    fgRes.ColHidden(HRC_POS) = CBool(m_nFilter(HFC_TYPE))
                    fgRes.ColHidden(HRC_CONTRACTS) = CBool(m_nFilter(HFC_TYPE))
        
                    fgRes.AutoSize 0, fgRes.Cols - 1, , 100
        
                    m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
                    
                    ClearTotGrid False
                    ClearResGrid False
                    SetRefreshHint True

                Case HFC_WEIGHTED
                    m_nFilter(HFC_WEIGHTED) = CBool(sValue)
                    
                    .AutoSize 0, .Cols - 1, , 100
                    
                    ClearTotGrid False
                    ClearResGrid False
                    SetRefreshHint True
            End Select
        End If
    End With
End Sub

Private Sub fgFlt_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    With fgFlt
        Select Case Col
            Case HFC_GROUP, HFC_VALUE, HFC_TYPE
                .ComboList = .Cell(flexcpData, Row, Col)
        End Select
    End With
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_HEDGESUMMARY_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_HEDGESUMMARY_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_HEDGESUMMARY_FILTER) Then
        m_bKeyDown(GT_HEDGESUMMARY_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_HEDGESUMMARY_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
            
        End Select
    End If
End Sub

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_HEDGESUMMARY_FILTER
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgFlt
        If Not m_bInProc And Not m_bRecalc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= HFC_GROUP And Col <= MFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            
            If Not Cancel Then
                Cancel = (Col = HFC_VALUE And m_nFilter(HFC_GROUP) = TYPE_ALL)
            End If
            
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgInst_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgInst
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = .RowData(Row)
            Select Case Col
                Case HIC_USE
                    Dim oldEnabl As Boolean
                    Dim newEnabl As Boolean
                    newEnabl = CBool(sValue)
                    oldEnabl = HedgeCalc.Instruments(nValue).Enabled
                    HedgeCalc.Instruments(nValue).Enabled = newEnabl
                    If (newEnabl = True And oldEnabl = False) Then
                        m_iEnableInstr = m_iEnableInstr + 1
                    ElseIf (newEnabl = False And oldEnabl = True And m_iEnableInstr > 0) Then
                        m_iEnableInstr = m_iEnableInstr - 1
                    End If
                        
                    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
            End Select
        End If
    End With
End Sub

Private Sub fgInst_DblClick()
    On Error Resume Next
    With fgInst
        m_enMenuGrid = GT_HEDGESUMMARY_INSTRUMENTS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgInst_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_HEDGESUMMARY_INSTRUMENTS) = True
    End If
End Sub

Private Sub fgInst_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_HEDGESUMMARY_INSTRUMENTS) Then
        m_bKeyDown(GT_HEDGESUMMARY_INSTRUMENTS) = False
        With fgInst
            m_enMenuGrid = GT_HEDGESUMMARY_INSTRUMENTS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
            
        End Select
    End If
End Sub

Private Sub fgInst_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgInst
            m_enMenuGrid = GT_HEDGESUMMARY_INSTRUMENTS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgInst_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgInst
        If Not m_bInProc And Not m_bRecalc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= HFC_GROUP And Col <= MFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            
            If Not Cancel Then
                Cancel = Not CBool(Col = HIC_USE)
            End If
            
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgRes_DblClick()
    On Error Resume Next
    With fgRes
        m_enMenuGrid = GT_HEDGESUMMARY_RESULTS
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgRes_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_HEDGESUMMARY_RESULTS) = True
    End If
End Sub

Private Sub fgRes_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_HEDGESUMMARY_RESULTS) Then
        m_bKeyDown(GT_HEDGESUMMARY_RESULTS) = False
        With fgRes
            m_enMenuGrid = GT_HEDGESUMMARY_RESULTS
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
            
        End Select
    End If
End Sub

Private Sub fgRes_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgRes
            m_enMenuGrid = GT_HEDGESUMMARY_RESULTS
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgTot_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgTot
            m_enMenuGrid = GT_HEDGESUMMARY_TOTAL
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bDataLoad Then
        DhIvLoader.CancelTask
        
        m_bDataLoad = False
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bRecalc Then
        m_bRecalc = False
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bInProc Then
        m_bInProc = False
        AdjustState
    End If
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_HEDGESUMMARY_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_HEDGESUMMARY_INSTRUMENTS
            g_ClipMgr.CopyGridToClipboard fgInst

        Case GT_HEDGESUMMARY_RESULTS
            g_ClipMgr.CopyGridToClipboard fgRes

        Case GT_HEDGESUMMARY_TOTAL
            g_ClipMgr.CopyGridToClipboard fgTot

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_HEDGESUMMARY_FILTER
            CustomizeFltGridLayout

        Case GT_HEDGESUMMARY_INSTRUMENTS
            CustomizeInstGridLayout
        
        Case GT_HEDGESUMMARY_RESULTS
            CustomizeResGridLayout
        
        Case GT_HEDGESUMMARY_TOTAL
            CustomizeTotGridLayout
        
    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, ByRef dPrice#)
    On Error Resume Next
    Select Case True
        Case m_enMenuGrid = GT_HEDGESUMMARY_INSTRUMENTS
            Set aUnd = g_Underlying(fgInst.RowData(m_nMenuGridRow))
        
        Case m_enMenuGrid = GT_HEDGESUMMARY_FILTER And m_nFilter(HFC_GROUP) = TYPE_UNDERLYING
            Set aUnd = g_Underlying(m_nFilter(HFC_VALUE))
    
        Case m_enMenuGrid = GT_HEDGESUMMARY_RESULTS
            Set aUnd = g_Underlying(fgRes.RowData(m_nMenuGridRow))
            
    End Select

    If bIsStock And Not aUnd Is Nothing Then
        dPrice = HedgeCalc.Stocks(aUnd.ID).PriceBid
        If dPrice <= 0# Then dPrice = HedgeCalc.Stocks(aUnd.ID).PriceLast
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, aUnd, dPrice
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, True, 100, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, Nothing, True, 10, dPrice
'    End If
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Hedger Summary View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
    OrderNew False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    OrderNew True
End Sub

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, aUnd, dPrice
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, Nothing, False, 10&, 0#
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Hedger Summary View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_HEDGESUMMARY_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Hedge Summary Filter", "Hedge Summary Filter", fgFlt
'
'        Case GT_HEDGESUMMARY_INSTRUMENTS
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Hedge Summary Instruments", "Hedge Summary Instruments", fgInst
'
'        Case GT_HEDGESUMMARY_RESULTS
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Hedge Summary Results", "Hedge Summary Results", fgRes
'
'        Case GT_HEDGESUMMARY_TOTAL
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Hedge Summary Total", "Hedge Summary Total", fgTot
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim aReq As clsHsRequestAtom, bFinished As Boolean
    Dim aRequest As PRICEPROVIDERSLib.QuoteUpdateParams
    
    aRequest = Request
    bFinished = False
    
    If ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If aRequest.Type <> enGrSTK And aRequest.Type <> enGrIDX Then
                
                Set aReq = m_QuoteRequests(GetColKeyFromQUP(aRequest))
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, aRequest.Symbol & ": " & Description
                
                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequestSupported And aRequest.Type <> enOPT Then
                        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    End If
                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                            Or m_bGroupRequestSupported And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
                End If
            Else
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequestSupported Then
                    m_GroupPriceProvider.CancelGroup
                End If
                
                CalcHedge
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
            End If
        Else
 '           Debug.Assert False
        End If
    
    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim aReq As clsHsRequestAtom
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aOpt As EtsMmHedgeLib.MmHsOptAtom
    Dim dPriceBid#, dPriceAsk#, dPriceLast#, nLotSize&, nID&
    Dim bFinished As Boolean
    bFinished = False
    
    If Params.Type <> enGrSTK And Params.Type <> enGrIDX Then
        Set aReq = m_QuoteRequests(GetColKeyFromQUP(Params))
        If Not aReq Is Nothing Then
            Debug.Print "A " & Params.Symbol
    
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
            
            If Results.LotSize > 0 Then
                nLotSize = Results.LotSize
            Else
                If Params.Type = enOPT Then
                    nLotSize = 100
                Else
                    nLotSize = 1
                End If
            End If
            
            Set aOpt = aReq.Opt
            Set aUnd = aReq.Und
            
            If Not aOpt Is Nothing Then
                aOpt.PriceBid = dPriceBid
                aOpt.PriceAsk = dPriceAsk
                aOpt.PriceLast = dPriceLast
                aOpt.LotSize = nLotSize
            ElseIf Not aUnd Is Nothing Then
                nID = aUnd.ID
                Set aUnd = HedgeCalc.Stocks(nID)
                If Not aUnd Is Nothing Then
                    aUnd.PriceBid = dPriceBid
                    aUnd.PriceAsk = dPriceAsk
                    aUnd.PriceLast = dPriceLast
                    aUnd.LotSize = nLotSize
                End If
                Set aUnd = HedgeCalc.Instruments(nID)
                If Not aUnd Is Nothing Then
                    aUnd.PriceBid = dPriceBid
                    aUnd.PriceAsk = dPriceAsk
                    aUnd.PriceLast = dPriceLast
                    aUnd.LotSize = nLotSize
                End If
            End If
            
            Set aOpt = Nothing
            Set aUnd = Nothing
                    
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            Set aReq = Nothing
                
            If m_bGroupRequestSupported And Params.Type <> enOPT Then
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
            End If
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                        Or m_bGroupRequestSupported And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
        End If
    Else
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If
    
    If bFinished Then
        m_bLastQuoteReqNow = False
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If

        CalcHedge

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
    
    End If
End Sub

Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aReq As clsHsRequestAtom
    Dim nCount&, aUnd As EtsMmHedgeLib.MmHsUndAtom
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        
        nCount = m_QuoteRequests.Count
        If nCount > 0 Then
            m_bInProc = True
            m_bLastQuoteReqNow = True
            AdjustState

            m_nLastQuoteReqCount = nCount
            m_nLastQuoteReqDone = 0&

            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nLastQuoteReqCount
            pbProgress.Visible = True

            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Last quotes request..."
            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents

            If Not m_bGroupRequestSupported Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                aParam.Symbol = ""
                aParam.Type = enMStart
                PriceProvider.RequestLastQuote aParam
                
                For Each aReq In m_QuoteRequests
                    If Not m_bLastQuoteReqNow Then Exit For
                    PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                    DoEvents
                Next
                aParam.Type = enMStop
                PriceProvider.RequestLastQuote aParam
            Else
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                
                For Each aReq In m_QuoteRequests
                    If Not m_bLastQuoteReqNow Then Exit For
                        
                    aParam = aReq.GetQuoteUpdateParam
                    If aParam.Type = enSTK Or aParam.Type = enIDX Then
                        PriceProvider.RequestLastQuote aParam
                        m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                        DoEvents
                    
                        If Not m_bLastQuoteReqNow Then Exit For
                        
                        Debug.Print "Q " & aParam.Symbol
                        
                        If aReq.Und.Opt.Count > 0 Then
                            Debug.Print "QG " & aParam.Symbol
                            aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                            DoEvents
                        End If
                    Else
                        Debug.Print "QO " & aParam.Symbol
                    End If
                Next
            End If

            m_bInProc = False
            AdjustState
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            lblProcess.Caption = ""
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
    
    PriceProvider.CancelLastQuote
    If m_bGroupRequestSupported Then
        m_GroupPriceProvider.CancelGroup
    End If
End Sub

Private Sub frmLayout_OnApply(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    SetGridLayout enGridType, gdGrid
End Sub

Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    SetGridLayout enGridType, gdGrid
    Screen.MousePointer = vbDefault
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    
    Select Case enGridType
        Case GT_HEDGESUMMARY_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
            FormatFltColumns True
            m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
            
        Case GT_HEDGESUMMARY_INSTRUMENTS
            gdGrid.CopyTo m_gdInst
            
            m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
            FormatInstColumns True
            m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
    
        Case GT_HEDGESUMMARY_RESULTS
            gdGrid.CopyTo m_gdRes
            
            m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
            FormatResColumns True
            m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
    
        Case GT_HEDGESUMMARY_TOTAL
            gdGrid.CopyTo m_gdTot
            
            m_GridLock(GT_HEDGESUMMARY_TOTAL).LockRedraw
            FormatTotColumns True
            m_GridLock(GT_HEDGESUMMARY_TOTAL).UnlockRedraw
    
    End Select
    
    UserControl_Resize
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim bRes As Boolean
        
    tmrShow.Enabled = False
    
    ClearViewAndData
    lblStatus.Caption = ""

    m_Grp.ID = m_nFilter(HFC_VALUE)
    m_Grp.GroupType = m_nFilter(HFC_GROUP)
    
    If Not m_bInited Then ' Open new window
         AdjustState
         InitInstData
         UpdateInstGrid
         FormatInstColumns True
         m_bInited = True
         AdjustState
         Exit Sub
    Else ' Update window from file
        UpdateInstData
        UpdateInstGrid
        AdjustState
    End If
    
    If m_bInProc Or m_bDataLoad Or m_iEnableInstr = 0 Then Exit Sub
    
    Screen.MousePointer = vbArrow
    DoEvents
    
    If m_Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
        m_bLastQuoteReqNow = False
    End If

    If m_nFilter(HFC_GROUP) <> 0 And m_nFilter(HFC_VALUE) <> 0 Then
        lblStatus.Visible = False
        imgStop.Visible = True
        imgStopDis.Visible = False
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = g_TradeChannel.Trades.Count
        pbProgress.Visible = True
        lblProcess.Caption = "Data loading..."
        lblProcess.Visible = True
        lblProcess.Refresh

        bRes = PositionsLoad
        If bRes Then bRes = IvDataLoad
            
        Set aStocks = HedgeCalc.Instruments
        For Each aUnd In aStocks
            If aUnd.Enabled Then
                m_QuoteRequests.Add GetColKeyFromUnd(aUnd), aUnd
            End If
        Next
        Set aStocks = Nothing
        
        If bRes Then
            AdjustCaption
            
            lblProcess.Caption = "Last quotes request..."
            lblProcess.Refresh

            RequestLastQuotes
        Else
            If m_nFilter(HFC_GROUP) = TYPE_ALL Then m_nFilter(HFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(HFC_VALUE) = 0
            ClearViewAndData
            lblStatus.Caption = ""
            FilterUpdateAll
            AdjustCaption
            AdjustState

            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
            imgStop.Visible = False
            imgStopDis.Visible = False
            
'            If m_bRunHedger Then RunHedger
        End If

    Else
        AdjustCaption
'        If m_bRunHedger Then RunHedger
    End If

    Screen.MousePointer = vbDefault
    
End Sub

Private Sub UserControl_Hide()
    On Error Resume Next
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
End Sub

Private Sub UserControl_Initialize()
    m_bInProc = False
    m_bDataLoad = False
    m_bRecalc = False
    m_bLastQuoteReqNow = False
    m_bInited = False
    m_bRunHedger = False
    m_fGain = BAD_DOUBLE_VALUE
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
        
'    m_GridLock(GT_HEDGESUMMARY_FILTER).LockRedraw
'    m_GridLock(GT_HEDGESUMMARY_TOTAL).LockRedraw
'    m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
'    m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
'
    With fgInst
        .Top = 0
        .Height = ScaleHeight
        .Width = .ColWidth(0) + .ColWidth(1) + .ColWidth(2) + ScaleX(.GridLineWidth * 2 + 17, vbPixels, vbTwips)
        .Left = ScaleWidth - .Width
    End With

    With fgFlt
        .Top = 0
        .Left = 0
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth - fgInst.Width - GRID_VERT_SPACE
    End With

    With fgTot
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Left = 0
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth - fgInst.Width - GRID_VERT_SPACE
    End With

    With fgRes
        
        .Top = fgTot.Top + fgTot.Height + GRID_VERT_SPACE
        .Left = 0
        .Height = ScaleHeight - .Top
        .Width = ScaleWidth - fgInst.Width - GRID_VERT_SPACE
    End With
'
'    m_GridLock(GT_HEDGESUMMARY_FILTER).UnlockRedraw
'    m_GridLock(GT_HEDGESUMMARY_TOTAL).UnlockRedraw
'    m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
'    m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
End Sub

Private Sub UserControl_Show()
    On Error Resume Next
    fgFlt.SetFocus
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub

Public Sub Term()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_bShutDown = True
    
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
    m_bInProc = False
    m_bInited = False
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
        Set PriceProvider = Nothing
    End If
    
    If Not DhIvLoader Is Nothing Then
        DhIvLoader.CancelTask
    End If
    
    Set m_GroupPriceProvider = Nothing
    Set HedgeCalc = Nothing
    Set DhIvLoader = Nothing
    Set frmLayout = Nothing
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set lblValue = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set geInst = Nothing
    Set geRes = Nothing
    Set aParams = Nothing
End Sub

Private Function PositionsLoad() As Boolean
    On Error GoTo EH
    PositionsLoad = False
    If m_bInProc Then Exit Function
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, nType&
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim aOpt As EtsMmHedgeLib.MmHsOptAtom
    Dim aEnt As EtsGeneralLib.EtsMmEntityAtom, sKey$, fIV#, fHV#, fVolOfVol#
    Dim aGUnd As EtsGeneralLib.UndAtom
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState
    
    Set aStocks = HedgeCalc.Stocks
    nType = m_nFilter(HFC_TYPE)

    If m_Grp.ID <> 0 And m_Grp.GroupType <> 0 Then
        
        Select Case m_Grp.GroupType
            Case TYPE_UNDERLYING
                Set aGUnd = g_Underlying(m_Grp.ID)
                If Not aGUnd Is Nothing Then
                    m_Grp.Name = aGUnd.Symbol
                    Set aGUnd = Nothing
                Else
                    m_bDataLoad = False
                End If
                
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
                
                Set aUndGroup = g_UnderlyingGroup(m_Grp.ID)
                If Not aUndGroup Is Nothing Then
                    m_Grp.Name = aUndGroup.Name
                    Set aUndGroup = Nothing
                Else
                    m_bDataLoad = False
                End If
            
            Case TYPE_TRADER
                Dim aTrader As EtsGeneralLib.EtsTraderAtom
                
                Set aTrader = g_Trader(m_Grp.ID)
                If Not aTrader Is Nothing Then
                    m_Grp.Name = aTrader.Name
                    Set aTrader = Nothing
                Else
                    m_bDataLoad = False
                End If
            
            Case TYPE_STRATEGY
                Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
                
                If m_Grp.ID > 0 Then
                    Set aStrategy = g_Strategy(m_Grp.ID)
                    If Not aStrategy Is Nothing Then
                        m_Grp.Name = aStrategy.Name
                        Set aStrategy = Nothing
                    Else
                        m_bDataLoad = False
                    End If
                Else
                    m_Grp.Name = NO_STRATEGY_NAME
                End If
            
            Case TYPE_ALL
                m_Grp.Name = "<All>"
                
            Case Else
                m_bDataLoad = False
        End Select
        
        On Error Resume Next
        If m_bDataLoad Then
            For Each aTrd In g_TradeChannel.Trades.FilterTradesCV(-1, 9, 0, Nothing, False, g_Params.DisplayZeroPositions)
                If CheckTradeFilter(aTrd) Then
                    If Not aTrd.Und.HaveSyntheticRoots Then
                        sKey = CStr(aTrd.UndID)
                        Set aUnd = aStocks(sKey)
                        If aUnd Is Nothing Then
                            Set aUnd = aStocks.Add(sKey)
                            aUnd.ID = aTrd.Und.ID
                            aUnd.UndType = aTrd.Und.UndType
                            aUnd.Symbol = aTrd.Und.Symbol
                            aUnd.LotSize = aTrd.Und.LotSize
                            aUnd.Qty = 0
                            aUnd.Pos = 0
                        
                            Set aUnd.Dividend = aTrd.Und.Dividend
                            If aUnd.UndType = enCtIndex Then
                                aUnd.Yield = aTrd.Und.Yield
                            End If
                            
                            aUnd.IsAmerican = aTrd.Und.IsAmerican
                            Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
                            Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
    
                            InitVola aUnd
                        End If
    
                        If aTrd.ContractType = enCtOption Then
                            sKey = CStr(aTrd.ContractID)
                            Set aOpt = aUnd.Opt(sKey)
                            
                            If aOpt Is Nothing Then
                                Set aOpt = aUnd.Opt.Add(sKey)
                                aOpt.ID = aTrd.ContractID
                                aOpt.Expiry = aTrd.Opt.Expiry
                                
                                aOpt.ExpiryOV = aTrd.Opt.ExpiryOV
                                aOpt.TradingClose = aTrd.Opt.TradingClose
                                
                                aOpt.LotSize = aTrd.LotSize
                                aOpt.OptType = aTrd.Opt.OptType
                                aOpt.Qty = 0
                                aOpt.UndID = aUnd.ID
                                aOpt.Strike = aTrd.Opt.Strike
                                aOpt.Symbol = aTrd.Opt.Symbol
                                If aTrd.Opt.Flex = 1 Then
                                    aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aOpt.ExpiryOV)
                                Else
                                    aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aOpt.ExpiryOV)
                                End If
                            End If
                            
                            aOpt.Qty = aOpt.Qty + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                            
                            Set aOpt = Nothing
                        Else
                            aUnd.Qty = aUnd.Qty + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                            aUnd.Pos = aUnd.Pos + aTrd.Price * aTrd.Quantity * aTrd.LotSize * IIf(aTrd.IsBuy, 1, -1)
                        End If
                    Else
                        LogEvent EVENT_INFO, aTrd.Symbol & ": Has been removed from calculation due to synthetic option type"
                    End If
                End If
                
                DoEvents
                If Not m_bDataLoad Then Exit For

                IncProgress pbProgress

                Set aTrd = Nothing
            Next

            UnderlyingsAdjustRates True

            PositionsLoad = m_bDataLoad
        End If
    End If

    ' Check for zero position
    If nType = 0 Then
        For Each aUnd In aStocks
            If Not aUnd Is Nothing Then
                For Each aOpt In aUnd.Opt
                    If Not aOpt Is Nothing Then
                        If aOpt.Qty = 0 Then
                            aUnd.Opt.Remove aOpt.ID
                        Else
                            m_QuoteRequests.Add GetColKeyFromOpt(aOpt), , aOpt
                        End If
                        Set aOpt = Nothing
                    End If
                Next
                If (aUnd.Qty = 0) And (aUnd.Opt.Count <= 0) Then
                    LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Delta"
                    aStocks.Remove aUnd.ID
                Else
                    m_QuoteRequests.Add GetColKeyFromUnd(aUnd), aUnd
                End If
                Set aUnd = Nothing
            End If
        Next
    Else
        For Each aUnd In aStocks
            If Not aUnd Is Nothing Then
                For Each aOpt In aUnd.Opt
                    If Not aOpt Is Nothing Then
                        If aOpt.Qty = 0 Then
                            aUnd.Opt.Remove aOpt.ID
                        Else
                            m_QuoteRequests.Add GetColKeyFromOpt(aOpt), , aOpt
                        End If
                        Set aOpt = Nothing
                    End If
                Next
                If aUnd.Opt.Count <= 0 Then
                    LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Vega"
                    aStocks.Remove aUnd.ID
                Else
                    m_QuoteRequests.Add GetColKeyFromUnd(aUnd), aUnd
                End If
                Set aUnd = Nothing
            End If
        Next
    End If
    
    m_bInProc = False
Ex:
    On Error Resume Next
    m_bDataLoad = False
    Set aStocks = Nothing
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
    GoTo Ex
End Function

Private Function IvDataLoad() As Boolean
    On Error GoTo EH
    IvDataLoad = False
    If m_bInProc Then Exit Function

    Dim aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim aInstruments As EtsMmHedgeLib.MmHsUndColl
    Dim nCount&
    
    Set aStocks = HedgeCalc.Stocks
    Set aInstruments = HedgeCalc.Instruments
    
    nCount = aStocks.Count + aInstruments.Count
    nCount = nCount * (nCount + 1)
    If nCount = 0 Then Exit Function
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState
    
    lblStatus.Caption = ""
    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = nCount
    pbProgress.Visible = True
    lblProcess.Caption = "IV data loading..."
    lblProcess.Visible = True
    lblProcess.Refresh
    
    With DhIvLoader
        m_IvComRequests.Clear
        
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = 100
        
        .HVTenor = g_nDefaultTerm
                
        ' Load historical data
        .IndexListClear
        .StocksListClear
    
        If m_bDataLoad Then HvIvLoad aInstruments
        If m_bDataLoad Then HvIvLoad aStocks
    
        nCount = .rsIndexData.RecordCount + .rsStockData.RecordCount
        
        If m_bDataLoad And nCount > 0 Then
            On Error GoTo EH
            If m_bDataLoad And .rsIndexData.RecordCount > 0 Then
                lblProcess.Caption = "Index data loading from IVolatility.com..."
                lblProcess.Refresh
                .LoadIndexHVFromIV
            End If
            If m_bDataLoad And .rsStockData.RecordCount > 0 Then
                lblProcess.Caption = "Stock data loading from IVolatility.com..."
                lblProcess.Refresh
                .LoadStocksHVFromIV
            End If
        End If
    
        ' Load correlation data
        .IndexListClear
        .StocksListClear
    
        If m_bDataLoad Then CorrelationsLoad aInstruments, aInstruments
        If m_bDataLoad Then CorrelationsLoad aInstruments, aStocks
'        If m_bDataLoad Then CorrelationsLoad aStocks, aInstruments
        If m_bDataLoad Then CorrelationsLoad aStocks, aStocks
    
        nCount = .rsIndexData.RecordCount + .rsStockData.RecordCount
        nCount = nCount * (nCount - 1) / 2 + nCount
        
        If m_bDataLoad And nCount > 0 Then
            On Error GoTo EH
            If m_bDataLoad And .rsIndexData.RecordCount > 1 Then
                lblProcess.Caption = "Index-index correlation data loading from IVolatility.com..."
                lblProcess.Refresh
                .LoadIndexIndexCorrFromIV
            End If
            If m_bDataLoad And .rsIndexData.RecordCount > 0 And .rsStockData.RecordCount > 0 Then
                lblProcess.Caption = "Stock-index correlation data loading from IVolatility.com..."
                lblProcess.Refresh
                .LoadStockIndexCorrFromIV
            End If
            If m_bDataLoad And .rsStockData.RecordCount > 1 Then
                lblProcess.Caption = "Stock-stock correlation data loading from IVolatility.com..."
                lblProcess.Refresh
                .LoadStockStockCorrFromIV
            End If
        End If
    End With
            
    m_bInProc = False
    If m_bDataLoad Then IvDataLoad = True
Ex:
    On Error Resume Next
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    m_IvComRequests.Clear
    m_bDataLoad = False
    Set aStocks = Nothing
    Set aInstruments = Nothing
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown And m_bDataLoad Then
        gCmn.ErrorMsgBox m_frmOwner, "Fail to load IV data."
    End If
    GoTo Ex
End Function

Private Sub CorrelationsLoad(aColl1 As EtsMmHedgeLib.MmHsUndColl, aColl2 As EtsMmHedgeLib.MmHsUndColl)
    On Error Resume Next
    
    Dim aUnd1 As EtsMmHedgeLib.MmHsUndAtom, aUnd2 As EtsMmHedgeLib.MmHsUndAtom
    Dim aCorrs As EtsMmHedgeLib.MmHsCorrelationColl
    Dim fPriceCorr#, fVolaCorr#, dtUpdateDate As Date
    
    Set aCorrs = HedgeCalc.Correlations
    
    For Each aUnd1 In aColl1
        If Not m_bDataLoad Then Exit Sub
        If aUnd1.Enabled Then
            For Each aUnd2 In aColl2
                If Not m_bDataLoad Then Exit Sub
                If aUnd2.Enabled And aUnd1.ID <> aUnd2.ID Then
                    fPriceCorr = BAD_DOUBLE_VALUE
                    dtUpdateDate = 0
                    fPriceCorr = aCorrs.Item(aUnd1.ID, aUnd2.ID)
                    If fPriceCorr = BAD_DOUBLE_VALUE Then
                        gDBW.usp_CorrelationDH_Get aUnd1.ID, aUnd2.ID, aUnd1.UndType, aUnd2.UndType, fPriceCorr, fVolaCorr, dtUpdateDate
                        fPriceCorr = IIf(m_nFilter(HFC_TYPE) = 0, fPriceCorr, fVolaCorr)
                        
                        If fPriceCorr > BAD_DOUBLE_VALUE And dtUpdateDate >= Date Then
                            aCorrs.Add aUnd1.ID, aUnd2.ID, fPriceCorr
                        Else
                            If aUnd1.UndType = enCtIndex Then
                                DhIvLoader.IndexListAddSymbol aUnd1.Symbol
                            Else
                                DhIvLoader.StocksListAddSymbol aUnd1.Symbol
                            End If
                            If aUnd2.UndType = enCtIndex Then
                                DhIvLoader.IndexListAddSymbol aUnd2.Symbol
                            Else
                                DhIvLoader.StocksListAddSymbol aUnd2.Symbol
                            End If
                            
                            m_IvComRequests.Add GetColKeyFromUnd(aUnd1), aUnd1
                            m_IvComRequests.Add GetColKeyFromUnd(aUnd2), aUnd2
                        End If
                    End If
                End If

                IncProgress pbProgress
                DoEvents
            Next
        Else
            IncProgressEx pbProgress, aColl2.Count
            DoEvents
        End If
    Next
       
    Set aCorrs = Nothing
End Sub
    
Private Sub CorrelationSave(Params1 As PRICEPROVIDERSLib.QuoteUpdateParams, _
    Params2 As PRICEPROVIDERSLib.QuoteUpdateParams, fCorr As Double, fVCorr As Double)

    On Error Resume Next
    Dim aReq1 As clsHsRequestAtom, aReq2 As clsHsRequestAtom
    Dim nId1 As Long, nId2 As Long, nType1 As Long, nType2 As Long
    Dim fEodCorr As Double
    
    Set aReq1 = m_IvComRequests(GetColKeyFromQUP(Params1))
    Set aReq2 = m_IvComRequests(GetColKeyFromQUP(Params2))
    
    If Not aReq1 Is Nothing And Not aReq1 Is Nothing Then
        nId1 = aReq1.Und.ID
        nId2 = aReq2.Und.ID
        nType1 = aReq1.Und.UndType
        nType2 = aReq2.Und.UndType
        
        fEodCorr = GetBetaFromEod(aReq1.Und.ID, aReq2.Und.ID)
        fCorr = IIf(fEodCorr > 0, fEodCorr, fCorr)
        
        HedgeCalc.Correlations.Remove nId1, nId2
        HedgeCalc.Correlations.Add nId1, nId2, IIf(m_nFilter(HFC_TYPE) = 0, fCorr, fVCorr)
        
        gDBW.usp_CorrelationDH_Save nId1, nId2, nType1, nType2, fCorr, fVCorr
    End If
Ex:
    Set aReq1 = Nothing
    Set aReq2 = Nothing
    Exit Sub
EH:
    LogEvent EVENT_WARNING, aReq1.Und.Symbol & "-" & aReq2.Und.Symbol & ": " & Err.Description
    GoTo Ex
End Sub
    
Private Sub HvIvLoad(aColl As EtsMmHedgeLib.MmHsUndColl)
    On Error Resume Next
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom
    Dim fHV#, fIV#, fVolOfVol#, fVega#, dtUpdateDate As Date

    For Each aUnd In aColl
        If Not m_bDataLoad Then Exit Sub
        If aUnd.Enabled And (Not aUnd.HV > BAD_DOUBLE_VALUE Or _
            Not aUnd.IV > BAD_DOUBLE_VALUE Or Not aUnd.VolOfVol > BAD_DOUBLE_VALUE) Then
            
            fHV = BAD_DOUBLE_VALUE
            fIV = BAD_DOUBLE_VALUE
            fVolOfVol = BAD_DOUBLE_VALUE
            fVega = BAD_DOUBLE_VALUE
            dtUpdateDate = 0
            
            gDBW.usp_DataHistoryDH_Get aUnd.ID, aUnd.UndType, fIV, fHV, fVolOfVol, fVega, dtUpdateDate

            If fHV > BAD_DOUBLE_VALUE And fIV > BAD_DOUBLE_VALUE And _
                fVolOfVol > BAD_DOUBLE_VALUE And dtUpdateDate >= Date Then
                
                aUnd.HV = fHV
                aUnd.IV = fIV
                aUnd.VolOfVol = fVolOfVol
            Else
                If aUnd.UndType = enCtIndex Then
                   DhIvLoader.IndexListAddSymbol aUnd.Symbol
                Else
                   DhIvLoader.StocksListAddSymbol aUnd.Symbol
                End If
                
                m_IvComRequests.Add GetColKeyFromUnd(aUnd), aUnd
            End If
        End If
    
        IncProgress pbProgress
        DoEvents
    Next
End Sub
    
Private Sub HvIvSave(Params As PRICEPROVIDERSLib.QuoteUpdateParams, fHV As Double, fIV As Double, fVolOfVol As Double)
    On Error Resume Next
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom
    Dim aReq As clsHsRequestAtom
    
    Set aReq = m_IvComRequests(GetColKeyFromQUP(Params))
    If Not aReq Is Nothing Then
        Set aUnd = HedgeCalc.Instruments(aReq.Und.ID)
        If Not aUnd Is Nothing Then
            aUnd.HV = fHV
            aUnd.IV = fIV
            aUnd.VolOfVol = fVolOfVol
            Set aUnd = Nothing
        End If
        Set aUnd = HedgeCalc.Stocks(aReq.Und.ID)
        If Not aUnd Is Nothing Then
            aUnd.HV = fHV
            aUnd.IV = fIV
            aUnd.VolOfVol = fVolOfVol
        End If
        
        On Error GoTo EH
        gDBW.usp_DataHistoryDH_Save aReq.Und.ID, aReq.Und.UndType, fIV, fHV, fVolOfVol, BAD_DOUBLE_VALUE
    End If
Ex:
    Set aUnd = Nothing
    Set aReq = Nothing
    Exit Sub
EH:
    LogEvent EVENT_WARNING, aReq.Und.Symbol & ": " & Err.Description
    GoTo Ex
End Sub
    
Private Sub UnderlyingsAdjustRates(ByVal bForceRateUpdate As Boolean)
    On Error Resume Next
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    
    Set aStocks = HedgeCalc.Stocks
    For Each aUnd In aStocks
        UnderlyingAdjustRates aUnd, bForceRateUpdate
    Next
    
    Set aStocks = Nothing
End Sub

Private Sub UnderlyingAdjustRates(ByRef aUnd As EtsMmHedgeLib.MmHsUndAtom, ByVal bForceRateUpdate As Boolean)
    On Error Resume Next
    Dim bUseMidRates As Boolean, cPosThreshold@
    Dim aOpt As EtsMmHedgeLib.MmHsOptAtom, aOptions As EtsMmHedgeLib.MmHsOptColl
    Dim aGUnd As EtsGeneralLib.UndAtom, dPos#, bIsHTB
    
    Dim dtNow As Date
    dtNow = GetNewYorkTime
    
    If aUnd Is Nothing Then Exit Sub
        
    Set aGUnd = g_UnderlyingAll(aUnd.ID)
    Debug.Assert (Not aGUnd Is Nothing)
    
    dPos = aGUnd.UndPosForRates ' aUnd.UndPos
    bIsHTB = aGUnd.IsHTB
    
    If GetIrRuleType = enRateBasedOnPosition Then 'And aUnd.Pos > BAD_DOUBLE_VALUE Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    Else
        bUseMidRates = True
    End If

    If bForceRateUpdate Or aUnd.UseMidRates <> bUseMidRates Or Not bUseMidRates Then
        aUnd.UseMidRates = bUseMidRates
        Set aOptions = aUnd.Opt
        
        Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
        
        For Each aOpt In aOptions
            If bUseMidRates Then
                aOpt.Rate = GetNeutralRate(dtNow, aOpt.ExpiryOV)
            Else
                If dPos < 0 Then
                    aOpt.Rate = GetShortRate(dtNow, aOpt.ExpiryOV)
                Else
                    aOpt.Rate = GetLongRate(dtNow, aOpt.ExpiryOV)
                End If
            End If
            
            If bIsHTBRatesExist Then
                aOpt.HTBRate = GetHTBRate(aUnd.ID, dtNow, aOpt.ExpiryOV)
            Else
                aOpt.HTBRate = BAD_DOUBLE_VALUE
            End If
        Next
    End If

    Set aGUnd = Nothing
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_nFilter(HFC_GROUP) = TYPE_UNDERLYING Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then FilterUpdateValue False
            End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If m_nFilter(HFC_GROUP) = TYPE_GROUP Then FilterUpdateValue False
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If m_nFilter(HFC_GROUP) = TYPE_STRATEGY Then FilterUpdateValue False
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Hedge Summary", "Hedge Summary", fgFlt, fgTot, fgRes
'    Screen.MousePointer = vbDefault
End Sub

Private Sub InitVola(ByRef aUnd As EtsMmHedgeLib.MmHsUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, aUnd.UndType, g_VolaSource
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

Private Sub CalcHedge()
    On Error Resume Next
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aColl As EtsMmHedgeLib.MmHsUndColl
    Dim aOpt As EtsMmHedgeLib.MmHsOptAtom
    Dim nType As Long, bWeighted As Boolean
    Dim nModel As EtsGeneralLib.EtsCalcModelTypeEnum
    Dim nStep As Long
    Dim bGreeksCalculated As Boolean
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    
    If m_bInProc Or m_bRecalc Then Exit Sub

    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    Screen.MousePointer = vbArrow
    m_bInProc = True
    m_bRecalc = True
    nType = m_nFilter(HFC_TYPE)
    nModel = g_Params.CalcModel
    bWeighted = m_nFilter(HFC_WEIGHTED)
    nStep = 10
    bGreeksCalculated = False
    
    ClearResGrid False
    ClearTotGrid False
    SetRefreshHint False
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = HedgeCalc.Stocks.Count + 6 * nStep
    lblProcess.Caption = "Calculate greeks..."
    lblProcess.Refresh
    
    Set aColl = HedgeCalc.Stocks
    For Each aUnd In aColl
'        If PriceMidEx(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, bUseLastPrice) <= BAD_DOUBLE_VALUE Then
        Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
        If aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule) <= 0 Then
            LogEvent EVENT_WARNING, aUnd.Symbol & ": Has been removed from calculation due to undefined Price"
            aColl.Remove aUnd.ID
        Else
            If nType = 0 Then
                If (aUnd.HV <= BAD_DOUBLE_VALUE) Or (aUnd.HV = 0) Then
                    LogEvent EVENT_WARNING, aUnd.Symbol & ": Has been removed from calculation due to zero or undefined HV"
                    aColl.Remove aUnd.ID
                Else
                    For Each aOpt In aUnd.Opt
                        If Not aOpt Is Nothing Then
'                            If PriceMidEx(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, bUseLastPrice) <= BAD_DOUBLE_VALUE Then
                            Debug.Assert (Not aUnd.OptPriceProfile Is Nothing)
                            If aUnd.OptPriceProfile.GetOptPriceMid(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, _
                                    enRoundingRule, g_Params.UseTheoVolatility, 1#) <= 0 Then
                                LogEvent EVENT_WARNING, aOpt.Symbol & ": Has been removed from calculation due to undefined Price"
                                aUnd.Opt.Remove aOpt.ID
                            End If
                            Set aOpt = Nothing
                        End If
                    Next
                    If (aUnd.Qty = 0) And (aUnd.Opt.Count <= 0) Then
                        LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Delta"
                        aColl.Remove aUnd.ID
                    Else
                        aUnd.CalcGreeks GM_DELTA Or GM_VEGA, nModel, bWeighted, g_Params.UseTheoVolatility, _
                            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
                            g_Main.CalculationParametrs
                            
                        If (aUnd.Delta <= BAD_DOUBLE_VALUE) Or (aUnd.Delta = 0) Then
                            LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Delta"
                            aColl.Remove aUnd.ID
                        End If
                    End If
                End If
            Else
                If (aUnd.IV <= BAD_DOUBLE_VALUE) Or (aUnd.IV = 0) Then
                    LogEvent EVENT_WARNING, aUnd.Symbol & ": Has been removed from calculation due to zero or undefined IV"
                    aColl.Remove aUnd.ID
                ElseIf (aUnd.VolOfVol <= BAD_DOUBLE_VALUE) Or (aUnd.VolOfVol = 0) Then
                    LogEvent EVENT_WARNING, aUnd.Symbol & ": Has been removed from calculation due to zero or undefined VolOfVol"
                    aColl.Remove aUnd.ID
                Else
                    For Each aOpt In aUnd.Opt
                        If Not aOpt Is Nothing Then
'                            If PriceMidEx(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, bUseLastPrice) <= BAD_DOUBLE_VALUE Then
                            Debug.Assert (Not aUnd.OptPriceProfile Is Nothing)
                            If aUnd.OptPriceProfile.GetOptPriceMid(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, _
                                    enRoundingRule, g_Params.UseTheoVolatility, 1#) <= 0 Then
                                LogEvent EVENT_WARNING, aOpt.Symbol & ": Has been removed from calculation due to undefined Price"
                                aUnd.Opt.Remove aOpt.ID
                            End If
                            Set aOpt = Nothing
                        End If
                    Next
                    If aUnd.Opt.Count <= 0 Then
                        LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Delta"
                        aColl.Remove aUnd.ID
                    Else
                        aUnd.CalcGreeks GM_VEGA Or GM_DELTA, nModel, bWeighted, g_Params.UseTheoVolatility, _
                            g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, _
                            g_Main.CalculationParametrs
                            
                        If (aUnd.Vega <= BAD_DOUBLE_VALUE) Or (aUnd.Vega = 0) Then
                            LogEvent EVENT_INFO, aUnd.Symbol & ": Has been removed from calculation due to zero Vega"
                            aColl.Remove aUnd.ID
                        End If
                    End If
                End If
            End If
        End If
        
        IncProgress pbProgress
        DoEvents
        If Not m_bRecalc Then Exit For
    Next
    
    bGreeksCalculated = True
    If aColl.Count <= 0 Then
        LogEvent EVENT_INFO, "No symbols with non-zero data in Portfolio"
        GoTo Ex
    End If
    
    On Error GoTo CEH
    If m_bRecalc Then
        lblProcess.Caption = "Calculate risks values..."
        lblProcess.Refresh
        HedgeCalc.CalcYs IIf(nType = 0, enEtsHedgeTypeDelta, enEtsHedgeTypeVega), g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    If m_bRecalc Then
        lblProcess.Caption = "Create equations..."
        lblProcess.Refresh
        HedgeCalc.CreateEqs
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    If m_bRecalc Then
        lblProcess.Caption = "Decompose matrix..."
        lblProcess.Refresh
        HedgeCalc.DecomposeMatrix
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    If m_bRecalc Then
        lblProcess.Caption = "Solve equations..."
        lblProcess.Refresh
        HedgeCalc.SolveEqs IIf(nType = 0, enEtsHedgeTypeDelta, enEtsHedgeTypeVega)
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    If m_bRecalc And nType = 0 Then
        lblProcess.Caption = "Calculate positions values..."
        lblProcess.Refresh
        HedgeCalc.CalcPositions g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    m_fGain = BAD_DOUBLE_VALUE
    If m_bRecalc Then
        lblProcess.Caption = "Calculate gain..."
        lblProcess.Refresh
        m_fGain = HedgeCalc.CalcGain
        DoEvents
    End If
    IncProgressEx pbProgress, nStep
    
    If m_bRecalc Then
        UpdateResGrid
        FormatResColumns True
        UpdateTotGrid
        FormatTotColumns True
    End If
    
Ex:
    Screen.MousePointer = vbDefault
    
    Set aColl = Nothing
    m_bInProc = False
    m_bRecalc = False
    AdjustState

'    If m_bRunHedger Then RunHedger
    Exit Sub
CEH:
    Dim sError$, nError&
    sError = Err.Description
    nError = Err.Number
    Err.Description = Mid(sError, 1, InStr(1, sError, vbLf))
    If Err.Description = "" Then Err.Description = "Unexpected error (" & CStr(nError) & ")"
EH:
    If Not bGreeksCalculated Then m_bRunHedger = False
    If InStr(Err.Description, "Invalid correlation value") <> 0 Then
        Err.Description = "Calculation cannot be done, because not all required corr values were got. Correlation value required for: " & Right(Err.Description, Len(Err.Description) - (InStr(Err.Description, "(") - 1))
        LogEvent EVENT_WARNING, Err.Description
        GoTo Ex
    End If
    LogEvent EVENT_ERROR, Err.Description
    GoTo Ex
End Sub

Private Sub UpdateResGrid()
    On Error Resume Next
    Dim fDelta#, fPos#, fVega#
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim nRow&, i&

    Set aStocks = HedgeCalc.Instruments
    
    With fgRes
        m_GridLock(GT_HEDGESUMMARY_RESULTS).LockRedraw
        
        For i = 1 To .Rows - 1
            Set aUnd = aStocks(.RowData(i))
            If aUnd Is Nothing Or Not aUnd.Enabled Then
                .RemoveItem i
                If i < .Rows Then i = i - 1
            End If
            Set aUnd = Nothing
        Next
        
        If m_nFilter(HFC_TYPE) = 0 Then
            For Each aUnd In aStocks
                If aUnd.Enabled Then
                    fPos = aUnd.Pos
                    fDelta = aUnd.Delta
                
                    nRow = -1
                    nRow = .FindRow(aUnd.ID)
                    If nRow = -1 Then
                        .AddItem aUnd.Symbol & vbTab & STR_NA & vbTab & STR_NA & vbTab & STR_NA
                        nRow = .Rows - 1
                        .RowData(nRow) = aUnd.ID
                    End If
                    .TextMatrix(nRow, HRC_POS) = IIf(fPos > BAD_DOUBLE_VALUE, CStr(fPos), STR_NA)
                    .TextMatrix(nRow, HRC_CONTRACTS) = IIf(fDelta > BAD_DOUBLE_VALUE, CStr(fDelta), STR_NA)
                    .TextMatrix(nRow, HRC_VEGAEQ) = STR_NA
                End If
                
                Set aUnd = Nothing
            Next
        Else
            For Each aUnd In aStocks
                If aUnd.Enabled Then
                    fVega = aUnd.Vega
                    
                    nRow = -1
                    nRow = .FindRow(aUnd.ID)
                    If nRow = -1 Then
                        .AddItem aUnd.Symbol & vbTab & STR_NA & vbTab & STR_NA & vbTab & STR_NA
                        nRow = .Rows - 1
                        .RowData(nRow) = aUnd.ID
                    End If
                    .TextMatrix(nRow, HRC_POS) = STR_NA
                    .TextMatrix(nRow, HRC_CONTRACTS) = STR_NA
                    .TextMatrix(nRow, HRC_VEGAEQ) = IIf(fVega > BAD_DOUBLE_VALUE, CStr(fVega), STR_NA)
                End If
                    
                Set aUnd = Nothing
            Next
                
        End If
                
        .Select 1, 0
        .Sort = flexSortGenericAscending
        .AutoSize 0, .Cols - 1, , 100
        m_GridLock(GT_HEDGESUMMARY_RESULTS).UnlockRedraw
    End With
    
    Set aStocks = Nothing
End Sub

Private Sub UpdateTotGrid()
    On Error Resume Next
    Dim fNET#
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    
    Set aStocks = HedgeCalc.Instruments
    
    fNET = BAD_DOUBLE_VALUE
    
    If m_nFilter(HFC_TYPE) = 0 Then
        For Each aUnd In aStocks
            If aUnd.Enabled And (aUnd.Pos > BAD_DOUBLE_VALUE) Then
                If Not fNET > BAD_DOUBLE_VALUE Then fNET = 0
                fNET = fNET + aUnd.Pos
            End If
        Next
    Else
        For Each aUnd In aStocks
            If aUnd.Enabled And (aUnd.Vega > BAD_DOUBLE_VALUE) Then
                If Not fNET > BAD_DOUBLE_VALUE Then fNET = 0
                fNET = fNET + aUnd.Vega
            End If
        Next
    End If
    
    If m_fGain >= 1 Then
        LogEvent EVENT_WARNING, "Unreliable result: 100% hedge achieved. Try to reload correlations or revise hedging instruments set"
        m_fGain = 1
    End If
    
    With fgTot
        .TextMatrix(1, 0) = IIf(fNET > BAD_DOUBLE_VALUE, CStr(fNET), STR_NA)
        .TextMatrix(1, 1) = IIf(m_fGain > BAD_DOUBLE_VALUE, CStr(m_fGain * 100), STR_NA)
        .AutoSize 0, .Cols - 1, , 100
    End With
    
    Set aStocks = Nothing
End Sub

Private Function SavePorfolioToText() As String
    On Error Resume Next
    Dim sText As String, sSymbol
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aStocks As EtsMmHedgeLib.MmHsUndColl
    Dim aOpt As EtsMmHedgeLib.MmHsOptAtom, aOptions As EtsMmHedgeLib.MmHsOptColl
    Dim fPrice#, fDelta#, fVega#
    
    Set aStocks = HedgeCalc.Stocks
    
    For Each aUnd In aStocks
'        fPrice = PriceMidEx(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, g_Params.UseLastPriceForCalcs)
        Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
        fPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
        
        sSymbol = aUnd.Symbol
        sSymbol = sSymbol & vbTab & "UL"
        sSymbol = sSymbol & vbTab & IIf(aUnd.Qty > BAD_DOUBLE_VALUE, CStr(aUnd.Qty), "")
        sSymbol = sSymbol & vbTab & IIf(fPrice > BAD_DOUBLE_VALUE, CStr(fPrice), "")
        sSymbol = sSymbol & vbTab & "0"
        sSymbol = sSymbol & vbTab & IIf(aUnd.IV > BAD_DOUBLE_VALUE, CStr(aUnd.IV), "")
        sText = sText & sSymbol & vbCrLf
        
        Set aOptions = aUnd.Opt
        
        For Each aOpt In aOptions
            fDelta = BAD_DOUBLE_VALUE
            fVega = BAD_DOUBLE_VALUE
            If aOpt.Delta > BAD_DOUBLE_VALUE And aOpt.LotSize > BAD_DOUBLE_VALUE _
                And aOpt.Qty > BAD_DOUBLE_VALUE Then fDelta = aOpt.Delta * aOpt.LotSize * aOpt.Qty
            If aOpt.Vega > BAD_DOUBLE_VALUE And aOpt.LotSize > BAD_DOUBLE_VALUE _
                And aOpt.Qty > BAD_DOUBLE_VALUE Then fVega = aOpt.Vega * aOpt.LotSize * aOpt.Qty
                
            sSymbol = aUnd.Symbol
            sSymbol = sSymbol & vbTab & Format$(aOpt.Expiry, "YYYYMMDD")
            sSymbol = sSymbol & vbTab & IIf(fDelta > BAD_DOUBLE_VALUE, CStr(fDelta), "")
            sSymbol = sSymbol & vbTab & IIf(fPrice > BAD_DOUBLE_VALUE, CStr(fPrice), "")
            sSymbol = sSymbol & vbTab & IIf(fVega > BAD_DOUBLE_VALUE, CStr(fVega), "")
            sSymbol = sSymbol & vbTab & IIf(aUnd.IV > BAD_DOUBLE_VALUE, CStr(aUnd.IV), "")
            sText = sText & sSymbol & vbCrLf
        Next
        
        Set aOptions = Nothing
    Next
    
    Set aStocks = Nothing
    
    sText = Replace(sText, ",", ".")
    sText = Mid(sText, 1, Len(sText) - 2)
    SavePorfolioToText = sText
    
End Function

Private Function SavePorfolioToFile() As String
    On Error Resume Next
    Dim sText As String
    Dim sBuffer As String
    Dim nFile
    
    sText = SavePorfolioToText
    If sText = "" Then Exit Function
    
    sBuffer = Space$(2048)
    If GetTempPath(2048, sBuffer) = 0 Then Exit Function
    
    sBuffer = Trim$(sBuffer)
    Mid$(sBuffer, Len(sBuffer), 1) = " "
    sBuffer = Trim$(sBuffer)
    
    If Mid$(sBuffer, Len(sBuffer), 1) <> "\" Then sBuffer = sBuffer & "\"
    sBuffer = sBuffer & "etshs" & Format$(g_nHedgeSummaryFileIdx, "000") & ".pfl"
    g_nHedgeSummaryFileIdx = g_nHedgeSummaryFileIdx + 1
    
    nFile = FreeFile()
    Open sBuffer For Output Access Write Lock Write As #nFile
    Print #nFile, sText
    Close #nFile
    
    SavePorfolioToFile = sBuffer
    
End Function

Private Sub RunHedger()
'    On Error Resume Next
'    m_bRunHedger = False
'    RaiseEvent OnRunHedger(SavePorfolioToFile())
End Sub

Public Sub PrepareHedgeFile()
    On Error Resume Next
    m_bRunHedger = True
    tmrShow.Enabled = True
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRefresh.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    'mnuCtxOrderNewStock                "New Stock Order..."
    'mnuCtxOrderNewOption               "New Option Order..."
    'mnuCtxRefresh                      "Refresh"
    'mnuCtxCopy                         "Copy Grid"
    'mnuCtxPrint                        "Print Grid..."
    'mnuCtxGridLayout                   "Grid Layout..."

    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    PopupMenu mnuCtx
End Sub

Private Function GetColKeyFromUnd(aUnd As EtsMmHedgeLib.MmHsUndAtom) As String
    On Error Resume Next
    GetColKeyFromUnd = ""
    GetColKeyFromUnd = CStr(IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex)) & "_" & aUnd.Symbol
End Function

Private Function GetColKeyFromOpt(aOpt As EtsMmHedgeLib.MmHsOptAtom) As String
    On Error Resume Next
    GetColKeyFromOpt = ""
    GetColKeyFromOpt = CStr(enCtOption) & "_" & aOpt.Symbol
End Function

Private Function GetColKeyFromQUP(ByRef aParams As PRICEPROVIDERSLib.QuoteUpdateParams) As String
    On Error Resume Next
    GetColKeyFromQUP = ""
    GetColKeyFromQUP = CStr(IIf(aParams.Type = enOPT, enCtOption, IIf(aParams.Type = enSTK, enCtStock, enCtIndex))) & "_" & aParams.Symbol
End Function

Private Sub UpdateInstData()
    On Error Resume Next
    If m_NewInsts Is Nothing Then Exit Sub
    
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom, aColl As EtsMmHedgeLib.MmHsUndColl
    Dim vntId As Variant
    
    ' clear instruments
    Set aColl = HedgeCalc.Instruments
    For Each aUnd In aColl
        aUnd.Enabled = False
    Next
    
    ' set new "enabled" value
    For Each vntId In m_NewInsts
        Set aUnd = aColl(vntId)
        If Not aUnd Is Nothing Then
            aUnd.Enabled = True
            Set aUnd = Nothing
        End If
        vntId = 0
    Next
    
    Set aColl = Nothing
    SetRefreshHint True
End Sub

Private Sub UpdateInstGrid()
    On Error Resume Next
    If m_NewInsts Is Nothing Then Exit Sub
    
    Dim vntId As Variant, nRow&

    m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).LockRedraw
    ' clear "enabled" flag
    fgInst.Cell(flexcpText, 1, HIC_USE, fgInst.Rows - 1) = "0"
    
    ' update "enabled" flag
    For Each vntId In m_NewInsts
        With fgInst
            nRow = .FindRow(vntId)
            If nRow <> -1 Then .TextMatrix(nRow, HIC_USE) = "1"
            vntId = 0
        End With
    Next
    
    m_GridLock(GT_HEDGESUMMARY_INSTRUMENTS).UnlockRedraw
    Set m_NewInsts = Nothing
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub HandleGridDblClick()
    On Error Resume Next
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
        
        If IsAltPressed Then
            Select Case True
                Case IsCtrlPressed
                    mnuCtxOrderNewOption_Click
                    
                Case IsShiftPressed
                    mnuCtxTntCardNew_Click
                
                Case Else
                    mnuCtxOrderNewStock_Click
            End Select
            
            FixDblClickHandled
        End If
    End If
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgRes, , , sHeader, FileSaveType)
    Screen.MousePointer = vbNormal
End Function

Private Function GetBetaFromEod(ByVal iContractID As Long, ByVal iHedgeSymbolID As Long) As Double
    On Error Resume Next
    Dim rs As ADODB.Recordset
    
    GetBetaFromEod = BAD_DOUBLE_VALUE
    
    Set rs = gDBW.usp_MmIndexUnderlyingBeta_Eod_Get(iContractID, iHedgeSymbolID)
    If rs.RecordCount = 1 Then GetBetaFromEod = ReadDbl(rs!fBeta)
End Function
