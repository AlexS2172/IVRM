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
'**************************************************
' Class module with common functions
' Created by Andrei Shchurov (shurov@egartech.com)
' Febrary 01, 2001
'**************************************************
Option Explicit



Public Enum etsPeriodTypes
    etsPeriodDay = 1
    etsPeriodWeek = 2
    etsPeriodMonth = 3
    etsPeriodYear = 4
End Enum


Public Enum etsFormatTypes
    etsLong = 1
    etsDouble = 2
    etsDate = 3
    etsPercent = 4
    etsPercent100 = 5
    etsString = 6
    etsBool = 7
End Enum


Private msLongFormat As String
Private msDoubleFormat As String
Private msDoubleWideFormat As String
Private msDateFormat As String
Private msDoubleExtFormat As String


Private Sub Class_Initialize()
On Error Resume Next
    Init
End Sub


Public Sub Init()
On Error Resume Next
    ' reading numbers format on initializing
    msLongFormat = Me.LongFormat
    msDoubleFormat = Me.DoubleFormat
    msDoubleWideFormat = Me.DoubleWideFormat
    msDateFormat = Me.DateFormat
    msDoubleExtFormat = Me.DoubleExtFormat
End Sub

'**************************************************
' Format$ properties
'**************************************************

Public Property Get LongFormat() As String
    If Not g_aUserXMLParams Is Nothing Then
        LongFormat = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Format", "Long", "#,###0")
    End If
End Property

Public Property Let LongFormat(NewValue As String)
    msLongFormat = NewValue
    'g_aUserXMLParams.SetStringValue APP_XML_KEY & "Format", "Long", NewValue
End Property

Public Property Get DoubleExtFormat() As String
    'DoubleExtFormat = GetStrKeyValueEx(APP_REG_KEY & "Format$", "DoubleExt", "#,##0.###0")
End Property
Public Property Get DoubleFormat() As String
    'DoubleFormat = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Format", "Double", "###0.0#")
End Property
Public Property Let DoubleFormat(NewValue As String)
    msDoubleFormat = NewValue
    'g_aUserXMLParams.SetStringValue APP_XML_KEY & "Format", "Double", NewValue
End Property

Public Property Get DoubleWideFormat() As String
    'DoubleWideFormat = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Format", "WideDouble", "#,##0.###0")
End Property

Public Property Let DoubleWideFormat(NewValue As String)
    msDoubleWideFormat = NewValue
    'g_aUserXMLParams.SetStringValue APP_XML_KEY & "Format", "WideDouble", NewValue
End Property


Public Property Get DateFormat() As String
    'DateFormat = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Format", "Date", "Short Date")
End Property

Public Property Let DateFormat(NewValue As String)
    msDateFormat = NewValue
    'g_aUserXMLParams.SetStringValue APP_XML_KEY & "Format", "Date", NewValue
End Property

'**************************************************
' working with forms and controls
'**************************************************

Public Sub SelectText(ByRef txt As Variant)
On Error Resume Next
    txt.SelStart = 0
    txt.SelLength = Len(txt.Text)
End Sub


Public Sub CustomizeForm(ByRef frm As Variant)
'Dim ctl
'On Error Resume Next
'    frm.BackColor = GCOLOR_DIALOGUE_BACK
'    For Each ctl In frm.Controls
'        If ctl.Tag = "noauto" Then GoTo NextCtl
'        If TypeOf ctl Is VB.PictureBox Then
'            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
'        End If
'NextCtl:
'    Next
'    CustomizeControls frm.Controls
End Sub

