VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsBatchReportingShedule"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ID As Long
Public ExecTime As Date
Public Enabled As Boolean
Public LastRun As Date

Private Sub Class_Initialize()
    On Error Resume Next
    Enabled = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
End Sub

Public Function IsReady(ByVal dtNow As Date) As Boolean
    On Error Resume Next
    IsReady = CBool(Enabled And TimeValue(CStr(ExecTime)) < TimeValue(CStr(dtNow)) _
                            And DateValue(CStr(LastRun)) < DateValue(CStr(dtNow)))
End Function
