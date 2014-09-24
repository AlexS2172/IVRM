Attribute VB_Name = "basParseDate"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Declare Function GetUserDefaultLCID% Lib "kernel32" ()
Public Declare Function GetLocaleInfo Lib "kernel32" Alias "GetLocaleInfoA" (ByVal Locale As Long, ByVal LCType As Long, ByVal lpLCData As String, ByVal cchData As Long) As Long

Declare Function Disp2LD Lib "BidAskDH.ocx" (ByVal Str1 As String) As Long
Declare Function LD2Disp Lib "BidAskDH.ocx" (ByVal What As Integer, ByVal Ldate As Long, ByVal Str1 As String) As Integer

Public Const LOCALE_SHORTDATE = &H21
Public Const DateDspFmt% = 0

Public Type LocDte
    Firts As String
    Second As String
    Third As String
    type As Integer
End Type

Public LocaleDate As LocDte
Public EMsg As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ScanDate(ByVal DateString As String, NewDate As Date) As Boolean
Dim DateYear As Integer
Dim DateMonth As Integer
Dim DateDay As Integer
Dim LD As Long
Dim Buf$
Dim X%
Dim StrToDisp$
    NewDate = Date
    LD = Disp2LD(DateString)
    Buf$ = String$(50, 0)
    X% = LD2Disp(4, LD, Buf$)
    StrToDisp$ = Left(Buf$, 8)
    
    DateDay = Val(Mid$(StrToDisp$, 7, 2))
    DateMonth = Val(Mid$(StrToDisp$, 5, 2))
    DateYear = Val(Mid$(StrToDisp$, 1, 4))
    If DateDay > 0 And DateDay < 32 Then
        If DateMonth > 0 And DateMonth < 13 Then
            If DateYear > 1900 And DateYear < 2099 Then
                NewDate = DateSerial(DateYear, DateMonth, DateDay)
                ScanDate = True
            End If
        End If
    End If
    
End Function

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Function DispDateJ(ByVal JDate As Long) As String
'    DispDateJ = DispDate(JulToLD(JDate))
'End Function
'
''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Function DispDate$(ByVal Ldate&)
'Dim Buf$
'Dim X%
'Dim StrToDisp$
'    Buf$ = String$(50, 0)
'    X% = LD2Disp(DateDspFmt%, Ldate&, Buf$)
'    StrToDisp$ = TrimAll$(Buf$)
'    Select Case DateDspFmt%
'    Case DateAsDDMONYY%:
'        Buf$ = Mid$(StrToDisp$, 1, 2) + " " + Mid$(StrToDisp$, 3, 3) + " " + Mid$(StrToDisp$, 6, 2)
'    Case DateAsDDMONYYYY%:
'        Buf$ = Mid$(StrToDisp$, 1, 2) + " " + Mid$(StrToDisp$, 3, 3) + " " + Mid$(StrToDisp$, 6, 4)
'    End Select
'    DispDate$ = Buf$
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ScanDate1(ByVal DateString As String, NewDate As Date) As Boolean
Dim DateYear As Integer
Dim DateMonth As Integer
Dim DateDay As Integer
    If IsDate(DateString) Then
        NewDate = CDate(DateString)
        ScanDate1 = True
        Exit Function
    End If
    If ScanDateMy(DateString, NewDate) Then
        ScanDate1 = True
        Exit Function
    End If
    If SplitDate(DateString, DateYear, DateMonth, DateDay) Then
        NewDate = DateSerial(DateYear, DateMonth, DateDay)
        ScanDate1 = True
    Else
        NewDate = 0
        ScanDate1 = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Accepts date like 20011231
