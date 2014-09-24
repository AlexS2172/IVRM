VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmLog 
   Caption         =   "ETS Event Log"
   ClientHeight    =   1695
   ClientLeft      =   60
   ClientTop       =   345
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
   Icon            =   "frmLog.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   1695
   ScaleWidth      =   4725
   Begin VSFlex7Ctl.VSFlexGrid fgLog 
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
      Begin VB.Timer tmrFlash 
         Enabled         =   0   'False
         Interval        =   500
         Left            =   360
         Top             =   240
      End
      Begin MSComctlLib.ImageList imgList 
         Left            =   960
         Top             =   360
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
               Picture         =   "frmLog.frx":0442
               Key             =   ""
            EndProperty
            BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmLog.frx":059C
               Key             =   ""
            EndProperty
            BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
               Picture         =   "frmLog.frx":06F6
               Key             =   ""
            EndProperty
         EndProperty
      End
   End
   Begin VB.CommandButton btnClear 
      Caption         =   "Clear"
      Height          =   315
      Left            =   2160
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton btnClose 
      Cancel          =   -1  'True
      Caption         =   "Close"
      Height          =   315
      Left            =   3480
      TabIndex        =   2
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<context>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
         Enabled         =   0   'False
         Shortcut        =   ^C
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPrint 
         Caption         =   "Print..."
         Enabled         =   0   'False
         Visible         =   0   'False
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

Private Enum ColIndexEnum
    IDX_ID = 0
    IDX_TIME
    IDX_MESSAGE
    COLUMN_COUNT
End Enum

Private m_nFlash As Long
Private m_nEventID As Long
Private m_bKeyDown As Boolean
Private m_nLogFile As Long

