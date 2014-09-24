VERSION 5.00
Begin VB.Form frmConnections 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Connections Settings (Loading...)"
   ClientHeight    =   6465
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   8385
   Icon            =   "frmConnections.frx":0000
   LinkTopic       =   "Form1"
   LockControls    =   -1  'True
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6465
   ScaleWidth      =   8385
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.Frame fraTN 
      Caption         =   " Trades Messages "
      Height          =   1185
      Left            =   120
      TabIndex        =   53
      Top             =   4740
      Width           =   8145
      Begin VB.TextBox txtTNSecondaryIP 
         Height          =   315
         Left            =   5220
         TabIndex        =   28
         Top             =   690
         Width           =   1395
      End
      Begin VB.PictureBox txtTNPort 
         BackColor       =   &H000000FF&
         Height          =   1000
         Left            =   0
         ScaleHeight     =   945
         ScaleWidth      =   945
         TabIndex        =   29
         Top             =   0
         Width           =   1000
      End
      Begin VB.TextBox txtTNPrimaryIP 
         Height          =   315
         Left            =   5220
         TabIndex        =   27
         Top             =   300
         Width           =   1395
      End
      Begin VB.TextBox txtTNPassword 
         Height          =   315
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   26
         Top             =   690
         Width           =   2775
      End
      Begin VB.TextBox txtTNAccount 
         Height          =   315
         Left            =   1080
         TabIndex        =   25
         Top             =   300
         Width           =   2775
      End
      Begin VB.Label lblTNSecondaryIP 
         Alignment       =   1  'Right Justify
         Caption         =   "Secondary IP :"
         Height          =   255
         Left            =   3900
         TabIndex        =   58
         Top             =   720
         Width           =   1215
      End
      Begin VB.Label lblTNPrimaryIP 
         Alignment       =   1  'Right Justify
         Caption         =   "Primary IP :"
         Height          =   255
         Left            =   4260
         TabIndex        =   57
         Top             =   330
         Width           =   855
      End
      Begin VB.Label lblTNPort 
         Alignment       =   1  'Right Justify
         Caption         =   "Port :"
         Height          =   255
         Left            =   6660
         TabIndex        =   56
         Top             =   540
         Width           =   495
      End
      Begin VB.Label lblTNPassword 
         Alignment       =   1  'Right Justify
         Caption         =   "Password :"
         Height          =   255
         Left            =   120
         TabIndex        =   55
         Top             =   720
         Width           =   855
      End
      Begin VB.Label lblTNAccount 
         Alignment       =   1  'Right Justify
         Caption         =   "Account :"
         Height          =   255
         Left            =   120
         TabIndex        =   54
         Top             =   330
         Width           =   855
      End
   End
   Begin VB.Frame fraVS 
      Caption         =   " Volatility Source "
      Height          =   825
      Left            =   4260
      TabIndex        =   51
      Top             =   120
      Width           =   4005
      Begin VB.ComboBox cboVSType 
         Height          =   315
         ItemData        =   "frmConnections.frx":000C
         Left            =   720
         List            =   "frmConnections.frx":000E
         Sorted          =   -1  'True
         Style           =   2  'Dropdown List
         TabIndex        =   2
         Top             =   300
         Width           =   3135
      End
      Begin VB.Label lblVSType 
         Alignment       =   1  'Right Justify
         Caption         =   "Type :"
         Height          =   255
         Left            =   120
         TabIndex        =   52
         Top             =   330
         Width           =   495
      End
   End
   Begin VB.Timer tmrLoad 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   4980
      Top             =   6060
   End
   Begin VB.Frame fraIV 
      Caption         =   " IVolatility.com "
      Height          =   3645
      Left            =   4260
      TabIndex        =   42
      Top             =   1020
      Width           =   4005
      Begin VB.TextBox txtIVAddr 
         Height          =   315
         Left            =   1080
         TabIndex        =   14
         Top             =   300
         Width           =   2775
      End
      Begin VB.TextBox txtIVAccount 
         Height          =   315
         Left            =   1080
         TabIndex        =   15
         Top             =   690
         Width           =   2775
      End
      Begin VB.TextBox txtIVPwd 
         Height          =   315
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   16
         Top             =   1080
         Width           =   2775
      End
      Begin VB.OptionButton optIVProxyNone 
         Caption         =   "None"
         Height          =   255
         Left            =   1080
         TabIndex        =   17
         Top             =   1560
         Width           =   735
      End
      Begin VB.OptionButton optIVProxySystem 
         Caption         =   "System"
         Height          =   255
         Left            =   1920
         TabIndex        =   18
         Top             =   1560
         Width           =   930
      End
      Begin VB.OptionButton optIVProxyCustom 
         Caption         =   "Custom"
         Height          =   255
         Left            =   2940
         TabIndex        =   19
         Top             =   1560
         Width           =   975
      End
      Begin VB.TextBox txtIVProxyLogin 
         Height          =   315
         Left            =   1080
         TabIndex        =   23
         Top             =   2760
         Width           =   2775
      End
      Begin VB.TextBox txtIVProxyPwd 
         Height          =   315
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   24
         Top             =   3150
         Width           =   2775
      End
      Begin VB.TextBox txtIVProxyAddr 
         Height          =   315
         Left            =   1080
         TabIndex        =   20
         Top             =   1920
         Width           =   1335
      End
      Begin VB.TextBox txtIVProxyPort 
         Height          =   315
         Left            =   3120
         TabIndex        =   21
         Top             =   1920
         Width           =   735
      End
      Begin VB.CheckBox chkIVProxyAuth 
         Caption         =   "Proxy Authentication"
         Height          =   255
         Left            =   1080
         TabIndex        =   22
         Top             =   2400
         Width           =   2055
      End
      Begin VB.Label lblIVAddr 
         Alignment       =   1  'Right Justify
         Caption         =   "Address :"
         Height          =   255
         Left            =   120
         TabIndex        =   50
         Top             =   330
         Width           =   855
      End
      Begin VB.Label lblIVAccount 
         Alignment       =   1  'Right Justify
         Caption         =   "Account :"
         Height          =   255
         Left            =   120
         TabIndex        =   49
         Top             =   720
         Width           =   855
      End
      Begin VB.Label lblIVPwd 
         Alignment       =   1  'Right Justify
         Caption         =   "Password :"
         Height          =   255
         Left            =   120
         TabIndex        =   48
         Top             =   1110
         Width           =   855
      End
      Begin VB.Label lblIVProxy 
         Alignment       =   1  'Right Justify
         Caption         =   "Use Proxy :"
         Height          =   255
         Left            =   120
         TabIndex        =   47
         Top             =   1560
         Width           =   855
      End
      Begin VB.Label lblIVProxyLogin 
         Alignment       =   1  'Right Justify
         Caption         =   "Login :"
         Height          =   255
         Left            =   120
         TabIndex        =   46
         Top             =   2790
         Width           =   855
      End
      Begin VB.Label lblIVProxyPwd 
         Alignment       =   1  'Right Justify
         Caption         =   "Password :"
         Height          =   255
         Left            =   120
         TabIndex        =   45
         Top             =   3180
         Width           =   855
      End
      Begin VB.Label lblIVProxyPort 
         Alignment       =   1  'Right Justify
         Caption         =   "Port :"
         Height          =   255
         Left            =   2520
         TabIndex        =   44
         Top             =   1950
         Width           =   495
      End
      Begin VB.Label lblIVProxyAddr 
         Alignment       =   1  'Right Justify
         Caption         =   "Address :"
         Height          =   255
         Left            =   120
         TabIndex        =   43
         Top             =   1950
         Width           =   855
      End
   End
   Begin VB.Frame fraDB 
      Caption         =   " Database "
      Height          =   825
      Left            =   120
      TabIndex        =   39
      Top             =   120
      Width           =   4005
      Begin VB.CommandButton btnDB 
         Caption         =   "Setup..."
         Height          =   315
         Left            =   2880
         TabIndex        =   1
         Top             =   300
         Width           =   975
      End
      Begin VB.TextBox txtDatabase 
         BackColor       =   &H8000000F&
         Height          =   315
         Left            =   165
         Locked          =   -1  'True
         TabIndex        =   0
         Top             =   300
         Width           =   2670
      End
   End
   Begin VB.Frame fraPP 
      Caption         =   " Price Provider "
      Height          =   3645
      Left            =   120
      TabIndex        =   32
      Top             =   1020
      Width           =   4005
      Begin VB.CheckBox chkPPProxyAuth 
         Caption         =   "Proxy Authentication"
         Height          =   255
         Left            =   1080
         TabIndex        =   11
         Top             =   2400
         Width           =   2055
      End
      Begin VB.TextBox txtPPProxyPort 
         Height          =   315
         Left            =   3120
         TabIndex        =   10
         Top             =   1920
         Width           =   735
      End
      Begin VB.TextBox txtPPProxyAddr 
         Height          =   315
         Left            =   1080
         TabIndex        =   9
         Top             =   1920
         Width           =   1335
      End
      Begin VB.TextBox txtPPProxyPwd 
         Height          =   315
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   13
         Top             =   3150
         Width           =   2775
      End
      Begin VB.TextBox txtPPProxyLogin 
         Height          =   315
         Left            =   1080
         TabIndex        =   12
         Top             =   2760
         Width           =   2775
      End
      Begin VB.OptionButton optPPProxyCustom 
         Caption         =   "Custom"
         Height          =   255
         Left            =   2940
         TabIndex        =   8
         Top             =   1560
         Width           =   975
      End
      Begin VB.OptionButton optPPProxySystem 
         Caption         =   "System"
         Height          =   255
         Left            =   1920
         TabIndex        =   7
         Top             =   1560
         Width           =   930
      End
      Begin VB.OptionButton optPPProxyNone 
         Caption         =   "None"
         Height          =   255
         Left            =   1080
         TabIndex        =   6
         Top             =   1560
         Width           =   735
      End
      Begin VB.TextBox txtPPPwd 
         Height          =   315
         IMEMode         =   3  'DISABLE
         Left            =   1080
         PasswordChar    =   "*"
         TabIndex        =   5
         Top             =   1080
         Width           =   2775
      End
      Begin VB.TextBox txtPPAccount 
         Height          =   315
         Left            =   1080
         TabIndex        =   4
         Top             =   690
         Width           =   2775
      End
      Begin VB.ComboBox cboPPType 
         Height          =   315
         ItemData        =   "frmConnections.frx":0010
         Left            =   1080
         List            =   "frmConnections.frx":0012
         Sorted          =   -1  'True
         Style           =   2  'Dropdown List
         TabIndex        =   3
         Top             =   300
         Width           =   2775
      End
      Begin VB.Label lblPPProxyAddr 
         Alignment       =   1  'Right Justify
         Caption         =   "Address :"
         Height          =   255
         Left            =   120
         TabIndex        =   41
         Top             =   1950
         Width           =   855
      End
      Begin VB.Label lblPPProxyPort 
         Alignment       =   1  'Right Justify
         Caption         =   "Port :"
         Height          =   255
         Left            =   2520
         TabIndex        =   40
         Top             =   1950
         Width           =   495
      End
      Begin VB.Label lblPPProxyPwd 
         Alignment       =   1  'Right Justify
         Caption         =   "Password :"
         Height          =   255
         Left            =   120
         TabIndex        =   38
         Top             =   3180
         Width           =   855
      End
      Begin VB.Label lblPPProxyLogin 
         Alignment       =   1  'Right Justify
         Caption         =   "Login :"
         Height          =   255
         Left            =   120
         TabIndex        =   37
         Top             =   2790
         Width           =   855
      End
      Begin VB.Label lblPPProxy 
         Alignment       =   1  'Right Justify
         Caption         =   "Use Proxy :"
         Height          =   255
         Left            =   120
         TabIndex        =   36
         Top             =   1560
         Width           =   855
      End
      Begin VB.Label lblPPPwd 
         Alignment       =   1  'Right Justify
         Caption         =   "Password :"
         Height          =   255
         Left            =   120
         TabIndex        =   35
         Top             =   1110
         Width           =   855
      End
      Begin VB.Label lblPPAccount 
         Alignment       =   1  'Right Justify
         Caption         =   "Account :"
         Height          =   255
         Left            =   120
         TabIndex        =   34
         Top             =   720
         Width           =   855
      End
      Begin VB.Label lblPPType 
         Alignment       =   1  'Right Justify
         Caption         =   "Type :"
         Height          =   255
         Left            =   120
         TabIndex        =   33
         Top             =   330
         Width           =   855
      End
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   7050
      TabIndex        =   31
      Top             =   6030
      Width           =   1215
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   5730
      TabIndex        =   30
      Top             =   6030
      Width           =   1215
   End
