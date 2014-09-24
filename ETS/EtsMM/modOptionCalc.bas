Attribute VB_Name = "modOptionCalc"
Option Explicit

Public Enum IrRuleType
    enAlwaysUseNeutralRate = 0
    enRateBasedOnPosition = 1
End Enum

Public Type InterestRateCurve
    ID As Long
    Name As String
    RuleType As IrRuleType
    CurveType As Long
    PosThreshold As Currency
End Type

Public g_IRs() As InterestRateCurve

Public Const MM_EGAR_BS                As Long = &H1&
Public Const MM_EGAR_BINOMIAL          As Long = &H2&
Public Const MM_EGAR_OPTIMIZED         As Long = &H3&
Public Const MM_EGAR_VSKLOG            As Long = &H4&

Public Const GM_NONE         As Long = &H0&
Public Const GM_THEOPRICE    As Long = &H1&
Public Const GM_DELTA        As Long = &H4&
Public Const GM_GAMMA        As Long = &H8&
Public Const GM_VEGA         As Long = &H10&
Public Const GM_THETA        As Long = &H20&
Public Const GM_RHO          As Long = &H40&
Public Const GM_VOLGA        As Long = &H80&

Public Const GM_DELTA_VEGA   As Long = &H100&
Public Const GM_DELTA_THETA  As Long = &H200&
Public Const GM_GAMMA_VEGA   As Long = &H400&
Public Const GM_GAMMA_THETA  As Long = &H800&
Public Const GM_ALL          As Long = &HFFFFFFFF

Public Const VF_OK          As Long = &H0&
Public Const VF_TOOLOW      As Long = &H1&
Public Const VF_TOOHIGH     As Long = &H2&
Public Const VF_UNRELIABLE  As Long = &H3&

Public Type GreeksData
    nMask As Long

    dTheoPrice As Double
    dAlpha As Double
    dDelta As Double
    dGamma As Double
    dVega As Double
    dTheta As Double
    dRho As Double
    dVolga As Double
    dDeltaVega As Double
    dDeltaTheta As Double
    dGammaVega As Double
    dGammaTheta As Double
End Type

Public Type REGULAR_DIVIDENDS
    nLastDivDate As Long
    nFrequency As Long
    dAmount As Double
End Type

Public Declare Function GetNYDateTimeAsDATE Lib "OptionCalc.dll" (ByRef dtNow As Double) As Long
Public Declare Function GetCalculationParams Lib "OptionCalc.dll" (ByVal dtNow As Double, _
                                                                    ByVal dtExpiry As Double, _
                                                                    ByVal dtCloseTime As Double, _
                                                                    ByVal bUseTime As Boolean, _
                                                                    ByRef dtDateCalc As Double, _
                                                                    ByRef dtExpiryCalc As Double, _
                                                                    ByRef dtCloseTimeCalc As Double, _
                                                                    ByRef dYte As Double) As Long

'Public Declare Function InterpolateRates2 Lib "OptionCalc.dll" (ByVal nCount As Long, pRates As Double, pdYTEs As Double, ByVal dYTE As Double) As Double
                                        
'Public Declare Function GetDividendsCount Lib "OptionCalc.dll" (ByVal dToday As Double, ByVal dYTE As Double, ByVal dLastDivDate As Double, ByVal nFrequency As Long) As Long
                                        
'Public Declare Function GetDividends2 Lib "OptionCalc.dll" (ByVal dToday As Double, ByVal dYTE As Double, ByVal dLastDivDate As Double, ByVal nFrequency As Long, ByVal dAmount As Double, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, pnCount As Long) As Long
                                        
'Public Declare Function GetBasketDividendsCount Lib "OptionCalc.dll" (ByVal dToday As Double, ByVal dDTE As Double, pDividends As REGULAR_DIVIDENDS, ByVal nCount As Long) As Long

'Public Declare Function GetBasketDividends Lib "OptionCalc.dll" (ByVal dToday As Double, ByVal dDTE As Double, pDividends As REGULAR_DIVIDENDS, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, ByVal nInCount As Long, pnOutCount As Long) As Long

Public Declare Function CalcGreeksMM2 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dStrike As Double, ByVal dVolatility As Double, ByVal dYte As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, pGreeks As GreeksData) As Long
                                       
'Public Declare Function CalcVolatilityMM2 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYte As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivDays As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long) As Double

