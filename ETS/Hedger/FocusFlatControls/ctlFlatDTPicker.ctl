VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Begin VB.UserControl FlatDTPicker 
   Appearance      =   0  'Flat
   AutoRedraw      =   -1  'True
   BackColor       =   &H00C0C0FF&
   ClientHeight    =   1935
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3705
   ScaleHeight     =   129
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   247
   ToolboxBitmap   =   "ctlFlatDTPicker.ctx":0000
   Begin MSComCtl2.DTPicker DTPicker1 
      Height          =   375
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   661
      _Version        =   393216
      Format          =   22740993
      CurrentDate     =   37008
   End
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      AutoRedraw      =   -1  'True
      BackColor       =   &H00C0FFC0&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   1680
      ScaleHeight     =   33
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   33
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   0
      Width           =   495
   End
End
Attribute VB_Name = "FlatDTPicker"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit



Private marrComboRgn(3) As POINTAPI
Private miComboRgnPtr As Long

'Event Declarations:
Event CallbackKeyDown(ByVal KeyCode As Integer, ByVal Shift As Integer, ByVal CallbackField As String, CallbackDate As Date) 'MappingInfo=DTPicker1,DTPicker1,-1,CallbackKeyDown
Attribute CallbackKeyDown.VB_Description = "Occurs when the user presses a key when the insertion point is over a callback field."
Event CloseUp() 'MappingInfo=DTPicker1,DTPicker1,-1,CloseUp
Attribute CloseUp.VB_Description = "Occurs when the user is finished selecting a date from the dropdown calendar."
Event FormatSize(ByVal CallbackField As String, Size As Integer) 'MappingInfo=DTPicker1,DTPicker1,-1,FormatSize
Attribute FormatSize.VB_Description = "Occurs when the control needs to know the maximum allowable size of a callback field."
'Event FormatDate(ByVal CallbackField As String, FormattedString As String)
Event Format(ByVal CallbackField As String, FormattedString As String)
Event Change() 'MappingInfo=DTPicker1,DTPicker1,-1,Change
Event Click() 'MappingInfo=DTPicker1,DTPicker1,-1,Click
Event DblClick() 'MappingInfo=DTPicker1,DTPicker1,-1,DblClick
Event DropDown() 'MappingInfo=DTPicker1,DTPicker1,-1,DropDown
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=DTPicker1,DTPicker1,-1,KeyDown
Event KeyPress(KeyAscii As Integer) 'MappingInfo=DTPicker1,DTPicker1,-1,KeyPress
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=DTPicker1,DTPicker1,-1,KeyUp


'Const m_def_CalendarBackColor = &H80000005
'Const m_def_CalendarForeColor = &H80000012
'Const m_def_CalendarTitleBackColor = &H8000000F
'Const m_def_CalendarTitleForeColor = &H80000012
'Const m_def_CalendarTrailingForeColor = &H80000011
Const m_def_CalendarBackColor = &H42CFFF
Const m_def_CalendarForeColor = &H80000012
Const m_def_CalendarTitleBackColor = m_def_ButtonBackColor
Const m_def_CalendarTitleForeColor = m_def_ButtonForeColor
Const m_def_CalendarTrailingForeColor = &H80000011




'Property Variables:
Dim m_ButtonBackColor As OLE_COLOR
Dim m_HoverBackColor As OLE_COLOR
Dim m_PressBackColor As OLE_COLOR
Dim m_DisabledBackColor As OLE_COLOR
Dim m_ButtonForeColor As OLE_COLOR
Dim m_HoverForeColor As OLE_COLOR
Dim m_PressForeColor As OLE_COLOR
Dim m_DisabledForeColor As OLE_COLOR

Dim m_Hover As Boolean
Dim m_Pressed As Boolean
Dim miActArrow As Long


'---------------------------
' button drawing
Private Sub DrawTwoButtons()
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
        Picture1.Line (0, 0)-(.Width - 1, midy), vbBlack, B
        .FillColor = iDownBackColor
        Picture1.Line (0, midy)-(.Width - 1, .Height - 1), vbBlack, B
        
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


