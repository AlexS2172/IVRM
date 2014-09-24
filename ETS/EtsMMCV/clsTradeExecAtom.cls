VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeExecAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public QtyToExec As Long
Public UndPrice As Double
Public UndTradePrice As Double
Public IsExec As Boolean
Public Trd As EtsGeneralLib.MmTradeInfoAtom

Private Sub Class_Initialize()
    On Error Resume Next
    IsExec = True
    QtyToExec = 0&
    UndPrice = 0#
    UndTradePrice = 0#
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set Trd = Nothing
End Sub
