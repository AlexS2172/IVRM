VERSION 5.00
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Begin VB.Form frmAssetGroupEditor 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Group Editor"
   ClientHeight    =   2925
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   9900
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2925
   ScaleWidth      =   9900
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VSFlex8Ctl.VSFlexGrid fgComponents 
      Height          =   2655
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   3735
      _cx             =   6588
      _cy             =   4683
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
      Rows            =   1
      Cols            =   4
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmAssetGroupEditor.frx":0000
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   1
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
   Begin VB.CommandButton cmdSave 
      Caption         =   "Save"
      Height          =   375
      Left            =   4200
      TabIndex        =   3
      Top             =   2160
      Width           =   855
   End
   Begin VB.CommandButton cmdRemove 
      Caption         =   "Remove"
      Height          =   375
      Left            =   4200
      TabIndex        =   2
      Top             =   720
      Width           =   855
   End
   Begin VB.CommandButton cmdAdd 
      Caption         =   "Add"
      Height          =   375
      Left            =   4200
      TabIndex        =   1
      Top             =   240
      Width           =   855
   End
   Begin VSFlex8Ctl.VSFlexGrid fgAsset 
      Height          =   2655
      Left            =   5280
      TabIndex        =   0
      Top             =   120
      Width           =   4455
      _cx             =   7858
      _cy             =   4683
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
      Rows            =   1
      Cols            =   5
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmAssetGroupEditor.frx":007A
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
Attribute VB_Name = "frmAssetGroupEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private m_nAssetID As Long
Private m_sAssetName As String
Private m_rsList As ADODB.Recordset
Private m_ComponentRC As Long
Private m_AssetRC As Long

Public Enum eComponentColls
    CC_SYMBOL = 0
    CC_INUSE = 1
    CC_TYPE = 2
    CC_ID = 3
End Enum

Public Enum eAssetColls
    AC_COMPONENT = 0
    AC_COEFFICIENT = 1
    AC_USEHEAD = 2
    AC_ID = 3
    AC_ISHEAD = 4
End Enum

Public Function Execute(ByVal ItemID As Long, ByVal ItemName As String) As Boolean
On Error GoTo Err
    Load Me
    m_nAssetID = ItemID
    m_sAssetName = ItemName
    m_AssetRC = 0
    m_ComponentRC = 0
    
    Me.Caption = m_sAssetName & " Asset Group"
    If (LoadData) Then
        Show vbModal
        Unload Me
    End If
    Exit Function
Err:
    ShowError
End Function

Private Function LoadData() As Boolean
On Error GoTo Err
    ' Prepare internal recordset
    Set m_rsList = New ADODB.Recordset
    Dim bAlreadyInGroup As Boolean
    Dim sOwnerGroup As String
    
    LoadData = False
    bAlreadyInGroup = False
    ' Load full component items list
    Set m_rsList = gDBW.usp_UnderlyingsForAssetGroup_Get()
    If Not m_rsList Is Nothing Then
        With m_rsList
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    
                    fgComponents.AddItem !vcSymbol
                    m_ComponentRC = m_ComponentRC + 1
                    fgComponents.TextMatrix(m_ComponentRC, CC_INUSE) = IIf(!iContractID <> m_nAssetID, !bIsUsed, True)
                    fgComponents.TextMatrix(m_ComponentRC, CC_ID) = !iContractID
                    fgComponents.TextMatrix(m_ComponentRC, CC_TYPE) = IIf(!iContractType = 1, "INDEX", "STOCK")
                    
                    If (m_nAssetID = !iContractID And !iInclusionType = 0 And !bIsUsed) Then
                            bAlreadyInGroup = True
                            sOwnerGroup = !vcSymbol
                            Exit Do
                    End If
                    
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    
    If (bAlreadyInGroup = False) Then
        'Init head component
        fgAsset.AddItem m_sAssetName
        m_AssetRC = m_AssetRC + 1
        fgAsset.TextMatrix(m_AssetRC, AC_COMPONENT) = m_sAssetName
        fgAsset.TextMatrix(m_AssetRC, AC_ISHEAD) = True
        fgAsset.TextMatrix(m_AssetRC, AC_ID) = m_nAssetID
        fgAsset.TextMatrix(m_AssetRC, AC_USEHEAD) = True
        fgAsset.TextMatrix(m_AssetRC, AC_COEFFICIENT) = 1#
    
        'Init elements of asset component
        Set m_rsList = gDBW.usp_GroupAsset_Get(m_nAssetID)
        If Not m_rsList Is Nothing Then
            With m_rsList
                If .RecordCount > 0 Then
                    .MoveFirst
                    Do While Not .EOF
    
                        fgAsset.AddItem !vcElementSymbol
                        m_AssetRC = m_AssetRC + 1
                        fgAsset.TextMatrix(m_AssetRC, AC_ISHEAD) = False
                        fgAsset.TextMatrix(m_AssetRC, AC_ID) = !iElementID
                        fgAsset.TextMatrix(m_AssetRC, AC_USEHEAD) = !bUseHead
                        fgAsset.TextMatrix(m_AssetRC, AC_COEFFICIENT) = !fCoefficient
    
                        .MoveNext
                    Loop
                End If
            End With
        End If
        LoadData = True
    Else
        MsgBox "Component already in group: " & sOwnerGroup
    End If
    
    Exit Function
