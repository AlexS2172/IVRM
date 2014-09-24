VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeChannel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Event TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
Public Event PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
Public Event PriceUpdate(aPrcData As MSGSTRUCTLib.PriceUpdate)
Public Event AssetUpdate(aUndData As MSGSTRUCTLib.UnderlyingUpdate)
Public Event ManualPriceUpdate(Data As MSGSTRUCTLib.ManualPriceUpdate)
Public Event PriceProfileUpdate()
Public Event GlobalParamsUpdate()
Public Event RoundingRuleUpdate()
Public Event ToleranceRangeUpdate()
Public Event HTBRatesUpdate(Data As MSGSTRUCTLib.UnderlyingUpdate)
Public Event UnderlyingUpdate(Data As MSGSTRUCTLib.UnderlyingUpdate)
Public Event DividendTypeUpdate(aUndData As MSGSTRUCTLib.UnderlyingUpdate)

Private WithEvents MmwMsgManager As MMWLib.MsgManager
Attribute MmwMsgManager.VB_VarHelpID = -1

Private m_Trd As EtsGeneralLib.MmTradeInfoColl            ' general collection of trades
Private m_TrdForMatch As EtsGeneralLib.MmTradeMatchColl   ' collection of trade referencies collections for matching
Private m_TrdByUndMan As EtsGeneralLib.MmTradeByUndColl      ' collection of manual trade referencies collections by "UnderlyingID_TraderID" key
Private m_TrdByUnd As EtsGeneralLib.MmTradeByUndColl       ' collection of trade referencies collections by "UnderlyingID" key
Private m_TrdByTraderAndContract As EtsGeneralLib.MmTradeByUndColl       ' collection of trade referencies collections by "TraderID_ContractID" key
Private m_TrdByFut As EtsGeneralLib.MmTradeByUndColl       ' collection of trade referencies collections by "FutureID" key

Private WithEvents m_TradeChannel As EtsGeneralLib.MmTradeChannel
Attribute m_TradeChannel.VB_VarHelpID = -1
Public Enum TradeActionEnum
    enTaTradeNew = 1
    enTaTradeUpdate = 2
    enTaTradeDelete = 3
End Enum

Private g_Progress As frmSplash
Private m_nLastSeqNum As Long
Private m_bCanProcessPriceMessages As Boolean
Private m_bCanProcessTradeMessages As Boolean

Private m_bIsTradesLoaded As Boolean
Private m_bIsTradesClone As Boolean

Private m_bManualTradeAction As Boolean
Private WithEvents m_tmrTradeUpdate As vB.Timer
Attribute m_tmrTradeUpdate.VB_VarHelpID = -1
Private m_TradeUpdateBuffer As EtsGeneralLib.MmTradeUpdateColl

Private Sub Class_Initialize()
    On Error Resume Next
    m_bCanProcessTradeMessages = False
    m_bCanProcessPriceMessages = False
    Set m_TradeChannel = New EtsGeneralLib.MmTradeChannel
   
    Set m_tmrTradeUpdate = frmMain.tmrTradeUpdate
    Set m_TradeUpdateBuffer = New EtsGeneralLib.MmTradeUpdateColl
    
    m_bIsTradesClone = False
    m_bIsTradesLoaded = False
    m_bManualTradeAction = False
End Sub

Public Sub InitMessaging()
    On Error GoTo EH
    
    If MmwMsgManager Is Nothing Then
        Dim aPubManager As MMWLib.IPubManager
        
        Set MmwMsgManager = New MMWLib.MsgManager
        m_bCanProcessTradeMessages = True
        m_bCanProcessPriceMessages = True
            
        Set aPubManager = MmwMsgManager
        aPubManager.RegPublisher enMtTradeUpdate, Nothing
        aPubManager.RegPublisher enMtPriceUpdate, Nothing
        aPubManager.RegPublisher enMtUnderlyingUpdate, Nothing
        aPubManager.RegPublisher enMtManualPriceUpdate, Nothing
        Set aPubManager = Nothing
    End If
        
    Exit Sub
EH:
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to initialize messaging service."
End Sub

Public Sub LoadTrades(Optional ByRef Progress As frmSplash = Nothing)
    On Error GoTo EH
    Dim rs As ADODB.Recordset, bShowProgress As Boolean, nTotal&, nDone&
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim enContractType As EtsGeneralLib.EtsContractTypeEnum
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    
    bShowProgress = Not Progress Is Nothing
    Set g_Progress = Progress
    
    If bShowProgress Then
        Progress.SetStatus "Loading trades data... 0%"
    End If
    
    Set m_Trd = m_TradeChannel.Trd
    Set m_TrdForMatch = m_TradeChannel.TrdForMatch
    Set m_TrdByUndMan = m_TradeChannel.TrdByUndMan
    Set m_TrdByUnd = m_TradeChannel.TrdByUnd
    Set m_TrdByTraderAndContract = m_TradeChannel.TrdByTraderAndContract
    Set m_TrdByFut = m_TradeChannel.TrdByFut
    
    m_TradeChannel.DatabaseString = g_Params.DbConnection
    Dim dt1 As Long, dt2 As Long
    dt1 = GetTickCount
    m_bIsTradesLoaded = True
    
    LoadLastSeqNum
    m_TradeChannel.LoadTrades g_CurTraderID, g_Main
    dt2 = GetTickCount
'    m_TrdForMatch.Clear
'    m_TrdByUndMan.Clear
'    m_TrdByUnd.Clear
'    m_TrdByTraderAndContract.Clear
'    m_TrdByFut.Clear
'    m_Trd.Clear
'
'    Set rs = gDBW.usp_MmTradeByTrader_Get(g_CurTraderID)
'    nDone = 0&
'    nTotal = rs.RecordCount
'    While Not rs.EOF
'        enContractType = ReadLng(rs!iContractTypeID)
'        If IsValidContractType(enContractType) Then
'            Set aUnd = g_Underlying(ReadLng(rs!iUnderlyingContractID))
'            Set aTrader = g_Trader(ReadLng(rs!iTraderID))
'            If Not aUnd Is Nothing And Not aTrader Is Nothing Then
'                Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
'                Set aTrd.Und = aUnd
'
'                aTrd.TradeID = ReadLng(rs!iTradeID)
'                aTrd.Price = ReadDbl(rs!fPrice)
'                aTrd.Quantity = ReadLng(rs!iQuantity)
'                aTrd.IsBuy = (ReadByte(rs!tiIsBuy) <> 0)
'                aTrd.TradeDate = GmtToLocal(ReadDate(rs!dtTradeDate))
'                aTrd.IsPosition = ReadByte(rs!tiIsPosition)
'                aTrd.Status = ReadByte(rs!tiStatus)
'                aTrd.BrokerCommission = ReadDbl(rs!fBrokerCommission)
'                aTrd.ClearingBrokerCommission = ReadDbl(rs!fClearingBrokerCommission)
'                aTrd.Mark = ReadLng(rs!iMark)
'
'                Set aTrd.Trader = aTrader
'                Set aTrd.Broker = g_Broker(ReadLng(rs!iBrokerID))
'                Set aTrd.ClearingBroker = g_ClearingBroker(ReadLng(rs!iClearingBrokerID))
'                Set aTrd.Strategy = g_Strategy(ReadLng(rs!iStrategyID))
'
'                Select Case enContractType
'                    Case enCtOption
'                        Set aTrd.OptRoot = aTrd.Und.Roots(ReadLng(rs!iOptionRootID))
'                        Set aTrd.Opt = New EtsGeneralLib.EtsOptAtom
'
'                        aTrd.Opt.ID = ReadLng(rs!iContractID)
'                        aTrd.Opt.Symbol = ReadStr(rs!vcSymbol)
'                        aTrd.Opt.RootID = aTrd.OptRoot.ID
'                        aTrd.Opt.OptType = ReadByte(rs!tiIsCall)
'                        aTrd.Opt.Expiry = ReadDate(rs!dtExpiry)
'                        aTrd.Opt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
'                        aTrd.Opt.PriceClose = ReadDbl(rs!fPriceClose)
'                        aTrd.Opt.PriceTheoClose = ReadDbl(rs!fPriceTheoClose)
'                        aTrd.Opt.Series = ReadStr(rs!vcContractName)
'                        aTrd.Opt.ImportID = ReadStr(rs!vcImportID)
'
'                        aTrd.SpotReference = ReadDbl(rs!fSpotReference)
'                        aTrd.TradedIV = ReadDbl(rs!fTradedIV)
'
'                    Case enCtFuture
'                        Set aTrd.FutRoot = aTrd.Und.FutRoots(ReadLng(rs!iFutureRootID))
'                        Set aTrd.Fut = aTrd.FutRoot.Futures(ReadLng(rs!iFutureID))
'
'                    Case enCtFutOption
'                        Set aTrd.FutRoot = aTrd.Und.FutRoots(ReadLng(rs!iFutureRootID))
'                        Set aTrd.Fut = aTrd.FutRoot.Futures(ReadLng(rs!iFutureID))
'                        Set aTrd.FutOpt = New EtsGeneralLib.EtsFutOptAtom
'
'                        aTrd.FutOpt.ID = ReadLng(rs!iContractID)
'                        aTrd.FutOpt.Symbol = ReadStr(rs!vcSymbol)
'                        aTrd.FutOpt.FutID = aTrd.Fut.ID
'                        aTrd.FutOpt.OptType = ReadByte(rs!tiIsCall)
'                        aTrd.FutOpt.Expiry = ReadDate(rs!dtExpiry)
'                        aTrd.FutOpt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
'                        aTrd.FutOpt.PriceClose = ReadDbl(rs!fPriceClose)
'                        aTrd.FutOpt.PriceTheoClose = ReadDbl(rs!fPriceTheoClose)
'                        aTrd.FutOpt.Series = ReadStr(rs!vcContractName)
'                        aTrd.FutOpt.ImportID = ReadStr(rs!vcImportID)
'
'                        aTrd.SpotReference = ReadDbl(rs!fSpotReference)
'                        aTrd.TradedIV = ReadDbl(rs!fTradedIV)
'
'                    Case Else ' enCtIndex, enCtStock
'                        If aUnd.TotalQtyInShares = BAD_LONG_VALUE Then aUnd.TotalQtyInShares = 0
'                        aUnd.TotalQtyInShares = aUnd.TotalQtyInShares + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
'
'                        If aTrd.Status <> enTrsSimulated Then
'                            If aUnd.PriceClose >= DBL_EPSILON Then
'                                aUnd.UndPosForRates = aUnd.UndPosForRates + aTrd.Quantity * aUnd.PriceClose * IIf(aTrd.IsBuy, 1, -1)
'                            End If
'                        End If
'                End Select
'
'                AdjustTraderContractAndStrategy aTrd.Trader.ID, aTrd.UndID, aTrd.StrategyID, False
'
'                m_Trd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
'
'                AddTradeToTradesByUnd aTrd
'                AddTradeToTradesByTraderAndContract aTrd
'                AddManualTradeToTradesByUndMan aTrd
'                AddTradeToTradesByFut aTrd
'
'                Set aTrd = Nothing
'            End If
'
'            Set aUnd = Nothing
'            Set aTrader = Nothing
'        Else
'            Debug.Assert False
'        End If
'
'        If bShowProgress Then
'            nDone = nDone + 1
'            lblProgress.Caption = "Loading trades data... " & CStr(nDone * 100 \ nTotal) & "% complete."
'            DoEvents
'        End If
'
'        rs.MoveNext
'    Wend
'
    Dim d1 As Long
    Dim d2 As Long
    
    d1 = dt2 - dt1
    d2 = GetTickCount - dt2
    FindAllActualTradesMatch
    
    If bShowProgress Then
        Progress.SetStatus "Loading trades data... 100% complete."
    End If
    Set g_Progress = Nothing
    
    m_tmrTradeUpdate.Interval = 1000 'g_Params.TradesLoadInterval
    m_tmrTradeUpdate.Enabled = True
    
    m_tmrTradeUpdate.Interval = 1000 'g_Params.TradesLoadInterval
    m_tmrTradeUpdate.Enabled = True
    
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to load trades. " & Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source

    Set rs = Nothing
    m_TrdForMatch.Clear
    m_TrdByUndMan.Clear
    m_TrdByUnd.Clear
    m_TrdByTraderAndContract.Clear
    m_TrdByFut.Clear
    m_Trd.Clear
    Set g_Progress = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Public Sub Start()
    On Error GoTo EH

    MmwMsgManager.SubTradeUpdate
    MmwMsgManager.SubBroadcastMessage
    MmwMsgManager.SubPriceUpdate
    MmwMsgManager.SubUnderlyingUpdate
    MmwMsgManager.SubManualPriceUpdate
    
    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to start real-time trades service."
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Dim aPubManager As MMWLib.IPubManager
    m_bCanProcessTradeMessages = False
    m_bCanProcessPriceMessages = False
    
    MmwMsgManager.UnsubBroadcastMessage
    MmwMsgManager.UnsubTradeUpdate
    MmwMsgManager.UnsubPriceUpdate
    MmwMsgManager.UnsubManualPriceUpdate
    
    Set aPubManager = MmwMsgManager
    aPubManager.UnregPublisher enMtTradeUpdate, Nothing
    aPubManager.UnregPublisher enMtPriceUpdate, Nothing
    aPubManager.UnregPublisher enMtUnderlyingUpdate, Nothing
    aPubManager.UnregPublisher enMtManualPriceUpdate, Nothing
    Set aPubManager = Nothing
    
    Set MmwMsgManager = Nothing
    
    Set m_tmrTradeUpdate = Nothing
    Set m_TradeUpdateBuffer = Nothing
End Sub

Public Property Get Trades() As EtsGeneralLib.MmTradeInfoColl
    On Error Resume Next
    Set Trades = m_Trd
End Property

Public Property Get TradeChannel() As EtsGeneralLib.MmTradeChannel
    On Error Resume Next
    Set TradeChannel = m_TradeChannel
End Property

Public Property Get TradesForMatch() As MmTradeMatchColl
    On Error Resume Next
    Set TradesForMatch = m_TrdForMatch

End Property

Public Property Get TradesByUnd() As MmTradeByUndColl
    On Error Resume Next
    Set TradesByUnd = m_TrdByUnd
End Property

Public Property Get TradesByFut() As MmTradeByUndColl
    On Error Resume Next
    Set TradesByFut = m_TrdByFut
End Property

Public Property Get TrdByTraderAndContract() As MmTradeByUndColl
    On Error Resume Next
    Set TrdByTraderAndContract = m_TrdByTraderAndContract
End Property

Public Sub PriceCloseForPub(ByVal ContractType As Long, ByVal ContractID As Long, ByVal UndID As Long, ByVal PriceClose As Double, ByVal PriceTheoclose As Double)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceCloseForPub Enter", "TradeChannel"
            
    Dim aPrcUpdate As MSGSTRUCTLib.PriceUpdate
    Dim aPubManager As MMWLib.IPubManager
    
    Set aPubManager = MmwMsgManager
    Set aPrcUpdate = New MSGSTRUCTLib.PriceUpdate
    
    aPrcUpdate.ContractType = ContractType
    
    If (ContractType = enCtIndex Or ContractType = enCtStock) Then
        aPrcUpdate.ContractID = UndID
    Else
        aPrcUpdate.ContractID = ContractID
    End If
    
    aPrcUpdate.UndID = UndID
    aPrcUpdate.PriceClose = PriceClose
    aPrcUpdate.TheoPriceClose = PriceTheoclose
    
    aPubManager.PubPriceUpdate aPrcUpdate
            
    Set aPubManager = Nothing
    
    RaiseEvent PriceUpdate(aPrcUpdate)
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceCloseForPub Exit", "TradeChannel"

End Sub

