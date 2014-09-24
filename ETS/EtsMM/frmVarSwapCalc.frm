VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmVarSwapCalc 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Variance Swap Calculator"
   ClientHeight    =   6480
   ClientLeft      =   45
   ClientTop       =   315
   ClientWidth     =   13470
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6480
   ScaleWidth      =   13470
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtATMVola 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   6000
      Locked          =   -1  'True
      TabIndex        =   72
      TabStop         =   0   'False
      Top             =   6000
      Width           =   1575
   End
   Begin VB.TextBox txtVommaDown 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9720
      Locked          =   -1  'True
      TabIndex        =   70
      TabStop         =   0   'False
      Top             =   5040
      Width           =   1095
   End
   Begin VB.TextBox txtVommaUp 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9720
      Locked          =   -1  'True
      TabIndex        =   69
      TabStop         =   0   'False
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox txtVannaDown 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   8520
      Locked          =   -1  'True
      TabIndex        =   68
      TabStop         =   0   'False
      Top             =   5040
      Width           =   1095
   End
   Begin VB.TextBox txtVannaUp 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   8520
      Locked          =   -1  'True
      TabIndex        =   67
      TabStop         =   0   'False
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox txtVegaDown 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   7320
      Locked          =   -1  'True
      TabIndex        =   66
      TabStop         =   0   'False
      Top             =   5040
      Width           =   1095
   End
   Begin VB.TextBox txtVegaUp 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   7320
      Locked          =   -1  'True
      TabIndex        =   65
      TabStop         =   0   'False
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox txtDeltaDown 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   4920
      Locked          =   -1  'True
      TabIndex        =   64
      TabStop         =   0   'False
      Top             =   5040
      Width           =   1095
   End
   Begin VB.TextBox txtDeltaUp 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   4920
      Locked          =   -1  'True
      TabIndex        =   63
      TabStop         =   0   'False
      Top             =   4560
      Width           =   1095
   End
   Begin VB.TextBox txtDeltaMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   4920
      Locked          =   -1  'True
      TabIndex        =   62
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtRho 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   12120
      Locked          =   -1  'True
      TabIndex        =   61
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtTheta 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   10920
      Locked          =   -1  'True
      TabIndex        =   60
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtVommaMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9720
      Locked          =   -1  'True
      TabIndex        =   59
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtVannaMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   8520
      Locked          =   -1  'True
      TabIndex        =   58
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtVegaMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   7320
      Locked          =   -1  'True
      TabIndex        =   57
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtGamma 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   6120
      Locked          =   -1  'True
      TabIndex        =   51
      TabStop         =   0   'False
      Top             =   3840
      Width           =   1095
   End
   Begin VB.TextBox txtRealizedClose 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   11880
      Locked          =   -1  'True
      TabIndex        =   46
      TabStop         =   0   'False
      Top             =   2400
      Width           =   1455
   End
   Begin VB.TextBox txtRealizedMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9600
      Locked          =   -1  'True
      TabIndex        =   45
      TabStop         =   0   'False
      Top             =   2400
      Width           =   1455
   End
   Begin VB.TextBox txtImplClose 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   11880
      Locked          =   -1  'True
      TabIndex        =   44
      TabStop         =   0   'False
      Top             =   1920
      Width           =   1455
   End
   Begin VB.TextBox txtImplMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9600
      Locked          =   -1  'True
      TabIndex        =   43
      TabStop         =   0   'False
      Top             =   1920
      Width           =   1455
   End
   Begin VB.TextBox txtImplAsk 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   8040
      Locked          =   -1  'True
      TabIndex        =   42
      Top             =   1920
      Width           =   1335
   End
   Begin VB.CommandButton btnCalculate 
      Caption         =   "Calculate"
      Enabled         =   0   'False
      Height          =   495
      Left            =   10560
      TabIndex        =   41
      Top             =   5760
      Width           =   2655
   End
   Begin VB.TextBox txtClose 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   11880
      Locked          =   -1  'True
      TabIndex        =   36
      TabStop         =   0   'False
      Top             =   1440
      Width           =   1455
   End
   Begin VB.TextBox txtMid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   9600
      Locked          =   -1  'True
      TabIndex        =   35
      TabStop         =   0   'False
      Top             =   1440
      Width           =   1455
   End
   Begin VB.TextBox txtAsk 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   8040
      Locked          =   -1  'True
      TabIndex        =   34
      TabStop         =   0   'False
      Top             =   1440
      Width           =   1335
   End
   Begin VB.TextBox txtImplBid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   6360
      Locked          =   -1  'True
      TabIndex        =   32
      TabStop         =   0   'False
      Top             =   1920
      Width           =   1455
   End
   Begin VB.TextBox txtBid 
      Alignment       =   1  'Right Justify
      Height          =   375
      Left            =   6360
      Locked          =   -1  'True
      TabIndex        =   31
      TabStop         =   0   'False
      Top             =   1440
      Width           =   1455
   End
   Begin VB.CommandButton btnGetData 
      Caption         =   "Get data"
      Default         =   -1  'True
      Height          =   255
      Left            =   2760
      TabIndex        =   27
      Top             =   120
      Width           =   1215
   End
   Begin VB.TextBox txtValuationDate 
      CausesValidation=   0   'False
      Height          =   285
      Left            =   2520
      TabIndex        =   26
      Top             =   3600
      Width           =   1455
   End
   Begin VB.Frame frmShift 
      Caption         =   "Shifts"
      Height          =   1335
      Left            =   120
      TabIndex        =   19
      Top             =   3960
      Width           =   3855
      Begin VB.CheckBox chkSftVolaPerc 
         Caption         =   "%%"
         Height          =   375
         Left            =   3000
         TabIndex        =   74
         Top             =   720
         Width           =   615
      End
      Begin VB.CheckBox chkSftSpotPerc 
         Caption         =   "%%"
         Height          =   375
         Left            =   3000
         TabIndex        =   73
         Top             =   240
         Width           =   735
      End
      Begin VB.CheckBox chkSftVolaAbs 
         Caption         =   "ABS"
         Height          =   375
         Left            =   2040
         TabIndex        =   25
         Top             =   720
         Width           =   615
      End
      Begin VB.CheckBox chkSftSpotAbs 
         Caption         =   "ABS"
         Height          =   375
         Left            =   2040
         TabIndex        =   24
         Top             =   240
         Width           =   735
      End
      Begin VB.TextBox txtSftVola 
         Height          =   375
         Left            =   1200
         TabIndex        =   23
         Top             =   720
         Width           =   615
      End
      Begin VB.TextBox txtSftSpot 
         Height          =   375
         Left            =   1200
         TabIndex        =   22
         Top             =   240
         Width           =   615
      End
      Begin VB.Label lblSftVola 
         Caption         =   "Volatility"
         Height          =   375
         Left            =   240
         TabIndex        =   21
         Top             =   840
         Width           =   855
      End
      Begin VB.Label lblSftSpot 
         Caption         =   "Spot price"
         Height          =   255
         Left            =   240
         TabIndex        =   20
         Top             =   360
         Width           =   975
      End
   End
   Begin VB.CommandButton btnDivs 
      Caption         =   "Dividends"
      Enabled         =   0   'False
      Height          =   495
      Left            =   120
      TabIndex        =   17
      Top             =   5760
      Width           =   3855
   End
   Begin VB.TextBox txtToday 
      Enabled         =   0   'False
      Height          =   285
      Left            =   2520
      TabIndex        =   16
      Top             =   3240
      Width           =   1455
   End
   Begin VB.CheckBox chkDivs 
      Caption         =   "Use dividends"
      Height          =   255
      Left            =   120
      TabIndex        =   14
      Top             =   5400
      Width           =   1455
   End
   Begin VB.TextBox txtCap 
      Enabled         =   0   'False
      Height          =   285
      Left            =   1920
      TabIndex        =   13
      Top             =   2640
      Width           =   2055
   End
   Begin VB.TextBox txtStrike 
      Height          =   285
      Left            =   1920
      TabIndex        =   12
      Top             =   2160
      Width           =   2055
   End
   Begin VB.TextBox txtNotional 
      CausesValidation=   0   'False
      Height          =   285
      Left            =   1920
      TabIndex        =   11
      Top             =   1800
      Width           =   2055
   End
   Begin VB.TextBox txtObservation 
      Enabled         =   0   'False
      Height          =   285
      Left            =   1920
      TabIndex        =   7
      Top             =   1440
      Width           =   2055
   End
   Begin VB.ComboBox cmbExpiries 
      CausesValidation=   0   'False
      Height          =   315
      Left            =   1920
      Style           =   2  'Dropdown List
      TabIndex        =   5
      Top             =   960
      Width           =   2055
   End
   Begin MSComCtl2.DTPicker dtpTrade 
      Height          =   375
      Left            =   1920
      TabIndex        =   3
      Top             =   480
      Width           =   2055
      _ExtentX        =   3625
      _ExtentY        =   661
      _Version        =   393216
      Format          =   50855937
      CurrentDate     =   39234
      MinDate         =   39234
   End
   Begin VB.ComboBox cmbUnderlying 
      Height          =   315
      ItemData        =   "frmVarSwapCalc.frx":0000
      Left            =   1200
      List            =   "frmVarSwapCalc.frx":0002
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   120
      Width           =   1455
   End
   Begin VB.Label lblATMVola 
      Caption         =   "Reference ATM Vola"
      Height          =   255
      Left            =   4320
      TabIndex        =   71
      Top             =   6000
      Width           =   1575
   End
   Begin VB.Line Line5 
      X1              =   4200
      X2              =   13320
      Y1              =   3120
      Y2              =   3120
   End
   Begin VB.Label lblRho 
      Caption         =   "Rho"
      Height          =   255
      Left            =   12240
      TabIndex        =   56
      Top             =   3480
      Width           =   735
   End
   Begin VB.Label lblTheta 
      Caption         =   "Theta"
      Height          =   255
      Left            =   11160
      TabIndex        =   55
      Top             =   3480
      Width           =   615
   End
   Begin VB.Label lblVomma 
      Caption         =   "Vomma"
      Height          =   255
      Left            =   9960
      TabIndex        =   54
      Top             =   3480
      Width           =   735
   End
   Begin VB.Label lblVanna 
      Caption         =   "Vanna"
      Height          =   255
      Left            =   8880
      TabIndex        =   53
      Top             =   3480
      Width           =   735
   End
   Begin VB.Label lblVega 
      Caption         =   "Vega"
      Height          =   255
      Left            =   7680
      TabIndex        =   52
      Top             =   3480
      Width           =   615
   End
   Begin VB.Label lblGamma 
      Caption         =   "Gamma"
      Height          =   255
      Left            =   6360
      TabIndex        =   50
      Top             =   3480
      Width           =   855
   End
   Begin VB.Label lblDown 
      Caption         =   "Down"
      Height          =   255
      Left            =   4320
      TabIndex        =   49
      Top             =   5040
      Width           =   495
   End
   Begin VB.Label lblUp 
      Caption         =   "Up"
      Height          =   255
      Left            =   4320
      TabIndex        =   48
      Top             =   4560
      Width           =   495
   End
   Begin VB.Label lblDelta 
      Caption         =   "Delta"
      Height          =   255
      Left            =   5160
      TabIndex        =   47
      Top             =   3480
      Width           =   735
   End
   Begin VB.Line Line4 
      X1              =   11880
      X2              =   13320
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Label lblClose 
      Caption         =   "Close"
      Height          =   255
      Left            =   12360
      TabIndex        =   40
      Top             =   240
      Width           =   615
   End
   Begin VB.Line Line3 
      X1              =   6360
      X2              =   11040
      Y1              =   600
      Y2              =   600
   End
   Begin VB.Label lblIntraday 
      Caption         =   "Intraday"
      Height          =   375
      Left            =   8160
      TabIndex        =   39
      Top             =   240
      Width           =   1095
   End
   Begin VB.Label lblMid 
      Caption         =   "Mid"
      Height          =   375
      Left            =   10200
      TabIndex        =   38
      Top             =   960
      Width           =   735
   End
   Begin VB.Label lblAsk 
      Caption         =   "Ask"
      Height          =   375
      Left            =   8400
      TabIndex        =   37
      Top             =   960
      Width           =   735
   End
   Begin VB.Label lblBid 
      Caption         =   "Bid"
      Height          =   375
      Left            =   6840
      TabIndex        =   33
      Top             =   960
      Width           =   495
   End
   Begin VB.Label lblRealized 
      Caption         =   "Realized Variance"
      Height          =   375
      Left            =   4320
      TabIndex        =   30
      Top             =   2400
      Width           =   2055
   End
   Begin VB.Label lblVolatility 
      Caption         =   "Fair Strike (Volatility)"
      Height          =   375
      Left            =   4320
      TabIndex        =   29
      Top             =   1920
      Width           =   1935
   End
   Begin VB.Label lblUndPrice 
      Caption         =   "Underlying spot price"
      Height          =   375
      Left            =   4320
      TabIndex        =   28
      Top             =   1440
      Width           =   1575
   End
   Begin VB.Label lblValDate 
      Caption         =   "Valuation date(days to expiration)"
      Height          =   255
      Left            =   120
      TabIndex        =   18
      Top             =   3600
      Width           =   2415
   End
   Begin VB.Label lblToday 
      Caption         =   "Current date(days to expiration)"
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   3240
      Width           =   2295
   End
   Begin VB.Line Line2 
      X1              =   120
      X2              =   3960
      Y1              =   3120
      Y2              =   3120
   End
   Begin VB.Line Line1 
      X1              =   4080
      X2              =   4080
      Y1              =   120
      Y2              =   6360
   End
   Begin VB.Label lblCap 
      Caption         =   "Variance Cap"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   2640
      Width           =   1575
   End
   Begin VB.Label lblStrike 
      Caption         =   "Variance strike price"
      Height          =   255
      Left            =   120
      TabIndex        =   9
      Top             =   2160
      Width           =   1575
   End
   Begin VB.Label lblNotional 
      Caption         =   "Notional"
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   1800
      Width           =   1215
   End
   Begin VB.Label lblObservation 
      Caption         =   "Observation period"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1440
      Width           =   1335
   End
   Begin VB.Label lblExpiration 
      Caption         =   "Expiration"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   960
      Width           =   1215
   End
   Begin VB.Label lblTradeDate 
      Caption         =   "Trade Date"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   600
      Width           =   1095
   End
   Begin VB.Label lblUnderlying 
      Caption         =   "Underlying"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "frmVarSwapCalc"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private startingLetters As String
