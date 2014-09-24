# Microsoft Developer Studio Project File - Name="VADBLayout" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VADBLayout - Win32 Unicode Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "VADBLayout.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "VADBLayout.mak" CFG="VADBLayout - Win32 Unicode Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "VADBLayout - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VADBLayout - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VADBLayout - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VADBLayout - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ETS/Production 2.1 CIBC/VADBLayout", GTADAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VADBLayout - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\EgarCommonLibrary\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /env win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib /nologo /subsystem:windows /dll /incremental:no /pdb:"../Bin/VADBLayoutD.pdb" /debug /machine:I386 /def:".\VADBLayout.def" /out:"../bin/VADBLayoutD.dll" /pdbtype:sept /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutD.dll
InputPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\EgCommon\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /env win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../bin/VADBLayoutUD.dll" /pdbtype:sept /libpath:"..\..\..\EgCommon\Lib" /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutUD.dll
InputPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutUD.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O1 /I "..\..\EgarCommonLibrary\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /FR /Yu"stdafx.h" /FD /c
# ADD MTL /env win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /machine:I386 /delayload:mtxex.dll
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib /nologo /subsystem:windows /dll /pdb:"../bin/VADBLayout.pdb" /debug /machine:I386 /def:".\VADBLayout.def" /out:"../bin/VADBLayout.dll" /implib:"../lib/VADBLayout.lib" /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Release
TargetPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayout.dll
InputPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayout.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Unicode Release"
# PROP BASE Intermediate_Dir "Unicode Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\..\EgCommon\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /env win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /delayload:mtxex.dll
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../bin/VADBLayoutU.dll" /pdbtype:sept /libpath:"..\..\..\EgCommon\Lib" /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\ReleaseU
TargetPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutU.dll
InputPath=\_Projects\EG1_CIBC_integration\ETS\bin\VADBLayoutU.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "VADBLayout - Win32 Debug"
# Name "VADBLayout - Win32 Unicode Debug"
# Name "VADBLayout - Win32 Release"
# Name "VADBLayout - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DBLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\DBLayoutAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VADBLayout.cpp
# End Source File
# Begin Source File

SOURCE=.\VADBLayout.def

!IF  "$(CFG)" == "VADBLayout - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VADBLayout.idl

!IF  "$(CFG)" == "VADBLayout - Win32 Debug"

# ADD MTL /tlb "..\tlb\VADBLayout.tlb" /h "VADBLayout.h" /iid "VADBLayout_i.c" /Oicf

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Debug"

# ADD MTL /tlb "..\tlb\VADBLayout.tlb" /h "VADBLayout.h" /iid "VADBLayout_i.c" /Oicf

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Release"

# ADD MTL /I "..\tlb" /tlb "..\tlb\VADBLayout.tlb" /h "VADBLayout.h" /iid "VADBLayout_i.c" /Oicf

!ELSEIF  "$(CFG)" == "VADBLayout - Win32 Unicode Release"

# ADD MTL /tlb "..\tlb\VADBLayout.tlb" /h "VADBLayout.h" /iid "VADBLayout_i.c" /Oicf

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\VADBLayout.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DBLayout.h
# End Source File
# Begin Source File

SOURCE=.\DBLayoutAccess.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
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

SOURCE=.\DBLayout.rgs
# End Source File
# Begin Source File

SOURCE=.\DBLayoutAccess.rgs
# End Source File
# End Group
# End Target
# End Project
