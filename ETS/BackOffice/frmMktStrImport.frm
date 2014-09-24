VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMktStrImport 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Import"
   ClientHeight    =   5640
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8220
   Icon            =   "frmMktStrImport.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5640
   ScaleWidth      =   8220
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox chkFilterDividends 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Filter out wrong dividends "
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   6360
      TabIndex        =   11
      Top             =   3360
      Value           =   1  'Checked
      Width           =   1755
   End
   Begin VB.Frame framePrices 
      Caption         =   "Prices"
      Height          =   1455
      Left            =   6240
      TabIndex        =   7
      Top             =   4080
      Width           =   1935
      Begin VB.OptionButton optPricesYest 
         Caption         =   "Yest"
         Height          =   375
         Left            =   120
         TabIndex        =   10
         Top             =   960
         Width           =   1695
      End
      Begin VB.OptionButton optPricesLast 
         Caption         =   "Last"
         Height          =   375
         Left            =   120
         TabIndex        =   9
         Top             =   600
         Width           =   1695
      End
      Begin VB.OptionButton optPricesNone 
         Caption         =   "None"
         Height          =   375
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Width           =   1695
      End
   End
   Begin VB.CheckBox chkDeleteOptions 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Delete options, futures && future options which are not on provider today"
      ForeColor       =   &H80000008&
      Height          =   1215
      Left            =   6360
      TabIndex        =   6
      Top             =   2100
      Width           =   1755
   End
   Begin VB.CheckBox chkRewriteVola 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Rewrite existing IV surface"
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   6360
      TabIndex        =   5
      Top             =   1440
      Value           =   1  'Checked
      Width           =   1755
   End
   Begin VB.CheckBox chkImportVola 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Import default IV"
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   6360
      TabIndex        =   4
      Top             =   840
      Value           =   1  'Checked
      Width           =   1755
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   6360
      TabIndex        =   2
      Top             =   360
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Cancel"
   End
   Begin MSComctlLib.ProgressBar pbOptions 
      Height          =   300
      Left            =   120
      TabIndex        =   1
      Top             =   360
      Width           =   6015
      _ExtentX        =   10610
      _ExtentY        =   529
      _Version        =   393216
      BorderStyle     =   1
      Appearance      =   0
      Max             =   300
   End
   Begin MSForms.TextBox txtLog 
      Height          =   4695
      Left            =   120
      TabIndex        =   3
      Tag             =   "xxx"
      Top             =   840
      Width           =   6015
      VariousPropertyBits=   -1400879073
      BorderStyle     =   1
      ScrollBars      =   3
      Size            =   "10610;8281"
      SpecialEffect   =   0
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblOptions 
      BackStyle       =   0  'Transparent
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   135
      Width           =   3495
   End
End
Attribute VB_Name = "frmMktStrImport"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents m_PriceProvider As PRICEPROVIDERSLib.StructureProviderEx
Attribute m_PriceProvider.VB_VarHelpID = -1
Private WithEvents m_QuotesProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute m_QuotesProvider.VB_VarHelpID = -1
Private WithEvents m_IVDataLoader As DHIVFeeder5.IVDataLoader
Attribute m_IVDataLoader.VB_VarHelpID = -1
Private WithEvents m_HttpFeeder As HttpFeeder
Attribute m_HttpFeeder.VB_VarHelpID = -1

Private m_sUndSymbol As String
Private m_nUndID As Long
Private m_enUndType As eContractType

Private m_bGotUndInfo As Boolean
Private m_bGotLastOption As Boolean
Private m_bCancelFlag As Boolean
Private m_bConnected As Boolean
Private m_bConnectedPrice As Boolean
Private m_bConnectBroken As Boolean
Private m_bHasOptions As Boolean
Private m_bSkipUnd As Boolean

Private m_bReWriteVola As Boolean
Private m_bLoadVola As Boolean

Private m_nOptionsCount As Long
Private m_nIVPointCount As Long

Private m_collUndExpNoIV As clsUndExpNoIVColl

Private m_collFutureRoots As clsFutureRootColl
Private m_nCurrentFutureRootIdx As Long
Private m_bGotLastFuture As Boolean

Private m_collFutures As clsFutureColl
Private m_collFuturesOpt As clsFutureColl
Private m_collFuturesPointer As clsFutureColl
Private m_nCurrentFutureIdx As Long
Private m_bGotLastFutureOption As Boolean
Private m_bHasFutureOptions As Boolean
Private m_nFutureOptionsCount As Long
Private m_nUndExchangeID As Long

Private m_bGotFutureInfo As Boolean
Private m_bSkipFuture As Boolean
Private m_bTheFirstFuture As Boolean

Private m_bLoadFuturePrices As Boolean
Private m_nOptionsImportNum As Long

Private m_sOptionsImportText As String

Private m_iPriceToLoad As Integer
Const PRICE_NONE As Integer = 0
Const PRICE_LAST As Integer = 1
Const PRICE_YEST As Integer = 2

Const OTM_IVTYPE As Integer = 2

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal sSymbol As String, ByVal nId As Long, ByVal enTypeId As eContractType, ByVal nUndExchID As Long) As Boolean
    On Error GoTo EH
    m_sUndSymbol = sSymbol
    m_nUndID = nId
    m_enUndType = enTypeId
    m_nOptionsImportNum = 0
    m_nUndExchangeID = nUndExchID
    
    Load Me
    btnCancel.Caption = "Start"
    
    ' Assume we are connected
    m_bConnected = True
    m_bConnectedPrice = True
    Me.Show vbModal
    Unload Me
    Exit Function
EH:
    ShowError
End Function

Private Sub chkImportVola_Click()
    On Error Resume Next
    
    If chkImportVola.Value = 0 Then
        chkRewriteVola.Value = 0
        chkRewriteVola.Enabled = False
    Else
        chkRewriteVola.Value = 1 'IIf(m_enUndType = GINT_COMMODITIES, 1, 0)
        chkRewriteVola.Enabled = False 'm_enUndType <> GINT_COMMODITIES
    End If
    
End Sub

#If DEBUG_REPLY > 1 Then

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_HttpFeeder_GotReply(ByVal message As String)
    Debug.Print "m_HttpFeeder_GotReply:"
    Debug.Print message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_HttpFeeder_GotRequest(ByVal message As String)
    Debug.Print "m_HttpFeeder_GotRequest:"
    Debug.Print message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_HttpFeeder_LogMessage(ByVal message As String)
    Debug.Print "m_HttpFeeder_LogMessage:"
    Debug.Print message
End Sub
#End If

Private Sub m_IVDataLoader_Status(ByVal message As String)
    Debug.Print message
End Sub

Private Sub m_IVDataLoader_SymbolIVPoint(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double)

    On Error GoTo EH

    If m_bLoadVola Then
        If m_bReWriteVola Then ' rewrite of existing volas
                                    
            m_nIVPointCount = m_nIVPointCount + 1
            
            Call gDBW.usp_CustomStrikeSkewDefaultPointFromIV_Save(m_nUndID, Strike, Expiration, OTM_IVTYPE, Vola * 100)
            
        Else
            If Not m_collUndExpNoIV(Expiration) Is Nothing Then
                
                m_nIVPointCount = m_nIVPointCount + 1
                
                Call gDBW.usp_CustomStrikeSkewDefaultPointFromIV_Save(m_nUndID _
                , Strike, Expiration, OTM_IVTYPE, Vola * 100)
            End If
        End If
    End If

Exit Sub

EH:
    AddToLog "Error occured while saving IV Point : " & Symbol & ", " & Strike & ", " & Expiration
    AddToLog Err.Description & " " & Err.Source
End Sub

Private Sub SymbolIVPointSave(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double, ByVal ExpiryOV As Date)

    On Error GoTo EH

    If m_bLoadVola Then
        If m_bReWriteVola Then ' rewrite of existing volas - If True
                                    
            m_nIVPointCount = m_nIVPointCount + 1
            
            Call gDBW.usp_CustomStrikeSkewDefaultPointFromIV_Save(m_nUndID, Strike, Expiration, OTM_IVTYPE, Vola * 100)
