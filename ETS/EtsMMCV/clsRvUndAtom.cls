VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ID As Long
Public ContractType As EtsGeneralLib.EtsContractTypeEnum
Public Symbol As String

Public Yield As Double
Public DivFreq As Long
Public DivDate As Date
Public DivAmt As Double
Public IsAmerican As Boolean
Public IsHTB As Boolean
Public ExpCalendarID As Long

Public Skew As Double
Public Kurt As Double

Public CalcGreeks As Boolean
Public CalcTotals As Boolean

Public VolaUpdated As Boolean
Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double
Public PriceClose As Double
Public LotSize As Long

Public Beta As Double

Public Qty As Long
Public OptQty As Long
Public UndPosForRates As Double
Public FutQty As Long

Public PnlMtm As Double
Public PnlTheo As Double

Public NetDelta As Double
Public OptDelta As Double
Public Gamma As Double
Public GammaPerc As Double
Public Vega As Double
Public WtdVega As Double
Public Theta As Double
Public DeltaEq As Double
Public GammaEq As Double
Public Rho As Double
Public ThetaDelta As Double
Public ThetaGamma As Double
Public VegaDelta As Double
Public VegaGamma As Double
Public TimeValue As Double
Public BetaWtdDeltaEq As Double

Public BadPnlMtm As Boolean
Public BadPnlTheo As Boolean
Public BadNetDelta As Boolean
Public BadOptDelta As Boolean
Public BadGamma As Boolean
Public BadGammaPerc As Boolean
Public BadVega As Boolean
Public BadWtdVega As Boolean
Public BadTheta As Boolean
Public BadDeltaEq As Boolean
Public BadGammaEq As Boolean
Public BadRho As Boolean
Public BadThetaDelta As Boolean
Public BadThetaGamma As Boolean
Public BadVegaDelta As Boolean
Public BadVegaGamma As Boolean
Public BadTimeValue As Boolean
Public BadBetaWtdDeltaEq As Boolean

Public Pos As New clsRvPosColl
Public HasOptPos As Boolean
Public VolaSrv As EtsGeneralLib.VolaControl

Public BasketIndex As EtsGeneralLib.IndexAtom

Public UseMidRates As Boolean

Public HasSynthetic As Boolean
Public SyntheticRoots As EtsGeneralLib.SynthRootColl
Public SynthGreeks As clsRvSynthGreeksColl
Public SynthPos As clsRvPosColl
Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public ReplacePriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum

Public Fut As clsRvFutColl

Private Sub Class_Initialize()
    On Error Resume Next
    
    Set VolaSrv = New EtsGeneralLib.VolaControl
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to create volatility control: " & Err.Description
    
    Clear
End Sub

Public Sub Clear()
    On Error Resume Next
    ID = 0
    ContractType = 0
    Symbol = ""
    UseMidRates = False
    HasOptPos = False
    
    Yield = 0
    DivFreq = 0
    DivDate = 0
    DivAmt = 0
    IsAmerican = False
    Skew = 0
    Kurt = 0
    
    CalcGreeks = False
    CalcTotals = False
    
    PriceBid = BAD_DOUBLE_VALUE
    PriceAsk = BAD_DOUBLE_VALUE
    PriceLast = BAD_DOUBLE_VALUE
    PriceClose = BAD_DOUBLE_VALUE
    LotSize = BAD_LONG_VALUE

    Beta = BAD_DOUBLE_VALUE
    
    Qty = BAD_LONG_VALUE
    OptQty = BAD_LONG_VALUE
    FutQty = BAD_LONG_VALUE
    UndPosForRates = 0
    
    Pos.Clear
    VolaSrv.CleanUp
    
    If Not SynthGreeks Is Nothing Then
        SynthGreeks.Clear
    End If
    
    If Not SynthPos Is Nothing Then
        SynthPos.Clear
    End If
    
    Set UndPriceProfile = Nothing
    Set OptPriceProfile = Nothing
    Set Fut = Nothing
    
    ReplacePriceStatus = enRpsNone
    
    ClearValues
End Sub

