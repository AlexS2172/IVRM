VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsColumnDef"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Caption As String
Public Description As String
Public Format As String
Public DataType As VSFlex7Ctl.DataTypeSettings
Public CanEdit As Boolean
Public CanChangeOrder As Boolean
Public Visible As Boolean
Public CanChangeVisible As Boolean
Public Width As Long

Public BackColor As Long
Public ForeColor As Long

Public AllowNegative_DifCols As Boolean
Public NegativeBackColor As Long
Public NegativeForeColor As Long
Public AllowNegativeDifColsForCell As Boolean

Public DescriptionAlt1 As String
Public ForeColorAlt1 As Long

Public DescriptionAlt2 As String
Public ForeColorAlt2 As Long

Public Sub Init(ByVal aCaption As String, _
                ByVal aDescription As String, _
                ByVal aFormat As String, _
                ByVal aDataType As VSFlex7Ctl.DataTypeSettings, _
                ByVal aCanEdit As Boolean, _
                ByVal aCanChangeOrder As Boolean, _
                ByVal aVisible As Boolean, _
                ByVal aCanChangeVisible As Boolean, _
                Optional ByVal aDescriptionAlt1 As String = "", _
                Optional ByVal aDescriptionAlt2 As String = "", _
                Optional ByVal aSupportSimulation As Boolean = False)

    Caption = aCaption
    Description = aDescription
    Format = aFormat
    DataType = aDataType
    CanEdit = aCanEdit
    CanChangeOrder = aCanChangeOrder
    Visible = aVisible
    CanChangeVisible = aCanChangeVisible
    Width = -1

    DescriptionAlt1 = aDescriptionAlt1
    DescriptionAlt2 = aDescriptionAlt2
    ResetColors
End Sub
                
Public Sub CopyTo(ByRef aDest As clsColumnDef)
    On Error Resume Next
    aDest.Caption = Caption
    aDest.Description = Description
    aDest.Format = Format
    aDest.DataType = DataType
    aDest.CanEdit = CanEdit
    aDest.CanChangeOrder = CanChangeOrder
    aDest.Visible = Visible
    aDest.CanChangeVisible = CanChangeVisible
    aDest.BackColor = BackColor
    aDest.ForeColor = ForeColor
    aDest.Width = Width
    
    aDest.DescriptionAlt1 = DescriptionAlt1
    aDest.ForeColorAlt1 = ForeColorAlt1
    aDest.DescriptionAlt2 = DescriptionAlt2
    aDest.ForeColorAlt2 = ForeColorAlt2
End Sub

Public Sub WriteToStorage(sSection As String, aStorage As clsSettingsStorage)
    If CanChangeVisible Then
        aStorage.SetLongValue sSection, "Visible", Visible
    End If
    aStorage.SetLongValue sSection, "Width", Width

    aStorage.SetLongValue sSection, "BackColor", BackColor
    aStorage.SetLongValue sSection, "ForeColor", ForeColor
    aStorage.SetLongValue sSection, "ForeColorAlt1", ForeColorAlt1
    aStorage.SetLongValue sSection, "ForeColorAlt2", ForeColorAlt2
    aStorage.SetStringValue sSection, "Format", Format
End Sub

Public Sub ReadFromStorage(sSection As String, aStorage As clsSettingsStorage)
    If CanChangeVisible Then
        Visible = aStorage.GetLongValue(sSection, "Visible", Visible)
    End If
    Width = aStorage.GetLongValue(sSection, "Width", Width)
    If Width <= 0 Then Width = -1

    BackColor = aStorage.GetLongValue(sSection, "BackColor", BackColor)
    ForeColor = aStorage.GetLongValue(sSection, "ForeColor", ForeColor)
    ForeColorAlt1 = aStorage.GetLongValue(sSection, "ForeColorAlt1", ForeColorAlt1)
    ForeColorAlt2 = aStorage.GetLongValue(sSection, "ForeColorAlt2", ForeColorAlt2)
    
    Dim sFormat As String
    
    If DataType = flexDTCurrency Or DataType = flexDTDecimal Or DataType = flexDTDouble Or DataType = flexDTSingle Then
        sFormat = aStorage.GetStringValue(sSection, "Format", Format)
        If IsItDouble(sFormat) Then
            Format = sFormat
        End If
    ElseIf DataType = flexDTString Then
        Format = ""
    End If
End Sub

'Only positions for column with number type must be loaded from ini file
Private Function IsItDouble(ByVal sFormat As String) As Boolean
    Dim iPos As Integer
    IsItDouble = False
    iPos = InStr(sFormat, ".")
    If iPos > 0 Then
        IsItDouble = True
    Else
        iPos = InStr(sFormat, ",")
        If iPos > 0 Then
            IsItDouble = True
        Else
            iPos = InStr(sFormat, "#")
            If iPos > 0 Then
                IsItDouble = True
            End If
        End If
    End If
End Function

Public Function IsItNumberFormat() As Boolean
    Select Case DataType
        Case flexDTShort
            IsItNumberFormat = True
        Case flexDTLong
            IsItNumberFormat = True
        Case flexDTSingle
            IsItNumberFormat = True
        Case flexDTDouble
            IsItNumberFormat = True
        Case flexDTDecimal
            IsItNumberFormat = True
        Case flexDTLong8
            IsItNumberFormat = True
        Case Else
            IsItNumberFormat = False
    End Select
End Function

Private Sub Class_Initialize()
    ResetColors
End Sub

Private Sub ResetColors()
    BackColor = GetSysColor(COLOR_WINDOW)
    ForeColor = GetSysColor(COLOR_WINDOWTEXT)
    ForeColorAlt1 = ForeColor
    ForeColorAlt2 = ForeColor
End Sub


