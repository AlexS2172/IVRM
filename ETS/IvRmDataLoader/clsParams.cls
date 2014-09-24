VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsParams"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Event DbConnectionChange()
Public Event PriceProviderChange()
Public Event SystemDbConnectionChange()
Public Event SystemVolatilitySourceChange()
Public Event SystemPriceProviderChange()

Public IsDebug As Boolean
Public DBVersion As String
Public DBPath As String

Private m_sDbConnection As String
Private m_sUserGroup As String
Private m_sSystemDbConnection As String
Private m_bChangingDbConnection As Boolean

Private m_nPriceProviderType As Long
Private m_bChangingPriceProvider As Boolean
Private m_nSystemPriceProviderType As Long
Private m_sPriceProviderProgID As String

Private m_nVolatilitySourceType As Long
Private m_sVolatilitySourceProgID As String
Private m_bChangingVolatilitySource As Boolean
Private m_nSystemVolatilitySourceType As Long
Private m_nTimeZoneBias As Long
Private m_bCommodity As Boolean

Public CurrentUserPrivateFolder$

Private m_sAppTitle As String
Private m_sAppLongTitle As String
Private m_sAppDescription As String
Private m_sAppCopyright As String
Private m_sAppIVRmTitle As String

Public DoNotCheckDbVersion As Boolean
Private m_hResource As Long

Public Enum WindowCode
        WND_MAIN
End Enum


Public Sub LoadSettings()
    On Error GoTo EH
    
    Dim sUserName$
    sUserName = CurrentNtUserName
    
    If Len(sUserName) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user name."
    End If
    
    g_sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", "")
    If Len(g_sUserGroup) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user " & AppIVRmTitle() & " group."
    End If
    m_sUserGroup = g_sUserGroup
    
    m_sDbConnection = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Database", g_sUserGroup, "")
    If Len(m_sDbConnection) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user database connection params."
    End If
    
    m_sSystemDbConnection = m_sDbConnection
    
    g_sFtpFolder = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "FTPFolder", "")
    g_sVolaFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "VolaFile", "")
    g_sIndexWeightFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "IndexWeightFile", "")
    g_sDividendsFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "DividendsFile", "")
    g_sInterestRatesFile = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "InterestRatesFile", "")
    g_sImportLogPath = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "ImportLogPath", "")
    g_sAppLogPath = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "ApplicationLogPath", "")
    g_sDataHistoryPath = g_aMainXMLParams.GetStringValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "DataHistoryPath", "")
    g_lStartTime = g_aMainXMLParams.GetLongValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "StartTime", -1)
    g_lStopTime = g_aMainXMLParams.GetLongValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "StopTime", -1)
    g_lFrequency = g_aMainXMLParams.GetLongValue(GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "Frequency", -1)

'    DoNotCheckDbVersion = (g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "DoNotCheckDbVersion", 0) <> 0)
    
'    Dim tzi(0 To 0) As TIME_ZONE_INFORMATION, nResult&
'    Erase tzi
'
'    nResult = GetTimeZoneInformation(tzi(0))
'    If nResult <> TIME_ZONE_ID_INVALID Then
'        m_nTimeZoneBias = tzi(0).Bias
'        If nResult = TIME_ZONE_ID_DAYLIGHT Then
'            m_nTimeZoneBias = m_nTimeZoneBias + tzi(0).DaylightDate.wMinute
'        End If
'    Else
'        m_nTimeZoneBias = 0
'    End If
    
    On Error Resume Next
    LoadCurrentUserPrivateFolderPath
    
    DBVersion = "N/A"
    DBPath = "N/A"
    
       
    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to load application settings."
End Sub

Public Sub SaveSettings()
    On Error Resume Next
End Sub

Private Function GetIsDebug() As Boolean
    On Error Resume Next
    Dim sFileName$, nCount&
    
    sFileName = String$(1024, 0)
    nCount = GetModuleFileName(App.hInstance, sFileName, 1024)
    sFileName = Left$(sFileName, nCount)
    
    GetIsDebug = (UCase$(Right$(sFileName, 7)) = "VB6.EXE")
End Function

Private Sub Class_Initialize()
    On Error Resume Next
    IsDebug = GetIsDebug()
    m_sDbConnection = ""
    m_sUserGroup = ""
    m_nPriceProviderType = -1
    m_bChangingDbConnection = False
    m_bChangingPriceProvider = False
    m_nTimeZoneBias = 0
    m_bCommodity = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
'    Set EtsSettings = Nothing
End Sub

