VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmTntCardEntry 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Trade Card Entry"
   ClientHeight    =   5625
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8850
   Icon            =   "frmTntCardEntry.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5625
   ScaleWidth      =   8850
   StartUpPosition =   1  'CenterOwner
   Begin VB.Frame fraAffectPos 
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   6480
      TabIndex        =   25
      Top             =   4080
      Width           =   2175
      Begin VB.CheckBox chkAffectPos 
         Caption         =   "Affect Position"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   10
         Top             =   210
         Width           =   1995
      End
   End
   Begin VB.Timer tmrTime 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   6120
      Top             =   5220
   End
   Begin VB.CheckBox btnSend 
      Caption         =   "Send Card"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00008000&
      Height          =   615
      Left            =   180
      Style           =   1  'Graphical
      TabIndex        =   11
      Top             =   4860
      Width           =   6195
   End
   Begin VB.Frame fraAction 
      Caption         =   " Action "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1335
      Left            =   180
      TabIndex        =   23
      Top             =   1020
      Width           =   6195
      Begin VB.OptionButton optAction 
         Caption         =   "SELL"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H000000C0&
         Height          =   870
         Index           =   0
         Left            =   3120
         Style           =   1  'Graphical
         TabIndex        =   3
         Top             =   300
         Width           =   2925
      End
      Begin VB.OptionButton optAction 
         Caption         =   "BUY"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FF0000&
         Height          =   870
         Index           =   1
         Left            =   120
         Style           =   1  'Graphical
         TabIndex        =   2
         Top             =   300
         Value           =   -1  'True
         Width           =   2925
      End
   End
   Begin VB.Frame fraStock 
      Caption         =   " Stock "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   915
      Left            =   180
      TabIndex        =   22
      Top             =   2400
      Width           =   2355
      Begin VB.ComboBox cmbStock 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   480
         Left            =   120
         Sorted          =   -1  'True
         Style           =   2  'Dropdown List
         TabIndex        =   4
         Top             =   300
         Width           =   2085
      End
   End
   Begin VB.Frame fraOption 
      Caption         =   " Option "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   915
      Left            =   2700
      TabIndex        =   21
      Top             =   2400
      Width           =   3675
      Begin VB.ComboBox cmbOption 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   480
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   5
         Top             =   300
         Width           =   3435
      End
   End
   Begin VB.Frame fraTrader 
      Caption         =   " Trader "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   915
      Left            =   180
      TabIndex        =   20
      Top             =   60
      Width           =   2655
      Begin VB.TextBox txtTrader 
         BackColor       =   &H8000000F&
         Enabled         =   0   'False
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   120
         TabIndex        =   1
         Top             =   300
         Width           =   2415
      End
   End
   Begin VB.Frame fraQty 
      Caption         =   " Quantity "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1335
      Left            =   180
      TabIndex        =   17
      Top             =   3360
      Width           =   2355
      Begin MSComCtl2.UpDown udQty 
         Height          =   480
         Left            =   2010
         TabIndex        =   18
         TabStop         =   0   'False
         Top             =   300
         Width           =   195
         _ExtentX        =   344
         _ExtentY        =   847
         _Version        =   393216
         OrigLeft        =   2010
         OrigTop         =   300
         OrigRight       =   2205
         OrigBottom      =   780
         Wrap            =   -1  'True
         Enabled         =   -1  'True
      End
      Begin EtsMMCv.ctlEditEx txtQty 
         Height          =   480
         Left            =   120
         TabIndex        =   6
         Top             =   300
         Width           =   1890
         _ExtentX        =   3334
         _ExtentY        =   847
         MouseIcon       =   "frmTntCardEntry.frx":0442
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HideSelection   =   -1  'True
      End
      Begin VB.Label lblLotSize 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Caption         =   "Lot Size : 100"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   19
         Top             =   870
         Width           =   2085
      End
   End
   Begin VB.Frame fraPrice 
      Caption         =   " Price "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1335
      Left            =   2700
      TabIndex        =   15
      Top             =   3360
      Width           =   3675
      Begin EtsMMCv.ctlEditEx txtPrice 
         Height          =   480
         Left            =   120
         TabIndex        =   7
         Top             =   300
         Width           =   3225
         _ExtentX        =   5689
         _ExtentY        =   847
         MouseIcon       =   "frmTntCardEntry.frx":045E
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HideSelection   =   -1  'True
      End
      Begin MSComCtl2.UpDown udPrice 
         Height          =   480
         Left            =   3360
         TabIndex        =   16
         TabStop         =   0   'False
         Top             =   300
         Width           =   195
         _ExtentX        =   344
         _ExtentY        =   847
         _Version        =   393216
         OrigLeft        =   2010
         OrigTop         =   300
         OrigRight       =   2205
         OrigBottom      =   780
         Wrap            =   -1  'True
         Enabled         =   -1  'True
      End
      Begin VB.Label lblTotPrice 
         Alignment       =   2  'Center
         BorderStyle     =   1  'Fixed Single
         Caption         =   "Total Price : 100"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   24
         Top             =   870
         Width           =   3435
      End
   End
   Begin VB.Frame fraCpt 
      Caption         =   " CounterParty "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3975
      Left            =   6480
      TabIndex        =   14
      Top             =   60
      Width           =   2175
      Begin VB.ListBox lstCpt 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3000
         IntegralHeight  =   0   'False
         ItemData        =   "frmTntCardEntry.frx":047A
         Left            =   120
         List            =   "frmTntCardEntry.frx":047C
         Sorted          =   -1  'True
         TabIndex        =   9
         Top             =   840
         Width           =   1935
      End
      Begin VB.TextBox txtCpt 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   495
         Left            =   120
         TabIndex        =   8
         Top             =   300
         Width           =   1935
      End
   End
   Begin VB.Frame fraDateTime 
      Caption         =   " Date/Time "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   915
      Left            =   3000
      TabIndex        =   0
      Top             =   60
      Width           =   3375
      Begin VB.Label lblDateTime 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "12/25/2003 12:55:55 AM"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   12
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Left            =   60
         TabIndex        =   13
         Top             =   390
         Width           =   3255
      End
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   13.5
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   615
      Left            =   6480
      TabIndex        =   12
      Top             =   4860
      Width           =   2175
   End