Public Sub ClearValues()
    Qty = BAD_LONG_VALUE
    OptQty = BAD_LONG_VALUE
    FutQty = BAD_LONG_VALUE
    PnlMtm = BAD_DOUBLE_VALUE
    PnlTheo = BAD_DOUBLE_VALUE
    NetDelta = BAD_DOUBLE_VALUE
    OptDelta = BAD_DOUBLE_VALUE
    Gamma = BAD_DOUBLE_VALUE
    GammaPerc = BAD_DOUBLE_VALUE
    Vega = BAD_DOUBLE_VALUE
    WtdVega = BAD_DOUBLE_VALUE
    Theta = BAD_DOUBLE_VALUE
    DeltaEq = BAD_DOUBLE_VALUE
    GammaEq = BAD_DOUBLE_VALUE
    Rho = BAD_DOUBLE_VALUE
    ThetaDelta = BAD_DOUBLE_VALUE
    ThetaGamma = BAD_DOUBLE_VALUE
    VegaDelta = BAD_DOUBLE_VALUE
    VegaGamma = BAD_DOUBLE_VALUE
    TimeValue = BAD_DOUBLE_VALUE
    BetaWtdDeltaEq = BAD_DOUBLE_VALUE

    BadPnlMtm = False
    BadPnlTheo = False
    BadNetDelta = False
    BadOptDelta = False
    BadGamma = False
    BadGammaPerc = False
    BadVega = False
    BadWtdVega = False
    BadTheta = False
    BadDeltaEq = False
    BadGammaEq = False
    BadRho = False
    BadThetaDelta = False
    BadThetaGamma = False
    BadVegaDelta = False
    BadVegaGamma = False
    BadTimeValue = False
    BadBetaWtdDeltaEq = False
End Sub

Public Sub SetAllValuesAsBad()
    BadPnlMtm = True
    BadPnlTheo = True
    BadNetDelta = True
    BadOptDelta = True
    BadGamma = True
    BadGammaPerc = True
    BadVega = True
    BadWtdVega = True
    BadTheta = True
    BadDeltaEq = True
    BadGammaEq = True
    BadRho = True
    BadThetaDelta = True
    BadThetaGamma = True
    BadVegaDelta = True
    BadVegaGamma = True
    BadTimeValue = True
    BadWtdVega = True
    BadBetaWtdDeltaEq = True
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set BasketIndex = Nothing
    Set VolaSrv = Nothing
    Set UndPriceProfile = Nothing
    Set OptPriceProfile = Nothing
    Set Fut = Nothing
End Sub

Public Sub GetSyntheticUnderlyingPrice(ByVal nOptRootID As Long, ByRef collUnd As clsRvUndColl, _
                                            ByRef dPriceBid As Double, ByRef dPriceAsk As Double, ByRef dPriceLast As Double)
            
    Dim SynthRoot As EtsGeneralLib.SynthRootAtom, dPrice#, SynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim Und As clsRvUndAtom, bBadPriceBid As Boolean, bBadPriceLast As Boolean, bBadPriceAsk As Boolean

    On Error Resume Next
    
    dPriceBid = BAD_DOUBLE_VALUE
    dPriceAsk = BAD_DOUBLE_VALUE
    dPriceLast = BAD_DOUBLE_VALUE
    bBadPriceBid = False
    bBadPriceLast = False
    bBadPriceAsk = False
    
    If Not HasSynthetic Or SyntheticRoots Is Nothing Then Exit Sub
    
    Set SynthRoot = SyntheticRoots(nOptRootID)
    If SynthRoot Is Nothing Then Exit Sub
    
    dPriceBid = SynthRoot.CashValue
    dPriceAsk = dPriceBid
    dPriceLast = dPriceBid
    
    For Each SynthRootComp In SynthRoot.SynthRootComponents
        Set Und = collUnd(CStr(SynthRootComp.UndID))
        If Not Und Is Nothing Then
            
            If Not bBadPriceBid And Und.PriceBid > 0# Then
                dPriceBid = dPriceBid + Und.PriceBid * SynthRootComp.Weight
            Else
                bBadPriceBid = True
                dPriceBid = BAD_DOUBLE_VALUE
            End If
            
            If Not bBadPriceAsk And Und.PriceAsk > 0# Then
                dPriceAsk = dPriceAsk + Und.PriceAsk * SynthRootComp.Weight
            Else
                bBadPriceAsk = True
                dPriceAsk = BAD_DOUBLE_VALUE
            End If
            
            If Not bBadPriceLast And Und.PriceLast > 0# Then
                dPriceLast = dPriceLast + Und.PriceLast * SynthRootComp.Weight
            Else
                bBadPriceLast = True
                dPriceLast = BAD_DOUBLE_VALUE
            End If
            
            Set Und = Nothing
        End If
    Next
End Sub

