VERSION 5.00
Object = "{03B609FD-3535-456B-986E-3D4A52D9D05C}#9.0#0"; "DHlButtons.ocx"
Begin VB.UserControl ctlIndicator 
   BackColor       =   &H00000000&
   ClientHeight    =   540
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3210
   ScaleHeight     =   540
   ScaleWidth      =   3210
   Begin DHlButtons.btnCancel btnCancel1 
      Height          =   255
      Left            =   840
      TabIndex        =   1
      Top             =   240
      Width           =   255
      _ExtentX        =   450
      _ExtentY        =   450
   End
   Begin EgarHedgerSQL.ctlProgress ctlProgress1 
      Height          =   195
      Left            =   1200
      TabIndex        =   0
      Top             =   240
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   344
   End
   Begin VB.Label lblCaption 
      BackColor       =   &H00000000&
      Caption         =   "Loading..."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   840
      TabIndex        =   3
      Top             =   0
      Width           =   2415
   End
   Begin VB.Label lblProgress 
      BackColor       =   &H00000000&
      Caption         =   "Progress"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C0C0&
      Height          =   255
      Left            =   0
      TabIndex        =   2
      Top             =   240
      Width           =   735
   End
End
Attribute VB_Name = "ctlIndicator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Event CancelClick()

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
    On Error Resume Next
    With lblCaption
        .Caption = Caption
        .Visible = Len(Caption) > 0
    End With
    btnCancel1.Enabled = CanCancel
    With ctlProgress1
        .Visible = True
        .StartProgress
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
    On Error Resume Next
    ctlProgress1.StopProgress
    With lblCaption
        .Caption = Caption
        .Visible = Len(Caption) > 0
    End With
    btnCancel1.Enabled = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel1_Click()
    RaiseEvent CancelClick
End Sub

