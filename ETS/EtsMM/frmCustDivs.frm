VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmCustomDivs 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Custom Dividends [for ...]"
   ClientHeight    =   2670
   ClientLeft      =   2565
   ClientTop       =   1455
   ClientWidth     =   8355
   Icon            =   "frmCustDivs.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2670
   ScaleWidth      =   8355
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VSFlex8UCtl.VSFlexGrid fgDividend 
      Bindings        =   "frmCustDivs.frx":000C
      Height          =   2175
      Left            =   4800
      TabIndex        =   19
      Top             =   240
      Width           =   3375
      _cx             =   5953
      _cy             =   3836
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
      AllowBigSelection=   0   'False
      AllowUserResizing=   0
      SelectionMode   =   1
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   0
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   -1  'True
      FormatString    =   $"frmCustDivs.frx":001F
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
      ExplorerBar     =   1
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
      DataMode        =   1
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
   Begin VB.CommandButton btnAdd 
      Caption         =   "Add     >>"
      Height          =   375
      Left            =   3720
      TabIndex        =   18
      Top             =   240
      Width           =   975
   End
   Begin VB.CommandButton btnClear 
      Caption         =   "Delete All"
      Height          =   375
      Left            =   3720
      TabIndex        =   17
      Top             =   1200
      Width           =   975
   End
   Begin VB.Frame frmAdd 
      Caption         =   "Dividend Information"
      Height          =   1935
      Left            =   120
      TabIndex        =   10
      Top             =   120
      Width           =   3495
      Begin VB.TextBox txtDivAmnt 
         Height          =   315
         Left            =   1800
         TabIndex        =   16
         Text            =   "0.0"
         Top             =   610
         Width           =   1454
      End
      Begin VB.TextBox txtStep 
         Height          =   315
         Left            =   1800
         TabIndex        =   14
         Text            =   "1"
         Top             =   980
         Width           =   1454
      End
      Begin MSComCtl2.DTPicker dtpDivDate 
         BeginProperty DataFormat 
            Type            =   1
            Format          =   "dd.MM.yyyy"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1049
            SubFormatType   =   3
         EndProperty
         Height          =   315
         Left            =   1800
         TabIndex        =   11
         Top             =   240
         Width           =   1454
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Format          =   55508993
         CurrentDate     =   38251
      End
      Begin VB.Label lblStep 
         Caption         =   "Add with step (days):"
         Height          =   255
         Left            =   180
         TabIndex        =   15
         Top             =   980
         Width           =   1575
      End
      Begin VB.Label Label1 
         Caption         =   "Date:"
         Height          =   255
         Left            =   1260
         TabIndex        =   13
         Top             =   240
         Width           =   375
      End
      Begin VB.Label Label2 
         Caption         =   "Amount ($):"
         Height          =   255
         Left            =   840
         TabIndex        =   12
         Top             =   610
         Width           =   855
      End
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete <<"
      Height          =   375
      Left            =   3720
      TabIndex        =   9
      Top             =   720
      Width           =   975
   End
   Begin VB.PictureBox picOptions 
      BorderStyle     =   0  'None
      Height          =   3780
      Index           =   3
      Left            =   -20000
      ScaleHeight     =   3780
      ScaleWidth      =   5685
      TabIndex        =   5
      TabStop         =   0   'False
      Top             =   480
      Width           =   5685
      Begin VB.Frame fraSample4 
         Caption         =   "Sample 4"
         Height          =   1785
         Left            =   2100
         TabIndex        =   8
         Top             =   840
         Width           =   2055
      End
   End
   Begin VB.PictureBox picOptions 
      BorderStyle     =   0  'None
      Height          =   3780
      Index           =   2
      Left            =   -20000
      ScaleHeight     =   3780
      ScaleWidth      =   5685
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   480
      Width           =   5685
      Begin VB.Frame fraSample3 
         Caption         =   "Sample 3"
         Height          =   1785
         Left            =   1545
         TabIndex        =   7
         Top             =   675
         Width           =   2055
      End
   End
   Begin VB.PictureBox picOptions 
      BorderStyle     =   0  'None
      Height          =   3780
      Index           =   1
      Left            =   -20000
      ScaleHeight     =   3780
      ScaleWidth      =   5685
      TabIndex        =   3
      TabStop         =   0   'False
      Top             =   480
      Width           =   5685
      Begin VB.Frame fraSample2 
         Caption         =   "Sample 2"
         Height          =   1785
         Left            =   645
         TabIndex        =   6
         Top             =   300
         Width           =   2055
      End
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Enabled         =   0   'False
      Height          =   375
      Left            =   2520
      TabIndex        =   2
      Top             =   2160
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1320
      TabIndex        =   1
      Top             =   2160
      Width           =   1095
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   2160
      Width           =   1095
   End
   Begin VB.Image imgProp 
      Height          =   180
      Left            =   3840
      Picture         =   "frmCustDivs.frx":0035
      Top             =   1920
      Visible         =   0   'False
      Width           =   180
   End
