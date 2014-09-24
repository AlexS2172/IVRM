VERSION 5.00
Begin VB.Form frmMMParameters 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Market Marker parameters"
   ClientHeight    =   5280
   ClientLeft      =   2850
   ClientTop       =   3600
   ClientWidth     =   10305
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5280
   ScaleWidth      =   10305
   ShowInTaskbar   =   0   'False
   Begin VB.CommandButton cmdApplyCmm 
      Caption         =   "Apply for CMM"
      Height          =   375
      Left            =   3960
      TabIndex        =   63
      Top             =   4800
      Width           =   1815
   End
   Begin VB.CommandButton cmdApplyPmm 
      Caption         =   "Apply for PMM"
      Height          =   375
      Left            =   1920
      TabIndex        =   61
      Top             =   4800
      Width           =   1815
   End
   Begin VB.TextBox edDOS 
      Height          =   285
      Index           =   1
      Left            =   7440
      TabIndex        =   38
      Text            =   "5"
      Top             =   1080
      Width           =   975
   End
   Begin VB.Frame Frame3 
      Caption         =   "Ticks"
      Height          =   3975
      Left            =   5040
      TabIndex        =   25
      Top             =   120
      Width           =   5175
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   2
         Left            =   1200
         TabIndex        =   60
         Text            =   "30"
         Top             =   1680
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   7
         Left            =   3600
         TabIndex        =   59
         Text            =   "5"
         Top             =   3480
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   7
         Left            =   2400
         TabIndex        =   58
         Text            =   "5"
         Top             =   3480
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   6
         Left            =   1200
         TabIndex        =   57
         Text            =   "80"
         Top             =   3120
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   6
         Left            =   3600
         TabIndex        =   56
         Text            =   "5"
         Top             =   3120
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   6
         Left            =   2400
         TabIndex        =   55
         Text            =   "5"
         Top             =   3120
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   5
         Left            =   1200
         TabIndex        =   54
         Text            =   "70"
         Top             =   2760
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   5
         Left            =   3600
         TabIndex        =   53
         Text            =   "5"
         Top             =   2760
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   5
         Left            =   2400
         TabIndex        =   52
         Text            =   "5"
         Top             =   2760
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   7
         Left            =   1200
         TabIndex        =   51
         Text            =   "60"
         Top             =   3480
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   4
         Left            =   3600
         TabIndex        =   50
         Text            =   "5"
         Top             =   2400
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   4
         Left            =   2400
         TabIndex        =   49
         Text            =   "5"
         Top             =   2400
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   4
         Left            =   1200
         TabIndex        =   48
         Text            =   "50"
         Top             =   2400
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   3
         Left            =   3600
         TabIndex        =   47
         Text            =   "5"
         Top             =   2040
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   3
         Left            =   2400
         TabIndex        =   46
         Text            =   "5"
         Top             =   2040
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   3
         Left            =   1200
         TabIndex        =   45
         Text            =   "40"
         Top             =   2040
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   2
         Left            =   3600
         TabIndex        =   44
         Text            =   "5"
         Top             =   1680
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   2
         Left            =   2400
         TabIndex        =   43
         Text            =   "5"
         Top             =   1680
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   1
         Left            =   3600
         TabIndex        =   42
         Text            =   "5"
         Top             =   1320
         Width           =   1095
      End
      Begin VB.TextBox edDOS 
         Height          =   285
         Index           =   0
         Left            =   2400
         TabIndex        =   41
         Text            =   "5"
         Top             =   1320
         Width           =   975
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   1
         Left            =   1200
         TabIndex        =   40
         Text            =   "20"
         Top             =   1320
         Width           =   975
      End
      Begin VB.TextBox edMAM 
         Height          =   285
         Index           =   0
         Left            =   3600
         TabIndex        =   39
         Text            =   "5"
         Top             =   960
         Width           =   1095
      End
      Begin VB.TextBox edTWV 
         Height          =   285
         Index           =   0
         Left            =   1200
         TabIndex        =   37
         Text            =   "10"
         Top             =   960
         Width           =   975
      End
      Begin VB.Label Label20 
         Caption         =   "8+ ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   36
         Top             =   3480
         Width           =   975
      End
      Begin VB.Label Label19 
         Caption         =   "7 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   35
         Top             =   3120
         Width           =   975
      End
      Begin VB.Label Label18 
         Caption         =   "6 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   34
         Top             =   2760
         Width           =   1095
      End
      Begin VB.Label Label17 
         Caption         =   "5 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   33
         Top             =   2400
         Width           =   975
      End
      Begin VB.Label Label16 
         Caption         =   "4 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   32
         Top             =   2040
         Width           =   975
      End
      Begin VB.Label Label15 
         Caption         =   "3 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   31
         Top             =   1680
         Width           =   975
      End
      Begin VB.Label Label14 
         Caption         =   "2 ticks away"
         Height          =   255
         Left            =   120
         TabIndex        =   30
         Top             =   1320
         Width           =   975
      End
      Begin VB.Label Label13 
         Caption         =   "1 tick away"
         Height          =   255
         Left            =   120
         TabIndex        =   29
         Top             =   960
         Width           =   975
      End
      Begin VB.Label Label12 
         Caption         =   "Match away market maximum size"
         Height          =   495
         Left            =   3600
         TabIndex        =   28
         Top             =   360
         Width           =   1455
      End
      Begin VB.Label Label11 
         Caption         =   "Deirved order maximum size"
         Height          =   495
         Left            =   2400
         TabIndex        =   27
         Top             =   360
         Width           =   1095
      End
      Begin VB.Label Label10 
         Caption         =   "Tick Worse Volume"
         Height          =   495
         Left            =   1200
         TabIndex        =   26
         Top             =   360
         Width           =   855
      End
   End
   Begin VB.TextBox edStepUpBuffer 
      Height          =   300
      Left            =   8445
      TabIndex        =   23
      Text            =   "20"
      Top             =   4200
      Width           =   1215
   End
   Begin VB.Frame Frame2 
      Caption         =   "Limitations"
      Height          =   1815
      Left            =   120
      TabIndex        =   11
      Top             =   2760
      Width           =   4815
      Begin VB.TextBox edFARMMFrac 
         Height          =   285
         Left            =   3360
         TabIndex        =   22
         Text            =   "50"
         Top             =   1320
         Width           =   1215
      End
      Begin VB.TextBox edFIRMFrac 
         Height          =   285
         Left            =   3360
         TabIndex        =   21
         Text            =   "50"
         Top             =   960
         Width           =   1215
      End
      Begin VB.TextBox edISEMMFrac 
         Height          =   285
         Left            =   3360
         TabIndex        =   20
         Text            =   "50"
         Top             =   600
         Width           =   1215
      End
      Begin VB.TextBox edFARMMAbs 
         Height          =   285
         Left            =   1560
         TabIndex        =   19
         Text            =   "5"
         Top             =   1320
         Width           =   1215
      End
      Begin VB.TextBox edFIRMAbs 
         Height          =   285
         Left            =   1560
         TabIndex        =   18
         Text            =   "5"
         Top             =   960
         Width           =   1215
      End
      Begin VB.TextBox edISEMMAbs 
         Height          =   285
         Left            =   1560
         TabIndex        =   17
         Text            =   "5"
         Top             =   600
         Width           =   1215
      End
      Begin VB.Label Label9 
         Caption         =   "FAR MM"
         Height          =   255
         Left            =   240
         TabIndex        =   16
         Top             =   1320
         Width           =   735
      End
      Begin VB.Label Label8 
         Caption         =   "FIRM"
         Height          =   255
         Left            =   240
         TabIndex        =   15
         Top             =   960
         Width           =   735
      End
      Begin VB.Label Label7 
         Caption         =   "ISE MM"
         Height          =   255
         Left            =   240
         TabIndex        =   14
         Top             =   600
         Width           =   1095
      End
      Begin VB.Label Label6 
         Caption         =   "Limit fraction, %"
         Height          =   255
         Left            =   3360
         TabIndex        =   13
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label Label5 
         Caption         =   "Limit absolute"
         Height          =   255
         Left            =   1560
         TabIndex        =   12
         Top             =   240
         Width           =   1335
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Option parameters"
      Height          =   2535
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   4815
      Begin VB.ComboBox cboStrikePriceGroup 
         Height          =   315
         Left            =   2400
         Style           =   2  'Dropdown List
         TabIndex        =   10
         Top             =   2040
         Width           =   2175
      End
      Begin VB.ComboBox cboExpirationGroup 
         Height          =   315
         Left            =   2400
         Style           =   2  'Dropdown List
         TabIndex        =   8
         Top             =   1560
         Width           =   2175
      End
      Begin VB.ComboBox cboInstrumentGroup 
         Height          =   315
         Left            =   2400
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   1080
         Width           =   2175
      End
      Begin VB.ComboBox cboUnderlying 
         Height          =   315
         Left            =   2400
         Style           =   2  'Dropdown List
         TabIndex        =   4
         Top             =   645
         Width           =   2175
      End
      Begin VB.OptionButton optOne 
         Caption         =   "For specific series"
         Height          =   375
         Index           =   1
         Left            =   2400
         TabIndex        =   3
         Top             =   240
         Width           =   1815
      End
      Begin VB.OptionButton optAll 
         Caption         =   "For all series in BIN"
         Height          =   375
         Index           =   0
         Left            =   240
         TabIndex        =   2
         Top             =   240
         Width           =   1815
      End
      Begin VB.Label Label21 
         Caption         =   "Underlying symbol"
         Height          =   255
         Left            =   960
         TabIndex        =   62
         Top             =   720
         Width           =   1455
      End
      Begin VB.Label Label4 
         Caption         =   "Strike price group"
         Height          =   255
         Left            =   960
         TabIndex        =   9
         Top             =   2040
         Width           =   1335
      End
      Begin VB.Label Label3 
         Caption         =   "Expiration group"
         Height          =   255
         Left            =   960
         TabIndex        =   7
         Top             =   1600
         Width           =   1335
      End
      Begin VB.Label Label2 
         Caption         =   "Instrument group"
         Height          =   255
         Left            =   960
         TabIndex        =   5
         Top             =   1160
         Width           =   1335
      End
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "Close"
      Height          =   375
      Left            =   6000
      TabIndex        =   0
      Top             =   4800
      Width           =   1815
   End
   Begin VB.Label Label1 
      Caption         =   "Step Up buffer after quotes regeneration "
      Height          =   255
      Left            =   5400
      TabIndex        =   24
      Top             =   4230
      Width           =   2895
   End
