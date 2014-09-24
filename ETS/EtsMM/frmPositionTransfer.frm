VERSION 5.00
Begin VB.Form frmPositionTransfer 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Position Transfer"
   ClientHeight    =   5460
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5715
   Icon            =   "frmPositionTransfer.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5460
   ScaleWidth      =   5715
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame fraTrade 
      Caption         =   " Transfer Amount "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   975
      Left            =   120
      TabIndex        =   21
      Top             =   2340
      Width           =   5475
      Begin EtsMM.ctlEditEx txtQuantity 
         Height          =   315
         Left            =   120
         TabIndex        =   3
         Top             =   480
         Width           =   1455
         _ExtentX        =   2566
         _ExtentY        =   556
         MouseIcon       =   "frmPositionTransfer.frx":000C
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
      Begin EtsMM.ctlEditEx txtPrice 
         Height          =   315
         Left            =   2520
         TabIndex        =   4
         Top             =   480
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   556
         MouseIcon       =   "frmPositionTransfer.frx":0028
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
      Begin VB.Label lblPriceTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Price :"
         Height          =   255
         Left            =   2520
         TabIndex        =   27
         Top             =   240
         Width           =   975
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Lot Size :"
         Height          =   255
         Index           =   4
         Left            =   1680
         TabIndex        =   26
         Top             =   240
         Width           =   735
      End
      Begin VB.Label lblQuantityTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Quantity :"
         Height          =   255
         Left            =   120
         TabIndex        =   25
         Top             =   240
         Width           =   1215
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Total Price :"
         Height          =   255
         Index           =   5
         Left            =   3600
         TabIndex        =   24
         Top             =   240
         Width           =   1755
      End
      Begin VB.Label lblLotSize 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   1680
         TabIndex        =   23
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblTotalPrice 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3600
         TabIndex        =   22
         Top             =   480
         Width           =   1755
      End
   End
   Begin VB.Frame fraTradeTo 
      Caption         =   " Transfer To (SELL Trade) "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1575
      Left            =   120
      TabIndex        =   15
      Top             =   3360
      Width           =   5475
      Begin VB.CommandButton btnStrategyToNew 
         Caption         =   "New..."
         Height          =   315
         Left            =   4560
         TabIndex        =   7
         Top             =   480
         Width           =   795
      End
      Begin VB.ComboBox cmbStrategyTo 
         Height          =   315
         Left            =   2040
         Style           =   2  'Dropdown List
         TabIndex        =   6
         Top             =   480
         Width           =   2415
      End
      Begin VB.ComboBox cmbTraderTo 
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   5
         Top             =   480
         Width           =   1815
      End
      Begin VB.CheckBox chkMarkTo 
         Caption         =   "Marked Trade"
         Height          =   255
         Left            =   3960
         TabIndex        =   8
         Top             =   1110
         Width           =   1455
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Quantity After Transfer :"
         Height          =   255
         Index           =   7
         Left            =   2040
         TabIndex        =   33
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label lblAfterQuantityTo 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   2040
         TabIndex        =   32
         Top             =   1080
         Width           =   1815
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Current Quantity :"
         Height          =   255
         Index           =   3
         Left            =   120
         TabIndex        =   29
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label lblCurQuantityTo 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   28
         Top             =   1080
         Width           =   1815
      End
      Begin VB.Label lblTraderTo 
         BackStyle       =   0  'Transparent
         Caption         =   "Trader :"
         Height          =   195
         Left            =   120
         TabIndex        =   17
         Top             =   240
         Width           =   1815
      End
      Begin VB.Label lblStrategyTo 
         BackStyle       =   0  'Transparent
         Caption         =   "Strategy :"
         Height          =   195
         Left            =   2040
         TabIndex        =   16
         Top             =   240
         Width           =   3315
      End
   End
   Begin VB.Frame fraTradeFrom 
      Caption         =   " Transfer From (BUY Trade) "
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   1575
      Left            =   120
      TabIndex        =   12
      Top             =   720
      Width           =   5475
      Begin VB.ComboBox cmbStrategyFrom 
         Height          =   315
         Left            =   2040
         Style           =   2  'Dropdown List
         TabIndex        =   1
         Top             =   480
         Width           =   2415
      End
      Begin VB.CheckBox chkMarkFrom 
         Caption         =   "Marked Trade"
         Height          =   255
         Left            =   3960
         TabIndex        =   2
         Top             =   1110
         Width           =   1455
      End
      Begin VB.ComboBox cmbTraderFrom 
         Height          =   315
         Left            =   120
         Style           =   2  'Dropdown List
         TabIndex        =   0
         Top             =   480
         Width           =   1815
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Quantity After Transfer :"
         Height          =   255
         Index           =   6
         Left            =   2040
         TabIndex        =   31
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label lblAfterQuantityFrom 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   2040
         TabIndex        =   30
         Top             =   1080
         Width           =   1815
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Current Quantity :"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   20
         Top             =   840
         Width           =   1815
      End
      Begin VB.Label lblCurQuantityFrom 
         BorderStyle     =   1  'Fixed Single
         Caption         =   "0"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   120
         TabIndex        =   19
         Top             =   1080
         Width           =   1815
      End
      Begin VB.Label lblTitle 
         BackStyle       =   0  'Transparent
         Caption         =   "Strategy :"
         Height          =   195
         Index           =   2
         Left            =   2040
         TabIndex        =   14
         Top             =   240
         Width           =   3315
      End
      Begin VB.Label lblTraderFrom 
         BackStyle       =   0  'Transparent
         Caption         =   "Trader :"
         Height          =   195
         Left            =   120
         TabIndex        =   13
         Top             =   240
         Width           =   1815
      End
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   4260
      TabIndex        =   10
      Top             =   5040
      Width           =   1335
   End
   Begin VB.CommandButton btnTransfer 
      Caption         =   "Transfer"
      Default         =   -1  'True
      Height          =   315
      Left            =   2820
      TabIndex        =   9
      Top             =   5040
      Width           =   1335
   End
   Begin VB.Label lblContractInfo 
      BorderStyle     =   1  'Fixed Single
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   315
      Left            =   120
      TabIndex        =   18
      Top             =   330
      Width           =   5475
   End
   Begin VB.Label lblContractTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Contract :"
      Height          =   195
      Left            =   120
      TabIndex        =   11
      Top             =   120
      Width           =   1020
   End
