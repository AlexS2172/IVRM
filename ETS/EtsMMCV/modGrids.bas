Attribute VB_Name = "modGrids"
Option Explicit

Public Const IDX_NONE As Long = -1
Public Const STR_NA As String = "--"
Public Const STR_ZERO As String = "0"
Public Const GRID_VERT_SPACE As Long = 0

' ------------------------------------------------------------------------------------------------------
' - Quotes View                                                                                        -
' ------------------------------------------------------------------------------------------------------
'
'Public Enum QuotesUndColumnEnum
'    QUC_NONE = IDX_NONE
'    QUC_KEY = 0
'
'    QUC_SYMBOL
'    QUC_EXCHANGE
'    QUC_BID
'    QUC_ASK
'    QUC_LAST
'    QUC_HIGH
'    QUC_LOW
'    QUC_OPEN
'    QUC_CLOSE
'    QUC_VOLUME
'    QUC_POS
'    QUC_NET_CHANGE
'    QUC_BID_SIZE
'    QUC_ASK_SIZE
'    QUC_UPDATE_TIME
'    QUC_NET_DELTA
'    QUC_OPT_DELTA
'    QUC_GAMMA_USD
'    QUC_VEGA
'    QUC_THETA
'    QUC_RHO
'    QUC_SYMBOL_NAME
'    QUC_DPC
'    QUC_FUT_ROOT
'    QUC_FUT_QTY
'    QUC_BA_EXCHANGE
'    QUC_COLUMN_COUNT
'    QUC_LAST_COLUMN = QUC_COLUMN_COUNT - 1
'End Enum
'
'Public Enum QuotesDivColumnEnum
'    QDC_TRADES = 0
'    QDC_MODEL
'    QDC_STYLE
'    QDC_CALENDAR
'    QDC_UNDPROFILE
'    QDC_OPTPROFILE
'    QDC_DIV
'    QDC_FREQ
'    QDC_DATE
'    QDC_AMT
'    QDC_RATE
'    QDC_RATE_VAL
'
'    QDC_COLUMN_COUNT
'    QDC_LAST_COLUMN = QDC_COLUMN_COUNT - 1
'End Enum
'
'Public Enum QuotesVolaColumnEnum
'    QVC_VOLA = 0
'    QVC_VOLA_VAL
'    QVC_VOLA_DATECALC
'
'    QVC_COLUMN_COUNT
'    QVC_LAST_COLUMN = QVC_COLUMN_COUNT - 1
'End Enum
'
'Public Enum QuotesOptColumnEnum
'    QOC_NONE = IDX_NONE
'    QOC_C_KEY = 0
'    QOC_P_KEY
'
'    QOC_C_SYMBOL
'    QOC_C_EXCHANGE
'    QOC_C_SERIES
'    QOC_C_EXPIRY
'    QOC_C_STRIKE
'    QOC_C_BID
'    QOC_C_ASK
'    QOC_C_LAST
'    QOC_C_VOLUME
'    QOC_C_QTY
'    QOC_C_VOLA
'    QOC_C_THEO_PRICE
'    QOC_C_IV
'    QOC_C_IV_BID
'    QOC_C_IV_ASK
'    QOC_C_OPEN_INTEREST
'    QOC_C_DELTA
'    QOC_C_GAMMA
'    QOC_C_VEGA
'    QOC_C_THETA
'    QOC_C_RHO
'    QOC_C_VEGA_DELTA
'    QOC_C_VEGA_GAMMA
'    QOC_C_THETA_DELTA
'    QOC_C_THETA_GAMMA
'    QOC_C_GAMMA_PER_THETA
'    QOC_C_NET_CHANGE
'    QOC_C_BID_SIZE
'    QOC_C_ASK_SIZE
'    QOC_C_IV_SPREAD
'    QOC_C_BA_EXCHANGE
'    QOC_C_UPDATE_TIME
'
'    QOC_P_SYMBOL
'    QOC_P_EXCHANGE
'    QOC_P_SERIES
'    QOC_P_EXPIRY
'    QOC_P_STRIKE
'    QOC_P_BID
'    QOC_P_ASK
'    QOC_P_LAST
'    QOC_P_VOLUME
'    QOC_P_QTY
'    QOC_P_VOLA
'    QOC_P_THEO_PRICE
'    QOC_P_IV
'    QOC_P_IV_BID
'    QOC_P_IV_ASK
'    QOC_P_OPEN_INTEREST
'    QOC_P_DELTA
'    QOC_P_GAMMA
'    QOC_P_VEGA
'    QOC_P_THETA
'    QOC_P_RHO
'    QOC_P_VEGA_DELTA
'    QOC_P_VEGA_GAMMA
'    QOC_P_THETA_DELTA
'    QOC_P_THETA_GAMMA
'    QOC_P_GAMMA_PER_THETA
'    QOC_P_NET_CHANGE
'    QOC_P_BID_SIZE
'    QOC_P_ASK_SIZE
'    QOC_P_IV_SPREAD
'    QOC_P_BA_EXCHANGE
'    QOC_P_UPDATE_TIME
'
'    QOC_ROOT
'    QOC_IS_SYNTH
'    QOC_DPC
'    QOC_SU_PRICE
'
'    QOC_COLUMN_COUNT
'    QOC_LAST_COLUMN = QOC_COLUMN_COUNT - 1
'End Enum

' ------------------------------------------------------------------------------------------------------
' - Trades View                                                                                        -
' ------------------------------------------------------------------------------------------------------

'Public Enum TradesFilterColumnEnum
'    TFC_NONE = IDX_NONE
'
'    TFC_UNDERLYING
'    TFC_TYPE
'    TFC_GROUP
'    TFC_TRADER
'    TFC_STRATEGY
'
'    TFC_COLUMN_COUNT
'    TFC_LAST_COLUMN = TFC_COLUMN_COUNT - 1
'End Enum

'Public Enum TradesListColumnEnum
'    TLC_NONE = IDX_NONE
'    TLC_KEY = 0
'
'    TLC_TYPE
'    TLC_UNDERLYING
'    TLC_SYMBOL
'    TLC_OPT_TYPE
'    TLC_EXPIRY
'    TLC_STRIKE
'    TLC_BS
'    TLC_PRICE
'    TLC_QTY
'    TLC_LOTSIZE
'    TLC_TOT_PRICE
'    TLC_DATE
'    TLC_TRADER
'    TLC_STRATEGY
'    TLC_BROKER
'    TLC_CLR_BROKER
'    TLC_COMMISSION
'    TLC_MATCHED
'    TLC_MARKED
'    TLC_FUT_ROOT
'    TLC_FUTURES
'    TLC_FUT_MATURITY
'    TLC_STOCKREFPRICE
'    TLC_TRADED_IV
'
'    TLC_COLUMN_COUNT
'    TLC_LAST_COLUMN = TLC_COLUMN_COUNT - 1
'End Enum

' ------------------------------------------------------------------------------------------------------
' - Risk View                                                                                          -
' ------------------------------------------------------------------------------------------------------

Public Enum RisksFilterColumnEnum
    RFC_NONE = IDX_NONE
    
    RFC_GROUP
    RFC_VALUE
    RFC_TYPE
    RFC_EXPIRY
    RFC_INDEX
    RFC_MODEL
    RFC_SIM
    RFC_SIM_TYPE
    RFC_SIM_DATE
    RFC_SIM_VOLA
    RFC_SIM_SPOT
    RFC_SIM_VOLA_SHIFT_TYPE
    RFC_SHIFT_ID
    RFC_SHIFT_FILE
    
    RFC_COLUMN_COUNT
    RFC_LAST_COLUMN = RFC_COLUMN_COUNT - 1
End Enum

Public Enum RisksTotalColumnEnum
    RTC_NONE = IDX_NONE

    RTC_PNL_MTM
    RTC_PNL_THEO
    RTC_PNL_EDGE
    RTC_VEGA
    RTC_WTD_VEGA
    RTC_THETA
    RTC_DELTA_EQ
    RTC_GAMMA_EQ
    RTC_RHO
    RTC_INDEX_DELTA_EQ
    RTC_BETA_WTD_DELTA
    RTC_OPT_DELTA
    RTC_NET_DELTA
    RTC_NET_GAMMA
    RTC_BETA_WTD_DELTA_USD

    RTC_COLUMN_COUNT
    RTC_LAST_COLUMN = RTC_COLUMN_COUNT - 1
End Enum

Public g_RTC_COLUMN_COUNT As Long
Public g_RTC_LAST_COLUMN As Long

Public Enum RisksPosColumnEnum
    RPC_NONE = IDX_NONE
    RPC_KEY = 0

    RPC_SYMBOL
    RPC_OPT_TYPE
    RPC_EXPIRY
    RPC_STRIKE
    RPC_BID
    RPC_ASK
    RPC_LAST
    RPC_UND_POS
    RPC_OPT_QTY
    RPC_PNL_MTM
    RPC_PNL_THEO
    RPC_PNL_EDGE
    RPC_NET_DELTA
    RPC_OPT_DELTA
    RPC_GAMMA
    RPC_NET_GAMMA
    RPC_GAMMA_SHARES
    RPC_VEGA
    RPC_WTD_VEGA
    RPC_THETA
    RPC_RHO
    RPC_VEGA_DELTA
    RPC_VEGA_GAMMA
    RPC_THETA_DELTA
    RPC_THETA_GAMMA
    RPC_UND
    RPC_DEL_UNIT
    RPC_TIME_VALUE
    RPC_NET_DELTA_USD
    RPC_BETA_WTD_DELTA_USD
    RPC_CLOSE
    RPC_FUT_ROOT
    RPC_FUTURES
    RPC_FUT_MATURITY
    RPC_FUT_QTY
    RPC_IMPORT_ID
    RPC_NETCHANGE
    RPC_DELTAP
    RPC_GAMMAP
    RPC_VEGAP
    RPC_THETAP
    RPC_RHOP
    RPC_DTE
    RPC_THEO_VOL
    RPC_MIV
    RPC_VOLGA
    
    RPC_COLUMN_COUNT
    RPC_LAST_COLUMN = RPC_COLUMN_COUNT - 1
End Enum

Public g_RPC_COLUMN_COUNT As Long
Public g_RPC_LAST_COLUMN As Long

' ------------------------------------------------------------------------------------------------------
' - Risk Matrix                                                                                        -
' ------------------------------------------------------------------------------------------------------

Public Enum MatrixFilterColumnEnum
    MFC_NONE = IDX_NONE
    
    MFC_GROUP
    MFC_VALUE
    MFC_TYPE
    MFC_EXPIRY
    MFC_SHIFT
    MFC_INDEX
    MFC_MODEL
    MFC_PNL
    MFC_DELTA
    MFC_NET_DELTA
    MFC_GAMMA
    MFC_GAMMA_PERC
    MFC_NET_GAMMA
    MFC_VEGA
    MFC_THETA
    MFC_RHO
    MFC_WTD_VEGA
    
    MFC_COLUMN_COUNT
    MFC_LAST_COLUMN = MFC_COLUMN_COUNT - 1
End Enum

Public Enum MatrixScenarioColumnEnum
    MSC_NONE = IDX_NONE
    
    MSC_NAME
    MSC_HORZ_AXIS
    MSC_HORZ_POINTS
    MSC_HORZ_STEP
    MSC_HORZ_UNITS
    MSC_VERT_AXIS
    MSC_VERT_POINTS
    MSC_VERT_STEP
    MSC_VERT_UNITS
    MSC_VOLA_SHIFT
    
    MSC_COLUMN_COUNT
    MSC_LAST_COLUMN = MSC_COLUMN_COUNT - 1
End Enum

Public Enum MatrixValuesColumnEnum
    MVC_NONE = IDX_NONE
    
    MVC_VALUE
    MVC_BASE
    
    MVC_COLUMN_COUNT
    MVC_LAST_COLUMN = MVC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Volatility Monitor                                                                                 -
' ------------------------------------------------------------------------------------------------------

Public Enum VolaFilterColumnEnum
    VFC_NONE = IDX_NONE
    
    VFC_GROUP
    VFC_VALUE
    VFC_EXPIRY_1
    VFC_EXPIRY_2
    VFC_VOLA_1
    VFC_VOLA_2
    VFC_VEGA
    VFC_GAMMA
    VFC_MODEL
    
    VFC_COLUMN_COUNT
    VFC_LAST_COLUMN = VFC_COLUMN_COUNT - 1
End Enum

Public Enum VolaUnderlyingColumnEnum
    VUC_NONE = IDX_NONE
    VUC_KEY = 0
    
    VUC_SYMBOL
    VUC_BID
    VUC_ASK
    VUC_LAST
    VUC_EXPIRY_1
    VUC_EXPIRY_2
    VUC_STRIKE_1
    VUC_STRIKE_2
    VUC_IV_1
    VUC_IV_2
    VUC_VOLA_1
    VUC_VOLA_2
'    VUC_TARG_1
'    VUC_TARG_2
    VUC_VOLA_IV_1
    VUC_VOLA_IV_2
'    VUC_TARG_IV_1
'    VUC_TARG_IV_2
    VUC_POS
    VUC_NET_DELTA
    VUC_OPT_DELTA
    VUC_GAMMA
    VUC_VEGA
    VUC_THETA
    VUC_RHO
    
    VUC_COLUMN_COUNT
    VUC_LAST_COLUMN = VUC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Quotation View                                                                                     -
' ------------------------------------------------------------------------------------------------------

Public Enum QuotationUndColumnEnum
    UUC_NONE = IDX_NONE
    UUC_KEY = 0
    
    UUC_SYMBOL
    UUC_EXCHANGE
    UUC_BID
    UUC_ASK
    UUC_LAST
    UUC_HIGH
    UUC_LOW
    UUC_OPEN
    UUC_CLOSE
    UUC_VOLUME
    UUC_POS
    UUC_NET_CHANGE
    UUC_BID_SIZE
    UUC_ASK_SIZE
    UUC_BIN_NUM
    UUC_TRADER_ROLE
    UUC_STATUS
    
    UUC_COLUMN_COUNT
    UUC_LAST_COLUMN = UUC_COLUMN_COUNT - 1
End Enum

Public Enum QuotationDivColumnEnum
    UDC_MODEL = 0
    UDC_STYLE
    UDC_CALENDAR
    UDC_DIV
    UDC_FREQ
    UDC_DATE
    UDC_AMT
    UDC_RATE
    UDC_RATE_VAL
    
    UDC_COLUMN_COUNT
    UDC_LAST_COLUMN = UDC_COLUMN_COUNT - 1
End Enum

Public Enum QuotationOptColumnEnum
    UOC_NONE = IDX_NONE
    UOC_C_KEY = 0
    UOC_P_KEY
    
    UOC_C_SYMBOL
    UOC_C_EXCHANGE
    UOC_C_SERIES
    UOC_C_EXPIRY
    UOC_C_STRIKE
    UOC_C_BID
    UOC_C_ASK
    UOC_C_LAST
    UOC_C_VOLUME
    UOC_C_QTY
    UOC_C_VOLA
    UOC_C_THEO_PRICE
    UOC_C_IV
    UOC_C_IV_BID
    UOC_C_IV_ASK
    UOC_C_OPEN_INTEREST
    UOC_C_DELTA
    UOC_C_GAMMA
    UOC_C_VEGA
    UOC_C_THETA
    UOC_C_RHO
    UOC_C_VEGA_DELTA
    UOC_C_VEGA_GAMMA
    UOC_C_THETA_DELTA
    UOC_C_THETA_GAMMA
    UOC_C_GAMMA_PER_THETA
    UOC_C_NET_CHANGE
    UOC_C_BID_SIZE
    UOC_C_ASK_SIZE
    UOC_C_TRADE_STATUS
    UOC_C_MM_BID_TICK_LEVEL
    UOC_C_MM_BID_QTY
    UOC_C_MM_BID_PREM
    UOC_C_MM_ASK_TICK_LEVEL
    UOC_C_MM_ASK_QTY
    UOC_C_MM_ASK_PREM
    
    UOC_P_SYMBOL
    UOC_P_EXCHANGE
    UOC_P_SERIES
    UOC_P_EXPIRY
    UOC_P_STRIKE
    UOC_P_BID
    UOC_P_ASK
    UOC_P_LAST
    UOC_P_VOLUME
    UOC_P_QTY
    UOC_P_VOLA
    UOC_P_THEO_PRICE
    UOC_P_IV
    UOC_P_IV_BID
    UOC_P_IV_ASK
    UOC_P_OPEN_INTEREST
    UOC_P_DELTA
    UOC_P_GAMMA
    UOC_P_VEGA
    UOC_P_THETA
    UOC_P_RHO
    UOC_P_VEGA_DELTA
    UOC_P_VEGA_GAMMA
    UOC_P_THETA_DELTA
    UOC_P_THETA_GAMMA
    UOC_P_GAMMA_PER_THETA
    UOC_P_NET_CHANGE
    UOC_P_BID_SIZE
    UOC_P_ASK_SIZE
    UOC_P_TRADE_STATUS
    UOC_P_MM_BID_TICK_LEVEL
    UOC_P_MM_BID_QTY
    UOC_P_MM_BID_PREM
    UOC_P_MM_ASK_TICK_LEVEL
    UOC_P_MM_ASK_QTY
    UOC_P_MM_ASK_PREM
    
    UOC_COLUMN_COUNT
    UOC_LAST_COLUMN = UOC_COLUMN_COUNT - 1
End Enum

Public Enum QuotationLogColumnEnum
    ULC_NONE = IDX_NONE
    
    ULC_TIME = 0
    ULC_MESSAGE
    
    ULC_COLUMN_COUNT
    ULC_LAST_COLUMN = ULC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Volatility Analysis                                                                                -
' ------------------------------------------------------------------------------------------------------

Public Enum VolaAnalysisFilterColumnEnum
    AFC_NONE = IDX_NONE
    
    AFC_GROUP
    AFC_VALUE
    AFC_TODAY
    AFC_EXPIRY
    AFC_VEGA_AMT
    AFC_MODEL
    
    AFC_COLUMN_COUNT
    AFC_LAST_COLUMN = AFC_COLUMN_COUNT - 1
End Enum

