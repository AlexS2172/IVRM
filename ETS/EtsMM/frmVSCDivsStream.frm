VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmVSCDivsStream 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Divs stream for Var Swap Calc"
   ClientHeight    =   6195
   ClientLeft      =   45
   ClientTop       =   315
   ClientWidth     =   5055
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6195
   ScaleWidth      =   5055
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnCancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   3720
      TabIndex        =   11
      Top             =   5640
      Width           =   1215
   End
   Begin VB.CommandButton btnOk 
      Caption         =   "OK"
      Height          =   375
      Left            =   3720
      TabIndex        =   10
      Top             =   5040
      Width           =   1215
   End
   Begin VB.CommandButton btnDeleteAll 
      Caption         =   "Delete all"
      Height          =   375
      Left            =   3720
      TabIndex        =   9
      Top             =   2640
      Width           =   1215
   End
   Begin VB.CommandButton btnDelete 
      Caption         =   "Delete"
      Height          =   375
      Left            =   3720
      TabIndex        =   8
      Top             =   2040
      Width           =   1215
   End
   Begin VB.CommandButton btnAdd 
      Caption         =   "Add"
      Height          =   375
      Left            =   3720
      TabIndex        =   7
      Top             =   1200
      Width           =   1215
   End
   Begin VB.TextBox txtStep 
      Height          =   375
      Left            =   1920
      TabIndex        =   6
      Top             =   1200
      Width           =   1455
   End
   Begin VB.TextBox txtAmount 
      Height          =   375
      Left            =   1920
      TabIndex        =   5
      Top             =   720
      Width           =   1455
   End
   Begin MSComCtl2.DTPicker dtpDate 
      Height          =   375
      Left            =   1920
      TabIndex        =   4
      Top             =   240
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   661
      _Version        =   393216
      Format          =   65077249
      CurrentDate     =   39240
   End
   Begin MSComctlLib.ListView lvDivs 
      Height          =   4095
      Left            =   120
      TabIndex        =   0
      Top             =   2040
      Width           =   3375
      _ExtentX        =   5953
      _ExtentY        =   7223
      View            =   3
      LabelWrap       =   -1  'True
      HideSelection   =   0   'False
      FullRowSelect   =   -1  'True
      _Version        =   393217
      ForeColor       =   -2147483640
      BackColor       =   -2147483643
      BorderStyle     =   1
      Appearance      =   1
      NumItems        =   4
      BeginProperty ColumnHeader(1) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Text            =   "Days to divs"
         Object.Width           =   1958
      EndProperty
      BeginProperty ColumnHeader(2) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Alignment       =   2
         SubItemIndex    =   1
         Text            =   "Date"
         Object.Width           =   2081
      EndProperty
      BeginProperty ColumnHeader(3) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         Alignment       =   1
         SubItemIndex    =   2
         Text            =   "Amount"
         Object.Width           =   1764
      EndProperty
      BeginProperty ColumnHeader(4) {BDD1F052-858B-11D1-B16A-00C0F0283628} 
         SubItemIndex    =   3
         Object.Width           =   0
      EndProperty
   End
   Begin VB.Label Label3 
      Caption         =   "Add with step(days)"
      Height          =   375
      Left            =   240
      TabIndex        =   3
      Top             =   1320
      Width           =   1575
   End
   Begin VB.Label Label2 
      Caption         =   "Amount($)"
      Height          =   375
      Left            =   240
      TabIndex        =   2
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label1 
      Caption         =   "Date"
      Height          =   255
      Left            =   240
      TabIndex        =   1
      Top             =   240
      Width           =   735
   End
End
Attribute VB_Name = "frmVSCDivsStream"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private addCount As Integer
Public bOk As Boolean
Private Dates() As Date, Amounts() As Double
Public DivCount As Long

