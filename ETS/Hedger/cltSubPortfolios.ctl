VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlSubPortfolios 
   BackColor       =   &H00009966&
   ClientHeight    =   9090
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   14760
   ScaleHeight     =   9090
   ScaleWidth      =   14760
   Begin EgarHedger.ctlSubPortfolio ctlSubPortfolio1 
      Height          =   4455
      Index           =   0
      Left            =   1680
      TabIndex        =   12
      Top             =   120
      Width           =   5895
      _ExtentX        =   11880
      _ExtentY        =   5106
   End
   Begin VB.PictureBox picLeft 
      Appearance      =   0  'Flat
      BackColor       =   &H00999999&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   5985
      Left            =   0
      ScaleHeight     =   5985
      ScaleWidth      =   1560
      TabIndex        =   0
      Top             =   0
      Width           =   1560
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
         Caption         =   "None"
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
         Index           =   4
         Left            =   120
         TabIndex        =   11
         Top             =   1920
         Value           =   1  'Checked
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
         Caption         =   "DJX"
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
         Index           =   3
         Left            =   120
         TabIndex        =   10
         Top             =   1560
         Value           =   1  'Checked
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
         Caption         =   "OEX"
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
         Index           =   2
         Left            =   120
         TabIndex        =   9
         Top             =   1200
         Value           =   1  'Checked
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
         Caption         =   "NDX"
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
         Index           =   1
         Left            =   120
         TabIndex        =   8
         Top             =   840
         Value           =   1  'Checked
         Width           =   855
      End
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
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
         Index           =   0
         Left            =   120
         TabIndex        =   7
         Top             =   480
         Value           =   1  'Checked
         Width           =   855
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   1
         Tag             =   "red"
         Top             =   4200
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
         Caption         =   "Get prices"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   2
         Left            =   120
         TabIndex        =   6
         Tag             =   "red"
         Top             =   5280
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
         TabIndex        =   5
         Top             =   5040
         Width           =   1335
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
         TabIndex        =   4
         Top             =   3600
         Width           =   1335
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Indexes:"
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
         TabIndex        =   3
         Top             =   120
         Width           =   1335
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   1
         X1              =   120
         X2              =   1440
         Y1              =   4920
         Y2              =   4920
      End
   End
   Begin EgarHedger.ctlSubPortfolio ctlSubPortfolio1 
      Height          =   3855
      Index           =   1
      Left            =   1680
      TabIndex        =   13
      Top             =   4680
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   5106
   End
   Begin EgarHedger.ctlSubPortfolio ctlSubPortfolio1 
      Height          =   2655
      Index           =   2
      Left            =   7680
      TabIndex        =   14
      Top             =   120
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   4683
   End
   Begin EgarHedger.ctlSubPortfolio ctlSubPortfolio1 
      Height          =   2895
      Index           =   3
      Left            =   7680
      TabIndex        =   15
      Top             =   2880
      Width           =   5895
      _ExtentX        =   10398
      _ExtentY        =   5106
   End
   Begin EgarHedger.ctlSubPortfolio ctlSubPortfolio1 
      Height          =   2655
      Index           =   4
      Left            =   7680
      TabIndex        =   16
      Top             =   5880
      Width           =   5895
      _ExtentX        =   11880
      _ExtentY        =   5106
   End
End
Attribute VB_Name = "ctlSubPortfolios"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Event ButtonClick(Index As Integer)
Public Event IndexSelectedChanged(ByVal IndexSymbol As String, ByVal IndexSelected As Boolean)
Public Event IndexVisibleChanged(ByVal IndexSymbol As String, ByVal IndexVisible As Boolean)

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Enum INDEXES
    IND_SPX = 0
    IND_NDX
    IND_OEX
    IND_DJX
    IND_NONE
End Enum

Private Const Border = 60
Private ControlsCount As Long
Private Editable As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btn_Click(Index As Integer)
    On Error Resume Next
    RaiseEvent ButtonClick(Index)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    EnableControls False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    EnableControls True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EnableControls(ByVal Enable As Boolean)
Dim i As Long
    On Error Resume Next
    For i = 0 To 2
        btn(i).Enabled = Enable
    Next
    For i = 0 To 4
        Check1(i).Enabled = Enable
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
Dim X0 As Single
Dim X1 As Single
Dim Y0 As Single
Dim Y1 As Single
Dim Y2 As Single
Dim Y3 As Single