Public Enum VolaAnalysisUnderlyingColumnEnum
    AUC_NONE = IDX_NONE
    AUC_KEY = 0
    
    AUC_ROW
    AUC_SYMBOL
    AUC_BUY
    AUC_SELL
    AUC_UND_BID
    AUC_UND_ASK
    AUC_UND_LAST
    AUC_OPT_ROOT
    AUC_EXPIRY
    AUC_EXPIRY_DAYS
    AUC_ATM_STRIKE
    AUC_SUM_BID
    AUC_SUM_ASK
    AUC_IV_SUM_BID
    AUC_IV_SUM_ASK
    AUC_CONTRACTS_BID
    AUC_CONTRACTS_ASK
    AUC_CALC_1
    AUC_CALC_2
    AUC_LOAD_1
    AUC_LOAD_2
    AUC_VEGA_BID
    AUC_VEGA_ASK
    AUC_DELTA_BID
    AUC_SHARES_DELTA_BID
    AUC_DELTA_ASK
    AUC_SHARES_DELTA_ASK
    
    AUC_COLUMN_COUNT
    AUC_LAST_COLUMN = AUC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Greeks View                                                                                        -
' ------------------------------------------------------------------------------------------------------

Public Enum GreeksFilterColumnEnum
    GFC_NONE = IDX_NONE
    
    GFC_GROUP
    GFC_VALUE
    GFC_TYPE
    GFC_MODEL
    GFC_MODE
    
    GFC_COLUMN_COUNT
    GFC_LAST_COLUMN = GFC_COLUMN_COUNT - 1
End Enum

Public Enum GreeksUnderlyingColumnEnum
    GUC_NONE = IDX_NONE
    GUC_KEY = 0
    
    GUC_SYMBOL
    GUC_VALUE
    GUC_EXPIRY_ODD
    GUC_EXPIRY_EVEN
    GUC_SYM_TOTAL
    
    GUC_COLUMN_COUNT
    GUC_LAST_COLUMN = GUC_COLUMN_COUNT - 1
End Enum

' Rows <-> parameters of the Greeks view
Public Enum GreeksUnderlyingRowEnum
    GUR_NONE = IDX_NONE
    
    GUR_TIME_VALUE = 0
    GUR_DELTA_EQ
    GUR_DELTA_IN_SHARES
    GUR_GAMMA_EQ
    GUR_GAMMA_IN_SHARES
    GUR_VEGA
    GUR_NETDELTA_EQ
    GUR_NETDELTA_IN_SHARES
    
    GUR_ROW_COUNT
    GUR_LAST_ROW = GUR_ROW_COUNT - 1
End Enum

Public g_GUR_ROW_COUNT As Long
Public g_GUR_LAST_ROW As Long

' ------------------------------------------------------------------------------------------------------
' - Hedge Summary                                                                                      -
' ------------------------------------------------------------------------------------------------------

Public Enum HedgeSummaryFilterColumnEnum
    HFC_NONE = IDX_NONE
    
    HFC_GROUP
    HFC_VALUE
    HFC_TYPE
    HFC_WEIGHTED
    
    HFC_COLUMN_COUNT
    HFC_LAST_COLUMN = HFC_COLUMN_COUNT - 1
End Enum

Public Enum HedgeSummaryInstrumentsColumnEnum
    HIC_NONE = IDX_NONE
    
    HIC_USE
    HIC_SYMBOL
    HIC_TYPE
    
    HIC_COLUMN_COUNT
    HIC_LAST_COLUMN = HIC_COLUMN_COUNT - 1
End Enum

Public Enum HedgeSummaryResultsColumnEnum
    HRC_NONE = IDX_NONE
    
    HRC_SYMBOL
    HRC_POS
    HRC_CONTRACTS
    HRC_VEGAEQ
    
    HRC_COLUMN_COUNT
    HRC_LAST_COLUMN = HRC_COLUMN_COUNT - 1
End Enum

Public Enum HedgeSummaryTotalColumnEnum
    HTC_NONE = IDX_NONE
    
    HTC_NET
    HTC_GAIN
    
    HTC_COLUMN_COUNT
    HTC_LAST_COLUMN = HTC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Underlyings View                                                                                   -
' ------------------------------------------------------------------------------------------------------

Public Enum UnderlyingsFilterColumnEnum
    NFC_NONE = IDX_NONE
    
    NFC_TRADER
    NFC_GROUP
    NFC_SYMBOL
    NFC_TYPE
    NFC_ACTIVE
    NFC_OPT_STYLE
    NFC_CALENDAR
    NFC_UND_PROFILE
    NFC_OPT_PROFILE
    NFC_DIVS
    NFC_HAS_POS
    NFC_HAS_OPTIONS
    NFC_HAS_SYNTH       '  Uncomment if SU works correctly
    NFC_HTB
    NFC_HEDGE_SYMBOL
    NFC_DIRTY
    NFC_HAS_FUTURES
    
    NFC_COLUMN_COUNT
    NFC_LAST_COLUMN = NFC_COLUMN_COUNT - 1
End Enum

Public Enum UnderlyinsListColumnEnum
    NLC_NONE = IDX_NONE
    NLC_KEY = 0
    
    NLC_SYMBOL
    NLC_TYPE
    NLC_OPT_STYLE
    NLC_CALENDAR
    NLC_UND_PROFILE
    NLC_OPT_PROFILE
    NLC_DIVS
    NLC_DIV_FREQ
    NLC_DIV_DATE
    NLC_DIV_AMT
    NLC_YIELD
    NLC_SKEW
    NLC_KURT
    NLC_ACTIVE
    NLC_HTB
    NLC_HAS_POS
    NLC_HAS_OPTIONS
    NLC_HAS_SYNTH           '  Uncomment if SU works correctly
    NLC_SYMBOL_NAME
    NLC_DIRTY
    NLC_HAS_FUTURES
    NLC_FUT_ROOT
    NLC_FUT_UND
    NLC_FUT_MATURITY
    NLC_SOQ
    NLC_ALLOW_LENDING
    NLC_HEDGE_SYMBOL
    
    NLC_COLUMN_COUNT
    NLC_LAST_COLUMN = NLC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Reports View                                                                                       -
' ------------------------------------------------------------------------------------------------------

Public Enum ReportsFilterColumnEnum
    PFC_NONE = IDX_NONE
    
    PFC_REPORT
    PFC_UNDERLYING
    PFC_TYPE
    PFC_GROUP
    PFC_TRADER_GROUP
    PFC_TRADER
    PFC_STRATEGY
    PFC_MAX_STRIKE_DISTANCE
    PFC_COLUMN_COUNT
    PFC_LAST_COLUMN = PFC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Orders View                                                                                        -
' ------------------------------------------------------------------------------------------------------

Public Enum OrdersFilterColumnEnum
    OFC_NONE = IDX_NONE
    
    OFC_FIRST_COLUMN
    
    OFC_UNDERLYING = OFC_FIRST_COLUMN
    OFC_CONTRACT_TYPE
    OFC_ORDER_STATUS
    OFC_GROUP
    OFC_TRADER
    
    OFC_COLUMN_COUNT
    OFC_LAST_COLUMN = OFC_COLUMN_COUNT - 1
End Enum

Public Enum OrdersListColumnEnum
    OLC_NONE = IDX_NONE
    OLC_KEY = 0
    
    OLC_FIRST_COLUMN
    
    OLC_UNDERLYING = OLC_FIRST_COLUMN
    OLC_SYMBOL
    OLC_SERIES
    OLC_ORDER_ID
    OLC_BS
    OLC_ORDER_QTY
    OLC_ORDER_TYPE
    OLC_ORDER_PRICE
    OLC_ORDER_STATUS
    OLC_ORDER_DATE
    OLC_EXEC_ID
    OLC_EXEC_STATUS
    OLC_EXEC_PRICE
    OLC_EXEC_QTY
    OLC_EXEC_DATE
    OLC_DEST
    OLC_TRADER
    OLC_OPT_TYPE
    OLC_EXPIRY
    OLC_STRIKE
    OLC_LOTSIZE
    
    OLC_COLUMN_COUNT
    OLC_LAST_COLUMN = OLC_COLUMN_COUNT - 1
End Enum

Public Enum OrdersLogColumnEnum
    OEC_NONE = IDX_NONE
    
    OEC_FIRST_COLUMN
    
    OEC_NUM = OEC_FIRST_COLUMN
    OEC_TIME
    OEC_MESSAGE
    
    OEC_COLUMN_COUNT
    OEC_LAST_COLUMN = OEC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Trades Messages View                                                                               -
' ------------------------------------------------------------------------------------------------------

Public Enum TntCardsFilterColumnEnum
    CFC_NONE = IDX_NONE
    
    CFC_FIRST_COLUMN
    
    CFC_UNDERLYING = CFC_FIRST_COLUMN
    CFC_STATUS
    CFC_GROUP
    CFC_CNT_PTY
    
    CFC_COLUMN_COUNT
    CFC_LAST_COLUMN = CFC_COLUMN_COUNT - 1
End Enum

Public Enum TntCardsListColumnEnum
    CLC_NONE = IDX_NONE
    CLC_KEY = 0
    
    CLC_FIRST_COLUMN
    
    CLC_UNDERLYING = CLC_FIRST_COLUMN
    CLC_SYMBOL
    CLC_SERIES
    CLC_TRADE_ID
    CLC_BS
    CLC_QTY
    CLC_PRICE
    CLC_STATUS
    CLC_DATE
    CLC_ACCOUNT
    CLC_CNT_PTY
    CLC_OPT_TYPE
    CLC_EXPIRY
    CLC_STRIKE
    CLC_LOTSIZE
    CLC_TOT_PRICE
    CLC_MESSAGE
    
    CLC_COLUMN_COUNT
    CLC_LAST_COLUMN = CLC_COLUMN_COUNT - 1
End Enum

Public Enum TntCardsLogColumnEnum
    CEC_NONE = IDX_NONE
    
    CEC_FIRST_COLUMN
    
    CEC_NUM = CEC_FIRST_COLUMN
    CEC_TIME
    CEC_MESSAGE
    
    CEC_COLUMN_COUNT
    CEC_LAST_COLUMN = CEC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Stock Hedge View                                                                                   -
' ------------------------------------------------------------------------------------------------------

Public Enum StockHedgeFilterColumnEnum
    SFC_NONE = IDX_NONE
    
    SFC_TRADER
    SFC_STRATEGY
    SFC_UNDERLYING
    
    SFC_COLUMN_COUNT
    SFC_LAST_COLUMN = SFC_COLUMN_COUNT - 1
End Enum

Public Enum StockHedgeTotalColumnEnum
    STC_NONE = IDX_NONE
    
    STC_SLICE
    STC_LOT_SIZE
    STC_MAX_SIZE
    STC_NET_DELTA
    STC_HEDGE_POS
    
    STC_COLUMN_COUNT
    STC_LAST_COLUMN = STC_COLUMN_COUNT - 1
End Enum

Public Enum StockHedgePosColumnEnum
    SPC_NONE = IDX_NONE
    SPC_KEY = 0
    
    SPC_TRADER
    SPC_STRATEGY
    SPC_SYMBOL
    SPC_NET_DELTA
    SPC_HEDGE_POS
    
    SPC_COLUMN_COUNT
    SPC_LAST_COLUMN = SPC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Index Hedge View                                                                                   -
' ------------------------------------------------------------------------------------------------------

Public Enum IndexHedge1FilterColumnEnum
    IFC_NONE = IDX_NONE
    
    IFC_TRADER
    IFC_STRATEGY
    IFC_INDEX
    
    IFC_COLUMN_COUNT
    IFC_LAST_COLUMN = IFC_COLUMN_COUNT - 1
End Enum

Public Enum IndexHedge1TotalColumnEnum
    ITC_NONE = IDX_NONE
    
    ITC_INDEX
    ITC_SLICE
    ITC_LOT_SIZE
    ITC_MAX_SIZE
    ITC_CONTRACTS
    ITC_DELTA
    ITC_NET_DELTA
    ITC_PRICE
    ITC_EXPOSURE
    
    ITC_COLUMN_COUNT
    ITC_LAST_COLUMN = ITC_COLUMN_COUNT - 1
End Enum

Public Enum IndexHedge1UndColumnEnum
    IUC_NONE = IDX_NONE
    IUC_KEY = 0
    
    IUC_SYMBOL
    IUC_WEIGHT
    IUC_PRICE
    IUC_IDX_SHARES
    IUC_CUR_SHARES
    IUC_TXN_SHARES
    
    IUC_COLUMN_COUNT
    IUC_LAST_COLUMN = IUC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Two Indices Hedge View                                                                             -
' ------------------------------------------------------------------------------------------------------

Public Enum IndexHedge2FilterColumnEnum
    JFC_NONE = IDX_NONE
    
    JFC_TRADER
    JFC_STRATEGY
    JFC_INDEX1
    JFC_INDEX2
    
    JFC_COLUMN_COUNT
    JFC_LAST_COLUMN = JFC_COLUMN_COUNT - 1
End Enum

Public Enum IndexHedge2TotalColumnEnum
    JTC_NONE = IDX_NONE
    
    JTC_INDEX
    JTC_SLICE
    JTC_LOT_SIZE
    JTC_MAX_SIZE
    JTC_CONTRACTS
    JTC_DELTA
    JTC_NET_DELTA
    JTC_PRICE
    JTC_EXPOSURE
    
    JTC_COLUMN_COUNT
    JTC_LAST_COLUMN = JTC_COLUMN_COUNT - 1
End Enum

Public Enum IndexHedge2UndColumnEnum
    JUC_NONE = IDX_NONE
    JUC_KEY = 0
    
    JUC_SYMBOL
    JUC_WEIGHT
    JUC_PRICE
    JUC_IDX_SHARES
    
    JUC_COLUMN_COUNT
    JUC_LAST_COLUMN = JUC_COLUMN_COUNT - 1
End Enum

Public Enum IndexHedge2ResColumnEnum
    JRC_NONE = IDX_NONE
    JRC_KEY = 0
    
    JRC_SYMBOL
    JRC_IDX1_SHARES
    JRC_IDX2_SHARES
    JRC_TXN_SHARES
    JRC_CUR_SHARES
    JRC_TRANSACT
   
    JRC_COLUMN_COUNT
    JRC_LAST_COLUMN = JRC_COLUMN_COUNT - 1
End Enum

' ------------------------------------------------------------------------------------------------------
' - Common                                                                                             -
' ------------------------------------------------------------------------------------------------------

Public Enum GridTypeEnum
    GT_FIRST = 0
    
    GT_QUOTES_UNDERLYING = GT_FIRST
    GT_QUOTES_DIVIDENDS
    GT_QUOTES_VOLA
    GT_QUOTES_OPTIONS
    
    GT_TRADES_FILTER
    GT_TRADES_LIST
    
    GT_RISKS_FILTER
    GT_RISKS_TOTALS
    GT_RISKS_POSITIONS
    
    GT_MATRIX_FILTER
    GT_MATRIX_SCENARIO
    GT_MATRIX_VALUES
    
    GT_VOLA_FILTER
    GT_VOLA_UNDERLYING

    GT_QUOTATION_UNDERLYING
    GT_QUOTATION_DIVIDENDS
    GT_QUOTATION_OPTIONS
    GT_QUOTATION_LOG
    
    GT_VOLAANALYSIS_FILTER
    GT_VOLAANALYSIS_UNDERLYING
    
    GT_GREEKS_FILTER
    GT_GREEKS_UNDERLYING
    GT_GREEKS_UND_ROWS
    
    GT_HEDGESUMMARY_FILTER
    GT_HEDGESUMMARY_INSTRUMENTS
    GT_HEDGESUMMARY_RESULTS
    GT_HEDGESUMMARY_TOTAL
    
    GT_UNDERLYINGS_FILTER
    GT_UNDERLYINGS_LIST
    
    GT_REPORTS_FILTER
    GT_REPORTS_RESULTS
    
    GT_ORDERS_FILTER
    GT_ORDERS_LIST
    GT_ORDERS_LOG
    
    GT_TNTCARDS_FILTER
    GT_TNTCARDS_LIST
    GT_TNTCARDS_LOG
    
    GT_STOCKHEDGE_FILTER
    GT_STOCKHEDGE_TOTALS
    GT_STOCKHEDGE_POSITIONS
    
    GT_INDEXHEDGE1_FILTER
    GT_INDEXHEDGE1_TOTALS
    GT_INDEXHEDGE1_UNDERLYINGS
    
    GT_INDEXHEDGE2_FILTER
    GT_INDEXHEDGE2_TOTALS_1
    GT_INDEXHEDGE2_TOTALS_2
    GT_INDEXHEDGE2_UNDERLYINGS_1
    GT_INDEXHEDGE2_UNDERLYINGS_2
    GT_INDEXHEDGE2_RESULTS
    
    GT_COUNT
    GT_LAST = GT_COUNT - 1
End Enum

Global g_DefGrids(GT_FIRST To GT_LAST) As clsGridDef

Public Sub InitDefGrids()
    On Error Resume Next
    
    InitQuotesGrids
    InitTradesGrids
    'InitRisksGrids
    InitRiskMatrixGrids
    InitVolaGrids
    InitQuotationGrids
    InitVolaAnalysisGrids
    InitGreeksGrids
    InitHedgeSummaryGrids
    InitUnderlyingsGrids
    InitReportsGrids
    InitOrdersGrids
    InitTntCardsGrids
    InitStockHedgeGrids
    InitIndexHedge1Grids
    InitIndexHedge2Grids
    
    LoadDefaults
End Sub

Public Sub ClearDefGrids()
    On Error Resume Next
    Erase g_DefGrids
End Sub

Private Sub LoadDefaults()
    On Error Resume Next
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, i&, sFileName$
    
    For i = GT_FIRST To GT_LAST
        sFileName = GetDefaultSettingsFileName(i)
        sPath = MakeCommonFilePath("EtsSettings", sFileName, False)
        aStorage.Init sPath
        aStorage.ReadData
        If Err.Number = 0 Then
            If aStorage.GetStringValue("Type", "Value") = GetDefaultSettingsTypeName(i) Then
                g_DefGrids(i).ReadFromStorage "Grid", aStorage, (i = GT_QUOTES_UNDERLYING _
                                                Or i = GT_QUOTES_OPTIONS Or i = GT_TRADES_LIST _
                                                Or i = GT_RISKS_TOTALS Or i = GT_RISKS_POSITIONS _
                                                Or i = GT_VOLA_UNDERLYING Or i = GT_QUOTATION_UNDERLYING _
                                                Or i = GT_QUOTATION_OPTIONS Or i = GT_VOLAANALYSIS_UNDERLYING _
                                                Or i = GT_UNDERLYINGS_LIST Or i = GT_ORDERS_LIST _
                                                Or i = GT_TNTCARDS_LIST)
            End If
        End If
    Next
End Sub

