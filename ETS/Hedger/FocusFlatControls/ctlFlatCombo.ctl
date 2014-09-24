VERSION 5.00
Begin VB.UserControl FlatCombo 
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
   ToolboxBitmap   =   "ctlFlatCombo.ctx":0000
   Begin VB.ComboBox Combo2 
      Height          =   315
      Left            =   0
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   480
      Width           =   1335
   End
   Begin VB.ComboBox Combo1 
      Height          =   315
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   1455
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
      Top             =   0
      Width           =   495
   End
End
Attribute VB_Name = "FlatCombo"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Public Enum eComboType
    ctDropdownCombo = 0
    ctDropdownList = 2
End Enum

Private WithEvents Combo As VB.ComboBox
Attribute Combo.VB_VarHelpID = -1




Private marrComboRgn(3) As POINTAPI
Private miComboRgnPtr As Long

'Event Declarations:
Event Change() 'MappingInfo=Combo,Combo,-1,Change
Attribute Change.VB_Description = "Occurs when the contents of a control have changed."
Event Click() 'MappingInfo=Combo,Combo,-1,Click
Event DblClick() 'MappingInfo=Combo,Combo,-1,DblClick
Event DropDown() 'MappingInfo=Combo,Combo,-1,DropDown
Event KeyDown(KeyCode As Integer, Shift As Integer) 'MappingInfo=Combo,Combo,-1,KeyDown
Attribute KeyDown.VB_Description = "Occurs when the user presses a key while an object has the focus."
Event KeyPress(KeyAscii As Integer) 'MappingInfo=Combo,Combo,-1,KeyPress
Attribute KeyPress.VB_Description = "Occurs when the user presses and releases an ANSI key."
Event KeyUp(KeyCode As Integer, Shift As Integer) 'MappingInfo=Combo,Combo,-1,KeyUp
Attribute KeyUp.VB_Description = "Occurs when the user releases a key while an object has the focus."
Event MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
Event MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
Event MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)

'Default Property Values:
Const m_def_Style = ctDropdownCombo


'Property Variables:
Dim m_Style As eComboType
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

Dim m_Sorted As Boolean




Private Sub DrawButton()
Dim i As Long
Dim midx As Long
Dim midy As Long
Dim h As Long, dw As Long
Dim iBackColor As Long
Dim iForeColor As Long

On Error Resume Next


    ' border
    UserControl.BackColor = Combo.BackColor
    Cls
    DrawWidth = 1
    Line (0, 0)-(ScaleWidth - 1, ScaleHeight - 1), vbBlack, B
    
    
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


Private Sub DrawCombo()
On Error Resume Next
    
    EnsureCombo1
    
    With Combo
        .Width = ScaleWidth
    
        marrComboRgn(0).x = 3
        marrComboRgn(0).Y = 3
        marrComboRgn(1).x = .Width - 17
        marrComboRgn(1).Y = marrComboRgn(0).Y
        marrComboRgn(2).x = marrComboRgn(1).x
        marrComboRgn(2).Y = .Height - 3
        marrComboRgn(3).x = marrComboRgn(0).x
        marrComboRgn(3).Y = marrComboRgn(2).Y
    
        If miComboRgnPtr <> 0 Then DeleteObject miComboRgnPtr
        miComboRgnPtr = CreatePolygonRgn(marrComboRgn(0), 4, 1)
        SetWindowRgn .hWnd, miComboRgnPtr, True
        
        UserControl.Size (.Width) * Screen.TwipsPerPixelX, (.Height) * Screen.TwipsPerPixelY
    End With
    
    With Picture1
        .Left = ScaleWidth - 15
        .Top = 0
        .Width = 15
        .Height = ScaleHeight
    End With
    DrawButton

    EnsureCombo2
End Sub

'-----------------------------
' dropping list down
Private Sub DropListDown()
On Error Resume Next
    EnsureCombo1
'    modMain.SetFocus Combo.hwnd
    Call ReleaseCapture
    SendMessage Combo.hWnd, CB_SHOWDROPDOWN, True, 0&
    EnsureCombo2
End Sub

Private Sub Picture1_GotFocus()
On Error Resume Next
    EnsureCombo1
    Combo.SetFocus
    EnsureCombo2