Private lastKeystrokeTime As Long
Private Const CB_FINDSTRING = &H14C
Private m_Notional As Double, m_Strike As Double, m_ShiftSpot As Double, m_ShiftVola As Double
Private m_Yield As Double
Private Divs As EtsGeneralLib.EtsIndexDivAtom
Private m_ValuationDate As Integer
Private bConnected As Boolean, bGotData As Boolean
Private WithEvents dds As IVDDSEnt
Attribute dds.VB_VarHelpID = -1
Private spotShiftAbs As Boolean
Private volaShiftAbs As Boolean
Private AskPrice As Double, BidPrice As Double, ClosePrice As Double
Private m_putVolas() As Double, m_callVolas() As Double, m_Strikes() As Double
Private m_undPrices() As Double, m_undPricesDates() As Date
Private m_ExpiryCount As Long, m_StrikeCount As Long
Private m_bGotHistoricalPrices As Boolean
Dim strColl As Collection, volaCol As Collection, expCol As Collection
Private m_initUnd As Long
Public m_nWindowIndex As Long
Private m_sCaption As String
Private tmpVDText As String
                                     
Private Sub SetUI()
    If bGotData = True Then
        dtpTrade.Enabled = True
        cmbExpiries.Enabled = True
        txtNotional.Enabled = True
        txtStrike.Enabled = True
        txtSftSpot.Enabled = True
        txtSftVola.Enabled = True
        txtValuationDate.Enabled = True
        chkDivs.Enabled = True
        chkSftSpotAbs.Enabled = True
        chkSftSpotAbs.Value = 1
        chkSftSpotPerc.Value = 0
        chkSftSpotPerc.Enabled = True
        chkSftVolaAbs.Enabled = True
        chkSftVolaPerc.Enabled = True
        chkSftVolaAbs.Value = 1
        chkSftVolaPerc.Value = 0
        If chkDivs.Value = 1 Then btnDivs.Enabled = True Else btnDivs.Enabled = False
        lblCap.Enabled = True
        lblExpiration.Enabled = True
        lblNotional.Enabled = True
        lblObservation.Enabled = True
        lblSftSpot.Enabled = True
        lblSftVola.Enabled = True
        lblStrike.Enabled = True
        lblToday.Enabled = True
        lblValDate.Enabled = True
        lblTradeDate.Enabled = True
        frmShift.Enabled = True
        lblBid.Enabled = True
        lblAsk.Enabled = True
        lblMid.Enabled = True
        lblClose.Enabled = True
        lblIntraday.Enabled = True
        txtBid.Enabled = True
        txtAsk.Enabled = True
        txtMid.Enabled = True
        txtClose.Enabled = True
        lblUndPrice.Enabled = True
        lblVolatility.Enabled = True
        lblRealized.Enabled = True
        btnCalculate.Enabled = True
        txtImplBid.Enabled = True
        txtImplAsk.Enabled = True
        txtImplMid.Enabled = True
        txtImplClose.Enabled = True
        txtRealizedClose.Enabled = True
        txtRealizedMid.Enabled = True
        lblDelta.Enabled = True
        lblGamma.Enabled = True
        lblVega.Enabled = True
        lblVanna.Enabled = True
        lblVomma.Enabled = True
        lblTheta.Enabled = True
        lblRho.Enabled = True
        txtDeltaMid.Enabled = True
        txtDeltaUp.Enabled = True
        txtDeltaDown.Enabled = True
        txtGamma.Enabled = True
        txtVegaMid.Enabled = True
        txtVegaUp.Enabled = True
        txtVegaDown.Enabled = True
        txtVannaMid.Enabled = True
        txtVannaUp.Enabled = True
        txtVannaDown.Enabled = True
        txtVommaMid.Enabled = True
        txtVommaUp.Enabled = True
        txtVommaDown.Enabled = True
        txtTheta.Enabled = True
        txtRho.Enabled = True
        lblUp.Enabled = True
        lblDown.Enabled = True
        lblATMVola.Enabled = True
        txtATMVola.Enabled = True
    Else
        dtpTrade.Enabled = False
        cmbExpiries.Enabled = False
        txtNotional.Enabled = False
        txtStrike.Enabled = False
        txtSftSpot.Enabled = False
        txtSftVola.Enabled = False
        txtValuationDate.Enabled = False
        chkDivs.Enabled = False
        chkSftSpotAbs.Enabled = False
        chkSftSpotPerc.Enabled = False
        chkSftSpotAbs.Value = 0
        chkSftSpotPerc.Value = 0
        chkSftVolaAbs.Enabled = False
        chkSftVolaPerc.Enabled = False
        chkSftVolaAbs.Value = 0
        chkSftVolaPerc.Value = 0
        btnDivs.Enabled = False
        lblCap.Enabled = False
        lblExpiration.Enabled = False
        lblNotional.Enabled = False
        lblObservation.Enabled = False
        lblSftSpot.Enabled = False
        lblSftVola.Enabled = False
        lblStrike.Enabled = False
        lblToday.Enabled = False
        lblValDate.Enabled = False
        lblTradeDate.Enabled = False
        frmShift.Enabled = False
        lblBid.Enabled = False
        lblAsk.Enabled = False
        lblMid.Enabled = False
        lblClose.Enabled = False
        lblIntraday.Enabled = False
        txtBid.Enabled = False
        txtAsk.Enabled = False
        txtMid.Enabled = False
        txtClose.Enabled = False
        lblUndPrice.Enabled = False
        lblVolatility.Enabled = False
        lblRealized.Enabled = False
        btnCalculate.Enabled = False
        txtImplBid.Text = ""
        txtImplAsk.Text = ""
        txtImplMid.Text = ""
        txtImplClose.Text = ""
        txtRealizedClose.Text = ""
        txtRealizedMid.Text = ""
        txtImplBid.Enabled = False
        txtImplAsk.Enabled = False
        txtImplMid.Enabled = False
        txtImplClose.Enabled = False
        txtRealizedClose.Enabled = False
        txtRealizedMid.Enabled = False
        lblDelta.Enabled = False
        lblGamma.Enabled = False
        lblVega.Enabled = False
        lblVanna.Enabled = False
        lblVomma.Enabled = False
        lblTheta.Enabled = False
        lblRho.Enabled = False
        txtDeltaMid.Enabled = False
        txtDeltaUp.Enabled = False
        txtDeltaDown.Enabled = False
        txtGamma.Enabled = False
        txtVegaMid.Enabled = False
        txtVegaUp.Enabled = False
        txtVegaDown.Enabled = False
        txtVannaMid.Enabled = False
        txtVannaUp.Enabled = False
        txtVannaDown.Enabled = False
        txtVommaMid.Enabled = False
        txtVommaUp.Enabled = False
        txtVommaDown.Enabled = False
        txtTheta.Enabled = False
        txtRho.Enabled = False
        txtDeltaMid.Text = ""
        txtDeltaUp.Text = ""
        txtDeltaDown.Text = ""
        txtGamma.Text = ""
        txtVegaMid.Text = ""
        txtVegaUp.Text = ""
        txtVegaDown.Text = ""
        txtVannaMid.Text = ""
        txtVannaUp.Text = ""
        txtVannaDown.Text = ""
        txtVommaMid.Text = ""
        txtVommaUp.Text = ""
        txtVommaDown.Text = ""
        txtTheta.Text = ""
        txtRho.Text = ""
        lblUp.Enabled = False
        lblDown.Enabled = False
        lblATMVola.Enabled = False
        txtATMVola.Enabled = False
        txtATMVola.Text = ""
    End If
