VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmOptionSelect 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Select option from list"
   ClientHeight    =   4050
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8010
   Icon            =   "frmOptionSelect.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4050
   ScaleWidth      =   8010
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgOptions 
      Height          =   3135
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   7815
      _cx             =   13785
      _cy             =   5530
      _ConvInfo       =   1
      Appearance      =   0
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   6600
      TabIndex        =   1
      Top             =   3600
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
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Height          =   300
      Left            =   5160
      TabIndex        =   2
      Top             =   3600
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
      Caption         =   "&OK"
   End
   Begin VB.Label lblOptions 
      BackStyle       =   0  'Transparent
      Caption         =   "Options :"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   735
   End
End
Attribute VB_Name = "frmOptionSelect"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    OPT_CHECK
    OPT_SYMBOL
    OPT_EXPORTSYMBOL
    OPT_ROOT
    OPT_EXPIRATION
    OPT_STRIKE
    OPT_CALLPUT
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_Opt As clsRecPosOptColl
Private m_bOK As Boolean
Private m_nOptId As Long
Private m_bIsExport As Boolean
Private m_sCurrentOriginalText As String
Private m_sExportSymbol As String
Private m_sSymbol As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal bIsExport As Boolean, _
    ByRef collOpt As clsRecPosOptColl, _
    ByVal sSymbol As String, _
    Optional nOptId As Long = 0&) As clsRecPosOptAtom
    
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    Set m_Opt = collOpt
    m_bIsExport = bIsExport
    m_nOptId = nOptId
    m_sCurrentOriginalText = ""
    m_sExportSymbol = ""
    m_sSymbol = sSymbol
    If m_bIsExport Then
        Me.Caption = "Set Import Id for option '" & sSymbol & "'"
        m_sExportSymbol = m_Opt(gCmn.ReadStr(m_nOptId)).ExportSymbol
    Else
        Me.Caption = "Select option from list for '" & sSymbol & "'"
    End If
    
    Load Me
    m_bOK = False
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
    
    If m_bOK Then Set Execute = collOpt(CStr(m_nOptId))
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnOK_Click()
    Dim aOpt As clsRecPosOptAtom, I&
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    
    If m_nOptId = 0 Then
        ShowWarning "Select one of options"
    Else
        If m_bIsExport Then
            If Not CheckValidityOfExportSymbol(m_nOptId, m_sExportSymbol) Then
                For I = 2 To fgOptions.Rows
                    If fgOptions.Cell(flexcpChecked, I, OPT_CHECK) = flexChecked Then
                        fgOptions.Select I, OPT_EXPORTSYMBOL
                        fgOptions.ShowCell I, OPT_EXPORTSYMBOL
                        fgOptions.EditCell
                        Exit For
                    End If
                Next
            Else
                m_bOK = True
            End If
        Else
            m_bOK = True
        End If
    End If
    
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
    Dim sMsg$
    On Error Resume Next
    sMsg = "Option '" & m_sSymbol & "' will be skiped." & vbCrLf & " Close dialog anyway?"
    
    If gCmn.MyMsgBox(sMsg, vbExclamation + vbYesNo) = vbNo Then Exit Sub
    Me.Hide
End Sub


Private Sub fgOptions_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim Opt As clsRecPosOptAtom, nRow&, sValue$
    On Error Resume Next
    
    With fgOptions
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            If Col = OPT_CHECK And Row > 1 Then
                Set Opt = .RowData(Row)
                If Not Opt Is Nothing Then
                    If .Cell(flexcpChecked, Row, OPT_CHECK) = flexChecked Then
                        If m_nOptId <> Opt.Id Then
                            m_nOptId = Opt.Id
                            
                            For nRow = 2 To .Rows
                                If nRow <> Row Then
                                    .Cell(flexcpChecked, nRow, OPT_CHECK) = flexUnchecked
                                End If
                            Next
                        End If
                    End If
                End If
            ElseIf Col = OPT_EXPORTSYMBOL And Row > 1 Then
                Set Opt = .RowData(Row)
                If Not Opt Is Nothing Then
                    If Len(sValue) > 0 Then
                        m_sExportSymbol = sValue
                        Opt.ExportSymbol = m_sExportSymbol
                    End If
                End If
            End If
        End If
    End With
End Sub

