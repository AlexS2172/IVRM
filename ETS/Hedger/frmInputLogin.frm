VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmInputLogin 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Login"
   ClientHeight    =   1890
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3555
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmInputLogin.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1890
   ScaleWidth      =   3555
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      IMEMode         =   3  'DISABLE
      Index           =   2
      Left            =   1200
      PasswordChar    =   "*"
      TabIndex        =   2
      Text            =   "Value"
      Top             =   960
      Width           =   2040
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      IMEMode         =   3  'DISABLE
      Index           =   1
      Left            =   1200
      PasswordChar    =   "*"
      TabIndex        =   1
      Text            =   "Value"
      Top             =   600
      Width           =   2040
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   0
      Left            =   1200
      TabIndex        =   0
      Text            =   "Value"
      Top             =   240
      Width           =   2040
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Default         =   -1  'True
      Height          =   300
      Left            =   360
      TabIndex        =   3
      Top             =   1440
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
      Caption         =   "   &OK"
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   1920
      TabIndex        =   4
      Top             =   1440
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
      Caption         =   "   &Cancel"
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Confirm :"
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   7
      Top             =   990
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Password :"
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   6
      Top             =   630
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Login :"
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   5
      Top             =   270
      Width           =   960
   End
End
Attribute VB_Name = "frmInputLogin"
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
Public Function Execute(ByRef frmOwner As Form, ByVal sCaption As String, ByRef sLogin As String, _
                        ByRef sPassword As String) As VbMsgBoxResult
    On Error Resume Next
    Load Me
    
    m_mbResult = vbCancel
    
    Caption = sCaption
    txtValue(0).Text = sLogin
    txtValue(1).Text = sPassword
    txtValue(2).Text = sPassword
    
    gCmn.SelectText txtValue(0)
    
    Show vbModal, frmOwner
    
    If m_mbResult = vbOK Then
        sLogin = txtValue(0).Text
        sPassword = txtValue(1).Text
    End If
    
    Unload Me
    Execute = m_mbResult
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    m_mbResult = vbCancel
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    If txtValue(1).Text <> txtValue(2).Text Then
        gCmn.MyMsgBox "Password and confirmation do not match!", vbCritical
    Else
        m_mbResult = vbOK
        Hide
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    With New clsCommon
        .CustomizeForm Me
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtValue_GotFocus(Index As Integer)
    On Error Resume Next
    With txtValue(Index)
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

