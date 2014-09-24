VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsSyntParAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public UnderlyingID     As Long
Public Weight           As Double
Public StrikeSettlement As Double

'for Greeks Calc
Public Symbol As String
Public UndPrice As Double
Public OptDelta As Double
Public OTMDelta As Double
Public TotalGamma As Double

'for Risk Matrix Calc
Public STD As Double
Public HV30d As Double

Private Sub Class_Initialize()
    On Error Resume Next
    UndPrice = 0#
    OptDelta = 0#
    OTMDelta = 0#
    TotalGamma = 0#
    STD = 0#
    HV30d = 0#
End Sub
