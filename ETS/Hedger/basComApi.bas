Attribute VB_Name = "basComApi"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Public Const strIUnknown = "{00000000-0000-0000-C000-000000000046}"

Private Declare Function CLSIDFromProgID Lib "ole32.dll" _
    (ByVal pOLESTR As Long, pCLSID As REFCLSID) As Long

'WINOLEAPI CoGetPSClsid(
'  REFIID riid,   //Interface whose proxy/stub CLSID is to be
'                 // returned
'  CLSID *pclsid  //Where to store returned proxy/stub CLSID
');


Private Type REFCLSID
    d1 As Long
    D2a As Integer
    D2b As Integer
    d3(0 To 7) As Byte
End Type

' system error mesages
Private Declare Function GetLastError Lib "kernel32" () As Long
Private Declare Function FormatMessage Lib "kernel32" Alias "FormatMessageA" (ByVal dwFlags As Long, lpSource As Any, ByVal dwMessageId As Long, ByVal dwLanguageId As Long, ByVal lpBuffer As String, ByVal nSize As Long, Arguments As Long) As Long

Public Enum FORMAT_MESSAGE
    FORMAT_MESSAGE_MAX_WIDTH_MASK = &HFF
    FORMAT_MESSAGE_ALLOCATE_BUFFER = &H100
    FORMAT_MESSAGE_IGNORE_INSERTS = &H200
    FORMAT_MESSAGE_FROM_STRING = &H400
    FORMAT_MESSAGE_FROM_HMODULE = &H800
    FORMAT_MESSAGE_FROM_SYSTEM = &H1000
    FORMAT_MESSAGE_ARGUMENT_ARRAY = &H2000
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function vbCLSIDFromProgID(ByVal ProgID As String) As String
Dim ClassID As REFCLSID
Dim s
    HResult CLSIDFromProgID(StrPtr(ProgID & Chr(0)), ClassID)
    s = "{" & _
        HexLong(ClassID.d1) & "-" & _
        HexWord(ClassID.D2a) & "-" & _
        HexWord(ClassID.D2b) & "-" & _
        HexByte(ClassID.d3(0)) & _
        HexByte(ClassID.d3(1)) & "-" & _
        HexByte(ClassID.d3(2)) & _
        HexByte(ClassID.d3(3)) & _
        HexByte(ClassID.d3(4)) & _
        HexByte(ClassID.d3(5)) & _
        HexByte(ClassID.d3(6)) & _
        HexByte(ClassID.d3(7)) & _
        "}"
    Debug.Print s
    vbCLSIDFromProgID = s
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function HexByte(ByVal d As Byte) As String
    HexByte = AddLeadingZeros(Hex(d), 2)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function HexWord(ByVal d As Integer) As String
    HexWord = AddLeadingZeros(Hex(d), 4)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function HexLong(ByVal d As Long) As String
    HexLong = AddLeadingZeros(Hex(d), 8)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function AddLeadingZeros(s As String, ByVal n As Long) As String
Dim k As Long
    k = n - Len(s)
    If k > 0 Then
        AddLeadingZeros = String(k, "0") & s
    Else
        AddLeadingZeros = s
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function HResult(ByVal r As Long) As Long
Dim s As String
    HResult = r
    If r <> 0 Then
        s = GetSystemMessage(r)
        Debug.Print "HResult " & Hex(r) & " " & s
        'vbObjectError + 29000 +
        Err.Raise r, "Win32", s
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSystemMessage(ByVal ErrCode As Long) As String
Dim MsgBuf As String
Dim MsgRet As String
Dim BufLen As Long
Dim RetLen As Long
    MsgBuf = String(2048, vbNullChar)
    BufLen = Len(MsgBuf)
    RetLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM Or FORMAT_MESSAGE_IGNORE_INSERTS, 0&, ErrCode, 0, MsgBuf, BufLen, 0&)
    If RetLen = 0 Then
        MsgRet = "Call FormatMessage Error!"
    Else
        MsgRet = Left$(MsgBuf, RetLen)
    End If
    GetSystemMessage = "(0x" & Hex(ErrCode) & ") " & MsgRet
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetPathFromFile(ByVal FileName As String) As String
Dim ProgID As String
    On Error GoTo Herr
    ProgID = GetProgIDFromFileName(FileName)
    If Len(ProgID) > 0 Then
        GetPathFromFile = GetPathFromProgID(ProgID)
    End If
    Exit Function
