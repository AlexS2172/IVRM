VERSION 5.00
Begin VB.UserControl ctlSubPortfolioSummary4 
   BackColor       =   &H00666666&
   ClientHeight    =   2595
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6315
   ScaleHeight     =   2595
   ScaleWidth      =   6315
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   6
      X1              =   3120
      X2              =   0
      Y1              =   1560
      Y2              =   1560
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   5
      X1              =   3120
      X2              =   0
      Y1              =   1860
      Y2              =   1860
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   0
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary4.ctx":0000
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary4.ctx":0152
      ToolTipText     =   "Open Help"
      Top             =   420
      Width           =   150
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   3120
      X2              =   0
      Y1              =   360
      Y2              =   360
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   2
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary4.ctx":01B3
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary4.ctx":0305
      ToolTipText     =   "Open Help"
      Top             =   1620
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   1
      Left            =   1560
      MouseIcon       =   "ctlSubPortfolioSummary4.ctx":0366
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubPortfolioSummary4.ctx":04B8
      ToolTipText     =   "Open Help"
      Top             =   1320
      Width           =   150
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
      Index           =   7
      X1              =   3120
      X2              =   0
      Y1              =   660
      Y2              =   660
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   8
      X1              =   3120
      X2              =   0
      Y1              =   960
      Y2              =   960
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   0
      X1              =   3120
      X2              =   -120
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
      Index           =   0
      Left            =   1680
      TabIndex        =   6
      Top             =   420
      Width           =   1425
   End
   Begin VB.Label lblCaption 
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "REMAINING"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   2025
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Deviation, $"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   4
      Top             =   420
      Width           =   1545
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
      TabIndex        =   3
      Top             =   1620
      Width           =   1425
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Stocks"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   2
      Top             =   1320
      Width           =   1545
   End
   Begin VB.Label Label1 
      BackColor       =   &H00666666&
      Caption         =   "Position, $"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   1
      Top             =   1620
      Width           =   1545
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   1
      Left            =   1680
      TabIndex        =   0
      Top             =   1320
      Width           =   1425
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   9
      X1              =   5880
      X2              =   0
      Y1              =   0
      Y2              =   0
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   10
      X1              =   0
      X2              =   0
      Y1              =   1800
      Y2              =   0
   End
End
Attribute VB_Name = "ctlSubPortfolioSummary4"
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
        Label1(1).Caption = "Contracts "
    Else
        Label1(1).Caption = "Stocks "
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
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals2(ByVal d1 As Long, ByVal D2 As Long, ByVal d3 As Double, ByVal D4 As Double)
    lblTotal(1).Caption = FormatCount(D2)
    lblTotal(2).Caption = FormatMoney(d3)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetSubTotals(ByVal d1 As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal D4 As Double)
    '
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
        ShowHelpID 213
    Case 2
        ShowHelpID 208
    Case Else
        Debug.Assert False
    End Select
End Sub

