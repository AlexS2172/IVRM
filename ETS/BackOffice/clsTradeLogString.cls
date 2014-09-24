VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsTradeLogString"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Description = "Trade log string class"
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"No"
Public m_sKey As String

Public m_nTradeID As Long
Public m_nErrorID As Long
Public m_dtExecutionDate As Date

Public Sub Init(nTradeID As Long, nErrorID As Long)
    On Error Resume Next
    m_nTradeID = nTradeID
    m_nErrorID = nErrorID
    m_dtExecutionDate = Now()
End Sub
