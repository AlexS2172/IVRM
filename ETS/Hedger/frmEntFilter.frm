VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEntFilter 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Filter's Properties"
   ClientHeight    =   3135
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4515
   Icon            =   "frmEntFilter.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3135
   ScaleWidth      =   4515
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      ForeColor       =   &H80000008&
      Height          =   2415
      Left            =   120
      ScaleHeight     =   2385
      ScaleWidth      =   4215
      TabIndex        =   4
      Top             =   120
      Width           =   4245
      Begin VB.TextBox txtFilterName 
         Appearance      =   0  'Flat
         Height          =   285
         Left            =   1560
         TabIndex        =   0
         Tag             =   "Text"
         Top             =   360
         Width           =   2415
      End
      Begin MSForms.ComboBox cmbGroups 
         Height          =   300
         Left            =   1560
         TabIndex        =   11
         Top             =   1800
         Width           =   2415
         VariousPropertyBits=   679499803
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "4260;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         Value           =   "cmbGroups"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.ComboBox cmbStrategies 
         Height          =   300
         Left            =   1560
         TabIndex        =   10
         Top             =   1320
         Width           =   2415
         VariousPropertyBits=   679499803
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "4260;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         Value           =   "cmbStrategies"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.ComboBox cmbTraders 
         Height          =   300
         Left            =   1560
         TabIndex        =   9
         Top             =   840
         Width           =   2415
         VariousPropertyBits=   679499803
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "4260;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         Value           =   "cmbTraders"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lbl0 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Filter Name"
         Height          =   195
         Left            =   240
         TabIndex        =   8
         Top             =   360
         Width           =   1035
         WordWrap        =   -1  'True
      End
      Begin VB.Label lbl2 
         BackStyle       =   0  'Transparent
         Caption         =   "Strategy"
         Height          =   195
         Left            =   240
         TabIndex        =   7
         Top             =   1320
         Width           =   1020
         WordWrap        =   -1  'True
      End
      Begin VB.Label lbl3 
         BackStyle       =   0  'Transparent
         Caption         =   "Stock Group"
         Height          =   195
         Left            =   240
         TabIndex        =   6
         Top             =   1800
         Width           =   1020
         WordWrap        =   -1  'True
      End
      Begin VB.Label lbl1 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Trader"
         Height          =   195
         Left            =   240
         TabIndex        =   5
         Top             =   840
         Width           =   1035
         WordWrap        =   -1  'True
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   3000
      TabIndex        =   3
      Top             =   2640
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "  &Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnApply 
      Height          =   300
      Left            =   1560
      TabIndex        =   2
      Top             =   2640
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "  &Apply"
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   120
      TabIndex        =   1
      Top             =   2640
      Width           =   1335
      _ExtentX        =   2355
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
      Caption         =   "  &OK"
   End
End
Attribute VB_Name = "frmEntFilter"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Edit Report Filter's properties
'
Option Explicit
Option Compare Text

Private miID As Long
Private msCaption As String

Private mbOK As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ExecuteEdit(ByVal ID As Long, pCaption As String) As Boolean
    On Error GoTo Herr
    mbOK = False
    miID = ID
    msCaption = pCaption
    Load Me
    LoadData
    Show vbModal
    Unload Me
    ExecuteEdit = mbOK
    pCaption = msCaption
    Exit Function
Herr:
    ShowError "Unable to execute Filter Properties dialogue"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ExecuteNew(ID As Long, pCaption As String) As Boolean
    On Error GoTo Herr
    mbOK = False
    miID = 0
    msCaption = ""
    Load Me
    LoadData
    Show vbModal
    Unload Me
    ExecuteNew = mbOK
    ID = miID
    pCaption = msCaption
    Exit Function
Herr:
    ShowError "Unable to execute Filter Properties dialogue"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    btnOk.SetFocus
    If btnApply.Enabled Then
        mbOK = SaveData
        If mbOK Then
            btnApply.Enabled = False
            Hide
        End If
    Else
        Hide
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnApply_Click()
    On Error Resume Next
    If btnApply.Enabled Then
        mbOK = SaveData
        If mbOK Then
            btnApply.Enabled = False
            LoadData
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    mbOK = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ClearData() As Boolean
    On Error Resume Next
    Me.txtFilterName = ""
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load data from database
'
Private Function LoadData() As Boolean
Dim rsFilters As ADODB.Recordset
Dim rsTraders As ADODB.Recordset
Dim rsStrategies As ADODB.Recordset
Dim rsGroups As ADODB.Recordset

Dim FilterName As String
Dim TraderID As Long
Dim StrategyID As Long
Dim GroupID As Long

