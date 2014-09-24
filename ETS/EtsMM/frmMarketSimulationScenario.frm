VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmMarketSimulationScenario 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Simulation Scenario"
   ClientHeight    =   2715
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   6195
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2715
   ScaleWidth      =   6195
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.ComboBox cmbVolatilityWeighting 
      Height          =   315
      Left            =   4440
      Style           =   2  'Dropdown List
      TabIndex        =   15
      Top             =   1800
      Width           =   1335
   End
   Begin VB.CommandButton btnVolatilitySpecificShift 
      Caption         =   "..."
      Height          =   255
      Left            =   5880
      TabIndex        =   14
      Top             =   1800
      Width           =   255
   End
   Begin VB.CommandButton btnPriceSpecificShift 
      Caption         =   "..."
      Height          =   255
      Left            =   5880
      TabIndex        =   13
      Top             =   360
      Width           =   255
   End
   Begin VB.ComboBox cmbPriceShiftDriver 
      Height          =   315
      Left            =   4440
      Style           =   2  'Dropdown List
      TabIndex        =   12
      Top             =   360
      Width           =   1335
   End
   Begin VB.ComboBox cmbTimeScenario 
      Height          =   315
      Left            =   120
      Style           =   2  'Dropdown List
      TabIndex        =   11
      Top             =   1080
      Width           =   1815
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Height          =   375
      Left            =   4440
      TabIndex        =   10
      Top             =   2280
      Width           =   1335
   End
   Begin VB.TextBox txtVolatilityShiftValue 
      Height          =   315
      Left            =   2040
      TabIndex        =   9
      Text            =   "0.0"
      Top             =   1800
      Width           =   855
   End
   Begin VB.TextBox txtPriceShiftValue 
      Height          =   315
      Left            =   2040
      TabIndex        =   8
      Text            =   "0.0"
      Top             =   360
      Width           =   855
   End
   Begin VB.ComboBox cmbPriceShiftUnit 
      Height          =   315
      Left            =   3000
      Style           =   2  'Dropdown List
      TabIndex        =   7
      Top             =   360
      Width           =   1335
   End
   Begin VB.ComboBox cmbVolatilityShiftUnit 
      Height          =   315
      Left            =   3000
      Style           =   2  'Dropdown List
      TabIndex        =   6
      Top             =   1800
      Width           =   1335
   End
   Begin VB.ComboBox cmbVolatilityScenario 
      Height          =   315
      Left            =   120
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   1800
      Width           =   1815
   End
   Begin MSComCtl2.DTPicker dtSimulationDate 
      Height          =   315
      Left            =   2040
      TabIndex        =   1
      Top             =   1080
      Width           =   2295
      _ExtentX        =   4048
      _ExtentY        =   556
      _Version        =   393216
      Format          =   48889857
      CurrentDate     =   40255
   End
   Begin VB.ComboBox cmbPriceScenario 
      Height          =   315
      Left            =   120
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   360
      Width           =   1815
   End
   Begin VB.Label lblVolatilityScenario 
      BackStyle       =   0  'Transparent
      Caption         =   "Volatility Scenario"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   1560
      Width           =   1335
   End
   Begin VB.Label lblPriceScenario 
      BackStyle       =   0  'Transparent
      Caption         =   "Price Scenario"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   1335
   End
   Begin VB.Label lblTimeScenario 
      BackStyle       =   0  'Transparent
      Caption         =   "Time Scenario"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   840
      Width           =   1335
   End
End
Attribute VB_Name = "frmMarketSimulationScenario"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Enum PriceScenarioEnum
    PS_NONE = 0
    PS_COMMON = 1
    PS_CORRELATED = 2
    PS_SPECIFIC = 3
End Enum

Public Enum TimeScenarioEnum
    TS_NONE = 0
    TS_COMMON = 1
End Enum

Public Enum VolatilityScenarioEnum
    VS_NONE = 0
    VS_COMMON = 1
    VS_SPECIFIC = 2
End Enum

Public Enum ShiftUnitEnum
    SU_ABS = 0
    SU_PRC = 1
End Enum

Public Enum VolatilityWeightEnum
    VW_NONE = 0
    VW_MATURITY = 1
End Enum

Private current_scenario As MarketSimulationScenario

Private Sub AddItem(ByRef box As ComboBox, _
                    ByVal item_caption As String, _
                    ByVal item_id As Long)
                    
On Error GoTo error_handler
'--------------------------'
    box.AddItem item_caption, box.ListCount
    box.ItemData(box.ListCount - 1) = item_id
    
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while AddItem. Description: " & _
    Err.Description
End Sub

