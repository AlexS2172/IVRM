Attribute VB_Name = "basMain"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Main()
    On Error GoTo Herr
    ClearLog
    frmISELog1.Execute
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowError(Optional ByVal Title As String = "")
Dim Msg As String
    Msg = Err.Description
    If Len(Title) = 0 Then
        Msg = Title & vbCrLf & Msg
    End If
    If Len(Msg) > 0 Then
        MsgBox Msg, vbCritical
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowWarning(ByVal Msg As String)
    MsgBox Msg, vbExclamation
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowMessage(ByVal Msg As String)
    MsgBox Msg, vbInformation
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Max(ByVal A As Variant, ByVal B As Variant) As Variant
    If A > B Then
        Max = A
    Else
        Max = B
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Min(ByVal A As Variant, ByVal B As Variant) As Variant
    If A < B Then
        Min = A
    Else
        Min = B
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CopyGridToClipboard(grd As VSFlexGrid)
Dim Buffer As String
    With Clipboard
        .Clear
        If SaveGridToText(grd, Buffer) Then
            .SetText Buffer
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SaveGridToText(grd As VSFlexGrid, Buffer As String) As Boolean
Dim i As Long
Dim j As Long
Dim k As Long
Dim s As String
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
    
    Buffer = ""
    With grd
        
        If .Rows > 1 And .Cols > 0 Then
            .GetSelection r1, c1, r2, c2
            If (r2 - r1 + 1) * (c2 - c1 + 1) > 1 Then
                Debug.Print "Selection"
            Else
                r1 = 1
                r2 = .Rows - 1
                c1 = 0
                c2 = .Cols - 1
            End If
            For i = r1 To r2
                If Not .RowHidden(i) Then
                    s = ""
                    k = 0
                    For j = c1 To c2
                        If Not .ColHidden(j) Then
                            If k > 0 Then
                                s = s & vbTab
                            End If
                            k = k + 1
                            If .Cell(flexcpChecked, i, j) = flexNoCheckbox Then
                                s = s & .TextMatrix(i, j)
                            Else
                                If .Cell(flexcpChecked, i, j) = flexChecked Then
                                    s = s & "1"
                                ElseIf .Cell(flexcpChecked, i, j) = flexUnchecked Then
                                    s = s & "0"
                                Else
                                    s = s & ""
                                End If
                            End If
                        End If
                    Next
                    If Len(s) > 0 Then
                        Buffer = Buffer & s & vbCrLf
                    End If
                End If
            Next
        End If
    End With
    SaveGridToText = Len(Buffer) > 0
End Function