Dim ID As Long
Dim Symbol As String
    
    On Error GoTo Herr
    LoadData = False
    
    FilterName = ""
    TraderID = 0
    StrategyID = 0
    GroupID = 0
    
    With cmbTraders
        .Clear
        .ColumnCount = 1
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "<None>"
        .Column(1, .ListCount - 1) = 0
        .ListIndex = 0
    End With
    
    With cmbStrategies
        .Clear
        .ColumnCount = 1
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "<None>"
        .Column(1, .ListCount - 1) = 0
        .ListIndex = 0
    End With
    
    With cmbGroups
        .Clear
        .ColumnCount = 1
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "<None>"
        .Column(1, .ListCount - 1) = 0
        .ListIndex = 0
    End With
    
    If miID <> 0 Then
        Dim FieldName As String
        Dim FieldValue As Long
        Set rsFilters = gDBW.usp_Filter_Get(miID)
        If Not rsFilters Is Nothing Then
            With rsFilters
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        FilterName = DBRead(!vcFilterName, "")
                        FieldName = DBRead(!vcFieldName, "")
                        FieldValue = Val(DBRead(!vcFieldValue, "0"))
                        Select Case FieldName
                        Case "traderID"
                            TraderID = FieldValue
                        Case "strategyID"
                            StrategyID = FieldValue
                        Case "groupID"
                            GroupID = FieldValue
                        End Select
                        .MoveNext
                    Loop
                End If
            End With
        End If
        Me.txtFilterName = FilterName
    End If
    
    Set rsTraders = gDBW.usp_Trader_Get()
    Set rsStrategies = gDBW.usp_Strategy_Get()
    Set rsGroups = gDBW.usp_ContractGroup_Get()
    
    If Not rsTraders Is Nothing Then
        With rsTraders
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    ID = DBRead(!iTraderID, 0)
                    Symbol = DBRead(!vcAcronym, "")
                    If ID <> 0 And Len(Symbol) > 0 Then
                        With cmbTraders
                            .AddItem Symbol
                            .Column(1, .ListCount - 1) = ID
                            If ID = TraderID Then
                                .ListIndex = .ListCount - 1
                            End If
                        End With
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
        
    If Not rsStrategies Is Nothing Then
        With rsStrategies
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    ID = DBRead(!iStrategyID, 0)
                    Symbol = DBRead(!vcStrategyName, "")
                    If ID <> 0 And Len(Symbol) > 0 Then
                        With cmbStrategies
                            .AddItem Symbol
                            .Column(1, .ListCount - 1) = ID
                            If ID = StrategyID Then
                                .ListIndex = .ListCount - 1
                            End If
                        End With
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    If Not rsGroups Is Nothing Then
        With rsGroups
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    ID = DBRead(!iGroupID, 0)
                    Symbol = DBRead(!vcGroupName, "")
                    If ID <> 0 And Len(Symbol) > 0 Then
                        With cmbGroups
                            .AddItem Symbol
                            .Column(1, .ListCount - 1) = ID
                            If ID = GroupID Then
                                .ListIndex = .ListCount - 1
                            End If
                        End With
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    ' disabling Apply button
    btnApply.Enabled = False
    LoadData = True
    Exit Function
Herr:
    ShowError "Unable to load Filter data"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Save data into database
'
Private Function SaveData() As Boolean
Dim FilterName As String
Dim TraderID As Long
Dim StrategyID As Long
Dim GroupID As Long
    
    On Error GoTo Herr
    SaveData = False
    TraderID = 0
    StrategyID = 0
    GroupID = 0
    
    FilterName = Trim$(Me.txtFilterName)
    If Len(FilterName) = 0 Then
        Err.Raise vbObjectError, "Filter Dialog", "Empty Filter name."
    End If
    
    With cmbTraders
        If .ListIndex > 0 Then
            TraderID = .Value
        End If
    End With
        
    With cmbStrategies
        If .ListIndex > 0 Then
            StrategyID = .Value
        End If
    End With
    
    With cmbGroups
        If .ListIndex > 0 Then
            GroupID = .Value
        End If
    End With
    
    gDBW.usp_Filter_Save _
        miID, _
        FilterName, _
        TraderID, _
        StrategyID, _
        GroupID
        
    msCaption = FilterName
    
    SaveData = True
    Exit Function
Herr:
    ShowError "Unable to save Filter data"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbTraders_Click()
    On Error Resume Next
    btnApply.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbStrategies_Click()
    On Error Resume Next
    btnApply.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbGroups_Click()
    On Error Resume Next
    btnApply.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub txtFilterName_Change()
    On Error Resume Next
    btnApply.Enabled = True
End Sub

