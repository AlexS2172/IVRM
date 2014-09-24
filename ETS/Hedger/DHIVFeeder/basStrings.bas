Attribute VB_Name = "basStrings"
Option Explicit
Option Compare Text

Public Const gstrNULL$ = ""     'Empty string
Public Const LS$ = vbLf & vbLf  'Double line break

Public Const gstrSEP_DIR$ = "\"         'Directory separator character
Public Const gstrSEP_REGKEY$ = "\"      'Registration key separator character.
Public Const gstrSEP_DRIVE$ = ":"       'Driver separater character, e.g., C:\
Public Const gstrSEP_DIRALT$ = "/"      'Alternate directory separator character
Public Const gstrSEP_EXT$ = "."         'Filename extension separator character
Public Const gstrSEP_PROGID = "."
Public Const gstrSEP_FILE$ = "|"        'Use the character for delimiting filename lists because it is not a valid character in a filename.
Public Const gstrSEP_LIST = "|"
Public Const gstrSEP_URL$ = "://"       'Separator that follows HPPT in URL address
Public Const gstrSEP_URLDIR$ = "/"      'Separator for dividing directories in URL addresses.

Public Const gstrUNC$ = "\\"            'UNC specifier \\
Public Const gstrCOLON$ = ":"
Public Const gstrSwitchPrefix1 = "/"
Public Const gstrSwitchPrefix2 = "-"
Public Const gstrCOMMA$ = ","
Public Const gstrDECIMAL$ = "."
Public Const gstrQUOTE$ = """"
Public Const gstrCCOMMENT$ = "//"       ' Comment specifier used in C, etc.
Public Const gstrASSIGN$ = "="

Public Const gintMAX_SIZE% = 255        'Maximum buffer size
Public Const gintMAX_PATH_LEN% = 260    'Maximum allowed path length including path, filename,
                                        'and command line arguments for NT (Intel) and Win95.

'-----------------------------------------------------------
' FUNCTION: AddQuotesToFN
'
' Given a pathname (directory and/or filename), returns
'   that pathname surrounded by double quotes if the
'   path contains spaces or commas.  This is required for
'   setting up an icon correctly, since otherwise such paths
'   would be interpreted as a pathname plus arguments.
'-----------------------------------------------------------
'
Public Function AddQuotesToFN(ByVal strFilename) As String
    If InStr(strFilename, " ") Or InStr(strFilename, ",") Then
        AddQuotesToFN = """" & strFilename & """"
    Else
        AddQuotesToFN = strFilename
    End If
End Function

'
' Replace all double quotes with single quotes
'
Public Sub ReplaceDoubleQuotes(str As String)
    Dim i As Integer
    
    For i = 1 To Len(str)
        If Mid$(str, i, 1) = """" Then
            Mid$(str, i, 1) = "'"
        End If
    Next i
End Sub

'
'Get the path portion of a filename
'
Public Function GetPathName(ByVal strFilename As String) As String
    Dim intPos As Integer
    Dim strPathOnly As String
    Dim dirTmp As DirListBox
    Dim i As Integer

    On Error Resume Next


    Err = 0
    
    intPos = Len(strFilename)

    '
    'Change all '/' chars to '\'
    '

    For i = 1 To Len(strFilename)
        If Mid$(strFilename, i, 1) = gstrSEP_DIRALT Then
            Mid$(strFilename, i, 1) = gstrSEP_DIR
        End If
    Next i

    If InStr(strFilename, gstrSEP_DIR) = intPos Then
        If intPos > 1 Then
            intPos = intPos - 1
        End If
    Else
        Do While intPos > 0
            If Mid$(strFilename, intPos, 1) <> gstrSEP_DIR Then
                intPos = intPos - 1
            Else
                Exit Do
            End If
        Loop
    End If

    If intPos > 0 Then
        strPathOnly = Left$(strFilename, intPos)
        If Right$(strPathOnly, 1) = gstrCOLON Then
            strPathOnly = strPathOnly & gstrSEP_DIR
        End If
    Else
        strPathOnly = CurDir$
    End If

    If Right$(strPathOnly, 1) = gstrSEP_DIR Then
        strPathOnly = Left$(strPathOnly, Len(strPathOnly) - 1)
    End If

    GetPathName = strPathOnly
    
    Err = 0
