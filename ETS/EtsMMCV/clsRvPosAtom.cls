VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvPosAtom"
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

Public Visible As Boolean

Public OptType As Long
Public Expiry As Date
Public Strike As Double
Public Vola As Double
Public Rate As Double
Public VegaWeight As Double
Public ExpiryMonth As Long
Public OptionRootID As Long

Public CalcGreeks As Boolean

Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double
Public PriceClose As Double
Public LotSize As Long

Public Qty As Long
Public QtyInShares As Long

Public QtyLTDBuy As Long
Public QtyLTDSell As Long
Public PosLTDBuy As Double
Public PosLTDSell As Double
Public QtyDailyPrevDateBuy As Long
Public QtyDailyPrevDateSell As Long
Public PosDailyPrevDateBuy As Double
Public PosDailyPrevDateSell As Double
Public QtyDailyTodayBuy As Long
Public QtyDailyTodaySell As Long
Public PosDailyTodayBuy As Double
Public PosDailyTodaySell As Double

Public PnlMtm As Double
Public PnlTheo As Double
    
Public PriceTheo As Double
Public Delta As Double
Public Gamma As Double
Public Vega As Double
Public Theta As Double
Public Rho As Double
Public ThetaDelta As Double
Public ThetaGamma As Double
Public VegaDelta As Double
Public VegaGamma As Double
    
Public DeltaInShares As Double
Public GammaInShares As Double
Public GammaInSharesPerc As Double
Public VegaInShares As Double
Public WtdVega As Double
Public ThetaInShares As Double
Public RhoInShares As Double
Public ThetaDeltaInShares As Double
Public ThetaGammaInShares As Double
Public VegaDeltaInShares As Double
Public VegaGammaInShares As Double
Public TimeValue As Double

Public IsSynthetic As Boolean
Public SynthGreeks As clsRvSynthGreeksColl
Public UndID As Long
Public SynthBeta As Double
Public ReplacePriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum

Public Fut As clsRvFutAtom

Public ImportId As String
    
Private Sub Class_Initialize()
    CalcGreeks = False
    
    PriceBid = BAD_DOUBLE_VALUE
    PriceAsk = BAD_DOUBLE_VALUE
    PriceLast = BAD_DOUBLE_VALUE
    PriceClose = BAD_DOUBLE_VALUE
    LotSize = BAD_LONG_VALUE
    
    Vola = 0
    Rate = 0
    VegaWeight = 0

    Qty = BAD_LONG_VALUE
    QtyInShares = BAD_LONG_VALUE

    QtyLTDBuy = BAD_LONG_VALUE
    PosLTDBuy = BAD_DOUBLE_VALUE
    QtyDailyPrevDateBuy = BAD_LONG_VALUE
    PosDailyPrevDateBuy = BAD_DOUBLE_VALUE
    QtyDailyTodayBuy = BAD_LONG_VALUE
    PosDailyTodayBuy = BAD_DOUBLE_VALUE

    QtyLTDSell = BAD_LONG_VALUE
    PosLTDSell = BAD_DOUBLE_VALUE
    QtyDailyPrevDateSell = BAD_LONG_VALUE
    PosDailyPrevDateSell = BAD_DOUBLE_VALUE
    QtyDailyTodaySell = BAD_LONG_VALUE
    PosDailyTodaySell = BAD_DOUBLE_VALUE
    
    ReplacePriceStatus = enRpsNone

    ClearValues
    ClearValuesInShares
End Sub

Public Sub ClearValues()
    PriceTheo = BAD_DOUBLE_VALUE
    Delta = BAD_DOUBLE_VALUE
    Gamma = BAD_DOUBLE_VALUE
    Vega = BAD_DOUBLE_VALUE
    Theta = BAD_DOUBLE_VALUE
    Rho = BAD_DOUBLE_VALUE
    ThetaDelta = BAD_DOUBLE_VALUE
    ThetaGamma = BAD_DOUBLE_VALUE
    VegaDelta = BAD_DOUBLE_VALUE
    VegaGamma = BAD_DOUBLE_VALUE
End Sub

Public Sub ClearValuesInShares()
    PnlMtm = BAD_DOUBLE_VALUE
    PnlTheo = BAD_DOUBLE_VALUE

    DeltaInShares = BAD_DOUBLE_VALUE
    GammaInShares = BAD_DOUBLE_VALUE
    GammaInSharesPerc = BAD_DOUBLE_VALUE
    VegaInShares = BAD_DOUBLE_VALUE
    WtdVega = BAD_DOUBLE_VALUE
    ThetaInShares = BAD_DOUBLE_VALUE
    RhoInShares = BAD_DOUBLE_VALUE
    ThetaDeltaInShares = BAD_DOUBLE_VALUE
    ThetaGammaInShares = BAD_DOUBLE_VALUE
    VegaDeltaInShares = BAD_DOUBLE_VALUE
    VegaGammaInShares = BAD_DOUBLE_VALUE
    TimeValue = BAD_DOUBLE_VALUE
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set Fut = Nothing
End Sub
