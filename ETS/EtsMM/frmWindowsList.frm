VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmWindowsList 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Windows"
   ClientHeight    =   3135
   ClientLeft      =   2775
   ClientTop       =   3720
   ClientWidth     =   2010
   Icon            =   "frmWindowsList.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3135
   ScaleWidth      =   2010
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin MSComctlLib.ImageList imlWinType 
      Left            =   1200
      Top             =   2400
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   15
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":0442
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":09DC
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":0F76
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":1510
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":1AAA
            Key             =   ""
         EndProperty
         BeginProperty ListImage6 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":2044
            Key             =   ""
         EndProperty
         BeginProperty ListImage7 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":25DE
            Key             =   ""
         EndProperty
         BeginProperty ListImage8 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":2B78
            Key             =   ""
         EndProperty
         BeginProperty ListImage9 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":3112
            Key             =   ""
         EndProperty
         BeginProperty ListImage10 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":36AC
            Key             =   ""
         EndProperty
         BeginProperty ListImage11 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":3C46
            Key             =   ""
         EndProperty
         BeginProperty ListImage12 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":41E0
            Key             =   ""
         EndProperty
         BeginProperty ListImage13 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":477A
            Key             =   ""
         EndProperty
         BeginProperty ListImage14 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":4D14
            Key             =   ""
         EndProperty
         BeginProperty ListImage15 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":52AE
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.Toolbar tlbToolBar 
      Align           =   1  'Align Top
      Height          =   330
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   2010
      _ExtentX        =   3545
      _ExtentY        =   582
      ButtonWidth     =   609
      ButtonHeight    =   582
      AllowCustomize  =   0   'False
      Wrappable       =   0   'False
      Style           =   1
      ImageList       =   "imlImages"
      DisabledImageList=   "imlImagesDis"
      _Version        =   393216
      BeginProperty Buttons {66833FE8-8583-11D1-B16A-00C0F0283628} 
         NumButtons      =   7
         BeginProperty Button1 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Activate Window"
            ImageIndex      =   1
         EndProperty
         BeginProperty Button2 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button3 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Tile Horizontally"
            ImageIndex      =   2
         EndProperty
         BeginProperty Button4 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Tile Vertically"
            ImageIndex      =   3
         EndProperty
         BeginProperty Button5 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Close Window(s)"
            ImageIndex      =   4
         EndProperty
         BeginProperty Button6 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Style           =   3
         EndProperty
         BeginProperty Button7 {66833FEA-8583-11D1-B16A-00C0F0283628} 
            Object.ToolTipText     =   "Always On Top"
            ImageIndex      =   5
         EndProperty
      EndProperty
   End
   Begin MSComctlLib.ImageList imlImages 
      Left            =   60
      Top             =   2400
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   5
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":5848
            Key             =   "_1"
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":59A2
            Key             =   "_2"
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":5AFC
            Key             =   "_3"
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":5C56
            Key             =   "_4"
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":5DB0
            Key             =   "_5"
         EndProperty
      EndProperty
   End
   Begin VSFlex7Ctl.VSFlexGrid fgWin 
      Height          =   1455
      Left            =   0
      TabIndex        =   0
      Top             =   360
      Width           =   2355
      _cx             =   4154
      _cy             =   2566
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
      ShowComboButton =   -1  'True
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
   Begin MSComctlLib.ImageList imlImagesDis 
      Left            =   600
      Top             =   2400
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   5
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":5F0A
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":6064
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":61BE
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":6318
            Key             =   ""
         EndProperty
         BeginProperty ListImage5 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmWindowsList.frx":6472
            Key             =   ""
         EndProperty
      EndProperty
   End
End
Attribute VB_Name = "frmWindowsList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const BTN_ACTIVATE& = 1&
Private Const BTN_TILE_HORZ& = 3&
Private Const BTN_TILE_VERT& = 4&
Private Const BTN_CLOSE_WIN& = 5&
Private Const BTN_STAY_TOP& = 7&

