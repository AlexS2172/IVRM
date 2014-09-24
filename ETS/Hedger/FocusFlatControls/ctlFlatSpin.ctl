VERSION 5.00
Begin VB.UserControl FlatSpin 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   1785
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3000
   KeyPreview      =   -1  'True
   ScaleHeight     =   119
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   200
   ToolboxBitmap   =   "ctlFlatSpin.ctx":0000
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H00C0FFC0&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   1320
      ScaleHeight     =   33
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   33
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
Const m_def_Min = 1
Const m_def_Max = 1000
Const m_def_ButtonWidth = 15
Const m_def_Value = 1


'Property Variables:
Dim m_Increment As Long
Dim m_Min As Long
Dim m_Max As Long
Dim m_ButtonWidth As Long

Dim m_ButtonBackColor As OLE_COLOR
Dim m_ButtonForeColor As OLE_COLOR
Dim m_DisabledForeColor As OLE_COLOR
Dim m_DisabledBackColor As OLE_COLOR
Dim m_HoverBackColor As OLE_COLOR
Dim m_HoverForeColor As OLE_COLOR
Dim m_PressBackColor As OLE_COLOR
Dim m_PressForeColor As OLE_COLOR

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
Event MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseDown
Attribute MouseDown.VB_Description = "Occurs when the user presses the mouse button while an object has the focus."
Event MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseMove
Attribute MouseMove.VB_Description = "Occurs when the user moves the mouse."
Event MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single) 'MappingInfo=Picture1,Picture1,-1,MouseUp
Attribute MouseUp.VB_Description = "Occurs when the user releases the mouse button while an object has the focus."

Dim m_Hover As Boolean
Dim m_Pressed As Boolean
Dim miActArrow As Long



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Alignment
Public Property Get Alignment() As Integer
Attribute Alignment.VB_Description = "Returns/sets the alignment of a CheckBox or OptionButton, or a control's text."
On Error Resume Next
    Alignment = Text1.Alignment
End Property

Public Property Let Alignment(ByVal New_Alignment As Integer)
On Error Resume Next
    Text1.Alignment() = New_Alignment
    Refresh
    PropertyChanged "Alignment"
End Property



Public Property Get ButtonBackColor() As OLE_COLOR
On Error Resume Next
    ButtonBackColor = m_ButtonBackColor
End Property

Public Property Let ButtonBackColor(ByVal New_ButtonBackColor As OLE_COLOR)
On Error Resume Next
    If m_ButtonBackColor = New_ButtonBackColor Then Exit Property
    m_ButtonBackColor = New_ButtonBackColor
    UserControl_Paint
    PropertyChanged "ButtonBackColor"
End Property

Public Property Get ButtonForeColor() As OLE_COLOR
Attribute ButtonForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
On Error Resume Next
    ButtonForeColor = m_ButtonForeColor
End Property

Public Property Let ButtonForeColor(ByVal New_ButtonForeColor As OLE_COLOR)
On Error Resume Next
    If m_ButtonForeColor = New_ButtonForeColor Then Exit Property
    m_ButtonForeColor = New_ButtonForeColor
    UserControl_Paint
    PropertyChanged "ButtonForeColor"
End Property


Public Property Get HoverBackColor() As OLE_COLOR
On Error Resume Next
    HoverBackColor = m_HoverBackColor
End Property

Public Property Let HoverBackColor(ByVal New_HoverBackColor As OLE_COLOR)
On Error Resume Next
    If m_HoverBackColor = New_HoverBackColor Then Exit Property
    m_HoverBackColor = New_HoverBackColor
    UserControl_Paint
    PropertyChanged "HoverBackColor"
End Property

Public Property Get PressBackColor() As OLE_COLOR
On Error Resume Next
    PressBackColor = m_PressBackColor
End Property

Public Property Let PressBackColor(ByVal New_PressBackColor As OLE_COLOR)
On Error Resume Next
    If m_PressBackColor = New_PressBackColor Then Exit Property
    m_PressBackColor = New_PressBackColor
    UserControl_Paint
    PropertyChanged "PressBackColor"
