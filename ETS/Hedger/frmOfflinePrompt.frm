VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmOfflinePrompt 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Egar Hedger"
   ClientHeight    =   1395
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4980
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmOfflinePrompt.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1395
   ScaleWidth      =   4980
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin ElladaFlatControls.FlatButton btnButton 
      Height          =   300
      Index           =   0
      Left            =   1080
      TabIndex        =   0
      Top             =   960
      Width           =   1275
      _ExtentX        =   2249
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "  Work &offline"
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Height          =   300
      Index           =   1
      Left            =   2640
      TabIndex        =   1
      Top             =   960
      Width           =   1275
      _ExtentX        =   2249
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "  &Settings"
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Cancel          =   -1  'True
      Default         =   -1  'True
      Height          =   300
      Index           =   2
      Left            =   3480
      TabIndex        =   2
      Top             =   360
      Visible         =   0   'False
      Width           =   1275
      _ExtentX        =   2249
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "  &Close"
   End
   Begin VB.Label lblPrompt 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "lblPrompt"
      ForeColor       =   &H80000008&
      Height          =   675
      Left            =   840
      TabIndex        =   3
      Top             =   120
      Width           =   3735
      WordWrap        =   -1  'True
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   3
      Left            =   120
      Picture         =   "frmOfflinePrompt.frx":000C
      Top             =   120
      Width           =   480
   End
End
Attribute VB_Name = "frmOfflinePrompt"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private m_mbResult As VbMsgBoxResult

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByRef frmOwner As Form, ByVal sPrompt As String) As VbMsgBoxResult
    On Error Resume Next
    Load Me
    m_mbResult = vbCancel
    lblPrompt.Caption = sPrompt
    Show vbModal, frmOwner
    Unload Me
    Execute = m_mbResult
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnButton_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case 0 ' offline
        m_mbResult = vbIgnore
    Case 1 ' settings
        m_mbResult = vbOK
    Case 2 ' close
        m_mbResult = vbCancel
    End Select
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    With New clsCommon
        .CustomizeForm Me
    End With
End Sub

