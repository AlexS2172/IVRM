VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsClipMgr"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_xlApp As Excel.Application
Private m_xlBook As Excel.Workbook
Private m_xlSheet As Excel.Worksheet

Private m_bTryToConnectToExcel As Boolean

Private Sub Class_Initialize()
    On Error Resume Next
    m_bTryToConnectToExcel = True
End Sub

Private Sub InitExcel()
    On Error GoTo EH

    CleanUp

    Set m_xlApp = GetObject("", "Excel.Application")
    Set m_xlBook = m_xlApp.Workbooks.Add
    Set m_xlSheet = m_xlBook.Worksheets(1)

    Exit Sub
EH:
    CleanUp
End Sub

Private Sub CleanUp()
    On Error Resume Next
    Set m_xlSheet = Nothing
    If Not m_xlBook Is Nothing Then
        m_xlBook.Close False
        Set m_xlBook = Nothing
    End If
    Set m_xlApp = Nothing
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Clipboard.Clear
    CleanUp
End Sub

Public Sub CopyGridToClipboard(ByRef fg As VSFlexGrid)
    On Error Resume Next
    Dim bUseCommonClipboard As Boolean

    bUseCommonClipboard = True

    If m_bTryToConnectToExcel Then
        m_bTryToConnectToExcel = False
        InitExcel
    End If

    If Not m_xlApp Is Nothing Then bUseCommonClipboard = Not SaveGridToExcelClipboard(fg)

    If bUseCommonClipboard Then SaveGridToText fg
End Sub

Private Function SaveGridToExcelClipboard(ByRef fg As VSFlexGrid) As Boolean
    On Error GoTo EH
    Dim nRow&, nCol&, nStartRow&, nStartCol&, nEndRow&, nEndCol&, nWidth&, nHeight&, nX&, nY&
    Dim sCells() As Variant, xlRange As Excel.Range

    SaveGridToExcelClipboard = False

    With fg
        If .Rows > 0 And .Cols > 0 Then
            .GetSelection nStartRow, nStartCol, nEndRow, nEndCol
            If (nEndRow - nStartRow + 1) * (nEndCol - nStartCol + 1) <= 1 Then
                nStartRow = 1
                nEndRow = .Rows - 1
                nStartCol = 0
                nEndCol = .Cols - 1
            End If

            If .SelectionMode = flexSelectionListBox Then
                nStartCol = 0
                nEndCol = .Cols - 1
            End If

            nWidth = 0&
            For nCol = nStartCol To nEndCol
                If Not .ColHidden(nCol) Then nWidth = nWidth + 1
            Next

            nHeight = 1&

            For nRow = nStartRow To nEndRow
                If Not .RowHidden(nRow) Then nHeight = nHeight + 1
            Next

            ReDim sCells(1 To nHeight, 1 To nWidth)

            m_xlSheet.Range(Cells(1, 1), Cells(nHeight, nWidth)).Clear

            Set xlRange = m_xlSheet.Range(Cells(1, 1), Cells(1, nWidth))
            xlRange.NumberFormat = "@"
            xlRange.Font.Bold = True
            xlRange.HorizontalAlignment = xlCenter
            Set xlRange = Nothing

            nX = 1&
            For nCol = nStartCol To nEndCol
                If Not .ColHidden(nCol) Then
                    sCells(1, nX) = .Cell(flexcpTextDisplay, 0, nCol)
                    nX = nX + 1
                End If
            Next

            nX = 1&
            Dim sTmp$

            On Error Resume Next
            For nCol = nStartCol To nEndCol
                If Not .ColHidden(nCol) Then

                    nY = 2&
                    Set xlRange = m_xlSheet.Range(Cells(nY, nX), Cells(nHeight, nX))

                    Select Case .ColDataType(nCol)
                        Case flexDTBoolean, flexDTString, flexDTNull, flexDTEmpty
                            xlRange.NumberFormat = "@"

                        Case Else
                            xlRange.NumberFormat = .ColFormat(nCol)
                    End Select

                    If nCol < .FixedCols Then
                        xlRange.Font.Bold = True
                        xlRange.HorizontalAlignment = xlCenter
                    End If

                    Set xlRange = Nothing

                    For nRow = nStartRow To nEndRow
                        If Not .RowHidden(nRow) Then

                            Select Case .ColDataType(nCol)
                                Case flexDTDate
                                    sTmp = .TextMatrix(nRow, nCol)
                                    If sTmp <> "" And sTmp <> STR_NA Then
                                        Err.Clear
                                        sCells(nY, nX) = CDate(.TextMatrix(nRow, nCol))
                                        If Err.Number <> 0 Then sCells(nY, nX) = ""
                                    Else
                                        sCells(nY, nX) = sTmp
                                    End If

                                Case flexDTDouble, flexDTSingle, flexDTDecimal
                                    sTmp = .TextMatrix(nRow, nCol)
                                    If sTmp <> "" And sTmp <> STR_NA Then
                                        Err.Clear
                                        sCells(nY, nX) = CDbl(.TextMatrix(nRow, nCol))
                                        If Err.Number <> 0 Then sCells(nY, nX) = ""
                                    Else
                                        sCells(nY, nX) = sTmp
                                    End If

                                Case flexDTLong, flexDTShort
                                    sTmp = .TextMatrix(nRow, nCol)
                                    If sTmp <> "" And sTmp <> STR_NA Then
                                        Err.Clear
                                        sCells(nY, nX) = CLng(.TextMatrix(nRow, nCol))
                                        If Err.Number <> 0 Then sCells(nY, nX) = ""
                                    Else
                                        sCells(nY, nX) = sTmp
                                    End If

                                Case flexDTBoolean
                                    If Len(.ColFormat(nCol)) > 0 Then
                                        sCells(nY, nX) = .Cell(flexcpTextDisplay, nRow, nCol)
                                    Else
                                        If .Cell(flexcpChecked, nRow, nCol) = flexChecked Then
                                            sCells(nY, nX) = "Yes"
                                        ElseIf .Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
                                            sCells(nY, nX) = "No"
                                        End If
                                    End If

                                Case Else
                                    sCells(nY, nX) = .Cell(flexcpTextDisplay, nRow, nCol)

                            End Select

                            nY = nY + 1
                        End If
                    Next

                    nX = nX + 1
                End If
            Next

            On Error GoTo EH
            m_xlSheet.Range(Cells(1, 1), Cells(nHeight, nWidth)) = sCells

            Set xlRange = m_xlSheet.Range(Cells(1, 1), Cells(nHeight, nWidth))
            xlRange.Columns.AutoFit
            xlRange.Select
            xlRange.Copy
            Set xlRange = Nothing
        End If
    End With

    SaveGridToExcelClipboard = True
    Exit Function
