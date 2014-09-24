VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form ExecutingFirm 
   Caption         =   "TNT Connector UI"
   ClientHeight    =   8250
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   13695
   LinkTopic       =   "Form1"
   ScaleHeight     =   8250
   ScaleWidth      =   13695
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer Timer1 
      Interval        =   15000
      Left            =   11880
      Top             =   6600
   End
   Begin MSComCtl2.DTPicker OptionExp 
      Height          =   375
      Left            =   4080
      TabIndex        =   35
      Top             =   7080
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   661
      _Version        =   393216
      Format          =   22609921
      CurrentDate     =   37811
   End
   Begin VB.ListBox TNTList 
      Height          =   2985
      ItemData        =   "TNT.frx":0000
      Left            =   0
      List            =   "TNT.frx":0002
      TabIndex        =   34
      Top             =   2760
      Width           =   13695
   End
   Begin VB.CommandButton AcceptTNT 
      Caption         =   "Accept selected TNT message"
      Height          =   2175
      Left            =   8520
      TabIndex        =   33
      Top             =   5880
      Width           =   2535
   End
   Begin VB.TextBox Strike 
      Height          =   285
      Left            =   6840
      TabIndex        =   31
      Text            =   "60"
      Top             =   7440
      Width           =   1575
   End
   Begin VB.TextBox Premium 
      Height          =   285
      Left            =   4080
      TabIndex        =   29
      Text            =   "2.6"
      Top             =   7920
      Width           =   1095
   End
   Begin VB.TextBox Quantity 
      Height          =   285
      Left            =   4080
      TabIndex        =   27
      Text            =   "100"
      Top             =   7560
      Width           =   1095
   End
   Begin VB.TextBox Symbol 
      Height          =   285
      Left            =   4080
      TabIndex        =   25
      Text            =   "AVP"
      Top             =   6720
      Width           =   1095
   End
   Begin VB.TextBox ContraMember 
      Height          =   285
      Left            =   6840
      TabIndex        =   24
      Text            =   "Z03"
      Top             =   7080
      Width           =   1575
   End
   Begin VB.TextBox ContraFirm 
      Height          =   285
      Left            =   6840
      TabIndex        =   23
      Top             =   6720
      Width           =   1575
   End
   Begin VB.TextBox Member 
      Height          =   285
      Left            =   6840
      TabIndex        =   22
      Text            =   "Z97"
      Top             =   6360
      Width           =   1575
   End
   Begin VB.TextBox Firm 
      Height          =   285
      Left            =   6840
      TabIndex        =   21
      Text            =   "Z97"
      Top             =   5880
      Width           =   1575
   End
   Begin VB.TextBox TradeIdEdit 
      Enabled         =   0   'False
      Height          =   285
      Left            =   3840
      TabIndex        =   20
      Text            =   "Z97001"
      Top             =   5880
      Width           =   1095
   End
   Begin VB.CheckBox IsCall 
      Caption         =   "isCall"
      Height          =   195
      Left            =   4080
      TabIndex        =   14
      Top             =   6360
      Width           =   855
   End
   Begin VB.CheckBox IsBuy 
      Caption         =   "isBuy"
      Height          =   195
      Left            =   3000
      TabIndex        =   13
      Top             =   6360
      Width           =   735
   End
   Begin VB.TextBox LOGIN 
      Height          =   285
      Left            =   1080
      TabIndex        =   11
      Text            =   "Z97"
      Top             =   7200
      Width           =   1335
   End
   Begin VB.TextBox PASSWORD 
      Height          =   285
      Left            =   1080
      TabIndex        =   9
      Text            =   "PHOENIXPA3"
      Top             =   6360
      Width           =   1335
   End
   Begin VB.TextBox PORT 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0.0000"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   285
      Left            =   1080
      TabIndex        =   7
      Text            =   "7095"
      Top             =   6720
      Width           =   1335
   End
   Begin VB.TextBox IP 
      Height          =   285
      Left            =   1080
      TabIndex        =   5
      Text            =   "192.168.24.235"
      Top             =   5880
      Width           =   1335
   End
   Begin VB.TextBox Text1 
      Height          =   2655
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Top             =   0
      Width           =   13695
   End
   Begin VB.CommandButton LogOff 
      Caption         =   "LogOff"
      Height          =   315
      Left            =   1320
      TabIndex        =   2
      Top             =   7680
      Width           =   1095
   End
   Begin VB.CommandButton LogOn 
      Caption         =   "LogOn"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   7680
      Width           =   975
   End
   Begin VB.CommandButton TradeAdd 
      Caption         =   "TradeAdd"
      Height          =   315
      Left            =   5400
      TabIndex        =   0
      Top             =   7800
      Width           =   3015
   End
   Begin VB.Line Line1 
      X1              =   2640
      X2              =   2640
      Y1              =   5880
      Y2              =   8040
   End
   Begin VB.Label Label13 
      Caption         =   "OptionExp"
      Height          =   195
      Left            =   3000
      TabIndex        =   32
      Top             =   7080
      Width           =   975
   End
   Begin VB.Label Label12 
      Caption         =   "Strike"
      Height          =   195
      Left            =   5400
      TabIndex        =   30
      Top             =   7440
      Width           =   1335
   End
   Begin VB.Label Label11 
      Caption         =   "Premium"
      Height          =   195
      Left            =   2880
      TabIndex        =   28
      Top             =   7920
      Width           =   975
   End
   Begin VB.Label Label6 
      Caption         =   "TradeQuantity"
      Height          =   195
      Left            =   2880
      TabIndex        =   26
      Top             =   7560
      Width           =   1095
   End
   Begin VB.Label Label10 
      Caption         =   "ContraMember"
      Height          =   195
      Left            =   5520
      TabIndex        =   19
      Top             =   7080
      Width           =   1215
   End
   Begin VB.Label Label9 
      Caption         =   "ContraFirm"
      Height          =   195
      Left            =   5400
      TabIndex        =   18
      Top             =   6720
      Width           =   1215
   End
   Begin VB.Label Label8 
      Caption         =   "ExecutingMember"
      Height          =   195
      Left            =   5160
      TabIndex        =   17
      Top             =   6360
      Width           =   1455
   End
   Begin VB.Label Label7 
      Caption         =   "ExecutingFirm"
      Height          =   195
      Left            =   5520
      TabIndex        =   16
      Top             =   5880
      Width           =   1095
   End
   Begin VB.Label Labl123 
      Caption         =   "OptionSymbol"
      Height          =   195
      Left            =   3000
      TabIndex        =   15
      Top             =   6720
      Width           =   1335
   End
   Begin VB.Label Label5 
      Caption         =   "TradeId"
      Height          =   195
      Left            =   3000
      TabIndex        =   12
      Top             =   5880
      Width           =   615
   End
   Begin VB.Label Label4 
      Caption         =   "Login"
      Height          =   195
      Left            =   120
      TabIndex        =   10
      Top             =   7320
      Width           =   855
   End
   Begin VB.Label Label3 
      Caption         =   "Password"
      Height          =   195
      Left            =   120
      TabIndex        =   8
      Top             =   6360
      Width           =   975
   End
   Begin VB.Label Label2 
      Caption         =   "Port"
      Height          =   195
      Left            =   120
      TabIndex        =   6
      Top             =   6840
      Width           =   375
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "IP"
      Height          =   195
      Left            =   120
      TabIndex        =   4
      Top             =   6000
      Width           =   150
   End
