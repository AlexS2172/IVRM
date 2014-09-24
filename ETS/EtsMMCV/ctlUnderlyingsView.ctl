VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlUnderlyingsView 
   ClientHeight    =   7095
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8565
   KeyPreview      =   -1  'True
   ScaleHeight     =   7095
   ScaleWidth      =   8565
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   840
      Top             =   4680
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
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
      Height          =   2535
      Left            =   0
      TabIndex        =   1
      Top             =   600
      Width           =   7575
      _cx             =   13361
      _cy             =   4471
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
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator11 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxCustomDividend 
         Caption         =   "Custom Dividends"
      End
      Begin VB.Menu mnuCtxHTBRates 
         Caption         =   "HTB Rates ..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
         Visible         =   0   'False
      End
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
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   0
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   1
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   2
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   3
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   4
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   5
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   6
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   7
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   8
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   9
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   10
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   11
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   12
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   13
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   14
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   15
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   16
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   17
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   18
      End
      Begin VB.Menu mnuCtxApplyToAll 
         Caption         =   "<Apply To All>"
         Index           =   19
      End
      Begin VB.Menu mnuCtxSeparator4 
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
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxHideCol 
         Caption         =   "Hide Column"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlUnderlyingsView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()

Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label

Private m_gdFlt As New clsGridDef
Private m_gdUnd As New clsGridDef

Private m_Und As New clsUvUndColl

Private WithEvents geUnd As clsGridEx
Attribute geUnd.VB_VarHelpID = -1

Private m_nFilter(NFC_TRADER To NFC_HAS_FUTURES) As Long
Private m_sFilterSymbol As String
Private m_nDirtyUndCount As Long

Private m_bInProc As Boolean
Private m_bShutDown As Boolean
Private m_sCurrentOriginalText As String
Private m_bFireEvent As Boolean

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_UNDERLYINGS_FILTER To GT_UNDERLYINGS_LIST) As Boolean
Private m_GridLock(GT_UNDERLYINGS_FILTER To GT_UNDERLYINGS_LIST) As New clsGridLock
Private m_nUndCount As Long
Private m_frmCustDivs As New frmCustomDivs
Private m_frmHTBRates As frmHTBRates

Private m_sComboList(NLC_TYPE To NLC_DIV_FREQ) As String

Private m_sDivTypeComboList(1 To 2) As String

Public m_frmOwner As Form
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Public Function Init() As Boolean
    On Error GoTo EH
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_UNDERLYINGS_FILTER) = False
    m_bKeyDown(GT_UNDERLYINGS_LIST) = False
    m_nUndCount = 0
    m_nDirtyUndCount = 0
    m_bFireEvent = False
    
    m_GridLock(GT_UNDERLYINGS_FILTER).Init fgFlt
    m_GridLock(GT_UNDERLYINGS_LIST).Init fgUnd
    
    InitColumns
    InitGrids
    
   
    Set geUnd = New clsGridEx
    geUnd.Init fgUnd
    
    FormatFltGrid
    FormatFltColumns
    
    InitFltData
    
    FormatUndGrid
    FormatUndColumns
    
    Set TradeChannel = g_TradeChannel
    
    fgFlt.Col = NFC_TRADER
    fgFlt.Row = 1
    
    Set m_frmHTBRates = New frmHTBRates
    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    Init = True
    m_frmCustDivs.InitRs

    LoadDefaultSettings
    AdjustCaption
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to init underlyings view."
End Function

Public Sub ShowData()
    On Error Resume Next
    FilterUpdate
    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdate()
    On Error Resume Next
    Dim nValue&
    
    With fgFlt
        m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
        
        nValue = m_nFilter(NFC_TRADER)
        If g_Trader(nValue) Is Nothing Then nValue = 0
        m_nFilter(NFC_TRADER) = nValue
        .TextMatrix(1, NFC_TRADER) = nValue
        
        nValue = m_nFilter(NFC_GROUP)
        If g_UnderlyingGroup(nValue) Is Nothing Then nValue = 0
        m_nFilter(NFC_GROUP) = nValue
        .TextMatrix(1, NFC_GROUP) = nValue
        
        .TextMatrix(1, NFC_SYMBOL) = IIf(Len(m_sFilterSymbol) <= 0, "<All>", m_sFilterSymbol)
        .TextMatrix(1, NFC_TYPE) = m_nFilter(NFC_TYPE)
        .TextMatrix(1, NFC_ACTIVE) = m_nFilter(NFC_ACTIVE)
        .TextMatrix(1, NFC_OPT_STYLE) = m_nFilter(NFC_OPT_STYLE)
        .TextMatrix(1, NFC_CALENDAR) = m_nFilter(NFC_CALENDAR)
        .TextMatrix(1, NFC_UND_PROFILE) = m_nFilter(NFC_UND_PROFILE)
        .TextMatrix(1, NFC_OPT_PROFILE) = m_nFilter(NFC_OPT_PROFILE)
        .TextMatrix(1, NFC_DIVS) = m_nFilter(NFC_DIVS)
        .TextMatrix(1, NFC_HAS_POS) = m_nFilter(NFC_HAS_POS)
        .TextMatrix(1, NFC_HAS_OPTIONS) = m_nFilter(NFC_HAS_OPTIONS)
        .TextMatrix(1, NFC_HAS_SYNTH) = m_nFilter(NFC_HAS_SYNTH)        '  Uncomment if SU works correctly
        .TextMatrix(1, NFC_HTB) = m_nFilter(NFC_HTB)
        .TextMatrix(1, NFC_HEDGE_SYMBOL) = m_nFilter(NFC_HEDGE_SYMBOL)
        .TextMatrix(1, NFC_DIRTY) = m_nFilter(NFC_DIRTY)
        .TextMatrix(1, NFC_HAS_FUTURES) = m_nFilter(NFC_HAS_FUTURES)
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim sComboList$, sValue$, nValue&
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Dim aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    
    With fgFlt
        m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
            If aTrader.ID = m_nFilter(NFC_TRADER) Then
                nValue = aTrader.ID
                sValue = Trim$(Str$(nValue))
            End If
        Next
        .ColComboList(NFC_TRADER) = sComboList
        .TextMatrix(1, NFC_TRADER) = sValue
        m_nFilter(NFC_TRADER) = nValue
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aUndGroup In g_UnderlyingGroup
            sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
            If aUndGroup.ID = m_nFilter(NFC_GROUP) Then
                nValue = aUndGroup.ID
                sValue = Trim$(Str$(aUndGroup.ID))
            End If
        Next
        .ColComboList(NFC_GROUP) = sComboList
        .TextMatrix(1, NFC_GROUP) = sValue
        m_nFilter(NFC_GROUP) = nValue
    
        m_sFilterSymbol = ""
        .ColComboList(NFC_SYMBOL) = "|<All>"
        .TextMatrix(1, NFC_SYMBOL) = "<All>"
    
        m_sComboList(NLC_TYPE) = "#1;Stock|#2;Index|#3;Basket|#4;Futures"
        .ColComboList(NFC_TYPE) = "#0;<All>|#1;Stock|#2;Index|#3;Basket|#4;Futures"
        .TextMatrix(1, NFC_TYPE) = "0"
        
        .ColComboList(NFC_ACTIVE) = "#0;<All>|#1;Active|#2;Not Active"
        .TextMatrix(1, NFC_ACTIVE) = "0"
    
        m_sComboList(NLC_OPT_STYLE) = "#0;European|#1;American"
        .ColComboList(NFC_OPT_STYLE) = "#0;<All>|#1;American|#2;European"
        .TextMatrix(1, NFC_OPT_STYLE) = "0"
    
