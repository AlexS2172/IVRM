VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsGridDef"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public HorizLinesVisible As Boolean
Public VertLinesVisible As Boolean
Public GridLinesColor As Long
Public BackColorSel As Long
Public ForeColorSel As Long
Public Font As New StdFont
Public IsRowSelection As Boolean
Public IsGlobal As Boolean

Private m_Col() As clsColumnDef
Private m_Idx() As Long
Private m_nLastColumnIdx As Long
Private m_nFirstColumnIdxForStorage As Long

Public Sub Init(ByVal nFirstColumnIdxForStorage As Long, ByVal nLastColumnIdx As Long)
    On Error Resume Next
    Dim i&
    
    m_nLastColumnIdx = nLastColumnIdx
    m_nFirstColumnIdxForStorage = nFirstColumnIdxForStorage
    
    HorizLinesVisible = True
    VertLinesVisible = True
    GridLinesColor = &H8000000F
    BackColorSel = &H8000000D
    ForeColorSel = &H8000000E
    
    Font.Name = "MS Sans Serif"
    Font.Size = 8
    Font.Bold = False
    Font.Italic = False
    Font.Strikethrough = False
    Font.Underline = False
    
    IsRowSelection = False
    IsGlobal = False
    
    Erase m_Col
    Erase m_Idx
    
    ReDim m_Col(0 To m_nLastColumnIdx) As clsColumnDef
    ReDim m_Idx(0 To m_nLastColumnIdx) As Long
    
    For i = 0 To m_nLastColumnIdx
        Set m_Col(i) = New clsColumnDef
        m_Idx(i) = IDX_NONE
    Next
End Sub

Public Property Get Col(ByVal nIndex As Long) As clsColumnDef
    Set Col = m_Col(nIndex)
End Property

Public Property Get Idx(ByVal nIndex As Long) As Long
    Idx = m_Idx(nIndex)
End Property
Public Property Get IdxCopy() As Long()
    IdxCopy = m_Idx
End Property

Public Property Let Idx(ByVal nIndex As Long, ByVal nNewValue As Long)
    m_Idx(nIndex) = nNewValue
End Property

Public Sub CopyTo(ByRef aDest As clsGridDef)
    On Error Resume Next
    Dim i&
    
    aDest.Init m_nFirstColumnIdxForStorage, m_nLastColumnIdx
    
    aDest.HorizLinesVisible = HorizLinesVisible
    aDest.VertLinesVisible = VertLinesVisible
    aDest.GridLinesColor = GridLinesColor
    aDest.BackColorSel = BackColorSel
    aDest.ForeColorSel = ForeColorSel
    
    aDest.Font.Name = Font.Name
    aDest.Font.Size = Font.Size
    aDest.Font.Bold = Font.Bold
    aDest.Font.Italic = Font.Italic
    aDest.Font.Strikethrough = Font.Strikethrough
    aDest.Font.Underline = Font.Underline
    
    aDest.IsRowSelection = IsRowSelection
    aDest.IsGlobal = IsGlobal
    
    For i = 0 To m_nLastColumnIdx
        m_Col(i).CopyTo aDest.Col(i)
        aDest.Idx(i) = m_Idx(i)
    Next
End Sub

Public Sub WriteToStorage(sSection As String, aStorage As clsSettingsStorage, Optional ByVal bIncludeIdxInfo As Boolean = True)
    Dim i&
    aStorage.SetLongValue sSection, "HorizLinesVisible", HorizLinesVisible
    aStorage.SetLongValue sSection, "VertLinesVisible", VertLinesVisible
    aStorage.SetLongValue sSection, "GridLinesColor", GridLinesColor
    aStorage.SetLongValue sSection, "BackColorSel", BackColorSel
    aStorage.SetLongValue sSection, "ForeColorSel", ForeColorSel
    aStorage.SetStringValue sSection, "FontName", Font.Name
    aStorage.SetDoubleValue sSection, "FontSize", Font.Size
    aStorage.SetLongValue sSection, "FontBold", Font.Bold
    aStorage.SetLongValue sSection, "FontItalic", Font.Italic
    aStorage.SetLongValue sSection, "FontStrikethrough", Font.Strikethrough
    aStorage.SetLongValue sSection, "FontUnderline", Font.Underline

    If bIncludeIdxInfo Then
        For i = 0 To m_nLastColumnIdx
            If m_Idx(i) <> IDX_NONE Then
                aStorage.SetStringValue sSection & "Cols", "Column" & CStr(i), m_Col(m_Idx(i)).Caption
            Else
                Exit For
            End If
        Next
    End If
    
    For i = m_nFirstColumnIdxForStorage To m_nLastColumnIdx
        m_Col(i).WriteToStorage sSection & "Col" & m_Col(i).Caption, aStorage
    Next
