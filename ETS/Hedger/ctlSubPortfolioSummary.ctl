VERSION 5.00
Begin VB.UserControl ctlSubPortfolioSummary 
   Alignable       =   -1  'True
   Appearance      =   0  'Flat
   BackColor       =   &H00000000&
   ClientHeight    =   330
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   7590
   ScaleHeight     =   330
   ScaleWidth      =   7590
   Begin VB.Label lblTotal4 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Left            =   5520
      TabIndex        =   7
      Top             =   0
      Width           =   465
   End
   Begin VB.Label lblTotal3 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Left            =   4080
      TabIndex        =   6
      Top             =   0
      Width           =   1185
   End
   Begin VB.Label lblTotal2 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Left            =   2520
      TabIndex        =   5
      Top             =   0
      Width           =   1305
   End
   Begin VB.Label lblTotal1 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      Caption         =   "123456789 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Left            =   960
      TabIndex        =   4
      Top             =   0
      Width           =   1305
   End
   Begin VB.Label lblCaption 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      Caption         =   "SPX"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Left            =   0
      TabIndex        =   8
      Top             =   15
      Width           =   705
   End
   Begin VB.Label Label4 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "r"
      BeginProperty Font 
         Name            =   "Symbol"
         Size            =   12
         Charset         =   2
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   315
      Left            =   5280
      TabIndex        =   3
      Top             =   -60
      Width           =   225
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Symbol"
         Size            =   12
         Charset         =   2
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   300
      Left            =   3840
      TabIndex        =   2
      Top             =   -45
      Width           =   225
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Symbol"
         Size            =   9.75
         Charset         =   2
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Left            =   2280
      TabIndex        =   1
      Top             =   0
      Width           =   225
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Q"
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Left            =   720
      TabIndex        =   0
      Top             =   0
      Width           =   225
   End
End
Attribute VB_Name = "ctlSubPortfolioSummary"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetCaption(ByVal NewValue As String)
    lblCaption = NewValue
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetTotals(ByVal d1 As Double, ByVal d2 As Double, ByVal d3 As Double, ByVal d4 As Double)
    lblTotal1.Caption = FormatMoney(d1)
    lblTotal2.Caption = FormatMoney(d2)
    If Abs(d4) > 1 Then
        lblTotal3.Caption = GSTR_NA
        lblTotal4.Caption = GSTR_NA
        lblTotal3.ForeColor = vbYellow
        lblTotal4.ForeColor = vbYellow
    Else
        lblTotal3.Caption = FormatMoney(d3)
        lblTotal4.Caption = FormatMoney(d4)
        lblTotal3.ForeColor = &H33FF99
        lblTotal4.ForeColor = &H33FF99
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatMoney(ByVal d As Double) As String
    FormatMoney = Format$(Round(d, 2), "Standard") & Space(1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
Dim w As Single
Dim X As Single
    X = lblCaption.Width + Label1.Width + Label2.Width + Label3.Width + Label4.Width + lblTotal4.Width
    w = Max(lblTotal4.Width, (ScaleWidth - X) / 3)
    lblTotal1.Width = w
    lblTotal2.Width = w
    lblTotal3.Width = w
    With Label2
        .Left = lblTotal1.Left + w
        lblTotal2.Left = .Left + .Width
    End With
    With Label3
        .Left = lblTotal2.Left + w
        lblTotal3.Left = .Left + .Width
    End With
    With Label4
        .Left = lblTotal3.Left + w
        lblTotal4.Left = .Left + .Width
    End With
End Sub