'
'        Else
'            If Not m_collUndExpNoIV(Expiration) Is Nothing Then
'
'                m_nIVPointCount = m_nIVPointCount + 1
'
'                Call gDBW.usp_CustomStrikeSkewDefaultPointFromIV_Save(m_nUndID _
'                , Strike, Expiration, OTM_IVTYPE, Vola * 100)
'            End If
        End If
    End If

Exit Sub

EH:
    AddToLog "Error occured while saving IV Point : " & Symbol & ", " & Strike & ", " & Expiration
    AddToLog Err.Description & " " & Err.Source
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PriceProviderReset()
    On Error GoTo EH
    
    ' reloading mytrack
    If g_StructureProvider Is Nothing Then
        Set g_StructureProvider = New PRICEPROVIDERSLib.StructureProviderEx
    End If
    Set m_PriceProvider = g_StructureProvider
    Dim Provider1 As PRICEPROVIDERSLib.IProvider
    Set Provider1 = m_PriceProvider
    If Not Provider1 Is Nothing Then
        Provider1.Type = g_Params.PriceProviderType
    End If
    If Not m_bConnected Then
        AddToLog "Connecting to real-time feed..."
        m_PriceProvider.Connect
        m_bConnected = True
    End If
    If m_iPriceToLoad <> PRICE_NONE Then
        QuotesProviderReset
    End If
    Me.Refresh
    DoEvents
            
    Exit Sub
EH:
    Dim Msg As String
    btnCancel.Caption = "Close"
    Msg = Err.Description
    AddToLog Msg
End Sub

Private Sub QuotesProviderReset()
    'On Error GoTo EH
    
    ' reloading mytrack
    If g_PriceProvider Is Nothing Then
        Set g_PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    End If
    Set m_QuotesProvider = g_PriceProvider
    Dim Provider1 As PRICEPROVIDERSLib.IProvider
    Set Provider1 = m_QuotesProvider
    If Not Provider1 Is Nothing Then
        Provider1.Type = g_Params.PriceProviderType
    End If
    If Not m_bConnectedPrice Then
        AddToLog "Connecting to real-time feed..."
        m_QuotesProvider.Connect
        m_bConnectedPrice = True
    End If
    'Me.Refresh
    'DoEvents
            
    Exit Sub
EH:
    Dim Msg As String
    btnCancel.Caption = "Close"
    Msg = Err.Description
    AddToLog Msg
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error GoTo EH
    
    gCmn.CustomizeForm Me
    
    lblOptions.Caption = "Retrieve options progress"
    
    m_nOptionsCount = 0
    
    If m_enUndType = GINT_INDEXES Or m_enUndType = GINT_COMMODITIES Then
        chkDeleteOptions.Caption = "Delete options, futures && future options which are not on provider today"
        LoadFutureRoots
    ElseIf m_enUndType = GINT_FUTURES Then
        chkDeleteOptions.Caption = "Delete future options which are not on provider today"
    Else
        chkDeleteOptions.Caption = "Delete options which are not on provider today"
    End If
        
    chkImportVola.Enabled = m_enUndType <> GINT_FUTURES
    chkRewriteVola.Enabled = m_enUndType <> GINT_FUTURES And m_enUndType <> GINT_COMMODITIES
    chkImportVola.Value = IIf(m_enUndType <> GINT_FUTURES, 1, 0)
    chkRewriteVola.Value = IIf(m_enUndType <> GINT_FUTURES, 1, 0)
    'Set RewriteVola to True for all types if ImportVola is Checked
    chkRewriteVola.Visible = False
'    If (m_enUndType = GINT_COMMODITIES) Then
'        chkImportVola.Enabled = False
'        chkRewriteVola.Enabled = False
'        chkImportVola.Value = 0
'        chkRewriteVola.Value = 0
'        chkImportVola.Visible = False
'        chkRewriteVola.Visible = False
'    Else
        chkImportVola.Visible = True
'        chkRewriteVola.Visible = True
'    End If
    
    optPricesNone.Value = True
    m_iPriceToLoad = PRICE_NONE
    
    If (m_enUndType = GINT_STOCKS) Then
        framePrices.Enabled = False
        optPricesNone.Enabled = False
        optPricesLast.Enabled = False
        optPricesYest.Enabled = False
    End If
    
    chkFilterDividends.Enabled = m_enUndType <> GINT_FUTURES
    chkFilterDividends.Value = IIf(m_enUndType <> GINT_FUTURES, 1, 0)
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Error loading import form"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = btnCancel.Caption = "Cancel"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    m_PriceProvider.CancelOptions
    Set m_PriceProvider = Nothing
    
    If Not m_IVDataLoader Is Nothing Then
        m_IVDataLoader.CancelTask
        Set m_IVDataLoader = Nothing
    End If
    
    If Not m_HttpFeeder Is Nothing Then
        Set m_HttpFeeder = Nothing
    End If
    
    Set m_collUndExpNoIV = Nothing
    Set m_collFutureRoots = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    If btnCancel.Caption = "Close" Then
        Unload Me
    ElseIf btnCancel.Caption = "Cancel" Then
        m_bCancelFlag = True
        If Not m_PriceProvider Is Nothing Then
            m_PriceProvider.CancelStock
            m_PriceProvider.CancelOptions
        End If
        
        If Not m_IVDataLoader Is Nothing Then
            m_IVDataLoader.CancelTask
        End If
    Else
        chkImportVola.Enabled = False
        chkRewriteVola.Enabled = False
        chkDeleteOptions.Enabled = False
        
        framePrices.Enabled = False
        optPricesNone.Enabled = False
        optPricesLast.Enabled = False
        optPricesYest.Enabled = False
        chkFilterDividends.Enabled = False
        
        m_bReWriteVola = False
        m_bLoadVola = False
        
        If chkImportVola.Value <> 0 Then
            m_bLoadVola = True
        End If
        
        If chkRewriteVola.Value <> 0 Then
            If gCmn.MyMsgBox("Existing volatility surface will be lost." & vbCrLf & _
            "Proceed anyway?", vbExclamation Or vbYesNo) = vbYes Then
                m_bReWriteVola = True
            Else
                chkRewriteVola.Value = 0
                m_bReWriteVola = False
                chkImportVola.Value = 0
                m_bLoadVola = False
            End If
        End If
        
        btnCancel.Caption = "Cancel"
        
        ImportData
    End If
End Sub


Private Sub ImportData()
    On Error Resume Next
    txtLog.SetFocus
    
    On Error GoTo Failed
    PriceProviderReset
'    If m_iPriceToLoad <> PRICE_NONE Then
'        QuotesProviderReset
'    End If
    
    ' setting flags
    m_bCancelFlag = False
        
    If m_enUndType <> GINT_FUTURES And m_enUndType <> GINT_COMMODITIES Then
        ImportOptions
    End If
    
    If Not m_bCancelFlag Then
        If m_enUndType = GINT_COMMODITIES Then
            If Not m_collFutureRoots Is Nothing Then
                ImportFutures
                ImportIndexFutureOptions
                If m_iPriceToLoad <> PRICE_NONE Then
                    ImportFuturesPrices
                    ImportFutureOptPrices
                End If
            Else
                ImportIndexFutureOptions
                If m_iPriceToLoad <> PRICE_NONE Then
                    ImportFuturesPrices
                    ImportFutureOptPrices
                End If
            End If
        Else
            If m_enUndType = GINT_INDEXES And Not m_collFutureRoots Is Nothing Then
                ImportFutures
                
                ImportIndexFutureOptions
                If m_iPriceToLoad <> PRICE_NONE Then
                    ImportFuturesPrices
                    ImportFutureOptPrices
                End If
            ElseIf m_enUndType = GINT_FUTURES Then
                ImportFutureOptions
                If m_iPriceToLoad <> PRICE_NONE Then
                    If m_collFutures Is Nothing Then Set m_collFutures = New clsFutureColl
                    m_collFutures.Clear
                    Dim aFuture As clsFutureAtom
                    Set aFuture = m_collFutures.Add(m_sUndSymbol)
                    aFuture.Symbol = m_sUndSymbol
                    aFuture.ID = m_nUndID
                    
                    ImportFuturesPrices
                    ImportFutureOptPrices
                End If
            End If
        End If
    End If
    
    If Not m_bCancelFlag And m_bLoadVola Then
        ImportDefaultVolas
    End If
Done:
    On Error Resume Next
    btnCancel.Caption = "Close"
    Exit Sub