Dim W1 As Single
Dim H1 As Single
Dim W2 As Single
Dim H2 As Single
Dim H3 As Single
    On Error Resume Next
    picLeft.Height = ScaleHeight
    
    X0 = picLeft.Width + Border
    Y0 = 0 + Border
    W1 = Max(0, (ScaleWidth - picLeft.Width - 2 * Border))
    W2 = Max(0, (ScaleWidth - picLeft.Width - 3 * Border) / 2)
    X1 = X0 + W2 + Border
    H1 = Max(0, (ScaleHeight - 2 * Border))
    H2 = Max(0, (ScaleHeight - 3 * Border) / 2)
    Y1 = Y0 + H2 + Border
    H3 = Max(0, (ScaleHeight - 4 * Border) / 3)
    Y2 = Y0 + H3 + Border
    Y3 = Y2 + H3 + Border
    
    Select Case ControlsCount
    Case 0
    Case 1
        With ctlSubPortfolio1(0)
            .Left = X0
            .Top = Y0
            .Width = W1
            .Height = H1
        End With
    Case 2
        With ctlSubPortfolio1(0)
            .Left = X0
            .Top = Y0
            .Width = W2
            .Height = H1
        End With
        With ctlSubPortfolio1(1)
            .Left = X1
            .Top = Y0
            .Width = W2
            .Height = H1
        End With
    Case 3
        With ctlSubPortfolio1(0)
            .Left = X0
            .Top = Y0
            .Width = W2
            .Height = H1
        End With
        With ctlSubPortfolio1(1)
            .Left = X1
            .Top = Y0
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(2)
            .Left = X1
            .Top = Y1
            .Width = W2
            .Height = H2
        End With
    Case 4
        With ctlSubPortfolio1(0)
            .Left = X0
            .Top = Y0
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(1)
            .Left = X0
            .Top = Y1
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(2)
            .Left = X1
            .Top = Y0
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(3)
            .Left = X1
            .Top = Y1
            .Width = W2
            .Height = H2
        End With
    Case 5
        With ctlSubPortfolio1(0)
            .Left = X0
            .Top = Y0
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(1)
            .Left = X0
            .Top = Y1
            .Width = W2
            .Height = H2
        End With
        With ctlSubPortfolio1(2)
            .Left = X1
            .Top = Y0
            .Width = W2
            .Height = H3
        End With
        With ctlSubPortfolio1(3)
            .Left = X1
            .Top = Y2
            .Width = W2
            .Height = H3
        End With
        With ctlSubPortfolio1(4)
            .Left = X1
            .Top = Y3
            .Width = W2
            .Height = H3
        End With
    Case Else
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
Dim i As Long
    For i = 0 To 4
        ctlSubPortfolio1(i).ClearGrid
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset)
Dim IndexSymbol As String
Dim IsVisible As Boolean
Dim IndexCode As INDEXES
Dim Count As Long
    Editable = False
    For Count = 0 To 4
        Set ctlSubPortfolio1(Count).PortfolioHedge = PortfolioHedge
    Next
    Count = 0
    With PortfolioHedge.rsIndexOptions.Clone
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                IndexSymbol = !IndexSymbol
                IsVisible = !Visible
                Select Case IndexSymbol
                Case "SPX"
                    IndexCode = IND_SPX
                Case "NDX"
                    IndexCode = IND_NDX
                Case "OEX"
                    IndexCode = IND_OEX
                Case "DJX"
                    IndexCode = IND_DJX
                Case "NONE"
                    IndexCode = IND_NONE
                    ' Display 'NONE' subportfolio if no records
                    If rs.RecordCount = 0 Then
                        IsVisible = True
                    End If
                Case Else
                    IndexCode = -1
                End Select
                If IndexCode >= 0 Then
                    Check1(IndexCode).Value = IIf(IsVisible, 1, 0)
                End If
                If IsVisible Then
                    ctlSubPortfolio1(Count).DisplayOutputRS rs, IndexSymbol
                    ctlSubPortfolio1(Count).Visible = True
                    Count = Count + 1
                End If
                .MoveNext
            Loop
        End If
    End With
    ControlsCount = Count
    Do While Count < 5
        ctlSubPortfolio1(Count).Visible = False
        Count = Count + 1
    Loop
    
    UserControl_Resize

