VERSION 5.00
Begin VB.Form frmWarning 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Warning"
   ClientHeight    =   1425
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5280
   Icon            =   "frmWarning.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1425
   ScaleWidth      =   5280
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   375
      Left            =   1920
      TabIndex        =   0
      Top             =   960
      Width           =   1335
   End
   Begin VB.Label lblWarning 
      Alignment       =   2  'Center
      Caption         =   "Label1"
      Height          =   615
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   5055
   End
End
Attribute VB_Name = "frmWarning"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub btnOK_Click()
    Unload Me
End Sub