End
Attribute VB_Name = "frmPositionTransfer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_bInit As Boolean
Private m_bIsOpened As Boolean
Private m_frmOwner As Form

Private m_TraderFrom As clsPtTraderColl
Private m_TraderTo As clsPtTraderColl

Private m_PtTraderFrom As clsPtTraderAtom
Private m_TrdFrom As EtsGeneralLib.MmTradeInfoAtom
Private m_nDefTraderFromID&

Private m_PtTraderTo As clsPtTraderAtom
Private m_TrdTo As EtsGeneralLib.MmTradeInfoAtom
Private m_nDefTraderToID&
Private m_nContractID As Long


Private m_Und As EtsGeneralLib.UndAtom

Public Sub TransferPosition(ByVal nUndContractID&, ByVal nContractID&, ByVal nTraderFromID&, _
                            ByVal sContractDesc$, frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Or nUndContractID = 0 Or nContractID = 0 Or g_UnderlyingAll(nUndContractID) Is Nothing Then Exit Sub
    m_nContractID = nContractID&
        
    If Not g_Params.ActualTradesEditable Then
        gCmn.MyMsgBox Me, "Position transfer is restricted when RTTF/Uploaded trades are protected." & vbCrLf & _
                        "Please set 'RTTF/Uploaded Trades Editable' option in Parameters window (Options->Parameters...->Advanced).", vbCritical
        Exit Sub
    End If
       
    Screen.MousePointer = vbHourglass
    m_bIsOpened = True
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False

    Load Me
    ReadRegEntries

    Set m_TraderFrom = New clsPtTraderColl
    Set m_TraderTo = New clsPtTraderColl
    
    lblContractInfo.Caption = sContractDesc
    
    Init nUndContractID, nContractID, nTraderFromID
    Me.Show vbModeless, m_frmOwner

    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to transfer position."
    On Error Resume Next
    Unload Me
End Sub

Private Sub Init(ByVal nUndContractID&, ByVal nContractID&, ByVal nTraderFromID&)
    On Error Resume Next
    m_bInit = True
    
    Set m_Und = g_UnderlyingAll(nUndContractID)
    
    InitTradersList nContractID, nTraderFromID
    FillTraderCombos nTraderFromID

    AdjustTradeFrom
    FillStrategyCombo cmbStrategyFrom, m_PtTraderFrom
    
    AdjustTradeTo
    FillStrategyCombo cmbStrategyTo, m_PtTraderTo

    ShowTradeFromData
    ShowTradeToData
    
    AdjustControlsState

    m_bInit = False
End Sub

Private Sub InitTradersList(ByVal nContractID&, ByRef nTraderFromID&)
    On Error Resume Next
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl, aTrdUnd As EtsGeneralLib.EtsTraderUndAtom
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    Dim aPtTraderFrom As clsPtTraderAtom
    Dim aPtTraderTo As clsPtTraderAtom
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim eStrategy As EtsStrategyAtom

    m_TraderFrom.Clear
    m_TraderTo.Clear
    
    For Each aTrader In g_Trader

        Set aTrdByUnd = g_TradeChannel.TrdByTraderAndContract(CStr(aTrader.ID) & "_" & CStr(nContractID))
            
            Set aPtTraderTo = m_TraderTo.Add(CStr(aTrader.ID))
            Set aPtTraderTo.Trader = aTrader
                aPtTraderTo.CurQuantity = 0
        
            If Not aTrdByUnd Is Nothing Then
                Set aPtTraderFrom = m_TraderFrom.Add(CStr(aTrader.ID))
                Set aPtTraderFrom.Trader = aTrader
    
                ' init quantity & last trade data
                aPtTraderFrom.CurQuantity = BAD_LONG_VALUE
                aPtTraderTo.CurQuantity = BAD_LONG_VALUE

                For Each aTrd In aTrdByUnd
                    If aTrd.Status = enTrsRealtime Or aTrd.Status = enTrsMatchedRealtime Then
                        If aPtTraderFrom.CurQuantity <= BAD_LONG_VALUE Then aPtTraderFrom.CurQuantity = 0&
                        If aPtTraderTo.CurQuantity <= BAD_LONG_VALUE Then aPtTraderTo.CurQuantity = 0&
                        aPtTraderFrom.CurQuantity = aPtTraderFrom.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                        aPtTraderTo.CurQuantity = aPtTraderTo.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)

                    If Not aPtTraderFrom.LastTrade Is Nothing Then
                        If aPtTraderFrom.LastTrade.TradeID < aTrd.TradeID Then Set aPtTraderFrom.LastTrade = aTrd
                    Else
                        Set aPtTraderFrom.LastTrade = aTrd
                    End If
                    
                    If Not aPtTraderTo.LastTrade Is Nothing Then
                        If aPtTraderTo.LastTrade.TradeID < aTrd.TradeID Then Set aPtTraderTo.LastTrade = aTrd
                    Else
                        Set aPtTraderTo.LastTrade = aTrd
                    End If
                    
                    If Not aPtTraderFrom.LastTrade.Strategy Is Nothing Then _
                        Set eStrategy = aPtTraderFrom.LastTrade.Strategy
                End If
            Next

            If nTraderFromID = 0 And aPtTraderFrom.CurQuantity > BAD_LONG_VALUE _
                And aPtTraderFrom.CurQuantity <> 0 Then nTraderFromID = aTrader.ID
            Set aTrdByUnd = Nothing
        End If

        If Not Not aPtTraderFrom.LastTrade Is Nothing Then _
            If Not aPtTraderFrom.LastTrade.Strategy Is Nothing Then _
                Set aPtTraderFrom.Strategy = aPtTraderFrom.LastTrade.Strategy

        If Not eStrategy Is Nothing And aPtTraderFrom.Strategy Is Nothing Then _
            Set aPtTraderFrom.Strategy = eStrategy
        Set aPtTraderTo.Strategy = aPtTraderFrom.Strategy
    Next
