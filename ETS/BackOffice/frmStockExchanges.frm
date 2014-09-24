VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmStockExchanges 
   BackColor       =   &H8000000E&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Exchanges"
   ClientHeight    =   5010
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5910
   Icon            =   "frmStockExchanges.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5010
   ScaleWidth      =   5910
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.ListBox lstGroupsOfAStock2 
      Appearance      =   0  'Flat
      Height          =   4080
      Left            =   3000
      Sorted          =   -1  'True
      Style           =   1  'Checkbox
      TabIndex        =   1
      Top             =   360
      Width           =   2775
   End
   Begin VB.ListBox lstGroupsOfAStock1 
      Appearance      =   0  'Flat
      CausesValidation=   0   'False
      Height          =   4080
      Left            =   120
      Sorted          =   -1  'True
      Style           =   1  'Checkbox
      TabIndex        =   0
      Top             =   360
      Width           =   2775
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   4440
      TabIndex        =   3
      Top             =   4560
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
      Left            =   3000
      TabIndex        =   2
      Top             =   4560
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
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   120
      Picture         =   "frmStockExchanges.frx":000C
      Top             =   4560
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Options"
      Height          =   255
      Left            =   3000
      TabIndex        =   5
      Top             =   120
      Width           =   2535
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Stock"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   120
      Width           =   2415
   End
End
Attribute VB_Name = "frmStockExchanges"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Select exchanges for a symbol
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
    Me.Caption = "Exchanges for " & m_ItemName
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
' Loading Exchanges list
'
Private Function LoadData() As Boolean
Dim rs As ADODB.Recordset
Dim Title As String
Dim ID As Long
Dim Selected1 As Boolean, IsUnderlyingExchange As Boolean, IsOptionExchange As Boolean
Dim Selected2 As Boolean

    ' Prepare internal recordset
    Set rsList = New ADODB.Recordset
    With rsList
        With .Fields
            .Append "ID", adInteger
            .Append "Name", adVarChar, 64
            .Append "OldValue1", adBoolean
            .Append "NewValue1", adBoolean
            .Append "OldValue2", adBoolean
            .Append "NewValue2", adBoolean
            .Append "Changed", adBoolean
            .Append "ToDelete", adBoolean
            .Append "ToInsert", adBoolean
            .Append "IsUnderlyingExchange", adBoolean
            .Append "IsOptionExchange", adBoolean
        End With
        .Open
    End With

    ' Load exchanges for the current contract
    Set rs = gDBW.usp_ContractInExchange_Get(m_ItemID)
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = !iExchangeId
                Title = rs!vcExchangeName
                Selected1 = DBRead(!iType1, 0) <> 0
                Selected2 = DBRead(!iType2, 0) <> 0
                IsUnderlyingExchange = DBRead(!tiIsUnderlyingExchange, 0) <> 0
                IsOptionExchange = DBRead(!tiIsOptionExchange, 0) <> 0
                
                With rsList
                    .AddNew
                    !ID = ID
                    !Name = Title
                    !OldValue1 = Selected1
                    !NewValue1 = Selected1
                    !OldValue2 = Selected2
                    !NewValue2 = Selected2
                    !Changed = False
                    !ToDelete = False
                    !ToInsert = False
                    !IsUnderlyingExchange = IsUnderlyingExchange
                    !IsOptionExchange = IsOptionExchange
                    .Update
                End With
                .MoveNext
            Loop
        End If
    End With

    ' Fill listboxes with Exchange names
    lstGroupsOfAStock1.Clear
    lstGroupsOfAStock2.Clear
    With rsList
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                Title = !Name
                ID = !ID
                Selected1 = !OldValue1
                Selected2 = !OldValue2
                IsUnderlyingExchange = !IsUnderlyingExchange
                IsOptionExchange = !IsOptionExchange
                
                If IsUnderlyingExchange Then
                    With lstGroupsOfAStock1
                        .AddItem Title
                        .ItemData(.NewIndex) = ID
                        .Selected(.NewIndex) = Selected1
                    End With
                End If
                
                If IsOptionExchange Then
                    With lstGroupsOfAStock2
                        .AddItem Title
                        .ItemData(.NewIndex) = ID
                        .Selected(.NewIndex) = Selected2
                    End With
                End If
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
    
    ' Scan listbox and update recordset
    With lstGroupsOfAStock1
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    If Selected <> !OldValue1 Then
                        NeedSaveData = True
                        Exit Function
                    End If
                Else
                    Debug.Assert False
                End If
            End With
        Next i
    End With
    
    ' Scan listbox and update recordset
    With lstGroupsOfAStock2
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    If Selected <> !OldValue2 Then
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
' saving Exchanges for the current contract
'
Private Function SaveData() As Boolean
Dim i As Long
Dim ID As Long
Dim Selected As Boolean
Dim v1 As Long
Dim v2 As Long
    
    ' Scan listbox and update recordset
    With lstGroupsOfAStock1
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    !NewValue1 = Selected
                    If !NewValue1 <> !OldValue1 Then
                        !Changed = True
                    End If
                    .Update
                Else
                    Debug.Assert False
                End If
            End With
        Next i
    End With
    
    ' Scan listbox and update recordset
    With lstGroupsOfAStock2
        For i = 0 To .ListCount - 1
            ID = .ItemData(i)
            Selected = .Selected(i)
            With rsList
                .MoveFirst
                .Find "ID=" & Format$(ID)
                If Not .EOF Then
                    !NewValue2 = Selected
                    If !NewValue2 <> !OldValue2 Then
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
                If (!NewValue1 = False) And (!NewValue2 = False) Then
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
                gDBW.usp_ContractInExchange_Del ID, m_ItemID
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
                v1 = IIf(!NewValue1, 1, 0)
                v2 = IIf(!NewValue2, 1, 0)
                gDBW.usp_ContractInExchange_Save ID, m_ItemID, v1, v2
                .MoveNext
            Loop
        End If
    End With
    
    SaveData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub lstGroupsOfAStock1_ItemCheck(Item As Integer)
    On Error Resume Next
    If m_Edit Then
        Changed = True
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub lstGroupsOfAStock2_ItemCheck(Item As Integer)
    On Error Resume Next
    If m_Edit Then
        Changed = True
    End If
End Sub