Public Sub LoadRisksDefaults()
    On Error Resume Next
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, sFileName$
    
    sFileName = GetDefaultSettingsFileName(GT_RISKS_TOTALS)
    sPath = MakeCommonFilePath("EtsSettings", sFileName, False)
    aStorage.Init sPath
    aStorage.ReadData
    If Err.Number = 0 Then
        If aStorage.GetStringValue("Type", "Value") = GetDefaultSettingsTypeName(GT_RISKS_TOTALS) Then
            g_DefGrids(GT_RISKS_TOTALS).ReadFromStorage "Grid", aStorage, True
        End If
    End If
    
    sFileName = GetDefaultSettingsFileName(GT_RISKS_POSITIONS)
    sPath = MakeCommonFilePath("EtsSettings", sFileName, False)
    aStorage.Init sPath
    aStorage.ReadData
    If Err.Number = 0 Then
        If aStorage.GetStringValue("Type", "Value") = GetDefaultSettingsTypeName(GT_RISKS_POSITIONS) Then
            g_DefGrids(GT_RISKS_POSITIONS).ReadFromStorage "Grid", aStorage, True
        End If
    End If

End Sub

Public Sub LoadGreekRowsDefaults()
    On Error Resume Next
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, sFileName$
    
    sFileName = GetDefaultSettingsFileName(GT_GREEKS_UND_ROWS)
    sPath = MakeCommonFilePath("EtsSettings", sFileName, False)
    aStorage.Init sPath
    aStorage.ReadData
    If Err.Number = 0 Then
        If aStorage.GetStringValue("Type", "Value") = GetDefaultSettingsTypeName(GT_GREEKS_UND_ROWS) Then
            g_DefGrids(GT_GREEKS_UND_ROWS).ReadFromStorage "Grid", aStorage
        End If
    End If

End Sub

Public Sub SaveGridDefAsDefault(ByRef gdGrid As clsGridDef, ByVal enGridType As GridTypeEnum)
'    On Error GoTo EH
    Dim aStorage As New clsSettingsStorage
    Dim sPath$, sFileName$
    
    sFileName = GetDefaultSettingsFileName(enGridType)
    
    sPath = MakeCommonFilePath("EtsSettings", sFileName, True)
    aStorage.Init sPath
    
    aStorage.SetStringValue "Type", "Value", GetDefaultSettingsTypeName(enGridType)
    gdGrid.WriteToStorage "Grid", aStorage, (enGridType = GT_QUOTES_UNDERLYING _
                                                Or enGridType = GT_QUOTES_OPTIONS Or enGridType = GT_TRADES_LIST _
                                                Or enGridType = GT_RISKS_TOTALS Or enGridType = GT_RISKS_POSITIONS _
                                                Or enGridType = GT_VOLA_UNDERLYING Or enGridType = GT_QUOTATION_UNDERLYING _
                                                Or enGridType = GT_QUOTATION_OPTIONS Or enGridType = GT_VOLAANALYSIS_UNDERLYING _
                                                Or enGridType = GT_UNDERLYINGS_LIST Or enGridType = GT_ORDERS_LIST _
                                                Or enGridType = GT_TNTCARDS_LIST Or enGridType = GT_GREEKS_UND_ROWS)

    aStorage.SaveData
    gdGrid.CopyTo g_DefGrids(enGridType)
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox "Fail to set this configuration as default for new window."
End Sub

Private Sub InitQuotesGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_QUOTES_UNDERLYING) = Nothing
    Set g_DefGrids(GT_QUOTES_DIVIDENDS) = Nothing
    Set g_DefGrids(GT_QUOTES_VOLA) = Nothing
    Set g_DefGrids(GT_QUOTES_OPTIONS) = Nothing
    
    Set g_DefGrids(GT_QUOTES_UNDERLYING) = New clsGridDef
    Set g_DefGrids(GT_QUOTES_DIVIDENDS) = New clsGridDef
    Set g_DefGrids(GT_QUOTES_VOLA) = New clsGridDef
    Set g_DefGrids(GT_QUOTES_OPTIONS) = New clsGridDef
    
    ' quotes underlying grid
    With g_DefGrids(GT_QUOTES_UNDERLYING)
        .Init QUC_SYMBOL, QUC_LAST_COLUMN
        .IsRowSelection = False
        
        .Idx(0) = QUC_SYMBOL
        .Idx(1) = QUC_EXCHANGE
        .Idx(2) = QUC_BID
        .Idx(3) = QUC_ASK
        .Idx(4) = QUC_LAST
        
        .Col(QUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(QUC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, True, False, True, False
        .Col(QUC_EXCHANGE).Init "Exch", "Exchange", "", flexDTString, False, True, True, True
        .Col(QUC_BID).Init "Bid", "Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QUC_ASK).Init "Ask", "Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QUC_LAST).Init "Last", "Last Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(QUC_HIGH).Init "Hi", "High Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QUC_LOW).Init "Lo", "Low Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QUC_OPEN).Init "Open", "Open Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QUC_CLOSE).Init "Close", "Close Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QUC_VOLUME).Init "Vol", "Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(QUC_POS).Init "UndPos", "Underlying Position", "#,##0", flexDTLong, False, True, False, True
        .Col(QUC_NET_CHANGE).Init "Chg", "Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QUC_BID_SIZE).Init "BidSz", "Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QUC_ASK_SIZE).Init "AskSz", "Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QUC_UPDATE_TIME).Init "UpdTm", "Last Update Time", "Long Time", flexDTDate, False, True, False, True
        .Col(QUC_BA_EXCHANGE).Init "B/A Exch", "Bid/Ask Exchange", "", flexDTString, False, True, True, True
        
        .Col(QUC_NET_DELTA).Init "NetDlt", "Net Delta", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_OPT_DELTA).Init "OptDlt", "Opiton Delta", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_GAMMA_USD).Init "Gma$1", "Gamma 1$", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_VEGA).Init "Vga", "Vega", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_THETA).Init "Tht", "Theta", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_RHO).Init "Rho", "Rho", "#,##0", flexDTDouble, False, True, False, True
        .Col(QUC_SYMBOL_NAME).Init "Name", "Symbol Name", "", flexDTString, False, True, False, True
        .Col(QUC_DPC).Init "DPC", "Delivery Per Contract", "#,##0", flexDTLong, False, True, False, True
        .Col(QUC_FUT_ROOT).Init "FutRoot", "Futures Root Symbol", "", flexDTString, False, True, False, True
        .Col(QUC_FUT_QTY).Init "FutQty", "Futures Contracts Quantity", "#,##0", flexDTLong, False, True, False, True
    
        .Col(QUC_BID).ForeColorAlt1 = &H8000&
        .Col(QUC_BID).ForeColorAlt2 = &HC0&
        .Col(QUC_ASK).ForeColorAlt1 = &H8000&
        .Col(QUC_ASK).ForeColorAlt2 = &HC0&
    End With
    
    ' quotes dividends/rates grid
    With g_DefGrids(GT_QUOTES_DIVIDENDS)
        .Init QDC_TRADES, QDC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To QDC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(QDC_TRADES).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(QDC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
        .Col(QDC_STYLE).Init "Style", "Options Style", "", flexDTString, True, False, True, False
        .Col(QDC_CALENDAR).Init "ExpCal", "Options Expiration Calendar", "", flexDTString, False, False, True, False
        .Col(QDC_UNDPROFILE).Init "UndProfile", "Underlyings Price Profile", "", flexDTString, True, False, True, False
        .Col(QDC_OPTPROFILE).Init "OptProfile", "Options Price Profile", "", flexDTString, True, False, True, False
        .Col(QDC_DIV).Init "Divs", "Dividends Type", "", flexDTString, True, False, True, False
        .Col(QDC_FREQ).Init "Freq", "Dividends Frequency", "", flexDTString, True, False, True, False
        .Col(QDC_DATE).Init "Date", "Last Dividends Date", "MM/DD/YYYY", flexDTDate, True, False, True, False
        .Col(QDC_AMT).Init "Amt", "Dividends Amount", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(QDC_RATE).Init "Rates", "Interest Rates Type", "", flexDTString, True, False, True, False
        .Col(QDC_FLT_TRADER).Init "Trader", "Trader Name", "", flexDTString, True, False, True, False
        .Col(QDC_FLT_STRATEGY).Init "Strategy", "Strategy Name", "", flexDTString, True, False, True, False
        .Col(QDC_RATE_VAL).Init "Rate", "Interest Rate Value", "#,##0.00", flexDTDouble, True, False, True, False

    End With
    
    ' quotes vola grid
    With g_DefGrids(GT_QUOTES_VOLA)
        .Init QVC_VOLA, QVC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To QVC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(QVC_VOLA).Init "Exp", "Expiry", "", flexDTString, False, False, True, False
        .Col(QVC_VOLA_VAL).Init "Vola", "ATM Volatility", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(QVC_VOLA_DATECALC).Init "Calculation Date", "Calculation Date", "Short Date", flexDTDate, True, False, True, True
        .Col(QVC_VOLA_SIM).Init "Vol Simulation", "Vol Simulation", "", flexDTBoolean, True, False, True, True
        .Col(QVC_VOLA_FLAT).Init "Flat Vol", "Flat Vol", "", flexDTBoolean, True, False, True, True
                
    End With
    
    ' quotes options grid
    With g_DefGrids(GT_QUOTES_OPTIONS)
        .Init QOC_C_SYMBOL, QOC_LAST_COLUMN
        .IsRowSelection = True
        
        .Idx(0) = QOC_C_SERIES
        .Idx(1) = QOC_C_EXCHANGE
        .Idx(2) = QOC_C_BID
        .Idx(3) = QOC_C_ASK
        .Idx(4) = QOC_C_LAST
        .Idx(5) = QOC_C_VOLA
        .Idx(6) = QOC_C_THEO_PRICE
        .Idx(7) = QOC_P_SERIES
        .Idx(8) = QOC_P_EXCHANGE
        .Idx(9) = QOC_P_BID
        .Idx(10) = QOC_P_ASK
        .Idx(11) = QOC_P_LAST
        .Idx(12) = QOC_P_VOLA
        .Idx(13) = QOC_P_THEO_PRICE
        
        .Col(QOC_C_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        .Col(QOC_P_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(QOC_C_SYMBOL).Init "cSym", "Call Symbol", "", flexDTString, False, True, False, True
        .Col(QOC_C_EXCHANGE).Init "cExch", "Call Exchange", "", flexDTString, False, True, False, True
        .Col(QOC_C_SERIES).Init "cSeries", "Call Series", "", flexDTString, False, True, True, True
        .Col(QOC_C_EXPIRY).Init "cExpiry", "Call Expiry", "MMM YY", flexDTDate, False, True, False, True
        .Col(QOC_C_STRIKE).Init "cStrike", "Call Strike", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_BID).Init "cBid", "Call Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QOC_C_ASK).Init "cAsk", "Call Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QOC_C_LAST).Init "cLast", "Call Last Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(QOC_C_VOLUME).Init "cVolu", "Call Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_C_QTY).Init "cQty", "Call Contracts Quantity", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_C_VOLA).Init "cVol", "Call Volatility", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(QOC_C_THEO_PRICE).Init "cThv", "Call Theoretical Price", "#,##0.00", flexDTDouble, False, True, True, True, "Theo < Bid", "Theo > Ask"
        .Col(QOC_C_IV).Init "cIVol", "Call Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_IV_BID).Init "cIV-b", "Call Bid Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_IV_ASK).Init "cIV-a", "Call Ask Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_OPEN_INTEREST).Init "cOI", "Call Open Interest", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_C_DELTA).Init "cDlt%", "Call Delta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_GAMMA).Init "cGma%", "Call Gamma %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_VEGA).Init "cVga%", "Call Vega %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_THETA).Init "cTht%", "Call Theta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_RHO).Init "cRho%", "Call Rho %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_VEGA_DELTA).Init "cVgaDlt", "Call Vega Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_VEGA_GAMMA).Init "cVgaGma", "Call Vega Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_THETA_DELTA).Init "cThtDlt", "Call Theta Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_THETA_GAMMA).Init "cThtGma", "Call Theta Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_GAMMA_PER_THETA).Init "cGma/Tht", "Call Gamma Per Theta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_NET_CHANGE).Init "cChg", "Call Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_BID_SIZE).Init "cBidSz", "Call Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_C_ASK_SIZE).Init "cAskSz", "Call Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_C_IV_SPREAD).Init "cIV-s", "Call Implied Volatility Spread", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_C_BA_EXCHANGE).Init "cB/A Exch", "Call Bid/Ask Exchange", "", flexDTString, False, True, True, True
        .Col(QOC_C_UPDATE_TIME).Init "cUpdTm", "Call Last Update Time", "Long Time", flexDTDate, False, True, False, True
        .Col(QOC_C_VOLGA).Init "cVolga", "Call Volga", "#,##0.00", flexDTDouble, False, True, False, True
    
        .Col(QOC_P_SYMBOL).Init "pSym", "Put Symbol", "", flexDTString, False, True, False, True
        .Col(QOC_P_EXCHANGE).Init "pExch", "Put Exchange", "", flexDTString, False, True, False, True
        .Col(QOC_P_SERIES).Init "pSeries", "Put Series", "", flexDTString, False, True, True, True
        .Col(QOC_P_EXPIRY).Init "pExpiry", "Put Expiry", "MMM YY", flexDTDate, False, True, False, True
        .Col(QOC_P_STRIKE).Init "pStrike", "Put Strike", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_BID).Init "pBid", "Put Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QOC_P_ASK).Init "pAsk", "Put Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(QOC_P_LAST).Init "pLast", "Put Last Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(QOC_P_VOLUME).Init "pVolu", "Put Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_P_QTY).Init "pQty", "Put Contracts Quantity", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_P_VOLA).Init "pVol", "Put Volatility", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(QOC_P_THEO_PRICE).Init "pThv", "Put Theoretical Price", "#,##0.00", flexDTDouble, False, True, True, True, "Theo < Bid", "Theo > Ask"
        .Col(QOC_P_IV).Init "pIVol", "Put Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_IV_BID).Init "pIV-b", "Put Bid Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_IV_ASK).Init "pIV-a", "Put Ask Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_OPEN_INTEREST).Init "pOI", "Put Open Interest", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_P_DELTA).Init "pDlt%", "Put Delta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_GAMMA).Init "pGma%", "Put Gamma %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_VEGA).Init "pVga%", "Put Vega %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_THETA).Init "pTht%", "Put Theta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_RHO).Init "pRho%", "Put Rho %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_VEGA_DELTA).Init "pVgaDlt", "Put Vega Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_VEGA_GAMMA).Init "pVgaGma", "Put Vega Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_THETA_DELTA).Init "pThtDlt", "Put Theta Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_THETA_GAMMA).Init "pThtGma", "Put Theta Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_GAMMA_PER_THETA).Init "pGma/Tht", "Put Gamma Per Theta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_NET_CHANGE).Init "pChg", "Put Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_BID_SIZE).Init "pBidSz", "Put Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_P_ASK_SIZE).Init "pAskSz", "Put Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(QOC_P_IV_SPREAD).Init "pIV-s", "Put Implied Volatility Spread", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(QOC_P_BA_EXCHANGE).Init "pB/A Exch", "Put Bid/Ask Exchange", "", flexDTString, False, True, True, True
        .Col(QOC_P_UPDATE_TIME).Init "pUpdTm", "Put Last Update Time", "Long Time", flexDTDate, False, True, False, True
        .Col(QOC_P_VOLGA).Init "pVolga", "Put Volga", "#,##0.00", flexDTDouble, False, True, False, True
    
        .Col(QOC_ROOT).Init "Root", "Option Pair Root/Futures", "", flexDTString, False, True, False, True
        .Col(QOC_IS_SYNTH).Init "Synth", "Synthetic Underlying", "", flexDTBoolean, False, True, False, True
        .Col(QOC_DPC).Init "DPC", "Delivery Per Contract", "", flexDTString, False, True, False, True
        .Col(QOC_SU_PRICE).Init "SU Price", "Synthetic Underlying Price", "#,##0.00", flexDTDouble, False, True, True, True
        
        .Col(QOC_DTE).Init "DTE", "DTE", "#,##0", flexDTLong, False, True, False, True
        
        .Col(QOC_C_BID).ForeColorAlt1 = &H8000&
        .Col(QOC_C_BID).ForeColorAlt2 = &HC0&
        .Col(QOC_C_ASK).ForeColorAlt1 = &H8000&
        .Col(QOC_C_ASK).ForeColorAlt2 = &HC0&
        .Col(QOC_C_THEO_PRICE).ForeColorAlt1 = &HC0&
        .Col(QOC_C_THEO_PRICE).ForeColorAlt2 = &H8000&
    
        .Col(QOC_P_BID).ForeColorAlt1 = &H8000&
        .Col(QOC_P_BID).ForeColorAlt2 = &HC0&
        .Col(QOC_P_ASK).ForeColorAlt1 = &H8000&
        .Col(QOC_P_ASK).ForeColorAlt2 = &HC0&
        .Col(QOC_P_THEO_PRICE).ForeColorAlt1 = &HC0&
        .Col(QOC_P_THEO_PRICE).ForeColorAlt2 = &H8000&
    End With
End Sub

