VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IDataService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Enum TERMS
    TERM_NONE = 0
    TERM_1
    TERM_30
    TERM_60
    TERM_90
    TERM_180
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadSupportedStockSymbols() As ADODB.Recordset
    ' Abstract
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub WriteSupportedStockSymbols(rsSupportedStockSymbols As ADODB.Recordset)
    ' Abstract
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadPrices(Optional rsSymbols As ADODB.Recordset) As ADODB.Recordset
    ' Abstract
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub WritePrices(rsSymbolPrices As ADODB.Recordset)
    ' Abstract
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadVariations _
( _
    rsIndexHV As ADODB.Recordset, _
    rsStockHV As ADODB.Recordset, _
    rsStockStockCorr As ADODB.Recordset, _
    rsStockIndexCorr As ADODB.Recordset, _
    Optional rsSymbols As ADODB.Recordset, _
    Optional ByVal Term As TERMS = TERM_30 _
) As Boolean
    ' Abstract
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub WriteVariations _
( _
    rsIndexHV As ADODB.Recordset, _
    rsStockHV As ADODB.Recordset, _
    rsStockStockCorr As ADODB.Recordset, _
    rsStockIndexCorr As ADODB.Recordset, _
    Optional rsSymbols As ADODB.Recordset, _
    Optional ByVal Term As TERMS = TERM_30 _
)
    ' Abstract
End Sub