Herr:
    Debug.Print Err.Description
    Debug.Assert False
    GetPathFromFile = ""
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetPathFromProgID(ByVal ProgID As String) As String
Dim Guid As String
    Guid = GetGuidFromProgID(ProgID)
    If Len(Guid) > 0 Then
        GetPathFromProgID = GetPathFromGUID(Guid)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetProgIDFromFileName(ByVal FileName As String) As String
Dim a As ADODB.Recordset
    Select Case FileName
    Case "MSAdo15.dll":             GetProgIDFromFileName = "ADODB.Recordset"
    Case "Dao360.dll":              GetProgIDFromFileName = "DAO.DBEngine.36"
    Case "Msinet.ocx":              GetProgIDFromFileName = "InetCtls.Inet.1"
    Case "DHIVFeeder4.dll":         GetProgIDFromFileName = "DHIVFeeder4.DHIVLoader"
    Case "DHMTFeeder2.dll":         GetProgIDFromFileName = "DHMTFeeder2.DHMTLoader"
    Case "MyTrackProviders.exe":    GetProgIDFromFileName = "MyTrackProviders.MyTrackPriceProvider"
    Case "PriceProviders.dll":      GetProgIDFromFileName = "PRICEPROVIDERS.PriceInfo"
    Case "ElladaFlatControls.ocx":  GetProgIDFromFileName = "ElladaFlatControls.ActiveLabel"
    Case "FocusFlatControls1.ocx":   GetProgIDFromFileName = "FocusFlatControls1.FlatButton"
    Case "DHlButtons.ocx":          GetProgIDFromFileName = "DHlButtons.btnCancel"
    Case "axButton.ocx":            GetProgIDFromFileName = "axButtonControl.axButton"
    Case "workflowic.dll":          GetProgIDFromFileName = "WorkFlowIC.MD5"
    Case "Vsflex7.ocx":             GetProgIDFromFileName = "VSFlexGrid.VSFlexGrid"
    Case "Vsocx6.ocx":              GetProgIDFromFileName = "vsElastic.vsElastic.1"
    Case "VSPrint7.ocx":            GetProgIDFromFileName = "VSPrinter7.VSPrinter.1"
    Case Else:                      GetProgIDFromFileName = ""
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGuidFromProgID(ByVal ProgID As String) As String
    Select Case ProgID
'    Case "ADODB.Recordset":                             GetGuidFromProgID = "{00000535-0000-0010-8000-00AA006D2EA4}"
'    Case "DAO.DBEngine.36":                             GetGuidFromProgID = "{00000100-0000-0010-8000-00AA006D2EA4}"
'    Case "DHIVFeeder4.DHIVLoader":                      GetGuidFromProgID = "{983C6610-09EC-46C3-B342-C9032D96454E}"
'    Case "DHMTFeeder1.DHMTLoader":                      GetGuidFromProgID = "{DDE6C58F-3E16-4FCE-98DB-36FAF3D8C634}"
'    Case "MyTrackProviders.MyTrackPriceProvider":       GetGuidFromProgID = "{FD68F05A-247C-4D19-B12D-852691429151}"
'    Case "ElladaFlatControls.ActiveLabel":              GetGuidFromProgID = "{C3957760-A0DD-448F-9DE4-0CDB873A029C}"
'    Case "FocusFlatControls1.FlatButton":                GetGuidFromProgID = "{AA2209D1-17AA-4DF7-B06E-3E47CDDB5CDC}"
'    Case "axButtonControl.axButton":                    GetGuidFromProgID = "{9EEA74CB-D2D8-11D2-8AAE-0008C7D2400A}"
'    Case "DHlButtons.btnCancel":                        GetGuidFromProgID = "{78F6CA6D-4FE9-4BEE-A8FB-71F8DD5070BC}"
'    Case "WorkFlowIC.MD5":                              GetGuidFromProgID = "{ED6C5EAF-781E-11D2-A932-0060977EAB89}"
    Case Else
        GetGuidFromProgID = vbCLSIDFromProgID(ProgID)
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetPathFromGUID(ByVal Guid As String) As String
Dim Key As String
Dim s As String
    Key = GetInprocServer32Key(Guid)
    GetStrKeyValue HKEY_CLASSES_ROOT, Key, "", s
    GetPathFromGUID = s
    If GetPathFromGUID = "" Then
        Key = GetLocalServer32Key(Guid)
        GetStrKeyValue HKEY_CLASSES_ROOT, Key, "", s
        GetPathFromGUID = s
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetInprocServer32Key(ByVal Guid As String) As String
    GetInprocServer32Key = "CLSID\" & Guid & "\InprocServer32\"
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetLocalServer32Key(ByVal Guid As String) As String
    GetLocalServer32Key = "CLSID\" & Guid & "\LocalServer32\"
End Function

