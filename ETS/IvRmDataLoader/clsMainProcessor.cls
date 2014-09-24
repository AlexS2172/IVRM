VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMainProcessor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Private m_collUnd As clsRecPosUndColl
Private m_collUndForVolaClear As clsRecPosUndColl
Private m_collPos As clsRecPosColl

Private LastErrorNumber As Long
Private LastErrorDescription  As String
Private LastErrorSource As String

Public Event UpdateLog(ByVal sInfo As String)

Private Sub LogMsg(ByVal sInfo As String)
    RaiseEvent UpdateLog(sInfo)
End Sub

'Private Function CheckForNewFiles() As String
'    Dim fdFileInfo As WIN32_FIND_DATA, lRet As Long, j As Long
'    Dim sNewFileName As String
'
'    lRet = FindFirstFileA(g_sFtpFolder & "\*.txt", fdFileInfo)
'    If lRet <> BAD_FINDE_RESULT Then
'        For j = 0 To MAX_PATH
'            If fdFileInfo.cFileName(j) <> 0 Then
'                sNewFileName = sNewFileName + Chr(fdFileInfo.cFileName(j))
'            Else
'                Exit For
'            End If
'        Next
'        CheckForNewFiles = sNewFileName
'    Else
'        CheckForNewFiles = ""
'    End If
'
'End Function

'Private Function GetFileTypeByName(ByVal sFileName As String) As PARSED_FILE_TYPE
'
'    GetParsedFileTypeByName = PTF_NOT_DEFINED
'
'    If sFileName <> "" Then
'        If InStr(1, UCase(sFileName), "IRF") <> 0 Then
'            GetParsedFileTypeByName = PFT_INTEREST_RATES_FILE
'        Else
'            If InStr(1, UCase(sFileName), "DVF") <> 0 Then
'                GetParsedFileTypeByName = PFT_DIVIDEND_FILE
'            Else
'                If InStr(1, UCase(sFileName), "IWF") <> 0 Then
'                    GetParsedFileTypeByName = PFT_INDEX_WEIGHT_FILE
'                Else
'                    If InStr(1, UCase(sFileName), "IVF") <> 0 Then
'                        GetParsedFileTypeByName = PFT_VOLA_FILE
'                    End If
'                End If
'            End If
'        End If
'    End If
'
'End Function

Private Function Idle()
    DoEvents
    If LastErrorNumber <> 0 Then
        Err.Raise LastErrorNumber, LastErrorSource, LastErrorDescription
    End If
    If False Then
        Err.Raise vbObjectError, "Task manager", "Task was canceled by user"
    End If
'    If tAlarm > 0 Then
'        If Now > tAlarm Then
'            tAlarm = 0
'            Err.Raise vbObjectError, "Task manager", "Timeout expired"
'        End If
'    End If
    Sleep 5
End Function

Private Function LoadDividentImport(ByVal Buffer As String, ByRef vCount As Long) As String
    vCount = 0
    Dim Lines As Variant
    Dim Items As Variant
    Dim DateParts As Variant
    Dim l As Variant
    Dim Col As String
    Dim Cell As String
    Dim s_Parent As String
    Dim nContractID As Long
    Dim sKey As String
    
    Dim Count As Long, bUpdate As Boolean
    Dim j As Long

    Dim strXML As String
    Dim mIdxSymArr() As String
    
    Dim rsIndexList As ADODB.Recordset
    Dim dtExpDate As Date
    Dim dDivAmount As Double
    Dim nDay As Long, nYear As Long, nMonth As Long
    
    j = 0
    Set rsIndexList = gDBW.usp_BoContractsSymbol_Get(GINT_INDEXES)
    ReDim mIdxSymArr(rsIndexList.RecordCount)
    With rsIndexList
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sKey = gCmn.ReadStr(.Fields("vcSymbol"))
                mIdxSymArr(j) = sKey
                j = j + 1
                .MoveNext
            Loop
        End If
    End With

    LoadDividentImport = ""
    strXML = "<Dividents>"

    Count = 0
    On Error GoTo EH
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) < 0 Then
            GoTo Done
        End If
    End If
    Count = 0

    For Each l In Lines
        On Error GoTo EH
        Count = Count + 1
        If Len(l) = 0 Then GoTo Skip
        
        Items = MySplit(l)
        If UBound(Items) <> 2 Then
            LogMsg "Wrong format of input file in line " & Str(Count)
            GoTo Skip
        End If
    
'        For j = 0 To rsIndexList.RecordCount
'            If mIdxSymArr(j) = UCase(Trim(Items(0))) Then
'                LogMsg "Symbol " & Trim(Items(0)) & " set as Index in ETS! It was Skipped"
'            GoTo Skip
'            End If
'        Next
        
        On Error GoTo ParseError
        
        Col = "Expiry Date"
        Cell = Trim(Items(1))
        
        'Correct div date parsing
        DateParts = Split(Cell, "/")
        If UBound(DateParts) <> 2 Then
            GoTo Skip
        Else
            nDay = DateParts(1)
            nMonth = DateParts(0)
            nYear = DateParts(2)

            If nMonth >= 1 And nMonth <= 12 And _
                nDay >= 1 And nDay <= 31 And _
                nYear >= 2000 Then
                    dtExpDate = DateSerial(nYear, nMonth, nDay)
            Else
                GoTo ParseError
            End If
        End If
        
        dtExpDate = CDate(Trim(Items(1)))
        
        Col = "Div Amount"
        Cell = Trim(Items(2))
        dDivAmount = gCmn.ReadDbl(Trim(Items(2)))
        
        If dDivAmount <= 0# Then
            LogMsg "Dividend in line " & Count & " with 0 of amount! It was skipped!"
            GoTo Skip
        End If
        
        strXML = strXML + "<Divs Symbol=""" + GetValidXMLText(Trim(Items(0))) + """ " + "DivDate=""" + GetValidXMLText(Format(dtExpDate)) + """ "
        strXML = strXML + "DivAmt=""" + GetValidXMLText(Trim(Items(2))) + """ " + "></Divs>"
        vCount = vCount + 1
        On Error GoTo ParseError
        LogMsg "Loaded: " & l
Skip:
        On Error Resume Next
        Idle
    Next
Done:
    On Error GoTo 0

    If strXML = "<Dividents>" Then
        Err.Raise vbObjectError, , "No Dividents found."
        Exit Function
    End If
    
    strXML = strXML + "</Dividents>"
    LoadDividentImport = strXML
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
FailedRead:
    Dim Msg As String
    Msg = Err.Description
    Resume CancelUpdate
    Exit Function
CancelUpdate:
    On Error Resume Next
    LoadDividentImport = ""
ParseError:
    LogMsg "Dividend skipped. Error parsing dividends data in line " & Count & " '" & Col & "'='" & Cell & "'. " & Msg
    Col = ""
    Cell = ""
    GoTo Skip
End Function

Private Function LoadStringFromFile(ByVal FileName As String, Optional lRetCode As Long = LSFF_OK) As String
    On Error GoTo EHANDLE
    
    Dim sData$, nSize&, fHandle&
    
    lRetCode = LSFF_OK
    
    nSize = FileLen(FileName)
    If nSize > 0 Then
        sData = Space(nSize)
        fHandle = FreeFile
        Open FileName For Binary Access Read As #fHandle
        Get #fHandle, , sData
        Close #fHandle
        LoadStringFromFile = sData
        lRetCode = LSFF_OK
    Else
        lRetCode = LSFF_EMPTY
    End If
EHANDLE:
    If Err.Number = 53 Then
        lRetCode = LSFF_NOT_EXISTS
    End If
End Function

Private Function LoadFirstStringFromFile(ByVal FileName As String, Optional lRetCode As Long = LSFF_OK) As String
    On Error GoTo EHANDLE
    
    Dim sData$, fHandle&
    lRetCode = LSFF_OK
    fHandle = FreeFile
    sData = ""
    Open FileName For Input As #fHandle
    If Not EOF(fHandle) Then
        Line Input #fHandle, sData   ' Read line into variable.
    End If
    Close #fHandle
    LoadFirstStringFromFile = sData
EHANDLE:
    If Err.Number = 53 Then
        lRetCode = LSFF_NOT_EXISTS
    End If
End Function

Private Function GetValidXMLText(sStr As String) As String
    On Error Resume Next
    Dim sOut As String
    
    sOut = sStr
    sOut = Replace(sOut, "&", "&amp;")
    sOut = Replace(sOut, "<", "&lt;")
    sOut = Replace(sOut, ">", "&gt;")
    sOut = Replace(sOut, "'", "&apos;")
    sOut = Replace(sOut, """", "&quot;")
    
    GetValidXMLText = sOut
End Function

