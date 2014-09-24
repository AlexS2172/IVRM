VERSION 5.00
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Begin VB.Form frmIndexDivComponents 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Index Dividends"
   ClientHeight    =   4335
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5655
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4335
   ScaleWidth      =   5655
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnOk 
      Appearance      =   0  'Flat
      Caption         =   "&Ok"
      Height          =   375
      Left            =   120
      TabIndex        =   1
      Top             =   3840
      Width           =   1335
   End
   Begin VSFlex8UCtl.VSFlexGrid fgDivComp 
      Height          =   3495
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   5415
      _cx             =   9551
      _cy             =   6165
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483636
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   2
      GridLineWidth   =   1
      Rows            =   1
      Cols            =   3
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmIndexDivComponents.frx":0000
      ScrollTrack     =   0   'False
      ScrollBars      =   3
      ScrollTips      =   0   'False
      MergeCells      =   0
      MergeCompare    =   0
      AutoResize      =   -1  'True
      AutoSizeMode    =   0
      AutoSearch      =   0
      AutoSearchDelay =   2
      MultiTotals     =   -1  'True
      SubtotalPosition=   1
      OutlineBar      =   0
      OutlineCol      =   0
      Ellipsis        =   0
      ExplorerBar     =   0
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   0
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   1
      WordWrap        =   0   'False
      TextStyle       =   0
      TextStyleFixed  =   0
      OleDragMode     =   0
      OleDropMode     =   0
      DataMode        =   0
      VirtualData     =   -1  'True
      DataMember      =   ""
      ComboSearch     =   3
      AutoSizeMouse   =   -1  'True
      FrozenRows      =   0
      FrozenCols      =   0
      AllowUserFreezing=   0
      BackColorFrozen =   0
      ForeColorFrozen =   0
      WallPaperAlignment=   9
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   24
   End
End
Attribute VB_Name = "frmIndexDivComponents"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private m_nUnderlyingID As Long
Private m_dtDivDate As Date
Private m_aBasketIndex As EtsGeneralLib.IndexAtom
Private m_aCompColl As EtsGeneralLib.IndexCompColl

Private Enum GridColumns
    IDX_STOCK = 0
    IDX_DIVAMOUNT = 1
    IDX_WEIGHTAMOUNT = 2
End Enum

Public Sub Init(ByVal nUnderlyingID As Long, ByVal dtDivDate As Date)
       
    On Error GoTo Err
    
    Dim aComp As EtsGeneralLib.IndexCompAtom
    Dim aUnd As EtsGeneralLib.UndAtom
    
    Dim iDivCount As Long, iRetCount As Long, iCycle As Long, iRowCounter As Long
    Dim DivAmounts() As Double
    Dim DivDates() As Double
    
    Dim dtNow As Date
    
    m_nUnderlyingID = nUnderlyingID
    m_dtDivDate = dtDivDate
    iRowCounter = 0
    dtNow = GetNewYorkTime
    
    Set m_aBasketIndex = g_BasketIndex(m_nUnderlyingID)
    
    If (Not m_aBasketIndex Is Nothing) Then
    
        Caption = m_aBasketIndex.Symbol & " dividends on " & Format(dtDivDate, "mmm d yyyy")
        
        Set m_IndexCompColl = m_aBasketIndex.Components
        If (Not m_IndexCompColl Is Nothing) Then
            For Each aComp In m_IndexCompColl
                Set aUnd = g_UnderlyingAll(aComp.ID)
                
                If (Not aUnd Is Nothing And aComp.Weight > 0) Then
                   If (Not aUnd.Dividend Is Nothing) Then
                      iRetCount = 0
                      aUnd.Dividend.GetDividendCount2 dtNow, CDate("31/12/3000"), CDate("00:00:00"), iDivCount
                      
                      If (iDivCount > 0) Then
                        aUnd.Dividend.GetDividends2 dtNow, CDate("31/12/3000"), CDate("00:00:00"), iDivCount, DivAmounts, DivDates, iRetCount
                        
                        If iRetCount > 0 Then
                          For iCycle = 0 To iRetCount - 1
                            If (Abs(CDate(DivDates(iCycle) * 365 + dtNow) - m_dtDivDate) < 0.001) Then
                                                        
                                fgDivComp.AddItem aUnd.Symbol
                                iRowCounter = iRowCounter + 1
                                fgDivComp.TextMatrix(iRowCounter, IDX_DIVAMOUNT) = DivAmounts(iCycle)
                                fgDivComp.TextMatrix(iRowCounter, IDX_WEIGHTAMOUNT) = aComp.Weight * DivAmounts(iCycle)
                                                             
                            End If
                          Next iCycle
                        End If
                        
                      End If
                   End If
                End If
            Next
        End If
    End If
    Exit Sub
Err:
    Debug.Print "Error"
End Sub

Private Sub btnOk_Click()
    Unload Me
End Sub
