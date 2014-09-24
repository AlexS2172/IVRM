VERSION 5.00
Object = "{E395ECF2-0B93-464D-AD1B-99A443BDF17F}#1.0#0"; "FocusFlatControls1.ocx"
Begin VB.Form frmDataLog 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Data load"
   ClientHeight    =   3015
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   6600
   Icon            =   "frmDataLog.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3015
   ScaleWidth      =   6600
   ShowInTaskbar   =   0   'False
   Begin FocusFlatControls1.FlatPrograssBar ctlProgress 
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   480
      Width           =   6375
      _ExtentX        =   11245
      _ExtentY        =   450
      BackColor       =   -2147483643
      Value           =   0
      ForeColor1      =   12632256
      ForeColor2      =   49152
   End
   Begin VB.PictureBox Picture1 
      Align           =   2  'Align Bottom
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   0
      ScaleHeight     =   495
      ScaleWidth      =   6600
      TabIndex        =   1
      Top             =   2520
      Width           =   6600
      Begin FocusFlatControls1.FlatButton btnCancel 
         Height          =   315
         Left            =   4680
         TabIndex        =   2
         Top             =   120
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   556
         Enabled         =   0   'False
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   " &Cancel"
      End
      Begin FocusFlatControls1.FlatButton btnOK 
         Height          =   315
         Left            =   3480
         TabIndex        =   3
         Top             =   120
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   556
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   " &Hide"
      End
   End
   Begin VB.TextBox txtLog 
      Appearance      =   0  'Flat
      Height          =   1695
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   840
      Width           =   6375
   End
   Begin VB.Label lblStatus 
      Caption         =   "Status"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   240
      Width           =   6375
   End
End
Attribute VB_Name = "frmDataLog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Private g_Top As Long
Private g_Left As Long
Private g_Width As Long
Private g_Height As Long
Private Const Border = 60

Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHistory()
    If Not gRuntime Then Exit Sub
    Show , frmMain
'    btnOK.Enabled = True
'    btnCancel.Enabled = False
    With Me.txtLog
        .SelStart = Len(.Text)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowLog()
    If Not gRuntime Then Exit Sub
    Show , frmMain
    btnOk.Enabled = False
    btnCancel.Enabled = True
    With Me.txtLog
        .SelStart = Len(.Text)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    gCmn.CustomizeForm Me
    With Me.txtLog
        .MaxLength = 0
        .Text = LoadStringFromFile(LogFile)
        .SelStart = Len(.Text)
    End With
    g_Top = MyGetSettingLong("LoadTop", Max(0, frmMain.Top + frmMain.Height - 2500))
    g_Left = MyGetSettingLong("LoadLeft", Max(0, frmMain.Left + frmMain.Width - 7000))
    g_Width = MyGetSettingLong("LoadWidth", 7000)
    g_Height = MyGetSettingLong("LoadHeight", 2500)
    Me.Width = Max(1000, g_Width)
    Me.Height = Max(300, g_Height)
    If g_Top = 0 And g_Top = 0 Then
        Me.Left = Max(0, (Screen.Width - Me.Width) / 2)
        Me.Top = Max(0, (Screen.Height - Me.Height) / 2)
    Else
        Me.Left = Max(0, g_Left)
        Me.Top = Max(0, g_Top)
    End If
    lblStatus.Caption = "Status"
    Set PortfolioHedge = gPortfolioHedge
    ctlProgress.Value = 0
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If Me.WindowState = vbNormal Then
        g_Left = Me.Left
        g_Top = Me.Top
        g_Width = Me.Width
        g_Height = Me.Height
    End If
    If UnloadMode = vbFormControlMenu Then
        If btnOk.Enabled = False Then
            Cancel = True
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set PortfolioHedge = Nothing
    MySaveSettingLong "LoadTop", g_Top
    MySaveSettingLong "LoadLeft", g_Left
    MySaveSettingLong "LoadWidth", g_Width
    MySaveSettingLong "LoadHeight", g_Height
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Resize()
Dim X As Single
    On Error Resume Next
    X = Max(0, Me.ScaleWidth - 2 * Border)
    With lblStatus
        .Left = Border
        .Width = X
    End With
    With ctlProgress
        .Left = Border
        .Width = X
    End With
    With Me.txtLog
        .Left = Border
        .Width = X
        .Height = Max(0, Me.ScaleHeight - Picture1.Height - .Top - Border)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Picture1_Resize()
    On Error Resume Next
    With btnCancel
        .Left = Max(500, Me.ScaleWidth - .Width - 60)
    End With
    With btnOk
        .Left = Max(60, btnCancel.Left - .Width - 60)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    On Error Resume Next
    If Len(Caption) > 0 Then
        Me.Caption = Caption
        lblStatus.Caption = Caption
    Else
        Me.Caption = "Data load"
        lblStatus.Caption = "Status"
    End If
    If CanCancel Then
        btnOk.Enabled = False
        btnCancel.Enabled = True
    Else
        btnOk.Enabled = True
        btnCancel.Enabled = False
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    On Error Resume Next
    PortfolioHedge_GotStartTask Caption, False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_LogMessage(ByVal Message As String)
    On Error Resume Next
    With Me.txtLog
        .SelStart = Len(.Text)
        .Text = .Text & Message & vbCrLf
        .SelStart = Len(.Text)
    End With
    DoEvents
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    PortfolioHedge.CancelTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
'    Me.Hide
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_Progress(ByVal ProgressPercent As Long)
    On Error Resume Next
    With ctlProgress
        .Value = ProgressPercent
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ProgressStart()
    On Error Resume Next
    With ctlProgress
        .Min = 0
        .Max = 100
        .Value = 0
'        .ForeColor1 = &H94D7F7
'        .ForeColor2 = &H104194
        .ForeColor1 = &HC0C0C0
        .ForeColor2 = &HC00000
        .Visible = True
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ProgressStop()
    On Error Resume Next
    With ctlProgress
        .Value = .Max
        .ForeColor1 = &HC0C0C0
        .ForeColor2 = &H808080
'        .Visible = False
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_Status(ByVal Message As String)
    On Error Resume Next
    lblStatus.Caption = Message
End Sub

