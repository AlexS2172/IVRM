VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmMain 
   Caption         =   "IvRmDataLoader"
   ClientHeight    =   5760
   ClientLeft      =   165
   ClientTop       =   645
   ClientWidth     =   9210
   LinkTopic       =   "Form1"
   ScaleHeight     =   5760
   ScaleWidth      =   9210
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrStartTimeCheck 
      Interval        =   6000
      Left            =   6720
      Top             =   0
   End
   Begin MSForms.ListBox lbLogView 
      Height          =   5580
      Left            =   0
      TabIndex        =   0
      Top             =   120
      Width           =   9135
      ScrollBars      =   3
      DisplayStyle    =   2
      Size            =   "16113;9842"
      MatchEntry      =   0
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Menu mnuMain 
      Caption         =   "Exit"
   End
   Begin VB.Menu mnuStart 
      Caption         =   "Process"
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private WithEvents aMainProcessor As clsMainProcessor
Attribute aMainProcessor.VB_VarHelpID = -1
Private bInProc As Boolean
Private lSleepInterval As Long
Private aEgFormat As New EgFormatLib.DateTimeFormat
Private nMinuteCounter As Long
Private bFirstStart As Boolean

Private Sub LogMsg(ByVal sInfo As String)
    lbLogView.AddItem Str(Time) + " " + sInfo
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, sInfo, "IvrmDataLoader"

End Sub

Private Sub Form_Initialize()
    Set aMainProcessor = New clsMainProcessor
End Sub

Public Function IsWorkingPeriod() As Boolean
    If (GetTimeFromLongToMinute(g_lStartTime) <= GetCurrentTimeInMinutes) And _
        (GetTimeFromLongToMinute(g_lStopTime) >= GetCurrentTimeInMinutes) Then
        IsWorkingPeriod = True
    Else
        IsWorkingPeriod = False
    End If
End Function

Private Sub Form_Load()
    tmrStartTimeCheck.Enabled = True
    lSleepInterval = Abs(GetTimeFromLongToMinute(g_lStartTime) - GetTimeFromLongToMinute(g_lStopTime)) / g_lFrequency
    tmrStartTimeCheck.Interval = 60000
    bFirstStart = True
    bInProc = False
    DoEvents
End Sub

Private Sub Form_Resize()
    lbLogView.Width = Width - 125
    lbLogView.Height = Abs(ScaleHeight - lbLogView.Top)
    Debug.Print lbLogView.Height
End Sub

Private Sub Form_Terminate()
    Set aMainProcessor = Nothing
    DeleteGlobals
End Sub

Private Sub aMainProcessor_UpdateLog(ByVal sInfo As String)
    LogMsg sInfo
End Sub

Private Sub mnuMain_Click()
    If Not bInProc Then Unload Me
End Sub

Private Sub SaveLastDataDateTime()
    If Not g_aMainXMLParams Is Nothing Then
        g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastDataDate", Format(Date)
        g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastDataTime", Format(Time)
    End If
End Sub