End
Attribute VB_Name = "ExecutingFirm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private WithEvents tx As TNTConnector.CMMHHTAdapter
Attribute tx.VB_VarHelpID = -1
'Private LastTnt As TNTConnector.TNTData
Private LastMessageNo As Long
Dim settings  As TNTConnector.ConnectionSettings
Dim TNTCollection(1000) As TNTConnector.TNTData
Dim ACKCollection(1000) As TNTConnector.ACKData
Private LastTntIndex As Long
Private LastAckIndex As Long
Private relogon As Boolean




Private Sub AcceptTNT_Click()
    Dim TradeId As TNTConnector.MessageDescriptor

    For i = 0 To TNTList.ListCount - 1
        If TNTList.Selected(i) Then
        
            Dim Tnt As TNTConnector.TNTData
            Tnt = TNTCollection(i)
            
            
            Tnt.ExecutingFirm = Tnt.ExecutingMember
'            Tnt.Quantifier = " " ??????????????????????????????????
            
            Tnt.TradeId = Tnt.ExecutingMember + Format(LastMessageNo)
            TradeId = tx.TradeAdd(Tnt)
            
            LastMessageNo = LastMessageNo + 1
            
'            TradeIdEdit.Text = TradeId.TradeId
'            If Tnt.IsBuy = True Then
'                IsBuy.Value = 1
'            Else
'                IsBuy.Value = 0
 '           End If