End Sub

Private Sub UserControl_Hide()
    EnsureCombo1
    Set Combo = Nothing
    EnsureCombo2
End Sub

Private Sub UserControl_Initialize()
'
End Sub

Private Sub UserControl_Resize()
On Error Resume Next
    EnsureCombo1
    DrawCombo
    EnsureCombo1
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,AddItem
Public Sub AddItem(ByVal Item As String, Optional ByVal Index As Variant)
Dim i As Long
Dim b As Boolean
On Error Resume Next
        
    EnsureCombo1
        
    If m_Sorted And IsMissing(Index) And (Combo.ListCount > 0) Then
        
        b = False
        For i = 0 To Combo.ListCount - 1
            If UCase(Item) <= UCase(Combo.List(i)) Then
                Combo.AddItem Item, i
                b = True
                Exit For
            End If
        Next i
        If Not b Then
            Combo.AddItem Item
        End If
        
    Else
        Combo.AddItem Item, Index
    End If

    EnsureCombo2
End Sub

Private Sub Combo_Change()
On Error Resume Next
    RaiseEvent Change
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,Clear
Public Sub Clear()
On Error Resume Next
    EnsureCombo1
    Combo.Clear
    EnsureCombo2
End Sub

Private Sub Combo_Click()
On Error Resume Next
    RaiseEvent Click
End Sub

Private Sub Combo_DblClick()
On Error Resume Next
    RaiseEvent DblClick
End Sub

Private Sub Combo_DropDown()
On Error Resume Next
    RaiseEvent DropDown
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Enabled
Public Property Get Enabled() As Boolean
On Error Resume Next
    EnsureCombo1
    Enabled = UserControl.Enabled
    EnsureCombo2
End Property

Public Property Let Enabled(ByVal New_Enabled As Boolean)
On Error Resume Next
    EnsureCombo1
    UserControl.Enabled() = New_Enabled
    Combo.Enabled = New_Enabled
    Picture1.Enabled = New_Enabled
    DrawButton
    PropertyChanged "Enabled"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,Font
Public Property Get Font() As StdFont
On Error Resume Next
    EnsureCombo1
    Set Font = Combo.Font
    EnsureCombo2
End Property

Public Property Set Font(ByVal New_Font As StdFont)
On Error Resume Next
    EnsureCombo1
    Set Combo.Font = New_Font
    DrawCombo
    PropertyChanged "Font"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,ItemData
Public Property Get ItemData(ByVal Index As Integer) As Long
On Error Resume Next
    EnsureCombo1
    ItemData = Combo.ItemData(Index)
    EnsureCombo2
End Property

Public Property Let ItemData(ByVal Index As Integer, ByVal New_ItemData As Long)
On Error Resume Next
    EnsureCombo1
    Combo.ItemData(Index) = New_ItemData
    PropertyChanged "ItemData"
    EnsureCombo2
End Property

Public Property Get NewIndex() As Long
On Error Resume Next
    EnsureCombo1
    NewIndex = Combo.NewIndex
    EnsureCombo2
End Property


Private Sub Combo_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyDown(KeyCode, Shift)
End Sub

Private Sub Combo_KeyPress(KeyAscii As Integer)
On Error Resume Next
    RaiseEvent KeyPress(KeyAscii)
End Sub

