VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsGridLock"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_fgGrid As VSFlex7Ctl.VSFlexGrid
Private m_nLockCount As Long

Public Sub Init(fgGrid As VSFlex7Ctl.VSFlexGrid)
    On Error Resume Next
    m_nLockCount = 0&
    Set m_fgGrid = fgGrid
End Sub

Public Sub LockRedraw()
    On Error Resume Next
    If m_fgGrid Is Nothing Then Exit Sub
    
    m_nLockCount = m_nLockCount + 1
    If m_nLockCount = 1 Then
        m_fgGrid.Redraw = flexRDNone
    End If
End Sub

Public Sub UnlockRedraw()
    On Error Resume Next
    If m_nLockCount = 0 Or m_fgGrid Is Nothing Then Exit Sub
    
    m_nLockCount = m_nLockCount - 1
    If m_nLockCount = 0 Then
        m_fgGrid.Redraw = flexRDBuffered
    End If
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_fgGrid = Nothing
End Sub


