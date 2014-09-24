Attribute VB_Name = "basGrids"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Grid with focus
'
Public g_grd As VSFlexGrid

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyFucusGridToClipboard() As Boolean
Dim Buffer As String
    On Error Resume Next
    If Not g_grd Is Nothing Then
        With Clipboard
            .Clear
            If SaveGridToText(g_grd, Buffer, False, True) Then
                .SetText Buffer
                CopyFucusGridToClipboard = True
            End If
        End With
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyGridToClipboard(grd As VSFlexGrid, ByVal All As Boolean) As Boolean
Dim Buffer As String
    With Clipboard
        .Clear
        If SaveGridToText(grd, Buffer, All) Then
            .SetText Buffer
            CopyGridToClipboard = True
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the clipboard content to the grid
'
Public Function CopyClipboardToGrid(grd As VSFlexGrid) As Boolean
Dim Buffer As String
    With Clipboard
        If .GetFormat(vbCFText) Then
            Buffer = .GetText
            CopyClipboardToGrid = LoadGridFromText(grd, Buffer)
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Parse text buffer and fill grid
'
Public Function LoadGridFromText(grd As VSFlexGrid, ByVal Buffer As String) As Boolean
Dim Lines As Variant
Dim Fields As Variant
Dim Line1 As Variant
Dim Field As Variant
Dim i As Long
Dim j As Long
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
    
    If Len(Buffer) > 0 Then
        Lines = Split(Buffer, vbCrLf)
        
        grd.GetSelection r1, c1, r2, c2
                
        If r1 < 2 Then
            Exit Function
            r1 = 2
            c1 = 0
        End If
        If c1 < 0 Then
            Exit Function
            c1 = 0
        End If
        i = r1
        'grd.Rows = i
        For Each Line1 In Lines
            If Len(Line1) > 0 Then
                Fields = Split(Line1, vbTab)
                i = i + 1
                If grd.Rows < i Then
                    ' Do not add new rows!
                    'grd.Rows = i
                Else
                    j = c1
                    For Each Field In Fields
Try:
                        j = j + 1
                        If j > grd.Cols Then
                            GoTo Skip
                        End If
                        If grd.ColHidden(j - 1) Then
                            GoTo Try
                        End If
                        grd.TextMatrix(i - 1, j - 1) = Field
