VERSION 5.00
Object = "{BEEECC20-4D5F-4F8B-BFDC-5D9B6FBDE09D}#1.0#0"; "vsflex8.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlMktStruct 
   Alignable       =   -1  'True
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   ClientHeight    =   7365
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   12270
   ScaleHeight     =   7365
   ScaleWidth      =   12270
   Begin ElladaFlatControls.FlatButton cmdAsset 
      Height          =   300
      Left            =   10080
      TabIndex        =   80
      Top             =   1680
      Width           =   1215
      _ExtentX        =   2143
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Asset"
   End
   Begin VB.PictureBox picLeft 
      Align           =   3  'Align Left
      Appearance      =   0  'Flat
      AutoSize        =   -1  'True
      BackColor       =   &H00733429&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   7365
      Left            =   0
      Picture         =   "ctlMktStruct.ctx":0000
      ScaleHeight     =   7365
      ScaleWidth      =   2220
      TabIndex        =   0
      Tag             =   "noauto"
      Top             =   0
      Width           =   2220
      Begin ElladaFlatControls.ActiveLabel btnContractCancel 
         Height          =   255
         Left            =   120
         TabIndex        =   20
         Tag             =   "red"
         Top             =   2640
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   450
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Cancel"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractImport 
         Height          =   195
         Left            =   120
         TabIndex        =   21
         Tag             =   "red"
         Top             =   3960
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Import Current ..."
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractApply 
         Height          =   195
         Left            =   120
         TabIndex        =   19
         Tag             =   "red"
         Top             =   2400
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Apply"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractDelete 
         Height          =   195
         Left            =   120
         TabIndex        =   18
         Tag             =   "red"
         Top             =   2040
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Delete"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractStock 
         Height          =   195
         Left            =   120
         TabIndex        =   17
         Tag             =   "red"
         Top             =   240
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Stock"
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnOptionDelete 
         Height          =   195
         Left            =   120
         TabIndex        =   24
         Top             =   6840
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   12632064
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   16777088
         Caption         =   "Delete"
         Alignment       =   1
         PressForeColor  =   16777152
      End
      Begin ElladaFlatControls.ActiveLabel btnOptionEdit 
         Height          =   195
         Left            =   120
         TabIndex        =   23
         Top             =   6600
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   12632064
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   16777088
         Caption         =   "Edit..."
         Alignment       =   1
         PressForeColor  =   16777152
      End
      Begin ElladaFlatControls.ActiveLabel btnOptionAdd 
         Height          =   195
         Left            =   120
         TabIndex        =   22
         Top             =   6360
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   12632064
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   16777088
         Caption         =   "Add..."
         Alignment       =   1
         PressForeColor  =   16777152
      End
      Begin ElladaFlatControls.ActiveLabel btnUnd 
         Height          =   195
         Left            =   120
         TabIndex        =   43
         Tag             =   "red"
         Top             =   4320
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Underlyings List ..."
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnSymbLoadIV 
         Height          =   195
         Left            =   120
         TabIndex        =   47
         Tag             =   "red"
         Top             =   4680
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Load Symbols from IV"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnCAExecute 
         Height          =   195
         Left            =   120
         TabIndex        =   49
         Tag             =   "red"
         Top             =   3480
         Visible         =   0   'False
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "New Corporate Action"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnCAList 
         Height          =   195
         Left            =   120
         TabIndex        =   50
         Tag             =   "red"
         Top             =   3720
         Visible         =   0   'False
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Corporate Actions List"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnStockToIndex 
         Height          =   195
         Left            =   120
         TabIndex        =   54
         Tag             =   "red"
         Top             =   4980
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Convert To Index"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnUpdateComposition 
         Height          =   195
         Left            =   120
         TabIndex        =   68
         Tag             =   "red"
         Top             =   5280
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Update Composition"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractAdd 
         Height          =   195
         Left            =   120
         TabIndex        =   72
         Tag             =   "red"
         Top             =   1800
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "New"
         Alignment       =   1
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractIndex 
         Height          =   195
         Left            =   120
         TabIndex        =   73
         Tag             =   "red"
         Top             =   480
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Index"
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractFutures 
         Height          =   195
         Left            =   120
         TabIndex        =   74
         Tag             =   "red"
         Top             =   720
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Futures"
         PressForeColor  =   12648447
      End
      Begin ElladaFlatControls.ActiveLabel btnContractFunderlying 
         Height          =   195
         Left            =   120
         TabIndex        =   75
         Tag             =   "red"
         Top             =   960
         Width           =   1875
         _ExtentX        =   3307
         _ExtentY        =   344
         BackColor       =   7549993
         ForeColor       =   33023
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8438015
         Caption         =   "Futre's Underlying"
         PressForeColor  =   12648447
      End
      Begin MSForms.ComboBox cboFutUndType 
         Height          =   300
         Left            =   120
         TabIndex        =   64
         Top             =   5520
         Width           =   1935
         VariousPropertyBits=   681592859
         BorderStyle     =   1
         DisplayStyle    =   7
         Size            =   "3413;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
   End
   Begin VB.PictureBox picMain 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   6495
      Left            =   2280
      ScaleHeight     =   6495
      ScaleWidth      =   9615
      TabIndex        =   16
      Top             =   0
      Width           =   9615
      Begin ElladaFlatControls.FlatButton btnSurface 
         Height          =   300
         Left            =   6360
         TabIndex        =   86
         Top             =   1680
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Vola Surfaces"
      End
      Begin VB.Frame Shape2 
         Appearance      =   0  'Flat
         BackColor       =   &H00FFFFFF&
         ForeColor       =   &H00FFFFFF&
         Height          =   975
         Left            =   120
         TabIndex        =   76
         Top             =   2040
         Width           =   2895
         Begin VB.OptionButton opAmerican 
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            Caption         =   "American Options"
            ForeColor       =   &H80000008&
            Height          =   255
            Left            =   120
            TabIndex        =   78
            Top             =   240
            Value           =   -1  'True
            Width           =   1695
         End
         Begin VB.OptionButton opEuropean 
            Appearance      =   0  'Flat
            BackColor       =   &H80000005&
            Caption         =   "European Options"
            ForeColor       =   &H80000008&
            Height          =   255
            Left            =   120
            TabIndex        =   77
            Top             =   600
            Width           =   1695
         End
      End
      Begin MSComCtl2.DTPicker dtMaturityPicker 
         Height          =   300
         Left            =   6360
         TabIndex        =   71
         Top             =   1320
         Visible         =   0   'False
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         _Version        =   393216
         CustomFormat    =   "MMM,dd yyy hh:mm tt"
         Format          =   20447233
         CurrentDate     =   38974
      End
      Begin VB.CheckBox chkDysplayAmountInFCE 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Display Dlt&&Gma in Future Contract equivalent"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   1680
         TabIndex        =   59
         Top             =   1680
         Width           =   3735
      End
      Begin VB.OptionButton rbUseCustFreq 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Customl Periodical"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   360
         TabIndex        =   61
         Tag             =   "1"
         Top             =   3480
         Width           =   1695
      End
      Begin VB.OptionButton rbUseCustStream 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Custom Stream"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   360
         TabIndex        =   60
         Tag             =   "1"
         Top             =   4200
         Width           =   1695
      End
      Begin VB.OptionButton rbUseBasketIndex 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Basket Index"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   360
         TabIndex        =   83
         Tag             =   "1"
         Top             =   4920
         Width           =   1695
      End
      Begin VB.OptionButton rbUseIndexYield 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Index Yield"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   360
         TabIndex        =   82
         Tag             =   "1"
         Top             =   4560
         Width           =   1695
      End
      Begin VB.OptionButton rbUseMarketDiv 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Market Dividends"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   360
         TabIndex        =   81
         Tag             =   "1"
         Top             =   3840
         Width           =   1695
      End
      Begin VB.TextBox txtLotSize 
         Appearance      =   0  'Flat
         Enabled         =   0   'False
         Height          =   300
         Left            =   4800
         TabIndex        =   57
         Top             =   1320
         Width           =   1335
      End
      Begin VB.TextBox txtKurt 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   6360
         TabIndex        =   53
         Top             =   1320
         Width           =   1335
      End
      Begin VB.TextBox txtSkew 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   4800
         TabIndex        =   52
         Top             =   1320
         Width           =   1335
      End
      Begin VB.TextBox txtDivAmt 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   1
         Left            =   6360
         TabIndex        =   11
         Top             =   3480
         Width           =   1335
      End
      Begin VB.TextBox txtYield 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   4800
         TabIndex        =   5
         Top             =   4560
         Width           =   1335
      End
      Begin MSComCtl2.DTPicker dtpDivDate 
         Height          =   300
         Index           =   0
         Left            =   4800
         TabIndex        =   7
         Top             =   3840
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         _Version        =   393216
         Format          =   20447233
         CurrentDate     =   36960
      End
      Begin VB.TextBox txtDivAmt 
         Appearance      =   0  'Flat
         Height          =   300
         Index           =   0
         Left            =   6360
         TabIndex        =   8
         Top             =   3840
         Width           =   1335
      End
      Begin ElladaFlatControls.FlatButton btnMarketValues 
         Height          =   300
         Left            =   3480
         TabIndex        =   14
         Top             =   1680
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Prices..."
      End
      Begin ElladaFlatControls.FlatButton btnGroups 
         Height          =   300
         Left            =   120
         TabIndex        =   12
         Top             =   1680
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Groups..."
      End
      Begin VB.TextBox txtSymbol 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   120
         TabIndex        =   2
         Top             =   720
         Width           =   1335
      End
      Begin VB.TextBox txtName 
         Appearance      =   0  'Flat
         Height          =   300
         Left            =   1680
         TabIndex        =   3
         Tag             =   "text"
         Top             =   720
         Width           =   5535
      End
      Begin VSFlex8Ctl.VSFlexGrid grdOptions 
         Height          =   1095
         Left            =   120
         TabIndex        =   15
         Top             =   5400
         Width           =   8835
         _cx             =   15584
         _cy             =   1931
         Appearance      =   0
         BorderStyle     =   1
         Enabled         =   -1  'True
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
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
         BackColorBkg    =   -2147483643
         BackColorAlternate=   -2147483643
         GridColor       =   -2147483633
         GridColorFixed  =   -2147483632
         TreeColor       =   -2147483632
         FloodColor      =   192
         SheetBorder     =   -2147483642
         FocusRect       =   3
         HighLight       =   2
         AllowSelection  =   0   'False
         AllowBigSelection=   0   'False
         AllowUserResizing=   0
         SelectionMode   =   0
         GridLines       =   1
         GridLinesFixed  =   1
         GridLineWidth   =   1
         Rows            =   50
         Cols            =   10
         FixedRows       =   1
         FixedCols       =   1
         RowHeightMin    =   0
         RowHeightMax    =   0
         ColWidthMin     =   0
         ColWidthMax     =   0
         ExtendLastCol   =   0   'False
         FormatString    =   ""
         ScrollTrack     =   -1  'True
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
         TabBehavior     =   1
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
         Begin MSComCtl2.DTPicker dtPicker2 
            Height          =   300
            Left            =   5160
            TabIndex        =   85
            Top             =   240
            Visible         =   0   'False
            Width           =   2295
            _ExtentX        =   4048
            _ExtentY        =   529
            _Version        =   393216
            CustomFormat    =   "MMM,dd yyy hh:mm tt"
            Format          =   20447235
            CurrentDate     =   38974.5416666667
         End
      End
      Begin ElladaFlatControls.ActiveLabel btnContractNav 
         Height          =   195
         Index           =   4
         Left            =   5400
         TabIndex        =   28
         Top             =   165
         Width           =   1020
         _ExtentX        =   1799
         _ExtentY        =   344
         BackColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Last >>"
      End
      Begin ElladaFlatControls.ActiveLabel btnContractNav 
         Height          =   195
         Index           =   3
         Left            =   4320
         TabIndex        =   27
         Top             =   165
         Width           =   930
         _ExtentX        =   1640
         _ExtentY        =   344
         BackColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Next >"
      End
      Begin ElladaFlatControls.ActiveLabel btnContractNav 
         Height          =   195
         Index           =   2
         Left            =   1560
         TabIndex        =   26
         Top             =   165
         Width           =   1215
         _ExtentX        =   2143
         _ExtentY        =   344
         BackColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "< Previous"
      End
      Begin ElladaFlatControls.ActiveLabel btnContractNav 
         Height          =   195
         Index           =   1
         Left            =   600
         TabIndex        =   25
         Top             =   165
         Width           =   885
         _ExtentX        =   1561
         _ExtentY        =   344
         BackColor       =   16777215
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "<< First"
      End
      Begin ElladaFlatControls.FlatButton btnExchanges 
         Height          =   300
         Left            =   1200
         TabIndex        =   13
         Top             =   1680
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Exchanges..."
      End
      Begin MSComCtl2.DTPicker dtpDivDate 
         Height          =   300
         Index           =   1
         Left            =   4800
         TabIndex        =   10
         Top             =   3480
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         _Version        =   393216
         Format          =   20447233
         CurrentDate     =   36960
      End
      Begin ElladaFlatControls.FlatButton btnComponents 
         Height          =   300
         Left            =   3240
         TabIndex        =   40
         Top             =   4920
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Components..."
      End
      Begin ElladaFlatControls.FlatButton btnComponentsDividends 
         Height          =   300
         Left            =   4800
         TabIndex        =   41
         Top             =   4920
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Dividends..."
      End
      Begin ElladaFlatControls.FlatButton btnOptionRoots 
         Height          =   300
         Left            =   2400
         TabIndex        =   48
         Top             =   1680
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   529
         Enabled         =   0   'False
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Roots..."
      End
      Begin ElladaFlatControls.FlatButton btnBeta 
         Height          =   300
         Left            =   4800
         TabIndex        =   51
         Top             =   1680
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Underlying Beta..."
      End
      Begin VB.CheckBox chkIsActive 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         Caption         =   "Active"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   1680
         TabIndex        =   42
         Top             =   1320
         Width           =   1335
      End
      Begin ElladaFlatControls.FlatButton btnFuturesRoots 
         Height          =   300
         Left            =   3480
         TabIndex        =   55
         Top             =   1320
         Width           =   1095
         _ExtentX        =   1931
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Future roots..."
      End
      Begin ElladaFlatControls.FlatButton btnCustDivs 
         Height          =   300
         Left            =   3240
         TabIndex        =   79
         Top             =   4200
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Dividend Stream"
      End
      Begin ElladaFlatControls.FlatButton AddButton 
         Height          =   300
         Left            =   120
         TabIndex        =   70
         Top             =   5040
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Add"
      End
      Begin ElladaFlatControls.FlatButton DeleteButton 
         Height          =   300
         Left            =   1680
         TabIndex        =   62
         Top             =   5040
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "Delete"
      End
      Begin VSFlex8Ctl.VSFlexGrid fgRoots 
         Height          =   2895
         Left            =   120
         TabIndex        =   69
         Top             =   2040
         Width           =   8895
         _cx             =   15690
         _cy             =   5106
         Appearance      =   0
         BorderStyle     =   1
         Enabled         =   -1  'True
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
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
         BackColorBkg    =   -2147483643
         BackColorAlternate=   -2147483643
         GridColor       =   -2147483633
         GridColorFixed  =   -2147483632
         TreeColor       =   -2147483632
         FloodColor      =   192
         SheetBorder     =   -2147483642
         FocusRect       =   3
         HighLight       =   2
         AllowSelection  =   0   'False
         AllowBigSelection=   0   'False
         AllowUserResizing=   0
         SelectionMode   =   0
         GridLines       =   1
         GridLinesFixed  =   1
         GridLineWidth   =   1
         Rows            =   50
         Cols            =   10
         FixedRows       =   1
         FixedCols       =   1
         RowHeightMin    =   0
         RowHeightMax    =   0
         ColWidthMin     =   0
         ColWidthMax     =   0
         ExtendLastCol   =   0   'False
         FormatString    =   ""
         ScrollTrack     =   -1  'True
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
         TabBehavior     =   1
         OwnerDraw       =   0
         Editable        =   2
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
         Begin VB.Timer Timer1 
            Interval        =   500
            Left            =   7320
            Top             =   2160
         End
         Begin VB.Image imgProp 
            Height          =   180
            Left            =   3360
            Picture         =   "ctlMktStruct.ctx":86D4
            Top             =   240
            Visible         =   0   'False
            Width           =   180
         End
      End
      Begin VB.Label lblDivType 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BorderStyle     =   1  'Fixed Single
         Caption         =   "Dividend Type"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   120
         TabIndex        =   84
         Top             =   3120
         Width           =   2895
      End
      Begin VB.Label lblYield 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Yield (%) :"
         ForeColor       =   &H80000008&
         Height          =   255
         Left            =   3240
         TabIndex        =   36
         Top             =   4560
         Width           =   1215
      End
      Begin VB.Label lblDivFreq 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Div. Frequency :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   1
         Left            =   3240
         TabIndex        =   39
         Top             =   3240
         Width           =   1215
      End
      Begin VB.Label lblDivDate 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Last Div. Date :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   1
         Left            =   4800
         TabIndex        =   38
         Top             =   3240
         Width           =   1215
      End
      Begin VB.Label lblDivAmt 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Last Div. Amount :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   1
         Left            =   6360
         TabIndex        =   37
         Top             =   3240
         Width           =   1455
      End
      Begin VB.Label lblDivAmt 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Last Div. Amount :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   0
         Left            =   6360
         TabIndex        =   35
         Top             =   3240
         Visible         =   0   'False
         Width           =   1455
      End
      Begin VB.Label lblDivDate 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Last Div. Date :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   0
         Left            =   4800
         TabIndex        =   34
         Top             =   3240
         Visible         =   0   'False
         Width           =   1215
      End
      Begin VB.Label lblDivFreq 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Div. Frequency :"
         ForeColor       =   &H80000008&
         Height          =   255
         Index           =   0
         Left            =   3240
         TabIndex        =   33
         Top             =   3240
         Visible         =   0   'False
         Width           =   1215
      End
      Begin VB.Shape Shape1 
         Height          =   2175
         Left            =   120
         Top             =   3120
         Width           =   7695
      End
      Begin MSForms.ComboBox cboType 
         Height          =   300
         Left            =   7320
         TabIndex        =   67
         Top             =   720
         Width           =   1215
         VariousPropertyBits=   679495707
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "2143;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label txtDefCal 
         Height          =   255
         Left            =   7800
         TabIndex        =   66
         Top             =   1320
         Visible         =   0   'False
         Width           =   1095
      End
      Begin VB.Label lblType 
         BackStyle       =   0  'Transparent
         Caption         =   "Type :"
         Height          =   255
         Left            =   7320
         TabIndex        =   65
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblDefCal 
         BackStyle       =   0  'Transparent
         Caption         =   "Default Opt Expiration calendar:"
         Height          =   255
         Left            =   5400
         TabIndex        =   63
         Top             =   1320
         Visible         =   0   'False
         Width           =   2295
      End
      Begin VB.Label lblLotSize 
         BackStyle       =   0  'Transparent
         Caption         =   "Lot Size :"
         Height          =   255
         Left            =   4800
         TabIndex        =   58
         Top             =   1080
         Width           =   1335
      End
      Begin VB.Label lblMaturity 
         BackStyle       =   0  'Transparent
         Caption         =   "Maturity Date :"
         Height          =   255
         Left            =   6360
         TabIndex        =   56
         Top             =   1080
         Width           =   1335
      End
      Begin VB.Label lblSkew 
         BackStyle       =   0  'Transparent
         Caption         =   "Skew :"
         Height          =   255
         Left            =   4800
         TabIndex        =   44
         Top             =   1080
         Width           =   735
      End
      Begin VB.Label lblKurt 
         BackStyle       =   0  'Transparent
         Caption         =   "Kurtosis :"
         Height          =   255
         Left            =   6360
         TabIndex        =   46
         Top             =   1080
         Width           =   735
      End
      Begin MSForms.ComboBox cboDivFreq 
         Height          =   300
         Index           =   1
         Left            =   3240
         TabIndex        =   9
         Top             =   3480
         Width           =   1335
         BorderStyle     =   1
         DisplayStyle    =   7
         Size            =   "2355;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.ComboBox cboDivFreq 
         Height          =   300
         Index           =   0
         Left            =   3240
         TabIndex        =   6
         Top             =   3840
         Width           =   1335
         BorderStyle     =   1
         DisplayStyle    =   7
         Size            =   "2355;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.ComboBox cboContracts 
         Height          =   300
         Left            =   2880
         TabIndex        =   1
         Top             =   120
         Width           =   1215
         VariousPropertyBits=   679495707
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "2143;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lblContractParams 
         Appearance      =   0  'Flat
         AutoSize        =   -1  'True
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Contract Params"
         ForeColor       =   &H00808000&
         Height          =   195
         Left            =   120
         TabIndex        =   32
         Top             =   1380
         Width           =   1530
      End
      Begin MSForms.ComboBox cboPrimExchange 
         Height          =   300
         Left            =   3240
         TabIndex        =   4
         Top             =   2400
         Width           =   1335
         VariousPropertyBits=   679495707
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "2355;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lblSymbol 
         BackStyle       =   0  'Transparent
         Caption         =   "Symbol :"
         Height          =   255
         Left            =   120
         TabIndex        =   31
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblName 
         BackStyle       =   0  'Transparent
         Caption         =   "Name :"
         Height          =   255
         Left            =   1680
         TabIndex        =   30
         Top             =   480
         Width           =   735
      End
      Begin VB.Label lblPrimExchange 
         BackStyle       =   0  'Transparent
         Caption         =   "Prim. Exchange :"
         Height          =   255
         Left            =   3240
         TabIndex        =   29
         Top             =   2160
         Width           =   1575
      End
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Skew :"
      Height          =   255
      Left            =   0
      TabIndex        =   45
      Top             =   0
      Width           =   735
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "Options"
      Begin VB.Menu mnuOptionCallPrices 
         Caption         =   "Call Prices..."
      End
      Begin VB.Menu mnuOptionPutPrices 
         Caption         =   "Put Prices..."
      End
      Begin VB.Menu mnuOptionsLine3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionEdit 
         Caption         =   "Edit Options"
      End
      Begin VB.Menu mnuOptionsLine1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsAdd 
         Caption         =   "Add Options..."
      End
      Begin VB.Menu mnuOptionsLine2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsDelete 
         Caption         =   "Delete options"
      End
      Begin VB.Menu mnuOptionsLine4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsDefSort 
         Caption         =   "Default Options Sorting"
      End
   End
