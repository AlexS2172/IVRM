VERSION 5.00
Begin VB.Form frmLicense 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "License Key"
   ClientHeight    =   2025
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5385
   Icon            =   "frmLicense.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2025
   ScaleWidth      =   5385
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtKey 
      Height          =   315
      Left            =   1320
      TabIndex        =   0
      Top             =   1080
      Width           =   3735
   End
   Begin VB.TextBox txtUID 
      BackColor       =   &H00C0FFFF&
      Height          =   315
      Left            =   1320
      Locked          =   -1  'True
      TabIndex        =   3
      Top             =   120
      Width           =   3735
   End
   Begin EtsMM.ctlLabelEx lblMail 
      Height          =   255
      Left            =   1320
      TabIndex        =   6
      Top             =   690
      Width           =   3735
      _ExtentX        =   6588
      _ExtentY        =   450
      BackColor       =   -2147483633
      ForeColor       =   16711680
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   -1  'True
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "ETS_Support@egartech.com"
      Alignment       =   2
      PressForeColor  =   16711680
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   2782
      TabIndex        =   2
      Top             =   1560
      Width           =   1215
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "Register"
      Default         =   -1  'True
      Height          =   315
      Left            =   1387
      TabIndex        =   1
      Top             =   1560
      Width           =   1215
   End
   Begin VB.Label lblMailPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "* Please send this code to EGAR support service :"
      Height          =   255
      Left            =   1320
      TabIndex        =   7
      Top             =   480
      Width           =   3735
   End
   Begin VB.Label lblUID 
      Alignment       =   1  'Right Justify
      Caption         =   "Product ID* :"
      Height          =   315
      Left            =   120
      TabIndex        =   5
      Top             =   150
      Width           =   1095
   End
   Begin VB.Label lblKey 
      Alignment       =   1  'Right Justify
      Caption         =   "License Key :"
      Height          =   315
      Left            =   120
      TabIndex        =   4
      Top             =   1110
      Width           =   1095
   End
End
Attribute VB_Name = "frmLicense"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Public m_mbResult As VbMsgBoxResult
'Private m_hComplete As Long
'
'Public Function Execute(ByRef sKey As String, ByVal nUID As Long, ByRef frmOwner As Form) As VbMsgBoxResult
'    On Error Resume Next
'    Dim bRestoreEnabled As Boolean
'
'    m_mbResult = vbCancel
'
'    Load Me
'
'    txtUID.Text = Hex$(nUID)
'    txtKey.Text = sKey
'
'    txtKey_Change
'
'    If Not frmOwner Is Nothing Then
'        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
'        If m_hComplete <> 0 Then
'            ResetEvent m_hComplete
'
'            If frmOwner.Enabled Then
'                frmOwner.Enabled = False
'                bRestoreEnabled = True
'            End If
'            Me.Show vbModeless, frmOwner
'
'            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLEVENTS) <> WAIT_OBJECT_0
'                Sleep 0
'                DoEvents
'            Wend
'
'            If m_hComplete <> 0 Then
'                CloseHandle m_hComplete
'                m_hComplete = 0
'            End If
'
'            If bRestoreEnabled Then frmOwner.Enabled = True
'        Else
'            Me.Show vbModal
'        End If
'    Else
'        Me.Show vbModal
'    End If
'
'    If m_mbResult = vbOK Then
'        sKey = Trim$(txtKey.Text)
'    End If
'
'    Unload Me
'    Execute = m_mbResult
'    Exit Function
'EH:
'    On Error Resume Next
'    If bRestoreEnabled Then frmOwner.Enabled = True
'    Unload Me
'    gCmn.ErrorMsgBox Me, "Fail to open license key window."
'End Function
'
'Private Sub btnOK_Click()
'    On Error Resume Next
'    m_mbResult = vbOK
'    btnCancel_Click
'End Sub
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
'    If m_hComplete <> 0 Then
'        SetEvent m_hComplete
'    Else
'        Me.Hide
'    End If
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
''    gCmn.CustomizeForm Me
'End Sub
'
'Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
'    On Error Resume Next
'    If m_hComplete <> 0 Then SetEvent m_hComplete
'End Sub
'
'Private Sub Form_Terminate()
'    On Error Resume Next
'    If m_hComplete <> 0 Then
'        CloseHandle m_hComplete
'        m_hComplete = 0
'    End If
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    Form_Terminate
'End Sub
'
'Private Sub lblMail_Click()
'    On Error Resume Next
'    Dim sMsg$
'    sMsg = "mailto:ETS_Support@egartech.com" & _
'        "?Subject=ETS Registration request" & _
'        "&Body=I would like to register the ETS application for  ___  month(s)." & _
'        "%0D%0A%0D%0AUser: " & g_sLicenseUser & _
'        "%0D%0ACompany: " & g_sLicenseCompany & _
'        "%0D%0AProduct ID: " & txtUID.Text
'
'    ShellExecute Me.hWnd, "open", sMsg, "", "", 1
'End Sub
'
'Private Sub txtKey_Change()
'    On Error Resume Next
'    btnOK.Enabled = Len(Trim$(txtKey.Text)) > 0
'End Sub
