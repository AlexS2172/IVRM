VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmGridLayout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Customize Columns"
   ClientHeight    =   6990
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   8430
   Icon            =   "frmGridLayout.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6990
   ScaleWidth      =   8430
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Frame fraGrid 
      Caption         =   " Grid Format "
      Height          =   2595
      Left            =   120
      TabIndex        =   31
      Top             =   3240
      Width           =   4035
      Begin VB.CheckBox chkGridLineHoriz 
         Caption         =   "Horizontal Lines"
         Height          =   255
         Left            =   2070
         TabIndex        =   11
         Top             =   875
         Width           =   1935
      End
      Begin VB.CheckBox chkGridLineVert 
         Caption         =   "Vertical Lines"
         Height          =   255
         Left            =   2070
         TabIndex        =   10
         Top             =   570
         Width           =   1935
      End
      Begin VB.PictureBox picGridLine 
         BackColor       =   &H00FFFFFF&
         Height          =   180
         Left            =   120
         ScaleHeight     =   120
         ScaleWidth      =   1785
         TabIndex        =   34
         Top             =   600
         Width           =   1845
      End
      Begin VB.PictureBox picGridSelPreview 
         Height          =   540
         Left            =   120
         ScaleHeight     =   480
         ScaleWidth      =   3735
         TabIndex        =   32
         TabStop         =   0   'False
         Top             =   1200
         Width           =   3795
         Begin EtsMM.ctlLabelEx lblGridSelPreview 
            Height          =   480
            Left            =   0
            TabIndex        =   33
            Tag             =   "noauto"
            Top             =   0
            Width           =   3735
            _ExtentX        =   6588
            _ExtentY        =   847
            BackColor       =   -2147483643
            Enabled         =   0   'False
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Arial"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Caption         =   "Selected Row"
            Alignment       =   2
            DisabledForeColor=   -2147483640
            HoverEffect     =   0
         End
      End
      Begin VB.CommandButton btnGridSelFore 
         Caption         =   "Fore Color..."
         Height          =   315
         Left            =   2070
         TabIndex        =   13
         Top             =   1800
         Width           =   1845
      End
      Begin VB.CommandButton btnGridSelBack 
         Caption         =   "Back Color..."
         Height          =   315
         Left            =   120
         TabIndex        =   12
         Top             =   1800
         Width           =   1845
      End
      Begin VB.CommandButton btnGridFont 
         Caption         =   "Font..."
         Height          =   315
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Width           =   1845
      End
      Begin VB.CommandButton btnGridLine 
         Caption         =   "Lines Color..."
         Height          =   315
         Left            =   120
         TabIndex        =   9
         Top             =   840
         Width           =   1845
      End
      Begin EtsMM.ctlLabelEx lblGridFont 
         Height          =   300
         Left            =   2070
         TabIndex        =   37
         Tag             =   "noauto"
         Top             =   240
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   529
         BackColor       =   -2147483633
         Enabled         =   0   'False
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Font Name"
         DisabledForeColor=   0
         HoverEffect     =   0
      End
   End
   Begin VB.CheckBox chkApplyToAll 
      Caption         =   "Apply This Configuration for All Opened Windows"
      Height          =   495
      Left            =   120
      TabIndex        =   20
      Top             =   5940
      Width           =   4035
   End
   Begin VB.CheckBox chkDefaultForNew 
      Caption         =   "Set as Default for New Windows"
      Height          =   495
      Left            =   4260
      TabIndex        =   21
      Top             =   5940
      Width           =   4035
   End
   Begin VB.Frame fraOrder 
      Caption         =   " Columns Order "
      Height          =   3015
      Left            =   120
      TabIndex        =   28
      Top             =   120
      Width           =   8175
      Begin VB.ListBox lstDest 
         Height          =   2205
         ItemData        =   "frmGridLayout.frx":000C
         Left            =   4155
         List            =   "frmGridLayout.frx":000E
         MultiSelect     =   2  'Extended
         TabIndex        =   5
         Top             =   480
         Width           =   3135
      End
      Begin VB.ListBox lstSrc 
         Height          =   2205
         ItemData        =   "frmGridLayout.frx":0010
         Left            =   120
         List            =   "frmGridLayout.frx":0012
         MultiSelect     =   2  'Extended
         Sorted          =   -1  'True
         TabIndex        =   0
         Top             =   480
         Width           =   3135
      End
      Begin VB.CommandButton btnDown 
         Caption         =   "&Down"
         Height          =   315
         Left            =   7410
         TabIndex        =   7
         Top             =   1560
         Width           =   615
      End
      Begin VB.CommandButton btnUp 
         Caption         =   "&Up"
         Height          =   315
         Left            =   7410
         TabIndex        =   6
         Top             =   1200
         Width           =   615
      End
      Begin VB.CommandButton btnMoveRightOne 
         Caption         =   ">"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3390
         TabIndex        =   1
         Top             =   720
         Width           =   615
      End
      Begin VB.CommandButton btnMoveLeftOne 
         Caption         =   "<"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3390
         TabIndex        =   3
         Top             =   1800
         Width           =   615
      End
      Begin VB.CommandButton btnMoveRightAll 
         Caption         =   ">>"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3390
         TabIndex        =   2
         Top             =   1080
         Width           =   615
      End
      Begin VB.CommandButton btnMoveLeftAll 
         Caption         =   "<<"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3390
         TabIndex        =   4
         Top             =   2160
         Width           =   615
      End
      Begin VB.Label lblStatus 
         BackStyle       =   0  'Transparent
         Caption         =   "Status"
         BeginProperty Font 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   195
         Left            =   4155
         TabIndex        =   40
         Top             =   2730
         Width           =   3690
      End
      Begin VB.Label lblSrc 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Available Columns :"
         Height          =   195
         Left            =   120
         TabIndex        =   30
         Top             =   240
         Width           =   1785
         WordWrap        =   -1  'True
      End
      Begin VB.Label lblDest 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Selected Columns :"
         Height          =   390
         Left            =   4155
         TabIndex        =   29
         Top             =   240
         Width           =   2265
         WordWrap        =   -1  'True
      End
   End
   Begin VB.Frame fraColumn 
      Caption         =   " Column Format "
      Height          =   2595
      Left            =   4260
      TabIndex        =   25
      Top             =   3240
      Width           =   4035
      Begin VB.ComboBox cmbDecimalPlaces 
         Height          =   315
         Left            =   1290
         Style           =   2  'Dropdown List
         TabIndex        =   42
         Top             =   2175
         Width           =   675
      End
      Begin VB.CheckBox chkThousandSeparator 
         Caption         =   "Use 1000 Separator"
         Height          =   195
         Left            =   2070
         TabIndex        =   19
         Top             =   2220
         Width           =   1875
      End
      Begin VB.PictureBox picAltPreview2 
         Height          =   540
         Left            =   2070
         ScaleHeight     =   480
         ScaleWidth      =   1785
         TabIndex        =   38
         TabStop         =   0   'False
         Top             =   1200
         Width           =   1845
         Begin EtsMM.ctlLabelEx lblAltPreview2 
            Height          =   480
            Left            =   0
            TabIndex        =   39
            Tag             =   "noauto"
            Top             =   0
            Width           =   1785
            _ExtentX        =   3149
            _ExtentY        =   847
            BackColor       =   16777215
            Enabled         =   0   'False
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Arial"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Caption         =   "AaBbCcXxYyZz"
            Alignment       =   2
            DisabledForeColor=   0
            HoverEffect     =   0
         End
      End
      Begin VB.PictureBox picAltPreview1 
         Height          =   540
         Left            =   120
         ScaleHeight     =   480
         ScaleWidth      =   1785
         TabIndex        =   35
         TabStop         =   0   'False
         Top             =   1200
         Width           =   1845
         Begin EtsMM.ctlLabelEx lblAltPreview1 
            Height          =   480
            Left            =   0
            TabIndex        =   36
            Tag             =   "noauto"
            Top             =   0
            Width           =   1785
            _ExtentX        =   3149
            _ExtentY        =   847
            BackColor       =   16777215
            Enabled         =   0   'False
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Arial"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Caption         =   "AaBbCcXxYyZz"
            Alignment       =   2
            DisabledForeColor=   0
            HoverEffect     =   0
         End
      End
      Begin VB.PictureBox picPreview 
         Height          =   540
         Left            =   120
         ScaleHeight     =   480
         ScaleWidth      =   3735
         TabIndex        =   26
         TabStop         =   0   'False
         Top             =   240
         Width           =   3795
         Begin EtsMM.ctlLabelEx lblPreview 
            Height          =   480
            Left            =   0
            TabIndex        =   27
            Tag             =   "noauto"
            Top             =   0
            Width           =   3735
            _ExtentX        =   6588
            _ExtentY        =   847
            BackColor       =   16777215
            Enabled         =   0   'False
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "Arial"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Caption         =   "AaBbCcXxYyZz"
            Alignment       =   2
            DisabledForeColor=   0
            HoverEffect     =   0
         End
      End
      Begin VB.CommandButton btnFore 
         Caption         =   "Fore Color..."
         Height          =   315
         Left            =   1425
         TabIndex        =   15
         Top             =   840
         Width           =   1195
      End
      Begin VB.CommandButton btnBack 
         Caption         =   "Back Color..."
         Height          =   315
         Left            =   120
         TabIndex        =   14
         Top             =   840
         Width           =   1195
      End
      Begin VB.CommandButton btnApplyToAllCols 
         Caption         =   "Apply to All..."
         Height          =   315
         Left            =   2720
         TabIndex        =   16
         Top             =   840
         Width           =   1195
      End
      Begin VB.CommandButton btnAltFore1 
         Caption         =   "Fore Color..."
         Height          =   315
         Left            =   120
         TabIndex        =   17
         Top             =   1800
         Width           =   1845
      End
      Begin VB.CommandButton btnAltFore2 
         Caption         =   "Fore Color..."
         Height          =   315
         Left            =   2070
         TabIndex        =   18
         Top             =   1800
         Width           =   1845
      End
      Begin VB.Label lblDecimalPlaces 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Dec. Places :"
         Height          =   195
         Left            =   120
         TabIndex        =   41
         Top             =   2220
         Width           =   1095
      End
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   4200
      TabIndex        =   22
      Top             =   6540
      Width           =   1245
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   5625
      TabIndex        =   23
      Top             =   6540
      Width           =   1245
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Height          =   315
      Left            =   7050
      TabIndex        =   24
      Top             =   6540
      Width           =   1245
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   7440
      Top             =   0
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
      CancelError     =   -1  'True
   End