End
Attribute VB_Name = "frmCustomDivs"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private m_rs As Recordset
Private m_iStockID As Long
Private m_strSymbol As String
Private m_Cn As Connection
Private m_DataDirty As Boolean
Private m_CustDivs As New EtsGeneralLib.EtsDivColl
Private m_bGotData As Boolean
Private m_bReadOnly As Boolean
Private m_BasketDivs As EtsGeneralLib.EtsIndexDivColl
Private m_dtLastExpity As Date
Private m_bSortDate As Boolean
Private m_bSortAmount As Boolean
Private m_frmIndexDivComp As frmIndexDivComponents

Private Sub DrawPictures()
On Error GoTo Err
    Dim iRow As Long
    
    If fgDividend.Rows < 2 Then Exit Sub
    
    For iRow = 1 To fgDividend.Rows - 1
        fgDividend.Cell(flexcpPicture, iRow, 4) = imgProp.Picture
        fgDividend.Cell(flexcpPictureAlignment, iRow, 4) = flexPicAlignLeftCenter
    Next iRow
    Exit Sub
Err:
    Debug.Print "Error while drawing pictures"
End Sub

Private Sub fgDividend_AfterDataRefresh()
    DrawPictures
End Sub

Private Sub fgDividend_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    SetDataDirty (True)
End Sub

Private Sub fgDividend_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Cancel = m_bReadOnly Or Col = 3
End Sub

Private Sub fgDividend_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error GoTo Ex
    If Col = 2 Or Col = 3 Then
        If m_bSortDate Then
            m_rs.Sort = "[Dividend Date] DESC"
        Else
            m_rs.Sort = "[Dividend Date] ASC"
        End If
        m_bSortDate = Not m_bSortDate
    End If
    
    If Col = 4 Then
        If m_bSortAmount Then
            m_rs.Sort = "[Dividend Amount] DESC"
        Else
            m_rs.Sort = "[Dividend Amount] ASC"
        End If
        m_bSortAmount = Not m_bSortAmount
    End If
Ex:
End Sub

Private Sub fgDividend_Click()
    Dim iRow As Long
   
End Sub

Private Sub fgDividend_DblClick()
On Error GoTo Err
    Dim dtDivDate As Date
    If (Not m_BasketDivs Is Nothing) Then
    
        If (m_BasketDivs.Count <= 0) Then Exit Sub
        
        If (Not m_frmIndexDivComp Is Nothing) Then
            dtDivDate = CDate(fgDividend.TextMatrix(fgDividend.Row, 2))
            m_frmIndexDivComp.Init m_iStockID, dtDivDate
            m_frmIndexDivComp.Show vbModal
        End If
        
    End If
    Exit Sub
Err:
    Exit Sub
End Sub

