VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CStockStockCorrelator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Private CorrMatrix() As Single
Private Symbols As New Collection

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    On Error Resume Next
    Clear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    On Error Resume Next
    Clear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
    Set Symbols = New Collection
    ReDim CorrMatrix(0) As Single
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub AddStock(ByVal Symbol As String)
    On Error Resume Next
    With Symbols
        .Add CLng(.Count + 1), Symbol
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get SymbolID(ByVal Symbol As String) As Long
    On Error Resume Next
    SymbolID = Symbols.Item(Symbol)
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get Count() As Long
    Count = Symbols.Count
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Get MatrixSize(ByVal MatrixSide As Long) As Long
    MatrixSize = (MatrixSide * (MatrixSide + 1)) / 2
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub AllocateMemory()
Dim MatrixSize1 As Long
    MatrixSize1 = MatrixSize(Count)
    ReDim CorrMatrix(MatrixSize1) As Single
'    Debug.Print "Matrix memory allocation"
'    Debug.Print "Matrix Side", Count
'    Debug.Print "Matrix Size", MatrixSize1
'    Debug.Print "Memory, KB", MatrixSize1 * 4 \ 1024
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MatrixIndex(ByVal Index1 As Long, ByVal Index2 As Long) As Long
    If Index1 > Index2 Then
        MatrixIndex = (Index1 * (Index1 - 1)) \ 2 + Index2
    Else
        MatrixIndex = (Index2 * (Index2 - 1)) \ 2 + Index1
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let StockStockCorr(ByVal Symbol1 As String, ByVal Symbol2 As String, ByVal NewValue As Double)
    dStockStockCorr(SymbolID(Symbol1), SymbolID(Symbol2)) = NewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get StockStockCorr(ByVal Symbol1 As String, ByVal Symbol2 As String) As Double
    StockStockCorr = dStockStockCorr(SymbolID(Symbol1), SymbolID(Symbol2))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let dStockStockCorr(ByVal SymbolID1 As Long, ByVal SymbolID2 As Long, ByVal NewValue As Double)
    CorrMatrix(MatrixIndex(SymbolID1, SymbolID2)) = NewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get dStockStockCorr(ByVal SymbolID1 As Long, ByVal SymbolID2 As Long) As Double
    dStockStockCorr = CorrMatrix(MatrixIndex(SymbolID1, SymbolID2))
End Property

