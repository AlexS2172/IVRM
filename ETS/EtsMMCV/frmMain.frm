VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   Appearance      =   0  'Flat
   Caption         =   "ETS"
   ClientHeight    =   870
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   5790
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   870
   ScaleWidth      =   5790
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrTradeUpdate 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   2880
      Top             =   240
   End
   Begin VB.Timer tmrPerformance 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   4320
      Top             =   360
   End
   Begin VB.Timer tmrTntProcessorReconnectHelper 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   4680
      Top             =   360
   End
   Begin VB.Timer tmrTntProcessorHelper 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   4200
      Top             =   360
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   840
      Top             =   240
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Image imgSort 
      Height          =   135
      Index           =   0
      Left            =   1800
      Picture         =   "frmMain.frx":0E42
      Top             =   360
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image imgSort 
      Height          =   135
      Index           =   1
      Left            =   2160
      Picture         =   "frmMain.frx":0F38
      Top             =   360
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Image imgSort 
      Height          =   135
      Index           =   2
      Left            =   2520
      Picture         =   "frmMain.frx":102E
      Top             =   360
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save As..."
      End
      Begin VB.Menu mnuFileSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileTradeView 
         Caption         =   "&Trades..."
         Enabled         =   0   'False
         Shortcut        =   ^T
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileRiskView 
         Caption         =   "&Risks..."
         Shortcut        =   ^R
      End
      Begin VB.Menu mnuFileQuotesView 
         Caption         =   "&Quotes..."
         Enabled         =   0   'False
         Shortcut        =   ^Q
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileRiskMatrix 
         Caption         =   "Risk &Matrix..."
         Enabled         =   0   'False
         Shortcut        =   ^M
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVola 
         Caption         =   "Volat&ility Monitor..."
         Enabled         =   0   'False
         Shortcut        =   ^I
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileQuotationView 
         Caption         =   "ISE M&arket Maker..."
         Enabled         =   0   'False
         Shortcut        =   ^K
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVolaAnalysis 
         Caption         =   "Volatilit&y Analysis..."
         Enabled         =   0   'False
         Shortcut        =   ^Y
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileGreeksView 
         Caption         =   "Gr&eeks..."
         Shortcut        =   ^E
      End
      Begin VB.Menu mnuFileHedgeSummaryView 
         Caption         =   "&Hedge Summary..."
         Enabled         =   0   'False
         Shortcut        =   ^H
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileReportsView 
         Caption         =   "Re&ports..."
         Enabled         =   0   'False
         Shortcut        =   ^J
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileOrderView 
         Caption         =   "Or&ders..."
         Enabled         =   0   'False
         Shortcut        =   ^B
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileTntCards 
         Caption         =   "Trades Messa&ges..."
         Enabled         =   0   'False
         Shortcut        =   ^U
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileStockHedge 
         Caption         =   "Sto&ck Hedge..."
         Enabled         =   0   'False
         Shortcut        =   ^F
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileIndex1Hedge 
         Caption         =   "I&ndex Hedge..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileIndex2Hedge 
         Caption         =   "T&wo Indices Hedge..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileSeparator44 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileOTCOptionCalc 
         Caption         =   "OTC Option Calc..."
         Shortcut        =   ^{F12}
      End
      Begin VB.Menu mnuFileBatchReporting 
         Caption         =   "Batch Reporting ..."
      End
      Begin VB.Menu mnuFileBatchCapability 
         Caption         =   "Batch Capability ..."
      End
      Begin VB.Menu mnuFileSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsParameters 
         Caption         =   "&Parameters..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuOptionsConnections 
         Caption         =   "&Connections..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuOptionsSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsShowEventLog 
         Caption         =   "&Show Event Log Immediately"
      End
      Begin VB.Menu mnuOptionsEventLog 
         Caption         =   "&Event Log"
         Shortcut        =   ^L
      End
      Begin VB.Menu mnuOptionsSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuOptionsAlwaysOnTop 
         Caption         =   "&Always On Top"
         Visible         =   0   'False
      End
   End
   Begin VB.Menu mnuManage 
      Caption         =   "&Manage"
      Begin VB.Menu mnuManageUnderlyings 
         Caption         =   "&Underlyings..."
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuExerciseScreen 
         Caption         =   "Exercise Screen..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
   End
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuToolsVolaManager 
         Caption         =   "&Volatility Manager..."
      End
      Begin VB.Menu mnuToolsBackOffice 
         Caption         =   "ETS &Back Office..."
      End
      Begin VB.Menu mnuToolsEtsMmStd 
         Caption         =   "IvRm &Standard ..."
      End
      Begin VB.Menu mnuToolsHedger 
         Caption         =   "&Hedger..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuToolsStrategyScanner 
         Caption         =   "&Strategy Scanner..."
      End
      Begin VB.Menu mnuToolsOptionsScanner 
         Caption         =   "&Options Scanner..."
      End
   End
   Begin VB.Menu mnuWindow 
      Caption         =   "&Window"
      Begin VB.Menu mnuWindowArrange 
         Caption         =   "&Arrange"
      End
      Begin VB.Menu mnuWindowCloseAll 
         Caption         =   "&Close All"
      End
      Begin VB.Menu mnuWindowSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowTileHorizontaly 
         Caption         =   "Tile &Horizontaly"
      End
      Begin VB.Menu mnuWindowTileVertically 
         Caption         =   "Tile &Vertically"
      End
      Begin VB.Menu mnuWindowCascade 
         Caption         =   "Ca&scade"
      End
      Begin VB.Menu mnuWindowSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowShowInTaskBar 
         Caption         =   "&Show All In Taskbar"
      End
      Begin VB.Menu mnuWindowSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 1>"
         Index           =   1
         Shortcut        =   ^{F1}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 2>"
         Index           =   2
         Shortcut        =   ^{F2}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 3>"
         Index           =   3
         Shortcut        =   ^{F3}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 4>"
         Index           =   4
         Shortcut        =   ^{F4}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 5>"
         Index           =   5
         Shortcut        =   ^{F5}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 6>"
         Index           =   6
         Shortcut        =   ^{F6}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 7>"
         Index           =   7
         Shortcut        =   ^{F7}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 8>"
         Index           =   8
         Shortcut        =   ^{F8}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 9>"
         Index           =   9
         Shortcut        =   ^{F9}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowList 
         Caption         =   "&Windows..."
         Shortcut        =   ^W
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&?"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Contents..."
         Shortcut        =   {F1}
      End
      Begin VB.Menu mnuHelpEgarOnWeb 
         Caption         =   "Egar on &Web..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuHelpSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHelpAbout 
         Caption         =   "&About..."
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
 
