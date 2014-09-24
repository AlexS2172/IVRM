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
Option Explicit

Public Event DbConnectionChange()
Public Event PriceProviderChange()

Public IsDebug As Boolean
Public DBVersion As String

Private WithEvents EtsSettings As ETSMANAGERLib.Settings
Attribute EtsSettings.VB_VarHelpID = -1
Private m_sDbConnection As String
Private m_bChangingDbConnection As Boolean

Private m_nPriceProviderType As Long
Private m_bChangingPriceProvider As Boolean

Public Sub LoadSettings()
    On Error GoTo EH
    
    If EtsSettings Is Nothing Then Set EtsSettings = New ETSMANAGERLib.Settings
    m_sDbConnection = EtsSettings.DatabaseConnection
    m_nPriceProviderType = EtsSettings.PriceProvider
    
    On Error Resume Next
    
    DBVersion = "N/A"
    
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
    m_nPriceProviderType = -1
    m_bChangingDbConnection = False
    m_bChangingPriceProvider = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set EtsSettings = Nothing
End Sub

Private Sub EtsSettings_DatabaseConnectionChanged(ByVal bsConnectionString As String)
    On Error Resume Next
    If Not m_bChangingDbConnection Then
        m_sDbConnection = bsConnectionString
        RaiseEvent DbConnectionChange
    End If
End Sub

Public Property Get DbConnection() As String
    On Error Resume Next
    DbConnection = m_sDbConnection
End Property

Public Property Let DbConnection(NewValue As String)
    On Error GoTo EH
    m_sDbConnection = NewValue
    m_bChangingDbConnection = True
    If Not EtsSettings Is Nothing Then
        EtsSettings.DatabaseConnection = m_sDbConnection
    Else
        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
    End If
    m_bChangingDbConnection = False
    Exit Property
EH:
    m_bChangingDbConnection = False
    gCmn.ErrorHandler "Fail to save DB connection settings."
End Property

Private Sub EtsSettings_PriceProviderChanged(ByVal lProviderType As Long)
    On Error Resume Next
    If Not m_bChangingPriceProvider Then
        m_nPriceProviderType = lProviderType
        RaiseEvent PriceProviderChange
    End If
End Sub

Public Property Get PriceProviderType() As Long
    On Error Resume Next
    PriceProviderType = m_nPriceProviderType
End Property

Public Property Let PriceProviderType(NewValue As Long)
    On Error GoTo EH
    m_nPriceProviderType = NewValue
    m_bChangingPriceProvider = True
    If Not EtsSettings Is Nothing Then
        EtsSettings.PriceProvider = m_nPriceProviderType
    Else
        Err.Raise vbObjectError + 1024, "Application Settings", "Application settings object is not created."
    End If
    m_bChangingPriceProvider = False
    Exit Property
EH:
    m_bChangingPriceProvider = False
    gCmn.ErrorHandler "Fail to save price provider settings."
End Property




