Attribute VB_Name = "modLicense"
Option Explicit

Public Type typLicense
    UID As Long
    Modules As Long
    Day As Byte
    Month As Byte
    Year As Integer
End Type

Private Const PARITY_KEY As Double = 78.34646
'Private Const MAX_ENCODED As Long = 40
'Private Const MAX_KEY_LEN As Long = 60
Private Const PRODUCT_KEY_LEN As Long = 7
Private Const EXPDATE_KEY_LEN As Long = 6
Private Const MODULES_KEY_LEN As Long = 7
Private Const PARITY_KEY_LEN As Long = 4

Public Function GetLicenseByKey(ByVal sKey As String, lcLicense As typLicense) As Boolean
    On Error Resume Next
    GetLicenseByKey = False
    
    Dim sKeyLic As String: sKeyLic = ""
    Dim sParity As String: sParity = ""

    sKeyLic = Mid$(sKey, 1, PRODUCT_KEY_LEN + MODULES_KEY_LEN + EXPDATE_KEY_LEN)
    sParity = Mid$(sKey, PRODUCT_KEY_LEN + MODULES_KEY_LEN + EXPDATE_KEY_LEN + 1, PARITY_KEY_LEN)
    
    If Base36ToDec(sParity) <> CalculateParity(sKeyLic) Then Exit Function

    lcLicense.UID = Base36ToDec(Mid$(sKey, 1, PRODUCT_KEY_LEN))
    lcLicense.Modules = Base36ToDec(Mid$(sKey, PRODUCT_KEY_LEN + 1, MODULES_KEY_LEN))
    DecodeExpireDate Base36ToDec(Mid$(sKey, PRODUCT_KEY_LEN + MODULES_KEY_LEN + 1, EXPDATE_KEY_LEN)), lcLicense

    GetLicenseByKey = CheckLicData(lcLicense)
End Function

Private Function Base36ToDec(ByVal sSource As String) As Long
    On Error Resume Next
    Base36ToDec = 0
    If Len(sSource) <= 0 Then Exit Function
    
    Dim i As Long, lDigit As Long, sSymbol As String * 1
    Dim lLen As Long: lLen = Len(sSource)
    For i = lLen To 1 Step -1
        lDigit = 0
        sSymbol = Mid$(sSource, i, 1)

        If IsNumeric(sSymbol) Then
            lDigit = Asc(sSymbol) - Asc("0")
        Else
            lDigit = Asc(UCase$(sSymbol)) - Asc("A") + 10
        End If
    
        Base36ToDec = Base36ToDec + Int(lDigit * (36 ^ (lLen - i)))
    Next
End Function

Private Function CalculateParity(ByVal sSource As String) As Long
    On Error Resume Next
    CalculateParity = 0
    
    Dim dCoder As Double: dCoder = 3.12
    Dim i As Long, dRetVal As Double: dRetVal = 0#
    
    For i = 1 To Len(sSource)
        dRetVal = dRetVal + Asc(Mid$(sSource, i, 1)) * PARITY_KEY
    Next

    CalculateParity = Int(dRetVal)
End Function

Private Sub DecodeExpireDate(ByVal lCode As Long, lcLicense As typLicense)
    On Error Resume Next
    Dim lDecoded As Long
    lDecoded = ((lCode And &H2AAAAAAA) / 2) Or ((lCode And &H15555555) * 2)

    lcLicense.Day = CByte((lDecoded / (2 ^ 22)) And &HFF&)
    lcLicense.Month = CByte((lDecoded / (2 ^ 16)) And &HF&)
    lcLicense.Year = CInt(lDecoded And &HFFFF&)
End Sub

Private Function CheckLicData(lcLicense As typLicense) As Boolean
    On Error Resume Next
    CheckLicData = False
    If lcLicense.Day < 1 Or lcLicense.Day > 31 Then Exit Function
    If lcLicense.Month < 1 Or lcLicense.Month > 12 Then Exit Function
    If lcLicense.Year < 2000 Or lcLicense.Year > 9999 Then Exit Function
    If lcLicense.Modules = 0 Then Exit Function
    CheckLicData = True
End Function

' server side code
'Public Function GetKeyByLicense(lcLicense As typLicense, sKey As String) As Boolean
'    On Error Resume Next
'    GetKeyByLicense = False
'    sKey = ""
'
'    If Not CheckLicData(lcLicense) Then Exit Function
'
'    sKey = sKey & DecToBase36(lcLicense.UID, PRODUCT_KEY_LEN)
'    sKey = sKey & DecToBase36(lcLicense.Modules, MODULES_KEY_LEN)
'    sKey = sKey & DecToBase36(EncodeExpireDate(lcLicense), EXPDATE_KEY_LEN)
'    sKey = sKey & DecToBase36(CalculateParity(sKey), PARITY_KEY_LEN)
'
'    GetKeyByLicense = True
'End Function
'
'
'Private Function EncodeExpireDate(lcLicense As typLicense) As Long
'    On Error Resume Next
'    EncodeExpireDate = 0
'    EncodeExpireDate = (lcLicense.Month * (2 ^ 14)) Or lcLicense.Year
'    EncodeExpireDate = ((EncodeExpireDate And &H2AAAA) / 2) Or ((EncodeExpireDate And &H15555) * 2)
'End Function
'
'
'Private Function DecToBase36(ByVal lSource As Long, ByVal lMaxLen As Long) As String
'    On Error Resume Next
'    DecToBase36 = ""
'
'    Dim lDiv As Long: lDiv = lSource
'    Dim lMod As Long
'
'    While lDiv <> 0
'        lMod = lDiv Mod 36
'        lDiv = lDiv \ 36
'
'        DecToBase36 = DecToBase36 & Chr(CByte(lMod + IIf(lMod > 9, Asc("A") - 10, Asc("0"))))
'    Wend
'    If Len(DecToBase36) < lMaxLen Then
'        DecToBase36 = DecToBase36 & String(lMaxLen - Len(DecToBase36), "0")
'    End If
'
'    DecToBase36 = Left(StrReverse(DecToBase36), lMaxLen)
'End Function
'