Private WithEvents FormEvents As clsFormEvents
Attribute FormEvents.VB_VarHelpID = -1
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1
Private m_frmParams As frmParams
'Private WithEvents frmConnect As frmConnections
 
Private Enum ArrangeEnum
    AE_TILE_HORIZ
    AE_TILE_VERT
    AE_CASCADE
End Enum

Private m_sFilePath As String
Private m_sFileName As String
Private m_bIsNew As Boolean

Private m_nHeight As Long
Private m_nWidth As Long
Private m_nMaximizedPosXY As Long
Private m_enCurArrange As ArrangeEnum

Private Sub Form_Load()
    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Load Enter"
    
    If gCmn Is Nothing Then
'        If Not g_PerformanceLog Is Nothing Then _
'            g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Load Exit - Not Done"
        Exit Sub
    End If
    'gCmn.CustomizeForm Me
    
    m_sFilePath = MakeCommonFilePath("", "", False)
    m_sFileName = "Untitled.ets"
    m_bIsNew = True
    
    m_enCurArrange = AE_TILE_HORIZ
    
    AdjustFormSize
    
    If Not g_Params.IsDebug Then
        Set FormEvents = New clsFormEvents
        Set FormEvents.HookedForm = Me
    End If

    AdjustCaption
    mnuFileExit.Caption = "Exit" & vbTab & "Alt+F4"
    mnuFileQuotationView.Enabled = g_bIseEnable
    
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
        
    mnuOptionsShowEventLog.Checked = g_Params.ShowEventLog
    mnuOptionsAlwaysOnTop.Checked = g_Params.MainWinAlwaysOnTop
    
    mnuToolsOptionsScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    
    mnuWindowShowInTaskbar.Checked = g_Params.ShowWindowsInTaskbar

    SetWindowPos Me.hWnd, IIf(g_Params.MainWinAlwaysOnTop, HWND_TOPMOST, HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE Or SWP_NOACTIVATE
    
    Set m_frmParams = New frmParams
'    Set frmConnect = New frmConnections
 '   If Not g_PerformanceLog Is Nothing Then _
 '       g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Load Exit - Done"

End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    Dim sTraderName$
    
    If g_CurTraderID <> 0 Then
        sTraderName = " - " & g_Trader(g_CurTraderID).Name
    Else
        sTraderName = ""
    End If
    
    Me.Caption = APP_TITLE & " v" & App.Major & "." & App.Minor & "." & App.Revision & sTraderName & " - [" & m_sFileName & "]"
    App.Title = Me.Caption
End Sub

Private Sub AdjustFormSize()
    On Error Resume Next
    m_nMaximizedPosXY = -GetSystemMetrics(SM_CYSIZEFRAME)
    m_nHeight = GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYMENU)
    Me.Height = m_nHeight * 15 '  Me.ScaleY(m_nHeight, vbTwips)

    Dim mbi(0) As MENUBARINFO, hMenu&, i&, nCount&
    m_nWidth = 0
    hMenu = GetMenu(Me.hWnd)
    If hMenu <> 0 Then
        nCount = GetMenuItemCount(hMenu)
        If nCount <= 0 Then nCount = 5
        For i = 1 To nCount
            Erase mbi
            mbi(0).cbSize = Len(mbi(0))
            If GetMenuBarInfo(Me.hWnd, OBJID_MENU, i, mbi(0)) <> 0 Then
                m_nWidth = m_nWidth + mbi(0).rcBar.Right - mbi(0).rcBar.Left
            End If
        Next
    End If
    If m_nWidth = 0 Then m_nWidth = 250 Else m_nWidth = m_nWidth + 50
End Sub

Private Sub FormEvents_ActivateApp()
    EnableRealTime True
End Sub

Private Sub FormEvents_DeactivateApp()
    EnableRealTime False
End Sub

Public Sub EnableRealTime(bEnable As Boolean)
    Dim aFrm As Form, sTypeName$
    On Error Resume Next
    
    Set aFrm = Screen.ActiveForm
    If Not aFrm Is Nothing Then
        sTypeName = TypeName(aFrm)
        If sTypeName = "frmQuotesViewSingle" Or _
            sTypeName = "frmRiskView" Then
            aFrm.EnableRealTime bEnable
        End If
        Set aFrm = Nothing
    End If
End Sub

Private Sub FormEvents_GetMinMaxInfo(MaxSizeX As Long, MaxSizeY As Long, MaxPosX As Long, MaxPosY As Long, MinTrackSizeX As Long, MinTrackSizeY As Long, MaxTrackSizeX As Long, MaxTrackSizeY As Long)
    On Error Resume Next
    MaxPosX = m_nMaximizedPosXY
    MaxPosY = m_nMaximizedPosXY
    MaxSizeY = m_nHeight
    MinTrackSizeY = m_nHeight
    MaxTrackSizeY = m_nHeight
    MinTrackSizeX = m_nWidth
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Query Unload Enter"
    
    If Not FormEvents Is Nothing Then
        Set FormEvents.HookedForm = Nothing
        Set FormEvents = Nothing
    End If
    
    Unload frmWindowsList
    
    Screen.MousePointer = vbHourglass
    WindowsCloseAll
    Screen.MousePointer = vbDefault
    
    Hide
    DoEvents
    
 '   If Not g_PerformanceLog Is Nothing Then _
 '       g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Query Unload Exit - Done"
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Query Unload Unload - Enter"
    
    If gCmn Is Nothing Then
 '       If Not g_PerformanceLog Is Nothing Then _
 '           g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Query Unload Unload Exit - Not Done"
            
        Exit Sub
    End If
    
    g_lMainState = Me.WindowState
    If g_lMainState = vbNormal Then
        g_lMainTop = Me.Top
        g_lMainLeft = Me.Left
        g_lMainWidth = Me.Width
        g_lMainHeight = Me.Height
    End If
    
    Set aParams = Nothing
    Set m_frmParams = Nothing