End Sub

Private Sub FillTraderCombos(ByVal nTraderFromID&)
    On Error Resume Next
    Dim aPtTrader As clsPtTraderAtom, nTraderToID&

    cmbTraderFrom.Clear
    cmbTraderTo.Clear
    
    If nTraderFromID = 0 Then nTraderFromID = m_nDefTraderFromID
    nTraderToID = m_nDefTraderToID

    For Each aPtTrader In m_TraderFrom
        cmbTraderFrom.AddItem aPtTrader.Trader.Name
        cmbTraderFrom.ItemData(cmbTraderFrom.NewIndex) = aPtTrader.Trader.ID
        If aPtTrader.Trader.ID = nTraderFromID Then
            cmbTraderFrom.ListIndex = cmbTraderFrom.NewIndex
        End If
    Next
    
    For Each aPtTrader In m_TraderTo
        cmbTraderTo.AddItem aPtTrader.Trader.Name
        cmbTraderTo.ItemData(cmbTraderTo.NewIndex) = aPtTrader.Trader.ID
        If aPtTrader.Trader.ID = nTraderToID Then
            cmbTraderTo.ListIndex = cmbTraderTo.NewIndex
        End If
    Next
    
    If cmbTraderFrom.ListIndex = -1 Then cmbTraderFrom.ListIndex = 0
    If cmbTraderTo.ListIndex = -1 Then cmbTraderTo.ListIndex = 0
End Sub

Private Sub cmbTraderFrom_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    m_bInit = True
    
    AdjustTradeFrom
    FillStrategyCombo cmbStrategyFrom, m_PtTraderFrom
    
    AdjustTradeTo

    ShowTradeFromData
    ShowTradeToData
    
    AdjustControlsState
    m_bInit = False
End Sub

Private Sub cmbTraderTo_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    m_bInit = True
    
    AdjustTradeTo
    FillStrategyCombo cmbStrategyTo, m_PtTraderTo
    
    ShowTradeFromData
    ShowTradeToData
    
    AdjustControlsState
    m_bInit = False
End Sub

