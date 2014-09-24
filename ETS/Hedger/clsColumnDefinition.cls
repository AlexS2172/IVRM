VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsColumnDefinition"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Public sName As String
Public sCaption As String
Public sCaption2 As String
Public MergeCol As Boolean
Public vsDataType As VSFlex7Ctl.DataTypeSettings
Public sFormat As String
Public bCanEdit As Boolean
Public bCanChangeOrder As Boolean
Public bVisible As Boolean
Public lIndex As Long

Public imgHelp As VB.Image

