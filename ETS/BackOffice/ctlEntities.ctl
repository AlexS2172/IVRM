VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlEntities 
   Appearance      =   0  'Flat
   BackColor       =   &H00FFFFFF&
   ClientHeight    =   9570
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   13200
   ScaleHeight     =   9570
   ScaleWidth      =   13200
   Begin VB.TextBox txtMaturity 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   10200
      TabIndex        =   26
      Top             =   480
      Width           =   975
   End
   Begin VB.TextBox txtLowPremium 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   4380
      TabIndex        =   23
      Top             =   480
      Width           =   1335
   End
   Begin VB.Timer Timer2 
      Interval        =   1
      Left            =   9240
      Top             =   3480
   End
   Begin ElladaFlatControls.FlatButton btnDefineHTBstocks 
      Height          =   315
      Left            =   6360
      TabIndex        =   15
      Top             =   480
      Width           =   2055
      _ExtentX        =   3625
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Define HTB Underlyings..."
   End
   Begin VB.TextBox txtPositionThreshold 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   7560
      TabIndex        =   13
      Top             =   480
      Width           =   1455
   End
   Begin VB.PictureBox picLeft 
      Align           =   3  'Align Left
      Appearance      =   0  'Flat
      BackColor       =   &H00733429&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   9570
      Left            =   0
      Picture         =   "ctlEntities.ctx":0000
      ScaleHeight     =   9570
      ScaleWidth      =   2190
      TabIndex        =   3
      Top             =   0
      Width           =   2189
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   4
         Tag             =   "red"
         Top             =   120
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Groups"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   5
         Tag             =   "red"
         Top             =   480
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Exchanges"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   3
         Left            =   120
         TabIndex        =   6
         Tag             =   "red"
         Top             =   840
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Holidays"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   7
         Left            =   120
         TabIndex        =   7
         Tag             =   "red"
         Top             =   2280
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Brokers"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   6
         Left            =   120
         TabIndex        =   8
         Tag             =   "red"
         Top             =   1920
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Strategies"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   5
         Left            =   120
         TabIndex        =   9
         Tag             =   "red"
         Top             =   1560
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Traders"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   8
         Left            =   135
         TabIndex        =   10
         Tag             =   "red"
         Top             =   2640
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "IR Curves"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   4
         Left            =   120
         TabIndex        =   16
         Tag             =   "red"
         Top             =   1200
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Trader Groups"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   9
         Left            =   120
         TabIndex        =   21
         Tag             =   "red"
         Top             =   3000
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Orders Destinations"
         Alignment       =   1
         PressForeColor  =   12640511
      End
      Begin ElladaFlatControls.ActiveLabel btnEntity 
         Height          =   255
         Index           =   10
         Left            =   120
         TabIndex        =   22
         Tag             =   "red"
         Top             =   3360
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Counterparties"
         Alignment       =   1
         PressForeColor  =   12640511
      End
   End
   Begin VSFlex7Ctl.VSFlexGrid grdEntity 
      Height          =   4095
      Left            =   2400
      TabIndex        =   0
      Top             =   960
      Width           =   6735
      _cx             =   11880
      _cy             =   7223
      _ConvInfo       =   1
      Appearance      =   2
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
      BackColorSel    =   -2147483637
      ForeColorSel    =   0
      BackColorBkg    =   16777215
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
      Begin VB.Timer Timer1 
         Left            =   4920
         Top             =   1320
      End
      Begin MSComCtl2.DTPicker DTPicker1 
         Height          =   255
         Left            =   2760
         TabIndex        =   1
         Top             =   1200
         Visible         =   0   'False
         Width           =   1215
         _ExtentX        =   2143
         _ExtentY        =   450
         _Version        =   393216
         Format          =   25493505
         CurrentDate     =   36936
      End
      Begin VB.Image imgProp 
         Height          =   180
         Left            =   4440
         Picture         =   "ctlEntities.ctx":86D4
         Top             =   1200
         Visible         =   0   'False
         Width           =   180
      End
   End
   Begin ElladaFlatControls.FlatButton btnDown 
      Height          =   300
      Left            =   6840
      TabIndex        =   17
      Top             =   60
      Width           =   615
      _ExtentX        =   1085
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
      Caption         =   "Down"
   End
   Begin ElladaFlatControls.FlatButton btnUp 
      Height          =   300
      Left            =   6360
      TabIndex        =   18
      Top             =   60
      Width           =   375
      _ExtentX        =   661
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
      Caption         =   "Up"
   End
   Begin ElladaFlatControls.FlatButton btnRecalc 
      Height          =   315
      Left            =   11280
      TabIndex        =   20
      Top             =   480
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Calc Vega Weights"
   End
   Begin VB.Label lblMaturity 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Maturity,Days"
      Height          =   255
      Left            =   9120
      TabIndex        =   25
      Top             =   510
      Visible         =   0   'False
      Width           =   975
   End
   Begin VB.Label lblLowPremium 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Low Premium Limit, $ :"
      Height          =   255
      Left            =   2400
      TabIndex        =   24
      Top             =   510
      Width           =   1935
   End
   Begin VB.Label lblPriority 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Priority :"
      Height          =   255
      Left            =   5280
      TabIndex        =   19
      Top             =   120
      Visible         =   0   'False
      Width           =   975
   End
   Begin MSForms.ComboBox cmbRateRule 
      Height          =   315
      Left            =   3480
      TabIndex        =   14
      Top             =   480
      Width           =   2175
      VariousPropertyBits=   746604571
      BorderStyle     =   1
      DisplayStyle    =   7
      Size            =   "3836;556"
      MatchEntry      =   1
      ShowDropButtonWhen=   1
      SpecialEffect   =   0
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblPositionThreshold 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Position Threshold :"
      Height          =   255
      Left            =   5760
      TabIndex        =   12
      Top             =   510
      Width           =   1455
   End
   Begin VB.Label lblRateRule 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Rate Rule :"
      Height          =   255
      Left            =   2400
      TabIndex        =   11
      Top             =   510
      Width           =   975
   End
   Begin VB.Label lblCaption 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "lblCaption"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   375
      Left            =   2400
      TabIndex        =   2
      Top             =   0
      Width           =   3615
   End
   Begin VB.Menu mnuContext 
      Caption         =   "Context"
      Begin VB.Menu mnuAdd 
         Caption         =   "Add"
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "Edit"
      End
      Begin VB.Menu mnuDelete 
         Caption         =   "Delete"
      End
      Begin VB.Menu mnuContextLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuProperty 
         Caption         =   "Property"
      End
   End
End
Attribute VB_Name = "ctlEntities"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

' new value string
Private Const STR_NEWVALUE = "<New Value>"

' entity type constants
Public Enum eEntityType
    ENT_GROUPS = 1
    ENT_EXCHANGES = 2
    ENT_HOLYDAYS = 3
    ENT_TRADERGROUPS = 4
    ENT_TRADERS = 5
    ENT_STRATEGIES = 6
    ENT_BROKERS = 7
    ENT_IRCURVES = 8
    ENT_ORDDESTIN = 9
    ENT_COUNTERPARTIES = 10
    ENT_END
End Enum

' control state constants
Private Enum eState
    STATE_NONE
    STATE_LOAD
    STATE_EDIT
    STATE_NEW
    STATE_DELETE
End Enum

' column definition
Private Type TEntColumn
    Caption As String
    Name As String
    Type As VbVarType
    Visible As Boolean
    ColWidth As Single
    EditWidth As Single
    HaveProperties As Boolean
    DataBinding As Boolean
    DisableEdit As Boolean
    ChangeEvent As Boolean
    ValidateEvent As Boolean
    FormatString As String
    EditMaxLen As Long
