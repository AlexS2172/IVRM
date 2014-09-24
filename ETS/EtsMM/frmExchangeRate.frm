VERSION 5.00
Begin VB.Form frmExchangeRate 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Currency exchange rate"
   ClientHeight    =   1035
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   3015
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1035
   ScaleWidth      =   3015
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtRate 
      Height          =   315
      Left            =   1560
      TabIndex        =   2
      Top             =   120
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   315
      Left            =   1560
      TabIndex        =   1
      Top             =   600
      Width           =   1335
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   600
      Width           =   1335
   End
   Begin VB.Label lblRegions 
      Caption         =   "USD/CAN:"
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   180
      Width           =   1335
   End
End
Attribute VB_Name = "frmExchangeRate"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_dRate As Double

Public Sub EditRate(frmOwner As Form)
    On Error Resume Next
    If m_bIsOpened Then Exit Sub
    
    m_bIsOpened = True
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    Load Me
    
    m_dRate = CDbl(g_Params.ExchangeRate)
    txtRate.Text = CStr(m_dRate)

    Me.Show vbModeless, m_frmOwner
    
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub SetPPRegionRate(sRegion As String, dRate As Double)
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Dim aPriceProvider As New PRICEPROVIDERSLib.BatchPriceInfo
    Dim aBatchPriceProvider As PRICEPROVIDERSLib.IBatchPriceProvider
    
    Set aPT = aPriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    aPriceProvider.Connect
    Set aBatchPriceProvider = aPriceProvider
    aBatchPriceProvider.SetRegionRate sRegion, dRate
    aPriceProvider.Disconnect
    Set aPriceProvider = Nothing
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
  
    If m_dRate > 0# Then
        g_Params.ExchangeRate = m_dRate
        SetPPRegionRate CAN_REGION, m_dRate
        Dim aXMLLoader As ETSXMLParamsLib.XMLLoader
        Set aXMLLoader = New ETSXMLParamsLib.XMLLoader
        If (Not aXMLLoader Is Nothing) Then
            g_Params.SaveSettings
            aXMLLoader.Save MainXMLFilePath, g_aMainXMLParams 'Save ExchangeRate Immediatly
        End If
    Else
        MsgBox "Wrong Rate Value", vbInformation, "Rate Value"
        Exit Sub
    End If

    Unload Me
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub txtRate_Change()
    On Error Resume Next
    Dim dRate As Double
    
    Err.Clear
    dRate = CDbl(txtRate.Text)
    If Err.Number <> 0 Then
        txtRate.Text = CStr(m_dRate)
        Exit Sub
    End If
    m_dRate = dRate
    
End Sub
