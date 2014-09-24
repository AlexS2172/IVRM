VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsGvRequestAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Und As EtsMmRisksLib.MmRvUndAtom
Public Pos As EtsMmRisksLib.MmRvPosAtom

Public Function GetQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error Resume Next
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams

    aParam.Exchange = ""
    
    If Not Pos Is Nothing Then
        aParam.Type = IIf(Pos.ContractType = enCtOption, enOPT, IIf(Pos.ContractType = enCtStock, enSTK, enIDX))
        aParam.Symbol = Pos.Symbol
    Else
        aParam.Type = IIf(Und.ContractType = enCtStock, enSTK, enIDX)
        aParam.Symbol = Und.Symbol
    End If

    GetQuoteUpdateParam = aParam
End Function

Private Sub Class_Terminate()
    On Error Resume Next
    Set Und = Nothing
    Set Pos = Nothing
End Sub


