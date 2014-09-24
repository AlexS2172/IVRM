VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmFutureRootList 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Index Future Roots"
   ClientHeight    =   3720
   ClientLeft      =   45
   ClientTop       =   720
   ClientWidth     =   9360
   Icon            =   "frmFutureRootList.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3720
   ScaleWidth      =   9360
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgRoots 
      Height          =   3495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   7695
      _cx             =   13573
      _cy             =   6165
      _ConvInfo       =   1
      Appearance      =   0
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
      BackColorBkg    =   -2147483643
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   3
      HighLight       =   2
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   1
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
      ScrollTrack     =   -1  'True
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
      TabBehavior     =   1
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   -1  'True
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
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   7920
      TabIndex        =   1
      Top             =   2040
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Height          =   300
      Left            =   7920
      TabIndex        =   2
      Top             =   1560
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "&OK"
   End
   Begin ElladaFlatControls.FlatButton btnAdd 
      Height          =   300
      Left            =   7920
      TabIndex        =   3
      Top             =   120
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Add"
   End
   Begin ElladaFlatControls.FlatButton btnDel 
      Height          =   300
      Left            =   7920
      TabIndex        =   4
      Top             =   600
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Delete"
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   8400
      Picture         =   "frmFutureRootList.frx":000C
      Top             =   3360
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Menu mnuFutureRoots 
      Caption         =   "FutureRoots"
      Begin VB.Menu mnuAdd 
         Caption         =   "Add Future Root"
      End
      Begin VB.Menu mnuDel 
         Caption         =   "Delete Future Root"
      End
   End
End
Attribute VB_Name = "frmFutureRootList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    FRT_ROOTSYMBOL = 0
    FRT_ROOTNAME
    FRT_EXPORTSYMBOL
    FRT_FUTURELOTSIZE
    FRT_OPTIONLOTSIZE
    FRT_COLUMNS_COUNT
    LAST_COLUMN = FRT_COLUMNS_COUNT - 1
End Enum

Private m_nIndexId As Long
Private m_sIndexSymbol As String
Private m_bOK As Boolean

Private m_Roots As New clsFutureRootColl
Private m_sCurrentOriginalText As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal IndexId As Long, ByVal IndexSymbol As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_nIndexId = IndexId
    m_sIndexSymbol = IndexSymbol
    m_sCurrentOriginalText = ""
    Load Me
    Me.Caption = "Future Roots for " & m_sIndexSymbol & " index"
    m_bOK = False
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
    Execute = m_bOK
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnAdd_Click()
    On Error GoTo ErrHnd
    AddRow
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub btnOK_Click()
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bOK = SaveData
    Screen.MousePointer = vbDefault
    If m_bOK Then Hide
    Exit Sub
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

Private Sub btnDel_Click()
    On Error GoTo ErrHnd
    DeleteRow fgRoots.RowSel
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub fgRoots_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim sValue$, aRoot As clsFutureRootAtom
    On Error Resume Next
    With fgRoots
        sValue = Trim$(.Cell(flexcpTextDisplay, Row, Col))
        If m_sCurrentOriginalText <> sValue And Row > 1 Then
            Set aRoot = .RowData(Row)
            If Not aRoot Is Nothing Then
                If Col = FRT_ROOTSYMBOL Then
                    aRoot.Symbol = sValue
                    m_Roots.Remove (m_sCurrentOriginalText)
                    m_Roots.Add sValue, aRoot
                ElseIf Col = FRT_EXPORTSYMBOL Then
                    aRoot.ExportSymbol = sValue
                ElseIf Col = FRT_ROOTNAME Then
                    aRoot.Name = sValue
                ElseIf Col = FRT_FUTURELOTSIZE Then
                    aRoot.FutureLotSize = gCmn.ReadLng(sValue)
                ElseIf Col = FRT_OPTIONLOTSIZE Then
                    aRoot.OptionLotSize = gCmn.ReadLng(sValue)
                End If
                
                If aRoot.State = ELEM_UNCHANGED Then
                    aRoot.State = ELEM_UPDATED
                End If
                imgSave.Visible = True
            End If
        End If
    End With
End Sub

Private Sub fgRoots_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With fgRoots
        If (Button And vbRightButton) And .Row > -1 And .Col > -1 Then
            PopupMenu mnuFutureRoots, , , , mnuAdd
        End If
    End With
