VERSION 5.00
Begin VB.UserControl FlatButton 
   Appearance      =   0  'Flat
   BackColor       =   &H00FFFFFF&
   ClientHeight    =   780
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   1815
   DefaultCancel   =   -1  'True
   ScaleHeight     =   780
   ScaleWidth      =   1815
End
Attribute VB_Name = "FlatButton"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'**************************************************
' Flat Button Control
' Created by Andrei Shchurov (shurov@egartech.com)
' October 25, 2000
'
' This Flat Buton is used in place of all standatd button and in navigation panel
' Major properties:
' Alignment - caption alignment (left, right and center)
' HoverEffect - button behaviour when mouse hover it (None, Underline and Displace)
' BorderStyle - border around button
' CanGetFocus - defines if the button can get focus (usefull for borderless buttons)
' CanDrawFocus - defines if the button can draw thick border arround focused button
' Colors for different button states
'**************************************************
Option Explicit



Enum efbAlignment
    fbAlignLeft
    fbAlignRight
    fbAlignCenter
End Enum

Enum ebfHoverEffect
    fbHENone
    fbHEUnderline
    fbHEDisplace
End Enum

Enum efbBorderStyle
    fbBSNone
    fbBSBorder
End Enum

'Default Property Values:
Const m_def_CanGetFocus = True
Const m_def_DisabledForeColor = &H808080
Const m_def_DisabledBackColor = &HC0C0C0
Const m_def_BackColor = &HC0C0C0
Const m_def_ForeColor = &H0
Const m_def_HoverBackColor = &HE0E0E0
Const m_def_HoverForeColor = &HFF0000
Const m_def_PressBackColor = &H808080
Const m_def_PressForeColor = &H0

Const m_def_HoverEffect = fbHENone
Const m_def_CanDrawFocus = True
Const m_def_CaptionShift = 20
Const m_def_Alignment = fbAlignCenter
Const m_def_Caption = ""
Const m_def_BorderStyle = fbBSBorder


'Property Variables:
Dim m_CanGetFocus As Boolean
Dim m_DisabledForeColor As OLE_COLOR
Dim m_DisabledBackColor As OLE_COLOR
Dim m_BackColor As OLE_COLOR
Dim m_ForeColor As OLE_COLOR
Dim m_HoverBackColor As OLE_COLOR
Dim m_HoverForeColor As OLE_COLOR
Dim m_PressBackColor As OLE_COLOR
Dim m_PressForeColor As OLE_COLOR

Dim m_CanDrawFocus As Boolean
Dim m_CaptionShift As Integer
Dim m_Alignment As efbAlignment
Dim m_Caption As String
Dim m_Font As Font
Dim m_HoverEffect As ebfHoverEffect
Dim m_BorderStyle As efbBorderStyle

Dim m_Hover As Boolean
Dim m_Pressed As Boolean

'Event Declarations:
Event Click() 'MappingInfo=UserControl,UserControl,-1,Click
Attribute Click.VB_Description = "Occurs when the user presses and then releases a mouse button over an object."
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyDown
Attribute KeyDown.VB_Description = "Occurs when the user presses a key while an object has the focus."
Event KeyPress(KeyAscii As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyPress
Attribute KeyPress.VB_Description = "Occurs when the user presses and releases an ANSI key."
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=UserControl,UserControl,-1,KeyUp
Attribute KeyUp.VB_Description = "Occurs when the user releases a key while an object has the focus."
Event MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseDown
Attribute MouseDown.VB_Description = "Occurs when the user presses the mouse button while an object has the focus."
Event MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseMove
Attribute MouseMove.VB_Description = "Occurs when the user moves the mouse."
Event MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single) 'MappingInfo=UserControl,UserControl,-1,MouseUp
Attribute MouseUp.VB_Description = "Occurs when the user releases the mouse button while an object has the focus."




'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,&H8000000F&
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
    BackColor = m_BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
    If m_BackColor = New_BackColor Then Exit Property
    m_BackColor = New_BackColor
    UserControl_Paint
    PropertyChanged "BackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
    ForeColor = m_ForeColor
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
    If m_ForeColor = New_ForeColor Then Exit Property
    m_ForeColor = New_ForeColor
    UserControl_Paint
    PropertyChanged "ForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