Public Declare Function CalcVolatilityMM3 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYte As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivDays As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, ByRef nFlag As Long) As Double

Public Declare Function CalcFutureOptionGreeks2 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, _
                                                                      ByVal dFuturePrice As Double, _
                                                                      ByVal bSpotGreeks As Boolean, _
                                                                      ByVal dStrike As Double, _
                                                                      ByVal dVolatility As Double, _
                                                                      ByVal dYte As Double, _
                                                                      ByVal nIsCall As Long, _
                                                                      ByVal nIsAmerican As Long, _
                                                                      ByVal nSteps As Long, _
                                                                      ByVal dSkew As Double, _
                                                                      ByVal dKurt As Double, _
                                                                      ByVal nModel As Long, _
                                                                      pGreeks As GreeksData) As Long
                                                    
Public Declare Function CalcFutureOptionGreeks3 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, _
                                                                      ByVal dYield As Double, _
                                                                      ByVal dFuturePrice As Double, _
                                                                      ByVal bSpotGreeks As Boolean, _
                                                                      ByVal dStrike As Double, _
                                                                      ByVal dVolatility As Double, _
                                                                      ByVal dYte As Double, _
                                                                      ByVal nIsCall As Long, _
                                                                      ByVal nIsAmerican As Long, _
                                                                      ByVal nSteps As Long, _
                                                                      ByVal dSkew As Double, _
                                                                      ByVal dKurt As Double, _
                                                                      ByVal nModel As Long, _
                                                                      ByVal nCount As Long, _
                                                                      pDivAmnts As Double, _
                                                                      pDivYears As Double, _
                                                                      pGreeks As GreeksData) As Long
                                        
Public Declare Function CalcFutureOptionVolatility Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dFuturePrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYte As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, ByRef nFlag As Long) As Double

Public Type VSGreeksData
    nMask As Long

    dTheoPrice As Double
    dAlpha As Double
    dDelta As Double
    dGamma As Double
    dVega As Double
    dTheta As Double
    dRho As Double
    dDeltaVega As Double
    dDeltaTheta As Double
    dGammaVega As Double
    dGammaTheta As Double
    dVomma As Double
    dVommaUp As Double
    dVommaDown As Double
    dVanna As Double
    dVannaUp As Double
    dVannaDown As Double
    dDeltaUp As Double
    dDeltaDown As Double
    dVegaUp As Double
    dVegaDown As Double
End Type

Public Declare Function VarianceSwapGreeks Lib "OptionCalc.dll" (ByVal dtValuationDate As Double, _
                                                                      ByVal dRate As Double, _
                                                                      ByVal dSpotPrice As Double, _
                                                                      ByVal dYield As Double, _
                                                                      pDivAmnts As Double, _
                                                                      pDivDates As Double, _
                                                                      ByVal nDivCount As Long, _
                                                                      ByVal dtExpirationDate As Double, _
                                                                      pStrikesPrices As Double, _
                                                                      pCallVolatilities As Double, _
                                                                      pPutVolatilities As Double, _
                                                                      ByVal nStrikesCount As Long, _
                                                                      VSGeeks As VSGreeksData) As Long
                                                                      
Public Declare Function ImpliedVariance Lib "OptionCalc.dll" (ByVal dtValuationDate As _
                                                               Double, ByVal dInterestRate As _
                                                               Double, ByVal dSpotPrice As _
                                                               Double, ByVal dDividendYield As _
                                                               Double, pdDividendAmounts As _
                                                               Double, pdtDividendDates As _
                                                               Double, ByVal nDividendCount As _
                                                               Long, ByVal dtExpirationDate As _
                                                               Double, pdStrikePrices As _
                                                               Double, pdCallVolatilities As _
                                                               Double, pdPutVolatilities As _
                                                               Double, ByVal nSkewSize As Long) As Double
                                                               
Public Declare Function RealizedVariance Lib "OptionCalc.dll" (pDates As Date, _
                                                                pPrices As Double, _
                                                                ByVal nDaysCount As Long, _
                                                                ByVal dtFrom As Double, _
                                                                ByVal dtTo As Double, _
                                                                pdDividendAmounts As Double, _
                                                                pdtDividendDates As Double, _
                                                                ByVal nDividendsCount As Long) As Double


