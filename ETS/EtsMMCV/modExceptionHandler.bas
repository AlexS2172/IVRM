Attribute VB_Name = "modExceptionHandler"
'CSEH: Skip
'*****************************************************************************************
'* Module      : MExceptionHandler
'* Description : Unhandled Exception Handler filter.
'* Notes       : This module is used to provide your application a handler for the
'*               unhandled exceptions that may cause your program to crash.
'* Source      : AxTools Source+ 2000 - The Source+ Library
'* Usage       : Call "InstallExceptionHandler" on project initialization (in Form_Load,
'*               Main etc.) and "UninstallExceptionHandler" on project cleanup (in
'*               Form_Unload, etc.)
'*****************************************************************************************

Option Explicit

' Private class API function declarations
Private Declare Sub CopyMemory Lib "kernel32" Alias "RtlMoveMemory" (pDest As EXCEPTION_RECORD, ByVal LPEXCEPTION_RECORD As Long, ByVal lngBytes As Long)
Private Declare Function SetUnhandledExceptionFilter Lib "kernel32" (ByVal lpTopLevelExceptionFilter As Long) As Long
Private Declare Sub RaiseException Lib "kernel32" (ByVal dwExceptionCode As Long, ByVal dwExceptionFlags As Long, ByVal nNumberOfArguments As Long, lpArguments As Long)

' Public enums
Public Enum EExceptionType
    eExceptionType_AccessViolation = &HC0000005
    eExceptionType_DataTypeMisalignment = &H80000002
    eExceptionType_Breakpoint = &H80000003
    eExceptionType_SingleStep = &H80000004
    eExceptionType_ArrayBoundsExceeded = &HC000008C
    eExceptionType_FaultDenormalOperand = &HC000008D
    eExceptionType_FaultDivideByZero = &HC000008E
    eExceptionType_FaultInexactResult = &HC000008F
    eExceptionType_FaultInvalidOperation = &HC0000090
    eExceptionType_FaultOverflow = &HC0000091
    eExceptionType_FaultStackCheck = &HC0000092
    eExceptionType_FaultUnderflow = &HC0000093
    eExceptionType_IntegerDivisionByZero = &HC0000094
    eExceptionType_IntegerOverflow = &HC0000095
    eExceptionType_PriviledgedInstruction = &HC0000096
    eExceptionType_InPageError = &HC0000006
    eExceptionType_IllegalInstruction = &HC000001D
    eExceptionType_NoncontinuableException = &HC0000025
    eExceptionType_StackOverflow = &HC00000FD
    eExceptionType_InvalidDisposition = &HC0000026
    eExceptionType_GuardPageViolation = &H80000001
    eExceptionType_InvalidHandle = &HC0000008
    eExceptionType_ControlCExit = &HC000013A
End Enum

' Private enums
'Private Enum EExceptionHandlerReturn
'    EXCEPTION_CONTINUE_EXECUTION = -1
'    EXCEPTION_CONTINUE_SEARCH = 0
'    EXCEPTION_EXECUTE_HANDLER = 1
'End Enum

' Private class constants
'Maximum number of parameters an Exception_Record can have
Private Const EXCEPTION_MAXIMUM_PARAMETERS = 15

' Private class type definitions
'Structure that contains processor-specific register data
Private Type CONTEXT
    FltF0        As Double
    FltF1        As Double
    FltF2        As Double
    FltF3        As Double
    FltF4        As Double
    FltF5        As Double
    FltF6        As Double
    FltF7        As Double
    FltF8        As Double
    FltF9        As Double
    FltF10       As Double
    FltF11       As Double
    FltF12       As Double
    FltF13       As Double
    FltF14       As Double
    FltF15       As Double
    FltF16       As Double
    FltF17       As Double
    FltF18       As Double
    FltF19       As Double
    FltF20       As Double
    FltF21       As Double
    FltF22       As Double
    FltF23       As Double
    FltF24       As Double
    FltF25       As Double
    FltF26       As Double
    FltF27       As Double
    FltF28       As Double
    FltF29       As Double
    FltF30       As Double
    FltF31       As Double
    
    IntV0        As Double
    IntT0        As Double
    IntT1        As Double
    IntT2        As Double
    IntT3        As Double
    IntT4        As Double
    IntT5        As Double
    IntT6        As Double
    IntT7        As Double
    IntS0        As Double
    IntS1        As Double
    IntS2        As Double
    IntS3        As Double
    IntS4        As Double
    IntS5        As Double
    IntFp        As Double
    IntA0        As Double
    IntA1        As Double
    IntA2        As Double
    IntA3        As Double
    IntA4        As Double
    IntA5        As Double
    IntT8        As Double
    IntT9        As Double
    IntT10       As Double
    IntT11       As Double
    IntRa        As Double
    IntT12       As Double
    IntAt        As Double
    IntGp        As Double
    IntSp        As Double
    IntZero      As Double
    
    Fpcr         As Double
    SoftFpcr     As Double
    
    Fir          As Double
    Psr          As Long
    
    ContextFlags As Long
    Fill(4)      As Long