Private Const IDX_GREEKS& = 1&
Private Const IDX_MATRIX& = 2&
Private Const IDX_QUOTATION& = 3&
Private Const IDX_QUOTES& = 4&
Private Const IDX_RISKS& = 5&
Private Const IDX_TRADES& = 6&
Private Const IDX_VOLA& = 7&
Private Const IDX_VOLAANALYSIS& = 8&
Private Const IDX_HEDGESUMMARY& = 9&
Private Const IDX_UNDERLYINGS& = 10&
Private Const IDX_REPORTS& = 11&
Private Const IDX_ORDERS& = 12&
Private Const IDX_TNTCARDS& = 13&
Private Const IDX_STOCKHEDGE& = 14&
Private Const IDX_INDEXHEDGE1& = 15&
Private Const IDX_INDEXHEDGE2& = 16&
Private Const IDX_ADDEXSCREEN& = 17&

Private m_nTop As Long
Private m_nLeft As Long
Private m_nWidth As Long
Private m_nHeight As Long
Private m_bStayOnTop As Boolean
Private m_GridLock As New clsGridLock

Private m_bIsLoaded As Boolean
Private m_bInProc As Boolean

Public Sub Execute(frmOwner As Form)
    On Error GoTo EH

    If Not m_bIsLoaded Then
        m_bInProc = False
        
        Load Me
        m_bIsLoaded = True
        
        ReloadWindowsList
        
        Me.Show vbModeless ', frmOwner
    Else
        Me.Show vbModeless ', frmOwner
        Me.SetFocus
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to load Windows List form."
    On Error Resume Next
    Unload Me
End Sub

Private Sub fgWin_DblClick()
    On Error Resume Next
    WindowActivate
End Sub

Private Sub fgWin_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If Shift = 0 Then
        Select Case KeyCode
            Case vbKeyReturn
                WindowActivate
                
            Case vbKeyF12
                ShowWindow frmMain.hWnd, SW_SHOWNORMAL
                frmMain.SetFocus
                
        End Select
    End If
End Sub

