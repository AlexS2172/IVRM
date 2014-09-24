VERSION 5.00
Object = "{0BE3824E-5AFE-4B11-A6BC-4B3AD564982A}#8.0#0"; "olch2x8.ocx"
Begin VB.UserControl ctlRiskMatrixGraph 
   ClientHeight    =   6960
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9210
   ScaleHeight     =   6960
   ScaleMode       =   0  'User
   ScaleWidth      =   8427.631
   Begin VB.Frame fraControls 
      BorderStyle     =   0  'None
      Height          =   915
      Left            =   0
      TabIndex        =   1
      Top             =   5520
      Width           =   8895
      Begin VB.CommandButton btnApply 
         Caption         =   "Apply"
         Height          =   315
         Left            =   4800
         TabIndex        =   9
         Top             =   540
         Visible         =   0   'False
         Width           =   1335
      End
      Begin VB.ComboBox cbxGraphType 
         Height          =   315
         Left            =   1080
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   120
         Width           =   2055
      End
      Begin VB.ComboBox cbxXAxis 
         Height          =   315
         Left            =   1080
         Style           =   2  'Dropdown List
         TabIndex        =   5
         Top             =   480
         Width           =   2055
      End
      Begin VB.Frame fraChart 
         Caption         =   "Chart"
         Height          =   855
         Left            =   3240
         TabIndex        =   2
         Top             =   0
         Width           =   1455
         Begin VB.OptionButton optSingleLine 
            Caption         =   "Single Line"
            Height          =   255
            Left            =   120
            TabIndex        =   4
            Top             =   240
            Width           =   1215
         End
         Begin VB.OptionButton optMultiLine 
            Caption         =   "Multi Line"
            Height          =   255
            Left            =   120
            TabIndex        =   3
            Top             =   540
            Width           =   975
         End
      End
      Begin VB.Label lblGraphType 
         Caption         =   "Graph Type:"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   120
         Width           =   975
      End
      Begin VB.Label lblXAxis 
         Caption         =   "X Axis:"
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   480
         Width           =   615
      End
   End
   Begin C1Chart2D8.Chart2D chGraph 
      Height          =   5175
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   9210
      _Version        =   524288
      _Revision       =   8
      _ExtentX        =   16245
      _ExtentY        =   9128
      _StockProps     =   0
      ControlProperties=   "ctlRiskMatrixGraph.ctx":0000
   End
End
Attribute VB_Name = "ctlRiskMatrixGraph"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Enum RmGraphTypeEnum
    RMG_PNL = 0
    RMG_DELTA
    RMG_NET_DELTA
    RMG_GAMMA
    RMG_GAMMA_PERC
    RMG_NET_GAMMA
    RMG_VEGA
    RMG_THETA
    RMG_RHO
    RMG_WTD_VEGA
End Enum

Private WithEvents ctlSource As ctlRiskMatrix
Attribute ctlSource.VB_VarHelpID = -1

Private m_GraphType As RmGraphTypeEnum
Private m_XAxis As RmAxisTypeEnum
Private m_YAxis As RmAxisTypeEnum

Private m_bIsSingleLine As Boolean

Private m_Res() As EtsMmRisksLib.MmRvMatrixCalcResultType
Private m_Axis(RMA_HORZ To RMA_VERT) As RmAxisTypeEnum
Private m_Filter(MFC_GROUPS To MFC_COLUMN_COUNT) As Long


Public Sub Init(ByRef Res() As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                ByRef Axis() As RmAxisTypeEnum, _
                ByRef Filter As EtsGeneralLib.EtsFilterData, _
                ByVal nDefID As Long, _
                ByRef ctlView As ctlRiskMatrix)
    On Error Resume Next
    
    SetResArray Res
    SetAxisArray Axis
    SetFilterArray Filter
    
    Set ctlSource = ctlView
    
    m_GraphType = nDefID - MFC_PNL
    m_XAxis = m_Axis(RMA_HORZ)
    m_YAxis = m_Axis(RMA_VERT)
    
    m_bIsSingleLine = False
    
    InitControls
    RefreshControls
    RefreshGraph
End Sub

Private Sub Term()
    On Error Resume Next
    Erase m_Res
    Erase m_Axis
    Erase m_Filter
    
    Set ctlSource = Nothing
