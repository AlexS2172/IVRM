VERSION 5.00
Begin VB.Form frmRiskMatrixGraph 
   Caption         =   "Risk Matrix Graph"
   ClientHeight    =   6615
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   9225
   Icon            =   "frmRiskMatrixGraph.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6615
   ScaleWidth      =   9225
   StartUpPosition =   3  'Windows Default
   Begin EtsMM.ctlRiskMatrixGraph ctlGraph 
      Height          =   3255
      Left            =   840
      TabIndex        =   0
      Top             =   480
      Width           =   7095
      _ExtentX        =   12515
      _ExtentY        =   5741
   End
End
Attribute VB_Name = "frmRiskMatrixGraph"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents ctlSource As ctlRiskMatrix
Attribute ctlSource.VB_VarHelpID = -1
Public Sub ShowRiskMatrixGraph(ByRef Res() As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                               ByRef Axis() As RmAxisTypeEnum, _
                               ByRef Filter As EtsGeneralLib.EtsFilterData, _
                               ByVal nDefID As Long, _
                               ByRef ctlView As ctlRiskMatrix)
    On Error Resume Next
    
    ctlGraph.Init Res, Axis, Filter, nDefID, ctlView

    Set ctlSource = ctlView
    
    Load Me
    Me.Show
End Sub

Private Sub ctlSource_OnScreenClose()
    On Error Resume Next
    Unload Me
End Sub


Private Sub Form_Load()
    On Error Resume Next
    g_Params.SetIcon WND_RISKMATRIX, Me.hWnd, False
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    
    ctlGraph.Move 0, 0, ScaleWidth, ScaleHeight
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set ctlSource = Nothing
End Sub