'    ctlSubPortfolio1(0).DisplayOutputRS rs, "SPX"
'    ctlSubPortfolio1(1).DisplayOutputRS rs, "NDX"
'    ctlSubPortfolio1(2).DisplayOutputRS rs, "OEX"
'    ctlSubPortfolio1(3).DisplayOutputRS rs, "DJX"
'    ctlSubPortfolio1(4).DisplayOutputRS rs, "NONE"
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateOutputRS(rs As ADODB.Recordset)
Dim IndexSymbol As String
Dim IsVisible As Boolean
Dim IndexCode As INDEXES
Dim Count As Long
    Editable = False
    DoEvents
    Count = 0
    With PortfolioHedge.rsIndexOptions
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                IndexSymbol = !IndexSymbol
                IsVisible = !Visible
                Select Case IndexSymbol
                Case "SPX"
                    IndexCode = IND_SPX
                Case "NDX"
                    IndexCode = IND_NDX
                Case "OEX"
                    IndexCode = IND_OEX
                Case "DJX"
                    IndexCode = IND_DJX
                Case "NONE"
                    IndexCode = IND_NONE
                Case Else
                    IndexCode = -1
                End Select
                If IndexCode >= 0 Then
                    Check1(IndexCode).Value = IIf(IsVisible, 1, 0)
                End If
                If IsVisible Then
                    ctlSubPortfolio1(Count).Visible = True
                    Count = Count + 1
                End If
                .MoveNext
            Loop
        End If
    End With
    ControlsCount = Count
    Do While Count < 5
        ctlSubPortfolio1(Count).Visible = False
        Count = Count + 1
    Loop
    
    UserControl_Resize
    
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateSummaries()
Dim Count As Long
    For Count = 0 To 4
        ctlSubPortfolio1(Count).UpdateSummary
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolio1_StockHedgingIndexChanged(Index As Integer, ByVal RowID As Variant, HedgingIndexSymbol As String, StockBeta As Double, StockCorr As Double)
Dim Criteria As String
Dim StockSymbol As String
Dim PrevHedgingIndexSymbol As String
    On Error GoTo Herr
    If PortfolioHedge.rsStockData Is Nothing Then
        StockBeta = 0
        StockCorr = 0
        Exit Sub
    End If
    With PortfolioHedge.rsPositions
        .Bookmark = RowID
        If Not .EOF Then
            StockSymbol = !StockSymbol

            With PortfolioHedge.rsStockData.Clone
                .Filter = "IndexSymbol='" & HedgingIndexSymbol & "'"
                If .RecordCount > 0 Then
                    .MoveFirst
                End If
                Criteria = "StockSymbol='" & StockSymbol & "'"
                .Find Criteria, 0, adSearchForward, adBookmarkFirst
                If Not .EOF Then
                    StockBeta = !Beta
                    StockCorr = !Corr
                Else
                    ' !!! Not found
                    StockBeta = 0
                    StockCorr = 0
                    Exit Sub
                End If
            End With
            If IsNull(!HedgingIndexSymbol) Then
                PrevHedgingIndexSymbol = ""
            Else
                PrevHedgingIndexSymbol = !HedgingIndexSymbol
            End If
            
            .Filter = "StockSymbol='" & StockSymbol & "'"
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    !HedgingIndexSymbol = HedgingIndexSymbol
                    !Beta = StockBeta
                    !Corr = StockCorr
                    .Update
                    .MoveNext
                Loop
            End If
            
            If PrevHedgingIndexSymbol <> HedgingIndexSymbol Then
                MoveStock StockSymbol, PrevHedgingIndexSymbol, HedgingIndexSymbol
            End If
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolio1_StockHedgingIndicesListAdvise(Index As Integer, ByVal StockSymbol As String, HedgingIndexSymbolList As String)
Dim Criteria As String
Dim IndexSymbol As String
    On Error GoTo Herr
    HedgingIndexSymbolList = ""
    If PortfolioHedge.rsStockData Is Nothing Then
        Exit Sub
    End If
    With PortfolioHedge.rsStockData.Clone
        Criteria = "StockSymbol='" & StockSymbol & "'"
        .Filter = Criteria
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                IndexSymbol = !IndexSymbol
                With PortfolioHedge.rsIndexOptions.Clone
                    If .RecordCount > 0 Then
                        .MoveFirst
                    End If
                    .Find "IndexSymbol='" & IndexSymbol & "'", 0, adSearchForward, adBookmarkFirst
                    If Not .EOF Then
                        If !Selected Then
                            HedgingIndexSymbolList = HedgingIndexSymbolList & IndexSymbol & "|"
                        End If
                    End If
                End With
                .MoveNext
            Loop
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Check1_Click(Index As Integer)
Dim IndexSymbol As String
Dim Selected As Boolean
    On Error GoTo Herr
    Select Case Index
    Case IND_SPX
        IndexSymbol = "SPX"
    Case IND_NDX
        IndexSymbol = "NDX"
    Case IND_OEX
        IndexSymbol = "OEX"
    Case IND_DJX
        IndexSymbol = "DJX"
    Case IND_NONE
        IndexSymbol = "NONE"
    Case Else
        Exit Sub
    End Select
    
    Selected = Check1(Index).Value <> 0
    If Editable Then
