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
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' working with forms and controls
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SelectText(ByRef txt As Variant)
    On Error Resume Next
    txt.SelStart = 0
    txt.SelLength = Len(txt.Text)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CustomizeForm(ByRef frm As Variant)
Dim ctl
    On Error Resume Next
    CustomizeControls frm.Controls
    frm.BackColor = GCOLOR_DIALOGUE_BACK
    For Each ctl In frm.Controls
        If ctl.Tag = "noauto" Then GoTo NextCtl
        If TypeOf ctl Is VB.PictureBox Then
            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
        End If
NextCtl:
    Next ctl
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub EnableChildren(ByRef ctlParent As Variant, ByVal bEnabled As Boolean)
Dim ctlChild As Variant
    On Error Resume Next
    For Each ctlChild In ctlParent
        ctlChild.Enabled = bEnabled
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CustomizeControls(ByRef ctls As Variant)
Dim ctl
    On Error Resume Next
    For Each ctl In ctls
        If ctl.Tag = "noauto" Then GoTo NextCtl
'        ' settings for MSForms.ComboBox
'        If TypeOf ctl Is MSForms.ComboBox Then
'            ctl.SelectionMargin = False
'        End If
        If TypeOf ctl Is ElladaFlatControls.FlatButton Then
            ctl.BackColor = GCOLOR_BUTTON_BACK
            ctl.ForeColor = GCOLOR_BUTTON_FORE
            ctl.HoverBackColor = GCOLOR_BUTTON_HOVER_BACK
            ctl.HoverForeColor = GCOLOR_BUTTON_HOVER_FORE
            ctl.DisabledBackColor = GCOLOR_BUTTON_DISABLED_BACK
            ctl.DisabledForeColor = GCOLOR_BUTTON_DISABLED_FORE
            ctl.PressBackColor = GCOLOR_BUTTON_PRESS_BACK
            ctl.PressForeColor = GCOLOR_BUTTON_PRESS_FORE
            ctl.Alignment = 2
            ctl.CaptionShift = 100
        End If
        If TypeOf ctl Is FocusFlatControls1.FlatButton Then
            ctl.BackColor = GCOLOR_BUTTON_BACK
            ctl.ForeColor = GCOLOR_BUTTON_FORE
            ctl.HoverBackColor = GCOLOR_BUTTON_HOVER_BACK
            ctl.HoverForeColor = GCOLOR_BUTTON_HOVER_FORE
            ctl.DisabledBackColor = GCOLOR_BUTTON_DISABLED_BACK
            ctl.DisabledForeColor = GCOLOR_BUTTON_DISABLED_FORE
            ctl.PressBackColor = GCOLOR_BUTTON_PRESS_BACK
            ctl.PressForeColor = GCOLOR_BUTTON_PRESS_FORE
            ctl.Alignment = 2
        End If
        If TypeOf ctl Is FocusFlatControls1.FlatSpin Then
            ctl.ButtonBackColor = GCOLOR_BUTTON_BACK
            ctl.ButtonForeColor = GCOLOR_BUTTON_FORE
            ctl.HoverBackColor = GCOLOR_BUTTON_HOVER_BACK
            ctl.HoverForeColor = GCOLOR_BUTTON_HOVER_FORE
            ctl.DisabledBackColor = GCOLOR_BUTTON_DISABLED_BACK
            ctl.DisabledForeColor = GCOLOR_BUTTON_DISABLED_FORE
            ctl.PressBackColor = GCOLOR_BUTTON_PRESS_BACK
            ctl.PressForeColor = GCOLOR_BUTTON_PRESS_FORE
        End If
        If TypeOf ctl Is FocusFlatControls1.FlatCombo Then
            ctl.ButtonBackColor = GCOLOR_BUTTON_BACK
            ctl.ButtonForeColor = GCOLOR_BUTTON_FORE
            ctl.HoverBackColor = GCOLOR_BUTTON_HOVER_BACK
            ctl.HoverForeColor = GCOLOR_BUTTON_HOVER_FORE
            ctl.DisabledBackColor = GCOLOR_BUTTON_DISABLED_BACK
            ctl.DisabledForeColor = GCOLOR_BUTTON_DISABLED_FORE
            ctl.PressBackColor = GCOLOR_BUTTON_PRESS_BACK
            ctl.PressForeColor = GCOLOR_BUTTON_PRESS_FORE
        End If
        If TypeOf ctl Is ElladaFlatControls.ActiveLabel Then
            If ctl.Tag = "red" Then
                ctl.ForeColor = GCOLOR_RED_ACTIVE_LABEL_FORE
                ctl.HoverForeColor = GCOLOR_RED_ACTIVE_LABEL_HOVERFORE
                ctl.PressForeColor = GCOLOR_RED_ACTIVE_LABEL_PRESSFORE
            End If
            ctl.Font.Bold = True
        End If
        If TypeOf ctl Is VB.Frame Then
            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
        End If
        If TypeOf ctl Is VB.TextBox Then
            If ctl.Tag = "text" Then
                ctl.MaxLength = GINT_MAX_TEXT_SIZE
            ElseIf ctl.Tag = "" Then
                ctl.MaxLength = GINT_MAX_NUMBER_TEXT_SIZE
            End If
        End If
