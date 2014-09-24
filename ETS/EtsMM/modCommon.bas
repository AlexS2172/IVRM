Attribute VB_Name = "modCommon"
Option Explicit

Public Enum FilterTypeEnum
    ' Sharky: remove these constants as obsolete
    'TYPE_INDEX = 1
    'TYPE_STOCK = 2
    'TYPE_OPTION = 3
    
    TYPE_UNDERLYING = 4
    TYPE_GROUP = 5
    TYPE_TRADER = 6
    
    ' Sharky: TYPE_TRADER_GROUP is a replacement TYPE_BOOK for compatibility with old saved layouts
    ' It do not used anywhere in app for now. Just ignore it in opening layouts
    TYPE_TRADER_GROUP = 7
    
    TYPE_STRATEGY = 8
    TYPE_ALL = 9
    TYPE_FILE = 10
    TYPE_CNT_PTY = 11
    TYPE_POSITION = 12
End Enum

Public Enum ManualPriceUpdateEnum
    MPU_DELTE = 1
    MPU_ADD = 2
    MPU_UPDATE = 3
End Enum


Public Const DEFAULT_TRADER_HEDGE_STATUS& = EtsMmStockHedgeLib.enMmShGroupStatusAll

Public Const NO_STRATEGY_ID& = -2
Public Const NO_STRATEGY_NAME$ = "<Unassigned>"
Public Const TNT_ACCOUNT_MAXLEN& = 4&
Public Const TNT_PASSWORD_MAXLEN& = 10&

Public Const INVISIBLE_COLUMN As Long = -1
Public Const STORAGE_VALUE_DELIMITER As String = ","

Public Const BAD_DOUBLE_VALUE As Double = -1E+308
Public Const BAD_LONG_VALUE As Long = -2147483647
Public Const DBL_EPSILON As Double = 2.22044604925031E-16

Public Const STRIKE_DECIMALS_COUNT As Long = 4&

Public Const USD_ID& = 0&
Public Const USD_KEY$ = "0"
Public Const USD_SYMBOL$ = "$"

Public Const USA_REGION As String = "USA"
Public Const CAN_REGION As String = "CAD"

'Public Const ROL_UND& = 0&
'Public Const ROL_TOP& = 1&
'Public Const ROL_AGG& = 2&
'Public Const ROL_POS& = 3&

Public Enum EventTypeEnum
    EVENT_INFO = EtsGeneralLib.enEvtInfo
    EVENT_WARNING = EtsGeneralLib.enEvtWarning
    EVENT_ERROR = EtsGeneralLib.enEvtError
End Enum

Public Type NodeSortItemType
    NumKey As Double
    StrKey As String
    Node As VSFlexNode
End Type

Public Const RM_MAX_POINTS As Long = 25&

Public Type SortItem
    Key As String
    Value As Variant
End Type

' helps for prevent grid cell editing immediate after handled dblclick, e.g. new trade dialog raise
Public Const DBL_CLICK_HANDLE_TIMEOUT& = 1& ' in seconds
Private m_dtDblClickHandleTime As Date

Public Const FUT_ROOT_PREFIX$ = "/"
Private m_sCurrentDirectory As String

Public Const ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_TRIAL& = 1&
Public Const ASP_SUBSCRIPTION_STATUS_SUBSCRIBED_WITH_PAYMENT& = 0&
Public Const ASP_SUBSCRIPTION_STATUS_EXPIRED& = -1
Public Const ASP_SUBSCRIPTION_STATUS_CANCELED_BY_USER& = -2
Public Const ASP_SUBSCRIPTION_STATUS_NOT_SUBSCRIBED& = -3
Public Const ASP_SUBSCRIPTION_STATUS_CANCELED_BY_ADMIN& = -4
Public Const ASP_SUBSCRIPTION_STATUS_SUSPENDED& = -5
Public Const ASP_SUBSCRIPTION_STATUS_CARD_INVALID& = -6&


Private Declare Function GetSystemMetrics Lib "user32" ( _
      ByVal nIndex As Long _
   ) As Long

Private Const SM_CXICON = 11
Private Const SM_CYICON = 12

Private Const SM_CXSMICON = 49
Private Const SM_CYSMICON = 50
   
Private Declare Function LoadImageAsString Lib "user32" Alias "LoadImageA" ( _
      ByVal hInst As Long, _
      ByVal lpsz As String, _
      ByVal uType As Long, _
      ByVal cxDesired As Long, _
      ByVal cyDesired As Long, _
      ByVal fuLoad As Long _
   ) As Long
   
Private Const LR_DEFAULTCOLOR = &H0
Private Const LR_MONOCHROME = &H1
Private Const LR_COLOR = &H2
Private Const LR_COPYRETURNORG = &H4
Private Const LR_COPYDELETEORG = &H8
Private Const LR_LOADFROMFILE = &H10
Private Const LR_LOADTRANSPARENT = &H20
Private Const LR_DEFAULTSIZE = &H40
Private Const LR_VGACOLOR = &H80
Private Const LR_LOADMAP3DCOLORS = &H1000
Private Const LR_CREATEDIBSECTION = &H2000
Private Const LR_COPYFROMRESOURCE = &H4000
Private Const LR_SHARED = &H8000&

Private Const IMAGE_ICON = 1

Private Declare Function SendMessageLong Lib "user32" Alias "SendMessageA" ( _
      ByVal hWnd As Long, ByVal wMsg As Long, _
      ByVal wParam As Long, ByVal lParam As Long _
   ) As Long

Private Const WM_SETICON = &H80

Private Const ICON_SMALL = 0
Private Const ICON_BIG = 1

Private Declare Function GetWindow Lib "user32" ( _
   ByVal hWnd As Long, ByVal wCmd As Long) As Long
Private Const GW_OWNER = 4


Public Sub Set_Icon( _
      ByVal hWnd As Long, _
      ByVal sIconResName As String, _
      ByVal hLibrary As Long, _
      Optional ByVal bSetAsAppIcon As Boolean = True _
   )
Dim lhWndTop As Long
Dim lhWnd As Long
Dim cx As Long
Dim cy As Long
Dim hIconLarge As Long
Dim hIconSmall As Long
      
   If (bSetAsAppIcon) Then
      ' Find VB's hidden parent window:
      lhWnd = hWnd
      lhWndTop = lhWnd
      Do While Not (lhWnd = 0)
         lhWnd = GetWindow(lhWnd, GW_OWNER)
         If Not (lhWnd = 0) Then
            lhWndTop = lhWnd
         End If
      Loop
   End If
   
   cx = GetSystemMetrics(SM_CXICON)
   cy = GetSystemMetrics(SM_CYICON)
   hIconLarge = LoadImageAsString( _
         hLibrary, sIconResName, _
         IMAGE_ICON, _
         cx, cy, _
         LR_SHARED)
   If (bSetAsAppIcon) Then
      SendMessageLong lhWndTop, WM_SETICON, ICON_BIG, hIconLarge
   End If
   SendMessageLong hWnd, WM_SETICON, ICON_BIG, hIconLarge
   
   cx = GetSystemMetrics(SM_CXSMICON)
   cy = GetSystemMetrics(SM_CYSMICON)
   hIconSmall = LoadImageAsString( _
         hLibrary, sIconResName, _
         IMAGE_ICON, _
         cx, cy, _
         LR_SHARED)
   If (bSetAsAppIcon) Then
      SendMessageLong lhWndTop, WM_SETICON, ICON_SMALL, hIconSmall
   End If
   SendMessageLong hWnd, WM_SETICON, ICON_SMALL, hIconSmall
End Sub


Public Sub LogEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    Dim sSource As String
    
    If enType = EVENT_WARNING Or enType = EVENT_ERROR Then
        If Not g_PerformanceLog Is Nothing Then
            If Not Err Is Nothing Then sSource = Err.Source
            g_PerformanceLog.LogMmInfo IIf(enType = EVENT_WARNING, enLogWarning, enLogFaults), sMessage, sSource
        End If
    End If
    
    ShowEventLog True, (enType <> EVENT_ERROR)
    g_frmLog.LogEvent enType, sMessage
    
End Sub

Public Sub ShowEventLog(ByVal bShow As Boolean, Optional ByVal bCheckFlag As Boolean = False, Optional ByVal bForceShow As Boolean = False)
    On Error Resume Next
    If bShow And (Not bCheckFlag Or bCheckFlag And g_Params.ShowEventLog) Then
        If Not bForceShow Then
            ShowWindow g_frmLog.hWnd, SW_SHOWNOACTIVATE
        Else
            g_frmLog.Show vbModeless ', frmMain
        End If
        If Err.Number = 0 Then g_Params.EventLogVisible = True
    ElseIf Not bShow Then 'And (Not bCheckFlag Or bCheckFlag And g_Params.ShowEventLog) Then
        g_frmLog.Hide
        g_Params.EventLogVisible = False
    End If
End Sub