End Property

Public Property Get HoverForeColor() As OLE_COLOR
On Error Resume Next
    HoverForeColor = m_HoverForeColor
End Property

Public Property Let HoverForeColor(ByVal New_HoverForeColor As OLE_COLOR)
On Error Resume Next
    If m_HoverForeColor = New_HoverForeColor Then Exit Property
    m_HoverForeColor = New_HoverForeColor
    UserControl_Paint
    PropertyChanged "HoverForeColor"
End Property

Public Property Get PressForeColor() As OLE_COLOR
On Error Resume Next
    PressForeColor = m_PressForeColor
End Property

Public Property Let PressForeColor(ByVal New_PressForeColor As OLE_COLOR)
On Error Resume Next
    If m_PressForeColor = New_PressForeColor Then Exit Property
    m_PressForeColor = New_PressForeColor
    UserControl_Paint
    PropertyChanged "PressForeColor"
End Property


Public Property Get DisabledForeColor() As OLE_COLOR
On Error Resume Next
    DisabledForeColor = m_DisabledForeColor
End Property

Public Property Let DisabledForeColor(ByVal New_DisabledForeColor As OLE_COLOR)
On Error Resume Next
    If m_DisabledForeColor = New_DisabledForeColor Then Exit Property
    m_DisabledForeColor = New_DisabledForeColor
    UserControl_Paint
    PropertyChanged "DisabledForeColor"
End Property

Public Property Get DisabledBackColor() As OLE_COLOR
On Error Resume Next
    DisabledBackColor = m_DisabledBackColor
End Property

Public Property Let DisabledBackColor(ByVal New_DisabledBackColor As OLE_COLOR)
On Error Resume Next
    If m_DisabledBackColor = New_DisabledBackColor Then Exit Property
    m_DisabledBackColor = New_DisabledBackColor
    UserControl_Paint
    PropertyChanged "DisabledBackColor"
End Property






Private Sub Picture1_GotFocus()
On Error Resume Next
    Text1.SetFocus
End Sub

Private Sub Text1_Change()
On Error Resume Next
    RaiseEvent Change
End Sub

Private Sub Text1_Click()
On Error Resume Next
    RaiseEvent Click
End Sub


'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Font
Public Property Get Font() As StdFont
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
On Error Resume Next
    Set Font = Text1.Font
End Property

Public Property Set Font(ByVal New_Font As StdFont)
On Error Resume Next
    Set Text1.Font = New_Font
    Refresh
    PropertyChanged "Font"
End Property


Private Sub Text1_GotFocus()
On Error Resume Next
    Text1.SelStart = 0
    Text1.SelLength = Len(Text1.Text)
End Sub

