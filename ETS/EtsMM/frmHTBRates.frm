VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.Form frmHTBRates 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "HTB Rates for underlying ..."
   ClientHeight    =   3105
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   6015
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3105
   ScaleWidth      =   6015
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Height          =   315
      Left            =   4560
      TabIndex        =   5
      Top             =   2700
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   315
      Left            =   4560
      TabIndex        =   4
      Top             =   2280
      Width           =   1335
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   4560
      TabIndex        =   3
      Top             =   1860
      Width           =   1335
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete"
      Height          =   315
      Left            =   4560
      TabIndex        =   2
      Top             =   540
      Width           =   1335
   End
   Begin VB.CommandButton btnAdd 
      Caption         =   "Add"
      Height          =   315
      Left            =   4560
      TabIndex        =   1
      Top             =   120
      Width           =   1335
   End
   Begin VSFlex7Ctl.VSFlexGrid fgRates 
      Height          =   2895
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4335
      _cx             =   7646
      _cy             =   5106
      _ConvInfo       =   1
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
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   2
      Cols            =   4
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
      Editable        =   0
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
   End
End
Attribute VB_Name = "frmHTBRates"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Enum HTBRatesStatusEnum
    RS_NONE = IDX_NONE
    RS_COMMON
    RS_NEW
    RS_UPDATED
    RS_DELETED
End Enum

Private Enum HTBRatesColumnEnum
    HTB_NONE = IDX_NONE
    HTB_RATE_ID
    HTB_PERIOD
    HTB_PERIOD_TYPE
    HTB_SHORT_RATE
    HTB_LONG_RATE
    HTB_COLUMN_COUNT
    HTB_LAST_COLUMN = HTB_COLUMN_COUNT - 1
End Enum

Private m_frmOwner As Form
Private m_bIsOpened As Boolean

Private m_IsDirty As Boolean

Private m_sLastValue As String

Private m_ID As Long
Private m_Symbol As String
Private m_IsAllowLending As Boolean

Private m_RateColl As EtsGeneralLib.EtsRateColl


