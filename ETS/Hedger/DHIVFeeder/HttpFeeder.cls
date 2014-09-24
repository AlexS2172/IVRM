VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "HttpFeeder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "HttpFeeder"

Public Event GotRequest(ByVal Message As String)
Public Event GotReply(ByVal Message As String)
Public Event StateChanged(ByVal State As Long, ByVal Message As String)
Public Event LogMessage(ByVal Message As String)

Private Const TIMEOUT = 200 ' sec
Private WithEvents IVFeed As InetCtlsObjects.Inet
Attribute IVFeed.VB_VarHelpID = -1

Private m_VolServerName As String
Private m_VolServerPort As Long
Private m_VolServerLogin As String
Private m_VolServerPassword As String
Private m_VolServerAppPath As String

Private m_UseProxy As ETSMANAGERLib.UseProxyType
Private m_ProxyName As String
Private m_ProxyPort As Long

Private m_ProxyAuthType As ETSMANAGERLib.ProxyAuthentificationType
Private m_ProxyLogin As String
Private m_ProxyPassword As String

Private Data As String
Private tStart As Date
'Private fDone As Boolean
'Private pSuccess As Boolean
'Private pDescription As String

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean

Friend Property Get VolServerLogin() As String
    On Error Resume Next
    VolServerLogin = m_VolServerLogin
End Property

Friend Property Get VolServerPassword() As String
    On Error Resume Next
    VolServerPassword = m_VolServerPassword
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    On Error GoTo EH
    Dim IvSettings As ETSMANAGERLib.IvSettings
    
    Set IvSettings = New ETSMANAGERLib.IvSettings
    
    With IvSettings
        m_VolServerName = .VolServerName
        If m_VolServerName = "" Then
            m_VolServerName = "http://www.ivolatility.com"
'        Else
'            Dim nPos&
'            nPos = InStr(1, m_VolServerName, "http://", vbTextCompare)
'            If nPos > 0 Then
'                m_VolServerName = Left$(m_VolServerName, nPos - 1) & Mid$(m_VolServerName, nPos + 7)
'            End If
        End If
        m_VolServerPort = .VolServerPort
        
        m_VolServerLogin = .VolLogin
        m_VolServerPassword = .VolPassword
        m_VolServerAppPath = .VolServerAppPath
        If m_VolServerAppPath = "" Then m_VolServerAppPath = "data_load"
        
        m_UseProxy = .UseProxy
        m_ProxyName = .ProxyName
        m_ProxyPort = .ProxyPort
        m_ProxyAuthType = .ProxyAuthType
        m_ProxyLogin = .ProxyLogin
        m_ProxyPassword = .ProxyPassword
    End With
    
    Set IVFeed = New InetCtlsObjects.Inet
    
    Set IvSettings = Nothing
    Exit Sub
EH:
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = "Fail to load IV connection settings. " & Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source
    
    Set IvSettings = Nothing
    Err.Raise nNumber, sSource, sDescription, sHelpFile, nHelpContext
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    On Error Resume Next
    Set IVFeed = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub IVFeed_StateChanged(ByVal State As Integer)
Dim vtData As Variant
Dim strData As String
Dim bDone As Boolean
    On Error GoTo Herr
    RaiseEvent StateChanged(State, InetState(State))
    Select Case State
        ' ... Other cases not shown.
    Case icError ' 11
        ' In case of error, return ResponseCode and ResponseInfo.
        strData = IVFeed.ResponseCode & ":" & IVFeed.ResponseInfo
        RaiseEvent LogMessage(strData)
    Case icResponseCompleted ' 12
        bDone = False
        ' Get first chunk.
        On Error GoTo Failed
        vtData = IVFeed.GetChunk(1024, icString)
        Dim V
        DoEvents
        Do While Not bDone
            strData = strData & vtData
            ' Get next chunk.
            vtData = IVFeed.GetChunk(1024, icString)
            DoEvents
            If Len(vtData) = 0 Then
                bDone = True
            End If
        Loop
        Data = strData
    Case Else
        Debug.Print State, InetState(State)
    End Select
    Exit Sub
Herr:
    Debug.Print "Http error:", Err.Number, Err.Description
    RaiseEvent LogMessage("Http error: " & Err.Description)
    CancelRequested = True
    Exit Sub
