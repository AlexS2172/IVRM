VERSION 5.00
Begin VB.UserControl ctlTopPanel2 
   BackColor       =   &H00000000&
   ClientHeight    =   810
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9060
   ScaleHeight     =   810
   ScaleWidth      =   9060
   Begin VB.PictureBox picTopLeft 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   120
      Picture         =   "ctlTopPanel2.ctx":0000
      ScaleHeight     =   615
      ScaleWidth      =   2700
      TabIndex        =   0
      Top             =   60
      Width           =   2700
   End
   Begin EgarHedgerSQL.ctlIndicator ctlIndicator1 
      Height          =   615
      Left            =   5760
      TabIndex        =   1
      Top             =   120
      Width           =   3135
      _ExtentX        =   5530
      _ExtentY        =   1085
   End
End
Attribute VB_Name = "ctlTopPanel2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const BTN_FIRST = 0

Public Enum BTN_TOP_PANEL2
    BTN_TOP_PANEL2_CLOSE = BTN_FIRST
    BTN_TOP_PANEL2_LOAD_PRICES
    BTN_TOP_PANEL2_LOAD_IV
    BTN_TOP_PANEL2_REFRESH
    BTN_TOP_PANEL2_COUNT
End Enum

Private Const BTN_LAST = BTN_TOP_PANEL2_COUNT - 1

Public Event ButtonClick(ByVal Index As BTN_TOP_PANEL2)
Public Event CancelClick()
Public Busy As Boolean

Private Const Border = 60

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlIndicator1_CancelClick()
    On Error Resume Next
    RaiseEvent CancelClick
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
Dim i As Long
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Busy = False
    With ctlIndicator1
        .StopTask ""
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    With ctlIndicator1
        .Left = Max(3120, ScaleWidth - .Width - Border)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
Dim i As Long
    On Error Resume Next
    Busy = True
    With ctlIndicator1
        .StartTask Caption, CanCancel
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
Dim i As Long
    On Error Resume Next
    Busy = False
    With ctlIndicator1
        .StopTask Caption
    End With
End Sub

