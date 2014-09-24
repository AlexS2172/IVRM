# Microsoft Developer Studio Project File - Name="IVAnalyzer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=IVAnalyzer - Win32 Debug Multiproviders
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "IVAnalyzer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "IVAnalyzer.mak" CFG="IVAnalyzer - Win32 Debug Multiproviders"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "IVAnalyzer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "IVAnalyzer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "IVAnalyzer - Win32 Debug Multiproviders" (based on "Win32 (x86) Application")
!MESSAGE "IVAnalyzer - Win32 Release WSB" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ETS/Production 2.1 CIBC/IVManager", KUADAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "IVAnalyzer - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\EgarCommonLibrary\Include" /I "..\..\MarketDataAdapters\tlb" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /D "_WSB" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Vfw32.lib ..\lib\edaTools.lib /nologo /subsystem:windows /pdb:"../bin/VolaManager.pdb" /debug /machine:I386 /out:"..\bin/VolaManager.exe" /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /profile /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManager.exe
InputPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManager.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IVAnalyzer - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\EgarCommonLibrary\Include" /I "..\..\MarketDataAdapters\tlb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /D "_WSB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Vfw32.lib ..\lib\edaToolsD.lib /nologo /subsystem:windows /incremental:no /pdb:"../bin/VolaManagerD.pdb" /debug /machine:I386 /out:"..\bin/VolaManagerD.exe" /pdbtype:sept /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManagerD.exe
InputPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManagerD.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IVAnalyzer - Win32 Debug Multiproviders"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "IVAnalyzer___Win32_Debug_Multiproviders"
# PROP BASE Intermediate_Dir "IVAnalyzer___Win32_Debug_Multiproviders"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_Multiproviders"
# PROP Intermediate_Dir "Debug_Multiproviders"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\EgCommon\Include" /I "..\..\EgarCommonLibrary\Include" /I "..\..\MarketDataAdapters\tlb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Vfw32.lib ..\lib\edaToolsD.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"..\bin/VolaManagerD.exe" /pdbtype:sept
# ADD LINK32 Vfw32.lib ..\lib\edaToolsD.lib /nologo /subsystem:windows /incremental:no /debug /machine:I386 /out:"..\bin/VolaManagerD.exe" /pdbtype:sept /libpath:"..\..\EgarCommonLibrary\lib"
# Begin Custom Build - Performing registration
OutDir=.\Debug_Multiproviders
TargetPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManagerD.exe
InputPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManagerD.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "IVAnalyzer - Win32 Release WSB"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "IVAnalyzer___Win32_Release_WSB"
# PROP BASE Intermediate_Dir "IVAnalyzer___Win32_Release_WSB"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_WSB"
# PROP Intermediate_Dir "Release_WSB"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\..\EgCommon\Include" /I "..\..\EgarCommonLibrary\Include" /I "..\..\MarketDataAdapters\tlb" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_VOLA_MANAGER" /D "_MULTIPROVIDERS" /D "_WSB" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL" /d "_VOLA_MANAGER"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Vfw32.lib ..\lib\edaTools.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"..\bin/VolaManager.exe"
# ADD LINK32 Vfw32.lib ..\lib\edaTools.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"..\bin\VolaManager.exe" /libpath:"..\..\EgarCommonLibrary\lib"
# Begin Custom Build - Performing registration
OutDir=.\Release_WSB
TargetPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManager.exe
InputPath=\ClearCaseViews\EG1_ASP_Inst_Development\ETS\bin\VolaManager.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "IVAnalyzer - Win32 Release"
# Name "IVAnalyzer - Win32 Debug"
# Name "IVAnalyzer - Win32 Debug Multiproviders"
# Name "IVAnalyzer - Win32 Release WSB"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddSymbolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\APPBAR.CPP
# End Source File
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChartWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\CreateDefaultVolaDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomVolaData.cpp
# End Source File
# Begin Source File

SOURCE=.\DataProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLayoutBase.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLayoutSQL.cpp
# End Source File
# Begin Source File

SOURCE=.\DualQuadParams.cpp
# End Source File
# Begin Source File

SOURCE=.\EditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\EGButton.cpp
# End Source File
# Begin Source File

SOURCE=.\EGSpin.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupStructDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IconBar.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IRCurve.cpp
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzer.cpp
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzer.idl
# ADD MTL /h "IVAnalyzer_i.h" /iid "IVAnalyzer_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzer.rc
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\IVConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\IVControls.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LicenseKey.cpp
# End Source File
# Begin Source File

SOURCE=.\MktStructureBaseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MktStructureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModulVer.cpp
# End Source File
# Begin Source File

SOURCE=.\MonitorFunctionsWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\NumericEditEx.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsConnectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsSkewPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupText.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoveSymbolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RuleDefinitionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RulesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StrikeSkewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolGroupsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TimeSkewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolTipCtrlEx.cpp
# End Source File
# Begin Source File

SOURCE=.\VolaAddNewSurfaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolaEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolaManagementDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolaNextDaySurfaceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\VolaOpenSurfaceDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddSymbolDlg.h
# End Source File
# Begin Source File

SOURCE=.\APPBAR.H
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\ChartDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChartWnd.h
# End Source File
# Begin Source File

SOURCE=.\CreateDefaultVolaDlg.h
# End Source File
# Begin Source File

SOURCE=.\CustomVolaData.h
# End Source File
# Begin Source File

SOURCE=.\DataDef.h
# End Source File
# Begin Source File

SOURCE=.\DataHolder.h
# End Source File
# Begin Source File

