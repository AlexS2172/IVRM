VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmMain 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Test Transport Performance"
   ClientHeight    =   3315
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   10470
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3315
   ScaleWidth      =   10470
   StartUpPosition =   2  'CenterScreen
   Begin VB.TextBox txtSeqLenMax 
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
      Left            =   7320
      TabIndex        =   16
      Top             =   780
      Width           =   915
   End
   Begin VB.TextBox txtSeqLenPeriod 
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
      Left            =   7320
      TabIndex        =   15
      Top             =   300
      Width           =   915
   End
   Begin VB.TextBox txtLog 
      Height          =   1635
      Left            =   60
      Locked          =   -1  'True
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   9
      Top             =   1280
      Width           =   10335
   End
   Begin MSComctlLib.ProgressBar pbProcess 
      Height          =   255
      Left            =   60
      TabIndex        =   8
      Top             =   2960
      Width           =   3195
      _ExtentX        =   5636
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   0
      Scrolling       =   1
   End
   Begin VB.CommandButton cmdCancel 
      Caption         =   "&Cancel"
      Height          =   315
      Left            =   9000
      TabIndex        =   7
      Top             =   2960
      Width           =   1275
   End
   Begin VB.CommandButton cmdRun 
      Caption         =   "&Run"
      Height          =   315
      Left            =   7560
      TabIndex        =   6
      Top             =   2960
      Width           =   1275
   End
   Begin VB.TextBox txtSeqNum 
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
      Left            =   3600
      TabIndex        =   4
      Top             =   780
      Width           =   915
   End
   Begin VB.TextBox txtSeqLen 
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
      Left            =   1500
      TabIndex        =   2
      Top             =   780
      Width           =   915
   End
   Begin VB.Frame frmQuery 
      Caption         =   " Query "
      Height          =   1175
      Left            =   60
      TabIndex        =   0
      Top             =   60
      Width           =   8775
      Begin VB.OptionButton rbSeries 
         Caption         =   "Series"
         Height          =   315
         Left            =   4860
         TabIndex        =   12
         Top             =   720
         Width           =   855
      End
      Begin VB.OptionButton rbOnce 
         Caption         =   "Once"
         Height          =   255
         Left            =   4860
         TabIndex        =   11
         Top             =   300
         Width           =   855
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "Seq Len Max :"
         Height          =   195
         Left            =   5940
         TabIndex        =   14
         Top             =   780
         Width           =   1035
      End
      Begin VB.Label lblSeqLenPeriod 
         AutoSize        =   -1  'True
         Caption         =   "Seq Len Period :"
         Height          =   195
         Left            =   5940
         TabIndex        =   13
         Top             =   300
         Width           =   1185
      End
      Begin MSForms.ComboBox cbTargetCompID 
         Height          =   315
         Left            =   1440
         TabIndex        =   10
         Top             =   240
         Width           =   2535
         VariousPropertyBits=   746604571
         DisplayStyle    =   3
         Size            =   "4471;556"
         MatchEntry      =   1
         ShowDropButtonWhen=   2
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lblSeqNum 
         AutoSize        =   -1  'True
         Caption         =   "Seq Num :"
         Height          =   195
         Left            =   2640
         TabIndex        =   5
         Top             =   780
         Width           =   750
      End
      Begin VB.Label lblSeqLen 
         AutoSize        =   -1  'True
         Caption         =   "Seq Len :"
         Height          =   195
         Left            =   600
         TabIndex        =   3
         Top             =   780
         Width           =   690
      End
      Begin VB.Label lblTargetCompID 
         AutoSize        =   -1  'True
         Caption         =   "Target Comp ID :"
         Height          =   195
         Left            =   120
         TabIndex        =   1
         Top             =   300
         Width           =   1215
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents MmwMsgManagerS As MMWLib.MsgManager
Attribute MmwMsgManagerS.VB_VarHelpID = -1
Private WithEvents MmwMsgManagerR As MMWLib.MsgManager
Attribute MmwMsgManagerR.VB_VarHelpID = -1

Private g_UID As String
Private g_bRun As Boolean
Private g_nSeqLen As Long
Private g_nSeqNum As Long
Private g_nSeqLenPeriod As Long
Private g_nSeqLenMax As Long
Private g_nSeq As Long
Private g_tStart As Date
Private g_nStart As Double
Private g_nStop As Double

Private Declare Function GetComputerName Lib "kernel32" Alias "GetComputerNameA" (ByVal lpBuffer As String, nSize As Long) As Long

Private Sub cmdClose_Click()
    Unload Me
End Sub

Private Sub cbTargetCompID_Validate(Cancel As Boolean)
    With cbTargetCompID
        Dim n As Long
        For n = 0 To .ListCount - 1
            If .List(n) = .Text Then Exit Sub
        Next
        .AddItem .Text
    End With
End Sub

Private Sub cmdCancel_Click()
    SetStopState
    AddInfoStr
End Sub

Private Sub cmdRun_Click()
    On Error GoTo EH
    Dim aDynamicMessage As New DynamicMessage
    Dim aPubManager As MMWLib.IPubManager
    Dim str As String
    
    aDynamicMessage.Value("Query") = True
    aDynamicMessage.Value("Sender") = g_UID
    aDynamicMessage.Value("SeqLen") = CLng(txtSeqLen)
    aDynamicMessage.Value("SeqNum") = CLng(txtSeqNum)
    aDynamicMessage.Value("SeqLenPeriod") = CLng(txtSeqLenPeriod)
    
    If rbOnce Then
        aDynamicMessage.Value("SeqLenMax") = CLng(txtSeqLen)
    Else
        aDynamicMessage.Value("SeqLenMax") = CLng(txtSeqLenMax)
    End If
    
    SetRunState
    
    Set aPubManager = MmwMsgManagerS
    aPubManager.PubDynamicMessage UCase(cbTargetCompID), aDynamicMessage
    
    Set aPubManager = Nothing
    Set aDynamicMessage = Nothing
    
    Exit Sub
EH:
    SetStopState
    
    Set aPubManager = Nothing
    Set aDynamicMessage = Nothing
    MsgBox Err.Description
End Sub

Private Sub Form_Load()
    On Error GoTo EH
    Dim nRes As Long
    Dim nSize As Long
    
    g_UID = Space(255)
    nSize = Len(g_UID)
    
    If GetComputerName(g_UID, nSize) = 0 Or nSize = 0 Then
        Err.Raise vbObject, , "Failed to get computer name"
    End If
    
    g_UID = UCase(Left$(g_UID, nSize))
    
    cbTargetCompID.AddItem g_UID
    cbTargetCompID.ListIndex = 0
    txtSeqLen = 1000
    txtSeqNum = 100
    txtSeqLenPeriod = 200
    txtSeqLenMax = 2000
    rbOnce = True
    rbSeries = False
    
    txtLog.Text = " <Target>" & Chr(vbKeyTab) & Chr(vbKeyTab) & "<Time>" & Chr(vbKeyTab) & _
        Chr(vbKeyTab) & "<SeqLen>" & Chr(vbKeyTab) & "<SeqNum>" & _
        Chr(vbKeyTab) & "<ActualSeqs>" & Chr(vbKeyTab) & "<TotalTime>" & _
        Chr(vbKeyTab) & "<byte per second>" & _
        Chr(vbKeyReturn) & Chr(10) & Chr(vbKeyReturn) & Chr(10)
    
    SetStopState
    
    Set MmwMsgManagerS = New MMWLib.MsgManager
    Set MmwMsgManagerR = New MMWLib.MsgManager
            
    If MmwMsgManagerS Is Nothing Or MmwMsgManagerR Is Nothing Then
        Err.Raise vbObject, , "Failed to create MsgManager object."
    End If
        
    Dim aPubManager As MMWLib.IPubManager
    Dim aSubManager As MMWLib.ISubManager
    
    Set aPubManager = MmwMsgManagerS
    aPubManager.RegPublisher enMtDynamicMessage, Nothing
    Set aPubManager = Nothing
    
    Set aSubManager = MmwMsgManagerR
    aSubManager.SubDynamicMessage g_UID
    Set aSubManager = Nothing
    
    Exit Sub
EH:
    Set aPubManager = Nothing
    Set MmwMsgManagerS = Nothing
    Set MmwMsgManagerR = Nothing
    
    MsgBox "Fail to initialize. " & Err.Description
    Unload Me
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error GoTo EH
    Dim aPubManager As MMWLib.IPubManager
    Dim aSubManager As MMWLib.ISubManager
    
    If Not MmwMsgManagerS Is Nothing Then
        Set aPubManager = MmwMsgManagerS
        aPubManager.UnregPublisher enMtDynamicMessage, Nothing
        Set aPubManager = Nothing
    End If
    
    If Not MmwMsgManagerR Is Nothing And g_UID <> "" Then
        Set aSubManager = MmwMsgManagerR
        aSubManager.UnsubDynamicMessage g_UID
        Set aSubManager = Nothing
    End If
        
    Exit Sub
EH:
    Set MmwMsgManagerS = Nothing
    Set MmwMsgManagerR = Nothing
    Set aSubManager = Nothing
    Set aPubManager = Nothing
End Sub