Public Sub CustomizeControls(ByRef ctls As Variant)
'On Error Resume Next
'Dim ctl
'    For Each ctl In ctls
'        If ctl.Tag = "noauto" Then GoTo NextCtl
'        ' settings for MSForms.ComboBox
'        If TypeOf ctl Is MSForms.ComboBox Then
'            ctl.SelectionMargin = False
'
'        ElseIf TypeOf ctl Is ElladaFlatControls.FlatButton Then
'            ctl.BackColor = GCOLOR_BUTTON_BACK
'            ctl.ForeColor = GCOLOR_BUTTON_FORE
'            ctl.HoverBackColor = GCOLOR_BUTTON_HOVER_BACK
'            ctl.HoverForeColor = GCOLOR_BUTTON_HOVER_FORE
'            ctl.DisabledBackColor = GCOLOR_BUTTON_DISABLED_BACK
'            ctl.DisabledForeColor = GCOLOR_BUTTON_DISABLED_FORE
'            ctl.PressBackColor = GCOLOR_BUTTON_PRESS_BACK
'            ctl.PressForeColor = GCOLOR_BUTTON_PRESS_FORE
'
'            ctl.Alignment = 0
'            ctl.CaptionShift = 100
'        ElseIf TypeOf ctl Is ElladaFlatControls.ActiveLabel Then
'            If ctl.Tag = "red" Then
'                ctl.ForeColor = GCOLOR_RED_ACTIVE_LABEL_FORE
'                ctl.HoverForeColor = GCOLOR_RED_ACTIVE_LABEL_HOVERFORE
'                ctl.PressForeColor = GCOLOR_RED_ACTIVE_LABEL_PRESSFORE
'            End If
'            ctl.Font.Bold = False
'        ElseIf TypeOf ctl Is VB.Frame Then
'            ctl.BackColor = GCOLOR_DIALOGUE_BACK
'        ElseIf (TypeOf ctl Is VB.OptionButton) Or TypeOf ctl Is VB.CheckBox Then
'            ctl.BackColor = ctl.Parent.BackColor
'        ElseIf TypeOf ctl Is VB.TextBox Then
'            If ctl.Tag = "text" Then
'                ctl.MaxLength = GINT_MAX_TEXT_SIZE
'            ElseIf ctl.Tag = "" Then
'                ctl.MaxLength = GINT_MAX_NUMBER_TEXT_SIZE
'            End If
'        ElseIf TypeOf ctl Is vsOcx6LibCtl.vsElastic Then
'            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
'        ElseIf TypeOf ctl Is vsOcx6LibCtl.vsIndexTab Then
'            ctl.BackColor = GCOLOR_DIALOGUE_BACK
'            ctl.BackTabColor = GCOLOR_DIALOGUE_BACK2
'            ctl.FrontTabColor = GCOLOR_BUTTON_PRESS_BACK
'        End If
''        If TypeOf ctl Is VB.PictureBox Then
''            ctl.BackColor = GCOLOR_DIALOGUE_BACK2
''        End If
'NextCtl:
'    Next ctl
End Sub

' standard MsgBox replacment
Public Function MyMsgBox(ByVal sPrompt, _
                        Optional ByVal iButtons As VbMsgBoxStyle = vbOKOnly + vbInformation, _
                        Optional ByVal sTitle As String = "", _
                        Optional ByVal sHelpFile As String = "", _
                        Optional ByVal iContext As Long = 0) As VbMsgBoxResult
On Error GoTo ErrHnd
    If Len(sTitle) = 0 Then sTitle = AppTitle()
    
    MyMsgBox = frmMyMsgBox.Execute(sPrompt, iButtons, sTitle, sHelpFile, iContext)
Exit Function
ErrHnd:
    MyMsgBox = MsgBox(sPrompt, iButtons, sTitle, sHelpFile, iContext)
End Function

' error handler procedure (compiles error path and passes error to upper)
Public Sub ErrorHandler(sSrc As String, Optional sDesc As String = "")
    Dim lErrNumber As Long
    Dim sErrSource As String
    Dim sErrDescription As String
    Dim sErrHelpFile As String
    Dim lErrHelpContext As Long

    If Not Err Is Nothing Then
        With Err
            lErrNumber = .Number
            sErrSource = .Source
            If sErrSource <> "" Then
                If sSrc <> "" Then
                        sErrSource = sErrSource & "-> " & sSrc
                End If
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
    End If

End Sub

' error message box
Public Sub ErrorMsgBox(sSrc As String, Optional sDesc As String = "")
    Dim sErrSource As String
    Dim sErrDesc As String
    Dim sErrHelpFile As String
    Dim lErrHelpContext As Long

    Screen.MousePointer = 0
    If Not Err Is Nothing Then
        With Err
            If .Number <> 0 Then
                sErrSource = .Source
                If sErrSource <> "" Then
                    sErrSource = sSrc & vbCrLf & sErrSource
                Else
                    sErrSource = sSrc
                End If
                sErrSource = sErrSource & vbCrLf & "Error " & .Number
                sErrHelpFile = .HelpFile
                lErrHelpContext = .HelpContext
                
                If sDesc = "" Then
                    sErrDesc = .Description
                Else
                    sErrDesc = sDesc
                End If
                sErrDesc = sErrSource & vbCrLf & sErrDesc
            End If
        
        End With
    
        Err.Clear
    End If
    frmErrorMsgBox.Execute sSrc, sErrDesc
    'sErrDesc = sErrDesc & vbCrLf & sErrSource
    'MyMsgBox sErrDesc, vbCritical
End Sub


'**************************************************
'            formating functions
'**************************************************

Public Function FmtLng(ByVal v As Long) As String
    On Error Resume Next
    FmtLng = Format$(v, msLongFormat)
End Function

Public Function FmtExtDbl(ByVal v As Double) As String
    On Error Resume Next
   
    FmtExtDbl = Format$(v, msDoubleExtFormat)