End
Attribute VB_Name = "frmConnections"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Public Event OnOK(ByVal bDBChanged As Boolean, ByVal bPPChanged As Boolean, ByVal bIVChanged As Boolean, ByVal bVSChanged As Boolean, ByVal bTNChanged As Boolean)
'
'Public Enum ConnectionsShowOptionsEnum
'    enCsoPriceProviderOnly
'    enCsoVolatilitySourceOnly
'    enCsoTntOnly
'    enCsoAll
'End Enum
'
'Private Type ProviderDataType
'    ID As Long
'    Name As String
'    UseLogin As Boolean
'    ProgID As String
'    Account As String
'    Password As String
'    UseProxy As PRICEPROVIDERSLib.ProviderUseProxyType
'    ProxyAddress As String
'    ProxyPort As Long
'    ProxyAuthType As PRICEPROVIDERSLib.ProviderProxyAuthType
'    ProxyLogin As String
'    ProxyPassword As String
'
'    Provider As PRICEPROVIDERSLib.IProviderData
'End Type
'
'Private Type IvDataType
'    Address As String
'    Account As String
'    Password As String
'    UseProxy As ETSMANAGERLib.UseProxyType
'    ProxyAddress As String
'    ProxyPort As Long
'    ProxyAuthType As ETSMANAGERLib.ProxyAuthentificationType
'    ProxyLogin As String
'    ProxyPassword As String
'End Type
'
'Private Type VolatilitySourceDataType
'    ID As Long
'    Name As String
'    ProgID As String
'End Type
'
'Private m_mbResult As VbMsgBoxResult
'
'Private m_bShowModal As Boolean
'Private m_frmOwner As Form
'Private m_bIsOpened As Boolean
'Private m_bInit As Boolean
'Private m_bLoading As Boolean
'Private m_bDBChanged As Boolean
'Private m_bPPChanged As Boolean
'Private m_bIVChanged As Boolean
'Private m_bVSChanged As Boolean
'Private m_bTNChanged As Boolean
'Private m_bUpdateCurSettings As Boolean
'Private m_enShowOptions As ConnectionsShowOptionsEnum
'
'Private m_ProviderData() As ProviderDataType
'Private m_nCurPPIndex As Long
'
'Private m_idtIvSettings As IvDataType
'Private m_sConnectionString As String
'
'Private m_VolatilitySourceData() As VolatilitySourceDataType
'Private m_nCurVSIndex As Long
'
'Private m_TntConnSettings As TNTConnector.ConnectionSettings
'
'Public Function Execute(ByVal bShowModal As Boolean, ByVal enShowOptions As ConnectionsShowOptionsEnum, _
'                        ByVal bUpdateCurSettings As Boolean, ByRef frmOwner As Form) As VbMsgBoxResult
'    On Error Resume Next
'    If m_bIsOpened Then
'        Me.SetFocus
'        Exit Function
'    End If
'
'    Execute = vbCancel
'
'    m_bShowModal = bShowModal
'    m_bDBChanged = False
'    m_bPPChanged = False
'    m_bIVChanged = False
'    m_bVSChanged = False
'    m_bTNChanged = False
'    m_nCurPPIndex = 0
'    m_nCurVSIndex = 0
'    m_bUpdateCurSettings = bUpdateCurSettings
'
'    m_enShowOptions = enShowOptions
'    If m_enShowOptions <> enCsoPriceProviderOnly _
'        And m_enShowOptions <> enCsoVolatilitySourceOnly _
'        And m_enShowOptions <> enCsoTntOnly Then m_enShowOptions = enCsoAll
'
'    If m_bShowModal Then
'        Screen.MousePointer = vbHourglass
'        Load Me
'        m_mbResult = vbCancel
'
'        m_bLoading = True
'
'        AdjustSize
'        UpdateButtons
'
'        tmrLoad.Enabled = True
'        Show vbModal, frmOwner
'        Unload Me
'
'        Execute = m_mbResult
'    Else
'        Screen.MousePointer = vbHourglass
'        Set m_frmOwner = frmOwner
'        If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
'
'        m_bIsOpened = True
'        m_bLoading = True
'
'        AdjustSize
'        UpdateButtons
'        Show vbModeless, frmOwner
'
'        tmrLoad.Enabled = True
'    End If
'End Function
'
'Private Sub AdjustSize()
'    On Error Resume Next
'    Dim dH!, dW!
'
'    If m_enShowOptions <> enCsoAll Then
'        dH = Height - ScaleHeight
'        dW = Width - ScaleWidth
'
'        Select Case m_enShowOptions
'            Case enCsoPriceProviderOnly
'                fraDB.Visible = False
'                fraVS.Visible = False
'                fraIV.Visible = False
'                fraTN.Visible = False
'
'                fraPP.Top = fraDB.Top
'                btnCancel.Top = fraPP.Top + fraPP.Height + 105
'                btnCancel.Left = fraPP.Left + fraPP.Width - btnCancel.Width
'
'            Case enCsoVolatilitySourceOnly
'                fraDB.Visible = False
'                fraIV.Visible = False
'                fraPP.Visible = False
'                fraTN.Visible = False
'
'                fraVS.Top = fraDB.Top
'                fraVS.Left = fraPP.Left
'                btnCancel.Top = fraVS.Top + fraVS.Height + 105
'                btnCancel.Left = fraVS.Left + fraVS.Width - btnCancel.Width
'
'            Case enCsoTntOnly
'                fraDB.Visible = False
'                fraIV.Visible = False
'                fraPP.Visible = False
'                fraVS.Visible = False
'
'                fraTN.Top = fraDB.Top
'                btnCancel.Top = fraTN.Top + fraTN.Height + 105
'                btnCancel.Left = fraTN.Left + fraTN.Width - btnCancel.Width
'        End Select
'
'        btnOK.Top = btnCancel.Top
'        btnOK.Left = btnCancel.Left - 105 - btnOK.Width
'
'        Height = btnCancel.Top + btnOK.Height + 105 + dH
'        Width = btnCancel.Width + btnCancel.Left + 120 + dW
'    End If
'End Sub
'
'Private Sub LoadData()
'    On Error Resume Next
'    Dim sErrMsg$
'
'    sErrMsg = "connections settings."
'    m_bInit = True
'
'    On Error GoTo EH
'    If m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoPriceProviderOnly Then
'        Dim i&, nCount&, bSucceed As Boolean
'
'        sErrMsg = "price providers settings."
'
'        Dim aProviders As New PRICEPROVIDERSLib.Providers
'        aProviders.Initialize
'
'        nCount = aProviders.Count
'
'        Erase m_ProviderData
'        cboPPType.Clear
'
'        If nCount > 0 Then
'            ReDim m_ProviderData(1 To nCount)
'            For i = 1 To nCount
'                bSucceed = FillProviderData(m_ProviderData(i), aProviders(i))
'                With m_ProviderData(i)
'                    If bSucceed Then AddPPInfoToList .Name, .ID, i
'                End With
'            Next
'        End If
'
'        If cboPPType.ListCount > 0 Then
'            If cboPPType.ListIndex < 0 Then
'                cboPPType.ListIndex = 0
'                m_nCurPPIndex = cboPPType.ItemData(cboPPType.ListIndex)
'                m_bPPChanged = True
'            End If
'        End If
'
'        'UpdatePPControls
'    End If
'
'    On Error GoTo EH
'    If m_enShowOptions = enCsoAll Then
'        Dim aIVSettings As ETSMANAGERLib.IVSettings
'        sErrMsg = "IVolatility.com settings."
'
'        Set aIVSettings = New ETSMANAGERLib.IVSettings
'
'        m_idtIvSettings.Address = aIVSettings.VolServerName
'        m_idtIvSettings.Account = aIVSettings.VolLogin
'        m_idtIvSettings.Password = aIVSettings.VolPassword
'        m_idtIvSettings.UseProxy = aIVSettings.UseProxy
'        m_idtIvSettings.ProxyAddress = aIVSettings.ProxyName
'        m_idtIvSettings.ProxyPort = aIVSettings.ProxyPort
'        m_idtIvSettings.ProxyAuthType = aIVSettings.ProxyAuthType
'        m_idtIvSettings.ProxyLogin = aIVSettings.ProxyLogin
'        m_idtIvSettings.ProxyPassword = aIVSettings.ProxyPassword
'
'        Set aIVSettings = Nothing
'        'UpdateIVControls
'    End If
'
'    If m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoVolatilitySourceOnly Then
'        Dim aVolaSources As New VolatilitySourcesLib.VolatilitySources, sName$, sFileName$, sVersion$, nID&, sProgID$
'
'        sErrMsg = "volatility source settings."
'
'        Erase m_VolatilitySourceData
'        cboVSType.Clear
'
'        nCount = aVolaSources.Count
'
'        If nCount > 0 Then
'            ReDim m_VolatilitySourceData(1 To nCount)
'            For i = 1 To nCount
'                aVolaSources.GetVolatilitySourceInfo i, nID, sName, sFileName, sVersion, sProgID
'                With m_VolatilitySourceData(i)
'                    .ID = nID
'                    .Name = sName
'                    .ProgID = sProgID
'
'                    cboVSType.AddItem .Name
'                    cboVSType.ItemData(cboVSType.NewIndex) = i
'
'                    If g_Params.SystemVolatilitySourceType = .ID Then
'                        cboVSType.ListIndex = cboVSType.NewIndex
'                        m_nCurVSIndex = i
'                    End If
'                End With
'            Next
'        End If
'
'        If cboVSType.ListCount > 0 Then
'            If cboVSType.ListIndex < 0 Then
'                cboVSType.ListIndex = 0
'                m_nCurVSIndex = cboVSType.ItemData(cboVSType.ListIndex)
'                m_bVSChanged = True
'            End If
'        End If
'    End If
'
'    If m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoTntOnly Then
'        sErrMsg = "TNT settings."
'        'txtTNAccount.MaxLength = TNT_ACCOUNT_MAXLEN
'        txtTNPassword.MaxLength = TNT_PASSWORD_MAXLEN
'
'        If Not g_TntProcessor Is Nothing Then
'            If g_TntProcessor.UseTntMessaging And g_CurTraderID <> 0 Then
'                m_TntConnSettings = g_TntProcessor.ConnSettings
'            End If
'        End If
'
'    End If
'EX:
'    m_bInit = False
'    Exit Sub
'EH:
'    m_bInit = False
'    gCmn.ErrorMsgBox IIf(m_bShowModal, Nothing, Me), "Fail to load " & sErrMsg
'    UpdatePPControls
'    UpdateIVControls
'    UpdateTNControls
'    GoTo EX
'End Sub
'
'Private Sub AddPPInfoToList(sName As String, nID As Long, nItemData As Long)
'    On Error Resume Next
'    With cboPPType
'        .AddItem sName
'        .ItemData(.NewIndex) = nItemData
'
'        If g_Params.SystemPriceProviderType = nID Then
'            .ListIndex = .NewIndex
'            m_nCurPPIndex = nItemData
'        End If
'    End With
'End Sub
'
'Private Function FillProviderData(ProviderData As ProviderDataType, Provider As IProviderData) As Boolean
'    On Error GoTo PPEH
'    With ProviderData
'        Set .Provider = Provider
'        .ID = .Provider.ProviderID
'        .Name = .Provider.Description
'        .UseLogin = .Provider.NeedLogin
'        .ProgID = .Provider.ProgID(enPriceInfo)
'
'        .Account = ""
'        .Password = ""
'        .UseProxy = enProxyNone
'        .ProxyAddress = ""
'        .ProxyPort = 0
'        .ProxyAuthType = enProxyAuthNone
'
'        If .UseLogin Then
'            .Account = .Provider.UserAccount
'            .Password = .Provider.UserPassword
'            .UseProxy = .Provider.UseProxy
'            If .UseProxy <> enProxyNone Then
'                .ProxyAddress = .Provider.ProxyName
'                .ProxyPort = .Provider.ProxyPort
'                .ProxyAuthType = .Provider.ProxyAuthorization
'                If .ProxyAuthType <> enProxyAuthNone Then
'                    .ProxyLogin = .Provider.ProxyLogin
'                    .ProxyPassword = .Provider.ProxyPassword
'                End If
'            End If
'        End If
'    End With
'    FillProviderData = True
'    Exit Function
'PPEH:
'    FillProviderData = False
'End Function
'
'Private Sub SaveData()
'    On Error GoTo EH
'    Dim sErrMsg$
'    sErrMsg = "connections settings."
'
'    If m_enShowOptions = enCsoAll And m_bDBChanged Then
'        If g_Params.SystemDbConnection <> m_sConnectionString Then g_Params.SystemDbConnection = m_sConnectionString
'        If m_bUpdateCurSettings And g_Params.DbConnection <> m_sConnectionString Then g_Params.DbConnection = m_sConnectionString
'    End If
'
'    If (m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoPriceProviderOnly) And m_bPPChanged Then
'        If m_nCurPPIndex > 0 Then
'            sErrMsg = "price providers settings."
'
'            With m_ProviderData(m_nCurPPIndex)
'                If .UseLogin Then
'                    .Provider.UserAccount = .Account
'                    .Provider.UserPassword = .Password
'                    If .UseProxy <> enProxyNone Then
'                        .Provider.UseProxy = .UseProxy
'                        .Provider.ProxyName = .ProxyAddress
'                        .Provider.ProxyPort = .ProxyPort
'                        If .ProxyAuthType <> enProxyAuthNone Then
'                            .Provider.ProxyAuthorization = .ProxyAuthType
'                            .Provider.ProxyLogin = .ProxyLogin
'                            .Provider.ProxyPassword = .ProxyPassword
'                        End If
'                    End If
'                End If
'
'                If g_Params.SystemPriceProviderType <> .ID Then g_Params.SystemPriceProviderType = .ID
'                If m_bUpdateCurSettings And g_Params.PriceProviderType <> .ID Then
'                    g_Params.PriceProviderType = .ID
'                    g_Params.PriceProviderProgID = .ProgID
'                End If
'            End With
'        End If
'    End If
'
'    If m_enShowOptions = enCsoAll And m_bIVChanged Then
'        Dim aIVSettings As ETSMANAGERLib.IVSettings
'        sErrMsg = "IVolatility.com settings."
'
'        Set aIVSettings = New ETSMANAGERLib.IVSettings
'
'        aIVSettings.VolServerName = m_idtIvSettings.Address
'        aIVSettings.VolLogin = m_idtIvSettings.Account
'        aIVSettings.VolPassword = m_idtIvSettings.Password
'        aIVSettings.UseProxy = m_idtIvSettings.UseProxy
'        aIVSettings.ProxyName = m_idtIvSettings.ProxyAddress
'        aIVSettings.ProxyPort = m_idtIvSettings.ProxyPort
'        aIVSettings.ProxyAuthType = m_idtIvSettings.ProxyAuthType
'        aIVSettings.ProxyLogin = m_idtIvSettings.ProxyLogin
'        aIVSettings.ProxyPassword = m_idtIvSettings.ProxyPassword
'
'        Set aIVSettings = Nothing
'    End If
'
'    If (m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoVolatilitySourceOnly) And m_bVSChanged Then
'        If m_nCurVSIndex > 0 Then
'            sErrMsg = "volatility source settings."
'            With m_VolatilitySourceData(m_nCurVSIndex)
'                If g_Params.SystemVolatilitySourceType <> .ID Then g_Params.SystemVolatilitySourceType = .ID
'                If m_bUpdateCurSettings And g_Params.VolatilitySourceType <> .ID Then
'                    g_Params.VolatilitySourceType = .ID
'                    g_Params.VolatilitySourceProgID = .ProgID
'                End If
'            End With
'        End If
'    End If
'
'    If (m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoTntOnly) Then
'        sErrMsg = "TNT settings."
'        If Not g_TntProcessor Is Nothing Then
'            If g_TntProcessor.UseTntMessaging And g_CurTraderID <> 0 Then
'                g_TntProcessor.SetNewConnSettings m_TntConnSettings
'            End If
'        End If
'
'    End If
'
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox IIf(m_bShowModal, Nothing, Me), "Fail to save " & sErrMsg
'End Sub
'
'Private Sub UpdatePPControls()
'    On Error Resume Next
'    Dim bDisableAll As Boolean
'    m_bInit = True
'
'    bDisableAll = True
'    If m_nCurPPIndex > 0 Then
'        With m_ProviderData(m_nCurPPIndex)
'            If .UseLogin Then
'                bDisableAll = False
'
'                lblPPAccount.Enabled = True
'                lblPPPwd.Enabled = True
'                txtPPAccount.Enabled = True
'                txtPPPwd.Enabled = True
'
'                txtPPAccount.Text = .Account
'                txtPPPwd.Text = .Password
'
'                If .UseProxy <> enProxyNone Then
'                    lblPPProxy.Enabled = True
'                    optPPProxyNone.Enabled = True
'                    optPPProxySystem.Enabled = True
'                    optPPProxyCustom.Enabled = True
'
'                    optPPProxyNone.Value = (.UseProxy = enProxyDisable)
'                    optPPProxySystem.Value = (.UseProxy = enProxyDefault)
'                    optPPProxyCustom.Value = (.UseProxy = enProxyCustom)
'
'                    txtPPProxyAddr.Text = .ProxyAddress
'                    txtPPProxyPort.Text = IIf(.ProxyPort > 0, .ProxyPort, "")
'                    txtPPProxyLogin.Text = .ProxyLogin
'                    txtPPProxyPwd.Text = .ProxyPassword
'                    chkPPProxyAuth.Value = IIf(.ProxyAuthType = enProxyAuthEnable, 1, 0)
'
'                    If .UseProxy = enProxyCustom Then
'                        lblPPProxyAddr.Enabled = True
'                        lblPPProxyPort.Enabled = True
'
'                        txtPPProxyAddr.Enabled = True
'                        txtPPProxyPort.Enabled = True
'
'                        If .ProxyAuthType <> enProxyAuthNone Then
'                            chkPPProxyAuth.Enabled = True
'
'                            If .ProxyAuthType = enProxyAuthDisable Then
'                                lblPPProxyLogin.Enabled = True
'                                lblPPProxyPwd.Enabled = True
'
'                                txtPPProxyLogin.Enabled = True
'                                txtPPProxyPwd.Enabled = True
'                            Else
'                                lblPPProxyLogin.Enabled = False
'                                lblPPProxyPwd.Enabled = False
'
'                                txtPPProxyLogin.Enabled = False
'                                txtPPProxyPwd.Enabled = False
'                            End If
'                        Else
'                            lblPPProxyLogin.Enabled = False
'                            lblPPProxyPwd.Enabled = False
'
'                            chkPPProxyAuth.Enabled = False
'                            txtPPProxyLogin.Enabled = False
'                            txtPPProxyPwd.Enabled = False
'
'                            chkPPProxyAuth.Value = 0
'                            txtPPProxyLogin.Text = ""
'                            txtPPProxyPwd.Text = ""
'                        End If
'                    Else
'                        lblPPProxyAddr.Enabled = False
'                        lblPPProxyPort.Enabled = False
'                        lblPPProxyLogin.Enabled = False
'                        lblPPProxyPwd.Enabled = False
'
'                        chkPPProxyAuth.Enabled = False
'                        txtPPProxyAddr.Enabled = False
'                        txtPPProxyPort.Enabled = False
'                        txtPPProxyLogin.Enabled = False
'                        txtPPProxyPwd.Enabled = False
'                    End If
'                Else
'                    lblPPProxy.Enabled = False
'                    lblPPProxyAddr.Enabled = False
'                    lblPPProxyPort.Enabled = False
'                    lblPPProxyLogin.Enabled = False
'                    lblPPProxyPwd.Enabled = False
'
'                    optPPProxyNone.Enabled = False
'                    optPPProxySystem.Enabled = False
'                    optPPProxyCustom.Enabled = False
'                    txtPPProxyAddr.Enabled = False
'                    txtPPProxyPort.Enabled = False
'                    chkPPProxyAuth.Enabled = False
'                    txtPPProxyLogin.Enabled = False
'                    txtPPProxyPwd.Enabled = False
'
'                    optPPProxyNone.Value = True
'                    chkPPProxyAuth.Value = 0
'                    txtPPProxyAddr.Text = ""
'                    txtPPProxyPort.Text = ""
'                    txtPPProxyLogin.Text = ""
'                    txtPPProxyPwd.Text = ""
'                End If
'            End If
'        End With
'    End If
'
'    If bDisableAll Then
'        lblPPAccount.Enabled = False
'        lblPPPwd.Enabled = False
'        lblPPProxy.Enabled = False
'        lblPPProxyAddr.Enabled = False
'        lblPPProxyPort.Enabled = False
'        lblPPProxyLogin.Enabled = False
'        lblPPProxyPwd.Enabled = False
'
'        txtPPAccount.Enabled = False
'        txtPPPwd.Enabled = False
'        optPPProxyNone.Enabled = False
'        optPPProxySystem.Enabled = False
'        optPPProxyCustom.Enabled = False
'        txtPPProxyAddr.Enabled = False
'        txtPPProxyPort.Enabled = False
'        chkPPProxyAuth.Enabled = False
'        txtPPProxyLogin.Enabled = False
'        txtPPProxyPwd.Enabled = False
'
'        txtPPAccount.Text = ""
'        txtPPPwd.Text = ""
'        optPPProxyNone.Value = True
'        optPPProxySystem.Value = False
'        optPPProxyCustom.Value = False
'        txtPPProxyAddr.Text = ""
'        txtPPProxyPort.Text = ""
'        chkPPProxyAuth.Value = 0
'        txtPPProxyLogin.Text = ""
'        txtPPProxyPwd.Text = ""
'    End If
'
'    AdjustPPControlsColors
'
'    m_bInit = False
'End Sub
'
'Private Sub UpdateTNControls()
'    On Error Resume Next
'    m_bInit = True
'    Dim bEnabled As Boolean
'
'    bEnabled = False
'    If Not g_TntProcessor Is Nothing Then
'        bEnabled = g_TntProcessor.UseTntMessaging And g_CurTraderID <> 0
'    End If
'
'    lblTNAccount.Enabled = False
'    txtTNAccount.Enabled = False
'
'    fraTN.Enabled = bEnabled
'    lblTNPassword.Enabled = bEnabled
'    lblTNPrimaryIP.Enabled = bEnabled
'    lblTNSecondaryIP.Enabled = bEnabled
'    lblTNPort.Enabled = bEnabled
'
'    txtTNPassword.Enabled = bEnabled
'    txtTNPrimaryIP.Enabled = bEnabled
'    txtTNSecondaryIP.Enabled = bEnabled
'    txtTNPort.Enabled = bEnabled
'
'    If bEnabled Then
'        With m_TntConnSettings
'            txtTNAccount.Text = .ID
'            txtTNPassword.Text = .Password
'            txtTNPrimaryIP.Text = .PrimaryIP
'            txtTNSecondaryIP.Text = .SecondaryIP
'            txtTNPort.Text = .Port
'        End With
'    End If
'
'    AdjustTNControlsColors
'
'    m_bInit = False
'End Sub
'
'Private Sub UpdateIVControls()
'    On Error Resume Next
'    m_bInit = True
'
'    With m_idtIvSettings
'        txtIVAddr.Text = .Address
'        txtIVAccount.Text = .Account
'        txtIVPwd.Text = .Password
'
'        optIVProxyNone.Value = (.UseProxy = ETSMANAGERLib.UseProxyType.enProxyDisable)
'        optIVProxySystem.Value = (.UseProxy = ETSMANAGERLib.UseProxyType.enProxyDefault)
'        optIVProxyCustom.Value = (.UseProxy = ETSMANAGERLib.UseProxyType.enProxyCustom)
'
'        txtIVProxyAddr.Text = .ProxyAddress
'        txtIVProxyPort.Text = IIf(.ProxyPort > 0, .ProxyPort, "")
'        txtIVProxyLogin.Text = .ProxyLogin
'        txtIVProxyPwd.Text = .ProxyPassword
'        chkIVProxyAuth.Value = IIf(.ProxyAuthType = ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthEnable, 1, 0)
'
'        If .UseProxy = ETSMANAGERLib.UseProxyType.enProxyCustom Then
'            lblIVProxyAddr.Enabled = True
'            lblIVProxyPort.Enabled = True
'
'            txtIVProxyAddr.Enabled = True
'            txtIVProxyPort.Enabled = True
'
'            chkIVProxyAuth.Enabled = True
'
'            If .ProxyAuthType = ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthEnable Then
'                lblIVProxyLogin.Enabled = True
'                lblIVProxyPwd.Enabled = True
'
'                txtIVProxyLogin.Enabled = True
'                txtIVProxyPwd.Enabled = True
'            Else
'                lblIVProxyLogin.Enabled = False
'                lblIVProxyPwd.Enabled = False
'
'                txtIVProxyLogin.Enabled = False
'                txtIVProxyPwd.Enabled = False
'            End If
'        Else
'            lblIVProxyAddr.Enabled = False
'            lblIVProxyPort.Enabled = False
'            lblIVProxyLogin.Enabled = False
'            lblIVProxyPwd.Enabled = False
'
'            chkIVProxyAuth.Enabled = False
'            txtIVProxyAddr.Enabled = False
'            txtIVProxyPort.Enabled = False
'            txtIVProxyLogin.Enabled = False
'            txtIVProxyPwd.Enabled = False
'        End If
'    End With
'
'    AdjustIVControlsColors
'
'    m_bInit = False
'End Sub
'
'Private Sub AdjustPPControlsColors()
'    On Error Resume Next
'    txtPPAccount.BackColor = IIf(txtPPAccount.Enabled, &H80000005, &H8000000F)
'    txtPPPwd.BackColor = IIf(txtPPPwd.Enabled, &H80000005, &H8000000F)
'    txtPPProxyAddr.BackColor = IIf(txtPPProxyAddr.Enabled, &H80000005, &H8000000F)
'    txtPPProxyPort.BackColor = IIf(txtPPProxyPort.Enabled, &H80000005, &H8000000F)
'    txtPPProxyLogin.BackColor = IIf(txtPPProxyLogin.Enabled, &H80000005, &H8000000F)
'    txtPPProxyPwd.BackColor = IIf(txtPPProxyPwd.Enabled, &H80000005, &H8000000F)
'End Sub
'
'Private Sub AdjustIVControlsColors()
'    On Error Resume Next
'    txtIVAccount.BackColor = IIf(txtIVAccount.Enabled, &H80000005, &H8000000F)
'    txtIVPwd.BackColor = IIf(txtIVPwd.Enabled, &H80000005, &H8000000F)
'    txtIVProxyAddr.BackColor = IIf(txtIVProxyAddr.Enabled, &H80000005, &H8000000F)
'    txtIVProxyPort.BackColor = IIf(txtIVProxyPort.Enabled, &H80000005, &H8000000F)
'    txtIVProxyLogin.BackColor = IIf(txtIVProxyLogin.Enabled, &H80000005, &H8000000F)
'    txtIVProxyPwd.BackColor = IIf(txtIVProxyPwd.Enabled, &H80000005, &H8000000F)
'End Sub
'
'Private Sub AdjustTNControlsColors()
'    On Error Resume Next
'    txtTNAccount.BackColor = IIf(txtTNAccount.Enabled, &H80000005, &H8000000F)
'    txtTNPassword.BackColor = IIf(txtTNPassword.Enabled, &H80000005, &H8000000F)
'    txtTNPrimaryIP.BackColor = IIf(txtTNPrimaryIP.Enabled, &H80000005, &H8000000F)
'    txtTNSecondaryIP.BackColor = IIf(txtTNSecondaryIP.Enabled, &H80000005, &H8000000F)
'    txtTNPort.BackColor = IIf(txtTNPort.Enabled, &H80000005, &H8000000F)
'End Sub
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
'    If m_bLoading Then Exit Sub
'
'    If m_bShowModal Then
'        m_mbResult = vbCancel
'    End If
'
'    Unload Me
'End Sub
'
'Private Sub btnDB_Click()
'    On Error GoTo EH
'    Dim sConnString$
'
'    sConnString = m_sConnectionString
'    If gDBW.ChangeDatabase(Me, , False, sConnString) Then
'        If m_sConnectionString <> sConnString Then
'            m_sConnectionString = sConnString
'            txtDatabase.Text = ExtractDbPath(m_sConnectionString)
'            m_bDBChanged = True
'        End If
'    End If
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox IIf(m_bShowModal, Nothing, Me), "Fail to change database connection."
'End Sub
'
'Private Sub btnOK_Click()
'    On Error Resume Next
'    If m_bLoading Then Exit Sub
'
'    'If m_bDBChanged Or m_bPPChanged Or m_bIVChanged Or m_bVSChanged Or m_bTNChanged Then
'        SaveData
'        If Not m_bShowModal Then
'            RaiseEvent OnOK(m_bDBChanged, m_bPPChanged, m_bIVChanged, m_bVSChanged, m_bTNChanged)
'        End If
'    'End If
'
'    If m_bShowModal Then
'        m_mbResult = vbOK
'    End If
'
'    Unload Me
'End Sub
'
'Private Sub cboPPType_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        Dim nIdx&, nLBnd&, nUBnd&
'        nIdx = -1
'        If cboPPType.ListCount > 0 Then
'            If cboPPType.ListIndex >= 0 Then nIdx = cboPPType.ItemData(cboPPType.ListIndex)
'        End If
'
'        nLBnd = 0
'        nLBnd = LBound(m_ProviderData)
'        nUBnd = -1
'        nUBnd = UBound(m_ProviderData)
'        If nIdx >= nLBnd And nIdx <= nUBnd Then
'            m_nCurPPIndex = nIdx
'        Else
'            m_nCurPPIndex = 0
'        End If
'
'        UpdatePPControls
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub cboVSType_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        Dim nIdx&, nLBnd&, nUBnd&
'        nIdx = -1
'        If cboVSType.ListCount > 0 Then
'            If cboVSType.ListIndex >= 0 Then nIdx = cboVSType.ItemData(cboVSType.ListIndex)
'        End If
'
'        nLBnd = 0
'        nLBnd = LBound(m_VolatilitySourceData)
'        nUBnd = -1
'        nUBnd = UBound(m_VolatilitySourceData)
'        If nIdx >= nLBnd And nIdx <= nUBnd Then
'            m_nCurVSIndex = nIdx
'        Else
'            m_nCurVSIndex = 0
'        End If
'
'        m_bVSChanged = True
'    End If
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
''    gCmn.CustomizeForm Me
'End Sub
'
'Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
'    On Error Resume Next
'    If UnloadMode = vbFormControlMenu Then Cancel = m_bLoading
'End Sub
'
'Private Sub Form_Terminate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'
'    If Not m_bShowModal Then
'        If Not m_frmOwner Is Nothing Then
'            m_frmOwner.Enabled = True
'            m_frmOwner.SetFocus
'        End If
'    End If
'
'    Erase m_ProviderData
'    Erase m_VolatilitySourceData
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    Form_Terminate
'End Sub
'
'Private Sub optPPProxyCustom_Click()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).UseProxy = enProxyCustom
'
'        lblPPProxyPort.Enabled = True
'        lblPPProxyAddr.Enabled = True
'
'        txtPPProxyPort.Enabled = True
'        txtPPProxyAddr.Enabled = True
'
'        If m_ProviderData(m_nCurPPIndex).ProxyAuthType <> enProxyAuthNone Then
'            chkPPProxyAuth.Enabled = True
'
'            If m_ProviderData(m_nCurPPIndex).ProxyAuthType = enProxyAuthEnable Then
'                lblPPProxyLogin.Enabled = True
'                lblPPProxyPwd.Enabled = True
'
'                txtPPProxyLogin.Enabled = True
'                txtPPProxyPwd.Enabled = True
'            Else
'                lblPPProxyLogin.Enabled = False
'                lblPPProxyPwd.Enabled = False
'
'                txtPPProxyLogin.Enabled = False
'                txtPPProxyPwd.Enabled = False
'            End If
'        End If
'
'        AdjustPPControlsColors
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub optPPProxyNone_Click()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).UseProxy = enProxyDisable
'
'        lblPPProxyPort.Enabled = False
'        lblPPProxyAddr.Enabled = False
'
'        txtPPProxyPort.Enabled = False
'        txtPPProxyAddr.Enabled = False
'
'        If m_ProviderData(m_nCurPPIndex).ProxyAuthType <> enProxyAuthNone Then
'            chkPPProxyAuth.Enabled = False
'
'            lblPPProxyLogin.Enabled = False
'            lblPPProxyPwd.Enabled = False
'
'            txtPPProxyLogin.Enabled = False
'            txtPPProxyPwd.Enabled = False
'        End If
'
'        AdjustPPControlsColors
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub optPPProxySystem_Click()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).UseProxy = enProxyDefault
'
'        lblPPProxyPort.Enabled = False
'        lblPPProxyAddr.Enabled = False
'
'        txtPPProxyPort.Enabled = False
'        txtPPProxyAddr.Enabled = False
'
'        If m_ProviderData(m_nCurPPIndex).ProxyAuthType <> enProxyAuthNone Then
'            chkPPProxyAuth.Enabled = False
'
'            lblPPProxyLogin.Enabled = False
'            lblPPProxyPwd.Enabled = False
'
'            txtPPProxyLogin.Enabled = False
'            txtPPProxyPwd.Enabled = False
'        End If
'
'        AdjustPPControlsColors
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub tmrLoad_Timer()
'    On Error Resume Next
'    tmrLoad.Enabled = False
'    Me.Enabled = False
'
'    LoadData
'
'    DoEvents
'    m_bLoading = False
'
'    Me.Caption = "Connections Settings"
'    Me.Enabled = True
'
'    If m_enShowOptions = enCsoAll Then
'        m_sConnectionString = g_Params.SystemDbConnection
'        txtDatabase.Text = ExtractDbPath(m_sConnectionString)
'    End If
'
'    If m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoPriceProviderOnly Then UpdatePPControls
'    If m_enShowOptions = enCsoAll Then UpdateIVControls
'    If m_enShowOptions = enCsoAll Or m_enShowOptions = enCsoTntOnly Then UpdateTNControls
'
'    UpdateButtons
'
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub txtPPAccount_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).Account = txtPPAccount.Text
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub txtPPProxyAddr_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).ProxyAddress = txtPPProxyAddr.Text
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub txtPPProxyLogin_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).ProxyLogin = txtPPProxyLogin.Text
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub txtPPProxyPort_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        Dim nValue&, nSelStart&, bRevert As Boolean
'
'        bRevert = False
'        nValue = 0&
'        If Len(Trim$(txtPPProxyPort.Text)) <> 0 Then
'            nValue = CLng(txtPPProxyPort.Text)
'            bRevert = (Err.Number <> 0)
'        End If
'
'        If Not bRevert Then
'            If nValue < 0 Then
'                nValue = m_ProviderData(m_nCurPPIndex).ProxyPort
'                bRevert = True
'            End If
'
'            m_ProviderData(m_nCurPPIndex).ProxyPort = nValue
'        End If
'
'        If bRevert Then
'            nSelStart = txtPPProxyPort.SelStart
'            txtPPProxyPort.Text = IIf(m_ProviderData(m_nCurPPIndex).ProxyPort > 0, m_ProviderData(m_nCurPPIndex).ProxyPort, "")
'            txtPPProxyPort.SelStart = nSelStart
'        Else
'            m_bPPChanged = True
'        End If
'    End If
'End Sub
'
'Private Sub txtPPProxyPwd_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).ProxyPassword = txtPPProxyPwd.Text
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub txtPPPwd_Change()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).Password = txtPPPwd.Text
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub chkPPProxyAuth_Click()
'    On Error Resume Next
'    If Not m_bInit And m_nCurPPIndex > 0 Then
'        m_ProviderData(m_nCurPPIndex).ProxyAuthType = IIf(chkPPProxyAuth.Value <> 0, enProxyAuthEnable, enProxyAuthDisable)
'        If m_ProviderData(m_nCurPPIndex).ProxyAuthType = enProxyAuthEnable Then
'            lblPPProxyLogin.Enabled = True
'            lblPPProxyPwd.Enabled = True
'
'            txtPPProxyLogin.Enabled = True
'            txtPPProxyPwd.Enabled = True
'        Else
'            lblPPProxyLogin.Enabled = False
'            lblPPProxyPwd.Enabled = False
'
'            txtPPProxyLogin.Enabled = False
'            txtPPProxyPwd.Enabled = False
'        End If
'
'        AdjustPPControlsColors
'        m_bPPChanged = True
'    End If
'End Sub
'
'Private Sub optIVProxyCustom_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.UseProxy = ETSMANAGERLib.UseProxyType.enProxyCustom
'
'        lblIVProxyPort.Enabled = True
'        lblIVProxyAddr.Enabled = True
'
'        txtIVProxyPort.Enabled = True
'        txtIVProxyAddr.Enabled = True
'
'        chkIVProxyAuth.Enabled = True
'
'        If m_idtIvSettings.ProxyAuthType = ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthEnable Then
'            lblIVProxyLogin.Enabled = True
'            lblIVProxyPwd.Enabled = True
'
'            txtIVProxyLogin.Enabled = True
'            txtIVProxyPwd.Enabled = True
'        Else
'            lblIVProxyLogin.Enabled = False
'            lblIVProxyPwd.Enabled = False
'
'            txtIVProxyLogin.Enabled = False
'            txtIVProxyPwd.Enabled = False
'        End If
'
'        AdjustIVControlsColors
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub optIVProxyNone_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.UseProxy = ETSMANAGERLib.UseProxyType.enProxyDisable
'
'        lblIVProxyPort.Enabled = False
'        lblIVProxyAddr.Enabled = False
'
'        txtIVProxyPort.Enabled = False
'        txtIVProxyAddr.Enabled = False
'
'        chkIVProxyAuth.Enabled = False
'
'        lblIVProxyLogin.Enabled = False
'        lblIVProxyPwd.Enabled = False
'
'        txtIVProxyLogin.Enabled = False
'        txtIVProxyPwd.Enabled = False
'
'        AdjustIVControlsColors
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub optIVProxySystem_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.UseProxy = ETSMANAGERLib.UseProxyType.enProxyDefault
'
'        lblIVProxyPort.Enabled = False
'        lblIVProxyAddr.Enabled = False
'
'        txtIVProxyPort.Enabled = False
'        txtIVProxyAddr.Enabled = False
'
'        chkIVProxyAuth.Enabled = False
'
'        lblIVProxyLogin.Enabled = False
'        lblIVProxyPwd.Enabled = False
'
'        txtIVProxyLogin.Enabled = False
'        txtIVProxyPwd.Enabled = False
'
'        AdjustIVControlsColors
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVAddr_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.Address = txtIVAddr.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVAccount_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.Account = txtIVAccount.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVProxyAddr_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.ProxyAddress = txtIVProxyAddr.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVProxyLogin_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.ProxyLogin = txtIVProxyLogin.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVProxyPort_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        Dim nValue&, nSelStart&, bRevert As Boolean
'
'        bRevert = False
'        nValue = 0&
'        If Len(Trim$(txtIVProxyPort.Text)) <> 0 Then
'            nValue = CLng(txtIVProxyPort.Text)
'            bRevert = (Err.Number <> 0)
'        End If
'
'        If Not bRevert Then
'            If nValue < 0 Then
'                nValue = m_idtIvSettings.ProxyPort
'                bRevert = True
'            End If
'
'            m_idtIvSettings.ProxyPort = nValue
'        End If
'
'        If bRevert Then
'            nSelStart = txtIVProxyPort.SelStart
'            txtIVProxyPort.Text = IIf(m_idtIvSettings.ProxyPort > 0, m_idtIvSettings.ProxyPort, "")
'            txtIVProxyPort.SelStart = nSelStart
'        Else
'            m_bIVChanged = True
'        End If
'    End If
'End Sub
'
'Private Sub txtIVProxyPwd_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.ProxyPassword = txtIVProxyPwd.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub txtIVPwd_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.Password = txtIVPwd.Text
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub chkIVProxyAuth_Click()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_idtIvSettings.ProxyAuthType = IIf(chkIVProxyAuth.Value <> 0, _
'                                        ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthEnable, _
'                                        ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthDisable)
'
'        If m_idtIvSettings.ProxyAuthType = ETSMANAGERLib.ProxyAuthentificationType.enProxyAuthEnable Then
'            lblIVProxyLogin.Enabled = True
'            lblIVProxyPwd.Enabled = True
'
'            txtIVProxyLogin.Enabled = True
'            txtIVProxyPwd.Enabled = True
'        Else
'            lblIVProxyLogin.Enabled = False
'            lblIVProxyPwd.Enabled = False
'
'            txtIVProxyLogin.Enabled = False
'            txtIVProxyPwd.Enabled = False
'        End If
'
'        AdjustIVControlsColors
'        m_bIVChanged = True
'    End If
'End Sub
'
'Private Sub UpdateButtons()
'    On Error Resume Next
'    btnCancel.Enabled = Not m_bLoading
'    btnOK.Enabled = Not m_bLoading
'    btnDB.Enabled = Not m_bLoading
'End Sub
'
'Private Function ExtractDbPath(ByVal sConnectionString As String) As String
'    On Error Resume Next
'    Dim aConn As New ADODB.Connection
'    aConn.ConnectionString = sConnectionString
'    ExtractDbPath = sConnectionString
'    ExtractDbPath = aConn.Properties("Data Source").Value & "\" & aConn.Properties("Initial Catalog").Value
'End Function
'
''Private Sub txtTNAccount_Change()
''    On Error Resume Next
''    If Not m_bInit Then
''        m_tntConnSettings.Account = txtTNAccount.Text
''        m_bTNChanged = True
''    End If
''End Sub
'
'Private Sub txtTNPassword_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_TntConnSettings.Password = txtTNPassword.Text
'        m_bTNChanged = True
'    End If
'End Sub
'
'Private Sub txtTNPort_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        Dim nValue&
'
'        nValue = CLng(txtTNPort.Text)
'
'        If nValue <> m_TntConnSettings.Port Then
'            m_TntConnSettings.Port = nValue
'            m_bTNChanged = True
'        End If
'    End If
'End Sub
'
'Private Sub txtTNPrimaryIP_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_TntConnSettings.PrimaryIP = txtTNPrimaryIP.Text
'        m_bTNChanged = True
'    End If
'End Sub
'
'Private Sub txtTNSecondaryIP_Change()
'    On Error Resume Next
'    If Not m_bInit Then
'        m_TntConnSettings.SecondaryIP = txtTNSecondaryIP.Text
'        m_bTNChanged = True
'    End If
'End Sub