Private Sub CopyTradeForPub(aSrc As EtsGeneralLib.MmTradeInfoAtom, aDst As MSGSTRUCTLib.TradeUpdate)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "CopyTradeForPub Enter", "TradeChannel"
    
    Set aDst.Contract = g_Main.GetContract(aSrc.ContractID)
    
    aDst.SeqNum = aSrc.SeqNum
    aDst.TradeID = aSrc.TradeID
    aDst.TradeStatus = aSrc.Status
    aDst.Mark = aSrc.Mark
    aDst.TradeDate = LocalToGmt(aSrc.TradeDate)
    aDst.TraderID = aSrc.Trader.ID
    aDst.SymbolID = aSrc.ContractID
    aDst.Quantity = aSrc.Quantity
    aDst.price = aSrc.price
    aDst.IsBuy = aSrc.IsBuy
    aDst.IsPosition = aSrc.IsPosition
    aDst.OrderID = aSrc.OrderID
    aDst.ExecStatus = aSrc.ExecStatus
    
    If Not aSrc.Broker Is Nothing Then
        aDst.BrokerID = aSrc.Broker.ID
        aDst.BrokerCommission = aSrc.BrokerCommission
    End If
    
    If Not aSrc.ClearingBroker Is Nothing Then
        aDst.ClearingBrokerID = aSrc.ClearingBroker.ID
        aDst.ClearingBrokerCommission = aSrc.ClearingBrokerCommission
    End If
    
    If Not aSrc.Strategy Is Nothing Then aDst.strategyID = aSrc.Strategy.ID
    
    aDst.SymbolType = ContractTypeToSymbolType(aSrc.ContractType)
    aDst.Symbol = aSrc.Symbol
    aDst.UndSymbolID = aSrc.Und.ID
    aDst.undSymbol = aSrc.Und.Symbol
    aDst.LotSize = aSrc.LotSize
    aDst.IsPosition = aSrc.IsPosition
    
    Select Case aSrc.ContractType
        Case enCtOption
            aDst.OptionRootID = aSrc.OptRoot.ID
            
            aDst.Expiry = aSrc.Opt.Expiry
            aDst.ExpiryOV = aSrc.Opt.ExpiryOV
            aDst.TradingClose = aSrc.Opt.TradingClose
            aDst.IsCall = aSrc.Opt.OptType
            aDst.Strike = aSrc.Opt.Strike
            aDst.PriceClose = aSrc.Opt.PriceClose
            aDst.PriceTheoclose = aSrc.Opt.PriceTheoclose
            
            aDst.SpotReference = aSrc.SpotReference
            aDst.TradedIV = aSrc.TradedIV
            aDst.ImportID = aSrc.Opt.ImportID
            
        Case enCtFuture
            aDst.FutID = aSrc.Fut.ID
            aDst.FutRootID = aSrc.FutRoot.ID
            
            aDst.PriceClose = aSrc.Fut.PriceClose
            aDst.PriceTheoclose = aSrc.Fut.PriceTheoclose
        
        Case enCtFutOption
            aDst.FutID = aSrc.Fut.ID
            aDst.FutRootID = aSrc.FutRoot.ID
            
            aDst.Expiry = aSrc.FutOpt.Expiry
            aDst.ExpiryOV = aSrc.FutOpt.ExpiryOV
            aDst.TradingClose = aSrc.FutOpt.TradingClose
            aDst.IsCall = aSrc.FutOpt.OptType
            aDst.Strike = aSrc.FutOpt.Strike
            aDst.PriceClose = aSrc.FutOpt.PriceClose
            aDst.PriceTheoclose = aSrc.FutOpt.PriceTheoclose
           
            aDst.SpotReference = aSrc.SpotReference
            aDst.TradedIV = aSrc.TradedIV
            aDst.ImportID = aSrc.FutOpt.ImportID
            
        Case Else ' enCtIndex, enCtStock
            aDst.PriceClose = aSrc.Und.PriceClose
    End Select
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "CopyTradeForPub Exit", "TradeChannel"

End Sub