End Function

Public Function FmtDbl(ByVal v As Double) As String
    On Error Resume Next
    FmtDbl = Format$(v, msDoubleFormat) 'msDoubleFormat  "#####0.0#"
End Function
Public Function FmtWideDbl(ByVal v As Double) As String
    On Error Resume Next
    FmtWideDbl = Format$(v, msDoubleWideFormat)
End Function

Public Function FmtDate(ByVal v As Date) As String
    On Error Resume Next
    FmtDate = Format$(v, msDateFormat)
End Function






'**************************************************
'            reading functions
'**************************************************

Public Function ReadDbl(ByVal v As Variant) As Double
    On Error Resume Next
    ReadDbl = 0
    If Not IsNull(v) Then
        ReadDbl = CDbl(v)
    End If
    If Err.Number > 0 Then
        ReadDbl = Val(v)
    End If
End Function

Public Function ReadLng(ByVal v As Variant) As Long
    On Error Resume Next
    ReadLng = 0&
    If Not IsNull(v) Then
        If IsNumeric(v) Then
            ReadLng = CLng(v)
        End If
    End If
    If Err.Number > 0 Then
        ReadLng = CLng(Val(v))
    End If
End Function

Public Function ReadCur(ByVal v As Variant) As Currency
    On Error Resume Next
    ReadCur = 0
    If Not IsNull(v) Then
        ReadCur = CCur(v)
    End If
    If Err.Number > 0 Then
        ReadCur = CCur(Val(v))
    End If
End Function

Public Function ReadStr(ByVal v As Variant) As String
    On Error Resume Next
    If Not IsNull(v) Then
        ReadStr = Trim(CStr(v))
    End If
End Function

Public Function ReadByte(ByVal v As Variant) As Byte
    On Error Resume Next
    If Not IsNull(v) Then
        ReadByte = CByte(v)
    End If
End Function

Public Function ReadDate(ByVal v As Variant) As Date
    On Error GoTo Herr
    If Not IsNull(v) Then
        ReadDate = CDate(v)
        If ReadDate <= DateSerial(1900, 1, 1) Then ReadDate = Date
    Else
        ReadDate = Date
    End If
    Exit Function
Herr:
    ReadDate = Date
End Function

Public Function ReadTime(ByVal v As Variant) As Date
    On Error GoTo Herr
    If Not IsNull(v) Then
        ReadTime = CDate(v)
        If ReadTime < 0# Or ReadTime >= 1# Then ReadTime = 0#
    Else
        ReadTime = 0#
    End If
    Exit Function
Herr:
    ReadTime = 0#
End Function



'********************************************************
'                        Other
'********************************************************


'Public Function SetNull(ByVal v As Variant) As Variant
'    On Error Resume Next
'    If IsNumeric(v) Then
'        If v <= 0 Then
'            SetNull = Null
'        Else
'            SetNull = v
'        End If
'    Else
'        If CStr(v) = "" Then
'            SetNull = Null
'        Else
'            SetNull = v
'        End If
'    End If
'End Function


Public Sub GetPeriod(ByVal sPeriod As String, dSpot As Date, _
dExp As Date, lPeriod As Long, lDays As Long, nInterval As etsPeriodTypes, sInterval As String)
    
    On Error Resume Next
    Dim n As Integer
    Dim s As String
    
    If Not sPeriod Like "*day*" Then
        n = InStr(1, sPeriod, "y")
    End If
    If n > 0 Then
        s = Trim(Left(sPeriod, n - 1))
        lPeriod = Str2Num(s)
        If lPeriod = 0 Then lPeriod = 1

        If lPeriod > 1 Then
            sInterval = "years"
        Else
            sInterval = "year"
        End If
        
        nInterval = etsPeriodYear
        dExp = DateAdd("yyyy", lPeriod, dSpot)
        
    Else
        n = InStr(1, sPeriod, "m")
        If n > 0 Then
            s = Trim(Left(sPeriod, n - 1))
            lPeriod = Str2Num(s)
            If lPeriod = 0 Then lPeriod = 1
            If lPeriod > 1 Then
                sInterval = "months"
            Else
                sInterval = "month"
            End If
            nInterval = etsPeriodMonth
            dExp = DateAdd("m", lPeriod, dSpot)
        Else
            n = InStr(1, sPeriod, "w")
            If n > 0 Then
                s = Trim(Left(sPeriod, n - 1))
                lPeriod = Str2Num(s)
                If lPeriod = 0 Then lPeriod = 1
                If lPeriod > 1 Then
                    sInterval = "weeks"
                Else
                    sInterval = "week"
                End If
                nInterval = etsPeriodWeek
                dExp = DateAdd("ww", lPeriod, dSpot)
            Else
                n = InStr(1, sPeriod, "d")
                If n > 0 Then
                    s = Trim(Left(sPeriod, n - 1))
                    lPeriod = Str2Num(s)
                    If lPeriod = 0 Then lPeriod = 1
                    If lPeriod > 1 Then
                        sInterval = "days"
                    Else
                        sInterval = "day"
                    End If
                    nInterval = etsPeriodDay
                    dExp = DateAdd("d", lPeriod, dSpot)
                End If
            End If
        End If
    End If
    
    If n > 0 Then
        If Not ((sInterval Like "day*") Or (sInterval Like "week*")) Then
            Select Case Weekday(dExp, vbSunday)
            Case vbSunday
                dExp = dExp - 2
            Case vbSaturday
                dExp = dExp - 1
            End Select
        End If
        
        lDays = DateDiff("d", dSpot, dExp)
    End If