'    Set frmConnect = Nothing
    
    ShutDown True
    
  '  If Not g_PerformanceLog Is Nothing Then _
  '      g_PerformanceLog.LogMmInfo enlogEnhDebug, "MainForm Query Unload Unload Exit - Done"
    
End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

'Public Sub ShowConnectionsOptions(Optional ByVal enShowOptions As ConnectionsShowOptionsEnum = enCsoAll)
'    On Error Resume Next
''    frmConnect.Execute False, enShowOptions, False, Me
'End Sub

'Private Sub frmConnect_OnOK(ByVal bDBChanged As Boolean, ByVal bPPChanged As Boolean, ByVal bIVChanged As Boolean, _
'                            ByVal bVSChanged As Boolean, ByVal bTNChanged As Boolean)
'    On Error Resume Next
'    Dim sMsg$
'
'    If bDBChanged Or bPPChanged Or bVSChanged Then
'        If bDBChanged Then sMsg = ", database"
'        If bPPChanged Then sMsg = ", price provider"
'        If bVSChanged Then sMsg = ", volatility source"
'
'        gCmn.MyMsgBox Me, "New " & Mid$(sMsg, 2) & " settings will be actual only after application restart.", vbExclamation + vbOKOnly
'    End If
'
'    If g_TntProcessor.UseTntMessaging _
'        And (Not g_TntProcessor.IsConnected Or g_TntProcessor.NeedToReconnect) Then
'
'        g_TntProcessor.ReConnect
'    End If
'End Sub

Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    ShowExerciseScreen
End Sub

Private Sub mnuFileBatchCapability_Click()
    On Error Resume Next
    g_frmProjectionsCV.ShowData
End Sub

Private Sub mnuFileBatchReporting_Click()
    On Error Resume Next
    g_frmProjectionsSTD.ShowData
End Sub

Private Sub mnuFileExit_Click()
    On Error Resume Next
    Unload Me
End Sub

Public Sub OpenFromFile(ByVal strPath$)
    On Error GoTo EH
    Dim aStorage As New clsSettingsStorage
    Dim nOperation&
        If Not g_PerformanceLog Is Nothing Then _
            nOperation = g_PerformanceLog.BeginLogMmOperation
    
    Screen.MousePointer = vbHourglass
    DoEvents
    aStorage.Init strPath
    aStorage.ReadData
    If aStorage.GetStringValue("Type", "Value") = "ETS Layout" Then
        ReadFromFile aStorage
        
        m_sFilePath = ExtractFilePath(strPath)
        m_sFileName = ExtractFileName(strPath)
        m_bIsNew = False
        
        ReloadWindowsList
        AdjustCaption
        
        Screen.MousePointer = vbDefault
    Else
        Screen.MousePointer = vbDefault
        gCmn.MyMsgBox Me, "Invalid ETS Layout file '" & strPath & "'.", vbCritical
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENLAYOUT, m_sFileName
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    If Len(dlgCommon.FileName) > 0 Then
        gCmn.ErrorMsgBox Me, "Fail to open ETS Layout file '" & strPath & "'."
    Else
        gCmn.ErrorMsgBox Me, "Fail to open ETS Layout file."
    End If
    On Error Resume Next
    ReloadWindowsList
    AdjustCaption
End Sub

Private Sub mnuFileOpen_Click()
    On Error Resume Next
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
        .CancelError = True
        .DefaultExt = ".ets"
        .DialogTitle = "Open ETS Layout"
        .FileName = ""
        .InitDir = m_sFilePath
        .Filter = "ETS Layout (*.ets)|*.ets|All Files|*.*"
        .FilterIndex = 1
        .ShowOpen
        If Err.Number = 0 Then OpenFromFile .FileName
    End With
End Sub

Private Sub mnuFileOtcOptionCalc_Click()
    On Error GoTo EH
    Dim sPath As String
    Dim sParams As String
    Dim lCalcModel As Long
    
    lCalcModel = CLng(g_Params.CalcModel)
    sPath = App.Path & "\OTCCalc\OTCOptionCalc.exe"
    sParams = "0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 " & CStr(lCalcModel)

    If ShellExecute(0&, "Open", sPath, sParams, "", SW_SHOWNORMAL) <= 32 Then
         gCmn.MyMsgBox Me, "Fail to open OTC OptionCalc at '" & sPath & "'.", vbCritical
    End If

    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open OTC Option Calc."

End Sub

Private Sub mnuFileSave_Click()
    On Error GoTo EH
    
    If m_bIsNew Then
        With dlgCommon
            .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                    Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
            .CancelError = True
            .DefaultExt = ".ets"
            .DialogTitle = "Save ETS Layout"
            .InitDir = ""
            .FileName = m_sFilePath & m_sFileName
            .Filter = "ETS Layout (*.ets)|*.ets|All Files|*.*"
            .FilterIndex = 1
            On Error Resume Next
            .ShowSave
            If Err.Number = 0 Then
                On Error GoTo EH
                Screen.MousePointer = vbHourglass
                SaveToFile ExtractFilePath(.FileName), ExtractFileName(.FileName)
                Screen.MousePointer = vbDefault
            End If
        End With
    Else
        SaveToFile m_sFilePath, m_sFileName
    End If
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save ETS layout."
End Sub

Private Sub mnuFileSaveAs_Click()
    On Error GoTo EH
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
        .CancelError = True
        .DefaultExt = ".ets"
        .DialogTitle = "Save ETS Layout As"
        .InitDir = ""
        .FileName = m_sFilePath & m_sFileName
        .Filter = "ETS Layout (*.ets)|*.ets|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowSave
        If Err.Number = 0 Then
            On Error GoTo EH
            Screen.MousePointer = vbHourglass
            SaveToFile ExtractFilePath(.FileName), ExtractFileName(.FileName)
            Screen.MousePointer = vbDefault
        End If
    End With
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save ETS layout."
End Sub

