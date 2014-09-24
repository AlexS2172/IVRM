VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMktStrOptionPairEditor 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Option Call/Put Editor"
   ClientHeight    =   4005
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5550
   Icon            =   "frmMktStrOptionPairEditor.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4005
   ScaleWidth      =   5550
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   3375
      Left            =   120
      ScaleHeight     =   3345
      ScaleWidth      =   5265
      TabIndex        =   0
      Top             =   120
      Width           =   5295
      Begin MSComCtl2.DTPicker dtpCloseTime 
         Height          =   345
         Left            =   3000
         TabIndex        =   22
         Top             =   840
         Width           =   2175
         _ExtentX        =   3836
         _ExtentY        =   609
         _Version        =   393216
         CustomFormat    =   "hh:mm tt"
         Format          =   50397186
         CurrentDate     =   39743.5652777778
      End
      Begin MSComCtl2.DTPicker dtpExpiryOV 
         Height          =   300
         Left            =   3000
         TabIndex        =   20
         Top             =   480
         Width           =   2175
         _ExtentX        =   3836
         _ExtentY        =   529
         _Version        =   393216
         CustomFormat    =   "MMM, dd yyyy hh:mm tt"
         Format          =   20578307
         CurrentDate     =   39540.5652777778
      End
      Begin VB.CheckBox chkManuallyEntered 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Flex option"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   1
         Left            =   1800
         TabIndex        =   19
         Top             =   3000
         Width           =   1695
      End
      Begin VB.TextBox txtStrike 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   720
         TabIndex        =   1
         Text            =   "txtStrike"
         Top             =   120
         Width           =   1215
      End
      Begin VB.Frame fraCall 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Call"
         ForeColor       =   &H80000008&
         Height          =   1575
         Left            =   120
         TabIndex        =   3
         Top             =   1320
         Width           =   2295
         Begin VB.TextBox txtExportSymbol 
            Appearance      =   0  'Flat
            Height          =   300
            Index           =   1
            Left            =   960
            TabIndex        =   15
            Text            =   "txtExportSymbol"
            Top             =   600
            Width           =   1215
         End
         Begin VB.TextBox txtSymbol 
            Appearance      =   0  'Flat
            Height          =   300
            Index           =   1
            Left            =   960
            TabIndex        =   4
            Text            =   "txtSymbol"
            Top             =   240
            Width           =   1215
         End
         Begin ElladaFlatControls.FlatButton btnPrices 
            Height          =   300
            Index           =   1
            Left            =   960
            TabIndex        =   5
            Top             =   960
            Width           =   1215
            _ExtentX        =   2143
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
            Caption         =   "Prices..."
         End
         Begin VB.Label lblExportSymbol 
            Alignment       =   1  'Right Justify
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            BackStyle       =   0  'Transparent
            Caption         =   "Import Id :"
            ForeColor       =   &H80000008&
            Height          =   255
            Index           =   1
            Left            =   0
            TabIndex        =   16
            Top             =   600
            Width           =   855
         End
         Begin VB.Label lblSymbol 
            Alignment       =   1  'Right Justify
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            BackStyle       =   0  'Transparent
            Caption         =   "Symbol :"
            ForeColor       =   &H80000008&
            Height          =   255
            Index           =   1
            Left            =   120
            TabIndex        =   12
            Top             =   240
            Width           =   735
         End
      End
      Begin VB.Frame fraPut 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Put"
         ForeColor       =   &H80000008&
         Height          =   1575
         Left            =   2640
         TabIndex        =   6
         Top             =   1320
         Width           =   2415
         Begin VB.TextBox txtExportSymbol 
            Appearance      =   0  'Flat
            Height          =   300
            Index           =   0
            Left            =   960
            TabIndex        =   17
            Text            =   "txtExportSymbol"
            Top             =   600
            Width           =   1215
         End
         Begin VB.TextBox txtSymbol 
            Appearance      =   0  'Flat
            Height          =   300
            Index           =   0
            Left            =   960
            TabIndex        =   7
            Text            =   "txtSymbol"
            Top             =   240
            Width           =   1215
         End
         Begin ElladaFlatControls.FlatButton btnPrices 
            Height          =   300
            Index           =   0
            Left            =   960
            TabIndex        =   8
            Top             =   960
            Width           =   1215
            _ExtentX        =   2143
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
            Caption         =   "Prices..."
         End
         Begin VB.Label lblExportSymbol 
            Alignment       =   1  'Right Justify
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            BackStyle       =   0  'Transparent
            Caption         =   "Import Id :"
            ForeColor       =   &H80000008&
            Height          =   255
            Index           =   0
            Left            =   0
            TabIndex        =   18
            Top             =   600
            Width           =   855
         End
         Begin VB.Label lblSymbol 
            Alignment       =   1  'Right Justify
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            BackStyle       =   0  'Transparent
            Caption         =   "Symbol :"
            ForeColor       =   &H80000008&
            Height          =   255
            Index           =   0
            Left            =   120
            TabIndex        =   11
            Top             =   240
            Width           =   735
         End
      End
      Begin MSComCtl2.DTPicker dtpExpire 
         Height          =   300
         Left            =   3000
         TabIndex        =   2
         Top             =   120
         Width           =   2175
         _ExtentX        =   3836
         _ExtentY        =   529
         _Version        =   393216
         Format          =   20578305
         CurrentDate     =   38718
         MinDate         =   38718
      End
      Begin VB.Label lblTradingClose 
         BackStyle       =   0  'Transparent
         Caption         =   "TradingClose:"
         Height          =   255
         Left            =   1800
         TabIndex        =   23
         Top             =   840
         Width           =   975
      End
      Begin VB.Label Label1 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "ExpiryOV:"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   2040
         TabIndex        =   21
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblStrike 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Strike :"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   120
         TabIndex        =   14
         Top             =   120
         Width           =   495
      End
      Begin VB.Label Label3 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Expiry :"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   2160
         TabIndex        =   13
         Top             =   120
         Width           =   615
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   4080
      TabIndex        =   10
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
      Default         =   -1  'True
      Height          =   300
      Left            =   2640
      TabIndex        =   9
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
End
Attribute VB_Name = "frmMktStrOptionPairEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_bChanged As Boolean
Private mbDirty As Boolean