Public Function CopyTradeFromSub(aSrc As MSGSTRUCTLib.TradeUpdate, aDst As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "CopyTradeFromSub Enter", "TradeChannel"
    
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    Dim enContractType As EtsGeneralLib.EtsContractTypeEnum
    
    CopyTradeFromSub = False
    
    Set aUnd = g_Underlying(aSrc.UndSymbolID)
    Set aTrader = g_Trader(aSrc.TraderID)
    If Not aUnd Is Nothing And Not aTrader Is Nothing Then
        Set aDst.Und = aUnd
        
        Set aDst.Contract = g_Main.GetContract(aSrc.SymbolID)
        
        aDst.SeqNum = aSrc.SeqNum
        aDst.TradeID = aSrc.TradeID
        aDst.price = aSrc.price
        aDst.Quantity = aSrc.Quantity
        aDst.IsBuy = aSrc.IsBuy
        aDst.IsPosition = aSrc.IsPosition
        aDst.TradeDate = GmtToLocal(aSrc.TradeDate)
        aDst.Status = aSrc.TradeStatus
        aDst.BrokerCommission = aSrc.BrokerCommission
        aDst.ClearingBrokerCommission = aSrc.ClearingBrokerCommission
        aDst.Mark = aSrc.Mark
        aDst.OrderID = aSrc.OrderID
        aDst.ExecStatus = aSrc.ExecStatus
        
        Set aDst.Trader = aTrader
        Set aDst.Broker = g_Broker(aSrc.BrokerID)
        Set aDst.ClearingBroker = g_ClearingBroker(aSrc.ClearingBrokerID)
        Set aDst.Strategy = g_Strategy(aSrc.strategyID)
        
        enContractType = SymbolTypeToContractType(aSrc.SymbolType)
        

        Select Case enContractType
            Case enCtOption
                Set aDst.OptRoot = aDst.Und.Roots(aSrc.OptionRootID)
                Set aDst.Opt = New EtsGeneralLib.EtsOptAtom
                
                aDst.Opt.ID = aSrc.SymbolID
                aDst.Opt.Symbol = aSrc.Symbol
                aDst.Opt.RootID = aDst.OptRoot.ID
                aDst.Opt.OptType = aSrc.IsCall
                aDst.Opt.Expiry = aSrc.Expiry
                aDst.Opt.ExpiryOV = aSrc.ExpiryOV
                aDst.Opt.TradingClose = aSrc.TradingClose
                aDst.Opt.Strike = Round(aSrc.Strike, STRIKE_DECIMALS_COUNT)
                aDst.Opt.PriceClose = aSrc.PriceClose
                aDst.Opt.PriceTheoclose = aSrc.PriceTheoclose
                
                aDst.SpotReference = aSrc.SpotReference
                aDst.TradedIV = aSrc.TradedIV
                aDst.Opt.ImportID = aSrc.ImportID
                
                CopyTradeFromSub = True
                
            Case enCtFuture, enCtFutOption
                If AdjustUnderlyingFuturesData(aUnd, aSrc.FutRootID, aSrc.FutID) Then
                    Set aDst.FutRoot = aDst.Und.FutRoots(aSrc.FutRootID)
                    Set aDst.Fut = aDst.FutRoot.Futures(aSrc.FutID)
                
                    If enContractType = enCtFutOption Then
                        Set aDst.FutOpt = New EtsGeneralLib.EtsFutOptAtom
                        
                        aDst.FutOpt.ID = aSrc.SymbolID
                        aDst.FutOpt.Symbol = aSrc.Symbol
                        aDst.FutOpt.FutID = aDst.Fut.ID
                        aDst.FutOpt.OptType = aSrc.IsCall
                        aDst.FutOpt.Expiry = aSrc.Expiry
                        aDst.FutOpt.ExpiryOV = aSrc.ExpiryOV
                        aDst.FutOpt.TradingClose = aSrc.TradingClose
                        aDst.FutOpt.Strike = Round(aSrc.Strike, STRIKE_DECIMALS_COUNT)
                        aDst.FutOpt.PriceClose = aSrc.PriceClose
                        aDst.FutOpt.PriceTheoclose = aSrc.PriceTheoclose
                        
                        aDst.SpotReference = aSrc.SpotReference
                        aDst.TradedIV = aSrc.TradedIV
                        aDst.FutOpt.ImportID = aSrc.ImportID
                    End If
                
                    CopyTradeFromSub = True
                Else
                    AddEvent EVENT_WARNING, "New futures was added by trade feed. Please restart application."
                End If
            
            Case Else ' enCtIndex, enCtStock
                CopyTradeFromSub = True
        
        End Select
    Else
        AddEvent EVENT_WARNING, "New underlying '" & aSrc.undSymbol & "' was added by trade feed. Please restart application."
    End If
    
    Set aUnd = Nothing
    Set aTrader = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "CopyTradeFromSub Exit", "TradeChannel"

End Function

Private Sub AdjustTraderContractAndStrategy(ByVal nTraderID&, ByVal nUndID&, ByVal nStrategyID&, _
                                            Optional ByVal bFireEvents As Boolean = False)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AdjustTraderContractAndStrategy Enter", "TradeChannel"
    
    Dim aUnd As EtsGeneralLib.UndAtom, aTrader As EtsGeneralLib.EtsTraderAtom
    Dim aTrdUnd As EtsGeneralLib.EtsTraderUndAtom, aContract As EtsGeneralLib.EtsContractAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim bNewStrategy As Boolean
    
    Set aUnd = g_UnderlyingAll(nUndID)
    If Not aUnd Is Nothing Then
        
        aUnd.IsTraderContract = True
        
        Set aTrader = g_Trader(nTraderID)
        If Not aTrader Is Nothing Then
        
            bNewStrategy = LoadStrategy(nStrategyID, aStrategy)
            
            Set aTrdUnd = aTrader.Und(nUndID)
            If aTrdUnd Is Nothing Then
                Set aTrdUnd = aTrader.Und.Add(nUndID, aUnd.Symbol)
                Set aTrdUnd.Und = aUnd
            End If
            
            If Not aStrategy Is Nothing Then
                If aTrdUnd.Strategy(aStrategy.ID) Is Nothing Then _
                    aTrdUnd.Strategy.Add aStrategy.ID, aStrategy.Name, aStrategy
            End If
            
            Set aTrdUnd = Nothing
            
            Set aContract = g_ContractAll(aUnd.ID)
            If Not aContract Is Nothing Then
                If g_Contract(aUnd.ID) Is Nothing Then g_Contract.Add aContract.Und.ID, aContract.Und.Symbol, aContract
                Set aContract = Nothing
            End If
            
            If g_Underlying(aUnd.ID) Is Nothing Then
                g_Underlying.Add aUnd.ID, aUnd.Symbol, aUnd
                'g_UnderlyingBySym.Add aUnd.Symbol, aUnd
                
                g_Params.MakeUnderlingComboLists
                If bFireEvents Then g_Params.FireNewUnderlyingAdded aUnd.ID
            End If
            
            If bFireEvents And bNewStrategy And Not aStrategy Is Nothing Then g_Params.FireNewStrategyAdded aStrategy.ID
            
            Set aStrategy = Nothing
            Set aTrader = Nothing
        End If
        Set aUnd = Nothing
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AdjustTraderContractAndStrategy Exit", "TradeChannel"

End Sub

Private Function LoadStrategy(ByVal nStrategyID&, ByRef aStrategy As EtsGeneralLib.EtsStrategyAtom) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "LoadStrategy Enter", "TradeChannel"
    
    Dim rs As ADODB.Recordset
    
    LoadStrategy = False
    Set aStrategy = Nothing
    If nStrategyID = 0 Then Exit Function
    
    Set aStrategy = g_Strategy(nStrategyID)
    If aStrategy Is Nothing Then
        Set rs = gDBW.usp_Strategy_Get(nStrategyID)
        If Not rs.EOF Then
            Set aStrategy = New EtsGeneralLib.EtsStrategyAtom
            aStrategy.ID = nStrategyID
            aStrategy.Name = Trim$(ReadStr(rs!vcStrategyName))
            aStrategy.Desc = ReadStr(rs!vcDescription)
        
            g_Strategy.Add aStrategy.ID, aStrategy.Name, aStrategy
            
            LoadStrategy = True
        End If
        Set rs = Nothing
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "LoadStrategy Exit", "TradeChannel"
    
    Exit Function
EH:
    AddEvent EVENT_ERROR, "Fail to load new strategy for trade. " & Err.Description
    Set aStrategy = Nothing
    Set rs = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "LoadStrategy Exit with an Error", "TradeChannel"

End Function

Public Function NewTrade(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Enter", "TradeChannel"
    
    Dim aPubManager As MMWLib.IPubManager
    Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
    Dim aNewTrd As New EtsGeneralLib.MmTradeInfoAtom, nNewTradeID&, dtNewTradeDate As Date
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoAtom, sKey$
    Dim aUnd As EtsGeneralLib.UndAtom, aTrader As EtsGeneralLib.EtsTraderAtom
    Dim sImportID$
    
    NewTrade = False
    Screen.MousePointer = vbHourglass

    With aNewTrdInfo
        .SeqNum = gDBW.usp_Trade_New(.ContractID, .Quantity, .price, IIf(.IsBuy, 1, 0), _
                                .Trader.ID, .strategyID, .BrokerID, .BrokerCommission, .ClearingBrokerID, _
                                .ClearingBrokerCommission, .TradedIV, .SpotReference, 0, nNewTradeID, _
                                dtNewTradeDate, .Status, .Mark, .Und.ID, .OptRootID, _
                                .FutRootID, .FutID, sImportID, .OrderID, .ExecStatus)
        If .SeqNum <= 0 Then GoTo Ex
        .TradeID = nNewTradeID
        .TradeDate = GmtToLocal(dtNewTradeDate)
        
        If .ContractType = enCtFutOption Then
            .FutOpt.ImportID = sImportID
        ElseIf .ContractType = enCtOption Then
            .Opt.ImportID = sImportID
        End If
        
    End With
    
    AdjustTraderContractAndStrategy aNewTrdInfo.Trader.ID, aNewTrdInfo.Und.ID, aNewTrdInfo.strategyID
    
    aTrdUpdate.Action = enTeNew
    CopyTradeForPub aNewTrdInfo, aTrdUpdate
    
    Set aPubManager = MmwMsgManager
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aNewTrdInfo.CopyTo aNewTrd
    
    m_Trd.Add aNewTrd.TradeID, aNewTrd.TradeDate, aNewTrd
    
    AddTradeToTradesByUnd aNewTrd
    AddTradeToTradesByTraderAndContract aNewTrd
    AddTradeToTradesByFut aNewTrd
    
    If aNewTrd.Status = enTsManual Then
        AddManualTradeToTradesByUndMan aNewTrd
    
    ElseIf aNewTrd.Status = enTsRealtime Then
        FindActualTradeMatch aNewTrd
    End If
    
    On Error Resume Next
    
    
    If aNewTrd.SeqNum > m_nLastSeqNum + 1 Then
       ' reload missed trades
       LoadMissedTrades m_nLastSeqNum + 1, aNewTrd.SeqNum - 1
    End If
    m_nLastSeqNum = aNewTrd.SeqNum
    m_bManualTradeAction = True
    
    Debug.Print "NewTrade: " & m_nLastSeqNum
    
    RaiseTradeAction aNewTrd, Nothing, enTaTradeNew
    NewTrade = True
Ex:
    Set aPubManager = Nothing
    Screen.MousePointer = vbDefault
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Exit", "TradeChannel"
    
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to create new trade"
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Exit with an error", "TradeChannel"

End Function

Public Function DeleteTrade(aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Exit DeleteTrade Enter", "TradeChannel"
    
    Dim aPubManager As MMWLib.IPubManager
    Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
    Dim aOldTrd As New EtsGeneralLib.MmTradeInfoAtom
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoAtom, sKey$
    
    DeleteTrade = False
    Screen.MousePointer = vbHourglass
    
    With aOldTrdInfo
        .SeqNum = gDBW.usp_Trade_Del(.TradeID)
        If .SeqNum <= 0 Then GoTo Ex
    End With
    
    aTrdUpdate.Action = enTeDelete
    CopyTradeForPub aOldTrdInfo, aTrdUpdate
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "DeleteTrade PubTradeUpdate call", "TradeChannel"
        
    Set aPubManager = MmwMsgManager
    aPubManager.PubTradeUpdate aTrdUpdate
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "DeleteTrade PubTradeUpdate end", "TradeChannel"
    
    aOldTrdInfo.CopyTo aOldTrd
    
    m_Trd.Remove aOldTrd.TradeID
    
    RemoveTradeFromTradesByUnd aOldTrd
    RemoveTradeFromTradesByTraderAndContract aOldTrd
    RemoveTradeFromTradesByFut aOldTrd
    
    If aOldTrd.Status = enTsManual Then
        RemoveManualTradeFromTradesByUndMan aOldTrd
        RemoveManualTradeFromMatch aOldTrd
    
    ElseIf aOldTrd.Status = enTsRealtime And Not m_TrdForMatch.Item(aOldTrd.TradeID) Is Nothing Then
        m_TrdForMatch.Remove aOldTrd.TradeID
    End If
    
    On Error Resume Next
    If aOldTrd.SeqNum > m_nLastSeqNum + 1 Then
       ' reload missed trades
       LoadMissedTrades m_nLastSeqNum + 1, aOldTrd.SeqNum - 1
    End If
   
    m_nLastSeqNum = aOldTrd.SeqNum
    m_bManualTradeAction = True
    
    RaiseTradeAction Nothing, aOldTrd, enTaTradeDelete
    DeleteTrade = True
Ex:
    Set aPubManager = Nothing
    Screen.MousePointer = vbDefault
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Exit DeleteTrade Exit", "TradeChannel"
    
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to delete trade"
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "NewTrade Exit DeleteTrade Exit with errors", "TradeChannel"

End Function
Public Sub UpdateTradeIV(aTrd As EtsGeneralLib.MmTradeInfoAtom)
On Error GoTo EH
    Screen.MousePointer = vbHourglass
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogInformation, "UpdateTradeIV: IV" & CStr(aTrd.TradedIV) & " Trade ID:" & CStr(aTrd.TradeID), "TradeChannel"

    gDBW.usp_Trade_UpdateIV aTrd.TradeID, aTrd.TradedIV
    Screen.MousePointer = vbDefault
Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorHandler "Fail to update Trade IV"
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "UpdateTradeIV Exit with error " & CStr(Err.Number) & "Description: " & Err.Description, "TradeChannel"

End Sub

Public Function UpdateTrade(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "UpdateTrade Enter", "TradeChannel"
    
    Dim aPubManager As MMWLib.IPubManager
    Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
    Dim aNewTrd As New EtsGeneralLib.MmTradeInfoAtom, aOldTrd As New EtsGeneralLib.MmTradeInfoAtom, nNewTradeID&, dtNewTradeDate As Date
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoAtom, sKey$, sImportID$
    
    
    Screen.MousePointer = vbHourglass
    UpdateTrade = False
    
    If Not aOldTrdInfo Is Nothing Then
        dtNewTradeDate = LocalToGmt(aOldTrdInfo.TradeDate)
     End If
     
    With aNewTrdInfo
        .SeqNum = gDBW.usp_Trade_Update(aOldTrdInfo.TradeID, .ContractID, .Quantity, .price, IIf(.IsBuy, 1, 0), _
                                .Trader.ID, .strategyID, .BrokerID, .BrokerCommission, .ClearingBrokerID, _
                                .ClearingBrokerCommission, .TradedIV, .SpotReference, .IsPosition, nNewTradeID, _
                                dtNewTradeDate, .Status, .Mark, .Und.ID, .OptRootID, _
                                .FutRootID, .FutID, sImportID, .OrderID, .ExecStatus)
        If .SeqNum <= 0 Then GoTo Ex
        .TradeID = nNewTradeID
        .TradeDate = GmtToLocal(dtNewTradeDate)
        
        If .ContractType = enCtFutOption Then
            .FutOpt.ImportID = sImportID
        ElseIf .ContractType = enCtOption Then
            .Opt.ImportID = sImportID
        End If
        
        aOldTrdInfo.SeqNum = .SeqNum - 1
    End With
    
    AdjustTraderContractAndStrategy aNewTrdInfo.Trader.ID, aNewTrdInfo.Und.ID, aNewTrdInfo.strategyID
    
    Set aPubManager = MmwMsgManager
    
    aTrdUpdate.Action = enTeDelete
    CopyTradeForPub aOldTrdInfo, aTrdUpdate
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aTrdUpdate.Action = enTeNew
    CopyTradeForPub aNewTrdInfo, aTrdUpdate
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aOldTrdInfo.CopyTo aOldTrd
    m_Trd.Remove aOldTrd.TradeID
    
    RemoveTradeFromTradesByUnd aOldTrd
    RemoveTradeFromTradesByTraderAndContract aOldTrd
    RemoveTradeFromTradesByFut aOldTrd
    
    If aOldTrd.Status = enTsManual Then
        RemoveManualTradeFromTradesByUndMan aOldTrd
        RemoveManualTradeFromMatch aOldTrd
        
    ElseIf aOldTrd.Status = enTsRealtime And Not m_TrdForMatch(aOldTrd.TradeID) Is Nothing Then
        m_TrdForMatch.Remove aOldTrd.TradeID
    End If
    
    aNewTrdInfo.CopyTo aNewTrd
    m_Trd.Add aNewTrd.TradeID, aNewTrd.TradeDate, aNewTrd
    
    AddTradeToTradesByUnd aNewTrd
    AddTradeToTradesByTraderAndContract aNewTrd
    AddTradeToTradesByFut aNewTrd
    
    If aNewTrd.Status = enTsManual Then
        AddManualTradeToTradesByUndMan aNewTrd
        
    ElseIf aNewTrd.Status = enTsRealtime Then
        FindActualTradeMatch aNewTrd
    End If
    
    On Error Resume Next
    If aNewTrd.SeqNum > m_nLastSeqNum + 1 Then
       ' reload missed trades
       LoadMissedTrades m_nLastSeqNum + 1, aNewTrd.SeqNum - 1
    End If
    
    m_nLastSeqNum = aNewTrd.SeqNum
    m_bManualTradeAction = True
    
    RaiseTradeAction aNewTrd, aOldTrd, enTaTradeUpdate
    UpdateTrade = True
Ex:
    Set aPubManager = Nothing
    Screen.MousePointer = vbDefault
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "UpdateTrade Exit", "TradeChannel"
    
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to update trade"
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "UpdateTrade Exit with errors " + CStr(aNewTrdInfo.SeqNum), "TradeChannel"

End Function

Public Function TransferPosition(ByRef aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, _
                                ByRef aTrdTo As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Transfer position Enter", "TradeChannel"
    
    Dim aPubManager As MMWLib.IPubManager
    Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
    Dim nSeqNumFrom&, nTradeIDFrom&, dtTradeDateFrom As Date
    Dim nSeqNumTo&, nTradeIDTo&, dtTradeDateTo As Date
    Dim aTrdFromCopy As New EtsGeneralLib.MmTradeInfoAtom, aTrdToCopy As New EtsGeneralLib.MmTradeInfoAtom
    
    If aTrdFrom Is Nothing Or aTrdTo Is Nothing Then
        Debug.Assert False
        GoTo Ex
    End If
    
    If aTrdFrom.Status <> enTrsRealtime Or aTrdTo.Status <> enTrsRealtime Then
        Debug.Assert False
        GoTo Ex
    End If
    
    TransferPosition = False
    Screen.MousePointer = vbHourglass
    
    If 0 <> gDBW.usp_Trade_PositionTransfer(aTrdFrom.ContractID, aTrdFrom.Quantity, aTrdFrom.price, aTrdFrom.Status, _
                aTrdFrom.Trader.ID, aTrdFrom.strategyID, aTrdFrom.Mark, IIf(aTrdFrom.IsBuy, 1, 0), _
                aTrdTo.Trader.ID, aTrdTo.strategyID, aTrdTo.Mark, IIf(aTrdTo.IsBuy, 1, 0), _
                nSeqNumFrom, nTradeIDFrom, dtTradeDateFrom, _
                nSeqNumTo, nTradeIDTo, dtTradeDateTo, _
                aTrdFrom.UndID, aTrdFrom.OptRootID, aTrdFrom.FutRootID, aTrdFrom.FutID) Then GoTo Ex
        
    ' trade 'from'
    aTrdFrom.SeqNum = nSeqNumFrom
    aTrdFrom.TradeID = nTradeIDFrom
    aTrdFrom.TradeDate = GmtToLocal(dtTradeDateFrom)
    
    AdjustTraderContractAndStrategy aTrdFrom.Trader.ID, aTrdFrom.Und.ID, aTrdFrom.strategyID
    
    aTrdUpdate.Action = enTeNew
    CopyTradeForPub aTrdFrom, aTrdUpdate
    
    Set aPubManager = MmwMsgManager
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aTrdFrom.CopyTo aTrdFromCopy
    
    m_Trd.Add aTrdFromCopy.TradeID, aTrdFromCopy.TradeDate, aTrdFromCopy
    
    AddTradeToTradesByUnd aTrdFromCopy
    AddTradeToTradesByTraderAndContract aTrdFromCopy
    AddTradeToTradesByFut aTrdFromCopy
    
    FindActualTradeMatch aTrdFromCopy
    
    ' trade 'to'
    aTrdTo.SeqNum = nSeqNumTo
    aTrdTo.TradeID = nTradeIDTo
    aTrdTo.TradeDate = GmtToLocal(dtTradeDateTo)
    
    AdjustTraderContractAndStrategy aTrdTo.Trader.ID, aTrdTo.Und.ID, aTrdTo.strategyID
    
    aTrdUpdate.Action = enTeNew
    CopyTradeForPub aTrdTo, aTrdUpdate
    
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aTrdTo.CopyTo aTrdToCopy
    
    m_Trd.Add aTrdToCopy.TradeID, aTrdToCopy.TradeDate, aTrdToCopy
    
    AddTradeToTradesByUnd aTrdToCopy
    AddTradeToTradesByTraderAndContract aTrdToCopy
    AddTradeToTradesByFut aTrdToCopy
    
    FindActualTradeMatch aTrdToCopy
  
    On Error Resume Next
    m_nLastSeqNum = aTrdToCopy.SeqNum
    RaiseEvent PositionTransfer(aTrdFromCopy, aTrdToCopy)
    TransferPosition = True
Ex:
    Set aPubManager = Nothing
    Screen.MousePointer = vbDefault
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Transfer position Exit", "TradeChannel"
    
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to transfer position."
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Transfer position Exit with errors", "TradeChannel"

End Function

Private Sub m_tmrTradeUpdate_Timer()
    On Error Resume Next
    Dim aTempBuffer As EtsGeneralLib.MmTradeUpdateColl

    If m_TradeUpdateBuffer.Count = 0 Then Exit Sub

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "m_tmrTradeUpdate_Timer Enter: " & m_TradeUpdateBuffer.Count, "TradeChannel"
        
    m_tmrTradeUpdate.Enabled = False
        
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enLogDebug, "m_tmrTradeUpdate_Timer Enter.", "TradeChannel"
'
'    m_tmrTradeUpdate.Enabled = False
'
'    LoadMissedTrades m_nLastSeqNum + 1, 0
'
'    m_tmrTradeUpdate.Enabled = True
'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    
    If m_bIsTradesClone Then
        Sleep 0
        DoEvents
    End If

    ' copy buffer
    m_bIsTradesClone = True
    Set aTempBuffer = m_TradeUpdateBuffer.Clone
    m_TradeUpdateBuffer.Clear
    m_bIsTradesClone = False

    ' process trades
    Dim aTradeInfo As MSGSTRUCTLib.ITradeUpdate
    For Each aTradeInfo In aTempBuffer
        DoTradeUpdate aTradeInfo
    Next

    m_tmrTradeUpdate.Enabled = True

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "m_tmrTradeUpdate_Timer Exit.", "TradeChannel"
End Sub

Private Sub m_TradeChannel_Notification(ByVal bsText As String, ByVal lPercent As Long)
    On Error Resume Next
    On Error Resume Next
    If Not g_Progress Is Nothing Then
        If lPercent >= 0 Then
               g_Progress.Caption = bsText & " " & CStr(lPercent) & "%"
        Else
               g_Progress.Caption = bsText
        End If
        g_Progress.Refresh
    End If
End Sub

Private Sub MmwMsgManager_OnBroadcastMessage(ByVal Data As MSGSTRUCTLib.IBroadcastMessage)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "MmwMsgManager_OnBroadcastMessage Enter : " & Data.Description, "TradeChannel"
    
    Dim enEvType As EventTypeEnum, sMessage$, enMsgBoxStyle As VbMsgBoxStyle
    
    Select Case Data.Severity
        Case enSevError
            enEvType = EVENT_ERROR
            enMsgBoxStyle = vbCritical
            
        Case enSevWarning
            enEvType = EVENT_WARNING
            enMsgBoxStyle = vbExclamation
            
        Case Else
            enEvType = EVENT_INFO
            enMsgBoxStyle = vbInformation
    End Select
    
    Select Case Data.Facility
        Case enFacRTTF
            sMessage = "RTTF: "
            
        Case enFacETSMM
            sMessage = "IVRm: "
        
        Case enFacBO
            sMessage = "Back Office: "
        
        Case enFacETSFL
            sMessage = "EtsFL: "
        
        Case enFacVolaMan
            sMessage = "Volatility Manager: "
            
        Case enFacIVRMDataLoader
            sMessage = "IVRMDataUploader: "
    End Select
    
    sMessage = sMessage & Data.Description & " (code:" & Trim$(Str$(Data.Code)) & ")"
    
'    Select Case Data.Priority
'        Case enPrHigh
'            gCmn.MyMsgBox frmMain, sMessage, enMsgBoxStyle
'        Case Else
'    End Select
    AddEvent enEvType, sMessage
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "MmwMsgManager_OnBroadcastMessage Exit : " & Data.Description, "TradeChannel"
    
End Sub
Public Sub CalcTradedIV(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nDivCount&, RetCount&
    Dim dDivDte() As Double, aBaskDivs() As REGULAR_DIVIDENDS, nBaskDivCount&
    Dim dDivAmts() As Double, aIdx As EtsGeneralLib.IndexAtom
    Dim dRate#, dHTBRate#, aUnd As EtsGeneralLib.UndAtom, dYield#
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aBasketDivs As EtsGeneralLib.EtsIndexDivColl
    Dim enDivType As EtsGeneralLib.EtsDivTypeEnum
    Dim nFlag As Long
    Dim dtNow As Date, dYte As Double
        
    dtNow = GetNewYorkTime
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    dYield = 0#
    
    If aTrd.ContractType = enCtOption Then
        aTrd.TradedIV = 0#
        If aTrd.SpotReference > 0 And aTrd.price > 0 Then
            nDivCount = 0
            ReDim dDivDte(0 To 0)
            ReDim dDivAmts(0 To 0)
            
            If Not aTrd.Und.HaveSyntheticRoots Or aTrd.Und.SyntheticRoots(aTrd.OptRoot.ID) Is Nothing Then
                
                Set aUnd = aTrd.Und
                Set aDiv = aUnd.Dividend
                
                enDivType = enDivCustomStream
                If (Not aDiv Is Nothing) Then
                    enDivType = aDiv.DivType
                End If
                
                dRate = GetNeutralRate(dtNow, aTrd.Opt.ExpiryOV)
                If IsHTBRatesExist(aTrd.UndID, False) Then
                    dHTBRate = GetHTBRate(aTrd.UndID, dtNow, aTrd.Opt.ExpiryOV)
                Else
                    dHTBRate = BAD_DOUBLE_VALUE
                End If
                
                GetCalcParams dtNow, aTrd.Opt.ExpiryOV, aTrd.Opt.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
                
                Select Case enDivType
                        Case enDivMarket, enDivCustomPeriodical, enDivCustomStream
                            If Not aDiv Is Nothing Then
                                aDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                                If nDivCount > 0 Then
                                    aDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                                End If
                                Set aDiv = Nothing
                            End If
                        Case enDivStockBasket
                            If Not aUnd.BasketIndex Is Nothing Then
                                Set aBasketDivs = aUnd.BasketIndex.BasketDivs
                                If Not aBasketDivs Is Nothing Then
                                    aBasketDivs.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                                    If nDivCount > 0 Then
                                            aBasketDivs.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                                    End If
                                End If
                                Set aBasketDivs = Nothing
                            End If
                        Case enDivIndexYield
                            dYield = aUnd.Yield
                End Select
                
                aTrd.TradedIV = CalcVolatilityMM3(dRate, aTrd.Und.Yield, dHTBRate, aTrd.SpotReference, aTrd.price, aTrd.Opt.Strike, dYte, _
                                        aTrd.Opt.OptType, IIf(aTrd.Und.IsAmerican, 1, 0), nDivCount, dDivAmts(0), dDivDte(0), 100, 0#, 0#, IIf(g_Params.CalcModel = enCmBinomial, enCmBinomial, enCmBinomialOpt), nFlag)
                
                Erase dDivDte
                Erase dDivAmts
                
                Err.Clear
                aTrd.TradedIV = aTrd.TradedIV + 0
                If Err.Number <> 0 Then
                    aTrd.TradedIV = 0#
                ElseIf aTrd.TradedIV < 0 Then
                    aTrd.TradedIV = 0#
                End If
                
            Else ' synthetic underlyings
                
                Set aSynthRoot = aTrd.Und.SyntheticRoots(aTrd.OptRoot.ID)
                
                dRate = GetNeutralRate(dtNow, aTrd.Opt.ExpiryOV)
                If IsHTBRatesExist(aTrd.UndID, False) Then
                    dHTBRate = GetHTBRate(aTrd.UndID, dtNow, aTrd.Opt.ExpiryOV)
                Else
                    dHTBRate = BAD_DOUBLE_VALUE
                End If
                
                GetCalcParams dtNow, aTrd.Opt.ExpiryOV, aTrd.Opt.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
                
                If aSynthRoot.Basket Then
                
                    Dim aBasketDiv As EtsGeneralLib.EtsIndexDivColl
                    Set aBasketDiv = aSynthRoot.BasketDivs
                    
                    If Not aBasketDiv Is Nothing Then
                        aBasketDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount
                        If nBaskDivCount > 0 Then _
                            aBasketDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount, dDivAmts, dDivDte, nDivCount
                    End If
                    Set aBasketDiv = Nothing
                
                    Erase aBaskDivs
                End If
            
                If nDivCount <= 0 Then
                    dYield = aSynthRoot.Yield
                End If
                
                aTrd.TradedIV = CalcVolatilityMM3(dRate, dYield, dHTBRate, aTrd.SpotReference, aTrd.price, aTrd.Opt.Strike, dYte, _
                                        aTrd.Opt.OptType, IIf(aTrd.Und.IsAmerican, 1, 0), nDivCount, dDivAmts(0), dDivDte(0), 100, 0#, 0#, IIf(g_Params.CalcModel = enCmBinomial, enCmBinomial, enCmBinomialOpt), nFlag)
                
                Erase dDivDte
                Erase dDivAmts
                
                Err.Clear
                aTrd.TradedIV = aTrd.TradedIV + 0
                If Err.Number <> 0 Then
                    aTrd.TradedIV = 0#
                ElseIf aTrd.TradedIV < 0 Then
                    aTrd.TradedIV = 0#
                End If
            End If
        End If
    
    ElseIf aTrd.ContractType = enCtFutOption Then
        aTrd.TradedIV = 0#
        If aTrd.SpotReference > 0 And aTrd.price > 0 Then
            nDivCount = 0
            ReDim dDivDte(0 To 0)
            ReDim dDivAmts(0 To 0)
            
            dRate = GetNeutralRate(dtNow, aTrd.FutOpt.ExpiryOV)
            If IsHTBRatesExist(aTrd.UndID, False) Then
                dHTBRate = GetHTBRate(aTrd.UndID, dtNow, aTrd.FutOpt.ExpiryOV)
            Else
                dHTBRate = BAD_DOUBLE_VALUE
            End If
            
            GetCalcParams dtNow, aTrd.FutOpt.ExpiryOV, aTrd.FutOpt.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
            
            aTrd.TradedIV = CalcVolatilityMM3(dRate, dRate, dHTBRate, aTrd.SpotReference, aTrd.price, aTrd.FutOpt.Strike, dYte, _
                                    aTrd.FutOpt.OptType, 1, nDivCount, dDivAmts(0), dDivDte(0), 100, 0#, 0#, IIf(g_Params.CalcModel = enCmBinomial, enCmBinomial, enCmBinomialOpt), nFlag)
            
            Erase dDivDte
            Erase dDivAmts
            
            Err.Clear
            aTrd.TradedIV = aTrd.TradedIV + 0
            If Err.Number <> 0 Then
                aTrd.TradedIV = 0#
            ElseIf aTrd.TradedIV < 0 Then
                aTrd.TradedIV = 0#
            End If
        End If
    End If
End Sub

Private Sub MmwMsgManager_OnManualPriceUpdate(ByVal Data As MSGSTRUCTLib.IManualPriceUpdate)
On Error GoTo Exception
    
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim aFut As EtsGeneralLib.EtsFutAtom
    Dim aFutRoot As EtsGeneralLib.EtsFutRootAtom
    Dim bFound As Boolean
    

    If (Data.ContractType = enCtFuture) Then
    
        Set aUnd = g_UnderlyingAll(Data.UndID)
        If (Not aUnd Is Nothing) Then
        
            Set aFut = aUnd.ActiveFuture
            If (Not aFut Is Nothing) Then
                If (aFut.ID = Data.ContractID) Then
                    If (Data.Status = enUsAdd Or Data.Status = enUsUpdate) Then
                        aFut.manualActivePrice = Data.ActivePrice
                    Else
                        aFut.manualActivePrice = 0#
                    End If
                End If
                Set aFut = Nothing
            End If
                        
            bFound = False
            If (Not aUnd.FutRoots Is Nothing) Then
                For Each aFutRoot In aUnd.FutRoots
                    For Each aFut In aFutRoot.Futures
                        If (aFut.ID = Data.ContractID) Then
                            If (Data.Status = enUsAdd Or Data.Status = enUsUpdate) Then
                                aFut.manualActivePrice = Data.ActivePrice
                            Else
                                aFut.manualActivePrice = 0#
                            End If
                            bFound = True
                            If (bFound) Then Exit For
                        End If
                    Next
                    If (bFound) Then Exit For
                Next
                Set aFut = Nothing
            End If
            
        End If
        Set aUnd = Nothing
        
    ElseIf (Data.ContractType = enCtIndex Or Data.ContractType = enCtStock) Then
    
        Set aUnd = g_UnderlyingAll(Data.UndID)
        If (Not aUnd Is Nothing) Then
            If (Data.Status = enUsAdd Or Data.Status = enUsUpdate) Then
                aUnd.manualActivePrice = Data.ActivePrice
            Else
                aUnd.manualActivePrice = 0#
            End If
        End If
        Set aUnd = Nothing
        
    End If
    
    RaiseEvent ManualPriceUpdate(Data)
    
    Exit Sub
Exception:
    Debug.Print "Error while trying to update global collection: TradeChannel.cls"
End Sub

Private Sub MmwMsgManager_OnPriceUpdate(ByVal Data As MSGSTRUCTLib.IPriceUpdate)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aTrdColl As EtsGeneralLib.MmTradeInfoColl
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "MmwMsgManager_OnPriceUpdate Enter ", "TradeChannel"
        
    If Not m_bCanProcessPriceMessages Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogInformation, "MmwMsgManager_OnPriceUpdate Exit - Can't process ", "TradeChannel"
        Exit Sub
    End If
    
    Select Case Data.ContractType
        Case enCtOption, enCtFutOption
            Set aTrdColl = m_TrdByUnd(Data.UndID)
            If Not aTrdColl Is Nothing Then
                For Each aTrd In aTrdColl
                    If aTrd.ContractID = Data.ContractID Then
                        aTrd.Opt.PriceClose = Data.PriceClose
                        aTrd.Opt.PriceTheoclose = Data.TheoPriceClose
                    End If
                    Set aTrd = Nothing
                Next
            End If
    
        Case enCtFuture
            Set aTrdColl = m_TrdByFut(Data.ContractID)
            If Not aTrdColl Is Nothing Then
                For Each aTrd In aTrdColl
                    If aTrd.ContractID = Data.ContractID Then
                        aTrd.Fut.PriceClose = Data.PriceClose
                        aTrd.Fut.PriceTheoclose = Data.TheoPriceClose
                    End If
                    Set aTrd = Nothing
                Next
            End If
            
        Case Else
            Set aUnd = g_Underlying(Data.UndID)
            If Not aUnd Is Nothing Then
                aUnd.PriceClose = Data.PriceClose
                aUnd.PriceTheoclose = Data.TheoPriceClose
            End If
            
    End Select
    
    RaiseEvent PriceUpdate(Data)
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "MmwMsgManager_OnPriceCloseUpdate Exit ", "TradeChannel"
        
End Sub
Private Sub MmwMsgManager_OnTradeUpdate(ByVal Data As MSGSTRUCTLib.ITradeUpdate)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "MmwMsgManager_OnTradeUpdate Enter : " & Data.SeqNum, "TradeChannel"
    
    If Not m_bCanProcessTradeMessages Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogInformation, "MmwMsgManager_OnTradeUpdate Exit - Can't process : " & Data.SeqNum, "TradeChannel"
        Exit Sub
    End If
    
    If Not m_bIsTradesLoaded Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogInformation, "MmwMsgManager_OnTradeUpdate Exit - Trades Not Loaded.", "TradeChannel"
        Exit Sub
    End If
    
    
    If m_bIsTradesClone Then
        Sleep 0
        DoEvents
    End If

    m_bIsTradesClone = True
    m_TradeUpdateBuffer.Add Data.SeqNum, Data
    m_bIsTradesClone = False

    
'    DoTradeUpdate Data
    
    
'    Dim nSeqNum&, aTrd As EtsGeneralLib.MmTradeInfoAtom, aAction As TRADE_ACTION
'    Dim tmpTradedIV As Double
'
'    aAction = Data.Action
'
'    If Not g_PerformanceLog Is Nothing Then
'        If aAction = enTeNew Then
'            g_PerformanceLog.LogMmInfo enLogUserAction, "New trade Enter: " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
'        Else
'            If aAction = enTeDelete Then
'                g_PerformanceLog.LogMmInfo enLogUserAction, "Delete Trade: " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
'            Else
'                g_PerformanceLog.LogMmInfo enLogUserAction, "Unknown operation" & CStr(aAction) & " : " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
'            End If
'        End If
'    End If
'
'
'    If aAction = enTeNew Or aAction = enTeDelete Then
'
'        nSeqNum = Data.SeqNum
'
'        If nSeqNum > m_nLastSeqNum Then
'
'            If nSeqNum > m_nLastSeqNum + 1 Then
'                ' reload missed trades
'                LoadMissedTrades m_nLastSeqNum + 1, nSeqNum - 1
'            End If
'
'            m_nLastSeqNum = nSeqNum
'
'            If g_CurTraderID = 0 And Not g_Trader(Data.TraderID) Is Nothing Or g_CurTraderID = Data.TraderID Then
'
'                If Not g_UnderlyingAll(Data.UndSymbolID) Is Nothing Then
'
'                    Select Case aAction
'                        Case enTeNew
'                            If m_Trd(aTrd.TradeID) Is Nothing Then
'                                If IsValidSymbolType(Data.SymbolType) Then
'                                    AdjustTraderContractAndStrategy Data.TraderID, Data.UndSymbolID, Data.StrategyID
'
'                                    Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
'                                    If CopyTradeFromSub(Data, aTrd) Then
'                                        If Data.MsState <> enTmssNoChanges Then
'                                            AddEvent EVENT_WARNING, "Underlying '" & aTrd.Und.Symbol & _
'                                                                    "' has market structure changes." & _
'                                                                    " Please update market structure."
'                                        If Not g_PerformanceLog Is Nothing Then _
'                                            g_PerformanceLog.LogMmInfo enLogWarning, "Underlying '" & aTrd.Und.Symbol & _
'                                                                    "' has market structure changes." & _
'                                                                    " Please update market structure.", "TradeChannel"
'
'                                        End If
'
'                                        If aTrd.ContractType = enCtOption And aTrd.TradedIV <= 0# Then
'                                            tmpTradedIV = aTrd.TradedIV
'                                            CalcTradedIV aTrd
'                                            If aTrd.TradedIV <> tmpTradedIV And aTrd.TradedIV > 0# Then _
'                                                UpdateTradeIV aTrd
'                                        End If
'                                        m_Trd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
'
'
'                                        AddTradeToTradesByUnd aTrd
'                                        AddTradeToTradesByTraderAndContract aTrd
'                                        AddTradeToTradesByFut aTrd
'
'                                        If aTrd.Status = enTsManual Then
'                                            AddManualTradeToTradesByUndMan aTrd
'
'                                        ElseIf aTrd.Status = enTsRealtime Then
'                                            FindActualTradeMatch aTrd
'                                        End If
'
'                                        RaiseTradeAction aTrd, Nothing, enTaTradeNew
'                                    End If
'                                Else
'                                    Debug.Assert False
'                                End If
'                            Else
'                                Debug.Assert False
'                            End If
'
'                        Case enTeDelete
'                            Set aTrd = m_Trd(Data.TradeID)
'
'                            If Not aTrd Is Nothing Then
'                                m_Trd.Remove aTrd.TradeID
'
'                                RemoveTradeFromTradesByUnd aTrd
'                                RemoveTradeFromTradesByTraderAndContract aTrd
'                                RemoveTradeFromTradesByFut aTrd
'
'                                If aTrd.Status = enTsManual Then
'                                    RemoveManualTradeFromTradesByUndMan aTrd
'                                    RemoveManualTradeFromMatch aTrd
'
'                                ElseIf aTrd.Status = enTsRealtime Then
'                                    m_TrdForMatch.Remove aTrd.TradeID
'                                End If
'
'                                RaiseTradeAction Nothing, aTrd, enTaTradeDelete
'                            Else
'                                Debug.Assert False
'                            End If
'                    End Select
'
'                    Set aTrd = Nothing
'
'                ElseIf aAction = enTeNew Then
'                    AddEvent EVENT_WARNING, "New underlying '" & Data.UndSymbol & "' was added by trade feed. Please restart application."
'                    If Not g_PerformanceLog Is Nothing Then _
'                        g_PerformanceLog.LogMmInfo enLogWarning, "New underlying '" & Data.UndSymbol & "' was added by trade feed. Please restart application.", "TradeChannel"
'                End If
'
'            ElseIf g_CurTraderID = 0 Then
'                AddEvent EVENT_WARNING, "New trader was added by trade feed. Please restart application."
'                If Not g_PerformanceLog Is Nothing Then _
'                    g_PerformanceLog.LogMmInfo enLogWarning, "New trader was added by trade feed. Please restart application.", "TradeChannel"
'            End If
'        End If
'    Else
'        AddEvent EVENT_WARNING, "Invalid trade data was received."
'        If Not g_PerformanceLog Is Nothing Then _
'            g_PerformanceLog.LogMmInfo enLogWarning, "Invalid trade data was received." & Data.Description, "TradeChannel"
'
'    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "MmwMsgManager_OnTradeUpdate Exit: " & Data.SeqNum, "TradeChannel"
    
End Sub

Private Sub DoTradeUpdate(ByRef Data As MSGSTRUCTLib.ITradeUpdate)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "DoTradeUpdate Enter: " & Data.SeqNum, "TradeChannel"
    
    Dim nSeqNum&, aTrd As EtsGeneralLib.MmTradeInfoAtom, aAction As TRADE_ACTION
    Dim tmpTradedIV As Double
    
    aAction = Data.Action
    
    If Not g_PerformanceLog Is Nothing Then
        If aAction = enTeNew Then
            g_PerformanceLog.LogMmInfo enLogUserAction, "New trade Enter: " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
        Else
            If aAction = enTeDelete Then
                g_PerformanceLog.LogMmInfo enLogUserAction, "Delete Trade: " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
            Else
                g_PerformanceLog.LogMmInfo enLogUserAction, "Unknown operation" & CStr(aAction) & " : " & Data.Symbol & " " & CStr(Data.TradeID), "TradeChannel"
            End If
        End If
    End If

    
    If aAction = enTeNew Or aAction = enTeDelete Then
        
        nSeqNum = Data.SeqNum
        
        If (m_bManualTradeAction And m_nLastSeqNum > nSeqNum) Then
            'Sometimes when we have new trade created manualy
            'LastSeqNum stay more greater then getting from trade channel
            'and we lose trade from interval
            '[lastSeqNum(befor some manual actions), lastSeqNum(after some manual actions)]
            m_nLastSeqNum = nSeqNum - 2
            m_bManualTradeAction = False
        End If
        'If nSeqNum > m_nLastSeqNum Then
            
            If nSeqNum > m_nLastSeqNum + 1 Then
                ' reload missed trades
                LoadMissedTrades m_nLastSeqNum + 1, nSeqNum - 1
            End If
            
            If nSeqNum > m_nLastSeqNum Then
                m_nLastSeqNum = nSeqNum
            End If
            
            If g_CurTraderID = 0 And Not g_Trader(Data.TraderID) Is Nothing Or g_CurTraderID = Data.TraderID Then
                
                If Not g_UnderlyingAll(Data.UndSymbolID) Is Nothing Then
                    
                    Select Case aAction
                        Case enTeNew
                            If m_Trd(Data.TradeID) Is Nothing Then
                                If IsValidSymbolType(Data.SymbolType) Then
                                    AdjustTraderContractAndStrategy Data.TraderID, Data.UndSymbolID, Data.strategyID
                                    
                                    Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
                                    If CopyTradeFromSub(Data, aTrd) Then
                                        If Data.MsState <> enTmssNoChanges Then
                                            AddEvent EVENT_WARNING, "Underlying '" & aTrd.Und.Symbol & _
                                                                    "' has market structure changes." & _
                                                                    " Please update market structure."
                                        If Not g_PerformanceLog Is Nothing Then _
                                            g_PerformanceLog.LogMmInfo enLogWarning, "Underlying '" & aTrd.Und.Symbol & _
                                                                    "' has market structure changes." & _
                                                                    " Please update market structure.", "TradeChannel"
                                                                    
                                        End If
                                        
                                        If aTrd.ContractType = enCtOption And aTrd.TradedIV <= 0# Then
                                            tmpTradedIV = aTrd.TradedIV
                                            CalcTradedIV aTrd
                                            If aTrd.TradedIV <> tmpTradedIV And aTrd.TradedIV > 0# Then _
                                                UpdateTradeIV aTrd
                                        End If
                                        m_Trd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
                                       
                                    
                                        AddTradeToTradesByUnd aTrd
                                        AddTradeToTradesByTraderAndContract aTrd
                                        AddTradeToTradesByFut aTrd
                                    
                                        If aTrd.Status = enTsManual Then
                                            AddManualTradeToTradesByUndMan aTrd
                                            
                                        ElseIf aTrd.Status = enTsRealtime Then
                                            FindActualTradeMatch aTrd
                                        End If
                                        
                                        RaiseTradeAction aTrd, Nothing, enTaTradeNew
                                    End If
                                Else
                                    Debug.Assert False
                                End If
                            'Else
                            '    Debug.Assert False
                            End If
                        
                        Case enTeDelete
                            Set aTrd = m_Trd(Data.TradeID)
                            
                            If Not aTrd Is Nothing Then
                                m_Trd.Remove aTrd.TradeID
                            
                                RemoveTradeFromTradesByUnd aTrd
                                RemoveTradeFromTradesByTraderAndContract aTrd
                                RemoveTradeFromTradesByFut aTrd
                                
                                If aTrd.Status = enTsManual Then
                                    RemoveManualTradeFromTradesByUndMan aTrd
                                    RemoveManualTradeFromMatch aTrd
                                    
                                ElseIf aTrd.Status = enTsRealtime Then
                                    m_TrdForMatch.Remove aTrd.TradeID
                                End If
                            
                                RaiseTradeAction Nothing, aTrd, enTaTradeDelete
                            Else
                                Debug.Assert False
                            End If
                    End Select
                    
                    Set aTrd = Nothing
                    
                ElseIf aAction = enTeNew Then
                    AddEvent EVENT_WARNING, "New underlying '" & Data.undSymbol & "' was added by trade feed. Please restart application."
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogWarning, "New underlying '" & Data.undSymbol & "' was added by trade feed. Please restart application.", "TradeChannel"
                End If
                
            ElseIf g_CurTraderID = 0 Then
                AddEvent EVENT_WARNING, "New trader was added by trade feed. Please restart application."
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogWarning, "New trader was added by trade feed. Please restart application.", "TradeChannel"
            End If
        'End If
    Else
        AddEvent EVENT_WARNING, "Invalid trade data was received."
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogWarning, "Invalid trade data was received." & Data.Description, "TradeChannel"
        
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "DoTradeUpdate Exit: " & Data.SeqNum, "TradeChannel"
End Sub

Private Sub LoadMissedTrades(ByVal nMinSeqNum As Long, ByVal nMaxSeqNum As Long)
    On Error GoTo EH
    
    Dim Position  As Long
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogWarning, "Loading missed trades", "TradeChannel"
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Load missed trades Enter", "TradeChannel"
    
    Position = 1
    Dim rs As ADODB.Recordset
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nSeqNum&, enAction As TradeActionEnum, nTraderID&, aTrader As EtsGeneralLib.EtsTraderAtom
    Dim aUnd As EtsGeneralLib.UndAtom, enContractType As EtsGeneralLib.EtsContractTypeEnum
    Dim sKey$, aSynthRootComp As EtsGeneralLib.SynthRootCompAtom, nStrategyID&
    Dim sTradeInfo As String

    If nMinSeqNum > nMaxSeqNum Then Exit Sub
    Position = 2
    
    Set rs = gDBW.usp_MmTradeSeqByTrader_Get(g_CurTraderID, nMinSeqNum, nMaxSeqNum)
    Position = 3
    
    While Not rs.EOF
        Position = 4
        nSeqNum = ReadLng(rs!iSeqNum)
        Position = 5
        nTraderID = ReadLng(rs!iTraderID)
        Position = 6
        
        'If nSeqNum > m_nLastSeqNum Then m_nLastSeqNum = nSeqNum
        
        If (g_CurTraderID = 0 Or g_CurTraderID = nTraderID) _
            And nSeqNum >= nMinSeqNum And nSeqNum <= nMaxSeqNum Then
             
            Position = 7
    
            enAction = IIf(ReadByte(rs!tiActionID) <> 0, enTaTradeNew, enTaTradeDelete)
            Position = 8

            If enAction = enTaTradeNew Then
                nStrategyID = ReadLng(rs!iStrategyID)
            
                Position = 9
                
                enContractType = ReadLng(rs!iContractTypeID)
                Position = 10
                
                If IsValidContractType(enContractType) Then
                    Position = 11
                    Set aUnd = g_UnderlyingAll(ReadLng(rs!iUnderlyingContractID))
                    Position = 12
                    Set aTrader = g_Trader(nTraderID)
                    Position = 13
                    If Not aTrader Is Nothing Then
                        Position = 14
                        If Not aUnd Is Nothing Then
                            Position = 15
                            AdjustTraderContractAndStrategy aTrader.ID, aUnd.ID, nStrategyID
                            Position = 16
                            
                            Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
                            Position = 17
                            Set aTrd.Und = aUnd
                            Position = 18
                            
                            aTrd.TradeID = ReadLng(rs!iTradeID)
                            Position = 19
                            aTrd.price = ReadDbl(rs!fPrice)
                            Position = 20
                            aTrd.Quantity = ReadLng(rs!iQuantity)
                            Position = 21
                            aTrd.IsBuy = (ReadByte(rs!tiIsBuy) <> 0)
                            Position = 22
                            aTrd.TradeDate = GmtToLocal(ReadDate(rs!dtTradeDate))
                            Position = 23
                            aTrd.IsPosition = ReadByte(rs!tiIsPosition)
                            Position = 24
                            aTrd.Status = ReadByte(rs!tiStatus)
                            Position = 25
                            aTrd.BrokerCommission = ReadDbl(rs!fBrokerCommission)
                            Position = 26
                            aTrd.ClearingBrokerCommission = ReadDbl(rs!fClearingBrokerCommission)
                            Position = 27
                            aTrd.Mark = ReadLng(rs!iMark)
                            Position = 28
                            
                            Set aTrd.Trader = aTrader
                            Position = 29
                            Set aTrd.Broker = g_Broker(ReadLng(rs!iBrokerID))
                            Position = 30
                            Set aTrd.ClearingBroker = g_ClearingBroker(ReadLng(rs!iClearingBrokerID))
                            Position = 31
                            Set aTrd.Strategy = g_Strategy(ReadLng(rs!iStrategyID))
                            Position = 32
                            
                            aTrd.OrderID = ReadLng(rs!iOrderID)
                            aTrd.ExecStatus = ReadLng(rs!iExecStatus)
                            
                            Select Case enContractType
                                Case enCtOption
                                    Position = 33
                                    Set aTrd.OptRoot = aTrd.Und.Roots(ReadLng(rs!iOptionRootID))
                                    Position = 34
                                    If Not aTrd.OptRoot Is Nothing Then
                                        Position = 35
                                        Set aTrd.Opt = New EtsGeneralLib.EtsOptAtom
                                        Position = 36
                                    
                                        aTrd.Opt.ID = ReadLng(rs!iContractID)
                                        Position = 37
                                        aTrd.Opt.Symbol = ReadStr(rs!vcSymbol)
                                        Position = 38
                                        aTrd.Opt.RootID = aTrd.OptRoot.ID
                                        Position = 39
                                        aTrd.Opt.OptType = ReadByte(rs!tiIsCall)
                                        Position = 40
                                        aTrd.Opt.Expiry = ReadDate(rs!dtExpiry)
                                        aTrd.Opt.ExpiryOV = ReadDate(rs!dtExpiryOV)
                                        aTrd.Opt.TradingClose = ClipDays(ReadDate(rs!dtTradingClose))
                                        
                                        Position = 41
                                        aTrd.Opt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
                                        Position = 42
                                        aTrd.Opt.PriceClose = ReadDbl(rs!fPriceClose)
                                        Position = 43
                                        aTrd.Opt.PriceTheoclose = ReadDbl(rs!fPriceTheoClose)
                                        Position = 44
                                        aTrd.Opt.Series = ReadStr(rs!vcContractName)
                                        Position = 45
                                    
                                        aTrd.SpotReference = ReadDbl(rs!fSpotReference)
                                        Position = 46
                                        aTrd.TradedIV = ReadDbl(rs!fTradedIV)
                                        Position = 47
                                        aTrd.Opt.ImportID = ReadStr(rs!vcImportID)
                                        Position = 48
                                    Else
                                        Position = 49
                                        AddEvent EVENT_WARNING, "New option root for '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Position = 50
                                        Err.Raise vbObjectError + 100, "Trade Channel", "New option root for '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Position = 51
                                    End If
                                Case enCtFuture
                                        Position = 52
                                    Set aTrd.FutRoot = aTrd.Und.FutRoots(ReadLng(rs!iFutureRootID))
                                        Position = 53
                                    If aTrd.FutRoot Is Nothing Then
                                        Position = 54
                                        AddEvent EVENT_WARNING, "New future root for '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Position = 55
                                        Err.Raise vbObjectError + 100, "Trade Channel", "New future root for '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Position = 56
                                    End If
                                    
                                    Position = 57
                                    Set aTrd.Fut = aTrd.FutRoot.Futures(ReadLng(rs!iFutureID))
                                    Position = 58
                                    If aTrd.Fut Is Nothing Then
                                        Position = 59
                                        AddEvent EVENT_WARNING, "New future '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Err.Raise vbObjectError + 100, "Trade Channel", "New future '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                                        Position = 60
                                    End If
                                    
                                Case enCtFutOption
                                    Position = 61
                                    Set aTrd.FutRoot = aTrd.Und.FutRoots(ReadLng(rs!iFutureRootID))
                                    Position = 61
                                    Set aTrd.Fut = aTrd.FutRoot.Futures(ReadLng(rs!iFutureID))
                                    Position = 63
                                    Set aTrd.FutOpt = New EtsGeneralLib.EtsFutOptAtom
                                    Position = 64
                                    
                                    aTrd.FutOpt.ID = ReadLng(rs!iContractID)
                                    Position = 65
                                    aTrd.FutOpt.Symbol = ReadStr(rs!vcSymbol)
                                    Position = 66
                                    aTrd.FutOpt.FutID = aTrd.Fut.ID
                                    Position = 67
                                    aTrd.FutOpt.OptType = ReadByte(rs!tiIsCall)
                                    Position = 68
                                    aTrd.FutOpt.Expiry = ReadDate(rs!dtExpiry)
                                    aTrd.FutOpt.ExpiryOV = ReadDate(rs!dtExpiryOV)
                                    aTrd.FutOpt.TradingClose = ClipDays(ReadDate(rs!dtTradingClose))
                                    Position = 69
                                    aTrd.FutOpt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
                                    Position = 70
                                    aTrd.FutOpt.PriceClose = ReadDbl(rs!fPriceClose)
                                    Position = 71
                                    aTrd.FutOpt.PriceTheoclose = ReadDbl(rs!fPriceTheoClose)
                                    Position = 72
                                    aTrd.FutOpt.Series = ReadStr(rs!vcContractName)
                                    Position = 73
                                    
                                    aTrd.SpotReference = ReadDbl(rs!fSpotReference)
                                    Position = 74
                                    aTrd.TradedIV = ReadDbl(rs!fTradedIV)
                                    Position = 75
                                    aTrd.FutOpt.ImportID = ReadStr(rs!vcImportID)
                                    
                            End Select
                            Position = 76

                            If Not m_Trd(aTrd.TradeID) Is Nothing Then
                                Position = 7601
                                m_Trd.Remove aTrd.TradeID
                            End If
                            Position = 7602
                            
                            m_Trd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
                            Position = 77
                            
                            Set aTrd.Contract = g_Main.GetContract(aTrd.ContractID)
                            
                            AddTradeToTradesByUnd aTrd
                            Position = 78
                            AddTradeToTradesByTraderAndContract aTrd
                            Position = 79
                            AddTradeToTradesByFut aTrd
                            Position = 80
                        
                            If aTrd.Status = enTsManual Then
                                Position = 81
                                AddManualTradeToTradesByUndMan aTrd
                                Position = 82
                                
                            ElseIf aTrd.Status = enTsRealtime Then
                                Position = 83
                                FindActualTradeMatch aTrd
                                Position = 84
                            End If
                            
                            Position = 85
                            RaiseTradeAction aTrd, Nothing, enTaTradeNew
                            Position = 86
                            
                            If ReadLng(rs!iStructureMissed) <> 0 Then
                                AddEvent EVENT_WARNING, "Underlying '" & aTrd.Und.Symbol & _
                                                        "' has market structure changes." & _
                                                        " Please update market structure."
                            End If
                            Position = 87

                            Set aUnd = Nothing
                        Else
                            Position = 88
                            AddEvent EVENT_WARNING, "New underlying '" & ReadStr(rs!vcUnderlyingSymbol) & "' was added by trade feed. Please restart application."
                        End If
                        
                        Position = 89
                        Set aTrader = Nothing
                
                    ElseIf g_CurTraderID = 0 Then
                        Position = 90
                        AddEvent EVENT_WARNING, "New trader was added by trade feed. Please restart application."
                    End If
                Else
                    Debug.Assert False
                End If
            Else
                Position = 91
                Set aTrd = m_Trd(rs!iTradeID)
                Position = 92
                If Not aTrd Is Nothing Then
                    Position = 93
                    m_Trd.Remove aTrd.TradeID
                    Position = 94
                
                    RemoveTradeFromTradesByUnd aTrd
                    Position = 95
                    RemoveTradeFromTradesByTraderAndContract aTrd
                    Position = 96
                    RemoveTradeFromTradesByFut aTrd
                    Position = 97
                    
                    If aTrd.Status = enTsManual Then
                        Position = 98
                        RemoveManualTradeFromTradesByUndMan aTrd
                        Position = 99
                        RemoveManualTradeFromMatch aTrd
                        Position = 100
                        
                    ElseIf aTrd.Status = enTsRealtime Then
                        Position = 101
                        If Not m_TrdForMatch(aTrd.TradeID) Is Nothing Then
                            Position = 102
                            m_TrdForMatch.Remove aTrd.TradeID
                        End If
                        Position = 103
                    End If
                
                     Position = 104
                    RaiseTradeAction Nothing, aTrd, enTaTradeDelete
                Else
                    'Debug.Assert False
                End If
            End If
                
            
            If (Not aTrd Is Nothing) Then
                If (Not aTrd.Contract Is Nothing) Then
                    sTradeInfo = " TradeID: " & CStr(aTrd.TradeID) & " Symbol: " & aTrd.Contract.Symbol
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogWarning, "Loaded missed " & sTradeInfo, "TradeChannel"
                End If
            End If
                
            Set aTrd = Nothing
        End If
        Position = 104
        rs.MoveNext
    Wend
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Load missed trades Exit", "TradeChannel"

    Exit Sub
EH:

    AddEvent EVENT_WARNING, "Fail to load missed trades! " & Err.Description & " " & CStr(Position)
    Set rs = Nothing
    Set aTrd = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Load missed trades Exit with errors", "TradeChannel"

End Sub

Private Sub LoadLastSeqNum()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "LoadLastSeqNum Enter", "TradeChannel"
    
    m_nLastSeqNum = gDBW.usp_LastTradeSeqNum_Get
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "LoadLastSeqNum Exit", "TradeChannel"


    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to load last trade sequence number"
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "LoadLastSeqNum Exit with error", "TradeChannel"

End Sub

Private Sub FindAllActualTradesMatch()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "FindAllActualTradesMatch Enter", "TradeChannel"
    
    Dim aTrdActual As EtsGeneralLib.MmTradeInfoAtom
    
    For Each aTrdActual In m_Trd
        If aTrdActual.Status = enTsRealtime Then
            FindActualTradeMatch aTrdActual
        End If
    Next

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "FindAllActualTradesMatch Exit", "TradeChannel"

End Sub

Private Sub FindActualTradeMatch(ByRef aTrdActual As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "FindActualTradeMatch Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$, aTrdCheck As EtsGeneralLib.MmTradeInfoAtom
    Dim aTrdForMatch As EtsGeneralLib.MmTradeMatchAtom, aTrd As EtsGeneralLib.MmTradeInfoAtom
    
    If aTrdActual.Status = enTsRealtime Then
        sKey = CStr(aTrdActual.UndID) & "_" & CStr(aTrdActual.Trader.ID)
        Set aTrdByUnd = m_TrdByUndMan(sKey)
        If Not aTrdByUnd Is Nothing Then
        
            For Each aTrdCheck In aTrdByUnd
                If aTrdActual.ContractID = aTrdCheck.ContractID _
                    And aTrdActual.IsBuy = aTrdCheck.IsBuy _
                    And aTrdActual.TradeDate > aTrdCheck.TradeDate _
                    And CLng(aTrdActual.TradeDate) = CLng(aTrdCheck.TradeDate) Then
                
                    If aTrdForMatch Is Nothing Then
                        Set aTrdForMatch = m_TrdForMatch(aTrdActual.TradeID)
                        If aTrdForMatch Is Nothing Then
                            Set aTrdForMatch = New EtsGeneralLib.MmTradeMatchAtom
                            m_TrdForMatch.Add aTrdActual.TradeID, aTrdActual.TradeDate, aTrdForMatch
                            Set aTrdForMatch.TrdActual = aTrdActual
                            aTrdForMatch.Updated = True
                        End If
                    End If
                    
                    Set aTrd = aTrdForMatch.TrdManual(aTrdCheck.TradeID)
                    If aTrd Is Nothing Then
                        aTrdForMatch.TrdManual.Add aTrdCheck.TradeID, aTrdCheck.TradeDate, aTrdCheck
                        aTrdForMatch.Updated = True
                    End If
                End If
            Next
            
        End If
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "FindActualTradeMatch Exit", "TradeChannel"


End Sub

Private Sub RemoveManualTradeFromMatch(ByRef aTrdManual As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveManualTradeFromMatch Enter", "TradeChannel"
    
    Dim sKey$, aTrdForMatch As EtsGeneralLib.MmTradeMatchAtom
        
    If aTrdManual.Status = enTsManual Then
        For Each aTrdForMatch In m_TrdForMatch
            Debug.Assert Not aTrdForMatch.TrdActual Is Nothing
        
            If Not aTrdForMatch.TrdManual(aTrdManual.TradeID) Is Nothing Then
                aTrdForMatch.TrdManual.Remove aTrdManual.TradeID
                aTrdForMatch.Updated = True
                
                If Not aTrdForMatch.TrdMatch Is Nothing Then
                    If aTrdForMatch.TrdMatch.TradeID = aTrdManual.TradeID Then
                        Set aTrdForMatch.TrdMatch = Nothing
                    End If
                End If
            End If
            
            If aTrdForMatch.TrdManual.Count <= 0 And Not m_TrdForMatch(aTrdForMatch.TrdActual.TradeID) Is Nothing Then
                m_TrdForMatch.Remove aTrdForMatch.TrdActual.TradeID
            End If
        Next
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveManualTradeFromMatch Exit", "TradeChannel"

End Sub

Public Sub ProccessTradesMatch(ByRef aTrdForMatch As MmTradeMatchAtom)
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProccessTradesMatch Enter", "TradeChannel"
    
    Dim aPubManager As MMWLib.IPubManager
    Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
    Dim aTrdForMatchCheck As EtsGeneralLib.MmTradeMatchAtom
    Dim aOldManualTrdInfo As New EtsGeneralLib.MmTradeInfoAtom, aOldActualTrdInfo As New EtsGeneralLib.MmTradeInfoAtom
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoAtom, sKey$, aNewActualTrdInfo As New EtsGeneralLib.MmTradeInfoAtom
    Dim nOldManualSeqNum&, nOldManualTradeID&, nLastSeqNum&, nOldActualSeqNum&
    Dim nNewActualSeqNum&, nNewActualTradeID&, dtNewActualTradeDate As Date

    Screen.MousePointer = vbHourglass
    dtNewActualTradeDate = 0
    
    If Not aTrdForMatch.TrdMatch Is Nothing Then
        aTrdForMatch.TrdMatch.CopyTo aOldManualTrdInfo
        nOldManualTradeID = aOldManualTrdInfo.TradeID
    Else
        dtNewActualTradeDate = LocalToGmt(aTrdForMatch.TrdActual.TradeDate)
    End If
    
    aTrdForMatch.TrdActual.CopyTo aOldActualTrdInfo
    aOldActualTrdInfo.CopyTo aNewActualTrdInfo
    aNewActualTrdInfo.Status = enTsMatchedRealtime
    
    nLastSeqNum = 0
    nOldActualSeqNum = 0
    nNewActualSeqNum = 0
    nNewActualTradeID = 0
    
    nLastSeqNum = gDBW.usp_Trade_Match(nOldManualTradeID, aOldActualTrdInfo.TradeID, _
                        nOldManualSeqNum, nOldActualSeqNum, _
                        nNewActualSeqNum, nNewActualTradeID, _
                        dtNewActualTradeDate)
    
    If nLastSeqNum <= 0 Then
        Err.Raise vbObjectError + 1025, "Trade Channel", "Fail to store trade matching status in database."
    End If
    
    aOldActualTrdInfo.SeqNum = nOldActualSeqNum
    aNewActualTrdInfo.SeqNum = nNewActualSeqNum
    aNewActualTrdInfo.TradeID = nNewActualTradeID
    aNewActualTrdInfo.TradeDate = GmtToLocal(dtNewActualTradeDate)
    
    Set aPubManager = MmwMsgManager
    
    If Not aTrdForMatch.TrdMatch Is Nothing Then ' delete manual trade
        aOldManualTrdInfo.TradeID = nOldManualTradeID
        aOldManualTrdInfo.SeqNum = nOldManualSeqNum
        
        aTrdUpdate.Action = enTeDelete
        CopyTradeForPub aOldManualTrdInfo, aTrdUpdate
        
        aPubManager.PubTradeUpdate aTrdUpdate
        
        m_Trd.Remove aOldManualTrdInfo.TradeID
        
        RemoveTradeFromTradesByUnd aOldManualTrdInfo
        RemoveTradeFromTradesByTraderAndContract aOldManualTrdInfo
        RemoveManualTradeFromTradesByUndMan aOldManualTrdInfo
        RemoveTradeFromTradesByFut aOldManualTrdInfo
    
        For Each aTrdForMatchCheck In m_TrdForMatch
            Debug.Assert Not aTrdForMatchCheck.TrdActual Is Nothing
        
            If Not aTrdForMatchCheck.TrdManual(aOldManualTrdInfo.TradeID) Is Nothing Then
                aTrdForMatchCheck.TrdManual.Remove aOldManualTrdInfo.TradeID
                aTrdForMatchCheck.Updated = True
                
                If Not aTrdForMatchCheck.TrdMatch Is Nothing Then
                    If aTrdForMatchCheck.TrdMatch.TradeID = aOldManualTrdInfo.TradeID Then
                        Set aTrdForMatchCheck.TrdMatch = Nothing
                    End If
                End If
            End If
            
            ' prevent current matching item from deletion
            If aTrdForMatchCheck.TrdActual.TradeID <> aOldManualTrdInfo.TradeID _
                And aTrdForMatchCheck.TrdManual.Count <= 0 Then
                m_TrdForMatch.Remove aTrdForMatchCheck.TrdActual.TradeID
            End If
        Next
        
        On Error Resume Next
        m_nLastSeqNum = aOldManualTrdInfo.SeqNum
        RaiseTradeAction Nothing, aOldManualTrdInfo, enTaTradeDelete
        On Error GoTo EH
    End If

    aTrdUpdate.Action = enTeDelete
    CopyTradeForPub aOldActualTrdInfo, aTrdUpdate
    aPubManager.PubTradeUpdate aTrdUpdate
    
    aTrdUpdate.Action = enTeNew
    CopyTradeForPub aNewActualTrdInfo, aTrdUpdate
    aPubManager.PubTradeUpdate aTrdUpdate
    
    ' remove old actual trade
    If Not m_Trd(aOldActualTrdInfo.TradeID) Is Nothing Then _
        m_Trd.Remove aOldActualTrdInfo.TradeID
    
    If Not m_TrdForMatch(aOldActualTrdInfo.TradeID) Is Nothing Then _
        m_TrdForMatch.Remove aOldActualTrdInfo.TradeID
    
    RemoveTradeFromTradesByUnd aOldActualTrdInfo
    RemoveTradeFromTradesByTraderAndContract aOldActualTrdInfo
    RemoveTradeFromTradesByFut aOldActualTrdInfo
    
    ' add new actual trade
    m_Trd.Add aNewActualTrdInfo.TradeID, aNewActualTrdInfo.TradeDate, aNewActualTrdInfo
    
    AddTradeToTradesByUnd aNewActualTrdInfo
    AddTradeToTradesByTraderAndContract aNewActualTrdInfo
    AddTradeToTradesByFut aNewActualTrdInfo
    
    ' raise updates notification
    On Error Resume Next
    m_nLastSeqNum = nLastSeqNum
    RaiseTradeAction Nothing, aOldActualTrdInfo, enTaTradeDelete
    RaiseTradeAction aNewActualTrdInfo, Nothing, enTaTradeNew

Ex:
    Set aPubManager = Nothing
    Screen.MousePointer = vbDefault
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProccessTradesMatch Exit", "TradeChannel"
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    Set aPubManager = Nothing
    gCmn.ErrorHandler "Fail to match trade"
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProccessTradesMatch Exit with errors", "TradeChannel"

End Sub

Public Sub MakeTradesListForExec(ByRef collTrades As clsTradeExecColl, ByVal nUnderlyingID As Long, _
                                ByVal nFuturesID As Long, ByVal nOptionID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "MakeTradesListForExec Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aTrdExec As clsTradeExecAtom
    
    If nUnderlyingID <> 0 Then
        Set aTrdByUnd = m_TrdByUnd(CStr(nUnderlyingID))
    Else
        Set aTrdByUnd = m_TrdByFut(CStr(nFuturesID))
    End If
    
    If Not aTrdByUnd Is Nothing Then
        For Each aTrd In aTrdByUnd
            If aTrd.ContractID = nOptionID And aTrd.Status <> enTsSimulated _
                And (g_CurTraderID = 0 Or g_CurTraderID = aTrd.Trader.ID) Then
                
                Set aTrdExec = collTrades.Add(CStr(aTrd.TradeID))
                aTrdExec.IsExec = (aTrd.price >= 0)
                aTrdExec.QtyToExec = IIf(aTrdExec.IsExec, aTrd.Quantity, 0)
                
                If aTrd.ContractType = enCtOption Then
                    aTrdExec.UndTradePrice = aTrd.Opt.Strike
                Else
                    aTrdExec.UndTradePrice = aTrd.FutOpt.Strike
                End If
                
                aTrdExec.UndPrice = 0#
                Set aTrdExec.Trd = aTrd
                Set aTrdExec = Nothing
            End If
        Next
        
        Set aTrdByUnd = Nothing
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "MakeTradesListForExec Exit", "TradeChannel"

End Sub
Public Sub ProcessCommonTradeExec(ByRef aTrdExec As clsTradeExecAtom, ByVal aPriceOfIndex As Double, aPriceOfTrade As Double, ByVal bExercise As Boolean)
    On Error GoTo EH

        Dim aPubManager As MMWLib.IPubManager
        Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
        Dim aNewOptTrdInfo As New EtsGeneralLib.MmTradeInfoAtom
        Dim aNewUndTrdInfo As New EtsGeneralLib.MmTradeInfoAtom, nLastSeqNum&
        Dim nNewOptTradeSeqNum&, nNewOptTradeID&, dtNewOptTradeDate As Date
        Dim nNewUndTradeSeqNum&, nNewUndTradeID&, dtNewUndTradeDate As Date
    
        ' fill opposite option trade
        aTrdExec.Trd.CopyTo aNewOptTrdInfo
        aNewOptTrdInfo.Quantity = aTrdExec.QtyToExec
        aNewOptTrdInfo.price = aPriceOfTrade
        aNewOptTrdInfo.IsBuy = Not aTrdExec.Trd.IsBuy
        aNewOptTrdInfo.TradeDate = Now
        aNewOptTrdInfo.IsPosition = 0
        aNewOptTrdInfo.Status = enTsRealtime 'enTsManual
        aNewOptTrdInfo.BrokerCommission = 0#
        aNewOptTrdInfo.ClearingBrokerCommission = 0#
        
        If bExercise Then ' fill new underlying trade

            Set aNewUndTrdInfo.Fut = aNewOptTrdInfo.Fut
                        
            If aNewOptTrdInfo.ContractType <> enCtFutOption Then
                Set aNewUndTrdInfo.Und = aNewOptTrdInfo.Und
            ElseIf aNewUndTrdInfo.Fut Is Nothing Then
                Set aNewUndTrdInfo.Und = aNewOptTrdInfo.Und
                Dim lFutId As Long
                lFutId = aNewOptTrdInfo.FutOpt.FutID
                'aNewOptTrdInfo.FutRoot.Futures(lFutId)
                Set aNewUndTrdInfo.Fut = aNewOptTrdInfo.FutRoot.Futures(lFutId)
            End If
            
            Set aNewUndTrdInfo.FutRoot = aNewOptTrdInfo.FutRoot
        
            aNewUndTrdInfo.price = aTrdExec.UndTradePrice
            
            If aNewOptTrdInfo.ContractType = enCtOption Then
                aNewUndTrdInfo.Quantity = aNewOptTrdInfo.Quantity * aNewOptTrdInfo.OptRoot.LotSize
                aNewUndTrdInfo.IsBuy = IIf(aNewOptTrdInfo.Opt.OptType = enOtPut, Not aTrdExec.Trd.IsBuy, aTrdExec.Trd.IsBuy)
            Else
                aNewUndTrdInfo.Quantity = aNewOptTrdInfo.Quantity * aNewOptTrdInfo.FutRoot.OptLotSize
                aNewUndTrdInfo.IsBuy = IIf(aNewOptTrdInfo.FutOpt.OptType = enOtPut, Not aTrdExec.Trd.IsBuy, aTrdExec.Trd.IsBuy)
            End If
        
            aNewUndTrdInfo.TradeDate = Now
            aNewUndTrdInfo.Status = enTsRealtime 'enTsManual
            aNewUndTrdInfo.BrokerCommission = 0#
            aNewUndTrdInfo.ClearingBrokerCommission = 0#
            aNewUndTrdInfo.Mark = aNewOptTrdInfo.Mark
        
            Set aNewUndTrdInfo.Trader = aNewOptTrdInfo.Trader
            Set aNewUndTrdInfo.Broker = aNewOptTrdInfo.Broker
            Set aNewUndTrdInfo.ClearingBroker = aNewOptTrdInfo.ClearingBroker
            Set aNewUndTrdInfo.Strategy = aNewOptTrdInfo.Strategy
        End If
    
        ' store trades to db
        
        Dim bisIndex As Integer
        bisIndex = IIf(aNewOptTrdInfo.Und.UndType = enCtIndex, 1, 0)
        If aNewOptTrdInfo.ContractType = enCtFutOption Then
            bisIndex = 0
        End If
        
        
        nLastSeqNum = gDBW.usp_Trade_Exec(IIf(bExercise, 1, 0), aNewOptTrdInfo.TradeID, aNewOptTrdInfo.Quantity, _
                                        IIf(aNewOptTrdInfo.IsBuy, 1, 0), aNewOptTrdInfo.Status, _
                                        aNewOptTrdInfo.BrokerCommission, aNewOptTrdInfo.ClearingBrokerCommission, _
                                        aNewUndTrdInfo.Quantity, aNewUndTrdInfo.price, _
                                        IIf(aNewUndTrdInfo.IsBuy, 1, 0), aNewUndTrdInfo.Status, _
                                        aNewUndTrdInfo.BrokerCommission, aNewUndTrdInfo.ClearingBrokerCommission, _
                                        IIf(aNewOptTrdInfo.ContractType = enCtOption, 0, 1), _
                                        IIf(bisIndex = 1, aPriceOfIndex, aNewOptTrdInfo.price), _
                                        bisIndex, _
                                        nNewOptTradeSeqNum, nNewOptTradeID, dtNewOptTradeDate, _
                                        nNewUndTradeSeqNum, nNewUndTradeID, dtNewUndTradeDate)
    
        If nLastSeqNum <= 0 Then GoTo Ex
        
        If bisIndex = 1 Then
            aNewOptTrdInfo.price = aPriceOfIndex
        End If
        Set aNewOptTrdInfo.Strategy = aTrdExec.Trd.Strategy
        Set aNewOptTrdInfo.Trader = aTrdExec.Trd.Trader
        
        aNewOptTrdInfo.SeqNum = nNewOptTradeSeqNum
        aNewOptTrdInfo.TradeID = nNewOptTradeID
        aNewOptTrdInfo.TradeDate = GmtToLocal(dtNewOptTradeDate)

        If bisIndex <> 1 Then
            If bExercise Then
                aNewUndTrdInfo.SeqNum = nNewUndTradeSeqNum
                aNewUndTrdInfo.TradeID = nNewUndTradeID
                aNewUndTrdInfo.TradeDate = GmtToLocal(dtNewUndTradeDate)
                Set aNewUndTrdInfo.Und = aNewOptTrdInfo.Und
            End If
        End If
    
        Set aPubManager = MmwMsgManager
    
        ' publish new trades
        aTrdUpdate.Action = enTeNew
        CopyTradeForPub aNewOptTrdInfo, aTrdUpdate
        aPubManager.PubTradeUpdate aTrdUpdate
    
        If bisIndex <> 1 Then
            If bExercise Then
                aTrdUpdate.Action = enTeNew
                CopyTradeForPub aNewUndTrdInfo, aTrdUpdate
                aPubManager.PubTradeUpdate aTrdUpdate
            End If
        End If
        
        ' add new trades to internal storage
        m_Trd.Add aNewOptTrdInfo.TradeID, aNewOptTrdInfo.TradeDate, aNewOptTrdInfo
    
        AddTradeToTradesByUnd aNewOptTrdInfo
        AddTradeToTradesByTraderAndContract aNewOptTrdInfo
        AddManualTradeToTradesByUndMan aNewOptTrdInfo
        AddTradeToTradesByFut aNewOptTrdInfo
    
        If bisIndex <> 1 Then
            If bExercise Then
                m_Trd.Add aNewUndTrdInfo.TradeID, aNewUndTrdInfo.TradeDate, aNewUndTrdInfo
            
                AddTradeToTradesByUnd aNewUndTrdInfo
                AddTradeToTradesByTraderAndContract aNewUndTrdInfo
                AddManualTradeToTradesByUndMan aNewUndTrdInfo
                AddTradeToTradesByFut aNewUndTrdInfo
            End If
        End If
        ' notify views
        
        
        On Error Resume Next
        m_nLastSeqNum = nLastSeqNum
        RaiseTradeAction aNewOptTrdInfo, Nothing, enTaTradeNew

        If bisIndex <> 1 Then
            If bExercise Then RaiseTradeAction aNewUndTrdInfo, Nothing, enTaTradeNew
        End If

Ex:
    Set aPubManager = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec Exit", "TradeChannel"
    
    Exit Sub
EH:
    AddEvent EVENT_ERROR, "Fail to exercise/expiry trade: " & Err.Description
    Set aPubManager = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec Exit with errors", "TradeChannel"


End Sub

Public Sub ProcessSynteticTradeExec(ByRef aTrdExec As clsTradeExecAtom, aPriceOfIndex As Double, aPriceOfTrade As Double)
    On Error GoTo EH
       
        Dim aPubManager As MMWLib.IPubManager
        Dim aTrdUpdate As New MSGSTRUCTLib.TradeUpdate
        Dim aNewOptTrdInfo As New EtsGeneralLib.MmTradeInfoAtom
        Dim aNewUndTrdInfo As New EtsGeneralLib.MmTradeInfoAtom, nLastSeqNum&
        Dim nNewOptTradeSeqNum&, nNewOptTradeID&, dtNewOptTradeDate As Date
        Dim nNewUndTradeSeqNum&, nNewUndTradeID&, dtNewUndTradeDate As Date
        Dim aSyntRoot As SynthRootAtom
        Dim aSyntRootCompAtom As SynthRootCompAtom
        Dim dblWeight As Double
        Set aPubManager = MmwMsgManager
        ' check posibility of exersize
              Set aSyntRoot = aTrdExec.Trd.Und.SyntheticRoots(aTrdExec.Trd.OptRootID)
        If Not aSyntRoot Is Nothing Then
           For Each aSyntRootCompAtom In aSyntRoot.SynthRootComponents
             If aSyntRootCompAtom.Weight > 0# And aSyntRootCompAtom.Settlement = BAD_DOUBLE_VALUE Then
                     AddEvent EVENT_ERROR, "Unable to exersize. Strike settlment allocation is not specified"
                     MsgBox "Please specify strike settlment allocation in the ETS Bakoffice", vbOKOnly, "Exersize"
                     Set aPubManager = Nothing
                     GoTo Ex
             End If
           Next
        End If
        ' fill opposite option trade
        aTrdExec.Trd.CopyTo aNewOptTrdInfo
        aNewOptTrdInfo.Quantity = aTrdExec.QtyToExec
        aNewOptTrdInfo.price = aPriceOfTrade
        aNewOptTrdInfo.IsBuy = Not aTrdExec.Trd.IsBuy
        aNewOptTrdInfo.TradeDate = Now
        aNewOptTrdInfo.IsPosition = 0
        aNewOptTrdInfo.Status = enTsRealtime 'enTsManual
        aNewOptTrdInfo.BrokerCommission = 0#
        aNewOptTrdInfo.ClearingBrokerCommission = 0#
        
        Dim bisIndex As Integer
        bisIndex = IIf(aNewOptTrdInfo.Und.UndType = enCtIndex, 1, 0)
        If aNewOptTrdInfo.ContractType = enCtFutOption Then
            bisIndex = 0
        End If
        
        ' store trades to db
        nLastSeqNum = gDBW.usp_Trade_Exec(1, aNewOptTrdInfo.TradeID, aNewOptTrdInfo.Quantity, _
                                        IIf(aNewOptTrdInfo.IsBuy, 1, 0), aNewOptTrdInfo.Status, _
                                        aNewOptTrdInfo.BrokerCommission, aNewOptTrdInfo.ClearingBrokerCommission, _
                                        aNewUndTrdInfo.Quantity, aNewUndTrdInfo.price, _
                                        IIf(aNewUndTrdInfo.IsBuy, 1, 0), aNewUndTrdInfo.Status, _
                                        aNewUndTrdInfo.BrokerCommission, aNewUndTrdInfo.ClearingBrokerCommission, _
                                        IIf(aNewOptTrdInfo.ContractType = enCtOption, 0, 1), _
                                        IIf(bisIndex = 1, aPriceOfIndex, aNewOptTrdInfo.price), _
                                        bisIndex, _
                                        nNewOptTradeSeqNum, nNewOptTradeID, dtNewOptTradeDate, _
                                        nNewUndTradeSeqNum, nNewUndTradeID, dtNewUndTradeDate)

       ' aTrdExec.IsExec = True

        If nLastSeqNum <= 0 Then GoTo Ex
        
        If aNewOptTrdInfo.Und.UndType = enCtIndex Then
            aNewOptTrdInfo.price = aPriceOfIndex
        End If
        Set aNewOptTrdInfo.Strategy = aTrdExec.Trd.Strategy
        Set aNewOptTrdInfo.Trader = aTrdExec.Trd.Trader
    
        aNewOptTrdInfo.SeqNum = nNewOptTradeSeqNum
        aNewOptTrdInfo.TradeID = nNewOptTradeID
        aNewOptTrdInfo.TradeDate = GmtToLocal(dtNewOptTradeDate)
    
        If nLastSeqNum <= 0 Then GoTo Ex
        ' add new trades to internal storage
        m_Trd.Add aNewOptTrdInfo.TradeID, aNewOptTrdInfo.TradeDate, aNewOptTrdInfo
    
        AddTradeToTradesByUnd aNewOptTrdInfo
        AddTradeToTradesByTraderAndContract aNewOptTrdInfo
        AddManualTradeToTradesByUndMan aNewOptTrdInfo
        AddTradeToTradesByFut aNewOptTrdInfo
        
        ' notify views
        On Error Resume Next
        m_nLastSeqNum = nLastSeqNum
        RaiseTradeAction aNewOptTrdInfo, Nothing, enTaTradeNew
        
        
        Set aSyntRoot = aTrdExec.Trd.Und.SyntheticRoots(aTrdExec.Trd.OptRootID)
        If Not aSyntRoot Is Nothing Then
           For Each aSyntRootCompAtom In aSyntRoot.SynthRootComponents
            If aSyntRootCompAtom.Weight > 0# Then
                Dim aUndTrdInfo As New EtsGeneralLib.MmTradeInfoAtom
                Dim nUndTradeSeqNum&, nUndTradeID&, dtUndTradeDate As Date
                
                Set aUndTrdInfo.Und = g_UnderlyingAll(aSyntRootCompAtom.UndID)
                'Set aUndTrdInfo.Fut = aNewOptTrdInfo.Fut
                'Set aUndTrdInfo.FutRoot = aNewOptTrdInfo.FutRoot
                If aSyntRootCompAtom.Weight > 0# Then
                    dblWeight = aSyntRootCompAtom.Weight
                Else
                    dblWeight = 1#
                End If
                
                aUndTrdInfo.price = aTrdExec.Trd.Opt.Strike * aSyntRootCompAtom.Settlement / dblWeight
        
                aUndTrdInfo.Quantity = aNewOptTrdInfo.Quantity * aNewOptTrdInfo.OptRoot.LotSize * aSyntRootCompAtom.Weight
                aUndTrdInfo.IsBuy = IIf(aNewOptTrdInfo.Opt.OptType = enOtPut, Not aTrdExec.Trd.IsBuy, aTrdExec.Trd.IsBuy)
        
                aUndTrdInfo.TradeDate = Now
                aUndTrdInfo.Status = enTsRealtime 'enTsManual
                aUndTrdInfo.BrokerCommission = 0#
                aUndTrdInfo.ClearingBrokerCommission = 0#
                aUndTrdInfo.Mark = aNewOptTrdInfo.Mark
        
                Set aUndTrdInfo.Trader = aNewOptTrdInfo.Trader
                Set aUndTrdInfo.Broker = aNewOptTrdInfo.Broker
                Set aUndTrdInfo.ClearingBroker = aNewOptTrdInfo.ClearingBroker
                Set aUndTrdInfo.Strategy = aNewOptTrdInfo.Strategy
                
                nLastSeqNum = gDBW.usp_Trade_SU_Exec(aNewOptTrdInfo.TradeID, aSyntRootCompAtom.UndID, _
                                                     aUndTrdInfo.Quantity, aUndTrdInfo.price, _
                                                     IIf(aUndTrdInfo.IsBuy, 1, 0), aUndTrdInfo.Status, _
                                                     aUndTrdInfo.BrokerCommission, aUndTrdInfo.ClearingBrokerCommission, _
                                                     nUndTradeSeqNum, nUndTradeID, dtUndTradeDate)
                                                     
               aUndTrdInfo.SeqNum = nUndTradeSeqNum
               aUndTrdInfo.TradeID = nUndTradeID
               aUndTrdInfo.TradeDate = GmtToLocal(dtUndTradeDate)
                                                     
                aTrdUpdate.Action = enTeNew
                CopyTradeForPub aUndTrdInfo, aTrdUpdate
                aPubManager.PubTradeUpdate aTrdUpdate
                
                m_Trd.Add aUndTrdInfo.TradeID, aUndTrdInfo.TradeDate, aUndTrdInfo
        
                AddTradeToTradesByUnd aUndTrdInfo
                AddTradeToTradesByTraderAndContract aUndTrdInfo
                AddManualTradeToTradesByUndMan aUndTrdInfo
                AddTradeToTradesByFut aUndTrdInfo
                RaiseTradeAction aUndTrdInfo, Nothing, enTaTradeNew
                Set aUndTrdInfo = Nothing
            End If
           Next
        End If
Ex:
    Set aPubManager = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec Exit", "TradeChannel"
    
    Exit Sub
EH:
    AddEvent EVENT_ERROR, "Fail to exercise/expiry trade: " & Err.Description
    Set aPubManager = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec Exit with errors", "TradeChannel"

        
End Sub

Public Sub ProcessTradeExec(ByRef aTrdExec As clsTradeExecAtom, ByVal bExercise As Boolean, aPriceOfIndex As Double, aPriceOfTrade As Double)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec Enter", "TradeChannel"
    
    Dim bSynth As Boolean
    bSynth = False
    If Not aTrdExec.Trd.OptRoot Is Nothing Then
        bSynth = aTrdExec.Trd.OptRoot.IsSynth
    End If
    
    If Not bExercise Or aTrdExec.Trd.ContractType <> enCtOption Or (aTrdExec.Trd.ContractType = enCtOption And Not bSynth) Then
        ProcessCommonTradeExec aTrdExec, aPriceOfIndex, aPriceOfTrade, bExercise
    Else
        ProcessSynteticTradeExec aTrdExec, aPriceOfIndex, aPriceOfTrade
    End If
End Sub


Private Sub RaiseTradeAction(aNewTrd As EtsGeneralLib.MmTradeInfoAtom, _
                            aOldTrd As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec RaiseTradeAction Enter", "TradeChannel"
    
    If (enAction = enTaTradeUpdate Or enTaTradeDelete) And Not aOldTrd Is Nothing Then
        If (aOldTrd.ContractType = enCtStock Or aOldTrd.ContractType = enCtIndex) Then
            
            If aOldTrd.Und.TotalQtyInShares = BAD_LONG_VALUE Then aOldTrd.Und.TotalQtyInShares = 0
            aOldTrd.Und.TotalQtyInShares = aOldTrd.Und.TotalQtyInShares - aOldTrd.Quantity * IIf(aOldTrd.IsBuy, 1, -1)
            
            If aOldTrd.Status <> enTrsSimulated And aOldTrd.Und.PriceClose >= DBL_EPSILON Then
                aOldTrd.Und.UndPosForRates = aOldTrd.Und.UndPosForRates _
                                    - aOldTrd.Quantity * aOldTrd.Und.PriceClose * IIf(aOldTrd.IsBuy, 1, -1)
            End If
        End If
    End If

    If (enAction = enTaTradeUpdate Or enTaTradeNew) And Not aNewTrd Is Nothing Then
        If (aNewTrd.ContractType = enCtStock Or aNewTrd.ContractType = enCtIndex) Then
        
            If aNewTrd.Und.TotalQtyInShares = BAD_LONG_VALUE Then aNewTrd.Und.TotalQtyInShares = 0
            aNewTrd.Und.TotalQtyInShares = aNewTrd.Und.TotalQtyInShares + aNewTrd.Quantity * IIf(aNewTrd.IsBuy, 1, -1)
        
            If aNewTrd.Status <> enTrsSimulated And aNewTrd.Und.PriceClose >= DBL_EPSILON Then
                aNewTrd.Und.UndPosForRates = aNewTrd.Und.UndPosForRates _
                                    + aNewTrd.Quantity * aNewTrd.Und.PriceClose * IIf(aNewTrd.IsBuy, 1, -1)
            End If
        End If
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec prepare to RaiseEvent", "TradeChannel"
    
    RaiseEvent TradeAction(aNewTrd, aOldTrd, enAction)

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "ProcessTradeExec RaiseTradeAction Exit", "TradeChannel"

End Sub

Private Sub AddTradeToTradesByUnd(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTradeToTradesByUnd Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.FutID = 0 Then
        sKey = CStr(aTrd.UndID)
        
        Set aTrdByUnd = m_TrdByUnd(sKey)
        If aTrdByUnd Is Nothing Then
            Set aTrdByUnd = New EtsGeneralLib.MmTradeInfoColl
            m_TrdByUnd.Add sKey, aTrdByUnd
        End If
        If Not aTrdByUnd(aTrd.TradeID) Is Nothing Then aTrdByUnd.Remove aTrd.TradeID
        aTrdByUnd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
        Set aTrdByUnd = Nothing
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTradeToTradesByUnd Exit", "TradeChannel"

End Sub

Private Sub RemoveTradeFromTradesByUnd(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByUnd Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.FutID = 0 Then
        sKey = CStr(aTrd.UndID)
        Set aTrdByUnd = m_TrdByUnd(sKey)
        If Not aTrdByUnd Is Nothing Then
            aTrdByUnd.Remove aTrd.TradeID
            If aTrdByUnd.Count <= 0 Then m_TrdByUnd.Remove sKey
            Set aTrdByUnd = Nothing
        End If
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByUnd Exit", "TradeChannel"

End Sub

Private Sub AddManualTradeToTradesByUndMan(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddManualTradeToTradesByUndMan Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.Status = enTsManual Then
        sKey = CStr(aTrd.UndID) & "_" & CStr(aTrd.Trader.ID)
        Set aTrdByUnd = m_TrdByUndMan(sKey)
        If aTrdByUnd Is Nothing Then
            Set aTrdByUnd = New EtsGeneralLib.MmTradeInfoColl
            m_TrdByUndMan.Add sKey, aTrdByUnd
        End If
        If Not aTrdByUnd(aTrd.TradeID) Is Nothing Then aTrdByUnd.Remove aTrd.TradeID
        aTrdByUnd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
        Set aTrdByUnd = Nothing
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddManualTradeToTradesByUndMan Exit", "TradeChannel"

End Sub

Private Sub RemoveManualTradeFromTradesByUndMan(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveManualTradeFromTradesByUndMan Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.Status = enTsManual Then
        sKey = CStr(aTrd.UndID) & "_" & CStr(aTrd.Trader.ID)
        Set aTrdByUnd = m_TrdByUndMan(sKey)
        If Not aTrdByUnd Is Nothing Then
            aTrdByUnd.Remove aTrd.TradeID
            If aTrdByUnd.Count <= 0 Then m_TrdByUndMan.Remove sKey
            Set aTrdByUnd = Nothing
        End If
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveManualTradeFromTradesByUndMan Exit", "TradeChannel"

End Sub

Private Sub AddTradeToTradesByTraderAndContract(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTradeToTradesByTraderAndContract Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    sKey = CStr(aTrd.Trader.ID) & "_" & CStr(aTrd.ContractID)
    Set aTrdByUnd = m_TrdByTraderAndContract(sKey)
    If aTrdByUnd Is Nothing Then
        Set aTrdByUnd = New EtsGeneralLib.MmTradeInfoColl
        m_TrdByTraderAndContract.Add sKey, aTrdByUnd
    End If
    If Not aTrdByUnd(aTrd.TradeID) Is Nothing Then aTrdByUnd.Remove aTrd.TradeID
    aTrdByUnd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
    Set aTrdByUnd = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AddTradeToTradesByTraderAndContract Exit", "TradeChannel"

End Sub

Private Sub RemoveTradeFromTradesByTraderAndContract(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByTraderAndContract Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    sKey = CStr(aTrd.Trader.ID) & "_" & CStr(aTrd.ContractID)
    Set aTrdByUnd = m_TrdByTraderAndContract(sKey)
    If Not aTrdByUnd Is Nothing Then
        aTrdByUnd.Remove aTrd.TradeID
        If aTrdByUnd.Count <= 0 Then m_TrdByTraderAndContract.Remove sKey
        Set aTrdByUnd = Nothing
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByTraderAndContract Exit", "TradeChannel"

End Sub

Private Sub AddTradeToTradesByFut(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByTraderAndContract Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.FutID <> 0 Then
        sKey = CStr(aTrd.FutID)
        Set aTrdByUnd = m_TrdByFut(sKey)
        If aTrdByUnd Is Nothing Then
            Set aTrdByUnd = New EtsGeneralLib.MmTradeInfoColl
            m_TrdByFut.Add sKey, aTrdByUnd
        End If
        If Not aTrdByUnd(aTrd.TradeID) Is Nothing Then aTrdByUnd.Remove aTrd.TradeID
        aTrdByUnd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
        Set aTrdByUnd = Nothing
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByTraderAndContract Exit", "TradeChannel"

End Sub

Private Sub RemoveTradeFromTradesByFut(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByFut Enter", "TradeChannel"
    
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, sKey$
    
    If aTrd.FutID <> 0 Then
        sKey = CStr(aTrd.FutID)
        Set aTrdByUnd = m_TrdByFut(sKey)
        If Not aTrdByUnd Is Nothing Then
            aTrdByUnd.Remove aTrd.TradeID
            If aTrdByUnd.Count <= 0 Then m_TrdByFut.Remove sKey
            Set aTrdByUnd = Nothing
        End If
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RemoveTradeFromTradesByFut Exit", "TradeChannel"

End Sub

Private Function IsValidSymbolType(ByVal enSymbolType As MSGSTRUCTLib.SYMBOL_TYPE) As Boolean
    On Error Resume Next
    IsValidSymbolType = (enSymbolType = enStIndex Or enSymbolType = enStStock _
                        Or enSymbolType = enStOption Or enSymbolType = enStFuture _
                        Or enSymbolType = enStFutureOption)
End Function

Private Function SymbolTypeToContractType(ByVal enSymbolType As MSGSTRUCTLib.SYMBOL_TYPE) As EtsGeneralLib.EtsContractTypeEnum
    On Error Resume Next
    Select Case enSymbolType
        Case enStIndex
            SymbolTypeToContractType = enCtIndex
            
        Case enStStock
            SymbolTypeToContractType = enCtStock
            
        Case enStOption
            SymbolTypeToContractType = enCtOption
            
        Case enStFuture
            SymbolTypeToContractType = enCtFuture
            
        Case enStFutureOption
            SymbolTypeToContractType = enCtFutOption
            
        Case Else
            Debug.Assert False
    End Select
End Function

Private Function ContractTypeToSymbolType(ByVal enContractType As EtsGeneralLib.EtsContractTypeEnum) As MSGSTRUCTLib.SYMBOL_TYPE
    On Error Resume Next
    Select Case enContractType
        Case enCtIndex
            ContractTypeToSymbolType = enStIndex
            
        Case enCtStock
            ContractTypeToSymbolType = enStStock
            
        Case enCtOption
            ContractTypeToSymbolType = enStOption
            
        Case enCtFuture
            ContractTypeToSymbolType = enStFuture
            
        Case enCtFutOption
            ContractTypeToSymbolType = enStFutureOption
            
        Case enCtFutUnd
            ContractTypeToSymbolType = enStFutUnd
            
        Case Else
            Debug.Assert False
    End Select
End Function

Private Function AdjustUnderlyingFuturesData(ByRef aUnd As EtsGeneralLib.UndAtom, ByVal nFutRootID&, ByVal nFutID&) As Boolean
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AdjustUnderlyingFuturesData Enter", "TradeChannel"
    
    Dim rs As ADODB.Recordset
    Dim aFutRoot As EtsGeneralLib.EtsFutRootAtom, aFut As EtsGeneralLib.EtsFutAtom
    
    AdjustUnderlyingFuturesData = False
    Set aFutRoot = Nothing
    If nFutRootID = 0 Then Exit Function
    
    Set aFutRoot = aUnd.FutRoots(nFutRootID)
    If aFutRoot Is Nothing Then
        Set rs = gDBW.usp_MmFutureRoot_Get(nFutRootID)
        If Not rs.EOF Then
            Set aFutRoot = New EtsGeneralLib.EtsFutRootAtom
            aFutRoot.ID = nFutRootID
            aFutRoot.Symbol = ReadStr(rs!vcFutureRootSymbol)
            aFutRoot.Name = ReadStr(rs!vcFutureRootName)
            aFutRoot.UndID = aUnd.ID
            aFutRoot.FutLotSize = ReadLng(rs!iFutureLotSize)
            aFutRoot.OptLotSize = ReadLng(rs!iOptionLotSize)
            
            aUnd.FutRoots.Add aFutRoot.ID, aFutRoot.Symbol, aFutRoot
        End If
        Set rs = Nothing
    End If
    
    If Not aFutRoot Is Nothing Then
        If aFutRoot.Futures(nFutID) Is Nothing Then
            Set rs = gDBW.usp_MmFuture_Get(nFutRootID)
            While Not rs.EOF
                If aFutRoot.Futures(nFutID) Is Nothing Then
                    Set aFut = New EtsGeneralLib.EtsFutAtom
                    
                    aFut.ID = nFutID
                    aFut.Symbol = ReadStr(rs!vcFutureSymbol)
                    aFut.ContractName = ReadStr(rs!vcFutureName)
                    aFut.FutRootID = nFutRootID
                    aFut.MaturityDate = ReadDate(rs!dtMaturityDate)
                    aFut.IsAmerican = (ReadByte(rs!tiCalcOptionType) <> 0)
                    aFut.IsActive = (ReadByte(rs!tiIsActive) <> 0)
                    aFut.PriceClose = ReadDbl(rs!fPriceClose)
                    aFut.HaveOptions = (ReadLng(rs!iHaveOptions) <> 0)
                    
                    Set aFut.UndPriceProfile = g_PriceProfile(ReadLng(rs!iUndPriceProfileID))
                    Set aFut.OptPriceProfile = g_PriceProfile(ReadLng(rs!iOptPriceProfileID))
                    
                    If aFut.UndPriceProfile Is Nothing Then
                        Set aFut.UndPriceProfile = g_Main.DefStkPriceProfile
                    End If
                    
                    If aFut.OptPriceProfile Is Nothing Then
                        Set aFut.OptPriceProfile = g_Main.DefOptPriceProfile
                    End If
                    
                    Debug.Assert Not aFut.UndPriceProfile Is Nothing And Not aFut.OptPriceProfile Is Nothing
                    
                    aFutRoot.Futures.Add aFut.ID, aFut.Symbol, aFut
                    Set aFut = Nothing
                End If
                rs.MoveNext
            Wend
            Set rs = Nothing
            
            AdjustUnderlyingFuturesData = (Not aFutRoot.Futures(nFutID) Is Nothing)
        Else
            AdjustUnderlyingFuturesData = True
        End If
    
        Set aFutRoot = Nothing
    End If
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AdjustUnderlyingFuturesData Exit", "TradeChannel"
    
    Exit Function
EH:
    AddEvent EVENT_ERROR, "Fail to load new future root or futures for trade. " & Err.Description
    Set aFutRoot = Nothing
    Set aFut = Nothing
    Set rs = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "AdjustUnderlyingFuturesData Exit with errors", "TradeChannel"

End Function

Public Sub AddEvent(ByVal enType As EventTypeEnum, ByVal sMessage As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, sMessage, "TradeChannel"
    
    LogEvent enType, "Trades Processor: " & sMessage
End Sub

Private Sub MmwMsgManager_OnUnderlyingUpdate(ByVal Data As MSGSTRUCTLib.IUnderlyingUpdate)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim bIsUpdated As Boolean
    
    If Data Is Nothing Then Exit Sub
    
    If (Data.UpdStatus = enUndNoChanges) Then Exit Sub
    
    bIsUpdated = CBool(Data.UpdStatus And enUndAggregationUpdate)
    If (bIsUpdated) Then
        Set aUnd = g_UnderlyingAll(Data.UndID)
        If Not aUnd Is Nothing Then
            aUnd.Coeff = Data.Coeff
            aUnd.PriceByHead = Data.UseHead
            
            RaiseEvent AssetUpdate(Data)
        End If
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enUndDividendUpdate)
    If (bIsUpdated) Then
        Set aUnd = g_UnderlyingAll(Data.UndID)
        If Not aUnd Is Nothing Then
        
            If aUnd.Dividend Is Nothing Then
                Set aUnd.Dividend = New EtsGeneralLib.EtsDivAtom
            End If
            
            If (Data.DivType = enDivIndexYield) Then
                aUnd.Dividend.DivType = Data.DivType
                aUnd.Yield = Data.Yield
            ElseIf (Data.DivType = enDivMarket) Then
                aUnd.Dividend.DivType = Data.DivType
                aUnd.Dividend.DivAmt = Data.DivAmt
                aUnd.Dividend.DivDate = Data.DivDate
                aUnd.Dividend.DivFreq = Data.DivFreq
            ElseIf (Data.DivType = enDivCustomPeriodical) Then
                aUnd.Dividend.DivType = Data.DivType
                aUnd.Dividend.DivAmtCust = Data.DivAmtCust
                aUnd.Dividend.DivDateCust = Data.DivDateCust
                aUnd.Dividend.DivFreqCust = Data.DivFreqCust
            ElseIf (Data.DivType = enDivCustomStream) Then
                aUnd.Dividend.DivType = Data.DivType
                LoadCustomDivs aUnd
            ElseIf (Data.DivType = enDivStockBasket) Then
                aUnd.Dividend.DivType = Data.DivType
            End If
            
            RaiseEvent DividendTypeUpdate(Data)
            
        End If
    End If
    
    
    bIsUpdated = CBool(Data.UpdStatus And enUndAllTheoCloseUpdate)
    If (bIsUpdated) Then
        m_TradeChannel.UpdatePriceInfo g_CurTraderID, g_Main
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enGlobalParamsUpdate)
    If (bIsUpdated) Then
        Debug.Print "Params Updated"
        If (Not g_Params Is Nothing) Then
            g_Params.ReloadCustomParams
            RaiseEvent GlobalParamsUpdate
        End If
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enPriceRoundRuleUpdate)
    If (bIsUpdated) Then
        If (Not g_Params Is Nothing) Then
            g_Params.LoadPriceRoundingRule
            RaiseEvent RoundingRuleUpdate
        End If
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enToleranceRangeUpdate)
    If (bIsUpdated) Then
        If (Not g_Params Is Nothing) Then
            g_Params.LoadUndPriceToleranceValue
            RaiseEvent ToleranceRangeUpdate
        End If
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enPriceProfileUpdate)
    If (bIsUpdated) Then
        Debug.Print "Price Profile Updated"
        If (Not g_Main Is Nothing) Then
            g_Main.LoadPriceProfile
            g_Params.FirePriceProfilesChange
            RaiseEvent PriceProfileUpdate
        End If
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enUndSettingsUpdate)
    If (bIsUpdated) Then
        Debug.Print "Underlying Settings Updated"
        Set aUnd = g_UnderlyingAll(Data.UndID)
        
        If Not aUnd Is Nothing Then
            
            If (aUnd.IsActive <> Data.IsActive) Then
                aUnd.IsActive = Data.IsActive
            End If
            
            If (aUnd.IsHedgeSymbol <> Data.IsHedgeSymbol) Then
                aUnd.IsHedgeSymbol = Data.IsHedgeSymbol
            End If
            
            If (aUnd.IsHTB <> Data.IsHTB) Then
                aUnd.IsHTB = Data.IsHTB
            End If
            
            If (aUnd.Kurt <> Data.Kurt) Then
                aUnd.Kurt = Data.Kurt
            End If
            
            Dim OptProfile As EtsGeneralLib.EtsPriceProfileAtom
            If (Not aUnd.OptPriceProfile Is Nothing) Then
                Set OptProfile = aUnd.OptPriceProfile
                If (OptProfile.ID <> Data.OptProfile) Then
                    If (Not g_Main.PriceProfile(Data.OptProfile) Is Nothing) Then
                        Set aUnd.OptPriceProfile = g_Main.PriceProfile(Data.OptProfile)
                    End If
                End If
            End If
            
            'Not realized now
            If (aUnd.PrimaryExchangeID <> Data.PrimaryExchange) Then
                'aUnd.PrimaryExchangeID = Data.PrimaryExchange
            End If
            
            If (aUnd.IsAmerican <> CBool(Data.Region)) Then
                aUnd.IsAmerican = CBool(Data.Region)
            End If
            
            If (aUnd.Skew <> Data.Skew) Then
                aUnd.Skew = Data.Skew
            End If
            
            If (aUnd.SOQ <> Data.SOQ) Then
                aUnd.SOQ = Data.SOQ
            End If
            
            Dim StkProfile As EtsGeneralLib.EtsPriceProfileAtom
            If (Not aUnd.UndPriceProfile Is Nothing) Then
                Set StkProfile = aUnd.UndPriceProfile
                If (StkProfile.ID <> Data.StkProfile) Then
                    If (Not g_Main.PriceProfile(Data.StkProfile) Is Nothing) Then
                        Set aUnd.UndPriceProfile = g_Main.PriceProfile(Data.StkProfile)
                    End If
                End If
            End If
            
            If (aUnd.IsAllowLending <> Data.AllowLending) Then
                aUnd.IsAllowLending = Data.AllowLending
            End If
            
        End If
        
        RaiseEvent UnderlyingUpdate(Data)
    End If
    
    bIsUpdated = CBool(Data.UpdStatus And enHTBRatesUpdated)
    If (bIsUpdated) Then
        Debug.Print "HTBRates updated"
        If (Data.UndID > 0) Then
            ReloadRates Data.UndID
            RaiseEvent HTBRatesUpdate(Data)
        End If
    End If
    
End Sub

Public Sub PubUnderlyingUpdate(ByRef Data As MSGSTRUCTLib.IUnderlyingUpdate)
    On Error Resume Next
    Dim aPubManager As MMWLib.IPubManager
    
    Set aPubManager = MmwMsgManager
    aPubManager.PubUnderlyingUpdate Data
    
    Set aPubManager = Nothing
    
End Sub

Public Sub PubManualPriceUpdate(ByRef Data As MSGSTRUCTLib.IManualPriceUpdate)
    On Error Resume Next
    Dim aPubManager As MMWLib.IPubManager
    
    Set aPubManager = MmwMsgManager
    aPubManager.PubManualPriceUpdate Data
    
    Set aPubManager = Nothing
    
End Sub

Private Function LoadCustomDivs(ByRef aUnd As EtsGeneralLib.UndAtom) As Boolean
    On Error Resume Next
    Dim expDate As Date
    Dim frmCustDivs As frmCustomDivs
    
    LoadCustomDivs = False
    
    expDate = Now + 365 * 5
    
    Set frmCustDivs = New frmCustomDivs
        
    frmCustDivs.Init aUnd.ID, aUnd.Symbol, expDate, aUnd.Dividend.DivType = enDivStockBasket, aUnd.Dividend.DivType = enDivCustomStream
    frmCustDivs.BasketDivs = IIf(aUnd.Dividend.DivType = enDivStockBasket, aUnd.BasketIndex.BasketDivs, Nothing)
            
    If frmCustDivs.UpdateCollection Then
        Set aUnd.Dividend.CustomDivs = frmCustDivs.CustomDividend
        LoadCustomDivs = True
    End If
    
    Set frmCustDivs = Nothing
    
End Function



