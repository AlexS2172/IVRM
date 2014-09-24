VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsOptionPairAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public iCallID As Variant
Public sCallSymbol As String
Public bCallManual As Boolean
Public fCallClose As Double
Public fCallTheo As Double
Public sCallImportSymbol As String
Public fStrike As Double
Public dExpiry As Date
Public dExpiryOV As Date
Public dTradingClose As Date
Public iPutID As Variant
Public bPutManual As Boolean
Public sPutSymbol As String
Public fPutClose As Double
Public fPutTheo As Double
Public sPutImportSymbol As String
Public iRootID As Long

Private Sub Class_Initialize()
    On Error Resume Next
    iCallID = Null
    iPutID = Null
End Sub
