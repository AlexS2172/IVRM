VERSION 5.00
Object = "{C932BA88-4374-101B-A56C-00AA003668DC}#1.1#0"; "msmask32.ocx"
Begin VB.UserControl ctlEditEx 
   ClientHeight    =   3600
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4800
   DataBindingBehavior=   1  'vbSimpleBound
   ScaleHeight     =   3600
   ScaleWidth      =   4800
   Begin MSMask.MaskEdBox txtMask 
      Height          =   375
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2385
      _ExtentX        =   4207
      _ExtentY        =   661
      _Version        =   393216
      PromptChar      =   "_"
   End
   Begin VB.TextBox txtText 
      Height          =   345
      Left            =   570
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   420
      Width           =   2385
   End
End
Attribute VB_Name = "ctlEditEx"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_enAllowedKeys As ExAllowedKeysEnum
Private m_bAutoSelectAll As Boolean
Private m_bHasFocus As Boolean

Event Click()
Event DblClick()
Event KeyDown(KeyCode As Integer, Shift As Integer)
Event KeyPress(KeyAscii As Integer)
Event KeyUp(KeyCode As Integer, Shift As Integer)
Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
Event OLEDragOver(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single, State As Integer)
Event OLEDragDrop(Data As DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
Event OLEGiveFeedback(Effect As Long, DefaultCursors As Boolean)
Event OLEStartDrag(Data As DataObject, AllowedEffects As Long)
Event OLESetData(Data As DataObject, DataFormat As Integer)
Event OLECompleteDrag(Effect As Long)
Event Change()
Event ValidationError(InvalidText As String, StartPosition As Integer)
Event Validation(Cancel As Boolean)

Public Enum ExAllowedKeysEnum
    enExAkAll = 0
    
    enExAkNoSpaces = 2 ^ 0
    enExAkNoSingleQuotes = 2 ^ 1
    enExAkNoDoubleQuotes = 2 ^ 2
    
    enExAkUppercase = 2 ^ 3
    enExAkLowercase = 2 ^ 4
    
    enExAkAllowDecimal = 2 ^ 5
    enExAkAllowMinus = 2 ^ 6                ' allow minus at any place
    enExAkAllowSpaces = 2 ^ 7
    enExAkAllowStars = 2 ^ 8
    enExAkAllowPounds = 2 ^ 9
    enExAkAllowForwardSlash = 2 ^ 10
    enExAkAllowParenthesis = 2 ^ 11
    enExAkAllowDollarSigns = 2 ^ 12
    enExAkAllowColon = 2 ^ 13
    enExAkAllowAMPM = 2 ^ 14
    enExAkAllowNegative = 2 ^ 15            ' allow minus at first place only
    
    enExAkOnlyNumbers = 2 ^ 16
    
    enExAkOnlyPhone = enExAkOnlyNumbers + enExAkAllowParenthesis + enExAkAllowSpaces + enExAkAllowMinus + enExAkAllowPounds
    enExAkOnlyDate = enExAkOnlyNumbers + enExAkAllowForwardSlash
    enExAkOnlyMoney = enExAkOnlyNumbers + enExAkAllowDollarSigns + enExAkAllowNegative + enExAkAllowDecimal
    enExAkOnlyTime = enExAkOnlyNumbers + enExAkAllowColon + enExAkAllowSpaces + enExAkAllowAMPM
    enExAkOnlyNumbersWithDecimals = enExAkOnlyNumbers + enExAkAllowDecimal
    enExAkOnlyNumbersWithDecimalsAndNegative = enExAkOnlyNumbers + enExAkAllowDecimal + enExAkAllowNegative
End Enum

Public Property Get AutoSelectAll() As Boolean
    AutoSelectAll = m_bAutoSelectAll
End Property

Public Property Let AutoSelectAll(ByVal bAuto As Boolean)
    m_bAutoSelectAll = bAuto
    PropertyChanged "AutoSelectAll"
End Property

Private Function KeyNotAllowed(ByVal KeyAscii As Integer, ByVal enAllowed As ExAllowedKeysEnum) As Boolean
    KeyNotAllowed = True
    Dim sChar As String
    sChar = Chr(KeyAscii)
    Select Case True
        Case (enAllowed And enExAkNoDoubleQuotes) And sChar = """"
        Case (enAllowed And enExAkNoSingleQuotes) And sChar = "'"
        Case (enAllowed And enExAkNoSpaces) And sChar = " "
        Case Else
            KeyNotAllowed = False
    End Select
End Function

Private Function KeyAllowed(ByVal KeyAscii As Integer, ByVal enAllowed As ExAllowedKeysEnum) As Boolean
    KeyAllowed = True
    Dim sChar As String
    sChar = Chr(KeyAscii)
    Select Case True
        Case (enAllowed And enExAkAllowAMPM) And InStr("AMP", UCase(sChar)) > 0
        Case (enAllowed And enExAkAllowColon) And sChar = ":"
        Case (enAllowed And enExAkAllowDecimal) And sChar = "." And InStr(GetUnselectedText(txtMask), ".") = 0
        Case (enAllowed And enExAkAllowDollarSigns) And sChar = "$"
        Case (enAllowed And enExAkAllowForwardSlash) And sChar = "/"
        Case (enAllowed And enExAkAllowMinus) And sChar = "-"
        Case (enAllowed And enExAkAllowNegative) And sChar = "-" And txtMask.SelStart = 0 And InStr(GetUnselectedText(txtMask), "-") = 0
        Case (enAllowed And enExAkAllowParenthesis) And InStr("()", sChar) > 0
        Case (enAllowed And enExAkAllowPounds) And sChar = "#"
        Case (enAllowed And enExAkAllowSpaces) And sChar = " "
        Case (enAllowed And enExAkAllowStars) And sChar = "*"
        Case Else
            KeyAllowed = False
    End Select
End Function

Private Function OnKeyPress(ByVal KeyAscii As KeyCodeConstants, ByVal enAllowed As ExAllowedKeysEnum) As Integer
    ' Allow All System Keys Through
    If KeyAscii < 32 Then
        
        ' AutoTab
        If txtMask.AutoTab And KeyAscii = 13 Then
            KeyAscii = 0
            SendKeys "{TAB}"
        End If
        
        OnKeyPress = KeyAscii
        Exit Function
    End If
    
    ' Uliminate In-Eligible Keystrokes
    Select Case True
        Case KeyAllowed(KeyAscii, enAllowed)
        Case KeyNotAllowed(KeyAscii, enAllowed)
            KeyAscii = 0
        Case (enAllowed And enExAkOnlyNumbers) And (KeyAscii < vbKey0 Or KeyAscii > vbKey9)
            KeyAscii = 0
    End Select
    
    ' Coerce values
    Select Case True
        Case (enAllowed And enExAkLowercase)
            KeyAscii = Asc(LCase(Chr(KeyAscii)))
        Case (enAllowed And enExAkUppercase)
            KeyAscii = Asc(UCase(Chr(KeyAscii)))
    End Select
    
    OnKeyPress = KeyAscii
End Function

Public Property Get AllowedKeys() As ExAllowedKeysEnum
    AllowedKeys = m_enAllowedKeys
End Property

Public Property Let AllowedKeys(ByVal eKeys As ExAllowedKeysEnum)
    m_enAllowedKeys = eKeys
    PropertyChanged "AllowedKeys"
End Property

Public Property Get Alignment() As AlignmentConstants
    Alignment = txtText.Alignment
End Property

Public Property Let Alignment(ByVal eAlignment As AlignmentConstants)
    txtText.Alignment = eAlignment
    PropertyChanged "Alignment"
End Property

Private Sub txtMask_GotFocus()
    m_bHasFocus = True
    HideMaskedEdit False
    If m_bAutoSelectAll Then
        txtMask.SelStart = 0
        txtMask.SelLength = Len(txtMask.FormattedText)
    End If
End Sub

Private Sub txtMask_LostFocus()
    m_bHasFocus = False
    ApplyMask
    HideMaskedEdit True
End Sub

Public Function HideMaskedEdit(bHide As Boolean)
    If bHide Then
        txtMask.Move -txtMask.Width, -txtMask.Height
    Else
        txtMask.Move 0, 0
    End If
End Function

Private Sub txtMask_OLEDragDrop(Data As MSMask.DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent OLEDragDrop(Data, Effect, Button, Shift, X, Y)
End Sub

Private Sub txtMask_OLEDragOver(Data As MSMask.DataObject, Effect As Long, Button As Integer, Shift As Integer, X As Single, Y As Single, State As Integer)
    On Error Resume Next
    RaiseEvent OLEDragOver(Data, Effect, Button, Shift, X, Y, State)
End Sub

Private Sub txtMask_OLEStartDrag(Data As MSMask.DataObject, AllowedEffects As Long)
    On Error Resume Next
    RaiseEvent OLEStartDrag(Data, AllowedEffects)
End Sub

Private Sub txtText_GotFocus()
    On Error Resume Next
    m_bHasFocus = True
    HideMaskedEdit False
    ctlSetFocus txtMask
End Sub

Private Sub txtText_LostFocus()
    On Error Resume Next
    m_bHasFocus = False
End Sub

Private Sub txtText_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub txtText_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub txtText_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

Private Sub UserControl_GotFocus()
    On Error Resume Next
    m_bHasFocus = True
    HideMaskedEdit False
    ctlSetFocus txtMask
End Sub

Private Sub UserControl_Initialize()
    On Error Resume Next
    HideMaskedEdit True
    m_bAutoSelectAll = True
End Sub

Private Sub UserControl_InitProperties()
    On Error Resume Next
    m_enAllowedKeys = enExAkAll
    m_bAutoSelectAll = True
End Sub

Private Sub UserControl_LostFocus()
    On Error Resume Next
    m_bHasFocus = False
    HideMaskedEdit True
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    On Error Resume Next
    Me.BackColor = PropBag.ReadProperty("BackColor", &H80000005)
    Me.ForeColor = PropBag.ReadProperty("ForeColor", &H80000008)
    Me.Enabled = PropBag.ReadProperty("Enabled", True)
    Me.MousePointer = PropBag.ReadProperty("MousePointer", 0)
    Set Me.MouseIcon = PropBag.ReadProperty("MouseIcon", Nothing)
    Set Me.Font = PropBag.ReadProperty("Font", Ambient.Font)
    Me.Appearance = PropBag.ReadProperty("Appearance", 1)
    Me.OLEDropMode = PropBag.ReadProperty("OLEDropMode", 0)
    Me.BackStyle = PropBag.ReadProperty("BackStyle", 1)
    Me.BorderStyle = PropBag.ReadProperty("BorderStyle", 1)
    Me.ClipMode = PropBag.ReadProperty("ClipMode", 0)
    Me.PromptInclude = PropBag.ReadProperty("PromptInclude", True)
    Me.AllowPrompt = PropBag.ReadProperty("AllowPrompt", False)
    Me.AutoTab = PropBag.ReadProperty("AutoTab", False)
    Me.MaxLength = PropBag.ReadProperty("MaxLength", 64)
    Me.Format = PropBag.ReadProperty("Format", "")
    Me.Mask = PropBag.ReadProperty("Mask", "")
    Me.PromptChar = PropBag.ReadProperty("PromptChar", "_")
    Me.OLEDragMode = PropBag.ReadProperty("OLEDragMode", 0)
    Me.WhatsThisHelpID = PropBag.ReadProperty("WhatsThisHelpID", 0)
    txtMask.CausesValidation = PropBag.ReadProperty("CausesValidate", True)
    Me.ToolTipText = PropBag.ReadProperty("ToolTipText", "")
    Me.SelLength = PropBag.ReadProperty("SelLength", 0)
    Me.SelStart = PropBag.ReadProperty("SelStart", 0)
    Me.SelText = PropBag.ReadProperty("SelText", "")
    Me.Text = PropBag.ReadProperty("Text", "")
    Me.HideSelection = PropBag.ReadProperty("HideSelection", False)
    Me.Locked = PropBag.ReadProperty("Locked", False)
    Me.Alignment = PropBag.ReadProperty("Alignment", 0)
    m_enAllowedKeys = PropBag.ReadProperty("AllowedKeys", 0)
    m_bAutoSelectAll = PropBag.ReadProperty("AutoSelectAll", True)
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If txtMask.Left >= 0 Then
        txtMask.Move txtMask.Left, txtMask.Top, UserControl.Width, UserControl.Height
    Else
        txtMask.Move txtMask.Left - (UserControl.Width - txtMask.Left), _
                    txtMask.Top - (UserControl.Height - txtMask.Height), _
                        UserControl.Width, UserControl.Height
    End If
    txtText.Move 0, 0, UserControl.Width, UserControl.Height
End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
    On Error Resume Next
    PropBag.WriteProperty "BackColor", txtMask.BackColor, &H80000005
    PropBag.WriteProperty "ForeColor", txtMask.ForeColor, &H80000008
    PropBag.WriteProperty "Enabled", txtMask.Enabled, True
    PropBag.WriteProperty "MousePointer", txtMask.MousePointer, 0
    PropBag.WriteProperty "MouseIcon", MouseIcon, Nothing
    PropBag.WriteProperty "Font", txtMask.Font, Ambient.Font
    PropBag.WriteProperty "Appearance", txtMask.Appearance, 1
    PropBag.WriteProperty "OLEDropMode", txtMask.OLEDropMode, 0
    PropBag.WriteProperty "BackStyle", UserControl.BackStyle, 1
    PropBag.WriteProperty "BorderStyle", txtMask.BorderStyle, 1
    PropBag.WriteProperty "ClipMode", txtMask.ClipMode, 0
    PropBag.WriteProperty "PromptInclude", txtMask.PromptInclude, True
    PropBag.WriteProperty "AllowPrompt", txtMask.AllowPrompt, False
    PropBag.WriteProperty "AutoTab", txtMask.AutoTab, False
    PropBag.WriteProperty "MaxLength", txtMask.MaxLength, 64
    PropBag.WriteProperty "Format", txtMask.Format, ""
    PropBag.WriteProperty "Mask", txtMask.Mask, ""
    PropBag.WriteProperty "PromptChar", txtMask.PromptChar, "_"
    PropBag.WriteProperty "OLEDragMode", txtMask.OLEDragMode, 0
    PropBag.WriteProperty "WhatsThisHelpID", txtMask.WhatsThisHelpID, 0
    PropBag.WriteProperty "CausesValidate", txtMask.CausesValidation, True
    PropBag.WriteProperty "ToolTipText", txtMask.ToolTipText, ""
    PropBag.WriteProperty "SelLength", txtMask.SelLength, 0
    PropBag.WriteProperty "SelStart", txtMask.SelStart, 0
    PropBag.WriteProperty "SelText", txtMask.SelText, ""
    PropBag.WriteProperty "FormattedText", txtMask.FormattedText, ""
    PropBag.WriteProperty "Text", txtMask.Text, ""
    PropBag.WriteProperty "HideSelection", txtMask.HideSelection, False
    PropBag.WriteProperty "Locked", txtText.Locked, False
    PropBag.WriteProperty "Alignment", txtText.Alignment, 0
    PropBag.WriteProperty "AllowedKeys", m_enAllowedKeys, 0
    PropBag.WriteProperty "AutoSelectAll", m_bAutoSelectAll, True
End Sub

Public Property Get hwnd() As Long
    hwnd = UserControl.hwnd
End Property

Public Property Get BackColor() As OLE_COLOR
    BackColor = txtMask.BackColor
End Property

Public Property Let BackColor(ByVal NewValue As OLE_COLOR)
    txtMask.BackColor = NewValue
    txtText.BackColor = NewValue
    PropertyChanged "BackColor"
End Property

Public Property Get ForeColor() As OLE_COLOR
    ForeColor = txtMask.ForeColor
End Property

Public Property Let ForeColor(ByVal NewValue As OLE_COLOR)
    txtMask.ForeColor = NewValue
    txtText.ForeColor = NewValue
    PropertyChanged "ForeColor"
End Property

Public Property Get Enabled() As Boolean
    Enabled = txtMask.Enabled
End Property

Public Property Let Enabled(ByVal NewValue As Boolean)
    txtMask.Enabled = NewValue
    txtText.Enabled = NewValue
    PropertyChanged "Enabled"
End Property

Public Property Get MousePointer() As MousePointerConstants
    MousePointer = txtMask.MousePointer
End Property

Public Property Let MousePointer(ByVal NewValue As MousePointerConstants)
    txtMask.MousePointer = NewValue
    txtText.MousePointer = NewValue
    PropertyChanged "MousePointer"
End Property

Public Property Get MouseIcon() As Picture
    Set MouseIcon = txtMask.MouseIcon
End Property

Public Property Set MouseIcon(ByVal NewValue As Picture)
    Set txtMask.MouseIcon = NewValue
    Set txtText.MouseIcon = NewValue
    PropertyChanged "MouseIcon"
End Property

Public Property Get Font() As Font
    Set Font = txtMask.Font
End Property

Public Property Set Font(ByVal NewValue As Font)
    Set txtMask.Font = NewValue
    Set txtText.Font = NewValue
    PropertyChanged "Font"
End Property

Public Property Get Appearance() As AppearanceConstants
    Appearance = txtMask.Appearance
End Property

Public Property Let Appearance(ByVal NewValue As AppearanceConstants)
    txtMask.Appearance = NewValue
    txtText.Appearance = NewValue
    PropertyChanged "Appearance"
End Property

Public Property Get OLEDropMode() As OLEDropConstants
    OLEDropMode = txtMask.OLEDropMode
End Property

Public Property Let OLEDropMode(ByVal NewValue As OLEDropConstants)
    txtMask.OLEDropMode = NewValue
    PropertyChanged "OLEDropMode"
End Property

Public Property Get BackStyle() As Integer
    BackStyle = UserControl.BackStyle
End Property

Public Property Let BackStyle(ByVal NewValue As Integer)
    UserControl.BackStyle = NewValue
    PropertyChanged "BackStyle"
End Property

Public Property Get BorderStyle() As MSMask.BorderStyleConstants
    BorderStyle = txtMask.BorderStyle
End Property

Public Property Let BorderStyle(ByVal NewValue As MSMask.BorderStyleConstants)
    txtMask.BorderStyle = NewValue
    txtText.BorderStyle = NewValue
    PropertyChanged "BorderStyle"
End Property

Public Sub Refresh()
    txtMask.Refresh
End Sub

Public Sub OLEDrag()
    txtMask.OLEDrag
End Sub

Private Sub UserControl_Click()
    On Error Resume Next
    RaiseEvent Click
End Sub

Private Sub UserControl_DblClick()
    On Error Resume Next
    RaiseEvent DblClick
End Sub

Private Sub txtMask_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub txtMask_KeyPress(KeyAscii As Integer)
    On Error Resume Next
    If txtText.Locked Then
        KeyAscii = 0
    End If
    KeyAscii = OnKeyPress(KeyAscii, m_enAllowedKeys)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub txtMask_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

Private Sub txtMask_OLEGiveFeedback(Effect As Long, DefaultCursors As Boolean)
    On Error Resume Next
    RaiseEvent OLEGiveFeedback(Effect, DefaultCursors)
End Sub

Private Sub txtMask_OLESetData(Data As MSMask.DataObject, DataFormat As Integer)
    On Error Resume Next
    RaiseEvent OLESetData(Data, DataFormat)
End Sub

Private Sub txtMask_OLECompleteDrag(Effect As Long)
    On Error Resume Next
    RaiseEvent OLECompleteDrag(Effect)
End Sub

Public Property Get ClipMode() As ClipModeConstants
    ClipMode = txtMask.ClipMode
End Property

Public Property Let ClipMode(ByVal NewValue As ClipModeConstants)
    txtMask.ClipMode = NewValue
    PropertyChanged "ClipMode"
End Property

Public Property Get PromptInclude() As Boolean
    PromptInclude = txtMask.PromptInclude
End Property

Public Property Let PromptInclude(ByVal NewValue As Boolean)
    txtMask.PromptInclude = NewValue
    PropertyChanged "PromptInclude"
End Property

Public Property Get AllowPrompt() As Boolean
    AllowPrompt = txtMask.AllowPrompt
End Property

Public Property Let AllowPrompt(ByVal NewValue As Boolean)
    txtMask.AllowPrompt = NewValue
    PropertyChanged "AllowPrompt"
End Property

Public Property Get AutoTab() As Boolean
    AutoTab = txtMask.AutoTab
End Property

Public Property Let AutoTab(ByVal NewValue As Boolean)
    txtMask.AutoTab = NewValue
    PropertyChanged "AutoTab"
End Property

Public Property Get MaxLength() As Integer
    MaxLength = txtMask.MaxLength
End Property

Public Property Let MaxLength(ByVal NewValue As Integer)
    txtMask.MaxLength = NewValue
    txtText.MaxLength = NewValue
    PropertyChanged "MaxLength"
End Property

Public Property Get Format() As String
    Format = txtMask.Format
End Property

Public Property Let Format(ByVal NewValue As String)
    txtMask.Format = NewValue
    ApplyMask
    PropertyChanged "Format"
End Property

Public Property Get Mask() As String
    Mask = txtMask.Mask
End Property

Public Property Let Mask(ByVal NewValue As String)
    txtMask.Mask = NewValue
    PropertyChanged "Mask"
End Property

Public Property Get PromptChar() As String
    PromptChar = txtMask.PromptChar
End Property

Public Property Let PromptChar(ByVal NewValue As String)
    txtMask.PromptChar() = NewValue
    PropertyChanged "PromptChar"
End Property

Public Property Get OLEDragMode() As OLEDragConstants
    OLEDragMode = txtMask.OLEDragMode
End Property

Public Property Let OLEDragMode(ByVal NewValue As OLEDragConstants)
    txtMask.OLEDragMode = NewValue
    PropertyChanged "OLEDragMode"
End Property

Public Property Get WhatsThisHelpID() As Long
    WhatsThisHelpID = txtMask.WhatsThisHelpID
End Property

Public Property Let WhatsThisHelpID(ByVal NewValue As Long)
    txtMask.WhatsThisHelpID = NewValue
    txtText.WhatsThisHelpID = NewValue
    PropertyChanged "WhatsThisHelpID"
End Property

Public Property Get CausesValidate() As Boolean
    CausesValidate = txtMask.CausesValidation
End Property

Public Property Let CausesValidate(ByVal NewValue As Boolean)
    txtMask.CausesValidation = NewValue
    PropertyChanged "CausesValidate"
End Property

Public Property Get ToolTipText() As String
    ToolTipText = txtMask.ToolTipText
End Property

Public Property Let ToolTipText(ByVal NewValue As String)
    txtMask.ToolTipText = NewValue
    PropertyChanged "ToolTipText"
End Property

Private Function ApplyMask()
    On Error Resume Next
    If Len(txtMask.FormattedText) > txtText.MaxLength And txtText.MaxLength > 0 Then
        Me.MaxLength = Len(txtMask.FormattedText)
    End If
    txtText.Text = txtMask.FormattedText
End Function

Private Sub txtMask_Change()
    On Error Resume Next
    ApplyMask
    If m_bHasFocus Then
        txtMask.DataChanged = True
        Extender.DataChanged = True
        RaiseEvent Change
    End If
End Sub

Private Sub txtMask_ValidationError(InvalidText As String, StartPosition As Integer)
    On Error Resume Next
    RaiseEvent ValidationError(InvalidText, StartPosition)
End Sub

Private Sub txtMask_Validate(Cancel As Boolean)
    On Error Resume Next
    RaiseEvent Validation(Cancel)
End Sub

Public Property Get SelLength() As Integer
    SelLength = txtMask.SelLength
End Property

Public Property Let SelLength(ByVal NewValue As Integer)
    txtMask.SelLength = NewValue
    PropertyChanged "SelLength"
End Property

Public Property Get SelStart() As Integer
    SelStart = txtMask.SelStart
End Property

Public Property Let SelStart(ByVal NewValue As Integer)
    txtMask.SelStart = NewValue
    PropertyChanged "SelStart"
End Property

Public Property Get SelText() As String
    SelText = txtMask.SelText
End Property

Public Property Let SelText(ByVal NewValue As String)
    txtMask.SelText = NewValue
    PropertyChanged "SelText"
End Property

Public Property Get FormattedText() As String
    FormattedText = txtMask.FormattedText
End Property

Public Property Get Text() As String
    Text = txtMask.Text
End Property

Public Property Let Text(ByVal NewValue As String)
    On Error Resume Next
    If Trim(NewValue) = "" Then
        If txtMask.Mask <> "" Then
            txtMask.Text = Replace(txtMask.Mask, "#", txtMask.PromptChar)
        Else
            txtMask.Text = Empty
        End If
    Else
        txtMask.Text = NewValue
    End If

    If m_bHasFocus Then
        UserControl.PropertyChanged "Text"
    End If
End Property

Public Property Get ClipText() As String
    ClipText = txtMask.ClipText
End Property

Public Property Get HideSelection() As Boolean
    HideSelection = txtMask.HideSelection
End Property

Public Property Let HideSelection(ByVal NewValue As Boolean)
    txtMask.HideSelection = NewValue
    PropertyChanged "HideSelection"
End Property

Public Property Get Locked() As Boolean
    Locked = txtText.Locked
End Property

Public Property Let Locked(ByVal NewValue As Boolean)
    txtText.Locked = NewValue
    PropertyChanged "Locked"
End Property

Private Function GetUnselectedText(txt As MaskEdBox) As String
    On Error Resume Next
    If txt.SelLength > 0 Then
        GetUnselectedText = Left$(txt.Text, txt.SelStart) & Mid$(txt.Text, txt.SelStart + txt.SelLength + 1)
    Else
        GetUnselectedText = txt.Text
    End If
End Function

Private Function ctlSetFocus(ByRef objToSetFocusTo As Object) As Boolean
    On Error Resume Next
    objToSetFocusTo.SetFocus
    ctlSetFocus = Err.Number = 0
    Err.Clear
End Function

Public Sub SetFocus()
    UserControl.SetFocus
End Sub



