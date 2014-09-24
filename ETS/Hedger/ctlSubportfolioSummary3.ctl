VERSION 5.00
Begin VB.UserControl ctlSubportfolioSummary3 
   BackColor       =   &H00666666&
   ClientHeight    =   5115
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6480
   ScaleHeight     =   5115
   ScaleWidth      =   6480
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   13
      X1              =   1560
      X2              =   0
      Y1              =   420
      Y2              =   420
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   12
      Visible         =   0   'False
      X1              =   1560
      X2              =   1560
      Y1              =   4275
      Y2              =   660
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   11
      X1              =   1560
      X2              =   0
      Y1              =   4260
      Y2              =   4260
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   10
      X1              =   1560
      X2              =   0
      Y1              =   3960
      Y2              =   3960
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   9
      X1              =   1560
      X2              =   0
      Y1              =   3660
      Y2              =   3660
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   11
      Left            =   0
      TabIndex        =   12
      Top             =   4020
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   10
      Left            =   0
      TabIndex        =   11
      Top             =   3720
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   9
      Left            =   0
      TabIndex        =   10
      Top             =   3420
      Width           =   1575
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   8
      X1              =   1560
      X2              =   0
      Y1              =   3360
      Y2              =   3360
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   7
      X1              =   1560
      X2              =   0
      Y1              =   2820
      Y2              =   2820
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   5
      X1              =   1560
      X2              =   0
      Y1              =   2520
      Y2              =   2520
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   4
      X1              =   1560
      X2              =   0
      Y1              =   2220
      Y2              =   2220
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   3
      X1              =   1560
      X2              =   0
      Y1              =   1920
      Y2              =   1920
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   2
      X1              =   1560
      X2              =   0
      Y1              =   1620
      Y2              =   1620
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   1560
      X2              =   0
      Y1              =   1320
      Y2              =   1320
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   0
      X1              =   1560
      X2              =   0
      Y1              =   1020
      Y2              =   1020
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   6
      X1              =   1560
      X2              =   0
      Y1              =   720
      Y2              =   720
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   7
      Left            =   0
      TabIndex        =   8
      Top             =   2580
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   6
      Left            =   0
      TabIndex        =   7
      Top             =   1980
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   5
      Left            =   0
      TabIndex        =   6
      Top             =   1680
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   4
      Left            =   0
      TabIndex        =   5
      Top             =   1380
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   2
      Left            =   0
      TabIndex        =   4
      Top             =   780
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   1
      Left            =   0
      TabIndex        =   3
      Top             =   480
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "1,234,56,789.00 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   0
      Left            =   0
      TabIndex        =   2
      Top             =   2280
      Width           =   1575
   End
   Begin VB.Label lblCaption 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Short"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   120
      Width           =   1515
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   3
      Left            =   0
      TabIndex        =   0
      Top             =   1080
      Width           =   1575
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      BackStyle       =   0  'Transparent
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   8
      Left            =   0
      TabIndex        =   9
      Top             =   3120
      Width           =   1575
   End
End
Attribute VB_Name = "ctlSubportfolioSummary3"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Clear
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
Public Sub SetCaption(ByVal NewValue As String)
    lblCaption = NewValue
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals(ByVal d1 As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal D4 As Double)
    lblTotal(0).Caption = FormatMoney(-d1)
    lblTotal(1).Caption = FormatMoney(D2)
    lblTotal(2).Caption = FormatMoney(d3)
    lblTotal(3).Caption = FormatMoney(D4 * 100)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals2(ByVal d5 As Long, ByVal d6 As Long, ByVal d7 As Double, ByVal d8 As Double)
    lblTotal(4).Caption = FormatCount(d5)
    lblTotal(5).Caption = FormatCount(d6)
    lblTotal(6).Caption = FormatMoney(d7)
    lblTotal(7).Caption = FormatMoney(d8)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals3(ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double)
    lblTotal(8).Caption = FormatMoney(-Q1)
    lblTotal(9).Caption = FormatMoney(-Q2)
    lblTotal(10).Caption = FormatMoney(-Q3)
    lblTotal(11).Caption = FormatMoney(-Q4)
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

