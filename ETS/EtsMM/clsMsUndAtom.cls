VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMsUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ID As Long
Public UndType As Long
Public Symbol As String
Public Description As String
    
Public IsAmerican As Boolean
Public IsHTB As Boolean
Public ExpCalendarID As Long

Public Yield As Double
Public Dividend As EtsGeneralLib.EtsIndexDivAtom

Public PriceBid As Double
Public PriceAsk As Double
Public PriceOpen As Double
Public PriceClose As Double
Public PriceLow As Double
Public PriceHigh As Double
Public Volume As Long

Public LotSize As Long

Public IsTraderContract As Boolean

Public Skew As Double
Public Kurt As Double

Public IdxComp As New EtsGeneralLib.IndexCompColl

Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom


