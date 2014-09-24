VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPerformanceLog"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
Private m_objEtsLog As CETSLog

Public Function Init(ByVal sFileName As String, ByVal sFilePath As String)
    On Error GoTo EH
    
    Dim sData As String
    
    Set m_objEtsLog = New CETSLog
    Init = True
        
    m_objEtsLog.SetFilePathType enAbsolute
    m_objEtsLog.FilePath = sFilePath
    
    m_objEtsLog.FileName = Trim$(sFileName)
    m_objEtsLog.MinLogLevel = g_lMinLogLevel
    
    
    Clear
    
    Exit Function
EH:
    Init = False
End Function

Public Sub Clear()
    On Error GoTo Exi

    m_objEtsLog.LogMmInfo enLogNone, "EtsMm Performace Log", ""
    'm_objEtsLog.LogMmInfo enLogNone, "OS: " & GetOSType, ""
    'm_objEtsLog.LogMmInfo enLogNone, GetSystemInfoString, ""
    m_objEtsLog.LogMmInfo enLogNone, MakeAppComponentsDescription(True), ""
    m_objEtsLog.LogMmInfo enLogNone, "Log format:", ""
    m_objEtsLog.LogMmInfo enLogNone, "1: Time" & vbTab & "Window name" & vbTab & "Action name" & vbTab & "Underlyings" & vbTab & "Options" & vbTab & "Futures" & vbTab & "Interval (ms)", ""
    m_objEtsLog.LogMmInfo enLogNone, "2: Time" & vbTab & "RAM : " & vbTab & "Total Committed (MB)" & vbTab & "Total Available (MB)" & vbTab & "EtsMM Private (MB)" & vbTab & "CPU: " & vbTab & "Total usage (%)" & vbTab & "EtsMM usage (%)", ""
    m_objEtsLog.LogMmInfo enLogNone, "", ""
Exi:
End Sub



Public Function BeginLogMmOperation() As Long
    BeginLogMmOperation = m_objEtsLog.BeginLogMmOperation
End Function

Public Sub ContinueLogMmOperation(ByVal nOperation As Long, ByVal enOperation As eOperType, ByVal sWindowName As String, _
    Optional ByVal nUnds As Long = 0, Optional ByVal nOpts As Long = 0, Optional ByVal nFuts As Long = 0, Optional ByVal nTime As Long = -1)
    On Error Resume Next
    m_objEtsLog.ContinueLogMmOperation nOperation, enLogInformation, enOperation, sWindowName, nUnds, nOpts, nFuts, nTime
    
End Sub

Public Sub FinishLogMmOperation(ByVal nOperation As Long, ByVal enOperation As eOperType, ByVal sWindowName As String, _
    Optional ByVal nUnds As Long = 0, Optional ByVal nOpts As Long = 0, Optional ByVal nFuts As Long = 0, Optional ByVal nTime As Long = -1)
    On Error Resume Next
    m_objEtsLog.FinishLogMmOperation nOperation, enLogInformation, enOperation, sWindowName, nUnds, nOpts, nFuts, nTime
End Sub

Public Sub LogMmInfo(ByVal lLogLevel As LogLevelEnum, sInformation As String, ByVal sWindowName As String, _
    Optional ByVal nUnds As Long = 0, Optional ByVal nOpts As Long = 0, Optional ByVal nFuts As Long = 0)
    
    On Error Resume Next
    m_objEtsLog.LogMmInfo lLogLevel, sInformation, sWindowName
    
End Sub

Public Function CheckLogMmOperation(ByVal nOperation As Long) As Long
    Dim sKey$: sKey = CStr(nOperation)
    
    On Error Resume Next
    CheckLogMmOperation = m_objEtsLog.CheckLogMmOperation(nOperation)
    
End Function

Public Sub LogPerformanceCounters()
    
    On Error Resume Next
    m_objEtsLog.LogPerformanceCounters enLogInformationExt
    
'    If Not g_Params.LogPerformanceCounters Or m_Operations.Count = 0 Then Exit Sub
'    sInfo = CStr(Now) & vbTab
'    r = PdhCollectQueryData(m_hPDHQuery)
'    'Get counter value and process data.
'    If PdhVbIsGoodStatus(r) Then
'        sInfo = sInfo & "RAM: " & vbTab
'        dblVal = PdhVbGetDoubleCounterValue(m_hPDHCntCm, lngCounterStatus)
'        If PdhVbIsGoodStatus(lngCounterStatus) Then
'            sInfo = sInfo & Format$(dblVal / &H100000, "###,###") & vbTab
'        End If
'        dblVal = PdhVbGetDoubleCounterValue(m_hPDHCntAm, lngCounterStatus)
'        If PdhVbIsGoodStatus(lngCounterStatus) Then
'            sInfo = sInfo & Format$(dblVal / &H100000, "###,###") & vbTab
'        End If
'        dblVal = PdhVbGetDoubleCounterValue(m_hPDHCntMmPm, lngCounterStatus)
'        If PdhVbIsGoodStatus(lngCounterStatus) Then
'            sInfo = sInfo & Format$(dblVal / &H100000, "###,###") & vbTab
'        End If
'        sInfo = sInfo & "CPU: " & vbTab
'        dblVal = PdhVbGetDoubleCounterValue(m_hPDHCntPt, lngCounterStatus)
'        If PdhVbIsGoodStatus(lngCounterStatus) Then
'            sInfo = sInfo & Format$(dblVal, "##0") & vbTab
'        End If
'        dblVal = PdhVbGetDoubleCounterValue(m_hPDHCntMmPt, lngCounterStatus)
'        If PdhVbIsGoodStatus(lngCounterStatus) Then
'            sInfo = sInfo & Format$(dblVal, "##0") & vbTab
'        End If
'        SaveLogInfo sInfo
'    End If
End Sub

Private Sub Class_Initialize()
'    Dim strCounterPath As String
'    Dim r As Long
'    On Error Resume Next
'    m_hPDHQuery = 0
'    m_hPDHCntCm = 0
'    m_hPDHCntAm = 0
'    m_hPDHCntPt = 0
'    m_hPDHCntMmPm = 0
'    m_hPDHCntMmPt = 0
'
'    If g_Params.LogPerformanceCounters Then
'        r = PdhVbOpenQuery(m_hPDHQuery)
'        If PdhVbIsGoodStatus(r) Then
'            strCounterPath = "\Memory\Committed Bytes"
'            r = PdhVbAddCounter(m_hPDHQuery, strCounterPath, m_hPDHCntCm)
'            strCounterPath = "\Memory\Available Bytes"
'            r = PdhVbAddCounter(m_hPDHQuery, strCounterPath, m_hPDHCntAm)
'            strCounterPath = "\Processor(_Total)\% Processor Time"
'            r = PdhVbAddCounter(m_hPDHQuery, strCounterPath, m_hPDHCntPt)
'            strCounterPath = "\Process(EtsMM)\Private Bytes"
'            r = PdhVbAddCounter(m_hPDHQuery, strCounterPath, m_hPDHCntMmPm)
'            strCounterPath = "\Process(EtsMM)\% Processor Time"
'            r = PdhVbAddCounter(m_hPDHQuery, strCounterPath, m_hPDHCntMmPt)
'        End If
'    End If
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_objEtsLog = Nothing
End Sub



