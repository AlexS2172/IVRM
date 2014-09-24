VERSION 5.00
Object = "{3B008041-905A-11D1-B4AE-444553540000}#1.0#0"; "Vsocx6.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{C932BA88-4374-101B-A56C-00AA003668DC}#1.1#0"; "MSMASK32.OCX"
Begin VB.Form frmParams 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Parameters"
   ClientHeight    =   4935
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   10230
   Icon            =   "frmParams.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4935
   ScaleWidth      =   10230
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin vsOcx6LibCtl.vsIndexTab vsTabs 
      Height          =   4335
      Left            =   120
      TabIndex        =   0
      Top             =   0
      Width           =   9885
      _ExtentX        =   17436
      _ExtentY        =   7646
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Enabled         =   -1  'True
      Appearance      =   1
      MousePointer    =   0
      _ConvInfo       =   1
      Version         =   600
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      FrontTabColor   =   -2147483633
      BackTabColor    =   -2147483633
      TabOutlineColor =   0
      FrontTabForeColor=   -2147483630
      Caption         =   "&Calculations|&Price Profiles|&Quotes|&Risks|Risk &Matrix|&Orders/Msgs|&Reports|&Advanced "
      Align           =   0
      Appearance      =   1
      CurrTab         =   0
      FirstTab        =   0
      Style           =   3
      Position        =   0
      AutoSwitch      =   -1  'True
      AutoScroll      =   -1  'True
      TabPreview      =   -1  'True
      ShowFocusRect   =   -1  'True
      TabsPerPage     =   0
      BorderWidth     =   0
      BoldCurrent     =   0   'False
      DogEars         =   -1  'True
      MultiRow        =   0   'False
      MultiRowOffset  =   200
      CaptionStyle    =   0
      TabHeight       =   0
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   7
         Left            =   12330
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   111
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraStartUp 
            Caption         =   "StartUp"
            Height          =   735
            Left            =   120
            TabIndex        =   150
            Top             =   2520
            Width           =   3855
            Begin VB.TextBox txtExpDays 
               BeginProperty DataFormat 
                  Type            =   1
                  Format          =   "0"
                  HaveTrueFalseNull=   0
                  FirstDayOfWeek  =   0
                  FirstWeekOfYear =   0
                  LCID            =   1033
                  SubFormatType   =   1
               EndProperty
               Height          =   315
               Left            =   2520
               TabIndex        =   152
               Top             =   240
               Width           =   555
            End
            Begin VB.CheckBox chkCheckForExp 
               Caption         =   "Check For Expired Options in"
               Height          =   255
               Left            =   120
               TabIndex        =   151
               Top             =   240
               Width           =   2415
            End
            Begin VB.Label Label2 
               Caption         =   "day(s)."
               Height          =   255
               Left            =   3120
               TabIndex        =   153
               Top             =   240
               Width           =   495
            End
         End
         Begin VB.Frame fraAdvSpeedOpt 
            Caption         =   " Speed Optimization "
            Height          =   735
            Left            =   120
            TabIndex        =   125
            Top             =   120
            Width           =   3885
            Begin MSForms.CheckBox chkAdvActiveRealTime 
               Height          =   375
               Left            =   120
               TabIndex        =   62
               Top             =   240
               Width           =   2295
               VariousPropertyBits=   746588177
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "4048;661"
               Value           =   "1"
               Caption         =   "Active Window Real Time"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.CheckBox chkAdvIvRealTime 
               Height          =   375
               Left            =   120
               TabIndex        =   126
               Top             =   540
               Visible         =   0   'False
               Width           =   2295
               VariousPropertyBits=   746588179
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "4048;661"
               Value           =   "0"
               Caption         =   "Calculate IV in Real Time"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraAdvDefTrader 
            Caption         =   " Default Active Trader "
            Height          =   735
            Left            =   4110
            TabIndex        =   123
            Top             =   120
            Width           =   3885
            Begin VB.ComboBox cboAdvDefTrader 
               Height          =   315
               ItemData        =   "frmParams.frx":000C
               Left            =   810
               List            =   "frmParams.frx":000E
               Sorted          =   -1  'True
               Style           =   2  'Dropdown List
               TabIndex        =   66
               Top             =   270
               Width           =   2805
            End
            Begin VB.Label lblAdvDefTrader 
               Alignment       =   1  'Right Justify
               Caption         =   "Trader :"
               Height          =   255
               Left            =   120
               TabIndex        =   124
               Top             =   315
               Width           =   615
            End
         End
         Begin VB.Frame fraAdvCalcOpt 
            Caption         =   " Calculation Optimization "
            Enabled         =   0   'False
            Height          =   1515
            Left            =   120
            TabIndex        =   119
            Top             =   960
            Width           =   3885
            Begin VB.TextBox txtAdvCalcSleepFreq 
               Enabled         =   0   'False
               Height          =   315
               Left            =   2100
               TabIndex        =   64
               Text            =   "10"
               Top             =   600
               Width           =   555
            End
            Begin VB.TextBox txtAdvCalcSleepAmt 
               Enabled         =   0   'False
               Height          =   315
               Left            =   2100
               TabIndex        =   65
               Text            =   "1"
               Top             =   1020
               Width           =   555
            End
            Begin VB.Label lblAdvCalcSleepFreq 
               Alignment       =   1  'Right Justify
               Caption         =   "Delay Frequency, steps :"
               Height          =   255
               Left            =   180
               TabIndex        =   122
               Top             =   630
               Width           =   1875
            End
            Begin MSForms.CheckBox chkAdvUseCalcSleep 
               Height          =   375
               Left            =   2085
               TabIndex        =   63
               Top             =   210
               Width           =   540
               VariousPropertyBits=   746588177
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "952;661"
               Value           =   "1"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
               ParagraphAlign  =   2
            End
            Begin VB.Label lblAdvCalcSleepAmt 
               Alignment       =   1  'Right Justify
               Caption         =   "Delay Time, ms :"
               Height          =   255
               Left            =   180
               TabIndex        =   121
               Top             =   1050
               Width           =   1875
            End
            Begin VB.Label lblAdvUseCalcSleep 
               Alignment       =   1  'Right Justify
               Caption         =   "Use Calculation Delay :"
               Height          =   315
               Left            =   180
               TabIndex        =   120
               Top             =   270
               Width           =   1875
            End
         End
         Begin VB.Frame fraAdvCalcExch 
            Caption         =   " Default Exchanges for Calcs "
            Height          =   1095
            Left            =   120
            TabIndex        =   114
            Top             =   2520
            Visible         =   0   'False
            Width           =   2535
            Begin VB.ComboBox cboAdvCalcDefExchUnd 
               Height          =   315
               ItemData        =   "frmParams.frx":0010
               Left            =   1050
               List            =   "frmParams.frx":0012
               Sorted          =   -1  'True
               Style           =   2  'Dropdown List
               TabIndex        =   116
               Top             =   270
               Width           =   1365
            End
            Begin VB.ComboBox cboAdvCalcDefExchOpt 
               Height          =   315
               ItemData        =   "frmParams.frx":0014
               Left            =   1050
               List            =   "frmParams.frx":0016
               Sorted          =   -1  'True
               Style           =   2  'Dropdown List
               TabIndex        =   115
               Top             =   660
               Width           =   1365
            End
            Begin VB.Label lblAdvCalcDefExchUnd 
               Alignment       =   1  'Right Justify
               Caption         =   "Underlying :"
               Height          =   255
               Left            =   120
               TabIndex        =   118
               Top             =   315
               Width           =   855
            End
            Begin VB.Label lblAdvCalcDefExchOpt 
               Alignment       =   1  'Right Justify
               Caption         =   "Options :"
               Height          =   255
               Left            =   120
               TabIndex        =   117
               Top             =   675
               Width           =   855
            End
         End
         Begin VB.Frame fraAdvActTrd 
            Caption         =   " Trade Editing "
            Height          =   1095
            Left            =   4110
            TabIndex        =   113
            Top             =   960
            Width           =   3885
            Begin MSForms.OptionButton optAdvActTrdEditable 
               Height          =   375
               Left            =   120
               TabIndex        =   68
               Top             =   600
               Width           =   4095
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "7223;661"
               Value           =   "0"
               Caption         =   "RTTF/Uploaded Trades Editable"
               GroupName       =   "Adv1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optAdvActTrdProtected 
               Height          =   375
               Left            =   120
               TabIndex        =   67
               Top             =   240
               Width           =   4095
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "7223;661"
               Value           =   "1"
               Caption         =   "RTTF/Uploaded Trades Protected"
               GroupName       =   "Adv1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraDefNewTrd 
            Caption         =   " Default New Trade "
            Height          =   1095
            Left            =   4110
            TabIndex        =   112
            Top             =   2160
            Width           =   3885
            Begin MSForms.OptionButton optDefNewTrdSimulated 
               Height          =   375
               Left            =   120
               TabIndex        =   69
               Top             =   240
               Width           =   1095
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1931;661"
               Value           =   "1"
               Caption         =   "Simulated"
               GroupName       =   "DefTrd"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optDefNewTrdManual 
               Height          =   375
               Left            =   120
               TabIndex        =   70
               Top             =   600
               Width           =   1095
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1931;661"
               Value           =   "0"
               Caption         =   "Manual"
               GroupName       =   "DefTrd"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optDefNewTrdActual 
               Height          =   375
               Left            =   1740
               TabIndex        =   71
               Top             =   240
               Width           =   1095
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1931;661"
               Value           =   "0"
               Caption         =   "Actual"
               GroupName       =   "DefTrd"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optDefNewTrdLastEntered 
               Height          =   375
               Left            =   1740
               TabIndex        =   72
               Top             =   600
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "0"
               Caption         =   "Last Entered"
               GroupName       =   "DefTrd"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   6
         Left            =   12030
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   96
         Top             =   345
         Width           =   9795
         Begin VB.Frame Frame2 
            Caption         =   "With Early Exercise"
            Height          =   2055
            Left            =   240
            TabIndex        =   143
            Top             =   360
            Width           =   4215
            Begin VB.Frame Frame3 
               Caption         =   "Upcoming div Days"
               Height          =   1335
               Left            =   120
               TabIndex        =   145
               Top             =   600
               Width           =   3975
               Begin VB.TextBox ctlDaysToDiv 
                  ForeColor       =   &H80000003&
                  Height          =   285
                  Left            =   2640
                  TabIndex        =   148
                  Text            =   "60"
                  Top             =   480
                  Width           =   975
               End
               Begin VB.OptionButton optUncOff 
                  Caption         =   "Off"
                  Height          =   375
                  Left            =   120
                  TabIndex        =   147
                  Top             =   720
                  Value           =   -1  'True
                  Width           =   975
               End
               Begin VB.OptionButton optUncOn 
                  Caption         =   "On"
                  Height          =   255
                  Left            =   120
                  TabIndex        =   146
                  Top             =   360
                  Width           =   975
               End
               Begin VB.Label Label1 
                  Caption         =   "Days to Div"
                  Height          =   255
                  Left            =   1440
                  TabIndex        =   149
                  Top             =   480
                  Width           =   1095
               End
            End
            Begin VB.CheckBox ckOptWithPos 
               Caption         =   "Options with Positions Only"
               Height          =   255
               Left            =   120
               TabIndex        =   144
               Top             =   240
               Value           =   1  'Checked
               Width           =   3495
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   5
         Left            =   11730
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   142
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraOrdOptIncrements 
            Caption         =   " Option Orders Default Increments "
            Height          =   1665
            Left            =   120
            TabIndex        =   134
            Top             =   120
            Width           =   3885
            Begin EtsMM.ctlEditEx txtOrdOptIncrPriceCheap 
               Height          =   315
               Left            =   2400
               TabIndex        =   54
               Top             =   720
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":0018
               font            =   "frmParams.frx":0036
               hideselection   =   -1  'True
            End
            Begin EtsMM.ctlEditEx txtOrdOptIncrQty 
               Height          =   315
               Left            =   2400
               TabIndex        =   53
               Top             =   270
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":005A
               font            =   "frmParams.frx":0078
               hideselection   =   -1  'True
            End
            Begin EtsMM.ctlEditEx txtOrdOptIncrPriceExpensive 
               Height          =   315
               Left            =   2400
               TabIndex        =   55
               Top             =   1170
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":009C
               font            =   "frmParams.frx":00BA
               hideselection   =   -1  'True
            End
            Begin VB.Label lblOrdOptIncrPriceExpensive 
               Alignment       =   1  'Right Justify
               Caption         =   "Price Increment for Prices Greater than $3, $ :"
               Height          =   435
               Left            =   180
               TabIndex        =   137
               Top             =   1080
               Width           =   2115
            End
            Begin VB.Label lblOrdOptIncrPriceCheap 
               Alignment       =   1  'Right Justify
               Caption         =   "Price Increment for Prices Less than $3, $ :"
               Height          =   435
               Left            =   180
               TabIndex        =   136
               Top             =   630
               Width           =   2115
            End
            Begin VB.Label lblOrdOptIncrQty 
               Alignment       =   1  'Right Justify
               Caption         =   "Quantity Increment :"
               Height          =   315
               Left            =   180
               TabIndex        =   135
               Top             =   300
               Width           =   2115
            End
         End
         Begin VB.Frame fraCrdUseTnt 
            Caption         =   " Trades Messages Using "
            Height          =   735
            Left            =   4110
            TabIndex        =   133
            Top             =   1890
            Width           =   3885
            Begin VB.CheckBox chkCrdUseTnt 
               Caption         =   "Use Trades Messages Service"
               Height          =   255
               Left            =   180
               TabIndex        =   61
               Top             =   300
               Width           =   3495
            End
         End
         Begin VB.Frame fraCrdIncrements 
            Caption         =   " Trades Messages Default Increments "
            Height          =   1665
            Left            =   4110
            TabIndex        =   130
            Top             =   120
            Width           =   3885
            Begin EtsMM.ctlEditEx txtCrdIncrQty 
               Height          =   315
               Left            =   2400
               TabIndex        =   58
               Top             =   270
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":00DE
               font            =   "frmParams.frx":00FC
               hideselection   =   -1  'True
            End
            Begin EtsMM.ctlEditEx txtCrdIncrPriceCheap 
               Height          =   315
               Left            =   2400
               TabIndex        =   59
               Top             =   720
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":0120
               font            =   "frmParams.frx":013E
               hideselection   =   -1  'True
            End
            Begin EtsMM.ctlEditEx txtCrdIncrPriceExpensive 
               Height          =   315
               Left            =   2400
               TabIndex        =   60
               Top             =   1170
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":0162
               font            =   "frmParams.frx":0180
               hideselection   =   -1  'True
            End
            Begin VB.Label lblCrdIncrPriceExpensive 
               Alignment       =   1  'Right Justify
               Caption         =   "Price Increment for Prices Greater than $3, $ :"
               Height          =   435
               Left            =   180
               TabIndex        =   138
               Top             =   1080
               Width           =   2115
            End
            Begin VB.Label lblCrdIncrQty 
               Alignment       =   1  'Right Justify
               Caption         =   "Quantity Increment :"
               Height          =   315
               Left            =   180
               TabIndex        =   132
               Top             =   300
               Width           =   2115
            End
            Begin VB.Label lblCrdIncrPriceCheap 
               Alignment       =   1  'Right Justify
               Caption         =   "Price Increment for Prices Less than $3, $ :"
               Height          =   435
               Left            =   180
               TabIndex        =   131
               Top             =   630
               Width           =   2115
            End
         End
         Begin VB.Frame fraOrdStkIncrements 
            Caption         =   " Stock Orders Default Increments "
            Height          =   1215
            Left            =   120
            TabIndex        =   127
            Top             =   1890
            Width           =   3885
            Begin EtsMM.ctlEditEx txtOrdStkIncrQty 
               Height          =   315
               Left            =   2400
               TabIndex        =   56
               Top             =   270
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":01A4
               font            =   "frmParams.frx":01C2
               hideselection   =   -1  'True
            End
            Begin EtsMM.ctlEditEx txtOrdStkIncrPrice 
               Height          =   315
               Left            =   2400
               TabIndex        =   57
               Top             =   720
               Width           =   795
               _extentx        =   1402
               _extenty        =   556
               mouseicon       =   "frmParams.frx":01E6
               font            =   "frmParams.frx":0204
               hideselection   =   -1  'True
            End
            Begin VB.Label lblOrdStkIncrPrice 
               Alignment       =   1  'Right Justify
               Caption         =   "Price Increment, $ :"
               Height          =   315
               Left            =   180
               TabIndex        =   129
               Top             =   750
               Width           =   2115
            End
            Begin VB.Label lblOrdStkIncrQty 
               Alignment       =   1  'Right Justify
               Caption         =   "Quantity Increment :"
               Height          =   315
               Left            =   180
               TabIndex        =   128
               Top             =   300
               Width           =   2115
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   4
         Left            =   11430
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   79
         TabStop         =   0   'False
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraMatrixRequestType 
            Caption         =   " Market Data Request Type "
            Enabled         =   0   'False
            Height          =   1095
            Left            =   120
            TabIndex        =   95
            Top             =   1320
            Width           =   3885
            Begin MSForms.OptionButton optMatrixReqTypeAlwaysNonGroup 
               Height          =   375
               Left            =   120
               TabIndex        =   52
               Top             =   600
               Width           =   3210
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5662;661"
               Value           =   "1"
               Caption         =   "Always Use Non-group Request"
               GroupName       =   "Matrix2"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optMatrixReqTypeOnPriceProvider 
               Height          =   375
               Left            =   120
               TabIndex        =   51
               Top             =   240
               Width           =   3495
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "6165;661"
               Value           =   "0"
               Caption         =   "Depends on Price Provider"
               GroupName       =   "Matrix2"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraMatrixTimeScenario 
            Caption         =   " Time Scenario "
            Height          =   1095
            Left            =   120
            TabIndex        =   87
            Top             =   120
            Width           =   3885
            Begin MSForms.OptionButton optMatrixIgnoreOpts 
               Height          =   375
               Left            =   120
               TabIndex        =   50
               Top             =   600
               Width           =   3180
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5609;661"
               Value           =   "0"
               Caption         =   "Ignore Options with Expiration Passed"
               GroupName       =   "Matrix1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optMatrixExerciseOpts 
               Height          =   375
               Left            =   120
               TabIndex        =   49
               Top             =   240
               Width           =   3615
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "6376;661"
               Value           =   "1"
               Caption         =   "Exercise Options"
               GroupName       =   "Matrix1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   3
         Left            =   11130
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   78
         TabStop         =   0   'False
         Top             =   345
         Width           =   9795
         Begin VB.CheckBox chkRTEditMsg 
            Caption         =   "Show Notifications"
            Height          =   255
            Left            =   4200
            TabIndex        =   154
            Top             =   2640
            Width           =   3375
         End
         Begin VB.Frame frNetExposure 
            Caption         =   "Net Exposure "
            Height          =   855
            Left            =   120
            TabIndex        =   157
            Top             =   2520
            Width           =   3885
            Begin MSMask.MaskEdBox entAUM 
               BeginProperty DataFormat 
                  Type            =   1
                  Format          =   "0.000;(0.000)"
                  HaveTrueFalseNull=   0
                  FirstDayOfWeek  =   0
                  FirstWeekOfYear =   0
                  LCID            =   1033
                  SubFormatType   =   1
               EndProperty
               Height          =   300
               Left            =   2400
               TabIndex        =   156
               Top             =   300
               Width           =   1335
               _ExtentX        =   2355
               _ExtentY        =   529
               _Version        =   393216
               Format          =   "#,##0.##0"
               PromptChar      =   "_"
            End
            Begin VB.Label lbAUM 
               Caption         =   "Assets Under Management:"
               Height          =   255
               Left            =   240
               TabIndex        =   155
               Top             =   360
               Width           =   2055
            End
         End
         Begin VB.Frame fraRiskRequestType 
            Caption         =   " Market Data Request Type "
            Enabled         =   0   'False
            Height          =   1095
            Left            =   4110
            TabIndex        =   94
            Top             =   1320
            Width           =   3885
            Begin MSForms.OptionButton optRiskReqTypeOnPriceProvider 
               Height          =   375
               Left            =   120
               TabIndex        =   47
               Top             =   240
               Width           =   3975
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "7011;661"
               Value           =   "0"
               Caption         =   "Depends on Price Provider"
               GroupName       =   "Risk3"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optRiskReqTypeAlwaysNonGroup 
               Height          =   375
               Left            =   120
               TabIndex        =   48
               Top             =   600
               Width           =   3195
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5636;661"
               Value           =   "1"
               Caption         =   "Always Use Non-group Request"
               GroupName       =   "Risk3"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraRiskBidAskClick 
            Caption         =   " Bid/Ask Column Click "
            Height          =   1095
            Left            =   120
            TabIndex        =   86
            Top             =   120
            Width           =   3885
            Begin MSForms.OptionButton optRiskBidColBuy 
               Height          =   375
               Left            =   120
               TabIndex        =   43
               Top             =   600
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "0"
               Caption         =   "Buy/Sell Trade"
               GroupName       =   "Risk1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optRiskBidColSell 
               Height          =   375
               Left            =   120
               TabIndex        =   42
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "1"
               Caption         =   "Sell/Buy Trade"
               GroupName       =   "Risk1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraRiskOtherColClick 
            Caption         =   " Other Column Click "
            Height          =   1095
            Left            =   120
            TabIndex        =   85
            Top             =   1320
            Width           =   3885
            Begin MSForms.OptionButton optRiskOtherColSell 
               Height          =   375
               Left            =   120
               TabIndex        =   44
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "1"
               Caption         =   "Sell Trade"
               GroupName       =   "Risks2"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optRiskOtherColBuy 
               Height          =   375
               Left            =   120
               TabIndex        =   45
               Top             =   600
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "0"
               Caption         =   "Buy Trade"
               GroupName       =   "Risks2"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraRiskRealtime 
            Caption         =   " Real Time "
            Enabled         =   0   'False
            Height          =   1095
            Left            =   4110
            TabIndex        =   84
            Top             =   120
            Width           =   3885
            Begin MSForms.CheckBox chkRiskDirectlyRealtime 
               Height          =   495
               Left            =   120
               TabIndex        =   46
               Top             =   240
               Width           =   3210
               VariousPropertyBits=   746588177
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5662;873"
               Value           =   "0"
               Caption         =   "Switch to Real Time Mode Directly After Value Selection"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   2
         Left            =   10830
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   77
         TabStop         =   0   'False
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraQuoteRequestType 
            Caption         =   " Market Data Request Type "
            Enabled         =   0   'False
            Height          =   1095
            Left            =   4110
            TabIndex        =   139
            Top             =   2520
            Width           =   3885
            Begin MSForms.OptionButton optQuoteReqTypeAlwaysNonGroup 
               Height          =   375
               Left            =   120
               TabIndex        =   41
               Top             =   600
               Width           =   3195
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5636;661"
               Value           =   "0"
               Caption         =   "Always Use Non-group Request"
               GroupName       =   "Quote4"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteReqTypeOnPriceProvider 
               Height          =   375
               Left            =   120
               TabIndex        =   40
               Top             =   240
               Width           =   3975
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "7011;661"
               Value           =   "1"
               Caption         =   "Depends on Price Provider"
               GroupName       =   "Quote4"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraQuoteFitToImplied 
            Caption         =   " Fit to Implied "
            Height          =   1155
            Left            =   4110
            TabIndex        =   109
            Top             =   1260
            Width           =   3885
            Begin MSForms.CheckBox chkQuoteFitToImpFlatNoBid 
               Height          =   375
               Left            =   120
               TabIndex        =   36
               Top             =   240
               Width           =   3210
               VariousPropertyBits=   746588179
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5662;661"
               Value           =   "0"
               Caption         =   "Use Flat Vola if No Bid"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteFitToImpCurveOTM 
               Height          =   375
               Left            =   2880
               TabIndex        =   39
               Top             =   630
               Width           =   915
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1614;661"
               Value           =   "0"
               Caption         =   "OTM"
               GroupName       =   "Quote3"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteFitToImpCurvePuts 
               Height          =   375
               Left            =   1980
               TabIndex        =   38
               Top             =   630
               Width           =   855
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1508;661"
               Value           =   "0"
               Caption         =   "Puts"
               GroupName       =   "Quote3"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteFitToImpCurveCalls 
               Height          =   375
               Left            =   1140
               TabIndex        =   37
               Top             =   630
               Width           =   915
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1614;661"
               Value           =   "1"
               Caption         =   "Calls"
               GroupName       =   "Quote3"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin VB.Label lblQuoteFitToImpCurveType 
               Caption         =   "Curve Type :"
               Height          =   255
               Left            =   120
               TabIndex        =   110
               Top             =   690
               Width           =   1575
            End
         End
         Begin VB.Frame fraQuoteOptDisplay 
            Caption         =   " Options Display "
            Height          =   735
            Left            =   120
            TabIndex        =   92
            Top             =   1800
            Width           =   3885
            Begin MSForms.CheckBox chkQuoteUseExpirySeparation 
               Height          =   615
               Left            =   120
               TabIndex        =   32
               Top             =   120
               Width           =   2895
               VariousPropertyBits=   746588179
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5106;1085"
               Value           =   "0"
               Caption         =   "Use Colors for Expiries Separation"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraQuoteExch 
            Caption         =   " Initial Visible Exchanges "
            Height          =   1095
            Left            =   4110
            TabIndex        =   89
            Top             =   120
            Width           =   3885
            Begin VB.ComboBox cboQuoteDefExchOpt 
               Height          =   315
               ItemData        =   "frmParams.frx":0228
               Left            =   1050
               List            =   "frmParams.frx":022A
               Sorted          =   -1  'True
               Style           =   2  'Dropdown List
               TabIndex        =   35
               Top             =   660
               Width           =   2535
            End
            Begin VB.ComboBox cboQuoteDefExchUnd 
               Height          =   315
               ItemData        =   "frmParams.frx":022C
               Left            =   1050
               List            =   "frmParams.frx":022E
               Sorted          =   -1  'True
               Style           =   2  'Dropdown List
               TabIndex        =   34
               Top             =   270
               Width           =   2535
            End
            Begin VB.Label lblQuoteDefExchOpt 
               Alignment       =   1  'Right Justify
               Caption         =   "Options :"
               Height          =   255
               Left            =   120
               TabIndex        =   91
               Top             =   675
               Width           =   855
            End
            Begin VB.Label lblQuoteDefExchUnd 
               Alignment       =   1  'Right Justify
               Caption         =   "Underlying :"
               Height          =   255
               Left            =   120
               TabIndex        =   90
               Top             =   315
               Width           =   855
            End
         End
         Begin VB.Frame fraQuoteRealtime 
            Caption         =   " Real Time "
            Height          =   975
            Left            =   120
            TabIndex        =   83
            Top             =   2640
            Width           =   3885
            Begin MSForms.CheckBox chkQuoteDirectlyRealtime 
               Height          =   615
               Left            =   120
               TabIndex        =   33
               Top             =   240
               Width           =   3210
               VariousPropertyBits=   746588179
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5662;1085"
               Value           =   "0"
               Caption         =   "Switch to Real Time Mode Directly After Symbol Selection"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraQuoteOtherColClick 
            Caption         =   " Other Column Click "
            Height          =   735
            Left            =   120
            TabIndex        =   82
            Top             =   960
            Width           =   3885
            Begin MSForms.OptionButton optQuoteOtherColBuy 
               Height          =   375
               Left            =   1920
               TabIndex        =   31
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "0"
               Caption         =   "Buy Trade"
               GroupName       =   "Quote2"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteOtherColSell 
               Height          =   375
               Left            =   120
               TabIndex        =   30
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "1"
               Caption         =   "Sell Trade"
               GroupName       =   "Quote2"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraQuoteBidAskClick 
            Caption         =   " Bid/Ask Column Click "
            Height          =   735
            Left            =   120
            TabIndex        =   81
            Top             =   120
            Width           =   3885
            Begin MSForms.OptionButton optQuoteBidColSell 
               Height          =   375
               Left            =   120
               TabIndex        =   28
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "1"
               Caption         =   "Sell/Buy Trade"
               GroupName       =   "Quote1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optQuoteBidColBuy 
               Height          =   375
               Left            =   1920
               TabIndex        =   29
               Top             =   240
               Width           =   1695
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "2990;661"
               Value           =   "0"
               Caption         =   "Buy/Sell Trade"
               GroupName       =   "Quote1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   1
         Left            =   10530
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   76
         TabStop         =   0   'False
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraPpOpt 
            Caption         =   " Settings "
            Height          =   2775
            Left            =   3360
            TabIndex        =   101
            Top             =   840
            Width           =   4695
            Begin VB.CheckBox chkZeroBidRuleForPnL 
               Caption         =   "When zero Bid use 0 for long and ask/2 for short"
               Height          =   315
               Left            =   240
               TabIndex        =   141
               Top             =   480
               Width           =   4215
            End
            Begin VB.PictureBox picPpOptNoBidOrAsk 
               BorderStyle     =   0  'None
               Height          =   1275
               Left            =   180
               ScaleHeight     =   1275
               ScaleWidth      =   4035
               TabIndex        =   104
               Top             =   810
               Width           =   4035
               Begin VB.OptionButton optPpOptReplaceWithZero 
                  Caption         =   "Replace Missing Bid/Ask with 0"
                  Height          =   255
                  Left            =   240
                  TabIndex        =   25
                  Top             =   960
                  Width           =   3495
               End
               Begin VB.OptionButton optPpOptReplaceWithLast 
                  Caption         =   "Replace missing Bid or Ask with Last"
                  Height          =   255
                  Left            =   240
                  TabIndex        =   22
                  Top             =   240
                  Value           =   -1  'True
                  Width           =   3675
               End
               Begin VB.OptionButton optPpOptReplaceWithOpposite 
                  Caption         =   "Replace missing Bid/Ask with Ask/Bid"
                  Height          =   255
                  Left            =   240
                  TabIndex        =   23
                  Top             =   480
                  Width           =   3675
               End
               Begin VB.OptionButton optPpOptReplaceWithTheo 
                  Caption         =   "Replace missing Bid/Ask with last Theo"
                  Height          =   255
                  Left            =   240
                  TabIndex        =   24
                  Top             =   720
                  Width           =   3675
               End
               Begin VB.Label lblPpOptNoBidOrAsk 
                  Caption         =   "If no Bid or Ask then :"
                  Height          =   195
                  Left            =   0
                  TabIndex        =   105
                  Top             =   0
                  Width           =   3075
               End
            End
            Begin VB.PictureBox picPpOptNoBidAndAsk 
               BorderStyle     =   0  'None
               Height          =   735
               Left            =   240
               ScaleHeight     =   735
               ScaleWidth      =   4035
               TabIndex        =   102
               Top             =   1920
               Width           =   4035
               Begin VB.OptionButton optPpOptBothBadUseLast 
                  Caption         =   "Use Last"
                  Height          =   255
                  Left            =   120
                  TabIndex        =   26
                  Top             =   480
                  Value           =   -1  'True
                  Width           =   1635
               End
               Begin VB.OptionButton optPpOptBothBadUseTheo 
                  Caption         =   "Use last Theo"
                  Height          =   255
                  Left            =   2100
                  TabIndex        =   27
                  Top             =   480
                  Width           =   1695
               End
               Begin VB.Label lblPpOptNoBidAndAsk 
                  Caption         =   "If no Bid and Ask then :"
                  Height          =   195
                  Left            =   0
                  TabIndex        =   103
                  Top             =   240
                  Width           =   3075
               End
            End
            Begin VB.CheckBox chkPpOptNonMidmarketPnL 
               Caption         =   "Use Bid/Ask for Long/Short Positions for P&&L"
               Height          =   315
               Left            =   240
               TabIndex        =   21
               Top             =   480
               Visible         =   0   'False
               Width           =   3735
            End
            Begin VB.Label txtLabel 
               Caption         =   "Use MidMarket for P&&L"
               Height          =   255
               Left            =   180
               TabIndex        =   140
               Top             =   285
               Width           =   3615
            End
         End
         Begin VB.TextBox txtPpDesc 
            Height          =   315
            Left            =   4440
            MaxLength       =   255
            TabIndex        =   16
            Top             =   480
            Width           =   3555
         End
         Begin VB.TextBox txtPpName 
            Height          =   315
            Left            =   4440
            MaxLength       =   50
            TabIndex        =   15
            Top             =   120
            Width           =   3555
         End
         Begin VB.Frame fraPpUnd 
            Caption         =   " Settings "
            Height          =   2775
            Left            =   3360
            TabIndex        =   97
            Top             =   840
            Width           =   4695
            Begin VB.CheckBox chkPpUndCheckTolerance 
               Caption         =   "Use Last Price instead Bid and Ask if Bid or Ask is not within Tolerance range (or no Bid or Ask)"
               Height          =   495
               Left            =   180
               TabIndex        =   20
               Top             =   1200
               Width           =   4335
            End
            Begin VB.OptionButton optPpUndUseMidmarket 
               Caption         =   "Use Midmarket"
               Height          =   255
               Left            =   180
               TabIndex        =   18
               Top             =   600
               Width           =   3975
            End
            Begin VB.CheckBox chkPpUndNonMidmarketPnL 
               Caption         =   "Use Bid/Ask for Long/Short Positions for P&&L"
               Height          =   315
               Left            =   420
               TabIndex        =   19
               Top             =   900
               Width           =   3735
            End
            Begin VB.OptionButton optPpUndAlwaysLast 
               Caption         =   "Always Use Last"
               Height          =   315
               Left            =   180
               TabIndex        =   17
               Top             =   240
               Width           =   3975
            End
            Begin VB.Label lblPpUndNotes 
               BackStyle       =   0  'Transparent
               Caption         =   "NOTE: If no Bid and no Ask then Last will always used"
               ForeColor       =   &H00800000&
               Height          =   315
               Left            =   180
               TabIndex        =   98
               Top             =   2340
               Width           =   4395
            End
         End
         Begin VB.ListBox lstPpProfiles 
            Height          =   3840
            IntegralHeight  =   0   'False
            ItemData        =   "frmParams.frx":0230
            Left            =   120
            List            =   "frmParams.frx":0232
            TabIndex        =   14
            Top             =   120
            Width           =   3015
         End
         Begin VB.Label lblPpDesc 
            Alignment       =   1  'Right Justify
            Caption         =   "Description :"
            Height          =   315
            Left            =   3300
            TabIndex        =   100
            Top             =   510
            Width           =   1095
         End
         Begin VB.Label lblPpName 
            Alignment       =   1  'Right Justify
            Caption         =   "Name :"
            Height          =   315
            Left            =   3300
            TabIndex        =   99
            Top             =   150
            Width           =   1095
         End
      End
      Begin VB.PictureBox picContainer 
         BorderStyle     =   0  'None
         Height          =   3945
         Index           =   0
         Left            =   45
         ScaleHeight     =   3945
         ScaleWidth      =   9795
         TabIndex        =   75
         TabStop         =   0   'False
         Top             =   345
         Width           =   9795
         Begin VB.Frame fraCalcPriceRound 
            Caption         =   " Price Rounding "
            Height          =   1455
            Left            =   120
            TabIndex        =   108
            Top             =   2160
            Width           =   3885
            Begin VB.OptionButton optCalcPriceRoundDown 
               Caption         =   "Round Down to nearest tick"
               Height          =   375
               Left            =   150
               TabIndex        =   6
               Top             =   990
               Width           =   2955
            End
            Begin VB.OptionButton optCalcPriceRoundUp 
               Caption         =   "Round Up to nearest tick"
               Height          =   375
               Left            =   150
               TabIndex        =   5
               Top             =   630
               Width           =   2955
            End
            Begin VB.OptionButton optCalcPriceRoundNone 
               Caption         =   "No rounding"
               Height          =   375
               Left            =   150
               TabIndex        =   4
               Top             =   270
               Value           =   -1  'True
               Width           =   2955
            End
         End
         Begin VB.Frame fraCalcUndPriceTolerance 
            Caption         =   " Underlying Prices Tolerance "
            Height          =   735
            Left            =   120
            TabIndex        =   106
            Top             =   1320
            Width           =   3885
            Begin VB.TextBox txtCalcUndTolerance 
               Height          =   315
               Left            =   1320
               TabIndex        =   3
               Text            =   "10"
               Top             =   300
               Width           =   615
            End
            Begin VB.Label lblCalcUndTolerance 
               Alignment       =   1  'Right Justify
               Caption         =   "Tolerance, % :"
               Height          =   255
               Left            =   60
               TabIndex        =   107
               Top             =   330
               Width           =   1215
            End
         End
         Begin VB.Frame fraCalcModel 
            Caption         =   " Calculation Model "
            Height          =   1035
            Left            =   4110
            TabIndex        =   93
            Top             =   2160
            Width           =   3885
            Begin MSForms.OptionButton optCalcModelOptimized 
               Height          =   375
               Left            =   1740
               TabIndex        =   12
               Top             =   240
               Width           =   1125
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1984;661"
               Value           =   "0"
               Caption         =   "Optimized"
               GroupName       =   "Calc5"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optCalcModelStandard 
               Height          =   375
               Left            =   120
               TabIndex        =   11
               Top             =   240
               Width           =   1035
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1826;661"
               Value           =   "1"
               Caption         =   "Standard"
               GroupName       =   "Calc5"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optCalcModelVskLog 
               Height          =   375
               Left            =   120
               TabIndex        =   13
               Top             =   600
               Width           =   1035
               VariousPropertyBits=   746588177
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "1826;661"
               Value           =   "0"
               Caption         =   "VSK Log"
               GroupName       =   "Calc5"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraCalcVola 
            Caption         =   " Volatility "
            Height          =   1935
            Left            =   4110
            TabIndex        =   88
            Top             =   120
            Width           =   3885
            Begin MSForms.CheckBox chkCalcUseTheoBadMarketVola 
               Height          =   495
               Left            =   360
               TabIndex        =   10
               Top             =   1260
               Width           =   2955
               VariousPropertyBits=   746588177
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5212;873"
               Value           =   "0"
               Caption         =   "Use Theo Vola if Bad Market Vola"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.CheckBox chkCalcUseTheoNoBid 
               Height          =   375
               Left            =   360
               TabIndex        =   9
               Top             =   960
               Width           =   3015
               VariousPropertyBits=   746588177
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               DisplayStyle    =   4
               Size            =   "5318;661"
               Value           =   "0"
               Caption         =   "Use Theo Vola if No Bid"
               FontEffects     =   1073750016
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optCalcUseMarketVola 
               Height          =   375
               Left            =   120
               TabIndex        =   8
               Top             =   600
               Width           =   2505
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "4419;661"
               Value           =   "0"
               Caption         =   "Use Market Volatility"
               GroupName       =   "Calc4"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optCalcUseTheoVola 
               Height          =   375
               Left            =   120
               TabIndex        =   7
               Top             =   240
               Width           =   2325
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "4101;661"
               Value           =   "1"
               Caption         =   "Use Theo Surface"
               GroupName       =   "Calc4"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin VB.Frame fraCalcPnlCalcs 
            Caption         =   " P&&L Calculation Type "
            Height          =   1095
            Left            =   120
            TabIndex        =   80
            Top             =   120
            Width           =   3885
            Begin VB.CheckBox chkUseTheoCloseForPNL 
               Caption         =   "Use Theo Close"
               Height          =   435
               Left            =   1800
               TabIndex        =   158
               Top             =   600
               Width           =   1695
            End
            Begin MSForms.OptionButton optCalcPnlDaily 
               Height          =   375
               Left            =   120
               TabIndex        =   2
               Top             =   600
               Width           =   3015
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5318;661"
               Value           =   "0"
               Caption         =   "Daily"
               GroupName       =   "Calc1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
            Begin MSForms.OptionButton optCalcPnlLTD 
               Height          =   375
               Left            =   120
               TabIndex        =   1
               Top             =   240
               Width           =   3015
               VariousPropertyBits=   746588179
               ForeColor       =   -2147483630
               DisplayStyle    =   5
               Size            =   "5318;661"
               Value           =   "1"
               Caption         =   "LTD"
               GroupName       =   "Calc1"
               FontHeight      =   165
               FontCharSet     =   204
               FontPitchAndFamily=   2
            End
         End
         Begin MSForms.CheckBox chkCalcUseTimePrecision 
            Height          =   255
            Left            =   4110
            TabIndex        =   159
            Top             =   3360
            Width           =   3855
            BackColor       =   -2147483633
            ForeColor       =   -2147483630
            DisplayStyle    =   4
            Size            =   "6800;450"
            Value           =   "1"
            Caption         =   "Use Time Precision"
            FontHeight      =   165
            FontCharSet     =   204
            FontPitchAndFamily=   2
         End
      End
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Default         =   -1  'True
      Height          =   315
      Left            =   5400
      TabIndex        =   73
      Top             =   4440
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   6840
      TabIndex        =   74
      Top             =   4440
      Width           =   1335
   End