Private mfStrike As Double
Private mdExpiry As Date
Private m_OptPair As clsOptionPairAtom
Private m_iUnderlyingID As Long
Private m_iUnderlyingTypeID As Long
Private m_iExpCalendarID As Long

Private mbLoading As Boolean

Public Function Execute( _
    ByVal iUnderlyingID As Long, _
    ByVal iUnderlyingTypeID As Long, _
    ByVal iExpCalendarID As Long, _
    ByRef OptPair As clsOptionPairAtom, _
    ByVal bEditScreen _
) As Boolean
    On Error GoTo EH
    mbDirty = False
    Set m_OptPair = OptPair
    m_iUnderlyingID = iUnderlyingID
    m_iUnderlyingTypeID = iUnderlyingTypeID
    m_iExpCalendarID = iExpCalendarID
    If bEditScreen Then
        Load Me
        If m_iUnderlyingTypeID = 4 Then Me.chkManuallyEntered(1).Enabled = False
        Me.Show vbModal
        Execute = mbDirty
        Unload Me
    Else
        LoadData
        SaveDataForOptionAndFuture
    End If
    Exit Function
EH:
    ShowError "Error loading Option Pair Editor"
End Function

Private Sub btnOK_Click()
On Error Resume Next
    If m_bChanged Then SaveDataForOptionAndFuture
    If Not m_bChanged Then Me.Hide
End Sub

Private Sub btnCancel_Click()
On Error Resume Next
    Me.Hide
End Sub

Private Sub btnPrices_Click(Index As Integer)
Dim iOptID As Variant
On Error GoTo EH

    If Index Then
        iOptID = m_OptPair.iCallID
    Else
        iOptID = m_OptPair.iPutID
    End If
    
    If IsNull(iOptID) Then
        ShowWarning "Define and save option first"
        Exit Sub
    End If
    If m_bChanged Then
        ShowWarning "Apply changes first"
    Else
        mbDirty = (frmMarketValues.Execute(txtSymbol(Index).Text, IIf(m_iUnderlyingTypeID <> GINT_FUTURES, CONTRACT_TYPE_OPTION, 0))) Or mbDirty
    End If
Exit Sub
EH:
    ShowError "Unable to load Market Values View"
End Sub

Private Sub dtpCloseTime_Change()
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub Form_Load()
On Error Resume Next
    gCmn.CustomizeForm Me
    
    mbDirty = False
    LoadData
    m_bChanged = False
End Sub

