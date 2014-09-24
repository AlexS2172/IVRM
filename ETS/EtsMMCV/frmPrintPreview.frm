VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{54850C51-14EA-4470-A5E4-8C5DB32DC853}#1.0#0"; "VSPrint8.ocx"
Object = "{1BCC7098-34C1-4749-B1A3-6C109878B38F}#1.0#0"; "VSPDF8.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmPrintPreview 
   Caption         =   "Print Preview"
   ClientHeight    =   7245
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   11970
   Icon            =   "frmPrintPreview.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   ScaleHeight     =   7245
   ScaleWidth      =   11970
   StartUpPosition =   3  'Windows Default
   Begin VSFlex7Ctl.VSFlexGrid fg 
      Height          =   315
      Left            =   10020
      TabIndex        =   15
      Top             =   3060
      Visible         =   0   'False
      Width           =   255
      _cx             =   450
      _cy             =   556
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
   Begin VSPrinter8LibCtl.VSPrinter vsPrinter 
      Height          =   5835
      Left            =   0
      TabIndex        =   2
      Top             =   600
      Width           =   8835
      _cx             =   15584
      _cy             =   10292
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      MousePointer    =   0
      BackColor       =   -2147483643
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty HdrFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   14.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      AutoRTF         =   -1  'True
      Preview         =   -1  'True
      DefaultDevice   =   0   'False
      PhysicalPage    =   -1  'True
      AbortWindow     =   -1  'True
      AbortWindowPos  =   0
      AbortCaption    =   "Printing..."
      AbortTextButton =   "Cancel"
      AbortTextDevice =   "on the %s on %s"
      AbortTextPage   =   "Now printing Page %d of"
      FileName        =   ""
      MarginLeft      =   1440
      MarginTop       =   1440
      MarginRight     =   1440
      MarginBottom    =   1440
      MarginHeader    =   0
      MarginFooter    =   0
      IndentLeft      =   0
      IndentRight     =   0
      IndentFirst     =   0
      IndentTab       =   720
      SpaceBefore     =   0
      SpaceAfter      =   0
      LineSpacing     =   100
      Columns         =   1
      ColumnSpacing   =   180
      ShowGuides      =   2
      LargeChangeHorz =   300
      LargeChangeVert =   300
      SmallChangeHorz =   30
      SmallChangeVert =   30
      Track           =   0   'False
      ProportionalBars=   -1  'True
      Zoom            =   29.8573975044563
      ZoomMode        =   3
      ZoomMax         =   400
      ZoomMin         =   10
      ZoomStep        =   25
      EmptyColor      =   -2147483636
      TextColor       =   0
      HdrColor        =   0
      BrushColor      =   0
      BrushStyle      =   0
      PenColor        =   0
      PenStyle        =   0
      PenWidth        =   0
      PageBorder      =   0
      Header          =   ""
      Footer          =   ""
      TableSep        =   "|;"
      TableBorder     =   7
      TablePen        =   0
      TablePenLR      =   0
      TablePenTB      =   0
      NavBar          =   3
      NavBarColor     =   -2147483633
      ExportFormat    =   0
      URL             =   ""
      Navigation      =   3
      NavBarMenuText  =   "Whole &Page|Page &Width|&Two Pages|Thumb&nail"
   End
   Begin VB.PictureBox picTop 
      Align           =   1  'Align Top
      BorderStyle     =   0  'None
      Height          =   435
      Left            =   0
      ScaleHeight     =   435
      ScaleWidth      =   11970
      TabIndex        =   0
      Top             =   0
      Width           =   11970
      Begin VB.CommandButton btnSave 
         Caption         =   "Save As..."
         Height          =   300
         Left            =   10260
         TabIndex        =   14
         Top             =   60
         Width           =   915
      End
      Begin VB.ComboBox cbxView 
         Height          =   315
         Left            =   6120
         Style           =   2  'Dropdown List
         TabIndex        =   11
         Top             =   60
         Width           =   1575
      End
      Begin VB.Timer tmrPrintGrid 
         Enabled         =   0   'False
         Interval        =   100
         Left            =   1800
         Top             =   60
      End
      Begin VB.CommandButton btnPrint 
         Caption         =   "Print..."
         Height          =   300
         Left            =   9240
         TabIndex        =   13
         Top             =   60
         Width           =   915
      End
      Begin VB.CommandButton btnPageSetup 
         Caption         =   "Page Setup..."
         Height          =   300
         Left            =   7860
         TabIndex        =   12
         Top             =   60
         Width           =   1275
      End
      Begin VB.ComboBox cbxZoom 
         Height          =   315
         Left            =   3900
         Style           =   2  'Dropdown List
         TabIndex        =   9
         Top             =   60
         Width           =   1575
      End
      Begin VB.CommandButton btnNav 
         Height          =   300
         Index           =   4
         Left            =   2760
         Picture         =   "frmPrintPreview.frx":0E42
         Style           =   1  'Graphical
         TabIndex        =   7
         ToolTipText     =   "Last Page"
         Top             =   60
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         Height          =   300
         Index           =   3
         Left            =   2340
         Picture         =   "frmPrintPreview.frx":0E8D
         Style           =   1  'Graphical
         TabIndex        =   6
         ToolTipText     =   "Next Page"
         Top             =   60
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         Height          =   300
         Index           =   2
         Left            =   480
         Picture         =   "frmPrintPreview.frx":0ED4
         Style           =   1  'Graphical
         TabIndex        =   4
         ToolTipText     =   "Previous Page"
         Top             =   60
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         Height          =   300
         Index           =   1
         Left            =   60
         Picture         =   "frmPrintPreview.frx":0F1C
         Style           =   1  'Graphical
         TabIndex        =   3
         ToolTipText     =   "First Page"
         Top             =   60
         Width           =   375
      End
      Begin VB.Label lblView 
         Alignment       =   1  'Right Justify
         Caption         =   "View :"
         Height          =   315
         Left            =   5520
         TabIndex        =   10
         Top             =   90
         Width           =   555
      End
      Begin VB.Label lblZoom 
         Alignment       =   1  'Right Justify
         Caption         =   "Zoom :"
         Height          =   315
         Left            =   3180
         TabIndex        =   8
         Top             =   90
         Width           =   675
      End
      Begin VB.Label lblPage 
         Alignment       =   2  'Center
         Caption         =   "Page 1 of 1"
         Height          =   315
         Left            =   900
         TabIndex        =   5
         Top             =   90
         Width           =   1395
      End
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   1
      Top             =   6945
      Width           =   11970
      _ExtentX        =   21114
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   10320
      Top             =   1620
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VSPDF8LibCtl.VSPDF8 vsPdfCtrl 
      Left            =   9780
      Top             =   1680
   End