Private Sub InitTradesGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_TRADES_FILTER) = Nothing
    Set g_DefGrids(GT_TRADES_LIST) = Nothing
    
    Set g_DefGrids(GT_TRADES_FILTER) = New clsGridDef
    Set g_DefGrids(GT_TRADES_LIST) = New clsGridDef
    
    ' trades filter grid
    With g_DefGrids(GT_TRADES_FILTER)
        .Init TFC_UNDERLYING, TFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To TFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(TFC_UNDERLYING).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
        .Col(TFC_TYPE).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(TFC_GROUP).Init "Group", "Contracts Group", "", flexDTString, True, False, True, False
        .Col(TFC_TRADER_GROUP).Init "TraderGroup", "Trader Group", "", flexDTString, True, False, True, False
        .Col(TFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(TFC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, True, False, True, False
        .Col(TFC_POSTYPE).Init "Type", "Type", "", flexDTString, True, False, True, False
    End With

    ' trades list grid
    With g_DefGrids(GT_TRADES_LIST)
        .Init TLC_TYPE, TLC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = TLC_TYPE To TLC_COMMISSION
            .Idx(i - TLC_TYPE) = i
        Next
        
        .Col(TLC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(TLC_TYPE).Init "Type", "Trade Type", "", flexDTString, False, True, True, True
        .Col(TLC_UNDERLYING).Init "Und", "Underlying Symbol", "", flexDTString, False, True, True, True
        .Col(TLC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, True, True, True
        .Col(TLC_OPT_TYPE).Init "C/P", "Option Type", "", flexDTString, False, True, True, True
        .Col(TLC_EXPIRY).Init "Exp", "Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(TLC_DTE).Init "DTE", "DTE", "#,##0", flexDTLong, False, True, True, True
        .Col(TLC_STRIKE).Init "Str", "Option Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(TLC_BS).Init "B/S", "Buy/Sell", "Buy;Sell", flexDTBoolean, False, True, True, True
        .Col(TLC_PRICE).Init "Prc", "Trade Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(TLC_QTY).Init "Qty", "Trade Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(TLC_LOTSIZE).Init "Lot", "Trade Lot Size", "#,##0", flexDTLong, False, True, True, True
        .Col(TLC_TOT_PRICE).Init "TotPrc", "Trade Total Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(TLC_DATE).Init "Date", "Trade Date", "MM/DD/YYYY HH:MM:SS AM/PM", flexDTDate, False, True, True, True
        .Col(TLC_TRADER).Init "Trader", "Trader", "", flexDTString, False, True, True, True
        .Col(TLC_STRATEGY).Init "Strategy", "Strategy", "#,##0.00", flexDTString, False, True, True, True
        .Col(TLC_BROKER).Init "Broker", "Broker", "#,##0.00", flexDTString, False, True, True, True
        .Col(TLC_CLR_BROKER).Init "ClrBroker", "Clearing Broker", "#,##0", flexDTString, False, True, True, True
        .Col(TLC_COMMISSION).Init "Commission", "Total Commission", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(TLC_MATCHED).Init "Match", "Matched Trade", "", flexDTString, False, True, False, True
        .Col(TLC_MARKED).Init "Mark", "Marked Trade", "Yes;No", flexDTBoolean, False, True, False, True
        .Col(TLC_FUT_ROOT).Init "FutRoot", "Futures Root Symbol", "", flexDTString, False, True, True, True
        .Col(TLC_FUTURES).Init "Fut", "Futures Symbol", "", flexDTString, False, True, True, True
        .Col(TLC_FUT_MATURITY).Init "FutMat", "Futures Maturity", "MMMYY", flexDTDate, False, True, True, True
        .Col(TLC_STOCKREFPRICE).Init "RefPrice", "Stock Reference Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(TLC_TRADED_IV).Init "TrIV%", "Traded IV%", "#,####0.0000", flexDTDouble, False, True, True, True
        .Col(TLC_TRADETYPE).Init "Pos", "Position", "", flexDTString, False, True, True, True
    End With
End Sub

Private Sub InitOrdersGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_ORDERS_FILTER) = Nothing
    Set g_DefGrids(GT_ORDERS_LIST) = Nothing
    Set g_DefGrids(GT_ORDERS_LOG) = Nothing
    
    Set g_DefGrids(GT_ORDERS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_ORDERS_LIST) = New clsGridDef
    Set g_DefGrids(GT_ORDERS_LOG) = New clsGridDef
    
    ' orders filter grid
    With g_DefGrids(GT_ORDERS_FILTER)
        .Init OFC_FIRST_COLUMN, OFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = OFC_FIRST_COLUMN To OFC_LAST_COLUMN
            .Idx(i - OFC_FIRST_COLUMN) = i
        Next
        
        .Col(OFC_UNDERLYING).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
        .Col(OFC_CONTRACT_TYPE).Init "Contract", "Contract Type", "", flexDTString, True, False, True, False
        .Col(OFC_ORDER_STATUS).Init "Status", "Orders Status", "", flexDTString, True, False, True, False
        .Col(OFC_GROUP).Init "Group", "Contracts Group", "", flexDTString, True, False, True, False
        .Col(OFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
    End With

    ' orders list grid
    With g_DefGrids(GT_ORDERS_LIST)
        .Init OLC_FIRST_COLUMN, OLC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = OLC_FIRST_COLUMN To OLC_LAST_COLUMN
            .Idx(i - OLC_FIRST_COLUMN) = i
        Next
        
        .Col(OLC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(OLC_UNDERLYING).Init "Und", "Underlying Symbol", "", flexDTString, False, True, True, True
        .Col(OLC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, True, True, True
        .Col(OLC_SERIES).Init "Series", "Option Series", "", flexDTString, False, True, True, True
        .Col(OLC_ORDER_ID).Init "Order", "Order ID", "", flexDTString, False, True, True, True
        .Col(OLC_BS).Init "B/S", "Buy/Sell", "", flexDTString, False, True, True, True
        .Col(OLC_ORDER_QTY).Init "Qty", "Order Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(OLC_ORDER_TYPE).Init "Type", "Order Type", "", flexDTString, False, True, True, True
        .Col(OLC_ORDER_PRICE).Init "Prc", "Order Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(OLC_ORDER_STATUS).Init "Status", "Order Status", "", flexDTString, False, True, True, True
        .Col(OLC_ORDER_DATE).Init "Date", "Order Date", "", flexDTDate, False, True, True, True
        .Col(OLC_EXEC_ID).Init "Exec", "Execution ID", "", flexDTString, False, True, True, True
        .Col(OLC_EXEC_STATUS).Init "ExStatus", "Execution Status", "", flexDTString, False, True, True, True
        .Col(OLC_EXEC_PRICE).Init "Avg/ExPrc", "Average/Execution Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(OLC_EXEC_QTY).Init "Cum/ExQty", "Cumulative/Execution Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(OLC_EXEC_DATE).Init "ExDate", "Execution Date", "", flexDTDate, False, True, True, True
        .Col(OLC_DEST).Init "Dest", "Order Destination", "", flexDTString, False, True, True, True
        .Col(OLC_TRADER).Init "Trader", "Trader", "", flexDTString, False, True, True, True
        .Col(OLC_OPT_TYPE).Init "C/P", "Option Type", "", flexDTString, False, True, True, True
        .Col(OLC_EXPIRY).Init "Exp", "Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(OLC_STRIKE).Init "Str", "Option Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(OLC_LOTSIZE).Init "Lot", "Order Lot Size", "#,##0", flexDTLong, False, True, True, True
    End With

    ' orders log grid
    With g_DefGrids(GT_ORDERS_LOG)
        .Init OEC_TIME, OEC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To OEC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(OEC_NUM).Init "#", "Event Number", "", flexDTLong, False, False, True, False
        .Col(OEC_TIME).Init "Time", "Event Time", "", flexDTDate, False, False, True, False
        .Col(OEC_MESSAGE).Init "Message", "Event Message", "", flexDTString, False, False, True, False
    End With
End Sub

Private Sub InitTntCardsGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_TNTCARDS_FILTER) = Nothing
    Set g_DefGrids(GT_TNTCARDS_LIST) = Nothing
    Set g_DefGrids(GT_TNTCARDS_LOG) = Nothing
    
    Set g_DefGrids(GT_TNTCARDS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_TNTCARDS_LIST) = New clsGridDef
    Set g_DefGrids(GT_TNTCARDS_LOG) = New clsGridDef
    
    ' cards filter grid
    With g_DefGrids(GT_TNTCARDS_FILTER)
        .Init CFC_FIRST_COLUMN, CFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = CFC_FIRST_COLUMN To CFC_LAST_COLUMN
            .Idx(i - CFC_FIRST_COLUMN) = i
        Next
        
        .Col(CFC_UNDERLYING).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
        .Col(CFC_STATUS).Init "Status", "Trade Message Status", "", flexDTString, True, False, True, False
        .Col(CFC_GROUP).Init "Group", "Contracts Group", "", flexDTString, True, False, True, False
        .Col(CFC_CNT_PTY).Init "CntPty", "CounterParty", "", flexDTString, True, False, True, False
    End With

    ' cards list grid
    With g_DefGrids(GT_TNTCARDS_LIST)
        .Init CLC_FIRST_COLUMN, CLC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = CLC_FIRST_COLUMN To CLC_LAST_COLUMN
            .Idx(i - CLC_FIRST_COLUMN) = i
        Next
        
        .Col(CLC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(CLC_UNDERLYING).Init "Und", "Underlying Symbol", "", flexDTString, False, True, True, True
        .Col(CLC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, True, True, True
        .Col(CLC_SERIES).Init "Series", "Option Series", "", flexDTString, False, True, True, True
        .Col(CLC_TRADE_ID).Init "Trade", "Trade ID", "", flexDTString, False, True, True, True
        .Col(CLC_BS).Init "B/S", "Buy/Sell", "Buy;Sell", flexDTBoolean, False, True, True, True
        .Col(CLC_QTY).Init "Qty", "Trade Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(CLC_PRICE).Init "Prc", "Trade Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(CLC_STATUS).Init "Status", "Trades Status", "", flexDTString, False, True, True, True
        .Col(CLC_DATE).Init "Date", "Trade Date", "", flexDTDate, False, True, True, True
        .Col(CLC_ACCOUNT).Init "Acct", "Trader Account", "", flexDTString, False, True, True, True
        .Col(CLC_CNT_PTY).Init "CntPty", "CounterParty", "", flexDTString, False, True, True, True
        .Col(CLC_OPT_TYPE).Init "C/P", "Option Type", "C;P", flexDTBoolean, False, True, True, True
        .Col(CLC_EXPIRY).Init "Exp", "Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(CLC_STRIKE).Init "Str", "Option Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(CLC_LOTSIZE).Init "Lot", "Trade Lot Size", "#,##0", flexDTLong, False, True, True, True
        .Col(CLC_TOT_PRICE).Init "TotPrc", "Total Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(CLC_MESSAGE).Init "Msg", "Message", "", flexDTString, False, True, True, True
    End With

    ' cards log grid
    With g_DefGrids(GT_TNTCARDS_LOG)
        .Init CEC_TIME, CEC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To CEC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(CEC_NUM).Init "#", "Event Number", "", flexDTLong, False, False, True, False
        .Col(CEC_TIME).Init "Time", "Event Time", "", flexDTDate, False, False, True, False
        .Col(CEC_MESSAGE).Init "Message", "Event Message", "", flexDTString, False, False, True, False
    End With
End Sub

Public Function AddRisksColumn(Info As CVLib.cvInfo, GridType As GridTypeEnum) As Long
    On Error Resume Next
    
    Select Case GridType
        Case GT_RISKS_POSITIONS
            g_RPC_LAST_COLUMN = g_RPC_LAST_COLUMN + 1
            g_RPC_COLUMN_COUNT = g_RPC_COLUMN_COUNT + 1
    
            g_DefGrids(GT_RISKS_POSITIONS).Idx(g_RPC_LAST_COLUMN - RPC_SYMBOL) = g_RPC_LAST_COLUMN
            g_DefGrids(GT_RISKS_POSITIONS).Col(g_RPC_LAST_COLUMN).Init Info.Caption, Info.Name, _
            "#,##0.00", IIf(Info.Type = cvNumeric, flexDTDouble, flexDTString), False, True, True, True, "", "", IIf(Info.Type = cvNumeric, True, False)
    
            AddRisksColumn = g_RPC_LAST_COLUMN
            Exit Function
        Case GT_RISKS_TOTALS
            g_RTC_LAST_COLUMN = g_RTC_LAST_COLUMN + 1
            g_RTC_COLUMN_COUNT = g_RTC_COLUMN_COUNT + 1
    
            g_DefGrids(GT_RISKS_TOTALS).Idx(g_RTC_LAST_COLUMN) = g_RTC_LAST_COLUMN
            g_DefGrids(GT_RISKS_TOTALS).Col(g_RTC_LAST_COLUMN).Init Info.Caption, Info.Name, _
            "#,##0.00", IIf(Info.Type = cvNumeric, flexDTDouble, flexDTString), False, True, True, True, "", "", IIf(Info.Type = cvNumeric, True, False)
    
            AddRisksColumn = g_RTC_LAST_COLUMN
            Exit Function
    End Select
    
End Function

Public Sub InitRisksGrids(Optional ByVal cvGrpCount As Long = 0, Optional ByVal cvOptCount As Long = 0)
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_RISKS_FILTER) = Nothing
    Set g_DefGrids(GT_RISKS_TOTALS) = Nothing
    Set g_DefGrids(GT_RISKS_POSITIONS) = Nothing
    
    Set g_DefGrids(GT_RISKS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_RISKS_TOTALS) = New clsGridDef
    Set g_DefGrids(GT_RISKS_POSITIONS) = New clsGridDef
    
    ' risks filter grid
    With g_DefGrids(GT_RISKS_FILTER)
        .Init RFC_GROUP, RFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To RFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(RFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(RFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(RFC_TYPE).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(RFC_EXPIRY).Init "Expiry", "Options Expiry", "", flexDTString, True, False, True, False
        .Col(RFC_INDEX).Init "Index", "Index for Calculations", "", flexDTString, True, False, True, False
        .Col(RFC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
        .Col(RFC_SIM).Init "Simulation", "Simulation Mode", "", flexDTBoolean, True, False, True, False
        .Col(RFC_SIM_DATE).Init "Simulation Date", "Simulation Date", "", flexDTDate, False, False, False, False
        .Col(RFC_SIM_VOLA).Init "VolaShift%", "Volatility shift", "", flexDTLong, True, False, False, False
        .Col(RFC_SIM_SPOT).Init "SpotShift%", "Spot price shift", "", flexDTLong, True, False, False, False
        .Col(RFC_SIM_VOLA_SHIFT_TYPE).Init "VolaShiftType", "Volatility Shift Type", "", flexDTString, True, False, False, False
        .Col(RFC_SIM_TYPE).Init "Simulation Type", "Simulation Type", "", flexDTString, True, False, False, False
        .Col(RFC_SHIFT_ID).Init "Shift #", "Shift #", "", flexDTLong, True, False, False, False
        .Col(RFC_SHIFT_FILE).Init "Shift File", "Shift File Name", "", flexDTString, True, False, False, False
    End With
    
    ' risks total grid
    With g_DefGrids(GT_RISKS_TOTALS)
        .Init RTC_PNL_MTM, RTC_LAST_COLUMN + cvGrpCount
        .IsRowSelection = False
        
        For i = 0 To RTC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(RTC_PNL_MTM).Init "P&L MTM", "Mark to Market P&L", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_PNL_THEO).Init "P&L Theo", "Theoretical P&L", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_PNL_EDGE).Init "P&L Edge", "P&L Edge", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_VEGA).Init "Vga", "Vega $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_WTD_VEGA).Init "WtdVga", "Weighted Vega", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_THETA).Init "Tht", "Theta $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_DELTA_EQ).Init "NetDlt$", "Delta $ Equivalent", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_GAMMA_EQ).Init "NetGma$", "Gamma $ Equivalent", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_RHO).Init "Rho", "Rho $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_INDEX_DELTA_EQ).Init "IdxDltEq", "Index Equivalent Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_BETA_WTD_DELTA).Init "BetaWtdDlt", "Beta Weighted Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_OPT_DELTA).Init "OptDlt$", "Option Delta $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_NET_DELTA).Init "NetDlt", "Underlyings NetDlt Sum", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_NET_GAMMA).Init "NetGma$1", "Underlyings Gma$1 Sum", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RTC_BETA_WTD_DELTA_USD).Init "BtWtdDlt$", "Beta Weighted Delta $ Equivalent", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
    
        For i = RTC_PNL_MTM To RTC_LAST_COLUMN
            .Col(i).ForeColorAlt1 = &HC0&
        Next
        
        g_RTC_LAST_COLUMN = RTC_LAST_COLUMN
        g_RTC_COLUMN_COUNT = RTC_COLUMN_COUNT
    End With
    
    ' risks list grid
    With g_DefGrids(GT_RISKS_POSITIONS)
        .Init RPC_SYMBOL, RPC_LAST_COLUMN + cvOptCount
        .IsRowSelection = True
        
        For i = RPC_SYMBOL To RPC_LAST_COLUMN
            .Idx(i - RPC_SYMBOL) = i
        Next
    
        .Col(RPC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(RPC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, False, True, False
        .Col(RPC_OPT_TYPE).Init "C/P", "Option Type", "", flexDTString, False, True, True, True
        .Col(RPC_EXPIRY).Init "Exp", "Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(RPC_STRIKE).Init "Str", "Option Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(RPC_BID).Init "Bid", "Bid Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(RPC_ASK).Init "Ask", "Ask Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(RPC_LAST).Init "Last", "Last Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(RPC_UND_POS).Init "UndPos", "Underlying Position", "#,##0", flexDTLong, False, True, True, True
        .Col(RPC_OPT_QTY).Init "OptQty", "Option Contracts Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(RPC_PNL_MTM).Init "P&L MTM", "Mark to Market P&L", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_PNL_THEO).Init "P&L Theo", "Theoretical P&L", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_PNL_EDGE).Init "P&L Edge", "P&L Edge", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_NET_DELTA).Init "NetDlt", "Net Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_OPT_DELTA).Init "OptDlt", "Option Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_GAMMA).Init "Gma1%", "Gamma 1%", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_NET_GAMMA).Init "NetGma$", "Net Gamma $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_GAMMA_SHARES).Init "Gma$1", "Gamma $1", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_VEGA).Init "Vga", "Vega $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_WTD_VEGA).Init "WtdVga", "Weighted Vega", "#,##0.0000", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_THETA).Init "Tht", "Theta $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_RHO).Init "Rho", "Rho $", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_VEGA_DELTA).Init "VgaDlt", "Vega Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_VEGA_GAMMA).Init "VgaGma", "Vega Gamma", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_THETA_DELTA).Init "ThtDlt", "Theta Delta", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_THETA_GAMMA).Init "ThtGma", "Theta Gamma", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_UND).Init "Und", "Underlying Symbol", "", flexDTString, False, True, True, True
        .Col(RPC_DEL_UNIT).Init "DU", "Deliverable Units", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(RPC_TIME_VALUE).Init "TmVal", "Option Time Value", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_NET_DELTA_USD).Init "NetDlt$", "Net Delta $ Equivalent", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_BETA_WTD_DELTA_USD).Init "BtWtdDlt$", "Beta Weighted Delta $ Equivalent", "#,##0.00", flexDTDouble, False, True, True, True, "Some Items N/A", "", True
        .Col(RPC_CLOSE).Init "Close", "Close Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(RPC_FUT_ROOT).Init "FutRoot", "Futures Root Symbol", "", flexDTString, False, True, True, True
        .Col(RPC_FUTURES).Init "Fut", "Futures Symbol", "", flexDTString, False, True, True, True
        .Col(RPC_FUT_MATURITY).Init "FutMat", "Futures Maturity", "MMMYY", flexDTDate, False, True, True, True
        .Col(RPC_FUT_QTY).Init "FutQty", "Futures Contracts Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(RPC_IMPORT_ID).Init "ImpId", "Import Id", "", flexDTString, False, True, True, True
        .Col(RPC_NETCHANGE).Init "Chg", "Net Change", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(RPC_DELTAP).Init "dlt%", "Delta %", "#,##0.00", flexDTDouble, False, True, False, True, "", "", True
        .Col(RPC_GAMMAP).Init "gma%", "Gamma %", "#,##0.00", flexDTDouble, False, True, False, True, "", "", True
        .Col(RPC_VEGAP).Init "vga%", "Vega %", "#,##0.00", flexDTDouble, False, True, False, True, "", "", True
        .Col(RPC_THETAP).Init "tht%", "Theta %", "#,##0.00", flexDTDouble, False, True, False, True, "", "", True
        .Col(RPC_RHOP).Init "rho%", "Rho %", "#,##0.00", flexDTDouble, False, True, False, True, "", "", True
        .Col(RPC_DTE).Init "DTE", "DTE", "#,##0", flexDTLong, False, True, True, True, "", "", True
        .Col(RPC_THEO_VOL).Init "TheoVol", "Theo Volatility", "#,##0", flexDTDouble, True, True, True, True, "", "", True
        .Col(RPC_MIV).Init "MIV", "MIV", "#,##0", flexDTDouble, False, True, True, True, "", "", True
        .Col(RPC_VOLGA).Init "Volga", "Volga", "#,##0", flexDTDouble, False, True, True, True, "", "", True
        
        For i = RPC_PNL_MTM To RPC_BETA_WTD_DELTA_USD
            .Col(i).ForeColorAlt1 = &HC0&
        Next
        
        g_RPC_LAST_COLUMN = RPC_LAST_COLUMN
        g_RPC_COLUMN_COUNT = RPC_COLUMN_COUNT
    End With
