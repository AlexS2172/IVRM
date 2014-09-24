VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsVolaSrv"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private Const DEF_VOLA As Double = 0#

Private vmVola As VMELib.SymbolVolatility
Private vvv As VMELib.VolatilitySource

Private m_Symbol As String
Private m_Price As Double

Private Sub Class_Initialize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_Price = 0#
End Sub

Public Sub Init(sSymbol As String, aType As ContractTypes, VolaSource As VMELib.VolatilitySource)
    On Error GoTo EH
    
    CleanUp
    m_Symbol = sSymbol
    Set vmVola = VolaSource.SymbolVolatility(m_Symbol, IIf(aType = ContractTypes.CONTRACT_TYPE_STOCK, MSGSTRUCTLib.enStStock, MSGSTRUCTLib.enStIndex))
    
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & m_Symbol & "': " & Err.Description
End Sub

Public Property Let Price(NewValue As Double)
    On Error Resume Next
    m_Price = NewValue
End Property

Public Property Get Price() As Double
    On Error Resume Next
    Price = m_Price
End Property

Public Property Get Vola(Expiry As Date, Strike As Double) As Double
    On Error GoTo EH
    
    If Not vmVola Is Nothing Then
        Vola = vmVola.Volatility(m_Price, Strike, DateSerial(Year(Expiry), Month(Expiry), 1)) / 100#
        If Vola < 0 Then
            Vola = DEF_VOLA
        End If
    Else
        Vola = DEF_VOLA
    End If
    Exit Property
EH:
    gCmn.ErrorHandler "Fail to get volatility."
End Property

Public Sub CleanUp()
    On Error Resume Next
    m_Symbol = ""
    m_Price = 0#
    Set vmVola = Nothing
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    CleanUp
End Sub
