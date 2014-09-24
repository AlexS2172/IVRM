VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{E395ECF2-0B93-464D-AD1B-99A443BDF17F}#1.0#0"; "FocusFlatControls1.ocx"
Begin VB.UserControl ctlPortfolio 
   BackColor       =   &H00333399&
   ClientHeight    =   6585
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8730
   ScaleHeight     =   6585
   ScaleWidth      =   8730
   Begin EgarHedgerSQL.ctlPortfolioHedge ctlPortfolioHedge1 
      Height          =   3735
      Left            =   1680
      TabIndex        =   0
      Top             =   120
      Width           =   6135
      _ExtentX        =   10821
      _ExtentY        =   6588
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
      TabIndex        =   10
      Top             =   0
      Width           =   1560
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   0
         Left            =   120
         TabIndex        =   1
         Tag             =   "red"
         ToolTipText     =   "Create new empty portfolio"
         Top             =   360
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
         Caption         =   "New"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   2
         Left            =   120
         TabIndex        =   3
         Tag             =   "red"
         ToolTipText     =   "Save portfolio to text file"
         Top             =   1080
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
         Caption         =   "Save"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   2
         Tag             =   "red"
         ToolTipText     =   "Load portfolio from text file"
         Top             =   720
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
         Caption         =   "Open"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   3
         Left            =   1440
         TabIndex        =   4
         Tag             =   "red"
         ToolTipText     =   "Copy portfolio data to the Clipboard"
         Top             =   2160
         Visible         =   0   'False
         Width           =   1395
         _ExtentX        =   2461
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
         Caption         =   "Copy All"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   4
         Left            =   1440
         TabIndex        =   5
         Tag             =   "red"
         ToolTipText     =   "Load portfolio data from the Clipboard"
         Top             =   2520
         Visible         =   0   'False
         Width           =   1395
         _ExtentX        =   2461
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
         Caption         =   "Paste All"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   6
         Left            =   120
         TabIndex        =   8
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
         Index           =   5
         Left            =   120
         TabIndex        =   7
         Tag             =   "red"
         ToolTipText     =   "Start price feed for Stocks"
         Top             =   5400
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
         Index           =   7
         Left            =   120
         TabIndex        =   9
         Tag             =   "red"
         ToolTipText     =   "Split portfolio on the subportfolios"
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
         Caption         =   "Summary"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   8
         Left            =   1440
         TabIndex        =   6
         Tag             =   "red"
         ToolTipText     =   "Create portfolio linked to the Excell cell range"
         Top             =   2880
         Visible         =   0   'False
         Width           =   1395
         _ExtentX        =   2461
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
         Caption         =   "Link  Portfolio"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin FocusFlatControls1.FlatCombo cmbHedge 
         Height          =   315
         Left            =   240
         TabIndex        =   15
         Top             =   2040
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   556
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Text            =   ""
         Style           =   2
      End
      Begin FocusFlatControls1.FlatCombo cmbExpirations 
         Height          =   315
         Left            =   240
         TabIndex        =   16
         Top             =   2880
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   556
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Text            =   ""
         Style           =   2
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Exp Month :"
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
         Index           =   4
         Left            =   120
         TabIndex        =   17
         Top             =   2520
         Width           =   1335
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
         Index           =   3
         Left            =   120
         TabIndex        =   14
         Top             =   4440
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
         Index           =   2
         Left            =   120
         TabIndex        =   13
         Top             =   3600
         Width           =   1335
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   2
         X1              =   120
         X2              =   1440
         Y1              =   4320
         Y2              =   4320
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Hedge type :"
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
         TabIndex        =   12
         Top             =   1680
         Width           =   1335
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Portfolio:"
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
         TabIndex        =   11
         Top             =   120
         Width           =   1335
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   1
         X1              =   120
         X2              =   1440
         Y1              =   3480
         Y2              =   3480
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   0
         X1              =   120
         X2              =   1440
         Y1              =   1560
         Y2              =   1560
      End
   End
End
Attribute VB_Name = "ctlPortfolio"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Event ButtonClick(Index As Integer)

