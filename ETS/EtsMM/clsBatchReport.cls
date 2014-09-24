VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsBatchReport"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Enum SheduleStatusEnum
    SS_COMMON = 0
    SS_NEW
    SS_UPDATED
    SS_DELETED
End Enum

Public Enum BRReportFormat
    BRF_CSV = 0
    BRF_HTML
End Enum

Public Status As SheduleStatusEnum

Public ID As Long
Public UserName As String
Public IsActive As Boolean
Public LayoutFile As String
Public LayoutFilePath As String
Public GroupID As Long
Public TraderID As Long
Public strategyID As Long
Public TraderGroupID As Long
Public IsPosExpanded As Long
Public LastRun As Date
Public ResultFile As String
Public ResultFilePath As String
Public UseDefaultLayout As Boolean
Public FileNameHeader As String
Public ReportFormat As BRReportFormat

Public Function Init() As Boolean
    On Error Resume Next
    Dim bResult As Boolean: bResult = True
    
    LayoutFile = ExtractFileName(LayoutFilePath)
    LayoutFilePath = ExtractFilePath(LayoutFilePath)
    
    Status = SS_COMMON
    
    Init = bResult
End Function

Private Sub Class_Initialize()
    On Error Resume Next
    
    ID = BAD_LONG_VALUE
    UserName = ""
    IsActive = True
    LayoutFile = ""
    LayoutFilePath = ""
    GroupID = 0
    TraderID = 0
    strategyID = 0
    TraderGroupID = 0
    IsPosExpanded = True
    LastRun = 0
    ResultFile = ""
    ResultFilePath = g_Params.CurrentUserPrivateFolder
    UseDefaultLayout = True
    FileNameHeader = ""
    ReportFormat = BRF_CSV
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
End Sub
