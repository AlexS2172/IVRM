VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVersionInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private Declare Function GetFileVersionInfo Lib "Version.dll" Alias "GetFileVersionInfoA" (ByVal lptstrFilename As String, ByVal dwhandle As Long, ByVal dwlen As Long, lpData As Any) As Long

Private Declare Function GetFileVersionInfoSize Lib "Version.dll" Alias "GetFileVersionInfoSizeA" (ByVal lptstrFilename As String, lpdwHandle As Long) As Long

Private Declare Function VerQueryValue Lib "Version.dll" Alias "VerQueryValueA" (pBlock As Any, ByVal lpSubBlock As String, lplpBuffer As Any, puLen As Long) As Long

Private Declare Sub MoveMemory Lib "kernel32" Alias "RtlMoveMemory" (Dest As Any, ByVal Source As Long, ByVal Length As Long)

Private Declare Function lstrcpy Lib "kernel32" Alias "lstrcpyA" (ByVal lpString1 As String, ByVal lpString2 As Long) As Long


Private Declare Function GetLastError Lib "kernel32" () As Long

Private m_sFilePath As String
Private m_sLangCharset As String
Private m_lBufferLen As Long
Private m_btBuffer() As Byte

Private Sub Class_Initialize()
    On Error Resume Next
    CleanUp
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    CleanUp
End Sub

Private Sub CleanUp()
    On Error Resume Next
    m_sFilePath = ""
    m_sLangCharset = ""
    m_lBufferLen = 0
    Erase m_btBuffer
End Sub

Public Function SetObjectGUID(ByVal sGUID As String) As Boolean
    On Error Resume Next
    Dim sFilePath$
    SetObjectGUID = False
    
    GetStrKeyValue HKEY_CLASSES_ROOT, "CLSID\" & sGUID & "\LocalServer32", "", sFilePath
    If Len(Trim$(sFilePath)) = 0 Then
        GetStrKeyValue HKEY_CLASSES_ROOT, "CLSID\" & sGUID & "\InprocServer32", "", sFilePath
    End If

    If Len(Trim$(sFilePath)) <> 0 Then
        SetObjectGUID = SetFilePath(sFilePath)
    End If
End Function

Public Function SetObjectProgID(ByVal sProgID As String) As Boolean
    On Error Resume Next
    Dim aGuid As GUID, sUID$, sTmp$, i&
    SetObjectProgID = False
    
    If CLSIDFromProgID(StrPtr(sProgID & Chr(0)), aGuid) = 0 Then
        sUID = "{"
        sTmp = Hex$(aGuid.Data1)
        sUID = sUID & String$(8 - Len(sTmp), "0") & sTmp & "-"
        sTmp = Hex$(aGuid.Data2)
        sUID = sUID & String$(4 - Len(sTmp), "0") & sTmp & "-"
        sTmp = Hex$(aGuid.Data3)
        sUID = sUID & String$(4 - Len(sTmp), "0") & sTmp & "-"
    
        For i = 0 To 7
            sTmp = Hex$(aGuid.Data4(i))
            sUID = sUID & String$(2 - Len(sTmp), "0") & sTmp
            If i = 1 Then sUID = sUID & "-"
        Next
        sUID = sUID & "}"
        
        SetObjectProgID = SetObjectGUID(sUID)
    End If
End Function