End Sub

Public Sub ReadFromStorage(sSection As String, aStorage As clsSettingsStorage, Optional ByVal bIncludeIdxInfo As Boolean = True)
    Dim i&, nIdx&
    
    HorizLinesVisible = aStorage.GetLongValue(sSection, "HorizLinesVisible", HorizLinesVisible)
    VertLinesVisible = aStorage.GetLongValue(sSection, "VertLinesVisible", VertLinesVisible)
    GridLinesColor = aStorage.GetLongValue(sSection, "GridLinesColor", GridLinesColor)
    BackColorSel = aStorage.GetLongValue(sSection, "BackColorSel", BackColorSel)
    ForeColorSel = aStorage.GetLongValue(sSection, "ForeColorSel", ForeColorSel)
    Font.Name = aStorage.GetStringValue(sSection, "FontName", Font.Name)
    Font.Size = aStorage.GetDoubleValue(sSection, "FontSize", Font.Size)
    Font.Bold = aStorage.GetLongValue(sSection, "FontBold", Font.Bold)
    Font.Italic = aStorage.GetLongValue(sSection, "FontItalic", Font.Italic)
    Font.Strikethrough = aStorage.GetLongValue(sSection, "FontStrikethrough", Font.Strikethrough)
    Font.Underline = aStorage.GetLongValue(sSection, "FontUnderline", Font.Underline)
    
    Dim bOldSettings As Boolean
    Dim nCheckValue As Long
    nCheckValue = aStorage.GetLongValue(sSection & "Col" & CStr(m_nFirstColumnIdxForStorage), "BackColor", IDX_NONE)
    If nCheckValue <> IDX_NONE Then
        bOldSettings = True
    End If
    
    If bIncludeIdxInfo Then
        For i = 0 To m_nLastColumnIdx
            m_Idx(i) = IDX_NONE
        Next
    
        nIdx = 0
        For i = 0 To m_nLastColumnIdx
            If bOldSettings Then
                m_Idx(nIdx) = aStorage.GetLongValue(sSection & "Cols", "Column" & CStr(i), IDX_NONE)
            Else
                Dim sCaption As String
                sCaption = aStorage.GetStringValue(sSection & "Cols", "Column" & CStr(i), STR_NA)
                m_Idx(nIdx) = GetColumnIdxByCaption(sCaption)
            End If
            If m_Idx(nIdx) = IDX_NONE Then
                Exit For
            End If
            If m_Idx(nIdx) >= m_nFirstColumnIdxForStorage And m_Idx(nIdx) <= m_nLastColumnIdx Then nIdx = nIdx + 1
        Next
    End If
    
    For i = m_nFirstColumnIdxForStorage To m_nLastColumnIdx
        If bOldSettings Then
            m_Col(i).ReadFromStorage sSection & "Col" & CStr(i), aStorage
        Else
            m_Col(i).ReadFromStorage sSection & "Col" & m_Col(i).Caption, aStorage
        End If
    Next
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Erase m_Col
    Erase m_Idx
End Sub

Public Function GetColumnIdxByCaption(sCaption As String) As Long
    On Error Resume Next
    Dim i As Long
    For i = 0 To m_nLastColumnIdx
        If sCaption = m_Col(i).Caption Then
            GetColumnIdxByCaption = i
            Exit Function
        End If
    Next
    
    GetColumnIdxByCaption = IDX_NONE
End Function

Public Function GetVisibleCount() As Long
    On Error Resume Next
    Dim i As Long
    
    GetVisibleCount = 0
    
    For i = 0 To m_nLastColumnIdx
        If m_Idx(i) <> IDX_NONE Then
            GetVisibleCount = GetVisibleCount + 1
        Else
            Exit For
        End If
    Next
    
End Function
