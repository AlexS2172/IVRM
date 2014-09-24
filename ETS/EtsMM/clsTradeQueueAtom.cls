VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeQueueAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public TrdOld As EtsGeneralLib.MmTradeInfoAtom
Public TrdNew As EtsGeneralLib.MmTradeInfoAtom
Public enTradeAction As TradeActionEnum

Private Sub Class_Initialize()
    On Error Resume Next

End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set TrdOld = Nothing
    Set TrdNew = Nothing
End Sub


