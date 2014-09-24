VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeLogColl"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Description = "Trade log collection"
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Collection" ,"clsTradeLogString"
Attribute VB_Ext_KEY = "Member0" ,"clsTradeLogString"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'local variable to hold collection
Private m_Col As Collection
Public m_nTradesCount As Long

Public Function Add(Optional aTradeLog As clsTradeLogString = Nothing) As clsTradeLogString
    On Error Resume Next
    'create a new object
    Dim objNewMember As clsTradeLogString
    
    If aTradeLog Is Nothing Then
        Set objNewMember = New clsTradeLogString
    Else
        Set objNewMember = aTradeLog
    End If

    'set the properties passed into the method
    m_Col.Add objNewMember

    'return the object created
    Set Add = objNewMember
    Set objNewMember = Nothing
End Function

Public Property Get Item(vntIndexKey As Variant) As clsTradeLogString
Attribute Item.VB_UserMemId = 0
    'used when referencing an element in the collection
    'vntIndexKey contains either the Index or Key to the collection,
    'this is why it is declared as a Variant
    'Syntax: Set foo = x.Item(xyz) or Set foo = x.Item(5)
    On Error Resume Next
    Set Item = m_Col(vntIndexKey)
End Property



Public Property Get Count() As Long
    'used when retrieving the number of elements in the
    'collection. Syntax: Debug.Print x.Count
    Count = m_Col.Count
End Property


Public Sub Remove(vntIndexKey As Variant)
    'used when removing an element from the collection
    'vntIndexKey contains either the Index or Key, which is why
    'it is declared as a Variant
    'Syntax: x.Remove(xyz)
    On Error Resume Next
    m_Col.Remove vntIndexKey
End Sub


Public Property Get NewEnum() As IUnknown
Attribute NewEnum.VB_UserMemId = -4
Attribute NewEnum.VB_MemberFlags = "40"
    'this property allows you to enumerate
    'this collection with the For...Each syntax
    On Error Resume Next
    Set NewEnum = m_Col.[_NewEnum]
End Property


Private Sub Class_Initialize()
    'creates the collection when this class is created
    Set m_Col = New Collection
End Sub


Private Sub Class_Terminate()
    'destroys collection when this class is terminated
    Set m_Col = Nothing
End Sub

Public Sub Clear()
    On Error Resume Next
    m_nTradesCount = 0
    Set m_Col = Nothing
    Set m_Col = New Collection
End Sub

Public Sub CreateTradeLog(nCount As Long)
    On Error Resume Next
    Dim sFirmName As String
    Dim sTradeLogDir As String
    Dim sTradeLogFileName As String
    Dim nLogID As Long
    Dim dtNow As Date
    
    dtNow = Now()
    sFirmName = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "FirmName", "")
    sTradeLogDir = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "TradeLogDir", "C:\Program Files\Common Files\egar\Logs\")
    If Mid(sTradeLogDir, Len(sTradeLogDir), 1) <> "\" Then sTradeLogDir = sTradeLogDir & "\"
    sTradeLogFileName = "ftptrades_" & sFirmName & "_" & Format(dtNow, "yyMMDD") _
                                                 & "_" & Format(dtNow, "HHmm") & ".txt"
    
    nLogID = FreeFile()
    Open sTradeLogDir & sTradeLogFileName For Output Access Write As #nLogID
    Print #nLogID, "Egar trades log for " & sFirmName & ", " & Format(dtNow, "yyMMDD") _
                                                      & ", " & Format(dtNow, "HHmm")
    Print #nLogID, "Processed " & CStr(nCount) & " out of " & CStr(m_nTradesCount) _
                                               & " trades, found " & CStr(Count()) _
                                               & " errors."
    
    Dim aTLogString As clsTradeLogString
    For Each aTLogString In Me
        Print #nLogID, Format(aTLogString.m_nTradeID, "000000") & _
                       Format(aTLogString.m_dtExecutionDate, "HHmm") & _
                       Format(aTLogString.m_dtExecutionDate, "yyMMDD") & _
                       Format(aTLogString.m_nErrorID, "0000")
    Next
    
    Close #nLogID
End Sub

