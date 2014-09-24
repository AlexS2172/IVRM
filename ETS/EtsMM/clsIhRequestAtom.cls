VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsIhRequestAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Idx As EtsMmIndexHedgeLib.MmIhIndexAtom
'Public Opt As EtsMmIndexHedgeLib.MmIhOptAtom
'
'' Used only in 'Index Hedge' view
'Public Und As EtsMmIndexHedgeLib.MmIhUndAtom
'
'' Used only in 'Two Indices Hedge' view
'Private m_Und2(1 To 2) As EtsMmIndexHedgeLib.MmIhUndAtom ' NOTE: 1st and 2nd must be the same
'
'Public Function GetQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
'    On Error Resume Next
'    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
'
'    aParam.Exchange = ""
'
'    If Not Opt Is Nothing Then
'        aParam.Type = enOPT
'        aParam.Symbol = Opt.Symbol
'
'    ElseIf Not Und Is Nothing Then
'        aParam.Type = enSTK
'        aParam.Symbol = Und.Symbol
'
'    ElseIf Not Und2(1) Is Nothing Then
'        aParam.Type = enSTK
'        aParam.Symbol = m_Und2(1).Symbol
'
'    ElseIf Not Und2(2) Is Nothing Then
'        aParam.Type = enSTK
'        aParam.Symbol = m_Und2(2).Symbol
'
'    Else
'        aParam.Type = enIDX
'        aParam.Symbol = Idx.Symbol
'    End If
'
'    GetQuoteUpdateParam = aParam
'End Function
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    Set Und = Nothing
'    Set Opt = Nothing
'    Set Idx = Nothing
'    Set Und2(1) = Nothing
'    Set Und2(2) = Nothing
'End Sub
'
'Public Property Get Und2(nIdx&) As EtsMmIndexHedgeLib.MmIhUndAtom
'    Set Und2 = m_Und2(nIdx)
'End Property
'
'Public Property Set Und2(nIdx&, aUnd As EtsMmIndexHedgeLib.MmIhUndAtom)
'    Set m_Und2(nIdx) = aUnd
'End Property
'
'
