VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsOrdersProcessor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Event OnNewOrder(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'Public Event OnNewOrderExec(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom, ByRef aOrdExec As EtsMmGeneralLib.MmOrderExecAtom)
'Public Event OnNewEvent(ByRef aEvent As EtsGeneralLib.EtsEventAtom)
'Public Event OnUpdateOrder(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'Public Event OnUpdateOrderExec(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom, ByRef aOrdExec As EtsMmGeneralLib.MmOrderExecAtom)
'Public Event OnClearEvents()
'
'Private WithEvents MmwMsgManager As MMWLib.MsgManager
'
'Private m_Ord As New EtsMmGeneralLib.MmOrderColl
'Private m_OrdReq As New EtsMmGeneralLib.MmOrderByReqColl
'Private m_Evt As New EtsGeneralLib.EtsEventColl
'
'Private m_nLastOrderSeqNum As Long
'Private m_nLastOrderExecSeqNum As Long
'Private m_bCanProcessOrdersMessages As Boolean
'Private m_nEventID&
'Private m_nOrdersGatewayStatus&
'
'Private Sub Class_Initialize()
'    On Error Resume Next
'    m_nEventID = 0&
'    m_bCanProcessOrdersMessages = False
'    m_Evt.Clear
'    m_nOrdersGatewayStatus = -1
'End Sub
'
'Public Sub InitMessaging()
'    On Error GoTo EH
'
'    If MmwMsgManager Is Nothing Then
'        Set MmwMsgManager = New MMWLib.MsgManager
'        m_bCanProcessOrdersMessages = True
'
'        AddEvent enEvtInfo, "Orders processing initialized."
'    End If
'
'    Exit Sub
'EH:
'    gCmn.ErrorHandler "Orders Processor: Fail to initialize messaging service."
'End Sub
'
'Public Sub LoadOrders(Optional ByRef lblProgress As VB.Label = Nothing)
'    On Error GoTo EH
'    Dim rsOrd As ADODB.Recordset, rsExec As ADODB.Recordset, bShowProgress As Boolean, nTotal&, nDone&
'    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, enContractType As EtsGeneralLib.EtsContractTypeEnum
'    Dim aExec As EtsMmGeneralLib.MmOrderExecAtom, aUnd As EtsGeneralLib.UndAtom, aRoot As EtsGeneralLib.EtsOptRootAtom
'
'    bShowProgress = Not lblProgress Is Nothing
'
'    If bShowProgress Then
'        lblProgress.Caption = "Loading orders data... 0% complete."
'        lblProgress.Refresh
'    End If
'
'    m_Ord.Clear
'    m_OrdReq.Clear
'
'    Set rsOrd = gDBW.usp_OrderByTrader_Get(g_CurTraderID)
'    Set rsExec = gDBW.usp_OrderExec_Get(0, g_CurTraderID)
'    nDone = 0&
'    nTotal = rsOrd.RecordCount + rsExec.RecordCount
'
'    While Not rsOrd.EOF
'        Set aUnd = g_Underlying(ReadLng(rsOrd!iUnderlyingID))
'        If Not aUnd Is Nothing Then
'            enContractType = ReadLng(rsOrd!iContractTypeID)
'            Set aRoot = aUnd.Roots(ReadLng(rsOrd!iOptionRootID))
'
'            If enContractType <> enCtOption _
'                Or enContractType = enCtOption And Not aRoot Is Nothing Then
'
'                Set aOrd = New EtsMmGeneralLib.MmOrderAtom
'                aOrd.ID = ReadLng(rsOrd!iOrderID)
'                aOrd.SeqNum = ReadLng(rsOrd!iSeqNum)
'                aOrd.UpdateDate = ReadDate(rsOrd!dtActionDate)
'                Set aOrd.Trader = g_Trader(ReadLng(rsOrd!iTraderID))
'                Set aOrd.Dest = g_OrderExecDest(ReadLng(rsOrd!iDestID))
'                aOrd.Status = ReadLng(rsOrd!iStatus)
'                aOrd.OrderType = ReadLng(rsOrd!iOrderType)
'                aOrd.IsBuy = (ReadByte(rsOrd!tiIsBuy) <> 0)
'                aOrd.OrderQty = ReadLng(rsOrd!iOrderQty)
'                aOrd.OrderPrice = ReadDbl(rsOrd!fOrderPrice)
'                aOrd.OrderDate = GmtToLocal(ReadDate(rsOrd!dtOrderDate))
'                aOrd.OrderID = ReadStr(rsOrd!vcClOrderID)
'                aOrd.AvgPrice = ReadDbl(rsOrd!fAvgPrice)
'                aOrd.CumQty = ReadLng(rsOrd!iCumQty)
'
'                Set aOrd.Und = aUnd
'
'                If enContractType = enCtOption Then
'                    Set aOrd.OptRoot = aRoot
'
'                    Set aOrd.Opt = New EtsGeneralLib.EtsOptAtom
'                    aOrd.Opt.ID = ReadLng(rsOrd!iContractID)
'                    aOrd.Opt.Symbol = ReadStr(rsOrd!vcSymbol)
'                    aOrd.Opt.RootID = aRoot.ID
'                    aOrd.Opt.Expiry = ReadDate(rsOrd!dtExpiry)
'                    aOrd.Opt.Strike = ReadDbl(rsOrd!fStrike)
'                    aOrd.Opt.OptType = ReadByte(rsOrd!tiIsCall)
'                Else
'                    Set aOrd.OptRoot = Nothing
'                    Set aOrd.Opt = Nothing
'                End If
'
'                AdjustOptSeries aOrd.Opt, aOrd.OptRoot
'
'                m_Ord.Add aOrd.ID, aOrd.OrderDate, aOrd
'
'                Set aOrd = Nothing
'            End If
'        End If
'
'        Set aRoot = Nothing
'        Set aUnd = Nothing
'
'        If bShowProgress Then
'            nDone = nDone + 1
'            lblProgress.Caption = "Loading orders data... " & CStr(nDone * 100 \ nTotal) & "% complete."
'            DoEvents
'        End If
'
'        rsOrd.MoveNext
'    Wend
'    Set rsOrd = Nothing
'
'    While Not rsExec.EOF
'        Set aOrd = m_Ord(ReadLng(rsExec!iOrderID))
'        If Not aOrd Is Nothing Then
'            Set aExec = New EtsMmGeneralLib.MmOrderExecAtom
'            aExec.ID = ReadLng(rsExec!iExecID)
'            aExec.SeqNum = ReadLng(rsExec!iSeqNum)
'            aExec.ExecStatus = ReadLng(rsExec!iExecStatus)
'            aExec.ExecQty = ReadLng(rsExec!iExecQty)
'            aExec.ExecPrice = ReadDbl(rsExec!fExecPrice)
'            aExec.ExecDate = GmtToLocal(ReadDate(rsExec!dtExecDate))
'            aExec.ExecID = ReadStr(rsExec!vcBrokerExecID)
'
'            aOrd.Execs.Add aExec.ID, aExec.ExecDate, aExec
'
'            Set aExec = Nothing
'            Set aOrd = Nothing
'        End If
'
'        If bShowProgress Then
'            nDone = nDone + 1
'            lblProgress.Caption = "Loading orders data... " & CStr(nDone * 100 \ nTotal) & "% complete."
'            DoEvents
'        End If
'
'        rsExec.MoveNext
'    Wend
'    Set rsExec = Nothing
'
'    LoadLastSeqNums
'
'    If bShowProgress Then
'        lblProgress.Caption = "Loading orders data... 100% complete."
'        lblProgress.Refresh
'    End If
'
'    Exit Sub
'EH:
'    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
'    sDescription = "Orders Processor: Fail to load orders. " & Err.Description
'    nHelpContext = Err.HelpContext
'    sHelpFile = Err.HelpFile
'    nNumber = Err.Number
'    sSource = Err.Source
'
'    Set rsOrd = Nothing
'    Set rsExec = Nothing
'    m_Ord.Clear
'    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
'End Sub
'
'Public Sub Start()
'    On Error GoTo EH
'    Dim aPubManager As MMWLib.IPubManager
'    Dim aReq As New MSGSTRUCTLib.FixGatewayStatusRequest
'
'    m_nOrdersGatewayStatus = -1
'
'    MmwMsgManager.SubFixGatewayStatus
'    MmwMsgManager.SubOrdersNotificationMessages
'
'    Set aPubManager = MmwMsgManager
'    aReq.ClientType = 0
'    aPubManager.PubFixGatewayStatusRequest aReq
'    Set aPubManager = Nothing
'
'    Exit Sub
'EH:
'    gCmn.ErrorHandler "Orders Processor: Fail to start real-time orders service."
'End Sub
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    m_bCanProcessOrdersMessages = False
'
'    MmwMsgManager.UnsubFixGatewayStatus
'    MmwMsgManager.UnsubOrdersNotificationMessages
'
'    Set MmwMsgManager = Nothing
'End Sub
'
'Private Sub LoadLastSeqNums()
'    On Error GoTo EH
'
'    m_nLastOrderSeqNum = gDBW.usp_OrderLastSeqNum_Get
'    m_nLastOrderExecSeqNum = gDBW.usp_OrderExecLastSeqNum_Get
'
'    Exit Sub
'EH:
'    gCmn.ErrorHandler "Orders Processor: Fail to load last orders sequence number."
'End Sub
'
'Public Property Get Orders() As EtsMmGeneralLib.MmOrderColl
'    On Error Resume Next
'    Set Orders = m_Ord
'End Property
'
'Public Property Get Events() As EtsGeneralLib.EtsEventColl
'    On Error Resume Next
'    Set Events = m_Evt
'End Property
'
'Private Sub AddEvent(ByVal enEvtType As EtsEventTypeEnum, ByRef sEvtMessage As String)
'    On Error Resume Next
'    Dim aEvent As New EtsGeneralLib.EtsEventAtom
'
'    m_nEventID = m_nEventID + 1
'    aEvent.EvtID = m_nEventID
'    aEvent.EvtTime = Now()
'    aEvent.EvtType = enEvtType
'    aEvent.EvtMessage = sEvtMessage
'
'    Err.Clear
'    m_Evt.Add aEvent.EvtID, aEvent.EvtTime, aEvent
'    If Err.Number = 0 Then
'        RaiseEvent OnNewEvent(aEvent)
'    Else
'        LogEvent EVENT_ERROR, "Orders Processor: fail to log new order event with message '" & sEvtMessage & "'"
'    End If
'End Sub
'
'Public Sub ClearEvents()
'    On Error Resume Next
'    m_Evt.Clear
'    RaiseEvent OnClearEvents
'End Sub
'
'Public Sub OrderNew(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'    On Error GoTo EH
'    Dim aPubManager As MMWLib.IPubManager
'    Dim aReq As New MSGSTRUCTLib.Order
'
'    Screen.MousePointer = vbHourglass
'
'    AdjustOptSeries aOrd.Opt, aOrd.OptRoot
'
'    aOrd.RequestUid = GenerateUID
'    m_OrdReq.Add aOrd.RequestUid, Now(), aOrd
'
'    CopyOrderForPub aOrd, aReq
'    If aOrd.OrderType = enOrtMarket Then
'        AddEvent enEvtInfo, "Submit new order request: " & aOrd.Symbol & " " & aOrd.OrderTypeString & " " & aOrd.OrderQty
'    Else
'        AddEvent enEvtInfo, "Submit new order request: " & aOrd.Symbol & " " & aOrd.OrderTypeString & " " & aOrd.OrderQty & " at " & aOrd.OrderPrice
'    End If
'
'    Set aPubManager = MmwMsgManager
'    aPubManager.PubRequestNewOrder aReq
'
'EX:
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'    Exit Sub
'EH:
'    AddEvent enEvtError, "Orders Processor: fail to submit new order request: " & vbCrLf & Err.Description
'    On Error Resume Next
'    m_OrdReq.Remove aOrd.RequestUid
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'End Sub
'
'Public Sub OrderCancel(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'    On Error GoTo EH
'    Dim aPubManager As MMWLib.IPubManager
'    Dim aReq As New MSGSTRUCTLib.Order
'
'    Screen.MousePointer = vbHourglass
'
'    aOrd.RequestUid = GenerateUID
'    m_OrdReq.Add aOrd.RequestUid, Now(), aOrd
'
'    CopyOrderForPub aOrd, aReq
'    AddEvent enEvtInfo, "Submit order cancel request: " & aOrd.OrderID
'
'    Set aPubManager = MmwMsgManager
'    aPubManager.PubCancelOrder aReq
'
'EX:
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'    Exit Sub
'EH:
'    AddEvent enEvtError, "Orders Processor: fail to submit order cancel request: " & vbCrLf & Err.Description
'    On Error Resume Next
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'End Sub
'
'Public Sub OrderAlternate(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'    On Error GoTo EH
'    Dim aPubManager As MMWLib.IPubManager
'    Dim aReq As New MSGSTRUCTLib.Order
'
'    Screen.MousePointer = vbHourglass
'
'    aOrd.RequestUid = GenerateUID
'    m_OrdReq.Add aOrd.RequestUid, Now(), aOrd
'
'    CopyOrderForPub aOrd, aReq
'    AddEvent enEvtInfo, "Submit order cancel/replace request: " & aOrd.OrderID
'
'    Set aPubManager = MmwMsgManager
'    aPubManager.PubAlterOrder aReq
'
'EX:
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'    Exit Sub
'EH:
'    AddEvent enEvtError, "Orders Processor: fail to submit order cancel/replace request: " & vbCrLf & Err.Description
'    On Error Resume Next
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'End Sub
'
'Public Sub OrderStatusRequest(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'    On Error GoTo EH
'    Dim aPubManager As MMWLib.IPubManager
'    Dim aReq As New MSGSTRUCTLib.Order
'
'    Screen.MousePointer = vbHourglass
'
'    aOrd.RequestUid = GenerateUID
'    m_OrdReq.Add aOrd.RequestUid, Now(), aOrd
'
'    CopyOrderForPub aOrd, aReq
'    AddEvent enEvtInfo, "Submit order status request: " & aOrd.OrderID
'
'    Set aPubManager = MmwMsgManager
'    aPubManager.PubRequestOrderStatus aReq
'
'EX:
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'    Exit Sub
'EH:
'    AddEvent enEvtError, "Orders Processor: fail to submit order status request: " & vbCrLf & Err.Description
'    On Error Resume Next
'    Screen.MousePointer = vbDefault
'    Set aPubManager = Nothing
'End Sub
'
'Private Sub CopyOrderForPub(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom, ByRef aReq As MSGSTRUCTLib.Order)
'    On Error Resume Next
'    aReq.FixProvider = enMsFpOConnor
'
'    aReq.ID = aOrd.ID
'    aReq.SeqNum = 0&
'    aReq.TransactTime = 0#
'    aReq.TraderID = aOrd.Trader.ID
'    aReq.OrdStatus = aOrd.Status
'    aReq.OrderType = aOrd.OrderType
'    aReq.IsBuy = IIf(aOrd.IsBuy, 1, 0)
'    aReq.Qty = aOrd.OrderQty
'    aReq.Price = IIf(aOrd.OrderType = enOrtLimit, aOrd.OrderPrice, 0#)
'    aReq.SendingTime = aOrd.OrderDate
'    aReq.AvgPrice = aOrd.AvgPrice
'    aReq.CumQty = aOrd.CumQty
'    aReq.ClOrdID = aOrd.OrderID
'    aReq.RequestUid = aOrd.RequestUid
'
'    aReq.Account = aOrd.Trader.Name
'    'aReq.ExecBroker
'    aReq.DestID = aOrd.Dest.ID
'    aReq.ExDestination = aOrd.Dest.Code
'    aReq.LeavesQty = 0&
'    aReq.Text = ""
'    aReq.ExecType = 0
'    aReq.UndID = aOrd.Und.ID
'
'    If Not aOrd.Opt Is Nothing And Not aOrd.OptRoot Is Nothing Then
'        aReq.SymbolType = enStOption
'        aReq.OptionID = aOrd.Opt.ID
'        aReq.Symbol = aOrd.Symbol
'        aReq.OptionRootID = aOrd.OptRoot.ID
'        aReq.OptionRootSymbol = aOrd.OptRoot.Name
'        aReq.Expiry = aOrd.Opt.Expiry
'        aReq.Strike = aOrd.Opt.Strike
'        aReq.IsCall = IIf(aOrd.Opt.OptType <> enOtPut, 1, 0)
'    Else
'        aReq.SymbolType = IIf(aOrd.Und.UndType = enCtStock, enStStock, enStIndex)
'        aReq.Symbol = aOrd.Und.Symbol
'        aReq.OptionID = 0&
'        aReq.OptionRootID = 0&
'        aReq.OptionRootSymbol = ""
'        aReq.Expiry = 0#
'        aReq.Strike = 0#
'        aReq.IsCall = 0
'    End If
'End Sub
'
'Private Sub MmwMsgManager_OnFixGatewayStatus(ByVal Data As MSGSTRUCTLib.IFixGatewayStatus)
'    On Error Resume Next
'    If Not m_bCanProcessOrdersMessages Then Exit Sub
'    Dim nNewOrdersGatewayStatus&, sHost$, sIP$
'
'    nNewOrdersGatewayStatus = IIf(Data.IsOnline, 1, 0)
'    sHost = Trim$(Data.Host)
'    sIP = Trim$(Data.IP)
'
'    If m_nOrdersGatewayStatus <> nNewOrdersGatewayStatus Then
'        m_nOrdersGatewayStatus = nNewOrdersGatewayStatus
'
'        If m_nOrdersGatewayStatus = 1 Then
'            AddEvent enEvtInfo, "Orders gateway at host '" & sHost & "' (" & sIP & ") is online."
'        Else
'            If Len(sHost) > 0 Or Len(sIP) > 0 Then
'                AddEvent enEvtWarning, "Orders gateway at host '" & IIf(Len(sHost) > 0, sHost, "<unknown>") _
'                                            & "'" & IIf(Len(sIP) > 0, " (" & sIP & ")", "") & " is offline."
'            Else
'                AddEvent enEvtError, "No online orders gateway found."
'            End If
'        End If
'    End If
'End Sub
'
'Private Sub MmwMsgManager_OnExecutionReport(ByVal Data As MSGSTRUCTLib.IExecutionReport)
'    On Error Resume Next
'    If Not m_bCanProcessOrdersMessages Then Exit Sub
'    Dim nSeqNum&, aOrd As EtsMmGeneralLib.MmOrderAtom, aExec As EtsMmGeneralLib.MmOrderExecAtom
'    Dim bValidExec As Boolean: bValidExec = False
'
'    With Data
'        nSeqNum = .SeqNum
'        If nSeqNum > m_nLastOrderExecSeqNum Then ' valid sequence
'            If nSeqNum > m_nLastOrderExecSeqNum + 1 Then
'                ReloadMissedOrderExecs m_nLastOrderExecSeqNum + 1, nSeqNum - 1
'            End If
'
'            m_nLastOrderExecSeqNum = nSeqNum
'            Set aOrd = m_Ord(.OrderID)
'
'            If Not aOrd Is Nothing Then ' current trader order
'                Set aExec = aOrd.Execs(.ID)
'                If aExec Is Nothing Then ' new order exec
'                    Set aExec = New EtsMmGeneralLib.MmOrderExecAtom
'
'                    If AddNewOrderExecFromSub(Data, aOrd, aExec) Then
'                        RaiseEvent OnNewOrderExec(aOrd, aExec)
'                        FireOrderExecEvent aOrd.OrderID, Data
'                    Else
'                        AddEvent enEvtError, "Orders Processor: Invalid data got for order execution " & Data.ExecID & " - invalid order execution internal ID."
'                    End If
'                Else
'                    CopyOrderExecUpdatesFromSub Data, aOrd, aExec
'                    RaiseEvent OnUpdateOrderExec(aOrd, aExec)
'                    FireOrderExecEvent aOrd.OrderID, Data
'                End If
'
'                Set aExec = Nothing
'                Set aOrd = Nothing
'            End If
'        Else
'             AddEvent enEvtWarning, "Orders Processor: Invalid data was received for execution report" & Data.ExecID & " - out of sequence."
'        End If
'    End With
'End Sub
'
'Private Sub MmwMsgManager_OnOrder(ByVal Data As MSGSTRUCTLib.IOrder)
'    On Error Resume Next
'    If Not m_bCanProcessOrdersMessages Then Exit Sub
'    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, sMsg$, nSeqNum&, bValidOrder As Boolean: bValidOrder = False
'
'    With Data
'        nSeqNum = .SeqNum
'        If nSeqNum = 0 Then ' request rejected before sending to fix
'            Set aOrd = m_OrdReq(.RequestUid)
'            If Not aOrd Is Nothing Then ' my request
'                m_OrdReq.Remove .RequestUid
'                FireOrderEvent Data
'            End If
'        Else
'            If nSeqNum > m_nLastOrderSeqNum Then ' valid sequence
'                If nSeqNum > m_nLastOrderSeqNum + 1 Then
'                    ReloadMissedOrders m_nLastOrderSeqNum + 1, nSeqNum - 1
'                End If
'
'                m_nLastOrderSeqNum = nSeqNum
'
'                If g_CurTraderID = 0 Or g_CurTraderID = .TraderID Then
'                    Set aOrd = m_Ord(.ID)
'                    If aOrd Is Nothing Then ' new order
'
'                        Set aOrd = m_OrdReq(.RequestUid)
'
'                        If Not aOrd Is Nothing Then ' my new order
'                            m_OrdReq.Remove .RequestUid
'                            CopyOrderUpdatesFromSub Data, aOrd
'                            aOrd.RequestUid = ""
'                            bValidOrder = True
'
'                        Else ' other trader order
'                            Set aOrd = New EtsMmGeneralLib.MmOrderAtom
'                            bValidOrder = CopyFullOrderFromSub(Data, aOrd)
'                            aOrd.RequestUid = ""
'                        End If
'
'                        If bValidOrder Then
'                            Err.Clear
'                            m_Ord.Add aOrd.ID, aOrd.OrderDate, aOrd
'                            If Err.Number = 0 Then
'                                RaiseEvent OnNewOrder(aOrd)
'                                FireOrderEvent Data
'                            Else
'                                AddEvent enEvtError, "Orders Processor: Invalid data was received for order " & Data.ClOrdID & " - invalid order internal ID."
'                            End If
'                        Else
'                            If aOrd.Dest Is Nothing Then
'                                sMsg = "unknown destination '" & .ExDestination & "'."
'                            End If
'
'                            If aOrd.Trader Is Nothing Then
'                                sMsg = "unknown trader '" & .Account & "'."
'                            End If
'
'                            If aOrd.Und Is Nothing And g_CurTraderID = 0 Then
'                                sMsg = "unknown underlying."
'
'                            ElseIf aOrd.OptRoot Is Nothing And Not aOrd.Opt Is Nothing Then
'                                sMsg = "unknown option root."
'                            End If
'
'                            If Len(sMsg) > 0 Then _
'                                AddEvent enEvtWarning, "Orders Processor: Invalid data was received for order " & aOrd.OrderID & " - " & sMsg
'                        End If
'
'                        Set aOrd = Nothing
'                    Else
'                        ' update m_Ord
'                        CopyOrderUpdatesFromSub Data, aOrd
'                        RaiseEvent OnUpdateOrder(aOrd)
'                        FireOrderEvent Data
'                    End If
'                End If
'            Else
'                 AddEvent enEvtWarning, "Orders Processor: Invalid data was received for order " & Data.ClOrdID & " - out of sequence."
'            End If
'        End If
'    End With
'End Sub
'
'Private Function AddNewOrderExecFromSub(ByRef aData As MSGSTRUCTLib.IExecutionReport, _
'                                        ByRef aOrd As EtsMmGeneralLib.MmOrderAtom, _
'                                        ByRef aExec As EtsMmGeneralLib.MmOrderExecAtom) As Boolean
'    On Error Resume Next
'    AddNewOrderExecFromSub = False
'
'    aExec.SeqNum = aData.SeqNum
'    aExec.ID = aData.ID
'
'    aExec.ExecID = aData.ExecID
'    aExec.ExecStatus = aData.ExecType
'    aExec.ExecPrice = aData.LastPrice
'    aExec.ExecQty = aData.LastShares
'    aExec.ExecDate = GmtToLocal(aData.TransactTime)
'
'    aOrd.Status = aData.OrdStatus
'    aOrd.AvgPrice = aData.AvgPrice
'    aOrd.CumQty = aData.CumQty
'
'    'aData.ExecRefID
'    'aData.LeavesQty
'    'aData.OrderID
'    'aData.Text
'
'    Err.Clear
'    aOrd.Execs.Add aExec.ID, aExec.ExecDate, aExec
'    AddNewOrderExecFromSub = (Err.Number = 0)
'End Function
'
'Private Function CopyOrderExecUpdatesFromSub(ByRef aData As MSGSTRUCTLib.IExecutionReport, _
'                                        ByRef aOrd As EtsMmGeneralLib.MmOrderAtom, _
'                                        ByRef aExec As EtsMmGeneralLib.MmOrderExecAtom) As Boolean
'    On Error Resume Next
'
'    aExec.SeqNum = aData.SeqNum
'    aExec.ID = aData.ID
'
'    aExec.ExecID = aData.ExecID
'    aExec.ExecStatus = aData.ExecType
'    aExec.ExecPrice = aData.LastPrice
'    aExec.ExecQty = aData.LastShares
'    aExec.ExecDate = GmtToLocal(aData.TransactTime)
'
'    aOrd.Status = aData.OrdStatus
'    aOrd.AvgPrice = aData.AvgPrice
'    aOrd.CumQty = aData.CumQty
'End Function
'
'Private Function CopyFullOrderFromSub(ByRef aSrc As MSGSTRUCTLib.IOrder, ByRef aDst As EtsMmGeneralLib.MmOrderAtom) As Boolean
'    On Error Resume Next
'
'    CopyFullOrderFromSub = True
'
'    aDst.ID = aSrc.ID
'    aDst.SeqNum = aSrc.SeqNum
'    aDst.UpdateDate = aSrc.TransactTime
'
'    aDst.Status = aSrc.OrdStatus
'    aDst.OrderType = aSrc.OrderType
'    aDst.IsBuy = (aSrc.IsBuy <> 0)
'    aDst.OrderQty = aSrc.Qty
'    aDst.OrderPrice = aSrc.Price
'    aDst.OrderDate = GmtToLocal(aSrc.SendingTime)
'    aDst.AvgPrice = aSrc.AvgPrice
'    aDst.CumQty = aSrc.CumQty
'    aDst.OrderID = aSrc.ClOrdID
'    aDst.RequestUid = aSrc.RequestUid
'
'    Set aDst.Trader = g_Trader(aSrc.TraderID)
'    If aDst.Trader Is Nothing Then CopyFullOrderFromSub = False
'
'    Set aDst.Dest = g_OrderExecDest(aSrc.DestID)
'    If aDst.Dest Is Nothing Then CopyFullOrderFromSub = False
'
'    Set aDst.Und = g_Underlying(aSrc.UndID)
'    If Not aDst.Und Is Nothing Then
'        If aSrc.SymbolType = enStOption Then
'            Set aDst.OptRoot = aDst.Und.Roots(aSrc.OptionRootID)
'            If Not aDst.OptRoot Is Nothing Then
'                Set aDst.Opt = New EtsGeneralLib.EtsOptAtom
'
'                aDst.Opt.ID = aSrc.OptionID
'                aDst.Opt.Symbol = aSrc.Symbol
'                aDst.Opt.RootID = aDst.OptRoot.ID
'                aDst.Opt.Expiry = aSrc.Expiry
'                aDst.Opt.Strike = aSrc.Strike
'                aDst.Opt.OptType = IIf(aSrc.IsCall <> 0, enOtCall, enOtPut)
'            Else
'                CopyFullOrderFromSub = False
'            End If
'        End If
'
'        AdjustOptSeries aDst.Opt, aDst.OptRoot
'    Else
'        CopyFullOrderFromSub = False
'    End If
'
'    'aSrc.Account
'    'aSrc.ExecBroker
'    'aSrc.ExDestination
'    'aSrc.LeavesQty
'    'aSrc.Text
'    'aSrc.ExecType
'End Function
'
'Private Sub CopyOrderUpdatesFromSub(ByRef aSrc As MSGSTRUCTLib.IOrder, ByRef aDst As EtsMmGeneralLib.MmOrderAtom)
'    On Error Resume Next
'    aDst.ID = aSrc.ID
'    aDst.SeqNum = aSrc.SeqNum
'    aDst.UpdateDate = aSrc.TransactTime
'    aDst.Status = aSrc.OrdStatus
'    aDst.OrderQty = aSrc.Qty
'    aDst.OrderPrice = aSrc.Price
'    aDst.OrderDate = GmtToLocal(aSrc.SendingTime)
'    aDst.AvgPrice = aSrc.AvgPrice
'    aDst.CumQty = aSrc.CumQty
'    aDst.OrderID = aSrc.ClOrdID
'End Sub
'
'Private Sub FireOrderEvent(ByRef aData As MSGSTRUCTLib.IOrder)
'    On Error Resume Next
'    Dim sText$, sOrder$
'    sText = Trim$(aData.Text)
'    sOrder = Trim$(aData.ClOrdID)
'    If Len(sText) > 0 Then AddEvent enEvtInfo, IIf(Len(sOrder) > 0, sOrder & ": ", "") & sText
'End Sub
'
'Private Sub FireOrderExecEvent(ByVal sOrder As String, ByRef aData As MSGSTRUCTLib.IExecutionReport)
'    On Error Resume Next
'    Dim sText$
'    sText = Trim$(aData.Text)
'    If Len(sText) > 0 Then AddEvent enEvtInfo, IIf(Len(sOrder) > 0, sOrder & ": ", "") & sText
'End Sub
'
'Private Sub ReloadMissedOrders(ByVal nMinOrderSeqNum As Long, ByVal nMaxOrderSeqNum As Long)
'    On Error Resume Next
'    ReloadMissedOrdersAndExecs nMinOrderSeqNum, nMaxOrderSeqNum, m_nLastOrderExecSeqNum + 1, 0
'End Sub
'
'Private Sub ReloadMissedOrderExecs(ByVal nMinOrderExecSeqNum As Long, ByVal nMaxOrderExecSeqNum As Long)
'    On Error Resume Next
'    ReloadMissedOrdersAndExecs m_nLastOrderSeqNum + 1, 0, nMinOrderExecSeqNum, nMaxOrderExecSeqNum
'End Sub
'
'Private Sub ReloadMissedOrdersAndExecs(ByVal nMinOrderSeqNum As Long, ByVal nMaxOrderSeqNum As Long, _
'                                        ByVal nMinOrderExecSeqNum As Long, ByVal nMaxOrderExecSeqNum As Long)
'    On Error GoTo EH
'    Dim rsOrd As ADODB.Recordset, rsExec As ADODB.Recordset, nOrdID&, bNewOrder As Boolean, bValidOrder As Boolean
'    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, enContractType As EtsGeneralLib.EtsContractTypeEnum
'    Dim aExec As EtsMmGeneralLib.MmOrderExecAtom, aUnd As EtsGeneralLib.UndAtom, aRoot As EtsGeneralLib.EtsOptRootAtom
'    Dim bNewExec As Boolean, nExecID&
'
'    Set rsOrd = gDBW.usp_OrderSeqByTrader_Get(g_CurTraderID, nMinOrderSeqNum, nMaxOrderSeqNum)
'    Set rsExec = gDBW.usp_OrderExecSeq_Get(0, g_CurTraderID, nMinOrderExecSeqNum, nMaxOrderExecSeqNum)
'
'    While Not rsOrd.EOF
'        bValidOrder = False
'        bNewOrder = False
'        nOrdID = ReadLng(rsOrd!iOrderID)
'
'        Set aOrd = m_Ord(nOrdID)
'        If aOrd Is Nothing Then
'            Set aUnd = g_Underlying(ReadLng(rsOrd!iUnderlyingID))
'            If Not aUnd Is Nothing Then
'                enContractType = ReadLng(rsOrd!iContractTypeID)
'                Set aRoot = aUnd.Roots(ReadLng(rsOrd!iOptionRootID))
'
'                If enContractType <> enCtOption _
'                    Or enContractType = enCtOption And Not aRoot Is Nothing Then
'
'                    Set aOrd = New EtsMmGeneralLib.MmOrderAtom
'
'                    aOrd.ID = ReadLng(rsOrd!iOrderID)
'                    Set aOrd.Trader = g_Trader(ReadLng(rsOrd!iTraderID))
'                    Set aOrd.Dest = g_OrderExecDest(ReadLng(rsOrd!iDestID))
'                    aOrd.OrderType = ReadLng(rsOrd!iOrderType)
'
'                    Set aOrd.Und = aUnd
'
'                    If enContractType = enCtOption Then
'                        Set aOrd.OptRoot = aRoot
'
'                        Set aOrd.Opt = New EtsGeneralLib.EtsOptAtom
'                        aOrd.Opt.ID = ReadLng(rsOrd!iContractID)
'                        aOrd.Opt.Symbol = ReadStr(rsOrd!vcSymbol)
'                        aOrd.Opt.RootID = aRoot.ID
'                        aOrd.Opt.Expiry = ReadDate(rsOrd!dtExpiry)
'                        aOrd.Opt.Strike = ReadDbl(rsOrd!fStrike)
'                        aOrd.Opt.OptType = ReadByte(rsOrd!tiIsCall)
'                    Else
'                        Set aOrd.OptRoot = Nothing
'                        Set aOrd.Opt = Nothing
'                    End If
'
'                    AdjustOptSeries aOrd.Opt, aOrd.OptRoot
'
'                    m_Ord.Add aOrd.ID, aOrd.OrderDate, aOrd
'                    bNewOrder = True
'                    bValidOrder = True
'                End If
'
'                Set aRoot = Nothing
'            End If
'
'            Set aUnd = Nothing
'        Else
'            bValidOrder = True
'        End If
'
'        If bValidOrder Then
'            aOrd.SeqNum = ReadLng(rsOrd!iSeqNum)
'            aOrd.UpdateDate = ReadDate(rsOrd!dtActionDate)
'            aOrd.Status = ReadLng(rsOrd!iStatus)
'            aOrd.IsBuy = (ReadByte(rsOrd!tiIsBuy) <> 0)
'            aOrd.OrderQty = ReadLng(rsOrd!iOrderQty)
'            aOrd.OrderPrice = ReadDbl(rsOrd!fOrderPrice)
'            aOrd.OrderDate = GmtToLocal(ReadDate(rsOrd!dtOrderDate))
'            aOrd.OrderID = ReadStr(rsOrd!vcClOrderID)
'            aOrd.AvgPrice = ReadDbl(rsOrd!fAvgPrice)
'            aOrd.CumQty = ReadLng(rsOrd!iCumQty)
'
'            If aOrd.SeqNum > m_nLastOrderSeqNum Then m_nLastOrderSeqNum = aOrd.SeqNum
'
'            If bNewOrder Then
'                RaiseEvent OnNewOrder(aOrd)
'            Else
'                RaiseEvent OnUpdateOrder(aOrd)
'            End If
'
'            Set aOrd = Nothing
'        End If
'
'        rsOrd.MoveNext
'    Wend
'    Set rsOrd = Nothing
'
'    While Not rsExec.EOF
'        bNewExec = False
'        nExecID = ReadLng(rsExec!iExecID)
'        Set aOrd = m_Ord(ReadLng(rsExec!iOrderID))
'        If Not aOrd Is Nothing Then
'            Set aExec = aOrd.Execs(nExecID)
'
'            If aExec Is Nothing Then
'                Set aExec = New EtsMmGeneralLib.MmOrderExecAtom
'                aExec.ID = ReadLng(rsExec!iExecID)
'                aOrd.Execs.Add aExec.ID, aExec.ExecDate, aExec
'                bNewExec = True
'            End If
'
'            aExec.SeqNum = ReadLng(rsExec!iSeqNum)
'            aExec.ExecStatus = ReadLng(rsExec!iExecStatus)
'            aExec.ExecQty = ReadLng(rsExec!iExecQty)
'            aExec.ExecPrice = ReadDbl(rsExec!fExecPrice)
'            aExec.ExecDate = GmtToLocal(ReadDate(rsExec!dtExecDate))
'            aExec.ExecID = ReadStr(rsExec!vcBrokerExecID)
'
'            If bNewExec Then
'                RaiseEvent OnNewOrderExec(aOrd, aExec)
'            Else
'                RaiseEvent OnUpdateOrderExec(aOrd, aExec)
'            End If
'
'            If aExec.SeqNum > m_nLastOrderExecSeqNum Then m_nLastOrderExecSeqNum = aExec.SeqNum
'
'            Set aExec = Nothing
'            Set aOrd = Nothing
'        End If
'
'        rsExec.MoveNext
'    Wend
'    Set rsExec = Nothing
'
'    Exit Sub
'EH:
'    AddEvent enEvtWarning, "Orders Processor: fail to load missed orders and order executions: " & vbCrLf & Err.Description
'    Set rsOrd = Nothing
'    Set rsExec = Nothing
'End Sub
'
