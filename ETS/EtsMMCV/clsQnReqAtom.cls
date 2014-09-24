VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQnReqAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public Key As String
'Public Exch As EtsGeneralLib.ExchAtom
'Public Und As clsQnUndAtom
'Public Opt As clsQnOptAtom
'
'Public Function GetQuoteUpdateParam() As ISEPROVIDERLib.QuoteUpdateParams
'    On Error Resume Next
'    Dim aParam As ISEPROVIDERLib.QuoteUpdateParams
'
'    aParam.Exchange = ""
'    If Not Exch Is Nothing Then aParam.Exchange = Exch.Code
'
'    If Not Opt Is Nothing Then
'        aParam.Type = enOPT
'        aParam.Symbol = Opt.Symbol
'    Else
'        aParam.Type = IIf(Und.UndType = 2, enSTK, enIDX)
'        aParam.Symbol = Und.Symbol
'    End If
'
'    GetQuoteUpdateParam = aParam
'End Function
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    Set Exch = Nothing
'    Set Und = Nothing
'    Set Opt = Nothing
'End Sub
'
'
