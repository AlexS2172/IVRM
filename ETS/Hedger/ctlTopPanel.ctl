VERSION 5.00
Begin VB.UserControl ctlTopPanel 
   BackColor       =   &H00000000&
   ClientHeight    =   3825
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   12420
   ScaleHeight     =   3825
   ScaleWidth      =   12420
   Begin VB.PictureBox picTopNavigation 
      Appearance      =   0  'Flat
      BackColor       =   &H00000000&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   3120
      Index           =   0
      Left            =   2640
      ScaleHeight     =   3120
      ScaleWidth      =   6465
      TabIndex        =   1
      Tag             =   "noauto"
      Top             =   360
      Width           =   6465
      Begin VB.Label btn 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "   Stocks"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   4
         Left            =   5280
         MouseIcon       =   "ctlTopPanel.ctx":0000
         MousePointer    =   99  'Custom
         TabIndex        =   7
         Top             =   90
         Width           =   975
      End
      Begin VB.Label btn 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "Long"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   3
         Left            =   3960
         MouseIcon       =   "ctlTopPanel.ctx":0152
         MousePointer    =   99  'Custom
         TabIndex        =   6
         Top             =   90
         Width           =   1095
      End
      Begin VB.Label btn 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "Short"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   2
         Left            =   2760
         MouseIcon       =   "ctlTopPanel.ctx":02A4
         MousePointer    =   99  'Custom
         TabIndex        =   5
         Top             =   90
         Width           =   975
      End
      Begin VB.Label btn 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "    Summary"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   1
         Left            =   1440
         MouseIcon       =   "ctlTopPanel.ctx":03F6
         MousePointer    =   99  'Custom
         TabIndex        =   4
         Top             =   90
         Width           =   1035
      End
      Begin VB.Label btn 
         Alignment       =   2  'Center
         BackStyle       =   0  'Transparent
         Caption         =   "    Portfolio"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   0
         Left            =   120
         MouseIcon       =   "ctlTopPanel.ctx":0548
         MousePointer    =   99  'Custom
         TabIndex        =   3
         Top             =   90
         Width           =   1095
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   5
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":069A
         Top             =   2400
         Width           =   6435
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   4
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":93BC
         Top             =   1920
         Width           =   6435
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   0
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":120DE
         Top             =   0
         Width           =   6435
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   1
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":1AE00
         Top             =   480
         Width           =   6435
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   2
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":23B22
         Top             =   960
         Width           =   6435
      End
      Begin VB.Image imgState 
         Height          =   420
         Index           =   3
         Left            =   0
         Picture         =   "ctlTopPanel.ctx":2C844
         Top             =   1440
         Width           =   6435
      End
   End
   Begin VB.PictureBox picTopLeft 
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   615
      Left            =   0
      Picture         =   "ctlTopPanel.ctx":35566
      ScaleHeight     =   615
      ScaleWidth      =   2700
      TabIndex        =   0
      Top             =   60
      Width           =   2700
   End
   Begin EgarHedgerSQL.ctlIndicator ctlIndicator1 
      Height          =   615
      Left            =   9120
      TabIndex        =   2
      Top             =   120
      Width           =   3135
      _ExtentX        =   5530
      _ExtentY        =   1085
   End
End
Attribute VB_Name = "ctlTopPanel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Event ButtonClick(ByVal Index As Long)
Public Event CancelClick()

Private Const BTN_FIRST = 0
Private Const BTN_LAST = 4

Private CurrentTab As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btn_Click(Index As Integer)
    On Error Resume Next
    RaiseEvent ButtonClick(Index)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub BtnSelect(ByVal Index As Long)
Dim i As Long
Dim c As Long
    On Error Resume Next
    CurrentTab = Index
    If CurrentTab <= BTN_FIRST Then
        CurrentTab = BTN_FIRST
    End If
    If CurrentTab >= BTN_LAST Then
        CurrentTab = BTN_LAST
    End If
    
'    Select Case Index
'    Case 0
'        c = GCOLOR_TAB_1
'    Case 1
'        c = GCOLOR_TAB_2
'    Case 2
'        c = GCOLOR_TAB_3
'    Case 3
'        c = GCOLOR_TAB_4
'    Case 4
'        c = GCOLOR_TAB_5
'    Case Else
'        c = GCOLOR_TAB_0
'    End Select
    
    imgState(0).Visible = False
    imgState(Index + 1).Visible = True
'    btn(Index).BackColor = c
    btn(Index).ForeColor = GCOLOR_LABEL_1
    
    For i = BTN_FIRST To BTN_LAST
        If i <> Index Then
'            btn(i).BackColor = GCOLOR_TAB_0
            btn(i).ForeColor = GCOLOR_LABEL_0
            imgState(i + 1).Visible = False
        End If
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Dim i As Long
    With imgState
        For i = .LBound To .UBound
            With .Item(i)
                .Visible = (i = 0)
                .Top = 0
                .Left = 0
            End With
        Next
    End With
    ctlIndicator1.StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    With ctlIndicator1
        .Left = Max(8760, ScaleWidth - .Width - 40)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
Dim i As Long
    On Error Resume Next
    For i = BTN_FIRST To BTN_LAST
        btn(i).Enabled = False
    Next
    With ctlIndicator1
        .StartTask Caption, CanCancel
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
Dim i As Long
    On Error Resume Next
    With ctlIndicator1
        .StopTask Caption
    End With
    For i = BTN_FIRST To BTN_LAST
        btn(i).Enabled = True
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlIndicator1_CancelClick()
    On Error Resume Next
    RaiseEvent CancelClick
End Sub