End Sub

Private Sub InitRiskMatrixGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_MATRIX_FILTER) = Nothing
    Set g_DefGrids(GT_MATRIX_SCENARIO) = Nothing
    Set g_DefGrids(GT_MATRIX_VALUES) = Nothing
    
    Set g_DefGrids(GT_MATRIX_FILTER) = New clsGridDef
    Set g_DefGrids(GT_MATRIX_SCENARIO) = New clsGridDef
    Set g_DefGrids(GT_MATRIX_VALUES) = New clsGridDef

    ' matrix filter grid
    With g_DefGrids(GT_MATRIX_FILTER)
        .Init MFC_GROUP, MFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To MFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(MFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(MFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(MFC_TYPE).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(MFC_SHIFT).Init "Shift", "Shift Type", "", flexDTString, True, False, True, False
        .Col(MFC_EXPIRY).Init "Expiry", "Options Expiry", "", flexDTString, True, False, True, False
        .Col(MFC_INDEX).Init "Index", "Index for Calculations", "", flexDTString, True, False, True, False
        .Col(MFC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, True, False, True, False
        
        .Col(MFC_PNL).Init "P&L", "Theo P&L", "", flexDTBoolean, True, False, True, False
        .Col(MFC_DELTA).Init "NetDlt$", "Net Delta $ Equivalent", "", flexDTBoolean, True, False, True, False
        .Col(MFC_NET_DELTA).Init "NetDlt", "Net Delta", "", flexDTBoolean, True, False, True, False
        .Col(MFC_GAMMA).Init "NetGma$", "Net Gamma $ Equivalent", "", flexDTBoolean, True, False, True, False
        .Col(MFC_GAMMA_PERC).Init "Gma1%", "Gamma 1%", "", flexDTBoolean, True, False, True, False
        .Col(MFC_NET_GAMMA).Init "NetGma$1", "Net Gamma Per 1$", "", flexDTBoolean, True, False, True, False
        .Col(MFC_VEGA).Init "Vga", "Vega", "", flexDTBoolean, True, False, True, False
        .Col(MFC_THETA).Init "Tht", "Theta", "", flexDTBoolean, True, False, True, False
        .Col(MFC_RHO).Init "Rho", "Rho", "", flexDTBoolean, True, False, True, False
        .Col(MFC_WTD_VEGA).Init "WtdVga", "Weighted Vega", "", flexDTBoolean, True, False, True, False
    End With
    
    ' matrix scenario grid
    With g_DefGrids(GT_MATRIX_SCENARIO)
        .Init MSC_NAME, MSC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To MSC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(MSC_NAME).Init "Scenario", "Scenario Name", "", flexDTString, True, False, True, False
        .Col(MSC_HORZ_AXIS).Init "hAxis", "Horizontal Axis Type", "", flexDTString, True, False, True, False
        .Col(MSC_HORZ_POINTS).Init "hPts", "Horizontal Points Count", "", flexDTString, True, False, True, False
        .Col(MSC_HORZ_STEP).Init "hStep", "Horizontal Step Value", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(MSC_HORZ_UNITS).Init "hUnits", "Horizontal Units Type", "", flexDTString, True, False, True, False
        .Col(MSC_VERT_AXIS).Init "vAxis", "Vertical Axis Type", "", flexDTString, True, False, True, False
        .Col(MSC_VERT_POINTS).Init "vPts", "Vertical Points Count", "", flexDTString, True, False, True, False
        .Col(MSC_VERT_STEP).Init "vStep", "Vertical Step Value", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(MSC_VERT_UNITS).Init "vUnits", "Vertical Units Type", "", flexDTString, True, False, True, False
        .Col(MSC_VOLA_SHIFT).Init "VolShift", "Volatility Shift Type", "", flexDTString, True, False, True, False
    End With
    
    ' matrix values grid
    With g_DefGrids(GT_MATRIX_VALUES)
        .Init MVC_VALUE, MSC_LAST_COLUMN
        .IsRowSelection = False
        
        .Idx(0) = MVC_VALUE
        .Idx(1) = MVC_BASE
        
        .Col(MVC_VALUE).Init "Values", "Matrix Values", "#,##0.00", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(MVC_BASE).Init "Base", "Matrix Base Lines", "#,##0.00", flexDTDouble, False, False, True, False, "Some Items N/A"
    
        .Col(MVC_BASE).BackColor = &H80000018
    
        .Col(MVC_VALUE).ForeColorAlt1 = &HC0&
        .Col(MVC_BASE).ForeColorAlt1 = &HC0&
    End With
End Sub

Private Sub InitVolaGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_VOLA_FILTER) = Nothing
    Set g_DefGrids(GT_VOLA_UNDERLYING) = Nothing
    
    Set g_DefGrids(GT_VOLA_FILTER) = New clsGridDef
    Set g_DefGrids(GT_VOLA_UNDERLYING) = New clsGridDef
    
    ' vola monitor filter grid
    With g_DefGrids(GT_VOLA_FILTER)
        .Init VFC_GROUP, VFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To VFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(VFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(VFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(VFC_EXPIRY_1).Init "Mth1", "First Option Expiry Month", "", flexDTString, True, False, True, False
        .Col(VFC_EXPIRY_2).Init "Mth2", "Second Option Expiry Month", "", flexDTString, True, False, True, False
        .Col(VFC_VOLA_1).Init "Vola1", "First Month Volatility", "", flexDTString, True, False, True, False
        .Col(VFC_VOLA_2).Init "Vola2", "Second Month Volatility", "", flexDTString, True, False, True, False
        .Col(VFC_VEGA).Init "Vega", "Vega Type", "", flexDTString, True, False, True, False
        .Col(VFC_GAMMA).Init "Gamma", "Gamma Type", "", flexDTString, True, False, True, False
        .Col(VFC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
    End With

    ' vola monitor underlying grid
    With g_DefGrids(GT_VOLA_UNDERLYING)
        .Init VUC_SYMBOL, VUC_LAST_COLUMN
        .IsRowSelection = True
    
        For i = VUC_SYMBOL To VUC_LAST_COLUMN
            .Idx(i - VUC_SYMBOL) = i
        Next
        
        .Col(VUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(VUC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, False, True, False
        .Col(VUC_BID).Init "Bid", "Bid Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(VUC_ASK).Init "Ask", "Ask Price", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(VUC_LAST).Init "Last", "Last Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_EXPIRY_1).Init "Exp1", "1st Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(VUC_EXPIRY_2).Init "Exp2", "2nd Option Expiry", "MMMYY", flexDTDate, False, True, True, True
        .Col(VUC_STRIKE_1).Init "Str1", "1st Expiry ATM Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_STRIKE_2).Init "Str2", "2nd Expiry ATM Strike", "#,##0.00", flexDTDouble, False, True, True, True
    
        .Col(VUC_IV_1).Init "IVol1", "1st Expiry Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_IV_2).Init "IVol2", "2nd Expiry Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
    
        .Col(VUC_VOLA_1).Init "Vol1", "1st Expiry Theo Volatility", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_VOLA_2).Init "Vol2", "2nd Expiry Theo Volatility", "#,##0.00", flexDTDouble, False, True, True, True
    
        '.Col(VUC_TARG_1).Init "TVol1", "1st Expiry Target Volatility", "#,##0.00", flexDTDouble, False, True, True, True
        '.Col(VUC_TARG_2).Init "TVol2", "2nd Expiry Target Volatility", "#,##0.00", flexDTDouble, False, True, True, True
    
        .Col(VUC_VOLA_IV_1).Init "Vol1-IVol1", "1st Expiry Theo - Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_VOLA_IV_2).Init "Vol2-IVol2", "2nd Expiry Theo - Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
    
        '.Col(VUC_TARG_IV_1).Init "TVol1-IVol1", "1st Expiry Target - Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
        '.Col(VUC_TARG_IV_2).Init "TVol2-IVol2", "2nd Expiry Target - Implied Volatility", "#,##0.00", flexDTDouble, False, True, True, True
    
        .Col(VUC_POS).Init "Pos", "Position", "#,##0", flexDTLong, False, True, True, True
    
        .Col(VUC_NET_DELTA).Init "NetDlt", "Net Delta", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_OPT_DELTA).Init "OptDlt", "Options Delta", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_GAMMA).Init "Gma$1", "Gamma 1$", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_VEGA).Init "Vga", "Vega", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_THETA).Init "Tht", "Theta", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(VUC_RHO).Init "Rho", "Rho", "#,##0.00", flexDTDouble, False, True, True, True
    End With
End Sub

Private Sub InitQuotationGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_QUOTATION_UNDERLYING) = Nothing
    Set g_DefGrids(GT_QUOTATION_DIVIDENDS) = Nothing
    Set g_DefGrids(GT_QUOTATION_OPTIONS) = Nothing
    Set g_DefGrids(GT_QUOTATION_LOG) = Nothing
    
    Set g_DefGrids(GT_QUOTATION_UNDERLYING) = New clsGridDef
    Set g_DefGrids(GT_QUOTATION_DIVIDENDS) = New clsGridDef
    Set g_DefGrids(GT_QUOTATION_OPTIONS) = New clsGridDef
    Set g_DefGrids(GT_QUOTATION_LOG) = New clsGridDef
    
    ' quotation underlying grid
    With g_DefGrids(GT_QUOTATION_UNDERLYING)
        .Init UUC_SYMBOL, UUC_LAST_COLUMN
        .IsRowSelection = False
        
        .Idx(0) = UUC_SYMBOL
        .Idx(1) = UUC_STATUS
        .Idx(2) = UUC_BID
        .Idx(3) = UUC_ASK
        .Idx(4) = UUC_TRADER_ROLE
        .Idx(5) = UUC_BIN_NUM
        
        .Col(UUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(UUC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, True, False, True, False
        .Col(UUC_EXCHANGE).Init "Exch", "Exchange", "", flexDTString, False, True, False, True
        .Col(UUC_BID).Init "Bid", "Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UUC_ASK).Init "Ask", "Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UUC_LAST).Init "Last", "Last Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(UUC_HIGH).Init "Hi", "High Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UUC_LOW).Init "Lo", "Low Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UUC_OPEN).Init "Open", "Open Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UUC_CLOSE).Init "Close", "Close Price", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UUC_VOLUME).Init "Vol", "Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(UUC_POS).Init "Pos", "Position", "#,##0", flexDTLong, False, True, False, True
        .Col(UUC_NET_CHANGE).Init "Chg", "Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UUC_BID_SIZE).Init "BidSz", "Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UUC_ASK_SIZE).Init "AskSz", "Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UUC_BIN_NUM).Init "Bin#", "Bin Number", "#,##0", flexDTLong, False, True, True, True
        .Col(UUC_TRADER_ROLE).Init "TrRole", "Trader Role", "", flexDTString, False, True, True, True
        .Col(UUC_STATUS).Init "Stat", "Trading Status", "", flexDTString, False, True, True, True
    
        .Col(UUC_BID).ForeColorAlt1 = &H8000&
        .Col(UUC_BID).ForeColorAlt2 = &HC0&
        .Col(UUC_ASK).ForeColorAlt1 = &H8000&
        .Col(UUC_ASK).ForeColorAlt2 = &HC0&
    End With
    
    ' quotation dividends/rates grid
    With g_DefGrids(GT_QUOTATION_DIVIDENDS)
        .Init UDC_MODEL, UDC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To UDC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(UDC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
        .Col(UDC_STYLE).Init "Style", "Options Style", "", flexDTString, True, False, True, False
        .Col(UDC_CALENDAR).Init "ExpCal", "Options Expiration Calendar", "", flexDTString, False, False, True, False
        .Col(UDC_DIV).Init "Divs", "Dividends Type", "", flexDTString, True, False, True, False
        .Col(UDC_FREQ).Init "Freq", "Dividends Frequency", "", flexDTString, True, False, True, False
        .Col(UDC_DATE).Init "Date", "Last Dividends Date", "MM/DD/YYYY", flexDTDate, True, False, True, False
        .Col(UDC_AMT).Init "Amt", "Dividends Amount", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(UDC_RATE).Init "Rates", "Interest Rates Type", "", flexDTString, True, False, True, False
        .Col(UDC_RATE_VAL).Init "Rate", "Interest Rate Value", "#,##0.00", flexDTDouble, True, False, True, False
    End With
    
    ' quotation options grid
    With g_DefGrids(GT_QUOTATION_OPTIONS)
        .Init UOC_C_SYMBOL, UOC_LAST_COLUMN
        .IsRowSelection = True
        
        .Idx(0) = UOC_C_SERIES
        .Idx(1) = UOC_C_TRADE_STATUS
        .Idx(2) = UOC_C_VOLA
        .Idx(3) = UOC_C_THEO_PRICE
        .Idx(4) = UOC_C_MM_BID_TICK_LEVEL
        .Idx(5) = UOC_C_MM_BID_QTY
        .Idx(6) = UOC_C_MM_BID_PREM
        .Idx(7) = UOC_C_BID_SIZE
        .Idx(8) = UOC_C_BID
        .Idx(9) = UOC_C_ASK
        .Idx(10) = UOC_C_ASK_SIZE
        .Idx(11) = UOC_C_MM_ASK_PREM
        .Idx(12) = UOC_C_MM_ASK_QTY
        .Idx(13) = UOC_C_MM_ASK_TICK_LEVEL
        
        .Idx(14) = UOC_P_SERIES
        .Idx(15) = UOC_P_TRADE_STATUS
        .Idx(16) = UOC_P_VOLA
        .Idx(17) = UOC_P_THEO_PRICE
        .Idx(18) = UOC_P_MM_BID_TICK_LEVEL
        .Idx(19) = UOC_P_MM_BID_QTY
        .Idx(20) = UOC_P_MM_BID_PREM
        .Idx(21) = UOC_P_BID_SIZE
        .Idx(22) = UOC_P_BID
        .Idx(23) = UOC_P_ASK
        .Idx(24) = UOC_P_ASK_SIZE
        .Idx(25) = UOC_P_MM_ASK_PREM
        .Idx(26) = UOC_P_MM_ASK_QTY
        .Idx(27) = UOC_P_MM_ASK_TICK_LEVEL
        
        .Col(UOC_C_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        .Col(UOC_P_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(UOC_C_SYMBOL).Init "cSym", "Call Symbol", "", flexDTString, False, True, False, True
        .Col(UOC_C_EXCHANGE).Init "cExch", "Call Exchange", "", flexDTString, False, True, False, True
        .Col(UOC_C_SERIES).Init "cSeries", "Call Series", "", flexDTString, False, True, True, True
        .Col(UOC_C_EXPIRY).Init "cExpiry", "Call Expiry", "MMM YY", flexDTDate, False, True, False, True
        .Col(UOC_C_STRIKE).Init "cStrike", "Call Strike", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_BID).Init "cBid", "Call Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_C_ASK).Init "cAsk", "Call Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_C_LAST).Init "cLast", "Call Last Price", "#,##0.00", flexDTDouble, True, True, False, True
        .Col(UOC_C_VOLUME).Init "cVolu", "Call Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_C_QTY).Init "cQty", "Call Contracts Uuantity", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_C_VOLA).Init "cVol", "Call Volatility", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(UOC_C_THEO_PRICE).Init "cThv", "Call Theoretical Price", "#,##0.00", flexDTDouble, False, True, True, True, "Theo < Bid", "Theo > Ask"
        .Col(UOC_C_IV).Init "cIVol", "Call Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_IV_BID).Init "cIV-b", "Call Bid Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_IV_ASK).Init "cIV-a", "Call Ask Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_OPEN_INTEREST).Init "cOI", "Call Open Interest", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_C_DELTA).Init "cDlt%", "Call Delta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_GAMMA).Init "cGma%", "Call Gamma %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_VEGA).Init "cVga%", "Call Vega %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_THETA).Init "cTht%", "Call Theta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_RHO).Init "cRho%", "Call Rho %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_VEGA_DELTA).Init "cVgaDlt", "Call Vega Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_VEGA_GAMMA).Init "cVgaGma", "Call Vega Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_THETA_DELTA).Init "cThtDlt", "Call Theta Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_THETA_GAMMA).Init "cThtGma", "Call Theta Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_GAMMA_PER_THETA).Init "cGma/Tht", "Call Gamma Per Theta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_NET_CHANGE).Init "cChg", "Call Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_C_BID_SIZE).Init "cBidSz", "Call Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_C_ASK_SIZE).Init "cAskSz", "Call Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_C_TRADE_STATUS).Init "cStat", "Call Trading Status", "", flexDTString, False, True, True, True
        .Col(UOC_C_MM_BID_TICK_LEVEL).Init "cBidLev", "Call Bid Tick Worse Level", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_C_MM_BID_QTY).Init "cMmBidQty", "Call MM Bid Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_C_MM_BID_PREM).Init "cMmBidPrem", "Call MM Bid Premium", "#,##0.00", flexDTDouble, False, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_C_MM_ASK_TICK_LEVEL).Init "cAskLev", "Call Ask Tick Worse Level", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_C_MM_ASK_QTY).Init "cMmAskQty", "Call MM Ask Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_C_MM_ASK_PREM).Init "cMmAskPrem", "Call MM Ask Premium", "#,##0.00", flexDTDouble, False, True, True, True, "Up Tick", "Down Tick"
    
        .Col(UOC_P_SYMBOL).Init "pSym", "Put Symbol", "", flexDTString, False, True, False, True
        .Col(UOC_P_EXCHANGE).Init "pExch", "Put Exchange", "", flexDTString, False, True, False, True
        .Col(UOC_P_SERIES).Init "pSeries", "Put Series", "", flexDTString, False, True, True, True
        .Col(UOC_P_EXPIRY).Init "pExpiry", "Put Expiry", "MMM YY", flexDTDate, False, True, False, True
        .Col(UOC_P_STRIKE).Init "pStrike", "Put Strike", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_BID).Init "pBid", "Put Bid Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_P_ASK).Init "pAsk", "Put Ask Price", "#,##0.00", flexDTDouble, True, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_P_LAST).Init "pLast", "Put Last Price", "#,##0.00", flexDTDouble, True, True, False, True
        .Col(UOC_P_VOLUME).Init "pVolu", "Put Volume", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_P_QTY).Init "pQty", "Put Contracts Uuantity", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_P_VOLA).Init "pVol", "Put Volatility", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(UOC_P_THEO_PRICE).Init "pThv", "Put Theoretical Price", "#,##0.00", flexDTDouble, False, True, True, True, "Theo < Bid", "Theo > Ask"
        .Col(UOC_P_IV).Init "pIVol", "Put Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_IV_BID).Init "pIV-b", "Put Bid Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_IV_ASK).Init "pIV-a", "Put Ask Implied Volatility", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_OPEN_INTEREST).Init "pOI", "Put Open Interest", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_P_DELTA).Init "pDlt%", "Put Delta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_GAMMA).Init "pGma%", "Put Gamma %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_VEGA).Init "pVga%", "Put Vega %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_THETA).Init "pTht%", "Put Theta %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_RHO).Init "pRho%", "Put Rho %", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_VEGA_DELTA).Init "pVgaDlt", "Put Vega Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_VEGA_GAMMA).Init "pVgaGma", "Put Vega Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_THETA_DELTA).Init "pThtDlt", "Put Theta Delta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_THETA_GAMMA).Init "pThtGma", "Put Theta Gamma", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_GAMMA_PER_THETA).Init "pGma/Tht", "Put Gamma Per Theta", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_NET_CHANGE).Init "pChg", "Put Net Change", "#,##0.00", flexDTDouble, False, True, False, True
        .Col(UOC_P_BID_SIZE).Init "pBidSz", "Put Bid Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_P_ASK_SIZE).Init "pAskSz", "Put Ask Size", "#,##0", flexDTLong, False, True, False, True
        .Col(UOC_P_TRADE_STATUS).Init "pStat", "Put Trading Status", "", flexDTString, False, True, True, True
        .Col(UOC_P_MM_BID_TICK_LEVEL).Init "pBidLev", "Put Bid Tick Worse Level", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_P_MM_BID_QTY).Init "pMmBidQty", "Put MM Bid Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_P_MM_BID_PREM).Init "pMmBidPrem", "Put MM Bid Premium", "#,##0.00", flexDTDouble, False, True, True, True, "Up Tick", "Down Tick"
        .Col(UOC_P_MM_ASK_TICK_LEVEL).Init "pAskLev", "Put Ask Tick Worse Level", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_P_MM_ASK_QTY).Init "pMmAskQty", "Put MM Ask Quantity", "#,##0", flexDTLong, False, True, True, True
        .Col(UOC_P_MM_ASK_PREM).Init "pMmAskPrem", "Put MM Ask Premium", "#,##0.00", flexDTDouble, False, True, True, True, "Up Tick", "Down Tick"
    
        .Col(UOC_C_BID).ForeColorAlt1 = &H8000&
        .Col(UOC_C_BID).ForeColorAlt2 = &HC0&
        .Col(UOC_C_ASK).ForeColorAlt1 = &H8000&
        .Col(UOC_C_ASK).ForeColorAlt2 = &HC0&
        .Col(UOC_C_THEO_PRICE).ForeColorAlt1 = &HC0&
        .Col(UOC_C_THEO_PRICE).ForeColorAlt2 = &H8000&
        .Col(UOC_C_MM_BID_PREM).ForeColorAlt1 = &H8000&
        .Col(UOC_C_MM_BID_PREM).ForeColorAlt2 = &HC0&
        .Col(UOC_C_MM_ASK_PREM).ForeColorAlt1 = &H8000&
        .Col(UOC_C_MM_ASK_PREM).ForeColorAlt2 = &HC0&
    
        .Col(UOC_P_BID).ForeColorAlt1 = &H8000&
        .Col(UOC_P_BID).ForeColorAlt2 = &HC0&
        .Col(UOC_P_ASK).ForeColorAlt1 = &H8000&
        .Col(UOC_P_ASK).ForeColorAlt2 = &HC0&
        .Col(UOC_P_THEO_PRICE).ForeColorAlt1 = &HC0&
        .Col(UOC_P_THEO_PRICE).ForeColorAlt2 = &H8000&
        .Col(UOC_P_MM_BID_PREM).ForeColorAlt1 = &H8000&
        .Col(UOC_P_MM_BID_PREM).ForeColorAlt2 = &HC0&
        .Col(UOC_P_MM_ASK_PREM).ForeColorAlt1 = &H8000&
        .Col(UOC_P_MM_ASK_PREM).ForeColorAlt2 = &HC0&
    End With

    ' quotation log grid
    With g_DefGrids(GT_QUOTATION_LOG)
        .Init ULC_TIME, ULC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To ULC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(ULC_TIME).Init "Time", "Event Time", "", flexDTDate, False, False, True, False
        .Col(ULC_MESSAGE).Init "Message", "Event Message", "", flexDTString, False, False, True, False
    End With