Public Function PriceMidEx(ByVal dPriceBid As Double, ByVal dPriceAsk As Double, ByVal dPriceLast As Double, Optional ByVal bUseLast As Boolean = True) As Double
    If dPriceBid > 0# And dPriceAsk > 0# Then
        PriceMidEx = (dPriceBid + dPriceAsk) / 2#
    ElseIf bUseLast And dPriceLast > 0# Then
        PriceMidEx = dPriceLast
    ElseIf dPriceBid > 0# Or dPriceAsk > 0# Then
        If dPriceBid > 0# And dPriceAsk <= 0# Then
            If bUseLast Then
                PriceMidEx = dPriceBid
            Else
                PriceMidEx = dPriceBid / 2#
            End If
        Else
            If bUseLast Then
                PriceMidEx = dPriceAsk
            Else
                PriceMidEx = dPriceAsk / 2#
            End If
        End If
    Else
        PriceMidEx = BAD_DOUBLE_VALUE
    End If
End Function

Public Function VolaMidEx(ByVal dVolaCall As Double, ByVal dVolaPut As Double) As Double
    If dVolaCall > 0# And dVolaPut > 0# Then
        VolaMidEx = (dVolaCall + dVolaPut) / 2
    ElseIf dVolaCall > BAD_DOUBLE_VALUE Or dVolaPut > BAD_DOUBLE_VALUE Then
        If dVolaCall > BAD_DOUBLE_VALUE And dVolaPut = BAD_DOUBLE_VALUE Then
            VolaMidEx = dVolaCall
        Else
            VolaMidEx = dVolaPut
        End If
    Else
        VolaMidEx = BAD_DOUBLE_VALUE
    End If
End Function

Public Sub ClearInterestRates()
    On Error Resume Next
    Erase g_IRs
End Sub

' lPeriodType: 1 - days, 2 - weeks, 3 - months, 4 - years
Public Function GetDays(ByVal dtDate As Date, ByVal lNumber As Long, ByVal lPeriodType As Long) As Long
    On Error Resume Next
    Select Case lPeriodType
        Case 1
            GetDays = DateDiff("d", dtDate, DateAdd("d", lNumber, dtDate))
        Case 2
            GetDays = DateDiff("d", dtDate, DateAdd("ww", lNumber, dtDate))
        Case 3
            GetDays = DateDiff("d", dtDate, DateAdd("m", lNumber, dtDate))
        Case 4
            GetDays = DateDiff("d", dtDate, DateAdd("yyyy", lNumber, dtDate))
        Case Else
            GetDays = 0
    End Select
End Function

Public Function GetYears(ByVal dtDate As Date, ByVal lNumber As Long, ByVal lPeriodType As Long) As Double
    On Error Resume Next
    GetYears = CDbl(GetDays(dtDate, lNumber, lPeriodType)) / 365#
End Function

Public Sub LoadInterestRates()
    On Error GoTo EH
    Dim rsCurve As ADODB.Recordset
    Dim rsPoint As ADODB.Recordset
    Dim nCurveCount&, nDTE&, nCurveIdx&, nPointIdx&
    Dim dYte As Double
    
    ClearInterestRates
    nCurveCount = 0&
    
    Set rsCurve = gDBW.usp_IRCurve_Get
    nCurveCount = rsCurve.RecordCount
    If nCurveCount > 0 Then
        ReDim g_IRs(0 To nCurveCount)
        nCurveIdx = 0
        
        While Not rsCurve.EOF
            g_IRs(nCurveIdx).ID = ReadLng(rsCurve!iCurveID)
            g_IRs(nCurveIdx).Name = ReadLng(rsCurve!vcCurveName)
            g_IRs(nCurveIdx).RuleType = IIf(ReadLng(rsCurve!iRuleTypeID) <> 0, enRateBasedOnPosition, enAlwaysUseNeutralRate)
            g_IRs(nCurveIdx).CurveType = ReadLng(rsCurve!iCurveTypeID)
            g_IRs(nCurveIdx).PosThreshold = ReadCur(rsCurve!mPositionThreshold)
            
            nCurveIdx = nCurveIdx + 1
            rsCurve.MoveNext
        Wend
    End If
Ex:
    Set rsCurve = Nothing
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = "Fail to load interest rates." & vbCrLf & Err.Source
    
    ClearInterestRates
    Set rsCurve = Nothing
    
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

