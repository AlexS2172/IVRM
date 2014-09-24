VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.UserControl ctlComboUpDown 
   BackStyle       =   0  'Transparent
   ClientHeight    =   3600
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   4800
   ScaleHeight     =   3600
   ScaleWidth      =   4800
   Begin MSComCtl2.UpDown ud 
      Height          =   315
      Left            =   2580
      TabIndex        =   1
      Top             =   420
      Width           =   195
      _ExtentX        =   344
      _ExtentY        =   556
      _Version        =   393216
      OrigLeft        =   2580
      OrigTop         =   420
      OrigRight       =   2775
      OrigBottom      =   735
      Wrap            =   -1  'True
      Enabled         =   -1  'True
   End
   Begin VB.ComboBox cmb 
      Height          =   315
      ItemData        =   "ctlComboUpDown.ctx":0000
      Left            =   300
      List            =   "ctlComboUpDown.ctx":0002
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   420
      Width           =   1755
   End
End
Attribute VB_Name = "ctlComboUpDown"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const SM_CXEDGE& = 45&
Private Const SM_CYEDGE& = 46&
Private Const SM_CXVSCROLL& = 2&
Private Declare Function GetSystemMetrics Lib "user32" (ByVal nIndex As Long) As Long

Private m_bUpDownInside As Boolean

'Private Const GWL_STYLE& = -16&
'Private Const CBS_SORT& = &H100&
'Private Declare Function GetWindowLong Lib "user32" Alias "GetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long) As Long
'Private Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hwnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long

Public Event Click()

Private Sub UserControl_Initialize()
    On Error Resume Next
    ud.BuddyControl = cmb
    ud.BuddyProperty = "ListIndex"
    ud.SyncBuddy = True
    ud.Wrap = True
    ud.Max = 0
    ud.Min = 0
    
    cmb.Left = 0
    cmb.Top = 0
End Sub

Private Sub UserControl_InitProperties()
    On Error Resume Next
    Set UserControl.Font = Ambient.Font
    Set cmb.Font = UserControl.Font
    m_bUpDownInside = False
    ud.Visible = False
End Sub

Private Sub UserControl_ReadProperties(PropBag As PropertyBag)
    On Error Resume Next
    cmb.BackColor = PropBag.ReadProperty("BackColor", &H80000005)
    cmb.ForeColor = PropBag.ReadProperty("ForeColor", &H80000012)
    
    UserControl.Enabled = PropBag.ReadProperty("Enabled", True)
    cmb.Enabled = UserControl.Enabled
    ud.Enabled = UserControl.Enabled
    
    Set UserControl.Font = PropBag.ReadProperty("Font", Ambient.Font)
    Set cmb.Font = UserControl.Font

    m_bUpDownInside = PropBag.ReadProperty("UpDownInside", False)
    ud.Visible = PropBag.ReadProperty("UpDownVisible", False)
    
    'SetComboSorted PropBag.ReadProperty("Sorted", True)
End Sub

Private Sub UserControl_WriteProperties(PropBag As PropertyBag)
    On Error Resume Next
    PropBag.WriteProperty "BackColor", cmb.BackColor, &H80000005
    PropBag.WriteProperty "ForeColor", cmb.ForeColor, &H80000012
    PropBag.WriteProperty "Enabled", UserControl.Enabled, True
    PropBag.WriteProperty "Font", UserControl.Font, Ambient.Font
    PropBag.WriteProperty "UpDownInside", m_bUpDownInside, False
    PropBag.WriteProperty "UpDownVisible", ud.Visible, False
    'PropBag.WriteProperty "Sorted", cmb.Sorted, True
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If ud.Visible Then
        If m_bUpDownInside Then
            cmb.Width = Width
            ud.Left = cmb.Left + cmb.Width - ScaleX(GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXEDGE), vbPixels, vbTwips)
            ud.Top = cmb.Top + ScaleY(GetSystemMetrics(SM_CYEDGE), vbPixels, vbTwips)
            ud.Height = cmb.Height - ScaleY(GetSystemMetrics(SM_CYEDGE) * 2, vbPixels, vbTwips)
        Else
            ud.Left = Width - ScaleX(GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXEDGE), vbPixels, vbTwips)
            ud.Top = 0
            ud.Height = Height
            cmb.Width = Width - ud.Width
        End If
    Else
        cmb.Width = Width
    End If
    Height = cmb.Height
End Sub

Public Property Get BackColor() As OLE_COLOR
Attribute BackColor.VB_Description = "Returns/sets the background color used to display text and graphics in an object."
    On Error Resume Next
    BackColor = cmb.BackColor
End Property

Public Property Let BackColor(ByVal NewValue As OLE_COLOR)
    On Error Resume Next
    cmb.BackColor = NewValue
    PropertyChanged "BackColor"
End Property

