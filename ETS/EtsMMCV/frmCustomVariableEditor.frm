VERSION 5.00
Begin VB.Form frmCustomVariableEditor 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Custom Variable Editor"
   ClientHeight    =   6735
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9270
   Icon            =   "frmCustomVariableEditor.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6735
   ScaleWidth      =   9270
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnSysFunc 
      Caption         =   "System Functions >>"
      Height          =   315
      Left            =   7320
      TabIndex        =   13
      Top             =   5340
      Width           =   1815
   End
   Begin VB.CommandButton btnSysVars 
      Caption         =   "System Variables >>"
      Height          =   315
      Left            =   7320
      TabIndex        =   14
      Top             =   5820
      Width           =   1815
   End
   Begin VB.CommandButton btnCustomVars 
      Caption         =   "Custom Variables >>"
      Height          =   315
      Left            =   7320
      TabIndex        =   15
      Top             =   6300
      Width           =   1815
   End
   Begin VB.TextBox txtCode 
      BeginProperty Font 
         Name            =   "Lucida Console"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   3375
      Left            =   120
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   6
      Text            =   "frmCustomVariableEditor.frx":058A
      Top             =   3240
      Width           =   7095
   End
   Begin VB.ComboBox cbVarsLevel 
      Height          =   315
      Left            =   120
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   360
      Width           =   3495
   End
   Begin VB.TextBox txtDesc 
      Height          =   1935
      Left            =   3720
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   5
      Top             =   960
      Width           =   3495
   End
   Begin VB.TextBox txtVariable 
      Height          =   195
      Left            =   2520
      TabIndex        =   1
      Top             =   720
      Visible         =   0   'False
      Width           =   150
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Enabled         =   0   'False
      Height          =   315
      Left            =   7800
      TabIndex        =   9
      Top             =   1320
      Width           =   1335
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete"
      Enabled         =   0   'False
      Height          =   315
      Left            =   7800
      TabIndex        =   12
      Top             =   2580
      Width           =   1335
   End
   Begin VB.TextBox txtCaption 
      Height          =   315
      Left            =   120
      TabIndex        =   3
      Top             =   1560
      Width           =   3495
   End
   Begin VB.CommandButton btnNew 
      Caption         =   "New"
      Height          =   315
      Left            =   7800
      TabIndex        =   10
      Top             =   2100
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   315
      Left            =   7800
      TabIndex        =   8
      Top             =   840
      Width           =   1335
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   7800
      TabIndex        =   7
      Top             =   360
      Width           =   1335
   End
   Begin VB.ComboBox cbVariable 
      Height          =   315
      ItemData        =   "frmCustomVariableEditor.frx":0590
      Left            =   120
      List            =   "frmCustomVariableEditor.frx":0592
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   960
      Width           =   3495
   End
   Begin VB.ListBox lbAddLevels 
      Height          =   735
      Left            =   120
      Style           =   1  'Checkbox
      TabIndex        =   4
      Top             =   2160
      Width           =   3495
   End
   Begin VB.Label lblDesc 
      Alignment       =   2  'Center
      Caption         =   "Description"
      Height          =   255
      Left            =   3720
      TabIndex        =   20
      Top             =   720
      Width           =   3495
   End
   Begin VB.Label lblVarLevel 
      Caption         =   "Variables Level:"
      Height          =   255
      Left            =   120
      TabIndex        =   19
      Top             =   120
      Width           =   3495
   End
   Begin VB.Label Label6 
      Caption         =   "Caption:"
      Height          =   255
      Left            =   120
      TabIndex        =   18
      Top             =   1320
      Width           =   1095
   End
   Begin VB.Label Label7 
      Caption         =   "Variable name:"
      Height          =   255
      Left            =   120
      TabIndex        =   17
      Top             =   720
      Width           =   1095
   End
   Begin VB.Label Label3 
      Alignment       =   2  'Center
      Caption         =   "Aggregation Levels"
      Height          =   255
      Left            =   120
      TabIndex        =   16
      Top             =   1920
      Width           =   3495
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "Custom Variable Definition Code"
      Height          =   255
      Left            =   120
      TabIndex        =   11
      Top             =   3000
      Width           =   7095
   End