End Sub

Private Sub fgRoots_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    m_sCurrentOriginalText = Trim$(fgRoots.Cell(flexcpTextDisplay, Row, Col))
End Sub

Private Sub fgRoots_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    Dim Row As Long
    On Error Resume Next
    With fgRoots
        If (Button And vbRightButton) Then
            If .MouseRow >= 2 Then
                .Row = .MouseRow
                .Col = .MouseCol
            End If
        End If
    End With
End Sub

Private Sub fgRoots_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Dim nVal&, aRoot As clsFutureRootAtom
    On Error Resume Next
        
    With fgRoots
        If Row > 1 Then
            If Col = FRT_ROOTSYMBOL Then
                Set aRoot = .RowData(Row)
                If Not aRoot Is Nothing Then
                    If aRoot.Symbol <> .EditText And IsSymbolExists(.EditText) Then
                        Cancel = True
                    End If
                End If
                
            ElseIf Col = FRT_EXPORTSYMBOL Then
                Set aRoot = .RowData(Row)
                If Not aRoot Is Nothing Then
                    If aRoot.ExportSymbol <> .EditText And IsExportSymbolExists(.EditText) Then
                        Cancel = True
                    End If
                End If
                
            ElseIf Col = FRT_FUTURELOTSIZE Or Col = FRT_OPTIONLOTSIZE Then
                nVal = gCmn.ReadLng(fgRoots.EditText)
                fgRoots.EditText = Format$(nVal, "0000")
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim i As Long
    On Error Resume Next
    
    With fgRoots
        .Rows = 0
        .Cols = FRT_COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = FRT_COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 2
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(FRT_ROOTSYMBOL) = True
        .MergeCol(FRT_ROOTNAME) = True
        .MergeCol(FRT_EXPORTSYMBOL) = True
        .MergeCol(FRT_FUTURELOTSIZE) = True
        .MergeCol(FRT_OPTIONLOTSIZE) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = FRT_ROOTSYMBOL
        .TextMatrix(0, i) = "Symbol"
        .TextMatrix(1, i) = "Symbol"
        .ColDataType(i) = flexDTString
        .ColWidth(i) = 800
        .EditWidth(i) = 800
                
        i = FRT_ROOTNAME
        .TextMatrix(0, i) = "Name"
        .TextMatrix(1, i) = "Name"
        .ColDataType(i) = flexDTString
        .ColWidth(i) = 3000
        .EditWidth(i) = 3000
               
        i = FRT_EXPORTSYMBOL
        .TextMatrix(0, i) = "Import Id"
        .TextMatrix(1, i) = "Import Id"
        .ColDataType(i) = flexDTString
        .ColWidth(i) = 800
        .EditWidth(i) = 800
               
        i = FRT_FUTURELOTSIZE
        .TextMatrix(0, i) = "Future" & vbCrLf & "Lot Size"
        .TextMatrix(1, i) = "Future" & vbCrLf & "Lot Size"
        .ColWidth(i) = 1000
        .EditWidth(i) = 1000
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = "#,###"
        .ColEditMask(i) = "9999"
        
        i = FRT_OPTIONLOTSIZE
        .TextMatrix(0, i) = "Option" & vbCrLf & "Lot Size"
        .TextMatrix(1, i) = "Option" & vbCrLf & "Lot Size"
        .ColWidth(i) = 1000
        .EditWidth(i) = 1000
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = "#,###"
        .ColEditMask(i) = "9999"
        
                
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here load Option Roots Properties from DB
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset
    Dim nRow As Long
    Dim sKey As String
    Dim aRoot As clsFutureRootAtom
        
    Set rs = gDBW.usp_BoFutureRoot_Get(m_nIndexId)
    
    m_Roots.Clear
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sKey = gCmn.ReadStr(.Fields("vcFutureRootSymbol"))
                Set aRoot = m_Roots.Add(sKey)
                aRoot.State = ELEM_UNCHANGED
                aRoot.Symbol = sKey
                aRoot.ExportSymbol = gCmn.ReadStr(.Fields("vcExportSymbol"))
                aRoot.RootID = gCmn.ReadLng(.Fields("iFutureRootID"))
                aRoot.Name = gCmn.ReadStr(.Fields("vcFutureRootName"))
                aRoot.FutureLotSize = gCmn.ReadLng(.Fields("iFutureLotSize"))
                aRoot.OptionLotSize = gCmn.ReadLng(.Fields("iOptionLotSize"))
                .MoveNext
            Loop
        End If
    End With
    
    With fgRoots
        .Redraw = flexRDNone
    
        nRow = 2
        For Each aRoot In m_Roots
                            
            .AddItem ""
                        
            .RowData(nRow) = aRoot
                    
            .TextMatrix(nRow, FRT_ROOTSYMBOL) = aRoot.Symbol
            .TextMatrix(nRow, FRT_ROOTNAME) = aRoot.Name
            .TextMatrix(nRow, FRT_EXPORTSYMBOL) = aRoot.ExportSymbol
            .TextMatrix(nRow, FRT_FUTURELOTSIZE) = aRoot.FutureLotSize
            .Cell(flexcpAlignment, nRow, FRT_FUTURELOTSIZE) = flexAlignRightCenter
            .TextMatrix(nRow, FRT_OPTIONLOTSIZE) = aRoot.OptionLotSize
            .Cell(flexcpAlignment, nRow, FRT_OPTIONLOTSIZE) = flexAlignRightCenter
            nRow = nRow + 1
        Next
    
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
            mnuDel.Enabled = True
            btnDel.Enabled = True
        Else
            mnuDel.Enabled = False
            btnDel.Enabled = False
        End If
        .Redraw = flexRDBuffered
    End With
    
    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here save Option Roots Properties to DB