Private Sub DrawOneButton()
Dim i As Long
Dim midx As Long
Dim midy As Long
Dim h As Long, dw As Long
Dim iBackColor As Long
Dim iForeColor As Long
On Error Resume Next


    If UserControl.Enabled Then
        iBackColor = m_ButtonBackColor
        iForeColor = m_ButtonForeColor
        If m_Hover Then
            iBackColor = m_HoverBackColor
            iForeColor = m_HoverForeColor
        End If
        If m_Pressed Then
            iBackColor = m_PressBackColor
            iForeColor = m_PressForeColor
        End If
    Else
        iBackColor = m_DisabledBackColor
        iForeColor = m_DisabledForeColor
    End If
    
    
    
    ' button
    With Picture1
        .BackColor = iBackColor
        .DrawWidth = 1
        Picture1.Line (0, 0)-(.Width - 1, .Height - 1), vbBlack, B
        
        midx = 7 '.Width / 2
        midy = .Height / 2
        h = midy + 2
        dw = 0
        For i = 1 To 5
            Picture1.Line (midx - dw, h)-(midx + dw, h), iForeColor
            dw = dw + 1
            h = h - 1
        Next i
    End With
End Sub


Private Sub DrawButton()
    ' border
    If UserControl.Enabled Then
        UserControl.BackColor = vbWindowBackground
    Else
        UserControl.BackColor = vbButtonFace
    End If
    Cls
    DrawWidth = 1
    Line (0, 0)-(ScaleWidth - 1, ScaleHeight - 1), vbBlack, B
    
    
    
    
    If DTPicker1.UpDown Then
        DrawTwoButtons
    Else
        DrawOneButton
    End If
End Sub

Private Sub DrawCombo()
On Error Resume Next
    With DTPicker1
    
        .Move 0, 0, ScaleWidth, ScaleHeight
    
        marrComboRgn(0).X = 3
        marrComboRgn(0).Y = 3
        marrComboRgn(1).X = .Width - 17
        marrComboRgn(1).Y = marrComboRgn(0).Y
        marrComboRgn(2).X = marrComboRgn(1).X
        marrComboRgn(2).Y = .Height - 3
        marrComboRgn(3).X = marrComboRgn(0).X
        marrComboRgn(3).Y = marrComboRgn(2).Y
    
        If miComboRgnPtr <> 0 Then DeleteObject miComboRgnPtr
        miComboRgnPtr = CreatePolygonRgn(marrComboRgn(0), 4, 1)
        SetWindowRgn .hwnd, miComboRgnPtr, True
        
    End With
    
    With Picture1
        .Left = ScaleWidth - 15
        .Top = 0
        .Width = 15
        .Height = ScaleHeight
    End With
    DrawButton
End Sub

'-----------------------------
' dropping list down
Private Sub DropListDown()
On Error Resume Next
    Call ReleaseCapture
    DTPicker1.SetFocus
    SendKeys "{F4}"
End Sub

Private Sub DTPicker1_Format(ByVal CallbackField As String, FormattedString As String)
On Error Resume Next
    'RaiseEvent FormatDate(CallbackField, FormattedString)
    RaiseEvent Format(CallbackField, FormattedString)
End Sub

Private Sub Picture1_GotFocus()
On Error Resume Next
    DTPicker1.SetFocus
End Sub

Private Sub UserControl_Resize()
On Error Resume Next
    DrawCombo
End Sub

Private Sub DTPicker1_Change()
On Error Resume Next
    RaiseEvent Change
End Sub

Private Sub DTPicker1_Click()
On Error Resume Next
    RaiseEvent Click
End Sub

Private Sub DTPicker1_DblClick()
On Error Resume Next
    RaiseEvent DblClick
End Sub

Private Sub DTPicker1_DropDown()
On Error Resume Next
    RaiseEvent DropDown
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
On Error Resume Next
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
On Error Resume Next
    UserControl.Enabled() = New_Enabled
    DTPicker1.Enabled = New_Enabled
    Picture1.Enabled = New_Enabled
    DrawButton
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,Font
Public Property Get Font() As Font
On Error Resume Next
    Set Font = DTPicker1.Font
End Property

Public Property Set Font(ByVal New_Font As Font)
On Error Resume Next
    Set DTPicker1.Font = New_Font
    DrawCombo
    PropertyChanged "Font"
End Property

