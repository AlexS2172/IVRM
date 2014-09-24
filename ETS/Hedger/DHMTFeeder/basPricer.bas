Attribute VB_Name = "basPricer"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Returns one of the available prices
'
Public Function GetPrice(ByVal Bid As Double, ByVal Ask As Double, ByVal Last As Double) As Double
    
    If Bid <> 0 And Ask <> 0 Then
        GetPrice = (Bid + Ask) / 2
        
    ElseIf Bid = 0 And Ask = 0 Then
        GetPrice = Last
        
    ElseIf Bid = 0 Then
        GetPrice = Ask
        
    Else
        GetPrice = Bid
        
    End If

End Function