Public Sub ReloadRates(ByVal iContractID As Long)
On Error GoTo err_exception
    Dim iID As Long
    Dim rsRates As ADODB.Recordset
    Dim aNewRates As EtsGeneralLib.EtsRateColl
    Set aNewRates = New EtsGeneralLib.EtsRateColl
    Dim aRate As EtsGeneralLib.EtsRateAtom
    Dim enPeriodType As EtsGeneralLib.EtsPeriodTypeEnum
    Dim iPeriodAmount As Long
    Dim iPeriodInDays As Long
    
    aNewRates.Clear
    
    Set rsRates = gDBW.usp_IRPoint_Get(Null, 0, iContractID)
    If rsRates.RecordCount > 0 Then
        
        While Not rsRates.EOF
            iID = ReadLng(rsRates!iContractID)
            If (iID = iContractID) Then
                enPeriodType = ReadLng(rsRates!iPeriodTypeID)
                iPeriodAmount = ReadLng(rsRates!iNum)
                iPeriodInDays = aNewRates.GetPeriodInDays(Date, enPeriodType, iPeriodAmount)
                Set aRate = aNewRates.Add(iPeriodInDays)
                
                If (Not aRate Is Nothing) Then
                    aRate.ID = ReadLng(rsRates!iIRpointID)
                    aRate.LongRate = ReadDbl(rsRates!fLongRate)
                    aRate.ShortRate = ReadDbl(rsRates!fShortRate)
                    aRate.PeriodAmount = iPeriodAmount
                    aRate.PeriodType = enPeriodType
                End If
                Set aRate = Nothing
            End If
            rsRates.MoveNext
        Wend
                
    End If
    Set rsRates = Nothing
    
    'Reset global collection
    If (Not g_UnderlyingAll(iContractID) Is Nothing) Then
        Set g_UnderlyingAll(iContractID).HTBRates = aNewRates
    End If

Exit Sub
err_exception:
    Debug.Print "Error while reloading rates."
End Sub

Public Function GetShortRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    GetShortRate = 0
    GetShortRate = g_Main.Rates.GetShortRate(dtToday, dtDate)
End Function

Public Function GetLongRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
        
    GetLongRate = 0
    GetLongRate = g_Main.Rates.GetLongRate(dtToday, dtDate)
End Function

Public Function GetNeutralRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next

    GetNeutralRate = 0
    GetNeutralRate = g_Main.Rates.GetNeutralRate(dtToday, dtDate)
End Function

Public Function IsHTBRatesExist(ByVal ID As Long, Optional ByVal bShowEventLog As Boolean = True) As Boolean
    On Error Resume Next
    IsHTBRatesExist = False
    
    Dim aUnd As EtsGeneralLib.UndAtom
    Set aUnd = g_UnderlyingAll(ID)
    
    If aUnd Is Nothing Then Exit Function
    If Not aUnd.IsHTB Then Exit Function
    
    If Not aUnd.HTBRates Is Nothing Then
        If aUnd.HTBRates.Count > 0 Then IsHTBRatesExist = True
    End If
    
    If Not IsHTBRatesExist And bShowEventLog Then _
        LogEvent EVENT_ERROR, "Can't find HTB rates for underlying " & aUnd.Symbol & "."
End Function


