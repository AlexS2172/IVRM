VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsLogMgr"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_sFileName As String
Private m_sFilePath As String

Public Sub Init(ByVal sFileName$, ByVal sLogName$)
    On Error GoTo EH
    Dim nFile&
    
    m_sFileName = Trim$(sFileName)
    m_sFilePath = MakeCommonFilePath("Logs", m_sFileName, True)
    
    nFile = FreeFile
    Open m_sFilePath For Output Access Write Lock Write As #nFile
    
    Print #nFile, sLogName
    Print #nFile, APP_TITLE & " Version " & App.Major & "." & App.Minor & "." & App.Revision
    Print #nFile, "OS: " & GetOSType
    Print #nFile, GetSystemInfoString
    Print #nFile, MakeAppComponentsDescription(True)
        
    Print #nFile, ""
    
    On Error Resume Next
    Close #nFile
        
    Exit Sub
EH:
    Dim nErrNum&, sErrDesc$, sErrSrc$
    nErrNum = Err.Number
    sErrSrc = Err.Source
    sErrDesc = Err.Description
    On Error Resume Next
    Close #nFile
    On Error GoTo 0
    Err.Raise nErrNum, sErrSrc, sErrDesc
End Sub

Public Sub LogInfo(ByRef sInfo$)
    On Error GoTo EH
    Dim nFile&
    
    nFile = FreeFile
    Open m_sFilePath For Append Access Write Lock Write As #nFile
    
    Print #nFile, sInfo;
        
    On Error Resume Next
    Close #nFile
    
    Exit Sub
EH:
    Dim nErrNum&, sErrDesc$, sErrSrc$
    nErrNum = Err.Number
    sErrSrc = Err.Source
    sErrDesc = Err.Description
    On Error Resume Next
    Close #nFile
    On Error GoTo 0
    Err.Raise nErrNum, sErrSrc, sErrDesc
End Sub