End
Attribute VB_Name = "frmGridLayout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK(ByVal enGridType As GridTypeEnum, ByRef gdGrid As clsGridDef)
Public Event OnApply(ByVal enGridType As GridTypeEnum, ByRef gdGrid As clsGridDef)

Private WithEvents frmApply As frmLayoutApply
Attribute frmApply.VB_VarHelpID = -1

Private m_gdGrid As New clsGridDef
Private m_nLastColIdx As Long
Private m_nFirstColIdx As Long
Private m_bDirty As Boolean
Private m_enGridType As GridTypeEnum
Private m_nCurColIdx As Long

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bInitValues As Boolean

Public Sub Execute(ByVal enGridType As GridTypeEnum, ByVal nFirstColIdx As Long, ByVal nLastColIdx As Long, _
                    ByRef gdGrid As clsGridDef, ByRef frmOwner As Form)

    On Error GoTo EH
    If m_bIsOpened Or nLastColIdx <= 0 Then Exit Sub
    
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    
    m_nCurColIdx = -1
    m_enGridType = enGridType
    m_nFirstColIdx = nFirstColIdx
    m_nLastColIdx = nLastColIdx
    If m_nFirstColIdx > m_nLastColIdx Or m_nFirstColIdx <= 0 Then m_nFirstColIdx = 0
    
    gdGrid.CopyTo m_gdGrid
    
    Load Me
    InitLists
    
    Dim sViewName$, sColumnsName$
    sViewName = GetDefaultGridViewName(enGridType)
    sColumnsName = GetDefaultGridColumnsName(enGridType)
    Me.Caption = sViewName & " " & sColumnsName & " Grid Layout"
    chkDefaultForNew.Caption = "Set as Default for New " & sViewName & " Windows"
    chkApplyToAll.Caption = "Apply This Configuration for All Opened " & sViewName & " Windows"

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Grid Layout for """ & sColumnsName & """ Executed", m_frmOwner.Caption

    LoadSettings
    
    If m_gdGrid.IsRowSelection Then
        lblGridSelPreview.Left = 0
        lblGridSelPreview.Top = 0
        lblGridSelPreview.Width = picGridSelPreview.ScaleWidth
        lblGridSelPreview.Height = picGridSelPreview.ScaleHeight
        lblGridSelPreview.Caption = "Selected Row"
        btnGridSelFore.Enabled = True
        btnGridSelBack.Caption = "Back Color..."
    Else
        lblGridSelPreview.Left = 60
        lblGridSelPreview.Top = 60
        lblGridSelPreview.Width = picGridSelPreview.ScaleWidth - 120
        lblGridSelPreview.Height = picGridSelPreview.ScaleHeight - 120
        lblGridSelPreview.Caption = "Selected Cell"
        lblGridSelPreview.BackColor = &H80000005
        lblGridSelPreview.DisabledForeColor = &H80000008
        btnGridSelFore.Enabled = False
        btnGridSelBack.Caption = "Frame Color..."
    End If
    
    chkApplyToAll.Enabled = Not m_gdGrid.IsGlobal
    chkDefaultForNew.Enabled = Not m_gdGrid.IsGlobal
    chkDefaultForNew.Value = IIf(m_gdGrid.IsGlobal, 1, 0)
    
    ShowPreview
    ShowGridPreview
    m_bDirty = False
    UpdateControls
    
    Me.Show vbModeless, m_frmOwner
    lstSrc.SetFocus
    
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open format dialog."
    On Error Resume Next
    Unload Me
