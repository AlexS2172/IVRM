VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmMarketValues 
   Appearance      =   0  'Flat
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Market Values"
   ClientHeight    =   2715
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   5520
   Icon            =   "frmMarketValues.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2715
   ScaleWidth      =   5520
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   2055
      Left            =   120
      ScaleHeight     =   2025
      ScaleWidth      =   5265
      TabIndex        =   5
      Top             =   120
      Width           =   5295
      Begin VB.TextBox txt5ExpVolume 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   3720
         TabIndex        =   13
         Top             =   1560
         Width           =   1335
      End
      Begin VB.TextBox txt10dVolume 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   1320
         TabIndex        =   11
         Top             =   1560
         Width           =   1335
      End
      Begin VB.TextBox txtTheo 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   3720
         TabIndex        =   9
         Top             =   1080
         Width           =   1335
      End
      Begin VB.TextBox txtName 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   1320
         TabIndex        =   1
         Tag             =   "text"
         Top             =   600
         Width           =   3735
      End
      Begin VB.TextBox txtClose 
         Alignment       =   1  'Right Justify
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   1320
         TabIndex        =   2
         Top             =   1080
         Width           =   1335
      End
      Begin VB.Label lbl5ExpVolume 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "5 Expiration Volume :"
         Height          =   375
         Left            =   2760
         TabIndex        =   14
         Top             =   1440
         Width           =   855
      End
      Begin VB.Label lbl10dVolume 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "10 Day Volume :"
         Height          =   375
         Left            =   360
         TabIndex        =   12
         Top             =   1440
         Width           =   855
      End
      Begin VB.Label lblTheo 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Theo :"
         Height          =   255
         Left            =   3000
         TabIndex        =   10
         Top             =   1080
         Width           =   615
      End
      Begin MSForms.ComboBox cboContracts 
         Height          =   300
         Left            =   1320
         TabIndex        =   0
         Top             =   120
         Width           =   1335
         VariousPropertyBits=   679495707
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "2355;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lblContracts 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Contracts :"
         Height          =   255
         Left            =   120
         TabIndex        =   8
         Top             =   240
         Width           =   1095
      End
      Begin VB.Label lblName 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Name :"
         Height          =   255
         Left            =   120
         TabIndex        =   7
         Top             =   600
         Width           =   1095
      End
      Begin VB.Label lblClose 
         Alignment       =   1  'Right Justify
         BackStyle       =   0  'Transparent
         Caption         =   "Close :"
         Height          =   255
         Left            =   360
         TabIndex        =   6
         Top             =   1080
         Width           =   855
      End
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Default         =   -1  'True
      Height          =   300
      Left            =   2400
      TabIndex        =   3
      Top             =   2280
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
      Cancel          =   -1  'True
      Height          =   300
      Left            =   3840
      TabIndex        =   4
      Top             =   2280
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
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   120
      Picture         =   "frmMarketValues.frx":000C
      Top             =   2280
      Visible         =   0   'False
      Width           =   240
   End
End
Attribute VB_Name = "frmMarketValues"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private miContractID As Long
Private miContractTypeID As Long
Private msSymbol As String
Private msName As String
Private mfTheo As Double
Private mfClose As Double
Private miVolume10Day As Long
Private miVolume5Expiration As Long

Private mbLoading As Boolean

Private m_bChanged As Boolean
Private m_bOK As Boolean

' executing the form
Public Function Execute(ByVal sSymbol As String, ByVal iContractTypeID As Long) As Boolean
On Error GoTo ErrHnd:
    msSymbol = sSymbol
    m_bOK = False
    Load Me
    
    m_bChanged = False
    ' if symbol is working only with it
    If msSymbol > "" Then
        cboContracts.Enabled = False
        txtName.Enabled = False
    End If
    
    ' loading data for a symbol
    LoadData msSymbol, iContractTypeID
    
    mbLoading = True
    cboContracts.Text = msSymbol
    mbLoading = False
    
    Me.Show vbModal
    Execute = m_bOK
    Unload Me
Exit Function
ErrHnd:
    ShowError
End Function

