Attribute VB_Name = "modHookEvents"
' **************************************************************
' Module            modHookEvents
'
' Filename          modHookEvents.bas
'
' Author            Oleg Sharkevich
'                   eGar Technology Inc.
'
' Description
'
' Form subclussing core functions module for ETS
'
' **************************************************************

Option Explicit

' max number of available window procedure address
Private Const MAX_WNDPROC = 10

Private Declare Function CallWindowProc Lib "user32" Alias "CallWindowProcA" (ByVal lpPrevWndFunc As Long, ByVal hWnd As Long, ByVal Msg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
Private Declare Function SetWindowLong Lib "user32" Alias "SetWindowLongA" (ByVal hWnd As Long, ByVal nIndex As Long, ByVal dwNewLong As Long) As Long
Private Const GWL_WNDPROC = -4

Private Type TWndInfo
    ' handle of subclassed window - zero if none
    hWnd As Long
    ' address of original window procedure
    wndProcAddr As Long
    ' address of custom window procedure
    localProcAddr As Long
    ' pointer to clsFormEvent object to be notified
    pFormEvents As Long
End Type

' data about forms being subclassed is kept in this structure
Private m_wndInfo(1 To MAX_WNDPROC) As TWndInfo

Private Sub InitLocalData()

    ' initialize the m_wndInfo() array
    On Error Resume Next
    
    m_wndInfo(1).localProcAddr = ProcAddr(AddressOf WndProc1)
    m_wndInfo(2).localProcAddr = ProcAddr(AddressOf WndProc2)
    m_wndInfo(3).localProcAddr = ProcAddr(AddressOf WndProc3)
    m_wndInfo(4).localProcAddr = ProcAddr(AddressOf WndProc4)
    m_wndInfo(5).localProcAddr = ProcAddr(AddressOf WndProc5)
    m_wndInfo(6).localProcAddr = ProcAddr(AddressOf WndProc6)
    m_wndInfo(7).localProcAddr = ProcAddr(AddressOf WndProc7)
    m_wndInfo(8).localProcAddr = ProcAddr(AddressOf WndProc8)
    m_wndInfo(9).localProcAddr = ProcAddr(AddressOf WndProc9)
    m_wndInfo(10).localProcAddr = ProcAddr(AddressOf WndProc10)

End Sub

Private Function ProcAddr(ByVal address As Long) As Long
    ' support routine for assigning the address
    ' of a routine to a variable
    ProcAddr = address
End Function

Public Function HookWindow(aFormEvents As clsFormEvents, ByVal hWnd As Long) As Integer
    
    '-------------------------------------------------
    ' start the subclassing of a window
    ' Return the index of the slot used
    '  or zero if error (no more available slots)
    '-------------------------------------------------
    
    Dim nIndex As Integer
    On Error Resume Next
    
    ' do we need to init the m_wndInfo() array?
    If m_wndInfo(1).localProcAddr = 0 Then InitLocalData
    
    ' search the first available slot
    nIndex = 1
    Do
        If m_wndInfo(nIndex).hWnd = 0 Then Exit Do
        nIndex = nIndex + 1
    Loop Until nIndex > MAX_WNDPROC
    
    ' return zero if no available slots
    If nIndex > MAX_WNDPROC Then
        HookWindow = 0
        Exit Function
    End If
    
    ' save data in local structure
    With m_wndInfo(nIndex)
        .pFormEvents = ObjPtr(aFormEvents)
        .hWnd = hWnd
        ' enforce new window procedure, get old address
        .wndProcAddr = SetWindowLong(hWnd, GWL_WNDPROC, .localProcAddr)
    End With
    
    ' return the index to signal that everything is OK
    HookWindow = nIndex
End Function

Public Sub UnhookWindow(Index As Integer)

    ' end the subclassing of a window, free the slot
    
    With m_wndInfo(Index)
        ' stop subclassing
        SetWindowLong .hWnd, GWL_WNDPROC, .wndProcAddr
    
        ' reset all fields , except localProcAddr
        .hWnd = 0
        .wndProcAddr = 0
        .pFormEvents = 0
    End With
                
End Sub

Private Function WndProc(ByVal ndx As Integer, ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    
    ' a generic window procedure, called by the
    ' many WndProc<n> procedures in the pool
    Dim bPreProcess As Boolean
    Dim nRetVal As Long
    Dim bCancel As Boolean
    Dim aFormEvents As clsFormEvents
    
    On Error Resume Next
    
    ' some defensive programming
    ' check that hWnd is correct for that slot
    If m_wndInfo(ndx).hWnd <> hWnd Then
        ' somehow, the object got destroyed without first calling UnhookMessages
        UnhookWindow ndx
        Exit Function
    End If
    
    ' see if the message is to be subclassed, and when
    Select Case uMsg
    Case WM_ACTIVATEAPP, WM_MOVE, WM_MOVING, WM_SIZING, WM_GETMINMAXINFO, WM_NCHITTEST, WM_SETCURSOR, WM_COMPACTING, WM_DISPLAYCHANGE
    Case WM_PAINT
        bPreProcess = True
    Case Else
        ' for all other msgs, call the original window proc and exit
        WndProc = CallWindowProc(m_wndInfo(ndx).wndProcAddr, hWnd, uMsg, wParam, lParam)
        Exit Function
    End Select
    
    ' build a reference to the connected object
    CopyMemory aFormEvents, m_wndInfo(ndx).pFormEvents, 4
    ' defensive programmig: check that the object
    ' still exists, and that the hWnds match
    If aFormEvents.hWnd <> hWnd Then
        ' this line is executed both if hWnd's don't match
        ' and if the class does not exist (On Error Resume Next)
        
        ' call the standard window procedure
        WndProc = CallWindowProc(m_wndInfo(ndx).wndProcAddr, hWnd, uMsg, wParam, lParam)
        ' but then stop subclassing
        UnhookWindow ndx
        GoTo EX
    End If
    
    If bPreProcess Then
        ' message pre-processing
        ' first, notify the event to the CFormEvents object
        WndProc = aFormEvents.BeforeMessage(hWnd, uMsg, wParam, lParam, bCancel)
        ' exit now if the object cancelled standard window proc processing
        If bCancel Then GoTo EX
    End If
            
    ' call the standard window procedure
    nRetVal = CallWindowProc(m_wndInfo(ndx).wndProcAddr, hWnd, uMsg, wParam, lParam)
    
    ' message post-processing
    ' notify the event to the CFormEvents object
    aFormEvents.AfterMessage hWnd, uMsg, wParam, lParam, nRetVal
    WndProc = nRetVal
    
EX:
    ' delete temporary object without Setting it to Nothing
    ' which would cause a GPF since ref counter is not correct
    CopyMemory aFormEvents, 0&, 4
    
End Function

Private Function WndProc1(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc1 = WndProc(1, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc2(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc2 = WndProc(2, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc3(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc3 = WndProc(3, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc4(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc4 = WndProc(4, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc5(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc5 = WndProc(5, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc6(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc6 = WndProc(6, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc7(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc7 = WndProc(7, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc8(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc8 = WndProc(8, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc9(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc9 = WndProc(9, hWnd, uMsg, wParam, lParam)
End Function

Private Function WndProc10(ByVal hWnd As Long, ByVal uMsg As Long, ByVal wParam As Long, ByVal lParam As Long) As Long
    WndProc10 = WndProc(10, hWnd, uMsg, wParam, lParam)
End Function



