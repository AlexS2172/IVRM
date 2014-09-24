VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmPosReconcile 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Positions Reconciliation"
   ClientHeight    =   6600
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   9510
   Icon            =   "frmPosReconcile.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6600
   ScaleWidth      =   9510
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgPos 
      Height          =   6135
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   7215
      _cx             =   12726
      _cy             =   10821
      _ConvInfo       =   1
      Appearance      =   2
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
      BackColorSel    =   -2147483637
      ForeColorSel    =   0
      BackColorBkg    =   16777215
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
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   7440
      TabIndex        =   2
      Top             =   360
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Fix Positions"
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   7440
      TabIndex        =   3
      Top             =   720
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Cancel Reconciliation"
   End
   Begin ElladaFlatControls.FlatButton btnSelectAll 
      Height          =   300
      Left            =   7440
      TabIndex        =   4
      Top             =   1560
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Select All"
   End
   Begin ElladaFlatControls.FlatButton btnDeselectAll 
      Height          =   300
      Left            =   7440
      TabIndex        =   5
      Top             =   1920
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Deselect All"
   End
   Begin ElladaFlatControls.FlatButton btnCopy 
      Height          =   300
      Left            =   7440
      TabIndex        =   6
      Top             =   3120
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Copy to Clipboard"
   End
   Begin ElladaFlatControls.FlatButton btnSelectAllPrice 
      Height          =   300
      Left            =   7440
      TabIndex        =   7
      Top             =   2280
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Select All Prices"
   End
   Begin ElladaFlatControls.FlatButton btnDeselectAllPrice 
      Height          =   300
      Left            =   7440
      TabIndex        =   8
      Top             =   2640
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Deselect All Prices"
   End
   Begin VB.Label lblPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "Positions Mismatch :"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   2415
   End
   Begin VB.Menu mnuGrid 
      Caption         =   "<Menu>"
      Visible         =   0   'False
      Begin VB.Menu mnuGridCopy 
         Caption         =   "Copy"
         Shortcut        =   ^C
      End
   End
End
Attribute VB_Name = "frmPosReconcile"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Enum GridColumns
    POS_FIX
    POS_TRADER
    POS_UND_SYMBOL
    POS_OLD_STRATEGY
    POS_IMP_STRATEGY
    POS_TRADE_STRATEGY
    POS_SYMBOL
    POS_EXPORTSYMBOL
    POS_OLD_QTY
    POS_NEW_QTY
    POS_QTY_DIF
    POS_FIX_QTY
    POS_OLD_PRC
    POS_NEW_PRC
    POS_VAL_DIF
    POS_FIX_PRC
    
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Public result As VbMsgBoxResult
Private m_collPos As clsRecPosColl
Private m_collStrategyByName As clsStrategyColl
Private m_btFileFormat As Byte
Private m_bStrategySupport As Boolean

Private m_bBaseStrategyComboList$

Public Function Execute(ByRef collPos As clsRecPosColl, ByVal btFileFormat As Byte, _
                    ByRef frmEOD As frmEndOfDay, ByRef collStrategyByName As clsStrategyColl, Optional bStrategySupport As Boolean = False) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    Set m_collPos = collPos
    Set m_collStrategyByName = collStrategyByName
    m_btFileFormat = btFileFormat
    m_bStrategySupport = bStrategySupport
    
    Execute = False
    If LoadData Then
        Load Me
        ResizeControls
        Screen.MousePointer = vbDefault
        Show vbModal
        Unload Me
        If result = vbOK Then Execute = True
    Else
        frmEOD.LogMsg "Check Postitions Succeeded."
        Execute = True
    End If
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function
    
Private Sub MakeBaseStrategyComboList()
    On Error Resume Next
    Dim aStrategy As clsStrategyAtom
    Dim arrStrategyList$(), nIdx&
    
    m_bBaseStrategyComboList = "|#0;--"
    If m_collStrategyByName.Count > 0 Then
        ReDim arrStrategyList(0 To m_collStrategyByName.Count - 1)
        nIdx = 0&
        
        For Each aStrategy In m_collStrategyByName
            arrStrategyList(nIdx) = "#" & Trim$(Str$(aStrategy.StrategyId)) & ";" & aStrategy.StrategyName
            nIdx = nIdx + 1
        Next
    
        m_bBaseStrategyComboList = "|" & Join$(arrStrategyList, "|")
        Erase arrStrategyList
    End If
