VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Begin VB.Form frmManualPrices 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Manual prices"
   ClientHeight    =   4080
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   3165
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4080
   ScaleWidth      =   3165
   StartUpPosition =   3  'Windows Default
   Begin VSFlex8UCtl.VSFlexGrid fgMP 
      Height          =   2895
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   2895
      _cx             =   5106
      _cy             =   5106
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
      SelectionMode   =   3
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
   Begin VB.CommandButton Command3 
      Caption         =   "OK"
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   3600
      Width           =   1215
   End
   Begin VB.CommandButton Command2 
      Caption         =   "Discard changes"
      Height          =   375
      Left            =   1440
      TabIndex        =   1
      Top             =   3120
      Width           =   1575
   End
   Begin VB.CommandButton Command1 
      Caption         =   "Delete"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   3120
      Width           =   1215
   End
End
Attribute VB_Name = "frmManualPrices"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Type TManualPrice
    ctrID As Long
    Sym As String
    price As Double
    OrigPrice As Double
    IsRemoved As Boolean
    UndID As Long
    ContractType As Long
End Type

Private mprices() As TManualPrice

Private m_sEditedValue As String

Public ChangedCount As Long
Private m_nRecCount As Long


Private Sub InitGrid()
    
    With fgMP
        .Redraw = flexRDNone
        
        .Rows = 1
        .Cols = 3
        
        .AllowBigSelection = True
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .TabBehavior = flexTabControls
        
        ' format columns
        .ColHidden(0) = True
        .TextMatrix(0, 0) = "#"
        .ColDataType(0) = flexDTLong
        
        .ColHidden(1) = False
        .TextMatrix(0, 1) = "Symbol"
        .ColDataType(IDX_TIME) = flexDTString
        .ColFormat(IDX_TIME) = ""
        
        .ColHidden(2) = False
        .TextMatrix(0, 2) = "Active Price"
        .ColDataType(2) = flexDTString
        .ColFormat(2) = "#,##0.00"
        .Editable = flexEDKbd
        
        .ColAlignment(0) = flexAlignCenterCenter
        .ColAlignment(1) = flexAlignLeftCenter
        .ColAlignment(2) = flexAlignLeftCenter
        
        .Cell(flexcpAlignment, 0, 0, 0, 1) = flexAlignCenterCenter
        .Cell(flexcpPictureAlignment, 0, 0, 0, 2) = flexPicAlignRightCenter
    
        .Redraw = flexRDBuffered
    End With
    
End Sub

Private Sub AddGridRow(ByVal RowNum As Integer, ByRef mPrice As TManualPrice)
On Error Resume Next
        fgMP.TextMatrix(RowNum, 0) = mPrice.ctrID
        fgMP.TextMatrix(RowNum, 1) = mPrice.Sym
        fgMP.TextMatrix(RowNum, 2) = mPrice.price
        fgMP.RowHidden(RowNum) = mPrice.IsRemoved
End Sub

Private Sub RefreshGridRows()
    
    Dim i As Integer
        
    For i = 1 To m_nRecCount
        AddGridRow i, mprices(i)
    Next i
        
End Sub


Private Sub LoadRecords()
On Error Resume Next

    Dim rs As ADODB.Recordset
    Dim i As Integer
    
    'Load All manual prices from db
    Set rs = gDBW.usp_MmManualPrice_Get
    'make array
    m_nRecCount = rs.RecordCount
    ReDim Preserve mprices(1 To m_nRecCount)
    fgMP.Rows = m_nRecCount + 1
    
    i = 1
    While Not rs.EOF
    
        mprices(i).ctrID = rs!ContractID
        mprices(i).Sym = rs!Symbol
        mprices(i).price = rs!manualPrice
        mprices(i).OrigPrice = rs!manualPrice
        mprices(i).UndID = rs!UnderlyingID
        mprices(i).ContractType = rs!ContractTypeID
        mprices(i).IsRemoved = False
       
        AddGridRow i, mprices(i)
        i = i + 1
        rs.MoveNext
    
    Wend
    rs.Close

End Sub

Private Sub SetPriceByContractID(ByVal ctrID As Long, ByVal price As Double)
    Dim i As Integer
        
    For i = 1 To m_nRecCount
        If mprices(i).ctrID = ctrID Then
            mprices(i).price = price
            Exit For
        End If
    Next i
End Sub

Private Sub DeleteRecord(ByVal ctrID As Long)
    Dim i As Integer
        
    For i = 1 To m_nRecCount
        If mprices(i).ctrID = ctrID Then
            mprices(i).IsRemoved = True
            Exit For
        End If
    Next i
End Sub


Private Sub Command1_Click()
On Error Resume Next
    
    Dim sValue As String, dValue As Double, nValue As Long
    Dim r As Long
    
    For r = 1 To fgMP.Rows - 1
        If (fgMP.IsSelected(r)) Then
            sValue = Trim$(fgMP.TextMatrix(r, 0))
            nValue = Abs(ReadLng(sValue))
            DeleteRecord (nValue)
        End If
    Next
    
    RefreshGridRows