End
Attribute VB_Name = "frmCustomVariableEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnVariablesChanged(ByVal nNewID As Long)

Private m_bIsOpened As Boolean

Private m_nLevelID As Long
Private m_nWeight As Long
Private m_nContextID As Long


Private m_Contexts As Collection
Private m_Levels As Collection
Private m_SysVars As Collection
Private m_SysFuncs As Collection
Private m_Variables As Collection
Private m_GreekVars As Collection

Private m_bCaptionChange As Boolean
Private m_bVariableTransfer As Boolean

Private m_frmOwner As Form

Private m_bIsSysFuncShow As Boolean
Private m_bIsSysVarsShow As Boolean
Private m_bIsCstVarsShow As Boolean

Private m_frmSysFunc As frmCustomVariableList
Private m_frmSysVars As frmCustomVariableList
Private m_frmCstVars As frmCustomVariableList

Private m_bFixedLevel As Boolean
Private m_bAllAggregations As Boolean

Private m_bIsFromRisks As Boolean

Private m_bIsUserEdit As Boolean


Public Sub EditVariable(ByVal nContextID As Long, ByVal nLevelID As Long, ByVal nID As Long, frmOwner As Form, _
                        Optional ByVal bFixedLevel As Boolean = False, _
                        Optional ByVal bAllAggregations As Boolean = False, _
                        Optional sVarCaption As String = STR_NA)
    On Error Resume Next
    
    If m_bIsOpened Then Exit Sub
    m_bIsOpened = True
    
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    If nContextID = g_cvContextInfo.GetRisksContextID Then
        m_bIsFromRisks = True
        m_nLevelID = nLevelID
        m_nContextID = g_cvContextInfo.GetRisksContextID
    Else
        m_bIsFromRisks = False
        m_nLevelID = g_cvContextInfo.GetRisksLevelID
        m_nContextID = g_cvContextInfo.GetRisksContextID
    End If
    
    m_bIsUserEdit = True
    
    m_bIsSysFuncShow = False
    m_bIsSysVarsShow = False
    m_bIsCstVarsShow = False
    
    Set m_frmSysFunc = New frmCustomVariableList
    Set m_frmSysVars = New frmCustomVariableList
    Set m_frmCstVars = New frmCustomVariableList
    
    m_bFixedLevel = bFixedLevel
    m_bAllAggregations = bAllAggregations
    
    LoadDBInfo
    ClearControls
    InitControls nID, sVarCaption
    
    Load Me
    Me.Show
End Sub