Err:
    Debug.Print "Some Error on data loading"
End Function

Private Sub AddToAssetGroup()
    If (fgComponents.Row < 1 Or fgComponents.Rows <= 1) Then Exit Sub
    If (fgComponents.ValueMatrix(fgComponents.Row, CC_INUSE) = False And fgComponents.ValueMatrix(fgComponents.Row, CC_ID) <> m_nAssetID) Then
        fgComponents.TextMatrix(fgComponents.Row, CC_INUSE) = True
        fgAsset.AddItem fgComponents.TextMatrix(fgComponents.Row, CC_SYMBOL)
        m_AssetRC = m_AssetRC + 1
        fgAsset.TextMatrix(m_AssetRC, AC_COEFFICIENT) = 1#
        fgAsset.TextMatrix(m_AssetRC, AC_USEHEAD) = True
        fgAsset.TextMatrix(m_AssetRC, AC_ID) = fgComponents.ValueMatrix(fgComponents.Row, CC_ID)
    End If
End Sub

Private Sub cmdAdd_Click()
   AddToAssetGroup
End Sub

Private Sub cmdRemove_Click()
    Dim iRow As Long
    If (fgAsset.Rows <= 2 Or fgAsset.Row < 1) Then Exit Sub
    For iRow = 1 To fgComponents.Rows - 1
        If (fgComponents.ValueMatrix(iRow, CC_ID) = fgAsset.ValueMatrix(fgAsset.Row, AC_ID) And fgAsset.ValueMatrix(fgAsset.Row, AC_ISHEAD) = False) Then
            fgComponents.TextMatrix(iRow, CC_INUSE) = False
            fgAsset.RemoveItem fgAsset.Row
            m_AssetRC = m_AssetRC - 1
            Exit For
        End If
    Next iRow
End Sub

Private Sub cmdSave_Click()
On Error GoTo Err
        
    Dim iRow As Long
    Dim sXML As String
    '<root>
    '<asset headid="908118" elemid="1452042" coeff="77.6000000001" usehead="0" />
    '</root>
    sXML = "<root>"
    For iRow = 1 To fgAsset.Rows - 1
        If (m_nAssetID <> fgAsset.ValueMatrix(iRow, AC_ID)) Then
            sXML = sXML + "<asset headid=""" & Str(m_nAssetID) & _
            """ elemid=""" & Str(fgAsset.ValueMatrix(iRow, AC_ID)) & _
            """ coeff=""" & Str(fgAsset.ValueMatrix(iRow, AC_COEFFICIENT)) & _
            """ usehead=""" & IIf(fgAsset.ValueMatrix(iRow, AC_USEHEAD), "1", "0") & """ />"
        End If
    Next iRow
    sXML = sXML + "</root>"
    
    If (fgAsset.Rows > 2) Then
        gDBW.usp_AssetGroup_Save sXML
    Else
        gDBW.usp_GroupAsset_Del m_nAssetID
    End If
    
    Exit Sub
Err:
    Debug.Print "Error while trying to save data..."
End Sub

Private Sub fgAsset_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    If (Col = AC_COEFFICIENT) Then
        If (fgAsset.ValueMatrix(Row, Col) < 0) Then
            fgAsset.TextMatrix(Row, Col) = Abs(fgAsset.ValueMatrix(Row, Col))
        End If
    End If
End Sub

Private Sub fgAsset_MouseDown(Button As Integer, Shift As Integer, X As Single, y As Single)
    If (fgAsset.Rows <= 1) Then Exit Sub
    If (fgAsset.MouseRow <= 1) Then Exit Sub
    If (fgAsset.MouseCol = AC_USEHEAD Or fgAsset.MouseCol = AC_COEFFICIENT) And (fgAsset.ValueMatrix(fgAsset.MouseRow, AC_ISHEAD) = False) Then
        fgAsset.Select fgAsset.MouseRow, fgAsset.MouseCol
        fgAsset.EditCell
    End If
End Sub

Private Sub fgComponents_Click()
    If (fgComponents.MouseRow = 0) Then fgComponents.Sort = flexSortGenericAscending
End Sub

Private Sub fgComponents_DblClick()
    AddToAssetGroup
End Sub
