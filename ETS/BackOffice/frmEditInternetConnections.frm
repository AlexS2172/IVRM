VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEditInternetConnections 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Internet Connections"
   ClientHeight    =   3900
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4830
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmEditInternetConnections.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3900
   ScaleWidth      =   4830
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      IMEMode         =   3  'DISABLE
      Index           =   5
      Left            =   1320
      PasswordChar    =   "*"
      TabIndex        =   9
      Text            =   "Value"
      Top             =   2880
      Width           =   3120
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   4
      Left            =   1320
      TabIndex        =   8
      Text            =   "Value"
      Top             =   2520
      Width           =   3120
   End
   Begin VB.OptionButton optValue 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "None"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   1320
      TabIndex        =   2
      Top             =   1320
      Width           =   735
   End
   Begin VB.OptionButton optValue 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "System"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   2400
      TabIndex        =   3
      Top             =   1320
      Width           =   975
   End
   Begin VB.OptionButton optValue 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Custom"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   3480
      TabIndex        =   4
      Top             =   1320
      Width           =   1095
   End
   Begin VB.CheckBox chkProxy 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Enable proxy"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1320
      TabIndex        =   7
      Top             =   2160
      Width           =   1455
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   3
      Left            =   3480
      TabIndex        =   6
      Text            =   "Value"
      Top             =   1680
      Width           =   960
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   2
      Left            =   1320
      TabIndex        =   5
      Text            =   "Value"
      Top             =   1680
      Width           =   1440
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   0
      Left            =   1320
      TabIndex        =   0
      Text            =   "Value"
      Top             =   480
      Width           =   3120
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      IMEMode         =   3  'DISABLE
      Index           =   1
      Left            =   1320
      PasswordChar    =   "*"
      TabIndex        =   1
      Text            =   "Value"
      Top             =   840
      Width           =   3120
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   3360
      TabIndex        =   11
      Top             =   3480
      Width           =   1335
      _ExtentX        =   2355
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
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   1920
      TabIndex        =   10
      Top             =   3480
      Width           =   1335
      _ExtentX        =   2355
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
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Password :"
      Height          =   255
      Index           =   5
      Left            =   240
      TabIndex        =   19
      Top             =   2880
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Login :"
      Height          =   255
      Index           =   4
      Left            =   240
      TabIndex        =   18
      Top             =   2520
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Use Proxy :"
      Height          =   255
      Index           =   6
      Left            =   360
      TabIndex        =   17
      Top             =   1320
      Width           =   855
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "iVolatility authorization"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1328
      TabIndex        =   16
      Top             =   120
      Width           =   2175
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H80000010&
      Height          =   3135
      Left            =   120
      Top             =   240
      Width           =   4575
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Port :"
      Height          =   255
      Index           =   3
      Left            =   2760
      TabIndex        =   15
      Top             =   1680
      Width           =   600
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Address :"
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   14
      Top             =   1680
      Width           =   1080
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Account :"
      Height          =   255
      Index           =   0
      Left            =   240
      TabIndex        =   13
      Top             =   480
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Password :"
      Height          =   255
      Index           =   1
      Left            =   240
      TabIndex        =   12
      Top             =   840
      Width           =   960
   End
End
Attribute VB_Name = "frmEditInternetConnections"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_VolServerName As String
Private m_VolServerPort As Long
Private m_VolServerLogin As String
Private m_VolServerPassword As String
Private m_VolServerAppPath As String

Private m_UseProxy As ETSMANAGERLib.UseProxyType
Private m_ProxyName As String
Private m_ProxyPort As Long

Private m_ProxyAuthType As ETSMANAGERLib.ProxyAuthentificationType
Private m_ProxyLogin As String
Private m_ProxyPassword As String

Private m_mbResult As VbMsgBoxResult
Private m_bChanged As Boolean
    
Private m_IvSettings As ETSMANAGERLib.IvSettings

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As VbMsgBoxResult
    On Error GoTo EH
    Load Me
    LoadInternetSettings
    AdjustControlsAppearance
    m_bChanged = False
    m_mbResult = vbCancel
    Me.Show vbModal, frmMain
    Execute = m_mbResult
    
    Unload Me
    Exit Function