Private Sub FillStrategyCombo(ByRef aCombo As ComboBox, ByRef aPtTrader As clsPtTraderAtom)
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim aTrdUnd As EtsGeneralLib.EtsTraderUndAtom

    aCombo.Clear
    If aPtTrader Is Nothing Then Exit Sub
    
    aCombo.AddItem "<None>"
    aCombo.ItemData(aCombo.NewIndex) = 0
    
    Set aTrdUnd = aPtTrader.Trader.Und(m_Und.ID)
    If Not aTrdUnd Is Nothing Then
        For Each aStrategy In aTrdUnd.Strategy
            aCombo.AddItem aStrategy.Name
            aCombo.ItemData(aCombo.NewIndex) = aStrategy.ID
        Next
        Set aTrdUnd = Nothing
    End If
End Sub

Private Sub AdjustTradeFrom()
    On Error Resume Next
    Dim nTraderID&: nTraderID = 0&
    
    If cmbTraderFrom.ListIndex >= 0 Then _
        nTraderID = cmbTraderFrom.ItemData(cmbTraderFrom.ListIndex)
    
    If nTraderID = 0 Then
        Set m_PtTraderFrom = Nothing
        Set m_TrdFrom = Nothing
        cmbStrategyFrom.Clear
        Exit Sub
    End If
    
    Set m_PtTraderFrom = Nothing
    Set m_TrdFrom = Nothing
    
    Set m_PtTraderFrom = m_TraderFrom(CStr(nTraderID)).Copy
    If m_PtTraderFrom Is Nothing Then Exit Sub
    
    If Not m_PtTraderFrom.LastTrade Is Nothing Or (m_PtTraderFrom.CurQuantity > BAD_LONG_VALUE And m_PtTraderFrom.CurQuantity <> 0) Then
        If Not m_PtTraderFrom.Strategy Is Nothing Or (m_PtTraderFrom.CurQuantity > BAD_LONG_VALUE And m_PtTraderFrom.CurQuantity <> 0) Then
            Set m_TrdFrom = New EtsGeneralLib.MmTradeInfoAtom
            If Not m_PtTraderFrom.LastTrade Is Nothing Then _
                m_PtTraderFrom.LastTrade.CopyTo m_TrdFrom

            m_TrdFrom.IsPosition = False
            m_TrdFrom.Status = enTrsRealtime
            m_TrdFrom.Quantity = 0 'Abs(m_PtTraderFrom.CurQuantity)
            m_TrdFrom.IsBuy = (m_PtTraderFrom.CurQuantity < 0)
            Set m_TrdFrom.Broker = Nothing
            m_TrdFrom.BrokerCommission = 0#
            Set m_TrdFrom.ClearingBroker = Nothing
            m_TrdFrom.ClearingBrokerCommission = 0#
            If Not m_TrdFrom.Opt Is Nothing Then
                m_TrdFrom.price = m_TrdFrom.Opt.PriceClose
            ElseIf Not m_TrdFrom.Und Is Nothing Then
                m_TrdFrom.price = m_TrdFrom.Und.PriceClose
            End If
            
            
            Set m_TrdFrom.Strategy = m_PtTraderFrom.Strategy
        End If
    End If
End Sub

Private Sub AdjustTradeTo()
    On Error Resume Next
    Dim nTraderID&: nTraderID = 0&
    
    If cmbTraderTo.ListIndex >= 0 Then _
        nTraderID = cmbTraderTo.ItemData(cmbTraderTo.ListIndex)
    
    If nTraderID = 0 Then
        Set m_PtTraderTo = Nothing
        Set m_TrdTo = Nothing
        cmbStrategyTo.Clear
        Exit Sub
    End If
    
    Set m_PtTraderTo = Nothing
    Set m_TrdTo = Nothing
    
    Set m_PtTraderTo = m_TraderTo(CStr(nTraderID)).Copy
    If m_PtTraderTo Is Nothing Then Exit Sub
    
    If Not m_TrdFrom Is Nothing Then
        Set m_TrdTo = New EtsGeneralLib.MmTradeInfoAtom
        m_TrdFrom.CopyTo m_TrdTo
        
        Set m_TrdTo.Trader = m_PtTraderTo.Trader
        Set m_TrdTo.Strategy = m_PtTraderTo.Strategy
        m_TrdTo.IsBuy = Not m_TrdFrom.IsBuy
    End If
End Sub

Private Sub ShowTradeFromData()
    On Error Resume Next
    Dim i&
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nTraderID&: nTraderID = 0&
    If cmbTraderFrom.ListIndex >= 0 Then _
       nTraderID = cmbTraderFrom.ItemData(cmbTraderFrom.ListIndex)
    
    If Not m_PtTraderFrom Is Nothing Then
        If Not m_PtTraderFrom.Strategy Is Nothing Or m_PtTraderFrom.CurQuantity <> 0 Then

            If Not m_PtTraderFrom.Strategy Is Nothing Then
                For i = 0 To cmbStrategyFrom.ListCount - 1
                     If m_PtTraderFrom.Strategy.ID = cmbStrategyFrom.ItemData(i) Then cmbStrategyFrom.ListIndex = i
                Next
            Else
                cmbStrategyFrom.ListIndex = 0
            End If
            cmbStrategyFrom.Enabled = True
           
            m_PtTraderFrom.CurQuantity = 0
            Set aTrdByUnd = g_TradeChannel.TrdByTraderAndContract(CStr(nTraderID) & "_" & CStr(m_nContractID))
            If Not aTrdByUnd Is Nothing Then
            
                Dim nStrategyID As Long
                nStrategyID = IIf(m_PtTraderFrom.Strategy Is Nothing, 0, m_PtTraderFrom.Strategy.ID)
            
                 For Each aTrd In aTrdByUnd
                    If aTrd.strategyID = nStrategyID Then
                       If aTrd.Status = enTrsRealtime Or aTrd.Status = enTrsMatchedRealtime Then
                            m_PtTraderFrom.CurQuantity = m_PtTraderFrom.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                       End If
                    End If
                 Next
            End If
        Else
            
            If Not m_TrdFrom Is Nothing Then
                cmbStrategyFrom.ListIndex = 0
                cmbStrategyFrom.Enabled = True
            Else
                cmbStrategyFrom.ListIndex = -1
                cmbStrategyFrom.Enabled = False
            End If
            m_PtTraderFrom.CurQuantity = 0
            Set aTrdByUnd = g_TradeChannel.TrdByTraderAndContract(CStr(nTraderID) & "_" & CStr(m_nContractID))
            If Not aTrdByUnd Is Nothing Then
                 For Each aTrd In aTrdByUnd
                   If aTrd.Status = enTrsRealtime Or aTrd.Status = enTrsMatchedRealtime Then
                      m_PtTraderFrom.CurQuantity = m_PtTraderFrom.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                   End If
                 Next
            End If
           
        End If
        
        lblCurQuantityFrom.Caption = IIf(m_PtTraderFrom.CurQuantity > BAD_LONG_VALUE, _
                                        Format$(m_PtTraderFrom.CurQuantity, "#,##0"), "N/A")
        
        If Not m_TrdFrom Is Nothing Then
            fraTradeFrom.Caption = " Transfer From (" & IIf(m_TrdFrom.IsBuy, "BUY", "SELL") & " Trade) "
            
            Dim bSameTrader As Boolean: bSameTrader = False
            If Not m_PtTraderTo Is Nothing Then
                bSameTrader = (m_PtTraderFrom.Trader.ID = m_PtTraderTo.Trader.ID And cmbStrategyTo.ItemData(cmbStrategyTo.ListIndex) = cmbStrategyFrom.ItemData(cmbStrategyFrom.ListIndex))
            End If
            
            If Not bSameTrader Then
                lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity + m_TrdFrom.Quantity, "#,##0")
            
'                lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity + _
'                                                    IIf(m_TrdFrom.IsBuy, 1, -1) * m_TrdFrom.Quantity, "#,##0")
            Else
                lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity, "#,##0")
            End If
                                                
            txtQuantity.Text = Format$(m_TrdFrom.Quantity, "#,##0")
            txtPrice.Text = Format$(m_TrdFrom.price, "#,##0.#0")
            lblLotSize.Caption = Format$(m_TrdFrom.LotSize, "#,##0")
            lblTotalPrice.Caption = Format$(m_TrdFrom.TotalPrice, "#,##0.#0")
            chkMarkFrom.Value = IIf(m_TrdFrom.Mark <> 0, 1, 0)
        Else
            fraTradeFrom.Caption = " Transfer From "
            lblAfterQuantityFrom.Caption = "N/A"
            txtQuantity.Text = ""
            txtPrice.Text = ""
            lblLotSize.Caption = "N/A"
            lblTotalPrice.Caption = "N/A"
            chkMarkFrom.Value = False
        End If
    Else
        cmbStrategyFrom.ListIndex = -1
        cmbStrategyFrom.Enabled = False
        fraTradeFrom.Caption = " Transfer From "
        lblCurQuantityFrom.Caption = "N/A"
        lblAfterQuantityFrom.Caption = "N/A"
        txtQuantity.Text = ""
        txtPrice.Text = ""
        lblLotSize.Caption = "N/A"
        lblTotalPrice.Caption = "N/A"
        chkMarkFrom.Value = False
    End If
End Sub

