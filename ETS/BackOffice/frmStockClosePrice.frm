VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmStockClosePrice 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Stock Close Price"
   ClientHeight    =   2055
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   8985
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2055
   ScaleWidth      =   8985
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnSave 
      Caption         =   "Save"
      Enabled         =   0   'False
      Height          =   375
      Left            =   7440
      TabIndex        =   5
      Top             =   1320
      Width           =   1455
   End
   Begin VB.CheckBox chkCanadianOnly 
      Caption         =   "Only Canadian"
      Enabled         =   0   'False
      Height          =   255
      Left            =   7440
      TabIndex        =   4
      Top             =   480
      Value           =   1  'Checked
      Visible         =   0   'False
      Width           =   1455
   End
   Begin VB.TextBox txtRate 
      Enabled         =   0   'False
      Height          =   285
      Left            =   7440
      TabIndex        =   2
      Text            =   "1.0"
      Top             =   120
      Width           =   1455
   End
   Begin MSComctlLib.ProgressBar pbProgress 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   1800
      Width           =   8985
      _ExtentX        =   15849
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   1
   End
   Begin VB.CommandButton btnOk 
      Caption         =   "Load"
      Height          =   375
      Left            =   7440
      TabIndex        =   0
      Top             =   840
      Width           =   1455
   End
   Begin MSForms.ListBox lbLog 
      Height          =   1575
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   7215
      ScrollBars      =   3
      DisplayStyle    =   2
      Size            =   "12726;2778"
      MatchEntry      =   0
      FontHeight      =   165
      FontCharSet     =   0
      FontPitchAndFamily=   2
   End
End
Attribute VB_Name = "frmStockClosePrice"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents m_PriceProvider As PRICEPROVIDERSLib.StructureProviderEx
Attribute m_PriceProvider.VB_VarHelpID = -1
Private m_PriceColl As clsClosePriceColl
Private m_bConnected As Boolean
Private m_bConnectBroken As Boolean
Private m_bCanadianOnly As Boolean
Private m_nLoaded As Long

Public Function Execute() As Boolean
On Error GoTo Err
    
    Load Me
    Init
    LoadData
    Me.Show vbModal
    Unload Me
    Exit Function
    
Err:
    Debug.Print "Fail to execute window."
End Function

Private Sub RequestStock()
On Error GoTo Err

        Dim aPrc As clsClosePriceAtom

        For Each aPrc In m_PriceColl
            
            If Not m_bConnected Then
                m_PriceProvider.Connect
            End If
            
            m_bConnectBroken = False
            aPrc.IsUpdated = False
                             
            Dim sParams As PRICEPROVIDERSLib.StockParams
            
            sParams.Stock = aPrc.Symbol
            If (Len(aPrc.Exchange) <> 0) Then
                sParams.Exchange = aPrc.Exchange
            Else
                sParams.Exchange = ""
            End If
                
            m_PriceProvider.RequestStock sParams
            DoEvents
            Sleep 1
            
            
        Next
        Exit Sub
        
Err:
        gCmn.ErrorHandler "Fail to RequestStock: [RequestStock()]"
End Sub

Private Sub SaveContract()
On Error GoTo Err

        Dim aPrc As clsClosePriceAtom

        For Each aPrc In m_PriceColl
             If (aPrc.IsUpdated) Then
                Dim nRet As Integer
                nRet = gDBW.usp_Contract_ClosePrice_Save(aPrc.ContractID, aPrc.ClosePrice)
             End If
        Next
        Exit Sub
        
Err:
    gCmn.ErrorHandler "Fail to save contract price: [SaveContracta()]"
End Sub

Private Sub Init()

    'get memory for coll
    Set m_PriceColl = New clsClosePriceColl
    Debug.Assert Not m_PriceColl Is Nothing
    'set PP as connected
    m_bConnected = True
    'init PP
    PriceProviderReset
    'init global member variables
    m_bCanadianOnly = True
    m_nLoaded = 0
    ''''''''''''''''''''''''''''''
    ShowRate
    
End Sub
Private Sub PriceProviderReset()
On Error GoTo Err

    If g_StructureProvider Is Nothing Then
        Set g_StructureProvider = New PRICEPROVIDERSLib.StructureProviderEx
    End If
    
    Set m_PriceProvider = g_StructureProvider
    
    Dim Provider As PRICEPROVIDERSLib.IProvider
    Set Provider = m_PriceProvider
    If Not Provider Is Nothing Then
        Provider.Type = g_Params.PriceProviderType
    End If
    
    If Not m_bConnected Then
        m_PriceProvider.Connect
        m_bConnected = True
    End If

    Me.Refresh
    DoEvents

    Exit Sub
    
Err:
    gCmn.ErrorHandler "Fail to reset PriceProvider: [PriceProviderReset()]"
End Sub

