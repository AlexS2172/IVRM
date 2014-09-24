VERSION 5.00
Begin VB.Form frmEntityEditor 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Entity Editor"
   ClientHeight    =   1380
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4920
   Icon            =   "frmEntityEditor.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1380
   ScaleWidth      =   4920
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtDescription 
      Height          =   315
      Left            =   1140
      TabIndex        =   1
      Top             =   540
      Width           =   3615
   End
   Begin VB.TextBox txtName 
      Height          =   315
      Left            =   1140
      TabIndex        =   0
      Top             =   120
      Width           =   3615
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   1193
      TabIndex        =   2
      Top             =   960
      Width           =   1215
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   2513
      TabIndex        =   3
      Top             =   960
      Width           =   1215
   End
   Begin VB.Label lblDescription 
      Alignment       =   1  'Right Justify
      Caption         =   "Description :"
      Height          =   255
      Left            =   60
      TabIndex        =   5
      Top             =   570
      Width           =   1035
   End
   Begin VB.Label lblName 
      Alignment       =   1  'Right Justify
      Caption         =   "Name :"
      Height          =   255
      Left            =   60
      TabIndex        =   4
      Top             =   150
      Width           =   1035
   End
End
Attribute VB_Name = "frmEntityEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public m_mbResult As VbMsgBoxResult
Private m_hComplete As Long

Public Function Execute(ByVal sCaption As String, ByRef sName As String, ByRef sDescription As String, _
                        Optional ByVal nNameMaxLen As Long = 0&, _
                        Optional ByVal nDescriptionMaxLen As Long = 0&, _
                        Optional ByRef frmOwner As Form = Nothing) As VbMsgBoxResult
    On Error Resume Next
    Dim bRestoreEnabled As Boolean
    
    m_mbResult = vbCancel
    
    Load Me

    Me.Caption = sCaption
    
    txtName.MaxLength = nNameMaxLen
    txtDescription.MaxLength = nDescriptionMaxLen

    txtName.Text = sName
    txtDescription.Text = sDescription
    
    txtName_Change
    
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
        sName = Trim$(txtName.Text)
        sDescription = Trim$(txtDescription.Text)
    End If
    
    Unload Me
    Execute = m_mbResult
    Exit Function
EH:
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
    gCmn.ErrorMsgBox Me, "Fail to open entity editor window."
End Function

Private Sub btnOK_Click()
    On Error Resume Next
    m_mbResult = vbOK
    btnCancel_Click
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
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

Private Sub txtName_Change()
    On Error Resume Next
    btnOK.Enabled = Len(Trim$(txtName.Text)) > 0
End Sub


