Attribute VB_Name = "basIRates"
Option Explicit

Public Type REGULAR_DIVIDENDS
    nLastDivDate As Long
    nFrequency As Long
    dAmount As Double
End Type

Public Declare Function InterpolateRates Lib "OptionCalc.dll" (ByVal nCount As Long, pRates As InterestRatePoint, ByVal nDTE As Long) As Double

Public Enum IrRuleType
    enAlwaysUseNeutralRate = 0
    enRateBasedOnPosition = 1
End Enum

Public Type InterestRatePoint
    Rate As Double
    Dte As Long
End Type

Public Type InterestRateCurve
    ID As Long
    Name As String
    RuleType As IrRuleType
    CurveType As Long
    PosThreshold As Currency
    PointsCount As Long
    
    ShortRate() As InterestRatePoint
    LongRate() As InterestRatePoint
    NeutralRate() As InterestRatePoint
    HTBRate() As InterestRatePoint
    NeutralHTBRate() As InterestRatePoint
End Type

Public g_IRs() As InterestRateCurve

Public Function GetIRateRuleType() As IrRuleType
    GetIRateRuleType = g_IRs(0).RuleType
End Function

Public Function GetIRatePosThreshold() As Long
    GetIRatePosThreshold = g_IRs(0).PosThreshold
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Buy  q > 0 Bid Long   Put
' Sell q < 0 Ask Short Call
'
Public Function GetRate(ByVal Moment As Date, ByVal Expiry As Date, ByVal IsHTB As Boolean, ByVal Position As Double) As Double
    'GetRate = 0.5 * (GetShortRate(Moment, Expiry) + GetLongRate(Moment, Expiry))
    If GetIRateRuleType = enAlwaysUseNeutralRate Then
        ' Neutral rate
        If IsHTB Then
            GetRate = GetNeutralHTBRate(Moment, Expiry)
        Else
            GetRate = GetNeutralRate(Moment, Expiry)
        End If
    Else
        ' Pos based
        If Abs(Position) > GetIRatePosThreshold Then
            ' Big positions
            If Position > 0 Then
                'Buy Bid Long
                GetRate = GetLongRate(Moment, Expiry)
            Else
                'Sell q < 0 Ask Short
                If IsHTB Then
                    GetRate = GetHTBRate(Moment, Expiry)
                Else
                    GetRate = GetShortRate(Moment, Expiry)
                End If
            End If
        Else
            ' Small positions
            If IsHTB Then
                GetRate = GetNeutralHTBRate(Moment, Expiry)
            Else
                GetRate = GetNeutralRate(Moment, Expiry)
            End If
        End If
    End If
End Function

Public Function GetShortRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    
    If g_IRs(0).PointsCount > 0 Then
        GetShortRate = InterpolateRates(g_IRs(0).PointsCount, g_IRs(0).ShortRate(0), dtDate - dtToday)
    Else
        GetShortRate = 0
    End If
End Function

Public Function GetLongRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    
    If g_IRs(0).PointsCount > 0 Then
        GetLongRate = InterpolateRates(g_IRs(0).PointsCount, g_IRs(0).LongRate(0), dtDate - dtToday)
    Else
        GetLongRate = 0
    End If
End Function

Public Function GetNeutralRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    
    If g_IRs(0).PointsCount > 0 Then
        GetNeutralRate = InterpolateRates(g_IRs(0).PointsCount, g_IRs(0).NeutralRate(0), dtDate - dtToday)
    Else
        GetNeutralRate = 0
    End If
End Function

Public Function GetHTBRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    
    If g_IRs(0).PointsCount > 0 Then
        GetHTBRate = InterpolateRates(g_IRs(0).PointsCount, g_IRs(0).HTBRate(0), dtDate - dtToday)
    Else
        GetHTBRate = 0
    End If
End Function

Public Function GetNeutralHTBRate(ByVal dtToday As Date, ByVal dtDate As Date) As Double
    On Error Resume Next
    
    If g_IRs(0).PointsCount > 0 Then
        GetNeutralHTBRate = InterpolateRates(g_IRs(0).PointsCount, g_IRs(0).NeutralHTBRate(0), dtDate - dtToday)
    Else
        GetNeutralHTBRate = 0
    End If
End Function