'        sValue = "0"
'        nValue = 0
'        sComboList = ""
'        For Each aExpCal In g_ExpCalendar
'            If aExpCal.Hidden = 0 Then
'                sComboList = sComboList & "|#" & Trim$(Str$(aExpCal.ID)) & ";" & aExpCal.Name
'                If aExpCal.ID = m_nFilter(NFC_CALENDAR) Then
'                    nValue = aExpCal.ID
'                    sValue = Trim$(Str$(aExpCal.ID))
'                End If
'            End If
'        Next
'        If Len(sComboList) > 0 Then
'            m_sComboList(NLC_CALENDAR) = Mid$(sComboList, 2)
'        Else
'            m_sComboList(NLC_CALENDAR) = "#0;<None>"
'        End If
'
'        sComboList = "#0;<All>" & sComboList
'        .ColComboList(NFC_CALENDAR) = sComboList
'        .TextMatrix(1, NFC_CALENDAR) = sValue
'        m_nFilter(NFC_CALENDAR) = nValue
    
        sValue = "0"
        nValue = 0
        sComboList = ""
        For Each aPrProf In g_PriceProfile
            If Not aPrProf.IsOptionProfile Then
                sComboList = sComboList & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
                If aPrProf.ID = m_nFilter(NFC_UND_PROFILE) Then
                    nValue = aPrProf.ID
                    sValue = Trim$(Str$(aPrProf.ID))
                End If
            End If
        Next
        If Len(sComboList) > 0 Then
            m_sComboList(NLC_UND_PROFILE) = Mid$(sComboList, 2)
        Else
            m_sComboList(NLC_UND_PROFILE) = "#0;<None>"
        End If
        
        sComboList = "#0;<All>" & sComboList
        .ColComboList(NFC_UND_PROFILE) = sComboList
        .TextMatrix(1, NFC_UND_PROFILE) = sValue
        m_nFilter(NFC_UND_PROFILE) = nValue
    
        sValue = "0"
        nValue = 0
        sComboList = ""
        For Each aPrProf In g_PriceProfile
            If aPrProf.IsOptionProfile Then
                sComboList = sComboList & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
                If aPrProf.ID = m_nFilter(NFC_OPT_PROFILE) Then
                    nValue = aPrProf.ID
                    sValue = Trim$(Str$(aPrProf.ID))
                End If
            End If
        Next
        If Len(sComboList) > 0 Then
            m_sComboList(NLC_OPT_PROFILE) = Mid$(sComboList, 2)
        Else
            m_sComboList(NLC_OPT_PROFILE) = "#0;<None>"
        End If
        
        sComboList = "#0;<All>" & sComboList
        .ColComboList(NFC_OPT_PROFILE) = sComboList
        .TextMatrix(1, NFC_OPT_PROFILE) = sValue
        m_nFilter(NFC_OPT_PROFILE) = nValue
    
        'Stock Div Type Combo List
        m_sDivTypeComboList(1) = "#0;Market|#1;Custom Periodical|#2;Custom Stream"
        'Index Div Type Combo List
        m_sDivTypeComboList(2) = "#0;Market|#1;Custom Periodical|#2;Custom Stream|#3;Stock Basket|#4;Index Yield"
        
        m_sComboList(NLC_DIVS) = "#0;Market|#1;Custom Periodical|#2;Custom Stream|#3;Stock Basket|#4;Index Yield"
        
        m_sComboList(NLC_DIV_FREQ) = "#0;<No Divs>|#1;Annually|#2;Semiannually|#4;Quarterly|#12;Monthly"
        .ColComboList(NFC_DIVS) = "#0;<All>|#1;With Dividends|#2;Without Dividends|#3;With Market Dividends|#4;With Custom Periodical|#5;With Custom Stream"
        .TextMatrix(1, NFC_DIVS) = "0"
        
        .ColComboList(NFC_HAS_POS) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_HAS_POS) = "0"
    
        .ColComboList(NFC_HAS_OPTIONS) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_HAS_OPTIONS) = "0"
    
        .ColComboList(NFC_HAS_SYNTH) = "#0;<All>|#1;Yes|#2;No"      '  Uncomment if SU works correctly
       .TextMatrix(1, NFC_HAS_SYNTH) = "0"                          '  Uncomment if SU works correctly
                
        .ColComboList(NFC_HTB) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_HTB) = "0"
        
        .ColComboList(NFC_HEDGE_SYMBOL) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_HEDGE_SYMBOL) = "0"
        
        .ColComboList(NFC_DIRTY) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_DIRTY) = "0"
        
        .ColComboList(NFC_HAS_FUTURES) = "#0;<All>|#1;Yes|#2;No"
        .TextMatrix(1, NFC_HAS_FUTURES) = "0"
        
        m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsShow(ByVal bReload As Boolean)
    On Error Resume Next
    Dim aContract As EtsGeneralLib.EtsContractAtom, nRow&, nOldRow&, nOldCol&, bNoFilter As Boolean, i&
    Dim aUnd As clsUvUndAtom
    
    m_bInProc = True
    AdjustState
        
    Screen.MousePointer = vbArrow
    DoEvents
    
    lblStatus.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Visible = True
    lblProcess.Caption = IIf(bReload, "Data loading...", "Filter applying...")
    lblProcess.Refresh
    lblProcess.Visible = True
    
    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw

        nOldRow = .Row
        nOldCol = .Col
        
        m_nUndCount = 0
        
        bNoFilter = NoFilter
        
        If bReload Then
            .Rows = 1
            m_Und.Clear
            pbProgress.Max = g_ContractAll.Count
            
            For Each aContract In g_ContractAll
                Set aUnd = New clsUvUndAtom
                
                If aUnd.Init(aContract) Then
                    m_Und.Add CStr(aContract.ID), aUnd
                    
                    .AddItem ""
                    nRow = .Rows - 1
                    
                    .RowData(nRow) = aUnd
                    .TextMatrix(nRow, NLC_KEY) = aUnd.ID
                    
                    UnderlyingUpdate nRow, False, aUnd
                    
                    If bNoFilter Then
                        .RowHidden(nRow) = False
                        m_nUndCount = m_nUndCount + 1
                    Else
                        If CheckUnderlyingFilter(aUnd) Then
                            .RowHidden(nRow) = False
                            m_nUndCount = m_nUndCount + 1
                        Else
                            .RowHidden(nRow) = True
                        End If
                    End If
                End If
                Set aUnd = Nothing
                
                DoEvents
                If m_bShutDown Then Exit Sub
                IncProgress pbProgress
            Next
            
            If m_bShutDown Then Exit Sub
            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh
            
            i = -1
            i = .ColIndex(NLC_SYMBOL)
            If i >= 0 Then
                .Col = i
                .Row = 1
                .Sort = flexSortGenericAscending
            End If
            geUnd.ShowSortImage i, -1
            
            FormatUndColumns
            UnderlyingsUpdateDirtyStatus
        Else
            pbProgress.Max = .Rows - 1
            
            For nRow = 1 To .Rows - 1
                Set aUnd = .RowData(nRow)
                If Not aUnd Is Nothing Then
                    If bNoFilter Then
                        .RowHidden(nRow) = False
                        m_nUndCount = m_nUndCount + 1
                    Else
                        If CheckUnderlyingFilter(aUnd) Then
                            .RowHidden(nRow) = False
                            m_nUndCount = m_nUndCount + 1
                        Else
                            .RowHidden(nRow) = True
                            .IsSelected(nRow) = True
                        End If
                    End If
                    Set aUnd = Nothing
                Else
                    Debug.Assert False
                End If
                
                DoEvents
                If m_bShutDown Then Exit Sub
                IncProgress pbProgress
            Next
            
            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh
        End If
    
        If nOldRow > 0 And m_nUndCount > 0 Then
            If .RowHidden(nOldRow) Then
                For nRow = nOldRow To .Rows - 1
                    If Not .RowHidden(nRow) Then
                        nOldRow = nRow
                        Exit For
                    End If
                Next
            
                If .RowHidden(nOldRow) Then
                    For nRow = nOldRow To 1 Step -1
                        If Not .RowHidden(nRow) Then
                            nOldRow = nRow
                            Exit For
                        End If
                    Next
                End If
            
                If .RowHidden(nOldRow) Then nOldRow = -1
            End If
            
            .Row = nOldRow
            If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
            '.SetFocus
        Else
            If m_nUndCount > 0 Then
                .Row = -1
                
                For nRow = 1 To .Rows - 1
                    If Not .RowHidden(nRow) Then
                        .Row = nRow
                        Exit For
                    End If
                Next
                
                If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
                '.SetFocus
            Else
                .Row = -1
                .Col = -1
                
                'fgFlt.SetFocus
            End If
        End If
        
        lblStatus.Caption = Format$(m_nUndCount, "#,##0") & " underlying(s)"
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
    
    DoEvents
    If m_bShutDown Then Exit Sub
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    
    m_bInProc = False
    AdjustState
    Screen.MousePointer = vbDefault
End Sub

Private Sub UnderlyingUpdate(ByVal nRow As Long, ByVal bUpdateDirtyStatus As Boolean, Optional aRowUnd As clsUvUndAtom = Nothing)
    On Error Resume Next
    
    Dim nCol&, bValidDivs As Boolean, clrBackColor As OLE_COLOR
    Dim aUnd As clsUvUndAtom, sValue$, aEnt As EtsGeneralLib.EtsMmEntityAtom
    Dim aTmpUnd As EtsGeneralLib.UndAtom
    Dim aTmpDiv As EtsGeneralLib.EtsIndexDivAtom
    
    Dim dDate As Double
    Dim dAmount As Double
    Dim dFreq As Double
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim arDate() As Double
    Dim arAmount() As Double
    Dim dTmpDate As Double
    Dim dTmpAmount As Double
    
    ReDim arDate(1)
    ReDim arAmount(1)
    Dim aCustDiv As EtsGeneralLib.EtsDivColl
    Dim m_rs As Recordset
        

    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
    
        If aRowUnd Is Nothing Then
            If nRow >= 0 Then
                Set aUnd = .RowData(nRow)
            End If
        Else
            Set aUnd = aRowUnd
        End If

        If Not aUnd Is Nothing Then
            bValidDivs = aUnd.IsValidDivs
            Set aDiv = aUnd.Dividend
            If Not aDiv Is Nothing Then
                If aDiv.DivType = enDivCustomStream Then
                    dDate = arDate(0)
                    dAmount = arAmount(0)
                    dFreq = 0
    
                    Set aCustDiv = Nothing
                    If aUnd.ID >= 0 Then
                        Set aTmpUnd = g_Underlying(aUnd.ID)
                        If Not aTmpUnd Is Nothing Then
                            Set aTmpDiv = aTmpUnd.Dividend
                            If Not aTmpDiv Is Nothing Then
                                Set aCustDiv = aTmpDiv.CustomDivs
                            End If
                        End If
                    End If
                    
                    If Not aCustDiv Is Nothing Then
                        aCustDiv.GetNearest CLng(Date), 0, arAmount(0), arDate(0)
                        dDate = arDate(0)
                        dAmount = arAmount(0)
                        dFreq = 0
                                                Else
                        Set m_rs = gDBW.usp_MmCustomDividend_Get(aUnd.ID)
                        If Not m_rs.EOF Then
                            Do
                                dTmpAmount = CDbl(m_rs!DivAmnt)
                                dTmpDate = CDbl(m_rs!DivYtes)
                                m_rs.MoveNext
                                If (dTmpAmount > 0) And (dTmpDate > CDbl(Now)) Then
                                    Exit Do
                                End If
                                
                            Loop While Not m_rs.EOF
                        End If
                        m_rs.Close
                        If (dTmpDate > 0) And (dTmpAmount > 0) Then
                            dAmount = dTmpAmount
                            dDate = dTmpDate
                        End If
                    End If
                    Set aCustDiv = Nothing
    
                                                    Else
                    If aDiv.DivType = enDivCustomPeriodical Then
                        dDate = aDiv.DivDateCust
                        dAmount = aDiv.DivAmtCust
                        dFreq = aDiv.DivFreqCust
                                                            Else
                        dDate = aDiv.DivDate
                        dAmount = aDiv.DivAmt
                        dFreq = aDiv.DivFreq
                    
                    End If ' is custom periodical
                End If ' is custom stream
            End If ' Not aDiv is Nothing
            Set aCustDiv = Nothing
            Set aTmpDiv = Nothing
            Set aTmpUnd = Nothing
            
            
            
            Dim i&, nIdx&
            i = 0
            nIdx = m_gdUnd.Idx(0)
            While nIdx >= 0 And i <= NLC_LAST_COLUMN

                nCol = i + 1
                Select Case nIdx
                    Case NLC_SYMBOL
                        .TextMatrix(nRow, nCol) = aUnd.Symbol
                    
                    Case NLC_TYPE
                        .TextMatrix(nRow, nCol) = aUnd.UndTypeName
                    
                    Case NLC_OPT_STYLE
                        .TextMatrix(nRow, nCol) = IIf(aUnd.IsAmerican, 1, 0)
                    
                    Case NLC_CALENDAR
                        .TextMatrix(nRow, nCol) = 0
                    
                    Case NLC_UND_PROFILE
                        If Not aUnd.UndPriceProfile Is Nothing Then
                            .TextMatrix(nRow, nCol) = aUnd.UndPriceProfile.ID
                        End If
                    
                    Case NLC_OPT_PROFILE
                        If Not aUnd.OptPriceProfile Is Nothing Then
                            .TextMatrix(nRow, nCol) = aUnd.OptPriceProfile.ID
                        End If
                        
                    
                    Case NLC_DIVS
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            .TextMatrix(nRow, nCol) = aUnd.UseCustDivsName
                        Else
                            .TextMatrix(nRow, nCol) = ""
                        End If
                    
                    Case NLC_DIV_FREQ
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            .TextMatrix(nRow, nCol) = aUnd.DivFreqName
                        Else
                            .TextMatrix(nRow, nCol) = ""
                        End If
                    
                    Case NLC_DIV_DATE
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            .TextMatrix(nRow, nCol) = IIf(bValidDivs, dDate, STR_NA)
                            If dDate = 0 Then
                                .TextMatrix(nRow, nCol) = STR_NA
                             End If
                        Else
                            .TextMatrix(nRow, nCol) = ""
                        End If
                    
                    Case NLC_DIV_AMT
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            .TextMatrix(nRow, nCol) = IIf(bValidDivs, dAmount, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = ""
                        End If
                    
                    Case NLC_YIELD
                        If (aUnd.UndType <> enCtIndex) Then
                            .TextMatrix(nRow, nCol) = ""
                        Else
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType = enCtIndex, aUnd.Yield, "")
                        End If
                    
                    Case NLC_SKEW
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType <> enCtFuture, aUnd.Skew, "")
                    
                    Case NLC_KURT
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType <> enCtFuture, aUnd.Kurt, "")
                    
                    Case NLC_ACTIVE
                        .TextMatrix(nRow, nCol) = aUnd.IsActive
                    
                    Case NLC_HTB
                        If aUnd.UndType <> enCtFuture Then
                            If aUnd.IsHTB Then
                                .Cell(flexcpChecked, nRow, nCol) = flexChecked
                                '.TextMatrix(nRow, nCol) = "   "
                            Else
                                .Cell(flexcpChecked, nRow, nCol) = flexUnchecked
                                '.TextMatrix(nRow, nCol) = "  "
                            End If
                        Else
                            .Cell(flexcpChecked, nRow, nCol) = flexNoCheckbox
                            .TextMatrix(nRow, nCol) = " "
                        End If
                        
                    Case NLC_ALLOW_LENDING
                        If aUnd.UndType <> enCtFuture And aUnd.IsHTB Then
                            If aUnd.IsAllowLending Then
                                .Cell(flexcpChecked, nRow, nCol) = flexChecked
                            Else
                                .Cell(flexcpChecked, nRow, nCol) = flexUnchecked
                            End If
                        Else
                            .Cell(flexcpChecked, nRow, nCol) = flexNoCheckbox
                            .TextMatrix(nRow, nCol) = " "
                        End If
                        
                    Case NLC_HEDGE_SYMBOL
                        If aUnd.UndType <> enCtFuture Then
                            If aUnd.IsHedgeSymbol Then
                                .Cell(flexcpChecked, nRow, nCol) = flexChecked
                            Else
                                .Cell(flexcpChecked, nRow, nCol) = flexUnchecked
                            End If
                        Else
                            .Cell(flexcpChecked, nRow, nCol) = flexNoCheckbox
                            .TextMatrix(nRow, nCol) = " "
                        End If
                        
                    Case NLC_HAS_POS
                        .TextMatrix(nRow, nCol) = aUnd.HavePos
    
                    Case NLC_HAS_OPTIONS
                        .TextMatrix(nRow, nCol) = aUnd.HaveOptions
                    
                    Case NLC_HAS_SYNTH                                                  '  Uncomment if SU works correctly
                        If aUnd.UndType <> enCtFuture Then
                            If aUnd.HaveSynthRoots Then
                                .Cell(flexcpChecked, nRow, nCol) = flexChecked          '  Uncomment if SU works correctly
                                .TextMatrix(nRow, nCol) = "   "
                            Else
                                .Cell(flexcpChecked, nRow, nCol) = flexUnchecked
                                .TextMatrix(nRow, nCol) = "  "
                            End If
                        Else
                            .Cell(flexcpChecked, nRow, nCol) = flexNoCheckbox
                            .TextMatrix(nRow, nCol) = " "
                        End If                                                          '  Uncomment if SU works correctly
                    
                    Case NLC_SYMBOL_NAME
                        .TextMatrix(nRow, nCol) = aUnd.ContractName
                        
                    Case NLC_DIRTY
                        .TextMatrix(nRow, nCol) = aUnd.IsDirty
                    
                    Case NLC_HAS_FUTURES
                        If aUnd.UndType <> enCtFuture Then
                            If aUnd.HaveFutures Then
                                .Cell(flexcpChecked, nRow, nCol) = flexChecked
                                .TextMatrix(nRow, nCol) = "   "
                            Else
                                .Cell(flexcpChecked, nRow, nCol) = flexUnchecked
                                .TextMatrix(nRow, nCol) = "  "
                            End If
                        Else
                            .Cell(flexcpChecked, nRow, nCol) = flexNoCheckbox
                            .TextMatrix(nRow, nCol) = " "
                        End If
                        
                    Case NLC_FUT_ROOT
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType <> enCtFuture, "", aUnd.FutRootSymbol)
                        
                    Case NLC_FUT_UND
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType <> enCtFuture, "", aUnd.FutUndSymbol)
                        
                    Case NLC_FUT_MATURITY
                        .TextMatrix(nRow, nCol) = IIf(aUnd.UndType <> enCtFuture, "", aUnd.FutMaturity)
                        
                    Case NLC_SOQ
                        If aUnd.UndType = enCtIndex And aUnd.SOQ >= 0 Then
                            .TextMatrix(nRow, nCol) = aUnd.SOQ
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If
                    
                End Select
                
                If bUpdateDirtyStatus Then
                    clrBackColor = m_gdUnd.Col(nIdx).BackColor
                    If aUnd.IsDirty Then clrBackColor = GetAlternateColor(clrBackColor)
                    .Cell(flexcpBackColor, nRow, nCol) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
                End If

                i = i + 1
                nIdx = m_gdUnd.Idx(i)
            Wend
            
            Set aUnd = Nothing
        End If
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
    
