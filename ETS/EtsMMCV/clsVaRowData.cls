VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVaRowData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Und As EtsMmVolaAnalysisLib.MmVaUndAtom
'Public Exp As EtsMmVolaAnalysisLib.MmVaExpAtom
'Public Exch As EtsGeneralLib.ExchAtom
'Public OptRoot As EtsMmVolaAnalysisLib.MmVaOptRootAtom
'Public OptPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom

Private Sub Class_Terminate()
    On Error Resume Next
    Set Und = Nothing
'    Set Exp = Nothing
'    Set Exch = Nothing
'    Set OptPair = Nothing
'    Set OptRoot = Nothing
End Sub

