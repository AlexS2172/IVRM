VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmSymbolsLoadFrIV 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Load Symbols List from IV"
   ClientHeight    =   3720
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8220
   Icon            =   "frmSymbolsLoadFrIV.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3720
   ScaleWidth      =   8220
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CheckBox chkSetActive 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Set All New Symbols as Active"
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   6360
      TabIndex        =   5
      Top             =   1560
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin VB.CheckBox chkIsOptional 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Import Only Optional Underlyings"
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   6360
      TabIndex        =   4
      Top             =   840
      Value           =   1  'Checked
      Width           =   1695
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   6360
      TabIndex        =   2
      Top             =   360
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Cancel"
   End
   Begin VB.TextBox txtLog 
      Appearance      =   0  'Flat
      Height          =   2655
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   3
      Tag             =   "xxx"
      Top             =   840
      Width           =   6015
   End
   Begin MSComctlLib.ProgressBar pbStocks 
      Height          =   300
      Left            =   120
      TabIndex        =   1
      Top             =   360
      Width           =   6015
      _ExtentX        =   10610
      _ExtentY        =   529
      _Version        =   393216
      BorderStyle     =   1
      Appearance      =   0
   End
   Begin VB.Label lblStocks 
      BackStyle       =   0  'Transparent
      Caption         =   "Prompt"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   3375
   End
End
Attribute VB_Name = "frmSymbolsLoadFrIV"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents DHIVDataLoader As DHIVFeeder5.IVDataLoader
Attribute DHIVDataLoader.VB_VarHelpID = -1
Private WithEvents DHHttpFeeder As HttpFeeder
Attribute DHHttpFeeder.VB_VarHelpID = -1

Private m_bCancel As Boolean

Private miSymbolsCount As Long

Private LastErrorNumber As Long
Private LastErrorDescription  As String
Private LastErrorSource As String
Private Const RT_DELAY = 0

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As Boolean
    
    On Error GoTo Herr
    Load Me
    
    btnCancel.Caption = "Start"
    
    Me.Show vbModal
    Unload Me
    Exit Function
Herr:
    ShowError
End Function


Private Sub DHIVDataLoader_LogMessage(ByVal message As String)
    DebugSave "DHIVDataLoader_LogMessage:"
    DebugSave message
End Sub

Private Sub DHIVDataLoader_Status(ByVal message As String)
    AddToLog message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error GoTo Herr
    
    gCmn.CustomizeForm Me
    
    lblStocks.Caption = "Underlying progress"
    
    miSymbolsCount = 0
    m_bCancel = False
    
    Exit Sub
Herr:
    gCmn.ErrorMsgBox "Error loading form"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = btnCancel.Caption = "Cancel"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    
    If Not DHIVDataLoader Is Nothing Then
        DHIVDataLoader.CancelTask
        Set DHIVDataLoader = Nothing
    End If
    
    If Not DHHttpFeeder Is Nothing Then
        Set DHHttpFeeder = Nothing
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    If btnCancel.Caption = "Close" Then
        Unload Me
    ElseIf btnCancel.Caption = "Cancel" Then
        m_bCancel = True
        
        If Not DHIVDataLoader Is Nothing Then
            DHIVDataLoader.CancelTask
        End If
        
    Else
        chkIsOptional.Enabled = False
        chkSetActive.Enabled = False
        btnCancel.Caption = "Cancel"
        LoadSymbols
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadSymbols()
    On Error GoTo Herr
    
    If Not CheckInternetConnection Then
        EditInternetConnections
    End If

    If DHIVDataLoader Is Nothing Then
        On Error Resume Next
        Set DHIVDataLoader = New DHIVFeeder5.IVDataLoader
        On Error GoTo Herr
        If DHIVDataLoader Is Nothing Then
            LastErrorDescription = "IV Loader component can not be started." & vbCrLf & "Repair application installation."
            Err.Raise vbObjectError, "IV Loader", LastErrorDescription
            Exit Sub
        End If
        On Error Resume Next
        Set DHHttpFeeder = DHIVDataLoader.GetDHIVFeeder.GetHttpFeeder
    End If
                
    On Error GoTo Herr
    
    Me.Refresh
    DoEvents
    
' make all work
    If Not DHIVDataLoader.LoadSymbolList(chkIsOptional.Value) Then
        CancelExecute
        Exit Sub
    End If
    
Done:
    ExecuteCompleted
    Exit Sub
Herr:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    
    Dim Msg As String
    btnCancel.Caption = "Close"
    Msg = LastErrorDescription
    
    If m_bCancel Then
        ShowMessage "Task was canceled by user"
    Else
        ShowError
    End If
    
    AddToLog Msg
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddToLog(ByVal s As String)
    On Error Resume Next
    With txtLog
        .Text = Right(.Text, 64000) & s & vbCrLf
        .SelStart = Len(.Text)
    End With
End Sub

Private Sub DHIVDataLoader_SaveSymbol(ByVal Symbol As String, ByVal SymbolType As String, ByVal Company As String, ByVal PrimExchange As String)
    On Error GoTo Skip
    Dim vId As Long, btIsActive As Byte
    
    
    btIsActive = IIf(chkSetActive.Value <> 0, 1, 0)
    
    If UCase$(SymbolType) = "S" Or UCase$(SymbolType) = "F" Then
        vId = gDBW.usp_Stock_Save(, , Symbol, Company, , , , , , , , 0, 1, , , btIsActive, , , 0)
    ElseIf UCase$(SymbolType) = "I" Then
        vId = gDBW.usp_Index_Save(, Symbol, Company, , , , , , , btIsActive, , , 0, , 1)
    Else
        AddToLog "Can't save Symbol '" & Symbol & "' "
        AddToLog "Unknown symbol type: '" & SymbolType & "' "
        Exit Sub
    End If
    
    If vId <> -1 Then miSymbolsCount = miSymbolsCount + 1
    
    Exit Sub

Skip:
    AddToLog Err.Description & " " & Err.Source
    
    AddToLog "Error occured while save Symbol '" & Symbol & "' "
    Exit Sub
Herr:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source


End Sub

Private Sub DHIVDataLoader_Progress(ByVal ProgressPercent As Long)
    On Error Resume Next
    pbStocks.Value = ProgressPercent
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVDataLoader_ProgressStart()
    On Error Resume Next
    pbStocks.Min = 0
    pbStocks.Max = 100
    pbStocks.Value = 0
    miSymbolsCount = 0
End Sub

#If DEBUG_REPLY > 1 Then

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_GotReply(ByVal message As String)
    DebugSave "DHHttpFeeder_GotReply:"
    DebugSave message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_GotRequest(ByVal message As String)
    DebugSave "DHHttpFeeder_GotRequest:"
    DebugSave message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_LogMessage(ByVal message As String)
    DebugSave "DHHttpFeeder_LogMessage:"
    DebugSave message
End Sub

#End If

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DebugSave(ByVal message As String)
    Debug.Print message
End Sub


Public Function Idle()
    DoEvents
    If LastErrorNumber <> 0 Then
        Err.Raise LastErrorNumber, LastErrorSource, LastErrorDescription
    End If
    If m_bCancel Then
        btnCancel.Caption = "Close"
        Err.Raise vbObjectError, "Task manager", "Task was canceled by user"
    End If
    Sleep RT_DELAY
End Function


Public Sub ExecuteCompleted()
    On Error Resume Next
    pbStocks.Value = pbStocks.Max
    
    AddToLog "Executing complete"
    AddToLog miSymbolsCount & " symbols uploaded"
    
    btnCancel.Caption = "Close"
    
    btnCancel.Enabled = True
    chkIsOptional.Enabled = True
    chkSetActive.Enabled = True
    Me.MousePointer = vbDefault
    Me.Refresh
End Sub


Public Sub CancelExecute()
    On Error Resume Next
    
    pbStocks.Value = pbStocks.Min
    AddToLog "Executing canceled"
    AddToLog miSymbolsCount & " stocks uploaded"
    
    btnCancel.Caption = "Close"
    
    btnCancel.Enabled = True
    chkIsOptional.Enabled = True
    chkSetActive.Enabled = True
    Me.MousePointer = vbDefault
    Me.Refresh
End Sub