Private Sub DTPicker1_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub DTPicker1_KeyPress(KeyAscii As Integer)
On Error Resume Next
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub DTPicker1_KeyUp(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
Dim Index As Integer
On Error Resume Next

    
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    Set DTPicker1.Font = PropBag.ReadProperty("Font", Ambient.Font)
    m_ButtonBackColor = PropBag.ReadProperty("ButtonBackColor", m_def_ButtonBackColor)
    m_HoverBackColor = PropBag.ReadProperty("HoverBackColor", m_def_HoverBackColor)
    m_PressBackColor = PropBag.ReadProperty("PressBackColor", m_def_PressBackColor)
    m_DisabledBackColor = PropBag.ReadProperty("DisabledBackColor", m_def_DisabledBackColor)
    m_ButtonForeColor = PropBag.ReadProperty("ButtonForeColor", m_def_ButtonForeColor)
    m_HoverForeColor = PropBag.ReadProperty("HoverForeColor", m_def_HoverForeColor)
    m_PressForeColor = PropBag.ReadProperty("PressForeColor", m_def_PressForeColor)
    m_DisabledForeColor = PropBag.ReadProperty("DisabledForeColor", m_def_DisabledForeColor)

    DTPicker1.CalendarBackColor = PropBag.ReadProperty("CalendarBackColor", m_def_CalendarBackColor)
    DTPicker1.CalendarForeColor = PropBag.ReadProperty("CalendarForeColor", m_def_CalendarForeColor)
    DTPicker1.CalendarTitleBackColor = PropBag.ReadProperty("CalendarTitleBackColor", m_def_CalendarTitleBackColor)
    DTPicker1.CalendarTitleForeColor = PropBag.ReadProperty("CalendarTitleForeColor", m_def_CalendarTitleForeColor)
    DTPicker1.CalendarTrailingForeColor = PropBag.ReadProperty("CalendarTrailingForeColor", m_def_CalendarTrailingForeColor)
    DTPicker1.CheckBox = PropBag.ReadProperty("CheckBox", False)
    DTPicker1.CustomFormat = PropBag.ReadProperty("CustomFormat", "")
    DTPicker1.Format = PropBag.ReadProperty("Format", 1)
    DTPicker1.MaxDate = PropBag.ReadProperty("MaxDate", #12/31/9999#)
    DTPicker1.MinDate = PropBag.ReadProperty("MinDate", #1/1/1601#)
    DTPicker1.UpDown = PropBag.ReadProperty("UpDown", False)
    DTPicker1.Value = PropBag.ReadProperty("Value", VBA.Date)
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
Dim Index As Integer
On Error Resume Next

    Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
    Call PropBag.WriteProperty("Font", DTPicker1.Font, Ambient.Font)
    Call PropBag.WriteProperty("ButtonBackColor", m_ButtonBackColor, m_def_ButtonBackColor)
    Call PropBag.WriteProperty("HoverBackColor", m_HoverBackColor, m_def_HoverBackColor)
    Call PropBag.WriteProperty("PressBackColor", m_PressBackColor, m_def_PressBackColor)
    Call PropBag.WriteProperty("DisabledBackColor", m_DisabledBackColor, m_def_DisabledBackColor)
    Call PropBag.WriteProperty("ButtonForeColor", m_ButtonForeColor, m_def_ButtonForeColor)
    Call PropBag.WriteProperty("HoverForeColor", m_HoverForeColor, m_def_HoverForeColor)
    Call PropBag.WriteProperty("PressForeColor", m_PressForeColor, m_def_PressForeColor)
    Call PropBag.WriteProperty("DisabledForeColor", m_DisabledForeColor, m_def_DisabledForeColor)
    Call PropBag.WriteProperty("CalendarBackColor", DTPicker1.CalendarBackColor, m_def_CalendarBackColor)
    Call PropBag.WriteProperty("CalendarForeColor", DTPicker1.CalendarForeColor, m_def_CalendarForeColor)
    Call PropBag.WriteProperty("CalendarTitleBackColor", DTPicker1.CalendarTitleBackColor, m_def_CalendarTitleBackColor)
    Call PropBag.WriteProperty("CalendarTitleForeColor", DTPicker1.CalendarTitleForeColor, m_def_CalendarTitleForeColor)
    Call PropBag.WriteProperty("CalendarTrailingForeColor", DTPicker1.CalendarTrailingForeColor, m_def_CalendarTrailingForeColor)
    Call PropBag.WriteProperty("CheckBox", DTPicker1.CheckBox, False)
    Call PropBag.WriteProperty("CustomFormat", DTPicker1.CustomFormat, "")
    Call PropBag.WriteProperty("Format", DTPicker1.Format, 1)
    Call PropBag.WriteProperty("MaxDate", DTPicker1.MaxDate, #12/31/9999#)
    Call PropBag.WriteProperty("MinDate", DTPicker1.MinDate, #1/1/1601#)
    Call PropBag.WriteProperty("UpDown", DTPicker1.UpDown, False)
    Call PropBag.WriteProperty("Value", DTPicker1.Value, VBA.Date)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ButtonBackColor() As OLE_COLOR
On Error Resume Next
    ButtonBackColor = m_ButtonBackColor
End Property

Public Property Let ButtonBackColor(ByVal New_ButtonBackColor As OLE_COLOR)
On Error Resume Next
    m_ButtonBackColor = New_ButtonBackColor
    DrawButton
    PropertyChanged "ButtonBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverBackColor() As OLE_COLOR
On Error Resume Next
    HoverBackColor = m_HoverBackColor
End Property

Public Property Let HoverBackColor(ByVal New_HoverBackColor As OLE_COLOR)
On Error Resume Next
    m_HoverBackColor = New_HoverBackColor
    DrawButton
    PropertyChanged "HoverBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressBackColor() As OLE_COLOR
On Error Resume Next
    PressBackColor = m_PressBackColor
End Property

Public Property Let PressBackColor(ByVal New_PressBackColor As OLE_COLOR)
On Error Resume Next
    m_PressBackColor = New_PressBackColor
    DrawButton
    PropertyChanged "PressBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledBackColor() As OLE_COLOR
On Error Resume Next
    DisabledBackColor = m_DisabledBackColor
End Property

Public Property Let DisabledBackColor(ByVal New_DisabledBackColor As OLE_COLOR)
On Error Resume Next
    m_DisabledBackColor = New_DisabledBackColor
    DrawButton
    PropertyChanged "DisabledBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ButtonForeColor() As OLE_COLOR
On Error Resume Next
    ButtonForeColor = m_ButtonForeColor
End Property

Public Property Let ButtonForeColor(ByVal New_ButtonForeColor As OLE_COLOR)
On Error Resume Next
    m_ButtonForeColor = New_ButtonForeColor
    DrawButton
    PropertyChanged "ButtonForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverForeColor() As OLE_COLOR
On Error Resume Next
    HoverForeColor = m_HoverForeColor
End Property

Public Property Let HoverForeColor(ByVal New_HoverForeColor As OLE_COLOR)
On Error Resume Next
    m_HoverForeColor = New_HoverForeColor
    DrawButton
    PropertyChanged "HoverForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressForeColor() As OLE_COLOR
On Error Resume Next
    PressForeColor = m_PressForeColor
End Property

Public Property Let PressForeColor(ByVal New_PressForeColor As OLE_COLOR)
On Error Resume Next
    m_PressForeColor = New_PressForeColor
    DrawButton
    PropertyChanged "PressForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledForeColor() As OLE_COLOR
On Error Resume Next
    DisabledForeColor = m_DisabledForeColor
End Property

Public Property Let DisabledForeColor(ByVal New_DisabledForeColor As OLE_COLOR)
On Error Resume Next
    m_DisabledForeColor = New_DisabledForeColor
    DrawButton
    PropertyChanged "DisabledForeColor"
End Property

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    Set DTPicker1.Font = Ambient.Font
    DTPicker1.CalendarBackColor = m_def_CalendarBackColor
    DTPicker1.CalendarForeColor = m_def_CalendarForeColor
    DTPicker1.CalendarTitleBackColor = m_def_CalendarTitleBackColor
    DTPicker1.CalendarTitleForeColor = m_def_CalendarTitleForeColor
    DTPicker1.CalendarTrailingForeColor = m_def_CalendarTrailingForeColor
    
    
    m_ButtonBackColor = m_def_ButtonBackColor
    m_HoverBackColor = m_def_HoverBackColor
    m_PressBackColor = m_def_PressBackColor
    m_DisabledBackColor = m_def_DisabledBackColor
    m_ButtonForeColor = m_def_ButtonForeColor
    m_HoverForeColor = m_def_HoverForeColor
    m_PressForeColor = m_def_PressForeColor
    m_DisabledForeColor = m_def_DisabledForeColor
End Sub




Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)
On Error Resume Next
    
    miActArrow = 0
    If Y < ScaleHeight / 2 Then
        miActArrow = 1
    Else
        miActArrow = -1
    End If
    
    m_Pressed = True
    DrawButton
    'RaiseEvent MouseDown(Button, Shift, X, Y)
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
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

    If WindowFromPoint(curPos.X, curPos.Y) <> Picture1.hwnd Then
     'If (X < 0) Or (Y < 0) Or (X > UserControl.Width) Or (Y > UserControl.Height) Then
          bTemp = False
          If GetCapture = Picture1.hwnd Then Call ReleaseCapture
     Else
          ' the mouse is still over the button, so be sure to call SetCapture again
          bTemp = True
          If GetCapture <> Picture1.hwnd Then Call SetCapture(Picture1.hwnd)
     End If


     ' only paint if necessary
'     If m_Hover <> bTemp Then
          If Button <> 0 Then m_Pressed = m_Pressed And True
          m_Hover = bTemp
          If m_Hover = False Then m_Pressed = False
          DrawButton
'     End If

'    RaiseEvent MouseMove(Button, Shift, X, Y)
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
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
    
    DrawButton
    
    If bTemp1 Then
        If DTPicker1.UpDown Then
            If miActArrow = 1 Then
                SendKeys "{UP}"
            Else
                SendKeys "{DOWN}"
            End If
        Else
            DropListDown
        End If
    End If

'    RaiseEvent MouseUp(Button, Shift, X, Y)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CalendarBackColor
Public Property Get CalendarBackColor() As OLE_COLOR
Attribute CalendarBackColor.VB_Description = "Returns/sets the background color used to display the month portion of the dropdown calendar."
On Error Resume Next
    CalendarBackColor = DTPicker1.CalendarBackColor
End Property

Public Property Let CalendarBackColor(ByVal New_CalendarBackColor As OLE_COLOR)
On Error Resume Next
    DTPicker1.CalendarBackColor() = New_CalendarBackColor
    PropertyChanged "CalendarBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CalendarForeColor
Public Property Get CalendarForeColor() As OLE_COLOR
Attribute CalendarForeColor.VB_Description = "Returns/sets the foreground color used to display text in the month portion of the dropdown calendar."
On Error Resume Next
    CalendarForeColor = DTPicker1.CalendarForeColor
End Property

Public Property Let CalendarForeColor(ByVal New_CalendarForeColor As OLE_COLOR)
On Error Resume Next
    DTPicker1.CalendarForeColor() = New_CalendarForeColor
    PropertyChanged "CalendarForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CalendarTitleBackColor
Public Property Get CalendarTitleBackColor() As OLE_COLOR
Attribute CalendarTitleBackColor.VB_Description = "Returns/sets the background color used to display the title portion of the dropdown calendar."
On Error Resume Next
    CalendarTitleBackColor = DTPicker1.CalendarTitleBackColor
End Property

Public Property Let CalendarTitleBackColor(ByVal New_CalendarTitleBackColor As OLE_COLOR)
On Error Resume Next
    DTPicker1.CalendarTitleBackColor() = New_CalendarTitleBackColor
    PropertyChanged "CalendarTitleBackColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CalendarTitleForeColor
Public Property Get CalendarTitleForeColor() As OLE_COLOR
Attribute CalendarTitleForeColor.VB_Description = "Returns/sets the foreground color used to display the title portion of the dropdown calendar."
On Error Resume Next
    CalendarTitleForeColor = DTPicker1.CalendarTitleForeColor
End Property

Public Property Let CalendarTitleForeColor(ByVal New_CalendarTitleForeColor As OLE_COLOR)
On Error Resume Next
    DTPicker1.CalendarTitleForeColor() = New_CalendarTitleForeColor
    PropertyChanged "CalendarTitleForeColor"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CalendarTrailingForeColor
Public Property Get CalendarTrailingForeColor() As OLE_COLOR
Attribute CalendarTrailingForeColor.VB_Description = "Returns/sets the foreground color used to display the days at the beginning and end of the dropdown calendar that are from previous and following months."
On Error Resume Next
    CalendarTrailingForeColor = DTPicker1.CalendarTrailingForeColor
End Property

Public Property Let CalendarTrailingForeColor(ByVal New_CalendarTrailingForeColor As OLE_COLOR)
On Error Resume Next
    DTPicker1.CalendarTrailingForeColor() = New_CalendarTrailingForeColor
    PropertyChanged "CalendarTrailingForeColor"
End Property

Private Sub DTPicker1_CallbackKeyDown(ByVal KeyCode As Integer, ByVal Shift As Integer, ByVal CallbackField As String, CallbackDate As Date)
On Error Resume Next
    RaiseEvent CallbackKeyDown(KeyCode, Shift, CallbackField, CallbackDate)
End Sub
''''
'''''WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'''''MappingInfo=DTPicker1,DTPicker1,-1,CheckBox
''''Public Property Get CheckBox() As Boolean
''''    CheckBox = DTPicker1.CheckBox
''''End Property
''''
''''Public Property Let CheckBox(ByVal New_CheckBox As Boolean)
''''    DTPicker1.CheckBox() = New_CheckBox
''''    PropertyChanged "CheckBox"
''''End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,CustomFormat
Public Property Get CustomFormat() As String
Attribute CustomFormat.VB_Description = "Returns/sets the custom format string used to format the date and/or time displayed in the control."
On Error Resume Next
    CustomFormat = DTPicker1.CustomFormat
End Property

Public Property Let CustomFormat(ByVal New_CustomFormat As String)
On Error Resume Next
    DTPicker1.CustomFormat() = New_CustomFormat
    PropertyChanged "CustomFormat"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,Format
Public Property Get Format() As FormatConstants
Attribute Format.VB_Description = "Determines whether dates and times are displayed using standard or custom formatting."
On Error Resume Next
    Format = DTPicker1.Format
End Property

Public Property Let Format(ByVal New_Format As FormatConstants)
On Error Resume Next
    DTPicker1.Format() = New_Format
    PropertyChanged "Format"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,MaxDate
Public Property Get MaxDate() As Date
Attribute MaxDate.VB_Description = "Returns/sets the latest date that can be displayed or accepted by the control."
On Error Resume Next
    MaxDate = DTPicker1.MaxDate
End Property

Public Property Let MaxDate(ByVal New_MaxDate As Date)
On Error Resume Next
    DTPicker1.MaxDate() = New_MaxDate
    PropertyChanged "MaxDate"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,MinDate
Public Property Get MinDate() As Date
Attribute MinDate.VB_Description = "Returns/sets the earliest date that can be displayed or accepted by the control."
On Error Resume Next
    MinDate = DTPicker1.MinDate
End Property

Public Property Let MinDate(ByVal New_MinDate As Date)
On Error Resume Next
    DTPicker1.MinDate() = New_MinDate
    PropertyChanged "MinDate"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,UpDown
Public Property Get UpDown() As Boolean
    UpDown = DTPicker1.UpDown
End Property

Public Property Let UpDown(ByVal New_UpDown As Boolean)
    DTPicker1.UpDown() = New_UpDown
    DrawButton
    PropertyChanged "UpDown"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=DTPicker1,DTPicker1,-1,Value
Public Property Get Value() As Variant
Attribute Value.VB_Description = "Returns/sets the current date."
On Error Resume Next
    Value = DTPicker1.Value
End Property

Public Property Let Value(ByVal New_Value As Variant)
On Error Resume Next
    DTPicker1.Value() = New_Value
    PropertyChanged "Value"
End Property


Private Sub DTPicker1_CloseUp()
On Error Resume Next
    RaiseEvent CloseUp
End Sub

Private Sub DTPicker1_FormatSize(ByVal CallbackField As String, Size As Integer)
On Error Resume Next
    RaiseEvent FormatSize(CallbackField, Size)
End Sub

Public Property Get hwnd() As Long
On Error Resume Next
    hwnd = DTPicker1.hwnd
End Property

