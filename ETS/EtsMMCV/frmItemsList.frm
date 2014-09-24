VERSION 5.00
Begin VB.Form frmItemsList 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Select Items"
   ClientHeight    =   3465
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4410
   Icon            =   "frmItemsList.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3465
   ScaleWidth      =   4410
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton btnClrAll 
      Caption         =   "&Clear All"
      Height          =   315
      Left            =   3180
      TabIndex        =   4
      Top             =   1560
      Width           =   1095
   End
   Begin VB.CommandButton btnSelAll 
      Caption         =   "&Select All"
      Height          =   315
      Left            =   3180
      TabIndex        =   3
      Top             =   1200
      Width           =   1095
   End
   Begin VB.ListBox lstItems 
      Height          =   2985
      ItemData        =   "frmItemsList.frx":000C
      Left            =   120
      List            =   "frmItemsList.frx":000E
      Style           =   1  'Checkbox
      TabIndex        =   0
      Top             =   360
      Width           =   2895
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   3180
      TabIndex        =   1
      Top             =   360
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   3180
      TabIndex        =   2
      Top             =   720
      Width           =   1095
   End
   Begin VB.Label lblPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "Prompt :"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   2775
   End
End
Attribute VB_Name = "frmItemsList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK(ByVal nKey As Long, ByRef aItems As EtsGeneralLib.EtsMmEntityAtomColl)

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bDirty As Boolean
Private m_Items As New EtsGeneralLib.EtsMmEntityAtomColl
Private m_nKey As Long
Private m_bAllowAllDeselect As Boolean

Public Sub Execute(ByVal sCaption As String, ByVal sPrompt As String, ByRef aItems As EtsGeneralLib.EtsMmEntityAtomColl, _
                    ByVal nKey As Long, ByRef frmOwner As Form, Optional ByVal bAllowAllDeselect As Boolean = False)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    m_bDirty = False
    m_nKey = nKey
    m_bAllowAllDeselect = bAllowAllDeselect
    
    Load Me
    Me.Caption = sCaption
    lblPrompt.Caption = sPrompt
    
    Dim i&, nCount&, aItem As EtsGeneralLib.EtsMmEntityAtom, aInItem As EtsGeneralLib.EtsMmEntityAtom
    
    m_Items.Clear
    lstItems.Clear
    
    nCount = aItems.Count
    For i = 1 To nCount
        Set aInItem = aItems(i)
        If m_Items(CStr(aInItem.ID)) Is Nothing Then
            Set aItem = m_Items.Add(CStr(aInItem.ID))
            aItem.ID = aInItem.ID
            aItem.Name = aInItem.Name
            aItem.Data = aInItem.Data
            aItem.Data3 = aInItem.Data3
            
            lstItems.AddItem aItem.Name
            lstItems.ItemData(lstItems.NewIndex) = aItem.ID
            lstItems.Selected(lstItems.NewIndex) = (aItem.Data <> 0)
            Set aItem = Nothing
        End If
        Set aInItem = Nothing
    Next
    
    m_bDirty = False
    
    If lstItems.ListCount > 0 Then
        lstItems.ListIndex = 0
        btnOK.Enabled = True
    Else
        btnOK.Enabled = False
    End If
    
    Me.Show vbModeless, m_frmOwner
    
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open items dialog."
    On Error Resume Next
    Unload Me
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    'Me.Hide
    Unload Me
End Sub

Private Sub btnClrAll_Click()
    On Error Resume Next
    Dim i&, nIndex&
    
    With lstItems
        nIndex = .ListIndex
        For i = 0 To .ListCount - 1
            .Selected(i) = False
        Next
        .ListIndex = nIndex
    End With
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Dim nCount&, i&, aItem As EtsGeneralLib.EtsMmEntityAtom
    'Me.Hide
    If m_bDirty Then
        Screen.MousePointer = vbHourglass
        
        With lstItems
            nCount = .ListCount - 1
            For i = 0 To nCount
                Set aItem = m_Items(CStr(.ItemData(i)))
                If Not aItem Is Nothing Then
                    aItem.Data = IIf(.Selected(i), 1, 0)
                    
                    'If Not g_PerformanceLog Is Nothing Then _
                        'g_PerformanceLog.LogMmInfo enLogUserAction, "After Edit " & Me.Caption & " """ & aItem.Name & IIf(aItem.Data = 1, """ Selected.", """ Not Selected."), m_frmOwner.Caption
                    
                    Set aItem = Nothing
                End If
            Next
        End With
        
        RaiseEvent OnOK(m_nKey, m_Items)
        Screen.MousePointer = vbDefault
    End If
    Unload Me
End Sub

Private Sub btnSelAll_Click()
    On Error Resume Next
    Dim i&, nIndex&
    
    With lstItems
        nIndex = .ListIndex
        For i = 0 To .ListCount - 1
            .Selected(i) = True
        Next
        .ListIndex = nIndex
    End With
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    m_Items.Clear
    lstItems.Clear
    Form_Terminate
End Sub

Private Sub lstItems_ItemCheck(Item As Integer)
    On Error Resume Next
    m_bDirty = True
    btnOK.Enabled = (m_bAllowAllDeselect Or lstItems.SelCount > 0)
End Sub