'
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
End Sub

Private Sub btnApply_Click()
    Dim i As Integer
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    SaveSettings
    If m_bDirty Then
        SaveLists
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogUserAction, "Changing Grid Layout Apply clicked. State of Checkboxes: " _
                                                            & "Apply To All: " & chkApplyToAll.Value _
                                                            & "Default For New: " & chkDefaultForNew.Value, m_frmOwner.Caption
    
    If chkApplyToAll.Value <> 0 Then
        ApplyToAll
    ElseIf m_bDirty Then
        RaiseEvent OnApply(m_enGridType, m_gdGrid)
    End If
    
    '
    Dim sTest As String
    sTest = lstDest.List(1)
    
    'lstDest.ListCount
    'lstDest.ListIndex
    
    m_bDirty = False
    UpdateControls

    If chkDefaultForNew.Value <> 0 Then
        SaveAsDefaultForNew
    End If
    Screen.MousePointer = vbDefault
End Sub

Private Sub btnApplyToAllCols_Click()
    On Error Resume Next
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        frmApply.Execute True, True, Me
    End If
End Sub

Private Sub btnGridSelBack_Click()
    On Error Resume Next
    With dlgCommon
        .Flags = cdlCCRGBInit Or cdlCCFullOpen
        .Color = m_gdGrid.BackColorSel
        .ShowColor
        If Err.Number = 0 Then
            m_gdGrid.BackColorSel = .Color
            ShowGridPreview
            m_bDirty = True
            UpdateControls
        End If
    End With
End Sub

Private Sub btnGridFont_Click()
    On Error Resume Next
    With dlgCommon
        .Flags = cdlCFScreenFonts Or cdlCFEffects
        .FontName = m_gdGrid.Font.Name
        .FontSize = m_gdGrid.Font.Size
        .FontBold = m_gdGrid.Font.Bold
        .FontItalic = m_gdGrid.Font.Italic
        .FontStrikethru = m_gdGrid.Font.Strikethrough
        .FontUnderline = m_gdGrid.Font.Underline
        .ShowFont
        If Err.Number = 0 Then
            m_gdGrid.Font.Name = .FontName
            m_gdGrid.Font.Size = .FontSize
            m_gdGrid.Font.Bold = .FontBold
            m_gdGrid.Font.Italic = .FontItalic
            m_gdGrid.Font.StrikeThru = .FontStrikethru
            m_gdGrid.Font.Underline = .FontUnderline
            ShowPreview
            ShowGridPreview
            m_bDirty = True
            UpdateControls
        End If
    End With
End Sub