Failed:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading symbols. " & CStr(Err.Number) & " " & Err.Description
    gCmn.ErrorMsgBox "Error loading symbols"
    GoTo Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportOptions()
    On Error GoTo Failed

    Dim sSymbol As String
    Dim sMsg As String
    Dim bDeleteOptions, bAllOptionsDel As Boolean
    Dim sStockExchange As String
    
    If m_nUndExchangeID <> 0 Then
        Dim rstExchange As ADODB.Recordset
        Set rstExchange = gDBW.usp_Exchange_Get(m_nUndExchangeID)
        If rstExchange.RecordCount <> 1 Then
            Debug.Print "Unable to ident an exchange."
        Else
            With rstExchange
                .MoveFirst
                sStockExchange = DBRead(!vcExchangeCode)
            End With
            sStockExchange = Trim(sStockExchange)
        End If
    Else
        sStockExchange = ""
    End If
    
    bAllOptionsDel = False
    
    AddToLog "Option import started."
    Me.Caption = "Import options"
        
    bDeleteOptions = (chkDeleteOptions.Value = 1)
    
    m_nOptionsCount = 0
    
    On Error GoTo EH
    ' symbol import
          
    lblOptions.Caption = "Retrieve options on " & m_sUndSymbol
    pbOptions.Value = pbOptions.Min

Retry1:
    If Not m_bConnected Then
        m_PriceProvider.Connect
    End If
    m_bConnectBroken = False
    
    m_bGotUndInfo = False
    m_bGotLastOption = False
    
    gDBW.usp_EodMsLoad_Init m_nUndID
                 
    Dim sParams As PRICEPROVIDERSLib.StockParams
    sParams.Exchange = sStockExchange
    sParams.Stock = m_sUndSymbol
    m_bSkipUnd = False
    m_PriceProvider.RequestStock sParams
    ' Wait for request done signal
    Do
        If m_bGotUndInfo Then
            Exit Do
        End If
        If m_bCancelFlag Then
            Exit Do
        End If
        If m_bConnectBroken Then
            m_bConnected = False
            GoTo Retry1
        End If
        
        DoEvents
    Loop
    If m_bSkipUnd Then
        If Not m_bCancelFlag And bDeleteOptions Then
            bAllOptionsDel = DeleteOptions
            DoEvents
        End If
        GoTo SkipOptionLoad
    End If
    
    gDBW.usp_EodMsUnderlyingCache_Save m_nUndID
    
    Dim oParams As PRICEPROVIDERSLib.OptionParams
    oParams.Exchange = sStockExchange
    oParams.UnderlyingSymbol = m_sUndSymbol
    m_bHasOptions = False
    m_sOptionsImportText = ""
    m_PriceProvider.RequestOptions oParams
    ' Wait for request done signal
    Do
        If m_bGotLastOption Then
            pbOptions.Value = pbOptions.Max
            Exit Do
        End If
        If m_bCancelFlag Then
            Exit Do
        End If
        If m_bConnectBroken Then
            m_bConnected = False
            GoTo Retry1
        End If
        DoEvents
    Loop
        
    If Not m_bCancelFlag And bDeleteOptions Then
        bAllOptionsDel = DeleteOptions
    End If
    
    If Not bDeleteOptions Or bAllOptionsDel Then _
        gDBW.usp_EodMsUnderlyingCache_Check m_nUndID
    
SkipOptionLoad:

    If Not m_bHasOptions Then
        AddToLog "No options has been loaded for symbol " & m_sUndSymbol & "."
    End If
            
    ' import end
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Option import completed" & vbCrLf
        If m_bHasOptions Then
            sMsg = sMsg & CStr(m_nOptionsCount) & " options were updated." & vbCrLf
        End If
        AddToLog sMsg
    Else
        AddToLog "Data import terminated by user request."
    End If

Done:
    Exit Sub
    
EH:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve information for " & m_sUndSymbol & ": " & Error
    GoTo Done
    Exit Sub

Failed:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading Options. " & CStr(Err.Number) & " " & Err.Description

    gCmn.ErrorMsgBox "Error loading Options."
    GoTo Done
End Sub

Private Function DeleteOptions() As Boolean
    Dim rstLog As ADODB.Recordset
    
    Set rstLog = gDBW.usp_ExpiredOptions_Del(m_nUndID)
    DeleteOptions = True
    
    If rstLog.RecordCount > 0 Then
        DeleteOptions = False
        rstLog.MoveFirst
    End If
    
    Do While Not rstLog.EOF And Not m_bCancelFlag
        AddToLog "Failed to remove " & rstLog.Fields("vcSymbol") & ". Symbol has trades or orders."
        rstLog.MoveNext
    Loop
    Set rstLog = Nothing
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportFutures()
    On Error GoTo Failed

    Dim sSymbol As String
    Dim sMsg As String
    Dim bDeleteFutures As Boolean
    Dim IStructureProv As PRICEPROVIDERSLib.IStructureProvider2
    
    Set IStructureProv = m_PriceProvider
    If IStructureProv Is Nothing Then Exit Sub
        
    AddToLog "Futures import started."
    Me.Caption = "Import futures"
    
    If m_collFutures Is Nothing Then Set m_collFutures = New clsFutureColl

    m_collFutures.Clear
        
    bDeleteFutures = (chkDeleteOptions.Value = 1)
        
    For m_nCurrentFutureRootIdx = 1 To m_collFutureRoots.Count
        
        On Error GoTo EH
        lblOptions.Caption = "Retrieve futures of " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & " root"
        pbOptions.Value = pbOptions.Min
    
Retry1:
        If Not m_bConnected Then
            m_PriceProvider.Connect
        End If
        m_bConnectBroken = False
        
        m_bGotLastFuture = False
                             
        Dim sParams As PRICEPROVIDERSLib.FutureRootParams
        
        sParams.FuturesRootSymbol = m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol
        
        IStructureProv.RequestFutures sParams
        
        ' Wait for request done signal
        Do
            If m_bGotLastFuture Then
                pbOptions.Value = pbOptions.Max
                Exit Do
            End If
            If m_bCancelFlag Then
                Exit Do
            End If
            If m_bConnectBroken Then
                m_bConnected = False
                GoTo Retry1
            End If
            DoEvents
        Loop
            
'        If m_collFutureRoots(m_nCurrentFutureRootIdx).HasFutures Then
'           GoTo NextFutureRoot
'        End If
            
        If Not m_collFutureRoots(m_nCurrentFutureRootIdx).HasFutures Then
            AddToLog "No futures has been loaded for root " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & "."
        End If

        If Not m_bCancelFlag And bDeleteFutures Then
            DeleteFutures m_collFutureRoots(m_nCurrentFutureRootIdx).RootID
        End If

'Retry2:
'        If Not m_bConnected Then
'            m_PriceProvider.Connect
'        End If
'        m_bConnectBroken = False
'
'        m_bGotLastFuture = False
'
'        sParams.FuturesRootSymbol = "/" & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol
'
'        IStructureProv.RequestFutures sParams
'
'        ' Wait for request done signal
'        Do
'            If m_bGotLastFuture Then
'                pbOptions.Value = pbOptions.Max
'                Exit Do
'            End If
'            If m_bCancelFlag Then
'                Exit Do
'            End If
'            If m_bConnectBroken Then
'                m_bConnected = False
'                GoTo Retry2
'            End If
'            DoEvents
'        Loop
'
'        If Not m_collFutureRoots(m_nCurrentFutureRootIdx).HasFutures Then
'            AddToLog "No futures has been loaded for root " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & "."
'        End If
'
'        If Not m_bCancelFlag And bDeleteFutures Then
'            DeleteFutures m_collFutureRoots(m_nCurrentFutureRootIdx).RootID
'        End If

NextFutureRoot:
        
    Next
            
    ' import end
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Futures import completed" & vbCrLf
        sMsg = sMsg & CStr(m_collFutures.Count) & " futures were updated." & vbCrLf
        AddToLog sMsg
    Else
        AddToLog "Data import terminated by user request."
    End If

Done:
    Exit Sub
    
EH:
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve information for " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & ": " & Error
    GoTo NextFutureRoot
    Exit Sub

Failed:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading futures."
    gCmn.ErrorMsgBox "Error loading futures."
    GoTo Done
