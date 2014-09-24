VERSION 5.00
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form Form1 
   BackColor       =   &H80000005&
   Caption         =   "Future editor"
   ClientHeight    =   2550
   ClientLeft      =   60
   ClientTop       =   300
   ClientWidth     =   5550
   LinkTopic       =   "Form1"
   ScaleHeight     =   2550
   ScaleWidth      =   5550
   StartUpPosition =   3  'Windows Default
   Begin ElladaFlatControls.FlatButton FlatButton3 
      Height          =   375
      Left            =   4080
      TabIndex        =   7
      Top             =   2040
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   661
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton FlatButton2 
      Height          =   375
      Left            =   2520
      TabIndex        =   6
      Top             =   2040
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   661
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Ok"
   End
   Begin VB.Frame Frame1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Future"
      ForeColor       =   &H80000008&
      Height          =   1695
      Left            =   240
      TabIndex        =   0
      Top             =   240
      Width           =   5175
      Begin ElladaFlatControls.FlatButton FlatButton1 
         Height          =   375
         Left            =   360
         TabIndex        =   5
         Top             =   1080
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   661
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "&Prices"
      End
      Begin MSComCtl2.DTPicker dtPMaurity 
         Height          =   375
         Left            =   3480
         TabIndex        =   3
         Top             =   360
         Width           =   1335
         _ExtentX        =   2355
         _ExtentY        =   661
         _Version        =   393216
         Format          =   60030977
         CurrentDate     =   38982
      End
      Begin VB.TextBox txtSymbol 
         Appearance      =   0  'Flat
         Height          =   315
         Left            =   960
         TabIndex        =   1
         Text            =   "txtSymbol"
         Top             =   360
         Width           =   1335
      End
      Begin VB.Label Label2 
         BackColor       =   &H80000005&
         Caption         =   "Maturity"
         Height          =   255
         Left            =   2760
         TabIndex        =   4
         Top             =   360
         Width           =   615
      End
      Begin VB.Label Label1 
         BackColor       =   &H80000005&
         Caption         =   "Symbol:"
         Height          =   255
         Left            =   240
         TabIndex        =   2
         Top             =   360
         Width           =   615
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
