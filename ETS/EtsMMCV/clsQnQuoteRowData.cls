VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQnQuoteRowData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Und As clsQnUndAtom
'Public UndQuote As clsQnQuoteAtom
'Public Exp As clsQnExpAtom
'Public Exch As EtsGeneralLib.ExchAtom
'Private m_Opt(enOtPut To enOtCall) As clsQnOptAtom
'Private m_OptQuote(enOtPut To enOtCall) As clsQnQuoteAtom
'
'Public Property Get Opt(OptType As Long) As clsQnOptAtom
'    Set Opt = m_Opt(OptType)
'End Property
'
'Public Property Set Opt(OptType As Long, aOpt As clsQnOptAtom)
'    Set m_Opt(OptType) = aOpt
'End Property
'
'Public Property Get OptQuote(OptType As Long) As clsQnQuoteAtom
'    Set OptQuote = m_OptQuote(OptType)
'End Property
'
'Public Property Set OptQuote(OptType As Long, aQuote As clsQnQuoteAtom)
'    Set m_OptQuote(OptType) = aQuote
'End Property
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    Set m_OptQuote(enOtPut) = Nothing
'    Set m_OptQuote(enOtCall) = Nothing
'    Set m_Opt(enOtPut) = Nothing
'    Set m_Opt(enOtCall) = Nothing
'End Sub
'
