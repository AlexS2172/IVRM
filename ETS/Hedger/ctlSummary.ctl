VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlSummary 
   BackColor       =   &H00319ECE&
   ClientHeight    =   7425
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   13965
   ScaleHeight     =   7425
   ScaleWidth      =   13965
   Begin EgarHedgerSQL.ctlSubportfolioSummary0 ctlSubportfolioSummary01 
      Height          =   6975
      Left            =   1680
      TabIndex        =   6
      Top             =   120
      Width           =   8895
      _ExtentX        =   15690
      _ExtentY        =   12303
   End
   Begin VB.PictureBox picLeft 
      Appearance      =   0  'Flat
      BackColor       =   &H00999999&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   7065
      Left            =   0
      ScaleHeight     =   7065
      ScaleWidth      =   1560
      TabIndex        =   0
      Top             =   0
      Width           =   1560
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   1
         Tag             =   "red"
         ToolTipText     =   "Start volatility and correlations data feed for Indexes and Stocks"
         Top             =   3840
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8454016
         Caption         =   "Get IV data"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   0
         Left            =   120
         TabIndex        =   2
         Tag             =   "red"
         ToolTipText     =   "Start price feed for Indexes"
         Top             =   5760
         Visible         =   0   'False
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
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
         HoverForeColor  =   8454016
         Caption         =   "Get prices"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   2
         Left            =   120
         TabIndex        =   4
         Tag             =   "red"
         ToolTipText     =   "Go to the subportfolios summary page"
         Top             =   4680
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8454016
         Caption         =   "Hedge"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Next step:"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00333333&
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   5
         Top             =   4440
         Width           =   1335
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   1
         X1              =   120
         X2              =   1440
         Y1              =   4320
         Y2              =   4320
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   0
         X1              =   120
         X2              =   1440
         Y1              =   3480
         Y2              =   3480
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Import data:"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00333333&
         Height          =   255
         Index           =   2
         Left            =   120
         TabIndex        =   3
         Top             =   3600
         Width           =   1335
      End
   End
End
Attribute VB_Name = "ctlSummary"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Event ButtonClick(Index As Integer)

Private Const Border = 60

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    BackColor = GCOLOR_TAB_2
    Set PortfolioHedge = gPortfolioHedge
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btn_Click(Index As Integer)
    On Error Resume Next
    RaiseEvent ButtonClick(Index)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    EnableControls False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    EnableControls True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EnableControls(ByVal Enable As Boolean)
Dim i As Long
    On Error Resume Next
    With btn
        For i = .LBound To .UBound
            .Item(i).Enabled = Enable
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    picLeft.Height = ScaleHeight
    With ctlSubportfolioSummary01
        .Top = Border
        .Left = picLeft.Width + Border
        .Width = Max(0, ScaleWidth - .Left - Border)
        .Height = Max(0, ScaleHeight - .Top - Border)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
    ctlSubportfolioSummary01.Clear
End Sub