End
Attribute VB_Name = "frmParams"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_CalcSleepFreq As Long
Private m_CalcSleepAmt As Long
Private m_bDirty As Boolean
Private m_bToleranceDirty As Boolean
Private m_bRoundingRuleDirty As Boolean
Private m_bPriceProfileDirty As Boolean

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bInitializing As Boolean

Private m_nOrdStkIncrQty&
Private m_dOrdStkIncrPrice#
Private m_nOrdOptIncrQty&
Private m_dOrdOptIncrPriceCheap#
Private m_dOrdOptIncrPriceExpensive#

Private m_nCrdIncrQty&
Private m_dCrdIncrPriceCheap#
Private m_dCrdIncrPriceExpensive#

Private m_bAUMChange As Boolean

Private m_dUndPriceToleranceValue As Double
Private m_nSOQ_EADaysToDivDays As Long

Private m_PriceProfile As New EtsGeneralLib.EtsPriceProfileColl

Public Sub Execute(ByRef frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then
        Me.SetFocus
        Exit Sub
    End If
    
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    m_bAUMChange = False
    Load Me
    LoadData
    m_bDirty = False
    m_bPriceProfileDirty = False

    Me.Show vbModeless, m_frmOwner
    
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open parameters dialog."
    On Error Resume Next
    Unload Me
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
End Sub

Private Sub cboAdvDefTrader_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub cboAdvCalcDefExchOpt_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub cboAdvCalcDefExchUnd_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkCalcUseTimePrecision_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkCheckForExp_Click()
    On Error Resume Next
    txtExpDays.Enabled = chkCheckForExp.Value
    m_bDirty = True
End Sub

Private Sub chkRTEditMsg_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkUseTheoCloseForPNL_Click()
    m_bDirty = True
End Sub

Private Sub ckOptWithPos_Click()
    g_Params.EAOptWithPosition = ckOptWithPos.Value
    m_bDirty = True
End Sub

Private Sub chkCrdUseTnt_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkQuoteFitToImpFlatNoBid_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkZeroBidRuleForPnL_Click()
On Error Resume Next
    If m_bInitializing Then Exit Sub
    
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.UseZeroBidRule = chkZeroBidRuleForPnL.Value
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub entAUM_Change()
    On Error Resume Next
    m_bAUMChange = True
    m_bDirty = True
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
    vsTabs.CurrTab = 0
    
    fraPpUnd.Left = 3300
    fraPpUnd.Top = 840
    fraPpOpt.Left = fraPpUnd.Left
    fraPpOpt.Top = fraPpUnd.Top
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    m_PriceProfile.Clear
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
    
    m_PriceProfile.Clear
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub LoadData()
    On Error Resume Next
    m_bInitializing = True
    
    chkUseTheoCloseForPNL.Value = IIf(g_Params.UseTheoCloseForPNL, 1, 0)
    
    optCalcPnlDaily.Value = IIf(g_Params.PnLCalcType = PNLCT_DAILY, 1, 0)
    optCalcPnlLTD.Value = Not optCalcPnlDaily.Value
    
    LogParameter "CalcPnlDaily", optCalcPnlDaily.Value, True
    LogParameter "CalcPnlLTD", optCalcPnlLTD.Value, True
    
    m_dUndPriceToleranceValue = g_Params.UndPriceToleranceValue * 100#
    txtCalcUndTolerance.Text = CStr(m_dUndPriceToleranceValue)
    
    LogParameter "CalcUndTolerance", txtCalcUndTolerance.Text, True
    
    optCalcPriceRoundNone.Value = (g_Params.PriceRoundingRule = enPrrRoundNone)
    optCalcPriceRoundUp.Value = (g_Params.PriceRoundingRule = enPrrRoundUp)
    optCalcPriceRoundDown.Value = (g_Params.PriceRoundingRule = enPrrRoundDown)
    
    chkRTEditMsg.Value = IIf(g_Params.ShowMessageUnableToEdit, vbChecked, vbUnchecked)
    
    LogParameter "CalcPriceRoundNone", optCalcPriceRoundNone.Value, True
    LogParameter "CalcPriceRoundUp", optCalcPriceRoundUp.Value, True
    LogParameter "CalcPriceRoundDown", optCalcPriceRoundDown.Value, True

    If g_Params.NetExposureAUM > BAD_DOUBLE_VALUE And g_Params.NetExposureAUM <> 0 Then _
        entAUM.Text = g_Params.NetExposureAUM
        
    optCalcUseTheoVola.Value = IIf(g_Params.UseTheoVolatility, 1, 0)
    optCalcUseMarketVola.Value = Not optCalcUseTheoVola.Value
    chkCalcUseTheoNoBid.Value = IIf(g_Params.UseTheoNoBid, 1, 0)
    chkCalcUseTheoNoBid.Enabled = optCalcUseMarketVola.Value
    chkCalcUseTheoBadMarketVola.Value = IIf(g_Params.UseTheoBadMarketVola, 1, 0)
    chkCalcUseTheoBadMarketVola.Enabled = optCalcUseMarketVola.Value
    chkCalcUseTimePrecision.Value = IIf(g_Params.UseTTE, 1, 0)
    
    LogParameter "CalcUseTheoVola", optCalcUseTheoVola.Value, True
    LogParameter "CalcUseMarketVola", optCalcUseMarketVola.Value, True
    LogParameter "CalcUseTheoNoBid", chkCalcUseTheoNoBid.Value, True
    LogParameter "CalcUseTheoBadMarketVola", chkCalcUseTheoBadMarketVola.Value, True

    optCalcModelStandard.Value = IIf(g_Params.CalcModel = enCmBinomial, 1, 0)
    optCalcModelOptimized.Value = IIf(g_Params.CalcModel = enCmBinomialOpt, 1, 0)
    optCalcModelVskLog.Value = IIf(g_Params.CalcModel = enCmVskLog, 1, 0)
    
    LogParameter "CalcModelStandard", optCalcModelStandard.Value, True
    LogParameter "CalcModelOptimized", optCalcModelOptimized.Value, True
    LogParameter "CalcModelVskLog", optCalcModelVskLog.Value, True

    optQuoteBidColBuy.Value = IIf(g_Params.QuoteBidColBuy <> 0, 1, 0)
    optQuoteOtherColBuy.Value = IIf(g_Params.QuoteOtherColBuy <> 0, 1, 0)
    chkQuoteDirectlyRealtime.Value = IIf(g_Params.QuoteDirectlyToRealtime, 1, 0)
    
    LogParameter "QuoteBidColBuy", optQuoteBidColBuy.Value, True
    LogParameter "QuoteOtherColBuy", optQuoteOtherColBuy.Value, True
    LogParameter "QuoteDirectlyRealtime", chkQuoteDirectlyRealtime.Value, True

    chkQuoteUseExpirySeparation.Value = IIf(g_Params.QuoteOptExpirySeparation, 1, 0)
    LogParameter "QuoteUseExpirySeparation", chkQuoteUseExpirySeparation.Value, True
    
    chkQuoteFitToImpFlatNoBid.Value = IIf(g_Params.QuoteFitToImpFlatNoBid, 1, 0)
    LogParameter "QuoteFitToImpFlatNoBid", chkQuoteFitToImpFlatNoBid.Value, True

    optQuoteFitToImpCurveCalls.Value = (g_Params.QuoteFitToImpCurveType = enFitToImpCurveCalls)
    optQuoteFitToImpCurvePuts.Value = (g_Params.QuoteFitToImpCurveType = enFitToImpCurvePuts)
    optQuoteFitToImpCurveOTM.Value = (g_Params.QuoteFitToImpCurveType = enFitToImpCurveOTM)
    
    LogParameter "QuoteFitToImpCurveCalls", optQuoteFitToImpCurveCalls.Value, True
    LogParameter "QuoteFitToImpCurvePuts", optQuoteFitToImpCurvePuts.Value, True
    LogParameter "QuoteFitToImpCurveOTM", optQuoteFitToImpCurveOTM.Value, True

    optQuoteReqTypeAlwaysNonGroup.Value = IIf(g_Params.QuoteReqTypeAlwaysNonGroup, 1, 0)
    optQuoteReqTypeOnPriceProvider.Value = IIf(g_Params.QuoteReqTypeAlwaysNonGroup, 0, 1)
    
    LogParameter "QuoteReqTypeAlwaysNonGroup", optQuoteReqTypeAlwaysNonGroup.Value, True
    LogParameter "QuoteReqTypeOnPriceProvider", optQuoteReqTypeOnPriceProvider.Value, True
    
    optRiskBidColBuy.Value = IIf(g_Params.RiskBidColBuy <> 0, 1, 0)
    optRiskOtherColBuy.Value = IIf(g_Params.RiskOtherColBuy <> 0, 1, 0)
    chkRiskDirectlyRealtime.Value = IIf(g_Params.RiskDirectlyToRealtime, 1, 0)
    
    LogParameter "RiskBidColBuy", optRiskBidColBuy.Value, True
    LogParameter "RiskOtherColBuy", optRiskOtherColBuy.Value, True
    LogParameter "RiskDirectlyRealtime", chkRiskDirectlyRealtime.Value, True
    
    optRiskReqTypeAlwaysNonGroup.Value = IIf(g_Params.RiskReqTypeAlwaysNonGroup, 1, 0)
    optRiskReqTypeOnPriceProvider.Value = IIf(g_Params.RiskReqTypeAlwaysNonGroup, 0, 1)
    
    LogParameter "RiskReqTypeAlwaysNonGroup", optRiskReqTypeAlwaysNonGroup.Value, True
    LogParameter "RiskReqTypeOnPriceProvider", optRiskReqTypeOnPriceProvider.Value, True
    
    optMatrixExerciseOpts.Value = IIf(g_Params.MatrixExerciseOptions, 1, 0)
    optMatrixIgnoreOpts.Value = Not optMatrixExerciseOpts.Value
    optMatrixReqTypeAlwaysNonGroup.Value = IIf(g_Params.MatrixReqTypeAlwaysNonGroup, 1, 0)
    optMatrixReqTypeOnPriceProvider.Value = IIf(g_Params.MatrixReqTypeAlwaysNonGroup, 0, 1)
    
    LogParameter "MatrixExerciseOpts", optMatrixExerciseOpts.Value, True
    LogParameter "MatrixIgnoreOpts", optMatrixIgnoreOpts.Value, True
    LogParameter "MatrixReqTypeAlwaysNonGroup", optMatrixReqTypeAlwaysNonGroup.Value, True
    LogParameter "MatrixReqTypeOnPriceProvider", optMatrixReqTypeOnPriceProvider.Value, True
    
    chkAdvActiveRealTime.Value = IIf(g_Params.ActiveRealTime, 1, 0)
    chkAdvIvRealTime.Value = IIf(g_Params.CalcIvInRuntime, 1, 0)
    
    LogParameter "AdvActiveRealTime", chkAdvActiveRealTime.Value, True
    LogParameter "AdvIvRealTime", chkAdvIvRealTime.Value, True
    
    
    Dim aExch As EtsGeneralLib.ExchAtom, nCount&, i&
    cboQuoteDefExchUnd.Clear
    cboQuoteDefExchUnd.AddItem "<All>"
    cboQuoteDefExchUnd.ItemData(cboQuoteDefExchUnd.NewIndex) = 0
    cboQuoteDefExchUnd.ListIndex = 0
    
    cboQuoteDefExchOpt.Clear
    cboQuoteDefExchOpt.AddItem "<All>"
    cboQuoteDefExchOpt.ItemData(cboQuoteDefExchUnd.NewIndex) = 0
    cboQuoteDefExchOpt.ListIndex = 0
    
    cboAdvCalcDefExchUnd.Clear
    cboAdvCalcDefExchOpt.Clear
    
    For Each aExch In g_Exch
        
        If aExch.IsUnderlying <> 0 Then
        
            LogParameter "Underlying Exchange with id " & CStr(aExch.ID), aExch.Name, True
        
            cboQuoteDefExchUnd.AddItem aExch.Name
            cboQuoteDefExchUnd.ItemData(cboQuoteDefExchUnd.NewIndex) = aExch.ID
            If Not g_Params.QuoteUndExchAll Then
                If g_Params.QuoteDefUndExchID = aExch.ID Then cboQuoteDefExchUnd.ListIndex = cboQuoteDefExchUnd.NewIndex
            End If
        
            cboAdvCalcDefExchUnd.AddItem aExch.Name
            cboAdvCalcDefExchUnd.ItemData(cboAdvCalcDefExchUnd.NewIndex) = aExch.ID
            If g_Params.CalcDefUndExchID = aExch.ID Then cboAdvCalcDefExchUnd.ListIndex = cboAdvCalcDefExchUnd.NewIndex
        End If
        
        If aExch.IsOption <> 0 Then
        
            LogParameter "Option Exchange with id " & CStr(aExch.ID), aExch.Name, True
            
            cboQuoteDefExchOpt.AddItem aExch.Name
            cboQuoteDefExchOpt.ItemData(cboQuoteDefExchOpt.NewIndex) = aExch.ID
            If Not g_Params.QuoteOptExchAll Then
                If g_Params.QuoteDefOptExchID = aExch.ID Then cboQuoteDefExchOpt.ListIndex = cboQuoteDefExchOpt.NewIndex
            End If
        
            cboAdvCalcDefExchOpt.AddItem aExch.Name
            cboAdvCalcDefExchOpt.ItemData(cboAdvCalcDefExchOpt.NewIndex) = aExch.ID
            If g_Params.CalcDefOptExchID = aExch.ID Then cboAdvCalcDefExchOpt.ListIndex = cboAdvCalcDefExchOpt.NewIndex
        End If
    Next
    
    Dim rs As ADODB.Recordset, nID&, sName$
    
    cboAdvDefTrader.Clear
    cboAdvDefTrader.AddItem "<No Default Trader>"
    cboAdvDefTrader.ItemData(cboAdvDefTrader.NewIndex) = 0
    cboAdvDefTrader.ListIndex = cboAdvDefTrader.NewIndex
    
    cboAdvDefTrader.AddItem "<All Traders>"
    cboAdvDefTrader.ItemData(cboAdvDefTrader.NewIndex) = 0
    If g_Params.UseDefTrader And g_Params.DefTraderID = 0 Then cboAdvDefTrader.ListIndex = cboAdvDefTrader.NewIndex
    
    Set rs = gDBW.usp_Trader_Get(0)
    While Not rs.EOF
        nID = ReadLng(rs!iTraderID)
        sName = Trim$(ReadStr(rs!vcAcronym))
        
        LogParameter "Trader with id " & CStr(nID), sName, True
        
        i = i + 1
        cboAdvDefTrader.AddItem sName
        cboAdvDefTrader.ItemData(cboAdvDefTrader.NewIndex) = nID
        
        If g_Params.UseDefTrader Then
            If g_Params.DefTraderID = nID Then cboAdvDefTrader.ListIndex = cboAdvDefTrader.NewIndex
        End If
        
        rs.MoveNext
    Wend
    Set rs = Nothing
    
    LogParameter "DefTraderID", g_Params.DefTraderID, True
    
    m_CalcSleepAmt = g_Params.CalcSleepAmt
    m_CalcSleepFreq = g_Params.CalcSleepFreq
    
    chkAdvUseCalcSleep.Value = IIf(g_Params.UseCalcSleep, 1, 0)
    chkAdvUseCalcSleep_Click
    txtAdvCalcSleepAmt.Text = m_CalcSleepAmt
    txtAdvCalcSleepFreq.Text = m_CalcSleepFreq
    
    LogParameter "AdvUseCalcSleep", chkAdvUseCalcSleep.Value, True
    LogParameter "AdvCalcSleepAmt", txtAdvCalcSleepAmt.Text, True
    LogParameter "AdvCalcSleepFreq", txtAdvCalcSleepFreq.Text, True
    
    optAdvActTrdEditable.Value = IIf(g_Params.ActualTradesEditable <> 0, 1, 0)
    optAdvActTrdProtected.Value = IIf(g_Params.ActualTradesEditable <> 0, 0, 1)
    
    LogParameter "AdvActTrdEditable", optAdvActTrdEditable.Value, True
    LogParameter "AdvActTrdProtected", optAdvActTrdProtected.Value, True
    
    optDefNewTrdSimulated.Value = IIf(g_Params.DefTradeType = enTsSimulated Or g_Params.ActualTradesEditable = 0, 1, 0)
    optDefNewTrdManual.Value = IIf(g_Params.DefTradeType = enTsManual, 1, 0)
    optDefNewTrdActual.Value = IIf(g_Params.DefTradeType = enTrsRealtime And g_Params.ActualTradesEditable, 1, 0)
    optDefNewTrdActual.Enabled = g_Params.ActualTradesEditable
    optDefNewTrdLastEntered.Value = IIf(g_Params.DefTradeType = -1, 1, 0)
    
    LogParameter "DefNewTrdSimulated", optDefNewTrdSimulated.Value, True
    LogParameter "DefNewTrdManual", optDefNewTrdManual.Value, True
    LogParameter "DefNewTrdActual", optDefNewTrdActual.Value, True
    LogParameter "DefNewTrdLastEntered", optDefNewTrdLastEntered.Value, True
    
    m_nOrdStkIncrQty = g_Params.OrdDefStkQtyIncrement
    m_dOrdStkIncrPrice = g_Params.OrdDefStkPrcIncrement
    
    m_nOrdOptIncrQty = g_Params.OrdDefOptQtyIncrement
    m_dOrdOptIncrPriceCheap = g_Params.OrdDefOptPrcIncrementCheap
    m_dOrdOptIncrPriceExpensive = g_Params.OrdDefOptPrcIncrementExpensive
    
    txtOrdStkIncrQty.Text = m_nOrdStkIncrQty
    txtOrdStkIncrPrice.Text = m_dOrdStkIncrPrice
    
    LogParameter "OrdStkIncrQty", txtOrdStkIncrQty.Text, True
    LogParameter "OrdStkIncrPrice", txtOrdStkIncrPrice.Text, True
    
    txtOrdOptIncrQty.Text = m_nOrdOptIncrQty
    txtOrdOptIncrPriceCheap.Text = m_dOrdOptIncrPriceCheap
    txtOrdOptIncrPriceExpensive.Text = m_dOrdOptIncrPriceExpensive
    
    LogParameter "OrdOptIncrQty", txtOrdOptIncrQty.Text, True
    LogParameter "OrdOptIncrPriceCheap", txtOrdOptIncrPriceCheap.Text, True
    LogParameter "OrdOptIncrPriceExpensive", txtOrdOptIncrPriceExpensive.Text, True
    
    m_nCrdIncrQty = g_Params.CrdDefQtyIncrement
    m_dCrdIncrPriceCheap = g_Params.CrdDefPrcIncrementCheap
    m_dCrdIncrPriceExpensive = g_Params.CrdDefPrcIncrementExpensive
    
    txtCrdIncrQty.Text = m_nCrdIncrQty
    txtCrdIncrPriceCheap.Text = m_dCrdIncrPriceCheap
    txtCrdIncrPriceExpensive.Text = m_dCrdIncrPriceExpensive
    
    LogParameter "CrdIncrQty", txtCrdIncrQty.Text, True
    LogParameter "CrdIncrPriceCheap", txtCrdIncrPriceCheap.Text, True
    LogParameter "CrdIncrPriceExpensive", txtCrdIncrPriceExpensive.Text, True
    
    ckOptWithPos.Value = g_Params.EAOptWithPosition
    ctlDaysToDiv.Enabled = g_Params.EAUpcomingDivs
    optUncOn = g_Params.EAUpcomingDivs
    optUncOff = Not optUncOn
    ctlDaysToDiv.Text = g_Params.EADaysToDiv
    
    LogParameter "OptionsWithPositionsOnly", ckOptWithPos.Value, True
    LogParameter "UpcomingDivDays", optUncOn, True
    LogParameter "DaysToDiv", ctlDaysToDiv.Text, True
    
    If g_Params.SOQ_CheckExpiredOptions Then
        chkCheckForExp.Value = 1
        txtExpDays.Enabled = True
    Else
        chkCheckForExp.Value = 0
        txtExpDays.Enabled = False
    End If

    m_nSOQ_EADaysToDivDays = g_Params.SOQ_EADaysToDivDays
    txtExpDays.Text = m_nSOQ_EADaysToDivDays
    
    LogParameter "CheckExpiredOptions", IIf(chkCheckForExp.Value <> 0, "Yes", "No"), True
    LogParameter "EADaysToExpiry", txtExpDays.Text, True

    vsTabs.TabVisible(5) = False
    
    LoadPriceProfiles
    
    m_bInitializing = False
End Sub

Private Sub SaveData()
    On Error Resume Next
    Dim sValue$, nID&

    g_Params.PnLCalcType = IIf(optCalcPnlDaily.Value <> 0, PNLCT_DAILY, PNLCT_LTD)
    g_Params.UseTheoVolatility = IIf(optCalcUseTheoVola.Value <> 0, 1, 0)
    g_Params.UseTheoNoBid = (chkCalcUseTheoNoBid.Value <> 0)
    g_Params.UseTheoBadMarketVola = (chkCalcUseTheoBadMarketVola.Value <> 0)
    g_Params.NetExposureAUM = CDbl(entAUM.Text)
    g_Params.ShowMessageUnableToEdit = IIf(chkRTEditMsg.Value = vbChecked, True, False)
    g_Params.UseTTE = IIf(chkCalcUseTimePrecision.Value = True, True, False)
    g_Main.CalculationParametrs.UseTimePrecision = g_Params.UseTTE
    
    If (Not g_Main Is Nothing) Then
        g_Main.UseTheoCloseForPNL = IIf(chkUseTheoCloseForPNL.Value = vbChecked, True, False)
        g_Params.UseTheoCloseForPNL = g_Main.UseTheoCloseForPNL
    End If
    
    LogParameter "PnLCalcType", g_Params.PnLCalcType, False
    LogParameter "UseTheoVolatility", g_Params.UseTheoVolatility, False
    LogParameter "UseTheoNoBid", g_Params.UseTheoNoBid, False
    LogParameter "UseTheoBadMarketVola", g_Params.UseTheoBadMarketVola, False
    
    If cboAdvCalcDefExchUnd.ListIndex < 0 Then
        g_Params.CalcDefUndExchID = 0
    Else
        nID = cboAdvCalcDefExchUnd.ItemData(cboAdvCalcDefExchUnd.ListIndex)
        g_Params.CalcDefUndExchID = nID
    End If
    LogParameter "CalcDefUndExchID", g_Params.CalcDefUndExchID, False
   
    If cboAdvCalcDefExchOpt.ListIndex < 0 Then
        g_Params.CalcDefOptExchID = 0
    Else
        nID = cboAdvCalcDefExchOpt.ItemData(cboAdvCalcDefExchOpt.ListIndex)
        g_Params.CalcDefOptExchID = nID
    End If
    LogParameter "CalcDefOptExchID", g_Params.CalcDefOptExchID, False
   
    g_Params.UseCalcSleep = (chkAdvUseCalcSleep.Value <> 0)
    g_Params.CalcSleepAmt = m_CalcSleepAmt
    g_Params.CalcSleepFreq = m_CalcSleepFreq
    
    LogParameter "UseCalcSleep", g_Params.UseCalcSleep, False
    LogParameter "CalcSleepAmt", g_Params.CalcSleepAmt, False
    LogParameter "CalcSleepFreq", g_Params.CalcSleepFreq, False
    
    g_Params.QuoteBidColBuy = IIf(optQuoteBidColBuy.Value <> 0, 1, 0)
    g_Params.QuoteOtherColBuy = IIf(optQuoteOtherColBuy.Value <> 0, 1, 0)
    g_Params.QuoteDirectlyToRealtime = chkQuoteDirectlyRealtime.Value
    g_Params.QuoteOptExpirySeparation = chkQuoteUseExpirySeparation.Value
    
    LogParameter "QuoteBidColBuy", g_Params.QuoteBidColBuy, False
    LogParameter "QuoteOtherColBuy", g_Params.QuoteOtherColBuy, False
    LogParameter "QuoteDirectlyToRealtime", g_Params.QuoteDirectlyToRealtime, False
    LogParameter "QuoteOptExpirySeparation", g_Params.QuoteOptExpirySeparation, False

    If cboQuoteDefExchUnd.ListIndex < 0 Then
        g_Params.QuoteDefUndExchID = 0
        g_Params.QuoteUndExchAll = True
    Else
        nID = cboQuoteDefExchUnd.ItemData(cboQuoteDefExchUnd.ListIndex)
        sValue = cboQuoteDefExchUnd.List(cboQuoteDefExchUnd.ListIndex)
        
        If nID <> 0 Then
            g_Params.QuoteUndExchAll = False
            g_Params.QuoteDefUndExchID = nID
        Else
            If sValue = "<All>" Then
                g_Params.QuoteUndExchAll = True
            Else
                g_Params.QuoteUndExchAll = False
            End If
            g_Params.QuoteDefUndExchID = 0
        End If
    End If
    
    LogParameter "QuoteDefUndExchID", g_Params.QuoteDefUndExchID, False
    LogParameter "QuoteUndExchAll", g_Params.QuoteUndExchAll, False

    If cboQuoteDefExchOpt.ListIndex <= 0 Then
        g_Params.QuoteDefOptExchID = 0
        g_Params.QuoteOptExchAll = True
    Else
        nID = cboQuoteDefExchOpt.ItemData(cboQuoteDefExchOpt.ListIndex)
        sValue = cboQuoteDefExchOpt.List(cboQuoteDefExchOpt.ListIndex)
        
        If nID <> 0 Then
            g_Params.QuoteOptExchAll = False
            g_Params.QuoteDefOptExchID = nID
        Else
            If sValue = "<All>" Then
                g_Params.QuoteOptExchAll = True
            Else
                g_Params.QuoteOptExchAll = False
            End If
            g_Params.QuoteDefOptExchID = 0
        End If
    End If
    
    LogParameter "QuoteDefOptExchID", g_Params.QuoteDefOptExchID, False
    LogParameter "QuoteOptExchAll", g_Params.QuoteOptExchAll, False

    g_Params.QuoteFitToImpFlatNoBid = (chkQuoteFitToImpFlatNoBid.Value <> 0)
    g_Params.QuoteFitToImpCurveType = IIf(optQuoteFitToImpCurveCalls.Value <> 0, enFitToImpCurveCalls, _
                                        IIf(optQuoteFitToImpCurvePuts.Value <> 0, enFitToImpCurvePuts, enFitToImpCurveOTM))

    g_Params.QuoteReqTypeAlwaysNonGroup = (optQuoteReqTypeAlwaysNonGroup.Value <> 0)
    
    LogParameter "QuoteFitToImpFlatNoBid", g_Params.QuoteFitToImpFlatNoBid, False
    LogParameter "QuoteFitToImpCurveType", g_Params.QuoteFitToImpCurveType, False
    LogParameter "QuoteReqTypeAlwaysNonGroup", g_Params.QuoteReqTypeAlwaysNonGroup, False

    g_Params.RiskBidColBuy = IIf(optRiskBidColBuy.Value <> 0, 1, 0)
    g_Params.RiskOtherColBuy = IIf(optRiskOtherColBuy.Value <> 0, 1, 0)
    g_Params.RiskDirectlyToRealtime = chkRiskDirectlyRealtime.Value
    g_Params.RiskReqTypeAlwaysNonGroup = (optRiskReqTypeAlwaysNonGroup.Value <> 0)
    
    LogParameter "RiskBidColBuy", g_Params.RiskBidColBuy, False
    LogParameter "RiskOtherColBuy", g_Params.RiskOtherColBuy, False
    LogParameter "RiskDirectlyToRealtime", g_Params.RiskDirectlyToRealtime, False
    LogParameter "RiskReqTypeAlwaysNonGroup", g_Params.RiskReqTypeAlwaysNonGroup, False
    
    g_Params.MatrixExerciseOptions = IIf(optMatrixExerciseOpts.Value <> 0, 1, 0)
    g_Params.MatrixReqTypeAlwaysNonGroup = (optMatrixReqTypeAlwaysNonGroup.Value <> 0)
    
    LogParameter "MatrixExerciseOptions", g_Params.MatrixExerciseOptions, False
    LogParameter "MatrixReqTypeAlwaysNonGroup", g_Params.MatrixReqTypeAlwaysNonGroup, False
    
    g_Params.CalcIvInRuntime = chkAdvIvRealTime.Value
    g_Params.ActiveRealTime = chkAdvActiveRealTime.Value
    
    LogParameter "CalcIvInRuntime", g_Params.CalcIvInRuntime, False
    LogParameter "ActiveRealTime", g_Params.ActiveRealTime, False

    g_Params.CalcModel = IIf(optCalcModelStandard.Value <> 0, enCmBinomial, IIf(optCalcModelOptimized.Value <> 0, enCmBinomialOpt, enCmVskLog))
    LogParameter "CalcModel", g_Params.CalcModel, False

    If cboAdvDefTrader.ListIndex < 0 Then
        g_Params.UseDefTrader = False
        g_Params.DefTraderID = 0
    Else
        nID = cboAdvDefTrader.ItemData(cboAdvDefTrader.ListIndex)
        sValue = cboAdvDefTrader.List(cboAdvDefTrader.ListIndex)
        
        If nID <> 0 Then
            g_Params.UseDefTrader = True
            g_Params.DefTraderID = nID
        Else
            If sValue = "<All Traders>" Then
                g_Params.UseDefTrader = True
                g_Params.DefTraderID = 0
            Else
                g_Params.UseDefTrader = False
            End If
        End If
    End If
    
    LogParameter "UseDefTrader", g_Params.UseDefTrader, False
    LogParameter "DefTraderID", g_Params.DefTraderID, False

    g_Params.SaveUndPriceToleranceValue m_dUndPriceToleranceValue / 100#
    g_Params.SavePriceRoundingRule IIf(optCalcPriceRoundUp.Value, enPrrRoundUp, IIf(optCalcPriceRoundDown.Value, enPrrRoundDown, enPrrRoundNone))
    
    If g_Params.ActualTradesEditable <> (optAdvActTrdEditable.Value <> 0) Then
        g_Params.SaveActualTradesEditable (optAdvActTrdEditable.Value <> 0)
    End If
    
    g_Params.DefTradeType = IIf(optDefNewTrdSimulated.Value <> 0, enTsSimulated, _
        IIf(optDefNewTrdManual.Value <> 0, enTsManual, _
        IIf(optDefNewTrdActual.Value <> 0, enTsRealtime, -1)))
        
    LogParameter "DefTradeType", g_Params.DefTradeType, False

    g_Params.OrdDefStkQtyIncrement = m_nOrdStkIncrQty
    g_Params.OrdDefStkPrcIncrement = m_dOrdStkIncrPrice
    
    LogParameter "OrdDefStkQtyIncrement", g_Params.OrdDefStkQtyIncrement, False
    LogParameter "OrdDefStkPrcIncrement", g_Params.OrdDefStkPrcIncrement, False
    
    g_Params.OrdDefOptQtyIncrement = m_nOrdOptIncrQty
    g_Params.OrdDefOptPrcIncrementCheap = m_dOrdOptIncrPriceCheap
    g_Params.OrdDefOptPrcIncrementExpensive = m_dOrdOptIncrPriceExpensive
    
    LogParameter "OrdDefOptQtyIncrement", g_Params.OrdDefOptQtyIncrement, False
    LogParameter "OrdDefOptPrcIncrementCheap", g_Params.OrdDefOptPrcIncrementCheap, False
    LogParameter "OrdDefOptPrcIncrementExpensive", g_Params.OrdDefOptPrcIncrementExpensive, False

    g_Params.CrdDefQtyIncrement = m_nCrdIncrQty
    g_Params.CrdDefPrcIncrementCheap = m_dCrdIncrPriceCheap
    g_Params.CrdDefPrcIncrementExpensive = m_dCrdIncrPriceExpensive
    
    LogParameter "CrdDefQtyIncrement", g_Params.CrdDefQtyIncrement, False
    LogParameter "CrdDefPrcIncrementCheap", g_Params.CrdDefPrcIncrementCheap, False
    LogParameter "CrdDefPrcIncrementExpensive", g_Params.CrdDefPrcIncrementExpensive, False
    
    LogParameter "OptionsWithPositionsOnly", g_Params.EAOptWithPosition, False
    LogParameter "UpcomingDivDays", g_Params.EAUpcomingDivs, False
    LogParameter "DaysToDiv", g_Params.EADaysToDiv, False
    
    g_Params.SOQ_CheckExpiredOptions = CBool(chkCheckForExp.Value)
    g_Params.SOQ_EADaysToDivDays = m_nSOQ_EADaysToDivDays
    
    LogParameter "CheckExpiredOptions", IIf(g_Params.SOQ_CheckExpiredOptions <> 0, "Yes", "No"), False
    LogParameter "EADaysToExpiry", CStr(g_Params.SOQ_EADaysToDivDays), False
    
    frmMain.UpdateSettings
    
End Sub

Private Sub CopyPriceProfile(ByRef aSrc As EtsGeneralLib.EtsPriceProfileAtom, _
                            ByRef aDest As EtsGeneralLib.EtsPriceProfileAtom)
    On Error Resume Next
    aDest.ID = aSrc.ID
    aDest.Name = aSrc.Name
    aDest.Description = aSrc.Description
    aDest.DefaultID = aSrc.DefaultID
    aDest.IsOptionProfile = aSrc.IsOptionProfile
    aDest.AlwaysUseLast = aSrc.AlwaysUseLast
    aDest.UseMidMarketForPnL = aSrc.UseMidMarketForPnL
    aDest.BadUndPriceRule = aSrc.BadUndPriceRule
    aDest.BadOptSinglePriceRule = aSrc.BadOptSinglePriceRule
    aDest.BadOptBothPriceRule = aSrc.BadOptBothPriceRule
    aDest.UseZeroBidRule = aSrc.UseZeroBidRule
    
    LogParameter "PriceProfile ID", aDest.ID, True
    LogParameter "PriceProfile Name", aDest.Name, True
    LogParameter "PriceProfile Description", aDest.Description, True
    LogParameter "PriceProfile DefaultID", aDest.DefaultID, True
    LogParameter "PriceProfile IsOptionProfile", aDest.IsOptionProfile, True
    LogParameter "PriceProfile AlwaysUseLast", aDest.AlwaysUseLast, True
    LogParameter "PriceProfile UseMidMarketForPnL", aDest.UseMidMarketForPnL, True
    LogParameter "PriceProfile BadUndPriceRule", aDest.BadUndPriceRule, True
    LogParameter "PriceProfile BadOptSinglePriceRule", aDest.BadOptSinglePriceRule, True
    LogParameter "PriceProfile BadOptBothPriceRule", aDest.BadOptBothPriceRule, True
    LogParameter "PriceProfile UseZeroBidRule", aDest.UseZeroBidRule, True
    
End Sub

Private Function IsPriceProfilesEqual(ByRef aSrc As EtsGeneralLib.EtsPriceProfileAtom, _
                                    ByRef aDest As EtsGeneralLib.EtsPriceProfileAtom) As Boolean
    On Error Resume Next
    IsPriceProfilesEqual = Not (Trim$(aSrc.Name) <> Trim$(aDest.Name) _
                            Or Trim$(aSrc.Description) <> Trim$(aDest.Description) _
                            Or aSrc.DefaultID <> aDest.DefaultID _
                            Or aSrc.IsOptionProfile <> aDest.IsOptionProfile _
                            Or aSrc.AlwaysUseLast <> aDest.AlwaysUseLast _
                            Or aSrc.UseMidMarketForPnL <> aDest.UseMidMarketForPnL _
                            Or aSrc.BadUndPriceRule <> aDest.BadUndPriceRule _
                            Or aSrc.BadOptSinglePriceRule <> aDest.BadOptSinglePriceRule _
                            Or aSrc.BadOptBothPriceRule <> aDest.BadOptBothPriceRule _
                            Or aSrc.UseZeroBidRule <> aDest.UseZeroBidRule)
End Function

Private Sub LoadPriceProfiles()
    On Error Resume Next
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom, aGPrProf As EtsGeneralLib.EtsPriceProfileAtom
    
    lstPpProfiles.Clear
    m_PriceProfile.Clear

    For Each aGPrProf In g_PriceProfile
        Set aPrProf = New EtsGeneralLib.EtsPriceProfileAtom
        CopyPriceProfile aGPrProf, aPrProf
        m_PriceProfile.Add aPrProf.ID, aPrProf.Name, aPrProf
    
        lstPpProfiles.AddItem aPrProf.Name
        lstPpProfiles.ItemData(lstPpProfiles.NewIndex) = aPrProf.ID
        
        Set aPrProf = Nothing
    Next
        
    If m_PriceProfile.Count > 0 Then
        lstPpProfiles.ListIndex = 0
    End If

    ShowPriceProfile
End Sub

Private Sub SavePriceProfiles()
    On Error Resume Next
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom, aGPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Dim bFireChange As Boolean

    bFireChange = False

    For Each aPrProf In m_PriceProfile
        Set aGPrProf = g_PriceProfile(aPrProf.ID)
        If Not aGPrProf Is Nothing Then
            If Not IsPriceProfilesEqual(aPrProf, aGPrProf) Then
            
                If SavePriceProfileToDB(aPrProf) Then
                    CopyPriceProfile aPrProf, aGPrProf
                    bFireChange = True
                End If
            
                Set aGPrProf = Nothing
            End If
        End If
    Next
    
    If (bFireChange) Then
        g_Params.FirePriceProfilesChange
    End If
    
End Sub

Private Function SavePriceProfileToDB(ByRef aPrProf As EtsGeneralLib.EtsPriceProfileAtom) As Boolean
    On Error GoTo EH

    SavePriceProfileToDB = False

    With aPrProf
        If .IsOptionProfile Then
            gDBW.usp_PriceProfile_Save .ID, .Name, .Description, 1, 0, _
                                    IIf(.UseMidMarketForPnL, 1, 0), _
                                    .BadOptSinglePriceRule, .BadOptBothPriceRule, .UseZeroBidRule
        Else
            gDBW.usp_PriceProfile_Save .ID, .Name, .Description, 0, IIf(.AlwaysUseLast, 1, 0), _
                                    IIf(.UseMidMarketForPnL, 1, 0), _
                                    .BadUndPriceRule, 0, .UseZeroBidRule
        End If
    End With

    SavePriceProfileToDB = True
    Exit Function
EH:
    Dim sDesc$: sDesc = Err.Description
    LogEvent EVENT_ERROR, "Fail to save price profile '" & aPrProf.Name & "': " & sDesc
End Function

Private Sub ShowPriceProfile()
    On Error Resume Next
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        lblPpName.Enabled = True
        lblPpDesc.Enabled = True
        
        txtPpName.Text = aPrProf.Name
        txtPpDesc.Text = aPrProf.Description
        
        txtPpName.Locked = True ' Sharky: (aPrProf.DefaultID <> 0)
        txtPpDesc.Locked = txtPpName.Locked
        
        txtPpName.BackColor = IIf(txtPpName.Locked, &H8000000F, &H80000005)
        txtPpDesc.BackColor = txtPpName.BackColor
        
        If aPrProf.IsOptionProfile Then
            fraPpUnd.Visible = False
            fraPpOpt.Visible = True
            
            chkPpOptNonMidmarketPnL.Value = IIf(aPrProf.UseMidMarketForPnL, 0, 1)
            
            chkZeroBidRuleForPnL.Value = IIf(aPrProf.UseZeroBidRule, 1, 0)
            
            optPpOptReplaceWithLast.Value = (aPrProf.BadOptSinglePriceRule = enObsprReplaceWithLast)
            optPpOptReplaceWithOpposite.Value = (aPrProf.BadOptSinglePriceRule = enObsprReplaceWithOpposite)
            optPpOptReplaceWithTheo.Value = (aPrProf.BadOptSinglePriceRule = enObsprReplaceWithTheo)
            optPpOptReplaceWithZero.Value = (aPrProf.BadOptSinglePriceRule = enObsprReplaceWithZero)
            
            optPpOptBothBadUseLast.Value = (aPrProf.BadOptBothPriceRule = enObbprUseLast)
            optPpOptBothBadUseTheo.Value = (aPrProf.BadOptBothPriceRule = enObbprUseTheo)
            
            Select Case aPrProf.DefaultID
                Case 1
                    chkZeroBidRuleForPnL.Visible = True
                    txtLabel.Caption = "Use MidMarket for P&&L"
                    
                    chkPpOptNonMidmarketPnL.Value = 0
                    chkPpOptNonMidmarketPnL.Enabled = False
                    
                    If Not aPrProf.UseMidMarketForPnL Then
                        aPrProf.UseMidMarketForPnL = True
                        m_bPriceProfileDirty = True
                    End If
                    
                Case 2
                    chkZeroBidRuleForPnL.Visible = False
                    txtLabel.Caption = "Use Bid/Ask for Long/Short Positions for P&&L"
                    
                    chkPpOptNonMidmarketPnL.Value = 1
                    chkPpOptNonMidmarketPnL.Enabled = False
                    If aPrProf.UseZeroBidRule Then
                        aPrProf.UseZeroBidRule = False
                        m_bPriceProfileDirty = True
                    End If
                    If aPrProf.UseMidMarketForPnL Then
                        aPrProf.UseMidMarketForPnL = False
                        m_bPriceProfileDirty = True
                    End If
                
                Case Else
                    chkPpOptNonMidmarketPnL.Enabled = True
            End Select
        Else
            fraPpUnd.Visible = True
            fraPpOpt.Visible = False
            
            optPpUndAlwaysLast.Value = aPrProf.AlwaysUseLast
            optPpUndUseMidmarket.Value = Not optPpUndAlwaysLast.Value
            chkPpUndNonMidmarketPnL.Value = IIf(aPrProf.UseMidMarketForPnL, 0, 1)
            chkPpUndCheckTolerance.Value = IIf(aPrProf.BadUndPriceRule = enUbprCheckTolerance, 1, 0)
            
            chkPpUndNonMidmarketPnL.Enabled = Not aPrProf.AlwaysUseLast
            chkPpUndCheckTolerance.Enabled = chkPpUndNonMidmarketPnL.Enabled
        
            Select Case aPrProf.DefaultID
                Case 1
                    optPpUndAlwaysLast.Value = False
                    optPpUndUseMidmarket.Value = True
                    chkPpUndNonMidmarketPnL.Value = 0
                    
                    optPpUndAlwaysLast.Enabled = False
                    optPpUndUseMidmarket.Enabled = False
                    chkPpUndNonMidmarketPnL.Enabled = False
                    
                    lblPpUndNotes.Visible = True
                    
                    If aPrProf.AlwaysUseLast Or Not aPrProf.UseMidMarketForPnL Then
                        aPrProf.AlwaysUseLast = False
                        aPrProf.UseMidMarketForPnL = True
                        
                        m_bPriceProfileDirty = True
                    End If
                
                Case 2
                    optPpUndAlwaysLast.Value = False
                    optPpUndUseMidmarket.Value = True
                    chkPpUndNonMidmarketPnL.Value = 1
                    
                    optPpUndAlwaysLast.Enabled = False
                    optPpUndUseMidmarket.Enabled = False
                    chkPpUndNonMidmarketPnL.Enabled = False
                    
                    lblPpUndNotes.Visible = True
                    
                    If aPrProf.AlwaysUseLast Or aPrProf.UseMidMarketForPnL Then
                        aPrProf.AlwaysUseLast = False
                        aPrProf.UseMidMarketForPnL = False
                        
                        m_bPriceProfileDirty = True
                    End If
                
                Case 3
                    optPpUndAlwaysLast.Value = True
                    optPpUndUseMidmarket.Value = False
                    chkPpUndNonMidmarketPnL.Value = 0
                    chkPpUndCheckTolerance.Value = 0
                    
                    optPpUndAlwaysLast.Enabled = False
                    optPpUndUseMidmarket.Enabled = False
                    chkPpUndNonMidmarketPnL.Enabled = False
                    chkPpUndCheckTolerance.Enabled = False
                    
                    lblPpUndNotes.Visible = False
                    
                    If Not aPrProf.AlwaysUseLast Then
                        aPrProf.AlwaysUseLast = True
                        aPrProf.UseMidMarketForPnL = False
                        aPrProf.BadUndPriceRule = enUbprDoNotCheckTolerance
                        
                        m_bPriceProfileDirty = True
                    End If
                
                Case Else
                    optPpUndAlwaysLast.Enabled = True
                    optPpUndUseMidmarket.Enabled = True
                    lblPpUndNotes.Visible = True
            End Select
        End If
        
        Set aPrProf = Nothing
    Else
        lblPpName.Enabled = False
        lblPpDesc.Enabled = False
        txtPpName.Locked = False
        txtPpDesc.Locked = False
        txtPpName.BackColor = &H8000000F
        txtPpDesc.BackColor = txtPpName.BackColor
        
        fraPpUnd.Visible = False
        fraPpOpt.Visible = False
    End If
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    'Me.Hide
    Unload Me
End Sub

Private Sub btnOk_Click()
    On Error Resume Next
    'Me.Hide
    
    Screen.MousePointer = vbHourglass
    If m_bDirty Then SaveData
    If m_bPriceProfileDirty Then SavePriceProfiles
    
    If (m_bDirty) Then
        Dim aXMLLoader As ETSXMLParamsLib.XMLLoader
        Set aXMLLoader = New ETSXMLParamsLib.XMLLoader
        If (Not aXMLLoader Is Nothing) Then
            g_Params.SaveSettings
            aXMLLoader.Save CurrentUserXMLFilePath$, g_aUserXMLParams
        End If
    End If
    
    PubChanges
    
    Screen.MousePointer = vbDefault
    
    Unload Me
End Sub

Private Sub chkAdvActiveRealTime_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkAdvIvRealTime_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub lstPpProfiles_Click()
    On Error Resume Next
    If Not m_bInitializing Then
        m_bInitializing = True
        ShowPriceProfile
        m_bInitializing = False
    End If
End Sub

Private Sub optAdvActTrdEditable_Click()
    On Error Resume Next
    optDefNewTrdActual.Enabled = True
    m_bDirty = True
    If Not m_bInitializing And optAdvActTrdEditable.Value <> 0 Then
        gCmn.MyMsgBox Me, "Deleted actual trades can't be restored.", vbExclamation
    End If
End Sub

Private Sub optAdvActTrdProtected_Click()
    On Error Resume Next
    optDefNewTrdActual.Enabled = False
    If optDefNewTrdActual.Value <> 0 Then optDefNewTrdSimulated.Value = 1
    m_bDirty = True
End Sub

Private Sub optCalcModelOptimized_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcModelStandard_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcModelVskLog_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcPnlDaily_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcPnlLTD_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcUseMarketVola_Click()
    On Error Resume Next
    m_bDirty = True
    chkCalcUseTheoNoBid.Enabled = optCalcUseMarketVola.Value
    chkCalcUseTheoBadMarketVola.Enabled = optCalcUseMarketVola.Value
End Sub

Private Sub chkQuoteDirectlyRealtime_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optCalcUseTheoVola_Click()
    On Error Resume Next
    m_bDirty = True
    chkCalcUseTheoNoBid.Enabled = optCalcUseMarketVola.Value
    chkCalcUseTheoBadMarketVola.Enabled = optCalcUseMarketVola.Value
End Sub

Private Sub optDefNewTrdActual_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optDefNewTrdLastEntered_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optDefNewTrdManual_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optDefNewTrdSimulated_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optUncOff_Click()
    ctlDaysToDiv.Enabled = False
    g_Params.EAUpcomingDivs = False
    m_bDirty = True
End Sub

Private Sub optUncOn_Click()
    ctlDaysToDiv.Enabled = True
    g_Params.EAUpcomingDivs = True
    m_bDirty = True
End Sub

Private Sub optMatrixExerciseOpts_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optMatrixIgnoreOpts_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optPpOptReplaceWithZero_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptSinglePriceRule = enObsprReplaceWithZero
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optQuoteBidColBuy_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteBidColSell_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteFitToImpCurveCalls_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteFitToImpCurvePuts_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteFitToImpCurveOTM_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteOtherColBuy_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteOtherColSell_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkRiskDirectlyRealtime_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optRiskBidColBuy_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optRiskBidColSell_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optRiskOtherColBuy_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optRiskOtherColSell_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub cboQuoteDefExchOpt_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub cboQuoteDefExchUnd_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkQuoteUseExpirySeparation_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkCalcUseTheoNoBid_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkCalcUseTheoBadMarketVola_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub chkAdvUseCalcSleep_Click()
    On Error Resume Next
    lblAdvCalcSleepFreq.Enabled = chkAdvUseCalcSleep.Value
    lblAdvCalcSleepAmt.Enabled = chkAdvUseCalcSleep.Value
    txtAdvCalcSleepFreq.Enabled = chkAdvUseCalcSleep.Value
    txtAdvCalcSleepAmt.Enabled = chkAdvUseCalcSleep.Value
    
    txtAdvCalcSleepFreq.BackColor = IIf(chkAdvUseCalcSleep.Value, &H80000005, &H8000000F)
    txtAdvCalcSleepAmt.BackColor = txtAdvCalcSleepFreq.BackColor
    m_bDirty = True
End Sub

Private Sub optRiskReqTypeAlwaysNonGroup_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optRiskReqTypeOnPriceProvider_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteReqTypeAlwaysNonGroup_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optQuoteReqTypeOnPriceProvider_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optMatrixReqTypeAlwaysNonGroup_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub optMatrixReqTypeOnPriceProvider_Click()
    On Error Resume Next
    m_bDirty = True
End Sub

Private Sub ctlDaysToDiv_Change()
    On Error Resume Next
    Dim nValue&: nValue = -1
    nValue = CLng(ctlDaysToDiv.Text)
    If nValue >= 0 And nValue <= 1000 Then
        g_Params.EADaysToDiv = ctlDaysToDiv.Text
        m_bDirty = True
    End If
End Sub

Private Sub txtAdvCalcSleepFreq_Validate(Cancel As Boolean)
    On Error Resume Next
    Dim nValue&: nValue = -1

    nValue = CLng(txtAdvCalcSleepFreq.Text)
    If nValue > 0 And nValue <= 1000 And m_CalcSleepFreq <> nValue Then
        m_CalcSleepFreq = nValue
        m_bDirty = True
    End If
    txtAdvCalcSleepFreq.Text = Format$(m_CalcSleepFreq, "#,##0")
End Sub

Private Sub txtAdvCalcSleepAmt_Validate(Cancel As Boolean)
    On Error Resume Next
    Dim nValue&: nValue = 0

    nValue = CLng(txtAdvCalcSleepAmt.Text)
    If nValue > 0 And nValue <= 1000 And m_CalcSleepAmt <> nValue Then
        m_CalcSleepAmt = nValue
        m_bDirty = True
    End If
    txtAdvCalcSleepAmt.Text = Format$(m_CalcSleepAmt, "#,##0")
End Sub

Private Sub txtExpDays_Change()
    On Error Resume Next
    Dim nTmp As Long
    
    m_bDirty = True
    
    nTmp = CLng(txtExpDays.Text)
    m_nSOQ_EADaysToDivDays = nTmp
    
    If txtExpDays.Text <> "" Then txtExpDays.Text = CStr(m_nSOQ_EADaysToDivDays)
End Sub

Private Sub txtOrdOptIncrPriceCheap_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = Round(CDbl(txtOrdOptIncrPriceCheap.Text), 2)
    If dValue > 0.01 And dValue <= 100# And m_dOrdOptIncrPriceCheap <> dValue Then
        m_dOrdOptIncrPriceCheap = dValue
        m_bDirty = True
    End If
    txtOrdOptIncrPriceCheap.Text = m_dOrdOptIncrPriceCheap
End Sub

Private Sub txtOrdOptIncrPriceExpensive_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = Round(CDbl(txtOrdOptIncrPriceExpensive.Text), 2)
    If dValue > 0.01 And dValue <= 100# And m_dOrdOptIncrPriceExpensive <> dValue Then
        m_dOrdOptIncrPriceExpensive = dValue
        m_bDirty = True
    End If
    txtOrdOptIncrPriceExpensive.Text = m_dOrdOptIncrPriceExpensive
End Sub

Private Sub txtOrdStkIncrPrice_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = Round(CDbl(txtOrdStkIncrPrice.Text), 2)
    If dValue > 0.01 And dValue <= 100# And m_dOrdStkIncrPrice <> dValue Then
        m_dOrdStkIncrPrice = dValue
        m_bDirty = True
    End If
    txtOrdStkIncrPrice.Text = m_dOrdStkIncrPrice
End Sub

Private Sub txtOrdOptIncrQty_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim nValue&: nValue = 0

    nValue = CLng(txtOrdOptIncrQty.Text)
    If nValue > 0 And nValue <= 10000 And m_nOrdOptIncrQty <> nValue Then
        m_nOrdOptIncrQty = nValue
        m_bDirty = True
    Else
        txtOrdOptIncrQty.Text = m_nOrdOptIncrQty
    End If
End Sub

Private Sub txtOrdStkIncrQty_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim nValue&: nValue = 0

    nValue = CLng(txtOrdStkIncrQty.Text)
    If nValue > 0 And nValue <= 10000 And m_nOrdStkIncrQty <> nValue Then
        m_nOrdStkIncrQty = nValue
        m_bDirty = True
    Else
        txtOrdStkIncrQty.Text = m_nOrdStkIncrQty
    End If
End Sub

Private Sub txtCrdIncrPriceCheap_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = Round(CDbl(txtCrdIncrPriceCheap.Text), 2)
    If dValue > 0.01 And dValue <= 100# And m_dCrdIncrPriceCheap <> dValue Then
        m_dCrdIncrPriceCheap = dValue
        m_bDirty = True
    End If
    txtCrdIncrPriceCheap.Text = m_dCrdIncrPriceCheap
End Sub

Private Sub txtCrdIncrPriceExpensive_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = Round(CDbl(txtCrdIncrPriceExpensive.Text), 2)
    If dValue > 0.01 And dValue <= 100# And m_dCrdIncrPriceExpensive <> dValue Then
        m_dCrdIncrPriceExpensive = dValue
        m_bDirty = True
    End If
    txtCrdIncrPriceExpensive.Text = m_dCrdIncrPriceExpensive
End Sub

Private Sub txtCrdIncrQty_Validation(Cancel As Boolean)
    On Error Resume Next
    If m_bInitializing Then Exit Sub
    Dim nValue&: nValue = 0

    nValue = CLng(txtCrdIncrQty.Text)
    If nValue > 0 And nValue <= 10000 And m_nCrdIncrQty <> nValue Then
        m_nCrdIncrQty = nValue
        m_bDirty = True
    Else
        txtCrdIncrQty.Text = m_nCrdIncrQty
    End If
End Sub

Private Sub txtPpName_Change()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.DefaultID = 0 Then
            m_bPriceProfileDirty = True
            
            aPrProf.Name = Trim$(txtPpName.Text)
            m_bInitializing = True
            txtPpName.Text = aPrProf.Name
            m_bInitializing = False
        End If
        Set aPrProf = Nothing
    End If
End Sub

Private Sub txtPpDesc_Change()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.DefaultID = 0 Then
            m_bPriceProfileDirty = True
            
            aPrProf.Description = Trim$(txtPpDesc.Text)
            m_bInitializing = True
            txtPpDesc.Text = aPrProf.Description
            m_bInitializing = False
        End If
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpUndAlwaysLast_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If Not aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.AlwaysUseLast = True
        
            chkPpUndNonMidmarketPnL.Enabled = Not aPrProf.AlwaysUseLast
            chkPpUndCheckTolerance.Enabled = chkPpUndNonMidmarketPnL.Enabled
        End If

        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpUndUseMidmarket_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If Not aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.AlwaysUseLast = False
        
            chkPpUndNonMidmarketPnL.Enabled = Not aPrProf.AlwaysUseLast
            chkPpUndCheckTolerance.Enabled = chkPpUndNonMidmarketPnL.Enabled
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub chkPpUndCheckTolerance_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If Not aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadUndPriceRule = IIf(chkPpUndCheckTolerance.Value <> 0, enUbprCheckTolerance, enUbprDoNotCheckTolerance)
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub chkPpUndNonMidmarketPnL_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If Not aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.UseMidMarketForPnL = (chkPpUndNonMidmarketPnL.Value = 0)
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpOptBothBadUseLast_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptBothPriceRule = enObbprUseLast
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpOptBothBadUseTheo_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptBothPriceRule = enObbprUseTheo
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpOptReplaceWithLast_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptSinglePriceRule = enObsprReplaceWithLast
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpOptReplaceWithOpposite_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptSinglePriceRule = enObsprReplaceWithOpposite
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub optPpOptReplaceWithTheo_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.BadOptSinglePriceRule = enObsprReplaceWithTheo
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub chkPpOptNonMidmarketPnL_Click()
    On Error Resume Next
    If m_bInitializing Then Exit Sub

    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Set aPrProf = m_PriceProfile(lstPpProfiles.ItemData(lstPpProfiles.ListIndex))
    
    If Not aPrProf Is Nothing Then
        If aPrProf.IsOptionProfile Then
            m_bPriceProfileDirty = True
            aPrProf.UseMidMarketForPnL = (chkPpOptNonMidmarketPnL.Value = 0)
        End If
        
        Set aPrProf = Nothing
    End If
End Sub

Private Sub txtCalcUndTolerance_Change()
    On Error Resume Next
    Dim dValue#: dValue = 0#

    dValue = CDbl(txtCalcUndTolerance.Text)
    If dValue >= 0# And dValue <= 100# And m_dUndPriceToleranceValue <> dValue Then
        m_dUndPriceToleranceValue = dValue
        m_bDirty = True
        m_bToleranceDirty = True
        txtCalcUndTolerance.Text = CStr(m_dUndPriceToleranceValue)
    End If
End Sub

Private Sub optCalcPriceRoundNone_Click()
    On Error Resume Next
    m_bDirty = True
    m_bRoundingRuleDirty = True
End Sub

Private Sub optCalcPriceRoundUp_Click()
    On Error Resume Next
    m_bDirty = True
    m_bRoundingRuleDirty = True
End Sub

Private Sub optCalcPriceRoundDown_Click()
    On Error Resume Next
    m_bDirty = True
    m_bRoundingRuleDirty = True
End Sub

Private Sub LogParameter(sName As String, sValue As String, bLoad As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Parameters View " & IIf(bLoad, "Load", "Save") & " Parameter: " _
                                                    & """" & sName & """ = """ & sValue & """", m_frmOwner.Caption
End Sub

Private Sub PubChanges()
On Error GoTo Exception

    Dim aData As MSGSTRUCTLib.UnderlyingUpdate
    Set aData = New MSGSTRUCTLib.UnderlyingUpdate
    
    aData.UpdStatus = enUndNoChanges
    
    If (m_bDirty) Then
        aData.UpdStatus = aData.UpdStatus Or enGlobalParamsUpdate
    End If
    
    If (m_bRoundingRuleDirty) Then
        aData.UpdStatus = aData.UpdStatus Or enPriceRoundRuleUpdate
    End If
    
    If (m_bToleranceDirty) Then
        aData.UpdStatus = aData.UpdStatus Or enToleranceRangeUpdate
    End If
    
    If (m_bPriceProfileDirty) Then
        aData.UpdStatus = aData.UpdStatus Or enPriceProfileUpdate
    End If
    
    If (aData.UpdStatus <> enUndNoChanges) Then
        g_TradeChannel.PubUnderlyingUpdate aData
    End If
    
Exit Sub
Exception:
    gCmn.ErrorMsgBox m_frmOwner, "Unable To Pub Changes. Please, restart application."
End Sub
