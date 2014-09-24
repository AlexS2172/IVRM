VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsMainParser"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit
Public Event UpdateLog(ByVal sInfo As String)

Private Sub MsgLog(ByVal sInfo As String)
    RaiseEvent UpdateLog(sInfo)
End Sub
Private Function LoadStringFromFile(ByVal FileName As String) As String
    Dim s$, n&, f&
    n = FileLen(FileName)
    If n > 0 Then
        s = Space(n)
        f = FreeFile
        Open FileName For Binary Access Read As #f
        Get #f, , s
        Close #f
        LoadStringFromFile = s
    End If
End Function
Private Function LoadFileDataLines(ByVal sFileName As String, ByRef Lines As Variant) As Boolean
    On Error GoTo Err
    
    Dim Buffer As String

    Buffer = LoadStringFromFile(sFileName)
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
    End If
    
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            LoadFileDataLines = False
            GoTo Err
        End If
    End If
    LoadFileDataLines = True
    Exit Function
Err:
    MsgLog ("Fail to read file: " & sFileName)
End Function
Public Function LoadSymbolVolaFromFile(ByVal sFileName As String, ByRef collSymVola As clsSymbolVolaColl) As Long
    On Error GoTo Err
    
    Dim Lines As Variant
    Dim l As Variant
    Dim nBegin&, nEnd&, nLineNumber&
    Dim Buffer As String
    Dim a_svAtom As clsSymbolVolaAtom
    Dim lParsResult As Long
    
    MsgLog ("[Start] Load Symbol Vola")
           
    If LoadFileDataLines(sFileName, Lines) Then
        
        nEnd = UBound(Lines)
        collSymVola.Clear
        nBegin = 0
    
        For Each l In Lines
            On Error GoTo Err
            Set a_svAtom = ParseSymbolVolaStringLine(l)
            If Not a_svAtom Is Nothing Then
                collSymVola.Add a_svAtom.Symbol, a_svAtom
                MsgLog ("[Loaded] " & l)
            Else
                MsgLog ("[Fail to parse] " & l)
            End If
        Next
        LoadSymbolVolaFromFile = nEnd + 1
    Else
        LoadSymbolVolaFromFile = -1
    End If
    MsgLog ("[Stop] Load Symbol Vola")
    Exit Function
Err:
    MsgLog ("[Stop with errors] Load Symbol Vola")
End Function
Private Function LoadIndexWeightFromFile(ByVal sFileName As String, ByRef collIndexWeight As clsIndexWeightColl) As Long
    Dim Lines As Variant
    Dim l As Variant
    Dim nBegin&, nEnd&, nLineNumber&
    Dim Buffer As String
    Dim a_iwAtom As New clsIndexWeightAtom
    Dim lParsResult As Long

    On Error GoTo Err
       
    If LoadFileDataLines(sFileName, Lines) Then
    
        nEnd = UBound(Lines)
        collIndexWeight.Clear
        nBegin = 0
    
        For Each l In Lines
            On Error GoTo Err
            Set a_iwAtom = ParseIndexWeightStringLine(l)
            If Not a_iwAtom Is Nothing Then
                collIndexWeight.Add a_iwAtom.IndexSymbol & "_" & a_iwAtom.ComponentSymbol, a_iwAtom
            End If
        Next
        LoadIndexWeightFromFile = nEnd + 1
    Else
        LoadIndexWeightFromFile = -1
    End If
    
Err:
    
End Function
Private Function LoadDividendsFromFile(ByVal sFileName As String, ByRef collDiv As clsDividendsColl) As Long
    Dim Lines As Variant
    Dim l As Variant
    Dim nBegin&, nEnd&, nLineNumber&
    Dim Buffer As String
    Dim a_dvAtom As New clsDividendAtom
    Dim lParsResult As Long

    On Error GoTo Err
       
    If LoadFileDataLines(sFileName, Lines) Then
    
        nEnd = UBound(Lines)
        collDiv.Clear
        nBegin = 0
    
        For Each l In Lines
            On Error GoTo Err
            Set a_dvAtom = ParseIndexWeightStringLine(l)
            If Not a_dvAtom Is Nothing Then
                collDiv.Add a_dvAtom.StockSymbol & "_" & Str(a_dvAtom.DividendDate), a_dvAtom
            End If
        Next
        LoadDividendsFromFile = nEnd + 1
    Else
        LoadDividendsFromFile = -1
    End If
    
Err:
        