Private Sub FillVolatilitWeightingControl()
    On Error GoTo error_handler
    '--------------------------'
        
        '---- initialize combo box ----'
        cmbVolatilityWeighting.Clear
        
        ' add default value'
        AddItem cmbVolatilityWeighting, "Parallel", VW_NONE
        AddItem cmbVolatilityWeighting, "Weighted", VW_MATURITY
        
        cmbVolatilityWeighting.ListIndex = VW_NONE
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while fill Volatilit weighting Control. Description: " & _
                Err.Description
End Sub

Private Sub FillTimeControl()
    On Error GoTo error_handler
    '--------------------------'
        dtSimulationDate.Value = Date
        
        '---- initialize combo box ----'
        cmbTimeScenario.Clear
        
        ' add default value'
        AddItem cmbTimeScenario, "No simulation", TS_NONE
        AddItem cmbTimeScenario, "Common", TS_COMMON
        
        cmbTimeScenario.ListIndex = TS_NONE
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while fill Time Control. Description: " & _
                Err.Description
End Sub


Private Sub FillShiftDriverControl()
    On Error GoTo error_handler
    '--------------------------'
        Dim shift_driver As EtsGeneralLib.IndexAtom
        
        If (Not g_HedgeSymbols Is Nothing) Then
                
            For Each shift_driver In g_HedgeSymbols
                
                AddItem cmbPriceShiftDriver, shift_driver.Symbol, shift_driver.ID
                               
            Next
            
            cmbPriceShiftDriver.ListIndex = 0
            
        End If
        
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while initializing ShiftDriver control. Description: " & _
                Err.Description
End Sub

Private Function getPriceShiftDriver() As IAsset
    On Error GoTo error_handler
    '--------------------------'
        
        Set getPriceShiftDriver = Nothing
        
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbPriceShiftDriver.ListIndex
        item_value = cmbPriceShiftDriver.ItemData(list_index)
        
        If (Not g_Main Is Nothing) Then
            Set getPriceShiftDriver = g_Main.UnderlyingAll(item_value)
        End If
        
    Exit Function
'------------'
error_handler:
    Debug.Print "Can't return PriceShiftDriver. Description: " & _
                Err.Description
End Function
Private Sub FillPriceScenarioControl()
    On Error GoTo error_handler
    '--------------------------'
    
    '---- initialize combo box ----'
    cmbPriceScenario.Clear
    
    ' add default value'
    AddItem cmbPriceScenario, "No simulation", PS_NONE
    AddItem cmbPriceScenario, "Common", PS_COMMON
    AddItem cmbPriceScenario, "Correlated", PS_CORRELATED
    AddItem cmbPriceScenario, "Specific", PS_SPECIFIC
    
    cmbPriceScenario.ListIndex = PS_NONE
    
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while fill Price Scenario Control. Description: " & _
    Err.Description
End Sub

Private Sub FillVolatilityScenarioControl()
    On Error GoTo error_handler
    '--------------------------'
    
    '---- initialize combo box ----'
    cmbVolatilityScenario.Clear
    
    ' add default value'
    AddItem cmbVolatilityScenario, "No simulation", VS_NONE
    AddItem cmbVolatilityScenario, "Common", VS_COMMON
    AddItem cmbVolatilityScenario, "Specific", VS_SPECIFIC
    
    cmbVolatilityScenario.ListIndex = VS_NONE
    
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while fill Volatility Scenario Control. Description: " & _
    Err.Description
End Sub

Private Sub FillScenarioShiftUnitControl()
    On Error GoTo error_handler
    '--------------------------'
    
    '---- initialize combo box ----'
    cmbVolatilityShiftUnit.Clear
    
    ' add default value'
    AddItem cmbVolatilityShiftUnit, "Absolute", SU_ABS
    AddItem cmbVolatilityShiftUnit, "Percent(%)", SU_PRC
    
    cmbVolatilityShiftUnit.ListIndex = SU_PRC

    
    '---- initialize combo box ----'
    cmbPriceShiftUnit.Clear
    
    AddItem cmbPriceShiftUnit, "Absolute", SU_ABS
    AddItem cmbPriceShiftUnit, "Percent(%)", SU_PRC
    
    cmbPriceShiftUnit.ListIndex = SU_PRC
    
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while fill Scenario Shift Units Control. Description: " & _
    Err.Description
End Sub

Private Function getPriceShiftValue() As Double
    On Error GoTo error_handler
    '--------------------------'
        
        Dim shift_value As Double
        
        shift_value = CDbl(txtPriceShiftValue.Text)
        
        getPriceShiftValue = shift_value

    Exit Function
