VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmCorpAction 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Corporate Action Manager"
   ClientHeight    =   8265
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8730
   Icon            =   "frmCorpAction.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8265
   ScaleWidth      =   8730
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VSFlex7Ctl.VSFlexGrid fgOptions 
      Height          =   4455
      Left            =   120
      TabIndex        =   5
      Top             =   3240
      Width           =   8415
      _cx             =   14843
      _cy             =   7858
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
      Begin VB.Timer Timer2 
         Interval        =   500
         Left            =   4440
         Top             =   720
      End
   End
   Begin ElladaFlatControls.FlatButton btnExecute 
      Height          =   300
      Left            =   5760
      TabIndex        =   4
      Top             =   7800
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
      Caption         =   "&Execute"
   End
   Begin ElladaFlatControls.FlatButton btnCopyLog 
      Height          =   300
      Left            =   3720
      TabIndex        =   30
      Top             =   7800
      Visible         =   0   'False
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
      Caption         =   "Copy log to clipboard"
   End
   Begin VB.Frame fraParams 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Split parameters"
      ForeColor       =   &H80000008&
      Height          =   1575
      Left            =   120
      TabIndex        =   7
      Top             =   840
      Width           =   8415
      Begin VB.Frame fraFractShares 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Fractional Num of Shares"
         ForeColor       =   &H80000008&
         Height          =   1335
         Left            =   5640
         TabIndex        =   24
         Top             =   120
         Width           =   2655
         Begin VB.OptionButton optRuleCash 
            BackColor       =   &H80000005&
            Caption         =   "Cash paid for fractional part"
            Height          =   255
            Left            =   120
            TabIndex        =   27
            Top             =   240
            Width           =   2295
         End
         Begin VB.OptionButton optRoundUp 
            BackColor       =   &H80000005&
            Caption         =   "Num of shares Round up"
            Height          =   255
            Left            =   120
            TabIndex        =   26
            Top             =   600
            Width           =   2295
         End
         Begin VB.OptionButton optRoundDown 
            BackColor       =   &H80000005&
            Caption         =   "Num of shares Round down"
            Height          =   255
            Left            =   120
            TabIndex        =   25
            Top             =   960
            Width           =   2295
         End
      End
      Begin VB.TextBox txtNumCont 
         Appearance      =   0  'Flat
         Enabled         =   0   'False
         Height          =   285
         Left            =   4200
         TabIndex        =   14
         Top             =   960
         Width           =   1215
      End
      Begin VB.TextBox txtMult 
         Appearance      =   0  'Flat
         Height          =   285
         Left            =   1200
         TabIndex        =   12
         Top             =   960
         Width           =   1215
      End
      Begin VB.TextBox txtDelivPerCont 
         Appearance      =   0  'Flat
         Height          =   285
         Left            =   4200
         TabIndex        =   10
         Top             =   360
         Width           =   1215
      End
      Begin VB.TextBox txtRate 
         Appearance      =   0  'Flat
         Height          =   285
         Left            =   1200
         TabIndex        =   8
         Top             =   360
         Width           =   1215
      End
      Begin VB.Label lblNumCont 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Number of Contracts :"
         Height          =   255
         Left            =   2520
         TabIndex        =   15
         Top             =   960
         Width           =   1575
      End
      Begin VB.Label lblMult 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Multiplier :"
         Height          =   255
         Left            =   360
         TabIndex        =   13
         Top             =   960
         Width           =   735
      End
      Begin VB.Label lblDelivPerCont1 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Delivery per Contract :"
         Height          =   255
         Left            =   2520
         TabIndex        =   11
         Top             =   360
         Width           =   1575
      End
      Begin VB.Label lblRate 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Split Rate :"
         Height          =   255
         Left            =   120
         TabIndex        =   9
         Top             =   360
         Width           =   975
      End
   End
   Begin MSComCtl2.DTPicker dtpEffecDate 
      Height          =   315
      Left            =   7080
      TabIndex        =   1
      Top             =   360
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   556
      _Version        =   393216
      Enabled         =   0   'False
      Format          =   163119105
      CurrentDate     =   36936
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   7200
      TabIndex        =   3
      Top             =   7800
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
      Caption         =   "&Close"
   End
   Begin MSComctlLib.TreeView tvwLog 
      Height          =   4455
      Left            =   120
      TabIndex        =   31
      Top             =   3240
      Visible         =   0   'False
      Width           =   8415
      _ExtentX        =   14843
      _ExtentY        =   7858
      _Version        =   393217
      Indentation     =   106
      Style           =   2
      BorderStyle     =   1
      Appearance      =   0
   End
   Begin VB.Frame fraParamsM 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Merging/Spin-off parameters"
      ForeColor       =   &H80000008&
      Height          =   1935
      Left            =   120
      TabIndex        =   17
      Top             =   840
      Width           =   8415
      Begin VB.TextBox txtMultM 
         Appearance      =   0  'Flat
         Height          =   285
         Left            =   1200
         TabIndex        =   18
         Top             =   960
         Width           =   1215
      End
      Begin VSFlex7Ctl.VSFlexGrid fgDelivPerCont 
         Height          =   1455
         Left            =   2520
         TabIndex        =   21
         Top             =   360
         Width           =   4335
         _cx             =   7646
         _cy             =   2566
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
         Begin VB.Timer Timer1 
            Interval        =   500
            Left            =   2400
            Top             =   480
         End
      End
      Begin ElladaFlatControls.FlatButton btnParamAdd 
         Height          =   300
         Left            =   6960
         TabIndex        =   22
         Top             =   360
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
         Caption         =   "&Add"
      End
      Begin ElladaFlatControls.FlatButton btnParamDel 
         Height          =   300
         Left            =   6960
         TabIndex        =   23
         Top             =   840
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
         Caption         =   "&Delete"
      End
      Begin MSForms.ComboBox cboSymbol 
         Height          =   315
         Left            =   1200
         TabIndex        =   29
         Top             =   360
         Width           =   1215
         VariousPropertyBits=   746604571
         BorderStyle     =   1
         DisplayStyle    =   7
         Size            =   "2143;556"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lblSymbol 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Underlying :"
         Height          =   255
         Left            =   120
         TabIndex        =   28
         Top             =   360
         Width           =   975
      End
      Begin VB.Label lblDelivPerContM 
         BackStyle       =   0  'Transparent
         Caption         =   "Delivery per Contract:"
         Height          =   255
         Left            =   2520
         TabIndex        =   20
         Top             =   120
         Width           =   2415
      End
      Begin VB.Label lblMultM 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Multiplier :"
         Height          =   255
         Left            =   360
         TabIndex        =   19
         Top             =   960
         Width           =   735
      End
   End
   Begin MSForms.ComboBox cboCAType 
      Height          =   315
      Left            =   120
      TabIndex        =   16
      Top             =   360
      Width           =   3855
      VariousPropertyBits=   746604571
      BorderStyle     =   1
      DisplayStyle    =   7
      Size            =   "6800;556"
      MatchEntry      =   1
      ShowDropButtonWhen=   1
      SpecialEffect   =   0
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblOptions 
      BackStyle       =   0  'Transparent
      Caption         =   "Options Adjustment Parameters :"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   2880
      Width           =   2415
   End
   Begin VB.Label lblEffecDate 
      BackStyle       =   0  'Transparent
      Caption         =   "Effective date :"
      Height          =   255
      Left            =   7080
      TabIndex        =   2
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label lblCAType 
      BackStyle       =   0  'Transparent
      Caption         =   "CA Type :"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   1215
   End
   Begin VB.Menu mnuDelivPerContr 
      Caption         =   "Delivery Per Contract"
      Visible         =   0   'False
      Begin VB.Menu mnuAddUnd 
         Caption         =   "Add Underlying"
      End
      Begin VB.Menu mnuDeleteUnd 
         Caption         =   "Delete Underlying"
      End
   End
End
Attribute VB_Name = "frmCorpAction"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const COLOR21 = &HDDDDDD
Const COLOR22 = &HCCCCCC

Const ROOTNODE = "r0"

Private Enum eCATypes
    CA_SPLIT
    CA_STOCK_DIV
    CA_MERGE_SPINOFF
End Enum

