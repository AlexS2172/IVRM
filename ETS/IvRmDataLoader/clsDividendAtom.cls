VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDividendAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public StockSymbol As String
Public DividendDate As Date
Public DividendAmount As Double


Public Sub Clear()
    StockSymbol = ""
    DividendDate = CDate(0)
    DividendAmount = BAD_DOUBLE_VALUE
End Sub