Private Sub btnGridSelFore_Click()
    On Error Resume Next
    With dlgCommon
        .Flags = cdlCCRGBInit Or cdlCCFullOpen
        .Color = m_gdGrid.ForeColorSel
        .ShowColor
        If Err.Number = 0 Then
            m_gdGrid.ForeColorSel = .Color
            ShowGridPreview
            m_bDirty = True
            UpdateControls
        End If
    End With
End Sub

Private Sub btnGridLine_Click()
    On Error Resume Next
    With dlgCommon
        .Flags = cdlCCRGBInit Or cdlCCFullOpen
        .Color = m_gdGrid.GridLinesColor
        .ShowColor
        If Err.Number = 0 Then
            m_gdGrid.GridLinesColor = .Color
            ShowGridPreview
            m_bDirty = True
            UpdateControls
        End If
    End With
End Sub

Private Sub chkGridLineHoriz_Click()
    On Error Resume Next
    m_gdGrid.HorizLinesVisible = (chkGridLineHoriz.Value <> 0)
    m_bDirty = True
    UpdateControls
End Sub

Private Sub chkGridLineVert_Click()
    On Error Resume Next
    m_gdGrid.VertLinesVisible = (chkGridLineVert.Value <> 0)
    m_bDirty = True
    UpdateControls
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Dim i&
    
    Set frmApply = New frmLayoutApply
    
    For i = 0 To 10
        cmbDecimalPlaces.AddItem i
    Next
    
    lblGridFont.BackColor = fraGrid.BackColor
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set frmApply = Nothing
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set frmApply = Nothing
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub InitLists()
    On Error Resume Next
    Dim i&, nIdx&
    
    lstDest.Clear
    lstSrc.Clear

    For i = m_nFirstColIdx To m_nLastColIdx
        m_gdGrid.Col(i).Visible = False
    Next

    For i = 0 To m_nLastColIdx
        nIdx = m_gdGrid.Idx(i)
        If nIdx >= m_nFirstColIdx And nIdx <= m_nLastColIdx Then
            m_gdGrid.Col(nIdx).Visible = True
            lstDest.AddItem m_gdGrid.Col(nIdx).Caption & vbTab & m_gdGrid.Col(nIdx).Description
            lstDest.ItemData(lstDest.NewIndex) = m_gdGrid.Idx(i)
        End If
    Next
    
    For i = m_nFirstColIdx To m_nLastColIdx
        If Not m_gdGrid.Col(i).Visible Then
            lstSrc.AddItem m_gdGrid.Col(i).Caption & vbTab & m_gdGrid.Col(i).Description
            lstSrc.ItemData(lstSrc.NewIndex) = i
        End If
    Next
    
    If lstSrc.ListCount > 0 Then
        lstSrc.ListIndex = 0
        m_nCurColIdx = lstSrc.ItemData(lstSrc.ListIndex)
    End If
    
    If lstDest.ListCount > 0 Then
        lstDest.ListIndex = 0
        If m_nCurColIdx < 0 Then m_nCurColIdx = lstDest.ItemData(lstDest.ListIndex)
    End If
    
    m_bDirty = False
    UpdateControls
End Sub

Private Sub SaveLists()
    On Error Resume Next
    Dim i&, nCount&

    nCount = lstSrc.ListCount - 1
    For i = 0 To nCount
        m_gdGrid.Col(lstSrc.ItemData(i)).Visible = False
    Next
    
    For i = 0 To m_nLastColIdx
        m_gdGrid.Idx(i) = -1
    Next
    
    nCount = lstDest.ListCount - 1
    For i = 0 To nCount
        m_gdGrid.Idx(i) = lstDest.ItemData(i)
        m_gdGrid.Col(m_gdGrid.Idx(i)).Visible = True
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Grid Layout Column """ & m_gdGrid.Col(m_gdGrid.Idx(i)).Caption & """ Selected", m_frmOwner.Caption
    Next
End Sub

' proc for moving list's items from Src to Dest
' Opt = 0 - move only selected items, Opt = 1 - move all items
Private Sub MoveItem(ByRef Src As ListBox, ByRef Dest As ListBox, ByVal bAll As Boolean)
    On Error Resume Next
    Dim i&, nIdx&, nCount&
    
    i = 0
    nCount = Src.ListCount
    Do While i < nCount
        If bAll Or Not bAll And Src.Selected(i) Then
            nIdx = Src.ItemData(i)
            If m_gdGrid.Col(nIdx).CanChangeOrder Then
                Src.RemoveItem i
                Dest.AddItem m_gdGrid.Col(nIdx).Caption & vbTab & m_gdGrid.Col(nIdx).Description
                Dest.ItemData(Dest.NewIndex) = nIdx
                Dest.Selected(Dest.NewIndex) = True
                nCount = nCount - 1
                m_bDirty = True
            Else
                i = i + 1
            End If
        Else
            i = i + 1
        End If
    Loop
    
    UpdateControls
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Changing Layout was Canceled without Modification. ", Me.Caption
 
    'Me.Hide
    Unload Me
End Sub

Private Sub btnMoveRightOne_Click()
    On Error Resume Next
    MoveItem lstSrc, lstDest, False
End Sub

Private Sub btnMoveLeftOne_Click()
    On Error Resume Next
    MoveItem lstDest, lstSrc, False
End Sub

Private Sub btnMoveRightAll_Click()
    On Error Resume Next
    MoveItem lstSrc, lstDest, True
End Sub

