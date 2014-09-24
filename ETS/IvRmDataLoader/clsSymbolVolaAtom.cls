VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsSymbolVolaAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Symbol As String
Public Expiry As Date
Public Strike As Double
Public Volatility As Double

Public Sub Clear()
    Symbol = ""
    Expiry = CDate(0)
    Strike = BAD_DOUBLE_VALUE
    Volatility = BAD_DOUBLE_VALUE
End Sub