Private Sub SaveToFile(ByVal sFilePath As String, ByVal sFileName As String)
    On Error GoTo EH
    Dim aStorage As New clsSettingsStorage, sKey$, aFrm As clsFormAtom, nCount&, i&, sType$
    
    aStorage.Init sFilePath & sFileName
    aStorage.SetStringValue "Type", "Value", "ETS Layout"
    
    nCount = g_ViewFrm.Count
    If nCount > 0 Then
        aStorage.SetLongValue "Windows", "Count", nCount
        For i = 1 To nCount
            Set aFrm = g_ViewFrm(i)
            
            sType = WindowTypeString(aFrm.Frm)
            If Len(sType) > 0 Then
                sKey = CStr(i)

                aStorage.SetStringValue "Windows", "Win" & sKey, sType
                
                aStorage.SetStringValue "Windows", "WinState" & sKey, aFrm.Frm.WindowState
                If aFrm.Frm.WindowState = vbNormal Then
                    aStorage.SetLongValue "Windows", "WinTop" & sKey, aFrm.Frm.Top
                    aStorage.SetLongValue "Windows", "WinLeft" & sKey, aFrm.Frm.Left
                    aStorage.SetLongValue "Windows", "WinWidth" & sKey, aFrm.Frm.Width
                    aStorage.SetLongValue "Windows", "WinHeight" & sKey, aFrm.Frm.Height
                End If
                
                g_ViewFrm(i).Frm.SaveLayout aStorage, sKey
            End If
            
            Set aFrm = Nothing
        Next
    End If
    
    aStorage.SaveData
    
    m_sFilePath = sFilePath
    m_sFileName = sFileName
    m_bIsNew = False
    
    AdjustCaption
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save ETS layout."
    On Error Resume Next
    Set aFrm = Nothing
    aStorage.ClearFile
End Sub

Private Sub ReadFromFile(aStorage As clsSettingsStorage)
    On Error GoTo EH
    Dim sKey$, nCount&, i&, sType$
    Dim aFrm As Form, nOperation&
    
    WindowsCloseAll
    
    nCount = aStorage.GetLongValue("Windows", "Count", 0)
    
    If nCount > 0 Then
        For i = 1 To nCount
        
            If Not g_PerformanceLog Is Nothing Then _
                nOperation = g_PerformanceLog.BeginLogMmOperation
    
            sKey = CStr(i)
            sType = aStorage.GetStringValue("Windows", "Win" & sKey)
        
            Select Case sType
                Case "Quotes"
                    Set aFrm = New frmQuotesViewSingle
                    OpenView aFrm, aStorage, sKey, "Quotes"
                    
                Case "Risks"
                    Set aFrm = New frmRiskView
                    OpenView aFrm, aStorage, sKey, "Risks"
                    
                Case "Matrix"
                    Set aFrm = New frmRiskMatrix
                    OpenView aFrm, aStorage, sKey, "Risk Matrix"
                    
                Case "Trades"
                    Set aFrm = New frmTradeView
                    OpenView aFrm, aStorage, sKey, "Trades"
                    
                Case "Vola"
                    Set aFrm = New frmVolaMonitor
                    OpenView aFrm, aStorage, sKey, "Volatility Monitor"
                    
'                Case "Quotation"
'                    Set aFrm = New frmQuotationView
'                    OpenView aFrm, aStorage, sKey, "ISE Market Maker"

                Case "VolaAnalysis"
                    Set aFrm = New frmVolaAnalysis
                    OpenView aFrm, aStorage, sKey, "Volatility Analysis"
                    
                Case "Greeks"
                    Set aFrm = New frmGreeksView
                    OpenView aFrm, aStorage, sKey, "Greeks"
                    
                Case "HedgeSummary"
                    Set aFrm = New frmHedgeSummaryView
                    OpenView aFrm, aStorage, sKey, "Hedge Summary"
                    
                Case "Reports"
                    Set aFrm = New frmReportsView
                    OpenView aFrm, aStorage, sKey, "Reports"
                    
'                Case "Orders"
'                    Set aFrm = New frmOrderView
'                    OpenView aFrm, aStorage, sKey, "Orders"
'
'                Case "TntCards"
'                    Set aFrm = New frmTntCardView
'                    OpenView aFrm, aStorage, sKey, "Trades Messages"
'
'                Case "StockHedge"
'                    Set aFrm = New frmStockHedgeView
'                    OpenView aFrm, aStorage, sKey, "Stock Hedge"
'
'                Case "IndexHedge1"
'                    Set aFrm = New frmIndexHedge1View
'                    OpenView aFrm, aStorage, sKey, "Index Hedge"
'
'                Case "IndexHedge2"
'                    Set aFrm = New frmIndexHedge2View
'                    OpenView aFrm, aStorage, sKey, "Two Indices Hedge"
                    
            End Select

            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENWINDOW, aFrm.GetCaption
            
            Set aFrm = Nothing
        Next
    End If
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open ETS layout."
    
    On Error Resume Next
    Unload aFrm
    Set aFrm = Nothing
End Sub

Private Sub OpenView(aFrm As Form, aStorage As clsSettingsStorage, ByVal sKey As String, ByVal sWindowName As String)
    On Error GoTo EH
    Dim aForm As New clsFormAtom

    g_ViewFrmID = g_ViewFrmID + 1
    
    Set aForm.Frm = aFrm
    g_ViewFrm.Add aForm, CStr(g_ViewFrmID)
    Load aForm.Frm
    aForm.Frm.Init CStr(g_ViewFrmID)
    
    Dim nTop&, nLeft&, nWidth&, nHeight&, nState&
    nState = aStorage.GetLongValue("Windows", "WinState" & sKey, vbNormal)
    nTop = aStorage.GetLongValue("Windows", "WinTop" & sKey)
    nLeft = aStorage.GetLongValue("Windows", "WinLeft" & sKey)
    nWidth = aStorage.GetLongValue("Windows", "WinWidth" & sKey)
    nHeight = aStorage.GetLongValue("Windows", "WinHeight" & sKey)
    
    If nWidth > 0 And nHeight > 0 Then
        aForm.Frm.Left = nLeft
        aForm.Frm.Top = nTop
        aForm.Frm.Width = nWidth
        aForm.Frm.Height = nHeight
    Else
        aForm.Frm.Width = g_ptDefWindowSize.X
        aForm.Frm.Height = g_ptDefWindowSize.Y
    End If
    
    If Not g_Params.ShowWindowsInTaskbar Then
        aForm.Frm.ShowWindowInTaskbar False, False
    End If
    
    Dim nCmdShow&
    
    If nState = vbMinimized Then
        nCmdShow = SW_SHOWMINIMIZED
    ElseIf nState = vbMaximized Then
        nCmdShow = SW_SHOWMAXIMIZED
    Else
        nCmdShow = SW_SHOWNORMAL
    End If
    
    ShowWindow aForm.Frm.hWnd, nCmdShow
    aForm.Frm.OpenLayout aStorage, sKey
    'aForm.Frm.WindowState = nState
    'frm.Show
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open " & sWindowName & " window layout."
    On Error Resume Next
    Unload aFrm
    g_ViewFrm.Remove CStr(g_ViewFrmID)
