VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvSynthGreeksAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ContractID As Long
Public ContractType As Long
Public Symbol As String
Public SynthUndID As Long
Public SynthUndSymbol As String
Public IsTotal As Boolean

Public Visible As Boolean

Public DeliveryUnits As Double
    
Public DeltaInShares As Double
Public GammaInShares As Double
Public GammaInSharesPerc As Double
Public DeltaInMoney As Double
Public BetaWtdDeltaInMoney As Double
    
Public BadDelta As Boolean
Public BadGamma As Boolean
Public BadGammaPerc As Boolean
Public BadDeltaInMoney As Boolean
Public BadBetaWtdDeltaInMoney As Boolean
    
Private Sub Class_Initialize()
    DeliveryUnits = BAD_LONG_VALUE
    
    ClearValuesInShares
End Sub

Public Sub ClearValuesInShares()

    DeltaInShares = BAD_DOUBLE_VALUE
    GammaInShares = BAD_DOUBLE_VALUE
    GammaInSharesPerc = BAD_DOUBLE_VALUE
    DeltaInMoney = BAD_DOUBLE_VALUE
    BetaWtdDeltaInMoney = BAD_DOUBLE_VALUE
    
    BadDelta = False
    BadGamma = False
    BadGammaPerc = False
    BadDeltaInMoney = False
    BadBetaWtdDeltaInMoney = False
    
End Sub

