Attribute VB_Name = "basValidates"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private g_sCurrencySymbol As String
Public Const PF_DATE_FMT = "MMMyyyy"
Public Const dtNull As Date = #1/1/1900#

Public Const GSTR_NA = "N/A"

Public Const GF_MAX_NUMBER = 100000000000#
Public Const GINT_MAX_NUMBER_TEXT_SIZE = 20
Public Const GINT_MAX_TEXT_SIZE = 255

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetCurrencySymbolString() As String
    On Error Resume Next
    If g_sCurrencySymbol = "" Then

        g_sCurrencySymbol = Trim$(FormatCurrency(0, 0, vbFalse, vbFalse, vbFalse))

        If g_sCurrencySymbol = "" Then
            g_sCurrencySymbol = "$"
        End If
    End If

    GetCurrencySymbolString = g_sCurrencySymbol
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function PFFormatDate(ByVal dtDate As Date) As String
    On Error Resume Next
    PFFormatDate = UCase$(Format$(dtDate, PF_DATE_FMT))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FormatDate(ByVal Moment As Date) As String
    FormatDate = Format$(Month(Moment)) & "/" & Format$(Day(Moment)) & "/" & Format$(Year(Moment))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FormatDuration(ByVal Duration As Long) As String
Dim l As Long
Dim s As Long
Dim m As Long
Dim h As Long
Dim d As Long
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
                d = l Mod 24
            End If
        End If
    End If
    FormatDuration = _
        IIf(d > 0, Format$(d) & IIf(d = 1, " day ", " days "), "") & _
        IIf(h > 0, Format$(h) & IIf(h = 1, " hour ", " hours "), "") & _
        IIf(m > 0, Format$(m) & " min ", "") & _
        Format$(s) & " sec"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadString(Value As Variant) As String
    On Error Resume Next
    ReadString = 0
    ReadString = CStr(Value)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadDouble(Value As Variant) As Double
    On Error Resume Next
    ReadDouble = 0
    ReadDouble = CDbl(Value)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadCurrency(Value As Variant) As Currency
    On Error Resume Next
    ReadCurrency = 0
    ReadCurrency = CCur(Value)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadDate(Value As Variant) As Date
    On Error Resume Next
    ReadDate = 0
    ReadDate = CDate(Value)
    If ReadDate <= DateSerial(1900, 1, 1) Then ReadDate = 0
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReadLong(Value As Variant) As Long
    On Error Resume Next
    ReadLong = 0
    ReadLong = CLng(Value)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Max(ByVal a, ByVal b)
    If a > b Then
        Max = a
    Else
        Max = b
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Min(ByVal a, ByVal b)
    If a < b Then
        Min = a
    Else
        Min = b
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Swap(a, b)
Dim c
    c = a
    a = b
    b = c
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBRead(v As Variant, ByVal NullValue)
    If IsNull(v) Then
        DBRead = NullValue
    Else
        DBRead = v
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBWrite(v As Variant, ByVal NullValue)
    If v = NullValue Then
        DBWrite = Null
    Else
        DBWrite = v
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Converts boolean value to 0/1 for db storage
'
Public Function DBBit(ByVal v As Boolean) As Long
    If v Then
        DBBit = 1
    Else
        DBBit = 0
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBLong(v As Variant, Optional ByVal NullValue As Long = 0) As Long
    If IsNull(v) Then
        DBLong = NullValue
    Else
        DBLong = CLng(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBDouble(v As Variant, Optional ByVal NullValue As Double = 0) As Double
    If IsNull(v) Then
        DBDouble = NullValue
    Else
        DBDouble = CDbl(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBDate(v As Variant, Optional ByVal NullValue As Date = 0) As Date
    If IsNull(v) Then
        DBDate = NullValue
    Else
        DBDate = CDate(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBFormat(v As Variant, Optional ByVal NullValue As String = "") As String
    If IsNull(v) Then
        DBFormat = NullValue
    Else
        DBFormat = Format$(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetItemDouble(Items As Collection, ByVal Index As Long, Optional DefaultValue As Double = 0, Optional MustExist As Boolean = False, Optional ByVal Info As String) As String
Dim v As Variant
    v = GetItem(Items, Index, "", MustExist, Info)
    If v = "" Then
        GetItemDouble = DefaultValue
    Else
        GetItemDouble = ScanDouble(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ScanDouble(ByVal Value As String) As Double
    On Error GoTo WrongFormat
    If Len(Value) > 20 Then
        ScanDouble = 0
        Exit Function
    End If
'    Value = ValidateString(Value, True, True, True)
    If InStr(Value, ".") > 0 Then
        ScanDouble = Val(Value)
    Else
        ScanDouble = CDbl(Value)
    End If
    Exit Function
WrongFormat:
    On Error GoTo Herr:
    ScanDouble = Val(Value)
    Exit Function
Herr:
    Err.Raise vbObjectError, "Scan double", "Wrong format for values of type 'double'."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' CURRENCY 8 bytes
' A scaled integer between  
'   – 922,337,203,685,477.5808
' and 922,337,203,685,477.5807
'
' We restrict it to be
' - positive
' - integer i.e. no fractions
' - not more than 14 symbols
' so it can be from 1 to 99 999 999 999 999
'
Public Function ValidateCurrency(TC As TextBox) As Currency
Dim s As String
Dim i As Long
    On Error Resume Next
    '////////////////////////
    ' Restrict user input '/
    '//////////////////////
    With TC
        i = .SelStart
        s = ValidateString(.Text, False, False, False, 14, 0)
        .Text = s
        .SelStart = i
    End With
    '///////////////////////
    ' Convert user input '/
    '/////////////////////
    On Error GoTo Fail
    ValidateCurrency = CCur(TC.Text)
    Exit Function
Fail:
    On Error Resume Next
    TC.Text = ""
    ValidateCurrency = 0
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ValidatePrice(TC As TextBox) As Double
Dim s As String
Dim i As Long
    On Error Resume Next
    With TC
        ' Validating UserInput
        i = .SelStart
        s = ValidateString(.Text, False, False, True, 10, 1000000000)
'        s = VBCleanEntry(.Text, ".", 2)
        .Text = s
        .SelStart = i
        ValidatePrice = Val(.Text)
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ValidateAmount(TC As TextBox) As Long
    Dim s As String
    Dim i As Long
    
    On Error Resume Next
    With TC
        ' Validating UserInput
        i = .SelStart
        s = ValidateString(.Text, False, False, False, 10, 1000000000)
'        s = VBCleanEntry(.Text, ".", 2)
        .Text = s
        .SelStart = i
        ValidateAmount = Val(.Text)
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ValidatePercent(TC As TextBox) As Double
Dim s As String
Dim i As Long
    On Error Resume Next
    With TC
        i = .SelStart
        s = ValidateString(.Text, False, True, True, 6, 100)
        Dim j As Long
        j = InStr(s, ".")
        If j > 0 Then
            Dim f As String
            Dim g As String
            f = Mid(s, j + 1)
            If Len(f) > 2 Then
                f = Left(f, 2)
            End If
            g = Left(s, j - 1)
            s = g & "." & f
        End If
        .Text = s
        .SelStart = i
        ValidatePercent = Val(.Text)
    End With
End Function

'   -------------------------------------------------------------
'   function validates parsed string
'   Use it on Change Event
'   © 2000 Dmitry Grechishkin, grechishkin@egartech.com

    Public Function ValidateString( _
                            strInputString As String, _
                            blnAllowNegative As Boolean, _
                            blnAllowZero As Boolean, _
                            blnAllowFractions As Boolean, _
                            Optional lngMaxLen As Long = 0, _
                            Optional lngMaxValue As Long = 0 _
                            ) As String
    
    Dim strTmpValue  As String
    Dim strCurrentSymbol  As String
    Dim strLeftStroke As String
    Dim strRightStroke As String
    Dim lngLenght As Long
    Dim lngDotPosition As Long
    Dim blnInvalidSymbol As Boolean
    Dim blnCorrectDot  As Boolean
    
    Dim i As Long
    Dim j As Long
    
    On Error Resume Next
    
    strTmpValue = Trim$(strInputString)
    lngLenght = Len(strTmpValue)
    
    If lngLenght > 0 Then
'   ---------------------
'       Validates user input independently from locals and uses ',' or '.' as decimal separator
        For i = 1 To lngLenght
        blnInvalidSymbol = True
'
        If blnAllowFractions And (Mid$(strTmpValue, i, 1) = ",") Then
            Mid$(strTmpValue, i, 1) = "."
        End If
        
        strCurrentSymbol = Mid$(strTmpValue, i, 1)
'            Truncates value if it exeeds max value
             If lngMaxValue > 0 Then
                If Abs(Val(strTmpValue)) > lngMaxValue Then
                    strTmpValue = Left$(strTmpValue, lngLenght - 1)
                    blnInvalidSymbol = True
                    GoTo EX
                End If
             End If
             If lngMaxLen > 0 Then
'            If negative values are allowed to be inputted
                If Len(Trim$(Replace(strTmpValue, "-", " "))) > lngMaxLen Then
                    strTmpValue = Left$(strTmpValue, lngMaxLen)
                    blnInvalidSymbol = True
                    GoTo EX
                End If
             End If
             
             
            If i = 1 Then
                If blnAllowNegative And (strCurrentSymbol = "-") Then
                    blnInvalidSymbol = False
                    GoTo CHECK
                End If
'               if zero values are allowed to input
                If Not blnAllowZero And (strCurrentSymbol = "0") Then
                    blnInvalidSymbol = True
                    GoTo CHECK
                End If
            End If

'               --------------------------
'               numeric validation
                For j = 0 To 9
                    If strCurrentSymbol = Trim$(str$(j)) Then
                        blnInvalidSymbol = False
                        GoTo CHECK
                    End If
                Next
'               --------------------------
'            decimal separator is single in string
             If (lngDotPosition > 0) Then
                If (lngDotPosition = i) Then
                    blnCorrectDot = True
                Else
                    blnCorrectDot = False
                End If
             Else
                    blnCorrectDot = True
             End If
             
             If blnAllowFractions And (strCurrentSymbol = ".") And blnCorrectDot Then
                blnInvalidSymbol = False
                lngDotPosition = i
                GoTo CHECK
             End If

CHECK:
'   If any invalid symbol is found, cut it away
    If blnInvalidSymbol Then
        strLeftStroke = Left$(strTmpValue, i - 1)
        strRightStroke = Right$(strTmpValue, lngLenght - i)
        strTmpValue = strLeftStroke + strRightStroke
    End If
        Next
'   ---------------------
EX:
     ValidateString = strTmpValue
    
    End If
    On Error GoTo 0
End Function