End Sub

Public Function IsConnected() As Boolean
    IsConnected = bConnected
End Function
Private Sub InitUndList()
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim Idx As Long
    
    Idx = 0
    For Each aUnd In g_Main.UnderlyingAll
        cmbUnderlying.AddItem aUnd.Symbol
        cmbUnderlying.ItemData(cmbUnderlying.ListCount - 1) = aUnd.ID
        If m_initUnd <> 0 Then
            If aUnd.ID = m_initUnd Then Idx = cmbUnderlying.ListCount - 1
        End If
    Next
    cmbUnderlying.ListIndex = Idx
End Sub
Private Sub GetUnderlyingHistoricalPrices()
    On Error GoTo GetUnderlyingHistoricalPrices_Error
    Dim dtFrom As Date, dtTo As Date, i&
    Dim undSymbol As String, rs As ADODB.Recordset
    
    dtFrom = dtpTrade.Value
    dtTo = Date
    undSymbol = cmbUnderlying.Text
    m_bGotHistoricalPrices = False
    Set rs = gDBW.usp_IvUnderlyingHistoricalPrices_Get(undSymbol, dtFrom, dtTo)
    If rs.RecordCount > 0 Then
        m_bGotHistoricalPrices = True
        ReDim m_undPrices(0 To rs.RecordCount - 1)
        ReDim m_undPricesDates(0 To rs.RecordCount - 1)
        i = 0
        While Not rs.EOF
            m_undPrices(i) = Round(rs!price, 2)
            m_undPricesDates(i) = rs!t_date
            rs.MoveNext
            i = i + 1
        Wend
    End If
    Exit Sub