End Sub

Private Function DeleteFutures(ByVal nFutureRootId) As Boolean
    Dim rstLog As ADODB.Recordset
    
    Set rstLog = gDBW.usp_EodOldFutures_Del(nFutureRootId)
    DeleteFutures = True
    
    If rstLog.RecordCount > 0 Then
        DeleteFutures = False
        rstLog.MoveFirst
    End If
    
    Do While Not rstLog.EOF And Not m_bCancelFlag
        AddToLog "Failed to remove " & rstLog.Fields("vcSymbol") & ". Contract has trades or orders."
        rstLog.MoveNext
    Loop
    Set rstLog = Nothing
End Function

Private Sub ImportFuturesPrices()
    On Error GoTo Failed

    Dim sSymbol As String
    Dim sMsg As String
    Dim bDeleteFutures As Boolean
    Dim PriceProvider As PRICEPROVIDERSLib.PriceInfo
    Dim pbOptionsValue As Double
    
    m_bLoadFuturePrices = True
    
    Set PriceProvider = m_QuotesProvider
    If PriceProvider Is Nothing Then Exit Sub
        
    Set m_collFuturesPointer = m_collFutures
        
       
    AddToLog "Future price import started."
    Me.Caption = "Import future prices"
    
    If m_collFutures Is Nothing Then
    AddToLog "Future price import stoped."
    Exit Sub
    End If
    
    If m_collFutures.Count = 0 Then
        AddToLog "Future price import stoped."
    Exit Sub
    End If
    'dProcent = (pbOptions.Max - pbOptions.Min) / m_collFutures.Count
    pbOptions.Min = 0
    pbOptions.Value = 0
    pbOptions.Max = m_collFutures.Count
    
    For m_nCurrentFutureIdx = 1 To m_collFutures.Count
Retry1:
        On Error GoTo EH
        lblOptions.Caption = "Retrieve future prices of " & m_collFutures(m_nCurrentFutureIdx).Symbol & " future"
        If pbOptions.Value < pbOptions.Max Then pbOptions.Value = pbOptions.Value + 1
        If Not m_bConnectedPrice Then
            m_QuotesProvider.Connect
        End If
        m_bConnectBroken = False
        
        m_bGotLastFuture = False
                             
        Dim sParams As PRICEPROVIDERSLib.QuoteUpdateParams
        
        sParams.Symbol = m_collFutures(m_nCurrentFutureIdx).Symbol
        sParams.Type = enFUT
        
        PriceProvider.RequestLastQuote sParams
        
        ' Wait for request done signal
        Do
            If m_bGotLastFuture Then
                'pbOptions.Value = pbOptions.Max
                'pbOptionsValue = dProcent + pbOptions.Value 'pbOptions.Max
                'If pbOptionsValue > pbOptions.Max Then
                '    pbOptionsValue = pbOptions.Max
                'End If
                'pbOptions.Value = pbOptions.Max
                Exit Do
            End If
            If m_bCancelFlag Then
                Exit Do
            End If
            If m_bConnectBroken Then
                m_bConnectedPrice = False
                GoTo Retry1
            End If
            DoEvents
        Loop
        GoTo NextFutureRoot
                        
'        If Not m_collFutures(m_nCurrentFutureIdx).HasFutures Then
'            AddToLog "No futures has been loaded for root " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & "."
'        End If

'        If Not m_bCancelFlag And bDeleteFutures Then
'            DeleteFutures m_collFutureRoots(m_nCurrentFutureRootIdx).RootID
'        End If

EH:
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve prices for " & m_collFutures(m_nCurrentFutureIdx).Symbol & ": " & Error
    
NextFutureRoot:
    Next
            
    ' import end
Done:
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Future prices import completed" & vbCrLf
        sMsg = sMsg & CStr(m_collFutures.Count) & " future prices were updated." & vbCrLf
        AddToLog sMsg
    Else
        AddToLog "Price import terminated by user request."
    End If
    
    Exit Sub
Failed:
    On Error Resume Next
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading future prices."
    gCmn.ErrorMsgBox "Error loading future prices."
    GoTo Done
End Sub

Private Sub ImportFutureOptPrices()
    On Error GoTo Failed

    Dim sSymbol As String
    Dim sMsg As String
    Dim bDeleteFutures As Boolean
    Dim PriceProvider As PRICEPROVIDERSLib.PriceInfo
    Dim pbOptionsValue As Double
    
    m_bLoadFuturePrices = False
    
    Set m_collFuturesPointer = m_collFuturesOpt
    
    Set PriceProvider = m_QuotesProvider
    If PriceProvider Is Nothing Then Exit Sub
        
    AddToLog "Future option prices import started."
    Me.Caption = "Import future option prices"
    
    If m_collFuturesOpt Is Nothing Then
        AddToLog "Future option price import stoped."
    Exit Sub
    End If

    
    If m_collFuturesOpt.Count = 0 Then
        AddToLog "Future option price import stoped."
    Exit Sub
    End If

    'dProcent = (pbOptions.Max - pbOptions.Min) / m_collFuturesOpt.Count
    'pbOptions.Value = pbOptions.Min
    pbOptions.Min = 0
    pbOptions.Max = m_collFuturesOpt.Count
    pbOptions.Value = 0

    For m_nCurrentFutureIdx = 1 To m_collFuturesOpt.Count
        
        On Error GoTo EH
Retry1:
        lblOptions.Caption = "Retrieve future option prices of " & m_collFuturesOpt(m_nCurrentFutureIdx).Symbol & " future"
    
        If pbOptions.Value < pbOptions.Max Then pbOptions.Value = pbOptions.Value + 1
        If Not m_bConnectedPrice Then
            m_QuotesProvider.Connect
        End If
        m_bConnectBroken = False
        
        m_bGotLastFuture = False
                             
        Dim sParams As PRICEPROVIDERSLib.QuoteUpdateParams
        
        sParams.Symbol = m_collFuturesOpt(m_nCurrentFutureIdx).Symbol
        sParams.Type = enFOP
        
        PriceProvider.RequestLastQuote sParams
        
        ' Wait for request done signal
        Do
            If m_bGotLastFuture Then
                'pbOptionsValue = dProcent + pbOptions.Value 'pbOptions.Max
                'If pbOptionsValue > pbOptions.Max Then
                '    pbOptionsValue = pbOptions.Max
                'End If
                'pbOptions.Value = pbOptionsValue
                Exit Do
            End If
            If m_bCancelFlag Then
                Exit Do
            End If
            If m_bConnectBroken Then
                m_bConnectedPrice = False
                GoTo Retry1
            End If
            DoEvents
        Loop
        GoTo NextFutureRoot
        
'        If Not m_collFuturesOpt(m_nCurrentFutureIdx).HasFutures Then
'            AddToLog "No futures has been loaded for root " & m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol & "."
'        End If

'        If Not m_bCancelFlag And bDeleteFutures Then
'            DeleteFutures m_collFutureRoots(m_nCurrentFutureRootIdx).RootID
'        End If

EH:
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve prices for " & m_collFuturesOpt(m_nCurrentFutureIdx).Symbol & ": " & Error
    
NextFutureRoot:
    Next
    
Done:
    ' import end
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Future option prices import completed" & vbCrLf
        sMsg = sMsg & CStr(m_collFuturesOpt.Count) & " future option prices were updated." & vbCrLf
        AddToLog sMsg
    Else
        AddToLog "Price import terminated by user request."
    End If

    Exit Sub

Failed:
    On Error Resume Next
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading futures."
    gCmn.ErrorMsgBox "Error loading futures prices."
    GoTo Done
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportIndexFutureOptions()
    On Error GoTo Failed

    Dim sSymbol$, sMsg$
    Dim bDeleteFutureOptions As Boolean
    Dim IStructureProv As PRICEPROVIDERSLib.IStructureProvider2
    
    Set IStructureProv = m_PriceProvider
    If IStructureProv Is Nothing Then Exit Sub
        
    AddToLog "Futures options import started."
    Me.Caption = "Import futures options "
    
    If m_collFuturesOpt Is Nothing Then Set m_collFuturesOpt = New clsFutureColl
    m_collFuturesOpt.Clear
    
    bDeleteFutureOptions = (chkDeleteOptions.Value = 1)
    m_nFutureOptionsCount = 0
        
    For m_nCurrentFutureIdx = 1 To m_collFutures.Count
        
        On Error GoTo EH
        lblOptions.Caption = "Retrieve future options of " & m_collFutures(m_nCurrentFutureIdx).Symbol & " future"
        pbOptions.Value = pbOptions.Min
    