End
Attribute VB_Name = "frmMMParameters"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Sub cmdApply_Click(bPmm As Boolean)
On Error GoTo ErrHandler
    Dim nTick As Integer
    
    If optAll.Item(0).Value = True Then
        frmMain.objMMParams.UnderlyingSymbol = ""
        frmMain.objMMParams.InstrumentGroup = enIGAll
        frmMain.objMMParams.StrikePriceGroup = enSPGAll
        frmMain.objMMParams.ExpirationGroup = enEGAll
    Else
        frmMain.objMMParams.UnderlyingSymbol = cboUnderlying.Text
        Select Case cboInstrumentGroup.Text
            Case "All"
                frmMain.objMMParams.InstrumentGroup = enIGAll
            Case "European Call"
                frmMain.objMMParams.InstrumentGroup = enIGEuropeanCallOption
            Case "European Put"
                frmMain.objMMParams.InstrumentGroup = enIGEuropeanPutOption
            Case "American Call"
                frmMain.objMMParams.InstrumentGroup = enIGAmericanCallOption
            Case "American Put"
                frmMain.objMMParams.InstrumentGroup = enIGAmericanPutOption
        End Select
        Select Case cboExpirationGroup.Text
            Case "All"
                frmMain.objMMParams.ExpirationGroup = enEGAll
            Case "Near"
                frmMain.objMMParams.ExpirationGroup = enEGNear
            Case "< 9 months"
                frmMain.objMMParams.ExpirationGroup = enEGLess9Month
            Case "LEAPS"
                frmMain.objMMParams.ExpirationGroup = enEGLEAPS
        End Select
        Select Case cboStrikePriceGroup.Text
            Case "All"
                frmMain.objMMParams.StrikePriceGroup = enSPGAll
            Case "Deep In The Money"
                frmMain.objMMParams.StrikePriceGroup = enSPGDeepInTheMoney
            Case "In The Money"
                frmMain.objMMParams.StrikePriceGroup = enSPGInTheMoney
            Case "At The Money"
                frmMain.objMMParams.StrikePriceGroup = enSPGAtTheMoney
            Case "Out The Money"
                frmMain.objMMParams.StrikePriceGroup = enSPGOutOfTheMoney
            Case "Deep Out The Money"
                frmMain.objMMParams.StrikePriceGroup = enSPGOutOfTheMoney
        End Select
    End If
    
    frmMain.objMMParams.IseMMTradeLimitAbsolute = edISEMMAbs.Text
    frmMain.objMMParams.FarMMTradeLimitAbsolute = edFARMMAbs.Text
    frmMain.objMMParams.FirmTradeLimitAbsolute = edFIRMAbs.Text
    frmMain.objMMParams.IseMmTradeLimitFraction = edISEMMFrac.Text
    frmMain.objMMParams.FarMmTradeLimitFraction = edFARMMFrac.Text
    frmMain.objMMParams.FirmTradeLimitFraction = edFIRMFrac.Text
    frmMain.objMMParams.StepUpAfterRegenBuffer = edStepUpBuffer.Text
    For nTick = 0 To 7
        frmMain.objMMParams.TickWorseVolume(nTick + 1) = edTWV(nTick).Text
        frmMain.objMMParams.DerivedOrderMaxSize(nTick + 1) = edDOS(nTick).Text
        frmMain.objMMParams.MatchAwayMarketMaxSize(nTick + 1) = edMAM(nTick).Text
    Next
    
    If bPmm Then
        frmMain.objMMManager.SetPmmParameters frmMain.objMMParams
    Else
        frmMain.objMMManager.SetCmmParameters frmMain.objMMParams
    End If
    
    Exit Sub

