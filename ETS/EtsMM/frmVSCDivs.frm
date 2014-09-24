VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmVSCDivs 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Dividends for VS calc"
   ClientHeight    =   4440
   ClientLeft      =   45
   ClientTop       =   315
   ClientWidth     =   4335
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4440
   ScaleWidth      =   4335
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2520
      TabIndex        =   12
      Top             =   3960
      Width           =   1575
   End
   Begin VB.CommandButton btnOk 
      Caption         =   "OK"
      Height          =   375
      Left            =   120
      TabIndex        =   11
      Top             =   3960
      Width           =   1575
   End
   Begin VB.Frame frmDiscrete 
      Caption         =   "Discrete"
      Height          =   2895
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   4095
      Begin VB.CommandButton btnStream 
         Caption         =   "Show stream"
         Height          =   375
         Left            =   120
         TabIndex        =   10
         Top             =   2280
         Width           =   1815
      End
      Begin VB.ComboBox cmbFreq 
         Height          =   315
         Left            =   2160
         TabIndex        =   6
         Top             =   1680
         Width           =   1695
      End
      Begin VB.TextBox txtAmount 
         Height          =   375
         Left            =   2160
         TabIndex        =   5
         Top             =   1200
         Width           =   1695
      End
      Begin MSComCtl2.DTPicker dtpDivDate 
         Height          =   375
         Left            =   2160
         TabIndex        =   4
         Top             =   720
         Width           =   1695
         _ExtentX        =   2990
         _ExtentY        =   661
         _Version        =   393216
         Format          =   68354049
         CurrentDate     =   39238
      End
      Begin VB.CheckBox chkPeriodic 
         Caption         =   "Periodic"
         Height          =   255
         Left            =   240
         TabIndex        =   3
         Top             =   360
         Width           =   1095
      End
      Begin VB.Label lblDivFreq 
         Caption         =   "Dividends frequency"
         Height          =   375
         Left            =   480
         TabIndex        =   9
         Top             =   1680
         Width           =   1815
      End
      Begin VB.Label lblDivAmount 
         Caption         =   "Dividends amount"
         Height          =   375
         Left            =   480
         TabIndex        =   8
         Top             =   1200
         Width           =   1455
      End
      Begin VB.Label lblDivDate 
         Caption         =   "Dividends date"
         Height          =   375
         Left            =   480
         TabIndex        =   7
         Top             =   720
         Width           =   1455
      End
   End
   Begin VB.TextBox txtYield 
      Height          =   375
      Left            =   3000
      TabIndex        =   1
      Top             =   240
      Width           =   1095
   End
   Begin VB.CheckBox chkYield 
      Caption         =   "Continuous rate: %"
      Height          =   375
      Left            =   360
      TabIndex        =   0
      Top             =   240
      Width           =   1695
   End
End
Attribute VB_Name = "frmVSCDivs"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public m_Yield As Double
Public useYield As Boolean
Private DivType As EtsDivTypeEnum
Private DivDates() As Date
Private DivAmounts() As Double
Public mDivDate As Date
Public mDivAmount As Double
Public mDivFreq As Long
Public bOk As Boolean
Public DivCount As Long
Private Const CB_FINDSTRING = &H14C
Private inSetupUI As Boolean

Public Sub Init(pYield As Double, pDivDates() As Double, pDivAmounts() As Double, pDivDate As Date, _
                pDivAmount As Double, pDivType As EtsDivTypeEnum, pDivFreq As Long, pDivCount As Long)
    inSetupUI = False
    bOk = False
    m_Yield = pYield
    DivType = pDivType
    dtpDivDate.MinDate = DateAdd("yyyy", -5, Date)
    DivCount = pDivCount
    If DivCount > 0 Then
        ReDim DivDates(UBound(pDivDates))
        ReDim DivAmounts(UBound(pDivDates))
        For i = LBound(DivDates) To UBound(DivDates)
            DivDates(i) = DateAdd("d", pDivDates(i) * 365, Date)
            DivAmounts(i) = pDivAmounts(i)
        Next i
    End If
    useYield = (m_Yield > 0)
    mDivDate = pDivDate
    mDivAmount = pDivAmount
    mDivFreq = pDivFreq
    cmbFreq.AddItem "Unknown", 0
    cmbFreq.AddItem "Monthly", 1
    cmbFreq.AddItem "Quoterly", 2
    cmbFreq.AddItem "Semiannual", 3
    cmbFreq.AddItem "Annual", 4
    SetupUI
End Sub
Private Sub SetupFrequencyCombo()
    Dim Idx As Long, searchString As String
    
    Select Case mDivFreq
        Case 12
            searchString = "Monthly"
        Case 4
            searchString = "Quoterly"
        Case 2
            searchString = "Semiannual"
        Case 1
            searchString = "Annual"
        Case Else
            searchString = "Unknown"
    End Select
    Idx = SendMessage(cmbFreq.hWnd, CB_FINDSTRING, -1, ByVal searchString)
    If Idx <> -1 Then cmbFreq.ListIndex = Idx
