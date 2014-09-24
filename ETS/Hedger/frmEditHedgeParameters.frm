VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E395ECF2-0B93-464D-AD1B-99A443BDF17F}#1.0#0"; "FocusFlatControls1.ocx"
Begin VB.Form frmEditHedgeParameters 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Hedge Parameters"
   ClientHeight    =   3360
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5760
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmEditHedgeParameters.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3360
   ScaleWidth      =   5760
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin FocusFlatControls1.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   315
      Left            =   4440
      TabIndex        =   7
      Top             =   2760
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   " &Cancel"
   End
   Begin FocusFlatControls1.FlatButton btnOK 
      Default         =   -1  'True
      Height          =   315
      Left            =   3240
      TabIndex        =   6
      Top             =   2760
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   " &OK"
   End
   Begin FocusFlatControls1.FlatCombo cmbTerm 
      Height          =   315
      Left            =   1680
      TabIndex        =   1
      Top             =   1080
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Text            =   ""
      Style           =   2
   End
   Begin VB.CheckBox chkIndexSelected 
      Appearance      =   0  'Flat
      Caption         =   "DJX"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   4080
      TabIndex        =   5
      Top             =   2040
      Value           =   1  'Checked
      Width           =   735
   End
   Begin VB.CheckBox chkIndexSelected 
      Appearance      =   0  'Flat
      Caption         =   "OEX"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   4080
      TabIndex        =   4
      Top             =   1560
      Value           =   1  'Checked
      Width           =   735
   End
   Begin VB.CheckBox chkIndexSelected 
      Appearance      =   0  'Flat
      Caption         =   "NDX"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   4080
      TabIndex        =   3
      Top             =   1080
      Value           =   1  'Checked
      Width           =   735
   End
   Begin VB.CheckBox chkIndexSelected 
      Appearance      =   0  'Flat
      Caption         =   "SPX"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   4080
      TabIndex        =   2
      Top             =   600
      Value           =   1  'Checked
      Width           =   855
   End
   Begin VB.TextBox txtGainBarrier 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   1680
      TabIndex        =   0
      Top             =   600
      Width           =   1095
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      Caption         =   "Price source"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   600
      TabIndex        =   14
      Top             =   1920
      Width           =   1335
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape2 
      BorderColor     =   &H80000010&
      Height          =   1095
      Left            =   120
      Top             =   2040
      Width           =   2895
   End
   Begin MSForms.OptionButton optFeedPrices 
      Height          =   375
      Left            =   480
      TabIndex        =   13
      Top             =   2520
      Width           =   1455
      VariousPropertyBits=   746588177
      ForeColor       =   -2147483630
      DisplayStyle    =   5
      Size            =   "2566;661"
      Value           =   "0"
      Caption         =   "Realtime feed"
      SpecialEffect   =   0
      GroupName       =   "PnLCalculation"
      FontEffects     =   1073750016
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.OptionButton optDBPrices 
      Height          =   375
      Left            =   480
      TabIndex        =   12
      Top             =   2160
      Width           =   1455
      VariousPropertyBits=   746588179
      ForeColor       =   -2147483630
      DisplayStyle    =   5
      Size            =   "2566;661"
      Value           =   "1"
      Caption         =   "Database"
      SpecialEffect   =   0
      GroupName       =   "PnLCalculation"
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Image imgHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   2
      Left            =   4965
      MouseIcon       =   "frmEditHedgeParameters.frx":000C
      MousePointer    =   99  'Custom
      Picture         =   "frmEditHedgeParameters.frx":015E
      ToolTipText     =   "Open Help"
      Top             =   120
      Width           =   150
   End
   Begin VB.Image imgHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   1
      Left            =   2820
      MouseIcon       =   "frmEditHedgeParameters.frx":01BF
      MousePointer    =   99  'Custom
      Picture         =   "frmEditHedgeParameters.frx":0311
      ToolTipText     =   "Open Help"
      Top             =   1080
      Width           =   150
   End
   Begin VB.Image imgHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   0
      Left            =   2820
      MouseIcon       =   "frmEditHedgeParameters.frx":0372
      MousePointer    =   99  'Custom
      Picture         =   "frmEditHedgeParameters.frx":04C4
      ToolTipText     =   "Open Help"
      Top             =   600
      Width           =   150
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Caption         =   "Hedge parameters"
      Height          =   255
      Index           =   1
      Left            =   840
      TabIndex        =   11
      Top             =   120
      Width           =   1455
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H80000010&
      Height          =   1455
      Index           =   1
      Left            =   120
      Top             =   240
      Width           =   2895
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Term, days :"
      Height          =   255
      Index           =   0
      Left            =   240
      TabIndex        =   10
      Top             =   1080
      Width           =   1335
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      Caption         =   "Hedging indexes"
      Height          =   255
      Index           =   0
      Left            =   3600
      TabIndex        =   9
      Top             =   120
      Width           =   1455
      WordWrap        =   -1  'True
   End
   Begin VB.Shape Shape1 
      BorderColor     =   &H80000010&
      Height          =   2295
      Index           =   0
      Left            =   3240
      Top             =   240
      Width           =   2295
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Desired Gain, % :"
      Height          =   255
      Index           =   1
      Left            =   240
      TabIndex        =   8
      Top             =   600
      Width           =   1335
   End