Retry1:
        If Not m_bConnected Then
            m_PriceProvider.Connect
        End If
        m_bConnectBroken = False
        
        m_bGotLastFutureOption = False
        m_bHasFutureOptions = False
                             
        Dim sParams As PRICEPROVIDERSLib.FuturesOptionParams
        
        sParams.FutureSymbol = m_collFutures(m_nCurrentFutureIdx).Symbol
        
        IStructureProv.RequestFuturesOptions sParams
                       
        ' Wait for request done signal
        Do
            If m_bGotLastFutureOption Then
                pbOptions.Value = pbOptions.Max
                Exit Do
            End If
            If m_bCancelFlag Then
                Exit Do
            End If
            If m_bConnectBroken Then
                m_bConnected = False
                GoTo Retry1
            End If
            DoEvents
        Loop
            
        If Not m_bHasFutureOptions Then
            AddToLog "No future options has been loaded for future " & m_collFutures(m_nCurrentFutureIdx).Symbol & "."
        End If
        
        If Not m_bCancelFlag And bDeleteFutureOptions Then
            DeleteFutureOptions m_collFutures(m_nCurrentFutureIdx).ID
        End If
NextFuture:
    Next
            
    On Error Resume Next
    ' import end
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Futures options import completed" & vbCrLf
        sMsg = sMsg & CStr(m_nFutureOptionsCount) & " futures options were updated." & vbCrLf
        AddToLog sMsg
    Else
        AddToLog "Data import terminated by user request."
    End If

Done:
    Exit Sub
EH:
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve information for " & m_collFutures(m_nCurrentFutureIdx).Symbol & ": " & Error
    GoTo NextFuture
    Exit Sub

Failed:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading Future options. " & CStr(Err.Number) & " " & Err.Description
    gCmn.ErrorMsgBox "Error loading  Future options."
    GoTo Done
End Sub

Private Function DeleteFutureOptions(ByVal nFutureId) As Boolean
    Dim rstLog As ADODB.Recordset
    
    Set rstLog = gDBW.usp_EodOldFutureOptions_Del(nFutureId)
    DeleteFutureOptions = True
    
    If rstLog.RecordCount > 0 Then
        DeleteFutureOptions = False
        rstLog.MoveFirst
    End If
    
    Do While Not rstLog.EOF And Not m_bCancelFlag
        AddToLog "Failed to remove " & rstLog.Fields("vcSymbol") & ". Contract has trades or orders."
        rstLog.MoveNext
    Loop
    Set rstLog = Nothing
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportFutureOptions()
    Dim bDeleteFutureOptions As Boolean, sMsg$
    Dim IStructureProv As PRICEPROVIDERSLib.IStructureProvider2
    On Error GoTo Failed
    
    Set IStructureProv = m_PriceProvider
    If IStructureProv Is Nothing Then Exit Sub
    
    AddToLog "Future Option import started."
    Me.Caption = "Import future options"
        
    bDeleteFutureOptions = (chkDeleteOptions.Value = 1)
    
    m_nFutureOptionsCount = 0
    
    If m_collFuturesOpt Is Nothing Then Set m_collFuturesOpt = New clsFutureColl
    m_collFuturesOpt.Clear
    
    On Error GoTo EH
    lblOptions.Caption = "Retrieve future options on " & m_sUndSymbol
    pbOptions.Value = pbOptions.Min

Retry1:
    If Not m_bConnected Then
        IStructureProv.Connect
    End If
    m_bConnectBroken = False
    
    m_bGotFutureInfo = False
    m_bGotLastFutureOption = False
                 
    Dim sParams As PRICEPROVIDERSLib.FutureParams
    Dim sOptParams As PRICEPROVIDERSLib.OptionParams
    sParams.FutureSymbol = m_sUndSymbol
    m_bSkipFuture = False
    IStructureProv.RequestFuture sParams
    m_bTheFirstFuture = True
    ' Wait for request done signal
    Do
        If m_bGotFutureInfo Then
            Exit Do
        End If
        If m_bCancelFlag Then
            Exit Do
        End If
        If m_bConnectBroken Then
            m_bConnected = False
            GoTo Retry1
        End If
        
        DoEvents
    Loop
    
    If Not m_bGotFutureInfo Then
    
Retry1_G:
    sParams.FutureSymbol = m_sUndSymbol & "G"
    m_bSkipFuture = False
    IStructureProv.RequestFuture sParams

    ' Wait for request done signal
    Do
        If m_bGotFutureInfo Then
            Exit Do
        End If
        If m_bCancelFlag Then
            Exit Do
        End If
        If m_bConnectBroken Then
            m_bConnected = False
            GoTo Retry1_G
        End If

        DoEvents
    Loop

    End If

    If m_bSkipFuture Then
        GoTo SkipOptionLoad
    End If
    
    m_bTheFirstFuture = False
    Dim oParams As PRICEPROVIDERSLib.FuturesOptionParams
    oParams.Exchange = ""
    oParams.FutureSymbol = m_sUndSymbol
   
   m_bHasFutureOptions = False
    IStructureProv.RequestFuturesOptions oParams

'    If g_Params.PriceProviderType <> 6 Then
'        oParams.FutureSymbol = "/" & m_sUndSymbol & "G"
'        IStructureProv.RequestFuturesOptions oParams
'    Else
'        oParams.FutureSymbol = m_sUndSymbol
'    End If
    
    
    ' Wait for request done signal
    Do
        If m_bGotLastFutureOption Then
            pbOptions.Value = pbOptions.Max
            Exit Do
        End If
        If m_bCancelFlag Then
            Exit Do
        End If
        If m_bConnectBroken Then
            m_bConnected = False
            GoTo Retry1
        End If
        DoEvents
    Loop
        
SkipOptionLoad:
    On Error Resume Next
    
    If Not m_bHasFutureOptions Then
        AddToLog "No future options has been loaded for symbol " & m_sUndSymbol & "."
    End If

    If Not m_bCancelFlag And bDeleteFutureOptions Then
        DeleteFutureOptions m_nUndID
    End If
            
    ' import end
    If Not m_bCancelFlag Then
        pbOptions.Value = pbOptions.Max
        sMsg = "Future option import completed" & vbCrLf
        If m_bHasFutureOptions Then
            sMsg = sMsg & CStr(m_nFutureOptionsCount) & " future options were updated." & vbCrLf
        End If
        AddToLog sMsg
    Else
        AddToLog "Data import terminated by user request."
    End If

Done:
    Exit Sub
    
EH:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    AddToLog "Error retrieve information for " & m_sUndSymbol & ": " & Error
    GoTo Done
    Exit Sub

Failed:
    m_bCancelFlag = True
    Debug.Print Err.Number, Err.Description
    Debug.Assert False
    AddToLog "Error loading future options. " & CStr(Err.Number) & " " & Err.Description
    gCmn.ErrorMsgBox "Error loading future options."
    GoTo Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddToLog(ByVal s As String)
    On Error Resume Next
    With txtLog
        .SelStart = Len(.Text)
        .SelText = s & vbCrLf
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
        If ErrorNumber = enNotConnected _
        Or ErrorNumber = enConnectionWasDisconnected Then m_bConnectBroken = True
    
    If m_bCancelFlag Then
        Exit Sub
    End If
    If ReqType = enRequestOptions Then
        m_bGotLastOption = True
        If ErrorNumber = enNoAccessForData Then
            m_bSkipUnd = True
        End If
        AddToLog "Price provider error for " & Request.UnderlyingSymbol & " options request: " & Description
    ElseIf ReqType = enRequestStock Then
        m_bGotUndInfo = True
        AddToLog "Price provider error for " & Request.Stock & " request: " & Description
        If ErrorNumber = enNoDataAvailableForSymbol Then
            m_bSkipUnd = True
        End If
    ElseIf ReqType = enRequestFutures Then
        m_bGotLastFuture = True
        AddToLog "Price provider error for " & Request.FuturesRootSymbol & " futures request: " & Description
    ElseIf ReqType = enRequestFuturesOptions Then
        If m_bTheFirstFuture Then
            m_bTheFirstFuture = False
                             Else
        m_bGotLastFutureOption = True
            AddToLog "Price provider error for " & Request.FutureSymbol & " future options request: " & Description
        
        End If
    ElseIf ReqType = enRequestFuture Then
        m_bGotFutureInfo = True
        AddToLog "Price provider error for " & Request.FutureSymbol & " future request: " & Description
        If ErrorNumber = enNoDataAvailableForSymbol Or ErrorNumber = enSymbolNotSupported Then
            m_bSkipFuture = True
        End If
    Else
        AddToLog "Price provider error: " & Description
    End If
