VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQvRequestAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Exch As EtsGeneralLib.ExchAtom
Public Und As EtsMmQuotesLib.MmQvUndAtom
Public Opt As EtsMmQuotesLib.MmQvOptAtom
Public Exp As EtsMmQuotesLib.MmQvExpAtom
Public Fut As EtsMmQuotesLib.MmQvFutAtom
Public FutRoot As EtsMmQuotesLib.MmQvFutRootAtom

Public Function GetQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error Resume Next
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams

    aParam.Exchange = Exch.Code
    
    If Fut Is Nothing Then
        If Not Opt Is Nothing Then
            aParam.Type = enOPT
            aParam.Symbol = Opt.Symbol
        Else
            aParam.Type = IIf(Und.UndType = enCtStock, enSTK, enIDX)
            aParam.Symbol = Und.Symbol
        End If
    Else
        If Not Opt Is Nothing Then
            aParam.Type = enFOP
            aParam.Symbol = Opt.Symbol
        Else
            aParam.Type = enFUT
            aParam.Symbol = Fut.Symbol
        End If
    End If

    GetQuoteUpdateParam = aParam
End Function

Private Sub Class_Terminate()
    On Error Resume Next
    Set Exch = Nothing
    Set Und = Nothing
    Set Opt = Nothing
    Set Exp = Nothing
    Set Fut = Nothing
    Set FutRoot = Nothing
End Sub

