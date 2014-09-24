Attribute VB_Name = "basMatrix1"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Linear equations
' Matrix invertion
' Matrix determinant calculation
' Matrix by vector multiplication
' Matrix and vector handling
'
Option Explicit

Public Const DumpFormat = "0.00"

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearVector(Vector() As Double, ByVal n As Long, Optional ByVal Value As Double = 0)
Dim i As Long
    For i = 1 To n
        Vector(i) = Value
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearMatrix(Matrix() As Double, ByVal n As Long, Optional ByVal Value As Double = 0)
Dim i As Long
Dim j As Long
    For i = 1 To n
        For j = 1 To n
            Matrix(i, j) = Value
        Next
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FillVector(Vector() As Double, ByVal n As Long, ParamArray Value()) As Variant
Dim i As Long
    For i = 1 To n
        Vector(i) = Value(i - 1)
    Next
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub FillMatrixCol(Matrix() As Double, ByVal n As Long, ByVal Col As Long, ParamArray Vector())
Dim i As Long
    For i = 1 To n
        Matrix(i, Col) = Vector(i - 1)
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub FillMatrixRow(Matrix() As Double, ByVal n As Long, ByVal Row As Long, ParamArray Vector())
Dim i As Long
    For i = 1 To n
        Matrix(Row, i) = Vector(i - 1)
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MultiplyMatrixByVector(Matrix() As Double, Vector() As Double, ByVal n As Long) As Variant
Dim i As Long
Dim j As Long
Dim V() As Double
    ReDim V(1 To n) As Double
    For i = 1 To n
        V(i) = 0
        For j = 1 To n
            V(i) = V(i) + Matrix(i, j) * Vector(j)
        Next
    Next
    MultiplyMatrixByVector = V
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function InverseMatrix(Matrix() As Double, ByVal n As Long) As Variant
Dim det As Double
Dim M() As Double
Dim i As Long
Dim j As Long
Dim f As Boolean
Dim g As Boolean
    If n > 0 Then
        ReDim M(1 To n, 1 To n) As Double
        det = MatrixDet(Matrix, n)
        If det = 0 Then
            Err.Raise vbObjectError, "Matrix algebra library", "Matrix invertion failed. Matrix determinat is zero."
        Else
            If n = 1 Then
                M(1, 1) = 1 / det
            Else
                f = True
                For i = 1 To n
                    g = f
                    For j = 1 To n
                        If g Then
                            M(i, j) = MatrixSubDet(Matrix, n, i, j) / det
                        Else
                            M(i, j) = -MatrixSubDet(Matrix, n, i, j) / det
                        End If
                        g = Not g
                    Next
                    f = Not f
                Next
            End If
        End If
    Else
        '
    End If
    InverseMatrix = M
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MatrixDet(Matrix() As Double, ByVal n As Long) As Double
Dim d As Double
Dim r As Double
Dim i As Long
Dim f As Boolean
    Select Case n
    Case 0
        MatrixDet = 1
    Case 1
        MatrixDet = Matrix(1, 1)
    Case 2
        MatrixDet = MatrixDet2x2(Matrix)
    Case Is > 2
        d = 0
        f = True
        For i = 1 To n
            r = Matrix(1, i) * MatrixSubDet(Matrix, n, 1, i)
            If f Then
                d = d + r
            Else
                d = d - r
            End If
            f = Not f
        Next
        MatrixDet = d
    Case Else
        Err.Raise vbObjectError, "Matrix algebra library", "Matrix determinat calculation failed. Matrix dimention less then 1."
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MatrixDet2x2(Matrix() As Double) As Double
    MatrixDet2x2 = Matrix(1, 1) * Matrix(2, 2) - Matrix(1, 2) * Matrix(2, 1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MatrixSubDet(Matrix() As Double, ByVal n As Long, ByVal r As Long, ByVal c As Long) As Double
Dim M() As Double
    If n > 1 Then
        M = SubMatrix(Matrix, n, r, c)
        MatrixSubDet = MatrixDet(M, n - 1)
    Else
        MatrixSubDet = 1
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SubMatrix(Matrix() As Double, ByVal n As Long, ByVal r As Long, ByVal c As Long) As Variant
Dim i As Long
Dim j As Long
Dim k As Long
Dim l As Long
Dim M() As Double
    If n > 1 Then
        ReDim M(1 To n - 1, 1 To n - 1) As Double
        k = 0
        For i = 1 To n
            If i <> r Then
                k = k + 1
                l = 0
                For j = 1 To n
                    If j <> c Then
                        l = l + 1
                        M(k, l) = Matrix(i, j)
                    End If
                Next
            End If
        Next
    End If
    SubMatrix = M
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DumpMatrix(Matrix() As Double, ByVal n As Long)
Dim i As Long
Dim j As Long
    For i = 1 To n
        For j = 1 To n
            Debug.Print Format$(Matrix(i, j), DumpFormat),
        Next
        Debug.Print
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Solve Matrix
'
Public Function SolveMatrix(Matrix() As Double, Vector() As Double, ByVal n As Long) As Variant
Dim M() As Double
    M = InverseMatrix(Matrix, n)
    SolveMatrix = MultiplyMatrixByVector(M, Vector, n)
End Function