'        If TypeOf ctl Is VB.PictureBox Then
'            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
'        End If
NextCtl:
    Next ctl
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' standard MsgBox replacement
'
Public Function MyMsgBox( _
    ByVal sPrompt, _
    Optional ByVal iButtons As VbMsgBoxStyle = vbOKOnly + vbInformation, _
    Optional ByVal sTitle As String = APP_TITLE, _
    Optional ByVal sHelpFile As String, _
    Optional ByVal iContext As Long = 0 _
) As VbMsgBoxResult
    On Error GoTo Herr
    MyMsgBox = frmMyMsgBox.Execute(sPrompt, iButtons, sTitle, sHelpFile, iContext)
    Exit Function
Herr:
    MyMsgBox = MsgBox(sPrompt, iButtons, sTitle, sHelpFile, iContext)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' standard Input box replacement
'
Public Function MyInputBox( _
    ByVal sPrompt As String, _
    Optional ByVal sTitle As String = APP_TITLE, _
    Optional ByVal sDefault As String, _
    Optional ByVal XPos As Single, _
    Optional ByVal YPos As Single, _
    Optional ByVal sHelpFile As String, _
    Optional ByVal iContext As Long = 0 _
) As String
    On Error GoTo Herr
    MyInputBox = frmMyInputBox.Execute(sPrompt, sTitle, sDefault, XPos, YPos, sHelpFile, iContext)
    Exit Function
Herr:
    MyInputBox = InputBox(sPrompt, sTitle, sDefault, XPos, YPos, sHelpFile, iContext)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' error handler procedure (compiles error path and passes error to upper)
'
Public Sub ErrorHandler(sSrc As String, Optional sDesc As String)
Dim lErrNumber As Long
Dim sErrSource As String
Dim sErrDescription As String
Dim sErrHelpFile As String
Dim lErrHelpContext As Long
    With Err
        lErrNumber = .Number
        sErrSource = .Source
        If sErrSource <> "" Then
            sErrSource = sErrSource & "-> " & sSrc
        Else
            sErrSource = sSrc
        End If
        If sDesc = "" Then
            sErrDescription = .Description
        Else
            sErrDescription = sDesc
        End If
        sErrHelpFile = .HelpFile
        lErrHelpContext = .HelpContext
    End With
    Err.Clear
    Err.Raise lErrNumber, sErrSource, sErrDescription, sErrHelpFile, lErrHelpContext
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' error message box
'
Public Sub ErrorMsgBox(sSrc As String, Optional sDesc As String)
Dim sErrSource As String
Dim sErrDesc As String
Dim sErrHelpFile As String
Dim lErrHelpContext As Long
    With Err
        sErrSource = .Source
        If sErrSource <> "" Then
            sErrSource = sErrSource & "-> " & sSrc
        Else
            sErrSource = sSrc
        End If
        sErrSource = "Error " & .Number & Chr(10) & sErrSource
        sErrHelpFile = .HelpFile
        lErrHelpContext = .HelpContext
        If sDesc = "" Then
            sErrDesc = .Description
        Else
            sErrDesc = sDesc
        End If
    End With
    Err.Clear
    sErrDesc = sErrDesc & Chr(10) & Chr(10) & sErrSource
    MyMsgBox sErrDesc, vbCritical
End Sub

