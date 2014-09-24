VERSION 5.00
Begin VB.UserControl FlatPrograssBar 
   Alignable       =   -1  'True
   Appearance      =   0  'Flat
   AutoRedraw      =   -1  'True
   BackColor       =   &H80000005&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   720
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   2385
   ScaleHeight     =   48
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   159
End
Attribute VB_Name = "FlatPrograssBar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'Default Property Values:
Const m_def_Max = 100
Const m_def_Min = 0
Const m_def_Value = 50
Const m_def_ForeColor1 = &H94D7F7
Const m_def_ForeColor2 = &H104194

'Property Variables:
Dim m_Max As Long
Dim m_Min As Long
Dim m_Value As Long
Dim m_ForeColor1 As OLE_COLOR
Dim m_ForeColor2 As OLE_COLOR



'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,BackColor
Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
On Error Resume Next
    BackColor = UserControl.BackColor
End Property

Public Property Let BackColor(ByVal New_BackColor As OLE_COLOR)
On Error Resume Next
    UserControl.BackColor() = New_BackColor
    UserControl_Paint
    PropertyChanged "BackColor"
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
    UserControl_Paint
    PropertyChanged "Enabled"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,BorderStyle
Public Property Get BorderStyle() As Integer
Attribute BorderStyle.VB_Description = "Returns/sets the border style for an object."
On Error Resume Next
    BorderStyle = UserControl.BorderStyle
End Property

Public Property Let BorderStyle(ByVal New_BorderStyle As Integer)
On Error Resume Next
    UserControl.BorderStyle() = New_BorderStyle
    UserControl_Paint
    PropertyChanged "BorderStyle"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MappingInfo=UserControl,UserControl,-1,Refresh
Public Sub Refresh()
Attribute Refresh.VB_Description = "Forces a complete repaint of a object."
On Error Resume Next
    UserControl_Paint
End Sub

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,100
Public Property Get Max() As Long
On Error Resume Next
    Max = m_Max
End Property

Public Property Let Max(ByVal New_Max As Long)
On Error Resume Next
    m_Max = New_Max
    If m_Max <= m_Min Then m_Max = m_Min + 1
    UserControl_Paint
    PropertyChanged "Max"
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
    If m_Max <= m_Min Then m_Min = m_Max - 1
    UserControl_Paint
    PropertyChanged "Min"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=8,0,0,0
Public Property Get Value() As Long
Attribute Value.VB_UserMemId = 0
On Error Resume Next
    Value = m_Value
End Property

Public Property Let Value(ByVal New_Value As Long)
On Error Resume Next
    m_Value = New_Value
    If m_Value > m_Max Then m_Value = m_Max
    If m_Value < m_Min Then m_Value = m_Min
    UserControl_Paint
    PropertyChanged "Value"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ForeColor1() As OLE_COLOR
On Error Resume Next
    ForeColor1 = m_ForeColor1
End Property

Public Property Let ForeColor1(ByVal New_ForeColor1 As OLE_COLOR)
On Error Resume Next
    m_ForeColor1 = New_ForeColor1
    UserControl_Paint
    PropertyChanged "ForeColor1"
End Property

'WARNING! DO NOT REMOVE OR MODIFY THE FOLLOWING COMMENTED LINES!
'MemberInfo=10,0,0,0
Public Property Get ForeColor2() As OLE_COLOR
On Error Resume Next
    ForeColor2 = m_ForeColor2
End Property

Public Property Let ForeColor2(ByVal New_ForeColor2 As OLE_COLOR)
On Error Resume Next
    m_ForeColor2 = New_ForeColor2
    UserControl_Paint
    PropertyChanged "ForeColor2"
End Property

'Initialize Properties for User Control
Private Sub UserControl_InitProperties()
On Error Resume Next
    m_Max = m_def_Max
    m_Min = m_def_Min
    m_Value = m_def_Value
    m_ForeColor1 = m_def_ForeColor1
    m_ForeColor2 = m_def_ForeColor2
End Sub

Private Sub UserControl_Paint()
On Error Resume Next
    DrawProgress
End Sub

'Load property values from storage
Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
On Error Resume Next

    UserControl.BackColor = PropBag.ReadProperty("BackColor", &HFFFFFF)
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    UserControl.BorderStyle = PropBag.ReadProperty("BorderStyle", 1)
    m_Max = PropBag.ReadProperty("Max", m_def_Max)
    m_Min = PropBag.ReadProperty("Min", m_def_Min)
    m_Value = PropBag.ReadProperty("Value", m_def_Value)
    m_ForeColor1 = PropBag.ReadProperty("ForeColor1", m_def_ForeColor1)
    m_ForeColor2 = PropBag.ReadProperty("ForeColor2", m_def_ForeColor2)
End Sub

Private Sub UserControl_Resize()
On Error Resume Next
    UserControl_Paint
End Sub

'Write property values to storage
Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
On Error Resume Next

    Call PropBag.WriteProperty("BackColor", UserControl.BackColor, &HFFFFFF)
    Call PropBag.WriteProperty("Enabled", UserControl.Enabled, True)
    Call PropBag.WriteProperty("BorderStyle", UserControl.BorderStyle, 1)
    Call PropBag.WriteProperty("Max", m_Max, m_def_Max)
    Call PropBag.WriteProperty("Min", m_Min, m_def_Min)
    Call PropBag.WriteProperty("Value", m_Value, m_def_Value)
    Call PropBag.WriteProperty("ForeColor1", m_ForeColor1, m_def_ForeColor1)
    Call PropBag.WriteProperty("ForeColor2", m_ForeColor2, m_def_ForeColor2)
End Sub


Private Sub DrawProgress()
Const LINEWIDTH = 5
Const SPACEWIDTH = 5
Dim i As Long
Dim iLineCount As Long
Dim iLineCount2 As Long
Dim X1 As Long, X2 As Long, Y1 As Long, Y2 As Long
On Error Resume Next

    iLineCount = ScaleWidth \ (LINEWIDTH + SPACEWIDTH)
    iLineCount2 = iLineCount * ((m_Value - m_Min) / (m_Max - m_Min))
    
    Cls
    
    FillColor = m_ForeColor2
    ForeColor = m_ForeColor2
    For i = 1 To iLineCount2
        X1 = (i - 1) * (LINEWIDTH + SPACEWIDTH) + SPACEWIDTH
        X2 = X1 + LINEWIDTH
        Y1 = SPACEWIDTH
        Y2 = ScaleHeight - SPACEWIDTH
        Line (X1, Y1)-(X2, Y2), , BF
    Next i
    
    FillColor = m_ForeColor1
    ForeColor = m_ForeColor1
    For i = iLineCount2 + 1 To iLineCount
        X1 = (i - 1) * (LINEWIDTH + SPACEWIDTH) + SPACEWIDTH
        X2 = X1 + LINEWIDTH
        Y1 = SPACEWIDTH
        Y2 = ScaleHeight - SPACEWIDTH
        Line (X1, Y1)-(X2, Y2), , BF
    Next i
    
End Sub

Public Property Get hWnd() As Variant
On Error Resume Next
    hWnd = UserControl.hWnd
End Property
