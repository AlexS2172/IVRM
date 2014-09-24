VERSION 5.00
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmSubSurfaceEditor 
   BackColor       =   &H80000005&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Volatility Surface Editor"
   ClientHeight    =   3675
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7200
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3675
   ScaleWidth      =   7200
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtSurfaceName 
      Height          =   285
      Left            =   5280
      TabIndex        =   3
      Text            =   "New Surface"
      Top             =   2760
      Width           =   1695
   End
   Begin ElladaFlatControls.FlatButton btnCreate 
      Height          =   375
      Left            =   5280
      TabIndex        =   2
      Top             =   3120
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   661
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Create New Surface"
   End
   Begin ElladaFlatControls.FlatButton btnSave 
      Height          =   375
      Left            =   5280
      TabIndex        =   1
      Top             =   120
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   661
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Save"
   End
   Begin VSFlex8Ctl.VSFlexGrid fgSurface 
      Height          =   3375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   4935
      _cx             =   8705
      _cy             =   5953
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
   Begin VB.Label lblSurface 
      Caption         =   "Surface Name"
      Height          =   255
      Left            =   5280
      TabIndex        =   4
      Top             =   2400
      Width           =   1695
   End
End
Attribute VB_Name = "frmSubSurfaceEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    IDX_ROOTSYMBOL = 0
    IDX_VOLA_SURFACE
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_nContractID As Long
Private m_sContractName As String
Private m_sComboRootList As String
Private m_sSurfaceList As String

Private Const MSTR_DEF_COMBO = "#-1;<Common>"

Private m_Roots As New clsOptRootColl

Private rsList As ADODB.Recordset

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal BaseInstrumentID As Long, _
                        ByRef sInstrumentName As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_nContractID = BaseInstrumentID
    
    m_sContractName = sInstrumentName
    
    Load Me
    Me.Caption = "Surface editor for: " & m_sContractName
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
    Execute = True
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim i As Long
    On Error Resume Next
       
    With fgSurface
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
        .MergeCol(IDX_ROOTSYMBOL) = True
        .MergeCol(IDX_VOLA_SURFACE) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = IDX_ROOTSYMBOL
        .TextMatrix(0, i) = "Root" & vbCrLf & "Symbol"
        .TextMatrix(1, i) = "Root" & vbCrLf & "Symbol"
        
        .ColDataType(i) = flexDTString
        
        i = IDX_VOLA_SURFACE
        .TextMatrix(0, i) = "Volatility" & vbCrLf & "Surface"
        .TextMatrix(1, i) = "Volatility" & vbCrLf & "Surface"
        .ColDataType(i) = flexDTString
        .ColComboList(i) = MSTR_DEF_COMBO
                                   
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here load Option Roots Properties from DB
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset
    Dim nRow As Long
    Dim sKey As String
    Dim Root As clsOptRootAtom
    
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    ' Read Option Root Structure '
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    Set rs = gDBW.usp_OptionRoot_Get(Null, m_nContractID)
    
    m_sComboRootList = ""
    m_Roots.Clear
    With rs
        If .RecordCount > 0 Then

            .MoveFirst
            Do While Not .EOF
                sKey = Trim$(Str$(gCmn.ReadLng(.Fields("iRootID"))))
                Set Root = m_Roots.Add(sKey)
                Root.IsChanged = False

                Root.Symbol = gCmn.ReadStr(.Fields("vcSymbol"))
                Root.RootID = gCmn.ReadLng(.Fields("iRootID"))
                Root.UnderlyingID = gCmn.ReadLng(.Fields("iUnderlyingID"))
                Root.IsSynthetic = gCmn.ReadByte(.Fields("tiIsSynthetic"))
                Root.IsFittingEnabled = gCmn.ReadByte(.Fields("tiIsFitEnabled"))
                
                Root.VolaSurfaceID = -1 'No custom volatility surface
                Root.VolaSurfaceGroupID = -1 'No custom volatility surface
                
                m_sComboRootList = m_sComboRootList & "|#" & sKey & ";" & Root.Symbol
                
                .MoveNext
            Loop
        End If
    End With

    
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    '   Read Surface Structure   '
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    ReadSurfaceStructure
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    
    ReloadSurfaceList
    
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    '         Insert Data to Grid          '
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    nRow = 2
    For Each Root In m_Roots

        With fgSurface
            .AddItem ""

            .RowData(nRow) = Root

            .TextMatrix(nRow, IDX_ROOTSYMBOL) = Root.Symbol
            .TextMatrix(nRow, IDX_VOLA_SURFACE) = Root.VolaSurfaceID
            
        End With
        nRow = nRow + 1
    Next

    With fgSurface
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
        End If
        .Redraw = flexRDBuffered
    End With

    LoadData = True
End Function

