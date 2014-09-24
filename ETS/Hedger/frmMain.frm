VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmMain 
   BackColor       =   &H00006633&
   Caption         =   "EGAR Hedger"
   ClientHeight    =   7125
   ClientLeft      =   60
   ClientTop       =   630
   ClientWidth     =   11700
   Icon            =   "frmMain.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   7125
   ScaleWidth      =   11700
   Begin EgarHedgerSQL.ctlStocks ctlStocks1 
      Height          =   5655
      Left            =   6960
      TabIndex        =   7
      Top             =   720
      Width           =   4335
      _ExtentX        =   7646
      _ExtentY        =   9975
   End
   Begin EgarHedgerSQL.ctlSummary ctlSummary1 
      Height          =   5655
      Left            =   3000
      TabIndex        =   6
      Top             =   720
      Width           =   3615
      _ExtentX        =   6376
      _ExtentY        =   9975
   End
   Begin EgarHedgerSQL.ctlSubportfolios2 ctlSubportfolios21 
      Height          =   5655
      Left            =   2040
      TabIndex        =   5
      Top             =   720
      Width           =   3375
      _ExtentX        =   5953
      _ExtentY        =   9975
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   2760
      Top             =   120
   End
   Begin VB.TextBox txtDDEData 
      Appearance      =   0  'Flat
      Height          =   975
      Left            =   8400
      TabIndex        =   4
      Top             =   3360
      Visible         =   0   'False
      Width           =   1335
   End
   Begin VB.TextBox txtDDESelection 
      Appearance      =   0  'Flat
      Height          =   285
      Left            =   8400
      TabIndex        =   3
      Top             =   3000
      Visible         =   0   'False
      Width           =   1335
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   8400
      Top             =   2400
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin EgarHedgerSQL.ctlPortfolio ctlPortfolio1 
      Height          =   5655
      Left            =   0
      TabIndex        =   2
      Top             =   720
      Width           =   3375
      _ExtentX        =   5953
      _ExtentY        =   9975
   End
   Begin EgarHedgerSQL.ctlTopPanel ctlTopPanel1 
      Height          =   735
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   11655
      _ExtentX        =   20558
      _ExtentY        =   1296
   End
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   6870
      Width           =   11700
      _ExtentX        =   20638
      _ExtentY        =   450
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   3
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            AutoSize        =   1
            Object.Width           =   16828
            Key             =   "pnStatus"
         EndProperty
         BeginProperty Panel2 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Alignment       =   1
            Key             =   "pnVega"
         EndProperty
         BeginProperty Panel3 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
            Object.Width           =   811
            MinWidth        =   811
            Key             =   "pnTerm"
         EndProperty
      EndProperty
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuImportTrades 
         Caption         =   "Import Trades ..."
      End
      Begin VB.Menu mnuFileBar3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuNew 
         Caption         =   "&New"
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuLoad 
         Caption         =   "&Open ..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuSave 
         Caption         =   "&Save"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuSaveAs 
         Caption         =   "Save &As ..."
      End
      Begin VB.Menu mnuFileBar1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuPrint 
         Caption         =   "&Print ..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuFileBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu mnuEdit 
      Caption         =   "&Edit"
      Begin VB.Menu mnuCopy 
         Caption         =   "&Copy"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuPaste 
         Caption         =   "&Paste"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuEditBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCopyPortfolio 
         Caption         =   "C&opy All"
      End
      Begin VB.Menu mnuPastePortfolio 
         Caption         =   "P&aste All"
      End
      Begin VB.Menu mnuDDELink 
         Caption         =   "&Link Portfolio"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewHelpTags 
         Caption         =   "Help &Tags"
         Shortcut        =   ^H
      End
      Begin VB.Menu mnuViewFeedLog 
         Caption         =   "View feed log ..."
      End
   End
   Begin VB.Menu mnuService 
      Caption         =   "&Service"
      Begin VB.Menu mnuGetRealtimePrices 
         Caption         =   "Get realtime prices ..."
         Visible         =   0   'False
      End
      Begin VB.Menu mnuGetIVData 
         Caption         =   "Get IV data ..."
      End
      Begin VB.Menu mnuServeceBar2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuDBConnection 
         Caption         =   "&DB Connection ..."
      End
      Begin VB.Menu mnuInternetConnections 
         Caption         =   "Internet Connections ..."
      End
      Begin VB.Menu mnuFileBar2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuHedgeParameters 
         Caption         =   "Hedge parameters ..."
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&Help"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Contents ..."
         Visible         =   0   'False
      End
      Begin VB.Menu mnuEgarWeb 
         Caption         =   "Egar on the &Web ..."
      End
      Begin VB.Menu mnuHelpBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuAbout 
         Caption         =   "&About ..."
         Shortcut        =   ^{F1}
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1
Public DDEActive As Boolean
Private TaskQueryUnload As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Enum for Tabs on the main screen
'
Private Enum BTN_TOP
    BTN_PORTFOLIO = 0
    BTN_SUMMARY
    BTN_SHORT_HEDGE
    BTN_LONG_HEDGE
    BTN_STOCKS
End Enum