'-------------'
error_handler:
    If (Err.Number = 13) Then 'check type mismatch
        Err.Raise vbObjectError + 4, "Simulation Scenario", "Invalid price shift value."
    End If
End Function

Private Function getVolatilityShiftValue() As Double
    On Error GoTo error_handler
    '--------------------------'
        
        Dim shift_value As Double
        
        shift_value = CDbl(txtVolatilityShiftValue.Text)
        
        If (SU_ABS = getVolatilityShiftUnit) Then
            shift_value = shift_value * 0.01
        End If
        
        getVolatilityShiftValue = shift_value

    Exit Function
'-------------'
error_handler:
    If (Err.Number = 13) Then 'check type mismatch
        Err.Raise vbObjectError + 4, "Simulation Scenario", "Invalid volatility shift value."
    End If
End Function

Private Function getTimeShiftValue() As Double
    On Error GoTo error_handler
    '--------------------------'
        
        Dim shift_value As Double
        
        shift_value = ClipTime(dtSimulationDate.Value) - CLng(Date)
        
        getTimeShiftValue = shift_value

    Exit Function
'-------------'
error_handler:
    If (Err.Number = 13) Then  'check type mismatch
        Err.Raise vbObjectError + 4, "Simulation Scenario", "Invalid time shift value."
    End If
End Function

Private Function getVolatilityShiftUnit() As Long
    On Error GoTo error_handler
    '--------------------------'
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbVolatilityShiftUnit.ListIndex
        item_value = cmbVolatilityShiftUnit.ItemData(list_index)
    
        getVolatilityShiftUnit = item_value
        
    Exit Function
'-------------'
error_handler:
    MsgBox "Error while getting volatility shift unit. Description: " & _
            Err.Description
End Function

Private Function getPriceShiftUnit() As Long
    On Error GoTo error_handler
    '--------------------------'
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbPriceShiftUnit.ListIndex
        item_value = cmbPriceShiftUnit.ItemData(list_index)
    
        getPriceShiftUnit = item_value
                
    Exit Function
'-------------'
error_handler:
    MsgBox "Error while getting price shift unit. Description: " & _
            Err.Description
End Function

Private Function getPriceShiftScenario() As Long
    On Error GoTo error_handler
    '--------------------------'
        getPriceShiftScenario = 0
        
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbPriceScenario.ListIndex
        item_value = cmbPriceScenario.ItemData(list_index)
    
        getPriceShiftScenario = item_value
                
    Exit Function
'-------------'
error_handler:
    MsgBox "Error while getting price scenario. Description: " & _
            Err.Description
End Function

Private Function getVolatilityShiftScenario() As Long
    On Error GoTo error_handler
    '--------------------------'
    
        getVolatilityShiftScenario = 0
    
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbVolatilityScenario.ListIndex
        item_value = cmbVolatilityScenario.ItemData(list_index)
        
        getVolatilityShiftScenario = item_value
                
    Exit Function
'-------------'
error_handler:
    MsgBox "Error while getting price scenario. Description: " & _
            Err.Description
End Function

Private Function getVolatilityWeightingMode() As Long
    On Error GoTo error_handler
    '--------------------------'
    
        getVolatilityWeightingMode = 0
    
        Dim list_index As Long
        Dim item_value As Long
    
        list_index = cmbVolatilityWeighting.ListIndex
        item_value = cmbVolatilityWeighting.ItemData(list_index)
        
        getVolatilityWeightingMode = item_value
                
    Exit Function
'-------------'
error_handler:
    MsgBox "Error while getting volatility weighting mode. Description: " & _
            Err.Description
End Function

Private Function getTimeShiftScenario() As Long
    On Error GoTo error_handler
    '--------------------------'
        getTimeShiftScenario = 0
        
        Dim list_index As Long
        Dim item_value As Long
        
        list_index = cmbTimeScenario.ListIndex
        item_value = cmbTimeScenario.ItemData(list_index)
        
        getTimeShiftScenario = item_value
                
    Exit Function
'--------------'
error_handler:
    MsgBox "Error while getting time scenario. Description: " & _
            Err.Description
End Function

Public Sub InitializeControls()
    On Error GoTo error_handler
    '--------------------------'
    
    FillTimeControl
    FillPriceScenarioControl
    FillVolatilityScenarioControl
    FillScenarioShiftUnitControl
    FillShiftDriverControl
    FillVolatilitWeightingControl
    
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while initialize controls. Description: " & _
    Err.Description
End Sub

