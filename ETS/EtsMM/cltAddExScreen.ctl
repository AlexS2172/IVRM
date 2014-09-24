VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Begin VB.UserControl cltAddExScreen 
   ClientHeight    =   3600
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4800
   ScaleHeight     =   3600
   ScaleWidth      =   4800
   Begin VB.Timer tmrInit 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   3360
      Top             =   240
   End
   Begin VSFlex8Ctl.VSFlexGrid flxFilter 
      Height          =   375
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   1455
      _cx             =   2566
      _cy             =   661
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
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
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
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
   End
   Begin VSFlex8UCtl.VSFlexGrid fgExerScr 
      Height          =   2415
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   4455
      _cx             =   7858
      _cy             =   4260
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
      Rows            =   50
      Cols            =   10
      FixedRows       =   1
      FixedCols       =   1
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
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
   End
End
Attribute VB_Name = "cltAddExScreen"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Public Enum ExerscrCols
    ESC_NONE = IDX_NONE
    ESC_KEY
    ESC_OptType
    ESC_UNDTYPE
    ESC_ACCOUNT
    ESC_GROUP
    ESC_UND
    ESC_OPTION
    ESC_STRIKE
    ESC_EXPIRY
    ESC_SOQ
    ESC_C_P
    ESC_QTY
    ESC_B_S
    ESC_UND_PRICE
    ESC_OPT_PRICE
    ESC_SIZE
    ESC_EXEC
    ESC_ATM
    ESC_LEAVE_AS_IT_IS
    ESC_IS_INDEX
    ESC_COLUMN_COUNT
    ESC_LAST_COLUMN = ESC_COLUMN_COUNT - 1
End Enum

Private Enum ExcScrFilter
    ESC_FLT_DATE = 0
    ESC_FLT_COLUMN_COUNT
    ESC_FLT_LAST_COLUMN = ESC_FLT_COLUMN_COUNT - 1
End Enum

Private m_nQuoteReqCount As Long
Private m_nQuoteReqDone As Long
Private m_nQuoteGroupReqCount As Long
Private m_nQuoteGroupReqDone As Long

Private m_ExpCollection As Collection

Private m_dtCurrentDate As Date
Private m_dtExpSelected As Date

Public m_PrPrProgressBar As ProgressBar
Public m_SattusBar As Label
Private m_Positions As New clsTradeByUndColl

Private m_ColOfNotEntedSOQ As Collection

Private m_QuoteRequests As New clsAddExScreenRequest

Private m_nLastQuoteGroupReqDone  As Integer

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPriceWithNotify
Private m_BatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
Public m_frmOwner As Form
Private m_bApplyFilter As Boolean

Public Sub Init(ByVal bApplyFilter)
    m_bShutDown = True
    
    Set m_ColOfNotEntedSOQ = Nothing
    Set m_ExpCollection = Nothing
    Set m_ExpCollection = New Collection
    
   If bApplyFilter Then
    m_dtExpSelected = DateSerial(Year(DateTime.Now), Month(DateTime.Now), Day(DateTime.Now))
    m_dtCurrentDate = m_dtExpSelected
   End If
    With flxFilter
        .rows = 2
        .Cols = ESC_FLT_COLUMN_COUNT
        .ColWidthMin = 2200
                        
        .ColDataType(ESC_FLT_DATE) = flexDTDate
        .ColFormat(ESC_FLT_DATE) = ""
        .TextMatrix(0, ESC_FLT_DATE) = "Expiry Date"
         
 '       AllowBigSelection = False
        .AllowSelection = False
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
        
'        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
        .AllowUserResizing = flexResizeColumns
        .Cell(flexcpAlignment, 0, 0, 1, ESC_FLT_LAST_COLUMN) = flexAlignCenterCenter
        
        .HighLight = flexHighlightWithFocus

    
    End With
    
    With fgExerScr
        .rows = 1
        .Cols = ESC_COLUMN_COUNT
        
        .ColHidden(ESC_KEY) = True
        .ColHidden(ESC_OptType) = True
        .ColHidden(ESC_UNDTYPE) = True
        
        .ColDataType(ESC_ACCOUNT) = flexDTString
        .ColFormat(ESC_ACCOUNT) = ""
        .TextMatrix(0, ESC_ACCOUNT) = "Account"
        
        .ColDataType(ESC_GROUP) = flexDTString
        .ColFormat(ESC_GROUP) = ""
        .TextMatrix(0, ESC_GROUP) = "Group"
        
        .ColDataType(ESC_UND) = flexDTString
        .ColFormat(ESC_UND) = ""
        .TextMatrix(0, ESC_UND) = "Und"
        
        .ColDataType(ESC_OPTION) = flexDTString
        .ColFormat(ESC_OPTION) = ""
        .TextMatrix(0, ESC_OPTION) = "Option"
        
        .ColDataType(ESC_STRIKE) = flexDTDouble
        .ColFormat(ESC_STRIKE) = "#,##0.00"
        .TextMatrix(0, ESC_STRIKE) = "Strike"
        
        .ColDataType(ESC_EXPIRY) = flexDTDate
        .ColFormat(ESC_EXPIRY) = "DD MMM YY"
        .TextMatrix(0, ESC_EXPIRY) = "Expiry"
        
        .ColDataType(ESC_SOQ) = flexDTLong
        .ColFormat(ESC_SOQ) = "#,##0.00"
        .TextMatrix(0, ESC_SOQ) = "SOQ"
    
        .ColDataType(ESC_C_P) = flexDTString
        .ColFormat(ESC_C_P) = ""
        .TextMatrix(0, ESC_C_P) = "C/P"
        
        .ColDataType(ESC_QTY) = flexDTLong
        .ColFormat(ESC_QTY) = "#,###"
        .TextMatrix(0, ESC_QTY) = "Qty"
        
        .ColDataType(ESC_B_S) = flexDTString
        .ColFormat(ESC_B_S) = ""
        .TextMatrix(0, ESC_B_S) = "B/S"
        
        .ColDataType(ESC_UND_PRICE) = flexDTDouble
        .ColFormat(ESC_UND_PRICE) = "#,##0.00"
        .TextMatrix(0, ESC_UND_PRICE) = "Und Price"
        
        .ColDataType(ESC_OPT_PRICE) = flexDTDouble
        .ColFormat(ESC_OPT_PRICE) = "#,##0.00"
        .TextMatrix(0, ESC_OPT_PRICE) = "Opt Price"
        
        .ColDataType(ESC_SIZE) = flexDTLong
        .ColFormat(ESC_SIZE) = "#,##0"
        .TextMatrix(0, ESC_SIZE) = "Size"
        
        .ColDataType(ESC_EXEC) = flexDTString
        .ColFormat(ESC_EXEC) = ""
        .TextMatrix(0, ESC_EXEC) = "Exerc"
        
        .ColDataType(ESC_LEAVE_AS_IT_IS) = flexDTString
        .ColFormat(ESC_LEAVE_AS_IT_IS) = ""
        .TextMatrix(0, ESC_LEAVE_AS_IT_IS) = "Leave as it"
        
        .ColDataType(ESC_ATM) = flexDTString
        .ColFormat(ESC_ATM) = ""
        .TextMatrix(0, ESC_ATM) = "ATM"
        
        .ColDataType(ESC_IS_INDEX) = flexDTLong
        .ColFormat(ESC_IS_INDEX) = ""
        .TextMatrix(0, ESC_IS_INDEX) = "Is Index"
        
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
        .Cell(flexcpAlignment, 0, 1, 0, ESC_LAST_COLUMN) = flexAlignCenterCenter
        
        .HighLight = flexHighlightWithFocus
        .ColHidden(ESC_IS_INDEX) = True
        
        .FocusRect = flexFocusHeavy
    End With
    
    m_SattusBar.Caption = ""
    
    'Col(ESC_ACCOUNT).Init "Account", "Account", "", flexDTString, True, False, True, False
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequest = False
    
    m_bGroupRequestSupported = g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    Set m_BatchPriceProvider = PriceProvider
    If m_BatchPriceProvider Is Nothing Then LogEvent EVENT_ERROR, "Selected price provider is not compatible with Add Excesize view."
    
    m_bApplyFilter = bApplyFilter
    tmrInit.Enabled = True
