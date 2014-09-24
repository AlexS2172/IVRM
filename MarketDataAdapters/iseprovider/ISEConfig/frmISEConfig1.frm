VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmISEConfig1 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "ISE Connector Settings"
   ClientHeight    =   6360
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4980
   Icon            =   "frmISEConfig1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6360
   ScaleWidth      =   4980
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   6
      Left            =   2760
      TabIndex        =   11
      Top             =   5160
      Width           =   1200
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   5
      Left            =   2760
      TabIndex        =   10
      Top             =   4800
      Width           =   1200
   End
   Begin VB.ComboBox cmbAccounts 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   1560
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   720
      Width           =   2895
   End
   Begin VB.ComboBox cmbGateways 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   1560
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   5
      Top             =   2760
      Width           =   2895
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      IMEMode         =   3  'DISABLE
      Index           =   1
      Left            =   1560
      PasswordChar    =   "*"
      TabIndex        =   2
      Top             =   1560
      Width           =   1440
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   0
      Left            =   1560
      TabIndex        =   1
      Top             =   1200
      Width           =   1440
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   3
      Left            =   1560
      TabIndex        =   6
      Top             =   3240
      Width           =   1440
   End
   Begin VB.TextBox txtValue 
      Appearance      =   0  'Flat
      Height          =   315
      Index           =   4
      Left            =   1560
      TabIndex        =   7
      Top             =   3600
      Width           =   1440
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   3720
      TabIndex        =   13
      Top             =   5880
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
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Height          =   300
      Left            =   2520
      TabIndex        =   12
      Top             =   5880
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
      Caption         =   "&OK"
   End
   Begin ElladaFlatControls.FlatButton cmbAdd1 
      Height          =   300
      Left            =   3360
      TabIndex        =   3
      Top             =   1200
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
      Caption         =   "Add"
   End
   Begin ElladaFlatControls.FlatButton cmbRemove1 
      Height          =   300
      Left            =   3360
      TabIndex        =   4
      Top             =   1560
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
      Caption         =   "Remove"
   End
   Begin ElladaFlatControls.FlatButton cmbAdd2 
      Height          =   300
      Left            =   3360
      TabIndex        =   8
      Top             =   3240
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
      Caption         =   "Add"
   End
   Begin ElladaFlatControls.FlatButton cmbRemove2 
      Height          =   300
      Left            =   3360
      TabIndex        =   9
      Top             =   3600
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
      Caption         =   "Remove"
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Local logoff time, HH:MM :"
      Height          =   255
      Index           =   5
      Left            =   360
      TabIndex        =   22
      Top             =   5160
      Width           =   2280
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Local logon time, HH:MM :"
      Height          =   255
      Index           =   2
      Left            =   360
      TabIndex        =   21
      Top             =   4800
      Width           =   2280
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      Caption         =   "Server Session"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1800
      TabIndex        =   20
      Top             =   4440
      Width           =   1215
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape3 
      BorderColor     =   &H80000010&
      Height          =   1095
      Left            =   120
      Top             =   4560
      Width           =   4695
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      Caption         =   "Gateways"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1800
      TabIndex        =   19
      Top             =   2400
      Width           =   1095
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape2 
      BorderColor     =   &H80000010&
      Height          =   1695
      Left            =   120
      Top             =   2520
      Width           =   4695
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      Caption         =   "Accounts"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1800
      TabIndex        =   14
      Top             =   240
      Width           =   1215
      WordWrap        =   -1  'True
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Password :"
      Height          =   255
      Index           =   1
      Left            =   480
      TabIndex        =   18
      Top             =   1560
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Account :"
      Height          =   255
      Index           =   0
      Left            =   480
      TabIndex        =   17
      Top             =   1200
      Width           =   960
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Address :"
      Height          =   255
      Index           =   3
      Left            =   360
      TabIndex        =   16
      Top             =   3240
      Width           =   1080
   End
   Begin VB.Label lblPrompt 
      Alignment       =   1  'Right Justify
      Caption         =   "Port :"
      Height          =   255
      Index           =   4
      Left            =   360
      TabIndex        =   15
      Top             =   3600
      Width           =   1080
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H80000010&
      Height          =   1815
      Left            =   120
      Top             =   360
      Width           =   4695
   End