Private Sub MmwMsgManagerR_OnDynamicMessage(ByVal Data As MSGSTRUCTLib.IDynamicMessage)
    On Error GoTo EH
    
    If Not Data.Value("Query") Then
        If g_bRun Then
            g_nSeq = Data.Value("Seq")
            If g_nSeqNum <= g_nSeq Then
                g_nSeqLen = Data.Value("SeqLen")
                g_nStop = Timer
                
                AddInfoStr
                
                g_nSeq = 0
                g_tStart = Now
                g_nStart = Timer
                
                If Data.Value("SeqLen") > g_nSeqLenMax - g_nSeqLenPeriod Then
                    SetStopState
                End If

            End If
        End If
    Else
        Dim nSeqLen
        Dim nSeqNum
        Dim nSeqLenPeriod
        Dim nSeqLenMax
        Dim aPubManager As MMWLib.IPubManager
        Dim sUID As String
        Dim aDynamicMessage As New DynamicMessage
        Dim n
        Dim k
        
        nSeqLen = Data.Value("SeqLen")
        nSeqNum = Data.Value("SeqNum")
        nSeqLenPeriod = Data.Value("SeqLenPeriod")
        nSeqLenMax = Data.Value("SeqLenMax")
        sUID = Data.Value("Sender")
        
        Set aPubManager = MmwMsgManagerS
        
        aDynamicMessage.Value("Query") = False
        aDynamicMessage.Value("SeqNum") = nSeqNum
        aDynamicMessage.Value("SeqLenPeriod") = nSeqLenPeriod
        aDynamicMessage.Value("SeqLenMax") = nSeqLenMax
        aDynamicMessage.Value("Data") = Space(nSeqLen)
        
        
        While nSeqLen <= nSeqLenMax
            aDynamicMessage.Value("SeqLen") = nSeqLen
            
            For n = 1 To nSeqNum
                aDynamicMessage.Value("Seq") = n
                aPubManager.PubDynamicMessage sUID, aDynamicMessage
                DoEvents
            Next
            
            nSeqLen = nSeqLen + nSeqLenPeriod
        Wend
    
        Set aPubManager = Nothing
    End If
    
    Exit Sub
EH:
    SetStopState
    AddInfoStr
End Sub

Private Sub SetStopState()
    cmdRun.Enabled = True
    cmdCancel.Enabled = False
    cbTargetCompID.Enabled = True
    txtSeqLen.Enabled = True
    txtSeqNum.Enabled = True
    
    If Not rbOnce Then
        txtSeqLenPeriod.Enabled = True
        txtSeqLenMax.Enabled = True
    End If
    
    rbOnce.Enabled = True
    rbSeries.Enabled = True
'    pbProcess.BorderStyle = ccNone
    
    g_bRun = False
    g_nStop = Timer
End Sub

Private Sub SetRunState()
    cmdRun.Enabled = False
    cmdCancel.Enabled = True
    cbTargetCompID.Enabled = False
    txtSeqLen.Enabled = False
    txtSeqNum.Enabled = False
    txtSeqLenPeriod.Enabled = False
    txtSeqLenMax.Enabled = False
    rbOnce.Enabled = False
    rbSeries.Enabled = False
'    pbProcess.BorderStyle = ccFixedSingle
    
    g_bRun = True
    g_nSeqLen = txtSeqLen
    g_nSeqNum = txtSeqNum
    g_nSeqLenPeriod = txtSeqLenPeriod
    
    If rbOnce Then
        g_nSeqLenMax = txtSeqLen
    Else
        g_nSeqLenMax = txtSeqLenMax
    End If
    
    g_nSeq = 0
    g_tStart = Now
    g_nStart = Timer
    g_nStop = g_nStart
End Sub

Private Sub AddInfoStr()
    Dim dbTime As Double
    Dim dbBytePerSecond As Double
    
    dbTime = g_nStop - g_nStart
    dbBytePerSecond = g_nSeq * g_nSeqLen / dbTime
    
    txtLog.SelStart = Len(txtLog)
    txtLog.SelLength = 1
    txtLog.SelText = " " & cbTargetCompID & Chr(vbKeyTab) & Chr(vbKeyTab) & _
        FormatDateTime(g_tStart, vbLongTime) & Chr(vbKeyTab) & g_nSeqLen & _
        Chr(vbKeyTab) & Chr(vbKeyTab) & g_nSeqNum & Chr(vbKeyTab) & Chr(vbKeyTab) & _
        g_nSeq & Chr(vbKeyTab) & Chr(vbKeyTab) & Format(dbTime, "#####0.000000") & _
        Chr(vbKeyTab) & Chr(vbKeyTab) & Format(dbBytePerSecond, "#######0.0") & _
        Chr(vbKeyReturn) & Chr(10)
End Sub

Private Sub rbOnce_Click()
    txtSeqLenPeriod.Enabled = False
    txtSeqLenMax.Enabled = False
End Sub

Private Sub rbSeries_Click()
    txtSeqLenPeriod.Enabled = True
    txtSeqLenMax.Enabled = True
End Sub
