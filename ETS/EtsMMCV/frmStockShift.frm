VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "Comdlg32.ocx"
Begin VB.Form frmStockShift 
   Caption         =   "Stock specific shift"
   ClientHeight    =   5295
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4230
   Icon            =   "frmStockShift.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5295
   ScaleMode       =   0  'User
   ScaleWidth      =   4230
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkShowAllSft 
      Caption         =   "Show all shifts"
      Height          =   315
      Left            =   2760
      TabIndex        =   2
      Top             =   720
      Width           =   1335
   End
   Begin VB.CheckBox chkShowAllUnd 
      Caption         =   "Show all underlyings"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   720
      Width           =   2175
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   360
      Top             =   4800
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnClose 
      Caption         =   "Close"
      Height          =   315
      Left            =   2760
      TabIndex        =   5
      Top             =   4920
      Width           =   1335
   End
   Begin VB.CommandButton btnLoad 
      Caption         =   "Load File"
      Height          =   315
      Left            =   1320
      TabIndex        =   4
      Top             =   4920
      Width           =   1335
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000004&
      Height          =   315
      Left            =   120
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   360
      Width           =   3975
   End
   Begin VSFlex7Ctl.VSFlexGrid fgShifts 
      Height          =   3735
      Left            =   120
      TabIndex        =   3
      Top             =   1080
      Width           =   3975
      _cx             =   7011
      _cy             =   6588
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
      AllowUserResizing=   1
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   10
      Cols            =   4
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
   Begin VB.Label lblFile 
      Caption         =   "Current File:"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   975
   End
End
Attribute VB_Name = "frmStockShift"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_bIsOpened As Boolean
Private m_frmOwner As Form


Public Event OnShiftsChanged(ByRef aShifts As Collection)


Private m_nFrmWidth As Long
Private m_nFrmHeight As Long
Private Const m_nMinFrmHeight As Long = 2845
Private Const m_nMinFrmWidth As Long = 4350

Private m_bInit As Boolean

Private m_nShiftID As Long
Private m_bShowCurUnd As Boolean
Private m_bShowCurSft As Boolean
Private m_sFullName As String

Private m_Shifts As Collection
Private m_ActualShifts As Collection

Private m_Unds As EtsMmRisksLib.MmRvUndColl


Public Sub ShowData(Optional ByVal sFileName As String = STR_NA, _
                    Optional ByRef aShifts As Collection = Nothing, _
                    Optional ByVal bShowCurUnd As Boolean = True, _
                    Optional ByVal bShowCurSft As Boolean = True, _
                    Optional ByRef aUnds As EtsMmRisksLib.MmRvUndColl = Nothing, _
                    Optional ByVal nShiftID As Long = BAD_LONG_VALUE, _
                    Optional ByRef frmOwner As Form = Nothing)
    On Error Resume Next
    
    If m_bIsOpened Then Exit Sub
    m_bIsOpened = True
    
    Set m_frmOwner = Nothing
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    Set m_Shifts = Nothing
    Set m_ActualShifts = Nothing
    Set m_Unds = Nothing
    
    m_sFullName = sFileName
    m_bShowCurUnd = bShowCurUnd
    m_bShowCurSft = bShowCurSft
    
    m_nShiftID = nShiftID
    
    Set m_Shifts = aShifts
    Set m_Unds = aUnds
    
    LoadData sFileName
    FillActualShifts
    InitGrid
    
    Load Me
    Me.Show
End Sub

Private Sub InitGrid()
    On Error Resume Next
    
    m_bInit = True
    
    chkShowAllUnd.Value = IIf(m_bShowCurUnd, 0, 1)
    chkShowAllSft.Value = IIf(m_bShowCurSft, 0, 1)
    txtFile.Text = IIf(m_sFullName = STR_NA, "", m_sFullName)
    
    fgShifts.Cols = 4
    fgShifts.Rows = m_ActualShifts.Count + 1
    
    fgShifts.TextMatrix(0, 1) = "Stock Symbol"
    fgShifts.TextMatrix(0, 2) = "Shift #"
    fgShifts.TextMatrix(0, 3) = "Shift Amount%"
    
    Dim i As Long
    Dim aStockShift As clsStockShift
    For Each aStockShift In m_ActualShifts
        i = i + 1
        
        fgShifts.TextMatrix(i, 0) = i
        fgShifts.TextMatrix(i, 1) = aStockShift.Symbol
        fgShifts.TextMatrix(i, 2) = aStockShift.ShiftID
        fgShifts.TextMatrix(i, 3) = aStockShift.Shift
    Next
    
    fgShifts.AutoSize (0)
    fgShifts.AutoSize (1)
    fgShifts.AutoSize (2)
    fgShifts.AutoSize (3)
    
    m_bInit = False
End Sub

