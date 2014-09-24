VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsScreenExport"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const DECIMAL_PLACES As Long = 10

Public Enum FileSaveTypeEnum
    SE_REWRITE
    SE_ADDTOEND
End Enum


' Batch capability for projections
Private m_IsInit As Boolean

Private m_nParamsCount As Long
Private m_nSpotShiftCount As Long
Private m_nVolaShiftCount As Long

Private m_BatchBuffer() As String
Private m_BatchSpotValues() As Long
Private m_BatchVolaValues() As Long
Private m_BatchParamNames() As String

Private Const HTML_HEADER As String = _
"<html xmlns:o=""urn:schemas-microsoft-com:office:office""" & vbCrLf & "xmlns:x=""urn:schemas-microsoft-com:office:excel"">" & vbCrLf & "" & vbCrLf & "<head>" & vbCrLf & "" & vbCrLf & "<style>" & vbCrLf & "<!--table" & vbCrLf & "   {mso-displayed-decimal-separator:""\."";" & vbCrLf & "  mso-displayed-thousand-separator:""\,"";}" & vbCrLf & "@page" & vbCrLf & "  {margin:1.0in .75in 1.0in .75in;" & vbCrLf & "  mso-header-margin:.5in;" & vbCrLf & "   mso-footer-margin:.5in;}" & vbCrLf & "tr" & vbCrLf & "  {mso-height-source:auto;}" & vbCrLf & "col" & vbCrLf & "    {mso-width-source:auto;}" & vbCrLf & "br" & vbCrLf & "  {mso-data-placement:same-cell;}" & vbCrLf & ".style0" & vbCrLf & "  {mso-number-format:General;" & vbCrLf & "   text-align:general;" & vbCrLf & "   vertical-align:bottom;" & vbCrLf & "    white-space:nowrap;" & vbCrLf & "   mso-rotate:0;" & vbCrLf & " mso-background-source:auto;" & vbCrLf & "   mso-pattern:auto;" & vbCrLf & " color:windowtext;" & vbCrLf & " font-size:10.0pt;" _
 & vbCrLf & "   font-weight:400;" & vbCrLf & "  font-style:normal;" & vbCrLf & "    text-decoration:none;" & vbCrLf & " font-family:Arial;" & vbCrLf & "    mso-generic-font-family:auto;" & vbCrLf & " mso-font-charset:0;" & vbCrLf & "   border:none;" & vbCrLf & "  mso-protection:locked visible;" & vbCrLf & "    mso-style-name:Normal;" & vbCrLf & "    mso-style-id:0;}" & vbCrLf & "td" & vbCrLf & "  {mso-style-parent:style0;" & vbCrLf & " padding-top:1px;" & vbCrLf & "  padding-right:1px;" & vbCrLf & "    padding-left:1px;" & vbCrLf & " mso-ignore:padding;" & vbCrLf & "   color:windowtext;" & vbCrLf & " font-size:10.0pt;" & vbCrLf & " font-weight:400;" & vbCrLf & "  font-style:normal;" & vbCrLf & "    text-decoration:none;" & vbCrLf & " font-family:Arial;" & vbCrLf & "    mso-generic-font-family:auto;" & vbCrLf & " mso-font-charset:0;" & vbCrLf & "   mso-number-format:General;" & vbCrLf & "    text-align:general;" & vbCrLf & "   vertical-align:bottom;" & vbCrLf & "    border:none;" _
 & vbCrLf & "   mso-background-source:auto;" & vbCrLf & "   mso-pattern:auto;" & vbCrLf & " mso-protection:locked visible;" & vbCrLf & "    white-space:nowrap;" & vbCrLf & "   mso-rotate:0;}" & vbCrLf & ".xl24" & vbCrLf & " {mso-style-parent:style0;" & vbCrLf & " mso-number-format:""dd/mm/yyyy hh:mm:ss"";}" & vbCrLf & ".xl25" & vbCrLf & "    {mso-style-parent:style0;" & vbCrLf & " font-weight:700;" & vbCrLf & "  font-family:Arial, sans-serif;" & vbCrLf & "    mso-font-charset:0;}" & vbCrLf & "-->" & vbCrLf & "</style>" & vbCrLf & "<!--[if gte mso 9]><xml>" & vbCrLf & " <x:ExcelWorkbook>" & vbCrLf & "  <x:ExcelWorksheets>" & vbCrLf & "   <x:ExcelWorksheet>" & vbCrLf & "    <x:Name>Sheet1</x:Name>" & vbCrLf & "    <x:WorksheetOptions>" & vbCrLf & "     <x:Print>" & vbCrLf & "      <x:ValidPrinterInfo/>" & vbCrLf & "      <x:HorizontalResolution>600</x:HorizontalResolution>" & vbCrLf & "      <x:VerticalResolution>600</x:VerticalResolution>" & vbCrLf & "     </x:Print>" & vbCrLf & "     <x:Selected/>" _
 & vbCrLf & "     <x:Panes>" & vbCrLf & "      <x:Pane>" & vbCrLf & "       <x:Number>3</x:Number>" & vbCrLf & "       <x:ActiveRow>6</x:ActiveRow>" & vbCrLf & "       <x:ActiveCol>1</x:ActiveCol>" & vbCrLf & "      </x:Pane>" & vbCrLf & "     </x:Panes>" & vbCrLf & "     <x:ProtectContents>False</x:ProtectContents>" & vbCrLf & "     <x:ProtectObjects>False</x:ProtectObjects>" & vbCrLf & "     <x:ProtectScenarios>False</x:ProtectScenarios>" & vbCrLf & "    </x:WorksheetOptions>" & vbCrLf & "   </x:ExcelWorksheet>" & vbCrLf & "   <x:ExcelWorksheet>" & vbCrLf & "    <x:Name>Sheet2</x:Name>" & vbCrLf & "    <x:WorksheetOptions>" & vbCrLf & "     <x:ProtectContents>False</x:ProtectContents>" & vbCrLf & "     <x:ProtectObjects>False</x:ProtectObjects>" & vbCrLf & "     <x:ProtectScenarios>False</x:ProtectScenarios>" & vbCrLf & "    </x:WorksheetOptions>" & vbCrLf & "   </x:ExcelWorksheet>" & vbCrLf & "   <x:ExcelWorksheet>" & vbCrLf & "    <x:Name>Sheet3</x:Name>" & vbCrLf & "    <x:WorksheetOptions>" _
 & vbCrLf & "     <x:ProtectContents>False</x:ProtectContents>" & vbCrLf & "     <x:ProtectObjects>False</x:ProtectObjects>" & vbCrLf & "     <x:ProtectScenarios>False</x:ProtectScenarios>" & vbCrLf & "    </x:WorksheetOptions>" & vbCrLf & "   </x:ExcelWorksheet>" & vbCrLf & "  </x:ExcelWorksheets>" & vbCrLf & "  <x:WindowHeight>12660</x:WindowHeight>" & vbCrLf & "  <x:WindowWidth>19020</x:WindowWidth>" & vbCrLf & "  <x:WindowTopX>120</x:WindowTopX>" & vbCrLf & "  <x:WindowTopY>135</x:WindowTopY>" & vbCrLf & "  <x:ProtectStructure>False</x:ProtectStructure>" & vbCrLf & "  <x:ProtectWindows>False</x:ProtectWindows>" & vbCrLf & " </x:ExcelWorkbook>" & vbCrLf & "</xml><![endif]-->" & vbCrLf & "</head>" & vbCrLf & "" & vbCrLf & "<body link=blue vlink=purple>" & vbCrLf & "" & vbCrLf & "<table>"
 