Private Sub StartProcessing()
    On Error GoTo Err
    Dim strPrStep As String
    
    Dim sCurrentProcFileName As String
    
        If g_sFtpFolder <> "" Then
        
            strPrStep = "DIVIDENDS"
            If g_sDividendsFile <> "" Then
                sCurrentProcFileName = aEgFormat.egStrftime(g_sDividendsFile)
                If aMainProcessor.ExecuteDividentImport(g_sFtpFolder & "\" & sCurrentProcFileName) Then
                    CopyAndDelete sCurrentProcFileName
                    PubMessage "Dividends’ file has been uploaded. Please restart your IV RM to see the changes."
                    g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastDivFile", "[" & Format(Time) & "]" & sCurrentProcFileName
                    SaveLastDataDateTime
                End If
                Sleep 5
            Else
                LogMsg "Dividends source file not defined"
            End If
            LogMsg ""

            strPrStep = "VOLATILITY"
            If g_sVolaFile <> "" Then
                sCurrentProcFileName = aEgFormat.egStrftime(g_sVolaFile)
                If aMainProcessor.ExecuteVolatilityImport(g_sFtpFolder & "\" & sCurrentProcFileName) Then
                    CopyAndDelete sCurrentProcFileName
                    PubMessage "Volatilities’ file has been uploaded. Please restart your IV RM to see the changes."
                    g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastVolaFile", "[" & Format(Time) & "]" & sCurrentProcFileName
                    SaveLastDataDateTime
                End If
                Sleep 5
            Else
                LogMsg "Volatiliti source file not defined"
            End If
            LogMsg ""

            strPrStep = "INTERESTRATES"
            If Len(g_sInterestRatesFile) <> 0 Then
                sCurrentProcFileName = aEgFormat.egStrftime(g_sInterestRatesFile)
                If aMainProcessor.ExecuteImportCurve(g_sFtpFolder & "\" & sCurrentProcFileName) Then
                    CopyAndDelete sCurrentProcFileName
                    PubMessage "Interest file has been uploaded. Please restart your IV RM to see the changes."
                    g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastIRFile", "[" & Format(Time) & "]" & sCurrentProcFileName
                    SaveLastDataDateTime
                End If
                Sleep 5
            Else
                LogMsg "Interest Rates source file not defined"
            End If
            LogMsg ""
            
            strPrStep = "INDEXWEIGHT"
            If Len(g_sIndexWeightFile) <> 0 Then
                sCurrentProcFileName = aEgFormat.egStrftime(g_sIndexWeightFile)
                If aMainProcessor.ExecuteLoadBasketWeights(g_sFtpFolder & "\" & sCurrentProcFileName) Then
                    CopyAndDelete sCurrentProcFileName
                    PubMessage "Index components’ file has been uploaded. Please restart your IV RM to see the changes."
                    g_aMainXMLParams.SetStringValue GROUP_XML_KEY & g_sUserGroup & "\DataLoader", "LastIWFile", "[" & Format(Time) & "]" & sCurrentProcFileName
                    SaveLastDataDateTime
                End If
                Sleep 5
            Else
                LogMsg "Index source file not defined"
            End If
            LogMsg ""
            SaveSettings
        Else
            LogMsg "FtpFolder not defined"
        End If
        
    Exit Sub
Err:
    LogMsg "[Error] on Processing: " & strPrStep
End Sub

Private Function CopyAndDelete(ByVal sFileName As String)

    Dim lRet As Long, j As Long
    Dim vLines As Variant
    Dim sFileExt As String
    Dim sOnlyFileName As String

    vLines = Split(sFileName, ".")
    sFileExt = vLines(UBound(vLines))
    sOnlyFileName = vLines(0)

    lRet = CopyFileA(g_sFtpFolder & "\" & sFileName, g_sDataHistoryPath & "\" & sOnlyFileName & "." & _
                                            Format(Date, "MM-DD-YYYY") & "." & Format(Time, "hh-mm") & "." & sFileExt, False)
    If lRet <> 0 Then
        lRet = DeleteFileA(g_sFtpFolder & "\" & sFileName)
        If lRet <> 0 Then
            LogMsg "[Complete] Delete and Copy to history path: " & sFileName
        Else
            LogMsg "[Error] Can't delete file! " & g_sFtpFolder & "\" & sFileName
        End If
    Else
        LogMsg "[Error] Can't copy file to history! " & g_sFtpFolder & "\" & sFileName
    End If

End Function

Private Sub mnuStart_Click()
    StartProcessing
End Sub

Private Sub tmrStartTimeCheck_Timer()

    If nMinuteCounter >= lSleepInterval Or bFirstStart Then
    
        If IsWorkingPeriod And Not bInProc Then
            StartProcessing
            bInProc = False
            bFirstStart = False
        End If
        nMinuteCounter = 0
        
    Else
        
       nMinuteCounter = nMinuteCounter + 1
       
    End If
End Sub
