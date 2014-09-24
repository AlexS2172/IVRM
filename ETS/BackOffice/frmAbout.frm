VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmAbout 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
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
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtProductInfo 
      Appearance      =   0  'Flat
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
   Begin ElladaFlatControls.FlatButton btnClose 
      Cancel          =   -1  'True
      Default         =   -1  'True
      Height          =   285
      Left            =   4200
      TabIndex        =   6
      Top             =   6000
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "OK"
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
      Caption         =   "Copyright © 2001-2003, EGAR Technology Inc."
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

Private Sub btnClose_Click()
    On Error Resume Next
    ' close dialog
    Unload Me
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    Set imgIcon.Picture = g_Params.Icon(WND_MAIN)

    ' set About dlg title & App info
    Me.Caption = "About " & AppLongTitle()
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = AppLongTitle()
    lblDescription.Caption = AppDescription() ' "Volatility Dispersion Trading Analytic tools"
    txtProductInfo.Text = MakeAppComponentsDescription 'MakeComponentsDesc
    lblCopyright.Caption = AppCopyright()
    
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
    ShowWarning "System Information is unavailable at this time."
End Sub