End Sub

Private Sub m_PriceProvider_OnFuture(Params As PRICEPROVIDERSLib.FutureParams, Results As PRICEPROVIDERSLib.FutureResultsEx)
    Dim nId&, sBadContractName$
    'Dim bWithG As Boolean
    'bWithG = False
    On Error GoTo EH
    If m_bCancelFlag Then
        Exit Sub
    End If

    If m_sUndSymbol <> Params.FutureSymbol Then
        If m_sUndSymbol & "G" <> Params.FutureSymbol Then
           Exit Sub
                                                        Else
           'bWithG = True
           Results.Symbol = Left(Results.Symbol, (Len(Results.Symbol) - 1))
        End If
    End If

    m_bGotFutureInfo = True
    With Results
        nId = gDBW.usp_BoFuture_Import( _
            m_nUndID, _
            Null, _
            .ExpirationDate, _
            .Symbol, _
            .Description, _
            .ContractSize, _
            .OptionContractSize, _
            .ClosePrice, _
            sBadContractName, _
            .MinimumTick, _
            .UnitOfMeasure, _
            .UnitOfMeasureCurrency, _
            .ExpirationDate _
            )
    End With

    If nId = -2 Then
        AddToLog "Future Data save error: '" & Results.Symbol & "' Future '" & sBadContractName & "' with open position and/or open options positions already exist in DB"
    ElseIf nId = -3 Then
        AddToLog "Future Data save error: '" & Results.Symbol & "' Future '" & sBadContractName & "' with orders and/or options orders already exist in DB"
    ElseIf nId <= 0 Then
        AddToLog "Future Data save error: '" & Results.Symbol & "'"
    Else
        If m_nUndID <> nId Then
            m_nUndID = nId
            frmMain.RefreshMarket
        End If
    End If

    Exit Sub
EH:
    AddToLog "Future Data save error: '" & Results.Symbol & "' info: " & Err.Description
End Sub

Private Sub m_PriceProvider_OnFutureByRoot(Params As PRICEPROVIDERSLib.FutureRootParams, Results As PRICEPROVIDERSLib.FutureResultsEx, ByVal IsLastFutures As Boolean)
    Dim aFuture As clsFutureAtom, nId&, sBadContractName$
    On Error GoTo EH
    If m_bCancelFlag Then
        Exit Sub
    End If
    
    If Results.ExpirationDate > (Date - 30) Then
        
        If m_collFutureRoots(m_nCurrentFutureRootIdx).Symbol <> Params.FuturesRootSymbol Then
            Exit Sub
        End If
        
        m_collFutureRoots(m_nCurrentFutureRootIdx).HasFutures = True
        
        If Len(Results.Symbol) > 0 Then
            If pbOptions.Value < pbOptions.Max Then
                pbOptions.Value = pbOptions.Value + 1
            End If
                
            With Results
                nId = gDBW.usp_BoFuture_Import( _
                    Null, _
                    m_collFutureRoots(m_nCurrentFutureRootIdx).RootID, _
                    .ExpirationDate, _
                    .Symbol, _
                    .Description, _
                    .ContractSize, _
                    .OptionContractSize, _
                    .ClosePrice, _
                    sBadContractName, _
                    .MinimumTick, _
                    .UnitOfMeasure, _
                    .UnitOfMeasureCurrency, _
                    .ExpirationDate _
                    )
                    
                    If nId > 0 Then
                        Set aFuture = m_collFutures.Add(.Symbol)
                        aFuture.ID = nId
                        aFuture.Symbol = .Symbol
                        aFuture.Name = .Description
                    ElseIf nId = -2 Then
                        AddToLog "Future Data save error: '" & Results.Symbol & "' Future '" & sBadContractName & "' with open position already exist in DB"
                    ElseIf nId = -3 Then
                        AddToLog "Future Data save error: '" & Results.Symbol & "' Future '" & sBadContractName & "' with orders already exist in DB"
                    Else
                        AddToLog "Future Data save error: '" & Results.Symbol & "'"
                    End If
                    Set aFuture = Nothing
            End With
        End If
    Else
        AddToLog "Future Data Error: '" & Results.Symbol & "' info: Wrong Maturity Date " & CStr(Results.ExpirationDate)
    End If
    
    If IsLastFutures Then m_bGotLastFuture = True
    Exit Sub
EH:
    If IsLastFutures Then m_bGotLastFuture = True
    AddToLog "Future Data save error: '" & Results.Symbol & "' info: " & Err.Description
End Sub

Private Sub m_QuotesProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    
    m_bGotLastFuture = True
    If ErrorNumber = enNotConnected _
        Or ErrorNumber = enConnectionWasDisconnected Then m_bConnectBroken = True
    
    If m_bCancelFlag Then
        Exit Sub
    End If
    
    AddToLog "Price provider error for " & Request.Symbol & " price request: " & Description

End Sub

Private Function GetFutureOptionContractName(ByVal sFutureSymbol As String, ByRef aResults As PRICEPROVIDERSLib.FuturesOptionResultsEx) As String
    Dim sContractName$
    On Error Resume Next
    With aResults
        sContractName = sFutureSymbol
        sContractName = sContractName & " " & StdExpiryName(.ExpirationDate)
        sContractName = sContractName & " " & IIf(.OptionType = enCall, "Call", "Put")
        sContractName = sContractName & " " & gCmn.FmtDbl(.StrikePrice)
    End With
    GetFutureOptionContractName = sContractName
End Function


Private Sub m_PriceProvider_OnFuturesOption(Params As PRICEPROVIDERSLib.FuturesOptionParams, Results As PRICEPROVIDERSLib.FuturesOptionResultsEx, ByVal IsLastOption As Boolean)
    Dim aFuture As clsFutureAtom, sCurrentFuture$, nCurrentFutureId&, nId&, sBadContractName$, sFutureOptionContractName$
    Dim dExpiryOV As Date, dTradingClose As Date
    On Error GoTo EH
    If m_bCancelFlag Then
        Exit Sub
    End If
    
    If m_enUndType = GINT_FUTURES Then
        sCurrentFuture = m_sUndSymbol
        nCurrentFutureId = m_nUndID
    ElseIf m_enUndType = GINT_INDEXES Or m_enUndType = GINT_COMMODITIES Then
        sCurrentFuture = m_collFutures(m_nCurrentFutureIdx).Symbol
        nCurrentFutureId = m_collFutures(m_nCurrentFutureIdx).ID
    Else
        Exit Sub
    End If
        
    If sCurrentFuture <> Params.FutureSymbol Then
        Exit Sub
    End If
    
    m_bHasFutureOptions = True
    m_nFutureOptionsCount = m_nFutureOptionsCount + 1
    If pbOptions.Value < pbOptions.Max Then
           pbOptions.Value = pbOptions.Value + 1
    End If
                    
    sFutureOptionContractName = GetFutureOptionContractName(sCurrentFuture, Results)
    With Results
        dTradingClose = DateSerial(1900, 1, 1)
        If m_enUndType = GINT_COMMODITIES Then
                dTradingClose = dTradingClose + "14:00:00"
        Else:   dTradingClose = dTradingClose + "16:15:00" 'GINT_STOCKS
'        else if m_enUndType = GINT_INTEREST Then dTradingClose = DateSerial(1900, 1, 1) + "15:00:00"
        End If

        dExpiryOV = .LastTradingDate
        Select Case m_enUndType
            Case GINT_COMMODITIES
                dExpiryOV = dExpiryOV + "14:00:00"
            Case GINT_INDEXES, GINT_FUTURES
                dExpiryOV = dExpiryOV + "16:15:00"