Private Sub btnOK_Click()
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bOK = SaveData
    Screen.MousePointer = vbDefault
    If m_bOK Then Hide
    Exit Sub
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    imgSave.Visible = False
    imgSave.ToolTipText = "Data has been changed"
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Get Changed() As Boolean
    Changed = m_bChanged
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Let Changed(ByVal vNewValue As Boolean)
    m_bChanged = vNewValue
    imgSave.Visible = vNewValue
End Property

Private Function ValidateData() As Boolean
Dim ctl As Control
On Error GoTo ErrHnd
    If Not ParseValue(txtClose, mfClose, vbDouble) Then
        txtClose.SetFocus
        Exit Function
    End If
   
    If miContractTypeID < 3 Then
        If Not ParseValue(txt10dVolume, miVolume10Day, vbLong) Then
            txt10dVolume.SetFocus
            Exit Function
        End If
        If Not ParseValue(txt5ExpVolume, miVolume5Expiration, vbLong) Then
            txt5ExpVolume.SetFocus
            Exit Function
        End If
    End If
           
    If mfClose < 0 Then
        ShowWarning "Negative Close Price"
        txtClose.SetFocus
        Exit Function
    End If
    
    If miContractTypeID < 3 Then
        If miVolume10Day < 0 Then
            ShowWarning "Negative Average 10 Previous Trade Days Volume"
            txt10dVolume.SetFocus
            Exit Function
        End If
        
        If miVolume5Expiration < 0 Then
            ShowWarning "Negative Average 5 Previous Expiration Days Volume"
            txt5ExpVolume.SetFocus
            Exit Function
        End If
    End If

    ValidateData = True
Exit Function
ErrHnd:
    ShowError
End Function

' loading data for selected symbol
Private Function LoadData(ByVal sSymbol As String, Optional ByVal iContractTypeID As Long = 0) As Boolean
Dim rst As ADODB.Recordset

    ' checking data changes
    If Changed() Then
        If gCmn.MyMsgBox("Do you want to save chahges?", vbYesNo) = vbYes Then
            If Not SaveData Then Exit Function
        End If
    End If
    
    ' starting loading data
    mbLoading = True
    
    miContractID = 0
    miContractTypeID = 0
    msName = ""
    mfTheo = 0
    mfClose = 0
    
    txtName.Enabled = False
    txtClose.Enabled = False
    
    ' reading data from database
    Set rst = gDBW.usp_BoContractBySymbol_Get(Trim(sSymbol))
    
    ' if data is loaded then we eneble controls and fill them
    ' otherwise we disable controls
    If rst.RecordCount > 0 Then
        rst.MoveFirst
        ' !!! Select the correct one from the list !!!
        Do While Not rst.EOF
            ' Use only the given Exchange
            miContractID = gCmn.ReadLng(rst!iContractID)
            miContractTypeID = gCmn.ReadLng(rst!iContractTypeID)
            msSymbol = gCmn.ReadStr(rst!vcSymbol)
            msName = gCmn.ReadStr(rst!vcContractName)
            mfTheo = gCmn.ReadDbl(rst!fPriceTheo)
            mfClose = gCmn.ReadDbl(rst!fPriceClose)
            
            If iContractTypeID <> 0 And iContractTypeID <> miContractTypeID Then GoTo Skip
            
            If miContractTypeID < 3 Then
                miVolume10Day = gCmn.ReadLng(rst!iVolume10Day)
                miVolume5Expiration = gCmn.ReadLng(rst!iVolume5Expiration)
            End If
            GoTo Found
Skip:
            rst.MoveNext
        Loop
    End If
    Exit Function

Found:
On Error Resume Next
        
    ' displaying data
    txtName.Text = msName
    
    txtClose.Text = gCmn.FmtWideDbl(mfClose)
    txtTheo.Text = gCmn.FmtWideDbl(mfTheo)
    txt10dVolume.Text = gCmn.FmtLng(miVolume10Day)
    txt5ExpVolume.Text = gCmn.FmtLng(miVolume5Expiration)
        
    If miContractTypeID > 2 Then
        txtTheo.Visible = CBool(miContractTypeID <> 4)
        lblTheo.Visible = CBool(miContractTypeID <> 4)
        txt10dVolume.Visible = False
        lbl10dVolume.Visible = False
        txt5ExpVolume.Visible = False
        lbl5ExpVolume.Visible = False
    Else
        txtTheo.Visible = False
        lblTheo.Visible = False
        txt10dVolume.Visible = True
        lbl10dVolume.Visible = True
        txt5ExpVolume.Visible = True
        lbl5ExpVolume.Visible = True
    End If
    txtName.Enabled = False
    txtTheo.Enabled = False
    txtClose.Enabled = True
        
    txtName.Enabled = txtName.Enabled And False
   
    Changed = False

    'end of data loading
    mbLoading = False