Public Sub LoadInterestRates()
    On Error GoTo EH
    Dim rsCurve As ADODB.Recordset
    Dim rsPoint As ADODB.Recordset
    Dim nCurveCount&, nDTE&
    
    ClearInterestRates
    nCurveCount = 0&
    
    Set rsCurve = gDBW.usp_IRCurve_Get
    While Not rsCurve.EOF
        If nCurveCount > 0 Then
            ReDim Preserve g_IRs(0 To nCurveCount)
        Else
            ReDim g_IRs(0 To nCurveCount)
        End If
        
        g_IRs(nCurveCount).ID = DBRead(rsCurve!iCurveID, 0)
        g_IRs(nCurveCount).Name = DBRead(rsCurve!vcCurveName, 0)
        g_IRs(nCurveCount).RuleType = IIf(DBRead(rsCurve!iRuleTypeID, 0) <> 0, enRateBasedOnPosition, enAlwaysUseNeutralRate)
        g_IRs(nCurveCount).CurveType = DBRead(rsCurve!iCurveTypeID, 0)
        g_IRs(nCurveCount).PosThreshold = DBRead(rsCurve!mPositionThreshold, 0)
        
        g_IRs(nCurveCount).PointsCount = 0&
        Set rsPoint = gDBW.usp_IRPoint_Get(g_IRs(nCurveCount).ID)
        While Not rsPoint.EOF
            If g_IRs(nCurveCount).PointsCount > 0 Then
                ReDim Preserve g_IRs(nCurveCount).ShortRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim Preserve g_IRs(nCurveCount).LongRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim Preserve g_IRs(nCurveCount).NeutralRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim Preserve g_IRs(nCurveCount).HTBRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim Preserve g_IRs(nCurveCount).NeutralHTBRate(0 To g_IRs(nCurveCount).PointsCount)
            Else
                ReDim g_IRs(nCurveCount).ShortRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim g_IRs(nCurveCount).LongRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim g_IRs(nCurveCount).NeutralRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim g_IRs(nCurveCount).HTBRate(0 To g_IRs(nCurveCount).PointsCount)
                ReDim g_IRs(nCurveCount).NeutralHTBRate(0 To g_IRs(nCurveCount).PointsCount)
            End If
            
            nDTE = GetDays(Date, DBRead(rsPoint!iNum, 0), DBRead(rsPoint!iPeriodTypeID, 0))
            
            g_IRs(nCurveCount).ShortRate(g_IRs(nCurveCount).PointsCount).Dte = nDTE
            g_IRs(nCurveCount).ShortRate(g_IRs(nCurveCount).PointsCount).Rate = DBRead(rsPoint!fShortRate, 0)
            
            g_IRs(nCurveCount).LongRate(g_IRs(nCurveCount).PointsCount).Dte = nDTE
            g_IRs(nCurveCount).LongRate(g_IRs(nCurveCount).PointsCount).Rate = DBRead(rsPoint!fLongRate, 0)
            
            g_IRs(nCurveCount).NeutralRate(g_IRs(nCurveCount).PointsCount).Dte = nDTE
            g_IRs(nCurveCount).NeutralRate(g_IRs(nCurveCount).PointsCount).Rate = (g_IRs(nCurveCount).ShortRate(g_IRs(nCurveCount).PointsCount).Rate + g_IRs(nCurveCount).LongRate(g_IRs(nCurveCount).PointsCount).Rate) / 2#
            
            g_IRs(nCurveCount).HTBRate(g_IRs(nCurveCount).PointsCount).Dte = nDTE
            g_IRs(nCurveCount).HTBRate(g_IRs(nCurveCount).PointsCount).Rate = DBRead(rsPoint!fHTBRate, 0)
            
            g_IRs(nCurveCount).NeutralHTBRate(g_IRs(nCurveCount).PointsCount).Dte = nDTE
            g_IRs(nCurveCount).NeutralHTBRate(g_IRs(nCurveCount).PointsCount).Rate = (g_IRs(nCurveCount).HTBRate(g_IRs(nCurveCount).PointsCount).Rate + g_IRs(nCurveCount).LongRate(g_IRs(nCurveCount).PointsCount).Rate) / 2#
            
            g_IRs(nCurveCount).PointsCount = g_IRs(nCurveCount).PointsCount + 1
            rsPoint.MoveNext
        Wend
        
        nCurveCount = nCurveCount + 1
        rsCurve.MoveNext
    Wend
    
EX:
    Set rsCurve = Nothing
    Exit Sub
EH:
    ShowError "Fail to load interest rates"
    ClearInterestRates
    GoTo EX
End Sub

Public Sub ClearInterestRates()
    On Error Resume Next
    Erase g_IRs
End Sub

' lPeriodType: 1 - days, 2 - weeks, 3 - months, 4 - years
Public Function GetDays(ByVal dtDate As Date, ByVal lNumber As Long, ByVal lPeriodType As Long) As Long
    On Error Resume Next
    Select Case lPeriodType
        Case 1
            GetDays = DateDiff("d", dtDate, DateAdd("d", lNumber, dtDate))
        Case 2
            GetDays = DateDiff("d", dtDate, DateAdd("ww", lNumber, dtDate))
        Case 3
            GetDays = DateDiff("d", dtDate, DateAdd("m", lNumber, dtDate))
        Case 4
            GetDays = DateDiff("d", dtDate, DateAdd("yyyy", lNumber, dtDate))
        Case Else
            GetDays = 0
    End Select
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetRatesArray(ByVal IsHTB As Boolean, ByVal Position As Double, RatesArr() As InterestRatePoint) As Long
    On Error Resume Next
    If GetIRateRuleType = enAlwaysUseNeutralRate Then
        ' Neutral rate
        If IsHTB Then
            RatesArr = g_IRs(0).NeutralHTBRate()
        Else
            RatesArr = g_IRs(0).NeutralRate()
        End If
    Else
        ' Pos based
        If Abs(Position) > GetIRatePosThreshold Then
            ' Big positions
            If Position > 0 Then
                'Buy Bid Long
                RatesArr = g_IRs(0).LongRate()
            Else
                'Sell q < 0 Ask Short
                If IsHTB Then
                    RatesArr = g_IRs(0).HTBRate()
                Else
                    RatesArr = g_IRs(0).ShortRate()
                End If
            End If
        Else
            ' Small positions
            If IsHTB Then
                RatesArr = g_IRs(0).NeutralHTBRate()
             Else
                RatesArr = g_IRs(0).NeutralRate()
            End If
        End If
    End If
    GetRatesArray = g_IRs(0).PointsCount
End Function