Private Function LoadDBInfo() As Boolean
    On Error Resume Next
    Dim rs As ADODB.Recordset
    Dim aContext As clsCVContext
    Dim aLevel As clsCVLevel
    Dim aSysVar As clsCVSysVar
    Dim aSysFunc As clsCVSysFunc
    Dim aVariable As clsCVVariable
    
    Set m_Contexts = Nothing
    Set m_Levels = Nothing
    Set m_SysVars = Nothing
    Set m_SysFuncs = Nothing
    Set m_Variables = Nothing
    
    Set m_Contexts = New Collection
    Set m_Levels = New Collection
    Set m_SysVars = New Collection
    Set m_SysFuncs = New Collection
    Set m_Variables = New Collection
    
    LoadDBInfo = False
    
    Set rs = gDBW.usp_cvContext_Get(m_nContextID, 6)
    
    'Context
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aContext = New clsCVContext
        
        aContext.ID = ReadLng(rs!ID)
        aContext.Name = ReadStr(rs!Name)
        
        m_Contexts.Add aContext, CStr(aContext.ID)
        
        Set aContext = Nothing
        rs.MoveNext
    Wend
    
    'Aggregation levels
    Set rs = rs.NextRecordset
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aLevel = New clsCVLevel
        
        aLevel.ID = ReadLng(rs!ID)
        aLevel.Name = ReadStr(rs!Name)
        aLevel.ContextID = ReadLng(rs!ContextID)
        aLevel.Weight = ReadLng(rs!Weight)
        
        m_Levels.Add aLevel, CStr(aLevel.ID)
        
        If aLevel.ID = m_nLevelID Then m_nWeight = aLevel.Weight
        
        Set aLevel = Nothing
        rs.MoveNext
    Wend
    
    'System variables
    Set rs = rs.NextRecordset
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aSysVar = New clsCVSysVar
        
        aSysVar.ID = ReadLng(rs!ID)
        aSysVar.ContextID = ReadLng(rs!ContextID)
        aSysVar.LevelID = ReadLng(rs!LevelID)
        aSysVar.Name = ReadStr(rs!Name)
        aSysVar.TypeCV = ReadLng(rs!Type)
        aSysVar.IsGlobal = rs!Global
        aSysVar.Description = ReadStr(rs!Description)
        
        m_SysVars.Add aSysVar, aSysVar.Name
        
        Set aSysVar = Nothing
        rs.MoveNext
    Wend
    
    'System functions
    Set rs = rs.NextRecordset
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aSysFunc = New clsCVSysFunc
        
        aSysFunc.ID = ReadLng(rs!ID)
        aSysFunc.ContextID = ReadLng(rs!ContextID)
        aSysFunc.LevelID = ReadLng(rs!LevelID)
        aSysFunc.Name = ReadStr(rs!Name)
        aSysFunc.TypeCV = ReadLng(rs!Type)
        aSysFunc.IsGlobal = rs!Global
        aSysFunc.Description = ReadStr(rs!Description)
        
        m_SysFuncs.Add aSysFunc, aSysFunc.Name
        
        Set aSysFunc = Nothing
        rs.MoveNext
    Wend
    
    'Custom Variables
    Set rs = rs.NextRecordset
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aVariable = New clsCVVariable
        
        aVariable.ID = ReadLng(rs!ID)
        aVariable.Name = ReadStr(rs!Name)
        aVariable.ContextID = ReadLng(rs!ContextID)
        aVariable.LevelID = ReadLng(rs!LevelID)
        aVariable.Caption = ReadStr(rs!Caption)
        aVariable.Format = ReadStr(rs!Format)
        aVariable.Source = ReadStr(rs!Source)
        aVariable.AggLevels = ReadStr(rs!AggLevels)
        aVariable.TypeCV = ReadLng(rs!Type)
        aVariable.Weight = ReadLng(rs!Weight)
        aVariable.Description = ReadStr(rs!Description)
        
        m_Variables.Add aVariable, aVariable.Name
        
        Set aVariable = Nothing
        rs.MoveNext
    Wend
    
    
    
    Set m_GreekVars = Nothing
    Set m_GreekVars = New Collection
    
    Set rs = gDBW.usp_cvContext_Get(g_cvContextInfo.GetGreekContextID, 6)
    
    'Aggregation levels
    rs.NextRecordset
    
    'System variables
    rs.NextRecordset
    
    'System functions
    rs.NextRecordset
    
    'Custom Variables
    Set rs = rs.NextRecordset
    If rs Is Nothing Then Exit Function
    While Not rs.EOF
        Set aVariable = New clsCVVariable
        
        aVariable.ID = ReadLng(rs!ID)
        aVariable.Name = ReadStr(rs!Name)
        aVariable.ContextID = ReadLng(rs!ContextID)
        aVariable.LevelID = ReadLng(rs!LevelID)
        aVariable.Caption = ReadStr(rs!Caption)
        aVariable.Format = ReadStr(rs!Format)
        aVariable.Source = ReadStr(rs!Source)
        aVariable.AggLevels = ReadStr(rs!AggLevels)
        aVariable.TypeCV = ReadLng(rs!Type)
        aVariable.Weight = ReadLng(rs!Weight)
        aVariable.Description = ReadStr(rs!Description)
        
        m_GreekVars.Add aVariable, aVariable.Name
        
        Set aVariable = Nothing
        rs.MoveNext
    Wend
    
    LoadDBInfo = True
      