Private CurrentStep As BTN_TOP
Private DefaultFolder As String
Private Editable As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Enum for buttons on the 'Portfolio' tab
'
Private Enum BTN_PORTFOLIO_BUTTONS
    BTN_PORTFOLIO_NEW = 0
    BTN_PORTFOLIO_LOAD
    BTN_PORTFOLIO_SAVE
    BTN_PORTFOLIO_COPY
    BTN_PORTFOLIO_PASTE
    BTN_PORTFOLIO_GET_PRICES
    BTN_PORTFOLIO_GET_IV_DATA
    BTN_PORTFOLIO_NEXT
    BTN_PORTFOLIO_LINK
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Enum for buttons on the 'SubPortfolios' tab
'
Private Enum BTN_SUBPORTFOLIOS_BUTTONS
    BTN_SUBPORTFOLIOS_GET_PRICES
    BTN_SUBPORTFOLIOS_GET_IV_DATA
    BTN_SUBPORTFOLIOS_NEXT
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Enum for buttons on the 'Summary' tab
'
Private Enum BTN_SUMMARY_BUTTONS
    BTN_SUMMARY_GET_PRICES
    BTN_SUMMARY_GET_IV_DATA
    BTN_SUMMARY_NEXT
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlTopPanel1_ButtonClick(ByVal Index As Long)
    On Error Resume Next
    SelectTab Index
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SelectTab(ByVal Index As BTN_TOP)
    On Error Resume Next
    StartTask
    HideStocWindowStatic
    ctlPortfolio1.SaveExpiry
    
    Select Case Index
    Case BTN_PORTFOLIO
        PortfolioHedge.NeedSplitPortfolio = True
        ctlPortfolio1.Visible = True
        ctlSubportfolios21.Visible = False
        ctlSummary1.Visible = False
        ctlStocks1.Visible = False
        ctlStocks1.fVisible = False
        ctlStocks1.NeedRefresh = True
        If PortfolioHedge.fNeedRefreshPortfolio Then
            ctlPortfolio1.DisplayOutputRS PortfolioHedge.rsPortfolioData
            PortfolioHedge.fNeedRefreshPortfolio = False
        End If

    Case BTN_SHORT_HEDGE
        If PortfolioHedge.NeedSplitPortfolio Then
            If Not SplitPortfolio Then
                GoTo Failed
            End If
        End If
        ctlSubportfolios21.ShortMode = True
        ctlSubportfolios21.Visible = True
        ctlSubportfolios21.Refresh
        ctlPortfolio1.Visible = False
        ctlSummary1.Visible = False
        ctlStocks1.Visible = False
        ctlStocks1.fVisible = False
    
    Case BTN_LONG_HEDGE
        If PortfolioHedge.NeedSplitPortfolio Then
            If Not SplitPortfolio Then
                GoTo Failed
            End If
        End If
        ctlSubportfolios21.ShortMode = False
        ctlSubportfolios21.Visible = True
        ctlSubportfolios21.Refresh
        ctlPortfolio1.Visible = False
        ctlSummary1.Visible = False
        ctlStocks1.Visible = False
        ctlStocks1.fVisible = False
    
    Case BTN_SUMMARY
        If PortfolioHedge.NeedSplitPortfolio Then
            If Not SplitPortfolio Then
                GoTo Failed
            End If
        End If
        ctlSummary1.Visible = True
        ctlPortfolio1.Visible = False
        ctlSubportfolios21.Visible = False
        ctlStocks1.Visible = False
        ctlStocks1.fVisible = False
    
    Case BTN_STOCKS
        ctlSummary1.Visible = False
        ctlPortfolio1.Visible = False
        ctlSubportfolios21.Visible = False
        ctlStocks1.Visible = True
        ctlStocks1.fVisible = True
        ctlStocks1.Refresh
    
    End Select
    ctlTopPanel1.BtnSelect Index
    CurrentStep = Index
    
    StopTask
    Exit Sub
Failed:
    If Not gRuntime Then Exit Sub
    If CurrentStep = BTN_PORTFOLIO Then
        If PortfolioHedge.fNeedRefreshPortfolio Then
            ctlPortfolio1.DisplayOutputRS PortfolioHedge.rsPortfolioData
            PortfolioHedge.fNeedRefreshPortfolio = False
        End If
    End If
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolio1_ButtonClick(Index As Integer)
    On Error Resume Next
    StartTask
    Select Case Index
    Case BTN_PORTFOLIO_NEW
        NewPortfolio
        
    Case BTN_PORTFOLIO_LOAD
        LoadPortfolioFromFile
        
    Case BTN_PORTFOLIO_SAVE
        SavePortfolioToFile False
        
    Case BTN_PORTFOLIO_COPY
        SavePortfolioToClipboard
        
    Case BTN_PORTFOLIO_PASTE
        LoadPortfolioFromClipboard
        
    Case BTN_PORTFOLIO_LINK
        SetDDESelection
    
'    Case BTN_PORTFOLIO_GET_PRICES
'        LoadStockPrices
        'UpdatePositions
    
    Case BTN_PORTFOLIO_GET_IV_DATA
        LoadStockIVData
        'UpdatePositions
    
    Case BTN_PORTFOLIO_NEXT
        SelectTab BTN_SUMMARY
    
    Case Else
        Debug.Assert False
    End Select
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolios21_ButtonClick(Index As Integer)
    On Error Resume Next
    StartTask
    HideStocWindowStatic
    Select Case Index
'    Case BTN_SUBPORTFOLIOS_GET_PRICES
'        LoadIndexPrices
'        If Not gRuntime Then Exit Sub
'        LoadStockPrices
'        If Not gRuntime Then Exit Sub
'        UpdatePositions
'        If Not gRuntime Then Exit Sub
'        '//////////////////////////////
'        CalculateContractAmmounts   '/
'        '////////////////////////////
    Case BTN_SUBPORTFOLIOS_GET_IV_DATA
        LoadStockIVData
        If Not gRuntime Then Exit Sub
        UpdatePositions
        If Not gRuntime Then Exit Sub
        '//////////////////////////////
        CalculateContractAmmounts   '/
        '////////////////////////////
    Case BTN_SUBPORTFOLIOS_NEXT
        SelectTab BTN_SUMMARY
    Case Else
        Debug.Assert False
    End Select
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSummary1_ButtonClick(Index As Integer)
    On Error Resume Next
    StartTask
    Select Case Index