Private Sub ShowTradeToData()
    On Error Resume Next
    Dim i&
    Dim aTrdByUnd As EtsGeneralLib.MmTradeInfoColl
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nTraderID&: nTraderID = 0&
    If cmbTraderTo.ListIndex >= 0 Then _
       nTraderID = cmbTraderTo.ItemData(cmbTraderTo.ListIndex)
    
    If Not m_PtTraderTo Is Nothing Then
        If Not m_PtTraderTo.Strategy Is Nothing Then
            For i = 0 To cmbStrategyTo.ListCount - 1
                If m_PtTraderTo.Strategy.ID = cmbStrategyTo.ItemData(i) Then cmbStrategyTo.ListIndex = i
            Next
            
            cmbStrategyTo.Enabled = Not m_TrdTo Is Nothing
            m_PtTraderTo.CurQuantity = 0
            Set aTrdByUnd = g_TradeChannel.TrdByTraderAndContract(CStr(nTraderID) & "_" & CStr(m_nContractID))
            If Not aTrdByUnd Is Nothing Then
                 For Each aTrd In aTrdByUnd
                    If aTrd.strategyID = m_PtTraderTo.Strategy.ID Then
                       If aTrd.Status = enTrsRealtime Or aTrd.Status = enTrsMatchedRealtime Then
                            m_PtTraderTo.CurQuantity = m_PtTraderTo.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                       End If
                    End If
                 Next
            End If
            
        Else
            If Not m_TrdTo Is Nothing Then
                cmbStrategyTo.ListIndex = 0
                cmbStrategyTo.Enabled = True
            Else
                cmbStrategyTo.ListIndex = -1
                cmbStrategyTo.Enabled = False
            End If
            
            m_PtTraderTo.CurQuantity = 0
            Set aTrdByUnd = g_TradeChannel.TrdByTraderAndContract(CStr(nTraderID) & "_" & CStr(m_nContractID))
            If Not aTrdByUnd Is Nothing Then
                 For Each aTrd In aTrdByUnd
                   If aTrd.Status = enTrsRealtime Or aTrd.Status = enTrsMatchedRealtime Then
                      m_PtTraderTo.CurQuantity = m_PtTraderTo.CurQuantity + aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                   End If
                 Next
            End If
            
        End If
        
        lblCurQuantityTo.Caption = IIf(m_PtTraderTo.CurQuantity > BAD_LONG_VALUE, _
                                        Format$(m_PtTraderTo.CurQuantity, "#,##0"), "N/A")
        
        If Not m_TrdTo Is Nothing Then
            fraTradeTo.Caption = " Transfer To (" & IIf(m_TrdTo.IsBuy, "BUY", "SELL") & " Trade) "
            
            Dim bSameTrader As Boolean: bSameTrader = False
            If Not m_PtTraderFrom Is Nothing Then
                bSameTrader = (m_PtTraderFrom.Trader.ID = m_PtTraderTo.Trader.ID And cmbStrategyTo.ItemData(cmbStrategyTo.ListIndex) = cmbStrategyFrom.ItemData(cmbStrategyFrom.ListIndex))
            End If
            
            If m_PtTraderTo.CurQuantity > BAD_LONG_VALUE Then
                If Not bSameTrader Then
                    lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity + m_TrdTo.Quantity, "#,##0")
                