End Function

Private Sub btnApply_Click()
    On Error Resume Next
    Dim nVarID As Long
    Dim nGreekVarID As Long
    Dim nRisksVarID As Long
    Dim bIsNewVar As Boolean
    
    bIsNewVar = txtVariable.Visible
    SaveCustomVariable nRisksVarID, nGreekVarID
    
    nVarID = IIf(m_bIsFromRisks, nRisksVarID, nGreekVarID)
    
    If nVarID <> BAD_LONG_VALUE Then
        RaiseEvent OnVariablesChanged(IIf(bIsNewVar, nVarID, BAD_LONG_VALUE))
    End If
    
    If nRisksVarID <> BAD_LONG_VALUE Then
        ClearControls
        LoadDBInfo
        InitControls nRisksVarID
        
        If m_bIsCstVarsShow Then m_frmCstVars.OnEditor_DataChanged m_Variables, m_nLevelID
    End If
    
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    
    If btnApply.Enabled Or txtVariable.Visible = True Then
        'Cancel creation/editing of variable
        Dim nCurVarID As Long: nCurVarID = BAD_LONG_VALUE
        nCurVarID = cbVariable.ItemData(cbVariable.ListIndex)
        
        ClearControls
        InitControls nCurVarID
    Else
        Unload Me
    End If
    
End Sub

Private Sub btnCustomVars_Click()
    On Error Resume Next
    
    m_bIsCstVarsShow = Not m_bIsCstVarsShow
    
    If m_bIsCstVarsShow Then
        m_frmCstVars.ShowList m_Variables, enCstVarsList, m_nLevelID, Me
        btnCustomVars.Caption = "Custom Variables <<"
    Else
        Unload m_frmCstVars
        btnCustomVars.Caption = "Custom Variables >>"
    End If
    
End Sub

Private Sub btnDelete_Click()
    On Error Resume Next
    Dim nGreekVarID As Long
    Dim nGreekRetVal As Long
    Dim nRisksRetVal As Long
    
    ' Risks variable
    nRisksRetVal = gDBW.usp_cvCV_Del(cbVariable.ItemData(cbVariable.ListIndex))
    
    ' Greeks variable
    nGreekVarID = m_GreekVars(cbVariable.Text).ID
    If nGreekVarID <> 0 Then
        nGreekRetVal = gDBW.usp_cvCV_Del(nGreekVarID)
    Else
        nGreekRetVal = 1
    End If
    
    If nRisksRetVal = 0 Or nGreekRetVal = 0 Then LogEvent EVENT_WARNING, "Can't delete variable " & cbVariable.Text & ". Another custom variables use this variable."
    
    RaiseEvent OnVariablesChanged(BAD_LONG_VALUE)
    
    ClearControls
    LoadDBInfo
    InitControls
    
    If m_bIsCstVarsShow Then m_frmCstVars.OnEditor_DataChanged m_Variables, m_nLevelID
End Sub

Private Sub btnNew_Click()
    On Error Resume Next
    
    cbVariable.Visible = False
    txtVariable.Visible = True
    
    btnDelete.Enabled = False
    btnApply.Enabled = False
    
    ClearControls False
    
    txtVariable.SetFocus
    
    m_bCaptionChange = False
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Dim nVarID As Long
    Dim nGreekVarID As Long
    Dim nRisksVarID As Long
    Dim bIsNewVar As Boolean
    
    bIsNewVar = txtVariable.Visible
    
    If btnApply.Enabled Then
        SaveCustomVariable nRisksVarID, nGreekVarID
    
        nVarID = IIf(m_bIsFromRisks, nRisksVarID, nGreekVarID)
        
        If nVarID <> BAD_LONG_VALUE Then
            RaiseEvent OnVariablesChanged(IIf(bIsNewVar, nVarID, BAD_LONG_VALUE))
        End If
    End If
    
    Unload Me
End Sub