End Sub

Private Function WindowTypeString(aFrm As Form) As String
    On Error Resume Next
    
    WindowTypeString = ""
    
    If TypeOf aFrm Is frmQuotesViewSingle Then
        WindowTypeString = "Quotes"
        
    ElseIf TypeOf aFrm Is frmRiskView Then
        WindowTypeString = "Risks"
    
    ElseIf TypeOf aFrm Is frmRiskMatrix Then
        WindowTypeString = "Matrix"
    
    ElseIf TypeOf aFrm Is frmTradeView Then
        WindowTypeString = "Trades"
    
    ElseIf TypeOf aFrm Is frmVolaMonitor Then
        WindowTypeString = "Vola"

'    ElseIf TypeOf aFrm Is frmQuotationView Then
'        WindowTypeString = "Quotation"
 
    ElseIf TypeOf aFrm Is frmVolaAnalysis Then
        WindowTypeString = "VolaAnalysis"

    ElseIf TypeOf aFrm Is frmGreeksView Then
        WindowTypeString = "Greeks"
    
    ElseIf TypeOf aFrm Is frmHedgeSummaryView Then
        WindowTypeString = "HedgeSummary"
    
    ElseIf TypeOf aFrm Is frmReportsView Then
        WindowTypeString = "Reports"
    
'    ElseIf TypeOf aFrm Is frmOrderView Then
'        WindowTypeString = "Orders"
'
'    ElseIf TypeOf aFrm Is frmTntCardView Then
'        WindowTypeString = "TntCards"
'
'    ElseIf TypeOf aFrm Is frmStockHedgeView Then
'        WindowTypeString = "StockHedge"
'
'    ElseIf TypeOf aFrm Is frmIndexHedge1View Then
'        WindowTypeString = "IndexHedge1"
'
'    ElseIf TypeOf aFrm Is frmIndexHedge2View Then
'        WindowTypeString = "IndexHedge2"
    
    End If
End Function

Private Sub mnuHelpAbout_Click()
    On Error Resume Next
    HelpAbout
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    HelpContents
End Sub

Private Sub mnuHelpEgarOnWeb_Click()
    On Error Resume Next
    HelpEgarOnWeb
End Sub

Public Sub HelpAbout()
    On Error Resume Next
    frmAbout.Execute Me
End Sub

Public Sub HelpContents()
    On Error Resume Next
    Dim sPath$, hHelpWnd&
    
    hHelpWnd = FindWindow(ByVal 0&, "ETS Market Maker Guide")
    If hHelpWnd = 0 Then
        sPath = App.Path & "\Documents\ETS Market Maker Guide.chm"
        If ShellExecute(0&, "Open", sPath, "", vbNullString, SW_SHOWNORMAL) <= 32 Then
            gCmn.MyMsgBox Me, "Fail to open help file '" & sPath & "'.", vbCritical
        End If
    Else
        If IsIconic(hHelpWnd) = 0 Then
            SetWindowPos hHelpWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE
        Else
            ShowWindow hHelpWnd, SW_RESTORE
        End If
    End If
End Sub

Public Sub HelpEgarOnWeb()
    On Error Resume Next
'    ShellExecute 0&, "Open", "http://www.egartech.com", "", vbNullString, 1
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    ShowEventLog (Not mnuOptionsEventLog.Checked), , True
End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    ShowParameters
End Sub

Public Sub ShowParameters()
    On Error Resume Next
    m_frmParams.Execute Me
End Sub

Private Sub mnuOptionsConnections_Click()
    On Error Resume Next
'    ShowConnectionsOptions
End Sub

Private Sub mnuOptionsShowEventLog_Click()
    On Error Resume Next
    g_Params.ShowEventLog = Not g_Params.ShowEventLog
    mnuOptionsShowEventLog.Checked = g_Params.ShowEventLog
End Sub

Public Sub RunEtsMmStd()
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\EtsMM.exe"
    If Shell(sPath, vbNormalFocus) = 0 Then
        gCmn.MyMsgBox Me, "Fail to run EtsMM(Standard) application. ('" & sPath & "')", vbCritical
    End If
End Sub

Public Sub RunBackOffice()
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\EtsBO.exe"
    If Shell(sPath, vbNormalFocus) = 0 Then
        gCmn.MyMsgBox Me, "Fail to run ETS Back Office application. ('" & sPath & "')", vbCritical
    End If
End Sub

Public Sub RunHedger(Optional ByVal sParam As String = "")
'    On Error Resume Next
'    Dim sPath$
'    sPath = App.Path & "\EgarHedgerSQL.exe"
'    If sParam <> "" Then sPath = sPath & " " & sParam
'    If Shell(sPath, vbNormalFocus) = 0 Then
'        gCmn.MyMsgBox Me, "Fail to run Hedger application. ('" & sPath & "')", vbCritical
'    End If
End Sub

Public Sub RunVolaManager(Optional ByVal sSymbol As String = "")
    On Error Resume Next
    Dim sPath$
    sPath = App.Path & "\VolaManager.exe"
    If Shell(sPath & IIf(Len(sSymbol) > 0, " " & sSymbol, ""), vbNormalFocus) = 0 Then
        gCmn.MyMsgBox Me, "Fail to run Volatility Manager application. ('" & sPath & "')", vbCritical
    End If
End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    RunBackOffice
End Sub

Private Sub mnuToolsEtsMmStd_Click()
    On Error Resume Next
    RunEtsMmStd
End Sub

Private Sub mnuToolsHedger_Click()
    On Error Resume Next
'    RunHedger
End Sub

Private Sub mnuToolsOptionsScanner_Click()
    On Error Resume Next
    RunOptionsScanner
