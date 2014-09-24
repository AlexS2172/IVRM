VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmOrderEditor 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Order Editor"
   ClientHeight    =   2370
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5895
   Icon            =   "frmOrderEditor.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2370
   ScaleWidth      =   5895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Tag             =   "text"
   Begin VB.Timer tmrTradeDate 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   5040
      Top             =   720
   End
   Begin VB.Timer tmrPrice 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   5400
      Top             =   720
   End
   Begin VB.CommandButton btnSubmit 
      Caption         =   "Submit"
      Default         =   -1  'True
      Height          =   315
      Left            =   3000
      TabIndex        =   5
      Top             =   1920
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   4440
      TabIndex        =   6
      Top             =   1920
      Width           =   1335
   End
   Begin EtsMM.ctlLabelEx lblTotalPrice 
      Height          =   315
      Left            =   3480
      TabIndex        =   7
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   1515
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   556
      BackColor       =   -2147483633
      Enabled         =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      Caption         =   "0"
      DisabledForeColor=   -2147483640
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblTradeDate 
      Height          =   315
      Left            =   120
      TabIndex        =   8
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   1485
      Width           =   1485
      _ExtentX        =   2619
      _ExtentY        =   556
      BackColor       =   -2147483633
      Enabled         =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      Caption         =   "0"
      DisabledForeColor=   -2147483640
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblTradeTime 
      Height          =   315
      Left            =   1800
      TabIndex        =   9
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   1485
      Width           =   1485
      _ExtentX        =   2619
      _ExtentY        =   556
      BackColor       =   -2147483633
      Enabled         =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      Caption         =   "0"
      DisabledForeColor=   -2147483640
      HoverEffect     =   0
   End
   Begin VB.Label lblStatus 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Last quotes request..."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   18
      Top             =   1920
      Visible         =   0   'False
      Width           =   2295
   End
   Begin MSForms.TextBox txtLotSize 
      CausesValidation=   0   'False
      Height          =   315
      Left            =   2520
      TabIndex        =   3
      Top             =   930
      Width           =   735
      VariousPropertyBits=   679495705
      BackColor       =   16119285
      Size            =   "1296;556"
      BorderColor     =   -2147483640
      FontEffects     =   1073750016
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.TextBox txtQuantity 
      Height          =   315
      Left            =   3480
      TabIndex        =   4
      Top             =   930
      Width           =   1215
      VariousPropertyBits=   679495707
      BackColor       =   16119285
      Size            =   "2143;556"
      Value           =   "10"
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.TextBox txtPrice 
      Height          =   315
      Left            =   1200
      TabIndex        =   2
      Top             =   930
      Width           =   1095
      VariousPropertyBits=   679495707
      BackColor       =   16119285
      Size            =   "1931;556"
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxContract 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   345
      Width           =   5655
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "9975;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxBuySell 
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   930
      Width           =   855
      VariousPropertyBits=   547375131
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "1508;556"
      ListRows        =   2
      MatchEntry      =   0
      ShowDropButtonWhen=   2
      Value           =   "Buy"
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Total Price:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   5
      Left            =   3480
      TabIndex        =   17
      Top             =   1320
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Quantity:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   4
      Left            =   3480
      TabIndex        =   16
      Top             =   705
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Lot Size:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   2520
      TabIndex        =   15
      Top             =   705
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Price:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   1200
      TabIndex        =   14
      Top             =   705
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Buy/Sell:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   13
      Top             =   705
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Contract:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   12
      Top             =   120
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trade Date:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   6
      Left            =   120
      TabIndex        =   11
      Top             =   1290
      Width           =   975
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trade Time:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   7
      Left            =   1800
      TabIndex        =   10
      Top             =   1290
      Width           =   975
   End