'           Case GINT_INTEREST
'                dExpiryOV = dExpiryOV + "13:00:00"
            Case Else
                AddToLog "Wrong Underlying Type: '" & CStr(m_enUndType) & "'."
        End Select
        
        nId = gDBW.usp_EodFutureOption_Import(nCurrentFutureId, _
            .StrikePrice, _
            .ExpirationDate, _
            dExpiryOV, _
            dTradingClose, _
            (.OptionType = enCall), _
            .Series, _
            sFutureOptionContractName, _
            .ClosePrice, _
            sBadContractName)
                
            If nId = -2 Then
                AddToLog "Future Option Data save error: '" & Results.Series & "' Future option '" & sBadContractName & "' with position already exist in DB"
            ElseIf nId = -3 Then
                AddToLog "Future Option Data save error: '" & Results.Series & "' Future option '" & sBadContractName & "' with orders already exist in DB"
            ElseIf nId <= 0 Then
                AddToLog "Future Option Data save error: '" & Results.Series & "'"
            End If
                
    End With
        
    If IsLastOption Then
        m_bGotLastFutureOption = True
        AddToLog Params.FutureSymbol & " " & CStr(pbOptions.Value) & " future options were loaded"
    End If
    
    Exit Sub
EH:
    AddToLog "Future Option Data save error: '" & Results.Series & "' info: " & Err.Description
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_PriceProvider_OnOption(Params As PRICEPROVIDERSLib.OptionParams, Results As PRICEPROVIDERSLib.OptionResultsEx, ByVal IsLastOption As Boolean)
    Dim sOptSymb$, bIsCall As Boolean, fStrike#, dExpiry As Date, dExpiryOV As Date, dTradingClose As Date, LotSize&
    Dim sXMLString$

    On Error GoTo EH
    If m_bCancelFlag Then
        Exit Sub
    End If

    If m_sUndSymbol <> Params.UnderlyingSymbol Then
        Exit Sub
    End If
    
    If IsLastOption Then
        m_bGotLastOption = True
    End If
        
    m_bHasOptions = True
    m_nOptionsCount = m_nOptionsCount + 1
    If pbOptions.Value < pbOptions.Max Then
        pbOptions.Value = pbOptions.Value + 1
    Else
        pbOptions.Max = (pbOptions.Max * 2) + 1
        pbOptions.Value = pbOptions.Value + 1
    End If
     
    With Results
        sOptSymb = UCase(Trim(Replace(.Series, " ", "")))
        bIsCall = (.OptionType = PRICEPROVIDERSLib.OptionTypeEnum.enCall)
        fStrike = .StrikePrice
        dExpiry = .ExpirationDate
        LotSize = .LotSize
        
        dTradingClose = DateSerial(1900, 1, 1)
        If m_enUndType = GINT_INDEXES Then
                dTradingClose = dTradingClose + "16:15:00"
        Else:   dTradingClose = dTradingClose + "16:00:00" 'GINT_STOCKS
        End If

        Select Case .ExpirationType
            Case 0 ' Standard expiry on 3-rd Saturday
                Select Case .SettlementType
                    Case enSTAM
                        dExpiryOV = dExpiry - 1 + "09:30:00"
                    Case enSTPM
                        dExpiryOV = dExpiry - 1 + "16:15:00"
                    Case enSTUndefined
                        dExpiryOV = dExpiry - 1 + "16:00:00"
                End Select
            Case 1, 2, 4, 5, 6 ' Weekly options below on corresponding week and 6 - Vola index option
                If .SettlementType = enSTAM Then
                        dExpiryOV = dExpiry + "09:30:00"
                Else:   dExpiryOV = dExpiry + "16:15:00"
                End If
            Case 7 ' Quarterly option
                dExpiryOV = dExpiry + "16:00:00"
            Case Else
                AddToLog "Wrong ExpirationType: '" & CStr(.ExpirationType) & "'."
        End Select
    End With
    
   If fStrike > 0 And Len(sOptSymb) > 2 Then
        If Len(m_sOptionsImportText) = 0 Then m_sOptionsImportText = "<Import>"
        If Len(sOptSymb) > 2 And fStrike > 0# Then
            m_sOptionsImportText = m_sOptionsImportText + _
                "<Option Symbol = " & Chr(34) & sOptSymb & Chr(34) & _
                " IsCall = " & Chr(34) & IIf(bIsCall, "1", "0") & Chr(34) & _
                " Strike = " & Chr(34) & CStr(fStrike) & Chr(34) & _
                " Expiry = " & Chr(34) & CStr(dExpiry) & Chr(34) & _
                " ExpiryOV = " & Chr(34) & CStr(dExpiryOV) & Chr(34) & _
                " TradingClose = " & Chr(34) & CStr(dTradingClose) & Chr(34) & _
                " LotSize = " & Chr(34) & CStr(LotSize) & Chr(34) & _
                " />"
                m_nOptionsImportNum = m_nOptionsImportNum + 1
        End If
    End If

    If (IsLastOption Or m_nOptionsImportNum >= 20) And Len(m_sOptionsImportText) > 0 Then
        sXMLString = m_sOptionsImportText + "</Import>"
        m_sOptionsImportText = ""
        gDBW.usp_Option_Bulk_Import m_nUndID, sXMLString
        m_sOptionsImportText = ""
        m_nOptionsImportNum = 0
    End If
        
    Exit Sub
EH:
    AddToLog "Option Data save error: '" & Params.UnderlyingSymbol & "'  info: " & Err.Description
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_PriceProvider_OnStock(Params As PRICEPROVIDERSLib.StockParams, Results As PRICEPROVIDERSLib.StockResultsEx)
    Dim iConrtactID As Long
    Dim bIsStock As Boolean
    Dim vcContractName As String
    Dim iDivFreq As Long
    Dim fDivAmt As Double
    Dim dDivDate As Date

    On Error GoTo EH
    
    If m_sUndSymbol <> Params.Stock Then
        Exit Sub
    End If
    
    If m_bCancelFlag Then
        Exit Sub
    End If
            
    With Results
        '.Name
        '.Exchange
        Dim sFreq As String
        bIsStock = IIf(.StockType = enStock, True, False)
        vcContractName = Trim(.Description)
        iDivFreq = .DivFreq
        If iDivFreq = 0 Then
            fDivAmt = 0
            dDivDate = 0#
        Else
            If chkFilterDividends.Value <> 0 Then
                Dim dtAllowed As Date
                dtAllowed = Now
                Select Case iDivFreq
                    Case 1
                        dtAllowed = dtAllowed - 365
                        sFreq = "Annualy"
                    Case 2
                        dtAllowed = dtAllowed - 183
                        sFreq = "Semiannualy"
                    Case 4
                        dtAllowed = dtAllowed - 91
                        sFreq = "Quarterly"
                    Case 12
                        dtAllowed = dtAllowed - 31
                        sFreq = "Monthly"
                        
               End Select
               If dtAllowed > .DivLastDate Then
                    iDivFreq = 0
                    fDivAmt = 0
                    dDivDate = 0#
                    AddToLog Params.Stock & " Dividends were filtered out. The Next Div was at " & .DivLastDate & " Expected frequency:" & sFreq
    
               Else
                fDivAmt = .DivAmount
                dDivDate = .DivLastDate
               End If
            Else
                fDivAmt = .DivAmount
                dDivDate = .DivLastDate
            End If
        End If
    End With
        
    gDBW.usp_Stock_Import _
        m_nUndID, _
        bIsStock, _
        vcContractName, _
        iDivFreq, _
        fDivAmt, _
        dDivDate, _
        Results.Exchange
        
    m_bGotUndInfo = True
          
    Exit Sub
EH:
    m_bGotUndInfo = True
    AddToLog "Underlying Data save error: '" & Params.Stock & "' info: " & Err.Description
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportDefaultVolas()
    
    Dim sSymbol As String
    Dim sMsg As String, bImport As Boolean
    Dim rstUndNoIV As ADODB.Recordset ' Underlyings without IV
    Dim rsIVPoints As ADODB.Recordset
    Dim dStrike#, dtExpiration As Date, dVola#, pbStep As Single
    
    On Error GoTo Failed
        
    bImport = True
    
    m_nIVPointCount = 0
        
    lblOptions.Caption = "Import default IV surface"
    Me.Caption = "Import default IV surface"
        