End
Attribute VB_Name = "ctlMktStruct"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

'****************************************************
' type declarations
'****************************************************
' control state enumeration
Public Enum eState
    STATE_NONE = 0
    STATE_CONTRACT_ADD = 1
    STATE_CONTRACT_EDIT = 2
    STATE_CONTRACT_IMPORT = 3
    STATE_LOAD = 4
End Enum

' contract type enumeration
Public Enum eContractType
    GINT_INDEXES = 1
    GINT_STOCKS = 2
    GINT_FUTURES = 4
    GINT_FUTUREOPTIONS = 5
'    GINT_INTEREST = 6
    GINT_COMMODITIES = 6
End Enum

Const COLOR11 = &HFFFFFF
Const COLOR12 = &HEEEEEE
Const COLOR21 = &HDDDDDD
Const COLOR22 = &HCCCCCC

Private Enum GridColumns
    OPT_CALLSYMBOL = 0
    OPT_CALLIMPORTSYMBOL = 1
    OPT_CALLCLOSEPRICE = 2
    OPT_STRIKE = 3
    OPT_EXPIRATION = 4
    OPT_EXPIRYVALUATION = 5
    OPT_PUTSYMBOL = 6
    OPT_PUTIMPORTSYMBOL = 7
    OPT_PUTCLOSEPRICE = 8
    OPT_HIDDENCOLUMN = 9
'    OPT_TRADINGCLOSE = 10
    OPT_COLUMNS_COUNT = 10
    OPT_LAST_COLUMN = OPT_COLUMNS_COUNT - 1
End Enum

Private Enum RootGridColumns
    root_symbol = 0
    ROOT_NAME = 1
    ROOT_IMPORTID = 2
    ROOT_FUTCONTRACTSIZE = 3
    ROOT_OPTCONTRACTSIZE = 4
    ROOT_QUOTEFORMAT = 5
    ROOT_QUOTATIONUNITAMMOUNT = 6
    ROOT_QUOTATIONUNITUNIT = 7
    ROOT_QUOTEDENOMINATION = 8
    ROOT_TICKSIZE = 9
    ROOT_TICKVALUE = 10
    ROOT_HIDDENCOLUMN = 11
    ROOT_COLUMNS_COUNT = 12
    ROOT_LAST_COLUMN = ROOT_COLUMNS_COUNT - 1
End Enum

Private Enum ICGridColumns
    IC_FUTURESYMBOL
    IC_DESCRIPTION
    IC_SIZE
    IC_CLOSEPRICE
    IC_MATURITYDATE
    IC_HIDDENCOLUMN
    IC_COLUMNS_COUNT
    IC_LAST_COLUMN = IC_COLUMNS_COUNT - 1
End Enum


' contract variables
Private Type TContract
    ID As Long
    TypeID As eContractType
    Symbol As String
    Name As String
    ExchangeID As Long
    
    DivFreq As Long
    DivDate As Date
    DivAmt As Double
    
    DivType As EtsGeneralLib.EtsDivTypeEnum
    
    DivUse2 As Boolean
    DivFreq2 As Long
    DivDate2 As Date
    DivAmt2 As Double
    
    Yield As Double
    OptionsStyle As eOptionsStyle
    CalendarType As eCalendarType
    IsBasket As Boolean
    IsActive As Byte
    Skew As Double
    Kurt As Double

    MaturityDate As Date
    LotSize As Long
    GreeksCalcType As Long
End Type

' interest/commodity future list variables
Private Type ICFuture
    CommodityID As Long
    Symbol As String
    Name As String
    priceClose As Double
    MaturityDate As Date
    OptionLotSize As Long
    FutureLotSize As Long
End Type

'****************************************************
' private members
'****************************************************
Private m_State As eState
Private m_ContractType As eContractType
Private m_Contract As TContract
Private m_OldContract As TContract

Private m_rsContractList As ADODB.Recordset
Private m_rsOptionList As ADODB.Recordset
Private m_sContracts As String
Private m_sIndexes As String
Private PrevV As eContractType

Dim m_Roots As New clsFutureRootColl
Dim bMatChanged As Boolean
Dim bDblClickHandled As Boolean
Dim dtFutMaturity As Date

Private m_sCurrentOriginalText As String

Private m_frmCustDivs As New frmCustomDivs
Private m_bFireEvent As Boolean
Private m_bDTPIsDroppedDown As Boolean



'****************************************************
' events
'****************************************************
'Event StateChange()
'Event ContractTypeIDChange()
'Event ContractChange()
'Event ContractListChange()
'Event OptionListChange()

'****************************************************
'****************************************************
'****************************************************

Public Sub Activate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    ExchangeListLoad
    ICTypeListLoad
    SetState STATE_NONE
    SetContractButtonsState
    
    m_sCurrentOriginalText = ""
End Sub

Private Sub AddButton_Click()
On Error GoTo ErrHnd
    AddRow
    AddButton.SetFocus
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub btnBeta_Click()
    On Error GoTo EH
    frmBetas.Execute m_Contract.ID, m_Contract.Symbol, m_sIndexes
    Exit Sub
EH:
    ShowError
End Sub

Private Sub btnCAExecute_Click()
    On Error GoTo EH
    If frmCorpAction.Execute(m_Contract.ID, m_Contract.Symbol, m_sContracts) Then ContractLoad
    Exit Sub
EH:
    ShowError
End Sub

Private Sub btnCAList_Click()
    On Error GoTo EH
    If frmCAList.Execute Then ContractLoad
    Exit Sub
EH:
    ShowError
End Sub

Private Sub btnContractFunderlying_Click()
    On Error Resume Next
    If m_State = STATE_LOAD Or m_ContractType = GINT_COMMODITIES Then Exit Sub
    If g_Params.Commodity Then _
        SetContractType GINT_COMMODITIES
End Sub

Private Sub btnContractFutures_Click()
    On Error Resume Next
    If m_State = STATE_LOAD Or m_ContractType = GINT_FUTURES Then Exit Sub
    SetContractType GINT_FUTURES
End Sub

Private Sub btnContractIndex_Click()
    On Error Resume Next
    If m_State = STATE_LOAD Or m_ContractType = GINT_INDEXES Then Exit Sub
    SetContractType GINT_INDEXES

End Sub

Private Sub btnContractStock_Click()
    On Error Resume Next
    If m_State = STATE_LOAD Or m_ContractType = GINT_STOCKS Then Exit Sub
    SetContractType GINT_STOCKS
End Sub

Private Sub btnCustDivs_Click()
    On Error Resume Next
    If (m_Contract.TypeID = GINT_STOCKS) Or (m_Contract.TypeID = GINT_INDEXES) Then
        m_frmCustDivs.Init m_Contract.ID, m_Contract.Symbol
        m_frmCustDivs.Show vbModal, Me
    End If
End Sub

Private Sub btnFuturesRoots_Click()
    On Error Resume Next
    If ContractCanProceed Then
        If frmFutureRootList.Execute(m_Contract.ID, m_Contract.Symbol) Then ContractLoad
    End If
End Sub

Private Sub btnStockToIndex_Click()
    Dim cType As eContractType
    On Error GoTo EH
    If m_ContractType <> GINT_INDEXES And m_ContractType <> GINT_STOCKS Then Exit Sub
    
    cType = m_ContractType
    
    Screen.MousePointer = vbHourglass
    
    m_OldContract = m_Contract
    
    If gDBW.usp_ContractType_Change(m_Contract.ID) Then
        SetContractType IIf(cType = GINT_STOCKS, GINT_INDEXES, GINT_STOCKS)
        m_Contract = m_OldContract
        ContractLoad
    End If
    
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

Private Sub btnSurface_Click()
    If ContractCanProceed Then
        If frmSubSurfaceEditor.Execute(m_Contract.ID, m_Contract.Symbol) Then ContractLoad
    End If
End Sub

Private Sub btnSymbLoadIV_Click()
    On Error GoTo EH
    Dim iAnswr As VbMsgBoxResult
    Dim ContractCanProceed As Boolean
    
    If (m_State = STATE_CONTRACT_ADD) Or (m_State = STATE_CONTRACT_EDIT) Then
        iAnswr = gCmn.MyMsgBox("Do you wish to save changes?", vbYesNoCancel)
        If iAnswr = vbYes Then
            ' save contract
            ContractCanProceed = ContractSave
        ElseIf iAnswr = vbNo Then
            ' cancel changes
            ContractCanProceed = ContractCancel
        Else
            ContractCanProceed = False
        End If
    Else
        ContractCanProceed = True
    End If
    
    If ContractCanProceed Then
        m_OldContract = m_Contract
        
        frmSymbolsLoadFrIV.Execute
        
        ContractListLoad
        
        m_Contract = m_OldContract
        ContractLoad
    End If
    Exit Sub
EH:
    ShowError
End Sub

Private Sub btnUpdateComposition_Click()
    On Error Resume Next
    Dim frmPrg As New frmProgress
    
    frmPrg.SetDefaultFont
    frmPrg.SetCaption = "Update composition for index " & txtSymbol.Text
    frmPrg.SetStatus = "Load index composition from IV"
    
    frmPrg.Initialize 1, enUpdateBasketIndexInfo, txtSymbol.Text, m_Contract.ID
End Sub

Private Sub cboFutUndType_Change()
On Error Resume Next
    If m_State = STATE_LOAD Then Exit Sub
    ICContractListLoad
    ContractLoad
End Sub

Private Sub cboType_Change()
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub chkDysplayAmountInFCE_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If

End Sub

Private Sub chkIsActive_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub cmdAsset_Click()
    On Error Resume Next
    If ContractCanProceed Then frmAssetGroupEditor.Execute m_Contract.ID, m_Contract.Symbol
End Sub

Private Sub ComboBox1_Change()

End Sub

Private Sub DeleteButton_Click()
On Error GoTo ErrHnd
    DeleteRow fgRoots.RowSel
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Function IsSymbolExists(Symbol As String) As Boolean
    Dim aRoot As clsFutureRootAtom
    Dim bExistsInCollection As Boolean
    Dim bExistsInDB As Boolean
    On Error Resume Next
    
    For Each aRoot In m_Roots
        If aRoot.Symbol = Symbol Then 'And aRoot.State <> ELEM_DELETED Then
            bExistsInCollection = True
            Exit For
        End If
    Next

    If Not bExistsInCollection Then
        bExistsInDB = (0 <> gDBW.usp_BoFutureRootWithSymbol_Exist(Symbol))
    End If
    
    IsSymbolExists = bExistsInCollection Or bExistsInDB
End Function

Private Function GetNextSymbol(sLast As String) As String
    Dim sSymbol$, i&
    On Error Resume Next
    sSymbol = Mid(sLast, 5)
    i = Val(sSymbol) + 1
    GetNextSymbol = "<New " & Str(i) & ">"
End Function

Private Function GetNewSymbol() As String
    Dim sSymbol$, i&
    On Error Resume Next
    sSymbol = "<New>"
    If Not IsSymbolExists(sSymbol) Then
        GetNewSymbol = sSymbol
        Exit Function
    Else
        i = 0
        While True
            i = i + 1
            sSymbol = CStr(i)
            If Len(sSymbol) > 8 Then sSymbol = Right(sSymbol, 8)
            sSymbol = "<New " & sSymbol & ">"
            If Not IsSymbolExists(sSymbol) Then
                GetNewSymbol = sSymbol
                Exit Function
            End If
        Wend
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here save Future Roots Properties to DB
Private Function SaveData(aRoot As clsFutureRootAtom) As Boolean
    Dim Ret As Long
    Ret = 0
    'Dim aRoot As clsFutureRootAtom
    On Error GoTo EH
    'For Each aRoot In m_Roots
        If aRoot.State = ELEM_INSERTED Then
INS:
            Dim bNeedUpdate As Boolean

            Ret = gDBW.usp_BoFutureRoot_Save(Null, m_Contract.ID, aRoot.Symbol, aRoot.Name, _
                aRoot.FutureLotSize, aRoot.OptionLotSize, aRoot.ExportSymbol, _
                aRoot.tiQuoteFormat, aRoot.iQuotationUnitAmmount, aRoot.iQuotationUnitID, aRoot.fQuoteDenomination, aRoot.fTickSize, aRoot.fTickValue)
            If Ret > 0 Then
                aRoot.RootID = Ret
                If bNeedUpdate Then
                    LoadFutureRoots
                End If
            ElseIf Ret = -3 Then
                aRoot.Symbol = GetNextSymbol(aRoot.Symbol)
                bNeedUpdate = True
                GoTo INS
            Else: GoTo EH
            End If
        ElseIf aRoot.State = ELEM_UPDATED Then
            gDBW.usp_BoFutureRoot_Update aRoot.RootID, m_Contract.ID, aRoot.Symbol, aRoot.Name, _
                aRoot.FutureLotSize, aRoot.OptionLotSize, aRoot.ExportSymbol, _
                aRoot.tiQuoteFormat, aRoot.iQuotationUnitAmmount, aRoot.iQuotationUnitID, aRoot.fQuoteDenomination, aRoot.fTickSize, aRoot.fTickValue
        ElseIf aRoot.State = ELEM_DELETED Then
            gDBW.usp_BoFutureRoot_Del aRoot.RootID
        End If
        
'NextRoot:
    'Next
    SaveData = True
    Exit Function
EH:
    Dim sErrMsg$
    If Not aRoot Is Nothing Then
        sErrMsg = "Can't "
        If aRoot.State = ELEM_INSERTED Then
            sErrMsg = sErrMsg & "insert "
        ElseIf aRoot.State = ELEM_UPDATED Then
            sErrMsg = sErrMsg & "update "
        ElseIf aRoot.State = ELEM_DELETED Then
            sErrMsg = sErrMsg & "delete "
        End If
        sErrMsg = sErrMsg & aRoot.Symbol
        sErrMsg = sErrMsg & " future root."
