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
Public SupportSimulation As Boolean
Public UseInSimulation  As Boolean
Public ShowDeltaInSimulation As Boolean

Public BackColor As Long
Public ForeColor As Long

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
    SupportSimulation = aSupportSimulation
    UseInSimulation = False
    ShowDeltaInSimulation = False
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
    aDest.SupportSimulation = SupportSimulation
    aDest.UseInSimulation = UseInSimulation
    aDest.ShowDeltaInSimulation = ShowDeltaInSimulation
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
    aStorage.SetLongValue sSection, "SupportSimulation", IIf(SupportSimulation, 1, 0)
    aStorage.SetLongValue sSection, "UseInSimulation", IIf(UseInSimulation, 1, 0)
    aStorage.SetLongValue sSection, "ShowDeltaInSimulation", IIf(ShowDeltaInSimulation, 1, 0)
End Sub

Public Sub ReadFromStorage(sSection As String, aStorage As clsSettingsStorage)
    If CanChangeVisible Then
        Visible = aStorage.GetLongValue(sSection, "Visible", Visible)
    End If
    Width = aStorage.GetLongValue(sSection, "Width", Width)
    If Width <= 0 Then Width = -1

    Dim tmpBoolValue As Long
    
    BackColor = aStorage.GetLongValue(sSection, "BackColor", BackColor)
    ForeColor = aStorage.GetLongValue(sSection, "ForeColor", ForeColor)
    ForeColorAlt1 = aStorage.GetLongValue(sSection, "ForeColorAlt1", ForeColorAlt1)
    ForeColorAlt2 = aStorage.GetLongValue(sSection, "ForeColorAlt2", ForeColorAlt2)
    Format = aStorage.GetStringValue(sSection, "Format", Format)
    tmpBoolValue = aStorage.GetLongValue(sSection, "SupportSimulation", 0)
    'SupportSimulation = IIf(tmpBoolValue = 1, True, False)
    tmpBoolValue = aStorage.GetLongValue(sSection, "UseInSimulation", 0)
    UseInSimulation = IIf(tmpBoolValue = 1, True, False)
    tmpBoolValue = aStorage.GetLongValue(sSection, "ShowDeltaInSimulation", 0)
    ShowDeltaInSimulation = IIf(tmpBoolValue = 1, True, False)
End Sub

Private Sub Class_Initialize()
    ResetColors
End Sub

Private Sub ResetColors()
    BackColor = GetSysColor(COLOR_WINDOW)
    ForeColor = GetSysColor(COLOR_WINDOWTEXT)
    ForeColorAlt1 = ForeColor
    ForeColorAlt2 = ForeColor
End Sub


