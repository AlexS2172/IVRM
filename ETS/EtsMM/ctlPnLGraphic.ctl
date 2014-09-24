VERSION 5.00
Object = "{54850C51-14EA-4470-A5E4-8C5DB32DC853}#1.0#0"; "vsprint8.ocx"
Object = "{C115893A-A3BF-43AF-B28D-69DB846077F3}#1.0#0"; "vsflex8u.ocx"
Object = "{0AFE7BE0-11B7-4A3E-978D-D4501E9A57FE}#1.0#0"; "c1Sizer.ocx"
Object = "{0BE3824E-5AFE-4B11-A6BC-4B3AD564982A}#8.0#0"; "olch2x8.ocx"
Begin VB.UserControl ctlPnLGraphic 
   ClientHeight    =   9705
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   13440
   ScaleHeight     =   9705
   ScaleWidth      =   13440
   Begin C1SizerLibCtl.C1Elastic szMain 
      Height          =   9705
      Left            =   0
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   0
      Width           =   13440
      _cx             =   23707
      _cy             =   17119
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Enabled         =   -1  'True
      Appearance      =   4
      MousePointer    =   0
      Version         =   801
      BackColor       =   0
      ForeColor       =   -2147483630
      FloodColor      =   6553600
      ForeColorDisabled=   -2147483631
      Caption         =   ""
      Align           =   0
      AutoSizeChildren=   4
      BorderWidth     =   2
      ChildSpacing    =   1
      Splitter        =   -1  'True
      FloodDirection  =   0
      FloodPercent    =   0
      CaptionPos      =   1
      WordWrap        =   -1  'True
      MaxChildSize    =   0
      MinChildSize    =   0
      TagWidth        =   0
      TagPosition     =   0
      Style           =   0
      TagSplit        =   2
      PicturePos      =   4
      CaptionStyle    =   0
      ResizeFonts     =   0   'False
      GridRows        =   0
      GridCols        =   0
      Frame           =   3
      FrameStyle      =   0
      FrameWidth      =   1
      FrameColor      =   -2147483628
      FrameShadow     =   -2147483632
      FloodStyle      =   1
      _GridInfo       =   ""
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   9
      Begin C1SizerLibCtl.C1Elastic szBottom 
         Height          =   4605
         Left            =   30
         TabIndex        =   6
         TabStop         =   0   'False
         Top             =   5070
         Width           =   13380
         _cx             =   23601
         _cy             =   8123
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Enabled         =   -1  'True
         Appearance      =   4
         MousePointer    =   0
         Version         =   801
         BackColor       =   0
         ForeColor       =   -2147483630
         FloodColor      =   6553600
         ForeColorDisabled=   -2147483631
         Caption         =   ""
         Align           =   0
         AutoSizeChildren=   4
         BorderWidth     =   0
         ChildSpacing    =   1
         Splitter        =   -1  'True
         FloodDirection  =   0
         FloodPercent    =   0
         CaptionPos      =   1
         WordWrap        =   -1  'True
         MaxChildSize    =   0
         MinChildSize    =   0
         TagWidth        =   0
         TagPosition     =   0
         Style           =   0
         TagSplit        =   2
         PicturePos      =   4
         CaptionStyle    =   0
         ResizeFonts     =   0   'False
         GridRows        =   0
         GridCols        =   0
         Frame           =   3
         FrameStyle      =   0
         FrameWidth      =   1
         FrameColor      =   -2147483628
         FrameShadow     =   -2147483632
         FloodStyle      =   1
         _GridInfo       =   ""
         AccessibleName  =   ""
         AccessibleDescription=   ""
         AccessibleValue =   ""
         AccessibleRole  =   9
         Begin C1SizerLibCtl.C1Elastic szVal 
            Height          =   3495
            Left            =   0
            TabIndex        =   24
            TabStop         =   0   'False
            Top             =   1110
            Width           =   13380
            _cx             =   23601
            _cy             =   6165
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Enabled         =   -1  'True
            Appearance      =   4
            MousePointer    =   0
            Version         =   801
            BackColor       =   -2147483633
            ForeColor       =   -2147483630
            FloodColor      =   6553600
            ForeColorDisabled=   -2147483631
            Caption         =   ""
            Align           =   0
            AutoSizeChildren=   1
            BorderWidth     =   6
            ChildSpacing    =   4
            Splitter        =   0   'False
            FloodDirection  =   0
            FloodPercent    =   0
            CaptionPos      =   1
            WordWrap        =   -1  'True
            MaxChildSize    =   0
            MinChildSize    =   0
            TagWidth        =   0
            TagPosition     =   0
            Style           =   0
            TagSplit        =   2
            PicturePos      =   4
            CaptionStyle    =   0
            ResizeFonts     =   0   'False
            GridRows        =   0
            GridCols        =   0
            Frame           =   3
            FrameStyle      =   0
            FrameWidth      =   1
            FrameColor      =   -2147483628
            FrameShadow     =   -2147483632
            FloodStyle      =   1
            _GridInfo       =   ""
            AccessibleName  =   ""
            AccessibleDescription=   ""
            AccessibleValue =   ""
            AccessibleRole  =   9
            Begin VSFlex8UCtl.VSFlexGrid fgVal 
               Height          =   3315
               Left            =   90
               TabIndex        =   26
               Top             =   90
               Width           =   13200
               _cx             =   23283
               _cy             =   5847
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
               BackColorBkg    =   -2147483633
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
         Begin C1SizerLibCtl.C1Elastic szOpt 
            Height          =   1095
            Left            =   0
            TabIndex        =   23
            TabStop         =   0   'False
            Top             =   0
            Width           =   13380
            _cx             =   23601
            _cy             =   1931
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Enabled         =   -1  'True
            Appearance      =   4
            MousePointer    =   0
            Version         =   801
            BackColor       =   -2147483633
            ForeColor       =   -2147483630
            FloodColor      =   6553600
            ForeColorDisabled=   -2147483631
            Caption         =   ""
            Align           =   0
            AutoSizeChildren=   1
            BorderWidth     =   6
            ChildSpacing    =   4
            Splitter        =   0   'False
            FloodDirection  =   0
            FloodPercent    =   0
            CaptionPos      =   1
            WordWrap        =   -1  'True
            MaxChildSize    =   0
            MinChildSize    =   0
            TagWidth        =   0
            TagPosition     =   0
            Style           =   0
            TagSplit        =   2
            PicturePos      =   4
            CaptionStyle    =   0
            ResizeFonts     =   0   'False
            GridRows        =   0
            GridCols        =   0
            Frame           =   3
            FrameStyle      =   0
            FrameWidth      =   1
            FrameColor      =   -2147483628
            FrameShadow     =   -2147483632
            FloodStyle      =   1
            _GridInfo       =   ""
            AccessibleName  =   ""
            AccessibleDescription=   ""
            AccessibleValue =   ""
            AccessibleRole  =   9
            Begin VSFlex8UCtl.VSFlexGrid fgOpt 
               Height          =   915
               Left            =   90
               TabIndex        =   25
               Top             =   90
               Width           =   13200
               _cx             =   23283
               _cy             =   1614
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
               BackColorBkg    =   -2147483633
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
               Rows            =   50
               Cols            =   10
               FixedRows       =   1
               FixedCols       =   1
               RowHeightMin    =   0
               RowHeightMax    =   0
               ColWidthMin     =   0
               ColWidthMax     =   0
               ExtendLastCol   =   -1  'True
               FormatString    =   ""
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
      End
      Begin C1SizerLibCtl.C1Elastic szTop 
         Height          =   5025
         Left            =   30
         TabIndex        =   5
         TabStop         =   0   'False
         Top             =   30
         Width           =   13380
         _cx             =   23601
         _cy             =   8864
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Enabled         =   -1  'True
         Appearance      =   4
         MousePointer    =   0
         Version         =   801
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         FloodColor      =   6553600
         ForeColorDisabled=   -2147483631
         Caption         =   ""
         Align           =   0
         AutoSizeChildren=   4
         BorderWidth     =   1
         ChildSpacing    =   0
         Splitter        =   0   'False
         FloodDirection  =   0
         FloodPercent    =   0
         CaptionPos      =   1
         WordWrap        =   -1  'True
         MaxChildSize    =   0
         MinChildSize    =   0
         TagWidth        =   0
         TagPosition     =   0
         Style           =   0
         TagSplit        =   2
         PicturePos      =   4
         CaptionStyle    =   0
         ResizeFonts     =   0   'False
         GridRows        =   0
         GridCols        =   0
         Frame           =   3
         FrameStyle      =   0
         FrameWidth      =   1
         FrameColor      =   -2147483628
         FrameShadow     =   -2147483632
         FloodStyle      =   1
         _GridInfo       =   ""
         AccessibleName  =   ""
         AccessibleDescription=   ""
         AccessibleValue =   ""
         AccessibleRole  =   9
         Begin C1SizerLibCtl.C1Elastic szTop1 
            Height          =   4995
            Left            =   15
            TabIndex        =   7
            TabStop         =   0   'False
            Top             =   15
            Width           =   13350
            _cx             =   23548
            _cy             =   8811
            BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
               Name            =   "MS Sans Serif"
               Size            =   8.25
               Charset         =   204
               Weight          =   400
               Underline       =   0   'False
               Italic          =   0   'False
               Strikethrough   =   0   'False
            EndProperty
            Enabled         =   -1  'True
            Appearance      =   2
            MousePointer    =   0
            Version         =   801
            BackColor       =   0
            ForeColor       =   -2147483630
            FloodColor      =   6553600
            ForeColorDisabled=   -2147483631
            Caption         =   ""
            Align           =   0
            AutoSizeChildren=   2
            BorderWidth     =   0
            ChildSpacing    =   1
            Splitter        =   -1  'True
            FloodDirection  =   0
            FloodPercent    =   0
            CaptionPos      =   1
            WordWrap        =   -1  'True
            MaxChildSize    =   0
            MinChildSize    =   0
            TagWidth        =   0
            TagPosition     =   0
            Style           =   0
            TagSplit        =   2
            PicturePos      =   4
            CaptionStyle    =   0
            ResizeFonts     =   0   'False
            GridRows        =   0
            GridCols        =   0
            Frame           =   3
            FrameStyle      =   0
            FrameWidth      =   1
            FrameColor      =   -2147483628
            FrameShadow     =   -2147483632
            FloodStyle      =   1
            _GridInfo       =   ""
            AccessibleName  =   ""
            AccessibleDescription=   ""
            AccessibleValue =   ""
            AccessibleRole  =   9
            Begin C1SizerLibCtl.C1Elastic szChart 
               Height          =   4995
               Left            =   3780
               TabIndex        =   8
               TabStop         =   0   'False
               Top             =   0
               Width           =   9570
               _cx             =   16880
               _cy             =   8811
               BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                  Name            =   "MS Sans Serif"
                  Size            =   8.25
                  Charset         =   204
                  Weight          =   400
                  Underline       =   0   'False
                  Italic          =   0   'False
                  Strikethrough   =   0   'False
               EndProperty
               Enabled         =   -1  'True
               Appearance      =   4
               MousePointer    =   0
               Version         =   801
               BackColor       =   0
               ForeColor       =   -2147483630
               FloodColor      =   6553600
               ForeColorDisabled=   -2147483631
               Caption         =   ""
               Align           =   0
               AutoSizeChildren=   2
               BorderWidth     =   0
               ChildSpacing    =   0
               Splitter        =   0   'False
               FloodDirection  =   0
               FloodPercent    =   0
               CaptionPos      =   1
               WordWrap        =   -1  'True
               MaxChildSize    =   0
               MinChildSize    =   0
               TagWidth        =   0
               TagPosition     =   0
               Style           =   0
               TagSplit        =   2
               PicturePos      =   4
               CaptionStyle    =   0
               ResizeFonts     =   0   'False
               GridRows        =   0
               GridCols        =   0
               Frame           =   3
               FrameStyle      =   0
               FrameWidth      =   1
               FrameColor      =   -2147483628
               FrameShadow     =   -2147483632
               FloodStyle      =   1
               _GridInfo       =   ""
               AccessibleName  =   ""
               AccessibleDescription=   ""
               AccessibleValue =   ""
               AccessibleRole  =   9
               Begin C1Chart2D8.Chart2D chrtPnL 
                  Height          =   4995
                  Left            =   0
                  TabIndex        =   9
                  Top             =   0
                  Width           =   9570
                  _Version        =   524288
                  _Revision       =   8
                  _ExtentX        =   16880
                  _ExtentY        =   8811
                  _StockProps     =   0
                  ControlProperties=   "ctlPnLGraphic.ctx":0000
               End
            End
            Begin C1SizerLibCtl.C1Elastic C1Elastic2 
               Height          =   4995
               Left            =   0
               TabIndex        =   10
               TabStop         =   0   'False
               Top             =   0
               Width           =   3765
               _cx             =   6641
               _cy             =   8811
               BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
                  Name            =   "MS Sans Serif"
                  Size            =   8.25
                  Charset         =   204
                  Weight          =   400
                  Underline       =   0   'False
                  Italic          =   0   'False
                  Strikethrough   =   0   'False
               EndProperty
               Enabled         =   -1  'True
               Appearance      =   4
               MousePointer    =   0
               Version         =   801
               BackColor       =   -2147483633
               ForeColor       =   -2147483630
               FloodColor      =   6553600
               ForeColorDisabled=   -2147483631
               Caption         =   ""
               Align           =   0
               AutoSizeChildren=   0
               BorderWidth     =   6
               ChildSpacing    =   4
               Splitter        =   0   'False
               FloodDirection  =   0
               FloodPercent    =   0
               CaptionPos      =   1
               WordWrap        =   -1  'True
               MaxChildSize    =   0
               MinChildSize    =   0
               TagWidth        =   0
               TagPosition     =   0
               Style           =   0
               TagSplit        =   2
               PicturePos      =   4
               CaptionStyle    =   0
               ResizeFonts     =   0   'False
               GridRows        =   0
               GridCols        =   0
               Frame           =   3
               FrameStyle      =   0
               FrameWidth      =   1
               FrameColor      =   -2147483628
               FrameShadow     =   -2147483632
               FloodStyle      =   1
               _GridInfo       =   ""
               AccessibleName  =   ""
               AccessibleDescription=   ""
               AccessibleValue =   ""
               AccessibleRole  =   9
               Begin VB.ComboBox cmbStrategy 
                  Height          =   315
                  Left            =   1920
                  Style           =   2  'Dropdown List
                  TabIndex        =   28
                  Top             =   960
                  Width           =   1545
               End
               Begin VB.ComboBox cmbTrader 
                  Height          =   315
                  ItemData        =   "ctlPnLGraphic.ctx":0599
                  Left            =   1920
                  List            =   "ctlPnLGraphic.ctx":059B
                  Style           =   2  'Dropdown List
                  TabIndex        =   27
                  Top             =   600
                  Width           =   1545
               End
               Begin VB.ComboBox cmbVolatility 
                  Height          =   315
                  Left            =   1920
                  Style           =   2  'Dropdown List
                  TabIndex        =   18
                  Top             =   1560
                  Width           =   1545
               End
               Begin VB.TextBox txtPointsCount 
                  Alignment       =   1  'Right Justify
                  Height          =   315
                  Left            =   1920
                  TabIndex        =   17
                  Text            =   "11"
                  Top             =   2520
                  Width           =   1545
               End
               Begin VB.TextBox txtChartWidth 
                  Alignment       =   1  'Right Justify
                  Height          =   315
                  Left            =   1920
                  TabIndex        =   16
                  Text            =   "20"
                  Top             =   2040
                  Width           =   1545
               End
               Begin VB.ComboBox cmbTradeUnd 
                  CausesValidation=   0   'False
                  Height          =   315
                  Left            =   1920
                  TabIndex        =   15
                  Text            =   "<select>"
                  Top             =   240
                  Width           =   1545
               End
               Begin VSFlex8UCtl.VSFlexGrid fgExp 
                  Height          =   960
                  Left            =   330
                  TabIndex        =   11
                  Top             =   3960
                  Width           =   3075
                  _cx             =   5424
                  _cy             =   1693
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
                  Rows            =   50
                  Cols            =   10
                  FixedRows       =   1
                  FixedCols       =   1
                  RowHeightMin    =   0
                  RowHeightMax    =   0
                  ColWidthMin     =   0
                  ColWidthMax     =   0
                  ExtendLastCol   =   -1  'True
                  FormatString    =   ""
                  ScrollTrack     =   0   'False
                  ScrollBars      =   0
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
               Begin VSFlex8UCtl.VSFlexGrid fgUnd 
                  Height          =   495
                  Left            =   330
                  TabIndex        =   12
                  Top             =   3120
                  Width           =   3075
                  _cx             =   5424
                  _cy             =   873
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
                  Rows            =   50
                  Cols            =   10
                  FixedRows       =   1
                  FixedCols       =   1
                  RowHeightMin    =   0
                  RowHeightMax    =   0
                  ColWidthMin     =   0
                  ColWidthMax     =   0
                  ExtendLastCol   =   -1  'True
                  FormatString    =   ""
                  ScrollTrack     =   0   'False
                  ScrollBars      =   0
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
               Begin VB.Label lblVolatility 
                  Caption         =   "Volatility"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   315
                  Left            =   330
                  TabIndex        =   22
                  Top             =   1560
                  Width           =   1245
               End
               Begin VB.Label lblPointsCount 
                  Caption         =   "Calc Points Count"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   315
                  Left            =   330
                  TabIndex        =   21
                  Top             =   2520
                  Width           =   1485
               End
               Begin VB.Label lblChartWidth 
                  Caption         =   "Chart Width (%)"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   420
                  Left            =   330
                  TabIndex        =   20
                  Top             =   2040
                  Width           =   1365
               End
               Begin VB.Label lblTradeUnd 
                  Caption         =   "Underlying"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   315
                  Left            =   330
                  TabIndex        =   19
                  Top             =   240
                  Width           =   1245
               End
               Begin VB.Label lblTrader 
                  Caption         =   "Trader"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   315
                  Left            =   330
                  TabIndex        =   29
                  Top             =   600
                  Width           =   885
               End
               Begin VB.Label lblStrategy 
                  Caption         =   "Strategy"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   435
                  Left            =   330
                  TabIndex        =   30
                  Top             =   960
                  Width           =   1005
               End
               Begin VB.Label txtExpInputs 
                  Caption         =   "Graph Expiration Inputs"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   240
                  Left            =   330
                  TabIndex        =   14
                  Top             =   3720
                  Width           =   2145
               End
               Begin VB.Label lblStockInputs 
                  Caption         =   "Stock Inputs"
                  BeginProperty Font 
                     Name            =   "Tahoma"
                     Size            =   8.25
                     Charset         =   204
                     Weight          =   700
                     Underline       =   0   'False
                     Italic          =   0   'False
                     Strikethrough   =   0   'False
                  EndProperty
                  ForeColor       =   &H00404040&
                  Height          =   195
                  Left            =   330
                  TabIndex        =   13
                  Top             =   2880
                  Width           =   945
               End
            End
         End
      End
   End
   Begin VB.PictureBox picGraphic 
      Height          =   225
      Left            =   10320
      ScaleHeight     =   165
      ScaleWidth      =   1005
      TabIndex        =   3
      Top             =   11880
      Visible         =   0   'False
      Width           =   1065
   End
   Begin VSPrinter8LibCtl.VSPrinter vsPrinterMain 
      Height          =   960
      Left            =   12000
      TabIndex        =   2
      Top             =   11760
      Visible         =   0   'False
      Width           =   1170
      _cx             =   2064
      _cy             =   1693
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      MousePointer    =   0
      BackColor       =   -2147483643
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   11.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty HdrFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   14.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      AutoRTF         =   -1  'True
      Preview         =   -1  'True
      DefaultDevice   =   0   'False
      PhysicalPage    =   -1  'True
      AbortWindow     =   -1  'True
      AbortWindowPos  =   0
      AbortCaption    =   "Printing..."
      AbortTextButton =   "Cancel"
      AbortTextDevice =   "on the %s on %s"
      AbortTextPage   =   "Now printing Page %d of"
      FileName        =   ""
      MarginLeft      =   1440
      MarginTop       =   1440
      MarginRight     =   1440
      MarginBottom    =   1440
      MarginHeader    =   0
      MarginFooter    =   0
      IndentLeft      =   0
      IndentRight     =   0
      IndentFirst     =   0
      IndentTab       =   720
      SpaceBefore     =   0
      SpaceAfter      =   0
      LineSpacing     =   100
      Columns         =   1
      ColumnSpacing   =   180
      ShowGuides      =   2
      LargeChangeHorz =   300
      LargeChangeVert =   300
      SmallChangeHorz =   30
      SmallChangeVert =   30
      Track           =   0   'False
      ProportionalBars=   -1  'True
      Zoom            =   0.946969696969697
      ZoomMode        =   3
      ZoomMax         =   400
      ZoomMin         =   10
      ZoomStep        =   25
      EmptyColor      =   -2147483636
      TextColor       =   0
      HdrColor        =   0
      BrushColor      =   0
      BrushStyle      =   0
      PenColor        =   0
      PenStyle        =   0
      PenWidth        =   0
      PageBorder      =   0
      Header          =   ""
      Footer          =   ""
      TableSep        =   "|;"
      TableBorder     =   7
      TablePen        =   0
      TablePenLR      =   0
      TablePenTB      =   0
      NavBar          =   3
      NavBarColor     =   -2147483633
      ExportFormat    =   0
      URL             =   ""
      Navigation      =   3
      NavBarMenuText  =   "Whole &Page|Page &Width|&Two Pages|Thumb&nail"
      AutoLinkNavigate=   0   'False
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   9
   End
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   8760
      Top             =   12240
   End
   Begin VB.Timer tmrFill 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   8280
      Top             =   12240
   End
   Begin VB.CommandButton cmdPrint 
      Caption         =   "Print"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   9480
      TabIndex        =   1
      Top             =   12360
      Width           =   975
   End
   Begin VB.CommandButton cmdReload 
      Caption         =   "Reload"
      BeginProperty Font 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   10560
      TabIndex        =   0
      Top             =   12360
      Width           =   975
   End
   Begin VB.Menu menuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu menuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
   End