End Sub

Private Sub InitVolaAnalysisGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_VOLAANALYSIS_FILTER) = Nothing
    Set g_DefGrids(GT_VOLAANALYSIS_UNDERLYING) = Nothing
    
    Set g_DefGrids(GT_VOLAANALYSIS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_VOLAANALYSIS_UNDERLYING) = New clsGridDef
    
    ' vola monitor filter grid
    With g_DefGrids(GT_VOLAANALYSIS_FILTER)
        .Init AFC_GROUP, AFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To AFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(AFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(AFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(AFC_TODAY).Init "Today", "Today Date", "", flexDTDate, False, False, True, False
        .Col(AFC_EXPIRY).Init "Expiry", "Expiry Month", "", flexDTString, True, False, True, False
        .Col(AFC_VEGA_AMT).Init "VgaAmt", "Vega Amount", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(AFC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
    End With

    ' vola monitor underlying grid
    With g_DefGrids(GT_VOLAANALYSIS_UNDERLYING)
        .Init AUC_ROW, AUC_LAST_COLUMN
        .IsRowSelection = True
    
        For i = AUC_ROW To AUC_LAST_COLUMN
            .Idx(i - AUC_ROW) = i
        Next
        
        .Col(AUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(AUC_ROW).Init "#", "Row Number", "", flexDTLong, False, False, True, False
        .Col(AUC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, False, True, False
        .Col(AUC_BUY).Init "Buy", "Buy Options", "", flexDTBoolean, True, False, True, False
        .Col(AUC_SELL).Init "Sell", "Sell Options", "", flexDTBoolean, True, False, True, False
        .Col(AUC_UND_BID).Init "Bid", "Underlying Bid Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_UND_ASK).Init "Ask", "Underlying Ask Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_UND_LAST).Init "Last", "Underlying Last Price", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_EXPIRY).Init "Exp", "Expiry Date", "", flexDTDate, False, True, True, True
        .Col(AUC_EXPIRY_DAYS).Init "DTE", "Days to Expiry", "#,##0", flexDTLong, False, True, True, True
        .Col(AUC_ATM_STRIKE).Init "AtmStr", "ATM Strike", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_OPT_ROOT).Init "Root", "Option Root", "", flexDTString, True, True, True, True, "Multiple Roots"
        .Col(AUC_SUM_BID).Init "sBid", "Straddle Bid (Call Bid + Put Bid)", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_SUM_ASK).Init "sAsk", "Straddle Ask (Call Ask + Put Ask)", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_IV_SUM_BID).Init "sIV-b", "Straddle IV Bid (Call IV Bid + Put IV Bid) / 2", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_IV_SUM_ASK).Init "sIV-a", "Straddle IV Ask (Call IV Ask + Put IV Ask) / 2", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_CONTRACTS_BID).Init "Cont-b", "Contract on Bid (Vega Amount / Vega on Bid)", "#,##0", flexDTLong, False, True, True, True
        .Col(AUC_CONTRACTS_ASK).Init "Cont-a", "Contract on Ask (Vega Amount / Vega on Ask)", "#,##0", flexDTLong, False, True, True, True
        .Col(AUC_CALC_1).Init "Calc1", "1st Calculated Number", "#,##0.00", flexDTDouble, True, True, True, True, "Calculation Error"
        .Col(AUC_CALC_2).Init "Calc2", "2nd Calculated Number", "#,##0.00", flexDTDouble, True, True, True, True, "Calculation Error"
        .Col(AUC_LOAD_1).Init "Load1", "1st Loaded Number", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(AUC_LOAD_2).Init "Load2", "2nd Loaded Number", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(AUC_VEGA_BID).Init "Vga-b", "Vega on Bid %", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_VEGA_ASK).Init "Vga-a", "Vega on Ask %", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_DELTA_BID).Init "Dlt-b", "Delta on Bid %", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_SHARES_DELTA_BID).Init "Shrs-b", "Shares Delta on Bid", "#,##0", flexDTLong, False, True, True, True
        .Col(AUC_DELTA_ASK).Init "Dlt-a", "Delta on Ask %", "#,##0.00", flexDTDouble, False, True, True, True
        .Col(AUC_SHARES_DELTA_ASK).Init "Shrs-a", "Shares Delta on Ask", "#,##0", flexDTLong, False, True, True, True
    
        .Col(AUC_CALC_1).ForeColorAlt1 = &HFF&
        .Col(AUC_CALC_2).ForeColorAlt1 = &HFF&
        .Col(AUC_OPT_ROOT).ForeColorAlt1 = &HFF0000
    End With
End Sub

Private Sub InitGreeksGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_GREEKS_FILTER) = Nothing
    Set g_DefGrids(GT_GREEKS_UNDERLYING) = Nothing
    
    Set g_DefGrids(GT_GREEKS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_GREEKS_UNDERLYING) = New clsGridDef
    
    ' greeks filter grid
    With g_DefGrids(GT_GREEKS_FILTER)
        .Init GFC_GROUP, GFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To GFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(GFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(GFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(GFC_TYPE).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(GFC_MODEL).Init "Model", "Options Calculation Model", "", flexDTString, False, False, True, False
        .Col(GFC_MODE).Init "Mode", "Greeks Mode", "", flexDTString, True, False, True, False
    End With
    
    ' greeks underlyings grid
    With g_DefGrids(GT_GREEKS_UNDERLYING)
        .Init GUC_SYMBOL, GUC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = GUC_SYMBOL To GUC_LAST_COLUMN
            .Idx(i - GUC_SYMBOL) = i
        Next
    
        .Col(GUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(GUC_SYMBOL).Init "Sym", "Symbol", "", flexDTString, False, False, True, False
        .Col(GUC_VALUE).Init "Val", "Greek Value", "", flexDTString, False, False, True, False
        .Col(GUC_EXPIRY_ODD).Init "OddExp", "Odd Expiry", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(GUC_EXPIRY_EVEN).Init "EvenExp", "Even Expiry", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(GUC_SYM_TOTAL).Init "Total", "Symbol Total", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
    
        For i = GUC_EXPIRY_ODD To GUC_SYM_TOTAL
            .Col(i).ForeColorAlt1 = &HC0&
        Next
    End With
End Sub

Public Function AddGreeksRow(Info As CVLib.cvInfo) As Long
    On Error Resume Next
    
    g_GUR_LAST_ROW = g_GUR_LAST_ROW + 1
    g_GUR_ROW_COUNT = g_GUR_ROW_COUNT + 1

    g_DefGrids(GT_GREEKS_UND_ROWS).Idx(g_GUR_LAST_ROW - GUR_TIME_VALUE) = g_GUR_LAST_ROW
    g_DefGrids(GT_GREEKS_UND_ROWS).Col(g_GUR_LAST_ROW).Init Info.Caption, Info.Name, _
    "#,##0.00", IIf(Info.Type = cvNumeric, flexDTDouble, flexDTString), False, True, True, True

    AddGreeksRow = g_GUR_LAST_ROW
    
End Function

Public Sub InitGreeksRows(Optional ByVal cvCount As Long = 0)
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_GREEKS_UND_ROWS) = Nothing
    Set g_DefGrids(GT_GREEKS_UND_ROWS) = New clsGridDef
    
    With g_DefGrids(GT_GREEKS_UND_ROWS)
        .Init GUR_TIME_VALUE, GUR_LAST_ROW + cvCount
        .IsRowSelection = True
        
        For i = GUR_TIME_VALUE To GUR_LAST_ROW
            .Idx(i - GUR_TIME_VALUE) = i
        Next
    
        .Col(GUR_TIME_VALUE).Init "TimeValue", "TimeValue", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_DELTA_EQ).Init "OptDlt$", "OptDlt$", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_DELTA_IN_SHARES).Init "OptDlt", "OptDlt", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_GAMMA_EQ).Init "Gma$", "Gma$", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_GAMMA_IN_SHARES).Init "Gma$1", "Gma$1", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_VEGA).Init "Vega", "Vega", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_NETDELTA_EQ).Init "NetDlt$", "NetDlt$", "#,##0", flexDTEmpty, False, True, True, True
        .Col(GUR_NETDELTA_IN_SHARES).Init "NetDlt", "NetDlt", "#,##0", flexDTEmpty, False, True, True, True
        
        g_GUR_LAST_ROW = GUR_LAST_ROW
        g_GUR_ROW_COUNT = GUR_ROW_COUNT
    End With
End Sub

Private Sub InitHedgeSummaryGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_HEDGESUMMARY_FILTER) = Nothing
    Set g_DefGrids(GT_HEDGESUMMARY_INSTRUMENTS) = Nothing
    Set g_DefGrids(GT_HEDGESUMMARY_RESULTS) = Nothing
    Set g_DefGrids(GT_HEDGESUMMARY_TOTAL) = Nothing
    
    Set g_DefGrids(GT_HEDGESUMMARY_FILTER) = New clsGridDef
    Set g_DefGrids(GT_HEDGESUMMARY_INSTRUMENTS) = New clsGridDef
    Set g_DefGrids(GT_HEDGESUMMARY_RESULTS) = New clsGridDef
    Set g_DefGrids(GT_HEDGESUMMARY_TOTAL) = New clsGridDef

    ' hedge summary filter grid
    With g_DefGrids(GT_HEDGESUMMARY_FILTER)
        .Init HFC_GROUP, HFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To HFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(HFC_GROUP).Init "Type", "Values Type", "", flexDTString, True, False, True, False
        .Col(HFC_VALUE).Init "Value", "Value", "", flexDTString, True, False, True, False
        .Col(HFC_TYPE).Init "Hedge", "Hedge Type", "", flexDTString, True, False, True, False
        .Col(HFC_WEIGHTED).Init "WtdVega", "Weighted Vega", "", flexDTBoolean, True, False, False, True
    End With
    
    ' hedge summary instruments grid
    With g_DefGrids(GT_HEDGESUMMARY_INSTRUMENTS)
        .Init HIC_USE, HIC_LAST_COLUMN
        .IsRowSelection = False

        For i = 0 To HIC_LAST_COLUMN
            .Idx(i) = i
        Next

        .Col(HIC_USE).Init "Use", "Use for Hedge", "", flexDTBoolean, True, False, True, False
        .Col(HIC_SYMBOL).Init "Symbol", "Instrument Symbol", "", flexDTString, False, False, True, False
        .Col(HIC_TYPE).Init "Type", "Instrument Type", "", flexDTString, False, False, True, False
    End With

    ' hedge summary results grid
    With g_DefGrids(GT_HEDGESUMMARY_RESULTS)
        .Init HRC_SYMBOL, HRC_LAST_COLUMN
        .IsRowSelection = False

        For i = 0 To HRC_LAST_COLUMN
            .Idx(i) = i
        Next

        .Col(HRC_SYMBOL).Init "Symbol", "Hedge Symbol", "", flexDTString, False, False, True, False
        .Col(HRC_POS).Init "DeltaEq, $", "Hedge Position", "#,##0.00", flexDTDouble, False, False, True, True, "Some Items N/A"
        .Col(HRC_CONTRACTS).Init "DeltaEq", "Hedge Contracts", "#,##0.00", flexDTDouble, False, False, True, True, "Some Items N/A"
        .Col(HRC_VEGAEQ).Init "VegaEq, $", "Vega Equivalent", "#,##0.00", flexDTDouble, False, False, False, True, "Some Items N/A"
    End With

    ' hedge summary total grid
    With g_DefGrids(GT_HEDGESUMMARY_TOTAL)
        .Init HTC_NET, HTC_LAST_COLUMN
        .IsRowSelection = False

        For i = 0 To HTC_LAST_COLUMN
            .Idx(i) = i
        Next

        .Col(HTC_NET).Init "NET, $", "NET", "#,##0.00", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(HTC_GAIN).Init "Gain, %", "Gain", "#,##0.00", flexDTDouble, False, False, False, False, "Some Items N/A"
    End With
End Sub