Private Function MySplit(ByVal InputText As String) As Variant
    Dim intIndex&
    Dim Items As Variant, strReplacedText$
    
    On Error Resume Next
    
    Items = Split(InputText, """")
    
    For intIndex = LBound(Items) To UBound(Items)
        If intIndex Mod 2 = 1 Then
            Items(intIndex) = Trim(Replace(Items(intIndex), ",", " "))
        End If
    Next
    
    strReplacedText = Join(Items)
    
    MySplit = Split(strReplacedText, ",")
    
End Function

Public Function ExecuteDividentImport(ByVal sFileName As String) As Boolean
    On Error GoTo EH
    
    Dim strBuff As String
    Dim lCount As Long
    Dim strXML As String
    Dim lRetCode As Long
    
    LogMsg "[Run] Dividents Import"
   
    ExecuteDividentImport = True
    
    If Len(sFileName) = 0 Then
        LogMsg "Name of is empty"
        Exit Function
    End If
    
    strBuff = LoadStringFromFile(sFileName, lRetCode)
    
    If lRetCode = LSFF_OK Then
    
        LogMsg "[Run] Load dividends from file."
        strXML = LoadDividentImport(strBuff, lCount)
        LogMsg "[Complete] Load dividends from file."
    
        LogMsg "[Run] Save dividend to DB"
        Dim rsNotExist As ADODB.Recordset
        Set rsNotExist = gDBW.usp_BO_DividentsImport(strXML)
        Dim iNotExCount As Integer
        iNotExCount = 0
        If Not rsNotExist Is Nothing Then
            With rsNotExist
                If .RecordCount > 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        Dim strStock As String
                        strStock = !SymbolsNotExist
                        LogMsg ("File include the stock" + strStock + ". Please, add it.")
                        Dim lID As Integer
                        On Error Resume Next
                        iNotExCount = iNotExCount + 1
                        .MoveNext
                    Loop
                LogMsg (CStr(iNotExCount) + " Unknown stocks")
                End If
            End With
        End If
        LogMsg "[Complete] Save didivend to DB"
        
    Else
    
        ExecuteDividentImport = False
        
        If lRetCode = LSFF_EMPTY Then
            LogMsg sFileName & " is empty"
        Else
            LogMsg sFileName & " not exists"
        End If
        
    End If
    
Done:
    On Error Resume Next
    LogMsg "[Complete] Dividend Import"
    Exit Function
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg "[Complete with Error] " & LastErrorDescription
    ExecuteDividentImport = False
    GoTo Done
End Function

Public Function ExecuteVolatilityImport(ByVal sFileName As String) As Boolean
    On Error GoTo EH
    Dim nCount&

    If Len(sFileName) = 0 Then Exit Function

    LogMsg "[Run] Volatility Import"
    LogMsg "Volatility file: " & sFileName
        
    nCount = LoadImportedVolatilityFromFile(sFileName)
    
    ExecuteVolatilityImport = IIf(nCount <> 0, True, False)
    
Done:
    On Error Resume Next
    LogMsg "[Complete] Volatility import"
    Exit Function
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg "[Error] " & LastErrorDescription
End Function

Private Function ParseFileFormatTypeForVolatilityImport(ByRef sFirstString As String) As Byte
    Dim Items$()
    
    Items = Split(sFirstString, ",")
    
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file in line " & 1
    End If
       
    Erase Items
    
Done:
    ParseFileFormatTypeForVolatilityImport = 1
End Function

Private Function ParseFileFormatTypeForCurvesImport(ByRef sFirstString As String) As Byte
    Dim Items$()
    Items = Split(sFirstString, ",")
    
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Curves Import", "Wrong format of input file in line " & 1
    End If
    
    Erase Items
Done:
    ParseFileFormatTypeForCurvesImport = 1
End Function

Private Sub LoadUnderlyings()
    Dim rs As ADODB.Recordset, sKey$
    Dim aUnd As clsRecPosUndAtom
    
    m_collUnd.Clear
    Set rs = gDBW.usp_BoUnderlyings_Get()
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!vcSymbol)
        Set aUnd = m_collUnd.Item(sKey)
        If aUnd Is Nothing Then
            Set aUnd = m_collUnd.Add(sKey)
            aUnd.ID = gCmn.ReadLng(rs!iContractID)
            aUnd.ContractType = gCmn.ReadLng(rs!iContractTypeID)
            aUnd.Symbol = sKey
            Set aUnd = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Function LoadDataForVolatilityImportFromDatabase() As Boolean
    On Error GoTo EH
               
    LoadUnderlyings
    m_collUndForVolaClear.Clear
    m_collPos.Clear
    
    LoadDataForVolatilityImportFromDatabase = True
    
    Exit Function
EH:
    LogMsg "Fail to load data from database: " & Err.Description
    LoadDataForVolatilityImportFromDatabase = False
End Function

Private Function ParseLineOfVolatilityFile(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aUnd As clsRecPosUndAtom
    Dim sUnderlyingSymbol$, sKey$, sCol$, sValue$
    Dim Items$()
    Dim dtExpirationDate As Date, dStrike#, dVola#
    
    ParseLineOfVolatilityFile = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    sUnderlyingSymbol = ""
    dStrike = 0#: dtExpirationDate = 0#: dVola = 0#
    
    sCol = "Underlying Symbol":    sValue = Items(0):  If Len(sValue) > 0 Then sUnderlyingSymbol = sValue
    sCol = "Expiration Date":      sValue = Items(1):  If Len(sValue) > 4 Then dtExpirationDate = CDate(sValue)
    sCol = "Strike":               sValue = Items(2):  If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Volatility":           sValue = Items(3):  If Len(sValue) > 0 Then dVola = gCmn.ReadDbl(sValue)
    
    sCol = ""
    sValue = ""
    
    If sUnderlyingSymbol = "" Then
        sCol = "Underlying Symbol"
        Err.Raise vbObjectError, "Volatility Import", "Empty Underlying Symbol"
    ElseIf dtExpirationDate = 0 Then
        sCol = "Expiration Date"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Expiration Date"
    ElseIf dStrike = 0 Then
        sCol = "Strike"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Strike"
    ElseIf dVola <= 0 Then
        sCol = "Volatility"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Volatility"
    End If

    On Error GoTo 0
    
    Set aUnd = m_collUnd.Item(sUnderlyingSymbol)
    If aUnd Is Nothing Then
        LogMsg "Volatility skipped. Underlying not in database: " & sUnderlyingSymbol & "."
        bStructureExist = False
        Exit Function
    End If
    
    LogMsg "Loaded: " & sLine
    AddVolatilityFromText aUnd, dtExpirationDate, dStrike, dVola
    
    Erase Items
    ParseLineOfVolatilityFile = True
    Exit Function
ParseError:
    Erase Items
    LogMsg "Volatility skipped. Error parsing Volatility data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Sub AddVolatilityFromText(ByRef aUnd As clsRecPosUndAtom, _
                                ByVal dtExpirationDate As Date, _
                                ByVal dStrike As Double, _
                                ByVal dVola As Double)
    Dim aPos As clsRecPosAtom, sKey$
    
    sKey = CStr(m_collPos.Count)
    
    Set aPos = m_collPos.Item(sKey)
    If aPos Is Nothing Then
        Set aPos = m_collPos.Add(sKey)
        Set aPos.Und = aUnd
    Else
        Debug.Assert False
    End If
    
    If m_collUndForVolaClear.Item(aUnd.Symbol) Is Nothing Then _
        m_collUndForVolaClear.Add(aUnd.Symbol).ID = aUnd.ID
    
    aPos.PosValidNew = True
    aPos.QtyNew = CLng(dtExpirationDate)
    aPos.PriceOld = dStrike
    aPos.PriceNew = dVola
    
    aPos.bFix = True
End Sub

Private Function LoadVolatilityFromText(ByVal Buffer As String) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim bStructureExist As Boolean, nNewTradesCount&

    nNewTradesCount = 0
    bStructureExist = True
    On Error GoTo EH
        
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
    
    For nLineNumber = nBegin To nEnd
        On Error GoTo EH
        
        sLine = Lines(nLineNumber)
        
        If ParseLineOfVolatilityFile(nLineNumber, sLine, bStructureExist) Then nNewTradesCount = nNewTradesCount + 1
        
        On Error Resume Next

        On Error GoTo 0
        Idle
    Next
    
    If bStructureExist Then
        LoadVolatilityFromText = True
    Else
        If gCmn.MyMsgBox("Unknown underlyings was found." & _
                        " You must to add these underlyings before import volatilities." & _
                        " Continue anyway?", vbExclamation + vbYesNo) = vbNo Then
                        
            LogMsg "Volatility import canceled."
            LoadVolatilityFromText = False
        Else
            LoadVolatilityFromText = True
        End If
    End If
    
Done:
    On Error GoTo 0
    If nNewTradesCount = 0 Then
        Err.Raise vbObjectError, , "No volatilities found"
        Exit Function
    End If
    Erase Lines
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function

Private Function SaveVolatilityToDatabase&()
    Dim i&, nCount&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    iStoredTradesCount = 0
    
    nCount = m_collPos.Count + m_collUndForVolaClear.Count
    
    If nCount > 0 Then
    
        nNewCount = 0
        
        For i = 1 To m_collPos.Count
            On Error GoTo Failed
            Set aPos = m_collPos.Item(i)

            gDBW.usp_BoCustomStrikeSkewPoint_Default_Save aPos.Und.ID, aPos.PriceOld, aPos.QtyNew, aPos.PriceNew
            
            nNewCount = nNewCount + 1
            LogMsg "Saved: " & aPos.Und.Symbol
Skip:
            On Error GoTo Herr
            Idle
        Next
    End If
    
    SaveVolatilityToDatabase = nNewCount
            
    Exit Function
Herr:
    LogMsg "Error saving volatilities: " & Err.Description
    Exit Function

Failed:
    LogMsg "Error saving volatilities: " & Err.Description
    Resume Skip
End Function

Private Function LoadImportedVolatilityFromFile&(ByVal sFileName$)
    Dim collPos As New clsRecPosColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&
    Dim lRetCode As Long
    
    On Error GoTo FailedFile

    sFirstString = LoadFirstStringFromFile(sFileName, lRetCode)
    
    If lRetCode = LSFF_OK Then
    
        bFormatType = ParseFileFormatTypeForVolatilityImport(sFirstString)
        
        If bFormatType <> 1 Then
            Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file"
        Else
        
            LogMsg ("[Run] Load data from database.")
            bRet = LoadDataForVolatilityImportFromDatabase
        
            If bRet Then
            
                LogMsg ("[Complete] Load data from database.")
                LogMsg ("[Run] Read volatilities from file.")
                On Error GoTo FailedFile
                
                bRet = LoadVolatilityFromText(LoadStringFromFile(sFileName))
                
                If bRet Then
                
                    LogMsg ("[Complete] Read volatilities from file.")
                    On Error GoTo FailedSave
                    
                    LogMsg ("[Run] Save volatilities")
                    nRet = SaveVolatilityToDatabase()
                    LogMsg ("[Complete] Save volatilities")
                    LoadImportedVolatilityFromFile = nRet
                                        
                Else
                    LogMsg "[Error] Read volatilities from file."
                End If
                
            Else
                LogMsg "[Error] Load data form database"
            End If
        
        End If
    
        Exit Function
    Else
    
        LoadImportedVolatilityFromFile = 0
        
        If lRetCode = LSFF_EMPTY Then
            LogMsg "Source file is empty"
            DeleteFileA sFileName
        Else
            LogMsg "File not exists"
        End If
        
        Exit Function
        
    End If
    
FailedFile:
    Dim Msg As String
    Msg = "[Complete with Error] Reading from file failed."
    GoTo EH
    
FailedSave:
    Msg = "[Complete with Error] Saving volatilities failed."
    GoTo EH
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    
    LogMsg Msg
    
End Function

Public Function ExecuteImportCurve(ByVal sFileName As String) As Boolean
    On Error GoTo EH
    Dim nCount&
    
    If Len(sFileName) = 0 Then Exit Function
    
    LogMsg ("[Run] IR Curve Import")
    LogMsg ("IR file name: " & sFileName)
        
    nCount = LoadImportedCurvesFromFile(sFileName)
    
    ExecuteImportCurve = IIf(nCount = 0, True, False)
    
Done:
    On Error Resume Next
    LogMsg "[Comlete] Curves Import"
    Exit Function
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    ExecuteImportCurve = False

End Function
Private Function LoadImportedCurvesFromFile&(ByVal sFileName$)
    Dim collPos As New clsRecPosColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&
    Dim lRetCode As Long
    Dim sXML$
    
    On Error GoTo FailedFile
    
    sFirstString = LoadFirstStringFromFile(sFileName, lRetCode)
    
    If lRetCode = LSFF_OK Then
    
        bFormatType = ParseFileFormatTypeForCurvesImport(sFirstString)
        
        If bFormatType <> 1 Then
            Err.Raise vbObjectError, "Curves Import", "Wrong format of input file"
        Else
                            
            LogMsg ("[Run] Reading curves from file...")
        
            On Error GoTo FailedFile
            bRet = LoadCurvesFromText(LoadStringFromFile(sFileName), sXML)
            
            If bRet Then
            
                LogMsg ("[Complete] Reading curves from file...")
                On Error GoTo FailedSave
        
                LogMsg ("[Run] Saving curves")
                nRet = SaveCurvesToDatabase(sXML)
                LogMsg ("[Complete] Saving curves")
                LoadImportedCurvesFromFile = nRet
                
            Else
                LogMsg ("[Error] Reading curves from file...")
            End If
        
        End If
        
        Exit Function
        
    Else
    
        LoadImportedCurvesFromFile = -1
    
        If lRetCode = LSFF_EMPTY Then
            LogMsg "Source file is empty"
            DeleteFileA sFileName
        Else
            LogMsg "File not exists"
        End If
        
        Exit Function
        
    End If
    
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
    GoTo EH
    
FailedSave:
    Msg = "Saving curves failed."
    GoTo EH
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    
    If Len(Msg) = 0 Then
        Msg = "Error: " & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    
    On Error Resume Next
    LogMsg Msg
    
End Function

Private Function LoadCurvesFromText(ByVal Buffer As String, ByRef sXML$) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim nNewTradesCount&

    nNewTradesCount = 0
    On Error GoTo EH
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
       
    sXML = "<IRCurve>"
    
    For nLineNumber = nBegin To nEnd
        On Error GoTo EH
        
        sLine = Lines(nLineNumber)
        
        Dim sResult As String
        
        If ParseLineOfCurvesFile(nLineNumber, sLine, sResult) Then nNewTradesCount = nNewTradesCount + 1
        sXML = sXML + sResult

        On Error GoTo 0
        Idle
    Next
    
    sXML = sXML + "</IRCurve>"
    
    LoadCurvesFromText = True
    
Done:
    On Error GoTo 0
    If nNewTradesCount = 0 Then
        Err.Raise vbObjectError, , "No curves found"
        Exit Function
    End If
    Erase Lines
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function

Private Function ParseLineOfCurvesFile(ByVal nLineNumber As Long, ByRef sLine As String, ByRef sResult As String) As Boolean
    Dim Items$(), iDays As Integer, dBaseRate As Double, iLongSpread As Integer, iShortSpread As Integer
    Dim sCol$, sValue$
    
    ParseLineOfCurvesFile = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 3 Then
        Erase Items
        LogMsg "Wrong format of input file in line " & Str(nLineNumber)
        sResult = ""
        Exit Function
    End If
    
    On Error GoTo ParseError
    iDays = 0: dBaseRate = -1: iLongSpread = -1: iShortSpread = -1
  
    sCol = "Period":               sValue = Items(0):  If Len(sValue) > 0 Then iDays = gCmn.ReadLng(sValue)
    sCol = "Base Rate":            sValue = Items(1):  If Len(sValue) > 0 Then dBaseRate = gCmn.ReadDbl(sValue)
    sCol = "Long Spread":          sValue = Items(2):  If Len(sValue) > 0 Then iLongSpread = gCmn.ReadLng(sValue)
    sCol = "Short Spread":         sValue = Items(3):  If Len(sValue) > 0 Then iShortSpread = gCmn.ReadLng(sValue)
    
    sCol = ""
    sValue = ""
    
    If iDays = 0 Then
        sCol = "Period"
        Err.Raise vbObjectError, "Curves Import", "Empty Period"
    ElseIf dBaseRate = -1 Then
        sCol = "Base Rate"
        Err.Raise vbObjectError, "Curves Import", "Empty Base Rate"
    ElseIf iLongSpread = -1 Then
        sCol = "Long Spread"
        Err.Raise vbObjectError, "Curves Import", "Empty Long Spread"
    ElseIf iShortSpread = -1 Then
        sCol = "Short Spread"
        Err.Raise vbObjectError, "Curves Import", "Empty Short Spread"
    End If

    On Error GoTo 0
    
    dBaseRate = dBaseRate * 0.01
    
    LogMsg "Loaded: " & sLine
    
    sResult = "<IRPoint PeriodTypeID=""1"" Num = """ + CStr(iDays) + """ NeutralRate = """ + CStr(dBaseRate) + """ ShortRate = """ + CStr(dBaseRate - iShortSpread * 0.0001) + _
                            """ LongRate=""" + CStr(dBaseRate + iLongSpread * 0.0001) + """ />"

    Erase Items
    ParseLineOfCurvesFile = True
    Exit Function
ParseError:
    Erase Items
    LogMsg "Curves skipped. Error parsing curve data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function SaveCurvesToDatabase(ByVal sXML$)
    Dim i&, nCount&, nSeqNum&, sParent$, nNewCount&, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    On Error GoTo Failed
    nSeqNum = gDBW.usp_IRPoints_Import(sXML)
                            
    If nSeqNum <> 0 Then
        LogMsg "Fail to import curve. Please correct source curves file."
    End If
    Exit Function

Failed:
    LogMsg "Error saving curves: " & Err.Description
End Function

Public Function ExecuteLoadBasketWeights(ByVal sFileName As String) As Boolean
    On Error GoTo EH
    Dim nCount&
    Dim collIndices As New clsBasketIndexColl
    
    If Len(sFileName) = 0 Then Exit Function
    
    LogMsg ("[Run] Load Index Basket Weights.")
    
    If LoadBasketIndicesFromDB(collIndices) Then
        nCount = LoadBasketIndicesFromFile(sFileName, collIndices)
        If nCount > 0 Then _
            nCount = SaveBasketIndicesToDB(collIndices, nCount)
    End If
    
    ExecuteLoadBasketWeights = IIf(nCount, True, False)
Done:
    On Error Resume Next
    LogMsg "[Complete] Load Index Basket Weights."
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function LoadBasketIndicesFromDB(collIndices As clsBasketIndexColl) As Boolean
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom, sKey$
    Dim rs As ADODB.Recordset
    
    LoadBasketIndicesFromDB = False
    
    collIndices.Clear


    Set rs = gDBW.usp_BoIndexWithBasketInfo_Get
    While Not rs.EOF
    
        sKey = Trim$(UCase$(gCmn.ReadStr(rs!vcSymbol)))
        
        If collIndices.Item(sKey) Is Nothing Then
            Set aIdx = collIndices.Add(sKey)
            aIdx.ID = gCmn.ReadLng(rs!iIndexID)
            aIdx.Symbol = gCmn.ReadStr(rs!vcSymbol)
            aIdx.IsBasket = gCmn.ReadByte(rs!tiIsBasket)
            aIdx.HasComponents = gCmn.ReadByte(rs!tiHasComponents)
            Set aIdx = Nothing
        End If
        
        rs.MoveNext
    Wend
    Set rs = Nothing
    
    LoadBasketIndicesFromDB = True
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to load index basket weights info from database."
    LogMsg "[Error] Fail to load indices from DB"
End Function

Private Function LoadBasketIndicesFromFile&(ByVal sFileName$, collIndices As clsBasketIndexColl)
    On Error GoTo FailedFile
    Dim aIdx As clsBasketIndexAtom, aIdxComp As clsBasketIndexCompAtom, nCount&, sWeights$
    Dim arrWeight() As String, nLBnd&, nUBnd&
    Dim arrItems() As String, nItLBnd&, nItUBnd&, i&
    Dim sIndex$, sStock$, dWeight#, sParent$, nPos&
    Dim lRetCode As Long


    nCount = 0
    
    LogMsg ("File: " & sFileName)
    sWeights = LoadStringFromFile(sFileName, lRetCode)
    
    If lRetCode = LSFF_OK Then
    
        LogMsg ("[Run] Read Index Basket Weights from file")
    
        On Error Resume Next
        
        arrWeight = Split(sWeights, vbLf)
        nLBnd = 0
        nUBnd = -1
        nLBnd = LBound(arrWeight)
        nUBnd = UBound(arrWeight)
        
        If nUBnd >= nLBnd Then
        
            For i = nLBnd To nUBnd
                On Error Resume Next
                If Len(Trim$(arrWeight(i))) > 0 Then
                    arrItems = Split(arrWeight(i), ",")
                    nItLBnd = 0
                    nItUBnd = -1
                    nItLBnd = LBound(arrItems)
                    nItUBnd = UBound(arrItems)
                    
                    On Error GoTo FailedFile
                    If nItUBnd - nItLBnd >= 2 Then
                        sIndex = Trim$(UCase$(arrItems(nItLBnd)))
                        nPos = InStr(1, sIndex, " INDEX")
                        If nPos > 0 Then sIndex = Trim$(Left$(sIndex, nPos - 1))
                        
                        If IsValidTicker(sIndex) Then
                            sStock = Trim$(UCase$(arrItems(nItLBnd + 1)))
                            
                            If IsValidTicker(sStock) Then
                                If Len(sIndex) > 0 And Len(sStock) > 0 Then
                                    dWeight = gCmn.ReadDbl(arrItems(nItLBnd + 2)) / 100#
                                    
                                    LogMsg "Loaded: " & sIndex & " " & sStock
                                    
                                    Set aIdx = collIndices.Item(sIndex)
                                    If aIdx Is Nothing Then
                                        Set aIdx = collIndices.Add(sIndex)
                                        aIdx.ID = 0
                                        aIdx.Symbol = sIndex
                                    End If
                                    
                                    Set aIdxComp = aIdx.NewComponents.Item(sStock)
                                    If aIdxComp Is Nothing Then
                                        Set aIdxComp = aIdx.NewComponents.Add(sStock)
                                        aIdxComp.ID = 0
                                        aIdxComp.Symbol = sStock
                                        nCount = nCount + 1
                                    End If
                                    aIdxComp.Weight = dWeight
                                    
                                    Set aIdxComp = Nothing
                                    Set aIdx = Nothing
                                Else
                                    LogMsg "Line " & i + 1 & " skipped: No index or stock info."
                                End If
                            Else
                                LogMsg "Line " & i + 1 & " skipped: Invalid stock ticker '" & sStock & "'"
                            End If
                        Else
                            LogMsg "Line " & i + 1 & " skipped: Invalid index ticker '" & sIndex & "'"
                        End If
                    Else
                        LogMsg "Line " & i + 1 & " skipped: Invalid format."
                    End If
                    
                    Erase arrItems
                End If
                
                On Error GoTo EH
                Idle
            Next
        End If
        Erase arrWeight
        
        If nCount = 0 Then
            Err.Raise vbObjectError, , " No index basket weights found."
            Exit Function
        End If
        
        LogMsg "Loaded index basket weights loaded. " & Str(nCount)
        
        LoadBasketIndicesFromFile = nCount
    
        Exit Function
    
    Else
    
        LoadBasketIndicesFromFile = 0
    
        If lRetCode = LSFF_EMPTY Then
            LogMsg "Source file is empty and it will be deleted"
            DeleteFileA sFileName
        Else
            LogMsg "File not exists"
        End If
        
        Exit Function
        
    End If
    
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed"
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    
    If Len(Msg) = 0 Then
        Msg = "Error: " & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    
    On Error Resume Next
    
    Erase arrWeight
    Erase arrItems
    LoadBasketIndicesFromFile = nCount
    LogMsg Msg
    
End Function

Private Function SaveBasketIndicesToDB&(collIndices As clsBasketIndexColl, ByVal nWeightCount&)
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom, aIdxComp As clsBasketIndexCompAtom, nCount&
    Dim i&, sParent$, nContractID&, nContractTypeID&
    Dim rs As ADODB.Recordset, sMsg$, nIndexID&, nStockID&, nMsHint&
    
    LogMsg ("Save Index Basket Weights")
    nCount = 0
    
    If collIndices.Count > 0 Then
        
        For Each aIdx In collIndices
            On Error GoTo FailedSave
            
            If aIdx.ID <> 0 Then
                gDBW.usp_BoIndexWithBasketInfo_ClearWeights aIdx.ID
            End If
            
            If aIdx.NewComponents.Count > 0 Then
                'gDBW.usp_BoIndexWithBasketInfo_ClearWeights aIdx.ID
                
                For Each aIdxComp In aIdx.NewComponents
                    nIndexID = aIdx.ID
                    nStockID = aIdxComp.ID
                    nMsHint = gDBW.usp_BoIndexBasketWeight_Save(nIndexID, aIdx.Symbol, nStockID, aIdxComp.Symbol, aIdxComp.Weight, 1, 1, 0, sMsg)
                    
                    If nMsHint > 0 Then ' new contracts
                        If (nMsHint And 1) = 1 Then _
                            LogMsg "New index was added to database: " & aIdx.Symbol & ". Please update market structure."
                        
                        If (nMsHint And 2) = 2 Then _
                            LogMsg "New stock was added to database: " & aIdxComp.Symbol & ". Please update market structure."
                    
                    ElseIf nMsHint < 0 Then ' error
                        LogMsg "Fail to save basket index weight: " & sMsg
                        If nMsHint >= -4 Then Exit For ' index errors
                    Else
                        LogMsg "Saved: " & aIdx.Symbol & " " & aIdxComp.Symbol
                    End If
                    
                    nCount = nCount + 1
                    On Error GoTo EH
                    Idle
                Next
                
            End If
            
        Next
        
    End If
    
    SaveBasketIndicesToDB = nCount

    Exit Function
    
FailedSave:
    Dim Msg As String
    Msg = "Fail to save index basket weights."
    
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    
    SaveBasketIndicesToDB = nCount
    LogMsg Msg
    
End Function

Private Sub Class_Initialize()
    Set m_collUnd = New clsRecPosUndColl
    Set m_collUndForVolaClear = New clsRecPosUndColl
    Set m_collPos = New clsRecPosColl
End Sub

Private Sub Class_Terminate()
    Set m_collUnd = Nothing
    Set m_collUndForVolaClear = Nothing
    Set m_collPos = Nothing
End Sub