GetUnderlyingHistoricalPrices_Error:
    m_bGotHistoricalPrices = False
    MsgBox "Unable to retrieve hisotorical prices: " & Err.Description, vbExclamation
End Sub
Private Sub LogData()
    Dim logMsg$, i
    
    LogEvent EVENT_INFO, "Dumping data used at variance swap calculation"
    For i = LBound(m_callVolas) To UBound(m_callVolas)
        logMsg = "Strike " & CStr(Round(m_Strikes(i), 2)) & " cIV - " & CStr(Round(m_callVolas(i), 8)) & ", pIV - " & CStr(Round(m_putVolas(i), 8))
        LogEvent EVENT_INFO, logMsg
    Next
    
    For i = LBound(m_undPrices) To UBound(m_undPrices)
        logMsg = "UndPrice(" & CStr(m_undPricesDates(i)) & ") - " & CStr(Round(m_undPrices(i), 2))
        LogEvent EVENT_INFO, logMsg
    Next
        
End Sub
Private Sub btnCalculate_Click()
    On Error Resume Next
    Dim Exp As String, strikesToSort() As Long, i&
    Dim g As VSGreeksData, dtValDate As Date, dtExpDate As Date, dtTradeDate As Date
    Dim dDivDte() As Double, dDivAmts() As Double, nDivCount As Long, implVariance As Double
    Dim Yield As Double, realVariance As Double, Strike
    
    If bGotData And volaCol.Count > 0 And strColl.Count > 0 Then
         If IsDate(cmbExpiries.Text) Then
            Me.MousePointer = ccHourglass
            If tmpVDText <> "" Then
                If IsNumeric(tmpVDText) Then
                    dtValDate = DateAdd("d", CLng(tmpVDText), Date)
                End If
            Else
                dtValDate = Date
            End If
            dtExpDate = CDate(cmbExpiries.Text)
            dtTradeDate = dtpTrade.Value
            If dtExpDate < dtValDate Then
                MsgBox "Valuation date should be less than expiry", vbExclamation
                Exit Sub
            End If
            m_bGotHistoricalPrices = False
            If dtTradeDate < Date Then GetUnderlyingHistoricalPrices
            
            nDivCount = 0
            ReDim dDivDte(0 To 0)
            ReDim dDivAmts(0 To 0)
            
            '''''''''''''''''''''''''
'            dtValDate = "3/30/2006": dtExpDate = "4/22/2006"
'            LoadTestData
            '''''''''''''''''''''''''
            
            If chkDivs.Value = 1 Then
                Divs.GetDividendCount dtValDate, dtExpDate, nDivCount
                If nDivCount > 0 Then
                    Divs.GetDividends dtValDate, dtExpDate, nDivCount, dDivAmts, dDivDte, nDivCount
                End If
                Yield = m_Yield
            Else
                Yield = 0
            End If
            
            ReDim m_putVolas(0 To strColl.Count - 1)
            ReDim m_callVolas(0 To strColl.Count - 1)
            ReDim strikesToSort(0 To strColl.Count - 1)
            ReDim m_Strikes(0 To strColl.Count - 1)
            
            
            i = 0
            For Each Strike In strColl
                strikesToSort(i) = CLng(Strike * 1000)
                i = i + 1
            Next
            SortArray strikesToSort, LBound(strikesToSort), UBound(strikesToSort)
            For i = LBound(strikesToSort) To UBound(strikesToSort)
                m_putVolas(i) = 0
                m_callVolas(i) = 0
                m_putVolas(i) = ShiftVola(CDbl(volaCol.Item(cmbExpiries.Text & CStr(CDbl(strikesToSort(i)) / 1000) & "P")))
                m_callVolas(i) = ShiftVola(CDbl(volaCol.Item(cmbExpiries.Text & CStr(CDbl(strikesToSort(i)) / 1000) & "C")))
                m_Strikes(i) = CDbl(strikesToSort(i)) / 1000
            Next
            
            InterpolateAbsentVolas m_callVolas, m_Strikes
            InterpolateAbsentVolas m_putVolas, m_Strikes
            i = VarianceSwapGreeks(dtValDate, GetNeutralRate(dtValDate, dtExpDate), _
                                    midPrice(), Yield, _
                                    dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount), dtExpDate, _
                                    m_Strikes(0), m_callVolas(0), m_putVolas(0), CLng(strColl.Count), g)
            