'Private Sub EtsSettings_DatabaseConnectionChanged(ByVal bsConnectionString As String)
'    On Error Resume Next
'    If Not m_bChangingDbConnection Then
'        m_sSystemDbConnection = bsConnectionString
'        RaiseEvent SystemDbConnectionChange
'    End If
'End Sub
Public Property Get Commodity() As Boolean
    Commodity = m_bCommodity
End Property

Public Property Get DbConnection() As String
    On Error Resume Next
    DbConnection = m_sDbConnection
End Property

Public Property Let DbConnection(NewValue As String)
'    On Error GoTo EH
'    m_sDbConnection = NewValue
'    m_bChangingDbConnection = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.DatabaseConnectiLeft$bConnection
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'    m_bChangingDbConnection = False
'    Exit Property
'EH:
'    m_bChangingDbConnection = False
'    gCmn.ErrorHandler "Fail to save DB connection settings."
End Property
Public Property Get UserGroup() As String
    On Error Resume Next
    UserGroup = m_sUserGroup
End Property

Public Property Let UserGroup(NewValue As String)
    On Error Resume Next
    m_sUserGroup = NewValue
End Property


'Private Sub EtsSettings_PriceProviderChanged(ByVal lProviderType As Long)
'    On Error Resume Next
'    If Not m_bChangingPriceProvider Then
'        m_nSystemPriceProviderType = lProviderType
'        RaiseEvent SystemPriceProviderChange
'    End If
'End Sub

Public Property Get SystemPriceProviderType() As Long
    On Error Resume Next
    SystemPriceProviderType = m_nSystemPriceProviderType
End Property

Public Property Let SystemPriceProviderType(ByVal NewValue As Long)
'    On Error GoTo EH
'    m_bChangingPriceProvider = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.PriceProvider = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_nSystemPriceProviderType = NewValue
'
'    m_bChangingPriceProvider = False
'    Exit Property
'EH:
'    m_bChangingPriceProvider = False
'    gCmn.ErrorHandler "Fail to save price provider settings."
End Property

Public Property Get PriceProviderType() As Long
    On Error Resume Next
    PriceProviderType = m_nPriceProviderType
End Property

Public Property Let PriceProviderType(NewValue As Long)
'    On Error GoTo EH
'    m_nPriceProviderType = NewValue
'    m_bChangingPriceProvider = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.PriceProvider = m_nPriceProviderType
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'    m_bChangingPriceProvider = False
'    Exit Property
'EH:
'    m_bChangingPriceProvider = False
'    gCmn.ErrorHandler "Fail to save price provider settings."
End Property

Public Property Get PriceProviderProgID() As String
    On Error Resume Next
    PriceProviderProgID = m_sPriceProviderProgID
End Property

Public Property Let PriceProviderProgID(ByVal NewValue As String)
    On Error Resume Next
    m_sPriceProviderProgID = NewValue
End Property

Public Property Get VolatilitySourceType() As Long
    On Error Resume Next
    VolatilitySourceType = m_nVolatilitySourceType
End Property

Public Property Let VolatilitySourceType(NewValue As Long)
    On Error Resume Next
    m_nVolatilitySourceType = NewValue
End Property

Public Property Get VolatilitySourceProgID() As String
    On Error Resume Next
    VolatilitySourceProgID = m_sVolatilitySourceProgID
End Property

Public Property Let VolatilitySourceProgID(ByVal NewValue As String)
    On Error Resume Next
    m_sVolatilitySourceProgID = NewValue
End Property

'Private Sub EtsSettings_VolatilitySourceChanged(ByVal lVolatilitySourceType As Long)
'    On Error Resume Next
'    If Not m_bChangingVolatilitySource Then
'        m_nSystemVolatilitySourceType = lVolatilitySourceType
'        RaiseEvent SystemVolatilitySourceChange
'    End If
'End Sub

Public Property Get SystemVolatilitySourceType() As Long
    On Error Resume Next
    SystemVolatilitySourceType = m_nSystemVolatilitySourceType
End Property

Public Property Let SystemVolatilitySourceType(NewValue As Long)
'    On Error GoTo EH
'    m_bChangingVolatilitySource = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.VolatilitySource = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_nSystemVolatilitySourceType = NewValue
'
'    m_bChangingVolatilitySource = False
'    Exit Property
'EH:
'    m_bChangingVolatilitySource = False
'    gCmn.ErrorHandler "Fail to save volatility source settings."
End Property



Public Property Get SystemDbConnection() As String
    On Error Resume Next
    SystemDbConnection = m_sSystemDbConnection