Private Sub fgOptions_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Dim Opt As clsRecPosOptAtom
    On Error Resume Next
    Cancel = True
    Select Case Col
        Case OPT_CHECK
            If m_bIsExport Then
                Cancel = True
            Else
                If fgOptions.Cell(flexcpChecked, Row, OPT_CHECK) = flexUnchecked Then
                    Cancel = False
                Else
                    Cancel = True
                End If
            End If
        Case OPT_EXPORTSYMBOL
            If m_bIsExport Then
                If fgOptions.Cell(flexcpChecked, Row, OPT_CHECK) = flexChecked Then
                    Cancel = False
                Else
                    Cancel = True
                End If
            Else
                Cancel = True
            End If
        Case Else
            Cancel = True
    End Select
    If Not Cancel Then m_sCurrentOriginalText = Trim$(fgOptions.TextMatrix(Row, Col))
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
Dim I As Long
    On Error Resume Next
    
    With fgOptions
        .Rows = 0
        .Cols = COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = COLUMNS_COUNT
        
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
        .MergeCol(OPT_CHECK) = True
        .MergeCol(OPT_SYMBOL) = True
        .MergeCol(OPT_EXPORTSYMBOL) = True
        .MergeCol(OPT_ROOT) = True
        .MergeCol(OPT_EXPIRATION) = True
        .MergeCol(OPT_STRIKE) = True
        .MergeCol(OPT_CALLPUT) = True
               
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        I = OPT_CHECK
        .TextMatrix(0, I) = "Select"
        .TextMatrix(1, I) = "Select"
        .ColDataType(I) = flexDTBoolean
        .ColWidth(I) = 700
        .EditWidth(I) = 700
                
        I = OPT_SYMBOL
        .TextMatrix(0, I) = "Option" & vbCrLf & "Symbol"
        .TextMatrix(1, I) = "Option" & vbCrLf & "Symbol"
        .ColDataType(I) = flexDTString
        .ColWidth(I) = 1000
        .EditWidth(I) = 1000
                       
        I = OPT_EXPORTSYMBOL
        .TextMatrix(0, I) = "Import Id"
        .TextMatrix(1, I) = "Import Id"
        .ColDataType(I) = flexDTString
        .ColHidden(I) = Not m_bIsExport
        .ColWidth(I) = 2000
        .EditWidth(I) = 2000

        I = OPT_ROOT
        .TextMatrix(0, I) = "Option Root"
        .TextMatrix(1, I) = "Option Root"
        .ColDataType(I) = flexDTString
        .ColWidth(I) = 1000
        .EditWidth(I) = 1000
        
        I = OPT_EXPIRATION
        .TextMatrix(0, I) = "Expiration"
        .TextMatrix(1, I) = "Expiration"
        .ColDataType(I) = flexDTDate
        .ColFormat(I) = "MMM YYYY"
        .ColWidth(I) = 1000
        .EditWidth(I) = 1000
        
        I = OPT_STRIKE
        .TextMatrix(0, I) = "Strike"
        .TextMatrix(1, I) = "Strike"
        .ColDataType(I) = flexDTString
        .ColWidth(I) = 1000
        .EditWidth(I) = 1000
        
        I = OPT_CALLPUT
        .TextMatrix(0, I) = "Call/Put"
        .TextMatrix(1, I) = "Call/Put"
        .ColDataType(I) = flexDTString
        .ColComboList(I) = "#0;Put|#1;Call"
        .ColWidth(I) = 800
        .EditWidth(I) = 800
            
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here load Option Roots Properties from DB
Private Function LoadData() As Boolean
    Dim Opt As clsRecPosOptAtom, nRow&
                    
    With fgOptions
        .Redraw = flexRDNone
        For Each Opt In m_Opt
            .AddItem ""
            nRow = .Rows - 1
            .RowData(nRow) = Opt
            .Cell(flexcpChecked, nRow, OPT_CHECK) = flexUnchecked
            .TextMatrix(nRow, OPT_SYMBOL) = Opt.Symbol
            .TextMatrix(nRow, OPT_EXPORTSYMBOL) = Opt.ExportSymbol
            .TextMatrix(nRow, OPT_ROOT) = Opt.OptionRootSymbol
            .TextMatrix(nRow, OPT_EXPIRATION) = Opt.Expiry
            .TextMatrix(nRow, OPT_STRIKE) = Opt.Strike
            .TextMatrix(nRow, OPT_CALLPUT) = IIf(Opt.IsCall, "Call", "Put")
            
            If m_bIsExport And Opt.Id = m_nOptId Then .Cell(flexcpChecked, nRow, OPT_CHECK) = flexChecked
        Next
        
        If Not m_bIsExport Then
            m_nOptId = m_Opt(1).Id
        End If
        .Redraw = flexRDBuffered
    End With
    
    LoadData = True
End Function


Private Function CheckValidityOfExportSymbol(ByVal nOptId As Long, ByVal sExportSymbol As String) As Boolean
    Dim aOpt As clsRecPosOptAtom
    On Error Resume Next
    CheckValidityOfExportSymbol = False
    If Len(sExportSymbol) = 0 Then
        ShowWarning "Import Id must be non empty"
        Exit Function
    End If
    For Each aOpt In m_Opt
        If aOpt.Id <> nOptId And UCase(aOpt.ExportSymbol) = UCase(sExportSymbol) Then
            ShowWarning "Import Id must be unique"
            Exit Function
        End If
    Next
    CheckValidityOfExportSymbol = True
End Function