Public Sub LogEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    Dim nRow&, nMsgRows&, bShowNewRow As Boolean
   
    With fgLog
        .Redraw = flexRDNone
        'sMessage = Replace(sMessage, vbLf, vbCrLf, , , vbTextCompare)
        m_nEventID = m_nEventID + 1
        
        bShowNewRow = (.Row < 1 Or .Row = .Rows - 1)
        
        .AddItem m_nEventID & vbTab & Now & vbTab & sMessage
        nRow = .Rows - 1
        
        nMsgRows = InStrCount(sMessage, vbCrLf)
        
        If nMsgRows > 0 Then
            .RowHeight(nRow) = .RowHeight(0) * (nMsgRows + 1)
        End If
        
        Set .Cell(flexcpPicture, nRow, IDX_TIME) = imgList.ListImages(enType).Picture
        .Cell(flexcpPictureAlignment, nRow, IDX_TIME) = flexPicAlignLeftCenter
        
        .Redraw = flexRDBuffered
    
        If bShowNewRow Then
            If .Col < 0 Then .Col = IDX_MESSAGE
            .Row = nRow
            SendMessage .hWnd, WM_VSCROLL, SB_BOTTOM, 0&
        End If
    End With
        
    If Me.Visible Then
        m_nFlash = 0
        tmrFlash.Enabled = True
    End If
    
    WriteToFile enType, Now, sMessage
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    
    With fgLog
        .Redraw = flexRDNone
        
        .Rows = 1
        .Cols = COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
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
        
        ' format columns
        .ColHidden(IDX_ID) = False
        .TextMatrix(0, IDX_ID) = "#"
        .ColDataType(IDX_ID) = flexDTLong
        
        .ColHidden(IDX_TIME) = False
        .TextMatrix(0, IDX_TIME) = "Time"
        .ColDataType(IDX_TIME) = flexDTDate
        .ColFormat(IDX_TIME) = ""
        
        .ColHidden(IDX_MESSAGE) = False
        .TextMatrix(0, IDX_MESSAGE) = "Message"
        .ColDataType(IDX_MESSAGE) = flexDTString
        .ColFormat(IDX_MESSAGE) = ""
        
        .ColAlignment(IDX_ID) = flexAlignCenterCenter
        .ColAlignment(IDX_TIME) = flexAlignLeftCenter
        .ColAlignment(IDX_MESSAGE) = flexAlignLeftCenter
        
        .Cell(flexcpAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexAlignCenterCenter
        .Cell(flexcpPictureAlignment, 0, 0, 0, COLUMN_COUNT - 1) = flexPicAlignRightCenter
    
        .Redraw = flexRDBuffered
    End With
End Sub

Public Sub AdjustGridColWidths()
    On Error Resume Next
    Dim bRemoveFirst As Boolean
    
    With fgLog
        .Redraw = flexRDNone
        
        bRemoveFirst = (.Rows = 1)
        If bRemoveFirst Then
            .AddItem 1000 & vbTab & Now & vbTab & String(50, "W")
            Set .Cell(flexcpPicture, .Rows - 1, IDX_TIME) = imgList.ListImages(EVENT_INFO).Picture
            .Cell(flexcpPictureAlignment, .Rows - 1, IDX_TIME) = flexPicAlignLeftCenter
        End If
        
        .AutoSize 0, 1
        .ColWidth(2) = .Width - .ColWidth(0) - .ColWidth(1) - ScaleX(GetSystemMetrics(SM_CXVSCROLL) + 10, vbPixels, vbTwips)
    
        If bRemoveFirst Then .RemoveItem 1
    
        .Redraw = flexRDBuffered
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

Private Sub fgLog_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
        m_bKeyDown = True
    End If
End Sub

Private Sub fgLog_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown Then
        m_bKeyDown = False
    
        If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'            mnuCopy_Click
        End If
    End If
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
'    gCmn.CustomizeForm Me
    
    m_nEventID = 0
    m_bKeyDown = False
    FormatGrid
    
    InitLogFile
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = (UnloadMode = vbFormControlMenu)
    mnuClose_Click
    
    CloseLogFile
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    
    btnClose.Left = ScaleWidth - btnClose.Width - 60
    btnClear.Left = btnClose.Left - btnClear.Width - 60
    btnClose.Top = ScaleHeight - btnClose.Height - 60
    btnClear.Top = btnClose.Top
    
    fgLog.Width = ScaleWidth - 120
    fgLog.Height = btnClose.Top - 120
End Sub

Private Sub fgLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    With fgLog
        If Button = vbRightButton And .MouseRow > -1 And .MouseCol > -1 Then
            PopupMenu mnuContext
        End If
    End With
End Sub

Private Sub mnuClear_Click()
    On Error Resume Next
    With fgLog
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
    g_Params.EventLogVisible = False
End Sub

Private Sub mnuPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute Me, "Event Log", "Event Log", fgLog
'    Screen.MousePointer = vbDefault
End Sub

Private Sub tmrFlash_Timer()
    On Error Resume Next
    If m_nFlash < 4 Then
        FlashWindow Me.hWnd, 1
        m_nFlash = m_nFlash + 1
    Else
        FlashWindow Me.hWnd, 0
        tmrFlash.Enabled = False
    End If
End Sub

Private Sub mnuCopy_Click()
'    On Error GoTo EH
'    g_ClipMgr.CopyGridToClipboard fgLog
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to copy the grid content to the clipboard."
End Sub

Private Sub InitLogFile()
    On Error Resume Next
    Dim sFileName$, sFilePath$
    Dim sUser$, sUserGroup$
    
    sUser = CurrentNtUserName
    sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUser, "UserGroup", "")
    sFilePath = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\EventLog", sUserGroup, "")
    
    If sFilePath = "" Then Exit Sub
    
    sFileName = "elog_" & sUser & Format$(Now(), "_mmddyyyy.log")
    sFileName = sFilePath & IIf(Mid(sFilePath, Len(sFilePath), 1) <> "\", "\", "") & sFileName
    
    m_nLogFile = FreeFile
    Open sFileName For Binary Access Write Shared As #m_nLogFile
End Sub

Private Sub WriteToFile(ByVal enType As EventTypeEnum, ByVal dtDate As Date, ByVal sMessage As String)
    On Error Resume Next
    Dim sType$, sData$
    
    If enType = EVENT_ERROR Then sType = "ERROR" & vbTab
    If enType = EVENT_INFO Then sType = "INFORMATION"
    If enType = EVENT_WARNING Then sType = "WARNING" & vbTab
    
    sData = CStr(m_nEventID) & vbTab & CStr(dtDate) & vbTab & sType & vbTab & sMessage & vbCrLf
    
    Put #m_nLogFile, LOF(m_nLogFile) + 1, sData
End Sub

Private Sub CloseLogFile()
    On Error Resume Next
    Close #m_nLogFile
End Sub
