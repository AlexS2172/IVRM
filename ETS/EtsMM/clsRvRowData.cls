VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRvRowData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_Und As EtsMmRisksLib.MmRvUndAtom
Private m_Pos As EtsMmRisksLib.MmRvPosAtom
Private m_SynthGreeks As EtsMmRisksLib.MmRvSynthGreeksAtom

Public Property Get Und() As EtsMmRisksLib.MmRvUndAtom
    Set Und = m_Und
End Property

Public Property Set Und(aUnd As EtsMmRisksLib.MmRvUndAtom)
    Set m_Und = aUnd
End Property

Public Property Get Pos() As EtsMmRisksLib.MmRvPosAtom
    Set Pos = m_Pos
End Property

Public Property Set Pos(aPos As EtsMmRisksLib.MmRvPosAtom)
    Set m_Pos = aPos
End Property

Public Property Get SynthGreeks() As EtsMmRisksLib.MmRvSynthGreeksAtom
    Set SynthGreeks = m_SynthGreeks
End Property

Public Property Set SynthGreeks(aGreeks As EtsMmRisksLib.MmRvSynthGreeksAtom)
    Set m_SynthGreeks = aGreeks
End Property

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_Und = Nothing
    Set m_Pos = Nothing
    Set m_SynthGreeks = Nothing
End Sub