EH:
    gCmn.ErrorMsgBox "Fail to show IV connection settings dialog."
    Execute = vbCancel
    Unload Me
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    m_mbResult = vbCancel
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    SaveInternetSettings
    If m_bChanged Then
        m_mbResult = vbOK
    End If
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub chkProxy_Click()
    On Error Resume Next
    AdjustControlsAppearance
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    gCmn.CustomizeForm Me
    m_mbResult = vbCancel
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AdjustControlsAppearance()
    On Error Resume Next
    txtValue(2).Enabled = optValue(2).Value
    txtValue(3).Enabled = optValue(2).Value
    chkProxy.Enabled = optValue(2).Value
    txtValue(4).Enabled = optValue(2).Value And chkProxy.Value <> 0
    txtValue(5).Enabled = optValue(2).Value And chkProxy.Value <> 0
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set m_IvSettings = Nothing
End Sub

Private Sub optValue_Click(Index As Integer)
    On Error Resume Next
    AdjustControlsAppearance
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtValue_Change(Index As Integer)
    On Error Resume Next
    Select Case Index
        Case 3 'proxy port
            ValidateAmount txtValue(Index)
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadInternetSettings()
    On Error GoTo EH
    
    If m_IvSettings Is Nothing Then Set m_IvSettings = New ETSMANAGERLib.IvSettings
    
    With m_IvSettings
        m_VolServerLogin = .VolLogin
        m_VolServerPassword = .VolPassword
        
        m_UseProxy = .UseProxy
        m_ProxyName = .ProxyName
        m_ProxyPort = .ProxyPort
        m_ProxyAuthType = .ProxyAuthType
        m_ProxyLogin = .ProxyLogin
        m_ProxyPassword = .ProxyPassword
    End With
    
    txtValue(0).Text = m_VolServerLogin
    txtValue(1).Text = m_VolServerPassword
    
    optValue(0).Value = (m_UseProxy = enProxyDisable)
    optValue(1).Value = (m_UseProxy = enProxyDefault)
    optValue(2).Value = (m_UseProxy = enProxyCustom)
        
    txtValue(2).Text = m_ProxyName
    txtValue(3).Text = m_ProxyPort
    
    chkProxy.Value = IIf(m_ProxyAuthType = enProxyAuthEnable, 1, 0)
    
    txtValue(4).Text = m_ProxyLogin
    txtValue(5).Text = m_ProxyPassword
    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to load IV connection settings."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveInternetSettings()
    On Error Resume Next
    
    Dim lPort&: lPort = 0
    
    m_bChanged = False
    If m_VolServerLogin <> txtValue(0).Text Then
        m_VolServerLogin = txtValue(0).Text
        m_bChanged = True
    End If
    
    If m_VolServerPassword <> txtValue(1).Text Then
        m_VolServerPassword = txtValue(1).Text
        m_bChanged = True
    End If
    
    If optValue(0).Value Then
        m_UseProxy = enProxyDisable
    ElseIf optValue(1).Value Then
        m_UseProxy = enProxyDefault
    Else
        m_UseProxy = enProxyCustom
    End If
    
    If m_ProxyName <> txtValue(2).Text Then
        m_ProxyName = txtValue(2).Text
        m_bChanged = True
    End If
    
    lPort = Val(txtValue(3).Text)
    If m_ProxyPort <> lPort Then
        m_ProxyPort = lPort
        m_bChanged = True
    End If
    
    If m_ProxyAuthType <> IIf(chkProxy.Value <> 0, enProxyAuthEnable, enProxyAuthDisable) Then
        m_ProxyAuthType = IIf(chkProxy.Value <> 0, enProxyAuthEnable, enProxyAuthDisable)
        m_bChanged = True
    End If
    
    
    On Error GoTo EH
    
    If m_IvSettings Is Nothing Then Set m_IvSettings = New ETSMANAGERLib.IvSettings
    
    With m_IvSettings
        .VolLogin = m_VolServerLogin
        .VolPassword = m_VolServerPassword
        
        .UseProxy = m_UseProxy
        .ProxyName = m_ProxyName
        .ProxyPort = m_ProxyPort
        .ProxyAuthType = m_ProxyAuthType
        .ProxyLogin = m_ProxyLogin
        .ProxyPassword = m_ProxyPassword
    End With

    Exit Sub
EH:
    gCmn.ErrorMsgBox "Fail to save IV connection settings."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtValue_GotFocus(Index As Integer)
    On Error Resume Next
    With txtValue(Index)
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