'        If Ret < 0 Then
'            If Ret = -1 Then
'                sErrMsg = sErrMsg & " Underlying for new future root is not specified!"
'            ElseIf Ret = -2 Then
'                sErrMsg = sErrMsg & " Symbol for new future root is not specified!"
'            ElseIf Ret = -3 Or Ret = -6 Then
'                sErrMsg = sErrMsg & " Future root with symbol " & aRoot.Symbol & " is already exists!"
'            ElseIf Ret = -104 Then
'                sErrMsg = sErrMsg & " Future root with import ID " & aRoot.ExportSymbol & " is already exists!"
'            ElseIf Ret = -4 Or Ret = -7 Then
'                sErrMsg = sErrMsg & " Invalid future root maturity calendar specified!"
'            End If
'        End If
        ShowError sErrMsg
    Else
        ShowError "Error while save data"
    End If
'    GoTo NextRoot
End Function


Private Sub AddRow()
    Dim aRoot As clsFutureRootAtom, sKey$, nRow&
    
    sKey = GetNewSymbol
    Set aRoot = m_Roots.Add(sKey)
        
    If Not aRoot Is Nothing Then
        aRoot.FutureLotSize = 100
        aRoot.Symbol = sKey
        aRoot.OptionLotSize = 1
        aRoot.RootID = 0
        aRoot.State = ELEM_INSERTED
        aRoot.Name = "<New Value>"
        
        aRoot.tiQuoteFormat = 0
        aRoot.iQuotationUnitAmmount = 1
        
        aRoot.iQuotationUnitID = Val(Mid(fgRoots.ColComboList(ROOT_QUOTATIONUNITUNIT), 2))
        aRoot.fQuoteDenomination = -1
        aRoot.fTickSize = -1
        aRoot.fTickValue = -1
        
        With fgRoots
             .AddItem ""
             nRow = .Rows - 1
            .RowData(nRow) = aRoot
                    
            .TextMatrix(nRow, root_symbol) = aRoot.Symbol
            .TextMatrix(nRow, ROOT_NAME) = aRoot.Name
            .TextMatrix(nRow, ROOT_IMPORTID) = aRoot.ExportSymbol
            .TextMatrix(nRow, ROOT_FUTCONTRACTSIZE) = aRoot.FutureLotSize
            .Cell(flexcpAlignment, nRow, ROOT_FUTCONTRACTSIZE) = flexAlignRightCenter
            .TextMatrix(nRow, ROOT_OPTCONTRACTSIZE) = aRoot.OptionLotSize
            .Cell(flexcpAlignment, nRow, ROOT_OPTCONTRACTSIZE) = flexAlignRightCenter
            
            .Select nRow, root_symbol
            .EditCell
        End With
        
        'imgSave.Visible = True
    End If
    'mnuDel.Enabled = True
    DeleteButton.Enabled = True
    If Not SaveData(aRoot) Then
        DeleteRow (nRow)
    End If
    
    ContractLoad                    ' to refresh MatCalendar column
End Sub

Private Sub DeleteRow(ByVal Row As Long)
    Dim aRoot As clsFutureRootAtom
    Dim b As Boolean
    
    With fgRoots
        
        If Row < 2 Then Exit Sub
    
        Set aRoot = .RowData(Row)
        
        If Row = .Rows - 1 Then
            .RemoveItem Row
            .Row = Row - 1
            .RowSel = Row - 1
        Else
            .RemoveItem Row
        End If
        
        If .RowSel < 2 Then
            'mnuDel.Enabled = False
            DeleteButton.Enabled = False
        End If
            
        If Not aRoot Is Nothing Then
            aRoot.State = ELEM_DELETED
        End If
    End With
    b = SaveData(aRoot)
    'imgSave.Visible = True

End Sub

Private Function IsExportSymbolExists(ExportSymbol As String) As Boolean
    Dim aRoot As clsFutureRootAtom
    On Error Resume Next
    If Len(ExportSymbol) > 0 Then
        For Each aRoot In m_Roots
            If aRoot.ExportSymbol = ExportSymbol And aRoot.State <> ELEM_DELETED Then
                IsExportSymbolExists = True
                Exit Function
            End If
        Next
    End If
    IsExportSymbolExists = False
End Function


Private Sub dtMaturityPicker_Change()
    bMatChanged = True
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub dtMaturityPicker_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    SendKeys "{F4}"
End Sub

Private Sub dtMaturityPicker_CloseUp()
    If (bMatChanged) Then
        On Error Resume Next
        If m_State = STATE_NONE Then
            SetState STATE_CONTRACT_EDIT
            m_OldContract = m_Contract
            SetContractButtonsState
        End If
        dtFutMaturity = dtMaturityPicker.Value
        bMatChanged = False
    End If
End Sub

Private Sub dtMaturityPicker_LostFocus()
    If (bMatChanged) Then
        On Error Resume Next
        If m_State = STATE_NONE Then
            SetState STATE_CONTRACT_EDIT
            m_OldContract = m_Contract
            SetContractButtonsState
        End If
        dtFutMaturity = dtMaturityPicker.Value
        bMatChanged = False
    End If
End Sub


Private Sub fgRoots_AfterEdit(ByVal Row As Long, ByVal Col As Long)
Dim sValue$, aRoot As clsFutureRootAtom
Dim b As Boolean
    On Error Resume Next
    With fgRoots
        sValue = Trim$(.Cell(flexcpTextDisplay, Row, Col))
        If m_sCurrentOriginalText <> sValue And Row > 1 Then
            Set aRoot = .RowData(Row)
            If Not aRoot Is Nothing Then
        
                If Col = root_symbol Then
                    aRoot.Symbol = sValue
                    m_Roots.Remove (m_sCurrentOriginalText)
                    m_Roots.Add sValue, aRoot
                ElseIf Col = ROOT_IMPORTID Then
                    aRoot.ExportSymbol = sValue
                ElseIf Col = ROOT_NAME Then
                    aRoot.Name = sValue
                ElseIf Col = ROOT_FUTCONTRACTSIZE Then
                    aRoot.FutureLotSize = gCmn.ReadLng(sValue)
                'ElseIf Col = ROOT_OPTCONTRACTSIZE Then
                '    aRoot.OptionLotSize = gCmn.ReadLng(sValue)
                ElseIf Col = ROOT_QUOTEFORMAT Then
                    aRoot.tiQuoteFormat = gCmn.ReadLng(.ComboData)
                ElseIf Col = ROOT_QUOTATIONUNITAMMOUNT Then
                    aRoot.iQuotationUnitAmmount = gCmn.ReadLng(sValue)
                ElseIf Col = ROOT_QUOTATIONUNITUNIT Then
                    aRoot.iQuotationUnitID = gCmn.ReadLng(.ComboData)
                'ElseIf Col = ROOT_QUOTEDENOMINATION Then
                '    aRoot.CalendarType = gCmn.ReadLng(.ComboData)
                ElseIf Col = ROOT_TICKSIZE Then
                    aRoot.fTickSize = gCmn.ReadLng(sValue)
                ElseIf Col = ROOT_TICKVALUE Then
                    aRoot.fTickValue = gCmn.ReadLng(sValue)
                End If
                
                If aRoot.State = ELEM_UNCHANGED Or aRoot.State = ELEM_INSERTED Then
                    aRoot.State = ELEM_UPDATED
                End If
                
                'aRoot.State = ELEM_UPDATED
                b = SaveData(aRoot)
                
            End If
        End If
    End With
End Sub

Private Sub fgRoots_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    Select Case Col
        Case ROOT_OPTCONTRACTSIZE
            Cancel = True
    End Select
    m_sCurrentOriginalText = Trim$(fgRoots.Cell(flexcpTextDisplay, Row, Col))
End Sub

Private Sub fgRoots_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim nVal&, aRoot As clsFutureRootAtom
Dim aMsgBox As New frmMyMsgBox


    On Error Resume Next
        
    With fgRoots
        If Row > 1 Then
            Set aRoot = .RowData(Row)

            If Col = root_symbol Then
                'Set aRoot = .RowData(Row)
                If Not aRoot Is Nothing Then
                    If aRoot.Symbol <> .EditText And IsSymbolExists(.EditText) Then
                        aMsgBox.Execute "The root " & .EditText & " has been assigned to another underlying", vbOKOnly, "Info"
                        Cancel = True
                    End If
                End If
                
            ElseIf Col = ROOT_IMPORTID Then
                'Set aRoot = .RowData(Row)
                If Not aRoot Is Nothing Then
                    If aRoot.ExportSymbol <> .EditText And IsExportSymbolExists(.EditText) Then
                        aMsgBox.Execute "The root " & .EditText & " has been assigned to another underlying", vbOKOnly, "Info"
                        Cancel = True
                    End If
                End If
                
            ElseIf Col = ROOT_FUTCONTRACTSIZE Or Col = ROOT_OPTCONTRACTSIZE Then
                nVal = gCmn.ReadLng(fgRoots.EditText)
                fgRoots.EditText = Format$(nVal, "###0")
            End If
        End If
    End With
End Sub

Private Sub grdOptions_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim nRow As Long
    Dim dCurMonth As Date
    Dim iCurColor1 As OLE_COLOR
    Dim iCurColor2 As OLE_COLOR
    
    With grdOptions
        If (Col <> OPT_HIDDENCOLUMN) Then
            ' painting grid
            iCurColor1 = COLOR11
            iCurColor2 = COLOR21
            For nRow = 2 To .Rows - 1
                .Cell(flexcpBackColor, nRow, OPT_CALLSYMBOL, nRow, OPT_CALLCLOSEPRICE) = iCurColor1
                .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRATION) = iCurColor2
                .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRYVALUATION) = iCurColor2
                .Cell(flexcpBackColor, nRow, OPT_PUTSYMBOL, nRow, OPT_PUTCLOSEPRICE) = iCurColor1
            Next nRow
        End If
    End With
End Sub

Private Sub rbUseBasketIndex_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
    UpdateStateOfDivControls
End Sub

Private Sub rbUseCustFreq_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
    UpdateStateOfDivControls
End Sub

Private Sub rbUseCustStream_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
    UpdateStateOfDivControls
End Sub

Private Sub rbUseIndexYield_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
    UpdateStateOfDivControls
End Sub

Private Sub rbUseMarketDiv_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
    UpdateStateOfDivControls
End Sub

Private Sub txtKurt_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtKurt_GotFocus()
    On Error Resume Next
    gCmn.SelectText txtKurt
End Sub


Private Sub txtSkew_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtSkew_GotFocus()
    On Error Resume Next
    gCmn.SelectText txtSkew
End Sub

' control terminating
Private Sub UserControl_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Set m_rsContractList = Nothing
    Set m_rsOptionList = Nothing
End Sub

' resizing control
Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    With picMain
        .Top = 0
        .Left = picLeft.Width
        .Width = Max(0, ScaleWidth - .Left)
        .Height = Max(0, ScaleHeight - .Top)
    End With
End Sub

'****************************************************
'****************************************************
'****************************************************

' initialization
Public Function Init() As Boolean
    On Error GoTo EH
    Dim i As Long
    Dim rs As ADODB.Recordset

    m_bFireEvent = False
    Init = False
    
    gCmn.CustomizeControls Controls
        
    FormatOptionGrid
    FormatRootGrid
    
    SetState STATE_LOAD
    
    PrevV = GINT_STOCKS
    btnContractFunderlying.Visible = g_Params.Commodity
    
    ' filter init
    For i = 0 To 1
    With cboDivFreq(i)
        .style = fmStyleDropDownList
        .Clear
        .AddItem "<NONE>"
        .List(0, 1) = 0
        .AddItem "Annually"
        .List(1, 1) = 1
        .AddItem "Semiannually"
        .List(2, 1) = 2
        .AddItem "Quarterly"
        .List(3, 1) = 4
        .AddItem "Monthly"
        .List(4, 1) = 12
        
        .TextColumn = 1
        .BoundColumn = 2
        .ListIndex = 0
    End With
    Next
    
    ExchangeListLoad
    
    SetContractType GINT_STOCKS
    
    SetState STATE_NONE
    
    SetContractButtonsState
        
    Init = True
    Exit Function
EH:
    ShowError "Fail to initialize Market Structure control"
    SetContractButtonsState
End Function

Private Sub UpdateStateOfDivControls()
    'MarketDividends
    cboDivFreq(0).Enabled = False 'rbUseMarketDiv.Value
    dtpDivDate(0).Enabled = False 'rbUseMarketDiv.Value
    txtDivAmt(0).Enabled = False 'rbUseMarketDiv.Value
    'CustomPeriodical
    cboDivFreq(1).Enabled = rbUseCustFreq.Value
    dtpDivDate(1).Enabled = rbUseCustFreq.Value
    txtDivAmt(1).Enabled = rbUseCustFreq.Value
    'CustomStream
    btnCustDivs.Enabled = rbUseCustStream.Value
    'BasketDivs
    btnComponents.Enabled = rbUseBasketIndex.Value
    btnComponentsDividends.Enabled = rbUseBasketIndex.Value
    'Yield
    txtYield.Enabled = rbUseIndexYield.Value
End Sub

Private Sub SetDividendType(ByVal enDivType As Long)

    Dim bStkEnable As Boolean
    Dim bIdxEnable As Boolean
    
    'Set All As False
    rbUseMarketDiv.Value = False
    rbUseBasketIndex.Value = False
    rbUseIndexYield.Value = False
    rbUseCustStream.Value = False
    rbUseCustFreq.Value = False
    
    ' Set Actual Div Type Control
    If (enDivType = enDivMarket) Then
        rbUseMarketDiv.Value = True
    ElseIf (enDivType = enDivStockBasket) Then
        rbUseBasketIndex.Value = True
    ElseIf (enDivType = enDivIndexYield) Then
        rbUseIndexYield.Value = True
    ElseIf (enDivType = enDivCustomStream) Then
        rbUseCustStream.Value = True
    ElseIf (enDivType = enDivCustomPeriodical) Then
        rbUseCustFreq.Value = True
    End If
    
    bStkEnable = False: bIdxEnable = False
    If (m_ContractType = GINT_INDEXES) Then
        bIdxEnable = True
    ElseIf (m_ContractType = GINT_STOCKS) Then
        bStkEnable = True
    End If
    
    'Set Enables
    rbUseIndexYield.Enabled = bIdxEnable
    rbUseBasketIndex.Enabled = bIdxEnable
    rbUseMarketDiv.Enabled = bIdxEnable Or bStkEnable
    rbUseCustStream.Enabled = bIdxEnable Or bStkEnable
    rbUseCustFreq.Enabled = bIdxEnable Or bStkEnable
    
    'UpdateControl
    UpdateStateOfDivControls
    
End Sub


'****************************************************
' control properties
'****************************************************
Private Sub SetContractType(ByVal v As eContractType)
    On Error GoTo EH
    Dim i As Long
    
    Screen.MousePointer = vbHourglass
    DoEvents
    m_ContractType = v
    
    If (m_ContractType = GINT_COMMODITIES) Then
        If (PrevV <> GINT_COMMODITIES) Then
            FormatICGrid
            picMain_Resize
        End If
        
        
        opAmerican.Visible = False
        opEuropean.Visible = False
        Shape2.Visible = False
        btnGroups.Visible = True
        btnExchanges.Visible = False
        btnOptionRoots.Visible = False
        btnMarketValues.Visible = False
        btnBeta.Visible = False
        chkDysplayAmountInFCE.Visible = True
        btnUnd.Visible = False
        btnSymbLoadIV.Visible = False
        btnOptionAdd.Visible = False
        btnOptionEdit.Visible = False
        btnOptionDelete.Visible = False
        
        mnuOptionCallPrices.Enabled = False
        mnuOptionEdit.Enabled = False
        mnuOptionPutPrices.Enabled = False
        mnuOptions.Enabled = False
        mnuOptionsAdd.Enabled = False
        mnuOptionsDefSort.Enabled = False
        mnuOptionsDelete.Enabled = False

        LoadFutureRoots
        
     Else
        If (PrevV = GINT_COMMODITIES) Then
            FormatOptionGrid
              picMain_Resize

        End If
        
        mnuOptionCallPrices.Enabled = True
        mnuOptionEdit.Enabled = True
        mnuOptionPutPrices.Enabled = True
        mnuOptions.Enabled = True
        mnuOptionsAdd.Enabled = True
        mnuOptionsDefSort.Enabled = True
        mnuOptionsDelete.Enabled = True
        
        btnOptionEdit.Visible = True
        btnOptionDelete.Visible = True
        opAmerican.Visible = True
        opEuropean.Visible = True
        
        
        Shape2.Visible = True
        Shape2.Appearance = 0
        Shape2.BackColor = &HFFFFFF
        Shape2.ForeColor = &HFFFFFF
        btnGroups.Visible = True
        btnExchanges.Visible = True
        btnOptionRoots.Visible = True
        btnMarketValues.Visible = True
        btnBeta.Visible = True
        chkDysplayAmountInFCE.Visible = False
        btnUnd.Visible = True
        btnSymbLoadIV.Visible = True
        btnOptionAdd.Visible = True
    End If
    
    ' controls customizing
    btnUpdateComposition.Visible = (m_ContractType = GINT_INDEXES)
    btnFuturesRoots.Visible = (m_ContractType = GINT_INDEXES)
    btnSurface.Visible = (m_ContractType = GINT_INDEXES Or m_ContractType = GINT_STOCKS)
    
    
    For i = 0 To 1
        lblDivFreq(i).Visible = (m_ContractType = GINT_STOCKS Or m_ContractType = GINT_INDEXES)
        cboDivFreq(i).Visible = lblDivFreq(i).Visible
        lblDivDate(i).Visible = lblDivFreq(i).Visible
        dtpDivDate(i).Visible = lblDivFreq(i).Visible
        lblDivAmt(i).Visible = lblDivFreq(i).Visible
        txtDivAmt(i).Visible = lblDivFreq(i).Visible
    Next
    
    Dim bVisibleComponents As Boolean
    bVisibleComponents = ((m_ContractType = GINT_STOCKS Or m_ContractType = GINT_INDEXES) And m_ContractType <> GINT_COMMODITIES)
    
    lblPrimExchange.Visible = ((m_ContractType = GINT_STOCKS Or m_ContractType = GINT_INDEXES) And m_ContractType <> GINT_COMMODITIES)
    cboPrimExchange.Visible = lblPrimExchange.Visible
    Shape1.Visible = bVisibleComponents
    rbUseCustFreq.Visible = bVisibleComponents
    rbUseCustStream.Visible = bVisibleComponents
    btnCustDivs.Visible = bVisibleComponents
    
    'visible ctrls for divEdition
    rbUseIndexYield.Visible = bVisibleComponents
    rbUseBasketIndex.Visible = bVisibleComponents
    rbUseMarketDiv.Visible = bVisibleComponents
    rbUseCustStream.Visible = bVisibleComponents
    rbUseCustFreq.Visible = bVisibleComponents
    lblYield.Visible = bVisibleComponents
    txtYield.Visible = bVisibleComponents
    btnComponents.Visible = bVisibleComponents
    btnComponentsDividends.Visible = bVisibleComponents
    lblDivType.Visible = bVisibleComponents
    
    cmdAsset.Visible = (m_ContractType <> GINT_COMMODITIES And m_ContractType <> GINT_FUTURES)
    
    btnFuturesRoots.Visible = (lblYield.Visible Or m_ContractType = GINT_COMMODITIES)
    
    lblSkew.Visible = (m_ContractType <> GINT_FUTURES And m_ContractType <> GINT_COMMODITIES)
    txtSkew.Visible = lblSkew.Visible
    lblKurt.Visible = lblSkew.Visible
    txtKurt.Visible = lblSkew.Visible

    ' loading contracts list
    ContractListLoad
    
    btnStockToIndex.Visible = (m_ContractType <> GINT_FUTURES And m_ContractType <> GINT_COMMODITIES)
    btnStockToIndex.Caption = IIf(m_ContractType = GINT_INDEXES, "Convert To Stock", "Convert To Index")
    
    lblMaturity.Visible = (m_ContractType = GINT_FUTURES)
    dtMaturityPicker.Visible = lblMaturity.Visible
    dtMaturityPicker.Enabled = (m_ContractType = GINT_FUTURES)
    
    lblLotSize.Visible = (m_ContractType = GINT_FUTURES)
    txtLotSize.Visible = lblLotSize.Visible
    txtLotSize.Enabled = False
    
    Select Case m_ContractType
    Case GINT_STOCKS
        btnContractStock.Caption = ">Stock"
        btnContractIndex.Caption = "  Index"
        btnContractFutures.Caption = "  Futures"
        btnContractFunderlying.Caption = "  Future's Underlying"
    Case GINT_INDEXES
        btnContractStock.Caption = "  Stock"
        btnContractIndex.Caption = ">Index"
        btnContractFutures.Caption = "  Futures"
        btnContractFunderlying.Caption = "  Future's Underlying"
    Case GINT_FUTURES
        btnContractStock.Caption = "  Stock"
        btnContractIndex.Caption = "  Index"
        btnContractFutures.Caption = ">Futures"
        btnContractFunderlying.Caption = "  Future's Underlying"
    Case GINT_COMMODITIES
        btnContractStock.Caption = "  Stock"
        btnContractIndex.Caption = "  Index"
        btnContractFutures.Caption = "  Futures"
        btnContractFunderlying.Caption = ">Future's Underlying"
    End Select
    
    
    ContractLoad
    PrevV = m_ContractType
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