'    Case BTN_SUMMARY_GET_PRICES
'        LoadIndexPrices
'        If Not gRuntime Then Exit Sub
'        UpdatePositions
'        If Not gRuntime Then Exit Sub
'        '//////////////////////////////
'        CalculateContractAmmounts   '/
'        '////////////////////////////
    Case BTN_SUMMARY_GET_IV_DATA
        LoadStockIVData
        If Not gRuntime Then Exit Sub
        UpdatePositions
        If Not gRuntime Then Exit Sub
        '//////////////////////////////
        CalculateContractAmmounts   '/
        '////////////////////////////
    Case BTN_SUMMARY_NEXT
        SelectTab BTN_SHORT_HEDGE
    Case Else
        Debug.Assert False
    End Select
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Initialize()
    On Error Resume Next
    Set PortfolioHedge = gPortfolioHedge
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub

    TaskQueryUnload = False
    
    With ctlPortfolio1
        .Left = 0
        .Visible = True
    End With
    
    With ctlSubportfolios21
        .Left = 0
        .Visible = False
    End With
    
    With ctlSummary1
        .Left = 0
        .Visible = False
    End With
    
    With ctlStocks1
        .Left = 0
        .Visible = False
    End With
    
    DefaultFolder = App.Path
    'PortfolioHedge.LoadHedgeParameters
    PortfolioHedge.LoadStockData
    PortfolioHedge.InitIndices
    NewPortfolio
    
    SelectTab BTN_PORTFOLIO
    
    '///////////////
    ' Logging   '//
    '/////////////
    LogClear
'    LogSave ""
'    LogSave String(80, "/")
    LogSave "Program started " & Now
    LogSave ""
    
'    Load frmDataLog
    
    PortfolioHedge_VegaModeChanged PortfolioHedge.fVegaMode
    PortfolioHedge_HVTermChanged PortfolioHedge.g_lTerm
    
    If Not CheckInternetConnection Then
        EditInternetConnections
    End If
    
    On Error Resume Next
    mnuViewHelpTags.Checked = g_bShowHelpTags
    Me.StatusBar1.Panels("pnStatus").Text = ""
    PortfolioHedge.FeedInProgress = False
    Editable = True
    Exit Sub
Herr:
    ShowError
    Me.StatusBar1.Panels("pnStatus").Text = ""
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If TaskQueryUnload Then
        Exit Sub
    End If
    If UnloadMode = vbFormCode Or UnloadMode = vbFormControlMenu Then
        Cancel = True
        If PortfolioHedge.FeedInProgress Then
            If vbYes = gCmn.MyMsgBox("Price feed is in progress. Do you want to exit application?", vbYesNo) Then
                GoTo DoExit
            Else
                Exit Sub
            End If
'            gCmn.MyMsgBox "Price feed is in progress. You have to stop price feed before exit."
'            Cancel = True
        End If
DoExit:
        PortfolioHedge.CancelTask
        DoEvents
        If PortfolioHedge.NeedSavePortfolio Then
            If PortfolioHedge.IsEmptyPortfolio Then
                PortfolioHedge.NeedSavePortfolio = False
            Else
                Select Case gCmn.MyMsgBox("Do you want to save the current portfolio?", vbYesNoCancel)
                Case vbYes
                    SavePortfolioToFile False
                Case vbCancel
                    Exit Sub
                End Select
            End If
        End If
    
        gRuntime = False
        TaskQueryUnload = True
        Timer1.Enabled = True
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Timer1_Timer()
    If TaskQueryUnload Then
        Unload Me
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    
    gRuntime = False
    
    If Not PortfolioHedge Is Nothing Then
        PortfolioHedge.CancelTask
        PortfolioHedge.DisconnectMT
    End If
    
    LogSave String(80, "/")
    LogSave "Program terminated " & Now
    LogSave ""
    
    g_lMainState = Me.WindowState
    If g_lMainState = vbNormal Then
        g_lMainTop = Me.Top
        g_lMainLeft = Me.Left
        g_lMainWidth = Me.Width
        g_lMainHeight = Me.Height
    End If
    
    Dim i As Long
    For i = 1 To 100
        DoEvents
    Next
    
    Shutdown
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Resize()
Dim x As Double
Dim y As Double

    On Error Resume Next
    x = Max(0, ScaleWidth)
    y = Max(0, ScaleHeight - ctlTopPanel1.Height - StatusBar1.Height)
    
    With ctlTopPanel1
        .Width = x
    End With
    
    With ctlPortfolio1
        .Width = x
        .Height = y
    End With
    
    With ctlSubportfolios21
        .Width = x
        .Height = y
    End With
    
    With ctlSummary1
        .Width = x
        .Height = y
    End With
    
    With ctlStocks1
        .Width = x
        .Height = y
    End With
    
'    X = Max(0, ScaleWidth - picLeft.Width)
'    'lblGridCaption1.Width = X + 60
'    With picLeft
'        .Left = 0
'        .Height = Max(0, ScaleHeight - .Top - StatusBar1.Height)
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuHelpContents_Click()
    On Error Resume Next
    DisplayHelpContents
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuAbout_Click()
    On Error Resume Next
    frmAbout.Show vbModal, Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuExit_Click()
    On Error Resume Next
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    PortfolioHedge.StartTask Caption, CanCancel
    DoEvents
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    If Not PortfolioHedge Is Nothing Then
        PortfolioHedge.StopTask Caption
    End If
