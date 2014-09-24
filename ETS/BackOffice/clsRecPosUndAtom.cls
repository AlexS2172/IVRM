VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsRecPosUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
' underlying atom for position reconciliation
Option Explicit

Public ID As Long
Public ContractType As Long
Public Symbol As String

Private Opt As New clsRecPosOptColl
Private rsOptions As ADODB.Recordset
Public Root As New clsRecOptRootColl
Private rsOptionRoots As ADODB.Recordset

Private Sub Class_Terminate()
    On Error Resume Next
    Set rsOptions = Nothing
    Set Opt = Nothing
    Set rsOptionRoots = Nothing
    Set Root = Nothing
End Sub

Private Sub LoadOptions()
    If ID <> 0 And rsOptions Is Nothing Then
        Set rsOptions = gDBW.usp_BoOptionsForReconcile_Get(ID)
    End If
End Sub


Public Function FindOption(ByVal sOptionRoot As String, ByVal bIsCall As Byte, ByVal dStrike As Double, ByVal dtExpiryDate As Date, _
        Optional OptId As Long = 0, Optional OptRootId As Long = 0, Optional sOPRACode As String = "") As clsRecPosOptAtom
    Dim aOpt As clsRecPosOptAtom, sKey$, dtDate As Date, sFilter$, nCount#
    Dim nOptId As Long
    
    
    sKey = OptionKey(sOptionRoot, bIsCall, dStrike, dtExpiryDate)
    
    Set aOpt = Opt(sKey)
        
    nCount = 0#
    If aOpt Is Nothing Then
        LoadOptions
        rsOptions.Filter = ""
                
        If rsOptions.RecordCount > 0 Then
        
            If OptId > 0 Then
                sFilter = "iContractID =" & Str$(OptId)
            Else
                sFilter = "fStrike =" & Str$(Round(dStrike, 4)) & _
                                " AND dtExpiry = " & dtExpiryDate & _
                                " AND bIsCall = " & IIf(bIsCall, 1, 0)
                If Len(sOptionRoot) > 0 Then
                    sFilter = sFilter & " AND vcOptionRootSymbol = '" & sOptionRoot & "'"
                End If
            End If
            
            rsOptions.MoveFirst
            rsOptions.Filter = sFilter
            
            If (rsOptions.RecordCount = 0 And Len(sOPRACode) > 0) Then
                sFilter = " fStrike = " & Str$(Round(dStrike, 4)) & _
                          " AND vcSymbol = '" & sOPRACode & "'" & _
                          " AND bIsCall = " & IIf(bIsCall, 1, 0)
                If Len(sOptionRoot) > 0 Then
                    sFilter = sFilter & " AND vcOptionRootSymbol = '" & sOptionRoot & "'"
                End If
                rsOptions.Filter = sFilter
            End If
            
            If rsOptions.RecordCount = 0 And OptRootId <> 0 Then
                nOptId = gDBW.usp_FindOption(OptRootId, IIf(bIsCall, 1, 0), dStrike, dtExpiryDate)
                If nOptId <> 0 Then
                    sFilter = "iContractID =" & Str$(nOptId)
                    rsOptions.Filter = sFilter
                End If
            End If
            
            If rsOptions.RecordCount = 1 Then
                Set aOpt = Opt.Add(sKey)
                    
                aOpt.ID = gCmn.ReadLng(rsOptions!iContractID)
                aOpt.Symbol = gCmn.ReadStr(rsOptions!vcSymbol)
                aOpt.Expiry = gCmn.ReadDate(rsOptions!dtExpiry)
                
                On Error Resume Next
                aOpt.SymbolDisplay = Left$(aOpt.Symbol, Len(aOpt.Symbol) - 2) & " " & UCase(Format$(aOpt.Expiry, "MMMYY")) & " " & Format$(Int(dStrike)) & " " & IIf(bIsCall, "C", "P")
                Err.Clear
                On Error GoTo 0
                
                aOpt.LotSize = gCmn.ReadLng(rsOptions!iLotSize)
                aOpt.IsCall = bIsCall
                aOpt.Strike = Round(dStrike, 4)
                aOpt.OptionRootID = gCmn.ReadLng(rsOptions!iOptionRootID)
                aOpt.OptionRootSymbol = gCmn.ReadStr(rsOptions!vcOptionRootSymbol)
                aOpt.ExportSymbol = gCmn.ReadStr(rsOptions!vcExportSymbol)
                Set FindOption = aOpt
            End If
        End If
    
    Else
        Set FindOption = aOpt
    End If
    
