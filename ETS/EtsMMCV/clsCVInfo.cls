VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsCVInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_nRisksContextID As Long
Private m_nGreekContextID As Long

Private m_nRisksLevelID As Long
Private m_nGreekLevelID As Long

Private m_GreekAggregation(0 To 3) As Long

Private m_bIsInit As Boolean


Public Sub Init()
    On Error Resume Next
    Dim Idx As Long
    Dim nID As Long
    Dim sName As String
    Dim rs As ADODB.Recordset
    
    m_nGreekContextID = gDBW.usp_cvContextID_Get("GreeksView")
    m_nRisksContextID = gDBW.usp_cvContextID_Get("RiskView")
    
    If m_nGreekContextID = 0 Or m_nRisksContextID = 0 Then
        Exit Sub
    End If
    
    Set rs = gDBW.usp_cvContext_Get(m_nGreekContextID, 6)
    
    'Aggregation levels for greeks
    Set rs = rs.NextRecordset
    If rs Is Nothing Or rs.RecordCount <> 5 Then Exit Sub
    While Not rs.EOF
        
        nID = ReadLng(rs!ID)
        sName = ReadStr(rs!Name)
        
        If sName = "Positions" Then
            m_nGreekLevelID = nID
        End If
        
        If sName = "Underlyings" Or sName = "Grouping" Or _
                                    sName = "GroupingTotals" Or _
                                    sName = "Totals" Then
            m_GreekAggregation(Idx) = nID
            Idx = Idx + 1
        End If
        
        rs.MoveNext
    Wend
    
    If Idx <> 4 Then Exit Sub
    
    If m_nGreekContextID = 0 Or m_nRisksContextID = 0 Then
        Exit Sub
    End If
    
    'Aggregation levels for risks
    Set rs = gDBW.usp_cvContext_Get(m_nRisksContextID, 6)
    Set rs = rs.NextRecordset
    If rs Is Nothing Or rs.RecordCount <> 3 Then Exit Sub
    While Not rs.EOF
        nID = ReadLng(rs!ID)
        sName = ReadStr(rs!Name)
        
        If sName = "Positions" Then
            m_nRisksLevelID = nID
        End If
                
        rs.MoveNext
    Wend
    
    m_bIsInit = True
End Sub

Private Sub Class_Initialize()
    On Error Resume Next
    m_bIsInit = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Erase m_GreekAggregation
End Sub

Public Function IsInit() As Boolean
    On Error Resume Next
    IsInit = m_bIsInit
End Function

Public Function GetRisksContextID() As Long
    On Error Resume Next
    GetRisksContextID = m_nRisksContextID
End Function

Public Function GetGreekContextID() As Long
    On Error Resume Next
    GetGreekContextID = m_nGreekContextID
End Function

Public Function GetGreekLevelID() As Long
    On Error Resume Next
    GetGreekLevelID = m_nGreekLevelID
End Function

Public Function GetRisksLevelID() As Long
    On Error Resume Next
    GetRisksLevelID = m_nRisksLevelID
End Function

Public Sub FillGreekAggregation(ByRef Agg() As Long)
    On Error Resume Next
    Dim i As Long
    
    Erase Agg
    ReDim Agg(0 To 3)
    
    For i = 0 To 3
        Agg(i) = m_GreekAggregation(i)
    Next
    
End Sub
