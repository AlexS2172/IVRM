VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form TNTServerForm 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "TNT Server Emulator"
   ClientHeight    =   7905
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4410
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   7905
   ScaleWidth      =   4410
   StartUpPosition =   3  'Windows Default
   Begin MSComCtl2.DTPicker OptionExp 
      Height          =   285
      Left            =   1200
      TabIndex        =   35
      Top             =   2880
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   503
      _Version        =   393216
      CustomFormat    =   "MM/yy"
      Format          =   3932163
      CurrentDate     =   37811
   End
   Begin VB.CommandButton ApplyTradeAdd 
      Caption         =   "Apply"
      Height          =   255
      Left            =   3360
      TabIndex        =   34
      Top             =   1200
      Width           =   855
   End
   Begin VB.CommandButton ApplayLogon 
      Caption         =   "Apply"
      Height          =   255
      Left            =   3360
      TabIndex        =   33
      Top             =   840
      Width           =   855
   End
   Begin VB.TextBox TradeAddRejectReason 
      Height          =   285
      Left            =   2640
      TabIndex        =   32
      Text            =   "Text2"
      Top             =   1200
      Width           =   615
   End
   Begin VB.TextBox LogonRejectReason 
      Height          =   285
      Left            =   2640
      TabIndex        =   31
      Text            =   "Text1"
      Top             =   840
      Width           =   615
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Send TNT"
      Height          =   315
      Left            =   2520
      TabIndex        =   18
      Top             =   3600
      Width           =   1695
   End
   Begin VB.CheckBox IsBuy 
      Caption         =   "isBuy"
      Height          =   195
      Left            =   120
      TabIndex        =   17
      Top             =   2160
      Width           =   735
   End
   Begin VB.CheckBox IsCall 
      Caption         =   "isCall"
      Height          =   195
      Left            =   1080
      TabIndex        =   16
      Top             =   2160
      Width           =   855
   End
   Begin VB.TextBox TradeIdEdit 
      Height          =   285
      Left            =   840
      TabIndex        =   15
      Text            =   "Z97001"
      Top             =   1680
      Width           =   1095
   End
   Begin VB.TextBox Firm 
      Height          =   285
      Left            =   3720
      TabIndex        =   14
      Text            =   "Z97"
      Top             =   1680
      Width           =   495
   End
   Begin VB.TextBox Member 
      Height          =   285
      Left            =   3720
      TabIndex        =   13
      Text            =   "Z97"
      Top             =   2160
      Width           =   495
   End
   Begin VB.TextBox ContraFirm 
      Height          =   285
      Left            =   3720
      TabIndex        =   12
      Top             =   2520
      Width           =   495
   End
   Begin VB.TextBox ContraMember 
      Height          =   285
      Left            =   3720
      TabIndex        =   11
      Text            =   "Z03"
      Top             =   2880
      Width           =   495
   End
   Begin VB.TextBox Symbol 
      Height          =   285
      Left            =   1200
      TabIndex        =   10
      Text            =   "AVP"
      Top             =   2520
      Width           =   1095
   End
   Begin VB.TextBox Quantity 
      Height          =   285
      Left            =   1200
      TabIndex        =   9
      Text            =   "100"
      Top             =   3240
      Width           =   1095
   End
   Begin VB.TextBox Premium 
      Height          =   285
      Left            =   1200
      TabIndex        =   8
      Text            =   "2.6"
      Top             =   3600
      Width           =   1095
   End
   Begin VB.TextBox Strike 
      Height          =   285
      Left            =   3360
      TabIndex        =   7
      Text            =   "60"
      Top             =   3240
      Width           =   855
   End
   Begin VB.CheckBox RejectTradeAdd 
      Caption         =   "Reject Trade Add"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1200
      Width           =   1575
   End
   Begin VB.CheckBox RejectLogon 
      Caption         =   "Reject Logon"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   840
      Width           =   1575
   End
   Begin VB.CommandButton bsStop 
      Caption         =   "Stop"
      Enabled         =   0   'False
      Height          =   375
      Left            =   3000
      TabIndex        =   4
      Top             =   120
      Width           =   1215
   End
   Begin VB.CommandButton btStart 
      Caption         =   "Start"
      Height          =   375
      Left            =   1560
      TabIndex        =   3
      Top             =   120
      Width           =   1215
   End
   Begin VB.TextBox tntPort 
      Height          =   285
      Left            =   480
      TabIndex        =   1
      Text            =   "7095"
      Top             =   120
      Width           =   975
   End
   Begin VB.TextBox TNTLogScreen 
      Height          =   3675
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   0
      Top             =   4080
      Width           =   4095
   End
   Begin VB.Line Line2 
      X1              =   120
      X2              =   4200
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Label Label3 
      Caption         =   "Reason:"
      Height          =   255
      Left            =   1800
      TabIndex        =   30
      Top             =   1200
      Width           =   735
   End
   Begin VB.Label Label2 
      Caption         =   "Reason:"
      Height          =   255
      Left            =   1800
      TabIndex        =   29
      Top             =   840
      Width           =   735
   End
   Begin VB.Label Label5 
      Caption         =   "TradeId"
      Height          =   195
      Left            =   120
      TabIndex        =   28
      Top             =   1680
      Width           =   615
   End
   Begin VB.Label Labl123 
      Caption         =   "OptionSymbol"
      Height          =   195
      Left            =   120
      TabIndex        =   27
      Top             =   2520
      Width           =   1335
   End
   Begin VB.Label Label7 
      Caption         =   "ExecutingFirm"
      Height          =   195
      Left            =   2400
      TabIndex        =   26
      Top             =   1680
      Width           =   1095
   End
   Begin VB.Label Label8 
      Caption         =   "ExecutingMember"
      Height          =   195
      Left            =   2280
      TabIndex        =   25
      Top             =   2160
      Width           =   1455
   End
   Begin VB.Label Label9 
      Caption         =   "ContraFirm"
      Height          =   195
      Left            =   2520
      TabIndex        =   24
      Top             =   2520
      Width           =   1215
   End
   Begin VB.Label Label10 
      Caption         =   "ContraMember"
      Height          =   195
      Left            =   2520
      TabIndex        =   23
      Top             =   2880
      Width           =   1215
   End
   Begin VB.Label Label6 
      Caption         =   "TradeQuantity"
      Height          =   195
      Left            =   120
      TabIndex        =   22
      Top             =   3240
      Width           =   1095
   End
   Begin VB.Label Label11 
      Caption         =   "Premium"
      Height          =   195
      Left            =   240
      TabIndex        =   21
      Top             =   3600
      Width           =   975
   End
   Begin VB.Label Label12 
      Caption         =   "Strike"
      Height          =   195
      Left            =   2520
      TabIndex        =   20
      Top             =   3240
      Width           =   855
   End
   Begin VB.Label Label13 
      Caption         =   "OptionExp"
      Height          =   195
      Left            =   120
      TabIndex        =   19
      Top             =   2880
      Width           =   975
   End
   Begin VB.Line Line1 
      X1              =   120
      X2              =   4200
      Y1              =   1560
      Y2              =   1560
   End
   Begin VB.Label Label1 
      Caption         =   "Port:"
      Height          =   285
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   375
   End
