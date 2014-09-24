VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.Form frmNettingParams 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Confirm position netting params"
   ClientHeight    =   2115
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3390
   Icon            =   "frmNettingParams.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2115
   ScaleWidth      =   3390
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CheckBox chkDeleteManual 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Delete manual trades"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   495
      TabIndex        =   2
      Top             =   900
      Width           =   2475
   End
   Begin VB.CheckBox chkDeleteSimulated 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Delete simulated trades"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   495
      TabIndex        =   1
      Top             =   540
      Width           =   2475
   End
   Begin VB.CheckBox chkSaveHistory 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Save trades history"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   495
      TabIndex        =   3
      Top             =   1260
      Width           =   2475
   End
   Begin MSComCtl2.DTPicker dtpDate 
      Height          =   315
      Left            =   1500
      TabIndex        =   0
      Top             =   120
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   556
      _Version        =   393216
      Format          =   59441153
      CurrentDate     =   36936
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Height          =   300
      Left            =   525
      TabIndex        =   4
      Top             =   1620
      Width           =   1155
      _ExtentX        =   2037
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
      Caption         =   "&OK"
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   1785
      TabIndex        =   5
      Top             =   1620
      Width           =   1155
      _ExtentX        =   2037
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
      Caption         =   "&Cancel"
   End
   Begin VB.Label lblDate 
      BackStyle       =   0  'Transparent
      Caption         =   "Netting date :"
      Height          =   255
      Left            =   480
      TabIndex        =   6
      Top             =   180
      Width           =   1815
   End
End
Attribute VB_Name = "frmNettingParams"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_Date As Date
Private m_bOK As Boolean
Private m_bIsDelSimulated As Boolean
Private m_bIsDelManual As Boolean
Private m_bIsSaveHistory As Boolean

Public Function Execute(ByVal sCaption As String, ByVal sDateCaption As String, ByRef dtNetDate As Date, _
                        ByRef bIsDelSimulated As Boolean, ByRef bIsDelManual As Boolean, ByRef bIsSaveHistory As Boolean) As Boolean
    On Error GoTo EH
    m_Date = dtNetDate
    m_bIsDelSimulated = bIsDelSimulated
    m_bIsDelManual = bIsDelManual
    m_bIsSaveHistory = bIsSaveHistory
    dtpDate.Value = m_Date
    dtpDate.Tag = gCmn.FmtDate(m_Date)
    m_bOK = False
    
    chkDeleteSimulated.Value = IIf(bIsDelSimulated, 1, 0)
    chkDeleteSimulated.Visible = bIsDelSimulated
    
    chkDeleteManual.Value = IIf(bIsDelManual, 1, 0)
    chkDeleteManual.Visible = bIsDelManual
    
    chkSaveHistory.Value = IIf(bIsSaveHistory, 1, 0)
    chkSaveHistory.Visible = bIsSaveHistory
    
    Load Me
    Me.Caption = sCaption
    Me.lblDate.Caption = sDateCaption
    
    Show vbModal
    
    Unload Me
    
    If m_bOK Then
        dtNetDate = m_Date
        bIsDelSimulated = m_bIsDelSimulated
        bIsDelManual = m_bIsDelManual
        bIsSaveHistory = m_bIsSaveHistory
    End If
    
    Execute = m_bOK
    Exit Function
EH:
    ShowError
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    m_bOK = True
    Unload Me
End Sub

Private Sub chkDeleteManual_Click()
On Error Resume Next
    m_bIsDelManual = chkDeleteManual.Value
End Sub

Private Sub chkDeleteSimulated_Click()
    On Error Resume Next
    m_bIsDelSimulated = chkDeleteSimulated.Value
End Sub

Private Sub chkSaveHistory_Click()
    On Error Resume Next
    m_bIsSaveHistory = chkSaveHistory.Value
End Sub

Private Sub dtpDate_Change()
    On Error Resume Next
    m_Date = dtpDate.Value
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
End Sub