Private Sub LoadData()
On Error GoTo EH
    mbLoading = True
    txtSymbol(1) = m_OptPair.sCallSymbol
    txtSymbol(0) = m_OptPair.sPutSymbol
    txtExportSymbol(1) = m_OptPair.sCallImportSymbol
    txtExportSymbol(0) = m_OptPair.sPutImportSymbol
    
    chkManuallyEntered(1).Value = IIf(m_OptPair.bCallManual, 1, 0)
    'chkManuallyEntered(0).Value = IIf(m_OptPair.bPutManual, 1, 0)
    
    txtStrike.Text = gCmn.FmtDbl(m_OptPair.fStrike)
    dtpExpire.Value = m_OptPair.dExpiry
    dtpExpiryOV.Value = m_OptPair.dExpiryOV
    dtpCloseTime.Value = ClipDays(m_OptPair.dTradingClose)
    
    mbLoading = False
Exit Sub
EH:
    ShowError "Unable to load data"
End Sub

Public Sub SaveDataForOptionAndFuture()  'sosed
    On Error GoTo EH
    Dim nRet&, vId As Variant, nRootID&
    Dim rst As ADODB.Recordset
    
'    If Not p_OptPair Is Nothing Then
'        Set m_OptPair = p_OptPair
'    Else
        If Not CheckData Then Exit Sub