Attribute Enabled.VB_Description = "Returns/sets a value that determines whether an object can respond to user-generated events."
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
    UserControl.Enabled() = New_Enabled
    UserControl_Paint
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=6,0,0,0
Public Property Get Font() As Font
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
    Set Font = m_Font
End Property

Public Property Set Font(ByVal New_Font As Font)
    Set m_Font = New_Font
    UserControl_Paint
    PropertyChanged "Font"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Refresh
Public Sub Refresh()
Attribute Refresh.VB_Description = "Forces a complete repaint of a object."
On Error Resume Next
    UserControl.Refresh
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MouseIcon
Public Property Get MouseIcon() As Picture
Attribute MouseIcon.VB_Description = "Sets a custom mouse icon."
    Set MouseIcon = UserControl.MouseIcon
End Property

Public Property Set MouseIcon(ByVal New_MouseIcon As Picture)
    Set UserControl.MouseIcon = New_MouseIcon
    UserControl_Paint
    PropertyChanged "MouseIcon"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,MousePointer
Public Property Get MousePointer() As Integer
Attribute MousePointer.VB_Description = "Returns/sets the type of mouse pointer displayed when over part of an object."
    MousePointer = UserControl.MousePointer
End Property

Public Property Let MousePointer(ByVal New_MousePointer As Integer)
    UserControl.MousePointer() = New_MousePointer
    UserControl_Paint
    PropertyChanged "MousePointer"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverBackColor() As OLE_COLOR
    HoverBackColor = m_HoverBackColor
End Property

Public Property Let HoverBackColor(ByVal New_HoverBackColor As OLE_COLOR)
    If m_HoverBackColor = New_HoverBackColor Then Exit Property
    m_HoverBackColor = New_HoverBackColor
    UserControl_Paint
    PropertyChanged "HoverBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressBackColor() As OLE_COLOR
    PressBackColor = m_PressBackColor
End Property

Public Property Let PressBackColor(ByVal New_PressBackColor As OLE_COLOR)
    If m_PressBackColor = New_PressBackColor Then Exit Property
    m_PressBackColor = New_PressBackColor
    UserControl_Paint
    PropertyChanged "PressBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverForeColor() As OLE_COLOR
    HoverForeColor = m_HoverForeColor
End Property

Public Property Let HoverForeColor(ByVal New_HoverForeColor As OLE_COLOR)
    If m_HoverForeColor = New_HoverForeColor Then Exit Property
    m_HoverForeColor = New_HoverForeColor
    UserControl_Paint
    PropertyChanged "HoverForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressForeColor() As OLE_COLOR
    PressForeColor = m_PressForeColor
End Property

Public Property Let PressForeColor(ByVal New_PressForeColor As OLE_COLOR)
    If m_PressForeColor = New_PressForeColor Then Exit Property
    m_PressForeColor = New_PressForeColor
    UserControl_Paint
    PropertyChanged "PressForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=13,0,0,0
Public Property Get Caption() As String
    Caption = m_Caption
End Property

Public Property Let Caption(ByVal New_Caption As String)
    If m_Caption = New_Caption Then Exit Property
    m_Caption = New_Caption
    UserControl_Paint
    PropertyChanged "Caption"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,0
Public Property Get CaptionShift() As Integer
    CaptionShift = m_CaptionShift
End Property

Public Property Let CaptionShift(ByVal New_CaptionShift As Integer)
    If m_CaptionShift = New_CaptionShift Then Exit Property
    m_CaptionShift = New_CaptionShift
    UserControl_Paint
    PropertyChanged "CaptionShift"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,0
Public Property Get Alignment() As efbAlignment
    Alignment = m_Alignment
End Property

Public Property Let Alignment(ByVal New_Alignment As efbAlignment)
    If m_Alignment = New_Alignment Then Exit Property
    m_Alignment = New_Alignment
    UserControl_Paint
    PropertyChanged "Alignment"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get CanDrawFocus() As Boolean
    CanDrawFocus = m_CanDrawFocus
End Property

Public Property Let CanDrawFocus(ByVal New_CanDrawFocus As Boolean)
    If m_CanDrawFocus = New_CanDrawFocus Then Exit Property
    m_CanDrawFocus = New_CanDrawFocus
    UserControl_Paint
    PropertyChanged "CanDrawFocus"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledForeColor() As OLE_COLOR
    DisabledForeColor = m_DisabledForeColor