Private Sub btnSysFunc_Click()
    On Error Resume Next
    
    m_bIsSysFuncShow = Not m_bIsSysFuncShow
    
    If m_bIsSysFuncShow Then
        m_frmSysFunc.ShowList m_SysFuncs, enSysFuncList, m_nLevelID, Me
        btnSysFunc.Caption = "System Functions <<"
    Else
        Unload m_frmSysFunc
        btnSysFunc.Caption = "System Functions >>"
    End If
    
End Sub

Private Sub btnSysVars_Click()
    On Error Resume Next
    
    m_bIsSysVarsShow = Not m_bIsSysVarsShow
    
    If m_bIsSysVarsShow Then
        m_frmSysVars.ShowList m_SysVars, enSysVarsList, m_nLevelID, Me
        btnSysVars.Caption = "System Variables <<"
    Else
        Unload m_frmSysVars
        btnSysVars.Caption = "System Variables >>"
    End If
    
End Sub

Private Sub cbVariable_Click()
    On Error Resume Next
    
    If Not m_bIsUserEdit Then Exit Sub
    
    SetVariable cbVariable.ItemData(cbVariable.ListIndex)
End Sub

Private Sub cbVarsLevel_Click()
    On Error Resume Next
    
    If Not m_bIsUserEdit Then Exit Sub
    
    m_nLevelID = cbVarsLevel.ItemData(cbVarsLevel.ListIndex)
    m_nWeight = m_Levels(CStr(m_nLevelID)).Weight
    
    If m_bIsSysFuncShow Then m_frmSysFunc.OnEditor_DataChanged m_SysFuncs, m_nLevelID
    If m_bIsSysVarsShow Then m_frmSysVars.OnEditor_DataChanged m_SysVars, m_nLevelID
    If m_bIsCstVarsShow Then m_frmCstVars.OnEditor_DataChanged m_Variables, m_nLevelID
    
    ClearControls
    InitControls
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    
    m_bIsOpened = False
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    'If m_bIsSysFuncShow Then Unload m_frmSysFunc
    'If m_bIsSysVarsShow Then Unload m_frmSysVars
    'If m_bIsCstVarsShow Then Unload m_frmCstVars
    
    Set m_frmSysFunc = Nothing
    Set m_frmSysVars = Nothing
    Set m_frmCstVars = Nothing
    
    Set m_Contexts = Nothing
    Set m_Levels = Nothing
    Set m_SysVars = Nothing
    Set m_SysFuncs = Nothing
    Set m_Variables = Nothing
    Set m_GreekVars = Nothing
    
    Set m_frmOwner = Nothing
End Sub

Private Sub lbAddLevels_ItemCheck(Item As Integer)
    On Error Resume Next
    
    If Not m_bIsUserEdit Then Exit Sub
    
    If IsVarValid Then
        btnApply.Enabled = True
    Else
        btnApply.Enabled = False
    End If
End Sub

Private Sub InitControls(Optional ByVal nVarID As Long = BAD_LONG_VALUE, _
                         Optional ByVal sVarCaption As String = STR_NA)
    On Error Resume Next
    Dim sValue As Variant
    
    Dim aVariable As clsCVVariable
    Dim aSysFunc As clsCVSysFunc
    Dim aSysVar As clsCVSysVar
    Dim aLevel As clsCVLevel
    
    m_bIsUserEdit = False
    m_bCaptionChange = False
    
    txtVariable.Top = cbVariable.Top
    txtVariable.Left = cbVariable.Left
    txtVariable.Height = cbVariable.Height
    txtVariable.Width = cbVariable.Width
    
    txtVariable.Visible = False
    cbVariable.Visible = True
    
    Me.Caption = "Custom Variable Editor (" & m_Contexts(CStr(m_nContextID)).Name & "/" & m_Levels(CStr(m_nLevelID)).Name & ")"
    
    For Each aVariable In m_Variables
        If m_nLevelID = aVariable.LevelID Then
            cbVariable.AddItem aVariable.Name
            cbVariable.ItemData(cbVariable.NewIndex) = aVariable.ID
            
            If nVarID = BAD_LONG_VALUE And sVarCaption = aVariable.Caption Then nVarID = aVariable.ID
        End If
    Next
    
    For Each aLevel In m_Levels
        If m_nWeight > aLevel.Weight Then
            lbAddLevels.AddItem aLevel.Name
            lbAddLevels.ItemData(lbAddLevels.NewIndex) = aLevel.ID
        End If
        
        If Not m_bFixedLevel Or aLevel.ID = m_nLevelID Then
            cbVarsLevel.AddItem aLevel.Name
            cbVarsLevel.ItemData(cbVarsLevel.NewIndex) = aLevel.ID
        End If
        
        If aLevel.ID = m_nLevelID Then
            cbVarsLevel.ListIndex = cbVarsLevel.NewIndex
        End If
    Next
    
    m_bIsUserEdit = True
    
    If cbVariable.ListCount > 0 Then
        SetVariable (nVarID)
        btnDelete.Enabled = True
    Else
        btnDelete.Enabled = False
    End If
    
    If m_bAllAggregations Then
        lbAddLevels.Enabled = False
        lbAddLevels.ListIndex = -1
    End If
    