Private Sub Text1_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub Text1_KeyPress(KeyAscii As Integer)
On Error Resume Next
    If KeyAscii < vbKey0 Or KeyAscii > vbKey9 Then
        KeyAscii = 0
    End If
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub Text1_KeyUp(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Locked
Public Property Get Locked() As Boolean
Attribute Locked.VB_Description = "Determines whether a control can be edited."
On Error Resume Next
    Locked = Text1.Locked
End Property

Public Property Let Locked(ByVal New_Locked As Boolean)
On Error Resume Next
    Text1.Locked() = New_Locked
    Refresh
    PropertyChanged "Locked"
End Property

Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
On Error Resume Next
    
    miActArrow = 0
    If Y < ScaleHeight / 2 Then
        miActArrow = 1
    Else
        miActArrow = -1
    End If
    
    m_Pressed = True
    DrawButtons
    RaiseEvent MouseDown(Button, Shift, x, Y)
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
Dim bTemp As Boolean
Dim curPos As POINTAPI
On Error Resume Next
     
    miActArrow = 0
    If Y < ScaleHeight / 2 Then
        miActArrow = 1
    Else
        miActArrow = -1
    End If
     
     ' is the mouse off of the button
    GetCursorPos curPos
     
    If WindowFromPoint(curPos.x, curPos.Y) <> Picture1.hWnd Then
     'If (X < 0) Or (Y < 0) Or (X > UserControl.Width) Or (Y > UserControl.Height) Then
          bTemp = False
          If GetCapture = Picture1.hWnd Then Call ReleaseCapture
     Else
          ' the mouse is still over the button, so be sure to call SetCapture again
          bTemp = True
          If GetCapture <> Picture1.hWnd Then Call SetCapture(Picture1.hWnd)
     End If
         
         
     ' only paint if necessary
'     If m_Hover <> bTemp Then
          If Button <> 0 Then m_Pressed = m_Pressed And True
          m_Hover = bTemp
          If m_Hover = False Then m_Pressed = False
          DrawButtons
'     End If
     
    RaiseEvent MouseMove(Button, Shift, x, Y)
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
Dim bTemp1 As Boolean
Dim bTemp2 As Boolean
On Error Resume Next
        
    miActArrow = 0
    If Y < ScaleHeight / 2 Then
        miActArrow = 1
    Else
        miActArrow = -1
    End If
    
    bTemp1 = m_Pressed
    Call ReleaseCapture
    m_Hover = False
    m_Pressed = False
    
    DrawButtons
    
    If bTemp1 Then
        RaiseEvent MouseUp(Button, Shift, x, Y)
        If (Button = vbLeftButton) And (bTemp1) Then RaiseEvent Click
    End If
    
    IncrementValue miActArrow
    
    RaiseEvent MouseUp(Button, Shift, x, Y)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,Text
Public Property Get Text() As String
Attribute Text.VB_Description = "Returns/sets the text contained in the control."
Attribute Text.VB_UserMemId = 0
Attribute Text.VB_MemberFlags = "200"
On Error Resume Next
    Text = Text1.Text
End Property

Public Property Let Text(ByVal New_Text As String)
On Error Resume Next
    Text1.Text() = New_Text
    CheckMinMax
    Refresh
    PropertyChanged "Text"
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
On Error Resume Next
    Text1.Text = Str(New_Value)
    CheckMinMax
    PropertyChanged "Value"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get ButtonWidth() As Long
On Error Resume Next
    ButtonWidth = m_ButtonWidth
End Property

Public Property Let ButtonWidth(ByVal New_ButtonWidth As Long)
On Error Resume Next
    m_ButtonWidth = New_ButtonWidth
    Refresh
    PropertyChanged "ButtonWidth"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Min() As Long
On Error Resume Next
    Min = m_Min
End Property

Public Property Let Min(ByVal New_Min As Long)
On Error Resume Next
    m_Min = New_Min
    If m_Min > m_Max Then m_Min = m_Max
    CheckMinMax
    PropertyChanged "Min"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Max() As Long
On Error Resume Next
    Max = m_Max
End Property

Public Property Let Max(ByVal New_Max As Long)
On Error Resume Next
    m_Max = New_Max
    If m_Max < m_Min Then m_Max = m_Min
    CheckMinMax
    PropertyChanged "Max"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Increment() As Long
On Error Resume Next
    Increment = m_Increment
End Property

Public Property Let Increment(ByVal New_Increment As Long)
On Error Resume Next
    m_Increment = New_Increment
    PropertyChanged "Increment"
End Property


Private Sub Text1_LostFocus()
On Error Resume Next
    CheckMinMax
End Sub

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    Set Text1.Font = Ambient.Font
    Text1.Text = m_def_Value
    m_ButtonWidth = m_def_ButtonWidth
    m_Min = m_def_Min
    m_Max = m_def_Max
    m_Increment = m_def_Increment

    m_ButtonBackColor = m_def_ButtonBackColor
    m_ButtonForeColor = m_def_ButtonForeColor
    m_HoverBackColor = m_def_HoverBackColor
    m_HoverForeColor = m_def_HoverForeColor
    m_PressBackColor = m_def_PressBackColor
    m_PressForeColor = m_def_PressForeColor
    m_DisabledForeColor = m_def_DisabledForeColor
    m_DisabledBackColor = m_def_DisabledBackColor

End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
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
    Set Text1.Font = PropBag.ReadProperty("Font", Ambient.Font)
    Text1.Locked = PropBag.ReadProperty("Locked", False)
    Text1.Text = PropBag.ReadProperty("Text", "Text1")
    Text1.Text = PropBag.ReadProperty("Value", m_def_Value)
    m_ButtonWidth = PropBag.ReadProperty("ButtonWidth", m_def_ButtonWidth)
    m_Min = PropBag.ReadProperty("Min", m_def_Min)
    m_Max = PropBag.ReadProperty("Max", m_def_Max)
    m_Increment = PropBag.ReadProperty("Increment", m_def_Increment)
    Text1.MaxLength = PropBag.ReadProperty("MaxLength", 10)

    m_ButtonBackColor = PropBag.ReadProperty("ButtonBackColor", m_def_ButtonBackColor)
    m_ButtonForeColor = PropBag.ReadProperty("ButtonForeColor", m_def_ButtonForeColor)
    m_HoverBackColor = PropBag.ReadProperty("HoverBackColor", m_def_HoverBackColor)
    m_HoverForeColor = PropBag.ReadProperty("HoverForeColor", m_def_HoverForeColor)
    m_PressBackColor = PropBag.ReadProperty("PressBackColor", m_def_PressBackColor)
    m_PressForeColor = PropBag.ReadProperty("PressForeColor", m_def_PressForeColor)
    m_DisabledForeColor = PropBag.ReadProperty("DisabledForeColor", m_def_DisabledForeColor)
    m_DisabledBackColor = PropBag.ReadProperty("DisabledBackColor", m_def_DisabledBackColor)

    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    Text1.BackColor = PropBag.ReadProperty("BackColor", &H80000005)
    Text1.ForeColor = PropBag.ReadProperty("ForeColor", &H80000008)
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
On Error Resume Next

    Call PropBag.WriteProperty("Alignment", Text1.Alignment, 1)
    Call PropBag.WriteProperty("Font", Text1.Font, Ambient.Font)
    Call PropBag.WriteProperty("Locked", Text1.Locked, False)
    Call PropBag.WriteProperty("Text", Text1.Text, "Text1")
    Call PropBag.WriteProperty("Value", CLng(Text1.Text), m_def_Value)
    Call PropBag.WriteProperty("ButtonWidth", m_ButtonWidth, m_def_ButtonWidth)
    Call PropBag.WriteProperty("Min", m_Min, m_def_Min)
    Call PropBag.WriteProperty("Max", m_Max, m_def_Max)
    Call PropBag.WriteProperty("Increment", m_Increment, m_def_Increment)
    Call PropBag.WriteProperty("MaxLength", Text1.MaxLength, 10)

    Call PropBag.WriteProperty("ButtonBackColor", m_ButtonBackColor, m_def_ButtonBackColor)
    Call PropBag.WriteProperty("ButtonForeColor", m_ButtonForeColor, m_def_ButtonForeColor)
    Call PropBag.WriteProperty("HoverBackColor", m_HoverBackColor, m_def_HoverBackColor)
    Call PropBag.WriteProperty("HoverForeColor", m_HoverForeColor, m_def_HoverForeColor)
    Call PropBag.WriteProperty("PressBackColor", m_PressBackColor, m_def_PressBackColor)
    Call PropBag.WriteProperty("PressForeColor", m_PressForeColor, m_def_PressForeColor)
    Call PropBag.WriteProperty("DisabledForeColor", m_DisabledForeColor, m_def_DisabledForeColor)
    Call PropBag.WriteProperty("DisabledBackColor", m_DisabledBackColor, m_def_DisabledBackColor)

    Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
    Call PropBag.WriteProperty("BackColor", Text1.BackColor, &H80000005)
    Call PropBag.WriteProperty("ForeColor", Text1.ForeColor, &H80000008)
End Sub


Private Sub UserControl_Resize()
On Error Resume Next
    With Text1
        .Top = 1
        .Left = 1
        .Width = ScaleWidth - m_ButtonWidth - 2
        .Height = ScaleHeight
    End With
    With Picture1
        .Width = m_ButtonWidth
        .Height = ScaleHeight
        .Left = ScaleWidth - .Width
        .Top = 0
    End With
End Sub

Private Sub UserControl_Paint()
On Error Resume Next
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

Dim iUpForeColor As Long
Dim iDownForeColor As Long
Dim iUpBackColor As Long
Dim iDownBackColor As Long

On Error Resume Next
    
    If UserControl.Enabled Then
        
        iUpBackColor = m_ButtonBackColor
        iUpForeColor = m_ButtonForeColor
        iDownBackColor = m_ButtonBackColor
        iDownForeColor = m_ButtonForeColor
    
        If miActArrow > 0 Then
            If m_Hover Then
                iUpBackColor = m_HoverBackColor
                iUpForeColor = m_HoverForeColor
            End If
            If m_Pressed Then
                iUpBackColor = m_PressBackColor
                iUpForeColor = m_PressForeColor
            End If
        Else
            If m_Hover Then
                iDownBackColor = m_HoverBackColor
                iDownForeColor = m_HoverForeColor
            End If
            If m_Pressed Then
                iDownBackColor = m_PressBackColor
                iDownForeColor = m_PressForeColor
            End If
        End If
        
    Else
        iUpBackColor = m_DisabledBackColor
        iUpForeColor = m_DisabledForeColor
        
        iDownBackColor = m_DisabledBackColor
        iDownForeColor = m_DisabledForeColor
    End If
    
    
    With Picture1
        .Cls
        
        midx = .ScaleWidth / 2
        midy = .ScaleHeight / 2
        
        .FillStyle = 0
        .FillColor = iUpBackColor
        Picture1.Line (0, -1)-(.ScaleWidth, midy), vbBlack, B
        .FillColor = iDownBackColor
        Picture1.Line (0, midy)-(.ScaleWidth, .ScaleHeight), vbBlack, B
        
        h = midy / 2
        w = 1
        midy = midy - 1
        For i = 1 To 4
            Picture1.Line (midx - w, midy - h)-(midx + w, midy - h), iUpForeColor
            Picture1.Line (midx - w, midy + h + 2)-(midx + w, midy + h + 2), iDownForeColor
            w = w + 1
            h = h - 1
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
    If d = 0 Then Exit Sub
    v = CLng(Text1.Text) + m_Increment * d
    If v > m_Max Then v = m_Max
    If v < m_Min Then v = m_Min
    Text1.Text = v
    
On Error Resume Next
    CheckMinMax
    Text1.SetFocus
    Text1.SelStart = 0
    Text1.SelLength = Len(Text1.Text)
Exit Sub
E:
    Text1.Text = 0
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,MaxLength
Public Property Get MaxLength() As Long
Attribute MaxLength.VB_Description = "Returns/sets the maximum number of characters that can be entered in a control."
On Error Resume Next
    MaxLength = Text1.MaxLength
End Property

Public Property Let MaxLength(ByVal New_MaxLength As Long)
On Error Resume Next
    Text1.MaxLength() = New_MaxLength
    PropertyChanged "MaxLength"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
On Error Resume Next
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
On Error Resume Next
    UserControl.Enabled() = New_Enabled
    Text1.Enabled = New_Enabled
    Picture1.Enabled = New_Enabled
    DrawButtons
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
On Error Resume Next
    BackColor = Text1.BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
On Error Resume Next
    Text1.BackColor() = New_BackColor
    PropertyChanged "BackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Text1,Text1,-1,ForeColor
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
On Error Resume Next
    ForeColor = Text1.ForeColor
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
On Error Resume Next
    Text1.ForeColor() = New_ForeColor
    PropertyChanged "ForeColor"
End Property