Private Sub fgWin_SelChange()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    LoadSettings
    Me.Left = m_nLeft
    Me.Top = m_nTop
    Me.Width = m_nWidth
    Me.Height = m_nHeight
    tlbToolBar.Buttons(BTN_STAY_TOP).Value = IIf(m_bStayOnTop, tbrPressed, tbrUnpressed)

    m_GridLock.Init fgWin
    FormatGrid
    Me.Caption = AppTitle() & " Windows"
    SetWindowPos Me.hWnd, IIf(m_bStayOnTop, HWND_TOPMOST, HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE Or SWP_NOACTIVATE
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    m_nTop = Me.Top
    m_nLeft = Me.Left
    m_nWidth = Me.Width
    m_nHeight = Me.Height

    Cancel = (UnloadMode = vbFormControlMenu)
    Me.Hide
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    fgWin.Left = 0
    fgWin.Top = tlbToolBar.Height
    fgWin.Width = Me.ScaleWidth
    fgWin.Height = Me.ScaleHeight - fgWin.Top
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    m_bIsLoaded = False
    SaveSettings
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Form_Terminate
End Sub

Public Sub ReloadWindowsList()
    On Error Resume Next
    Dim nRow&, nNodeRow&, aFrm As clsFormAtom
    Dim nFirstRow&, nLastRow&, sTmp$
    
    If Not m_bIsLoaded Or m_bInProc Then Exit Sub
    m_bInProc = True
    
    With fgWin
        m_GridLock.LockRedraw
        
        ' remove old windows
        For nRow = .Rows - 1 To 0 Step -1
            sTmp = ""
            sTmp = .RowData(nRow)
            If sTmp <> "" Then
                If g_ViewFrm(sTmp) Is Nothing Then
                    .RemoveItem nRow
                End If
            End If
        Next
        
        ' add new windows
        For Each aFrm In g_ViewFrm
            nRow = .FindRow(aFrm.Frm.WindowKey, , , , True)
            If nRow < 0 Then
                nNodeRow = -1
                
                If TypeOf aFrm.Frm Is frmQuotesViewSingle Then
                    nNodeRow = .FindRow(CStr(IDX_QUOTES), , 0, True)
                    
                ElseIf TypeOf aFrm.Frm Is frmRiskView Then
                    nNodeRow = .FindRow(CStr(IDX_RISKS), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmRiskMatrix Then
                    nNodeRow = .FindRow(CStr(IDX_MATRIX), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmTradeView Then
                    nNodeRow = .FindRow(CStr(IDX_TRADES), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmVolaMonitor Then
                    nNodeRow = .FindRow(CStr(IDX_VOLA), , 0, True)
            
'                ElseIf TypeOf aFrm.Frm Is frmQuotationView Then
'                    nNodeRow = .FindRow(CStr(IDX_QUOTATION), , 0, True)
             
                ElseIf TypeOf aFrm.Frm Is frmVolaAnalysis Then
                    nNodeRow = .FindRow(CStr(IDX_VOLAANALYSIS), , 0, True)
            
                ElseIf TypeOf aFrm.Frm Is frmGreeksView Then
                    nNodeRow = .FindRow(CStr(IDX_GREEKS), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmHedgeSummaryView Then
                    nNodeRow = .FindRow(CStr(IDX_HEDGESUMMARY), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmUnderlyingsView Then
                    nNodeRow = .FindRow(CStr(IDX_UNDERLYINGS), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmReportsView Then
                    nNodeRow = .FindRow(CStr(IDX_REPORTS), , 0, True)
                
                ElseIf TypeOf aFrm.Frm Is frmOrderView Then
                    nNodeRow = .FindRow(CStr(IDX_ORDERS), , 0, True)

'                ElseIf TypeOf aFrm.Frm Is frmTntCardView Then
'                    nNodeRow = .FindRow(CStr(IDX_TNTCARDS), , 0, True)
'
'                ElseIf TypeOf aFrm.Frm Is frmStockHedgeView Then
'                    nNodeRow = .FindRow(CStr(IDX_STOCKHEDGE), , 0, True)
'
'                ElseIf TypeOf aFrm.Frm Is frmIndexHedge1View Then
'                    nNodeRow = .FindRow(CStr(IDX_INDEXHEDGE1), , 0, True)
'
'                ElseIf TypeOf aFrm.Frm Is frmIndexHedge2View Then
'                    nNodeRow = .FindRow(CStr(IDX_INDEXHEDGE2), , 0, True)
                 
                 ElseIf TypeOf aFrm.Frm Is frmAddExScreen Then
                    nNodeRow = .FindRow(CStr(IDX_ADDEXSCREEN), , 0, True)

                End If
                
                If nNodeRow >= 0 Then
                    nRow = nNodeRow + 1
                    
                    sTmp = aFrm.Frm.GetShortCaption
                    Do While nRow < .Rows
                        If .RowData(nRow) = "" Then Exit Do
                        nRow = nRow + 1
                    Loop
                    
                    .AddItem "" & vbTab & "   " & sTmp, nRow
                    .RowData(nRow) = aFrm.Frm.WindowKey
                Else
                    Debug.Assert False
                End If
            Else
                .TextMatrix(nRow, 1) = "   " & aFrm.Frm.GetShortCaption
            End If
        Next
        
        ' hide empty groups
        nFirstRow = 0
        nLastRow = 0
        While nLastRow > -1
            nLastRow = .FindRow("", nFirstRow + 1, , , True)

            .RowHidden(nFirstRow) = (nLastRow < 0 And nFirstRow = .Rows - 1 Or nLastRow - nFirstRow = 1)

            nFirstRow = nLastRow
        Wend
        
        m_GridLock.UnlockRedraw
    End With
    
    UpdateControls
    m_bInProc = False
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    Dim nWinCount&, i&, nRow&, sKey$
    
    With fgWin
        nWinCount = 0&
        For i = 0 To .SelectedRows - 1
            nRow = .SelectedRow(i)
            sKey = ""
            sKey = .RowData(nRow)
            If Len(sKey) > 0 Then
                nWinCount = nWinCount + 1
                If nWinCount > 1 Then Exit For
            End If
        Next
        
        tlbToolBar.Buttons(BTN_ACTIVATE).Enabled = (.SelectedRows = 1 And nWinCount = 1)
        tlbToolBar.Buttons(BTN_TILE_HORZ).Enabled = (nWinCount > 1)
        tlbToolBar.Buttons(BTN_TILE_VERT).Enabled = (nWinCount > 1)
        tlbToolBar.Buttons(BTN_CLOSE_WIN).Enabled = (nWinCount > 0)
    End With
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    Dim nTmp&

    m_nTop = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "WindowListTop", Me.Top)
    m_nLeft = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "WindowListLeft", Me.Left)
    m_nWidth = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "WindowListWidth", Me.Width)
    m_nHeight = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "WindowListHeight", Me.Height)
    m_bStayOnTop = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "WindowListStayOnTop", 1) <> 0)
    
    If m_nLeft < 0 Then m_nLeft = 0
    nTmp = GetSystemMetrics(SM_CXFULLSCREEN) * 15
    If m_nLeft > nTmp Then m_nLeft = nTmp - Me.Width
    If m_nWidth <= 0 Or m_nWidth > nTmp Then m_nWidth = Me.Width
    
    If m_nTop < 0 Then m_nTop = 0
    nTmp = GetSystemMetrics(SM_CYFULLSCREEN) * 15
    If m_nTop > nTmp Then m_nTop = nTmp - Me.Height
    If m_nHeight <= 0 Or m_nHeight > nTmp Then m_nHeight = Me.Height
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "WindowListTop", m_nTop
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "WindowListLeft", m_nLeft
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "WindowListWidth", m_nWidth
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "WindowListHeight", m_nHeight
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "WindowListStayOnTop", IIf(m_bStayOnTop, 1, 0)
End Sub

Private Sub tlbToolBar_ButtonClick(ByVal Button As MSComctlLib.Button)
    On Error Resume Next
    Select Case Button.Index
        Case BTN_ACTIVATE
            WindowActivate
            
        Case BTN_TILE_HORZ
            WindowsTileHoriz
            
        Case BTN_TILE_VERT
            WindowsTileVert
            
        Case BTN_CLOSE_WIN
            WindowsClose
            
        Case BTN_STAY_TOP
            m_bStayOnTop = Not m_bStayOnTop
            Button.Value = IIf(m_bStayOnTop, tbrPressed, tbrUnpressed)
            SetWindowPos Me.hWnd, IIf(m_bStayOnTop, HWND_TOPMOST, HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE Or SWP_NOACTIVATE
    End Select
End Sub

Private Sub WindowActivate()
    On Error Resume Next
    Dim i&, nRow&, aFrm As clsFormAtom, sKey$
    
    With fgWin
        If .SelectedRows = 1 Then
            nRow = .SelectedRow(0)
            sKey = ""
            sKey = .RowData(nRow)
            If Len(sKey) > 0 Then
                Set aFrm = g_ViewFrm(sKey)
                If Not aFrm Is Nothing Then
                    If IsIconic(aFrm.Frm.hWnd) = 0 Then
                        ShowWindow aFrm.Frm.hWnd, SW_SHOW
                    Else
                        ShowWindow aFrm.Frm.hWnd, SW_RESTORE
                    End If
                    aFrm.Frm.SetFocus
                    Set aFrm = Nothing
                End If
            End If
        End If
    End With
End Sub

Private Sub WindowsClose()
    On Error Resume Next
    Dim i&, nRow&, aFrm As clsFormAtom, sKey$, nFirstRow&, nLastRow&
    
    m_bInProc = True
    
    With fgWin
        m_GridLock.LockRedraw
        
        For i = .SelectedRows - 1 To 0 Step -1
            nRow = .SelectedRow(i)
            sKey = ""
            sKey = .RowData(nRow)
            If Len(sKey) > 0 Then
                Set aFrm = g_ViewFrm(sKey)
                If Not aFrm Is Nothing Then
                    Unload aFrm.Frm
                    .RemoveItem nRow
                End If
            End If
        Next
    
        nFirstRow = 0
        nLastRow = 0
        While nLastRow > -1
            nLastRow = .FindRow("", nFirstRow + 1, , , True)

            .RowHidden(nFirstRow) = (nLastRow < 0 And nFirstRow = .Rows - 1 Or nLastRow - nFirstRow = 1)

            nFirstRow = nLastRow
        Wend
        
        m_GridLock.UnlockRedraw
    End With
    
    m_bInProc = False
End Sub

Private Sub WindowsTileHoriz()
    On Error Resume Next
    Dim i&, nRow&, aForms As New clsFormColl, aFrm As clsFormAtom, sKey$
    
    With fgWin
        For i = 0 To .SelectedRows - 1
            nRow = .SelectedRow(i)
            sKey = ""
            sKey = .RowData(nRow)
            If Len(sKey) > 0 Then
                Set aFrm = g_ViewFrm(sKey)
                If Not aFrm Is Nothing Then
                    aForms.Add aFrm
                    Set aFrm = Nothing
                End If
            End If
        Next
        
        If aForms.Count > 0 Then
            TileWindowsHorizontally aForms
            aForms.Clear
        End If
    End With
End Sub

Private Sub WindowsTileVert()
    On Error Resume Next
    Dim i&, nRow&, aForms As New clsFormColl, aFrm As clsFormAtom, sKey$
    
    With fgWin
        For i = 0 To .SelectedRows - 1
            nRow = .SelectedRow(i)
            sKey = ""
            sKey = .RowData(nRow)
            If Len(sKey) > 0 Then
                Set aFrm = g_ViewFrm(sKey)
                If Not aFrm Is Nothing Then
                    aForms.Add aFrm
                    Set aFrm = Nothing
                End If
            End If
        Next
        
        If aForms.Count > 0 Then
            TileWindowsVertically aForms
            aForms.Clear
        End If
    End With
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    Dim nRow&
    
    With fgWin
        m_GridLock.LockRedraw
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight ' flexFlat
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchFromTop
        .AutoSizeMouse = False
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 0
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .HighLight = flexHighlightAlways
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarVertical
        .ScrollTrack = True
        .SelectionMode = flexSelectionListBox
        .TabBehavior = flexTabControls
    
        .Rows = 0
        .Cols = 2
        .ColHidden(0) = True
        .ColDataType(1) = flexDTString
        
        .AddItem CStr(IDX_TRADES) & vbTab & "Trades"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_TRADES).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_RISKS) & vbTab & "Risks"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_RISKS).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_QUOTES) & vbTab & "Quotes"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_QUOTES).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_MATRIX) & vbTab & "Risk Matrix"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_MATRIX).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_VOLA) & vbTab & "Vola Monitor"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_VOLA).Picture
        .RowData(nRow) = ""
        