End Sub

Public Sub AutoSizeGrig()
    With fgExerScr
        .AutoSize 0, .Cols - 1
    End With
End Sub

Private Function IsItIndex(ByVal iRow As Integer) As Boolean
    On Error Resume Next
    Dim iValue&
    iValue = CLng(fgExerScr.TextMatrix(iRow, ESC_IS_INDEX))
    IsItIndex = IIf(iValue > 0, True, False)
End Function

Private Sub EditPrice(ByVal Row As Long, ByVal lAutomaticCall As Integer)
    On Error Resume Next
    If Row > 0 Then
        Dim dUndPrice As Double, dPrice  As Double, dOoOptPrice As Double, dSOQ As Double
        dUndPrice = CDbl(fgExerScr.TextMatrix(Row, ESC_UND_PRICE))
        dStrike = CDbl(fgExerScr.TextMatrix(Row, ESC_STRIKE))
        dOoOptPrice = CDbl(fgExerScr.TextMatrix(Row, ESC_OPT_PRICE))
        
        Dim bCall As Boolean, bATM As Boolean
        
        bCall = IIf(fgExerScr.TextMatrix(Row, ESC_C_P) = "C", True, False)
            
        If IsItIndex(Row) Then
            'Dim sSym As String
            'sSym = fgExerScr.TextMatrix(Row, ESC_UND)
            If fgExerScr.TextMatrix(Row, ESC_SOQ) <> "--" Then
                dSOQ = CDbl(fgExerScr.TextMatrix(Row, ESC_SOQ))
            Else
                dSOQ = dUndPrice
            End If
            
            If dSOQ <= 0 Then
                dSOQ = dUndPrice
            End If
                
            If bCall Then
                If dSOQ - dStrike > 0 Then
                    fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = IIf(dSOQ - dStrike > 0, dSOQ - dStrike, 0)
                End If
            Else
                If dStrike - dSOQ > 0 Then
                    fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = IIf(dStrike - dSOQ > 0, dStrike - dSOQ, 0)
                End If
            End If
            'dStrike = dStrike + CDbl(fgExerScr.TextMatrix(Row, ESC_SOQ))
        Else
            If lAutomaticCall = 1 Then
                fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = "0"
            End If
        End If
                
        If dUndPrice < 0 Then
            dUndPrice = 0
            fgExerScr.TextMatrix(Row, ESC_UND_PRICE) = 0
        End If
        
        If IsItIndex(Row) Then
            dUndPrice = dSOQ
        End If
                    
        If dStrike >= 0 Then
        
            If bCall Then
                 If dStrike < dUndPrice Then bATM = True
            Else
                 If dStrike > dUndPrice Then bATM = True
            End If
                                  
            If bATM Then
                fgExerScr.Cell(flexcpChecked, Row, ESC_EXEC, Row, ESC_EXEC) = flexChecked
            Else
                fgExerScr.Cell(flexcpChecked, Row, ESC_EXEC, Row, ESC_EXEC) = flexUnchecked
                If lAutomaticCall = 1 Then
                    fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = 0
                End If
            End If
            
            If dStrike = dUndPrice Then
                fgExerScr.TextMatrix(Row, ESC_ATM) = "Y"
            Else
                fgExerScr.TextMatrix(Row, ESC_ATM) = ""
            End If
        Else
            fgExerScr.Cell(flexcpChecked, Row, ESC_EXEC, Row, ESC_EXEC) = flexNoCheckbox
        End If
                
    End If
End Sub

Private Sub fgExerScr_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Select Case Col
        Case ESC_SOQ
            Dim aSOQ As Double, aStrike As Double, sCP As String
            aSOQ = CDbl(fgExerScr.TextMatrix(Row, Col))
            If aSOQ > 0 Then
                aStrike = fgExerScr.TextMatrix(Row, ESC_STRIKE)
                sCP = fgExerScr.TextMatrix(Row, ESC_C_P)
                If sCP = "C" Then
                    If aSOQ - aStrike > 0 Then
                        fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = aSOQ - aStrike
                    Else
                        fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = 0
                    End If
                Else
                    If aStrike - aSOQ > 0 Then
                        fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = aStrike - aSOQ
                    Else
                        fgExerScr.TextMatrix(Row, ESC_OPT_PRICE) = 0
                    End If
                End If
                fgExerScr.Cell(flexcpForeColor, Row, ESC_SOQ, Row, ESC_SOQ) = RGB(3, 3, 3)
            'Else
                'fgExerScr.TextMatrix(Row, ESC_STRIKE) = 0
            End If
            fgExerScr.TextMatrix(Row, Col) = aSOQ
            EditPrice Row, 0
          
            If Not m_ColOfNotEntedSOQ Is Nothing Then
                RemoveIndex (Row)
                
                If m_ColOfNotEntedSOQ.Count = 0 Then
                    m_SattusBar.Caption = " "
                End If
            End If
        Case ESC_UND_PRICE
            EditPrice Row, 0
        Case ESC_OPT_PRICE
            EditPrice Row, 0
    End Select
End Sub

Private Function IsIndexAdded(Row As Integer) As Boolean
    Dim iIdx As Integer, iCur As Integer
    IsIndexAdded = False
    For iIdx = 1 To m_ColOfNotEntedSOQ.Count
        iCur = m_ColOfNotEntedSOQ(iIdx)
        If iCur = Row Then
            IsIndexAdded = True
            Exit For
        End If
    Next