'
'            IsCall.Value = Tnt.IsCall
'            Symbol.Text = Tnt.OptionSymbol
'            ContraFirm.Text = Tnt.ContraFirm
'            ContraMember.Text = Tnt.ContraMember
'            Strike.Text = Tnt.Strike
'            OptionExp.Value = Tnt.OptionExpDate
 '           Premium.Text = Tnt.PremiumPrice
 '           Quantity.Text = Tnt.TradeQuantity
 '           TradeIdEdit.Text = Tnt.TradeId
            
            Exit For
        End If

    Next i
    


End Sub
    
Private Sub AddToLog(ByVal str As String)
    
    str = Text1.Text + str
    Text1.Text = str
    Text1.SelStart = Len(str)
    
End Sub

Private Sub UpdateTNTList()
    TNTList.Clear
    
    Dim str As String
    
    For i = 0 To LastTntIndex - 1
        str = ConvertTNTToString(TNTCollection(i))
        TNTList.AddItem (str)
'      str = str + Format(saErrors(I).Description) + ", "
    Next i
End Sub

Private Function ConvertTNTToString(Tnt As TNTConnector.TNTData) As String

ConvertTNTToString = vbNewLine + "TradeId=" + Format(Tnt.TradeId) _
    + ", IsBuy=" + Format(Tnt.IsBuy) _
    + ", IsCall=" + Format(Tnt.IsCall) _
    + ", OptionSymbol=" + Format(Tnt.OptionSymbol) _
    + ", ContraFirm=" + Format(Tnt.ContraFirm) _
    + ", ContraMember=" + Format(Tnt.ContraMember) _
    + ", ExecMember = " + Format(Tnt.ExecutingMember) _
    + ", ExecFirm = " + Format(Tnt.ExecutingFirm) _
    + ", TradeDateTime=" + Format(Tnt.TradeDateTime) _
    + ", TradeQuantity=" + Format(Tnt.TradeQuantity) _
    + ", OptionExpDate=" + Format(Tnt.OptionExpDate) _
    + ", Strike=" + Format(Tnt.Strike) _
    + ", PremiumPrice=" + Format(Tnt.PremiumPrice) _
    + ", CMTA=" + Format(Tnt.CMTA) _
    + ", Quantifier=" + Format(Tnt.Quantifier) _
    + ", SpreadIndicator=" + Format(Tnt.SpreadIndicator) _
    + ", OptionalData=" + Format(Tnt.OptionalData)
End Function




Private Sub Timer1_Timer()
    On Error Resume Next
    If relogon = True Then
    
        AddToLog vbNewLine + "Reconnecting."
        Err.Clear
        tx.LogOff
       If Err.Number <> 0 Then AddToLog vbNewLine + "LogOff returns error: " + Err.Description
       Err.Clear
        tx.LogOn settings
        If Err.Number <> 0 Then AddToLog vbNewLine + "LogOn returns error: " + Err.Description
         
    End If
    relogon = False

End Sub

Private Sub TNTList_Click()
    TNTList_LostFocus
End Sub

Private Sub TNTList_KeyUp(KeyCode As Integer, Shift As Integer)
    TNTList_LostFocus
End Sub

Private Sub TNTList_LostFocus()




    For i = 0 To TNTList.ListCount - 1
        If TNTList.Selected(i) Then
        
            Dim Tnt As TNTConnector.TNTData
            Tnt = TNTCollection(i)
            
            TradeIdEdit.Text = Tnt.TradeId

            If Tnt.IsBuy = True Then
                IsBuy.Value = 1
            Else
                IsBuy.Value = 0
            End If

            If Tnt.IsCall Then
                IsCall.Value = 1
            Else
                IsCall.Value = 0
            End If
            
            Firm.Text = Tnt.ExecutingFirm
            Member.Text = Tnt.ExecutingMember
            
            Symbol.Text = Tnt.OptionSymbol
            ContraFirm.Text = Tnt.ContraFirm
            ContraMember.Text = Tnt.ContraMember
            Strike.Text = Tnt.Strike
            OptionExp.Value = Tnt.OptionExpDate
            Premium.Text = Tnt.PremiumPrice
            Quantity.Text = Tnt.TradeQuantity
            TradeIdEdit.Text = Tnt.TradeId
            
            Exit For
        End If

    Next i
End Sub