End Function

Public Sub InitRoot()
On Error Resume Next
    Dim aRoot As clsRecOptRootAtom
    Dim sOptionRoot As String
    Dim nOptRootID As Long
    '-----------------------------------------------------'
    LoadOptionRoots
    '-----------------------------------------------------'
    If rsOptionRoots.RecordCount > 0 Then
        rsOptionRoots.MoveFirst
        While Not rsOptionRoots.EOF
            sOptionRoot = gCmn.ReadStr(rsOptionRoots!vcSymbol)
            nOptRootID = gCmn.ReadLng(rsOptionRoots!iRootID)
                       
            If (Not Root Is Nothing) Then
                Set aRoot = Root.Add(sOptionRoot)
                
                If (Not aRoot Is Nothing) Then
                    aRoot.Symbol = sOptionRoot
                    aRoot.RootID = nOptRootID
                End If
            End If

            rsOptionRoots.MoveNext
        Wend
    End If
End Sub

Private Sub LoadOptionRoots()
    If ID <> 0 And rsOptionRoots Is Nothing Then
        Set rsOptionRoots = gDBW.usp_OptionRoot_Get(Null, ID)
    End If
End Sub

Public Function FindOptionsNoRoot(ByVal sUndSymbol As String, _
                    ByVal bIsCall As Byte, _
                    ByVal dStrike As Double, _
                    ByVal dtExpiryDate As Date, _
                    ByVal sImportId As String _
                    ) As clsRecPosOptAtom
    Dim aOpt As clsRecPosOptAtom, sKey$, dtDate As Date, sFilter$, nCount&, sOptionRoot$
    Dim collOpt As New clsRecPosOptColl
    Dim nOptRootID As Long
    
    nCount = 0&
    LoadOptionRoots
        
    If rsOptionRoots.RecordCount > 0 Then
        rsOptionRoots.MoveFirst
        While Not rsOptionRoots.EOF
            sOptionRoot = gCmn.ReadStr(rsOptionRoots!vcSymbol)
            nOptRootID = gCmn.ReadLng(rsOptionRoots!iRootID)
            Set aOpt = FindOption(sOptionRoot, bIsCall, dStrike, dtExpiryDate, , nOptRootID)
            
            If Not aOpt Is Nothing Then
                nCount = nCount + 1
                If sImportId = aOpt.ExportSymbol Then
                    Set FindOptionsNoRoot = aOpt
                    Exit Function
                ElseIf Not aOpt.IsSelectInTodayReconcile Then
                    collOpt.Add aOpt.ID, aOpt
                End If
                Set aOpt = Nothing
            End If
            rsOptionRoots.MoveNext
        Wend
    End If
    
    If nCount = 1 Then
        Set FindOptionsNoRoot = collOpt.Item(1)
        gDBW.usp_BoContractExportSymbol_Save collOpt.Item(1).ID, sImportId
    ElseIf nCount > 1 Then
        Set aOpt = frmOptionSelect.Execute(False, collOpt, sImportId)
        
        If Not aOpt Is Nothing Then
            gDBW.usp_BoContractExportSymbol_Save aOpt.ID, sImportId
            aOpt.ExportSymbol = sImportId
            aOpt.IsSelectInTodayReconcile = True
            Set FindOptionsNoRoot = aOpt
        End If
    End If
End Function

Private Static Function OptionKey(ByVal sOptionRoot, ByVal bIsCall As Byte, ByVal dStrike As Double, ByVal dtExpiryDate As Date) As String
    OptionKey = sOptionRoot & "_" & CStr(IIf(bIsCall, 1, 0)) & "_" & CStr(Round(dStrike, 4)) & "_" & Format(dtExpiryDate)
End Function