Private Function SaveData() As Boolean
    Dim aRoot As clsFutureRootAtom
    On Error GoTo EH
    For Each aRoot In m_Roots
        If aRoot.State = ELEM_INSERTED Then
            gDBW.usp_BoFutureRoot_Save Null, m_nIndexId, aRoot.Symbol, aRoot.Name, _
                aRoot.FutureLotSize, aRoot.OptionLotSize, aRoot.ExportSymbol, _
                aRoot.tiQuoteFormat, aRoot.iQuotationUnitAmmount, 2, aRoot.fQuoteDenomination, aRoot.fTickSize, aRoot.fTickValue
                
        ElseIf aRoot.State = ELEM_UPDATED Then
            gDBW.usp_BoFutureRoot_Save aRoot.RootID, m_nIndexId, aRoot.Symbol, aRoot.Name, _
                aRoot.FutureLotSize, aRoot.OptionLotSize, aRoot.ExportSymbol, _
                aRoot.tiQuoteFormat, aRoot.iQuotationUnitAmmount, aRoot.iQuotationUnitID, aRoot.fQuoteDenomination, aRoot.fTickSize, aRoot.fTickValue
                
        ElseIf aRoot.State = ELEM_DELETED Then
            gDBW.usp_BoFutureRoot_Del aRoot.RootID
        End If
NextRoot:
    Next
    SaveData = True
    Exit Function
EH:
    Dim sErrMsg$
    If Not aRoot Is Nothing Then
        sErrMsg = "Can't "
        If aRoot.State = ELEM_INSERTED Then
            sErrMsg = sErrMsg & "insert "
        ElseIf aRoot.State = ELEM_UPDATED Then
            sErrMsg = sErrMsg & "update "
        ElseIf aRoot.State = ELEM_DELETED Then
            sErrMsg = sErrMsg & "delete "
        End If
        sErrMsg = sErrMsg & aRoot.Symbol
        sErrMsg = sErrMsg & " future root."
        ShowError sErrMsg
    Else
        ShowError "Error while save data"
    End If
    GoTo NextRoot
End Function

Private Function GetCalendarComboBox() As String
    On Error Resume Next
    Dim rs As ADODB.Recordset, sComboList$
    On Error Resume Next
    sComboList = ""
    
    Set rs = gDBW.usp_ExpiryCalendar_Get
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sComboList = sComboList & "#" & gCmn.ReadStr(.Fields("iExpCalendarID"))
                sComboList = sComboList & ";" & gCmn.ReadStr(.Fields("vcExpCalendarName"))
                sComboList = sComboList & vbTab & gCmn.ReadStr(.Fields("vcExpCalendarDesc")) & "|"
                .MoveNext
            Loop
        End If
    End With
    GetCalendarComboBox = sComboList