Private Enum eCASharesRules
    SR_CASH
    SR_ROUNDUP
    SR_ROUNDDOWN
End Enum

Private Enum DeliveryPerContractGrid
    DPC_UNDSYMBOL = 0
    DPC_UNDWEIGHT
    DPC_UNDALLOCATION
    DPC_COLUMNS_COUNT
    DPC_LAST_COLUMN = DPC_COLUMNS_COUNT - 1
End Enum

Private Enum OptionsGrid
    OPT_OLDROOTSYMBOL = 0
    OPT_ROOTSYMBOL
    OPT_OLDSTRIKE
    OPT_STRIKE
    OPT_OLDSTRIKETICKER
    OPT_STRIKETICKER
    OPT_COLUMNS_COUNT
    OPT_LAST_COLUMN = OPT_COLUMNS_COUNT - 1
End Enum

Private m_bEdit As Boolean
Private m_bLoad As Boolean
Private m_bBusy As Boolean
Private m_bCancel As Boolean
Private m_bComplete As Boolean
Private m_sSymbol As String
Private m_iUndID As Long
Private m_enCAType As eCATypes
Private m_sContracts As String
Private m_sParentLogKey As String
Private m_tStart As Date
Private m_tStop As Date

Private m_iCAId As Long
Private m_iNewUndId As Long
Private m_dRate As Double
Private m_iMultiplier As Long
Private m_dDelivery As Double
Private m_dStrikeCoeff As Double
Private m_dCashValue As Double
Private m_clsSyntheticPar As New clsSyntParColl
Private m_enSharedRules As eCASharesRules

Private WithEvents m_CN As ADODB.Connection
Attribute m_CN.VB_VarHelpID = -1
Private Const nTimeOut = 180 'sec
Private nLastErrorNumber As Long
Private sLastErrorDescription As String
Private sLastErrorSource As String
Private nLastStatus As Long

Public Function Execute(ByVal UndID As Long, ByVal Symbol As String, ByRef sContracts As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_sSymbol = Symbol
    m_iUndID = UndID
    m_iNewUndId = UndID
    m_iCAId = 0
    m_sContracts = sContracts
    Load Me
    Me.Caption = "Corporate action for '" & m_sSymbol & "' underlying"
    m_bEdit = False
    m_bLoad = False
    m_bCancel = False
    m_bBusy = False
    m_bComplete = False
    If LoadData Then
        Screen.MousePointer = vbDefault
        m_bLoad = True
        Show vbModal
    End If
    Unload Me
    Screen.MousePointer = vbDefault
    Execute = m_bEdit
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnClose_Click()
    On Error Resume Next
    If Not m_bBusy Then
        Unload Me
    End If
End Sub

Private Sub UpdateStrikes()
    Dim nRow As Long, aRowData As clsOptionRootWithStike
    
    On Error Resume Next
    
    If Not m_bLoad Then Exit Sub

    If fgOptions.Rows < 3 Then Exit Sub
    
    fgOptions.Cell(flexcpBackColor, 2, OPT_STRIKE, fgOptions.Rows - 1, OPT_STRIKE) = GCOLOR_TABLE_WHITE_BACK
    fgOptions.Cell(flexcpBackColor, 2, OPT_STRIKETICKER, fgOptions.Rows - 1, OPT_STRIKETICKER) = GCOLOR_TABLE_WHITE_BACK
    
    If m_enCAType = CA_SPLIT Then
        
        m_dRate = gCmn.ReadDbl(txtRate.Text)
        m_iMultiplier = gCmn.ReadLng(txtMult.Text)
        m_dDelivery = gCmn.ReadDbl(txtDelivPerCont.Text)
        
        If Not ValidateParams(False) Then Exit Sub
                        
        If m_dDelivery > 0# Then _
            txtNumCont.Text = gCmn.FmtDbl(m_dRate * 100 / m_dDelivery)
        If m_iMultiplier > 0 And m_dRate > 0# Then _
            m_dStrikeCoeff = m_dDelivery / (m_dRate * m_iMultiplier)
        
        With fgOptions
            For nRow = 2 To .Rows - 1
                Set aRowData = .RowData(nRow)
                If Not aRowData Is Nothing Then
                    .TextMatrix(nRow, OPT_STRIKE) = Round(aRowData.Strike * m_dStrikeCoeff, 4)
                End If
            Next
        End With
        
    ElseIf m_enCAType = CA_STOCK_DIV Then
        
        m_dRate = (gCmn.ReadDbl(txtRate.Text) + 100) / 100
        
        m_iMultiplier = gCmn.ReadLng(txtMult.Text)
        m_dDelivery = gCmn.ReadDbl(txtDelivPerCont.Text)
        
        If Not ValidateParams(False) Then Exit Sub
        
        If m_dDelivery > 0# Then _
            txtNumCont.Text = gCmn.FmtDbl(m_dRate * 100 / m_dDelivery)
        
        If m_iMultiplier > 0 And m_dRate > 0# Then _
            m_dStrikeCoeff = m_dDelivery / (m_dRate * m_iMultiplier)
        
        With fgOptions
            For nRow = 2 To .Rows - 1
                .TextMatrix(nRow, OPT_STRIKE) = gCmn.ReadDbl(.TextMatrix(nRow, OPT_OLDSTRIKE)) * m_dStrikeCoeff
            Next
        End With
    ElseIf m_enCAType = CA_MERGE_SPINOFF Then
        With fgOptions
            For nRow = 2 To .Rows - 1
                Set aRowData = .RowData(nRow)
                If Not aRowData Is Nothing Then
                    .TextMatrix(nRow, OPT_STRIKE) = Round(aRowData.Strike, 4) 'Round(aRowData.Strike * m_dStrikeCoeff, 4)
                    .TextMatrix(nRow, OPT_STRIKETICKER) = aRowData.Ticker
                End If
            Next
            
            If fgOptions.Rows > 2 Then
                fgOptions.Cell(flexcpBackColor, 2, OPT_STRIKE, fgOptions.Rows - 1, OPT_STRIKE) = COLOR21
                fgOptions.Cell(flexcpBackColor, 2, OPT_STRIKETICKER, fgOptions.Rows - 1, OPT_STRIKETICKER) = COLOR21
            End If
        End With
    End If
End Sub

Private Sub btnExecute_Click()
    On Error GoTo Herr
    
    If m_bComplete And Not m_bCancel Then
        RollbackCA
    ElseIf Not m_bBusy And Not m_bCancel Then
        ExecuteCA
    ElseIf m_bBusy And Not m_bCancel Then
        m_bCancel = True
        btnExecute.Caption = IIf(m_bComplete, "&Rollback", "&Restart")
    ElseIf Not m_bBusy And m_bCancel Then
        m_bCancel = False
        btnExecute.Caption = "&Execute"
        EnableControls = True
    End If
    
    Exit Sub
Herr:
    ShowError
End Sub

Private Sub cboCAType_Change()
    On Error Resume Next
    
    m_enCAType = cboCAType.ListIndex
    
    If cboCAType.ListIndex = CA_MERGE_SPINOFF Then
        fraParams.Visible = False
        fraParamsM.Visible = True
    Else
        fraParams.Visible = True
        fraParamsM.Visible = False
        
        If cboCAType.ListIndex = CA_SPLIT Then
            txtRate.Text = gCmn.FmtDbl(1)
            fraParams.Caption = "Split parameters"
            lblRate.Caption = "Split Rate :"
        ElseIf cboCAType.ListIndex = CA_STOCK_DIV Then
            txtRate.Text = gCmn.FmtDbl(0)
            fraParams.Caption = "Stock dividends parameters"
            lblRate.Caption = "Dividends (%) :"
        End If
        
    End If
    
    UpdateStrikes
    
    Exit Sub
End Sub


Private Sub cboSymbol_Change()
    On Error Resume Next
    With cboSymbol
        If .MatchFound Then
            .ForeColor = &H0
            m_iNewUndId = .Value
        Else
            .ForeColor = &H808080
        End If
    End With
End Sub


Private Sub fgDelivPerCont_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    With fgDelivPerCont
        If IsUSD(.Row) Then
            btnParamDel.Enabled = False
            mnuDeleteUnd.Enabled = False
        Else
            mnuDeleteUnd.Enabled = True
            btnParamDel.Enabled = True
        End If
    End With

End Sub

Private Sub fgDelivPerCont_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)

    On Error GoTo Herr

    If (Col = DPC_UNDSYMBOL Or Col = DPC_UNDALLOCATION) And IsUSD(Row) Then
        Cancel = True
    Else
        Cancel = False
    End If
    
    Exit Sub
