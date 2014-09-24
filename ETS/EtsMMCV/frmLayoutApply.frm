VERSION 5.00
Begin VB.Form frmLayoutApply 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Apply Layout"
   ClientHeight    =   1635
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3030
   Icon            =   "frmLayoutApply.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1635
   ScaleWidth      =   3030
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Frame fraItems 
      Caption         =   " Layout Items to Apply "
      Height          =   975
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   2775
      Begin VB.CheckBox chkFore 
         Caption         =   "Fore Color"
         Height          =   255
         Left            =   240
         TabIndex        =   1
         Top             =   600
         Width           =   2415
      End
      Begin VB.CheckBox chkBack 
         Caption         =   "Back Color"
         Height          =   255
         Left            =   240
         TabIndex        =   0
         Top             =   240
         Width           =   2415
      End
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   360
      TabIndex        =   2
      Top             =   1200
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   1560
      TabIndex        =   3
      Top             =   1200
      Width           =   1095
   End
End
Attribute VB_Name = "frmLayoutApply"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK(ByVal bBack As Boolean, ByVal bFore As Boolean)

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bBack As Boolean
Private m_bFore As Boolean

Public Sub Execute(ByVal bBack As Boolean, ByVal bFore As Boolean, frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub

    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    Load Me

    m_bFore = bFore
    m_bBack = bBack
    chkBack.Value = IIf(bBack, 1, 0)
    chkFore.Value = IIf(bFore, 1, 0)

    UpdateControls
    
    Me.Show vbModeless, m_frmOwner
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open dialog."
    On Error Resume Next
    Unload Me
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    'Me.Hide
    Unload Me
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    'Me.Hide
    RaiseEvent OnOK(chkBack.Value <> 0, chkFore.Value <> 0)
    Unload Me
End Sub

Private Sub chkBack_Click()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    btnOK.Enabled = (chkBack.Value <> 0 Or chkFore.Value <> 0)
End Sub

Private Sub chkFore_Click()
    On Error Resume Next
    UpdateControls
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

