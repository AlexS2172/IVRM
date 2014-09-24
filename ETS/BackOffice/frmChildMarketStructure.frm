VERSION 5.00
Begin VB.Form frmChildMarketStructure 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   0  'None
   Caption         =   "Market Structure"
   ClientHeight    =   4605
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   7215
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MDIChild        =   -1  'True
   MinButton       =   0   'False
   Moveable        =   0   'False
   ScaleHeight     =   4605
   ScaleWidth      =   7215
   ShowInTaskbar   =   0   'False
   Tag             =   "1"
   Begin ElladaBO.ctlMktStruct ctlMktStruct1 
      Height          =   4335
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   6915
      _ExtentX        =   12197
      _ExtentY        =   7646
   End
End
Attribute VB_Name = "frmChildMarketStructure"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Function CanLeaveFocus() As Boolean
    On Error Resume Next
    CanLeaveFocus = ctlMktStruct1.ContractCanProceed
End Function

Private Sub Form_Load()
    On Error GoTo Herr
    Me.Visible = False
    ctlMktStruct1.Init
    Exit Sub
Herr:
    ShowError
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    With ctlMktStruct1
        .Move 0, 0, ScaleWidth - .Left, ScaleHeight - .Top
    End With
End Sub

Private Sub Form_Activate()
    On Error GoTo Herr
    frmMain.SelectActiveChild
    ctlMktStruct1.Activate
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function RefreshMarket()
    On Error Resume Next
    ctlMktStruct1.RefreshMarket
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function RefreshCurrent()
    On Error Resume Next
    ctlMktStruct1.RefreshCurrent
End Function