Failed:
    Debug.Print "Http error:", Err.Number, Err.Description
    RaiseEvent LogMessage("Connection error.")
    CancelRequested = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function InetState(ByVal State As Integer) As String
    Select Case State
    Case icNone '0
        InetState = ""
    Case icResolvingHost '1
        InetState = "Resolving Host"
    Case icHostResolved '2
        InetState = "Host Resolved"
    Case icConnecting '3
        InetState = "Connecting"
    Case icConnected '4
        InetState = "Connected"
    Case icRequesting '5
        InetState = "Requesting"
    Case icRequestSent '6
        InetState = "Request Sent"
    Case icReceivingResponse '7
        InetState = "Receiving Response"
    Case icResponseReceived '8
        InetState = "Response Received"
    Case icDisconnecting '9
        InetState = "Disconnecting"
    Case icDisconnected ' 10
        InetState = "Disconnected"
    Case icError '11
        InetState = "Error"
    Case icResponseCompleted '12
        InetState = "ResponseCompleted"
    Case Else
        InetState = Format$(State)
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function HttpCall(ByVal Address As String) As String
Dim Url As String
    On Error GoTo Herr
    
    TaskStarted
    
    Url = m_VolServerName & "/" & m_VolServerAppPath
    
'    Url = "http://"
'    If m_VolServerLogin <> "" Then
'        Url = Url & m_VolServerLogin
'        If m_VolServerPassword <> "" Then
'            Url = Url & ":" & m_VolServerPassword
'        End If
'        Url = Url & "@"
'    End If
'
'    Url = Url & m_VolServerName & "/" & m_VolServerAppPath
    Url = Url & "/" & Address
    Debug.Print "HttpCall  :", Url
    RaiseEvent GotRequest(Url)
    
    With IVFeed
        If .StillExecuting = True Then
            .Cancel
        End If
        tStart = Now
        
        If m_VolServerPort <> 0 Then
            .RemotePort = m_VolServerPort
        End If
        
        Select Case m_UseProxy
            Case enProxyDisable
                .Proxy = ""
                .AccessType = icDirect
                
            Case enProxyDefault
                .Proxy = ""
                .AccessType = icUseDefault
                
            Case enProxyCustom
                Dim S As String
                ' http=proxy:3128 '
                If Len(m_ProxyName) > 0 Then
                    S = m_ProxyName
                    If m_ProxyPort <> 0 Then
                        S = S & ":" & Format$(m_ProxyPort)
                    End If
                    .Proxy = S
                    .AccessType = icNamedProxy
                Else
                    .AccessType = icUseDefault
                End If
                
        End Select
        '//////////////////////
        .Execute Url, "GET" '/
        '////////////////////
        Do While .StillExecuting
            If CancelRequested Then
                .Cancel
                CancelAccepted = True
                Exit Do
            End If
            DoEvents
            Sleep 1
            If DateDiff("s", tStart, Now) > TIMEOUT Then
                ' Timeout condition has been detected
                CancelRequested = True
                RaiseEvent LogMessage("Http error: Timeout condition has been detected.")
            End If
        Loop
    End With

Done:
    If TaskFinished Then
        HttpCall = Data
    Else
        HttpCall = ""
    End If
    Debug.Print "Replay  :", "[" & Left(HttpCall, 200) & "]"
    RaiseEvent GotReply(Data)
    
    Exit Function
Herr:
    Debug.Print "Http error:", Err.Number, Err.Description
    RaiseEvent LogMessage("Http error: " & Err.Description)
    CancelRequested = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskStarted() As Boolean
    TaskStarted = Not TaskInProgress
    If TaskStarted Then
        TaskInProgress = True
        CancelRequested = False
        CancelAccepted = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskFinished() As Boolean
    TaskFinished = TaskInProgress And Not CancelRequested
    TaskInProgress = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskCanceled() As Boolean
    TaskCanceled = TaskInProgress And CancelAccepted
    TaskInProgress = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Busy() As Boolean
    Busy = TaskInProgress
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CancelTask() As Boolean
    CancelTask = Busy
    CancelRequested = True
    CancelAccepted = False
End Function

