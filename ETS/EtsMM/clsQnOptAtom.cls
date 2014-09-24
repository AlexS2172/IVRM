VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQnOptAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public ID As Long
'Public Symbol As String
'Public Expiry As Date
'Public OptType As Long
'Public Strike As Double
'Public Vola As Double
'Public Exp As clsQnExpAtom
'
'Public Qty As Long
'Public QtyInShares As Long
'
'Public Underlying As clsQnUndAtom
'Public Quotes As New clsQnQuoteColl
'
'Public DefQuote As clsQnQuoteAtom
'
'Private Sub Class_Initialize()
'    QtyInShares = BAD_LONG_VALUE
'    Qty = BAD_LONG_VALUE
'End Sub
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    Set DefQuote = Nothing
'End Sub
'
''Public Property Get Key() As String
''    On Error Resume Next
''    Key = UCase$(Format$(Expiry, "YYMMM")) & Trim$(Str$(Int(Strike))) & IIf(OptType = enOtCall, "C", "P")
''End Property
''
'
