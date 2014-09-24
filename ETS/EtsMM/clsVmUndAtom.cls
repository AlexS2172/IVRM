VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVmUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ID As Long
Public Symbol As String
Public UndType As Long
    
Public LotSize As Long
    
Public Yield As Double
Public DivFreq As Long
Public DivDate As Date
Public DivAmt As Double

Public IsHTB As Boolean

Public Dividend As EtsGeneralLib.EtsIndexDivAtom

Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double

Public QtyInShares As Long
Public Pos As Double
Public UndPosForRates As Double

Public VolaSrv As EtsGeneralLib.VolaControl

Public Expiry As New clsVmExpiryColl

Public CurExpiry1 As clsVmExpiryAtom
Public CurExpiry2 As clsVmExpiryAtom

Public IV1 As Double
Public IV2 As Double
Public Vola1 As Double
Public Vola2 As Double
Public TargetVola1 As Double
Public TargetVola2 As Double

Public NetDelta As Double
Public TotalDelta As Double
Public TotalGamma As Double
Public TotalVega As Double
Public TotalTheta As Double
Public TotalRho As Double

Public IsAmerican As Boolean
Public ExpCalendarID As Long

Public BasketIndex As EtsGeneralLib.IndexAtom

Public Skew As Double
Public Kurt As Double

Public UseMidRates As Boolean
Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public ReplacePriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum

Private Sub Class_Initialize()
    QtyInShares = BAD_LONG_VALUE
    Pos = BAD_DOUBLE_VALUE
    
    PriceBid = BAD_DOUBLE_VALUE
    PriceAsk = BAD_DOUBLE_VALUE
    PriceLast = BAD_DOUBLE_VALUE
    
    UndPosForRates = 0
    
    ReplacePriceStatus = enRpsNone
    
    ClearVolas
    ClearTheos
    
    On Error Resume Next
    Set VolaSrv = New EtsGeneralLib.VolaControl
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to create volatility control: " & Err.Description
End Sub

Public Sub ClearVolas()
    IV1 = BAD_DOUBLE_VALUE
    IV2 = BAD_DOUBLE_VALUE
    Vola1 = BAD_DOUBLE_VALUE
    Vola2 = BAD_DOUBLE_VALUE
    TargetVola1 = BAD_DOUBLE_VALUE
    TargetVola2 = BAD_DOUBLE_VALUE
End Sub

Public Sub ClearTheos()
    NetDelta = BAD_DOUBLE_VALUE
    TotalDelta = BAD_DOUBLE_VALUE
    TotalGamma = BAD_DOUBLE_VALUE
    TotalVega = BAD_DOUBLE_VALUE
    TotalTheta = BAD_DOUBLE_VALUE
    TotalRho = BAD_DOUBLE_VALUE
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set BasketIndex = Nothing
    Set VolaSrv = Nothing
    Set UndPriceProfile = Nothing
    Set OptPriceProfile = Nothing
End Sub