Private Sub btnMoveLeftAll_Click()
    On Error Resume Next
    MoveItem lstDest, lstSrc, True
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    Dim bCanMoveUp As Boolean, bCanMoveDn As Boolean, bCanVisible As Boolean, nCount&, nIdx&, i&, bCanChangeOrder As Boolean, bCanChangeVisible As Boolean
    Dim nCurIdx&, sStatus$, bCanChangeVisibleAll As Boolean
    
    nCount = lstDest.ListCount
    bCanMoveUp = False
    bCanMoveDn = False
    bCanVisible = False
    bCanChangeOrder = False
    bCanChangeVisible = False
    bCanChangeVisibleAll = False
    
    nCurIdx = -1
    If nCount > 0 Then
        nCurIdx = lstDest.ListIndex
        If nCurIdx >= 0 Then
            nIdx = lstDest.ItemData(nCurIdx)
            bCanChangeOrder = m_gdGrid.Col(nIdx).CanChangeOrder
            bCanChangeVisible = m_gdGrid.Col(nIdx).CanChangeVisible
            bCanMoveUp = nCurIdx > 0 And bCanChangeOrder
            bCanMoveDn = nCurIdx < lstDest.ListCount - 1 And bCanChangeOrder
        End If
        
        nCount = nCount - 1
        For i = 0 To nCount
            If lstDest.Selected(i) Then
                nIdx = lstDest.ItemData(i)
                bCanVisible = m_gdGrid.Col(nIdx).CanChangeVisible
                If Not bCanVisible Then Exit For
            End If
        Next
        
        If Not bCanChangeVisibleAll Then
            nIdx = lstDest.ItemData(nCount)
            bCanChangeVisibleAll = m_gdGrid.Col(nIdx).CanChangeVisible
        End If
    End If
    
    btnMoveLeftOne.Enabled = bCanVisible
    btnMoveLeftAll.Enabled = bCanChangeVisibleAll And nCount > 0
    btnDown.Enabled = bCanMoveDn
    btnUp.Enabled = bCanMoveUp
    
    sStatus = ""
    If nCurIdx >= 0 Then
        If Not bCanChangeVisible And Not bCanChangeOrder Then
            sStatus = "Can Not Hide or Move this Column."
        ElseIf Not bCanChangeVisible Then
            sStatus = "Can Not Hide this Column."
        ElseIf Not bCanChangeOrder Then
            sStatus = "Can Not Move this Column."
        End If
    End If
    
    lblStatus.Caption = sStatus
    
    btnOk.Enabled = (lstDest.ListCount > 0)
    btnApply.Enabled = btnOk.Enabled And m_bDirty
    
    nCount = lstSrc.ListCount
    bCanMoveUp = False
    bCanMoveDn = False
    bCanVisible = nCount > 0
    If nCount > 0 Then
        nCount = nCount - 1
        For i = 0 To nCount
            If lstSrc.Selected(i) Then
                nIdx = lstSrc.ItemData(i)
                bCanVisible = m_gdGrid.Col(nIdx).CanChangeVisible
                If Not bCanVisible Then Exit For
            End If
        Next
    End If
    
    btnMoveRightOne.Enabled = bCanVisible
    btnMoveRightAll.Enabled = nCount > 0
End Sub

' changing group order
' Direction: 1 - up, (-1) - down
Private Sub MoveListItem(lst As ListBox, Direction As Long)
    On Error Resume Next
    Dim i&, sCaption$, nIdx&
    Dim bSelected As Boolean
    
    With lst
        i = .ListIndex
        nIdx = .ItemData(.ListIndex)
        If m_gdGrid.Col(nIdx).CanChangeOrder Then
            If (i + Direction) < 0 Then Exit Sub
            If (i + Direction) >= .ListCount Then Exit Sub
            
            If m_gdGrid.Col(.ItemData(i + Direction)).CanChangeOrder Then
                sCaption = .List(i)
                bSelected = .Selected(i)
                .RemoveItem i
                
                .AddItem sCaption, i + Direction
                i = .NewIndex
                .ItemData(i) = nIdx
                .Selected(i) = bSelected
                .ListIndex = i
                m_bDirty = True
                UpdateControls
            End If
        End If
    End With
End Sub

Private Sub btnDown_Click()
    On Error Resume Next
    MoveListItem lstDest, 1
End Sub

Private Sub ApplyToAll()
    On Error Resume Next
    Dim i&, nCount&, aForm As clsFormAtom

    nCount = g_ViewFrm.Count
    For i = 1 To nCount
        Set aForm = g_ViewFrm(i)
        
        Select Case m_enGridType
            Case GT_QUOTES_UNDERLYING, GT_QUOTES_DIVIDENDS, GT_QUOTES_OPTIONS, GT_QUOTES_VOLA
                If TypeOf aForm.Frm Is frmQuotesViewSingle Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_TRADES_FILTER, GT_TRADES_LIST
                If TypeOf aForm.Frm Is frmTradeView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_RISKS_FILTER, GT_RISKS_TOTALS, GT_RISKS_POSITIONS
                If TypeOf aForm.Frm Is frmRiskView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_MATRIX_FILTER, GT_MATRIX_SCENARIO, GT_MATRIX_VALUES
                If TypeOf aForm.Frm Is frmRiskMatrix Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_VOLA_FILTER, GT_VOLA_UNDERLYING
                If TypeOf aForm.Frm Is frmVolaMonitor Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_QUOTATION_UNDERLYING, GT_QUOTATION_DIVIDENDS, GT_QUOTATION_OPTIONS, GT_QUOTATION_LOG
                If TypeOf aForm.Frm Is frmQuotationView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_VOLAANALYSIS_FILTER, GT_VOLAANALYSIS_UNDERLYING
                If TypeOf aForm.Frm Is frmVolaAnalysis Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_GREEKS_FILTER, GT_GREEKS_COLUMNS
                If TypeOf aForm.Frm Is frmGreeksView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_HEDGESUMMARY_FILTER, GT_HEDGESUMMARY_INSTRUMENTS, GT_HEDGESUMMARY_RESULTS, GT_HEDGESUMMARY_TOTAL
                If TypeOf aForm.Frm Is frmHedgeSummaryView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_REPORTS_FILTER
                If TypeOf aForm.Frm Is frmReportsView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_ORDERS_FILTER, GT_ORDERS_LIST, GT_ORDERS_LOG
                If TypeOf aForm.Frm Is frmOrderView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_TNTCARDS_FILTER, GT_TNTCARDS_LIST, GT_TNTCARDS_LOG
                If TypeOf aForm.Frm Is frmTntCardView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
            Case GT_STOCKHEDGE_FILTER, GT_STOCKHEDGE_TOTALS, GT_STOCKHEDGE_POSITIONS
                If TypeOf aForm.Frm Is frmStockHedgeView Then
                    aForm.Frm.SetGridLayout m_enGridType, m_gdGrid
                End If
                
        End Select
        Set aForm = Nothing
    Next