End
Attribute VB_Name = "frmPrintPreview"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Private m_frmOwner As Form
'Private m_bIsOpened As Boolean
'Private m_bInitNow As Boolean
'Private m_bInProc As Boolean
'Private m_fg() As VSFlexGrid
'Private m_nLastGrid As Long
'Private m_sTitle As String
'Private m_nRenderType As Long
'
'Public Sub Execute(ByRef frmOwner As Form, ByVal sTitle As String, ByVal sFormTitle As String, ParamArray arrGrids() As Variant)
'    On Error GoTo EH
'    If m_bIsOpened Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    Set m_frmOwner = frmOwner
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
'
'    m_bIsOpened = True
'    m_bInitNow = True
'    m_nRenderType = 0&
'    m_sTitle = sTitle
'    m_nLastGrid = -1
'
'    Erase m_fg
'    Dim i&
'
'    For i = LBound(arrGrids) To UBound(arrGrids)
'        If Not arrGrids(i) Is Nothing Then
'            If TypeOf arrGrids(i) Is VSFlexGrid Then
'                If m_nLastGrid >= 0 Then ReDim Preserve m_fg(0 To m_nLastGrid + 1) Else ReDim m_fg(0 To m_nLastGrid + 1)
'                m_nLastGrid = m_nLastGrid + 1
'
'                Set m_fg(m_nLastGrid) = arrGrids(i)
'            End If
'        End If
'    Next
'
'    Load Me
'
'    If Len(sFormTitle) > 0 Then
'        Me.Caption = sFormTitle & " - Print Preview"
'    Else
'        Me.Caption = "Print Preview"
'    End If
'
'    LoadSettings
'    InitCombos
'
'    btnPageSetup.Enabled = (Printers.Count > 0)
'    btnPrint.Enabled = (Printers.Count > 0)
'
'    Me.Show vbModeless, m_frmOwner
'
'    m_bInitNow = False
'    m_bInProc = False
'    tmrPrintGrid.Enabled = True
'
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to open print preview dialog."
'    On Error Resume Next
'    Erase m_fg
'    m_bInitNow = False
'    m_bInProc = False
'    Unload Me
'End Sub
'
'Private Sub btnSave_Click()
'    On Error GoTo EH
'    If m_bInProc Then Exit Sub
'
'    With dlgCommon
'        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
'        .CancelError = True
'        .DefaultExt = ".pdf"
'        .DialogTitle = "Save " & m_sTitle
'        .InitDir = ""
'        .FileName = m_sTitle & ".pdf"
'        .Filter = "Adobe PDF Files (*.pdf)|*.pdf|All Files|*.*"
'        .FilterIndex = 1
'        On Error Resume Next
'        .ShowSave
'        If Err.Number = 0 Then
'            On Error GoTo EH
'            m_bInProc = True
'            Screen.MousePointer = vbHourglass
'            vsPdfCtrl.ConvertDocument vsPrinter, .FileName
'            Screen.MousePointer = vbDefault
'            m_bInProc = False
'        End If
'    End With
'    Exit Sub
'EH:
'    m_bInProc = False
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to save " & m_sTitle & "."
'End Sub
'
'Private Sub cbxView_Click()
'    On Error Resume Next
'    If m_bInitNow Or m_bInProc Then Exit Sub
'    If m_nRenderType <> cbxView.ListIndex Then
'        m_nRenderType = cbxView.ListIndex
'        tmrPrintGrid.Enabled = True
'    End If
'End Sub
'
'Private Sub cbxZoom_Click()
'    On Error Resume Next
'    If Not m_bInitNow And cbxZoom.ListIndex >= 0 Then
'        If cbxZoom.ItemData(cbxZoom.ListIndex) <> zmPercentage Then
'            vsPrinter.ZoomMode = cbxZoom.ItemData(cbxZoom.ListIndex)
'        Else
'            vsPrinter.Zoom = Val(cbxZoom.List(cbxZoom.ListIndex))
'        End If
'    End If
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'
'    With vsPrinter
'        .Clear
'        .PhysicalPage = True
'        .NavBar = vpnbNone
'        .Track = True
'    End With
'
'End Sub
'
'Private Sub Form_Resize()
'    On Error Resume Next
'    vsPrinter.Move 0, picTop.Height, Me.ScaleWidth, Me.ScaleHeight - sbStatus.Height - picTop.Height
'End Sub
'
'Private Sub Form_Terminate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'
'    SaveSettings
'
'    If Not m_frmOwner Is Nothing Then
'        m_frmOwner.Enabled = True
'        m_frmOwner.SetFocus
'    End If
'
'    Erase m_fg
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    Form_Terminate
'End Sub
'
'Private Sub btnPageSetup_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    If m_bInProc Then Exit Sub
'
'    m_bInProc = True
'    UpdateStatus
'
'    If vsPrinter.NDevices > 0 Then
'        If vsPrinter.PrintDialog(pdPageSetup) Then
'            m_bInProc = False
'            UpdateStatus
'            tmrPrintGrid.Enabled = True
'        End If
'    Else
'        gCmn.CantAccessPrinterErrorMsgBox
'    End If
'
'    m_bInProc = False
'    UpdateStatus
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub btnPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    If m_bInProc Then Exit Sub
'
'    m_bInProc = True
'    UpdateStatus
'
'    vsPrinter.PrintDoc True
'
'    m_bInProc = False
'    UpdateStatus
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub InitCombos()
'    On Error Resume Next
'    Dim i&
'
'    With cbxView
'        .Clear
'        .AddItem "Normal", 0
'        .AddItem "As Is", 1
'        .ListIndex = IIf(m_nRenderType <> 0, 1, 0)
'    End With
'
'    With cbxZoom
'        .Clear
'
'        .AddItem "Thumbnail", 0
'        .ItemData(0) = zmThumbnail
'
'        .AddItem "Two Pages", 1
'        .ItemData(1) = zmTwoPages
'
'        .AddItem "Whole Page", 2
'        .ItemData(2) = zmWholePage
'
'        .AddItem "Page Width", 3
'        .ItemData(3) = zmPageWidth
'
'        .AddItem "Stretch", 4
'        .ItemData(4) = zmStretch
'
'        .AddItem "25%", 5
'        .ItemData(5) = zmPercentage
'
'        .AddItem "50%", 6
'        .ItemData(6) = zmPercentage
'
'        .AddItem "75%", 7
'        .ItemData(7) = zmPercentage
'
'        .AddItem "100%", 8
'        .ItemData(8) = zmPercentage
'
'        .AddItem "150%", 9
'        .ItemData(9) = zmPercentage
'
'        .AddItem "200%", 10
'        .ItemData(10) = zmPercentage
'
'        .AddItem "400%", 11
'        .ItemData(11) = zmPercentage
'
'        For i = 0 To .ListCount - 1
'            If .ItemData(i) = vsPrinter.ZoomMode Then
'                If vsPrinter.ZoomMode <> zmPercentage Then
'                    .ListIndex = i
'                    Exit For
'                ElseIf Val(.List(i)) = Round(vsPrinter.Zoom) Then
'                    .ListIndex = i
'                    Exit For
'                End If
'            End If
'        Next
'        If .ListIndex < 0 Then .ListIndex = 2
'    End With
'End Sub
'
'Private Sub LoadSettings()
'    On Error Resume Next
'    With vsPrinter
'        .ZoomMode = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintZoomMode", .ZoomMode)
'        If .ZoomMode = zmPercentage Then
'            .Zoom = Val(GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintZoom", Trim$(Str$(.Zoom))))
'        End If
'
'        .PaperSize = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintPaperSize", .PaperSize)
'        .Orientation = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintOrientation", .Orientation)
'        .MarginLeft = Val(GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginLeft", Trim$(Str$(.MarginLeft))))
'        .MarginRight = Val(GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginRight", Trim$(Str$(.MarginRight))))
'        .MarginTop = Val(GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginTop", Trim$(Str$(.MarginTop))))
'        .MarginBottom = Val(GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginBottom", Trim$(Str$(.MarginBottom))))
'
'
'        Dim nWidth&, nHeight&
'        nWidth = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintWinWidth")
'        nHeight = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintWinHeight")
'
'        If nWidth > 0 And nHeight > 0 Then
'            Me.Left = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintWinLeft")
'            Me.Top = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintWinTop")
'            Me.Width = nWidth
'            Me.Height = nHeight
'        Else
'            Me.Width = g_ptDefWindowSize.X
'            Me.Height = g_ptDefWindowSize.Y
'        End If
'
'        Me.WindowState = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintWinState", vbNormal)
'
'        m_nRenderType = GetLongKeyValueEx(APP_REG_KEY & "Settings", "PrintRenderType", m_nRenderType)
'    End With
'End Sub
'
'Private Sub SaveSettings()
'    On Error Resume Next
'    With vsPrinter
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintZoomMode", .ZoomMode
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintZoom", Trim$(Str$(.Zoom))
'
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintPaperSize", .PaperSize
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintOrientation", .Orientation
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginLeft", Trim$(Str$(.MarginLeft))
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginRight", Trim$(Str$(.MarginRight))
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginTop", Trim$(Str$(.MarginTop))
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginBottom", Trim$(Str$(.MarginBottom))
'
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintWinState", Me.WindowState
'        If Me.WindowState = vbNormal Then
'            SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintWinWidth", Me.Width
'            SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintWinHeight", Me.Height
'            SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintWinLeft", Me.Left
'            SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintWinTop", Me.Top
'        End If
'
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintRenderType", m_nRenderType
'    End With
'End Sub
'
'Private Sub tmrPrintGrid_Timer()
'    On Error Resume Next
'    tmrPrintGrid.Enabled = False
'    If m_bInProc Then Exit Sub
'
'    m_bInProc = True
'    UpdateStatus
'
'    Screen.MousePointer = vbHourglass
'    PrintGrids m_nRenderType <> 0
'    Screen.MousePointer = vbDefault
'
'    m_bInProc = False
'    UpdateStatus
'End Sub
'
'Private Sub btnNav_Click(Index As Integer)
'    On Error Resume Next
'    Select Case Index
'        Case 1 ' First
'            vsPrinter.PreviewPage = 1
'
'        Case 2 ' Prev
'            vsPrinter.PreviewPage = vsPrinter.PreviewPage - 1
'
'        Case 3 ' Next
'            vsPrinter.PreviewPage = vsPrinter.PreviewPage + 1
'
'        Case 4 ' last
'            vsPrinter.PreviewPage = vsPrinter.PageCount
'
'    End Select
'    UpdateStatus
'End Sub
'
'Private Sub UpdateStatus()
'    On Error Resume Next
'    lblPage.Caption = "Page " & vsPrinter.PreviewPage & " of " & vsPrinter.PageCount
'
'    btnNav(1).Enabled = (vsPrinter.PageCount > 0 And vsPrinter.PreviewPage > 1)
'    btnNav(2).Enabled = btnNav(1).Enabled
'    btnNav(3).Enabled = (vsPrinter.PageCount > vsPrinter.PreviewPage)
'    btnNav(4).Enabled = btnNav(3).Enabled
'
'    btnPageSetup.Enabled = Not m_bInProc
'    btnPrint.Enabled = Not m_bInProc
'    btnSave.Enabled = Not m_bInProc
'    cbxView.Enabled = Not m_bInProc
'    lblView.Enabled = Not m_bInProc
'End Sub
'
'Private Sub vsPrinter_AfterUserPage()
'    On Error Resume Next
'    UpdateStatus
'End Sub
'
'Private Sub vsPrinter_BeforeUserZoom(NewZoom As Integer)
'    On Error Resume Next
'    NewZoom = vsPrinter.Zoom
'End Sub
'
'Private Sub vsPrinter_EndDoc()
'    On Error Resume Next
'    UpdateStatus
'End Sub
'
'Private Sub vsPrinter_EndPage()
'    On Error Resume Next
'    UpdateStatus
'End Sub
'
'Private Function GetTempFilePath() As String
'    On Error Resume Next
'    Dim sPath$
'
'    sPath = Space$(4096)
'    If GetTempPath(4096, sPath) <> 0 Then
'        sPath = Trim$(sPath)
'        sPath = Left$(sPath, Len(sPath) - 1)
'        If Mid$(sPath, Len(sPath), 1) <> "\" Then sPath = sPath & "\"
'    End If
'
'    GetTempFilePath = Trim$(sPath) & "EtsMM_temp_" & Int(100000001 * Rnd) & ".tmp"
'End Function
'
'Private Sub PrintGrids(ByVal bAsIs As Boolean)
'    On Error Resume Next
'    Dim i&, dtNow As Date, bSelectionOnly As Boolean
'    Dim sPath$, nStartRow&, nEndRow&, nStartCol&, nEndCol&, nRow&, nCol&
'
'    With vsPrinter
'        .Clear
'        .Header = ""
'        .Footer = ""
'
'        .MarginHeader = .MarginTop - ScaleY(10, vbPoints, vbTwips)
'        .MarginFooter = .MarginBottom - ScaleY(10, vbPoints, vbTwips)
'
'        .StartDoc
'        .HdrFontName = "Tahoma" 'Me.FontName
'        .HdrFontSize = 8
'
'        ' title
'        .FontName = "Tahoma" ' Me.FontName
'        .FontBold = True
'        .FontSize = 12
'        .TextAlign = taCenterTop
'        .Paragraph = m_sTitle
'
'        sPath = GetTempFilePath
'
'        For i = 0 To m_nLastGrid
'            .FontName = m_fg(i).FontName
'            .TextAlign = taLeftTop
'            .FontBold = False
'            .FontSize = 8
'            .Paragraph = ""
'
'            Err.Clear
'            m_fg(i).SaveGrid sPath, flexFileAll, True
'            If Err.Number = 0 Then fg.LoadGrid sPath, flexFileAll, True
'            If Err.Number = 0 Then
'                bSelectionOnly = False
'
'                If m_fg(i).SelectionMode <> flexSelectionListBox And m_fg(i).AllowSelection Then
'                    m_fg(i).GetSelection nStartRow, nStartCol, nEndRow, nEndCol
'                    bSelectionOnly = nStartRow <> nEndRow Or nStartCol <> nEndCol
'                End If
'
'                If bSelectionOnly Then
'                    For nRow = fg.FixedRows To fg.Rows - 1
'                        If nRow < nStartRow Or nRow > nEndRow Then
'                            fg.RowHidden(nRow) = True
'                        End If
'                    Next
'
'                    For nCol = fg.FixedCols To fg.Cols - 1
'                        If nCol < nStartCol Or nCol > nEndCol Then
'                            fg.ColHidden(nCol) = True
'                        End If
'                    Next
'                End If
'
'                If Not bAsIs Then
'                    fg.FontName = "Tahoma"
'                    fg.FontBold = False
'                    fg.FontItalic = False
'                    fg.FontStrikethru = False
'                    fg.FontSize = 8
'
'                    fg.Appearance = flex3DLight
'                    fg.BorderStyle = flexBorderFlat
'
'                    fg.GridLinesFixed = flexGridFlat
'                    fg.GridColorFixed = RGB(128, 128, 128)
'                    fg.BackColorFixed = RGB(224, 224, 224)
'
'                    fg.GridColor = RGB(192, 192, 192)
'                    fg.Cell(flexcpBackColor, fg.FixedRows, fg.FixedCols, fg.Rows - 1, fg.Cols - 1) = RGB(255, 255, 255)
'
'                    fg.Cell(flexcpForeColor, 0, 0, fg.Rows - 1, fg.Cols - 1) = RGB(1, 1, 1)
'
'                    fg.AutoSize 0, fg.Cols - 1, False, 0
'                End If
'
'                .RenderControl = fg.hWnd
'            Else
'                .RenderControl = m_fg(i).hWnd
'            End If
'        Next
'
'        Kill sPath
'
'        .EndDoc
'
'        .FontName = "Tahoma" 'Me.FontName
'        .FontBold = False
'        .FontSize = 8
'
'        dtNow = Now
'        For i = 1 To .PageCount
'            .StartOverlay i
'
'            .TextAlign = taLeftTop
'            .CurrentX = .MarginLeft
'            .CurrentY = .MarginHeader
'            .Text = APP_TITLE
'
'            .TextAlign = taRightTop
'            .Text = Format$(dtNow, "General Date")
'
'            .CurrentX = .MarginLeft
'            .CurrentY = .PageHeight - .MarginBottom
'            .TextAlign = taRightTop
'            .Paragraph = "Page " & i & " of " & .PageCount
'
'            .EndOverlay
'        Next
'    End With
'End Sub
'
'Private Sub ShowPrinterErrorMsgBox()
'    With vsPrinter
'        Select Case .Error
'        Case 0
'        Case vperCantAccessPrinter
'            gCmn.CantAccessPrinterErrorMsgBox
'        Case Else
'            gCmn.ErrorMsgBox m_frmOwner, .ErrorDescription
'        End Select
'    End With
'End Sub
'
'Private Sub vsPrinter_Error()
'    ShowPrinterErrorMsgBox
'End Sub
