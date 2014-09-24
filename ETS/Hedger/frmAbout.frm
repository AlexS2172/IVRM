VERSION 5.00
Object = "{E395ECF2-0B93-464D-AD1B-99A443BDF17F}#1.0#0"; "FocusFlatControls1.ocx"
Begin VB.Form frmAbout 
   Appearance      =   0  'Flat
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About vStrategy"
   ClientHeight    =   6435
   ClientLeft      =   2340
   ClientTop       =   1935
   ClientWidth     =   5775
   ClipControls    =   0   'False
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmAbout.frx":0000
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6435
   ScaleWidth      =   5775
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtProductInfo 
      Appearance      =   0  'Flat
      Height          =   4575
      Left            =   1080
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   1  'Horizontal
      TabIndex        =   0
      Tag             =   "noauto"
      Top             =   1080
      Width           =   4455
   End
   Begin FocusFlatControls1.FlatButton btnSysInfo 
      Height          =   315
      Left            =   2760
      TabIndex        =   1
      Top             =   6000
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "  &System Info ..."
   End
   Begin FocusFlatControls1.FlatButton btnOK 
      Cancel          =   -1  'True
      Default         =   -1  'True
      Height          =   315
      Left            =   4200
      TabIndex        =   2
      Top             =   6000
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   " &OK"
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Product Info :"
      Height          =   225
      Left            =   1080
      TabIndex        =   7
      Top             =   840
      Width           =   4485
   End
   Begin VB.Label lblVersion 
      BackStyle       =   0  'Transparent
      Caption         =   "Version"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Left            =   1080
      TabIndex        =   6
      Top             =   360
      Width           =   2085
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Application Title"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   240
      Left            =   1080
      TabIndex        =   5
      Top             =   120
      Width           =   2205
   End
   Begin VB.Label lblDescription 
      BackStyle       =   0  'Transparent
      Caption         =   "Description"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   330
      Left            =   240
      TabIndex        =   4
      Top             =   2040
      Visible         =   0   'False
      Width           =   4485
   End
   Begin VB.Line Line1 
      BorderColor     =   &H80000010&
      BorderStyle     =   6  'Inside Solid
      X1              =   120
      X2              =   5684
      Y1              =   5805
      Y2              =   5805
   End
   Begin VB.Label lblCopyright 
      BackStyle       =   0  'Transparent
      Caption         =   "Copyright © 2002, EGAR Technology Inc."
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   225
      Left            =   1080
      TabIndex        =   3
      Top             =   600
      Width           =   4485
   End
   Begin VB.Image imgIcon 
      Appearance      =   0  'Flat
      Height          =   480
      Left            =   240
      Top             =   120
      Width           =   480
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' **************************************************************
' Module            frmAbout
'
' Filename          frmAbout.frm
'
' Author            Oleg Sharkevich
'                   eGar Technology Inc.
'
' Description
'
' About Dialog form module
'
' **************************************************************

Option Explicit

' registry keys consts for MSInfo app
Private Const REGKEY_SYSINFO_LOC = "SOFTWARE\Microsoft\Shared Tools Location"
Private Const REGVAL_SYSINFO_LOC = "MSINFO"
Private Const REGKEY_SYSINFO = "SOFTWARE\Microsoft\Shared Tools\MSINFO"
Private Const REGVAL_SYSINFO = "PATH"

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnSysInfo_Click()
    On Error Resume Next
    ' run MSInfo app
    StartSysInfo
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    ' close dialog
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    With New clsCommon
        .CustomizeForm Me
    End With
    Set imgIcon.Picture = frmIcon.Icon
    ' set About dlg title & App info
    Me.Caption = "About " & APP_TITLE
    lblVersion.Caption = "Version " & App.Major & "." & App.Minor & "." & App.Revision
    lblTitle.Caption = APP_TITLE
    Dim S As String
    'S = App.FileDescription
    'S = "Volatility Dispersion Trading Analytic tools"
    lblDescription.Caption = S
    S = S & MakeComponentsDesc
    txtProductInfo.Text = S
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' start sysinfo application loading
'
Public Sub StartSysInfo()
    On Error GoTo SysInfoErr
  
    Dim sSysInfoPath As String
    
    ' Try To Get System Info Program Path\Name From Registry...
    GetStrKeyValue HKEY_LOCAL_MACHINE, REGKEY_SYSINFO, REGVAL_SYSINFO, sSysInfoPath
    If sSysInfoPath = "" Then
        ' Try To Get System Info Program Path Only From Registry...
        GetStrKeyValue HKEY_LOCAL_MACHINE, REGKEY_SYSINFO_LOC, REGVAL_SYSINFO_LOC, sSysInfoPath
        If sSysInfoPath <> "" Then
            ' Validate Existance Of Known 32 Bit File lblVersion
            If (Dir$(sSysInfoPath & "\MSINFO32.EXE") <> "") Then
                sSysInfoPath = sSysInfoPath & "\MSINFO32.EXE"
                
            ' Error - File Can Not Be Found...
            Else
                GoTo SysInfoErr
            End If
        ' Error - Registry Entry Can Not Be Found...
        Else
            GoTo SysInfoErr
        End If
    End If
    
    ' open MSInfo app
    Shell sSysInfoPath, vbNormalFocus
    
    Exit Sub
