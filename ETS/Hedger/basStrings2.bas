Attribute VB_Name = "BasStrings2"
Option Explicit
Option Compare Text

Public Function GetLinesWithoutComments(Lines As Collection) As Collection
Dim V As Variant
Dim S As String
    Set GetLinesWithoutComments = New Collection
    For Each V In Lines
        S = TabTrim(StripComments(V))
        If S <> "" Then
            GetLinesWithoutComments.Add S
        End If
    Next
End Function

Public Function ReadLinesWithoutComments(ByVal FileName As String) As Collection
Dim F As Long
Dim S As String
    On Error GoTo Herr
    Set ReadLinesWithoutComments = New Collection
    F = FreeFile
    Open FileName For Input Access Read As #F
    Do While Not EOF(F)
        Line Input #F, S
        S = StripComments(S)
        If Len(S) > 0 Then
            ReadLinesWithoutComments.Add S
        End If
    Loop
    Close #F
    Exit Function
Herr:
    Dim Msg As String
    Msg = "Ошибка чтения файла" & vbCrLf _
        & "Имя файла: '" & FileName & "'" & vbCrLf _
        & Err.Description
    Err.Raise vbObjectError, "ReadLines", Msg
End Function

Public Function ReadLines(ByVal FileName As String) As Collection
Dim F As Long
Dim S As String
    On Error GoTo Herr
    Set ReadLines = New Collection
    F = FreeFile
    Open FileName For Input Access Read As #F
    Do While Not EOF(F)
        Line Input #F, S
        ReadLines.Add S
    Loop
    Close #F
    Exit Function
Herr:
    Dim Msg As String
    Msg = "Ошибка чтения файла" & vbCrLf _
        & "Имя файла: '" & FileName & "'" & vbCrLf _
        & Err.Description
    Err.Raise vbObjectError, "ReadLines", Msg
End Function

Public Function SplitLines(ByVal Buffer As String) As Collection
Dim LineStart As Long ' начало значимой части строки
Dim LineStop As Long ' конец значимой части строки
Dim LineLen As Long  ' длина значимой части строки
Dim CrPos As Long ' положение Cr в строке
Dim LfPos As Long ' положение Lf в строке
Dim LineBuf As String
Dim CurPos As Long
    Set SplitLines = New Collection
    CurPos = 1
    Do While CurPos < Len(Buffer)
        LineStart = CurPos
        LineStop = Len(Buffer) + 1
        ' найти конец строки
        CrPos = InStr(LineStart, Buffer, vbCr)
        If (CrPos > 0) And (CrPos < LineStop) Then LineStop = CrPos
        LfPos = InStr(LineStart, Buffer, vbLf)
        If (LfPos > 0) And (LfPos < LineStop) Then LineStop = LfPos
        CurPos = LineStop + 1
        If LfPos = (LineStop + 1) Then CurPos = LfPos + 1
        If CrPos = (LineStop + 1) Then CurPos = LineStop + 1
        LineLen = LineStop - LineStart
        If LineLen > 0 Then
            LineBuf = Mid$(Buffer, LineStart, LineLen)
            SplitLines.Add LineBuf
        End If
        DoEvents
    Loop
End Function

Public Function ReadFromFile(ByVal FileName As String) As String
Dim F As Long
Dim N As Long
    On Error GoTo Herr
    N = FileLen(FileName)
    ReadFromFile = Space(N)
    If N > 0 Then
        F = FreeFile
        Open FileName For Binary Access Read As #F
        Get #F, , ReadFromFile
        Close #F
    End If
    Exit Function
Herr:
    Dim Msg As String
    Msg = "Ошибка чтения файла" & vbCrLf _
        & "Имя файла: '" & FileName & "'" & vbCrLf _
        & Err.Description
    Err.Raise vbObjectError, "ReadFromFile", Msg
End Function

Public Function ReadFromTextFile(ByVal FileName As String) As String
Dim F As Long
Dim S As String
    On Error GoTo Herr
    F = FreeFile
    Open FileName For Input Access Read As #F
    Do While Not EOF(F)
        Line Input #F, S
        ReadFromTextFile = ReadFromTextFile & S & vbCrLf
    Loop
    Close #F
    Exit Function
Herr:
    Dim Msg As String
    Msg = "Ошибка чтения файла" & vbCrLf _
        & "Имя файла: '" & FileName & "'" & vbCrLf _
        & Err.Description
    Err.Raise vbObjectError, "ReadFromTextFile", Msg
End Function

Public Function StripComments(ByVal S As String, Optional Separator As String = ";") As String
Dim CommentPos As Long
    ' убрать коментарии
    CommentPos = InStr(1, S, Separator)
    If CommentPos > 0 Then
        StripComments = Left$(S, CommentPos - 1)
    Else
        StripComments = S
    End If
End Function

Public Function DivideString(ByVal S As String, LeftPart As String, RightPart As String, Optional Separator As String = "=") As Boolean
Dim SepPos As Long
    SepPos = InStr(S, Separator)
    If SepPos > 0 Then
        LeftPart = Left$(S, SepPos - 1)
        RightPart = Mid$(S, SepPos + Len(Separator))
        DivideString = True
    Else
        LeftPart = S
        RightPart = ""
        DivideString = False
    End If
End Function