End Type


Private Const MINT_ENTITIES_COUNT = ENT_END - 1 ' number of entities
Private Const MINT_COLUMN_COUNT = 12 ' max number of editing columns

' entity parameters definition
Private Type TEntity
    Cols As Long
    Col(MINT_COLUMN_COUNT) As TEntColumn
End Type

' array of entity definitions
Private m_arrEntDef(MINT_ENTITIES_COUNT) As TEntity

' current control state
Private m_State As eState
' current entity type
Private m_EntityType As eEntityType
' current entity recordset
Private m_rsEntity As ADODB.Recordset
Private m_IRCurve As InterestRateCurve

Private m_bDTPIsDroppedDown As Boolean

Private m_dSpotDate As Date
Private m_nMaturity As Long
Private m_dLowPremium As Double

Private Sub btnDefineHTBstocks_Click()
    On Error Resume Next
    Load frmUnderlyingParams
    frmUnderlyingParams.Show vbModal, frmMain
    Unload frmUnderlyingParams
End Sub

'**************************************************
'**************************************************
'              buttons events
'**************************************************
'**************************************************

Private Sub btnEntity_Click(Index As Integer)
    On Error Resume Next
    SetEntityType Index
End Sub

Private Sub cmbRateRule_Change()
    On Error GoTo EH
    
    lblPositionThreshold.Visible = (cmbRateRule.ListIndex = 1)
    txtPositionThreshold.Visible = lblPositionThreshold.Visible
    btnDefineHTBstocks.Visible = True
    
    If m_State <> STATE_LOAD Then
        m_IRCurve.RuleType = cmbRateRule.ListIndex
        gDBW.usp_IRCurve_Save m_IRCurve.ID, , m_IRCurve.RuleType
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Unable to save interest rate."
End Sub

'**************************************************
'**************************************************
'              datepicker events
'**************************************************
'**************************************************


Private Sub DTPicker1_Change()
On Error Resume Next
    grdEntity.Text = DTPicker1.Value
End Sub

Private Sub DTPicker1_CloseUp()
    Timer1.Enabled = True
    m_bDTPIsDroppedDown = False
End Sub

Private Sub DTPicker1_DropDown()
    m_bDTPIsDroppedDown = True
End Sub

Private Sub DTPicker1_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    ' close date picker when user hits escape or return
    Select Case KeyCode
        Case vbKeyEscape
            grdEntity.Text = DTPicker1.Tag
            DTPicker1.Visible = False
        Case vbKeyReturn
            DTPicker1.Visible = False
        Case vbKeyTab
            DTPicker1.Visible = False
            grdEntity.SetFocus
            SendKeys "{TAB}"
    End Select
End Sub

Private Sub DTPicker1_LostFocus()
On Error Resume Next
    Timer1.Enabled = True
    DTPicker1.Refresh
    DTPicker1.Visible = False
    
    'SendKeys "{ESC}"
    grdEntity.SetFocus
End Sub



Private Sub DTPicker1_Validate(Cancel As Boolean)
    grdEntity.SetFocus
End Sub

Private Sub grdEntity_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    grdEntity.EditMaxLength = m_arrEntDef(m_EntityType).Col(Col).EditMaxLen
End Sub

'**************************************************
'**************************************************
'              grid events
'**************************************************
'**************************************************


Private Sub grdEntity_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
On Error Resume Next
    
   
    With grdEntity
        ' Opening property window
        If (Button And vbLeftButton) Then
            If X >= (.Cell(flexcpLeft, , .Cols - 1) + .Cell(flexcpWidth, , .Cols - 1) - imgProp.Width) Then
                If (.MouseRow > 0) And (.MouseRow < .Rows - 2) Then
                    .Row = .MouseRow
                    EntityProp
                End If
            End If
        End If
        
        ' Opening context menu
        If (Button And vbRightButton) Then
            If (.EditWindow = 0) Then
                If (.MouseRow > 0) And (.MouseRow < .Rows - 1) Then
                    .Row = .MouseRow
                    PopupMenu mnuContext
                End If
            End If
        End If
    End With
End Sub

Private Sub grdEntity_BeforeRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
    If NewRow <> OldRow Then
        Cancel = Not EntityCanChangeRow
    End If
End Sub

Private Sub grdEntity_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
On Error Resume Next
    If OldRow = NewRow Then Exit Sub
    ColorCells NewRow
End Sub

Private Sub grdEntity_BeforeScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

Private Sub grdEntity_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

Private Sub grdEntity_ChangeEdit()
On Error Resume Next
        
    EntityEdit
    
End Sub

