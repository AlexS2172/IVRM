VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsQnUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'Option Explicit
'
'Public ID As Long
'Public Symbol As String
'Public UndType As Long
'
'Public Yield As Double
'Public DivFreq As Long
'Public DivDate As Date
'Public DivAmt As Double
'
'Public IsHTB As Boolean
'
'Public UseCustDivs As Boolean
'Public DivFreqCust As Long
'Public DivDateCust As Date
'Public DivAmtCust As Double
'
'Public UseCustRates As Double
'
'Public QtyInShares As Long
'Public Pos As Double
'
'Public VolaSrv As EtsGeneralLib.VolaControl
'
'Public Expiry As New clsQnExpColl
'Public Quotes As New clsQnQuoteColl
'
'Public NetDelta As Double
'Public TotalDelta As Double
'Public TotalGamma As Double
'Public TotalVega As Double
'Public TotalTheta As Double
'Public TotalRho As Double
'
''Public CalcModelType As EtsGeneralLib.EtsCalcModelTypeEnum
'Public IsAmerican As Boolean
'Public ExpCalendarID As Long
'
'Public BasketIndex As EtsGeneralLib.IndexAtom
'Public IsTraderContract As Boolean
'
'Public Skew As Double
'Public Kurt As Double
'
'Public UseMidRates As Boolean
'
'Public BinNum As Long
'Public TraderRole As ISEPROVIDERLib.TraderRoleEnum
'Public OptionsCount As Long
'
'Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
'Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
'
'Private Sub Class_Initialize()
'    QtyInShares = BAD_LONG_VALUE
'    Pos = BAD_DOUBLE_VALUE
'
'    NetDelta = BAD_DOUBLE_VALUE
'    TotalDelta = BAD_DOUBLE_VALUE
'    TotalGamma = BAD_DOUBLE_VALUE
'    TotalVega = BAD_DOUBLE_VALUE
'    TotalTheta = BAD_DOUBLE_VALUE
'    TotalRho = BAD_DOUBLE_VALUE
'
'    OptionsCount = 0
'
'    On Error Resume Next
'    Set VolaSrv = New EtsGeneralLib.VolaControl
'    If Err.Number <> 0 Then LogEvent EVENT_ERROR, "Fail to create volatility control: " & Err.Description
'End Sub
'
'Private Sub Class_Terminate()
'    On Error Resume Next
'    Set BasketIndex = Nothing
'    Set VolaSrv = Nothing
'    Set UndPriceProfile = Nothing
'    Set OptPriceProfile = Nothing
'End Sub
'
'Public Property Get TraderRoleName() As String
'    On Error Resume Next
'    TraderRoleName = ""
'    Select Case TraderRole
'        Case enTrPMM
'            TraderRoleName = "PMM"
'        Case enTrEAM
'            TraderRoleName = "EAM"
'        Case enTrCMM
'            TraderRoleName = "CMM"
'    End Select
'End Property
'
