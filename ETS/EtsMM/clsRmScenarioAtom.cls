VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRmScenarioAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private Const MAX_STEP_TIME As Long = 365&
Private Const MAX_STEP_SPOT As Double = 100#
Private Const MAX_STEP_VOLA As Double = 100#
Private Const MAX_STEP_ALL_PERC As Double = 100#

Public Enum RmAxisTypeEnum
    RMAT_FIRST = 0
    
    RMAT_VOLA = 0
    RMAT_SPOT = 1
    RMAT_TIME = 2

    RMAT_LAST = RMAT_TIME
End Enum

Public Enum RmUnitTypeEnum
    RMUT_ABS = 0
    RMUT_PERC = 1
End Enum

Public Enum RmAxisEnum
    RMA_HORZ = 0
    RMA_VERT = 1
End Enum

Public Enum RmVolaShiftTypeEnum
    RMVS_COMMON = 0
    RMVS_WEIGHTED = 1
End Enum

Public Enum RmVolaSpecificShiftTypeEnum
    RMVSS_NONE = 0
    RMVSS_GROUP_SPECIFIC = 1
End Enum

Public Enum RmSpotShiftTypeEnum
    RMSS_PARALLEL = 0
    RMSS_CORRELATED = 1
End Enum

Public Enum RmSpotSpecificShiftTypeEnum
    RMSSS_NONE = 0
    RMSSS_GROUP_SPECIFIC = 1
End Enum

Public ID As Long
Public ScenarioName As String
Public Description As String
Public Dirty As Boolean

Private m_Axis(RMA_HORZ To RMA_VERT) As RmAxisTypeEnum
Private m_Points(RMA_HORZ To RMA_VERT) As Long
Private m_Step(RMA_HORZ To RMA_VERT) As Double
Private m_Units(RMA_HORZ To RMA_VERT) As RmUnitTypeEnum

Private m_VolaShiftType As RmVolaShiftTypeEnum
Private m_SpotShiftType As RmSpotShiftTypeEnum

Private m_nCorrIndex As Long
Private m_enModel As EtsCalcModelTypeEnum
Private m_VolaSpecShiftType As RmVolaSpecificShiftTypeEnum
Private m_SpotSpecShiftType As RmSpotSpecificShiftTypeEnum
Private m_SpecificParams As String

Private m_Hour(RMA_HORZ To RMA_VERT) As Long
Private m_Minute(RMA_HORZ To RMA_VERT) As Long
Private m_ShowChange As Boolean

Public Property Get Axis(enType As RmAxisEnum) As RmAxisTypeEnum
    On Error Resume Next
    Axis = m_Axis(enType)
End Property

Public Property Let Axis(enType As RmAxisEnum, aValue As RmAxisTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_Axis(enType) <> aValue)
    m_Axis(enType) = aValue
End Property

Public Property Get AxisName(enType As RmAxisEnum) As String
    On Error Resume Next
    Select Case m_Axis(enType)
        Case RMAT_VOLA
            AxisName = "Vola"
        Case RMAT_SPOT
            AxisName = "Spot"
        Case RMAT_TIME
            AxisName = "Time"
    End Select
End Property

Public Property Get Points(enType As RmAxisEnum) As Long
    On Error Resume Next
    Points = m_Points(enType)
End Property

Public Property Let Points(enType As RmAxisEnum, aValue As Long)
    On Error Resume Next
    Dirty = Dirty Or (m_Points(enType) <> aValue)
    m_Points(enType) = aValue
End Property

Public Property Get Step(enType As RmAxisEnum) As Double
    On Error Resume Next
    Step = m_Step(enType)
End Property

Public Property Let Step(enType As RmAxisEnum, aValue As Double)
    On Error Resume Next
    Dirty = Dirty Or (m_Step(enType) <> aValue)
    m_Step(enType) = aValue
End Property

Public Property Get Hour(enType As RmAxisEnum) As Long
    On Error Resume Next
    Hour = m_Hour(enType)
End Property

Public Property Let Hour(enType As RmAxisEnum, aValue As Long)
    On Error Resume Next
    Dirty = Dirty Or (m_Hour(enType) <> aValue)
    m_Hour(enType) = aValue
End Property

Public Property Get Minute(enType As RmAxisEnum) As Long
    On Error Resume Next
    Minute = m_Minute(enType)
End Property

Public Property Let Minute(enType As RmAxisEnum, aValue As Long)
    On Error Resume Next
    Dirty = Dirty Or (m_Minute(enType) <> aValue)
    m_Minute(enType) = aValue
End Property

Public Property Get Units(enType As RmAxisEnum) As RmUnitTypeEnum
    On Error Resume Next
    Units = m_Units(enType)
End Property

Public Property Let Units(enType As RmAxisEnum, aValue As RmUnitTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_Units(enType) <> aValue)
    m_Units(enType) = aValue
End Property

Public Property Get VolaShiftType() As RmVolaShiftTypeEnum
    On Error Resume Next
    VolaShiftType = m_VolaShiftType
End Property