Private Sub grdEntity_KeyUp(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    With grdEntity
        Select Case KeyCode
        ' escape handling
        Case vbKeyEscape
            If Timer1.Enabled Then Exit Sub
            If ((m_State = STATE_EDIT) Or (m_State = STATE_NEW)) And (.EditWindow = 0) Then
                EntityUndoEdit
            End If
        ' insert handling
        Case vbKeyInsert
            If (m_State = STATE_NONE) Then
                mnuAdd_Click
            End If
        ' delete handling
        Case vbKeyDelete
            If (m_State = STATE_NONE) Then
                EntityDelete
            End If
        Case 93
            PopupMenu mnuContext, , .CellLeft + .Left, .CellTop + .CellHeight + .Top
        Case vbKeyF10
            If Shift = vbShiftMask Then PopupMenu mnuContext, , .CellLeft + .Left, .CellTop + .CellHeight + .Top
        End Select
    End With
End Sub

Private Sub grdEntity_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    With grdEntity
        If m_arrEntDef(m_EntityType).Col(Col).DisableEdit Then
            Cancel = True
            Exit Sub
        End If
        
        If .Cell(flexcpChecked, Row, Col) <> flexNoCheckbox Then
'            .Row = Row
            If .Row = Row Then
                EntityEdit
            Else
                Cancel = True
                Exit Sub
            End If
        End If
        
        ' if this is a date column, edit it with the date picker control
        If .ColDataType(Col) = flexDTDate Then
            
            ' we'll handle the editing ourselves
            Cancel = True
            
            ' position date picker control over cell
            DTPicker1.Move .CellLeft, .CellTop - 10, .CellWidth, .CellHeight + 50
            
            ' initialize value, save original in tag in case user hits escape
            DTPicker1.Value = IIf(.Text = "", Date, .Text)
            DTPicker1.Tag = .Text
                        
            grdEntity.Text = DTPicker1.Value
            
            ' show and activate date picker control
            DTPicker1.Visible = True
            DTPicker1.SetFocus
            
            EntityEdit

            ' make it drop down the calendar
            'SendKeys "{f4}"
                        
        End If
        
    End With
End Sub



Private Sub grdEntity_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim dExp As Date
    Dim lDays As Long
    Dim lPeriod As Long
    Dim sInt As String
    Dim eInt As etsPeriodTypes
    Dim f As Double
    Dim s As String
    Dim s1 As String
    

    EntityEdit
    
    With grdEntity
        Select Case m_EntityType
        Case ENT_IRCURVES
            Select Case Col
            Case 2
                gCmn.GetPeriod (.EditText), m_dSpotDate, _
                dExp, lPeriod, lDays, eInt, sInt
                .EditText = lPeriod & " " & sInt
                .TextMatrix(Row, 3) = eInt
                .TextMatrix(Row, 4) = lPeriod
                .TextMatrix(Row, 5) = dExp
            Case 6, 7, 8
                
                s = .EditText
                f = Abs(gCmn.Str2Num(s))
                If f >= 1 Then f = f / 100
                s1 = Format(f, "#,##0.0000%")
                If s1 <> s Then
                    .EditText = f
                End If
            Case 9, 10 '8,
                
                s = .EditText
                f = Abs(gCmn.Str2Num(s))
                If f >= 1 Then f = f / 100
                s1 = Format(f, "#,##0.0000%")
                If s1 <> s Then
                    .EditText = f
                End If
            
            End Select
            .TextMatrix(Row, 1) = m_IRCurve.ID
        End Select
    End With

End Sub



Private Sub lbMaturity_Click()

End Sub

'**************************************************
'**************************************************
'              menu events
'**************************************************
'**************************************************


Private Sub mnuAdd_Click()
On Error Resume Next
        
    With grdEntity
        .Row = .Rows - 2
        .EditCell
    End With
End Sub

Private Sub mnuDelete_Click()
On Error Resume Next
    EntityDelete
End Sub

Private Sub mnuEdit_Click()
On Error Resume Next
    With grdEntity
        .EditCell
    End With
End Sub



Private Sub mnuProperty_Click()
On Error Resume Next
    Timer2.Enabled = True
End Sub

Private Sub Timer1_Timer()
    Timer1.Enabled = False
End Sub

Private Sub Timer2_Timer()
    On Error Resume Next
    Timer2.Enabled = False
    EntityProp
End Sub

Private Sub txtLowPremium_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error GoTo EH
    Dim cLowPremium As Currency: cLowPremium = 0
    If KeyCode = vbKeyReturn Then
        
        On Error Resume Next
        cLowPremium = CCur(txtLowPremium.Text)
        On Error GoTo EH
        
        If cLowPremium > 0 Then
            m_dLowPremium = cLowPremium
            gDBW.usp_CommissionLowPremiumLimit_Save m_dLowPremium
        End If
        txtLowPremium.Text = Format$(m_dLowPremium, "#,###,##0.00;(#,###,##0.00)")
        
    ElseIf KeyCode = vbKeyEscape Then
        txtLowPremium.Text = Format$(m_dLowPremium, "#,###,##0.00;(#,###,##0.00)")
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Unable to save low premium level."
End Sub

Private Sub txtLowPremium_LostFocus()
    On Error Resume Next
    txtLowPremium_KeyDown vbKeyReturn, 0
End Sub

Private Sub txtPositionThreshold_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error GoTo EH
    Dim cThreshold As Currency: cThreshold = 0
    If KeyCode = vbKeyReturn Then
        
        On Error Resume Next
        cThreshold = CCur(txtPositionThreshold.Text)
        On Error GoTo EH
        
        If cThreshold > 0 Then
            m_IRCurve.PosThreshold = cThreshold
            gDBW.usp_IRCurve_Save m_IRCurve.ID, , , , m_IRCurve.PosThreshold
        End If
        txtPositionThreshold.Text = Format$(m_IRCurve.PosThreshold, "#,##0;(#,##0)")
        
    ElseIf KeyCode = vbKeyEscape Then
        txtPositionThreshold.Text = Format$(m_IRCurve.PosThreshold, "#,##0;(#,##0)")
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Unable to save interest rate."
End Sub

Private Sub txtPositionThreshold_LostFocus()
    On Error Resume Next
    txtPositionThreshold_KeyDown vbKeyReturn, 0
End Sub

'**************************************************
'**************************************************
'              usercontrol events
'**************************************************
'**************************************************


Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    With grdEntity
        .Width = UserControl.ScaleWidth - .Left
        If m_EntityType <> ENT_IRCURVES Then
            .Height = UserControl.ScaleHeight - (lblCaption.Top + lblCaption.Height + 500)
        Else
            .Height = UserControl.ScaleHeight - (cmbRateRule.Top + cmbRateRule.Height + 500)
        End If
    End With
End Sub


'**************************************************
'**************************************************
'              public functions
'**************************************************
'**************************************************

' control initializing
Public Function Init() As Boolean
On Error GoTo EH

    gCmn.CustomizeControls Controls

    Timer1.Interval = 500
    Timer1.Enabled = False
    
    Timer2.Interval = 1
    Timer2.Enabled = False
    
    Init = False
    
    EntDefInit
    
    GridInit
    
    cmbRateRule.AddItem "Always Use Neutral Rate"
    cmbRateRule.AddItem "Rate Based on Position"
    cmbRateRule.ListIndex = 0
    
    m_nMaturity = g_WtdVega.Maturity
    txtMaturity.Text = CStr(m_nMaturity)
    
    m_dSpotDate = VBA.Date
    Init = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Unable to initialize entities control"
End Function

' Gettimg Entity Type Property
Public Property Get EntityType() As eEntityType
On Error Resume Next
    EntityType = m_EntityType
End Property

' Setting Entity Type Property
Public Property Let EntityType(ByVal NewEntityType As eEntityType)
On Error Resume Next
    SetEntityType NewEntityType
End Property

' Setting Entity Type
Private Function SetEntityType(ByVal NewEntityType As eEntityType) As Boolean
On Error GoTo EH
    SetEntityType = False
    If EntityCanChangeRow Then
        m_EntityType = NewEntityType
        EntityDisplay
        EntityLoad
    End If
    SetEntityType = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Unable to set entity type"
End Function

' determine if we can change the current row
Public Function EntityCanChangeRow() As Boolean
Dim iAnswr As VbMsgBoxResult
On Error GoTo EH
    
    If m_bDTPIsDroppedDown Then Exit Function
    If DTPicker1.Visible Then Exit Function

    If (m_State = STATE_NEW) Or (m_State = STATE_EDIT) Then
        iAnswr = vbYes
        'gCmn.MyMsgBox
        'iAnswr = MsgBox("Do you wish to save changes?", vbYesNoCancel + vbQuestion)
'        iAnswr = MsgBox("Do you wish to save changes?", vbYesNo + vbQuestion)
        If iAnswr = vbYes Then
            ' saving data
            EntityCanChangeRow = EntitySave
        ElseIf iAnswr = vbNo Then
            ' undoing changes
            EntityCanChangeRow = EntityUndoEdit
        Else
            ' do nothing
            EntityCanChangeRow = False
        End If
    Else
        EntityCanChangeRow = True
    End If
Exit Function
EH:
    ShowError "Error while changing row"
End Function



'**************************************************
'**************************************************
'              private functions
'**************************************************
'**************************************************





'**************************************************
'**************************************************
'           working with entities
'**************************************************
'**************************************************

' loading entities list
Private Function EntityLoad() As Boolean
    Dim i&
    Dim rsIR As ADODB.Recordset
    On Error GoTo EH
    
    EntityLoad = False
    
    SetState STATE_LOAD
    
    mnuAdd.Enabled = False
    mnuEdit.Enabled = False
    mnuDelete.Enabled = False
    mnuProperty.Enabled = False
    
    If Not (m_rsEntity Is Nothing) Then
        If m_rsEntity.State = adStateOpen Then m_rsEntity.Close
    End If
    
    ' loading data from DB
    Select Case m_EntityType
    Case ENT_BROKERS
        Set m_rsEntity = gDBW.usp_Broker_Get(Null)
    Case ENT_EXCHANGES
        Set m_rsEntity = gDBW.usp_Exchange_Get(Null)
    Case ENT_GROUPS
        Set m_rsEntity = gDBW.usp_ContractGroup_Get(Null)
    Case ENT_HOLYDAYS
        Set m_rsEntity = gDBW.usp_Holiday_Get(Null, Null)
    Case ENT_STRATEGIES
        Set m_rsEntity = gDBW.usp_Strategy_Get(Null)
    Case ENT_TRADERS
        Set m_rsEntity = gDBW.usp_BoTrader_Get(Null)
    Case ENT_TRADERGROUPS
        Set m_rsEntity = gDBW.usp_BoTraderGroup_Get(Null)
    Case ENT_IRCURVES
        Set rsIR = gDBW.usp_IRCurve_Get(Null)
        If Not rsIR.EOF Then
            m_IRCurve.ID = gCmn.ReadLng(rsIR!iCurveID)
            m_IRCurve.Name = gCmn.ReadLng(rsIR!vcCurveName)
            m_IRCurve.RuleType = gCmn.ReadLng(rsIR!iRuleTypeID)
            m_IRCurve.CurveType = gCmn.ReadLng(rsIR!iCurveTypeID)
            m_IRCurve.PosThreshold = gCmn.ReadCur(rsIR!mPositionThreshold)
        Else
            m_IRCurve.Name = "<Default>"
            m_IRCurve.RuleType = 0
            m_IRCurve.CurveType = 0
            m_IRCurve.PosThreshold = 100000
            m_IRCurve.ID = gDBW.usp_IRCurve_Save(Null, m_IRCurve.Name, m_IRCurve.RuleType, m_IRCurve.CurveType, m_IRCurve.PosThreshold)
        End If
        
        If m_IRCurve.RuleType < 0 Then
            m_IRCurve.RuleType = 0
        ElseIf m_IRCurve.RuleType > 1 Then
            m_IRCurve.RuleType = 1
        End If
        
        Set m_rsEntity = gDBW.usp_IRPoint_Get(m_IRCurve.ID)
    
    Case ENT_ORDDESTIN
        Set m_rsEntity = gDBW.usp_OrderExecDest_Get
    Case ENT_COUNTERPARTIES
        Set m_rsEntity = gDBW.usp_TntCounterParty_Get
    End Select
    
    ' displaying data
    Dim m_iRow As Long
    m_iRow = 0
    With grdEntity
        .Rows = m_rsEntity.RecordCount + 2
        If Not m_rsEntity.BOF Then m_rsEntity.MoveFirst
        For i = 1 To m_rsEntity.RecordCount
            FillRow i
            m_rsEntity.MoveNext
        Next i
        .Rows = .Rows + 1
        For i = 1 To m_arrEntDef(m_EntityType).Cols - 1
            If Not m_arrEntDef(m_EntityType).Col(i).DisableEdit Then
                .Cell(flexcpText, .Rows - 2, i, .Rows - 1, i) = STR_NEWVALUE
            End If
        Next
        .RowHidden(.Rows - 1) = True
        .Row = 1
        .Col = 1
        .AutoSize 0, m_arrEntDef(m_EntityType).Cols - 1, , 100
    End With
    
    If m_EntityType = ENT_IRCURVES Then
        cmbRateRule.ListIndex = m_IRCurve.RuleType
        txtPositionThreshold.Text = Format$(m_IRCurve.PosThreshold, "#,##0;(#,##0)")
    End If
    
    If m_EntityType = ENT_BROKERS Then
        m_dLowPremium = GetCommissionLowPremiumLimit
        txtLowPremium.Text = Format$(m_dLowPremium, "#,###,##0.00;(#,###,##0.00)")
    End If
    
    ColorCellsAll
    
    ' enabling menu items
    mnuAdd.Enabled = True
    mnuEdit.Enabled = (m_rsEntity.RecordCount > 0)
    mnuDelete.Enabled = (m_rsEntity.RecordCount > 0)
    mnuProperty.Enabled = (m_arrEntDef(m_EntityType).Col(m_arrEntDef(m_EntityType).Cols - 1).HaveProperties) _
                            And (m_rsEntity.RecordCount > 0)
    
    SetState STATE_NONE
    
    EntityLoad = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Unable to load data" & vbCrLf & Err.Description
    SetState STATE_NONE
End Function


' filling grig row
Private Sub FillRow(ByVal iRow As Long)
Dim Ent As TEntity
Dim i As Long
Dim lPeriod As Long
Dim lPeriodType As etsPeriodTypes
Dim sPeriod As String
Dim dExp As Date

    On Error GoTo EH
    Ent = m_arrEntDef(m_EntityType)
    For i = 0 To Ent.Cols - 1
        If Ent.Col(i).DataBinding Then
            Select Case Ent.Col(i).Type
            Case vbBoolean
                grdEntity.TextMatrix(iRow, i) = gCmn.ReadLng(m_rsEntity.Fields(Ent.Col(i).Name).Value)
            Case vbLong
                grdEntity.TextMatrix(iRow, i) = gCmn.ReadLng(m_rsEntity.Fields(Ent.Col(i).Name).Value)
            Case vbDate
                grdEntity.TextMatrix(iRow, i) = gCmn.ReadDate(m_rsEntity.Fields(Ent.Col(i).Name).Value)
            Case vbString
                grdEntity.TextMatrix(iRow, i) = gCmn.ReadStr(m_rsEntity.Fields(Ent.Col(i).Name).Value)
            Case vbDouble
                grdEntity.TextMatrix(iRow, i) = gCmn.ReadDbl(m_rsEntity.Fields(Ent.Col(i).Name).Value)
            End Select
        End If
        
        If m_EntityType = ENT_IRCURVES Then
            'If I = 8 Then
            '    grdEntity.TextMatrix(iRow, 8) = (gCmn.ReadDbl(m_rsEntity.Fields(Ent.Col(6).Name).Value) + _
            '                                    gCmn.ReadDbl(m_rsEntity.Fields(Ent.Col(7).Name).Value)) / 2#
            'Else
            If i = 10 Then
                grdEntity.TextMatrix(iRow, 10) = (gCmn.ReadDbl(m_rsEntity.Fields(Ent.Col(7).Name).Value) + _
                                                gCmn.ReadDbl(m_rsEntity.Fields(Ent.Col(9).Name).Value)) / 2#
            End If
        End If
        
        ' drawing property buttons
        If Ent.Col(m_arrEntDef(m_EntityType).Cols - 1).HaveProperties Then
            grdEntity.Cell(flexcpPicture, iRow, grdEntity.Cols - 1) = imgProp.Picture
            grdEntity.Cell(flexcpPictureAlignment, iRow, grdEntity.Cols - 1) = flexPicAlignRightCenter
        Else
            grdEntity.Cell(flexcpPicture, iRow, grdEntity.Cols - 1) = Nothing
        End If
        
        ' drawing comboboxes
        If Not Ent.Col(i).DisableEdit And Ent.Col(i).Type = vbDate Then
            grdEntity.ColComboList(i) = "Dummy"
        Else
            grdEntity.ColComboList(i) = ""
        End If
    Next i
    
    If m_EntityType = ENT_IRCURVES Then
        grdEntity.TextMatrix(iRow, 1) = m_IRCurve.ID
        lPeriod = grdEntity.TextMatrix(iRow, 4)
        lPeriodType = grdEntity.TextMatrix(iRow, 3)
        Select Case lPeriodType
        Case etsPeriodDay
            sPeriod = lPeriod & " day"
            dExp = DateAdd("d", lPeriod, m_dSpotDate)
        Case etsPeriodWeek
            sPeriod = lPeriod & " week"
            dExp = DateAdd("w", lPeriod, m_dSpotDate)
        Case etsPeriodMonth
            sPeriod = lPeriod & " month"
            dExp = DateAdd("m", lPeriod, m_dSpotDate)
        Case etsPeriodYear
            sPeriod = lPeriod & " year"
            dExp = DateAdd("yyyy", lPeriod, m_dSpotDate)
        End Select
        If lPeriod > 1 Then
            sPeriod = sPeriod & "s"
        End If
        grdEntity.TextMatrix(iRow, 2) = sPeriod
        grdEntity.TextMatrix(iRow, 5) = dExp
        
    End If
    
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Error occured when filling grid with values"
End Sub


' Setting the display for a current entity type
Private Function EntityDisplay() As Boolean
Dim i As Long
Dim Ent As TEntity
On Error GoTo EH

    EntityDisplay = False
    
    btnUp.Visible = False
    btnDown.Visible = False
    lblPriority.Visible = False

    lblRateRule.Visible = m_EntityType = ENT_IRCURVES
    cmbRateRule.Visible = m_EntityType = ENT_IRCURVES
    btnRecalc.Visible = m_EntityType = ENT_IRCURVES
    txtMaturity.Visible = m_EntityType = ENT_IRCURVES
    lblMaturity.Visible = m_EntityType = ENT_IRCURVES
    
    lblPositionThreshold.Visible = m_EntityType = ENT_IRCURVES
    txtPositionThreshold.Visible = m_EntityType = ENT_IRCURVES
    btnDefineHTBstocks.Visible = m_EntityType = ENT_IRCURVES
    
    lblLowPremium.Visible = m_EntityType = ENT_BROKERS
    txtLowPremium.Visible = m_EntityType = ENT_BROKERS
    
    ' adjusting screen
    Select Case m_EntityType
    Case ENT_BROKERS
        lblCaption.Caption = "Brokers"
    Case ENT_EXCHANGES
        lblCaption.Caption = "Exchanges"
    Case ENT_GROUPS
        lblCaption.Caption = "Groups"
    Case ENT_HOLYDAYS
        lblCaption.Caption = "Holidays"
    Case ENT_STRATEGIES
        lblCaption.Caption = "Strategies"
    Case ENT_TRADERS
        lblCaption.Caption = "Traders"
    Case ENT_IRCURVES
        lblCaption.Caption = "IR Curves"
    Case ENT_TRADERGROUPS
        lblCaption.Caption = "Traders Groups"
    Case ENT_ORDDESTIN
        lblCaption.Caption = "Orders destinations"
    Case ENT_COUNTERPARTIES
        lblCaption.Caption = "Counterparties"
    End Select
    
    ' highlighting active button
    For i = 1 To MINT_ENTITIES_COUNT
        btnEntity(i).ForeColor = GCOLOR_RED_ACTIVE_LABEL_FORE
        btnEntity(i).HoverForeColor = GCOLOR_RED_ACTIVE_LABEL_HOVERFORE
        btnEntity(i).PressForeColor = GCOLOR_RED_ACTIVE_LABEL_PRESSFORE
    Next i
    btnEntity(m_EntityType).ForeColor = vbYellow
    btnEntity(m_EntityType).HoverForeColor = vbYellow
    
    
    ' adjusting grid
    Ent = m_arrEntDef(m_EntityType)
    With grdEntity
        .Rows = 1
        .Cols = Ent.Cols
        For i = 0 To Ent.Cols - 1
            .ColHidden(i) = Not (Ent.Col(i).Visible)
            .TextMatrix(0, i) = Ent.Col(i).Caption
            .ColKey(i) = Ent.Col(i).Name
            .ColWidth(i) = Ent.Col(i).ColWidth
            .ColDataType(i) = Ent.Col(i).Type
            .ColFormat(i) = Ent.Col(i).FormatString
            .ColComboList(i) = ""
        Next i
    
        .Cell(flexcpBackColor, 0, 0, 0, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .Cell(flexcpForeColor, 0, 0, 0, .Cols - 1) = GCOLOR_TABLE_CAPTION_FORE
    
        If m_EntityType = ENT_IRCURVES Then
            .Top = cmbRateRule.Top + cmbRateRule.Height + 100
            .Height = UserControl.ScaleHeight - .Top
        
            lblRateRule.Visible = True
            cmbRateRule.Visible = True
            btnRecalc.Visible = True
            txtMaturity.Visible = True
            lblMaturity.Visible = True
            
            cmbRateRule_Change
            .ExplorerBar = flexExNone
                    
        ElseIf m_EntityType = ENT_BROKERS Then
            .Top = lblLowPremium.Top + lblLowPremium.Height + 100
            .Height = UserControl.ScaleHeight - .Top
            .ExplorerBar = flexExNone
                                
        Else
            .Top = lblCaption.Top + lblCaption.Height + 100
            .Height = UserControl.ScaleHeight - .Top
            .ExplorerBar = flexExNone
            
        End If
    End With
    
    EntityDisplay = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Error occured while setting entity type"
End Function

' saving entity data
Private Function EntitySave() As Boolean
Dim Ret As Long
Dim pars(MINT_COLUMN_COUNT) As Variant
Dim i As Long
Dim Ent As TEntity
On Error GoTo EH

    EntitySave = False
    
    ' parsing
    Ent = m_arrEntDef(m_EntityType)
    With grdEntity
        For i = 0 To Ent.Cols - 1
            If Ent.Col(i).DataBinding Then
                Select Case Ent.Col(i).Type
                Case vbBoolean
                    pars(i) = IIf(gCmn.ReadLng(.TextMatrix(.Row, i)) = 0, 0, 1)
                Case vbLong
                    pars(i) = gCmn.ReadLng(.TextMatrix(.Row, i))
                Case vbDouble
                    pars(i) = gCmn.ReadDbl(.TextMatrix(.Row, i))
                    If i = 8 And .TextMatrix(.Row, i) = "<New Value>" Then
                        pars(i) = Null
                    End If

                Case vbDate
                    pars(i) = gCmn.ReadDate(.TextMatrix(.Row, i))
                Case vbString
                    pars(i) = Trim$(.TextMatrix(.Row, i))
                End Select

            End If
        Next i
    End With
    
    If m_State = STATE_NEW Then
        pars(0) = Null
    Else
        pars(0) = gCmn.ReadLng(pars(0))
        pars(0) = IIf(pars(0) = 0, Null, pars(0))
    End If
    
    ' writing
    Select Case m_EntityType
    Case ENT_BROKERS
        If Len(pars(2)) > 64 Then
            ShowWarning "Unable to save data" & vbCrLf & "Broker name should be less than 64 symbols"
            Exit Function
        End If
        Ret = gDBW.usp_Broker_Save(pars(0), pars(2), pars(3), pars(1), pars(4), pars(5), pars(7), pars(6))
    Case ENT_EXCHANGES
        If Len(CStr(pars(2))) > 1 Then
            ShowWarning "The maximum length of OCC Exchange symbol is 1"
            Exit Function
        End If
        Ret = gDBW.usp_Exchange_Save(pars(0), pars(3), pars(4), pars(1), pars(5), pars(6), pars(2))
        Call ChangeExchanges
    Case ENT_GROUPS
        Ret = gDBW.usp_ContractGroup_Save(pars(0), pars(1), pars(2))
    Case ENT_HOLYDAYS
        pars(1) = gCmn.ReadDate(pars(1))
        Ret = gDBW.usp_Holiday_Save(pars(0), pars(1), pars(2))
        Call ReloadHolidays
    Case ENT_STRATEGIES
        Ret = gDBW.usp_BoStrategy_Save(pars(0), pars(1), pars(2))
    Case ENT_TRADERS
        If Len(Trim$(pars(1))) = 0 Or Len(Trim$(pars(2))) = 0 Then
            ShowWarning "Please, fill the new trader name and(or) acronym field."
            Exit Function
        End If
        Ret = gDBW.usp_BoTrader_Save(pars(0), pars(1), pars(2), pars(3), DBWrite(pars(4), 0))
    Case ENT_IRCURVES
        Ret = gDBW.usp_IRPoint_Save(pars(0), pars(1), pars(3), pars(4), pars(6), pars(9), pars(7), pars(8))
        LoadInterestRates
    Case ENT_TRADERGROUPS
        Ret = gDBW.usp_BoTraderGroup_Save(pars(0), pars(1), pars(2))
    Case ENT_ORDDESTIN
        Ret = gDBW.usp_OrderExecDest_Save(pars(0), pars(1), pars(2), pars(3), pars(4))
    Case ENT_COUNTERPARTIES
        If Len(pars(1)) > 4 Then
            ShowWarning "Unable to save data" & vbCrLf & "The counterparty name should be no longer than 4 letters"
            Exit Function
        End If
        Ret = gDBW.usp_TntCounterParty_Save(pars(0), pars(1), pars(2), True)
    End Select
    
    ' setting id for a new tntity record
    With grdEntity
        If m_State = STATE_NEW Then
            .TextMatrix(.Row, 0) = Ret
            .Rows = .Rows + 1
            .RowHidden(.Rows - 1) = True
        End If
    End With
    
    SetState STATE_NONE
    
    ' reloading the list
    EntityReload
    
    EntitySave = Ret
    
Exit Function
EH:
    If Err.Number = &H80040E2F Then
        ShowWarning "Unable to save data" & vbCrLf & "Entity name is not unique"
    Else
        'gCmn.ErrorMsgBox "Unable to save data" & vbCrLf & Err.Description
        ShowError
    End If
    EntitySave = False
End Function

' temporary data reloading
Private Sub EntityReload()
Dim iCurRow As Long
Dim iTopRow As Long
On Error Resume Next
    With grdEntity
        'getting current position
        iCurRow = .Row
        iTopRow = .TopRow
        ' loading data
        EntityLoad
        ' setting current position
        .TopRow = iTopRow
        .Row = iCurRow
    End With
End Sub


' deleting entity row
Private Function EntityDelete() As Boolean
    Dim iID As Long
    Dim Ret As Boolean, nRet&
    On Error GoTo EH

    If gCmn.MyMsgBox("Do you wish to delete current record?", vbYesNo + vbQuestion) = vbNo Then
        Exit Function
    End If
        
    With grdEntity
        ' reading id
        iID = gCmn.ReadLng(.TextMatrix(.Row, 0))
        
        ' if new row then undoing chsnges
        If iID = 0 Then
            EntityUndoEdit
            Exit Function
        End If
    End With
    

    ' deleting item
    Select Case m_EntityType
        
    Case ENT_BROKERS
        Ret = gDBW.usp_Broker_Del(iID, 0)
        If Not Ret Then ShowWarning "Cannot delete broker." & vbCrLf & "There are trades linked to this broker."
    Case ENT_EXCHANGES
        Ret = gDBW.usp_Exchange_Del(iID)
        Call ChangeExchanges
    Case ENT_GROUPS
        Ret = gDBW.usp_ContractGroup_Del(iID)
    Case ENT_HOLYDAYS
        Ret = gDBW.usp_Holiday_Del(iID)
        Call ReloadHolidays
    Case ENT_STRATEGIES
        nRet = gDBW.usp_BoStrategy_Del(iID)
        Select Case nRet
            Case -1
                ShowWarning "Cannot delete strategy." & vbCrLf & "There are trade(s) linked to this strategy."
            Case -2
                ShowWarning "Cannot delete strategy." & vbCrLf & "There are trader(s) with equals default strategy."
            Case -3
                ShowWarning "Cannot delete strategy." & vbCrLf & "There are trader(s) rule(s) linked to this strategy."
        End Select
    Case ENT_TRADERS
        nRet = gDBW.usp_BoTrader_Del(iID)
        Select Case nRet
            Case -1
                ShowWarning "Cannot delete trader." & vbCrLf & "There are trade(s) linked to this trader."
            Case -2
                ShowWarning "Cannot delete trader." & vbCrLf & "There are order(s) linked to this trader."
            Case -3
                ShowWarning "Cannot delete trader." & vbCrLf & "There are trade message(s) linked to this trader."
        End Select
    Case ENT_IRCURVES
        Ret = gDBW.usp_IRPoint_Del(iID)
    Case ENT_TRADERGROUPS
        Ret = gDBW.usp_BoTraderGroup_Del(iID)
    Case ENT_ORDDESTIN
        Ret = gDBW.usp_OrderExecDest_Del(iID, 0)
        If Not Ret Then ShowWarning "Cannot delete order destination." & vbCrLf & "There are orders linked to this destination."
    Case ENT_COUNTERPARTIES
        nRet = gDBW.usp_TntCounterParty_Del(iID, 0)
        If nRet <> 0 Then ShowWarning "Cannot delete counterparty." & vbCrLf & "There are trade messages(s) linked to this counterparty."
    End Select

    EntityReload

    SetState STATE_NONE
    
    EntityDelete = Ret
Exit Function
EH:
    Dim Msg As String
    If Err.Number = &H80040E2F Then
        On Error Resume Next
        ShowWarning "This item can not be deleted." & vbCrLf & "Other items may depend on it."
    Else
        Msg = "Unable to delete data" & vbCrLf & _
            Err.Description
        gCmn.ErrorMsgBox Msg
    End If
    EntityDelete = False
End Function

' setting edit mode for a grid
Private Function EntityEdit() As Boolean
On Error GoTo EH
    With grdEntity
        If .Row = .Rows - 2 Then
            SetState STATE_NEW
            .RowHidden(.Rows - 1) = False
        Else
            SetState STATE_EDIT
        End If
    End With
    EntityEdit = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Error occured when trying to enter in edit mode" & vbCrLf & Err.Description
End Function

' undoing changes
Private Function EntityUndoEdit() As Boolean
Dim iID As Long
Dim i As Long
On Error GoTo EH
    
    With grdEntity
        Select Case m_State
        Case STATE_EDIT
            ' if in edit mode then reloading current row
            iID = gCmn.ReadLng(.TextMatrix(.Row, 0))
            m_rsEntity.Filter = m_arrEntDef(m_EntityType).Col(0).Name & "=" & iID
            If m_rsEntity.RecordCount > 0 Then
                If Not m_rsEntity.BOF Then m_rsEntity.MoveFirst
                FillRow .Row
                SetState STATE_NONE
            Else
            End If
        Case STATE_NEW
            ' if in new mode then deleting current row
            For i = 0 To m_arrEntDef(m_EntityType).Cols - 1
                If Not m_arrEntDef(m_EntityType).Col(i).DisableEdit Then
                    .Cell(flexcpText, .Rows - 2, i, .Rows - 1, i) = STR_NEWVALUE
                End If
            Next
            .RowHidden(.Rows - 1) = True
            SetState STATE_NONE
        End Select
    End With
    EntityUndoEdit = True
Exit Function
EH:
    gCmn.ErrorMsgBox "Error occured when trying to undo edit mode" & vbCrLf & Err.Description
End Function

'**************************************************
'**************************************************
'                other functions
'**************************************************
'**************************************************

' setting control state
Private Sub SetState(ByVal X As eState)
On Error Resume Next

    m_State = X
    If (m_State = STATE_LOAD) Then
        Screen.MousePointer = vbHourglass
    Else
        Screen.MousePointer = vbDefault
    End If

    ColorCellsAll ' grdEntity.Row
End Sub


' grid initialization
Private Sub GridInit()
    On Error Resume Next
    Dim i As Long
    
    With grdEntity
        .Rows = 1
        .Cols = MINT_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoResize = False
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = False
        .DataMode = flexDMFree
        .Editable = flexEDKbdMouse
        .Ellipsis = flexNoEllipsis
        .ExplorerBar = flexExNone
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusLight  'flexFocusRaised
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .GridLineWidth = 1
        .MergeCells = flexMergeFree
        .MergeCompare = flexMCExact
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollBars = flexScrollBarBoth
        .ScrollTips = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabCells
        Set .WallPaper = Nothing
        .WallPaperAlignment = flexPicAlignLeftTop
        .WordWrap = False
        
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
    End With
End Sub


' initializing column data
Private Sub EntDefInit()
Dim i As Long
Const INT_IDVISIBLE = False
    On Error Resume Next
    'ENT_EXCHANGES
    With m_arrEntDef(ENT_EXCHANGES)
        .Cols = 8
        i = 0
        .Col(i).Name = "iExchangeID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcExchangeCode"
        .Col(i).Type = vbString
        .Col(i).Caption = "Code"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 10
        .Col(i).DataBinding = True
        i = 2
        .Col(i).Name = "vcOccCode"
        .Col(i).Type = vbString
        .Col(i).Caption = "OCC Code"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 10
        .Col(i).DataBinding = True
        .Col(i).EditMaxLen = 1
        i = 3
        .Col(i).Name = "vcExchangeName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 4
        .Col(i).Name = "vcDescription"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 5
        .Col(i).Name = "tiIsUnderlying"
        .Col(i).Type = vbBoolean
        .Col(i).Caption = "Underlying"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 6
        .Col(i).Name = "tiIsOption"
        .Col(i).Type = vbBoolean
        .Col(i).Caption = "Option"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 7
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).HaveProperties = True
        
    End With
    
    'ENT_BROKERS
    With m_arrEntDef(ENT_BROKERS)
        .Cols = 9
        i = 0
        .Col(i).Name = "iBrokerID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "tiIsClearing"
        .Col(i).Type = vbBoolean
        .Col(i).Caption = "Clearing"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 2
        .Col(i).Name = "vcBrokerName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 3
        .Col(i).Name = "vcDescription"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 4
        .Col(i).Name = "fUndComm"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Stock Commission, $"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,###,##0.###0$"
        i = 5
        .Col(i).Name = "fOptRegComm"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Reg. Opt. Comm., $"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,###,##0.###0$"
        i = 6
        .Col(i).Name = "fLowPremComm"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Low Prem. Comm., $"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,###,##0.###0$"
        i = 7
        .Col(i).Name = "fOptSpreadComm"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Spread Opt. Comm., $"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,###,##0.###0$"
        i = 8
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
    End With

    'ENT_GROUPS
    With m_arrEntDef(ENT_GROUPS)
        .Cols = 4
        i = 0
        .Col(i).Name = "iGroupID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcGroupName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 2
        .Col(i).Name = "vcDescription"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 3
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).HaveProperties = True
    End With

    'ENT_HOLYDAYS
    With m_arrEntDef(ENT_HOLYDAYS)
        .Cols = 4
        i = 0
        .Col(i).Name = "iHolidayID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "dtHolidayDate"
        .Col(i).Type = vbDate
        .Col(i).Caption = "Date"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 2
        .Col(i).Name = "vcHolidayName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 3
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
    End With

    'ENT_STRATEGIES
    With m_arrEntDef(ENT_STRATEGIES)
        .Cols = 4
        i = 0
        .Col(i).Name = "iStrategyID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcStrategyName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 2
        .Col(i).Name = "vcDescription"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 3
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).HaveProperties = True
    End With

    'ENT_TRADERS
    With m_arrEntDef(ENT_TRADERS)
        .Cols = 7
        i = 0
        .Col(i).Name = "iTraderID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcAcronym"
        .Col(i).Type = vbString
        .Col(i).Caption = "Acronym"
        .Col(i).Visible = True
        .Col(i).ColWidth = 2000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 2
        .Col(i).Name = "vcTraderName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 3
        .Col(i).Name = "vcClearingTraderCode"
        .Col(i).Type = vbString
        .Col(i).Caption = "Clearing Trader Code"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 4
        .Col(i).Name = "iTraderGroupID"
        .Col(i).Type = vbLong
        .Col(i).Caption = "Trader Group"
        .Col(i).Visible = False
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 5
        .Col(i).Name = "vcGroupName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Trader Group"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).DisableEdit = True
        
        i = 6
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).HaveProperties = True
    
    End With

    'ENT_IRCURVES
    With m_arrEntDef(ENT_IRCURVES)
        .Cols = 12
        i = 0
        .Col(i).Name = "iIRpointID"
        .Col(i).Type = vbLong
        .Col(i).Visible = False
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        
        i = 1
        .Col(i).Name = "iCurveID"
        .Col(i).Type = vbLong
        .Col(i).Visible = False
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        
        i = 2
        .Col(i).Name = "Period"
        .Col(i).Type = vbString
        .Col(i).Caption = "Period"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1500
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = False
        
        i = 3
        .Col(i).Name = "iPeriodTypeID"
        .Col(i).Type = vbLong
        .Col(i).Visible = False
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).DisableEdit = True
        
        i = 4
        .Col(i).Name = "iNum"
        .Col(i).Type = vbLong
        .Col(i).Caption = "Num"
        .Col(i).Visible = False
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).DisableEdit = True
        
        i = 5
        .Col(i).Name = "Expire Date"
        .Col(i).Type = vbDate
        .Col(i).Caption = "Expire Date"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1500
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        
        i = 6
        .Col(i).Name = "fShortRate"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Short Rate, %"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,##0.#0%"
        
        i = 7
        .Col(i).Name = "fLongRate"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Long Rate, %"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,##0.#0%"
        
        i = 8
        .Col(i).Name = "NeutralRate"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Neutral Rate, %"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True 'False
        '.Col(I).DisableEdit = True
        .Col(i).FormatString = "#,##0.#0%"
        
        i = 9
        .Col(i).Name = "fHTBRate"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "HTB Rate, %"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).FormatString = "#,##0.#0%"
        
        i = 10
        .Col(i).Name = "NeutralHTBRate"
        .Col(i).Type = vbDouble
        .Col(i).Caption = "Neutral Rate for HTB, %"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).FormatString = "#,##0.#0%"
        
        i = 11
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
    End With
    
    'ENT_TRADERGROUPS
    With m_arrEntDef(ENT_TRADERGROUPS)
        .Cols = 4
        i = 0
        .Col(i).Name = "iTraderGroupID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcGroupName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 2
        .Col(i).Name = "vcGroupDesc"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 6000
        .Col(i).EditWidth = 4000
        .Col(i).DataBinding = True
        
        i = 3
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
        .Col(i).HaveProperties = True
    
    End With
    
    'ENT_ORDDESTIN
    With m_arrEntDef(ENT_ORDDESTIN)
        .Cols = 6
        i = 0
        .Col(i).Name = "iDestID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcCode"
        .Col(i).Type = vbString
        .Col(i).Caption = "Code"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 10
        .Col(i).DataBinding = True
        .Col(i).EditMaxLen = 50
        i = 2
        .Col(i).Name = "vcName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        .Col(i).EditMaxLen = 255
        i = 3
        .Col(i).Name = "tiIsOption"
        .Col(i).Type = vbBoolean
        .Col(i).Caption = "IsOption"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        i = 4
        .Col(i).Name = "tiIsStock"
        .Col(i).Type = vbBoolean
        .Col(i).Caption = "IsUnderlying"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 1000
        .Col(i).DataBinding = True
        
        i = 5
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
    End With
    
    'ENT_COUNTERPARTIES
    With m_arrEntDef(ENT_COUNTERPARTIES)
        .Cols = 4
        i = 0
        .Col(i).Name = "iCptID"
        .Col(i).Type = vbLong
        .Col(i).Visible = INT_IDVISIBLE
        .Col(i).ColWidth = 500
        .Col(i).DataBinding = True
        i = 1
        .Col(i).Name = "vcName"
        .Col(i).Type = vbString
        .Col(i).Caption = "Name"
        .Col(i).Visible = True
        .Col(i).ColWidth = 1000
        .Col(i).EditWidth = 4
        .Col(i).DataBinding = True
        .Col(i).EditMaxLen = 4
        i = 2
        .Col(i).Name = "vcDesc"
        .Col(i).Type = vbString
        .Col(i).Caption = "Description"
        .Col(i).Visible = True
        .Col(i).ColWidth = 3000
        .Col(i).EditWidth = 255
        .Col(i).DataBinding = True
        .Col(i).EditMaxLen = 255
        i = 3
        .Col(i).Name = "LastCol"
        .Col(i).Type = vbString
        .Col(i).Caption = ""
        .Col(i).Visible = True
        .Col(i).ColWidth = 120
        .Col(i).EditWidth = 120
        .Col(i).DataBinding = False
        .Col(i).DisableEdit = True
    End With
    