Private Sub TradeAdd_Click()
    On Error Resume Next
    Dim sd As TNTConnector.TNTData
    Dim TradeId As TNTConnector.MessageDescriptor

    sd.TradeId = Member.Text + Format(LastMessageNo)   'TradeIdEdit.Text
    sd.TradeDateTime = Now
    sd.IsBuy = (IsBuy.Value = 1)
    sd.IsCall = (IsCall.Value = 1)
    sd.OptionSymbol = Symbol.Text
    sd.ExecutingFirm = Firm.Text
    sd.ExecutingMember = Member.Text
    sd.ContraFirm = ContraFirm.Text
    sd.ContraMember = ContraMember.Text
    
    sd.Strike = Strike.Text
    sd.CMTA = ""
    sd.OptionExpDate = OptionExp.Value
    sd.PremiumPrice = Premium.Text
    sd.TradeQuantity = Quantity.Text
        
    Err.Clear
    TradeId = tx.TradeAdd(sd)
    If Err.Number <> 0 Then AddToLog vbNewLine + "TradeAdd returns error: " + Err.Description
        
    TradeIdEdit.Text = TradeId.TradeId
    LastMessageNo = LastMessageNo + 1
End Sub

Private Sub Form_Load()
    Set tx = New TNTConnector.CMMHHTAdapter
    LastTntIndex = 0
    LastAckIndex = 0
    relogon = False
    
'    Dim tnt As tntsettings.CMMHHTSettings
'    Set tnt = New tntsettings.CMMHHTSettings
'    tnt.IPPort = 7093
'    tnt.IPPrimary = "192.168.24.235"
'    tnt.IPSecondary = "192.168.24.235"
'    tnt.UserID = "PP1"
'    tnt.UserPassword = "PHOENIXPA1"
  
    
End Sub


Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    tx.LogOff
    LastMessageNo = 0
End Sub

Private Sub LOGIN_Change()
    Firm.Text = LOGIN.Text
    Member.Text = LOGIN.Text
End Sub

Private Sub LogOff_Click()
    On Error Resume Next
    relogon = False
     Err.Clear
     tx.LogOff
    If Err.Number <> 0 Then AddToLog vbNewLine + "LogOff returns error: " + Err.Description
End Sub

Private Sub LogOn_Click()

'    Dim tnt As tntsettings.CMMHHTSettings
'    tnt.IPPort = PORT.Text '7095
'    tnt.IPPrimary = IP.Text ' "192.168.24.235"
'    tnt.IPSecondary = IP.Text '"192.168.24.235"
'    tnt.UserID = LOGIN.Text '"PP1"
'    Set tnt = Nothing
    
    On Error Resume Next

    settings.PASSWORD = PASSWORD.Text
    settings.PORT = PORT.Text
    settings.PrimaryIP = IP.Text
    settings.SecondaryIP = IP.Text
    settings.ID = LOGIN.Text
    Err.Clear
    tx.LogOn settings
    If Err.Number <> 0 Then AddToLog vbNewLine + "LogOn returns error: " + Err.Description
End Sub



Private Sub tx_ConnectorError(ByVal sCode As Long, ByVal bsMessage As String)
 AddToLog vbNewLine + "ConnectorError: " + bsMessage + vbNewLine
 If sCode <> 0 Then
    relogon = True
 End If
End Sub

Private Sub tx_LogoffResponse(ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)

Dim str As String
str = vbNewLine + " ##### LogoffResponse ####: " + Format(sCode) + " "

For i = 0 To UBound(saErrors)
      str = str + Format(saErrors(i).Description) + ", "
 Next i
 AddToLog (str)
'Text1.Text = Text1.Text + vbNewLine

End Sub

Private Sub tx_LogonResponse(ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)
Dim str As String
str = vbNewLine + " ##### LogoOnResponse ##### : " + Format(sCode) + " "

For i = 0 To UBound(saErrors)
      str = str + Format(saErrors(i).Description) + ", "
 Next i
 AddToLog (str)
End Sub

Private Sub tx_TerminateConnection()
    AddToLog (" ##### TerminateConnection. ##### ")
End Sub


Private Sub tx_TradeAddResponse(pData As TNTConnector.ACKData, ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)
Dim str As String
str = vbNewLine + " ##### TradeAddResponse ##### :" + Format(sCode) + " "
For i = 0 To UBound(saErrors)
      str = str + Format(saErrors(i).Description) + ", "
 Next i
str = str + "ACKData: SequenceNo = " + Format(pData.SequenceNo) + ", POEID = " + Format(pData.POEID)
  AddToLog (str)
  ACKCollection(LastAckIndex) = pData
  LastAckIndex = LastAckIndex + 1

  
End Sub

Private Sub tx_TradeNotification(pData As TNTConnector.TNTData)

    TNTCollection(LastTntIndex) = pData
    LastTntIndex = LastTntIndex + 1
    UpdateTNTList
    
    Dim str As String
    str = vbNewLine + " ##### TradeNotification ##### :" + ConvertTNTToString(pData)
    AddToLog (str)

End Sub

