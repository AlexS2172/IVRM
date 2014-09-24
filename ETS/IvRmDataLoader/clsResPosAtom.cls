VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRecPosAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' position reconciliation atom
Option Explicit

'Public Opt As clsRecPosOptAtom
Public Und As clsRecPosUndAtom
'Public Trader As clsRecPosTraderAtom

'Public FutOpt As clsRecPosFutOptAtom
'Public Fut As clsRecPosFutAtom

Public PosValidOld As Boolean
Public QtyOld As Long
Public PriceOld As Double
Public LotSizeOld As Long
Public StrategyOldID&
Public StrategyOld As String

Public PosValidNew As Boolean
Public QtyNew As Long
Public PriceNew As Double
Public LotSizeNew As Long
Public StrategyNewID&
Public StrategyNew As String

Public StrategyForTradeID&
Public StrategyForTrade$

Public TradeDate As Date

Public UndSymbol As String

Public bFix As Boolean
Public bFixPrice As Boolean

Public ImportSymbol As String