Private Const HTML_FOOTER As String = "</table>" & vbCrLf & "" & vbCrLf & "</body>" & vbCrLf & "" & vbCrLf & "</html>"


Private Sub Class_Initialize()
    On Error Resume Next
    ClearBatchBuffer
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    ClearBatchBuffer
End Sub

Public Function Save(ByVal sFileName As String, ByVal sFilePath As String, ByRef fg As VSFlexGrid, _
                Optional ByVal del As String = ",", _
                Optional ByVal bSaveAll As Boolean = True, _
                Optional ByVal sHeader As String = "", _
                Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND, _
                Optional ByVal bLastPart As Boolean = True) As Boolean
    On Error Resume Next
    Dim bFirstCol As Boolean: bFirstCol = True
    Dim nRow, nCol As Long
    Dim dTmp As Double
    Dim nTmp As Long
    Dim dtTmp As Date
    
    
    Dim sTempPath As String
    sTempPath = g_Params.CurrentUserPrivateFolder
    sTempPath = sTempPath & IIf(Mid(sTempPath, Len(sTempPath), 1) = "\", "", "\")
    
    
    Save = False
    
    If Not fg.Rows > 0 Or Not fg.Rows > 0 Then
        Exit Function
    End If
    
    Dim hFile As Long
    
    hFile = FreeFile
    If hFile = 0 Then
        Exit Function
    End If
    
    If FileSaveType = SE_REWRITE Then Kill sTempPath & sFileName
    Open sTempPath & sFileName For Binary Access Write As #hFile
    If FileSaveType = SE_ADDTOEND Then Seek #hFile, LOF(hFile) + 1
    
    If sHeader <> "" Then Put #hFile, , CStr(sHeader & vbCrLf)
    
    nRow = 0
    While nRow < fg.Rows And nRow > -1
'        If Not fg.RowHidden(nRow) Then
        
            For nCol = 0 To fg.Cols - 1
                If Not fg.ColHidden(nCol) Then
                
                    Select Case fg.ColDataType(nCol)
                        Case flexDTBoolean
'                            If fg.Cell(flexcpChecked, nRow, nCol) = flexChecked Then
'                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & "Yes")
'                            Else
'                                If fg.Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
'                                    Put #hFile, , CStr(IIf(bFirstCol, "", del) & "No")
'                                Else
                                    Put #hFile, , CStr(IIf(bFirstCol, "", del) & fg.Cell(flexcpTextDisplay, nRow, nCol))
'                                End If
'                            End If
                                
                        Case flexDTDouble, flexDTSingle, flexDTDecimal
                            Err.Clear
                            dTmp = CDbl(fg.TextMatrix(nRow, nCol))
                            If Err.Number <> 0 Then
                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & fg.Cell(flexcpTextDisplay, nRow, nCol))
                            Else
                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & Round(dTmp, DECIMAL_PLACES))
                            End If
                            
                        Case flexDTLong, flexDTShort
                            Err.Clear
                            nTmp = CLng(fg.TextMatrix(nRow, nCol))
                            If Err.Number <> 0 Then
                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & fg.Cell(flexcpTextDisplay, nRow, nCol))
                            Else
                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & nTmp)
                            End If
                            
                        Case flexDTDate
