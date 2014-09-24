VERSION 5.00
Begin VB.Form frmInfoMsgBox 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Information"
   ClientHeight    =   1770
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   6495
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1770
   ScaleWidth      =   6495
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkInfoCheck 
      Caption         =   "Don't show this message again"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   960
      Width           =   2535
   End
   Begin VB.CommandButton btnAction 
      Caption         =   "Ok"
      Height          =   375
      Left            =   2160
      TabIndex        =   0
      Top             =   1320
      Width           =   1575
   End
   Begin VB.Label lblInfoContent 
      Alignment       =   2  'Center
      Caption         =   "Unable to edit price in Real Time mode"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   735
      Left            =   105
      TabIndex        =   1
      Top             =   120
      Width           =   6270
      WordWrap        =   -1  'True
   End
End
Attribute VB_Name = "frmInfoMsgBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Public Enum EnumMessageBoxMode
    MBM_RT_CLOSE_EDITION = 1
    MBM_FIT2IMP_NO_BIDS = 2
    MBM_LAST = 3
End Enum
Public Mode As EnumMessageBoxMode
Private Sub btnAction_Click()
    Select Case Mode
        Case MBM_RT_CLOSE_EDITION
            If chkInfoCheck.Value = vbChecked Then
                g_Params.ShowMessageUnableToEdit = False
            End If
        Case MBM_FIT2IMP_NO_BIDS
            If chkInfoCheck.Value = vbUnchecked Then
                g_Params.QuoteFitToImpFlatNoBid = False
            End If
        Case MBM_LAST
    End Select
    
    Me.Hide
    Unload Me
End Sub

Public Sub Init(ByVal Title As String, ByVal Content As String, ByVal lMode As EnumMessageBoxMode, Optional ByVal chkVisible As Boolean = True)
    Mode = lMode
    frmInfoMsgBox.Caption = Title
    lblInfoContent.Caption = Content
    SetCheckBoxVisibility chkVisible
End Sub

Public Sub SetCheckBoxVisibility(Optional ByVal chkVisible As Boolean = False)
    chkInfoCheck.Visible = chkVisible
End Sub
Public Sub SetCheckBoxCaption(Optional ByVal sCaption As String)
    chkInfoCheck.Caption = sCaption
End Sub
Public Sub SetMode(Optional ByVal lMode As EnumMessageBoxMode = MBM_RT_CLOSE_EDITION)
    Mode = lMode
End Sub
Public Sub SetCheckBoxChecked(Optional ByVal bChecked As Boolean = False)
    chkInfoCheck.Value = IIf(bChecked, vbChecked, vbUnchecked)
End Sub