Public Property Let VolaShiftType(ByVal aValue As RmVolaShiftTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_VolaShiftType <> aValue)
    m_VolaShiftType = aValue
End Property

Public Property Get SpotShiftType() As RmSpotShiftTypeEnum
    On Error Resume Next
    SpotShiftType = m_SpotShiftType
End Property

Public Property Let SpotShiftType(ByVal aValue As RmSpotShiftTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_SpotShiftType <> aValue)
    m_SpotShiftType = aValue
End Property

Public Property Get SpotSpecificShiftType() As RmSpotSpecificShiftTypeEnum
    On Error Resume Next
    SpotSpecificShiftType = m_SpotSpecShiftType
End Property

Public Property Let SpotSpecificShiftType(ByVal aValue As RmSpotSpecificShiftTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_SpotSpecShiftType <> aValue)
    m_SpotSpecShiftType = aValue
End Property

Public Property Get VolaSpecificShiftType() As RmVolaSpecificShiftTypeEnum
    On Error Resume Next
    VolaSpecificShiftType = m_VolaSpecShiftType
End Property

Public Property Let VolaSpecificShiftType(ByVal aValue As RmVolaSpecificShiftTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_VolaSpecShiftType <> aValue)
    m_VolaSpecShiftType = aValue
End Property

Public Property Get CorrIndex() As Long
    On Error Resume Next
    CorrIndex = m_nCorrIndex
End Property

Public Property Let CorrIndex(ByVal aValue As Long)
    On Error Resume Next
    Dirty = Dirty Or (m_nCorrIndex <> aValue)
    m_nCorrIndex = aValue
End Property

Public Property Get CalcModel() As EtsCalcModelTypeEnum
    On Error Resume Next
    CalcModel = m_enModel
End Property

Public Property Let CalcModel(ByVal aValue As EtsCalcModelTypeEnum)
    On Error Resume Next
    Dirty = Dirty Or (m_enModel <> aValue)
    m_enModel = aValue
End Property

Public Property Get SpecificParams() As String
    On Error Resume Next
    SpecificParams = m_SpecificParams
End Property

Public Property Let SpecificParams(ByVal aValue As String)
    On Error Resume Next
    Dirty = Dirty Or (m_SpecificParams <> aValue)
    m_SpecificParams = aValue
End Property

Private Sub Class_Initialize()
    On Error Resume Next
    Clear
End Sub

Private Sub Class_Terminate()
On Error Resume Next
    Erase m_Axis
    Erase m_Points
    Erase m_Step
    Erase m_Units
    Erase m_Hour
    Erase m_Minute
End Sub

Public Sub FixValues()
    On Error Resume Next
    Dim i&
    
    For i = RMA_HORZ To RMA_VERT
        If m_Axis(i) < RMAT_FIRST Then
            Axis(i) = RMAT_FIRST
        ElseIf m_Axis(i) > RMAT_LAST Then
            Axis(i) = RMAT_LAST
        End If
        
        If m_Units(i) <> RMUT_ABS Then Units(i) = RMUT_PERC
        FixUnits
        FixPoints i
    Next
    
    FixAxis RMA_VERT
End Sub

Public Sub FixAxis(enType As RmAxisEnum)
    On Error Resume Next
    Dim enOtherType As RmAxisEnum
    
    enOtherType = 1 - enType
    
    If m_Axis(enType) = m_Axis(enOtherType) Then
        Select Case m_Axis(enOtherType)
            Case RMAT_VOLA
                Axis(enType) = RMAT_SPOT
            Case RMAT_SPOT
                Axis(enType) = RMAT_TIME
            Case RMAT_TIME
                Axis(enType) = RMAT_VOLA
        End Select
    End If
End Sub

Public Sub FixPoints(enType As RmAxisEnum)
    On Error Resume Next
    If m_Points(enType) < 1 Or m_Points(enType) > RM_MAX_POINTS Then
        Points(enType) = 5
    Else
        If (m_Points(enType) Mod 2) <> 1 Then
            Points(enType) = m_Points(enType) + 1
        End If
    End If
End Sub

Public Sub FixUnits(Optional ByVal bSingleUnd As Boolean = True)
    On Error Resume Next
    
    If m_Axis(RMA_HORZ) = RMAT_TIME Then
        Units(RMA_HORZ) = RMUT_ABS
    ElseIf Not bSingleUnd Then
        Units(RMA_HORZ) = RMUT_PERC
    End If

    If m_Axis(RMA_VERT) = RMAT_TIME Then
        Units(RMA_VERT) = RMUT_ABS
    ElseIf Not bSingleUnd Then
        Units(RMA_VERT) = RMUT_PERC
    End If
    
    FixSteps
End Sub