End
Attribute VB_Name = "frmTntCardEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Private Const MAXLEN_TNT_COUNTERPARTY& = 4&
'Private Const MAXLEN_TNT_COUNTERPARTY_1& = 5&
'
'' defaults
'Private m_nDefCntPtyID&
'Private m_bDefAffectPos As Boolean
'
'' states & supplementary
'Private m_bIsOpened As Boolean
'Private m_bInitNow As Boolean
'Private m_nCurQtyValueIdx&
'Private m_nCurPrcValueIdx&
'
'' order datas
'Private m_Opts As EtsGeneralLib.EtsOptColl
'
'Private m_Crd As EtsMmGeneralLib.MmTntCardAtom
'Private m_sCntPtyName$
'
'Public Sub NewCard(ByRef frmOwner As Form, ByRef aUnd As EtsGeneralLib.UndAtom, _
'                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
'                        ByVal bIsBuy As Boolean, ByVal nQty&, ByVal dPrice#)
'    On Error GoTo EH
'
'    If Not g_TntProcessor.CheckUsingTngMessagingWithWarning(frmOwner) Then Exit Sub
'
'    Screen.MousePointer = vbHourglass
'
'    m_bInitNow = True
'
'    If Not m_bIsOpened Then Load Me
'
'    Me.Caption = "New Trade Message"
'
'    Set m_Crd = Nothing
'    Set m_Crd = New EtsMmGeneralLib.MmTntCardAtom
'
'    If aUnd Is Nothing Then Set aOpt = Nothing
'
'    If aOpt Is Nothing Then bIsBuy = False: dPrice = 0#: nQty = 10&
'
'    m_Crd.Account = g_TntProcessor.CurTraderAccount
'    m_Crd.IsBuy = bIsBuy
'    m_Crd.Qty = Abs(nQty)
'    If m_Crd.Qty = 0 Then m_Crd.Qty = 10
'    m_Crd.Price = IIf(dPrice <= 0#, 0#, dPrice)
'
'    lblDateTime.Caption = Now
'
'    Set m_Crd.Und = aUnd
'    Set m_Crd.Opt = aOpt
'    If Not m_Crd.Und Is Nothing And Not m_Crd.Opt Is Nothing Then Set m_Crd.OptRoot = m_Crd.Und.Roots(m_Crd.Opt.RootID)
'
'    InitNewCardCntPty
'
'    LoadData
'
'    m_bInitNow = False
'
'    tmrTime.Enabled = True
'    Me.Show vbModeless ', frmOwner
'
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to create new trade message."
'    On Error Resume Next
'    m_bInitNow = False
'    Set m_Crd = Nothing
'    Set m_Opts = Nothing
'    Unload Me
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub LoadData()
'    On Error Resume Next
'    optAction(1).Value = m_Crd.IsBuy
'    optAction(0).Value = Not optAction(1).Value
'
'    txtQty.Text = m_Crd.Qty
'    txtPrice.Text = m_Crd.Price
'
'    cmbStock.Clear
'    cmbOption.Clear
'
'    txtTrader.Text = m_Crd.Account
'
'    chkAffectPos.Value = IIf(m_bDefAffectPos, 1, 0)
'
'    FillStocksCombo
'
'    If Not m_Crd.Und Is Nothing Then
'        cmbStock.AddItem m_Crd.Und.Symbol
'        cmbStock.ItemData(cmbStock.NewIndex) = m_Crd.Und.ID
'        cmbStock.ListIndex = cmbStock.NewIndex
'
'        If Not m_Crd.Opt Is Nothing And Not m_Crd.OptRoot Is Nothing Then
'            AdjustOptSeries m_Crd.Opt, m_Crd.OptRoot
'
'            cmbOption.AddItem m_Crd.Opt.Symbol & " " & Format$(m_Crd.Opt.Expiry, "MMMYY") & " " & _
'                         Trim$(Str$(m_Crd.Opt.Strike)) & " " & IIf(m_Crd.Opt.OptType = enOtCall, "C", "P")
'
'            cmbOption.ItemData(cmbOption.NewIndex) = m_Crd.Opt.ID
'            cmbOption.ListIndex = cmbOption.NewIndex
'        Else
'            Set m_Crd.Opt = Nothing
'            Set m_Crd.OptRoot = Nothing
'            Set m_Opts = Nothing
'        End If
'    Else
'        Set m_Crd.Opt = Nothing
'        Set m_Crd.OptRoot = Nothing
'        Set m_Opts = Nothing
'    End If
'
'    LoadCounterParties
'
'    AdjustLotSize
'    AdjustTotalPrice
'    AdjustCardSendAvailability
'End Sub
'
'Private Sub LoadCounterParties()
'    On Error Resume Next
'    Dim aCntPty As EtsMmGeneralLib.MmTntCounterPartyAtom
'
'    lstCpt.Clear
'    txtCpt.Text = ""
'
'    For Each aCntPty In g_TntCntPty
'        lstCpt.AddItem aCntPty.Name
'        lstCpt.ItemData(lstCpt.NewIndex) = aCntPty.ID
'
'        If aCntPty.Name = m_sCntPtyName Then
'            lstCpt.ListIndex = lstCpt.NewIndex
'            txtCpt.Text = m_sCntPtyName
'        End If
'    Next
'End Sub
'
'Private Sub InitNewCardCntPty()
'    On Error Resume Next
'    ' take last cntpty
'    m_sCntPtyName = g_TntCntPty(m_nDefCntPtyID).Name
'    If Len(m_sCntPtyName) <= 0 Then
'        ' take first cntpty
'        Dim aCntPty As EtsMmGeneralLib.MmTntCounterPartyAtom
'        For Each aCntPty In g_TntCntPty
'            m_sCntPtyName = UCase$(aCntPty.Name)
'            Exit For
'        Next
'    End If
'End Sub
'
'Private Sub AdjustLotSize()
'    On Error Resume Next
'    Dim sLotSize$: sLotSize = "N/A"
'
'    If Not m_Crd.Opt Is Nothing And Not Not m_Crd.OptRoot Then
'        sLotSize = CStr(m_Crd.OptRoot.LotSize)
'    End If
'
'    lblLotSize.Caption = "Lot Size : " & sLotSize
'End Sub
'
'Private Sub AdjustTotalPrice()
'    On Error Resume Next
'    Dim sTotPrc$: sTotPrc = "N/A"
'
'    If Not m_Crd.Opt Is Nothing And Not Not m_Crd.OptRoot Then
'        sTotPrc = Format$(m_Crd.Price * m_Crd.Qty * m_Crd.OptRoot.LotSize, "#,##0.00")
'    End If
'
'    lblTotPrice.Caption = "Total Price : " & sTotPrc
'End Sub
'
'Private Sub cmbOption_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim nOptID&
'
'    If cmbOption.ListIndex > -1 And Not m_Opts Is Nothing Then
'        nOptID = cmbOption.ItemData(cmbOption.ListIndex)
'
'        If Not m_Crd.Opt Is Nothing Then
'            If nOptID = m_Crd.Opt.ID Then Exit Sub
'        End If
'
'        Set m_Crd.Opt = m_Opts(nOptID)
'        Set m_Crd.OptRoot = m_Crd.Und.Roots(m_Crd.Opt.RootID)
'
'        AdjustOptSeries m_Crd.Opt, m_Crd.OptRoot
'    Else
'        Set m_Crd.Opt = Nothing
'        Set m_Crd.OptRoot = Nothing
'    End If
'
'    AdjustLotSize
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
'Private Sub cmbOption_DropDown()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim rs As ADODB.Recordset, aOpt As EtsGeneralLib.EtsOptAtom, nOptID&, nIdx&
'
'    If Not m_Crd.Und Is Nothing Then
'        If m_Opts Is Nothing Then
'            Screen.MousePointer = vbHourglass
'            cmbOption.Clear
'
'            Set m_Opts = New EtsGeneralLib.EtsOptColl
'            Set rs = gDBW.usp_UnderlyingOptionsForOrder_Get(m_Crd.Und.ID)
'
'            While Not rs.EOF
'                nOptID = ReadLng(rs!iOptionID)
'                If m_Opts(nOptID) Is Nothing Then
'                    Set aOpt = m_Opts.Add(nOptID, Trim$(Str$(nIdx)))
'                    nIdx = nIdx + 1
'
'                    aOpt.ID = nOptID
'                    aOpt.RootID = ReadLng(rs!iOptionRootID)
'                    aOpt.Symbol = ReadStr(rs!vcSymbol)
'                    aOpt.OptType = IIf(ReadByte(rs!tiIsCall) <> 0, enOtCall, enOtPut)
'                    aOpt.Expiry = ReadDate(rs!dtExpiry)
'                    aOpt.Strike = ReadDbl(rs!fStrike)
'
'                    cmbOption.AddItem aOpt.Symbol & " " & Format$(aOpt.Expiry, "MMMYY") & " " & _
'                                 Trim$(Str$(aOpt.Strike)) & " " & IIf(aOpt.OptType = enOtCall, "C", "P")
'
'                    cmbOption.ItemData(cmbOption.NewIndex) = nOptID
'                    If Not m_Crd.Opt Is Nothing Then
'                        If m_Crd.Opt.ID = nOptID Then
'                            m_bInitNow = True
'                            cmbOption.ListIndex = cmbOption.NewIndex
'                            m_bInitNow = False
'                        End If
'                    End If
'
'                    Set aOpt = Nothing
'                End If
'                rs.MoveNext
'            Wend
'
'            Set rs = Nothing
'            Screen.MousePointer = vbDefault
'        End If
'    Else
'        Set m_Crd.Opt = Nothing
'        Set m_Opts = Nothing
'        Set m_Crd.OptRoot = Nothing
'        cmbOption.Clear
'    End If
'End Sub
'
'Private Sub FillStocksCombo()
'    On Error Resume Next
'    Dim aUnd As EtsGeneralLib.UndAtom, nUndID&
'
'    Screen.MousePointer = vbHourglass
'    If Not m_Crd.Und Is Nothing Then nUndID = m_Crd.Und.ID
'
'    cmbStock.Clear
'
'    For Each aUnd In g_Underlying
'        cmbStock.AddItem aUnd.Symbol
'        cmbStock.ItemData(cmbStock.NewIndex) = aUnd.ID
'        If aUnd.ID = nUndID Then
'            If m_bInitNow Then
'                cmbStock.ListIndex = cmbStock.NewIndex
'            Else
'                m_bInitNow = True
'                cmbStock.ListIndex = cmbStock.NewIndex
'                m_bInitNow = False
'            End If
'        End If
'    Next
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub cmbStock_DropDown()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    If cmbStock.ListCount <> g_Underlying.Count Then
'        FillStocksCombo
'    End If
'End Sub
'
'Private Sub cmbStock_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim nUndID&
'
'    If cmbStock.ListIndex > -1 Then
'        nUndID = cmbStock.ItemData(cmbStock.ListIndex)
'        If Not m_Crd.Und Is Nothing Then
'            If m_Crd.Und.ID = nUndID Then Exit Sub
'        End If
'
'        Set m_Crd.Und = g_Underlying(nUndID)
'    Else
'        Set m_Crd.Und = Nothing
'    End If
'
'    Set m_Crd.Opt = Nothing
'    Set m_Opts = Nothing
'    Set m_Crd.OptRoot = Nothing
'    cmbOption.Clear
'
'    AdjustLotSize
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    m_bIsOpened = True
'    txtCpt.MaxLength = MAXLEN_TNT_COUNTERPARTY
'
'    SetWindowLong txtCpt.hWnd, GWL_STYLE, GetWindowLong(txtCpt.hWnd, GWL_STYLE) Or ES_UPPERCASE
'
'    m_nCurPrcValueIdx = udPrice.Value
'    m_nCurQtyValueIdx = udQty.Value
'
'    udQty.Left = 2205 - udQty.Width
'    txtQty.Width = udQty.Left - txtQty.Left
'
'    udPrice.Left = 3555 - udPrice.Width
'    txtPrice.Width = udPrice.Left - txtPrice.Left
'
'    LoadSettings
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    m_bIsOpened = False
'    m_bInitNow = False
'
'    Set m_Crd = Nothing
'    Set m_Opts = Nothing
'
'    SaveSettings
'End Sub
'
'Private Sub btnSend_Click()
'    On Error Resume Next
'    If btnSend.Value = 0 Then Exit Sub
'
'    btnSend.Value = 0
'    If m_bInitNow Then Exit Sub
'
'    If SendCard() Then Unload Me
'End Sub
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Unload Me
'End Sub
'
'Private Function SendCard() As Boolean
'    On Error Resume Next
'
'    SendCard = False
'    If Not IsCardSendAvailable Then Exit Function
'    On Error GoTo EH
'
'    Set m_Crd.CntPty = g_TntCntPtyByName(UCase$(Trim$(m_sCntPtyName)))
'    If m_Crd.CntPty Is Nothing Then
'        Set m_Crd.CntPty = New EtsMmGeneralLib.MmTntCounterPartyAtom
'        m_Crd.CntPty.ID = 0
'        m_Crd.CntPty.Name = UCase$(Trim$(m_sCntPtyName))
'    End If
'    m_Crd.MakeTrade = (chkAffectPos.Value <> 0)
'
'    g_TntProcessor.CardNew m_Crd
'    SendCard = True
'
'    m_nDefCntPtyID = m_Crd.CntPty.ID
'    m_bDefAffectPos = m_Crd.MakeTrade
'
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to send trade message: " & Err.Description
'    SendCard = False
'End Function
'
'Private Sub AdjustCardSendAvailability()
'    On Error Resume Next
'    btnSend.Enabled = IsCardSendAvailable
'End Sub
'
'Private Function IsCardSendAvailable() As Boolean
'    On Error Resume Next
'    Dim bCurDataValid As Boolean, bAllDataValid As Boolean
'
'    bAllDataValid = False
'    bCurDataValid = False
'
'    ' stock
'    bCurDataValid = (Not m_Crd.Und Is Nothing)
'    fraStock.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bCurDataValid
'
'    ' option
'    bCurDataValid = (Not m_Crd.Opt Is Nothing And Not m_Crd.OptRoot Is Nothing)
'    fraOption.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' qty
'    bCurDataValid = (m_Crd.Qty > 0)
'    fraQty.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' price
'    bCurDataValid = (m_Crd.Price > DBL_EPSILON)
'    fraPrice.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' destination
'    bCurDataValid = (Len(m_sCntPtyName) > 0)
'    fraCpt.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' trader
'    bCurDataValid = Len(Trim$(m_Crd.Account)) > 0
'    fraTrader.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    IsCardSendAvailable = bAllDataValid
'End Function
'
'Private Sub lstCpt_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_sCntPtyName = ""
'    m_sCntPtyName = lstCpt.List(lstCpt.ListIndex)
'    txtCpt.Text = m_sCntPtyName
'End Sub
'
'Private Sub optAction_Click(Index As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Crd.IsBuy = optAction(1).Value
'End Sub
'
'Private Sub tmrTime_Timer()
'    On Error Resume Next
'    lblDateTime.Caption = Now
'End Sub
'
'Private Sub LoadSettings()
'    On Error Resume Next
'    m_nDefCntPtyID = GetLongKeyValueEx(APP_REG_KEY & "TntCard Entry", "CntPtyID")
'    m_bDefAffectPos = GetLongKeyValueEx(APP_REG_KEY & "TntCard Entry", "AffectPos")
'End Sub
'
'Private Sub SaveSettings()
'    On Error Resume Next
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TntCard Entry", "CntPtyID", m_nDefCntPtyID
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "TntCard Entry", "AffectPos", IIf(m_bDefAffectPos, 1, 0)
'End Sub
'
'Private Sub txtCpt_KeyPress(KeyAscii As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'    If Chr$(KeyAscii) = " " Then KeyAscii = 0
'End Sub
'
'Private Sub txtCpt_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'    Dim sName$, nSelStart&, nCntPtyIdx&
'    Dim aCntPty As EtsMmGeneralLib.MmTntCounterPartyAtom
'    Dim sFindText$
'
'    nCntPtyIdx = LB_ERR
'
'    If KeyCode <> vbKeyBack And KeyCode > 48 Then
'        nSelStart = txtCpt.SelStart
'        sName = Left$(txtCpt.Text, nSelStart)
'
'        If Len(sName) > 0 Then
'            sFindText = sName & Chr$(0)
'            nCntPtyIdx = SendMessage(lstCpt.hWnd, LB_FINDSTRING, 0&, ByVal sFindText)
'
'            If nCntPtyIdx <> LB_ERR Then
'                Set aCntPty = g_TntCntPty(lstCpt.ItemData(nCntPtyIdx))
'
'                If Not aCntPty Is Nothing Then
'                    txtCpt.Text = aCntPty.Name
'                    txtCpt.SelStart = Len(sName) ' - 1
'                    txtCpt.SelLength = Len(txtCpt.Text) - txtCpt.SelStart
'                    Set aCntPty = Nothing
'                End If
'            End If
'        End If
'    Else
'        sName = txtCpt.Text
'
'        If Len(sName) > 0 Then
'            sFindText = sName & Chr$(0)
'            nCntPtyIdx = SendMessage(lstCpt.hWnd, LB_FINDSTRINGEXACT, 0&, ByVal sFindText)
'        End If
'    End If
'
'    m_bInitNow = True
'    lstCpt.ListIndex = nCntPtyIdx
'    m_bInitNow = False
'
'    m_sCntPtyName = UCase$(txtCpt.Text)
'
'    AdjustCardSendAvailability
'End Sub
'
'Private Sub txtPrice_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Crd.Price = 0#
'    m_Crd.Price = CDbl(txtPrice.Text)
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
'Private Sub udPrice_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim dIncrement#
'
'    If m_Crd.Price < 3# Then
'        dIncrement = g_Params.CrdDefPrcIncrementCheap
'    Else
'        dIncrement = g_Params.CrdDefPrcIncrementExpensive
'    End If
'
'    If udPrice.Value = m_nCurPrcValueIdx + 1 _
'        Or udPrice.Value = udPrice.Min And m_nCurPrcValueIdx = udPrice.Max Then
'
'        m_Crd.Price = m_Crd.Price + dIncrement
'    Else
'        If m_Crd.Price - dIncrement > 0 Then
'            m_Crd.Price = m_Crd.Price - dIncrement
'        End If
'    End If
'
'    m_nCurPrcValueIdx = udPrice.Value
'
'    txtPrice.Text = m_Crd.Price
'    EditExSelectAllAndSetFocus txtPrice
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
'Private Sub txtQty_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Crd.Qty = 0&
'    m_Crd.Qty = CLng(txtQty.Text)
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
'Private Sub udQty_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    If udQty.Value = m_nCurQtyValueIdx + 1 _
'        Or udQty.Value = udQty.Min And m_nCurQtyValueIdx = udQty.Max Then
'
'        m_Crd.Qty = m_Crd.Qty + g_Params.CrdDefQtyIncrement
'    Else
'        If m_Crd.Qty - g_Params.CrdDefQtyIncrement > 0 Then
'            m_Crd.Qty = m_Crd.Qty - g_Params.CrdDefQtyIncrement
'        End If
'    End If
'
'    m_nCurQtyValueIdx = udQty.Value
'
'    txtQty.Text = m_Crd.Qty
'    EditExSelectAllAndSetFocus txtQty
'    AdjustCardSendAvailability
'    AdjustTotalPrice
'End Sub
'
