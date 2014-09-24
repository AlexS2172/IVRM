VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvFutAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ID As Long
Public Symbol As String
Public Maturity As Date
Public IsAmerican As Boolean
Public ExpCalendarID As Long
Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom

Public FutRootID As Long
Public FutRootSymbol As String

Public UndID As Long

Public CalcGreeks As Boolean

Public PriceBid As Double
Public PriceAsk As Double
Public PriceLast As Double
Public PriceClose As Double

Public OptLotSize As Long
Public FutLotSize As Long

Public ContractName As String
Public ReplacePriceStatus As EtsGeneralLib.EtsReplacePriceStatusEnum

Private Sub Class_Initialize()
    On Error Resume Next
    PriceBid = BAD_DOUBLE_VALUE
    PriceAsk = BAD_DOUBLE_VALUE
    PriceLast = BAD_DOUBLE_VALUE
    PriceClose = BAD_DOUBLE_VALUE
    
    FutLotSize = BAD_LONG_VALUE
    OptLotSize = BAD_LONG_VALUE

    IsAmerican = False
    
    CalcGreeks = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set UndPriceProfile = Nothing
    Set OptPriceProfile = Nothing
End Sub