SOURCE=.\DataProcessor.h
# End Source File
# Begin Source File

SOURCE=.\DBLayout.h
# End Source File
# Begin Source File

SOURCE=.\DBLayoutBase.h
# End Source File
# Begin Source File

SOURCE=.\DBLayoutSQL.h
# End Source File
# Begin Source File

SOURCE=.\DualQuadParams.h
# End Source File
# Begin Source File

SOURCE=.\EditEx.h
# End Source File
# Begin Source File

SOURCE=.\EGButton.h
# End Source File
# Begin Source File

SOURCE=.\EGSpin.h
# End Source File
# Begin Source File

SOURCE=.\FlatComboBox.h
# End Source File
# Begin Source File

SOURCE=.\GroupStructDlg.h
# End Source File
# Begin Source File

SOURCE=.\IconBar.h
# End Source File
# Begin Source File

SOURCE=.\InfoBar.h
# End Source File
# Begin Source File

SOURCE=.\IRCurve.h
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzer.h
# End Source File
# Begin Source File

SOURCE=.\IVAnalyzerBar.h
# End Source File
# Begin Source File

SOURCE=.\IVConnection.h
# End Source File
# Begin Source File

SOURCE=.\IVControls.h
# End Source File
# Begin Source File

SOURCE=.\LicenseDlg.h
# End Source File
# Begin Source File

SOURCE=.\LicenseKey.h
# End Source File
# Begin Source File

SOURCE=.\MktStructureBaseDlg.h
# End Source File
# Begin Source File

SOURCE=.\MktStructureDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModulVer.h
# End Source File
# Begin Source File

SOURCE=.\MonitorFunctionsWrapper.h
# End Source File
# Begin Source File

SOURCE=.\NumericEditEx.h
# End Source File
# Begin Source File

SOURCE=.\OptionsConnectionPage.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionsSheet.h
# End Source File
# Begin Source File

SOURCE=.\OptionsSkewPage.h
# End Source File
# Begin Source File

SOURCE=.\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\PopupText.h
# End Source File
# Begin Source File

SOURCE=.\RemoveSymbolDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RuleDefinitionDlg.h
# End Source File
# Begin Source File

SOURCE=.\RulesDlg.h
# End Source File
# Begin Source File

SOURCE=.\SplashDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StrikeSkewDlg.h
# End Source File
# Begin Source File

SOURCE=.\SymbolCombo.h
# End Source File
# Begin Source File

SOURCE=.\SymbolGroupsDlg.h
# End Source File
# Begin Source File

SOURCE=.\TimeSkewDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolTipCtrlEx.h
# End Source File
# Begin Source File

SOURCE=.\VolaAddNewSurfaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\VolaEditorDlg.h
# End Source File
# Begin Source File

SOURCE=.\VolaManagementDlg.h
# End Source File
# Begin Source File

SOURCE=.\VolaMgmtInstance.h
# End Source File
# Begin Source File

SOURCE=.\VolaNextDaySurfaceDlg.h
# End Source File
# Begin Source File

SOURCE=.\VolaOpenSurfaceDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\app.ico
# End Source File
# Begin Source File

SOURCE=.\res\bar_icon.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ch_imgs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\delete.bmp
# End Source File
# Begin Source File

SOURCE=.\res\dq_par.ico
# End Source File
# Begin Source File

SOURCE=".\res\EGAR ETS.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\EGAR ETS.gif"
# End Source File
# Begin Source File

SOURCE=".\res\EGAR ONE.gif"
# End Source File
# Begin Source File

SOURCE=".\res\EGAR ONE.jpg"
# End Source File
# Begin Source File

SOURCE=.\res\hand_pre.cur
# End Source File
# Begin Source File

SOURCE=.\res\IVAnalyzer.rc2
# End Source File
# Begin Source File

SOURCE=.\ivmgr.rgs
# End Source File
# Begin Source File

SOURCE=.\res\Logo.gif
# End Source File
# Begin Source File

SOURCE=.\res\new.bmp
# End Source File
# Begin Source File

SOURCE=.\res\prop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Splash.jpg
# End Source File
# Begin Source File

SOURCE=.\res\Splash_IVM.jpg
# End Source File
# Begin Source File

SOURCE=.\res\svc_imgs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\sysmenu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tb_imgs.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vmr_del.ico
# End Source File
# Begin Source File

SOURCE=.\res\vmr_edit.ico
# End Source File
# Begin Source File

SOURCE=.\res\vmr_new.ico
# End Source File
# Begin Source File

SOURCE=.\res\ws_imgs.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section IVAnalyzer : {8E203240-537D-11D3-BD8C-000000000000}
# 	2:5:Class:CVSFlexDataSource
# 	2:10:HeaderFile:vsflexdatasource.h
# 	2:8:ImplFile:vsflexdatasource.cpp
# End Section
# Section IVAnalyzer : {D76D712E-4A96-11D3-BD95-D296DC2DD072}
# 	2:21:DefaultSinkHeaderFile:vsflexgrid.h
# 	2:16:DefaultSinkClass:CVSFlexGrid
# End Section
# Section IVAnalyzer : {0050004F-0045-0052-5400-59005F005200}
# 	1:14:IDR_IVANALYZER:103
# End Section
# Section IVAnalyzer : {D76D712B-4A96-11D3-BD95-D296DC2DD072}
# 	2:5:Class:CVSFlexGrid
# 	2:10:HeaderFile:vsflexgrid.h
# 	2:8:ImplFile:vsflexgrid.cpp
# End Section
