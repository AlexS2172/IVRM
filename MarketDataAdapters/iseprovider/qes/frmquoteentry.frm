VERSION 5.00
Begin VB.Form frmQuoteEntry 
   Caption         =   "Form1"
   ClientHeight    =   3495
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6900
   LinkTopic       =   "Form1"
   ScaleHeight     =   3495
   ScaleWidth      =   6900
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3720
      TabIndex        =   17
      Top             =   3000
      Width           =   2175
   End
   Begin VB.CommandButton cmdOk 
      Caption         =   "OK"
      Height          =   375
      Left            =   840
      TabIndex        =   16
      Top             =   3000
      Width           =   2055
   End
   Begin VB.TextBox edAskStepUpBuffer 
      Height          =   285
      Left            =   3960
      TabIndex        =   15
      Top             =   2520
      Width           =   1815
   End
   Begin VB.TextBox edBidStepUpBuffer 
      Height          =   285
      Left            =   1320
      TabIndex        =   14
      Top             =   2520
      Width           =   1935
   End
   Begin VB.TextBox edAskOpenQuantity 
      Height          =   285
      Left            =   3960
      TabIndex        =   12
      Top             =   2040
      Width           =   1815
   End
   Begin VB.TextBox edBidOpenQuantity 
      Height          =   285
      Left            =   1320
      TabIndex        =   11
      Top             =   2040
      Width           =   1935
   End
   Begin VB.TextBox edAskPremium 
      Height          =   285
      Left            =   3960
      TabIndex        =   9
      Top             =   1560
      Width           =   1815
   End
   Begin VB.TextBox edBidPremium 
      Height          =   285
      Left            =   1320
      TabIndex        =   8
      Top             =   1560
      Width           =   1935
   End
   Begin VB.TextBox edAskQuantity 
      Height          =   285
      Left            =   3960
      TabIndex        =   6
      Top             =   1080
      Width           =   1815
   End
   Begin VB.TextBox edBidQuantity 
      Height          =   285
      Left            =   1320
      TabIndex        =   2
      Top             =   1080
      Width           =   1935
   End
   Begin VB.TextBox edSeries 
      Enabled         =   0   'False
      Height          =   285
      Left            =   840
      TabIndex        =   0
      Top             =   240
      Width           =   5655
   End
   Begin VB.Label Label7 
      Caption         =   "Step up buffer"
      Height          =   255
      Left            =   120
      TabIndex        =   13
      Top             =   2520
      Width           =   1095
   End
   Begin VB.Label Label6 
      Caption         =   "Open quanity"
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   2040
      Width           =   975
   End
   Begin VB.Label Label5 
      Caption         =   "Premium"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   1560
      Width           =   855
   End
   Begin VB.Label Label4 
      Caption         =   "ASK"
      Height          =   255
      Left            =   4320
      TabIndex        =   5
      Top             =   720
      Width           =   735
   End
   Begin VB.Label Label3 
      Caption         =   "BID"
      Height          =   255
      Left            =   1560
      TabIndex        =   4
      Top             =   720
      Width           =   615
   End
   Begin VB.Label Label2 
      Caption         =   "Quantity"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   1080
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Series"
      Height          =   255
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   615
   End
End
Attribute VB_Name = "frmQuoteEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public strBidQuantity As String
Public strBidPremium  As String
Public strAskPremium  As String
Public strAskQuantity  As String
Public strAskOpenQuantity As String
Public strBidOpenQuantity As String
Public strAskStepUpBuffer As String
Public strBidStepUpBuffer As String

Public bOk As Boolean

Private Sub cmdCancel_Click()
    bOk = False
    Unload Me
End Sub

Private Sub cmdOk_Click()
    strBidQuantity = edBidQuantity
    strBidPremium = edBidPremium
    strAskPremium = edAskPremium
    strAskQuantity = edAskQuantity
    strAskOpenQuantity = edAskOpenQuantity
    strBidOpenQuantity = edBidOpenQuantity
    strAskStepUpBuffer = edAskStepUpBuffer
    strBidStepUpBuffer = edBidStepUpBuffer
    bOk = True
    Unload Me
End Sub

