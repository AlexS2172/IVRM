VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMyInputBox 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Input"
   ClientHeight    =   1500
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4845
   Icon            =   "frmMyInputBox.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1500
   ScaleWidth      =   4845
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtInput 
      Appearance      =   0  'Flat
      Height          =   285
      Left            =   120
      TabIndex        =   0
      Top             =   1080
      Width           =   4575
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Default         =   -1  'True
      Height          =   300
      Index           =   0
      Left            =   3600
      TabIndex        =   1
      Top             =   120
      Width           =   1095
      _ExtentX        =   1931
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
      Caption         =   "OK"
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Cancel          =   -1  'True
      Height          =   300
      Index           =   1
      Left            =   3600
      TabIndex        =   2
      Top             =   480
      Width           =   1095
      _ExtentX        =   1931
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
      Caption         =   "Cancel"
   End
   Begin VB.Label lblPrompt 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "lblPrompt"
      ForeColor       =   &H80000008&
      Height          =   300
      Left            =   120
      TabIndex        =   3
      Top             =   720
      Width           =   3375
      WordWrap        =   -1  'True
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   0
      Left            =   120
      Picture         =   "frmMyInputBox.frx":000C
      Top             =   120
      Width           =   480
   End
End
Attribute VB_Name = "frmMyInputBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

' message bx parameters
Private msPrompt As String
Private msTitle As String
Private msDefault As String
Private mXPos As Single
Private mYPos As Single
Private msHelpFile As String
Private miContext As String
Private miInputBoxResult As String

' button indexes
Private Const BTN_OK = 0
Private Const BTN_CANCEL = 1

' executing the form
Public Function Execute( _
    ByVal sPrompt As String, _
    Optional ByVal sTitle As String = APP_TITLE, _
    Optional ByVal sDefault As String, _
    Optional ByVal XPos As Single, _
    Optional ByVal YPos As Single, _
    Optional ByVal sHelpFile As String, _
    Optional ByVal iContext As Long = 0 _
) As String
    
    On Error GoTo Herr
    msPrompt = sPrompt
    msTitle = IIf(sTitle > "", sTitle, App.Title)
    msDefault = sDefault
    mXPos = XPos
    mYPos = YPos
    msHelpFile = sHelpFile
    miContext = iContext
    
    miInputBoxResult = ""
    Load Me
    Init
    Me.Show vbModal
    Unload Me
    Execute = miInputBoxResult
    Exit Function
Herr:
    ShowError
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Init()
    On Error Resume Next
    Me.Caption = msTitle
    lblPrompt = msPrompt
    txtInput = msDefault
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnButton_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case BTN_OK
        miInputBoxResult = txtInput
    Case BTN_CANCEL
        miInputBoxResult = ""
    End Select
    Me.Hide
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
Private Sub txtInput_GotFocus()
    On Error Resume Next
    With txtInput
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