Public Sub ShowHTBRates(ByVal UndID As Long, frmOwner As Form, ByVal bIsAllowLending As Boolean)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    
    If m_bIsOpened Then Exit Sub
    
    Set aUnd = g_UnderlyingAll(UndID)
    If aUnd Is Nothing Then
        LogEvent EVENT_ERROR, "Can't find underlying with id = """ & CStr(UndID) & """"
        Exit Sub
    End If
    
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    Set m_RateColl = Nothing
    If Not aUnd.HTBRates Is Nothing Then
        Set m_RateColl = aUnd.HTBRates.CopyTo()
    Else
        Set m_RateColl = New EtsGeneralLib.EtsRateColl
    End If
    
    m_ID = aUnd.ID
    m_Symbol = aUnd.Symbol
    m_IsAllowLending = bIsAllowLending 'aUnd.IsAllowLending
    
    m_IsDirty = False
    
    RefreshScreen
    
    Load Me
    Me.Show
End Sub

Private Sub InitGrid()
    On Error Resume Next
    
    With fgRates
        .Rows = m_RateColl.Count + 1
        .Cols = HTB_COLUMN_COUNT
        
        .ColDataType(HTB_RATE_ID) = flexDTLong
        .ColFormat(HTB_RATE_ID) = ""
        .TextMatrix(0, HTB_RATE_ID) = "ID"
        .ColHidden(HTB_RATE_ID) = True
        
        .ColDataType(HTB_PERIOD) = flexDTLong
        .ColFormat(HTB_PERIOD) = ""
        .TextMatrix(0, HTB_PERIOD) = "Period"
        
        .ColDataType(HTB_PERIOD_TYPE) = flexDTString
        .ColFormat(HTB_PERIOD_TYPE) = ""
        .TextMatrix(0, HTB_PERIOD_TYPE) = "Period Type"
        .ColComboList(HTB_PERIOD_TYPE) = "#1;Day(s)|#2;Week(s)|#3;Month(s)|#4;Year(s)"
        
        .ColDataType(HTB_SHORT_RATE) = flexDTDouble
        .ColFormat(HTB_SHORT_RATE) = "#,##0.00"
        .TextMatrix(0, HTB_SHORT_RATE) = "ShortRate%"
        
        .ColDataType(HTB_LONG_RATE) = flexDTDouble
        .ColFormat(HTB_LONG_RATE) = "#,##0.00"
        .TextMatrix(0, HTB_LONG_RATE) = "LongRate%"
        
        
        AllowBigSelection = False
        .AllowSelection = False
        '.AllowUserFreezing = flexFreezeNone
        '.AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
        .AllowUserResizing = flexResizeColumns
        .Cell(flexcpAlignment, 0, 1, 0, HTB_LAST_COLUMN) = flexAlignCenterCenter
        
        .HighLight = flexHighlightWithFocus
        .FocusRect = flexFocusHeavy
    End With
    
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    Me.Caption = "HTB Rates for underlying " & m_Symbol
    btnApply.Enabled = m_IsDirty
End Sub

Private Sub LoadData()
    On Error Resume Next
    Dim aRate As EtsGeneralLib.EtsRateAtom
    Dim nRow As Long: nRow = 1
    
    For Each aRate In m_RateColl
        fgRates.TextMatrix(nRow, HTB_RATE_ID) = aRate.ID
        fgRates.TextMatrix(nRow, HTB_PERIOD) = aRate.PeriodAmount
        fgRates.TextMatrix(nRow, HTB_PERIOD_TYPE) = aRate.PeriodType
        fgRates.TextMatrix(nRow, HTB_SHORT_RATE) = aRate.ShortRate * 100
        fgRates.TextMatrix(nRow, HTB_LONG_RATE) = aRate.LongRate * 100
        
        fgRates.RowData(nRow) = aRate
        fgRates.RowHidden(nRow) = False
        
        nRow = nRow + 1
    Next
    
End Sub

Private Sub RefreshScreen()
    On Error Resume Next
    UpdateControls
    InitGrid
    LoadData
End Sub

Private Sub SaveDataAndUpdateCollection()
    On Error Resume Next
    Dim nRateID As Long
    Dim nPeriodInDays As Long
    Dim aRate As EtsGeneralLib.EtsRateAtom
    Dim nRow As Long
    
    If fgRates.Rows < 2 Or Not m_IsDirty Then Exit Sub
    
    m_RateColl.Clear
    
    For nRow = 1 To fgRates.Rows - 1
        Set aRate = fgRates.RowData(nRow)
    
        If fgRates.RowHidden(nRow) And aRate.ID <> BAD_LONG_VALUE Then
            ' delete existing rate
            gDBW.usp_IRPoint_Del aRate.ID, g_IRs(0).ID
        End If
        
        If Not fgRates.RowHidden(nRow) And aRate.ID <> BAD_LONG_VALUE Then
            ' update rate
            gDBW.usp_IRPoint_Del aRate.ID, g_IRs(0).ID
                                  
            nRateID = gDBW.usp_IRPoint_Update(aRate.PeriodType, _
                                                aRate.PeriodAmount, _
                                                aRate.ShortRate, _
                                                aRate.LongRate, _
                                                m_ID)
                                                
            aRate.ID = nRateID
            aRate.CalcPeriodInDays Date
            
            nPeriodInDays = m_RateColl.GetPeriodInDays(Date, aRate.PeriodType, aRate.PeriodAmount)
            m_RateColl.Add nPeriodInDays, aRate
        End If
        
        If Not fgRates.RowHidden(nRow) And aRate.ID = BAD_LONG_VALUE Then
            ' add new rate
            nRateID = gDBW.usp_IRPoint_Update(aRate.PeriodType, _
                                                aRate.PeriodAmount, _
                                                aRate.ShortRate, _
                                                aRate.LongRate, _
                                                m_ID)
            
            aRate.ID = nRateID
            aRate.CalcPeriodInDays Date
            
            nPeriodInDays = m_RateColl.GetPeriodInDays(Date, aRate.PeriodType, aRate.PeriodAmount)
            m_RateColl.Add nPeriodInDays, aRate
        End If
        
        Set aRate = Nothing
    Next
    
    m_IsDirty = False
    Set g_UnderlyingAll(m_ID).HTBRates = m_RateColl
    
    RefreshScreen
    
    PubChanges
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox Null, "Fail to save rates data."
End Sub

Private Sub ClearData()
    On Error Resume Next
    Set m_frmOwner = Nothing
    Set m_RateColl = Nothing
    fgRates.Rows = 0
End Sub

Private Sub btnAdd_Click()
    On Error Resume Next
    Dim aRate As EtsGeneralLib.EtsRateAtom
    Dim nRow As Long
    
    fgRates.AddItem ""
    nRow = fgRates.Rows - 1
    If nRow > 0 Then
        Set aRate = New EtsGeneralLib.EtsRateAtom
        
        aRate.ID = BAD_LONG_VALUE
        aRate.PeriodType = enPeriodDays
        aRate.PeriodAmount = 0
        aRate.ShortRate = 0
        aRate.LongRate = 0
    
        fgRates.TextMatrix(nRow, HTB_RATE_ID) = aRate.ID
        fgRates.TextMatrix(nRow, HTB_PERIOD) = aRate.PeriodAmount
        fgRates.TextMatrix(nRow, HTB_PERIOD_TYPE) = aRate.PeriodType
        fgRates.TextMatrix(nRow, HTB_SHORT_RATE) = aRate.ShortRate
        fgRates.TextMatrix(nRow, HTB_LONG_RATE) = aRate.LongRate
        
        fgRates.RowData(nRow) = aRate
        Set aRate = Nothing
    End If
    
    m_IsDirty = True
    UpdateControls
    
End Sub

Private Sub btnApply_Click()
    On Error Resume Next
    SaveDataAndUpdateCollection
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnDelete_Click()
    On Error Resume Next
    Dim nRow As Long
    
    nRow = fgRates.RowSel
    If nRow > 0 Then
        fgRates.RowHidden(nRow) = True
        m_IsDirty = True
        UpdateControls
    End If
End Sub

Private Sub btnOk_Click()
    On Error Resume Next
    
    SaveDataAndUpdateCollection
    Unload Me
End Sub

Private Sub fgRates_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nValue As Long
    Dim dValue As Double
    Dim sValue As String
    Dim aRate As EtsGeneralLib.EtsRateAtom
    
    sValue = Trim(fgRates.TextMatrix(Row, Col))
    If sValue = m_sLastValue Then Exit Sub
    
    Set aRate = fgRates.RowData(Row)
    
    Select Case Col
        Case HTB_PERIOD
            Err.Clear
            nValue = CLng(sValue)
            If Err.Number <> 0 Then
                fgRates.TextMatrix(Row, Col) = m_sLastValue
            Else
                aRate.PeriodAmount = nValue
                m_IsDirty = True
            End If
            
        Case HTB_PERIOD_TYPE
            Err.Clear
            nValue = CLng(sValue)
            If Err.Number <> 0 Then
                fgRates.TextMatrix(Row, Col) = m_sLastValue
            Else
                aRate.PeriodType = nValue
                m_IsDirty = True
            End If
            
        Case HTB_SHORT_RATE
            Err.Clear
            dValue = CDbl(sValue)
            If Err.Number <> 0 Then
                fgRates.TextMatrix(Row, Col) = m_sLastValue
            Else
                aRate.ShortRate = dValue / 100
                m_IsDirty = True
            End If
            
        Case HTB_LONG_RATE
            Err.Clear
            dValue = CDbl(sValue)
            If Err.Number <> 0 Then
                fgRates.TextMatrix(Row, Col) = m_sLastValue
            Else
                aRate.LongRate = dValue / 100
                m_IsDirty = True
            End If
    End Select
    
    UpdateControls
    Set aRate = Nothing
End Sub

Private Sub fgRates_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    
    If Col = HTB_RATE_ID Or (Col = HTB_LONG_RATE And Not m_IsAllowLending) Then Cancel = True
    If Not Cancel Then m_sLastValue = Trim(fgRates.TextMatrix(Row, Col))
End Sub

Private Sub fgRates_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim dValue As Double
    
    If Col <> HTB_PERIOD And Col <> HTB_SHORT_RATE And Col <> HTB_LONG_RATE Then Exit Sub
    
    Err.Clear
    dValue = CDbl(fgRates.EditText())
    
    If Col = HTB_PERIOD Then
        Cancel = CBool(dValue <= 0 Or Err.Number <> 0)
    Else
        Cancel = CBool(Err.Number <> 0)
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    m_bIsOpened = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    m_bIsOpened = False
    ClearData
End Sub

Private Sub PubChanges()
On Error GoTo Exception

    Dim aData As MSGSTRUCTLib.UnderlyingUpdate
    Set aData = New MSGSTRUCTLib.UnderlyingUpdate
    
    aData.UndID = m_ID
    aData.UpdStatus = enHTBRatesUpdated
    
    g_TradeChannel.PubUnderlyingUpdate aData
    
Exit Sub
Exception:
    gCmn.ErrorMsgBox m_frmOwner, "Unable To Pub HTB Changes. Please, restart application."
End Sub
