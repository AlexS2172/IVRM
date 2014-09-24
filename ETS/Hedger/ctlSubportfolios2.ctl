VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlSubportfolios2 
   BackColor       =   &H00639E31&
   ClientHeight    =   6270
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9810
   ScaleHeight     =   6270
   ScaleWidth      =   9810
   Begin VB.PictureBox picLeft 
      Appearance      =   0  'Flat
      BackColor       =   &H00999999&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   5985
      Left            =   0
      ScaleHeight     =   5985
      ScaleWidth      =   1560
      TabIndex        =   4
      Top             =   0
      Width           =   1560
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   5
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
         TabIndex        =   6
         Tag             =   "red"
         ToolTipText     =   "Start price feed for Stocks"
         Top             =   5520
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
         TabIndex        =   7
         Tag             =   "red"
         ToolTipText     =   "Go to the subportfolios summary page"
         Top             =   4680
         Visible         =   0   'False
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
         Caption         =   "Summary"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
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
         Index           =   1
         Left            =   120
         TabIndex        =   9
         Top             =   3600
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
         Index           =   2
         Left            =   120
         TabIndex        =   8
         Top             =   4440
         Visible         =   0   'False
         Width           =   1335
      End
   End
   Begin EgarHedgerSQL.ctlSubportfolio2 ctlSubportfolio21 
      Height          =   2655
      Index           =   1
      Left            =   1560
      TabIndex        =   1
      Top             =   3120
      Width           =   3975
      _ExtentX        =   7011
      _ExtentY        =   8493
   End
   Begin EgarHedgerSQL.ctlSubportfolio2 ctlSubportfolio21 
      Height          =   2655
      Index           =   0
      Left            =   1560
      TabIndex        =   0
      Top             =   0
      Width           =   3975
      _ExtentX        =   7011
      _ExtentY        =   8493
   End
   Begin EgarHedgerSQL.ctlSubportfolio2 ctlSubportfolio21 
      Height          =   2655
      Index           =   2
      Left            =   5640
      TabIndex        =   2
      Top             =   0
      Width           =   3975
      _ExtentX        =   7011
      _ExtentY        =   4683
   End
   Begin EgarHedgerSQL.ctlSubportfolio2 ctlSubportfolio21 
      Height          =   2655
      Index           =   3
      Left            =   5640
      TabIndex        =   3
      Top             =   3120
      Width           =   3975
      _ExtentX        =   7011
      _ExtentY        =   8493
   End
End
Attribute VB_Name = "ctlSubportfolios2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Public Event ButtonClick(Index As Integer)

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Enum enmSubportfolios2
    HDG_SHORT = 0
    HDG_LONG
    HDG_NONE
    HDG_NONE2
End Enum

Private Const Border = 60
Private Editable As Boolean
Private mShortMode As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    BackColor = GCOLOR_TAB_3
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
    With ctlSubportfolio21
        For i = .LBound To .UBound
            .Item(i).Editable = Enable
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
Dim X0 As Single
Dim X1 As Single
Dim Y0 As Single
Dim y1 As Single
Dim y2 As Single
Dim y3 As Single

Dim W1 As Single
Dim H1 As Single
Dim W2 As Single
Dim H2 As Single
Dim H3 As Single

Dim ControlsCount As Long
Dim o(0 To 4) As Long
Dim i As Long
    On Error Resume Next
'
'    ControlsCount = 0
'    For i = 0 To 3
'        If ctlSubportfolio21(i).Visible Then
'            o(ControlsCount) = i
'            ControlsCount = ControlsCount + 1
'        End If
'    Next
'
'    If ControlsCount < 1 Then
'        Exit Sub
'    End If
    
    picLeft.Height = ScaleHeight
    
    X0 = picLeft.Width + Border
    Y0 = 0 + Border
    W1 = Max(0, (ScaleWidth - picLeft.Width - 2 * Border))
    W2 = Max(0, (ScaleWidth - picLeft.Width - 3 * Border) / 2)
    X1 = X0 + W2 + Border
    H1 = Max(0, (ScaleHeight - 2 * Border))
    H2 = Max(0, (ScaleHeight - 3 * Border) / 2)
    y1 = Y0 + H2 + Border
    H3 = Max(0, (ScaleHeight - 4 * Border) / 3)
    y2 = Y0 + H3 + Border
    y3 = y2 + H3 + Border
    
    With ctlSubportfolio21(0)
        .Left = X0
        .Top = Y0
        .Width = W2
        .Height = H1
    End With
    With ctlSubportfolio21(1)
        .Left = X0
        .Top = Y0
        .Width = W2
        .Height = H1
    End With
    With ctlSubportfolio21(2)
        .Left = X1
        .Top = Y0
        .Width = W2
        .Height = H1
    End With
    With ctlSubportfolio21(3)
        .Left = X1
        .Top = Y0
        .Width = W2
        .Height = H1
    End With
    
