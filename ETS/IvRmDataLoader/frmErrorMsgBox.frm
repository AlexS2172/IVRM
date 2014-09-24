VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmErrorMsgBox 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Error"
   ClientHeight    =   3015
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   5400
   Icon            =   "frmErrorMsgBox.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3015
   ScaleWidth      =   5400
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtInfo 
      Appearance      =   0  'Flat
      Height          =   1575
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   3
      Tag             =   "noauto"
      Top             =   1320
      Width           =   5175
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Cancel          =   -1  'True
      Default         =   -1  'True
      Height          =   300
      Left            =   2040
      TabIndex        =   1
      Top             =   840
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
   Begin ElladaFlatControls.FlatButton btnExtendedInfo 
      Height          =   300
      Left            =   4560
      TabIndex        =   2
      Top             =   840
      Width           =   735
      _ExtentX        =   1296
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
      Caption         =   "Info >>"
   End
   Begin VB.Label lblDescription 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Description"
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   720
      TabIndex        =   0
      Top             =   120
      Width           =   4215
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
Private h As Single

Public Sub Execute(ByVal sDescription As String, Optional ByVal sInfo As String = "")
Dim D As Single
    On Error GoTo EH
    Load Me
    
    h = 1695
    lblDescription.AutoSize = True
    lblDescription.Caption = sDescription
    
    D = lblDescription.Height - 615
    If D < 0 Then D = 0
    
    h = 1695 + D
    Me.Height = h
    btnOK.Top = 840 + D
    btnExtendedInfo.Top = 840 + D
    txtInfo.Top = 1320 + D - 60
    txtInfo.Height = 1575 + D
    Me.Caption = AppTitle() & " Error"
    If Len(Trim(sInfo)) = 0 Then sInfo = "<No additional information available>"
    txtInfo.Text = sInfo & vbCrLf & vbCrLf & _
                     AppTitle() & " Components Info :" & vbCrLf & _
                    "------------------------------------" & vbCrLf & _
                    MakeAppComponentsDescription(True, False)
    
    Me.Show vbModal, g_frmOwner
    Exit Sub
EH:
    MsgBox sDescription, vbCritical + vbOKOnly
End Sub

Private Sub btnExtendedInfo_Click()
    On Error Resume Next
    m_bExpanded = Not m_bExpanded
    
    If m_bExpanded Then
        txtInfo.Visible = True
        btnExtendedInfo.Caption = "<< Info"
        Me.Height = h + h
    Else
        Me.Height = h
        txtInfo.Visible = False
        btnExtendedInfo.Caption = "Info >>"
    End If
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub Form_Load()
    On Error Resume Next
    h = 1695
    gCmn.CustomizeForm Me
    m_bExpanded = True
    btnExtendedInfo_Click
End Sub