End Function

Private Function IsExpiryAdded(Exp As Date) As Boolean
    Dim iIdx As Integer, dtCur As Date
    IsExpiryAdded = False
    For iIdx = 1 To m_ExpCollection.Count
        dtCur = m_ExpCollection(iIdx)
        If dtCur = Exp Then
            IsExpiryAdded = True
            Exit For
        End If
    Next
End Function


Private Sub RemoveIndex(Row As Integer)
    Dim iIdx As Integer, iCur As Integer
    If Not m_ColOfNotEntedSOQ Is Nothing Then
        For iIdx = 1 To m_ColOfNotEntedSOQ.Count
            iCur = m_ColOfNotEntedSOQ(iIdx)
            If iCur = Row Then
                m_ColOfNotEntedSOQ.Remove iIdx
                Exit For
            End If
        Next
    End If
End Sub

Private Sub fgExerScr_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Cancel = True
    Select Case Col
        Case ESC_OPT_PRICE
            Cancel = False
        Case ESC_SOQ
            Cancel = False
            If IsItIndex(Row) = False Then Cancel = True
        Case ESC_EXEC
            Cancel = False
        Case ESC_UND_PRICE
            Cancel = False
        Case ESC_LEAVE_AS_IT_IS
            Cancel = False
        End Select
End Sub

Private Sub flxFilter_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    FilterUpdate
End Sub

Private Sub flxFilter_LostFocus()
    FilterUpdate
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next

    
    Dim iRespSymbol As Integer
    iRespSymbol = ESC_UND
    'If Params.Type = enFUT Then
        'iRespSymbol = ESC_OPTION
'    End If
    
    For iRowIterator = 1 To fgExerScr.rows
        Dim strUnd As String
        strUnd = fgExerScr.TextMatrix(iRowIterator, iRespSymbol)
        If strUnd = Request.Symbol Then
            fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE) = "--"
        End If
    Next
        
    Dim bFinished As Boolean
    bFinished = False
    m_nQuoteReqDone = m_nQuoteReqDone + 1
    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount)
    If bFinished = True Then
        CallFinish
    End If
End Sub

Private Sub CallFinish()
    On Error Resume Next
    Dim iRow As Long
    m_PrPrProgressBar.Visible = False
    m_SattusBar.Visible = True
    
    For iRow = 1 To fgExerScr.rows
        EditPrice iRow, 1
    Next
    
    m_SattusBar.Caption = ""
    UpdateStatusBarLabel
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
   ' If m_bShutDown Or Not m_bLastQuoteReqNow Then Exit Sub
   
   Dim dLastPrice As Double: dLastPrice = Results.LastPrice
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnLastQuote Started Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption
    m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
    m_PrPrProgressBar.Value = m_PrPrProgressBar.Value + 1
    
    
    Dim iRespSymbol As Integer
    iRespSymbol = ESC_UND
'    If Params.Type = enFOP Then
'        iRespSymbol = ESC_OPTION
'    End If
    
    Dim iRowIterator As Integer
    iRowIterator = 1
    For iRowIterator = 1 To fgExerScr.rows - 1
    
        Results.LastPrice = dLastPrice
    
        Dim strUnd As String
        strUnd = fgExerScr.TextMatrix(iRowIterator, iRespSymbol)
        Dim aRowTrdByUnd  As clsTradeByUndAtom
        Set aRowTrdByUnd = fgExerScr.RowData(iRowIterator)
        Dim indAdd As Integer
        indAdd = aRowTrdByUnd.IsSymbolExist(Params.Symbol)
'        If strUnd = Params.Symbol And Results.LastPrice > 0 Then
'            fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE) = Results.LastPrice
'        End If
        If indAdd > 0 Then
            
            If aRowTrdByUnd.m_IsSynth = False Then
                fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE) = Results.LastPrice
            Else
                Dim SynRootComp As SynthRootCompAtom, dPriceLoop As Double
                Dim aUnd As UndAtom, aUndLoop As UndAtom
                Set aUnd = g_Underlying.BySortKey(Params.Symbol)
                
EndLoop:
                If Not aUnd Is Nothing Then
                    Set SynRootComp = aRowTrdByUnd.m_SyntRootAtom.SynthRootComponents(aUnd.ID)
                    If fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE) <> "" Then
                        dPriceLoop = fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE)
                    Else
                        dPriceLoop = aRowTrdByUnd.m_SyntRootAtom.CashValue
                    End If
                    dPriceLoop = dPriceLoop + SynRootComp.Weight * Results.LastPrice
                    
                    fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE) = dPriceLoop
                    Results.LastPrice = dPriceLoop
                    aRowTrdByUnd.m_UndColl.Remove (indAdd)
                End If
            End If
            
            Dim bATM As Boolean
            
            Dim dPrice As Double
            dPrice = CDbl(fgExerScr.TextMatrix(iRowIterator, ESC_UND_PRICE))
            
            If dPrice >= 0 Then
                If dPrice = Results.LastPrice Then
                    fgExerScr.Cell(flexcpChecked, iRowIterator, ESC_EXEC, iRowIterator, ESC_EXEC) = flexChecked
                Else
                    fgExerScr.Cell(flexcpChecked, iRowIterator, ESC_EXEC, iRowIterator, ESC_EXEC) = flexUnchecked
                End If
                
                Dim bCall As Boolean
                bCall = IIf(fgExerScr.TextMatrix(iRowIterator, ESC_C_P) = "C", True, False)
                If bCall Then
                    If dPrice > Results.LastPrice Then bATM = True
                Else
                    If dPrice < Results.LastPrice Then bATM = True
                End If
                   
                If IsItIndex(iRowIterator) Then
                    Dim dSOQ As Double
                    dSOQ = fgExerScr.TextMatrix(iRowIterator, ESC_SOQ)
                    If dSOQ <= 0 Then
                        'fgExerScr.TextMatrix(iRowIterator, ESC_SOQ) = IIf(Results.LastPrice > 0, Results.LastPrice, 0)
                        fgExerScr.Cell(flexcpForeColor, iRowIterator, ESC_SOQ, iRowIterator, ESC_SOQ) = RGB(255, 3, 3)
                        'm_SattusBar.Caption = "Start opening quotation prices are missing for some indices. Please enter proper SOQ price or last index price will be used insted of SOQ"
                        If m_ColOfNotEntedSOQ Is Nothing Then
                            Set m_ColOfNotEntedSOQ = New Collection
                        End If
                        If fgExerScr.RowHidden(iRowIterator) = False Then
                            If IsIndexAdded(iRowIterator) = False Then
                                m_ColOfNotEntedSOQ.Add iRowIterator
                            End If
                        End If
                        EditPrice iRowIterator, IIf(Results.LastPrice > 0, Results.LastPrice, 0)
                    Else
                        If Not m_ColOfNotEntedSOQ Is Nothing Then
                            RemoveIndex (iRowIterator)
                            If m_ColOfNotEntedSOQ.Count = 0 Then
                                m_SattusBar.Caption = ""
                            End If
                        End If
                        EditPrice iRowIterator, 0
                    End If
                Else
                    EditPrice iRowIterator, 0
                End If
                
            Else
                fgExerScr.Cell(flexcpChecked, iRowIterator, ESC_EXEC, iRowIterator, ESC_EXEC) = flexNoCheckbox
            End If
            
        End If
    Next
        
    Dim bFinished As Boolean
    bFinished = False
    m_nQuoteReqDone = m_nQuoteReqDone + 1
    bFinished = (m_nQuoteReqDone = m_nQuoteReqCount)
    If bFinished = True Then
        CallFinish
    End If
