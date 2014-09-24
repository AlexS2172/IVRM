VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQnQuoteAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Exch As EtsGeneralLib.ExchAtom
'
'Public BidPrice As Double
'Public AskPrice As Double
'Public OpenPrice As Double
'Public ClosePrice As Double
'Public HighPrice As Double
'Public LowPrice As Double
'Public LastPrice As Double
'Public NetChange As Double
'Public SizeBid As Long
'Public SizeAsk As Long
'
'Public Volume As Long
'Public LotSize As Long
'Public OpenInterest As Long
'
'Public BidPriceTick As Double
'Public AskPriceTick As Double
'
'Public Series As String
''Public Vola As Double
'
'Public IV As Double
'Public IVBid As Double
'Public IVAsk As Double
'
'Public TheoPrice As Double
'Public Vega As Double
'Public Delta As Double
'Public Gamma As Double
'Public Theta As Double
'Public Rho As Double
'Public VegaDelta As Double
'Public VegaGamma As Double
'Public ThetaDelta As Double
'Public ThetaGamma As Double
'Public GammaPerTheta As Double
'
'Public TradingStatus As Long
'
'Public MmOpenQuantityBid As Long
'Public MmOpenQuantityAsk As Long
'Public MmPremiumBid As Double
'Public MmPremiumAsk As Double
'Public MmQuantityBid As Long
'Public MmQuantityAsk As Long
'Public MmStepUpBufferBid As Long
'Public MmStepUpBufferAsk As Long
'Public MmTickWorseLevelBid As Long
'Public MmTickWorseLevelAsk As Long
'
'Private Sub Class_Initialize()
'    BidPrice = BAD_DOUBLE_VALUE
'    AskPrice = BAD_DOUBLE_VALUE
'    OpenPrice = BAD_DOUBLE_VALUE
'    ClosePrice = BAD_DOUBLE_VALUE
'    HighPrice = BAD_DOUBLE_VALUE
'    LowPrice = BAD_DOUBLE_VALUE
'    LastPrice = BAD_DOUBLE_VALUE
'    NetChange = BAD_DOUBLE_VALUE
'    SizeBid = BAD_LONG_VALUE
'    SizeAsk = BAD_LONG_VALUE
'
'    Volume = BAD_LONG_VALUE
'    LotSize = BAD_LONG_VALUE
'    OpenInterest = BAD_LONG_VALUE
'
'    BidPriceTick = 0
'    AskPriceTick = 0
'
'    ClearValues True
'
'    MmOpenQuantityBid = BAD_LONG_VALUE
'    MmOpenQuantityAsk = BAD_LONG_VALUE
'    MmPremiumBid = BAD_DOUBLE_VALUE
'    MmPremiumAsk = BAD_DOUBLE_VALUE
'    MmQuantityBid = BAD_LONG_VALUE
'    MmQuantityAsk = BAD_LONG_VALUE
'    MmStepUpBufferBid = BAD_LONG_VALUE
'    MmStepUpBufferAsk = BAD_LONG_VALUE
'    MmTickWorseLevelBid = BAD_LONG_VALUE
'    MmTickWorseLevelAsk = BAD_LONG_VALUE
'End Sub
'
'Public Sub ClearValues(ByVal bWithIV As Boolean)
'    If bWithIV Then
'        IV = BAD_DOUBLE_VALUE
'        IVBid = BAD_DOUBLE_VALUE
'        IVAsk = BAD_DOUBLE_VALUE
'    End If
'
'    TheoPrice = BAD_DOUBLE_VALUE
'    Vega = BAD_DOUBLE_VALUE
'    Delta = BAD_DOUBLE_VALUE
'    Gamma = BAD_DOUBLE_VALUE
'    Theta = BAD_DOUBLE_VALUE
'    Rho = BAD_DOUBLE_VALUE
'    VegaDelta = BAD_DOUBLE_VALUE
'    VegaGamma = BAD_DOUBLE_VALUE
'    ThetaDelta = BAD_DOUBLE_VALUE
'    ThetaGamma = BAD_DOUBLE_VALUE
'    GammaPerTheta = BAD_DOUBLE_VALUE
'End Sub
'