Skip:
                    Next
                End If
            End If
        Next
        grd.Select r1, c1, r2, c2
        LoadGridFromText = True
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Save grid content to the text buffer
'
Public Function SaveGridToText(grd As VSFlexGrid, Buffer As String, ByVal All As Boolean, Optional ByVal AllowOneCell As Boolean = True) As Boolean
Dim i As Long
Dim j As Long
Dim k As Long
Dim S As String
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
Dim CopyHiddenRows As Boolean
    CopyHiddenRows = False
    Buffer = ""
    With grd
        If .Rows > 2 And .Cols > 0 Then
            If All Then
                r1 = 2
                r2 = .Rows - 1
                c1 = 0
                c2 = .Cols - 1
            Else
                .GetSelection r1, c1, r2, c2
                If r1 < 2 Then
                    SaveGridToText = False
                    Exit Function
                End If
            End If
            For i = r1 To r2
                If CopyHiddenRows Or Not .RowHidden(i) Then
                    S = ""
                    k = 0
                    For j = c1 To c2
                        If Not .ColHidden(j) Then
                            If k > 0 Then
                                S = S & vbTab
                            End If
                            k = k + 1
                            If .Cell(flexcpChecked, i, j) = flexNoCheckbox Then
                                S = S & .TextMatrix(i, j)
                            Else
                                If .Cell(flexcpChecked, i, j) = flexChecked Then
                                    S = S & "1"
                                ElseIf .Cell(flexcpChecked, i, j) = flexUnchecked Then
                                    S = S & "0"
                                Else
                                    S = S & ""
                                End If
                            End If
                        End If
                    Next
                    If Len(S) > 0 Then
                        Buffer = Buffer & S & vbCrLf
                    End If
                End If
            Next
        End If
    End With
    SaveGridToText = Len(Buffer) > 0
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function MultiSelection(grd As VSFlexGrid) As Boolean
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
    With grd
        If .Rows > 1 And .Cols > 0 Then
            .GetSelection r1, c1, r2, c2
            MultiSelection = (r2 - r1 + 1) * (c2 - c1 + 1) > 1
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintGrid(grd As VSFlexGrid, ByVal Caption As String)
Dim frm As frmPrintPreview
    On Error GoTo Herr
    Set frm = New frmPrintPreview
    Load frm
    frm.WindowState = vbMaximized
    frm.PrintGrid grd, Caption
    frm.Show vbModal, frmMain
    Unload frm
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ColorRowsEx(grd As VSFlexGrid, Optional ByVal RowsPalette As enmRowsPalette = RowsPaletteGray)
Dim i As Long
Dim c0 As Long
Dim c1 As Long
    On Error Resume Next
    Select Case RowsPalette
    Case RowsPaletteGray
        c0 = GCOLOR_TABLE_ODD_ROW_GRAY
        c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
    Case RowsPaletteRed
        c0 = GCOLOR_TABLE_ODD_ROW_RED
        c1 = GCOLOR_TABLE_EVEN_ROW_RED
    Case RowsPaletteGreen
        c0 = GCOLOR_TABLE_ODD_ROW_GREEN
        c1 = GCOLOR_TABLE_EVEN_ROW_GREEN
    Case RowsPaletteBlue
        c0 = GCOLOR_TABLE_ODD_ROW_BLUE
        c1 = GCOLOR_TABLE_EVEN_ROW_BLUE
    Case Else
        c0 = GCOLOR_TABLE_ODD_ROW_GRAY
        c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
    End Select
    With grd
        .Redraw = flexRDNone
        For i = 2 To .Rows - 1
            .Cell(flexcpBackColor, i, 0, i, .Cols - 1) = IIf(i Mod 2 > 0, c0, c1)
        Next
        .Redraw = flexRDBuffered
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ColorRows(grd As VSFlexGrid)
Dim i As Long
Dim j As Long
Dim c0 As Long
Dim c1 As Long
Dim c As Long
    On Error Resume Next
    c0 = GCOLOR_TABLE_ODD_ROW_GRAY
    c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
    With grd
        .Redraw = flexRDNone
        For i = 2 To .Rows - 1
            For j = 0 To .Cols - 1
                c = .Cell(flexcpBackColor, i, j)
                Select Case c
                Case BKG_COLOR_PRICE_FEED, GCOLOR_TABLE_ODD_ROW_BLUE, GCOLOR_TABLE_EVEN_ROW_BLUE
                    c0 = GCOLOR_TABLE_ODD_ROW_BLUE
                    c1 = GCOLOR_TABLE_EVEN_ROW_BLUE
                Case BKG_COLOR_CHILD_WARN, GCOLOR_TABLE_ODD_ROW_RED, GCOLOR_TABLE_EVEN_ROW_RED
                    c0 = GCOLOR_TABLE_ODD_ROW_RED
                    c1 = GCOLOR_TABLE_EVEN_ROW_RED
                Case BKG_COLOR_PRICE_MANUAL
                    c0 = BKG_COLOR_PRICE_MANUAL
                    c1 = BKG_COLOR_PRICE_MANUAL
                Case BKG_COLOR_ROW_CELECTED
                    c0 = BKG_COLOR_ROW_CELECTED
                    c1 = BKG_COLOR_ROW_CELECTED
                Case Else
                    c0 = GCOLOR_TABLE_ODD_ROW_GRAY
                    c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
                End Select
                .Cell(flexcpBackColor, i, j) = IIf(i Mod 2 > 0, c0, c1)
            Next
        Next
        .Redraw = True
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ColorCell(grd As VSFlexGrid, ByVal Row As Long, ByVal Col As Long, ByVal DataStatus As enmDATA_STATUS)
Dim c As Long
Dim c0 As Long
Dim c1 As Long
    If DataStatus = DATA_FEED Then
        c0 = GCOLOR_TABLE_ODD_ROW_BLUE
        c1 = GCOLOR_TABLE_EVEN_ROW_BLUE
    ElseIf DataStatus = DATA_MANUAL Then
        c0 = BKG_COLOR_PRICE_MANUAL
        c1 = BKG_COLOR_PRICE_MANUAL
    Else
        c0 = GCOLOR_TABLE_ODD_ROW_GRAY
        c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
    End If
    c = IIf(Row Mod 2 > 0, c0, c1)
    With grd
        .Cell(flexcpBackColor, Row, .ColIndex(Col)) = c
    End With
End Sub