End
Attribute VB_Name = "ctlPnLGraphic"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Public Enum PnLGraphUndColEnum
    PUC_NONE = IDX_NONE
    
    PUC_SYMBOL
    PUC_POSITION
    PUC_PRICE
    
    PUC_COLUMN_COUNT
    PUC_LAST_COLUMN = PUC_COLUMN_COUNT - 1
End Enum
    
Public Enum PnLGraphExpColEnum
    PEC_NONE = IDX_NONE
    
    PEC_ID
    PEC_SHIFT
    PEC_COLOR
    
    PEC_COLUMN_COUNT
    PEC_LAST_COLUMN = PEC_COLUMN_COUNT - 1
End Enum

Public Enum PnLGraphOptColEnum
    POC_NONE = IDX_NONE
    
    POC_SYMBOL
    POC_EXPDATE
    POC_STRIKE
    POC_TYPE
    POC_POSITION
    POC_VOL
    POC_RFR
    POC_UNYLD
    POC_DENOM
    POC_PRICE
    
    POC_COLUMN_COUNT
    POC_LAST_COLUMN = POC_COLUMN_COUNT - 1
End Enum

Public Enum PnLVolaTypeEnum
    PVT_THEO = 0
    PVT_BIDIMPLD
    PVT_MRKIMPLD
    PVT_HIST
End Enum

Const CNT_COEF = 5
Const MAX_CALC_POINTS = 50
Const MAX_CHART_WIDTH = 99

Public Event OnSetCaption()
Public Event OnStateChange()

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
Private VolaSource As VolatilitySourcesLib.VolatilitySource
Private TradeChannel As clsTradeChannel

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label
Public lblValue As vB.Label
Public WithEvents imgStop As vB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As vB.Image

Private m_QuoteRequests As New EtsMmRisksLib.MmRvReqColl
Private m_Idx As New EtsMmRisksLib.MmRvUndAtom
Private m_Grp As New EtsMmRisksLib.MmRvGrpAtom
Private m_Und As New EtsMmRisksLib.MmRvUndColl
Private m_Exp As New EtsGeneralLib.EtsMmEntityAtomColl

Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Private m_sCurUndText As String
Private m_nGreeks As Long

Public m_frmOwner As Form

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bRecalc As Boolean
Private m_bLastQuoteReqNow As Boolean

Private m_bDataEditedByUser As Boolean
Private m_enUsedVolaType As PnLVolaTypeEnum
Private m_dUndHVola As Double
Private m_dCurUndMidPrice As Double

Private Type MatrixCalcResultType
    ShiftX As Double
    ShiftY As Double
        
    PnL As Double
    Delta As Double
    NetDelta As Double
    Gamma As Double
    GammaPerc As Double
    NetGamma As Double
    Theta As Double
    Vega As Double
    Rho As Double
    WtdVega As Double

    BadPnL As Boolean
    BadDelta As Boolean
    BadNetDelta As Boolean
    BadGamma As Boolean
    BadGammaPerc  As Boolean
    BadNetGamma As Boolean
    BadTheta As Boolean
    BadVega As Boolean
    BadRho As Boolean
    BadWtdVega As Boolean
End Type

Private m_nLabelCount As Integer
Private m_nChartWidth As Integer
Private m_dHorStep As Double
Private m_nVerStep As Long
Private m_nCurUndID As Long

Private m_nGroupType As Long
Private m_nGroupID As Long
'Private m_nGroupName As String

Private m_Res() As MatrixCalcResultType
Private m_BasePoint(RMA_HORZ To RMA_VERT) As Long

Private m_bGroupRequest As Boolean
Private m_bGroupRequestSupported As Boolean
Private m_bShutDown As Boolean
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Private m_Filter As EtsFilterData


Public Function Init() As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    
    m_sCurUndText = ""
    
    InitUndList
    InitVols
    InitGrids
    InitPrinter
    InitButtons
    
    InitializeFilter
    'cmbTradeUnd.SetFocus
    
    Set VolaSource = g_VolaSource
    Set TradeChannel = g_TradeChannel
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequest = False
    m_bDataEditedByUser = False
    m_enUsedVolaType = PVT_THEO
    
    m_bGroupRequestSupported = False ' g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    Set aParams = g_Params
    
    Init = True
    
    AdjustCaption
    AdjustState
    'InitMessaging
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize risk matrix."
End Function
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    Dim nValue As Long
    
    InitValGrids
    nValue = aFilters.Data(POC_SYMBOL)
    If Not g_Underlying(nValue) Is Nothing Then
        If g_Underlying(nValue).UndType = enCtFutOption Or _
            g_Underlying(nValue).UndType = enCtFutRoot Or _
            g_Underlying(nValue).UndType = enCtFutUnd Or _
            g_Underlying(nValue).UndType = enCtFuture Then
            Exit Sub
        Else
            m_nGroupType = TYPE_UNDERLYING
            m_nGroupID = nValue
            m_nCurUndID = nValue
            cmbTradeUnd.Text = g_Underlying(nValue).Symbol
            m_sCurUndText = cmbTradeUnd.Text
            tmrShow.Enabled = True
        End If
    End If
End Sub
Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
       
    'ClearValGrid True
    
    InitValGrids
    If Not g_Underlying(nValue) Is Nothing Then
        If g_Underlying(nValue).UndType = enCtFutOption Or _
            g_Underlying(nValue).UndType = enCtFutRoot Or _
            g_Underlying(nValue).UndType = enCtFutUnd Or _
            g_Underlying(nValue).UndType = enCtFuture Then
            Exit Sub
        Else
            m_nGroupType = nType
            m_nGroupID = nValue
            m_nCurUndID = nValue
            cmbTradeUnd.Text = g_Underlying(nValue).Symbol
            m_sCurUndText = cmbTradeUnd.Text
            tmrShow.Enabled = True
        End If
    Else
        m_nGroupType = TYPE_UNDERLYING
    End If
    
End Sub

Public Sub StopNow()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
    
        'fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
        InitValGrids
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bDataLoad Then
        m_bDataLoad = False
    
        'fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
        InitValGrids
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bRecalc Then
        m_bRecalc = False
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bInProc Then
        m_bInProc = False
        AdjustState
    End If
End Sub

Private Function IndexLoad() As Boolean
    On Error Resume Next
    IndexLoad = False
    If m_bInProc Then Exit Function
    Dim aGIdx As EtsGeneralLib.IndexAtom, aComp As EtsGeneralLib.IndexCompAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, sKey$
    Dim aReq As EtsMmRisksLib.MmRvReqAtom, nKey&, aPos As EtsMmRisksLib.MmRvPosAtom
    
    m_bInProc = True
    AdjustState
    
    If m_Idx.ID <> 0 Then
        For Each aUnd In m_Und
            aUnd.Beta = BAD_DOUBLE_VALUE
        Next
        
        sKey = CStr(enCtIndex) & "_" & m_Idx.Symbol
        Set aReq = m_QuoteRequests(sKey)
        If Not aReq Is Nothing Then
            If aReq.IndexOnly Then
                If Not PriceProvider Is Nothing Then
                    PriceProvider.CancelLastQuote aReq.GetQuoteUpdateParam
                    If m_bGroupRequest Then
                        m_GroupPriceProvider.CancelGroup
                    End If
                End If
                Set aReq = Nothing
                
                m_QuoteRequests.Remove sKey
            End If
        End If
    
        m_Idx.Clear
    End If
    
    'If m_nFilter(MFC_INDEX) <> 0 Then
        Set aGIdx = g_Index.BySortKey("DJX")
        nKey = aGIdx.ID
        Debug.Assert Not aGIdx Is Nothing
        If Not aGIdx Is Nothing Then
            m_Idx.ID = nKey
            m_Idx.ContractType = enCtIndex
            m_Idx.Symbol = aGIdx.Symbol
            
            For Each aUnd In m_Und
                Set aComp = aGIdx.Components(aUnd.ID)
                
                If Not aComp Is Nothing Then
                    aUnd.Beta = aComp.Beta
                    Set aComp = Nothing
                End If
                
'                If aUnd.HasSynthetic Then
'                    For Each aPos In aUnd.Pos
'                        If aPos.IsSynthetic Then
'                            Set aComp = aGIdx.SyntheticRootBetas(aPos.OptionRootID)
'
'                            If Not aComp Is Nothing Then
'                                aPos.SynthBeta = aComp.Beta
'                                Set aComp = Nothing
'                            End If
'
'                        End If
'                    Next
'                End If
            Next
            
            sKey = CStr(enCtIndex) & "_" & m_Idx.Symbol
            If m_QuoteRequests(sKey) Is Nothing Then
                Set aReq = m_QuoteRequests.Add(sKey)
                Set aReq.Und = m_Idx
                aReq.IndexOnly = True
                Set aReq = Nothing
            End If
            Set aGIdx = Nothing
'        Else
'            m_nFilter(MFC_INDEX) = 0
'            FilterUpdateIndex True
        End If
'    End If
    
    IndexLoad = True
    m_bInProc = False
    AdjustState
End Function

Public Sub Term()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_bShutDown = True
    
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
    m_bInProc = False
    
    'Set frmLayout = Nothing
    'Set frmWtdVega = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    Set m_Filter = Nothing
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        Set PriceProvider = Nothing
    End If
    Set m_GroupPriceProvider = Nothing
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set lblValue = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
    
    Erase m_Res
End Sub


Public Sub OnFilterChanged()
On Error GoTo error_handler
'--------------------------'
    Dim lValue As Long
    Dim lListIndex As Long
    Dim lItemValue As Long
    Dim bFilterModified As Boolean
    
    bFilterModified = False
    '--- get trader filter ---'
    lValue = m_Filter.Data(enFtTrader)
    lListIndex = cmbTrader.ListIndex
    lItemValue = cmbTrader.ItemData(lListIndex)
    
    If (lValue <> lItemValue) Then
        m_Filter.Data(enFtTrader) = lItemValue
        bFilterModified = True
    End If
    
    '--- get strategy filter ---'
    lValue = m_Filter.Data(enFtStrategy)
    lListIndex = cmbStrategy.ListIndex
    lItemValue = cmbStrategy.ItemData(lListIndex)
    
    If (lValue <> lItemValue) Then
        m_Filter.Data(enFtStrategy) = lItemValue
        bFilterModified = True
    End If
    
    '--- update view ---
    If (bFilterModified And Not m_bInProc) Then
        tmrShow.Enabled = True
    End If
    
Exit Sub
error_handler:
    Debug.Print True
End Sub

Public Sub InitializeFilter()
On Error GoTo error_handler
'--------------------------'
    Dim itemIndex As Long
    
    'create filter atom'
    Set m_Filter = Nothing
    Set m_Filter = New EtsFilterData
    
    m_Filter.Initialize enFtSymbol, enFtCountTypes
    
    '---- initialize traders combo box ----'
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    
    itemIndex = 0
    cmbTrader.Clear
    
    ' add default value'
    cmbTrader.AddItem "<all>", itemIndex
    cmbTrader.ItemData(itemIndex) = BAD_LONG_VALUE
    itemIndex = itemIndex + 1
    
    For Each aTrader In g_Trader
        cmbTrader.AddItem aTrader.Name, itemIndex
        cmbTrader.ItemData(itemIndex) = aTrader.ID
        itemIndex = itemIndex + 1
    Next
    
    cmbTrader.ListIndex = 0
    
    '---- initialize strategy combo box ----'
    Dim aStrategy As EtsStrategyAtom
    
    itemIndex = 0
    cmbStrategy.Clear
    
    ' add default value '
    cmbStrategy.AddItem "<all>", itemIndex
    cmbStrategy.ItemData(itemIndex) = BAD_LONG_VALUE
    itemIndex = itemIndex + 1
    
    For Each aStrategy In g_Strategy
        cmbStrategy.AddItem aStrategy.Name, itemIndex
        cmbStrategy.ItemData(itemIndex) = aStrategy.ID
        itemIndex = itemIndex + 1
    Next
    
    cmbStrategy.ListIndex = 0
    