Herr:
    Cancel = True

End Sub

Private Sub fgDelivPerCont_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    With fgDelivPerCont
        If Button And vbRightButton Then
            If .MouseRow >= 2 Then
                .Row = .MouseRow
                .Col = .MouseCol
            End If
        End If
    End With
End Sub

Private Sub fgOptions_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)

    On Error GoTo Herr

    If Col = OPT_OLDROOTSYMBOL Or Col = OPT_OLDSTRIKE Or Col = OPT_OLDSTRIKETICKER Or _
     ((Col = OPT_STRIKETICKER Or Col = OPT_STRIKE) And m_enCAType = CA_MERGE_SPINOFF) Then
        Cancel = True
    Else
        Cancel = False
    End If
    
    Exit Sub
Herr:
    Cancel = True

End Sub

Private Sub fgOptions_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    With fgOptions
        If Button And vbRightButton Then
            If .MouseRow >= 2 Then
                .Row = .MouseRow
                .Col = .MouseCol
            End If
        End If
    End With
End Sub

Private Sub fgOptions_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim DblVal As Double, l As Long, aRoot As clsCARootAtom
Dim aRowData As clsOptionRootWithStike, aRowData2 As clsOptionRootWithStike, sSymbol$
Dim collRoots As New clsCARootColl, nRow&
    
    On Error Resume Next
    
    With fgOptions
        If Col = OPT_STRIKE And Row > 1 Then
            On Error GoTo BadDbl
                DblVal = CDbl(.EditText)
                If DblVal <= 0# Then
                    Cancel = True
                    Exit Sub
                End If
        ElseIf Col = OPT_ROOTSYMBOL And Row > 1 Then
                                    
            Set aRowData = .RowData(Row)
            If Not aRowData Is Nothing Then
            
                For nRow = 2 To .Rows - 1
                    Set aRowData2 = .RowData(nRow)
            
                    If Not aRowData Is Nothing Then
                        
                        If aRowData.Symbol <> aRowData2.Symbol Then
                                                                                        
                            sSymbol = .TextMatrix(nRow, OPT_ROOTSYMBOL)
                                                                                        
                            Set aRoot = collRoots(sSymbol)
                            
                            If aRoot Is Nothing Then
                                Set aRoot = collRoots.Add(sSymbol)
                                aRoot.NewSymbol = sSymbol
                                aRoot.OldSymbol = aRowData.Symbol
                            End If
                        End If
                    End If
                Next
                
                Set aRoot = collRoots(.EditText)
                
                If Not aRoot Is Nothing Then
                    Cancel = True
                    Exit Sub
                End If
            End If
                                
        End If
    End With
    
    On Error Resume Next
    Cancel = False
    Exit Sub
BadDbl:
    Cancel = True
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrids
    
    With tvwLog
        .Nodes.Clear
        .Nodes.Add , , ROOTNODE, ""
    End With
End Sub

Private Sub InitGrids()
    Dim i As Long
    On Error Resume Next
    
    With fgDelivPerCont
        .Rows = 0
        .Cols = DPC_COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = DPC_COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExSortShow
        .FixedCols = 0
        .FixedRows = 2
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(DPC_UNDSYMBOL) = True
        .MergeCol(DPC_UNDWEIGHT) = True
        .MergeCol(DPC_UNDALLOCATION) = True
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        i = DPC_UNDSYMBOL
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "symbol"
        
        .ColDataType(i) = flexDTString
                
        i = DPC_UNDWEIGHT
        .TextMatrix(0, i) = "Cash Amount /" & vbCrLf & "Underlying weight"
        .TextMatrix(1, i) = "Cash Amount /" & vbCrLf & "Underlying weight"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.000000"
        
        i = DPC_UNDALLOCATION
        .TextMatrix(0, i) = "Settlement" & vbCrLf & "allocation (%)"
        .TextMatrix(1, i) = "Settlement" & vbCrLf & "allocation (%)"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
        .Cell(flexcpAlignment, 0, 0, 1, DPC_LAST_COLUMN) = flexAlignCenterCenter
        .Cell(flexcpFontBold, 0, 0, 1, DPC_LAST_COLUMN) = True
                                   
    End With
    
    With fgOptions
        .Rows = 0
        .Cols = OPT_COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = OPT_COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDKbdMouse
        .FixedCols = 0
        .FixedRows = 2
        
        .Cell(flexcpFontBold, 0, 0, 1, OPT_LAST_COLUMN) = True
        .Cell(flexcpAlignment, 0, 0, 1, OPT_LAST_COLUMN) = flexAlignCenterCenter
        
        .MergeCells = flexMergeRestrictAll
                
        .MergeCol(OPT_OLDROOTSYMBOL) = True
        .MergeCol(OPT_ROOTSYMBOL) = True
        
        .MergeRow(0) = True
        .MergeRow(1) = True

        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        i = OPT_OLDROOTSYMBOL
        .TextMatrix(0, i) = "Old Root" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Old Root" & vbCrLf & "symbol"
        .ColDataType(i) = flexDTString
                
        i = OPT_ROOTSYMBOL
        .TextMatrix(0, i) = "Root" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Root" & vbCrLf & "symbol"
        .ColDataType(i) = flexDTString
        
        i = OPT_OLDSTRIKE
        .TextMatrix(0, i) = "Strike"
        .TextMatrix(1, i) = "Old"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
                
        i = OPT_STRIKE
        .TextMatrix(0, i) = "Strike"
        .TextMatrix(1, i) = "New"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
                                   
        i = OPT_OLDSTRIKETICKER
        .TextMatrix(0, i) = "Strike ticker"
        .TextMatrix(1, i) = "Old"
        .ColDataType(i) = flexDTString
                
        i = OPT_STRIKETICKER
        .TextMatrix(0, i) = "Strike ticker"
        .TextMatrix(1, i) = "New"
        .ColDataType(i) = flexDTString
        
        
    End With
    
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset
    Dim arr(), Items, Lines, l As Variant, i As Long, aRowData As clsOptionRootWithStike
    
    cboCAType.AddItem "Split/Reverse split", CA_SPLIT
    cboCAType.AddItem "Stock Dividends", CA_STOCK_DIV
    cboCAType.AddItem "Merging/Spin-off", CA_MERGE_SPINOFF
    
    txtRate.Text = gCmn.FmtDbl(1)
    txtMult.Text = gCmn.FmtLng(100)
    txtMultM.Text = gCmn.FmtLng(100)
    txtDelivPerCont.Text = gCmn.FmtDbl(100)
    txtNumCont.Text = gCmn.FmtDbl(1)
    
    cboCAType.ListIndex = CA_SPLIT
    
    m_dCashValue = 0
    
    dtpEffecDate.Value = Date
    dtpEffecDate.Tag = gCmn.FmtDate(Date)
    
    Lines = Split(m_sContracts, "|")
    If UBound(Lines) = 0 Then
        ReDim arr(1, 0)
        arr(0, 0) = "<NONE>"
        arr(1, 0) = GINT_NONEID
    Else
        ReDim arr(1, UBound(Lines) - 1)
        i = 0
        For Each l In Lines
            Items = Split(l, ";")
            
            If UBound(Items) = 1 Then
                arr(0, i) = gCmn.ReadStr(Items(1))
                arr(1, i) = gCmn.ReadLng(Replace(Items(0), "#", ""))
                i = i + 1
            End If
        Next
        ReDim Preserve arr(1, i - 1)
    End If
    
    With cboSymbol
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
        If .ListCount > 0 Then .Text = m_sSymbol
    End With
    
    m_enSharedRules = SR_CASH
    optRuleCash.Value = True
    
    fgDelivPerCont.ColComboList(DPC_UNDSYMBOL) = m_sContracts
    
    Set rs = gDBW.usp_OptionRootWithStrikes_Get(Null, m_iUndID)
    
    With rs
        If .RecordCount > 0 Then
            
            .MoveFirst
            
            Do While Not .EOF
                fgOptions.AddItem ""
                
                
                Set aRowData = New clsOptionRootWithStike
                If Not aRowData Is Nothing Then
                    aRowData.OptionRootID = gCmn.ReadLng(.Fields("iOptionRootID"))
                    aRowData.Symbol = gCmn.ReadStr(.Fields("vcSymbol"))
                    aRowData.Strike = gCmn.ReadDbl(.Fields("fStrike"))
                    aRowData.Ticker = gCmn.ReadStr(.Fields("vcStrikeTicker"))
                                            
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_OLDROOTSYMBOL) = aRowData.Symbol
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_ROOTSYMBOL) = aRowData.Symbol
                    
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_OLDSTRIKE) = aRowData.Strike
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_STRIKE) = aRowData.Strike
                    
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_OLDSTRIKETICKER) = aRowData.Ticker
                    fgOptions.TextMatrix(fgOptions.Rows - 1, OPT_STRIKETICKER) = aRowData.Ticker
                    
                    fgOptions.RowData(fgOptions.Rows - 1) = aRowData
                End If
                
                .MoveNext
            Loop
            
        End If
    End With
        
    With fgDelivPerCont
        .AddItem ""
                                
        .RowData(.Rows - 1) = USD_KEY
        .TextMatrix(.Rows - 1, DPC_UNDSYMBOL) = USD_SYMBOL
        .TextMatrix(.Rows - 1, DPC_UNDWEIGHT) = 0
        .Cell(flexcpAlignment, .Rows - 1, DPC_UNDWEIGHT) = flexAlignRightCenter
        
        .AddItem ""
            
        .RowData(.Rows - 1) = Trim$(Str$(m_iUndID))
                    
        .TextMatrix(.Rows - 1, DPC_UNDWEIGHT) = 1
        .TextMatrix(.Rows - 1, DPC_UNDALLOCATION) = 100
        .Cell(flexcpText, .Rows - 1, DPC_UNDSYMBOL) = m_iUndID
        .Cell(flexcpAlignment, .Rows - 1, DPC_UNDWEIGHT) = flexAlignRightCenter
    End With
    
    If fgOptions.Rows > 2 Then
        fgOptions.Cell(flexcpBackColor, 2, OPT_OLDROOTSYMBOL, fgOptions.Rows - 1, OPT_OLDROOTSYMBOL) = COLOR21
        fgOptions.Cell(flexcpBackColor, 2, OPT_OLDSTRIKE, fgOptions.Rows - 1, OPT_OLDSTRIKE) = COLOR21
        fgOptions.Cell(flexcpBackColor, 2, OPT_OLDSTRIKETICKER, fgOptions.Rows - 1, OPT_OLDSTRIKETICKER) = COLOR21
    End If
    
    LoadData = True
    
