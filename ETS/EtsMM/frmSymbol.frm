VERSION 5.00
Begin VB.Form frmSymbol 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Add New Symbol"
   ClientHeight    =   1050
   ClientLeft      =   2835
   ClientTop       =   3435
   ClientWidth     =   2550
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   620.375
   ScaleMode       =   0  'User
   ScaleWidth      =   2394.312
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtSymbolName 
      Height          =   285
      Left            =   1320
      TabIndex        =   1
      Top             =   165
      Width           =   1095
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   600
      Width           =   1095
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1320
      TabIndex        =   3
      Top             =   600
      Width           =   1095
   End
   Begin VB.Label lblLabels 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Caption         =   "&Symbol name:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   0
      Top             =   180
      Width           =   1095
   End
End
Attribute VB_Name = "frmSymbol"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public IsComplete As Boolean
Public SymbolName As String

Private Sub cmdCancel_Click()
    IsComplete = False
    Me.Hide
End Sub

Private Function IsContractExist(ByVal Symbol As String) As EtsGeneralLib.EtsContractAtom
On Error GoTo Exception
    Set IsContractExist = Nothing
    If (Not g_ContractAll Is Nothing) Then
        If Not g_ContractAll.BySortKey(Symbol) Is Nothing Then
            Set IsContractExist = g_ContractAll.BySortKey(Symbol)
        End If
    End If
Exit Function
Exception:
    Debug.Print "Error in [frmSymbol::IsContractExist(...)]"
End Function

Private Function IsTraderContract(ByVal Symbol As String) As Boolean
On Error GoTo Exception
    IsTraderContract = False
    If (Not g_Contract Is Nothing) Then
        If Not g_Contract.BySortKey(Symbol) Is Nothing Then
            IsTraderContract = True
        End If
    End If
Exit Function
Exception:
    Debug.Print "Error in [frmSymbol::IsTraderContract(...)]"
End Function


Private Sub Signal(ByVal IsFound As Boolean, ByVal InTraderList As Boolean, ByVal Symbol As String, Optional ByVal ContractName As String = "")
    Dim sBuf As String
    sBuf = IIf(InTraderList = False, " Please, add it to current trader stock list.", "")
    If (IsFound) Then
        MsgBox Symbol & " (" & ContractName & ") already exist." & sBuf, vbOKOnly, "Information"
    End If
End Sub
Private Sub cmdOK_Click()
On Error Resume Next
    Dim sysContract As EtsGeneralLib.EtsContractAtom
    If Len(txtSymbolName.Text) > 0 Then
        SymbolName = Trim(txtSymbolName.Text)
        Set sysContract = IsContractExist(SymbolName)
        If (sysContract Is Nothing) Then
            IsComplete = True
            Me.Hide
        Else
            Signal True, IsTraderContract(SymbolName), sysContract.Symbol, sysContract.ContractName
            txtSymbolName.SetFocus
        End If
    Else
        txtSymbolName.SetFocus
    End If
End Sub