End Sub

Private Sub SaveCustomVariable(ByRef nRisksVarID As Long, ByRef nGreekVarID As Long)
    On Error Resume Next
    Dim i, j As Long
    
    Dim sVariableName As String
    Dim sCaption As String
    Dim sSource As String
    Dim sDesc As String
    
    Dim Errors() As String
    Dim Code() As Byte
    
    Dim cvCompiler1 As New cvCompilerLib.cvCompiler
    Dim cvCompiler2 As New cvCompilerLib.cvCompiler
    
    sVariableName = IIf(txtVariable.Visible, txtVariable.Text, cbVariable.Text)
    sCaption = txtCaption.Text
    sSource = txtCode.Text
    sDesc = txtDesc.Text
    
    Dim nCount, nSelectedCount As Long
    nCount = lbAddLevels.ListCount
    For i = 0 To nCount - 1
        If lbAddLevels.Selected(i) Then nSelectedCount = nSelectedCount + 1
    Next
    
    ReDim Aggregation(nSelectedCount - 1) As Long
    For i = 0 To nCount - 1
        If lbAddLevels.Selected(i) Then
            Aggregation(j) = lbAddLevels.ItemData(i)
            j = j + 1
        End If
    Next
    
    ' Risks variable
    cvCompiler1.ConnectionString = g_Params.DbConnection
    
    nRisksVarID = cvCompiler1.Compile(m_nContextID, m_nLevelID, sVariableName, sCaption, "", sSource, sDesc, Aggregation, Errors, Code)
        
    If nRisksVarID = 0 Then
        For i = 0 To UBound(Errors)
            LogEvent EVENT_ERROR, Errors(i)
            i = i + 1
        Next
        nRisksVarID = BAD_LONG_VALUE
    End If
    
    
    If m_Levels(CStr(m_nLevelID)).Name = "Positions" Then
        ' Greeks variable
        g_cvContextInfo.FillGreekAggregation Aggregation
        
        cvCompiler2.ConnectionString = g_Params.DbConnection
        nGreekVarID = cvCompiler2.Compile(g_cvContextInfo.GetGreekContextID, _
                                          g_cvContextInfo.GetGreekLevelID, _
                                          sVariableName, sCaption, "", sSource, sDesc, Aggregation, Errors, Code)
                                         
        If nGreekVarID = 0 Then
            For i = 0 To UBound(Errors)
                LogEvent EVENT_ERROR, Errors(i)
                i = i + 1
            Next
            nGreekVarID = BAD_LONG_VALUE
        End If
    Else
        nGreekVarID = BAD_LONG_VALUE
    End If
    
    
    If txtVariable.Visible And nRisksVarID <> BAD_LONG_VALUE Then
        txtVariable.Visible = False
        cbVariable.Visible = True
    End If
    
End Sub