End Sub

Private Sub btnOk_Click()
    On Error Resume Next
    SaveSettings
    'Me.Hide
    If m_bDirty Then
        SaveLists
        If Not g_PerformanceLog Is Nothing Then _
           g_PerformanceLog.LogMmInfo enLogUserAction, "Changing Grid Layout OK clicked. State of Checkboxes: " _
                                                            & "Check Apply To All: " & chkApplyToAll.Value _
                                                            & "; Check Default For New: " & chkDefaultForNew.Value, m_frmOwner.Caption
    Else
            g_PerformanceLog.LogMmInfo enLogUserAction, "Changing Grid Layout OK clicked After Apply ", m_frmOwner.Caption
    End If
    
    If chkApplyToAll.Value <> 0 Then
        ApplyToAll
    ElseIf m_bDirty Then
        RaiseEvent OnOK(m_enGridType, m_gdGrid)
    End If
    
    If chkDefaultForNew.Value <> 0 Then
        Screen.MousePointer = vbHourglass
        SaveAsDefaultForNew
        Screen.MousePointer = vbDefault
    End If
    
    Unload Me
End Sub

Private Sub btnUp_Click()
    On Error Resume Next
    MoveListItem lstDest, -1
End Sub

Private Sub frmApply_OnOK(ByVal bBack As Boolean, ByVal bFore As Boolean)
    On Error Resume Next
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Dim aCurCol As clsColumnDef, aCol As clsColumnDef, i&
        Set aCurCol = m_gdGrid.Col(m_nCurColIdx)
        
        For i = m_nFirstColIdx To m_nLastColIdx
            If i <> m_nCurColIdx Then
                Set aCol = m_gdGrid.Col(i)
                
                If bBack Then
                    aCol.BackColor = aCurCol.BackColor
                End If
                
                If bFore Then
                    aCol.ForeColor = aCurCol.ForeColor
                End If
                
                Set aCol = Nothing
            End If
        Next
        
        Set aCurCol = Nothing
        m_bDirty = True
    End If
End Sub

Private Sub lstDest_Click()
    On Error Resume Next
    
    If lstDest.ListCount > 0 And lstDest.ListIndex >= 0 Then
        m_nCurColIdx = lstDest.ItemData(lstDest.ListIndex)
        ShowPreview
        UpdateControls
    End If
End Sub

Private Sub lstDest_DblClick()
    On Error Resume Next
    btnMoveLeftOne_Click
End Sub

Private Sub lstSrc_Click()
    On Error Resume Next
    
    If lstSrc.ListCount > 0 And lstSrc.ListIndex >= 0 Then
        m_nCurColIdx = lstSrc.ItemData(lstSrc.ListIndex)
        ShowPreview
        UpdateControls
    End If
End Sub

Private Sub lstSrc_DblClick()
    On Error Resume Next
    btnMoveRightOne_Click
End Sub

