VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.MDIForm frmMain 
   Appearance      =   0  'Flat
   BackColor       =   &H8000000F&
   Caption         =   "ETS BO"
   ClientHeight    =   7470
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   12435
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   ScrollBars      =   0   'False
   StartUpPosition =   3  'Windows Default
   WindowState     =   2  'Maximized
   Begin MSComDlg.CommonDialog cdlgMain 
      Left            =   360
      Top             =   1680
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox picTopNavigation 
      Align           =   1  'Align Top
      Appearance      =   0  'Flat
      BackColor       =   &H00400000&
      ForeColor       =   &H80000008&
      Height          =   360
      Left            =   0
      ScaleHeight     =   330
      ScaleWidth      =   12405
      TabIndex        =   2
      Tag             =   "noauto"
      Top             =   975
      Width           =   12435
      Begin ElladaFlatControls.ActiveLabel btnViewMore 
         Height          =   255
         Left            =   240
         TabIndex        =   4
         Top             =   0
         Visible         =   0   'False
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   450
         BackColor       =   4194304
         ForeColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   12380159
         Caption         =   "More..."
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnViewChild 
         Height          =   255
         Index           =   0
         Left            =   2160
         TabIndex        =   5
         Top             =   0
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   450
         BackColor       =   4194304
         ForeColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   12380159
         Caption         =   "Market Structure"
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnViewChild 
         Height          =   255
         Index           =   1
         Left            =   3840
         TabIndex        =   6
         Top             =   0
         Width           =   855
         _ExtentX        =   1508
         _ExtentY        =   450
         BackColor       =   4194304
         ForeColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   12380159
         Caption         =   "Entities"
         PressForeColor  =   12648447
      End
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   7215
      Width           =   12435
      _ExtentX        =   21934
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   3
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   16298
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   6
            Alignment       =   1
            TextSave        =   "11/5/2010"
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Style           =   5
            Alignment       =   1
            TextSave        =   "4:45 PM"
         EndProperty
      EndProperty
   End
   Begin VB.PictureBox picTopBanner 
      Align           =   1  'Align Top
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H00FFFFFF&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   975
      Left            =   0
      ScaleHeight     =   975
      ScaleWidth      =   12435
      TabIndex        =   0
      Tag             =   "noauto"
      Top             =   0
      Width           =   12435
      Begin VB.Label lblCaption 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         BeginProperty Font 
            Name            =   "System"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   240
         Left            =   2520
         TabIndex        =   3
         Top             =   660
         Width           =   60
      End
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFilePrint 
         Caption         =   "&Print"
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileLine1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewChild 
         Caption         =   "Market Structure"
         Index           =   0
      End
      Begin VB.Menu mnuViewChild 
         Caption         =   "Entities"
         Index           =   1
      End
      Begin VB.Menu mnuOptionsSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsEventLog 
         Caption         =   "&Event Log"
         Shortcut        =   ^L
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Enabled         =   0   'False
      Visible         =   0   'False
      Begin VB.Menu mnuConnections 
         Caption         =   "&Connections..."
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuEndOfDayManager 
         Caption         =   "End Of Day Manager..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuEndOfDay 
         Caption         =   "End Of Day Reconcilation, etc..."
      End
      Begin VB.Menu mnuOptionsSeparator111 
         Caption         =   "-"
      End
      Begin VB.Menu mnuEtsMM 
         Caption         =   "IV Risk Manager ..."
      End
      Begin VB.Menu mnuEtsMMCv 
         Caption         =   "Custom Variables Extention ..."
      End
      Begin VB.Menu mnuVolaManager 
         Caption         =   "Volatility Manager ..."
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&?"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Contents..."
         Shortcut        =   {F1}
      End
      Begin VB.Menu mnuHelpEgarOnWeb 
         Caption         =   "Egar on Web..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuHelpLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About..."
      End
   End
   Begin VB.Menu mnuViewMore 
      Caption         =   "View More"
      Visible         =   0   'False
      Begin VB.Menu mnuViewMoreChild 
         Caption         =   "Volatility"
         Index           =   4
      End
      Begin VB.Menu mnuViewMoreChild 
         Caption         =   "Risk Matrix"
         Index           =   5
      End
      Begin VB.Menu mnuViewMoreChild 
         Caption         =   "Market Structure"
         Index           =   6
      End
      Begin VB.Menu mnuViewMoreChild 
         Caption         =   "Entities"
         Index           =   7
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
 
Private Enum eWindowType
    WINTYPE_MARKETSTRUCTURE
    WINTYPE_ENTITIES
    INT_CHILDFORMSCOUNT
End Enum


Private marrChildForms(INT_CHILDFORMSCOUNT - 1) As Form
Attribute marrChildForms.VB_VarHelpID = -1
'Private WithEvents frmConnect As frmConnections