End Property

Public Property Let DisabledForeColor(ByVal New_DisabledForeColor As OLE_COLOR)
    If m_DisabledForeColor = New_DisabledForeColor Then Exit Property
    m_DisabledForeColor = New_DisabledForeColor
    UserControl_Paint
    PropertyChanged "DisabledForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledBackColor() As OLE_COLOR
    DisabledBackColor = m_DisabledBackColor
End Property

Public Property Let DisabledBackColor(ByVal New_DisabledBackColor As OLE_COLOR)
    If m_DisabledBackColor = New_DisabledBackColor Then Exit Property
    m_DisabledBackColor = New_DisabledBackColor
    UserControl_Paint
    PropertyChanged "DisabledBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get HoverEffect() As ebfHoverEffect
    HoverEffect = m_HoverEffect
End Property

Public Property Let HoverEffect(ByVal New_HoverEffect As ebfHoverEffect)
    If m_HoverEffect = New_HoverEffect Then Exit Property
    m_HoverEffect = New_HoverEffect
    UserControl_Paint
    PropertyChanged "HoverEffect"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=7,0,0,0
Public Property Get BorderStyle() As efbBorderStyle
    BorderStyle = m_BorderStyle
End Property

Public Property Let BorderStyle(ByVal New_BorderStyle As efbBorderStyle)
    If m_BorderStyle = New_BorderStyle Then Exit Property
    m_BorderStyle = New_BorderStyle
    UserControl_Paint
    PropertyChanged "BorderStyle"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=0,0,0,0
Public Property Get CanGetFocus() As Boolean
    CanGetFocus = m_CanGetFocus
End Property

Public Property Let CanGetFocus(ByVal New_CanGetFocus As Boolean)
    If m_CanGetFocus = New_CanGetFocus Then Exit Property
    m_CanGetFocus = New_CanGetFocus
    UserControl_Paint
    PropertyChanged "CanGetFocus"
End Property