End Sub

Private Sub btnApply_Click()
    On Error Resume Next
    RefreshGraph
End Sub

Private Sub cbxGraphType_Click()
    On Error Resume Next
    m_GraphType = cbxGraphType.ItemData(cbxGraphType.ListIndex)
    
    RefreshGraph
End Sub

Private Sub cbxXAxis_Click()
    On Error Resume Next
    m_XAxis = cbxXAxis.ItemData(cbxXAxis.ListIndex)
    
    If m_XAxis = m_Axis(RMA_HORZ) Then
        m_YAxis = m_Axis(RMA_VERT)
    Else
        m_YAxis = m_Axis(RMA_HORZ)
    End If
    
    RefreshGraph
End Sub

Private Sub chGraph_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    Dim sText As String
    Dim px As Long
    Dim py As Long
    
    Dim nRegion As Long
    Dim nSeries As Long
    Dim nPnt As Long
    Dim nDistance As Long
    
    px = X / Screen.TwipsPerPixelX
    py = Y / Screen.TwipsPerPixelY
    
    nRegion = chGraph.ChartGroups(1).CoordToDataIndex(px, py, oc2dFocusXY, nSeries, nPnt, nDistance)
    
    If nDistance <= 5 And nRegion = oc2dRegionInChartArea Then
        sText = sText & GetXAxisName & ": " & Format(chGraph.ChartGroups(1).Data.X(1, nPnt), "#,##0.00") & " "
        sText = sText & GetSeriesName & ": " & Format(GetSeriesValue(nSeries), "#,##0.00") & " "
        sText = sText & GetYAxisName & ": " & Format(chGraph.ChartGroups(1).Data.Y(nSeries, nPnt), "#,##0.00")
    
        chGraph.ToolTipText = sText
        
        chGraph.ChartArea.Markers("X").Index.Series = nSeries
        chGraph.ChartArea.Markers("X").Index.Point = nPnt
        
        chGraph.ChartArea.Markers("Y").Value.Y = chGraph.ChartGroups(1).Data.Y(nSeries, nPnt)

        chGraph.ChartArea.Markers("X").IsShowing = True
        chGraph.ChartArea.Markers("Y").IsShowing = True
    Else
        chGraph.ChartArea.Markers("X").IsShowing = False
        chGraph.ChartArea.Markers("Y").IsShowing = False
        chGraph.ToolTipText = ""
    End If
        
End Sub