' SetWindowPos constants
Const SWP_FRAMECHANGED = &H20        '  The frame changed: send WM_NCCALCSIZE
Const SWP_DRAWFRAME = SWP_FRAMECHANGED

Private Sub ShowChildForm(frm As Form)
Dim f As Form
Dim i As Long
    On Error GoTo Herr
    If Not (Me.ActiveForm Is Nothing) Then
        If (Not Me.ActiveForm.CanLeaveFocus) And ShowForm Then Exit Sub
    End If
    
    ShowForm = False
    With frm
        .Visible = True
        .SetFocus
        MDIForm_Resize
        SelectActiveChild
    End With
    Exit Sub
Herr:
    ShowError "Error loading form"
End Sub

Public Sub SelectActiveChild()
Dim i As Long
    On Error GoTo Herr
    lblCaption.Caption = UCase(Me.ActiveForm.Caption)
    For i = 0 To INT_CHILDFORMSCOUNT - 1
        If ActiveForm Is marrChildForms(i) Then
            btnViewChild(i).ForeColor = vbYellow
        Else
            btnViewChild(i).ForeColor = vbWhite
        End If
        btnViewChild(i).HoverForeColor = btnViewChild(i).ForeColor
        MDIForm_Resize
    Next i
    Exit Sub
Herr:
    ShowError "Error activating form"
End Sub


Private Sub btnViewChild_Click(Index As Integer)
    On Error Resume Next
    btnViewChild(Index).MousePointer = vbHourglass
    mnuViewChild_Click Index
    btnViewChild(Index).MousePointer = 99
End Sub

Private Sub btnViewMore_Click()
    On Error Resume Next
    PopupMenu mnuViewMore, , btnViewMore.Left, picTopNavigation.Top + picTopNavigation.Height
End Sub


'Private Sub frmConnect_OnOK(ByVal bDBChanged As Boolean, ByVal bPPChanged As Boolean, ByVal bIVChanged As Boolean, ByVal bVSChanged As Boolean)
'    On Error Resume Next
'    If bDBChanged Or bPPChanged Or bVSChanged Then
'        gCmn.MyMsgBox "New settings will be actual only after application restart.", vbExclamation + vbOKOnly
'    End If
'End Sub

Private Sub MDIForm_Load()
    On Error GoTo Herr
      
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
        
    Me.Caption = AppTitle() & " v" & App.Major & "." & App.Minor & "." & App.Revision & " - " & g_Params.UserGroup & "/" & CurrentNtUserName()
    App.Title = Me.Caption
        
    picTopBanner.BackColor = GCOLOR_TABLE_CAPTION_BACK
        
    Set marrChildForms(WINTYPE_MARKETSTRUCTURE) = frmChildMarketStructure
    Set marrChildForms(WINTYPE_ENTITIES) = frmChildEntities
    
    mnuViewChild_Click WINTYPE_MARKETSTRUCTURE
    
'    Set frmConnect = New frmConnections
    
    CheckMarketStructureInCache
    
    DeleteOldOrders
    DeleteOldCards
    g_Params.SetIcon WND_MAIN, Me.hWnd, True
    mnuEtsMM.Caption = g_Params.AppIvRmTitle
    
    mnuEtsMMCv.Enabled = g_Params.IsCVEnabled
    mnuEtsMMCv.Visible = g_Params.IsCVEnabled
    
    Set picTopBanner.Picture = g_Params.TopBanner()
    Exit Sub
Herr:
    ShowError "Error loading form"
End Sub

Private Sub MDIForm_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Hide
    DoEvents
End Sub

Private Sub MDIForm_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Dim frm As Form
    Set frm = Me.ActiveForm
    If frm Is Nothing Then Exit Sub
    frm.Top = 0
    frm.Left = 0
    frm.Width = Me.ScaleWidth
    frm.Height = Me.ScaleHeight
'    SetWindowPos frm.hwnd, 0, 0, 0, 0, 0, SWP_NOOWNERZORDER Or SWP_NOMOVE Or SWP_NOSIZE Or SWP_DRAWFRAME

End Sub

Private Sub MDIForm_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    Set frmConnect = Nothing
    g_Params.Terminate
    ShutDown True
End Sub
'***************************************************************
'     Menu Handler
'***************************************************************


Private Sub mnuEndOfDayManager_Click()
    RunEndOfDayManager
End Sub

Private Sub mnuEtsMM_Click()
    On Error Resume Next
    RunEtsMM
End Sub

Private Sub mnuEtsMMCv_Click()
    On Error Resume Next
    RunEtsMMCv
End Sub

Private Sub mnuFileExit_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
'   Show Print Dialog - DEMO MODE
    cdlgMain.ShowPrinter
End Sub

