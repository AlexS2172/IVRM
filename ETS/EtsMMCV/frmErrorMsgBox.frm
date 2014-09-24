VERSION 5.00
Begin VB.Form frmErrorMsgBox 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "ETS Error"
   ClientHeight    =   3840
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5490
   Icon            =   "frmErrorMsgBox.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3840
   ScaleWidth      =   5490
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtInfo 
      Height          =   2355
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Tag             =   "noauto"
      Top             =   1320
      Width           =   5235
   End
   Begin VB.CommandButton btnOK 
      Cancel          =   -1  'True
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   2055
      TabIndex        =   1
      Top             =   840
      Width           =   1275
   End
   Begin VB.CommandButton btnExtendedInfo 
      Caption         =   "Info >>"
      Height          =   315
      Left            =   4620
      TabIndex        =   2
      Top             =   840
      Width           =   735
   End
   Begin VB.Label lblDescription 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Description"
      ForeColor       =   &H80000008&
      Height          =   675
      Left            =   720
      TabIndex        =   0
      Top             =   120
      UseMnemonic     =   0   'False
      Width           =   4635
      WordWrap        =   -1  'True
   End
   Begin VB.Image imgIcon 
      Height          =   480
      Index           =   1
      Left            =   120
      Picture         =   "frmErrorMsgBox.frx":000C
      Top             =   120
      Width           =   480
   End
End
Attribute VB_Name = "frmErrorMsgBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private m_bExpanded As Boolean

Private m_hComplete As Long

Public Sub Execute(ByRef frmOwner As Form, ByVal sDescription As String, Optional ByVal sInfo As String = "")
    On Error GoTo EH
    Dim bRestoreEnabled As Boolean, nTempMouse&
    
    Load Me
    
    nTempMouse = Screen.MousePointer
    Screen.MousePointer = vbDefault
    
    lblDescription.Caption = sDescription
    
    'If Len(Trim$(sInfo)) = 0 Then sInfo = "<No additional information available>"
     txtInfo.Text = sInfo & vbCrLf & vbCrLf & _
                    "ETS Components Info :" & vbCrLf & _
                    "------------------------------------" & vbCrLf & _
                    MakeAppComponentsDescription(True, False)
    
    If Not frmOwner Is Nothing Then
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            Me.Show vbModeless, frmOwner
        
            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLINPUT) <> WAIT_OBJECT_0
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
    
    Screen.MousePointer = nTempMouse
    
    Unload Me
    Exit Sub
EH:
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
    MsgBox sDescription & vbCrLf & sInfo, vbCritical + vbOKOnly
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description
End Sub

Private Sub btnExtendedInfo_Click()
    On Error Resume Next
    m_bExpanded = Not m_bExpanded
    
    If m_bExpanded Then
        txtInfo.Visible = True
        btnExtendedInfo.Caption = "<< Info"
        Me.Height = 4215
    Else
        Me.Height = 1695
        txtInfo.Visible = False
        btnExtendedInfo.Caption = "Info >>"
    End If
End Sub

Private Sub btnOK_Click()
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
    m_bExpanded = True
    
    Dim nTabStops(0 To 1) As Long
    nTabStops(0) = 80
    nTabStops(1) = 120
    SendMessage txtInfo.hWnd, EM_SETTABSTOPS, 2, nTabStops(0)
    
    btnExtendedInfo_Click
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
