Attribute VB_Name = "basTextFiles"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadBStringFromFile(ByVal FileName As String) As String
Dim b() As Byte
Dim n As Long
Dim f As Long
    n = FileLen(FileName)
'    If n > 102400 Then
'        Err.Raise vbObjectError, "LoadStringFromFile", "The file size exceeds 100kb limit."
'    End If
    If n > 0 Then
        ReDim b(0 To n - 1)
        f = FreeFile
        Open FileName For Binary Access Read As #f
        Get #f, , b
        Close #f
        LoadBStringFromFile = StrConv(b, vbUnicode)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStringFromFile(ByVal FileName As String) As String
Dim s As String
Dim n As Long
Dim f As Long
    n = FileLen(FileName)
'    If n > 102400 Then
'        Err.Raise vbObjectError, "LoadStringFromFile", "The file size exceeds 100kb limit."
'    End If
    If n > 0 Then
        s = Space(n)
        f = FreeFile
        Open FileName For Binary Access Read As #f
        Get #f, , s
        Close #f
        LoadStringFromFile = s
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SaveStringToFile(ByVal FileName As String, ByVal s As String) As Boolean
Dim n As Long
Dim f As Long
    n = Len(s)
    If n > 0 Then
        If Dir(FileName) <> "" Then
            Kill FileName
        End If
        f = FreeFile
        Open FileName For Binary Access Write As #f
        Put #f, , s
        Close #f
    End If
    SaveStringToFile = True
End Function