End Function
Private Function LoadInterestRatesFromFile(ByVal sFileName As String, ByRef collIntRate As clsInterestRateColl) As Long
    Dim Lines As Variant
    Dim l As Variant
    Dim nBegin&, nEnd&, nLineNumber&
    Dim Buffer As String
    Dim a_irAtom As New clsInterestRateAtom
    Dim lParsResult As Long

    On Error GoTo Err
       
    If LoadFileDataLines(sFileName, Lines) Then
    
        nEnd = UBound(Lines)
        collIntRate.Clear
        nBegin = 0
    
        For Each l In Lines
            On Error GoTo Err
            Set a_irAtom = ParseInterestRatesStringLine(l)
            If Not a_irAtom Is Nothing Then
                collIntRate.Add Str(a_irAtom.Period) & "_" & Str(a_irAtom.LongUSDSpread) & "_" & Str(a_irAtom.ShortUSDSpread), a_irAtom
            End If
        Next
        LoadInterestRatesFromFile = nEnd + 1
    Else
        LoadInterestRatesFromFile = -1
    End If
    
Err:
End Function
Private Function ParseSymbolVolaStringLine(ByVal Buffer As String) As clsSymbolVolaAtom
    Dim Lines As Variant
    Dim sLine As String
    Dim aSVAtom As New clsSymbolVolaAtom
           
    Lines = Split(Buffer, ",")
    If UBound(Lines) <= 0 Then
        GoTo Err
    End If
       
    If UBound(Lines) = 3 Then
        aSVAtom.Clear
        aSVAtom.Symbol = gCmn.ReadStr(Lines(0))
        aSVAtom.Expiry = gCmn.ReadDate(Lines(1))
        aSVAtom.Strike = gCmn.ReadDbl(Lines(2))
        aSVAtom.Volatility = gCmn.ReadDbl(Lines(3))
        Set ParseSymbolVolaStringLine = aSVAtom
    Else
        Set ParseSymbolVolaStringLine = Nothing
    End If
Err:
    
End Function
Private Function ParseIndexWeightStringLine(ByVal Buffer As String) As clsIndexWeightAtom
    Dim Lines As Variant
    Dim sLine As String
    Dim aIWAtom As New clsIndexWeightAtom
    
    Lines = Split(Buffer, ",")
    If UBound(Lines) <= 0 Then
        GoTo Err
    End If
       
    If UBound(Lines) = 2 Then
        aIWAtom.Clear
        aIWAtom.IndexSymbol = gCmn.ReadStr(Lines(0))
        aIWAtom.ComponentSymbol = gCmn.ReadStr(Lines(1))
        aIWAtom.Weight = gCmn.ReadDbl(Lines(2))
        Set ParseIndexWeightStringLine = aIWAtom
    Else
        Set ParseIndexWeightStringLine = Nothing
    End If
Err:
End Function
Private Function ParseDividendsStringLine(ByVal Buffer As String) As clsDividendAtom
    Dim Lines As Variant
    Dim sLine As String
    Dim aDVAtom As New clsDividendAtom
    
    Lines = Split(Buffer, ",")
    If UBound(Lines) <= 0 Then
        GoTo Err
    End If
       
    If UBound(Lines) = 2 Then
        aDVAtom.Clear
        aDVAtom.StockSymbol = gCmn.ReadStr(Lines(0))
        aDVAtom.DividendDate = gCmn.ReadDate(Lines(1))
        aDVAtom.DividendAmount = gCmn.ReadDbl(Lines(2))
        Set ParseDividendsStringLine = aDVAtom
    Else
        Set ParseDividendsStringLine = Nothing
    End If
Err:
End Function
Private Function ParseInterestRatesStringLine(ByVal Buffer As String) As clsInterestRateAtom
    Dim Lines As Variant
    Dim sLine As String
    Dim aIRAtom As New clsInterestRateAtom
    
    Lines = Split(Buffer, ",")
    If UBound(Lines) <= 0 Then
        GoTo Err
    End If
       
    If UBound(Lines) = 3 Then
        aIRAtom.Clear
        aIRAtom.Period = gCmn.ReadLng(Lines(0))
        aIRAtom.BaseUSDRatePrc = gCmn.ReadDbl(Lines(1))
        aIRAtom.LongUSDSpread = gCmn.ReadDbl(Lines(2))
        aIRAtom.ShortUSDSpread = gCmn.ReadDbl(Lines(3))
        Set ParseInterestRatesStringLine = aIRAtom
    Else
        Set ParseInterestRatesStringLine = Nothing
    End If
Err:
End Function