End Sub


Private Function NoFilter() As Boolean
    On Error Resume Next
    NoFilter = Not (m_nFilter(NFC_TRADER) <> 0 _
                    Or m_nFilter(NFC_GROUP) <> 0 _
                    Or Len(m_sFilterSymbol) > 0 _
                    Or m_nFilter(NFC_TYPE) <> 0 _
                    Or m_nFilter(NFC_ACTIVE) <> 0 _
                    Or m_nFilter(NFC_OPT_STYLE) <> 0 _
                    Or m_nFilter(NFC_CALENDAR) <> 0 _
                    Or m_nFilter(NFC_UND_PROFILE) <> 0 _
                    Or m_nFilter(NFC_OPT_PROFILE) <> 0 _
                    Or m_nFilter(NFC_DIVS) <> 0 _
                    Or m_nFilter(NFC_HAS_POS) <> 0 _
                    Or m_nFilter(NFC_HAS_OPTIONS) <> 0 _
                    Or m_nFilter(NFC_HAS_SYNTH) <> 0 _
                     Or m_nFilter(NFC_HTB) <> 0 _
                    Or m_nFilter(NFC_HEDGE_SYMBOL) <> 0 _
                    Or m_nFilter(NFC_DIRTY) <> 0 _
                    Or m_nFilter(NFC_HAS_FUTURES) <> 0)

'    NoFilter = Not (m_nFilter(NFC_TRADER) <> 0 _            '  Uncomment and change back if SU works correctly
'                    Or m_nFilter(NFC_GROUP) <> 0 _
'                    Or Len(m_sFilterSymbol) > 0 _
'                    Or m_nFilter(NFC_TYPE) <> 0 _
'                    Or m_nFilter(NFC_ACTIVE) <> 0 _
'                    Or m_nFilter(NFC_OPT_STYLE) <> 0 _
'                    Or m_nFilter(NFC_CALENDAR) <> 0 _
'                    Or m_nFilter(NFC_UND_PROFILE) <> 0 _
'                    Or m_nFilter(NFC_OPT_PROFILE) <> 0 _
'                    Or m_nFilter(NFC_DIVS) <> 0 _
'                    Or m_nFilter(NFC_HAS_POS) <> 0 _
'                    Or m_nFilter(NFC_HAS_OPTIONS) <> 0 _
'                    Or m_nFilter(NFC_HAS_SYNTH) <> 0 _
'                    Or m_nFilter(NFC_HTB) <> 0 _
'                    Or m_nFilter(NFC_DIRTY) <> 0 _
'                    Or m_nFilter(NFC_HAS_FUTURES) <> 0)

End Function

Private Function CheckUnderlyingFilter(ByRef aUnd As clsUvUndAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aCustDiv As EtsGeneralLib.EtsDivColl
    Dim aAmt() As Double
    Dim aDate() As Double
    Dim aCustStreamDate As Double
    ReDim aAmt(1)
    ReDim aDate(1)
    Dim bNoDivs As Boolean
    Dim bNoCustDivs As Boolean
    
    bNoDivs = False
    bNoCustDivs = False
    
    aCustStreamDate = 0
    Set aDiv = aUnd.Dividend
    If aDiv Is Nothing Then
        bNoDivs = True
        bNoCustDivs = True
    End If
    
    CheckUnderlyingFilter = False
    
    If m_nFilter(NFC_TRADER) <> 0 Then
        bMatched = False
        bMatched = Not g_Trader(m_nFilter(NFC_TRADER)).Und(aUnd.ID) Is Nothing
        If Not bMatched Then Exit Function
    End If
    
    If m_nFilter(NFC_GROUP) <> 0 Then
        bMatched = False
        bMatched = Not g_UnderlyingGroup(m_nFilter(NFC_GROUP)).Und(aUnd.ID) Is Nothing
        If Not bMatched Then Exit Function
    End If
    
    If Len(m_sFilterSymbol) > 0 Then
        bMatched = False
        bMatched = InStr(1, aUnd.Symbol, m_sFilterSymbol) = 1
        If Not bMatched Then Exit Function
    End If
    
    If m_nFilter(NFC_TYPE) <> 0 Then
        ' "#0;<All>|#1;Stock|#2;Index|#3;Basket|#4;Futures"
        Dim aIdx As EtsGeneralLib.IndexAtom
        
        Select Case m_nFilter(NFC_TYPE)
            Case 1
                If aUnd.UndType <> enCtStock Then Exit Function
            Case 2
                If aUnd.UndType <> enCtIndex Then Exit Function
            Case 3
                If aUnd.UndType <> enCtIndex Then Exit Function
                If Not aUnd.IsBasket Then Exit Function
            Case 4
                If aUnd.UndType <> enCtFuture Then Exit Function
        End Select
    End If
    
    If m_nFilter(NFC_ACTIVE) <> 0 Then
        ' "#0;<All>|#1;Active|#2;Not Active"
        If aUnd.IsActive <> (m_nFilter(NFC_ACTIVE) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_OPT_STYLE) <> 0 Then
        ' "#0;<All>|#1;American|#2;European"
        If aUnd.IsAmerican <> (m_nFilter(NFC_OPT_STYLE) = 1) Then Exit Function
    End If
        
'    If m_nFilter(NFC_CALENDAR) <> 0 Then
'        If aUnd.ExpCalendarID <> m_nFilter(NFC_CALENDAR) Then Exit Function
'    End If
    
    If m_nFilter(NFC_UND_PROFILE) <> 0 Then
        If aUnd.UndPriceProfile.ID <> m_nFilter(NFC_UND_PROFILE) Then Exit Function
    End If
    
    If m_nFilter(NFC_OPT_PROFILE) <> 0 Then
        If aUnd.OptPriceProfile.ID <> m_nFilter(NFC_OPT_PROFILE) Then Exit Function
    End If
    
    If m_nFilter(NFC_DIVS) <> 0 Then
        ' "#0;<All>|#1;With Divs|#2;Without Divs|#3;With Mkt Divs|#4;With Man Divs"
        Select Case m_nFilter(NFC_DIVS)
            Case 1
                If (aUnd.UndType = enCtStock) Then
                    If ((aDiv.DivType = enDivMarket) And (aDiv.DivFreq <= 0 Or aDiv.DivAmt = 0)) Then Exit Function
                    If ((aDiv.DivType = enDivCustomPeriodical) And (aDiv.DivFreqCust <= 0 Or aDiv.DivAmtCust = 0)) Then Exit Function
                    If ((aDiv.DivType = enDivCustomStream) And (bNoCustDivs)) Then Exit Function
                                                Else
                    If (aUnd.UndType = enCtIndex) And (aUnd.Yield <= 0) Then Exit Function
                    If (aUnd.UndType = enCtFuture) Then Exit Function
                End If
                
            Case 2
                If (aUnd.UndType = enCtStock) Then
                    If ((aDiv.DivType = enDivMarket) And (aDiv.DivFreq > 0 And aDiv.DivAmt > 0)) Then Exit Function
                    If ((aDiv.DivType = enDivCustomPeriodical) And (aDiv.DivFreqCust > 0 And aDiv.DivAmtCust > 0)) Then Exit Function
                    If ((aDiv.DivType = enDivCustomStream) And (Not bNoCustDivs)) Then Exit Function
                                                Else
                    If (aUnd.UndType = enCtIndex) And (aUnd.Yield > 0) Then Exit Function
                End If
            
            Case 3
                If (aUnd.UndType = enCtFuture) Then Exit Function
                If ((aUnd.UndType = enCtStock) And (aUnd.Dividend.DivType <> enDivMarket)) Then Exit Function
    
            Case 4
                If (aUnd.UndType = enCtFuture) Then Exit Function
                If ((aUnd.UndType = enCtStock) And (aUnd.Dividend.DivType <> enDivCustomPeriodical)) Then Exit Function
            
            Case 5
                If (aUnd.UndType = enCtFuture) Then Exit Function
                If ((aUnd.UndType = enCtStock) And (aUnd.Dividend.DivType <> enDivCustomStream)) Then Exit Function
        
        End Select
    End If
    
    Set aDiv = Nothing
    
    If m_nFilter(NFC_HAS_POS) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.HavePos <> (m_nFilter(NFC_HAS_POS) = 1) Then Exit Function
    End If
        
    If m_nFilter(NFC_HAS_OPTIONS) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.HaveOptions <> (m_nFilter(NFC_HAS_OPTIONS) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_HAS_SYNTH) <> 0 Then       '  Uncomment if SU works correctly
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.UndType = enCtFuture Or aUnd.HaveSynthRoots <> (m_nFilter(NFC_HAS_SYNTH) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_HTB) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.UndType = enCtFuture Or aUnd.IsHTB <> (m_nFilter(NFC_HTB) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_HEDGE_SYMBOL) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.UndType = enCtFuture Or aUnd.IsHedgeSymbol <> (m_nFilter(NFC_HEDGE_SYMBOL) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_DIRTY) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.IsDirty <> (m_nFilter(NFC_DIRTY) = 1) Then Exit Function
    End If
    
    If m_nFilter(NFC_HAS_FUTURES) <> 0 Then
        ' "#0;<All>|#1;Yes|#2;No"
        If aUnd.UndType = enCtFuture Or aUnd.HaveFutures <> (m_nFilter(NFC_HAS_FUTURES) = 1) Then Exit Function
    End If
    
    CheckUnderlyingFilter = True
End Function

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_UNDERLYINGS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_UNDERLYINGS_LIST).CopyTo m_gdUnd
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = NFC_COLUMN_COUNT
        
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
    
    With fgUnd
        .Rows = 1
        .Cols = 10
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone ' flexSearchFromTop
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
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionListBox
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightAlways
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
    
        nCols = NFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = NFC_TRADER To NFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = NFC_TRADER To NFC_LAST_COLUMN
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
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
    
        nCols = 0
        For i = 0 To NLC_LAST_COLUMN
            If m_gdUnd.Idx(i) <> NLC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            m_gdUnd.Idx(0) = NLC_SYMBOL
            nCols = 1
        End If
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColDataType(NLC_KEY) = flexDTString
        .ColHidden(NLC_KEY) = True
        .ColKey(NLC_KEY) = NLC_NONE
        
        For i = 0 To NLC_LAST_COLUMN
            nIdx = m_gdUnd.Idx(i)
            If nIdx <> NLC_NONE Then
                .ColDataType(i + 1) = m_gdUnd.Col(nIdx).DataType
                .ColFormat(i + 1) = m_gdUnd.Col(nIdx).Format
                .ColKey(i + 1) = nIdx
                .TextMatrix(0, i + 1) = m_gdUnd.Col(nIdx).Caption
                .ColWidth(i + 1) = IIf(m_gdUnd.Col(nIdx).Width > 0, m_gdUnd.Col(nIdx).Width, -1)
                
                If nIdx >= NLC_OPT_STYLE And nIdx <= NLC_OPT_PROFILE Then
                    .ColComboList(i + 1) = m_sComboList(nIdx)
                Else
                    .ColComboList(i + 1) = ""
                End If
                
                If nIdx = NLC_DIV_DATE Then
                    .ColEditMask(i + 1) = "99/99/9999"
                Else
                    .ColEditMask(i + 1) = ""
                End If
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geUnd.AdjustSortImages
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
End Sub

Private Sub FormatUndColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
        
        aFont.Name = m_gdUnd.Font.Name
        aFont.Size = m_gdUnd.Font.Size
        aFont.Bold = m_gdUnd.Font.Bold
        aFont.Italic = m_gdUnd.Font.Italic
        aFont.Strikethrough = m_gdUnd.Font.Strikethrough
        aFont.Underline = m_gdUnd.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> NLC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).BackColor <> 0, m_gdUnd.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).ForeColor <> 0, m_gdUnd.Col(nIdx).ForeColor, RGB(1, 1, 1))

                If nRows > 1 Then
'                    If nIdx = NLC_TYPE Or nIdx = NLC_ACTIVE Or nIdx = NLC_HAS_POS _         '  Uncomment and change back if SU works correctly
'                       Or nIdx = NLC_HAS_OPTIONS Or nIdx = NLC_HAS_SYNTH _
'                      Or nIdx = NLC_HAS_FUTURES Or nIdx = NLC_HTB Then
'                        .ColAlignment(i) = flexAlignCenterCenter
'                    Else
'                        .ColAlignment(i) = flexAlignGeneral
'                    End If
                    
                    If nIdx = NLC_TYPE Or nIdx = NLC_ACTIVE Or nIdx = NLC_HAS_POS _
                        Or nIdx = NLC_HAS_OPTIONS Or nIdx = NLC_HAS_SYNTH _
                        Or nIdx = NLC_HAS_FUTURES Or nIdx = NLC_HTB Or nIdx = NLC_ALLOW_LENDING _
                        Or nIdx = NLC_HEDGE_SYMBOL Then
                        .ColAlignment(i) = flexAlignCenterCenter
                    Else
                        .ColAlignment(i) = flexAlignGeneral
                    End If
                    .Cell(flexcpPictureAlignment, 1, i, .Rows - 1, i) = flexPicAlignCenterCenter
                    
                End If
            End If
        Next

        If m_gdUnd.HorizLinesVisible Then
            If m_gdUnd.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdUnd.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdUnd.GridLinesColor

        .BackColorSel = m_gdUnd.BackColorSel
        .ForeColorSel = m_gdUnd.ForeColorSel
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim aFilterAtom As EtsGeneralLib.EtsMmFilterAtom, nRow&
    Dim aUnd As clsUvUndAtom, aContract As EtsGeneralLib.EtsContractAtom
    
    If m_bFireEvent Then Exit Sub
    
    On Error Resume Next
    For Each aFilterAtom In collUpdUnd
        Set aContract = g_ContractAll(aFilterAtom.ID)
        Set aUnd = m_Und(CStr(aFilterAtom.ID))
        If Not aContract Is Nothing And Not aUnd Is Nothing Then
            aUnd.Init aContract
            nRow = fgUnd.FindRow(aUnd.ID, 1, NLC_KEY, , True)
            If nRow > 0 Then UnderlyingUpdate nRow, False, aUnd
        End If
        Set aFilterAtom = Nothing
        Set aContract = Nothing
        Set aUnd = Nothing
    Next
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
        
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            If Col <> NFC_SYMBOL Then
                nValue = CLng(sValue)
                If Err.Number <> 0 Then nValue = val(sValue)
                If Err.Number = 0 Then
                    m_nFilter(Col) = nValue
                    UnderlyingsShow False
                    .AutoSize 0, .Cols - 1, , 100
                Else
                    FilterUpdate
                End If
            Else
                sValue = UCase$(sValue)
                If UCase$(sValue) = "<ALL>" Then sValue = ""
                
                If sValue = "" Then
                    .TextMatrix(1, NFC_SYMBOL) = "<All>"
                Else
                    .TextMatrix(1, NFC_SYMBOL) = sValue
                End If
                
                If m_sFilterSymbol <> sValue Then
                    m_sFilterSymbol = sValue
                    
                    UnderlyingsShow False
                    .AutoSize 0, .Cols - 1, , 100
                End If
            End If
        End If
    End With
End Sub

