VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsEntityAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ID As Long
Public Name As String
Public Data As Long
Public Data2 As Long
Public Data3 As Double

Public ExtData As clsFilterAtomColl

Public dValue1 As Double
Public dValue2 As Double
Public dValue3 As Double
Public dValue4 As Double
Public sValue1 As String
Public sValue2 As String
Public vValue As Variant

Private Sub Class_Initialize()
    Clear
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set ExtData = Nothing
    vValue = Null
End Sub

Public Sub Clear()
    ID = 0
    Name = ""
    Data = 0
    Data2 = 0
    Data3 = 0
    dValue1 = 0
    dValue2 = 0
    dValue3 = 0
    dValue4 = 0
    sValue1 = ""
    sValue2 = ""
    vValue = Null
    If Not ExtData Is Nothing Then ExtData.Clear
End Sub