End Sub

' coloring all the cells
Private Sub ColorCellsAll()
Dim iRow As Long
On Error Resume Next
    For iRow = 1 To grdEntity.Rows - 1
        ColorCells iRow
    Next iRow
End Sub

' coloring cells of a row
Private Sub ColorCells(Optional ByVal iRow As Long = -1)
Dim iBackColorSel As Long
Dim iForeColorSel As Long
Dim iForeColor As Long
On Error Resume Next
    With grdEntity
        
        If iRow = .Row Then
            Select Case m_State
            Case STATE_EDIT, STATE_NEW
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                iForeColorSel = GCOLOR_TABLE_CHANGED_FORE
                iForeColor = GCOLOR_TABLE_WHITE_FORE
            Case Else
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                If iRow >= .Rows - 2 Then
                    iForeColorSel = GCOLOR_TABLE_DELETED_FORE
                    iForeColor = GCOLOR_TABLE_DELETED_FORE
                Else
                    iForeColorSel = GCOLOR_TABLE_WHITE_FORE
                    iForeColor = GCOLOR_TABLE_WHITE_FORE
                End If
            End Select
        Else
            If iRow >= .Rows - 2 Then
                iForeColor = GCOLOR_TABLE_DELETED_FORE
            Else
                iForeColor = GCOLOR_TABLE_WHITE_FORE
            End If
        End If
        
        If iRow = .Row Then
            .BackColorSel = iBackColorSel
            .ForeColorSel = iForeColorSel
        End If
        .Cell(flexcpForeColor, iRow, 0, iRow, m_arrEntDef(m_EntityType).Cols - 1) = iForeColor
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' call property window for the current row
'
Private Function EntityProp() As Boolean
Dim iID As Long
Dim sName As String
    On Error GoTo EH
    ' reading id
    iID = gCmn.ReadLng(grdEntity.TextMatrix(grdEntity.Row, 0))
    If iID = 0 Then Exit Function
    Select Case m_EntityType
    
    ' for groups
    Case ENT_GROUPS
        sName = (grdEntity.TextMatrix(grdEntity.Row, 1))
        frmEntGroupProp.Execute iID, sName
            
    ' for exchanges
    Case ENT_EXCHANGES
        sName = (grdEntity.TextMatrix(grdEntity.Row, 1))
        frmEntExchangeProp.Execute iID, sName
    
    ' for trader stocks
    Case ENT_TRADERS
        sName = (grdEntity.TextMatrix(grdEntity.Row, 2))
        frmEntTraderProp.Execute iID, sName
        EntityReload
       
    Case ENT_TRADERGROUPS
        sName = (grdEntity.TextMatrix(grdEntity.Row, 1))
        frmEntTraderGroupProp.Execute iID, sName
    
    Case ENT_STRATEGIES
        sName = (grdEntity.TextMatrix(grdEntity.Row, 1))
        frmEntStrategyProp.Execute iID, sName
                
    End Select
        
    Exit Function