End
Attribute VB_Name = "frmOrderEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'Option Compare Text
'
''Private m_NewTrd As clsTradeEditor
'Private WithEvents PriceProvider As ISEPROVIDERLib.ISEPriceProvider
'Private MmManager As ISEPROVIDERLib.ISEMMManager
'
'Private m_Contracts As New Collection
'Private m_frmOwner As Form
'Private m_bIsOpened As Boolean
'
'Public Sub NewOrder(ByRef Und As clsQnUndAtom, ByVal nContractID As Long, ByVal bIsBuy As Boolean, frmOwner As Form)
'    On Error GoTo EH
'    If m_bIsOpened Then Exit Sub
'    m_bIsOpened = True
'
'    Dim sOptString As String
'
'    Dim aExpiry As clsQnExpAtom, aStrike As clsQnStrikeAtom, aOption As clsQnOptAtom
'
'    Screen.MousePointer = vbHourglass
'    Set m_frmOwner = frmOwner
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
'
'    Load Me
'
'    lblTradeDate.Caption = Format$(Now, "Short Date")
'    lblTradeTime.Caption = Format$(Now, "Long Time")
'
'    '
'    Set PriceProvider = New ISEPROVIDERLib.ISEPriceProvider
'    Set MmManager = New ISEPROVIDERLib.ISEMMManager
'    PriceProvider.Connect
'    MmManager.Connect
'
'    ' buy / sell flag
'    cbxBuySell.List = Array("Buy", "Sell")
'    cbxBuySell.ListIndex = IIf(bIsBuy, 0, 1)
'
'    ' contracts load
'    For Each aExpiry In Und.Expiry
'        For Each aStrike In aExpiry.Strike
'            Set aOption = aStrike.aOption(enOtCall)
'            If Not aOption Is Nothing And Not aOption.DefQuote Is Nothing Then
'                sOptString = aOption.Symbol & vbTab & "(" & Und.Symbol & " " & aOption.DefQuote.Series & " Call)"
'                cbxContract.AddItem sOptString
'                m_Contracts.Add aOption, sOptString
'                If aOption.ID = nContractID Then cbxContract.Value = sOptString
'            End If
'
'            Set aOption = aStrike.aOption(enOtPut)
'            If Not aOption Is Nothing And Not aOption.DefQuote Is Nothing Then
'                sOptString = aOption.Symbol & vbTab & "(" & Und.Symbol & " " & aOption.DefQuote.Series & " Put)"
'                cbxContract.AddItem sOptString
'                m_Contracts.Add aOption, sOptString
'                If aOption.ID = nContractID Then cbxContract.Value = sOptString
'            End If
'        Next
'    Next
'
'    Me.Caption = "New Order"
'    tmrTradeDate.Enabled = True
'    Me.Show vbModeless, m_frmOwner
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to create new order."
'    On Error Resume Next
'    Unload Me
'End Sub
'
'Private Sub CancelLastQuote()
'    On Error Resume Next
'    lblStatus.Visible = False
'    PriceProvider.CancelLastQuote
'End Sub
'
'Private Sub cbxContract_Change()
'    On Error GoTo EH
'    Dim aOption As clsQnOptAtom
'    Dim aQuote As ISEPROVIDERLib.QuoteUpdateParams
'
'    Set aOption = m_Contracts(cbxContract.Value)
'
'    CancelLastQuote
'
'    ' update lot size
'    txtLotSize = aOption.DefQuote.LotSize
'
'    ' request last quote
'    aQuote.Exchange = ""
'    aQuote.Symbol = aOption.Symbol
'    aQuote.Type = enOPT
'    lblStatus.Visible = True
'    PriceProvider.RequestLastQuote aQuote
'    Exit Sub
'EH:
'    LogEvent EVENT_ERROR, "Fail to request last quotes: " & Err.Description & " (" & Err.Source & ")"
'End Sub
'
'Private Sub PriceProvider_OnLastQuote(Params As ISEPROVIDERLib.QuoteUpdateParams, Results As ISEPROVIDERLib.ISEQuoteUpdateResult)
'    On Error GoTo EH
'    Dim aOption As clsQnOptAtom, dValue#
'
'    If lblStatus.Visible = False Then
'        Exit Sub
'    End If
'
'    CancelLastQuote
'
'    Set aOption = m_Contracts(cbxContract.Value)
'    If Params.Symbol = aOption.Symbol Then
'        If cbxBuySell.Value = "Sell" Then
'            dValue = Results.AskPrice
'        Else
'            dValue = Results.BidPrice
'        End If
'        If dValue <= BAD_DOUBLE_VALUE Then dValue = 0#
'        txtPrice.Value = PutNum(dValue, "#,##0.00")
'    End If
'EH:
'End Sub
'
'Private Sub tmrTradeDate_Timer()
'    On Error Resume Next
'        lblTradeDate.Caption = Format$(Now, "Short Date")
'        lblTradeTime.Caption = Format$(Now, "Long Time")
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
''    gCmn.CustomizeForm Me
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    CleanUp
'End Sub
'
'Private Sub CleanUp()
'    On Error Resume Next
'    tmrTradeDate.Enabled = False
'
'    Set m_Contracts = Nothing
'    Set m_Contracts = New Collection
'
'    If Not PriceProvider Is Nothing Then
'        PriceProvider.CancelLastQuote
'        Set PriceProvider = Nothing
'    End If
'
'    If Not MmManager Is Nothing Then
'        Set MmManager = Nothing
'    End If
'
'    If Not m_frmOwner Is Nothing Then
'        m_frmOwner.Enabled = True
'        m_frmOwner.SetFocus
'    End If
'
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
'End Sub
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
'    CancelLastQuote
'    Unload Me
'End Sub
'
'Private Sub btnSubmit_Click()
'    On Error GoTo EH
'    Dim Price As Double
'    Dim Quantity As Long
'    Dim aOrder As ISEPROVIDERLib.ISEOrder
'    Dim aOption As clsQnOptAtom
'
'    Price = Abs(GetNum(txtPrice.Value))
'    Quantity = Abs(GetNum(txtQuantity.Value))
'
'    If Price <= 0 Then
'        gCmn.MyMsgBox Me, "Please specify valid trade price.", vbCritical
'        txtPrice.SetFocus
'        Exit Sub
'    End If
'
'    If Quantity <= 0 Then
'        gCmn.MyMsgBox Me, "Please specify valid trade quantity.", vbCritical
'        txtQuantity.SetFocus
'        Exit Sub
'    End If
'
'    ' fill order
'    Set aOption = m_Contracts(cbxContract.Value)
'    aOrder.Series = aOption.Symbol
'    aOrder.Price = Price
'    aOrder.Quantity = Quantity
'    If cbxBuySell.Value = "Sell" Then
'        aOrder.BidOrAsk = enAsk
'    Else
'        aOrder.BidOrAsk = enBid
'    End If
'
'    ' send order
'    MmManager.EnterOrder aOrder
'    CancelLastQuote
'    Unload Me
'
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to submit order."
'End Sub
'
'Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
'    On Error Resume Next
'    If lblStatus.Visible = False Then
'        Exit Sub
'    End If
'
'    CancelLastQuote
'
'    If ReqType = enRequestLastQuote Then
'        LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
'    End If
'End Sub
'
'Private Sub txtPrice_Change()
'    On Error Resume Next
'    CancelLastQuote
'    UpdateTotalPrice
'End Sub
'
'Private Sub UpdateTotalPrice()
'    Dim Price As Double
'    Dim TotalPrice As Double
'    Dim Quantity As Long
'    Dim TradeLotSize As Long
'
'    On Error Resume Next
'    Price = Abs(GetNum(txtPrice.Value))
'    Quantity = Abs(GetNum(txtQuantity.Value))
'    TradeLotSize = Abs(GetNum(txtLotSize.Value))
'
'    TotalPrice = Price * Quantity * TradeLotSize
'    lblTitle(2).ForeColor = IIf(Price > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
'    lblTitle(4).ForeColor = IIf(Quantity > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
'
'    lblTotalPrice.Caption = PutNum(TotalPrice, "#,##0.00")
'End Sub
'
'Private Sub txtPrice_Validate(Cancel As Boolean)
'    On Error Resume Next
'    Dim s As String, s1 As String, w As Double, Price As Double, Value As String
'    s = txtPrice.Value
'    w = Abs(GetNum(s))
'    s1 = PutNum(w, "#,##0.00")
'    If s1 <> s Then
'        Price = Abs(w)
'        Value = s1
'        lblTitle(2).ForeColor = IIf(Price > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
'    End If
'End Sub
'
'Private Sub txtQuantity_Change()
'    On Error Resume Next
'    CancelLastQuote
'    UpdateTotalPrice
'End Sub
'
'
'Private Sub txtQuantity_Validate(Cancel As Boolean)
'    On Error Resume Next
'    Dim s As String, s1 As String, w As Double, Quantity As Double
'    s = txtQuantity.Value
'    w = CLng(Abs(GetNum(s)))
'    s1 = PutNum(w, "#,##0")
'    If s1 <> s Then
'        Quantity = Abs(w)
'        txtQuantity.Value = s1
'        lblTitle(4).ForeColor = IIf(Quantity > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
'    End If
'End Sub
'
'Private Function GetNum(ByVal sNumber As String) As Double
'    On Error Resume Next
'    Dim s As String
'    sNumber = Trim$(sNumber)
'    If Not IsNumeric(sNumber) Then
'        s = Right$(sNumber, 1)
'        If s = "%" Then
'            sNumber = Left$(sNumber, Len(sNumber) - 1)
'            If Not IsNumeric(sNumber) Then
'                GetNum = Val(sNumber)
'            Else
'                GetNum = CDbl(sNumber) / 100
'            End If
'        Else
'            GetNum = Val(sNumber)
'        End If
'    Else
'        GetNum = CDbl(sNumber)
'    End If
'End Function
'
'
'Private Function PutNum(fNumber As Double, Optional sFormat As String = "") As String
'    On Error Resume Next
'    PutNum = Format$(fNumber, sFormat)
'End Function
'
