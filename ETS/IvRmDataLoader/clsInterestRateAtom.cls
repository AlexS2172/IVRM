VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsInterestRateAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Period As Long
Public BaseUSDRatePrc As Double
Public LongUSDSpread As Double
Public ShortUSDSpread As Double

Public Sub Clear()
    Period = BAD_LONG_VALUE
    BaseUSDRatePrc = BAD_DOUBLE_VALUE
    LongUSDSpread = BAD_DOUBLE_VALUE
    ShortUSDSpread = BAD_DOUBLE_VALUE
End Sub