'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    m_BackColor = m_def_BackColor
    m_ForeColor = m_def_ForeColor
    Set m_Font = Ambient.Font
    m_HoverBackColor = m_def_HoverBackColor
    m_HoverForeColor = m_def_HoverForeColor
    m_PressBackColor = m_def_PressBackColor
    m_PressForeColor = m_def_PressForeColor
    m_Caption = UserControl.Name
    m_CaptionShift = m_def_CaptionShift
    m_Alignment = m_def_Alignment
    m_CanDrawFocus = m_def_CanDrawFocus
    m_DisabledForeColor = m_def_DisabledForeColor
    m_DisabledBackColor = m_def_DisabledBackColor
    m_HoverEffect = m_def_HoverEffect
    m_BorderStyle = m_def_BorderStyle
    m_CanGetFocus = m_def_CanGetFocus
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
On Error Resume Next

    m_BackColor = PropBag.ReadProperty("BackColor", m_def_BackColor)
    m_ForeColor = PropBag.ReadProperty("ForeColor", m_def_ForeColor)
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    Set m_Font = PropBag.ReadProperty("Font", Ambient.Font)
    Set MouseIcon = PropBag.ReadProperty("MouseIcon", Nothing)
    UserControl.MousePointer = PropBag.ReadProperty("MousePointer", 0)
    m_HoverBackColor = PropBag.ReadProperty("HoverBackColor", m_def_HoverBackColor)
    m_HoverForeColor = PropBag.ReadProperty("HoverForeColor", m_def_HoverForeColor)
    m_PressBackColor = PropBag.ReadProperty("PressBackColor", m_def_PressBackColor)
    m_PressForeColor = PropBag.ReadProperty("PressForeColor", m_def_PressForeColor)
    m_Caption = PropBag.ReadProperty("Caption", m_def_Caption)
    m_CaptionShift = PropBag.ReadProperty("CaptionShift", m_def_CaptionShift)
    m_Alignment = PropBag.ReadProperty("Alignment", m_def_Alignment)
    m_CanDrawFocus = PropBag.ReadProperty("CanDrawFocus", m_def_CanDrawFocus)
    m_DisabledForeColor = PropBag.ReadProperty("DisabledForeColor", m_def_DisabledForeColor)
    m_DisabledBackColor = PropBag.ReadProperty("DisabledBackColor", m_def_DisabledBackColor)
    m_HoverEffect = PropBag.ReadProperty("HoverEffect", m_def_HoverEffect)
    m_BorderStyle = PropBag.ReadProperty("BorderStyle", m_def_BorderStyle)
    m_CanGetFocus = PropBag.ReadProperty("CanGetFocus", m_def_CanGetFocus)
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
On Error Resume Next

    Call PropBag.WriteProperty("BackColor", m_BackColor, m_def_BackColor)
    Call PropBag.WriteProperty("ForeColor", m_ForeColor, m_def_ForeColor)
    Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
    Call PropBag.WriteProperty("Font", m_Font, Ambient.Font)
    Call PropBag.WriteProperty("MouseIcon", MouseIcon, Nothing)
    Call PropBag.WriteProperty("MousePointer", UserControl.MousePointer, 0)
    Call PropBag.WriteProperty("HoverBackColor", m_HoverBackColor, m_def_HoverBackColor)
    Call PropBag.WriteProperty("HoverForeColor", m_HoverForeColor, m_def_HoverForeColor)
    Call PropBag.WriteProperty("Caption", m_Caption, m_def_Caption)
    Call PropBag.WriteProperty("CaptionShift", m_CaptionShift, m_def_CaptionShift)
    Call PropBag.WriteProperty("Alignment", m_Alignment, m_def_Alignment)
    Call PropBag.WriteProperty("PressBackColor", m_PressBackColor, m_def_PressBackColor)
    Call PropBag.WriteProperty("PressForeColor", m_PressForeColor, m_def_PressForeColor)
    Call PropBag.WriteProperty("CanDrawFocus", m_CanDrawFocus, m_def_CanDrawFocus)
    Call PropBag.WriteProperty("DisabledForeColor", m_DisabledForeColor, m_def_DisabledForeColor)
    Call PropBag.WriteProperty("DisabledBackColor", m_DisabledBackColor, m_def_DisabledBackColor)
    Call PropBag.WriteProperty("HoverEffect", m_HoverEffect, m_def_HoverEffect)
    Call PropBag.WriteProperty("BorderStyle", m_BorderStyle, m_def_BorderStyle)
    Call PropBag.WriteProperty("CanGetFocus", m_CanGetFocus, m_def_CanGetFocus)
End Sub


Private Sub UserControl_EnterFocus()
    UserControl_Paint
End Sub

Private Sub UserControl_ExitFocus()
    UserControl_Paint
End Sub

Private Sub UserControl_KeyDown(KeyCode As Integer, Shift As Integer)
    If Not m_CanGetFocus Then
        KeyCode = 0
        Exit Sub
    End If
    If KeyCode = vbKeySpace Then
        RaiseEvent Click
    End If
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub UserControl_KeyPress(KeyAscii As Integer)
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub UserControl_KeyUp(KeyCode As Integer, Shift As Integer)
    UserControl_Paint
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

Private Sub UserControl_AccessKeyPress(KeyAscii As Integer)
    If m_CanGetFocus Then
        RaiseEvent Click
    End If
End Sub

Private Sub UserControl_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Not UserControl.Enabled Then Exit Sub
    m_Pressed = True
    UserControl_Paint
    RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim bTemp As Boolean
Dim curPos As POINTAPI
On Error Resume Next
    If Not UserControl.Enabled Then Exit Sub
     
     ' is the mouse off of the button
    GetCursorPos curPos
     
    If WindowFromPoint(curPos.X, curPos.Y) <> UserControl.hwnd Then
     'If (X < 0) Or (Y < 0) Or (X > UserControl.Width) Or (Y > UserControl.Height) Then
          bTemp = False
          If GetCapture = UserControl.hwnd And Not m_Pressed Then ReleaseCapture
     Else
          ' the mouse is still over the button, so be sure to call SetCapture again
          bTemp = True
          If GetCapture <> UserControl.hwnd And Not m_Pressed Then Call SetCapture(UserControl.hwnd)
     End If
         
     ' only paint if necessary
     If m_Hover <> bTemp Then
          'If Button <> 0 Then m_Pressed = m_Pressed And bTemp
          m_Hover = bTemp
          'If m_Hover = False Then m_Pressed = False
          UserControl_Paint
     End If
     
    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim bTemp1 As Boolean