End Function

'-----------------------------------------------------------
' FUNCTION: GetFileName
'
' Return the filename portion of a path
'
'-----------------------------------------------------------
'
Public Function GetFileName(ByVal strPath As String) As String
    Dim strFilename As String
    Dim iSep As Integer
    
    strFilename = strPath
    Do
        iSep = InStr(strFilename, gstrSEP_DIR)
        If iSep = 0 Then iSep = InStr(strFilename, gstrCOLON)
        If iSep = 0 Then
            GetFileName = strFilename
            Exit Function
        Else
            strFilename = Right(strFilename, Len(strFilename) - iSep)
        End If
    Loop
End Function

'
'Returns "" if the path is not complete, or is a UNC pathname
'
Public Function strGetDriveFromPath(ByVal strPath As String) As String
    If Len(strPath) < 2 Then
        Exit Function
    End If
    
    If Mid$(strPath, 2, 1) <> gstrCOLON Then
        Exit Function
    End If
    
    strGetDriveFromPath = Mid$(strPath, 1, 1) & gstrCOLON & gstrSEP_DIR
End Function

'-----------------------------------------------------------
' FUNCTION: strExtractFilenameArg
'
' Extracts a quoted or unquoted filename from a string
'   containing command-line arguments
'
' IN: [str] - string containing a filename.  This filename
'             begins at the first character, and continues
'             to the end of the string or to the first space
'             or switch character, or, if the string begins
'             with a double quote, continues until the next
'             double quote
' OUT: Returns the filename, without quotes
'      str is set to be the remainder of the string after
'      the filename and quote (if any)
'
'-----------------------------------------------------------
'
Public Function strExtractFilenameArg(str As String, fErr As Boolean)
    Dim strFilename As String
    
    str = Trim$(str)
    
    Dim iEndFilenamePos As Integer
    If Left$(str, 1) = """" Then
        ' Filenames is surrounded by quotes
        iEndFilenamePos = InStr(2, str, """") ' Find matching quote
        If iEndFilenamePos > 0 Then
            strFilename = Mid$(str, 2, iEndFilenamePos - 2)
            str = Right$(str, Len(str) - iEndFilenamePos)
        Else
            fErr = True
            Exit Function
        End If
    Else
        ' Filename continues until next switch or space or quote
        Dim iSpacePos As Integer
        Dim iSwitch1 As Integer
        'Dim iSwitch2 As Integer
        Dim iQuote As Integer
        
        iSpacePos = InStr(str, " ")
        iSwitch1 = InStr(str, gstrSwitchPrefix1)
        'iSwitch2 = InStr(str, gstrSwitchPrefix2)
        iQuote = InStr(str, """")
        
        If iSpacePos = 0 Then iSpacePos = Len(str) + 1
        If iSwitch1 = 0 Then iSwitch1 = Len(str) + 1
        'If iSwitch2 = 0 Then iSwitch2 = Len(str) + 1
        If iQuote = 0 Then iQuote = Len(str) + 1
        
        iEndFilenamePos = iSpacePos
        If iSwitch1 < iEndFilenamePos Then iEndFilenamePos = iSwitch1
        'If iSwitch2 < iEndFilenamePos Then iEndFilenamePos = iSwitch2
        If iQuote < iEndFilenamePos Then iEndFilenamePos = iQuote
        
        strFilename = Left$(str, iEndFilenamePos - 1)
        If iEndFilenamePos > Len(str) Then
            str = ""
        Else
            str = Right(str, Len(str) - iEndFilenamePos + 1)
        End If
    End If
    
    strFilename = Trim$(strFilename)
    If strFilename = "" Then
        fErr = True
        Exit Function
    End If
    
    fErr = False
    strExtractFilenameArg = strFilename
    str = Trim$(str)
End Function

'-----------------------------------------------------------
' SUB: AddDirSep
' Add a trailing directory path separator (back slash) to the
' end of a pathname unless one already exists
'
' IN/OUT: [strPathName] - path to add separator to
'-----------------------------------------------------------
'
Public Sub AddDirSep(strPathName As String)
    If Right(Trim(strPathName), Len(gstrSEP_URLDIR)) <> gstrSEP_URLDIR And _
       Right(Trim(strPathName), Len(gstrSEP_DIR)) <> gstrSEP_DIR Then
        strPathName = RTrim$(strPathName) & gstrSEP_DIR
    End If
End Sub

'-----------------------------------------------------------
' SUB: AddURLDirSep
' Add a trailing URL path separator (forward slash) to the
' end of a URL unless one (or a back slash) already exists
'
' IN/OUT: [strPathName] - path to add separator to
'-----------------------------------------------------------
'
Public Sub AddURLDirSep(strPathName As String)
    If Right(Trim(strPathName), Len(gstrSEP_URLDIR)) <> gstrSEP_URLDIR And _
       Right(Trim(strPathName), Len(gstrSEP_DIR)) <> gstrSEP_DIR Then
        strPathName = Trim(strPathName) & gstrSEP_URLDIR
    End If
End Sub

'-----------------------------------------------------------
' FUNCTION: GetUNCShareName
'
' Given a UNC names, returns the leftmost portion of the
' directory representing the machine name and share name.
' E.g., given "\\SCHWEIZ\PUBLIC\APPS\LISTING.TXT", returns
' the string "\\SCHWEIZ\PUBLIC"
'
' Returns a string representing the machine and share name
'   if the path is a valid pathname, else returns NULL
'-----------------------------------------------------------
'
Public Function GetUNCShareName(ByVal strFN As String) As Variant
    GetUNCShareName = Null
    If IsUNCName(strFN) Then
        Dim iFirstSeparator As Integer
        iFirstSeparator = InStr(3, strFN, gstrSEP_DIR)
        If iFirstSeparator > 0 Then
            Dim iSecondSeparator As Integer
            iSecondSeparator = InStr(iFirstSeparator + 1, strFN, gstrSEP_DIR)
            If iSecondSeparator > 0 Then
                GetUNCShareName = Left$(strFN, iSecondSeparator - 1)
            Else
                GetUNCShareName = strFN
            End If
        End If
    End If
End Function

'-----------------------------------------------------------
' FUNCTION: IsUNCName
'
' Determines whether the pathname specified is a UNC name.
' UNC (Universal Naming Convention) names are typically
' used to specify machine resources, such as remote network
' shares, named pipes, etc.  An example of a UNC name is
' "\\SERVER\SHARE\FILENAME.EXT".
'
' IN: [strPathName] - pathname to check
'
' Returns: True if pathname is a UNC name, False otherwise
'-----------------------------------------------------------
'
Public Function IsUNCName(ByVal strPathName As String) As Integer
    Const strUNCNAME$ = "\\//\"        'so can check for \\, //, \/, /\

    IsUNCName = ((InStr(strUNCNAME, Left$(strPathName, 2)) > 0) And _
                 (Len(strPathName) > 1))
End Function

'-----------------------------------------------------------
' FUNCTION: StripTerminator
'
' Returns a string without any zero terminator.  Typically,
' this was a string returned by a Windows API call.
'
' IN: [strString] - String to remove terminator from
'
' Returns: The value of the string passed in minus any
'          terminating zero.
'-----------------------------------------------------------
'
Public Function StripTerminator(ByVal strString As String) As String
    Dim intZeroPos As Integer

    intZeroPos = InStr(strString, vbNullChar)
    If intZeroPos > 0 Then
        StripTerminator = Left$(strString, intZeroPos - 1)
    Else
        StripTerminator = strString
    End If
End Function

'-----------------------------------------------------------
' FUNCTION: ExtractFilenameItem
'
' Extracts a quoted or unquoted filename from a string.
'
' IN: [str] - string to parse for a filename.
'     [intAnchor] - index in str at which the filename begins.
'             The filename continues to the end of the string
'             or up to the next comma in the string, or, if
'             the filename is enclosed in quotes, until the
'             next double quote.
' OUT: Returns the filename, without quotes.
'      [intAnchor] is set to the comma, or else one character
'             past the end of the string
'      [fErr] is set to True if a parsing error is discovered
'
'-----------------------------------------------------------
'
Public Function strExtractFilenameItem(ByVal str As String, intAnchor As Integer, fErr As Boolean) As String
    While Mid$(str, intAnchor, 1) = " "
        intAnchor = intAnchor + 1
    Wend
    
    Dim iEndFilenamePos As Integer
    Dim strFilename As String
    If Mid$(str, intAnchor, 1) = """" Then
        ' Filename is surrounded by quotes
        iEndFilenamePos = InStr(intAnchor + 1, str, """") ' Find matching quote
        If iEndFilenamePos > 0 Then
            strFilename = Mid$(str, intAnchor + 1, iEndFilenamePos - 1 - intAnchor)
            intAnchor = iEndFilenamePos + 1
            While Mid$(str, intAnchor, 1) = " "
                intAnchor = intAnchor + 1
            Wend
            If (Mid$(str, intAnchor, 1) <> gstrCOMMA) And (Mid$(str, intAnchor, 1) <> "") Then
                fErr = True
                Exit Function
            End If
        Else
            fErr = True
            Exit Function
        End If
    Else
        ' Filename continues until next comma or end of string
        Dim iCommaPos As Integer
        
        iCommaPos = InStr(intAnchor, str, gstrCOMMA)
        If iCommaPos = 0 Then
            iCommaPos = Len(str) + 1
        End If
        iEndFilenamePos = iCommaPos
        
        strFilename = Mid$(str, intAnchor, iEndFilenamePos - intAnchor)
        intAnchor = iCommaPos
    End If
    
    strFilename = Trim$(strFilename)
    If strFilename = "" Then
        fErr = True
        Exit Function
    End If
    
    fErr = False
    strExtractFilenameItem = strFilename
End Function

'-----------------------------------------------------------
' FUNCTION: Extension
'
' Extracts the extension portion of a file/path name
'
' IN: [strFilename] - file/path to get the extension of
'
' Returns: The extension if one exists, else gstrNULL
'-----------------------------------------------------------
'
Public Function Extension(ByVal strFilename As String) As String
    Dim intPos As Integer

    Extension = gstrNULL

    intPos = Len(strFilename)

    Do While intPos > 0
        Select Case Mid$(strFilename, intPos, 1)
            Case gstrSEP_EXT
                Extension = Mid$(strFilename, intPos + 1)
                Exit Do
            Case gstrSEP_DIR, gstrSEP_DIRALT
                Exit Do
            'End Case
        End Select

        intPos = intPos - 1
    Loop
End Function
'
' This routine adds quotation marks around an unquoted string, by default.  If the string is already quoted
' it returns without making any changes unless vForce is set to True (vForce defaults to False) except that white
' space before and after the quotes will be removed unless vTrim is False.  If the string contains leading or
' trailing white space it is trimmed unless vTrim is set to False (vTrim defaults to True).
'
Public Function strQuoteString(strUnQuotedString As String, Optional vForce As Variant, Optional vTrim As Variant)
    Dim strQuotedString As String
    
    If IsMissing(vForce) Then
        vForce = False
    End If
    If IsMissing(vTrim) Then
        vTrim = True
    End If
    
    strQuotedString = strUnQuotedString
    '
    ' Trim the string if necessary
    '
    If vTrim = True Then
        strQuotedString = Trim(strQuotedString)
    End If
    '
    ' See if the string is already quoted
    '
    If vForce = False Then
        If (Left(strQuotedString, 1) = gstrQUOTE) And (Right(strQuotedString, 1) = gstrQUOTE) Then
            '
            ' String is already quoted.  We are done.
            '
            GoTo DoneQuoteString
        End If
    End If
    '
    ' Add the quotes
    '
    strQuotedString = gstrQUOTE & strQuotedString & gstrQUOTE
DoneQuoteString:
    strQuoteString = strQuotedString
End Function

'
' This routine tests to see if strQuotedString is wrapped in quotation
' marks, and, if so, remove them.
'
Public Function strUnQuoteString(ByVal strQuotedString As String)
    strQuotedString = Trim(strQuotedString)

    If Mid$(strQuotedString, 1, 1) = gstrQUOTE And Right$(strQuotedString, 1) = gstrQUOTE Then
        '
        ' It's quoted.  Get rid of the quotes.
        '
        strQuotedString = Mid$(strQuotedString, 2, Len(strQuotedString) - 2)
    End If
    strUnQuoteString = strQuotedString
End Function

'
' This routine verifies that the length of the filename strFilename is valid.
' Under NT (Intel) and Win95 it can be up to 259 (gintMAX_PATH_LEN-1) characters
' long.  This length must include the drive, path, filename, commandline
' arguments and quotes (if the string is quoted).
'
Public Function fCheckFNLength(strFilename As String) As Boolean
    fCheckFNLength = (Len(strFilename) < gintMAX_PATH_LEN)
End Function

'
' This routine reads from a strDelimit separated list, strList, and locates the next
' item in the list following intAnchor.  Basically it finds the next
' occurance of strDelimit that is not inside quotes.  If strDelimit is not
' found the routine returns 0.  Note intAnchor must be outside of quotes
' or this routine will return incorrect results.
'
' strDelimit is typically a comma.
'
' If there is an error this routine returns -1.
'
Public Function intGetNextFldOffset(ByVal intAnchor As Integer, strList As String, strDelimit As String, Optional CompareType As Variant) As Integer
    Dim intQuote As Integer
    Dim intDelimit As Integer
    
    Const CompareBinary = 0
    Const CompareText = 1

    If IsMissing(CompareType) Then
        CompareType = CompareText
    End If
    
    If intAnchor = 0 Then intAnchor = 1
    
    intQuote = InStr(intAnchor, strList, gstrQUOTE, CompareType)
    intDelimit = InStr(intAnchor, strList, strDelimit, CompareType)
    
    If (intQuote > intDelimit) Or (intQuote = 0) Then
        '
        ' The next delimiter is not within quotes.  Therefore,
        ' we have found what we are looking for.  Note that the
        ' case where there are no delimiters is also handled here.
        '
        GoTo DoneGetNextFldOffset
    ElseIf intQuote < intDelimit Then
        '
        ' A quote appeared before the next delimiter.  This
        ' means we might be inside quotes.  We still need to check
        ' if the closing quote comes after the delmiter or not.
        '
        intAnchor = intQuote + 1
        intQuote = InStr(intAnchor, strList, gstrQUOTE, CompareType)
        If (intQuote > intDelimit) Then
            '
            ' The delimiter was inside quotes.  Therefore, ignore it.
            ' The next delimiter after the closing quote must be outside
            ' of quotes or else we have a corrupt file.
            '
            intAnchor = intQuote + 1
            intDelimit = InStr(intAnchor, strList, strDelimit, CompareType)
            '
            ' Sanity check.  Make sure there is not another quote before
            ' the delimiter we just found.
            '
            If intDelimit > 0 Then
                intQuote = InStr(intAnchor, strList, gstrQUOTE, CompareType)
                If (intQuote > 0) And (intQuote < intDelimit) Then
                    '
                    ' Something is wrong.  We've encountered a stray
                    ' quote.  Means the string is probably corrupt.
                    '
                    intDelimit = -1 ' Error
                End If
            End If
        End If
    End If
DoneGetNextFldOffset:
    intGetNextFldOffset = intDelimit
End Function