'    Select Case ControlsCount
'    Case 0
'    Case 1
'        With ctlSubportfolio21(o(0))
'            .Left = X0
'            .Top = Y0
'            .Width = W1
'            .Height = H1
'        End With
'    Case 2
'        With ctlSubportfolio21(o(0))
'            .Left = X0
'            .Top = Y0
'            .Width = W2
'            .Height = H1
'        End With
'        With ctlSubportfolio21(o(1))
'            .Left = X1
'            .Top = Y0
'            .Width = W2
'            .Height = H1
'        End With
'    Case 3
'        With ctlSubportfolio21(o(0))
'            .Left = X0
'            .Top = Y0
'            .Width = W2
'            .Height = H1
'        End With
'        With ctlSubportfolio21(o(1))
'            .Left = X1
'            .Top = Y0
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(2))
'            .Left = X1
'            .Top = Y1
'            .Width = W2
'            .Height = H2
'        End With
'    Case 4
'        With ctlSubportfolio21(o(0))
'            .Left = X0
'            .Top = Y0
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(1))
'            .Left = X0
'            .Top = Y1
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(2))
'            .Left = X1
'            .Top = Y0
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(3))
'            .Left = X1
'            .Top = Y1
'            .Width = W2
'            .Height = H2
'        End With
'    Case 5
'        With ctlSubportfolio21(o(0))
'            .Left = X0
'            .Top = Y0
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(1))
'            .Left = X0
'            .Top = Y1
'            .Width = W2
'            .Height = H2
'        End With
'        With ctlSubportfolio21(o(2))
'            .Left = X1
'            .Top = Y0
'            .Width = W2
'            .Height = H3
'        End With
'        With ctlSubportfolio21(o(3))
'            .Left = X1
'            .Top = Y2
'            .Width = W2
'            .Height = H3
'        End With
'        With ctlSubportfolio21(o(4))
'            .Left = X1
'            .Top = Y3
'            .Width = W2
'            .Height = H3
'        End With
'    Case Else
'        '
'    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
Dim i As Long
    With ctlSubportfolio21
        For i = .LBound To .UBound
            .Item(i).Clear
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset)
Dim Count As Long
    Editable = False
    If PortfolioHedge.HedgeMode = "" Then
        ctlSubportfolio21(0).Visible = True
        ctlSubportfolio21(1).Visible = False
        ctlSubportfolio21(2).Visible = True
        ctlSubportfolio21(3).Visible = False
        ctlSubportfolio21(0).DisplayOutputRS rs, "OPT"
        ctlSubportfolio21(2).DisplayOutputRS rs, "NONE"
    Else
        If mShortMode Then
            ctlSubportfolio21(0).Visible = True
            ctlSubportfolio21(1).Visible = False
            ctlSubportfolio21(2).Visible = True
            ctlSubportfolio21(3).Visible = False
        Else
            ctlSubportfolio21(0).Visible = False
            ctlSubportfolio21(1).Visible = True
            ctlSubportfolio21(2).Visible = False
            ctlSubportfolio21(3).Visible = True
        End If
        ctlSubportfolio21(0).DisplayOutputRS rs, "SHORT"
        ctlSubportfolio21(1).DisplayOutputRS rs, "LONG"
        ctlSubportfolio21(2).DisplayOutputRS rs, "NOT_SHORT"
        ctlSubportfolio21(3).DisplayOutputRS rs, "NOT_LONG"
    End If
