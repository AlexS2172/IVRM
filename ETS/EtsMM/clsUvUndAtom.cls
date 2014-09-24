VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsUvUndAtom"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public ID As Long
Public Symbol As String
Public ContractName As String
Public UndType As EtsGeneralLib.EtsContractTypeEnum

Public Dividend As EtsGeneralLib.EtsIndexDivAtom
Public Yield As Double

Public IsAmerican As Boolean
Public UndPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public OptPriceProfile As EtsGeneralLib.EtsPriceProfileAtom
Public Skew As Double
Public Kurt As Double

Public IsBasket As Boolean
Public HaveSynthRoots As Boolean
Public HaveOptions As Boolean
Public HavePos As Boolean
Public IsActive As Boolean
Public IsHTB As Boolean
Public SOQ As Double
Public IsAllowLending As Boolean
Public IsHedgeSymbol As Boolean

Public InAsset  As Boolean
Public IsHead   As Boolean
Public HeadComp As String
Public Coeff    As Double
Public UseHead  As Boolean

Public IsDirty As Boolean

Public HaveFutures As Boolean
Public FutRootSymbol As String
Public FutUndSymbol As String
Public FutMaturity As Date

Private m_Und As EtsGeneralLib.UndAtom
Private m_Fut As EtsGeneralLib.EtsFutAtom
Private m_FutRoot As EtsGeneralLib.EtsFutRootAtom

Public Function Init(ByRef aContract As EtsGeneralLib.EtsContractAtom) As Boolean
    On Error Resume Next
    Set m_Und = aContract.Und
    Set m_Fut = aContract.Fut
    Set m_FutRoot = aContract.FutRoot
    
    
    If aContract.ContractType = enCtIndex Or aContract.ContractType = enCtStock Or aContract.ContractType = enCtFutUnd And Not m_Und Is Nothing Then
        ID = m_Und.ID
        Symbol = m_Und.Symbol
        ContractName = Trim$(m_Und.ContractName)
        UndType = m_Und.UndType
    
        Set Dividend = m_Und.Dividend.Clone
        If Not Dividend Is Nothing Then
            CorrectDivs
        End If

        If UndType = enCtIndex Then
            Yield = m_Und.Yield
        End If
    
        IsAmerican = m_Und.IsAmerican
        Set UndPriceProfile = m_Und.UndPriceProfile
        Set OptPriceProfile = m_Und.OptPriceProfile
        Skew = m_Und.Skew
        Kurt = m_Und.Kurt

        IsBasket = m_Und.IsBasketIndex
        
        SOQ = m_Und.SOQ
        
        IsHead = m_Und.IsHead
        InAsset = IsHead Or (Not m_Und.HeadComponent Is Nothing)
        If IsHead Then
            HeadComp = m_Und.Symbol
        Else
            If Not m_Und.HeadComponent Is Nothing Then
                HeadComp = m_Und.HeadComponent.Symbol
            End If
        End If
        Coeff = m_Und.Coeff
        UseHead = m_Und.PriceByHead Or m_Und.IsHead

        HaveSynthRoots = m_Und.HaveSyntheticRoots
        HaveOptions = m_Und.HaveOptions
        HavePos = Not g_TradeChannel.TradesByUnd(CStr(m_Und.ID)) Is Nothing
        IsActive = m_Und.IsActive
        IsHTB = m_Und.IsHTB
        HaveFutures = m_Und.HaveFutures
        IsHedgeSymbol = m_Und.IsHedgeSymbol
        IsAllowLending = m_Und.IsAllowLending
        
        Init = True
        
    ElseIf aContract.ContractType = enCtFuture And Not m_Fut Is Nothing And Not m_FutRoot Is Nothing Then
        ID = m_Fut.ID
        Symbol = m_Fut.Symbol
        ContractName = Trim$(m_Fut.ContractName)
        UndType = enCtFuture
    
        IsAmerican = m_Fut.IsAmerican
        IsActive = m_Fut.IsActive
    
        Set UndPriceProfile = m_Fut.UndPriceProfile
        Set OptPriceProfile = m_Fut.OptPriceProfile
        
        HaveOptions = m_Fut.HaveOptions
        HavePos = Not g_TradeChannel.TradesByFut(CStr(m_Fut.ID)) Is Nothing
        
        FutRootSymbol = m_FutRoot.Symbol
        FutUndSymbol = m_Und.Symbol
        FutMaturity = m_Fut.MaturityDate
        
        InAsset = False
        IsHead = False
        Init = True
    End If

    IsDirty = False