Private Sub InitUnderlyingsGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_UNDERLYINGS_FILTER) = Nothing
    Set g_DefGrids(GT_UNDERLYINGS_LIST) = Nothing
    
    Set g_DefGrids(GT_UNDERLYINGS_FILTER) = New clsGridDef
    Set g_DefGrids(GT_UNDERLYINGS_LIST) = New clsGridDef
    
    ' underlyings filter grid
    With g_DefGrids(GT_UNDERLYINGS_FILTER)
        .Init NFC_TRADER, NFC_LAST_COLUMN
        .IsRowSelection = False
        .IsGlobal = True
        
        For i = 0 To NFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(NFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(NFC_GROUP).Init "Group", "Contracts Group", "", flexDTString, True, False, True, False
        .Col(NFC_SYMBOL).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
        .Col(NFC_TYPE).Init "Type", "Underlying Type", "", flexDTString, True, False, True, False
        .Col(NFC_ACTIVE).Init "Active", "Active Underlying", "", flexDTString, True, False, True, False
        .Col(NFC_OPT_STYLE).Init "Style", "Options Style", "", flexDTString, True, False, True, False
        .Col(NFC_CALENDAR).Init "ExpCal", "Options Expiration Calendar", "", flexDTString, True, False, True, False
        .Col(NFC_UND_PROFILE).Init "UndProfile", "Underlyings Price Profile", "", flexDTString, True, False, True, False
        .Col(NFC_OPT_PROFILE).Init "OptProfile", "Options Price Profile", "", flexDTString, True, False, True, False
        .Col(NFC_DIVS).Init "Divs", "Dividends", "", flexDTString, True, False, True, False
        .Col(NFC_HAS_POS).Init "HasPos", "Has Positions", "", flexDTString, True, False, True, False
        .Col(NFC_HAS_OPTIONS).Init "HasOpts", "Has Options", "", flexDTString, True, False, True, False
        .Col(NFC_HAS_SYNTH).Init "HasSynth", "Has Synthetic Roots", "", flexDTString, True, False, True, False     '  Uncomment if SU works correctly
        .Col(NFC_HTB).Init "HTB", "Is Hard to Borrow", "", flexDTString, True, False, True, False
        .Col(NFC_HEDGE_SYMBOL).Init "Hedge Symbol", "Is Hedge Symbol", "", flexDTString, True, False, True, False
        .Col(NFC_DIRTY).Init "Changed", "Is Parameters Changed", "", flexDTString, True, False, True, False
        .Col(NFC_HAS_FUTURES).Init "HasFuts", "Has Futures", "", flexDTString, True, False, True, False
    End With

    ' underlyngs list grid
    With g_DefGrids(GT_UNDERLYINGS_LIST)
        .Init NLC_SYMBOL, NLC_LAST_COLUMN
        .IsRowSelection = True
        .IsGlobal = True
        
        For i = NLC_SYMBOL To NLC_HAS_SYNTH     '  Change back if SU works correctly
            .Idx(i - NLC_SYMBOL) = i
        Next
        
        .Col(NLC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(NLC_SYMBOL).Init "Sym", "Underlying Symbol", "", flexDTString, False, True, True, False
        .Col(NLC_TYPE).Init "Type", "Underlying Type", "", flexDTString, False, True, True, True
        .Col(NLC_OPT_STYLE).Init "Style", "Options Style", "", flexDTString, True, True, True, True
        .Col(NLC_CALENDAR).Init "ExpCal", "Options Expiration Calendar", "", flexDTString, True, True, True, True
        .Col(NLC_UND_PROFILE).Init "UndProfile", "Underlyings Price Profile", "", flexDTString, True, True, True, True
        .Col(NLC_OPT_PROFILE).Init "OptProfile", "Options Price Profile", "", flexDTString, True, True, True, True
        .Col(NLC_DIVS).Init "Divs", "Dividends Type", "", flexDTString, True, True, True, True
        .Col(NLC_DIV_FREQ).Init "DivFreq", "Dividends Frequency", "", flexDTString, True, True, True, True
        .Col(NLC_DIV_DATE).Init "DivDate", "Last Dividends Date", "MM/DD/YYYY", flexDTDate, True, True, True, True
        .Col(NLC_DIV_AMT).Init "DivAmt", "Dividends Amount", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(NLC_YIELD).Init "Yield", "Yield", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(NLC_SKEW).Init "Skew", "Skew", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(NLC_KURT).Init "Kurt", "Kurtosis", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(NLC_ACTIVE).Init "Active", "Active Underlying", "", flexDTBoolean, True, True, True, True
        .Col(NLC_HTB).Init "HTB", "Is Hard to Borrow", "", flexDTBoolean, True, True, True, True
        .Col(NLC_HAS_POS).Init "HasPos", "Has Positions", "", flexDTBoolean, False, True, True, True
        .Col(NLC_HAS_OPTIONS).Init "HasOpt", "Has Options", "", flexDTBoolean, False, True, True, True
        .Col(NLC_HAS_SYNTH).Init "HasSynth", "Has Synthetic Roots", "", flexDTBoolean, False, True, True, True       '  Uncomment if SU works correctly
        .Col(NLC_SYMBOL_NAME).Init "Name", "Symbol Name", "", flexDTString, True, True, False, True
        .Col(NLC_DIRTY).Init "Changed", "Is Parameters Changed", "", flexDTBoolean, False, True, False, True
        .Col(NLC_HAS_FUTURES).Init "HasFut", "Has Futures", "", flexDTString, False, True, False, True
        .Col(NLC_FUT_ROOT).Init "FutRoot", "Futures Root Name", "", flexDTString, False, True, False, True
        .Col(NLC_FUT_UND).Init "FutUnd", "Futures Underlying Symbol", "", flexDTString, False, True, False, True
        .Col(NLC_FUT_MATURITY).Init "FutMat", "Futures Maturity Date", "MM/DD/YYYY", flexDTDate, False, True, False, True
        .Col(NLC_SOQ).Init "SOQ", "Start Opening Quotation", "#,##0.00", flexDTDouble, True, True, True, True
        .Col(NLC_ALLOW_LENDING).Init "Allow Lending", "Is Allow Lending", "", flexDTBoolean, True, True, True, True
        .Col(NLC_HEDGE_SYMBOL).Init "Hedge Symbol", "Is Hedge Symbol", "", flexDTBoolean, True, True, True, True
    End With
End Sub

Private Sub InitReportsGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_REPORTS_FILTER) = Nothing
    
    Set g_DefGrids(GT_REPORTS_FILTER) = New clsGridDef
    
    ' reports filter grid
    With g_DefGrids(GT_REPORTS_FILTER)
        .Init PFC_REPORT, PFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To PFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(PFC_REPORT).Init "Report", "Report Type", "", flexDTString, True, False, True, False
        .Col(PFC_UNDERLYING).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
        .Col(PFC_TYPE).Init "Trades", "Trades Type", "", flexDTString, True, False, True, False
        .Col(PFC_GROUP).Init "Group", "Contracts Group", "", flexDTString, True, False, True, False
        .Col(PFC_TRADER_GROUP).Init "TraderGroup", "TraderGroup", "", flexDTString, True, False, True, False
        .Col(PFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(PFC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, True, False, True, False
        .Col(PFC_MAX_STRIKE_DISTANCE).Init "Max Strike distance", "MAx Strike distance", "##.####", flexDTDouble, True, False, False, False
    End With
End Sub

Private Sub InitStockHedgeGrids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_STOCKHEDGE_FILTER) = Nothing
    Set g_DefGrids(GT_STOCKHEDGE_TOTALS) = Nothing
    Set g_DefGrids(GT_STOCKHEDGE_POSITIONS) = Nothing
    
    Set g_DefGrids(GT_STOCKHEDGE_FILTER) = New clsGridDef
    Set g_DefGrids(GT_STOCKHEDGE_TOTALS) = New clsGridDef
    Set g_DefGrids(GT_STOCKHEDGE_POSITIONS) = New clsGridDef
    
    ' filter grid
    With g_DefGrids(GT_STOCKHEDGE_FILTER)
        .Init SFC_TRADER, SFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To SFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(SFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(SFC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, True, False, True, False
        .Col(SFC_UNDERLYING).Init "Symbol", "Underlying Symbol", "", flexDTString, True, False, True, False
    End With
    
    ' total grid
    With g_DefGrids(GT_STOCKHEDGE_TOTALS)
        .Init STC_SLICE, STC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To STC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(STC_SLICE).Init "Slice", "Slice, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(STC_LOT_SIZE).Init "Lot", "Lot Size", "#,##0", flexDTLong, True, False, True, False
        .Col(STC_MAX_SIZE).Init "MaxSz", "Max Size", "#,##0", flexDTLong, True, False, True, False
        .Col(STC_NET_DELTA).Init "NetDlt", "Net Delta Sum", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(STC_HEDGE_POS).Init "Hedge", "Hedge Value Sum", "#,##0", flexDTLong, False, False, True, False, "Some Items N/A"
    
        .Col(STC_NET_DELTA).ForeColorAlt1 = &HC0&
        .Col(STC_HEDGE_POS).ForeColorAlt1 = &HC0&
        
        .Col(STC_NET_DELTA).BackColor = &H80000018
        .Col(STC_HEDGE_POS).BackColor = &H80000018
    End With
    
    ' list grid
    With g_DefGrids(GT_STOCKHEDGE_POSITIONS)
        .Init SPC_TRADER, SPC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = SPC_TRADER To SPC_LAST_COLUMN
            .Idx(i - 1) = i
        Next
    
        .Col(SPC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(SPC_TRADER).Init "Trader", "Trader", "", flexDTString, False, False, True, False
        .Col(SPC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, False, False, True, False
        .Col(SPC_SYMBOL).Init "Symbol", "Symbol", "", flexDTString, False, False, True, False
        .Col(SPC_NET_DELTA).Init "NetDlt", "Net Delta", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(SPC_HEDGE_POS).Init "Hedge", "Hedge Value", "#,##0", flexDTLong, True, False, True, False, "Some Items N/A"
    
        .Col(SPC_NET_DELTA).ForeColorAlt1 = &HC0&
        .Col(SPC_HEDGE_POS).ForeColorAlt1 = &HC0&
    End With
End Sub

Private Sub InitIndexHedge1Grids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_INDEXHEDGE1_FILTER) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE1_TOTALS) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE1_UNDERLYINGS) = Nothing
    
    Set g_DefGrids(GT_INDEXHEDGE1_FILTER) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE1_TOTALS) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE1_UNDERLYINGS) = New clsGridDef
    
    ' filter grid
    With g_DefGrids(GT_INDEXHEDGE1_FILTER)
        .Init IFC_TRADER, IFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To IFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(IFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(IFC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, True, False, True, False
        .Col(IFC_INDEX).Init "Index", "Basket Index", "", flexDTString, True, False, True, False
    End With
    
    ' total grid
    With g_DefGrids(GT_INDEXHEDGE1_TOTALS)
        .Init ITC_INDEX, ITC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To ITC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(ITC_INDEX).Init "Index", "Basket Index", "", flexDTString, False, False, True, False
        .Col(ITC_SLICE).Init "Slice", "Slice, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(ITC_LOT_SIZE).Init "Lot", "Lot Size", "#,##0", flexDTLong, True, False, True, False
        .Col(ITC_MAX_SIZE).Init "MaxSz", "Max Size", "#,##0", flexDTLong, True, False, True, False
        .Col(ITC_CONTRACTS).Init "Contracts", "Contracts Count", "#,##0", flexDTLong, True, False, True, False
        .Col(ITC_DELTA).Init "Delta%", "Delta, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(ITC_NET_DELTA).Init "NetDlt", "Net Delta Sum", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(ITC_PRICE).Init "Price", "Index Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(ITC_EXPOSURE).Init "Exposure", "Exposure", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
    
        .Col(ITC_NET_DELTA).ForeColorAlt1 = &HC0&
        .Col(ITC_EXPOSURE).ForeColorAlt1 = &HC0&
        
        .Col(ITC_NET_DELTA).BackColor = &H80000018
        .Col(ITC_PRICE).BackColor = &H80000018
        .Col(ITC_EXPOSURE).BackColor = &H80000018
    End With
    
    ' list grid
    With g_DefGrids(GT_INDEXHEDGE1_UNDERLYINGS)
        .Init IUC_SYMBOL, IUC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = IUC_SYMBOL To IUC_LAST_COLUMN
            .Idx(i - 1) = i
        Next
    
        .Col(IUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(IUC_SYMBOL).Init "Symbol", "Symbol", "", flexDTString, False, False, True, False
        .Col(IUC_WEIGHT).Init "Weight", "Weight, %", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(IUC_PRICE).Init "Price", "Symbol Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(IUC_IDX_SHARES).Init "IdxShares", "Equivalent Index Shares", "#,##0", flexDTLong, False, False, True, False
        .Col(IUC_CUR_SHARES).Init "CurShares", "Current Stock Position", "#,##0", flexDTLong, False, False, True, False
        .Col(IUC_TXN_SHARES).Init "TxnShares", "HedgeValue", "#,##0", flexDTLong, False, False, True, False
    End With
End Sub

Private Sub InitIndexHedge2Grids()
    On Error Resume Next
    Dim i&
    Set g_DefGrids(GT_INDEXHEDGE2_FILTER) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE2_TOTALS_1) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_1) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE2_TOTALS_2) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_2) = Nothing
    Set g_DefGrids(GT_INDEXHEDGE2_RESULTS) = Nothing
    
    Set g_DefGrids(GT_INDEXHEDGE2_FILTER) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE2_TOTALS_1) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_1) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE2_TOTALS_2) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_2) = New clsGridDef
    Set g_DefGrids(GT_INDEXHEDGE2_RESULTS) = New clsGridDef
    
    ' filter grid
    With g_DefGrids(GT_INDEXHEDGE2_FILTER)
        .Init JFC_TRADER, JFC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To JFC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(JFC_TRADER).Init "Trader", "Trader", "", flexDTString, True, False, True, False
        .Col(JFC_STRATEGY).Init "Strategy", "Strategy", "", flexDTString, True, False, True, False
        .Col(JFC_INDEX1).Init "Index 1", "1st Basket Index", "", flexDTString, True, False, True, False
        .Col(JFC_INDEX2).Init "Index 2", "2nd Basket Index", "", flexDTString, True, False, True, False
    End With
    
    ' total grid 1
    With g_DefGrids(GT_INDEXHEDGE2_TOTALS_1)
        .Init JTC_INDEX, JTC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To JTC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(JTC_INDEX).Init "Index1", "Basket Index 2", "", flexDTString, False, False, True, False
        .Col(JTC_SLICE).Init "Slice", "Slice, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(JTC_LOT_SIZE).Init "Lot", "Lot Size", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_MAX_SIZE).Init "MaxSz", "Max Size", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_CONTRACTS).Init "Contracts", "Contracts Count", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_DELTA).Init "Delta%", "Delta, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(JTC_NET_DELTA).Init "NetDlt", "Net Delta Sum", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(JTC_PRICE).Init "Price", "Index Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JTC_EXPOSURE).Init "Exposure", "Exposure", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
    
        .Col(JTC_NET_DELTA).ForeColorAlt1 = &HC0&
        .Col(JTC_EXPOSURE).ForeColorAlt1 = &HC0&
        
        .Col(JTC_NET_DELTA).BackColor = &H80000018
        .Col(JTC_PRICE).BackColor = &H80000018
        .Col(JTC_EXPOSURE).BackColor = &H80000018
    End With
    
    ' list grid 1
    With g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_1)
        .Init JUC_SYMBOL, JUC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = JUC_SYMBOL To JUC_LAST_COLUMN
            .Idx(i - 1) = i
        Next
    
        .Col(JUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(JUC_SYMBOL).Init "Symbol", "Symbol", "", flexDTString, False, False, True, False
        .Col(JUC_WEIGHT).Init "Weight", "Weight, %", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JUC_PRICE).Init "Price", "Symbol Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JUC_IDX_SHARES).Init "IdxShares", "Equivalent Index Shares", "#,##0", flexDTLong, False, False, True, False
    End With

    ' total grid 1
    With g_DefGrids(GT_INDEXHEDGE2_TOTALS_2)
        .Init JTC_INDEX, JTC_LAST_COLUMN
        .IsRowSelection = False
        
        For i = 0 To JTC_LAST_COLUMN
            .Idx(i) = i
        Next
        
        .Col(JTC_INDEX).Init "Index2", "Basket Index 2", "", flexDTString, False, False, True, False
        .Col(JTC_SLICE).Init "Slice", "Slice, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(JTC_LOT_SIZE).Init "Lot", "Lot Size", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_MAX_SIZE).Init "MaxSz", "Max Size", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_CONTRACTS).Init "Contracts", "Contracts Count", "#,##0", flexDTLong, True, False, True, False
        .Col(JTC_DELTA).Init "Delta%", "Delta, %", "#,##0.00", flexDTDouble, True, False, True, False
        .Col(JTC_NET_DELTA).Init "NetDlt", "Net Delta Sum", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
        .Col(JTC_PRICE).Init "Price", "Index Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JTC_EXPOSURE).Init "Exposure", "Exposure", "#,##0", flexDTDouble, False, False, True, False, "Some Items N/A"
    
        .Col(JTC_NET_DELTA).ForeColorAlt1 = &HC0&
        .Col(JTC_EXPOSURE).ForeColorAlt1 = &HC0&
        
        .Col(JTC_NET_DELTA).BackColor = &H80000018
        .Col(JTC_PRICE).BackColor = &H80000018
        .Col(JTC_EXPOSURE).BackColor = &H80000018
    End With
    
    ' list grid 1
    With g_DefGrids(GT_INDEXHEDGE2_UNDERLYINGS_2)
        .Init JUC_SYMBOL, JUC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = JUC_SYMBOL To JUC_LAST_COLUMN
            .Idx(i - 1) = i
        Next
    
        .Col(JUC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(JUC_SYMBOL).Init "Symbol", "Symbol", "", flexDTString, False, False, True, False
        .Col(JUC_WEIGHT).Init "Weight", "Weight, %", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JUC_PRICE).Init "Price", "Symbol Price", "#,##0.00", flexDTDouble, False, False, True, False
        .Col(JUC_IDX_SHARES).Init "IdxShares", "Equivalent Index Shares", "#,##0", flexDTLong, False, False, True, False
    End With

    ' results grid
    With g_DefGrids(GT_INDEXHEDGE2_RESULTS)
        .Init JRC_SYMBOL, JRC_LAST_COLUMN
        .IsRowSelection = True
        
        For i = JRC_SYMBOL To JRC_LAST_COLUMN
            .Idx(i - 1) = i
        Next
    
        .Col(JRC_KEY).Init "", "", "", flexDTEmpty, False, False, False, False
        
        .Col(JRC_SYMBOL).Init "Symbol", "Symbol", "", flexDTString, False, False, True, False
        .Col(JRC_IDX1_SHARES).Init "IdxShares1", "Equivalent 1st Index Shares", "#,##0", flexDTLong, False, False, True, False
        .Col(JRC_IDX2_SHARES).Init "IdxShares2", "Equivalent 2nd Index Shares", "#,##0", flexDTLong, False, False, True, False
        .Col(JRC_TXN_SHARES).Init "IndexTxn", "IndexTxn", "#,##0", flexDTLong, False, False, True, False
        .Col(JRC_CUR_SHARES).Init "CurShares", "Current Stock Position", "#,##0", flexDTLong, False, False, True, False
        .Col(JRC_TRANSACT).Init "Transact", "HedgeValue", "#,##0", flexDTLong, False, False, True, False
    End With