'    UserControl_Resize
    If Not rs Is Nothing Then
        PortfolioHedge.CalculateContractAmmounts
    End If
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let ShortMode(ByVal vNewValue As Boolean)
    mShortMode = vNewValue
    If vNewValue Then
        BackColor = GCOLOR_TAB_3
    Else
        BackColor = GCOLOR_TAB_4
    End If
    If PortfolioHedge.HedgeMode = "" Then
    Else
        If vNewValue Then
            BackColor = &H639E31
            ctlSubportfolio21(0).Visible = True
            ctlSubportfolio21(1).Visible = False
            ctlSubportfolio21(2).Visible = True
            ctlSubportfolio21(3).Visible = False
        Else
            ctlSubportfolio21(0).Visible = False
            ctlSubportfolio21(1).Visible = True
            ctlSubportfolio21(2).Visible = False
            ctlSubportfolio21(3).Visible = True
        End If
    End If
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Refresh()
    '!!!
    UserControl_Resize
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    With PortfolioHedge
        If Not .rsPositions Is Nothing Then
            With .rsPositions.Clone
                .Filter = "StockSymbol='" & StockSymbol & "'"
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        !StockPrice = StockPrice
                        .Update
                        .MoveNext
                    Loop
                End If
            End With
        End If
    End With
    Dim Count As Long
    With ctlSubportfolio21
        For Count = .LBound To .UBound
            .Item(Count).SetStockPriceData StockSymbol, StockPrice, DataStatus
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    With PortfolioHedge
        If Not .rsPositions Is Nothing Then
            With .rsPositions.Clone
                .Filter = "StockSymbol='" & StockSymbol & "'"
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        !HV = HV
                        .Update
                        .MoveNext
                    Loop
                End If
            End With
        End If
    End With
    Dim Count As Long
    With ctlSubportfolio21
        For Count = .LBound To .UBound
            .Item(Count).SetStockHVData StockSymbol, HV, DataStatus
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    With PortfolioHedge
        If Not .rsPositions Is Nothing Then
            With .rsPositions.Clone
                .Filter = "StockSymbol='" & StockSymbol & "'"
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        !IV = IV
                        .Update
                        .MoveNext
                    Loop
                End If
            End With
        End If
    End With
    Dim Count As Long
    With ctlSubportfolio21
        For Count = .LBound To .UBound
            .Item(Count).SetStockIVData StockSymbol, IV, DataStatus
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockHVData3(ByVal StockSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    PortfolioHedge_GotStockHVData StockSymbol, HV, DataStatus
    PortfolioHedge_GotStockIVData StockSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubportfolio21_PositionMoved(Index As Integer, ByVal RowID As Variant, ByVal NewHedgingSymbol As String, ByVal OldHedgingSymbol As String)
Dim Count As Long
Dim HedgeSymbol As String
    On Error GoTo Herr
    If PortfolioHedge.rsPositions Is Nothing Then
        Exit Sub
    End If
    With PortfolioHedge.rsPositions
        .Bookmark = RowID
        If Not .EOF Then
            
            Select Case OldHedgingSymbol
            Case "LONG"
                !LongHedge = False
            Case "SHORT"
                !ShortHedge = False
            Case "OPT"
                !LongHedge = False
                !ShortHedge = False
            End Select
            
            Select Case NewHedgingSymbol
            Case "LONG"
                !LongHedge = True
            Case "SHORT"
                !ShortHedge = True
            Case "OPT"
                !LongHedge = True
                !ShortHedge = True
            Case "NONE"
                '???
            End Select
            
            .Update
        End If
    End With
    With ctlSubportfolio21
        For Count = .LBound To .UBound
            With .Item(Count)
                HedgeSymbol = .HedgeSymbolFilter
                If HedgeSymbol = NewHedgingSymbol Then
                    .DisplayOutputRS PortfolioHedge.rsPositions, HedgeSymbol, RowID
                Else
                    .DisplayOutputRS PortfolioHedge.rsPositions, HedgeSymbol
                End If
            End With
        Next
    End With
    PortfolioHedge.CalculateContractAmmounts
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGrid(ByVal Index As Integer) As VSFlexGrid
    On Error Resume Next
    Set GetGrid = ctlSubportfolio21(Index).GetGrid
End Function