End
Attribute VB_Name = "TNTServerForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents TNT As CTNTServer
Attribute TNT.VB_VarHelpID = -1



Private Sub bsStop_Click()
    btStart.Enabled = True
    bsStop.Enabled = False
    tntPort.Enabled = True
    TNT.Stop
    
   TNTLogScreen.Text = TNTLogScreen.Text + "Service is stopped" + vbNewLine

End Sub

Private Sub btStart_Click()
   
    TNT.Start CInt(tntPort.Text)
    
    btStart.Enabled = False
    bsStop.Enabled = True
    tntPort.Enabled = False
    TNTLogScreen.Text = TNTLogScreen.Text + "Service is started" + vbNewLine
    
End Sub

Private Sub Command1_Click()
    Dim data As TNTServerData
    
    
    data.TradeID = TradeIdEdit.Text
    data.TradeDateTime = Now
    data.IsBuy = (IsBuy.Value = 1)
    data.IsCall = (IsCall.Value = 1)
    data.OptionSymbol = Symbol.Text
    data.ExecutingFirm = Firm.Text
    data.ExecutingMember = Member.Text
    data.ContraFirm = ContraFirm.Text
    data.ContraMember = ContraMember.Text
    data.Strike = Strike.Text
    data.CMTA = ""
    data.OptionExpDate = OptionExp.Value
    data.PremiumPrice = Premium.Text
    data.TradeQuantity = Quantity.Text
        
    
    TNT.SendTNT data
    
    
    
    
    
End Sub

Private Sub Form_Load()
 Set TNT = New CTNTServer
 Dim str As String
 
 OptionExp.Value = Date
 
TNT.get_RejectLogon RejectLogon.Value, str
LogonRejectReason.Text = str
TNT.get_RejectTradeAdd RejectTradeAdd.Value, str
 TradeAddRejectReason.Text = str
'    tntPort.Text = ""
'    TNTLogScreen.Text = ""
'    Dim settings As TNTSettings.CMMHHTSettings
'    Set settings = New TNTSettings.CMMHHTSettings
    
'    tntPort.Text = settings.IPPort
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
If Not TNT Is Nothing Then
    TNT.Stop
    Set TNT = Nothing
End If
End Sub

Private Sub TNT_LogMessage(ByVal bsMessage As String)

Dim str As String
str = TNTLogScreen.Text
str = str + bsMessage + vbNewLine
If Len(str) > 2048 Then
   str = Right(str, 1024)
End If
TNTLogScreen.Text = str
    
End Sub

Private Sub ApplayLogon_Click()
TNT.put_RejectLogon RejectLogon.Value, LogonRejectReason.Text
End Sub

Private Sub ApplyTradeAdd_Click()
TNT.put_RejectTradeAdd RejectTradeAdd.Value, TradeAddRejectReason.Text
End Sub