' setting control state
Private Sub SetState(ByVal X As eState)
    On Error GoTo EH
    If X = STATE_CONTRACT_EDIT And m_Contract.ID = 0 Then X = STATE_CONTRACT_ADD
    
    m_State = X
    Screen.MousePointer = IIf(m_State = STATE_LOAD, vbHourglass, vbDefault)
    DoEvents
    Exit Sub
EH:
    ShowError
End Sub

'****************************************************
' Working with contracts
'****************************************************

' importing contract data from internet
Public Function ContractImport() As Boolean
    On Error GoTo EH
    If m_Contract.ID <> 0 Then frmMktStrImport.Execute m_Contract.Symbol, m_Contract.ID, m_Contract.TypeID, m_Contract.ExchangeID
    ContractImport = True
    ContractLoad
    Exit Function
EH:
    ShowError
End Function

' adding a new contract
Public Function ContractAdd() As Boolean
    On Error GoTo EH
    ' saving old contract
    m_OldContract = m_Contract
    ' setting new contract params
    m_Contract.ID = 0
    ' loading contract
    ContractLoad
    ' setting Add state
    SetState STATE_CONTRACT_ADD
    SetContractButtonsState
    ContractAdd = True
    Exit Function
EH:
    ShowError
End Function

' deleting contract
Public Function ContractDelete() As Boolean
    On Error GoTo EH
    Dim iNewID As Long, nRet&, sContractType$

    If m_State <> STATE_NONE Then Exit Function
    
    ContractDelete = False
    iNewID = 0
    
    If m_Contract.ID = 0 Then
        ' setting old contract ID
        iNewID = m_OldContract.ID
    Else
        ' deleting contract from database
        If gCmn.MyMsgBox("Do you wish to delete the current contract?", vbYesNo) <> vbYes Then Exit Function
        
        Select Case m_Contract.TypeID
            Case GINT_STOCKS
                nRet = gDBW.usp_BoStock_Del(m_Contract.ID)
                sContractType = "Stock"
                
            Case GINT_INDEXES
                nRet = gDBW.usp_BoIndex_Del(m_Contract.ID)
                sContractType = "Index"
        
            Case GINT_FUTURES
                nRet = gDBW.usp_BoFuture_Del(m_Contract.ID, 0)
                sContractType = "Futures"
            Case GINT_COMMODITIES
                nRet = gDBW.usp_BoCommodity_Del(m_Contract.ID, 0)
                sContractType = "Futures Underlying"
            
            'Case GINT_INTEREST
            '    nRet = gDBW.usp_BoCommodity_Del(m_Contract.ID, 0)
        
        End Select
            
        If nRet < 0 Then
            Select Case m_Contract.TypeID
                Case GINT_STOCKS, GINT_INDEXES
                    Select Case nRet
                        Case -1
                            ShowWarning "Unable to delete the " & sContractType & ". There is an open position on it"
                        Case -2
                            ShowWarning "Unable to delete the " & sContractType & ". There is an order on it"
                        Case -3
                            ShowWarning "Unable to delete the " & sContractType & ". There are an open positions on its options"
                        Case -4
                            ShowWarning "Unable to delete the " & sContractType & ". There are an orders on its options"
                        Case -5
                            ShowWarning "Unable to delete the " & sContractType & ". Fail to remove Futures"
                        Case Else
                            ShowWarning "Unable to delete the " & sContractType & ". Error (" & CStr(nRet) & ")"
                    End Select
                Case GINT_FUTURES
                    Select Case nRet
                        Case -2
                            ShowWarning "Unable to delete the futures. There is an open position on it or on its options"
                        Case -3
                            ShowWarning "Unable to delete the futures. There is an order on it or on its options"
                        Case Else
                            ShowWarning "Unable to delete the futures."
                    End Select
                Case GINT_COMMODITIES
                    Select Case nRet
                        Case -1
                            ShowWarning "Unable to delete the Future Underlying. There is an open position on it"
                        Case -2
                            ShowWarning "Unable to delete the Future Underlying. There is an order on it"
                        Case -5
                            ShowWarning "Unable to delete the Future Underlying. Unable to Modify Index Beta"
                        Case -6
                            ShowWarning "Unable to delete the Future Underlying. Unable to remove it from a group"
                        Case -7
                            ShowWarning "Unable to delete the Future Underlying. Unable to remove Trader Contract"
                        Case -8
                            ShowWarning "Unable to delete the Future Underlying. Unable to remove Commodity"
                        Case -21
                            ShowWarning "Unable to delete the Future Underlying. Unable to remove TradeLog"
                        Case -101
                            ShowWarning "Unable to delete the Future Underlying. Underlying for future roots to delete is not specified!"
                        Case -102
                            ShowWarning "Unable to delete the Future Underlying. Can't remove future roots, because its futures or its future options has trades or position!"
                        Case -103
                            ShowWarning "Unable to delete the Future Underlying. Can't remove future roots, because its futures or its future options has orders!"
                        Case -104
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Future Options!"
                        Case -105
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Futures!"
                        Case -106
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Future Root!"
                        Case -107
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Contract Price!"
                        Case -108
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Contract Exchange Link!"
                        Case -109
                            ShowWarning "Unable to delete the Future Underlying. Can't remove Contract!"
                            
                        Case Else
                            ShowWarning "Unable to delete the futures.: " & CStr(nRet)
                    End Select
            End Select
            
            
            If m_Contract.TypeID <> GINT_FUTURES Then
            Else
            End If
            Exit Function
        End If
        
        On Error Resume Next
        With cboContracts
            If .ListIndex > 0 Then
                iNewID = gCmn.ReadLng(.List(.ListIndex - 1, 1))
                
            ElseIf .ListCount > 0 Then
                iNewID = gCmn.ReadLng(.List(.ListCount - 1, 1))
            End If
        End With
    End If
    
    SetState STATE_NONE
    
    ' loading contracts list
    ContractListLoad
    
    ' setting new contract id
    m_Contract.ID = iNewID
    ContractLoad
    
    ContractDelete = True
    Exit Function
EH:
    ShowError
End Function

' checking if we can procede working with contracts
Public Function ContractCanProceed() As Boolean
    On Error GoTo EH
    
    Dim aRoot As clsFutureRootAtom
    
    If m_bDTPIsDroppedDown Then Exit Function
    If dtPicker2.Visible Then Exit Function
    
    If m_State = STATE_CONTRACT_ADD Or m_State = STATE_CONTRACT_EDIT Then
        Select Case gCmn.MyMsgBox("Do you wish to save changes?", vbYesNoCancel)
            Case vbYes
                ' save contract
                ContractCanProceed = ContractSave
            Case vbNo
                ' cancel changes
                ContractCanProceed = ContractCancel
            Case Else
                ContractCanProceed = False
        End Select
    Else
        ContractCanProceed = True
    End If
    Exit Function
EH:
    ShowError
End Function