Private Sub ReloadSurfaceList()
On Error GoTo Exception
    Dim rs As ADODB.Recordset
    Dim sKey As String
    
    Dim sSurfaceName As String
    m_sSurfaceList = ""
    Set rs = gDBW.usp_SubSurfaceList_Get(m_nContractID)
    With rs
        If .RecordCount > 0 Then

            .MoveFirst
            Do While Not .EOF
                
                sKey = Trim$(Str$(gCmn.ReadLng(.Fields("iSurfaceID"))))
                sSurfaceName = gCmn.ReadStr(.Fields("vcSurfaceName"))
                    
                m_sSurfaceList = m_sSurfaceList & "|#" & sKey & ";" & sSurfaceName
                                                  
                .MoveNext
            Loop
        End If
    End With
    
    fgSurface.ColComboList(IDX_VOLA_SURFACE) = MSTR_DEF_COMBO & m_sSurfaceList & "|"
    
Exit Sub
Exception:
    Debug.Print "Error in [ReloadSurfaceList] " & Err.Description
End Sub

Private Sub ReadSurfaceStructure()
On Error GoTo Exception

    Dim rs As ADODB.Recordset
    Dim sKey As String
    Dim Root As clsOptRootAtom
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    '   Read Surface Structure   '
    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
    Set rs = gDBW.usp_SubSurface_Get(Null, m_nContractID)
    With rs
        If .RecordCount > 0 Then

            .MoveFirst
            Do While Not .EOF
                sKey = Trim$(Str$(gCmn.ReadLng(.Fields("iRootID"))))
                Set Root = m_Roots(sKey)
                
                If (Not Root Is Nothing) Then
                    Root.VolaSurfaceID = gCmn.ReadLng(.Fields("iSurfaceID"))
                    Root.VolaSurfaceGroupID = gCmn.ReadLng(.Fields("iGroupID"))
                End If
                               
                .MoveNext
            Loop
        End If
    End With
    
Exit Sub
Exception:
Debug.Print "Error in ReadSurfaceStructure(): " & Err.Description
End Sub

Private Sub btnCreate_Click()
On Error GoTo Exception
    If (Len(Trim(txtSurfaceName.Text)) > 0) Then
        CreateNewSurface m_nContractID, txtSurfaceName.Text
        ReloadSurfaceList
    Else
        MsgBox "Surface name not defined."
    End If
Exit Sub
Exception:
    Debug.Print "Error in btnCreate_Click(): " & Err.Description
End Sub

Private Sub CreateNewSurface(ByVal nBaseAssetID As Long, ByVal vcSurfaceName As String)
On Error GoTo Exception
    gDBW.usp_VolaSurfaceData_Save , nBaseAssetID, vcSurfaceName, 0
Exit Sub
Exception:
    Debug.Print "Error in CreateNewSurface: " & Err.Description
End Sub

Private Sub btnSave_Click()
    SaveData
End Sub

Private Sub fgSurface_AfterEdit(ByVal Row As Long, ByVal Col As Long)
On Error Resume Next
    Dim RowRoot As clsOptRootAtom
    If (fgSurface.Rows < 3) Then Exit Sub
    
    Set RowRoot = fgSurface.RowData(Row)
    If (Not RowRoot Is Nothing) Then
        If (RowRoot.VolaSurfaceID <> fgSurface.ValueMatrix(Row, Col)) Then
            RowRoot.IsChanged = True
        End If
    End If
    
End Sub


Private Sub SaveData()
On Error GoTo Exception
    Dim Root As clsOptRootAtom
    Dim RowRoot As clsOptRootAtom
    Dim nRow As Long
    Dim iNewSurfaceID As Long
    
    
    If (fgSurface.Rows < 3) Then Exit Sub
    
    For nRow = 2 To fgSurface.Rows - 1

        With fgSurface

            Set RowRoot = .RowData(nRow)
           
            If (Not RowRoot Is Nothing) Then
                If (RowRoot.IsChanged) Then
                    iNewSurfaceID = fgSurface.ValueMatrix(nRow, IDX_VOLA_SURFACE)
                    If (iNewSurfaceID > 0) Then
                        If (RowRoot.VolaSurfaceGroupID > 0) Then
                            gDBW.usp_SubSurface_Save RowRoot.VolaSurfaceGroupID, RowRoot.UnderlyingID, RowRoot.RootID, iNewSurfaceID
                        Else
                            gDBW.usp_SubSurface_Save Null, RowRoot.UnderlyingID, RowRoot.RootID, iNewSurfaceID
                        End If
                    Else
                        If (RowRoot.VolaSurfaceGroupID > 0) Then
                            gDBW.usp_SubSurface_Del RowRoot.VolaSurfaceGroupID, RowRoot.UnderlyingID, RowRoot.RootID
                        End If
                    End If
                End If
            End If
            
        End With
        
    Next
    
Exit Sub
Exception:
    Debug.Print "Error in SaveData(): " & Err.Description
End Sub

Private Sub fgSurface_Click()
    If (fgSurface.Rows > 2) Then
        txtSurfaceName.Text = "Surface_" & fgSurface.TextMatrix(fgSurface.Row, IDX_ROOTSYMBOL)
    End If
End Sub

Private Sub fgSurface_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    
    Select Case Col
        Case IDX_ROOTSYMBOL
            Cancel = True
        Case IDX_VOLA_SURFACE
            Cancel = False
        Case Else
            Cancel = True
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~'
'Init Grid View

Private Sub Form_Load()
On Error GoTo Exception
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
    Exit Sub
Exception:
    Screen.MousePointer = vbDefault
    ShowError
End Sub