Exit Sub
error_handler:
    Debug.Assert False
    Debug.Print Err.Description
End Sub

Public Sub InitUndList()
    Dim aUnd As EtsGeneralLib.UndAtom
    Dim iIndex As Integer
    iIndex = 0
    For Each aUnd In g_Underlying
      If aUnd.UndType = enCtIndex Or aUnd.UndType = enCtStock Then
            If Not g_TradeChannel.TradesByUnd(aUnd.ID) Is Nothing Then
                cmbTradeUnd.AddItem aUnd.Symbol, iIndex
                cmbTradeUnd.ItemData(iIndex) = aUnd.ID
                iIndex = iIndex + 1
            End If
      ElseIf aUnd.UndType = enCtFuture Then
'            If Not g_TradeChannel.TradesByFut(aUnd.ID) Is Nothing Then
'                cmbTradeUnd.AddItem aUnd.Symbol, iIndex
'                cmbTradeUnd.ItemData(iIndex) = aUnd.ID
'                iIndex = iIndex + 1
'            End If
      End If
    Next
End Sub

Private Sub ClearViewAndData()
    On Error Resume Next
    
    fgExp.Cell(flexcpText, 1, PEC_SHIFT, 3, PEC_SHIFT) = "" ' Should Create a sub for exp grid init
    fgUnd.Clear flexClearScrollable
    fgOpt.rows = 1
    lblValue.Caption = ""

    m_Grp.Clear
    m_Und.Clear
    m_Exp.Clear
    m_QuoteRequests.Clear
    
    vsPrinterMain.Clear
    ClearChart
    
    InitValGrids
    
    SetRefreshHint False
End Sub

Public Sub InitGrids()
    With fgUnd
        .rows = 2
        .Cols = PUC_COLUMN_COUNT
        .ColWidthMin = CInt(fgUnd.Width / PUC_COLUMN_COUNT)
        
        
        .ColDataType(PUC_SYMBOL) = flexDTString
        .ColFormat(PUC_SYMBOL) = ""
        .TextMatrix(0, PUC_SYMBOL) = "Symbol"
        
        .ColDataType(PUC_POSITION) = flexDTLong
        .ColFormat(PUC_POSITION) = "#,###"
        .TextMatrix(0, PUC_POSITION) = "Position"
        
        .ColDataType(PUC_PRICE) = flexDTDouble
        .ColFormat(PUC_PRICE) = "#,##0.00"
        .TextMatrix(0, PUC_PRICE) = "Price"
        
        .AllowSelection = False
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        '.ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollTrack = True
    End With
    
    With fgExp
        .rows = 4
        .Cols = PEC_COLUMN_COUNT
        
        .TextMatrix(0, PEC_ID) = "Line"
        .TextMatrix(0, PEC_SHIFT) = "Days After Today"
        .TextMatrix(0, PEC_COLOR) = "Color"
        
        .TextMatrix(1, PEC_ID) = "1"
        
        .TextMatrix(2, PEC_ID) = "2"
        .TextMatrix(3, PEC_ID) = "3"
        
        .Select 1, PEC_COLOR
        .CellBackColor = vbBlue
        
        .Select 2, PEC_COLOR
        .CellBackColor = vbRed
        
        .Select 3, PEC_COLOR
        .CellBackColor = vbGreen
        
        .Select 0, 0
        
        ' Values are set to minimize cols width
        .ColWidth(PEC_ID) = 600
        .ColAlignment(PEC_ID) = flexAlignLeftCenter
        .ColWidth(PEC_SHIFT) = CInt((fgExp.Width - .ColWidth(PEC_ID)) / 2)
        .ColWidth(PEC_COLOR) = CInt((fgExp.Width - .ColWidth(PEC_ID)) / 2)
        
        .AllowSelection = False
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        '.ExtendLastCol = False
        .FixedCols = 1
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollTrack = True
    End With
    
    With fgOpt
        .rows = 1
        .Cols = POC_COLUMN_COUNT
        .ColWidthMin = 600
        
        .ColDataType(POC_SYMBOL) = flexDTString
        .ColFormat(POC_SYMBOL) = ""
        .TextMatrix(0, POC_SYMBOL) = "Symbol"
        
        .ColDataType(POC_EXPDATE) = flexDTDate
        .ColFormat(POC_EXPDATE) = "MMM,DD YYYY"
        .TextMatrix(0, POC_EXPDATE) = "Exp"
        
        .ColDataType(POC_STRIKE) = flexDTDouble
        .ColFormat(POC_STRIKE) = "#,##0.00"
        .TextMatrix(0, POC_STRIKE) = "Strike"
        
        .ColDataType(POC_TYPE) = flexDTString
        .ColFormat(POC_TYPE) = ""
        .TextMatrix(0, POC_TYPE) = "C/P"
        
        .ColDataType(POC_POSITION) = flexDTLong
        .ColFormat(POC_POSITION) = "#,###"
        .TextMatrix(0, POC_POSITION) = "Pos"
        
        
        .ColDataType(POC_VOL) = flexDTDouble
        .ColFormat(POC_VOL) = "#,##0.00"
        .TextMatrix(0, POC_VOL) = "Volatility"
        
        
        .ColDataType(POC_RFR) = flexDTDouble
        .ColFormat(POC_RFR) = "#,##0.00"
        .TextMatrix(0, POC_RFR) = "RFR (%)"
        
        
        .TextMatrix(0, POC_UNYLD) = "UnYld"
        
        
        .ColDataType(POC_DENOM) = flexDTLong
        .ColFormat(POC_DENOM) = "#,###"
        .TextMatrix(0, POC_DENOM) = "Dnm"
        
        .ColDataType(POC_PRICE) = flexDTDouble
        .ColFormat(POC_PRICE) = "#,##0.00"
        .TextMatrix(0, POC_PRICE) = "Price"
        
        .AllowSelection = False
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        '.ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollTrack = True
        
        .ColAlignment(POC_TYPE) = flexAlignCenterCenter
        .Cell(flexcpAlignment, 0, 1, 0, POC_LAST_COLUMN) = flexAlignCenterCenter
    End With
    
    With fgVal
        .rows = 1
        .Cols = 1
        
        .RowHeightMin = 280
        .AllowSelection = False
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortShow
        '.ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 0
        .FocusRect = flexFocusNone
        .FrozenCols = 1
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .ScrollTrack = True
    End With
End Sub

Private Sub InitValGrids()
    fgVal.Cols = 1
    fgVal.rows = 0
End Sub

Private Sub InitVols()
    cmbVolatility.AddItem ("Trade Volatility")
    cmbVolatility.AddItem ("Current Bid IV")
    cmbVolatility.AddItem ("Market IV")
    cmbVolatility.AddItem ("Historical Volatility")
    
    cmbVolatility.Text = cmbVolatility.List(0)
End Sub

Private Sub InitButtons()
    cmdPrint.Enabled = False
End Sub

Private Sub cmbStrategy_Click()
    OnFilterChanged
End Sub

Private Sub cmbTrader_Click()
    OnFilterChanged
End Sub

Private Sub cmbTradeUnd_LostFocus()
    If (cmbTradeUnd.ListCount <> 0) Then
        If cmbTradeUnd.ListIndex < 0 Then cmbTradeUnd.ListIndex = 0
        If m_Grp.ID <> cmbTradeUnd.ItemData(cmbTradeUnd.ListIndex) Then
            If Validate_Und Then
                If m_sCurUndText = cmbTradeUnd.Text Then
                    SetRefreshHint True
                Else
                    m_nCurUndID = cmbTradeUnd.ItemData(cmbTradeUnd.ListIndex)
                    m_nGroupID = m_nCurUndID
                    m_sCurUndText = cmbTradeUnd.Text
                    tmrShow.Enabled = True
                End If
                m_bDataEditedByUser = False
            Else
                cmbTradeUnd_GotFocus
            End If
        End If
    End If
End Sub


Private Sub cmbTradeUnd_GotFocus()
    cmbTradeUnd.SelStart = 0
    cmbTradeUnd.SelLength = Len(cmbTradeUnd.Text)
End Sub

Private Sub cmbTradeUnd_KeyDown(KeyCode As Integer, Shift As Integer)
    If (cmbTradeUnd.ListCount <> 0) Then
        If KeyCode = 13 Then
            If Validate_Und Then
                If m_sCurUndText = cmbTradeUnd.Text Then
                    SetRefreshHint True
                Else
                    m_nCurUndID = g_Underlying.BySortKey(cmbTradeUnd.Text).ID
                    m_nGroupID = m_nCurUndID
                    m_sCurUndText = cmbTradeUnd.Text
                    m_nGroupType = TYPE_UNDERLYING
                    tmrShow.Enabled = True
                End If
                m_bDataEditedByUser = False
            Else
                cmbTradeUnd_GotFocus
            End If
        End If
    End If
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub


Private Sub cmbVolatility_Click()
    m_enUsedVolaType = cmbVolatility.ListIndex
    If m_nCurUndID <> 0 Then
        m_bDataEditedByUser = False
        SetRefreshHint True
    End If
End Sub
Public Sub PrintView()
    cmdPrint_Click
End Sub


Private Sub cmdPrint_Click()
'    Dim aDefGridFont As Font
    Dim sBuffer As String
    On Error Resume Next

    With vsPrinterMain
        '.Preview = False
        .FontName = "Tahoma"
        .FontSize = "8"
        .HdrFontSize = "12"
        .HdrFontBold = True
        
        .FontBold = True
        .Header = "PnL Projections for " & m_Grp.Name & "||Page %d"
        .Footer = Format(Now, "dd-mmm-yy") & "||"

        .MarginLeft = "1.5cm"
        .MarginTop = "2cm"
        .MarginRight = "2cm"
        .MarginBottom = "2cm"
        .Orientation = orLandscape
        
        .StartDoc
        
        .Paragraph = vbCrLf & vbCrLf & vbCrLf & "PnL Results for Underlying:" & vbCrLf
        fgUnd.ExtendLastCol = False
        .RenderControl = fgUnd.hWnd
        fgUnd.ExtendLastCol = True
        
        fgOpt.ExtendLastCol = False
        .Paragraph = vbCrLf & vbCrLf & vbCrLf & vbCrLf & "Positions for " & m_Grp.Name & ":" & vbCrLf
        .RenderControl = fgOpt.hWnd
        fgOpt.ExtendLastCol = True
        
        sBuffer = Space$(2048)
        If GetTempPath(2048, sBuffer) = 0 Then
            .Paragraph = "P&L Graphic is cannot be printed for this symbol." & vbCrLf
            .EndDoc
            .PrintDoc
        End If
        
        sBuffer = Trim$(sBuffer)
        Mid$(sBuffer, Len(sBuffer), 1) = " "
        sBuffer = Trim$(sBuffer)
        If Mid$(sBuffer, Len(sBuffer), 1) <> "\" Then sBuffer = sBuffer & "\"
        sBuffer = sBuffer & "etsgpnl" & Format$(g_nGraphicalPnLUntitledIdx, "000") & ".bmp"
        
        .Paragraph = vbCrLf & vbCrLf & vbCrLf & vbCrLf & vbCrLf & "Line 1 Greeks:" & vbCrLf
        .RenderControl = fgVal.hWnd
        
        If chrtPnL.DrawToFile(sBuffer, oc2dFormatBitmap) = True Then
            .DrawPicture LoadPicture(sBuffer), "14cm", "1.5cm"
        End If
        
        .EndDoc
        
        .PrintDoc True
        
        Kill sBuffer
EH:
    End With
End Sub

Private Sub cmdReload_Click()
    If m_nCurUndID = 0 Then Exit Sub
    m_bDataEditedByUser = False
    Refresh
End Sub

Private Sub fgExp_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim aExp As EtsGeneralLib.EtsMmEntityAtom
    On Error Resume Next
    Select Case Col
        Case PEC_SHIFT:
            If fgExp.TextMatrix(Row, Col) <> "" Then
                If Not m_Exp.Item(CStr(Row - 1)) Is Nothing Then
                    m_Exp(CStr(Row - 1)).Name = Date + fgExp.TextMatrix(Row, Col)
                ElseIf Row <> 1 Then
                    Set aExp = New EtsGeneralLib.EtsMmEntityAtom
                    aExp.Name = Date + fgExp.TextMatrix(Row, Col)
                    m_Exp.Add CStr(Row - 1), aExp
                    Set aExp = Nothing
                End If
            End If
            m_bDataEditedByUser = True
            SetRefreshHint True
    End Select
End Sub

Private Sub fgExp_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    If m_nCurUndID = 0 Or Col = PEC_COLOR Then
        Cancel = True
        Exit Sub
    End If
End Sub

Private Sub fgExp_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nShift&
    
    nShift = 0
    Err.Clear
    
    Select Case Col
        Case PEC_SHIFT:
            If fgExp.EditText <> "" Then _
                nShift = CLng(fgExp.EditText)
    End Select
    
    If Err.Number <> 0 Then Cancel = True
End Sub

Private Sub fgOpt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, dVola#
    
    Set aPos = m_Und(m_nCurUndID).Pos.BySortKey(fgOpt.TextMatrix(Row, POC_SYMBOL))
    Select Case Col
        Case POC_POSITION:
            If aPos.Qty <> fgOpt.TextMatrix(Row, Col) Then
                aPos.Qty = fgOpt.TextMatrix(Row, Col)
                m_bDataEditedByUser = True
                SetRefreshHint True
            End If
        Case POC_PRICE:
            m_bDataEditedByUser = True
            SetRefreshHint True
        Case POC_VOL:
            dVola = fgOpt.ValueMatrix(Row, Col) / 100
            aPos.Quote.Vola = dVola
            m_bDataEditedByUser = True
            SetRefreshHint True
    End Select
End Sub

Private Sub fgOpt_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    If m_nCurUndID = 0 Or Col <> POC_PRICE And Col <> POC_POSITION And Col <> POC_VOL Then
        Cancel = True
        Exit Sub
    End If
End Sub

Private Sub fgOpt_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim dPrice#, dVol#, nPos&
    
    Err.Clear
    
    Select Case Col
        Case POC_VOL:
            dvo = CDbl(fgOpt.EditText)
        Case POC_PRICE:
            dPrice = CDbl(fgOpt.EditText)
        Case POC_POSITION:
            nPos = CLng(fgOpt.EditText)
    End Select
    
    If Err.Number <> 0 Then Cancel = True
End Sub

Private Sub EditCurUndPosQty(sSymbolName As String, nNewQty As Long)
    m_Und(m_nCurUndID).Pos.BySortKey(sSymbolName).Qty = nNewQty
    m_Und(m_nCurUndID).Qty = nNewQty
End Sub

Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    
    Dim sSymbolName As String
    sSymbolName = m_Und(m_nCurUndID).Symbol
    
    Select Case Col
        Case PUC_POSITION:
            EditCurUndPosQty sSymbolName, fgUnd.TextMatrix(Row, Col)
            m_bDataEditedByUser = True
            SetRefreshHint True
        Case PUC_PRICE:
            'm_Und(m_nCurUndID).VolaSrv.UnderlyingPrice = fgUnd.TextMatrix(Row, Col)
            m_bDataEditedByUser = True
            SetRefreshHint True
    End Select
End Sub

Private Sub fgUnd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    
    If Col = PUC_SYMBOL Or m_nCurUndID = 0 Then
        Cancel = True
        Exit Sub
    End If
End Sub

Private Sub fgUnd_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nPosition&, dPrice#
    
    nPosition = 0
    dPrice = 0
    Err.Clear
    
    Select Case Col
        Case PUC_POSITION:
            nPosition = CLng(fgUnd.EditText)
            
        Case PUC_PRICE:
            dPrice = CDbl(fgUnd.EditText)
    End Select
    
    If Err.Number <> 0 Then Cancel = True
    'fgUnd.FinishEditing Cancel
End Sub

Private Sub fgVal_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Cancel = True
End Sub

Private Sub menuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRefresh.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = ""
    If m_Grp.ID <> 0 Then sCaption = sCaption & m_Grp.Name & " - "
    sCaption = sCaption & "PnL Projections"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    If m_Grp.ID <> 0 Then
        GetShortCaption = m_Grp.Name
    Else
        GetShortCaption = "<Empty>"
    End If
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Property Get DataLoad() As Boolean
    DataLoad = m_bDataLoad
End Property

Public Property Get LastQuoteReqNow() As Boolean
    LastQuoteReqNow = m_bLastQuoteReqNow
