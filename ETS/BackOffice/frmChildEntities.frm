VERSION 5.00
Begin VB.Form frmChildEntities 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   0  'None
   Caption         =   "Entities"
   ClientHeight    =   3705
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   5250
   ControlBox      =   0   'False
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   NegotiateMenus  =   0   'False
   ScaleHeight     =   3705
   ScaleWidth      =   5250
   ShowInTaskbar   =   0   'False
   Tag             =   "5"
   Begin ElladaBO.ctlEntities ctlEntities1 
      Height          =   2415
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3375
      _ExtentX        =   5953
      _ExtentY        =   4260
   End
End
Attribute VB_Name = "frmChildEntities"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Function CanLeaveFocus() As Boolean
    On Error Resume Next
    CanLeaveFocus = ctlEntities1.EntityCanChangeRow
End Function

Private Sub Form_Load()
    On Error GoTo Herr
    Me.Visible = False
    With ctlEntities1
        .Init
        .EntityType = ENT_GROUPS
    End With
    Exit Sub
Herr:
    ShowError
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    With ctlEntities1
        .Move 0, 0, ScaleWidth - .Left, ScaleHeight - .Top
    End With
End Sub

Private Sub Form_Activate()
    On Error GoTo Herr
    frmMain.SelectActiveChild
    Exit Sub
Herr:
    ShowError
End Sub

Public Sub RefreshTraders()
    On Error Resume Next
    ctlEntities1.RefreshTraders
End Sub

