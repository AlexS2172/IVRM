VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEntTraderProp 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Trader's Properties"
   ClientHeight    =   2310
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4485
   Icon            =   "frmEntTraderProp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2310
   ScaleWidth      =   4485
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   1635
      Left            =   120
      ScaleHeight     =   1605
      ScaleWidth      =   4215
      TabIndex        =   0
      Top             =   120
      Width           =   4245
      Begin VB.ComboBox cmbGroups 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   2520
         TabIndex        =   7
         Text            =   "Combo1"
         Top             =   600
         Width           =   1335
      End
      Begin ElladaFlatControls.FlatButton btnStocksList 
         Height          =   300
         Left            =   2520
         TabIndex        =   5
         Top             =   1020
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
         Caption         =   "Edit list ..."
      End
      Begin VB.Label lbl1 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Trader's Group"
         Height          =   195
         Left            =   240
         TabIndex        =   6
         Top             =   600
         Width           =   1875
         WordWrap        =   -1  'True
      End
      Begin VB.Label lbl3 
         BackStyle       =   0  'Transparent
         Caption         =   "Selected Stocks List"
         Height          =   195
         Left            =   240
         TabIndex        =   4
         Top             =   1020
         Width           =   1860
         WordWrap        =   -1  'True
      End
      Begin VB.Label lbl0 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Properties for trader :"
         Height          =   195
         Left            =   240
         TabIndex        =   1
         Top             =   240
         Width           =   3675
         WordWrap        =   -1  'True
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   3000
      TabIndex        =   2
      Top             =   1860
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
      Left            =   1560
      TabIndex        =   3
      Top             =   1860
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
      Picture         =   "frmEntTraderProp.frx":000C
      Top             =   1860
      Visible         =   0   'False
      Width           =   240
   End
End
Attribute VB_Name = "frmEntTraderProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Edit Trader's properties
'
Option Explicit

Private m_nID As Long
Private m_sName As String
Private m_nGroupID As Long

Private m_bChanged As Boolean
Private m_bOK As Boolean
Private rsList As ADODB.Recordset

Private m_bLoadData As Boolean
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal Id As Long, ByVal Name As String) As Boolean
    On Error GoTo Herr
    m_nID = Id
    m_sName = Name
    m_bLoadData = True
    m_bOK = False
    Load Me
    lbl0.Caption = "Properties for trader : " & m_sName
    Show vbModal
    Unload Me
    Execute = m_bOK
    Exit Function
Herr:
    ShowError "Unable to execute Trader Properties dialogue"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    If m_bChanged Then m_bOK = SaveData
    If Not m_bChanged Then Hide
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
    LoadData
    imgSave.Visible = False
    imgSave.ToolTipText = "Data has been changed"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load data from database
'
Private Function LoadData() As Boolean
    Dim rsGroups As ADODB.Recordset
    Dim rsTrader As ADODB.Recordset
    Dim I As Long
    Dim s As String
    
    On Error GoTo Herr
    LoadData = False
       
    Set rsTrader = gDBW.usp_BoTrader_Get(m_nID)
    If Not rsTrader Is Nothing Then
        With rsTrader
            If Not .EOF Then
                m_nGroupID = DBRead(!iTraderGroupID, 0)
            End If
        End With
    End If
    
    cmbGroups.Clear
    cmbGroups.AddItem "< None >"
    cmbGroups.ItemData(cmbGroups.NewIndex) = 0
    cmbGroups.ListIndex = 0
    
    Set rsGroups = gDBW.usp_BoTraderGroup_Get(Null)
    If Not rsGroups Is Nothing Then
        With rsGroups
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    I = gCmn.ReadLng(!iTraderGroupID)
                    cmbGroups.AddItem gCmn.ReadStr(!vcGroupName)
                    cmbGroups.ItemData(cmbGroups.NewIndex) = I
                    If m_nGroupID = I Then _
                        cmbGroups.ListIndex = cmbGroups.NewIndex

                    .MoveNext
                Loop
            End If
        End With
    End If
    
    m_bChanged = False
    m_bLoadData = False
    LoadData = True
    Exit Function
Herr:
    ShowError "Unable to load trader"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Save data into database
'
Private Function SaveData() As Boolean
    Dim IdGr&
    On Error GoTo Herr
    SaveData = False
    
    IdGr = cmbGroups.ItemData(cmbGroups.ListIndex)
    gDBW.usp_BoTrader_Save m_nID, Null, Null, Null, DBWrite(IdGr, 0)
    m_nGroupID = IdGr
    m_bChanged = False
    SaveData = True
    imgSave.Visible = m_bChanged
    Exit Function
Herr:
    ShowError "Unable to save trader stocks"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbGroups_Click()
    On Error Resume Next
    If m_bLoadData Then Exit Sub
    
    m_bChanged = cmbGroups.ItemData(cmbGroups.ListIndex) <> m_nGroupID
    
    imgSave.Visible = m_bChanged
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnStocksList_Click()
    On Error Resume Next
    If m_bChanged Then
        If gCmn.MyMsgBox("Save trader data?", vbYesNo) = vbNo Then Exit Sub
    End If
    Me.MousePointer = vbHourglass
    If m_bChanged Then SaveData
    frmEntTraderContractsProp.Execute m_nID, m_sName
    Me.MousePointer = vbDefault
End Sub