Private Sub Form_Load()
    dtpDivDate.Value = CDate(Now)
End Sub

Public Sub InitRs()
'    Set m_rs = New Recordset
'    Set m_rs.ActiveConnection = gDBW.Connection
'    m_rs.LockType = adLockBatchOptimistic
    SetDataDirty False

End Sub

Public Property Get BasketDivs() As EtsGeneralLib.EtsIndexDivColl
    Set BasketDivs = m_BasketDivs
End Property

Public Property Let BasketDivs(ByRef div As EtsGeneralLib.EtsIndexDivColl)
    Set m_BasketDivs = div
    Dim iCount As Long
    Dim i As Long
    Dim DivAmounts() As Double
    Dim DivDates() As Double
    On Error GoTo Ex
    fgDividend.Redraw = flexRDNone
    Dim dte As Integer
    Dim dtNYTime As Date
    
    dtNYTime = GetNewYorkTime
    If m_BasketDivs.Count > 0 Then
    
      m_BasketDivs.GetDividendCount2 dtNYTime, m_dtLastExpity, CDate("00:00:00"), iCount
      m_BasketDivs.GetDividends2 dtNYTime, m_dtLastExpity, CDate("00:00:00"), iCount, DivAmounts, DivDates, iCount
      For i = 0 To iCount - 1
      m_rs.AddNew
          m_rs.Fields.Item("Dividend Date") = DivDates(i) * 365 + dtNYTime
          m_rs.Fields.Item("Dividend Amount") = DivAmounts(i)
      m_rs.Update
      
      Next i
      AllowChange (Not m_bReadOnly)
      DrawPictures
     
    End If
    
  If m_rs.RecordCount > 0 Then m_rs.MoveFirst

  SetDataDirty False
Ex:
    fgDividend.Redraw = flexRDBuffered
End Property



Public Sub Init(iStockID As Long, strSimbol As String, dtLastExpity As Date, bReadOnly As Boolean, bLoadForIndex As Boolean)
    Err.Clear
    On Error GoTo EH
    
    m_bGotData = False
    m_bReadOnly = bReadOnly
    Set m_BasketDivs = Nothing
    m_dtLastExpity = dtLastExpity
    fgDividend.Redraw = flexRDNone

    Set m_rs = gDBW.usp_MmEtsCustomDividend_Get(iStockID, bLoadForIndex)
    
    m_iStockID = iStockID
    m_strSymbol = strSimbol
    
    Caption = "Custom Dividends [for " & m_strSymbol & "]"
    fgDividend.Enabled = True
    Set fgDividend.DataSource = m_rs
    
    fgDividend.ColHidden(0) = True
    fgDividend.ColHidden(1) = True
    fgDividend.ColHidden(3) = True
    fgDividend.ColFormat(4) = "##,###0.##00"
      

    btnAdd.Enabled = Not m_bReadOnly
    btnDelete.Enabled = Not m_bReadOnly
    btnClear.Enabled = Not m_bReadOnly
    btnApply.Enabled = Not m_bReadOnly
    btnOk.Enabled = Not m_bReadOnly
    m_bSortDate = True
    m_bSortAmount = True
    SetDataDirty False
    If m_rs.RecordCount > 0 Then m_rs.MoveFirst
    
    Set m_frmIndexDivComp = New frmIndexDivComponents
    
    fgDividend.Redraw = flexRDBuffered
    Exit Sub
EH:
    fgDividend.Redraw = flexRDBuffered
    gCmn.ErrorMsgBox Me, "Fail to Init CustomDivs for " & strSimbol & ", " & Err.Description
End Sub

