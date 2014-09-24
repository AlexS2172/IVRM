Attribute VB_Name = "basHelpTips"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public g_frmHelp As frmHelp
Private m_frm As Form

Private Const SM_CYSMCAPTION = 51
Private Const SM_CXDLGFRAME = 7
Private Const SM_CYDLGFRAME = 8

Public g_lHelpTop As Long
Public g_lHelpLeft As Long
Public g_lHelpWidth As Long
Public g_lHelpHeight As Long
Public g_bShowHelpTags As Boolean

Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpEx(Optional ByVal sTitle As String = "", Optional ByVal bModal As Boolean = False, Optional ByVal sText As String = "")
    On Error Resume Next
    If g_frmHelp Is Nothing Then
        
        Set g_frmHelp = New frmHelp
        Load g_frmHelp
    End If
    
    If sTitle <> "" Then
        g_frmHelp.Caption = "Help - " & sTitle
    Else
        g_frmHelp.Caption = "Help"
    End If
    
    g_frmHelp.lblHelp.Caption = sText
    g_frmHelp.Height = _
        g_frmHelp.lblHelp.Height _
        + g_frmHelp.lblHelp.Top * 2 _
        + 2 * g_frmHelp.ScaleY(GetSystemMetrics(SM_CYDLGFRAME), vbPixels, vbTwips) _
        + g_frmHelp.ScaleY(GetSystemMetrics(SM_CYSMCAPTION), vbPixels, vbTwips)
    
    If m_frm Is Nothing Then
        g_frmHelp.Show IIf(bModal, vbModal, vbModeless), frmMain
    Else
        Err.Clear
        g_frmHelp.Show IIf(bModal, vbModal, vbModeless), m_frm
        If Err Then
            Debug.Print Err.Number, Err.Description
        End If
        If Err = 401 Then
            'Can 't show non-modal form when modal form is displayed
            g_frmHelp.Show vbModal, m_frm
        End If
        Set m_frm = Nothing
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ShowHelp(ByVal sTitle As String, ByVal sText As String)
    ShowHelpEx sTitle, False, Space(8) & sText
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpID(ByVal HelpID As Long, Optional frm As Form)
    
    If Not frm Is Nothing Then
        Set m_frm = frm
    End If
    
    Select Case HelpID
    'Portfolio tab
    Case 101:   ShowHelp "Underlying symbol", "Stock symbol"
    Case 102:   ShowHelp "Expiry date", "Stock option expiration date. Use UL for stock positions."
    Case 103:   ShowHelp "Delta", "Delta expressed as underlying stock shares amount. Used for Delta Hedge only."
    Case 104:   ShowHelp "Stock Price", "Current spot price of underlying stock."
    Case 105:   ShowHelp "Vega", "Vega is a rate of change of the option position value corresponding to 1% change in the implied volatility"
    Case 106:   ShowHelp "Stock HV", "The Stock HV % column displays an annualized historical volatility calculated on the basis of the recent 252 days."
    Case 107:   ShowHelp "Stock IV", "The Stock IV % column displays  an annualized implied volatility index calculated on the basis of 16 vega weighted ATM optins and normalized on 252 days."
    
    'Summary tab
    Case 201:   ShowHelp "Equivalent index position, $", "Dollar equivalent of index positions used for Portfolio hedge. Calculated as sum of individual index dollar equivalent positions."
    Case 202:   ShowHelp "Unhedged deviation, $", "Measure of unhedged subportfolio risk. With 95% probability loss will be not greater than this value multiplied by a factor of 1.65"
    Case 203:   ShowHelp "Hedged deviation, $", "Measure of hedged subportfolio risk. With 95% probability loss will be not greater than this value multiplied by a factor of 1.65"
    Case 204:   ShowHelp "Gain, %", "The Gain shows Hedging efficiency expressed as percentage of deviation decrease achieved by hedging."
    Case 205:   ShowHelp "Original positions count", "Number of positions in original portfolio."
    Case 206:   ShowHelp "Hedged positions", "Number of positions included into Hedged Portfolio."
    Case 207:   ShowHelp "Equivalent hedged positions, $", "Dollar equivalent of positions in Hedged Portfolio."
    Case 208:   ShowHelp "Equivalent non-hedged positions, $", "Dollar equivalent of remaining positions not included in hedged Portfolio."
    
    Case 209:   ShowHelp "Index symbol", "Index symbol"
    Case 210:   ShowHelp "Equivalent index position, $", "Dollar equivalent of index position used to hedge Portfolio"
    Case 211:   ShowHelp "Index price", "Market index price"
    Case 212:   ShowHelp "Equivalent index vega", "Index Vega value calculated for ATM of second month expiration."
    
    Case 213:   ShowHelp "Unhedged positions", "Number of positions not included into Hedged Portfolio."

    
    'Stocks tab
    Case 301:   ShowHelp "Underlying symbol", "Stock symbol."
    Case 302:   ShowHelp "Index symbol", "Index symbol."
    Case 303:   ShowHelp "Index HV, %", "The Index HV % column displays annualized historical volatility calculated on the bases of the recent 252 days."
    Case 304:   ShowHelp "Index IV, %", "The Index IV % column displays annualized implied volatility index calculated on the basis of 16 vega weighted ATM optins and normalized on 252 days.."
    Case 305:   ShowHelp "Index price", "Market index price"
    Case 306:   ShowHelp "Index vega", "Index Vega value calculated for ATM of second month expiration."
    
    Case 307:   ShowHelpID 101 'ShowHelp "Underlying symbol", ""
    Case 308:   ShowHelpID 104 'ShowHelp "Stock Price", ""
    Case 309:   ShowHelpID 106 'ShowHelp "Stock HV, %", ""
    Case 310:   ShowHelpID 107 'ShowHelp "Stock IV, %", ""
    Case 311:   ShowHelp "Hedging index", "The most correlated Index with this stock"
    Case 312:   ShowHelp "Price Corr", "Correlation between stock and index price returns."
    Case 313:   ShowHelp "Voly Corr", "Correlation between stock and index implied volatility returns."
    
    'Short&Long
    Case 401:   ShowHelpID 101 'ShowHelp "Underlying symbol", ""
    Case 402:   ShowHelpID 102 'ShowHelp "Expiration date", ""
    Case 403:   ShowHelpID 103 'ShowHelp "Delta", ""
    Case 404:   ShowHelpID 104 'ShowHelp "Stock Price", ""
    Case 405:   ShowHelpID 105 'ShowHelp "Vega", ""
    Case 406:   ShowHelpID 106 'ShowHelp "Stock HV, %", ""
    Case 407:   ShowHelpID 107 'ShowHelp "Stock IV, %", ""
    Case 408:   ShowHelp "Remove", "Click here to remove this position from hedged portfolio"
    Case 409:   ShowHelp "Add", "Click here to add this position to hedged portfolio"
    
    Case 410:   ShowHelp "Hedged", "Shows values for positions included in hedged portfolio"
    Case 411:   ShowHelp "Unhedged", "Shows values for positions not included in hedged portfolio"
    Case 412:   ShowHelp "Index positions, $", "Hedge Index Dollar equivalent position"
    
    Case 413:   ShowHelpID 202 'ShowHelp "Deviation, $", ""
    Case 414:   ShowHelpID 207 'ShowHelp "Equivalent, $", ""
    Case 415:   ShowHelpID 206 'ShowHelp "Positions", ""
    Case 416:   ShowHelpID 204 'ShowHelp "Gain, %", ""
    Case 417:   ShowHelp "Equivalent index vega", "Index Vega value calculated for ATM of second month expiration."
    
    Case 418:   ShowHelp "SHORT", "Shows one of 2 alternative ways to hedge Portfolio. SHORT hedging assumes mainly short index positions."
    Case 419:   ShowHelp "LONG", "Shows one of 2 alternative ways to hedge Portfolio. LONG hedging assumes mainly long index positions."
    Case 420:   ShowHelp "HEDGE", "Shows Portfolio hedge with desirable or best achieved Gain level."
    Case 421:   ShowHelp "NONE", "Shows positions excluded during calculations from the Hedged Portfolio. You can add any position back to Hedged Portfolio using 'Add' link near position."
    
    'Hedge params
    Case 501:   ShowHelp "Desired gain, %", "Choose the desired decrease of portfolio deviation in percents between 0 and 100. Set to 0 to hedge entire portfolio . Set to 100 to calculate the most efficient hedge (some positions will be excluded). "
    Case 502:   ShowHelp "Term, days", "Choose term to use correlations and volatilities based on"
    Case 503:   ShowHelp "Hedging indexes", "Select indexes that will be used for hedging. By default all indexes are selected."
    
    Case Else
        Debug.Assert False
    End Select

End Sub

