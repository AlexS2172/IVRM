VERSION 5.00
Begin VB.Form frmSplash 
   BorderStyle     =   0  'None
   Caption         =   "ETS"
   ClientHeight    =   5310
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   7005
   ControlBox      =   0   'False
   Icon            =   "frmSplash.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   MousePointer    =   11  'Hourglass
   Moveable        =   0   'False
   ScaleHeight     =   5310
   ScaleWidth      =   7005
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrPerformance 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   6480
      Top             =   4440
   End
   Begin VB.PictureBox picMain 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   4980
      Left            =   0
      MousePointer    =   11  'Hourglass
      Picture         =   "frmSplash.frx":0442
      ScaleHeight     =   4950
      ScaleWidth      =   6840
      TabIndex        =   0
      Top             =   0
      Width           =   6870
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Initialization..."
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   60
         MousePointer    =   11  'Hourglass
         TabIndex        =   2
         Top             =   4740
         Width           =   6735
      End
      Begin VB.Label lblVersion 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Demo Version"
         BeginProperty Font 
            Name            =   "Tahoma"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FFFFFF&
         Height          =   255
         Left            =   4365
         MousePointer    =   11  'Hourglass
         TabIndex        =   1
         Top             =   3600
         Width           =   2415
      End
   End
End
Attribute VB_Name = "frmSplash"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Me.Width = picMain.Width
    Me.Height = picMain.Height

    Me.Caption = APP_TITLE & ", v" & App.Major & "." & App.Minor & "." & App.Revision
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
End Sub

Private Sub tmrPerformance_Timer()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
    	g_PerformanceLog.LogPerformanceCounters
End Sub
