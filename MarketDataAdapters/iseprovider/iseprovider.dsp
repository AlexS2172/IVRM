# Microsoft Developer Studio Project File - Name="ISEProvider" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ISEProvider - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ISEProvider.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ISEProvider.mak" CFG="ISEProvider - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ISEProvider - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ISEProvider - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ISE Connection/ISEProvider", XTZBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ISEProvider - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MT /W3 /Gm /GX /Zi /Od /I "..\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_EGDEBUG" /FR /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../bin/ISEProviderD.exe" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\PROJ\ETS\Work\bin\ISEProviderD.exe
InputPath=\PROJ\ETS\Work\bin\ISEProviderD.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "ISEProvider - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ISEProvider___Win32_Release"
# PROP BASE Intermediate_Dir "ISEProvider___Win32_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O1 /I "..\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_DLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Zi /Od /I "..\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_ATL_DLL" /D "_STLP_USE_STATIC_LIB" /FAs /FR /Yu"stdafx.h" /FD /Zm200 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:windows /machine:I386 /out:"../bin/ISEProvider.exe"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib /nologo /subsystem:windows /pdb:"C:\PROGRA~1\COMMON~1\EGAR\ISEProvider.pdb" /debug /machine:I386 /out:"../bin/ISEProvider.exe"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=\PROJ\ETS\Work\bin\ISEProvider.exe
InputPath=\PROJ\ETS\Work\bin\ISEProvider.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "ISEProvider - Win32 Debug"
# Name "ISEProvider - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ISEBBO.CPP
# End Source File
# Begin Source File

SOURCE=.\ISEBroadcasts.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEData.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEDataFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEDataTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEMultiSession.cpp
# End Source File
# Begin Source File

SOURCE=.\ISESession.cpp
# End Source File
# Begin Source File

SOURCE=.\ISESessionFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\ISESubscr.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEUserInfo.cpp
# End Source File
# End Group
# Begin Group "Utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ISESettingsManager.cpp
# End Source File
# Begin Source File

SOURCE=.\OmniApiUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\Trace.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ISEMMManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEPriceProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEProvider.idl
# ADD MTL /tlb ".\ISEProvider.tlb" /h "ISEProvider.h" /iid "ISEProvider_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\ISEProvider.rc
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\ISEStructureProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\ISETracer.cpp
# End Source File
# Begin Source File

SOURCE=.\MMParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectID.cpp
# End Source File
# Begin Source File

SOURCE=.\ProviderInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Core.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ISEConst.h
# End Source File
# Begin Source File

SOURCE=.\ISEData.h
# End Source File
# Begin Source File

SOURCE=.\ISEDataTypes.h
# End Source File
# Begin Source File

SOURCE=.\ISEEvents.h
# End Source File
# Begin Source File

SOURCE=.\ISEManager.h
# End Source File
# Begin Source File

SOURCE=.\ISEManagerBase.h
# End Source File
# Begin Source File

SOURCE=.\ISEMessage.h
# End Source File
# Begin Source File

SOURCE=.\ISEMultiSession.h
# End Source File
# Begin Source File

SOURCE=.\ISESession.h
# End Source File
# Begin Source File

SOURCE=.\ISEStatus.h
# End Source File
# Begin Source File

SOURCE=.\ISEUser.h
# End Source File
# Begin Source File

SOURCE=.\OmexVer.h
# End Source File
# End Group
# Begin Group "Utils.h"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExchangesMapping.h
# End Source File
# Begin Source File

SOURCE=.\ISESettingsManager.h
# End Source File
# Begin Source File

SOURCE=.\OmniApiUtils.h
# End Source File
# Begin Source File

SOURCE=.\ThreadWrapper.h
# End Source File
# Begin Source File

SOURCE=.\Trace.h
# End Source File
# Begin Source File

SOURCE=.\tst.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ISEMMManager.h
# End Source File
# Begin Source File

SOURCE=.\ISEPriceProvider.h
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceInfo.h
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceProvider.h
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureInfo.h
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureProvider.h
# End Source File
# Begin Source File

SOURCE=.\ISEStructureProvider.h
# End Source File
# Begin Source File

SOURCE=.\ISETracer.h
# End Source File
# Begin Source File

SOURCE=.\MMParameters.h
# End Source File
# Begin Source File

SOURCE=.\ObjectID.h
# End Source File
# Begin Source File

SOURCE=.\ProviderInfo.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\ISEInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEMMManager.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEPriceInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEPriceProvider.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEProvider.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStdPriceProvider.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStdStructureProvider.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStructureInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\ISEStructureProvider.rgs
# End Source File
# Begin Source File

SOURCE=.\ISETracer.rgs
# End Source File
# Begin Source File

SOURCE=.\MMParameters.rgs
# End Source File
# Begin Source File

SOURCE=.\ProviderInfo.rgs
# End Source File
# End Group
# Begin Source File

SOURCE=.\readme.txt
# End Source File
# End Target
# End Project