Dim bTemp2 As Boolean
Dim curPos As POINTAPI
On Error Resume Next
    If Not UserControl.Enabled Then Exit Sub
    
    bTemp1 = m_Pressed
    'Call ReleaseCapture
    'm_Hover = False
    RaiseEvent MouseUp(Button, Shift, X, Y)
    m_Pressed = False
    UserControl_Paint
    
    If bTemp1 Then
     ' is the mouse off of the button
        GetCursorPos curPos
         
        If WindowFromPoint(curPos.X, curPos.Y) = UserControl.hwnd Then
            If Button = vbLeftButton And bTemp1 Then RaiseEvent Click
        End If
    End If
End Sub




Private Sub UserControl_Paint()
    DrawButton
End Sub

Sub DrawButton()
Dim cap1 As String
Dim cap2 As String
Dim cap3 As String
Dim s As String
Dim s2 As String
Dim p As Integer
Dim tmpFontUnderline As Boolean
On Error Resume Next

    With UserControl
        ' setting font
        .Font.Bold = m_Font.Bold
        .Font.Italic = m_Font.Italic
        .Font.Name = m_Font.Name
        .Font.Size = m_Font.Size
        .Font.Underline = m_Font.Underline
        
        ' setting color
        If .Enabled Then
        
            If m_Hover Then
                If m_HoverEffect = fbHEUnderline Then .Font.Underline = True
                If m_Pressed Then
                    .BackColor = m_PressBackColor
                    .ForeColor = m_PressForeColor
                Else
                    .BackColor = m_HoverBackColor
                    .ForeColor = m_HoverForeColor
                End If
            Else
                .BackColor = m_BackColor
                .ForeColor = m_ForeColor
            End If
            
        Else
            .BackColor = m_DisabledBackColor
            .ForeColor = m_DisabledForeColor
        End If
        
        ' clearing control
        Cls
        
        If m_BorderStyle = fbBSBorder Then
            ' drawing border
            .DrawWidth = 1
            Line (0, 0)-(.Width - 10, .Height - 10), , B
        End If
                
        If (GetFocus = UserControl.hwnd) And m_CanDrawFocus Then
            'drawing focused button
            .DrawWidth = 1
            Line (10, 10)-(.Width - 30, .Height - 30), , B
        End If
        
        ' searching access key
        cap1 = m_Caption
        cap2 = ""
        cap3 = ""
        p = InStr(1, m_Caption, "&")
        If p > 0 Then
            If InStr(p + 1, m_Caption, "&") <> (p + 1) Then
                cap1 = Left(m_Caption, p - 1)
                cap2 = Mid(m_Caption, p + 1, 1)
                cap3 = Mid(m_Caption, p + 2)
            End If
        End If

        ' calculating caption posiion
        ' y position
        .CurrentY = (.Height - .TextHeight(m_Caption) - 10) / 2
        ' x position
        Select Case m_Alignment
        Case fbAlignLeft
            .CurrentX = m_CaptionShift
        Case fbAlignRight
            .CurrentX = .Width - .TextWidth(m_Caption) - m_CaptionShift
        Case Else
            .CurrentX = (.Width - .TextWidth(m_Caption)) / 2
        End Select
        
        If m_Hover And m_HoverEffect = fbHEDisplace Then
            ' implementing displace effect
            .CurrentX = .CurrentX + 20
            .CurrentY = .CurrentY + 20
        End If
        
        ' cutting long caption
        s = cap1 & cap2 & cap3
        s2 = s
        Do While ((.CurrentX + .TextWidth(s2)) > Width) And (Len(s) > 0)
            s = Left(s, Len(s) - 1)
            s2 = s & "... "
        Loop
        cap1 = Left(s2, Len(cap1))
        cap2 = Mid(s2, Len(cap1) + 1, Len(cap2))
        cap3 = Mid(s2, Len(cap1) + Len(cap2) + 1, Len(cap3))
        
        ' printinc a caption
        Print cap1;
        tmpFontUnderline = .Font.Underline
        .Font.Underline = True
        Print cap2;
        .Font.Underline = tmpFontUnderline
        Print cap3;
        ' setting access key
        .AccessKeys = cap2
    End With
End Sub
