VERSION 5.00
Begin VB.UserControl ctlSubportfolioSummary0 
   BackColor       =   &H00666666&
   ClientHeight    =   9330
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9300
   ScaleHeight     =   9330
   ScaleWidth      =   9300
   Begin VB.PictureBox Picture1 
      BackColor       =   &H00666666&
      BorderStyle     =   0  'None
      Height          =   705
      Left            =   360
      ScaleHeight     =   705
      ScaleWidth      =   6975
      TabIndex        =   6
      Top             =   0
      Width           =   6975
      Begin VB.Label Label2 
         BackColor       =   &H00808080&
         Caption         =   "Possible Hedging"
         ForeColor       =   &H00FFFFFF&
         Height          =   495
         Left            =   0
         TabIndex        =   7
         Top             =   120
         Width           =   6945
      End
   End
   Begin EgarHedgerSQL.ctlSubportfolioSummary1 ctlSubportfolioSummary11 
      Height          =   4335
      Left            =   120
      TabIndex        =   0
      Top             =   600
      Width           =   2535
      _ExtentX        =   4471
      _ExtentY        =   7646
   End
   Begin EgarHedgerSQL.ctlSubportfolioSummary3 ctlSubportfolioSummary31 
      Height          =   4335
      Index           =   0
      Left            =   2640
      TabIndex        =   1
      Top             =   600
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   7646
   End
   Begin EgarHedgerSQL.ctlSubportfolioSummary3 ctlSubportfolioSummary31 
      Height          =   4335
      Index           =   1
      Left            =   4200
      TabIndex        =   2
      Top             =   600
      Width           =   1575
      _ExtentX        =   2778
      _ExtentY        =   7646
   End
   Begin EgarHedgerSQL.ctlSubportfolioSummary3 ctlSubportfolioSummary31 
      Height          =   4335
      Index           =   2
      Left            =   5760
      TabIndex        =   3
      Top             =   600
      Width           =   2055
      _ExtentX        =   20346
      _ExtentY        =   7435
   End
   Begin EgarHedgerSQL.ctlIndexOptions ctlIndexOptions1 
      Height          =   1680
      Index           =   0
      Left            =   375
      TabIndex        =   4
      Top             =   5415
      Width           =   7050
      _ExtentX        =   12091
      _ExtentY        =   2990
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   5
      X1              =   7320
      X2              =   360
      Y1              =   5400
      Y2              =   5400
   End
   Begin VB.Line Line1 
      BorderColor     =   &H00808080&
      Index           =   1
      X1              =   360
      X2              =   360
      Y1              =   6840
      Y2              =   5400
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Index data"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   12
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FFFFFF&
      Height          =   255
      Left            =   360
      TabIndex        =   5
      Top             =   5040
      Width           =   3375
   End
End
Attribute VB_Name = "ctlSubportfolioSummary0"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Set PortfolioHedge = gPortfolioHedge
    If Not PortfolioHedge Is Nothing Then
        SetVegaMode PortfolioHedge.VegaMode
    End If
    ctlSubportfolioSummary31(0).SetCaption "Original"
    ctlSubportfolioSummary31(1).SetCaption "Short Hedge"
    ctlSubportfolioSummary31(2).SetCaption "Long Hedge"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
'    With ctlIndexOptions1(0)
'        .Width = Max(1000, ScaleWidth - .Left)
'        .Height = Max(500, ScaleHeight - .Top)
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ClearResults()
    On Error Resume Next
    Clear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