End Sub

Public Function LoadData() As Boolean
    On Error GoTo EH
    Dim nRow&, i&, nCount&, aPos As clsRecPosAtom, nPosDif&, dValDif#
    result = vbCancel
    
    LoadData = False
    MakeBaseStrategyComboList
    
    fgPos.Rows = 1
    nCount = m_collPos.Count
    
    For i = 1 To nCount
        Set aPos = m_collPos(i)
        If (aPos.PosValidNew <> aPos.PosValidOld Or aPos.QtyNew <> aPos.QtyOld) _
            Or (m_btFileFormat = 3 And (Abs(aPos.PriceNew - aPos.PriceOld) >= DBL_EPSILON Or aPos.StrategyOld <> aPos.StrategyNew)) _
            Or (m_btFileFormat = 1 And aPos.StrategyOld <> aPos.StrategyNew And m_bStrategySupport) Then
            
            fgPos.AddItem ""
            nRow = fgPos.Rows - 1
            fgPos.RowData(nRow) = i
            
            fgPos.Cell(flexcpChecked, nRow, POS_FIX) = IIf(aPos.bFix, flexChecked, flexUnchecked)
            fgPos.Cell(flexcpChecked, nRow, POS_FIX_QTY) = IIf(aPos.bFix, flexChecked, flexUnchecked)
            fgPos.Cell(flexcpChecked, nRow, POS_FIX_PRC) = IIf(aPos.bFixPrice, flexChecked, flexUnchecked)
            
            fgPos.TextMatrix(nRow, POS_TRADER) = aPos.Trader.Acronym
            fgPos.TextMatrix(nRow, POS_UND_SYMBOL) = aPos.UndSymbol
            fgPos.TextMatrix(nRow, POS_OLD_STRATEGY) = IIf(Len(aPos.StrategyOld) > 0, aPos.StrategyOld, "--")
            fgPos.TextMatrix(nRow, POS_IMP_STRATEGY) = IIf(Len(aPos.StrategyNew) > 0, aPos.StrategyNew, "--")
            fgPos.TextMatrix(nRow, POS_TRADE_STRATEGY) = IIf(Len(aPos.StrategyForTrade) > 0, aPos.StrategyForTrade, "--")
            fgPos.TextMatrix(nRow, POS_EXPORTSYMBOL) = IIf(Len(aPos.ImportSymbol) > 0, aPos.ImportSymbol, "--")
            fgPos.TextMatrix(nRow, POS_OLD_PRC) = IIf(aPos.PosValidOld And aPos.PriceOld >= DBL_EPSILON, aPos.PriceOld, "--")
            fgPos.TextMatrix(nRow, POS_NEW_PRC) = IIf(aPos.PosValidNew And aPos.PriceNew >= DBL_EPSILON, aPos.PriceNew, "--")
            
            If Not aPos.Opt Is Nothing Then
                fgPos.TextMatrix(nRow, POS_SYMBOL) = aPos.Opt.SymbolDisplay
            ElseIf Not aPos.FutOpt Is Nothing Then
                fgPos.TextMatrix(nRow, POS_SYMBOL) = aPos.FutOpt.SymbolDisplay
            ElseIf Not aPos.Fut Is Nothing Then
                fgPos.TextMatrix(nRow, POS_SYMBOL) = aPos.Fut.SymbolDisplay
            Else
                fgPos.TextMatrix(nRow, POS_SYMBOL) = aPos.Und.Symbol
            End If
                        
            fgPos.TextMatrix(nRow, POS_OLD_QTY) = IIf(aPos.PosValidOld, aPos.QtyOld, "--")
            fgPos.TextMatrix(nRow, POS_NEW_QTY) = IIf(aPos.PosValidNew, aPos.QtyNew, "--")
            
            nPosDif = IIf(aPos.PosValidNew, aPos.QtyNew, 0&) - IIf(aPos.PosValidOld, aPos.QtyOld, 0&)
            dValDif = IIf(aPos.PosValidNew, aPos.QtyNew * aPos.PriceNew, 0#) - IIf(aPos.PosValidOld, aPos.QtyOld * aPos.PriceOld, 0#)
            
            fgPos.TextMatrix(nRow, POS_QTY_DIF) = nPosDif
            fgPos.TextMatrix(nRow, POS_VAL_DIF) = dValDif
            
            Set aPos = Nothing
            
            LoadData = True
        End If
    Next
    
    fgPos.AutoSize 0, LAST_COLUMN
    
    Exit Function
EH:
    Set aPos = Nothing
    gCmn.ErrorHandler "Fail to show positions report."
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    result = vbCancel
    Hide
End Sub

Private Sub btnCopy_Click()
    On Error Resume Next
    CopyGridToClipboard fgPos
End Sub

Private Sub btnDeselectAll_Click()
    On Error Resume Next
    Dim i&, nCount&, aPos As clsRecPosAtom, nPosIdx&
        
    nCount = fgPos.Rows - 1
    If m_btFileFormat <> 3 Then
        For i = 1 To nCount
            fgPos.Cell(flexcpChecked, i, POS_FIX) = flexUnchecked
            fgPos.Cell(flexcpChecked, i, POS_FIX_QTY) = flexUnchecked
        Next
    Else
        For i = 1 To nCount
            nPosIdx = fgPos.RowData(i)
            Set aPos = m_collPos(nPosIdx)
            If Not aPos Is Nothing Then
                If aPos.QtyNew <> aPos.QtyOld Or aPos.PosValidNew <> aPos.PosValidOld Then fgPos.Cell(flexcpChecked, i, POS_FIX_QTY) = flexUnchecked
            End If
        Next
    End If
End Sub

Private Sub btnDeselectAllPrice_Click()
    On Error Resume Next
    Dim i&, nCount&, aPos As clsRecPosAtom, nPosIdx&
    
    nCount = fgPos.Rows - 1
    If m_btFileFormat <> 3 Then
        For i = 1 To nCount
            fgPos.Cell(flexcpChecked, i, POS_FIX_PRC) = flexUnchecked
        Next
    Else
        For i = 1 To nCount
            nPosIdx = fgPos.RowData(i)
            Set aPos = m_collPos(nPosIdx)
            If Not aPos Is Nothing Then
                If Abs(aPos.PriceNew - aPos.PriceOld) >= DBL_EPSILON Then fgPos.Cell(flexcpChecked, i, POS_FIX_PRC) = flexUnchecked
            End If
        Next
    End If
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Dim i&, nCount&, aPos As clsRecPosAtom
    
    nCount = fgPos.Rows - 1
    For i = 1 To nCount
        Set aPos = m_collPos(fgPos.RowData(i))
        If Not aPos Is Nothing Then
            If m_btFileFormat <> 3 Then
                aPos.bFix = (fgPos.Cell(flexcpChecked, i, POS_FIX) = flexChecked)
            Else
                aPos.bFix = (fgPos.Cell(flexcpChecked, i, POS_FIX_QTY) = flexChecked)
                aPos.bFixPrice = (fgPos.Cell(flexcpChecked, i, POS_FIX_PRC) = flexChecked)
            End If
            Set aPos = Nothing
        End If
    Next
    
    result = vbOK
    Hide
End Sub

Private Sub btnSelectAll_Click()
    On Error Resume Next
    Dim i&, nCount&, aPos As clsRecPosAtom, nPosIdx&
    
    nCount = fgPos.Rows - 1
    For i = 1 To nCount
        fgPos.Cell(flexcpChecked, i, POS_FIX) = flexChecked
        fgPos.Cell(flexcpChecked, i, POS_FIX_QTY) = flexChecked
    Next
End Sub

Private Sub btnSelectAllPrice_Click()
    On Error Resume Next
    Dim i&, nCount&, aPos As clsRecPosAtom, nPosIdx&
    
    nCount = fgPos.Rows - 1
    For i = 1 To nCount
        fgPos.Cell(flexcpChecked, i, POS_FIX_PRC) = flexChecked
    Next
End Sub

Private Sub fgPos_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim aStrategy As clsStrategyAtom, sStrategyName$
    Dim aPos As clsRecPosAtom
    
    If m_btFileFormat = 3 And Col = POS_TRADE_STRATEGY Then
        Set aPos = m_collPos(fgPos.RowData(Row))
        If Not aPos Is Nothing Then
            If fgPos.ComboIndex >= 0 Then
                sStrategyName = Trim$(fgPos.ComboItem)
            Else
                sStrategyName = Trim$(fgPos.EditText)
            End If
            
            If Len(sStrategyName) > 0 And sStrategyName <> "--" Then
                Set aStrategy = m_collStrategyByName(UCase$(sStrategyName))
                If Not aStrategy Is Nothing Then
                    aPos.StrategyForTrade = aStrategy.StrategyName
                    aPos.StrategyForTradeID = aStrategy.StrategyId
                Else
                    aPos.StrategyForTrade = sStrategyName
                    aPos.StrategyForTradeID = 0
                End If
            Else
                aPos.StrategyForTrade = ""
                aPos.StrategyForTradeID = 0
                fgPos.TextMatrix(Row, Col) = "--"
            End If
            Set aPos = Nothing
        End If
    End If
End Sub

Private Sub fgPos_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim aPos As clsRecPosAtom, i&
    
    fgPos.ComboList = ""
    
    If m_btFileFormat = 3 And Col = POS_TRADE_STRATEGY Then
        Set aPos = m_collPos(fgPos.RowData(Row))
        If Not aPos Is Nothing Then
            If aPos.StrategyForTradeID <> 0 Then
                fgPos.ComboList = m_bBaseStrategyComboList
                
            ElseIf Len(aPos.StrategyForTrade) > 0 Then
                fgPos.ComboList = "|#-2;" & aPos.StrategyForTrade & m_bBaseStrategyComboList
            Else
                fgPos.ComboList = m_bBaseStrategyComboList
            End If
            Set aPos = Nothing
            fgPos.EditMaxLength = 12
            
        End If
    End If
End Sub

Private Sub fgPos_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    If Button = vbRightButton Then PopupMenu mnuGrid
End Sub

Private Sub fgPos_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Dim aPos As clsRecPosAtom, i&
    On Error Resume Next
    If m_btFileFormat = 3 Then
        Cancel = (Col <> POS_FIX_QTY) And (Col <> POS_FIX_PRC) And (Col <> POS_TRADE_STRATEGY)
        If Not Cancel Then
            i = fgPos.RowData(Row)
            Set aPos = m_collPos(i)
            If Not aPos Is Nothing Then
                If Col <> POS_TRADE_STRATEGY Then
                    If aPos.QtyNew = aPos.QtyOld And aPos.PosValidNew = aPos.PosValidOld And Col = POS_FIX_QTY Then Cancel = True
                    If Abs(aPos.PriceNew - aPos.PriceOld) < DBL_EPSILON And Col = POS_FIX_PRC Then Cancel = True
                End If
                Set aPos = Nothing
            Else
                Cancel = True
            End If
        End If
    Else
        Cancel = (Col <> POS_FIX)
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    
    InitGrid
    
    If m_btFileFormat <> 3 Then
        btnSelectAllPrice.Visible = False
        btnDeselectAllPrice.Visible = False
        btnSelectAll.Caption = "Select All"
        btnDeselectAll.Caption = "Deselect All"
    Else
        btnSelectAllPrice.Visible = True
        btnDeselectAllPrice.Visible = True
        btnSelectAll.Caption = "Select All Qty"
        btnDeselectAll.Caption = "Deselect All Qty"
    End If
End Sub

Private Sub InitGrid()
    On Error Resume Next
    Dim i&
    
    With fgPos
        .Rows = 1
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .AutoResize = False
        .AutoSizeMouse = False
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusRaised
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabControls
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = POS_FIX
        .TextMatrix(0, i) = "Fix"
        .ColDataType(i) = flexDTBoolean
        .ColHidden(i) = m_btFileFormat = 3
                
        i = POS_TRADER
        .TextMatrix(0, i) = "Trader"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
                
        i = POS_OLD_STRATEGY
        .TextMatrix(0, i) = "Cur Strategy"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .ColHidden(i) = m_btFileFormat <> 3 And m_btFileFormat <> 5 And Not (m_btFileFormat = 1 And m_bStrategySupport)
        
        i = POS_IMP_STRATEGY
        .TextMatrix(0, i) = "Imp Strategy"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .ColHidden(i) = m_btFileFormat <> 3 And m_btFileFormat <> 5 And Not (m_btFileFormat = 1 And m_bStrategySupport)
        
        i = POS_TRADE_STRATEGY
        .TextMatrix(0, i) = "New Strategy"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .ColHidden(i) = m_btFileFormat <> 3
        
        i = POS_UND_SYMBOL
        .TextMatrix(0, i) = "Underlying"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .ColHidden(i) = m_btFileFormat = 3

        i = POS_SYMBOL
        .TextMatrix(0, i) = "Symbol"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = POS_EXPORTSYMBOL
        .TextMatrix(0, i) = "Import Id"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .ColHidden(i) = m_btFileFormat <> 3
        
        i = POS_OLD_QTY
        .TextMatrix(0, i) = "Cur Qty"
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = "#,##0"
    
        i = POS_NEW_QTY
        .TextMatrix(0, i) = "Imp Qty"
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = "#,##0"
        
        i = POS_QTY_DIF
        .TextMatrix(0, i) = "Qty Diff"
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = "#,##0"
        .ColHidden(i) = m_btFileFormat <> 3
        
        i = POS_FIX_QTY
        .TextMatrix(0, i) = "Fix Qty"
        .ColDataType(i) = flexDTBoolean
        .ColHidden(i) = m_btFileFormat <> 3
                
        i = POS_OLD_PRC
        .TextMatrix(0, i) = "Cur Prc"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "#,##0.00"
        .ColHidden(i) = m_btFileFormat <> 3

        i = POS_NEW_PRC
        .TextMatrix(0, i) = "Imp Prc"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "#,##0.00"
        .ColHidden(i) = m_btFileFormat <> 3
        
        i = POS_VAL_DIF
        .TextMatrix(0, i) = "Val Diff"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "#,##0.00"
        .ColHidden(i) = m_btFileFormat <> 3
        
        i = POS_FIX_PRC
        .TextMatrix(0, i) = "Fix Prc"
        .ColDataType(i) = flexDTBoolean
        .ColHidden(i) = m_btFileFormat <> 3
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
    End With
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set m_collPos = Nothing
    Set m_collStrategyByName = Nothing
End Sub

Private Sub mnuGridCopy_Click()
    On Error Resume Next
    btnCopy_Click
End Sub

Private Sub ResizeControls()
    Dim nCol&, nGridColWidth&, nWidthMax&
    On Error Resume Next
    With fgPos
        nGridColWidth = ScaleX(.GridLineWidth, vbPixels, vbTwips)
        For nCol = 0 To .Cols - 1
            If Not .ColHidden(nCol) Then nGridColWidth = nGridColWidth + .ColWidth(nCol) + ScaleX(.GridLineWidth, vbPixels, vbTwips)
        Next
        
        nWidthMax = ScaleX(GetSystemMetrics(SM_CXFULLSCREEN), vbPixels, vbTwips) - 2500
        .Width = nGridColWidth + ScaleX(GetSystemMetrics(SM_CXVSCROLL), vbPixels, vbTwips)
        
        If .Width > nWidthMax Then .Width = nWidthMax
        
        Me.Width = .Width + 2385
        btnOk.Left = .Width + 225
        btnCancel.Left = .Width + 225
        btnSelectAll.Left = .Width + 225
        btnDeselectAll.Left = .Width + 225
        btnSelectAllPrice.Left = .Width + 225
        btnDeselectAllPrice.Left = .Width + 225
        btnCopy.Left = .Width + 225
    End With
End Sub
