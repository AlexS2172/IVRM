VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{CDE57A40-8B86-11D0-B3C6-00A0C90AEA82}#1.0#0"; "MSDATGRD.OCX"
Begin VB.Form frmCustomDivs 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Custom Dividends [for ...]"
   ClientHeight    =   2805
   ClientLeft      =   2565
   ClientTop       =   1455
   ClientWidth     =   8655
   Icon            =   "frmCustDivs.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2805
   ScaleWidth      =   8655
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnAdd 
      Caption         =   "Add     >>"
      Height          =   375
      Left            =   3840
      TabIndex        =   19
      Top             =   240
      Width           =   1095
   End
   Begin VB.CommandButton btnClear 
      Caption         =   "Delete All"
      Height          =   375
      Left            =   3840
      TabIndex        =   18
      Top             =   1200
      Width           =   1095
   End
   Begin VB.Frame frmAdd 
      Caption         =   "Dividend Information"
      Height          =   1935
      Left            =   120
      TabIndex        =   11
      Top             =   120
      Width           =   3615
      Begin VB.TextBox txtDivAmnt 
         Height          =   315
         Left            =   1800
         TabIndex        =   17
         Text            =   "0.0"
         Top             =   720
         Width           =   1454
      End
      Begin VB.TextBox txtStep 
         Height          =   315
         Left            =   1800
         TabIndex        =   15
         Text            =   "1"
         Top             =   1200
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
         TabIndex        =   12
         Top             =   240
         Width           =   1454
         _ExtentX        =   2566
         _ExtentY        =   556
         _Version        =   393216
         Format          =   164298753
         CurrentDate     =   38251
      End
      Begin VB.Label lblStep 
         Caption         =   "Add with step (days):"
         Height          =   255
         Left            =   120
         TabIndex        =   16
         Top             =   1200
         Width           =   1575
      End
      Begin VB.Label Label1 
         Caption         =   "Date:"
         Height          =   255
         Left            =   1260
         TabIndex        =   14
         Top             =   240
         Width           =   375
      End
      Begin VB.Label Label2 
         Caption         =   "Amount ($):"
         Height          =   255
         Left            =   840
         TabIndex        =   13
         Top             =   720
         Width           =   855
      End
   End
   Begin MSDataGridLib.DataGrid VSFlexGrid1 
      Height          =   2535
      Left            =   5040
      TabIndex        =   10
      Top             =   120
      Width           =   3495
      _ExtentX        =   6165
      _ExtentY        =   4471
      _Version        =   393216
      AllowUpdate     =   -1  'True
      AllowArrows     =   -1  'True
      HeadLines       =   1
      RowHeight       =   15
      WrapCellPointer =   -1  'True
      FormatLocked    =   -1  'True
      AllowDelete     =   -1  'True
      BeginProperty HeadFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ColumnCount     =   2
      BeginProperty Column00 
         DataField       =   "DivYtes"
         Caption         =   "Dividend Date"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   1
            Format          =   "M/d/yyyy"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1049
            SubFormatType   =   3
         EndProperty
      EndProperty
      BeginProperty Column01 
         DataField       =   "DivAmnt"
         Caption         =   "Dividend Amount"
         BeginProperty DataFormat {6D835690-900B-11D0-9484-00A0C91110ED} 
            Type            =   1
            Format          =   "0.00"
            HaveTrueFalseNull=   0
            FirstDayOfWeek  =   0
            FirstWeekOfYear =   0
            LCID            =   1049
            SubFormatType   =   1
         EndProperty
      EndProperty
      SplitCount      =   1
      BeginProperty Split0 
         BeginProperty Column00 
         EndProperty
         BeginProperty Column01 
            ColumnWidth     =   1604.976
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete <<"
      Height          =   375
      Left            =   3840
      TabIndex        =   9
      Top             =   720
      Width           =   1095
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
      Left            =   2640
      TabIndex        =   2
      Top             =   2280
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1320
      TabIndex        =   1
      Top             =   2280
      Width           =   1215
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   2280
      Width           =   1095
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
Private m_CN As Connection
Private m_DataDirty As Boolean
Private m_CustDivs As New EtsGeneralLib.EtsDivColl
Private m_bGotData As Boolean


Private Sub Form_Load()
    dtpDivDate.Value = CDate(Now)
End Sub

Public Sub InitRs()
'    Set m_rs = New Recordset
'    Set m_rs.ActiveConnection = gDBW.Connection
'    m_rs.LockType = adLockBatchOptimistic
    SetDataDirty False

End Sub

Public Sub Init(iStockID As Long, strSimbol As String)
    Err.Clear
    On Error GoTo EH
    
    m_bGotData = False
    
    
    Set m_rs = gDBW.usp_MmCustomDividend_Get(iStockID)
    
    m_iStockID = iStockID
    m_strSymbol = strSimbol
    
    Caption = "Custom Dividends [for " & m_strSymbol & "]"
    Set VSFlexGrid1.DataSource = m_rs

    
    Exit Sub