End Type
'Structure that describes an exception.
Private Type EXCEPTION_RECORD
    ExceptionCode                                        As Long
    ExceptionFlags                                       As Long
    pExceptionRecord                                     As Long  ' Pointer to an EXCEPTION_RECORD structure
    ExceptionAddress                                     As Long
    NumberParameters                                     As Long
    ExceptionInformation(EXCEPTION_MAXIMUM_PARAMETERS)   As Long
End Type
'Structure that contains exception information that can be used by a debugger.
'Private Type EXCEPTION_DEBUG_INFO
'    pExceptionRecord     As EXCEPTION_RECORD
'    dwFirstChance        As Long
'End Type
'The EXCEPTION_POINTERS structure contains an exception record with a
'machine-independent description of an exception and a context record
'with a machine-dependent description of the processor context at the
'time of the exception.
Private Type EXCEPTION_POINTERS
    pExceptionRecord     As EXCEPTION_RECORD
    ContextRecord        As CONTEXT
End Type

' Private variables for internal use
Private m_bFilterInstalled As Boolean


'*****************************************************************************************
'* Function    : ExceptionHandler
'* Notes       : This function receives an exception code value and returns the text
'*               description of the exception.
'*               This function will be called when an unhandled exception occurs.It raises
'*               an error so that it can be trapped with an ON ERROR statement in the
'*               procedure that caused the exception.
'*****************************************************************************************
Public Function ExceptionHandler(ByRef ExceptionPtrs As EXCEPTION_POINTERS) As Long
    On Error Resume Next
    
    Dim Rec As EXCEPTION_RECORD
    Dim strException As String
  
    'Get the current exception record.
    Rec = ExceptionPtrs.pExceptionRecord
  
    'If Rec.pExceptionRecord is not zero, then it is a nested exception and
    'Rec.pExceptionRecord points to another EXCEPTION_RECORD structure.  Follow
    'the pointers back to the original exception.
    Do Until Rec.pExceptionRecord = 0
        'A friendly declaration of CopyMemory.
        CopyMemory Rec, Rec.pExceptionRecord, Len(Rec)
    Loop
  
    'Translate the exception code into a user-friendly string.
    strException = GetExceptionDescription(Rec.ExceptionCode)
  
    'Raise an error to return control to the calling procedure.
    On Error GoTo 0
    Err.Raise 10000, "ExceptionHandler", strException
End Function


'*****************************************************************************************
'* Sub         : InstallExceptionHandler
'* Notes       : Installs the custom exception filter.
'*****************************************************************************************
Public Sub InstallExceptionHandler()
    On Error Resume Next
    
    If Not m_bFilterInstalled Then
        Call SetUnhandledExceptionFilter(AddressOf ExceptionHandler)
        m_bFilterInstalled = True
    End If
End Sub



'*****************************************************************************************
'* Function    : GetExceptionDescription
'* Notes       : Returns a string containing the description of the occurred exception.
'*****************************************************************************************
Public Function GetExceptionDescription(ExceptionType As EExceptionType) As String
    On Error Resume Next
    
    Dim strDescription As String
  
    Select Case ExceptionType
        
        Case eExceptionType_AccessViolation
            strDescription = "Access Violation"
        
        Case eExceptionType_DataTypeMisalignment
            strDescription = "Data Type Misalignment"
        
        Case eExceptionType_Breakpoint
            strDescription = "Breakpoint"
        
        Case eExceptionType_SingleStep
            strDescription = "Single Step"
        
        Case eExceptionType_ArrayBoundsExceeded
            strDescription = "Array Bounds Exceeded"
        
        Case eExceptionType_FaultDenormalOperand
            strDescription = "Float Denormal Operand"
        
        Case eExceptionType_FaultDivideByZero
            strDescription = "Divide By Zero"
        
        Case eExceptionType_FaultInexactResult
            strDescription = "Floating Point Inexact Result"
        
        Case eExceptionType_FaultInvalidOperation
            strDescription = "Invalid Operation"
        
        Case eExceptionType_FaultOverflow
            strDescription = "Float Overflow"
        
        Case eExceptionType_FaultStackCheck
            strDescription = "Float Stack Check"
        
        Case eExceptionType_FaultUnderflow
            strDescription = "Float Underflow"
        
        Case eExceptionType_IntegerDivisionByZero
            strDescription = "Integer Divide By Zero"
        
        Case eExceptionType_IntegerOverflow
            strDescription = "Integer Overflow"
        
        Case eExceptionType_PriviledgedInstruction
            strDescription = "Privileged Instruction"
        
        Case eExceptionType_InPageError
            strDescription = "In Page Error"
        
        Case eExceptionType_IllegalInstruction
            strDescription = "Illegal Instruction"
        
        Case eExceptionType_NoncontinuableException
            strDescription = "Non Continuable Exception"
        
        Case eExceptionType_StackOverflow
            strDescription = "Stack Overflow"
        
        Case eExceptionType_InvalidDisposition
            strDescription = "Invalid Disposition"
        
        Case eExceptionType_GuardPageViolation
            strDescription = "Guard Page Violation"
        
        Case eExceptionType_InvalidHandle
            strDescription = "Invalid Handle"
        
        Case eExceptionType_ControlCExit
            strDescription = "Control-C Exit"
        
        Case Else
            strDescription = "Unknown (&H" & Right$("00000000" & Hex$(ExceptionType), 8) & ")"
    
    End Select
    
    GetExceptionDescription = strDescription