End
Attribute VB_Name = "frmISEConfig1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private m_Changed As Boolean
Private m_Edit As Boolean
Private Accounts As New Collection
Private Gateways As New Collection
Private LogonTime As Date
Private LogoffTime As Date

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As Boolean
    Load Me
    m_Changed = False
    m_Edit = False
    If LoadSettings Then
        m_Edit = True
        Me.Show
    End If
    Execute = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error GoTo Herr
    If CheckSettings Then
        If SaveSettings Then
            Unload Me
        End If
    End If
    Exit Sub
Herr:
    ShowError
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtValue_Change(Index As Integer)
Dim Key As String
Dim pAccount As CAccount
Dim pGateway As CGateway
    If Not m_Edit Then
        Exit Sub
    End If
    On Error Resume Next
    Select Case Index
    Case 0 ' Account
        With cmbAccounts
            If .ListCount > 0 Then
                Key = .ItemData(.ListIndex)
                If Accounts.Count > 0 Then
                    Set pAccount = Accounts.Item(Key)
                    With pAccount
                        .Account = txtValue(0).Text
                    End With
                End If
                .List(.ListIndex) = pAccount.Account
            End If
        End With
    
    Case 1 ' Password
        With cmbAccounts
            If .ListCount > 0 Then
                Key = .ItemData(.ListIndex)
                If Accounts.Count > 0 Then
                    Set pAccount = Accounts.Item(Key)
                    With pAccount
                        .Password = txtValue(1).Text
                    End With
                End If
            End If
        End With
    
    Case 3 ' Gateway
        With cmbGateways
            If .ListCount > 0 Then
                Key = .ItemData(.ListIndex)
                If Gateways.Count > 0 Then
                    Set pGateway = Gateways.Item(Key)
                    With pGateway
                        .Address = txtValue(3).Text
                    End With
                End If
                .List(.ListIndex) = pGateway.Address & " : " & pGateway.Port
            End If
        End With
    Case 4 ' Gateway port
        ValidateAmount txtValue(Index)
        With cmbGateways
            If .ListCount > 0 Then
                Key = .ItemData(.ListIndex)
                If Gateways.Count > 0 Then
                    Set pGateway = Gateways.Item(Key)
                    With pGateway
                        .Port = txtValue(4).Text
                    End With
                End If
                .List(.ListIndex) = pGateway.Address & " : " & pGateway.Port
            End If
        End With
    Case 5, 6
    
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CheckSettings() As Boolean
Dim Msg As String
    m_Changed = False
    
'    If txtValue(1).Text <> txtValue(2).Text Then
'        Err.Raise vbObjectError, "Check Settings", "Account password and confirmation do not match!"
'        txtValue(1).SetFocus
'        Exit Function
'    End If

    On Error Resume Next
    With txtValue(5)
        If Len(.Text) > 0 Then
            LogonTime = TimeValue(.Text)
        Else
            LogonTime = 0
        End If
    End With
    If Err Then
        Msg = "Failed to interpret 'Logon time' field: " & Err.Description
        txtValue(5).SetFocus
        MsgBox Msg, vbExclamation
        CheckSettings = False
        Exit Function
    End If
    
    On Error Resume Next
    With txtValue(6)
        If Len(.Text) > 0 Then
            LogoffTime = TimeValue(.Text)
        Else
            LogoffTime = 0
        End If
    End With
    If Err Then
        Msg = "Failed to interpret 'Logoff time' field: " & Err.Description
        txtValue(6).SetFocus
        MsgBox Msg, vbExclamation
        Exit Function
    End If

    m_Changed = True
    CheckSettings = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadSettings() As Boolean