Public Function SplitString(ByVal S As String, _
    Optional ByVal Separator As String = ",", _
    Optional ByVal AddEmptyItem As Boolean = True, _
    Optional ByVal AddLastEmptyItem As Boolean = True) As Collection
Dim BufLen As Long
Dim SepLen As Long
Dim SepPos As Long
Dim CurPos As Long
    Set SplitString = New Collection
    BufLen = Len(S)
    If BufLen < 1 Then
        If AddEmptyItem Then
            SplitString.Add ""
        End If
        Exit Function
    End If
    SepLen = Len(Separator)
    If SepLen < 1 Then
        SplitString.Add S
        Exit Function
    End If
    CurPos = 1
    Do
        If CurPos > BufLen Then
            If AddLastEmptyItem Then
                SplitString.Add ""
            End If
            Exit Do
        End If
        SepPos = InStr(CurPos, S, Separator)
        If SepPos < CurPos Then
            SplitString.Add Mid$(S, CurPos)
            Exit Do
        Else
            SplitString.Add Mid$(S, CurPos, SepPos - CurPos)
            CurPos = SepPos + SepLen
        End If
    Loop
End Function

Public Function SplitItems(ByVal S As String, Optional Separator As String = ",") As Collection
Dim C As Collection
Dim V As Variant
    Set SplitItems = New Collection
    Set C = SplitString(S, Separator, False, False)
    For Each V In C
        SplitItems.Add TabTrim(V)
    Next
End Function

Public Function GetItem(Items As Collection, ByVal Index As Long, Optional DefaultValue As String = "", Optional MustExist As Boolean = False, Optional ByVal Info As String) As String
    On Error GoTo NotFound
    GetItem = Items.Item(Index)
    If GetItem = "" Then GoTo NotFound
    Exit Function
NotFound:
    GetItem = DefaultValue
    If MustExist Then
        Dim Msg As String
        Msg = "Не найден обязательный параметр" & vbCrLf
        Msg = Msg & "Индекс " & Index
        If Info <> "" Then Msg = Msg & " '" & Info & "'"
        Msg = Msg & vbCrLf & "Строка параметров: '" & CombineItems(Items) & "'"
        Err.Raise vbObjectError, "Функция GetItem", Msg
    End If
End Function

Public Function CombineItems(Items As Collection, _
    Optional Separator As String = ",", _
    Optional AddLastSeparator As Boolean = False) As String
Dim FirstItem As Boolean
Dim S As String
Dim V As Variant
    On Error Resume Next
    CombineItems = ""
    FirstItem = True
    For Each V In Items
        S = CStr(V)
        If FirstItem Then
            CombineItems = S
            FirstItem = False
        Else
            CombineItems = CombineItems & Separator & S
        End If
    Next
    If AddLastSeparator Then
        CombineItems = CombineItems & Separator
    End If
End Function

Public Function MakeAdminSharePath(ByVal ServerName As String, ByVal FilePath As String) As String
Dim DriveLetter As String
Dim S As String
    DriveLetter = UCase(Left$(FilePath, 1))
    S = Mid$(FilePath, 3)
    MakeAdminSharePath = "\\" & ServerName & "\" & DriveLetter & "$" & S
End Function

Public Function ExtractFileName(ByVal FilePath As String) As String
    ExtractFileName = Mid$(FilePath, 1 + GetLastDirSeparator(FilePath))
End Function

Public Function ExtractPath(ByVal FilePath As String) As String
    ExtractPath = Left$(FilePath, GetLastDirSeparator(FilePath))
End Function

Public Function GetLastDirSeparator(ByVal FilePath As String) As Long
    GetLastDirSeparator = Len(FilePath)
    Do While GetLastDirSeparator > 0
        If Mid$(FilePath, GetLastDirSeparator, 1) = "\" Then Exit Do
        GetLastDirSeparator = GetLastDirSeparator - 1
    Loop
End Function

Public Function RTabTrim(ByVal S As String) As String
Dim CurPos As Long
    CurPos = Len(S)
    Do While CurPos >= 1
        If Asc(Mid$(S, CurPos, 1)) > Asc(" ") Then Exit Do
        CurPos = CurPos - 1
    Loop
    RTabTrim = Left$(S, CurPos)
End Function

Public Function LTabTrim(ByVal S As String) As String
Dim CurPos As Long
    CurPos = 1
    Do While CurPos <= Len(S)
        If Asc(Mid$(S, CurPos, 1)) > Asc(" ") Then Exit Do
        CurPos = CurPos + 1
    Loop
    LTabTrim = Mid$(S, CurPos)
End Function

Public Function TabTrim(ByVal S As String) As String
    TabTrim = RTabTrim(LTabTrim(S))
End Function

Public Function ReplaceArgument(S As String, ByVal S1 As String, ByVal S2 As String) As Boolean
Dim intPos As Long
Dim intStart As Long
Dim SWork As String
Dim SLeft As String
Dim SRight As String
    intStart = 1
    SWork = S
    intPos = InStr(intStart, SWork, S1)
    If intPos > 0 Then
        intStart = intPos + Len(S1)
        SLeft = Left(SWork, intPos - 1)
        SRight = Mid(SWork, intStart)
        SWork = SLeft & S2 & SRight
        S = SWork
        ReplaceArgument = True
    Else
        ReplaceArgument = False
    End If
End Function