EH:
    On Error Resume Next
    Set xlRange = Nothing
    Erase sCells
    CleanUp
    SaveGridToExcelClipboard = False
End Function

Private Sub SaveGridToText(ByRef fg As VSFlexGrid)
    On Error Resume Next
    Dim sBuffer$, nRow&, nCol&, nStartRow&, nStartCol&, nEndRow&, nEndCol&, sTmp$, bFirstCol As Boolean

    Clipboard.Clear
    sBuffer = ""

    With fg
        If .Rows > 1 And .Cols > 0 Then
            .GetSelection nStartRow, nStartCol, nEndRow, nEndCol
            If (nEndRow - nStartRow + 1) * (nEndCol - nStartCol + 1) <= 1 Then
                nStartRow = 1
                nEndRow = .Rows - 1
                nStartCol = 0
                nEndCol = .Cols - 1
            End If

            If .SelectionMode = flexSelectionListBox Then
                nStartCol = 0
                nEndCol = .Cols - 1
            End If

            sTmp = ""
            bFirstCol = True
            For nCol = nStartCol To nEndCol
                If Not .ColHidden(nCol) Then
                    If Not bFirstCol Then
                        sTmp = sTmp & vbTab
                    Else
                        bFirstCol = False
                    End If
                    sTmp = sTmp & .Cell(flexcpTextDisplay, 0, nCol)
                End If
            Next
            If Len(sTmp) > 0 Then
                sBuffer = sBuffer & sTmp & vbCrLf
            End If

            For nRow = nStartRow To nEndRow
                If Not .RowHidden(nRow) Then
                    sTmp = ""
                    bFirstCol = True
                    For nCol = nStartCol To nEndCol
                        If Not .ColHidden(nCol) Then
                            If Not bFirstCol Then
                                sTmp = sTmp & vbTab
                            Else
                                bFirstCol = False
                            End If

                            Select Case .ColDataType(nCol)
                                Case flexDTDate
                                    sTmp = sTmp & .TextMatrix(nRow, nCol)

                                Case flexDTBoolean
                                    If Len(.ColFormat(nCol)) > 0 Then
                                        sTmp = sTmp & .Cell(flexcpTextDisplay, nRow, nCol)
                                    Else
                                        If .Cell(flexcpChecked, nRow, nCol) = flexChecked Then
                                            sTmp = sTmp & "Yes"
                                        ElseIf .Cell(flexcpChecked, nRow, nCol) = flexUnchecked Then
                                            sTmp = sTmp & "No"
                                        End If
                                    End If

                                Case flexDTString, flexDTNull, flexDTEmpty
                                    sTmp = sTmp & .Cell(flexcpTextDisplay, nRow, nCol)

                                Case Else
                                    sTmp = sTmp & .TextMatrix(nRow, nCol) '.Cell(flexcpTextDisplay, nRow, nCol)
                            End Select
                        End If
                    Next

                    If Len(sTmp) > 0 Then
                        sBuffer = sBuffer & sTmp & vbCrLf
                    End If
                End If
            Next
        End If
    End With

   Clipboard.SetText sBuffer
End Sub