Public Function UpdateCollection() As Boolean
        UpdateCollection = False
        Err.Clear
        On Error Resume Next
        Set m_CustDivs = Nothing

        
        Dim aCurUnd As EtsGeneralLib.UndAtom
        Set aCurUnd = g_Underlying(m_iStockID)
    
        If aCurUnd Is Nothing Then
            gCmn.ErrorMsgBox Me, "Failed to Update CustomDiv collection for " & m_strSymbol & ", " & Err.Description
            Exit Function
        End If
        
        Set aCurUnd.Dividend.CustomDivs = Nothing
        
        If (m_rs.EOF And m_rs.BOF) Then
            UpdateCollection = True
            Exit Function
        End If
        
        Err.Clear
        m_rs.MoveFirst
        If Err <> 0 Then
            UpdateCollection = True
            Exit Function
        End If
        
        'On Error GoTo EH
         Set m_CustDivs = New EtsGeneralLib.EtsDivColl
         Err.Clear
    

        While Not m_rs.EOF
           If CDbl(m_rs.Fields.Item("Dividend Amount")) = CDbl(0) Then
                gCmn.ErrorMsgBox Me, "Failed to add zero dividend. Line removed. "
                m_rs.Delete
                Err.Clear
                                            Else
           
                m_CustDivs.AddNonUnique m_rs.Fields.Item("Dividend Date"), m_rs.Fields.Item("Dividend Amount")
                If Err <> 0 Then
                    Err.Clear
                    gCmn.ErrorMsgBox Me, "Failed to add a dividend (non-unique date or type mismatch). Line removed. " & Err.Description
                    m_rs.Delete
                    Err.Clear
                End If

           End If
           
           m_rs.MoveNext
        Wend
        m_rs.MoveLast
                
        Set aCurUnd.Dividend.CustomDivs = m_CustDivs
        
        If (Not aCurUnd.Dividend.Holidays Is Nothing) Then
                Set m_CustDivs.Holidays = aCurUnd.Dividend.Holidays
        End If
        
        UpdateCollection = True
        Exit Function
EH:
        Set m_CustDivs = Nothing
        Set aCurUnd.Dividend.CustomDivs = Nothing
        gCmn.ErrorMsgBox Me, "Failed to Update CustomDiv collection for " & m_strSymbol & ", " & Err.Description
End Function

Public Property Get CustomDividend() As EtsGeneralLib.EtsDivColl
    On Error Resume Next
    Set CustomDividend = m_CustDivs
End Property

Public Function ReloadDividends(ByVal lStockId As Long) As Boolean
    Err.Clear
    On Error Resume Next
    
    Dim rs As Recordset
    Dim CustDivs As EtsGeneralLib.EtsDivColl
    Dim aCurUnd As EtsGeneralLib.UndAtom

    ReloadDividends = False
    Set CustDivs = Nothing
    
    Set aCurUnd = g_Underlying(lStockId)
    If aCurUnd Is Nothing Then
        Exit Function
    End If
        
    Set aCurUnd.Dividend.CustomDivs = Nothing
        
    Set rs = gDBW.usp_MmCustomDividend_Get(lStockId)
        
    If (rs.EOF And rs.BOF) Then
        ReloadDividends = True
        Exit Function
    End If
        
        
    rs.MoveFirst
    If Err <> 0 Then
        Exit Function
    End If
    Set CustDivs = New EtsGeneralLib.EtsDivColl
    While Not rs.EOF
       If CDbl(rs!DivAmnt) = CDbl(0) Then
            rs.Delete
                                        Else
       
            CustDivs.Add rs!DivYtes, rs!DivAmnt
            If Err <> 0 Then
                rs.Delete
                Err.Clear
            End If
       End If
       
       rs.MoveNext
    Wend
    rs.Close
    
    Set aCurUnd.Dividend.CustomDivs = CustDivs
    
    ReloadDividends = True
    Exit Function
EH:
        Set CustDivs = Nothing
        Set aCurUnd.Dividend.CustomDivs = Nothing
        ReloadDividends = False
End Function

Public Property Get GotData() As Boolean
    On Error Resume Next
    GotData = m_bGotData
End Property