End Function


Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If UnloadMode = vbFormControlMenu Then
        Cancel = m_bBusy
    End If
End Sub

Private Sub Form_Resize()
    Dim iWidth, iCol As Long
    On Error Resume Next
    
    With fgDelivPerCont
        .ColWidth(DPC_UNDSYMBOL) = 0.27 * .Width - 100
        .ColWidth(DPC_UNDWEIGHT) = 0.4 * .Width - 100
        .ColWidth(DPC_UNDALLOCATION) = 0.32 * .Width - 100
    End With
    
    With fgOptions
    
        .Width = btnClose.Left + btnClose.Width - .Left
        
        .Height = btnClose.Top - .Top - 100
        
        iWidth = .Width
        If iWidth < 4000 Then
            iWidth = 4000
        End If
                
        For iCol = 0 To .Cols - 1
            .ColWidth(iCol) = iWidth / .Cols - 50
        Next iCol
    End With
    
End Sub


Private Sub optRoundDown_Click()
    On Error Resume Next
    m_enSharedRules = SR_ROUNDDOWN
End Sub

Private Sub optRoundUp_Click()
    On Error Resume Next
    m_enSharedRules = SR_ROUNDUP
End Sub

Private Sub optRuleCash_Click()
    On Error Resume Next
    m_enSharedRules = SR_CASH
End Sub

Private Sub txtDelivPerCont_Validate(Cancel As Boolean)
    On Error Resume Next
    If Not IsNumeric(txtDelivPerCont.Text) Then
        txtDelivPerCont.SetFocus
        ShowWarning "Invalid Delivery Per Contract value."
        Cancel = True
    ElseIf gCmn.ReadDbl(txtDelivPerCont.Text) <= 0 Then
        txtDelivPerCont.SetFocus
        ShowWarning "Invalid Delivery Per Contract value."
        Cancel = True
    Else
        txtDelivPerCont.Text = gCmn.FmtDbl(gCmn.ReadDbl(txtDelivPerCont.Text))
        UpdateStrikes
    End If
End Sub

Private Sub txtMult_Validate(Cancel As Boolean)
    On Error Resume Next
    If Not IsNumeric(txtMult.Text) Then
        txtMult.SetFocus
        ShowWarning "Invalid Multiplier value."
        Cancel = True
    ElseIf gCmn.ReadLng(txtMult.Text) <= 0 Then
        txtMult.SetFocus
        ShowWarning "Invalid Multiplier value."
        Cancel = True
    Else
        txtMult.Text = gCmn.FmtLng(gCmn.ReadLng(txtMult.Text))
        txtMultM.Text = txtMult.Text
        UpdateStrikes
    End If
End Sub

Private Sub txtMultM_Validate(Cancel As Boolean)
    On Error Resume Next
    If Not IsNumeric(txtMultM.Text) Then
        txtMultM.SetFocus
        ShowWarning "Invalid Multiplier value."
        Cancel = True
    ElseIf gCmn.ReadLng(txtMultM.Text) <= 0 Then
        txtMultM.SetFocus
        ShowWarning "Invalid Multiplier value."
        Cancel = True
    Else
        txtMultM.Text = gCmn.FmtLng(gCmn.ReadLng(txtMultM.Text))
        txtMult.Text = txtMultM.Text
        UpdateStrikes
    End If
End Sub

Private Sub txtRate_Validate(Cancel As Boolean)
    On Error Resume Next
    If Not IsNumeric(txtRate.Text) Then
        txtRate.SetFocus
        
        If m_enCAType = CA_SPLIT Then
            ShowWarning "Invalid Rate value."
        ElseIf m_enCAType = CA_STOCK_DIV Then
            ShowWarning "Invalid Dividends value."
        End If
        
        Cancel = True
    ElseIf gCmn.ReadDbl(txtRate.Text) < 0 Then
        txtRate.SetFocus
        
        If m_enCAType = CA_SPLIT Then
            ShowWarning "Invalid Rate value."
        ElseIf m_enCAType = CA_STOCK_DIV Then
            ShowWarning "Invalid Dividends value."
        End If
        
        Cancel = True
    Else
        If m_enCAType = CA_SPLIT And gCmn.ReadDbl(txtRate.Text) = 0# Then
            ShowWarning "Invalid Rate value."
            Cancel = True
        Else
            txtRate.Text = gCmn.FmtDbl(gCmn.ReadDbl(txtRate.Text))
            UpdateStrikes
        End If
    End If
End Sub

Private Sub txtDelivPerCont_Change()
    On Error Resume Next
    
    If Not IsNumeric(txtDelivPerCont.Text) Then
        txtDelivPerCont.ForeColor = vbRed
    ElseIf gCmn.ReadDbl(txtDelivPerCont.Text) <= 0 Then
        txtDelivPerCont.ForeColor = vbRed
    Else
        m_dDelivery = gCmn.ReadDbl(txtDelivPerCont.Text)
        txtDelivPerCont.ForeColor = 0
    End If
    
End Sub

Private Sub txtMult_Change()
    On Error Resume Next
    If Not IsNumeric(txtMult.Text) Then
        txtMult.ForeColor = vbRed
    ElseIf gCmn.ReadLng(txtMult.Text) <= 0 Then
        txtMult.ForeColor = vbRed
    Else
        m_iMultiplier = gCmn.ReadLng(txtMult.Text)
        txtMult.ForeColor = 0
    End If
