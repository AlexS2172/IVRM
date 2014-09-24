VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPosReconcileProcessor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_btFileFormat As Byte
Private m_dtUTCTradeDate As Date
Private m_frmEOD As frmEndOfDay
Private m_collPos As clsRecPosColl
Private m_collTrader As clsRecPosTraderColl
Private m_collUnd As clsRecPosUndColl
Private m_collRoots As clsRecOptRootColl
Private m_collPrices As clsRecContrPriceColl
Private m_collTraderContracts As clsTraderContractColl
Private m_collFut As clsRecPosFutColl
Private m_CollTrades As clsTradesColl
Private m_collExch As clsExchColl

Private m_collStrategy As clsStrategyColl
Private m_collStrategyByName As clsStrategyColl

Private m_collUndForVolaClear As clsRecPosUndColl

Private Const FLD_ACCOUNT = "Account"
Private Const FLD_CALLPUT = "CallPut"
Private Const FLD_BASESYM = "BaseSym"
Private Const FLD_EXPYEAR = "ExpYear"
Private Const FLD_EXPMONTH = "ExpMonth"
Private Const FLD_EXPDAY = "ExpDay"
Private Const FLD_STRIKE = "Strike"
Private Const FLD_STRIKEFRAC = "StrikeFrac"
Private Const FLD_LONGSHORT = "LongShort"
Private Const FLD_SECTYPE = "SecType"
Private Const FLD_MARKETPRICE = "MarketPrice"
Private Const FLD_POSITION = "Position"
Private Const FLD_SYMBOL = "Symbol"
Private Const FLD_STRATEGY = "Strategy"
Private Const FLD_MULTIPLIER = "ContractMultiplier"
Private Const FLD_DECIMALSTRIKE = "DecimalStrike"
Private Const FLD_CONTRACTYEAR = "ContractYear"
Private Const FLD_CONTRACTMONTH = "ContractMonth"
Private Const FLD_TRADESYMBOL = "TradeSymbol"

Private m_nMinLineLength As Long
Private m_collFieldsPosition As clsFieldPositionColl
Private m_bStrategySupport As Boolean


Private Sub Class_Initialize()
    On Error Resume Next
    m_dtUTCTradeDate = 0#
    Set m_frmEOD = Nothing
    Set m_collPos = Nothing
    Set m_collTrader = Nothing
    Set m_collUnd = Nothing
    Set m_collRoots = Nothing
    Set m_collPrices = Nothing
    Set m_collTraderContracts = Nothing
    Set m_collFut = Nothing
    Set m_collStrategy = Nothing
    Set m_collStrategyByName = Nothing
    Set m_collUndForVolaClear = Nothing
    Set m_collFieldsPosition = Nothing
    Set m_CollTrades = Nothing
    Set m_collExch = Nothing
    m_btFileFormat = 0
    m_bStrategySupport = False
End Sub

Public Sub Init(ByRef Form As frmEndOfDay)
    On Error Resume Next
    Set m_frmEOD = Form
    Set m_collPos = New clsRecPosColl
    Set m_collTrader = New clsRecPosTraderColl
    Set m_collUnd = New clsRecPosUndColl
    Set m_collRoots = New clsRecOptRootColl
    Set m_collPrices = New clsRecContrPriceColl
    Set m_collTraderContracts = New clsTraderContractColl
    Set m_collFut = New clsRecPosFutColl
    Set m_collStrategy = New clsStrategyColl
    Set m_collStrategyByName = New clsStrategyColl
    Set m_collUndForVolaClear = New clsRecPosUndColl
    Set m_collFieldsPosition = New clsFieldPositionColl
    Set m_CollTrades = New clsTradesColl
    Set m_collExch = New clsExchColl
    
    m_bStrategySupport = False
End Sub

Public Function ParseFileFormatType(ByRef sFirstString As String) As Byte

    'New reconcilation format (id 5) - start
    Dim sConfigFile$, sUserName$, sUserGroup$
    
    sUserName = CurrentNtUserName
    sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", "")
    sConfigFile = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\BOReconcilation", sUserGroup, "")
    
    If sConfigFile <> "" Then
        m_frmEOD.LogMsg "Config file: " & sConfigFile
    
        If ParseConfigFile(sConfigFile) = False Then
            Err.Raise vbObjectError, "Load Positions", "Wrong format of config file."
            Exit Function
        End If
        
        If IsConfigValid = False Then
            Err.Raise vbObjectError, "Load Positions", "Invalid config file."
            Exit Function
        End If
        
        m_dtUTCTradeDate = Now
        m_btFileFormat = 5
        ParseFileFormatType = m_btFileFormat
        Exit Function
    End If
    'New reconcilation format (id 5) - end
    
    
    Dim Items$()
    If Left(sFirstString, 6) = "BSC219" And Mid$(sFirstString, 9, 10) = "MICROHEDGE" _
            And Mid$(sFirstString, 20, 3) = "POS" Then
        On Error GoTo Format3
        m_dtUTCTradeDate = CDate(Mid$(sFirstString, 29, 8))
        On Error GoTo 0
        m_btFileFormat = 2
    Else
Format3:
        If Len(sFirstString) > 150 Then
            
            On Error GoTo Format1
            m_dtUTCTradeDate = CDate(Mid$(sFirstString, 126, 8))
            On Error GoTo 0
            If m_dtUTCTradeDate > DateSerial(1900, 1, 1) And Mid$(sFirstString, 128, 1) = "/" And Mid$(sFirstString, 131, 1) = "/" Then
                m_btFileFormat = 3
                ParseFileFormatType = m_btFileFormat
                Exit Function
            End If
        End If
Format1:
        Items = Split(sFirstString, ",")
        
        If UBound(Items) < 16 Then
            If (UBound(Items) = 10) Then 'Goldman Sachs format
                m_dtUTCTradeDate = Now()
                m_btFileFormat = 6 'Goldman Sachs #6
                ParseFileFormatType = m_btFileFormat
                Erase Items
                Exit Function
            ElseIf (UBound(Items) = 15) Then
                'Sage format without Strategy support
                m_dtUTCTradeDate = CDate(Items(0))
                m_bStrategySupport = False
                m_btFileFormat = 1
                ParseFileFormatType = m_btFileFormat
                Erase Items
                Exit Function
            Else
                Erase Items
                GoTo Format4
            End If
        End If
        
        'Sage format with Strategy support
        m_dtUTCTradeDate = CDate(Items(0))
        m_bStrategySupport = True
        m_btFileFormat = 1
        ParseFileFormatType = m_btFileFormat
        Erase Items
        Exit Function
Format4:
        
        If Len(sFirstString) > 100 Then
            
            m_dtUTCTradeDate = Now()
            
            m_btFileFormat = 4
            ParseFileFormatType = m_btFileFormat
            Exit Function
        Else
            Err.Raise vbObjectError, "Load Positions", "Wrong format of input file in line " & 1
        End If
    End If
Done:
    ParseFileFormatType = m_btFileFormat
End Function

Public Sub SetFileFormatForNetting()
    m_btFileFormat = 3
End Sub

Public Function ParseFileFormatTypeForTradesImport(ByRef sFirstString As String) As Byte
    Dim Items$()
    Items = Split(sFirstString, ",")
    
    If UBound(Items) < 11 Then
        Erase Items
        Err.Raise vbObjectError, "Trades Import", "Wrong format of input file in line " & 1
    End If
    
    m_btFileFormat = 1
    
    Erase Items
Done:
    ParseFileFormatTypeForTradesImport = m_btFileFormat
End Function

Public Function ParseFileFormatTypeForCurvesImport(ByRef sFirstString As String) As Byte
    Dim Items$()
    Items = Split(sFirstString, ",")
    
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Curves Import", "Wrong format of input file in line " & 1
    End If
    
    m_btFileFormat = 1
    
    Erase Items
Done:
    ParseFileFormatTypeForCurvesImport = m_btFileFormat
End Function

Public Function ParseFileFormatTypeForVolatilityImport(ByRef sFirstString As String) As Byte
    Dim Items$()
    Items = Split(sFirstString, ",")
    
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file in line " & 1
    End If
    
    m_btFileFormat = 1
    
    Erase Items
Done:
    ParseFileFormatTypeForVolatilityImport = m_btFileFormat
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadPositionsFromDatabase(ByVal bSetNewStrategyToOld As Boolean) As Boolean
    On Error GoTo EH
               
    LoadTraders
    LoadUnderlyings
    LoadFutures
    LoadTraderContracts
    LoadStrategies
    LoadPositions bSetNewStrategyToOld
    m_collPrices.Clear
    
    LoadPositionsFromDatabase = True
    
    Exit Function
EH:
    m_frmEOD.LogMsg "Fail to load positions from database: " & Err.Description & m_frmEOD.pbProgress.Value
    LoadPositionsFromDatabase = False
End Function

Public Function LoadDataForTradesImportFromDatabase() As Boolean
    On Error GoTo EH
               
    LoadTraders
    LoadUnderlyings
    LoadFutures
    LoadTraderContracts
    LoadStrategies
    m_collPos.Clear
    m_collPrices.Clear
    
    LoadDataForTradesImportFromDatabase = True
    
    Exit Function
EH:
    m_frmEOD.LogMsg "Fail to load data from database: " & Err.Description
    LoadDataForTradesImportFromDatabase = False
End Function

Public Function LoadDataForVolatilityImportFromDatabase() As Boolean
    On Error GoTo EH
               
    LoadUnderlyings
    m_collUndForVolaClear.Clear
    m_collPos.Clear
    m_collPrices.Clear
    
    LoadDataForVolatilityImportFromDatabase = True
    
    Exit Function
EH:
    m_frmEOD.LogMsg "Fail to load data from database: " & Err.Description
    LoadDataForVolatilityImportFromDatabase = False
End Function

Public Function SaveNewLotSizeToDatabase() As Boolean
    Dim iRootCount&, aRoot As clsRecOptRootAtom
    ' save new lot size
    If m_collRoots.Count Then
        m_frmEOD.pbProgress.Min = 0
        m_frmEOD.pbProgress.Max = m_collRoots.Count
        iRootCount = 0
                
        m_frmEOD.pbProgress.Value = 0
        
        For Each aRoot In m_collRoots
            
            If aRoot.NewLotSize <= 0 Then aRoot.NewLotSize = IIf(aRoot.OldLotSize > 0, aRoot.OldLotSize, 100)
            
            If aRoot.NewLotSize <> aRoot.OldLotSize Then
                gDBW.usp_OptionRoot_Save aRoot.RootID, Null, aRoot.NewLotSize, Null
                iRootCount = iRootCount + 1
            End If
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            m_frmEOD.Idle
        Next
        m_frmEOD.LogMsg ("Lot Size updated for " & iRootCount & " option roots")
    End If
    SaveNewLotSizeToDatabase = False
End Function

Public Function SaveClosePriceToDatabase() As Boolean
    Dim iPriceCount, aPrice As clsRecContrPriceAtom
    iPriceCount = 0
    
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Max = 100
    
    iPriceCount = m_collPrices.Count
    If iPriceCount > 0 Then m_frmEOD.pbProgress.Max = iPriceCount
    
    m_frmEOD.pbProgress.Value = 0
    iPriceCount = 0
    For Each aPrice In m_collPrices
        If SaveClosePrice(aPrice.ContractType, aPrice.ContractID, aPrice.Symbol, aPrice.ClosePrice) Then
            iPriceCount = iPriceCount + 1
            m_frmEOD.pbProgress.Value = iPriceCount
        End If
        m_frmEOD.Idle
    Next
    m_frmEOD.LogMsg ("Close price load for " & iPriceCount & " contracts")
End Function

Private Sub LoadStrategies()
    Dim rs As ADODB.Recordset, sName$, nId&
    Dim aStrategy As clsStrategyAtom
    
    m_collStrategy.Clear
    m_collStrategyByName.Clear
    
    Set rs = gDBW.usp_Strategy_Get()
    While Not rs.EOF
        nId = gCmn.ReadLng(rs!iStrategyID)
        sName = gCmn.ReadStr(rs!vcStrategyName)
        
        Set aStrategy = m_collStrategy(CStr(nId))
        If aStrategy Is Nothing Then
            Set aStrategy = m_collStrategy.Add(CStr(nId))
            aStrategy.StrategyID = nId
            aStrategy.StrategyName = sName
            
            If m_collStrategyByName(UCase$(sName)) Is Nothing Then
                m_collStrategyByName.Add UCase$(sName), aStrategy
            End If
            
            Set aStrategy = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub
Private Sub LoadExchange()
On Error GoTo ErrHandler

    Dim rs As ADODB.Recordset
    Dim aExch As clsExchAtom
    Dim sKey As String
    
    m_collExch.Clear

    Set rs = gDBW.usp_Exchange_Get(Null)
    While Not rs.EOF
        sKey = UCase$(Trim$(gCmn.ReadLng(rs!iExchangeId)))
        
        If m_collExch(sKey) Is Nothing Then
            Set aExch = m_collExch.Add(sKey)
            aExch.ID = gCmn.ReadLng(rs!iExchangeId)
            aExch.Code = gCmn.ReadStr(rs!vcExchangeCode)
            aExch.Symbol = gCmn.ReadStr(rs!vcExchangeName)
            Set aExch = Nothing
        End If
        
        rs.MoveNext
    Wend
    Set rs = Nothing
    
Exit Sub
ErrHandler:
    Debug.Print "Error on Loading Exchanges"
End Sub


Private Sub LoadTrades(Optional ByVal bSupportStrategy As Boolean = True)
    Dim rs As ADODB.Recordset, sKey$
    Dim aTrade As clsTradeAtom
    Dim nKeyCounter As Long: nKeyCounter = 0
    Dim aExch As clsExchAtom
    
    m_CollTrades.Clear
    Set rs = gDBW.usp_BoTradesForExport_Get()
    
    If (rs.RecordCount > 0) Then
        If (bSupportStrategy) Then
            rs.Sort = "lContractID, lTraderID, lStrategyID, bIsPosition DESC, lSeqNum"
        Else
            rs.Sort = "lContractID, lTraderID, bIsPosition DESC, lSeqNum"
        End If
    End If
    
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!lSeqNum)
        Set aTrade = m_CollTrades(sKey)
        If aTrade Is Nothing Then
            Set aTrade = m_CollTrades.Add(sKey)
            
            aTrade.ActionDate = gCmn.ReadDate(rs!dtActionDate)
            aTrade.BrokerID = gCmn.ReadLng(rs!lBrokerID)
            aTrade.BrokerName = gCmn.ReadStr(rs!vcBrokerName)
            aTrade.ExchangeID = gCmn.ReadLng(rs!lExchangeID)
            
            aTrade.CCY = "USD"
            If (Not m_collExch Is Nothing) Then
                If (Not m_collExch(CStr(aTrade.ExchangeID)) Is Nothing) Then
                    Set aExch = m_collExch(CStr(aTrade.ExchangeID))
                    If (aExch.Code = "TO" Or aExch.Code = "TV") Then
                        aTrade.CCY = "CAD"
                    End If
                End If
            End If
    
            aTrade.Comission = gCmn.ReadDbl(rs!fBrokerCommission)
            aTrade.ContractID = gCmn.ReadLng(rs!lContractID)
            aTrade.ContractType = gCmn.ReadLng(rs!lContractType)
            aTrade.FutureID = gCmn.ReadLng(rs!lFutureID)
            aTrade.FutureRootID = gCmn.ReadLng(rs!lFutureRootID)
            aTrade.ImportID = gCmn.ReadStr(rs!vcImportID)
            aTrade.IsBuy = gCmn.ReadByte(rs!bIsBuy)
            aTrade.LotSize = gCmn.ReadLng(rs!lLotSize)
            aTrade.OptionRootID = gCmn.ReadLng(rs!lOptionRootID)
            aTrade.Price = gCmn.ReadDbl(rs!fPrice)
            aTrade.Qty = gCmn.ReadLng(rs!lQty)
            aTrade.RealQty = aTrade.Qty * IIf(aTrade.IsBuy, 1, -1)
            aTrade.SeqNum = gCmn.ReadLng(rs!lSeqNum)
            aTrade.StrategyName = gCmn.ReadStr(rs!vcStrategyName)
            aTrade.Symbol = gCmn.ReadStr(rs!vcSymbol)
            aTrade.TradeDate = gCmn.ReadDate(rs!dtTradeDate)
            aTrade.TradeID = gCmn.ReadLng(rs!lID)
            aTrade.TraderID = gCmn.ReadLng(rs!lTraderID)
            aTrade.TraderName = gCmn.ReadStr(rs!vcTraderName)
            aTrade.StrategyID = gCmn.ReadLng(rs!lStrategyID)
            aTrade.UnderlyingID = gCmn.ReadLng(rs!lUnderlyingID)
            aTrade.IsPos = gCmn.ReadByte(rs!bIsPosition)
            
            aTrade.Strike = gCmn.ReadDbl(rs!dStrike)
            aTrade.IsCall = gCmn.ReadByte(rs!bIsCall)
            aTrade.Expiry = gCmn.ReadDate(rs!dtExpiry)
            aTrade.RootName = gCmn.ReadStr(rs!vcRootSymbol)
                       
            Set aTrade = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Function GetTradeKey(ByRef aTrade As clsTradeAtom, ByVal bSupportStrategy As Boolean) As String
    If (bSupportStrategy) Then
        GetTradeKey = CStr(aTrade.ContractID) & "_" & CStr(aTrade.TraderID) & "_" & CStr(aTrade.StrategyID)
    Else
        GetTradeKey = CStr(aTrade.ContractID) & "_" & CStr(aTrade.TraderID)
    End If