'                    lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity + _
'                                                        IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdTo.Quantity, "#,##0")
                Else
                    lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity, "#,##0")
                End If
            Else
                lblAfterQuantityTo.Caption = Format$(IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdTo.Quantity, "#,##0")
            End If
            chkMarkTo.Value = IIf(m_TrdTo.Mark <> 0, 1, 0)
        Else
            fraTradeTo.Caption = " Transfer To "
            lblAfterQuantityTo.Caption = "N/A"
            chkMarkTo.Value = False
        End If
    Else
        cmbStrategyTo.ListIndex = -1
        cmbStrategyTo.Enabled = False
        fraTradeTo.Caption = " Transfer To "
        lblCurQuantityTo.Caption = "N/A"
        lblAfterQuantityTo.Caption = "N/A"
        chkMarkTo.Value = False
    End If
    
    btnStrategyToNew.Enabled = True 'cmbStrategyTo.Enabled
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnTransfer_Click()
    On Error Resume Next
    
    If m_PtTraderFrom Is Nothing Then
        gCmn.MyMsgBox Me, "No trader for transfer from was specified." & vbCrLf & "Please specify trader with position on this contract.", vbCritical
        cmbTraderFrom.SetFocus
        Exit Sub
    End If
    
    If m_PtTraderTo Is Nothing Then
        gCmn.MyMsgBox Me, "No trader for transfer from was specified." & vbCrLf & "Please specify trader for transfer to.", vbCritical
        cmbTraderTo.SetFocus
        Exit Sub
    End If

    If m_TrdFrom Is Nothing Or Not m_TrdFrom Is Nothing And m_TrdTo Is Nothing Then
        gCmn.MyMsgBox Me, "No position for transfer." & vbCrLf & "Please specify trader with position on this contract.", vbCritical
        cmbTraderFrom.SetFocus
        Exit Sub
    End If

    If m_PtTraderTo.Trader.ID = m_PtTraderFrom.Trader.ID Then
        Dim bValidStrategy As Boolean: bValidStrategy = False
        
        If Not m_PtTraderTo.Strategy Is Nothing Or Not m_PtTraderFrom.Strategy Is Nothing Then
            If Not m_PtTraderTo.Strategy Is Nothing And Not m_PtTraderFrom.Strategy Is Nothing Then
                bValidStrategy = (m_PtTraderTo.Strategy.ID <> m_PtTraderFrom.Strategy.ID)
            Else
                bValidStrategy = True
            End If
        End If
    
        If Not bValidStrategy Then
            gCmn.MyMsgBox Me, "Can't transfer position to the same strategy of one trader." & vbCrLf & "Please specify another strategy for transfer to.", vbCritical
            cmbTraderTo.SetFocus
            Exit Sub
        End If
    End If

    If (IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdFrom.Quantity) <= 0 Then
        If m_TrdTo.IsBuy Then
            gCmn.MyMsgBox Me, "Only Buy transfer is allowed." & vbCrLf & "Please specify valid quantity for transfer.", vbCritical
        Else
            gCmn.MyMsgBox Me, "Only Sell transfer is allowed." & vbCrLf & "Please specify valid quantity for transfer.", vbCritical
        End If
        txtQuantity.SetFocus
        Exit Sub
    End If

    If Abs(m_TrdFrom.Quantity) > Abs(m_PtTraderFrom.CurQuantity) Then
        gCmn.MyMsgBox Me, "Quantity for transfer can't exceed current position quantity." & vbCrLf & "Please specify valid quantity for transfer.", vbCritical
        txtQuantity.SetFocus
        Exit Sub
    End If

    If m_TrdFrom.price < 0# Then
        gCmn.MyMsgBox Me, "Price for transfer must be greater or equal then zero." & vbCrLf & "Please specify valid quantity for transfer.", vbCritical
        txtQuantity.SetFocus
        Exit Sub
    End If

    m_nDefTraderFromID = m_TrdFrom.Trader.ID
    m_nDefTraderToID = m_TrdTo.Trader.ID
    SaveRegEntries

    On Error GoTo EH
    m_TrdFrom.Quantity = IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdFrom.Quantity
    m_TrdTo.Quantity = IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdTo.Quantity
    g_TradeChannel.TransferPosition m_TrdFrom, m_TrdTo

    Unload Me
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to transfer position."
    Unload Me
End Sub

Private Sub btnStrategyToNew_Click()
    On Error Resume Next
    If Not m_PtTraderTo Is Nothing Then CreateAndSaveNewStrategy cmbStrategyTo, Me
End Sub

Private Sub cmbStrategyFrom_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    
    If Not m_PtTraderFrom Is Nothing Then
        If cmbStrategyFrom.ListIndex > 0 Then
            Set m_PtTraderFrom.Strategy = g_Strategy(cmbStrategyFrom.ItemData(cmbStrategyFrom.ListIndex))
        Else
            Set m_PtTraderFrom.Strategy = Nothing
        End If
        
        If Not m_TrdFrom Is Nothing Then Set m_TrdFrom.Strategy = m_PtTraderFrom.Strategy
        
        ShowTradeFromData
        ShowTradeToData
        
    End If
    
    AdjustControlsState
End Sub