End
Attribute VB_Name = "frmEditHedgeParameters"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Private m_mbResult As VbMsgBoxResult
Private bChanged As Boolean
Private Editable As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As VbMsgBoxResult
    On Error Resume Next
    Load Me
    ShowHelpTags
    LoadParameters
    Editable = True
    bChanged = False
    m_mbResult = vbCancel
    Me.Show vbModal, frmMain
    Execute = m_mbResult
    Unload Me
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    m_mbResult = vbCancel
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    SaveParameters
    If bChanged Then
        m_mbResult = vbOK
    End If
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Initialize()
    On Error Resume Next
    Set PortfolioHedge = gPortfolioHedge
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    m_mbResult = vbCancel
    With New clsCommon
        .CustomizeForm Me
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadParameters()
Dim S As String
    On Error GoTo EH
    Editable = False
    txtGainBarrier.Text = PortfolioHedge.g_dGainBarrier * 100
    Me.chkIndexSelected(0).Value = IIf(PortfolioHedge.g_SelectSPX, 1, 0)
    Me.chkIndexSelected(1).Value = IIf(PortfolioHedge.g_SelectNDX, 1, 0)
    Me.chkIndexSelected(2).Value = IIf(PortfolioHedge.g_SelectOEX, 1, 0)
    Me.chkIndexSelected(3).Value = IIf(PortfolioHedge.g_SelectDJX, 1, 0)
    
    cmbTerm.Clear
    If Not PortfolioHedge.rsTerms Is Nothing Then
        With PortfolioHedge.rsTerms
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If !Supported Then
                        If Not IsNull(!Term) Then
                            S = Format$(!Term)
                            cmbTerm.AddItem S
                        End If
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    S = PortfolioHedge.HVTerm
    If cmbTerm.ListCount = 0 Then
        cmbTerm.AddItem S
    End If
    cmbTerm.Text = S
    
    optDBPrices.Value = True 'Not g_ReportPricesFeed
    optFeedPrices.Value = False 'g_ReportPricesFeed
    
    AdjustControlsAppearence
    Editable = True
    Exit Sub
EH:
    ShowError "frmEditHedgeParameters_LoadParameters failed."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveParameters()
Dim dValue As Double
Dim bValue As Boolean
Dim lValue As Long
    On Error Resume Next
    
    bChanged = False
    
    lValue = Val(Me.cmbTerm.Text)
    If PortfolioHedge.HVTerm <> lValue Then
        PortfolioHedge.HVTerm = lValue
        bChanged = True
    End If
    
    dValue = 0
    dValue = CDbl(txtGainBarrier.Text)
    If dValue <= 0 Then dValue = Val(txtGainBarrier.Text)
    dValue = dValue / 100
    If PortfolioHedge.g_dGainBarrier <> dValue Then
        PortfolioHedge.g_dGainBarrier = dValue
        bChanged = True
    End If
    
    bValue = Me.chkIndexSelected(0).Value <> 0
    If PortfolioHedge.g_SelectSPX <> bValue Then
        PortfolioHedge.g_SelectSPX = bValue
        bChanged = True
    End If
    
    bValue = Me.chkIndexSelected(1).Value <> 0
    If PortfolioHedge.g_SelectNDX <> bValue Then
        PortfolioHedge.g_SelectNDX = bValue
        bChanged = True
    End If
    
    bValue = Me.chkIndexSelected(2).Value <> 0
    If PortfolioHedge.g_SelectOEX <> bValue Then
        PortfolioHedge.g_SelectOEX = bValue
        bChanged = True
    End If
    
    bValue = Me.chkIndexSelected(3).Value <> 0
    If PortfolioHedge.g_SelectDJX <> bValue Then
        PortfolioHedge.g_SelectDJX = bValue
        bChanged = True
    End If
    
    With PortfolioHedge
        .SetIndexSelected "SPX", .g_SelectSPX
        .SetIndexSelected "NDX", .g_SelectNDX
        .SetIndexSelected "OEX", .g_SelectOEX
        .SetIndexSelected "DJX", .g_SelectDJX
    End With

    g_ReportPricesFeed = False 'optFeedPrices.Value
    
    Exit Sub
EH:
    ShowError "frmEditHedgeParameters_SaveParameters failed."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AdjustControlsAppearence()
    On Error Resume Next
    Dim dValue As Double, lValue As Long
    dValue = 0
    dValue = CDbl(cmbTerm.Text)
    If dValue <= 0 Then dValue = Val(cmbTerm.Text)
    If dValue <= 0 Then
        btnOK.Enabled = False
        btnOK.Default = False
        Exit Sub
    End If
    
    dValue = 0
    dValue = CDbl(txtGainBarrier.Text)
    If dValue <= 0 Then dValue = Val(txtGainBarrier.Text)
    If dValue <= 0 Then
        btnOK.Enabled = False
        btnOK.Default = False
        Exit Sub
    End If
    
    btnOK.Enabled = True
    btnOK.Default = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtGainBarrier_Change()
    On Error Resume Next
    If Editable Then
        ValidatePercent txtGainBarrier
        AdjustControlsAppearence
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbTerm_Change()
    On Error Resume Next
    If Editable Then
        AdjustControlsAppearence
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub chkPerExpiration_Click()
    On Error Resume Next
    If Editable Then
        AdjustControlsAppearence
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ShowHelpTipsChanged()
    On Error Resume Next
    ShowHelpTags
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpTags()
    On Error Resume Next
    Dim i As Long
    For i = imgHelp.LBound To imgHelp.UBound
        imgHelp(i).Visible = g_bShowHelpTags
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub imgHelp_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case 0
        ShowHelpID 501, Me
    Case 1
        ShowHelpID 502, Me
    Case 2
        ShowHelpID 503, Me
    Case Else
        Debug.Assert False
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtGainBarrier_GotFocus()
    On Error Resume Next
    With txtGainBarrier
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub

