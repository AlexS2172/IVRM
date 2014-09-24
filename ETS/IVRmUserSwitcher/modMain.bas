Attribute VB_Name = "modMain"
Public Const PRODUCT_REG_KEY As String = "SOFTWARE\Egar\ETS\"
' Reg Key ROOT Types...
Public Const HKEY_LOCAL_MACHINE As Long = &H80000002
Public Const HKEY_CURRENT_USER As Long = &H80000001
Public Const HKEY_CLASSES_ROOT As Long = &H80000000

' Reg Key Security Options...
Public Const READ_CONTROL As Long = &H20000
Public Const KEY_QUERY_VALUE As Long = &H1
Public Const KEY_SET_VALUE As Long = &H2
Public Const KEY_CREATE_SUB_KEY As Long = &H4
Public Const KEY_ENUMERATE_SUB_KEYS As Long = &H8
Public Const KEY_NOTIFY As Long = &H10
Public Const KEY_CREATE_LINK As Long = &H20
Public Const KEY_ALL_ACCESS As Long = KEY_QUERY_VALUE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
Public Const KEY_READ As Long = READ_CONTROL + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY

Public Const APP_XML_KEY As String = "ETS\MarketMaker\"
Public Const PRODUCT_XML_KEY As String = "ETS\"
Public Const GROUP_XML_KEY As String = "ETS\Asp\Groups\"


Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Any, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByRef lpData As Any, ByRef lpcbData As Long) As Long
Declare Function RegSetValueExA Lib "advapi32.dll" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long                                ' Note that if you declare the lpData parameter as String, you must pass it By Value.
Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
Public Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" (ByVal lpBuffer As String, nSize As Long) As Long

Public Const ERROR_SUCCESS As Long = 0
Public Const REG_SZ As Long = 1                         ' Unicode nul terminated string
Public Const REG_DWORD As Long = 4                      ' 32-bit number


Public g_XMLParams As ETSXMLParamsLib.XMLParams


Public Function MainXMLFilePath$()
    On Error Resume Next
    MainXMLFilePath = GetStrKeyValueEx(PRODUCT_REG_KEY & "Asp" & "\Settings", "PathToMainXML", "")
End Function

Public Function GetStrKeyValueEx(strKeyName As String, strValueName As String, Optional ByVal strDefault As String = "") As String
    On Error Resume Next
    Dim strValue$
    
    If Not GetStrKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, strValue, strDefault) Then _
        GetStrKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue, strDefault
        
    GetStrKeyValueEx = strValue
End Function


' get string value from registry
Public Function GetStrKeyValue(lngKeyRoot As Long, strKeyName As String, strValueName As String, strValue As String, Optional ByVal strDefault As String = "") As Boolean
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    Dim KeyValType As Long                                  ' Data Type Of A Registry Key
    Dim tmpVal As String                                    ' Tempory Storage For A Registry Key Value
    Dim KeyValSize As Long                                  ' Size Of Registry Key Variable
    '------------------------------------------------------------
    ' Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey) ' Open Registry Key
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    
    tmpVal = String$(16384, 0)                              ' Allocate Variable Space
    KeyValSize = 16384                                      ' Mark Variable Size
    
    '------------------------------------------------------------
    ' Retrieve Registry Key Value...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, strValueName, 0, _
                         KeyValType, ByVal tmpVal, KeyValSize)    ' Get/Create Key Value
                        
    If rc <> ERROR_SUCCESS Or KeyValType <> REG_SZ Then GoTo GetKeyError          ' Handle Errors
    
    If KeyValSize <= 0 Then
        tmpVal = ""
    ElseIf (Asc(Mid$(tmpVal, KeyValSize, 1)) = 0) Then           ' Win95 Adds Null Terminated String...
        tmpVal = Left$(tmpVal, KeyValSize - 1)               ' Null Found, Extract From String
    Else                                                    ' WinNT Does NOT Null Terminate String...
        tmpVal = Left$(tmpVal, KeyValSize)                   ' Null Not Found, Extract String Only
    End If
    
    strValue = tmpVal                                       ' Return Success
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
    GetStrKeyValue = True
    Exit Function                                           ' Exit
    
GetKeyError:      ' Cleanup After An Error Has Occured...
    GetStrKeyValue = False
    strValue = strDefault                                   ' Set Return Val To Default String
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Function


Public Function CurrentNtUserName$()
    On Error Resume Next
    Dim sUserName$, nSize&
    sUserName = String$(512, 0)
    nSize = 512
    
    If GetUserName(sUserName, nSize) <> 0 Then
        CurrentNtUserName = Left$(sUserName, nSize - 1)
    Else
        CurrentNtUserName = ""
    End If
End Function

