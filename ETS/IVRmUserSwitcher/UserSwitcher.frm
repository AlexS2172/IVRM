VERSION 5.00
Begin VB.Form frmChangeUser 
   Caption         =   "Change Group"
   ClientHeight    =   1950
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   4680
   Icon            =   "UserSwitcher.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   1950
   ScaleWidth      =   4680
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame Frame1 
      Height          =   1095
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4455
      Begin VB.ComboBox cbGroup 
         Height          =   315
         ItemData        =   "UserSwitcher.frx":15162
         Left            =   1440
         List            =   "UserSwitcher.frx":15164
         Style           =   2  'Dropdown List
         TabIndex        =   4
         Top             =   600
         Width           =   2775
      End
      Begin VB.Label lblCurrentUserName 
         Height          =   255
         Left            =   1440
         TabIndex        =   6
         Top             =   240
         Width           =   2775
      End
      Begin VB.Label lblCurrentUser 
         Caption         =   "Current User:"
         Height          =   255
         Left            =   240
         TabIndex        =   5
         Top             =   240
         Width           =   975
      End
      Begin VB.Label lbUser 
         Caption         =   "Current Group:"
         Height          =   255
         Left            =   120
         TabIndex        =   3
         Top             =   600
         Width           =   1215
      End
   End
   Begin VB.CommandButton btCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   2400
      TabIndex        =   1
      Top             =   1440
      Width           =   1335
   End
   Begin VB.CommandButton btApply 
      Caption         =   "Apply"
      Height          =   375
      Left            =   840
      TabIndex        =   0
      Top             =   1440
      Width           =   1335
   End
End
Attribute VB_Name = "frmChangeUser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

 Dim sUserName$, sUserGroup$

Private Sub btApply_Click()
    If Len(cbGroup.Text) > 0 And cbGroup.Text <> sUserGroup Then
    ' save new group Data
        Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
        Set g_aMainXMLParams = aXMLLoader.Load(MainXMLFilePath)
        If g_aMainXMLParams Is Nothing Then
            Set g_aMainXMLParams = New ETSXMLParamsLib.XMLParams
        End If
        g_aMainXMLParams.SetStringValue PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", cbGroup.Text
        aXMLLoader.Save MainXMLFilePath, g_aMainXMLParams
    End If
    Unload Me
End Sub

Private Sub btCancel_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub Form_Load()
On Error GoTo ex
    Dim aXMLLoader As New ETSXMLParamsLib.XMLLoader
    Set g_aMainXMLParams = aXMLLoader.Load(MainXMLFilePath)
    If g_aMainXMLParams Is Nothing Then
        Set g_aMainXMLParams = New ETSXMLParamsLib.XMLParams
    End If
    
    sUserName = CurrentNtUserName
    
    If Len(sUserName) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user name."
    End If
    
    sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", "")
    If Len(sUserGroup) <= 0 Then
        Err.Raise vbObjectError + 1023, "Application Settings", "Fail to get current user ETS group."
    End If
    
    Set g_aMainXMLParams = Nothing
    lblCurrentUserName.Caption = sUserName
    FillGroups sUserGroup
    
    Exit Sub
   
ex:
    btApply.Enabled = False
    cbGroup.Clear

End Sub

Private Sub FillGroups(sCurrentGroup As String)
        Dim oXMLDom As MSXML2.DOMDocument40
        Dim XMLNodes As MSXML2.IXMLDOMNodeList
        Dim XMLNode As MSXML2.IXMLDOMNode
        Dim iPos As Integer
        Dim iCurrentPos As Integer
    
        iPos = -1
        iCurrentPos = 0
        Set oXMLDom = New MSXML2.DOMDocument40
        
        oXMLDom.async = False
        oXMLDom.validateOnParse = False
        oXMLDom.resolveExternals = False
        oXMLDom.preserveWhiteSpace = True

        oXMLDom.Load MainXMLFilePath
       Set XMLNodes = oXMLDom.selectNodes("ETS/Asp/Groups/*[Parameter [@Name = ""Visible""][@Value =""1""]]")
       
       If XMLNodes.length > 0 Then
            cbGroup.Enabled = True
            For Each XMLNode In XMLNodes
                If XMLNode.nodeName = sCurrentGroup Then
                    iPos = iCurrentPos
                End If
                cbGroup.AddItem XMLNode.nodeName, iCurrentPos
                iCurrentPos = iCurrentPos + 1
            Next
       Else
            cbGroup.Enabled = False
            btApply.Enabled = False
       End If
       If iPos >= 0 Then cbGroup.ListIndex = iPos
       
       
        
End Sub