SysInfoErr:
    ' notify error
    gCmn.MyMsgBox "System Information Is Unavailable At This Time", vbOKOnly + vbInformation
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function MakeComponentsDesc() As String
    On Error Resume Next
    Dim cVI As clsVersionInfo, sPath As String, sVersion As String, sFile As String
    MakeComponentsDesc = " Components:" & vbTab & vbTab & "Version:" & vbTab & vbTab & "Path:" & vbCrLf

''''    sVersion = ""
''''    Dim rs As ADODB.Recordset
''''    Set rs = gDBW.usp_DataInfo_Get(, "version")
''''    If Not rs Is Nothing Then
''''        If Not rs.EOF Then
''''            sVersion = rs!vcKeyValue
''''        End If
''''        Set rs = Nothing
''''    End If
''''    MakeComponentsDesc = MakeComponentsDesc & " " & "Database" & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") & vbCrLf

    Set cVI = New clsVersionInfo
    
    sPath = App.Path
    If Right$(sPath, 1) <> "\" And Right$(sPath, 1) <> "/" Then
        sPath = sPath & "\"
    End If
    
    Dim ProgID As String
    Dim ProgPath  As String
    
    sFile = "MSAdo15.dll"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf
'
'    sFile = "Dao360.dll"
'    ProgPath = GetPathFromFile(sFile)
'    If ProgPath = "" Then
'        ProgPath = sPath & sFile
'    End If
'    If cVI.SetFilePath(ProgPath) Then
'        sVersion = cVI.FixedFileVersion
'    ElseIf cVI.SetFilePath(sFile) Then
'        sVersion = cVI.FixedFileVersion
'    Else
'        sVersion = ""
'    End If
'    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
'        & vbTab & ProgPath & vbCrLf
    
    sFile = "Msinet.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf
    
    sFile = "DHIVFeeder4.dll"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf
    
    sFile = "DHMTFeeder2.dll"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf
'
'    sFile = "MyTrackProviders.exe"
'    ProgPath = GetPathFromFile(sFile)
'    If ProgPath = "" Then
'        ProgPath = sPath & sFile
'    End If
'    If cVI.SetFilePath(ProgPath) Then
'        sVersion = cVI.FixedFileVersion
'    ElseIf cVI.SetFilePath(sFile) Then
'        sVersion = cVI.FixedFileVersion
'    Else
'        sVersion = ""
'    End If
'    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
'        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "PriceProviders.dll"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf


    sFile = "ElladaFlatControls.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "FocusFlatControls1.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "DHlButtons.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf

    sFile = "axButton.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "workflowic.dll"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf
'
'    sFile = "edaTools.dll"
'    ProgPath = "" 'GetPathFromFile(sFile)
'    If ProgPath = "" Then
'        ProgPath = sPath & sFile
'    End If
'    If cVI.SetFilePath(ProgPath) Then
'        sVersion = cVI.FixedFileVersion
'    ElseIf cVI.SetFilePath(sFile) Then
'        sVersion = cVI.FixedFileVersion
'    Else
'        sVersion = ""
'    End If
'    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
'        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "Vsflex7.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf

    sFile = "Vsocx6.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath & vbCrLf

    sFile = "VSPrint7.ocx"
    ProgPath = GetPathFromFile(sFile)
    If ProgPath = "" Then
        ProgPath = sPath & sFile
    End If
    If cVI.SetFilePath(ProgPath) Then
        sVersion = cVI.FixedFileVersion
    ElseIf cVI.SetFilePath(sFile) Then
        sVersion = cVI.FixedFileVersion
    Else
        sVersion = ""
    End If
    MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & vbTab & ProgPath
    ' With full path?
    'MakeComponentsDesc = MakeComponentsDesc & " " & sFile & vbTab & vbTab & IIf(sVersion <> "", sVersion, "N/A") _
        & vbTab & ProgPath & vbCrLf

    Set cVI = Nothing
End Function