ErrHandler:
    frmError.labText = "Failed to set market maker parameters : " & Error
    frmError.Show vbModal
End Sub

Private Sub cmdApplyCmm_Click()
    cmdApply_Click False
End Sub

Private Sub cmdApplyPmm_Click()
    cmdApply_Click True
End Sub

Private Sub cmdClose_Click()
    Unload Me
End Sub

Private Sub OnAllUnderlyings()
    optAll.Item(0).Value = True
    cboUnderlying.Enabled = False
    cboInstrumentGroup.Enabled = False
    cboExpirationGroup.Enabled = False
    cboStrikePriceGroup.Enabled = False
End Sub

Private Sub OnOneUnderlying()
    optOne.Item(1).Value = True
    cboUnderlying.Enabled = True
    cboInstrumentGroup.Enabled = True
    cboExpirationGroup.Enabled = True
    cboStrikePriceGroup.Enabled = True
End Sub

Private Sub Form_Load()
Dim sUnderlying
Dim iIndex As Integer

    OnAllUnderlyings

    ' underlyings
    cboUnderlying.Clear
    For iIndex = 0 To frmMain.cboUnderlyings.ListCount - 1
        cboUnderlying.AddItem frmMain.cboUnderlyings.List(iIndex)
    Next
    If frmMain.cboUnderlyings.ListCount > 0 Then
        cboUnderlying.Text = frmMain.cboUnderlyings.List(0)
    End If
    
    ' instrument groups
    cboInstrumentGroup.Clear
    cboInstrumentGroup.AddItem "All"
