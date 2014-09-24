VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.Form frmStockShift 
   Caption         =   "Asset specific shift"
   ClientHeight    =   5295
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4230
   Icon            =   "frmStockShift.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   5295
   ScaleMode       =   0  'User
   ScaleWidth      =   4230
   StartUpPosition =   3  'Windows Default
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   360
      Top             =   4800
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Height          =   315
      Left            =   2760
      TabIndex        =   3
      Top             =   4920
      Width           =   1335
   End
   Begin VB.CommandButton btnLoad 
      Caption         =   "Load"
      Height          =   315
      Left            =   1320
      TabIndex        =   2
      Top             =   4920
      Width           =   1335
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000004&
      Height          =   315
      Left            =   120
      Locked          =   -1  'True
      TabIndex        =   0
      Top             =   360
      Width           =   3975
   End
   Begin VSFlex7Ctl.VSFlexGrid fgShifts 
      Height          =   3975
      Left            =   120
      TabIndex        =   1
      Top             =   840
      Width           =   3975
      _cx             =   7011
      _cy             =   7011
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   1
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   10
      Cols            =   4
      FixedRows       =   1
      FixedCols       =   1
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   ""
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
   End
   Begin VB.Label lblFile 
      Caption         =   "Current File:"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   975
   End
End
Attribute VB_Name = "frmStockShift"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private file_name As String
Private current_key_values() As KeyValue
Private scenario_id As Long
Private current_scenario As MarketSimulationScenario

Public Sub Execute(ByRef scenario As MarketSimulationScenario, ByVal scenarioid As Long)
    On Error GoTo error_handler
    '--------------------------'
        If (Not scenario Is Nothing) Then
                                   
            Load Me
                                   
            scenario_id = scenarioid
            Set current_scenario = scenario
            
            current_scenario.GetSpecificValues scenario_id, current_key_values
            
            FormatGrid
            ShowValuesOnGrid
            
            Me.Show vbModal
            
        End If
    Exit Sub
'------------'
error_handler:
    MsgBox "Error while executing Specific Shift form."
End Sub

Private Sub FormatGrid()
    On Error GoTo error_handler
    '--------------------------'

        fgShifts.Cols = 2
        fgShifts.rows = 1
        
        fgShifts.TextMatrix(0, 0) = "Asset Symbol"
        fgShifts.TextMatrix(0, 1) = "Ratio"
                    
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while formating grid. Description: " & _
                Err.Description
End Sub

Private Sub ShowValuesOnGrid()
    On Error GoTo error_handler
    '--------------------------'
        Dim ID As Long
        Dim rows As Long
        
        rows = IIf(UBound(current_key_values) > 0, UBound(current_key_values) + 1, 0)
        fgShifts.rows = 1
        For ID = 1 To rows
            fgShifts.AddItem current_key_values(ID - 1).Key
            fgShifts.TextMatrix(ID, 0) = current_key_values(ID - 1).Key
            fgShifts.TextMatrix(ID, 1) = current_key_values(ID - 1).Value
        Next ID
        
        fgShifts.AutoSize (0)
        fgShifts.AutoSize (1)
        
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while displaying grid. Description: " & _
                 Err.Description
End Sub

Private Sub CloseWindow()
    On Error GoTo error_handler
    '-------------------------'
        SendMessage Me.hWnd, WM_CLOSE, 0, 0&
    Exit Sub
'------------'
error_handler:
    Debug.Print "Error while closing window. Description: " & _
                Err.Description
End Sub

Public Sub LoadData(ByVal sFileName As String)
    On Error GoTo error_handler
    '-------------------------'
    
        file_name = sFileName
        
        If file_name = STR_NA Or file_name = "" Then Exit Sub
                
        Dim file_length As Long
        Dim string_buffer As String
        
        file_length = FileLen(file_name)
        If file_length > 0 Then
            string_buffer = Space(file_length)
            
            Dim hFile As Long
            hFile = FreeFile
            Open file_name For Binary Access Read As #hFile
            
            Get #hFile, , string_buffer
            Close #hFile
            
        Else
            LogEvent EVENT_ERROR, "Empty stock shift file " & file_name
            Exit Sub
        End If
        
        Dim vtFullInfo As Variant
        vtFullInfo = Split(string_buffer, vbNewLine)
        
        Dim lines_count As Long
        lines_count = UBound(vtFullInfo)
        If lines_count <= 0 Then
            LogEvent EVENT_ERROR, "Can't load data from stock shift file " & file_name
            Exit Sub
        End If
        
        Dim vtShiftInfo As Variant
        
        Erase current_key_values
        ReDim current_key_values(lines_count)
        
        Dim i As Long
        For i = 0 To lines_count
        
            vtShiftInfo = Split(vtFullInfo(i), vbTab)
            
            If UBound(vtShiftInfo) = 1 Then
                current_key_values(i).Key = CStr(vtShiftInfo(0))
                current_key_values(i).Value = CDbl(vtShiftInfo(1))
            ElseIf vtFullInfo(i) <> "" Then
                    LogEvent EVENT_WARNING, "Can't parse line " & CStr(i + 1) & " from stock shift file " & file_name
            End If
            
        Next
        
        g_Params.StockShiftFile = file_name
    
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while loading data from file. Description: " & _
                Err.Description
End Sub

Private Sub btnApply_Click()
    On Error GoTo error_handler
    '--------------------------'
    
    CloseWindow
    current_scenario.SetSpecificValues scenario_id, current_key_values
    
    Exit Sub
'-------------'
error_handler:
    Debug.Print "Error while apply buton pressed. Description: " & _
                Err.Description
End Sub

Private Sub btnLoad_Click()
    On Error GoTo error_handler
    '--------------------------'
    
    dlgCommon.Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                                     Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
    
    dlgCommon.CancelError = True
    dlgCommon.DefaultExt = ".txt"
    dlgCommon.DialogTitle = "Load shifts"
    dlgCommon.FileName = ""
    dlgCommon.Filter = "Text files (*.txt)|*.txt|All Files|*.*"
    dlgCommon.FilterIndex = 1
    
    dlgCommon.ShowOpen
    
    If Err.Number = 0 Then
        LoadData dlgCommon.FileName
        ShowValuesOnGrid
    End If
    
    Exit Sub
'-----------'
error_handler:
    Debug.Print "Error while trying to load data from file. Description: " & _
                Err.Description
End Sub