Public Sub LoadData(ByVal sFileName As String)
    On Error Resume Next
    
    m_sFullName = sFileName
    
    If m_sFullName = STR_NA Or m_sFullName = "" Then Exit Sub
    
    Set m_Shifts = Nothing
    Set m_Shifts = New Collection
    
    Dim nLength As Long
    Dim sData As String
    
    nLength = FileLen(m_sFullName)
    If nLength > 0 Then
        sData = Space(nLength)
        
        Dim hFile As Long
        hFile = FreeFile
        Open m_sFullName For Binary Access Read As #hFile
        Get #hFile, , sData
        Close #hFile
    Else
        LogEvent EVENT_ERROR, "Empty stock shift file " & m_sFullName
        Exit Sub
    End If
    
    Dim vtFullInfo As Variant
    vtFullInfo = Split(sData, vbCrLf)
    
    Dim nLinesCount As Long
    nLinesCount = UBound(vtFullInfo) + 1
    If nLinesCount <= 0 Then
        LogEvent EVENT_ERROR, "Can't load data from stock shift file " & m_sFullName
        Exit Sub
    End If
    
    Dim aStockShift As clsStockShift
    Dim vtShiftInfo As Variant
    Dim i As Long
    For i = 0 To nLinesCount - 1
        vtShiftInfo = Split(vtFullInfo(i), vbTab)
        If UBound(vtShiftInfo) + 1 = 3 Then
        
            Set aStockShift = New clsStockShift
            
            aStockShift.Symbol = CStr(vtShiftInfo(0))
            aStockShift.ShiftID = CLng(vtShiftInfo(1))
            aStockShift.Shift = CLng(vtShiftInfo(2))
            
            m_Shifts.Add aStockShift, aStockShift.Symbol & CStr(aStockShift.ShiftID)
            
            Set aStockShift = Nothing
        Else
            If vtFullInfo(i) <> "" Then _
                LogEvent EVENT_WARNING, "Can't parse line " & CStr(i + 1) & " from stock shift file " & m_sFullName
        End If
    Next
    
    g_Params.StockShiftFile = m_sFullName
    RaiseEvent OnShiftsChanged(m_Shifts)
    
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnLoad_Click()
    On Error Resume Next
    
    dlgCommon.Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                                     Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
    
    dlgCommon.CancelError = True
    dlgCommon.DefaultExt = ".txt"
    dlgCommon.DialogTitle = "Load stock shifts"
    dlgCommon.FileName = ""
    'dlgCommon.InitDir = m_sFilePath
    dlgCommon.Filter = "Text files (*.txt)|*.txt|All Files|*.*"
    dlgCommon.FilterIndex = 1
    
    dlgCommon.ShowOpen
    If Err.Number = 0 Then
        Screen.MousePointer = vbHourglass
        
        'm_sFullName = dlgCommon.FileName
        LoadData dlgCommon.FileName
        FillActualShifts
        InitGrid
        
        Screen.MousePointer = vbDefault
    End If
End Sub

Private Sub chkShowAllUnd_Click()
    On Error Resume Next
    m_bShowCurUnd = Not CBool(chkShowAllUnd.Value)
    
    If m_bInit Then Exit Sub
    
    FillActualShifts
    InitGrid
End Sub

Private Sub chkShowAllSft_Click()
    On Error Resume Next
    m_bShowCurSft = Not CBool(chkShowAllSft.Value)
    
    If m_bInit Then Exit Sub
    
    FillActualShifts
    InitGrid
End Sub

Private Sub Form_Load()
    On Error Resume Next
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    Dim bExit As Boolean
    
    If Me.Height < m_nMinFrmHeight Then
        Me.Height = m_nMinFrmHeight
        bExit = True
    End If
    If Me.Width < m_nMinFrmWidth Then
        Me.Width = m_nMinFrmWidth
        bExit = True
    End If
    
    If bExit Then Exit Sub
    
    fgShifts.Move fgShifts.Left, fgShifts.Top, fgShifts.Width + (Me.Width - m_nFrmWidth), fgShifts.Height + (Me.Height - m_nFrmHeight)
    
    btnLoad.Move btnLoad.Left + (Me.Width - m_nFrmWidth), btnLoad.Top + (Me.Height - m_nFrmHeight)
    btnClose.Move btnClose.Left + (Me.Width - m_nFrmWidth), btnClose.Top + (Me.Height - m_nFrmHeight)
    
    txtFile.Width = txtFile.Width + (Me.Width - m_nFrmWidth)
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    m_bIsOpened = False
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    
    Set m_Unds = Nothing
    Set m_Shifts = Nothing
    Set m_ActualShifts = Nothing
End Sub

Private Sub FillActualShifts()
    On Error Resume Next
    
    Set m_ActualShifts = Nothing
    Set m_ActualShifts = New Collection
    
    Dim aStockShift As clsStockShift
    For Each aStockShift In m_Shifts
        If m_bShowCurSft Then
            If aStockShift.ShiftID = m_nShiftID Then
                If m_bShowCurUnd And Not m_Unds Is Nothing Then
                    If Not m_Unds.BySortKey(aStockShift.Symbol) Is Nothing Then
                        m_ActualShifts.Add aStockShift, aStockShift.Symbol & CStr(aStockShift.ShiftID)
                    End If
                Else
                    m_ActualShifts.Add aStockShift, aStockShift.Symbol & CStr(aStockShift.ShiftID)
                End If
            End If
        Else
            If m_bShowCurUnd And Not m_Unds Is Nothing Then
                If Not m_Unds.BySortKey(aStockShift.Symbol) Is Nothing Then
                    m_ActualShifts.Add aStockShift, aStockShift.Symbol & CStr(aStockShift.ShiftID)
                End If
            Else
                m_ActualShifts.Add aStockShift, aStockShift.Symbol & CStr(aStockShift.ShiftID)
            End If
        End If
    Next

End Sub