End Function

Public Property Get UndTypeName() As String
    Select Case UndType
        Case enCtStock
            UndTypeName = "Stock"
            
        Case enCtIndex
            UndTypeName = IIf(IsBasket, "Basket", "Index")
            
        Case enCtFuture
            UndTypeName = "Futures"
        
        Case enCtFutUnd
            UndTypeName = "Future Underlying"
            
        Case Else
            Debug.Assert False
            UndTypeName = ""
    End Select
End Property

Public Property Get UndID()
    If UndType <> enCtFuture Then
        UndID = ID
    Else
        UndID = m_Und.ID
    End If
End Property

Public Property Get IsAmericanName() As String
    IsAmericanName = IIf(IsAmerican, "American", "European")
End Property

Public Property Get DivFreqName() As String
    Dim lFreq As Long
    Dim enType As EtsDivTypeEnum
    
    If m_Und Is Nothing Then
        DivFreqName = ""
        Exit Sub
    End If
    
    If Dividend Is Nothing Then
        DivFreqName = ""
        Exit Sub
    End If
    
    If UndType = enCtStock Or UndType = enCtIndex Then
        enType = Dividend.DivType
            
        If enType = enDivMarket Then
            lFreq = Dividend.DivFreq
        ElseIf (enType = enDivCustomPeriodical) Then
            lFreq = Dividend.DivFreqCust
        Else
            lFreq = -1
        End If
             
        Select Case lFreq
            Case 1
                DivFreqName = "Annually"
            Case 2
                DivFreqName = "Semiannually"
            Case 4
                DivFreqName = "Quarterly"
            Case 12
                DivFreqName = "Monthly"
            Case 0
                DivFreqName = "<No Divs>"
            Case Else
                DivFreqName = "--"
        End Select
        
    End If
    
End Property

Public Property Get UseCustDivsName() As String
    Dim enType As EtsDivTypeEnum
    
    If m_Und Is Nothing Then
        UseCustDivsName = ""
        Exit Sub
    End If
    
    If m_Und.Dividend Is Nothing Then
        UseCustDivsName = ""
        Exit Sub
    End If
    
    enType = Dividend.DivType
    
    If UndType = enCtStock Or enCtIndex Then
        Select Case enType
            Case enDivMarket
                UseCustDivsName = "Market"
            Case enDivCustomPeriodical
                UseCustDivsName = "Custom Periodical"
            Case enDivCustomStream
                UseCustDivsName = "Custom Stream"
            Case enDivStockBasket
                UseCustDivsName = "Stock Basket"
            Case enDivIndexYield
                UseCustDivsName = "Index Yield"
           Case Else
                UseCustDivsName = ""
        End Select
    Else
        UseCustDivsName = ""
    End If
End Property

Private Sub CorrectDivs()
    On Error Resume Next
    Dim DivFreq As Long
    Dim DivFreqCust As Long
    Dim DivDate As Double
    Dim DivDateCust As Double
    
    If m_Und Is Nothing Then
        Exit Sub
    End If
    
    If m_Und.Dividend Is Nothing Then
        Exit Sub
    End If
    
    DivFreq = m_Und.Dividend.DivFreq
    DivFreqCust = m_Und.Dividend.DivFreqCust
    DivDate = m_Und.Dividend.DivDate
    DivDateCust = m_Und.Dividend.DivDateCust
    
    If DivFreq <> 1 And DivFreq <> 2 And DivFreq <> 4 And DivFreq <> 12 And DivDate <= 0 Then
        DivFreq = 0
    End If
    
    If DivFreqCust <> 1 And DivFreqCust <> 2 And DivFreqCust <> 4 And DivFreqCust <> 12 And DivDateCust <= 0 Then
        DivFreqCust = 0
    End If
End Sub