End Function

' saving data for selected contract
Private Function SaveData() As Boolean
    If Not Changed Then
        SaveData = True
        Exit Function
    End If
    If Not ValidateData Then
        SaveData = False
        Exit Function
    End If
    If miContractTypeID > 2 Then
        If gDBW.usp_Contract_Save(miContractID, msSymbol, _
            msName, 0, 0, 0, 0, 0, _
            0, mfClose, 0, 0, 0, 0) Then
            Changed = False
            SaveData = True
        End If
    Else
        If gDBW.usp_Contract_Save(miContractID, msSymbol, _
            msName, 0, 0, 0, 0, 0, _
            0, mfClose, 0, 0, 0, 0, miVolume10Day, miVolume5Expiration) Then
            Changed = False
            SaveData = True
        End If
    End If
End Function

Private Sub cboContracts_Change()
On Error Resume Next
    If Not mbLoading Then
        LoadData cboContracts.Text
        If cboContracts.MatchFound Then
            cboContracts.ForeColor = &H0
        Else
            cboContracts.ForeColor = &HA0A0A0
        End If
    End If
End Sub

Private Sub cboContracts_Validate(Cancel As Boolean)
On Error Resume Next
    If Not cboContracts.MatchFound Then
        LoadData ""
    End If
End Sub


' validating values
Private Function ParseValue(ByRef ctl As Control, ByRef Value As Variant, ByVal iVarType As Long) As Boolean
Dim v As Variant
On Error GoTo ErrHnd
    If ctl.Text = GSTR_NA Then
        GoTo Ext
    End If
    If ctl.Text = "" Then
        GoTo Ext
    End If
    If Not IsNumeric(ctl.Text) Then
        ShowWarning "Value " & ctl.Text & " is not a correct number"
        Exit Function
    End If
    If ctl.Visible Then
        If iVarType = vbLong Then
            v = gCmn.ReadLng(ctl.Text)
            ctl.Text = gCmn.FmtLng(v)
        Else
            v = gCmn.ReadDbl(ctl.Text)
            ctl.Text = gCmn.FmtDbl(v)
        End If
    End If
Ext:
    Value = v
    ParseValue = True
Exit Function
ErrHnd:
    ShowWarning "Value " & ctl.Text & " is not a correct number"
    Err.Raise Err.Number
End Function




'*****************************************************
'       controls events handlers
'*****************************************************


Private Sub txtName_Change()
On Error Resume Next
    If Not mbLoading Then Changed = True
End Sub

Private Sub txtName_GotFocus()
On Error Resume Next
    gCmn.SelectText txtName
End Sub



Private Sub txtClose_Change()
On Error Resume Next
    If Not mbLoading Then Changed = True
End Sub

Private Sub txtClose_GotFocus()
On Error Resume Next
    gCmn.SelectText txtClose
End Sub

Private Sub txtClose_Validate(Cancel As Boolean)
On Error Resume Next
    ParseValue txtClose, mfClose, vbDouble
End Sub

Private Sub txt10dVolume_Change()
On Error Resume Next
    If Not mbLoading Then Changed = True
End Sub

Private Sub txt10dVolume_GotFocus()
On Error Resume Next
    gCmn.SelectText txt10dVolume
End Sub

Private Sub txt10dVolume_Validate(Cancel As Boolean)
On Error Resume Next
    ParseValue txt10dVolume, miVolume10Day, vbLong
End Sub

Private Sub txt5ExpVolume_Change()
On Error Resume Next
    If Not mbLoading Then Changed = True
End Sub

Private Sub txt5ExpVolume_GotFocus()
On Error Resume Next
    gCmn.SelectText txt5ExpVolume
End Sub

Private Sub txt5ExpVolume_Validate(Cancel As Boolean)
On Error Resume Next
    ParseValue txt5ExpVolume, miVolume5Expiration, vbLong
End Sub


