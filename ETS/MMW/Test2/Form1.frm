VERSION 5.00
Begin VB.Form Form1 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "CEResponseInit"
   ClientHeight    =   8010
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   13035
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8010
   ScaleWidth      =   13035
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton CEResponseUnSub 
      Caption         =   "UnSub response"
      Height          =   375
      Left            =   5640
      TabIndex        =   25
      Top             =   1800
      Width           =   1335
   End
   Begin VB.CommandButton CEResponseSub 
      Caption         =   "Sub response"
      Height          =   375
      Left            =   5640
      TabIndex        =   24
      Top             =   1320
      Width           =   1335
   End
   Begin VB.CommandButton CERequest 
      Caption         =   "CERequest"
      Height          =   495
      Left            =   4080
      TabIndex        =   23
      Top             =   1440
      Width           =   975
   End
   Begin VB.CommandButton btnChkHHReqStatus 
      Caption         =   "Get HHReq Status"
      Height          =   495
      Left            =   480
      TabIndex        =   22
      Top             =   1320
      Width           =   1815
   End
   Begin VB.Frame fTrade 
      Caption         =   "Trade"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000C0&
      Height          =   1215
      Left            =   10080
      TabIndex        =   17
      Top             =   0
      Width           =   2895
      Begin VB.CommandButton btnUnsubTrade 
         Caption         =   "Unsub"
         Height          =   375
         Left            =   120
         TabIndex        =   20
         Top             =   720
         Width           =   735
      End
      Begin VB.CommandButton btnSubTrade 
         Caption         =   "Sub"
         Height          =   375
         Left            =   120
         TabIndex        =   19
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton btnPubTrade 
         Caption         =   "Pub"
         Height          =   735
         Left            =   1920
         TabIndex        =   18
         Top             =   360
         Width           =   855
      End
   End
   Begin VB.Frame fIncoming 
      Caption         =   "Incoming"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   6015
      Left            =   0
      TabIndex        =   15
      Top             =   1920
      Width           =   12975
      Begin VB.CommandButton btnClear 
         Caption         =   "Clear"
         Height          =   315
         Left            =   1560
         TabIndex        =   21
         Top             =   0
         Width           =   735
      End
      Begin VB.ListBox lstMsg 
         Height          =   6105
         ItemData        =   "Form1.frx":0000
         Left            =   0
         List            =   "Form1.frx":0002
         TabIndex        =   16
         Top             =   360
         Width           =   12735
      End
   End
   Begin VB.Frame fSymbol 
      Caption         =   "Symbol"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   1215
      Left            =   0
      TabIndex        =   8
      Top             =   0
      Width           =   3975
      Begin VB.TextBox eCurrency 
         Height          =   285
         Left            =   2760
         TabIndex        =   13
         Text            =   "USD"
         Top             =   720
         Width           =   975
      End
      Begin VB.TextBox eExchange 
         Height          =   285
         Left            =   1680
         TabIndex        =   11
         Text            =   "NASD"
         Top             =   720
         Width           =   975
      End
      Begin VB.TextBox eSymbolName 
         Height          =   285
         Left            =   240
         TabIndex        =   9
         Text            =   "MSFT"
         Top             =   720
         Width           =   1335
      End
      Begin VB.Label Label3 
         Caption         =   "Currency"
         Height          =   255
         Left            =   2760
         TabIndex        =   14
         Top             =   480
         Width           =   855
      End
      Begin VB.Label Label2 
         Caption         =   "Exchange"
         Height          =   255
         Left            =   1680
         TabIndex        =   12
         Top             =   480
         Width           =   855
      End
      Begin VB.Label Label1 
         Caption         =   "Symbol name"
         Height          =   255
         Left            =   240
         TabIndex        =   10
         Top             =   480
         Width           =   1095
      End
   End
   Begin VB.Frame fVola 
      Caption         =   "Volatility"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H0000C000&
      Height          =   1215
      Left            =   7080
      TabIndex        =   4
      Top             =   0
      Width           =   2895
      Begin VB.CommandButton btnPubVola 
         Caption         =   "Pub"
         Height          =   735
         Left            =   1920
         TabIndex        =   7
         Top             =   360
         Width           =   855
      End
      Begin VB.CommandButton btnSubVola 
         Caption         =   "Sub"
         Height          =   375
         Left            =   120
         TabIndex        =   6
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton btnUnsubVola 
         Caption         =   "Unsub"
         Height          =   375
         Left            =   120
         TabIndex        =   5
         Top             =   720
         Width           =   735
      End
   End
   Begin VB.Frame fQuote 
      Caption         =   "Quote"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   1215
      Left            =   4080
      TabIndex        =   0
      Top             =   0
      Width           =   2895
      Begin VB.CommandButton btnPubQuote 
         Caption         =   "Pub"
         Height          =   735
         Left            =   1920
         TabIndex        =   3
         Top             =   360
         Width           =   855
      End
      Begin VB.CommandButton btnSubQuote 
         Caption         =   "Sub"
         Height          =   375
         Left            =   120
         TabIndex        =   2
         Top             =   360
         Width           =   735
      End
      Begin VB.CommandButton btnUnsubQuote 
         Caption         =   "Unsub"
         Height          =   375
         Left            =   120
         TabIndex        =   1
         Top             =   720
         Width           =   735
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private sClUID As String
Private sRespUID As String
Private nSeq As Long