Public Function IsValidDivs() As Boolean
    On Error Resume Next
    IsValidDivs = False
    If m_Und Is Nothing Then
        Exit Function
    End If
    
    
    Dim enType As EtsDivTypeEnum
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Set aDiv = Dividend
    
    If aDiv Is Nothing Then
        Exit Function
    End If
    
     enType = aDiv.DivType
     
     If enType = enDivMarket Then
         IsValidDivs = (aDiv.DivFreq = 1 Or aDiv.DivFreq = 2 Or aDiv.DivFreq = 4 Or aDiv.DivFreq = 12) _
                     And aDiv.DivDate > 0
     End If
     
     If enType = enDivCustomPeriodical Then
         IsValidDivs = (aDiv.DivFreqCust = 1 Or aDiv.DivFreqCust = 2 Or aDiv.DivFreqCust = 4 Or aDiv.DivFreqCust = 12) _
                     And aDiv.DivDateCust > 0
     End If
    
     If enType = enDivCustomStream Or enDivStockBasket Or enDivIndexYield Then
         IsValidDivs = True
     End If

End Function

Public Sub AdjustDirty()
    On Error Resume Next
    IsDirty = True
    
    If UndType = enCtStock Or UndType = enCtIndex Then
        If ID <> m_Und.ID Then Exit Sub
        If ContractName <> Trim$(m_Und.ContractName) Then Exit Sub
        
        If UndType = enCtIndex Then
            If Yield <> m_Und.Yield Then Exit Sub
        End If
        
        If Dividend.DivType <> m_Und.Dividend.DivType Then Exit Sub
        If Dividend.DivType = enDivCustomPeriodical And (Dividend.DivFreqCust <> m_Und.Dividend.DivFreqCust Or Dividend.DivAmtCust <> m_Und.Dividend.DivAmtCust Or Dividend.DivDateCust <> m_Und.Dividend.DivDateCust) Then Exit Sub

    
        If IsAmerican <> m_Und.IsAmerican Then Exit Sub
        If UndPriceProfile.ID <> m_Und.UndPriceProfile.ID Then Exit Sub
        If OptPriceProfile.ID <> m_Und.OptPriceProfile.ID Then Exit Sub
    
        If Skew <> m_Und.Skew Then Exit Sub
        If Kurt <> m_Und.Kurt Then Exit Sub
        If IsHedgeSymbol <> m_Und.IsHedgeSymbol Then Exit Sub
        
        If IsHTB <> m_Und.IsHTB Then Exit Sub
        If IsAllowLending <> m_Und.IsAllowLending Then Exit Sub
        If IsHedgeSymbol <> m_Und.IsHedgeSymbol Then Exit Sub
        If IsActive <> m_Und.IsActive Then Exit Sub
        If SOQ <> m_Und.SOQ Then Exit Sub
        
    Else
        If ID <> m_Fut.ID Then Exit Sub
        If ContractName <> Trim$(m_Fut.ContractName) Then Exit Sub
    
        If IsAmerican <> m_Fut.IsAmerican Then Exit Sub
        If UndPriceProfile.ID <> m_Fut.UndPriceProfile.ID Then Exit Sub
        If OptPriceProfile.ID <> m_Fut.OptPriceProfile.ID Then Exit Sub
    
        If IsActive <> m_Fut.IsActive Then Exit Sub
    End If
    
    If InAsset And Not IsHead Then
        If Coeff <> m_Und.Coeff Then Exit Sub
        If UseHead <> m_Und.PriceByHead Then Exit Sub
    End If

    IsDirty = False
End Sub