End Property

Public Property Let SystemDbConnection(ByVal NewValue As String)
'    On Error GoTo EH
'    m_bChangingDbConnection = True
'    If Not EtsSettings Is Nothing Then
'        EtsSettings.DatabaseConnection = NewValue
'    Else
'        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
'    End If
'
'    m_sSystemDbConnection = NewValue
'
'    m_bChangingDbConnection = False
'    Exit Property
'EH:
'    m_bChangingDbConnection = False
'    gCmn.ErrorHandler "Fail to save DB connection settings."
End Property

Public Property Get TimeZoneBias() As Long
    On Error Resume Next
    TimeZoneBias = m_nTimeZoneBias
End Property

Private Sub LoadCurrentUserPrivateFolderPath()
    On Error Resume Next
    Dim sPath$, nLen&: nLen = -1
    Dim aFSO As Scripting.FileSystemObject
    
    Set aFSO = New Scripting.FileSystemObject
    If Not aFSO Is Nothing Then
        sPath = String$(2048, 0)
        
        If SHGetSpecialFolderPath(0, sPath, CSIDL_PERSONAL, 0) Then
            nLen = InStr(1, sPath, Chr$(0), vbBinaryCompare) - 1
            If nLen >= 0 Then
                sPath = Left$(sPath, nLen) & "\ETS"
                If Not aFSO.FolderExists(sPath) Then
                    Err.Clear
                    aFSO.CreateFolder sPath
                    If Err.Number = 0 Then CurrentUserPrivateFolder = sPath
                Else
                    CurrentUserPrivateFolder = sPath
                End If
            End If
        End If
        
        Set aFSO = Nothing
    End If
End Sub

Public Property Get AppIVRmTitle() As String
    AppIVRmTitle = m_sAppIVRmTitle
End Property

Public Property Get AppTitle() As String
    AppTitle = m_sAppTitle
End Property
Public Property Get AppLongTitle() As String
    AppLongTitle = m_sAppLongTitle
End Property
Public Property Get AppDescription() As String
    AppDescription = m_sAppDescription
End Property
Public Property Get AppCopyright() As String
    AppCopyright = m_sAppCopyright
End Property

Private Sub LoadResource(sLibrary As String)
   On Error Resume Next
   
    m_hResource = LoadLibrary(sLibrary)
    If m_hResource = 0 Then
        m_hResource = LoadLibrary("IVRmRes.dll")
    End If
End Sub

Private Sub FreeResource()
   On Error Resume Next
   If m_hResource <> 0 Then FreeLibrary m_hResource
End Sub


Public Property Get TopBanner() As Picture
    On Error Resume Next
    If m_hResource <> 0 Then
        Dim hBitmap  As Long
        hBitmap = LoadBitmap(m_hResource, "BOTitle")
        If hBitmap <> 0 Then
            Set TopBanner = LoadBitmapFromResource(hBitmap)
        End If
    End If
End Property

Public Property Get Splash() As Picture
    On Error Resume Next
    If m_hResource <> 0 Then
        Dim hBitmap  As Long
        hBitmap = LoadBitmap(m_hResource, "Splash_IVRm_BO")
        If hBitmap <> 0 Then
            Set Splash = LoadBitmapFromResource(hBitmap)
        End If
    End If
End Property

Public Sub SetIcon(enWindow As WindowCode, hwnd As Long, Optional ByVal bSetAsAppIcon As Boolean = True)
  On Error Resume Next
    Dim sResourceCode As String
    
    If m_hResource <> 0 Then _
        sResourceCode = GetWindowText(enWindow)
    
    If Len(sResourceCode) > 0 Then
        Set_Icon hwnd, sResourceCode, m_hResource, bSetAsAppIcon
    End If
End Sub

Public Property Get Icon(enWindow As WindowCode) As IPictureDisp
  On Error Resume Next
    Dim sResourceCode As String
    
    If m_hResource <> 0 Then _
        sResourceCode = GetWindowText(enWindow)
    
    If Len(sResourceCode) > 0 Then
        Set Icon = LoadIconFromResource(LoadIcon(m_hResource, sResourceCode))
    End If
End Property

Public Sub Terminate()
    FreeResource
End Sub

Private Function GetWindowText(enWindow As WindowCode) As String
    Dim sResourceCode$
    On Error Resume Next
    
        Select Case enWindow
            Case WND_MAIN:
                sResourceCode = "Main"
        End Select
GetWindowText = sResourceCode
End Function