'            i = VarianceSwapGreeks(dtValDate, 0.0483, _
'                                    midPrice(), Yield, _
'                                    dDivAmts(0), dDivDte(0), nDivCount, dtExpDate, _
'                                    m_Strikes(0), m_callVolas(0), m_putVolas(0), 35, g)
                                    
            txtDeltaMid.Text = CStr(Round(g.dDelta, 3))
            txtDeltaUp.Text = CStr(Round(g.dDeltaUp, 3))
            txtDeltaDown.Text = CStr(Round(g.dDeltaDown, 3))
            txtGamma.Text = CStr(Round(g.dGamma, 3))
            txtVegaMid.Text = CStr(Round(g.dVega, 3))
            txtVegaUp.Text = CStr(Round(g.dVegaUp, 3))
            txtVegaDown.Text = CStr(Round(g.dVegaDown, 3))
            txtVannaMid.Text = CStr(Round(g.dVanna, 3))
            txtVommaMid.Text = CStr(Round(g.dVomma, 3))
            txtVannaUp.Text = CStr(Round(g.dVannaUp, 3))
            txtVommaUp.Text = CStr(Round(g.dVommaUp, 3))
            txtVannaDown.Text = CStr(Round(g.dVannaDown, 3))
            txtVommaDown.Text = CStr(Round(g.dVommaDown, 3))
            txtTheta.Text = CStr(Round(g.dTheta, 3))
            txtRho.Text = CStr(Round(g.dRho, 3))
                                    
            LogData
            
            If ShiftPrice(AskPrice) > 0 Then
                implVariance = ImpliedVariance(dtValDate, GetNeutralRate(dtValDate, dtExpDate), _
                                                ShiftPrice(AskPrice), Yield, _
                                                dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount), dtExpDate, _
                                                m_Strikes(0), m_callVolas(0), m_putVolas(0), strColl.Count)
                txtImplAsk.Text = CStr(Round(implVariance, 2)) & " (" & CStr(Round(Sqr(implVariance), 2)) & ")"
            Else
                txtImplAsk.Text = "N/A"
            End If
            If ShiftPrice(BidPrice) > 0 Then
                implVariance = ImpliedVariance(dtValDate, GetNeutralRate(dtValDate, dtExpDate), _
                                                ShiftPrice(BidPrice), Yield, _
                                                dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount), dtExpDate, _
                                                m_Strikes(0), m_callVolas(0), m_putVolas(0), strColl.Count)
                txtImplBid.Text = CStr(Round(implVariance, 2)) & " (" & CStr(Round(Sqr(implVariance), 2)) & ")"
            Else
                txtImplBid.Text = "N/A"
            End If
            If midPrice() > 0 Then
                implVariance = ImpliedVariance(dtValDate, GetNeutralRate(dtValDate, dtExpDate), _
                                                midPrice(), Yield, _
                                                dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount), dtExpDate, _
                                                m_Strikes(0), m_callVolas(0), m_putVolas(0), strColl.Count)
                txtImplMid.Text = CStr(Round(implVariance, 2)) & " (" & CStr(Round(Sqr(implVariance), 2)) & ")"
            Else
                txtImplMid.Text = "N/A"
            End If
            If ShiftPrice(ClosePrice) > 0 Then
                implVariance = ImpliedVariance(dtValDate, GetNeutralRate(dtValDate, dtExpDate), _
                                                ShiftPrice(ClosePrice), Yield, _
                                                dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount), dtExpDate, _
                                                m_Strikes(0), m_callVolas(0), m_putVolas(0), strColl.Count)
                txtImplClose.Text = CStr(Round(implVariance, 2)) & " (" & CStr(Round(Sqr(implVariance), 2)) & ")"
            Else
                txtImplClose.Text = "N/A"
            End If
            
            If m_bGotHistoricalPrices = True Then
                realVariance = RealizedVariance(m_undPricesDates(0), m_undPrices(0), _
                                                    UBound(m_undPrices) - LBound(m_undPrices) + 1, _
                                                    dtTradeDate, Date, dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount))
                txtRealizedClose.Text = CStr(Round(realVariance, 2))
            Else
                txtRealizedClose.Text = "N/A"
            End If
            
            ReDim Preserve m_undPrices(0 To UBound(m_undPrices) + 1)
            ReDim Preserve m_undPricesDates(0 To UBound(m_undPrices))
            m_undPrices(UBound(m_undPrices)) = midPrice()
            m_undPricesDates(UBound(m_undPrices)) = Date
            realVariance = RealizedVariance(m_undPricesDates(0), m_undPrices(0), _
                                            UBound(m_undPrices) - LBound(m_undPrices) + 1, _
                                            dtTradeDate, Date, dDivAmts(0), dDivDte(0), IIf(Yield > 0, 0, nDivCount))
            txtRealizedMid.Text = CStr(Round(realVariance, 2))
            
            PutAtmVola
            
         End If
    End If
    Me.MousePointer = ccArrow
End Sub

Private Sub InterpolateAbsentVolas(Vola() As Double, Strikes() As Double)
    Dim leftPoint As Double, rightPoint As Double, i&, j&
    Dim strikeLeft As Double, strikeRight As Double
    Dim dV As Double, dSv As Double, dS As Double
    
    leftPoint = 0: strikeLeft = 0
    For i = LBound(Vola) To UBound(Vola)
        If Vola(i) > 0 Then
            leftPoint = Vola(i)
            strikeLeft = Strikes(i)
        Else
            ' look up for right point
            rightPoint = 0: strikeRight = 0
            For j = i To UBound(Vola)
                If Vola(j) > 0 Then
                    rightPoint = Vola(j)
                    strikeRight = Strikes(j)
                    Exit For
                End If
            Next
            
            If leftPoint > 0 And rightPoint > 0 Then
                dSv = Strikes(i) - strikeLeft
                dS = strikeRight - strikeLeft
                If dS = 0 Then dS = 1
                dV = rightPoint - leftPoint
                Vola(i) = (dV * dSv) / dS + leftPoint
            Else
                If leftPoint > 0 Then Vola(i) = leftPoint Else Vola(i) = rightPoint
            End If
            leftPoint = Vola(i)
            strikeLeft = Strikes(i)
        End If
    Next
    
End Sub

Private Sub btnDivs_Click()
    Dim DivDates() As Double, DivAmounts() As Double, DivCount As Long, Dates() As Date
    Dim dte As Date, amt As Double, DivFreq As Long, i&
    DivCount = 0
    If Not Divs.CustomDivs Is Nothing Then
        
        Divs.CustomDivs.GetDividendCount Date, CDate("12/12/2999"), DivCount
        If DivCount > 0 Then
            Divs.CustomDivs.GetDividends Date, CDate("12/12/2999"), DivCount, DivAmounts, DivDates, DivCount
        End If
    Else
        ReDim DivAmounts(0)
        ReDim DivDates(0)
    End If
    Load frmVSCDivs
    dte = IIf(Divs.DivType = enDivMarket, Divs.DivDate, Divs.DivDateCust)
    amt = IIf(Divs.DivType = enDivMarket, Divs.DivAmt, Divs.DivAmtCust)
    DivFreq = IIf(Divs.DivType = enDivMarket, Divs.DivFreq, Divs.DivFreqCust)
    frmVSCDivs.Init m_Yield, DivDates, DivAmounts, dte, amt, Divs.DivType, DivFreq, DivCount
    frmVSCDivs.Show vbModal
    If frmVSCDivs.bOk = True Then
        Divs.ResetInternalCache
        If frmVSCDivs.useYield = True Then
            m_Yield = frmVSCDivs.m_Yield
        Else
            m_Yield = 0
            If frmVSCDivs.GetDivsStream(Dates, DivAmounts) > 0 Then
                Divs.CustomDivs.Clear
                For i = LBound(Dates) To UBound(Dates)
                    Divs.CustomDivs.AddNonUnique Dates(i), DivAmounts(i)
                Next
                Divs.DivType = enDivCustomStream
            Else
               Divs.DivFreq = frmVSCDivs.mDivFreq
               Divs.DivFreqCust = frmVSCDivs.mDivFreq
               Divs.DivAmt = frmVSCDivs.mDivAmount
               Divs.DivAmtCust = frmVSCDivs.mDivAmount
               Divs.DivDate = frmVSCDivs.mDivDate
               Divs.DivDateCust = frmVSCDivs.mDivDate
               Divs.DivType = enDivMarket
            End If
        End If
    End If
End Sub

Private Sub btnGetData_Click()
    Dim selectedUnd As EtsGeneralLib.UndAtom, selectedUndID As Long
    
    FillExpirationCombo
    selectedUndID = cmbUnderlying.ItemData(cmbUnderlying.ListIndex)
    Set selectedUnd = g_Main.UnderlyingAll(selectedUndID)
    If selectedUnd.IsBasketIndex = True Then
        Set Divs = New EtsGeneralLib.EtsIndexDivAtom
        Set Divs.CustomDivs = New EtsDivColl
    Else
        Set Divs = selectedUnd.Dividend.Clone
        If Divs.CustomDivs Is Nothing Then Set Divs.CustomDivs = New EtsDivColl
    End If
    GetStockData
    SetUI
    Me.Caption = "Variance Swap Calculator " & cmbUnderlying.Text
