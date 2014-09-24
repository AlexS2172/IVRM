VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTntProcessor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Private Const MAX_TRADE_NO& = &HFFF&
'Private Const MAX_TRADE_NO_LEN& = 3
'Private Const TRADE_EXPIRY_PERIOD& = 90&
'
'Private Enum TntServerConnectionStateEnum
'    enTntScsConnecting = 0
'    enTntScsConnected = 1
'    enTntScsDisconnecting = 2
'    enTntScsDisconnected = 3
'End Enum
'
'Public Enum TntAccountCheckingResultEnum
'    enTntAcrSucceeded = 0
'    enTntAcrNotMatch = -1
'    enTntAcrNotSet = -2
'    enTntAcrNotLoggedAsTrader = -3
'    enTntAcrUnexpected = -4
'End Enum
'
'Public Event OnNewCard(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'Public Event OnUpdateCard(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'Public Event OnDeleteCard(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'Public Event OnNewEvent(ByRef aEvent As EtsGeneralLib.EtsEventAtom)
'Public Event OnClearEvents()
'Public Event OnConnectStateChange()
'
'Private WithEvents TntAdapter As TNTConnector.CMMHHTAdapter
'Private WithEvents tmrReconnectTimer As VB.Timer
'Private WithEvents tmrTntCardExpiryTimer As VB.Timer
'
'Private m_Card As New EtsMmGeneralLib.MmTntCardColl
'Private m_CardBySeq As New EtsMmGeneralLib.MmTntCardBySeqColl
'Private m_CardForExpiryCheck As New EtsMmGeneralLib.MmTntCardColl
'Private m_Evt As New EtsGeneralLib.EtsEventColl
'Private m_nEventID&
'
'Private m_CurTrader As EtsGeneralLib.EtsTraderAtom
'Private m_ConnSettings As TNTConnector.ConnectionSettings
'Private m_NewConnSettings As TNTConnector.ConnectionSettings
'
'Private m_bIsReconnectNow As Boolean
'Private m_hDisconnectTimer&
'Private m_bIsConnected As Boolean
'Private m_bShutDown As Boolean
'Private m_enConnectState As TntServerConnectionStateEnum
'
'Private m_bUseTntMessaging As Boolean
'
'Private Sub Class_Initialize()
'    On Error Resume Next
'    m_bIsConnected = False
'    m_bShutDown = False
'    m_bIsReconnectNow = False
'    m_hDisconnectTimer = 0&
'    m_enConnectState = enTntScsDisconnected
'
'    m_bUseTntMessaging = (GetLongKeyValueEx(APP_REG_KEY & "TNT Messages", "UseTntMessaging", 1) <> 0)
'End Sub
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    m_bIsConnected = False
'
'    Set tmrTntCardExpiryTimer = Nothing
'
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "UseTntMessaging", IIf(m_bUseTntMessaging, 1, 0)
'
'    If m_hDisconnectTimer <> 0 Then CloseHandle m_hDisconnectTimer
'    Set tmrReconnectTimer = Nothing
'    Set TntAdapter = Nothing
'End Sub
'
'Public Sub InitAdapter()
'    On Error GoTo EH
'
'    m_bShutDown = False
'
'    If GetLongKeyValueEx(APP_REG_KEY & "TNT Messages", "FirstStart", 1) <> 0 Then
'
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "FirstStart", 0
'        If gCmn.MyMsgBox(g_frmOwner, "This version of ETS enables to submit trades messages" & _
'                                    " to Pacific Options Exchange Trading System (POETS). " & vbCrLf & _
'                                    "Do you want to use this functionality?" & _
'                                    " (You can also enable or disable it in Parameters window.)", _
'                    vbYesNo + vbQuestion) = vbNo Then
'            m_bUseTntMessaging = False
'        Else
'            m_bUseTntMessaging = True
'        End If
'    End If
'
'    LoadConnSettings
'
'    Set m_CurTrader = Nothing
'    If g_CurTraderID <> 0 Then Set m_CurTrader = g_Trader(g_CurTraderID)
'
'    AddEvent IIf(m_bUseTntMessaging, enEvtInfo, enEvtWarning), False, _
'        "Trades Messages functionality is " & IIf(m_bUseTntMessaging, "enabled.", "disabled.")
'
'    Exit Sub
'EH:
'    AddEvent enEvtError, True, "Fail to initialize trades messages server connector." & vbCrLf & Err.Description
'End Sub
'
'Public Sub Start()
'    On Error Resume Next
'    Set tmrReconnectTimer = frmMain.tmrTntProcessorReconnectHelper
'    Set tmrTntCardExpiryTimer = frmMain.tmrTntProcessorHelper
'
'    tmrTntCardExpiryTimer.Enabled = True
'    If m_bUseTntMessaging Then Connect
'End Sub
'
'Private Sub CardsDeleteOld()
'    On Error Resume Next
'    Err.Clear
'    gDBW.usp_TntCardOld_Del m_CurTrader.ID
'    If Err.Number <> 0 Then
'        AddEvent enEvtError, False, "Fail to delete old trade messages: an error was occured while deleting old trade message from database: " & vbCrLf & Err.Description
'    End If
'End Sub
'
'Public Sub LoadCards(Optional ByRef lblProgress As VB.Label = Nothing)
'    On Error GoTo EH
'    Dim rsCrd As ADODB.Recordset, bShowProgress As Boolean, nTotal&, nDone&
'    Dim aCrd As EtsMmGeneralLib.MmTntCardAtom, nCardID&
'
'    If Not m_CurTrader Is Nothing Then
'        bShowProgress = Not lblProgress Is Nothing
'
'        If bShowProgress Then
'            lblProgress.Caption = "Loading trades messages data... 0% complete."
'            lblProgress.Refresh
'        End If
'
'        CardsDeleteOld
'
'        m_CardForExpiryCheck.Clear
'        m_Card.Clear
'
'        Set rsCrd = gDBW.usp_TntCardByTrader_Get(m_CurTrader.ID)
'        nDone = 0&
'        nTotal = rsCrd.RecordCount
'
'        While Not rsCrd.EOF
'            If m_CurTrader.ID = ReadLng(rsCrd!iTraderID) Then
'                nCardID = ReadLng(rsCrd!iCardID)
'                If nCardID < MAX_TRADE_NO Then
'
'                    Set aCrd = New EtsMmGeneralLib.MmTntCardAtom
'                    aCrd.ID = nCardID
'                    aCrd.TradeDate = ReadDate(rsCrd!dtTradeDate)
'                    aCrd.TradeID = GetTradeID(nCardID)
'
'                    aCrd.IsBuy = (ReadByte(rsCrd!tiIsBuy) <> 0)
'                    aCrd.Qty = ReadLng(rsCrd!iQty)
'                    aCrd.Price = ReadDbl(rsCrd!fPrice)
'                    aCrd.PoeID = ReadStr(rsCrd!vcPoeID)
'                    aCrd.Status = ReadByte(rsCrd!tiStatus)
'                    aCrd.Account = ReadStr(rsCrd!vcAccount)
'
'                    aCrd.ExecutingFirm = ReadStr(rsCrd!vcExecutingFirm)
'                    aCrd.CMTA = ReadStr(rsCrd!vcCMTA)
'                    aCrd.Quantifier = ReadStr(rsCrd!vcQuantifier)
'                    aCrd.ContraFirm = ReadStr(rsCrd!vcContraFirm)
'                    aCrd.SpreadIndicator = ReadStr(rsCrd!vcSpreadIndicator)
'                    aCrd.OptionalData = ReadStr(rsCrd!vcOptionalData)
'
'                    Set aCrd.CntPty = g_TntCntPty(ReadLng(rsCrd!iCptID))
'
'                    Set aCrd.Und = g_UnderlyingAll(ReadLng(rsCrd!iUndID))
'                    If aCrd.Und Is Nothing Then
'                        Set aCrd.Und = New EtsGeneralLib.UndAtom
'                        aCrd.Und.ID = 0
'                        aCrd.Und.Symbol = "???"
'                    End If
'
'                    Set aCrd.OptRoot = aCrd.Und.Roots(ReadLng(rsCrd!iRootID))
'                    If aCrd.OptRoot Is Nothing Then
'                        Set aCrd.OptRoot = New EtsGeneralLib.EtsOptRootAtom
'                        aCrd.OptRoot.ID = 0
'                        aCrd.OptRoot.Name = ReadStr(rsCrd!vcRootSymbol)
'                        aCrd.OptRoot.LotSize = 100
'                        aCrd.OptRoot.UndID = aCrd.Und.ID
'                    End If
'
'                    Set aCrd.Opt = New EtsGeneralLib.EtsOptAtom
'                    aCrd.Opt.ID = ReadLng(rsCrd!iOptID)
'                    aCrd.Opt.Symbol = ReadStr(rsCrd!vcOptSymbol)
'                    aCrd.Opt.Expiry = ReadDate(rsCrd!dtOptExpiry)
'                    aCrd.Opt.OptType = IIf(ReadByte(rsCrd!tiOptIsCall) <> 0, enOtCall, enOtPut)
'                    aCrd.Opt.Strike = ReadDbl(rsCrd!fOptStrike)
'                    aCrd.Opt.RootID = aCrd.OptRoot.ID
'
'                    MakeCardMsgAndDesc aCrd
'
'                    m_Card.Add aCrd.TradeID, aCrd.TradeDate, aCrd
'
'                    If aCrd.Status = enTcsInNew Then
'                        If DateDiff("s", aCrd.TradeDate, Now) < TRADE_EXPIRY_PERIOD Then
'                            m_CardForExpiryCheck.Add aCrd.TradeID, aCrd.TradeDate, aCrd
'                        Else
'                            aCrd.IsExpired = True
'                        End If
'                    End If
'
'                    Set aCrd = Nothing
'                End If
'            End If
'
'            If bShowProgress Then
'                nDone = nDone + 1
'                lblProgress.Caption = "Loading trades messages data... " & CStr(nDone * 100 \ nTotal) & "% complete."
'                DoEvents
'            End If
'
'            rsCrd.MoveNext
'        Wend
'        Set rsCrd = Nothing
'
'        If bShowProgress Then
'            lblProgress.Caption = "Loading trades messages data... 100% complete."
'            lblProgress.Refresh
'        End If
'
'    End If
'
'    Exit Sub
'EH:
'    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
'    sDescription = "Trades Messages Processor: Fail to load trades messages. " & Err.Description
'    nHelpContext = Err.HelpContext
'    sHelpFile = Err.HelpFile
'    nNumber = Err.Number
'    sSource = Err.Source
'
'    Set rsCrd = Nothing
'    m_CardForExpiryCheck.Clear
'    m_Card.Clear
'    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'End Sub
'
'Public Property Get CurTraderAccount() As String
'    On Error Resume Next
'    If Not m_CurTrader Is Nothing Then
'        CurTraderAccount = m_CurTrader.TntAccount
'    Else
'        CurTraderAccount = ""
'    End If
'End Property
'
'Public Property Get ConnSettings() As TNTConnector.ConnectionSettings
'    On Error Resume Next
'    ConnSettings = m_ConnSettings
'End Property
'
'Public Sub SetNewConnSettings(ByRef aNewValue As TNTConnector.ConnectionSettings)
'    On Error Resume Next
'    If Not m_CurTrader Is Nothing Then
'        With m_NewConnSettings
'            .ID = m_CurTrader.TntAccount
'            .Password = Trim$(aNewValue.Password)
'            .PrimaryIP = Trim$(aNewValue.PrimaryIP)
'            .SecondaryIP = Trim$(aNewValue.SecondaryIP)
'            .Port = aNewValue.Port
'        End With
'    End If
'End Sub
'
'Private Sub LoadConnSettings()
'    On Error Resume Next
'    With m_ConnSettings
'        .ID = UCase$(Trim$(GetStrKeyValueEx(APP_REG_KEY & "TNT Messages", "TraderID", "")))
'        .Password = Trim$(GetStrKeyValueEx(APP_REG_KEY & "TNT Messages", "Password", ""))
'        .PrimaryIP = Trim$(GetStrKeyValueEx(APP_REG_KEY & "TNT Messages", "PrimaryIP", ""))
'        .SecondaryIP = Trim$(GetStrKeyValueEx(APP_REG_KEY & "TNT Messages", "SecondaryIP", ""))
'        .Port = GetLongKeyValueEx(APP_REG_KEY & "TNT Messages", "Port", 0)
'    End With
'End Sub
'
'Private Sub SaveConnSettings()
'    On Error Resume Next
'    With m_ConnSettings
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "TraderID", .ID
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "Password", .Password
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "PrimaryIP", .PrimaryIP
'        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "SecondaryIP", .SecondaryIP
'        SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TNT Messages", "Port", .Port
'    End With
'End Sub
'
'Public Property Get NeedToReconnect() As Boolean
'    On Error Resume Next
'    NeedToReconnect = m_NewConnSettings.Password <> m_ConnSettings.Password _
'                    Or m_NewConnSettings.PrimaryIP <> m_ConnSettings.PrimaryIP _
'                    Or m_NewConnSettings.SecondaryIP <> m_ConnSettings.SecondaryIP _
'                    Or m_NewConnSettings.Port <> m_ConnSettings.Port
'End Property
'
'Private Function GetTradeID$(ByVal nTradeID&)
'    On Error Resume Next
'    Dim sNo$
'    sNo = Hex$(nTradeID)
'    GetTradeID = m_CurTrader.TntAccount & String$(MAX_TRADE_NO_LEN - Len(sNo), "0") & sNo
'End Function
'
'Public Function CheckCurrentTraderAccount(ByRef sReasonDesc$) As TntAccountCheckingResultEnum
'    On Error GoTo EH
'
'    CheckCurrentTraderAccount = enTntAcrSucceeded
'
'    If Not m_CurTrader Is Nothing Then
'
'        If Len(m_ConnSettings.ID) > 0 Then
'
'            If m_CurTrader.TntAccount <> UCase$(m_ConnSettings.ID) Then
'                sReasonDesc = "An active trader was changed." & vbCrLf & _
'                              "Please enter connection settings for '" & m_CurTrader.TntAccount & "' to access" & _
'                              " to POETS if you want to use Trades Messages functionality." & vbCrLf & _
'                              "The connection settings can be adjusted" & _
'                              " in the Connections Settings window (Options->Connections...)."
'
'                m_ConnSettings.ID = m_CurTrader.TntAccount
'                m_ConnSettings.Password = ""
'                CheckCurrentTraderAccount = enTntAcrNotMatch
'            End If
'
'        Else
'            sReasonDesc = "Please enter connection settings for access" & _
'                          " to POETS if you want to use Trades Messages functionality." & vbCrLf & _
'                          "The connection settings can be adjusted" & _
'                          " in the Connections Settings window (Options->Connections...)."
'
'            m_ConnSettings.ID = m_CurTrader.TntAccount
'            CheckCurrentTraderAccount = enTntAcrNotSet
'        End If
'
'    Else
'        sReasonDesc = "Trades Messages functionality is restricted under <All Traders> login." & vbCrLf & _
'                      "Please start the application for an individual trader."
'
'        CheckCurrentTraderAccount = enTntAcrNotLoggedAsTrader
'    End If
'
'    Exit Function
'EH:
'    CheckCurrentTraderAccount = enTntAcrUnexpected
'    sReasonDesc = "Fail to check trades messages connection settings: " & Err.Description
'End Function
'
'Public Sub ReConnect()
'    On Error Resume Next
'
'    m_ConnSettings = m_NewConnSettings
'
'    SaveConnSettings
'
'    If m_bIsConnected Then
'        m_bIsReconnectNow = True
'        m_bIsReconnectNow = Disconnect(False)
'    Else
'        tmrReconnectTimer.Enabled = True
'    End If
'End Sub
'
'Private Sub tmrReconnectTimer_Timer()
'    On Error Resume Next
'    tmrReconnectTimer.Enabled = False
'
'    Connect
'End Sub
'
'Public Sub Connect()
'    On Error GoTo EH
'    Dim sReasonDesc$, enResult As TntAccountCheckingResultEnum
'
'    If TntAdapter Is Nothing Then
'        Set TntAdapter = New TNTConnector.CMMHHTAdapter
'        AddEvent enEvtInfo, False, "Trades messages processing initialized."
'    End If
'
'    If Not m_bIsConnected Then
'        enResult = CheckCurrentTraderAccount(sReasonDesc)
'        Select Case enResult
'            Case enTntAcrSucceeded
'                AddEvent enEvtInfo, False, "Logging on to trades messages server..."
'                TntAdapter.Logon m_ConnSettings
'
'            Case enTntAcrNotMatch, enTntAcrNotSet, enTntAcrNotLoggedAsTrader
'                AddEvent enEvtWarning, True, sReasonDesc
'
'            Case enTntAcrUnexpected
'                AddEvent enEvtError, True, sReasonDesc
'        End Select
'    End If
'
'    Exit Sub
'EH:
'    m_bIsConnected = False
'    AddEvent enEvtError, True, "Fail to connect to trades messages server: " & vbCrLf & Err.Description
'End Sub
'
'Public Sub ShutDown()
'    On Error Resume Next
'    m_bShutDown = True
'    g_TntProcessor.Disconnect True, False
'
'    m_Card.Clear
'    m_CardBySeq.Clear
'    m_CardForExpiryCheck.Clear
'    m_Evt.Clear
'End Sub
'
'Public Function Disconnect(ByVal bAsync As Boolean, _
'                        Optional ByVal bRaiseMsgBoxErrors As Boolean = True) As Boolean
'    On Error GoTo EH
'
'    Disconnect = False
'
'    If TntAdapter Is Nothing Then
'        AddEvent enEvtError, bRaiseMsgBoxErrors, "Trades messages server connector is not initialized."
'        Exit Function
'    End If
'
'    If m_bIsConnected Then
'        AddEvent enEvtInfo, False, "Logging off from trades messages server..."
'
'        If bAsync Then m_hDisconnectTimer = CreateWaitableTimer(ByVal 0&, 1, ByVal 0&)
'
'        TntAdapter.Logoff
'
'        If m_hDisconnectTimer <> 0 Then
'            Dim liDueTime As LARGE_INTEGER
'            liDueTime.LowPart = -10000& * 2000&
'            liDueTime.HighPart = -1&
'
'            If SetWaitableTimer(m_hDisconnectTimer, liDueTime, 0&, 0&, 0&, 0&) <> 0 Then
'
'                While m_bIsConnected And MsgWaitForMultipleObjects(1, m_hDisconnectTimer, 0, INFINITE, QS_ALLINPUT) <> WAIT_OBJECT_0
'                    Sleep 0
'                    DoEvents
'                Wend
'            End If
'
'            CloseHandle m_hDisconnectTimer
'        End If
'
'        Disconnect = True
'    End If
'
'    Exit Function
'EH:
'    AddEvent enEvtError, bRaiseMsgBoxErrors, "Fail to disconnect from trades messages server: " & vbCrLf & Err.Description
'    Disconnect = False
'End Function
'
'Public Sub NewEvent(ByVal enEvtType As EtsEventTypeEnum, ByRef sEvtMessage As String)
'    On Error Resume Next
'    AddEvent enEvtType, False, sEvtMessage
'End Sub
'
'Private Sub AddEvent(ByVal enEvtType As EtsEventTypeEnum, ByVal bShowMsgBox As Boolean, ByRef sEvtMessage As String)
'    On Error Resume Next
'    Dim aEvent As New EtsGeneralLib.EtsEventAtom
'
'    m_nEventID = m_nEventID + 1
'    aEvent.EvtID = m_nEventID
'    aEvent.EvtTime = Now()
'    aEvent.EvtType = enEvtType
'    aEvent.EvtMessage = sEvtMessage 'Replace$(sEvtMessage, vbCrLf, " ")
'
'    Err.Clear
'    m_Evt.Add aEvent.EvtID, aEvent.EvtTime, aEvent
'    If Err.Number = 0 Then
'        RaiseEvent OnNewEvent(aEvent)
'    Else
'        LogEvent EVENT_ERROR, "Trades Messages processor: Fail to log new order event with message '" & sEvtMessage & "'"
'    End If
'
'    If bShowMsgBox Then
'        ShowMsgBox g_frmOwner, IIf(enEvtType = enEvtError, vbCritical, _
'                IIf(enEvtType = enEvtWarning, vbExclamation, vbInformation)), sEvtMessage
'    End If
'End Sub
'
'Public Sub ClearEvents()
'    On Error Resume Next
'    m_Evt.Clear
'    RaiseEvent OnClearEvents
'End Sub
'
'Public Property Get Events() As EtsGeneralLib.EtsEventColl
'    On Error Resume Next
'    Set Events = m_Evt
'End Property
'
'Public Property Get Cards() As EtsMmGeneralLib.MmTntCardColl
'    On Error Resume Next
'    Set Cards = m_Card
'End Property
'
'Private Function GetTntErrorDescString(saErrors() As TNTConnector.ErrorDescription) As String
'    On Error Resume Next
'    Dim nLBound&, nUBound&, i&
'    Dim sDesc$, sMsg$
'
'    nLBound = 0
'    nLBound = LBound(saErrors)
'
'    nUBound = -1
'    nUBound = UBound(saErrors)
'
'    If nUBound >= nLBound Then
'        For i = nLBound To nUBound
'            sMsg = Trim$(saErrors(i).Description)
'            If Len(sMsg) > 0 Then sDesc = sDesc & vbCrLf & sMsg & ". "
'        Next
'    Else
'        sDesc = "No error description available."
'    End If
'
'    GetTntErrorDescString = sDesc
'End Function
'
'Private Sub CardCntPtyApply(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error GoTo 0
'    Dim nCntPtyID&, sCntPtyName$
'
'    If Not aCard.CntPty Is Nothing Then
'        If aCard.CntPty.ID = 0 Then
'            sCntPtyName = UCase$(Trim$(aCard.CntPty.Name))
'
'            If Len(sCntPtyName) > 0 Then
'
'                Set aCard.CntPty = Nothing
'                Set aCard.CntPty = g_TntCntPtyByName(sCntPtyName)
'
'                If aCard.CntPty Is Nothing Then
'
'                    nCntPtyID = gDBW.usp_TntCounterParty_Save(0, sCntPtyName)
'                    Set aCard.CntPty = g_TntCntPty(nCntPtyID)
'
'                    If aCard.CntPty Is Nothing Then
'
'                        Set aCard.CntPty = New EtsMmGeneralLib.MmTntCounterPartyAtom
'                        aCard.CntPty.ID = nCntPtyID
'                        aCard.CntPty.Name = sCntPtyName
'                        aCard.CntPty.Desc = sCntPtyName
'
'                        g_TntCntPty.Add aCard.CntPty.ID, aCard.CntPty.Name, aCard.CntPty
'                        g_TntCntPtyByName.Add aCard.CntPty.Name, aCard.CntPty
'
'                        g_Params.FireNewTntCntPtyAdded nCntPtyID
'                    End If
'                End If
'            Else
'                Err.Raise vbObjectError + 512, "Trades Messages Processor", "Empty counterparty name."
'            End If
'        End If
'    Else
'        Err.Raise vbObjectError + 512, "Trades Messages Processor", "Empty counterparty name."
'    End If
'End Sub
'
'Private Sub CardSaveStatusToDB(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom, ByVal bShowError As Boolean)
'    On Error Resume Next
'    Err.Clear
'    gDBW.usp_TntCard_Update m_CurTrader.ID, aCard.ID, aCard.Status
'    If Err.Number <> 0 And bShowError Then
'        AddEvent enEvtError, False, "Fail to save trade message status: an error was occured while saving trade message status to database: " & vbCrLf & Err.Description
'    End If
'End Sub
'
'Private Function CardDeleteFromDB(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom, ByVal bShowError As Boolean) As Boolean
'    On Error Resume Next
'    Err.Clear
'    gDBW.usp_TntCard_Del m_CurTrader.ID, aCard.ID
'    If Err.Number = 0 Then
'        CardDeleteFromDB = True
'    Else
'        If bShowError Then AddEvent enEvtError, False, "Fail to delete trade message: an error was occured while deleting trade message to database: " & vbCrLf & Err.Description
'        CardDeleteFromDB = False
'    End If
'End Function
'
'Private Function CardSaveNewOutToDB(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom) As Boolean
'    On Error GoTo EH
'    Dim rs As ADODB.Recordset
'
'    CardSaveNewOutToDB = False
'    With aCard
'        .Status = enTcsOutPendingExecute
'        .TradeDate = Now
'
'        Set rs = gDBW.usp_TntCardOut_New(m_CurTrader.ID, .CntPty.ID, .TradeDate, IIf(.IsBuy, 1, 0), _
'                                        .Qty, .Price, .PoeID, .Status, .Account, .ExecutingFirm, _
'                                        .CMTA, .Quantifier, .ContraFirm, .SpreadIndicator, .OptionalData, _
'                                        .Und.ID, .OptRoot.ID, .OptRoot.Name, .Opt.ID, .Opt.Symbol, _
'                                        .Opt.Expiry, .Opt.Strike, IIf(.Opt.OptType = enOtCall, 1, 0), _
'                                        MAX_TRADE_NO)
'
'        If Not rs.EOF Then
'            .ID = ReadLng(rs!iCardID)
'            If .ID > 0 Then
'                .TradeID = GetTradeID(.ID)
'                CardSaveNewOutToDB = True
'            Else
'                AddEvent enEvtError, False, "Fail to submit new trade message: " & _
'                        vbCrLf & "No new unique trade message identifier is available - trade message number exceeds limit of " & MAX_TRADE_NO & "."
'            End If
'        Else
'            AddEvent enEvtError, False, "Fail to submit new trade message: unexpected error was occured while saving trade message to database."
'        End If
'    End With
'    Set rs = Nothing
'
'    Exit Function
'EH:
'    AddEvent enEvtError, False, "Fail to submit new trade message: an error was occured while saving trade message to database: " & vbCrLf & Err.Description
'    Set rs = Nothing
'End Function
'
'Public Sub CardNew(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    If TntAdapter Is Nothing Then
'        AddEvent enEvtError, True, "Fail to submit new trade message: Trades messages server connector is not initialized."
'        Exit Sub
'    End If
'
'    If Not m_bIsConnected Then
'        AddEvent enEvtError, True, "Fail to submit new trade message: Not logged on to trades messages server."
'        Exit Sub
'    End If
'
'    Err.Clear
'    CardCntPtyApply aCard
'    If Err.Number = 0 Then
'        If CardSaveNewOutToDB(aCard) Then
'            Dim aTntData As TNTConnector.TNTData
'
'            MakeNewCardForPub aCard, aTntData
'            MakeCardMsgAndDesc aCard
'
'            Err.Clear
'            aCard.SeqNum = TntAdapter.TradeAdd(aTntData).SequenceNo
'            If Err.Number = 0 Then
'                m_Card.Add aCard.TradeID, aCard.TradeDate, aCard
'                m_CardBySeq.Add aCard.SeqNum, aCard.TradeDate, aCard
'
'                AddEvent enEvtInfo, False, "New trade message was submitted: " & aCard.Desc
'
'                RaiseEvent OnNewCard(aCard)
'            Else
'                AddEvent enEvtError, False, "Fail to submit new trade message: Trades messages server connector error: " & vbCrLf & Err.Description
'                gDBW.usp_TntCard_Del m_CurTrader.ID, aCard.ID
'            End If
'        End If
'    Else
'        AddEvent enEvtError, False, "Fail to submit new trade message: an error was occured while saving trade message counterparty: " & vbCrLf & Err.Description
'    End If
'End Sub
'
'Public Sub CardAccept(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    If m_Card(aCard.TradeID) Is Nothing Then Exit Sub
'
'    If TntAdapter Is Nothing Then
'        AddEvent enEvtError, False, "Fail to accept/ACK trade message: Trades messages server connector is not initialized."
'        Exit Sub
'    End If
'
'    If Not m_bIsConnected Then
'        AddEvent enEvtError, False, "Fail to accept/ACK trade message: Not logged on to trades messages server."
'        Exit Sub
'    End If
'
'    If aCard.Status = enTcsInNew Or aCard.Status = enTcsInFailed Then
'        Dim aTntData As TNTConnector.TNTData
'
'        Err.Clear
'        gDBW.usp_TntCard_Update m_CurTrader.ID, aCard.ID, enTcsInPendingAccept
'        If Err.Number = 0 Then
'            MakeCardAcceptForPub aCard, aTntData
'
'            Err.Clear
'            aCard.SeqNum = TntAdapter.TradeAdd(aTntData).SequenceNo
'            If Err.Number = 0 Then
'                aCard.Status = enTcsInPendingAccept
'
'                m_CardBySeq.Add aCard.SeqNum, aCard.TradeDate, aCard
'
'                AddEvent enEvtInfo, False, "Trade message accept/ACK was submitted: " & aCard.Desc
'
'                RaiseEvent OnUpdateCard(aCard)
'            Else
'                AddEvent enEvtError, False, "Fail to accept/ACK trade message: Trades messages server connector error: " & vbCrLf & Err.Description
'                gDBW.usp_TntCard_Update m_CurTrader.ID, aCard.ID, aCard.Status
'            End If
'        Else
'            AddEvent enEvtError, False, "Fail to accept/ACK trade message: an error was occured while updating trade message status in database: " & vbCrLf & Err.Description
'        End If
'    Else
'        AddEvent enEvtError, False, "Fail to accept/ACK trade message: can accept new income trades messages only. " & aCard.Desc
'    End If
'End Sub
'
'Public Sub CardReject(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    If m_Card(aCard.TradeID) Is Nothing Then Exit Sub
'
'    If aCard.Status = enTcsInNew Or aCard.Status = enTcsInFailed Then
'        Err.Clear
'        gDBW.usp_TntCard_Update m_CurTrader.ID, aCard.ID, enTcsInRejected
'        If Err.Number = 0 Then
'            aCard.Status = enTcsInRejected
'            aCard.IsExpired = False
'
'            m_CardForExpiryCheck.Remove aCard.TradeID
'
'            AddEvent enEvtInfo, False, "Trade message was rejected: " & aCard.Desc
'
'            RaiseEvent OnUpdateCard(aCard)
'        Else
'            AddEvent enEvtError, False, "Fail to reject trade message: an error was occured while updating trade message status in database: " & vbCrLf & Err.Description
'        End If
'    Else
'        AddEvent enEvtError, False, "Fail to reject trade message " & aCard.Desc & ": can reject new income trades messages only."
'    End If
'End Sub
'
'Public Sub CardDelete(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    If m_Card(aCard.TradeID) Is Nothing Then Exit Sub
'
'    Err.Clear
'    gDBW.usp_TntCard_Del m_CurTrader.ID, aCard.ID
'    If Err.Number = 0 Then
'
'        m_Card.Remove aCard.TradeID
'        aCard.IsExpired = False
'        m_CardForExpiryCheck.Remove aCard.TradeID
'
'        AddEvent enEvtInfo, False, "Trade message was deleted: " & aCard.Desc
'
'        RaiseEvent OnDeleteCard(aCard)
'    Else
'        AddEvent enEvtError, False, "Fail to delete trade message: an error was occured while deleting trade message from database: " & vbCrLf & Err.Description
'    End If
'End Sub
'
'Private Sub tmrTntCardExpiryTimer_Timer()
'    On Error Resume Next
'    tmrTntCardExpiryTimer.Enabled = False
'    CheckCardsExpiry
'    If Not m_bShutDown Then tmrTntCardExpiryTimer.Enabled = True
'End Sub
'
'Private Sub CheckCardsExpiry()
'    On Error Resume Next
'    Dim aCard As EtsMmGeneralLib.MmTntCardAtom
'
'    For Each aCard In m_CardForExpiryCheck
'        aCard.IsExpired = DateDiff("s", aCard.TradeDate, Now) > TRADE_EXPIRY_PERIOD
'        If aCard.IsExpired Then
'            m_CardForExpiryCheck.Remove aCard.TradeID
'            RaiseEvent OnUpdateCard(aCard)
'        End If
'    Next
'End Sub
'
'Private Sub TntAdapter_ConnectorError(ByVal sCode As Long, ByVal bsMessage As String)
'    On Error Resume Next
'    m_bIsConnected = False
'
'    If m_hDisconnectTimer = 0 Then ' not syncronous logoff
'        RaiseEvent OnConnectStateChange
'        AddEvent enEvtError, True, "Fail to connect to trades messages server: " & _
'                bsMessage & " (" & sCode & ")." & vbCrLf & "Please check connection settings."
'    End If
'End Sub
'
'Private Sub TntAdapter_LogoffResponse(ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)
'    On Error Resume Next
'    m_bIsConnected = False
'
'    If m_hDisconnectTimer = 0 Then ' not syncronous logoff
'
'        If sCode = 0 Then
'            AddEvent enEvtInfo, False, "Logging off from trades messages server succeeded."
'            RaiseEvent OnConnectStateChange
'        Else
'            AddEvent enEvtError, False, "Fail to logging off from trades messages server: " & GetTntErrorDescString(saErrors)
'        End If
'
'        If m_bIsReconnectNow Then
'            m_bIsReconnectNow = False
'            tmrReconnectTimer.Enabled = True
'        End If
'    Else
'        m_bIsReconnectNow = False
'    End If
'End Sub
'
'Private Sub TntAdapter_LogonResponse(ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)
'    On Error Resume Next
'    If sCode = 0 Then
'        m_bIsConnected = True
'        AddEvent enEvtInfo, False, "Logging on to trades messages server succeeded."
'        RaiseEvent OnConnectStateChange
'    Else
'        m_bIsConnected = False
'        RaiseEvent OnConnectStateChange
'        AddEvent enEvtError, True, "Fail to logging on to trades messages server:" & _
'            GetTntErrorDescString(saErrors) & vbCrLf & "Please check connection settings."
'    End If
'End Sub
'
'Private Sub TntAdapter_TerminateConnection()
'    On Error Resume Next
'    m_bIsConnected = False
'    RaiseEvent OnConnectStateChange
'    AddEvent enEvtWarning, True, "Automatically logged off for the day by trades messages server."
'End Sub
'
'Private Sub TntAdapter_TradeAddResponse(pData As TNTConnector.ACKData, ByVal sCode As Long, saErrors() As TNTConnector.ErrorDescription)
'    On Error Resume Next
'    Dim aCard As EtsMmGeneralLib.MmTntCardAtom
'
'    Set aCard = m_CardBySeq(pData.SequenceNo)
'    If Not aCard Is Nothing Then
'        m_CardBySeq.Remove pData.SequenceNo
'        aCard.PoeID = pData.PoeID
'    End If
'
'    If Not aCard Is Nothing Then
'        If Not m_Card(aCard.TradeID) Is Nothing Then ' check if not deleted after sending to POETS
'            If aCard.Status = enTcsOutPendingExecute Or aCard.Status = enTcsInPendingAccept Then
'                If sCode = 0 Then
'                    If aCard.Status = enTcsOutPendingExecute Then
'                        aCard.Status = enTcsOutExecuted
'                        If aCard.MakeTrade Then MakeManualTradeFromCard aCard
'
'                    ElseIf aCard.Status = enTcsInPendingAccept Then
'                        aCard.Status = enTcsInAccepted
'                        aCard.IsExpired = False
'                        m_CardForExpiryCheck.Remove aCard.TradeID
'                    End If
'                Else
'                    If aCard.Status = enTcsOutPendingExecute Then
'                        aCard.Status = enTcsOutFailed
'                        AddEvent enEvtWarning, False, "New trade message " & aCard.Desc & " was rejected by POETS: " & GetTntErrorDescString(saErrors)
'
'                    ElseIf aCard.Status = enTcsInPendingAccept Then
'                        aCard.Status = enTcsInFailed
'                        AddEvent enEvtWarning, False, "Accept/ACK of trade message " & aCard.Desc & " was rejected by POETS: " & GetTntErrorDescString(saErrors)
'                    End If
'                End If
'
'                CardSaveStatusToDB aCard, True
'                RaiseEvent OnUpdateCard(aCard)
'            Else
'                Debug.Assert False
'            End If
'        End If
'    Else
'        AddEvent enEvtWarning, False, "Unexpected acknowledgement message was received: sequence number: " & _
'                        pData.SequenceNo & ", POEID: " & pData.PoeID & _
'                        IIf(sCode = 0, ".", ". Error(s): " & GetTntErrorDescString(saErrors))
'    End If
'End Sub
'
'Private Sub TntAdapter_TradeNotification(pData As TNTConnector.TNTData)
'    On Error Resume Next
'    Dim aCard As New EtsMmGeneralLib.MmTntCardAtom, sTradeID$
'
'    If CopyCardFromSub(pData, aCard) Then
'        m_Card.Add aCard.TradeID, aCard.TradeDate, aCard
'
'        If DateDiff("s", aCard.TradeDate, Now) < TRADE_EXPIRY_PERIOD Then
'            m_CardForExpiryCheck.Add sTradeID, aCard.TradeDate, aCard
'        Else
'            aCard.IsExpired = True
'        End If
'
'        RaiseEvent OnNewCard(aCard)
'    End If
'End Sub
'
'Public Sub MakeManualTradeFromCard(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error GoTo EH
'    Dim aTrd As New EtsGeneralLib.MmTradeInfoAtom
'
'    aCard.MakeTrade = False
'
'    CopyTradeFromCard aCard, aTrd
'
'    g_TradeChannel.NewTrade aTrd
'
'    AddEvent enEvtInfo, False, "New manual trade was created for trade message " & aCard.Desc & "."
'
'    Exit Sub
'EH:
'    AddEvent enEvtError, False, "Fail to create new manual trade for trade message " & aCard.Desc & ": " & vbCrLf & Err.Description
'End Sub
'
'Public Sub CopyTradeFromCard(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom, ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aUnd As EtsGeneralLib.UndAtom
'
'    Set aTrd.Und = aCard.Und
'
'    aTrd.SeqNum = 0
'    aTrd.TradeID = 0
'    aTrd.Price = aCard.Price
'    aTrd.Quantity = aCard.Qty
'    aTrd.IsBuy = aCard.IsBuy
'    aTrd.TradeDate = aCard.TradeDate
'    aTrd.Status = enTrsManual
'    aTrd.BrokerCommission = 0#
'    aTrd.ClearingBrokerCommission = 0#
'    aTrd.Mark = 0
'
'    Set aTrd.Trader = m_CurTrader
'    Set aTrd.Broker = Nothing
'    Set aTrd.ClearingBroker = Nothing
'    Set aTrd.Strategy = Nothing
'
'    Set aTrd.OptRoot = aCard.OptRoot
'    Set aTrd.Opt = New EtsGeneralLib.EtsOptAtom
'
'    aTrd.Opt.ID = aCard.Opt.ID
'    aTrd.Opt.Symbol = aCard.Opt.Symbol
'    aTrd.Opt.RootID = aTrd.OptRoot.ID
'    aTrd.Opt.OptType = aCard.Opt.OptType
'    aTrd.Opt.Expiry = aCard.Opt.Expiry
'    aTrd.Opt.Strike = Round(aCard.Opt.Strike, STRIKE_DECIMALS_COUNT)
'    aTrd.Opt.PriceClose = aCard.Opt.PriceClose
'    aTrd.Opt.PriceTheoClose = aCard.Opt.PriceTheoClose
'
'    aTrd.SpotReference = 0#
'    aTrd.TradedIV = 0#
'End Sub
'
'Private Sub MakeNewCardForPub(ByRef aSrc As EtsMmGeneralLib.MmTntCardAtom, ByRef aDst As TNTConnector.TNTData)
'    On Error Resume Next
'    aDst.TradeID = aSrc.TradeID
'    aDst.IsBuy = aSrc.IsBuy
'    aDst.TradeDateTime = aSrc.TradeDate
'    aDst.TradeQuantity = aSrc.Qty
'    aDst.IsCall = (aSrc.Opt.OptType = enOtCall)
'    aDst.OptionSymbol = aSrc.OptRoot.Name
'    aDst.OptionExpDate = aSrc.Opt.Expiry
'    aDst.Strike = aSrc.Opt.Strike
'    aDst.PremiumPrice = aSrc.Price
'    aDst.ExecutingFirm = aSrc.Account
'    aDst.ExecutingMember = aSrc.Account
'    aDst.CMTA = ""
'    aDst.Quantifier = ""
'    aDst.ContraFirm = ""
'    aDst.ContraMember = aSrc.CntPty.Name
'    aDst.SpreadIndicator = ""
'    aDst.OptionalData = ""
'End Sub
'
'Private Sub MakeCardAcceptForPub(ByRef aSrc As EtsMmGeneralLib.MmTntCardAtom, ByRef aDst As TNTConnector.TNTData)
'    On Error Resume Next
'    aDst.TradeID = aSrc.TradeID
'    aDst.IsBuy = aSrc.IsBuy
'    aDst.TradeDateTime = aSrc.TradeDate
'    aDst.TradeQuantity = aSrc.Qty
'    aDst.IsCall = (aSrc.Opt.OptType = enOtCall)
'    aDst.OptionSymbol = aSrc.OptRoot.Name
'    aDst.OptionExpDate = aSrc.Opt.Expiry
'    aDst.Strike = aSrc.Opt.Strike
'    aDst.PremiumPrice = aSrc.Price
'    aDst.ExecutingFirm = aSrc.ExecutingFirm
'    aDst.ExecutingMember = aSrc.Account
'    aDst.CMTA = aSrc.CMTA
'    aDst.Quantifier = aSrc.Quantifier
'    aDst.ContraFirm = aSrc.ContraFirm
'    aDst.ContraMember = aSrc.CntPty.Name
'    aDst.SpreadIndicator = aSrc.SpreadIndicator
'    aDst.OptionalData = aSrc.OptionalData
'End Sub
'
'Private Function CopyCardFromSub(ByRef aSrc As TNTConnector.TNTData, ByRef aDst As EtsMmGeneralLib.MmTntCardAtom) As Boolean
'    On Error GoTo EH
'    Dim sRoot$, nWarning&
'    Dim rs As ADODB.Recordset
'
'    CopyCardFromSub = False
'
'    ' check root
'    sRoot = Trim$(aSrc.OptionSymbol)
'    If Len(sRoot) <= 0 Then
'        AddEvent enEvtError, False, "Invalid trade message was received from '" & aSrc.ExecutingMember & "': empty option symbol."
'        Exit Function
'    End If
'
'    ' check strike
'    If aSrc.Strike <= 0# Then
'        AddEvent enEvtError, False, "Invalid trade message was received from '" & aSrc.ExecutingMember & "': invalid option strike " & aSrc.Strike & " ."
'        Exit Function
'    End If
'
'    ' check expiry
'    If aSrc.OptionExpDate <= 0# Then
'        AddEvent enEvtError, False, "Invalid trade message was received from '" & aSrc.ExecutingMember & "': invalid option expiry " & aSrc.Strike & " ."
'        Exit Function
'    End If
'
'    ' check symbol
'    If Len(Trim$(aSrc.OptionSymbol)) <= 0 Then
'        AddEvent enEvtError, False, "Invalid trade message was received from '" & aSrc.ExecutingMember & "': invalid option symbol '" & aSrc.OptionSymbol & "' ."
'        Exit Function
'    End If
'
'    ' find a root
'    Set aDst.OptRoot = g_OptRootByName(sRoot)
'    If Not aDst.OptRoot Is Nothing Then Set aDst.Und = g_UnderlyingAll(aDst.OptRoot.UndID)
'
'    If aDst.OptRoot Is Nothing Or aDst.Und Is Nothing Then
'        ' create fake underlying, root and option
'        Set aDst.Und = New EtsGeneralLib.UndAtom
'        aDst.Und.ID = 0
'        aDst.Und.Symbol = "???"
'
'        Set aDst.OptRoot = New EtsGeneralLib.EtsOptRootAtom
'        aDst.OptRoot.ID = 0
'        aDst.OptRoot.Name = sRoot
'        aDst.OptRoot.LotSize = 100
'        aDst.OptRoot.UndID = aDst.Und.ID
'    End If
'
'    ' find a counterparty
'    If Not FindCardCounterParty(aSrc, aDst) Then Exit Function
'
'    aDst.SeqNum = 0
'    aDst.TradeDate = aSrc.TradeDateTime
'    aDst.IsBuy = aSrc.IsBuy
'    aDst.Qty = aSrc.TradeQuantity
'    aDst.Price = aSrc.PremiumPrice
'    aDst.PoeID = ""
'    aDst.Status = enTcsInNew
'    aDst.Account = aSrc.ExecutingMember
'    aDst.ExecutingFirm = aSrc.ExecutingFirm
'
'    aDst.CMTA = aSrc.CMTA
'    aDst.Quantifier = aSrc.Quantifier
'    aDst.ContraFirm = aSrc.ContraFirm
'    aDst.SpreadIndicator = aSrc.SpreadIndicator
'    aDst.OptionalData = aSrc.OptionalData
'
'    With aDst
'        Set rs = gDBW.usp_TntCardIn_New(m_CurTrader.ID, .CntPty.ID, .TradeDate, IIf(.IsBuy, 1, 0), _
'                                        .Qty, .Price, .PoeID, .Status, .Account, .ExecutingFirm, _
'                                        .CMTA, .Quantifier, .ContraFirm, .SpreadIndicator, .OptionalData, _
'                                        .Und.ID, .OptRoot.ID, .OptRoot.Name, 0, .OptRoot.Name, _
'                                        aSrc.OptionExpDate, aSrc.Strike, IIf(aSrc.IsCall, 1, 0), _
'                                        MAX_TRADE_NO)
'        If Not rs.EOF Then
'            aDst.ID = ReadLng(rs!iCardID)
'            If aDst.ID <> 0 Then
'                aDst.TradeID = GetTradeID(aDst.ID)
'
'                If m_Card(aDst.TradeID) Is Nothing Then
'                    nWarning = ReadLng(rs!iWarning)
'
'                    Set aDst.Opt = New EtsGeneralLib.EtsOptAtom
'
'                    If nWarning = 0 Then
'                        .Opt.ID = ReadLng(rs!iOptID)
'                        .Opt.Symbol = ReadStr(rs!vcOptSymbol)
'                    Else
'                        .Opt.ID = 0
'                        .Opt.Symbol = aDst.OptRoot.Name
'                    End If
'
'                    .Opt.Expiry = aSrc.OptionExpDate
'                    .Opt.OptType = IIf(aSrc.IsCall, enOtCall, enOtPut)
'                    .Opt.Strike = aSrc.Strike
'                    .Opt.RootID = aDst.OptRoot.ID
'
'                    AdjustOptSeries aDst.Opt, aDst.OptRoot
'
'                    If nWarning <> 0 Then _
'                        AddEvent enEvtWarning, False, "New trade message " & aDst.TradeID & " with unknown option was received: " & aDst.Opt.Series & " ."
'
'                    If aDst.Und.ID <> 0 Then
'                        If Not aDst.Und.IsTraderContract Then
'                            AddEvent enEvtWarning, False, "New trade message with the underlying which is not" & _
'                                                        " in the list of underlyings you manage was received: " & _
'                                                        aDst.TradeID & ", symbol " & aDst.Und.Symbol & "."
'                        End If
'                    Else
'                        AddEvent enEvtWarning, False, "New trade message with unknown underlying was received: " & aDst.TradeID & ", option root symbol: " & sRoot & "."
'                    End If
'
'                Else
'                    AddEvent enEvtError, False, "Invalid trade message was received: trade message " & aDst.TradeID & " is already was received."
'                    CardDeleteFromDB aDst, False
'                End If
'
'            Else
'                AddEvent enEvtError, False, "Fail to process income trade message from '" & aSrc.ExecutingMember & "': " & _
'                                    vbCrLf & "No new unique trade message identifier is available - trade message number exceeds limit of " & MAX_TRADE_NO & "."
'            End If
'        Else
'            AddEvent enEvtError, False, "Invalid trade message was received from '" & aSrc.ExecutingMember & "': unexpected error while checking option data."
'        End If
'    End With
'
'    MakeCardMsgAndDesc aDst
'
'    CopyCardFromSub = True
'
'    Exit Function
'EH:
'    AddEvent enEvtError, False, "Fail to process income trade message: an error was occured while saving trade message to database: " & Err.Description
'End Function
'
'Private Sub MakeCardMsgAndDesc(ByRef aCard As EtsMmGeneralLib.MmTntCardAtom)
'    On Error Resume Next
'    aCard.Msg = IIf(aCard.IsBuy, "Buy ", "Sell ") & CStr(aCard.Qty) & " " & _
'                aCard.OptRoot.Name & " " & UCase$(Format$(aCard.Opt.Expiry, "MMMYY")) & " " & _
'                CStr(Int(aCard.Opt.Strike)) & IIf(aCard.Opt.OptType = enOtCall, " Calls", " Puts") & _
'                " @ " & CStr(aCard.Price) & IIf(aCard.Status >= enTcsInNew, " From: " & aCard.Account, " To: " & aCard.CntPty.Name)
'
'    aCard.Desc = aCard.TradeID & " (" & aCard.Msg & ")"
'End Sub
'
'Private Function FindCardCounterParty(ByRef aSrc As TNTConnector.TNTData, ByRef aDst As EtsMmGeneralLib.MmTntCardAtom) As Boolean
'    On Error GoTo EH
'    Dim nCntPtyID&, sCntPtyName$
'    FindCardCounterParty = False
'
'    sCntPtyName = UCase$(Trim$(aSrc.ContraMember))
'
'    If Len(sCntPtyName) > 0 Then
'        Set aDst.CntPty = g_TntCntPtyByName(sCntPtyName)
'
'        If aDst.CntPty Is Nothing Then
'            nCntPtyID = gDBW.usp_TntCounterParty_Save(0, sCntPtyName)
'            Set aDst.CntPty = g_TntCntPty(nCntPtyID)
'
'            If aDst.CntPty Is Nothing Then
'                Set aDst.CntPty = New EtsMmGeneralLib.MmTntCounterPartyAtom
'                aDst.CntPty.ID = nCntPtyID
'                aDst.CntPty.Name = sCntPtyName
'                aDst.CntPty.Desc = sCntPtyName
'
'                g_TntCntPty.Add aDst.CntPty.ID, aDst.CntPty.Name, aDst.CntPty
'                g_TntCntPtyByName.Add aDst.CntPty.Name, aDst.CntPty
'
'                g_Params.FireNewTntCntPtyAdded nCntPtyID
'            End If
'        End If
'    Else
'        Set aDst.CntPty = New EtsMmGeneralLib.MmTntCounterPartyAtom
'        aDst.CntPty.ID = 0
'        aDst.CntPty.Name = ""
'    End If
'
'    FindCardCounterParty = True
'    Exit Function
'EH:
'    AddEvent enEvtError, False, "Invalid trade message was received: " & Trim$(aSrc.TradeID) & ": unexpected error while counterparty data. " & Err.Description
'End Function
'
'Public Property Get IsConnected() As Boolean
'    On Error Resume Next
'    IsConnected = m_bIsConnected
'End Property
'
'Public Property Get UseTntMessaging() As Boolean
'    On Error Resume Next
'    UseTntMessaging = m_bUseTntMessaging
'End Property
'
'Public Property Let UseTntMessaging(ByVal bUseTntMessaging As Boolean)
'    On Error Resume Next
'    If m_bUseTntMessaging <> bUseTntMessaging Then
'        m_bUseTntMessaging = bUseTntMessaging
'
'        AddEvent IIf(m_bUseTntMessaging, enEvtInfo, enEvtWarning), False, _
'            "Trades Messages functionality is " & IIf(m_bUseTntMessaging, "enabled.", "disabled.")
'
'        g_Params.FireUseTntMessagingChanged
'
'        If m_bUseTntMessaging Then
'            tmrReconnectTimer.Enabled = True
'        Else
'            Disconnect False
'        End If
'    End If
'End Property
'
'Private Function ShowMsgBox(ByVal frmOwner As Form, ByVal enStyle As VbMsgBoxStyle, ByVal sPrompt As String) As VbMsgBoxResult
'    On Error Resume Next
'    If m_bUseTntMessaging Then
'        Dim bDoNotUseTnt As Boolean: bDoNotUseTnt = False
'        ShowMsgBox = gCmn.TntMsgBox(frmOwner, enStyle, bDoNotUseTnt, sPrompt)
'
'        If bDoNotUseTnt Then UseTntMessaging = False
'    End If
'End Function
'
'Public Function CheckUsingTngMessagingWithWarning(ByVal frmOwner As Form) As Boolean
'    On Error Resume Next
'    If Not m_bUseTntMessaging Then
'        gCmn.MyMsgBox frmOwner, "Trades Messages functionality is disabled." & vbCrLf & _
'            "You can enable this functionality in Parameters dialog.", vbCritical
'
'        CheckUsingTngMessagingWithWarning = False
'        Exit Function
'    End If
'
'    If m_CurTrader Is Nothing Then
'        ShowMsgBox frmOwner, vbCritical, "Trades messages are restricted under <All Traders> login. Please log in as a trader."
'
'        CheckUsingTngMessagingWithWarning = False
'        Exit Function
'    End If
'
'    CheckUsingTngMessagingWithWarning = True
'End Function
