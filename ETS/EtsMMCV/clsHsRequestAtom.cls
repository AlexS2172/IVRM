VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsHsRequestAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Und As EtsMmHedgeLib.MmHsUndAtom
Public Opt As EtsMmHedgeLib.MmHsOptAtom

Public Function GetQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error Resume Next
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams

    aParam.Exchange = ""
    
    If Not Opt Is Nothing Then
        aParam.Type = enOPT
        aParam.Symbol = Opt.Symbol
    Else
        aParam.Type = IIf(Und.UndType = enCtStock, enSTK, enIDX)
        aParam.Symbol = Und.Symbol
    End If

    GetQuoteUpdateParam = aParam
End Function

Private Sub Class_Terminate()
    On Error Resume Next
    Set Und = Nothing
    Set Opt = Nothing
End Sub