Private Sub fgFlt_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nIdx&
    
    With fgUnd
        nIdx = .ColKey(Col)
        If nIdx = NFC_SYMBOL Then
            .EditMaxLength = 20
        End If
    End With
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_UNDERLYINGS_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_UNDERLYINGS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_UNDERLYINGS_FILTER) Then
        m_bKeyDown(GT_UNDERLYINGS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_UNDERLYINGS_FILTER
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
            m_enMenuGrid = GT_UNDERLYINGS_FILTER
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgFlt_SetupEditStyle(ByVal Row As Long, ByVal Col As Long, ByVal IsCombo As Boolean, style As Long, StyleEx As Long)
    On Error Resume Next
'    If Row = 1 And fgFlt.ColKey(Col) = NFC_SYMBOL Then Style = Style Or CBS_UPPERCASE
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgFlt
        If Not m_bInProc And Not m_bShutDown And Col >= NFC_TRADER And Col <= NFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgUnd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - NLC_SYMBOL
    nNewPos = Position - NLC_SYMBOL
    nIdx = m_gdUnd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then
                m_gdUnd.Idx(i) = m_gdUnd.Idx(i - 1)
            End If
        Next
        m_gdUnd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then
                m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
            End If
        Next
        m_gdUnd.Idx(nNewPos) = nIdx
    End If
End Sub

Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        With fgUnd
            nIdx = .ColKey(Col)
            If nIdx >= NLC_SYMBOL And nIdx <= NLC_LAST_COLUMN Then
                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgUnd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nIdx&, aUnd As clsUvUndAtom
    
    With fgUnd
        nIdx = .ColKey(Col)
        
        .ComboList = ""
        If Not m_bInProc And m_gdUnd.Col(nIdx).CanEdit Then
            Set aUnd = .RowData(Row)
            If Not aUnd Is Nothing Then
                Select Case nIdx
                        
                    Case NLC_DIVS
                        If (aUnd.UndType = enCtStock) Then
                            .ComboList = m_sDivTypeComboList(1)
                        ElseIf (aUnd.UndType = enCtIndex) Then
                            .ComboList = m_sDivTypeComboList(2)
                        End If
                    Case NLC_DIV_FREQ
                        If aUnd.Dividend.DivType = enDivCustomStream _
                           Or aUnd.Dividend.DivType = enDivStockBasket _
                           Or aUnd.Dividend.DivType = enDivIndexYield Then
                          Cancel = True
                         Else
                            If aUnd.UndType = enCtStock Or enCtIndex Then .ComboList = m_sComboList(nIdx)
                         End If
                    Case NLC_DIV_DATE, NLC_DIV_AMT
                        If aUnd.Dividend.DivType = enDivCustomStream _
                           Or aUnd.Dividend.DivType = enDivStockBasket _
                           Or aUnd.Dividend.DivType = enDivIndexYield Then
                               Cancel = True
                        End If
                    Case NLC_SOQ
                        If aUnd.UndType = enCtIndex Then
                            .TextMatrix(Row, Col) = aUnd.SOQ
                            Cancel = False
                        Else
                            Cancel = True
                        End If
                        
                End Select
                Set aUnd = Nothing
            End If
        End If
    End With
End Sub

Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nIdx&, aUnd As clsUvUndAtom
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    If m_bInProc Then Exit Sub
    
    With fgUnd
        nIdx = .ColKey(Col)
        
        If m_gdUnd.Col(nIdx).CanEdit Then
            Set aUnd = .RowData(Row)
            If Not aUnd Is Nothing Then
                Select Case nIdx
                    Case NLC_DIVS, NLC_DIV_FREQ
                        Cancel = (aUnd.UndType <> enCtStock And aUnd.UndType <> enCtIndex)
                    
                    Case NLC_DIV_DATE, NLC_DIV_AMT
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            Cancel = Not aUnd.IsValidDivs
                        End If
                    
                    Case NLC_YIELD
                        Cancel = (aUnd.UndType <> enCtIndex)
                        
                    Case NLC_SKEW, NLC_KURT, NLC_HTB
                        Cancel = (aUnd.UndType = enCtFuture)
                        
                    Case NLC_ALLOW_LENDING
                        Cancel = (aUnd.UndType = enCtFuture Or Not aUnd.IsHTB)
                        
                    Case NLC_HEDGE_SYMBOL
                        Cancel = (aUnd.UndType = enCtFuture)
        
                    Case Else 'NLC_OPT_STYLE, NLC_CALENDAR, NLC_UND_PROFILE, NLC_OPT_PROFILE, NLC_SYMBOL_NAME, NLC_ACTIVE
                        Cancel = False
        
                End Select
                Set aUnd = Nothing
            End If
        End If
    
        If Not Cancel Then
            m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End If
    End With
End Sub

Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&, aUnd As clsUvUndAtom, sValue$, nValue&
    Dim dValue#, dtValue As Date, bValue As Boolean, bOldDirty As Boolean
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    
    With fgUnd
        nIdx = .ColKey(Col)
        
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            Set aUnd = .RowData(Row)
            Set aDiv = aUnd.Dividend
            
            If Not aUnd Is Nothing Then
        
                Select Case nIdx
                    Case NLC_OPT_STYLE
                        bValue = CBool(.ComboData)
                        If aUnd.IsAmerican <> bValue Then
                            aUnd.IsAmerican = bValue
                        End If
                        
                    Case NLC_CALENDAR
                        nValue = .ComboData
'                        If aUnd.ExpCalendarID <> nValue Then
'                            aUnd.ExpCalendarID = nValue
'                        End If
                        
                    Case NLC_UND_PROFILE
                        nValue = .ComboData
                        If aUnd.UndPriceProfile.ID <> nValue Then
                            Set aUnd.UndPriceProfile = g_PriceProfile(nValue)
                        End If
                        
                    Case NLC_OPT_PROFILE
                        nValue = .ComboData
                        If aUnd.OptPriceProfile.ID <> nValue Then
                            Set aUnd.OptPriceProfile = g_PriceProfile(nValue)
                        End If
                        
                    Case NLC_DIVS
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            nValue = .ComboData
                            If aDiv.DivType <> nValue Then
                            If (aDiv.DivType = enDivMarket) And (nValue = 1) Then
                                If (aDiv.DivAmtCust = 0) Then
                                    aDiv.DivAmtCust = aDiv.DivAmt
                                End If
                                
                                If (aDiv.DivDateCust = 0) Or (aDiv.DivDateCust = CLng(Now)) Then
                                    aDiv.DivDateCust = aDiv.DivDate
                                End If
                                
                                If (aDiv.DivFreqCust = 0) Then
                                    aDiv.DivFreqCust = aDiv.DivFreq
                                End If
                            End If

                                aDiv.DivType = nValue
                            End If
                        End If
                        
                    Case NLC_DIV_FREQ
                        
                        If aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            nValue = .ComboData
                            If aDiv.DivType = enDivCustomPeriodical Then
                                If aDiv.DivFreqCust <> nValue Then
                                    aDiv.DivFreqCust = nValue
                                    If aDiv.DivFreqCust <> 0 Then
                                        If aDiv.DivDateCust <= 0 Then
                                            aDiv.DivDateCust = Date
                                        End If
                                    End If
                                End If
                            End If
                            
                            If aDiv.DivType = enDivMarket Then
                                If aDiv.DivFreq <> nValue Then
                                    aDiv.DivType = enDivCustomPeriodical
                                    aDiv.DivFreqCust = nValue
                                    aDiv.DivAmtCust = aDiv.DivAmt
                                    aDiv.DivDateCust = aDiv.DivDate
                                    
                                    If aDiv.DivFreqCust <> 0 Then
                                        If aDiv.DivDateCust <= 0 Then
                                            aDiv.DivDateCust = Date
                                        End If
                                    End If
                                End If
                            End If
                        
                        End If
                                                
                    Case NLC_DIV_DATE
                        dtValue = CDate(sValue)
                        If dtValue > 0 And aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            
                            If aDiv.DivType = enDivCustomPeriodical Then
                                If aDiv.DivDateCust <> dtValue Then
                                    aDiv.DivDateCust = dtValue
                                End If
                            End If
                            
                            If aDiv.DivType = enDivMarket Then
                                If aDiv.DivDate <> dtValue Then
                                    aDiv.DivType = enDivCustomPeriodical
                                    aDiv.DivFreqCust = aDiv.DivFreq
                                    aDiv.DivAmtCust = aDiv.DivAmt
                                    aDiv.DivDateCust = dtValue
                                    
                                    If aDiv.DivFreqCust <> 0 Then
                                        If aDiv.DivDateCust <= 0 Then
                                            aDiv.DivDateCust = Date
                                        End If
                                    End If
                                End If
                            End If
                        End If
                        
                    Case NLC_DIV_AMT
                        dValue = .ValueMatrix(Row, Col)
                        If dValue > 0 And aUnd.UndType = enCtStock Or aUnd.UndType = enCtIndex Then
                            
                            If aDiv.DivType = enDivCustomPeriodical Then
                                If aDiv.DivAmtCust <> dValue Then
                                    aDiv.DivAmtCust = dValue
                                End If
                            End If
                            
                            If aDiv.DivType = enDivMarket Then
                                If aDiv.DivAmt <> dValue Then
                                    aDiv.DivType = enDivCustomPeriodical
                                    aDiv.DivFreqCust = aDiv.DivFreq
                                    aDiv.DivAmtCust = dValue
                                    aDiv.DivDateCust = aDiv.DivDate
                                    
                                    If aDiv.DivFreqCust <> 0 Then
                                        If aDiv.DivDateCust <= 0 Then
                                            aDiv.DivDateCust = Date
                                        End If
                                    End If
                                End If
                            End If
                        End If
                        
                    Case NLC_YIELD
                        If aUnd.UndType = enCtIndex Then
                            dValue = .ValueMatrix(Row, Col)
                            If aUnd.Yield <> dValue Then
                                aUnd.Yield = dValue
                            End If
                        End If
                        
                    Case NLC_SKEW
                        If aUnd.UndType <> enCtFuture Then
                            dValue = .ValueMatrix(Row, Col)
                            If aUnd.Skew <> dValue Then
                                aUnd.Skew = dValue
                            End If
                        End If
                        
                    Case NLC_SOQ
                        If aUnd.UndType = enCtIndex Then
                            dValue = .ValueMatrix(Row, Col)
                            If aUnd.SOQ <> dValue Then
                                aUnd.SOQ = dValue
                            End If
                        End If
                        
                    Case NLC_KURT
                        If aUnd.UndType <> enCtFuture Then
                            dValue = .ValueMatrix(Row, Col)
                            If aUnd.Kurt <> dValue Then
                                aUnd.Kurt = dValue
                            End If
                        End If
                        
                    Case NLC_ACTIVE
                        bValue = CBool(sValue)
                        If aUnd.IsActive <> bValue Then
                            aUnd.IsActive = bValue
                        End If
                        
                    Case NLC_HTB
                        If aUnd.UndType <> enCtFuture Then
                            bValue = CBool(sValue)
                            If aUnd.IsHTB <> bValue Then
                                aUnd.IsHTB = bValue
                                If Not aUnd.IsHTB Then aUnd.IsAllowLending = False
                            End If
                        End If
                        
                    Case NLC_ALLOW_LENDING
                        If aUnd.UndType <> enCtFuture And aUnd.IsHTB Then
                            bValue = CBool(sValue)
                            If aUnd.IsAllowLending <> bValue Then
                                aUnd.IsAllowLending = bValue
                            End If
                        End If
                        
                    Case NLC_HEDGE_SYMBOL
                        If aUnd.UndType <> enCtFuture Then
                            bValue = CBool(sValue)
                            If aUnd.IsHedgeSymbol <> bValue Then
                                aUnd.IsHedgeSymbol = bValue
                            End If
                        End If
                        
                End Select
                
                bOldDirty = aUnd.IsDirty
                aUnd.AdjustDirty
                
                UnderlyingUpdate Row, True, aUnd
                
                If bOldDirty <> aUnd.IsDirty Then
                    m_nDirtyUndCount = m_nDirtyUndCount + IIf(aUnd.IsDirty, 1, -1)
                    Debug.Assert m_nDirtyUndCount >= 0 And m_nDirtyUndCount <= m_Und.Count
                    AdjustState
                    AdjustCaption
                End If
                
                Set aUnd = Nothing
            End If
            
        End If
    End With
End Sub

Private Sub fgUnd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fgUnd.ColKey(Col)
    nNewIdx = fgUnd.ColKey(Position)
    
    If nOldIdx = NLC_NONE Or nNewIdx = NLC_NONE Then
        Position = Col
    ElseIf Not m_gdUnd.Col(nOldIdx).CanChangeOrder Or Not m_gdUnd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgUnd_DblClick()
    On Error Resume Next
    With fgUnd
        m_enMenuGrid = GT_UNDERLYINGS_LIST
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_UNDERLYINGS_LIST) = True
    End If
End Sub

Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_UNDERLYINGS_LIST) Then
        m_bKeyDown(GT_UNDERLYINGS_LIST) = False
        With fgUnd
            m_enMenuGrid = GT_UNDERLYINGS_LIST
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                mnuCtxTradeNew_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
        
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                mnuCtxAutosizeGrid_Click
    
            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                mnuCtxAutosizeCol_Click
        End Select
    End If
End Sub

Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgUnd
            m_enMenuGrid = GT_UNDERLYINGS_LIST
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                And .SelectedRows <= 1 Then
                .Row = m_nMenuGridRow
            End If
            ShowPopup
        End With
    End If
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    Dim aUnd As clsUvUndAtom, aMenu As VB.Menu
    Dim bUseCustomDiv As Boolean

    'mnuCtxTradeNew          "New Trade..."
    'mnuCtxOrderNewStock     "New Stock Order..."
    'mnuCtxOrderNewOption    "New Option Order..."
    'mnuCtxApplyToAll        "<Apply To All>"
    'mnuCtxRefresh           "Refresh"
    'mnuCtxCopy              "Copy Grid"
    'mnuCtxPrint             "Print Grid..."
    'mnuCtxHideCol           "Hide Column"
    'mnuCtxAutosizeCol       "Autosize Column"
    'mnuCtxAutosizeGrid      "Autosize Grid"
    'mnuCtxGridLayout        "Grid Layout..."

    mnuCtxTradeNew.Enabled = False
    mnuCtxRefresh.Enabled = Not m_bInProc
    mnuCtxCopy.Enabled = Not m_bInProc
    
   
    For Each aMenu In mnuCtxApplyToAll
        aMenu.Enabled = False
        aMenu.Visible = False
    Next
    
    mnuCtxSeparator3.Visible = False
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    Select Case m_enMenuGrid
        Case GT_UNDERLYINGS_FILTER
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True

            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_UNDERLYINGS_LIST
            With fgUnd
                If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                    Set aUnd = .RowData(m_nMenuGridRow)
                    
                    bUseCustomDiv = (aUnd.UndType = enCtIndex And aUnd.IsBasket)
                    If bUseCustomDiv = False Then bUseCustomDiv = (Not aUnd.Dividend Is Nothing And aUnd.Dividend.DivType = enDivCustomStream)
                    mnuCtxCustomDividend.Enabled = bUseCustomDiv
                    
                    mnuCtxHTBRates.Enabled = aUnd.IsHTB
                End If
                
                If Not m_bInProc And Not aUnd Is Nothing _
                                            And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                            And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                
                    Dim bEnabled As Boolean, i&, nRow&
                    
                    bEnabled = True
                    If .Rows > 1 Then
                        mnuCtxTradeNew.Enabled = (.SelectedRows = 1)
                        AdjustApplyToAllMenu
                    End If
                End If
                
                Set aUnd = Nothing
                
                mnuCtxHideCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdUnd.Idx(1) <> NLC_NONE)
                mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
                mnuCtxAutosizeGrid.Enabled = True
                
                PopupMenu mnuCtx, , , , mnuCtxTradeNew
            End With
            
    End Select
End Sub

