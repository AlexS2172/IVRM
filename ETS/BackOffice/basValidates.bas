Attribute VB_Name = "basValidates"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Max(ByVal A As Variant, ByVal B As Variant) As Variant
    If A > B Then
        Max = A
    Else
        Max = B
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Min(ByVal A As Variant, ByVal B As Variant) As Variant
    If A < B Then
        Min = A
    Else
        Min = B
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBRead(ByVal V As Variant, Optional ByVal NullValue As Variant = 0) As Variant
    On Error Resume Next
    DBRead = IIf(IsNull(V), NullValue, V)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DBWrite(ByVal V As Variant, Optional ByVal NullValue As Variant = 0) As Variant
    On Error Resume Next
    DBWrite = IIf(V = NullValue, Null, V)
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
Dim S As String
Dim i As Long
    On Error Resume Next
    '////////////////////////
    ' Restrict user input '/
    '//////////////////////
    With TC
        i = .SelStart
        S = ValidateString(.Text, False, False, False, 14, 0)
        .Text = S
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
Dim S As String
Dim i As Long
    On Error Resume Next
    With TC
        ' Validating UserInput
        i = .SelStart
        S = ValidateString(.Text, False, False, True, 10, 1000000000)
'        s = VBCleanEntry(.Text, ".", 2)
        .Text = S
        .SelStart = i
        ValidatePrice = Val(.Text)
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ValidateAmount(TC As TextBox) As Long
    Dim S As String
    Dim i As Long
    
    On Error Resume Next
    With TC
        ' Validating UserInput
        i = .SelStart
        S = ValidateString(.Text, False, False, False, 10, 1000000000)
'        s = VBCleanEntry(.Text, ".", 2)
        .Text = S
        .SelStart = i
        ValidateAmount = Val(.Text)
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ValidatePercent(TC As TextBox) As Double
Dim S As String
Dim i As Long
    On Error Resume Next
    With TC
        i = .SelStart
        S = ValidateString(.Text, False, True, True, 6, 100)
        Dim j As Long
        j = InStr(S, ".")
        If j > 0 Then
            Dim f As String
            Dim g As String
            f = Mid(S, j + 1)
            If Len(f) > 2 Then
                f = Left(f, 2)
            End If
            g = Left(S, j - 1)
            S = g & "." & f
        End If
        .Text = S
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
                    If strCurrentSymbol = Trim$(Str$(j)) Then
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

Public Function IsBadValue(Value As Variant) As Boolean
    On Error Resume Next
    Select Case VarType(Value)
        Case vbDouble
            Value = Value + 0
            
            If Err.Number = 0 Then
                IsBadValue = Not (Value > BAD_DOUBLE_VALUE)
            Else
                IsBadValue = True
            End If
            
        Case vbLong
            IsBadValue = (Value = BAD_LONG_VALUE)
            
        Case Else
            IsBadValue = False
            
    End Select
    
End Function

