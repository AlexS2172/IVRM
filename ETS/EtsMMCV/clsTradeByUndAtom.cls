VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeByUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Trd As New EtsGeneralLib.MmTradeInfoColl
Public m_SyntRootAtom As SynthRootAtom
Public m_UndColl As Collection
Public m_IsSynth As Boolean

Private Sub Class_Terminate()
    On Error Resume Next
    Set Trd = Nothing
End Sub

Public Function IsSymbolExist(strSym As String) As Integer
    On Error GoTo Ex
    IsSymbolExist = 1
    Dim strLoop As Variant
    For Each strLoop In m_UndColl
        If strSym = strLoop Then
            Exit Function
        End If
        IsSymbolExist = IsSymbolExist + 1
    Next
Ex:
    IsSymbolExist = -1
End Function
