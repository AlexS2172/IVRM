Attribute VB_Name = "basVegaWeight"
Option Explicit

Public Type WtdVegaItem
    ID As Long
    Name As String
    Expiry As Date
    Value As Double
End Type

Public Type WtdVegaData
    Maturity As Long
    Count As Long
    Items() As WtdVegaItem
End Type

Public g_WtdVega As WtdVegaData

Public Sub ClearWtdVegaSettings()
    On Error Resume Next
    g_WtdVega.Maturity = 0
    g_WtdVega.Count = 0
    Erase g_WtdVega.Items
End Sub

Public Sub LoadWtdVegaSettings()
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    Dim dWtdVega#
    
    ClearWtdVegaSettings
    
    Set rs = gDBW.usp_WtdVegaMaturity_Get()
    If Not rs.EOF Then
        g_WtdVega.Maturity = DBRead(rs!iWtdVegaMaturity, 0)
        If g_WtdVega.Maturity <= 0 Then g_WtdVega.Maturity = 30
    Else
        g_WtdVega.Maturity = 30
    End If
    Set rs = Nothing
    
    Set rs = gDBW.usp_Expiry_Get(Null, Null)
    While Not rs.EOF
        If DBRead(rs!dtExpiryDate, 0#) > Date Then
            If g_WtdVega.Count > 0 Then
                ReDim Preserve g_WtdVega.Items(0 To g_WtdVega.Count)
            Else
                ReDim g_WtdVega.Items(0 To g_WtdVega.Count)
            End If
            
            g_WtdVega.Items(g_WtdVega.Count).ID = DBRead(rs!iExpiryID, 0)
            g_WtdVega.Items(g_WtdVega.Count).Name = DBRead(rs!vcExpiryPeriod, "")
            g_WtdVega.Items(g_WtdVega.Count).Expiry = DBRead(rs!dtExpiryDate, 0#)
            
            dWtdVega = DBRead(rs!fVegaWeight, 0)
            If dWtdVega <= 0# Then dWtdVega = g_WtdVega.Maturity / (g_WtdVega.Items(g_WtdVega.Count).Expiry - Date)
            g_WtdVega.Items(g_WtdVega.Count).Value = dWtdVega
            g_WtdVega.Count = g_WtdVega.Count + 1
        End If
        rs.MoveNext
    Wend
    
EX:
    Set rs = Nothing
    Exit Sub
EH:
    ShowError "Fail to load weighted vega settings"
    ClearWtdVegaSettings
    GoTo EX
End Sub

Public Sub SaveWtdVegaSettings()
    On Error GoTo EH
    Dim i&, nCount&
    
    gDBW.usp_WtdVegaMaturity_Save g_WtdVega.Maturity
    
    nCount = g_WtdVega.Count - 1
    For i = 0 To nCount
        gDBW.usp_Expiry_Save g_WtdVega.Items(i).ID, Null, Null, g_WtdVega.Items(i).Value
    Next

EX:
    Exit Sub
EH:
    ShowError "Fail to save weighted vega settings"
    GoTo EX
End Sub

Public Function GetVegaWeight(dtExpiry As Date) As Double
    On Error Resume Next
    Dim i&, nCount&, nDTE&
    
    GetVegaWeight = 0#
    If dtExpiry <= Date Then Exit Function
    
    nCount = g_WtdVega.Count - 1
    For i = 0 To nCount
        If Year(g_WtdVega.Items(i).Expiry) = Year(dtExpiry) And Month(g_WtdVega.Items(i).Expiry) = Month(dtExpiry) Then
            GetVegaWeight = g_WtdVega.Items(i).Value
            Exit Function
        End If
    Next
    
    GetVegaWeight = g_WtdVega.Maturity / (dtExpiry - Date)
End Function

