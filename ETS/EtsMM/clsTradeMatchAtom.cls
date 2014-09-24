VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeMatchAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public TrdActual As EtsMmGeneralLib.MmTradeInfoAtom
Public TrdManual As New EtsMmGeneralLib.MmTradeInfoColl
Public TrdMatch As EtsMmGeneralLib.MmTradeInfoAtom
Public Updated As Boolean

Private Sub Class_Initialize()
    On Error Resume Next
    Updated = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set TrdActual = Nothing
    Set TrdManual = Nothing
    Set TrdMatch = Nothing
End Sub