End Sub

Private Sub mnuToolsStrategyScanner_Click()
    On Error Resume Next
    RunStrategyScanner
End Sub

Private Sub mnuToolsVolaManager_Click()
    On Error Resume Next
    RunVolaManager
End Sub

Private Sub mnuOptionsAlwaysOnTop_Click()
    On Error Resume Next
    g_Params.MainWinAlwaysOnTop = Not g_Params.MainWinAlwaysOnTop
    mnuOptionsAlwaysOnTop.Checked = g_Params.MainWinAlwaysOnTop
    
    SetWindowPos Me.hWnd, IIf(g_Params.MainWinAlwaysOnTop, HWND_TOPMOST, HWND_NOTOPMOST), 0, 0, 0, 0, SWP_NOMOVE Or SWP_NOSIZE Or SWP_NOACTIVATE
End Sub

Private Sub mnuFileQuotesView_Click()
    On Error Resume Next
    ShowQuotes
End Sub

Private Sub mnuFileRiskView_Click()
    On Error Resume Next
    ShowRisks
End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
    ShowRiskMatrix
End Sub

Private Sub mnuFileTradeView_Click()
    On Error Resume Next
    ShowTrades
End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    ShowVola
End Sub

Private Sub mnuFileQuotationView_Click()
    On Error Resume Next
'    ShowQuotation
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    ShowVolaAnalysis
End Sub

Private Sub mnuFileGreeksView_Click()
    On Error Resume Next
    ShowGreeks
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    ShowHedgeSummary
End Sub

Private Sub mnuFileOrderView_Click()
    On Error Resume Next
'    ShowOrders
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    ShowReports
End Sub

Private Sub mnuFileTntCards_Click()
    On Error Resume Next
'    ShowTntCards
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
'    ShowStockHedge
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
'    ShowIndex1Hedge
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
'    ShowIndex2Hedge
End Sub

Public Sub WindowsArrange()
    On Error Resume Next
    
    Select Case m_enCurArrange
        Case AE_TILE_HORIZ
            mnuWindowTileVertically_Click
            
        Case AE_TILE_VERT
            mnuWindowCascade_Click
            
        Case Else 'AE_CASCADE
            mnuWindowTileHorizontaly_Click
    End Select
End Sub

Public Sub WindowsTileHorizontally()
    On Error Resume Next
    m_enCurArrange = AE_TILE_HORIZ
    TileWindowsHorizontally g_ViewFrm
End Sub

Public Sub WindowsTileVertically()
    On Error Resume Next
    m_enCurArrange = AE_TILE_VERT
    TileWindowsVertically g_ViewFrm
End Sub

Public Sub WindowsCascade()
    On Error Resume Next
    m_enCurArrange = AE_CASCADE
    CascadeWindows g_ViewFrm
End Sub

Public Sub WindowsCloseAll()
    On Error Resume Next
    Dim i&, nCount&
    
    nCount = g_ViewFrm.Count
    For i = nCount To 1 Step -1
        Unload g_ViewFrm(i).Frm
    Next
    
    g_ViewFrm.Clear
End Sub

Public Sub WindowsGo(Index As Integer)
    On Error Resume Next
    If Not g_ViewFrm(Index) Is Nothing Then
        g_ViewFrm(Index).Frm.SetFocus
    End If
End Sub

Public Sub WindowsList()
    On Error Resume Next
    frmWindowsList.Execute Me
End Sub

Private Sub mnuWindowArrange_Click()
    On Error Resume Next
    WindowsArrange
End Sub

Private Sub mnuWindowCloseAll_Click()
    On Error Resume Next
    WindowsCloseAll
End Sub

Private Sub mnuWindowGo_Click(Index As Integer)
    On Error Resume Next
    WindowsGo Index
End Sub

Private Sub mnuWindowList_Click()
    On Error Resume Next
    WindowsList
End Sub

Public Sub WindowsShowInTaskBar(ByVal bShow As Boolean)
    On Error Resume Next
    Dim i&, nCount&
    
    g_Params.ShowWindowsInTaskbar = bShow
    mnuWindowShowInTaskbar.Checked = g_Params.ShowWindowsInTaskbar
    
    nCount = g_ViewFrm.Count
    For i = 1 To nCount
        g_ViewFrm(i).Frm.ShowWindowInTaskbar g_Params.ShowWindowsInTaskbar, True
    Next
        
End Sub

Private Sub mnuWindowShowInTaskBar_Click()
    On Error Resume Next
    WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar
End Sub

Private Sub mnuWindowTileHorizontaly_Click()
    On Error Resume Next
    WindowsTileHorizontally
End Sub

Private Sub mnuWindowTileVertically_Click()
    On Error Resume Next
    WindowsTileVertically
End Sub

Private Sub mnuWindowCascade_Click()
    On Error Resume Next
    WindowsCascade
End Sub

Public Sub ReloadWindowsList()
    On Error Resume Next
    Dim i&, nCount&
    
    nCount = g_ViewFrm.Count
    For i = 1 To 9
        If i <= nCount Then
            g_ViewFrm(i).Frm.SetWinIndex i
            mnuWindowGo(i).Caption = "&" & CStr(i) & " " & g_ViewFrm(i).Frm.GetCaption
            mnuWindowGo(i).Visible = True
            mnuWindowGo(i).Enabled = True
        Else
            mnuWindowGo(i).Visible = False
            mnuWindowGo(i).Enabled = False
        End If
    Next
    
    If nCount > 9 Then
        For i = 10 To nCount
            g_ViewFrm(i).Frm.SetWinIndex i
        Next
    End If
    
    For i = 1 To nCount
        g_ViewFrm(i).Frm.AdjustWindowsListCaptions
    Next
    frmWindowsList.ReloadWindowsList
End Sub

Public Sub AdjustWindowsListCaptions()
    On Error Resume Next
    Dim i&, nCount&
    
    nCount = g_ViewFrm.Count
    For i = 1 To 9
        If i <= nCount Then
            mnuWindowGo(i).Caption = "&" & CStr(i) & " " & g_ViewFrm(i).Frm.GetCaption
        End If
    Next
    
    For i = 1 To nCount
        g_ViewFrm(i).Frm.AdjustWindowsListCaptions
    Next
    
    frmWindowsList.ReloadWindowsList