Private Sub mnuHelpAbout_Click()
    On Error Resume Next
    frmAbout.Show vbModal, Me
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    Dim sPath$, hHelpWnd&
    
    hHelpWnd = FindWindow(ByVal 0&, "ETS Back Office Guide")
    If hHelpWnd = 0 Then
        sPath = App.Path & "\Documents\ETS Back Office Guide.chm"
        If ShellExecute(0&, "Open", sPath, "", vbNullString, SW_SHOWNORMAL) <= 32 Then
            gCmn.MyMsgBox "Fail to open help file '" & sPath & "'.", vbCritical
        End If
    Else
        SetWindowPos hHelpWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE
    End If
End Sub

Private Sub mnuHelpEgarOnWeb_Click()
'    On Error Resume Next
'Dim lReturn As Long
'    lReturn = ShellExecute(0&, "Open", "http://www.egartech.com", "", vbNullString, 1)
End Sub

Private Sub mnuConnections_Click()
'    On Error GoTo EH
'    frmConnect.Execute False, enCsoAll, False, Me
'    Exit Sub
'EH:
'    ShowError
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    ShowEventLog (Not mnuOptionsEventLog.Checked)
End Sub

Private Sub mnuViewChild_Click(Index As Integer)
    On Error Resume Next
    ShowChildForm marrChildForms(Index)
End Sub

Private Sub mnuViewMoreChild_Click(Index As Integer)
    On Error Resume Next
    mnuViewChild_Click Index
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuEndOfDay_Click()
    On Error GoTo Herr
    frmEndOfDay.Execute
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub RefreshTraders()
    On Error Resume Next
    marrChildForms(WINTYPE_ENTITIES).RefreshTraders
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function RefreshMarket()
    On Error Resume Next
    marrChildForms(WINTYPE_MARKETSTRUCTURE).RefreshMarket
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function RefreshCurrent()
    On Error Resume Next
    marrChildForms(WINTYPE_MARKETSTRUCTURE).RefreshCurrent
End Function

Private Sub CheckMarketStructureInCache()
 '   On Error GoTo EH
 '   Dim rs As ADODB.Recordset, sMsg$
        
 '   Set rs = gDBW.usp_EodMsUnderlyingCache_Get
 '   If Not rs.EOF Then
 '       sMsg = "You need to finish update market structure for some underlyings. Do it after reconcilation position or deleting erasing trades (Symbol(s): "
 
 '       While Not rs.EOF And Len(sMsg) < 1000
 '           sMsg = sMsg & " " & DBRead(rs!vcUndSymbol, "") & ","
 '           rs.MoveNext
 '       Wend
 
 '        If rs.EOF Then
 '           sMsg = Left$(sMsg, Len(sMsg) - 1) & ")"
 '       Else
 '           sMsg = Left$(sMsg, Len(sMsg) - 1) & "...)"
 '       End If
                
 '       gCmn.MyMsgBox sMsg
 '   End If
    
 '   Set rs = Nothing
 '   Exit Sub
'EH:
 '   ShowError
 '   Set rs = Nothing
End Sub

Public Sub RunEndOfDayManager()
'    On Error Resume Next
'    Dim sPath$
'    sPath = App.Path & "\EtsEodManager.exe"
'    If Shell(sPath, vbNormalFocus) = 0 Then
'        gCmn.MyMsgBox Me, "Fail to run ETS End Of Day Manager application. ('" & sPath & "')", vbCritical
'    End If
End Sub

Private Sub DeleteOldOrders()
    On Error GoTo EH
    gDBW.usp_FixOldOrder_Del
    Exit Sub
EH:
    ShowError
End Sub

Private Sub DeleteOldCards()
    On Error GoTo EH
    gDBW.usp_TntCardOld_Del
    Exit Sub
EH:
    ShowError
End Sub


Private Sub mnuVolaManager_Click()
    On Error Resume Next
    RunVolaManager
End Sub
Public Sub RunVolaManager(Optional ByVal sSymbol As String = "")
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\VolaManager.exe"
    If Shell(sPath & IIf(Len(sSymbol) > 0, " " & sSymbol, ""), vbNormalFocus) = 0 Then
        gCmn.MyMsgBox "Fail to run Volatility Manager application. ('" & sPath & "')", vbCritical
    End If
End Sub

Public Sub RunEtsMM()
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\EtsMm.exe"
    If Shell(sPath, vbNormalFocus) = 0 Then
        gCmn.MyMsgBox "Fail to run " & AppIvRmTitle() & " application. ('" & sPath & "')", vbCritical
    End If
End Sub

Public Sub RunEtsMMCv()
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\EtsMmCv.exe"
    If Shell(sPath, vbNormalFocus) = 0 Then
        gCmn.MyMsgBox "Fail to run " & AppIvRmTitle() & " application. ('" & sPath & "')", vbCritical
    End If
End Sub

