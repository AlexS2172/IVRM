VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsFutureRootAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public State As CollectionElementState

Public RootID As Long
Public Symbol As String
Public Name As String
Public ExportSymbol As String
Public FutureLotSize As Long
Public OptionLotSize As Long
Public HasFutures As Boolean
Public tiQuoteFormat As Long
Public iQuotationUnitAmmount As Long
Public iQuotationUnitID As Long
Public fQuoteDenomination As Double
Public fTickSize As Double
Public fTickValue As Double
Public vcQuotationUnitName As String


Private Sub Class_Initialize()
    On Error Resume Next
    State = ELEM_UNCHANGED
    HasFutures = False
End Sub
