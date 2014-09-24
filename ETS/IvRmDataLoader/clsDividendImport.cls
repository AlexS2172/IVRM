VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDividendImport"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private LastErrorNumber As Long
Private LastErrorDescription  As String
Private LastErrorSource As String

Public Event UpdateLog(ByVal sInfo As String)

Private Sub LogMsg(ByVal sInfo As String)
    RaiseEvent UpdateLog(sInfo)
End Sub

Private Function LoadStringFromFile(ByVal FileName As String) As String
    Dim s$, n&, f&
    n = FileLen(FileName)
    If n > 0 Then
        s = Space(n)
        f = FreeFile
        Open FileName For Binary Access Read As #f
        Get #f, , s
        Close #f
        LoadStringFromFile = s
    End If
End Function

Private Function ExecuteDividentImport(ByVal sFileName As String) As Boolean
    On Error GoTo EH
    
    LogMsg ("Process Dividents Import")
    LogTaskStarted
    ExecuteDividentImport = True
    Dim FileName As String
    FileName = sFileName
    If Len(FileName) = 0 Then
        Exit Function
    End If

    Dim strBuff As String, lCount As Long, strXML As String
    strBuff = LoadStringFromFile(FileName)

    strXML = LoadDividentImport(strBuff, lCount)

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
                    LogMsg ("In file finded stock" + strStock + ". Please, add it.")
                    Dim lID As Integer
                    On Error Resume Next
                    'lID = gDBW.usp_Stock_Save(, , strStock, , , , , , , , , 0, 1, , , 1)

                    iNotExCount = iNotExCount + 1
                    .MoveNext
                Loop
            LogMsg (CStr(iNotExCount) + " Unknown stocks")
            End If
        End With
    End If

    'Set rsNotExist = gDBW.usp_BO_DividentsImport(strXML)

Done:
    On Error Resume Next
    LogMsg Str(lCount) & " Divident Import"
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function LoadDividentImport(ByVal Buffer As String, ByRef vCount As Long) As String
    vCount = 0
    Dim Lines As Variant
    Dim Items As Variant
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

    LogMsg ("Read Dividents Import")

    Count = 0
    On Error GoTo EH

    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
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
            Err.Raise vbObjectError, "Load Stock Positions", "Wrong format of input file in line " & Count
            GoTo Skip
        End If
    
        For j = 0 To rsIndexList.RecordCount
            If mIdxSymArr(j) = Trim(Items(0)) Then
                LogMsg "Stock " & Trim(Items(0)) & " set as Index in ETS! It was Skipped!"
            GoTo Skip
            End If
        Next
        
        strXML = strXML + "<Divs Symbol=""" + GetValidXMLText(Trim(Items(0))) + """ " + "DivDate=""" + GetValidXMLText(Trim(Items(1))) + """ "
        strXML = strXML + "DivAmt=""" + GetValidXMLText(Trim(Items(2))) + """ " + "></Divs>"
        vCount = vCount + 1
        On Error GoTo ParseError
Skip:
        On Error Resume Next
        Idle
    Next
Done:
    On Error GoTo 0
    m_Parent = s_Parent
    If strXML = "<Dividents>" Then 'Empty
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
    LogMsg "Trade skipped. Error parsing trades data in line " & Count & " '" & Col & "'='" & Cell & "'. " & Msg
    Col = ""
    Cell = ""
    GoTo Skip
End Function