Private Sub AllowChange(bAlow As Boolean)
    'VSFlexGrid1.AllowDelete = bAlow
    'VSFlexGrid1.AllowUpdate = bAlow
    frmAdd.Enabled = bAlow
End Sub



Private Sub btnAdd_Click()
    Err.Clear
    
    On Error Resume Next
    
    Dim dblValue As Double
    Dim dtValue As Date
    Dim lStep   As Long
    Dim lDate As Long
    
    
    
    dblValue = CDbl(txtDivAmnt.Text)
    If Err <> 0 Then
        gCmn.ErrorMsgBox Me, "Wrong Dividend Amount Value"
        txtDivAmnt.SetFocus
        Exit Sub
    End If
    
    If dblValue = 0 Then
        gCmn.ErrorMsgBox Me, "The Dividend Amount Value is not in acceptable range"
        txtDivAmnt.SetFocus
        Exit Sub
    End If
    
    dtValue = CDate(dtpDivDate.Value)
    If Err <> 0 Then
        gCmn.ErrorMsgBox Me, "Wrong Dividend Date Value"
        dtpDivDate.SetFocus
        Exit Sub
    End If
    
    
    m_rs.AddNew
    m_rs!StockID = m_iStockID
    m_rs.Fields.Item("Dividend Date") = Int(dtValue)
    m_rs.Fields.Item("Dividend Amount") = CDbl(dblValue)
    m_rs.Update

    If Err <> 0 Then
        gCmn.ErrorMsgBox Me, "Failed to add a dividend!"
        Exit Sub
    End If
    SetDataDirty True
    
    lStep = CLng(txtStep.Text)
    If Err <> 0 Then
        gCmn.ErrorMsgBox Me, "Wrong Date Step Value"
        dtpDivDate.SetFocus
        Exit Sub
    End If
    
    dtpDivDate.Value = dtpDivDate.Value + lStep
End Sub

Private Sub btnApply_Click()
    Err.Clear
    On Error GoTo EH
    gDBW.Connection().BeginTrans
    m_rs.UpdateBatch
    If UpdateCollection Then
        m_bGotData = True
        m_rs.Sort = "[Dividend Date]"
        gDBW.Connection().CommitTrans
                        Else
        gDBW.Connection().RollbackTrans
        m_bGotData = False
    End If
    SetDataDirty False
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to Apply: " & Err.Description
    gDBW.Connection().RollbackTrans
End Sub

Private Sub btnCancel_Click()
    m_rs.CancelBatch
    If Not m_rs.BOF Then m_rs.MoveFirst
    Unload Me
End Sub

Private Sub btnClear_Click()
    On Error Resume Next
    If (m_rs.EOF And m_rs.BOF) Then
        Exit Sub
    End If
    m_rs.MoveFirst
    Err.Clear
    
    SetDataDirty True
    
    While Not m_rs.EOF
        m_rs.Delete
        m_rs.MoveNext
        If Err <> 0 Then
            gCmn.ErrorMsgBox Me, "Error occured while deleting a dividend: " & Err.Description
            Exit Sub
        End If
    Wend
    
    
End Sub

Private Sub btnDelete_Click()
    On Error Resume Next
    If (m_rs.EOF And m_rs.BOF) Then
        Exit Sub
    End If
    
    SetDataDirty True
    
    If Not m_rs.EOF Then
        m_rs.Delete
        m_rs.MoveNext
    End If
    
End Sub

Private Sub btnOk_Click()
    btnApply_Click
    Unload Me
End Sub

Private Sub txtDivAmnt_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = CLng(13) Then
        btnAdd_Click
    End If
End Sub

Private Sub SetDataDirty(ByVal IsDirty As Boolean)
    If m_bReadOnly = False Then
        m_DataDirty = IsDirty
        btnApply.Enabled = m_DataDirty
    Else
        m_DataDirty = False
    End If
End Sub