Public Sub SaveChanges()
    On Error Resume Next
    
    If UndType = enCtStock Or UndType = enCtIndex Then
        m_Und.ContractName = Trim$(ContractName)
    
        If UndType = enCtIndex Then
            m_Und.Yield = Yield
        End If
        
        Set m_Und.Dividend = Dividend.Clone
    
        m_Und.IsAmerican = IsAmerican
        Set m_Und.UndPriceProfile = UndPriceProfile
        Set m_Und.OptPriceProfile = OptPriceProfile
        m_Und.Skew = Skew
        m_Und.Kurt = Kurt
    
        m_Und.IsActive = IsActive
        m_Und.IsHTB = IsHTB
        m_Und.IsAllowLending = IsAllowLending
        m_Und.SOQ = SOQ
        
        Dim aData As MSGSTRUCTLib.UnderlyingUpdate
        Set aData = New MSGSTRUCTLib.UnderlyingUpdate
        
        aData.DivAmt = m_Und.Dividend.DivAmt
        aData.DivAmtCust = m_Und.Dividend.DivAmtCust
        aData.DivDate = m_Und.Dividend.DivDate
        aData.DivDateCust = m_Und.Dividend.DivDateCust
        aData.DivFreq = m_Und.Dividend.DivFreq
        aData.DivFreqCust = m_Und.Dividend.DivFreqCust
        aData.DivType = m_Und.Dividend.DivType
        aData.Yield = m_Und.Yield
        
        
        aData.UndID = m_Und.ID
        aData.UpdStatus = enUndDividendUpdate
        
        g_TradeChannel.PubUnderlyingUpdate aData
        
        
        SaveHedgeSymbol
    Else
        m_Fut.ContractName = Trim$(ContractName)
        
        m_Fut.IsAmerican = IsAmerican
        Set m_Fut.UndPriceProfile = UndPriceProfile
        Set m_Fut.OptPriceProfile = OptPriceProfile
    
        m_Fut.IsActive = IsActive
    End If
    
    If InAsset And Not IsHead Then
        SaveAsset
    End If

    IsDirty = False
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    Set m_Und = Nothing
    Set m_Fut = Nothing
    Set m_FutRoot = Nothing
End Sub

Private Sub SaveHedgeSymbol()
    On Error Resume Next
    Dim rs As ADODB.Recordset
    Dim aIdx As EtsGeneralLib.IndexAtom, aIdxComp As EtsGeneralLib.IndexCompAtom
    Dim dBeta As Double
    Dim nID As Long
    
    If m_Und.IsHedgeSymbol = IsHedgeSymbol Then Exit Sub
    
    If IsHedgeSymbol = False Then
        g_HedgeSymbols.Remove m_Und.ID
    Else
    
        ' load betas from EOD database
        gDBW.usp_HedgeSymbolBeta_Load (m_Und.ID)
    
        ' fill collection
        Set aIdx = g_HedgeSymbols(m_Und.ID)
        If aIdx Is Nothing Then
            Set aIdx = New EtsGeneralLib.IndexAtom
            aIdx.ID = m_Und.ID
            aIdx.Symbol = m_Und.Symbol
                    
            Set aIdx.UndPriceProfile = g_UnderlyingAll(aIdx.ID).UndPriceProfile
            Set aIdx.OptPriceProfile = g_UnderlyingAll(aIdx.ID).OptPriceProfile
                    
            g_HedgeSymbols.Add aIdx.ID, aIdx.Symbol, aIdx
        End If
    
        Set rs = gDBW.usp_IndexUnderlyingBeta_Get(aIdx.ID)
        While Not rs.EOF
            dBeta = ReadDbl(rs!fBeta)
            
            If dBeta <> 0# Then
                nID = ReadLng(rs!iContractID)
                
                Set aIdxComp = aIdx.Components(nID)
                If aIdxComp Is Nothing Then
                    Set aIdxComp = aIdx.Components.Add(nID)
                    aIdxComp.ID = nID
                End If
                    
                aIdxComp.Beta = dBeta
                aIdx.HaveComponentBetas = True
                    
                Set aIdxComp = Nothing
            End If
            
            rs.MoveNext
        Wend
        
        Set aIdx = Nothing
    End If
        
    m_Und.IsHedgeSymbol = IsHedgeSymbol

End Sub

Private Sub SaveAsset()
    On Error Resume Next
    
    If Not m_Und Is Nothing Then
        If Not m_Und.HeadComponent Is Nothing Then
    
            m_Und.Coeff = Coeff
            m_Und.PriceByHead = UseHead
            
            gDBW.usp_AssetGroupElement_Save m_Und.HeadComponent.ID, m_Und.ID, IIf(m_Und.PriceByHead, 1, 0), m_Und.Coeff
            
            Dim Data As MSGSTRUCTLib.UnderlyingUpdate
            Set Data = New MSGSTRUCTLib.UnderlyingUpdate
            
            Data.UpdStatus = enUndAggregationUpdate
            Data.UndID = m_Und.ID
            Data.Coeff = m_Und.Coeff
            Data.UseHead = m_Und.PriceByHead
            
            g_TradeChannel.PubUnderlyingUpdate Data
            
        End If
    End If
    
End Sub

