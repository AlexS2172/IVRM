VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsProjectionInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'Public Enum SheduleStatusEnum
'    SS_COMMON = 0
'
'    SS_NEW
'    SS_UPDATED
'    SS_DELETED
'End Enum

Public Status As SheduleStatusEnum

Public ID As Long
Public UserName As String
Public ProjectionFile As String
Public LayoutFile As String
Public AggByHolding As Boolean
Public SheduleEnabled As Boolean
Public StartTime As Date
Public EveryDay As Boolean

Public ResultFileName As String
Public LastCalcDate As Date

Public ProjectionFileName As String
Public LayoutFileName As String

Public ShiftsCount As Long

Public Groups As Collection


Public Function Init() As Boolean
    On Error Resume Next
    Dim bResult As Boolean: bResult = True
    
    ProjectionFileName = ExtractFileName(ProjectionFile)
    LayoutFileName = ExtractFileName(LayoutFile)
    Status = SS_COMMON
    
    If Not LoadProjectionFile Then bResult = False
    
    Init = bResult
End Function


Public Function LoadProjectionFile() As Boolean
    On Error Resume Next
    LoadProjectionFile = False
    
    If ProjectionFile = STR_NA Or ProjectionFile = "" Then Exit Function
    
    Dim nLen As Long
    Dim sData As String
    
    nLen = FileLen(ProjectionFile)
    If nLen > 0 Then
        sData = Space(nLen)
        
        Dim hFile As Long
        hFile = FreeFile
        Open ProjectionFile For Binary Access Read As #hFile
        Get #hFile, , sData
        Close #hFile
    Else
        LogEvent EVENT_ERROR, "Empty projection file " & ProjectionFile
        Exit Function
    End If
    
    Dim vtData As Variant
    vtData = Split(sData, vbCrLf)
    
    Dim nLinesCount As Long
    nLinesCount = UBound(vtData) + 1
    If nLinesCount <= 0 Then
        LogEvent EVENT_ERROR, "Can't load data from projection file " & ProjectionFile
        Exit Function
    End If
    
    Set Groups = Nothing
    Set Groups = New Collection
    
    Dim i As Long
    Dim aGroup As clsProjectionGroupInfo
    For i = 0 To nLinesCount - 1
        Set aGroup = New clsProjectionGroupInfo
    
        If aGroup.ParseFileLine(Trim(vtData(i)), i + 1, ProjectionFile) Then
            Groups.Add aGroup
            ShiftsCount = ShiftsCount + aGroup.Shifts.Count
        End If
        
        Set aGroup = Nothing
    Next
    
    LoadProjectionFile = True
End Function

Private Sub Class_Initialize()
    On Error Resume Next
    Set Groups = New Collection
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set Groups = Nothing
End Sub

Public Function IsCalcComplete() As Boolean
    On Error Resume Next
    Dim dtDateNow As Date: dtDateNow = DateValue(CStr(Now))
    
    IsCalcComplete = CBool(SheduleEnabled And _
                           ((EveryDay And DateValue(CStr(LastCalcDate)) = dtDateNow) Or _
                            (Not EveryDay And LastCalcDate <> 0)))
End Function
