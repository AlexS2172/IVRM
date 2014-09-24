VERSION 5.00
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmBatchCapability 
   Caption         =   "Batch Capability"
   ClientHeight    =   4035
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9570
   Icon            =   "frmBatchCapability.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   4035
   ScaleWidth      =   9570
   StartUpPosition =   3  'Windows Default
   Begin EtsMMCv.ctlRiskView ctlView 
      Height          =   255
      Left            =   8160
      TabIndex        =   6
      Top             =   1320
      Visible         =   0   'False
      Width           =   255
      _ExtentX        =   450
      _ExtentY        =   450
   End
   Begin VB.CommandButton btnSave 
      Caption         =   "Save"
      Height          =   315
      Left            =   8160
      TabIndex        =   4
      Top             =   3000
      Width           =   1335
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   6000
      Top             =   2280
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete"
      Height          =   315
      Left            =   8160
      TabIndex        =   3
      Top             =   2580
      Width           =   1335
   End
   Begin VB.CommandButton btnAddNew 
      Caption         =   "Add"
      Height          =   315
      Left            =   8160
      TabIndex        =   2
      Top             =   2160
      Width           =   1335
   End
   Begin VB.Timer tmrCalc 
      Enabled         =   0   'False
      Interval        =   60000
      Left            =   5040
      Top             =   2400
   End
   Begin VB.CommandButton btnClose 
      Caption         =   "Close"
      Height          =   315
      Left            =   8160
      TabIndex        =   5
      Top             =   3420
      Width           =   1335
   End
   Begin VB.CommandButton btnStart 
      Caption         =   "Start Now"
      Height          =   315
      Left            =   8160
      TabIndex        =   1
      Top             =   60
      Width           =   1335
   End
   Begin MSComctlLib.ProgressBar pbProgress 
      Height          =   240
      Left            =   0
      TabIndex        =   7
      Top             =   3780
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   423
      _Version        =   393216
      Appearance      =   1
      Scrolling       =   1
   End
   Begin MSComctlLib.ProgressBar pbOverall 
      Height          =   240
      Left            =   5820
      TabIndex        =   10
      Top             =   3780
      Visible         =   0   'False
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   423
      _Version        =   393216
      Appearance      =   1
      Scrolling       =   1
   End
   Begin VSFlex8Ctl.VSFlexGrid fgShedule 
      Height          =   3735
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   8055
      _cx             =   14208
      _cy             =   6588
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   1
      SelectionMode   =   1
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   1
      Cols            =   7
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   2
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
      Begin VB.Timer tmrClearView 
         Enabled         =   0   'False
         Interval        =   10
         Left            =   3120
         Top             =   1920
      End
   End
   Begin VB.Image imgOverallStop 
      Height          =   240
      Left            =   7800
      MouseIcon       =   "frmBatchCapability.frx":0E42
      MousePointer    =   99  'Custom
      Picture         =   "frmBatchCapability.frx":0F94
      ToolTipText     =   "Cancel"
      Top             =   3780
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Label lblOverall 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Overall:"
      Height          =   195
      Left            =   5220
      TabIndex        =   6
      Top             =   3780
      Visible         =   0   'False
      Width           =   540
   End
   Begin VB.Image imgStopDis 
      Enabled         =   0   'False
      Height          =   240
      Left            =   0
      Picture         =   "frmBatchCapability.frx":10DE
      Top             =   3780
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Image imgStop 
      Height          =   240
      Left            =   1980
      MouseIcon       =   "frmBatchCapability.frx":1228
      MousePointer    =   99  'Custom
      Picture         =   "frmBatchCapability.frx":137A
      ToolTipText     =   "Cancel"
      Top             =   3780
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Label lblStatus 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Height          =   195
      Left            =   60
      TabIndex        =   9
      Top             =   3780
      Width           =   45
   End
   Begin VB.Label lblProcess 
      AutoSize        =   -1  'True
      BackStyle       =   0  'Transparent
      Caption         =   "Process status..."
      Height          =   195
      Left            =   2280
      TabIndex        =   8
      Top             =   3780
      Visible         =   0   'False
      Width           =   1170
   End
End
Attribute VB_Name = "frmBatchCapability"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private m_frmOwner As Form

Private m_bIsLoaded As Boolean
Private m_bIsOpened As Boolean
Private m_bInit As Boolean
Private m_bCalc As Boolean
Private m_bExecProjection As Boolean

Private m_CurProjection As clsProjectionInfo
Private m_CurGroup As clsProjectionGroupInfo
Private m_CurShift As clsStockShift

Private m_bIsNewFile As Boolean