Private Function SplitComboList(nIdx As Long) As Variant
    Dim vList, vListIDs, vListNames, n&, sValue$, nPos&
    
    Debug.Assert nIdx >= LBound(m_sComboList) And nIdx <= UBound(m_sComboList)
    vList = Split(m_sComboList(nIdx), "|")
    
    ReDim vListIDs(0 To UBound(vList))
    ReDim vListNames(0 To UBound(vList))
    
'   "#1;Stock|#2;Index|#3;Basket|#4;Futures"
    For n = 0 To UBound(vList)
        sValue = vList(n)
        nPos = InStrRev(sValue, ";")
        vListIDs(n) = Mid(sValue, 2, nPos - 2)
        vListNames(n) = Mid(sValue, nPos + 1)
    Next
    
    ReDim vList(0 To 1) As Variant
    vList(0) = vListIDs
    vList(1) = vListNames
    
    SplitComboList = vList
End Function

Private Sub AdjustApplyToAllMenu()
    On Error Resume Next
    Dim aUnd As clsUvUndAtom
    Dim nSelCount&, i&, bValue As Boolean, nRow&, nColIdx&, sRowValue$
    Dim vList, sValue$, aMenu As Menu, nBound&, bFuturesSelected As Boolean
    
    With fgUnd
        nColIdx = .ColKey(m_nMenuGridCol)
        If .SelectedRows <= 1 _
            Or nColIdx <> NLC_OPT_STYLE _
            And nColIdx <> NLC_CALENDAR _
            And nColIdx <> NLC_UND_PROFILE _
            And nColIdx <> NLC_OPT_PROFILE _
            And nColIdx <> NLC_ACTIVE _
            And nColIdx <> NLC_HTB Then Exit Sub
        
        bFuturesSelected = False
        nSelCount = 0
        For i = 0 To .SelectedRows - 1
            nRow = .SelectedRow(i)
            If Not .RowHidden(nRow) Then
                Set aUnd = .RowData(nRow)
                If Not aUnd Is Nothing Then
                    nSelCount = nSelCount + 1
                    If aUnd.UndType = enCtFuture Then bFuturesSelected = True
                    Set aUnd = Nothing
                End If
                
                If nSelCount > 1 Then Exit For
            End If
        Next
        
        bValue = False
        If nSelCount > 1 Then
            Select Case nColIdx
                Case NLC_OPT_STYLE
                    vList = SplitComboList(nColIdx)
                    If IsArray(vList) Then
                        nBound = UBound(vList(0))
                        If nBound >= mnuCtxApplyToAll.Count Then nBound = mnuCtxApplyToAll.Count - 1
                        For i = 0 To nBound
                            mnuCtxApplyToAll(i).Caption = "Set selected to " & vList(1)(i)
                            mnuCtxApplyToAll(i).Tag = vList(0)(i)
                            mnuCtxApplyToAll(i).Visible = True
                        Next
                    End If
                    
                Case NLC_ACTIVE
                     mnuCtxApplyToAll(0).Caption = "Set selected to Active"
                     mnuCtxApplyToAll(0).Tag = True
                     mnuCtxApplyToAll(1).Caption = "Set selected to Inactive"
                     mnuCtxApplyToAll(1).Tag = False
                     mnuCtxApplyToAll(0).Visible = True
                     mnuCtxApplyToAll(1).Visible = True
                    
                Case NLC_HTB
                     mnuCtxApplyToAll(0).Caption = "Set selected to HTB"
                     mnuCtxApplyToAll(0).Tag = True
                     mnuCtxApplyToAll(1).Caption = "Set selected to not HTB"
                     mnuCtxApplyToAll(1).Tag = False
                     mnuCtxApplyToAll(0).Visible = True
                     mnuCtxApplyToAll(1).Visible = True
                     
                Case Else
                    vList = SplitComboList(nColIdx)
                    If IsArray(vList) Then
                        nBound = UBound(vList(0))
                        If nBound >= mnuCtxApplyToAll.Count Then nBound = mnuCtxApplyToAll.Count - 1
                        For i = 0 To nBound
                            mnuCtxApplyToAll(i).Caption = "Assign " & vList(1)(i) & " to selected"
                            mnuCtxApplyToAll(i).Tag = vList(0)(i)
                            mnuCtxApplyToAll(i).Visible = True
                        Next
                    End If
                    
            End Select
                    
            For i = 0 To .SelectedRows - 1
                nRow = .SelectedRow(i)
                If Not .RowHidden(nRow) Then
                    If nColIdx <> NLC_HTB Then
                        sRowValue = .TextMatrix(nRow, m_nMenuGridCol)
                    Else
                        sRowValue = IIf(.Cell(flexcpChecked, nRow, m_nMenuGridCol) = flexChecked, True, _
                                    IIf(.Cell(flexcpChecked, nRow, m_nMenuGridCol) = flexUnchecked, False, _
                                    .TextMatrix(nRow, m_nMenuGridCol) = " "))
                    End If
                    
                    If sValue = "" Then
                        sValue = sRowValue
                        
                    ElseIf sValue <> sRowValue Then
                        bValue = True
                        Exit For
                    End If
                End If
            Next
                
            For i = 0 To mnuCtxApplyToAll.Count
                Set aMenu = mnuCtxApplyToAll(i)
                If Not aMenu.Visible Then
                    Set aMenu = Nothing
                    Exit For
                End If
                aMenu.Enabled = (bValue Or aMenu.Tag <> sValue) And (nColIdx <> NLC_HTB Or nColIdx = NLC_HTB And Not bFuturesSelected)
                Set aMenu = Nothing
            Next
                
            mnuCtxSeparator3.Visible = mnuCtxApplyToAll(0).Visible
            
            Erase vList
        End If
    End With
End Sub

Private Sub mnuCtxApplyToAll_Click(Index As Integer)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    Dim i&, nRow&, nColIdx&
    Dim aUnd As clsUvUndAtom, bUpdated As Boolean, bOldDirty As Boolean
    Dim aMenu As Menu, vNewValue As Variant
    
    With fgUnd
        nColIdx = .ColKey(m_nMenuGridCol)
        If .SelectedRows <= 1 _
            Or nColIdx <> NLC_OPT_STYLE _
            And nColIdx <> NLC_CALENDAR _
            And nColIdx <> NLC_UND_PROFILE _
            And nColIdx <> NLC_OPT_PROFILE _
            And nColIdx <> NLC_ACTIVE _
            And nColIdx <> NLC_HTB Then Exit Sub
                
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
        
        m_bInProc = True
        AdjustState
            
        Screen.MousePointer = vbArrow
        DoEvents
        
        lblStatus.Visible = False
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = .SelectedRows
        pbProgress.Visible = True
        lblProcess.Caption = "Applying new value..."
        lblProcess.Refresh
        lblProcess.Visible = True
        
        Set aMenu = mnuCtxApplyToAll(Index)
        vNewValue = aMenu.Tag

        For i = 0 To .SelectedRows - 1
            nRow = .SelectedRow(i)
            If Not .RowHidden(nRow) Then
                Set aUnd = .RowData(nRow)
                
                If Not aUnd Is Nothing Then
                    bUpdated = False
                    
                    Select Case nColIdx
                        Case NLC_OPT_STYLE
                            If aUnd.IsAmerican <> CByte(vNewValue) Then
                                aUnd.IsAmerican = CByte(vNewValue)
                                bUpdated = True
                            End If
                            
                        Case NLC_CALENDAR
'                            If aUnd.ExpCalendarID <> CLng(vNewValue) Then
'                                aUnd.ExpCalendarID = CLng(vNewValue)
'                                bUpdated = True
'                            End If
                            
                        Case NLC_UND_PROFILE
                            If aUnd.UndPriceProfile.ID <> CLng(vNewValue) Then
                                Set aUnd.UndPriceProfile = g_PriceProfile(CLng(vNewValue))
                                bUpdated = True
                            End If
                            
                        Case NLC_OPT_PROFILE
                            If aUnd.OptPriceProfile.ID <> CLng(vNewValue) Then
                                Set aUnd.OptPriceProfile = g_PriceProfile(CLng(vNewValue))
                                bUpdated = True
                            End If
                            
                        Case NLC_ACTIVE
                            If aUnd.IsActive <> CBool(vNewValue) Then
                                aUnd.IsActive = CBool(vNewValue)
                                bUpdated = True
                            End If
                        
                        Case NLC_HTB
                            If aUnd.IsHTB <> CBool(vNewValue) Then
                                aUnd.IsHTB = CBool(vNewValue)
                                bUpdated = True
                            End If
                            
                    End Select
                    
                    If bUpdated Then
                        bOldDirty = aUnd.IsDirty
                        aUnd.AdjustDirty
                        
                        UnderlyingUpdate nRow, True, aUnd
                    
                        If bOldDirty <> aUnd.IsDirty Then
                            m_nDirtyUndCount = m_nDirtyUndCount + IIf(aUnd.IsDirty, 1, -1)
                            Debug.Assert m_nDirtyUndCount >= 0 And m_nDirtyUndCount <= m_Und.Count
                            AdjustState
                            AdjustCaption
                        End If
                    End If
                    
                    Set aUnd = Nothing
                End If
            End If
            
            DoEvents
            If m_bShutDown Then Exit Sub
            IncProgress pbProgress
        Next
        
        DoEvents
        If m_bShutDown Then Exit Sub
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        
        m_bInProc = False
        AdjustState
        Screen.MousePointer = vbDefault
        Set aMenu = Nothing
    
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
    
    m_bInProc = False
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
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
        Case GT_UNDERLYINGS_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
            
        Case GT_UNDERLYINGS_LIST
            gdGrid.CopyTo m_gdUnd
            
            m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
            FormatUndGrid
            FormatUndColumns
            UnderlyingsUpdate True
            m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
            
            AdjustState
    
    End Select
    
    UserControl_Resize
End Sub

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    Dim nIdx&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_UNDERLYINGS_LIST
            With fgUnd
                m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= NLC_SYMBOL And nIdx <= NLC_LAST_COLUMN Then
                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    Dim nIdx&, i&, nCount&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_UNDERLYINGS_LIST
            With fgUnd
                m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
                .AutoSize 0, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= NLC_SYMBOL And nIdx <= NLC_LAST_COLUMN Then
                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_UNDERLYINGS_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_UNDERLYINGS_LIST
            g_ClipMgr.CopyGridToClipboard fgUnd

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_UNDERLYINGS_FILTER
            CustomizeFltGridLayout

        Case GT_UNDERLYINGS_LIST
            CustomizeUndGridLayout
        
    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    Dim i&, nColIdx&, bMove As Boolean
    
    If m_enMenuGrid = GT_UNDERLYINGS_LIST Then
        bMove = False
        
        If m_gdUnd.Idx(1) = NLC_NONE Then Exit Sub
        
        nColIdx = fgUnd.ColKey(m_nMenuGridCol)
        For i = 0 To NLC_LAST_COLUMN
            If m_gdUnd.Idx(i) = nColIdx Then
                m_gdUnd.Idx(i) = NLC_NONE
                m_gdUnd.Col(nColIdx).Visible = False
                bMove = True
            End If
            
            If bMove Then
                If i + 1 <= NLC_LAST_COLUMN Then
                    m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
                Else
                    m_gdUnd.Idx(i) = NLC_NONE
                End If
            End If
        Next
        
        If bMove Then
            FormatUndGrid
            FormatUndColumns
            UnderlyingsUpdate True
            AdjustState
        End If
            
    End If
End Sub

