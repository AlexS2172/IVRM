VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsSettingsStorage"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private Const ETS_SS_VERSION As String = "1.0"

Private m_sFileName As String
Private m_Sections As New clsSettingSectionColl

Public Sub Init(ByVal sFileName As String)
    On Error Resume Next
    
    m_Sections.Clear
    m_sFileName = Trim$(sFileName)
End Sub

Public Sub ReadData()
    On Error Resume Next
    Dim sValue$, sKey$, nPos&, bVersionValid As Boolean
    Dim aSection As clsSettingSectionAtom, nFile&
    Dim nErrNumber&, sErrDesc$
    
    m_Sections.Clear
    
    nFile = FreeFile
    Err.Clear
    
    Open m_sFileName For Input Access Read Lock Write As #nFile
    If Err.Number <> 0 Then
        sErrDesc = Err.Description
        nErrNumber = Err.Number
        Close #nFile
        On Error GoTo 0
        Err.Raise nErrNumber, "Settings Storage", "Fail to read " & AppTitle() & " Settings Storage file '" & m_sFileName & "'. " & sErrDesc
        Exit Sub
    End If
    
    ' check file version
    bVersionValid = False
    Err.Clear
    Line Input #nFile, sValue
    If Err.Number = 0 And UCase$(Trim$(sValue)) = UCase$("[ETS Settings Storage]") Then
        Line Input #nFile, sValue
        If Err.Number = 0 And UCase$(Trim$(sValue)) = UCase$("Version=" & ETS_SS_VERSION) Then
            bVersionValid = True
        End If
    End If
    
    If Not bVersionValid Then
        Close #nFile
        On Error GoTo 0
        Err.Raise vbObjectError + 512, "Settings Storage", "Fail to read " & AppTitle() & " Settings Storage file '" & m_sFileName & "'. Invalid file version, " & ETS_SS_VERSION & " expected."
        Exit Sub
    End If

    ' read data
    While Not EOF(nFile)
        Line Input #nFile, sValue
        If InStr(sValue, "[") = 1 And InStr(sValue, "]") = Len(sValue) Then  ' Section
            Set aSection = Nothing
            Set aSection = m_Sections.Add(sValue)
            
        ElseIf Not aSection Is Nothing Then
            nPos = InStr(sValue, "=")
            If nPos > 0 Then
                sKey = Left$(sValue, nPos - 1)
                sValue = Mid$(sValue, nPos + 1)
                aSection.Values.Add sKey, sValue
            End If
        End If
        Sleep 0
    Wend
    Set aSection = Nothing
    
    Close #nFile
End Sub

Public Sub ClearFile()
    On Error Resume Next
    Dim nFile&
    nFile = FreeFile
    Open m_sFileName For Output Access Write Lock Read Write As #nFile
    
    ' save version
    Print #nFile, "[ETS Settings Storage]"
    Print #nFile, "Version=" & ETS_SS_VERSION
    
    Close #nFile
End Sub

Public Sub SaveData()
    On Error GoTo EH
    Dim aValue As clsSettingAtom, nFile&
    Dim aSection As clsSettingSectionAtom
    Dim nErrNumber&, sErrDesc$
    
    nFile = FreeFile
    Open m_sFileName For Output Access Write Lock Read Write As #nFile
    
    ' save version
    Print #nFile, "[ETS Settings Storage]"
    Print #nFile, "Version=" & ETS_SS_VERSION
    
    For Each aSection In m_Sections
        Print #nFile, aSection.Section
        
        For Each aValue In aSection.Values
            Print #nFile, aValue.Key & "=" & aValue.Value
        Next
    Next
    
    On Error Resume Next
    Close #nFile
    
    Exit Sub
EH:
    sErrDesc = Err.Description
    nErrNumber = Err.Number
    On Error Resume Next
    Close #nFile
    On Error GoTo 0
    Err.Raise nErrNumber, "Settings Storage", "Fail to save " & AppTitle() & " Settings Storage file '" & m_sFileName & "'. " & sErrDesc
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    m_Sections.Clear
End Sub

Public Function GetStringValue(ByVal Section As String, ByVal KeyName As String, Optional ByVal DefaultValue As String = "") As String
    On Error Resume Next
    
    GetStringValue = DefaultValue
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Function
    
    GetStringValue = Trim$(m_Sections("[" & Section & "]").Values(KeyName).Value)
End Function

Public Function GetLongValue(ByVal Section As String, ByVal KeyName As String, Optional ByVal DefaultValue As Long = 0&) As Long
    On Error Resume Next
    Dim sValue$
    
    GetLongValue = DefaultValue
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Function
    
    Err.Clear
    sValue = Trim$(m_Sections("[" & Section & "]").Values(KeyName).Value)
    If Err.Number = 0 Then
        GetLongValue = CLng(sValue)
    End If
End Function

Public Function GetDoubleValue(ByVal Section As String, ByVal KeyName As String, Optional ByVal DefaultValue As Double = 0#) As Double
    On Error Resume Next
    Dim sValue$
    
    GetDoubleValue = DefaultValue
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Function
    
    Err.Clear
    sValue = Trim$(m_Sections("[" & Section & "]").Values(KeyName).Value)
    If Err.Number = 0 Then
        GetDoubleValue = CDbl(sValue)
        If Err.Number <> 0 Then
            Err.Clear
            GetDoubleValue = val(sValue)
            If Err.Number <> 0 Then GetDoubleValue = DefaultValue
        End If
    End If
End Function

Public Sub SetStringValue(ByVal Section As String, ByVal KeyName As String, ByVal Value As String)
    On Error Resume Next
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Sub
    
    Dim sSection$, aValue As clsSettingAtom
    Dim aSection As clsSettingSectionAtom
    
    sSection = "[" & Section & "]"
    
    Set aSection = m_Sections(sSection)
    If aSection Is Nothing Then Set aSection = m_Sections.Add(sSection)
    
    Set aValue = aSection.Values(KeyName)
    If aValue Is Nothing Then
        aSection.Values.Add KeyName, Value
    Else
        aValue.Value = Value
        Set aValue = Nothing
    End If
    Set aSection = Nothing
End Sub

Public Sub SetLongValue(ByVal Section As String, ByVal KeyName As String, ByVal Value As Long)
    On Error Resume Next
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Sub
    
    SetStringValue Section, KeyName, Trim$(Str$(Value))
End Sub

Public Sub SetDoubleValue(ByVal Section As String, ByVal KeyName As String, ByVal Value As Double)
    On Error Resume Next
    If Len(m_sFileName) <= 0 Or Len(Section) <= 0 Or Len(KeyName) <= 0 Then Exit Sub
    
    SetStringValue Section, KeyName, Trim$(Str$(Value))
End Sub

