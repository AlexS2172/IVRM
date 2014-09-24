VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRpGrpAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public Enum ReportTypeEnum
    REPORT_NONE = -1
    REPORT_POSITION_GREEK_BY_MONTH = 0
    REPORT_POSITION_GREEK_SUMMARY
    REPORT_POSITION_WITH_EARLY_EXERCISE
    REPORT_POSITION_MATRIX_BY_STOCK
    REPORT_SYNTHETIC
    REPORT_PNL
    REPORT_RISK_MATRIX
    REPORT_EXERCISED_STOCK_POSITION
    REPORT_CLOSE_TO_SPOT
    REPORT_COUNT
    REPORT_LAST = REPORT_COUNT - 1
End Enum

Public ID As Long
Public GroupType As FilterTypeEnum
Public Report As ReportTypeEnum
    
Public Sub Clear()
    ID = 0
    GroupType = 0
    Report = REPORT_NONE
End Sub

Private Sub Class_Initialize()
    On Error Resume Next
    Clear
End Sub