Public Event Copy()
Public Event Paste()

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Private Const Border = 60
Private fEditExpiry As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    BackColor = GCOLOR_TAB_1
    
    Set PortfolioHedge = gPortfolioHedge
    With cmbHedge
        .Clear
        .AddItem "Delta", 0
        .AddItem "Vega", 1
        If PortfolioHedge.VegaMode Then
            .Text = "Vega"
        Else
            .Text = "Delta"
        End If
    End With
    
    If PortfolioHedge.VegaMode Then
        cmbExpirations.Enabled = True
        PortfolioHedge.fPerExpiration = True
    Else
        cmbExpirations.Enabled = False
        PortfolioHedge.fPerExpiration = False
    End If
    
    If PortfolioHedge.dPerExpirationMonth < 1 Then
        PortfolioHedge.dPerExpirationMonth = 1
    End If
    If PortfolioHedge.dPerExpirationMonth > 12 Then
        PortfolioHedge.dPerExpirationMonth = 12
    End If
    Dim i As Long
    With cmbExpirations
        .Clear
        .AddItem "All"
        .ListIndex = 0
'        For i = 1 To 12
'            .AddItem GetMonthNameEng(i)
'        Next
'        .ListIndex = PortfolioHedge.dPerExpirationMonth - 1
    End With
    'PortfolioHedge.dPerExpirationYear
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
Private Sub ctlPortfolioHedge1_Copy()
    On Error Resume Next
    RaiseEvent Copy
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_Paste()
    On Error Resume Next
    RaiseEvent Paste
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the text buffer
'
Public Function CopyToText(Buffer As String, ByVal All As Boolean) As Boolean
    CopyToText = ctlPortfolioHedge1.CopyToText(Buffer, All)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyToClipboard(ByVal All As Boolean) As Boolean
    CopyToClipboard = ctlPortfolioHedge1.CopyToClipboard(All)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CopyFromClipboard() As Boolean
    CopyFromClipboard = ctlPortfolioHedge1.CopyFromClipboard
End Function

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
    cmbHedge.Enabled = Enable
    '!!!
    cmbExpirations.Enabled = Enable And PortfolioHedge.VegaMode
    ctlPortfolioHedge1.Editable = Enable
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    picLeft.Height = ScaleHeight
    With ctlPortfolioHedge1
        .Left = picLeft.Width + Border
        .Top = Border
        .Width = Max(0, ScaleWidth - picLeft.Width - 2 * Border)
        .Height = Max(0, ScaleHeight - 2 * Border)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset)
    ctlPortfolioHedge1.DisplayOutputRS rs, True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_DeleteRow(ByVal RowID As Variant)
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    If Not PortfolioHedge.rsPortfolioData Is Nothing Then
        With PortfolioHedge.rsPortfolioData.Clone
            .Bookmark = RowID
            If Not .EOF Then
                PortfolioHedge.ChangeExpiry !Expiration, Null
                .Delete
            End If
        End With
    End If
    Exit Sub
Herr:
    If Err.Number = &H80040E23 Then
        'Row handle referred to a deleted row or a row marked for deletion.
    Else
        ShowError
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_InsertRow(RowID As Variant)
    On Error GoTo Herr
    PortfolioHedge.NeedSavePortfolio = True
    If Not PortfolioHedge.rsPortfolioData Is Nothing Then
        With PortfolioHedge.rsPortfolioData
            .AddNew
            .Update
            RowID = .Bookmark
        End With
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockSymbolChanged(ByVal RowID As Variant, StockSymbol As String, StockPrice As Double, StockIV As Double)
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Debug.Assert False
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    If Len(StockSymbol) = 0 Then
        StockPrice = 0
        StockIV = 0
    Else
        StockPrice = PortfolioHedge.GetStockPrice(StockSymbol)
        StockIV = PortfolioHedge.GetStockIV(StockSymbol)
    End If
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            !StockSymbol = UCase(StockSymbol)
            !StockPrice = StockPrice
            !IV = StockIV
            If Not PortfolioHedge.CheckStockSymbol(StockSymbol) Then
    '        gCmn.MyMsgBox "This Symbol is not found in the list of supported stocks.", vbCritical
    '            LogSave "Unsupported Symbol '" & StockSymbol & "'"
                !Unsupported = True
            End If
            .Update
            StockSymbol = !StockSymbol
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockOptionExpiryChanged(ByVal RowID As Variant, OptionExpiry As Variant)
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            PortfolioHedge.ChangeExpiry !Expiration, OptionExpiry
            If IsDate(OptionExpiry) Then
                !Expiration = OptionExpiry
            Else
                !Expiration = Null
            End If
            .Update
            OptionExpiry = !Expiration
        End If
    End With
    Exit Sub
