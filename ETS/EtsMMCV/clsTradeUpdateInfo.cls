VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeUpdateInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public TradeID As Long
Public SeqNum As Long

Public ContractID As Long
Public ContractTypeID As Long
Public Symbol As String
Public ContractName As String

Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double
Public PriceClose As Double
Public PriceTheoClose As Double
Public LotSize As Long

Public UndContractID As Long
Public UndContractTypeID As Long
Public UndSymbol As String

Public UndPriceBid As Double
Public UndPriceAsk As Double
Public UndPriceLast As Double
Public UndPriceClose As Double
Public UndLotSize As Long

Public Expiry As Date
Public OptType As Long
Public Strike As Double
Public OptionRootID As Long

Public DivFreq As Long
Public DivDate As Date
Public DivAmt As Double
Public Yield As Double

Public TraderID As Long
Public BookID As Long
Public StrategyID As Long
Public BrokerID As Long
Public ClearingBrokerID As Long

Public Price As Double
Public Quantity As Long
Public IsBuy As Byte
Public TradeDate As Date
Public SpotReference As Double
Public TradedIV As Double
Public IsPosition As Byte
Public Status As TRADE_STATUS

Public TotalPrice As Double
Public BrokerCommissionID As Long
Public BrokerCommission As Double
Public ClearingBrokerCommissionID As Long
Public ClearingBrokerCommission As Double
Public TotalCommission As Double

Public IsSynthetic As Boolean
Public CashValue As Double
Public SynthUndPrice As clsTcUndPriceColl

Public Sub CopyTo(aDest As clsTradeUpdateInfo)
    On Error Resume Next
    aDest.TradeID = TradeID
    aDest.SeqNum = SeqNum
    
    aDest.ContractID = ContractID
    aDest.ContractTypeID = ContractTypeID
    aDest.Symbol = Symbol
    
    aDest.PriceBid = PriceBid
    aDest.PriceAsk = PriceAsk
    aDest.PriceLast = PriceLast
    aDest.PriceClose = PriceClose
    aDest.PriceTheoClose = PriceTheoClose
    aDest.LotSize = LotSize
    
    aDest.UndContractID = UndContractID
    aDest.UndContractTypeID = UndContractTypeID
    aDest.UndSymbol = UndSymbol
    
    aDest.UndPriceBid = UndPriceBid
    aDest.UndPriceAsk = UndPriceAsk
    aDest.UndPriceLast = UndPriceLast
    aDest.UndPriceClose = UndPriceClose
    aDest.UndLotSize = UndLotSize
    
    aDest.Expiry = Expiry
    aDest.OptType = OptType
    aDest.Strike = Strike
    aDest.OptionRootID = OptionRootID
    
    aDest.DivFreq = DivFreq
    aDest.DivDate = DivDate
    aDest.DivAmt = DivAmt
    aDest.Yield = Yield
    
    aDest.TraderID = TraderID
    aDest.BookID = BookID
    aDest.StrategyID = StrategyID
    aDest.BrokerID = BrokerID
    aDest.ClearingBrokerID = ClearingBrokerID
    
    aDest.Price = Price
    aDest.Quantity = Quantity
    aDest.IsBuy = IsBuy
    aDest.TradeDate = TradeDate
    aDest.SpotReference = SpotReference
    aDest.TradedIV = TradedIV
    aDest.IsPosition = IsPosition
    aDest.Status = Status
    
    aDest.TotalPrice = TotalPrice
    aDest.BrokerCommissionID = BrokerCommissionID
    aDest.BrokerCommission = BrokerCommission
    aDest.ClearingBrokerCommissionID = ClearingBrokerCommissionID
    aDest.ClearingBrokerCommission = ClearingBrokerCommission
    aDest.TotalCommission = TotalCommission
    
    aDest.IsSynthetic = IsSynthetic
    aDest.CashValue = CashValue
    
    CopySynthPrices aDest
    
End Sub

Public Sub CopySynthPrices(aDest As clsTradeUpdateInfo)
    Dim aPrice As clsTcUndPriceAtom, aPrice2 As clsTcUndPriceAtom
    On Error Resume Next
    
    If Not IsSynthetic Then Exit Sub
    
    If Not SynthUndPrice Is Nothing Then
        If SynthUndPrice.Count Then
            If Not aDest.SynthUndPrice Is Nothing Then aDest.SynthUndPrice = Nothing
            Set aDest.SynthUndPrice = New clsTcUndPriceColl
            For Each aPrice In SynthUndPrice
                Set aPrice2 = aDest.SynthUndPrice.Add(CStr(aPrice.UndTypeID) & "_" & aPrice.Symbol)
                aPrice2.UndID = aPrice.UndID
                aPrice2.UndTypeID = aPrice.UndTypeID
                aPrice2.Symbol = aPrice.Symbol
                aPrice2.PriceAsk = aPrice.PriceAsk
                aPrice2.PriceBid = aPrice.PriceBid
                aPrice2.PriceLast = aPrice.PriceLast
                aPrice2.PriceClose = aPrice.PriceClose
                aPrice2.Weight = aPrice.Weight
            Next
        End If
    End If
End Sub

Public Function GetTradeTypeString() As String
    On Error Resume Next
    Select Case Status
        Case enTsMatchedRealtime, enTsRealtime
            GetTradeTypeString = "Actual"
        
        Case enTsManual
            GetTradeTypeString = "Manual"
            
        Case enTsSimulated
            GetTradeTypeString = "Simulated"
        
        Case Else
            GetTradeTypeString = "<Unknown>"
    End Select
End Function

Public Function GetTradeMatchedString() As String
    On Error Resume Next
    Select Case Status
        Case enTsMatchedRealtime
            GetTradeMatchedString = "Yes"
        
        Case enTsRealtime
            GetTradeMatchedString = "No"
            
        Case Else
            GetTradeMatchedString = ""
    End Select
End Function