Public Function MakeLicenseDesc() As String
    On Error Resume Next
    MakeLicenseDesc = " User: " & vbTab & IIf(Len(g_sLicenseUser) > 0, g_sLicenseUser, "N/A") & vbCrLf
    MakeLicenseDesc = MakeLicenseDesc & " Company: " & vbTab & IIf(Len(g_sLicenseCompany) > 0, g_sLicenseCompany, "N/A") & vbCrLf
    MakeLicenseDesc = MakeLicenseDesc & " Login: " & vbTab & CurrentNtUserName() & vbCrLf
    MakeLicenseDesc = MakeLicenseDesc & " Product ID: " & vbTab & Hex$(g_lcLicense.UID) & vbCrLf
    MakeLicenseDesc = MakeLicenseDesc & " Product Key: " & vbTab & IIf(Len(g_sLicenseKey) > 0, g_sLicenseKey, "N/A") & vbCrLf
    If g_lcLicense.Year > 0 And g_lcLicense.Month > 0 And g_lcLicense.Day > 0 Then
        MakeLicenseDesc = MakeLicenseDesc & " Key Expiration: " & vbTab & DateSerial(g_lcLicense.Year, g_lcLicense.Month, g_lcLicense.Day) & vbCrLf
    Else
        MakeLicenseDesc = MakeLicenseDesc & " Key Expiration: " & vbTab & "N/A" & vbCrLf
    End If
End Function

Public Function MakeAppComponentsDescription(Optional ByVal bAddAppVersion As Boolean = False, Optional ByVal bReloadDBVersion As Boolean = True) As String
    On Error Resume Next
    Dim cVI As clsVersionInfo, sVersion$, sFile$, sPath$, sGUID$
    
    MakeAppComponentsDescription = " Components :" & vbTab & "Version :" & vbTab & "Path :" & vbCrLf
   
    If bAddAppVersion Then
        MakeAppComponentsDescription = MakeAppComponentsDescription & " " & AppTitle() & vbTab & App.Major & "." & App.Minor & "." & App.Revision & vbCrLf
    End If
        
    If bReloadDBVersion Then
        LoadDBVersion
    End If
    
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & "Database" & _
                                    vbTab & IIf(g_Params.DBVersion <> "", g_Params.DBVersion, "N/A") & _
                                    vbTab & IIf(g_Params.DBPath <> "", g_Params.DBPath, "N/A") & vbCrLf

    Set cVI = New clsVersionInfo
    
