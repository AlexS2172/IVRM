VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsOvRowData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Ord As EtsMmGeneralLib.MmOrderAtom
Public Exec As EtsMmGeneralLib.MmOrderExecAtom
Public Trade As EtsGeneralLib.MmTradeInfoAtom

Private Sub Class_Terminate()
    On Error Resume Next
    Set Ord = Nothing
    Set Exec = Nothing
End Sub
