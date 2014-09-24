VERSION 5.00
Begin VB.UserControl ctlSubPortfolioSummary2 
   Alignable       =   -1  'True
   BackColor       =   &H00666666&
   ClientHeight    =   3585
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6915
   ScaleHeight     =   3585
   ScaleWidth      =   6915
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   11
      X1              =   5040
      X2              =   -240
      Y1              =   1860
      Y2              =   1860
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   5
      Left            =   4920
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":0000
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":0152
      ToolTipText     =   "Open Help"
      Top             =   420
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   4
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":01B3
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":0305
      ToolTipText     =   "Open Help"
      Top             =   1620
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   3
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":0366
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":04B8
      ToolTipText     =   "Open Help"
      Top             =   1320
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   2
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":0519
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":066B
      ToolTipText     =   "Open Help"
      Top             =   1020
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   1
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":06CC
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":081E
      ToolTipText     =   "Open Help"
      Top             =   720
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   0
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary2.ctx":087F
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary2.ctx":09D1
      ToolTipText     =   "Open Help"
      Top             =   420
      Width           =   150
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   10
      X1              =   0
      X2              =   0
      Y1              =   1800
      Y2              =   0
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   8
      X1              =   5040
      X2              =   -840
      Y1              =   960
      Y2              =   960
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   7
      X1              =   5040
      X2              =   -840
      Y1              =   660
      Y2              =   660
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   6
      X1              =   5040
      X2              =   -840
      Y1              =   1560
      Y2              =   1560
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   4
      X1              =   3120
      X2              =   3120
      Y1              =   1860
      Y2              =   360
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   3
      X1              =   1680
      X2              =   1680
      Y1              =   1860
      Y2              =   360
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   2
      X1              =   5040
      X2              =   5040
      Y1              =   1860
      Y2              =   360
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   5040
      X2              =   -240
      Y1              =   360
      Y2              =   360
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   0
      X1              =   5040
      X2              =   -960
      Y1              =   1260
      Y2              =   1260
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   4
      Left            =   1680
      TabIndex        =   19
      Top             =   1620
      Width           =   1425
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   3
      Left            =   1680
      TabIndex        =   18
      Top             =   1320
      Width           =   1425
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Position, $"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   5
      Left            =   120
      TabIndex        =   17
      Top             =   1620
      Width           =   1425
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Stocks"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   16
      Top             =   1320
      Width           =   1425
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Hedged dev., $"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   15
      Top             =   720
      Width           =   1425
   End
   Begin VB.Label txtQi 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   3
      Left            =   3720
      LinkItem        =   "Equvalent index position"
      TabIndex        =   14
      Top             =   1620
      Width           =   1305
   End
   Begin VB.Label lblQi 
      Alignment       =   2  'Center
      BackColor       =   &H00666666&
      Caption         =   "DJX"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   3
      Left            =   3240
      TabIndex        =   13
      Top             =   1620
      Width           =   465
   End
   Begin VB.Label txtQi 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   2
      Left            =   3720
      LinkItem        =   "Equvalent index position"
      TabIndex        =   12
      Top             =   1320
      Width           =   1305
   End
   Begin VB.Label lblQi 
      Alignment       =   2  'Center
      BackColor       =   &H00666666&
      Caption         =   "OEX"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   2
      Left            =   3240
      TabIndex        =   11
      Top             =   1320
      Width           =   465
   End
   Begin VB.Label txtQi 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   0
      Left            =   3720
      LinkItem        =   "Equvalent index position"
      TabIndex        =   10
      Top             =   720
      Width           =   1305
   End
   Begin VB.Label lblQi 
      Alignment       =   2  'Center
      BackColor       =   &H00666666&
      Caption         =   "SPX"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   0
      Left            =   3240
      TabIndex        =   9
      Top             =   720
      Width           =   465
   End
   Begin VB.Label txtQi 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   1
      Left            =   3720
      LinkItem        =   "Equvalent index position"
      TabIndex        =   8
      Top             =   1020
      Width           =   1305
   End
   Begin VB.Label lblQi 
      Alignment       =   2  'Center
      BackColor       =   &H00666666&
      Caption         =   "NDX"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   1
      Left            =   3240
      TabIndex        =   7
      Top             =   1020
      Width           =   465
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Gain, %"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   3
      Left            =   120
      TabIndex        =   6
      Top             =   1020
      Width           =   1425
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   2
      Left            =   1680
      LinkItem        =   "Gain"
      TabIndex        =   5
      Top             =   1020
      Width           =   1425
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Unhedged dev., $"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   4
      Top             =   420
      Width           =   1425
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      BackColor       =   &H00666666&
      Caption         =   "Index positions, $ "
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   0
      Left            =   3240
      TabIndex        =   3
      Top             =   420
      Width           =   1665
   End
   Begin VB.Label lblCaption 
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "SHORT"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   1545
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   1
      Left            =   1680
      TabIndex        =   1
      Top             =   720
      Width           =   1425
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   0
      Left            =   1680
      TabIndex        =   0
      Top             =   420
      Width           =   1425
   End
End
Attribute VB_Name = "ctlSubPortfolioSummary2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetVegaMode(ByVal VegaMode As Boolean)
    If VegaMode Then
        Label1(4).Caption = "Contracts "
        Label1(0).Caption = "Equiv. Index vega "
    Else
        Label1(4).Caption = "Stocks "
        Label1(0).Caption = "Index positions, $ "
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetCaption(ByVal NewValue As String)
    If NewValue = "NONE" Then
        lblCaption = "REMAINING"
    Else
        lblCaption = NewValue
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals(ByVal d1 As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal D4 As Double)
    lblTotal(0).Caption = FormatMoney(D2)
    lblTotal(1).Caption = FormatMoney(d3)
    lblTotal(2).Caption = FormatMoney(D4 * 100)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals2(ByVal d1 As Long, ByVal D2 As Long, ByVal d3 As Double, ByVal D4 As Double)
    lblTotal(3).Caption = FormatCount(D2)
    lblTotal(4).Caption = FormatMoney(d3)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetSubTotals(ByVal d1 As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal D4 As Double)
    txtQi(0).Caption = FormatMoney(-d1)
    txtQi(1).Caption = FormatMoney(-D2)
    txtQi(2).Caption = FormatMoney(-d3)
    txtQi(3).Caption = FormatMoney(-D4)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatCount(ByVal c As Long) As String
    FormatCount = Format$(c) & Space(1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatMoney(ByVal d As Double) As String
    FormatMoney = Format$(Round(d, 2), "Standard") & Space(1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Clear
    ShowHelpTags
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
Dim i As Long
    With lblTotal
        For i = .LBound To .UBound
            .Item(i).Caption = ""
        Next
    End With
    With txtQi
        For i = .LBound To .UBound
            .Item(i).Caption = ""
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpTags()
    On Error Resume Next
    Dim i As Long
    For i = imgSdHelp.LBound To imgSdHelp.UBound
        imgSdHelp(i).Visible = g_bShowHelpTags
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub imgSDHelp_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case 0
        ShowHelpID 202
    Case 1
        ShowHelpID 203
    Case 2
        ShowHelpID 204
    Case 3
        ShowHelpID 206
    Case 4
        ShowHelpID 207
    Case 5
        ShowHelpID 210
    Case Else
        Debug.Assert False
    End Select
End Sub

