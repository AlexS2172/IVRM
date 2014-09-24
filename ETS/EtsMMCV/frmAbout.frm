VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About MyApp"
   ClientHeight    =   6435
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5760
   ClipControls    =   0   'False
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6435
   ScaleWidth      =   5760
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtProductInfo 
      Height          =   3615
      Left            =   1080
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Tag             =   "noauto"
      Top             =   2040
      Width           =   4455
   End
   Begin VB.CommandButton btnClose 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   4200
      TabIndex        =   6
      Top             =   6000
      Width           =   1335
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000014&
      BorderStyle     =   6  'Inside Solid
      Index           =   1
      X1              =   120
      X2              =   5684
      Y1              =   5820
      Y2              =   5820
   End
   Begin VB.Image imgIcon 
      Appearance      =   0  'Flat
      Height          =   480
      Left            =   240
      Top             =   120
      Width           =   480
   End
   Begin VB.Label lblCopyright 
      BackStyle       =   0  'Transparent
      Caption         =   "Copyright © 2001-2005, EGAR Technology Inc."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Left            =   1080
      TabIndex        =   5
      Top             =   1440
      Width           =   4485
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000010&
      BorderStyle     =   6  'Inside Solid
      Index           =   0
      X1              =   120
      X2              =   5684
      Y1              =   5805
      Y2              =   5805
   End
   Begin VB.Label lblDescription 
      BackStyle       =   0  'Transparent
      Caption         =   "Description"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   450
      Left            =   1080
      TabIndex        =   4
      Top             =   840
      Width           =   4485
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Application Title"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   360
      Left            =   1080
      TabIndex        =   3
      Top             =   120
      Width           =   4485
   End
   Begin VB.Label lblVersion 
      BackStyle       =   0  'Transparent
      Caption         =   "Version"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Left            =   1080
      TabIndex        =   2
      Top             =   480
      Width           =   4485
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Product Info :"
      Height          =   225
      Left            =   1080
      TabIndex        =   1
      Top             =   1800
      Width           =   4485
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' **************************************************************
' Module            frmAbout
'
' Filename          frmAbout.frm
'
' Author            Oleg Sharkevich
'                   eGar Technology Inc.
'
' Description
'
' About Dialog form module
'
' **************************************************************

Option Explicit

' registry keys consts for MSInfo app
Private Const REGKEY_SYSINFO_LOC = "SOFTWARE\Microsoft\Shared Tools Location"
Private Const REGVAL_SYSINFO_LOC = "MSINFO"
Private Const REGKEY_SYSINFO = "SOFTWARE\Microsoft\Shared Tools\MSINFO"
Private Const REGVAL_SYSINFO = "PATH"

Private m_frmOwner As Form
Private m_bIsOpened As Boolean

Public Sub Execute(ByRef frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then
        Me.SetFocus
        Exit Sub
    End If
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    Load Me
    Me.Show vbModeless, m_frmOwner
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open format dialog."
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    ' close dialog
    Unload Me
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
    Set imgIcon.Picture = frmMain.Icon
    ' set About dlg title & App info
    Me.Caption = "About " & APP_TITLE
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = APP_TITLE
    lblDescription.Caption = App.FileDescription
    txtProductInfo.Text = MakeLicenseDesc & vbCrLf & MakeAppComponentsDescription(True)
    
    Dim nTabStops(0 To 1) As Long
    nTabStops(0) = 80
    nTabStops(1) = 120
    SendMessage txtProductInfo.hWnd, EM_SETTABSTOPS, 2, nTabStops(0)
End Sub

' start sysinfo application loading
Public Sub StartSysInfo()
    On Error GoTo SysInfoErr
  
    Dim sSysInfoPath As String
    
    ' Try To Get System Info Program Path\Name From Registry...
    GetStrKeyValue HKEY_LOCAL_MACHINE, REGKEY_SYSINFO, REGVAL_SYSINFO, sSysInfoPath
    If sSysInfoPath = "" Then
        ' Try To Get System Info Program Path Only From Registry...
        GetStrKeyValue HKEY_LOCAL_MACHINE, REGKEY_SYSINFO_LOC, REGVAL_SYSINFO_LOC, sSysInfoPath
        If sSysInfoPath <> "" Then
            ' Validate Existance Of Known 32 Bit File lblVersion
            If (Dir$(sSysInfoPath & "\MSINFO32.EXE") <> "") Then
                sSysInfoPath = sSysInfoPath & "\MSINFO32.EXE"
                
            ' Error - File Can Not Be Found...
            Else
                GoTo SysInfoErr
            End If
        ' Error - Registry Entry Can Not Be Found...
        Else
            GoTo SysInfoErr
        End If
    End If
    
    ' open MSInfo app
    Shell sSysInfoPath, vbNormalFocus
    
    Exit Sub
SysInfoErr:
    ' notify error
    gCmn.MyMsgBox Me, "System Information is unavailable at this time.", vbOKOnly + vbInformation
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

