VERSION 5.00
Begin VB.Form frmNewStrategy 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Add strategy"
   ClientHeight    =   2355
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4920
   Icon            =   "frmNewStrategy.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2355
   ScaleWidth      =   4920
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.OptionButton optNew 
      Caption         =   "New strategy"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   2715
   End
   Begin VB.OptionButton optExisting 
      Caption         =   "Existing strategy"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   2595
   End
   Begin VB.TextBox txtName 
      Height          =   315
      Left            =   1140
      MaxLength       =   12
      TabIndex        =   3
      Top             =   1140
      Width           =   3615
   End
   Begin VB.ComboBox cmbName 
      Height          =   315
      ItemData        =   "frmNewStrategy.frx":000C
      Left            =   1140
      List            =   "frmNewStrategy.frx":000E
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   420
      Width           =   3615
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   2520
      TabIndex        =   6
      Top             =   1920
      Width           =   1215
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   1200
      TabIndex        =   5
      Top             =   1920
      Width           =   1215
   End
   Begin VB.TextBox txtDescription 
      Height          =   315
      Left            =   1140
      MaxLength       =   120
      TabIndex        =   4
      Top             =   1500
      Width           =   3615
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Name :"
      Height          =   255
      Left            =   60
      TabIndex        =   9
      Top             =   1170
      Width           =   1035
   End
   Begin VB.Label lblName 
      Alignment       =   1  'Right Justify
      Caption         =   "Name :"
      Height          =   255
      Left            =   60
      TabIndex        =   8
      Top             =   450
      Width           =   1035
   End
   Begin VB.Label lblDescription 
      Alignment       =   1  'Right Justify
      Caption         =   "Description :"
      Height          =   255
      Left            =   60
      TabIndex        =   7
      Top             =   1530
      Width           =   1035
   End
End
Attribute VB_Name = "frmNewStrategy"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public m_mbResult As VbMsgBoxResult
Private m_hComplete As Long

Public Function Execute(ByRef nStrategyID&, ByRef sName As String, ByRef sDescription As String, _
                        Optional ByRef frmOwner As Form = Nothing) As VbMsgBoxResult
    On Error Resume Next
    Dim bRestoreEnabled As Boolean
    
    m_mbResult = vbCancel
    
    Load Me

    LoadStrategies
    optExisting.Value = True
    
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
        If optExisting.Value Then
            nStrategyID = cmbName.ItemData(cmbName.ListIndex)
        Else
            sName = Trim$(txtName.Text)
            sDescription = Trim$(txtDescription.Text)
        End If
    End If
    
    Unload Me
    Execute = m_mbResult
    Exit Function
EH:
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
    gCmn.ErrorMsgBox Me, "Fail to open new strategy editor window."
End Function

Private Sub LoadStrategies()
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    cmbName.Clear
    For Each aStrategy In g_Strategy
        cmbName.AddItem aStrategy.Name
        cmbName.ItemData(cmbName.NewIndex) = aStrategy.ID
    Next
End Sub

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

Private Sub cmbName_Click()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    If optExisting.Value Then
        btnOK.Enabled = (cmbName.ListIndex >= 0)
        
        cmbName.Enabled = True
        txtName.Enabled = False
        txtDescription.Enabled = False
    Else
        btnOK.Enabled = Len(Trim$(txtName.Text)) > 0
    
        cmbName.Enabled = False
        txtName.Enabled = True
        txtDescription.Enabled = True
    End If

    cmbName.BackColor = IIf(cmbName.Enabled, &H80000005, &H8000000F)
    txtName.BackColor = IIf(txtName.Enabled, &H80000005, &H8000000F)
    txtDescription.BackColor = IIf(txtDescription.Enabled, &H80000005, &H8000000F)
End Sub

Private Sub optExisting_Click()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub optNew_Click()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub txtName_Change()
    On Error Resume Next
    UpdateControls
End Sub
