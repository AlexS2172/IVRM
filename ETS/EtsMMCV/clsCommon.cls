VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsCommon"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

' standard MsgBox replacment
Public Function MyMsgBox(ByVal frmOwner As Form, ByVal sPrompt As String, _
                        Optional ByVal enStyle As VbMsgBoxStyle = vbOKOnly + vbInformation, _
                        Optional ByVal sTitle As String = APP_TITLE, _
                        Optional ByVal sBtnText1 As String = "", _
                        Optional ByVal sBtnText2 As String = "", _
                        Optional ByVal sBtnText3 As String = "") As VbMsgBoxResult
    On Error GoTo EH
    Dim aFrm As frmMsgBox
    
    Set aFrm = New frmMsgBox
    MyMsgBox = aFrm.Execute(frmOwner, enStyle, sPrompt, sTitle, False, , , _
                            sBtnText1, sBtnText2, sBtnText3)
    Set aFrm = Nothing
    Exit Function
EH:
    On Error Resume Next
    Set aFrm = Nothing
    Err.Clear
    MyMsgBox = MsgBox(sPrompt, enStyle, sTitle)
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description
End Function

Public Function TntMsgBox(ByRef frmOwner As Form, _
                        ByVal enStyle As VbMsgBoxStyle, _
                        ByRef bDoNotUseTntSelected As Boolean, _
                        ByVal sPrompt As String, _
                        Optional ByVal sBtnText1 As String = "", _
                        Optional ByVal sBtnText2 As String = "", _
                        Optional ByVal sBtnText3 As String = "") As VbMsgBoxResult
    On Error GoTo EH
    Dim aFrm As frmMsgBox
    
    Set aFrm = New frmMsgBox
    TntMsgBox = aFrm.Execute(frmOwner, enStyle, sPrompt, "Trades Messages", _
                            True, bDoNotUseTntSelected, "&Do Not Use Trades Messages Service", _
                            sBtnText1, sBtnText2, sBtnText3)
    Set aFrm = Nothing
    Exit Function
EH:
    On Error Resume Next
    Set aFrm = Nothing
    Err.Clear
    TntMsgBox = MsgBox(sPrompt, enStyle, "Trades Messages")
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description
End Function


' error handler procedure (compiles error path and passes error to upper)
Public Sub ErrorHandler(sDesc As String)
    Dim lErrNumber As Long
    Dim sErrSource As String
    Dim sErrDescription As String
    Dim sErrHelpFile As String
    Dim lErrHelpContext As Long

    If Not Err Is Nothing Then
        With Err
            lErrNumber = .Number
            sErrSource = .Source
            If sDesc = "" Then
                sErrDescription = .Description
            Else
                sErrDescription = sDesc & vbCrLf & .Description
            End If
            sErrHelpFile = .HelpFile
            lErrHelpContext = .HelpContext
        End With
    
        Err.Clear
        Err.Raise lErrNumber, sErrSource, sErrDescription, sErrHelpFile, lErrHelpContext
    End If

End Sub

' error message box
Public Sub ErrorMsgBox(ByRef frmOwner As Form, ByVal sDesc As String)
    Dim sErrSource As String
    Dim sErrInfo As String

    If Not Err Is Nothing Then
        With Err
            If .Number <> 0 Then
                sErrSource = .Source & vbCrLf & "Error " & .Number
                sErrInfo = sErrSource & vbCrLf & .Description
            End If
            
            If .LastDllError <> 0 Then
                sErrInfo = sErrInfo & vbCrLf & "Last Dll Error Code: " & .LastDllError
            End If
        
        End With
    
        Err.Clear
    End If
    
    'On Error Resume Next
    
    On Error GoTo EH
    Dim aFrm As frmErrorMsgBox
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogFaults, "Description: " & sDesc & " ErrInfo: " & sErrInfo, "ErrorMsgBox"
    
    Set aFrm = New frmErrorMsgBox
    aFrm.Execute frmOwner, sDesc, sErrInfo
    Set aFrm = Nothing
    Exit Sub
EH:
    On Error Resume Next
    Set aFrm = Nothing
    MsgBox sDesc & vbCrLf & sErrInfo, vbCritical + vbOKOnly
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description
    
    'If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description
    'sErrDesc = sErrDesc & vbCrLf & sErrSource
    'MyMsgBox sErrDesc, vbCritical
End Sub

Public Sub CantAccessPrinterErrorMsgBox()
    MsgBox "Before you can print, you need to install a printer." & vbCrLf & _
        "To do this, click Start, point to Settings, click Printers, and then double-click Add Printer.", _
        vbExclamation, "Printing Error"
End Sub

