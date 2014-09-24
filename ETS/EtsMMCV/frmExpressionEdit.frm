VERSION 5.00
Begin VB.Form frmExpressionEdit 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Expression Editor"
   ClientHeight    =   2175
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5325
   Icon            =   "frmExpressionEdit.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2175
   ScaleWidth      =   5325
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CheckBox chkApplyToAll 
      Caption         =   "Apply to All"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   1740
      Width           =   2655
   End
   Begin VB.TextBox txtExpression 
      BeginProperty Font 
         Name            =   "Courier New"
         Size            =   9
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1335
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   300
      Width           =   5055
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   4080
      TabIndex        =   3
      Top             =   1740
      Width           =   1095
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   2880
      TabIndex        =   2
      Top             =   1740
      Width           =   1095
   End
   Begin VB.Label lblPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "Prompt :"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   60
      Width           =   2775
   End
End
Attribute VB_Name = "frmExpressionEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_mbResult As VbMsgBoxResult
Private m_hComplete As Long

Public Function Execute(ByVal sCaption As String, ByVal sPrompt As String, ByVal sApplyToAllText As String, _
                        ByRef sExpression As String, ByRef bApplyToAll As Boolean, ByRef frmOwner As Form) As VbMsgBoxResult
    On Error GoTo EH
    Dim bRestoreEnabled As Boolean
    
    m_mbResult = vbCancel
    Load Me
    txtExpression.Text = sExpression
    lblPrompt.Caption = sPrompt
    chkApplyToAll.Value = bApplyToAll
    chkApplyToAll.Caption = sApplyToAllText
    
    If Not frmOwner Is Nothing Then
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            Me.Show vbModeless, frmOwner
        
            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLEVENTS) <> WAIT_OBJECT_0
                Sleep 0
                DoEvents
            Wend
            
            If m_hComplete <> 0 Then
                CloseHandle m_hComplete
                m_hComplete = 0
            End If
            
            If bRestoreEnabled Then frmOwner.Enabled = True
        Else
            Me.Show vbModal
        End If
    Else
        Me.Show vbModal
    End If
    
    If m_mbResult = vbOK Then
        sExpression = Replace$(Trim$(txtExpression.Text), vbCrLf, "")
        bApplyToAll = (chkApplyToAll.Value <> 0)
    End If
    
    Unload Me
    Execute = m_mbResult
    Exit Function
EH:
    gCmn.ErrorMsgBox Me, "Fail to open expression editor window."
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    m_mbResult = vbCancel
    
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    m_mbResult = vbOK
    
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_hComplete <> 0 Then SetEvent m_hComplete
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If m_hComplete <> 0 Then
        CloseHandle m_hComplete
        m_hComplete = 0
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

