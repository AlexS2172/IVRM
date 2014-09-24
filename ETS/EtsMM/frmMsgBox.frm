VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmMsgBox 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "IVRm"
   ClientHeight    =   1365
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4380
   Icon            =   "frmMsgBox.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   1365
   ScaleWidth      =   4380
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton btnAction 
      Caption         =   "3"
      Height          =   315
      Index           =   3
      Left            =   2783
      TabIndex        =   2
      Top             =   960
      Width           =   1095
   End
   Begin VB.CheckBox chkOption 
      Height          =   255
      Left            =   720
      TabIndex        =   3
      Top             =   600
      Width           =   3495
   End
   Begin MSComctlLib.ImageList ilMsgType 
      Left            =   2580
      Top             =   0
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   32
      ImageHeight     =   32
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   4
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMsgBox.frx":000C
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMsgBox.frx":045E
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMsgBox.frx":08B0
            Key             =   ""
         EndProperty
         BeginProperty ListImage4 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "frmMsgBox.frx":0D02
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin VB.CommandButton btnAction 
      Caption         =   "2"
      Height          =   315
      Index           =   2
      Left            =   1643
      TabIndex        =   1
      Top             =   960
      Width           =   1095
   End
   Begin VB.CommandButton btnAction 
      Caption         =   "1"
      Height          =   315
      Index           =   1
      Left            =   503
      TabIndex        =   0
      Top             =   960
      Width           =   1095
   End
   Begin VB.Label lblPrompt 
      AutoSize        =   -1  'True
      Caption         =   "Prompt"
      Height          =   195
      Left            =   720
      TabIndex        =   4
      Top             =   120
      Width           =   495
   End
   Begin VB.Image imgMsgType 
      Height          =   480
      Left            =   120
      Top             =   120
      Width           =   480
   End
End
Attribute VB_Name = "frmMsgBox"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private Const MAX_BUTTONS& = 3&
Private Const MIN_WIDTH! = 4470!
Private Const MIN_HEIGHT! = 1740!

Private m_hComplete As Long

Private m_enResult As VbMsgBoxResult
Private m_enButton(1 To MAX_BUTTONS) As VbMsgBoxResult
Private m_bShowOption As Boolean

Public Function Execute(ByRef frmOwner As Form, _
                        ByVal enStyle As VbMsgBoxStyle, _
                        ByVal sPrompt As String, _
                        Optional ByVal sTitle As String = "", _
                        Optional ByVal bShowOption As Boolean = False, _
                        Optional ByRef bOptionValue As Boolean, _
                        Optional ByVal sOptionPrompt As String = "", _
                        Optional ByVal sBtnText1 As String = "", _
                        Optional ByVal sBtnText2 As String = "", _
                        Optional ByVal sBtnText3 As String = "") As VbMsgBoxResult
                        
    Dim bRestoreEnabled As Boolean, nTempMouse&
    
    nTempMouse = Screen.MousePointer
    Screen.MousePointer = vbDefault

    m_enResult = 0
    m_bShowOption = bShowOption And Not IsMissing(bOptionValue)
    
    Load Me
    
    InitBox enStyle, sPrompt, sTitle, bOptionValue, sOptionPrompt, sBtnText1, sBtnText2, sBtnText3
    
    bRestoreEnabled = False
    m_hComplete = 0
    If Not frmOwner Is Nothing Then
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            
            Me.Show vbModeless, frmOwner
        
            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLINPUT) <> WAIT_OBJECT_0
                Sleep 0
                DoEvents
            Wend
            
            If m_hComplete <> 0 Then
                CloseHandle m_hComplete
                m_hComplete = 0
            End If
            
            If bRestoreEnabled Then frmOwner.Enabled = True
        Else
            Me.Show vbModal, frmOwner
        End If
    Else
        Me.Show vbModal, frmOwner
    End If
    
    If m_bShowOption Then bOptionValue = (chkOption.Value <> 0)
    
    Screen.MousePointer = nTempMouse
    
    Unload Me
    
    Execute = m_enResult
    Exit Function
EH:
    On Error Resume Next
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
    
    Err.Clear
    Execute = MsgBox(sPrompt, enStyle, sTitle)
    If Err.Number <> 0 Then LogEvent EVENT_ERROR, Err.Description

    Screen.MousePointer = nTempMouse
End Function

