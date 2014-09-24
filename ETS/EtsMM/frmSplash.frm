VERSION 5.00
Begin VB.Form frmSplash 
   BorderStyle     =   0  'None
   Caption         =   "ETS"
   ClientHeight    =   3645
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   7860
   ControlBox      =   0   'False
   Icon            =   "frmSplash.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   MousePointer    =   11  'Hourglass
   Moveable        =   0   'False
   ScaleHeight     =   3645
   ScaleWidth      =   7860
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrPerformance 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   0
      Top             =   0
   End
   Begin VB.PictureBox picMain 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   2685
      Left            =   0
      MousePointer    =   11  'Hourglass
      ScaleHeight     =   2655
      ScaleWidth      =   6300
      TabIndex        =   0
      Top             =   0
      Width           =   6330
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
         ForeColor       =   &H00000000&
         Height          =   255
         Left            =   0
         MousePointer    =   11  'Hourglass
         TabIndex        =   1
         Top             =   840
         Width           =   3615
      End
   End
End
Attribute VB_Name = "frmSplash"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private WithEvents m_Main As EtsGeneralLib.EtsMain
Attribute m_Main.VB_VarHelpID = -1

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Set m_Main = g_Main
    
    Me.Width = picMain.Width
    Me.Height = picMain.Height

    Me.Caption = AppTitle() & ", v" & App.Major & "." & App.Minor & "." & App.Revision
    
    Set picMain.Picture = g_Params.Splash
    Me.Top = picMain.Top
    Me.Left = picMain.Left
    Me.Height = picMain.Height
    Me.Width = picMain.Width
    
    lblStatus.Left = Me.Left
    lblStatus.Width = Me.Width
    lblStatus.Top = Me.Top + Me.Height - lblStatus.Height
    
        
    'lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
End Sub

Private Sub m_Main_Progress(ByVal bsMessage As String, ByVal lProgress As Long)
    If lProgress >= 0 Then
        lblStatus.Caption = bsMessage & " " & CStr(lProgress) & "% complete."
    Else
        lblStatus.Caption = bsMessage
    End If
    lblStatus.Refresh
End Sub

Private Sub tmrPerformance_Timer()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogPerformanceCounters
End Sub

Public Sub SetStatus(ByVal bsMessage As String)
    On Error Resume Next
    lblStatus.Caption = bsMessage
    lblStatus.Refresh
End Sub
