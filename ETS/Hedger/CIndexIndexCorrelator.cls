VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "CIndexIndexCorrelator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Private Const THIS = "CIndexIndexCorrelator"

Private Const MatrixSize = 4

Private dIndexPrice(1 To MatrixSize) As Double
Private dIndexHV(1 To MatrixSize) As Double
Private dIndexIndexCorr(1 To MatrixSize, 1 To MatrixSize) As Double
Private bIndexSelected(1 To MatrixSize) As Boolean

Private CMatrix(1 To MatrixSize, 1 To MatrixSize) As Double
Private IMatrix() As Double
Private CVector(1 To MatrixSize) As Double
Private XVector() As Double
Private QVector(1 To MatrixSize) As Double
Private dDP As Double
Private dD As Double
Private dQ As Double
Private dGain As Double
Private fSolveMatrix As Boolean

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub Class_Initialize()
'    On Error GoTo Herr
'    fSolveMatrix = False
'    Exit Sub
'Herr:
'    Debug.Print Err.Description
'    Debug.Assert False
'    Resume Next
'End Sub
'
''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub Class_Terminate()
'    On Error Resume Next
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexCount() As Long
    IndexCount = MatrixSize
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexSymbol(ByVal ID As Long) As String
    Select Case ID
    Case 1: IndexSymbol = "SPX"
    Case 2: IndexSymbol = "NDX"
    Case 4: IndexSymbol = "OEX"
    Case 3: IndexSymbol = "DJX"
    Case Else
        Err.Raise vbObjectError, THIS, "Property Get IndexSymbol failed: ID(" & ID & ") is out of range."
    End Select
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexID(ByVal Symbol As String) As Long
    Select Case UCase(Symbol)
    Case "SPX": IndexID = 1
    Case "NDX": IndexID = 2
    Case "OEX": IndexID = 3
    Case "DJX": IndexID = 4
    Case Else
        Err.Raise vbObjectError, THIS, "Property Get IndexID failed: Symbol(" & Symbol & ") is not supported."
    End Select
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexSelected(ByVal Symbol As String) As Boolean
    IndexSelected = bIndexSelected(IndexID(Symbol))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let IndexSelected(ByVal Symbol As String, ByVal vNewValue As Boolean)
    bIndexSelected(IndexID(Symbol)) = vNewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexPrice(ByVal Symbol As String) As Double
    IndexPrice = dIndexPrice(IndexID(Symbol))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let IndexPrice(ByVal Symbol As String, ByVal vNewValue As Double)
    dIndexPrice(IndexID(Symbol)) = vNewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexHV(ByVal Symbol As String) As Double
    IndexHV = dIndexHV(IndexID(Symbol))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let IndexHV(ByVal Symbol As String, ByVal vNewValue As Double)
    dIndexHV(IndexID(Symbol)) = vNewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get IndexIndexCorr(ByVal Symbol1 As String, ByVal Symbol2 As String) As Double
    IndexIndexCorr = dIndexIndexCorr(IndexID(Symbol1), IndexID(Symbol2))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let IndexIndexCorr(ByVal Symbol1 As String, ByVal Symbol2 As String, ByVal vNewValue As Double)
Dim ID1 As Long
Dim ID2 As Long
    ID1 = IndexID(Symbol1)
    ID2 = IndexID(Symbol2)
    If ID1 = ID2 Then
        dIndexIndexCorr(ID1, ID2) = 1
    Else
        If bIndexSelected(ID1) And bIndexSelected(ID2) Then
            dIndexIndexCorr(ID1, ID2) = vNewValue
            dIndexIndexCorr(ID2, ID1) = vNewValue
        Else
            dIndexIndexCorr(ID1, ID2) = 0
            dIndexIndexCorr(ID2, ID1) = 0
        End If
    End If
    fSolveMatrix = False
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let CorPortInd(ByVal Symbol As String, ByVal vNewValue As Double)
    CVector(IndexID(Symbol)) = vNewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let VarPort(ByVal vNewValue As Double)
    dDP = Sqr(vNewValue)
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get d() As Double
    d = dD ' * dDP * dDP
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get Gain() As Double
    Gain = dGain
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get QTotal() As Double
    QTotal = dQ
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get QByIndex(ByVal Symbol As String) As Double
    QByIndex = QVector(IndexID(Symbol))
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CalculateIMatrix() As Boolean
Dim i As Long
Dim j As Long
    ClearMatrix CMatrix, MatrixSize
    For i = 1 To MatrixSize
        CMatrix(i, i) = 1
        For j = i + 1 To MatrixSize
            CMatrix(i, j) = dIndexIndexCorr(i, j)
            CMatrix(j, i) = CMatrix(i, j)
        Next
    Next
    IMatrix = basMatrix1.InverseMatrix(CMatrix, MatrixSize)
    CalculateIMatrix = True
    fSolveMatrix = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CalculateQVector() As Boolean
Dim i As Long
    If Not fSolveMatrix Then
        If Not CalculateIMatrix Then
            Exit Function
        End If
    End If
    XVector = basMatrix1.MultiplyMatrixByVector(IMatrix, CVector, MatrixSize)
    dQ = 0
    dD = 1
    For i = 1 To MatrixSize
        'Debug.Print "i C X H", i, CVector(i), XVector(i), dIndexHV(i)
        dD = dD - CVector(i) * XVector(i)
        If dIndexHV(i) = 0 Then
            QVector(i) = 0
        Else
            QVector(i) = dDP * XVector(i) / dIndexHV(i) * Sqr(252)
        End If
        dQ = dQ + QVector(i)
    Next
    If dD <= 0 Then
        dGain = 1
    Else
        dGain = 1 - Sqr(dD)
    End If
    CalculateQVector = True
End Function

