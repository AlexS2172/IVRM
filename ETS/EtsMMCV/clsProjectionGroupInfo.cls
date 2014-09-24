VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsProjectionGroupInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Private Const FIELDS_DELIMITER As String = ","
Private Const INTERVAL_DELIMITER As String = ";"

Public VolaShiftCount As Long
Public SpotShiftCount As Long

Public OriginalStr As String

Public Shifts As Collection

Private Function GetGroupID(ByVal sGroupName As String) As Long
    On Error Resume Next
    GetGroupID = BAD_LONG_VALUE
    
    Dim aGroup As EtsGeneralLib.EtsUndGroupAtom
    Set aGroup = g_UnderlyingGroup.BySortKey(sGroupName)
    
    If Not aGroup Is Nothing Then
        GetGroupID = aGroup.ID
    End If
End Function

Public Function ParseFileLine(ByVal Str As String, ByVal StrNum As Long, ByVal File As String) As Boolean
    On Error Resume Next
    Dim vtGroupInfo As Variant
    Dim nGroupID As Long
    Dim sSymbol As String
    Dim nDateShift As Long
    Dim dtToday As Date, dtCalcDate As Date
    Dim nSpotValue As Long, nSpotEndValue As Long, nSpotStep As Long
    Dim nVolaValue As Long, nVolaEndValue As Long, nVolaStep As Long
    Dim aShift As clsStockShift
    
    ParseFileLine = False
     
    ' example of str "TRANSPORTATION,10,(-30;30;5),(-50;50;5)"
    If Str = "" Then Exit Function
    
    dtToday = GetNewYorkTime
    Str = Trim(Str)
    vtGroupInfo = Split(Str, FIELDS_DELIMITER)
    If UBound(vtGroupInfo) + 1 <> 4 Then
        LogEvent EVENT_ERROR, "Can't parse line #" & StrNum & " from projection file " & File
        Exit Function
    End If
    
    sSymbol = Trim(vtGroupInfo(0))
    If sSymbol = "<All>" Then
        nGroupID = -1
    Else
        nGroupID = GetGroupID(sSymbol)
        If nGroupID = BAD_LONG_VALUE Then
            LogEvent EVENT_ERROR, "Can't find group """ & sSymbol & """ from projection file " & File
            Exit Function
        End If
    End If
    
    nDateShift = CLng(vtGroupInfo(1))
    dtCalcDate = CDate(CDbl(dtToday) + CDbl(nDateShift))
    'dtCalcDate = DateSerial(Year(dtToday), Month(dtToday), Day(dtToday) + nDateShift)

    If Not ParseIntervalStr(Trim(vtGroupInfo(2)), nSpotValue, nSpotEndValue, nSpotStep) Or _
       Not ParseIntervalStr(Trim(vtGroupInfo(3)), nVolaValue, nVolaEndValue, nVolaStep) Then
       
        LogEvent EVENT_ERROR, "Can't parse line #" & StrNum & " from projection file " & File
        Exit Function
    End If
    
    Dim nStartVolaValue As Long: nStartVolaValue = nVolaValue
    
    While nSpotValue <= nSpotEndValue
        VolaShiftCount = 0
        nVolaValue = nStartVolaValue
        
        While nVolaValue <= nVolaEndValue
        
            Set aShift = New clsStockShift
            
            aShift.Symbol = sSymbol
            aShift.SymbolID = nGroupID
            aShift.IsGroup = True

            aShift.Shift = nSpotValue
            aShift.ShiftID = SpotShiftCount
            
            aShift.VolaShift = nVolaValue
            aShift.VolaShiftID = VolaShiftCount

            aShift.DateShift = nDateShift
            aShift.CalcDate = dtCalcDate

            Shifts.Add aShift

            Set aShift = Nothing
        
            VolaShiftCount = VolaShiftCount + 1
            nVolaValue = IIf(nVolaStep > 0, nVolaValue + nVolaStep, nVolaEndValue + 1)
        Wend
    
        SpotShiftCount = SpotShiftCount + 1
        nSpotValue = IIf(nSpotStep > 0, nSpotValue + nSpotStep, nSpotEndValue + 1)
    Wend
    
    OriginalStr = Str
    ParseFileLine = True
End Function

Private Function ParseIntervalStr(ByVal Str As String, nStart As Long, nEnd As Long, nStep As Long) As Boolean
    On Error GoTo EH
    Dim vtParams As Variant
    ParseIntervalStr = False
    
    ' example of str "(-30;30;5)"
    
    If Len(Str) < 7 Or Mid(Str, 1, 1) <> "(" Or Mid(Str, Len(Str)) <> ")" Then Exit Function
    
    Str = Mid(Str, 2, Len(Str) - 2)
    vtParams = Split(Str, INTERVAL_DELIMITER)
    
    If UBound(vtParams) + 1 <> 3 Then Exit Function
    
    nStart = CLng(vtParams(0))
    nEnd = CLng(vtParams(1))
    nStep = CLng(vtParams(2))
    
    If nStart > nEnd Or nStep < 0 Then Exit Function
    
    ParseIntervalStr = True
    Exit Function
EH:
    ParseIntervalStr = False
End Function

Private Sub Class_Initialize()
    On Error Resume Next
    Set Shifts = New Collection
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set Shifts = Nothing
End Sub
