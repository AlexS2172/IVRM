Attribute VB_Name = "basLicense"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Global g_sLicenseKey As String
Global g_lcLicense As typLicense
Global g_sLicenseCompany As String
Global g_sLicenseUser As String

Private Declare Function GetVolumeInformation Lib "kernel32" Alias "GetVolumeInformationA" _
                                            (ByVal lpRootPathName As String, _
                                             lpVolumeNameBuffer As Any, _
                                             ByVal nVolumeNameSize As Long, _
                                             lpVolumeSerialNumber As Long, _
                                             lpMaximumComponentLength As Any, _
                                             lpFileSystemFlags As Any, _
                                             lpFileSystemNameBuffer As Any, _
                                             ByVal nFileSystemNameSize As Long) As Long

Public Declare Function CalcCRC32 Lib "edaTools.dll" (lpData As Any, ByVal nDataLen As Long) As Long
Public Declare Function GetWindowsDirectory Lib "kernel32" Alias "GetWindowsDirectoryA" (ByVal lpBuffer As String, ByVal nSize As Long) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CheckLicense(ByRef frmOwner As Form, ByRef bNewLicense As Boolean) As Boolean
Dim bSucceed As Boolean
Dim bExpired As Boolean
Dim lcLicense As typLicense
Dim sKey As String
    On Error Resume Next
    bSucceed = False
    bExpired = False
    
    LoadLicense
    
    sKey = g_sLicenseKey
    bNewLicense = False
    Do While True
        bExpired = False
        If sKey <> "" Then
            If GetLicenseByKey(sKey, lcLicense) Then
                If lcLicense.UID = g_lcLicense.UID _
                    And lcLicense.Modules = g_lcLicense.Modules Then
                    
                    If lcLicense.Year > Year(Date) _
                        Or lcLicense.Year = Year(Date) _
                            And (lcLicense.Month > Month(Date) _
                                Or lcLicense.Month = Month(Date) And lcLicense.Day > Day(Date)) Then
                            bSucceed = True
                            g_lcLicense.Year = lcLicense.Year
                            g_lcLicense.Month = lcLicense.Month
                        g_lcLicense.Day = lcLicense.Day
                    End If
                    If bSucceed Then
                        Exit Do
                    End If
                    bExpired = True
                End If
            End If
        End If
        If bExpired Then
            gCmn.MyMsgBox "Your EGAR Hedger license is expired!" & vbCrLf & "Please contact technical support at 1-212-223-3552.", vbCritical
        Else
            gCmn.MyMsgBox "EGAR Hedger is not licensed," & vbCrLf & "or you have invalid license key!" & vbCrLf & "Please contact technical support at 1-212-223-3552.", vbCritical
        End If
        lcLicense = g_lcLicense
        bNewLicense = True
        If frmLicense.Execute(sKey, g_lcLicense.UID, frmOwner) <> vbOK Then
            Exit Do
        End If
    Loop
    
    If bSucceed Then
        If g_sLicenseKey <> sKey Then
            g_sLicenseKey = sKey
            g_lcLicense = lcLicense
            SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "License", "Key", g_sLicenseKey
        End If
    Else
        gCmn.MyMsgBox "EGAR Hedger is not licensed!" & vbCrLf & "Please contact technical support at 1-212-223-3552.", vbCritical
    End If
    CheckLicense = bSucceed
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadLicense() As Boolean
    On Error Resume Next
    Dim sUID As String, sTmp As String, lTmp As Long
    
    LoadLicense = False
    g_lcLicense.Modules = &H7FFFFFFF
    GetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "License", "Key", g_sLicenseKey, ""
    GetStrKeyValue HKEY_LOCAL_MACHINE, APP_REG_KEY & "License", "Company", g_sLicenseCompany, ""
    GetStrKeyValue HKEY_LOCAL_MACHINE, APP_REG_KEY & "License", "User", g_sLicenseUser, ""
    
    sTmp = String$(1024, 0)
    If GetWindowsDirectory(sTmp, 1024) <> 0 Then
        sTmp = Left$(sTmp, 3)
    Else
        sTmp = "C:\"
    End If
    
    lTmp = GetVolumeSerialNo(sTmp)
    If lTmp < 0 Then lTmp = -lTmp
    sUID = Hex$(lTmp)
    sUID = APP_NAME & sUID & g_sLicenseCompany & g_sLicenseUser
    
    g_lcLicense.UID = CalcCRC(sUID)
    If g_lcLicense.UID = 0 Then Exit Function
    LoadLicense = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetVolumeSerialNo(ByVal sPathName As String) As Long
    On Error Resume Next
    GetVolumeSerialNo = 0
    GetVolumeInformation sPathName, ByVal 0, 0, GetVolumeSerialNo, ByVal 0, ByVal 0, ByVal 0, 0
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function CalcCRC(ByVal sData As String) As Long
    On Error GoTo EH
    Dim nLen As Long
    CalcCRC = 0
    nLen = Len(sData)
    CalcCRC = CalcCRC32(ByVal sData, nLen)
    If CalcCRC < 0 Then CalcCRC = -CalcCRC
    Exit Function
EH:
    ShowError "Fail to get license."
End Function

