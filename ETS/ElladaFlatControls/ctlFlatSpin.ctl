VERSION 5.00
Begin VB.UserControl FlatSpin 
   Appearance      =   0  'Flat
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   1785
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3000
   KeyPreview      =   -1  'True
   ScaleHeight     =   1785
   ScaleWidth      =   3000
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   1320
      ScaleHeight     =   495
      ScaleWidth      =   495
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   240
      Width           =   495
   End
   Begin VB.TextBox Text1 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      Height          =   285
      Left            =   120
      MaxLength       =   10
      TabIndex        =   0
      Text            =   "Text1"
      Top             =   240
      Width           =   1095
   End
End
Attribute VB_Name = "FlatSpin"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'**************************************************
' Flat Spin Control
' Created by Andrei Shchurov (shurov@egartech.com)
' October 25, 2000
'
' Major properties:
' Value - value of control
' Text - text of control
' Min - minimun possible value
' Max - maximun possible value
' Increment - spin increment
'**************************************************
Option Explicit

'Default Property Values:
Const m_def_Increment = 1
Const m_def_Min = 0
Const m_def_Max = 1000
Const m_def_ButtonWidth = 200
Const m_def_Enabled = True
Const m_def_ForeColor = 0
Const m_def_Value = 0

'Property Variables:
Dim m_Increment As Long
Dim m_Min As Long
Dim m_Max As Long
Dim m_ButtonWidth As Long
Dim m_Enabled As Boolean
Dim m_ForeColor As OLE_COLOR

'Event Declarations:
Event Change() 'MappingInfo=Text1,Text1,-1,Change
Attribute Change.VB_Description = "Occurs when the contents of a control have changed."
Event Click() 'MappingInfo=Text1,Text1,-1,Click
Attribute Click.VB_Description = "Occurs when the user presses and then releases a mouse button over an object."
Event DblClick() 'MappingInfo=Text1,Text1,-1,DblClick
Attribute DblClick.VB_Description = "Occurs when the user presses and releases a mouse button and then presses and releases it again over an object."
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=Text1,Text1,-1,KeyDown
Attribute KeyDown.VB_Description = "Occurs when the user presses a key while an object has the focus."
Event KeyPress(KeyAscii As Integer) 'MappingInfo=Text1,Text1,-1,KeyPress
Attribute KeyPress.VB_Description = "Occurs when the user presses and releases an ANSI key."
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=Text1,Text1,-1,KeyUp
Attribute KeyUp.VB_Description = "Occurs when the user releases a key while an object has the focus."
Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseDown
Attribute MouseDown.VB_Description = "Occurs when the user presses the mouse button while an object has the focus."
Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseMove
Attribute MouseMove.VB_Description = "Occurs when the user moves the mouse."
Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseUp
Attribute MouseUp.VB_Description = "Occurs when the user releases the mouse button while an object has the focus."




'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Alignment
Public Property Get Alignment() As Integer
Attribute Alignment.VB_Description = "Returns/sets the alignment of a CheckBox or OptionButton, or a control's text."
    Alignment = Text1.Alignment
End Property

Public Property Let Alignment(ByVal New_Alignment As Integer)
    Text1.Alignment() = New_Alignment
    Refresh
    PropertyChanged "Alignment"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
    BackColor = Text1.BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
    Text1.BackColor() = New_BackColor
    UserControl.BackColor = New_BackColor
    Refresh
    PropertyChanged "BackColor"
End Property

Private Sub Text1_Change()
    RaiseEvent Change
End Sub

Private Sub Text1_Click()
    RaiseEvent Click
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
    UserControl.Enabled = New_Enabled
    Text1.Enabled = New_Enabled
    Refresh
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Font
Public Property Get Font() As Font
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
    Set Font = Text1.Font
End Property

Public Property Set Font(ByVal New_Font As Font)
    Set Text1.Font = New_Font
    Refresh
    PropertyChanged "Font"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
    ForeColor = m_ForeColor
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
    m_ForeColor = New_ForeColor
    Refresh
    PropertyChanged "ForeColor"
End Property

Private Sub Text1_GotFocus()
    Text1.SelStart = 0
    Text1.SelLength = Len(Text1.Text)