Public Function SetFilePath(ByVal sFilePath As String) As Boolean
    On Error Resume Next
    Dim sBuffer$, nLen&
    SetFilePath = False
    sFilePath = Trim$(sFilePath)
    
    If InStr(sFilePath, "\") = 0 And InStr(sFilePath, "/") = 0 And InStr(sFilePath, ":") = 0 Then
        Dim hModule&
        
        sBuffer = String$(2048, 0)
        hModule = 0&
        hModule = GetModuleHandle(sFilePath)
        If hModule <> 0& Then
            nLen = GetModuleFileName(hModule, sBuffer, 2048)
            If nLen > 0 Then sFilePath = Left$(sBuffer, nLen)
        Else
            hModule = LoadLibrary(sFilePath)
            If hModule <> 0& Then
                nLen = GetModuleFileName(hModule, sBuffer, 2048)
                If nLen > 0 Then sFilePath = Left$(sBuffer, nLen)
                FreeLibrary hModule
            End If
        End If
    End If
    
    sFilePath = GetLongPathNameEx(sFilePath)
    
    If m_sFilePath = sFilePath Then
        SetFilePath = True
        Exit Function
    ElseIf sFilePath = "" Then
        SetFilePath = False
        Exit Function
    End If
    
    CleanUp
    
    SetFilePath = True
    m_sFilePath = sFilePath
    
    Dim lDummy As Long, rc As Long
    
    m_lBufferLen = GetFileVersionInfoSize(m_sFilePath, lDummy)
    If m_lBufferLen < 1 Then
        lDummy = GetLastError
        Exit Function
    End If
    
    ReDim m_btBuffer(m_lBufferLen)
    rc = GetFileVersionInfo(m_sFilePath, 0&, m_lBufferLen, m_btBuffer(0))
    If rc = 0 Then Exit Function
    
    Dim lVerPointer As Long
    
    rc = VerQueryValue(m_btBuffer(0), "\VarFileInfo\Translation", lVerPointer, m_lBufferLen)
    
    If rc = 0 Then Exit Function
    
    'lVerPointer is a pointer to four 4 bytes of Hex number,
    'first two bytes are language id, and last two bytes are code
    'page. However, Lang_Charset_String needs a  string of
    '4 hex digits, the first two characters correspond to the
    'language id and last two the last two character correspond
    'to the code page id.
    
    Dim bytBuffer(255) As Byte
    MoveMemory bytBuffer(0), lVerPointer, m_lBufferLen
    Dim HexNumber As Long
    
    HexNumber = bytBuffer(2) + bytBuffer(3) * &H100 + bytBuffer(0) * &H10000 + bytBuffer(1) * &H1000000
    m_sLangCharset = Hex$(HexNumber)
    'now we change the order of the language id and code page
    'and convert it into a string representation.
    'For example, it may look like 040904E4
    'Or to pull it all apart:
    '04------        = SUBLANG_ENGLISH_USA
    '--09----        = LANG_ENGLISH
    '----04E4 = 1252 = Codepage for Windows:Multilingual
    
    Do While Len(m_sLangCharset) < 8
       m_sLangCharset = "0" & m_sLangCharset
    Loop
    
End Function

Public Property Get FilePath() As String
    On Error Resume Next
    FilePath = m_sFilePath
End Property

Public Property Get FileName() As String
    On Error Resume Next
    Dim oFS As Object, oFile As Object
    
    FileName = m_sFilePath
    Set oFS = CreateObject("Scripting.FileSystemObject")
    If Not oFS Is Nothing Then
        Set oFile = oFS.GetFile(m_sFilePath)
        If Not oFile Is Nothing Then
            FileName = oFile.Name
            Set oFile = Nothing
        End If
        Set oFS = Nothing
    End If
End Property

Private Function GetValue(ByVal strValueName As String) As String
    On Error Resume Next
    GetValue = ""
    If m_sFilePath = "" Then Exit Function
    
    Dim strBuffer As String, strTemp As String
    Dim rc As Long, lBufferLen As Long
    Dim lVerPointer As Long
    
    strBuffer = String$(1024, 0)
    strTemp = "\StringFileInfo\" & m_sLangCharset & "\" & strValueName
    rc = VerQueryValue(m_btBuffer(0), strTemp, lVerPointer, lBufferLen)
    
    If rc = 0 Then Exit Function
    
    lstrcpy strBuffer, lVerPointer
    strBuffer = Mid$(strBuffer, 1, InStr(strBuffer, Chr$(0)) - 1)
    GetValue = strBuffer
End Function

Public Property Get CompanyName() As String
    On Error Resume Next
    CompanyName = GetValue("CompanyName")
End Property

Public Property Get FileDescription() As String
    On Error Resume Next
    FileDescription = GetValue("FileDescription")
End Property

Public Property Get FileVersion() As String
    On Error Resume Next
    FileVersion = GetValue("FileVersion")
End Property

Public Property Get InternalName() As String
    On Error Resume Next
    InternalName = GetValue("InternalName")
End Property

Public Property Get LegalCopyright() As String
    On Error Resume Next
    LegalCopyright = GetValue("LegalCopyright")
End Property

Public Property Get OriginalFileName() As String
    On Error Resume Next
    OriginalFileName = GetValue("OriginalFileName")
End Property

Public Property Get ProductName() As String
    On Error Resume Next
    ProductName = GetValue("ProductName")
End Property

Public Property Get ProductVersion() As String
    On Error Resume Next
    ProductVersion = GetValue("ProductVersion")
End Property

Public Property Get FixedFileVersion() As String
    On Error Resume Next
    Dim sVersion$, arrParts() As String, nLBound&, nUBound&, nValue&, i&
    
    sVersion = Trim$(GetValue("FileVersion"))
    If sVersion = "" Then Exit Property
    
    sVersion = Replace$(sVersion, " ", "", 1, -1, vbTextCompare)
    sVersion = Replace$(sVersion, ",", ".", 1, -1, vbTextCompare)

    arrParts = Split(sVersion, ".")
    nLBound = 0
    nUBound = -1
    nLBound = LBound(arrParts)
    nUBound = UBound(arrParts)
    
    For i = nLBound To nUBound
        Err.Clear
        nValue = CLng(arrParts(i))
        If Err.Number <> 0 Then nValue = Val(arrParts(i))
        If Err.Number = 0 Then arrParts(i) = Trim$(Str$(nValue))
    Next
    
    Err.Clear
    FixedFileVersion = Join(arrParts, ".")
    If Err.Number <> 0 Then FixedFileVersion = sVersion
    
    Erase arrParts
End Property

Public Property Get FixedProductVersion() As String
    On Error Resume Next
    FixedProductVersion = GetValue("ProductVersion")
    If FixedProductVersion = "" Then Exit Property
    
    FixedProductVersion = Replace(FixedProductVersion, " ", "", 1, -1, vbTextCompare)
    FixedProductVersion = Replace(FixedProductVersion, ",", ".", 1, -1, vbTextCompare)
End Property




