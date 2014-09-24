Attribute VB_Name = "basQuatedSymbols"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Const QUOTE = "'"
Public Const QUOTE2 = "''"
Public Const DOUBLE_QUOTE = """"

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function QuotedSymbol(ByVal Symbol As String) As String
    InValidateStockSymbol Symbol
    If Left(Symbol, 1) <> QUOTE And Right(Symbol, 1) <> QUOTE Then
        QuotedSymbol = QUOTE & Symbol & QUOTE
    Else
        QuotedSymbol = Symbol
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Converts Symbol to form acceptable by ADO.Recrdset functions
'
Public Function ValidateStockSymbol(StockSymbol As String) As Boolean
Dim BadSymbols As String
Dim i As Long
    BadSymbols = ",./\"
    StockSymbol = UCase(StockSymbol)
    For i = 1 To Len(BadSymbols)
        StockSymbol = Replace(StockSymbol, Mid$(BadSymbols, i, 1), "")
    Next
    ValidateStockSymbol = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Converts Symbol to form acceptable by IVolatility ASP's
' AGR'A -> AGR''A
'
Public Function InValidateStockSymbol(StockSymbol As String) As Boolean
    StockSymbol = UCase(StockSymbol)
    StockSymbol = Replace(StockSymbol, QUOTE, QUOTE2)
    InValidateStockSymbol = True
End Function