Private Sub UnderlyingUpdateDirtyStatus(ByVal nRow As Long, Optional aRowUnd As clsUvUndAtom = Nothing)
    On Error Resume Next
    Dim nCol&, clrBackColor As OLE_COLOR
    Dim aUnd As clsUvUndAtom

    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
    
        Set aUnd = aRowUnd
        If aUnd Is Nothing Then Set aUnd = .RowData(nRow)
        If Not aUnd Is Nothing Then
            Dim i&, nIdx&
            i = 0
            nIdx = m_gdUnd.Idx(0)
            While nIdx >= 0 And i <= NLC_LAST_COLUMN

                nCol = i + 1
                clrBackColor = m_gdUnd.Col(nIdx).BackColor
                If aUnd.IsDirty Then clrBackColor = GetAlternateColor(clrBackColor)
                .Cell(flexcpBackColor, nRow, nCol) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))

                i = i + 1
                nIdx = m_gdUnd.Idx(i)
            Wend
            
            Set aUnd = Nothing
        End If
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsUpdateDirtyStatus()
    On Error Resume Next
    Dim i&, nRows&
    
    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
        
        nRows = .Rows - 1
        For i = 1 To nRows
            UnderlyingUpdateDirtyStatus i
        Next
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsUpdate(ByVal bUpdateDirtyStatus As Boolean)
    On Error Resume Next
    Dim i&, nRows&
    
    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
        
        nRows = .Rows - 1
        For i = 1 To nRows
            UnderlyingUpdate i, bUpdateDirtyStatus
        Next
        
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
End Sub

Private Function GetFirstVisibleSelectedRow&()
    On Error Resume Next
    Dim i&, nRow&
    GetFirstVisibleSelectedRow = 0
    For i = 0 To fgUnd.SelectedRows - 1
        nRow = fgUnd.SelectedRow(i)
        If Not fgUnd.RowHidden(nRow) Then
            GetFirstVisibleSelectedRow = nRow
            Exit For
        End If
    Next
End Function

Private Sub FillDataForOrderFromCurrentSelection(ByRef aUnd As EtsGeneralLib.UndAtom)
    On Error Resume Next
    Dim nRow&
    
    If m_enMenuGrid = GT_UNDERLYINGS_LIST Then
        With fgUnd
            nRow = GetFirstVisibleSelectedRow()
            If nRow > 0 Then
                .Row = nRow
                Set aUnd = g_Underlying(.RowData(nRow).UndID)
            End If
        End With
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection aUnd
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, True, 100, 0#
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, Nothing, True, 10, 0#
'    End If
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Underlyings View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxHTBRates_Click()
    On Error Resume Next
    Dim nUndID As Long: nUndID = BAD_LONG_VALUE
    Dim bIsAllowLending As Boolean: bIsAllowLending = True
    Dim nRow As Long

    nRow = GetFirstVisibleSelectedRow()
    If nRow > 0 Then
        fgUnd.Row = nRow
        nUndID = fgUnd.RowData(nRow).UndID
        bIsAllowLending = fgUnd.RowData(nRow).IsAllowLending
        
        If Not m_frmHTBRates Is Nothing And nUndID <> BAD_LONG_VALUE Then
            m_frmHTBRates.ShowHTBRates nUndID, m_frmOwner, bIsAllowLending
        End If
    End If
    
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
'    Dim aUnd As EtsGeneralLib.UndAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection aUnd
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, Nothing, False, 10&, 0#
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Underlyings View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aUnd As clsUvUndAtom
    Dim aGUnd As EtsGeneralLib.UndAtom
    Dim i&, nRow&, nVisibleSelectedRows&, nFirstVisibleSelectedRow&
    
    m_bInProc = True
    
    If m_enMenuGrid = GT_UNDERLYINGS_LIST Then
        With fgUnd
            nVisibleSelectedRows = 0
            nFirstVisibleSelectedRow = .Rows
            For i = 0 To .SelectedRows - 1
                nRow = .SelectedRow(i)
                If Not .RowHidden(nRow) Then
                    nVisibleSelectedRows = nVisibleSelectedRows + 1
                    If nRow < nFirstVisibleSelectedRow Then nFirstVisibleSelectedRow = nRow
                End If
            Next
            
            If nVisibleSelectedRows = 1 And nFirstVisibleSelectedRow < .Rows Then
                .Row = nFirstVisibleSelectedRow
                Set aUnd = .RowData(nFirstVisibleSelectedRow)
                If Not aUnd Is Nothing Then
                    Set aGUnd = g_Underlying(aUnd.UndID)
                    If Not aGUnd Is Nothing Then
                        Set aGUnd = Nothing
                        m_bInProc = False
                        On Error GoTo EH
                        frmTradeEditor.NewTrade aUnd.ID, aUnd.UndID, 1, m_frmOwner
                    End If
                    Set aUnd = Nothing
                End If
            End If
        End With
    End If

    m_bInProc = False

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    Set aUnd = Nothing
    ResetMenuData
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_UNDERLYINGS_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Underlyings Filter", "Underlyings Filter", fgFlt
'
'        Case GT_UNDERLYINGS_LIST
'            frmPrintPreview.Execute m_frmOwner, "Underlyings List", "Underlyings List", fgUnd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
    UnderlyingsShow True
End Sub

Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nRow&, aUnd As clsUvUndAtom
    
    With fgUnd
        Set aUnd = m_Und(CStr(IIf(aTrd.ContractType <> enCtFuture, aTrd.UndID, aTrd.FutID)))
        If Not aUnd Is Nothing Then
            If Not aUnd.HavePos Then
                aUnd.HavePos = True
                nRow = .FindRow(aUnd.ID, 1, NLC_KEY, , True)
            
                If nRow > 0 Then
                    m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
                    UnderlyingUpdate nRow, False, aUnd
                    m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
                End If
            End If
        End If
    End With
End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    Select Case enAction
        Case enTaTradeNew
            Debug.Assert Not aNewTrdInfo Is Nothing
            AddTrade aNewTrdInfo

    End Select
End Sub

Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Debug.Assert Not aTrdFrom Is Nothing
    Debug.Assert Not aTrdTo Is Nothing
    
    AddTrade aTrdFrom
    AddTrade aTrdTo
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Dim nTotalCols As Long
    Dim nGridColWidth As Long, i As Long, nGridHeight As Long
        
    With fgFlt
        .Top = 0
        .Left = 0
        nGridHeight = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
        
        nTotalCols = -1
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nTotalCols = nTotalCols + 1
        Next
            
        nGridColWidth = 0
        If nTotalCols = .RightCol And .LeftCol = 0 Then
            nGridColWidth = 0&
            For i = 0 To .Cols - 1
                If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth '+ ScaleY(.GridLineWidth, vbPixels, vbTwips)
            Next
            If nGridColWidth > 0 Then nGridColWidth = nGridColWidth - ScaleY(.GridLineWidth, vbPixels, vbTwips)
            
            If ScaleWidth < nGridColWidth Then
                nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
                .ScrollBars = flexScrollBarHorizontal
            Else
                .ScrollBars = flexScrollBarNone
            End If
        ElseIf nTotalCols <> .RightCol Or .LeftCol <> 0 Then
              nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
             .ScrollBars = flexScrollBarHorizontal
        Else
          .ScrollBars = flexScrollBarNone
        End If
        
        .Height = nGridHeight
    End With

    With fgUnd
        .Left = 0
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Width = ScaleWidth
        .Height = ScaleHeight - .Top
        .Refresh
    End With
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub

Public Sub Term()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    SaveDefaultSettings
    
    m_bShutDown = True
    m_Und.Clear
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set geUnd = Nothing
    
    Set m_frmHTBRates = Nothing
    Set frmLayout = Nothing
    Set TradeChannel = Nothing
    Set aParams = Nothing
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Private Sub AdjustState()
    On Error Resume Next
    RaiseEvent OnStateChange
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = "Underlyings"
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = "Underlyings"
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_UNDERLYINGS_FILTER, NFC_TRADER, NFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeUndGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_UNDERLYINGS_LIST, NLC_SYMBOL, NLC_LAST_COLUMN, m_gdUnd, m_frmOwner
    End If
End Sub

Private Sub SaveDefaultSettings()
    On Error GoTo EH
    If m_bShutDown Then Exit Sub
    
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, i&
    
    sPath = MakeCommonFilePath("EtsSettings", "UnderlyingsFilterData.Default", True)
    aStorage.Init sPath

    ' common info
    For i = NFC_TRADER To NFC_DIRTY
        If i <> NFC_SYMBOL Then _
            aStorage.SetLongValue "UndFlt", "Filter" & CStr(i), m_nFilter(i)
    Next
    
    aStorage.SetStringValue "UndFlt", "Filter" & CStr(NFC_SYMBOL), IIf(UCase$(m_sFilterSymbol) = "<ALL>", "", m_sFilterSymbol)
    
    aStorage.SetStringValue "Windows", "WinState", m_frmOwner.WindowState
    If m_frmOwner.WindowState = vbNormal Then
        aStorage.SetLongValue "Windows", "WinTop", m_frmOwner.Top
        aStorage.SetLongValue "Windows", "WinLeft", m_frmOwner.Left
        aStorage.SetLongValue "Windows", "WinWidth", m_frmOwner.Width
        aStorage.SetLongValue "Windows", "WinHeight", m_frmOwner.Height
    End If
    aStorage.SaveData
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to save default filters for underlyings view: " & Err.Description
End Sub

Private Sub LoadDefaultSettings()
    On Error Resume Next
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, i&
    
    sPath = MakeCommonFilePath("EtsSettings", "UnderlyingsFilterData.Default", False)
    aStorage.Init sPath
    aStorage.ReadData
    
    ' common info
    For i = NFC_TRADER To NFC_DIRTY
        If i <> NFC_SYMBOL Then _
            m_nFilter(i) = aStorage.GetLongValue("UndFlt", "Filter" & CStr(i), m_nFilter(i))
    Next
    
    m_sFilterSymbol = Trim$(UCase$(aStorage.GetStringValue("UndFlt", "Filter" & CStr(NFC_SYMBOL))))
    
    If m_nFilter(NFC_TYPE) < 0 Or m_nFilter(NFC_TYPE) > 4 Then m_nFilter(NFC_TYPE) = 0
    If m_nFilter(NFC_ACTIVE) < 0 Or m_nFilter(NFC_ACTIVE) > 2 Then m_nFilter(NFC_ACTIVE) = 0
    If m_nFilter(NFC_OPT_STYLE) < 0 Or m_nFilter(NFC_OPT_STYLE) > 2 Then m_nFilter(NFC_OPT_STYLE) = 0
    If m_nFilter(NFC_DIVS) < 0 Or m_nFilter(NFC_DIVS) > 4 Then m_nFilter(NFC_DIVS) = 0
    If m_nFilter(NFC_HAS_POS) < 0 Or m_nFilter(NFC_HAS_POS) > 2 Then m_nFilter(NFC_HAS_POS) = 0
    If m_nFilter(NFC_HAS_OPTIONS) < 0 Or m_nFilter(NFC_HAS_OPTIONS) > 2 Then m_nFilter(NFC_HAS_OPTIONS) = 0
    If m_nFilter(NFC_HAS_SYNTH) < 0 Or m_nFilter(NFC_HAS_SYNTH) > 2 Then m_nFilter(NFC_HAS_SYNTH) = 0       '  Uncomment if SU works correctly
    If m_nFilter(NFC_HTB) < 0 Or m_nFilter(NFC_HTB) > 2 Then m_nFilter(NFC_HTB) = 0
    If m_nFilter(NFC_HEDGE_SYMBOL) < 0 Or m_nFilter(NFC_HEDGE_SYMBOL) > 2 Then m_nFilter(NFC_HEDGE_SYMBOL) = 0
    If m_nFilter(NFC_DIRTY) < 0 Or m_nFilter(NFC_DIRTY) > 2 Then m_nFilter(NFC_DIRTY) = 0
    If m_nFilter(NFC_HAS_FUTURES) < 0 Or m_nFilter(NFC_HAS_FUTURES) > 2 Then m_nFilter(NFC_HAS_FUTURES) = 0
    
    Dim nTop&, nLeft&, nWidth&, nHeight&, nState&
    nState = aStorage.GetLongValue("Windows", "WinState", vbNormal)
    nTop = aStorage.GetLongValue("Windows", "WinTop")
    nLeft = aStorage.GetLongValue("Windows", "WinLeft")
    nWidth = aStorage.GetLongValue("Windows", "WinWidth")
    nHeight = aStorage.GetLongValue("Windows", "WinHeight")
    
    If nWidth > 0 And nHeight > 0 Then
        m_frmOwner.Left = nLeft
        m_frmOwner.Top = nTop
        m_frmOwner.Width = nWidth
        m_frmOwner.Height = nHeight
    Else
        m_frmOwner.Width = g_ptDefWindowSize.X
        m_frmOwner.Height = g_ptDefWindowSize.Y
    End If
