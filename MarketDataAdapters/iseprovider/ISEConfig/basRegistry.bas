Attribute VB_Name = "basRegistry"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

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
Public Const ERROR_FILE_NOT_FOUND = 2
Public Const ERROR_ACCESS_DENIED = 5
Public Const ERROR_MORE_DATA = 234&
Public Const ERROR_NO_MORE_ITEMS = 259&

Public Const REG_SZ = 1                         ' Unicode nul terminated string
Public Const REG_DWORD = 4                      ' 32-bit number

Public Type FILETIME
    dwLowDateTime As Long
    dwHighDateTime As Long
End Type

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegCreateKeyEx Lib "advapi32.dll" Alias "RegCreateKeyExA" _
( _
    ByVal hKey As Long _
  , ByVal lpSubKey As String _
  , ByVal Reserved As Long _
  , ByVal lpClass As String _
  , ByVal dwOptions As Long _
  , ByVal samDesired As Long _
  , lpSecurityAttributes As Any _
  , phkResult As Long _
  , lpdwDisposition As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegOpenKeyEx Lib "advapi32" Alias "RegOpenKeyExA" _
( _
    ByVal hKey As Long _
  , ByVal lpSubKey As String _
  , ByVal ulOptions As Long _
  , ByVal samDesired As Long _
  , ByRef phkResult As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegQueryValueEx Lib "advapi32" Alias "RegQueryValueExA" _
( _
    ByVal hKey As Long _
  , ByVal lpValueName As String _
  , ByVal lpReserved As Long _
  , ByRef lpType As Long _
  , ByRef lpData As Any _
  , ByRef lpcbData As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Note that if you declare the lpData parameter as String, you must pass it By Value.
'
Public Declare Function RegSetValueExA Lib "advapi32.dll" _
( _
    ByVal hKey As Long _
  , ByVal lpValueName As String _
  , ByVal Reserved As Long _
  , ByVal dwType As Long _
  , lpData As Any _
  , ByVal cbData As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegCloseKey Lib "advapi32" _
( _
    ByVal hKey As Long _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegDeleteKey Lib "advapi32" Alias "RegDeleteKeyA" _
( _
    ByVal hKey As Long _
  , ByVal lpSubKey As String _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Declare Function RegEnumKeyEx Lib "advapi32" Alias "RegEnumKeyExA" _
( _
    ByVal hKey As Long _
  , ByVal dwIndex As Long _
  , ByVal lpName As String _
  , lpcName As Long _
  , ByVal lpReserved As Long _
  , ByVal lpClass As String _
  , ByVal lpcClass As Long _
  , lpftLastWriteTime As FILETIME _
) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Win32Error(ByVal ErrCode As Long)
    If ErrCode = ERROR_ACCESS_DENIED Then
        Err.Raise vbObjectError, , "Failed to access registry. Access denied."
    Else
        Err.Raise ErrCode, , "Failed to access registry"
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' get string value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
'
Public Function GetStrKeyValueEx( _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    Optional ByVal strDefault As String = "" _
) As String
Dim strValue As String
    On Error Resume Next
    If Not GetStrKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, strValue) Then
        If Not GetStrKeyValue(HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue) Then
            strValue = strDefault
        End If
    End If
    GetStrKeyValueEx = strValue
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' get long value from HKEY_CURRENT_USER and if fail then from HKEY_LOCAL_MACHINE
'
Public Function GetLongKeyValueEx( _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    Optional ByVal lngDefault As Long = 0 _
) As Long
Dim nValue As Long
    On Error Resume Next
    If Not GetLongKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, nValue) Then
        If Not GetLongKeyValue(HKEY_LOCAL_MACHINE, strKeyName, strValueName, nValue) Then
            nValue = lngDefault
        End If
    End If
    GetLongKeyValueEx = nValue
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetLongKeyValueEx( _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    ByVal lngValue As Long _
) As Boolean
    SetLongKeyValueEx = SetLongKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, lngValue)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStrKeyValueEx( _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    ByVal strValue As String _
) As Boolean
    SetStrKeyValueEx = SetStrKeyValue(HKEY_CURRENT_USER, strKeyName, strValueName, strValue)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' get string value from registry
'
Public Function GetStrKeyValue( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    strValue As String _
) As Boolean
Dim rc As Long
Dim hKey As Long
Dim KeyValType As Long
Dim tmpVal As String
Dim KeyValSize As Long
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey)
    If rc = ERROR_SUCCESS Then
        
        KeyValSize = 16384                                          ' Mark Variable Size
        tmpVal = String$(KeyValSize, vbNullChar)                    ' Allocate Variable Space
        
        rc = RegQueryValueEx(hKey, strValueName, 0, KeyValType, ByVal tmpVal, KeyValSize)
        If rc = ERROR_SUCCESS And KeyValType = REG_SZ Then
            If KeyValSize <= 0 Then
                strValue = ""
            ElseIf (Asc(Mid$(tmpVal, KeyValSize, 1)) = 0) Then      ' Win95 Adds Null Terminated String...
                strValue = Left$(tmpVal, KeyValSize - 1)            ' Null Found, Extract From String
            Else                                                    ' WinNT Does NOT Null Terminate String...
                strValue = Left$(tmpVal, KeyValSize)                ' Null Not Found, Extract String Only
            End If
            GetStrKeyValue = True
        ElseIf rc = ERROR_FILE_NOT_FOUND Then
            '
        Else
            Win32Error rc
        End If
    ElseIf rc = ERROR_FILE_NOT_FOUND Then
        '
    Else
        Win32Error rc
    End If
    rc = RegCloseKey(hKey)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' get long value from registry
'
Public Function GetLongKeyValue( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    lngValue As Long _
) As Boolean
Dim rc As Long
Dim hKey As Long
Dim KeyValType As Long
Dim tmpVal As Long
Dim KeyValSize As Long
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey)
    If rc = ERROR_SUCCESS Then
        KeyValSize = 4
        rc = RegQueryValueEx(hKey, strValueName, 0, KeyValType, tmpVal, KeyValSize)
        If rc = ERROR_SUCCESS And KeyValType = REG_DWORD Then
            lngValue = tmpVal
            GetLongKeyValue = True
        ElseIf rc = ERROR_FILE_NOT_FOUND Then
            '
        Else
            Win32Error rc
        End If
    ElseIf rc = ERROR_FILE_NOT_FOUND Then
        '
    Else
        Win32Error rc
    End If
    rc = RegCloseKey(hKey)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' write string value from registry
'
Public Function SetStrKeyValue( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    ByVal strValue As String _
) As Boolean
Dim rc As Long
Dim hKey As Long
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    If rc = ERROR_SUCCESS Then
        If Len(strValue) > 0 Then
            rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal strValue, Len(strValue))
            If rc = ERROR_SUCCESS Then
                SetStrKeyValue = True
            Else
                Win32Error rc
            End If
        Else
            rc = RegSetValueExA(hKey, strValueName, 0, REG_SZ, ByVal 0, 0)
            If rc = ERROR_SUCCESS Then
                SetStrKeyValue = True
            Else
                Win32Error rc
            End If
        End If
    Else
        Win32Error rc
    End If
    rc = RegCloseKey(hKey)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' write string value from registry