End Sub

Public Sub ShowQuotes(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmQuotesViewSingle

    ShowView aFrm, nType, nValue, "Quotes"
End Sub

Public Sub ShowRisks(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmRiskView

    ShowView aFrm, nType, nValue, "Risks"
End Sub

Public Sub ShowRiskMatrix(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmRiskMatrix

    ShowView aFrm, nType, nValue, "Risk Matrix"
End Sub

Public Sub ShowTrades(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmTradeView

    ShowView aFrm, nType, nValue, "Trades"
End Sub

Public Sub ShowVola(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmVolaMonitor

    ShowView aFrm, nType, nValue, "Volatility Monitor"
End Sub

Public Sub ShowQuotation(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmQuotationView
'
'    ShowView aFrm, nType, nValue, "ISE Market Maker"
End Sub

Public Sub ShowVolaAnalysis(Optional ByVal nType As Long = TYPE_FILE, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmVolaAnalysis

    ShowView aFrm, nType, nValue, "Volatility Analysis"
End Sub

Public Sub ShowGreeks(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmGreeksView

    ShowView aFrm, nType, nValue, "Greeks"
End Sub

Public Sub ShowHedgeSummary(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmHedgeSummaryView

    ShowView aFrm, nType, nValue, "Hedge Summary"
End Sub

Public Sub ShowReports(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim aFrm As New frmReportsView

    ShowView aFrm, nType, nValue, "Reports"
End Sub

Public Sub ShowOrders(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmOrderView
'
'    ShowView aFrm, nType, nValue, "Orders"
End Sub

Public Sub ShowTntCards(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmTntCardView
'
'    ShowView aFrm, nType, nValue, "Trades Messages"
End Sub

Public Sub ShowStockHedge(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmStockHedgeView
'
'    ShowView aFrm, nType, nValue, "Stock Hedge"
End Sub

Public Sub ShowIndex1Hedge(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmIndexHedge1View
'
'    ShowView aFrm, nType, nValue, "Index Hedge"
End Sub

Public Sub ShowIndex2Hedge(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
'    Dim aFrm As New frmIndexHedge2View
'
'    ShowView aFrm, nType, nValue, "Two Indices Hedge"
End Sub

Private Sub ShowView(aFrm As Form, ByVal nType As Long, ByVal nValue As Long, ByVal sWindowName As String)
    On Error GoTo EH
    Dim aForm As New clsFormAtom, nOperation&
    
    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
    
    Screen.MousePointer = vbHourglass
    DoEvents
    g_ViewFrmID = g_ViewFrmID + 1
    
    Set aForm.Frm = aFrm
    g_ViewFrm.Add aForm, CStr(g_ViewFrmID)
    Load aForm.Frm
    aForm.Frm.Init CStr(g_ViewFrmID)
    aForm.Frm.ShowData nType, nValue
    If Not g_Params.ShowWindowsInTaskbar Then
        aForm.Frm.ShowWindowInTaskbar False, False
    End If
    
    If g_ptDefWindowSize.X > 0 And g_ptDefWindowSize.Y > 0 Then
        aForm.Frm.Width = g_ptDefWindowSize.X
        aForm.Frm.Height = g_ptDefWindowSize.Y
    End If
    
    aForm.Frm.Show
    ReloadWindowsList
    Screen.MousePointer = vbDefault
    
    If Not g_PerformanceLog Is Nothing Then
        If nType <> 0 Or nValue <> 0 Then
           g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENWINDOW, aFrm.GetCaption & "  Specific type, value", nType, nValue
        Else
           g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENWINDOW, aFrm.GetCaption
        End If
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open " & sWindowName & " window."
    
    On Error Resume Next
    Unload aFrm
    g_ViewFrm.Remove CStr(g_ViewFrmID)
    ReloadWindowsList
    On Error GoTo 0
    Screen.MousePointer = vbDefault

     If Not g_PerformanceLog Is Nothing Then
        If nType <> 0 Or nValue <> 0 Then
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENWINDOW, aFrm.GetCaption & "  Specific type, value", nType, nValue
        Else
            g_PerformanceLog.FinishLogMmOperation nOperation, OPER_OPENWINDOW, aFrm.GetCaption
        End If
    End If
    
End Sub

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    ShowManageUnderlyings
End Sub

Public Sub ShowManageUnderlyings()
    On Error Resume Next
    
    If g_frmUnderlyings Is Nothing Then
        On Error GoTo EH
        
        Screen.MousePointer = vbHourglass
        DoEvents
        g_ViewFrmID = g_ViewFrmID + 1
        
        Set g_frmUnderlyings = New clsFormAtom
        Set g_frmUnderlyings.Frm = New frmUnderlyingsView
        
        g_ViewFrm.Add g_frmUnderlyings, CStr(g_ViewFrmID)
        Load g_frmUnderlyings.Frm
        g_frmUnderlyings.Frm.Init CStr(g_ViewFrmID)
        g_frmUnderlyings.Frm.ShowData
        
        If Not g_Params.ShowWindowsInTaskbar Then
            g_frmUnderlyings.Frm.ShowWindowInTaskbar False, False
        End If
        
'        If g_ptDefWindowSize.X > 0 And g_ptDefWindowSize.Y > 0 Then
'            g_frmUnderlyings.Frm.Width = g_ptDefWindowSize.X
'            g_frmUnderlyings.Frm.Height = g_ptDefWindowSize.Y
'        End If
        
        g_frmUnderlyings.Frm.Show
        ReloadWindowsList
        Screen.MousePointer = vbDefault
        
    Else
        If IsIconic(g_frmUnderlyings.Frm.hWnd) = 0 Then
            ShowWindow g_frmUnderlyings.Frm.hWnd, SW_SHOW
        Else
            ShowWindow g_frmUnderlyings.Frm.hWnd, SW_RESTORE
        End If
        g_frmUnderlyings.Frm.SetFocus
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open underlyings window."
    
    On Error Resume Next
    g_frmUnderlyings.Frm.Term
    Set g_frmUnderlyings = Nothing
    On Error GoTo 0
    Screen.MousePointer = vbDefault
End Sub

Private Sub tmrPerformance_Timer()
'    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogPerformanceCounters
End Sub

Public Sub StopExtraRT()
    On Error Resume Next
    Dim i&, nCount&, aForm As clsFormAtom
    Dim lCurrentRT As Long
    Dim lMinRT As Long
    Dim lMinRTFormNum As Long

    
    Do While g_RTQuantity > g_MaxRTQuantity
        lMinRT = 1000000
        lMinRTFormNum = -1
        lCurrentRT = -1
        
        For i = 1 To g_ViewFrm.Count
            Set aForm = g_ViewFrm(i)
            If TypeOf aForm.Frm Is frmQuotesViewSingle Then
                If aForm.Frm.IsRealTime Then
                    lCurrentRT = aForm.Frm.lRTNum
                    If lCurrentRT < lMinRT Then
                        lMinRT = lCurrentRT
                        lMinRTFormNum = i
                    End If
                End If
            End If
            Set aForm = Nothing
        Next
        
        If lMinRTFormNum > 0 Then
            Set aForm = g_ViewFrm(lMinRTFormNum)
            If Not aForm Is Nothing Then
                aForm.Frm.StopRT
            End If
            Set aForm = Nothing
        End If
    Loop
    

End Sub

Public Sub RunStrategyScanner()
    On Error Resume Next
    Dim sInfo As STARTUPINFO
    Dim ComandString As String
    Dim sNull As String
    Dim hWnd As Long
        
    hWnd = FindWindow(g_Params.ScanStrategyWindClass, "Intraday Strategy Scanner")
    If (hWnd <> 0) Then
        BringWindowToTop (hWnd)
        SetActiveWindow (hWnd)
        If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogUserAction, "StrategyScanner activated , which was loaded before ETS Start.", Me.Caption
    Else
        ComandString = g_Params.ScanStrategyName & " " & g_Params.ScannersLogin & " " & g_Params.ScanStrategyPath
        sInfo.cb = Len(sInfo)

        If WaitForSingleObject(g_pStrategyInfo.hProcess, 0) = WAIT_OBJECT_0 Or g_pStrategyInfo.hProcess = 0 Then
            If CreateProcess(sNull, _
                             ComandString, _
                             ByVal 0&, ByVal 0&, 1&, NORMAL_PRIORITY_CLASS, ByVal 0&, sNull, sInfo, g_pStrategyInfo) = 0 Then
                gCmn.MyMsgBox Me, "Fail to run StrategyScanner application.", vbCritical
                If Not g_PerformanceLog Is Nothing Then _
                       g_PerformanceLog.LogMmInfo enLogUserAction, "StrategyScanner loaded successfully. Comand String : " & ComandString, Me.Caption
            End If
        Else
            hWnd = FindWindow(g_Params.ScanStrategyWindClass, "Intraday Strategy Scanner")
            BringWindowToTop (hWnd)
            SetActiveWindow (hWnd)
            If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogUserAction, "Second StrategyScanner is not loaded, because another copy already runs, it is activated", Me.Caption
        End If
    End If
End Sub

Public Sub RunOptionsScanner()
    On Error Resume Next
    Dim sInfo As STARTUPINFO
    Dim ComandString As String
    Dim sNull As String
    Dim nOperation&
    Dim hWnd As Long
        
    hWnd = FindWindow(g_Params.ScanOptionsWindClass, "Intraday Scanner")
    If (hWnd <> 0) Then
        BringWindowToTop (hWnd)
        SetActiveWindow (hWnd)
         If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogUserAction, "OptionScanner activated , which was loaded before ETS Start.", Me.Caption
    Else
        ComandString = g_Params.ScanOptionsName & " " & g_Params.ScannersLogin & " " & g_Params.ScanOptionsPath
        sInfo.cb = Len(sInfo)

        If WaitForSingleObject(g_pOptionInfo.hProcess, 0) = WAIT_OBJECT_0 Or g_pOptionInfo.hProcess = 0 Then
            If CreateProcess(sNull, _
                            ComandString, _
                            ByVal 0&, ByVal 0&, 1&, NORMAL_PRIORITY_CLASS, ByVal 0&, sNull, sInfo, g_pOptionInfo) = 0 Then
                gCmn.MyMsgBox Me, "Fail to run OptionScanner application.", vbCritical
                Else
                    If Not g_PerformanceLog Is Nothing Then _
                         g_PerformanceLog.LogMmInfo enLogUserAction, "OptionsScanner loaded successfully. Comand string: " & ComandString, Me.Caption
            End If
        Else
            hWnd = FindWindow(g_Params.ScanOptionsWindClass, "Intraday Scanner")
            BringWindowToTop (hWnd)
            SetActiveWindow (hWnd)
            If Not g_PerformanceLog Is Nothing Then _
                   g_PerformanceLog.LogMmInfo enLogUserAction, "Other OptionScanner is not loaded, because another copy already runs, it is activated.", Me.Caption
        End If
    End If
End Sub

Public Sub ShowFixForm(aFrm As Form, ByVal sWindowName As String, frmAtom As clsFormAtom)
    On Error GoTo EH
    
    If frmAtom Is Nothing Then
        Dim nOperation&
        
        Screen.MousePointer = vbHourglass
        DoEvents
        g_ViewFrmID = g_ViewFrmID + 1
        
        Set frmAtom = New clsFormAtom
        Set frmAtom.Frm = aFrm
        g_ViewFrm.Add frmAtom, CStr(g_ViewFrmID)
        Load frmAtom.Frm
        frmAtom.Frm.Init CStr(g_ViewFrmID)

        If Not g_Params.ShowWindowsInTaskbar Then
            frmAtom.Frm.ShowWindowInTaskbar g_Params.ShowWindowsInTaskbar, False
        End If
                
        frmAtom.Frm.Show
        ReloadWindowsList
        Screen.MousePointer = vbDefault
    Else
        frmAtom.Frm.SetFocus
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open " & sWindowName & " window."
    
    On Error Resume Next
    Unload aFrm
    g_ViewFrm.Remove CStr(g_ViewFrmID)
    ReloadWindowsList
    On Error GoTo 0
    Screen.MousePointer = vbDefault
End Sub

Public Sub ShowExerciseScreen()
    On Error Resume Next
    ShowFixForm frmAddExScreen, "Exercise Screen", g_frmEarlyExer
End Sub
