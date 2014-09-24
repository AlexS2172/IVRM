Attribute VB_Name = "modOptionCalc"
Option Explicit

Public Const GT_THEOPRICE    As Long = &H1&
Public Const GT_DELTA        As Long = &H4&
Public Const GT_GAMMA        As Long = &H8&
Public Const GT_VEGA         As Long = &H10&
Public Const GT_THETA        As Long = &H20&
Public Const GT_RHO          As Long = &H40&
Public Const GT_DELTA_VEGA   As Long = &H100&
Public Const GT_DELTA_THETA  As Long = &H200&
Public Const GT_GAMMA_VEGA   As Long = &H400&
Public Const GT_GAMMA_THETA  As Long = &H800&
Public Const GT_ALL          As Long = &HFFFFFFFF

Public Type GreeksData
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
End Type

Public Declare Function InterpolateRates Lib "OptionCalc.dll" ( _
                                        ByVal nCount As Long, _
                                        pRates As InterestRatePoint, _
                                        ByVal nDTE As Long) As Double
                                        
Public Declare Function GetDividendsCount Lib "OptionCalc.dll" ( _
                                        ByVal nToday As Long, _
                                        ByVal nDTE As Long, _
                                        ByVal nLastDivDate As Long, _
                                        ByVal nFrequency As Long) As Long
                                        
Public Declare Function GetDividends2 Lib "OptionCalc.dll" ( _
                                        ByVal nToday As Long, _
                                        ByVal nDTE As Long, _
                                        ByVal nLastDivDate As Long, _
                                        ByVal nFrequency As Long, _
                                        ByVal dAmount As Double, _
                                        ByVal nCount As Long, _
                                        pDivAmnts As Double, _
                                        pDivYears As Double, _
                                        pnCount As Long) As Long
                                        
Public Declare Function CalcGreeksMM Lib "OptionCalc.dll" ( _
                                        ByVal dDomesticRate As Double, _
                                        ByVal dForeignRate As Double, _
                                        ByVal dSpotPrice As Double, _
                                        ByVal dStrike As Double, _
                                        ByVal dVolatility As Double, _
                                        ByVal nDTE As Long, _
                                        ByVal nIsCall As Long, _
                                        ByVal nIsAmerican As Long, _
                                        ByVal nCount As Long, _
                                        pDivAmnts As Double, _
                                        pDivYears As Double, _
                                        ByVal nSteps As Long, _
                                        ByVal nModel As Long, _
                                        pGreeks As GreeksData) As Long
                                        
Public Declare Function CalcVolatilityMM Lib "OptionCalc.dll" ( _
                                        ByVal dDomesticRate As Double, _
                                        ByVal dForeignRate As Double, _
                                        ByVal dSpotPrice As Double, _
                                        ByVal dOptionPrice As Double, _
                                        ByVal dStrike As Double, _
                                        ByVal nDTE As Long, _
                                        ByVal nIsCall As Long, _
                                        ByVal nIsAmerican As Long, _
                                        ByVal nCount As Long, _
                                        pDivAmnts As Double, _
                                        pDivDays As Double, _
                                        ByVal nSteps As Long, _
                                        ByVal nModel As Long) As Double

'Public Declare Function GetDividends Lib "OptionCalc.dll" ( _
'                                        ByVal nToday As Long, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nLastDivDate As Long, _
'                                        ByVal nFrequency As Long, _
'                                        ByVal dAmount As Double, _
'                                        ByVal nCount As Long, _
'                                        pDividends As DIVIDEND, _
'                                        pnCount As Long) As Long
                                        
'Public Declare Function GetDividendsM Lib "OptionCalc.dll" ( _
'                                        ByVal nToday As Long, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nLastDivDate As Long, _
'                                        ByVal nFrequency As Long, _
'                                        ByVal dAmount As Double, _
'                                        ByVal nCount As Long, _
'                                        pDivAmnts As Double, _
'                                        pDivDays As Double, _
'                                        pnCount As Long) As Long
                                        
'Public Declare Function CalcGreeks Lib "OptionCalc.dll" ( _
'                                        ByVal dDomesticRate As Double, _
'                                        ByVal dForeignRate As Double, _
'                                        ByVal dSpotPrice As Double, _
'                                        ByVal dStrike As Double, _
'                                        ByVal dVolatility As Double, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nIsCall As Long, _
'                                        ByVal nIsAmerican As Long, _
'                                        ByVal nCount As Long, _
'                                        pDividends As DIVIDEND, _
'                                        ByVal nSteps As Long, _
'                                        pGreeks As GreeksData) As Long
                        
'Public Declare Function CalcGreeksM Lib "OptionCalc.dll" ( _
'                                        ByVal dDomesticRate As Double, _
'                                        ByVal dForeignRate As Double, _
'                                        ByVal dSpotPrice As Double, _
'                                        ByVal dStrike As Double, _
'                                        ByVal dVolatility As Double, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nIsCall As Long, _
'                                        ByVal nCount As Long, _
'                                        pDivAmnts As Double, _
'                                        pDivDays As Double, _
'                                        pGreeks As GreeksData) As Long
                                        
'Public Declare Function CalcGreeksEx Lib "OptionCalc.dll" ( _
'                                        ByVal dDomesticRate As Double, _
'                                        ByVal dForeignRate As Double, _
'                                        ByVal dSpotPrice As Double, _
'                                        ByVal dStrike As Double, _
'                                        ByVal dVolatility As Double, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nIsCall As Long, _
'                                        ByVal nIsAmerican As Long, _
'                                        ByVal nCount As Long, _
'                                        pDividends As DIVIDEND, _
'                                        ByVal nSteps As Long, _
'                                        pGreeks As GREEKS) As Long
                                        
'Public Declare Function CalcVolatility Lib "OptionCalc.dll" ( _
'                                        ByVal dDomesticRate As Double, _
'                                        ByVal dForeignRate As Double, _
'                                        ByVal dSpotPrice As Double, _
'                                        ByVal dOptionPrice As Double, _
'                                        ByVal dStrike As Double, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nIsCall As Long, _
'                                        ByVal nIsAmerican As Long, _
'                                        ByVal nCount As Long, _
'                                        pDividends As DIVIDEND, _
'                                        ByVal nSteps As Long) As Double

'Public Declare Function CalcVolatilityM Lib "OptionCalc.dll" ( _
'                                        ByVal dDomesticRate As Double, _
'                                        ByVal dForeignRate As Double, _
'                                        ByVal dSpotPrice As Double, _
'                                        ByVal dOptionPrice As Double, _
'                                        ByVal dStrike As Double, _
'                                        ByVal nDTE As Long, _
'                                        ByVal nIsCall As Long, _
'                                        ByVal nCount As Long, _
'                                        pDivAmnts As Double, _
'                                        pDivDays As Double) As Double

