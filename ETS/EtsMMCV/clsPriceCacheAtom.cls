VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsPriceCacheAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ContractID As Long
Public BidPrice As Double
Public AskPrice As Double
Public ClosePrice As Double
Public TheoClosePrice As Double
Public LotSize As Long

Public RefCount As Long

Private Sub Class_Initialize()
    BidPrice = BAD_DOUBLE_VALUE
    AskPrice = BAD_DOUBLE_VALUE
    ClosePrice = BAD_DOUBLE_VALUE
    TheoClosePrice = BAD_DOUBLE_VALUE
End Sub