EH:
    ShowError "Fail to Init CustomDivs for " & strSimbol & ", " & Err.Description
End Sub

'Private Function UpdateCollection() As Boolean
'        UpdateCollection = False
'        Err.Clear
'        On Error Resume Next
'        Set m_CustDivs = Nothing
'
'
'        Dim aCurUnd As EtsGeneralLib.UndAtom
'        If Not g_Underlying Is Nothing Then
'            Set aCurUnd = g_Underlying(m_iStockID)
'        End If
'
'        If Not aCurUnd Is Nothing Then
'            Set aCurUnd.Dividend.CustomDivs = Nothing
'                                    Else
'            UpdateCollection = True
'            Exit Function
'        End If
'
'
'        If (m_rs.EOF And m_rs.BOF) Then
'            UpdateCollection = True
'            Exit Function
'        End If
'
'
'        m_rs.MoveFirst
'        If Err <> 0 Then
'            UpdateCollection = True
'            Exit Function
'        End If
'
'        If Not aCurUnd Is Nothing Then
'         Set m_CustDivs = New EtsGeneralLib.EtsDivColl
'         Err.Clear
'        End If
'
'
'        While Not m_rs.EOF
'           If CDbl(m_rs!DivAmnt) = CDbl(0) Then
'                ShowError "Failed to add zero dividend. Line removed. "
'                m_rs.Delete
'                Err.Clear
'                                            Else
'
'                m_CustDivs.Add m_rs!DivYtes, m_rs!DivAmnt
'                If Err <> 0 Then
'                    Err.Clear
'                    ShowError "Failed to add a dividend (non-unique date or type mismatch). Line removed. " & Err.Description
'                    m_rs.Delete
'                    Err.Clear
'                End If
'
'           End If
'
'           m_rs.MoveNext
'        Wend
'        m_rs.MoveLast
'
'
'        Set aCurUnd.Dividend.CustomDivs = m_CustDivs
'        UpdateCollection = True
'        Exit Function
'EH:
'        Set m_CustDivs = Nothing
'        Set aCurUnd.Dividend.CustomDivs = Nothing
'        ShowError "Failed to Update CustomDiv collection for " & m_strSymbol & ", " & Err.Description
'End Function

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
'    If Not g_Underlying Is Nothing Then
'        Set aCurUnd = g_Underlying(lStockId)
'    End If
    
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
    VSFlexGrid1.AllowDelete = bAlow
    VSFlexGrid1.AllowUpdate = bAlow
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
        ShowError "Wrong Dividend Amount Value"
        txtDivAmnt.SetFocus
        Exit Sub
    End If
    
    If dblValue = 0 Then
        ShowError "Wrong Dividend Amount Value (null)"
        txtDivAmnt.SetFocus
        Exit Sub
    End If
    
    dtValue = CDate(dtpDivDate.Value)
    If Err <> 0 Then
        ShowError "Wrong Dividend Date Value"
        dtpDivDate.SetFocus
        Exit Sub
    End If
    
    
    m_rs.AddNew
    m_rs!StockID = m_iStockID
    m_rs!DivYtes = Int(dtValue)
    m_rs!DivAmnt = CDbl(dblValue)
    m_rs.Update

    If Err <> 0 Then
        ShowError "Failed to add a dividend!"
        Exit Sub
    End If
    SetDataDirty True
    
    lStep = CLng(txtStep.Text)
    If Err <> 0 Then
        ShowError "Wrong Date Step Value"
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
    'If UpdateCollection Then
        m_bGotData = True
        m_rs.Sort = "DivYtes"
        gDBW.Connection().CommitTrans
'     Else
'        gDBW.Connection().RollbackTrans
'        m_bGotData = False
    'End If
    SetDataDirty False
    Exit Sub
EH:
    ShowError "Fail to Apply: " & Err.Description
    gDBW.Connection().RollbackTrans
End Sub

Private Sub btnCancel_Click()
    m_rs.CancelBatch
    m_rs.Sort = "DivYtes"
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
            ShowError "Error occured while deleting a dividend: " & Err.Description
            Exit Sub
        End If
    Wend
    
    
End Sub

Private Sub btnDelete_Click()
    On Error Resume Next
    Err.Clear
    If Not (m_rs.BOF Or m_rs.EOF) Then
        m_rs.Delete
    End If
    If Err <> 0 Then
        ShowError "Error occured while deleting a dividend: " & Err.Description
        Exit Sub
    End If
    SetDataDirty True
End Sub

Private Sub btnOK_Click()
    btnApply_Click
    Unload Me
End Sub

Private Sub txtDivAmnt_KeyDown(KeyCode As Integer, Shift As Integer)
    If KeyCode = CLng(13) Then
        btnAdd_Click
    End If
End Sub

Private Sub SetDataDirty(ByVal IsDirty As Boolean)
    m_DataDirty = IsDirty
    btnApply.Enabled = m_DataDirty
End Sub

Private Sub VSFlexGrid1_Change()
    SetDataDirty True
End Sub