End Sub

Private Sub Text1_KeyDown(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub Text1_KeyPress(KeyAscii As Integer)
    If KeyAscii < vbKey0 Or KeyAscii > vbKey9 Then
        KeyAscii = 0
    End If
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub Text1_KeyUp(KeyCode As Integer, Shift As Integer)
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Locked
Public Property Get Locked() As Boolean
Attribute Locked.VB_Description = "Determines whether a control can be edited."
    Locked = Text1.Locked
End Property

Public Property Let Locked(ByVal New_Locked As Boolean)
    Text1.Locked() = New_Locked
    Refresh
    PropertyChanged "Locked"
End Property

Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Y < Height / 2 Then
        IncrementValue 1
    Else
        IncrementValue -1
    End If
    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Text
Public Property Get Text() As String
Attribute Text.VB_Description = "Returns/sets the text contained in the control."
Attribute Text.VB_UserMemId = 0
Attribute Text.VB_MemberFlags = "200"
    Text = Text1.Text
End Property

Public Property Let Text(ByVal New_Text As String)
    Text1.Text() = New_Text
    Refresh
    PropertyChanged "Text"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Picture1,Picture1,-1,BackColor
Public Property Get ButtonBackColor() As OLE_COLOR
Attribute ButtonBackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
    ButtonBackColor = Picture1.BackColor
End Property

Public Property Let ButtonBackColor(ByVal New_ButtonBackColor As OLE_COLOR)
    Picture1.BackColor() = New_ButtonBackColor
    Refresh
    PropertyChanged "ButtonBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Value() As Long
On Error GoTo E
    Value = CLng(Text1.Text)
Exit Property
E:
    Value = 0
End Property

Public Property Let Value(ByVal New_Value As Long)
    Text1.Text = New_Value
    PropertyChanged "Value"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get ButtonWidth() As Long
    ButtonWidth = m_ButtonWidth
End Property

Public Property Let ButtonWidth(ByVal New_ButtonWidth As Long)
    m_ButtonWidth = New_ButtonWidth
    Refresh
    PropertyChanged "ButtonWidth"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Min() As Long
    Min = m_Min
End Property

Public Property Let Min(ByVal New_Min As Long)
    m_Min = New_Min
    If m_Min > m_Max Then m_Min = m_Max
    CheckMinMax
    PropertyChanged "Min"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Max() As Long
    Max = m_Max
End Property

Public Property Let Max(ByVal New_Max As Long)
    m_Max = New_Max
    If m_Max < m_Min Then m_Max = m_Min
    CheckMinMax
    PropertyChanged "Max"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Increment() As Long
    Increment = m_Increment
End Property

Public Property Let Increment(ByVal New_Increment As Long)
    m_Increment = New_Increment
    PropertyChanged "Increment"
End Property


Private Sub Text1_LostFocus()
    CheckMinMax
End Sub

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    m_Enabled = m_def_Enabled
    m_ForeColor = m_def_ForeColor
    Text1.Text = m_def_Value
    m_ButtonWidth = m_def_ButtonWidth
    m_Min = m_def_Min
    m_Max = m_def_Max
    m_Increment = m_def_Increment
End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    If (KeyCode = vbKeyDown) Then
        IncrementValue -1
        KeyCode = 0
    End If
    If (KeyCode = vbKeyUp) Then
        IncrementValue 1
        KeyCode = 0
    End If
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
On Error Resume Next

    Text1.Alignment = PropBag.ReadProperty("Alignment", 1)
    Text1.BackColor = PropBag.ReadProperty("BackColor", &H80000005)
    m_Enabled = PropBag.ReadProperty("Enabled", m_def_Enabled)
    Set Text1.Font = PropBag.ReadProperty("Font", Ambient.Font)
    m_ForeColor = PropBag.ReadProperty("ForeColor", m_def_ForeColor)
    Text1.Locked = PropBag.ReadProperty("Locked", False)
    Text1.Text = PropBag.ReadProperty("Text", "Text1")
    Picture1.BackColor = PropBag.ReadProperty("ButtonBackColor", &H80000005)
    Text1.Text = PropBag.ReadProperty("Value", m_def_Value)
    m_ButtonWidth = PropBag.ReadProperty("ButtonWidth", m_def_ButtonWidth)
    m_Min = PropBag.ReadProperty("Min", m_def_Min)
    m_Max = PropBag.ReadProperty("Max", m_def_Max)
    m_Increment = PropBag.ReadProperty("Increment", m_def_Increment)
    Text1.MaxLength = PropBag.ReadProperty("MaxLength", 10)
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
On Error Resume Next

    Call PropBag.WriteProperty("Alignment", Text1.Alignment, 1)
    Call PropBag.WriteProperty("BackColor", Text1.BackColor, &H80000005)
    Call PropBag.WriteProperty("Enabled", m_Enabled, m_def_Enabled)
    Call PropBag.WriteProperty("Font", Text1.Font, Ambient.Font)
    Call PropBag.WriteProperty("ForeColor", m_ForeColor, m_def_ForeColor)
    Call PropBag.WriteProperty("Locked", Text1.Locked, False)
    Call PropBag.WriteProperty("Text", Text1.Text, "Text1")
    Call PropBag.WriteProperty("ButtonBackColor", Picture1.BackColor, &H80000005)
    Call PropBag.WriteProperty("Value", CLng(Text1.Text), m_def_Value)
    Call PropBag.WriteProperty("ButtonWidth", m_ButtonWidth, m_def_ButtonWidth)
    Call PropBag.WriteProperty("Min", m_Min, m_def_Min)
    Call PropBag.WriteProperty("Max", m_Max, m_def_Max)
    Call PropBag.WriteProperty("Increment", m_Increment, m_def_Increment)
    Call PropBag.WriteProperty("MaxLength", Text1.MaxLength, 10)
End Sub


Private Sub UserControl_Resize()
On Error Resume Next
    With Text1
        .Top = 20
        .Left = 20
        .Width = Width - m_ButtonWidth - 40
        .Height = Height - 60
    End With
    With Picture1
        .Width = m_ButtonWidth
        .Height = Height
        .Left = Width - .Width
        .Top = 0
    End With
End Sub

Private Sub UserControl_Paint()
    DrawButtons
End Sub

'---------------------------
' button drawing
Private Sub DrawButtons()
Dim X1 As Long, X2 As Long, Y1 As Long, Y2 As Long
Dim SX1 As Long, SY1 As Long
Dim i As Long
Dim midx As Long, w As Long
Dim midy As Long, h As Long
On Error Resume Next
    
    With Picture1
        .Cls
        midx = .Width / 2
        midy = .Height / 2 - 30
        
        Picture1.Line (0, midy)-(.Width, midy), m_ForeColor
        Picture1.Line (0, 0)-(0, .Height), m_ForeColor
        
        h = midy / 2 + 40
        w = 5
        midy = midy + 5
        For i = 1 To 4
            Picture1.Line (midx - w, midy - h)-(midx + w, midy - h), m_ForeColor
            Picture1.Line (midx - w, midy + h)-(midx + w, midy + h), m_ForeColor
            w = w + 15
            h = h - 15
        Next i
    
    End With
End Sub


Private Sub CheckMinMax()
Dim v As Long
On Error GoTo E
    v = CLng(Text1.Text)
    If v < m_Min Then v = m_Min
    If v > m_Max Then v = m_Max
    Text1.Text = v
Exit Sub
E:
    Text1.Text = m_Min
End Sub

Private Sub IncrementValue(d As Long)
Dim v As Long
On Error GoTo E
    v = CLng(Text1.Text) + m_Increment * d
    If v > m_Max Then v = m_Max
    If v < m_Min Then v = m_Min
    Text1.Text = v
Exit Sub
E:
    Text1.Text = 0
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,MaxLength
Public Property Get MaxLength() As Long
Attribute MaxLength.VB_Description = "Returns/sets the maximum number of characters that can be entered in a control."
    MaxLength = Text1.MaxLength
End Property

Public Property Let MaxLength(ByVal New_MaxLength As Long)
On Error Resume Next
    Text1.MaxLength() = New_MaxLength
    PropertyChanged "MaxLength"
End Property

