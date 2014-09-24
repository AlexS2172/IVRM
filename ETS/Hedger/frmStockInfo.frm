VERSION 5.00
Begin VB.Form frmStockInfo 
   BackColor       =   &H00666666&
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Stock Info"
   ClientHeight    =   2940
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   2775
   Icon            =   "frmStockInfo.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2940
   ScaleWidth      =   2775
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   6
      X1              =   360
      X2              =   2400
      Y1              =   2480
      Y2              =   2480
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   5
      X1              =   360
      X2              =   2400
      Y1              =   2240
      Y2              =   2240
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   4
      X1              =   360
      X2              =   2400
      Y1              =   2000
      Y2              =   2000
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   3
      X1              =   360
      X2              =   2400
      Y1              =   1760
      Y2              =   1760
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   2
      X1              =   360
      X2              =   2400
      Y1              =   1160
      Y2              =   1160
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   360
      X2              =   2400
      Y1              =   920
      Y2              =   920
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      BackColor       =   &H00666666&
      Caption         =   "Corr"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Index           =   7
      Left            =   360
      TabIndex        =   15
      Top             =   1320
      Width           =   2055
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   0
      X1              =   360
      X2              =   2400
      Y1              =   680
      Y2              =   680
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   6
      Left            =   1560
      TabIndex        =   14
      Top             =   2280
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "DJX, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   6
      Left            =   360
      TabIndex        =   13
      Top             =   2280
      Width           =   1095
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   5
      Left            =   1560
      TabIndex        =   12
      Top             =   2040
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "OEX, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   5
      Left            =   360
      TabIndex        =   11
      Top             =   2040
      Width           =   1095
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   4
      Left            =   1560
      TabIndex        =   10
      Top             =   1800
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "NDX, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   4
      Left            =   360
      TabIndex        =   9
      Top             =   1800
      Width           =   1095
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   3
      Left            =   1560
      TabIndex        =   8
      Top             =   1560
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "SPX, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   3
      Left            =   360
      TabIndex        =   7
      Top             =   1560
      Width           =   1095
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   2
      Left            =   1560
      TabIndex        =   6
      Top             =   960
      Width           =   855
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "iV, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   2
      Left            =   360
      TabIndex        =   5
      Top             =   960
      Width           =   1095
   End
   Begin VB.Label lblCaption 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "STOCK"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   480
      TabIndex        =   2
      Top             =   120
      Width           =   1875
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Price, $"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   255
      Index           =   0
      Left            =   360
      TabIndex        =   4
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "HV, %"
      BeginProperty Font 
         Name            =   "Microsoft Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00E0FFFF&
      Height          =   375
      Index           =   1
      Left            =   360
      TabIndex        =   3
      Top             =   720
      Width           =   1095
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "1,234.00 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   0
      Left            =   1560
      TabIndex        =   1
      Top             =   480
      Width           =   855
   End
   Begin VB.Label lblTotal 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H00666666&
      Caption         =   "0 "
      ForeColor       =   &H0033FF99&
      Height          =   255
      Index           =   1
      Left            =   1560
      TabIndex        =   0
      Top             =   720
      Width           =   855
   End
End
Attribute VB_Name = "frmStockInfo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Dim g_lLeft As Long
Dim g_lTop As Long
'Dim g_lWidth As Long
'Dim g_lHeight As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Execute(ByVal StockSymbol As String)
    On Error Resume Next
    lblCaption = StockSymbol
    With gPortfolioHedge
        If .VegaMode Then
            Label1(7).Caption = "Voly Corr"
        Else
            Label1(7).Caption = "Price Corr"
        End If
        lblTotal(0) = FormatMoney(.GetStockPrice(StockSymbol))
        lblTotal(1) = FormatPercent(.GetStockHV(StockSymbol))
        lblTotal(2) = FormatPercent(.GetStockIV(StockSymbol))
        lblTotal(3) = FormatPercent(.GetStockIndexCorr(StockSymbol, "SPX"))
        lblTotal(4) = FormatPercent(.GetStockIndexCorr(StockSymbol, "NDX"))
        lblTotal(5) = FormatPercent(.GetStockIndexCorr(StockSymbol, "OEX"))
        lblTotal(6) = FormatPercent(.GetStockIndexCorr(StockSymbol, "DJX"))
    End With
    Show , frmMain
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatCount(ByVal c As Long) As String
    FormatCount = Format$(c) & Space(1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatPercent(ByVal d As Double) As String
    FormatPercent = FormatMoney(d * 100)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FormatMoney(ByVal d As Double) As String
    FormatMoney = Format$(Round(d, 2), "Standard") & Space(1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If KeyCode = vbKeyEscape Then
        CloseMe
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    g_lTop = MyGetSettingLong("InfoTop", 0)
    g_lLeft = MyGetSettingLong("InfoLeft", 0)
    'g_lWidth = MyGetSettingLong("InfoWidth", 0)
    'g_lHeight = MyGetSettingLong("InfoHeight", 0)
    'If g_lWidth > 0 And g_lHeight > 0 Then
        If g_lLeft < 0 Then g_lLeft = 0
        If g_lTop < 0 Then g_lTop = 0
        If g_lLeft > Screen.Width - 300 Then g_lLeft = Screen.Width - 300
        If g_lTop > Screen.Height - 300 Then g_lTop = Screen.Height - 300
        Me.Left = g_lLeft
        Me.Top = g_lTop
        'Me.Width = g_lWidth
        'me.Height = g_lHeight
    'Else
    '    Me.Left = (Screen.Width - Me.Width) / 2
    '    Me.Top = (Screen.Height - Me.Height) / 2
    'End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = (UnloadMode = vbFormControlMenu)
    CloseMe
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CloseMe()
    On Error Resume Next
    g_lLeft = Me.Left
    g_lTop = Me.Top
    'g_lWidth = Me.Width
    'g_lHeight = Me.Height
    
    MySaveSettingLong "InfoTop", g_lTop
    MySaveSettingLong "InfoLeft", g_lLeft
    'MySaveSettingLong "InfoWidth", g_lWidth
    'MySaveSettingLong "InfoHeight", g_lHeight
    
    Hide
End Sub