Dim WithEvents SubMan As MMWLib.MsgManager
Attribute SubMan.VB_VarHelpID = -1
Dim PubMan As MMWLib.IPubManager

Dim nQuoteSequence As Integer
Dim nTradeSequence As Integer
Dim nVolaSequence As Integer

Dim nQuoteRecieve As Integer
Dim nTradeRecieve As Integer
Dim nVolaRecieve As Integer

Dim IsLogon As Boolean

Public Function GenerateUID() As String
    On Error Resume Next
    Dim aGuid As GUID, S$, i&, sUID$

    CoCreateGuid aGuid

    S = Hex$(aGuid.Data1)
    sUID = String$(8 - Len(S), "0") & S
    S = Hex$(aGuid.Data2)
    sUID = sUID & String$(4 - Len(S), "0") & S
    S = Hex$(aGuid.Data3)
    sUID = sUID & String$(4 - Len(S), "0") & S

    For i = 0 To 7
        S = Hex$(aGuid.Data4(i))
        sUID = sUID & String$(2 - Len(S), "0") & S
    Next
    GenerateUID = sUID
End Function

Private Sub Log(ByVal Msg As String)
    Dim f As Long
    f = FreeFile
    Open "MMWTestLog.txt" For Append As #f
    Print #f, Msg & Chr(13) & Chr(10)
    Close #f
End Sub


Private Sub btnChkHHReqStatus_Click()
    On Error GoTo ErrHandler

    PubMan.GetHHRequestStatus
    
    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub CERequest_Click()
    Dim req As New MSGSTRUCTLib.CERequest
    sClUID = GenerateUID()
    
    req.UID = sClUID
    req.UndID = CLng(eSymbolName)
    req.CalcIV = enMsgCalcIvNone
    req.CalcModel = enMsgCmBinomialOpt
    req.CalcSteps = 100
    req.GreeksMask = &HFFFFFFFF
    req.IsRealtime = True
    req.Today = 0
    req.UseLastPrice = True
    req.UseTheoNoBid = False
    req.UseTheoVola = True
    
    PubMan.PubCERequest req
    
End Sub



Private Sub CEResponseSub_Click()
    SubMan.SubCEResponse (sClUID)
End Sub

Private Sub CEResponseUnSub_Click()
    SubMan.UnsubCEResponse (sClUID)
End Sub

Private Sub Form_Load()
    On Error GoTo ErrHandler
    
    Form1.Caption = "MMW Test - Router Server OFFLINE"
    
    IsLogon = False
    nTradeSequence = 1
    nVolaSequence = 1
    nQuoteSequence = 1
    
    nQuoteRecieve = 52
    nTradeRecieve = 1
    nVolaRecieve = 1
    
    Set SubMan = New MMWLib.MsgManager
    Set PubMan = SubMan
    
    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error GoTo ErrHandler
    
    Set SubMan = Nothing
    Set PubMan = Nothing
    
    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnClear_Click()
    lstMsg.Clear
End Sub

Private Sub FillSymbol(Symbol As MSGSTRUCTLib.SymbolObject)
    On Error GoTo ErrHandler
    
    Symbol.Set enStStock, eSymbolName.Text, eExchange.Text, eCurrency.Text

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Quote
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''



Private Sub btnSubQuote_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.SubQuote Symbol
    
    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnUnsubQuote_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.UnsubQuote Symbol

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnPubQuote_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    Dim QuoteMsg As New MSGSTRUCTLib.Quote

    FillSymbol Symbol
    
    QuoteMsg.AskPrice = 51 + nQuoteSequence
    QuoteMsg.AskSize = 1000
    QuoteMsg.BidPrice = 49 + nQuoteSequence
    QuoteMsg.BidSize = 2000
    QuoteMsg.LastPrice = 50 + nQuoteSequence
    
    PubMan.PubQuote Symbol, QuoteMsg
    
    nQuoteSequence = nQuoteSequence + 1

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub




Private Sub SubMan_OnCEResponse(pResponses() As MSGSTRUCTLib.ICEResponse)
    Dim nCount&, nInd&
    
    On Error GoTo ErrHandler
    nCount = UBound(pResponses) - LBound(pResponses) + 1
    
    If nCount > 0 Then
            
        LockWindowUpdate lstMsg.hWnd
        
        For nInd = LBound(pResponses) To UBound(pResponses)
            lstMsg.AddItem "[ RESPONE " & _
                    "ContractID = (" & pResponses(nInd).ContractID & ") " & _
                    "PriceBid = (" & pResponses(nInd).PriceBid & " )" & _
                    "TheoPrice = (" & pResponses(nInd).PriceTheo & " )" & _
                    "Delta = (" & pResponses(nInd).Delta & " )" & _
                    "Gamma = (" & pResponses(nInd).Gamma & " )]", 0
        Next
        
        LockWindowUpdate 0
        lstMsg.Refresh
        DoEvents
        
    End If
                    
    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description

End Sub

Private Sub SubMan_OnHHRequestStatus(ByVal lNumSubscribers As Long)
 
    lstMsg.AddItem "[Num of HHReq Subscribers Hosts is " & lNumSubscribers & " ]"
End Sub

Private Sub SubMan_OnLogoff(ByVal bsRouterName As String)
    IsLogon = False
    Form1.Caption = "MMW Test - Router Server OFFLINE"
End Sub

Private Sub SubMan_OnLogon(ByVal bsRouterName As String)
    IsLogon = True
    Form1.Caption = "MMW Test - Router Server (" & bsRouterName & ") ONLINE"
End Sub

Private Sub SubMan_OnQuote(ByVal Symbol As MSGSTRUCTLib.ISymbolObject, ByVal Data As MSGSTRUCTLib.IQuote)
    On Error GoTo ErrHandler
    
    lstMsg.AddItem "[ QUOTE/" & Symbol.GetString & "]    " & _
                    "Ask = (" & Data.AskPrice & ", " & Data.AskSize & ") " & _
                    "Bid = (" & Data.BidPrice & ", " & Data.BidSize & ") " & _
                    "Last = " & Data.LastPrice, 0
                    
    If nQuoteRecieve <> Data.AskPrice Then
        Log "Unexpected quote message !!!"
        nQuoteRecieve = Data.AskPrice
    End If
    
    nQuoteRecieve = nQuoteRecieve + 1
    Log "Client recieve quote. Ask Price : (" & Data.AskPrice & ")"

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Volatility
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''


