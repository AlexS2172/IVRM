VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAddExScreenRequest"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'local variable to hold collection
Private m_Col As New Collection

Public Sub AddRequest(ByRef aReq As PRICEPROVIDERSLib.QuoteUpdateParams)
    m_Col.Add aReq
End Sub

Public Function Count() As Integer
    Count = m_Col.Count
End Function

Public Property Get GetItem(vntIndexKey As Variant) As PRICEPROVIDERSLib.QuoteUpdateParams
    GetItem = m_Col(vntIndexKey)
End Property

Public Sub Clear()
    Set m_Col = Nothing
    Set m_Col = New Collection
End Sub

Public Function WasRequestAdded(ByRef aReq As PRICEPROVIDERSLib.QuoteUpdateParams) As Boolean
    WasRequestAdded = False
    Dim aReqLoop As PRICEPROVIDERSLib.QuoteUpdateParams, iIdx As Integer
    For iIdx = 1 To m_Col.Count
        aReqLoop = m_Col(iIdx)
        If aReqLoop.Symbol = aReq.Symbol And aReqLoop.Type = aReq.Type Then
            WasRequestAdded = True
            Exit For
        End If
    Next
End Function