'        RaiseEvent IndexSelectedChanged(IndexSymbol, Selected)
        RaiseEvent IndexVisibleChanged(IndexSymbol, Selected)
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub MoveStock(ByVal StockSymbol As String, ByVal PrevHedgingIndexSymbol As String, ByVal HedgingIndexSymbol As String)
Dim Count As Long
Dim NeedShowIt As String
    frmMain.StartTask "Calculating..."
    NeedShowIt = HedgingIndexSymbol
    If Len(NeedShowIt) > 0 Then
        RaiseEvent IndexVisibleChanged(NeedShowIt, True)
    End If
    For Count = 0 To 4
        If ctlSubPortfolio1(Count).IndexSymbolFilter = HedgingIndexSymbol Then
            ctlSubPortfolio1(Count).DisplayOutputRS PortfolioHedge.rsPositions, HedgingIndexSymbol
            If ctlSubPortfolio1(Count).Visible Then
                NeedShowIt = ""
            End If
        End If
        If ctlSubPortfolio1(Count).IndexSymbolFilter = PrevHedgingIndexSymbol Then
            ctlSubPortfolio1(Count).UpdateTotals PortfolioHedge.rsPositions
        End If
    Next
    frmMain.StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlSubPortfolio1_StockMoved(Index As Integer, ByVal RowID As Variant, ByVal StockSymbol As String, ByVal PrevHedgingIndexSymbol As String, ByVal NewHedgingIndexSymbol As String, SelectedRow As Long)