End Sub

Private Sub chkDivs_Click()
    btnDivs.Enabled = chkDivs.Value
End Sub

Private Sub chkSftSpotAbs_Click()
    spotShiftAbs = IIf(chkSftSpotAbs.Value = 1, True, False)
'    txtAsk.Text = CStr(Round(AskPrice, 2))
'    txtBid.Text = CStr(Round(ShiftPrice(BidPrice), 2))
    txtMid.Text = CStr(Round(midPrice(), 4))
    txtClose.Text = CStr(Round(ShiftPrice(ClosePrice), 2))
    chkSftSpotPerc.Value = IIf(chkSftSpotAbs.Value = 1, 0, 1)
End Sub
Private Sub chkSftSpotPerc_Click()
    spotShiftAbs = IIf(chkSftSpotPerc.Value = 1, False, True)
'    txtAsk.Text = CStr(ShiftPrice(AskPrice))
'    txtBid.Text = CStr(ShiftPrice(BidPrice))
    txtMid.Text = CStr(Round(midPrice(), 4))
    txtClose.Text = CStr(Round(ShiftPrice(ClosePrice), 2))
    chkSftSpotAbs.Value = IIf(chkSftSpotPerc.Value = 1, 0, 1)
End Sub

Private Sub chkSftVolaAbs_Click()
    volaShiftAbs = IIf(chkSftVolaAbs.Value = 1, True, False)
    chkSftVolaPerc.Value = IIf(chkSftVolaAbs.Value = 1, 0, 1)
End Sub
Private Sub chkSftVolaPerc_Click()
    volaShiftAbs = IIf(chkSftVolaPerc.Value = 1, False, True)
    chkSftVolaAbs.Value = IIf(chkSftVolaPerc.Value = 1, 0, 1)
End Sub

Private Sub cmbExpiries_Click()
    Dim expiryDate As Date
    If IsDate(cmbExpiries.Text) Then
       expiryDate = CDate(cmbExpiries.Text)
       txtObservation.Text = DateDiff("d", dtpTrade.Value, expiryDate)
       txtToday.Text = CStr(Date) & " (" & CStr(DateDiff("d", Date, expiryDate)) & ")"
    End If
End Sub

Private Sub cmbUnderlying_Click()
    bGotData = False
    SetUI
End Sub

Private Sub cmbUnderlying_KeyPress(KeyAscii As Integer)
    Dim enteredCharCode As Long, timeSpan As Long, Idx As Long
    Dim searchString As String, selectedUndID
    
    enteredCharCode = Asc(UCase(Chr(KeyAscii)))
    If enteredCharCode >= Asc("A") And enteredCharCode <= Asc("Z") Then
        timeSpan = GetTickCount - lastKeystrokeTime
        If timeSpan < 3000 Then
            searchString = startingLetters + Chr(enteredCharCode)
            Idx = SendMessage(cmbUnderlying.hWnd, CB_FINDSTRING, -1, ByVal searchString)
            If Idx <> -1 Then
                cmbUnderlying.ListIndex = Idx
                lastKeystrokeTime = GetTickCount
                startingLetters = searchString
            Else
                lastKeystrokeTime = GetTickCount
                Set Divs = Nothing
                cmbExpiries.Clear
            End If
        Else
            searchString = Chr(enteredCharCode)
            Idx = SendMessage(cmbUnderlying.hWnd, CB_FINDSTRING, -1, ByVal searchString)
            If Idx <> -1 Then
                cmbUnderlying.ListIndex = Idx
                lastKeystrokeTime = GetTickCount
                startingLetters = searchString
                selectedUndID = cmbUnderlying.ItemData(cmbUnderlying.ListIndex)
            Else
                Set Divs = Nothing
                cmbExpiries.Clear
            End If
        End If
    End If
    cmbUnderlying.SetFocus
    KeyAscii = 0
End Sub
Private Sub ConnectToIvFeed()
    On Error GoTo ConnectToIvFeed_Error
    
    If gDBW.CheckIvDBConnection = False Then
        MsgBox "Unable to connect to IV database", vbExclamation
        Exit Sub
    End If
    
    bConnected = False
    If g_Params.IvFeedPassword = "" Or g_Params.IvFeedUser = "" Then
        frmIvFeedLogin.txtUserName.Text = g_Params.IvFeedUser
        frmIvFeedLogin.txtPassword.Text = g_Params.IvFeedPassword
        frmIvFeedLogin.Show vbModal
        If frmIvFeedLogin.OKPressed = True Then
            Screen.MousePointer = ccHourglass
            dds.Connect frmIvFeedLogin.txtUserName.Text, frmIvFeedLogin.txtPassword.Text
            g_Params.IvFeedUser = frmIvFeedLogin.txtUserName.Text
            g_Params.IvFeedPassword = frmIvFeedLogin.txtPassword.Text
            bConnected = True
        End If
    Else
        Screen.MousePointer = ccHourglass
        dds.Connect g_Params.IvFeedUser, g_Params.IvFeedPassword
        bConnected = True
    End If
    Screen.MousePointer = ccDefault
    Exit Sub
ConnectToIvFeed_Error:
    Screen.MousePointer = ccDefault
    MsgBox "Unable to connect to IV feed:" + Chr$(10) + Chr$(10) + Error$, vbExclamation
    g_Params.IvFeedUser = ""
    g_Params.IvFeedPassword = ""
End Sub


Private Sub dtpTrade_Change()
    Dim expiryDate As Date
    If IsDate(cmbExpiries.Text) Then
       expiryDate = CDate(cmbExpiries.Text)
       txtObservation.Text = DateDiff("d", dtpTrade.Value, expiryDate)
    End If
End Sub

Private Sub Form_Load()
    AskPrice = 0
    BidPrice = 0
    ClosePrice = 0
    bConnected = False
    bGotData = False
    spotShiftAbs = False
    volaShiftAbs = False
    'InitUndList
    startingLetters = ""
    lastKeystrokeTime = 0
    dtpTrade.MinDate = DateAdd("yyyy", -5, Date)
    dtpTrade.Value = Date
    m_Notional = 0: m_Strike = 0: m_ShiftSpot = 0: m_ShiftVola = 0
    Set Divs = New EtsGeneralLib.EtsIndexDivAtom
    Set dds = New IVDDSEnt
    ConnectToIvFeed
    If bConnected = False Then
        Set dds = Nothing
        Set Divs = Nothing
        Me.Hide
    Else
        ShowData
        SetUI
    End If
End Sub
Public Sub Init(sKey As String)
    m_sCaption = sKey + " VS Calc"
End Sub
Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "VS Calc ShowData Call", Me.Caption
        
    m_initUnd = nValue
    cmbUnderlying.Clear
    InitUndList
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "VS Calc ShowData Done", Me.Caption

End Sub
Private Sub Form_Terminate()
    If gDBW.IvConnection.State = ADODB.adStateOpen Then gDBW.IvConnection.Close
    dds.Disconnect
End Sub