Public Sub Init(pDates() As Date, pAmounts() As Double, pDivCount As Long)
    Dim lstItem As ListItem, sortKey As String
    
    ReDim Dates(0)
    ReDim Amounts(0)
    lvDivs.ListItems.Clear
    bOk = False
    DivCount = pDivCount
    If DivCount > 0 Then
        For i = LBound(pDates) To UBound(pDates)
            Set lstItem = lvDivs.ListItems.Add(, , DateDiff("d", Date, pDates(i)))
            lstItem.SubItems(1) = pDates(i)
            lstItem.SubItems(2) = pAmounts(i)
            sortKey = CStr(Year(pDates(i)))
            If Month(pDates(i)) < 10 Then sortKey = sortKey & "0"
            sortKey = sortKey & CStr(Month(pDates(i)))
            If Day(pDates(i)) < 10 Then sortKey = sortKey & "0"
            sortKey = sortKey & CStr(Day(pDates(i)))
            lstItem.SubItems(3) = sortKey
        Next i
    End If
    dtpDate.MinDate = Date
    addCount = 1
    lvDivs.sortKey = 3
    lvDivs.Sorted = True
End Sub

Private Sub btnAdd_Click()
    Dim lstItem As ListItem, dte As Date, amount As Double, sortKey As String
    
    If IsNumeric(txtAmount.Text) = True Then
        If IsNumeric(txtStep.Text) = True Or txtStep.Text = "" Then
            dte = dtpDate.Value
            If txtStep.Text <> "" Then
                dte = DateAdd("d", CDbl(txtStep.Text) * addCount, dte)
                addCount = addCount + 1
            End If
            Set lstItem = lvDivs.FindItem(CStr(DateDiff("d", Date, dte)), lvwText)
                  
            If lstItem Is Nothing Then
                Set lstItem = lvDivs.ListItems.Add(, , DateDiff("d", Date, dte))
                lstItem.SubItems(1) = dte
                lstItem.SubItems(2) = txtAmount.Text
                sortKey = CStr(Year(dte))
                If Month(dte) < 10 Then sortKey = sortKey & "0"
                sortKey = sortKey & CStr(Month(dte))
                If Day(dte) < 10 Then sortKey = sortKey & "0"
                sortKey = sortKey & CStr(Day(dte))
                lstItem.SubItems(3) = sortKey
            Else
                amount = CDbl(lstItem.SubItems(2))
                amount = amount + CDbl(txtAmount.Text)
                lstItem.SubItems(2) = amount
            End If
        End If
    End If
End Sub

Private Sub btnCancel_Click()
    Unload Me
End Sub

Private Sub btnDelete_Click()
    If Not lvDivs.SelectedItem Is Nothing Then
        lvDivs.ListItems.Remove lvDivs.SelectedItem.Index
        DivCount = DivCount - 1
    End If
End Sub

Private Sub btnDeleteAll_Click()
    DivCount = 0
    lvDivs.ListItems.Clear
End Sub

Private Sub btnOk_Click()
    bOk = True
    If lvDivs.ListItems.Count > 0 Then
        ReDim Dates(0 To lvDivs.ListItems.Count - 1)
        ReDim Amounts(0 To lvDivs.ListItems.Count - 1)
        
        For i = 1 To lvDivs.ListItems.Count
            Dates(i - 1) = CDate(lvDivs.ListItems(i).ListSubItems(1).Text)
            Amounts(i - 1) = CDbl(lvDivs.ListItems(i).ListSubItems(2).Text)
        Next
    End If
    DivCount = lvDivs.ListItems.Count
    Unload Me
End Sub

Private Sub dtpDate_Change()
    addCount = 1
    txtStep.Text = ""
End Sub


Private Sub txtAmount_Change()
    addCount = 1
    txtStep.Text = ""
End Sub

Private Sub txtStep_Change()
    addCount = 1
End Sub
Public Function GetDivsStream(pDates() As Date, pAmounts() As Double) As Long
    Dim i As Long
    GetDivsStream = DivCount
        If DivCount > 0 Then
        ReDim pDates(UBound(Dates) - LBound(Dates))
        ReDim pAmounts(UBound(Dates) - LBound(Dates))
        For i = LBound(Dates) To UBound(Dates)
            pDates(i) = Dates(i)
            pAmounts(i) = Amounts(i)
        Next
    End If
End Function