'    If TaskQueryUnload Then
'        Unload Me
'    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    ctlTopPanel1.StartTask Caption, CanCancel
    EnableControls False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    ctlTopPanel1.StopTask Caption
    EnableControls True
    Me.StatusBar1.Panels("pnStatus").Text = ""
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EnableControls(ByVal Enable As Boolean)
    On Error Resume Next
    Editable = Enable
    
    'mnuFile.Enabled = Enable
    mnuImportTrades.Enabled = Enable
    mnuNew.Enabled = Enable
    mnuLoad.Enabled = Enable
    mnuSave.Enabled = Enable
    mnuSaveAs.Enabled = Enable
    mnuPrint.Enabled = Enable
    'mnuExit.Enabled = Enable

    mnuEdit.Enabled = Enable
'    mnuCopy.Enabled = Enable
'    mnuPaste.Enabled = Enable
'    mnuCopyPortfolio.Enabled = Enable
'    mnuPastePortfolio.Enabled = Enable
'    mnuDDELink.Enabled = Enable
    
    'mnuView.Enabled = Enable
    mnuViewHelpTags.Enabled = Enable
    mnuViewFeedLog.Enabled = Enable
    
    'mnuService.Enabled = Enable
    mnuGetRealtimePrices.Enabled = Enable
    mnuGetIVData.Enabled = Enable
    mnuHedgeParameters.Enabled = Enable
    mnuInternetConnections.Enabled = Enable
    mnuDBConnection.Enabled = Enable
    
    'mnuHelp.Enabled = Enable
    'mnuHelpContents.Enabled = Enable
    'mnuEgarWeb.Enabled = Enable
    'mnuAbout.Enabled = Enable

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub NewPortfolio()
    On Error Resume Next
    If PortfolioHedge.NeedSavePortfolio Then
        If PortfolioHedge.IsEmptyPortfolio Then
            PortfolioHedge.NeedSavePortfolio = False
        Else
            Select Case gCmn.MyMsgBox("Do you want to save the current portfolio?", vbYesNoCancel)
            Case vbYes
                SavePortfolioToFile False
            Case vbCancel
                Exit Sub
            End Select
        End If
    End If
    On Error GoTo 0
    ClearDDE
    PortfolioHedge.NewPortfolio
    PortfolioHedge.PortfolioName = ""
    AfterNewPortfolio
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub AfterNewPortfolio()
    On Error Resume Next
    ctlSubportfolios21.Clear
    ctlSubportfolios21.DisplayOutputRS Nothing
    ctlSummary1.Clear
    ctlStocks1.NeedRefresh = True
    ctlStocks1.Refresh
    PortfolioHedge.NeedSavePortfolio = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub AfterLoadPortfolio()
'    AfterNewPortfolio
    ctlSubportfolios21.Clear
    ctlSubportfolios21.DisplayOutputRS Nothing
    ctlSummary1.Clear
    PortfolioHedge.NeedSavePortfolio = False
    Select Case CurrentStep
    Case BTN_PORTFOLIO
    Case BTN_SHORT_HEDGE, BTN_LONG_HEDGE
        SplitPortfolio
    Case BTN_SUMMARY
        SplitPortfolio
    Case BTN_STOCKS
        ctlStocks1.NeedRefresh = True
        ctlStocks1.Refresh
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadPortfolioFromFile()
    On Error Resume Next
    If PortfolioHedge.NeedSavePortfolio Then
        If PortfolioHedge.IsEmptyPortfolio Then
            PortfolioHedge.NeedSavePortfolio = False
        Else
            Select Case gCmn.MyMsgBox("Do you want to save the current portfolio?", vbYesNoCancel)
            Case vbYes
                SavePortfolioToFile False
            Case vbCancel
                Exit Sub
            End Select
        End If
    End If
    On Error GoTo Herr
    With Me.CommonDialog1
        .CancelError = True
        .Filter = "Portfolio files(*.pfl)|*.pfl|All files(*.*)|*.*"
        .InitDir = DefaultFolder
        .FileName = ""
        .ShowOpen
        DefaultFolder = .InitDir
        
        InitializePortfolioFromFile .FileName
        
    End With
    AfterLoadPortfolio
    Exit Sub
Herr:
    'Debug.Print Err.Number, Err.Description
    If Err.Number = 32755 Then
        ' Cancel error
        Exit Sub
    End If
    ShowError
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub InitializePortfolioFromFile(ByVal sFileName As String)
    Dim sText As String
    
    PortfolioHedge.PortfolioName = sFileName
    If Len(sFileName) > 0 Then
        ClearDDE
        PortfolioHedge.NewPortfolio
        StartTask "Loading Portfolio..."
        
        sText = LoadStringFromFile(sFileName)
        sText = Replace(sText, ",", ".")
        LoadPorfolioFromText sText, True
        
        StopTask
    End If