Private Sub txtNotional_Validate(Cancel As Boolean)
    Cancel = (IsNumeric(txtNotional.Text) <> True)
    If Cancel Then
        MsgBox "Bad input for this field", vbExclamation + vbOKOnly
    Else
        m_Notional = CDbl(txtNotional.Text)
        If m_Notional <= 0 Then
            MsgBox "The value in this field should be greater than zero", vbExclamation + vbOKOnly
            Cancel = True
        End If
    End If
End Sub
Public Sub SetWinIndex(ByVal nIndex As Long)
    On Error Resume Next
    m_nWindowIndex = nIndex
    'AdjustCaption
End Sub
Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = m_sCaption
End Function
Private Sub txtSftVola_Validate(Cancel As Boolean)
    If txtSftVola.Text <> "" Then
        Cancel = (IsNumeric(txtSftVola.Text) <> True)
        If Cancel Then MsgBox "Bad input for this field", vbExclamation + vbOKOnly _
        Else m_ShiftVola = CDbl(txtSftVola.Text)
    Else
        m_ShiftVola = 0
    End If
End Sub
Private Sub txtSftSpot_Validate(Cancel As Boolean)
    If txtSftSpot.Text <> "" Then
        Cancel = (IsNumeric(txtSftSpot.Text) <> True)
        If Cancel Then MsgBox "Bad input for this field", vbExclamation + vbOKOnly _
        Else m_ShiftSpot = CDbl(txtSftSpot.Text)
    Else
        m_ShiftSpot = 0
    End If
    'txtAsk.Text = CStr(ShiftPrice(AskPrice))
    'txtBid.Text = CStr(ShiftPrice(BidPrice))
    txtMid.Text = CStr(Round(midPrice(), 4))
    'txtClose.Text = CStr(ShiftPrice(ClosePrice))
End Sub
Private Sub txtStrike_Change()
    Dim bCancel As Boolean
    bCancel = (IsNumeric(txtStrike.Text) <> True)
    If bCancel = False Then
        m_Strike = CDbl(txtStrike.Text)
        txtCap.Text = CStr(m_Strike * 6.25)
    Else
        txtCap.Text = ""
    End If
End Sub
Private Sub FillExpirationCombo()
    On Error GoTo FillExpirationCombo_Error
    Dim optn As OptionsEnt
    Dim undSymbol As String, Expiry As String, gotOne As Boolean, needToAdd As Boolean
    Dim i&, optTypeLetter As String
    Dim addedStrikes As String, MyPos, savedExpiry
    
    If dds Is Nothing Then Exit Sub
    
    Set strColl = New Collection
    Set volaCol = New Collection
    Set expCol = New Collection
    m_ExpiryCount = 0
    m_StrikeCount = 0
    gotOne = False
    bGotData = gotOne
    
  
    Me.MousePointer = ccHourglass
    cmbExpiries.Clear
    undSymbol = cmbUnderlying.List(cmbUnderlying.ListIndex)
    Set optn = dds.GetOptionsByStock(undSymbol)
    Do
        gotOne = True
        Expiry = CStr(optn.ExpirationDate)
        needToAdd = True
        
        For Each savedExpiry In expCol
            If savedExpiry = Expiry Then
                needToAdd = False
                Exit For
            End If
        Next
        
        If needToAdd = True Then
            cmbExpiries.AddItem Expiry
            expCol.Add (Expiry)
            m_ExpiryCount = m_ExpiryCount + 1
        End If
        
        If optn.OptionType = enCall Then optTypeLetter = "C" Else optTypeLetter = "P"
        
        volaCol.Add optn.IV, Expiry & CStr(optn.StrikePrice) & optTypeLetter
        
        MyPos = InStr(addedStrikes, "<" & CStr(optn.StrikePrice) & ">")
        If IsNull(MyPos) Then
            strColl.Add optn.StrikePrice, CStr(optn.StrikePrice)
            addedStrikes = addedStrikes & "<" & CStr(optn.StrikePrice) & ">"
            m_StrikeCount = m_StrikeCount + 1
        Else
            If MyPos = 0 Then
                strColl.Add optn.StrikePrice, CStr(optn.StrikePrice)
                addedStrikes = addedStrikes & "<" & CStr(optn.StrikePrice) & ">"
                m_StrikeCount = m_StrikeCount + 1
            End If
        End If
    Loop While optn.Next
    
    If volaCol.Count = 0 Then GoTo FillExpirationCombo_Error
    Me.MousePointer = ccDefault
    bGotData = gotOne
    If gotOne = True Then cmbExpiries.ListIndex = 0
    Exit Sub
FillExpirationCombo_Error:
    bGotData = False
    Me.MousePointer = ccDefault
    MsgBox "Unable to retrieve expirations or volas for options of underlying " + undSymbol + Chr$(10) + Error$, vbExclamation
    cmbExpiries.Clear
End Sub
Private Sub GetStockData()
    On Error GoTo GetStockData_Error
    Dim stck As StocksEnt, undSymbol As String
     
    If dds Is Nothing Then Exit Sub
    undSymbol = cmbUnderlying.List(cmbUnderlying.ListIndex)
    dds.Attach enStockData
    Set stck = dds.GetStockBySymbol(undSymbol)
    If stck Is Nothing Then GoTo GetStockData_Error
    AskPrice = stck.AskPrice
    BidPrice = stck.BidPrice
    ClosePrice = stck.PriceClose
    txtAsk.Text = CStr(Round(AskPrice, 2))
    txtBid.Text = CStr(Round(BidPrice, 2))
    txtClose.Text = CStr(Round(ClosePrice, 2))
    txtMid.Text = CStr(Round(midPrice(), 4))
    
    Exit Sub
GetStockData_Error:
    bGotData = False
    Me.MousePointer = ccDefault
    MsgBox "Unable to retrieve price data for " + undSymbol + Chr$(10) + Error$, vbExclamation
End Sub

Private Function ShiftPrice(price As Double) As Double
    If m_ShiftSpot <> 0 Then
        If spotShiftAbs = True Then
            ShiftPrice = price + m_ShiftSpot
        Else
            ShiftPrice = price + (price / 100 * m_ShiftSpot)
        End If
    Else
        ShiftPrice = price
    End If
End Function

Private Function ShiftVola(Vola As Double) As Double
    If m_ShiftVola <> 0 Then
        If volaShiftAbs = True Then
            ShiftVola = Vola + m_ShiftVola
        Else
            ShiftVola = Vola + (Vola / 100 * m_ShiftVola)
        End If
    Else
        ShiftVola = Vola
    End If
End Function

Private Function midPrice() As Double
    Dim p1 As Double, p2 As Double
    p1 = IIf(AskPrice > 0, AskPrice, IIf(BidPrice > 0, BidPrice, ClosePrice))
    p2 = IIf(BidPrice > 0, BidPrice, IIf(AskPrice > 0, AskPrice, ClosePrice))
    midPrice = (ShiftPrice(p1) + ShiftPrice(p2)) / 2