Public Function GetHTBRate(ByVal ID As Long, ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    GetHTBRate = BAD_DOUBLE_VALUE
    
    Dim aUnd As EtsGeneralLib.UndAtom
    Set aUnd = g_UnderlyingAll(ID)
    
    If aUnd Is Nothing Then Exit Function
    If aUnd.HTBRates Is Nothing Then Exit Function
    If aUnd.HTBRates.Count = 0 Then Exit Function
    
    If aUnd.IsAllowLending Then
        GetHTBRate = IIf(aUnd.UndPosForRates <= 0, aUnd.HTBRates.GetShortRate(dtToday, dtDate), _
                                                  aUnd.HTBRates.GetLongRate(dtToday, dtDate))
    Else
        If aUnd.UndPosForRates <= 0 Then GetHTBRate = aUnd.HTBRates.GetShortRate(dtToday, dtDate)
    End If
End Function

'Public Function GetHTBRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
'    On Error Resume Next
'
'    If g_IRs(0).PointsCount > 0 Then
'        GetHTBRate = InterpolateRates2(g_IRs(0).PointsCount, g_IRs(0).HTBRateValue(0), g_IRs(0).HTBRateDTE(0), CDbl(dtDate - dtToday) / 365#)
'    Else
'        GetHTBRate = 0
'    End If
'End Function

'Public Function GetNeutralHTBRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
'    On Error Resume Next
'
'    If g_IRs(0).PointsCount > 0 Then
'        GetNeutralHTBRate = InterpolateRates2(g_IRs(0).PointsCount, g_IRs(0).NeutralHTBRateValue(0), g_IRs(0).NeutralHTBRateDTE(0), CDbl(dtDate - dtToday) / 365#)
'    Else
'        GetNeutralHTBRate = 0
'    End If
'End Function
Public Function GetPosThreshold() As Currency
    On Error Resume Next
    
    GetPosThreshold = g_IRs(0).PosThreshold
End Function

Public Function GetIrRuleType() As IrRuleType
    On Error Resume Next
    
    GetIrRuleType = g_IRs(0).RuleType
End Function

Public Sub FixGreeks(aGreeks As GreeksData)
    On Error Resume Next
    FixBadDouble aGreeks.dTheoPrice
    If aGreeks.dTheoPrice < 0 Then aGreeks.dTheoPrice = BAD_DOUBLE_VALUE
    
    FixBadDouble aGreeks.dAlpha
    FixBadDouble aGreeks.dDelta
    FixBadDouble aGreeks.dGamma
    FixBadDouble aGreeks.dVega
    FixBadDouble aGreeks.dTheta
    FixBadDouble aGreeks.dRho
    FixBadDouble aGreeks.dVolga
    FixBadDouble aGreeks.dDeltaVega
    FixBadDouble aGreeks.dDeltaTheta
    FixBadDouble aGreeks.dGammaVega
    FixBadDouble aGreeks.dGammaTheta
End Sub

Public Sub ClearGreeks(aGreeks As GreeksData)
    On Error Resume Next
    
    aGreeks.dAlpha = 0#
    aGreeks.dDelta = 0#
    aGreeks.dGamma = 0#
    aGreeks.dVega = 0#
    aGreeks.dTheta = 0#
    aGreeks.dRho = 0#
    aGreeks.dDeltaVega = 0#
    aGreeks.dDeltaTheta = 0#
    aGreeks.dGammaVega = 0#
    aGreeks.dGammaTheta = 0#
End Sub

' fix INF & NAN values
Public Sub FixBadDouble(Value As Double)
    On Error Resume Next
    Value = Value + 0
    
    If Err.Number <> 0 Then
        Value = BAD_DOUBLE_VALUE
    End If
End Sub

'Public Function GetBasketIndexDividends(aIdx As EtsGeneralLib.IndexAtom, aDivs() As REGULAR_DIVIDENDS) As Long
'    On Error Resume Next
'    Dim i&, nCount&, aIdxDiv As EtsGeneralLib.EtsRegularDividend
'    Erase aDivs
'
'    nCount = aIdx.BasketDivsCount
'    If aIdx.IsBasket And nCount > 0 Then
'        ReDim aDivs(0 To nCount - 1)
'
'        For i = 0 To nCount - 1
'            aIdxDiv = aIdx.BasketDiv(i)
'            aDivs(i).nLastDivDate = aIdxDiv.LastDate
'            aDivs(i).nFrequency = aIdxDiv.Freq
'            aDivs(i).dAmount = aIdxDiv.amt
'        Next
'
'        GetBasketIndexDividends = nCount
'    Else
'        GetBasketIndexDividends = 0
'        ReDim aDivs(0 To 0)
'    End If
'End Function

'Public Function GetSyntheticBasketDividends(aSynthRoot As EtsGeneralLib.SynthRootAtom, aDivs() As REGULAR_DIVIDENDS) As Long
'    On Error Resume Next
'    Dim i&, nCount&, aDiv As EtsGeneralLib.EtsRegularDividend
'    Erase aDivs
'
'    nCount = aSynthRoot.BasketDivsCount
'    If aSynthRoot.Basket And nCount > 0 Then
'        ReDim aDivs(0 To nCount - 1)
'
'        For i = 0 To nCount - 1
'            aDiv = aSynthRoot.BasketDiv(i)
'            aDivs(i).nLastDivDate = aDiv.LastDate
'            aDivs(i).nFrequency = aDiv.Freq
'            aDivs(i).dAmount = aDiv.amt
'        Next
'
'        GetSyntheticBasketDividends = nCount
'    Else
'        GetSyntheticBasketDividends = 0
'        ReDim aDivs(0 To 0)
'    End If
'End Function