Herr:
    gCmn.MyMsgBox "Wrong date format", vbCritical
    OptionExpiry = ""
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockDeltaChanged(ByVal RowID As Variant, StockDelta As Double)
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            !Delta = StockDelta
            .Update
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockPriceChanged(ByVal RowID As Variant, StockPrice As Double)
Dim StockSymbol As String
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            !StockPrice = StockPrice
            .Update
            StockSymbol = !StockSymbol
        End If
    End With
    PortfolioHedge.SetStockPrice StockSymbol, StockPrice, DATA_MANUAL
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockVegaChanged(ByVal RowID As Variant, StockVega As Double)
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            !Vega = StockVega
            .Update
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlPortfolioHedge1_StockIVChanged(ByVal RowID As Variant, StockIV As Double)
Dim StockSymbol As String
    On Error GoTo Herr
    If IsEmpty(RowID) Then
        Exit Sub
    End If
    PortfolioHedge.NeedSavePortfolio = True
    With PortfolioHedge.rsPortfolioData
        .Bookmark = RowID
        If Not .EOF Then
            !IV = StockIV
            .Update
            StockSymbol = !StockSymbol
        End If
    End With
    PortfolioHedge.SetStockIVData StockSymbol, StockIV, DATA_MANUAL
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    PortfolioHedge.NeedSavePortfolio = True
    ctlPortfolioHedge1.SetStockPriceData StockSymbol, StockPrice, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetPortfolioField(ByVal StockSymbol As String, ByVal FieldName As String, ByVal NewValue As Variant)
    PortfolioHedge.NeedSavePortfolio = True
    If Not PortfolioHedge.rsPortfolioData Is Nothing Then
        With PortfolioHedge.rsPortfolioData.Clone
            .Filter = "StockSymbol='" & StockSymbol & "'"
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If Not .EOF Then
                        .Fields.Item(FieldName) = NewValue
                        .Update
                        .MoveNext
                    End If
                Loop
            End If
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockHVData3(ByVal StockSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    PortfolioHedge.NeedSavePortfolio = True
    PortfolioHedge_GotStockIVData StockSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    PortfolioHedge.NeedSavePortfolio = True
    SetPortfolioField StockSymbol, "IV", IV
    ctlPortfolioHedge1.SetStockIVData StockSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGrid() As VSFlexGrid
    On Error Resume Next
    Set GetGrid = ctlPortfolioHedge1.GetGrid
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_AdviseVegaMode(ByVal NewVegaMode As Boolean)
    If NewVegaMode Then
        cmbHedge.Text = "Vega"
    Else
        cmbHedge.Text = "Delta"
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbHedge_Click()
    On Error Resume Next
    If cmbHedge.Text = "Vega" Then
        PortfolioHedge.VegaMode = True
    Else
        PortfolioHedge.VegaMode = False
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_VegaModeChanged(ByVal NewVegaMode As Boolean)
    On Error Resume Next
    If NewVegaMode Then
        cmbExpirations.Enabled = True
    Else
        cmbExpirations.Enabled = False
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_NewExpiry(rs As ADODB.Recordset)
Dim m As Long
Dim y As Long
Dim i As Long
    On Error Resume Next
    fEditExpiry = False
    With cmbExpirations
        .Clear
        .AddItem "All", 0
        .ItemData(0) = 0
        i = 0
        If Not rs Is Nothing Then
            With rs
                If .RecordCount <> 0 Then
                    .Sort = "ExpYear,ExpMonth"
                    .MoveFirst
                    Do While Not .EOF
                        m = !ExpMonth
                        y = !ExpYear
                        With cmbExpirations
                            .AddItem GetMonthNameEng(m) & Right$(Format$(y, "0000"), 2)
                            .ItemData(.ListCount - 1) = y * 16 + m
                            If y = PortfolioHedge.dPerExpirationYear Then
                                If m = PortfolioHedge.dPerExpirationMonth Then
                                    i = .ListCount - 1
                                End If
                            End If
                        End With
                        .MoveNext
                    Loop
                End If
            End With
        End If
        .ListIndex = i
    End With
    fEditExpiry = True
    PortfolioHedge.NeedUpdateExpiry = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbExpirations_Click()
Dim m As Long
Dim y As Long
Dim i As Long
    On Error Resume Next
    If fEditExpiry Then
        SaveExpiry
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SaveExpiry()
Dim m As Long
Dim y As Long
Dim i As Long
    On Error Resume Next
    m = 0
    y = 0
    With cmbExpirations
        If .ListIndex >= 0 Then
            i = .ItemData(.ListIndex)
            If i > 0 Then
                m = i And 15
                y = (i And &HFFF0) \ 16
            End If
        End If
    End With
    PortfolioHedge.dPerExpirationMonth = m
    PortfolioHedge.dPerExpirationYear = y
End Sub