Dim Count As Long
Dim Criteria As String
Dim StockCorr As Double
Dim StockBeta As Double

    On Error GoTo Herr
    If PortfolioHedge.rsStockData Is Nothing Then
        StockBeta = 0
        StockCorr = 0
        Exit Sub
    End If
    With PortfolioHedge.rsPositions
        .Bookmark = RowID
        If Not .EOF Then
            StockSymbol = !StockSymbol
            If NewHedgingIndexSymbol = "NONE" Then
                StockBeta = 0
                StockCorr = 0
            Else
            With PortfolioHedge.rsStockData.Clone
                .Filter = "IndexSymbol='" & NewHedgingIndexSymbol & "'"
                If .RecordCount > 0 Then
                   .MoveFirst
                End If
                Criteria = "StockSymbol='" & StockSymbol & "'"
                .Find Criteria, 0, adSearchForward, adBookmarkFirst
                If Not .EOF Then
                    StockBeta = !Beta
                    StockCorr = !Corr
                Else
                    ' !!! Not found
                    StockBeta = 0
                    StockCorr = 0
                End If
            End With
            End If
            
            '////////////////////////////
            ' Check minimal correlation
            If NewHedgingIndexSymbol <> "NONE" Then
                If StockCorr = 0 Then
                    If vbOK = gCmn.MyMsgBox( _
                        "The correlation of the Stock with target Index is zero." _
                        & vbCrLf & "This is not allowed." _
                        , vbCritical Or vbOKOnly) Then
                        Exit Sub
                    End If
                Else
                    If PortfolioHedge.CorrelationBarrier > 0 Then
                        If StockCorr < PortfolioHedge.CorrelationBarrier Then
                            If vbYes <> gCmn.MyMsgBox( _
                                "The correlation of the Stock with target Index is less than Correlation Barrier." _
                                & vbCrLf & "Do you want to continue?" _
                                , vbExclamation Or vbYesNo) Then
                                Exit Sub
                            End If
                        End If
                    End If
                End If
            End If
            
            .Filter = "StockSymbol='" & StockSymbol & "'"
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    If NewHedgingIndexSymbol = "NONE" Then
                        !HedgingIndexSymbol = Null
                        !Beta = 0
                        !Corr = 0
                    Else
                        !HedgingIndexSymbol = NewHedgingIndexSymbol
                        !Beta = StockBeta
                        !Corr = StockCorr
                    End If
                    .Update
                    .MoveNext
                Loop
            End If
        End If
    End With

    Dim NeedShowIt As String
    NeedShowIt = NewHedgingIndexSymbol
    If Len(NeedShowIt) > 0 Then
        RaiseEvent IndexVisibleChanged(NeedShowIt, True)
    End If
    For Count = 0 To 4
        If ctlSubPortfolio1(Count).IndexSymbolFilter = NewHedgingIndexSymbol Then
            ctlSubPortfolio1(Count).DisplayOutputRS PortfolioHedge.rsPositions, NewHedgingIndexSymbol, StockSymbol, SelectedRow
            If ctlSubPortfolio1(Count).Visible Then
                NeedShowIt = ""
            End If
        End If
        If ctlSubPortfolio1(Count).IndexSymbolFilter = PrevHedgingIndexSymbol Then
            ctlSubPortfolio1(Count).DisplayOutputRS PortfolioHedge.rsPositions, PrevHedgingIndexSymbol
        End If
    Next
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Count As Long
Dim Criteria As String
Dim IndexSymbol As String
    With PortfolioHedge
        If Not .rsPositions Is Nothing Then
            With .rsPositions
                If .RecordCount > 0 Then
                    .MoveFirst
                End If
                Criteria = "StockSymbol='" & StockSymbol & "'"
                .Find Criteria, 0, adSearchForward, adBookmarkFirst
                If Not .EOF Then
                    If IsNull(!HedgingIndexSymbol) Then
                        IndexSymbol = "NONE"
                    Else
                        IndexSymbol = !HedgingIndexSymbol
                    End If
                    !StockPrice = StockPrice
                    .Update
                    For Count = 0 To 4
                        If ctlSubPortfolio1(Count).IndexSymbolFilter = IndexSymbol Then
                            ctlSubPortfolio1(Count).SetStockPriceData StockSymbol, StockPrice, DataStatus
                        End If
                    Next
                End If
            End With
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockIndexBetaCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal Beta As Double, ByVal DateSet As Date, ByVal DataStatus As enmDATA_STATUS)
Dim Count As Long
Dim Criteria As String
Dim HedgingIndexSymbol As String
    With PortfolioHedge
        If Not .rsPositions Is Nothing Then
            With .rsPositions
                If .RecordCount > 0 Then
                    .MoveFirst
                End If
                Criteria = "StockSymbol='" & StockSymbol & "'"
                .Find Criteria, 0, adSearchForward, adBookmarkFirst
                If Not .EOF Then
                    If IsNull(!HedgingIndexSymbol) Then
                        HedgingIndexSymbol = "NONE"
                    Else
                        HedgingIndexSymbol = !HedgingIndexSymbol
                    End If
                    If HedgingIndexSymbol = IndexSymbol Then
                        !Beta = Beta
                        !Corr = Corr
                        .Update
                    End If
                    For Count = 0 To 4
                        If ctlSubPortfolio1(Count).IndexSymbolFilter = HedgingIndexSymbol Then
                            ctlSubPortfolio1(Count).SetStockIndexBetaCorrData StockSymbol, IndexSymbol, Corr, Beta, DateSet, DataStatus
                        End If
                    Next
                End If
            End With
        End If
    End With
End Sub