Dim Index As Variant
Dim pAccount As CAccount
Dim pGateway As CGateway
    
    cmbAccounts.Clear
    For Index = 1 To 3
        Set pAccount = New CAccount
        With pAccount
            .Index = Index
            .Key = "Bins\1\Account " & Format$(Index)
            .Account = GetSettingString(.Key, "User")
            .Password = GetSettingString(.Key, "Password")
            If .Account = "" Then
                .Status = enStatus_Del
            Else
                .Status = enStatus_Edit
            End If
        End With
        If pAccount.Status <> enStatus_Del Then
            Accounts.Add pAccount, Format$(Index)
            With cmbAccounts
                .AddItem pAccount.Account
                .ItemData(.NewIndex) = Index
            End With
        End If
    Next
    
    cmbGateways.Clear
    For Each Index In EnumSettingsKey("Gateways\")
        Set pGateway = New CGateway
        With pGateway
            .Index = CLng(Index)
            .Key = "Gateways\" & Format$(Index)
            .Address = GetSettingString(.Key, "Addr")
            .Port = GetSettingString(.Key, "Port")
            If .Address = "" Then
                .Status = enStatus_Del
            Else
                .Status = enStatus_Edit
            End If
        End With
        Gateways.Add pGateway, Format$(Index)
        If pGateway.Status <> enStatus_Del Then
            With cmbGateways
                .AddItem pGateway.Address & " : " & pGateway.Port
                .ItemData(.NewIndex) = Index
            End With
        End If
    Next
    
    With cmbAccounts
        If .ListCount > 0 Then
            .ListIndex = 0
        End If
        txtValue(0).Enabled = .ListCount > 0
        txtValue(1).Enabled = .ListCount > 0
        cmbRemove1.Enabled = .ListCount > 0
    End With
    
    With cmbGateways
        If .ListCount > 0 Then
            .ListIndex = 0
        End If
        txtValue(3).Enabled = .ListCount > 0
        txtValue(4).Enabled = .ListCount > 0
        cmbRemove2.Enabled = .ListCount > 0
    End With
    
    txtValue(5).Text = GetSettingString("", "Logon Time")
    txtValue(6).Text = GetSettingString("", "Logoff Time")
    
    LoadSettings = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveSettings() As Boolean
Dim Index As Integer
Dim pAccount As CAccount
Dim pGateway As CGateway
    
    For Each pAccount In Accounts
        With pAccount
            If .Status = enStatus_Del Then
                DelSettingsKey .Key
                Accounts.Remove Format$(.Index)
            Else
                SetSettingString .Key, "User", .Account
                SetSettingString .Key, "Password", .Password
            End If
        End With
    Next
    
    For Each pGateway In Gateways
        With pGateway
            If .Status = enStatus_Del Then
                DelSettingsKey .Key
                Gateways.Remove Format$(.Index)
            Else
                SetSettingString .Key, "Addr", .Address
                SetSettingString .Key, "Port", .Port
            End If
        End With
    Next
   
    On Error Resume Next
    ' Normalize keys: 1,2,3... instead of 3,5,8
    For Each pAccount In Accounts
        With pAccount
            DelSettingsKey .Key
        End With
    Next
    
    Index = 0
    For Each pAccount In Accounts
        With pAccount
            If .Status <> enStatus_Del Then
                If Len(.Account) > 0 Then
                    Index = Index + 1
                    .Index = Index
                    .Key = "Bins\1\Account " & Format$(Index)
                    SetSettingString .Key, "User", .Account
                    SetSettingString .Key, "Password", .Password
                End If
            End If
        End With
    Next
    
    For Each pGateway In Gateways
        With pGateway
            DelSettingsKey .Key
        End With
    Next
    
    Index = 0
    For Each pGateway In Gateways
        With pGateway
            If .Status <> enStatus_Del Then
                If Len(.Address) > 0 Then
                    Index = Index + 1
                    .Index = Index
                    .Key = "Gateways\" & Format$(Index)
                    SetSettingString .Key, "Addr", .Address
                    SetSettingString .Key, "Port", .Port
                End If
            End If
        End With
    Next
    
    If Len(txtValue(5).Text) > 0 Then
        SetSettingString "", "Logon Time", Format$(LogonTime, "HH:MM")
    Else
        SetSettingString "", "Logon Time", ""
    End If
    If Len(txtValue(6).Text) > 0 Then
        SetSettingString "", "Logoff Time", Format$(LogoffTime, "HH:MM")
    Else
        SetSettingString "", "Logoff Time", ""
    End If
    
    SaveSettings = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbGateways_Click()
Dim Key As String
Dim pGateway As CGateway
    On Error Resume Next
    With cmbGateways
        If .ListCount > 0 Then
            Key = .ItemData(.ListIndex)
            If Gateways.Count > 0 Then
                Set pGateway = Gateways.Item(Key)
                With pGateway
                    txtValue(3).Text = .Address
                    txtValue(4).Text = .Port
                End With
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbAccounts_Click()
Dim Key As String
Dim pAccount As CAccount
    On Error Resume Next
    With cmbAccounts
        If .ListCount > 0 Then
            Key = .ItemData(.ListIndex)
            If Accounts.Count > 0 Then
                Set pAccount = Accounts.Item(Key)
                With pAccount
                    txtValue(0).Text = .Account
                    txtValue(1).Text = .Password
                End With
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbAdd1_Click()
Dim pAccount As CAccount
Dim NewIndex As Long
Dim Count As Long
    On Error Resume Next
    NewIndex = 1
    For Each pAccount In Accounts
        With pAccount
            If .Status <> enStatus_Del Then
                Count = Count + 1
                If Count >= 3 Then
                    ShowWarning "Not more than 3 accounts is allowed"
                    Exit Sub
                End If
            End If
            NewIndex = Max(NewIndex, .Index + 1)
        End With
    Next
    Set pAccount = New CAccount
    With pAccount
        .Index = NewIndex
        .Status = enStatus_New
        .Key = "Bins\1\Account " & Format$(NewIndex)
        .Account = "New account"
        .Password = ""
    End With
    Accounts.Add pAccount, Format$(NewIndex)
    With cmbAccounts
        .AddItem pAccount.Account
        .ItemData(.NewIndex) = NewIndex
        .ListIndex = .NewIndex
    End With
    txtValue(0).Enabled = True
    txtValue(1).Enabled = True
    cmbRemove1.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbAdd2_Click()
Dim pGateway As CGateway
Dim NewIndex As Long
    On Error Resume Next
    NewIndex = 1
    For Each pGateway In Gateways
        With pGateway
            NewIndex = Max(NewIndex, .Index + 1)
        End With
    Next
    Set pGateway = New CGateway
    With pGateway
        .Index = NewIndex
        .Status = enStatus_New
        .Key = "Gateways\" & Format$(NewIndex)
        .Address = "127.0.0.1"
        .Port = "0"
    End With
    Gateways.Add pGateway, Format$(NewIndex)
    With cmbGateways
        .AddItem pGateway.Address & " : " & pGateway.Port
        .ItemData(.NewIndex) = NewIndex
        .ListIndex = .NewIndex
    End With
    txtValue(3).Enabled = True
    txtValue(4).Enabled = True
    cmbRemove2.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbRemove1_Click()
Dim Key As String
Dim pAccount As CAccount
Dim NewIndex As Long
    On Error Resume Next
    NewIndex = -1
    With cmbAccounts
        If .ListCount > 0 Then
            Key = .ItemData(.ListIndex)
            With Accounts
                If .Count > 0 Then
                    Set pAccount = .Item(Key)
                    With pAccount
                        .Status = enStatus_Del
                    End With
                End If
            End With
            NewIndex = .ListIndex - 1
            .RemoveItem .ListIndex
        End If
        If NewIndex < 0 Then
            If .ListCount > 0 Then
                NewIndex = 0
            End If
        End If
        If NewIndex >= 0 Then
            .ListIndex = NewIndex
        Else
            txtValue(0) = ""
            txtValue(1) = ""
        End If
        txtValue(0).Enabled = .ListCount > 0
        txtValue(1).Enabled = .ListCount > 0
        cmbRemove1.Enabled = .ListCount > 0
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbRemove2_Click()
Dim Key As String
Dim pGateway As CGateway
Dim NewIndex As Long
    On Error Resume Next
    NewIndex = -1
    With cmbGateways
        If .ListCount > 0 Then
            Key = .ItemData(.ListIndex)
            With Gateways
                If .Count > 0 Then
                    Set pGateway = .Item(Key)
                    With pGateway
                        .Status = enStatus_Del
                    End With
                End If
            End With
            NewIndex = .ListIndex - 1
            .RemoveItem .ListIndex
        End If
        If NewIndex < 0 Then
            If .ListCount > 0 Then
                NewIndex = 0
            End If
        End If
        If NewIndex >= 0 Then
            .ListIndex = NewIndex
        Else
            txtValue(3) = ""
            txtValue(4) = ""
        End If
        txtValue(3).Enabled = .ListCount > 0
        txtValue(4).Enabled = .ListCount > 0
        cmbRemove2.Enabled = .ListCount > 0
    End With
End Sub