Private Sub ShowPreview()
    On Error Resume Next
    Dim aCol As clsColumnDef, nPos&
    m_bInitValues = True
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
        picPreview.BackColor = aCol.BackColor
        
        lblPreview.Caption = aCol.Description
        lblPreview.BackColor = aCol.BackColor
        lblPreview.DisabledForeColor = aCol.ForeColor
        
        lblPreview.Font.Name = m_gdGrid.Font.Name
        lblPreview.Font.Size = m_gdGrid.Font.Size
        lblPreview.Font.Bold = m_gdGrid.Font.Bold
        lblPreview.Font.Italic = m_gdGrid.Font.Italic
        lblPreview.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblPreview.Font.Underline = m_gdGrid.Font.Underline
        lblPreview.Refresh
    
        picAltPreview1.BackColor = aCol.BackColor
        lblAltPreview1.BackColor = aCol.BackColor
        lblAltPreview1.Font.Name = m_gdGrid.Font.Name
        lblAltPreview1.Font.Size = m_gdGrid.Font.Size
        lblAltPreview1.Font.Bold = m_gdGrid.Font.Bold
        lblAltPreview1.Font.Italic = m_gdGrid.Font.Italic
        lblAltPreview1.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblAltPreview1.Font.Underline = m_gdGrid.Font.Underline
        If Len(aCol.DescriptionAlt1) > 0 Then
            lblAltPreview1.Caption = aCol.DescriptionAlt1
            lblAltPreview1.DisabledForeColor = aCol.ForeColorAlt1
            btnAltFore1.Enabled = True
        Else
            lblAltPreview1.Caption = "N/A"
            lblAltPreview1.DisabledForeColor = aCol.ForeColor
            btnAltFore1.Enabled = False
        End If
        lblAltPreview1.Refresh
        
        picAltPreview2.BackColor = aCol.BackColor
        lblAltPreview2.BackColor = aCol.BackColor
        lblAltPreview2.Font.Name = m_gdGrid.Font.Name
        lblAltPreview2.Font.Size = m_gdGrid.Font.Size
        lblAltPreview2.Font.Bold = m_gdGrid.Font.Bold
        lblAltPreview2.Font.Italic = m_gdGrid.Font.Italic
        lblAltPreview2.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblAltPreview2.Font.Underline = m_gdGrid.Font.Underline
        If Len(aCol.DescriptionAlt2) > 0 Then
            lblAltPreview2.Caption = aCol.DescriptionAlt2
            lblAltPreview2.DisabledForeColor = aCol.ForeColorAlt2
            btnAltFore2.Enabled = True
        Else
            lblAltPreview2.Caption = "N/A"
            lblAltPreview2.DisabledForeColor = aCol.ForeColor
            btnAltFore2.Enabled = False
        End If
        lblAltPreview2.Refresh
        
        btnBack.Enabled = True
        btnFore.Enabled = True
        btnApplyToAllCols.Enabled = True
    
        Select Case aCol.DataType
            Case flexDTSingle, flexDTDouble, flexDTCurrency, flexDTDecimal
                chkThousandSeparator.Enabled = True
                lblDecimalPlaces.Enabled = True
                cmbDecimalPlaces.Enabled = True
                cmbDecimalPlaces.BackColor = &H80000005
                
                chkThousandSeparator.Value = IIf(InStr(aCol.Format, ",") > 0, 1, 0)
                
                nPos = InStr(aCol.Format, ".")
                If nPos > 0 Then
                    nPos = Len(aCol.Format) - Len(Left$(aCol.Format, nPos))
                    If nPos > 10 Then nPos = 10
                    cmbDecimalPlaces.ListIndex = nPos
                Else
                    cmbDecimalPlaces.ListIndex = 0
                End If
                
            Case flexDTShort, flexDTLong
                chkThousandSeparator.Enabled = True
                lblDecimalPlaces.Enabled = False
                cmbDecimalPlaces.Enabled = False
                cmbDecimalPlaces.ListIndex = -1
                cmbDecimalPlaces.BackColor = &H8000000F
                
                chkThousandSeparator.Value = IIf(InStr(aCol.Format, ",") > 0, 1, 0)
                
            Case Else
                chkThousandSeparator.Enabled = False
                chkThousandSeparator.Value = 0
                lblDecimalPlaces.Enabled = False
                cmbDecimalPlaces.Enabled = False
                cmbDecimalPlaces.ListIndex = -1
                cmbDecimalPlaces.BackColor = &H8000000F
        End Select
        
        Set aCol = Nothing
    Else
        picPreview.BackColor = &H80000005
        
        lblPreview.Caption = "<no column selected>"
        lblPreview.BackColor = &H80000005
        lblPreview.DisabledForeColor = &H80000008
        lblPreview.Font.Name = m_gdGrid.Font.Name
        lblPreview.Font.Size = m_gdGrid.Font.Size
        lblPreview.Font.Bold = m_gdGrid.Font.Bold
        lblPreview.Font.Italic = m_gdGrid.Font.Italic
        lblPreview.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblPreview.Font.Underline = m_gdGrid.Font.Underline
        lblPreview.Refresh
        
        picAltPreview1.BackColor = aCol.BackColor
        lblAltPreview1.BackColor = aCol.BackColor
        lblAltPreview1.Font.Name = m_gdGrid.Font.Name
        lblAltPreview1.Font.Size = m_gdGrid.Font.Size
        lblAltPreview1.Font.Bold = m_gdGrid.Font.Bold
        lblAltPreview1.Font.Italic = m_gdGrid.Font.Italic
        lblAltPreview1.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblAltPreview1.Font.Underline = m_gdGrid.Font.Underline
        lblAltPreview1.Caption = "N/A"
        lblAltPreview1.DisabledForeColor = aCol.ForeColor
        lblAltPreview1.Refresh
        
        picAltPreview2.BackColor = aCol.BackColor
        lblAltPreview2.BackColor = aCol.BackColor
        lblAltPreview2.Font.Name = m_gdGrid.Font.Name
        lblAltPreview2.Font.Size = m_gdGrid.Font.Size
        lblAltPreview2.Font.Bold = m_gdGrid.Font.Bold
        lblAltPreview2.Font.Italic = m_gdGrid.Font.Italic
        lblAltPreview2.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblAltPreview2.Font.Underline = m_gdGrid.Font.Underline
        lblAltPreview2.Caption = "N/A"
        lblAltPreview2.DisabledForeColor = aCol.ForeColor
        lblAltPreview2.Refresh
        
        btnBack.Enabled = False
        btnFore.Enabled = False
        btnApplyToAllCols.Enabled = False
        btnAltFore1.Enabled = False
        btnAltFore2.Enabled = False
        
        chkThousandSeparator.Enabled = False
        chkThousandSeparator.Value = 0
        lblDecimalPlaces.Enabled = False
        cmbDecimalPlaces.Enabled = False
        cmbDecimalPlaces.ListIndex = -1
        cmbDecimalPlaces.BackColor = &H8000000F
    End If

    m_bInitValues = False
End Sub

