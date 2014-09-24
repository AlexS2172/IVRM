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
'    PointsCount As Long
    
'    ShortRateDTE() As Long
'    ShortRateValue() As Double
'    LongRateDTE() As Long
'    LongRateValue() As Double
'    NeutralRateDTE() As Long
'    NeutralRateValue() As Double
'    HTBRateDTE() As Long
'    HTBRateValue() As Double
'    NeutralHTBRateDTE() As Long
'    NeutralHTBRateValue() As Double
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
Public Declare Function InterpolateRates2 Lib "OptionCalc.dll" (ByVal nCount As Long, pRates As Double, pnYTEs As Double, ByVal dYTE As Double) As Double
Public Declare Function CalcGreeksMM2 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dStrike As Double, ByVal dVolatility As Double, ByVal dYTE As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, pGreeks As GreeksData) As Long
Public Declare Function CalcVolatilityMM2 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYTE As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long) As Double
Public Declare Function CalcVolatilityMM3 Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dForeignRate As Double, ByVal dHTBRate As Double, ByVal dSpotPrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYTE As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nCount As Long, pDivAmnts As Double, pDivYears As Double, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, ByRef nFlag As Long) As Double
Public Declare Function CalcFutureOptionGreeks Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dFuturePrice As Double, ByVal dStrike As Double, ByVal dVolatility As Double, ByVal dYTE As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, pGreeks As GreeksData) As Long
Public Declare Function CalcFutureOptionVolatility Lib "OptionCalc.dll" (ByVal dDomesticRate As Double, ByVal dFuturePrice As Double, ByVal dOptionPrice As Double, ByVal dStrike As Double, ByVal dYTE As Double, ByVal nIsCall As Long, ByVal nIsAmerican As Long, ByVal nSteps As Long, ByVal dSkew As Double, ByVal dKurt As Double, ByVal nModel As Long, ByRef nFlag As Long) As Double

'Public Function PriceMidEx(ByVal dPriceBid As Double, ByVal dPriceAsk As Double, ByVal dPriceLast As Double, Optional ByVal bUseLast As Boolean = True) As Double
'    If dPriceBid > 0# And dPriceAsk > 0# Then
'        PriceMidEx = (dPriceBid + dPriceAsk) / 2#
'    ElseIf bUseLast And dPriceLast > 0# Then
'        PriceMidEx = dPriceLast
'    ElseIf dPriceBid > 0# Or dPriceAsk > 0# Then
'        If dPriceBid > 0# And dPriceAsk <= 0# Then
'            If bUseLast Then
'                PriceMidEx = dPriceBid
'            Else
'                PriceMidEx = dPriceBid / 2#
'            End If
'        Else
'            If bUseLast Then
'                PriceMidEx = dPriceAsk
'            Else
'                PriceMidEx = dPriceAsk / 2#
'            End If
'        End If
'    Else
'        PriceMidEx = BAD_DOUBLE_VALUE
'    End If
'End Function

'Public Function VolaMidEx(ByVal dVolaCall As Double, ByVal dVolaPut As Double) As Double
'    If dVolaCall > 0# And dVolaPut > 0# Then
'        VolaMidEx = (dVolaCall + dVolaPut) / 2
'    ElseIf dVolaCall > BAD_DOUBLE_VALUE Or dVolaPut > BAD_DOUBLE_VALUE Then
'        If dVolaCall > BAD_DOUBLE_VALUE And dVolaPut = BAD_DOUBLE_VALUE Then
'            VolaMidEx = dVolaCall
'        Else
'            VolaMidEx = dVolaPut
'        End If
'    Else
'        VolaMidEx = BAD_DOUBLE_VALUE
'    End If
'End Function

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
    Dim nCurveCount&, nCurveIdx&, nPointIdx&
    Dim dYTE As Double
    
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
EX:
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
'------------------------------------------------------------------------------------------------------------------------------
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
'            aDivs(i).dAmount = aIdxDiv.Amt
'        Next
'
'        GetBasketIndexDividends = nCount
'    Else
'        GetBasketIndexDividends = 0
'        ReDim aDivs(0 To 0)
'    End If
'End Function
'
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
'            aDivs(i).dAmount = aDiv.Amt
'        Next
'
'        GetSyntheticBasketDividends = nCount
'    Else
'        GetSyntheticBasketDividends = 0
'        ReDim aDivs(0 To 0)
'    End If
'End Function





