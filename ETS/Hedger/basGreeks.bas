Attribute VB_Name = "basGreeks"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Enum ContractTypes
    CONTRACT_TYPE_INDEX = 1
    CONTRACT_TYPE_STOCK = 2
    CONTRACT_TYPE_OPTION = 3
End Enum
 
Public Const MM_EGAR_BS              As Long = &H1&
Public Const MM_EGAR_BINOMIAL        As Long = &H2&
Public Const MM_EGAR_OPTIMIZED       As Long = &H3&

Public Enum ETS_CALCMODEL
    ETS_BINOMIAL = MM_EGAR_BINOMIAL
    ETS_BINOMIAL_OPT = MM_EGAR_OPTIMIZED
End Enum

Public g_CalcModel As ETS_CALCMODEL

Public m_Symbol As String
Public m_Type As ContractTypes
Public m_Yield As Double
Public m_Price As Double
Public m_DivFreq As Long
Public m_DivAmt As Double
Public m_DivDate As Date
Public m_IsHTB As Boolean
Public m_StockPosition As Double

Public m_VolaSrv As New clsVolaSrv
Public m_Greeks As GreeksData

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetPrice(ByVal Bid As Variant, ByVal Ask As Variant, ByVal Last As Variant) As Variant
    On Error Resume Next
    GetPrice = PriceMid(DBRead(Ask, 0), DBRead(Bid, 0))
    If GetPrice = 0 Then
        GetPrice = Last
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function PriceMid(ByVal fPriceBid As Double, ByVal fPriceAsk As Double) As Double
    If fPriceBid = 0# Or fPriceAsk = 0# Then
        PriceMid = fPriceBid + fPriceAsk
    Else
        PriceMid = (fPriceBid + fPriceAsk) / 2
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NormalExpiry(ByVal Expiry As Date) As Date
    NormalExpiry = DateSerial(Year(Expiry), Month(Expiry), 1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockData( _
    ByVal Symbol As String, _
    ByVal pType As ContractTypes, _
    ByVal Price As Double, _
    ByVal DivFreq As Long, _
    ByVal DivAmt As Double, _
    ByVal DivDate As Date, _
    ByVal IsHTB As Boolean, _
    ByVal StockPosition As Double _
)
    
    m_Symbol = Symbol
    m_Price = Price
    m_Type = pType
    
    If m_Type = CONTRACT_TYPE_STOCK Then
        m_DivFreq = DivFreq
        m_DivAmt = DivAmt
        m_DivDate = DivDate
        m_Yield = 0
    Else
        m_DivFreq = 0
        m_DivAmt = 0
        m_DivDate = 0
        m_Yield = DivAmt
    End If
    
    Set m_VolaSrv = New clsVolaSrv
    m_VolaSrv.Init m_Symbol, m_Type, g_VolaSource
    m_VolaSrv.Price = m_Price
    
    m_IsHTB = IsHTB
    m_StockPosition = StockPosition * Price
    
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockPrice( _
    ByVal Price As Double _
)
    m_Price = Price
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGreeks( _
    ByVal Strike As Double, _
    ByVal Expiry As Date, _
    ByVal IsCall As Boolean _
) As Boolean

Dim R As Long
Dim dDivDte() As Double
Dim dDivAmts() As Double
Dim nDivCount As Long
Dim Dte As Long
Dim dRate As Double
Dim dYield As Double
Dim IV As Double
    
    Dte = DateDiff("d", Date, Expiry)
    If Dte < 0 Then Dte = 0
    'dRate = GetRate(Date, Expiry, m_IsHTB, m_StockPosition)
    dRate = GetRate(Date, Expiry, m_IsHTB, 0)
    
    nDivCount = 0
    ReDim dDivDte(0 To 0)
    ReDim dDivAmts(0 To 0)
    If m_Type = CONTRACT_TYPE_STOCK Then
        dYield = 0
        If m_DivFreq > 0 Then
            nDivCount = GetDividendsCount(Date, Dte, m_DivDate, m_DivFreq)
            If nDivCount > 0 Then
                ReDim dDivDte(0 To nDivCount - 1)
                ReDim dDivAmts(0 To nDivCount - 1)
                GetDividends2 Date, Dte, m_DivDate, m_DivFreq, m_DivAmt, nDivCount, dDivAmts(0), dDivDte(0), R
            End If
        End If
    Else
        dYield = m_Yield
    End If
    
    ' !!! Get IV !!!
    If Not m_VolaSrv Is Nothing Then
        IV = m_VolaSrv.Vola(Expiry, Strike)
    Else
        IV = 0
        LogEvent EVENT_ERROR, "Volatility surface object is not available. Symbol '" & m_Symbol & "'"
        Debug.Assert False
    End If
    If m_Price = 0 Then
        LogEvent EVENT_ERROR, "Greeks calculation failed. Zero spot price for symbol '" & m_Symbol & "'"
        Exit Function
    End If
    If IV = 0 Then
        LogEvent EVENT_WARNING, "Greeks calculation used default IV=30% instead of zero IV for option '" & m_Symbol & "' " & Format$(Expiry, "mmmyy") & " strike " & Format$(Strike)
        IV = 0.3
    End If
    If IV > 2.5 Then
        LogEvent EVENT_WARNING, "Greeks calculation used max IV=250% instead of too high IV for option '" & m_Symbol & "' " & Format$(Expiry, "mmmyy") & " strike " & Format$(Strike)
        IV = 2.5
    End If
    
    If m_Price <> 0 Then
        If CalcGreeksMM( _
            dRate, dYield, _
            m_Price, Strike, IV, Dte, _
            IIf(IsCall, 1, 0), 1, _
            nDivCount, dDivAmts(0), dDivDte(0), _
            100, g_CalcModel, m_Greeks _
        ) Then
            GetGreeks = True
            Exit Function
        Else
            LogEvent EVENT_ERROR, "Greeks calculation failed for option '" & m_Symbol & "' " & Format$(Expiry, "mmmyy") & " strike " & Format$(Strike)
        End If
    End If

End Function