Private Sub btnBack_Click()
    On Error Resume Next
    Dim aCol As clsColumnDef
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
        With dlgCommon
            .Flags = cdlCCRGBInit Or cdlCCFullOpen
            .Color = aCol.BackColor
            .ShowColor
            If Err.Number = 0 Then
                aCol.BackColor = .Color
                ShowPreview
                m_bDirty = True
                UpdateControls
            End If
        End With
        Set aCol = Nothing
    End If
End Sub

Private Sub btnFore_Click()
    On Error Resume Next
    Dim aCol As clsColumnDef
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
        With dlgCommon
            .Flags = cdlCCRGBInit Or cdlCCFullOpen
            .Color = aCol.ForeColor
            .ShowColor
            If Err.Number = 0 Then
                aCol.ForeColor = .Color
                ShowPreview
                m_bDirty = True
                UpdateControls
            End If
        End With
        Set aCol = Nothing
    End If
End Sub

Private Sub btnAltFore1_Click()
    On Error Resume Next
    Dim aCol As clsColumnDef
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
        With dlgCommon
            .Flags = cdlCCRGBInit Or cdlCCFullOpen
            .Color = aCol.ForeColorAlt1
            .ShowColor
            If Err.Number = 0 Then
                aCol.ForeColorAlt1 = .Color
                ShowPreview
                m_bDirty = True
                UpdateControls
            End If
        End With
        Set aCol = Nothing
    End If
End Sub

Private Sub btnAltFore2_Click()
    On Error Resume Next
    Dim aCol As clsColumnDef
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
        With dlgCommon
            .Flags = cdlCCRGBInit Or cdlCCFullOpen
            .Color = aCol.ForeColorAlt2
            .ShowColor
            If Err.Number = 0 Then
                aCol.ForeColorAlt2 = .Color
                ShowPreview
                m_bDirty = True
                UpdateControls
            End If
        End With
        Set aCol = Nothing
    End If
End Sub

Private Sub ShowGridPreview()
    On Error Resume Next
    
    lblGridFont.Caption = m_gdGrid.Font.Name & ", " & CLng(m_gdGrid.Font.Size) & "pt"
    lblGridFont.Refresh
    
    picGridLine.BackColor = m_gdGrid.GridLinesColor
    chkGridLineHoriz.Value = IIf(m_gdGrid.HorizLinesVisible, 1, 0)
    chkGridLineVert.Value = IIf(m_gdGrid.VertLinesVisible, 1, 0)
    
    picGridSelPreview.BackColor = m_gdGrid.BackColorSel
    If m_gdGrid.IsRowSelection Then
        lblGridSelPreview.BackColor = m_gdGrid.BackColorSel
        lblGridSelPreview.Font.Name = m_gdGrid.Font.Name
        lblGridSelPreview.Font.Size = m_gdGrid.Font.Size
        lblGridSelPreview.Font.Bold = m_gdGrid.Font.Bold
        lblGridSelPreview.Font.Italic = m_gdGrid.Font.Italic
        lblGridSelPreview.Font.Strikethrough = m_gdGrid.Font.Strikethrough
        lblGridSelPreview.Font.Underline = m_gdGrid.Font.Underline
        lblGridSelPreview.DisabledForeColor = m_gdGrid.ForeColorSel
        lblGridSelPreview.Refresh
    End If
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    'SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "GridLayout", "ApplyToAll", chkApplyToAll.Value
    'SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "GridLayout", "DefaultForNew", chkDefaultForNew.Value
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    'chkApplyToAll.Value = IIf(GetLongKeyValueEx(APP_REG_KEY & "GridLayout", "ApplyToAll", 0) <> 0, 1, 0)
    'chkDefaultForNew.Value = IIf(GetLongKeyValueEx(APP_REG_KEY & "GridLayout", "DefaultForNew", 1) <> 0, 1, 0)
End Sub

Private Sub SaveAsDefaultForNew()
    On Error GoTo EH
    
    SaveGridDefAsDefault m_gdGrid, m_enGridType
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to set this configuration as default for new window."
End Sub

Private Sub chkThousandSeparator_Click()
    If m_bInitValues Then Exit Sub
    m_bInitValues = True
    AdjustCurColNumberFormat
    m_bInitValues = False
End Sub

Private Sub cmbDecimalPlaces_Click()
    On Error Resume Next
    If m_bInitValues Then Exit Sub
    m_bInitValues = True
    AdjustCurColNumberFormat
    m_bInitValues = False
End Sub

Private Sub AdjustCurColNumberFormat()
    On Error Resume Next
    Dim aCol As clsColumnDef, sFormat$
    
    If m_nFirstColIdx <= m_nCurColIdx And m_nCurColIdx <= m_nLastColIdx Then
        Set aCol = m_gdGrid.Col(m_nCurColIdx)
    
        Select Case aCol.DataType
            Case flexDTSingle, flexDTDouble, flexDTCurrency, flexDTDecimal
                If chkThousandSeparator.Value <> 0 Then
                    sFormat = "#,##0"
                Else
                    sFormat = "###0"
                End If
                
                If cmbDecimalPlaces.ListIndex > 0 Then sFormat = sFormat + "." + String$(cmbDecimalPlaces.ListIndex, "0")
                
                If aCol.Format <> sFormat Then
                    aCol.Format = sFormat
                    m_bDirty = True
                End If
                
            Case flexDTShort, flexDTLong
                If chkThousandSeparator.Value <> 0 Then
                    sFormat = "#,##0"
                Else
                    sFormat = "###0"
                End If
        
                If aCol.Format <> sFormat Then
                    aCol.Format = sFormat
                    m_bDirty = True
                End If
                
        End Select
        
        Set aCol = Nothing
    End If
End Sub