Private Sub ctlSource_OnScreenRefresh(Res() As EtsMmRisksLib.MmRvMatrixCalcResultType, Axis() As RmAxisTypeEnum, Filter As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    
    SetResArray Res
    SetAxisArray Axis
    SetFilterArray Filter
        
    InitControls
    RefreshControls
    RefreshGraph
End Sub

Private Sub optMultiLine_Click()
    On Error Resume Next
    m_bIsSingleLine = Not optMultiLine.Value
    
    RefreshGraph
End Sub

Private Sub optSingleLine_Click()
    On Error Resume Next
    m_bIsSingleLine = optSingleLine.Value
    
    RefreshGraph
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    
    chGraph.Left = 0
    chGraph.Top = 0
    chGraph.Width = ScaleWidth
    chGraph.Height = ScaleHeight - chGraph.Top - fraControls.Height
    
    fraControls.Top = chGraph.Height
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub


Private Sub InitControls()
    On Error Resume Next
    
    cbxGraphType.Clear
    
    If m_Filter(MFC_PNL) <> 0 Then
        cbxGraphType.AddItem "PnL"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_PNL
    End If
    
    If m_Filter(MFC_NET_DELTA) <> 0 Then
        cbxGraphType.AddItem "NetDlt"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_NET_DELTA
    End If
    
    If m_Filter(MFC_DELTA) <> 0 Then
        cbxGraphType.AddItem "NetDlt$"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_DELTA
    End If
    
    If m_Filter(MFC_GAMMA) <> 0 Then
        cbxGraphType.AddItem "NetGma$"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_GAMMA
    End If
    
    If m_Filter(MFC_NET_GAMMA) <> 0 Then
        cbxGraphType.AddItem "NetGma$1"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_NET_GAMMA
    End If
    
    If m_Filter(MFC_GAMMA_PERC) <> 0 Then
        cbxGraphType.AddItem "Gma1%"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_GAMMA_PERC
    End If
    
    If m_Filter(MFC_VEGA) <> 0 Then
        cbxGraphType.AddItem "Vga"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_VEGA
    End If
    
    If m_Filter(MFC_THETA) <> 0 Then
        cbxGraphType.AddItem "Tht"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_THETA
    End If
    
    If m_Filter(MFC_RHO) <> 0 Then
        cbxGraphType.AddItem "Rho"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_RHO
    End If
    
    If m_Filter(MFC_WTD_VEGA) <> 0 Then
        cbxGraphType.AddItem "WtdVega"
        cbxGraphType.ItemData(cbxGraphType.NewIndex) = RMG_WTD_VEGA
    End If
    
    InitXAxisControl
End Sub

Private Sub InitXAxisControl()
    On Error Resume Next
    
    cbxXAxis.Clear
    
    AddXAxisControlItem m_Axis(RMA_HORZ)
    AddXAxisControlItem m_Axis(RMA_VERT)
End Sub

Private Sub AddXAxisControlItem(ByVal Item As RmAxisTypeEnum)
    On Error Resume Next
    
    Select Case Item
        Case RMAT_VOLA
            cbxXAxis.AddItem "Vola"
            cbxXAxis.ItemData(cbxXAxis.NewIndex) = RMAT_VOLA
        
        Case RMAT_SPOT
            cbxXAxis.AddItem "Spot"
            cbxXAxis.ItemData(cbxXAxis.NewIndex) = RMAT_SPOT
        
        Case RMAT_TIME
            cbxXAxis.AddItem "Time"
            cbxXAxis.ItemData(cbxXAxis.NewIndex) = RMAT_TIME
    End Select
    
End Sub

Private Sub RefreshXAxisControl()
    On Error Resume Next
    Dim i As Long
    
    For i = 0 To cbxXAxis.ListCount - 1
        If cbxXAxis.ItemData(i) = m_XAxis Then
           cbxXAxis.ListIndex = i
           Exit Sub
        End If
    Next
    
    cbxXAxis.ListIndex = 0
    m_XAxis = cbxXAxis.ItemData(0)
    
    If m_XAxis = m_Axis(RMA_HORZ) Then
        m_YAxis = m_Axis(RMA_VERT)
    Else
        m_YAxis = m_Axis(RMA_HORZ)
    End If
End Sub

Private Sub RefreshGraphTypeControl()
    On Error Resume Next
    Dim i As Long
    
    For i = 0 To cbxGraphType.ListCount - 1
        If cbxGraphType.ItemData(i) = m_GraphType Then
           cbxGraphType.ListIndex = i
           Exit Sub
        End If
    Next
    
    cbxGraphType.ListIndex = 0
    m_GraphType = cbxGraphType.ItemData(0)
End Sub

Private Sub RefreshControls()
    On Error Resume Next
    
    RefreshXAxisControl
    RefreshGraphTypeControl
    
    optSingleLine.Value = m_bIsSingleLine
    optMultiLine.Value = Not m_bIsSingleLine
End Sub


Private Sub SetResArray(ByRef Res() As EtsMmRisksLib.MmRvMatrixCalcResultType)
    On Error Resume Next
    Dim X As Long
    Dim Y As Long
    Dim nLastX As Long
    Dim nLastY As Long
    
    Erase m_Res
    
    nLastX = UBound(Res, 1)
    nLastY = UBound(Res, 2)
    
    If nLastX < 0 Or nLastY < 0 Then Exit Sub
    
    ReDim m_Res(0 To nLastX, 0 To nLastY)
    
    For X = 0 To nLastX
        For Y = 0 To nLastY
        
            m_Res(X, Y).ShiftX = Res(X, Y).ShiftX
            m_Res(X, Y).ShiftY = Res(X, Y).ShiftY
            
            m_Res(X, Y).PnL = Res(X, Y).PnL
            m_Res(X, Y).Delta = Res(X, Y).Delta
            m_Res(X, Y).NetDelta = Res(X, Y).NetDelta
            m_Res(X, Y).Gamma = Res(X, Y).Gamma
            m_Res(X, Y).GammaPerc = Res(X, Y).GammaPerc
            m_Res(X, Y).NetGamma = Res(X, Y).NetGamma
            m_Res(X, Y).Theta = Res(X, Y).Theta
            m_Res(X, Y).Vega = Res(X, Y).Vega
            m_Res(X, Y).Rho = Res(X, Y).Rho
            m_Res(X, Y).WtdVega = Res(X, Y).WtdVega
            
            m_Res(X, Y).BadPnL = Res(X, Y).BadPnL
            m_Res(X, Y).BadDelta = Res(X, Y).BadDelta
            m_Res(X, Y).BadNetDelta = Res(X, Y).BadNetDelta
            m_Res(X, Y).BadGamma = Res(X, Y).BadGamma
            m_Res(X, Y).BadGammaPerc = Res(X, Y).BadGammaPerc
            m_Res(X, Y).BadNetGamma = Res(X, Y).BadNetGamma
            m_Res(X, Y).BadTheta = Res(X, Y).BadTheta
            m_Res(X, Y).BadVega = Res(X, Y).BadVega
            m_Res(X, Y).BadRho = Res(X, Y).BadRho
            m_Res(X, Y).BadWtdVega = Res(X, Y).BadWtdVega
        
        Next
    Next
    
End Sub

Private Sub SetAxisArray(ByRef Axis() As RmAxisTypeEnum)
    On Error Resume Next
    Dim nCount As Long
    
    nCount = UBound(Axis) + 1
    
    If nCount <> 2 Then Exit Sub
    
    m_Axis(RMA_HORZ) = Axis(RMA_HORZ)
    m_Axis(RMA_VERT) = Axis(RMA_VERT)
End Sub

Private Sub SetFilterArray(ByRef Filter As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    Dim i As Long
       
    For i = MFC_GROUPS To MFC_COLUMN_COUNT
        m_Filter(i) = Filter.Data(i)
    Next
End Sub

Private Sub RefreshGraph()
    On Error Resume Next
    Dim i As Long
    Dim j As Long
    
    chGraph.IsBatched = True
    
    ' clear graph
    chGraph.ChartGroups(1).Data.NumSeries = 0
    chGraph.ChartGroups(1).SeriesLabels.RemoveAll
    
    ' redraw
    chGraph.Header.Text = cbxGraphType.Text
    chGraph.ChartGroups(1).Data.NumSeries = GetNumSeries
    
    For i = 1 To chGraph.ChartGroups(1).Data.NumSeries
        chGraph.ChartGroups(1).Data.NumPoints(i) = GetNumPoints
        
        AddSeriesLabels i
        FormatLine i
    Next
    
    
    For j = 1 To chGraph.ChartGroups(1).Data.NumSeries
        For i = 1 To chGraph.ChartGroups(1).Data.NumPoints(1)
            chGraph.ChartGroups(1).Data.X(j, i) = GetValueX(j, i)
            chGraph.ChartGroups(1).Data.Y(j, i) = GetValueY(j, i)
        Next
    Next
        
    chGraph.IsBatched = False
    
End Sub

Private Sub FormatLine(ByVal Idx As Long)
    On Error Resume Next
    
    chGraph.ChartGroups(1).Styles(Idx).Line.Width = 2
    chGraph.ChartGroups(1).Styles(Idx).Symbol.Size = 5
End Sub

Private Function GetNumSeries() As Long
    On Error Resume Next
    
    If m_bIsSingleLine Then
        GetNumSeries = 1
    Else
        If m_XAxis = m_Axis(RMA_HORZ) Then
            GetNumSeries = UBound(m_Res, 2) + 1
        Else
            GetNumSeries = UBound(m_Res, 1) + 1
        End If
    End If
    
End Function

Private Function GetNumPoints() As Long
    On Error Resume Next
    
    If m_XAxis = m_Axis(RMA_HORZ) Then
        GetNumPoints = UBound(m_Res, 1) + 1
    Else
        GetNumPoints = UBound(m_Res, 2) + 1
    End If
    
End Function

Private Function GetValueX(ByVal Series As Long, ByVal Point As Long) As Double
    On Error Resume Next
    
    If m_XAxis = m_Axis(RMA_HORZ) Then
        GetValueX = m_Res(Point - 1, GetResArrayIdx(Series)).ShiftX
    Else
        GetValueX = m_Res(GetResArrayIdx(Series), Point - 1).ShiftY
    End If
    
    GetValueX = GetValueX * IIf(m_XAxis <> RMAT_TIME, 100, 1)
End Function


Private Function GetValueY(ByVal Series As Long, ByVal Point As Long) As Double
    On Error Resume Next
    Dim X As Long
    Dim Y As Long
        
    If m_XAxis = m_Axis(RMA_HORZ) Then
        X = Point - 1
        Y = GetResArrayIdx(Series)
    Else
        X = GetResArrayIdx(Series)
        Y = Point - 1
    End If
    
    Select Case m_GraphType
        Case RMG_PNL
            GetValueY = m_Res(X, Y).PnL
        
        Case RMG_DELTA
            GetValueY = m_Res(X, Y).Delta
            
        Case RMG_NET_DELTA
            GetValueY = m_Res(X, Y).NetDelta
        
        Case RMG_GAMMA
            GetValueY = m_Res(X, Y).Gamma
        
        Case RMG_GAMMA_PERC
            GetValueY = m_Res(X, Y).GammaPerc
        
        Case RMG_NET_GAMMA
            GetValueY = m_Res(X, Y).NetGamma
        
        Case RMG_THETA
            GetValueY = m_Res(X, Y).Theta
        
        Case RMG_VEGA
            GetValueY = m_Res(X, Y).Vega
        
        Case RMG_RHO
            GetValueY = m_Res(X, Y).Rho
        
        Case RMG_WTD_VEGA
            GetValueY = m_Res(X, Y).WtdVega
    End Select
    
    ' TODO:
    If GetValueY = BAD_DOUBLE_VALUE Then GetValueY = 0
    
End Function

Private Sub AddSeriesLabels(ByVal Series As Long)
    On Error Resume Next
    Dim sText As String
    
    sText = GetSeriesName & " " & CStr(GetSeriesValue(Series))
    
    chGraph.ChartGroups(1).SeriesLabels.Add sText
End Sub

Private Function GetResArrayIdx(ByVal Series As Long) As Long
    On Error Resume Next
    
    If m_bIsSingleLine Then
    
        If m_XAxis = m_Axis(RMA_HORZ) Then
            If m_Axis(RMA_VERT) <> RMAT_TIME Then
                GetResArrayIdx = UBound(m_Res, 2) / 2
            Else
                GetResArrayIdx = 0
            End If
        Else
            If m_Axis(RMA_HORZ) <> RMAT_TIME Then
                GetResArrayIdx = UBound(m_Res, 1) / 2
            Else
                GetResArrayIdx = 0
            End If
        End If
        
    Else
        GetResArrayIdx = Series - 1
    End If
End Function

Private Function GetXAxisName() As String
    On Error Resume Next
    
    Select Case m_XAxis
        Case RMAT_VOLA
            GetXAxisName = "Vola"
        
        Case RMAT_SPOT
            GetXAxisName = "Spot"
        
        Case RMAT_TIME
            GetXAxisName = "Time"
    End Select
End Function

Private Function GetSeriesName() As String
    On Error Resume Next
    
    Select Case m_YAxis
        Case RMAT_VOLA
            GetSeriesName = "Vola"
        
        Case RMAT_SPOT
            GetSeriesName = "Spot"
        
        Case RMAT_TIME
            GetSeriesName = "Time"
    End Select
End Function

Private Function GetYAxisName() As String
    On Error Resume Next
    GetYAxisName = cbxGraphType.Text
End Function

Private Function GetSeriesValue(ByVal nID As Long) As Double
    On Error Resume Next
    Dim Idx As Long
    Idx = GetResArrayIdx(nID)
    
    If m_XAxis = m_Axis(RMA_HORZ) Then
        GetSeriesValue = m_Res(0, Idx).ShiftY
    Else
        GetSeriesValue = m_Res(Idx, 0).ShiftX
    End If
    
    GetSeriesValue = GetSeriesValue * IIf(m_YAxis <> RMAT_TIME, 100, 1)
End Function
