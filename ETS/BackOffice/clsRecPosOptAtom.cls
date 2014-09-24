VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRecPosOptAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
' option atom for position reconciliation
Option Explicit

Public Id As Long
Public Symbol As String
Public SymbolDisplay As String
Public LotSize As Long

Public IsCall As Byte
Public Strike As Double
Public Expiry As Date

Public OptionRootSymbol As String
Public OptionRootID As Long

Public ExportSymbol As String
Public IsSelectInTodayReconcile As Boolean

Private Sub Class_Initialize()
    On Error Resume Next
    IsSelectInTodayReconcile = False
End Sub