Private Sub btnAction_Click(Index As Integer)
    On Error Resume Next
    m_enResult = m_enButton(Index)
    
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub btnAction_KeyUp(Index As Integer, KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    Select Case KeyCode
        Case vbKeyLeft, vbKeyUp
            SendKeys "+{TAB}"
            
        Case vbKeyRight, vbKeyDown
            SendKeys "{TAB}"
    End Select
End Sub

Private Sub Form_Load()
 Me.Caption = AppTitle()
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_hComplete <> 0 Then SetEvent m_hComplete
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If m_hComplete <> 0 Then
        CloseHandle m_hComplete
        m_hComplete = 0
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Private Sub InitBox(ByVal enStyle As VbMsgBoxStyle, ByVal sPrompt As String, ByVal sTitle As String, _
                        ByVal bOptionValue As Boolean, ByVal sOptionPrompt As String, _
                        ByVal sBtnText1 As String, ByVal sBtnText2 As String, ByVal sBtnText3 As String)
    On Error Resume Next
    Dim nWidth!, nLeft!, nButtonTop!, nBtnCount&, i&
    
    Me.Caption = IIf(Len(sTitle) > 0, sTitle, AppTitle())
    
    chkOption.Visible = m_bShowOption
    If m_bShowOption Then
        chkOption.Caption = sOptionPrompt
        chkOption.Value = IIf(bOptionValue, 1, 0)
    End If
    
    Select Case True
        Case (enStyle And vbExclamation) = vbExclamation
            i = 2
            
        Case (enStyle And vbCritical) = vbCritical
            i = 3
            
        Case (enStyle And vbQuestion) = vbQuestion
            i = 4
    
        Case Else ' or (enStyle And vbInformation) = vbInformation
            i = 1
    End Select
    
    Set imgMsgType.Picture = ilMsgType.ListImages(i).Picture
    imgMsgType.Visible = True
    
    lblPrompt.Caption = sPrompt
    lblPrompt.Left = imgMsgType.Left + imgMsgType.Width + 120
    
    If lblPrompt.Top + lblPrompt.Height > imgMsgType.Top + imgMsgType.Height Then
        nButtonTop = lblPrompt.Top + lblPrompt.Height + 120
    Else
        nButtonTop = imgMsgType.Top + imgMsgType.Height + 120
    End If
    
    If m_bShowOption Then
        chkOption.Top = nButtonTop
        nButtonTop = nButtonTop + chkOption.Height + 120
        
        If lblPrompt.Left + lblPrompt.Width + 120 > MIN_WIDTH Then
            chkOption.Width = lblPrompt.Width
        End If
    End If
    
    If lblPrompt.Left + lblPrompt.Width + 120 > MIN_WIDTH Then
        nWidth = lblPrompt.Left + lblPrompt.Width + 120
    Else
        nWidth = MIN_WIDTH
    End If
    
    Select Case enStyle And &HF
        Case vbOKCancel
            m_enButton(1) = vbOK
            m_enButton(2) = vbCancel
            m_enButton(3) = 0
            
            nBtnCount = 2
            
            btnAction(1).Caption = IIf(Len(sBtnText1) <= 0, "OK", sBtnText1)
            btnAction(1).Default = True
            btnAction(1).Visible = True
            
            btnAction(2).Caption = IIf(Len(sBtnText2) <= 0, "Cancel", sBtnText2)
            btnAction(2).Cancel = True
            btnAction(2).Visible = True
            
            btnAction(3).Visible = False
            
        Case vbYesNo
            m_enButton(1) = vbYes
            m_enButton(2) = vbNo
            m_enButton(3) = 0
            
            nBtnCount = 2
            
            btnAction(1).Caption = IIf(Len(sBtnText1) <= 0, "&Yes", sBtnText1)
            btnAction(1).Default = True
            btnAction(1).Visible = True
            
            btnAction(2).Caption = IIf(Len(sBtnText2) <= 0, "&No", sBtnText2)
            btnAction(2).Cancel = True
            btnAction(2).Visible = True
            
            btnAction(3).Visible = False
            
        Case vbYesNoCancel
            m_enButton(1) = vbYes
            m_enButton(2) = vbNo
            m_enButton(3) = vbCancel
            
            nBtnCount = 3
            
            btnAction(1).Caption = IIf(Len(sBtnText1) <= 0, "&Yes", sBtnText1)
            btnAction(1).Default = True
            btnAction(1).Visible = True
            
            btnAction(2).Caption = IIf(Len(sBtnText2) <= 0, "&No", sBtnText2)
            btnAction(2).Visible = True
            
            btnAction(3).Caption = IIf(Len(sBtnText3) <= 0, "Cancel", sBtnText3)
            btnAction(3).Cancel = True
            btnAction(3).Visible = True
            
        Case Else
            m_enButton(1) = vbOK
            m_enButton(2) = 0
            m_enButton(3) = 0
            
            nBtnCount = 1
            
            btnAction(1).Caption = IIf(Len(sBtnText1) <= 0, "OK", sBtnText1)
            btnAction(1).Default = True
            btnAction(1).Cancel = True
            btnAction(1).Visible = True
            
            btnAction(2).Visible = False
            btnAction(3).Visible = False
            
    End Select
     
    For i = 1 To MAX_BUTTONS
        btnAction(i).Visible = (m_enButton(i) <> 0)
    Next
    
    Me.Width = nWidth + ScaleX(GetSystemMetrics(SM_CXSIZEFRAME) * 2, vbPixels, vbTwips)
    
    nLeft = (Me.ScaleWidth - (btnAction(1).Width * nBtnCount + 120 * (nBtnCount - 1))) / 2
    
    For i = 1 To nBtnCount
        btnAction(i).Left = nLeft + (btnAction(1).Width + 120) * (i - 1)
        btnAction(i).Top = nButtonTop
    Next
    
    Me.Height = btnAction(1).Top + btnAction(1).Height + 120 + ScaleY(GetSystemMetrics(SM_CYSIZEFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION), vbPixels, vbTwips)
    
    'Me.Left = (ScaleX(GetSystemMetrics(SM_CXFULLSCREEN), vbPixels, vbTwips) - Me.Width) / 2
    'Me.Top = (ScaleY(GetSystemMetrics(SM_CYFULLSCREEN), vbPixels, vbTwips) - Me.Height) / 2
End Sub