End Function

Private Sub AddRow()
    Dim aRoot As clsFutureRootAtom, sKey$, nRow&
    
    sKey = GetNewSymbol
    Set aRoot = m_Roots.Add(sKey)
        
    If Not aRoot Is Nothing Then
        aRoot.FutureLotSize = 100
        aRoot.Symbol = sKey
        aRoot.OptionLotSize = 1
        aRoot.RootID = 0
        aRoot.State = ELEM_INSERTED
        aRoot.Name = "<New Value>"
        
        With fgRoots
             .AddItem ""
             nRow = .Rows - 1
            .RowData(nRow) = aRoot
                    
            .TextMatrix(nRow, FRT_ROOTSYMBOL) = aRoot.Symbol
            .TextMatrix(nRow, FRT_ROOTNAME) = aRoot.Name
            .TextMatrix(nRow, FRT_EXPORTSYMBOL) = aRoot.ExportSymbol
            .TextMatrix(nRow, FRT_FUTURELOTSIZE) = aRoot.FutureLotSize
            .Cell(flexcpAlignment, nRow, FRT_FUTURELOTSIZE) = flexAlignRightCenter
            .TextMatrix(nRow, FRT_OPTIONLOTSIZE) = aRoot.OptionLotSize
            .Cell(flexcpAlignment, nRow, FRT_OPTIONLOTSIZE) = flexAlignRightCenter
            
            .Select nRow, FRT_ROOTSYMBOL
            .EditCell
        End With
        
        imgSave.Visible = True
    End If
    mnuDel.Enabled = True
    btnDel.Enabled = True
End Sub

Private Sub DeleteRow(ByVal Row As Long)
    Dim aRoot As clsFutureRootAtom
    With fgRoots
        
        If Row < 2 Then Exit Sub
    
        Set aRoot = .RowData(Row)
        
        If Row = .Rows - 1 Then
            .RemoveItem Row
            .Row = Row - 1
            .RowSel = Row - 1
        Else
            .RemoveItem Row
        End If
        
        If .RowSel < 2 Then
            mnuDel.Enabled = False
            btnDel.Enabled = False
        End If
            
        If Not aRoot Is Nothing Then
            If aRoot.State = ELEM_INSERTED Then
                m_Roots.Remove aRoot.Symbol
            Else
                aRoot.State = ELEM_DELETED
            End If
        End If
    End With
    
    imgSave.Visible = True

End Sub

Private Function IsSymbolExists(Symbol As String) As Boolean
    Dim aRoot As clsFutureRootAtom
    On Error Resume Next
    For Each aRoot In m_Roots
        If aRoot.Symbol = Symbol And aRoot.State <> ELEM_DELETED Then
            IsSymbolExists = True
            Exit Function
        End If
    Next
    IsSymbolExists = False
End Function

Private Function IsExportSymbolExists(ExportSymbol As String) As Boolean
    Dim aRoot As clsFutureRootAtom
    On Error Resume Next
    If Len(ExportSymbol) > 0 Then
        For Each aRoot In m_Roots
            If aRoot.ExportSymbol = ExportSymbol And aRoot.State <> ELEM_DELETED Then
                IsExportSymbolExists = True
                Exit Function
            End If
        Next
    End If
    IsExportSymbolExists = False
End Function


Private Function GetNewSymbol() As String
    Dim sSymbol$, i&
    On Error Resume Next
    sSymbol = "<New>"
    If Not IsSymbolExists(sSymbol) Then
        GetNewSymbol = sSymbol
        Exit Function
    Else
        i = 0
        While True
            i = i + 1
            sSymbol = CStr(i)
            If Len(sSymbol) > 8 Then sSymbol = Right(sSymbol, 8)
            sSymbol = "<New " & sSymbol & ">"
            If Not IsSymbolExists(sSymbol) Then
                GetNewSymbol = sSymbol
                Exit Function
            End If
        Wend
    End If
End Function

Private Sub mnuAdd_Click()
    On Error GoTo ErrHnd
    AddRow
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub mnuDel_Click()
    On Error GoTo ErrHnd
    DeleteRow fgRoots.RowSel
    Exit Sub
ErrHnd:
    ShowError
End Sub
