VERSION 5.00
Object = "{2B12169D-6738-11D2-BF5B-00A024982E5B}#29.1#0"; "axButton.ocx"
Begin VB.UserControl btnCancel 
   BackColor       =   &H00000000&
   ClientHeight    =   585
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   645
   MouseIcon       =   "btnCancel.ctx":0000
   MousePointer    =   99  'Custom
   Picture         =   "btnCancel.ctx":0152
   ScaleHeight     =   39
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   43
   ToolboxBitmap   =   "btnCancel.ctx":0464
   Begin axButtonControl.axButton AxButton1 
      Height          =   240
      Left            =   0
      TabIndex        =   0
      ToolTipText     =   "Cancel"
      Top             =   0
      Width           =   240
      _ExtentX        =   423
      _ExtentY        =   423
      BackColor       =   0
      ForeColor       =   0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Picture         =   "btnCancel.ctx":0776
      FlatPicture     =   "btnCancel.ctx":0A98
      DisabledPicture =   "btnCancel.ctx":0DBA
      MaskColor       =   0
      ColorDarkShadow =   0
      ColorLightShadow=   0
      ColorHighlight  =   0
   End
End
Attribute VB_Name = "btnCancel"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Event Click()

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AxButton1_Click()
    RaiseEvent Click
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let Enabled(ByVal vNewValue As Boolean)
    With AxButton1
        .Visible = vNewValue
    End With
    MousePointer = IIf(vNewValue, 99, 0)
End Property

