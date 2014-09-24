VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmOrderEntry 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Order Entry"
   ClientHeight    =   7035
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   9540
   Icon            =   "frmOrderEntry.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7035
   ScaleWidth      =   9540
   StartUpPosition =   1  'CenterOwner
   Begin VB.PictureBox picMain 
      Align           =   1  'Align Top
      BorderStyle     =   0  'None
      Height          =   6585
      Left            =   0
      ScaleHeight     =   6585
      ScaleWidth      =   9540
      TabIndex        =   15
      TabStop         =   0   'False
      Top             =   495
      Width           =   9540
      Begin VB.CommandButton btnCancel 
         Cancel          =   -1  'True
         Caption         =   "Cancel"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   615
         Left            =   6540
         TabIndex        =   13
         Top             =   5760
         Width           =   2805
      End
      Begin VB.Timer tmrTime 
         Enabled         =   0   'False
         Interval        =   1000
         Left            =   5520
         Top             =   180
      End
      Begin VB.Frame fraQtyKeypad 
         Caption         =   " Quantity Keypad "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   2295
         Left            =   6540
         TabIndex        =   49
         Top             =   3300
         Width           =   2805
         Begin VB.CheckBox btnQtyClear 
            Caption         =   "Clear"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00000080&
            Height          =   1875
            Left            =   2100
            Style           =   1  'Graphical
            TabIndex        =   62
            TabStop         =   0   'False
            Top             =   270
            Width           =   585
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   11
            Left            =   1440
            Style           =   1  'Graphical
            TabIndex        =   61
            TabStop         =   0   'False
            Top             =   1710
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "00"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   10
            Left            =   780
            Style           =   1  'Graphical
            TabIndex        =   60
            TabStop         =   0   'False
            Top             =   1710
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "0"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   9
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   59
            TabStop         =   0   'False
            Top             =   1710
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "9"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   8
            Left            =   1440
            Style           =   1  'Graphical
            TabIndex        =   58
            TabStop         =   0   'False
            Top             =   1230
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "8"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   7
            Left            =   780
            Style           =   1  'Graphical
            TabIndex        =   57
            TabStop         =   0   'False
            Top             =   1230
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "7"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   6
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   56
            TabStop         =   0   'False
            Top             =   1230
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "6"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   5
            Left            =   1440
            Style           =   1  'Graphical
            TabIndex        =   55
            TabStop         =   0   'False
            Top             =   750
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "5"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   4
            Left            =   780
            Style           =   1  'Graphical
            TabIndex        =   54
            TabStop         =   0   'False
            Top             =   750
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "4"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   3
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   53
            TabStop         =   0   'False
            Top             =   750
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "3"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   2
            Left            =   1440
            Style           =   1  'Graphical
            TabIndex        =   52
            TabStop         =   0   'False
            Top             =   270
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "2"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   1
            Left            =   780
            Style           =   1  'Graphical
            TabIndex        =   51
            TabStop         =   0   'False
            Top             =   270
            Width           =   615
         End
         Begin VB.CheckBox btnQtyDigit 
            Caption         =   "1"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   435
            Index           =   0
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   50
            TabStop         =   0   'False
            Top             =   270
            Width           =   615
         End
      End
      Begin VB.Frame fraDateTime 
         Caption         =   " Date/Time "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   3000
         TabIndex        =   46
         Top             =   0
         Width           =   3375
         Begin VB.Label lblOrderDate 
            Alignment       =   2  'Center
            BackStyle       =   0  'Transparent
            Caption         =   "12/25/2003 12:55:55 AM"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   12
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Left            =   60
            TabIndex        =   47
            Top             =   390
            Width           =   3255
         End
      End
      Begin VB.Frame fraDest 
         Caption         =   " Destination "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   2700
         TabIndex        =   45
         Top             =   4680
         Width           =   3675
         Begin VB.ComboBox cmbDest 
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   480
            Left            =   120
            Sorted          =   -1  'True
            Style           =   2  'Dropdown List
            TabIndex        =   11
            Top             =   300
            Width           =   3435
         End
      End
      Begin VB.Frame fraPrice 
         Caption         =   " Price "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   180
         TabIndex        =   44
         Top             =   4680
         Width           =   2355
         Begin EtsMMCv.ctlEditEx txtPrice 
            Height          =   480
            Left            =   120
            TabIndex        =   10
            Top             =   300
            Width           =   1875
            _ExtentX        =   3307
            _ExtentY        =   847
            MouseIcon       =   "frmOrderEntry.frx":0442
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            HideSelection   =   -1  'True
         End
         Begin MSComCtl2.UpDown udPrice 
            Height          =   480
            Left            =   1980
            TabIndex        =   64
            TabStop         =   0   'False
            Top             =   300
            Width           =   195
            _ExtentX        =   344
            _ExtentY        =   847
            _Version        =   393216
            BuddyControl    =   "txtPrice"
            BuddyDispid     =   196620
            OrigLeft        =   2010
            OrigTop         =   300
            OrigRight       =   2205
            OrigBottom      =   780
            Wrap            =   -1  'True
            Enabled         =   -1  'True
         End
      End
      Begin VB.Frame fraQty 
         Caption         =   " Quantity "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1335
         Left            =   180
         TabIndex        =   43
         Top             =   3300
         Width           =   2355
         Begin MSComCtl2.UpDown udQty 
            Height          =   480
            Left            =   2010
            TabIndex        =   63
            TabStop         =   0   'False
            Top             =   300
            Width           =   195
            _ExtentX        =   344
            _ExtentY        =   847
            _Version        =   393216
            BuddyControl    =   "txtQty"
            BuddyDispid     =   196622
            OrigLeft        =   2010
            OrigTop         =   300
            OrigRight       =   2205
            OrigBottom      =   780
            Wrap            =   -1  'True
            Enabled         =   -1  'True
         End
         Begin EtsMMCv.ctlEditEx txtQty 
            Height          =   480
            Left            =   120
            TabIndex        =   7
            Top             =   300
            Width           =   2085
            _ExtentX        =   3678
            _ExtentY        =   847
            MouseIcon       =   "frmOrderEntry.frx":045E
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            HideSelection   =   -1  'True
         End
         Begin VB.Label lblLotSize 
            Alignment       =   2  'Center
            BorderStyle     =   1  'Fixed Single
            Caption         =   "Lot Size : 100"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   315
            Left            =   120
            TabIndex        =   48
            Top             =   870
            Width           =   2085
         End
      End
      Begin VB.Frame fraTrader 
         Caption         =   " Trader "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   180
         TabIndex        =   42
         Top             =   0
         Width           =   2655
         Begin VB.ComboBox cmbTrader 
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   480
            Left            =   120
            Sorted          =   -1  'True
            Style           =   2  'Dropdown List
            TabIndex        =   2
            Top             =   300
            Width           =   2415
         End
      End
      Begin VB.Frame fraOption 
         Caption         =   " Option "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   2700
         TabIndex        =   41
         Top             =   2340
         Width           =   3675
         Begin VB.ComboBox cmbOption 
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   480
            Left            =   120
            Style           =   2  'Dropdown List
            TabIndex        =   6
            Top             =   300
            Width           =   3435
         End
      End
      Begin VB.Frame fraStock 
         Caption         =   " Stock "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   915
         Left            =   180
         TabIndex        =   40
         Top             =   2340
         Width           =   2355
         Begin VB.ComboBox cmbStock 
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   480
            Left            =   120
            Sorted          =   -1  'True
            Style           =   2  'Dropdown List
            TabIndex        =   5
            Top             =   300
            Width           =   2085
         End
      End
      Begin VB.Frame fraOrderType 
         Caption         =   " Order Type "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1335
         Left            =   2700
         TabIndex        =   39
         Top             =   3300
         Width           =   3675
         Begin VB.OptionButton optOrderType 
            Caption         =   "Market"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   900
            Index           =   1
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   8
            Top             =   300
            Value           =   -1  'True
            Width           =   1665
         End
         Begin VB.OptionButton optOrderType 
            Caption         =   "Limit"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   900
            Index           =   2
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   9
            Top             =   300
            Width           =   1665
         End
      End
      Begin VB.Frame fraOrderAction 
         Caption         =   " Order Action "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   1335
         Left            =   180
         TabIndex        =   38
         Top             =   960
         Width           =   6195
         Begin VB.OptionButton optOrderAction 
            Caption         =   "SELL"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H000000C0&
            Height          =   870
            Index           =   0
            Left            =   3120
            Style           =   1  'Graphical
            TabIndex        =   4
            Top             =   300
            Width           =   2925
         End
         Begin VB.OptionButton optOrderAction 
            Caption         =   "BUY"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   13.5
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            ForeColor       =   &H00FF0000&
            Height          =   870
            Index           =   1
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   3
            Top             =   300
            Value           =   -1  'True
            Width           =   2925
         End
      End
      Begin VB.Frame fraQuickQty 
         Caption         =   " Quick Quantity "
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   3255
         Index           =   0
         Left            =   6540
         TabIndex        =   16
         Top             =   0
         Width           =   2805
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "50000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   20
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   37
            TabStop         =   0   'False
            Top             =   2790
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "100"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   0
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   36
            TabStop         =   0   'False
            Top             =   270
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "200"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   1
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   35
            TabStop         =   0   'False
            Top             =   270
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "300"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   2
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   34
            TabStop         =   0   'False
            Top             =   270
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "400"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   3
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   33
            TabStop         =   0   'False
            Top             =   690
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "500"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   4
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   32
            TabStop         =   0   'False
            Top             =   690
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "600"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   5
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   31
            TabStop         =   0   'False
            Top             =   690
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "700"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   6
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   30
            TabStop         =   0   'False
            Top             =   1110
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "800"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   7
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   29
            TabStop         =   0   'False
            Top             =   1110
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "900"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   8
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   28
            TabStop         =   0   'False
            Top             =   1110
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "1000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   9
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   27
            TabStop         =   0   'False
            Top             =   1530
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "1500"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   10
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   26
            TabStop         =   0   'False
            Top             =   1530
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "2000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   11
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   25
            TabStop         =   0   'False
            Top             =   1530
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "3000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   12
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   24
            TabStop         =   0   'False
            Top             =   1950
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "4000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   13
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   23
            TabStop         =   0   'False
            Top             =   1950
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "5000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   14
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   22
            TabStop         =   0   'False
            Top             =   1950
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "7500"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   15
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   21
            TabStop         =   0   'False
            Top             =   2370
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "10000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   16
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   20
            TabStop         =   0   'False
            Top             =   2370
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "15000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   17
            Left            =   1860
            Style           =   1  'Graphical
            TabIndex        =   19
            TabStop         =   0   'False
            Top             =   2370
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "20000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   18
            Left            =   120
            Style           =   1  'Graphical
            TabIndex        =   18
            TabStop         =   0   'False
            Top             =   2790
            Width           =   825
         End
         Begin VB.CheckBox btnQuickQty 
            Caption         =   "30000"
            BeginProperty Font 
               Name            =   "MS Sans Serif"
               Size            =   9.75
               Charset         =   204
               Weight          =   700
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Height          =   375
            Index           =   19
            Left            =   990
            Style           =   1  'Graphical
            TabIndex        =   17
            TabStop         =   0   'False
            Top             =   2790
            Width           =   825
         End
      End
      Begin VB.CheckBox btnSendOrder 
         Caption         =   "Send Order"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   13.5
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00008000&
         Height          =   615
         Left            =   180
         Style           =   1  'Graphical
         TabIndex        =   12
         Top             =   5760
         Width           =   6195
      End
   End
   Begin VB.PictureBox picOrderContractType 
      Align           =   1  'Align Top
      BorderStyle     =   0  'None
      Height          =   495
      Left            =   0
      ScaleHeight     =   495
      ScaleWidth      =   9540
      TabIndex        =   14
      TabStop         =   0   'False
      Top             =   0
      Width           =   9540
      Begin VB.OptionButton optOrderContractType 
         Caption         =   "Option Order"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   1
         Left            =   4800
         Style           =   1  'Graphical
         TabIndex        =   1
         Top             =   60
         Width           =   4545
      End
      Begin VB.OptionButton optOrderContractType 
         BackColor       =   &H0000FFFF&
         Caption         =   "Stock Order"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   375
         Index           =   0
         Left            =   180
         Style           =   1  'Graphical
         TabIndex        =   0
         Top             =   60
         Value           =   -1  'True
         Width           =   4545
      End
   End
