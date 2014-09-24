VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMyMsgBox 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Form1"
   ClientHeight    =   1380
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5400
   Icon            =   "frmMyMsgBox.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1380
   ScaleWidth      =   5400
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Visible         =   0   'False
   Begin ElladaFlatControls.FlatButton btnButton 
      Height          =   300
      Index           =   1
      Left            =   600
      TabIndex        =   2
      Top             =   960
      Visible         =   0   'False
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
      Height          =   300
      Index           =   2
      Left            =   1440
      TabIndex        =   0
      Top             =   960
      Visible         =   0   'False
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
      Caption         =   "Yes"
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Height          =   300
      Index           =   3
      Left            =   2400
      TabIndex        =   1
      Top             =   960
      Visible         =   0   'False
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
      Caption         =   "No"
   End
   Begin ElladaFlatControls.FlatButton btnButton 
      Height          =   300
      Index           =   4
      Left            =   3240
      TabIndex        =   3
      Top             =   960
      Visible         =   0   'False
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
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   4
      Left            =   1440
      Picture         =   "frmMyMsgBox.frx":000C
      Top             =   600
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   3
      Left            =   960
      Picture         =   "frmMyMsgBox.frx":044E
      Top             =   600
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   2
      Left            =   480
      Picture         =   "frmMyMsgBox.frx":0890
      Top             =   600
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   1
      Left            =   0
      Picture         =   "frmMyMsgBox.frx":0CD2
      Top             =   600
      Visible         =   0   'False
      Width           =   480
   End
   Begin VB.Image imgDisplayIcon 
      Height          =   495
      Left            =   120
      Top             =   120
      Width           =   495
   End
   Begin VB.Label lblPrompt 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "lblPrompt"
      ForeColor       =   &H80000008&
      Height          =   675
      Left            =   720
      TabIndex        =   4
      Top             =   120
      Width           =   4575
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmMyMsgBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'**************************************************
' Custom message form
' Created by Andrei Shchurov (shurov@egartech.com)
' February 20, 2001
'**************************************************

' message box result
Private miMsgBoxResult As VbMsgBoxResult

' message bx parameters
Private msPrompt As String
Private miButtons As VbMsgBoxStyle
Private msTitle As String
Private msHelpFile As String
Private miContext As String

Private miTempMouse As Long

' button indexes
Private Const BTN_OK = 1
Private Const BTN_YES = 2
Private Const BTN_NO = 3
Private Const BTN_CANCEL = 4

' icon indexes
Private Const ICN_CRITICAL = 1
Private Const ICN_QUESTION = 2
Private Const ICN_EXCLAMATION = 3
Private Const ICN_INFORMATIN = 4


' button positions
Private Const INT_SPACE = 120

Private marrBtns() As Long
Private miBtnCnt As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' executing the form
'
Public Function Execute( _
    ByVal sPrompt, _
    Optional ByVal iButtons As VbMsgBoxStyle = vbOKOnly + vbInformation, _
    Optional ByVal sTitle As String = "", _
    Optional ByVal sHelpFile As String = "", _
    Optional ByVal iContext As Long = 0 _
) As VbMsgBoxResult
    
    On Error GoTo ErrHnd

    miTempMouse = Screen.MousePointer
    Screen.MousePointer = vbDefault
    
    miMsgBoxResult = 0
    
    msPrompt = sPrompt
    miButtons = iButtons
    msTitle = IIf(sTitle > "", sTitle, App.Title)
    msHelpFile = sHelpFile
    miContext = iContext
    
    Load Me
    Init
    Me.Show vbModal
    
    Screen.MousePointer = miTempMouse
    
    Unload Me
    
    Execute = miMsgBoxResult
    Exit Function
ErrHnd:
    Execute = MsgBox(sPrompt, iButtons, sTitle, sHelpFile, iContext)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' initialization