End Sub

Public Function GetDefaultSettingsFileName(ByVal enGridType As GridTypeEnum) As String
    On Error Resume Next
    Select Case enGridType
        Case GT_QUOTES_UNDERLYING
            GetDefaultSettingsFileName = "QuotesUnderlyingsCV.Default"
        Case GT_QUOTES_DIVIDENDS
            GetDefaultSettingsFileName = "QuotesDividendsCV.Default"
        Case GT_QUOTES_VOLA
            GetDefaultSettingsFileName = "QuotesVolasCV.Default"
        Case GT_QUOTES_OPTIONS
            GetDefaultSettingsFileName = "QuotesOptionsCV.Default"
        Case GT_TRADES_FILTER
            GetDefaultSettingsFileName = "TradesFilterCV.Default"
        Case GT_TRADES_LIST
            GetDefaultSettingsFileName = "TradesListCV.Default"
        Case GT_RISKS_FILTER
            GetDefaultSettingsFileName = "RisksFilterCV.Default"
        Case GT_RISKS_TOTALS
            GetDefaultSettingsFileName = "RisksTotalsCV.Default"
        Case GT_RISKS_POSITIONS
            GetDefaultSettingsFileName = "RisksPositionsCV.Default"
        Case GT_MATRIX_FILTER
            GetDefaultSettingsFileName = "MatrixFilterCV.Default"
        Case GT_MATRIX_SCENARIO
            GetDefaultSettingsFileName = "MatrixScenarioCV.Default"
        Case GT_MATRIX_VALUES
            GetDefaultSettingsFileName = "MatrixValuesCV.Default"
        Case GT_VOLA_FILTER
            GetDefaultSettingsFileName = "VolaFilterCV.Default"
        Case GT_VOLA_UNDERLYING
            GetDefaultSettingsFileName = "VolaUnderlyingsCV.Default"
        Case GT_QUOTATION_UNDERLYING
            GetDefaultSettingsFileName = "QuotationUnderlyingsCV.Default"
        Case GT_QUOTATION_DIVIDENDS
            GetDefaultSettingsFileName = "QuotationDividendsCV.Default"
        Case GT_QUOTATION_OPTIONS
            GetDefaultSettingsFileName = "QuotationOptionsCV.Default"
        Case GT_QUOTATION_LOG
            GetDefaultSettingsFileName = "QuotationLogCV.Default"
        Case GT_VOLAANALYSIS_FILTER
            GetDefaultSettingsFileName = "VolaAnalysisFilterCV.Default"
        Case GT_VOLAANALYSIS_UNDERLYING
            GetDefaultSettingsFileName = "VolaAnalysisUnderlyingsCV.Default"
        Case GT_GREEKS_FILTER
            GetDefaultSettingsFileName = "GreeksFilterCV.Default"
        Case GT_GREEKS_UNDERLYING
            GetDefaultSettingsFileName = "GreeksUnderlyingsCV.Default"
        Case GT_GREEKS_UND_ROWS
            GetDefaultSettingsFileName = "GreeksUndRowsCV.Default"
        Case GT_HEDGESUMMARY_FILTER
            GetDefaultSettingsFileName = "HedgeSummaryFilterCV.Default"
        Case GT_HEDGESUMMARY_INSTRUMENTS
            GetDefaultSettingsFileName = "HedgeSummaryInstrumentsCV.Default"
        Case GT_HEDGESUMMARY_RESULTS
            GetDefaultSettingsFileName = "HedgeSummaryResultsCV.Default"
        Case GT_HEDGESUMMARY_TOTAL
            GetDefaultSettingsFileName = "HedgeSummaryTotalCV.Default"
        Case GT_UNDERLYINGS_FILTER
            GetDefaultSettingsFileName = "UnderlyingsFilterCV.Default"
        Case GT_UNDERLYINGS_LIST
            GetDefaultSettingsFileName = "UnderlyingsListCV.Default"
        Case GT_REPORTS_FILTER
            GetDefaultSettingsFileName = "ReportsFilterCV.Default"
        Case GT_ORDERS_FILTER
            GetDefaultSettingsFileName = "OrdersFilterCV.Default"
        Case GT_ORDERS_LIST
            GetDefaultSettingsFileName = "OrdersListCV.Default"
        Case GT_ORDERS_LOG
            GetDefaultSettingsFileName = "OrdersLogCV.Default"
        Case GT_TNTCARDS_FILTER
            GetDefaultSettingsFileName = "TntCardsFilterCV.Default"
        Case GT_TNTCARDS_LIST
            GetDefaultSettingsFileName = "TntCardsListCV.Default"
        Case GT_TNTCARDS_LOG
            GetDefaultSettingsFileName = "TntCardsLogCV.Default"
        Case GT_STOCKHEDGE_FILTER
            GetDefaultSettingsFileName = "StockHedgeFilterCV.Default"
        Case GT_STOCKHEDGE_TOTALS
            GetDefaultSettingsFileName = "StockHedgeTotalsCV.Default"
        Case GT_STOCKHEDGE_POSITIONS
            GetDefaultSettingsFileName = "StockHedgePositionsCV.Default"
        Case GT_INDEXHEDGE1_FILTER
            GetDefaultSettingsFileName = "IndexHedge1FilterCV.Default"
        Case GT_INDEXHEDGE1_TOTALS
            GetDefaultSettingsFileName = "IndexHedge1TotalsCV.Default"
        Case GT_INDEXHEDGE1_UNDERLYINGS
            GetDefaultSettingsFileName = "IndexHedge1UnderlyingsCV.Default"
        Case GT_INDEXHEDGE2_FILTER
            GetDefaultSettingsFileName = "IndexHedge2FilterCV.Default"
        Case GT_INDEXHEDGE2_TOTALS_1
            GetDefaultSettingsFileName = "IndexHedge2Totals1CV.Default"
        Case GT_INDEXHEDGE2_UNDERLYINGS_1
            GetDefaultSettingsFileName = "IndexHedge2Underlyings1CV.Default"
        Case GT_INDEXHEDGE2_TOTALS_2
            GetDefaultSettingsFileName = "IndexHedge2Totals2CV.Default"
        Case GT_INDEXHEDGE2_UNDERLYINGS_2
            GetDefaultSettingsFileName = "IndexHedge2Underlyings2CV.Default"
        Case GT_INDEXHEDGE2_RESULTS
            GetDefaultSettingsFileName = "IndexHedge2ResultsCV.Default"
        Case Else
            GetDefaultSettingsFileName = ""
    End Select
End Function

Public Function GetDefaultSettingsTypeName(ByVal enGridType As GridTypeEnum) As String
    On Error Resume Next
    Select Case enGridType
        Case GT_QUOTES_UNDERLYING, GT_QUOTES_DIVIDENDS, GT_QUOTES_VOLA, GT_QUOTES_OPTIONS
            GetDefaultSettingsTypeName = "Quotes"
        Case GT_TRADES_FILTER, GT_TRADES_LIST
            GetDefaultSettingsTypeName = "Trades"
        Case GT_RISKS_FILTER, GT_RISKS_TOTALS, GT_RISKS_POSITIONS
            GetDefaultSettingsTypeName = "Risks"
        Case GT_MATRIX_FILTER, GT_MATRIX_SCENARIO, GT_MATRIX_VALUES
            GetDefaultSettingsTypeName = "Risk Matrix"
        Case GT_VOLA_FILTER, GT_VOLA_UNDERLYING
            GetDefaultSettingsTypeName = "Volatility"
        Case GT_QUOTATION_UNDERLYING, GT_QUOTATION_DIVIDENDS, GT_QUOTATION_OPTIONS, GT_QUOTATION_LOG
            GetDefaultSettingsTypeName = "Quotation"
        Case GT_VOLAANALYSIS_FILTER, GT_VOLAANALYSIS_UNDERLYING
            GetDefaultSettingsTypeName = "Volatility Analysis"
        Case GT_GREEKS_FILTER, GT_GREEKS_UNDERLYING, GT_GREEKS_UND_ROWS
            GetDefaultSettingsTypeName = "Greeks"
        Case GT_HEDGESUMMARY_FILTER, GT_HEDGESUMMARY_INSTRUMENTS, GT_HEDGESUMMARY_RESULTS, GT_HEDGESUMMARY_TOTAL
            GetDefaultSettingsTypeName = "Hedge Summary"
        Case GT_REPORTS_FILTER, GT_REPORTS_RESULTS
            GetDefaultSettingsTypeName = "Reports"
        Case GT_ORDERS_FILTER, GT_ORDERS_LIST, GT_ORDERS_LOG
            GetDefaultSettingsTypeName = "Orders"
        Case GT_TNTCARDS_FILTER, GT_TNTCARDS_LIST, GT_TNTCARDS_LOG
            GetDefaultSettingsTypeName = "Trade Messages"
        Case GT_STOCKHEDGE_FILTER, GT_STOCKHEDGE_TOTALS, GT_STOCKHEDGE_POSITIONS
            GetDefaultSettingsTypeName = "Stock Hedge"
        Case GT_INDEXHEDGE1_FILTER, GT_INDEXHEDGE1_TOTALS, GT_INDEXHEDGE1_UNDERLYINGS
            GetDefaultSettingsTypeName = "Index Hedge"
        Case GT_INDEXHEDGE2_FILTER, GT_INDEXHEDGE2_TOTALS_1, GT_INDEXHEDGE2_UNDERLYINGS_1, _
            GT_INDEXHEDGE2_TOTALS_2, GT_INDEXHEDGE2_UNDERLYINGS_2, GT_INDEXHEDGE2_RESULTS
            GetDefaultSettingsTypeName = "Two Indices Hedge"
        Case Else
            GetDefaultSettingsTypeName = ""
    End Select
End Function

Public Function GetDefaultGridViewName(ByVal enGridType As GridTypeEnum) As String
    On Error Resume Next
    Select Case enGridType
        Case GT_QUOTES_UNDERLYING, GT_QUOTES_DIVIDENDS, GT_QUOTES_VOLA, GT_QUOTES_OPTIONS
            GetDefaultGridViewName = "Quotes"
        Case GT_TRADES_FILTER, GT_TRADES_LIST
            GetDefaultGridViewName = "Trades"
        Case GT_RISKS_FILTER, GT_RISKS_TOTALS, GT_RISKS_POSITIONS
            GetDefaultGridViewName = "Risks"
        Case GT_MATRIX_FILTER, GT_MATRIX_SCENARIO, GT_MATRIX_VALUES
            GetDefaultGridViewName = "Risk Matrix"
        Case GT_VOLA_FILTER, GT_VOLA_UNDERLYING
            GetDefaultGridViewName = "Volatility Monitor"
        Case GT_QUOTATION_UNDERLYING, GT_QUOTATION_DIVIDENDS, GT_QUOTATION_OPTIONS, GT_QUOTATION_LOG
            GetDefaultGridViewName = "Quotation"
        Case GT_VOLAANALYSIS_FILTER, GT_VOLAANALYSIS_UNDERLYING
            GetDefaultGridViewName = "Volatility Analysis"
        Case GT_GREEKS_FILTER, GT_GREEKS_UNDERLYING, GT_GREEKS_UND_ROWS
            GetDefaultGridViewName = "Greeks"
        Case GT_HEDGESUMMARY_FILTER, GT_HEDGESUMMARY_INSTRUMENTS, GT_HEDGESUMMARY_RESULTS, GT_HEDGESUMMARY_TOTAL
            GetDefaultGridViewName = "Hedge Summary"
        Case GT_UNDERLYINGS_FILTER, GT_UNDERLYINGS_LIST
            GetDefaultGridViewName = "Underlyings"
        Case GT_REPORTS_FILTER, GT_REPORTS_RESULTS
            GetDefaultGridViewName = "Reports"
        Case GT_ORDERS_FILTER, GT_ORDERS_LIST, GT_ORDERS_LOG
            GetDefaultGridViewName = "Orders"
        Case GT_TNTCARDS_FILTER, GT_TNTCARDS_LIST, GT_TNTCARDS_LOG
            GetDefaultGridViewName = "Trades Messages"
        Case GT_STOCKHEDGE_FILTER, GT_STOCKHEDGE_TOTALS, GT_STOCKHEDGE_POSITIONS
            GetDefaultGridViewName = "Stock Hedge"
        Case GT_INDEXHEDGE1_FILTER, GT_INDEXHEDGE1_TOTALS, GT_INDEXHEDGE1_UNDERLYINGS
            GetDefaultGridViewName = "Index Hedge"
        Case GT_INDEXHEDGE2_FILTER, GT_INDEXHEDGE2_TOTALS_1, GT_INDEXHEDGE2_UNDERLYINGS_1, _
            GT_INDEXHEDGE2_TOTALS_2, GT_INDEXHEDGE2_UNDERLYINGS_2, GT_INDEXHEDGE2_RESULTS
            GetDefaultGridViewName = "Two Indices Hedge"
        Case Else
            GetDefaultGridViewName = ""
    End Select
End Function

Public Function GetDefaultGridColumnsName(ByVal enGridType As GridTypeEnum) As String
    On Error Resume Next
    Select Case enGridType
        Case GT_QUOTES_UNDERLYING
            GetDefaultGridColumnsName = "Underlying"
        Case GT_QUOTES_DIVIDENDS
            GetDefaultGridColumnsName = "Dividends/Rates"
        Case GT_QUOTES_VOLA
            GetDefaultGridColumnsName = "Volatilities"
        Case GT_QUOTES_OPTIONS
            GetDefaultGridColumnsName = "Options"
        Case GT_TRADES_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_TRADES_LIST
            GetDefaultGridColumnsName = "List"
        Case GT_RISKS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_RISKS_TOTALS
            GetDefaultGridColumnsName = "Totals"
        Case GT_RISKS_POSITIONS
            GetDefaultGridColumnsName = "Positions"
        Case GT_MATRIX_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_MATRIX_SCENARIO
            GetDefaultGridColumnsName = "Scenario"
        Case GT_MATRIX_VALUES
            GetDefaultGridColumnsName = "Values"
        Case GT_VOLA_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_VOLA_UNDERLYING
            GetDefaultGridColumnsName = "Underlyings"
        Case GT_QUOTATION_UNDERLYING
            GetDefaultGridColumnsName = "Underlying"
        Case GT_QUOTATION_DIVIDENDS
            GetDefaultGridColumnsName = "Dividends/Rates"
        Case GT_QUOTATION_OPTIONS
            GetDefaultGridColumnsName = "Options"
        Case GT_QUOTATION_LOG
            GetDefaultGridColumnsName = "Log"
        Case GT_VOLAANALYSIS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_VOLAANALYSIS_UNDERLYING
            GetDefaultGridColumnsName = "Underlyings"
        Case GT_GREEKS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_GREEKS_UNDERLYING
            GetDefaultGridColumnsName = "Underlyings"
        Case GT_GREEKS_UND_ROWS
            GetDefaultGridColumnsName = "Underlyings"
        Case GT_HEDGESUMMARY_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_HEDGESUMMARY_INSTRUMENTS
            GetDefaultGridColumnsName = "Instruments"
        Case GT_HEDGESUMMARY_RESULTS
            GetDefaultGridColumnsName = "Results"
        Case GT_HEDGESUMMARY_TOTAL
            GetDefaultGridColumnsName = "Total"
        Case GT_UNDERLYINGS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_UNDERLYINGS_LIST
            GetDefaultGridColumnsName = "List"
        Case GT_ORDERS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_ORDERS_LIST
            GetDefaultGridColumnsName = "List"
        Case GT_ORDERS_LOG
            GetDefaultGridColumnsName = "Log"
        Case GT_TNTCARDS_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_TNTCARDS_LIST
            GetDefaultGridColumnsName = "List"
        Case GT_TNTCARDS_LOG
            GetDefaultGridColumnsName = "Log"
        Case GT_STOCKHEDGE_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_STOCKHEDGE_TOTALS
            GetDefaultGridColumnsName = "Parameters/Totals"
        Case GT_STOCKHEDGE_POSITIONS
            GetDefaultGridColumnsName = "Stocks"
        Case GT_INDEXHEDGE1_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_INDEXHEDGE1_TOTALS
            GetDefaultGridColumnsName = "Parameters/Totals"
        Case GT_INDEXHEDGE1_UNDERLYINGS
            GetDefaultGridColumnsName = "Stocks"
        Case GT_INDEXHEDGE2_FILTER
            GetDefaultGridColumnsName = "Filter"
        Case GT_INDEXHEDGE2_TOTALS_1
            GetDefaultGridColumnsName = "1st Index Parameters/Totals"
        Case GT_INDEXHEDGE2_UNDERLYINGS_1
            GetDefaultGridColumnsName = "1st Index Stocks"
        Case GT_INDEXHEDGE2_TOTALS_2
            GetDefaultGridColumnsName = "2nd Index Parameters/Totals"
        Case GT_INDEXHEDGE2_UNDERLYINGS_2
            GetDefaultGridColumnsName = "2nd Index Stocks"
        Case GT_INDEXHEDGE2_RESULTS
            GetDefaultGridColumnsName = "Results"
        Case Else
            GetDefaultGridColumnsName = ""
    End Select
End Function

Public Function GetColumnsCount(ByVal enGridType As GridTypeEnum) As Long
    On Error Resume Next
    Select Case enGridType
        Case GT_RISKS_TOTALS
            GetColumnsCount = g_RTC_COLUMN_COUNT
        Case GT_RISKS_POSITIONS
            GetColumnsCount = g_RPC_COLUMN_COUNT
        Case GT_GREEKS_UND_ROWS
            GetColumnsCount = g_GUR_ROW_COUNT
        Case Else
            GetColumnsCount = 0
    End Select
End Function

Public Function DisplayNewCol(ByVal enGridType As GridTypeEnum, ByVal nID As Long)
    On Error Resume Next
    Dim i  As Long
    
    For i = 0 To GetColumnsCount(enGridType)
        If g_DefGrids(enGridType).Idx(i) = IDX_NONE Then
            g_DefGrids(enGridType).Idx(i) = nID
            Exit For
        End If
    Next
    
    Dim aGridTemp As New clsGridDef
    g_DefGrids(enGridType).CopyTo aGridTemp
    SaveGridDefAsDefault aGridTemp, enGridType
    
End Function


