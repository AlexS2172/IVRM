VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVmOptAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ID As Long
Public Symbol As String
Public Expiry As Date
Public OptType As Long
Public Strike As Double
Public Vola As Double
Public TargVola As Double
Public LotSize As Long

Public ExpiryOV As Date
Public TradingClose As Date

Public Exp As clsVmExpiryAtom

Public QtyInShares As Long
Public Qty As Long

Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double

Public IV As Double
Public Delta As Double
Public Gamma As Double
Public Vega As Double
Public Theta As Double
Public Rho As Double
Public RootID As Long

Private Sub Class_Initialize()
    Qty = BAD_LONG_VALUE
    QtyInShares = BAD_LONG_VALUE
    PriceBid = BAD_DOUBLE_VALUE
    PriceAsk = BAD_DOUBLE_VALUE
    PriceLast = BAD_DOUBLE_VALUE
    IV = BAD_DOUBLE_VALUE
    Vola = BAD_DOUBLE_VALUE
    ID = BAD_LONG_VALUE
    ClearValues
End Sub

Public Sub ClearValues()
    IV = BAD_DOUBLE_VALUE
    Delta = BAD_DOUBLE_VALUE
    Gamma = BAD_DOUBLE_VALUE
    Vega = BAD_DOUBLE_VALUE
    Theta = BAD_DOUBLE_VALUE
    Rho = BAD_DOUBLE_VALUE
    Vola = BAD_DOUBLE_VALUE
End Sub