EH:
    gCmn.ErrorMsgBox "Unable to load property form" & vbCrLf & Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' call property window for the current row
'
Private Sub ChangeBookPriority(ByVal Change As Integer)
Dim iID As Long
Dim iID2 As Long
Dim sName As String
Dim s As String
    On Error GoTo Herr
    With grdEntity
        ' reading id
        If Not (.Row >= 1 And .Row <= .Rows - 3) Then
            Exit Sub
        End If
        s = .TextMatrix(.Row, 0)
        If Not IsNumeric(s) Then Exit Sub
        iID = Val(s)
        If iID = 0 Then Exit Sub
        If Change = -1 Then
            'Move Up
            If .Row > 1 And .Row <= .Rows - 3 Then
                s = .TextMatrix(.Row - 1, 0)
                If Not IsNumeric(s) Then Exit Sub
                iID2 = Val(s)
                If iID2 = 0 Then Exit Sub
                gDBW.usp_BookSwap_Save iID, iID2
                .Row = .Row - 1
                ' reload
                ' set current row
                EntityReload
            End If
        ElseIf Change = 1 Then
            'Move Down
            If .Row >= 1 And .Row < .Rows - 3 Then
                s = .TextMatrix(.Row + 1, 0)
                If Not IsNumeric(s) Then Exit Sub
                iID2 = Val(s)
                If iID2 = 0 Then Exit Sub
                gDBW.usp_BookSwap_Save iID, iID2
                .Row = .Row + 1
                ' reload
                ' set current row
                EntityReload
            End If
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' After Import positios refresh to show new traders
'
Public Sub RefreshTraders()
    If m_EntityType = ENT_TRADERS Then
        SetEntityType ENT_TRADERS
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnRecalc_Click()
    On Error GoTo Herr
'    Screen.MousePointer = 11
    btnRecalc.Enabled = False
    m_nMaturity = CLng(txtMaturity.Text)
    If m_nMaturity = 0 Then
     m_nMaturity = g_WtdVega.Maturity
     txtMaturity.Text = CStr(m_nMaturity)
    Else
    SetMaturity m_nMaturity
    End If
    btnRecalc.Enabled = True
'    Screen.MousePointer = 0
    Exit Sub
Herr:
    btnRecalc.Enabled = True
'    Screen.MousePointer = 0
    ShowError
End Sub



Private Function GetCommissionLowPremiumLimit() As Double
    Dim rs As ADODB.Recordset
    On Error Resume Next
    GetCommissionLowPremiumLimit = 0#
    Set rs = gDBW.usp_CommissionLowPremiumLimit_Get
    If Not rs.EOF Then
        GetCommissionLowPremiumLimit = rs!fCommissionLowPremiumLimit
    End If
End Function
