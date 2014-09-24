VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMLColsManager 
   Appearance      =   0  'Flat
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Columns Manager"
   ClientHeight    =   3435
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5895
   Icon            =   "frmMLColsManager.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3435
   ScaleWidth      =   5895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      ForeColor       =   &H80000008&
      Height          =   2775
      Left            =   120
      ScaleHeight     =   2745
      ScaleWidth      =   5625
      TabIndex        =   0
      Top             =   120
      Width           =   5655
      Begin VB.ListBox lstSrc 
         Appearance      =   0  'Flat
         Height          =   2175
         Left            =   120
         MultiSelect     =   2  'Extended
         Sorted          =   -1  'True
         TabIndex        =   1
         Top             =   360
         Width           =   1935
      End
      Begin VB.ListBox lstDest 
         Appearance      =   0  'Flat
         Height          =   2175
         Left            =   2880
         MultiSelect     =   2  'Extended
         TabIndex        =   6
         Top             =   360
         Width           =   1935
      End
      Begin ElladaFlatControls.FlatButton btnDown 
         Height          =   300
         Left            =   4920
         TabIndex        =   8
         Top             =   960
         Width           =   615
         _ExtentX        =   1085
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
         Caption         =   "Down"
      End
      Begin ElladaFlatControls.FlatButton btnUp 
         Height          =   300
         Left            =   4920
         TabIndex        =   7
         Top             =   480
         Width           =   615
         _ExtentX        =   1085
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
         Caption         =   "Up"
      End
      Begin ElladaFlatControls.FlatButton btnMoveRightOne 
         Height          =   300
         Left            =   2160
         TabIndex        =   2
         Top             =   480
         Width           =   615
         _ExtentX        =   1085
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   ">"
      End
      Begin ElladaFlatControls.FlatButton btnMoveLeftOne 
         Height          =   300
         Left            =   2160
         TabIndex        =   3
         Top             =   960
         Width           =   615
         _ExtentX        =   1085
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "<"
      End
      Begin ElladaFlatControls.FlatButton btnMoveRightAll 
         Height          =   300
         Left            =   2160
         TabIndex        =   4
         Top             =   1680
         Width           =   615
         _ExtentX        =   1085
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   ">>"
      End
      Begin ElladaFlatControls.FlatButton btnMoveLeftAll 
         Height          =   300
         Left            =   2160
         TabIndex        =   5
         Top             =   2160
         Width           =   615
         _ExtentX        =   1085
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "<<"
      End
      Begin VB.Label lblDest 
         Alignment       =   2  'Center
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Selected"
         Height          =   195
         Left            =   2880
         TabIndex        =   10
         Top             =   120
         Width           =   1905
         WordWrap        =   -1  'True
      End
      Begin VB.Label lblSrc 
         Alignment       =   2  'Center
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Available"
         Height          =   195
         Left            =   120
         TabIndex        =   9
         Top             =   120
         Width           =   1905
         WordWrap        =   -1  'True
      End
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Height          =   300
      Left            =   1560
      TabIndex        =   11
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
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   3000
      TabIndex        =   12
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
End
Attribute VB_Name = "frmMLColsManager"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private m_arrCols() As clsColumnDefinition
Private m_lColCount As Long
Private m_bDirty As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal lColCount As Long, ByRef arrCols() As clsColumnDefinition) As Boolean
    m_lColCount = lColCount
    m_arrCols = arrCols
    InitLists
    Load Me
    Show vbModal
    Unload Me
    Execute = m_bDirty
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitLists()
Dim i As Long
Dim j As Long
    With lstSrc
        .Clear
        For i = 0 To m_lColCount - 1
            If Not m_arrCols(i).bVisible Then
                .AddItem m_arrCols(i).sName
                .ItemData(.NewIndex) = i
            End If
        Next i
    End With
    With lstDest
        For j = 0 To m_lColCount - 1
            For i = 0 To m_lColCount - 1
                If m_arrCols(i).bVisible And m_arrCols(i).lIndex = j Then
                    .AddItem m_arrCols(i).sName
                    .ItemData(.NewIndex) = i
                End If
            Next i
        Next j
    End With
    m_bDirty = False
    CheckButtonsState
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveArray()
Dim i As Long
    With lstSrc
        For i = 0 To .ListCount - 1
            m_arrCols(.ItemData(i)).bVisible = False
        Next i
    End With
    With lstDest
        For i = 0 To .ListCount - 1
            m_arrCols(.ItemData(i)).bVisible = True
            m_arrCols(.ItemData(i)).lIndex = i
        Next i
    End With
End Sub

'***************************************************************
'       working with lists
'***************************************************************

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' proc for moving list's items from Src to Dest
' Opt=0 - move only selected items, Opt=1 - move all items
'
Private Sub MoveItem(Src As ListBox, Dest As ListBox, Opt As Long)
Dim i As Long
Dim s As String
Dim ID As Long
Dim inx As Long
    On Error Resume Next
    With Src
        i = 0
        Do While i < .ListCount
            If ((Opt = 0) And (.Selected(i))) Or (Opt > 0) Then
                s = .List(i)
                ID = .ItemData(i)
                .RemoveItem i
                Dest.AddItem s
                Dest.ItemData(Dest.NewIndex) = ID
            Else
                i = i + 1
            End If
        Loop
    End With
    CheckButtonsState
    m_bDirty = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnMoveRightOne_Click()
    On Error Resume Next
    MoveItem lstSrc, lstDest, 0
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnMoveLeftOne_Click()
    On Error Resume Next
    MoveItem lstDest, lstSrc, 0
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnMoveRightAll_Click()
    On Error Resume Next
    MoveItem lstSrc, lstDest, 1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnMoveLeftAll_Click()
    On Error Resume Next
    MoveItem lstDest, lstSrc, 1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub CheckButtonsState()
    On Error Resume Next
    btnMoveLeftOne.Enabled = (lstDest.ListCount > 0)
    btnMoveLeftAll.Enabled = (lstDest.ListCount > 0)
    btnMoveRightOne.Enabled = (lstSrc.ListCount > 0)
    btnMoveRightAll.Enabled = (lstSrc.ListCount > 0)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' changing grouping order
' Direction: 1 - up, (-1) - down
'
Private Sub MoveListItem(lst As ListBox, Direction As Long)
Dim i As Long
Dim s As String
Dim b As Boolean
Dim ID As Long
    On Error Resume Next
    With lst
        If (.ListIndex + Direction) < 0 Then Exit Sub
        If (.ListIndex + Direction) >= .ListCount Then Exit Sub
        i = .ListIndex
        s = .List(.ListIndex)
        b = .Selected(.ListIndex)
        ID = .ItemData(.ListIndex)
        .RemoveItem i
        .AddItem s, i + Direction
        .ListIndex = .NewIndex
        .Selected(.NewIndex) = b
        .ItemData(.NewIndex) = ID
    End With
    m_bDirty = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnDown_Click()
    On Error Resume Next
    MoveListItem lstDest, 1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    If m_bDirty Then
        SaveArray
    End If
    Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnUp_Click()
    On Error Resume Next
    MoveListItem lstDest, -1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    With New clsCommon
        .CustomizeForm Me
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub lstDest_DblClick()
    On Error Resume Next
    btnMoveLeftOne_Click
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub lstSrc_DblClick()
    On Error Resume Next
    btnMoveRightOne_Click
End Sub

