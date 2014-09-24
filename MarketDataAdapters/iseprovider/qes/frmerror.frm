VERSION 5.00
Begin VB.Form frmError 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Error"
   ClientHeight    =   1980
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5700
   ClipControls    =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1366.631
   ScaleMode       =   0  'User
   ScaleWidth      =   5352.595
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdClose 
      Caption         =   "Close"
      Height          =   375
      Left            =   2040
      TabIndex        =   0
      Top             =   1560
      Width           =   1695
   End
   Begin VB.Label labText 
      Height          =   1215
      Left            =   240
      TabIndex        =   1
      Top             =   120
      Width           =   5175
   End
End
Attribute VB_Name = "frmError"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
    Private Sub cmdClose_Click()
    Unload Me
End Sub
