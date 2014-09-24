VERSION 5.00
Begin VB.UserControl ctlSubportfolioSummary1 
   BackColor       =   &H00666666&
   ClientHeight    =   4905
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   5385
   ScaleHeight     =   4905
   ScaleWidth      =   5385
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   12
      X1              =   2520
      X2              =   240
      Y1              =   420
      Y2              =   420
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   8
      Left            =   2355
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0000
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":0152
      ToolTipText     =   "Open Help"
      Top             =   3120
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   0
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":01B3
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":0305
      ToolTipText     =   "Open Help"
      Top             =   2280
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   2
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0366
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":04B8
      ToolTipText     =   "Open Help"
      Top             =   780
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   3
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0519
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":066B
      ToolTipText     =   "Open Help"
      Top             =   1080
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   4
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":06CC
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":081E
      ToolTipText     =   "Open Help"
      Top             =   1380
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   5
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":087F
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":09D1
      ToolTipText     =   "Open Help"
      Top             =   1680
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   6
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0A32
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":0B84
      ToolTipText     =   "Open Help"
      Top             =   1980
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   7
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0BE5
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":0D37
      ToolTipText     =   "Open Help"
      Top             =   2580
      Width           =   150
   End
   Begin VB.Image imgSdHelp 
      Appearance      =   0  'Flat
      Height          =   150
      Index           =   1
      Left            =   2360
      MouseIcon       =   "ctlSubportfolioSummary1.ctx":0D98
      MousePointer    =   99  'Custom
      Picture         =   "ctlSubportfolioSummary1.ctx":0EEA
      ToolTipText     =   "Open Help"
      Top             =   480
      Width           =   150
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "DJX equivalent, $"
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
      Index           =   11
      Left            =   240
      TabIndex        =   12
      Top             =   4020
      Width           =   2295
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   11
      X1              =   2520
      X2              =   240
      Y1              =   4260
      Y2              =   4260
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "OEX equivalent, $"
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
      Index           =   10
      Left            =   240
      TabIndex        =   11
      Top             =   3720
      Width           =   2295
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   10
      X1              =   2520
      X2              =   240
      Y1              =   3960
      Y2              =   3960
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "NDX equivalent, $"
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
      Index           =   9
      Left            =   240
      TabIndex        =   10
      Top             =   3420
      Width           =   2295
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   9
      X1              =   2520
      X2              =   240
      Y1              =   3660
      Y2              =   3660
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "SPX equivalent, $"
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
      Index           =   8
      Left            =   240
      TabIndex        =   9
      Top             =   3120
      Width           =   2295
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   8
      X1              =   2520
      X2              =   240
      Y1              =   3360
      Y2              =   3360
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Gain, %"
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
      Left            =   240
      TabIndex        =   8
      Top             =   1080
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Index position, $"
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
      Left            =   240
      TabIndex        =   7
      Top             =   2280
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Unhedged deviation, $"
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
      Index           =   1
      Left            =   240
      TabIndex        =   6
      Top             =   480
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Hedged deviation, $"
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
      Left            =   240
      TabIndex        =   5
      Top             =   780
      Width           =   2295
   End
   Begin VB.Label lblCaption 
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   240
      TabIndex        =   4
      Top             =   120
      Width           =   2595
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Original number of stocks"
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
      Left            =   240
      TabIndex        =   3
      Top             =   1380
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Hedged stocks"
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
      Left            =   240
      TabIndex        =   2
      Top             =   1680
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Hedged position, $"
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
      Left            =   240
      TabIndex        =   1
      Top             =   1980
      Width           =   2295
   End
   Begin VB.Label Label1 
      BackColor       =   &H00000000&
      BackStyle       =   0  'Transparent
      Caption         =   "Remaining position, $"
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
      Index           =   7
      Left            =   240
      TabIndex        =   0
      Top             =   2580
      Width           =   2295
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   6
      X1              =   2520
      X2              =   240
      Y1              =   720
      Y2              =   720
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   0
      X1              =   2520
      X2              =   240
      Y1              =   1020
      Y2              =   1020
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   2520
      X2              =   240
      Y1              =   1320
      Y2              =   1320
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   2
      X1              =   2520
      X2              =   240
      Y1              =   1620
      Y2              =   1620
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   3
      X1              =   2520
      X2              =   240
      Y1              =   1920
      Y2              =   1920
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   4
      X1              =   2520
      X2              =   240
      Y1              =   2220
      Y2              =   2220
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   5
      X1              =   2520
      X2              =   240
      Y1              =   2520
      Y2              =   2520
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   7
      X1              =   2520
      X2              =   240
      Y1              =   2820
      Y2              =   2820
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   13
      Visible         =   0   'False
      X1              =   2520
      X2              =   2520
      Y1              =   4140
      Y2              =   540
   End
End
Attribute VB_Name = "ctlSubportfolioSummary1"
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
    ShowHelpTags
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
        ShowHelpID 201
    Case 1
        ShowHelpID 202
    Case 2
        ShowHelpID 203
    Case 3
        ShowHelpID 204
    Case 4
        ShowHelpID 205
    Case 5
        ShowHelpID 206
    Case 6
        ShowHelpID 207
    Case 7
        ShowHelpID 208
    Case 8
        ShowHelpID 210
    Case Else
        Debug.Assert False
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetVegaMode(ByVal VegaMode As Boolean)
    If VegaMode Then
        Label1(4).Caption = "Original number of contracts"
        Label1(5).Caption = "Hedged contracts"
        Label1(0).Caption = "Equiv index vega"
        Label1(8).Caption = "Equiv SPX vega"
        Label1(9).Caption = "Equiv NDX vega"
        Label1(10).Caption = "Equiv OEX vega"
        Label1(11).Caption = "Equiv DJX vega"
    Else
        Label1(4).Caption = "Original number of stocks"
        Label1(5).Caption = "Hedged stocks"
        Label1(0).Caption = "Equiv index position, $"
        Label1(8).Caption = "Equiv SPX position, $"
        Label1(9).Caption = "Equiv NDX position, $"
        Label1(10).Caption = "Equiv OEX position, $"
        Label1(11).Caption = "Equiv DJX position, $"
    End If
End Sub

