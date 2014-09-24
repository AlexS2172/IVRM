VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmProgress 
   BackColor       =   &H00E0E0E0&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Progress Bar Dialog"
   ClientHeight    =   1620
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4680
   ForeColor       =   &H80000008&
   Icon            =   "frmProgress.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1620
   ScaleWidth      =   4680
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.Timer tmrExecute 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   240
      Top             =   720
   End
   Begin ElladaFlatControls.FlatButton FlatButton1 
      Height          =   300
      Left            =   1800
      TabIndex        =   0
      Top             =   1080
      Width           =   855
      _ExtentX        =   1508
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
   Begin MSComctlLib.ProgressBar pbProgress 
      Height          =   195
      Left            =   240
      TabIndex        =   1
      Top             =   120
      Width           =   4215
      _ExtentX        =   7435
      _ExtentY        =   344
      _Version        =   393216
      BorderStyle     =   1
      Appearance      =   0
      Max             =   1000
   End
   Begin VB.Label lblStatus 
      Alignment       =   2  'Center
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      Caption         =   "Recalculating Vega Weights..."
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   9.75
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00FF0000&
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   600
      Width           =   4455
   End
End
Attribute VB_Name = "frmProgress"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Enum eCalcTypeEnum
    enRecalculateVegaWeights = 0
    enUpdateBasketIndexInfo
End Enum

Private m_enCalcType As eCalcTypeEnum
Private m_sData As String
Private m_nData As Long

Private bCancel As Boolean
Private bFinish As Boolean
Private ThreadFunc As Long

Public Sub Initialize(ByVal ProgressMax As Integer, Optional ByVal CalcType As eCalcTypeEnum = enRecalculateVegaWeights, _
                                                    Optional ByVal sData As String = "", Optional ByVal nData As Long = 0)
    On Error Resume Next
    
    m_enCalcType = CalcType
    m_sData = sData
    m_nData = nData
    
    pbProgress.Min = 0
    pbProgress.Max = ProgressMax
    tmrExecute.Enabled = True
    
    Me.Show vbModal
    
End Sub

Public Property Get IsCancel() As Boolean
    On Error Resume Next
    
    IsCancel = bCancel
End Property

Public Property Let SetProgress(NewValue As Integer)
    On Error Resume Next
    pbProgress.Value = NewValue
    
    DoEvents
    Sleep 0
    
End Property

Public Property Let SetStatus(NewValue As String)
    On Error Resume Next
    lblStatus.Caption = NewValue
End Property

Public Property Let SetCaption(NewValue As String)
    On Error Resume Next
    Me.Caption = NewValue
End Property

Public Property Let SetMaxProgress(NewValue As String)
    On Error Resume Next
    pbProgress.Min = 0
    pbProgress.Max = NewValue
    pbProgress.Value = 0
End Property

Public Function SetDefaultFont()
    On Error Resume Next
    lblStatus.Font.Size = 8
    lblStatus.ForeColor = &H0&
    lblStatus.FontBold = False
End Function

Public Sub FinishProgress()
    On Error Resume Next
    
    If bCancel Then
        pbProgress.Value = pbProgress.Min
    Else
        SetStatus = "Operation completed"
        pbProgress.Value = pbProgress.Max
    End If
    
    bFinish = True
    FlatButton1.Caption = "Close"
    FlatButton1.Enabled = True
End Sub

Private Sub FlatButton1_Click()
    On Error Resume Next
    
    If Not bFinish Then
        SetStatus = "Operation canceled"
        bCancel = True
        pbProgress.Value = pbProgress.Min
        FlatButton1.Caption = "Close"
        FlatButton1.Enabled = False
    Else
        Unload Me
    End If
    
End Sub

Private Sub Form_Load()
    On Error Resume Next
    
    bCancel = False
    bFinish = False
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    
    Cancel = Not bFinish
End Sub

Private Sub tmrExecute_Timer()
    tmrExecute.Enabled = False
    '//////////////
    Select Case m_enCalcType
        Case enRecalculateVegaWeights
    RecalculateVegaWeights Me
        Case enUpdateBasketIndexInfo
            UpdateBasketIndexInfo
    End Select
    '////////////
    Exit Sub
Herr:
    ShowError

End Sub

Private Sub UpdateBasketIndexInfo()
    On Error GoTo EH
    Dim sIndex$, sStock$, dWeight#, dPriceIndex#, dPriceStock#
    Dim nIndexId&, nStockID&, sMsg$, nMsHint&, i&
    Dim nContractID As Long
    
    sIndex = m_sData
    nContractID = m_nData
    
    Dim rs As ADODB.Recordset
    Set rs = gDBW.usp_IndexDefinition_LoadFromIV(sIndex)
    If Not rs.RecordCount > 0 Then
        Me.SetProgress = 1
        Me.FinishProgress
        Me.SetStatus = "Error: No data available"
        Exit Sub
    End If
    
    Me.SetProgress = 1
    
    gDBW.usp_BoIndexWithBasketInfo_ClearWeights nContractID
    
    If Me.IsCancel Then
        Me.FinishProgress
        Exit Sub
    End If
    Me.SetMaxProgress = rs.RecordCount
    Me.SetStatus = "Save index composition"
    
    While Not rs.EOF
        dPriceIndex = DBRead(rs!Index_close_price, 0)
        dPriceStock = DBRead(rs!Component_close_price, 0)
        dWeight = DBRead(rs!Component_weight, 0)
        sStock = DBRead(rs!Component_symbol, "")
        
        nMsHint = gDBW.usp_BoIndexBasketWeight_Save(nIndexId, sIndex, nStockID, sStock, dWeight, 1, 1, 0, sMsg)
'        gDBW.usp_ContractPrice_Save Null, nStockID, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, dPriceStock
        
        DoEvents
        
        nIndexId = 0
        nStockID = 0
        
        rs.MoveNext
        
        i = i + 1
        Me.SetProgress = i
        If Me.IsCancel Then
            Me.FinishProgress
            Exit Sub
        End If
    Wend
    
'    gDBW.usp_ContractPrice_Save Null, nContractID, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, dPriceIndex
    
    Me.FinishProgress
    
    Exit Sub
EH:
    ShowError "Can't update composition"
    Me.FinishProgress
    Me.SetStatus = "Error during update"
End Sub
