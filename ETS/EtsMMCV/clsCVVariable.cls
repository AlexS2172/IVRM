VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsCVVariable"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ID As Long
Public Name As String
Public ContextID As Long
Public LevelID As Long
Public Caption As String
Public Format As String
Public Source As String
Public AggLevels As String
Public TypeCV As Long
Public Weight As Long
Public Description As String

'Private m_nCountAggLevels As Long
'Private m_AggLevels() As Long

'Public Sub ParseCVParams(ByVal cvParams As String)
    'On Error Resume Next
    'Dim sParams() As String
    
    'sParams = Split(Replace(cvParams, "<", ""), ">")
    
    'm_nCountAggLevels = UBound(sParams)
    'ReDim nParams(m_nCountAggLevels - 1) As Long
    
    'Dim i As Long
    'While i < m_nCountAggLevels
        'nParams(i) = CLng(sParams(i))
        'i = i + 1
    'Wend
    
    'm_AggLevels = nParams
'End Sub

'Public Function GetAggLevelsCount() As Long
    'On Error Resume Next
    'GetAggLevelsCount = m_nCountAggLevels
'End Function

'Public Function GetAggLevels() As Long()
    'On Error Resume Next
    'GetAggLevels = m_AggLevels
'End Function

Public Function IsAggOnLevel(ByVal nLevelID As Long) As Boolean
    On Error Resume Next
    
    IsAggOnLevel = False
    IsAggOnLevel = IIf(InStr(1, AggLevels, "<" & CStr(nLevelID) & ">") > 0, True, False)
End Function