'    On Error GoTo EH
'
'    Me.MousePointer = vbHourglass
'    DoEvents
'
'    If Not m_bReWriteVola Then ' If Not True
'       Set rstUndNoIV = gDBW.usp_CustomVolatilityPointIsExist_Get(DBWrite(m_sUndSymbol, ""))
'    End If
'    Me.MousePointer = vbDefault
'    DoEvents
'
'    If m_IVDataLoader Is Nothing Then
'        On Error Resume Next
'        Set m_IVDataLoader = New DHIVFeeder5.IVDataLoader
'        On Error GoTo EH
'        If m_IVDataLoader Is Nothing Then
'            Err.Raise vbObjectError, "IV Loader", "IV Loader component can not be started." & vbCrLf & "Repair application installation."
'            Exit Sub
'        End If
'        On Error Resume Next
'        If m_IVDataLoader.g_SymbolsCountLimit > 1000 Then
'            m_IVDataLoader.g_SymbolsCountLimit = 1000
'        End If
'        Set m_HttpFeeder = m_IVDataLoader.GetDHIVFeeder.GetHttpFeeder
'    End If
    
    On Error GoTo EH
        
    AddToLog "Default IV import started."
    AddToLog "Loading IV surface for " & m_sUndSymbol & " underlying. Please wait..."
              
'    With m_IVDataLoader
'        .IndexListClear
'        .StocksListClear
'    End With
'
'    Set m_collUndExpNoIV = New clsUndExpNoIVColl
'
'    If m_bLoadVola And Not m_bReWriteVola Then ' And Not True
'        Dim aUndExpNoIV As clsUndExpNoIVAtom
'
'        With rstUndNoIV
'            If .RecordCount > 0 Then .MoveFirst
'            Do While Not .EOF
'
'                If m_collUndExpNoIV(DBRead(!dtExpiry, Null)) Is Nothing Then
'                    Set aUndExpNoIV = m_collUndExpNoIV.Add(DBRead(!dtExpiry, Null))
'                    aUndExpNoIV.Expiration = DBRead(!dtExpiry, Null)
'                End If
'
'                .MoveNext
'            Loop
'        End With
'    End If
'
'Retry1:
'    If m_bLoadVola And Not m_bReWriteVola Then ' And Not True
'        If m_collUndExpNoIV.Count = 0 Then bImport = False
'    End If
'
'    If bImport Then
'        m_IVDataLoader.StocksListAddSymbol m_sUndSymbol
'    End If

    Me.MousePointer = vbHourglass
    DoEvents
    
    If m_enUndType = GINT_STOCKS Or m_enUndType = GINT_INDEXES Then
        ' Load Stock IV Surface
        Set rsIVPoints = gDBW.usp_DefaultIVSurfaceStock_Get(DBWrite(m_nUndID))
    
    ElseIf m_enUndType = GINT_COMMODITIES Then
        ' Load FutUnd IV Surface
        Set rsIVPoints = gDBW.usp_DefaultIVSurfaceFutureUnd_Get(DBWrite(m_nUndID))
    
    End If
    Me.MousePointer = vbDefault
    DoEvents
    
    pbOptions.Min = 0
    pbOptions.Max = 100
    pbOptions.Value = pbOptions.Min
    pbStep = 0
    
    With rsIVPoints
        If .RecordCount > 0 Then
            pbStep = pbOptions.Max / .RecordCount
            .MoveFirst
        End If
        Do While Not .EOF
        
            dStrike = DBRead(!fStrike)
            dVola = DBRead(!fIV)
            dtExpiration = DBRead(!dtExpDate)
            
            SymbolIVPointSave m_sUndSymbol, dStrike, dtExpiration, dVola, Date
            
            pbOptions.Value = Min(pbOptions.Value + pbStep, pbOptions.Max)
            .MoveNext
        Loop
    End With
                                                                
'    If Not m_IVDataLoader.LoadSymbolDefaultIVSurface Then
'        m_bCancelFlag = True
'        Err.Raise vbObjectError, "Default IV Import", "Task was canceled by user"
'    End If
        
Done:
    pbOptions.Value = pbOptions.Max
    sMsg = "Default IV import completed" & vbCrLf
    sMsg = sMsg & "Default IV surface loaded for  " & m_sUndSymbol & " underlying." & vbCrLf
    sMsg = sMsg & m_nIVPointCount & " IV Points saved."
    AddToLog sMsg
    
'    On Error Resume Next
'
'    If Not m_IVDataLoader Is Nothing Then
'        m_IVDataLoader.CancelTask
'    End If
    Exit Sub
EH:
    Dim Msg As String
    Msg = Err.Description
    
    Screen.MousePointer = vbDefault
    btnCancel.Caption = "Close"
    
    If m_bCancelFlag Then
        ShowMessage "Task was canceled by user"
    Else
        ShowMessage Msg
    End If
    
    AddToLog Msg

Failed:
    Screen.MousePointer = vbDefault
    GoTo Done
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_IVDataLoader_Progress(ByVal ProgressPercent As Long)
    On Error Resume Next
    pbOptions.Value = ProgressPercent
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub m_IVDataLoader_ProgressStart()
    On Error Resume Next
    pbOptions.Min = 0
    pbOptions.Max = 100
    pbOptions.Value = 0
End Sub


Private Sub LoadFutureRoots()
    Dim aRoot As clsFutureRootAtom, rs As ADODB.Recordset, sKey$
        
    Set rs = gDBW.usp_BoFutureRoot_Get(m_nUndID)
    m_nCurrentFutureRootIdx = 0
    With rs
        If .RecordCount > 0 Then
            If m_collFutureRoots Is Nothing Then
                Set m_collFutureRoots = New clsFutureRootColl
            End If
            m_collFutureRoots.Clear
            .MoveFirst
            Do While Not .EOF
                sKey = gCmn.ReadStr(.Fields("vcFutureRootSymbol"))
                Set aRoot = m_collFutureRoots.Add(sKey)
                aRoot.Symbol = sKey
                aRoot.RootID = gCmn.ReadLng(.Fields("iFutureRootID"))
                .MoveNext
            Loop
        End If
    End With
End Sub

Private Sub m_QuotesProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    Dim aFuture As clsFutureAtom, nId&, Price As Double
    On Error GoTo EH
    If m_bCancelFlag Then
        Exit Sub
    End If
    
    If m_collFuturesPointer(m_nCurrentFutureIdx).Symbol <> Params.Symbol Then
        Exit Sub
    End If
    
    m_bGotLastFuture = True
    
    If m_iPriceToLoad = PRICE_LAST Then
        Price = Results.LastPrice
    ElseIf m_iPriceToLoad = PRICE_YEST Then
        Price = Results.ClosePrice
    End If
        
    
    If IsBadValue(Price) Then
        AddToLog "The price for " & Params.Symbol & " is not in the range."
        Exit Sub
    End If
    
    'm_collFutureRoots(m_nCurrentFutureRootIdx).HasFutures = True
        
    'If pbOptions.Value < pbOptions.Max Then
    '    pbOptions.Value = pbOptions.Value + 1
    'End If
        
'    With Results
'        If m_bLoadFuturePrices Then
'            nId = gDBW.usp_Contract_ClosePrice_Save( _
'                m_collFuturesPointer(m_nCurrentFutureIdx).ID, _
'                price)
'        Else
'            nId = gDBW.usp_Contract_ClosePrice_Save( _
'                m_collFuturesPointer(m_nCurrentFutureIdx).ID, _
'                price)
'        End If
'    End With

    With Results
            nId = gDBW.usp_Contract_ClosePrice_Save( _
                m_collFuturesPointer(m_nCurrentFutureIdx).ID, _
                Price)
    End With
    Exit Sub
EH:
    AddToLog "Future price save error: '" & Params.Symbol & "' info: " & Err.Description
End Sub



Private Sub optPricesLast_Click()
    m_iPriceToLoad = PRICE_LAST
End Sub

Private Sub optPricesNone_Click()
    m_iPriceToLoad = PRICE_NONE
End Sub

Private Sub optPricesYest_Click()
    m_iPriceToLoad = PRICE_YEST
End Sub
