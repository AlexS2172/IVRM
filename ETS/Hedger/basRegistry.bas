Attribute VB_Name = "basRegistry"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal Reserved As Long, ByVal lpClass As String, ByVal dwOptions As Long, ByVal samDesired As Long, lpSecurityAttributes As Any, phkResult As Long, lpdwDisposition As Long) As Long
Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" (ByVal hKey As Long, ByVal lpSubKey As String, ByVal ulOptions As Long, ByVal samDesired As Long, ByRef phkResult As Long) As Long
Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" (ByVal hKey As Long, ByVal lpValueName As String, ByVal lpReserved As Long, ByRef lpType As Long, ByRef lpData As Any, ByRef lpcbData As Long) As Long
Declare Function RegSetValueExA Lib "advapi32.dll" (ByVal hKey As Long, ByVal lpValueName As String, ByVal Reserved As Long, ByVal dwType As Long, lpData As Any, ByVal cbData As Long) As Long                                ' Note that if you declare the lpData parameter as String, you must pass it By Value.
Declare Function RegCloseKey Lib "advapi32" (ByVal hKey As Long) As Long
Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

' Reg Key Security Options...
Public Const READ_CONTROL = &H20000
Public Const KEY_QUERY_VALUE = &H1
Public Const KEY_SET_VALUE = &H2
Public Const KEY_CREATE_SUB_KEY = &H4
Public Const KEY_ENUMERATE_SUB_KEYS = &H8
Public Const KEY_NOTIFY = &H10
Public Const KEY_CREATE_LINK = &H20
Public Const KEY_ALL_ACCESS = KEY_QUERY_VALUE + KEY_SET_VALUE + KEY_CREATE_SUB_KEY + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY + KEY_CREATE_LINK + READ_CONTROL
Public Const KEY_READ = READ_CONTROL + KEY_QUERY_VALUE + KEY_ENUMERATE_SUB_KEYS + KEY_NOTIFY
                     
' Reg Key ROOT Types...
Public Const HKEY_CLASSES_ROOT = &H80000000
Public Const HKEY_CURRENT_USER = &H80000001
Public Const HKEY_LOCAL_MACHINE = &H80000002
Public Const ERROR_SUCCESS = 0
Public Const REG_SZ = 1                         ' Unicode nul terminated string
Public Const REG_DWORD = 4                      ' 32-bit number


' get string value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
Public Function GetStrKeyValueEx(strKeyName As String, strValueName As String, Optional ByVal strDefault As String = "") As String
    On Error Resume Next
    Dim strValue$
    
    If Not GetStrKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, strValue, strDefault) Then _
        GetStrKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue, strDefault
        
    GetStrKeyValueEx = strValue
End Function

' get long value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
Public Function GetLongKeyValueEx(strKeyName As String, strValueName As String, Optional ByVal lngDefault As Long = 0) As Long
    On Error Resume Next
    Dim nValue&
    
    If Not GetLongKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, nValue, lngDefault) Then _
        GetLongKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, nValue, lngDefault
        
    GetLongKeyValueEx = nValue
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

' get long value from registry
Public Function GetLongKeyValue(lngKeyRoot As Long, strKeyName As String, strValueName As String, lngValue As Long, Optional ByVal lngDefault As Long = 0) As Boolean
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    Dim KeyValType As Long                                  ' Data Type Of A Registry Key
    Dim tmpVal As Long                                      ' Tempory Storage For A Registry Key Value
    Dim KeyValSize As Long                                  ' Size Of Registry Key Variable
    '------------------------------------------------------------
    ' Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey) ' Open Registry Key
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    
    KeyValSize = 4                                          ' Mark Variable Size
    
    '------------------------------------------------------------
    ' Retrieve Registry Key Value...
    '------------------------------------------------------------
    rc = RegQueryValueEx(hKey, strValueName, 0, _
                         KeyValType, tmpVal, KeyValSize)    ' Get/Create Key Value
                        
    If rc <> ERROR_SUCCESS Or KeyValType <> REG_DWORD Then GoTo GetKeyError            ' Handle Errors
    
    lngValue = tmpVal                                ' Return Value
    GetLongKeyValue = True
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
    Exit Function                                           ' Exit
    
GetKeyError:      ' Cleanup After An Error Has Occured...
    GetLongKeyValue = False
    lngValue = lngDefault                            ' Set Return Val To Default value
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Function


' write string value from registry
Public Sub SetStrKeyValue(ByVal lngKeyRoot As Long, ByVal strKeyName As String, ByVal strValueName As String, ByVal strValue As String)
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    '------------------------------------------------------------
    ' Create or Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    '------------------------------------------------------------
    ' Set Registry Key Value...
    '------------------------------------------------------------
    If Len(strValue) > 0 Then
        'rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal strValue, CLng(Len(strValue) + 1))
        rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal strValue, CLng(Len(strValue)))
    Else
        rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal 0, 0)
    End If
                        
GetKeyError:      ' Cleanup After An Error Has Occured...
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Sub

' write string value from registry
Public Sub SetLongKeyValue(ByVal lngKeyRoot As Long, ByVal strKeyName As String, ByVal strValueName As String, ByVal lngValue As Long)
    On Error Resume Next
    Dim rc As Long                                          ' Return Code
    Dim hKey As Long                                        ' Handle To An Open Registry Key
    '------------------------------------------------------------
    ' Create or Open RegKey Under lngKeyRoot {HKEY_LOCAL_MACHINE...}
    '------------------------------------------------------------
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    
    If (rc <> ERROR_SUCCESS) Then GoTo GetKeyError          ' Handle Error...
    '------------------------------------------------------------
    ' Set Registry Key Value...
    '------------------------------------------------------------
    rc = RegSetValueExA(hKey, strValueName, 0, REG_DWORD, lngValue, 4)
                        
GetKeyError:      ' Cleanup After An Error Has Occured...
    rc = RegCloseKey(hKey)                                  ' Close Registry Key
End Sub

