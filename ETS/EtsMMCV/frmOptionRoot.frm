VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Begin VB.Form frmOptionRoot 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "Options Roots"
   ClientHeight    =   3600
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   4530
   Icon            =   "frmOptionRoot.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3600
   ScaleWidth      =   4530
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   315
      Left            =   3360
      TabIndex        =   2
      Top             =   3180
      Width           =   1095
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   3360
      TabIndex        =   1
      Top             =   2760
      Width           =   1095
   End
   Begin VSFlex8UCtl.VSFlexGrid fgSelect 
      Height          =   3255
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   3135
      _cx             =   5530
      _cy             =   5741
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
      BackColor       =   16777215
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   16777215
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   0
      SelectionMode   =   1
      GridLines       =   14
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   3
      Cols            =   3
      FixedRows       =   2
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   -1  'True
      FormatString    =   $"frmOptionRoot.frx":000C
      ScrollTrack     =   0   'False
      ScrollBars      =   2
      ScrollTips      =   0   'False
      MergeCells      =   5
      MergeCompare    =   1
      AutoResize      =   0   'False
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
      WordWrap        =   -1  'True
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   1
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
   Begin VB.Label lblPrompt 
      Caption         =   "Label1"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   0
      Width           =   3135
   End
End
Attribute VB_Name = "frmOptionRoot"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK(ByVal nKey As Long, ByRef aItems As EtsGeneralLib.EtsMmEntityAtomColl)

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bDirty As Boolean
Private m_Items As New EtsGeneralLib.EtsMmEntityAtomColl
Private m_nKey As Long
Private m_bAllowAllDeselect As Boolean

Public Sub Execute(ByVal sCaption As String, ByVal sPrompt As String, ByRef aItems As EtsGeneralLib.EtsMmEntityAtomColl, _
                    ByVal nKey As Long, ByRef frmOwner As Form, Optional ByVal bAllowAllDeselect As Boolean = False)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    m_bDirty = False
    m_nKey = nKey
    m_bAllowAllDeselect = bAllowAllDeselect
    
    Load Me
    Me.Caption = sCaption
    lblPrompt.Caption = sPrompt
    
    Dim i&, nCount&, aItem As EtsGeneralLib.EtsMmEntityAtom, aInItem As EtsGeneralLib.EtsMmEntityAtom
    
    m_Items.Clear
    
    nCount = aItems.Count
    For i = 1 To nCount
        Set aInItem = aItems(i)
        If m_Items(CStr(aInItem.ID)) Is Nothing Then
            Set aItem = m_Items.Add(CStr(aInItem.ID))
            aItem.ID = aInItem.ID
            aItem.Name = aInItem.Name
            aItem.Data = aInItem.Data
            aItem.Data3 = aInItem.Data3
            Set aItem = Nothing
        End If
        Set aInItem = Nothing
    Next
    
    ' Header Merge
    fgSelect.MergeCellsFixed = flexMergeFree
    fgSelect.MergeRow(0) = True
    fgSelect.MergeRow(1) = True
    fgSelect.MergeCol(0) = True
    fgSelect.MergeCol(1) = True
    fgSelect.MergeCol(2) = True
    fgSelect.Cell(flexcpBackColor, 2, 0, 2, 2) = &H80000018
    
    For i = 1 To nCount
        Set aInItem = aItems(i)
        fgSelect.AddItem (aInItem.Name & vbTab & CStr(aInItem.Data) & vbTab & CStr(aInItem.Data3))
        fgSelect.RowData(fgSelect.Row) = i
    Next
       
    
    m_bDirty = False
    btnOK.Enabled = IsOKEnabled
    
    Me.Show vbModeless, m_frmOwner
   
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open items dialog."
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    'Me.Hide
    Unload Me
End Sub

Private Sub ClearAll(iColumn As Long)
    On Error Resume Next
    Dim i&, nIndex&
    
    With fgSelect
        If .Rows > 3 Then
            For i = 2 To .Rows - 1
                .TextMatrix(i, iColumn) = "0"
            Next
        End If
    End With
   
End Sub
Private Sub SelectAll(iColumn As Long)
    On Error Resume Next
    Dim i&, nIndex&
    
    With fgSelect
        If .Rows > 3 Then
            For i = 2 To .Rows - 1
                .TextMatrix(i, iColumn) = "-1"
            Next
        End If
    End With
End Sub



Private Sub btnOK_Click()
    On Error Resume Next
    Dim nCount&, i&, aItem As EtsGeneralLib.EtsMmEntityAtom
    'Me.Hide
    If m_bDirty Then
        Screen.MousePointer = vbHourglass
        
        With fgSelect
            If .Rows > 3 Then
                For i = 3 To .Rows - 1
                    Set aItem = m_Items(i - 2)
                    If Not aItem Is Nothing Then
                        aItem.Data = IIf(.TextMatrix(i, 1) <> "0", 1, 0)
                        aItem.Data3 = IIf(.TextMatrix(i, 2) <> "0", 1, 0)
                        
                        If Not g_PerformanceLog Is Nothing Then _
                            g_PerformanceLog.LogMmInfo enLogUserAction, "After Edit Option Roots """ & aItem.Name _
                                                                        & IIf(aItem.Data = 1, """ ""Display"" Selected.", """ ""Display"" Not Selected.") _
                                                                        & IIf(aItem.Data3 = 1, " ""Use when Fitting to IV"" Selected.", " ""Use when Fitting to IV"" Not Selected."), m_frmOwner.Caption
                        
                        Set aItem = Nothing
                    End If

                Next
            End If
        End With
               
        RaiseEvent OnOK(m_nKey, m_Items)
        Screen.MousePointer = vbDefault
    End If
    Unload Me
End Sub


Private Sub fgSelect_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    If Row = 2 Then
        Dim sCode As String
        sCode = fgSelect.TextMatrix(Row, Col)
        If sCode <> "0" Then
            SelectAll (Col)
        Else
            ClearAll (Col)
        End If
    End If
    
    m_bDirty = True
    btnOK.Enabled = (m_bAllowAllDeselect Or IsOKEnabled)
   
End Sub

Private Sub fgSelect_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
 If Row < 2 Or Col < 1 Then Cancel = True

End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    m_Items.Clear
    Form_Terminate
End Sub


Private Function IsOKEnabled() As Boolean
    On Error Resume Next
    Dim i&, nIndex1&: nIndex1& = 0
    Dim nIndex2&: nIndex2& = 0
    IsOKEnabled = True
    With fgSelect
        If .Rows > 3 Then
            For i = 3 To .Rows - 1
                If .TextMatrix(i, 1) <> "0" Then _
                    nIndex1 = nIndex1 + 1
                If .TextMatrix(i, 2) <> "0" Then _
                    nIndex2 = nIndex2 + 1
                    
            Next
        End If
    End With
    If nIndex1 = 0 Or nIndex2 = 0 Then _
        IsOKEnabled = False
End Function
