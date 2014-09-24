Attribute VB_Name = "basCalendar"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindBrakets(ByVal InString As String, OutString As String, Optional ByVal LeftString As String, Optional ByVal RightString As String) As Boolean
Dim i1 As Long
Dim i2 As Long

    If Len(LeftString) > 0 Then
        i1 = InStr(1, InString, LeftString)
        If i1 > 0 Then
            i1 = i1 + Len(LeftString)
        Else
            GoTo Fail
        End If
    Else
        i1 = 1
    End If
    
    If Len(RightString) > 0 Then
        i2 = InStr(i1, InString, RightString)
        If i2 > 0 Then
            'OK
        Else
            GoTo Fail
        End If
    Else
        i2 = Len(InString) + 1
    End If
    
    OutString = Mid$(InString, i1, i2 - i1)
    
    FindBrakets = True
    Exit Function
Fail:
    FindBrakets = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LastDateString() As String
    LastDateString = FormatDate(LastWorkingDay(Yesterday))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Formats date like 12/31/1999
'
Public Function FormatDate(ByVal Moment As Date) As String
    FormatDate = Format$(Year(Moment), "0000") _
        & "-" & Format$(Month(Moment), "00") _
        & "-" & Format$(Day(Moment), "00")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Today() As Date
    Today = Date
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Yesterday() As Date
    Yesterday = PrevDay(Today)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Tomorrow() As Date
    Tomorrow = NextDay(Today)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function PrevDay(ByVal Moment As Date) As Date
    PrevDay = Moment - 1
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NextDay(ByVal Moment As Date) As Date
    NextDay = Moment + 1
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IsWeekend(ByVal Moment As Date) As Boolean
    If Weekday(Moment, vbMonday) > 5 Then
        IsWeekend = True
    Else
        IsWeekend = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Not implemented
'
Public Function IsHolyday(ByVal Moment As Date) As Boolean
    IsHolyday = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IsWorkingDay(ByVal Moment As Date) As Boolean
    IsWorkingDay = Not (IsWeekend(Moment) Or IsHolyday(Moment))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LastWorkingDay(ByVal Moment As Date) As Date
    LastWorkingDay = Moment
    Do While Not IsWorkingDay(LastWorkingDay)
        LastWorkingDay = PrevDay(LastWorkingDay)
    Loop
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function PrevWorkingDay(ByVal Moment As Date) As Date
    PrevWorkingDay = LastWorkingDay(PrevDay(Moment))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetItemDate(Items As Collection, ByVal Index As Long, Optional DefaultValue As Date, Optional MustExist As Boolean = False, Optional ByVal Info As String) As String
Dim V As Variant
    V = GetItem(Items, Index, "", MustExist, Info)
    If V = "" Then
        GetItemDate = DefaultValue
    Else
        GetItemDate = ScanDate(V)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ScanDate(ByVal Value As String) As Double
    On Error GoTo WrongFormat
    ScanDate = CDate(Value)
    Exit Function
WrongFormat:
    On Error GoTo Herr:
    If Len(Value) = 8 Then ' May be ISO format YYYYMMDD ?
        ScanDate = CDate(Left(Value, 4) & "-" & Mid(Value, 5, 2) & "-" & Right(Value, 2))
        Exit Function
    ElseIf Len(Value) = 6 Then ' May be format YYMMDD ?
        ScanDate = CDate(Left(Value, 2) & "-" & Mid(Value, 3, 2) & "-" & Right(Value, 2))
        Exit Function
    End If
Herr:
    Err.Raise vbObjectError, "Load Data", "Wrong format for values of type 'Date'."
End Function