End Function

Private Function GetExternalKeyByTrade(ByRef aTrade As clsTradeAtom) As String
    If (Not aTrade Is Nothing) Then
        GetExternalKeyByTrade = "iVol" & Format(aTrade.SeqNum, "0000000") & "N"
    End If
End Function

Private Function GetTradeStringLine(ByRef aTrade As clsTradeAtom, ByVal sID As String, ByVal sTradeType As String, ByVal sCustodian As String, ByVal dFeesCoef As Double) As String
On Error GoTo ErrHandler:
    Dim sLine As String: sLine = ""
    Dim sDelimetr As String: sDelimetr = ";"
    Dim bIsOption As Boolean
    
    If (Not aTrade Is Nothing) Then
        sLine = sLine & Replace(GetExternalKeyByTrade(aTrade), "N", sID) & sDelimetr
        sLine = sLine & aTrade.Symbol & sDelimetr
        sLine = sLine & aTrade.GetOCCCode & sDelimetr
        sLine = sLine & Format(aTrade.TradeDate, "M/D/YYYY") & sDelimetr
        
        If (aTrade.ContractType = enCtFutOption Or aTrade.ContractType = enCtOption) Then
            sLine = sLine & Format(aTrade.TradeDate + 1, "M/D/YYYY") & sDelimetr
            bIsOption = True
        Else
            sLine = sLine & Format(aTrade.TradeDate + 3, "M/D/YYYY") & sDelimetr
            bIsOption = False
        End If
        
        sLine = sLine & sCustodian & sDelimetr
        sLine = sLine & aTrade.TraderName & sDelimetr
        sLine = sLine & IIf(Len(aTrade.StrategyName) > 0, aTrade.StrategyName, " ") & sDelimetr
        sLine = sLine & sTradeType & sDelimetr
        sLine = sLine & CStr(aTrade.RealQty) & sDelimetr
        sLine = sLine & CStr(Abs(aTrade.RealQty)) & sDelimetr
        sLine = sLine & CStr(aTrade.Price) & sDelimetr
        sLine = sLine & CStr(aTrade.Price * IIf(bIsOption, aTrade.LotSize, 1)) & sDelimetr
        sLine = sLine & CStr(aTrade.Price * CDbl(IIf(bIsOption, aTrade.LotSize, 1) * Abs(aTrade.RealQty)) * dFeesCoef) & sDelimetr
        sLine = sLine & IIf(Len(aTrade.BrokerName) > 0, aTrade.BrokerName, " ") & sDelimetr
        sLine = sLine & aTrade.CCY & vbCrLf
        
        GetTradeStringLine = sLine
    End If
    
Exit Function
ErrHandler:
    Debug.Print "Error on getting trade export text line."
End Function

Public Sub ExportTradesCFI(ByVal sFileName As String, Optional ByVal bSupportStrategy As Boolean = True)
On Error GoTo ErrHandler:
    Dim aTrd As clsTradeAtom
    Dim aTrdS As clsTradeAtom
    Dim sKey As String
    Dim nPosQty As Long: nPosQty = 0
    Dim nTrdQty As Long: nTrdQty = 0
    Dim sCustodian As String
    Dim dFeesCoef As Double
    
    Dim hFile As Long
    
    hFile = FreeFile
    If hFile = 0 Then
        Exit Sub
    End If
    
    Open sFileName For Binary Access Write As #hFile
    
    
    sCustodian = g_Params.CustodianDesc
    dFeesCoef = g_Params.SECFees
    
    Dim aTradeComputeColl As clsTradesColl
    Set aTradeComputeColl = New clsTradesColl
    
    LoadExchange
    
    If (Not m_CollTrades Is Nothing) Then
        If (m_CollTrades.Count = 0) Then
            LoadTrades bSupportStrategy
        End If
        
        'Begin export procedure
        For Each aTrd In m_CollTrades
        
            sKey = GetTradeKey(aTrd, bSupportStrategy)
            Set aTrdS = aTradeComputeColl(sKey)
            
            If (aTrdS Is Nothing) Then
                Set aTrdS = aTradeComputeColl.Add(sKey)
                aTrdS.RealQty = 0
                If (aTrd.IsPos) Then
                    aTrdS.RealQty = aTrd.RealQty
                End If
            End If
            
            nPosQty = aTrdS.RealQty
            nTrdQty = aTrd.RealQty
            
            If (Not aTrd.IsPos) Then ' Export only trades
                If (aTrd.ContractType = enCtFutOption Or aTrd.ContractType = enCtOption) Then
                    Put #hFile, , GetTradeStringLine(aTrd, "1", IIf(aTrd.IsBuy, "Buy", "Sell"), sCustodian, dFeesCoef)
                Else
                    If (nPosQty = 0) Then
                        Put #hFile, , GetTradeStringLine(aTrd, "1", IIf(aTrd.IsBuy, "Buy", "Sell Short"), sCustodian, dFeesCoef)
                    Else
                        If (nPosQty > 0) Then 'Long position
                            If (nTrdQty > 0) Then
                                Put #hFile, , GetTradeStringLine(aTrd, "1", "Buy", sCustodian, dFeesCoef)
                            Else
                                If (nPosQty + nTrdQty >= 0) Then
                                    Put #hFile, , GetTradeStringLine(aTrd, "1", "Sell", sCustodian, dFeesCoef)
                                Else
                                    aTrd.RealQty = -nPosQty
                                    Put #hFile, , GetTradeStringLine(aTrd, "1", "Sell", sCustodian, dFeesCoef)
                                    aTrd.RealQty = nPosQty + nTrdQty
                                    Put #hFile, , GetTradeStringLine(aTrd, "2", "Sell Short", sCustodian, dFeesCoef)
                                    aTrd.RealQty = aTrd.Qty * IIf(aTrd.IsBuy, 1, -1)
                                End If
                            End If
                        Else
                            If (nTrdQty < 0) Then
                                Put #hFile, , GetTradeStringLine(aTrd, "2", "Sell Short", sCustodian, dFeesCoef)
                            Else
                                If (nPosQty + nTrdQty > 0) Then
                                    aTrd.RealQty = Abs(nPosQty)
                                    Put #hFile, , GetTradeStringLine(aTrd, "1", "Buy Cover", sCustodian, dFeesCoef)
                                    aTrd.RealQty = nPosQty + nTrdQty
                                    Put #hFile, , GetTradeStringLine(aTrd, "2", "Buy", sCustodian, dFeesCoef)
                                    aTrd.RealQty = aTrd.Qty * IIf(aTrd.IsBuy, 1, -1)
                                Else
                                    Put #hFile, , GetTradeStringLine(aTrd, "1", "Buy Cover", sCustodian, dFeesCoef)
                                End If
                            End If
                        End If
                    End If
                End If
                aTrdS.RealQty = aTrdS.RealQty + nTrdQty
            End If
                        
            Set aTrdS = Nothing
        Next
    End If
    
    Set aTradeComputeColl = Nothing
    
    Close #hFile
    
Exit Sub
ErrHandler:
    Debug.Print "Error on trade export. " & Err.Description
End Sub

Private Sub LoadTraders()
    Dim rs As ADODB.Recordset, sKey$
    Dim aTrader As clsRecPosTraderAtom
    
    m_collTrader.Clear
    Set rs = gDBW.usp_BoTrader_Get()
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!vcAcronym)
        Set aTrader = m_collTrader(sKey)
        If aTrader Is Nothing Then
            Set aTrader = m_collTrader.Add(sKey)
            aTrader.ID = gCmn.ReadLng(rs!iTraderID)
            aTrader.Acronym = sKey
            Set aTrader = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Sub LoadTraderContracts()
    Dim rs As ADODB.Recordset, sKey$
    Dim aTrdCnt As clsTraderContractAtom
    
    m_collTraderContracts.Clear
    Set rs = gDBW.usp_BoTraderContract_Get(Null, Null)
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!iTraderID) & "_" & gCmn.ReadStr(rs!iContractID)
        Set aTrdCnt = m_collTraderContracts(sKey)
        If aTrdCnt Is Nothing Then
            Set aTrdCnt = m_collTraderContracts.Add(sKey)
            aTrdCnt.TraderID = gCmn.ReadLng(rs!iTraderID)
            aTrdCnt.ContractID = gCmn.ReadLng(rs!iContractID)
            Set aTrdCnt = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Sub LoadUnderlyings()
    Dim rs As ADODB.Recordset, sKey$
    Dim aUnd As clsRecPosUndAtom
    
    m_collUnd.Clear
    Set rs = gDBW.usp_BoUnderlyings_Get()
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!vcSymbol)
        Set aUnd = m_collUnd(sKey)
        If aUnd Is Nothing Then
            Set aUnd = m_collUnd.Add(sKey)
            aUnd.ID = gCmn.ReadLng(rs!iContractID)
            aUnd.ContractType = gCmn.ReadLng(rs!iContractTypeID)
            aUnd.Symbol = sKey
            Set aUnd = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Sub LoadFutures()
    Dim rs As ADODB.Recordset, sKey$, sExpSymbol$
    Dim aFut As clsRecPosFutAtom
    
    m_collFut.Clear
    Set rs = gDBW.usp_BoFuture_Get()
    While Not rs.EOF
        sExpSymbol = UCase$(Trim$(gCmn.ReadStr(rs!vcFutureRootExportSymbol)))
        If Len(sExpSymbol) = 0 Then
            sKey = "0_" & gCmn.ReadStr(rs!vcFutureRootSymbol) & "_" & Trim$(Str$(gCmn.ReadDate(rs!dtMaturityDate)))
        Else
            sKey = "1_" & sExpSymbol & "_" & Trim$(Str$(gCmn.ReadStr(rs!dtMaturityDate)))
        End If
        
        Set aFut = m_collFut(sKey)
        If aFut Is Nothing Then
            Set aFut = m_collFut.Add(sKey)
            aFut.ID = gCmn.ReadLng(rs!iContractID)
            aFut.Symbol = gCmn.ReadStr(rs!vcSymbol)
            aFut.Maturity = gCmn.ReadDate(rs!dtMaturityDate)
            aFut.IndexSymbol = gCmn.ReadStr(rs!vcUnderlyingSymbol)
            aFut.ExportSymbol = gCmn.ReadStr(rs!vcFutureRootExportSymbol)
                
            aFut.IndexId = gCmn.ReadLng(rs!iUnderlyingID)
            aFut.FutureRootID = gCmn.ReadLng(rs!iFutureRootID)
            aFut.SymbolDisplay = gCmn.ReadStr(rs!vcFutureRootSymbol) & " " & UCase(Format$(aFut.Maturity, "MMMYY"))
            
            Set aFut = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
End Sub

Private Sub LoadPositions(ByVal bSetNewStrategyToOld As Boolean)
    Dim rs As ADODB.Recordset, sKey$, iContractTypeID&, sExpSymbol$
    Dim aPos As clsRecPosAtom, aRoot As clsRecOptRootAtom
    Dim iKeyTypeValue
    Dim bUseStrategy  As Boolean
    
    bUseStrategy = (m_btFileFormat = 3 Or m_btFileFormat = 5 Or m_bStrategySupport)
    m_collPos.Clear
    Set rs = gDBW.usp_BoPositionForReconcile_Get(m_dtUTCTradeDate, bUseStrategy)
    If rs.RecordCount > 0 Then
        m_frmEOD.pbProgress.Max = rs.RecordCount
        While Not rs.EOF
            iContractTypeID = gCmn.ReadLng(rs!iContractTypeID)
            iKeyTypeValue = iContractTypeID
            If iKeyTypeValue = 1 Then
                iKeyTypeValue = 2
            End If
            If Not bUseStrategy Then
                sKey = gCmn.ReadStr(rs!vcSymbol) & "_" & gCmn.ReadStr(rs!vcAcronym) & "_" & CStr(iKeyTypeValue)
            Else
                sKey = gCmn.ReadStr(rs!vcSymbol) & "_" & gCmn.ReadStr(rs!vcAcronym) & "_" & CStr(iKeyTypeValue) & "_" & UCase$(gCmn.ReadStr(rs!vcStrategyName))
            End If
            
            Set aPos = m_collPos(sKey)
            If aPos Is Nothing Then
                Set aPos = m_collPos.Add(sKey)
                aPos.UndSymbol = gCmn.ReadStr(rs!vcUnderlyingSymbol)
                Set aPos.Und = m_collUnd(aPos.UndSymbol)
                
                If iContractTypeID > 3 Then
                    sExpSymbol = UCase$(Trim$(gCmn.ReadStr(rs!vcFutureRootExportSymbol)))
                    If Len(sExpSymbol) = 0 Then
                        sKey = "0_" & gCmn.ReadStr(rs!vcFutureRootSymbol) & "_" & Trim$(Format$(gCmn.ReadDate(rs!dtFutureMaturityDate)))
                    Else
                        sKey = "1_" & sExpSymbol & "_" & Trim$(Format$(gCmn.ReadDate(rs!dtFutureMaturityDate)))
                    End If
                    
                    Set aPos.Fut = m_collFut(sKey)

                    Debug.Assert Not aPos.Fut Is Nothing
                    
                    If Not aPos.Fut Is Nothing Then
                        If iContractTypeID = 5 Then
                            Set aPos.FutOpt = aPos.Fut.FindFutureOption(gCmn.ReadByte(rs!tiIsCall), gCmn.ReadDbl(rs!fStrike), gCmn.ReadDate(rs!dtExpiryDate), gCmn.ReadLng(rs!iContractID))
                            
                            If aPos.FutOpt Is Nothing Then
                                Err.Raise vbObjectError, AppTitle(), "Unexpected error was occured during reading positions from DB. Invalid future options data."
                            End If
                        End If
                    Else
                        Err.Raise vbObjectError, AppTitle(), "Unexpected error was occured during reading positions from DB. Invalid future data."
                    End If
                    
                End If
                
                Debug.Assert Not aPos.Und Is Nothing
                
                If Not aPos.Und Is Nothing Then
                    If gCmn.ReadLng(rs!iContractTypeID) = 3 Then
                        Dim contract_id As Long
                        Dim root_symbol As String
                        Dim expiration_date As Date
                        Dim Strike As Double
                        
                        contract_id = gCmn.ReadLng(rs!iContractID)
                        root_symbol = gCmn.ReadStr(rs!vcOptionRootSymbol)
                        expiration_date = gCmn.ReadDate(rs!dtExpiryDate)
                        Strike = gCmn.ReadDbl(rs!fStrike)
                        
                        Set aPos.Opt = aPos.Und.FindOption(root_symbol, gCmn.ReadByte(rs!tiIsCall), Strike, expiration_date, contract_id)
                        
                        If aPos.Opt Is Nothing Then
                            MsgBox "ID: " & CStr(contract_id) & _
                                    " Root: " & root_symbol & _
                                    " Expiry: " & CStr(expiration_date) & _
                                    " Strike: " & CStr(Strike)
                                    
                            Err.Raise vbObjectError, AppTitle(), "Unexpected error was occured during reading positions from DB. Invalid options data."
                        End If
                    End If
                Else
                    Err.Raise vbObjectError, AppTitle(), "Unexpected error was occured during reading positions from DB. Invalid stock data."
                End If
                Set aPos.Trader = m_collTrader(gCmn.ReadStr(rs!vcAcronym))
                
                aPos.QtyOld = gCmn.ReadLng(rs!iQty)
                aPos.PriceOld = gCmn.ReadDbl(rs!fPriceClose)
                aPos.LotSizeOld = gCmn.ReadLng(rs!iLotSize)
                aPos.PosValidOld = True
                
                aPos.QtyNew = 0
                aPos.PriceNew = 0
                aPos.LotSizeNew = 0
                aPos.PosValidNew = False
                
                aPos.TradeDate = m_dtUTCTradeDate
                aPos.bFix = False
                aPos.bFixPrice = False
                
                If Not aPos.Opt Is Nothing Then
                    If aPos.Opt.Expiry < m_dtUTCTradeDate Then
                        aPos.bFix = True
                    End If
                    
                    sKey = Trim$(Str$(gCmn.ReadLng(rs!iOptionRootID)))
                    Set aRoot = m_collRoots(sKey)
                    
                    If aRoot Is Nothing Then
                        Set aRoot = m_collRoots.Add(sKey)
                        aRoot.OldLotSize = gCmn.ReadLng(rs!iLotSize)
                        aRoot.Symbol = gCmn.ReadStr(rs!vcOptionRootSymbol)
                        aRoot.RootID = gCmn.ReadLng(rs!iOptionRootID)
                    End If
                    Set aRoot = Nothing
                    
                End If
                
                If Not aPos.Fut Is Nothing Then
                    If aPos.Fut.Maturity < m_dtUTCTradeDate Then
                        aPos.bFix = True
                    End If
                End If
                
                If Not aPos.FutOpt Is Nothing Then
                    If aPos.FutOpt.Expiry < m_dtUTCTradeDate Then
                        aPos.bFix = True
                    End If
                End If
                
                If aPos.QtyOld = 0 Then
                    aPos.bFix = True
                End If
                
                If aPos.PriceOld <= 0# Then
                    aPos.bFixPrice = True
                End If
                                
                aPos.StrategyOldID = gCmn.ReadLng(rs!iStrategyID)
                aPos.StrategyOld = gCmn.ReadStr(rs!vcStrategyName)

                If bSetNewStrategyToOld And aPos.StrategyOldID <> 0 Then
                    aPos.StrategyForTradeID = aPos.StrategyOldID
                    aPos.StrategyForTrade = aPos.StrategyOld
                End If

                Set aPos = Nothing
            End If
            
            On Error Resume Next
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            On Error GoTo 0
            m_frmEOD.Idle
            rs.MoveNext
        Wend
    End If
    Set rs = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Parses text stream and builds recordset