End Sub
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SavePortfolioToFile(ByVal SaveAs As Boolean)
Dim f As String
Dim S As String
Dim fNeedChoose As Boolean
    On Error GoTo Herr
    If PortfolioHedge.IsEmptyPortfolio Then
        gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
        Exit Sub
    End If
    f = PortfolioHedge.PortfolioName
    fNeedChoose = SaveAs Or (f = "NewPortfolio.pfl")
    With Me.CommonDialog1
        .CancelError = True
        .Filter = "Portfolio files(*.pfl)|*.pfl|All files(*.*)|*.*"
        .InitDir = DefaultFolder
        .FileName = f
        If fNeedChoose Then
            .ShowSave
        End If
        DefaultFolder = .InitDir
        f = .FileName
    End With
    PortfolioHedge.PortfolioName = f
    If Len(f) > 0 Then
        If fNeedChoose Then
            If Dir(f) <> "" Then
                If vbYes <> gCmn.MyMsgBox( _
                    "File exist:" & vbCrLf & _
                    f & vbCrLf & _
                    "Do you want to overwrite if?" _
                    , vbYesNo) Then
                    Exit Sub
                End If
            End If
        End If
        StartTask "Saving data"
        '////////////////////////////////
        S = SavePorfolioToText(True) '//
        S = Replace(S, ",", ".")    '//
        SaveStringToFile f, S       '/
        '////////////////////////////
        PortfolioHedge.NeedSavePortfolio = False
        StopTask
    End If
    Exit Sub
Herr:
    Debug.Print Err.Number
    If Err.Number = 32755 Then
        ' Cancel error
        Exit Sub
    End If
    ShowError
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ClipboardNotEmpty() As Boolean
    With Clipboard
        If .GetFormat(vbCFText) Then
            If Len(.GetText) > 0 Then
                ClipboardNotEmpty = True
            End If
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadPortfolioFromClipboard()
Dim S As String
    On Error GoTo Herr
    With Clipboard
        If .GetFormat(vbCFText) Then
            Debug.Print "LoadPortfolioFromClipboard"
            ClearDDE
            S = .GetText
            LoadPorfolioFromText S, False
        End If
    End With
    AfterLoadPortfolio
    PortfolioHedge.NeedSavePortfolio = True
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SavePortfolioToClipboard()
    SaveTextToClipboard SavePorfolioToText(False)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SaveTextToClipboard(ByVal S As String)
    On Error GoTo Herr
    With Clipboard
        .Clear
        .SetText S, vbCFText
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Parses text stream and builds porfolio
'
Public Function LoadPorfolioFromText(ByVal S As String, ByVal FromFile As Boolean) As Boolean
    On Error GoTo Herr
    LoadPorfolioFromText = PortfolioHedge.LoadPorfolioFromText(S, FromFile)
    Exit Function
Herr:
    Debug.Print Err.Number, Err.Description
    Err.Raise vbObjectError, "Load Portfolio", "Loading Portfolio data from file failed due to the following error: " & vbCrLf & Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SavePorfolioToText(ByVal ToFile As Boolean) As String
    SavePorfolioToText = PortfolioHedge.SavePorfolioToText(ToFile)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuNew_Click()
    On Error GoTo Herr
    NewPortfolio
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuLoad_Click()
    On Error GoTo Herr
    LoadPortfolioFromFile
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuImportTrades_Click()
    On Error GoTo Herr
    LoadPortfolioImportTrades
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuSave_Click()
    On Error GoTo Herr
    SavePortfolioToFile False
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuSaveAs_Click()
    On Error GoTo Herr
    SavePortfolioToFile True
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuEdit_Click()
    On Error Resume Next
    mnuCopy.Enabled = Editable And Not (g_grd Is Nothing)
    mnuCopyPortfolio.Enabled = Editable
    Select Case CurrentStep
    Case BTN_PORTFOLIO
        mnuPaste.Enabled = mnuCopy.Enabled And ClipboardNotEmpty
        mnuPastePortfolio.Enabled = Editable And ClipboardNotEmpty
        mnuDDELink.Enabled = Editable
    Case Else
        mnuPaste.Enabled = False
        mnuPastePortfolio.Enabled = False
        mnuDDELink.Enabled = False
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuCopy_Click()
    On Error GoTo Herr
    CopyFucusGridToClipboard
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuCopyPortfolio_Click()
    On Error GoTo Herr
    Select Case CurrentStep
    Case BTN_PORTFOLIO
        'ExecuteCopy COPY_PORTFOLIO
'        If Not ctlPortfolio1.CopyToClipboard(False) Then
            StartTask "Copying..."
            SavePortfolioToClipboard
'        End If
    Case BTN_LONG_HEDGE
        StartTask "Copying..."
        ExecuteCopy COPY_LONG_SUBPORTFOLIO
    Case BTN_SHORT_HEDGE
        StartTask "Copying..."
        ExecuteCopy COPY_SHORT_SUBPORTFOLIO
    Case BTN_SUMMARY
        StartTask "Copying..."
        ExecuteCopy COPY_PORTFOLIO_SUMMARY
    Case BTN_STOCKS
        StartTask "Copying..."
        ExecuteCopy COPY_STOCKS
        'ctlStocks1.CopyToClipboard (False)
    End Select
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
    Resume Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuPastePortfolio_Click()
    On Error GoTo Herr
    StartTask
    LoadPortfolioFromClipboard
    StopTask
    Exit Sub
Herr:
    ShowError
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuPaste_Click()
    On Error GoTo Herr
    If CurrentStep = BTN_PORTFOLIO Then
        If ctlPortfolio1.CopyFromClipboard Then
            PortfolioHedge.fNeedRescan = True
        Else
'            LoadPortfolioFromClipboard
        End If
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolio1_Copy()
    On Error GoTo Herr
    ctlPortfolio1.CopyToClipboard False
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolio1_Paste()
    On Error GoTo Herr
    If ctlPortfolio1.CopyFromClipboard Then
        PortfolioHedge.fNeedRescan = True
    Else
        LoadPortfolioFromClipboard
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SplitPortfolio() As Boolean
    On Error GoTo Herr
    