Private m_bFirstShift As Boolean
Private m_bFirstShiftInGroup As Boolean
Private m_bLastShift As Boolean
Private m_bLastShiftInGroup As Boolean
Private m_ShiftIdx As Long

Private m_sExportFileName As String

Private m_bManualStart As Boolean

Private m_nFrmHeight As Long
Private m_nFrmWidth As Long

Private m_bStopCalc As Boolean

Private Enum BatchCapabilityColumnEnum
    BCC_ID = 0
    
    BCC_USER
    BCC_PROJECTION
    BCC_LAYOUT
'    BCC_AGG
    BCC_SHEDULE
    BCC_TIME
    BCC_DAILY
    BCC_IS_SHEDULE_COMPLETE
    BCC_RESULT_FILE
    
    BCC_COLUMN_COUNT
End Enum

Private m_nRowsCount As Long

Private m_Shedule As Collection

Public Sub Init()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability Init Enter.", GetCaption

    If m_bIsLoaded Then Exit Sub
    m_bIsLoaded = True
    
    m_bCalc = False
    m_bInit = False
    m_bExecProjection = False
    
    m_bManualStart = False
    
    CopyShedule g_Params.ProjectionsShedule, m_Shedule

    Load Me
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability Init Exit.", GetCaption
End Sub

Public Sub ShowData(Optional ByRef frmOwner As Form = Nothing)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability ShowData Enter.", GetCaption
    
    If m_bIsOpened Then Exit Sub
    m_bIsOpened = True
    
    Set m_frmOwner = Nothing
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_nRowsCount = g_Params.ProjectionsShedule.Count + 1
    
    'CopyShedule g_Params.ProjectionsShedule, m_Shedule
    InitControls
    RefreshData
    
    Me.Show
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability ShowData Exit.", GetCaption
End Sub

Private Sub AddNewShedule()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability AddNewShedule Enter.", GetCaption
    
    Dim aNewShedule As clsProjectionInfo
    Set aNewShedule = New clsProjectionInfo
    
    aNewShedule.ProjectionFile = STR_NA
    aNewShedule.LayoutFile = STR_NA
    aNewShedule.AggByHolding = False
    aNewShedule.SheduleEnabled = False
    aNewShedule.StartTime = Now
    aNewShedule.EveryDay = False
    
    aNewShedule.Init
    aNewShedule.Status = SS_NEW
    
    m_Shedule.Add aNewShedule
    m_nRowsCount = m_nRowsCount + 1
    
    Set aNewShedule = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability AddNewShedule Exit.", GetCaption
End Sub

