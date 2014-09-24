Attribute VB_Name = "modCommon"
Option Explicit

Public Type POINTAPI
    X As Long
    Y As Long
End Type

Public Declare Function GetCursorPos& Lib "user32" (lpPoint As POINTAPI)
Public Declare Function WindowFromPoint& Lib "user32" (ByVal lpPointX As Long, ByVal lpPointY As Long)

Public Declare Function SetCapture Lib "user32" (ByVal hwnd As Long) As Long
Public Declare Function GetCapture Lib "user32" () As Long
Public Declare Function ReleaseCapture Lib "user32" () As Long
Public Declare Function GetFocus Lib "user32" () As Long