'
Public Function ScanDateMy(ByVal DateString As String, NewDate As Date) As Boolean
Dim s As String
    If Len(DateString) = 8 Then
        s = Mid(DateString, 5, 2) & "/" & Right(DateString, 2) & "/" & Left(DateString, 4)
        If IsDate(s) Then
            NewDate = DateValue(s)
            ScanDateMy = True
        End If
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SplitDate(ByVal DateString As String, DateYear As Integer, DateMonth As Integer, DateDay As Integer) As Boolean
    SplitDate = ParseDate(DateString, DateYear, DateMonth, DateDay)
    If SplitDate Then
        If DateMonth < 1 Or DateMonth > 12 Then
            SplitDate = False
            Exit Function
        End If
        If DateDay < 1 Or DateDay > 31 Then
            SplitDate = False
            Exit Function
        End If
        'Year
        If DateYear < 100 Then
            If DateYear < 30 Then
                DateYear = 2000 + DateYear
            Else
                DateYear = 1900 + DateYear
            End If
        End If
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ParseDateInit()
Dim iRet1 As Long
Dim iRet2 As Long
Dim Locale As Long
Dim Pos As Long
Dim lpLCDataVar As String
Dim Symbol As String
    Locale = GetUserDefaultLCID()
    iRet1 = GetLocaleInfo(Locale, LOCALE_SHORTDATE, lpLCDataVar, 0)
    Symbol = String$(iRet1, 0)
    iRet2 = GetLocaleInfo(Locale, LOCALE_SHORTDATE, Symbol, iRet1)
    Pos = InStr(Symbol, Chr$(0))
    If Pos > 0 Then
        Symbol = Left$(Symbol, Pos - 1)
    End If
    Select Case Val(Symbol)
    Case 0:
        LocaleDate.Firts = "mm"
        LocaleDate.Second = "dd"
        LocaleDate.Third = "yyyy"
        LocaleDate.type = 0
    Case 1:
        LocaleDate.Firts = "dd"
        LocaleDate.Second = "mm"
        LocaleDate.Third = "yyyy"
        LocaleDate.type = 1
    Case 2:
        LocaleDate.Firts = "yyyy"
        LocaleDate.Second = "mm"
        LocaleDate.Third = "dd"
        LocaleDate.type = 2
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ParseDate(Src As String, DateYear As Integer, DateMonth As Integer, DateDay As Integer) As Boolean
Dim ValidSep As String
Dim NVChar As String
Dim i As Integer, Pos As Integer, pos1 As Integer
Dim P1 As String, P2 As String, p3 As String
Dim X As String
Dim NoSep As Boolean
Dim NoMonth As Boolean
Dim SrcTemp As String
Dim First As String, Second As String, Third As String, DateType As Integer
    
    On Error GoTo Herr
    
    ParseDateInit
    
    ValidSep = "-/."
    ParseDate = True
    
    ''' Separator condition (must be 2 same separators)
    pos1 = 0
    Pos = 0
    
    Src = UCase(Src)
    First = LocaleDate.Firts
    Second = LocaleDate.Second
    Third = LocaleDate.Third
    DateType = LocaleDate.type
    
    NoSep = True
    For i = 1 To Len(ValidSep)
        NVChar = Mid(ValidSep, i, 1)
        Pos = InStr(1, Src, NVChar)
        If Pos Then
            NoSep = False
        End If
    Next i
    
    NoMonth = True
    For i = 1 To 12
        Pos = InStr(Src, GetMonthNameEng(i))
        If Pos = 0 Then
            Pos = InStr(Src, GetMonthNameSet(i))
        End If
        If Pos Then
            NoMonth = False
        End If
    Next i
    
    If NoMonth And NoSep Then 'Date is like 010101
        P1 = Mid(Src, 1, 2)
        P2 = Mid(Src, 3, 2)
        p3 = Mid(Src, 5)
    End If
    
    If Not NoMonth And Not NoSep Then 'Date is like 18.MAR.01 Removing separators
        For i = 1 To Len(ValidSep)
            Src = Replace(Src, Mid(ValidSep, i, 1), "")
        Next i
        NoSep = True
    End If
    
    If Not NoMonth And NoSep Then 'No separators in inputted string. Inserting the separators
        For i = 1 To 12
            Pos = InStr(Src, GetMonthNameEng(i))
            If Pos = 0 Then
                Pos = InStr(Src, GetMonthNameSet(i))
            End If
            If Pos > 1 Then
                DateMonth = i
                DateDay = Val(Mid(Src, 1, Pos - 1))
                If Mid(Src, Pos + 3) = "" Then
                    DateYear = Year(Date)
                    If DateSerial(DateYear, DateMonth, DateDay) < Date Then
                        DateYear = DateYear + 1
                    End If
                Else
                    DateYear = Val(Mid(Src, Pos + 3))
                End If
                LocaleDate.Firts = "dd"
                LocaleDate.Second = "mm"
                LocaleDate.Third = "yyyy"
                LocaleDate.type = 1
                ParseDate = True
                Exit Function
            End If
        Next i
    End If
    
    If Not NoSep Then
        For i = 1 To Len(ValidSep)
            NVChar = Mid(ValidSep, i, 1)
            Pos = InStr(1, Src, NVChar)
            If Pos Then
                P1 = Mid(Src, 1, Pos - 1)
                pos1 = InStr(Pos + 1, Src, NVChar)
                If pos1 = 0 Then
                    P2 = Mid(Src, Pos + 1, Len(Src) - Pos)
                Select Case LocaleDate.type
                Case 0: p3 = Format(Date, "yyyy") 'Year
                Case 1: p3 = Format(Date, "yyyy") 'Year
                Case 2: p3 = Format(Date, "dd") 'Day
                End Select
                    Exit For
                Else
                    P2 = Mid(Src, Pos + 1, pos1 - Pos - 1)
                    Pos = InStr(Src, " ")
                    If InStr(Src, " ") Then  ''' If Time present in string
                        p3 = Mid(Src, pos1 + 1, Pos - pos1 - 1)
                    Else
                        p3 = Mid(Src, pos1 + 1, Len(Src) - pos1)
                    End If
                    Exit For
                End If
            End If
        Next i
    End If
    
    If ParsePartofDate(P1, "yyyy", X, NVChar) Then
        If ParsePartofDate(P2, "mm", X, NVChar) Then
            If ParsePartofDate(p3, "dd", X, NVChar) Then
                DateYear = Val(P1)
                DateMonth = Val(P2)
                DateDay = Val(p3)
                ParseDate = True
                Exit Function
            End If
        End If
    End If
    
    ''' Write input date condition
    If ParsePartofDate(P1, LocaleDate.Firts, X, NVChar) Then
        If ParsePartofDate(P2, LocaleDate.Second, X, NVChar) Then
            If ParsePartofDate(p3, LocaleDate.Third, X, NVChar) Then
                If LocaleDate.type = 0 Then
                    DateMonth = Val(P1)
                    DateDay = Val(P2)
                    DateYear = Val(p3)
                    ParseDate = True
                ElseIf LocaleDate.type = 1 Then
                    DateDay = Val(P1)
                    DateMonth = Val(P2)
                    DateYear = Val(p3)
                    ParseDate = True
                ElseIf LocaleDate.type = 2 Then
                    DateYear = Val(P1)
                    DateMonth = Val(P2)
                    DateDay = Val(p3)
                    ParseDate = True
                Else
                    ParseDate = False
                    Exit Function
                End If
            End If
        Else
            ParseDate = False
            Exit Function
        End If
    Else
        ParseDate = False
        Exit Function
    End If

Done:
    LocaleDate.Firts = First
    LocaleDate.Second = Second
    LocaleDate.Third = Third
    LocaleDate.type = DateType
    Exit Function
Herr:
    ParseDate = False
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ParsePartofDate(Src As String, Cond As String, X As String, p As String) As Boolean
    Dim Part As String
    Dim t  As String
    ParsePartofDate = True
    If Cond = "dd" Then
        If Val(Src) < 0 Or Val(Src) > 31 Then ParsePartofDate = False
    End If
    If Cond = "yyyy" Then
        If Val(Src) < 0 Or Val(Src) > 2999 Then ParsePartofDate = False
    End If
    If Cond = "mm" Then
        If Val(Src) > 0 Then
            If Val(Src) < 0 Or Val(Src) > 12 Then ParsePartofDate = False
        Else  '' String Expration of month
            If Len(Src) > 3 Then
                ParsePartofDate = False
            End If
        End If
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetMonthNameEng(ByVal i As Integer) As String
    Select Case i
    Case 1
        GetMonthNameEng = "JAN"
    Case 2
        GetMonthNameEng = "FEB"
    Case 3
        GetMonthNameEng = "MAR"
    Case 4
        GetMonthNameEng = "APR"
    Case 5
        GetMonthNameEng = "MAY"
    Case 6
        GetMonthNameEng = "JUN"
    Case 7
        GetMonthNameEng = "JUL"
    Case 8
        GetMonthNameEng = "AUG"
    Case 9
        GetMonthNameEng = "SEP"
    Case 10
        GetMonthNameEng = "OCT"
    Case 11
        GetMonthNameEng = "NOV"
    Case 12
        GetMonthNameEng = "DEC"
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetMonthNameSet(ByVal i As Integer) As String
    GetMonthNameSet = UCase(MonthName(i, True))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetExpiryStd(ByVal Expiry As Date) As Date
Dim y As Long
Dim m As Long
Dim d As Long
Dim d0 As Long
Dim w As Long
Dim s As Long
    On Error GoTo Herr
    y = Year(Expiry)
    m = Month(Expiry)
    d0 = Day(Expiry)
    s = 0
    For d = 2 To 28
        w = Weekday(DateSerial(y, m, d), vbSunday)
        If w = vbSaturday Then
            s = s + 1
            If s = 3 Then
                GetExpiryStd = DateSerial(y, m, d)
                If GetExpiryStd <> Expiry Then
                    Debug.Print Expiry, "->", GetExpiryStd
                End If
                Exit Function
            End If
        End If
    Next
Herr:
    GetExpiryStd = Expiry
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IsNumericKey(ByVal KeyAscii As Integer) As Boolean
    IsNumericKey = InStr("0123456789.,-+" & Chr(vbKeyBack) & vbCrLf, Chr(KeyAscii)) > 0
End Function

