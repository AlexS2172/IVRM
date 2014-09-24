VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPtTraderAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Trader As EtsGeneralLib.EtsTraderAtom
Public Strategy As EtsGeneralLib.EtsStrategyAtom
Public LastTrade As EtsGeneralLib.MmTradeInfoAtom
Public CurQuantity&

Public Function Copy() As clsPtTraderAtom
    Dim atom As clsPtTraderAtom
    Set atom = New clsPtTraderAtom
    
    atom.CurQuantity = CurQuantity
    Set atom.Trader = Trader
    Set atom.Strategy = Strategy
    Set atom.LastTrade = LastTrade
    
    Set Copy = atom
End Function
