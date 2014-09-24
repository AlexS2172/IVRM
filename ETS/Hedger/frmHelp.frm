VERSION 5.00
Begin VB.Form frmHelp 
   Appearance      =   0  'Flat
   BackColor       =   &H80000018&
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Help"
   ClientHeight    =   1395
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   4680
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1395
   ScaleWidth      =   4680
   ShowInTaskbar   =   0   'False
   Begin VB.Label lblHelp 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000018&
      BackStyle       =   0  'Transparent
      Caption         =   "Help"
      ForeColor       =   &H80000008&
      Height          =   195
      Left            =   90
      TabIndex        =   0
      Top             =   60
      UseMnemonic     =   0   'False
      Width           =   4515
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmHelp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If KeyCode = vbKeyEscape Then
        CloseMe
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    g_lHelpTop = MyGetSettingLong("HelpTop", 0)
    g_lHelpLeft = MyGetSettingLong("HelpLeft", 0)
    g_lHelpWidth = MyGetSettingLong("HelpWidth", 0)
    g_lHelpHeight = MyGetSettingLong("HelpHeight", 0)
    g_bShowHelpTags = MyGetSettingBool("ShowHelpTags", g_bShowHelpTags)
    If g_lHelpWidth > 0 And g_lHelpHeight > 0 Then
        If g_lHelpLeft < 0 Then g_lHelpLeft = 0
        If g_lHelpTop < 0 Then g_lHelpTop = 0
        If g_lHelpLeft > Screen.Width - 300 Then g_lHelpLeft = Screen.Width - 300
        If g_lHelpTop > Screen.Height - 300 Then g_lHelpTop = Screen.Height - 300
        Me.Left = g_lHelpLeft
        Me.Top = g_lHelpTop
        Me.Width = g_lHelpWidth
        'me.Height = g_lHelpHeight
    Else
        Me.Left = (Screen.Width - Me.Width) / 2
        Me.Top = (Screen.Height - Me.Height) / 2
    End If
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = (UnloadMode = vbFormControlMenu)
    CloseMe
End Sub

Public Sub CloseMe()
    On Error Resume Next
    g_lHelpLeft = Me.Left
    g_lHelpTop = Me.Top
    g_lHelpWidth = Me.Width
    g_lHelpHeight = Me.Height
    
    MySaveSettingLong "HelpTop", g_lHelpTop
    MySaveSettingLong "HelpLeft", g_lHelpLeft
    MySaveSettingLong "HelpWidth", g_lHelpWidth
    MySaveSettingLong "HelpHeight", g_lHelpHeight
    MySaveSettingBool "ShowHelpTags", g_bShowHelpTags
    
    Hide
End Sub