End Sub
Private Sub SetupUI()
    inSetupUI = True
    If useYield = True Then
        chkYield.Value = 1
        txtYield.Text = m_Yield
        txtYield.Enabled = True
        btnStream.Enabled = False
        chkPeriodic.Enabled = False
        frmDiscrete.Enabled = False
        lblDivAmount.Enabled = False
        lblDivDate.Enabled = False
        lblDivFreq.Enabled = False
        dtpDivDate.Enabled = False
        txtAmount.Enabled = False
        cmbFreq.Enabled = False
    Else
        chkYield.Value = False
        txtYield.Text = ""
        txtYield.Enabled = False
        frmDiscrete.Enabled = True
        If DivType <> enDivCustomStream Then
            btnStream.Enabled = False
            chkPeriodic.Enabled = True
            chkPeriodic.Value = 1
            lblDivAmount.Enabled = True
            lblDivDate.Enabled = True
            lblDivFreq.Enabled = True
            dtpDivDate.Enabled = True
            frmDiscrete.Enabled = True
            txtAmount.Enabled = True
            cmbFreq.Enabled = True
            dtpDivDate.Value = IIf(mDivDate < Date, Date, mDivDate)
            txtAmount.Text = mDivAmount
            SetupFrequencyCombo
        Else
            frmDiscrete.Enabled = True
            chkPeriodic.Enabled = True
            chkPeriodic.Value = 0
            btnStream.Enabled = True
            lblDivAmount.Enabled = False
            lblDivDate.Enabled = False
            lblDivFreq.Enabled = False
            dtpDivDate.Enabled = False
            txtAmount.Enabled = False
            cmbFreq.Enabled = False
        End If
    End If
    inSetupUI = False
End Sub

Private Sub btnCancel_Click()
    Unload Me
End Sub

Private Sub btnOk_Click()
    If useYield = True Then
        If IsNumeric(txtYield.Text) Then
            If CDbl(txtYield.Text) <= 0# Then
                MsgBox "Incorrect value for yield", vbExclamation
                txtYield.SetFocus
                Exit Sub
            Else
                m_Yield = CDbl(txtYield.Text)
            End If
        Else
            MsgBox "Incorrect value for yield", vbExclamation
            txtYield.SetFocus
            Exit Sub
        End If
    Else
        If DivType = enDivMarket Then
            If IsNumeric(txtAmount.Text) Then
                If CDbl(txtAmount.Text) <= 0# Then
                    MsgBox "Incorrect value for amount", vbExclamation
                    txtAmount.SetFocus
                    Exit Sub
                End If
                mDivAmount = CDbl(txtAmount.Text)
                mDivDate = dtpDivDate.Value
            Else
                MsgBox "Incorrect value for amount", vbExclamation
                txtAmount.SetFocus
                Exit Sub
            End If
        End If
    End If
    bOk = True
    Unload Me
End Sub

Private Sub btnStream_Click()
    Dim Dates() As Date, Amounts() As Double
    
    Load frmVSCDivsStream
    frmVSCDivsStream.Init DivDates, DivAmounts, DivCount
    frmVSCDivsStream.Show vbModal
    If frmVSCDivsStream.bOk = True Then
        DivCount = frmVSCDivsStream.GetDivsStream(DivDates, DivAmounts)
    End If
End Sub

Private Sub chkPeriodic_Click()
    If chkPeriodic.Value = 0 Then
        DivType = enDivCustomStream
    Else
        DivType = enDivMarket
    End If
    If inSetupUI = False Then SetupUI
End Sub

Private Sub chkYield_Click()
    useYield = IIf(chkYield.Value = 0, False, True)
    If inSetupUI = False Then SetupUI
End Sub

Public Function GetDivsStream(pDates() As Date, pAmounts() As Double) As Long
    Dim i As Long
    
    GetDivsStream = 0
    If DivCount > 0 And DivType <> enDivMarket Then
        GetDivsStream = DivCount
        ReDim pDates(UBound(DivDates) - LBound(DivDates))
        ReDim pAmounts(UBound(DivDates) - LBound(DivDates))
        For i = LBound(DivDates) To UBound(DivDates)
            pDates(i) = DivDates(i)
            pAmounts(i) = DivAmounts(i)
        Next
    End If
End Function

Private Sub cmbFreq_Click()
    Select Case cmbFreq.ListIndex
        Case 1
            mDivFreq = 12
        Case 2
            mDivFreq = 4
        Case 3
            mDivFreq = 2
        Case 4
            mDivFreq = 1
        Case Else
            mDivFreq = 0
   End Select
End Sub
