VERSION 5.00
Begin VB.Form frmEnterOrder 
   Caption         =   "Enter order"
   ClientHeight    =   2730
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3825
   LinkTopic       =   "Form1"
   ScaleHeight     =   2730
   ScaleWidth      =   3825
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   495
      Left            =   2040
      TabIndex        =   9
      Top             =   2160
      Width           =   1695
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "Enter"
      Height          =   495
      Left            =   120
      TabIndex        =   8
      Top             =   2160
      Width           =   1575
   End
   Begin VB.TextBox edQuantity 
      Height          =   285
      Left            =   1440
      TabIndex        =   7
      Top             =   1680
      Width           =   2295
   End
   Begin VB.TextBox enPremium 
      Height          =   285
      Left            =   1440
      TabIndex        =   5
      Top             =   1200
      Width           =   2295
   End
   Begin VB.ComboBox cboBidAsk 
      Height          =   315
      Left            =   1440
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   720
      Width           =   2295
   End
   Begin VB.ComboBox cboSeries 
      Height          =   315
      Left            =   1440
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   240
      Width           =   2295
   End
   Begin VB.Label Label4 
      Caption         =   "Quantity"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   1680
      Width           =   1095
   End
   Begin VB.Label Label3 
      Caption         =   "Premium"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   1200
      Width           =   1095
   End
   Begin VB.Label Label2 
      Caption         =   "Bid / Ask"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Series"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   240
      Width           =   975
   End
End
Attribute VB_Name = "frmEnterOrder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdCancel_Click()
    Unload Me
End Sub

Private Sub cmdOk_Click()
On Error GoTo ErrHandler
    Dim OrderID As String
    Dim Order As ISEPROVIDERLib.ISEOrder
    
    Dim BidOrAsk As ISEPROVIDERLib.BidAskEnum
    If cboBidAsk.Text = "Bid" Then
        BidOrAsk = enBid
    Else
        BidOrAsk = enAsk
    End If
    
    Order.Series = cboSeries.Text
    Order.BidOrAsk = BidOrAsk
    Order.Price = enPremium.Text
    Order.Quantity = edQuantity.Text
    
    OrderID = frmMain.objMMManager.EnterOrder(Order)
    
    frmMain.LogMsg "Order submitted with ID = '" & OrderID & "'"
    
    Unload Me
    Exit Sub
    
ErrHandler:
    MsgBox Error
End Sub

Private Sub Form_Load()
    cboBidAsk.AddItem "Bid"
    cboBidAsk.AddItem "Ask"
    cboBidAsk.Text = "Bid"
    
    If frmMain.lstSeries.ListItems.Count <> 0 Then
   
        For iIndex = 1 To frmMain.lstSeries.ListItems.Count
            cboSeries.AddItem frmMain.lstSeries.ListItems(iIndex).SubItems(1)
        Next
        
        cboSeries.Text = frmMain.lstSeries.ListItems(1).SubItems(1)
        enPremium.Text = 10
        edQuantity.Text = 10
    Else
        cmdOk.Enabled = False
    End If
End Sub