'
Private Sub Init()
Dim fBtnWdth As Single
Dim fLeftBtnPos As Single
Dim i As Long
Dim iH As Long
Dim iW As Long
    Me.Caption = msTitle
    
    lblPrompt = msPrompt
    
    iH = lblPrompt.Top + lblPrompt.Height + 900
    If iH > Screen.Height - 600 Then iH = Screen.Height - 600
    If Me.Height < iH Then Me.Height = iH
    
    iW = lblPrompt.Left + lblPrompt.Width + 120
    If iW > Screen.Width - 600 Then iW = Screen.Width - 600
    If Me.Width < iW Then Me.Width = iW
    
    Me.Top = (Screen.Height - Me.Height) / 2
    Me.Left = (Screen.Width - Me.Width) / 2
    
    
    ' buttons initialization
    For i = 1 To 4
        btnButton(i).Visible = False
    Next i
    
    ReDim marrBtns(0)
    Select Case miButtons And &HF
    Case vbOKCancel
        ReDim marrBtns(2)
        marrBtns(1) = BTN_OK
        marrBtns(2) = BTN_CANCEL
        miMsgBoxResult = vbCancel
    Case vbYesNo
        ReDim marrBtns(2)
        marrBtns(1) = BTN_YES
        marrBtns(2) = BTN_NO
        miMsgBoxResult = vbNo
    Case vbYesNoCancel
        ReDim marrBtns(3)
        marrBtns(1) = BTN_YES
        marrBtns(2) = BTN_NO
        marrBtns(3) = BTN_CANCEL
        miMsgBoxResult = vbCancel
    Case Else
        ReDim marrBtns(1)
        marrBtns(1) = BTN_OK
        miMsgBoxResult = vbOK
    End Select
    
    ' positioning buttons
    miBtnCnt = UBound(marrBtns)
    fBtnWdth = btnButton(1).Width
    fLeftBtnPos = (Me.ScaleWidth - (fBtnWdth * miBtnCnt + INT_SPACE * (miBtnCnt - 1))) / 2
    For i = 1 To miBtnCnt
        btnButton(marrBtns(i)).Visible = True
        btnButton(marrBtns(i)).Top = Me.ScaleHeight - INT_SPACE - btnButton(marrBtns(i)).Height
        btnButton(marrBtns(i)).Left = fLeftBtnPos + (fBtnWdth + INT_SPACE) * (i - 1)
        If marrBtns(i) = BTN_CANCEL Then
            btnButton(BTN_CANCEL).Cancel = True
        ElseIf marrBtns(i) = BTN_OK Then
            btnButton(BTN_OK).Default = True
        ElseIf marrBtns(i) = BTN_YES Then
            btnButton(BTN_YES).Default = True
        End If
    Next i
    ' icon initialization
    If (miButtons And vbCritical) = vbCritical Then
        Set imgDisplayIcon.Picture = imgIcon(ICN_CRITICAL).Picture
    End If
    If (miButtons And vbQuestion) = vbQuestion Then
        Set imgDisplayIcon.Picture = imgIcon(ICN_QUESTION).Picture
    End If
    If (miButtons And vbExclamation) = vbExclamation Then
        Set imgDisplayIcon.Picture = imgIcon(ICN_EXCLAMATION).Picture
    End If
    If (miButtons And vbInformation) = vbInformation Then
        Set imgDisplayIcon.Picture = imgIcon(ICN_INFORMATIN).Picture
    End If
    
    With New clsCommon
        .CustomizeForm Me
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' button click handler
'
Private Sub btnButton_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case BTN_OK
        miMsgBoxResult = vbOK
    Case BTN_CANCEL
        miMsgBoxResult = vbCancel
    Case BTN_YES
        miMsgBoxResult = vbYes
    Case BTN_NO
        miMsgBoxResult = vbNo
    End Select
    Me.Hide
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'' handling left-right keys
''
'Private Sub btnButton_KeyDown(Index As Integer, KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    Select Case KeyCode
'    Case vbKeyLeft, vbKeyUp
'        SendKeys "+{TAB}"
'    Case vbKeyRight, vbKeyDown
'        SendKeys "{TAB}"
'    End Select
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Activate()
    On Error Resume Next
    ' setting focus
    btnButton(marrBtns(1)).SetFocus
    If ((miButtons And vbDefaultButton2) = vbDefaultButton2) And miBtnCnt >= 2 Then
        btnButton(marrBtns(2)).SetFocus
    End If
    If ((miButtons And vbDefaultButton3) = vbDefaultButton3) And miBtnCnt >= 3 Then
        btnButton(marrBtns(3)).SetFocus
    End If
    If ((miButtons And vbDefaultButton4) = vbDefaultButton4) And miBtnCnt >= 4 Then
        btnButton(marrBtns(4)).SetFocus
    End If
End Sub