End Sub

Public Sub Refresh()
    On Error Resume Next
    tmrShow.Enabled = True
End Sub

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aUnd As clsUvUndAtom
    
    CurUnderlyingID = 0
    With fgUnd
        If .Row > 0 Then
            If Not .RowHidden(.Row) Then
                Set aUnd = .RowData(.Row)
                If Not aUnd Is Nothing Then
                    CurUnderlyingID = aUnd.ID
                    Set aUnd = Nothing
                End If
            End If
        End If
    End With
End Function

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As clsUvUndAtom, nRow&
    
'    Dim aGUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
    
'    If m_nFilter(NFC_UNDERLYING) <> nNewUndID Then
'        Set aUnd = g_Underlying(nNewUndID)
'        If Not aUnd Is Nothing Then
'            If aUnd.IsTraderContract Then
'                With fgFlt
'                    m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
'
'                    sValue = "0"
'                    nValue = 0
'
'                    sComboList = g_Params.UnderlyingComboList
'                    If Len(sComboList) > 0 Then
'                        sComboList = "#0;<All>|" & sComboList
'                    Else
'                        sComboList = "#0;<All>"
'                    End If
'
'                    Set aUnd = g_Underlying(m_nFilter(NFC_UNDERLYING))
'                    If Not aUnd Is Nothing Then
'                        nValue = aUnd.ID
'                        sValue = Trim$(Str$(aUnd.ID))
'                        Set aUnd = Nothing
'                    End If
'                    .ColComboList(NFC_UNDERLYING) = sComboList
'                    .TextMatrix(1, NFC_UNDERLYING) = sValue
'                    m_nFilter(NFC_UNDERLYING) = nValue
'
'                    m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
'                End With
'            End If
'        End If
'    End If

    With fgUnd
        Set aUnd = m_Und(CStr(nNewUndID))
        If Not aUnd Is Nothing Then
            nRow = .FindRow(nNewUndID, 1, NLC_KEY, , True)
            
            If nRow > 0 Then
                m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
                If .RowHidden(nRow) And CheckUnderlyingFilter(aUnd) Then
                    .RowHidden(nRow) = False
                    m_nUndCount = m_nUndCount + 1
                    lblStatus.Caption = Format$(m_nUndCount, "#,##0") & " underlying(s)"
                End If
                m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
            End If
        End If
    End With
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom, sComboList$, sValue$, nValue&
    
    If m_nFilter(NFC_GROUP) <> nNewUndGroupID Then
        Set aUndGroup = g_UnderlyingGroup(nNewUndGroupID)
        If Not aUndGroup Is Nothing Then
            With fgFlt
                m_GridLock(GT_UNDERLYINGS_FILTER).LockRedraw
                
                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(NFC_GROUP) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(aUndGroup.ID))
                    End If
                Next
                .ColComboList(NFC_GROUP) = sComboList
                .TextMatrix(1, NFC_GROUP) = sValue
                m_nFilter(NFC_GROUP) = nValue
                
                m_GridLock(GT_UNDERLYINGS_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, "Underlyings", "Underlyings", fgFlt, fgUnd
'    Screen.MousePointer = vbDefault
End Sub

Public Property Get IsDirty() As Boolean
    On Error Resume Next
    IsDirty = (m_nDirtyUndCount > 0)
End Property

Private Function SaveUnderlying(ByRef aUnd As clsUvUndAtom) As Boolean
    On Error GoTo EH
    SaveUnderlying = False
    
    With aUnd
        Select Case .UndType
            Case enCtStock
                gDBW.usp_Stock_Save .ID, Null, Null, .ContractName, IIf(.IsHTB, 1, 0), .Dividend.DivFreq, .Dividend.DivAmt, IIf(.Dividend.DivDate > 0, .Dividend.DivDate, Null), _
                                CLng(.Dividend.DivType), .Dividend.DivAmtCust, .Dividend.DivFreqCust, IIf(.Dividend.DivDateCust > 0, .Dividend.DivDateCust, Null), IIf(.IsAmerican, 1, 0), Null, _
                                0, IIf(.IsActive, 1, 0), .Skew, .Kurt, .IsAllowLending, .IsHedgeSymbol, 0, .UndPriceProfile.ID, .OptPriceProfile.ID
                            
            Case enCtIndex
                gDBW.usp_Index_Save .ID, Null, .ContractName, .Yield, IIf(.IsHTB, 1, 0), IIf(.IsAmerican, 1, 0), Null, _
                                    0, IIf(.IsBasket, 1, 0), IIf(.IsActive, 1, 0), .Skew, .Kurt, _
                                    0, .SOQ, .IsAllowLending, .IsHedgeSymbol, .UndPriceProfile.ID, .OptPriceProfile.ID
                                
            Case enCtFuture
                gDBW.usp_MmFuture_Save .ID, .ContractName, 0, IIf(.IsAmerican, 1, 0), _
                                    IIf(.IsActive, 1, 0), .UndPriceProfile.ID, .OptPriceProfile.ID
            Case Else
                Debug.Assert False
                Exit Function
        End Select
        
        .SaveChanges
    End With
    
    SaveUnderlying = True
    Exit Function
EH:
    LogEvent EVENT_ERROR, "Fail to save underlying '" & aUnd.Symbol & "': " & Err.Description
End Function

Public Sub SaveUnderlyings()
    On Error Resume Next
    Dim collUpdUnd As New EtsGeneralLib.EtsMmFilterAtomColl
    Dim aUnd As clsUvUndAtom, nRow&, bFireEvent As Boolean
    
    m_bInProc = True
    AdjustState
        
    Screen.MousePointer = vbArrow
    DoEvents
    
    lblStatus.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Visible = True
    lblProcess.Caption = "Data saving..."
    lblProcess.Refresh
    lblProcess.Visible = True

    With fgUnd
        m_GridLock(GT_UNDERLYINGS_LIST).LockRedraw
        
        bFireEvent = False
        pbProgress.Max = .Rows - 1
        
        For nRow = 1 To .Rows - 1
            Set aUnd = .RowData(nRow)
            If Not aUnd Is Nothing Then
                If aUnd.IsDirty Then
                    If SaveUnderlying(aUnd) Then
                        
                        collUpdUnd.Add(CStr(aUnd.ID)).ID = aUnd.ID
                        
                        m_nDirtyUndCount = m_nDirtyUndCount - 1
                        Debug.Assert m_nDirtyUndCount >= 0 And m_nDirtyUndCount <= m_Und.Count
                        
                        UnderlyingUpdateDirtyStatus nRow, aUnd
                        bFireEvent = True
                    End If
                End If
                Set aUnd = Nothing
            End If
        
            DoEvents
            If m_bShutDown Then Exit Sub
            IncProgress pbProgress
        Next
    
        m_GridLock(GT_UNDERLYINGS_LIST).UnlockRedraw
    End With
    
    DoEvents
    If m_bShutDown Then Exit Sub
    
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    
    If bFireEvent Then
        m_bFireEvent = True
        g_Params.FireUnderlyingsParamsChange collUpdUnd
        m_bFireEvent = False
    End If
    
    m_bInProc = False
    AdjustState
    AdjustCaption
    Screen.MousePointer = vbDefault
End Sub

Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
    On Error Resume Next
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols Then
        
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
        
        ElseIf bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
            mnuCtxTradeNew_Click
            FixDblClickHandled
        End If
    End If
End Sub



Private Sub mnuCtxCustomDividend_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aUnd As clsUvUndAtom
    Dim aGUnd As EtsGeneralLib.UndAtom
    Dim i&, nRow&, nVisibleSelectedRows&, nFirstVisibleSelectedRow&
    Dim aCustDivs As EtsGeneralLib.EtsDivColl
    Dim aShareUnd As EtsGeneralLib.UndAtom
    Dim aTmpUnd As EtsGeneralLib.UndAtom
    Dim aTmpContract As EtsGeneralLib.EtsContractAtom
    
    If m_enMenuGrid = GT_UNDERLYINGS_LIST Then
        With fgUnd
        
            nVisibleSelectedRows = 0
            nFirstVisibleSelectedRow = .Rows
            For i = 0 To .SelectedRows - 1
                nRow = .SelectedRow(i)
                If Not .RowHidden(nRow) Then
                    nVisibleSelectedRows = nVisibleSelectedRows + 1
                    If nRow < nFirstVisibleSelectedRow Then nFirstVisibleSelectedRow = nRow
                End If
            Next
            
            If nVisibleSelectedRows = 1 And nFirstVisibleSelectedRow < .Rows Then
                .Row = nFirstVisibleSelectedRow
                Set aUnd = .RowData(nFirstVisibleSelectedRow)
                If Not aUnd Is Nothing Then
                    Set aGUnd = g_UnderlyingAll(aUnd.UndID)
                    If Not aGUnd Is Nothing Then
                        'm_bInProc = False
                        On Error GoTo EH
                        
                          
                        m_frmCustDivs.Init aGUnd.ID, aGUnd.Symbol, Now + 365 * 5, aGUnd.Dividend.DivType = enDivStockBasket, aGUnd.Dividend.DivType = enDivCustomStream
                        If aGUnd.IsBasketIndex And aGUnd.Dividend.DivType = enDivStockBasket Then
                            m_frmCustDivs.BasketDivs = g_BasketIndex(aGUnd.ID).BasketDivs
                        End If
                        m_frmCustDivs.Show vbModal, Me
                            
                        If m_frmCustDivs.GotData Then
                            Set aCustDivs = m_frmCustDivs.CustomDividend
                            Set aUnd.Dividend.CustomDivs = aCustDivs
                        
                                If Not aGUnd.Dividend Is Nothing Then
                                    Set aGUnd.Dividend.CustomDivs = aCustDivs
                                End If
                                
                                
                                Set aTmpUnd = g_Underlying(aGUnd.ID)
                                
                                If Not aTmpUnd Is Nothing Then
                                    If Not aTmpUnd.Dividend Is Nothing Then
                                        Set aTmpUnd.Dividend.CustomDivs = aCustDivs
                                    End If
                                End If
                                
                                Set aShareUnd = Nothing
                                Set aCustDivs = Nothing
                            End If
                        
                        If nRow > 0 Then UnderlyingUpdate nRow, False, aUnd
                        
                    End If
                    Set aUnd = Nothing
                    Set aGUnd = Nothing
                End If
            End If
        End With
    End If
    
    Exit Sub
    
EH:
    'm_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to update custom dividend."
    Set aUnd = Nothing
    'ResetMenuData
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgUnd, , , sHeader, FileSaveType)
    Screen.MousePointer = vbNormal
End Function