End
Attribute VB_Name = "frmOrderEntry"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'' defaults
'Private m_nDefTraderID&
'Private m_enDefOrderType As EtsGeneralLib.EtsOrderTypeEnum
'Private m_nDefStockDestID&
'Private m_nDefOptionDestID&
'
'' states & supplementary
'Private m_bIsOpened As Boolean
'Private m_bInitNow As Boolean
'Private m_nOptIdx&
'Private m_nCurQtyValueIdx&
'Private m_nCurPrcValueIdx&
'
'' order datas
'Private m_Opts As EtsGeneralLib.EtsOptColl
'Private m_bIsOptionOrder As Boolean
'Private m_bIsNewOrder As Boolean
'Private m_nStockDestID&
'Private m_nOptionDestID&
'
'Private m_Ord As EtsMmGeneralLib.MmOrderAtom
'
'Public Sub NewStockOrder(ByRef aUnd As EtsGeneralLib.UndAtom, _
'                            ByVal bIsBuy As Boolean, ByVal nQty&, ByVal dPrice#)
'    On Error GoTo EH
'    Screen.MousePointer = vbHourglass
'
'    m_bInitNow = True
'
'    If Not m_bIsOpened Then
'        Load Me
'    End If
'
'    m_bIsNewOrder = True
'    Me.Caption = "New Order"
'    btnSendOrder.Caption = "Send Order"
'
'    Set m_Ord = Nothing
'    Set m_Ord = New EtsMmGeneralLib.MmOrderAtom
'
'    m_Ord.IsBuy = bIsBuy
'    m_Ord.OrderQty = Abs(nQty)
'    If m_Ord.OrderQty = 0 Then m_Ord.OrderQty = 100
'    m_Ord.OrderPrice = IIf(dPrice <= 0#, 0#, dPrice)
'
'    InitNewOrderTrader
'
'    m_Ord.OrderType = m_enDefOrderType
'    m_bIsOptionOrder = False
'
'    lblOrderDate.Caption = Now
'
'    Set m_Ord.Und = aUnd
'    Set m_Ord.Dest = g_OrderExecDest(m_nDefStockDestID)
'
'    If m_Ord.Dest Is Nothing Then Set m_Ord.Dest = g_OrderExecDest(0)
'    If Not m_Ord.Dest.IsStock Then Set m_Ord.Dest = g_OrderExecDest(0)
'
'    m_nStockDestID = m_Ord.Dest.ID
'    m_nOptionDestID = m_nDefOptionDestID
'
'    LoadData
'
'    m_bInitNow = False
'
'    tmrTime.Enabled = True
'    Me.Show vbModeless ', frmMain
'
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to create new order."
'    On Error Resume Next
'    m_bInitNow = False
'    Set m_Ord = Nothing
'    Set m_Opts = Nothing
'    Unload Me
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub NewOptionOrder(ByRef aUnd As EtsGeneralLib.UndAtom, _
'                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
'                        ByVal bIsBuy As Boolean, ByVal nQty&, ByVal dPrice#)
'    On Error GoTo EH
'    Screen.MousePointer = vbHourglass
'
'    m_bInitNow = True
'
'    If Not m_bIsOpened Then
'        Load Me
'    End If
'
'    m_bIsNewOrder = True
'    Me.Caption = "New Order"
'    btnSendOrder.Caption = "Send Order"
'
'    Set m_Ord = Nothing
'    Set m_Ord = New EtsMmGeneralLib.MmOrderAtom
'
'    m_Ord.IsBuy = bIsBuy
'    m_Ord.OrderQty = Abs(nQty)
'    If m_Ord.OrderQty = 0 Then m_Ord.OrderQty = 10
'    m_Ord.OrderPrice = IIf(dPrice <= 0#, 0#, dPrice)
'
'    InitNewOrderTrader
'
'    m_Ord.OrderType = m_enDefOrderType
'    m_bIsOptionOrder = True
'
'    lblOrderDate.Caption = Now
'
'    Set m_Ord.Und = aUnd
'    Set m_Ord.Opt = aOpt
'    If Not m_Ord.Und Is Nothing And Not m_Ord.Opt Is Nothing Then Set m_Ord.OptRoot = m_Ord.Und.Roots(m_Ord.Opt.RootID)
'
'    Set m_Ord.Dest = g_OrderExecDest(m_nDefOptionDestID)
'    If m_Ord.Dest Is Nothing Then Set m_Ord.Dest = g_OrderExecDest(0)
'    If Not m_Ord.Dest.IsOption Then Set m_Ord.Dest = g_OrderExecDest(0)
'
'    m_nStockDestID = m_nDefStockDestID
'    m_nOptionDestID = m_Ord.Dest.ID
'
'    LoadData
'
'    m_bInitNow = False
'
'    tmrTime.Enabled = True
'    Me.Show vbModeless ', frmMain
'
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to create new order."
'    On Error Resume Next
'    m_bInitNow = False
'    Set m_Ord = Nothing
'    Set m_Opts = Nothing
'    Unload Me
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub AlterOrder(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
'    On Error GoTo EH
'    Screen.MousePointer = vbHourglass
'
'    m_bInitNow = True
'
'    If Not m_bIsOpened Then
'        Load Me
'    End If
'
'    m_bIsNewOrder = False
'    Me.Caption = "Cancel/Replace Order #" & aOrd.OrderID
'    btnSendOrder.Caption = "Cancel/Replace Order"
'
'    Set m_Ord = Nothing
'    Set m_Ord = New EtsMmGeneralLib.MmOrderAtom
'
'    InitOrderFrom aOrd
'
'    m_bIsOptionOrder = (aOrd.ContractType = enCtOption)
'    lblOrderDate.Caption = aOrd.OrderDate
'
'    LoadData
'
'    m_bInitNow = False
'
'    tmrTime.Enabled = False
'    Me.Show vbModeless ', frmMain
'
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to alter order."
'    On Error Resume Next
'    m_bInitNow = False
'    Set m_Opts = Nothing
'    Unload Me
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub LoadData()
'    On Error Resume Next
'
'    optOrderContractType(1).Value = m_bIsOptionOrder
'    optOrderContractType(0).Value = Not optOrderContractType(1).Value
'    optOrderContractType(0).Enabled = m_bIsNewOrder
'    optOrderContractType(1).Enabled = m_bIsNewOrder
'
'    optOrderType(1).Value = (m_Ord.OrderType = enOrtMarket)
'    optOrderType(2).Value = Not optOrderType(1).Value
'    optOrderType(1).Enabled = m_bIsNewOrder
'    optOrderType(2).Enabled = m_bIsNewOrder
'
'    optOrderAction(1).Value = m_Ord.IsBuy
'    optOrderAction(0).Value = Not optOrderAction(1).Value
'    optOrderAction(1).Enabled = m_bIsNewOrder
'    optOrderAction(0).Enabled = m_bIsNewOrder
'
'    txtQty.Text = m_Ord.OrderQty
'
'    If m_Ord.OrderType <> enOrtMarket Then
'        txtPrice.Text = m_Ord.OrderPrice
'    Else
'        txtPrice.Text = ""
'    End If
'
'    cmbStock.Clear
'    cmbOption.Clear
'    Set m_Opts = Nothing
'    m_nOptIdx = -1
'
'    FillStocksCombo
'
'    If Not m_Ord.Und Is Nothing Then
'        cmbStock.AddItem m_Ord.Und.Symbol
'        cmbStock.ItemData(cmbStock.NewIndex) = m_Ord.Und.ID
'        cmbStock.ListIndex = cmbStock.NewIndex
'
'        If m_bIsOptionOrder And Not m_Ord.Opt Is Nothing And Not m_Ord.OptRoot Is Nothing Then
'            cmbOption.AddItem m_Ord.Opt.Symbol & " " & Format$(m_Ord.Opt.Expiry, "MMMYY") & " " & _
'                         Trim$(Str$(m_Ord.Opt.Strike)) & " " & IIf(m_Ord.Opt.OptType = enOtCall, "C", "P")
'
'            cmbOption.ItemData(cmbOption.NewIndex) = m_Ord.Opt.ID
'            cmbOption.ListIndex = cmbOption.NewIndex
'            m_nOptIdx = cmbOption.ListIndex
'        Else
'            Set m_Ord.Opt = Nothing
'            Set m_Ord.OptRoot = Nothing
'            Set m_Opts = Nothing
'        End If
'    Else
'        Set m_Ord.Opt = Nothing
'        Set m_Ord.OptRoot = Nothing
'        Set m_Opts = Nothing
'    End If
'
'    cmbStock.Enabled = m_bIsNewOrder
'    cmbTrader.Enabled = m_bIsNewOrder
'    cmbDest.Enabled = m_bIsNewOrder
'
'    LoadTraders
'
'    AdjustContractTypeControls
'    AdjustLotSize
'    AdjustOrderTypeControls
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub LoadTraders()
'    On Error Resume Next
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'
'    cmbTrader.Clear
'
'    For Each aTrader In g_Trader
'        cmbTrader.AddItem aTrader.Name
'        cmbTrader.ItemData(cmbTrader.NewIndex) = aTrader.ID
'
'        If Not m_Ord.Trader Is Nothing Then
'            If aTrader.ID = m_Ord.Trader.ID Then _
'                cmbTrader.ListIndex = cmbTrader.NewIndex
'        End If
'    Next
'End Sub
'
'Private Sub LoadDests()
'    On Error Resume Next
'    Dim aDest As EtsMmGeneralLib.MmOrderExecDestAtom
'
'    cmbDest.Clear
'
'    For Each aDest In g_OrderExecDest
'        If Not m_bIsOptionOrder And aDest.IsStock _
'            Or m_bIsOptionOrder And aDest.IsOption Then
'
'            cmbDest.AddItem aDest.Name
'            cmbDest.ItemData(cmbDest.NewIndex) = aDest.ID
'
'            If Not m_Ord.Dest Is Nothing Then
'                If aDest.ID = m_Ord.Dest.ID Then _
'                    cmbDest.ListIndex = cmbDest.NewIndex
'            End If
'        End If
'    Next
'End Sub
'
'Private Function SendOrder() As Boolean
'    On Error Resume Next
'    Dim bSent As Boolean
'
'    bSent = False
'    If Not IsOrderSendAvailable Then Exit Function
'
'    If m_Ord.OrderType = enOrtMarket Then m_Ord.OrderPrice = 0#
'
'    On Error GoTo EH
'
'    If m_bIsNewOrder Then
'        If Not m_bIsOptionOrder Then
'            Set m_Ord.Opt = Nothing
'            Set m_Ord.OptRoot = Nothing
'        End If
'
'        g_OrdersProcessor.OrderNew m_Ord
'        bSent = True
'    Else
'        If gCmn.MyMsgBox(Me, "Are you sure to cancel/replace order #" & _
'                            m_Ord.OrderID & " for " & m_Ord.Symbol & "?", _
'                            vbYesNo + vbQuestion) = vbYes Then
'
'            If Not m_bIsOptionOrder Then
'                Set m_Ord.Opt = Nothing
'                Set m_Ord.OptRoot = Nothing
'            End If
'
'            g_OrdersProcessor.OrderAlternate m_Ord
'            bSent = True
'        End If
'    End If
'
'    If bSent Then
'        m_nDefTraderID = m_Ord.Trader.ID
'        m_enDefOrderType = m_Ord.OrderType
'        m_nDefStockDestID = m_nStockDestID
'        m_nDefOptionDestID = m_nOptionDestID
'    End If
'
'    SendOrder = bSent
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox Me, "Fail to send order: " & Err.Description
'    SendOrder = False
'End Function
'
'Private Sub AdjustLotSize()
'    On Error Resume Next
'    Dim sLotSize$: sLotSize = "N/A"
'
'    If m_bIsOptionOrder And Not m_Ord.Opt Is Nothing Then
'        If Not m_Ord.OptRoot Is Nothing Then
'            sLotSize = CStr(m_Ord.OptRoot.LotSize)
'        End If
'
'    ElseIf Not m_bIsOptionOrder And Not m_Ord.Und Is Nothing Then
'        sLotSize = "1"
'    End If
'
'    lblLotSize.Caption = "Lot Size : " & sLotSize
'End Sub
'
'Private Sub cmbOption_Click()
'    On Error Resume Next
'    If m_bInitNow Or Not m_bIsOptionOrder Then Exit Sub
'
'    Dim nOptID&
'
'    If cmbOption.ListIndex > -1 And Not m_Opts Is Nothing Then
'        nOptID = cmbOption.ItemData(cmbOption.ListIndex)
'
'        If Not m_Ord.Opt Is Nothing Then
'            If nOptID = m_Ord.Opt.ID Then Exit Sub
'        End If
'
'        Set m_Ord.Opt = m_Opts(nOptID)
'        Set m_Ord.OptRoot = m_Ord.Und.Roots(m_Ord.Opt.RootID)
'
'        m_nOptIdx = cmbOption.ListIndex
'    Else
'        Set m_Ord.Opt = Nothing
'        Set m_Ord.OptRoot = Nothing
'        m_nOptIdx = -1
'    End If
'
'    AdjustLotSize
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub cmbOption_DropDown()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim rs As ADODB.Recordset, aOpt As EtsGeneralLib.EtsOptAtom, nOptID&, nIdx&
'
'    If Not m_Ord.Und Is Nothing Then
'        If m_Opts Is Nothing Then
'            Screen.MousePointer = vbHourglass
'            cmbOption.Clear
'            m_nOptIdx = -1
'
'            Set m_Opts = New EtsGeneralLib.EtsOptColl
'            Set rs = gDBW.usp_UnderlyingOptionsForOrder_Get(m_Ord.Und.ID)
'
'            While Not rs.EOF
'                nOptID = ReadLng(rs!iOptionID)
'                If m_Opts(nOptID) Is Nothing Then
'                    Set aOpt = m_Opts.Add(nOptID, Trim$(Str$(nIdx)))
'                    nIdx = nIdx + 1
'
'                    aOpt.ID = nOptID
'                    aOpt.RootID = ReadLng(rs!iOptionRootID)
'                    aOpt.Symbol = ReadStr(rs!vcSymbol)
'                    aOpt.OptType = IIf(ReadByte(rs!tiIsCall) <> 0, enOtCall, enOtPut)
'                    aOpt.Expiry = ReadDate(rs!dtExpiry)
'                    aOpt.Strike = ReadDbl(rs!fStrike)
'
'                    cmbOption.AddItem aOpt.Symbol & " " & Format$(aOpt.Expiry, "MMMYY") & " " & _
'                                 Trim$(Str$(aOpt.Strike)) & " " & IIf(aOpt.OptType = enOtCall, "C", "P")
'
'                    cmbOption.ItemData(cmbOption.NewIndex) = nOptID
'                    If Not m_Ord.Opt Is Nothing Then
'                        If m_Ord.Opt.ID = nOptID Then
'                            m_bInitNow = True
'                            cmbOption.ListIndex = cmbOption.NewIndex
'                            m_nOptIdx = cmbOption.ListIndex
'                            m_bInitNow = False
'                        End If
'                    End If
'
'                    Set aOpt = Nothing
'                End If
'                rs.MoveNext
'            Wend
'
'            Set rs = Nothing
'            Screen.MousePointer = vbDefault
'        End If
'    Else
'        m_nOptIdx = -1
'        Set m_Ord.Opt = Nothing
'        Set m_Opts = Nothing
'        Set m_Ord.OptRoot = Nothing
'        cmbOption.Clear
'    End If
'End Sub
'
'Private Sub FillStocksCombo()
'    On Error Resume Next
'    Dim aUnd As EtsGeneralLib.UndAtom, nUndID&
'
'    Screen.MousePointer = vbHourglass
'    If Not m_Ord.Und Is Nothing Then nUndID = m_Ord.Und.ID
'
'    cmbStock.Clear
'
'    For Each aUnd In g_Underlying
'        cmbStock.AddItem aUnd.Symbol
'        cmbStock.ItemData(cmbStock.NewIndex) = aUnd.ID
'        If aUnd.ID = nUndID Then
'            If m_bInitNow Then
'                cmbStock.ListIndex = cmbStock.NewIndex
'            Else
'                m_bInitNow = True
'                cmbStock.ListIndex = cmbStock.NewIndex
'                m_bInitNow = False
'            End If
'        End If
'    Next
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub cmbStock_DropDown()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    If cmbStock.ListCount <> g_Underlying.Count Then
'        FillStocksCombo
'    End If
'End Sub
'
'Private Sub cmbStock_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim nUndID&
'
'    If cmbStock.ListIndex > -1 Then
'        nUndID = cmbStock.ItemData(cmbStock.ListIndex)
'        If Not m_Ord.Und Is Nothing Then
'            If m_Ord.Und.ID = nUndID Then Exit Sub
'        End If
'
'        Set m_Ord.Und = g_Underlying(nUndID)
'    Else
'        Set m_Ord.Und = Nothing
'    End If
'
'    Set m_Ord.Opt = Nothing
'    Set m_Opts = Nothing
'    Set m_Ord.OptRoot = Nothing
'    cmbOption.Clear
'    m_nOptIdx = -1
'
'    AdjustLotSize
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub cmbTrader_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim nTraderID&
'
'    If cmbTrader.ListIndex > -1 Then
'        nTraderID = cmbTrader.ItemData(cmbTrader.ListIndex)
'
'        If Not m_Ord.Trader Is Nothing Then
'            If m_Ord.Trader.ID = nTraderID Then Exit Sub
'        End If
'
'        Set m_Ord.Trader = g_Trader(nTraderID)
'    Else
'        Set m_Ord.Trader = Nothing
'    End If
'
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub cmbDest_Click()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    Dim nDestID&
'
'    If cmbDest.ListIndex > -1 Then
'        nDestID = cmbDest.ItemData(cmbDest.ListIndex)
'
'        If Not m_Ord.Dest Is Nothing Then
'            If m_Ord.Dest.ID = nDestID Then Exit Sub
'        End If
'
'        Set m_Ord.Dest = g_OrderExecDest(nDestID)
'
'        If m_bIsOptionOrder Then
'            m_nOptionDestID = m_Ord.Dest.ID
'        Else
'            m_nStockDestID = m_Ord.Dest.ID
'        End If
'    Else
'        Set m_Ord.Dest = Nothing
'        m_nOptionDestID = m_nDefOptionDestID
'        m_nStockDestID = m_nDefStockDestID
'    End If
'
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    m_bIsOpened = True
'    btnQtyClear.Caption = "C" & vbCrLf & "L" & vbCrLf & "E" & vbCrLf & "A" & vbCrLf & "R"
'    m_nCurPrcValueIdx = udPrice.Value
'    m_nCurQtyValueIdx = udQty.Value
'
'    udQty.Left = 2205 - udQty.Width
'    txtQty.Width = udQty.Left - txtQty.Left
'
'    udPrice.Left = 2205 - udPrice.Width
'    txtPrice.Width = udPrice.Left - txtPrice.Left
'
'    LoadSettings
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    m_bIsOpened = False
'    m_bInitNow = False
'
'    Set m_Ord = Nothing
'    Set m_Opts = Nothing
'
'    SaveSettings
'End Sub
'
'Private Sub btnQtyClear_Click()
'    On Error Resume Next
'    If btnQtyClear.Value = 0 Then Exit Sub
'    If m_bInitNow Then Exit Sub
'
'    btnQtyClear.Value = 0
'    txtQty.Text = "0"
'
'    m_Ord.OrderQty = 0
'
'    EditExSelectAllAndSetFocus txtQty
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub btnQtyDigit_Click(Index As Integer)
'    On Error Resume Next
'    If btnQtyDigit(Index).Value = 0 Then Exit Sub
'    If m_bInitNow Then Exit Sub
'
'    btnQtyDigit(Index).Value = 0
'    If txtQty.Text <> "0" Then
'        txtQty.Text = txtQty.Text & btnQtyDigit(Index).Caption
'    Else
'        txtQty.Text = btnQtyDigit(Index).Caption
'    End If
'
'    Dim nOrderQty&
'
'    nOrderQty = CLng(txtQty.Text)
'    m_Ord.OrderQty = nOrderQty
'
'    EditExSelectAllAndSetFocus txtQty
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub btnQuickQty_Click(Index As Integer)
'    On Error Resume Next
'    If btnQuickQty(Index).Value = 0 Then Exit Sub
'    If m_bInitNow Then Exit Sub
'
'    btnQuickQty(Index).Value = 0
'    txtQty.Text = btnQuickQty(Index).Caption
'
'    Dim nOrderQty&
'
'    nOrderQty = CLng(txtQty.Text)
'    m_Ord.OrderQty = nOrderQty
'
'    EditExSelectAllAndSetFocus txtQty
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub btnSendOrder_Click()
'    On Error Resume Next
'    If btnSendOrder.Value = 0 Then Exit Sub
'
'    btnSendOrder.Value = 0
'    If m_bInitNow Then Exit Sub
'
'    If SendOrder() Then Unload Me
'End Sub
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
''    If btnCancel.Value = 0 Then Exit Sub
''
''    btnCancel.Value = 0
'    If m_bInitNow Then Exit Sub
'
'    Unload Me
'End Sub
'
'Private Sub optOrderAction_Click(Index As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Ord.IsBuy = optOrderAction(1).Value
'End Sub
'
'Private Sub optOrderContractType_Click(Index As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_bIsOptionOrder = optOrderContractType(1).Value
'
'    AdjustContractTypeControls
'    AdjustLotSize
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub AdjustContractTypeControls()
'    On Error Resume Next
'    Dim i&
'    If Not m_bIsOptionOrder Then
'        ' stock order
'        For i = 6 To 20
'            btnQuickQty(i).Visible = True
'        Next
'
'        btnQuickQty(0).Caption = "100"
'        btnQuickQty(1).Caption = "200"
'        btnQuickQty(2).Caption = "300"
'        btnQuickQty(3).Caption = "400"
'        btnQuickQty(4).Caption = "500"
'        btnQuickQty(5).Caption = "600"
'
'        fraOption.Enabled = False
'        cmbOption.Enabled = False
'        cmbOption.ForeColor = &H80000011
'        cmbOption.BackColor = &H8000000F
'
'        optOrderContractType(0).BackColor = &HFFFF&
'        optOrderContractType(1).BackColor = &H8000000F
'
'        If Not m_Ord.Opt Is Nothing Then
'            m_nOptIdx = cmbOption.ListIndex
'
'            m_bInitNow = True
'            cmbOption.ListIndex = -1
'            m_bInitNow = False
'        End If
'
'        Set m_Ord.Dest = g_OrderExecDest(m_nStockDestID)
'
'        If m_Ord.Dest Is Nothing Then Set m_Ord.Dest = g_OrderExecDest(0)
'        If Not m_Ord.Dest.IsStock Then Set m_Ord.Dest = g_OrderExecDest(0)
'
'        m_nStockDestID = m_Ord.Dest.ID
'    Else
'        ' option order
'        For i = 6 To 20
'            btnQuickQty(i).Visible = False
'        Next
'
'        btnQuickQty(0).Caption = "5"
'        btnQuickQty(1).Caption = "10"
'        btnQuickQty(2).Caption = "20"
'        btnQuickQty(3).Caption = "30"
'        btnQuickQty(4).Caption = "40"
'        btnQuickQty(5).Caption = "50"
'
'        fraOption.Enabled = True
'        cmbOption.Enabled = m_bIsNewOrder
'        cmbOption.ForeColor = &H80000008
'        cmbOption.BackColor = &H80000005
'
'        optOrderContractType(0).BackColor = &H8000000F
'        optOrderContractType(1).BackColor = &HFFFF&
'
'        If Not m_Ord.Opt Is Nothing And m_nOptIdx >= 0 Then
'            m_bInitNow = True
'            cmbOption.ListIndex = m_nOptIdx
'            m_bInitNow = False
'        End If
'
'        Set m_Ord.Dest = g_OrderExecDest(m_nOptionDestID)
'
'        If m_Ord.Dest Is Nothing Then Set m_Ord.Dest = g_OrderExecDest(0)
'        If Not m_Ord.Dest.IsOption Then Set m_Ord.Dest = g_OrderExecDest(0)
'
'        m_nOptionDestID = m_Ord.Dest.ID
'    End If
'
'    m_bInitNow = True
'    LoadDests
'    m_bInitNow = False
'End Sub
'
'Private Sub AdjustOrderTypeControls()
'    On Error Resume Next
'    If m_Ord.OrderType = enOrtMarket Then
'        ' market order
'        fraPrice.Enabled = False
'        txtPrice.Enabled = False
'        udPrice.Enabled = False
'        txtPrice.ForeColor = &H80000011
'        txtPrice.BackColor = &H8000000F
'
'        txtPrice.Text = ""
'    Else
'        ' limit order
'        fraPrice.Enabled = True
'        txtPrice.Enabled = True
'        udPrice.Enabled = True
'        txtPrice.ForeColor = &H80000008
'        txtPrice.BackColor = &H80000005
'
'        txtPrice.Text = m_Ord.OrderPrice
'    End If
'End Sub
'
'Private Sub optOrderType_Click(Index As Integer)
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Ord.OrderType = IIf(optOrderType(2).Value, enOrtLimit, enOrtMarket)
'
'    AdjustOrderTypeControls
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub AdjustOrderSendAvailability()
'    On Error Resume Next
'    btnSendOrder.Enabled = IsOrderSendAvailable
'End Sub
'
'Private Function IsOrderSendAvailable() As Boolean
'    On Error Resume Next
'    Dim bCurDataValid As Boolean, bAllDataValid As Boolean
'
'    bAllDataValid = False
'    bCurDataValid = False
'
'    ' stock
'    If Not m_Ord.Und Is Nothing Then _
'        bCurDataValid = (m_bIsOptionOrder Or Not m_bIsOptionOrder And (m_Ord.Und.UndType <> enCtIndex))
'
'    fraStock.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bCurDataValid
'
'    ' option
'    bCurDataValid = (Not m_bIsOptionOrder _
'                        Or m_bIsOptionOrder And (Not m_Ord.Opt Is Nothing And Not m_Ord.OptRoot Is Nothing))
'
'    fraOption.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' qty
'    bCurDataValid = (m_Ord.OrderQty > 0)
'    fraQty.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' price
'    bCurDataValid = (m_Ord.OrderType = enOrtMarket _
'                        Or m_Ord.OrderType = enOrtLimit And m_Ord.OrderPrice > DBL_EPSILON)
'
'    fraPrice.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' destination
'    bCurDataValid = (Not m_Ord.Dest Is Nothing)
'    fraDest.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    ' trader
'    bCurDataValid = (Not m_Ord.Trader Is Nothing)
'    fraTrader.ForeColor = IIf(bCurDataValid, &H80000012, &HC0&)
'    bAllDataValid = bAllDataValid And bCurDataValid
'
'    IsOrderSendAvailable = bAllDataValid
'End Function
'
'Private Sub tmrTime_Timer()
'    On Error Resume Next
'    lblOrderDate.Caption = Now
'End Sub
'
'Private Sub LoadSettings()
'    On Error Resume Next
'    If g_CurTraderID = 0 Then
'        m_nDefTraderID = GetLongKeyValueEx(APP_REG_KEY & "Order Entry", "TraderID")
'    Else
'        m_nDefTraderID = g_CurTraderID
'    End If
'
'    m_enDefOrderType = GetLongKeyValueEx(APP_REG_KEY & "Order Entry", "OrderType", enOrtMarket)
'    If m_enDefOrderType <> enOrtMarket And m_enDefOrderType <> enOrtLimit Then m_enDefOrderType = enOrtMarket
'
'    m_nDefStockDestID = GetLongKeyValueEx(APP_REG_KEY & "Order Entry", "StockDestID")
'    m_nDefOptionDestID = GetLongKeyValueEx(APP_REG_KEY & "Order Entry", "OptionDestID")
'End Sub
'
'Private Sub SaveSettings()
'    On Error Resume Next
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Order Entry", "TraderID", m_nDefTraderID
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Order Entry", "OrderType", m_enDefOrderType
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Order Entry", "StockDestID", m_nDefStockDestID
'    SetLongKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Order Entry", "OptionDestID", m_nDefOptionDestID
'End Sub
'
'Private Sub txtPrice_Change()
'    On Error Resume Next
'    If m_bInitNow Or m_Ord.OrderType = enOrtMarket Then Exit Sub
'
'    m_Ord.OrderPrice = 0#
'    m_Ord.OrderPrice = CDbl(txtPrice.Text)
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub udPrice_Change()
'    On Error Resume Next
'    If m_bInitNow Or m_Ord.OrderType = enOrtMarket Then Exit Sub
'    Dim dIncrement#
'
'    If m_bIsOptionOrder Then
'        If m_Ord.OrderPrice < 3# Then
'            dIncrement = g_Params.OrdDefOptPrcIncrementCheap
'        Else
'            dIncrement = g_Params.OrdDefOptPrcIncrementExpensive
'        End If
'    Else
'        dIncrement = g_Params.OrdDefStkPrcIncrement
'    End If
'
'    If udPrice.Value = m_nCurPrcValueIdx + 1 _
'        Or udPrice.Value = udPrice.Min And m_nCurPrcValueIdx = udPrice.Max Then
'
'        m_Ord.OrderPrice = m_Ord.OrderPrice + dIncrement
'    Else
'        If m_Ord.OrderPrice - dIncrement > 0 Then
'            m_Ord.OrderPrice = m_Ord.OrderPrice - dIncrement
'        End If
'    End If
'
'    m_nCurPrcValueIdx = udPrice.Value
'
'    txtPrice.Text = m_Ord.OrderPrice
'    EditExSelectAllAndSetFocus txtPrice
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub txtQty_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    m_Ord.OrderQty = 0&
'    m_Ord.OrderQty = CLng(txtQty.Text)
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub udQty_Change()
'    On Error Resume Next
'    If m_bInitNow Then Exit Sub
'
'    If udQty.Value = m_nCurQtyValueIdx + 1 _
'        Or udQty.Value = udQty.Min And m_nCurQtyValueIdx = udQty.Max Then
'
'        If m_bIsOptionOrder Then
'            m_Ord.OrderQty = m_Ord.OrderQty + g_Params.OrdDefOptQtyIncrement
'        Else
'            m_Ord.OrderQty = m_Ord.OrderQty + g_Params.OrdDefStkQtyIncrement
'        End If
'    Else
'        If m_bIsOptionOrder Then
'            If m_Ord.OrderQty - g_Params.OrdDefOptQtyIncrement > 0 Then _
'                m_Ord.OrderQty = m_Ord.OrderQty - g_Params.OrdDefOptQtyIncrement
'        Else
'            If m_Ord.OrderQty - g_Params.OrdDefStkQtyIncrement > 0 Then _
'                m_Ord.OrderQty = m_Ord.OrderQty - g_Params.OrdDefStkQtyIncrement
'        End If
'    End If
'
'    m_nCurQtyValueIdx = udQty.Value
'
'    txtQty.Text = m_Ord.OrderQty
'    EditExSelectAllAndSetFocus txtQty
'    AdjustOrderSendAvailability
'End Sub
'
'Private Sub InitOrderFrom(ByRef aSrc As EtsMmGeneralLib.MmOrderAtom)
'    On Error Resume Next
'    m_Ord.ID = aSrc.ID
'    m_Ord.SeqNum = aSrc.SeqNum
'    m_Ord.UpdateDate = aSrc.UpdateDate
'    Set m_Ord.Trader = aSrc.Trader
'    Set m_Ord.Dest = aSrc.Dest
'    m_Ord.Status = aSrc.Status
'    m_Ord.OrderType = aSrc.OrderType
'    m_Ord.IsBuy = aSrc.IsBuy
'    m_Ord.OrderQty = aSrc.OrderQty
'    m_Ord.OrderPrice = aSrc.OrderPrice
'    m_Ord.OrderDate = aSrc.OrderDate
'    m_Ord.OrderID = aSrc.OrderID
'    m_Ord.AvgPrice = aSrc.AvgPrice
'    m_Ord.CumQty = aSrc.CumQty
'
'    Set m_Ord.Und = aSrc.Und
'
'    If Not aSrc.Opt Is Nothing And Not aSrc.OptRoot Is Nothing Then
'        Set m_Ord.OptRoot = aSrc.OptRoot
'
'        Set m_Ord.Opt = New EtsGeneralLib.EtsOptAtom
'        m_Ord.Opt.ID = aSrc.Opt.ID
'        m_Ord.Opt.Symbol = aSrc.Opt.Symbol
'        m_Ord.Opt.RootID = aSrc.Opt.RootID
'        m_Ord.Opt.Expiry = aSrc.Opt.Expiry
'        m_Ord.Opt.Strike = aSrc.Opt.Strike
'        m_Ord.Opt.OptType = aSrc.Opt.OptType
'
'        m_Ord.Opt.Series = aSrc.Opt.Series
'    Else
'        Set m_Ord.OptRoot = Nothing
'        Set m_Ord.Opt = Nothing
'    End If
'
'End Sub
'
'Private Sub InitNewOrderTrader()
'    On Error Resume Next
'    If g_CurTraderID <> 0 Then
'        ' take looged in trader
'        Set m_Ord.Trader = g_Trader(g_CurTraderID)
'    Else
'        ' take last order trader
'        Set m_Ord.Trader = g_Trader(m_nDefTraderID)
'        If m_Ord.Trader Is Nothing Then
'            ' take first trader
'            Dim aTrader As EtsGeneralLib.EtsTraderAtom
'            For Each aTrader In g_Trader
'                Set m_Ord.Trader = aTrader
'                Exit For
'            Next
'        End If
'    End If
'End Sub
'