'
Public Function LoadPositionsFromText(ByVal Buffer As String) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim bStructureExist As Boolean, nNewPosCount&

    nNewPosCount = 0
    bStructureExist = True
    On Error GoTo Herr
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    If m_btFileFormat = 2 Or m_btFileFormat = 6 Then
        nBegin = 1
    Else
        nBegin = 0
    End If
    
    nEnd = UBound(Lines)
    
    m_frmEOD.pbProgress.Max = UBound(Lines) + 1
    For nLineNumber = nBegin To nEnd
        On Error GoTo Herr
        
        sLine = Lines(nLineNumber)
        
        If m_btFileFormat = 1 Then
            If ParseLineOfFileFormatSage(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        ElseIf m_btFileFormat = 2 Then
            If ParseLineOfFileFormatMicroHedge(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        ElseIf m_btFileFormat = 3 Then
            If ParseLineOfFileFormatCIBC(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        ElseIf m_btFileFormat = 4 Then
            If ParseLineOfFileFormatClark(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        ElseIf m_btFileFormat = 5 Then
            If ParseLineOfFileFormatNew(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        ElseIf m_btFileFormat = 6 Then
            If ParseLineOfFileFormatGoldmanSachs(nLineNumber, sLine, bStructureExist) Then nNewPosCount = nNewPosCount + 1
        End If
        
        On Error Resume Next
        m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
        On Error GoTo 0
        m_frmEOD.Idle
    Next
    
    If bStructureExist Then
        LoadPositionsFromText = True
    Else
        If gCmn.MyMsgBox("Contracts without market structure was found." & _
                        " You must to refresh market structure before import positions." & _
                        " Continue anyway?", vbExclamation + vbYesNo) = vbNo Then
                        
            m_frmEOD.LogMsg "Position Reconciliation canceled."
            LoadPositionsFromText = False
        Else
            LoadPositionsFromText = True
        End If
    End If
    
Done:
    On Error GoTo 0
    If nNewPosCount = 0 Then
        Err.Raise vbObjectError, , "No positions found"
        Exit Function
    End If
    Erase Lines
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function
Private Function FindFuture(ByVal sFutName As String) As clsRecPosFutAtom
    Dim aFut As clsRecPosFutAtom
    For Each aFut In m_collFut
        If (aFut.Symbol = sFutName) Then
            Set FindFuture = aFut
            Exit Function
        End If
    Next
End Function

Private Function ParseLineOfFileFormatSage(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom
    Dim sAccountAcronym$, sStockSymbol$, sImportSymbol$, sStrategy$
    Dim Items$(), dtTradeDate As Date, sOptionRoot$, dStrike#, sOptionType$, nPosition&, dOptionMark#, dUnderlierStockMark#
    Dim nContractMultiplier&, sOptionStock$, dtExpirationDate As Date, sCol$, sValue$, dPrice#, dUndPrice#
    Dim aFut As clsRecPosFutAtom
    Dim aFop As clsRecPosFutOptAtom
    Dim sKey As String
    
    ParseLineOfFileFormatSage = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    '16 - Strategy Support or 15 - Strategy Not Supported
    If Not (UBound(Items) = 15 Or (UBound(Items) = 16 And m_bStrategySupport)) Then
        Erase Items
        Err.Raise vbObjectError, "Load Positions", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    dtTradeDate = 0#: sAccountAcronym = "": sStockSymbol = "": sOptionRoot = ""
    dStrike = 0#: sOptionType = "": nPosition = 0&: dOptionMark = 0#: dUnderlierStockMark = 0#
    nContractMultiplier = 0: sOptionStock = "": dtExpirationDate = 0#: sImportSymbol = ""
    sStrategy = ""
    
    sCol = "Trade Date":           sValue = Items(0):  If Len(sValue) > 4 Then dtTradeDate = CDate(sValue)
    sCol = "Account Acronym":      sValue = Items(1):  If Len(sValue) > 0 Then sAccountAcronym = sValue
    sCol = "Stock  Symbol":        sValue = Items(2):  If Len(sValue) > 0 Then sStockSymbol = sValue
    sCol = "Import Symbol":        sValue = Items(3):  If Len(sValue) > 0 Then sImportSymbol = sValue
    sCol = "Option Root":          sValue = Items(4):  If Len(sValue) > 0 Then sOptionRoot = sValue
    sCol = "Strike":               sValue = Items(6):  If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Option Type":          sValue = Items(7):  If Len(sValue) > 0 Then sOptionType = sValue
    sCol = "Position":             sValue = Items(8):  If Len(sValue) > 0 Then nPosition = gCmn.ReadLng(sValue)
    sCol = "Option Mark":          sValue = Items(9):  If Len(sValue) > 0 Then dOptionMark = gCmn.ReadDbl(sValue)
    sCol = "Underlier Stock Mark": sValue = Items(10): If Len(sValue) > 0 Then dUnderlierStockMark = gCmn.ReadDbl(sValue)
    sCol = "Contract Multiplier":  sValue = Items(11): If Len(sValue) > 0 Then nContractMultiplier = gCmn.ReadLng(sValue)
    sCol = "Option/Stock":         sValue = Items(12): If Len(sValue) > 0 Then sOptionStock = sValue
    sCol = "Expiration Date":      sValue = Items(13): If Len(sValue) > 4 Then dtExpirationDate = CDate(sValue)
    If (m_bStrategySupport) Then
        sCol = "Strategy":             sValue = Items(16): If Len(Trim(sValue)) > 0 Then sStrategy = sValue
    End If
    sCol = ""
    sValue = ""
    
    If sAccountAcronym = "" Then
        sCol = "Account Acronym"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Acronym"
    ElseIf sStockSymbol = "" Then
        sCol = "Stock Symbol"
        Err.Raise vbObjectError, "Load Positions", "Empty Stock Symbol"
    ElseIf dtTradeDate = 0 Then
        sCol = "Trade Date"
        Err.Raise vbObjectError, "Load Positions", "Empty Trade Date"
    ElseIf (sOptionStock = "OPTION" Or sOptionStock = "FUTOPT") Then
        If sOptionRoot = "" Then
            sCol = "Option Root"
            Err.Raise vbObjectError, "Load Positions", "Empty Option Root"
        ElseIf dtExpirationDate = 0 Then
            sCol = "Expiration Date"
            Err.Raise vbObjectError, "Load Positions", "Invalid Expiration Date"
        ElseIf dStrike = 0 Then
            sCol = "Strike"
            Err.Raise vbObjectError, "Load Positions", "Invalid Strike"
        End If
    ElseIf (m_bStrategySupport And sStrategy = "") Then
        sCol = "Strategy"
        Err.Raise vbObjectError, "Load Positions", "Invalid Strategy Name"
    End If

    On Error GoTo 0
        
    Set aTrader = FindTraderByName(sAccountAcronym)
    
    'check underlying for contract
    If (sOptionStock = "FUTURE" Or sOptionStock = "FUTOPT") Then
        Set aFut = FindFuture(sStockSymbol)
        If (aFut Is Nothing) Then
            m_frmEOD.LogMsg "Position skipped. Future not in database: " & sStockSymbol & " " & UCase(Format$(dtExpirationDate, "YYMMM")) & " Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        CheckTraderContract aTrader.ID, aFut.IndexId
    Else 'STOCK or OPTION
        Set aUnd = m_collUnd(sStockSymbol)
        If aUnd Is Nothing Then
            ' There is no attepmt to add new asset to database.
            ' This caused errors in position files from broker with invalid symbols
            ' Added by Eugene Karnygin to prevent errors.
            ' If Asset is not in database it should be added from master database not position file
            m_frmEOD.LogMsg "Position skipped. Asset: " & sStockSymbol & " is not in database. Please contact support to add the asset into database. After that start reconcilation again."
            'Set aUnd = AddNewUnderlyingToDatabase(sStockSymbol)
            
            bStructureExist = False
            Exit Function
        End If
        
        CheckTraderContract aTrader.ID, aUnd.ID
    End If
    
    'check option for contract
    If sOptionStock = "OPTION" Then
    
        Set aOpt = aUnd.FindOption(sOptionRoot, IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database: " & sOptionRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", stock " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        
        dPrice = dOptionMark
        dUndPrice = dUnderlierStockMark
    ElseIf sOptionStock = "FUTOPT" Then
    
        Set aFop = aFut.FindFutureOption(IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If (aFop Is Nothing) Then
            m_frmEOD.LogMsg "Position skipped. Option not in database: " & sOptionRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", future " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        
        dPrice = dOptionMark
        dUndPrice = dUnderlierStockMark
    Else
        dPrice = dUnderlierStockMark
    End If
        
    'Add positon from file to collection
    AddPositionFromText aOpt, aUnd, aFop, aFut, aTrader, nPosition, dPrice, nContractMultiplier, sImportSymbol, sStrategy
    
    'load price from file
    AddPriceFromText aOpt, aUnd, aFop, aFut, dPrice, dUndPrice
    
    If sOptionStock = "OPTION" Then
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If
    
    Erase Items
    ParseLineOfFileFormatSage = True
    Exit Function
ParseError:
    Erase Items
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function
Private Function ParseGSSymbol(ByVal sSymbol As String, ByVal sSeqType) As Object
    ''''''''''''''''''''''''''''''''''''''''''''''''
    Dim aOpt As New clsRecPosOptAtom
    Dim aUnd As New clsRecPosUndAtom
    Dim aFut As New clsRecPosFutAtom
    ''''''''''''''''''''''''''''''''''''''''''''''''
    If (sSeqType = "OPTION") Then
        
        ParseGSSymbol = aOpt
    ElseIf (sSeqType = "EQUITY") Then
        ParseGSSymbol = aUnd
    ElseIf (sSeqType = "FUTURE") Then
        ParseGSSymbol = aFut
    End If
End Function
Private Function ParseLineOfFileFormatGoldmanSachs(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    Dim aOpt As clsRecPosOptAtom
    Dim aUnd As clsRecPosUndAtom
    Dim aTrader As clsRecPosTraderAtom
    Dim aFut As clsRecPosFutAtom
    Dim aFop As clsRecPosFutOptAtom
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    Dim sAccount$, dtTradeDate As Date, sBuySell$, nQty As Long, sSymbol$, sUnderlyingSymbol$, sCallPut$
    Dim dPrice As Double, dUndPrice As Double, sSeqType$, sOPRACode$, sValue$
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    Dim Items$()
    Dim nContractMultiplier&, dtExpiryDate As Date, dtMaturityDate As Date, sCol$, sImportSymbol$
    Dim sKey$, sOptionRoot$, sStrike$, dStrike As Double, sFracStrike$, sExpiryYear$, sExpiryMonth$
    '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    
    ParseLineOfFileFormatGoldmanSachs = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 10 Then
        Erase Items
        Err.Raise vbObjectError, "Load Positions", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    
    dtTradeDate = 0#:        sAccount = "": sCallPut = ""
    nContractMultiplier = 0: sSeqType = "": dtExpiryDate = 0#
    
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    sCol = "AccountNumber":      sValue = Items(0):  If Len(sValue) > 0 Then sAccount = Trim(sValue)
    sCol = "TradeDate":          sValue = Items(1):  If Len(sValue) > 4 Then dtTradeDate = CDate(sValue)
    sCol = "BuySell":            sValue = Items(2):  If Len(sValue) > 0 Then sBuySell = Trim(sValue)
    sCol = "Quantity":           sValue = Items(3):  If Len(sValue) > 0 Then nQty = gCmn.ReadLng(sValue)
    sCol = "Symbol":             sValue = Items(4):  If Len(sValue) > 0 Then sSymbol = sValue
    sCol = "UnderlyingSymbol":   sValue = Items(5):  If Len(sValue) > 0 Then sUnderlyingSymbol = Trim(sValue)
    sCol = "USD Price":          sValue = Items(6):  If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue)
    sCol = "Investment Type":    sValue = Items(9):  If Len(sValue) > 0 Then sSeqType = Trim(sValue)
    sCol = "OPRA Code":          sValue = Items(10): sOPRACode = Trim(sValue)
    sCol = ""
    sValue = ""
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    ''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    If sAccount = "" Then
        sCol = "AccountNumber"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Number"
    ElseIf sUnderlyingSymbol = "" Then
        sCol = "UnderlyingSymbol"
        Err.Raise vbObjectError, "Load Positions", "Empty Underlying Symbol"
    ElseIf dtTradeDate = 0 Then
        sCol = "Trade Date"
        Err.Raise vbObjectError, "Load Positions", "Empty Trade Date"
    ElseIf (sSeqType = "OPTION") Then 'exapmle "AZN  JUL 07   55 1/2 C"
    
        sOptionRoot = Trim(Mid(sSymbol, 1, 4))
        sExpiryMonth = Trim(Mid(sSymbol, 6, 3))
        sExpiryYear = Trim(Mid(sSymbol, 10, 2))
        sStrike = Trim(Mid(sSymbol, 13, 4))
        sFracStrike = Trim(Mid(sSymbol, 18, 3))
        sCallPut = Trim(Mid(sSymbol, 22, 3))
        
        dtExpiryDate = CDate("1," & sExpiryMonth & "," & sExpiryYear)
        'make strike part
        dStrike = gCmn.ReadDbl(sStrike)
        If (Len(sFracStrike) > 0) Then
            Dim sUp$, sDown$
            sUp = Mid(sFracStrike, 1, 1)
            sDown = Mid(sFracStrike, 3, 1)
            dStrike = dStrike + gCmn.ReadDbl(sUp) / gCmn.ReadDbl(sDown)
            dStrike = Round(dStrike, 3)
        End If
        
        sOPRACode = Replace(sOPRACode, " ", "")
        
    ElseIf (sSeqType = "EQUITY") Then
        sSymbol = Trim(sSymbol)
    ElseIf (sSeqType = "FUTURE") Then
        sSymbol = Trim(sSymbol)
    End If

    On Error GoTo 0

    Set aTrader = FindTraderByName(sAccount)

    If (sSeqType = "FUTURE" Or sSeqType = "FUTOPT") Then
        Set aFut = FindFuture(sSymbol)
        If (aFut Is Nothing) Then
            m_frmEOD.LogMsg "Position skipped. Future not in database: " & sUnderlyingSymbol & " " & UCase(Format$(dtExpiryDate, "YYMMM")) & " Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        CheckTraderContract aTrader.ID, aFut.IndexId
        sImportSymbol = aFut.Symbol
    ElseIf (sSeqType = "EQUITY" Or sSeqType = "OPTION") Then
        Set aUnd = m_collUnd(sUnderlyingSymbol)
        If (aUnd Is Nothing) Then
            Set aUnd = AddNewUnderlyingToDatabase(sUnderlyingSymbol)
            bStructureExist = False
        End If
        CheckTraderContract aTrader.ID, aUnd.ID
        sImportSymbol = aUnd.Symbol
    End If

    If sSeqType = "OPTION" Then
        Set aOpt = aUnd.FindOption(sOptionRoot, IIf(sCallPut = "C", 1, 0), dStrike, dtExpiryDate, 0, 0, sOPRACode)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database: " & sOptionRoot & UCase(Format$(dtExpiryDate, "YYMMM")) & Format$(Int(dStrike)) & sCallPut & ", stock " & sUnderlyingSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        sImportSymbol = aOpt.Symbol
    ElseIf sSeqType = "FUTOPT" Then
        Set aFop = aFut.FindFutureOption(IIf(sCallPut = "C", 1, 0), dStrike, dtExpiryDate)
        If (aFop Is Nothing) Then
            m_frmEOD.LogMsg "Position skipped. Option not in database: " & sOptionRoot & UCase(Format$(dtExpiryDate, "YYMMM")) & Format$(Int(dStrike)) & sCallPut & ", future " & sUnderlyingSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        sImportSymbol = aFop.Symbol
    Else
        dPrice = 0 'dUnderlierStockMark
    End If

    AddPositionFromText aOpt, aUnd, aFop, aFut, aTrader, nQty, dPrice, nContractMultiplier, sImportSymbol, ""

    AddPriceFromText aOpt, aUnd, aFop, aFut, dPrice, dUndPrice

    If sSeqType = "OPTION" Then
        nContractMultiplier = 100
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If

    Erase Items
    ParseLineOfFileFormatGoldmanSachs = True
    Exit Function
ParseError:
    Erase Items
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function ParseLineOfFileFormatMicroHedge(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom
    Dim sAccountAcronym$, sStockSymbol$, sOptionRoot$, sTradeSymbol$, sLongShort$, dStrike#, sOptionType$
    Dim nPosition&, dOptionMark#, dUnderlierStockMark#, sCol$, sValue$, dPrice#, nNewPosCount&
    Dim nContractMultiplier&, sOptionStock$, dtExpirationDate As Date, iExpYear&, iExpMonth&, iExpDay&
    
    ParseLineOfFileFormatMicroHedge = False

    If Len(sLine) = 0 Or Left(sLine, 12) = "RECORD COUNT" Or Left(sLine, 11) = "END-OF-FILE" Then Exit Function
    
    If Len(sLine) < 80 Then
        m_frmEOD.LogMsg "Position skipped. String #" & nLineNumber & " has length less then 80"
        Exit Function
    End If
    
    On Error GoTo ParseError
    sAccountAcronym = "": sOptionRoot = "": sStockSymbol = "": sTradeSymbol = "": sLongShort = ""
    dStrike = 0#: sOptionType = "": nPosition = 0&: dOptionMark = 0#: dUnderlierStockMark = 0#
    nContractMultiplier = 0#: sOptionStock = "": dtExpirationDate = 0#: iExpYear = 0#: iExpMonth = 0#: iExpDay = 0#
    
    If Left(sLine, 3) <> "346" Then
        Err.Raise vbObjectError, "Load Positions", "Position skipped. String #" & nLineNumber & " started not with '346' liter"
    End If
    
    If Trim$(Mid$(sLine, 4, 1)) <> "" Then ' process only position string
        Exit Function
    End If
                
    sCol = "Account":              sValue = Trim$(Mid$(sLine, 9, 10)):   If Len(sValue) > 0 Then sAccountAcronym = sValue
    sCol = "Option Type":          sValue = Trim$(Mid$(sLine, 19, 1)):   If Len(sValue) > 0 Then sOptionType = sValue
    sCol = "Stock  Symbol":        sValue = Trim$(Mid$(sLine, 20, 6)):   If Len(sValue) > 0 Then sStockSymbol = sValue
    sCol = "Expiration Year":      sValue = Trim$(Mid$(sLine, 26, 2)):   If Len(sValue) > 0 Then iExpYear = gCmn.ReadLng(sValue)
    sCol = "Expiration Month":     sValue = Trim$(Mid$(sLine, 28, 2)):   If Len(sValue) > 0 Then iExpMonth = gCmn.ReadLng(sValue)
    sCol = "Expiration Day":       sValue = Trim$(Mid$(sLine, 87, 2)):   If Len(sValue) > 0 Then iExpDay = gCmn.ReadLng(sValue)
    sCol = "Strike dollar":        sValue = Trim$(Mid$(sLine, 30, 5)):   If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Strike fraq":          sValue = Trim$(Mid$(sLine, 35, 2)):   If Len(sValue) > 0 Then dStrike = dStrike + gCmn.ReadDbl(sValue) / 8
    sCol = "Long/Short":           sValue = Trim$(Mid$(sLine, 37, 1)):   If Len(sValue) > 0 Then sLongShort = sValue
    sCol = "Option/Stock":         sValue = Trim$(Mid$(sLine, 38, 1)):   If Len(sValue) > 0 Then sOptionStock = sValue
    sCol = "Trade Price":          sValue = Trim$(Mid$(sLine, 39, 10)):  If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue) / 1000000
    sCol = "Position":             sValue = Trim$(Mid$(sLine, 59, 7)):   If Len(sValue) > 0 Then nPosition = gCmn.ReadLng(sValue)
    sCol = "Trade Symbol":         sValue = Trim$(Mid$(sLine, 81, 3)):  If Len(sValue) > 0 Then sOptionRoot = sValue
    sCol = "Contract Multiplier":  sValue = Trim$(Mid$(sLine, 98, 5)):  If Len(sValue) > 0 Then nContractMultiplier = gCmn.ReadLng(sValue)
    sCol = "Decimal Strike":       sValue = Trim$(Mid$(sLine, 89, 9)):   If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue) / 10000
    
    sCol = ""
    sValue = ""
    
    dtExpirationDate = DateSerial(iExpYear, iExpMonth, iExpDay)
    nPosition = nPosition * IIf(sLongShort = "S", -1, 1)
    
    If sAccountAcronym = "" Then
        sCol = "Account Acronym"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Acronym"
    ElseIf sStockSymbol = "" Then
        sCol = "Stock Symbol"
        Err.Raise vbObjectError, "Load Positions", "Empty Stock Symbol"
    ElseIf sOptionStock = "O" Then
        If dtExpirationDate = 0 Then
            sCol = "Expiration Date"
            Err.Raise vbObjectError, "Load Positions", "Invalid Expiration Date"
        ElseIf dStrike = 0 Then
            sCol = "Strike"
            Err.Raise vbObjectError, "Load Positions", "Invalid Strike"
        End If
    End If

    On Error GoTo 0
        
    If sOptionStock <> "O" Then
        nContractMultiplier = 1
    End If
        
    ' check trader
    Set aTrader = FindTraderByName(sAccountAcronym)

    ' check underlying
    Set aUnd = m_collUnd(sStockSymbol)
    If aUnd Is Nothing Then
        Set aUnd = AddNewUnderlyingToDatabase(sStockSymbol)
        bStructureExist = False
    End If
    
    CheckTraderContract aTrader.ID, aUnd.ID
    
    If sOptionStock = "O" Then
    
        Set aOpt = aUnd.FindOption(sOptionRoot, IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database or not unique: " & sOptionRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", stock " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    Else
    End If
    
    AddPositionFromText aOpt, aUnd, Nothing, Nothing, aTrader, nPosition, dPrice, nContractMultiplier, "", ""
    
    AddPriceFromText aOpt, aUnd, Nothing, Nothing, dPrice
    
    If sOptionStock = "O" Then
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If
    
    nNewPosCount = nNewPosCount + 1
    ParseLineOfFileFormatMicroHedge = True
    Exit Function
ParseError:
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function ParseLineOfFileFormatCIBC(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom
    Dim sStockSymbol$, sPortiaId$, bIsCall As Boolean, dStrike#, sUndPortiaId$
    Dim nUndContructType&, sAccountAcronym$, sFutureRootSymbol$
    Dim aFutOpt As clsRecPosFutOptAtom, aFut As clsRecPosFutAtom
    Dim sStrategy$, sContactType$, nPosition&, sCol$, sValue$, sKey$, sOptionStock$
    Dim dPrice#, nContractMultiplier&, dtTradeDate As Date, nNewPosCount&
    Dim dtExpiryDate As Date
    Dim dtMaturityDate As Date
    
    m_bStrategySupport = True
    ParseLineOfFileFormatCIBC = False
    If Len(sLine) = 0 Then Exit Function
    
    If Len(sLine) < 150 Then
        m_frmEOD.LogMsg "Position skipped. String #" & nLineNumber & " has length less then 150"
        Exit Function
    End If
        
    On Error GoTo ParseError
    sAccountAcronym = "": sStrategy = "": sPortiaId = "": sContactType = "": nPosition = 0&
    dPrice = 0#: nContractMultiplier = 0&: dtTradeDate = 0#: sUndPortiaId = "": sFutureRootSymbol = ""
                    
    sCol = "Account":              sValue = Trim$(Mid$(sLine, 1, 15)):     If Len(sValue) > 0 Then sAccountAcronym = sValue
    sCol = "Strategy/Hedge group": sValue = Trim$(Mid$(sLine, 16, 15)):   If Len(sValue) > 0 Then sStrategy = sValue
    sCol = "Contract Portia ID":   sValue = Trim$(Mid$(sLine, 31, 15)):   If Len(sValue) > 0 Then sPortiaId = sValue
    sCol = "Contract type":        sValue = Trim$(Mid$(sLine, 66, 15)):   If Len(sValue) > 0 Then sContactType = UCase$(sValue)
    sCol = "Position":             sValue = Trim$(Mid$(sLine, 81, 15)):   If Len(sValue) > 0 Then nPosition = gCmn.ReadLng(sValue)
    sCol = "Close Price":          sValue = Trim$(Mid$(sLine, 96, 13)):  If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue)
    sCol = "Contract Multiplier":  sValue = Trim$(Mid$(sLine, 114, 5)):  If Len(sValue) > 0 Then nContractMultiplier = gCmn.ReadLng(sValue)
    sCol = "Date":                 sValue = Trim$(Mid$(sLine, 126, 8)):  If Len(sValue) = 8 Then dtTradeDate = gCmn.ReadDate(sValue)
    sCol = "Underlying Portia Id": sValue = Trim$(Mid$(sLine, 134, 20)):   If Len(sValue) > 0 Then sUndPortiaId = sValue
    
    sCol = ""
    sValue = "<null>"
                
    If sAccountAcronym = "" Then
        sCol = "Account Acronym"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Acronym"
    End If
    
    sValue = sPortiaId
    If sContactType = "OP" Then
        sCol = "Contract Portia ID"
        ParseCIBCOptPortiaId sPortiaId, bIsCall, dtExpiryDate, dStrike
        sCol = "Underlying Portia ID"
        ParseCIBCUndPortiaId sUndPortiaId, sStockSymbol, nUndContructType
    ElseIf sContactType = "ST" Or sContactType = "CVPFD" Then
        sCol = "Contract Portia ID"
        ParseCIBCUndPortiaId sPortiaId, sStockSymbol, nUndContructType
    ElseIf sContactType = "FUT" Then
        sCol = "Contract Portia ID"
        ParseCIBCUndPortiaId sUndPortiaId, sStockSymbol, nUndContructType
        ParseCIBCFutPortiaId sPortiaId, sFutureRootSymbol, dtMaturityDate
    ElseIf sContactType = "DEVON OPTION" Then
        sCol = "Contract Portia ID"
        ParseCIBCFutOptPortiaId sPortiaId, bIsCall, dtExpiryDate, dStrike
        ParseCIBCFutPortiaId sUndPortiaId, sFutureRootSymbol, dtMaturityDate
    Else
        sCol = "Contract type"
        sValue = sContactType
        Err.Raise vbObjectError, "Load Positions", "Invalid contruct type"
    End If
    
    If nPosition = 0& Then
        sCol = "Position"
        sValue = Trim$(Mid$(sLine, 81, 15))
        Err.Raise vbObjectError, "Load Positions", "Invalid Position"
    End If
    
    If nContractMultiplier <= 0& Then
        sCol = "Contract Multiplier"
        sValue = Trim$(Mid$(sLine, 114, 5))
        Err.Raise vbObjectError, "Load Positions", "Invalid Contract Multiplier"
    End If
    
    If dtTradeDate > m_dtUTCTradeDate Then
        sCol = "Date"
        sValue = CStr(dtTradeDate)
        Err.Raise vbObjectError, "Load Positions", "Invalid date"
    End If
        
    ' check future option
    If sContactType = "DEVON OPTION" Then
        ' try to find mapped root symbol first
        sKey = "1_" & sFutureRootSymbol & "_" & Trim$(Format$(dtMaturityDate))
        Set aFut = m_collFut(sKey)
        
        If aFut Is Nothing Then ' if not found then try to find normal symbol
            sKey = "0_" & sFutureRootSymbol & "_" & Trim$(Format$(dtMaturityDate))
            Set aFut = m_collFut(sKey)
        End If
        
        Set aFut = m_collFut(sKey)
        If aFut Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Future not in database: " & sUndPortiaId & ", its future options will be skipped also. Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        sStockSymbol = aFut.IndexSymbol
        
        Set aFutOpt = aFut.FindFutureOption(bIsCall, dStrike, dtExpiryDate, 0, sPortiaId)
        If aFutOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Future option not in database or not unique: " & sPortiaId & ", future: " & sUndPortiaId & ", index: " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    End If
                
    On Error GoTo 0
    ' check underlying
    Set aUnd = m_collUnd(sStockSymbol)
    If aUnd Is Nothing Then
        Set aUnd = AddNewUnderlyingToDatabase(sStockSymbol, nUndContructType)
        bStructureExist = False
    End If
    
    ' check futures
    If sContactType = "FUT" Then
        ' try to find mapped root symbol first
        sKey = "1_" & sFutureRootSymbol & "_" & Trim$(Format$(dtMaturityDate))
        Set aFut = m_collFut(sKey)
        
        If aFut Is Nothing Then ' if not found then try to find normal symbol
            sKey = "0_" & sFutureRootSymbol & "_" & Trim$(Format$(dtMaturityDate))
            Set aFut = m_collFut(sKey)
        End If
        
        Set aFut = m_collFut(sKey)
        If aFut Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Future not in database: " & sPortiaId & ", index: " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        Else
            If aFut.IndexSymbol <> sStockSymbol Then
                m_frmEOD.LogMsg "Position skipped. Future with import ID " & sPortiaId & " of index " & sStockSymbol & " is assigned to another index " & aFut.IndexSymbol & " in " & AppIvRmTitle() & " . Please correct a file or market structure. (line " & nLineNumber & ")"
                bStructureExist = False
                Exit Function
            End If
        End If
    End If

    ' check trader
    Set aTrader = FindTraderByName(sAccountAcronym)
        
    CheckTraderContract aTrader.ID, aUnd.ID
    
    If sContactType = "OP" Then
        Set aOpt = aUnd.FindOptionsNoRoot(sStockSymbol, bIsCall, dStrike, dtExpiryDate, sPortiaId)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database or not unique: " & sPortiaId & ", stock " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    End If
    
    AddPositionFromText aOpt, aUnd, aFutOpt, aFut, aTrader, nPosition, dPrice, nContractMultiplier, sPortiaId, sStrategy
    
    AddPriceFromText aOpt, aUnd, aFutOpt, aFut, dPrice
    
    If sOptionStock = "OP" Then
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If
    
    nNewPosCount = nNewPosCount + 1
    ParseLineOfFileFormatCIBC = True
    Exit Function
ParseError:
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function ParseLineOfFileFormatClark(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom
    Dim sAccountAcronym$, sStockSymbol$, sOptionRoot$, sTradeSymbol$, sLongShort$, dStrike#, sOptionType$
    Dim nPosition&, dOptionMark#, dUnderlierStockMark#, sCol$, sValue$, dPrice#, nNewPosCount&
    Dim nContractMultiplier&, sOptionStock$, dtExpirationDate As Date, iExpYear&, iExpMonth&, iExpDay&
    
    ParseLineOfFileFormatClark = False

    'If Len(sLine) = 0 Or Left(sLine, 12) = "RECORD COUNT" Or Left(sLine, 11) = "END-OF-FILE" Then Exit Function
    
    If Len(sLine) < 100 Then
        m_frmEOD.LogMsg "Position skipped. String #" & nLineNumber & " has length less then 100"
        Exit Function
    End If
    
    On Error GoTo ParseError
    sAccountAcronym = "": sOptionRoot = "": sStockSymbol = "": sTradeSymbol = "": sLongShort = ""
    dStrike = 0#: sOptionType = "": nPosition = 0&: dOptionMark = 0#: dUnderlierStockMark = 0#
    nContractMultiplier = 0#: sOptionStock = "": dtExpirationDate = 0#: iExpYear = 0#: iExpMonth = 0#: iExpDay = 1#
    
    If Left(sLine, 3) <> "346" Then
        Err.Raise vbObjectError, "Load Positions", "Position skipped. String #" & nLineNumber & " started not with '346' liter"
    End If
    
    If Trim$(Mid$(sLine, 4, 1)) <> "" Then ' process only position string
        Exit Function
    End If
                
    sCol = "Account":              sValue = Trim$(Mid$(sLine, 9, 8)):   If Len(sValue) > 0 Then sAccountAcronym = sValue
    sCol = "Option Type":          sValue = Trim$(Mid$(sLine, 19, 1)):   If Len(sValue) > 0 Then sOptionType = sValue
    sCol = "Stock  Symbol":        sValue = Trim$(Mid$(sLine, 20, 10)):   If Len(sValue) > 0 Then sStockSymbol = sValue
    sCol = "Expiration Year":      sValue = Trim$(Mid$(sLine, 30, 4)):   If Len(sValue) > 0 Then iExpYear = gCmn.ReadLng(sValue)
    sCol = "Expiration Month":     sValue = Trim$(Mid$(sLine, 34, 2)):   If Len(sValue) > 0 Then iExpMonth = gCmn.ReadLng(sValue)
    'sCol = "Expiration Day":       sValue = Trim$(Mid$(sLine, 87, 2)):   If Len(sValue) > 0 Then iExpDay = gCmn.ReadLng(sValue)
    sCol = "Strike dollar":        sValue = Trim$(Mid$(sLine, 36, 5)):   If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Strike fraq":          sValue = Trim$(Mid$(sLine, 41, 2)):   If Len(sValue) > 0 Then dStrike = dStrike + gCmn.ReadDbl(sValue) / 8
    sCol = "Long/Short":           sValue = Trim$(Mid$(sLine, 43, 1)):   If Len(sValue) > 0 Then sLongShort = sValue
    sCol = "Option/Stock":         sValue = Trim$(Mid$(sLine, 44, 1)):   If Len(sValue) > 0 Then sOptionStock = sValue
    sCol = "Trade Price":          sValue = Trim$(Mid$(sLine, 45, 10)):  If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue) / 1000000
    sCol = "Position":             sValue = Trim$(Mid$(sLine, 62, 10)):   If Len(sValue) > 0 Then nPosition = gCmn.ReadLng(sValue)
    sCol = "Trade Symbol":         sValue = Trim$(Mid$(sLine, 87, 10)):  If Len(sValue) > 0 Then sOptionRoot = sValue
    'sCol = "Contract Multiplier":  sValue = Trim$(Mid$(sLine, 98, 5)):  If Len(sValue) > 0 Then nContractMultiplier = gCmn.ReadLng(sValue)
    'sCol = "Decimal Strike":       sValue = Trim$(Mid$(sLine, 89, 9)):   If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue) / 10000
    
    sCol = ""
    sValue = ""
    
    dtExpirationDate = DateSerial(iExpYear, iExpMonth, iExpDay)
    nPosition = nPosition * IIf(sLongShort = "S", -1, 1)
    
    If sAccountAcronym = "" Then
        sCol = "Account Acronym"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Acronym"
    ElseIf sStockSymbol = "" Then
        sCol = "Stock Symbol"
        Err.Raise vbObjectError, "Load Positions", "Empty Stock Symbol"
    ElseIf sOptionStock = "O" Then
        If dtExpirationDate = 0 Then
            sCol = "Expiration Date"
            Err.Raise vbObjectError, "Load Positions", "Invalid Expiration Date"
        ElseIf dStrike = 0 Then
            sCol = "Strike"
            Err.Raise vbObjectError, "Load Positions", "Invalid Strike"
        End If
    End If

    On Error GoTo 0
        
    If sOptionStock <> "O" Then
        nContractMultiplier = 1
    End If
        
    ' check trader
    Set aTrader = FindTraderByName(sAccountAcronym)

    ' check underlying
    Set aUnd = m_collUnd(sStockSymbol)
    If aUnd Is Nothing Then
        Set aUnd = AddNewUnderlyingToDatabase(sStockSymbol)
        bStructureExist = False
    End If
    
    CheckTraderContract aTrader.ID, aUnd.ID
    
    If sOptionStock = "O" Then
    
        Set aOpt = aUnd.FindOption(sOptionRoot, IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database or not unique: " & sOptionRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", stock " & sStockSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    Else
    End If
    
    AddPositionFromText aOpt, aUnd, Nothing, Nothing, aTrader, nPosition, dPrice, nContractMultiplier, "", ""
    
    AddPriceFromText aOpt, aUnd, Nothing, Nothing, dPrice
    
    If sOptionStock = "O" Then
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If
    
    nNewPosCount = nNewPosCount + 1
    ParseLineOfFileFormatClark = True
    Exit Function
ParseError:
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function ParseLineOfFileFormatNew(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom, aFut As clsRecPosFutAtom
    Dim sAccountAcronym$, sUndSymbol$, sOptionRoot$, sLongShort$, dStrike#, dStrikeFrac#, sOptionType$
    Dim nPosition&, dOptionMark#, dUnderlierStockMark#, sCol$, sValue$, dPrice#, nNewPosCount&
    Dim nContractMultiplier&, sSeqType$, dtExpirationDate As Date, iExpYear&, iExpMonth&, iExpDay&, sStrategy$
    Dim iContractYear&, iContractMonth&, sTradeSymbol$, sFutureMaturityCodes$, dtMaturityDate As Date
    
    ParseLineOfFileFormatNew = False

    If Len(sLine) < m_nMinLineLength Then
        m_frmEOD.LogMsg "Position skipped. String #" & nLineNumber & " has length less then " & m_nMinLineLength
        Exit Function
    End If
    
    On Error GoTo ParseError
    sAccountAcronym = "": sOptionRoot = "": sUndSymbol = "": sTradeSymbol = "": sLongShort = ""
    dStrike = 0#: dStrikeFrac = 0#: sOptionType = "": nPosition = 0&: dOptionMark = 0#: dUnderlierStockMark = 0#
    nContractMultiplier = 0#: sSeqType = "": dtExpirationDate = 0#: iExpYear = 0#: iExpMonth = 0#: iExpDay = 1#
    iContractYear = 0#: iContractMonth = 0#: sTradeSymbol = ""
    sFutureMaturityCodes = "FGHJKMNQUVXZ"
    
    If Left(sLine, 3) <> "346" Then
        Err.Raise vbObjectError, "Load Positions", "Position skipped. String #" & nLineNumber & " started not with '346' liter"
    End If
    
    If Trim$(Mid$(sLine, 4, 1)) <> "" Then ' process only position string
        Exit Function
    End If

    Dim aPos As clsFieldPositionAtom
    For Each aPos In m_collFieldsPosition
        sCol = aPos.Key
        sValue = Trim$(Mid$(sLine, aPos.nStart, aPos.nLength))
    
        If aPos.Key = FLD_ACCOUNT Then
            If Len(sValue) > 0 Then sAccountAcronym = sValue
        ElseIf aPos.Key = FLD_CALLPUT Then
            If Len(sValue) > 0 Then sOptionType = sValue
        ElseIf aPos.Key = FLD_BASESYM Then
            If Len(sValue) > 0 Then sUndSymbol = sValue
        ElseIf aPos.Key = FLD_EXPYEAR Then
            If Len(sValue) > 0 Then iExpYear = gCmn.ReadLng(sValue)
        ElseIf aPos.Key = FLD_EXPMONTH Then
            If Len(sValue) > 0 Then iExpMonth = gCmn.ReadLng(sValue)
        ElseIf aPos.Key = FLD_EXPDAY Then
            If Len(sValue) > 0 Then iExpDay = gCmn.ReadLng(sValue)
        ElseIf aPos.Key = FLD_STRIKE Then
            If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
        ElseIf aPos.Key = FLD_STRIKEFRAC Then
            If Len(sValue) > 0 Then dStrikeFrac = gCmn.ReadDbl(sValue) / IIf(aPos.nOptional > 0, aPos.nOptional, 1)
        ElseIf aPos.Key = FLD_LONGSHORT Then
            If Len(sValue) > 0 Then sLongShort = sValue
        ElseIf aPos.Key = FLD_SECTYPE Then
            If Len(sValue) > 0 Then sSeqType = sValue
        ElseIf aPos.Key = FLD_MARKETPRICE Then
            If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue) / 1000000
        ElseIf aPos.Key = FLD_POSITION Then
            If Len(sValue) > 0 Then nPosition = gCmn.ReadLng(sValue)
        ElseIf aPos.Key = FLD_STRATEGY Then
            If Len(sValue) > 0 Then
                    sStrategy = sValue
                    m_bStrategySupport = True
            End If
        ElseIf aPos.Key = FLD_SYMBOL Then
            If Len(sValue) > 0 Then sOptionRoot = sValue
        ElseIf aPos.Key = FLD_MULTIPLIER Then
            If Len(sValue) > 0 Then nContractMultiplier = gCmn.ReadLng(sValue)
        ElseIf aPos.Key = FLD_DECIMALSTRIKE Then
            If dStrike = 0 And sSeqType = "O" Then
                If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue) / 10000
            End If
        ElseIf aPos.Key = FLD_CONTRACTYEAR Then
            If (Len(Trim(sValue))) = 4 Then
                iContractYear = gCmn.ReadLng(sValue)
            End If
        ElseIf aPos.Key = FLD_CONTRACTMONTH Then
            If (Len(Trim(sValue))) = 2 Then
                iContractMonth = gCmn.ReadLng(sValue)
            End If
        ElseIf aPos.Key = FLD_TRADESYMBOL Then
            If (Len(Trim(sValue)) > 0) Then
                sTradeSymbol = Trim(sValue)
            End If
        End If
    Next
    
    dStrike = dStrike + dStrikeFrac
    
    sCol = ""
    sValue = ""
    
    dtExpirationDate = DateSerial(iExpYear, iExpMonth, iExpDay) 'option support
    dtMaturityDate = DateSerial(iContractYear, iContractMonth, 1) 'futures support
    
    nPosition = nPosition * IIf(sLongShort = "S", -1, 1)
    
    If sAccountAcronym = "" Then
        sCol = "Account Acronym"
        Err.Raise vbObjectError, "Load Positions", "Empty Account Acronym"
    ElseIf sSeqType = "" Then
        sCol = "Underlying Symbol"
        Err.Raise vbObjectError, "Load Positions", "Empty Underlying Symbol"
    ElseIf sSeqType = "O" Then
        If dtExpirationDate = 0 Then
            sCol = "Expiration Date"
            Err.Raise vbObjectError, "Load Positions", "Invalid Expiration Date"
        ElseIf dStrike = 0 Then
            sCol = "Strike"
            Err.Raise vbObjectError, "Load Positions", "Invalid Strike"
        End If
    ElseIf sSeqType = "F" Then
        If (iContractMonth < 1 Or iContractMonth > 12 Or iContractYear < 2000) Then
            sCol = "Maturity Date"
            Err.Raise vbObjectError, "Load Positions", "Invalid Maturity Date"
        End If
    End If

    On Error GoTo 0
        
    If sSeqType <> "O" Then
        nContractMultiplier = 1
    End If
        
    ' check trader
    Set aTrader = FindTraderByName(sAccountAcronym)

    ' check underlying
    If (sSeqType = "F") Then 'only futures support, not future options
        'Make future symbol by MaturityDate and RootSymbol
        sUndSymbol = sTradeSymbol & "/" & Format(dtMaturityDate, "YY") & Mid(sFutureMaturityCodes, Month(dtMaturityDate), 1)
        'try to find it, if no found rise error, and not add it to DB
        Set aFut = FindFuture(sUndSymbol)
        If (aFut Is Nothing) Then
            'need to update market sructure
            m_frmEOD.LogMsg "Position skipped. Future not in database: " & sUndSymbol & " " & UCase(Format$(dtMaturityDate, "YYMMM")) & " Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        CheckTraderContract aTrader.ID, aFut.IndexId
    ElseIf (sSeqType = "O" Or sSeqType = "S") Then
        Set aUnd = m_collUnd(sUndSymbol)
        If aUnd Is Nothing Then
            Set aUnd = AddNewUnderlyingToDatabase(sUndSymbol)
            bStructureExist = False
            Exit Function
        End If
        CheckTraderContract aTrader.ID, aUnd.ID
    End If

    
    If (sSeqType = "O") Then
        Set aOpt = aUnd.FindOption(sOptionRoot, IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Position skipped. Option not in database or not unique: " & sOptionRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", stock " & sUndSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    End If
    
    AddPositionFromText aOpt, aUnd, Nothing, aFut, aTrader, nPosition, dPrice, nContractMultiplier, "", sStrategy
    
    AddPriceFromText aOpt, aUnd, Nothing, aFut, dPrice
    
    If sSeqType = "O" Then
        AddOptionRootFromText aOpt, aUnd, nContractMultiplier
    End If
    
    nNewPosCount = nNewPosCount + 1
    ParseLineOfFileFormatNew = True
    Exit Function
ParseError:
    m_frmEOD.LogMsg "Position skipped. Error parsing positions data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function FindTraderByName(ByRef sTraderName As String) As clsRecPosTraderAtom
    Dim aTrader As clsRecPosTraderAtom
    Set aTrader = m_collTrader(sTraderName)
    If aTrader Is Nothing Then
        Set aTrader = AddNewTraderToDatabase(sTraderName)
    End If
    Set FindTraderByName = aTrader
End Function

Private Function AddNewTraderToDatabase(ByRef sTraderName As String) As clsRecPosTraderAtom
    Dim nId&, aTrader As clsRecPosTraderAtom
    nId& = gDBW.usp_BoTrader_Save(Null, sTraderName, "Imported trader " & sTraderName, sTraderName)
    
    If nId > 0 Then
        Set aTrader = m_collTrader.Add(sTraderName)
        aTrader.ID = nId
        aTrader.Acronym = sTraderName
        m_frmEOD.LogMsg "Add new trader to database: '" & sTraderName & "'"
        Set AddNewTraderToDatabase = aTrader
    Else
        Err.Raise vbObjectError, "Add New Trader", "Fail to add new trader '" & sTraderName & "' to database."
    End If
End Function

Private Function AddNewUnderlyingToDatabase(ByVal sContractName As String, Optional ByVal nContractType As Long = 2) As clsRecPosUndAtom
    Dim nId&, aUnd As clsRecPosUndAtom
    
    If nContractType = 2 Then
            nId = gDBW.usp_Stock_Save(, , sContractName, , , , , , , , , 0, 1, , , 1)
        If nId > 0 Then
            Set aUnd = m_collUnd.Add(sContractName)
            aUnd.ID = nId
            aUnd.ContractType = 2
            aUnd.Symbol = sContractName
            m_frmEOD.LogMsg "Add new stock to database: '" & sContractName & "'. Please update market structure."
            Set AddNewUnderlyingToDatabase = aUnd
        Else
            Err.Raise vbObjectError, "Add New Stock", "Fail to add new stock '" & sContractName & "' to database."
        End If
    ElseIf nContractType = 1 Then
        nId = gDBW.usp_Index_Save(, sContractName, , , , , , , , 1)
        
        If nId > 0 Then
            Set aUnd = m_collUnd.Add(sContractName)
            aUnd.ID = nId
            aUnd.ContractType = 1
            aUnd.Symbol = sContractName
            m_frmEOD.LogMsg "Add new index to database: '" & sContractName & "'. Please update market structure."
            Set AddNewUnderlyingToDatabase = aUnd
        Else
            Err.Raise vbObjectError, "Add New Index", "Fail to add new index '" & sContractName & "' to database."
        End If
    End If
    
End Function

Private Sub AddPositionFromText(ByRef aOpt As clsRecPosOptAtom, _
                                ByRef aUnd As clsRecPosUndAtom, _
                                ByRef aFutOpt As clsRecPosFutOptAtom, _
                                ByRef aFut As clsRecPosFutAtom, _
                                ByRef aTrader As clsRecPosTraderAtom, _
                                ByVal nPosition As Long, _
                                ByVal dPrice As Double, _
                                ByVal nContractMultiplier As Long, _
                                ByVal sImportSymbol As String, _
                                ByVal sStrategy As String)
    Dim aPos As clsRecPosAtom, sKey$, aStrategy As clsStrategyAtom
    
    If Not aOpt Is Nothing Then
        sKey = aOpt.Symbol & "_" & aTrader.Acronym & "_3"

    ElseIf Not aFutOpt Is Nothing Then
        sKey = aFutOpt.Symbol & "_" & aTrader.Acronym & "_5"
    ElseIf Not aFut Is Nothing Then
        sKey = aFut.Symbol & "_" & aTrader.Acronym & "_4"
    Else
        sKey = aUnd.Symbol & "_" & aTrader.Acronym & "_2"
    End If
    
    If m_btFileFormat = 3 Or m_bStrategySupport Then
        sKey = sKey & "_" & UCase$(sStrategy)
    End If
    
    Set aPos = m_collPos(sKey)
    If aPos Is Nothing Then
        Set aPos = m_collPos.Add(sKey)
        
        If (Not aUnd Is Nothing) Then
            aPos.UndSymbol = aUnd.Symbol
        Else
            aPos.UndSymbol = aFut.IndexSymbol
        End If
        
        Set aPos.Und = aUnd
        Set aPos.Opt = aOpt
        Set aPos.Fut = aFut
        Set aPos.FutOpt = aFutOpt
        Set aPos.Trader = aTrader
        
        aPos.QtyOld = 0
        aPos.PriceOld = 0
        aPos.LotSizeOld = 0
        aPos.PosValidOld = False
                
    ElseIf aPos.PosValidNew Then
        If Len(sImportSymbol) > 0 Then
            sKey = "(" & aTrader.Acronym & ", " & sImportSymbol
        Else
            sKey = "(" & aTrader.Acronym & ", "
            If Not aOpt Is Nothing Then
                sKey = sKey & aOpt.Symbol
            ElseIf Not aFutOpt Is Nothing Then
                sKey = sKey & aFutOpt.Symbol
            ElseIf Not aFut Is Nothing Then
                sKey = sKey & aFut.Symbol
            Else
                sKey = sKey & aUnd.Symbol
            End If
        End If
        
        If m_btFileFormat = 3 Or m_bStrategySupport Then
            sKey = sKey & ", " & sStrategy & ")"
            Err.Raise vbObjectError, "Add Position from Text", "Fail to add new position for trader, contract and strategy '" & sKey & "' from position file. Combination (Trader, Contract, Strategy) must be unique in file!"
        Else
            sKey = sKey & ")"
            Err.Raise vbObjectError, "Add Position from Text", "Fail to add new position for trader and contract pair '" & sKey & "' from position file. Pair (Trader, Contract) must be unique in file!"
        End If
    End If
    
    'If Len(aPos.StrategyOld) = 0 Then LoadPositionStrategy aUnd.Id, aPos
    
    aPos.PosValidNew = True
    aPos.QtyNew = nPosition
    aPos.PriceNew = dPrice
    aPos.LotSizeNew = nContractMultiplier
    aPos.TradeDate = m_dtUTCTradeDate
    aPos.StrategyNew = Left$(Trim$(sStrategy), 12)
    
    Set aStrategy = m_collStrategyByName(UCase$(sStrategy))
    If Not aStrategy Is Nothing Then
        aPos.StrategyNewID = aStrategy.StrategyID
        aPos.StrategyNew = aStrategy.StrategyName
        Set aStrategy = Nothing
    End If
    
    If aPos.StrategyOldID <> 0 Then
        aPos.StrategyForTradeID = aPos.StrategyOldID
        aPos.StrategyForTrade = aPos.StrategyOld
    Else
        aPos.StrategyForTradeID = aPos.StrategyNewID
        aPos.StrategyForTrade = aPos.StrategyNew
    End If
    
    aPos.bFix = True
    aPos.bFixPrice = True
    aPos.ImportSymbol = sImportSymbol
    
End Sub

Private Sub AddPriceFromText(ByRef aOpt As clsRecPosOptAtom, _
                            ByRef aUnd As clsRecPosUndAtom, _
                            ByRef aFutOpt As clsRecPosFutOptAtom, _
                            ByRef aFut As clsRecPosFutAtom, _
                            ByVal dPrice As Double, _
                            Optional ByVal dUndPrice As Double = -1#)
    Dim aPrice As clsRecContrPriceAtom, sKey$
    
    If Not aOpt Is Nothing Then
        sKey = "3_" + Trim$(Str$(aOpt.ID))
        
        Set aPrice = m_collPrices(sKey)
        If aPrice Is Nothing Then
            Set aPrice = m_collPrices.Add(sKey)
            aPrice.ClosePrice = dPrice
            aPrice.ContractID = aOpt.ID
            aPrice.ContractType = 3
            aPrice.Symbol = aOpt.Symbol
        End If
                
        If dUndPrice > 0# Then
            sKey = Trim$(Str$(aUnd.ContractType)) + "_" + Trim$(Str$(aUnd.ID))
            Set aPrice = m_collPrices(sKey)
            If aPrice Is Nothing Then
                Set aPrice = m_collPrices.Add(sKey)
                aPrice.ClosePrice = dUndPrice
                aPrice.ContractID = aUnd.ID
                aPrice.ContractType = aUnd.ContractType
                aPrice.Symbol = aUnd.Symbol
            End If
        End If
    ElseIf Not aFutOpt Is Nothing Then
        sKey = "5_" + Trim$(Str$(aFutOpt.ID))
        
        Set aPrice = m_collPrices(sKey)
        If aPrice Is Nothing Then
            Set aPrice = m_collPrices.Add(sKey)
            aPrice.ClosePrice = dPrice
            aPrice.ContractID = aFutOpt.ID
            aPrice.ContractType = 5
            aPrice.Symbol = aFutOpt.Symbol
        End If
    ElseIf Not aFut Is Nothing Then
        sKey = "4_" + Trim$(Str$(aFut.ID))
        
        Set aPrice = m_collPrices(sKey)
        If aPrice Is Nothing Then
            Set aPrice = m_collPrices.Add(sKey)
            aPrice.ClosePrice = dPrice
            aPrice.ContractID = aFut.ID
            aPrice.ContractType = 4
            aPrice.Symbol = aFut.Symbol
        End If
    Else
        sKey = Trim$(Str$(aUnd.ContractType)) + "_" + Trim$(Str$(aUnd.ID))
        Set aPrice = m_collPrices(sKey)
        If aPrice Is Nothing Then
            Set aPrice = m_collPrices.Add(sKey)
            aPrice.ClosePrice = dPrice
            aPrice.ContractID = aUnd.ID
            aPrice.ContractType = aUnd.ContractType
            aPrice.Symbol = aUnd.Symbol
        End If
    End If
End Sub

Private Sub AddOptionRootFromText(ByRef aOpt As clsRecPosOptAtom, _
                            ByRef aUnd As clsRecPosUndAtom, _
                            ByVal nContractMultiplier As Long)
    Dim aRoot As clsRecOptRootAtom, sKey$
    
    sKey = Trim$(Str$(aOpt.OptionRootID))
        
    Set aRoot = m_collRoots(sKey)
        
    If aRoot Is Nothing Then
        Set aRoot = m_collRoots.Add(sKey)
        aRoot.Symbol = aOpt.OptionRootSymbol
        aRoot.RootID = aOpt.OptionRootID
    End If
    aRoot.NewLotSize = nContractMultiplier
    
End Sub

Private Sub CheckTraderContract(ByVal nTraderId As Long, ByVal nUndId As Long)
    Dim aTrdCnt As clsTraderContractAtom, sKey$
    
    sKey = gCmn.ReadStr(nTraderId) & "_" & gCmn.ReadStr(nUndId)
    
    Set aTrdCnt = m_collTraderContracts(sKey)
    If aTrdCnt Is Nothing Then
        gDBW.usp_BoTraderContract_Save nTraderId, nUndId
        Set aTrdCnt = m_collTraderContracts.Add(sKey)
        aTrdCnt.ContractID = nUndId
        aTrdCnt.TraderID = nTraderId
    End If
End Sub

Private Sub ParseCIBCOptPortiaId(ByRef sPortiaId As String, _
                                    ByRef bIsCall As Boolean, _
                                    ByRef dtExpiryDate As Date, _
                                    ByRef dStrike As Double)
    
    Dim Items As Variant, nItemNumber&, sItem$, nBeg&, nEnd&, sYear$, nYear&, nDecade&, nExpYear&
    Dim nMonth&
    
    
    sPortiaId = UCase(sPortiaId)
    Items = Split(sPortiaId, " ")
    nBeg = LBound(Items)
    nEnd = UBound(Items)
    If nEnd - nBeg <> 2 Then
        Err.Raise vbObjectError, , "Invalid Option Portia Id"
    Else
        dStrike = gCmn.ReadDbl(Items(nEnd))
        
        If dStrike <= 0# Then _
            Err.Raise vbObjectError, , "Invalid Option Strike"
        
        sItem = Items(nBeg + 1)
        nExpYear = gCmn.ReadLng(Left(sItem, Len(sItem) - 1))
        

        sItem = Right(sItem, 1)
        
        If Asc(sItem) >= Asc("A") And Asc(sItem) < Asc("M") Then
            bIsCall = True
            nMonth = Asc(sItem) - Asc("A") + 1
            dtExpiryDate = Str$(nMonth) & "/1/" & Str$(2000 + nExpYear)
        ElseIf Asc(sItem) >= Asc("M") And Asc(sItem) < Asc("Y") Then
            bIsCall = False
            nMonth = Asc(sItem) - Asc("M") + 1
            dtExpiryDate = Str$(nMonth) & "/1/" & Str$(2000 + nExpYear)
        Else
            Err.Raise vbObjectError, , "Invalid Option Month Code"
        End If
    End If
End Sub

Private Sub ParseCIBCUndPortiaId(ByRef sPortiaId As String, _
                                    ByRef sUndSymbol As String, _
                                    ByRef nContructType As Long)
    
    Dim Items As Variant, nItemNumber&, sItem$, nBeg&, nEnd&
        
    sPortiaId = UCase(sPortiaId)
    Items = Split(sPortiaId, " ")
    nBeg = LBound(Items)
    nEnd = UBound(Items)
    If nEnd - nBeg <> 1 Then
        Err.Raise vbObjectError, , "Invalid Underlying Portia Id"
    Else
        sUndSymbol = Items(nBeg)
        If Items(nEnd) = "US" Then
            nContructType = 2
        ElseIf Items(nEnd) = "INDEX" Then
            nContructType = 1
        Else
            Err.Raise vbObjectError, , "Invalid Underlying Portia Id"
        End If
    End If
End Sub

Private Sub ParseCIBCFutOptPortiaId(ByRef sPortiaId As String, _
                                    ByRef bIsCall As Boolean, _
                                    ByRef dtExpiryDate As Date, _
                                    ByRef dStrike As Double)
    
    Dim Items As Variant, nItemNumber&, sItem$, nBeg&, nEnd&, sYear$, nYear&, nDecade&, nExpYear&
    Dim nMonth&
    
   
    sPortiaId = UCase(sPortiaId)
    Items = Split(sPortiaId, " ")
    nBeg = LBound(Items)
    nEnd = UBound(Items)
    If nEnd - nBeg <> 2 Then
        Err.Raise vbObjectError, , "Invalid Future Option Portia Id"
    Else
        dStrike = gCmn.ReadDbl(Items(nEnd))
        
        If dStrike <= 0# Then _
            Err.Raise vbObjectError, , "Invalid Future Option Strike"
        
        sItem = Items(nBeg + 1)
        
        nExpYear = gCmn.ReadLng(Left(sItem, Len(sItem) - 1))

        sItem = Right(sItem, 1)
        
        If Asc(sItem) >= Asc("A") And Asc(sItem) < Asc("M") Then
            bIsCall = True
            nMonth = Asc(sItem) - Asc("A") + 1
            dtExpiryDate = Str$(nMonth) & "/1/" & Str$(2000 + nExpYear)
        ElseIf Asc(sItem) >= Asc("M") And Asc(sItem) < Asc("Y") Then
            bIsCall = False
            nMonth = Asc(sItem) - Asc("A") + 1
            dtExpiryDate = Str$(nMonth) & "/1/" & Str$(2000 + nExpYear)
        Else
            Err.Raise vbObjectError, , "Invalid Future Option Month Code"
        End If
    End If
End Sub

Private Sub ParseCIBCFutPortiaId(ByVal sPortiaId As String, _
                                ByRef sFutureRootSymbol As String, _
                                ByRef dtMaturityDate As Date)
    Dim sYear$, nYear&, nDecade&, nExpYear&, sMonth$, nMonth&
               
    sFutureRootSymbol = Left(sPortiaId, Len(sPortiaId) - 2)
    
    nExpYear = gCmn.ReadLng(Right(sPortiaId, 1))
    nMonth = 0
    
    sMonth = Mid$(sPortiaId, Len(sPortiaId) - 1, 1)
    Select Case sMonth
        Case "F":
            nMonth = nMonth + 1
        Case "G":
            nMonth = nMonth + 2
        Case "H":
            nMonth = nMonth + 3
        Case "J":
            nMonth = nMonth + 4
        Case "K":
            nMonth = nMonth + 5
        Case "M":
            nMonth = nMonth + 6
        Case "N":
            nMonth = nMonth + 7
        Case "Q":
            nMonth = nMonth + 8
        Case "U":
            nMonth = nMonth + 9
        Case "V":
            nMonth = nMonth + 10
        Case "X":
            nMonth = nMonth + 11
        Case "Z":
            nMonth = nMonth + 12
    End Select
    dtMaturityDate = Str$(nMonth) & "/1/" & Str$(2000 + nExpYear)
    
End Sub

Private Function SaveStrategy&(ByVal sStrategyName$)
    On Error GoTo EH
    
    SaveStrategy = gDBW.usp_BoStrategy_Save(Null, sStrategyName, Null)
    Exit Function
EH:
    m_frmEOD.LogMsg "Error save new stragegy '" & sStrategyName & "': " & Err.Description
End Function

Private Function SaveClosePrice(ByVal ContractType As Long, ByVal ContractID As Long, ByVal ContractSymbol As String, ByVal ClosePrice As Double)
    On Error GoTo Herr
    
    SaveClosePrice = False
    
    gDBW.usp_ContractPrice_Save _
        Null, _
        ContractID, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        ClosePrice, _
        Null, _
        Null, _
        Null, _
        Null, _
        Null, _
        ContractType, _
        Null
        
        SaveClosePrice = True
        
        Exit Function
Herr:
    m_frmEOD.LogMsg "Error save close prices for '" & ContractSymbol & "': " & Err.Description
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Scan for positions change
'
Public Function ShowPositionsReport() As Boolean
    Dim Symbol$, sParent$, sLogMsg$
    Dim nCount&, i&, aPos As clsRecPosAtom
    
    ShowPositionsReport = False
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Max = 100
    m_frmEOD.pbProgress.Value = 0
    On Error GoTo Herr
    
    If frmPosReconcile.Execute(m_collPos, m_btFileFormat, m_frmEOD, m_collStrategyByName, m_bStrategySupport) Then
        ShowPositionsReport = True
    End If
    
    On Error Resume Next
    m_frmEOD.PrintLogFile "Reconciled positions:"
    nCount = m_collPos.Count

    For i = 1 To nCount
        Set aPos = m_collPos(i)
        If aPos.PosValidNew <> aPos.PosValidOld Or aPos.QtyNew <> aPos.QtyOld _
            Or ((m_btFileFormat = 3) And (aPos.PriceNew <> aPos.PriceOld Or aPos.StrategyOld <> aPos.StrategyNew)) _
            Or (m_btFileFormat = 1 And m_bStrategySupport And aPos.StrategyOld <> aPos.StrategyNew) Then
            
            If m_btFileFormat <> 3 Then sLogMsg = Str$(aPos.bFix) & vbTab
            sLogMsg = sLogMsg & aPos.Trader.Acronym & vbTab
            If m_btFileFormat <> 3 Then sLogMsg = sLogMsg & aPos.UndSymbol & vbTab
            If m_btFileFormat = 3 Then
                sLogMsg = sLogMsg & IIf(Len(aPos.StrategyOld) > 0, aPos.StrategyOld, "--") & vbTab
                sLogMsg = sLogMsg & IIf(Len(aPos.StrategyNew) > 0, aPos.StrategyNew, "--") & vbTab
                sLogMsg = sLogMsg & IIf(Len(aPos.ImportSymbol) > 0, aPos.ImportSymbol, "--") & vbTab
            End If
            
            If Not aPos.Opt Is Nothing Then
                sLogMsg = sLogMsg & aPos.Opt.SymbolDisplay & vbTab
            ElseIf Not aPos.FutOpt Is Nothing Then
                sLogMsg = sLogMsg & aPos.FutOpt.SymbolDisplay & vbTab
            ElseIf Not aPos.Fut Is Nothing Then
                sLogMsg = sLogMsg & aPos.Fut.SymbolDisplay & vbTab
            Else
                sLogMsg = sLogMsg & aPos.Und.Symbol & vbTab
            End If
            
            sLogMsg = sLogMsg & IIf(aPos.PosValidOld, aPos.QtyOld, "--") & vbTab
            sLogMsg = sLogMsg & IIf(aPos.PosValidNew, aPos.QtyNew, "--")
            
            If m_btFileFormat = 3 Then
                sLogMsg = Str$(aPos.bFix) & vbTab
                sLogMsg = sLogMsg & IIf(aPos.PosValidOld And aPos.PriceOld >= DBL_EPSILON, aPos.PriceOld, "--") & vbTab
                sLogMsg = sLogMsg & IIf(aPos.PosValidNew And aPos.PriceNew >= DBL_EPSILON, aPos.PriceNew, "--") & vbTab
                sLogMsg = Str$(aPos.bFixPrice)
            End If
            m_frmEOD.PrintLogFile sLogMsg
        End If
        Set aPos = Nothing
    Next
    
Done:
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error Checking position: " & Err.Description
    Resume Done
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Insert Positions and Trades
'
Public Function SavePositionsToDatabase(ByVal bIsSaveHistory As Boolean) As Long
    Dim i&, nCount&, nSeqNum&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    Dim lSpResult As Long
        
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Value = 0
    m_frmEOD.pbProgress.Max = 100
    iStoredTradesCount = 0
    
    nCount = m_collPos.Count
    
    If nCount > 0 Then
        m_frmEOD.pbProgress.Max = nCount
        nNewCount = 0
        lSpResult = gDBW.usp_BoTrade_StoreToHistoryAndClear(bIsSaveHistory, m_dtUTCTradeDate, iStoredTradesCount)
        
        If lSpResult = 0 Then
            If bIsSaveHistory Then m_frmEOD.LogMsg (Str$(iStoredTradesCount) & " trades saved to history")
        Else
            m_frmEOD.LogMsg "SP usp_BoTrade_StoreToHistoryAndClear failed: " & Str$(lSpResult) & " Error code"
        End If
        
        For i = 1 To nCount
            On Error GoTo Failed
            Set aPos = m_collPos(i)
                        
            If aPos.bFix And aPos.PosValidNew Or Not aPos.bFix And aPos.PosValidOld _
                Or m_btFileFormat = 3 And aPos.bFixPrice Then

                vRootId = Null: vFutRootId = Null: vFutId = Null
                If Not aPos.Opt Is Nothing Then
                    nId = aPos.Opt.ID
                    sSymbol = aPos.Opt.SymbolDisplay
                    nTypeId = 3
                    vRootId = aPos.Opt.OptionRootID
                ElseIf Not aPos.FutOpt Is Nothing Then
                    nId = aPos.FutOpt.ID
                    sSymbol = aPos.FutOpt.SymbolDisplay
                    nTypeId = 5
                    vFutRootId = aPos.Fut.FutureRootID
                    vFutId = aPos.Fut.ID
                ElseIf Not aPos.Fut Is Nothing Then
                    nId = aPos.Fut.ID
                    sSymbol = aPos.Fut.SymbolDisplay
                    nTypeId = 4
                    vFutRootId = aPos.Fut.FutureRootID
                    vFutId = aPos.Fut.ID
                Else
                    nId = aPos.Und.ID
                    sSymbol = aPos.Und.Symbol
                    nTypeId = aPos.Und.ContractType
                End If
            End If
            
            If aPos.bFix And aPos.PosValidNew Or Not aPos.bFix And aPos.PosValidOld Then
                
                If (m_btFileFormat = 3 Or m_btFileFormat = 5 Or (m_btFileFormat = 1 And m_bStrategySupport)) And aPos.StrategyForTradeID = 0 And Len(aPos.StrategyForTrade) > 0 Then
                    aPos.StrategyForTradeID = SaveStrategy(aPos.StrategyForTrade)
                ElseIf (Not m_bStrategySupport) Then
                    aPos.StrategyForTradeID = 0
                End If
                
                If (Not aPos.bFix And aPos.PosValidOld And m_btFileFormat = 1 And m_bStrategySupport) Then
                    If (aPos.StrategyForTradeID = 0 And aPos.StrategyOldID <> 0) Then
                        aPos.StrategyForTradeID = aPos.StrategyOldID
                    End If
                ElseIf (aPos.bFix And aPos.PosValidNew And m_btFileFormat = 1 And m_bStrategySupport) Then
                    If (aPos.StrategyForTradeID = 0 And aPos.StrategyNewID <> 0) Then
                        aPos.StrategyForTradeID = aPos.StrategyNewID
                    End If
                End If
                
                Dim iUndID As Long
                If (Not aPos.Und Is Nothing) Then
                    iUndID = aPos.Und.ID
                Else
                    iUndID = aPos.Fut.IndexId
                End If
                
                nSeqNum = gDBW.usp_Trade_New(nId, _
                        Abs(IIf(aPos.bFix, aPos.QtyNew, aPos.QtyOld)), _
                        IIf(IIf(m_btFileFormat = 3, aPos.bFixPrice, aPos.bFix), aPos.PriceNew, aPos.PriceOld), _
                        CByte(IIf(IIf(aPos.bFix, aPos.QtyNew, aPos.QtyOld) < 0, 0, 1)), _
                        aPos.Trader.ID, IIf(aPos.StrategyForTradeID <> 0, aPos.StrategyForTradeID, Null), _
                        Null, Null, Null, Null, Null, Null, _
                        1, Null, aPos.TradeDate, 0, iUndID, vRootId, vFutRootId, vFutId)
                                        
                If nSeqNum <= 0 Then
                    Select Case nSeqNum
                        Case -101 'invalid parameters
                            sErrDesc = "Invalid contract and underlying data"
                        Case -102 'invalid contract value
                            sErrDesc = "Unknown contract data"
                        Case -103 'invalid contract data for underlying
                            sErrDesc = "Invalid data for underlying"
                        Case -104 'invalid contract data for option
                            sErrDesc = "Invalid data for option"
                        Case -105 'invalid option data for option
                            sErrDesc = "Unknown option contract"
                        Case -106 'invalid option root data for option
                            sErrDesc = "Unknown option root"
                        Case -107 'invalid underlying data for option
                            sErrDesc = "Option root is assigned to another underlying"
                        Case -108 'invalid contract data for futures
                            sErrDesc = "Invalid data for futures contract"
                        Case -109 'invalid futures data for futures
                            sErrDesc = "Unknown futures contract"
                        Case -110 'invalid futures root data for futures
                            sErrDesc = "Futures is assigned to another futures root"
                        Case -111 'invalid underlying data for futures
                            sErrDesc = "Futures is assigned to another underlying"
                        Case -112 'invalid contract data for futures option
                            sErrDesc = "Invalid data for futures option contract"
                        Case -113 'invalid futures option data for futures option
                            sErrDesc = "Unknown futures option contract"
                        Case -114 'invalid futures data for futures option
                            sErrDesc = "Futures option is assigned to another futures"
                        Case -115 'invalid futures root data for futures option
                            sErrDesc = "Futures option is assigned to another futures root"
                        Case -116 'invalid underlying data for futures option
                            sErrDesc = "Futures option is assigned to another underlying"
                    End Select
                
                    m_frmEOD.LogMsg "Fail to update position for " & sSymbol & ". " & _
                            sErrDesc & ". Please correct source positions file"
                End If
            End If
            
            If m_btFileFormat = 3 And aPos.bFixPrice And aPos.PriceNew > 0 Then
                SaveClosePrice nTypeId, nId, sSymbol, aPos.PriceNew
            End If
            
            nNewCount = nNewCount + 1
Skip:
            On Error Resume Next
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            On Error GoTo Herr
            m_frmEOD.Idle
        Next
    End If
    
    SavePositionsToDatabase = nNewCount
            
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error saving position: " & Err.Description
    Exit Function

Failed:
    m_frmEOD.LogMsg "Error saving position: " & Err.Description
    Resume Skip
End Function

Public Property Get TradeDate() As Date
    TradeDate = m_dtUTCTradeDate
End Property

Public Property Let TradeDate(NewValue As Date)
    m_dtUTCTradeDate = NewValue
End Property

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_frmEOD = Nothing
    Set m_collPos = Nothing
    Set m_collTrader = Nothing
    Set m_collUnd = Nothing
    Set m_collRoots = Nothing
    Set m_collPrices = Nothing
    Set m_collTraderContracts = Nothing
    Set m_collFut = Nothing
    Set m_collStrategy = Nothing
    Set m_collStrategyByName = Nothing
    Set m_CollTrades = Nothing
    Set m_collExch = Nothing
End Sub

Public Function LoadTradesFromText(ByVal Buffer As String) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim bStructureExist As Boolean, nNewTradesCount&

    nNewTradesCount = 0
    bStructureExist = True
    On Error GoTo EH
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
    
    m_frmEOD.pbProgress.Max = UBound(Lines) + 1
    For nLineNumber = nBegin To nEnd
        On Error GoTo EH
        
        sLine = Lines(nLineNumber)
        
        If ParseLineOfTradesFile(nLineNumber, sLine, bStructureExist) Then nNewTradesCount = nNewTradesCount + 1
        
        On Error Resume Next
        m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
        On Error GoTo 0
        m_frmEOD.Idle
    Next
    
    If bStructureExist Then
        LoadTradesFromText = True
    Else
        If gCmn.MyMsgBox("Contracts without market structure was found." & _
                        " You must to refresh market structure before import trades." & _
                        " Continue anyway?", vbExclamation + vbYesNo) = vbNo Then
                        
            m_frmEOD.LogMsg "Trades import canceled."
            LoadTradesFromText = False
        Else
            LoadTradesFromText = True
        End If
    End If
    
Done:
    On Error GoTo 0
    If nNewTradesCount = 0 Then
        Err.Raise vbObjectError, , "No trades found"
        Exit Function
    End If
    Erase Lines
    Exit Function
EH:
    m_frmEOD.LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function

Public Function LoadCurvesFromText(ByVal Buffer As String, ByRef sXML$) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim nNewTradesCount&
    'Dim sXML As String

    nNewTradesCount = 0
    On Error GoTo EH
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
    
    m_frmEOD.pbProgress.Max = UBound(Lines) + 1
    
    sXML = "<IRCurve>"
    
    For nLineNumber = nBegin To nEnd
        On Error GoTo EH
        
        sLine = Lines(nLineNumber)
        
        Dim sResult As String
        
        If ParseLineOfCurvesFile(nLineNumber, sLine, sResult) Then nNewTradesCount = nNewTradesCount + 1
        sXML = sXML + sResult
        On Error Resume Next
        m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
        On Error GoTo 0
        m_frmEOD.Idle
    Next
    
    sXML = sXML + "</IRCurve>"
    
    LoadCurvesFromText = True
    
Done:
    On Error GoTo 0
    If nNewTradesCount = 0 Then
        Err.Raise vbObjectError, , "No curves found"
        Exit Function
    End If
    Erase Lines
    Exit Function
EH:
    m_frmEOD.LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function


Private Function ParseLineOfTradesFile(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom, aTrader As clsRecPosTraderAtom
    Dim sTrader$, sUnderlyingSymbol$, sKey$
    Dim Items$(), dtTradeDate As Date, sRoot$, dStrike#, sOptionType$, nQuantity&, dPrice#, sContractSymbol$
    Dim dtExpirationDate As Date, sCol$, sValue$, sStrategy$, sContractType$
    Dim aFutOpt As clsRecPosFutOptAtom, aFut As clsRecPosFutAtom, dtMaturityDate As Date
    
    ParseLineOfTradesFile = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 11 Then
        Erase Items
        Err.Raise vbObjectError, "Trades Import", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    dtTradeDate = 0#: sTrader = "": sUnderlyingSymbol = "": sRoot = "": sStrategy = "": sContractSymbol = ""
    dStrike = 0#: sOptionType = "": nQuantity = 0&: dPrice = 0#
    dtExpirationDate = 0#: sContractType = ""
    
    sCol = "Trader":               sValue = Items(0):  If Len(sValue) > 0 Then sTrader = sValue
    sCol = "Strategy":             sValue = Items(1):  If Len(sValue) > 0 Then sStrategy = sValue
    sCol = "Trade Date/Time":      sValue = Items(2):  If Len(sValue) > 4 Then dtTradeDate = CDate(sValue)
    sCol = "Underlying Symbol":    sValue = Items(3):  If Len(sValue) > 0 Then sUnderlyingSymbol = sValue
    sCol = "Contract Type":        sValue = Items(4):  If Len(sValue) > 0 Then sContractType = sValue
    sCol = "Option/Future Root":   sValue = Items(5):  If Len(sValue) > 0 Then sRoot = sValue
    sCol = "Future Maturity Date": sValue = Items(6):  If Len(sValue) > 4 Then dtMaturityDate = CDate(sValue)
    sCol = "Expiration Date":      sValue = Items(7):  If Len(sValue) > 4 Then dtExpirationDate = CDate(sValue)
    sCol = "Strike":               sValue = Items(8):  If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Option Type":          sValue = Items(9):  If Len(sValue) > 0 Then sOptionType = sValue
    sCol = "Quantity":             sValue = Items(10): If Len(sValue) > 0 Then nQuantity = gCmn.ReadLng(sValue)
    sCol = "Price":                sValue = Items(11): If Len(sValue) > 0 Then dPrice = gCmn.ReadDbl(sValue)
    
    sCol = ""
    sValue = ""
    
    If sTrader = "" Then
        sCol = "Trader"
        Err.Raise vbObjectError, "Trades Import", "Empty Account Acronym"
    ElseIf sUnderlyingSymbol = "" Then
        sCol = "Underlying Symbol"
        Err.Raise vbObjectError, "Trades Import", "Empty Stock Symbol"
    ElseIf dtTradeDate = 0 Then
        sCol = "Trade Date"
        Err.Raise vbObjectError, "Trades Import", "Empty Trade Date"
    ElseIf sContractType = "OPT" Or sContractType = "FOP" Then
        If sRoot = "" Then
            sCol = "Option/Future Root"
            Err.Raise vbObjectError, "Trades Import", "Empty Option/Future Root"
        ElseIf sContractType = "FOP" And dtMaturityDate = 0 Then
            sCol = "Future Maturity Date"
            Err.Raise vbObjectError, "Trades Import", "Invalid Future Maturity Date"
        ElseIf dtExpirationDate = 0 Then
            sCol = "Expiration Date"
            Err.Raise vbObjectError, "Trades Import", "Invalid Expiration Date"
        ElseIf dStrike = 0 Then
            sCol = "Strike"
            Err.Raise vbObjectError, "Trades Import", "Invalid Strike"
        ElseIf sOptionType <> "C" And sOptionType <> "P" Then
            sCol = "Option Type"
            Err.Raise vbObjectError, "Trades Import", "Empty or Invalid Option Type"
        End If
    ElseIf sContractType = "FUT" Then
        If sRoot = "" Then
            sCol = "Option/Future Root"
            Err.Raise vbObjectError, "Trades Import", "Empty Option/Future Root"
        ElseIf dtMaturityDate = 0 Then
            sCol = "Future Maturity Date"
            Err.Raise vbObjectError, "Trades Import", "Invalid Future Maturity Date"
        End If
    End If

    On Error GoTo 0
    
    Set aTrader = FindTraderByName(sTrader)
    
    If sContractType = "FOP" Then

        ' try to find mapped root symbol first
        sKey = "1_" & sRoot & "_" & Trim$(Format$(dtMaturityDate))
        Set aFut = m_collFut(sKey)
        
        If aFut Is Nothing Then ' if not found then try to find normal symbol
            sKey = "0_" & sRoot & "_" & Trim$(Format$(dtMaturityDate))
            Set aFut = m_collFut(sKey)
        End If
        
        If aFut Is Nothing Then
            m_frmEOD.LogMsg "Trade skipped. Future not in database: " & sUnderlyingSymbol & ", its future options will be skipped also. Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        sUnderlyingSymbol = aFut.IndexSymbol
        
        Set aFutOpt = aFut.FindFutureOption(IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aFutOpt Is Nothing Then
            m_frmEOD.LogMsg "Trade skipped. Future option not in database or not unique: " & sRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", future: " & aFut.Symbol & ", index: " & sUnderlyingSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
    End If
    
    Set aUnd = m_collUnd(sUnderlyingSymbol)
    If aUnd Is Nothing Then
        m_frmEOD.LogMsg "Trade skipped. Underlying not in database: " & sUnderlyingSymbol & "."
        'Set aUnd = AddNewUnderlyingToDatabase(sUnderlyingSymbol)
        bStructureExist = False
        Exit Function
    End If
    
    CheckTraderContract aTrader.ID, aUnd.ID
    
    If sContractType = "OPT" Then
        Set aOpt = aUnd.FindOption(sRoot, IIf(sOptionType = "C", 1, 0), dStrike, dtExpirationDate)
        If aOpt Is Nothing Then
            m_frmEOD.LogMsg "Trade skipped. Option not in database: " & sRoot & UCase(Format$(dtExpirationDate, "YYMMM")) & Format$(Int(dStrike)) & sOptionType & ", stock " & sUnderlyingSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        End If
        
    ElseIf sContractType = "FUT" Then
        ' try to find mapped root symbol first
        sKey = "1_" & sRoot & "_" & Trim$(Format$(dtMaturityDate))
        Set aFut = m_collFut(sKey)
        
        If aFut Is Nothing Then ' if not found then try to find normal symbol
            sKey = "0_" & sRoot & "_" & Trim$(Format$(dtMaturityDate))
            Set aFut = m_collFut(sKey)
        End If
        
        Set aFut = m_collFut(sKey)
        If aFut Is Nothing Then
            m_frmEOD.LogMsg "Trade skipped. Future not in database: root: " & sRoot & ", index: " & sUnderlyingSymbol & ". Please update market structure."
            bStructureExist = False
            Exit Function
        Else
            If aFut.IndexSymbol <> sUnderlyingSymbol Then
                m_frmEOD.LogMsg "Trade skipped. Future with root " & sRoot & " of index " & sUnderlyingSymbol & " is assigned to another index " & aFut.IndexSymbol & " in " & AppIvRmTitle() & " . Please correct a file or market structure. (line " & nLineNumber & ")"
                bStructureExist = False
                Exit Function
            End If
        End If
    End If
    
    AddTradeFromText aOpt, aUnd, aFutOpt, aFut, aTrader, nQuantity, dPrice, sStrategy, dtTradeDate
    
    Erase Items
    ParseLineOfTradesFile = True
    Exit Function
ParseError:
    Erase Items
    m_frmEOD.LogMsg "Trades skipped. Error parsing trade data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Function ParseLineOfCurvesFile(ByVal nLineNumber As Long, ByRef sLine As String, ByRef sResult As String) As Boolean
    Dim Items$(), iDays As Integer, dBaseRate As Double, iLongSpread As Integer, iShortSpread As Integer
    Dim sCol$, sValue$
    
    ParseLineOfCurvesFile = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Curves Import", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    iDays = 0: dBaseRate = -1: iLongSpread = -1: iShortSpread = -1
    'dtTradeDate = 0#: sTrader = "": sUnderlyingSymbol = "": sRoot = "": sStrategy = "": sContractSymbol = ""
    'dStrike = 0#: sOptionType = "": nQuantity = 0&: dPrice = 0#
    'dtExpirationDate = 0#: sContractType = ""
  
    sCol = "Period":               sValue = Items(0):  If Len(sValue) > 0 Then iDays = gCmn.ReadLng(sValue)
    sCol = "Base Rate":            sValue = Items(1):  If Len(sValue) > 0 Then dBaseRate = gCmn.ReadDbl(sValue)
    sCol = "Long Spread":          sValue = Items(2):  If Len(sValue) > 0 Then iLongSpread = gCmn.ReadLng(sValue)
    sCol = "Short Spread":         sValue = Items(3):  If Len(sValue) > 0 Then iShortSpread = gCmn.ReadLng(sValue)
    
    sCol = ""
    sValue = ""
    
    If iDays = 0 Then
        sCol = "Period"
        Err.Raise vbObjectError, "Curves Import", "Empty Period"
    ElseIf dBaseRate = -1 Then
        sCol = "Base Rate"
        Err.Raise vbObjectError, "Curves Import", "Empty Base Rate"
    ElseIf iLongSpread = -1 Then
        sCol = "Long Spread"
        Err.Raise vbObjectError, "Curves Import", "Empty Long Spread"
    ElseIf iShortSpread = -1 Then
        sCol = "Short Spread"
        Err.Raise vbObjectError, "Curves Import", "Empty Short Spread"
    End If


    On Error GoTo 0
    
    dBaseRate = dBaseRate * 0.01
    
    sResult = "<IRPoint PeriodTypeID=""1"" Num = """ + CStr(iDays) + """ NeutralRate = """ + CStr(dBaseRate) + """ ShortRate = """ + CStr(dBaseRate - iShortSpread * 0.0001) + _
                            """ LongRate=""" + CStr(dBaseRate + iLongSpread * 0.0001) + """ />"

    Erase Items
    ParseLineOfCurvesFile = True
    Exit Function
ParseError:
    Erase Items
    m_frmEOD.LogMsg "Curves skipped. Error parsing curve data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function


Public Function SaveTradesToDatabase&()
    Dim i&, nCount&, nSeqNum&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Value = 0
    m_frmEOD.pbProgress.Max = 100
    iStoredTradesCount = 0
    
    nCount = m_collPos.Count
    
    If nCount > 0 Then
        m_frmEOD.pbProgress.Max = nCount
        nNewCount = 0
        
        For i = 1 To nCount
            On Error GoTo Failed
            Set aPos = m_collPos(i)
                        
            vRootId = Null: vFutRootId = Null: vFutId = Null
            If Not aPos.Opt Is Nothing Then
                nId = aPos.Opt.ID
                sSymbol = aPos.Opt.SymbolDisplay
                nTypeId = 3
                vRootId = aPos.Opt.OptionRootID
            ElseIf Not aPos.FutOpt Is Nothing Then
                nId = aPos.FutOpt.ID
                sSymbol = aPos.FutOpt.SymbolDisplay
                nTypeId = 5
                vFutRootId = aPos.Fut.FutureRootID
                vFutId = aPos.Fut.ID
            ElseIf Not aPos.Fut Is Nothing Then
                nId = aPos.Fut.ID
                sSymbol = aPos.Fut.SymbolDisplay
                nTypeId = 4
                vFutRootId = aPos.Fut.FutureRootID
                vFutId = aPos.Fut.ID
            Else
                nId = aPos.Und.ID
                sSymbol = aPos.Und.Symbol
                nTypeId = aPos.Und.ContractType
            End If
            
            If aPos.StrategyForTradeID = 0 And Len(aPos.StrategyForTrade) > 0 Then
                aPos.StrategyForTradeID = SaveStrategy(aPos.StrategyForTrade)
            End If
            
            nSeqNum = gDBW.usp_Trade_New(nId, _
                    Abs(aPos.QtyNew), _
                    aPos.PriceNew, _
                    CByte(IIf(aPos.QtyNew < 0, 0, 1)), _
                    aPos.Trader.ID, IIf(aPos.StrategyForTradeID <> 0, aPos.StrategyForTradeID, Null), _
                    Null, Null, Null, Null, Null, Null, _
                    0, Null, aPos.TradeDate, 0, aPos.Und.ID, vRootId, vFutRootId, vFutId)
                                    
            If nSeqNum <= 0 Then
                Select Case nSeqNum
                    Case -101 'invalid parameters
                        sErrDesc = "Invalid contract and underlying data"
                    Case -102 'invalid contract value
                        sErrDesc = "Unknown contract data"
                    Case -103 'invalid contract data for underlying
                        sErrDesc = "Invalid data for underlying"
                    Case -104 'invalid contract data for option
                        sErrDesc = "Invalid data for option"
                    Case -105 'invalid option data for option
                        sErrDesc = "Unknown option contract"
                    Case -106 'invalid option root data for option
                        sErrDesc = "Unknown option root"
                    Case -107 'invalid underlying data for option
                        sErrDesc = "Option root is assigned to another underlying"
                    Case -108 'invalid contract data for futures
                        sErrDesc = "Invalid data for futures contract"
                    Case -109 'invalid futures data for futures
                        sErrDesc = "Unknown futures contract"
                    Case -110 'invalid futures root data for futures
                        sErrDesc = "Futures is assigned to another futures root"
                    Case -111 'invalid underlying data for futures
                        sErrDesc = "Futures is assigned to another underlying"
                    Case -112 'invalid contract data for futures option
                        sErrDesc = "Invalid data for futures option contract"
                    Case -113 'invalid futures option data for futures option
                        sErrDesc = "Unknown futures option contract"
                    Case -114 'invalid futures data for futures option
                        sErrDesc = "Futures option is assigned to another futures"
                    Case -115 'invalid futures root data for futures option
                        sErrDesc = "Futures option is assigned to another futures root"
                    Case -116 'invalid underlying data for futures option
                        sErrDesc = "Futures option is assigned to another underlying"
                End Select
            
                m_frmEOD.LogMsg "Fail to import trade for " & sSymbol & ". " & _
                        sErrDesc & ". Please correct source trades file"
            End If
            
            nNewCount = nNewCount + 1
Skip:
            On Error Resume Next
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            On Error GoTo Herr
            m_frmEOD.Idle
        Next
    End If
    
    SaveTradesToDatabase = nNewCount
            
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error saving trades: " & Err.Description
    Exit Function

Failed:
    m_frmEOD.LogMsg "Error saving trades: " & Err.Description
    Resume Skip
End Function

Public Function SaveCurvesToDatabase(ByVal sXML$)
    Dim i&, nCount&, nSeqNum&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    On Error GoTo Failed
    nSeqNum = gDBW.usp_IRPoints_Import(sXML)
                            
    If nSeqNum <> 0 Then
    
        m_frmEOD.LogMsg "Fail to import curve. Please correct source curves file."
    End If
    Exit Function

Failed:
    m_frmEOD.LogMsg "Error saving curves: " & Err.Description
End Function

Private Sub AddTradeFromText(ByRef aOpt As clsRecPosOptAtom, _
                                ByRef aUnd As clsRecPosUndAtom, _
                                ByRef aFutOpt As clsRecPosFutOptAtom, _
                                ByRef aFut As clsRecPosFutAtom, _
                                ByRef aTrader As clsRecPosTraderAtom, _
                                ByVal nPosition As Long, _
                                ByVal dPrice As Double, _
                                ByVal sStrategy As String, _
                                ByVal dtTradeDate As Date)
    Dim aPos As clsRecPosAtom, sKey$, aStrategy As clsStrategyAtom
    
    sKey = CStr(m_collPos.Count)
    
    Set aPos = m_collPos(sKey)
    If aPos Is Nothing Then
        Set aPos = m_collPos.Add(sKey)
        aPos.UndSymbol = aUnd.Symbol
        Set aPos.Und = aUnd
        Set aPos.Opt = aOpt
        Set aPos.Fut = aFut
        Set aPos.FutOpt = aFutOpt
        Set aPos.Trader = aTrader
        
        aPos.QtyOld = 0
        aPos.PriceOld = 0
        aPos.LotSizeOld = 0
        aPos.PosValidOld = False
    Else
        Debug.Assert False
    End If
    
    'If Len(aPos.StrategyOld) = 0 Then LoadPositionStrategy aUnd.Id, aPos
    
    aPos.PosValidNew = True
    aPos.QtyNew = nPosition
    aPos.PriceNew = dPrice
    aPos.TradeDate = LocalToGmt(dtTradeDate)
    aPos.StrategyNew = Left$(Trim$(sStrategy), 12)
    
    Set aStrategy = m_collStrategyByName(UCase$(sStrategy))
    If Not aStrategy Is Nothing Then
        aPos.StrategyNewID = aStrategy.StrategyID
        aPos.StrategyNew = aStrategy.StrategyName
        Set aStrategy = Nothing
    End If
    
    aPos.StrategyForTradeID = aPos.StrategyNewID
    aPos.StrategyForTrade = aPos.StrategyNew
    
    aPos.bFix = True
End Sub

Public Function LoadVolatilityFromText(ByVal Buffer As String) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    Dim bStructureExist As Boolean, nNewTradesCount&

    nNewTradesCount = 0
    bStructureExist = True
    On Error GoTo EH
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
    
    m_frmEOD.pbProgress.Max = UBound(Lines) + 1
    For nLineNumber = nBegin To nEnd
        On Error GoTo EH
        
        sLine = Lines(nLineNumber)
        
        If ParseLineOfVolatilityFile(nLineNumber, sLine, bStructureExist) Then nNewTradesCount = nNewTradesCount + 1
        
        On Error Resume Next
        m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
        On Error GoTo 0
        m_frmEOD.Idle
    Next
    
    If bStructureExist Then
        LoadVolatilityFromText = True
    Else
        If gCmn.MyMsgBox("Unknown underlyings was found." & _
                        " You must to add these underlyings before import volatilities." & _
                        " Continue anyway?", vbExclamation + vbYesNo) = vbNo Then
                        
            m_frmEOD.LogMsg "Volatility import canceled."
            LoadVolatilityFromText = False
        Else
            LoadVolatilityFromText = True
        End If
    End If
    
Done:
    On Error GoTo 0
    If nNewTradesCount = 0 Then
        Err.Raise vbObjectError, , "No volatilities found"
        Exit Function
    End If
    Erase Lines
    Exit Function
EH:
    m_frmEOD.LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
End Function

Private Function ParseLineOfVolatilityFile(ByVal nLineNumber As Long, ByRef sLine As String, ByRef bStructureExist As Boolean) As Boolean
    Dim aOpt As clsRecPosOptAtom, aUnd As clsRecPosUndAtom
    Dim sUnderlyingSymbol$, sKey$, sCol$, sValue$
    Dim Items$()
    Dim dtExpirationDate As Date, dStrike#, dVola#
    Dim sRootSymbol As String
    
    ParseLineOfVolatilityFile = False
    If Len(sLine) = 0 Then
        Exit Function
    End If
    
    Items = Split(sLine, ",")
    If UBound(Items) < 3 Then
        Erase Items
        Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file in line " & nLineNumber
    End If
    
    On Error GoTo ParseError
    sUnderlyingSymbol = ""
    dStrike = 0#: dtExpirationDate = 0#: dVola = 0#: sRootSymbol = ""
    
    sCol = "Underlying Symbol":    sValue = Items(0):  If Len(sValue) > 0 Then sUnderlyingSymbol = sValue
    sCol = "Expiration Date":      sValue = Items(1):  If Len(sValue) > 4 Then dtExpirationDate = CDate(sValue)
    sCol = "Strike":               sValue = Items(2):  If Len(sValue) > 0 Then dStrike = gCmn.ReadDbl(sValue)
    sCol = "Volatility":           sValue = Items(3):  If Len(sValue) > 0 Then dVola = gCmn.ReadDbl(sValue)
    If (UBound(Items) = 4) Then
        sCol = "Root": sValue = Items(4): If Len(sValue) > 0 Then sRootSymbol = sValue
    End If
    
    sCol = ""
    sValue = ""
    
    If sUnderlyingSymbol = "" Then
        sCol = "Underlying Symbol"
        Err.Raise vbObjectError, "Volatility Import", "Empty Underlying Symbol"
    ElseIf dtExpirationDate = 0 Then
        sCol = "Expiration Date"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Expiration Date"
    ElseIf dStrike = 0 Then
        sCol = "Strike"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Strike"
    ElseIf dVola <= 0 Then
        sCol = "Volatility"
        Err.Raise vbObjectError, "Volatility Import", "Invalid Volatility"
    End If

    On Error GoTo 0
    
    Set aUnd = m_collUnd(sUnderlyingSymbol)
    If aUnd Is Nothing Then
        m_frmEOD.LogMsg "Volatility skipped. Underlying not in database: " & sUnderlyingSymbol & "."
        bStructureExist = False
        Exit Function
    End If
    
    AddVolatilityFromText aUnd, dtExpirationDate, dStrike, dVola, sRootSymbol
    
    Erase Items
    ParseLineOfVolatilityFile = True
    Exit Function
ParseError:
    Erase Items
    m_frmEOD.LogMsg "Volatility skipped. Error parsing Volatility data in line " & nLineNumber & " '" & sCol & "'='" & sValue & "'. " & Err.Description
End Function

Private Sub AddVolatilityFromText(ByRef aUnd As clsRecPosUndAtom, _
                                ByVal dtExpirationDate As Date, _
                                ByVal dStrike As Double, _
                                ByVal dVola As Double, _
                                ByVal sRootSymbol As String)
    Dim aPos As clsRecPosAtom, sKey$
    
    sKey = CStr(m_collPos.Count)
    
    Set aPos = m_collPos(sKey)
    If aPos Is Nothing Then
        Set aPos = m_collPos.Add(sKey)
        Set aPos.Und = aUnd
    Else
        Debug.Assert False
    End If
    
    If m_collUndForVolaClear(aUnd.Symbol) Is Nothing Then _
        m_collUndForVolaClear.Add(aUnd.Symbol).ID = aUnd.ID
    
    aPos.PosValidNew = True
    aPos.QtyNew = CLng(dtExpirationDate)
    aPos.PriceOld = dStrike
    aPos.PriceNew = dVola
    aPos.RootSymbol = sRootSymbol
    
    aPos.bFix = True
End Sub

Public Function ClearOldVolatilityFromDatabase&()
    Dim i&, nCount&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Value = 0
    m_frmEOD.pbProgress.Max = 100
    iStoredTradesCount = 0
    
    nCount = m_collUndForVolaClear.Count
    
    If nCount > 0 Then
        m_frmEOD.pbProgress.Max = nCount
        nNewCount = 0
        
        For i = 1 To m_collUndForVolaClear.Count
            On Error GoTo Failed
                        
            gDBW.usp_BoCustomStrikeSkewPoint_ByUnderlying_Del m_collUndForVolaClear(i).ID
            
            nNewCount = nNewCount + 1
Skip:
            On Error Resume Next
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            On Error GoTo Herr
            m_frmEOD.Idle
        Next
    End If
    
    ClearOldVolatilityFromDatabase = nNewCount
            
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error cleaning old volatilities: " & Err.Description
    Exit Function

Failed:
    m_frmEOD.LogMsg "Error cleaning old volatilities: " & Err.Description
    Resume Skip
End Function

Public Function SaveVolatilityToDatabase&()
    Dim i&, nCount&, sParent$, nNewCount&, aPos As clsRecPosAtom, iStoredTradesCount&
    Dim nId&, sSymbol$, nTypeId&, vRootId, vFutRootId, vFutId As Variant, sErrDesc$
    
    m_frmEOD.pbProgress.Min = 0
    m_frmEOD.pbProgress.Value = 0
    m_frmEOD.pbProgress.Max = 100
    iStoredTradesCount = 0
    
    nCount = m_collPos.Count + m_collUndForVolaClear.Count
    
    If nCount > 0 Then
        m_frmEOD.pbProgress.Max = nCount
        nNewCount = 0
        
        For i = 1 To m_collPos.Count
            On Error GoTo Failed
            Set aPos = m_collPos(i)
                        
            gDBW.usp_BoCustomStrikeSkewPoint_Default_Save aPos.Und.ID, aPos.PriceOld, aPos.QtyNew, aPos.PriceNew, aPos.RootSymbol
            
            
            nNewCount = nNewCount + 1
Skip:
            On Error Resume Next
            m_frmEOD.pbProgress.Value = m_frmEOD.pbProgress.Value + 1
            On Error GoTo Herr
            m_frmEOD.Idle
        Next
    End If
    
    SaveVolatilityToDatabase = nNewCount
            
    Exit Function
Herr:
    m_frmEOD.LogMsg "Error saving volatilities: " & Err.Description
    Exit Function

Failed:
    m_frmEOD.LogMsg "Error saving volatilities: " & Err.Description
    Resume Skip
End Function

Private Function ParseConfigFile(ByVal sFileName As String) As Boolean
    On Error Resume Next
    Dim Buffer$, nLength&, nFile&
    
    nLength = FileLen(sFileName)
    If Not nLength > 0 Then
        ParseConfigFile = False
        Exit Function
    End If
    
    Buffer = Space(nLength)
    nFile = FreeFile
    Open sFileName For Binary Access Read As #nFile
    Get #nFile, , Buffer
    Close #nFile
    
    Dim Lines As Variant
    Dim Items As Variant
    Dim sLine As String
    Dim nBegin&, nEnd&, nLineNumber&
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            ParseConfigFile = False
            Exit Function
        End If
    End If
    
    nBegin = 0
    nEnd = UBound(Lines)
    
    m_collFieldsPosition.Clear
    m_nMinLineLength = 0
    
    For nLineNumber = nBegin To nEnd
        sLine = Lines(nLineNumber)
        Items = Split(sLine, ",")
        
        If UBound(Items) >= 3 Then
            Dim sName$, nStart&, nLen&, nOptional&
            
            sName = Trim$(Items(0))
            nStart = CInt(Trim$(Items(1)))
            nLen = CInt(Trim$(Items(3)))
            
            If sName <> "" And nStart > 0 And nLen > 0 Then
                Dim aField As clsFieldPositionAtom
                Set aField = m_collFieldsPosition.Add(sName)
                
                aField.nStart = nStart
                aField.nLength = nLen
                If UBound(Items) >= 4 Then aField.nOptional = CInt(Trim$(Items(4)))
                
                If m_nMinLineLength < aField.nStart + aField.nLength - 1 Then
                    m_nMinLineLength = aField.nStart + aField.nLength - 1
                End If
            End If
        End If
        
        Erase Items
    Next
    
    Erase Lines
    ParseConfigFile = True
End Function

Private Function IsConfigValid() As Boolean
    On Error Resume Next
    
    If IsFieldExist(FLD_ACCOUNT) And IsFieldExist(FLD_CALLPUT) And IsFieldExist(FLD_BASESYM) And _
       IsFieldExist(FLD_EXPYEAR) And IsFieldExist(FLD_EXPMONTH) And IsFieldExist(FLD_STRIKE) And _
       IsFieldExist(FLD_STRIKEFRAC) And IsFieldExist(FLD_LONGSHORT) And IsFieldExist(FLD_SECTYPE) And _
       IsFieldExist(FLD_MARKETPRICE) And IsFieldExist(FLD_POSITION) And IsFieldExist(FLD_SYMBOL) Then
            IsConfigValid = True
            Exit Function
    End If
    
    IsConfigValid = False
End Function

Private Function IsFieldExist(ByVal sName As String) As Boolean
    On Error Resume Next
    Dim aField As clsFieldPositionAtom
    
    Set aField = m_collFieldsPosition(sName)
    If aField Is Nothing Then
        m_frmEOD.LogMsg "Can't find description for field: " & sName
        IsFieldExist = False
        Exit Function
    End If
    
    IsFieldExist = True
End Function