Private Sub SetVariable(ByVal nVarID As Long)
    On Error Resume Next
    Dim i As Long
    Dim nListID As Long
    
    m_bIsUserEdit = False
        
    If nVarID <> BAD_LONG_VALUE Then
        For i = 0 To cbVariable.ListCount - 1
            If cbVariable.ItemData(i) = nVarID Then
                nListID = i
            End If
        Next
    End If
    
    cbVariable.ListIndex = nListID
        
    txtCaption.Text = m_Variables(cbVariable.Text).Caption
    txtCode.Text = m_Variables(cbVariable.Text).Source
    txtDesc.Text = m_Variables(cbVariable.Text).Description
        
    For i = 0 To lbAddLevels.ListCount - 1
        lbAddLevels.Selected(i) = m_Variables(cbVariable.Text).IsAggOnLevel(lbAddLevels.ItemData(i))
    Next
    
    m_bIsUserEdit = True
    
    btnApply.Enabled = False
End Sub

Private Sub txtCaption_Change()
    On Error Resume Next
    
    If Not m_bVariableTransfer Then m_bCaptionChange = True
    
    If IsVarValid Then
        btnApply.Enabled = True
    Else
        btnApply.Enabled = False
    End If
End Sub

Private Sub txtCode_Change()
    On Error Resume Next
    
    If Not m_bIsUserEdit Then Exit Sub
    
    If IsVarValid Then
        btnApply.Enabled = True
    Else
        btnApply.Enabled = False
    End If
End Sub

Private Sub txtDesc_Change()
    On Error Resume Next
    
    If Not m_bIsUserEdit Then Exit Sub
    
    If IsVarValid Then
        btnApply.Enabled = True
    Else
        btnApply.Enabled = False
    End If
End Sub

Private Sub txtVariable_Change()
    On Error Resume Next
    
    If Not m_bCaptionChange Then
        m_bVariableTransfer = True
        txtCaption.Text = txtVariable.Text
        m_bVariableTransfer = False
    End If
    
    If IsVarValid Then
        btnApply.Enabled = True
    Else
        btnApply.Enabled = False
    End If
End Sub

Private Sub ClearControls(Optional ByVal bClearLists As Boolean = True)
    On Error Resume Next
    
    m_bIsUserEdit = False
    
    txtVariable.Text = ""
    txtCaption.Text = ""
    txtCode.Text = ""
    txtDesc.Text = ""
    
    Dim i As Long
    For i = 0 To lbAddLevels.ListCount - 1
        lbAddLevels.Selected(i) = IIf(m_bAllAggregations, True, False)
    Next
    
    If bClearLists Then
        cbVariable.Clear
        lbAddLevels.Clear
        cbVarsLevel.Clear
    End If
    
    m_bIsUserEdit = True
    
End Sub

Private Function IsVarValid() As Boolean
    On Error Resume Next
    Dim bIsVarTextValid As Boolean: bIsVarTextValid = True
    
    bIsVarTextValid = IIf(txtVariable.Visible, txtVariable.Text <> "", True)
    
    If bIsVarTextValid And txtCaption.Text <> "" And txtCode.Text <> "" Then
        IsVarValid = True
    Else
        IsVarValid = False
    End If
End Function

Public Sub OnVariableList_Close(ByVal ListType As cvListType)
    On Error Resume Next
    
    Select Case ListType
        Case enSysFuncList
            m_bIsSysFuncShow = False
            btnSysFunc.Caption = "System Functions >>"
            
        Case enSysVarsList
            m_bIsSysVarsShow = False
            btnSysVars.Caption = "System Variables >>"
            
        Case enCstVarsList
            m_bIsCstVarsShow = False
            btnCustomVars.Caption = "Custom Variables >>"
    End Select
    
End Sub

Public Sub OnVariableList_Selected(ByVal Name As String)
    On Error Resume Next
    Dim nPos As Long
  
    nPos = txtCode.SelStart
    txtCode.Text = Left(txtCode.Text, nPos) + Name + Right(txtCode.Text, Len(txtCode.Text) - nPos)
    txtCode.SetFocus
    txtCode.SelStart = nPos + Len(Name)
    
End Sub
