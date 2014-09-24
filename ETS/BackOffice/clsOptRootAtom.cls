VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsOptRootAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public IsChanged As Boolean

Public RootID As Long
Public UnderlyingID As Long
Public Symbol As String
Public IsSynthetic As Boolean
Public IsFittingEnabled As Boolean

Public LotSize As Long
Public Cash As Double

' for asign all to options to different root
Public NewRootID As Long
Public CanMove As Boolean

Public SynthParams As New clsSyntParColl

' for Greeks and Theo Price Calculation
Public synthPrice As Double

Public synthSkew As Double
Public synthKurt As Double
Public IsBasket As Boolean
Public synthYield As Double

Public synthDividends As New clsDividendsArr

Public SyntheticIndexBetas As New clsIndexBetaColl

Public VolaSurfaceID As Long
Public VolaSurfaceGroupID As Long

'for PoP calculation
Public synthPriceBid As Double
Public synthPriceAsk As Double

Private Sub Class_Initialize()
    On Error Resume Next
    NewRootID = 0
    CanMove = True
End Sub