Private Sub btnSubVola_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.SubVola Symbol

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnUnsubVola_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.UnsubVola Symbol

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnPubVola_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    Dim VolaMsg As New MSGSTRUCTLib.VMESurface
    
    
    FillSymbol Symbol
    
    VolaMsg.DiscreteAcceleration = True
    VolaMsg.InterpolationFactor = 1
    VolaMsg.OptType = 3
    VolaMsg.PriceOverride = 0
    VolaMsg.SmileAccelerator = 1
    VolaMsg.SurfaceID = nVolaSequence
    VolaMsg.UnderlinePrice = 50
    
    PubMan.PubVola Symbol, VolaMsg
    nVolaSequence = nVolaSequence + 1

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub SubMan_OnVola(ByVal Symbol As MSGSTRUCTLib.ISymbolObject, ByVal Data As MSGSTRUCTLib.IVMESurface)
    On Error GoTo ErrHandler
    
    lstMsg.AddItem "[ VOLATILITY/" & Symbol.GetString & "] " & _
                    ", S_ID: " & Data.SurfaceID & _
                    ", DA: " & Data.DiscreteAcceleration & _
                    ", IF: " & Data.InterpolationFactor & _
                    ", OT: " & Data.OptType & _
                    ", PO: " & Data.PriceOverride & _
                    ", SA: " & Data.SmileAccelerator & _
                    ", UP: " & Data.UnderlinePrice, 0

    If nVolaRecieve <> Data.SurfaceID Then
        Log "Unexpected volatility message !!!"
        nVolaRecieve = Data.SurfaceID
    End If
    
    nVolaRecieve = nVolaRecieve + 1
    Log "Client recieve vola. S_ID : (" & Data.SurfaceID & ")"

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Trade
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''

Private Sub btnSubTrade_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.SubTradeUpdate

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnUnsubTrade_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    FillSymbol Symbol
    SubMan.UnsubTradeUpdate

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub btnPubTrade_Click()
    On Error GoTo ErrHandler
    
    Dim Symbol As New MSGSTRUCTLib.SymbolObject
    Dim aMsg As New MSGSTRUCTLib.TradeUpdate

    FillSymbol Symbol
    
    aMsg.SeqNum = nTradeSequence
    aMsg.TradeID = 1
    aMsg.Quantity = 1000
    aMsg.Price = 25.6
    aMsg.IsBuy = True
    aMsg.TraderID = 2
    aMsg.BookID = 3
    aMsg.StrategyID = 4
    aMsg.BrokerID = 5
    aMsg.BrokerCommission = 0.1
    aMsg.ClearingBrokerID = 6
    aMsg.ClearingBrokerCommission = 0.05
    aMsg.SpotReference = 73.9
    aMsg.TradedIV = 0.67
    
    PubMan.PubTradeUpdate aMsg
    nTradeSequence = nTradeSequence + 1

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

Private Sub SubMan_OnTradeUpdate(ByVal Data As MSGSTRUCTLib.ITradeUpdate)
    On Error GoTo ErrHandler
    
    lstMsg.AddItem "[ TRADE ]" & _
                    ", SN: " & Data.SeqNum & _
                    ", T_ID: " & Data.TradeID & _
                    ", Q: " & Data.Quantity & _
                    ", P: " & Data.Price & _
                    ", IB: " & Data.IsBuy & _
                    ", TR_ID: " & Data.TraderID & _
                    ", B_ID: " & Data.BookID & _
                    ", S_ID: " & Data.StrategyID & _
                    ", BRID: " & Data.BrokerID & _
                    ", BC: " & Data.BrokerCommission & _
                    ", CB_ID: " & Data.ClearingBrokerID & _
                    ", CBC: " & Data.ClearingBrokerCommission & _
                    ", SR: " & Data.SpotReference & _
                    ", TIV: " & Data.TradedIV, 0
    
    If nTradeRecieve <> Data.SeqNum Then
        Log "Unexpected trade message !!!"
        nTradeRecieve = Data.SeqNum
    End If
    
    nTradeRecieve = nTradeRecieve + 1
    Log "Client recieve trade. SN : (" & Data.SeqNum & ")"

    Exit Sub
ErrHandler:
    MsgBox "Error: " & Err.Description
End Sub