End Function
Private Sub PutAtmVola()
    Dim atmCallVolas() As Double, atmPutVolas() As Double, strks() As Double
    Dim mPrice As Double, i&, strikeDelta As Double, atmVola As Double
    Dim lowIndex&, highIndex&, atmIndex&
    
    lowIndex = -1: highIndex = -1
    ReDim Preserve atmCallVolas(0 To 2)
    ReDim Preserve atmPutVolas(0 To 2)
    ReDim Preserve strks(0 To 2)
    
    mPrice = midPrice()
    For i = LBound(m_Strikes) To UBound(m_Strikes)
        strikeDelta = m_Strikes(i) - mPrice
        If strikeDelta < 0 Then lowIndex = i Else Exit For
    Next
    For i = UBound(m_Strikes) To LBound(m_Strikes) Step -1
        strikeDelta = m_Strikes(i) - mPrice
        If strikeDelta > 0 Then highIndex = i Else Exit For
    Next
    
    If lowIndex <> -1 Then
        atmCallVolas(0) = m_callVolas(lowIndex)
        atmPutVolas(0) = m_putVolas(lowIndex)
        strks(0) = m_Strikes(lowIndex)
        atmIndex& = 1
    Else
        atmIndex = 0
    End If
    
    atmCallVolas(atmIndex) = 0
    atmPutVolas(atmIndex) = 0
    strks(atmIndex) = mPrice
    
    If highIndex <> -1 Then
        ReDim Preserve atmCallVolas(0 To atmIndex + 1)
        ReDim Preserve atmPutVolas(0 To atmIndex + 1)
        ReDim Preserve strks(0 To atmIndex + 1)
        
        atmCallVolas(atmIndex + 1) = m_callVolas(highIndex)
        atmPutVolas(atmIndex + 1) = m_putVolas(highIndex)
        strks(atmIndex + 1) = m_Strikes(highIndex)
        
    Else
        ReDim Preserve atmCallVolas(0 To atmIndex)
        ReDim Preserve atmPutVolas(0 To atmIndex)
        ReDim Preserve strks(0 To atmIndex)
    End If
    
    InterpolateAbsentVolas atmCallVolas, strks
    InterpolateAbsentVolas atmPutVolas, strks
    
    atmVola = (atmCallVolas(atmIndex) + atmPutVolas(atmIndex)) / 2
    
    txtATMVola.Text = CStr(Round(atmVola * 100, 2))
End Sub
Private Sub LoadTestVolasAndStrikes()
    ReDim m_putVolas(0 To 34)
    ReDim m_callVolas(0 To 34)
    ReDim m_Strikes(0 To 34)
    m_putVolas(0) = 0.2154              '1150
    m_putVolas(1) = 0.1937              '1175
    m_putVolas(2) = 0.1784              '1190
    m_putVolas(3) = 0.166               '1200
    m_putVolas(4) = 0.159               '1205
    m_putVolas(5) = 0.1603              '1210
    m_putVolas(6) = 0.1439              '1225
    m_putVolas(7) = 0.1396              '1230
    m_putVolas(8) = 0.1355              '1235
    m_putVolas(9) = 0.1308              '1240
    m_callVolas(10) = 0.0782            '1245
    m_putVolas(10) = 0.1273             '1245
    m_callVolas(11) = 0.0861            '1250
    m_putVolas(11) = 0.1231             '1250
    m_callVolas(12) = 0.0906            '1255
    m_putVolas(12) = 0.1216             '1255
    m_callVolas(13) = 0.0932            '1260
    m_putVolas(13) = 0.115              '1260
    m_callVolas(14) = 0.0945            '1265
    m_putVolas(14) = 0.1139             '1265
    m_callVolas(15) = 0.0933            '1270
    m_putVolas(15) = 0.1088             '1270
    m_callVolas(16) = 0.0928            '1275
    m_putVolas(16) = 0.107              '1275
    m_callVolas(17) = 0.0951            '1280
    m_putVolas(17) = 0.1052             '1280
    m_callVolas(18) = 0.0918            '1285
    m_putVolas(18) = 0.1043             '1285
    m_callVolas(19) = 0.0899            '1290
    m_putVolas(19) = 0.0999             '1290
    m_callVolas(20) = 0.0892            '1295
    m_putVolas(20) = 0.099              '1295
    m_callVolas(21) = 0.0915            '1300
    m_putVolas(21) = 0.0942             '1300
    m_callVolas(22) = 0.0867            '1305
    m_putVolas(22) = 0.097              '1305
    m_callVolas(23) = 0.0877            '1310
    m_putVolas(23) = 0.0956             '1310
    m_callVolas(24) = 0.0846            '1315
    m_putVolas(24) = 0.0952             '1315
    m_callVolas(25) = 0.086             '1320
    m_putVolas(25) = 0.0899             '1320
    m_callVolas(26) = 0.0848            '1325
    m_putVolas(26) = 0.0956             '1325
    m_callVolas(27) = 0.0846            '1330
    m_putVolas(27) = 0.0969             '1330
    m_callVolas(28) = 0.083             '1335
    m_putVolas(28) = 0.0982             '1335
    m_callVolas(29) = 0.0842            '1340
    m_putVolas(29) = 0.1012             '1340
    m_callVolas(30) = 0.0824            '1345
    m_putVolas(30) = 0.1041             '1345
    m_callVolas(31) = 0.085             '1350
    m_putVolas(31) = 0.1087             '1350
    m_callVolas(32) = 0.0835            '1355
    m_putVolas(32) = 0.1124             '1355
    m_callVolas(33) = 0.0854            '1360
    m_putVolas(33) = 0.119              '1360
    m_callVolas(34) = 0.1158            '1400
    m_putVolas(34) = 0.1691             '1400
    m_Strikes(0) = 1150
    m_Strikes(1) = 1175
    m_Strikes(2) = 1190
    m_Strikes(3) = 1200
    m_Strikes(4) = 1205
    m_Strikes(5) = 1210
    m_Strikes(6) = 1225
    m_Strikes(7) = 1230
    m_Strikes(8) = 1235
    m_Strikes(9) = 1240
    m_Strikes(10) = 1245
    m_Strikes(11) = 1250
    m_Strikes(12) = 1255
    m_Strikes(13) = 1260
    m_Strikes(14) = 1265
    m_Strikes(15) = 1270
    m_Strikes(16) = 1275
    m_Strikes(17) = 1280
    m_Strikes(18) = 1285
    m_Strikes(19) = 1290
    m_Strikes(20) = 1295
    m_Strikes(21) = 1300
    m_Strikes(22) = 1305
    m_Strikes(23) = 1310
    m_Strikes(24) = 1315
    m_Strikes(25) = 1320
    m_Strikes(26) = 1325
    m_Strikes(27) = 1330
    m_Strikes(28) = 1335
    m_Strikes(29) = 1340
    m_Strikes(30) = 1345
    m_Strikes(31) = 1350
    m_Strikes(32) = 1355
    m_Strikes(33) = 1360
    m_Strikes(34) = 1400
End Sub
Private Sub LoadTestData()
    LoadTestVolasAndStrikes
    AskPrice = 1302.89
    BidPrice = 1302.89
    ClosePrice = 1302.89
    m_Yield = 0.018
    
End Sub
Private Sub txtValuationDate_LostFocus()
    With txtValuationDate
        If IsNumeric(.Text) Then
            If CLng(.Text) > 0 Then
                tmpVDText = .Text
                .Text = CStr(DateAdd("d", CLng(.Text), Date)) & " (" & tmpVDText & ")"
            Else
                MsgBox "Bad input in valuation date field. Expecting integer greater than zero", vbExclamation
            End If
        Else
            MsgBox "Bad input in valuation date field. Expecting integer greater than zero", vbExclamation
        End If
    End With
End Sub
Private Sub txtValuationDate_GotFocus()
    With txtValuationDate
        .SelStart = 0
        .SelLength = Len(.Text)
    End With
End Sub