End Sub

Public Sub CalcPeriod(dSpot As Date, dExp As Date, _
lPeriod As Long, lDays As Long, nInterval As etsPeriodTypes, sInterval As String, Optional ByExpDate = True)
    
    On Error Resume Next
    
    If ByExpDate Then
        lDays = DateDiff("d", dSpot, dExp)
    Else
        dExp = DateAdd("d", lDays, dSpot)
    End If
    
    lPeriod = DateDiff("yyyy", dSpot, dExp)
    If lPeriod >= 1 Then
        If DateSerial(Year(dSpot) + lPeriod, Month(dSpot), Day(dSpot)) - _
        DateSerial(Year(dExp), Month(dExp), Day(dExp)) <= 2 Then
            If lPeriod = 1 Then
                sInterval = "year"
            Else
                sInterval = "years"
            End If
            nInterval = etsPeriodYear
        Else
            lPeriod = 0
        End If
    End If
    
    If lPeriod = 0 Then
        lPeriod = DateDiff("m", dSpot, dExp)
        If lPeriod >= 1 Then
            If DateSerial(Year(dSpot), Month(dSpot) + lPeriod, Day(dSpot)) - _
            DateSerial(Year(dExp), Month(dExp), Day(dExp)) <= 2 Then
                If lPeriod = 1 Then
                    sInterval = "month"
                Else
                    sInterval = "months"
                End If
                nInterval = etsPeriodMonth
            Else
                lPeriod = 0
            End If
        End If
    End If
    
    If lPeriod = 0 Then
        lPeriod = lDays
        If lPeriod = 1 Then
            sInterval = "day"
        Else
            sInterval = "days"
        End If
        nInterval = etsPeriodDay
    End If
        
End Sub


Public Function Str2Num(ByVal sNumber As String) As Double
    On Error Resume Next
    Dim s As String
    sNumber = Trim(sNumber)
    If Not IsNumeric(sNumber) Then
        s = Right(sNumber, 1)
        If s = "%" Then
            sNumber = Left(sNumber, Len(sNumber) - 1)
            If Not IsNumeric(sNumber) Then
                Str2Num = Val(sNumber)
            Else
                Str2Num = CDbl(sNumber) / 100
            End If
        Else
            Str2Num = Val(sNumber)
        End If
    Else
        Str2Num = CDbl(sNumber)
    End If
End Function


Public Function ValidFormat(ByVal sText As String, nType As etsFormatTypes, Optional vValue As Variant, Optional sFormat As String) As String
    On Error GoTo e
    
    Select Case nType
    Case etsLong
        vValue = Abs(Str2Num(sText))
        ValidFormat = vValue
    Case etsDouble
        If sFormat = "" Then sFormat = "#,##0.#0"
        vValue = Abs(Str2Num(sText))
        ValidFormat = Format$(vValue, sFormat)
    Case etsDate
        If sFormat = "" Then sFormat = "mm/dd/yyyy"
        vValue = ReadDate(sText)
        ValidFormat = Format$(vValue, sFormat)
    Case etsPercent
        If sFormat = "" Then sFormat = "#,##0.#0%"
        vValue = Abs(Str2Num(sText))
        If vValue >= 1 Then vValue = vValue / 100
        ValidFormat = Format$(vValue, "#,##0.#0%")
    Case etsPercent100
        If sFormat = "" Then sFormat = "#,##0.#0%"
        vValue = Abs(Str2Num(sText))
        Do While vValue >= 1
            vValue = vValue / 100
        Loop
        ValidFormat = Format$(vValue, "#,##0.#0%")
    Case Else
        ValidFormat = sText
    End Select
    
    Exit Function
e:
    gCmn.ErrorMsgBox "ctlRiskMatrix.ValidFormat"
End Function


