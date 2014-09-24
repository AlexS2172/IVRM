VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMktStrGroupsOfAStock 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Groups Of"
   ClientHeight    =   3435
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4920
   Icon            =   "frmMktStrGroupsOfAStock.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3435
   ScaleWidth      =   4920
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   3480
      TabIndex        =   2
      Top             =   3000
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
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   2040
      TabIndex        =   1
      Top             =   3000
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
      Caption         =   "&OK"
   End
   Begin VB.ListBox lstGroupsOfAStock 
      Appearance      =   0  'Flat
      Height          =   2730
      Left            =   120
      Sorted          =   -1  'True
      Style           =   1  'Checkbox
      TabIndex        =   0
      Top             =   120
      Width           =   4695
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   120
      Picture         =   "frmMktStrGroupsOfAStock.frx":000C
      Top             =   3000
      Visible         =   0   'False
      Width           =   240
   End
End
Attribute VB_Name = "frmMktStrGroupsOfAStock"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Select groups for a symbol
'
Option Explicit

Private m_ItemID As Long
Private m_ItemName As String
Private m_bChanged As Boolean
Private m_bOK As Boolean
Private rsList As ADODB.Recordset
Private m_Edit As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal ItemID As Long, ByVal ItemName As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_ItemID = ItemID
    m_ItemName = ItemName
    Load Me
    Me.Caption = "Groups for " & m_ItemName
    m_bOK = False
    m_Edit = False
    If LoadData Then
        Changed = False
        Screen.MousePointer = vbDefault
        m_Edit = True
        Show vbModal
    End If
    Unload Me
    Execute = m_bOK
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bOK = SaveData
    Screen.MousePointer = vbDefault
    If m_bOK Then Hide
    Exit Sub
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    imgSave.Visible = False
    imgSave.ToolTipText = "Data has been changed"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Get Changed() As Boolean
    Changed = m_bChanged
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Let Changed(ByVal vNewValue As Boolean)
    m_bChanged = vNewValue
    imgSave.Visible = NeedSaveData
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load group list
'
Private Function LoadData() As Boolean
Dim rs As ADODB.Recordset
Dim Title As String
Dim ID As Long
Dim Selected As Boolean
    
    ' Prepare internal recordset
    Set rsList = New ADODB.Recordset
    With rsList
        With .Fields
            .Append "ID", adInteger
            .Append "Name", adVarChar, 64
            .Append "OldValue", adBoolean
            .Append "NewValue", adBoolean
            .Append "Changed", adBoolean
            .Append "ToDelete", adBoolean
            .Append "ToInsert", adBoolean
        End With
        .Open
    End With
    
    ' Load groups for the current contract
    Set rs = gDBW.usp_ContractInGroupByContract_Get(m_ItemID)
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = !iGroupID
                Title = !vcGroupName
                Selected = DBRead(!iMark, 0) <> 0
                With rsList
                    .AddNew
                    !ID = ID
                    !Name = Title
                    !OldValue = Selected
                    !NewValue = Selected
                    !Changed = False
                    !ToDelete = False
                    !ToInsert = False
                    .Update
                End With
                .MoveNext
            Loop
        End If
    End With
    
    ' Fill listbox with group names
    lstGroupsOfAStock.Clear
    With rsList
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                Title = !Name
                ID = !ID
                Selected = !OldValue
                With lstGroupsOfAStock
                    .AddItem Title
                    .ItemData(.NewIndex) = ID
                    .Selected(.NewIndex) = Selected
                End With
                .MoveNext
            Loop
        End If
    End With
    
    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NeedSaveData() As Boolean
Dim i As Long
Dim ID As Long
Dim Selected As Boolean
    On Error GoTo Herr
    ' Scan listbox to detect changes
    With lstGroupsOfAStock
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    If Selected <> !OldValue Then
                        NeedSaveData = True
                        Exit Function
                    End If
                Else
                    Debug.Assert False
                End If
            End With
        Next i
    End With
    Exit Function
Herr:
    Debug.Print Err.Description
    Debug.Assert False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Save groups for the current contract
'
Private Function SaveData() As Boolean
Dim i As Long
Dim ID As Long
Dim Selected As Boolean
Dim v As Long
    
    ' Scan listbox and update recordset
    With lstGroupsOfAStock
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    !NewValue = Selected
                    If !NewValue <> !OldValue Then
                        !Changed = True
                    End If
                    .Update
                Else
                    Debug.Assert False
                End If
            End With
        Next i
    End With
    
    ' Detect changed items
    With rsList
        .Filter = "Changed = TRUE"
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                If !NewValue = False Then
                    !ToDelete = True
                Else
                    !ToInsert = True
                End If
                .Update
                .MoveNext
            Loop
        End If
    End With
    
    ' Remove deselected items
    With rsList
        .Filter = "ToDelete = true"
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = !ID
                gDBW.usp_ContractInGroupByContract_Del m_ItemID, ID
                .MoveNext
            Loop
        End If
    End With
    
    ' Insert selected items
    With rsList
        .Filter = "ToInsert = true"
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = !ID
                gDBW.usp_ContractInGroupByContract_Save m_ItemID, ID
                .MoveNext
            Loop
        End If
    End With
    
    SaveData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub lstGroupsOfAStock_ItemCheck(Item As Integer)
    On Error Resume Next
    If m_Edit Then
        Changed = True
    End If
End Sub

