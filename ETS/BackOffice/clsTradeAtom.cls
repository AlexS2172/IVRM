VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Public Und As clsRecPosUndAtom
'Public Fut As clsRecPosFutAtom
'Public Opt As clsRecPosOptAtom
'Public FutOpt As clsRecPosFutOptAtom
'Public Trader As clsRecPosTraderAtom

Public ContractID As Long
Public TraderID As Long
Public StrategyID As Long
Public BrokerID As Long
Public UnderlyingID As Long
Public OptionRootID As Long
Public FutureID As Long
Public FutureRootID As Long

Public Symbol As String
Public SeqNum As Long
Public ContractType As EtsContractTypeEnum
Public TradeID As Long
Public StrategyName As String
Public IsBuy As Boolean
Public IsPos As Boolean
Public Qty As Long
Public RealQty As Long
Public LotSize As Long
Public Price As Double
Public ImportID As String
Public TradeDate As Date
Public ActionDate As Date
Public Comission As Double
Public BrokerName As String
Public TraderName As String
Public CCY As String

Public Strike As Double
Public IsCall As Boolean
Public Expiry As Date
Public RootName As String
Public ExchangeID As Long

Public Function GetOCCCode() As String
    If (ContractType = enCtFutOption Or ContractType = enCtOption) Then
        GetOCCCode = RootName & Format(Expiry, " MMM YY ") & Format(Strike, "#.0##") & " " & IIf(IsCall, "C", "P")
    Else
        GetOCCCode = Symbol
    End If
End Function
