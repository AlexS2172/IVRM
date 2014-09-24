VERSION 5.00
Begin VB.Form frmVolaShift 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Volatility Shift"
   ClientHeight    =   1770
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3030
   Icon            =   "frmVolaShift.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1770
   ScaleWidth      =   3030
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.OptionButton optSelExp 
      Caption         =   "Selected"
      Height          =   255
      Left            =   1320
      TabIndex        =   5
      Top             =   960
      Width           =   1575
   End
   Begin VB.OptionButton optAllExp 
      Caption         =   "All"
      Height          =   255
      Left            =   1320
      TabIndex        =   4
      Top             =   600
      Value           =   -1  'True
      Width           =   1575
   End
   Begin VB.TextBox txtValue 
      Height          =   315
      Left            =   1320
      TabIndex        =   3
      Top             =   120
      Width           =   735
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   1545
      TabIndex        =   1
      Top             =   1320
      Width           =   1215
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   225
      TabIndex        =   0
      Top             =   1320
      Width           =   1215
   End
   Begin VB.Label lblExp 
      Alignment       =   1  'Right Justify
      Caption         =   "Shift Expiries :"
      Height          =   255
      Left            =   0
      TabIndex        =   6
      Top             =   600
      Width           =   1215
   End
   Begin VB.Label lblValue 
      Alignment       =   1  'Right Justify
      Caption         =   "Shift Value, % :"
      Height          =   255
      Left            =   0
      TabIndex        =   2
      Top             =   150
      Width           =   1215
   End
End
Attribute VB_Name = "frmVolaShift"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK(ByVal nKey As Long, ByVal bShiftAllExpiries As Boolean, ByVal dShiftValue As Double)

Private m_nKey As Long
Private m_dValue As Double
Private m_bInit As Boolean

Private m_frmOwner As Form
Private m_bIsOpened As Boolean

Public Sub Execute(ByVal nKey As Long, ByVal sTitle As String, ByRef frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    m_bInit = True
    m_nKey = nKey
    
    Load Me
    Me.Caption = sTitle
    LoadSettings
    
    Me.Show vbModeless, m_frmOwner
    txtValue.SetFocus
    
    m_bInit = False
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open vola shift dialog."
    On Error Resume Next
    m_bInit = False
    Unload Me
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "VolaShift", "ShiftAll", optAllExp.Value
    g_aUserXMLParams.SetStringValue APP_XML_KEY & "VolaShift", "ShiftValue", Trim$(Str$(m_dValue))
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    optAllExp.Value = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "VolaShift", "ShiftAll", False)
    optSelExp.Value = Not optAllExp.Value
    
    m_dValue = 1#
    m_dValue = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "VolaShift", "ShiftValue", "1")
    txtValue.Text = Trim$(Str$(m_dValue))
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    If m_dValue = 0# Then Exit Sub
    
    SaveSettings
    
    RaiseEvent OnOK(m_nKey, optAllExp.Value, m_dValue)
    
    Unload Me
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Private Sub txtValue_Change()
    On Error Resume Next
    If m_bInit Then Exit Sub
    Dim dValue#, nSelStart&, bRevert As Boolean
    
    bRevert = False
    dValue = 0#
    dValue = CDbl(txtValue.Text)
    If Err.Number = 0 Then
        If dValue < -100# Then
            dValue = -100#
            bRevert = True
        End If
        
        If dValue > 100# Then
            dValue = 100#
            bRevert = True
        End If
        
        m_dValue = dValue
    Else
        bRevert = True
    End If
    
    If bRevert Then
        nSelStart = txtValue.SelStart
        txtValue.Text = Trim$(Str$(m_dValue))
        txtValue.SelStart = nSelStart
    End If
    btnOK.Enabled = (m_dValue <> 0#)
End Sub

Private Sub txtValue_Validate(Cancel As Boolean)
    On Error Resume Next
    txtValue_Change
End Sub