''''    If CurrentStep = BTN_PORTFOLIO Then
''''        If PortfolioHedge.fNeedRescan Then
''''            Dim S As String
''''            If Not ctlPortfolio1.CopyToText(S, True) Then
''''                Debug.Assert False
''''                Exit Function
''''            End If
''''            If Not PortfolioHedge.LoadPorfolioFromText(S, False) Then
''''                Debug.Assert False
''''                Exit Function
''''            End If
''''        End If
''''    End If
    
'    If PortfolioHedge.IsEmptyPortfolio Then
''        gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
'        SplitPortfolio = True
'        Exit Function
'    End If
    
    StartTask "Calculating..."
    '/////////////////////////////////////////////////
    SplitPortfolio = PortfolioHedge.SplitPortfolio '/
    '///////////////////////////////////////////////

Done:
    On Error Resume Next
    StopTask
    Exit Function
Herr:
    ShowError
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UpdatePositions()
    On Error GoTo Herr
    StartTask "Calculating..."
    If Not gRuntime Then GoTo Done
    If PortfolioHedge Is Nothing Then GoTo Done
    PortfolioHedge.UpdatePortfolio
    If Not gRuntime Then GoTo Done
    If PortfolioHedge Is Nothing Then GoTo Done
    PortfolioHedge.UpdatePositions
    If Not gRuntime Then GoTo Done
    If PortfolioHedge Is Nothing Then GoTo Done
    PortfolioHedge.UpdateIndexIndexCorrelator
'    If Not gRuntime Then GoTo Done
'    If PortfolioHedge Is Nothing Then GoTo Done
'    PortfolioHedge.UpdateStockStockCorrelator
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
    Resume Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub CalculateContractAmmounts()
    On Error GoTo Herr
    
    If PortfolioHedge.IsEmptyPortfolio Then
'        gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
        Exit Sub
    End If
    
    StartTask "Calculating..."
    '//////////////////////////////////
    PortfolioHedge.CalculateContractAmmounts
    '////////////////////////////////
    
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
    Resume Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_NewPortfolio(rs As ADODB.Recordset)
    On Error GoTo Herr
    ctlPortfolio1.DisplayOutputRS rs
    PortfolioHedge.InitIndices
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_NewSubPortfolio(rs As ADODB.Recordset)
    On Error GoTo Herr
    '!!!
    ctlSubportfolios21.DisplayOutputRS rs
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolios21_IndexVisibleChanged(ByVal IndexSymbol As String, ByVal IndexVisible As Boolean)
Dim Criteria As String
    On Error GoTo Herr
    With PortfolioHedge.rsIndexOptions
        If .RecordCount <> 0 Then
            .MoveFirst
        End If
        Criteria = "IndexSymbol='" & IndexSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If Not .EOF Then
            !Visible = IndexVisible
            .Update
            
            StartTask "Calculating..."
            'ctlSubPortfolios21.UpdateOutputRS PortfolioHedge.rsPositions
            ctlSubportfolios21.DisplayOutputRS PortfolioHedge.rsPositions
        
        End If
    End With
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolios21_IndexSelectedChanged(ByVal IndexSymbol As String, ByVal IndexSelected As Boolean)
Dim Criteria As String
    On Error GoTo Herr
    PortfolioHedge.SetIndexSelected IndexSymbol, IndexSelected
    If PortfolioHedge.NeedSplitPortfolio Then
        StartTask "Calculating..."
        PortfolioHedge.SplitPortfolio
    End If
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub ctlSummary1_IndexPriceChanged(ByVal IndexSymbol As String, ByVal IndexPrice As Double, ContractsAmount As Long, ByVal DataStatus As enmDATA_STATUS)
'Dim Criteria As String
'    On Error GoTo Herr
'    If DataStatus = DATA_MANUAL Then
'        If IndexPrice < 0 Then
'            gCmn.MyMsgBox "Invalid input: Negative price.", vbExclamation
'            IndexPrice = 0
'        End If
'    End If
'    With PortfolioHedge.rsIndexOptions
'        If .RecordCount <> 0 Then
'            .MoveFirst
'        End If
'        Criteria = "IndexSymbol='" & IndexSymbol & "'"
'        .Find Criteria, 0, adSearchForward, adBookmarkFirst
'        If Not .EOF Then
'            !IndexPrice = IndexPrice
'            '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'            ' Calculate Index Contracts ammount
'            '
'            ContractsAmount = 0
'            If IndexPrice <> 0 Then
'                If IsNull(!QValue) Then
'                    ContractsAmount = 0
'                Else
'                    ContractsAmount = !QValue / IndexPrice
'                End If
'            End If
'            !IndexContractsAmount = ContractsAmount
'            .Update
'        End If
'    End With
'    Exit Sub
'Herr:
'    ShowError
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockPrices(Optional ByVal AllSymbols As Boolean = False) As Boolean
Dim tStart As Date
Dim tStop As Date
Dim Duration As Long
    On Error GoTo Herr
'    If Not AllSymbols Then
'        If PortfolioHedge.IsEmptyPortfolio Then
'            gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
'            Exit Function
'        End If
'    End If
    
    tStart = Now
    StartTask "Loading prices...", True
    frmDataLog.ShowLog
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Stocks price feed started " & tStart
    
    '//////////////////////////////////////////////
    PortfolioHedge.LoadStockPrices AllSymbols   '/
    '////////////////////////////////////////////
    
    LoadStockPrices = True
    
Done:
    On Error Resume Next
    tStop = Now
    Duration = DateDiff("s", tStart, tStop)
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Stocks price feed terminated " & tStop
    PortfolioHedge.LogSave "Task Duration: " & FormatDuration(Duration)
    StopTask
    Exit Function
Herr:
    ShowError
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexPrices() As Boolean
Dim tStart As Date
Dim tStop As Date
Dim Duration As Long
    On Error GoTo Herr
'    If PortfolioHedge.IsEmptyPortfolio Then
'        gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
'        Exit Function
'    End If
    
    tStart = Now
    StartTask "Loading prices...", True
    frmDataLog.ShowLog
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Indexes price feed started " & tStart
    
    '///////////////////////////////////////////////////
    LoadIndexPrices = PortfolioHedge.LoadIndexPrices '/
    '/////////////////////////////////////////////////

Done:
    On Error Resume Next
    tStop = Now
    Duration = DateDiff("s", tStart, tStop)
    Duration = DateDiff("s", tStart, tStop)
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Indexes price feed terminated " & tStop
    PortfolioHedge.LogSave "Task Duration: " & FormatDuration(Duration)
    
    StopTask
    Exit Function
Herr:
'    Cancel = True
    LastErrDescription = Err.Description
    ShowError
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIVData(Optional ByVal AllSymbols As Boolean = False) As Boolean
Dim tStart As Date
Dim tStop As Date
Dim Duration As Long
    On Error GoTo Herr
'    If Not AllSymbols Then
'        If PortfolioHedge.IsEmptyPortfolio Then
'            gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
'            Exit Function
'        End If
'    End If
    
    tStart = Now
    StartTask "Loading IV data...", True
    frmDataLog.ShowLog
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Stocks IV data feed started " & tStart
    
    '//////////////////////////////////////////////
    PortfolioHedge.LoadStockIVData AllSymbols   '/
    '////////////////////////////////////////////
    
    LoadStockIVData = True
    
Done:
    On Error Resume Next
    tStop = Now
    Duration = DateDiff("s", tStart, tStop)
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Stocks IV data feed terminated " & tStop
    PortfolioHedge.LogSave "Task Duration: " & FormatDuration(Duration)
    
    StopTask
    Exit Function
Herr:
    LastErrDescription = Err.Description
    ShowError
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuGetRealtimePrices_Click()
'    On Error GoTo Herr
'    Select Case CurrentStep
'    Case BTN_PORTFOLIO
'        LoadStockPrices False
'    Case BTN_SUMMARY
'        ctlSummary1_ButtonClick BTN_SUMMARY_GET_PRICES
'    Case BTN_SHORT_HEDGE
'        ctlSubPortfolios21_ButtonClick BTN_SUBPORTFOLIOS_GET_PRICES
'    Case BTN_LONG_HEDGE
'        ctlSubPortfolios21_ButtonClick BTN_SUBPORTFOLIOS_GET_PRICES
'    Case BTN_STOCKS
'        ctlStocks1.LoadPrices
'    End Select
'    Exit Sub
'Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuGetIVData_Click()
    On Error GoTo Herr
    Select Case CurrentStep
    Case BTN_PORTFOLIO
        LoadStockIVData False
    Case BTN_SUMMARY
        ctlSummary1_ButtonClick BTN_SUMMARY_GET_IV_DATA
    Case BTN_SHORT_HEDGE
        ctlSubPortfolios21_ButtonClick BTN_SUBPORTFOLIOS_GET_IV_DATA
    Case BTN_LONG_HEDGE
        ctlSubPortfolios21_ButtonClick BTN_SUBPORTFOLIOS_GET_IV_DATA
    Case BTN_STOCKS
        ctlStocks1.LoadIVData
    End Select
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlTopPanel1_CancelClick()
    CancelTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CancelTask()
    On Error GoTo Herr
    PortfolioHedge.CancelTask
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_Status(ByVal Message As String)
'    With Me.StatusBar1
'        .Panels.Item("pnStatus").Text = Message
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_LogMessage(ByVal Message As String)
    With Me.StatusBar1
        LogSave Message
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuViewFeedLog_Click()
    On Error GoTo Herr
    'Shell "Notepad.exe " & LogFile, vbNormalFocus
    frmDataLog.ShowHistory
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuHedgeParameters_Click()
    On Error GoTo Herr
    EditHedgeParameters
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuInternetConnections_Click()
    On Error GoTo Herr
    EditInternetConnections
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EditHedgeParameters()
Dim Criteria As String
    On Error GoTo Herr
    If frmEditHedgeParameters.Execute = vbOK Then
        With PortfolioHedge
            Select Case CurrentStep
            Case BTN_SHORT_HEDGE, BTN_LONG_HEDGE
                StartTask "Calculating..."
                .SplitPortfolio
            Case BTN_SUMMARY
                StartTask "Calculating..."
                .SplitPortfolio
            End Select
        End With
    End If
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
    Resume Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EditInternetConnections()
    If frmEditInternetConnections.Execute = vbOK Then
'        On Error Resume Next
'        PortfolioHedge.NeedReconnectMT
'        gCmn.MyMsgBox "You need to restart application for changes to become effective.", vbInformation
    End If
    PortfolioHedge.ReconnectIV
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetDDESelection()
    On Error GoTo Herr
    DDEActive = False
    With txtDDESelection
        .LinkMode = vbLinkNone
        .LinkTopic = "Excel|System"
        .LinkItem = "Selection"
        .LinkMode = vbLinkManual
        .LinkRequest
        SetDDERange .Text
        .LinkMode = vbLinkNone
    End With
    Exit Sub
Herr:
    If Err = 282 Then
    
        gCmn.MyMsgBox "Link to Excel workbook can't be established." & vbCrLf & _
            "" & vbCrLf & _
            "Instructions: Run MS Excel, " & vbCrLf & " open sheet and select cells region." & vbCrLf & _
            "Back to EGAR Hedger and click 'Link'."

    Else
        ShowError
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetDDERange(ByVal Selection As String)
Dim DDEItem As String
Dim Workbook As String
Dim Range As String
Dim Anchor As Long
Dim Msg As String
    
    DDEItem = Selection
    Anchor = InStr(DDEItem, "!")
    If Anchor > 0 Then
        Workbook = Left(DDEItem, Anchor - 1)
        Range = Mid(DDEItem, Anchor + 1)
        Debug.Print "Workbook", Workbook
        Debug.Print "Range", Range
        
        Debug.Assert False
        Msg = "Excel selection is : " & Workbook & "!" & Range & vbCrLf _
            & "All data in this range will be dynamicaly copied into Portfolio."
        If vbOK = gCmn.MyMsgBox(Msg, vbOKCancel Or vbInformation) Then
            With txtDDEData
                .LinkTopic = "Excel|" & Workbook
                .LinkItem = Range
                '.LinkItem = "C1:C2"
                .LinkMode = vbLinkAutomatic
                '.LinkMode = vbLinkNotify
            End With
            DDEActive = True
        End If
    Else
        '?
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearDDE()
    DDEActive = False
    With txtDDEData
        .LinkTopic = ""
        .LinkItem = ""
        .LinkMode = vbLinkNone
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtDDEData_Change()
Dim S As String
    'If CurrentStep = BTN_PORTFOLIO Then
    'End If
    If DDEActive Then
        S = txtDDEData.Text
        If Not LoadPorfolioFromText(S, False) Then
            ClearDDE
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuDDELink_Click()
    On Error GoTo Herr
    SetDDESelection
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_VegaModeChanged(ByVal NewVegaMode As Boolean)
    On Error Resume Next
    If NewVegaMode Then
        Me.StatusBar1.Panels("pnVega").Text = "Vega hedge"
    Else
        Me.StatusBar1.Panels("pnVega").Text = "Delta hedge"
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_HVTermChanged(ByVal NewHVTerm As Long)
    On Error Resume Next
    Me.StatusBar1.Panels("pnTerm").Text = Format$(NewHVTerm)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuEgarWeb_Click()
    On Error Resume Next
    ShellExecute 0&, "Open", "http://www.egartech.com", "", vbNullString, 1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_PortfolioNameChanged(ByVal sNewValue As String)
    On Error Resume Next
    Me.Caption = APP_TITLE & ": " & sNewValue
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuPrint_Click()
    On Error GoTo Herr
    StartTask "Printing..."
    Select Case CurrentStep
    Case BTN_PORTFOLIO
        frmPrintPreview.ExecutePrint PRINT_PORTFOLIO
    Case BTN_SUMMARY
        frmPrintPreview.ExecutePrint PRINT_PORTFOLIO_SUMMARY
    Case BTN_SHORT_HEDGE
        frmPrintPreview.ExecutePrint PRINT_SHORT_SUBPORTFOLIO
    Case BTN_LONG_HEDGE
        frmPrintPreview.ExecutePrint PRINT_LONG_SUBPORTFOLIO
    Case BTN_STOCKS
        frmPrintPreview.ExecutePrint PRINT_STOCKS
    End Select
Done:
    On Error Resume Next
    StopTask
    Exit Sub
Herr:
    ShowError
    Resume Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuViewHelpTags_Click()
    On Error Resume Next
    mnuViewHelpTags.Checked = Not mnuViewHelpTags.Checked
    g_bShowHelpTags = mnuViewHelpTags.Checked
    PortfolioHedge.ShowHelpTagsUpdate
    If Not g_frmHelp Is Nothing And Not g_bShowHelpTags Then
        g_frmHelp.CloseMe
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuDBConnection_Click()
Dim fDisconnected As Boolean
Try:
    On Error GoTo Failed
    If ChangeDatabase(Me.hWnd) Then
        fDisconnected = True
        PortfolioHedge.NewPortfolio
        PortfolioHedge.UnLoadStockData
        CloseDataBase
        If OpenDataBase(Me.hWnd) Then
            PortfolioHedge.LoadStockData
            fDisconnected = False
        End If
    End If
    If fDisconnected Then
        Unload Me
    End If
    Exit Sub
Failed:
    ShowError
    Resume Try
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadPortfolioImportTrades()
    On Error Resume Next
    If PortfolioHedge.NeedSavePortfolio Then
        If PortfolioHedge.IsEmptyPortfolio Then
            PortfolioHedge.NeedSavePortfolio = False
        Else
            Select Case gCmn.MyMsgBox("Do you want to save the current portfolio?", vbYesNoCancel)
            Case vbYes
                SavePortfolioToFile True
            Case vbCancel
                Exit Sub
            End Select
        End If
    End If
    
    On Error GoTo Herr
    Dim rs As ADODB.Recordset
    ClearDDE
    PortfolioHedge.PortfolioName = "New Portfolio"
    
    StartTask "Importing trades...", True
    frmDataLog.ShowLog
    PortfolioHedge.LogSave ""
    PortfolioHedge.LogSave "Import trades"
    
    If frmImportTrades.Execute(rs) Then
        AfterLoadPortfolio
        PortfolioHedge.NeedSavePortfolio = True
    End If
Done:
    StopTask
    Exit Sub
Herr:
    'Debug.Print Err.Number, Err.Description
    ShowError
    Resume Done
End Sub

