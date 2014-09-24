VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRecPosFutAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
' underlying atom for position reconciliation
Option Explicit

Public Id As Long
Public Symbol As String
Public SymbolDisplay As String
Public Maturity As Date
Public IndexSymbol As String
Public ExportSymbol As String

Public IndexId As Long
Public FutureRootId As Long

Private FutOpt As New clsRecPosFutOptColl
Private rsFutOptions As ADODB.Recordset

Private Sub Class_Terminate()
    On Error Resume Next
    Set rsFutOptions = Nothing
    Set FutOpt = Nothing
End Sub

Private Sub LoadFutureOptions()
    If Id <> 0 And rsFutOptions Is Nothing Then
        Set rsFutOptions = gDBW.usp_BoFutureOption_Get(Id)
    End If
End Sub

Public Function FindFutureOption(ByVal bIsCall As Byte, ByVal dStrike As Double, ByVal dtExpiryDate As Date, _
        Optional OptId As Long = 0, _
        Optional ByVal sImportId As Variant = Null _
        ) As clsRecPosFutOptAtom
    Dim aOpt As clsRecPosFutOptAtom, sKey$, dtDate As Date, sFilter$, nCount#
    
    sKey = FutureOptionKey(bIsCall, dStrike, dtExpiryDate)
    
    Set aOpt = FutOpt(sKey)
    
    nCount = 0#
    If aOpt Is Nothing Then
        LoadFutureOptions
        rsFutOptions.Filter = ""
                
        If rsFutOptions.RecordCount > 0 Then
        
            If OptId > 0 Then
                sFilter = "iFutureOptionID =" & Str$(OptId)
            Else
                sFilter = "fStrike =" & Str$(Round(dStrike, 4)) & _
                            " AND dtExpiry = " & dtExpiryDate & _
                            " AND tiIsCall = " & IIf(bIsCall, 1, 0)
            End If
            
            rsFutOptions.MoveFirst
            rsFutOptions.Filter = sFilter
            
            If rsFutOptions.RecordCount = 1 Then
                Set aOpt = FutOpt.Add(sKey)
                    
                aOpt.Id = gCmn.ReadLng(rsFutOptions!iFutureOptionID)
                aOpt.Symbol = gCmn.ReadStr(rsFutOptions!vcFutureOptionSymbol)
                aOpt.Expiry = gCmn.ReadDate(rsFutOptions!dtExpiry)
                aOpt.IsCall = bIsCall
                aOpt.Strike = Round(dStrike, 4)
                
                If Not IsNull(sImportId) Then
                    aOpt.ExportSymbol = sImportId
                    gDBW.usp_BoContractExportSymbol_Save aOpt.Id, sImportId
                End If
                            
                On Error Resume Next
                aOpt.SymbolDisplay = Symbol & " " & UCase(Format$(aOpt.Expiry, "MMMYY")) & " " & Format$(Int(dStrike)) & " " & IIf(bIsCall, "C", "P")
                Err.Clear
                On Error GoTo 0

                Set FindFutureOption = aOpt
            End If
        End If
    
    Else
        Set FindFutureOption = aOpt
    End If
    
End Function

Private Static Function FutureOptionKey(ByVal bIsCall As Byte, ByVal dStrike As Double, ByVal dtExpiryDate As Date) As String
    FutureOptionKey = CStr(IIf(bIsCall, 1, 0)) & "_" & CStr(Round(dStrike, 4)) & "_" & Format(dtExpiryDate)
End Function