End Sub

Private Sub txtMultM_Change()
    On Error Resume Next
    
    If Not IsNumeric(txtMultM.Text) Then
        txtMultM.ForeColor = vbRed
    ElseIf gCmn.ReadLng(txtMultM.Text) <= 0 Then
        txtMultM.ForeColor = vbRed
    Else
        m_iMultiplier = gCmn.ReadLng(txtMultM.Text)
        txtMultM.ForeColor = 0
    End If
    
End Sub

Private Sub txtRate_Change()
    On Error Resume Next
    
    If Not IsNumeric(txtRate.Text) Then
        txtRate.ForeColor = vbRed
    ElseIf gCmn.ReadDbl(txtRate.Text) < 0 Then
        txtRate.ForeColor = vbRed
    Else
        If m_enCAType = CA_SPLIT And gCmn.ReadDbl(txtRate.Text) = 0# Then
            txtRate.ForeColor = vbRed
        Else
            m_dRate = gCmn.ReadDbl(txtRate.Text)
            txtRate.ForeColor = 0
        End If
    End If
    
End Sub


Private Sub fgDelivPerCont_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With fgDelivPerCont
        If (Button And vbRightButton) And .Row > -1 And .Col > -1 Then
            If IsUSD(.Row) Then
                btnParamDel.Enabled = False
                mnuDeleteUnd.Enabled = False
            Else
                mnuDeleteUnd.Enabled = True
                btnParamDel.Enabled = True
            End If
            PopupMenu mnuDelivPerContr, , , , mnuAddUnd
        End If
    End With

End Sub

Private Sub mnuAddUnd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub mnuDeleteUnd_Click()
On Error GoTo ErrHnd
                 
DeleteRow fgDelivPerCont.RowSel

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub AddRow()
    
    With fgDelivPerCont
        
        .AddItem ""
        .TextMatrix(.Rows - 1, DPC_UNDWEIGHT) = 1
        .Cell(flexcpText, .Rows - 1, DPC_UNDSYMBOL) = m_iUndID
        .Cell(flexcpAlignment, .Rows - 1, DPC_UNDWEIGHT) = flexAlignRightCenter
        .TextMatrix(.Rows - 1, DPC_UNDALLOCATION) = 100
        .Cell(flexcpAlignment, .Rows - 1, DPC_UNDALLOCATION) = flexAlignRightCenter
        
        .RowData(.Rows - 1) = Trim$(Str$(m_iUndID))
        
        .Select .Rows - 1, DPC_UNDSYMBOL
        .EditCell
    End With
    
End Sub

Private Sub DeleteRow(ByVal Row As Long)
    
    With fgDelivPerCont
        
        If Row < 2 Or IsUSD(Row) Then Exit Sub
    
        If Row = .Rows - 1 Then
            .RemoveItem Row
            .Row = Row - 1
            .RowSel = Row - 1
        Else
            .RemoveItem Row
        End If
        
        If IsUSD(.RowSel) Then
            mnuDeleteUnd.Enabled = False
            btnParamDel.Enabled = False
        End If
            
    End With

End Sub

Private Sub btnParamAdd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    gCmn.ErrorMsgBox Err.Description
End Sub

Private Sub btnParamDel_Click()
On Error GoTo ErrHnd
             
DeleteRow fgDelivPerCont.RowSel

Exit Sub
ErrHnd:
    gCmn.ErrorMsgBox Err.Description
End Sub

Private Sub fgDelivPerCont_AfterEdit(ByVal Row As Long, ByVal Col As Long)
Dim sOldID, sNewID As String, l As Long

On Error GoTo ErrHnd
    If Col = DPC_UNDSYMBOL And Row > 2 Then
            
        sOldID = fgDelivPerCont.RowData(Row)
        sNewID = Format$(fgDelivPerCont.Cell(flexcpText, Row, DPC_UNDSYMBOL))
                
        If sOldID <> sNewID Then
            fgDelivPerCont.RowData(Row) = sNewID
        End If
    ElseIf Col = DPC_UNDWEIGHT And fgDelivPerCont.RowData(Row) = USD_KEY Then
        m_dCashValue = gCmn.ReadDbl(fgDelivPerCont.Cell(flexcpText, Row, DPC_UNDWEIGHT))
    End If
    
    Exit Sub
ErrHnd:
    gCmn.ErrorMsgBox Err.Description
End Sub

Private Sub fgDelivPerCont_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    With fgDelivPerCont
        If NewRow > -1 And NewCol > -1 Then
            If IsUSD(NewRow) Then
                mnuDeleteUnd.Enabled = False
                btnParamDel.Enabled = False
            Else
                mnuDeleteUnd.Enabled = True
                btnParamDel.Enabled = True
            End If
        End If
    End With
End Sub

Private Sub fgDelivPerCont_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgDelivPerCont
        Select Case KeyCode
        Case vbKeyInsert
            AddRow
            Exit Sub
        Case vbKeyDelete
            DeleteRow fgDelivPerCont.RowSel
        End Select
    End With
End Sub

Private Sub fgDelivPerCont_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim DblVal As Double, l As Long

    On Error Resume Next
    
    If Col = DPC_UNDWEIGHT And Row > 1 Then
        On Error GoTo BadDbl
            DblVal = CDbl(fgDelivPerCont.EditText)
            If DblVal < 0 Then
                Cancel = True
            End If
    ElseIf Col = DPC_UNDALLOCATION And Row > 1 Then
        On Error GoTo BadDbl
            DblVal = CDbl(fgDelivPerCont.EditText)
            If DblVal < 0 Then
                Cancel = True
            End If
    End If
    
    On Error Resume Next
    Cancel = False
    Exit Sub
BadDbl:
    Cancel = True
End Sub

