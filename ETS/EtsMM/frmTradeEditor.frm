VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmTradeEditor 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Trade Editor"
   ClientHeight    =   5340
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7200
   Icon            =   "frmTradeEditor.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5340
   ScaleWidth      =   7200
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Tag             =   "text"
   Begin VB.ComboBox cmbExerciseStyle 
      Enabled         =   0   'False
      Height          =   315
      Left            =   4440
      Style           =   2  'Dropdown List
      TabIndex        =   60
      Top             =   1080
      Width           =   1215
   End
   Begin VB.ComboBox cmbSettlementType 
      Enabled         =   0   'False
      Height          =   315
      Left            =   5760
      Style           =   2  'Dropdown List
      TabIndex        =   59
      Top             =   1080
      Width           =   1335
   End
   Begin VB.CheckBox chkIsFlex 
      Caption         =   "Flex"
      Height          =   255
      Left            =   120
      TabIndex        =   58
      Top             =   1080
      Width           =   615
   End
   Begin VB.ComboBox cbxStrategy 
      Height          =   315
      Left            =   2400
      Style           =   2  'Dropdown List
      TabIndex        =   6
      Top             =   3255
      Width           =   1935
   End
   Begin VB.CommandButton btnStrategyNew 
      Caption         =   "New..."
      Height          =   315
      Left            =   4440
      TabIndex        =   47
      Top             =   3255
      Width           =   1095
   End
   Begin VB.CheckBox chkMark 
      Caption         =   "Marked"
      Height          =   255
      Left            =   6060
      TabIndex        =   7
      Top             =   3300
      Width           =   1035
   End
   Begin EtsMM.ctlEditEx txtQuantity 
      Height          =   315
      Left            =   3480
      TabIndex        =   2
      Top             =   2130
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":000C
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin VB.OptionButton optActual 
      Caption         =   "Actual"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   2940
      TabIndex        =   18
      Top             =   4950
      Width           =   1275
   End
   Begin VB.OptionButton optManual 
      Caption         =   "Manual"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   1620
      TabIndex        =   17
      Top             =   4950
      Width           =   1275
   End
   Begin VB.OptionButton optSimulated 
      Caption         =   "Simulated"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   255
      Left            =   120
      TabIndex        =   16
      Top             =   4950
      Width           =   1395
   End
   Begin VB.Timer tmrTradeDate 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   6000
      Top             =   0
   End
   Begin VB.Timer tmrPrice 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   6600
      Top             =   0
   End
   Begin VB.CommandButton btnSubmit 
      Caption         =   "Submit"
      Default         =   -1  'True
      Height          =   315
      Left            =   4320
      TabIndex        =   19
      Top             =   4920
      Width           =   1335
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   5760
      TabIndex        =   20
      Top             =   4920
      Width           =   1335
   End
   Begin EtsMM.ctlLabelEx lblTotalPrice 
      Height          =   315
      Left            =   4920
      TabIndex        =   21
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   2130
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      HoverForeColor  =   -2147483630
      Caption         =   "0"
      PressForeColor  =   -2147483630
      DisabledForeColor=   -2147483630
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblTradeDate 
      Height          =   315
      Left            =   120
      TabIndex        =   22
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   2685
      Width           =   1485
      _ExtentX        =   2619
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      HoverForeColor  =   -2147483630
      Caption         =   "0"
      PressForeColor  =   -2147483630
      DisabledForeColor=   -2147483630
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblTradeTime 
      Height          =   315
      Left            =   1680
      TabIndex        =   23
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   2685
      Width           =   1485
      _ExtentX        =   2619
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      HoverForeColor  =   -2147483630
      Caption         =   "0"
      PressForeColor  =   -2147483630
      DisabledForeColor=   -2147483630
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblTotalCommission 
      Height          =   315
      Left            =   5640
      TabIndex        =   24
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   4140
      Width           =   1455
      _ExtentX        =   2566
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      HoverForeColor  =   -2147483630
      Caption         =   "0"
      PressForeColor  =   -2147483630
      DisabledForeColor=   -2147483630
      HoverEffect     =   0
   End
   Begin EtsMM.ctlLabelEx lblLotSize 
      Height          =   315
      Left            =   2520
      TabIndex        =   43
      TabStop         =   0   'False
      Tag             =   "info"
      Top             =   2130
      Width           =   795
      _ExtentX        =   1402
      _ExtentY        =   556
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MousePointer    =   0
      HoverForeColor  =   -2147483630
      Caption         =   "0"
      PressForeColor  =   -2147483630
      DisabledForeColor=   -2147483630
      HoverEffect     =   0
   End
   Begin EtsMM.ctlEditEx txtPrice 
      Height          =   315
      Left            =   1200
      TabIndex        =   1
      Top             =   2130
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":0028
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtSpotRef 
      Height          =   315
      Left            =   3480
      TabIndex        =   3
      Top             =   2685
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":0044
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtCommission 
      Height          =   315
      Left            =   4440
      TabIndex        =   11
      Top             =   3840
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":0060
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtTradedIV 
      Height          =   315
      Left            =   4920
      TabIndex        =   4
      Top             =   2685
      Width           =   2175
      _ExtentX        =   3836
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":007C
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtClearingCommission 
      Height          =   315
      Left            =   4440
      TabIndex        =   15
      Top             =   4440
      Width           =   1095
      _ExtentX        =   1931
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":0098
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtRate 
      Height          =   315
      Left            =   3600
      TabIndex        =   10
      Top             =   3840
      Width           =   735
      _ExtentX        =   1296
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":00B4
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin EtsMM.ctlEditEx txtClearingRate 
      Height          =   315
      Left            =   3600
      TabIndex        =   14
      Top             =   4455
      Width           =   735
      _ExtentX        =   1296
      _ExtentY        =   556
      MouseIcon       =   "frmTradeEditor.frx":00D0
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      HideSelection   =   -1  'True
   End
   Begin VB.ComboBox cmbOptionType 
      Enabled         =   0   'False
      Height          =   315
      Left            =   3240
      Style           =   2  'Dropdown List
      TabIndex        =   51
      Top             =   1080
      Width           =   1095
   End
   Begin VB.TextBox txtStrike 
      BeginProperty DataFormat 
         Type            =   1
         Format          =   """$""#,##0.00;(""$""#,##0.00)"
         HaveTrueFalseNull=   0
         FirstDayOfWeek  =   0
         FirstWeekOfYear =   0
         LCID            =   1033
         SubFormatType   =   2
      EndProperty
      Enabled         =   0   'False
      Height          =   315
      Left            =   840
      TabIndex        =   50
      Text            =   "0"
      Top             =   1080
      Width           =   855
   End
   Begin MSComCtl2.DTPicker dtpExpiry 
      Height          =   315
      Left            =   1800
      TabIndex        =   49
      Top             =   1080
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   556
      _Version        =   393216
      Enabled         =   0   'False
      Format          =   48234497
      CurrentDate     =   40211
   End
   Begin VB.Label lblOptionSymbol 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      Caption         =   "Symbol:"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   57
      Top             =   1440
      Width           =   6975
   End
   Begin VB.Label lblOptionType 
      BackStyle       =   0  'Transparent
      Caption         =   "Type"
      Height          =   255
      Left            =   3240
      TabIndex        =   56
      Top             =   840
      Width           =   855
   End
   Begin VB.Label lblExerciseStyle 
      BackStyle       =   0  'Transparent
      Caption         =   "Style"
      Height          =   255
      Left            =   4440
      TabIndex        =   55
      Top             =   840
      Width           =   615
   End
   Begin VB.Label lblSettlementType 
      BackStyle       =   0  'Transparent
      Caption         =   "Settlement"
      Height          =   255
      Left            =   5760
      TabIndex        =   54
      Top             =   840
      Width           =   855
   End
   Begin VB.Label lblExpiry 
      BackStyle       =   0  'Transparent
      Caption         =   "Expiration date"
      Height          =   255
      Left            =   1800
      TabIndex        =   53
      Top             =   840
      Width           =   1095
   End
   Begin VB.Label lblStrike 
      BackStyle       =   0  'Transparent
      Caption         =   "Strike"
      Height          =   255
      Left            =   840
      TabIndex        =   52
      Top             =   840
      Width           =   855
   End
   Begin MSForms.ComboBox cbxBuySell 
      Height          =   315
      Left            =   120
      TabIndex        =   48
      Top             =   2160
      Width           =   855
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "1508;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Price:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   1200
      TabIndex        =   46
      Top             =   1905
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Comm.Type:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   21
      Left            =   2400
      TabIndex        =   45
      Top             =   3615
      Width           =   1095
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Comm.Type:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   20
      Left            =   2400
      TabIndex        =   44
      Top             =   4230
      Width           =   1095
   End
   Begin MSForms.ComboBox cbxCommType 
      Height          =   315
      Left            =   2400
      TabIndex        =   9
      Top             =   3840
      Width           =   1095
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "1931;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxClearingCommType 
      Height          =   315
      Left            =   2400
      TabIndex        =   13
      Top             =   4455
      Width           =   1095
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "1931;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxClearingBroker 
      Height          =   315
      Left            =   120
      TabIndex        =   12
      Top             =   4455
      Width           =   2175
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "3836;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxBroker 
      Height          =   315
      Left            =   120
      TabIndex        =   8
      Top             =   3840
      Width           =   2175
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "3836;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxTrader 
      Height          =   315
      Left            =   120
      TabIndex        =   5
      Top             =   3255
      Width           =   2175
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "3836;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin MSForms.ComboBox cbxContract 
      Height          =   315
      Left            =   120
      TabIndex        =   0
      Top             =   345
      Width           =   6975
      VariousPropertyBits=   547379227
      BackColor       =   16119285
      DisplayStyle    =   7
      Size            =   "12303;556"
      ListRows        =   10
      MatchEntry      =   1
      ShowDropButtonWhen=   2
      BorderColor     =   -2147483640
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Total Commission:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   19
      Left            =   5640
      TabIndex        =   42
      Top             =   3900
      Width           =   1455
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Commission:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   18
      Left            =   4440
      TabIndex        =   41
      Top             =   4230
      Width           =   1095
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Rate:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   17
      Left            =   3600
      TabIndex        =   40
      Top             =   4230
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Clearing Broker:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   16
      Left            =   120
      TabIndex        =   39
      Top             =   4230
      Width           =   1215
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Commission:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   15
      Left            =   4440
      TabIndex        =   38
      Top             =   3615
      Width           =   1095
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Rate:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   14
      Left            =   3600
      TabIndex        =   37
      Top             =   3615
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Broker:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   13
      Left            =   120
      TabIndex        =   36
      Top             =   3615
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Strategy:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   12
      Left            =   2400
      TabIndex        =   35
      Top             =   3030
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trader:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   10
      Left            =   120
      TabIndex        =   34
      Top             =   3030
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Total Price:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   5
      Left            =   4920
      TabIndex        =   33
      Top             =   1905
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Quantity:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   4
      Left            =   3480
      TabIndex        =   32
      Top             =   1905
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Lot Size:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   2520
      TabIndex        =   31
      Top             =   1905
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Buy/Sell:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   30
      Top             =   1905
      Width           =   735
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Contract:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   29
      Top             =   120
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trade Date:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   6
      Left            =   120
      TabIndex        =   28
      Top             =   2490
      Width           =   975
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Spot Reference:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   8
      Left            =   3480
      TabIndex        =   27
      Top             =   2490
      Width           =   1335
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Traded IV, %:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   9
      Left            =   4920
      TabIndex        =   26
      Top             =   2490
      Width           =   1335
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trade Time:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   7
      Left            =   1680
      TabIndex        =   25
      Top             =   2490
      Width           =   975
   End
End
Attribute VB_Name = "frmTradeEditor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1

Private m_bInit As Boolean
Private m_bCommInit As Boolean

Private m_NewTrd As EtsGeneralLib.MmTradeInfoAtom
Private m_OldTrd As EtsGeneralLib.MmTradeInfoAtom
Private m_TrdSrc As New EtsGeneralLib.MmTradeInfoColl
Private m_Price As New clsTePriceColl
Private m_PriceByKey As New clsTePriceColl

Private m_bLastQuoteReqNow As Boolean
Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long

Private m_nDefTraderID As Long
Private m_nDefBrokerID As Long
Private m_nDefClearingBrokerID As Long
Private m_enDefStatus As TRADE_STATUS

Private m_bIsProviderConnected As Boolean
Private m_bIsOpened As Boolean
Private m_frmOwner As Form
Private m_sCaption As String
Private m_AssetID As Long

Const MONITOR_DEFAULTTOPRIMARY = &H1

Private Type tagMONITORINFO
    cbSize      As Long 'Size of structure
    rcMonitor   As RECT 'Monitor rect
    rcWork      As RECT 'Working area rect
    dwFlags     As Long 'Flags
End Type


Private Declare Function GetMonitorInfo Lib "user32" _
        Alias "GetMonitorInfoA" ( _
        ByVal hMonitor As Long, _
        MonInfo As tagMONITORINFO) As Long

Private Declare Function MonitorFromWindow Lib "user32" ( _
        ByVal hWnd As Long, _
        dwFlags As Long) As Long

Private Declare Function MonitorFromRect Lib "user32" ( _
        rc As RECT, _
        ByVal dwFlags As Long) As Long

Private Function GetMonitorFromWindow(hWnd As Long, dwFlags As Long) As Long
    '=====================================================
    'Returns a monitor handle that the Window (hwnd) is in
    '=====================================================
    Dim lReturn As Long
    
    On Error GoTo GetMonitorFromWindow_Err
    lReturn = MonitorFromWindow(hWnd, dwFlags)
    GetMonitorFromWindow = lReturn
    Exit Function
GetMonitorFromWindow_Err:
    If Err.Number = 453 Then
        'Non-Multimonitor OS, return -1
        GetMonitorFromWindow = -1
    End If
End Function
        
Private Sub CenterFormOnMonitor(ByRef FormToCenter As Form, ByRef RForm As Form)
    '====================================================================
    'Centers the FormToCenter on the monitor that the ReferenceForm is on
    'or the primary monitor if the ReferenceForm is ommited
    '====================================================================
    Dim lMonitor        As Long
    Dim lReturn         As Long
    Dim MonitorInfo     As tagMONITORINFO
    Dim lMonitorWidth   As Long
    Dim lMonitorHeight  As Long
    
    On Error GoTo CenterFormOnMonitor_Err
    
    'Get the handle to the monitor that the reference form is on
    'If IsMissing(ReferenceForm) Then
    '    lMonitor = GetMonitorFromXYPoint(1, 1, MONITOR_DEFAULTTOPRIMARY)
    'Else
        lMonitor = GetMonitorFromWindow(RForm.hWnd, MONITOR_DEFAULTTOPRIMARY)
    'End If
    
    'If we get a valid lMonitor
    If lMonitor Then
        
        'init the structure
        MonitorInfo.cbSize = Len(MonitorInfo)
        
        'Get the monitor information
        lReturn = GetMonitorInfo(lMonitor, MonitorInfo)
        'If the Call does not fail then center the form over that monitor
        If lReturn Then
            With MonitorInfo
                lMonitorWidth = (.rcWork.Right - .rcWork.Left) * Screen.TwipsPerPixelX
                lMonitorHeight = (.rcWork.Bottom - .rcWork.Top) * Screen.TwipsPerPixelY
                FormToCenter.Move ((lMonitorWidth - FormToCenter.Width) \ 2) + .rcMonitor.Left * Screen.TwipsPerPixelX, ((lMonitorHeight - FormToCenter.Height) \ 2) + MonitorInfo.rcMonitor.Top * Screen.TwipsPerPixelX
            End With
        End If
    Else
        'There was not a monitor found, center on default screen
        FormToCenter.Move (Screen.Width - FormToCenter.Width) \ 2, (Screen.Height - FormToCenter.Height) \ 2
    End If
    Exit Sub
CenterFormOnMonitor_Err:
    If Err.Number = 453 Then
        'Non-Multimonitor OS
        FormToCenter.Move (Screen.Width - FormToCenter.Width) \ 2, (Screen.Width - FormToCenter.Width) \ 2
    End If
End Sub


Public Sub NewTrade(ByVal nContractID As Long, ByVal nUndContractID As Long, ByVal bIsBuy As Boolean, frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    Screen.MousePointer = vbHourglass
    m_bIsOpened = True
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_AssetID = nUndContractID
    
    Load Me
    ReadRegEntries
    Set m_NewTrd = Nothing
    Set m_OldTrd = Nothing
    
    If Not LoadTradeSourcesForNewTrade(nContractID, nUndContractID) Then
        Screen.MousePointer = vbDefault
        Unload Me
        Exit Sub
    End If
    
    m_NewTrd.IsBuy = bIsBuy
    m_NewTrd.TradeDate = Now
    If g_Params.MarkedTradesRuleOn Then m_NewTrd.Mark = 1
    
    If g_Params.DefTradeType = -1 Then
        m_NewTrd.Status = m_enDefStatus
    ElseIf g_Params.ActualTradesEditable Or g_Params.DefTradeType <> enTsRealtime Then
        m_NewTrd.Status = g_Params.DefTradeType
    Else
        m_NewTrd.Status = enTsSimulated
    End If
    
    InitNewTradeTrader
    
    Set m_NewTrd.Broker = g_Broker(m_nDefBrokerID)
    Set m_NewTrd.ClearingBroker = g_ClearingBroker(m_nDefClearingBrokerID)
    
    Init
    m_sCaption = "New Trade"
    Me.Caption = m_sCaption
    tmrTradeDate.Enabled = True
    Me.Show vbModeless, m_frmOwner
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to create new trade."
    On Error Resume Next
    Unload Me
End Sub

Public Sub EditTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom, frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    Screen.MousePointer = vbHourglass
    m_bIsOpened = True
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    Load Me
    
    ReadRegEntries
    
    Set m_NewTrd = New EtsGeneralLib.MmTradeInfoAtom
    Set m_OldTrd = New EtsGeneralLib.MmTradeInfoAtom
    
    aTrd.CopyTo m_OldTrd
    aTrd.CopyTo m_NewTrd
    
    LoadTradeSourcesForEditTrade aTrd
    
    If g_Params.ActualTradesEditable And m_NewTrd.Status = enTsMatchedRealtime Then m_NewTrd.Status = enTsRealtime
    
    Init
    m_sCaption = IIf(g_Params.ActualTradesEditable Or m_OldTrd.Status = enTsSimulated Or m_OldTrd.Status = enTsManual, "Edit Trade", "Trade Properties")
    Me.Caption = m_sCaption
    Me.Show vbModeless, frmOwner
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to create new trade."
    On Error Resume Next
    Unload Me
End Sub

Private Sub Init()
    On Error GoTo EH
    m_bInit = True
    
    cbxBuySell.List = Array("Buy", "Sell")
    cbxBuySell.ListIndex = IIf(m_NewTrd.IsBuy, 0, 1)
    
    FillContractsCombo
    
    lblTradeDate.Caption = Format$(m_NewTrd.TradeDate, "Short Date")
    lblTradeTime.Caption = Format$(m_NewTrd.TradeDate, "Long Time")
    
    FillTraderCombo
    FillCommissionTypeCombo cbxCommType
    FillCommissionTypeCombo cbxClearingCommType
    FillBrokerCombo cbxBroker, g_Broker, m_NewTrd.BrokerID
    FillBrokerCombo cbxClearingBroker, g_ClearingBroker, m_NewTrd.ClearingBrokerID
    
    cbxBuySell.Enabled = g_Params.ActualTradesEditable Or (m_NewTrd.Status = enTsSimulated Or m_NewTrd.Status = enTsManual)
    txtPrice.Enabled = cbxBuySell.Enabled
    txtQuantity.Enabled = cbxBuySell.Enabled
    txtSpotRef.Enabled = cbxBuySell.Enabled
    txtTradedIV.Enabled = cbxBuySell.Enabled
    cbxBroker.Enabled = cbxBuySell.Enabled
    cbxClearingBroker.Enabled = cbxBuySell.Enabled
    btnSubmit.Enabled = cbxBuySell.Enabled
    cbxContract.Enabled = cbxBuySell.Enabled And (m_OldTrd Is Nothing)
    cbxTrader.Enabled = cbxBuySell.Enabled And g_CurTraderID = 0 'And (m_OldTrd Is Nothing)
    optSimulated.Enabled = cbxBuySell.Enabled
    optSimulated.Visible = cbxBuySell.Enabled
    optSimulated.Value = (m_NewTrd.Status = enTsSimulated)
    optManual.Enabled = cbxBuySell.Enabled
    optManual.Visible = cbxBuySell.Enabled
    optManual.Value = (m_NewTrd.Status = enTsManual)
    
    optActual.Enabled = g_Params.ActualTradesEditable
    optActual.Visible = g_Params.ActualTradesEditable
    optActual.Value = g_Params.ActualTradesEditable And (m_NewTrd.Status = enTsRealtime)
    
    chkMark.Value = IIf(m_NewTrd.Mark <> 0, 1, 0)
    
    txtPrice.Text = m_NewTrd.price
    lblLotSize.Caption = m_NewTrd.LotSize
    txtQuantity.Text = m_NewTrd.Quantity
    lblTotalPrice.Caption = Format$(m_NewTrd.TotalPrice, "#,##0.00#####")
    
    lblTitle(4).ForeColor = IIf(m_NewTrd.Quantity > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    m_bIsProviderConnected = False
    
    'tmrPrice.Enabled = g_Params.ActualTradesEditable Or m_NewTrd.Status = enTsSimulated Or m_NewTrd.Status = enTsManual 'And (m_OldTrd Is Nothing)
    tmrPrice.Enabled = (m_OldTrd Is Nothing)
    
    CenterFormOnMonitor Me, m_frmOwner
    m_bInit = False
    Exit Sub
EH:
    gCmn.ErrorHandler "Fail to init trade editor."
End Sub

Private Sub btnStrategyNew_Click()
    On Error Resume Next
    If Not m_NewTrd Is Nothing Then CreateAndSaveNewStrategy cbxStrategy, Me
End Sub

Private Sub cbxStrategy_Click()
    On Error Resume Next
    If cbxStrategy.ListIndex <> 0 Then
        Set m_NewTrd.Strategy = g_Strategy(cbxStrategy.ItemData(cbxStrategy.ListIndex))
    Else
        Set m_NewTrd.Strategy = Nothing
    End If
End Sub

Private Sub chkIsFlex_Click()
On Error Resume Next
    ChangeState
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    CleanUp
End Sub

Private Sub CleanUp()
    On Error Resume Next
    tmrTradeDate.Enabled = False
    
    If Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then
            m_bLastQuoteReqNow = False
            PriceProvider.CancelLastQuote
        End If
        Set PriceProvider = Nothing
        m_bIsProviderConnected = False
    End If
    m_bLastQuoteReqNow = False
    
    Set m_NewTrd = Nothing
    Set m_OldTrd = Nothing
    
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then
        Unload Me
    Else
        Me.Caption = m_sCaption
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
    End If
End Sub

Private Sub btnSubmit_Click()
    On Error GoTo EH
    Dim bSucceed As Boolean
    
    txtQuantity_Change
    
    If m_NewTrd.price < 0 Then
        gCmn.MyMsgBox Me, "Please specify valid trade price.", vbCritical
        txtPrice.SetFocus
        Exit Sub
    End If
    
    If m_NewTrd.Quantity <= 0 Then
        gCmn.MyMsgBox Me, "Please specify valid trade quantity.", vbCritical
        txtQuantity.SetFocus
        Exit Sub
    End If
    
    If (getIsFlex) Then
        If (Not HookTradeSubmit) Then
            Exit Sub
        End If
    End If
    
    If m_bLastQuoteReqNow Then
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: Canlel Last Quote [Call]", m_frmOwner.Caption
            
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: Canlel Last Quote [Done]", m_frmOwner.Caption
    End If
    
    tmrTradeDate.Enabled = False
    
    m_nDefTraderID = m_NewTrd.Trader.ID
    m_nDefBrokerID = m_NewTrd.BrokerID
    m_nDefClearingBrokerID = m_NewTrd.ClearingBrokerID
    
    If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: SaveRegEntries [Call]", m_frmOwner.Caption
            
    SaveRegEntries
    
    If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: SaveRegEntries [Done]", m_frmOwner.Caption
    
    If m_OldTrd Is Nothing Then
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: NewTrade [Call]", m_frmOwner.Caption
            
        bSucceed = g_TradeChannel.NewTrade(m_NewTrd)
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: NewTrade [Done]", m_frmOwner.Caption
            
    Else
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: UpdateTrade [Call]", m_frmOwner.Caption
            
        bSucceed = g_TradeChannel.UpdateTrade(m_NewTrd, m_OldTrd)
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "frmTradeEditor: btnSubmit_Click: UpdateTrade [Done]", m_frmOwner.Caption
    End If
    
    If bSucceed Then
        On Error Resume Next
        Dim sTraderName$: sTraderName = "None"
        Dim sBrokerName$: sBrokerName = "None"
        Dim sClearingBrokerName$: sClearingBrokerName = "None"
        
        sTraderName = m_NewTrd.Trader.Name
        sBrokerName = m_NewTrd.Broker.Name
        sClearingBrokerName = m_NewTrd.ClearingBroker.Name
    
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "New Trade Submited: " _
                                        & "Symbol=""" & m_NewTrd.Symbol & """ " _
                                        & "Quantity=""" & m_NewTrd.Quantity & """ " _
                                        & "Price=""" & m_NewTrd.price & """ " _
                                        & "LotSize=""" & m_NewTrd.LotSize & """ " _
                                        & "IsBuy=""" & m_NewTrd.IsBuy & """ " _
                                        & "Status=""" & m_NewTrd.Status & """ " _
                                        & "SpotReference=""" & m_NewTrd.SpotReference & """ " _
                                        & "TradedIV=""" & m_NewTrd.TradedIV & """ " _
                                        & "Trader=""" & sTraderName & """ " _
                                        & "Broker=""" & sBrokerName & """ " _
                                        & "BrokerCommission=""" & m_NewTrd.BrokerCommission & """ " _
                                        & "ClearingBroker=""" & sClearingBrokerName & """ " _
                                        & "ClearingBrokerCommission=""" & m_NewTrd.ClearingBrokerCommission & """ " _
                                        & "Mark=""" & m_NewTrd.Mark & """ ", m_frmOwner.Caption
                                        
                                        '& "Strategy=""" & IIf(m_NewTrd.Strategy Is Nothing, "None", m_NewTrd.Strategy.Name) & """ "
        On Error GoTo EH
                                        
        Unload Me
    Else
        gCmn.MyMsgBox Me, "Fail to submit trade.", vbCritical
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to submit trade."
End Sub

Private Sub cbxContract_Change()
    On Error Resume Next
    Dim aTrdSrc As EtsGeneralLib.MmTradeInfoAtom, bReloadCommTypes As Boolean
    
    If cbxContract.MatchFound Then
        
        Set aTrdSrc = m_TrdSrc(cbxContract.Value)
        If Not aTrdSrc Is Nothing Then
                    
            bReloadCommTypes = (m_NewTrd.ContractType <> enCtOption) <> (aTrdSrc.ContractType <> enCtOption) _
                                Or (m_NewTrd.ContractType <> enCtFutOption) <> (aTrdSrc.ContractType <> enCtFutOption)
                    
            Set m_NewTrd.Opt = aTrdSrc.Opt
            Set m_NewTrd.OptRoot = aTrdSrc.OptRoot
            Set m_NewTrd.FutRoot = aTrdSrc.FutRoot
            Set m_NewTrd.Fut = aTrdSrc.Fut
            Set m_NewTrd.FutOpt = aTrdSrc.FutOpt
            Set m_NewTrd.Contract = g_Main.GetContract(m_NewTrd.ContractID)

            Set aTrdSrc = Nothing
        End If
                
        txtPrice.Text = m_NewTrd.price
        lblLotSize.Caption = m_NewTrd.LotSize
        txtQuantity.Text = m_NewTrd.Quantity
        
        If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
            lblTitle(8).Visible = True
            lblTitle(9).Visible = True
            txtSpotRef.Visible = True
            txtTradedIV.Visible = True
            
            txtSpotRef.Text = m_NewTrd.SpotReference
            txtTradedIV.Text = m_NewTrd.TradedIV * 100#
            
            cbxContract.SetFocus
        Else
            lblTitle(8).Visible = False
            lblTitle(9).Visible = False
            txtSpotRef.Visible = False
            txtTradedIV.Visible = False
        End If
    
        If bReloadCommTypes Then
            FillCommissionTypeCombo cbxCommType
            FillCommissionTypeCombo cbxClearingCommType
        End If
        
        If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
            AdjustBrokerCommissionsForOptionPriceChange
            AdjustClearingBrokerCommissionsForOptionPriceChange
        Else
            AdjustBrokerCommissions
            AdjustClearingBrokerCommissions
        End If
    
        If Not m_bInit And m_OldTrd Is Nothing Then
            If m_bLastQuoteReqNow Then
                m_bLastQuoteReqNow = False
                PriceProvider.CancelLastQuote
            End If
            tmrPrice.Enabled = True
        End If
    End If
End Sub

Private Function LoadTradeSourcesForNewTrade(ByVal nContractID&, ByVal nUndContractID&) As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset, aUnd As EtsGeneralLib.UndAtom
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, dtSortKey As Date: dtSortKey = 0#
    Dim enContractType As EtsGeneralLib.EtsContractTypeEnum, nID&, bValidContract As Boolean
    
    LoadTradeSourcesForNewTrade = False
    m_TrdSrc.Clear
    
    Set aUnd = g_UnderlyingAll(nUndContractID)
    If aUnd Is Nothing Then Exit Function
    
    If aUnd.UndType <> enCtFutUnd Then
        ' add underlying trade source
        Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
        m_TrdSrc.Add nUndContractID, dtSortKey, aTrd
        Set aTrd.Und = aUnd
        
        If nContractID = nUndContractID Then
            Set m_NewTrd = New EtsGeneralLib.MmTradeInfoAtom
            aTrd.CopyTo m_NewTrd
        End If
        Set aTrd = Nothing
    End If
    
    Set rs = gDBW.usp_MmContractForTradeByUnd_Get(nUndContractID)
    
    While Not rs Is Nothing
        While Not rs.EOF
            nID = ReadLng(rs!iContractID)
            enContractType = ReadLng(rs!iContractTypeID)
            
            If (enContractType = enCtOption _
                Or enContractType = enCtFuture _
                Or enContractType = enCtFutOption) _
                And m_TrdSrc(nID) Is Nothing Then
                
                dtSortKey = dtSortKey + 1#
                Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
                Set aTrd.Und = aUnd
                
                bValidContract = False
                
                Select Case enContractType
                    Case enCtOption
                        Set aTrd.OptRoot = aTrd.Und.Roots(ReadLng(rs!iOptionRootID))
                        If Not aTrd.OptRoot Is Nothing Then
                            bValidContract = True
                            
                            Set aTrd.Opt = New EtsGeneralLib.EtsOptAtom
                            
                            aTrd.Opt.ID = nID
                            aTrd.Opt.Symbol = ReadStr(rs!vcSymbol)
                            aTrd.Opt.RootID = aTrd.OptRoot.ID
                            aTrd.Opt.OptType = ReadByte(rs!tiIsCall)
                            aTrd.Opt.Expiry = ReadDate(rs!dtExpiry)
                            aTrd.Opt.ExpiryOV = ReadDate(rs!dtExpiryOV)
                            aTrd.Opt.TradingClose = ClipDays(ReadDate(rs!dtTradingClose))
                            aTrd.Opt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
                            aTrd.Opt.PriceClose = ReadDbl(rs!fPriceClose)
                            aTrd.Opt.PriceTheoclose = ReadDbl(rs!fPriceTheoClose)
                            aTrd.Opt.Series = ReadStr(rs!vcContractName)
                        End If
                
                    Case enCtFuture, enCtFutOption
                        Set aTrd.FutRoot = aTrd.Und.FutRoots(ReadLng(rs!iFutureRootID))
                        If Not aTrd.FutRoot Is Nothing Then
                            Set aTrd.Fut = aTrd.FutRoot.Futures(ReadLng(rs!iFutureID))
                            
                            If Not aTrd.Fut Is Nothing Then
                                bValidContract = True
                                
                                If enContractType = enCtFutOption Then
                                    Set aTrd.FutOpt = New EtsGeneralLib.EtsFutOptAtom
                                    
                                    aTrd.FutOpt.ID = ReadLng(rs!iContractID)
                                    aTrd.FutOpt.Symbol = ReadStr(rs!vcSymbol)
                                    aTrd.FutOpt.FutID = aTrd.Fut.ID
                                    aTrd.FutOpt.OptType = ReadByte(rs!tiIsCall)
                                    aTrd.FutOpt.Expiry = ReadDate(rs!dtExpiry)
                                    aTrd.FutOpt.ExpiryOV = ReadDate(rs!dtExpiryOV)
                                    aTrd.FutOpt.TradingClose = ClipDays(ReadDate(rs!dtTradingClose))
                                    aTrd.FutOpt.Strike = Round(ReadDbl(rs!fStrike), STRIKE_DECIMALS_COUNT)
                                    aTrd.FutOpt.PriceClose = ReadDbl(rs!fPriceClose)
                                    aTrd.FutOpt.PriceTheoclose = ReadDbl(rs!fPriceTheoClose)
                                    aTrd.FutOpt.Series = ReadStr(rs!vcContractName)
                                End If
                            End If
                        End If
                End Select
                
                If bValidContract Then
                    m_TrdSrc.Add nID, dtSortKey, aTrd
                    
                    If nContractID = nID Then
                        Set m_NewTrd = New EtsGeneralLib.MmTradeInfoAtom
                        aTrd.CopyTo m_NewTrd
                    End If
                End If
                
                Set aTrd = Nothing
            End If
            rs.MoveNext
        Wend
        
        Set rs = rs.NextRecordset
    Wend
    
    Set aUnd = Nothing

    If m_NewTrd Is Nothing Then
        Set m_NewTrd = New EtsGeneralLib.MmTradeInfoAtom
        If Not m_TrdSrc(nUndContractID) Is Nothing Then
            m_TrdSrc(nUndContractID).CopyTo m_NewTrd
        Else
            For Each aTrd In m_TrdSrc
                aTrd.CopyTo m_NewTrd
                GoTo Done
            Next
Done:
        End If
    End If

    LoadTradeSourcesForNewTrade = True
    Exit Function
EH:
    Set aUnd = Nothing
    Set aTrd = Nothing
    gCmn.ErrorHandler "Fail to load contracts list."
End Function

Private Sub LoadTradeSourcesForEditTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim aTrdSrc As EtsGeneralLib.MmTradeInfoAtom
    
    m_TrdSrc.Clear
    Set aTrdSrc = New EtsGeneralLib.MmTradeInfoAtom
    aTrd.CopyTo aTrdSrc
    m_TrdSrc.Add aTrd.ContractID, 0, aTrdSrc
    
    If aTrdSrc.ContractType = enCtOption Then
        If Len(Trim$(aTrdSrc.Opt.Series)) = 0 Then
            aTrdSrc.Opt.Series = aTrdSrc.Und.Symbol & " " & Format$(aTrdSrc.Opt.Expiry, "MMMM YYYY") & _
                             " " & IIf(aTrdSrc.Opt.OptType = enOtCall, "Call", "Put") & _
                             " " & aTrdSrc.Opt.Strike
        End If
        
    ElseIf aTrdSrc.ContractType = enCtFutOption Then
        If Len(Trim$(aTrdSrc.FutOpt.Series)) = 0 Then
            aTrdSrc.FutOpt.Series = aTrdSrc.FutRoot.Symbol & " " & Format$(aTrdSrc.FutOpt.Expiry, "MMMM YYYY") & _
                             " " & IIf(aTrdSrc.FutOpt.OptType = enOtCall, "Call", "Put") & _
                             " " & aTrdSrc.FutOpt.Strike
        End If
    End If
    
    Set aTrdSrc = Nothing
End Sub

Private Sub tmrPrice_Timer()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim aPrc As clsTePriceAtom, aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom, aSynthUnd As EtsGeneralLib.UndAtom
    
    tmrPrice.Enabled = False
    
    If m_bLastQuoteReqNow Or Not m_OldTrd Is Nothing Then Exit Sub
    
    Me.Caption = m_sCaption & " (Last quotes request...)"
    m_bLastQuoteReqNow = True
    
    If Not m_bIsProviderConnected Then
        m_bIsProviderConnected = True
        PriceProvider.Connect
    End If

    m_Price.Clear
    m_PriceByKey.Clear
    
    m_nLastQuoteReqDone = 0
    m_nLastQuoteReqCount = 1
    
    Select Case m_NewTrd.ContractType
        Case enCtOption, enCtStock, enCtIndex
            ' add underlying request
            Set aPrc = m_Price.Add(CStr(m_NewTrd.Und.ID))
            m_PriceByKey.Add CStr(m_NewTrd.Und.UndType) & "_" & CStr(m_NewTrd.Und.Symbol), aPrc
            Set aPrc = Nothing
            
            If m_NewTrd.ContractType = enCtOption Then
                ' add option request
                Set aPrc = m_Price.Add(CStr(m_NewTrd.Opt.ID))
                m_PriceByKey.Add CStr(enCtOption) & "_" & CStr(m_NewTrd.Opt.Symbol), aPrc
                Set aPrc = Nothing
                
                m_nLastQuoteReqCount = m_nLastQuoteReqCount + 1
                
                If m_NewTrd.Und.HaveSyntheticRoots And Not m_NewTrd.Und.SyntheticRoots Is Nothing Then
                    Set aSynthRoot = m_NewTrd.Und.SyntheticRoots(m_NewTrd.OptRoot.ID)
                    If Not aSynthRoot Is Nothing Then
                        If aSynthRoot.SynthRootComponents.Count > 0 Then
                            
                            For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                                Set aSynthUnd = g_UnderlyingAll(aSynthRootComp.UndID)
                                
                                If Not aSynthUnd Is Nothing Then
                                    If m_Price(CStr(aSynthUnd.ID)) Is Nothing Then
                                        
                                        ' add synthetic underlying request
                                        Set aPrc = m_Price.Add(CStr(aSynthUnd.ID))
                                        m_PriceByKey.Add CStr(aSynthUnd.UndType) & "_" & CStr(aSynthUnd.Symbol), aPrc
                                        Set aPrc = Nothing
                                        
                                        m_nLastQuoteReqCount = m_nLastQuoteReqCount + 1
                                        
                                        ' request synthetic underlying
                                        aParam.Exchange = ""
                                        aParam.Symbol = aSynthUnd.Symbol
                                        aParam.Type = IIf(aSynthUnd.UndType = enCtStock, enSTK, enIDX)
                                        PriceProvider.RequestLastQuote aParam
                                    End If
                                    
                                    Set aSynthUnd = Nothing
                                End If
                                
                            Next
                        End If
                    End If
                End If
                
                ' request option
                aParam.Exchange = ""
                aParam.Symbol = m_NewTrd.Opt.Symbol
                aParam.Type = enOPT
                PriceProvider.RequestLastQuote aParam
            End If
        
            ' request underlying
            If (Not g_Exch(m_NewTrd.Und.PrimaryExchangeID) Is Nothing) Then
                aParam.Exchange = g_Exch(m_NewTrd.Und.PrimaryExchangeID).Code
            Else
                aParam.Exchange = ""
            End If
            
            aParam.Symbol = m_NewTrd.Und.Symbol
            aParam.Type = IIf(m_NewTrd.Und.UndType = enCtStock, enSTK, enIDX)
            PriceProvider.RequestLastQuote aParam
            
    
        Case enCtFuture, enCtFutOption
            ' add futures request
            Set aPrc = m_Price.Add(CStr(m_NewTrd.Fut.ID))
            m_PriceByKey.Add CStr(enCtFuture) & "_" & CStr(m_NewTrd.Fut.Symbol), aPrc
            Set aPrc = Nothing
            
            If m_NewTrd.ContractType = enCtFutOption Then
                ' add futures option request
                Set aPrc = m_Price.Add(CStr(m_NewTrd.FutOpt.ID))
                m_PriceByKey.Add CStr(enCtFutOption) & "_" & CStr(m_NewTrd.FutOpt.Symbol), aPrc
                Set aPrc = Nothing
                
                m_nLastQuoteReqCount = m_nLastQuoteReqCount + 1
            
                ' request future option
                aParam.Exchange = ""
                aParam.Symbol = m_NewTrd.FutOpt.Symbol
                aParam.Type = enFOP
                PriceProvider.RequestLastQuote aParam
            End If
            
            ' request future
            aParam.Exchange = ""
            aParam.Symbol = m_NewTrd.Fut.Symbol
            aParam.Type = enFUT
            PriceProvider.RequestLastQuote aParam
    End Select
    
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Fail to request last quotes: " & Err.Description & " (" & Err.Source & ")"
    Set aSynthUnd = Nothing
    Set aPrc = Nothing
    m_bLastQuoteReqNow = False
    PriceProvider.CancelLastQuote
    Me.Caption = m_sCaption
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim aPrc As clsTePriceAtom, sKey$
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    If ReqType = enRequestLastQuote Then
        
        sKey = CStr(PpInstrumentTypeContractType(Request.Type)) & "_" & Request.Symbol
        Set aPrc = m_PriceByKey(sKey)
        If Not aPrc Is Nothing Then
            LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
        End If
        
        If m_nLastQuoteReqDone = m_nLastQuoteReqCount Then
            m_bLastQuoteReqNow = False
            ApplyPrices
        End If
    
        Set aPrc = Nothing
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    Dim aPrc As clsTePriceAtom, sKey$
    
    sKey = CStr(PpInstrumentTypeContractType(Params.Type)) & "_" & Params.Symbol
    Set aPrc = m_PriceByKey(sKey)
    If Not aPrc Is Nothing Then
        aPrc.PriceBid = IIf(Results.BidPrice > BAD_DOUBLE_VALUE, Results.BidPrice, 0#)
        aPrc.PriceAsk = IIf(Results.AskPrice > BAD_DOUBLE_VALUE, Results.AskPrice, 0#)
        aPrc.PriceLast = IIf(Results.LastPrice > BAD_DOUBLE_VALUE, Results.LastPrice, 0#)
        m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
        
        If m_nLastQuoteReqDone = m_nLastQuoteReqCount Then
            m_bLastQuoteReqNow = False
            ApplyPrices
        End If
        
        Set aPrc = Nothing
    End If
End Sub

Private Sub ApplyPrices()
    On Error Resume Next
    Dim aPrc As clsTePriceAtom, aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim dBid#: dBid = 0
    Dim dAsk#: dAsk = 0
    Dim dSpot#: dSpot = 0
    
    
    Me.Caption = m_sCaption
    
    If m_OldTrd Is Nothing Then
'        If m_NewTrd.IsBuy Then
            If m_NewTrd.ContractType = enCtOption Then
                ' apply spot reference
                If Not m_NewTrd.Und.HaveSyntheticRoots Or m_NewTrd.Und.SyntheticRoots Is Nothing Then
                    Set aPrc = m_Price(CStr(m_NewTrd.UndID))
                    m_NewTrd.SpotReference = m_NewTrd.Und.UndPriceProfile.GetUndPriceMid(aPrc.PriceBid, aPrc.PriceAsk, aPrc.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)

                    'm_NewTrd.SpotReference = IIf(aPrc.PriceAsk > 0, aPrc.PriceAsk, aPrc.PriceLast)
                    Set aPrc = Nothing
                    
                Else
                    Set aSynthRoot = m_NewTrd.Und.SyntheticRoots(m_NewTrd.OptRoot.ID)
                    If Not aSynthRoot Is Nothing Then
                        m_NewTrd.SpotReference = aSynthRoot.CashValue
                        
                        
                        If aSynthRoot.SynthRootComponents.Count > 0 Then
                            For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                                Set aPrc = m_Price(CStr(aSynthRootComp.UndID))
                                
                                If Not aPrc Is Nothing Then
                                    If dBid >= 0# Then
                                        dBid = dBid + aPrc.PriceBid * aSynthRootComp.Weight
                                    Else
                                        dBid = BAD_DOUBLE_VALUE
                                    End If
                                    If dAsk >= 0# Then
                                        dAsk = dAsk + aPrc.PriceAsk * aSynthRootComp.Weight
                                    Else
                                        dAsk = BAD_DOUBLE_VALUE
                                    End If
                                    If dSpot >= 0# Then
                                        dSpot = dSpot + aPrc.PriceLast * aSynthRootComp.Weight
                                    Else
                                        dSpot = BAD_DOUBLE_VALUE
                                    End If
                                    Set aPrc = Nothing
                                End If
                            Next
                            
                                                              
                            m_NewTrd.SpotReference = m_NewTrd.SpotReference + _
                                      g_UnderlyingAll(m_NewTrd.UndID).UndPriceProfile.GetUndPriceMid(dBid, dAsk, dSpot, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)

                        End If
                    Else
                        Set aPrc = m_Price(CStr(m_NewTrd.UndID))
                         m_NewTrd.SpotReference = m_NewTrd.Und.UndPriceProfile.GetUndPriceMid(aPrc.PriceBid, aPrc.PriceAsk, aPrc.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)

                        Set aPrc = Nothing
                   
                    End If
                End If
                
            ElseIf m_NewTrd.ContractType = enCtFutOption Then
                ' apply spot reference
                Set aPrc = m_Price(CStr(m_NewTrd.Fut.ID))
                m_NewTrd.SpotReference = m_NewTrd.Fut.UndPriceProfile.GetUndPriceMid(aPrc.PriceBid, aPrc.PriceAsk, aPrc.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                'm_NewTrd.SpotReference = IIf(aPrc.PriceAsk > 0, aPrc.PriceAsk, aPrc.PriceLast)
                Set aPrc = Nothing
            End If
        
            ' apply trade price
            Set aPrc = m_Price(CStr(m_NewTrd.ContractID))
            If Not aPrc Is Nothing Then
                If m_NewTrd.IsBuy Then
                    m_NewTrd.price = IIf(aPrc.PriceAsk > 0, aPrc.PriceAsk, aPrc.PriceLast)
                Else
                    m_NewTrd.price = IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast)
                End If
                Set aPrc = Nothing
            End If
'        Else
'            If m_NewTrd.ContractType = enCtOption Then
'                ' apply spot reference
'                If Not m_NewTrd.Und.HaveSyntheticRoots Or m_NewTrd.Und.SyntheticRoots Is Nothing Then
'                    Set aPrc = m_Price(CStr(m_NewTrd.UndID))
'
'                    m_NewTrd.SpotReference = m_NewTrd.Und.UndPriceProfile.GetUndPriceMid(aPrc.PriceBid, aPrc.PriceAsk, aPrc.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
'                    'm_NewTrd.SpotReference = IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast)
'                    Set aPrc = Nothing
'
'                Else
'                    Set aSynthRoot = m_NewTrd.Und.SyntheticRoots(m_NewTrd.OptRoot.ID)
'                    If Not aSynthRoot Is Nothing Then
'                        m_NewTrd.SpotReference = aSynthRoot.CashValue
'
'                        If aSynthRoot.SynthRootComponents.Count > 0 Then
'                            For Each aSynthRootComp In aSynthRoot.SynthRootComponents
'                                Set aPrc = m_Price(CStr(aSynthRootComp.UndID))
'
'                                If Not aPrc Is Nothing Then
'                                    m_NewTrd.SpotReference = m_NewTrd.SpotReference + _
'                                            IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast) * aSynthRootComp.Weight
'                                    Set aPrc = Nothing
'                                End If
'                            Next
'                        End If
'                    End If
'                End If
'
'            ElseIf m_NewTrd.ContractType = enCtFutOption Then
'                ' apply spot reference
'                Set aPrc = m_Price(CStr(m_NewTrd.Fut.ID))
'                m_NewTrd.SpotReference = IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast)
'                Set aPrc = Nothing
'            End If
'
'            ' apply trade price
'            Set aPrc = m_Price(CStr(m_NewTrd.ContractID))
'            If Not aPrc Is Nothing Then
'                m_NewTrd.Price = IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast)
'                Set aPrc = Nothing
'            End If
'        End If
    End If
    
    m_bInit = True
    
    txtPrice.Text = m_NewTrd.price
    lblLotSize.Caption = m_NewTrd.LotSize
    txtQuantity.Text = m_NewTrd.Quantity
    lblTotalPrice.Caption = Format$(m_NewTrd.TotalPrice, "#,##0.00#####")
    
    If m_OldTrd Is Nothing And (m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption) Then
        g_TradeChannel.CalcTradedIV m_NewTrd
        txtSpotRef.Text = m_NewTrd.SpotReference
        txtTradedIV.Text = m_NewTrd.TradedIV * 100#
        
        m_bInit = False
        
        AdjustBrokerCommissionsForOptionPriceChange
        AdjustClearingBrokerCommissionsForOptionPriceChange
    Else
        m_bInit = False
    End If
End Sub

'Private Function GetRates(UndID As Long, expDate As Date) As Double
'    On Error Resume Next
'    GetRates = 0
'
'    If UndID = 0 Then Exit Function
'
'    Dim dtNow As Date: dtNow = GetNewYorkTime
'
'    Dim bUseMidRates As Boolean, cPosThreshold@, dPos#
'
'    dPos = g_UnderlyingAll(UndID).UndPosForRates
'
'    If GetIrRuleType = enRateBasedOnPosition Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'    Else
'        bUseMidRates = True
'    End If
'
'    If bUseMidRates Then
'           GetRates = GetNeutralRate(dtNow, expDate)
'    Else
'           GetRates = IIf(dPos < 0, GetShortRate(dtNow, expDate), GetLongRate(dtNow, expDate))
'    End If
'
'End Function

Private Function IsBrokerCommissionShouldBeChanged() As Boolean
    On Error Resume Next
    If Not m_OldTrd Is Nothing Then
        IsBrokerCommissionShouldBeChanged = m_OldTrd.BrokerID <> m_NewTrd.BrokerID _
                                    Or m_OldTrd.IsBuy <> m_NewTrd.IsBuy _
                                    Or m_OldTrd.Quantity <> m_NewTrd.Quantity _
                                    Or m_OldTrd.price <> m_NewTrd.price
    Else
        IsBrokerCommissionShouldBeChanged = True
    End If
End Function

Private Function IsClearingBrokerCommissionShouldBeChanged() As Boolean
    On Error Resume Next
    If Not m_OldTrd Is Nothing Then
        IsClearingBrokerCommissionShouldBeChanged = m_OldTrd.ClearingBrokerID <> m_NewTrd.ClearingBrokerID _
                                    Or m_OldTrd.IsBuy <> m_NewTrd.IsBuy _
                                    Or m_OldTrd.Quantity <> m_NewTrd.Quantity _
                                    Or m_OldTrd.price <> m_NewTrd.price
    Else
        IsClearingBrokerCommissionShouldBeChanged = True
    End If
End Function

Private Sub AdjustBrokerCommissionsForOptionPriceChange()
    On Error Resume Next
    If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
        If Not m_NewTrd.Broker Is Nothing Then
            If m_NewTrd.price > g_Params.CommissionLowPremiumLimit Then
                If cbxCommType.ListIndex <> 0 Then
                    m_bCommInit = True
                    cbxCommType.ListIndex = 0
                    m_bCommInit = False
                    AdjustBrokerCommissions
                End If
            Else
                If cbxCommType.ListIndex <> 1 Then
                    m_bCommInit = True
                    cbxCommType.ListIndex = 1
                    m_bCommInit = False
                    AdjustBrokerCommissions
                End If
            End If
        End If
    Else
        AdjustBrokerCommissions
    End If
End Sub

Private Sub AdjustClearingBrokerCommissionsForOptionPriceChange()
    On Error Resume Next
    If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
        If Not m_NewTrd.ClearingBroker Is Nothing Then
            If m_NewTrd.price > g_Params.CommissionLowPremiumLimit Then
                If cbxClearingCommType.ListIndex <> 0 Then
                    m_bCommInit = True
                    cbxClearingCommType.ListIndex = 0
                    m_bCommInit = False
                    AdjustClearingBrokerCommissions
                End If
            Else
                If cbxClearingCommType.ListIndex <> 1 Then
                    m_bCommInit = True
                    cbxClearingCommType.ListIndex = 1
                    m_bCommInit = False
                    AdjustClearingBrokerCommissions
                End If
            End If
        End If
    Else
        AdjustClearingBrokerCommissions
    End If
End Sub

Private Sub AdjustBrokerCommissions(Optional ByVal bForceChange As Boolean = False)
    On Error Resume Next
    Dim nCommType&
    Dim dRate#
    
    If Not m_NewTrd.Broker Is Nothing Then
        If cbxCommType.ListIndex < 0 Then
            m_bCommInit = True
            cbxCommType.ListIndex = 0
            m_bCommInit = False
        End If
    
        nCommType = cbxCommType.ListIndex
    
        If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
            cbxCommType.Enabled = True
            Select Case nCommType
                Case 0 ' Regular
                    dRate = m_NewTrd.Broker.OptRegComm
                Case 1 ' LowPrem
                    dRate = m_NewTrd.Broker.LowPremComm
                Case 2 ' Spread
                    dRate = m_NewTrd.Broker.OptSpreadComm
            End Select
        Else
            cbxCommType.Enabled = False
            dRate = m_NewTrd.Broker.UndComm
        End If
        txtRate.Text = dRate
        txtRate.Enabled = True
        
        If bForceChange Or IsBrokerCommissionShouldBeChanged Then _
            m_NewTrd.BrokerCommission = m_NewTrd.Quantity * m_NewTrd.LotSize * dRate
        txtCommission.Enabled = True
    Else
        m_NewTrd.BrokerCommission = 0#
        txtCommission.Enabled = False
        txtRate.Enabled = False
        
        cbxCommType.Enabled = False
        m_bCommInit = True
        cbxCommType.ListIndex = -1
        m_bCommInit = False
        txtRate.Text = "N/A"
    End If

    txtCommission.Text = m_NewTrd.BrokerCommission
    lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
End Sub

Private Sub AdjustClearingBrokerCommissions(Optional ByVal bForceChange As Boolean = False)
    On Error Resume Next
    Dim nCommType&
    Dim dRate#
    
    If Not m_NewTrd.ClearingBroker Is Nothing Then
        If cbxClearingCommType.ListIndex < 0 Then
            m_bCommInit = True
            cbxClearingCommType.ListIndex = 0
            m_bCommInit = False
        End If
    
        nCommType = cbxClearingCommType.ListIndex
    
        If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
            cbxClearingCommType.Enabled = True
            Select Case nCommType
                Case 0 ' Regular
                    dRate = m_NewTrd.ClearingBroker.OptRegComm
                Case 1 ' LowPrem
                    dRate = m_NewTrd.ClearingBroker.LowPremComm
                Case 2 ' Spread
                    dRate = m_NewTrd.ClearingBroker.OptSpreadComm
            End Select
        Else
            cbxClearingCommType.Enabled = False
            dRate = m_NewTrd.ClearingBroker.UndComm
        End If
        txtClearingRate.Text = dRate
        txtClearingRate.Enabled = True
        
        If bForceChange Or IsClearingBrokerCommissionShouldBeChanged Then _
            m_NewTrd.ClearingBrokerCommission = m_NewTrd.Quantity * m_NewTrd.LotSize * dRate
        txtClearingCommission.Enabled = True
    Else
        m_NewTrd.ClearingBrokerCommission = 0#
        txtClearingCommission.Enabled = False
        txtClearingRate.Enabled = False
        
        cbxClearingCommType.Enabled = False
        m_bCommInit = True
        cbxClearingCommType.ListIndex = -1
        m_bCommInit = False
        txtClearingRate.Text = "N/A"
    End If

    txtClearingCommission.Text = m_NewTrd.ClearingBrokerCommission
    lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
End Sub

Private Sub cbxCommType_Change()
    On Error Resume Next
    If m_bInit Or m_bCommInit Then Exit Sub
    If cbxCommType.MatchFound Then AdjustBrokerCommissions True
End Sub

Private Sub cbxClearingCommType_Change()
    On Error Resume Next
    If m_bInit Or m_bCommInit Then Exit Sub
    If cbxClearingCommType.MatchFound Then AdjustClearingBrokerCommissions True
End Sub

Private Sub cbxBroker_Change()
    On Error Resume Next
    If cbxBroker.MatchFound Then
        Set m_NewTrd.Broker = g_Broker(cbxBroker.Value)
        AdjustBrokerCommissions
    End If
End Sub

Private Sub cbxClearingBroker_Change()
    On Error Resume Next
    If cbxClearingBroker.MatchFound Then
        Set m_NewTrd.ClearingBroker = g_ClearingBroker(cbxClearingBroker.Value)
        AdjustClearingBrokerCommissions
    End If
End Sub

Private Sub cbxBuySell_Change()
    On Error Resume Next
    If Not m_bInit Then
        m_NewTrd.IsBuy = (cbxBuySell.ListIndex = 0)
        Exit Sub
        
        If m_OldTrd Is Nothing Then
            Dim dPrice#: dPrice = 0#
            Dim dUndPrice#: dUndPrice = 0#
            Dim aPrc As clsTePriceAtom, aUndPrc As clsTePriceAtom
            
            Set aPrc = m_Price(CStr(m_NewTrd.ContractID))
            
            If m_NewTrd.ContractType = enCtFutOption Then
                Set aUndPrc = m_Price(CStr(m_NewTrd.FutID))
            Else
                Set aUndPrc = m_Price(CStr(m_NewTrd.UndID))
            End If
            
            If m_NewTrd.IsBuy Then
                dPrice = IIf(aPrc.PriceAsk > 0, aPrc.PriceAsk, aPrc.PriceLast)
                dUndPrice = IIf(aUndPrc.PriceAsk > 0, aUndPrc.PriceAsk, aUndPrc.PriceLast)
            Else
                dPrice = IIf(aPrc.PriceBid > 0, aPrc.PriceBid, aPrc.PriceLast)
                dUndPrice = IIf(aUndPrc.PriceBid > 0, aUndPrc.PriceBid, aUndPrc.PriceLast)
            End If
        
            If dPrice > 0# And m_NewTrd.price <> dPrice Then
                m_NewTrd.price = dPrice
                txtPrice.Text = m_NewTrd.price
            End If
        
            If (m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption) _
                And dUndPrice > 0# And m_NewTrd.SpotReference <> dUndPrice Then
                m_NewTrd.SpotReference = dUndPrice
                g_TradeChannel.CalcTradedIV m_NewTrd
                txtSpotRef.Text = m_NewTrd.SpotReference
                txtTradedIV.Text = m_NewTrd.TradedIV * 100#
            End If
        End If
    Else
        txtPrice.Text = m_NewTrd.price
    End If

    lblTotalPrice.Caption = Format$(m_NewTrd.TotalPrice, "#,##0.00#####")
End Sub

Private Sub cbxTrader_Change()
    On Error Resume Next
    Dim aTrdUnd As EtsGeneralLib.EtsTraderUndAtom
    Dim i&, nStrategyID&, nNewListIndex&
    
    If cbxTrader.MatchFound Then
        Set m_NewTrd.Trader = g_Trader(cbxTrader.Value)
        
        nStrategyID = m_NewTrd.strategyID
        
        FillStrategyCombo
        
        nNewListIndex = 0
        
        If nStrategyID <> 0 Then
            For i = 0 To cbxStrategy.ListCount - 1
                If nStrategyID = cbxStrategy.ItemData(i) Then
                    nNewListIndex = i
                End If
            Next
        End If
        
        cbxStrategy.ListIndex = nNewListIndex
    End If
End Sub

Private Sub optSimulated_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    m_NewTrd.Status = IIf(optSimulated.Value, enTsSimulated, m_NewTrd.Status)
    m_enDefStatus = enTsSimulated
End Sub

Private Sub optManual_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    m_NewTrd.Status = IIf(optManual.Value, enTsManual, m_NewTrd.Status)
    m_enDefStatus = enTsManual
End Sub

Private Sub optActual_Click()
    On Error Resume Next
    If m_bInit Or Not g_Params.ActualTradesEditable Then Exit Sub
    m_NewTrd.Status = IIf(optActual.Value, enTsRealtime, m_NewTrd.Status)
    m_enDefStatus = enTsRealtime
End Sub

Private Sub txtClearingCommission_Change()
    On Error Resume Next
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtClearingCommission.Text)
    
    If m_NewTrd.ClearingBrokerCommission <> dValue Then
        m_NewTrd.ClearingBrokerCommission = dValue
        lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
    
        If m_NewTrd.Quantity * m_NewTrd.LotSize > DBL_EPSILON Then _
            txtClearingRate.Text = Format$(m_NewTrd.ClearingBrokerCommission / (m_NewTrd.Quantity * m_NewTrd.LotSize), "#,##0.00##")
    End If
End Sub

Private Sub txtCommission_Change()
    On Error Resume Next
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtCommission.Text)
    
    If m_NewTrd.BrokerCommission <> dValue Then
        m_NewTrd.BrokerCommission = dValue
        lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
        
        If m_NewTrd.Quantity * m_NewTrd.LotSize > DBL_EPSILON Then _
            txtRate.Text = Format$(m_NewTrd.BrokerCommission / (m_NewTrd.Quantity * m_NewTrd.LotSize), "#,##0.00##")
    End If
End Sub

Private Sub txtPrice_Change()
    On Error Resume Next
    If m_bInit Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtPrice.Text)
    
    If m_NewTrd.price <> dValue Then
        m_NewTrd.price = dValue
        lblTitle(2).ForeColor = IIf(m_NewTrd.price > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
        
        lblTotalPrice.Caption = Format$(m_NewTrd.TotalPrice, "#,##0.00#####")
        
        g_TradeChannel.CalcTradedIV m_NewTrd
        
        txtTradedIV.Text = m_NewTrd.TradedIV * 100#
        
        AdjustBrokerCommissionsForOptionPriceChange
        AdjustClearingBrokerCommissionsForOptionPriceChange
    End If
End Sub

Private Sub txtQuantity_Change()
    On Error Resume Next
    If m_bInit Then Exit Sub
    Dim nValue&: nValue = 0&
    Dim dValue#
    
    
    nValue = CLng(txtQuantity.Text)
    
    If m_NewTrd.Quantity <> nValue Then
        m_NewTrd.Quantity = nValue
        lblTitle(4).ForeColor = IIf(m_NewTrd.Quantity > 0, GCOLOR_LABEL_VALID, GCOLOR_LABEL_INVALID)
        
        lblTotalPrice.Caption = Format$(m_NewTrd.TotalPrice, "#,##0.00#####")
        
        dValue = 0#
        dValue = CDbl(txtRate.Text)
        m_NewTrd.BrokerCommission = dValue * m_NewTrd.Quantity * m_NewTrd.LotSize
        txtCommission.Text = m_NewTrd.BrokerCommission
        
        dValue = 0#
        dValue = CDbl(txtClearingRate.Text)
        m_NewTrd.ClearingBrokerCommission = dValue * m_NewTrd.Quantity * m_NewTrd.LotSize
        txtClearingCommission.Text = m_NewTrd.ClearingBrokerCommission
        
        lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
        
        'AdjustBrokerCommissions
        'AdjustClearingBrokerCommissions
    End If
    
End Sub

Private Sub txtClearingRate_Click()
    On Error Resume Next
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtRate.Text)
    
    m_NewTrd.ClearingBrokerCommission = dValue * m_NewTrd.Quantity * m_NewTrd.LotSize
    lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
    
    txtClearingCommission.Text = m_NewTrd.ClearingBrokerCommission
End Sub

Private Sub txtQuantity_LostFocus()
    txtQuantity_Change
End Sub

Private Sub txtRate_Change()
    On Error Resume Next
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtRate.Text)
    
    m_NewTrd.BrokerCommission = dValue * m_NewTrd.Quantity * m_NewTrd.LotSize
    lblTotalCommission.Caption = Format$(m_NewTrd.TotalCommission, "#,##0.00")
    
    txtCommission.Text = m_NewTrd.BrokerCommission
End Sub

Private Sub txtSpotRef_Change()
    On Error Resume Next
    If m_bInit Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtSpotRef.Text)
    
    If m_NewTrd.SpotReference <> dValue Then
        m_NewTrd.SpotReference = dValue
        
        g_TradeChannel.CalcTradedIV m_NewTrd
        
        txtTradedIV.Text = m_NewTrd.TradedIV * 100#
    End If
End Sub

Private Sub txtTradedIV_Change()
    On Error Resume Next
    If m_bInit Then Exit Sub
    Dim dValue#: dValue = 0#
    
    dValue = CDbl(txtTradedIV.Text) / 100#
    
    If m_NewTrd.TradedIV <> dValue Then
        m_NewTrd.TradedIV = dValue
    End If
End Sub

Private Sub chkMark_Click()
    On Error Resume Next
    If m_bInit Then Exit Sub
    m_NewTrd.Mark = IIf(chkMark.Value <> 0, 1, 0)
End Sub

Private Sub tmrTradeDate_Timer()
    On Error Resume Next
    If m_OldTrd Is Nothing Then
        m_NewTrd.TradeDate = Now
        lblTradeDate.Caption = Format$(m_NewTrd.TradeDate, "Short Date")
        lblTradeTime.Caption = Format$(m_NewTrd.TradeDate, "Long Time")
    End If
End Sub

Private Sub FillContractsCombo()
    On Error Resume Next
    Dim i&, nCount&, aTrdSrc As EtsGeneralLib.MmTradeInfoAtom
    
    With cbxContract
        .ColumnCount = 3
        .BoundColumn = 1
        .TextColumn = 4
        .ColumnWidths = "0;70;;0"
        
        .Clear
        nCount = m_TrdSrc.Count
        If nCount > 0 Then
            i = 0
            For Each aTrdSrc In m_TrdSrc
                .AddItem aTrdSrc.ContractID, i
                .List(i, 1) = aTrdSrc.Symbol ' & vbTab & vbTab & "( " & aTrdSrc.ContractName & " )"
                .List(i, 2) = aTrdSrc.ContractName
                .List(i, 3) = aTrdSrc.Symbol & "           ( " & aTrdSrc.ContractName & " )"
                If aTrdSrc.ContractID = m_NewTrd.ContractID Then
                    .ListIndex = i
                End If
                i = i + 1
            Next
            
            If .ListIndex < 0 Then .ListIndex = 0
        End If
        
        .Enabled = (m_OldTrd Is Nothing)
    End With
End Sub

Private Sub FillTraderCombo()
    On Error Resume Next
    Dim i&, aTrader As EtsGeneralLib.EtsTraderAtom
    
    With cbxTrader
        .ColumnCount = 2
        .BoundColumn = 1
        .TextColumn = 2
        .ColumnWidths = "0;-1;"
        
        .Clear
        
        i = 0
        For Each aTrader In g_Trader
            .AddItem aTrader.ID, i
            .List(i, 1) = aTrader.Name
            If aTrader.ID = m_NewTrd.Trader.ID Then
                .ListIndex = i
            End If
            i = i + 1
        Next
        
        If .ListCount > 0 And .ListIndex < 0 Then
            .ListIndex = 0
        End If
    End With
End Sub

Private Sub FillStrategyCombo()
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, aTrdUnd As EtsGeneralLib.EtsTraderUndAtom
    
    cbxStrategy.Clear
    
    cbxStrategy.AddItem "<None>"
    cbxStrategy.ItemData(cbxStrategy.NewIndex) = 0
    
    If Not m_NewTrd.Trader Is Nothing Then
        Set aTrdUnd = m_NewTrd.Trader.Und(m_NewTrd.UndID)
        If Not aTrdUnd Is Nothing Then
            For Each aStrategy In aTrdUnd.Strategy
                cbxStrategy.AddItem aStrategy.Name
                cbxStrategy.ItemData(cbxStrategy.NewIndex) = aStrategy.ID
            Next
            Set aTrdUnd = Nothing
        End If
    End If
End Sub

Private Sub FillBrokerCombo(ByRef aCombo As MSForms.ComboBox, aBrokers As EtsGeneralLib.EtsBrokerColl, nValue As Long)
    On Error Resume Next
    Dim i&, aBroker As EtsGeneralLib.EtsBrokerAtom
    
    aCombo.ColumnCount = 2
    aCombo.BoundColumn = 1
    aCombo.TextColumn = 2
    aCombo.ColumnWidths = "0;-1;"
    
    aCombo.Clear
    
    aCombo.AddItem 0, 0
    aCombo.List(0, 1) = "<None>"
    If nValue = 0 Then aCombo.ListIndex = 0
    
    i = 1
    For Each aBroker In aBrokers
        aCombo.AddItem aBroker.ID, i
        aCombo.List(i, 1) = aBroker.Name
        If aBroker.ID = nValue Then
            aCombo.ListIndex = i
        End If
        i = i + 1
    Next
    
    If aCombo.ListCount > 0 And aCombo.ListIndex < 0 Then
        aCombo.ListIndex = 0
    End If
End Sub

Private Sub FillCommissionTypeCombo(ByRef aCombo As MSForms.ComboBox)
    On Error Resume Next
    aCombo.ColumnCount = 2
    aCombo.BoundColumn = 1
    aCombo.TextColumn = 2
    aCombo.ColumnWidths = "0;-1;"
    
    aCombo.Clear
    
    If m_NewTrd.ContractType = enCtOption Or m_NewTrd.ContractType = enCtFutOption Then
        aCombo.AddItem 0, 0
        aCombo.List(0, 1) = "Regular"
    
        aCombo.AddItem 1, 1
        aCombo.List(1, 1) = "LowPrem"
    
        aCombo.AddItem 2, 2
        aCombo.List(2, 1) = "Spread"
    Else
        aCombo.AddItem 0, 0
        aCombo.List(0, 1) = "Stock"
    End If
End Sub

Private Sub ReadRegEntries()
    On Error Resume Next
    
    If g_CurTraderID = 0 Then
        m_nDefTraderID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "TradeEditor", "TraderID", 0)
    Else
        m_nDefTraderID = g_CurTraderID
    End If
    m_nDefBrokerID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "TradeEditor", "BrokerID", 0)
    m_nDefClearingBrokerID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "TradeEditor", "ClearingBrokerID", 0)
    m_enDefStatus = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "TradeEditor", "TradeStatus", enTsSimulated)
    
    If Not g_Params.ActualTradesEditable Then
        If m_enDefStatus <> enTsManual And m_enDefStatus <> enTsSimulated Then m_enDefStatus = enTsSimulated
    Else
        If m_enDefStatus <> enTsManual And m_enDefStatus <> enTsSimulated _
            And m_enDefStatus <> enTsRealtime Then m_enDefStatus = enTsSimulated
    End If
End Sub

Private Sub SaveRegEntries()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "TradeEditor", "TraderID", m_nDefTraderID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "TradeEditor", "BrokerID", m_nDefBrokerID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "TradeEditor", "ClearingBrokerID", m_nDefClearingBrokerID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "TradeEditor", "TradeStatus", m_enDefStatus
End Sub

Private Sub InitNewTradeTrader()
    On Error Resume Next
    If g_CurTraderID <> 0 Then
        ' take looged in trader
        Set m_NewTrd.Trader = g_Trader(g_CurTraderID)
    Else
        ' take last trade trader
        Set m_NewTrd.Trader = g_Trader(m_nDefTraderID)
        If m_NewTrd.Trader Is Nothing Then
            ' take first trader
            Dim aTrader As EtsGeneralLib.EtsTraderAtom
            For Each aTrader In g_Trader
                Set m_NewTrd.Trader = aTrader
                Exit For
            Next
        End If
    End If
End Sub

'------- flex option stuff-----------------------------'
Private Sub InitFlexContractControls()
On Error Resume Next
    FillSettlementCombo
    FillStyleCombo
    FillOptionTypeCombo
End Sub

Private Sub FillSettlementCombo()
    On Error GoTo error_handler
    '--------------------------'

    Dim itemIndex As Long
    
    '---- initialize Settlement combo box ----'
    itemIndex = 0
    cmbSettlementType.Clear
    
    ' add default value'
    cmbSettlementType.AddItem "<undefined>", itemIndex
    cmbSettlementType.ItemData(itemIndex) = EtsGeneralLib.enSTUndefined
    
    itemIndex = itemIndex + 1
    cmbSettlementType.AddItem "<AM Settlemnt>", itemIndex
    cmbSettlementType.ItemData(itemIndex) = EtsGeneralLib.enSTAM
    
    itemIndex = itemIndex + 1
    cmbSettlementType.AddItem "<PM Settlemnt>", itemIndex
    cmbSettlementType.ItemData(itemIndex) = EtsGeneralLib.enSTPM
    
    cmbSettlementType.ListIndex = 0
Exit Sub
error_handler:
    Debug.Assert True
    Debug.Print "Error on intializing Settlement combo. Error description: " & Err.Description
End Sub

Private Sub FillStyleCombo()
    On Error GoTo error_handler
    '--------------------------'

    Dim itemIndex As Long
    
    '---- initialize Settlement combo box ----'
    itemIndex = 0
    cmbExerciseStyle.Clear
    
    ' add default value'
    cmbExerciseStyle.AddItem "American", itemIndex
    cmbExerciseStyle.ItemData(itemIndex) = enEsAmerican
    
    itemIndex = itemIndex + 1
    cmbExerciseStyle.AddItem "European", itemIndex
    cmbExerciseStyle.ItemData(itemIndex) = enEsEuropean
        
    cmbExerciseStyle.ListIndex = 0
Exit Sub
error_handler:
    Debug.Assert True
    Debug.Print "Error on intializing Style combo. Error description: " & Err.Description
End Sub

Private Sub FillOptionTypeCombo()
    On Error GoTo error_handler
    '--------------------------'
    
    Dim itemIndex As Long
    
    '---- initialize Settlement combo box ----'
    itemIndex = 0
    cmbOptionType.Clear
    
    ' add default value'
    cmbOptionType.AddItem "<Call/Put>", itemIndex
    cmbOptionType.ItemData(itemIndex) = BAD_LONG_VALUE
    
    itemIndex = itemIndex + 1
    
    cmbOptionType.AddItem "Call", itemIndex
    cmbOptionType.ItemData(itemIndex) = enOtCall
    
    itemIndex = itemIndex + 1
    cmbOptionType.AddItem "Put", itemIndex
    cmbOptionType.ItemData(itemIndex) = enOtPut
        
    cmbOptionType.ListIndex = 0
Exit Sub
error_handler:
    Debug.Assert True
    Debug.Print "Error on intializing OptionType combo. Error description: " & Err.Description
End Sub

Private Function getExpirationDate() As Date
        
    If (dtpExpiry.Value < Date) Then
        Err.Raise vbObjectError + 1, "Trade Editor", "Incorrect expiration date."
    End If
    
    getExpirationDate = dtpExpiry.Value
    
End Function

Private Function getOptionType() As EtsGeneralLib.EtsOptionTypeEnum
    
    Dim lListIndex As Long
    Dim lItemValue As Long
    
    lListIndex = cmbOptionType.ListIndex
    lItemValue = cmbOptionType.ItemData(lListIndex)
    
    If (lItemValue = BAD_LONG_VALUE) Then
        Err.Raise vbObjectError + 2, "Trade Editor", "Option type not defined."
    End If
    
    getOptionType = IIf(lItemValue = CLng(enOtCall), enOtCall, enOtPut)
    
End Function

Private Function getExerciseStyle() As EtsGeneralLib.ExerciseStyleEnum
    
    Dim lListIndex As Long
    Dim lItemValue As Long
    
    lListIndex = cmbExerciseStyle.ListIndex
    lItemValue = cmbExerciseStyle.ItemData(lListIndex)
    
    getExerciseStyle = IIf(lItemValue = CLng(EtsGeneralLib.enEsAmerican), EtsGeneralLib.enEsAmerican, EtsGeneralLib.enEsEuropean)
    
End Function

Private Function getSettlementType() As EtsGeneralLib.SettlementTypeEnum
    
    Dim lListIndex As Long
    Dim lItemValue As Long
    
    lListIndex = cmbSettlementType.ListIndex
    lItemValue = cmbSettlementType.ItemData(lListIndex)
    
    If (lItemValue = CLng(EtsGeneralLib.enSTAM)) Then
        getSettlementType = enSTAM
    ElseIf (lItemValue = CLng(EtsGeneralLib.enSTPM)) Then
        getSettlementType = enSTPM
    ElseIf (lItemValue = CLng(EtsGeneralLib.enSTUndefined)) Then
        Err.Raise vbObjectError + 3, "Trade Editor", "Settlement type not defined."
    End If
    
End Function

Private Function getStrike() As Double
On Error GoTo error_handler
'--------------------------'

    Dim dStrike As Double
    
    dStrike = CDbl(txtStrike.Text)
        
    getStrike = dStrike
    
    Exit Function
error_handler:
    If (Err.Number = 13 Or dStrike <= 0#) Then 'check type mismatch
        Err.Raise vbObjectError + 4, "Trade Editor", "Invalid strike value."
    End If
End Function

Private Function getAssetID() As Long
    If (m_AssetID <= 0) Then
        Err.Raise vbObjectError + 5, "Trade Editor", "Asset is not initialized."
    End If
    getAssetID = m_AssetID
End Function

Private Function getFlexOptionContract() As IContract
On Error GoTo error_handler
'--------------------------'
    
    Dim valuationTime As Date
    Dim settlementType As EtsGeneralLib.SettlementTypeEnum
    
    settlementType = getSettlementType
    
    If (settlementType = enSTAM) Then
        valuationTime = CDate("9:30AM")
    ElseIf (settlementType = enSTPM) Then
        valuationTime = CDate("4:15PM")
    ElseIf (settlementType = enSTUndefined) Then
        valuationTime = CDate("4:00PM")
    End If
    
    Set getFlexOptionContract = g_Main.CreateFlexContract(getAssetID, _
                                                            getExpirationDate, _
                                                            getStrike, _
                                                            getOptionType, _
                                                            CDate("4:00PM"), _
                                                            valuationTime, _
                                                            getExerciseStyle)

    
    Exit Function

error_handler:
    If (Err.Number >= vbObjectError + 1 And Err.Number <= vbObjectError + 5) Then
        MsgBox Err.Description, , "Check flex option params."
    Else
        Debug.Print "Error while trying to create FlexContract. Error: " & Err.Description
    End If
    
    Set getFlexOptionContract = Nothing
    
End Function
'--------------------------------------------------------------------------------------'
Private Function getIsFlex() As Boolean
On Error Resume Next
    getIsFlex = CBool(chkIsFlex.Value)
End Function

Private Sub ChangeState()
On Error Resume Next

    InitFlexContractControls
    
    cbxContract.Enabled = Not getIsFlex()
        
    txtStrike.Enabled = getIsFlex
    dtpExpiry.Enabled = getIsFlex
    cmbOptionType.Enabled = getIsFlex
    cmbExerciseStyle.Enabled = getIsFlex
    cmbSettlementType.Enabled = getIsFlex
    
End Sub

Private Function HookTradeSubmit() As Boolean
    On Error GoTo error_handler
    '--------------------------'
        HookTradeSubmit = False
    
        If (getIsFlex) Then
            Dim new_option As IOptionContract
            Set new_option = getFlexOptionContract
            
            If (Not new_option Is Nothing) Then
                Set m_NewTrd.Opt = new_option
                Set m_NewTrd.OptRoot = new_option.Root
                Set m_NewTrd.Contract = new_option
                
                HookTradeSubmit = True
            End If
        End If
        
    Exit Function
error_handler:
    Debug.Print "Error while hook flex contract submit. Error: " & Err.Description
End Function