End Function


'*****************************************************************************************
'* Function    : GetExceptionName
'* Notes       : Returns a string containing the name of the occurred exception.
'*****************************************************************************************
Public Function GetExceptionName(ExceptionType As EExceptionType) As String
    On Error Resume Next
    
    Dim strDescription As String
  
    Select Case ExceptionType
        
        Case eExceptionType_AccessViolation
            strDescription = "EXCEPTION_ACCESS_VIOLATION"
        
        Case eExceptionType_DataTypeMisalignment
            strDescription = "EXCEPTION_DATATYPE_MISALIGNMENT"
        
        Case eExceptionType_Breakpoint
            strDescription = "EXCEPTION_BREAKPOINT"
        
        Case eExceptionType_SingleStep
            strDescription = "EXCEPTION_SINGLE_STEP"
        
        Case eExceptionType_ArrayBoundsExceeded
            strDescription = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED"
        
        Case eExceptionType_FaultDenormalOperand
            strDescription = "EXCEPTION_FLT_DENORMAL_OPERAND"
        
        Case eExceptionType_FaultDivideByZero
            strDescription = "EXCEPTION_FLT_DIVIDE_BY_ZERO"
        
        Case eExceptionType_FaultInexactResult
            strDescription = "EXCEPTION_FLT_INEXACT_RESULT"
        
        Case eExceptionType_FaultInvalidOperation
            strDescription = "EXCEPTION_FLT_INVALID_OPERATION"
        
        Case eExceptionType_FaultOverflow
            strDescription = "EXCEPTION_FLT_OVERFLOW"
        
        Case eExceptionType_FaultStackCheck
            strDescription = "EXCEPTION_FLT_STACK_CHECK"
        
        Case eExceptionType_FaultUnderflow
            strDescription = "EXCEPTION_FLT_UNDERFLOW"
        
        Case eExceptionType_IntegerDivisionByZero
            strDescription = "EXCEPTION_INT_DIVIDE_BY_ZERO"
        
        Case eExceptionType_IntegerOverflow
            strDescription = "EXCEPTION_INT_OVERFLOW"
        
        Case eExceptionType_PriviledgedInstruction
            strDescription = "EXCEPTION_PRIVILEGED_INSTRUCTION"
        
        Case eExceptionType_InPageError
            strDescription = "EXCEPTION_IN_PAGE_ERROR"
        
        Case eExceptionType_IllegalInstruction
            strDescription = "EXCEPTION_ILLEGAL_INSTRUCTION"
        
        Case eExceptionType_NoncontinuableException
            strDescription = "EXCEPTION_NONCONTINUABLE_EXCEPTION"
        
        Case eExceptionType_StackOverflow
            strDescription = "EXCEPTION_STACK_OVERFLOW"
        
        Case eExceptionType_InvalidDisposition
            strDescription = "EXCEPTION_INVALID_DISPOSITION"
        
        Case eExceptionType_GuardPageViolation
            strDescription = "EXCEPTION_GUARD_PAGE_VIOLATION"
        
        Case eExceptionType_InvalidHandle
            strDescription = "EXCEPTION_INVALID_HANDLE"
        
        Case eExceptionType_ControlCExit
            strDescription = "EXCEPTION_CONTROL_C_EXIT"
        
        Case Else
            strDescription = "Unknown"
    
    End Select
    
    GetExceptionName = strDescription
End Function


'*****************************************************************************************
'* Sub         : RaiseAnException
'* Notes       : Raises an exception of the specified type.
'*****************************************************************************************
Public Sub RaiseAnException(ExceptionType As EExceptionType)
    
    'The following line raises an exception
    RaiseException ExceptionType, 0, 0, 0
End Sub

'*****************************************************************************************
'* Sub         : UninstallExceptionHandler
'* Notes       : Uninstalls the custom exception filter and restore the default one.
'*****************************************************************************************
Public Sub UninstallExceptionHandler()
    On Error Resume Next
    
    If m_bFilterInstalled Then
        Call SetUnhandledExceptionFilter(0&)
        m_bFilterInstalled = False
    End If
End Sub