Private Sub Combo_KeyUp(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    RaiseEvent KeyUp(KeyCode, Shift)
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,List
Public Property Get List(ByVal Index As Integer) As String
On Error Resume Next
    EnsureCombo1
    List = Combo.List(Index)
    EnsureCombo2
End Property

Public Property Let List(ByVal Index As Integer, ByVal New_List As String)
On Error Resume Next
    EnsureCombo1
    Combo.List(Index) = New_List
    PropertyChanged "List"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,ListCount
Public Property Get ListCount() As Integer
On Error Resume Next
    EnsureCombo1
    ListCount = Combo.ListCount
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,ListIndex
Public Property Get ListIndex() As Integer
On Error Resume Next
    EnsureCombo1
    ListIndex = Combo.ListIndex
    EnsureCombo2
End Property

Public Property Let ListIndex(ByVal New_ListIndex As Integer)
On Error Resume Next
    EnsureCombo1
    Combo.ListIndex() = New_ListIndex
    PropertyChanged "ListIndex"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,Locked
Public Property Get Locked() As Boolean
On Error Resume Next
    EnsureCombo1
    Locked = Combo.Locked
    EnsureCombo2
End Property

Public Property Let Locked(ByVal New_Locked As Boolean)
On Error Resume Next
    EnsureCombo1
    Combo.Locked() = New_Locked
    PropertyChanged "Locked"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,RemoveItem
Public Sub RemoveItem(ByVal Index As Integer)
On Error Resume Next
    EnsureCombo1
    Combo.RemoveItem Index
    EnsureCombo2
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,SelLength
Public Property Get SelLength() As Long
On Error Resume Next
    EnsureCombo1
    SelLength = Combo.SelLength
    EnsureCombo2
End Property

Public Property Let SelLength(ByVal New_SelLength As Long)
On Error Resume Next
    EnsureCombo1
    Combo.SelLength() = New_SelLength
    PropertyChanged "SelLength"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,SelStart
Public Property Get SelStart() As Long
On Error Resume Next
    EnsureCombo1
    SelStart = Combo.SelStart
    EnsureCombo2
End Property

Public Property Let SelStart(ByVal New_SelStart As Long)
On Error Resume Next
    EnsureCombo1
    Combo.SelStart() = New_SelStart
    PropertyChanged "SelStart"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,SelText
Public Property Get SelText() As String
On Error Resume Next
    EnsureCombo1
    SelText = Combo.SelText
    EnsureCombo2
End Property

Public Property Let SelText(ByVal New_SelText As String)
On Error Resume Next
    EnsureCombo1
    Combo.SelText() = New_SelText
    PropertyChanged "SelText"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,Text
Public Property Get Text() As String
Attribute Text.VB_ProcData.VB_Invoke_Property = ";Text"
Attribute Text.VB_UserMemId = 0
On Error Resume Next
    EnsureCombo1
    Text = Combo.Text
    EnsureCombo2
End Property

Public Property Let Text(ByVal New_Text As String)
On Error Resume Next
    EnsureCombo1
    Combo.Text() = New_Text
    PropertyChanged "Text"
    EnsureCombo2
End Property

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
Dim Index As Integer
On Error Resume Next

    m_Style = PropBag.ReadProperty("Style", m_def_Style)
    ApplyStyle
    
    
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    Set Combo.Font = PropBag.ReadProperty("Font", Ambient.Font)
    Combo.Locked = PropBag.ReadProperty("Locked", False)
    m_ButtonBackColor = PropBag.ReadProperty("ButtonBackColor", m_def_ButtonBackColor)
    m_HoverBackColor = PropBag.ReadProperty("HoverBackColor", m_def_HoverBackColor)
    m_PressBackColor = PropBag.ReadProperty("PressBackColor", m_def_PressBackColor)
    m_DisabledBackColor = PropBag.ReadProperty("DisabledBackColor", m_def_DisabledBackColor)
    m_ButtonForeColor = PropBag.ReadProperty("ButtonForeColor", m_def_ButtonForeColor)
    m_HoverForeColor = PropBag.ReadProperty("HoverForeColor", m_def_HoverForeColor)
    m_PressForeColor = PropBag.ReadProperty("PressForeColor", m_def_PressForeColor)
    m_DisabledForeColor = PropBag.ReadProperty("DisabledForeColor", m_def_DisabledForeColor)
    Combo.BackColor = PropBag.ReadProperty("BackColor", &H80000005)
    Combo.ForeColor = PropBag.ReadProperty("ForeColor", &H80000008)
    m_Sorted = PropBag.ReadProperty("Sorted", False)

On Error Resume Next
    Combo.Text = PropBag.ReadProperty("Text", "Combo3")

End Sub

Private Sub UserControl_Show()
    EnsureCombo1
End Sub

Private Sub UserControl_Terminate()
'
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
Dim Index As Integer
On Error Resume Next

    EnsureCombo1
    Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
    Call PropBag.WriteProperty("Font", Combo.Font, Ambient.Font)
    Call PropBag.WriteProperty("Locked", Combo.Locked, False)
    Call PropBag.WriteProperty("Text", Combo.Text, "Combo3")
    Call PropBag.WriteProperty("Style", m_Style, m_def_Style)
    Call PropBag.WriteProperty("ButtonBackColor", m_ButtonBackColor, m_def_ButtonBackColor)
    Call PropBag.WriteProperty("HoverBackColor", m_HoverBackColor, m_def_HoverBackColor)
    Call PropBag.WriteProperty("PressBackColor", m_PressBackColor, m_def_PressBackColor)
    Call PropBag.WriteProperty("DisabledBackColor", m_DisabledBackColor, m_def_DisabledBackColor)
    Call PropBag.WriteProperty("ButtonForeColor", m_ButtonForeColor, m_def_ButtonForeColor)
    Call PropBag.WriteProperty("HoverForeColor", m_HoverForeColor, m_def_HoverForeColor)
    Call PropBag.WriteProperty("PressForeColor", m_PressForeColor, m_def_PressForeColor)
    Call PropBag.WriteProperty("DisabledForeColor", m_DisabledForeColor, m_def_DisabledForeColor)
    Call PropBag.WriteProperty("BackColor", Combo.BackColor, &H80000005)
    Call PropBag.WriteProperty("ForeColor", Combo.ForeColor, &H80000008)
    Call PropBag.WriteProperty("Sorted", m_Sorted, False)
    EnsureCombo2
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Style() As eComboType
On Error Resume Next
    EnsureCombo1
    Style = m_Style
    EnsureCombo2
End Property

Public Property Let Style(ByVal New_Style As eComboType)
On Error Resume Next
    EnsureCombo1
    m_Style = New_Style
    ApplyStyle
    DrawCombo
    PropertyChanged "Style"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ButtonBackColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    ButtonBackColor = m_ButtonBackColor
    EnsureCombo2
End Property

Public Property Let ButtonBackColor(ByVal New_ButtonBackColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_ButtonBackColor = New_ButtonBackColor
    DrawButton
    PropertyChanged "ButtonBackColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverBackColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    HoverBackColor = m_HoverBackColor
    EnsureCombo2
End Property

Public Property Let HoverBackColor(ByVal New_HoverBackColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_HoverBackColor = New_HoverBackColor
    DrawButton
    PropertyChanged "HoverBackColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressBackColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    PressBackColor = m_PressBackColor
    EnsureCombo2
End Property

Public Property Let PressBackColor(ByVal New_PressBackColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_PressBackColor = New_PressBackColor
    DrawButton
    PropertyChanged "PressBackColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledBackColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    DisabledBackColor = m_DisabledBackColor
    EnsureCombo2
End Property

Public Property Let DisabledBackColor(ByVal New_DisabledBackColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_DisabledBackColor = New_DisabledBackColor
    DrawButton
    PropertyChanged "DisabledBackColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ButtonForeColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    ButtonForeColor = m_ButtonForeColor
    EnsureCombo2
End Property

Public Property Let ButtonForeColor(ByVal New_ButtonForeColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_ButtonForeColor = New_ButtonForeColor
    DrawButton
    PropertyChanged "ButtonForeColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get HoverForeColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    HoverForeColor = m_HoverForeColor
    EnsureCombo2
End Property

Public Property Let HoverForeColor(ByVal New_HoverForeColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_HoverForeColor = New_HoverForeColor
    DrawButton
    PropertyChanged "HoverForeColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get PressForeColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    PressForeColor = m_PressForeColor
    EnsureCombo2
End Property

Public Property Let PressForeColor(ByVal New_PressForeColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_PressForeColor = New_PressForeColor
    DrawButton
    PropertyChanged "PressForeColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get DisabledForeColor() As OLE_COLOR
On Error Resume Next
    EnsureCombo1
    DisabledForeColor = m_DisabledForeColor
    EnsureCombo2
End Property

Public Property Let DisabledForeColor(ByVal New_DisabledForeColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    m_DisabledForeColor = New_DisabledForeColor
    DrawButton
    PropertyChanged "DisabledForeColor"
    EnsureCombo2
End Property

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    m_Style = m_def_Style
    ApplyStyle
    
    Set Combo.Font = Ambient.Font
    m_ButtonBackColor = m_def_ButtonBackColor
    m_HoverBackColor = m_def_HoverBackColor
    m_PressBackColor = m_def_PressBackColor
    m_DisabledBackColor = m_def_DisabledBackColor
    m_ButtonForeColor = m_def_ButtonForeColor
    m_HoverForeColor = m_def_HoverForeColor
    m_PressForeColor = m_def_PressForeColor
    m_DisabledForeColor = m_def_DisabledForeColor
    m_Sorted = False
    EnsureCombo2
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
On Error Resume Next
    EnsureCombo1
    BackColor = Combo.BackColor
    EnsureCombo2
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    Combo.BackColor() = New_BackColor
    DrawButton
    PropertyChanged "BackColor"
    EnsureCombo2
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=Combo,Combo,-1,ForeColor
Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
On Error Resume Next
    EnsureCombo1
    ForeColor = Combo.ForeColor
    EnsureCombo2
End Property

Public Property Let ForeColor(ByVal New_ForeColor As OLE_COLOR)
On Error Resume Next
    EnsureCombo1
    Combo.ForeColor() = New_ForeColor
    DrawButton
    PropertyChanged "ForeColor"
    EnsureCombo2
End Property





Private Sub Picture1_MouseDown(Button As Integer, Shift As Integer, x As Single, Y As Single)
On Error Resume Next
    EnsureCombo1
    m_Pressed = True
    DrawButton
    RaiseEvent MouseDown(Button, Shift, x, Y)
    EnsureCombo2
End Sub

Private Sub Picture1_MouseMove(Button As Integer, Shift As Integer, x As Single, Y As Single)
Dim bTemp As Boolean
Dim curPos As POINTAPI
On Error Resume Next

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
          DrawButton
'     End If

    RaiseEvent MouseMove(Button, Shift, x, Y)
End Sub

Private Sub Picture1_MouseUp(Button As Integer, Shift As Integer, x As Single, Y As Single)
Dim bTemp1 As Boolean
Dim bTemp2 As Boolean
On Error Resume Next
    EnsureCombo1
    bTemp1 = m_Pressed
    Call ReleaseCapture
    
    If m_Pressed Then
        DropListDown
    End If
    
    
    m_Hover = False
    m_Pressed = False
    
    DrawButton

    RaiseEvent MouseUp(Button, Shift, x, Y)
    EnsureCombo2
End Sub


Private Sub ApplyStyle()
On Error Resume Next
    
    Set Combo = Nothing
    
    EnsureCombo1
    
    Combo.Move 0, 0, ScaleWidth
    DrawCombo
End Sub

Private Sub EnsureCombo1()
    If Combo Is Nothing Then
        If m_Style = ctDropdownCombo Then
            Combo2.Visible = False
            Set Combo = Combo1
        Else
            Combo1.Visible = False
            Set Combo = Combo2
        End If
        
        Combo.Visible = True
    End If
End Sub

Private Sub EnsureCombo2()
    If Not UserControl.Extender.Visible Then
        Set Combo = Nothing
    End If
End Sub

Public Property Get hWnd() As Long
On Error Resume Next
    EnsureCombo1
    hWnd = Combo.hWnd
    EnsureCombo2
End Property

Public Property Get Sorted() As Boolean
On Error Resume Next
    EnsureCombo1
    Sorted = m_Sorted
    EnsureCombo2
End Property

Public Property Let Sorted(ByVal NewValue As Boolean)
On Error Resume Next
    EnsureCombo1
    m_Sorted = NewValue
    PropertyChanged "Sorted"
    EnsureCombo2
End Property


Public Property Get TopIndex() As Integer
On Error Resume Next
    EnsureCombo1
    TopIndex = Combo.TopIndex
    EnsureCombo2
End Property

Public Property Let TopIndex(ByVal NewValue As Integer)
On Error Resume Next
    EnsureCombo1
    Combo.TopIndex = NewValue
    EnsureCombo2
End Property