Private Sub DeleteShedule(ByVal Idx As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability DeleteShedule Enter. Idx = " & CStr(Idx), GetCaption
    
    m_Shedule(Idx).Status = SS_DELETED
    m_nRowsCount = m_nRowsCount - 1
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability DeleteShedule Exit.", GetCaption
End Sub

Private Sub InitControls()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability InitControls Enter.", GetCaption
    
    m_bInit = True
    
    fgShedule.Cols = BCC_COLUMN_COUNT
    fgShedule.Rows = m_nRowsCount
    
    fgShedule.TextMatrix(0, BCC_ID) = "#"
    fgShedule.TextMatrix(0, BCC_USER) = "User"
    fgShedule.TextMatrix(0, BCC_PROJECTION) = "Projection"
    fgShedule.TextMatrix(0, BCC_LAYOUT) = "Layout"
'    fgShedule.TextMatrix(0, BCC_AGG) = "Aggregation"
    fgShedule.TextMatrix(0, BCC_SHEDULE) = "Shedule"
    fgShedule.TextMatrix(0, BCC_TIME) = "Time"
    fgShedule.TextMatrix(0, BCC_DAILY) = "Daily"
    fgShedule.TextMatrix(0, BCC_IS_SHEDULE_COMPLETE) = "IsComplete"
    fgShedule.TextMatrix(0, BCC_RESULT_FILE) = "ResultFile"
    
    fgShedule.ColComboList(BCC_PROJECTION) = "..."
    fgShedule.ColComboList(BCC_LAYOUT) = "..."
'    fgShedule.ColComboList(BCC_AGG) = "#1;By Holding|#0;By Contract"
    fgShedule.ColComboList(BCC_SHEDULE) = "#1;Enabled|#0;Disabled"
    fgShedule.ColComboList(BCC_DAILY) = "#1;Yes|#0;No"
    fgShedule.ColComboList(BCC_IS_SHEDULE_COMPLETE) = "#1;Yes|#0;No"
    
    fgShedule.ColAlignment(BCC_ID) = flexAlignLeftCenter
    fgShedule.ColAlignment(BCC_PROJECTION) = flexAlignLeftCenter
    fgShedule.ColAlignment(BCC_LAYOUT) = flexAlignLeftCenter
'    fgShedule.ColAlignment(BCC_AGG) = flexAlignLeftCenter
    fgShedule.ColAlignment(BCC_SHEDULE) = flexAlignLeftCenter
    fgShedule.ColAlignment(BCC_TIME) = flexAlignLeftCenter
    fgShedule.ColAlignment(BCC_DAILY) = flexAlignLeftCenter
    
    fgShedule.ColDataType(BCC_TIME) = flexDTDate
    fgShedule.ColFormat(BCC_TIME) = "hh:mm:ss AMPM"
    
    fgShedule.ColHidden(BCC_USER) = Not g_Params.m_bExecProjectionsForAll
    
    m_bInit = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability InitControls Exit.", GetCaption
End Sub

Private Sub RefreshData()
    On Error Resume Next
    Dim aProjection As clsProjectionInfo
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability RefreshData Enter.", GetCaption
    
    m_bInit = True
    
    Dim nRow As Long
    Dim Idx As Long
    For Each aProjection In m_Shedule
        Idx = Idx + 1
            
        If aProjection.Status <> SS_DELETED Then
            nRow = nRow + 1
            fgShedule.TextMatrix(nRow, BCC_ID) = Idx
            fgShedule.TextMatrix(nRow, BCC_USER) = aProjection.UserName
            fgShedule.TextMatrix(nRow, BCC_PROJECTION) = aProjection.ProjectionFileName
            fgShedule.TextMatrix(nRow, BCC_LAYOUT) = aProjection.LayoutFileName
'            fgShedule.TextMatrix(nRow, BCC_AGG) = IIf(aProjection.AggByHolding, 1, 0)
            fgShedule.TextMatrix(nRow, BCC_SHEDULE) = IIf(aProjection.SheduleEnabled, 1, 0)
            fgShedule.TextMatrix(nRow, BCC_TIME) = aProjection.StartTime
            fgShedule.TextMatrix(nRow, BCC_DAILY) = IIf(aProjection.EveryDay, 1, 0)
            fgShedule.TextMatrix(nRow, BCC_IS_SHEDULE_COMPLETE) = IIf(aProjection.IsCalcComplete, 1, 0)
            fgShedule.TextMatrix(nRow, BCC_RESULT_FILE) = IIf(aProjection.ResultFileName = "", STR_NA, aProjection.ResultFileName)
        
            fgShedule.RowData(nRow) = aProjection.ID
        End If
    Next
    
    fgShedule.AutoSize 0, fgShedule.Cols - 1, , 100
    
    RefreshProgressBar
    
    m_bInit = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability RefreshData Exit.", GetCaption
End Sub

Private Sub RefreshProgressBar()
    On Error Resume Next
    Dim nOverallCount As Double
    nOverallCount = m_CurProjection.ShiftsCount
    
    If nOverallCount > 0 And Not m_bLastShift Then
        pbOverall.Value = (m_ShiftIdx / nOverallCount) * 100
    
        pbOverall.Visible = True
        lblOverall.Visible = True
        imgOverallStop.Visible = True
    Else
        pbOverall.Value = pbOverall.Min
        
        pbOverall.Visible = False
        lblOverall.Visible = False
        imgOverallStop.Visible = False
    End If
End Sub

Private Sub btnAddNew_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnAddNew_Click Enter.", GetCaption
    
    If m_bInit Then Exit Sub
    
    AddNewShedule
    InitControls
    RefreshData
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnAddNew_Click Exit.", GetCaption
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnClose_Click Enter.", GetCaption
    
    m_bIsOpened = False
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    'Set m_Shedule = Nothing
    
    Me.Hide
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnClose_Click Exit.", GetCaption
End Sub

Private Sub btnDelete_Click()
    On Error Resume Next
    Dim nRow As Long
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnDelete_Click Enter.", GetCaption
    
    If m_bInit Then Exit Sub
    
    nRow = fgShedule.RowSel
    If nRow > 0 Then
        DeleteShedule (CLng(fgShedule.TextMatrix(nRow, BCC_ID)))
        InitControls
        RefreshData
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnDelete_Click Exit.", GetCaption
End Sub

Private Sub btnSave_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnSave_Click Enter.", GetCaption
    
    
    Screen.MousePointer = vbHourglass
    g_Params.ProjectionsShedule = m_Shedule
    g_Params.SaveProjectionShedule
    g_Params.LoadProjectionShedule
    
    CopyShedule g_Params.ProjectionsShedule, m_Shedule
    InitControls
    RefreshData
    Screen.MousePointer = vbDefault
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnSave_Click Exit.", GetCaption
End Sub

Private Sub btnStart_Click()
    On Error Resume Next
    Dim nRow As Long
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnStart_Click Enter.", GetCaption
    
    nRow = fgShedule.RowSel
    
    If nRow > 0 And Not m_bExecProjection Then
'        While m_bExecProjection
'            MMSleep 500
'        Wend
        
        m_bManualStart = True
        CalcProjection m_Shedule(CLng(fgShedule.TextMatrix(nRow, BCC_ID)))
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability btnStart_Click Exit.", GetCaption
End Sub

Private Sub ctlView_OnRefreshCancel()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshCancel Enter. " & GetCurInfo, GetCaption
        
    tmrClearView.Enabled = True
    
    Set m_CurProjection = Nothing
    Set m_CurGroup = Nothing
    Set m_CurShift = Nothing
    
    m_bManualStart = False
    
    If Not m_bLastShift Then m_bStopCalc = True
    If m_bLastShift Then m_bExecProjection = False
    
    RefreshProgressBar
        
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshCancel Exit.", GetCaption
End Sub

Private Sub ctlView_OnRefreshComplete()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshComplete Enter. " & GetCurInfo, GetCaption
    
    If m_bFirstShift Then m_sExportFileName = GetFileName
    If m_bFirstShiftInGroup Then ctlView.InitBatchBuffer m_CurGroup.SpotShiftCount, m_CurGroup.VolaShiftCount
        
    ctlView.SaveBatchResult m_CurShift.ShiftID, m_CurShift.Shift, m_CurShift.VolaShiftID, m_CurShift.VolaShift
    
    If m_bLastShiftInGroup Then
        ctlView.ExportBatchResults m_sExportFileName, m_CurGroup.OriginalStr, m_bIsNewFile
        m_bIsNewFile = False
    End If
        
    If m_bLastShift Then
'        If Not m_bManualStart Then
            m_CurProjection.LastCalcDate = Date
            m_CurProjection.ResultFileName = m_sExportFileName
            gDBW.usp_ProjectionShedule_CalcComplete m_CurProjection.ID, m_CurProjection.LastCalcDate, m_CurProjection.ResultFileName
            RefreshData
'        End If
        
        m_bExecProjection = False
        
        LogEvent EVENT_INFO, "Projection calculation finished. Projection file """ & m_CurProjection.ProjectionFileName & """ Layout file """ & m_CurProjection.LayoutFileName
    End If
    
    tmrClearView.Enabled = True
        
    Set m_CurProjection = Nothing
    Set m_CurGroup = Nothing
    Set m_CurShift = Nothing
    
    m_bManualStart = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshComplete Exit.", GetCaption
End Sub

Private Sub ctlView_OnRefreshEmpty()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshEmpty Enter. " & GetCurInfo, GetCaption
        
    LogEvent EVENT_ERROR, "Can't find positions in group """ & m_CurShift.Symbol & """."
    
    If m_bFirstShift Then m_sExportFileName = GetFileName
'    If m_bFirstShiftInGroup Then ctlView.InitBatchBuffer m_CurGroup.SpotShiftCount, m_CurGroup.VolaShiftCount
'
'    ctlView.SaveBatchResult m_CurShift.ShiftID, m_CurShift.Shift, m_CurShift.VolaShiftID, m_CurShift.VolaShift
'
'    If m_bLastShiftInGroup Then
'        ctlView.ExportBatchResults m_sExportFileName, m_CurGroup.OriginalStr, m_bIsNewFile
'        m_bIsNewFile = False
'    End If
        
    If m_bLastShift Then
'        If Not m_bManualStart Then
            m_CurProjection.LastCalcDate = Date
            m_CurProjection.ResultFileName = m_sExportFileName
            gDBW.usp_ProjectionShedule_CalcComplete m_CurProjection.ID, m_CurProjection.LastCalcDate, m_CurProjection.ResultFileName
            RefreshData
'        End If
        
        m_bExecProjection = False
        
        LogEvent EVENT_INFO, "Projection calculation finished. Projection file """ & m_CurProjection.ProjectionFileName & """ Layout file """ & m_CurProjection.LayoutFileName
    End If
    
    tmrClearView.Enabled = True
        
    Set m_CurProjection = Nothing
    Set m_CurGroup = Nothing
    Set m_CurShift = Nothing
    
    m_bManualStart = False
        
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshEmpty Exit.", GetCaption
End Sub

Private Sub ctlView_OnRefreshError()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshError Enter. " & GetCurInfo, GetCaption
    
    LogEvent EVENT_ERROR, "Error during projection. Projection file """ & m_CurProjection.ProjectionFileName & """ Layout file """ & m_CurProjection.LayoutFileName
    
    If m_bFirstShift Then m_sExportFileName = GetFileName
    If m_bFirstShiftInGroup Then ctlView.InitBatchBuffer m_CurGroup.SpotShiftCount, m_CurGroup.VolaShiftCount
    
    ctlView.SaveBatchResult m_CurShift.ShiftID, m_CurShift.Shift, m_CurShift.VolaShiftID, m_CurShift.VolaShift
    
    If m_bLastShiftInGroup Then
        ctlView.ExportBatchResults m_sExportFileName, m_CurGroup.OriginalStr, m_bIsNewFile
        m_bIsNewFile = False
    End If
    
    If m_bLastShift Then
'        If Not m_bManualStart Then
            m_CurProjection.LastCalcDate = Date
            m_CurProjection.ResultFileName = m_sExportFileName
            gDBW.usp_ProjectionShedule_CalcComplete m_CurProjection.ID, m_CurProjection.LastCalcDate, m_CurProjection.ResultFileName
            RefreshData
'        End If
        
        m_bExecProjection = False
            
        LogEvent EVENT_INFO, "Projection calculation finished. Projection file """ & m_CurProjection.ProjectionFileName & """ Layout file """ & m_CurProjection.LayoutFileName
    End If
    
    tmrClearView.Enabled = True
    
    Set m_CurProjection = Nothing
    Set m_CurGroup = Nothing
    Set m_CurShift = Nothing
    
    m_bManualStart = False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability OnRefreshError Exit.", GetCaption
End Sub

Private Sub fgShedule_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim dtValue As Date
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability fgShedule_AfterEdit Enter. Row/Col = " & CStr(Row) & "/" & CStr(Col), GetCaption
    
    If m_bInit Then Exit Sub
    
    Dim Idx As Long
    Idx = CLng(fgShedule.TextMatrix(Row, BCC_ID))
    
    Select Case Col
'        Case BCC_AGG
'            m_Shedule(Idx).AggByHolding = CBool(fgShedule.TextMatrix(Row, Col))
'            If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
        
        Case BCC_SHEDULE
            m_Shedule(Idx).SheduleEnabled = CBool(fgShedule.TextMatrix(Row, Col))
            If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
        
        Case BCC_TIME
            Err.Clear
            dtValue = CDate(fgShedule.TextMatrix(Row, Col))
            If Err.Number <> 0 Then
                fgShedule.TextMatrix(Row, Col) = m_Shedule(Idx).StartTime
            Else
                m_Shedule(Idx).StartTime = dtValue
                If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
            End If
        
        Case BCC_DAILY
            m_Shedule(Idx).EveryDay = CBool(fgShedule.TextMatrix(Row, Col))
            If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
        
        Case Else
    End Select
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability fgShedule_AfterEdit Exit.", GetCaption
End Sub

Private Sub fgShedule_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    If m_bInit Then Exit Sub
    
    If Col = BCC_ID Or Col = BCC_IS_SHEDULE_COMPLETE Or Col = BCC_RESULT_FILE Or Col = BCC_USER Then
        Cancel = True
        Exit Sub
    End If
    
End Sub

Private Sub fgShedule_CellButtonClick(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim Idx As Long
    Dim sFileName As String
    
    If m_bInit Then Exit Sub
    
    Idx = CLng(fgShedule.TextMatrix(Row, BCC_ID))
    
    Select Case Col
        Case BCC_PROJECTION
            sFileName = ProjectionFileDialog
            If sFileName <> STR_NA Then
                m_Shedule(Idx).ProjectionFile = sFileName
                m_Shedule(Idx).ProjectionFileName = ExtractFileName(sFileName)
                m_Shedule(Idx).LoadProjectionFile
                If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
            
                fgShedule.TextMatrix(Row, Col) = m_Shedule(Idx).ProjectionFileName
            End If
        
        Case BCC_LAYOUT
            sFileName = LayoutFileDialog
            If sFileName <> STR_NA Then
                m_Shedule(Idx).LayoutFile = sFileName
                m_Shedule(Idx).LayoutFileName = ExtractFileName(sFileName)
                If m_Shedule(Idx).Status = SS_COMMON Then m_Shedule(Idx).Status = SS_UPDATED
            
                fgShedule.TextMatrix(Row, Col) = m_Shedule(Idx).LayoutFileName
            End If
        
        Case Else
    End Select
    
End Sub

Private Function ProjectionFileDialog() As String
    On Error Resume Next
    
    ProjectionFileDialog = STR_NA
    
    dlgCommon.Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                                     Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
    
    dlgCommon.CancelError = True
    dlgCommon.DefaultExt = ".txt"
    dlgCommon.DialogTitle = "Load projection file"
    dlgCommon.FileName = ""
    dlgCommon.InitDir = g_Params.CurrentUserPrivateFolder
    dlgCommon.Filter = "Text files (*.txt;*.csv)|*.txt;*.csv|All Files|*.*"
    dlgCommon.FilterIndex = 1
    
    dlgCommon.ShowOpen
    If Err.Number = 0 Then
        ProjectionFileDialog = dlgCommon.FileName
    End If
    
End Function

Private Function LayoutFileDialog() As String
    On Error Resume Next
    
    LayoutFileDialog = STR_NA
    
    dlgCommon.Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                                     Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
    
    dlgCommon.CancelError = True
    dlgCommon.DefaultExt = ".txt"
    dlgCommon.DialogTitle = "Load layout file"
    dlgCommon.FileName = ""
    dlgCommon.InitDir = g_Params.CurrentUserPrivateFolder
    dlgCommon.Filter = "Risk View layout files (*.rvw)|*.rvw|All Files|*.*"
    dlgCommon.FilterIndex = 1
    
    dlgCommon.ShowOpen
    If Err.Number = 0 Then
        LayoutFileDialog = dlgCommon.FileName
    End If
    
End Function

Private Sub Form_Load()
    On Error Resume Next
    
    InitRisksView
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
    
    m_nRowsCount = 1
    
    tmrCalc.Enabled = True
    tmrClearView.Enabled = False
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    
    fgShedule.Move fgShedule.Left, fgShedule.Top, ScaleWidth - 1515, ScaleHeight - 300
    
    btnAddNew.Move btnAddNew.Left + (Me.Width - m_nFrmWidth), btnAddNew.Top + (Me.Height - m_nFrmHeight)
    btnStart.Move btnStart.Left + (Me.Width - m_nFrmWidth), btnStart.Top
    btnClose.Move btnClose.Left + (Me.Width - m_nFrmWidth), btnClose.Top + (Me.Height - m_nFrmHeight)
    btnSave.Move btnSave.Left + (Me.Width - m_nFrmWidth), btnSave.Top + (Me.Height - m_nFrmHeight)
    btnDelete.Move btnDelete.Left + (Me.Width - m_nFrmWidth), btnDelete.Top + (Me.Height - m_nFrmHeight)
    
    pbProgress.Move pbProgress.Left, pbProgress.Top + (Me.Height - m_nFrmHeight)
    lblProcess.Move lblProcess.Left, lblProcess.Top + (Me.Height - m_nFrmHeight)
    lblStatus.Move lblStatus.Left, lblStatus.Top + (Me.Height - m_nFrmHeight)
    imgStop.Move imgStop.Left, imgStop.Top + (Me.Height - m_nFrmHeight)
    imgStopDis.Move imgStopDis.Left, imgStopDis.Top + (Me.Height - m_nFrmHeight)
    
    lblOverall.Move lblOverall.Left + (Me.Width - m_nFrmWidth), lblOverall.Top + (Me.Height - m_nFrmHeight)
    pbOverall.Move pbOverall.Left + (Me.Width - m_nFrmWidth), pbOverall.Top + (Me.Height - m_nFrmHeight)
    imgOverallStop.Move imgOverallStop.Left + (Me.Width - m_nFrmWidth), imgOverallStop.Top + (Me.Height - m_nFrmHeight)
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
End Sub

Public Sub Term()
    On Error Resume Next
    Set m_Shedule = Nothing
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Cancel = True
    m_bIsOpened = False
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    'Set m_Shedule = Nothing
    
    Me.Hide
End Sub

Private Sub imgOverallStop_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability imgOverallStop_Click.", GetCaption
    
    tmrClearView.Enabled = True
    
    Set m_CurProjection = Nothing
    Set m_CurGroup = Nothing
    Set m_CurShift = Nothing
    
    m_bManualStart = False
    
    If Not m_bLastShift Then m_bStopCalc = True
    If m_bLastShift Then m_bExecProjection = False
    
    RefreshProgressBar
End Sub

Private Sub tmrCalc_Timer()
    On Error Resume Next
    Dim dtTimeNow As Date
    Dim dtDateNow As Date
    Dim dtDateDefault As Date
    Dim aProjection As clsProjectionInfo
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability tmrCalc_Timer Enter.", GetCaption
    
    dtTimeNow = TimeValue(CStr(Now))
    dtDateNow = DateValue(CStr(Now))
    
    For Each aProjection In m_Shedule 'g_Params.ProjectionsShedule
        If (aProjection.Status = SS_COMMON And aProjection.SheduleEnabled) And _
           ((aProjection.EveryDay And DateValue(CStr(aProjection.LastCalcDate)) < dtDateNow And TimeValue(CStr(aProjection.StartTime)) < dtTimeNow) Or _
            (Not aProjection.EveryDay And aProjection.LastCalcDate = 0 And TimeValue(CStr(aProjection.StartTime)) < dtTimeNow)) Then
                        
'                While m_bExecProjection
'                    MMSleep 500
'                Wend

                If m_bExecProjection Then
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability tmrCalc_Timer. Some projection already executed.", GetCaption
                    Exit Sub
                End If
                
                m_bManualStart = False
                CalcProjection aProjection
        End If
    Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability tmrCalc_Timer Exit.", GetCaption
End Sub

Private Sub CalcProjection(ByRef aProjection As clsProjectionInfo)
    On Error Resume Next
    Dim nShiftIdx As Long
    Dim aGroup As clsProjectionGroupInfo
    Dim aShift As clsStockShift
    Dim aStorage As clsSettingsStorage
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CalcProjection Enter." & vbTab & aProjection.ProjectionFileName & vbTab & aProjection.LayoutFileName, GetCaption
        
    LogEvent EVENT_INFO, "Projection calculation started. Projection file """ & aProjection.ProjectionFileName & """ Layout file """ & aProjection.LayoutFileName
    
    If aProjection.ShiftsCount = 0 Then
        LogEvent EVENT_ERROR, "Can't find projections in file " & aProjection.ProjectionFileName
        Exit Sub
    End If
    
    m_bIsNewFile = True
    m_bExecProjection = True
    m_bStopCalc = False
    m_ShiftIdx = 0
    
    For Each aGroup In aProjection.Groups
        nShiftIdx = 0
        For Each aShift In aGroup.Shifts
            While m_bCalc
                MMSleep 100
            Wend
            
            If m_bStopCalc Then
                m_bStopCalc = False
                m_bExecProjection = False
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CalcProjection Stop Calc.", GetCaption
                Exit Sub
            End If
            
            m_bCalc = True
            m_bFirstShift = CBool(m_ShiftIdx = 0)
            m_bFirstShiftInGroup = CBool(nShiftIdx = 0)
            m_bLastShift = False
            m_bLastShiftInGroup = False
            
            InitRisksView
            
            Set aStorage = New clsSettingsStorage
            aStorage.Init aProjection.LayoutFile
            aStorage.ReadData
            If aStorage.GetStringValue("Type", "Value") = "RisksView" Then
                ctlView.OpenFromFile aStorage, "", False
            Else
                m_bCalc = False
                LogEvent EVENT_ERROR, "Can't start projections calculation, invalid risks file " & aProjection.LayoutFile
                Exit Sub
            End If
            
            Set m_CurProjection = aProjection
            Set m_CurGroup = aGroup
            Set m_CurShift = aShift
            
            RefreshProgressBar
            
            If Not g_PerformanceLog Is Nothing Then _
                g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CalcProjection. " & GetCurInfo & vbTab & CStr(aShift.Shift) & vbTab & CStr(aShift.VolaShift) & vbTab & CStr(aShift.CalcDate), GetCaption
            
            ctlView.SetSimulationParams True, aShift.Shift, aShift.VolaShift, aShift.CalcDate
            ctlView.ShowData IIf(aShift.SymbolID = -1, TYPE_ALL, TYPE_GROUP), aShift.SymbolID
                        
            nShiftIdx = nShiftIdx + 1
            m_ShiftIdx = m_ShiftIdx + 1
        Next
        m_bLastShiftInGroup = True
    Next
    
    m_bLastShift = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CalcProjection Exit.", GetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = Me.Caption
End Function

Private Sub CopyShedule(ByRef aSrc As Collection, ByRef aDst As Collection)
    On Error Resume Next
    Dim aSrcItem As clsProjectionInfo
    Dim aDstItem As clsProjectionInfo
    Dim aSrcGroupItem As clsProjectionGroupInfo
    Dim aDstGroupItem As clsProjectionGroupInfo
    Dim aSrcShiftItem As clsStockShift
    Dim aDstShiftItem As clsStockShift

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CopyShedule Enter.", GetCaption

    Set aDst = Nothing
    Set aDst = New Collection

    For Each aSrcItem In aSrc
        Set aDstItem = New clsProjectionInfo

        aDstItem.ID = aSrcItem.ID
        aDstItem.UserName = aSrcItem.UserName
        aDstItem.ProjectionFile = aSrcItem.ProjectionFile
        aDstItem.ProjectionFileName = aSrcItem.ProjectionFileName
        aDstItem.LayoutFile = aSrcItem.LayoutFile
        aDstItem.LayoutFileName = aSrcItem.LayoutFileName
        aDstItem.ResultFileName = aSrcItem.ResultFileName

        aDstItem.AggByHolding = aSrcItem.AggByHolding
        aDstItem.EveryDay = aSrcItem.EveryDay
        aDstItem.SheduleEnabled = aSrcItem.SheduleEnabled
        aDstItem.StartTime = aSrcItem.StartTime
        
        aDstItem.LastCalcDate = aSrcItem.LastCalcDate
        
        aDstItem.ShiftsCount = aSrcItem.ShiftsCount

        For Each aSrcGroupItem In aSrcItem.Groups
            Set aDstGroupItem = New clsProjectionGroupInfo
            
            aDstGroupItem.OriginalStr = aSrcGroupItem.OriginalStr
            
            aDstGroupItem.SpotShiftCount = aSrcGroupItem.SpotShiftCount
            aDstGroupItem.VolaShiftCount = aSrcGroupItem.VolaShiftCount

            For Each aSrcShiftItem In aSrcGroupItem.Shifts
                Set aDstShiftItem = New clsStockShift
    
                aDstShiftItem.Symbol = aSrcShiftItem.Symbol
                aDstShiftItem.SymbolID = aSrcShiftItem.SymbolID
    
                aDstShiftItem.Shift = aSrcShiftItem.Shift
                aDstShiftItem.ShiftID = aSrcShiftItem.ShiftID
    
                aDstShiftItem.IsGroup = aSrcShiftItem.IsGroup
    
                aDstShiftItem.VolaShift = aSrcShiftItem.VolaShift
                aDstShiftItem.VolaShiftID = aSrcShiftItem.VolaShiftID
                
                aDstShiftItem.DateShift = aSrcShiftItem.DateShift
                aDstShiftItem.CalcDate = aSrcShiftItem.CalcDate
                
                aDstGroupItem.Shifts.Add aDstShiftItem
    
                Set aDstShiftItem = Nothing
            Next
            
            aDstItem.Groups.Add aDstGroupItem
            
            Set aDstGroupItem = Nothing
        Next

        aDstItem.Status = SS_COMMON

        aDst.Add aDstItem

        Set aDstItem = Nothing
    Next

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability CopyShedule Exit.", GetCaption
End Sub

Private Function GetCurInfo() As String
    On Error Resume Next
    
    GetCurInfo = IIf(m_CurProjection Is Nothing, " / ", m_CurProjection.ProjectionFileName & "/" & m_CurProjection.LayoutFileName) & "/" & IIf(m_CurShift Is Nothing, " ", m_CurShift.Symbol)
End Function

Private Sub InitRisksView()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability InitRisksView Enter.", GetCaption
    
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    Set ctlView.m_frmOwner = Me
    Set ctlView.pbProgress = pbProgress
    Set ctlView.lblProcess = lblProcess
    Set ctlView.lblStatus = lblStatus
    Set ctlView.imgStop = imgStop
    Set ctlView.imgStopDis = imgStopDis
    
    ctlView.Init False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogDebug, "BatchCapability InitRisksView Exit.", GetCaption
End Sub

Private Sub ClearRisksView()
    On Error Resume Next
    ctlView.Term
    
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
End Sub

Private Function GetHeader() As String
    On Error Resume Next
    
    GetHeader = "Group," & m_CurShift.Symbol & ",Day Shift," & m_CurShift.DateShift & ",Stock Shift," & m_CurShift.Shift & ",Volatility Shift," & m_CurShift.VolaShift
    
End Function

Private Function GetFileName() As String
    On Error Resume Next
    Dim sLayout As String: sLayout = m_CurProjection.LayoutFileName
    Dim sProjection As String: sProjection = m_CurProjection.ProjectionFileName
    
    If InStrRev(sLayout, ".") <> 0 Then sLayout = ExtractName(sLayout)
    If InStrRev(sProjection, ".") <> 0 Then sProjection = ExtractName(sProjection)
    
    GetFileName = m_CurProjection.UserName & "_" & sLayout & "_" & sProjection & Format(Date, "_MMddyy") & ".csv"
End Function

Private Sub tmrClearView_Timer()
    On Error Resume Next
    tmrClearView.Enabled = False
    m_bCalc = False
    
    ClearRisksView
End Sub
