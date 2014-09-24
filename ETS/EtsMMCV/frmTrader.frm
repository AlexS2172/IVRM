VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmTrader 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Active Trader"
   ClientHeight    =   1560
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   3780
   Icon            =   "frmTrader.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1560
   ScaleWidth      =   3780
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CheckBox chkDefault 
      Caption         =   "Use As Default Active Trader"
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   750
      Width           =   3495
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   743
      TabIndex        =   2
      Top             =   1140
      Width           =   1095
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   1943
      TabIndex        =   3
      Top             =   1140
      Width           =   1095
   End
   Begin MSForms.ComboBox cboTrader 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   3555
      VariousPropertyBits=   679499803
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "6271;556"
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblPrompt 
      BackStyle       =   0  'Transparent
      Caption         =   "Select an Active Trader :"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   2415
   End
End
Attribute VB_Name = "frmTrader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_mbResult As VbMsgBoxResult

Public Function Execute(ByRef nTraderID As Long, frmOwner As Form) As VbMsgBoxResult
    On Error Resume Next
    Load Me
    m_mbResult = vbCancel

    If LoadTraders(nTraderID, frmOwner) Then
        Show vbModal, frmOwner
        nTraderID = cboTrader.Value
        
        If m_mbResult = vbOK Then
            g_Params.DefTraderID = nTraderID
            
            If chkDefault.Value <> vbUnchecked Then
                g_Params.UseDefTrader = True
            Else
                g_Params.UseDefTrader = False
            End If
        End If
    End If
    
    Unload Me
    Execute = m_mbResult
End Function

Private Function LoadTraders(ByRef nTraderID As Long, frmOwner As Form) As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    Dim i&, nID&, sName$
    
    LoadTraders = False
    
    With cboTrader
        .SelectionMargin = False
        .ColumnCount = 2
        .BoundColumn = 1
        .TextColumn = 2
        .ColumnWidths = "0;-1;"
        .Clear
        
        .AddItem 0, 0
        .List(0, 0) = 0
        .List(0, 1) = "<All Traders>"
        
        If nTraderID = 0 Then
            .ListIndex = 0
            If g_Params.UseDefTrader Then m_mbResult = vbOK
        End If
        
        i = 0
        Set rs = gDBW.usp_Trader_Get(0)
        While Not rs.EOF
            nID = ReadLng(rs!iTraderID)
            sName = Trim$(ReadStr(rs!vcAcronym))
            
            i = i + 1
            .AddItem 0, i
            .List(i, 0) = nID
            .List(i, 1) = sName
            If nID = nTraderID Then
                .ListIndex = i
                If g_Params.UseDefTrader Then m_mbResult = vbOK
            End If
            
            rs.MoveNext
        Wend
        Set rs = Nothing
        
        If .ListIndex < 0 Then
            .ListIndex = 0
            g_Params.UseDefTrader = False
        End If
        btnOK.Enabled = True
        LoadTraders = (m_mbResult <> vbOK)
    End With
    Exit Function
EH:
    gCmn.ErrorMsgBox frmOwner, "Fail to load traders list."
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    Hide
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Hide
    m_mbResult = vbOK
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub
