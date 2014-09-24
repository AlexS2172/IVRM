VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "Transport client - OFFLINE"
   ClientHeight    =   3585
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6570
   LinkTopic       =   "Form1"
   ScaleHeight     =   3585
   ScaleWidth      =   6570
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame frmLog 
      Caption         =   "Log"
      Height          =   3375
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   3495
      Begin VB.TextBox eLog 
         Height          =   3015
         Left            =   120
         Locked          =   -1  'True
         MultiLine       =   -1  'True
         ScrollBars      =   2  'Vertical
         TabIndex        =   6
         Top             =   240
         Width           =   3255
      End
   End
   Begin VB.Frame frmSession 
      Caption         =   "Session"
      Height          =   3375
      Left            =   3720
      TabIndex        =   0
      Top             =   120
      Width           =   2775
      Begin VB.CommandButton btnUnsubscribe 
         Caption         =   "Unsubscribe"
         Height          =   375
         Left            =   1440
         TabIndex        =   9
         Top             =   720
         Width           =   1215
      End
      Begin VB.CommandButton btnSubscribe 
         Caption         =   "Subscribe"
         Height          =   375
         Left            =   120
         TabIndex        =   4
         Top             =   720
         Width           =   1215
      End
      Begin VB.TextBox eMessage 
         Height          =   1245
         Left            =   120
         MultiLine       =   -1  'True
         TabIndex        =   3
         Top             =   2040
         Width           =   2535
      End
      Begin VB.CommandButton btnSendMessage 
         Caption         =   "Send"
         Height          =   375
         Left            =   1800
         TabIndex        =   2
         Top             =   1440
         Width           =   855
      End
      Begin VB.TextBox eSubject 
         Height          =   285
         Left            =   120
         TabIndex        =   1
         Text            =   "MSFT"
         Top             =   1440
         Width           =   1575
      End
      Begin VB.Label lMessage 
         Caption         =   "Message"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   1800
         Width           =   855
      End
      Begin VB.Label lSubject 
         Caption         =   "Subject"
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   1200
         Width           =   615
      End
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Dim objHost As EGTPPRXLib.HostProxy

Dim WithEvents objSession As EGTPPRXLib.SessionProxy
Attribute objSession.VB_VarHelpID = -1

Dim MessNum As Long

Private Sub Log(ByVal Msg As String)
    Dim f As Long
    f = FreeFile
    Open "receivemsg.txt" For Append As #f
    Print #f, Msg & Chr(13) & Chr(10)
    Close #f
End Sub

Private Sub btnSubscribe_Click()
    On Error GoTo ErrHandler
    
    If Not objSession Is Nothing Then
        objSession.Subscribe eSubject.Text
    End If

Exit Sub
ErrHandler:
   MsgBox Err.Description
End Sub

Private Sub btnUnsubscribe_Click()
    On Error GoTo ErrHandler
    
    If Not objSession Is Nothing Then
        objSession.Unsubscribe eSubject.Text
    End If

Exit Sub
ErrHandler:
   MsgBox Err.Description
End Sub

Private Sub btnSendMessage_Click()
    Dim sString As String
    
    On Error GoTo ErrHandler
    
    sString = String(CLng(eMessage.Text), "1")
    If Not objSession Is Nothing Then
        objSession.SendMessage eSubject.Text, TRANSPORTLib.MsgPropertiesEnum.enMpGuarantee, sString
    End If
Exit Sub

ErrHandler:
   MsgBox Err.Description
End Sub


Private Sub objSession_OnLogoff(ByVal bsRouterName As String)
    MainForm.Caption = "Transport client - OFFLINE " & bsRouterName
    btnSendMessage.Enabled = False
    btnSubscribe.Enabled = False
    btnUnsubscribe.Enabled = False
End Sub

Private Sub objSession_OnLogon(ByVal bsRouterName As String)
    MainForm.Caption = "Transport client - ONLINE " & bsRouterName
    btnSendMessage.Enabled = True
    btnSubscribe.Enabled = True
    btnUnsubscribe.Enabled = True
End Sub




Private Sub Form_Load()
    On Error GoTo ErrHandler
    
    Set objHost = New EGTPPRXLib.HostProxy
    objHost.Logon ""
    
    Set objSession = objHost.CreateSession
    
    MessNum = 0
    
    If objSession.IsLogoned Then
        MainForm.Caption = "Transport client - ONLINE " & objHost.RouterName
        btnSendMessage.Enabled = True
        btnSubscribe.Enabled = True
        btnUnsubscribe.Enabled = True
    Else
        MainForm.Caption = "Transport client - OFFLINE " & objHost.RouterName
        btnSendMessage.Enabled = False
        btnSubscribe.Enabled = False
        btnUnsubscribe.Enabled = False
    End If
Exit Sub
ErrHandler:
    MsgBox Err.Description
End Sub

Private Sub Form_Unload(Cancel As Integer)
    Set objHost = Nothing
    Set objSession = Nothing
End Sub

Private Sub objSession_OnMessage(ByVal bsSubject As String, ByVal bsMessage As String)
Dim dInt As Integer
dInt = 0
End Sub
