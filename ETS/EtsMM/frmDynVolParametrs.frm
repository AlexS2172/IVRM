VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmDynVolParametrs 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Dynamic Volatility Settings"
   ClientHeight    =   2190
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7440
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2190
   ScaleWidth      =   7440
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox m_FitDiff 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   315
      Left            =   6480
      TabIndex        =   11
      Text            =   "Text2"
      Top             =   810
      Width           =   615
   End
   Begin VB.CheckBox m_Check_Fit_Only 
      Alignment       =   1  'Right Justify
      Caption         =   "Fit only if % difference > than"
      Height          =   255
      Left            =   2640
      TabIndex        =   10
      Top             =   840
      Width           =   3735
   End
   Begin VB.CommandButton m_btCancel 
      Caption         =   "Cancel"
      Height          =   255
      Left            =   6000
      TabIndex        =   2
      Top             =   1800
      Width           =   1335
   End
   Begin VB.CommandButton m_btSave 
      Caption         =   "Ok"
      Height          =   255
      Left            =   4560
      TabIndex        =   1
      Top             =   1800
      Width           =   1335
   End
   Begin VB.TextBox m_AutoFitTime 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   "0"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   1
      EndProperty
      Height          =   315
      Left            =   6480
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   480
      Width           =   615
   End
   Begin VB.Frame Frame1 
      Caption         =   "Market IV"
      Height          =   1575
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   2175
      Begin MSForms.OptionButton m_Btn4 
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   1200
         Width           =   1215
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   5
         Size            =   "2143;450"
         Value           =   "0"
         Caption         =   "Put IV"
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.OptionButton m_Btn3 
         Height          =   255
         Left            =   120
         TabIndex        =   6
         Top             =   840
         Width           =   1095
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   5
         Size            =   "1931;450"
         Value           =   "0"
         Caption         =   "Call IV"
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.OptionButton m_Btn2 
         Height          =   375
         Left            =   120
         TabIndex        =   5
         Top             =   480
         Width           =   1815
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   5
         Size            =   "3201;661"
         Value           =   "0"
         Caption         =   "Average IV"
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.OptionButton m_Btn1 
         Height          =   255
         Left            =   120
         TabIndex        =   4
         Top             =   240
         Width           =   1095
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   5
         Size            =   "1931;450"
         Value           =   "0"
         Caption         =   "OTM IV"
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "Auto Fit Rules"
      Height          =   1575
      Left            =   2400
      TabIndex        =   8
      Top             =   120
      Width           =   4935
      Begin VB.CheckBox m_Check_Auto_Fit 
         Alignment       =   1  'Right Justify
         Caption         =   "Auto fit Theo Vol to Market IV by timer (minutes):"
         Height          =   255
         Left            =   240
         TabIndex        =   9
         Top             =   360
         Width           =   3735
      End
   End
End
Attribute VB_Name = "frmDynVolParametrs"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Created By Artem Shilyaev
'Egar T

Option Explicit
Private m_bIsOpened As Boolean
Private m_frmOwner As Form
Private m_bDirty As Boolean
Private m_bPriceProfileDirty As Boolean
Private m_bInitializing As Boolean


Public Sub Execute(ByRef frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then
        Me.SetFocus
        Exit Sub
    End If
    
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
   ' If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    
    Load Me
    LoadData
    m_bDirty = False
    m_bPriceProfileDirty = False

    Me.Show vbModeless, m_frmOwner
    
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open parameters dialog."
    On Error Resume Next
    Unload Me
End Sub

Private Sub LoadData()
    On Error Resume Next
    m_AutoFitTime.Text = ""
    m_FitDiff.Text = ""
    m_bInitializing = True
        
    Dim rs As ADODB.Recordset
    Set rs = gDBW.usp_DataInfo_Get(, "IV:Use Market IV")
    If Not rs Is Nothing Then
        If Not rs.EOF Then
            Dim strRead As String
            strRead = ReadStr(rs!vcKeyValue)
            Select Case strRead
            Case "1"
                m_Btn1.Value = True
            Case "2"
                m_Btn2.Value = True
            Case "3"
                m_Btn3.Value = True
            Case "4"
                m_Btn4.Value = True
            End Select
        End If
   End If

   Set rs = gDBW.usp_DataInfo_Get(, "IV:Auto Fit IV")
   If Not rs Is Nothing Then
        If Not rs.EOF Then
            strRead = ReadStr(rs!vcKeyValue)
            If strRead = "0" Then
                EnableAutoFit False
            Else
                EnableAutoFit True
                m_Check_Auto_Fit.Value = 1
                m_AutoFitTime.Text = strRead
            End If
            
        End If
   End If

   Set rs = gDBW.usp_DataInfo_Get(, "IV:Fit difference only")
   If Not rs Is Nothing Then
        If Not rs.EOF Then
            strRead = ReadStr(rs!vcKeyValue)
            If strRead = "0" Then
                EnableFitOnly False
            Else
                EnableFitOnly True
                m_Check_Fit_Only.Value = 1
                m_FitDiff.Text = strRead
            End If
            
        End If
   End If

    
    m_bInitializing = False
End Sub
Private Sub EnableAutoFit(vEnable As Boolean)
    m_AutoFitTime.Enabled = vEnable
    m_Check_Fit_Only.Enabled = vEnable
    m_FitDiff.Enabled = vEnable
End Sub
Private Sub EnableFitOnly(vEnable As Boolean)
    m_FitDiff.Enabled = vEnable
End Sub

Private Sub m_Bt2_Click()

End Sub

Private Sub Form_Unload(Cancel As Integer)
    m_bIsOpened = False
End Sub

Private Sub m_btCancel_Click()
    Unload Me
End Sub

Private Sub m_btSave_Click()
    SaveData
    Unload Me
End Sub
Private Sub SaveData()
    Dim strValuy As String
    If m_Btn1.Value = True Then strValuy = "1"
    If m_Btn2.Value = True Then strValuy = "2"
    If m_Btn3.Value = True Then strValuy = "3"
    If m_Btn4.Value = True Then strValuy = "4"
    
    Dim IDinUpdate As New MSGSTRUCTLib.UpdateUndIVSettings
    'Dim IDinUpdate As MSGSTRUCTLib.IUpdateUndIVSettings
'    Set IDinUpdate = New MSGSTRUCTLib.IUpdateUndIVSettings
    
    gDBW.usp_DataInfo_Put 13, "IV:Use Market IV", strValuy
    
    IDinUpdate.UseMarketIV = strValuy
    
    Dim vrData As Variant
    
    If m_Check_Auto_Fit.Value = 1 Then
        vrData = m_AutoFitTime.Text
    Else
        vrData = 0
    End If
    
    gDBW.usp_DataInfo_Put 14, "IV:Auto Fit IV", vrData
    
    IDinUpdate.AutoFitIV = vrData
    If m_Check_Fit_Only.Value = 1 Then
        vrData = m_FitDiff.Text
    Else
        vrData = 0
    End If
    
    gDBW.usp_DataInfo_Put 15, "IV:Fit difference only", vrData
    IDinUpdate.FitOnly = vrData
    vrData = g_VolaSource.Set_IV_Settings(IDinUpdate)
    'g_VolaSource.SetIVSettings (IDinUpdate)
End Sub
Private Sub m_Check_Auto_Fit_Click()
    EnableAutoFit m_Check_Auto_Fit.Value
End Sub


Private Sub m_Check_Fit_Only_Click()
    EnableFitOnly m_Check_Fit_Only.Value
End Sub
