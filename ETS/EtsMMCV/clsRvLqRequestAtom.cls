VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvLqRequestAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Und As clsRvUndAtom
Public Pos As clsRvPosAtom
Public Fut As clsRvFutAtom
Public IndexOnly As Boolean

Public Function GetQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error Resume Next
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams

    aParam.Exchange = ""
    
    If Not Pos Is Nothing Then
        aParam.Type = ContractTypeToPpInstrumentType(Pos.ContractType)
        aParam.Symbol = Pos.Symbol
        
    ElseIf Not Fut Is Nothing Then
        aParam.Type = ContractTypeToPpInstrumentType(enCtFuture)
        aParam.Symbol = Fut.Symbol
    
    Else
        aParam.Type = ContractTypeToPpInstrumentType(Und.ContractType)
        aParam.Symbol = Und.Symbol
    End If

    GetQuoteUpdateParam = aParam
End Function

Public Function GetGroupQuoteUpdateParam() As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error Resume Next
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams

    aParam.Exchange = ""
    aParam.Symbol = Und.Symbol
    aParam.Type = IIf(Und.ContractType = enCtStock, enGrSTK, enGrIDX)

    GetGroupQuoteUpdateParam = aParam
End Function

Private Sub Class_Initialize()
    On Error Resume Next
    IndexOnly = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set Und = Nothing
    Set Pos = Nothing
End Sub