Private Function IsUSD(ByVal Row As Long) As Boolean
    On Error Resume Next
    IsUSD = False
    If fgDelivPerCont.RowData(Row) = USD_KEY Then IsUSD = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCopyLog_Click()
    Dim Buffer As String
    Dim NodX As Node
    Dim s As String
    Dim i As Long
    
    On Error Resume Next
    
    Buffer = ""
    With tvwLog
        For Each NodX In .Nodes
            s = NodX.FullPath
            i = UBound(Split(s, "\"))
            If i > 0 Then
                Buffer = Buffer & String(i - 1, vbTab)
            End If
            s = NodX.Text
            If Len(s) > 0 Then
                Buffer = Buffer & s & vbCrLf
            End If
        Next
    End With
    With Clipboard
        .Clear
        .SetText Buffer
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LogMsg(ByVal Msg As String) As String
    Static ID As Long
    Dim Key As String
    On Error Resume Next
    With tvwLog
        ID = ID + 1
        Key = "s" & Format$(ID)
        LogMsg = Key
        If Len(m_sParentLogKey) = 0 Then
            With .Nodes.Add(ROOTNODE, tvwChild, Key, Msg)
                .EnsureVisible
            End With
        Else
            With .Nodes.Add(m_sParentLogKey, tvwChild, Key, Msg)
                .EnsureVisible
            End With
        End If
        DoEvents
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogStarted()
    Dim Msg As String
    On Error Resume Next
    EnableControls = False
    m_sParentLogKey = ""
    m_tStart = Now
    
    Msg = "Corporate action processing for '" & m_sSymbol & "' underlying started "
    
    btnExecute.Caption = "&Cancel"
    m_sParentLogKey = LogMsg(Msg & m_tStart)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogFinished(Optional ByVal Result As String = "")
    Dim Msg As String, Duration As Double
    On Error Resume Next
    m_tStop = Now
    Duration = DateDiff("s", m_tStart, m_tStop)
    
    Msg = "Corporate action processing for '" & m_sSymbol & "' finished " & Result
    LogMsg Msg & m_tStop
    LogMsg "Duration (sec): " & FormatDuration(Duration)
    m_sParentLogKey = ""
    btnClose.Enabled = True
End Function

Private Sub ExecuteCA()
    Dim bIsSynthetic As Boolean
    Dim LastErrorDescription As String
    
    On Error GoTo Herr
    
    If Not ValidateParams(True) Then Exit Sub
    
    m_bBusy = True
    
    LogStarted
    
    If m_enCAType <> CA_MERGE_SPINOFF Then
        m_iNewUndId = m_iUndID
    End If

    m_iCAId = gDBW.usp_CorporateAction_Save(m_enCAType, Now, m_iUndID, 1, Null)
        
    bIsSynthetic = PrepareSyntheticParams
            
    ' Execute OptionRoot Adjustment
    ExecuteOptionRootAdjustment (bIsSynthetic)
    
    ' Execute Option Adjustment
    ExecuteOptionAdjustment

    ' Execute Underlying Adjustment
    ExecuteUnderlyingAdjustment
    
    ' Execute option position Adjustment
    ExecuteOptionPositionsAdjustment
    
    ' Execute underlying position Adjustment
    ExecuteUnderlyingPositionsAdjustment
    
    gDBW.usp_CorporateAction_Save Null, Null, Null, 2, m_iCAId

    m_sParentLogKey = ""
    LogFinished "successfully at "
    m_bBusy = False
    m_bCancel = False
    m_bComplete = True
    btnExecute.Caption = "&Rollback"
    m_bEdit = True
    Exit Sub
Herr:
    m_sParentLogKey = ""
    LastErrorDescription = Err.Description
    ShowError
    LogMsg LastErrorDescription
    LogFinished "with error at "
    On Error Resume Next
    gDBW.usp_LogCA_Rollback m_iCAId
    m_bBusy = False
    m_bCancel = True
    btnExecute.Caption = "&Restart"
End Sub

Private Function ValidateParams(ByVal bShowWarning As Boolean) As Boolean
    Dim dTotalPercenage#, l&, nRow&
    On Error Resume Next
    
    ValidateParams = True
    If Not m_bLoad Then Exit Function
    
    If m_enCAType = CA_SPLIT Then
        If (m_dRate <= 0) Or (m_dRate = 1) Then
            txtRate.SetFocus
            If bShowWarning Then ShowWarning "Split Rate must be positive value and not equal 1"
            ValidateParams = False
            Exit Function
        ElseIf m_iMultiplier <= 0 Then
            txtMult.SetFocus
            If bShowWarning Then ShowWarning "Invalid Multiplier Value"
            ValidateParams = False
            Exit Function
        ElseIf m_dDelivery <= 0 Then
            txtDelivPerCont.SetFocus
            If bShowWarning Then ShowWarning "Invalid Delivery Per Contract Value"
            ValidateParams = False
            Exit Function
        End If
    ElseIf m_enCAType = CA_STOCK_DIV Then
        If (m_dRate <= 0) Then
            txtRate.SetFocus
            If bShowWarning Then ShowWarning "Invalid Dividends must be positive value"
            ValidateParams = False
            Exit Function
        ElseIf m_iMultiplier <= 0 Then
            txtMult.SetFocus
            If bShowWarning Then ShowWarning "Invalid Multiplier Value"
            ValidateParams = False
            Exit Function
        ElseIf m_dDelivery <= 0 Then
            txtDelivPerCont.SetFocus
            If bShowWarning Then ShowWarning "Invalid Delivery Per Contract Value"
            ValidateParams = False
            Exit Function
        End If
    ElseIf m_enCAType = CA_MERGE_SPINOFF Then
    
        If fgDelivPerCont.Rows < 4 Then
            fgDelivPerCont.SetFocus
            If bShowWarning Then ShowWarning "Delivery Per Contract must contain minimum 1 underlying"
            ValidateParams = False
            Exit Function
        End If
        
        dTotalPercenage = 0
        With fgDelivPerCont
            For l = 2 To .Rows - 1
                If .RowData(l) <> USD_KEY Then
                    dTotalPercenage = dTotalPercenage + gCmn.ReadDbl(.Cell(flexcpText, l, DPC_UNDALLOCATION))
                End If
            Next
        End With
        If dTotalPercenage <> 100# Then
            fgDelivPerCont.SetFocus
            fgDelivPerCont.Select fgDelivPerCont.Rows - 1, DPC_UNDALLOCATION
            fgDelivPerCont.EditCell
            If bShowWarning Then ShowWarning "Sum of Settlement Allocations must be 100 %"
            ValidateParams = False
            Exit Function
        End If
    End If
    
    If bShowWarning Then
        Dim aRoot As clsCARootAtom, aStrike As clsCAStrikeAtom, aTicker As clsCATickerAtom
        Dim dStrike#, sTicker$, sSymbol$, aRowData As clsOptionRootWithStike
        Dim collRoots As New clsCARootColl
        
        With fgOptions
            For nRow = 2 To .Rows - 1
                sSymbol = .TextMatrix(nRow, OPT_ROOTSYMBOL)
                dStrike = .TextMatrix(nRow, OPT_STRIKE)
                sTicker = .TextMatrix(nRow, OPT_STRIKETICKER)
                Set aRowData = .RowData(nRow)
                
                If Not aRowData Is Nothing Then
                                    
                    Set aRoot = collRoots(sSymbol)
                    If Not aRoot Is Nothing Then
                        If aRoot.OldSymbol <> aRowData.Symbol Then
                            .SetFocus
                            .Select nRow, OPT_ROOTSYMBOL
                            .ShowCell nRow, OPT_ROOTSYMBOL
                            ShowWarning "Corporate Action can't merge different option root in one"
                            ValidateParams = False
                            Exit Function
                        End If
                    Else
                        Set aRoot = collRoots.Add(sSymbol)
                        aRoot.NewSymbol = sSymbol
                        aRoot.OldSymbol = aRowData.Symbol
                    End If
                    
                    Set aStrike = aRoot.Strikes(CStr(Round(dStrike, 4)))
                    If Not aStrike Is Nothing Then
                        .SetFocus
                        .Select nRow, OPT_STRIKE
                        .ShowCell nRow, OPT_STRIKE
                        ShowWarning "Different option in the same root can't have equal strikes." & vbCrLf & "Enter different strike or delete option."
                        ValidateParams = False
                        Exit Function
                    Else
                        Set aStrike = aRoot.Strikes.Add(CStr(Round(dStrike, 4)))
                        aStrike.Strike = Round(dStrike, 4)
                    End If
                    
                    Set aTicker = aRoot.Tickers(sTicker)
                    
                    If Not aTicker Is Nothing Then
                        .SetFocus
                        .Select nRow, OPT_STRIKETICKER
                        .ShowCell nRow, OPT_STRIKETICKER
                        ShowWarning "Different option in the same root can't have equal strike tickers." & vbCrLf & "Enter different strike ticker or delete option."
                        ValidateParams = False
                        Exit Function
                    Else
                        Set aTicker = aRoot.Tickers.Add(sTicker)
                        aTicker.StrikeTicker = sTicker
                    End If
                End If
            Next
        End With
        
    End If
    
End Function

Private Function PrepareSyntheticParams() As Boolean
    Dim Param As clsSyntParAtom, l  As Long, dTotalPercenage As Double
    Idle
    PrepareSyntheticParams = False
    m_clsSyntheticPar.Clear
    dTotalPercenage = 0
    
    If m_enCAType = CA_MERGE_SPINOFF Then
                
        With fgDelivPerCont
            For l = 2 To .Rows - 1
                
                Idle
                
                If .RowData(l) <> USD_KEY Then
                            
                    Set Param = m_clsSyntheticPar(CStr(.RowData(l)))
                    
                    If Param Is Nothing Then
                        Set Param = m_clsSyntheticPar.Add(CStr(.RowData(l)))
                        Param.UnderlyingID = .Cell(flexcpText, l, DPC_UNDSYMBOL)
                        Param.Weight = gCmn.ReadDbl(.Cell(flexcpText, l, DPC_UNDWEIGHT))
                        Param.StrikeSettlement = gCmn.ReadDbl(.Cell(flexcpText, l, DPC_UNDALLOCATION))
                        dTotalPercenage = dTotalPercenage + Param.StrikeSettlement
                    Else
                        Param.Weight = Param.Weight + gCmn.ReadDbl(.Cell(flexcpText, l, DPC_UNDWEIGHT))
                        Param.StrikeSettlement = Param.StrikeSettlement + .Cell(flexcpText, l, DPC_UNDALLOCATION)
                        dTotalPercenage = dTotalPercenage + gCmn.ReadDbl(.Cell(flexcpText, l, DPC_UNDALLOCATION))
                    End If
                End If
            Next
        End With
                
        PrepareSyntheticParams = True
    Else
        dTotalPercenage = 100#
        If m_iMultiplier = gCmn.ReadLng(m_dDelivery) Then
            PrepareSyntheticParams = False
        Else
            PrepareSyntheticParams = True
            
            Set Param = m_clsSyntheticPar.Add(Trim$(Str$(m_iNewUndId)))
            Param.UnderlyingID = m_iNewUndId
            If m_iMultiplier > 0 Then _
                Param.Weight = m_dDelivery / m_iMultiplier
            Param.StrikeSettlement = 100#
        End If
    End If
    
    If dTotalPercenage <> 100# Then Err.Raise vbObjectError, "Corporate Actions", "Sum of Settlement Allocations must be 100 %"
    
End Function

Private Sub ExecuteOptionRootAdjustment(ByVal bIsSynthetic As Boolean)
    Dim nRow, nRootID As Long, sNewRootSymbol As String
    Dim Param As clsSyntParAtom, aRowData As clsOptionRootWithStike
    Dim sLogKey As String, rsLog As ADODB.Recordset
    
    Idle
    sLogKey = m_sParentLogKey
    m_sParentLogKey = LogMsg("Start Option Roots adjustment")
    nRootID = -1
            
    With fgOptions
        For nRow = 2 To .Rows - 1
            Set aRowData = .RowData(nRow)
            
            If Not aRowData Is Nothing Then
            
                Idle
                If nRootID <> aRowData.OptionRootID Then
                    nRootID = aRowData.OptionRootID
                    
                    sNewRootSymbol = .Cell(flexcpText, nRow, OPT_ROOTSYMBOL)
                    
                    Set rsLog = gDBW.usp_OptionRootCA_Save(m_iCAId, nRootID, IIf(sNewRootSymbol = aRowData.Symbol, Null, sNewRootSymbol) _
                        , m_iNewUndId, m_iMultiplier, bIsSynthetic, m_dCashValue)
                        
                    With rsLog
                        If .RecordCount > 0 Then
                        .MoveFirst
                        Do While Not .EOF
                            Idle
                            LogMsg (.Fields("vcLogMessage"))
                            .MoveNext
                        Loop
                        End If
                    End With
                        
                        
                    If bIsSynthetic Then
                        For Each Param In m_clsSyntheticPar
                            gDBW.usp_SyntheticParamCA_Save m_iCAId, nRootID, Param.UnderlyingID, Param.Weight
                        Next
                    End If
                    
                End If
            End If
        Next
    End With
    LogMsg ("Complete Option Roots adjustment")
    m_sParentLogKey = sLogKey
End Sub

Private Sub ExecuteOptionAdjustment()
    Dim nRow, nRootID As Long, sNewTicker As String, dStrike, dNewStrike As Double
    Dim sLogKey As String, rsLog As ADODB.Recordset, aRowData As clsOptionRootWithStike
    Idle
    sLogKey = m_sParentLogKey
    m_sParentLogKey = LogMsg("Start Options adjustment")
    nRootID = -1
     
    With fgOptions
        For nRow = 2 To .Rows - 1
            Set aRowData = .RowData(nRow)
            
            If Not aRowData Is Nothing Then
            
                Idle
                If nRootID <> aRowData.OptionRootID Then
                    nRootID = aRowData.OptionRootID
                    
                    If m_enCAType <> CA_MERGE_SPINOFF Then
                        If m_iMultiplier > 0 And m_dRate > 0# Then
                            gDBW.usp_ContractPriceCA_Save m_iCAId, Null, nRootID, m_dDelivery / (m_dRate * m_iMultiplier), False
                        
                            LogMsg "Update Options price with multiplier " & m_dDelivery / (m_dRate * m_iMultiplier)
                        End If
                    End If
                    
                End If
                
                dNewStrike = gCmn.ReadDbl(.Cell(flexcpText, nRow, OPT_STRIKE))
                dStrike = aRowData.Strike
                If Round(dNewStrike, 4) <> Round(dStrike, 4) Then
                    
                    sNewTicker = .Cell(flexcpText, nRow, OPT_STRIKETICKER)
                    
                    Set rsLog = gDBW.usp_OptionStrikeCA_Save(m_iCAId, nRootID, dStrike, sNewTicker, dNewStrike)
                    
                    With rsLog
                        If .RecordCount > 0 Then
                        .MoveFirst
                        Do While Not .EOF
                            Idle
                            LogMsg (.Fields("vcLogMessage"))
                            .MoveNext
                        Loop
                        End If
                    End With
                    
                End If
            End If
        Next
    End With
    LogMsg ("Complete Options adjustment")
    m_sParentLogKey = sLogKey

End Sub

Private Sub ExecuteUnderlyingAdjustment()
    Dim sLogKey As String
    Idle
    sLogKey = m_sParentLogKey
    m_sParentLogKey = LogMsg("Start Underlying adjustment")
    
    If m_enCAType <> CA_MERGE_SPINOFF And m_dRate > 0# Then
        gDBW.usp_ContractPriceCA_Save m_iCAId, m_iUndID, Null, 1 / m_dRate, True
        LogMsg "Update Underlying price with multiplier " & 1 / m_dRate
    ElseIf m_enCAType = CA_MERGE_SPINOFF Then
        
    End If
    
    LogMsg ("Complete Underlying adjustment")
    m_sParentLogKey = sLogKey
    
End Sub

Private Sub ExecuteOptionPositionsAdjustment()
    Dim rsOptTrades As ADODB.Recordset
    Dim dPrice, dOldPrice, dContractCoeff As Double, iCount, iOldCount, iTradeID As Long
    Dim sLogKey As String, nTradeCount#, sLogStr$
    
    Idle
    sLogKey = m_sParentLogKey
    
    m_sParentLogKey = LogMsg("Start Option Positions adjustment")
    
    If m_dDelivery > 0# Then _
        dContractCoeff = gCmn.ReadDbl(m_dRate * 100 / m_dDelivery)
        
    If m_enCAType <> CA_MERGE_SPINOFF Then
        Set rsOptTrades = gDBW.usp_TradeCA_Get(Null, Null, m_iUndID, Null)
        
        With rsOptTrades
            nTradeCount = .RecordCount
            If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                Idle
                dOldPrice = gCmn.ReadDbl(.Fields("fPrice"))
                iOldCount = gCmn.ReadLng(.Fields("iQuantity"))
                iTradeID = gCmn.ReadLng(.Fields("iTradeID"))
                
                Select Case m_enSharedRules
                    Case SR_CASH
                        iCount = Int(iOldCount * dContractCoeff)
                        If iCount > 0 And m_iMultiplier > 0 Then dPrice = dOldPrice * m_dDelivery * iOldCount / (m_iMultiplier * iCount)
                    Case SR_ROUNDDOWN
                        iCount = Int(iOldCount * dContractCoeff)
                        If iCount > 0 And m_iMultiplier > 0 Then dPrice = dOldPrice * m_dDelivery * iOldCount / (m_iMultiplier * iCount)
                    Case SR_ROUNDUP
                        iCount = Fix(iOldCount * dContractCoeff * (-1)) * (-1)
                        If iCount > 0 And m_iMultiplier > 0 Then dPrice = dOldPrice * m_dDelivery * iOldCount / (m_iMultiplier * iCount)
                End Select
                
                If iCount > 0 Then gDBW.usp_TradeCA_Save m_iCAId, iTradeID, iCount, dPrice
                
                .MoveNext
            Loop
            End If
        End With
        
        If nTradeCount Then
            sLogStr = "Update " & nTradeCount & " trades"
            LogMsg sLogStr
        End If
        
    End If
    LogMsg ("Complete Option Positions adjustment")
    m_sParentLogKey = sLogKey
End Sub

Private Sub ExecuteUnderlyingPositionsAdjustment()
    Dim rsUndTrades As ADODB.Recordset
    Dim dPrice, dOldPrice As Double, iCount, iOldCount, iTradeID As Long
    Dim Param As clsSyntParAtom, iTraderID As Long, nDelTrade#, nAddTrade#, nUpdTrade#, sLogStr$
    Dim sLogKey As String, tiStatus, tiIsPosition, tiIsBuy, tiIsClose As Byte
    
    Idle
    sLogKey = m_sParentLogKey
    m_sParentLogKey = LogMsg("Start Underlying Positions adjustment")
                
    nDelTrade = 0
    nAddTrade = 0
    nUpdTrade = 0
    
    Set rsUndTrades = gDBW.usp_TradeCA_Get(Null, m_iUndID, Null, Null)
    If m_enCAType <> CA_MERGE_SPINOFF Then
        
        With rsUndTrades
            If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                Idle
                dOldPrice = gCmn.ReadDbl(.Fields("fPrice"))
                iOldCount = gCmn.ReadLng(.Fields("iQuantity"))
                iTradeID = gCmn.ReadLng(.Fields("iTradeID"))
                
                Select Case m_enSharedRules
                    Case SR_CASH
                        iCount = Int(iOldCount * m_dRate)
                        If iCount > 0 Then dPrice = dOldPrice * iOldCount / iCount
                    Case SR_ROUNDDOWN
                        iCount = Int(iCount * m_dRate)
                        If iCount > 0 Then dPrice = dOldPrice * iOldCount / iCount
                    Case SR_ROUNDUP
                        iCount = Fix(iCount * m_dRate * (-1)) * (-1)
                        If iCount > 0 Then dPrice = dOldPrice * iOldCount / iCount
                End Select
                
                If iCount > 0 Then
                    gDBW.usp_TradeCA_Save m_iCAId, iTradeID, iCount, dPrice
                    nUpdTrade = nUpdTrade + 1
                End If
                
                .MoveNext
            Loop
            End If
        End With
    Else
        With rsUndTrades
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    Idle
                    iTradeID = gCmn.ReadLng(.Fields("iTradeID"))
                    
                    dOldPrice = gCmn.ReadDbl(.Fields("fPrice"))
                    iOldCount = gCmn.ReadLng(.Fields("iQuantity"))
                    tiIsClose = gCmn.ReadByte(.Fields("tiIsClose"))
                    tiIsBuy = gCmn.ReadByte(.Fields("tiIsBuy"))
                    iTraderID = gCmn.ReadLng(.Fields("iTraderID"))
                    tiIsPosition = gCmn.ReadByte(.Fields("tiIsPosition"))
                    tiStatus = gCmn.ReadByte(.Fields("tiStatus"))
                    
                    If m_clsSyntheticPar(Trim$(Str$(m_iUndID))) Is Nothing Then
                        gDBW.usp_TradeCA_Del m_iCAId, iTradeID
                        nDelTrade = nDelTrade + 1
                    End If
                    
                    For Each Param In m_clsSyntheticPar
                        
                        iCount = Int(iOldCount * Param.Weight)
                        If Param.Weight > 0# Then _
                            dPrice = (dOldPrice - m_dCashValue) * (Param.StrikeSettlement / 100#) / Param.Weight
                        
                        If Param.UnderlyingID = m_iUndID Then
                            gDBW.usp_TradeCA_Save m_iCAId, iTradeID, iCount, dPrice ' Update existing trade
                            nUpdTrade = nUpdTrade + 1
                        Else
                            ' Create new trade
                            gDBW.usp_TradeCA_Save m_iCAId, _
                                                    Null, _
                                                    iCount, _
                                                    dPrice, _
                                                    Param.UnderlyingID, _
                                                    tiIsClose, _
                                                    tiIsBuy, _
                                                    iTraderID, _
                                                    tiIsPosition, _
                                                    tiStatus
                            
                            nAddTrade = nAddTrade + 1
                        End If
                        
                    Next
                                    
                    .MoveNext
                Loop
            End If
        End With
    
    End If
    
    If nUpdTrade Then
        sLogStr = "Update " & nUpdTrade & " trade(s)"
        LogMsg sLogStr
    End If
    
    If nDelTrade Then
        sLogStr = "Delete " & nDelTrade & " trade(s)"
        LogMsg sLogStr
    End If
    
    If nAddTrade Then
        sLogStr = "Insert " & nAddTrade & " trade(s)"
        LogMsg sLogStr
    End If
    
    LogMsg ("Complete Underlying Positions adjustment")
    m_sParentLogKey = sLogKey
End Sub

Private Property Let EnableControls(ByVal blnEnabled As Boolean)
Dim ctlBox As Variant
    fgOptions.Visible = blnEnabled
    lblOptions.Visible = blnEnabled
    btnCopyLog.Visible = Not blnEnabled
    For Each ctlBox In Me.Controls
        If Not TypeOf ctlBox Is MSComCtlLib.TreeView Then
            If Not ctlBox.Name = "btnExecute" Then
                ctlBox.Enabled = blnEnabled
            End If
        Else
            ctlBox.Visible = Not blnEnabled
        End If
    Next ctlBox
    btnCopyLog.Enabled = Not blnEnabled
        
End Property

Public Function Idle()
    DoEvents
    If m_bCancel Then
        Err.Raise vbObjectError, "Corporate Action Manager", "Corporate Action processing was canceled by user"
    End If
End Function

Private Sub RollbackCA()
    Dim LastErrorDescription As String
    Dim cm As ADODB.Command
    Dim tStart As Date
    
    On Error GoTo Herr
    LogMsg "Rollback of corporate action for '" & m_sSymbol & "' started"
    m_bBusy = True
    btnClose.Enabled = False
    btnExecute.Enabled = False
    
    tStart = Now
                 
    Set m_CN = gDBW.Connection
    Screen.MousePointer = vbHourglass
     
    Set cm = GetLogCARollbackCommand(m_iCAId)
    With cm
        .CommandTimeout = nTimeOut
        ' Execute SQL request asynchronously
        .Execute , , adAsyncExecute
    End With
    
    
    On Error GoTo Failed
    ' Wait for the request completion
    Do
        If Not m_bBusy Then
            Exit Do
        End If
        If DateDiff("s", tStart, Now) > nTimeOut Then
            Err.Raise vbObject, "Corporate Action", "Rollback CA timeout expired"
        End If
        Idle
    Loop
    
    On Error GoTo Herr
    If nLastStatus = adStatusOK Then
        Set m_CN = Nothing
        Screen.MousePointer = vbDefault
    Else
        Err.Raise nLastErrorNumber, "Corporate Actions List", sLastErrorDescription
    End If
    
    
    LogMsg "Rollback of corporate action for '" & m_sSymbol & "' successfully finished"
Done:
    m_bBusy = False
    m_bEdit = False
    m_bCancel = False
    btnClose.Enabled = True
    Exit Sub
Failed:
    Dim n, s
    n = Err.Number
    s = Err.Description
    On Error Resume Next
    m_CN.Cancel
    Set m_CN = Nothing
    On Error GoTo Herr
    Err.Clear
    Err.Raise n, "Corporate Action", s
Herr:
    LastErrorDescription = Err.Description
    Screen.MousePointer = vbDefault
    ShowError
    LogMsg "Rollback of corporate action for '" & m_sSymbol & "' finished with error :" & LastErrorDescription
    Set m_CN = Nothing
    GoTo Done
End Sub

Private Function GetLogCARollbackCommand( _
    ByVal CorpActionID As Long _
) As ADODB.Command
    Debug.Print "usp_LogCA_Rollback", CorpActionID
    Set GetLogCARollbackCommand = New ADODB.Command
    With GetLogCARollbackCommand
        Set .ActiveConnection = m_CN
        .CommandText = "usp_LogCA_Rollback"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(CorpActionID, 0)
        End With
    End With
End Function


Private Sub m_CN_ExecuteComplete(ByVal RecordsAffected As Long, ByVal pError As ADODB.Error, adStatus As ADODB.EventStatusEnum, ByVal pCommand As ADODB.Command, ByVal pRecordset As ADODB.Recordset, ByVal pConnection As ADODB.Connection)
    On Error Resume Next
    m_bBusy = False
    If Not pError Is Nothing Then
        nLastErrorNumber = pError.Number
        sLastErrorDescription = pError.Description
        Exit Sub
    End If
    nLastStatus = adStatusOK
End Sub