End Property

Public Function Group() As EtsMmRisksLib.MmRvGrpAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Function CurUnderlyingID() As Long
    On Error Resume Next
   
    If m_Grp.GroupType = TYPE_UNDERLYING Then
        CurUnderlyingID = m_nCurUndID
    Else
        CurUnderlyingID = 0
    End If
End Function

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
        
    If m_bInProc Or m_bDataLoad Then Exit Sub
    
    'Screen.MousePointer = vbArrow
    DoEvents
    
    If m_Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        m_bLastQuoteReqNow = False
        m_QuoteRequests.Clear
    End If
    
    ClearViewAndData
    lblStatus.Caption = ""

    AdjustCaption
    
    On Error Resume Next
    Dim bIncorrectInput As Boolean
    
    bIncorrectInput = False
    Err.Clear
    
    m_nLabelCount = CInt(txtPointsCount.Text)
    m_nChartWidth = CInt(txtChartWidth.Text)
    
    bIncorrectInput = (m_nLabelCount <= 1) Or (m_nLabelCount > MAX_CALC_POINTS) Or (m_nChartWidth <= 0) Or (m_nChartWidth > MAX_CHART_WIDTH)
    
    If bIncorrectInput Then
        ShowUserWarning
        Exit Sub
    End If
    
    If Err.Number <> 0 Then Exit Sub

    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = TradeChannel.Trades.Count
    pbProgress.Visible = True
    lblProcess.Caption = "Data loading..."
    lblProcess.Visible = True
    lblProcess.Refresh
    
    If PositionsLoad Then
        
        IndexLoad
        
        AdjustCaption
        lblProcess.Caption = "Last quotes request..."
        lblProcess.Refresh
        
        RequestLastQuotes False
    Else
        'If m_nFilter(MFC_GROUP) = TYPE_ALL Then m_nFilter(MFC_GROUP) = TYPE_UNDERLYING
        'm_nFilter(MFC_VALUE) = 0
        ClearViewAndData
        lblStatus.Caption = ""
        'FilterUpdateAll
        AdjustCaption
        AdjustState
        
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If

    Screen.MousePointer = vbDefault
End Sub

Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean
    
    CheckTradeFilter = False
    bMatched = False
    
    If Not aTrd Is Nothing Then
        ' futures & futures options trades are not supported yet
        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then Exit Function
        
        If aTrd.ContractType = enCtOption Then
            If aTrd.Opt.Expiry < Date Then Exit Function
        End If
        
        If aTrd.UndID = m_nCurUndID Then _
            bMatched = True
            
        bMatched = aTrd.CheckByFilter(m_Filter)
    End If

    CheckTradeFilter = bMatched
End Function

Private Sub aParams_PriceProfilesChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    Dim aGUnd As EtsGeneralLib.UndAtom
    
    If m_Grp.GroupType = 0 Or m_Grp.ID = 0 Then Exit Sub
    bUpdate = False
    
    On Error Resume Next
    For Each aFilterAtom In collUpdUnd
        If Not m_Und(aFilterAtom.ID) Is Nothing Then
            bUpdate = True
            Set aFilterAtom = Nothing
            Exit For
        End If
    Next
    If bUpdate Then SetRefreshHint True
End Sub