Private Function getSimulationScenario() As MarketSimulationScenario
    On Error GoTo error_handler
    '--------------------------'
                        
        If (current_scenario Is Nothing) Then
            Set current_scenario = New MarketSimulationScenario
        End If
                        
        current_scenario.PriceShiftUnit = getPriceShiftUnit
        current_scenario.PriceShiftValue = getPriceShiftValue
        current_scenario.PriceShiftScenario = getPriceShiftScenario
        
        current_scenario.VolatilityShiftUnit = getVolatilityShiftUnit
        current_scenario.VolatilityShiftValue = getVolatilityShiftValue
        current_scenario.VolatilityShiftScenario = getVolatilityShiftScenario
        
        current_scenario.TimeShiftValue = getTimeShiftValue
        current_scenario.TimeShiftScenario = getTimeShiftScenario
        current_scenario.TimeShiftUnit = 0
        
        current_scenario.MaturityWeghted = getVolatilityWeightingMode
        
        Dim shift_driver As IAsset
        Set shift_driver = getPriceShiftDriver
        
        Set current_scenario.PriceShiftDriver = shift_driver
        
        current_scenario.RebuildScenario
        
        Set getSimulationScenario = current_scenario
    Exit Function
'------------'
error_handler:
    If (Err.Number > vbObjectError) Then
        MsgBox "Error while trying to create Scenario. Description: " & Err.Description
        
        Err.Raise vbObjectError + 1, "Simulation Scenario", "Invalid value."
    Else
        Debug.Print "Error while creating Scenario. Description: " & Err.Description
    End If
End Function

Public Function Execute(ByRef scenario As MarketSimulationScenario) As MarketSimulationScenario
    On Error GoTo error_handler
    '--------------------------'
        Load Me
        
        InitializeControls
        
        Set current_scenario = scenario
        
        setControlsValueByScenario scenario
        
        Me.Show vbModal
        
        Set Execute = current_scenario
                
    Exit Function
'------------'
error_handler:
    MsgBox "Error while executing Market simulation form. Description: " & Err.Description
End Function

Private Sub setPriceShiftDriver(ByRef shift_driver As IAsset)
    On Error GoTo error_handler
    '--------------------------'
        Dim i As Long
                
        If (Not shift_driver Is Nothing) Then
            For i = 0 To cmbPriceShiftDriver.ListCount - 1
                If (cmbPriceShiftDriver.ItemData(i) = shift_driver.ID) Then
                    cmbPriceShiftDriver.ListIndex = i
                    Exit For
                End If
            Next i
        Else
            cmbPriceShiftDriver.ListIndex = -1
        End If
                
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting price scenario. Descriptio: " & _
            Err.Description
End Sub

Private Sub setVolatilityWeightingMode(ByVal Mode As Long)
    On Error GoTo error_handler
    '--------------------------'
    
        cmbVolatilityWeighting.ListIndex = Mode
        
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting volatility weighting mode. Descriptio: " & _
            Err.Description

End Sub

Private Sub setTimeScenario(ByVal scenario As Long)
    On Error GoTo error_handler
    '--------------------------'
    
        cmbTimeScenario.ListIndex = scenario
        
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting time scenario. Descriptio: " & _
            Err.Description
End Sub

Private Sub setPriceScenario(ByVal scenario As Long)
    On Error GoTo error_handler
    '--------------------------'
        
        cmbPriceScenario.ListIndex = scenario
        
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting price scenario. Descriptio: " & _
            Err.Description
End Sub

Private Sub setVolatilityScenario(ByVal scenario As Long)
    On Error GoTo error_handler
    '--------------------------'
    
        cmbVolatilityScenario.ListIndex = scenario
    
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting volatility scenario. Descriptio: " & _
            Err.Description
End Sub

Private Sub setPriceShiftValue(ByVal Value As Double)
    On Error GoTo error_handler
    '--------------------------'
        
        txtPriceShiftValue.Text = Value
    
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting price shift value. Descriptio: " & _
            Err.Description
End Sub

Private Sub setVolatilityShiftValue(ByVal Value As Double)
    On Error GoTo error_handler
    '--------------------------'
                
        If (SU_ABS = getVolatilityShiftUnit) Then
            Value = Value * 100#
        End If
        
        txtVolatilityShiftValue.Text = Value
    
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting volatility shift value. Descriptio: " & _
            Err.Description
End Sub

Private Sub setTimeShiftValue(ByVal Value As Double)
    On Error GoTo error_handler
    '--------------------------'
        
        dtSimulationDate.Value = GetNewYorkTime + Value
    
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting time shift value. Descriptio: " & _
            Err.Description
End Sub


Private Sub setPriceShiftUnit(ByVal unit As Long)
    On Error GoTo error_handler
    '--------------------------'
        
        cmbPriceShiftUnit.ListIndex = unit
                
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting price shift unit. Descriptio: " & _
            Err.Description