Public Property Get ForeColor() As OLE_COLOR
Attribute ForeColor.VB_Description = "Returns/sets the foreground color used to display text and graphics in an object."
    On Error Resume Next
    ForeColor = cmb.ForeColor
End Property

Public Property Let ForeColor(ByVal NewValue As OLE_COLOR)
    On Error Resume Next
    cmb.ForeColor = NewValue
    PropertyChanged "ForeColor"
End Property

Public Property Get Enabled() As Boolean
    On Error Resume Next
    Enabled = UserControl.Enabled
End Property

Public Property Let Enabled(ByVal NewValue As Boolean)
    On Error Resume Next
    UserControl.Enabled = NewValue
    cmb.Enabled = NewValue
    ud.Enabled = NewValue
    PropertyChanged "Enabled"
End Property

Public Property Get UpDownInside() As Boolean
    On Error Resume Next
    UpDownInside = m_bUpDownInside
End Property

Public Property Let UpDownInside(ByVal NewValue As Boolean)
    On Error Resume Next
    m_bUpDownInside = NewValue
    UserControl_Resize
    PropertyChanged "UpDownInside"
End Property

Public Property Get UpDownVisible() As Boolean
    On Error Resume Next
    UpDownVisible = ud.Visible
End Property

Public Property Let UpDownVisible(ByVal NewValue As Boolean)
    On Error Resume Next
    ud.Visible = NewValue
    UserControl_Resize
    PropertyChanged "UpDownVisible"
End Property

Public Property Get Font() As Font
Attribute Font.VB_Description = "Returns a Font object."
Attribute Font.VB_UserMemId = -512
    On Error Resume Next
    Set Font = UserControl.Font
End Property

Public Property Set Font(ByVal NewValue As Font)
    On Error Resume Next
    Set UserControl.Font = NewValue
    Set cmb.Font = NewValue
    UserControl_Resize
    PropertyChanged "Font"
End Property

Public Sub AddItem(ByVal Item As String, Optional ByVal Index As Variant)
Attribute AddItem.VB_Description = "Adds an item to a Listbox or ComboBox control or a row to a Grid control."
    cmb.AddItem Item, Index
    ud.Min = cmb.ListCount - 1
End Sub

Public Sub RemoveItem(ByVal Index As Integer)
    cmb.RemoveItem Index
    If cmb.ListCount > 0 Then
        ud.Min = cmb.ListCount - 1
    Else
        ud.Min = 0
    End If
End Sub

Public Sub Clear()
    cmb.Clear
    ud.Min = 0
End Sub

Private Sub cmb_Click()
    On Error Resume Next
    RaiseEvent Click
End Sub

Public Property Get ItemData(ByVal Index As Integer) As Long
Attribute ItemData.VB_Description = "Returns/sets a specific number for each item in a ComboBox or ListBox control."
    If Ambient.UserMode = False Then Err.Raise 387 ' N/A at design time
    ItemData = cmb.ItemData(Index)
End Property

Public Property Let ItemData(ByVal Index As Integer, ByVal NewValue As Long)
    If Ambient.UserMode = False Then Err.Raise 387 ' N/A at design time
    cmb.ItemData(Index) = NewValue
    PropertyChanged "ItemData"
End Property

Public Property Get ListIndex() As Integer
Attribute ListIndex.VB_Description = "Returns/sets the index of the currently selected item in the control."
    If Ambient.UserMode = False Then Err.Raise 387 ' N/A at design time
    ListIndex = cmb.ListIndex
End Property

Public Property Let ListIndex(ByVal NewValue As Integer)
    If Ambient.UserMode = False Then Err.Raise 387 ' N/A at design time
    cmb.ListIndex = NewValue
    PropertyChanged "ListIndex"
End Property

Public Property Get ListCount() As Integer
Attribute ListCount.VB_Description = "Returns the number of items in the list portion of a control."
    ListCount = cmb.ListCount
End Property

Public Property Get NewIndex() As Integer
Attribute NewIndex.VB_Description = "Returns the index of the item most recently added to a control."
    NewIndex = cmb.NewIndex
End Property

'Public Property Get Sorted() As Boolean
'    Sorted = cmb.Sorted
'End Property
'
'Public Property Let Sorted(ByVal NewValue As Boolean)
'    SetComboSorted NewValue
'    PropertyChanged "Sorted"
'End Property
'
'Private Sub SetComboSorted(ByVal NewValue As Boolean)
'    On Error Resume Next
'    Dim nStyle&, nNewStyle&
'    nStyle = GetWindowLong(cmb.hwnd, GWL_STYLE)
'
'    If NewValue Then
'        nNewStyle = nStyle Or CBS_SORT
'    Else
'        nNewStyle = nStyle And Not CBS_SORT
'    End If
'
'    If nStyle <> nNewStyle Then nStyle = SetWindowLong(cmb.hwnd, GWL_STYLE, nNewStyle)
'End Sub
