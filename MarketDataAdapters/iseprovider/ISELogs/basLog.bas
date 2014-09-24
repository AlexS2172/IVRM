Attribute VB_Name = "basLog"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Private m_LogFileName As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogFileName() As String
    If Len(m_LogFileName) = 0 Then
        m_LogFileName = App.Path & "\ISELog.txt"
        If Len(Dir$(App.Path & "\logs", vbDirectory)) > 0 Then
            m_LogFileName = App.Path & "\logs\ISELog.txt"
        End If
    End If
    LogFileName = m_LogFileName
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearLog()
Dim f As Long
    On Error Resume Next
    f = FreeFile
    Open LogFileName For Output As #f
    Close #f
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogMessage( _
    ByVal Msg As String, _
    Optional ByVal Style As VbMsgBoxStyle = vbInformation _
)
Dim f As Long
Dim Inf As String
    On Error Resume Next
    Select Case Style
    Case vbInformation
        Inf = " Info: "
    Case vbExclamation
        Inf = " Warning: "
    Case vbCritical
        Inf = " Error: "
    Case Else
        Inf = " (" & Format$(Style) & ") "
    End Select
    f = FreeFile
    Open LogFileName For Append As #f
    Print #f, Format$(Now) & Inf & Msg
    Close #f
End Sub