'    cboInstrumentGroup.AddItem "European Call"
'    cboInstrumentGroup.AddItem "European Put"
    cboInstrumentGroup.AddItem "American Call"
    cboInstrumentGroup.AddItem "American Put"
    cboInstrumentGroup.Text = "All"
    
    ' Expiration Group
    cboExpirationGroup.Clear
    cboExpirationGroup.AddItem "All"
    cboExpirationGroup.AddItem "Near"
    cboExpirationGroup.AddItem "< 9 months"
    cboExpirationGroup.AddItem "LEAPS"
    cboExpirationGroup.Text = "All"
    
    ' Strike Price Group
    cboStrikePriceGroup.Clear
    cboStrikePriceGroup.AddItem "All"
    cboStrikePriceGroup.AddItem "Deep In The Money"
    cboStrikePriceGroup.AddItem "In The Money"
    cboStrikePriceGroup.AddItem "At The Money"
    cboStrikePriceGroup.AddItem "Out The Money"
    cboStrikePriceGroup.AddItem "Deep Out The Money"
    cboStrikePriceGroup.Text = "All"
    
    
End Sub

Private Sub optAll_Click(Index As Integer)
    OnAllUnderlyings
End Sub

Private Sub optOne_Click(Index As Integer)
    OnOneUnderlying
End Sub
