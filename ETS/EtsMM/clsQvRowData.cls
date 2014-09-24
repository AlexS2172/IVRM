VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQvRowData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Und As EtsMmQuotesLib.MmQvUndAtom
Public UndQuote As EtsMmQuotesLib.MmQvQuoteAtom
Public Exp As EtsMmQuotesLib.MmQvExpAtom
Public Strike As EtsMmQuotesLib.MmQvStrikeAtom
Public Exch As EtsGeneralLib.ExchAtom
Public OptRoot As EtsMmQuotesLib.MmQvOptRootAtom
Private m_Opt(enOtPut To enOtCall) As EtsMmQuotesLib.MmQvOptAtom
Private m_OptQuote(enOtPut To enOtCall) As EtsMmQuotesLib.MmQvQuoteAtom
Public Fut As EtsMmQuotesLib.MmQvFutAtom
Public FutRoot As EtsMmQuotesLib.MmQvFutRootAtom

Public Property Get Opt(OptType As Long) As EtsMmQuotesLib.MmQvOptAtom
    Set Opt = m_Opt(OptType)
End Property

Public Property Set Opt(OptType As Long, aOpt As EtsMmQuotesLib.MmQvOptAtom)
    Set m_Opt(OptType) = aOpt
End Property

Public Property Get OptQuote(OptType As Long) As EtsMmQuotesLib.MmQvQuoteAtom
    Set OptQuote = m_OptQuote(OptType)
End Property

Public Property Set OptQuote(OptType As Long, aQuote As EtsMmQuotesLib.MmQvQuoteAtom)
    Set m_OptQuote(OptType) = aQuote
End Property

Private Sub Class_Terminate()
    On Error Resume Next
    Set Und = Nothing
    Set UndQuote = Nothing
    Set Exp = Nothing
    Set Exch = Nothing
    Set OptRoot = Nothing
    Set Strike = Nothing
    Set m_OptQuote(enOtPut) = Nothing
    Set m_OptQuote(enOtCall) = Nothing
    Set m_Opt(enOtPut) = Nothing
    Set m_Opt(enOtCall) = Nothing
    Set Fut = Nothing
    Set FutRoot = Nothing
End Sub