Dim rs As ADODB.Recordset
Dim i As Long
    With ctlSubportfolioSummary31
        For i = .LBound To .UBound
            .Item(i).Clear
        Next
    End With
    With ctlIndexOptions1
        For i = .LBound To .UBound
            .Item(i).Clear
        Next
    End With
    DisplayOutputRS Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_NewIndexData(rs As ADODB.Recordset)
    On Error GoTo Herr
    DisplayOutputRS rs
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset)
    If PortfolioHedge.HedgeMode = "" Then
        ctlSubportfolioSummary31(0).SetCaption "All Portfolio"
        ctlSubportfolioSummary31(1).SetCaption "Hedge"
        ctlSubportfolioSummary31(2).Visible = False
        ctlSubportfolioSummary31(2).SetCaption "Long"
        With Label2
            .Width = 6940 '5400
            .Caption = Space(4) & "There is one optimal hedge that covers all stocks in the Portfolio. 'Hedge' is the same as for 'All Portfolio'. You can manualy remove positions and compare results. Please use the next page."
        End With
    Else
        ctlSubportfolioSummary31(0).SetCaption "All Portfolio"
        ctlSubportfolioSummary31(1).SetCaption "Short"
        ctlSubportfolioSummary31(2).SetCaption "Long"
        ctlSubportfolioSummary31(2).Visible = True
        With Label2
            .Width = 6940
            .Caption = Space(4) & "There are two ways to hedge Portfolio - 'Short' and 'Long'. These are most effective in hedging terms but do not cover all stocks in the Portfolio. 'All Portfolio' shows a way to hedge all stocks."
        End With
    End If
    
    If Not rs Is Nothing Then
        ctlIndexOptions1(0).DisplayOutputRS rs, ""
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotSubportfolioTotals(ByVal Q As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal Gain As Double, ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double, ByVal HedgeSymbol As String)
Dim i As Long
    i = IndexOf(HedgeSymbol)
    If i >= 0 And i <= 2 Then
        ctlSubportfolioSummary31(i).SetTotals Q, D2, d3, Gain
        ctlSubportfolioSummary31(i).SetTotals3 Q1, Q2, Q3, Q4
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_VegaModeChanged(ByVal NewVegaMode As Boolean)
    On Error Resume Next
    SetVegaMode NewVegaMode
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetVegaMode(ByVal VegaMode As Boolean)
    ctlSubportfolioSummary11.SetVegaMode VegaMode
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function IndexOf(ByVal HedgeSymbol As String)
Dim i As Long
    i = -1
    If HedgeSymbol = "ALL" Then
        i = 0
    ElseIf HedgeSymbol = "OPT" Then
        i = 1
    ElseIf HedgeSymbol = "SHORT" Then
        i = 1
    ElseIf HedgeSymbol = "LONG" Then
        i = 2
    End If
    IndexOf = i
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotSubportfolioTotals2(ByVal d1 As Long, ByVal D2 As Long, ByVal d3 As Double, ByVal D4 As Double, ByVal HedgeSymbol As String)
Dim i As Long
    On Error Resume Next
    i = IndexOf(HedgeSymbol)
    If i >= 0 And i <= 2 Then
        ctlSubportfolioSummary31(i).SetTotals2 d1, D2, d3, D4
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlIndexOptions1_IndexPriceChanged(Index As Integer, ByVal IndexSymbol As String, ByVal IndexPrice As Double, ContractsAmount As Long, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
'    RaiseEvent IndexPriceChanged(IndexSymbol, IndexPrice, ContractsAmount, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexPriceData(ByVal IndexSymbol As String, ByVal IndexPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexOptions1(0).SetIndexPriceData IndexSymbol, IndexPrice, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexHVData(ByVal IndexSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexOptions1(0).SetIndexHVData IndexSymbol, HV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexIVData(ByVal IndexSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexOptions1(0).SetIndexIVData IndexSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexHVData3(ByVal IndexSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    If HV <> 0 Then
        ctlIndexOptions1(0).SetIndexHVData IndexSymbol, HV, DataStatus
    End If
    If IV <> 0 Then
        ctlIndexOptions1(0).SetIndexIVData IndexSymbol, IV, DataStatus
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ShowHelpTipsChanged()
    On Error Resume Next
    ctlSubportfolioSummary11.ShowHelpTags
    ctlIndexOptions1(0).ShowHelpTags
End Sub

