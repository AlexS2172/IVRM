VERSION 5.00
Begin VB.Form frmSplash 
   BorderStyle     =   0  'None
   Caption         =   "EtsBO"
   ClientHeight    =   10725
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
   ScaleHeight     =   10725
   ScaleWidth      =   7005
   StartUpPosition =   2  'CenterScreen
   Begin VB.PictureBox picMain 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   1125
      Left            =   0
      MousePointer    =   11  'Hourglass
      ScaleHeight     =   1095
      ScaleWidth      =   3660
      TabIndex        =   0
      Top             =   0
      Width           =   3690
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
         Width           =   3495
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
End Sub

