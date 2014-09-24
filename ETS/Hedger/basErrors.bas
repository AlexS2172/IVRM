Attribute VB_Name = "basErrors"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public LastErrDescription As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Shows error message box
'
Public Sub ShowError(Optional ByVal Header As String = "") 'Unexpected Error
Dim Msg As String
Dim Msg2 As String
Dim Img As VbMsgBoxStyle
Dim x As String
    x = Err.Description
    If Not gRuntime Then Exit Sub
    If Len(Header) > 0 Then
        Msg = Header & vbCrLf
    Else
        Msg = ""
    End If
    Msg2 = ""
    'Img = vbCritical
    Img = vbExclamation
    Select Case Err.Number
    Case 0
        If LastErrDescription <> "" Then
            Msg2 = LastErrDescription
        End If
        LastErrDescription = ""
    Case vbObjectError
        Msg2 = x
    Case vbObjectError + 1
        Msg2 = x
    Case Else
'        Msg = Msg & "Error number:" & Err.Number & " (0x" & Hex(Err.Number) & ")" & vbCrLf
        Msg2 = x
    End Select
    If Msg2 = "" Then
        Msg2 = x
    End If
    On Error GoTo Herr
    Screen.MousePointer = vbNormal
    DoEvents
    If Msg2 = "" Then
        Debug.Assert False
    Else
        If Len(Msg) > 0 Then
            Msg = Msg & "Description: " & Msg2
        Else
            Msg = Msg2
'            Img = vbInformation
        End If
        gCmn.MyMsgBox Msg, Img
    End If
    Exit Sub
Herr:
    On Error Resume Next
    MsgBox Msg, vbCritical
End Sub

