Attribute VB_Name = "Module1"
Public Type GUID
    Data1 As Long
    Data2 As Integer
    Data3 As Integer
    Data4(0 To 7) As Byte
End Type

Public Declare Function CoCreateGuid Lib "ole32" (ByRef pguid As GUID) As Long

Public Declare Function LockWindowUpdate Lib "user32" (ByVal hwndLock As Long) As Long

