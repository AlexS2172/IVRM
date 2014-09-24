VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDividendsArr"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'local variable to hold collection
Private m_DivCount As Long
Private m_DivC() As REGULAR_DIVIDENDS

Public Property Get Count() As Long
    On Error Resume Next
    Count = m_DivCount
End Property

Public Sub Resize(ByVal NewSize As Long)
    On Error Resume Next
    m_DivCount = NewSize
    If m_DivCount > 0 Then ReDim Preserve m_DivC(1 To m_DivCount)
End Sub

Public Sub Clear()
    On Error Resume Next
    If m_DivCount > 0 Then ReDim m_DivC(1 To m_DivCount)
    m_DivCount = 0
End Sub

Friend Property Get Item(Index As Long) As REGULAR_DIVIDENDS
    On Error Resume Next
    Item = m_DivC(Index)
End Property

Friend Property Let Item(Index As Long, Value As REGULAR_DIVIDENDS)
    On Error Resume Next
    m_DivC(Index) = Value
End Property



Private Sub Class_Initialize()
    m_DivCount = 0
End Sub