'                            Err.Clear
'                            dtTmp = CDate(fg.TextMatrix(nRow, nCol))
'                            If Err.Number <> 0 Then
                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & fg.Cell(flexcpTextDisplay, nRow, nCol))
'                            Else
'                                Put #hFile, , CStr(IIf(bFirstCol, "", del) & Format(dtTmp, "mm/dd/yyyy"))
'                            End If
        
                        Case Else
                            Put #hFile, , CStr(IIf(bFirstCol, "", del) & fg.Cell(flexcpTextDisplay, nRow, nCol))
                    End Select
                    
                    bFirstCol = False
                End If
            Next
                
            Put #hFile, , vbCrLf
'        End If
        
        bFirstCol = True
        
        If fg.IsSubtotal(nRow) And Not bSaveAll Then
            nRow = fg.GetNodeRow(nRow, flexNTNextSibling)
        Else
            nRow = nRow + 1
        End If
    Wend
    
    Close #hFile
        
    FileCopy sTempPath & sFileName, sFilePath & sFileName
    If bLastPart Then Kill sTempPath & sFileName
    
    Save = True
    Exit Function
EH:
    gCmn.ErrorMsgBox Nothing, "Fail to save information in the file """ & sFilePath & sFileName & """"
End Function

Public Function InitBatchBuffer(ByVal nParamsCount As Long, _
                                ByVal nSpotShiftCount As Long, _
                                ByVal nVolaShiftCount As Long) As Boolean
    On Error Resume Next
    InitBatchBuffer = False
    
    If nParamsCount <= 0 Or nSpotShiftCount <= 0 Or nVolaShiftCount <= 0 Then Exit Function
    
    ClearBatchBuffer
    
    ReDim m_BatchBuffer(0 To (nParamsCount - 1), 0 To (nSpotShiftCount - 1), 0 To (nVolaShiftCount - 1))
    ReDim m_BatchSpotValues(0 To (nSpotShiftCount - 1))
    ReDim m_BatchVolaValues(0 To (nVolaShiftCount - 1))
    ReDim m_BatchParamNames(0 To (nParamsCount - 1))
    
    m_nParamsCount = nParamsCount
    m_nSpotShiftCount = nSpotShiftCount
    m_nVolaShiftCount = nVolaShiftCount
    
    m_IsInit = True
    
    InitBatchBuffer = True
End Function

Public Function SaveInBatchBuffer(ByRef fg As VSFlexGrid, _
                                  ByVal nSpotShiftIdx As Long, ByVal nSpotShiftValue As Long, _
                                  ByVal nVolaShiftIdx As Long, ByVal nVolaShiftValue As Long) As Boolean
    On Error Resume Next
    Dim nRow As Long: nRow = 1
    Dim nCol As Long
    Dim dTmp As Double
    Dim nTmp As Long
    
    SaveInBatchBuffer = False
    
    If fg.Rows <> 2 Or fg.Cols <> m_nParamsCount Or _
       nSpotShiftIdx < 0 Or nSpotShiftIdx > (m_nSpotShiftCount - 1) Or _
       nVolaShiftIdx < 0 Or nVolaShiftIdx > (m_nVolaShiftCount - 1) Or Not m_IsInit Then Exit Function
    
    m_BatchSpotValues(nSpotShiftIdx) = nSpotShiftValue
    m_BatchVolaValues(nVolaShiftIdx) = nVolaShiftValue
    
    For nCol = 0 To m_nParamsCount
        Select Case fg.ColDataType(nCol)
            Case flexDTBoolean
                If Len(fg.ColFormat(nCol)) > 0 Then
                    m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(fg.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    If fg.Cell(flexcpChecked, nRow, nCol) = flexChecked Then
                        m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = "Yes"
                    End If
                    If fg.Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
                        m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = "No"
                    End If
                End If
                        
            Case flexDTDouble, flexDTSingle, flexDTDecimal
                Err.Clear
                dTmp = CDbl(fg.TextMatrix(nRow, nCol))
                If Err.Number <> 0 Then
                    m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(fg.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(Round(dTmp, DECIMAL_PLACES))
                End If
                        
            Case flexDTLong, flexDTShort
                Err.Clear
                nTmp = CLng(fg.TextMatrix(nRow, nCol))
                If Err.Number <> 0 Then
                    m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(fg.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(nTmp)
                End If

            Case Else
                m_BatchBuffer(nCol, nSpotShiftIdx, nVolaShiftIdx) = CStr(fg.Cell(flexcpTextDisplay, nRow, nCol))
        End Select
        
        m_BatchParamNames(nCol) = CStr(fg.Cell(flexcpTextDisplay, 0, nCol))
    Next
    
    SaveInBatchBuffer = True
End Function

Public Function DropBatchBufferToFile(ByVal sFileName As String, ByVal sFilePath As String, _
                                      Optional ByVal del As String = ",", _
                                      Optional ByVal sDescription As String = "", _
                                      Optional ByVal sGroupDesc As String = "", _
                                      Optional ByVal bRewrite As Boolean = False) As Boolean
    On Error Resume Next
    Dim nParamIdx, nSpotIdx, nVolaIdx As Long
    Dim bLastCol As Boolean
    
    DropBatchBufferToFile = False
    
    Dim hFile As Long: hFile = FreeFile
    If hFile = 0 Then
        Exit Function
    End If
    
    If bRewrite Then Kill sFilePath & sFileName
    Open sFilePath & sFileName For Binary Access Write As #hFile
    Seek #hFile, LOF(hFile) + 1
    
    Put #hFile, , CStr(sGroupDesc & vbCrLf)
    
    For nParamIdx = 0 To (m_nParamsCount - 1)
        Put #hFile, , CStr("Parameter" & del & m_BatchParamNames(nParamIdx) & del & sDescription & vbCrLf)
    
        For nSpotIdx = -1 To (m_nSpotShiftCount - 1)
            For nVolaIdx = -1 To (m_nVolaShiftCount - 1)
                bLastCol = CBool(nVolaIdx = m_nVolaShiftCount - 1)
                
                If nSpotIdx = -1 And nVolaIdx = -1 Then
                    ' empty cell
                    Put #hFile, , del
                Else
                    If nSpotIdx = -1 Then
                        ' vola value
                        Put #hFile, , CStr(m_BatchVolaValues(nVolaIdx) & IIf(bLastCol, "", del))
                    End If
                    
                    If nVolaIdx = -1 Then
                        ' spot value
                        Put #hFile, , CStr(m_BatchSpotValues(nSpotIdx) & del)
                    End If
                    
                    If nVolaIdx <> -1 And nSpotIdx <> -1 Then
                        ' param value
                        Put #hFile, , CStr(m_BatchBuffer(nParamIdx, nSpotIdx, nVolaIdx) & IIf(bLastCol, "", del))
                    End If
                End If
                
            Next
            
            Put #hFile, , vbCrLf
        Next
    Next
    
    Close #hFile
    ClearBatchBuffer
    
    DropBatchBufferToFile = True
End Function

Private Sub ClearBatchBuffer()
    On Error Resume Next
    m_IsInit = False
    Erase m_BatchBuffer
    Erase m_BatchSpotValues
    Erase m_BatchVolaValues
    Erase m_BatchParamNames
    
    m_nParamsCount = -1
    m_nSpotShiftCount = -1
    m_nVolaShiftCount = -1
End Sub

Public Function GenerateCSS(ByRef fgData As VSFlexGrid) As String
On Error GoTo ErrHandler:

    Dim nCol As Long: nCol = 1
    Dim sStyleCSS As String: Dim sName As String
    Dim sKov As String: sKov = """"
    
    
    If (Not fgData Is Nothing) Then
        sName = fgData.Name
        
        'table css class
        sStyleCSS = sStyleCSS & "table." & sName & "{"
        sStyleCSS = sStyleCSS & "font-family: """ & fgData.FontName & """;"
        sStyleCSS = sStyleCSS & "font-size: " & CStr(fgData.FontSize) & "px;"
        sStyleCSS = sStyleCSS & "border-style: solid; border-width: 1px; border-color: RGB(200, 200, 200); BORDER-COLLAPSE: collapse;"
        If (fgData.FontBold) Then
            sStyleCSS = sStyleCSS & "font-weight: bolder;"
        End If
        If (fgData.FontItalic) Then
            sStyleCSS = sStyleCSS & "font-style: italic;"
        End If
        sStyleCSS = sStyleCSS & "}" & vbCrLf
        
        'cols headers css class
        sStyleCSS = sStyleCSS & "th.table_header{ background-color: RGB(125, 125, 125); border-color: RGB(200, 200, 200);}" & vbCrLf
        
        'cols css class
        If (fgData.Cols > 0) Then
            For nCol = 0 To fgData.Cols - 1
                If (Not fgData.ColHidden(nCol)) Then
                    'make value col header
                    sStyleCSS = sStyleCSS & "td." & sName & "_Val_Col" & CStr(nCol) & "{"
                    If (fgData.FixedCols > nCol) Then
                        sStyleCSS = sStyleCSS & " font-weight: bolder;"
                    End If
                    sStyleCSS = sStyleCSS & "color: #" & CStr(IIf(Hex(fgData.Cell(flexcpBackColor, 1, nCol)) = "FFFFFF", 0, Hex(fgData.Cell(flexcpBackColor, 1, nCol)))) & ";"
                    'sStyleCSS = sStyleCSS & "background-color: #" & CStr(Hex(fgData.Cell(flexcpForeColor, 1, nCol))) & ";"
                    sStyleCSS = sStyleCSS & " border-color: RGB(200, 200, 200);" & vbCrLf
                    sStyleCSS = sStyleCSS & "}" & vbCrLf
                End If
            Next
        End If
        
        sStyleCSS = sStyleCSS & "tr.bold_line {font-weight: bolder; background-color: RGB(175, 175, 175);}" & vbCrLf
        
    End If
    
    GenerateCSS = sStyleCSS
    
    Exit Function
ErrHandler:
    Debug.Print "Wrong fgData value."
End Function

Public Function SaveToCSV(ByVal sFileName As String, ByVal sFilePath As String, _
                            ByRef fgVal As VSFlexGrid, _
                            Optional ByRef fgTot As VSFlexGrid = Null, _
                            Optional ByRef fgFlt As VSFlexGrid = Null, _
                            Optional ByVal bSaveAll As Boolean = True) As Boolean
    On Error Resume Next
    Dim hFile As Long
    Dim sReport As String

    SaveToCSV = False
    
    hFile = FreeFile
    If hFile = 0 Then
        Exit Function
    End If
    
    Kill sFilePath & sFileName
    Open sFilePath & sFileName For Binary Access Write As #hFile
    
    If (Not fgFlt Is Nothing) Then
         GridToCSV fgFlt, bSaveAll, hFile
    End If
    If (Not fgTot Is Nothing) Then
        GridToCSV fgTot, bSaveAll, hFile
    End If
    If (Not fgVal Is Nothing) Then
        GridToCSV fgVal, bSaveAll, hFile
    End If
    
    Close #hFile

    SaveToCSV = True
End Function

Private Function GridToCSV(ByRef fgData As VSFlexGrid, ByVal bSaveAll As Boolean, ByVal hFile As Long) As String
On Error Resume Next

    Dim bFirstRow As Boolean
    Dim bFirstCol As Boolean
    Dim bLastCol As Boolean
    Dim nRow, nCol As Long
    Dim sReport As String
    Dim sName As String
    Dim sDelim As String: sDelim = ","
    
        
    If Not fgData.Rows > 0 Or Not fgData.Cols > 0 Then
        Exit Function
    End If
    
    'Generate header of table
    For nCol = 0 To fgData.Cols - 1
        If (Not fgData.ColHidden(nCol)) Then
            sReport = sReport & fgData.Cell(flexcpTextDisplay, 0, nCol) & IIf(nCol <> fgData.Cols - 1, sDelim, "")
        End If
    Next
    sReport = sReport & vbCrLf
    
    Put #hFile, , sReport
    
    'Generate value table
    For nRow = 1 To fgData.Rows - 1
        sReport = ""
        If ((fgData.IsSubtotal(nRow) And fgData.RowOutlineLevel(nRow) = 1) Or bSaveAll Or fgData.Rows <= 2) Then
            For nCol = 0 To fgData.Cols - 1
                If (Not fgData.ColHidden(nCol)) Then
                    sReport = sReport & GetCellTextValueCSV(fgData, nCol, nRow) & IIf(nCol <> fgData.Cols - 1, sDelim, "")
                End If
            Next
            sReport = sReport & vbCrLf
            Put #hFile, , sReport
        End If
    Next
    
    GridToCSV = sReport
    
End Function




Public Function SaveToHTML(ByVal sFileName As String, ByVal sFilePath As String, _
                            ByRef fgVal As VSFlexGrid, _
                            Optional ByRef fgTot As VSFlexGrid = Null, _
                            Optional ByRef fgFlt As VSFlexGrid = Null, _
                            Optional ByVal bSaveAll As Boolean = True) As Boolean
    On Error Resume Next
    Dim hFile As Long
    Dim sHtmlReport As String

    SaveToHTML = False
    
    hFile = FreeFile
    If hFile = 0 Then
        Exit Function
    End If
    
    Kill sFilePath & sFileName
    Open sFilePath & sFileName For Binary Access Write As #hFile
    
    sHtmlReport = "<html><head><title>Report</title><STYLE type=""text/css"">" & vbCrLf & "<!--" & vbCrLf
    If (Not fgFlt Is Nothing) Then
        sHtmlReport = sHtmlReport & GenerateCSS(fgFlt)
    End If
    If (Not fgTot Is Nothing) Then
        sHtmlReport = sHtmlReport & GenerateCSS(fgTot)
    End If
    If (Not fgVal Is Nothing) Then
        sHtmlReport = sHtmlReport & GenerateCSS(fgVal)
    End If
    sHtmlReport = sHtmlReport & "-->" & vbCrLf & "</STYLE></head>"
    
    Put #hFile, , sHtmlReport
        
    sHtmlReport = "<Body>"
    Put #hFile, , sHtmlReport
    
    If (Not fgFlt Is Nothing) Then
         GridToHTML fgFlt, bSaveAll, hFile
    End If
    If (Not fgTot Is Nothing) Then
        GridToHTML fgTot, bSaveAll, hFile
    End If
    If (Not fgVal Is Nothing) Then
        GridToHTML fgVal, bSaveAll, hFile
    End If
    sHtmlReport = "</Body></html>"
    Put #hFile, , sHtmlReport
  
    Close #hFile

    SaveToHTML = True

End Function

Private Function GetCellTextValue(ByRef fgData As VSFlexGrid, ByVal nCol As Long, ByVal nRow As Long) As String
On Error Resume Next
        Dim dTmp As Double
        Dim nTmp As Long
        ' select user friendly cell value
        Select Case fgData.ColDataType(nCol)
            Case flexDTBoolean
                If Len(fgData.ColFormat(nCol)) > 0 Then
                    GetCellTextValue = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    If fgData.Cell(flexcpChecked, nRow, nCol) = flexChecked Then
                        GetCellTextValue = "Yes"
                    End If
                    If fgData.Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
                        GetCellTextValue = "No"
                    End If
                End If
                
            Case Else
                GetCellTextValue = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
        End Select

End Function

Private Function GetCellTextValueCSV(ByRef fgData As VSFlexGrid, ByVal nCol As Long, ByVal nRow As Long) As String
On Error Resume Next
        Dim dTmp As Double
        Dim nTmp As Long
        Dim dtTmp As Date
        ' select user friendly cell value
        Select Case fgData.ColDataType(nCol)
            Case flexDTBoolean
                If Len(fgData.ColFormat(nCol)) > 0 Then
                    GetCellTextValueCSV = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    If fgData.Cell(flexcpChecked, nRow, nCol) = flexChecked Then
                        GetCellTextValueCSV = "Yes"
                    End If
                    If fgData.Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
                        GetCellTextValueCSV = "No"
                    End If
                End If
                        
            Case flexDTDouble, flexDTSingle, flexDTDecimal
                Err.Clear
                dTmp = CDbl(fgData.TextMatrix(nRow, nCol))
                If Err.Number <> 0 Then
                    GetCellTextValueCSV = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    GetCellTextValueCSV = CStr(Round(dTmp, DECIMAL_PLACES))
                End If
            Case flexDTDate
                Err.Clear
                dtTmp = CDate(fgData.TextMatrix(nRow, nCol))
                If Err.Number <> 0 Then
                    GetCellTextValueCSV = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    GetCellTextValueCSV = Format(dtTmp, "YYYY-DD-MMM HH:MM")
                End If
            Case flexDTShort, flexDTLong
                Err.Clear
                nTmp = CLng(fgData.TextMatrix(nRow, nCol))
                If Err.Number <> 0 Then
                    GetCellTextValueCSV = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
                Else
                    GetCellTextValueCSV = Trim(Str(nTmp))
                End If
            Case Else
                GetCellTextValueCSV = CStr(fgData.Cell(flexcpTextDisplay, nRow, nCol))
        End Select

End Function

Private Function GridToHTML(ByRef fgData As VSFlexGrid, ByVal bSaveAll As Boolean, ByVal hFile As Long) As String
On Error Resume Next

    Dim bFirstRow As Boolean
    Dim bFirstCol As Boolean
    Dim bLastCol As Boolean
    Dim nRow, nCol As Long
    Dim sHtmlReport As String
    Dim sName As String
    Dim sKov As String: sKov = """"
    Dim sBoldAgg As String
    Dim sTab As String
        
    If Not fgData.Rows > 0 Or Not fgData.Cols > 0 Then
        Exit Function
    End If
    
    sName = fgData.Name
    
    sHtmlReport = "<table class=" & sKov & sName & sKov
    sHtmlReport = sHtmlReport & " rules=" & sKov & "all" & sKov
    sHtmlReport = sHtmlReport & ">" & vbCrLf
    
    'Generate header
    For nCol = 0 To fgData.Cols - 1
        If (Not fgData.ColHidden(nCol)) Then
            sHtmlReport = sHtmlReport & "<th nowrap class=" & sKov & "table_header" & sKov & ">" & fgData.Cell(flexcpTextDisplay, 0, nCol) & "</th>" & vbCrLf
        End If
    Next
    
    Put #hFile, , sHtmlReport
    
    'Generate value table
    For nRow = 1 To fgData.Rows - 1
    
        If (fgData.RowOutlineLevel(nRow) = 1 And fgData.Rows > 2 And fgData.IsSubtotal(nRow)) Then
            sBoldAgg = " class=" & sKov & "bold_line" & sKov 'make underlying level bold and gray colored
        Else
            sBoldAgg = ""
        End If
               
        sHtmlReport = "<tr" & sBoldAgg & " >" & vbCrLf
        If ((fgData.IsSubtotal(nRow) And fgData.RowOutlineLevel(nRow) = 1) Or bSaveAll Or fgData.Rows <= 2) Then
            For nCol = 0 To fgData.Cols - 1
                If (Not fgData.ColHidden(nCol)) Then
                    sHtmlReport = sHtmlReport & "<td nowrap class=" & sKov & sName & "_Val_Col" & CStr(nCol) & sKov & ">"
                    sHtmlReport = sHtmlReport & vbTab & GetValidXMLText(GetCellTextValue(fgData, nCol, nRow))
                    sHtmlReport = sHtmlReport & "</td>" & vbCrLf
                End If
            Next
        End If
        sHtmlReport = sHtmlReport & "</tr>" & vbCrLf
        Put #hFile, , sHtmlReport
    Next
    sHtmlReport = "</table>"
    Put #hFile, , sHtmlReport
    
    GridToHTML = sHtmlReport
    
End Function

Private Function GetValidXMLText(sStr As String) As String
    On Error Resume Next
    Dim sOut As String
    
    sOut = sStr
    sOut = Replace(sOut, "&", "&amp;")
    sOut = Replace(sOut, "<", "&lt;")
    sOut = Replace(sOut, ">", "&gt;")
    sOut = Replace(sOut, "'", "&apos;")
    sOut = Replace(sOut, """", "&quot;")
    
    GetValidXMLText = sOut
End Function
