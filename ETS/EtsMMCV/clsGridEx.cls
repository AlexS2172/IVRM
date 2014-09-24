VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsGridEx"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event AfterSort(ByVal Col As Long, Order As Integer)
Public Event BeforeSort(ByVal Col As Long, Order As Integer)

Private WithEvents m_fg As VSFlex7Ctl.VSFlexGrid
Attribute m_fg.VB_VarHelpID = -1

Private m_nSortOrder As Long
Private m_nSortCol As Long
Private m_strSortColKey As String
Private m_enMousePointer As MousePointerConstants

Private m_picSort(-1 To 1) As IPictureDisp

Public Sub Init(ByRef fg As VSFlex7Ctl.VSFlexGrid, _
                Optional ByRef picAsc As IPictureDisp = Nothing, _
                Optional ByRef picDesc As IPictureDisp = Nothing, _
                Optional ByRef picNoSort As IPictureDisp = Nothing)
    On Error Resume Next
    Set m_fg = fg
    Set m_picSort(-1) = IIf(picDesc Is Nothing, frmMain.imgSort(0).Picture, picDesc)
    Set m_picSort(0) = Nothing 'IIf(picNoSort Is Nothing, frmMain.imgSort(2).Picture, picNoSort)
    Set m_picSort(1) = IIf(picAsc Is Nothing, frmMain.imgSort(1).Picture, picAsc)
    
    m_nSortCol = -1
    m_nSortOrder = 0
    m_strSortColKey = ""
End Sub

Public Sub AdjustSortImages()
    On Error Resume Next
    
    If m_fg.Cols > 0 Then
        Set m_fg.Cell(flexcpPicture, 0, 0, 0, m_fg.Cols - 1) = m_picSort(0)
        
        If m_nSortCol >= 0 And m_nSortCol < m_fg.Cols Then
            If m_strSortColKey = m_fg.ColKey(m_nSortCol) Then
                Set m_fg.Cell(flexcpPicture, 0, m_nSortCol) = m_picSort(m_nSortOrder)
            Else
                If m_strSortColKey <> "" Then
                    m_nSortCol = m_fg.ColIndex(m_strSortColKey)
                    If m_nSortCol >= 0 Then
                        Set m_fg.Cell(flexcpPicture, 0, m_nSortCol) = m_picSort(m_nSortOrder)
                    Else
                        m_strSortColKey = ""
                    End If
                Else
                    m_strSortColKey = ""
                    m_nSortCol = -1
                End If
            End If
        End If
        m_fg.Cell(flexcpPictureAlignment, 0, 0, 0, m_fg.Cols - 1) = flexPicAlignRightCenter
    End If
End Sub

Public Sub ShowSortImage(ByVal nCol As Long, ByVal nOrder As Long)
    On Error Resume Next
    
    If m_nSortCol >= 0 And m_nSortCol < m_fg.Cols Then
        Set m_fg.Cell(flexcpPicture, 0, m_nSortCol) = m_picSort(0)
    End If
    
    m_nSortCol = -1
    m_strSortColKey = ""
    m_nSortOrder = 0
    
    If nCol >= 0 And nCol < m_fg.Cols Then
        m_nSortCol = nCol
        m_strSortColKey = m_fg.ColKey(nCol)
        m_nSortOrder = nOrder
        
        If m_nSortOrder < 0 Then
            m_nSortOrder = -1
        ElseIf m_nSortOrder > 0 Then
            m_nSortOrder = 1
        End If
    
        Set m_fg.Cell(flexcpPicture, 0, m_nSortCol) = m_picSort(m_nSortOrder)
    End If
End Sub

Private Sub Class_Initialize()
    On Error Resume Next
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_fg = Nothing
    Set m_picSort(0) = Nothing
    Set m_picSort(1) = Nothing
End Sub

Private Sub m_fg_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    m_enMousePointer = Screen.MousePointer
    Screen.MousePointer = vbHourglass
    RaiseEvent BeforeSort(Col, Order)
    If Order = flexSortNone Then
        Screen.MousePointer = vbDefault
    End If
End Sub

Public Sub FinishSorting(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim enOrder As VSFlex7Ctl.SortSettings
    
    RaiseEvent AfterSort(Col, Order)
    
    Set m_fg.Cell(flexcpPicture, 0, 0, 0, m_fg.Cols - 1) = m_picSort(0)
    
    enOrder = Order
    If enOrder = flexSortUseColSort Then enOrder = m_fg.ColSort(Col)
    
    Select Case enOrder
        Case flexSortGenericAscending, flexSortNumericAscending, flexSortStringNoCaseAscending, flexSortStringAscending
            m_nSortOrder = 1
        Case flexSortGenericDescending, flexSortNumericDescending, flexSortStringNoCaseDescending, flexSortStringDescending
            m_nSortOrder = -1
        Case Else
            m_nSortOrder = 0
    End Select
    
    m_nSortCol = Col
    m_strSortColKey = m_fg.ColKey(Col)

    If m_nSortCol >= 0 And m_nSortCol < m_fg.Cols Then
        Set m_fg.Cell(flexcpPicture, 0, m_nSortCol) = m_picSort(m_nSortOrder)
    End If
    Screen.MousePointer = m_enMousePointer
End Sub
Private Sub m_fg_AfterSort(ByVal Col As Long, Order As Integer)
    FinishSorting Col, Order
End Sub

Public Property Get SortColKey$()
    SortColKey = m_strSortColKey
End Property