'        If g_bIseEnable Then
'            .AddItem CStr(IDX_QUOTATION) & vbTab & "ISE Market Maker"
'            nRow = .Rows - 1
'            'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_QUOTATION).Picture
'            .RowData(nRow) = ""
'        End If
        
        .AddItem CStr(IDX_VOLAANALYSIS) & vbTab & "Vola Analysis"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_VOLAANALYSIS).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_GREEKS) & vbTab & "Greeks"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_GREEKS).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_HEDGESUMMARY) & vbTab & "Hedge Summary"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_HEDGESUMMARY).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_UNDERLYINGS) & vbTab & "Underlyings"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_UNDERLYINGS).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_REPORTS) & vbTab & "Reports"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_REPORTS).Picture
        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_ORDERS) & vbTab & "Orders"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_ORDERS).Picture
        .RowData(nRow) = ""
'
'        .AddItem CStr(IDX_TNTCARDS) & vbTab & "Trades Messages"
'        nRow = .Rows - 1
'        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_TNTCARDS).Picture
'        .RowData(nRow) = ""
'
'        .AddItem CStr(IDX_STOCKHEDGE) & vbTab & "Stock Hedge"
'        nRow = .Rows - 1
'        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_STOCKHEDGE).Picture
'        .RowData(nRow) = ""
'
'        .AddItem CStr(IDX_INDEXHEDGE1) & vbTab & "Index Hedge"
'        nRow = .Rows - 1
'        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_INDEXHEDGE1).Picture
'        .RowData(nRow) = ""
'
'        .AddItem CStr(IDX_INDEXHEDGE2) & vbTab & "Two Indices Hedge"
'        nRow = .Rows - 1
'        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_INDEXHEDGE1).Picture
'        .RowData(nRow) = ""
        
        .AddItem CStr(IDX_ADDEXSCREEN) & vbTab & "Exercise Screen"
        nRow = .Rows - 1
        'Set .Cell(flexcpPicture, nRow, 1) = imlWinType.ListImages(IDX_ADDEXSCREEN).Picture
        .RowData(nRow) = ""
        

        .Cell(flexcpPictureAlignment, 0, 1, .Rows - 1, 1) = flexPicAlignLeftCenter
        '.Cell(flexcpAlignment, 0, 1, .Rows - 1, 1) = flexAlignCenterCenter
        .Cell(flexcpFontBold, 0, 1, .Rows - 1, 1) = True
        .Cell(flexcpBackColor, 0, 1, .Rows - 1, 1) = &HC0FFC0   '&H80000014 '.BackColorFixed
        
        m_GridLock.UnlockRedraw
    End With
End Sub