Private Sub LoadData()
On Error GoTo Err
    
    Dim rs As ADODB.Recordset
    Dim aCPA As clsClosePriceAtom
    Dim sKey As String
   
    m_PriceColl.Clear
    Set rs = gDBW.usp_BoAllStocksGet
    While Not rs.EOF
        sKey = Trim$(gCmn.ReadStr(rs!sSymbol))
        If (m_PriceColl(sKey) Is Nothing And gCmn.ReadLng(rs!iContractType) = 2) Then
            Set aCPA = m_PriceColl.Add(sKey)
            aCPA.ContractID = gCmn.ReadLng(rs!iContractID)
            aCPA.Exchange = Trim$(UCase$(gCmn.ReadStr(rs!sExchange)))
            aCPA.Symbol = Trim$(gCmn.ReadStr(rs!sSymbol))
            aCPA.ContractType = gCmn.ReadLng(rs!iContractType)
            aCPA.IsUpdated = False
            Set aCPA = Nothing
        End If
        rs.MoveNext
    Wend
    
    Exit Sub
    
Err:
    gCmn.ErrorHandler "Fail to load contracts: [LoadData()]"
End Sub

Private Sub ResetProgress()
    pbProgress.Value = 0
End Sub

Private Sub btnOK_Click()
    
    SetAllControls (False)
    ResetProgress
    RequestStock
    
End Sub

Private Sub SetAllControls(bEnable As Boolean)
    chkCanadianOnly.Enabled = False
    txtRate.Enabled = False
    lbLog.Enabled = bEnable
    btnOk.Enabled = bEnable
    btnSave.Enabled = bEnable
End Sub

Private Sub ShowRate()
    txtRate.Text = g_Params.ExchangeRate
End Sub

Private Sub btnSave_Click()
    SaveContract
End Sub

Private Sub chkCanadianOnly_Click()
    m_bCanadianOnly = chkCanadianOnly.Value
End Sub

Private Sub Form_Unload(Cancel As Integer)
On Error Resume Next

    m_PriceProvider.CancelOptions
    Set m_PriceProvider = Nothing
    Set m_PriceColl = Nothing
    
End Sub

Private Sub ShowLog(sMsg As String)
On Error Resume Next

    lbLog.AddItem sMsg
    
End Sub

Private Sub IncProgress()
On Error Resume Next

    pbProgress.Max = m_PriceColl.Count
    pbProgress.Value = pbProgress.Value + 1
    
    m_nLoaded = m_nLoaded + 1
    If (m_PriceColl.Count = m_nLoaded) Then
        SetAllControls (True)
    End If
    
End Sub

Private Sub m_PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
On Error Resume Next
    
    
    If ErrorNumber = enNotConnected _
    Or ErrorNumber = enConnectionWasDisconnected Then m_bConnectBroken = True

    If ReqType = enRequestStock Then
        'ShowLog "Price provider error for " & Request.Stock & "." & Request.Exchange & " request: " & Description
        If ErrorNumber = enNoDataAvailableForSymbol Then
            'ShowLog "No data available for symbol " & Request.Stock & "." & Request.Exchange & " request."
        End If
        IncProgress
    End If

End Sub

Private Sub m_PriceProvider_OnStock(Params As PRICEPROVIDERSLib.StockParams, Results As PRICEPROVIDERSLib.StockResultsEx)
On Error GoTo Err

    Dim sKey As String
    Dim dClosePrice As Double
    
    dClosePrice = 0#
'    If (Results.CloseBid <> BAD_DOUBLE_VALUE) Then
'        dClosePrice = Results.CloseBid
'    End If
'    If (Results.CloseAsk <> BAD_DOUBLE_VALUE) Then
'        dClosePrice = dClosePrice + Results.CloseAsk
'    End If
'    dClosePrice = dClosePrice / 2#
    If (Results.ClosePrice <> BAD_DOUBLE_VALUE) Then
        dClosePrice = Results.ClosePrice
    End If
    
    
    sKey = Trim$(Params.Stock)
    
    If (m_bCanadianOnly) Then
        If (Results.Currency = "CAD") Then
            If (Not m_PriceColl(sKey) Is Nothing) Then
                m_PriceColl(sKey).IsUpdated = True
                m_PriceColl(sKey).ClosePrice = dClosePrice
                ShowLog "Loaded: " & Params.Stock & "." & Params.Exchange & ":" & " ClosePrice: " & dClosePrice
            End If
        End If
    Else
            If (Not m_PriceColl(sKey) Is Nothing) Then
                m_PriceColl(sKey).IsUpdated = True
                m_PriceColl(sKey).ClosePrice = dClosePrice
                ShowLog "Loaded: " & Params.Stock & "." & Params.Exchange & ":" & " ClosePrice: " & dClosePrice
            End If
    End If
    
    IncProgress
    
    Exit Sub
    
Err:
    ShowLog "OnStock event fail."
End Sub