Public Sub FixSteps()
    On Error Resume Next
    If m_Step(RMA_HORZ) < 0 Then Step(RMA_HORZ) = 0
    If m_Units(RMA_HORZ) <> RMUT_PERC Then
        Select Case m_Axis(RMA_HORZ)
            Case RMAT_VOLA
                If m_Step(RMA_HORZ) > MAX_STEP_VOLA Then Step(RMA_HORZ) = MAX_STEP_VOLA
            Case RMAT_SPOT
                If m_Step(RMA_HORZ) > MAX_STEP_SPOT Then Step(RMA_HORZ) = MAX_STEP_SPOT
            Case RMAT_TIME
                If m_Step(RMA_HORZ) > MAX_STEP_TIME Then Step(RMA_HORZ) = MAX_STEP_TIME
                If m_Step(RMA_HORZ) <> Fix(m_Step(RMA_HORZ)) Then Step(RMA_HORZ) = Fix(m_Step(RMA_HORZ))
        End Select
    Else
        If m_Step(RMA_HORZ) > MAX_STEP_ALL_PERC Then Step(RMA_HORZ) = MAX_STEP_ALL_PERC
    End If

    If m_Step(RMA_VERT) < 0 Then Step(RMA_VERT) = 0
    If m_Units(RMA_VERT) <> RMUT_PERC Then
        Select Case m_Axis(RMA_VERT)
            Case RMAT_VOLA
                If m_Step(RMA_VERT) > MAX_STEP_VOLA Then Step(RMA_VERT) = MAX_STEP_VOLA
            Case RMAT_SPOT
                If m_Step(RMA_VERT) > MAX_STEP_SPOT Then Step(RMA_VERT) = MAX_STEP_SPOT
            Case RMAT_TIME
                If m_Step(RMA_VERT) > MAX_STEP_TIME Then Step(RMA_VERT) = MAX_STEP_TIME
                If m_Step(RMA_VERT) <> Fix(m_Step(RMA_VERT)) Then Step(RMA_VERT) = Fix(m_Step(RMA_VERT))
        End Select
    Else
        If m_Step(RMA_VERT) > MAX_STEP_ALL_PERC Then Step(RMA_VERT) = MAX_STEP_ALL_PERC
    End If
End Sub

Public Sub CopyTo(aDest As clsRmScenarioAtom)
    On Error Resume Next
    aDest.ID = ID
    aDest.ScenarioName = ScenarioName
    aDest.Description = Description
    
    aDest.Axis(RMA_HORZ) = m_Axis(RMA_HORZ)
    aDest.Points(RMA_HORZ) = m_Points(RMA_HORZ)
    aDest.Step(RMA_HORZ) = m_Step(RMA_HORZ)
    aDest.Hour(RMA_HORZ) = m_Hour(RMA_HORZ)
    aDest.Minute(RMA_HORZ) = m_Minute(RMA_HORZ)
    aDest.Units(RMA_HORZ) = m_Units(RMA_HORZ)
    
    aDest.Axis(RMA_VERT) = m_Axis(RMA_VERT)
    aDest.Points(RMA_VERT) = m_Points(RMA_VERT)
    aDest.Step(RMA_VERT) = m_Step(RMA_VERT)
    aDest.Hour(RMA_VERT) = m_Hour(RMA_VERT)
    aDest.Minute(RMA_VERT) = m_Minute(RMA_VERT)
    aDest.Units(RMA_VERT) = m_Units(RMA_VERT)
    aDest.ShowChange = m_ShowChange
    
    aDest.VolaShiftType = m_VolaShiftType
    aDest.SpotShiftType = m_SpotShiftType
    
    aDest.CalcModel = m_enModel
    aDest.CorrIndex = m_nCorrIndex
    
    aDest.SpecificParams = m_SpecificParams
    
    aDest.SpotSpecificShiftType = m_SpotSpecShiftType
    aDest.VolaSpecificShiftType = m_VolaSpecShiftType
    
    aDest.Dirty = Dirty
End Sub

Public Sub Clear()
    On Error Resume Next
    ID = 0
    ScenarioName = "Untitled"
    Description = ""
    Dirty = True
    
    m_Axis(RMA_HORZ) = RMAT_SPOT
    m_Points(RMA_HORZ) = 5
    m_Step(RMA_HORZ) = 1
    m_Hour(RMA_HORZ) = 0
    m_Minute(RMA_HORZ) = 0
    m_Units(RMA_HORZ) = RMUT_PERC
    
    m_Axis(RMA_VERT) = RMAT_VOLA
    m_Points(RMA_VERT) = 5
    m_Step(RMA_VERT) = 1
    m_Hour(RMA_VERT) = 0
    m_Minute(RMA_VERT) = 0
    m_Units(RMA_VERT) = RMUT_PERC
    
    m_VolaShiftType = RMVS_COMMON
    m_SpotShiftType = RMSS_PARALLEL
    
    m_enModel = g_Params.CalcModel
    m_nCorrIndex = 0
    
    m_SpotSpecShiftType = RMSSS_NONE
    m_VolaSpecShiftType = RMVSS_NONE
    
    m_ShowChange = False
End Sub

Public Property Get ShowChange() As Boolean
    On Error Resume Next
    ShowChange = m_ShowChange
End Property

Public Property Let ShowChange(aValue As Boolean)
    On Error Resume Next
    Dirty = Dirty Or (m_ShowChange <> aValue)
    m_ShowChange = aValue
End Property