End Sub

Private Sub setVolatilityShiftUnit(ByVal unit As Long)
    On Error GoTo error_handler
    '--------------------------'
        
        cmbVolatilityShiftUnit.ListIndex = unit
                
    Exit Sub
'-----------'
error_handler:
    MsgBox "Error while setting volatility shift unit. Descriptio: " & _
            Err.Description
End Sub

Private Sub setControlsValueByScenario(ByRef scenario As MarketSimulationScenario)
    On Error GoTo error_handler
    '--------------------------'
    
        If (scenario Is Nothing) Then Exit Sub
        
        setTimeShiftValue scenario.TimeShiftValue
        setTimeScenario scenario.TimeShiftScenario
        
        setPriceShiftValue scenario.PriceShiftValue
        setPriceScenario scenario.PriceShiftScenario
        setPriceShiftUnit scenario.PriceShiftUnit
        
        setPriceShiftDriver scenario.PriceShiftDriver
        
        setVolatilityShiftUnit scenario.VolatilityShiftUnit
        setVolatilityShiftValue scenario.VolatilityShiftValue
        setVolatilityScenario scenario.VolatilityShiftScenario
        
        setVolatilityWeightingMode scenario.MaturityWeghted
        
    Exit Sub
'------------'
error_handler:
    MsgBox "Error while initializing controls by scenario. Description: " & _
            Err.Description
End Sub

Private Sub OnPriceScenarioChanged(ByVal scenario_id As Long)
    On Error GoTo error_handler
    '--------------------------'
        btnPriceSpecificShift.Enabled = IIf(scenario_id = PS_SPECIFIC, True, False)
        cmbPriceShiftDriver.Enabled = IIf(scenario_id = PS_CORRELATED, True, False)
        cmbPriceShiftUnit.Enabled = IIf(scenario_id <> PS_NONE, True, False)
        txtPriceShiftValue.Enabled = IIf(scenario_id <> PS_NONE, True, False)
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while calling OnPriceScenarioChanged. Description: " & _
                Err.Description
End Sub

Private Sub OnVolatilityScenarioChanged(ByVal scenario_id As Long)
    On Error GoTo error_handler
    '--------------------------'
        btnVolatilitySpecificShift.Enabled = IIf(scenario_id = VS_SPECIFIC, True, False)
        txtVolatilityShiftValue.Enabled = IIf(scenario_id <> VS_NONE, True, False)
        cmbVolatilityShiftUnit.Enabled = IIf(scenario_id <> VS_NONE, True, False)
        cmbVolatilityWeighting.Enabled = IIf(scenario_id <> VS_NONE, True, False)
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while calling OnVolatilityScenarioChanged. Description: " & _
                Err.Description
End Sub

Private Sub OnTimeScenarioChanged(ByVal scenario_id As Long)
    On Error GoTo error_handler
    '--------------------------'
       dtSimulationDate.Enabled = IIf(scenario_id = TS_COMMON, True, False)
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while calling OnTimeScenarioChanged. Description: " & _
                Err.Description
End Sub


Private Sub btnApply_Click()
    On Error GoTo error_handler
    '--------------------------'
        Set current_scenario = getSimulationScenario
        
        SendMessage Me.hWnd, WM_CLOSE, 0, 0&
    Exit Sub
'------------'
error_handler:
    If (Err.Number <= vbObjectError) Then
        Debug.Print "Error while applying scenario settings. Description: " & _
                    Err.Description
    End If
End Sub

Private Sub btnPriceSpecificShift_Click()
    On Error Resume Next
    '-------------------'
    If (current_scenario Is Nothing) Then
        Set current_scenario = New MarketSimulationScenario
    End If
    
    frmStockShift.Execute current_scenario, 0
    
End Sub

Private Sub btnVolatilitySpecificShift_Click()
    On Error Resume Next
    '-------------------'
    If (current_scenario Is Nothing) Then
        Set current_scenario = New MarketSimulationScenario
    End If
    
    frmStockShift.Execute current_scenario, 1
End Sub

Private Sub cmbPriceScenario_Click()
    On Error Resume Next
    '-------------------'
    OnPriceScenarioChanged getPriceShiftScenario
End Sub

Private Sub cmbTimeScenario_Click()
    On Error Resume Next
    '-------------------'
    OnTimeScenarioChanged getTimeShiftScenario
End Sub

Private Sub cmbVolatilityScenario_Click()
    On Error Resume Next
    '-------------------'
    OnVolatilityScenarioChanged getVolatilityShiftScenario
End Sub