End Sub

Private Sub Command2_Click()
On Error Resume Next
    Dim i As Integer
        
    For i = 1 To fgMP.Rows - 1
        mprices(i).IsRemoved = False
        mprices(i).price = mprices(i).OrigPrice
        AddGridRow i, mprices(i)
    Next i

End Sub

Private Sub Command3_Click()
    
    Dim i As Integer
    Dim isChg As Boolean
    Dim enCtType As EtsContractTypeEnum
    
    isChg = False
    
    For i = 1 To m_nRecCount
            
        If mprices(i).IsRemoved Then
                gDBW.usp_MmManualPrice_Del mprices(i).ctrID
                ChangedCount = ChangedCount + 1
                enCtType = mprices(i).ContractType
                
                If Not g_ContractAll(mprices(i).ctrID) Is Nothing Then
                    If (enCtType = enCtIndex Or enCtType = enCtStock) Then
                        If Not g_ContractAll(mprices(i).ctrID).Und Is Nothing Then
                            g_ContractAll(mprices(i).ctrID).Und.manualActivePrice = 0
                            PubManualPrice 0#, mprices(i).ctrID, mprices(i).UndID, mprices(i).ContractType, enUsDelete
                        End If
                        
                    ElseIf (enCtType = enCtFuture) Then
                        If Not g_ContractAll(mprices(i).ctrID).Fut Is Nothing Then
                            g_ContractAll(mprices(i).ctrID).Fut.manualActivePrice = 0
                            PubManualPrice 0#, mprices(i).ctrID, mprices(i).UndID, mprices(i).ContractType, enUsDelete
                        End If
                    End If
                End If
                
                isChg = True
        Else
                If mprices(i).price <> mprices(i).OrigPrice Then
                    gDBW.usp_MmManualPrice_Save mprices(i).ctrID, mprices(i).price
                    ChangedCount = ChangedCount + 1
                    
                    enCtType = mprices(i).ContractType
                    If Not g_ContractAll(mprices(i).ctrID) Is Nothing Then
                        If (enCtType = enCtIndex Or enCtType = enCtStock) Then
                            If Not g_ContractAll(mprices(i).ctrID).Und Is Nothing Then
                                If g_ContractAll(mprices(i).ctrID).Und.ActiveFuture Is Nothing Then
                                    g_ContractAll(mprices(i).ctrID).Und.manualActivePrice = mprices(i).price
                                    PubManualPrice mprices(i).price, mprices(i).ctrID, mprices(i).UndID, mprices(i).ContractType, enUsUpdate
                                End If
                            End If
                        ElseIf (enCtType = enCtFuture) Then
                            If Not g_ContractAll(mprices(i).ctrID).Fut Is Nothing Then
                                g_ContractAll(mprices(i).ctrID).Fut.manualActivePrice = mprices(i).price
                                PubManualPrice mprices(i).price, mprices(i).ctrID, mprices(i).UndID, mprices(i).ContractType, enUsUpdate
                            End If
                        End If
                    End If
                    
                    isChg = True
                End If
        End If
                        
    Next i

    Unload Me

End Sub

Private Sub fgMP_AfterEdit(ByVal Row As Long, ByVal Col As Long)
On Error Resume Next

    Dim sValue As String
    Dim dValue As Double
    
    If Col = 2 Then
        sValue = Trim$(fgMP.TextMatrix(Row, Col))
        
        If (sValue <> m_sEditedValue) Then
            dValue = Abs(ReadDbl(sValue))
            If dValue <> 0 Then
                SetPriceByContractID ReadLng(Trim$(fgMP.TextMatrix(Row, 0))), dValue
            End If
        End If
        
    End If

End Sub

Private Sub fgMP_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    m_sEditedValue = Trim$(fgMP.TextMatrix(Row, Col))
End Sub

Private Sub Form_Load()
On Error Resume Next
    ChangedCount = 0
    InitGrid
    LoadRecords
End Sub

Private Sub PubManualPrice(dPrice As Double, nContractID As Long, nUndID As Long, enCtType As EtsContractTypeEnum, enStatus As MANUAL_PRICE_UPDATE_STATUS)
On Error GoTo Exception

    Dim Data As MSGSTRUCTLib.ManualPriceUpdate
    Set Data = New MSGSTRUCTLib.ManualPriceUpdate
                                            
    Data.Status = enStatus
    Data.ContractID = nContractID
    Data.UndID = nUndID
    Data.ContractType = enCtType
    Data.ActivePrice = dPrice
                                            
    g_TradeChannel.PubManualPriceUpdate Data
    Exit Sub
    
Exception:
    Debug.Print "Error while trying to pub active price for contract"
End Sub
