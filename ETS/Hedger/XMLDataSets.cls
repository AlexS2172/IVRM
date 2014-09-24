VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "XMLDataSets"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Enum DataMembers
    SupportedStockSymbols = 1
    SymbolPrices
    IndexHV
    StockHV
    StockStockCorr
    StockIndexCorr
End Enum

Private Const THIS = "DataService"
Private Const SUBFOLDER = "\Data\"
Private Const EXTENTION = ".xml"

Private DataFolder As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    DataFolder = App.Path & SUBFOLDER
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetXMLFileName(ByVal DataMemberName As String) As String
    GetXMLFileName = DataFolder & DataMemberName & EXTENTION
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FileExist(ByVal FileName As String) As Boolean
    FileExist = Len(Dir(FileName)) > 0
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function OpenXMLFile(ByVal FileName As String) As ADODB.Recordset
    Set OpenXMLFile = New ADODB.Recordset
    OpenXMLFile.Open FileName
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveXMLFile(ByVal FileName As String, rs As ADODB.Recordset)
    If Len(Dir(FileName)) > 0 Then
        Kill FileName
    End If
    rs.Save FileName, adPersistXML
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetDataMemberName(ByVal DataMember As DataMembers) As String
    Select Case DataMember
    Case DataMembers.SupportedStockSymbols:     GetDataMemberName = "SupportedStockSymbols"
    Case DataMembers.SymbolPrices:              GetDataMemberName = "SymbolPrices"
    Case DataMembers.IndexHV:                   GetDataMemberName = "IndexHV"
    Case DataMembers.StockHV:                   GetDataMemberName = "StockHV"
    Case DataMembers.StockIndexCorr:            GetDataMemberName = "StockIndexCorr"
    Case DataMembers.StockStockCorr:            GetDataMemberName = "StockStockCorr"
    Case Else
        GetDataMemberName = ""
        Err.Raise vbObjectError, THIS, "Wrong DataMember identifier"
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetDataMemberFileName(ByVal DataMember As DataMembers) As String
    GetDataMemberFileName = GetXMLFileName(GetDataMemberName(DataMember))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function OpenDataMember(ByVal DataMember As DataMembers) As ADODB.Recordset
Dim FileName As String
    FileName = GetDataMemberFileName(DataMember)
    If FileExist(FileName) Then
        Set OpenDataMember = OpenXMLFile(FileName)
    Else
        ' No file
        Set OpenDataMember = NewDataMember(DataMember)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SaveDataMember(ByVal DataMember As DataMembers, rs As ADODB.Recordset)
    SaveXMLFile GetDataMemberFileName(DataMember), rs
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NewDataMember(ByVal DataMember As DataMembers) As ADODB.Recordset
    Select Case DataMember
    Case DataMembers.SupportedStockSymbols:     Set NewDataMember = NewSupportedStockSymbols
    Case DataMembers.SymbolPrices:              Set NewDataMember = NewSymbolPrices
    Case DataMembers.IndexHV:                   Set NewDataMember = NewIndexHV
    Case DataMembers.StockHV:                   Set NewDataMember = NewStockHV
    Case DataMembers.StockIndexCorr:            Set NewDataMember = NewStockIndexCorr
    Case DataMembers.StockStockCorr:            Set NewDataMember = NewStockStockCorr
    Case Else
        Set NewDataMember = Nothing
        Err.Raise vbObjectError, THIS, "Wrong DataMember identifier"
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewSupportedStockSymbols() As ADODB.Recordset
    Set NewSupportedStockSymbols = New ADODB.Recordset
    With NewSupportedStockSymbols
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "StockID", adInteger
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewSymbolPrices() As ADODB.Recordset
    Set NewSymbolPrices = New ADODB.Recordset
    With NewSymbolPrices
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "HV", adDouble
            .Append "ClosePrice", adDouble
            .Append "Selected", adBoolean
            .Append "Supported", adBoolean
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewIndexHV() As ADODB.Recordset
    Set NewIndexHV = New ADODB.Recordset
    With NewIndexHV
        With .Fields
            .Append "IndexSymbol", adVarChar, 20
            .Append "HV", adDouble
            .Append "ClosePrice", adDouble
            .Append "Selected", adBoolean
            .Append "Supported", adBoolean
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewStockHV() As ADODB.Recordset
    Set NewStockHV = New ADODB.Recordset
    With NewStockHV
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "HV", adDouble
            .Append "ClosePrice", adDouble
            .Append "Selected", adBoolean
            .Append "Supported", adBoolean
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewStockStockCorr() As ADODB.Recordset
    Set NewStockStockCorr = New ADODB.Recordset
    With NewStockStockCorr
        With .Fields
            .Append "StockSymbol1", adVarChar, 20
            .Append "StockSymbol2", adVarChar, 20
            .Append "Corr", adDouble
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function NewStockIndexCorr() As ADODB.Recordset
    Set NewStockIndexCorr = New ADODB.Recordset
    With NewStockIndexCorr
        With .Fields
            .Append "IndexSymbol", adVarChar, 20
            .Append "StockSymbol", adVarChar, 20
            .Append "Beta", adDouble
            .Append "Corr", adDouble
        End With
        .Open
    End With
End Function