Private Sub cmbStrategyTo_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    
    If Not m_PtTraderTo Is Nothing Then
        If cmbStrategyTo.ListIndex > 0 Then
            Set m_PtTraderTo.Strategy = g_Strategy(cmbStrategyTo.ItemData(cmbStrategyTo.ListIndex))
        Else
            Set m_PtTraderTo.Strategy = Nothing
        End If
        
        ShowTradeFromData
        ShowTradeToData
        
        If Not m_TrdTo Is Nothing Then Set m_TrdTo.Strategy = m_PtTraderTo.Strategy
    End If
    
    AdjustControlsState
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set m_TraderFrom = Nothing
    Set m_TraderTo = Nothing
    Set m_TrdFrom = Nothing
    Set m_PtTraderFrom = Nothing
    Set m_TrdTo = Nothing
    Set m_PtTraderTo = Nothing
    Set m_Und = Nothing

    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If

    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub AdjustControlsState()
    On Error Resume Next
    Dim bValid As Boolean
    
    bValid = False
    If Not m_PtTraderFrom Is Nothing Then
        bValid = Not m_TrdFrom Is Nothing
    End If
        
    lblTraderFrom.ForeColor = IIf(bValid, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
    txtQuantity.Enabled = bValid
    txtQuantity.BackColor = IIf(bValid, &H80000005, &H8000000F)
    txtPrice.Enabled = bValid
    txtPrice.BackColor = IIf(bValid, &H80000005, &H8000000F)
    chkMarkFrom.Enabled = bValid

    cmbStrategyFrom.BackColor = IIf(cmbStrategyFrom.Enabled, &H80000005, &H8000000F)
    cmbStrategyTo.BackColor = IIf(cmbStrategyTo.Enabled, &H80000005, &H8000000F)

    lblStrategyTo.ForeColor = IIf(cmbStrategyTo.Enabled _
                            And (cmbTraderFrom.ListIndex <> cmbTraderTo.ListIndex _
                                Or cmbTraderFrom.ListIndex = cmbTraderTo.ListIndex _
                                    And cmbStrategyFrom.ListIndex <> cmbStrategyTo.ListIndex), GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)

    lblTraderTo.ForeColor = IIf(cmbTraderTo.ListIndex >= 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
    chkMarkTo.Enabled = bValid
End Sub

Private Sub txtPrice_Change()
    On Error Resume Next
    If m_bInit Or m_PtTraderFrom Is Nothing Or m_PtTraderTo Is Nothing Then Exit Sub
    If m_TrdFrom Is Nothing Or m_TrdTo Is Nothing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtPrice.Text)
    
    If m_TrdFrom.price <> dValue Then
        m_TrdFrom.price = dValue
        lblPriceTitle.ForeColor = IIf(m_TrdFrom.price > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
        lblTotalPrice.Caption = Format$(m_TrdFrom.TotalPrice, "#,##0.00#####")
        
        m_TrdTo.price = dValue
    End If
End Sub

Private Sub txtQuantity_Change()
    On Error Resume Next
    If m_bInit Or m_PtTraderFrom Is Nothing Or m_PtTraderTo Is Nothing Then Exit Sub
    If m_TrdFrom Is Nothing Or m_TrdTo Is Nothing Then Exit Sub
    Dim nValue&: nValue = 0&

    nValue = CLng(txtQuantity.Text)

    If m_TrdFrom.Quantity <> nValue Then
        m_TrdFrom.Quantity = nValue
        Dim bSameTrader As Boolean: bSameTrader = False
        bSameTrader = (m_PtTraderFrom.Trader.ID = m_PtTraderTo.Trader.ID And cmbStrategyTo.ItemData(cmbStrategyTo.ListIndex) = cmbStrategyFrom.ItemData(cmbStrategyFrom.ListIndex))
    
        lblQuantityTitle.ForeColor = IIf(IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdFrom.Quantity > 0 _
                                And Abs(m_TrdFrom.Quantity) <= Abs(m_PtTraderFrom.CurQuantity), _
                                GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)

        If Not bSameTrader Then
            lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity - m_TrdFrom.Quantity, "#,##0")
        
'            lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity + _
'                                                IIf(m_TrdFrom.IsBuy, 1, -1) * m_TrdFrom.Quantity, "#,##0")
        Else
            lblAfterQuantityFrom.Caption = Format$(m_PtTraderFrom.CurQuantity, "#,##0")
        End If


        lblTotalPrice.Caption = Format$(m_TrdFrom.TotalPrice, "#,##0.#0")

        m_TrdTo.Quantity = nValue
    
    
        If m_PtTraderTo.CurQuantity > BAD_LONG_VALUE Then
            lblCurQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity, "#,##0")
            If Not bSameTrader Then
                lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity + m_TrdTo.Quantity, "#,##0")
                
'                lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity + _
'                                                    IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdTo.Quantity, "#,##0")
            Else
                lblAfterQuantityTo.Caption = Format$(m_PtTraderTo.CurQuantity, "#,##0")
            End If
        Else
            lblCurQuantityTo.Caption = "N/A"
            lblAfterQuantityTo.Caption = Format$(IIf(m_TrdTo.IsBuy, 1, -1) * m_TrdTo.Quantity, "#,##0")
        End If
    End If
End Sub

Private Sub chkMarkFrom_Click()
    On Error Resume Next
    If m_bInit Or m_PtTraderFrom Is Nothing Then Exit Sub
    If Not m_TrdFrom Is Nothing Then m_TrdFrom.Mark = IIf(chkMarkFrom.Value <> 0, 1, 0)
End Sub

Private Sub chkMarkTo_Click()
    On Error Resume Next
    If m_bInit Or m_PtTraderTo Is Nothing Then Exit Sub
    If Not m_TrdTo Is Nothing Then m_TrdTo.Mark = IIf(chkMarkTo.Value <> 0, 1, 0)
End Sub

Private Sub ReadRegEntries()
    On Error Resume Next
    m_nDefTraderFromID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "PositionTransfer", "DefTraderFromID", 0)
    m_nDefTraderToID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "PositionTransfer", "DefTraderToID", 0)
End Sub

Private Sub SaveRegEntries()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "PositionTransfer", "DefTraderFromID", m_nDefTraderFromID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "PositionTransfer", "DefTraderToID", m_nDefTraderToID
End Sub