'    sPath = App.Path
'    If Right$(sPath, 1) <> "\" And Right$(sPath, 1) <> "/" Then
'        sPath = sPath & "\"
'    End If
    
    sFile = "OptionCalc.dll"
    If cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf

    sFile = "edaTools.dll"
    If cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "MMW.dll"
    sGUID = "{D9614A17-5E4D-47D9-8EDB-8A6209CF7A1A}" ' MsgManager
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "MsgStruct.dll"
    sGUID = "{5B7B0165-1126-4C59-B85C-C0C6B4E8642F}" ' SymbolObject
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "ETSManager.exe"
    sGUID = "{C213BE7D-AFAE-4F1C-8E9D-A7D248FA987B}" ' Settings
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf

    sFile = "EtsGeneral.dll"
    sGUID = "{B1B9E070-7E0D-4311-9D15-4BA8640F57F8}" ' VolaControl
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmGeneral.dll"
    sGUID = "{2476A3F1-BF81-4398-8EFC-8D617109C2B4}" ' MmTradeInfoAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmQuotes.dll"
    sGUID = "{5E68B484-3559-4E9E-A71B-C51443B392C7}" ' MmQvQuoteAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmVolaAnalysis.dll"
    sGUID = "{00461023-02E2-49D4-87A6-6472ED80E12A}" ' MmVaUndAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmGreeks.dll"
    sGUID = "{5B8729DA-72F3-429A-98EB-4C7471DF1974}" ' MmGaPosAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmHedge.dll"
    sGUID = "{D106A8EB-08F3-4223-9FFA-1467BE851A75}" ' MmHsCalc
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmReports.dll"
    sGUID = "{506B7B8C-BB01-471C-B1DE-6B74BF92FBF9}" ' MmRpUndAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmStockHedge.dll"
    sGUID = "{89617A2A-92F9-466B-B721-B7CBAD8548E8}" ' MmShPosAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmIndexHedge.dll"
    sGUID = "{B46AE86B-8455-4F0A-A356-342A0DB74A19}" ' MmIhUndAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EtsMmRisks.dll"
    sGUID = "{E80B67A5-CC66-4346-80D7-BC13229D70E1}" ' MmRvGrpAtom
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "ISEProvider.exe"
    sGUID = "{846A4AA7-B0B8-4258-9A1C-DAC58AD9843E}" ' ISEMMManager
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "DHIVFeeder5.dll"
    sGUID = "{76AF32F9-5AF3-47D7-93E4-392249FB6320}" ' IVDataLoader
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "VolatilitySources.dll"
    sGUID = "{E313EC0E-5CC0-4658-882A-605F4374CFFE}" ' VolatilitySources
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
                                    
    sFile = "VADBLayout.dll"
    sGUID = "{A7166386-06A9-4D6A-8276-3A7CF0E0235E}" ' DBLayout
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    If cVI.SetObjectProgID(g_Params.VolatilitySourceProgID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
        MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    Else
        sGUID = ""
        Select Case g_Params.VolatilitySourceType
            Case 0
                sFile = "VME.exe"
                sGUID = "{079EBA0B-CAC8-4210-BD39-36F52BC5C01D}" ' VolatilitySource
            
            Case 1
                sFile = "EtsAvmAdapter.exe"
                sGUID = "{FEDDE460-9B8E-4BE1-B853-94B89AF81CAD}" ' VolatilitySource
        End Select
        
        If Len(sGUID) <> 0 Then
            If cVI.SetObjectGUID(sGUID) Then
                sFile = cVI.FileName
                sVersion = cVI.FixedFileVersion
                sPath = cVI.FilePath
            ElseIf cVI.SetFilePath(sFile) Then
                sVersion = cVI.FixedFileVersion
                sPath = cVI.FilePath
            Else
                sVersion = ""
                sPath = ""
            End If
            MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                            vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
        End If
    End If
    
    sFile = "PriceProviders.dll"
    sGUID = "{FA41723C-C387-4448-9245-FC2875C2EE3C}" ' PriceInfo
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    If cVI.SetObjectProgID(g_Params.PriceProviderProgID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
        MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    Else
        sGUID = ""
        Select Case g_Params.PriceProviderType
            Case 0
                sFile = "MyTrackProviders.exe"
                sGUID = "{266203B3-D1DC-4808-A5C7-265F7C312629}" ' MyTrackPriceInfo
    
            Case 1
                sFile = "HyperFeedProviders.exe"
                sGUID = "{15F78302-5620-4AAE-822D-99C15DCFBFB8}" ' HyperFeedPriceInfo
    
            Case 2
                sFile = "TrackDataProviders.exe"
                sGUID = "{6B0F8362-4C54-4602-99A1-246089E287C8}" ' TrackDataPriceInfo
    
            Case 3
                sFile = "ATFDataProvider.exe"
                sGUID = "{C1BB2BE2-9B75-43F2-A0C4-644CC2B05AEE}" ' ATFPriceInfo
    
        End Select
    
        If Len(sGUID) <> 0 Then
            If cVI.SetObjectProgID(sGUID) Then
                sFile = cVI.FileName
                sVersion = cVI.FixedFileVersion
                sPath = cVI.FilePath
            ElseIf cVI.SetFilePath(sFile) Then
                sVersion = cVI.FixedFileVersion
                sPath = cVI.FilePath
            Else
                sVersion = ""
                sPath = ""
            End If
            MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                            vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
        End If
    End If
    
    sFile = "TNTConnector.dll"
    sGUID = "{4FCA0EDC-37FE-4D17-B8C5-46E1E2E58B9B}" ' CMMHHTAdapter
    
    If cVI.SetObjectGUID(sGUID) Then
        sFile = cVI.FileName
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    sFile = "EgDebugUtil.dll"
    If cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
        sPath = cVI.FilePath
    Else
        sVersion = ""
        sPath = ""
    End If
    MakeAppComponentsDescription = MakeAppComponentsDescription & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") & _
                                    vbTab & IIf(sPath <> "", sPath, "N/A") & vbCrLf
    
    Set cVI = Nothing
End Function

Public Function GetLongKeyValueHKLM(strKeyName As String, strValueName As String, Optional ByVal lngDefault As Long = 0) As Long
    On Error Resume Next
    Dim LongValue As Long
    
    GetLongKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, LongValue, lngDefault
        
    GetLongKeyValueHKLM = LongValue
End Function

' get string value from HKEY_LOCAL_MACHINE
Public Function GetStrKeyValueHKLM(strKeyName As String, strValueName As String, Optional ByVal strDefault As String = "") As String
    On Error Resume Next
    Dim strValue$
    
    GetStrKeyValue HKEY_LOCAL_MACHINE, strKeyName, strValueName, strValue, strDefault
        
    GetStrKeyValueHKLM = strValue
End Function

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

Public Function ExtractFileName(ByVal sPath As String) As String
    On Error Resume Next
    ExtractFileName = ""
    
    If Len(sPath) > 0 Then
        ExtractFileName = Mid$(sPath, InStrRev(sPath, "\") + 1)
    End If
End Function

Public Function ExtractFilePath(ByVal sPath As String) As String
    On Error Resume Next
    ExtractFilePath = ""
    
    If Len(sPath) > 0 Then
        ExtractFilePath = Mid$(sPath, 1, InStrRev(sPath, "\"))
    End If
End Function

Public Function ExtractName(ByVal sPath As String) As String
    On Error Resume Next
    Dim sFileName
    
    ExtractName = ""
    sFileName = ExtractFileName(sPath)
    
    If Len(sFileName) > 0 Then
        ExtractName = Mid$(sFileName, 1, InStrRev(sFileName, ".") - 1)
    End If
End Function

Public Function ContractTypeName(ByVal TypeID As Long) As String
    On Error Resume Next
    If TypeID = enCtOption Then
        ContractTypeName = "Option"
    ElseIf TypeID = enCtStock Then
        ContractTypeName = "Stock"
    Else
        ContractTypeName = "Index"
    End If
End Function

Public Function BuySellName(ByVal bIsBuy As Boolean) As String
    On Error Resume Next
    BuySellName = IIf(bIsBuy, "Buy", "Sell")
End Function

Private Sub TileWindows(aForms As clsFormColl, ByVal nCols As Long)
    On Error Resume Next
    If aForms Is Nothing Then Exit Sub
    
    Dim nCount&, nRowCount&, nColCount&
    nCount = aForms.Count
    If nCount = 0 Then Exit Sub

    Dim rcScreen As RECT
'    mi.cbSize = Len(MONITORINFO)

'    If IsMultiScreenSupported Then
'        Dim mi As MONITORINFO, hMon&
'        hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );
'
'        CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );
'        rcScreen = mi.rcWork;
'    Else
        SystemParametersInfo SPI_GETWORKAREA, 0, rcScreen, 0
'    End If
    
    Dim nColWidth&, nQuot&, nRem&, nCol&, nRows&, i&, hwndLast&
    Dim nHeight&, nCurWndIdx&, hWndCur&, wpls(0) As WINDOWPLACEMENT
    
    nColWidth = (rcScreen.Right - rcScreen.Left) / nCols
    nQuot = nCount \ nCols
    nRem = nCount Mod nCols
    
    hwndLast = HWND_TOP
    nCurWndIdx = 1

    nColCount = nCols - 1
    For nCol = 0 To nColCount
        nRows = nQuot
        If nRem - (nCols - 1 - nCol) > 0 Then nRows = nRows + 1
        
        nHeight = (rcScreen.Bottom - rcScreen.Top) / nRows
        nRowCount = nRows - 1
        For i = 0 To nRowCount
            Debug.Assert nCurWndIdx <= nCount
            hWndCur = aForms(nCurWndIdx).Frm.hWnd
            Debug.Assert hWndCur <> 0
            
            Erase wpls
            wpls(0).Length = Len(wpls(0))
            GetWindowPlacement hWndCur, wpls(0)
            
            wpls(0).showCmd = SW_SHOWNOACTIVATE
            
'            if( theApp.IsMultyScreenSupported() )
'                wpls(0).rcNormalPosition = CRect( CPoint( mi.rcMonitor.left + iCol * nColWidth, mi.rcMonitor.top + i * nHeight ),
'                                               CSize( nColWidth, nHeight ) );
'            Else
            wpls(0).rcNormalPosition.Left = nCol * nColWidth
            wpls(0).rcNormalPosition.Top = i * nHeight
            wpls(0).rcNormalPosition.Right = wpls(0).rcNormalPosition.Left + nColWidth
            wpls(0).rcNormalPosition.Bottom = wpls(0).rcNormalPosition.Top + nHeight
            
            SetWindowPlacement hWndCur, wpls(0)

            nCurWndIdx = nCurWndIdx + 1
        Next
    Next
End Sub

Public Sub TileWindowsHorizontally(aForms As clsFormColl)
    On Error Resume Next
    If aForms Is Nothing Then Exit Sub
    Dim nCount&
    nCount = aForms.Count
    If nCount = 0 Then Exit Sub
    
    Dim nMaxRows&, nMaxCols&, bIncCols As Boolean, nCols&, nRemain&
    
    nMaxRows = 3
    nMaxCols = 1
    bIncCols = True
    
    Do While True
        nCols = nCount \ nMaxRows
        nRemain = nCount Mod nMaxRows
        If nRemain > 0 Then nCols = nCols + 1

        If nCols > nMaxCols Then
            If bIncCols Then
                nMaxCols = nMaxCols + 1
                bIncCols = False
            Else
                nMaxRows = nMaxRows + 1
                bIncCols = True
            End If
        Else
            Exit Do
        End If
    Loop

    TileWindows aForms, nMaxCols
End Sub

Public Sub TileWindowsVertically(aForms As clsFormColl)
    On Error Resume Next
    If aForms Is Nothing Then Exit Sub
    Dim nCount&
    nCount = aForms.Count
    If nCount = 0 Then Exit Sub
    
    Dim nMaxRows&, nMaxCols&, bIncCols As Boolean, nRows&, nRemain&
    
    nMaxRows = 1
    nMaxCols = 3
    bIncCols = False
    
    Do While True
        nRows = nCount \ nMaxCols
        nRemain = nCount Mod nMaxCols
        If nRemain > 0 Then nRows = nRows + 1

        If nRows > nMaxRows Then
            If bIncCols Then
                nMaxCols = nMaxCols + 1
                bIncCols = False
            Else
                nMaxRows = nMaxRows + 1
                bIncCols = True
            End If
        Else
            nMaxCols = nCount \ nMaxRows
            Exit Do
        End If
    Loop

    TileWindows aForms, nMaxCols
End Sub

Public Sub CascadeWindows(aForms As clsFormColl)
    On Error Resume Next
    If aForms Is Nothing Then Exit Sub
    Dim nCount&
    nCount = aForms.Count
    If nCount = 0 Then Exit Sub

    Dim nMargin&

    nMargin = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXSIZEFRAME) - 1

    
    Dim rcScreen As RECT, nScrWidth&, nScrHeight&
'    mi.cbSize = Len(MONITORINFO)

'    If IsMultiScreenSupported Then
'        Dim mi As MONITORINFO, hMon&
'        hMon = CMonitorFunctionsWrapper::MonitorFromWindow( m_hWnd );
'
'        CMonitorFunctionsWrapper::GetMonitorInfo( hMon, &mi );
'        rcScreen = mi.rcWork;
'    Else
        SystemParametersInfo SPI_GETWORKAREA, 0, rcScreen, 0
'    End If
    
    nScrWidth = rcScreen.Right - rcScreen.Left
    nScrHeight = rcScreen.Bottom - rcScreen.Top

    Dim NDelta&, a#, b#, C#, d#, X1#, X2#

    ' Solving quadratic equation - calculating the value that is necesssary to indent
    ' from each edge of the working area to get a half of area value

    NDelta = 0
    a = 1
    b = -(nScrWidth + nScrHeight)
    C = (nScrWidth * nScrHeight) / 2#

    d = b * b - 4 * a * C
    If d > 0 Then
        X1 = (-b + Sqr(d)) / 2 * a
        X2 = (-b - Sqr(d)) / 2 * a
        If X2 > 0 Or X1 > 0 Then
            NDelta = IIf(X2 > 0 And X2 < nScrWidth And X2 < nScrHeight, X2, IIf(X1 > 0, X1, 0))
            NDelta = (NDelta \ nMargin) * nMargin
        End If
    End If

    Dim nWidth&, nHeight&, hWndCur&, hWndAfter&, n&, i&, wpls(0) As WINDOWPLACEMENT, pt As POINTAPI, aForm As Form
    nWidth = nScrWidth - NDelta
    nHeight = nScrHeight - NDelta

    i = 0
    hWndAfter = frmMain.hWnd
    For n = 1 To nCount
        Set aForm = aForms(n).Frm
        hWndCur = aForm.hWnd
        Debug.Assert hWndCur <> 0
        aForm.EnableActiveRealTime = False
        
        Erase wpls
        wpls(0).Length = Len(wpls(0))
        GetWindowPlacement hWndCur, wpls(0)

        wpls(0).showCmd = SW_SHOWNOACTIVATE
        
'        if( theApp.IsMultyScreenSupported() )
'            pt = CPoint( mi.rcMonitor.left + i * nMargin, mi.rcMonitor.top + i * nMargin );
'        Else
        pt.X = i * nMargin
        pt.Y = i * nMargin
        
        If nScrWidth < pt.X + nWidth + nMargin Or nScrHeight < pt.Y + nHeight + nMargin Then
            pt.X = nScrWidth - nWidth
            pt.Y = nScrHeight - nHeight
            i = -1
        End If
            
        wpls(0).rcNormalPosition.Left = pt.X
        wpls(0).rcNormalPosition.Top = pt.Y
        wpls(0).rcNormalPosition.Right = wpls(0).rcNormalPosition.Left + nWidth
        wpls(0).rcNormalPosition.Bottom = wpls(0).rcNormalPosition.Top + nHeight

        SetWindowPlacement hWndCur, wpls(0)
        
        'SetWindowPos hWndCur, hWndAfter, pt.x, pt.y, nWidth, nHeight, SWP_NOACTIVATE
        
        If n < nCount Then
            aForm.SetFocus
            aForm.EnableActiveRealTime = True
        Else
            aForm.EnableActiveRealTime = True
            aForm.SetFocus
        End If
        
        i = i + 1
        hWndAfter = hWndCur
        Set aForm = Nothing
    Next
    
    'aForms(nCount).SetFocus
End Sub

Public Sub ShowFormInTaskBar(ByRef aFrm As Form, Optional ByVal bShow As Boolean = True)
    On Error Resume Next
    If aFrm Is Nothing Then Exit Sub

    Dim nExStyle&, nNewExStyle&
    nExStyle = GetWindowLong(aFrm.hWnd, GWL_EXSTYLE)
    If bShow Then
        nNewExStyle = nExStyle Or WS_EX_APPWINDOW
    Else
        nNewExStyle = nExStyle And Not WS_EX_APPWINDOW
    End If

    If nExStyle <> nNewExStyle Then
        SetWindowPos aFrm.hWnd, 0, 0, 0, 0, 0, SWP_HIDEWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
        SetWindowLong aFrm.hWnd, GWL_EXSTYLE, nNewExStyle
        SetWindowPos aFrm.hWnd, 0, 0, 0, 0, 0, SWP_SHOWWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
    End If
End Sub

' qsort array sorting
Public Sub SortArray(ByRef arrValues() As Long, ByVal nLBound As Long, ByVal nUBound As Long)
    Dim nTemp&, nBuffer&, nCurLow&, nCurHigh&, nCurMidPoint&
    
    nCurLow = nLBound ' Start current low and high at actual low/high
    nCurHigh = nUBound
    If nUBound <= nLBound Then Exit Sub      ' Error!
    nCurMidPoint = (nLBound + nUBound) \ 2 ' Find the approx midpoint of the array
    nTemp = arrValues(nCurMidPoint)

    Do While nCurLow <= nCurHigh

        Do While arrValues(nCurLow) < nTemp
            nCurLow = nCurLow + 1
            If nCurLow = nUBound Then Exit Do
        Loop
        
        Do While nTemp < arrValues(nCurHigh)
            nCurHigh = nCurHigh - 1
            If nCurHigh = nLBound Then Exit Do
        Loop

        If nCurLow <= nCurHigh Then ' if low is <= high then swap
            nBuffer = arrValues(nCurLow)
            arrValues(nCurLow) = arrValues(nCurHigh)
            arrValues(nCurHigh) = nBuffer
            nCurLow = nCurLow + 1 ' CurLow++
            nCurHigh = nCurHigh - 1 ' CurLow--
        End If
    Loop

    If nLBound < nCurHigh Then ' Recurse if necessary
        SortArray arrValues(), nLBound, nCurHigh
    End If

    If nCurLow < nUBound Then ' Recurse if necessary
        SortArray arrValues(), nCurLow, nUBound
    End If
End Sub

' qsort array sorting
Public Sub SortArrayEx(ByRef arrValues() As SortItem, ByVal nLBound As Long, ByVal nUBound As Long)
    Dim aTemp As SortItem, aBuffer As SortItem, nCurLow&, nCurHigh&, nCurMidPoint&

    nCurLow = nLBound ' Start current low and high at actual low/high
    nCurHigh = nUBound
    If nUBound <= nLBound Then Exit Sub      ' Error!
    nCurMidPoint = (nLBound + nUBound) \ 2 ' Find the approx midpoint of the array
    aTemp = arrValues(nCurMidPoint)

    Do While nCurLow <= nCurHigh

        Do While arrValues(nCurLow).Key < aTemp.Key
            nCurLow = nCurLow + 1
            If nCurLow = nUBound Then Exit Do
        Loop

        Do While aTemp.Key < arrValues(nCurHigh).Key
            nCurHigh = nCurHigh - 1
            If nCurHigh = nLBound Then Exit Do
        Loop

        If nCurLow <= nCurHigh Then ' if low is <= high then swap
            aBuffer = arrValues(nCurLow)
            arrValues(nCurLow) = arrValues(nCurHigh)
            arrValues(nCurHigh) = aBuffer
            nCurLow = nCurLow + 1 ' CurLow++
            nCurHigh = nCurHigh - 1 ' CurLow--
        End If
    Loop

    If nLBound < nCurHigh Then ' Recurse if necessary
        SortArrayEx arrValues(), nLBound, nCurHigh
    End If

    If nCurLow < nUBound Then ' Recurse if necessary
        SortArrayEx arrValues(), nCurLow, nUBound
    End If
End Sub

Public Sub SortEntCollByName(ByRef aEntColl As EtsGeneralLib.EtsMmEntityAtomColl)
    On Error Resume Next
    Dim arrItems() As SortItem, aEnt As EtsGeneralLib.EtsMmEntityAtom, i&, nCount&

    nCount = aEntColl.Count
    If nCount > 1 Then
        ReDim arrItems(1 To nCount)

        For i = 1 To nCount
            Set aEnt = aEntColl(i)
            arrItems(i).Key = aEnt.Name
            Set arrItems(i).Value = aEnt
            Set aEnt = Nothing
        Next

        SortArrayEx arrItems, 1, nCount

        For i = 1 To nCount
            Set aEnt = arrItems(i).Value
            aEntColl.Remove CStr(aEnt.ID)
            aEntColl.Add CStr(aEnt.ID), aEnt
            Set aEnt = Nothing
        Next
    End If
End Sub

' qsort grid node array sorting
Public Sub SortNodeArray(ByRef arrValues() As NodeSortItemType, ByVal nLBound As Long, ByVal nUBound As Long, ByVal bByStrKey As Boolean)
    Dim aTemp As NodeSortItemType, aBuffer As NodeSortItemType, nCurLow&, nCurHigh&, nCurMidPoint&
    
    nCurLow = nLBound
    nCurHigh = nUBound
    If nUBound <= nLBound Then Exit Sub
    nCurMidPoint = (nLBound + nUBound) \ 2
    aTemp = arrValues(nCurMidPoint)

    If bByStrKey Then
        Do While nCurLow <= nCurHigh
    
            Do While arrValues(nCurLow).StrKey < aTemp.StrKey
                nCurLow = nCurLow + 1
                If nCurLow = nUBound Then Exit Do
            Loop
            
            Do While aTemp.StrKey < arrValues(nCurHigh).StrKey
                nCurHigh = nCurHigh - 1
                If nCurHigh = nLBound Then Exit Do
            Loop
    
            If nCurLow <= nCurHigh Then
                aBuffer = arrValues(nCurLow)
                arrValues(nCurLow) = arrValues(nCurHigh)
                arrValues(nCurHigh) = aBuffer
                nCurLow = nCurLow + 1
                nCurHigh = nCurHigh - 1
            End If
        Loop
    Else
        Do While nCurLow <= nCurHigh
    
            Do While arrValues(nCurLow).NumKey < aTemp.NumKey
                nCurLow = nCurLow + 1
                If nCurLow = nUBound Then Exit Do
            Loop
            
            Do While aTemp.NumKey < arrValues(nCurHigh).NumKey
                nCurHigh = nCurHigh - 1
                If nCurHigh = nLBound Then Exit Do
            Loop
    
            If nCurLow <= nCurHigh Then
                aBuffer = arrValues(nCurLow)
                arrValues(nCurLow) = arrValues(nCurHigh)
                arrValues(nCurHigh) = aBuffer
                nCurLow = nCurLow + 1
                nCurHigh = nCurHigh - 1
            End If
        Loop
    End If

    If nLBound < nCurHigh Then
        SortNodeArray arrValues(), nLBound, nCurHigh, bByStrKey
    End If

    If nCurLow < nUBound Then
        SortNodeArray arrValues(), nCurLow, nUBound, bByStrKey
    End If
End Sub

Public Function IsBadLong(ByVal nValue&) As Boolean
    On Error Resume Next
    IsBadLong = (nValue <= BAD_LONG_VALUE)
End Function

Public Function IsBadDouble(ByVal dValue#) As Boolean
    On Error Resume Next
    dValue = dValue + 0
    If Err.Number = 0 Then
        IsBadDouble = Not (dValue > BAD_DOUBLE_VALUE)
    Else
        IsBadDouble = True
    End If
End Function

Public Function GetOrderStr(ByVal nNum As Long) As String
    On Error Resume Next
    Dim nRem&
    If nNum > 0 Then
        nRem = nNum - Int(nNum / 10) * 10
        
        If nNum < 11 Or nNum > 13 Then
            Select Case nRem
                Case 1
                    GetOrderStr = CStr(nNum) & "st"
                Case 2
                    GetOrderStr = CStr(nNum) & "nd"
                Case 3
                    GetOrderStr = CStr(nNum) & "rd"
                Case Else
                    GetOrderStr = CStr(nNum) & "th"
            End Select
        Else
            GetOrderStr = CStr(nNum) & "th"
        End If
    Else
        GetOrderStr = "Zero"
    End If
End Function

Public Sub GetDefWindowSize(ptSize As POINTAPI)
    On Error Resume Next
    Dim rcScreen As RECT, nScrWidth&, nScrHeight&, nMargin&
    Dim NDelta&, a#, b#, C#, d#, X1#, X2#

    nMargin = GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXSIZEFRAME) - 1

    SystemParametersInfo SPI_GETWORKAREA, 0, rcScreen, 0
    
    nScrWidth = rcScreen.Right - rcScreen.Left
    nScrHeight = rcScreen.Bottom - rcScreen.Top

    NDelta = 0
    a = 1
    b = -(nScrWidth + nScrHeight)
    C = (nScrWidth * nScrHeight) / 2#

    d = b * b - 4 * a * C
    If d > 0 Then
        X1 = (-b + Sqr(d)) / 2 * a
        X2 = (-b - Sqr(d)) / 2 * a
        If X2 > 0 Or X1 > 0 Then
            NDelta = IIf(X2 > 0 And X2 < nScrWidth And X2 < nScrHeight, X2, IIf(X1 > 0, X1, 0))
            NDelta = (NDelta \ nMargin) * nMargin
        End If
    End If

    ptSize.X = nScrWidth - NDelta
    ptSize.Y = nScrHeight - NDelta
End Sub

Public Function LoadDBVersion(Optional ByVal bCheckVersion As Boolean = False) As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    
    LoadDBVersion = False
 
    Set rs = gDBW.usp_DataInfo_Get(, "version")
    If Not rs Is Nothing Then
        If Not rs.EOF Then
            g_Params.DBVersion = ReadStr(rs!vcKeyValue)
            
            If bCheckVersion Then
                LoadDBVersion = CheckDBVersion
            Else
                LoadDBVersion = True
            End If
        End If
        Set rs = Nothing
    End If
    
    Set rs = Nothing
    
    On Error Resume Next
    g_Params.DBPath = gDBW.GetDbPath
    Exit Function
EH:
    Set rs = Nothing
    gCmn.ErrorMsgBox g_frmOwner, "Fail to get DB version."
End Function

Private Function CheckDBVersion() As Boolean
    On Error Resume Next
    Dim sVersion() As String
    Dim nLBound&: nLBound = -1
    Dim nUBound&: nUBound = -1
    Dim nMajor&, nMinor&, nBuild&
    
    CheckDBVersion = False

    If Not g_Params.DoNotCheckDbVersion And DB_VERSION_COMPATIBLE_MAJOR > 0 And DB_VERSION_COMPATIBLE_MINOR > 0 Then
        sVersion = Split(g_Params.DBVersion, ".")
        nLBound = LBound(sVersion)
        nUBound = UBound(sVersion)
        If nUBound >= 0 Then
            If nUBound = 2 Then
                nMajor = CLng(sVersion(0))
                nMinor = CLng(sVersion(1))
                nBuild = CLng(sVersion(2))
            End If
                
            If nMajor > 0 And nMinor > 0 _
                And (DB_VERSION_COMPATIBLE_BUILD = 0 Or DB_VERSION_COMPATIBLE_BUILD > 0 And nBuild > 0) Then
                
                If DB_VERSION_COMPATIBLE_MAJOR = nMajor _
                    And DB_VERSION_COMPATIBLE_MINOR = nMinor _
                    And (DB_VERSION_COMPATIBLE_BUILD = 0 _
                        Or DB_VERSION_COMPATIBLE_BUILD > 0 And DB_VERSION_COMPATIBLE_BUILD <= nBuild) Then
                    
                    CheckDBVersion = True
                Else
                    gCmn.MyMsgBox g_frmOwner, "Fail to check database version." & vbCrLf & _
                                            "Incompatible version '" & g_Params.DBVersion & "'. Expected '" & _
                                            Trim$(Str$(DB_VERSION_COMPATIBLE_MAJOR)) & "." & _
                                            Trim$(Str$(DB_VERSION_COMPATIBLE_MINOR)) & "." & _
                                            IIf(DB_VERSION_COMPATIBLE_BUILD > 0, _
                                                Trim$(Str$(DB_VERSION_COMPATIBLE_BUILD)), "x") & "' at least." & vbCrLf & _
                                            "Please check your database connection.", vbCritical
                End If
            Else
                gCmn.MyMsgBox g_frmOwner, "Fail to check database version." & vbCrLf & _
                                        "Invalid version number format '" & g_Params.DBVersion & "'." & vbCrLf & _
                                        "Please check your database connection.", vbCritical
            End If
        Else
            gCmn.MyMsgBox g_frmOwner, "Fail to check database version. Empty version number." & vbCrLf & _
                                    "Please check your database connection.", vbCritical
        End If
    Else
        CheckDBVersion = True
    End If
End Function

Public Function GetNewYorkTime() As Date
    On Error Resume Next
    Dim dtNY As Double
    Dim bOk As Boolean
    bOk = GetNYDateTimeAsDATE(dtNY)
    GetNewYorkTime = CDate(dtNY)
End Function

Public Function GetCalcParams(ByVal dtNow As Date, _
                                ByVal dtExpiry As Date, _
                                ByVal dtCloseTime As Date, _
                                ByVal bUseTime As Boolean, _
                                ByRef dtDateCalc As Date, _
                                ByRef dtExpiryCalc As Date, _
                                ByRef dtCloseTimeCalc As Date, _
                                ByRef dYte As Double) As Boolean
On Error Resume Next
    Dim dDateCalc As Double
    Dim dExpiryCalc As Double
    Dim dCloseTimeCalc As Double
    
    GetCalcParams = GetCalculationParams(CDbl(dtNow), CDbl(dtExpiry), CDbl(dtCloseTime), _
                                            bUseTime, dDateCalc, dExpiryCalc, dCloseTimeCalc, dYte)
    dtDateCalc = CDate(dDateCalc)
    dtExpiryCalc = CDate(dExpiryCalc)
    dtCloseTimeCalc = CDate(dCloseTimeCalc)
    
End Function

Public Function GmtToLocal(ByVal dtGmtTime As Date) As Date
    On Error Resume Next
    GmtToLocal = DateAdd("n", -g_Params.TimeZoneBias, dtGmtTime)
End Function

Public Function LocalToGmt(ByVal dtLocalTime As Date) As Date
    On Error Resume Next
    LocalToGmt = DateAdd("n", g_Params.TimeZoneBias, dtLocalTime)
End Function

Public Function GetAlternateColor(ByVal clrColor As OLE_COLOR) As OLE_COLOR
    On Error Resume Next
    Dim btR As Byte, btG As Byte, btB As Byte, dBrightness#
    
    If clrColor < 0& Then clrColor = GetSysColor(clrColor Xor &H80000000)
    
    btR = (clrColor And &HFF)
    btG = (clrColor And &HFF00&) \ &H100&
    btB = (clrColor And &HFF0000) \ &H10000
    
    dBrightness = btR * 0.3 + btG * 0.6 + btB * 0.1
    
    If dBrightness > 75 Then
        GetAlternateColor = RGB(IIf(btR - 30 > 0, btR - 30, 0), IIf(btG - 30 > 0, btG - 30, 0), IIf(btB - 30 > 0, btB - 30, 0))
        
    Else
        dBrightness = 85 - dBrightness
        GetAlternateColor = RGB(btR + dBrightness, btG + dBrightness, btB + dBrightness)
    End If
End Function

Public Sub IncProgress(ByRef pb As MSComctlLib.ProgressBar)
    On Error Resume Next
    If (pb.Max - pb.Value) > 1 Then
        pb.Value = pb.Value + 1
    End If
End Sub

Public Sub IncProgressEx(ByRef pb As MSComctlLib.ProgressBar, ByVal nCount As Long)
    On Error Resume Next
    pb.Value = pb.Value + nCount
End Sub

Public Function GetLongPathNameEx(ByVal strShortPathName As String) As String
    On Error Resume Next
    Dim sBuffer$, nLen&, strLongPathName$
    
    GetLongPathNameEx = ""
    
    sBuffer = String$(2048, 0)
    Err.Clear
    nLen = GetLongPathName(strShortPathName, sBuffer, 2048)
    If Err.Number = 0 And nLen > 0 Then
        strLongPathName = Left$(sBuffer, nLen)
    Else
        Dim strTemp$, iSlashPos%
        
        'Add \ to short name to prevent Instr from failing
        strShortPathName = strShortPathName & "\"
        
        'Start from 4 to ignore the "[Drive Letter]:\" characters
        iSlashPos = InStr(4, strShortPathName, "\")
        
        'Pull out each string between \ character for conversion
        While iSlashPos
            strTemp = Dir(Left$(strShortPathName, iSlashPos - 1), vbNormal + vbHidden + vbSystem + vbDirectory)
            If strTemp = "" Then
                'Error 52 - Bad File Name or Number
                Exit Function
            End If
            strLongPathName = strLongPathName & "\" & strTemp
            iSlashPos = InStr(iSlashPos + 1, strShortPathName, "\")
        Wend
        
        'Prefix with the drive letter
        strLongPathName = Left$(strShortPathName, 2) & strLongPathName
    End If
    
    GetLongPathNameEx = strLongPathName
End Function

Public Function IsCtrlPressed() As Boolean
    On Error Resume Next
    IsCtrlPressed = ((GetKeyState(VK_CONTROL) And &H80000000) <> 0)
End Function

Public Function IsAltPressed() As Boolean
    On Error Resume Next
    IsAltPressed = ((GetKeyState(VK_MENU) And &H80000000) <> 0)
End Function

Public Function IsShiftPressed() As Boolean
    On Error Resume Next
    IsShiftPressed = ((GetKeyState(VK_SHIFT) And &H80000000) <> 0)
End Function

Public Sub FixDblClickHandled()
    On Error Resume Next
    m_dtDblClickHandleTime = Now
End Sub

Public Function IsDblClickHandled() As Boolean
    On Error Resume Next
    IsDblClickHandled = (DateDiff("s", m_dtDblClickHandleTime, Now) < DBL_CLICK_HANDLE_TIMEOUT)
End Function

' Is key in (Ctrl+G, Ctrl+A)
Public Function IsHotKeyGridAutosize(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridAutosize = (Shift = vbCtrlMask _
                                And (KeyCode = vbKeyG _
                                    Or KeyCode = vbKeyA))
End Function

' Is key in (Ctrl+C, Ctrl+Ins)
Public Function IsHotKeyGridClip(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClip = (Shift = vbCtrlMask _
                                And (KeyCode = vbKeyC _
                                    Or KeyCode = vbKeyInsert))
End Function

' Is key in (Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyOrder = (KeyCode = vbKeyInsert _
                                And (Shift = vbAltMask _
                                    Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ins, Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyTradeOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyTradeOrder = (KeyCode = vbKeyInsert _
                                            And (Shift = 0 _
                                                Or Shift = vbAltMask _
                                                Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins)
Public Function IsHotKeyGridCommon(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommon = (Shift = vbCtrlMask _
                                And (KeyCode = vbKeyG _
                                    Or KeyCode = vbKeyA _
                                    Or KeyCode = vbKeyC _
                                    Or KeyCode = vbKeyInsert))
End Function

' Is key in (Ctrl+C, Ctrl+Ins, Ins, Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyGridClipOrTradeOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrTradeOrder = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = 0 _
                                            Or Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ctrl+C, Ctrl+Ins, Ins)
Public Function IsHotKeyGridClipOrTrade(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrTrade = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And Shift = 0)
End Function

' Is key in (Ctrl+C, Ctrl+Ins, Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyGridClipOrOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrOrder = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins, Ins, Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyGridCommonOrTradeOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommonOrTradeOrder = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyG _
                                            Or KeyCode = vbKeyA _
                                            Or KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = 0 _
                                            Or Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins, Ins)
Public Function IsHotKeyGridCommonOrTrade(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommonOrTrade = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyG _
                                            Or KeyCode = vbKeyA _
                                            Or KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And Shift = 0)
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins, Alt+Ins, Ctrl+Alt+Ins)
Public Function IsHotKeyGridCommonOrOrder(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommonOrOrder = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyG _
                                            Or KeyCode = vbKeyA _
                                            Or KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask))
End Function

' Is key in (Ctrl+C, Ctrl+Ins, Shift+Alt+Ins)
Public Function IsHotKeyGridClipOrCard(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrCard = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And Shift = vbShiftMask + vbAltMask)
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins, Alt+Ins, Ctrl+Alt+Ins, Shift+Alt+Ins)
Public Function IsHotKeyGridCommonOrOrderCard(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommonOrOrderCard = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyG _
                                            Or KeyCode = vbKeyA _
                                            Or KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask _
                                            Or Shift = vbShiftMask + vbAltMask))
End Function

' Is key in (Ctrl+C, Ctrl+Ins, Alt+Ins, Ctrl+Alt+Ins, Shift+Alt+Ins)
Public Function IsHotKeyGridClipOrOrderCard(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrOrderCard = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask _
                                            Or Shift = vbShiftMask + vbAltMask))
End Function

' Is key in (Ctrl+C, Ins, Ctrl+Ins, Alt+Ins, Ctrl+Alt+Ins, Shift+Alt+Ins)
Public Function IsHotKeyGridClipOrTradeOrderCard(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridClipOrTradeOrderCard = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = 0 _
                                            Or Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask _
                                            Or Shift = vbShiftMask + vbAltMask))
End Function

' Is key in (Ctrl+G, Ctrl+A, Ctrl+C, Ctrl+Ins, Ins, Alt+Ins, Ctrl+Alt+Ins, Shift+Alt+Ins)
Public Function IsHotKeyGridCommonOrTradeOrderCard(KeyCode As Integer, Shift As Integer) As Boolean
    On Error Resume Next
    IsHotKeyGridCommonOrTradeOrderCard = (Shift = vbCtrlMask _
                                        And (KeyCode = vbKeyG _
                                            Or KeyCode = vbKeyA _
                                            Or KeyCode = vbKeyC _
                                            Or KeyCode = vbKeyInsert)) _
                                    Or (KeyCode = vbKeyInsert _
                                        And (Shift = 0 _
                                            Or Shift = vbAltMask _
                                            Or Shift = vbCtrlMask + vbAltMask _
                                            Or Shift = vbShiftMask + vbAltMask))
End Function

Public Function GenerateUID() As String
    On Error Resume Next
    Dim aGuid As Guid, S$, i&, sUID$

    CoCreateGuid aGuid

    S = Hex$(aGuid.Data1)
    sUID = String$(8 - Len(S), "0") & S
    S = Hex$(aGuid.Data2)
    sUID = sUID & String$(4 - Len(S), "0") & S
    S = Hex$(aGuid.Data3)
    sUID = sUID & String$(4 - Len(S), "0") & S

    For i = 0 To 7
        S = Hex$(aGuid.Data4(i))
        sUID = sUID & String$(2 - Len(S), "0") & S
    Next
    GenerateUID = sUID
End Function

Public Sub AdjustOptSeries(ByRef aOpt As EtsGeneralLib.EtsOptAtom, ByRef aOptRoot As EtsGeneralLib.EtsOptRootAtom)
    On Error Resume Next
    If Not aOpt Is Nothing And Not aOptRoot Is Nothing Then
        aOpt.Series = aOptRoot.Name & " " & UCase$(Format$(aOpt.Expiry, "MMMYY")) & " " & Int(aOpt.Strike) & " " & IIf(aOpt.OptType = enOtCall, "C", "P")
    Else
        aOpt.Series = ""
    End If
End Sub

Public Sub EditExSelectAllAndSetFocus(ByRef aText As ctlEditEx)
    On Error Resume Next
    aText.SelStart = 0
    aText.SelLength = Len(aText.Text)
    aText.SetFocus
End Sub

Public Function InStrCount&(ByRef sString$, ByRef sWhat$)
    On Error Resume Next
    Dim nMsgRows&, nStart&
    
    nMsgRows = -1
    nStart = 0
    Do
        nMsgRows = nMsgRows + 1
        nStart = InStr(nStart + 1, sString, sWhat)
    Loop While nStart > 0
    
    InStrCount = nMsgRows
End Function

Private Function LenStr(Str As String) As Long
    Dim nLen, nPos As Long
    On Error Resume Next
    nLen = Len(Str)
    nPos = InStr(1, Str, vbNullChar)
    If nLen > nPos Then nLen = nPos
    LenStr = nLen
End Function

Public Function GetOSType() As String
    Dim osvi As OSVERSIONINFOEX
    Dim result As Long
    Dim sVersion As String
    
    On Error Resume Next
    sVersion = ""
    osvi.dwOSVersionInfoSize = Len(osvi)
    result = GetVersionEx(osvi)
    If result = 0 Then
        GetOSType = sVersion
        Exit Function
    End If
    
    If osvi.dwPlatformId = VER_PLATFORM_WIN32_NT Then
        If osvi.dwMajorVersion = 4 Then
            sVersion = "Microsoft Windows NT 4.0"
            If osvi.wProductType = VER_NT_WORKSTATION Then
                sVersion = sVersion & " Workstation"
            ElseIf osvi.wProductType = VER_NT_SERVER Then
                sVersion = sVersion & " Server"
            End If
        ElseIf osvi.dwMajorVersion = 5 Then
            If osvi.dwMinorVersion = 0 Then
                sVersion = "Microsoft Windows 2000"
            ElseIf osvi.dwMinorVersion = 1 Then
                sVersion = "Microsoft Windows XP"
            ElseIf osvi.dwMinorVersion = 2 Then
                sVersion = "Microsoft Windows Server 2003"
            Else
                sVersion = "Microsoft Windows NT 5." & Str$(osvi.dwMinorVersion)
            End If
            ' Windows 2000 or XP
            If osvi.wProductType = VER_NT_WORKSTATION Then
                ' This is a workstation verion
                If (osvi.wSuiteMask And VER_SUITE_PERSONAL) = VER_SUITE_PERSONAL Then
                    sVersion = sVersion & " Personal"
                Else
                    sVersion = sVersion & " Professional"
                End If
            ElseIf osvi.wProductType = VER_NT_SERVER Then
                If osvi.dwMinorVersion = 0 Then
                    ' This is Win2k
                    If (osvi.wSuiteMask And VER_SUITE_DATACENTER) = VER_SUITE_DATACENTER Then
                        sVersion = sVersion & " Datacenter Server"
                    ElseIf (osvi.wSuiteMask And VER_SUITE_ENTERPRISE) = VER_SUITE_ENTERPRISE Then
                        sVersion = sVersion & " Advanced Server"
                    Else
                        sVersion = sVersion & " Server"
                    End If
                Else
                    ' This is 2003
                    If (osvi.wSuiteMask And VER_SUITE_DATACENTER) = VER_SUITE_DATACENTER Then
                        sVersion = sVersion & " Datacenter Edition"
                    ElseIf (osvi.wSuiteMask And VER_SUITE_ENTERPRISE) = VER_SUITE_ENTERPRISE Then
                        sVersion = sVersion & " Enterprice Edition"
                    Else
                        sVersion = sVersion & " Standard Edition"
                    End If
                End If
            End If
        End If
    End If
    
    sVersion = sVersion & " " & Left(osvi.szCSDVersion, LenStr(osvi.szCSDVersion)) & " (Build " & Str$(osvi.dwBuildNumber And &HFFFF) & ")"
    
    GetOSType = sVersion
End Function

Public Function GetSystemInfoString() As String

    Dim sysinfo As SYSTEM_INFO
    Dim msg As String
    On Error Resume Next
    
    GetSystemInfo sysinfo
    msg = msg & "Processor Numbers: " & Str$(sysinfo.dwNumberOfProcessors) & " Architecture:" & Str$(sysinfo.dwOemId.wProcessorArchitecture) & _
    " Level:" & Str$(sysinfo.wProcessorLevel) & " Revision:" & Str$(sysinfo.wProcessorRevision) & vbCrLf
    
    ' Get free memory.
    Dim memsts As MEMORYSTATUS
    Dim memory As Long
    GlobalMemoryStatus memsts
    memory = memsts.dwTotalPhys
    msg = msg & "Total Physical Memory: "
    msg = msg & Format$(memory / &H100000, "###,###") & " MB"
    memory = memsts.dwTotalPageFile
    msg = msg & ", Total Committed Memory: "
    msg = msg & Format$(memory / &H100000, "###,###") & " MB"
    memory = memsts.dwTotalVirtual
    msg = msg & ", Total Virtual Memory: "
    msg = msg & Format$(memory / &H100000, "###,###") & " MB"

    GetSystemInfoString = msg
End Function

Public Function MAPI_SendMail(sTo As String, sSubject As String, sMessage As String, sFilePath As String)
    On Error Resume Next
    g_Main.SendMail sTo, g_Params.MailAddress, sSubject, sMessage, g_Params.SMTPServer, sFilePath
End Function

Public Sub SendMailToSupport(ByVal sSubject$, ByVal sBody$, Optional ByVal sAttachFile$ = "")
    On Error Resume Next
    Dim aFile(0 To 0) As MapiFile, aMessage As MAPIMessage
    Dim nRes&, aRecipient(1 To 1) As MapiRecip
    
    'aMessage.Reserved = 0
    aMessage.Subject = sSubject
    aMessage.NoteText = sBody
    'aMessage.MessageType = ""
    'aMessage.DateReceived = ""
    'aMessage.ConversiondID = ""
    'aMessage.Flags = 0
    'aMessage.Originator = 0
    aMessage.RecipCount = 1
    aMessage.FileCount = 1
    
    'aRecipient(1).Reserved = 0
    aRecipient(1).RecipClass = MAPI_TO
    aRecipient(1).Name = "ETS Support"
    aRecipient(1).Address = "ETS_Support@egartech.com"
    'aRecipient(1).EIDSize = 0
    'aRecipient(1).EntryID = ""
    
    'aFile(0).Reserved = 0
    'aFile(0).Flags = 0
    aFile(0).Position = -1
    aFile(0).PathName = sAttachFile
    'aFile(0).FileName = ""
    'aFile(0).FileType = ""
    
    Err.Clear
    nRes = MAPISendMail(0, GetDesktopWindow(), aMessage, aRecipient, aFile, MAPI_DIALOG, 0)
    If Err.Number <> 0 Then
        gCmn.MyMsgBox g_frmOwner, "Fail to send a message to ETS support" & vbCrLf & _
                        sAttachFile & vbCrLf & Err.Description, vbCritical
    End If
End Sub

Public Function ReadDbl(ByVal v As Variant) As Double
    On Error Resume Next
    ReadDbl = 0
    If Not IsNull(v) Then
      ReadDbl = CDbl(v)
      If Err.Number <> 0 Then
          ReadDbl = val(v)
      End If
    End If
End Function

Public Function ReadLng(ByVal v As Variant) As Long
    On Error Resume Next
    ReadLng = 0&
    If Not IsNull(v) Then
        ReadLng = CLng(v)
        If Err.Number <> 0 Then
            ReadLng = CLng(val(v))
        End If
    End If
End Function

Public Function ReadCur(ByVal v As Variant) As Currency
    On Error Resume Next
    ReadCur = 0
    If Not IsNull(v) Then
        ReadCur = CCur(v)
        If Err.Number <> 0 Then
            ReadCur = CCur(val(v))
        End If
    End If
End Function

Public Function ReadStr(ByVal v As Variant) As String
    On Error Resume Next
    If Not IsNull(v) Then ReadStr = Trim$(CStr(v))
End Function

Public Function ReadByte(ByVal v As Variant) As Byte
    On Error Resume Next
    If Not IsNull(v) Then ReadByte = CByte(v)
End Function

Public Function ReadDate(ByVal v As Variant) As Date
    On Error Resume Next
    If Not IsNull(v) Then ReadDate = CDate(v)
    If ReadDate <= DateSerial(1900, 1, 1) Then ReadDate = 0#
End Function

Public Function IsValidContractType(enContractType As EtsGeneralLib.EtsContractTypeEnum) As Boolean
    On Error Resume Next
    IsValidContractType = (enContractType = enCtIndex Or enContractType = enCtStock _
                            Or enContractType = enCtOption Or enContractType = enCtFuture _
                            Or enContractType = enCtFutOption)
End Function

Public Function ContractTypeToPpInstrumentType(ByVal enContractType As EtsGeneralLib.EtsContractTypeEnum) As PRICEPROVIDERSLib.InstrumentTypeEnum
    On Error Resume Next
    Select Case enContractType
        Case enCtOption
            ContractTypeToPpInstrumentType = enOPT
            
        Case enCtStock
            ContractTypeToPpInstrumentType = enSTK
            
        Case enCtIndex
            ContractTypeToPpInstrumentType = enIDX
            
        Case enCtFuture
            ContractTypeToPpInstrumentType = enFUT
            
        Case enCtFutOption
            ContractTypeToPpInstrumentType = enFOP
            
        Case Else
            Debug.Assert False
            ContractTypeToPpInstrumentType = 0
    End Select
End Function

Public Function PpInstrumentTypeContractType(ByVal enPpInstrumentType As PRICEPROVIDERSLib.InstrumentTypeEnum) As EtsGeneralLib.EtsContractTypeEnum
    On Error Resume Next
    Select Case enPpInstrumentType
        Case enOPT
            PpInstrumentTypeContractType = enCtOption
            
        Case enSTK
            PpInstrumentTypeContractType = enCtStock
            
        Case enIDX
            PpInstrumentTypeContractType = enCtIndex
            
        Case enFUT
            PpInstrumentTypeContractType = enCtFuture
            
        Case enFOP
            PpInstrumentTypeContractType = enCtFutOption
            
        Case Else
            Debug.Assert False
            PpInstrumentTypeContractType = 0
    End Select
End Function

Public Function PpIsNonGroupReqType(ByVal enReqType As PRICEPROVIDERSLib.RequestsTypeEnum) As Boolean
    On Error Resume Next
    PpIsNonGroupReqType = (enReqType = enOPT _
                        Or enReqType = enSTK _
                        Or enReqType = enIDX _
                        Or enReqType = enFUT _
                        Or enReqType = enFOP)
End Function

Public Function PpIsGroupReqType(ByVal enReqType As PRICEPROVIDERSLib.RequestsTypeEnum) As Boolean
    On Error Resume Next
    PpIsGroupReqType = (enReqType = enGrSTK Or enReqType = enGrIDX)
End Function

Public Sub CreateAndSaveNewStrategy(ByRef aCombo As ComboBox, ByRef frmOwner As vB.Form)
    On Error Resume Next
    Dim nStrategyID&, frmEdit As frmNewStrategy
    Dim sName$, sDesc$, aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim aTrdUnd As EtsGeneralLib.EtsTraderUndAtom, nIndex&, i&
    
    nStrategyID = 0&
    
    Set frmEdit = New frmNewStrategy
    
    If frmEdit.Execute(nStrategyID, sName, sDesc, frmOwner) = vbOK Then
        Err.Clear
        If nStrategyID = 0 Then nStrategyID = gDBW.usp_MmStrategy_Save(0, sName, sDesc)
        If Err.Number = 0 Then
            If nStrategyID > 0 Then
                Set aStrategy = g_Strategy(nStrategyID)
                If aStrategy Is Nothing Then
                    Set aStrategy = New EtsGeneralLib.EtsStrategyAtom
                    aStrategy.ID = nStrategyID
                    aStrategy.Name = sName
                    aStrategy.Desc = sDesc
                    
                    g_Strategy.Add aStrategy.ID, aStrategy.Name, aStrategy
                    g_Params.FireNewStrategyAdded nStrategyID
                End If
            
                nIndex = -1
                For i = 0 To aCombo.ListCount - 1
                    If aCombo.ItemData(i) = aStrategy.ID Then
                        nIndex = i
                        Exit For
                    End If
                Next
                
                If nIndex = -1 Then
                    aCombo.AddItem aStrategy.Name
                    aCombo.ItemData(aCombo.NewIndex) = aStrategy.ID
                    nIndex = aCombo.NewIndex
                End If
                
                aCombo.ListIndex = nIndex
                Set aStrategy = Nothing
            Else
                gCmn.MyMsgBox frmOwner, "Fail to save new strategy! Unexpected error.", vbCritical + vbOKOnly
            End If
        Else
            gCmn.ErrorMsgBox frmOwner, "Fail to save new strategy!"
        End If
        
        Set frmEdit = Nothing
    End If
End Sub

Public Function BrowseForFolder(frmOwner As Form, sTitle As String, sPath As String) As Boolean
    On Error GoTo EH
    Dim lpIDList As Long
    Dim szTitle As String
    Dim sBuffer As String
    Dim bi As BrowseInfo
    
    m_sCurrentDirectory = sPath & vbNullChar & vbNullChar
    
    sPath = ""
    BrowseForFolder = False
    
    szTitle = sTitle
    bi.hWndOwner = frmOwner.hWnd
    bi.lpszTitle = lstrcat(szTitle, "")
    bi.ulFlags = BIF_RETURNONLYFSDIRS + BIF_DONTGOBELOWDOMAIN + BIF_STATUSTEXT + BIF_NEWDIALOGSTYLE
    bi.lpfnCallback = GetAddressofFunction(AddressOf BrowseCallbackProc)
    
    lpIDList = SHBrowseForFolder(bi)
    If lpIDList Then
        sBuffer = Space$(MAX_PATH)
        SHGetPathFromIDList lpIDList, sBuffer
        sBuffer = Left$(sBuffer, InStr(sBuffer, vbNullChar) - 1)
        sPath = sBuffer
    End If
    BrowseForFolder = True
    Exit Function
EH:
    gCmn.ErrorMsgBox frmOwner, "Fail to open folders list"
End Function
 
Private Function BrowseCallbackProc(ByVal hWnd As Long, ByVal uMsg As Long, ByVal lp As Long, ByVal pData As Long) As Long
    On Error Resume Next
    Dim sBuffer As String
       
    Select Case uMsg
        Case BFFM_INITIALIZED
            SendMessage hWnd, BFFM_SETSELECTION, 1, ByVal m_sCurrentDirectory
        
        Case BFFM_SELCHANGED
            sBuffer = String$(MAX_PATH, 0)
            If SHGetPathFromIDList(lp, sBuffer) = 1 Then SendMessage hWnd, BFFM_SETSTATUSTEXT, 0, ByVal sBuffer
        
    End Select
    
    BrowseCallbackProc = 0
End Function

Private Function GetAddressofFunction(Add As Long) As Long
    GetAddressofFunction = Add
End Function


Public Function MakeCommonFilePath$(ByVal sSubPath$, ByRef sFileName$, ByVal bPrivatePathCheckWriteAccess As Boolean)
    On Error Resume Next
    Dim sPath$, nFile#, bUseDefault As Boolean
    
    bUseDefault = True
    
    sSubPath = "\" & IIf(Len(sSubPath) > 0, sSubPath & "\", "")
    
    If Len(g_Params.CurrentUserPrivateFolder) > 0 Then
        
        CheckAndCreateFolder g_Params.CurrentUserPrivateFolder & sSubPath
        sPath = g_Params.CurrentUserPrivateFolder & sSubPath & sFileName
        
        If Len(sFileName) > 0 Then
            ' check current user file availability
            Err.Clear
            nFile = FreeFile
            If bPrivatePathCheckWriteAccess Then
                Open sPath For Output Access Write Lock Read Write As #nFile
            Else
                Open sPath For Input Access Read Lock Write As #nFile
            End If
            
            bUseDefault = (Err.Number <> 0)
            
            Close #nFile
        Else
            bUseDefault = False
        End If
    End If
    
    If bUseDefault Then
        CheckAndCreateFolder App.Path & sSubPath
        sPath = App.Path & sSubPath & sFileName
    End If
    
    MakeCommonFilePath = sPath
End Function

Public Function CheckAndCreateFolder(ByRef sFolderPath$) As Boolean
    On Error Resume Next
    Dim aFSO As Scripting.FileSystemObject
    
    CheckAndCreateFolder = False
    Set aFSO = New Scripting.FileSystemObject
    If Not aFSO Is Nothing Then
        If Not aFSO.FolderExists(sFolderPath) Then
            Err.Clear
            aFSO.CreateFolder sFolderPath
            CheckAndCreateFolder = (Err.Number = 0)
        Else
            CheckAndCreateFolder = True
        End If
        Set aFSO = Nothing
    End If
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

Public Function MainXMLFilePath$()
    On Error Resume Next
    MainXMLFilePath = GetStrKeyValueEx(PRODUCT_REG_KEY & "Asp" & "\Settings", "PathToMainXML", "")
End Function

Public Function CurrentUserXMLFilePath$()
    On Error Resume Next
    
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    Dim aXMLParams As ETSXMLParamsLib.XMLParams
    Set aXMLParams = aXMLLoader.Load(MainXMLFilePath)
    
    CurrentUserXMLFilePath = aXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp" & "\Users\" & CurrentNtUserName, "PathToXML", "")
End Function

Public Sub MMSleep(ByVal nDuration As Long)
    On Error Resume Next
    Dim nStart As Long
    nStart = GetTickCount
    
    While nDuration > (GetTickCount - nStart)
        DoEvents
        Sleep 0
    Wend
End Sub

Public Function ClipDays(ByVal dtDate As Date) As Date
    On Error Resume Next
    ClipDays = TimeSerial(Hour(dtDate), Minute(dtDate), 0)
End Function

Public Function ClipTime(ByVal dtDate As Date) As Date
    On Error Resume Next
    ClipTime = DateSerial(Year(dtDate), Month(dtDate), Day(dtDate))
End Function

Public Function ClipSeconds(ByVal dtDate As Date) As Date
    On Error Resume Next
    ClipSeconds = ClipTime(dtDate) + ClipDays(dtDate)
End Function

Public Function IsFileExist(filespec As String) As Boolean
On Error Resume Next
    Dim fso As Scripting.FileSystemObject
    Set fso = New Scripting.FileSystemObject
    
    If (Not fso Is Nothing) Then
        IsFileExist = fso.FileExists(filespec)
    End If
    
    Set fso = Nothing
End Function

Public Function MakeComboListTraders(ByRef Traders As EtsTraderColl) As String
On Error GoTo error_handler
'--------------------------'
    Dim sComboList As String
    Dim aTrader As EtsGeneralLib.EtsTraderAtom

    For Each aTrader In Traders
        sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
    Next
    sComboList = "#0;<All>|" & sComboList
    
    MakeComboListTraders = sComboList
    
Exit Function
error_handler:
    Debug.Assert True
End Function

Public Function MakeComboListStrategys(ByRef Strategys As EtsStrategyColl) As String
On Error GoTo error_handler
'--------------------------'
    Dim sComboList As String
    Dim aStrategy As EtsStrategyAtom
    
    For Each aStrategy In Strategys
        sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
    Next
    sComboList = "#0;<All>|" & sComboList
    
    MakeComboListStrategys = sComboList
    
Exit Function
error_handler:
    Debug.Assert True
End Function
