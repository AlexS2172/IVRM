VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DataService"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Implements IDataService

Private XMLData As XMLDataSets

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set XMLData = New XMLDataSets
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    Set XMLData = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function IDataService_ReadSupportedStockSymbols() As ADODB.Recordset
    Set IDataService_ReadSupportedStockSymbols = _
        XMLData.OpenDataMember(SupportedStockSymbols)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub IDataService_WriteSupportedStockSymbols(rsSupportedStockSymbols As ADODB.Recordset)
    XMLData.SaveDataMember SupportedStockSymbols, rsSupportedStockSymbols
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function IDataService_ReadPrices(Optional rsSymbols As ADODB.Recordset) As ADODB.Recordset
    Set IDataService_ReadPrices = _
        XMLData.OpenDataMember(SymbolPrices)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub IDataService_WritePrices(rsSymbolPrices As ADODB.Recordset)
    XMLData.SaveDataMember SymbolPrices, rsSymbolPrices
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function IDataService_ReadVariations _
( _
    rsIndexHV As ADODB.Recordset, _
    rsStockHV As ADODB.Recordset, _
    rsStockStockCorr As ADODB.Recordset, _
    rsStockIndexCorr As ADODB.Recordset, _
    Optional rsSymbols As ADODB.Recordset, _
    Optional ByVal Term As TERMS = TERM_30 _
) As Boolean
    
    Set rsIndexHV = XMLData.OpenDataMember(IndexHV)
    Set rsStockHV = XMLData.OpenDataMember(StockHV)
    Set rsStockStockCorr = XMLData.OpenDataMember(StockStockCorr)
    Set rsStockIndexCorr = XMLData.OpenDataMember(StockIndexCorr)
    
    IDataService_ReadVariations = True
    
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub IDataService_WriteVariations _
( _
    rsIndexHV As ADODB.Recordset, _
    rsStockHV As ADODB.Recordset, _
    rsStockStockCorr As ADODB.Recordset, _
    rsStockIndexCorr As ADODB.Recordset, _
    Optional rsSymbols As ADODB.Recordset, _
    Optional ByVal Term As TERMS = TERM_30 _
)
    
    XMLData.SaveDataMember IndexHV, rsIndexHV
    XMLData.SaveDataMember StockHV, rsStockHV
    XMLData.SaveDataMember StockStockCorr, rsStockStockCorr
    XMLData.SaveDataMember StockIndexCorr, rsStockIndexCorr

End Sub

