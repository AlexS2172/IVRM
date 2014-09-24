VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvGrpAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public ID As Long
Public GroupType As FilterTypeEnum
Public Name As String
    
Public PnlMtm As Double
Public PnlTheo As Double
Public Vega As Double
Public WtdVega As Double
Public Theta As Double
Public DeltaEq As Double
Public GammaEq As Double
Public Rho As Double
Public IdxDeltaEq As Double
Public BetaWtdDelta As Double
Public OptDelta As Double
Public NetDelta As Double
Public NetGamma As Double
Public BetaWtdDeltaEq As Double

Public BadPnlMtm As Boolean
Public BadPnlTheo As Boolean
Public BadVega As Boolean
Public BadWtdVega As Boolean
Public BadTheta As Boolean
Public BadDeltaEq As Boolean
Public BadGammaEq As Boolean
Public BadRho As Boolean
Public BadIdxDeltaEq As Boolean
Public BadBetaWtdDelta As Boolean
Public BadOptDelta As Boolean
Public BadNetDelta As Boolean
Public BadNetGamma As Boolean
Public BadBetaWtdDeltaEq As Boolean

Public Sub Clear()
    ID = 0
    GroupType = 0
    Name = ""
       
    ClearValues
End Sub

Public Sub ClearValues()
    PnlMtm = BAD_DOUBLE_VALUE
    PnlTheo = BAD_DOUBLE_VALUE
    Vega = BAD_DOUBLE_VALUE
    WtdVega = BAD_DOUBLE_VALUE
    Theta = BAD_DOUBLE_VALUE
    DeltaEq = BAD_DOUBLE_VALUE
    GammaEq = BAD_DOUBLE_VALUE
    Rho = BAD_DOUBLE_VALUE
    IdxDeltaEq = BAD_DOUBLE_VALUE
    BetaWtdDelta = BAD_DOUBLE_VALUE
    OptDelta = BAD_DOUBLE_VALUE
    NetDelta = BAD_DOUBLE_VALUE
    NetGamma = BAD_DOUBLE_VALUE
    BetaWtdDeltaEq = BAD_DOUBLE_VALUE

    BadPnlMtm = False
    BadPnlTheo = False
    BadVega = False
    BadWtdVega = False
    BadTheta = False
    BadDeltaEq = False
    BadGammaEq = False
    BadRho = False
    BadIdxDeltaEq = False
    BadBetaWtdDelta = False
    BadOptDelta = False
    BadNetDelta = False
    BadNetGamma = False
    BadBetaWtdDeltaEq = False
End Sub

'Public Sub SetAllValuesAsBad()
'    BadPnlMtm = True
'    BadPnlTheo = True
'    BadVega = True
'    BadWtdVega = True
'    BadTheta = True
'    BadDeltaEq = True
'    BadGammaEq = True
'    BadRho = True
'    BadIdxDeltaEq = True
'    BadBetaWtdDelta = True
'    BadOptDelta = True
'End Sub

Private Sub Class_Initialize()
    On Error Resume Next
    Clear
End Sub