End Sub
Private Sub PriceProvider_OnSubscribed(Params As PRICEPROVIDERSLib.QuoteUpdateParams)
    Dim bFinished As Boolean, sKey$
    
    If PpIsNonGroupReqType(Params.Type) Then
        sKey = CStr(PpInstrumentTypeContractType(Params.Type)) & "_" & Params.Symbol
        If Len(Params.Exchange) > 0 Then
            sKey = sKey & "." & Params.Exchange
        End If
        
        If Not m_AuxClc.QuoteReqsAll(sKey) Is Nothing Then
            If Params.Type <> enOPT And m_bGroupRequest Then
                m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
            End If
            
            If Params.Type = enSTK Or Params.Type = enIDX Then
                m_nUndResponses = m_nUndResponses + 1
                
            ElseIf Params.Type = enFUT Then
                m_nFutResponses = m_nFutResponses + 1
                
            Else
                m_nOptResponses = m_nOptResponses + 1
            End If
            
            m_nQuoteReqDone = m_nQuoteReqDone + 1
            IncProgress m_PrPrProgressBar
            
            bFinished = (m_nQuoteReqDone = m_nQuoteReqCount _
                        Or m_bGroupRequest And m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
        End If
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nQuoteGroupReqDone = m_nQuoteGroupReqDone + 1
        bFinished = (m_nQuoteGroupReqCount = m_nQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bSubscribingNow = False
        m_nQuoteReqDone = 0&
        m_nQuoteReqCount = 0&
        m_nQuoteGroupReqDone = 0&
        m_nQuoteGroupReqCount = 0&
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.FinishLogMmOperation m_nOperation, OPER_SUBSCRIBEQUOTE, m_frmOwner.GetCaption, m_nUndResponses, m_nOptResponses, m_nFutResponses
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        m_Aux.RealTime = True
        m_Aux.RealTimeConnected = True
        If m_BatchPriceProvider.IsQuotesUpdated Or m_bVolaUpdated Then tmrRealTime.Enabled = True
        
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "PriceProvider_OnSubscribed Finished Symbol: " & Params.Symbol & ", Exchange:  " & Params.Exchange & ", Type:  " & Params.Type, m_frmOwner.GetCaption
End Sub

Private Sub tmrInit_Timer()
On Error Resume Next
    tmrInit.Enabled = False
    LoadScreen m_bApplyFilter
    RequestLastQuotes
    AutoSizeGrig
End Sub

Private Sub UserControl_Resize()
 '   Dim lH As Integer, lW As Integer
 '   lH = m_PrPrProgressBar.Height
 '   lW = m_PrPrProgressBar.Width
    
    With flxFilter
        .Left = 0
        .Top = 20 + GRID_VERT_SPACE
        .Width = ScaleWidth
        .Height = 480
        .Refresh
    End With
    
    With fgExerScr
        .Left = 0
        .Top = 520 + GRID_VERT_SPACE
        .Width = ScaleWidth
        Dim lScH As Long
        lScH = ScaleHeight - .Top - 20
        lScH = IIf(lScH > 0, lScH, 0)
        .Height = lScH
        .Refresh
    End With
'    m_PrPrProgressBar.Move 100, ScaleHeight - fgExerScr.Top - 2 * lH, lW, lH
End Sub

Private Function GetColKeyFromUnd(aUnd As UndAtom) As String
    On Error Resume Next
    GetColKeyFromUnd = ""
    GetColKeyFromUnd = CStr(IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex)) & "_" & aUnd.Symbol
End Function

Private Function GetIdByTrade(aTrd As MmTradeInfoAtom) As String
    GetIdByTrade = CStr(aTrd.UndID) & "_" & "_" & aTrd.Symbol & "_"
            
    If Not aTrd.Strategy Is Nothing Then
        GetIdByTrade = GetIdByTrade + CStr(aTrd.Strategy.ID)
    End If
    If Not aTrd.Trader Is Nothing Then
        GetIdByTrade = GetIdByTrade + CStr(aTrd.Trader.ID)
    End If
End Function

Private Sub LoadScreen(ByVal bApplyFilter As Boolean)
    Dim aTrd As MmTradeInfoAtom
    Dim aTrdColl As EtsGeneralLib.MmTradeInfoColl
    On Error GoTo Ex
    m_Positions.Clear
    
    Dim dtCur As Date
    dtCur = DateTime.Now
        
    m_QuoteRequests.Clear
    
    Set aTrdColl = g_TradeChannel.Trades.Clone
    
    m_SattusBar.Caption = "Data Loading"
    m_PrPrProgressBar.Visible = True
    m_PrPrProgressBar.Min = 0
    m_PrPrProgressBar.Value = 0
    m_PrPrProgressBar.Max = aTrdColl.Count + 1
    
    For Each aTrd In aTrdColl 'g_TradeChannel.Trades
        'Dim bAdd As Boolean
        'bAdd = True
        m_PrPrProgressBar.Value = m_PrPrProgressBar.Value + 1
        Dim aSyntRoot As SynthRootColl
        Dim aSynthAtom As SynthRootAtom
        Set aSyntRoot = Nothing
        Set aSynthAtom = Nothing
        If Not aTrd.OptRoot Is Nothing Then
            If aTrd.OptRoot.IsSynth = True Then 'Not aTrd.OptRoot Is Nothing Then
                If Not aTrd.Und.SyntheticRoots Is Nothing Then
                    Set aSyntRoot = aTrd.Und.SyntheticRoots
                    Set aSynthAtom = aSyntRoot(aTrd.OptRootID)
                End If
            End If
        End If
        
        Dim aReq As PRICEPROVIDERSLib.QuoteUpdateParams
        
        If aTrd.ContractType = enCtStock Or aTrd.ContractType = enCtOption Then
            aReq.Symbol = aTrd.Und.Symbol
            aReq.Type = enSTK
            If m_QuoteRequests.WasRequestAdded(aReq) = False Then
                m_QuoteRequests.AddRequest aReq
            End If
        End If
                
        Dim lKey As Long
        If Not aTrd.Opt Is Nothing Then
            If IsExpiryAdded(aTrd.Opt.Expiry) = False Then
                lKey = CLng(aTrd.Opt.Expiry)
                m_ExpCollection.Add aTrd.Opt.Expiry, CStr(-lKey)
            End If
        ElseIf Not aTrd.FutOpt Is Nothing Then
            If IsExpiryAdded(aTrd.FutOpt.Expiry) = False Then
                lKey = CLng(aTrd.FutOpt.Expiry)
                m_ExpCollection.Add aTrd.FutOpt.Expiry, CStr(-lKey)
            End If
        End If
        
        'If bAdd Then
            Dim aTrdByUnd As clsTradeByUndAtom, sKey$
            sKey = CStr(aTrd.UndID) & "_" & "_" & aTrd.Symbol & "_"
            
            If Not aTrd.Strategy Is Nothing Then
                sKey = sKey + CStr(aTrd.Strategy.ID)
            End If
            If Not aTrd.Trader Is Nothing Then
                sKey = sKey + CStr(aTrd.Trader.ID)
            End If
            
            On Error Resume Next
            Set aTrdByUnd = m_Positions(sKey)
            Err.Clear
            On Error GoTo Ex
            
            If aTrdByUnd Is Nothing Then
                DoEvents

                Set aTrdByUnd = New clsTradeByUndAtom
                Set aTrdByUnd.m_UndColl = New Collection
                m_Positions.Add sKey, aTrdByUnd
                aTrdByUnd.m_UndColl.Add aTrd.Symbol
                Set aTrdByUnd.m_SyntRootAtom = aSynthAtom
                If Not aSynthAtom Is Nothing Then
                    'Dim sKey1 As String
                    Dim SynRootComp As SynthRootCompAtom
                    aTrdByUnd.m_IsSynth = True
                    Dim aUnd1 As UndAtom
                    For Each SynRootComp In aSynthAtom.SynthRootComponents
                        Set aUnd1 = g_Underlying(SynRootComp.UndID)
                        If Not aUnd1 Is Nothing Then

                            Dim aReq1 As PRICEPROVIDERSLib.QuoteUpdateParams
                            aReq1.Symbol = aUnd1.Symbol
                            aReq1.Type = enSTK
                                
                            If m_QuoteRequests.WasRequestAdded(aReq1) = False Then
                                m_QuoteRequests.AddRequest aReq1
                            End If
                            aTrdByUnd.m_UndColl.Add (aUnd1.Symbol)
                        End If
                    Next
                Else
                    aTrdByUnd.m_IsSynth = False
                End If
            End If
            
            aTrdByUnd.Trd.Add aTrd.TradeID, aTrd.TradeDate, aTrd
        'End If
    Next
    Dim nRow As Integer
    nRow = 1
    For Each aTrdByUnd In m_Positions
        
        If Not aTrdByUnd Is Nothing Then
            Dim bAddToPR As Boolean
            bAddToPR = True
            
            On Error GoTo Ex1
Ex1:
            On Error GoTo Ex
            If bAddToPR Then
                fgExerScr.AddItem ""
                fgExerScr.RowHidden(nRow) = True
                fgExerScr.TextMatrix(nRow, ESC_QTY) = 0
                UpdateExScreen aTrdByUnd, nRow
                nRow = nRow + 1
            End If
        End If
    Next
    Dim nRows As Integer
    'Dim sKeyS As String
    nRows = fgExerScr.rows - 1

    m_SattusBar.Caption = "Filter Applying"
    m_SattusBar.Refresh
    m_PrPrProgressBar.Visible = True
    m_PrPrProgressBar.Min = 0
    m_PrPrProgressBar.Value = 0
    m_PrPrProgressBar.Max = nRows + 1

    For nRow = 1 To nRows
        Dim aQtyTmp As Integer
        'On Error Resume Next
        m_PrPrProgressBar.Value = m_PrPrProgressBar.Value + 1
        DoEvents
        aQtyTmp = fgExerScr.TextMatrix(nRow, ESC_QTY)
        If aQtyTmp <> 0 Then
            'fgExerScr.RowHidden(nRow) = False
            Dim aTrdByUndRow As clsTradeByUndAtom
            Set aTrdByUndRow = fgExerScr.RowData(nRow)
            If aTrdByUndRow.m_IsSynth = False Then
                
                aReq.Symbol = fgExerScr.TextMatrix(nRow, ESC_UND)
                Dim nColType1 As Integer
                nColType1 = fgExerScr.TextMatrix(nRow, ESC_UNDTYPE)
                If nColType1 = enCtIndex Then
                    aReq.Type = enIDX
                ElseIf nColType1 = enCtFutOption Then
                    aReq.Type = enFUT
                   ' aReq.Symbol = fgExerScr.TextMatrix(nRow, ESC_UND)
                Else
                    aReq.Type = enSTK
                End If
                If m_QuoteRequests.WasRequestAdded(aReq) = False Then
                    m_QuoteRequests.AddRequest aReq
                End If
                aTrdByUndRow.m_UndColl.Add (aReq.Symbol)
            End If
        Else
            'sKeyS = fgExerScr.TextMatrix(nRow, ESC_KEY)
            'aColRowsForRemove.Add sKeyS
            'fgExerScr.RemoveItem (nRow)
'            fgExerScr.RowHidden(nRow) = True
'            nRow = nRow - 1
'            nRows = nRows - 1
        End If
    Next

    nRow = 1
    While nRow < fgExerScr.rows
        aQtyTmp = fgExerScr.TextMatrix(nRow, ESC_QTY)
        If aQtyTmp = 0 Then
            fgExerScr.RemoveItem nRow
            nRow = 1
        Else
            nRow = nRow + 1
        End If
    Wend
Ex:
    
    Debug.Assert Err.Number = 0
    If nRow = 1 Then
        m_PrPrProgressBar.Visible = False
    End If
    
    FilterLoad bApplyFilter
    FilterUpdate
    
'    If bApplyFilter = True Then
'        FilterUpdate
'    End If

    Set aTrdColl = Nothing
    
    Exit Sub
End Sub

Private Function FindRowByKey(sKeyS As String) As Long
    FindRowByKey = -1
    For nRow = 1 To fgExerScr.rows - 1
        If fgExerScr.TextMatrix(nRow, ESC_KEY) = sKeyS Then
            FindRowByKey = nRow
            Exit For
        End If
    Next
End Function

Private Sub UpdateExScreen(ByRef aTrdByUnd As clsTradeByUndAtom, nRow As Integer)
    Dim iCol As Integer
    Dim aTrdAt As EtsGeneralLib.MmTradeInfoAtom
        
    For Each aTrdAt In aTrdByUnd.Trd
        fgExerScr.RowData(nRow) = aTrdByUnd
        With fgExerScr
            Dim bExex As Boolean
            bExec = True
            If aTrdAt.Opt Is Nothing And aTrdAt.FutOpt Is Nothing Then
                bExec = False
            End If
            'ElseIf Not aTrdAt.Opt Is Nothing Then
            '    If aTrdAt.Opt.Expiry > dtCur Then
            '        bExec = False
            '    End If
            'ElseIf Not aTrdAt.FutOpt Is Nothing Then
            '    If aTrdAt.FutOpt.Expiry > dtCur Then
            '        bExec = False
            '    End If
            'End If
            

            If bExec Then
                For iCol = 0 To ESC_COLUMN_COUNT
                  Select Case iCol
                    Case ESC_KEY
                        .TextMatrix(nRow, iCol) = GetIdByTrade(aTrdAt)
                    Case ESC_UNDTYPE
                        If aTrdAt.Fut Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.Und.UndType
                        Else
                            .TextMatrix(nRow, iCol) = enCtFutOption
                        End If
                     Case ESC_OptType
                        .TextMatrix(nRow, iCol) = aTrdAt.ContractType
                     Case ESC_ACCOUNT
                         .TextMatrix(nRow, iCol) = aTrdAt.Trader.Name
                     Case ESC_GROUP
                        If Not aTrdAt.Strategy Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.Strategy.Name
                        End If
                     Case ESC_UND
                        If aTrdAt.Fut Is Nothing Then
                             .TextMatrix(nRow, iCol) = aTrdAt.Und.Symbol
                        Else
                            .TextMatrix(nRow, iCol) = aTrdAt.Fut.Symbol
                        End If
                     Case ESC_OPTION
                        'If Not aTrdAt.Opt Is Nothing Then
                            If Not aTrdAt.Opt Is Nothing Then
                                .TextMatrix(nRow, iCol) = aTrdAt.Opt.Symbol
                            ElseIf Not aTrdAt.FutOpt Is Nothing Then
                                .TextMatrix(nRow, iCol) = aTrdAt.FutOpt.Symbol
                            End If
                        'End If
                     Case ESC_STRIKE
                        If Not aTrdAt.Opt Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.Opt.Strike
                        ElseIf Not aTrdAt.FutOpt Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.FutOpt.Strike
                        End If
                     Case ESC_EXPIRY
                        If Not aTrdAt.Opt Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.Opt.Expiry
'                            If m_dtExpSelected > aTrdAt.Opt.Expiry Or m_dtExpSelected = m_dtCurrentDate Then
'                                If aTrdAt.Opt.Expiry >= m_dtCurrentDate Then
'                                    m_dtExpSelected = aTrdAt.Opt.Expiry
'                                End If
'                            End If
                        ElseIf Not aTrdAt.FutOpt Is Nothing Then
                            .TextMatrix(nRow, iCol) = aTrdAt.FutOpt.Expiry
'                            If m_dtExpSelected > aTrdAt.FutOpt.Expiry Or m_dtExpSelected = m_dtCurrentDate Then
'                                If aTrdAt.FutOpt.Expiry >= m_dtCurrentDate Then
'                                    m_dtExpSelected = aTrdAt.FutOpt.Expiry
'                                End If
'                            End If
                        End If
                     Case ESC_SOQ
                        If aTrdAt.Und.UndType = enCtIndex And aTrdAt.Fut Is Nothing Then
                            .TextMatrix(nRow, iCol) = IIf(aTrdAt.Und.SOQ <> 0, aTrdAt.Und.SOQ, "--")
                            .TextMatrix(nRow, ESC_IS_INDEX) = 1
                        Else
                            .TextMatrix(nRow, iCol) = "--"
                            .TextMatrix(nRow, ESC_IS_INDEX) = 0
                        End If
                     Case ESC_C_P
                        If aTrdAt.ContractType = enCtOption Then
                            If Not aTrdAt.Opt Is Nothing Then
                                .TextMatrix(nRow, iCol) = IIf(aTrdAt.Opt.OptType = enOtCall, "C", "P")
                            End If
                        ElseIf aTrdAt.ContractType = enCtFutOption Then
                            If Not aTrdAt.FutOpt Is Nothing Then
                                .TextMatrix(nRow, iCol) = IIf(aTrdAt.FutOpt.OptType = enOtCall, "C", "P")
                            End If
                        End If
                     Case ESC_QTY
                        Dim aQty As Double
                        If aTrdAt.IsBuy Then
                            aQty = .TextMatrix(nRow, iCol) + Abs(aTrdAt.Quantity)
                        Else
                            aQty = .TextMatrix(nRow, iCol) - Abs(aTrdAt.Quantity)
                        End If
                        .TextMatrix(nRow, iCol) = aQty
                     Case ESC_B_S
                     Case ESC_UND_PRICE
                     Case ESC_OPT_PRICE
                        If aTrdAt.Und.UndType = enCtIndex And aTrdAt.Fut Is Nothing Then
                            Dim dOptPrice As Double
                            If Not aTrdAt.Opt Is Nothing Then
                                dOptPrice = IIf(aTrdAt.Opt.OptType = enOtCall, aTrdAt.Und.SOQ - aTrdAt.Opt.Strike, aTrdAt.Opt.Strike - aTrdAt.Und.SOQ)
                            ElseIf Not aTrdAt.FutOpt Is Nothing Then
                                dOptPrice = IIf(aTrdAt.FutOpt.OptType = enOtCall, aTrdAt.Und.SOQ - aTrdAt.FutOpt.Strike, aTrdAt.FutOpt.Strike - aTrdAt.Und.SOQ)
                            End If
                            If dOptPrice >= 0 Then
                                .TextMatrix(nRow, iCol) = dOptPrice
                            Else
                                .TextMatrix(nRow, iCol) = 0
                            End If
                        Else
                            .TextMatrix(nRow, iCol) = 0
                        End If
                     Case ESC_SIZE
                            .TextMatrix(nRow, iCol) = aTrdAt.LotSize
                     Case ESC_EXEC
                        .Cell(flexcpChecked, nRow, iCol, nRow, iCol) = flexUnchecked
                     Case ESC_ATM
                    End Select
                Next
            End If
        End With
    Next
    Dim aQtyTotal As Double
    aQtyTotal = fgExerScr.TextMatrix(nRow, ESC_QTY)
    If aQtyTotal > 0 Then
        fgExerScr.TextMatrix(nRow, ESC_B_S) = "B"
    Else
        fgExerScr.TextMatrix(nRow, ESC_QTY) = -aQtyTotal
        fgExerScr.TextMatrix(nRow, ESC_B_S) = "S"
    End If
    fgExerScr.Cell(flexcpChecked, nRow, ESC_LEAVE_AS_IT_IS, nRow, ESC_LEAVE_AS_IT_IS) = flexUnchecked
End Sub


Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim nCount&
    Dim qup As QuoteUpdateParams
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        
            
        nCount = m_QuoteRequests.Count
        m_nQuoteReqDone = 0
        m_nQuoteReqCount = nCount
        If nCount > 0 Then
            m_nLastQuoteGroupReqDone = 0
            m_bInProc = True
            m_bLastQuoteReqNow = True
            
            m_nLastQuoteReqCount = nCount
            m_nLastQuoteReqDone = 0&

            m_PrPrProgressBar.Min = 0
            m_PrPrProgressBar.Value = 0
            m_PrPrProgressBar.Max = m_nLastQuoteReqCount
            m_PrPrProgressBar.Visible = True

            DoEvents

            aParam.Type = enMStart
            PriceProvider.RequestLastQuote aParam
            
            If Not m_bGroupRequestSupported Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                qup.Exchange = ""
                qup.Symbol = ""
                qup.Type = enMStart
                PriceProvider.RequestLastQuote qup
                
                'For Each clsI In m_QuoteRequests
                Dim iIdx As Integer
                For iIdx = 1 To nCount
                    aParam = m_QuoteRequests.GetItem(iIdx)
                    If Not m_bLastQuoteReqNow Then Exit For
                    PriceProvider.RequestLastQuote aParam
                    DoEvents
                Next
                
                qup.Type = enMStop
                PriceProvider.RequestLastQuote qup
            Else
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                
                For Each aReq In m_QuoteRequests
                    If Not m_bLastQuoteReqNow Then Exit For
                        
                    aParam = aReq.GetQuoteUpdateParam
                    If aParam.Type = enSTK Or aParam.Type = enIDX Then
                        PriceProvider.RequestLastQuote aParam
                        m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                        DoEvents
                    
                        If Not m_bLastQuoteReqNow Then Exit For
                        
                        If aReq.Und.Opt.Count > 0 Then
                            aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                            DoEvents
                        End If
                    End If
                Next
            End If

            m_bInProc = False
        Else
            m_PrPrProgressBar.Visible = False
        End If
    Else
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
        
    PriceProvider.CancelLastQuote
    If m_bGroupRequestSupported Then
        m_GroupPriceProvider.CancelGroup
    End If
End Sub

Public Function ExecteTrade() As Boolean
    Dim nRow As Integer, nCount As Integer
    ExecteTrade = True
    nCount = fgExerScr.rows - 1
    For nRow = 1 To nCount
        On Error GoTo ExLoop1
        
        If fgExerScr.RowHidden(nRow) = False And fgExerScr.Cell(flexcpChecked, nRow, ESC_LEAVE_AS_IT_IS, nRow, ESC_LEAVE_AS_IT_IS) = flexUnchecked Then
        
            Dim aRowTrdByUnd As clsTradeByUndAtom
            Dim clsaTrd As clsTradeExecAtom
            Dim CntType As EtsContractTypeEnum, iCntType As Integer
            
            Set clsaTrd = New clsTradeExecAtom
            Set clsaTrd.Trd = New EtsGeneralLib.MmTradeInfoAtom
            clsaTrd.Trd.Status = enTrsRealtime
            Dim bExec As Boolean
            bExec = False
            Dim varCh As Variant
            
            Dim bHide  As Boolean
            bHide = fgExerScr.RowHidden(nRow)
            
            varCh = fgExerScr.Cell(flexcpChecked, nRow, ESC_EXEC, nRow, ESC_EXEC)
            If varCh <> 0 And bHide = False Then
              If varCh = 1 Then bExec = True
              fgExerScr.RowHidden(nRow) = True
              Set aRowTrdByUnd = fgExerScr.RowData(nRow)
            
              On Error Resume Next
              Set clsaTrd.Trd = New MmTradeInfoAtom
              iCntType = CInt(fgExerScr.TextMatrix(nRow, ESC_OptType))
        
              Dim aSOQ As Double, aStrike As Double, sBS As String, aQtyTmp As Integer
              If IsItIndex(nRow) Then
                  aSOQ = CDbl(fgExerScr.TextMatrix(nRow, ESC_SOQ))
              End If
              
              aStrike = CDbl(fgExerScr.TextMatrix(nRow, ESC_STRIKE))
              sBS = fgExerScr.TextMatrix(nRow, ESC_B_S)
          
              aQtyTmp = CInt(fgExerScr.TextMatrix(nRow, ESC_QTY))
              clsaTrd.Trd.Quantity = aQtyTmp
              clsaTrd.QtyToExec = aQtyTmp
              clsaTrd.Trd.IsBuy = IIf(sBS = "B", True, False)
              
              If bExec Then
                  clsaTrd.Trd.price = CDbl(fgExerScr.TextMatrix(nRow, ESC_OPT_PRICE))
                  clsaTrd.UndTradePrice = CDbl(fgExerScr.TextMatrix(nRow, ESC_STRIKE))
              End If
              On Error GoTo ExLoop1
              
              Dim aTrdAt As MmTradeInfoAtom
              For Each aTrdAt In aRowTrdByUnd.Trd
                  CntType = iCntType
                     
                  Set clsaTrd.Trd.Trader = aTrdAt.Trader
                  Set clsaTrd.Trd.Strategy = aTrdAt.Strategy
              
                  clsaTrd.Trd.BrokerCommission = aTrdAt.BrokerCommission
                  clsaTrd.Trd.ClearingBrokerCommission = aTrdAt.ClearingBrokerCommission
                  
                  clsaTrd.Trd.TradeID = aTrdAt.TradeID
                  
                  
                  Set clsaTrd.Trd.OptRoot = aTrdAt.OptRoot
                  Set clsaTrd.Trd.FutRoot = aTrdAt.FutRoot
                
                  Set clsaTrd.Trd.Und = aTrdAt.Und
                  If CntType = enCtOption Then
                        Set clsaTrd.Trd.Opt = aTrdAt.Opt
                  Else
                        Set clsaTrd.Trd.FutOpt = aTrdAt.FutOpt
                  End If
              Next
            
              Dim aSOQPrice As Double, aPriceOfIndex As Double
              aPriceOfIndex = 0
              aSOQPrice = 0
              If IsItIndex(nRow) Then
                If fgExerScr.TextMatrix(nRow, ESC_SOQ) <> "--" Then
                    aSOQPrice = CDbl(fgExerScr.TextMatrix(nRow, ESC_SOQ))
                End If
                
                If aSOQPrice <= 0 Then
                    aSOQPrice = CDbl(fgExerScr.TextMatrix(nRow, ESC_UND_PRICE))
                End If
                        
                If clsaTrd.Trd.Und.UndType = enCtIndex And bExec Then
                    If Not clsaTrd.Trd.Opt Is Nothing Then
                        If clsaTrd.Trd.Opt.OptType = enOtCall Then
                            aPriceOfIndex = aSOQPrice - clsaTrd.Trd.Opt.Strike
                        Else
                            aPriceOfIndex = clsaTrd.Trd.Opt.Strike - aSOQPrice
                        End If
                    End If
                    
                End If
              End If
    ''Exesuze
              g_TradeChannel.ProcessTradeExec clsaTrd, bExec, IIf(aPriceOfIndex > 0, aPriceOfIndex, 0), clsaTrd.Trd.price
            Else
                ExecteTrade = False
            End If
        End If
ExLoop1:
    Next
End Function

Public Sub TerminateCtl()
    PriceProvider.UnSubscribeQuote
End Sub

Private Sub FilterLoad(ByVal bApplyFilter As Boolean)
    
    Dim sCombo As String, dtExp As Variant, iExpDate As Long
    Dim colExpiry As EtsMmEntityAtomColl, dtExpDate As Date, iRow As Long, vr As Variant, iInd As Long
    Dim aExpiry As EtsMmEntityAtom
    

    Set colExpiry = New EtsMmEntityAtomColl
    On Error Resume Next
    If bApplyFilter Then m_dtExpSelected = CDate(99999)
    For iRow = 1 To fgExerScr.rows - 1
        dtExpDate = CDate(fgExerScr.TextMatrix(iRow, ESC_EXPIRY))
        iInd = Int(dtExpDate)
        Set aExpiry = New EtsMmEntityAtom
        aExpiry.Data = dtExpDate
        
        colExpiry.Add CStr(iInd), aExpiry
        If bApplyFilter And m_dtExpSelected > dtExpDate And dtExpDate > m_dtCurrentDate Then
            m_dtExpSelected = dtExpDate
        End If
        
        Set aExpiry = Nothing
    Next
    
    On Error GoTo Ex
    
    sCombo = "#0;<All>"
    
    For Each aExpiry In colExpiry
        dtExpDate = aExpiry.Data
        iInd = Int(dtExpDate)
        Dim stdDt As String
        stdDt = Format$(dtExpDate, "DD MMMM YYYY")
        sCombo = sCombo + "|#" + CStr(iInd) + ";" + stdDt
    Next
    
    With flxFilter
        .ColComboList(ESC_FLT_DATE) = sCombo
        If colExpiry.Count > 0 Then
            If bApplyFilter = True Then
                .TextMatrix(1, ESC_FLT_DATE) = Format$(m_dtExpSelected, "DD MMMM YYYY")
            Else
                dtExpDate = .TextMatrix(1, ESC_FLT_DATE)
                If colExpiry(CStr(Int(dtExpDate))) Is Nothing Then
                    .TextMatrix(1, ESC_FLT_DATE) = Format$(m_dtExpSelected, "DD MMMM YYYY")
                    m_dtExpSelected = dtExpDate
                End If
            End If
        Else
            .TextMatrix(1, ESC_FLT_DATE) = "<All>"
        End If
    End With
Ex:
End Sub

Public Sub FilterUpdate()
    On Error Resume Next
    Dim iRow  As Long, dtFiltDate As Date, dtLoop As Date, iRowShow As Integer
    iRowShow = 0
    'Exit Sub
    dtFiltDate = flxFilter.TextMatrix(1, ESC_FLT_DATE)
    For iRow = 1 To fgExerScr.rows - 1
        If fgExerScr.TextMatrix(iRow, ESC_EXPIRY) <> "" Then

            dtLoop = fgExerScr.TextMatrix(iRow, ESC_EXPIRY)
             
            If Int(dtLoop) = Int(dtFiltDate) Or Int(dtFiltDate) = 0 Then
                 fgExerScr.RowHidden(iRow) = False
                 iRowShow = iRowShow + 1
            Else
                fgExerScr.RowHidden(iRow) = True
            End If
             
            Dim lQty As Long
            lQty = 0
            lQty = fgExerScr.TextMatrix(iRow, ESC_QTY)
            If lQty = 0 Then
                fgExerScr.RowHidden(iRow) = True
            End If
        End If
'EX_Loop:
    Next
    m_dtExpSelected = dtFiltDate
    m_SattusBar.Caption = ""
    UpdateStatusBarLabel
    
    AutoSizeGrig
End Sub

Public Sub SelectAsItIsAll()
    On Error Resume Next
    Dim iRow  As Long
    For iRow = 1 To fgExerScr.rows - 1
        If fgExerScr.RowHidden(iRow) = False Then
            fgExerScr.Cell(flexcpChecked, iRow, ESC_LEAVE_AS_IT_IS, iRow, ESC_LEAVE_AS_IT_IS) = flexChecked
        End If
    Next
End Sub

Public Sub UnselectAll()
    On Error Resume Next
    Dim iRow  As Long
    For iRow = 1 To fgExerScr.rows - 1
        If fgExerScr.RowHidden(iRow) = False Then
            fgExerScr.Cell(flexcpChecked, iRow, ESC_LEAVE_AS_IT_IS, iRow, ESC_LEAVE_AS_IT_IS) = flexUnchecked
        End If
    Next
End Sub

Public Function GetSelectDate() As Date
    On Error Resume Next
    GetSelectDate = 0
    GetSelectDate = flxFilter.TextMatrix(1, ESC_FLT_DATE)
End Function

Public Sub SetSelectedDate(ByVal dtExt As Date)
    Dim sSel As String
    sSel = Format$(dtExt, "DD MMMM YYYY")
    If dtExt > 0 Then
        flxFilter.TextMatrix(1, ESC_FLT_DATE) = sSel
    Else
        flxFilter.TextMatrix(1, ESC_FLT_DATE) = "<All>"
    End If
End Sub

Private Sub UpdateStatusBarLabel()
    On Error Resume Next
    Dim iRowIterator As Long, bDisplayMsg As Boolean
    
    bDisplayMsg = False
    For iRowIterator = 1 To fgExerScr.rows - 1
        If IsItIndex(iRowIterator) And Not fgExerScr.RowHidden(iRowIterator) Then
            Dim dSOQ As Double
            dSOQ = fgExerScr.TextMatrix(iRowIterator, ESC_SOQ)
            If dSOQ <= 0 Then _
                bDisplayMsg = True
        End If
    Next
    
    If bDisplayMsg Then _
        m_SattusBar.Caption = "Start opening quotation prices are missing for some indices. Please enter proper SOQ price or last index price will be used insted of SOQ"
End Sub
