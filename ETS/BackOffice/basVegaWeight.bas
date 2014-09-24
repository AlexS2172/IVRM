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
Public ExpiryDateMax As Date

Public Sub ClearWtdVegaSettings()
    On Error Resume Next
    ExpiryDateMax = 0
    g_WtdVega.Maturity = 0
    g_WtdVega.Count = 0
    Erase g_WtdVega.Items
End Sub

Public Sub LoadWtdVegaSettings()
Dim rs As ADODB.Recordset
Dim dWtdVega#
Dim dtExpiry As Date
Dim rsExpiry As ADODB.Recordset
    
    On Error Resume Next
'    Set rs = ActualExpiryGet(Date)
    On Error GoTo EH
    
    ClearWtdVegaSettings
    
    Set rs = gDBW.usp_WtdVegaMaturity_Get()
    If Not rs.EOF Then
        g_WtdVega.Maturity = DBRead(rs!iWtdVegaMaturity, 0)
        If g_WtdVega.Maturity <= 0 Then g_WtdVega.Maturity = 30
    Else
        g_WtdVega.Maturity = 30
    End If
    Set rs = Nothing
    
    Set rsExpiry = gDBW.usp_MmVegaWeight_Get
    With rsExpiry
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                dtExpiry = DBRead(rsExpiry!dtExpiryOV, 0#)
                ExpiryDateMax = Max(ExpiryDateMax, dtExpiry)
                If dtExpiry > Date Then
                    If g_WtdVega.Count > 0 Then
                        ReDim Preserve g_WtdVega.Items(0 To g_WtdVega.Count)
                    Else
                        ReDim g_WtdVega.Items(0 To g_WtdVega.Count)
                    End If
                    
                    With g_WtdVega.Items(g_WtdVega.Count)
                        .ID = 0
                        .Name = ""
                        .Expiry = dtExpiry
                        dWtdVega = DBRead(rsExpiry!fVegaWeight, 0)
                        If dWtdVega <= 0 Then
                            dWtdVega = DefaultVegaWeight(.Expiry)
                        End If
                        .Value = dWtdVega
                    End With
                    
                    g_WtdVega.Count = g_WtdVega.Count + 1
                End If
            .MoveNext
            Loop
        End If
    End With
EX:
    Set rs = Nothing
    Exit Sub
EH:
    ShowError "Fail to load weighted vega settings"
    ClearWtdVegaSettings
    GoTo EX
End Sub

Public Sub SaveWtdVegaSettings(Optional frmProg As frmProgress = Nothing)
Dim i As Long
    On Error GoTo EH
        
    gDBW.usp_WtdVegaMaturity_Save g_WtdVega.Maturity
    
    If Not frmProg Is Nothing Then
        If frmProg.IsCancel Then
            Err.Raise vbObjectError, , "Operation calceled"
        Else
            frmProg.SetProgress = 2
        End If
    End If
        
    For i = 0 To g_WtdVega.Count - 1
        With g_WtdVega.Items(i)
            gDBW.usp_ExpiryVegaWeightSave_Save .Expiry, .Value
        End With
    
        If Not frmProg Is Nothing Then
            If frmProg.IsCancel Then
                Err.Raise vbObjectError, , "Operation calceled"
            Else
                frmProg.SetProgress = i + 3
            End If
        End If
        DoEvents
    Next
EX:
    If Not frmProg Is Nothing Then
        frmProg.FinishProgress
    End If
    
    Exit Sub
EH:

    ShowError "Fail to save weighted vega settings"
    GoTo EX
End Sub

'Public Sub SaveWtdVegaSettingsForEOD(Optional frmEOD As frmEndOfDay = Nothing)
'    Dim i As Long
'
'    gDBW.usp_WtdVegaMaturity_Save g_WtdVega.Maturity
'
'    If g_WtdVega.Count > 0 Then
'        If Not frmEOD Is Nothing Then
'            frmEOD.pbProgress.Max = g_WtdVega.Count
'            frmEOD.Idle
'        End If
'
'        For i = 0 To g_WtdVega.Count - 1
'            With g_WtdVega.Items(i)
'                gDBW.usp_Expiry_Save .ID, Null, Null, .Value ', .CalendarID
'            End With
'
'            If Not frmEOD Is Nothing Then
'                frmEOD.pbProgress.Value = frmEOD.pbProgress.Value + 1
'                frmEOD.Idle
'            End If
'        Next
'    End If
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DefaultVegaWeight(ByVal dtExpiry As Date) As Double
    On Error Resume Next
    If dtExpiry > Date Then
        DefaultVegaWeight = Round(Sqr(g_WtdVega.Maturity / (dtExpiry - Date)), 4)
    Else
        DefaultVegaWeight = 0#
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function GetVegaWeight(ByVal dtExpiry As Date, ByVal iCalendarID As Long) As Double
'Dim i As Long
'    On Error Resume Next
'    If dtExpiry > Date Then
'        For i = 0 To g_WtdVega.Count - 1
'            With g_WtdVega.Items(i)
'                If Year(.Expiry) = Year(dtExpiry) _
'                And Month(.Expiry) = Month(dtExpiry) _
'                And .CalendarID = iCalendarID Then
'                    GetVegaWeight = .Value
'                    Exit Function
'                End If
'            End With
'        Next
'        GetVegaWeight = DefaultVegaWeight(dtExpiry)
'    Else
'        GetVegaWeight = 0#
'    End If
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetMaturity(ByVal nMaturity As Long)

    g_WtdVega.Maturity = nMaturity
    frmProgress.Initialize g_WtdVega.Count + 2
        
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub RecalculateVegaWeights(Optional frmProg As frmProgress = Nothing)
Dim i As Long
    On Error Resume Next
            
    For i = 0 To g_WtdVega.Count - 1
        With g_WtdVega.Items(i)
            .Value = DefaultVegaWeight(.Expiry)
        End With
    Next
    
    If Not frmProg Is Nothing Then
        frmProg.SetProgress = 1
    End If
    
    DoEvents
    
    SaveWtdVegaSettings frmProg
End Sub

'Public Sub RecalculateVegaWeightsForEOD(Optional frmEOD As frmEndOfDay = Nothing)
'    Dim i As Long
'
'    For i = 0 To g_WtdVega.Count - 1
'        With g_WtdVega.Items(i)
'            .Value = DefaultVegaWeight(.Expiry)
'        End With
'    Next
'
'    If Not frmEOD Is Nothing Then
'        frmEOD.Idle
'    End If
'
'    DoEvents
'
'    SaveWtdVegaSettingsForEOD frmEOD
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function StdExpiryName(ByVal dtExpiry As Date) As String
    StdExpiryName = UCase(Format$(dtExpiry, "MMMYYYY"))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function StdExpiryID(ByVal dtExpiry As Date) As Long
'    StdExpiryID = CLng(dtExpiry) '(Year(dtExpiry) - 2000) * 12 + Month(dtExpiry)
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function StdExpiryByID(ByVal ExpiryID As Long) As Date
'    StdExpiryByID = CDate(ExpiryID) 'CalendarExpiry(DateSerial((ExpiryID - 1) \ 12, 1 + ((ExpiryID - 1) Mod 12), 20))
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function RegisterExpiry(ByVal dtExpiry As Date) As Boolean
'Dim rsExpiryCalendar As ADODB.Recordset
'Dim ExpiryID As Long
'Dim CalendarID As Long
'Dim i As Long
'Dim y As Long
'Dim m As Long
'    ExpiryID = (Year(dtExpiry) - 2000) * 12 + Month(dtExpiry)
'    If ExpiryID > ExpiryIDMax Then
'        ' Reserve expiries for 12 months in advance
'        'ExpiryID = ExpiryID + 12
'        Set rsExpiryCalendar = gDBW.usp_ExpiryCalendar_Get
'        With rsExpiryCalendar
'            If .RecordCount > 0 Then
'                .MoveFirst
'                Do While Not .EOF
'                    CalendarID = !iExpCalendarID
'                    For i = ExpiryIDMax + 1 To ExpiryID
'                        ExpiryIDMax = Max(ExpiryIDMax, i)
'                        y = (i - 1) \ 12 + 2000
'                        m = (i - 1) Mod 12 + 1
'                        dtExpiry = DateSerial(y, m, 20)
'                        dtExpiry = CalendarExpiry(dtExpiry, CalendarID)
'                        gDBW.usp_Expiry_Save i, StdExpiryName(dtExpiry), dtExpiry, DefaultVegaWeight(dtExpiry), CalendarID
'                    Next
'                    .MoveNext
'                Loop
'            End If
'        End With
'    End If
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function ActualExpiryGet(ByVal dtExpiry As Date) As ADODB.Recordset
'Dim rs As ADODB.Recordset
'Dim Sql As String
'    Sql = "select distinct o.expiryid, e.expiryperiod from [option] as o join expiry as e on e.expiryid=o.expiryid where o.actionid<3 order by o.expiryid"
'    Set rs = gDBW.OpenRSDynamic(Sql)
'    Set ActualExpiryGet = rs
'End Function

