Attribute VB_Name = "basLogFile"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Const gDebugFile As Boolean = False
Private tStart As Date
Private tStop As Date
Private Duration As Double
Private Performance As Double

Private gBusy As Boolean
Private gCancel As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub gStartTask()
    gBusy = True
    gCancel = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub gStopTask()
    gBusy = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub gIdle()
    DoEvents
    Sleep 10
    If gBusy And gCancel Then
        Err.Raise vbObjectError, , "Operation calceled"
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowError(Optional ByVal Title As String = "")
    If Len(Title) = 0 Then
        Title = Err.Description
    End If
    If Len(Title) > 0 Then
        gCmn.ErrorMsgBox Title
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowWarning(ByVal Msg As String)
    gCmn.MyMsgBox Msg, vbExclamation
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowMessage(ByVal Msg As String)
    gCmn.MyMsgBox Msg, vbInformation
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogFile() As String
    On Error Resume Next
    LogFile = MakeCommonFilePath("back_office", "Debug.log", True, True)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogClear()
    On Error Resume Next
    If Len(Dir(LogFile)) > 0 Then
        Kill LogFile
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogSave(ParamArray D())
Dim f As Long
Dim v
    On Error Resume Next
    If gDebugFile Then
        f = FreeFile
        Open LogFile For Append Access Write As #f
        For Each v In D
            Print #f, v,
        Next
            Print #f,
        Close #f
    Else
        For Each v In D
            Debug.Print v,
        Next
        Debug.Print
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FormatDuration(ByVal Duration As Long) As String
Dim l As Long
Dim s As Long
Dim m As Long
Dim h As Long
Dim D As Long
    l = Duration
    s = l Mod 60
    l = (l - s) \ 60
    If l > 0 Then
        m = l Mod 60
        l = (l - m) \ 60
        If l > 0 Then
            h = l Mod 60
            l = (l - m) \ 60
            If l > 0 Then
                D = l Mod 24
            End If
        End If
    End If
    FormatDuration = _
        IIf(D > 0, Format$(D) & IIf(D = 1, " day ", " days "), "") & _
        IIf(h > 0, Format$(h) & IIf(h = 1, " hour ", " hours "), "") & _
        IIf(m > 0, Format$(m) & " min ", "") & _
        Format$(s) & " sec"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogTaskStarted(Optional ByVal TaskCaption As String)
    On Error Resume Next
    tStart = Now
    LogSave "Task started " & tStart
    If Len(TaskCaption) > 0 Then
        LogSave TaskCaption
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogTaskFinished(Optional ByVal TaskCaption As String = "", Optional ByVal ItemsProcessed As Double = 0)
    On Error Resume Next
    tStop = Now
    Duration = DateDiff("s", tStart, tStop)
    If Duration > 0 And ItemsProcessed > 0 Then
        Performance = ItemsProcessed / Duration
    Else
        Performance = 0
    End If
    If Len(TaskCaption) > 0 Then
        LogSave TaskCaption
    End If
    LogSave "Task terminated " & tStop
    LogSave "Task Duration: " & FormatDuration(Duration)
    If ItemsProcessed <> 0 Then
        LogSave "Items processed: " & Format$(ItemsProcessed, "0.0")
    End If
    If Performance <> 0 Then
        LogSave "Performance, items/sec " & Format$(Performance, "0.0")
    End If
End Function

