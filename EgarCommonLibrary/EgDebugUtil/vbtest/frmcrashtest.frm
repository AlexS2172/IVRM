VERSION 5.00
Begin VB.Form frmCrashTest 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Crash Test"
   ClientHeight    =   1980
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3030
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1980
   ScaleWidth      =   3030
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnDoCrash 
      Caption         =   "Do The Crash!"
      Height          =   735
      Left            =   240
      TabIndex        =   1
      Top             =   960
      Width           =   2535
   End
   Begin VB.CommandButton btnSetCrashHandlerCallback 
      Caption         =   "SetCrashHandlerCallback"
      Height          =   735
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   2535
   End
End
Attribute VB_Name = "frmCrashTest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Sub DoCrash Lib "CrashDLL" ()

Private Sub btnDoCrash_Click()
    DoCrash
End Sub

Private Sub btnSetCrashHandlerCallback_Click()
    ' Set the callback.
    SetMiniDumpDefaultCrashHandler
    ' Disable this button.
    btnSetCrashHandlerCallback.Enabled = False
End Sub