Private Sub aParams_UndPriceToleranceChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Function PositionsLoad() As Boolean
    On Error GoTo EH
    PositionsLoad = False
    If m_bInProc Then Exit Function
    
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom, sExpiryKey$
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aExp As EtsGeneralLib.EtsMmEntityAtom, aReq As EtsMmRisksLib.MmRvReqAtom
    Dim collExp As EtsGeneralLib.EtsMmEntityAtomColl, arrExp() As Long, sKey$, aEnt As EtsGeneralLib.EtsMmEntityAtom, nQty&, nQtyInShares&
    Dim aGUnd As EtsGeneralLib.UndAtom, aSynthRoot As EtsGeneralLib.SynthRootAtom, aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    
    m_Grp.ID = m_nGroupID
    m_Grp.GroupType = m_nGroupType

    If m_Grp.ID = 0 Or m_Grp.GroupType = 0 Then Exit Function
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState
    Set collExp = New EtsGeneralLib.EtsMmEntityAtomColl
    
    If m_Grp.GroupType = TYPE_UNDERLYING Then
        Set aGUnd = g_Underlying(m_Grp.ID)
        If Not aGUnd Is Nothing Then
            m_Grp.Name = aGUnd.Symbol
            Set aGUnd = Nothing
        Else
            m_bDataLoad = False
        End If
    Else
        m_bDataLoad = False
    End If
   
    If m_bDataLoad And Not g_TradeChannel.TradesByUnd(m_nGroupID) Is Nothing Then
        
        For Each aTrd In g_TradeChannel.TradesByUnd(m_nGroupID)
            If CheckTradeFilter(aTrd) Then
                Set aUnd = m_Und(aTrd.UndID)
                If aUnd Is Nothing Then
                    Set aUnd = m_Und.Add(aTrd.UndID, aTrd.Und.Symbol)
                    aUnd.ID = aTrd.Und.ID
                    aUnd.ContractType = aTrd.Und.UndType
                    aUnd.Symbol = aTrd.Und.Symbol
                    
                    If aUnd.ContractType = enCtStock Then
                        Set aUnd.Dividend = aTrd.Und.Dividend
                    Else
                        Set aUnd.BasketIndex = g_Index(aUnd.ID)
                        If Not aUnd.BasketIndex Is Nothing Then
                            If Not aUnd.BasketIndex.IsBasket Then
                                Set aUnd.BasketIndex = Nothing
                                aUnd.Yield = aTrd.Und.Yield
                            End If
                        Else
                            aUnd.Yield = aTrd.Und.Yield
                        End If

                    End If
                    
                    aUnd.IsAmerican = aTrd.Und.IsAmerican
                    aUnd.IsHTB = aTrd.Und.IsHTB
                    aUnd.Skew = aTrd.Und.Skew
                    aUnd.Kurt = aTrd.Und.Kurt
                    aUnd.HasSynthetic = aTrd.Und.HaveSyntheticRoots
                    Set aUnd.SynthRoots = aTrd.Und.SyntheticRoots
                    Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
                    Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
                
                    aUnd.price.Close = aTrd.Und.PriceClose
                    aUnd.price.TheoClose = aTrd.Und.PriceTheoclose
                    aUnd.LotSize = aTrd.Und.LotSize
                    
                    aUnd.Qty = BAD_LONG_VALUE
                    aUnd.OptQty = BAD_LONG_VALUE
                    aUnd.UndPosForRates = 0
                    
                    InitVola aUnd
                    
                    sKey = CStr(aUnd.ContractType) & "_" & aUnd.Symbol
                    If m_QuoteRequests(sKey) Is Nothing Then
                        Set aReq = m_QuoteRequests.Add(sKey)
                        Set aReq.Und = aUnd
                        Set aReq = Nothing
                    End If
                    DoEvents
                    If Not m_bDataLoad Then Exit For
                End If
                
                Set aPos = aUnd.Pos(aTrd.ContractID)
                If aPos Is Nothing Then
                    Set aPos = aUnd.Pos.Add(aTrd.ContractID, aTrd.Symbol)
                    aPos.ID = aTrd.ContractID
                    
                    aPos.ContractType = aTrd.ContractType
                    
                    If aPos.ContractType = enCtOption Then
                        aUnd.HasOptPos = True
                        aPos.Symbol = aTrd.Opt.Symbol
                        aPos.Quote.price.Close = aTrd.Opt.PriceClose
                        aPos.Quote.price.TheoClose = aTrd.Opt.PriceTheoclose
                        aPos.Quote.LotSize = aTrd.OptRoot.LotSize
                        aPos.OptType = aTrd.Opt.OptType
                        aPos.Expiry = aTrd.Opt.Expiry
                        aPos.Strike = aTrd.Opt.Strike
                        aPos.OptionRootID = aTrd.OptRoot.ID
                        aPos.UndID = aUnd.ID
                        
                        aPos.ExpiryOV = aTrd.Opt.ExpiryOV
                        aPos.TradingClose = aTrd.Opt.TradingClose
                        
                        aPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aPos.ExpiryOV)
                        
                        sExpiryKey = CStr(CLng(aPos.Expiry))
                        Set aExp = collExp(sExpiryKey)
                        If aExp Is Nothing Then
                            Set aExp = collExp.Add(sExpiryKey)
                            aExp.ID = aPos.Expiry
                            aExp.Name = CStr(aPos.Expiry)
                            
                            If collExp.Count = 1 Then ReDim arrExp(1 To 1) As Long Else ReDim Preserve arrExp(1 To collExp.Count) As Long
                            arrExp(collExp.Count) = aExp.ID
                            
                            Set aExp = Nothing
                        End If
                                                    
                        If aUnd.HasSynthetic And Not aUnd.SynthRoots Is Nothing Then
                            ' load underlyings from synthetic roots
                            Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
                        
                            If Not aSynthRoot Is Nothing Then
                                
                                aPos.IsSynthetic = True
                                
                                For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                                    
                                    If aSynthRootComp.UndID <> aUnd.ID Then
                                    
                                        Set aSynthUnd = m_Und(aSynthRootComp.UndID)
                                        Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)

                                        If Not aGUnd Is Nothing Then

                                            If aSynthUnd Is Nothing Then
                                                
                                                Set aSynthUnd = m_Und.Add(aSynthRootComp.UndID, aGUnd.Symbol)
                                                aSynthUnd.ID = aSynthRootComp.UndID
                                                
                                                aSynthUnd.ContractType = aGUnd.UndType
                                                aSynthUnd.Symbol = aGUnd.Symbol
                                                aSynthUnd.IsAmerican = aGUnd.IsAmerican
                                                aSynthUnd.IsHTB = aGUnd.IsHTB
                                                aSynthUnd.Skew = aGUnd.Skew
                                                aSynthUnd.Kurt = aGUnd.Kurt
                                                aSynthUnd.HasSynthetic = aGUnd.HaveSyntheticRoots
                                                aSynthUnd.price.Close = aGUnd.PriceClose
                                                aSynthUnd.price.TheoClose = aGUnd.PriceTheoclose
                                                aSynthUnd.LotSize = aGUnd.LotSize
                                                Set aSynthUnd.SynthRoots = aGUnd.SyntheticRoots
                                                
                                                If aSynthUnd.ContractType = enCtStock Then
                                                     Set aSynthUnd.Dividend = aGUnd.Dividend
                                                Else
                                                    Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
                                                    If Not aSynthUnd.BasketIndex Is Nothing Then
                                                        If Not aSynthUnd.BasketIndex.IsBasket Then
                                                            Set aSynthUnd.BasketIndex = Nothing
                                                            aSynthUnd.Yield = aGUnd.Yield
                                                         End If
                                                    Else
                                                        aSynthUnd.Yield = aGUnd.Yield
                                                    End If
                                                    
                                                End If
                                                Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
                                                Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile
                                                                                                                                                                                                                            
                                                aSynthUnd.Qty = BAD_LONG_VALUE
                                                aSynthUnd.OptQty = BAD_LONG_VALUE
                                                aSynthUnd.UndPosForRates = 0
                                                
                                                InitVola aSynthUnd
                        
                                                sKey = CStr(aSynthUnd.ContractType) & "_" & aSynthUnd.Symbol
                                                If m_QuoteRequests(sKey) Is Nothing Then
                                                    Set aReq = m_QuoteRequests.Add(sKey)
                                                    Set aReq.Und = aSynthUnd
                                                End If
                                            End If
                                        
                                            Set aGUnd = Nothing
                                        End If
                                    End If
                                Next
                                Set aSynthUnd = Nothing
                            End If
                        End If
                    Else
                        aPos.Symbol = aUnd.Symbol
                        aPos.Quote.price.Close = aUnd.price.Close
                        aPos.Quote.price.TheoClose = aUnd.price.TheoClose
                        aPos.Quote.LotSize = aUnd.LotSize
                    End If
                
                    aPos.Qty = 0
                    aPos.QtyInShares = 0
                    
                    aPos.QtyLTDBuy = BAD_LONG_VALUE
                    aPos.PosLTDBuy = BAD_DOUBLE_VALUE
                    aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE
                    aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE
                    aPos.QtyDailyTodayBuy = BAD_LONG_VALUE
                    aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE
                    
                    aPos.QtyLTDSell = BAD_LONG_VALUE
                    aPos.PosLTDSell = BAD_DOUBLE_VALUE
                    aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE
                    aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE
                    aPos.QtyDailyTodaySell = BAD_LONG_VALUE
                    aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE
                    
                    sKey = CStr(aPos.ContractType) & "_" & aPos.Symbol
                    Set aReq = m_QuoteRequests(sKey)
                    If aReq Is Nothing Then
                        Set aReq = m_QuoteRequests.Add(sKey)
                        Set aReq.Und = aUnd
                    End If
                    
                    If aReq.Pos Is Nothing Then
                        Set aReq.Pos = aPos
                    End If
                    
                    Set aReq = Nothing
                End If
            
                nQty = aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
                nQtyInShares = nQty * aPos.Quote.LotSize
                
                'SUD-0909
                Dim dPriceClose As Double
                dPriceClose = aPos.Quote.price.Close
                If (Not g_Main Is Nothing) Then
                    If (g_Main.UseTheoCloseForPNL = True And aPos.Quote.price.TheoClose > 0#) Then
                        dPriceClose = aPos.Quote.price.TheoClose
                    End If
                End If
                
                If aTrd.IsBuy Then
                    If aPos.QtyLTDBuy = BAD_LONG_VALUE Then aPos.QtyLTDBuy = 0
                    aPos.QtyLTDBuy = aPos.QtyLTDBuy + nQtyInShares
                    If aTrd.IsPosition Then
                        If dPriceClose >= DBL_EPSILON Then
                            If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
                            aPos.PosLTDBuy = aPos.PosLTDBuy + dPriceClose * nQtyInShares
                        End If
                    Else
                        If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
                        aPos.PosLTDBuy = aPos.PosLTDBuy + aTrd.price * nQtyInShares
                    End If
                
                    If aTrd.TradeDate < Date Then
                        If aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateBuy = 0
                        aPos.QtyDailyPrevDateBuy = aPos.QtyDailyPrevDateBuy + nQtyInShares
                        If dPriceClose >= DBL_EPSILON Then
                            If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                            aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + dPriceClose * nQtyInShares
                        ElseIf Not aTrd.IsPosition Then
                            If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                            aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aTrd.price * nQtyInShares
                        End If
                    Else
                        If aPos.QtyDailyTodayBuy = BAD_LONG_VALUE Then aPos.QtyDailyTodayBuy = 0
                        aPos.QtyDailyTodayBuy = aPos.QtyDailyTodayBuy + nQtyInShares
                        If aTrd.IsPosition Then
                            If dPriceClose >= DBL_EPSILON Then
                                If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                                aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + dPriceClose * nQtyInShares
                            End If
                        Else
                            If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                            aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aTrd.price * nQtyInShares
                        End If
                    End If
                Else
                    If aPos.QtyLTDSell = BAD_LONG_VALUE Then aPos.QtyLTDSell = 0
                    aPos.QtyLTDSell = aPos.QtyLTDSell + nQtyInShares
                    If aTrd.IsPosition Then
                        If dPriceClose >= DBL_EPSILON Then
                            If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
                            aPos.PosLTDSell = aPos.PosLTDSell + dPriceClose * nQtyInShares
                        End If
                    Else
                        If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
                        aPos.PosLTDSell = aPos.PosLTDSell + aTrd.price * nQtyInShares
                    End If
                
                    If aTrd.TradeDate < Date Then
                        If aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateSell = 0
                        aPos.QtyDailyPrevDateSell = aPos.QtyDailyPrevDateSell + nQtyInShares
                        If dPriceClose >= DBL_EPSILON Then
                            If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                            aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + dPriceClose * nQtyInShares
                        ElseIf Not aTrd.IsPosition Then
                            If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                            aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aTrd.price * nQtyInShares
                        End If
                    Else
                        If aPos.QtyDailyTodaySell = BAD_LONG_VALUE Then aPos.QtyDailyTodaySell = 0
                        aPos.QtyDailyTodaySell = aPos.QtyDailyTodaySell + nQtyInShares
                        If aTrd.IsPosition Then
                            If dPriceClose >= DBL_EPSILON Then
                                If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                                aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + dPriceClose * nQtyInShares
                            End If
                        Else
                            If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                            aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aTrd.price * nQtyInShares
                        End If
                    End If
                End If
            
                aPos.Qty = aPos.Qty + nQty
                aPos.QtyInShares = aPos.QtyInShares + nQtyInShares
                
                If aPos.ContractType = enCtOption Then
                    If aUnd.OptQty = BAD_LONG_VALUE Then aUnd.OptQty = 0
                    aUnd.OptQty = aUnd.OptQty + nQty
                Else
                    If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
                    aUnd.Qty = aUnd.Qty + nQtyInShares
                End If
                
                Set aPos = Nothing
                Set aUnd = Nothing
            End If
            
            DoEvents
            If Not m_bDataLoad Then Exit For
            
            IncProgress pbProgress
            
            Set aTrd = Nothing
        Next
        
        If m_bDataLoad Then
            nCount = collExp.Count
            If nCount > 0 Then
                If nCount > 1 Then
                    SortArray arrExp, 1, nCount
                End If
                For i = 1 To nCount
                    m_Exp.Add CStr(i), collExp(CStr(arrExp(i)))
                Next
            End If
        End If
        
        UnderlyingsAdjustRates
        
        PositionsLoad = m_bDataLoad
    End If

    m_bInProc = False
Ex:
    On Error Resume Next
    m_bDataLoad = False
    Erase arrExp
    Set collExp = Nothing
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
    GoTo Ex
End Function

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, bFinished As Boolean
    
    bFinished = False
    
    If Not Request Is Nothing And ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                sKey = CStr(IIf(Request.Type = enOPT, enCtOption, IIf(Request.Type = enSTK, enCtStock, enCtIndex))) & "_" & Request.Symbol
                
                Set aReq = m_QuoteRequests(sKey)
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                
                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                                Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
                End If
            ElseIf PpIsGroupReqType(Request.Type) Then
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                AdjustState
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequest Then
                    m_GroupPriceProvider.CancelGroup
                End If
                
                CalcMatrix
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
                
            End If
        Else
            Debug.Assert False
        End If
    
    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, dPriceBid#, dPriceAsk#, dPriceLast#, nLotSize&
'    Dim  dPriceClose#
    Dim bFinished As Boolean
    bFinished = False
    
    If PpIsNonGroupReqType(Params.Type) Then
        sKey = CStr(IIf(Params.Type = enOPT, enCtOption, IIf(Params.Type = enSTK, enCtStock, enCtIndex))) & "_" & Params.Symbol
        Set aReq = m_QuoteRequests(sKey)
        If Not aReq Is Nothing Then
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
'            dPriceClose = Results.ClosePrice
            
            If Results.LotSize > 0 Then
                nLotSize = Results.LotSize
            Else
                If Params.Type = enOPT Then
                    nLotSize = 100
                Else
                    nLotSize = 1
                End If
            End If
            
            If Not aReq.IndexOnly Then
                If Not aReq.Pos Is Nothing Then
                    If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Bid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Ask = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Last = dPriceLast
                    
'                    If aReq.Pos.PriceClose <= 0 Then
'                        If dPriceClose > BAD_DOUBLE_VALUE Then aReq.Pos.PriceClose = dPriceClose
'                    End If
                    
                    If aReq.Pos.Quote.LotSize <= 0 Then
                        aReq.Pos.Quote.LotSize = nLotSize
                    End If
                End If
                
                If Params.Type <> enOPT Then
                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    
                    If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Und.price.Bid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Und.price.Ask = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Und.price.Last = dPriceLast
                    
'                    If aReq.Und.PriceClose <= 0 Then
'                        If dPriceClose > BAD_DOUBLE_VALUE Then aReq.Und.PriceClose = dPriceClose
'                    End If
                    
                    If aReq.Und.LotSize <= 0 Then
                        aReq.Und.LotSize = nLotSize
                    End If
                
'                    aReq.Und.VolaSrv.UnderlyingPrice = PriceMidEx(aReq.Und.PriceBid, aReq.Und.PriceBid, aReq.Und.PriceLast)
                    Debug.Assert (Not aReq.Und.UndPriceProfile Is Nothing)
                    aReq.Und.VolaSrv.UnderlyingPrice = aReq.Und.UndPriceProfile.GetUndPriceMid(aReq.Und.price.Bid, aReq.Und.price.Bid, aReq.Und.price.Last, _
                        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                        
                    If m_Idx.ID = aReq.Und.ID Then
                        If dPriceBid > BAD_DOUBLE_VALUE Then m_Idx.price.Bid = dPriceBid
                        If dPriceAsk > BAD_DOUBLE_VALUE Then m_Idx.price.Ask = dPriceAsk
                        If dPriceLast > BAD_DOUBLE_VALUE Then m_Idx.price.Last = dPriceLast
                    End If
                End If
            Else
                Debug.Assert m_Idx.ID = aReq.Und.ID
                If m_Idx.ID = aReq.Und.ID Then
                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1

                    If dPriceBid > BAD_DOUBLE_VALUE Then m_Idx.price.Bid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then m_Idx.price.Ask = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then m_Idx.price.Last = dPriceLast
                End If
            End If
        
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            Set aReq = Nothing
        
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                        Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
        End If
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If
    
    If bFinished Then
        m_bLastQuoteReqNow = False
        AdjustState
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        
        CalcMatrix

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        
    End If
End Sub

Private Sub RequestLastQuotes(ByVal bIndexOnly As Boolean)
    On Error GoTo EH
    Dim i&, nCount&, aIdxReq As EtsMmRisksLib.MmRvReqAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aReq As EtsMmRisksLib.MmRvReqAtom
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        nCount = 0
        
        m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.MatrixReqTypeAlwaysNonGroup
        
        If m_Idx.ID > 0 Then
            Set aIdxReq = m_QuoteRequests(CStr(enCtIndex) & "_" & m_Idx.Symbol)
        End If
        
        If Not bIndexOnly Then
            nCount = m_QuoteRequests.Count
            
        ElseIf Not aIdxReq Is Nothing Then
            If aIdxReq.IndexOnly Then
                nCount = 1
            End If
        End If
        
        If nCount > 0 Then
            m_bInProc = True
            m_bLastQuoteReqNow = True
            AdjustState
            
            m_nLastQuoteReqCount = nCount
            m_nLastQuoteReqDone = 0&
            
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nLastQuoteReqCount

            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents
            
            If Not m_bGroupRequest Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                
                aParam.Type = enMStart
                PriceProvider.RequestLastQuote aParam
                
                If Not bIndexOnly Then
                    For Each aReq In m_QuoteRequests
                        If Not m_bLastQuoteReqNow Then Exit For
                        PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                        DoEvents
                    Next
                Else
                    If Not aIdxReq Is Nothing Then
                        PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        Set aIdxReq = Nothing
                    End If
                End If
                
                aParam.Type = enMStop
                PriceProvider.RequestLastQuote aParam
            Else
                If Not bIndexOnly Then
                    m_nLastQuoteGroupReqCount = m_Und.Count * 2
                    m_nLastQuoteGroupReqDone = 0&
                
                    If Not aIdxReq Is Nothing Then
                        If aIdxReq.IndexOnly Then
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                            PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        End If
                    End If
                
                    For Each aUnd In m_Und
                        If Not m_bLastQuoteReqNow Then Exit For
                        aParam.Symbol = aUnd.Symbol
                        aParam.Exchange = ""
                        
                        aParam.Type = IIf(aUnd.ContractType = enCtStock, enSTK, enIDX)
                        PriceProvider.RequestLastQuote aParam
                        
                        If aUnd.HasOptPos Then
                            aParam.Type = IIf(aUnd.ContractType = enCtStock, enGrSTK, enGrIDX)
                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                        Else
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount - 1
                        End If
                        
                        DoEvents
                    Next
                Else
                    If Not aIdxReq Is Nothing Then
                        m_nLastQuoteGroupReqCount = 1
                        m_nLastQuoteGroupReqDone = 0&
                        PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        Set aIdxReq = Nothing
                    End If
                End If
            End If
            
            m_bInProc = False
            AdjustState
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    Set aUnd = Nothing
    Set aIdxReq = Nothing
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
    
    PriceProvider.CancelLastQuote
    If m_bGroupRequest Then
        m_GroupPriceProvider.CancelGroup
    End If
End Sub

Private Sub CalcMatrix()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, bCorrelatedShift As Boolean, nModel As EtsGeneralLib.EtsCalcModelTypeEnum
    Dim nLastX&, nLastY&, nX&, nY&, nUndCount&, i&, nRow&, nMask&, nBadForeColor&, nForeColor&
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    
    nUndCount = m_Und.Count
    If m_bInProc Or m_bRecalc Then Exit Sub
    
    
    ClearValGrid False
    SetRefreshHint False
    
    If nUndCount <= 0 Then Exit Sub
    
    nLastX = UBound(m_Res, 1)
    nLastY = UBound(m_Res, 2)
    Debug.Assert nLastX >= 0 And nLastY >= 0
    
    If nLastX < 0 Or nLastY < 0 Then Exit Sub
    
    m_bInProc = True
    m_bRecalc = True
    AdjustState

    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 1
    pbProgress.Visible = True
    lblProcess.Caption = "Calculating Greeks..."
    lblProcess.Visible = True
    lblProcess.Refresh

    pbProgress.Max = nUndCount * (UBound(m_Res, 1) + 1) * (m_Exp.Count + 1)
    
    nMask = GM_NONE Or GM_THEOPRICE Or GM_DELTA Or GM_GAMMA Or GM_THETA Or GM_VEGA Or GM_RHO
    
    bCorrelatedShift = False '(m_nFilter(MFC_SHIFT) = 1)
    If bCorrelatedShift Then
        For Each aUnd In m_Und
            If Not aUnd.HasSynthetic Then
                If IsBadDouble(aUnd.Beta) Or aUnd.Beta = 0# Then
                    bCorrelatedShift = False
                    Set aUnd = Nothing
                    Exit For
                End If
            Else
                For Each aPos In aUnd.Pos
                    If aPos.IsSynthetic Then
                            Dim sRoot As SynthRootAtom
                            Set sRoot = aUnd.SynthRoots(aPos.OptionRootID)
                            If Not sRoot Is Nothing Then
                                Dim sRootComp As SynthRootCompAtom
                                 For Each sRootComp In sRoot.SynthRootComponents
                                    If Not IsBadDouble(sRootComp.Weight) And sRootComp.Weight > 0# Then
                                        If Not g_Index(m_Idx.ID).HaveComponentBetas Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                        Dim IdxComp As IndexCompAtom
                                        Set IdxComp = g_Index(m_Idx.ID).Components(sRootComp.UndID)
                                        If IdxComp Is Nothing Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                        If IsBadDouble(IdxComp.Beta) Or IdxComp.Beta = 0# Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                    End If
                                 Next
                            Else
                                bCorrelatedShift = False
                                GoTo ExitFor
                            End If
                    Else
                        If IsBadDouble(aUnd.Beta) Or aUnd.Beta = 0# Then
                            bCorrelatedShift = False
                            GoTo ExitFor
                        End If
                    End If
                Next
            End If
        Next
ExitFor:
    End If

    If m_enUsedVolaType = PVT_HIST Then
        Dim rs As ADODB.Recordset
        Set rs = gDBW.usp_Underlying20dHVola_Get(m_Und(m_nCurUndID).Symbol)
        If rs.RecordCount > 0 Then
            m_dUndHVola = ReadDbl(rs!hv20_value)
        Else
            m_dUndHVola = 0#
        End If
        Set rs = Nothing
    End If

    nModel = g_Params.MatrixCalcModel
    
    For Each aUnd In m_Und
        CalcPosition aUnd, nLastX, nLastY, nMask, bCorrelatedShift, nModel
        DoEvents
        If Not m_bRecalc Then Exit For
    Next
  
    Dim nCol&, nValCol&
    Dim nPriceRow&, nDeltaRow&, nGammaRow&, nVegaRow&, nBackColor&
    
    With fgVal
        nPriceRow = 0
        nBackColor = &HFFFFFF
        .Cols = m_nLabelCount + 1

        .AddItem ""
        .TextMatrix(nPriceRow, 0) = "Price"
        For nCol = 1 To .Cols - 1
            .TextMatrix(nPriceRow, nCol) = Format(m_dCurUndMidPrice + m_Res((nCol - 1) * CNT_COEF, 0).ShiftX, "#,##0.00")
        Next
        .Cell(flexcpFontBold, nPriceRow, 1, nPriceRow, .Cols - 1) = True

        ' Fill projection table with values
        nValueRows = 4 'values that displayed in the table (p&l, delta, gamma etc)
        For i = 0 To 2
            If UBound(m_Res, 2) >= i Then
                With fgVal
                    .rows = .rows + nValueRows
                    nPnLRow = nValueRows * i + 1
                    nDeltaRow = nValueRows * i + 2
                    nGammaRow = nValueRows * i + 3
                    nVegaRow = nValueRows * i + 4
                    
                    .TextMatrix(nPnLRow, 0) = "PnL"
                    .TextMatrix(nDeltaRow, 0) = "Delta"
                    .TextMatrix(nGammaRow, 0) = "$Gamma"
                    .TextMatrix(nVegaRow, 0) = "Vega $Exp"
                    
                    For nCol = 1 To .Cols - 1
                        .ColDataType(nCol) = flexDTDouble
                        nValCol = (nCol - 1) * CNT_COEF
                        
                        .TextMatrix(nPnLRow, nCol) = IIf(m_Res(nValCol, i).PnL > BAD_DOUBLE_VALUE, Format(Round(m_Res(nValCol, i).PnL), "#,##0"), STR_NA)
                        .TextMatrix(nDeltaRow, nCol) = IIf(m_Res(nValCol, i).NetDelta > BAD_DOUBLE_VALUE, Format(Round(m_Res(nValCol, i).NetDelta), "#,##0"), STR_NA)
                        .TextMatrix(nGammaRow, nCol) = IIf(m_Res(nValCol, i).NetGamma > BAD_DOUBLE_VALUE, Format(Round(m_Res(nValCol, i).NetGamma), "#,##0"), STR_NA)
                        .TextMatrix(nVegaRow, nCol) = IIf(m_Res(nValCol, i).Vega > BAD_DOUBLE_VALUE, Format(Round(m_Res(nValCol, i).Vega), "#,##0"), STR_NA)
                    Next
                    
                End With
                
                nBackColor = IIf(nBackColor = &HEEEEEE, &HFFFFFF, &HEEEEEE)
                .Cell(flexcpBackColor, nPnLRow, 0, nPnLRow + nValueRows - 1, .Cols - 1) = nBackColor
            End If
        Next i
        
        .Cell(flexcpBackColor, 0, m_nLabelCount \ 2 + 1, .rows - 1) = &HEEEEEE
        .AutoSize 1, .Cols - 1
    End With
    
    tmrFill.Enabled = True
    
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    m_bInProc = False
    m_bRecalc = False
  
    AdjustState
End Sub

Private Sub ClearValGrid(ByVal bAutosize As Boolean)
    On Error Resume Next
    InitValGrids
    InitResults
End Sub

Private Sub InitResults()
    On Error Resume Next
    Dim nLastX&, nLastY&, nX&, nY&, dValue#, dShift#
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim dHorStep#
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    nLastX = (m_nLabelCount - 1) * CNT_COEF
    nLastY = IIf(m_Exp.Count > 2, 2, m_Exp.Count)
    Debug.Assert nLastX >= 0 And nLastY >= 0
    
    m_dCurUndMidPrice = Int(m_Und(m_nCurUndID).UndPriceProfile.GetUndPriceMid(m_Und(m_nCurUndID).price.Bid, m_Und(m_nCurUndID).price.Ask, m_Und(m_nCurUndID).price.Last, dToleranceValue, enRoundingRule) * 100) / 100
    
    m_dHorStep = (2 * m_dCurUndMidPrice * m_nChartWidth / 100) / ((m_nLabelCount - 1) * CNT_COEF)
    'm_dHorStep = Round(m_dHorStep, 2)
    m_dHorStep = Int(m_dHorStep * 100) / 100
    
    Erase m_Res
    ReDim m_Res(0 To nLastX, 0 To nLastY)
    
    m_BasePoint(RMA_HORZ) = nLastX \ 2
    m_BasePoint(RMA_VERT) = 1
    
    
    For nX = 0 To nLastX
        dShift = (nX - nLastX \ 2) * m_dHorStep
        
        For nY = 0 To nLastY
            m_Res(nX, nY).ShiftX = dShift
            
            m_Res(nX, nY).PnL = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Delta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).NetDelta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Gamma = BAD_DOUBLE_VALUE
            m_Res(nX, nY).GammaPerc = BAD_DOUBLE_VALUE
            m_Res(nX, nY).NetGamma = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Theta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Vega = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Rho = BAD_DOUBLE_VALUE
            m_Res(nX, nY).WtdVega = BAD_DOUBLE_VALUE
        
            m_Res(nX, nY).BadPnL = False
            m_Res(nX, nY).BadDelta = False
            m_Res(nX, nY).BadNetDelta = False
            m_Res(nX, nY).BadGamma = False
            m_Res(nX, nY).BadGammaPerc = False
            m_Res(nX, nY).BadNetGamma = False
            m_Res(nX, nY).BadTheta = False
            m_Res(nX, nY).BadVega = False
            m_Res(nX, nY).BadRho = False
            m_Res(nX, nY).BadWtdVega = False
        Next
    Next
    
    For nY = 0 To nLastY
        If nY <> 0 Then
            dShift = CDate(m_Exp(nY).Name) - Date
        Else
            dShift = fgExp.ValueMatrix(1, 1)
        End If
        
        For nX = 0 To nLastX
            m_Res(nX, nY).ShiftY = dShift
        Next
    Next
    
    Dim nUndID As Long
    nUndID = m_nCurUndID
    
    If m_Und.Count = 1 Then
        Debug.Assert (Not m_Und(1).UndPriceProfile Is Nothing)
        dValue = m_Und(1).UndPriceProfile.GetUndPriceMid(m_Und(1).price.Bid, m_Und(1).price.Ask, m_Und(1).price.Last, dToleranceValue, enRoundingRule)
        
        If dValue > 0# Then
            dShift = 0
            
            For nX = 0 To nLastX
                If dValue + m_Res(nX, 0).ShiftX < 0 Then
                    dShift = dShift + HOR_STEP
                    m_BasePoint(RMA_HORZ) = m_BasePoint(RMA_HORZ) - 1
                Else
                    Exit For
                End If
            Next
            
            If dShift > 0 Then
                For nX = 0 To nLastX
                    For nY = 0 To nLastY
                        m_Res(nX, nY).ShiftX = m_Res(nX, nY).ShiftX + dShift
                    Next
                Next
            End If
        End If
    End If
End Sub


Private Sub CalcPosition(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal nLastX As Long, ByVal nLastY As Long, ByVal nGreeksMask As Long, _
                        ByVal bCorrelatedShift As Boolean, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum)
    On Error Resume Next
    Dim dtToday As Date, dtNow As Date, nX&, nY&, aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aGreeks As GreeksData, dUndSpotBase#, dUndSpot#, dUndBidBase#, dUndBid#, dUndAskBase#, dUndAsk#, dOptMidPrice#
    Dim dSynthUndSpotBase#, dSynthUndSpot#, dSynthUndBidBase#, dSynthUndBid#
    Dim dSynthUndAskBase#, dSynthUndAsk#, dSynthUndLastBase#, nSynthOptRootID&
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom

    nSynthOptRootID = BAD_LONG_VALUE
    
    Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
    
    If m_bDataEditedByUser Then
        dUndSpotBase = fgUnd.TextMatrix(1, PUC_PRICE)
    Else
        dUndSpotBase = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
            g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    End If
    dUndBidBase = aUnd.UndPriceProfile.GetUndPriceBidForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    dUndAskBase = aUnd.UndPriceProfile.GetUndPriceAskForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    
    dUndSpot = dUndSpotBase: dUndBid = dUndBidBase: dUndAsk = dUndAskBase
    
'    If dUndSpotBase > 0# Or aUnd.HasSynthetic Then
        dtToday = GetNewYorkTime
        dtNow = dtToday
    
        For nX = 0 To nLastX
            nY = 0
            
            If dUndSpotBase > 0# Then
                dUndSpot = dUndSpotBase: dUndBid = dUndBidBase: dUndAsk = dUndAskBase
                ShiftSpot aUnd.Beta, RMUT_ABS, m_Res(nX, nY).ShiftX, bCorrelatedShift, dUndSpot, dUndBid, dUndAsk
            End If
            
            For nY = 0 To nLastY
                dtToday = DateAdd("d", m_Res(nX, nY).ShiftY, dtNow)

                Dim dOptionPnL#
                For Each aPos In aUnd.Pos
                    If CheckPosFilter(aPos) Then
                        If aPos.ContractType = enCtOption Then
                            dOptionPnL = BAD_DOUBLE_VALUE
                            If dUndSpot > 0# Or aUnd.HasSynthetic Then
                                    If Not aPos.IsSynthetic Then
                                        
                                        If dUndSpot <= 0# Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If
                                        
                                        If aPos.ExpiryOV > dtToday Then
                                            
                                            aGreeks.nMask = nGreeksMask
                                            If CalcGreeksCommon(aUnd, aPos, dtToday, m_Res(nX, nY), aGreeks, dUndSpot, dUndSpotBase, dOptMidPrice, nModel) Then
                                                
                                                dOptionPnL = (aGreeks.dTheoPrice - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                
                                                If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                    If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).PnL = 0#
                                                    m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                Else
                                                    m_Res(nX, nY).BadPnL = True
                                                End If
                                                
                                                CalcPosTotalsCommon aPos, aGreeks, m_Res(nX, nY), dUndSpot
                                            
                                            Else
                                                SetResultsBadStatus m_Res(nX, nY)
                                            End If
                                        Else
                                            CalcTheoPnLCommonExerc aPos, m_Res(nX, nY), dUndSpot, dUndBid, dUndAsk
                                            
                                            If m_bDataEditedByUser Then
                                                Dim nPosRow As Long
                                                nPosRow = fgOpt.FindRow(aPos.Symbol, , POC_SYMBOL)
                                                dOptMidPrice = fgOpt.ValueMatrix(nPosRow, POC_PRICE)
                                            Else
                                                dOptMidPrice = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0#)
                                            End If

                                            If aPos.OptType = enOtCall Then
                                                'call
                                                If aPos.Strike <= dUndSpot Then
                                                    dOptionPnL = ((dUndSpot - aPos.Strike) - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                Else
                                                    dOptionPnL = -dOptMidPrice * aPos.Qty * aPos.Quote.LotSize
                                                End If
                                                
                                                If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                    If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).PnL = 0#
                                                    m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                Else
                                                    m_Res(nX, nY).BadPnL = True
                                                End If
                                            Else
                                                'put
                                                If aPos.Strike >= dUndSpot Then
                                                    dOptionPnL = ((aPos.Strike - dUndSpot) - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                Else
                                                    dOptionPnL = -dOptMidPrice * aPos.Qty * aPos.Quote.LotSize
                                                End If
                                                
                                                If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                    If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).PnL = 0#
                                                    m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                Else
                                                    m_Res(nX, nY).BadPnL = True
                                                End If
                                            End If
                                        End If
                                    Else ' synthetic position
                                    
                                        If nSynthOptRootID <> aPos.OptionRootID Then
                                            
                                            If Not aUnd.SynthRoots Is Nothing Then Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
                                            
                                            aUnd.GetSyntheticUnderlyingPrice m_Und, aUnd.SynthRoots(aPos.OptionRootID), dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase
                                            
                                            If m_bDataEditedByUser Then
                                                dSynthUndSpotBase = fgUnd.TextMatrix(1, PUC_PRICE)
                                            Else
                                                dSynthUndSpotBase = aUnd.UndPriceProfile.GetUndPriceMid(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                    g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                                            End If
                                            dSynthUndBidBase = aUnd.UndPriceProfile.GetUndPriceBidForPnL(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                                            dSynthUndAskBase = aUnd.UndPriceProfile.GetUndPriceAskForPnL(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                                            
                                            dSynthUndSpot = dSynthUndSpotBase: dSynthUndBid = dSynthUndBidBase: dSynthUndAsk = dSynthUndAskBase
                                            
                                            nSynthOptRootID = aPos.OptionRootID
                                        End If
                                        
                                        If aSynthRoot Is Nothing Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If
                                        
                                        dSynthUndSpot = dSynthUndSpotBase: dSynthUndBid = dSynthUndBidBase: dSynthUndAsk = dSynthUndAskBase
                                        ShiftSyntSpot aUnd.SynthRoots(aPos.OptionRootID), RMUT_ABS, m_Res(nX, nY).ShiftX, _
                                                    bCorrelatedShift, dSynthUndSpot, dSynthUndBid, dSynthUndAsk
                                        
                                        If dSynthUndSpot <= 0# Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If
                                                                                                            
                                            If aPos.Expiry > dtToday Then
                                                
                                                aGreeks.nMask = nGreeksMask
                                                If CalcGreeksSynth(aUnd, aPos, dtToday, m_Res(nX, nY), aGreeks, aSynthRoot, dSynthUndSpot, dSynthUndSpotBase, dOptMidPrice, nModel) Then
                                                    
                                                    dOptionPnL = (aGreeks.dTheoPrice - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                    
                                                    If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                        If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then _
                                                            m_Res(nX, nY).PnL = 0#
                                                        m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                    Else
                                                        m_Res(nX, nY).BadPnL = True
                                                    End If
                                                    
                                                    CalcPosTotalsSynth aPos, aGreeks, m_Res(nX, nY), aSynthRoot, dSynthUndSpot, dSynthUndSpotBase
                                                
                                                Else
                                                    SetResultsBadStatus m_Res(nX, nY)
                                                End If
                                            Else
                                                CalcTheoPnLCommonExerc aPos, m_Res(nX, nY), dUndSpot, dUndBid, dUndAsk
                                                
                                                If m_bDataEditedByUser Then
                                                    nPosRow = fgOpt.FindRow(aPos.Symbol, 1, POC_SYMBOL)
                                                    dOptMidPrice = CDbl(fgOpt.TextMatrix(nPosRow, POC_PRICE))
                                                Else
                                                    dOptMidPrice = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0#)
                                                End If
                                                
                                                If aPos.Qty > 0 Then
                                                    'call
                                                    If aPos.Strike <= dUndSpot Then
                                                        dOptionPnL = ((dUndSpot - aPos.Strike) - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                    Else
                                                        dOptionPnL = -dOptMidPrice * aPos.Qty * aPos.Quote.LotSize
                                                    End If
                                                    
                                                    If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                        If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then _
                                                            m_Res(nX, nY).PnL = 0#
                                                        m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                    Else
                                                        m_Res(nX, nY).BadPnL = True
                                                    End If
                                                Else
                                                    'put
                                                    If aPos.Strike >= dUndSpot Then
                                                        dOptionPnL = ((dUndSpot - aPos.Strike) - dOptMidPrice) * aPos.Qty * aPos.Quote.LotSize
                                                    Else
                                                        dOptionPnL = -dOptMidPrice * aPos.Qty * aPos.Quote.LotSize
                                                    End If
                                                    
                                                    If dOptionPnL > BAD_DOUBLE_VALUE Then
                                                        If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then _
                                                            m_Res(nX, nY).PnL = 0#
                                                        m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dOptionPnL
                                                    Else
                                                        m_Res(nX, nY).BadPnL = True
                                                    End If
                                                End If
                                            End If
                                    End If
                            End If
                        Else
                                                        
                            Dim dStockPnL As Double
                            dStockPnL = BAD_DOUBLE_VALUE
                            
                            dStockPnL = (dUndSpot - dUndSpotBase) * aPos.Qty * aPos.Quote.LotSize
                            
                            If dStockPnL > BAD_DOUBLE_VALUE Then
                                If m_Res(nX, nY).PnL <= BAD_DOUBLE_VALUE Then _
                                    m_Res(nX, nY).PnL = 0#
                                m_Res(nX, nY).PnL = m_Res(nX, nY).PnL + dStockPnL
                            Else
                                m_Res(nX, nY).BadPnL = True
                            End If
                            
                            If m_Res(nX, nY).Delta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).Delta = 0#
                            If m_Res(nX, nY).NetDelta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).NetDelta = 0#
                            m_Res(nX, nY).Delta = m_Res(nX, nY).Delta + aPos.QtyInShares * dUndSpot
                            m_Res(nX, nY).NetDelta = m_Res(nX, nY).NetDelta + aPos.QtyInShares
                        End If
                
NextPos:
                            DoEvents
                            If Not m_bRecalc Then Exit Sub
                    End If
                Next
            Next
        Next
End Sub


Private Sub ShiftSpot(ByVal dBeta#, ByVal enUnits As RmUnitTypeEnum, _
                    ByVal dShift#, ByVal bCorrelatedShift As Boolean, _
                    ByRef dUndSpot#, ByRef dUndBid#, ByRef dUndAsk#)
    On Error Resume Next
    Dim dBetaFactor#
    
    dUndSpot = dUndSpot + dShift
    If dUndBid > 0# Then dUndBid = dUndBid + dShift
    If dUndAsk > 0# Then dUndAsk = dUndAsk + dShift
End Sub

Private Function CheckPosFilter(ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    bMatched = True
    CheckPosFilter = bMatched
End Function

Private Sub SetResultsBadStatus(ByRef aRes As MatrixCalcResultType)
    On Error Resume Next
    aRes.BadPnL = True
    aRes.BadDelta = True
    aRes.BadNetDelta = True
    aRes.BadGamma = True
    aRes.BadGammaPerc = True
    aRes.BadNetGamma = True
    aRes.BadTheta = True
    aRes.BadVega = True
    aRes.BadRho = True
    aRes.BadWtdVega = True
End Sub

Private Function CalcGreeksCommon(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dtToday As Date, _
                                ByRef aRes As MatrixCalcResultType, ByRef aGreeks As GreeksData, _
                                ByRef dUndSpot#, ByRef dUndSpotBase#, ByRef dOptMidPrice#, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum) As Boolean
    On Error Resume Next
    Dim nDivCount&, nBaskDivCount&, dYield#, dVola#, dOptSpot#, nIsAmerican&
    Dim dDivDte() As Double, dDivAmts() As Double, aBaskDivs() As REGULAR_DIVIDENDS
    Dim nFlag&
    Dim bIsBasket As Boolean
    Dim aBasketDiv As EtsGeneralLib.EtsIndexDivColl
    Dim enDivType As EtsGeneralLib.EtsDivTypeEnum
    Dim lSurfaceID As Long

    Dim dYte As Double
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    Dim dtNow As Date
        
    dtNow = dtToday
    GetCalcParams dtNow, aPos.ExpiryOV, aPos.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
    
    nDivCount = 0
    ReDim dDivDte(0 To 0)
    ReDim dDivAmts(0 To 0)
    
    nIsAmerican = IIf(aUnd.IsAmerican, 1, 0)
    CalcGreeksCommon = False
    
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Set aDiv = aUnd.Dividend
    
    enDivType = enDivCustomStream
    If (Not aDiv Is Nothing) Then
        enDivType = aDiv.DivType
    End If
                
    Select Case enDivType
        Case enDivMarket, enDivCustomPeriodical, enDivCustomStream
            If Not aDiv Is Nothing Then
                aDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                If nDivCount > 0 Then
                    aDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                End If
                Set aDiv = Nothing
            End If
        Case enDivStockBasket
            If Not aUnd.BasketIndex Is Nothing Then
                Set aBasketDivs = aUnd.BasketIndex.BasketDivs
                    If Not aBasketDivs Is Nothing Then
                        aBasketDivs.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                        If nDivCount > 0 Then
                            aBasketDivs.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                        End If
                    End If
                    Set aBasketDivs = Nothing
            End If
        Case enDivIndexYield
            dYield = aUnd.Yield
    End Select
    
   
    Dim nRow&
    dVola = 0#

    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aPos.OptionRootID)
    
    If m_bDataEditedByUser Then
        dVola = aPos.Quote.Vola
    Else
    Select Case m_enUsedVolaType
        Case PVT_THEO:
            dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            aPos.Quote.Vola = dVola
        Case PVT_BIDIMPLD:
            If (aPos.Quote.price.Bid <> BAD_DOUBLE_VALUE) Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, aUnd.price.Last, aPos.Quote.price.Bid, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            ElseIf (aPos.Quote.price.Ask <> BAD_DOUBLE_VALUE) Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, aUnd.price.Last, aPos.Quote.price.Ask / 2, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            Else
                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            End If
            aPos.Quote.Vola = dVola
        Case PVT_HIST:
            dVola = m_dUndHVola
            aPos.Quote.Vola = dVola
        Case PVT_MRKIMPLD:
            If m_bDataEditedByUser Then
                nRow = fgOpt.FindRow(aPos.Symbol, 1, POC_SYMBOL)
                dOptSpot = CDbl(fgOpt.TextMatrix(nRow, POC_PRICE))
            Else
                dOptSpot = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, False, 0#)
            End If
            
            If dOptSpot > 0# Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, dUndSpotBase, dOptSpot, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            Else
                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            End If
            
            aPos.Quote.Vola = dVola
    End Select
    End If
    
    If dVola > 0# Then
        ShiftVola aRes, aPos.VegaWeight, dVola
        
        Dim RetCount
        RetCount = CalcGreeksMM2(aPos.Rate, dYield, BAD_DOUBLE_VALUE, dUndSpot, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aUnd.Skew, aUnd.Kurt, nModel, aGreeks)
            
        If RetCount <> 0 Then
            FixGreeks aGreeks
            CalcGreeksCommon = True
        End If
    End If
    
    If m_bDataEditedByUser Then
        Dim nPosRow As Long
        nPosRow = fgOpt.FindRow(aPos.Symbol, 1, POC_SYMBOL)
        dOptMidPrice = CDbl(fgOpt.TextMatrix(nPosRow, POC_PRICE))
    Else
        dOptMidPrice = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0#)
    End If

    Erase dDivDte
    Erase dDivAmts
End Function


Private Sub ShiftVola(ByRef aRes As MatrixCalcResultType, ByVal dVegaWeight#, ByRef dVola#)
    On Error Resume Next
    If dVola <= 0# Then dVola = 0.001
End Sub


Private Sub CalcTheoPnLCommon(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, _
                                    ByRef aRes As MatrixCalcResultType, ByVal dtToday As Date)
    On Error Resume Next
    Dim dPnlTheo#: dPnlTheo = BAD_DOUBLE_VALUE
    Dim bBadPnl As Boolean: bBadPnl = False
    
    If (aGreeks.nMask And GM_THEOPRICE) = GM_THEOPRICE Then
        If aGreeks.dTheoPrice >= 0 Then 'DBL_EPSILON Then
            If g_Params.PnLCalcType = PNLCT_LTD Or dtToday <> Date Then
                If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                    If aPos.PosLTDBuy > BAD_DOUBLE_VALUE Then
                        dPnlTheo = aGreeks.dTheoPrice * aPos.QtyLTDBuy - aPos.PosLTDBuy
                    Else
                        bBadPnl = False
                    End If
                End If
                    
                If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                    If aPos.PosLTDSell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyLTDSell - aPos.PosLTDSell
                    Else
                        bBadPnl = False
                    End If
                End If
            Else
                If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                    If aPos.PosDailyPrevDateBuy > BAD_DOUBLE_VALUE Then
                        dPnlTheo = aGreeks.dTheoPrice * aPos.QtyDailyPrevDateBuy - aPos.PosDailyPrevDateBuy
                    Else
                        bBadPnl = False
                    End If
                End If

                If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                    If aPos.PosDailyPrevDateSell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyPrevDateSell - aPos.PosDailyPrevDateSell
                    Else
                        bBadPnl = False
                    End If
                End If

                If Not bBadPnl And aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                    If aPos.PosDailyTodayBuy > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyTodayBuy - aPos.PosDailyTodayBuy
                    Else
                        bBadPnl = False
                    End If
                End If
                    
                If Not bBadPnl And aPos.QtyDailyTodaySell > BAD_LONG_VALUE And aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                    If aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyTodaySell - aPos.PosDailyTodaySell
                    Else
                        bBadPnl = False
                    End If
                End If
            End If
            
            If Not bBadPnl And dPnlTheo > BAD_DOUBLE_VALUE Then
                If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
                aRes.PnL = aRes.PnL + dPnlTheo
            Else
                aRes.BadPnL = True
            End If
        Else
            aRes.BadPnL = True
        End If
    Else
        aRes.BadPnL = True
    End If
End Sub


Private Sub CalcPosTotalsCommon(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, ByRef aRes As MatrixCalcResultType, ByVal dUndSpot#)
    On Error Resume Next
    
    If (aGreeks.nMask And GM_DELTA) = GM_DELTA And Not IsBadDouble(aGreeks.dDelta) Then
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * dUndSpot
        aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares
    Else
        aRes.BadDelta = True
        aRes.BadNetDelta = True
    End If
    
    If (aGreeks.nMask And GM_GAMMA) = GM_GAMMA And Not IsBadDouble(aGreeks.dGamma) Then
        If aRes.Gamma <= BAD_DOUBLE_VALUE Then aRes.Gamma = 0#
        If aRes.NetGamma <= BAD_DOUBLE_VALUE Then aRes.NetGamma = 0#
        If aRes.GammaPerc <= BAD_DOUBLE_VALUE Then aRes.GammaPerc = 0#
        
        aRes.Gamma = aRes.Gamma + aGreeks.dGamma * aPos.QtyInShares * dUndSpot * dUndSpot / 100#
        aRes.NetGamma = aRes.NetGamma + aGreeks.dGamma * aPos.QtyInShares
        aRes.GammaPerc = aRes.GammaPerc + aGreeks.dGamma * aPos.QtyInShares * dUndSpot / 100#
    Else
        aRes.BadGamma = True
        aRes.BadNetGamma = True
        aRes.BadGammaPerc = True
    End If
    
    If (aGreeks.nMask And GM_VEGA) = GM_VEGA And Not IsBadDouble(aGreeks.dVega) Then
        If aRes.Vega <= BAD_DOUBLE_VALUE Then aRes.Vega = 0#
        aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.QtyInShares
    
        If aRes.WtdVega <= BAD_DOUBLE_VALUE Then aRes.WtdVega = 0#
        aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.QtyInShares * aPos.VegaWeight
    Else
        aRes.BadVega = True
        aRes.BadWtdVega = True
    End If
    
    If (aGreeks.nMask And GM_THETA) = GM_THETA And Not IsBadDouble(aGreeks.dTheta) Then
        If aRes.Theta <= BAD_DOUBLE_VALUE Then aRes.Theta = 0#
        aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.QtyInShares
    Else
        aRes.BadTheta = True
    End If
    
    If (aGreeks.nMask And GM_RHO) = GM_RHO And Not IsBadDouble(aGreeks.dRho) Then
        If aRes.Rho <= BAD_DOUBLE_VALUE Then aRes.Rho = 0#
        aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.QtyInShares
    Else
        aRes.BadRho = True
    End If
End Sub

Private Sub ShiftSyntSpot(ByRef aSynthAtom As SynthRootAtom, _
                          ByVal enUnits As RmUnitTypeEnum, _
                          ByVal dShift#, _
                          ByVal bCorrelatedShift As Boolean, _
                          ByRef dUndSpot#, _
                          ByRef dUndBid#, _
                          ByRef dUndAsk#)
    On Error Resume Next
    Dim dBetaFactor#
    Dim bBadAskValue As Boolean: bBadAskValue = False
    Dim bBadBidValue As Boolean: bBadBidValue = False
    Dim bBadSpotValue As Boolean: bBadSpotValue = False
    
    dUndSpot = 0#
    dUndBid = 0#
    dUndAsk = 0#
    
   
    Dim sRootComp As SynthRootCompAtom
    For Each sRootComp In aSynthAtom.SynthRootComponents
        Dim dCompUndSpot#, dUndCompBid#, dUndCompAsk#
        
        Dim aUnd As MmRvUndAtom
        Set aUnd = m_Und(sRootComp.UndID)
        
        dCompUndSpot = aUnd.price.Last
        dUndCompBid = aUnd.price.Bid
        dUndCompAsk = aUnd.price.Ask
        ShiftSpot aUnd.Beta, enUnits, dShift, bCorrelatedShift, dCompUndSpot, dUndCompBid, dUndCompAsk
        
        If Not bBadSpotValue And dCompUndSpot > 0# Then
            dUndSpot = dUndSpot + dCompUndSpot * sRootComp.Weight
        Else
            dUndSpot = BAD_DOUBLE_VALUE
            bBadSpotValue = True
        End If
        
        If Not bBadBidValue And dUndCompBid > 0# Then
            dUndBid = dUndBid + dUndCompBid * sRootComp.Weight
        Else
            dUndBid = BAD_DOUBLE_VALUE
            bBadBidValue = True
        End If
        
        If Not bBadAskValue And dUndCompAsk > 0# Then
            dUndAsk = dUndAsk + dUndCompAsk * sRootComp.Weight
        Else
            dUndAsk = BAD_DOUBLE_VALUE
            bBadAskValue = True
        End If
    Next
    
    If Not bBadSpotValue Then dUndSpot = dUndSpot + aSynthAtom.CashValue
    If Not bBadBidValue Then dUndBid = dUndBid + aSynthAtom.CashValue
    If Not bBadAskValue Then dUndAsk = dUndAsk + aSynthAtom.CashValue
    
    dUndSpot = aUnd.UndPriceProfile.GetUndPriceMid(dUndBid, dUndAsk, dUndSpot, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    
End Sub


Private Function CalcGreeksSynth(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dtToday As Date, _
                                ByRef aRes As MatrixCalcResultType, ByRef aGreeks As GreeksData, ByRef aSynthRoot As EtsGeneralLib.SynthRootAtom, _
                                ByRef dSynthUndSpot#, ByRef dSynthUndSpotBase#, ByRef dOptMidPrice#, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum) As Boolean
    On Error Resume Next
    Dim nDivCount&, RetCount&, nBaskDivCount&, dYield#, dVola#, dOptSpot#, nIsAmerican&
    Dim dDivDte() As Double, dDivAmts() As Double, aBaskDivs() As REGULAR_DIVIDENDS
    Dim nFlag&, dYte As Double
    Dim lSurfaceID As Long

    nDivCount = 0
    ReDim dDivDte(0 To 0)
    ReDim dDivAmts(0 To 0)
    
    Dim dtNow As Date
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    dtNow = dtToday
    GetCalcParams dtNow, aPos.ExpiryOV, aPos.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
    
    nIsAmerican = IIf(aUnd.IsAmerican, 1, 0)
    CalcGreeksSynth = False
    
    If aSynthRoot.Basket Then
            Dim aBasketDiv As EtsGeneralLib.EtsIndexDivColl
            Set aBasketDiv = aSynthRoot.BasketDivs
            If Not aBasketDiv Is Nothing Then
                aBasketDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount
                If nBaskDivCount > 0 Then _
                        aBasketDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount, dDivAmts, dDivDte, nDivCount
            End If
            Set aBasketDiv = Nothing
        Erase aBaskDivs
        If nDivCount <= 0 Then nDivCount = 0
    Else
        dYield = aSynthRoot.Yield
        nDivCount = 0
    End If
    
    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aPos.OptionRootID)
    
    Dim nRow&
    dVola = 0#
    If m_bDataEditedByUser Then
        dVola = aPos.Quote.Vola
    Else
    Select Case m_enUsedVolaType
        Case PVT_THEO:
            dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            aPos.Quote.Vola = dVola
        Case PVT_BIDIMPLD:
            If (aPos.Quote.price.Bid <> BAD_DOUBLE_VALUE) Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, aUnd.price.Last, aPos.Quote.price.Bid, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            ElseIf (aPos.Quote.price.Ask <> BAD_DOUBLE_VALUE) Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, aUnd.price.Last, aPos.Quote.price.Ask / 2, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            Else
                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            End If
            aPos.Quote.Vola = dVola
        Case PVT_HIST:
            dVola = m_dUndHVola
            aPos.Quote.Vola = dVola
        Case PVT_MRKIMPLD:
            If m_bDataEditedByUser Then
                nRow = fgOpt.FindRow(aPos.Symbol, 1, POC_SYMBOL)
                dOptSpot = CDbl(fgOpt.TextMatrix(nRow, POC_PRICE))
            Else
                dOptSpot = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, False, 0#)
            End If
            
            If dOptSpot > 0# Then
                nFlag = VF_OK
                dVola = CalcVolatilityMM3(aPos.Rate, dYield, BAD_DOUBLE_VALUE, dSynthUndSpotBase, dOptSpot, aPos.Strike, dYte, _
                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
                                    100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, nFlag)
                
                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                End If
            Else
                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
            End If
            
            aPos.Quote.Vola = dVola
    End Select
    End If
    
    If dVola > 0# Then
        ShiftVola aRes, aPos.VegaWeight, dVola
        
        RetCount = CalcGreeksMM2(aPos.Rate, dYield, BAD_DOUBLE_VALUE, dSynthUndSpot, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, aGreeks)
        
        If RetCount <> 0 Then
            FixGreeks aGreeks
            CalcGreeksSynth = True
        End If
    End If
    
    If m_bDataEditedByUser Then
        Dim nPosRow As Long
        nPosRow = fgOpt.FindRow(aPos.Symbol, 1, POC_SYMBOL)
        dOptMidPrice = CDbl(fgOpt.TextMatrix(nPosRow, POC_PRICE))
    Else
        dOptMidPrice = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, False, 0#)
        If dOptMidPrice = 0 Then dOptMidPrice = aGreeks.dTheoPrice
    End If

    Erase dDivDte
    Erase dDivAmts
End Function


Private Sub CalcPosTotalsSynth(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, ByRef aRes As MatrixCalcResultType, _
                                aSynthRoot As EtsGeneralLib.SynthRootAtom, ByVal dSynthUndSpot#, ByVal dSynthUndSpotBase#)
    On Error Resume Next
    Dim dTmp#, aSynthRootComp As EtsGeneralLib.SynthRootCompAtom, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    If (aGreeks.nMask And GM_DELTA) = GM_DELTA And Not IsBadDouble(aGreeks.dDelta) Then
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
            Set aSynthUnd = m_Und(aSynthRootComp.UndID)
            If Not aSynthUnd Is Nothing Then
'                dTmp = PriceMidEx(aSynthUnd.PriceBid, aSynthUnd.PriceAsk, aSynthUnd.PriceLast, g_Params.UseLastPriceForCalcs)
                Debug.Assert (Not aSynthUnd.UndPriceProfile Is Nothing)
                dTmp = aSynthUnd.UndPriceProfile.GetUndPriceMid(aSynthUnd.price.Bid, aSynthUnd.price.Ask, aSynthUnd.price.Last, dToleranceValue, enRoundingRule)
                If Not IsBadDouble(dTmp) And dTmp > 0# Then
                    dTmp = dTmp / dSynthUndSpotBase * dSynthUndSpot
                    aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * aSynthRootComp.Weight * dTmp
                End If
                aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * aSynthRootComp.Weight
            End If
        Next
    
        If aSynthRoot.CashValue > 0# Then
            aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * aSynthRoot.CashValue
            aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * aSynthRoot.CashValue
        End If
    Else
        aRes.BadDelta = True
        aRes.BadNetDelta = True
    End If
    
    If (aGreeks.nMask And GM_GAMMA) = GM_GAMMA And Not IsBadDouble(aGreeks.dGamma) Then
        If aRes.Gamma <= BAD_DOUBLE_VALUE Then aRes.Gamma = 0#
        If aRes.NetGamma <= BAD_DOUBLE_VALUE Then aRes.NetGamma = 0#
        If aRes.GammaPerc <= BAD_DOUBLE_VALUE Then aRes.GammaPerc = 0#
        
        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
            Set aSynthUnd = m_Und(aSynthRootComp.UndID)
            If Not aSynthUnd Is Nothing Then
'                dTmp = PriceMidEx(aSynthUnd.PriceBid, aSynthUnd.PriceAsk, aSynthUnd.PriceLast, g_Params.UseLastPriceForCalcs)
                Debug.Assert (Not aSynthUnd.UndPriceProfile Is Nothing)
                dTmp = aSynthUnd.UndPriceProfile.GetUndPriceMid(aSynthUnd.price.Bid, aSynthUnd.price.Ask, aSynthUnd.price.Last, dToleranceValue, enRoundingRule)
                If Not IsBadDouble(dTmp) And dTmp > 0# Then
                    dTmp = dTmp / dSynthUndSpotBase * dSynthUndSpot
                    aRes.Gamma = aRes.Gamma + aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight * dTmp * dTmp / 100#
                    aRes.GammaPerc = aRes.GammaPerc + aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight * dTmp / 100#
                End If
                aRes.NetGamma = aRes.NetGamma + aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight
            End If
        Next
    
        If aSynthRoot.CashValue > 0# Then
            aRes.Gamma = aRes.Gamma + aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue / 100#
            aRes.NetGamma = aRes.NetGamma + aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue
            aRes.GammaPerc = aRes.GammaPerc + aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue / 100#
        End If
    Else
        aRes.BadGamma = True
        aRes.BadNetGamma = True
        aRes.BadGammaPerc = True
    End If
    
    If (aGreeks.nMask And GM_VEGA) = GM_VEGA And Not IsBadDouble(aGreeks.dVega) Then
        If aRes.Vega <= BAD_DOUBLE_VALUE Then aRes.Vega = 0#
        aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.QtyInShares
    
        If aRes.WtdVega <= BAD_DOUBLE_VALUE Then aRes.WtdVega = 0#
        aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.QtyInShares * aPos.VegaWeight
    Else
        aRes.BadVega = True
        aRes.BadWtdVega = True
    End If
    
    If (aGreeks.nMask And GM_THETA) = GM_THETA And Not IsBadDouble(aGreeks.dTheta) Then
        If aRes.Theta <= BAD_DOUBLE_VALUE Then aRes.Theta = 0#
        aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.QtyInShares
    Else
        aRes.BadTheta = True
    End If
    
    If (aGreeks.nMask And GM_RHO) = GM_RHO And Not IsBadDouble(aGreeks.dRho) Then
        If aRes.Rho <= BAD_DOUBLE_VALUE Then aRes.Rho = 0#
        aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.QtyInShares
    Else
        aRes.BadRho = True
    End If
End Sub


Private Sub CalcUndPnL(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aRes As MatrixCalcResultType, _
                        ByVal dtToday As Date, ByVal dUndSpot#, ByVal dUndBid#, ByVal dUndAsk#)
    On Error Resume Next
    Dim bBadPnl As Boolean, dPnlTheo#
    Dim dSpotPriceBid#, dSpotPriceAsk#
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE

    dSpotPriceBid = dUndBid
    dSpotPriceAsk = dUndAsk

    If dSpotPriceBid > 0# And aPos.QtyLTDBuy > BAD_LONG_VALUE _
        Or dSpotPriceAsk > 0# And aPos.QtyLTDSell > BAD_LONG_VALUE Then
    
        If g_Params.PnLCalcType = PNLCT_LTD Or dtToday <> Date Then
            If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# And aPos.PosLTDBuy > BAD_DOUBLE_VALUE Then
                    dPnlTheo = dSpotPriceBid * aPos.QtyLTDBuy - aPos.PosLTDBuy
                Else
                    bBadPnl = True
                End If
            End If
                
            If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# And aPos.PosLTDSell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyLTDSell - aPos.PosLTDSell
                Else
                    bBadPnl = True
                End If
            End If
        Else
            If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# And aPos.PosDailyPrevDateBuy > BAD_DOUBLE_VALUE Then
                    dPnlTheo = dSpotPriceBid * aPos.QtyDailyPrevDateBuy - aPos.PosDailyPrevDateBuy
                Else
                    bBadPnl = True
                End If
            End If
                            
            If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# And aPos.PosDailyPrevDateSell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyDailyPrevDateSell - aPos.PosDailyPrevDateSell
                Else
                    bBadPnl = True
                End If
            End If
                        
            If Not bBadPnl And aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# And aPos.PosDailyTodayBuy > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceBid * aPos.QtyDailyTodayBuy - aPos.PosDailyTodayBuy
                Else
                    bBadPnl = True
                End If
            End If
                            
            If Not bBadPnl And aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# And aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyDailyTodaySell - aPos.PosDailyTodaySell
                Else
                    bBadPnl = True
                End If
            End If
        End If
    End If
    
    If Not bBadPnl And dPnlTheo > BAD_DOUBLE_VALUE Then
        If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
        aRes.PnL = aRes.PnL + dPnlTheo
    Else
        aRes.BadPnL = True
    End If
End Sub

Private Sub UnderlyingsAdjustRates()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    
    For Each aUnd In m_Und
        UnderlyingAdjustRates aUnd
    Next
End Sub

'Private Sub UnderlyingAdjustRates(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
'    On Error Resume Next
'    Dim aPos As EtsMmRisksLib.MmRvPosAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
'    If aUnd Is Nothing Then Exit Sub
'
'    Dim dtNow As Date
'    dtNow = GetNewYorkTime
'
'    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
'
'    If GetIrRuleType = enRateBasedOnPosition Then
'        cPosThreshold = Abs(GetPosThreshold)
'        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
'    Else
'        bUseMidRates = True
'    End If
'
'    aUnd.UseMidRates = bUseMidRates
'    Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
'
'    For Each aPos In aUnd.Pos
'        If aPos.ContractType = enCtOption Then
'            If bUseMidRates Then
'                If Not aUnd.IsHTB Then
'                    aPos.Rate = GetNeutralRate(dtNow, aPos.ExpiryOV)
'                Else
'                    aPos.Rate = GetNeutralHTBRate(dtNow, aPos.ExpiryOV)
'                End If
'            Else
'                If Not aUnd.IsHTB Then
'                    aPos.Rate = IIf(dPos < 0, GetShortRate(dtNow, aPos.ExpiryOV), GetLongRate(dtNow, aPos.ExpiryOV))
'                Else
'                    aPos.Rate = IIf(dPos < 0, GetHTBRate(dtNow, aPos.ExpiryOV), GetLongRate(dtNow, aPos.ExpiryOV))
'                End If
'            End If
'
'        End If
'    Next
'End Sub

Private Sub UnderlyingAdjustRates(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    If aUnd Is Nothing Then Exit Sub

    Dim dtToday As Date: dtToday = GetNewYorkTime
    
    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    Else
        bUseMidRates = True
    End If

    aUnd.UseMidRates = bUseMidRates
    Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
    
    For Each aPos In aUnd.Pos
        If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
            If bUseMidRates Then
                aPos.Rate = GetNeutralRate(dtToday, aPos.ExpiryOV)
            Else
                aPos.Rate = IIf(dPos < 0, GetShortRate(dtToday, aPos.ExpiryOV), GetLongRate(dtToday, aPos.ExpiryOV))
            End If
            
            If bIsHTBRatesExist Then
                aPos.HTBRate = GetHTBRate(aUnd.ID, dtToday, aPos.ExpiryOV)
            Else
                aPos.HTBRate = BAD_DOUBLE_VALUE
            End If
        End If
    Next
End Sub

Private Sub InitVola(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), VolaSource
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

Private Function Validate_Und() As Boolean
    Dim aUnd As EtsGeneralLib.UndAtom, nUndPos&, dwLI&
    
    Validate_Und = False
    'cmbTradeUnd.Text = UCase(cmbTradeUnd.Text)
    
    For dwLI = 0 To cmbTradeUnd.ListCount
        If cmbTradeUnd.List(dwLI) = cmbTradeUnd.Text Then
            cmbTradeUnd.ListIndex = dwLI
            Exit For
        End If
    Next
    If cmbTradeUnd.ListIndex = -1 Then
        cmbTradeUnd.ListIndex = 0
        Validate_Und = False
        Exit Function
    End If
    
    If Not g_Underlying(cmbTradeUnd.ItemData(cmbTradeUnd.ListIndex)) Is Nothing Then Validate_Und = True
End Function

Private Sub FillUndGrid(nUndID As Long)
    Dim nUndPos&, aUnd As MmRvUndAtom
    
    With fgUnd
        If Not m_Und(nUndID) Is Nothing Then
            Set aUnd = m_Und(nUndID)
            
            .TextMatrix(1, PUC_SYMBOL) = aUnd.Symbol
            .TextMatrix(1, PUC_POSITION) = IIf(aUnd.Qty = BAD_LONG_VALUE, "--", aUnd.Qty)
            .TextMatrix(1, PUC_PRICE) = m_dCurUndMidPrice
        Else
            .TextMatrix(1, PUC_SYMBOL) = g_Underlying(nUndID).Symbol
            .TextMatrix(1, PUC_POSITION) = 0
            .TextMatrix(1, PUC_PRICE) = g_Underlying(nUndID).PriceClose
        End If
    End With
End Sub

Private Sub FillUndPosGrid(UndID As Long)
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    Dim nCurRow&
    
    nCurRow = 0
    
    With fgOpt
        .rows = 1
        
        Set aUnd = m_Und(UndID)
        If aUnd Is Nothing Then Exit Sub
        
        For Each aPos In aUnd.Pos
            If aPos.ContractType = enCtOption Then
                .AddItem ""
                nCurRow = .rows - 1
                .TextMatrix(nCurRow, POC_SYMBOL) = aPos.Symbol
                .TextMatrix(nCurRow, POC_EXPDATE) = aPos.Expiry
                .TextMatrix(nCurRow, POC_STRIKE) = aPos.Strike
                .TextMatrix(nCurRow, POC_TYPE) = IIf(aPos.OptType, "C", "P")
                .TextMatrix(nCurRow, POC_POSITION) = aPos.Qty
                .TextMatrix(nCurRow, POC_RFR) = aPos.Rate * 100
                .TextMatrix(nCurRow, POC_UNYLD) = aUnd.Dividend.DivAmt
                .TextMatrix(nCurRow, POC_DENOM) = aPos.Quote.LotSize
                .TextMatrix(nCurRow, POC_VOL) = aPos.Quote.Vola * 100
                .TextMatrix(nCurRow, POC_PRICE) = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, aPos.Quote.PriceTheo)
            End If
        Next
        
        Set aUnd = Nothing
        
        .AutoSize 0, .Cols - 1
    End With
End Sub

Private Sub FillUndExpGrid()
    With fgExp
        .TextMatrix(1, PEC_SHIFT) = 0
        
        If Not m_Exp(1) Is Nothing Then _
            .TextMatrix(2, PEC_SHIFT) = CDate(m_Exp(1).Name) - Date
        
        If Not m_Exp(2) Is Nothing Then _
            .TextMatrix(3, PEC_SHIFT) = CDate(m_Exp(2).Name) - Date
    End With
End Sub

Private Sub tmrFill_Timer()
    tmrFill.Enabled = False
    If m_bInProc Or m_bLastQuoteReqNow Or m_bRecalc Then _
        Exit Sub
    
    If Not m_bDataEditedByUser Then
        FillUndGrid m_nCurUndID
        FillUndPosGrid m_nCurUndID
        FillUndExpGrid
    End If
    DrawChart
    
    cmdPrint.Enabled = True
End Sub

Private Sub DrawChart()
    On Error Resume Next
    
    If m_bInProc Or m_bRecalc Then Exit Sub
    
    m_bInProc = True
    
    With chrtPnL
        .IsBatched = True
        ClearChart
        
        With .ChartGroups(1).Data
            .NumSeries = UBound(m_Res, 2) + 1
        
            For i = 1 To .NumSeries
                .NumPoints(i) = UBound(m_Res, 1) + 1
            Next i
        
            For j = 1 To .NumPoints(1)
                .X(1, j) = m_dCurUndMidPrice + m_Res(j - 1, 0).ShiftX
            Next j
            
            For i = 1 To .NumSeries
                If fgExp.TextMatrix(i, PEC_SHIFT) <> "" Then
                    For j = 1 To .NumPoints(i)
                        .Y(i, j) = m_Res(j - 1, i - 1).PnL
                    Next j
                End If
            Next i
            
        End With
        
        With .ChartArea

            For i = 1 To m_nLabelCount
                ' Row Number 3 Refers to Price Row in Val Grids
                .Axes(1).ValueLabels.Add CDbl(fgVal.ValueMatrix(0, i)), Format(fgVal.ValueMatrix(0, i), "#,##0.00")
            Next i

            With .Axes(1)
                .AnnotationRotation = oc2dRotate90Degrees
                .AnnotationPlacement = oc2dAnnotateMinimum
                .LabelFormat.Format = "#,##0.00"
                .Font.Name = "Tahoma"
                .Font.Size = "8"
                .Min = .ValueLabels(1).Value
                .Max = .ValueLabels(m_nLabelCount).Value
            End With

        End With
        
        .IsBatched = False
    End With
    
    m_bInProc = False
    
End Sub

Private Sub ClearChart()
    With chrtPnL
        .ChartGroups(1).Data.NumSeries = 0
        .ChartArea.Axes(1).ValueLabels.RemoveAll
        .ChartArea.Axes(2).ValueLabels.RemoveAll
    End With
End Sub

Public Sub Refresh()
    On Error Resume Next
    Dim bIncorrectInput As Boolean
    
    If Not m_bDataEditedByUser Then
        tmrShow.Enabled = True
    Else
        If m_bInProc Or m_bDataLoad Then Exit Sub
    
        On Error Resume Next
        bIncorrectInput = False
        Err.Clear
        
        m_nLabelCount = CInt(txtPointsCount.Text)
        m_nChartWidth = CInt(txtChartWidth.Text)
        
        bIncorrectInput = (m_nLabelCount <= 1) Or (m_nLabelCount > MAX_CALC_POINTS) Or (m_nChartWidth <= 0) Or (m_nChartWidth > MAX_CHART_WIDTH)
        If bIncorrectInput Then
            ShowUserWarning
            Exit Sub
        End If
        
        If Err.Number <> 0 Then Exit Sub
        
        InitResults
        CalcMatrix
    End If
End Sub

Private Sub txtChartWidth_KeyDown(KeyCode As Integer, Shift As Integer)
    If Chr(KeyCode) = vbCr And m_nCurUndID <> 0 Then _
        SetRefreshHint True
End Sub

Private Sub txtPointsCount_KeyDown(KeyCode As Integer, Shift As Integer)
    If Chr(KeyCode) = vbCr And m_nCurUndID <> 0 Then _
        SetRefreshHint True
End Sub

Private Sub UserControl_Hide()
    On Error Resume Next
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
End Sub

Private Sub UserControl_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        PopupMenu menuCtx
    End If
End Sub

Private Sub InitPrinter()
   On Error Resume Next
   If Not vsPrinter Is Nothing Then
    With vsPrinter
        .PhysicalPage = True
        .NavBar = vpnbNone
        .Track = True
        .MarginHeader = .MarginTop - ScaleY(10, vbPoints, vbTwips)
        .MarginFooter = .MarginBottom - ScaleY(10, vbPoints, vbTwips)
    End With
   End If
End Sub

Private Sub UserControl_Resize()
    Dim nChangedFor&, nTotalCols&, nGridColWidth&, nGridHeight&
    nChangedFor = ScaleWidth - chrtPnL.Width - fgOpt.Width - 90
    
   szMain.Width = ScaleWidth
   szMain.Height = ScaleHeight - 100
    
'    fgOpt.Width = fgOpt.Width + nChangedFor \ 2
'    chrtPnL.Width = chrtPnL.Width + nChangedFor \ 2
'    chrtPnL.Left = UserControl.ScaleWidth - chrtPnL.Width - 15
'    'fgOpt.Width = chrtPnL.Left - fgOpt.Left - 60
'
'    With fgVal
'        nGridHeight = 2900
'        nTotalCols = .Cols - 1
'        nGridColWidth = 0
'        If nTotalCols = .RightCol And .LeftCol = 1 Then
'            nGridColWidth = 0&
'            For i = 0 To .Cols - 1
'                nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth
'            Next
'            If nGridColWidth > 0 Then nGridColWidth = nGridColWidth - ScaleY(.GridLineWidth, vbPixels, vbTwips)
'
'            If ScaleWidth < nGridColWidth Then
'                nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
'                .ScrollBars = flexScrollBarHorizontal
'            Else
'                .ScrollBars = flexScrollBarNone
'            End If
'        ElseIf nTotalCols <> .RightCol Or .LeftCol <> 1 Then
'              nGridHeight = nGridHeight + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
'             .ScrollBars = flexScrollBarHorizontal
'        Else
'          .ScrollBars = flexScrollBarNone
'        End If
'        .Height = nGridHeight
'        .Top = ScaleHeight - .Height - 90
'        .Width = ScaleWidth - 15
'    End With
'
'    If fgVal.Top - fgOpt.Top - 45 > 0 And fgVal.Top - chrtPnL.Top - 45 > 0 Then
'        fgOpt.Height = fgVal.Top - fgOpt.Top - 45
'        chrtPnL.Height = fgVal.Top - chrtPnL.Top - 45
'    End If
End Sub

Private Sub vsPrinterMain_Error()
    ShowPrinterErrorMsgBox
End Sub

Private Sub ShowPrinterErrorMsgBox()
    With vsPrinterMain
        Select Case .Error
        Case 0
        Case vperCantAccessPrinter
            gCmn.CantAccessPrinterErrorMsgBox
        Case Else
            gCmn.ErrorMsgBox m_frmOwner, .ErrorDescription
        End Select
    End With
End Sub

Private Sub ShowUserWarning()
    Dim frmWarn As frmWarning
    Set frmWarn = New frmWarning
    frmWarn.Caption = "P&L Projections"
    frmWarn.lblWarning = vbCrLf & "Incorrect parameter input for 'Chart Width' or/and 'Calc Points'"
    frmWarn.Show vbModal
End Sub

Private Sub CalcTheoPnLCommonExerc(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                    ByRef aRes As MatrixCalcResultType, _
                                    ByVal dUndSpot#, ByVal dUndBid#, ByVal dUndAsk#)
    On Error Resume Next
'    Dim bBadPnl As Boolean, dTmp#, dPnlTheo#
'
'    bBadPnl = False
'    dPnlTheo = BAD_DOUBLE_VALUE
    
    ' ITM options
    If aPos.OptType = enOtCall And aPos.Strike < dUndSpot _
        Or aPos.OptType = enOtPut And aPos.Strike > dUndSpot Then
        
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        dTmp = IIf(aPos.OptType = enOtCall, 1, -1)
        If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
            aRes.Delta = aRes.Delta + aPos.QtyLTDBuy * dTmp * dUndSpot
            aRes.NetDelta = aRes.NetDelta + aPos.QtyLTDBuy * dTmp
        End If
        If aPos.QtyLTDSell > BAD_LONG_VALUE Then
            aRes.Delta = aRes.Delta + aPos.QtyLTDSell * dTmp * dUndSpot
            aRes.NetDelta = aRes.NetDelta + aPos.QtyLTDSell * dTmp
        End If
    Else ' OTM options
'        dPnlTheo = CalcTheoPnLCommonExercOtm(aPos)
    End If

'    If dPnlTheo > BAD_DOUBLE_VALUE Then
'        If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
'        aRes.PnL = aRes.PnL + dPnlTheo
'    Else
'        aRes.BadPnL = True
'    End If
    
End Sub


