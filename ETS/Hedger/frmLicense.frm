VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{E395ECF2-0B93-464D-AD1B-99A443BDF17F}#1.0#0"; "FocusFlatControls1.ocx"
Begin VB.Form frmLicense 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "License Key"
   ClientHeight    =   1755
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5655
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmLicense.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1755
   ScaleWidth      =   5655
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtKey 
      Appearance      =   0  'Flat
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   1440
      TabIndex        =   2
      Top             =   900
      Width           =   3975
   End
   Begin VB.TextBox txtUID 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H80000018&
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   1440
      Locked          =   -1  'True
      TabIndex        =   1
      Top             =   450
      Width           =   3975
   End
   Begin FocusFlatControls1.FlatButton btnRegister 
      Default         =   -1  'True
      Height          =   315
      Left            =   2640
      TabIndex        =   4
      Top             =   1320
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "  &Register"
   End
   Begin FocusFlatControls1.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   315
      Left            =   4080
      TabIndex        =   5
      Top             =   1320
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   " &Cancel"
   End
   Begin ElladaFlatControls.ActiveLabel lbl 
      Height          =   315
      Left            =   1440
      TabIndex        =   6
      Tag             =   "red"
      ToolTipText     =   "Go to the subportfolios summary page"
      Top             =   120
      Width           =   3915
      _ExtentX        =   6906
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HoverForeColor  =   8454016
      Caption         =   "(Please send this code to EGAR support service)"
      Alignment       =   2
      PressForeColor  =   8454016
      DisabledForeColor=   12632256
   End
   Begin VB.Label lblKey 
      Alignment       =   1  'Right Justify
      Caption         =   "License Key :"
      Height          =   315
      Left            =   120
      TabIndex        =   3
      Top             =   930
      Width           =   1215
   End
   Begin VB.Label lblUID 
      Alignment       =   1  'Right Justify
      Caption         =   "Product ID :"
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   480
      Width           =   1215
   End
End
Attribute VB_Name = "frmLicense"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Public m_bResult As VbMsgBoxResult

Public Function Execute(sKey As String, ByVal lUID As Long, frmOwner As Form) As VbMsgBoxResult
    On Error Resume Next
    
    Load Me
    m_bResult = vbCancel
    
    txtUID.Text = Hex$(lUID)
    txtKey.Text = sKey
    
    AdjustControlsAppearance
    
    Show vbModal, frmOwner
    
    If m_bResult = vbOK Then
        sKey = txtKey.Text
    End If
    Execute = m_bResult
    Unload Me
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

Private Sub btnRegister_Click()
    On Error Resume Next
    m_bResult = vbOK
    Me.Hide
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub AdjustControlsAppearance()
    On Error Resume Next
    btnRegister.Enabled = Len(Trim$(txtKey.Text)) > 0
End Sub

Private Sub lbl_Click()
    On Error Resume Next
    Dim Msg As String
    'edaSupport
    Msg = "mailto:edaSupport@egartech.com" & _
        "?Subject=Egar Hedger Registration request" & _
        "&Body=I would like to register the Egar Hedger for  ___  month(s)." & vbCrLf & _
        " Product ID: " & txtUID.Text
    
    ShellExecute Me.hWnd, "open", Msg, "", "", 1
End Sub

Private Sub txtKey_Change()
    On Error Resume Next
    AdjustControlsAppearance
End Sub
