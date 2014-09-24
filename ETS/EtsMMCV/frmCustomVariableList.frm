VERSION 5.00
Begin VB.Form frmCustomVariableList 
   Caption         =   "Custom Variables"
   ClientHeight    =   6855
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   3735
   Icon            =   "frmCustomVariableList.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6855
   ScaleWidth      =   3735
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnClose 
      Caption         =   "Close"
      Height          =   315
      Left            =   2280
      TabIndex        =   4
      Top             =   6480
      Width           =   1335
   End
   Begin VB.CommandButton btnSelect 
      Caption         =   "Select"
      Height          =   315
      Left            =   120
      TabIndex        =   3
      Top             =   6480
      Width           =   1335
   End
   Begin VB.TextBox txtDesc 
      BackColor       =   &H8000000B&
      Height          =   1965
      Left            =   120
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   1
      Top             =   4440
      Width           =   3495
   End
   Begin VB.ListBox lbVars 
      Height          =   4065
      IntegralHeight  =   0   'False
      ItemData        =   "frmCustomVariableList.frx":058A
      Left            =   120
      List            =   "frmCustomVariableList.frx":058C
      Sorted          =   -1  'True
      TabIndex        =   0
      Top             =   120
      Width           =   3495
   End
   Begin VB.Label Label1 
      Caption         =   "Description"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   4200
      Width           =   3495
   End
End
Attribute VB_Name = "frmCustomVariableList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Public Enum cvListType
    enSysFuncList
    enSysVarsList
    enCstVarsList
End Enum

Private m_ListType As cvListType
Private m_List As Collection

Private m_nLevelID As Long

Private m_nFrmHeight As Long
Private m_nFrmWidth As Long

Private m_nFrmMinHeight As Long
Private m_nFrmMinWidth As Long

Private m_bIsOpened As Boolean

Private m_frmOwner As frmCustomVariableEditor

Public Sub ShowList(ByRef cvList As Collection, ByVal ListType As cvListType, ByVal nLevelID As Long, frmOwner As frmCustomVariableEditor)
    On Error Resume Next
    
    If m_bIsOpened Then Exit Sub
    m_bIsOpened = True
    
    Set m_frmOwner = frmOwner
    
    SetData cvList, ListType, nLevelID
    InitControls
    
    Load Me
    Me.Show
End Sub

Private Sub btnClose_Click()
    On Error Resume Next
    
    Unload Me
End Sub

Private Sub btnSelect_Click()
    On Error Resume Next
    
    m_frmOwner.OnVariableList_Selected (lbVars.Text)
End Sub

Private Sub Form_Load()
    On Error Resume Next
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
    
    m_nFrmMinHeight = 2220
    m_nFrmMinWidth = 3135
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    Dim bExitSub As Boolean
    
    If Me.Height < m_nFrmMinHeight Then
        Me.Height = m_nFrmMinHeight
        bExitSub = True
    End If
    
    If Me.Width < m_nFrmMinWidth Then
        Me.Width = m_nFrmMinWidth
        bExitSub = True
    End If
    
    If bExitSub Then Exit Sub
    
    Dim nHeightShift As Long: nHeightShift = (Me.Height - m_nFrmHeight) / 3
    Dim nWidthShift As Long: nWidthShift = Me.Width - m_nFrmWidth
    
    lbVars.Height = lbVars.Height + nHeightShift * 2
    lbVars.Width = lbVars.Width + nWidthShift
    
    Label1.Top = Label1.Top + nHeightShift * 2
    
    txtDesc.Top = txtDesc.Top + nHeightShift * 2
    txtDesc.Height = txtDesc.Height + nHeightShift
    txtDesc.Width = txtDesc.Width + nWidthShift
    
    btnSelect.Top = btnSelect.Top + nHeightShift * 3
    
    btnClose.Top = btnClose.Top + nHeightShift * 3
    btnClose.Left = btnClose.Left + nWidthShift
    
    m_nFrmHeight = Me.Height
    m_nFrmWidth = Me.Width
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    m_bIsOpened = False
    
    m_frmOwner.OnVariableList_Close (m_ListType)
End Sub

Private Sub InitControls(Optional ByVal VarName As String = STR_NA)
    On Error Resume Next
    Dim aSysFunc As clsCVSysFunc
    Dim aSysVar As clsCVSysVar
    Dim aCstVar As clsCVVariable
    
    lbVars.Clear
    txtDesc.Text = ""
    
    Select Case m_ListType
        Case enSysFuncList
            Me.Caption = "System functions"
            For Each aSysFunc In m_List
                If m_nLevelID = aSysFunc.LevelID Or (aSysFunc.IsGlobal And aSysFunc.LevelID < m_nLevelID) Then
                    lbVars.AddItem aSysFunc.Name
                End If
            Next
            
        Case enSysVarsList
            Me.Caption = "System variables"
            For Each aSysVar In m_List
                If m_nLevelID = aSysVar.LevelID Or (aSysVar.IsGlobal And aSysVar.LevelID < m_nLevelID) Then
                    lbVars.AddItem aSysVar.Name
                End If
            Next
        
        Case enCstVarsList
            Me.Caption = "Custom variables"
            For Each aCstVar In m_List
                If m_nLevelID = aCstVar.LevelID Then
                    lbVars.AddItem aCstVar.Name
                End If
            Next
        
        Case Else
    End Select
    
    If VarName <> STR_NA Then SetVariable (VarName)
    
End Sub

Private Sub SetVariable(ByVal VarName As String)
    On Error Resume Next
    
    For i = 0 To lbVars.ListCount
        If lbVars.List(i) = VarName Then
            lbVars.ListIndex = i
            Exit For
        End If
    Next
    
    txtDesc.Text = m_List(lbVars.Text).Description
End Sub

Private Sub lbVars_Click()
    On Error Resume Next
            
    txtDesc.Text = m_List(lbVars.Text).Description
End Sub

Private Sub SetData(ByRef cvList As Collection, ByVal ListType As cvListType, ByVal nLevelID As Long)
    On Error Resume Next
    
    m_ListType = ListType
    
    Set m_List = Nothing
    Set m_List = cvList
    
    m_nLevelID = nLevelID
End Sub

Public Sub OnEditor_DataChanged(ByRef cvList As Collection, ByVal nLevelID As Long)
    On Error Resume Next
    Dim sCurVarName As String
    sCurVarName = lbVars.Text
    
    SetData cvList, m_ListType, nLevelID
    InitControls
    SetVariable sCurVarName
    
End Sub

Private Sub lbVars_DblClick()
    On Error Resume Next
    
    m_frmOwner.OnVariableList_Selected (lbVars.Text)
End Sub