'    End If

    With m_OptPair
        If Len(.sCallSymbol) Then
            If m_iUnderlyingTypeID <> 4 Then ' general option
                nRet = gDBW.usp_Option_Save(.iCallID, .sCallSymbol, Null, m_iUnderlyingID, True, .fStrike, .dExpiry, .dExpiryOV, .dTradingClose, .bCallManual)
                If nRet < 0 Then GoTo EH
                If IsNull(.iCallID) And nRet > 0 Then .iCallID = nRet
            Else ' futures option
                vId = .iCallID
                nRet = gDBW.usp_BoFutureOption_Save(vId, m_iUnderlyingID, .sCallSymbol, , , 1, .fStrike, .dExpiry, .dExpiryOV, .dTradingClose, 1)
                If nRet < 0 Then GoTo EH
                If IsNull(.iCallID) And nRet > 0 Then .iCallID = vId
            End If
                
            gDBW.usp_BoContractExportSymbol_Save .iCallID, .sCallImportSymbol
        End If
        
        If Len(.sPutSymbol) Then
            If m_iUnderlyingTypeID <> 4 Then ' general option
                nRet = gDBW.usp_Option_Save(.iPutID, .sPutSymbol, Null, m_iUnderlyingID, False, .fStrike, .dExpiry, .dExpiryOV, .dTradingClose, .bCallManual)
                If nRet < 0 Then GoTo EH
                If IsNull(.iPutID) And nRet > 0 Then .iPutID = nRet
            Else ' futures option
                vId = .iPutID
                nRet = gDBW.usp_BoFutureOption_Save(vId, m_iUnderlyingID, .sPutSymbol, , , 0, .fStrike, .dExpiry, .dExpiryOV, .dTradingClose, 1)
                If nRet < 0 Then GoTo EH
                If IsNull(.iPutID) And nRet > 0 Then .iPutID = vId
            End If
        
            gDBW.usp_BoContractExportSymbol_Save .iPutID, .sPutImportSymbol
        End If
        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
        If m_iUnderlyingTypeID <> 4 And .bCallManual Then ' do this only for flex option
            Dim sRootName As String, l&
            
            'make a Root Name
            If Len(Trim$(.sPutSymbol)) Then
                l = Len(Trim$(.sPutSymbol))
                sRootName = Left(Trim$(.sPutSymbol), l - 2)
            Else
                If Len(.sCallSymbol) Then
                    l = Len(Trim$(.sCallSymbol))
                    sRootName = Left(Trim$(.sCallSymbol), l - 2)
                End If
            End If
            'get root id by UndId an RootName
            If Len(sRootName) > 0 Then 'check is root found
                Set rst = gDBW.usp_OptionRootID_Get(sRootName, m_iUnderlyingID)
                If rst.RecordCount = 1 Then
                    Dim RootToPub As New clsOptRootAtom
                    With RootToPub
                        .LotSize = gCmn.ReadLng(rst!iLotSize)
                        .UnderlyingID = gCmn.ReadLng(rst!iUnderlyingID)
                        .Symbol = gCmn.ReadStr(rst!vcSymbol)
                        .RootID = gCmn.ReadStr(rst!iOptionRootID)
                        .IsSynthetic = gCmn.ReadStr(rst!tiIsSynthetic)
                        .IsFittingEnabled = gCmn.ReadStr(rst!tiIsFitEnabled)
                        'Pub new FlexOptionRoot
                        PubNewFlexOptionRoot .Symbol, .RootID, .UnderlyingID, .LotSize
                    End With
                Else
                    Debug.Print "There are more then one root with the same name and underlying id."
                End If
            End If
        End If
        '''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
    End With
    mbDirty = True
    m_bChanged = False
    Exit Sub
EH:
    ShowError "Fail to save data"
End Sub

Private Function CheckData() As Boolean
    On Error GoTo EH
    Dim s As String, l As Long, rs As ADODB.Recordset, dtExpiry As Date, dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    mdExpiry = dtpExpire.Value
    dtExpiryOV = dtpExpiryOV.Value
    dtTradingClose = dtpCloseTime.Value
       
    s = Trim(txtStrike.Text)
    If IsNumeric(s) Then
        mfStrike = gCmn.ReadDbl(s)
    Else
        ShowWarning "Strike is not a valid value"
        Exit Function
    End If
    
    If mfStrike <= 0# Then
        ShowWarning "Strike must be positive value"
        Exit Function
    End If
    
    With m_OptPair
        .dExpiry = mdExpiry
        .dExpiryOV = dtExpiryOV
        .fStrike = mfStrike
        .sCallSymbol = Trim(UCase(txtSymbol(1).Text))
        .bCallManual = chkManuallyEntered(1).Value <> 0
        .sPutSymbol = Trim(UCase(txtSymbol(0).Text))
        .bPutManual = chkManuallyEntered(1).Value <> 0
        .sCallImportSymbol = Trim(UCase(txtExportSymbol(1).Text))
        .sPutImportSymbol = Trim(UCase(txtExportSymbol(0).Text))
        .dTradingClose = dtTradingClose
    End With
    
    If (Len(Trim$(m_OptPair.sCallSymbol)) < 3 And Len(Trim$(m_OptPair.sCallSymbol)) > 0) Or _
        (Len(Trim$(m_OptPair.sPutSymbol)) < 3 And Len(Trim$(m_OptPair.sPutSymbol)) > 0) Then
        ShowWarning "Symbols minimum length is 3"
        Exit Function
    End If
    
    If m_OptPair.sCallSymbol = m_OptPair.sPutSymbol Then
        ShowWarning "Symbols must be different"
        Exit Function
    End If
    
    If Len(Trim$(m_OptPair.sCallSymbol)) > 0 And Len(Trim$(m_OptPair.sPutSymbol)) > 0 And _
        Len(Trim$(m_OptPair.sCallSymbol)) <> Len(Trim$(m_OptPair.sPutSymbol)) Then
        ShowWarning "The length of Option pair symbols is not equal"
        Exit Function
    End If
    
    l = Len(Trim$(m_OptPair.sCallSymbol))
    
    If m_iUnderlyingTypeID <> 4 And Len(Trim$(m_OptPair.sCallSymbol)) > 0 And Len(Trim$(m_OptPair.sPutSymbol)) > 0 Then
        If Left(Trim$(m_OptPair.sCallSymbol), l - 2) <> Left(Trim$(m_OptPair.sPutSymbol), l - 2) Then
            ShowWarning "Option pair roots must be equal"
            Exit Function
        End If
    End If
    
    CheckData = True
Exit Function
EH:
    ShowError "Error checking values"
End Function

Private Sub txtStrike_Change()
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub txtStrike_GotFocus()
On Error Resume Next
    gCmn.SelectText txtStrike
End Sub

Private Sub txtExportSymbol_Change(Index As Integer)
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub txtExportSymbol_GotFocus(Index As Integer)
On Error Resume Next
    gCmn.SelectText txtExportSymbol(Index)
End Sub

Private Sub dtpExpire_Change()
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
    dtpExpiryOV.Value = dtpExpire.Value
End Sub

Private Sub dtpExpiryOV_Change()
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub chkManuallyEntered_Click(Index As Integer)
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub txtSymbol_Change(Index As Integer)
On Error Resume Next
    If mbLoading Then Exit Sub
    m_bChanged = True
End Sub

Private Sub txtSymbol_GotFocus(Index As Integer)
On Error Resume Next
    gCmn.SelectText txtSymbol(Index)
End Sub

