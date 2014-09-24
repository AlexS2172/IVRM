VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmLog 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Event Log"
   ClientHeight    =   1695
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   4725
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1695
   ScaleWidth      =   4725
   ShowInTaskbar   =   0   'False
   Begin VSFlex7Ctl.VSFlexGrid grdLog 
      Height          =   1095
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   4605
      _cx             =   8123
      _cy             =   1931
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
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
      Begin VB.Timer tmrFlash 
         Enabled         =   0   'False
         Interval        =   500
         Left            =   360
         Top             =   240
      End
      Begin MSComctlLib.ImageList imgList 
         Left            =   480
         Top             =   840
         _ExtentX        =   1005
         _ExtentY        =   1005
         BackColor       =   -2147483643
         ImageWidth      =   16
         ImageHeight     =   16
         MaskColor       =   12632256
         _Version        =   393216
         BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
            NumListImages   =   3
            BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmLog.frx":0000
               Key             =   ""
            EndProperty
            BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmLog.frx":015A
               Key             =   ""
            EndProperty
            BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmLog.frx":02B4
               Key             =   ""
            EndProperty
         EndProperty
      End
   End
   Begin ElladaFlatControls.FlatButton btnClear 
      Height          =   285
      Left            =   2160
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Clear"
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Cancel          =   -1  'True
      Height          =   285
      Left            =   3480
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Close"
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<context>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
      End
      Begin VB.Menu mnuClear 
         Caption         =   "Clear"
      End
      Begin VB.Menu mnuSeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuClose 
         Caption         =   "Close"
      End
   End
End
Attribute VB_Name = "frmLog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private Declare Function FlashWindow Lib "user32" (ByVal hwnd As Long, ByVal bInvert As Long) As Long

Private Enum ColIndexEnum
    IDX_TIME = 0
    IDX_MESSAGE
    COLUMN_COUNT
End Enum

Private m_nFlash As Long

Public Sub LogEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    
    With grdLog
        .Redraw = flexRDNone
        
        .AddItem Now & vbTab & sMessage
        Set .Cell(flexcpPicture, .Rows - 1, IDX_TIME) = imgList.ListImages(enType).Picture
        .Cell(flexcpPictureAlignment, .Rows - 1, IDX_TIME) = flexPicAlignLeftCenter
        .ShowCell .Rows - 1, IDX_TIME
        .Select .Rows - 1, IDX_MESSAGE
        .AutoSize 0, COLUMN_COUNT - 1, False, 100
        
        .Redraw = flexRDBuffered
    End With
        
    m_nFlash = 0
    tmrFlash.Enabled = True
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    
    With grdLog
        .Rows = 1
        .Cols = COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoResize = False
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone ' flexEDKbdMouse
        .Ellipsis = flexNoEllipsis
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .GridLineWidth = 1
        .MergeCells = flexMergeNever
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTips = True
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabCells
        
        .BackColorBkg = GCOLOR_TABLE_WHITE_BACK
        
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .BackColorAlternate = GCOLOR_TABLE_EVEN_ROW
'        .BackColorBkg = .BackColor
        .SheetBorder = .BackColorBkg
        .SheetBorder = .BackColorBkg
''        .GridColor = GCOLOR_TABLE_LINE
'        .GridColorFixed = GCOLOR_TABLE_CAPTION_LINE
        .BorderStyle = flexBorderFlat
        
        
        ' format columns
        .ColHidden(IDX_TIME) = False
        .TextMatrix(0, IDX_TIME) = "Time"
        .ColDataType(IDX_TIME) = flexDTDate
        .ColFormat(IDX_TIME) = ""
        
        .ColHidden(IDX_MESSAGE) = False
        .TextMatrix(0, IDX_MESSAGE) = "Message"
        .ColDataType(IDX_MESSAGE) = flexDTString
        .ColFormat(IDX_MESSAGE) = ""
        
        .ColAlignment(IDX_TIME) = flexAlignLeftCenter
        .ColAlignment(IDX_MESSAGE) = flexAlignLeftCenter
        
        .Cell(flexcpAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexAlignCenterCenter
        .Cell(flexcpPictureAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexPicAlignRightCenter
        .AutoSize 0, COLUMN_COUNT - 1, False, 200
    End With
End Sub

Private Sub btnClear_Click()
    On Error Resume Next
    mnuClear_Click
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    mnuClose_Click
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If KeyCode = vbKeyL Then
        If Shift = vbCtrlMask Then
            mnuClose_Click
        End If
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    
    FormatGrid
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = (UnloadMode = vbFormControlMenu)
    mnuClose_Click
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    
    btnClose.Left = ScaleWidth - btnClose.Width - 60
    btnClear.Left = btnClose.Left - btnClear.Width - 60
    btnClose.Top = ScaleHeight - btnClose.Height - 60
    btnClear.Top = btnClose.Top
    
    grdLog.Width = ScaleWidth - 120
    grdLog.Height = btnClose.Top - 120
End Sub

Private Sub grdLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    With grdLog
        If Button = 2 And .MouseRow > -1 And .MouseCol > -1 Then
            PopupMenu mnuContext
        End If
    End With
End Sub

Private Sub mnuClear_Click()
    On Error Resume Next
    With grdLog
        .Redraw = flexRDNone
        .Rows = 1
        .Redraw = flexRDBuffered
    End With
End Sub

Private Sub mnuClose_Click()
    On Error Resume Next
    g_lLogLeft = g_frmLog.Left
    g_lLogTop = g_frmLog.Top
    g_lLogWidth = g_frmLog.Width
    g_lLogHeight = g_frmLog.Height
    Hide
    frmMain.mnuOptionsEventLog.Checked = False
End Sub

Private Sub tmrFlash_Timer()
    On Error Resume Next
    If m_nFlash < 4 Then
        FlashWindow Me.hwnd, 1
        m_nFlash = m_nFlash + 1
    Else
        FlashWindow Me.hwnd, 0
        tmrFlash.Enabled = False
    End If
End Sub

Private Sub mnuCopy_Click()
    On Error GoTo EH
    CopyGridToClipboard grdLog
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Fail to copy the grid content to the clipboard."
End Sub

'Private Sub mnuPrint_Click()
'    On Error Resume Next
'    Load frmPrintPreview
'    frmPrintPreview.WindowState = vbMaximized
'
'    frmPrintPreview.PrintGrid grdLog, "Error Log"
'    frmPrintPreview.Show vbModal, frmMain
'    Unload frmPrintPreview
'End Sub