'
Public Function SetLongKeyValue( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String, _
    ByVal strValueName As String, _
    ByVal lngValue As Long _
) As Boolean
Dim rc As Long
Dim hKey As Long
    rc = RegCreateKeyEx(lngKeyRoot, strKeyName, 0, "", 0, KEY_ALL_ACCESS, ByVal 0, hKey, ByVal 0)
    If rc = ERROR_SUCCESS Then
        rc = RegSetValueExA(hKey, strValueName, 0, REG_DWORD, lngValue, 4)
        If rc = ERROR_SUCCESS Then
            SetLongKeyValue = True
        Else
            Win32Error rc
        End If
    Else
        Win32Error rc
    End If
    rc = RegCloseKey(hKey)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DeleteKey( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String _
)
Dim rc As Long
    rc = RegDeleteKey(lngKeyRoot, strKeyName)
    If rc = ERROR_SUCCESS Then
    
    ElseIf rc = ERROR_FILE_NOT_FOUND Then
    
    Else
        Win32Error rc
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSubkeys( _
    ByVal lngKeyRoot As Long, _
    ByVal strKeyName As String _
) As Collection

Dim rc As Long
Dim hKey As Long
Dim Buffer As String
Dim BufferSize As Long
Dim LastWriteTime As FILETIME
Dim dwIndex As Long
Dim strValueName As String

    Set GetSubkeys = New Collection
    
    rc = RegOpenKeyEx(lngKeyRoot, strKeyName, 0, KEY_READ, hKey)
    If rc = ERROR_SUCCESS Then
        dwIndex = 0
        Do
            BufferSize = 255
            Buffer = String$(BufferSize, vbNullChar)
            rc = RegEnumKeyEx( _
                hKey _
              , dwIndex _
              , Buffer _
              , BufferSize _
              , 0& _
              , vbNullString _
              , 0& _
              , LastWriteTime _
            )
            Select Case rc
            Case ERROR_NO_MORE_ITEMS
                Exit Do
            Case ERROR_SUCCESS, ERROR_MORE_DATA
                If BufferSize <= 0 Then
                    strValueName = ""
                ElseIf (Asc(Mid$(Buffer, BufferSize, 1)) = 0) Then
                    strValueName = Left$(Buffer, BufferSize - 1)
                Else
                    strValueName = Left$(Buffer, BufferSize)
                End If
                GetSubkeys.Add strValueName
            Case Else
                Win32Error rc
                Exit Do
            End Select
            dwIndex = dwIndex + 1
        Loop
    ElseIf rc = ERROR_FILE_NOT_FOUND Then
        '
    Else
        Win32Error rc
    End If
    rc = RegCloseKey(hKey)
End Function