' Save contract data
Private Function ContractSave() As Boolean
    On Error GoTo EH
    Dim vId As Variant
    Dim sSymbol As String
    Dim sName As String
    Dim vExchangeID As Variant
    Dim iDivFreq As Long
    Dim dDivDate As Date
    Dim fDivAmt As Double
    Dim iDivFreq2 As Long
    Dim dDivDate2 As Date
    Dim fDivAmt2 As Double
    Dim fYield As Double
    Dim fSkew As Double
    Dim fKurt As Double
    Dim m_State1 As eState
    Dim enDivType As EtsGeneralLib.EtsDivTypeEnum

    On Error Resume Next

    m_State1 = m_State
    
    If Not ((m_State = STATE_CONTRACT_ADD) Or (m_State = STATE_CONTRACT_EDIT)) Then
        m_State = m_State1
        Exit Function
    End If
    
    'sSymbol = UCase$(Trim$(txtSymbol.Text))
    sSymbol = Trim$(txtSymbol.Text)
    
    ' checking valid contract simbol
    If sSymbol = "" Then
        ShowWarning "Symbol is not specified"
        m_State = m_State1
        Exit Function
    End If
    
    ' checking if symbol already exists
    If UCase$(sSymbol) <> UCase$(m_Contract.Symbol) Then
        With m_rsContractList
            .Filter = "vcSymbol='" & FixDbString(sSymbol) & "'"
            If .RecordCount > 0 Then
                Debug.Print !vcSymbol
                ShowWarning "Symbol '" & sSymbol & "' already exists in database"
                m_State = m_State1
                Exit Function
            End If
        End With
    End If
        
    ' reading data from a form
    vId = DBWrite(m_Contract.ID, 0)
    sName = Trim(txtName.Text)
    
    Dim IsManual As Boolean
    Dim IsBasket As Boolean
    Dim IsAmerican As Boolean
    Dim IsActive As Byte
    Dim CalcGreeksType As Byte
    Dim lDivType As Long
    lDivType = 0
    
    IsActive = IIf(chkIsActive.Value <> 0, 1, 0)
    IsAmerican = opAmerican.Value
    CalcGreeksType = IIf(chkDysplayAmountInFCE.Value <> 0, 1, 0)
    
    If m_ContractType = GINT_STOCKS Then
        vExchangeID = DBWrite(cboPrimExchange.Value, 0)
        
        If (rbUseMarketDiv.Value = True) Then
            lDivType = 0
            IsManual = False
        ElseIf (rbUseCustFreq.Value = True) Then
            lDivType = 1
            IsManual = True
        ElseIf (rbUseCustStream.Value = True) Then
            lDivType = 2
            IsManual = True
        End If
        
        iDivFreq = 0
        fDivAmt = 0
        dDivDate = 0
        iDivFreq2 = 0
        fDivAmt2 = 0
        dDivDate2 = 0
        
        If IsManual Then
            iDivFreq2 = DBRead(cboDivFreq(1).Value, 0)
            If iDivFreq2 <> 0 Then
                If Not IsNumeric(txtDivAmt(1).Text) Then
                    ShowWarning "Invalid Div. Amount value"
                    m_State = m_State1
                    Exit Function
                End If
                fDivAmt2 = gCmn.ReadDbl(txtDivAmt(1).Text)
                If fDivAmt2 < 0 Then
                    ShowWarning "Negative Div. Amount value"
                    m_State = m_State1
                    Exit Function
                End If
                dDivDate2 = dtpDivDate(1).Value
            End If
        End If
    End If
    
    If m_ContractType = GINT_INDEXES Then
        vExchangeID = DBWrite(cboPrimExchange.Value, 0)
        
        If Not IsNumeric(txtYield.Text) Then
            ShowWarning "Invalid Yield value"
            m_State = m_State1
            Exit Function
        End If
        
        If (rbUseMarketDiv.Value = True) Then
            lDivType = 0
            IsManual = False
        ElseIf (rbUseCustFreq.Value = True) Then
            lDivType = 1
            IsManual = True
        ElseIf (rbUseCustStream.Value = True) Then
            lDivType = 2
            IsManual = True
        ElseIf (rbUseIndexYield.Value = True) Then
            lDivType = 4
            IsManual = False
        ElseIf (rbUseBasketIndex.Value = True) Then
            lDivType = 3
            IsManual = False
        End If
                
        'Read Market Divs
        iDivFreq = cboDivFreq(0).Value
        If (IsNumeric(txtDivAmt(0).Text)) Then
            fDivAmt = gCmn.ReadDbl(txtDivAmt(0).Text)
        End If
        dDivDate = dtpDivDate(0).Value
        
        'Read Custom Stream
        iDivFreq2 = cboDivFreq(1).Value
        If (IsNumeric(txtDivAmt(1).Text)) Then
            fDivAmt2 = gCmn.ReadDbl(txtDivAmt(1).Text)
        End If
        dDivDate2 = dtpDivDate(1).Value
                
        'Read Yield
        fYield = gCmn.ReadDbl(txtYield.Text) * 0.01
        If fYield < 0 Then
            ShowWarning "Negative Yield value"
            m_State = m_State1
            Exit Function
        End If
        
    End If
    
    If m_ContractType <> GINT_FUTURES Then
        If Not IsNumeric(txtSkew.Text) Then
            ShowWarning "Invalid Skew value"
            m_State = m_State1
            Exit Function
        End If
        fSkew = gCmn.ReadDbl(txtSkew.Text)
        
        If Not IsNumeric(txtKurt.Text) Then
            ShowWarning "Invalid Kurtosis value"
            m_State = m_State1
            Exit Function
        End If
        fKurt = gCmn.ReadDbl(txtKurt.Text)
    End If
    
    Dim lType As Long
    
    If m_ContractType = GINT_COMMODITIES Then
        lType = cboType.Value
        
        If lType < 1 Then
            ShowWarning "Type not assigned"
            m_State = m_State1
            Exit Function
        End If
    End If
    
    ' saving contract
    Select Case m_ContractType
        Case GINT_STOCKS
            If IsManual Then
                vId = gDBW.usp_Stock_Save( _
                    vId _
                    , vExchangeID _
                    , sSymbol _
                    , sName _
                    , _
                    , Null _
                    , Null _
                    , Null _
                    , iDivFreq2 _
                    , dDivDate2 _
                    , fDivAmt2 _
                    , lDivType _
                    , IIf(IsAmerican, 1, 0) _
                    , Null _
                    , 0 _
                    , IsActive _
                    , fSkew _
                    , fKurt)
            Else
                vId = gDBW.usp_Stock_Save( _
                    vId _
                    , vExchangeID _
                    , sSymbol _
                    , sName _
                    , _
                    , _
                    , _
                    , _
                    , _
                    , _
                    , _
                    , lDivType _
                    , IIf(IsAmerican, 1, 0) _
                    , Null _
                    , 0 _
                    , IsActive _
                    , fSkew _
                    , fKurt)
            End If
            
        Case GINT_INDEXES
            vId = gDBW.usp_Index_Save( _
                vId _
                , sSymbol _
                , sName _
                , fYield _
                , Null _
                , IIf(IsAmerican, 1, 0) _
                , Null _
                , 0 _
                , IIf(IsBasket, 1, 0) _
                , IsActive _
                , DBWrite(fSkew, 0) _
                , DBWrite(fKurt, 0) _
                , 1 _
                , vExchangeID _
                , lDivType _
                , IIf(fDivAmt >= 0#, iDivFreq, Null), IIf(fDivAmt >= 0#, dDivDate, Null), IIf(fDivAmt >= 0#, fDivAmt, Null) _
                , IIf(fDivAmt2 >= 0#, iDivFreq2, Null), IIf(fDivAmt2 >= 0#, dDivDate2, Null), IIf(fDivAmt2 >= 0#, fDivAmt2, Null))
    
        Case GINT_COMMODITIES
            vId = gDBW.usp_IC_Save( _
                vId _
                , sSymbol _
                , sName _
                , CLng(GINT_COMMODITIES) _
                , CalcGreeksType _
                , IsActive _
                , lType _
                )
        
'        Case GINT_INTEREST
'            vId = gDBW.usp_IC_Save( _
'                vId _
'                , sSymbol _
'                , sName _
'                , CLng(GINT_INTEREST) _
'                , CalcGreeksType _
'                , IsActive _
'                )
    
        
        Case GINT_FUTURES
            Debug.Assert vId <> 0
            gDBW.usp_BoFuture_Save vId, , sSymbol, sName, , dtFutMaturity, , IIf(IsAmerican, 1, 0), IIf(IsActive, 1, 0)
    End Select
    
    If m_State = STATE_CONTRACT_ADD Then
        If vId = -1 Then GoTo EH
    Else
        If vId <> m_OldContract.ID Then GoTo EH
        vId = m_OldContract.ID
    End If
    
    SetState STATE_NONE
    
    ' loading contracts list
    ContractListLoad
    
    ' loading saved contract from database
    m_Contract.ID = vId
    ContractLoad
    
    ContractSave = True
    Exit Function
EH:
    ShowError
    m_State = m_State1
End Function

' canceling any changes
Private Function ContractCancel() As Boolean
    On Error GoTo EH
    If m_State <> STATE_CONTRACT_ADD And m_State <> STATE_CONTRACT_EDIT Then Exit Function
    
    m_Contract = m_OldContract
    ContractCancel = ContractLoad
    Exit Function
EH:
    ShowError
End Function
Private Sub ICContractListLoad()
    On Error GoTo EH
    Dim arr() As Variant
    Dim arrContracts() As String
    Dim arrIndices() As String
    Dim i&, nRealContractsCount&, nRealIndicesCount&, enContractType As eContractType

    SetState STATE_LOAD
                                                                            
    '-------------------------------------
    ' loading contract list from database
    If Not m_rsContractList Is Nothing Then
        If m_rsContractList.State = adStateOpen Then m_rsContractList.Close
    End If
    Set m_rsContractList = gDBW.usp_BoContractsSymbol_Get(m_ContractType)
    
    '-------------------------------------
    
'    ' filling contracts & indices lists
'    With m_rsContractList
''        If cboFutUndType.Value > 0 Then
''            .Filter = "iFutureUnderlyingTypeID = " & cboFutUndType.Value
''        End If
'
'        m_sContracts = "#" & Format$(GINT_NONEID) & ";" & "<NONE>"
'        m_sIndexes = "#" & Format$(GINT_NONEID) & ";" & "<NONE>"
'
'        If .RecordCount > 0 Then
'            ReDim arrContracts(.RecordCount - 1)
'            ReDim arrIndices(.RecordCount - 1)
'        End If
'
'        nRealContractsCount = 0&
'        nRealIndicesCount = 0&
'        Do While Not .EOF
'            enContractType = gCmn.ReadLng(!iContractTypeID)
'            If enContractType = m_ContractType Then
'                arrContracts(nRealContractsCount) = "#" & Format$(gCmn.ReadLng(!iContractID)) & ";" & gCmn.ReadStr(.Fields("vcSymbol"))
'                nRealContractsCount = nRealContractsCount + 1
'
''                If enContractType = GINT_INDEXES Then
''                    arrIndices(nRealIndicesCount) = "#" & Format$(gCmn.ReadLng(!iContractID)) & ";" & gCmn.ReadStr(.Fields("vcSymbol"))
''                    nRealIndicesCount = nRealIndicesCount + 1
''                End If
'            End If
'            .MoveNext
'        Loop
'    End With
'
'    If nRealContractsCount > 0 Then
'        ReDim Preserve arrContracts(nRealContractsCount)
'        m_sContracts = Join$(arrContracts, "|")
'    End If
'
''    If nRealIndicesCount > 0 Then
''        ReDim Preserve arrIndices(nRealIndicesCount)
''        m_sIndexes = Join$(arrIndices, "|")
''    End If
'
'    Erase arrContracts
'    Erase arrIndices
'
    '-------------------------------------
    ' filling cboContracts
    With m_rsContractList
        If (cboFutUndType.Value = 0 Or _
            cboFutUndType.Value = 1 Or _
            cboFutUndType.Value = 2 Or _
            cboFutUndType.Value = 3 Or _
            cboFutUndType.Value = 4) Then
            .Filter = "iFutureUnderlyingTypeID = " & cboFutUndType.Value
        End If

        If Not .BOF Then .MoveFirst
        
        If .RecordCount > 0 Then
            If Not m_rsContractList.BOF Then m_rsContractList.MoveFirst
            m_Contract.ID = m_rsContractList!iContractID
            arr = .GetRows(, adBookmarkFirst, Array("vcSymbol", "iContractID", "iContractTypeID"))
        
        Else
            m_Contract.ID = 0
            
            ReDim arr(2, 0)
            arr(0, 0) = "<NONE>"
            arr(1, 0) = GINT_NONEID
            arr(2, 0) = GINT_NONEID
        End If
    End With
    
    With cboContracts
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
        If .ListCount > 0 Then .Text = m_Contract.Symbol
    End With
        
    SetState STATE_NONE
    Exit Sub
EH:
    Erase arrContracts
    Erase arrIndices
    ShowError
End Sub

Sub ICTypeListLoad()
    Dim arr() As Variant
    On Error GoTo EH
    
    SetState STATE_LOAD
                                                                            
    Dim m_rsTypeList As ADODB.Recordset
    
    Set m_rsTypeList = gDBW.usp_FutureUnderlyingType_Get(Null)

    With m_rsTypeList
        If Not .BOF Then .MoveFirst
        
        If .RecordCount > 0 Then
            arr = .GetRows(, adBookmarkFirst, Array("vcName", "iFutureUnderlyingTypeID"))
        
        Else
            m_Contract.ID = 0
            
            ReDim arr(2, 0)
            arr(0, 0) = "<NONE>"
            arr(1, 0) = GINT_NONEID
            arr(2, 0) = GINT_NONEID
        End If
    End With
    
    With cboFutUndType
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "All", 0
        .ListIndex = 0
    End With

    With cboType
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
    End With
    Exit Sub
EH:
    ShowError
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here load Future Roots Properties from DB
Private Function LoadFutureRoots() As Boolean
    Dim rs As ADODB.Recordset
    Dim nRow As Long
    Dim sKey As String
'    Dim m_Roots As clsFutureRootColl 'New
    Dim aRoot As clsFutureRootAtom
        
    Set rs = gDBW.usp_BoFutureRoot_Get(m_Contract.ID)
    
    m_Roots.Clear
    
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sKey = gCmn.ReadStr(.Fields("vcFutureRootSymbol"))
                Set aRoot = m_Roots.Add(sKey)
                aRoot.State = ELEM_UNCHANGED
                aRoot.Symbol = sKey
                aRoot.ExportSymbol = gCmn.ReadStr(.Fields("vcExportSymbol"))
                aRoot.RootID = gCmn.ReadLng(.Fields("iFutureRootID"))
                aRoot.Name = gCmn.ReadStr(.Fields("vcFutureRootName"))
                aRoot.FutureLotSize = gCmn.ReadLng(.Fields("iFutureLotSize"))
                aRoot.OptionLotSize = gCmn.ReadLng(.Fields("iOptionLotSize"))
                aRoot.tiQuoteFormat = gCmn.ReadLng(.Fields("tiQuoteFormat"))
                aRoot.iQuotationUnitAmmount = gCmn.ReadLng(.Fields("iQuotationUnitAmmount"))
                aRoot.iQuotationUnitID = gCmn.ReadLng(.Fields("iQuotationUnitID"))
                aRoot.fQuoteDenomination = gCmn.ReadDbl(.Fields("fQuoteDenomination"))
                aRoot.fTickSize = gCmn.ReadDbl(.Fields("fTickSize"))
                aRoot.fTickValue = gCmn.ReadDbl(.Fields("fTickValue"))
                If .Fields("iQuotationUnitID") = 40 Then
                     aRoot.vcQuotationUnitName = gCmn.ReadStr(.Fields("vcQuotationUnitName"))
                End If
'                If .Fields("vcQuotationUnitName") Is Not Null Then
'                    aRoot.vcQuotationUnitName = gCmn.ReadStr(.Fields("vcQuotationUnitName"))
'                End If
                .MoveNext
            Loop
        End If
    End With
    
    With fgRoots
        .Redraw = flexRDNone
    
        .Rows = 2
        
        nRow = 2
        For Each aRoot In m_Roots
                            
            .AddItem ""
                        
            .RowData(nRow) = aRoot
                    
            .TextMatrix(nRow, root_symbol) = aRoot.Symbol
            .TextMatrix(nRow, ROOT_NAME) = aRoot.Name
            .TextMatrix(nRow, ROOT_IMPORTID) = aRoot.ExportSymbol
            .TextMatrix(nRow, ROOT_FUTCONTRACTSIZE) = aRoot.FutureLotSize
            .Cell(flexcpAlignment, nRow, ROOT_FUTCONTRACTSIZE) = flexAlignRightCenter
            .TextMatrix(nRow, ROOT_OPTCONTRACTSIZE) = aRoot.OptionLotSize
            .Cell(flexcpAlignment, nRow, ROOT_OPTCONTRACTSIZE) = flexAlignRightCenter
            
            .TextMatrix(nRow, ROOT_QUOTEFORMAT) = aRoot.tiQuoteFormat
            .TextMatrix(nRow, ROOT_QUOTATIONUNITAMMOUNT) = aRoot.iQuotationUnitAmmount
            If aRoot.iQuotationUnitID = 40 Then
                .TextMatrix(nRow, ROOT_QUOTATIONUNITUNIT) = aRoot.vcQuotationUnitName
            Else
                .TextMatrix(nRow, ROOT_QUOTATIONUNITUNIT) = aRoot.iQuotationUnitID
            End If
            .TextMatrix(nRow, ROOT_QUOTEDENOMINATION) = aRoot.fQuoteDenomination
            .TextMatrix(nRow, ROOT_TICKSIZE) = aRoot.fTickSize
            .TextMatrix(nRow, ROOT_TICKVALUE) = aRoot.fTickValue
            
            nRow = nRow + 1
        Next
    
        If .Rows > 2 Then
'            .Row = 2
'            .Col = 0
            DeleteButton.Enabled = True
            DeleteButton.Enabled = True
        Else
            DeleteButton.Enabled = False
            DeleteButton.Enabled = False
        End If
        .Redraw = flexRDBuffered
    End With
    
    LoadFutureRoots = True
End Function

' contract list loading
Private Sub ContractListLoad()
    On Error GoTo EH
    
    If m_ContractType = GINT_COMMODITIES Then
        ICContractListLoad
        Exit Sub
    End If
    
    Dim arr() As Variant
    Dim arrContracts() As String
    Dim arrIndices() As String
    Dim i&, nRealContractsCount&, nRealIndicesCount&, enContractType As eContractType

    SetState STATE_LOAD
                                                                            
    '-------------------------------------
    ' loading contract list from database
    If Not m_rsContractList Is Nothing Then
        If m_rsContractList.State = adStateOpen Then m_rsContractList.Close
    End If
    Set m_rsContractList = gDBW.usp_BoContractsSymbol_Get(Null)
    
    '-------------------------------------
    
    ' filling contracts & indices lists
    With m_rsContractList
        m_sContracts = "#" & Format$(GINT_NONEID) & ";" & "<NONE>"
        m_sIndexes = "#" & Format$(GINT_NONEID) & ";" & "<NONE>"
        
        If .RecordCount > 0 Then
            ReDim arrContracts(.RecordCount - 1)
            ReDim arrIndices(.RecordCount - 1)
        End If
        
        nRealContractsCount = 0&
        nRealIndicesCount = 0&
        Do While Not .EOF
            enContractType = gCmn.ReadLng(!iContractTypeID)
            If enContractType = GINT_INDEXES Or enContractType = GINT_STOCKS Then
                arrContracts(nRealContractsCount) = "#" & Format$(gCmn.ReadLng(!iContractID)) & ";" & gCmn.ReadStr(.Fields("vcSymbol"))
                nRealContractsCount = nRealContractsCount + 1
            'End If
            'If enContractType = GINT_INDEXES Or enContractType = GINT_FUTURES Then
                arrIndices(nRealIndicesCount) = "#" & Format$(gCmn.ReadLng(!iContractID)) & ";" & gCmn.ReadStr(.Fields("vcSymbol"))
                nRealIndicesCount = nRealIndicesCount + 1
            End If
        
            .MoveNext
        Loop
    End With
    
    If nRealContractsCount > 0 Then
        ReDim Preserve arrContracts(nRealContractsCount)
        m_sContracts = Join$(arrContracts, "|")
    End If
    
    If nRealIndicesCount > 0 Then
        ReDim Preserve arrIndices(nRealIndicesCount)
        m_sIndexes = Join$(arrIndices, "|")
    End If
    
    Erase arrContracts
    Erase arrIndices
    
    '-------------------------------------
    ' filling cboContracts
    With m_rsContractList
        .Filter = "iContractTypeID = " & m_ContractType
        If Not .BOF Then .MoveFirst
        
        If .RecordCount > 0 Then
            If Not m_rsContractList.BOF Then m_rsContractList.MoveFirst
            m_Contract.ID = m_rsContractList!iContractID
            arr = .GetRows(, adBookmarkFirst, Array("vcSymbol", "iContractID", "iContractTypeID"))
        Else
            m_Contract.ID = 0
            
            ReDim arr(2, 0)
            arr(0, 0) = "<NONE>"
            arr(1, 0) = GINT_NONEID
            arr(2, 0) = GINT_NONEID
        End If
    End With
    
    With cboContracts
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
        If .ListCount > 0 Then .Text = m_Contract.Symbol
    End With
        
    SetState STATE_NONE
    Exit Sub
EH:
    Erase arrContracts
    Erase arrIndices
    ShowError
End Sub

Private Sub ExchangeListLoad()
    On Error GoTo EH
    Dim rst As ADODB.Recordset
    Dim arr() As Variant
    Dim i As Long
    
    ' loading Exchange list
    Set rst = gDBW.usp_Exchange_Get(Null)
    With rst
        ReDim arr(2, 0)
        arr(0, 0) = "<NBBO>"
        arr(1, 0) = GINT_NONEID
        arr(2, 0) = GINT_NONEID
        Do While Not .EOF
            i = i + 1
            ReDim Preserve arr(2, i)
            arr(0, i) = .Fields("vcExchangeName").Value
            arr(1, i) = .Fields("iExchangeID").Value
            arr(2, i) = .Fields("vcDescription").Value
            .MoveNext
        Loop
        .Close
    End With
    Set rst = Nothing
    
    '-------------------------------------
    ' filling exchanges list
    With cboPrimExchange
        .Column = arr
        .TextColumn = 1
        .BoundColumn = 2
    End With
    
    Exit Sub
EH:
    ShowError
End Sub

' Clear contract
Private Sub ContractClear()
    On Error Resume Next
    With m_Contract
        .ExchangeID = 0
        .ID = 0
        .Name = ""
        .Symbol = ""
        .TypeID = m_ContractType
        
        .DivAmt = 0
        .DivDate = Date
        .DivFreq = 0
        
        .DivAmt2 = 0
        .DivDate2 = Date
        .DivFreq2 = 0
        '.DivUse2 = False
        
        .Yield = 0
        .Skew = 0
        .Kurt = 0
        
        If .TypeID = GINT_INDEXES Then
            .OptionsStyle = OPTIONS_DEFAULT_INDEX
            .CalendarType = CALENDAR_DEFAULT_INDEX
        Else
            .OptionsStyle = OPTIONS_DEFAULT_STOCK
            .CalendarType = CALENDAR_DEFAULT_STOCK
        End If
        .IsBasket = False
        .IsActive = True
        .GreeksCalcType = True
    End With

    fgRoots.Redraw = flexRDNone
    fgRoots.Rows = 2
    m_Roots.Clear
    fgRoots.Redraw = flexRDBuffered
End Sub

' loading contract
Private Function ContractLoad() As Boolean
    On Error GoTo EH
    Dim rst As ADODB.Recordset
    Dim IsManual As Boolean
    Dim CurDivType As EtsGeneralLib.EtsDivTypeEnum
    
    SetState STATE_LOAD
    
    If m_Contract.ID = 0 Then
        ContractClear
    Else
        Select Case m_ContractType
        Case GINT_INDEXES
            Set rst = gDBW.usp_Index_Get(, m_Contract.ID)
        Case GINT_STOCKS
            Set rst = gDBW.usp_Stock_Get(, m_Contract.ID)
        Case GINT_FUTURES
            Set rst = gDBW.usp_BoFuture_Get(m_Contract.ID)
'        Case GINT_INTEREST
'            Set rst = gDBW.usp_BoInterest_Get(m_Contract.ID)
        Case GINT_COMMODITIES
            Set rst = gDBW.usp_BoCommodity_Get(m_Contract.ID)
        End Select
    
        If rst.RecordCount = 0 Then
            ContractClear
        Else
            With m_Contract
                .ID = gCmn.ReadLng(rst!iContractID)
                .TypeID = gCmn.ReadLng(rst!iContractTypeID)
                .Symbol = gCmn.ReadStr(rst!vcSymbol)
                .Name = gCmn.ReadStr(rst!vcContractName)
                .DivFreq = 0
                .DivAmt = 0
                .DivDate = 0
                '.DivUse2 = False
                .DivType = enDivMarket
                .DivFreq2 = 0
                .DivAmt2 = 0
                .DivDate2 = 0
                .Yield = 0
                .IsBasket = False
                .IsActive = gCmn.ReadByte(rst!tiIsActive)
                
                Select Case m_ContractType
                    Case GINT_STOCKS
                        .Skew = gCmn.ReadDbl(rst!fSkew)
                        .Kurt = gCmn.ReadDbl(rst!fKurt)
                        .ExchangeID = gCmn.ReadLng(rst!iPrimaryExchangeID)
                        .DivFreq = gCmn.ReadDbl(rst!iDivFreq)
                        If .DivFreq > 0 Then
                            .DivAmt = gCmn.ReadDbl(rst!fDivAmt)
                            .DivDate = gCmn.ReadDate(rst!dtDivDate)
                        End If
                        .DivType = DBRead(rst!tiDivCustom)
                                               
                        .DivFreq2 = gCmn.ReadDbl(rst!iDivFreqCustom)
                        If .DivFreq2 > 0 Then
                            .DivAmt2 = gCmn.ReadDbl(rst!fDivAmtCustom)
                            .DivDate2 = gCmn.ReadDate(rst!dtDivDateCustom)
                        End If
                        .OptionsStyle = DBRead(rst!tiCalcOptionType, OPTIONS_AMERICAN)
                        .CalendarType = DBRead(rst!iExpCalendarID, CALENDAR_THIRD_FRIDAY)
                        
                    Case GINT_INDEXES
                        .Skew = gCmn.ReadDbl(rst!fSkew)
                        .Kurt = gCmn.ReadDbl(rst!fKurt)
                        .Yield = gCmn.ReadDbl(rst!fYield)
                        .OptionsStyle = DBRead(rst!tiCalcOptionType, OPTIONS_EUROPIAN)
                        .CalendarType = DBRead(rst!iExpCalendarID, CALENDAR_THIRD_SATURDAY)
                        .IsBasket = DBRead(rst!tiIsBasket, False) <> 0
                        .ExchangeID = gCmn.ReadLng(rst!iPrimaryExchangeID)
                        
                        .DivFreq = gCmn.ReadDbl(rst!iDivFreq)
                        If .DivFreq > 0 Then
                            .DivAmt = gCmn.ReadDbl(rst!fDivAmt)
                            .DivDate = gCmn.ReadDate(rst!dtDivDate)
                        End If
                        .DivType = DBRead(rst!tiDivCustom)
                        
                        .DivFreq2 = gCmn.ReadDbl(rst!iDivFreqCustom)
                        If .DivFreq2 > 0 Then
                            .DivAmt2 = gCmn.ReadDbl(rst!fDivAmtCustom)
                            .DivDate2 = gCmn.ReadDate(rst!dtDivDateCustom)
                        End If
                        
                    Case GINT_FUTURES
                        .OptionsStyle = DBRead(rst!tiCalcOptionType, OPTIONS_AMERICAN)
                        .CalendarType = DBRead(rst!iExpCalendarID, CALENDAR_THIRD_SATURDAY)
                        .MaturityDate = DBRead(rst!dtMaturityDate, 0)
                        .LotSize = DBRead(rst!iFutureLotSize, 0)
                   
'                   Case GINT_INTEREST
'                        .GreeksCalcType = DBRead(rst!GreeksCalcType, 0)
                   Case GINT_COMMODITIES
                        .GreeksCalcType = DBRead(rst!GreeksCalcType, 0)
                        LoadFutureRoots
                        txtDefCal.Caption = gCmn.ReadStr(rst!expCalendarName)
                        SetCBOValue cboType, DBRead(rst!iFutureUnderlyingTypeID, 0)
                    End Select
            End With
        End If
    End If
    
    ' displaying contract data
    With m_Contract
        txtSymbol.Text = .Symbol
        txtName.Text = .Name
        
        txtSkew.Text = gCmn.FmtDbl(.Skew)
        txtKurt.Text = gCmn.FmtDbl(.Kurt)
        
        opAmerican.Value = (.OptionsStyle = OPTIONS_AMERICAN)
        opEuropean.Value = (.OptionsStyle <> OPTIONS_AMERICAN)
        
        Select Case m_ContractType
            Case GINT_STOCKS
                SetCBOValue cboPrimExchange, .ExchangeID
        
                IsManual = .DivUse2
                CurDivType = .DivType
                SetDividendType CurDivType
                                
                chkIsActive.Value = IIf(.IsActive <> 0, 1, 0)
                
                SetCBOValue cboDivFreq(0), .DivFreq
                dtpDivDate(0).Value = IIf(.DivDate = 0, Date, .DivDate)
                txtDivAmt(0).Text = gCmn.FmtDbl(.DivAmt)
                
                SetCBOValue cboDivFreq(1), .DivFreq2
                dtpDivDate(1).Value = IIf(.DivDate2 = 0, Date, .DivDate2)
                txtDivAmt(1).Text = gCmn.FmtDbl(.DivAmt2)
                       
            Case GINT_INDEXES
                SetCBOValue cboPrimExchange, .ExchangeID
                
                txtYield.Text = gCmn.FmtDbl(.Yield * 100)
                chkIsActive.Value = IIf(.IsActive <> 0, 1, 0)

                CurDivType = .DivType
                SetDividendType CurDivType
                
                SetCBOValue cboDivFreq(0), .DivFreq
                dtpDivDate(0).Value = IIf(.DivDate = 0, Date, .DivDate)
                txtDivAmt(0).Text = gCmn.FmtDbl(.DivAmt)
                
                SetCBOValue cboDivFreq(1), .DivFreq2
                dtpDivDate(1).Value = IIf(.DivDate2 = 0, Date, .DivDate2)
                txtDivAmt(1).Text = gCmn.FmtDbl(.DivAmt2)
                
            Case GINT_FUTURES
                dtMaturityPicker.Value = IIf(.MaturityDate <> 0, .MaturityDate, Date)
                dtFutMaturity = dtMaturityPicker.Value
                txtLotSize.Text = IIf(.LotSize <> 0, Format$(.LotSize, "#,##0"), "N/A")
                
'            Case GINT_INTEREST
'                 chkDysplayAmountInFCE.Value = IIf(.GreeksCalcType <> 0, 1, 0)
'                 chkIsActive.Value = IIf(.IsActive <> 0, 1, 0)
            Case GINT_COMMODITIES
                chkDysplayAmountInFCE.Value = IIf(.GreeksCalcType <> 0, 1, 0)
                 chkIsActive.Value = IIf(.IsActive <> 0, 1, 0)
        End Select
    End With
    
    SetCBOValue cboContracts, m_Contract.ID
    
    GeneralOptionListLoad

    SetState STATE_NONE
    
    SetContractButtonsState

    Exit Function
EH:
    ShowError
    SetState STATE_NONE
End Function

'////////////////////////////////////
' enabling navigation buttons
Private Sub SetContractButtonsState()
    On Error Resume Next

    cboContracts.Enabled = (m_State <> STATE_CONTRACT_ADD)

    With cboContracts
        btnContractNav(1).Enabled = (.ListIndex > 0) And (m_State <> STATE_CONTRACT_ADD)
        btnContractNav(2).Enabled = (.ListIndex > 0) And (m_State <> STATE_CONTRACT_ADD)
        btnContractNav(3).Enabled = (.ListIndex < .ListCount - 1) And (m_State <> STATE_CONTRACT_ADD)
        btnContractNav(4).Enabled = (.ListIndex < .ListCount - 1) And (m_State <> STATE_CONTRACT_ADD)
    End With
    
    btnContractAdd.Enabled = (m_State = STATE_NONE And m_ContractType <> GINT_FUTURES)
    
    btnContractDelete.Enabled = (m_State = STATE_NONE)
    btnContractApply.Enabled = (m_State = STATE_CONTRACT_ADD) Or (m_State = STATE_CONTRACT_EDIT)
    btnContractCancel.Enabled = (m_State = STATE_CONTRACT_ADD) Or (m_State = STATE_CONTRACT_EDIT)
    
    ' Sharky: market structure import is temporarly disabled for futures
    btnContractImport.Enabled = (m_State = STATE_NONE)
    
    btnStockToIndex.Enabled = (m_State = STATE_NONE And m_ContractType <> GINT_FUTURES)
    btnCAExecute.Enabled = (m_State = STATE_NONE And m_ContractType = GINT_STOCKS)
    btnCAList.Enabled = (m_State = STATE_NONE And m_ContractType = GINT_STOCKS)
    
    'check achuchev sub-marketdivs
    'btnUpdateComposition.Enabled = (m_State = STATE_NONE And chkIsBasket.Value)

    btnUnd.Enabled = (m_State = STATE_NONE)
    btnSymbLoadIV.Enabled = (m_State = STATE_NONE)
    
    btnGroups.Enabled = (m_State = STATE_NONE And m_ContractType <> GINT_FUTURES)
    btnExchanges.Enabled = (m_State = STATE_NONE)
    btnMarketValues.Enabled = (m_State = STATE_NONE)
    btnBeta.Enabled = (m_State = STATE_NONE And m_ContractType <> GINT_FUTURES)
    If (m_ContractType = GINT_FUTURES) Then
'        Dim m_rsTmp As ADODB.Recordset
'        Set m_rsTmp = gDBW.usp_FutureIC_Get(m_Contract.ID)
'        If Not m_rsTmp.EOF Then
            btnBeta.Enabled = True
'        End If
'        m_rsTmp.Close
    End If
    
    btnOptionAdd.Enabled = (m_State = STATE_NONE)
    btnOptionDelete.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
    btnOptionEdit.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
    
    btnOptionRoots.Enabled = (m_State = STATE_NONE And grdOptions.Rows > 2 And m_ContractType <> GINT_FUTURES)
    
    
    If m_ContractType = GINT_COMMODITIES Then
        fgRoots.Visible = True
        AddButton.Visible = True
        DeleteButton.Visible = True
        btnFuturesRoots.Visible = False
        'chkDisplayDltGma.Visible = True
        'chkDysplayAmountInFCE.Visible = False
        'txtDefCal.Visible = True
        'lblDefCal.Visible = True
        'chkDisplayDltGma.Visible = True
        cboFutUndType.Visible = True
        cboType.Visible = True
        lblType.Visible = True
        
        btnOptionAdd.Enabled = False
        btnOptionDelete.Enabled = False
        btnOptionEdit.Enabled = False
            
        mnuOptionsAdd.Enabled = False
        mnuOptionEdit.Enabled = False
        mnuOptionsDelete.Enabled = False
        mnuOptionCallPrices.Enabled = False
        mnuOptionPutPrices.Enabled = False
                                                                            
    Else
        fgRoots.Visible = False
        AddButton.Visible = False
        DeleteButton.Visible = False
        'btnFuturesRoots.Visible = True
        'chkDisplayDltGma.Visible = False
        'chkDysplayAmountInFCE.Visible = True
        txtDefCal.Visible = False
        lblDefCal.Visible = False
        'chkDisplayDltGma.Visible = False
        cboFutUndType.Visible = False
        cboType.Visible = False
        lblType.Visible = False
        
        btnOptionAdd.Enabled = (m_State = STATE_NONE)
        btnOptionDelete.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
        btnOptionEdit.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
            
        mnuOptionsAdd.Enabled = (m_State = STATE_NONE)
        mnuOptionEdit.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
        mnuOptionsDelete.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
        mnuOptionCallPrices.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
        mnuOptionPutPrices.Enabled = (m_State = STATE_NONE) And (grdOptions.Rows > 2)
    
    End If


End Sub

'*****************************************************************************************
'                             Working with options
'*****************************************************************************************

Private Sub GeneralOptionListLoad()
    If (m_ContractType = GINT_INDEXES Or m_ContractType = GINT_STOCKS) Then
        OptionListLoad
    End If
    
    If m_ContractType = GINT_FUTURES Then
        FutureOptionListLoad
    End If
    
    If m_ContractType = GINT_COMMODITIES Then
        ICFuturesLoad
    End If
End Sub

Private Sub ICFuturesLoad()
    On Error GoTo EH
    Dim icf As ICFuture
    Dim nRow&, sKey$
    Dim dCurMonth As Date
    Dim iCurColor1 As OLE_COLOR
    Dim iCurColor2 As OLE_COLOR

    With grdOptions
        ' clearing option grid
        .Redraw = flexRDNone
        .Rows = 1
        
        If m_Contract.ID <> 0 Then
            ' loading options list
            If Not (m_rsOptionList Is Nothing) Then
                If m_rsOptionList.State = adStateOpen Then m_rsOptionList.Close
            End If
            Set m_rsOptionList = gDBW.usp_ICFutures_Get(m_Contract.ID)
            
            If Not m_rsOptionList.BOF Then m_rsOptionList.MoveFirst
            
            ' filling option grid
            Do While Not m_rsOptionList.EOF
                ReadICFuture m_rsOptionList, icf
                
                ' Don't show expired options
                If icf.MaturityDate >= Date Then
                    
                    ' Insert option pair into grid
                    .AddItem ""
                    nRow = .Rows - 1
                    
                    .TextMatrix(nRow, IC_FUTURESYMBOL) = icf.Symbol
                    .TextMatrix(nRow, IC_DESCRIPTION) = icf.Name
                    .TextMatrix(nRow, IC_SIZE) = gCmn.FmtLng(icf.FutureLotSize)
                    .TextMatrix(nRow, IC_CLOSEPRICE) = CustomDbl(icf.priceClose)
                    .TextMatrix(nRow, IC_MATURITYDATE) = gCmn.FmtDate(icf.MaturityDate)
                    
                    sKey = Format$(icf.MaturityDate, "YYYYMMDD") + Format$(icf.FutureLotSize, "00000.00") + icf.Symbol
                    .TextMatrix(nRow, IC_HIDDENCOLUMN) = sKey
                End If
                m_rsOptionList.MoveNext
            Loop
        End If
        
        .Col = IC_HIDDENCOLUMN
        .Sort = flexSortStringAscending
        
        ' painting grid
        iCurColor1 = COLOR11
        iCurColor2 = COLOR21
        If .Rows > 2 Then dCurMonth = (.TextMatrix(2, IC_MATURITYDATE))
        For nRow = 1 To .Rows - 1
            If dCurMonth <> (.TextMatrix(nRow, IC_MATURITYDATE)) Then
                dCurMonth = (.TextMatrix(nRow, IC_MATURITYDATE))
                If iCurColor1 = COLOR11 Then
                    iCurColor1 = COLOR12
                    iCurColor2 = COLOR22
                Else
                    iCurColor1 = COLOR11
                    iCurColor2 = COLOR21
                End If
            End If
            .Cell(flexcpBackColor, nRow, IC_FUTURESYMBOL, nRow, IC_DESCRIPTION) = iCurColor1
            .Cell(flexcpBackColor, nRow, IC_SIZE, nRow, IC_MATURITYDATE) = iCurColor2
        Next nRow
        
        If .Rows > 1 Then .Row = 1
        .Redraw = flexRDBuffered
    End With
    
    SetContractButtonsState
    
    Exit Sub
EH:
    grdOptions.Redraw = flexRDBuffered
    ShowError

End Sub


Private Sub OptionListLoad()
    On Error GoTo EH
    Dim op As New clsOptionPairAtom
    Dim nRow&, sKey$
    Dim dCurMonth As Date
    Dim iCurColor1 As OLE_COLOR
    Dim iCurColor2 As OLE_COLOR
    
    With grdOptions
        ' clearing option grid
        .Redraw = flexRDNone
        .Rows = 2
        
        If m_Contract.ID <> 0 Then
            ' loading options list
            If Not m_rsOptionList Is Nothing Then
                If m_rsOptionList.State = adStateOpen Then m_rsOptionList.Close
            End If
            '!!! ExchangeID ???
            Set m_rsOptionList = gDBW.usp_BoOptionWithPrice_Get(m_Contract.ID)
            
            m_rsOptionList.Filter = ""
            If Not m_rsOptionList.BOF Then m_rsOptionList.MoveFirst
            
            ' filling option grid
            Do While Not m_rsOptionList.EOF
                OptionPairRead m_rsOptionList, op
                
                ' Don't show expired options
                If op.dExpiry >= Date Then
                    ' Insert option pair into grid
                    .AddItem ""
                    nRow = .Rows - 1
                    If op.sCallSymbol > "" Then
                        .TextMatrix(nRow, OPT_CALLSYMBOL) = op.sCallSymbol
                        .TextMatrix(nRow, OPT_CALLCLOSEPRICE) = CustomDbl(op.fCallClose)
                        .TextMatrix(nRow, OPT_CALLIMPORTSYMBOL) = op.sCallImportSymbol
                    Else
                        .TextMatrix(nRow, OPT_CALLSYMBOL) = ""
                        .TextMatrix(nRow, OPT_CALLCLOSEPRICE) = ""
                        .TextMatrix(nRow, OPT_CALLIMPORTSYMBOL) = ""
                    End If
                    .TextMatrix(nRow, OPT_STRIKE) = gCmn.FmtDbl(op.fStrike)
                    .TextMatrix(nRow, OPT_EXPIRATION) = gCmn.FmtDate(op.dExpiry)
                    .TextMatrix(nRow, OPT_EXPIRYVALUATION) = gCmn.FmtDateTime(op.dExpiryOV)
'                    .TextMatrix(nRow, OPT_TRADINGCLOSE) = gCmn.FmtDate(op.dTradingClose)

                    If op.sPutSymbol > "" Then
                        .TextMatrix(nRow, OPT_PUTSYMBOL) = op.sPutSymbol
                        .TextMatrix(nRow, OPT_PUTCLOSEPRICE) = CustomDbl(op.fPutClose)
                        .TextMatrix(nRow, OPT_PUTIMPORTSYMBOL) = op.sPutImportSymbol
                    Else
                        .TextMatrix(nRow, OPT_PUTSYMBOL) = ""
                        .TextMatrix(nRow, OPT_PUTCLOSEPRICE) = ""
                        .TextMatrix(nRow, OPT_PUTIMPORTSYMBOL) = ""
                    End If
                    sKey = Format$(op.dExpiry, "YYYYMMDD") + Format$(op.fStrike, "00000.00") + IIf(op.sCallSymbol > "", op.sCallSymbol, op.sPutSymbol)
                    .TextMatrix(nRow, OPT_HIDDENCOLUMN) = sKey
                End If
            Loop
        End If
        
        .Col = OPT_HIDDENCOLUMN
        .Sort = flexSortStringAscending
        
        ' painting grid
        iCurColor1 = COLOR11
        iCurColor2 = COLOR21
        If .Rows > 2 Then dCurMonth = (.TextMatrix(2, OPT_EXPIRATION))
        For nRow = 2 To .Rows - 1
            If dCurMonth <> (.TextMatrix(nRow, OPT_EXPIRATION)) Then
                dCurMonth = (.TextMatrix(nRow, OPT_EXPIRATION))
                If iCurColor1 = COLOR11 Then
                    iCurColor1 = COLOR12
                    iCurColor2 = COLOR22
                Else
                    iCurColor1 = COLOR11
                    iCurColor2 = COLOR21
                End If
            End If
            .Cell(flexcpBackColor, nRow, OPT_CALLSYMBOL, nRow, OPT_CALLCLOSEPRICE) = iCurColor1
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRYVALUATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_PUTSYMBOL, nRow, OPT_PUTCLOSEPRICE) = iCurColor1
        Next nRow
        
        If .Rows > 2 Then .Row = 2
        .Redraw = flexRDBuffered
    End With
    
    SetContractButtonsState
    
Exit Sub
EH:
    grdOptions.Redraw = flexRDBuffered
    ShowError
End Sub

Private Function CustomDbl(ByVal D As Double) As String
    If Round(D, 4) = 0 Then
        CustomDbl = ""
    Else
        
        If Round(D, 2) = Round(D + 0.000005, 4) Then
            CustomDbl = gCmn.FmtDbl(D)
        Else
            CustomDbl = gCmn.FmtExtDbl(D)
        End If
    End If
End Function

' formating Interest/CommoditiesFX grid
Private Sub FormatICGrid()
    On Error GoTo EH
    With grdOptions
        .Clear
        .Rows = 0
        .Cols = IC_COLUMNS_COUNT
        .AddItem "Futures" & vbTab & "Description" & vbTab & "Size" & vbTab & "Close" & vbTab & "Maturity Date"
        '.AddItem "Future" & vbTab & "Description" & vbTab & "Size" & vbTab & "Close" & vbTab & "Maturity Date"
        .FixedRows = 1 '2
        .FixedCols = 0
        
        '.MergeCells =
'        .MergeRow(0) = True
'        .MergeRow(1) = True
        '.MergeCol(OPT_STRIKE) = True
        '.MergeCol(OPT_EXPIRATION) = True
        
        .Cell(flexcpBackColor, 0, 0, 0, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 0, .Cols - 1) = True
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        .ExplorerBar = flexExSortShow
        
        .ColDataType(IC_MATURITYDATE) = flexDTDate
        .ColFormat(IC_MATURITYDATE) = "DD MMM YYYY"
        .ColHidden(IC_HIDDENCOLUMN) = True
    End With
    Exit Sub
EH:
    ShowError
End Sub


' formating options grid
Private Sub FormatOptionGrid()
    On Error GoTo EH
    With grdOptions
        .Clear
        .Rows = 0
        .Cols = OPT_COLUMNS_COUNT
        .AddItem "Calls" & vbTab & "Calls" & vbTab & "Calls" & vbTab & "Strike" & vbTab & "Expiration" & vbTab & "Expiration for option" & vbCr & " valuation" & vbTab & "Puts" & vbTab & "Puts" & vbTab & "Puts"
        .AddItem "Symbol" & vbTab & "Import Id" & vbTab & "Close" & vbTab & "Strike" & vbTab & "Expiration" & vbTab & "Expiration for option" & vbCr & " valuation" & vbTab & "Symbol" & vbTab & "Import Id" & vbTab & "Close"
        .FixedRows = 2
        .FixedCols = 0
        .Cell(flexcpBackColor, 0, 0, 1, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 1, .Cols - 1) = True
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(OPT_STRIKE) = True
        .MergeCol(OPT_EXPIRATION) = True
        .MergeCol(OPT_EXPIRYVALUATION) = True
        .Cell(flexcpAlignment, 0, 0, 1, .Cols - 1) = flexAlignCenterCenter
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        .ExplorerBar = flexExSortShow
        
        '.ColDataType(OPT_STRIKE) = flexDTDouble
        '.ColFormat(OPT_STRIKE) = "0.00"
        
        .ColDataType(OPT_EXPIRATION) = flexDTDate
        .ColFormat(OPT_EXPIRATION) = "MMM, DD YYYY"
'        .ColDataType(OPT_EXPIRYVALUATION) = flexDTString 'Date
'        .ColFormat(OPT_EXPIRYVALUATION) = "MMM,dd yyyy : hh:mm AM/PM"
        .ColHidden(OPT_HIDDENCOLUMN) = True
        .ColHidden(IC_HIDDENCOLUMN) = False
    End With
    Exit Sub
EH:
    ShowError
End Sub

Private Function GetCalendarComboBox() As String
    On Error Resume Next
    Dim rs As ADODB.Recordset, sComboList$
    On Error Resume Next
    sComboList = ""
    
    Set rs = gDBW.usp_ExpiryCalendar_Get
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sComboList = sComboList & "#" & gCmn.ReadStr(.Fields("iExpCalendarID"))
                sComboList = sComboList & ";" & gCmn.ReadStr(.Fields("vcExpCalendarName"))
                sComboList = sComboList & vbTab & gCmn.ReadStr(.Fields("vcExpCalendarDesc")) & "|"
                .MoveNext
            Loop
        End If
    End With
    GetCalendarComboBox = sComboList
End Function

Private Function GetQuotationUnitComboBox() As String
    On Error Resume Next
    Dim rs As ADODB.Recordset, sComboList$
    On Error Resume Next
    sComboList = ""
    
    Set rs = gDBW.usp_QuotationUnit_Get
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                sComboList = sComboList & "#" & gCmn.ReadStr(.Fields("iQuotationUnitID"))
                sComboList = sComboList & ";" & gCmn.ReadStr(.Fields("vcName"))
                sComboList = sComboList & vbTab & gCmn.ReadStr(.Fields("vcDesc")) & "|"
                .MoveNext
            Loop
        End If
    End With
    GetQuotationUnitComboBox = sComboList
End Function

Private Function GetQuoteFormatComboBox() As String
    On Error Resume Next
    On Error Resume Next
    
    GetQuoteFormatComboBox = "#0;$" & vbTab & "dollar|#1;c" & vbTab & "cent|#2;point" & vbTab & "point|"
End Function

Private Sub FormatRootGrid()
    On Error GoTo EH
    With fgRoots
        .Clear
        .Rows = 0
        .Cols = ROOT_COLUMNS_COUNT
        
        .AddItem "Symbol" & vbTab & "Name" & vbTab & "ImportID" & vbTab & "FutContractSize" & vbTab & "OptContractSize" & vbTab & "Quote Format" & vbTab & "Quotation Unit" & vbTab & "Quotation Unit" & vbTab & "Quote Denomination" & vbTab & "Tick Size" & vbTab & "Tick Value$"
        .AddItem "Symbol" & vbTab & "Name" & vbTab & "ImportID" & vbTab & "FutContractSize" & vbTab & "OptContractSize" & vbTab & "Quote Format" & vbTab & "Amount" & vbTab & "Unit" & vbTab & "Quote Denomination" & vbTab & "Tick Size" & vbTab & "Tick Value$"
        .FixedRows = 2
        .FixedCols = 0
        .Cell(flexcpBackColor, 0, 0, 1, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 1, .Cols - 1) = True
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(root_symbol) = True
        .MergeCol(ROOT_NAME) = True
        .MergeCol(ROOT_IMPORTID) = True
        .MergeCol(ROOT_FUTCONTRACTSIZE) = True
        .MergeCol(ROOT_OPTCONTRACTSIZE) = True
        .MergeCol(ROOT_QUOTEDENOMINATION) = True
        .MergeCol(ROOT_QUOTEFORMAT) = True
        .MergeCol(ROOT_TICKSIZE) = True
        .MergeCol(ROOT_TICKVALUE) = True
        .Cell(flexcpAlignment, 0, 0, 1, .Cols - 1) = flexAlignCenterCenter
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        .ExplorerBar = flexExSortShow
        
        .ColDataType(ROOT_FUTCONTRACTSIZE) = flexDTLong
        .ColDataType(ROOT_OPTCONTRACTSIZE) = flexDTLong
        .ColDataType(ROOT_QUOTATIONUNITAMMOUNT) = flexDTLong
        .ColDataType(ROOT_TICKSIZE) = flexDTDouble
        
        .ColHidden(ROOT_HIDDENCOLUMN) = True
        '.ColHidden(IC_HIDDENCOLUMN) = False
        
        .ColWidth(root_symbol) = 1000
        .ColWidth(ROOT_NAME) = 1000
        .ColWidth(ROOT_IMPORTID) = 1000
        .ColWidth(ROOT_FUTCONTRACTSIZE) = 1500
        .ColWidth(ROOT_OPTCONTRACTSIZE) = 1500
        .ColWidth(ROOT_QUOTEFORMAT) = 900
        .ColWidth(ROOT_QUOTATIONUNITAMMOUNT) = 1000
        .ColWidth(ROOT_QUOTATIONUNITUNIT) = 1000
        .ColWidth(ROOT_QUOTEDENOMINATION) = 1400
        .ColWidth(ROOT_TICKSIZE) = 800
        .ColWidth(ROOT_TICKVALUE) = 800
        
        Dim i As Long
        
        i = ROOT_QUOTEFORMAT
        .TextMatrix(0, i) = "Quote" & vbCrLf & "Format"
        .TextMatrix(1, i) = "Quote" & vbCrLf & "Format"
        .ColDataType(i) = flexDTString
        .ColComboList(i) = GetQuoteFormatComboBox
        
        i = ROOT_QUOTEDENOMINATION
        .TextMatrix(0, i) = "Quote" & vbCrLf & "Denomination"
        .TextMatrix(1, i) = "Quote" & vbCrLf & "Denomination"
        
        i = ROOT_TICKSIZE
        .TextMatrix(0, i) = "Tick" & vbCrLf & "Size"
        .TextMatrix(1, i) = "Tick" & vbCrLf & "Size"
        
        i = ROOT_TICKVALUE
        .TextMatrix(0, i) = "Tick" & vbCrLf & "Value$"
        .TextMatrix(1, i) = "Tick" & vbCrLf & "Value$"
        
        i = ROOT_QUOTATIONUNITUNIT
        .ColDataType(i) = flexDTString
        .ColComboList(i) = GetQuotationUnitComboBox
        
    End With
    Exit Sub
EH:
    ShowError
End Sub

' addin option pair to display
Public Sub OptionPairAdd()
    On Error GoTo EH
    Dim Opt As New clsOptionPairAtom
    Opt.dExpiry = Date
    Opt.dExpiryOV = Date + CDate("16:00:00")
    Opt.dTradingClose = CDate("16:00:00")
    If frmMktStrOptionPairEditor.Execute(m_Contract.ID, m_Contract.TypeID, m_Contract.CalendarType, Opt, True) Then GeneralOptionListLoad
    Exit Sub
EH:
    ShowError
End Sub

' editing option pair
Public Sub OptionPairEdit(bEdit As Boolean)
    On Error GoTo EH
    Dim Opt As New clsOptionPairAtom
    Dim iTopRow As Long
    Dim iCurRow As Long
    Dim iCurCol As Long
'    Dim sDateTime As String
    ' getting Call and Put symbols
    With grdOptions
        Opt.sCallSymbol = .TextMatrix(.Row, OPT_CALLSYMBOL)
        Opt.sPutSymbol = .TextMatrix(.Row, OPT_PUTSYMBOL)
        Opt.sCallImportSymbol = .TextMatrix(.Row, OPT_CALLIMPORTSYMBOL)
        Opt.sPutImportSymbol = .TextMatrix(.Row, OPT_PUTIMPORTSYMBOL)
    End With
    
    With m_rsOptionList
        .Filter = ""
        If .RecordCount > 0 Then
            ' getting Call and put IDs
            .Filter = "vcSymbol='" & FixDbString(Opt.sCallSymbol) & "'"
            If .RecordCount > 0 Then
                Opt.iCallID = .Fields("iContractID")
                Opt.fStrike = Round(.Fields("fStrike"), 4)
                Opt.dExpiry = .Fields("dtExpiry")
                Opt.dExpiryOV = .Fields("dtExpiryOV")
                Opt.dTradingClose = .Fields("dtTradeClose")
                Opt.dExpiryOV = grdOptions.TextMatrix(grdOptions.Row, OPT_EXPIRYVALUATION)
            If m_ContractType <> GINT_FUTURES Then Opt.bCallManual = IIf(.Fields("vcIsFlex") = 0, False, True)
                Opt.bPutManual = Opt.bCallManual
            End If
            
            .Filter = "vcSymbol='" & FixDbString(Opt.sPutSymbol) & "'"
            If .RecordCount > 0 Then
                Opt.iPutID = .Fields("iContractID")
                Opt.fStrike = Round(.Fields("fStrike"), 4)
                Opt.dExpiry = .Fields("dtExpiry")
                Opt.dExpiryOV = .Fields("dtExpiryOV")
                Opt.dExpiryOV = grdOptions.TextMatrix(grdOptions.Row, OPT_EXPIRYVALUATION)
                Opt.dTradingClose = .Fields("dtTradeClose")
                If m_ContractType <> GINT_FUTURES Then Opt.bCallManual = IIf(.Fields("vcIsFlex") = 0, False, True)
                Opt.bPutManual = Opt.bCallManual
            End If
            
            ' saving gtid state
            iTopRow = grdOptions.TopRow
            iCurRow = grdOptions.Row
            iCurCol = grdOptions.Col
            
            If bEdit Then ' showing option pair editor
                If frmMktStrOptionPairEditor.Execute(m_Contract.ID, m_Contract.TypeID, m_Contract.CalendarType, Opt, True) Then GeneralOptionListLoad
            Else ' Just save data for Expiration Valuation
                If frmMktStrOptionPairEditor.Execute(m_Contract.ID, m_Contract.TypeID, m_Contract.CalendarType, Opt, False) Then GeneralOptionListLoad
            End If
            
            ' restoring grid state
            If iTopRow < grdOptions.Rows Then grdOptions.TopRow = iTopRow
            If iCurRow < grdOptions.Rows Then grdOptions.Row = iCurRow
            If iCurCol < grdOptions.Cols Then grdOptions.Col = iCurCol
        Else
            ShowWarning "No option selected"
        End If
    End With
    Exit Sub
EH:
    ShowError
End Sub

Public Sub OptionPairDelete()
    On Error GoTo EH
    Dim Opt As New clsOptionPairAtom

    ' getting call and put symbols
    With grdOptions
        Opt.sCallSymbol = .TextMatrix(.Row, OPT_CALLSYMBOL)
        Opt.sPutSymbol = .TextMatrix(.Row, OPT_PUTSYMBOL)
    End With
    
    With m_rsOptionList
        .Filter = ""
        If .RecordCount > 0 Then
            ' deleting options from database
            If gCmn.MyMsgBox("Do you wish to delete the current options?", vbYesNo) = vbYes Then
                ' getting call and put IDs
                .Filter = "vcSymbol='" & FixDbString(Opt.sCallSymbol) & "'"
                Opt.iCallID = IIf(.RecordCount > 0, .Fields("iContractID"), Null)
                .Filter = "vcSymbol='" & FixDbString(Opt.sPutSymbol) & "'"
                Opt.iPutID = IIf(.RecordCount > 0, .Fields("iContractID"), Null)
                
                ' deleting options from database
                If m_Contract.TypeID = GINT_FUTURES Then
                    gDBW.usp_BoFutureOptionPair_Del Opt.iCallID, Opt.iPutID
                Else
                    gDBW.usp_OptionPair_Del Opt.iCallID, Opt.iPutID
                End If
                
                ' reloading option list
                GeneralOptionListLoad
            End If
        Else
            ShowWarning "No option selected"
        End If
    End With
    Exit Sub
EH:
    ShowError
End Sub
 

Private Sub ReadICFuture(ByRef rst As ADODB.Recordset, ByRef op As ICFuture)
    On Error GoTo EH
    With op
        .CommodityID = rst!CommodityID
        .Symbol = DBRead(rst!vcSymbol, "")
        .FutureLotSize = DBRead(rst!FutureLotSize, 0)
        .OptionLotSize = DBRead(rst!OptioLotSize, 0)
        .Name = DBRead(rst!FutureName, "")
        .priceClose = DBRead(rst!priceClose, 0)
        .MaturityDate = DBRead(rst!MaturityDate, 0)
    End With
    Exit Sub
EH:
    ShowError
End Sub

' reading option pair from recordset
Private Sub OptionPairRead(ByRef rst As ADODB.Recordset, ByRef op As clsOptionPairAtom, _
                                                        Optional ByVal bIsFutureOption As Boolean = False)
    On Error GoTo EH

    With op
        If rst!bIsCall <> 0 Then
            .iCallID = rst!iContractID
            .sCallSymbol = DBRead(rst!vcSymbol, "")
            .fCallClose = DBRead(rst!fPriceClose, 0)
            .sCallImportSymbol = DBRead(rst!vcExportSymbol, "")
            .iRootID = DBRead(rst!iOptionRootID, 0)
                        
            .dExpiry = DBRead(rst!dtExpiry, 0#)
            .dExpiryOV = DBRead(rst!dtExpiryOV, 0#)
            .dTradingClose = ClipDays(DBRead(rst!dtTradeClose, 0#))
            .fStrike = Round(DBRead(rst!fStrike, 0), 4)
            .iPutID = Null
            .sPutSymbol = ""
            .fPutClose = 0
            .sPutImportSymbol = ""
            If Not bIsFutureOption Then .bCallManual = IIf(DBRead(rst!vcIsFlex, 0) = 0, False, True)
            
            ' the next record expected to be Put for this option pair
            rst.MoveNext
            If rst.EOF Then Exit Sub
            Dim D As Date
            Dim f As Double
            Dim RId As Long
            
            D = DBRead(rst!dtExpiry, 0#)
            f = Round(DBRead(rst!fStrike, 0), 4)
            RId = DBRead(rst!iOptionRootID, 0)
            
            If (.iRootID = RId) And (Year(.dExpiry) = Year(D) And Month(.dExpiry) = Month(D)) And (.fStrike = f) Then
                .iPutID = rst!iContractID
                .sPutSymbol = DBRead(rst!vcSymbol, "")
                .fPutClose = DBRead(rst!fPriceClose, 0)
                .sPutImportSymbol = DBRead(rst!vcExportSymbol, "")
                .bPutManual = .bCallManual
                On Error GoTo EH
                
                rst.MoveNext
            End If
        Else
            .iCallID = Null
            .sCallSymbol = ""
            .fCallClose = 0
            .sCallImportSymbol = ""
            
            .dExpiry = DBRead(rst!dtExpiry, 0#)
            .dExpiryOV = DBRead(rst!dtExpiryOV, 0#)
            .dTradingClose = ClipDays(DBRead(rst!dtTradeClose, 0#))
            .fStrike = Round(DBRead(rst!fStrike, 0), 4)
            .iPutID = rst!iContractID
            .sPutSymbol = DBRead(rst!vcSymbol, "")
            .fPutClose = DBRead(rst!fPriceClose, 0)
            .sPutImportSymbol = DBRead(rst!vcExportSymbol, "")
            If Not bIsFutureOption Then .bCallManual = IIf(DBRead(rst!vcIsFlex, 0) = 0, False, True)
            .bPutManual = .bCallManual

            rst.MoveNext
        End If
    End With
    Exit Sub
EH:
    ShowError
End Sub

Private Function ShowMarketValues(ByVal sSymbol As String, Optional ByVal iContractTypeID As Long = 0) As Boolean
    On Error GoTo EH
    ShowMarketValues = frmMarketValues.Execute(sSymbol, iContractTypeID)
    Exit Function
EH:
    ShowError
End Function

' setting ComboBox value
Private Sub SetCBOValue(ByRef cbo As MSForms.ComboBox, ByVal Value As Long)
    On Error GoTo EH
    Dim i As Long

    With cbo
        For i = 0 To .ListCount - 1
            If .List(i, .BoundColumn - 1) = Value Then
                .ListIndex = i
                Exit Sub
            End If
        Next i
        .ListIndex = -1
    End With
    Exit Sub
EH:
    ShowError
End Sub

'**************************************************************
' Handling interface events
'**************************************************************
Private Sub btnContractAdd_Click()
    On Error Resume Next
    If ContractCanProceed Then ContractAdd
End Sub

Private Sub btnContractApply_Click()
    On Error Resume Next
    ContractSave
End Sub

Private Sub btnContractCancel_Click()
    On Error Resume Next
    ContractCancel
End Sub

Private Sub btnContractDelete_Click()
    On Error Resume Next
    ContractDelete
End Sub

Private Sub btnContractImport_Click()
    On Error Resume Next
    If ContractCanProceed Then ContractImport
End Sub

Private Sub btnContractNav_Click(Index As Integer)
    On Error Resume Next
    If Not ContractCanProceed Then Exit Sub
    With cboContracts
        If .ListCount = 0 Then Exit Sub
        Select Case Index
            Case 1
                If .ListIndex > 0 Then .ListIndex = 0
            Case 2
                If .ListIndex > 0 Then .ListIndex = .ListIndex - 1
            Case 3
                If .ListIndex < .ListCount - 1 Then .ListIndex = .ListIndex + 1
            Case 4
                If .ListIndex < .ListCount - 1 Then .ListIndex = .ListCount - 1
        End Select
        
        m_Contract.ID = .Value
        ContractLoad
    End With
End Sub

Private Sub btnOptionAdd_Click()
    On Error Resume Next
    If ContractCanProceed Then OptionPairAdd
End Sub

Private Sub btnOptionDelete_Click()
    On Error Resume Next
    If ContractCanProceed Then OptionPairDelete
End Sub

Private Sub btnOptionEdit_Click()
    On Error Resume Next
    If ContractCanProceed Then OptionPairEdit (True)
End Sub

Private Sub cboContracts_Change()
    On Error Resume Next
    If m_State = STATE_LOAD Then Exit Sub
    cboContracts.ForeColor = IIf(cboContracts.MatchFound, &H0, &H808080)
End Sub

Private Sub cboContracts_Validate(Cancel As Boolean)
    On Error Resume Next
    If m_State = STATE_LOAD Then Exit Sub
    Dim nNewId&
    
    With cboContracts
        If .MatchFound Then
            nNewId = gCmn.ReadLng(.Value)
            If nNewId <> m_Contract.ID Then
                m_Contract.ID = nNewId
                ContractLoad
                'LoadFutureRoots
            End If
        Else
            .Text = m_Contract.Symbol
            Cancel = True
        End If
    End With
End Sub

'Private Sub cboContractType_Click()
'    On Error Resume Next
'    If m_State = STATE_LOAD Then Exit Sub
'    SetContractType cboContractType.Value
'End Sub

Private Sub btnMarketValues_Click()
    On Error Resume Next
    If ShowMarketValues(m_Contract.Symbol, m_Contract.TypeID) Then ContractLoad
End Sub

Private Sub btnGroups_Click()
    On Error Resume Next
    If ContractCanProceed Then frmMktStrGroupsOfAStock.Execute m_Contract.ID, m_Contract.Symbol
End Sub

Private Sub btnExchanges_Click()
    On Error Resume Next
    If ContractCanProceed Then frmStockExchanges.Execute m_Contract.ID, m_Contract.Symbol
End Sub

Private Sub btnOptionRoots_Click()
    On Error Resume Next
    If ContractCanProceed Then
        If frmRootList.Execute(m_Contract.ID, m_Contract.TypeID, m_Contract.Symbol, m_sContracts, m_sIndexes) Then ContractLoad
    End If
End Sub

Private Sub mnuOptionCallPrices_Click()
    On Error Resume Next
    Dim iTopRow As Long
    Dim iCurRow As Long
    Dim iCurCol As Long
    
    If m_rsOptionList.RecordCount = 0 Then Exit Sub
    With grdOptions
        iTopRow = .TopRow
        iCurRow = .Row
        iCurCol = .Col
        If ShowMarketValues(.TextMatrix(.Row, OPT_CALLSYMBOL), IIf(m_ContractType <> GINT_FUTURES, CONTRACT_TYPE_OPTION, 0)) Then GeneralOptionListLoad
        If iTopRow < .Rows Then .TopRow = iTopRow
        If iCurRow < .Rows Then .Row = iCurRow
        If iCurCol < .Cols Then .Col = iCurCol
    End With
End Sub

Private Sub mnuOptionEdit_Click()
    On Error Resume Next
    OptionPairEdit (True)
End Sub

Private Sub mnuOptionPutPrices_Click()
    On Error Resume Next
    Dim iTopRow As Long
    Dim iCurRow As Long
    Dim iCurCol As Long
    
    If m_rsOptionList.RecordCount = 0 Then Exit Sub
    With grdOptions
        iTopRow = .TopRow
        iCurRow = .Row
        iCurCol = .Col
        If ShowMarketValues(.TextMatrix(.Row, OPT_PUTSYMBOL), IIf(m_ContractType <> GINT_FUTURES, CONTRACT_TYPE_OPTION, 0)) Then GeneralOptionListLoad
        If iTopRow < .Rows Then .TopRow = iTopRow
        If iCurRow < .Rows Then .Row = iCurRow
        If iCurCol < .Cols Then .Col = iCurCol
    End With
End Sub

Private Sub mnuOptionsAdd_Click()
    On Error Resume Next
    OptionPairAdd
End Sub

Private Sub mnuOptionsDelete_Click()
    On Error Resume Next
    OptionPairDelete
End Sub

Private Sub mnuOptionsDefSort_Click()
    Dim nRow As Long
    Dim dCurMonth As Date
    Dim iCurColor1 As OLE_COLOR
    Dim iCurColor2 As OLE_COLOR
    
    On Error Resume Next
    With grdOptions
        .Redraw = flexRDNone
        .Col = OPT_HIDDENCOLUMN
        .Sort = flexSortStringAscending
        
        iCurColor1 = COLOR11
        iCurColor2 = COLOR21
        If .Rows > 2 Then dCurMonth = (.TextMatrix(2, OPT_EXPIRATION))
        For nRow = 2 To .Rows - 1
            If dCurMonth <> (.TextMatrix(nRow, OPT_EXPIRATION)) Then
                dCurMonth = (.TextMatrix(nRow, OPT_EXPIRATION))
                If iCurColor1 = COLOR11 Then
                    iCurColor1 = COLOR12
                    iCurColor2 = COLOR22
                Else
                    iCurColor1 = COLOR11
                    iCurColor2 = COLOR21
                End If
            End If
            .Cell(flexcpBackColor, nRow, OPT_CALLSYMBOL, nRow, OPT_CALLCLOSEPRICE) = iCurColor1
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRYVALUATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_PUTSYMBOL, nRow, OPT_PUTCLOSEPRICE) = iCurColor1
        Next
        
        .Redraw = flexRDBuffered
    End With
End Sub

Private Sub picMain_Resize()
    Dim iCol As Long
    Dim iWidth As Single
    On Error Resume Next
    With grdOptions
        If (picMain.ScaleWidth - .Left) > 10 Then
            .Width = picMain.ScaleWidth - .Left
        Else
            .Width = 10
        End If
        If (picMain.ScaleHeight - .Top) > 10 Then
            .Height = picMain.ScaleHeight - .Top
        Else
            .Height = 10
        End If
        iWidth = .Width
        If iWidth < 7000 Then
            iWidth = 7000
        End If
        For iCol = 0 To .Cols - 2
            .ColWidth(iCol) = iWidth / (.Cols - 1) - 40
        Next iCol
    End With
       
    With fgRoots
        If (picMain.ScaleWidth - .Left) > 10 Then
            .Width = picMain.ScaleWidth - .Left - 10
        Else
            .Width = 10
        End If
    End With
End Sub

Private Sub cboPrimExchange_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub cboPrimExchange_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtName_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtSymbol_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub cboDivFreq_Click(Index As Integer)
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub dtpDivDate_Change(Index As Integer)
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtDivAmt_Change(Index As Integer)
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub txtYield_Change()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub cboContracts_GotFocus()
    On Error Resume Next
    gCmn.SelectText cboContracts
End Sub

Private Sub txtName_GotFocus()
    On Error Resume Next
    gCmn.SelectText txtName
End Sub

Private Sub txtSymbol_GotFocus()
    On Error Resume Next
    gCmn.SelectText txtSymbol
End Sub

Private Sub cboPrimExchange_GotFocus()
    On Error Resume Next
    gCmn.SelectText cboPrimExchange
End Sub

Private Sub txtDivAmt_GotFocus(Index As Integer)
    On Error Resume Next
    gCmn.SelectText txtDivAmt(Index)
End Sub

Private Sub txtYield_GotFocus()
    On Error Resume Next
    gCmn.SelectText txtYield
End Sub

Private Sub grdOptions_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    On Error Resume Next
    If Button And vbRightButton Then
        grdOptions.Col = grdOptions.MouseCol
        If grdOptions.MouseRow >= 2 Then grdOptions.Row = grdOptions.MouseRow
        ShowOptionPopUp
    End If
End Sub

' popup menu wrapper
Private Sub ShowOptionPopUp(Optional ByVal X As Variant, Optional ByVal y As Variant)
    On Error GoTo EH
    Dim iRow As Long
    Dim iCol As Long
    Dim objDefMenu As Menu
    
    With grdOptions
        iRow = .Row
        iCol = .Col
        
        Select Case iCol
            Case 0, 1, 2, 3
                Set objDefMenu = mnuOptionCallPrices
            Case 4, 5
                Set objDefMenu = mnuOptionEdit
            Case 6, 7, 8, 9
                Set objDefMenu = mnuOptionPutPrices
        End Select
        
        If Not objDefMenu Is Nothing Then
            If IsMissing(X) Then
                PopupMenu mnuOptions, , , , objDefMenu
            Else
                PopupMenu mnuOptions, , X, y, objDefMenu
            End If
        End If
    End With
    Exit Sub
EH:
    ShowError
End Sub

Private Sub grdOptions_DblClick()
    On Error GoTo EH
    Dim iRow As Long
    Dim iCol As Long

    If bDblClickHandled Then
        bDblClickHandled = False
        Exit Sub
    End If
    With grdOptions
        iRow = .Row
        iCol = .Col
        If (m_ContractType <> GINT_COMMODITIES) Then
            If iRow < 2 Then Exit Sub
            Select Case iCol
                Case OPT_CALLSYMBOL, OPT_CALLCLOSEPRICE, OPT_CALLIMPORTSYMBOL
                    mnuOptionCallPrices_Click
                Case OPT_STRIKE, OPT_EXPIRATION, OPT_EXPIRYVALUATION
                    OptionPairEdit (True)
                Case OPT_PUTSYMBOL, OPT_PUTCLOSEPRICE, OPT_PUTIMPORTSYMBOL
                    mnuOptionPutPrices_Click
            End Select
        Else
            If iRow < 1 Then Exit Sub
            Dim s As String
            s = grdOptions.TextMatrix(iRow, IC_FUTURESYMBOL)
            SetContractType GINT_FUTURES
            cboContracts.Text = s
            cboContracts_Validate (True)
        End If
    End With
    Exit Sub
EH:
    gCmn.ErrorMsgBox Err.Description
End Sub

Private Sub grdOptions_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If KeyCode = vbKeyReturn Then grdOptions_DblClick
End Sub

Public Function RefreshMarket()
    ContractListLoad
End Function

Public Function RefreshCurrent()
    ContractLoad
End Function

Private Sub btnUnd_Click()
    On Error GoTo EH
    Dim iAnswr As VbMsgBoxResult
    Dim ContractCanProceed As Boolean
    
    If (m_State = STATE_CONTRACT_ADD) Or (m_State = STATE_CONTRACT_EDIT) Then
        iAnswr = gCmn.MyMsgBox("Do you wish to save changes?", vbYesNoCancel)
        If iAnswr = vbYes Then
            ' save contract
            ContractCanProceed = ContractSave
        ElseIf iAnswr = vbNo Then
            ' cancel changes
            ContractCanProceed = ContractCancel
        Else
            ContractCanProceed = False
        End If
    Else
        ContractCanProceed = True
    End If
    
    If ContractCanProceed Then
        frmUndList.Execute
        ContractLoad
    End If
    Exit Sub
EH:
    ShowError
End Sub

Private Sub opAmerican_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub opEuropean_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
End Sub

Private Sub cboCalendar_Change()
    On Error Resume Next
    If m_State = STATE_LOAD Then Exit Sub
    With cboContracts
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End With
End Sub

Private Sub chkIsBasket_Click()
    On Error Resume Next
    If m_State = STATE_NONE Then
        SetState STATE_CONTRACT_EDIT
        m_OldContract = m_Contract
        SetContractButtonsState
    End If
    
'   check achuchev sub-marketdivs
'    Dim IsBasket As Boolean
'    IsBasket = (chkIsBasket.Value <> 0)
'    btnComponents.Visible = IsBasket
'    btnComponentsDividends.Visible = IsBasket
'    lblYield.Visible = Not IsBasket
'    txtYield.Visible = Not IsBasket
End Sub

Private Sub btnComponents_Click()
    On Error Resume Next
    If ContractCanProceed Then frmComponentsOfIndex.Execute m_Contract.ID, m_Contract.Symbol
End Sub

Private Sub btnComponentsDividends_Click()
    On Error Resume Next
    If ContractCanProceed Then frmComponentsDividends.Execute m_Contract.ID, m_Contract.Symbol
End Sub

Private Sub FutureOptionListLoad()
    On Error GoTo EH
    Dim op As New clsOptionPairAtom
    Dim nRow&, sKey$
    Dim dCurMonth As Date
    Dim iCurColor1 As OLE_COLOR
    Dim iCurColor2 As OLE_COLOR

    With grdOptions
        ' clearing option grid
        .Redraw = flexRDNone
        .Rows = 2
        
        If m_Contract.ID <> 0 Then
            ' loading options list
            If Not (m_rsOptionList Is Nothing) Then
                If m_rsOptionList.State = adStateOpen Then m_rsOptionList.Close
            End If
            Set m_rsOptionList = gDBW.usp_BoFutureOptionWithPrice_Get(m_Contract.ID)
            
            If Not m_rsOptionList.BOF Then m_rsOptionList.MoveFirst
            
            ' filling option grid
            Do While Not m_rsOptionList.EOF
                OptionPairRead m_rsOptionList, op, True
                
                ' Don't show expired options
                If op.dExpiry >= Date Then
                    ' Insert option pair into grid
                    .AddItem ""
                    nRow = .Rows - 1
                    
                    If op.sCallSymbol > "" Then
                        .TextMatrix(nRow, OPT_CALLSYMBOL) = op.sCallSymbol
                        .TextMatrix(nRow, OPT_CALLCLOSEPRICE) = CustomDbl(op.fCallClose)
                        .TextMatrix(nRow, OPT_CALLIMPORTSYMBOL) = op.sCallImportSymbol
                    Else
                        .TextMatrix(nRow, OPT_CALLSYMBOL) = ""
                        .TextMatrix(nRow, OPT_CALLCLOSEPRICE) = ""
                        .TextMatrix(nRow, OPT_CALLIMPORTSYMBOL) = ""
                    End If
                    .TextMatrix(nRow, OPT_STRIKE) = gCmn.FmtDbl(op.fStrike)
                    .TextMatrix(nRow, OPT_EXPIRATION) = gCmn.FmtDate(op.dExpiry)
                    .TextMatrix(nRow, OPT_EXPIRYVALUATION) = gCmn.FmtDateTime(op.dExpiryOV)
'                    .TextMatrix(nRow, OPT_TRADINGCLOSE) = gCmn.FmtDate(op.dTradingClose)
                    
                    If op.sPutSymbol > "" Then
                        .TextMatrix(nRow, OPT_PUTSYMBOL) = op.sPutSymbol
                        .TextMatrix(nRow, OPT_PUTCLOSEPRICE) = CustomDbl(op.fPutClose)
                        .TextMatrix(nRow, OPT_PUTIMPORTSYMBOL) = op.sPutImportSymbol
                    Else
                        .TextMatrix(nRow, OPT_PUTSYMBOL) = ""
                        .TextMatrix(nRow, OPT_PUTCLOSEPRICE) = ""
                        .TextMatrix(nRow, OPT_PUTIMPORTSYMBOL) = ""
                    End If
                    sKey = Format$(op.dExpiry, "YYYYMMDD") + Format$(op.fStrike, "00000.00") + IIf(op.sCallSymbol > "", op.sCallSymbol, op.sPutSymbol)
                    .TextMatrix(nRow, OPT_HIDDENCOLUMN) = sKey
                End If
            Loop
        End If
        
        .Col = OPT_HIDDENCOLUMN
        .Sort = flexSortStringAscending
        
        ' painting grid
        iCurColor1 = COLOR11
        iCurColor2 = COLOR21
        If .Rows > 2 Then dCurMonth = (.TextMatrix(2, OPT_EXPIRATION))
        For nRow = 2 To .Rows - 1
            If dCurMonth <> (.TextMatrix(nRow, OPT_EXPIRATION)) Then
                dCurMonth = (.TextMatrix(nRow, OPT_EXPIRATION))
                If iCurColor1 = COLOR11 Then
                    iCurColor1 = COLOR12
                    iCurColor2 = COLOR22
                Else
                    iCurColor1 = COLOR11
                    iCurColor2 = COLOR21
                End If
            End If
            .Cell(flexcpBackColor, nRow, OPT_CALLSYMBOL, nRow, OPT_CALLCLOSEPRICE) = iCurColor1
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_STRIKE, nRow, OPT_EXPIRYVALUATION) = iCurColor2
            .Cell(flexcpBackColor, nRow, OPT_PUTSYMBOL, nRow, OPT_PUTCLOSEPRICE) = iCurColor1
        Next nRow
        
        If .Rows > 2 Then .Row = 2
        .Redraw = flexRDBuffered
    End With
    
    SetContractButtonsState
    
    Exit Sub
EH:
    grdOptions.Redraw = flexRDBuffered
    ShowError
End Sub


