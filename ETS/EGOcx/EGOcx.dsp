# Microsoft Developer Studio Project File - Name="EGOcx" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EGOcx - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EGOcx.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EGOcx.mak" CFG="EGOcx - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EGOcx - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EGOcx - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EGOcx - Win32 Release Builder" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ETS/Production 2.1 CIBC/EGOcx", TABDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EGOcx - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\EgarCommonLibrary\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /pdb:"../bin/EGOcx.pdb" /debug /machine:I386 /def:".\EGOcx.def" /out:"..\bin/EGOcx.dll" /implib:"..\lib\EGOcx.lib" /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EGOcx - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\..\EgarCommonLibrary\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /incremental:no /pdb:"../bin/EGOcxD.pdb" /debug /machine:I386 /def:".\EGOcx.def" /out:"..\Bin/EGOcxD.dll" /implib:"..\lib\EGOcxD.lib" /pdbtype:sept /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EGOcx - Win32 Release Builder"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EGOcx___Win32_Release_Builder"
# PROP BASE Intermediate_Dir "EGOcx___Win32_Release_Builder"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseBuilder"
# PROP Intermediate_Dir "ReleaseBuilder"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\EgCommon\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_VOLA_BUILDER" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\EGOcx.def" /out:"..\bin/EGOcx.dll" /implib:"..\lib\EGOcx.lib"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /def:".\EGOcx.def" /out:"..\bin/EGOcx.dll" /implib:"..\lib\EGOcx.lib" /libpath:"..\..\EgarCommonLibrary\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "EGOcx - Win32 Release"
# Name "EGOcx - Win32 Debug"
# Name "EGOcx - Win32 Release Builder"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\accountingvf.cpp
# End Source File
# Begin Source File

SOURCE=.\actionmap.cpp
# End Source File
# Begin Source File

SOURCE=.\actionmapcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\axis.cpp
# End Source File
# Begin Source File

SOURCE=.\axiscollection.cpp
# End Source File
# Begin Source File

SOURCE=.\axisstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\bar.cpp
# End Source File
# Begin Source File

SOURCE=.\border.cpp
# End Source File
# Begin Source File

SOURCE=.\bubble.cpp
# End Source File
# Begin Source File

SOURCE=.\candle.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2d.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2dchecknewpointsresult.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2dcoordresult.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2ddata.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2ddatacoordresult.cpp
# End Source File
# Begin Source File

SOURCE=.\chart2ddataindexresult.cpp
# End Source File
# Begin Source File

SOURCE=.\chartarea.cpp
# End Source File
# Begin Source File

SOURCE=.\chartgroup.cpp
# End Source File
# Begin Source File

SOURCE=.\chartgroupcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\chartlabel.cpp
# End Source File
# Begin Source File

SOURCE=.\chartlabelcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\chartstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\chartstylecollection.cpp
# End Source File
# Begin Source File

SOURCE=.\coord.cpp
# End Source File
# Begin Source File

SOURCE=.\currencyvf.cpp
# End Source File
# Begin Source File

SOURCE=.\datacoord.cpp
# End Source File
# Begin Source File

SOURCE=.\dataindex.cpp
# End Source File
# Begin Source File

SOURCE=.\derivedboolean.cpp
# End Source File
# Begin Source File

SOURCE=.\deriveddouble.cpp
# End Source File
# Begin Source File

SOURCE=.\derivedenum.cpp
# End Source File
# Begin Source File

SOURCE=.\derivedlong.cpp
# End Source File
# Begin Source File

SOURCE=.\derivedstring.cpp
# End Source File
# Begin Source File

SOURCE=.\EGOcx.cpp
# End Source File
# Begin Source File

SOURCE=.\EGOcx.def

!IF  "$(CFG)" == "EGOcx - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EGOcx - Win32 Debug"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "EGOcx - Win32 Release Builder"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\EGOcx.odl
# End Source File
# Begin Source File

SOURCE=.\EGOcx.rc
# End Source File
# Begin Source File

SOURCE=.\fillstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\fractionvf.cpp
# End Source File
# Begin Source File

SOURCE=.\gridlines.cpp
# End Source File
# Begin Source File

SOURCE=.\hiloopenclose.cpp
# End Source File
# Begin Source File

SOURCE=.\image.cpp
# End Source File
# Begin Source File

SOURCE=.\interior.cpp
# End Source File
# Begin Source File

SOURCE=.\label.cpp
# End Source File
# Begin Source File

SOURCE=.\labelcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\legend.cpp
# End Source File
# Begin Source File

SOURCE=.\linestyle.cpp
# End Source File
# Begin Source File

SOURCE=.\location.cpp
# End Source File
# Begin Source File

SOURCE=.\marker.cpp
# End Source File
# Begin Source File

SOURCE=.\markercollection.cpp
# End Source File
# Begin Source File

SOURCE=.\numbervf.cpp
# End Source File
# Begin Source File

SOURCE=.\percentagevf.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\pie.cpp
# End Source File
# Begin Source File

SOURCE=.\plotarea.cpp
# End Source File
# Begin Source File

SOURCE=.\pointstylecollection.cpp
# End Source File
# Begin Source File

SOURCE=.\polar.cpp
# End Source File
# Begin Source File

SOURCE=.\scientificvf.cpp
# End Source File
# Begin Source File

SOURCE=.\slicestyle.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stringcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\symbolstyle.cpp
# End Source File
# Begin Source File

SOURCE=.\timescale.cpp
# End Source File
# Begin Source File

SOURCE=.\title.cpp
# End Source File
# Begin Source File

SOURCE=.\valueformat.cpp
# End Source File
# Begin Source File

SOURCE=.\valuelabel.cpp
# End Source File
# Begin Source File

SOURCE=.\valuelabelcollection.cpp
# End Source File
# Begin Source File

SOURCE=.\view3d.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexdatasource.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\vsflexnode.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\accountingvf.h
# End Source File
# Begin Source File

SOURCE=.\actionmap.h
# End Source File
# Begin Source File

SOURCE=.\actionmapcollection.h
# End Source File
# Begin Source File

SOURCE=.\axis.h
# End Source File
# Begin Source File

SOURCE=.\axiscollection.h
# End Source File
# Begin Source File

SOURCE=.\axisstyle.h
# End Source File
# Begin Source File

SOURCE=.\bar.h
# End Source File
# Begin Source File

SOURCE=.\border.h
# End Source File
# Begin Source File

SOURCE=.\bubble.h
# End Source File
# Begin Source File

SOURCE=.\candle.h
# End Source File
# Begin Source File

SOURCE=.\chart2d.h
# End Source File
# Begin Source File

SOURCE=.\chart2dchecknewpointsresult.h
# End Source File
# Begin Source File

SOURCE=.\chart2dcoordresult.h
# End Source File
# Begin Source File

SOURCE=.\chart2ddata.h
# End Source File
# Begin Source File

SOURCE=.\chart2ddatacoordresult.h
# End Source File
# Begin Source File

SOURCE=.\chart2ddataindexresult.h
# End Source File
# Begin Source File

SOURCE=.\chartarea.h
# End Source File
# Begin Source File

SOURCE=.\chartgroup.h
# End Source File
# Begin Source File

SOURCE=.\chartgroupcollection.h
# End Source File
# Begin Source File

SOURCE=.\chartlabel.h
# End Source File
# Begin Source File

SOURCE=.\chartlabelcollection.h
# End Source File
# Begin Source File

SOURCE=.\chartstyle.h
# End Source File
# Begin Source File

SOURCE=.\chartstylecollection.h
# End Source File
# Begin Source File

SOURCE=.\coord.h
# End Source File
# Begin Source File

SOURCE=.\currencyvf.h
# End Source File
# Begin Source File

SOURCE=.\datacoord.h
# End Source File
# Begin Source File

SOURCE=.\dataindex.h
# End Source File
# Begin Source File

SOURCE=.\derivedboolean.h
# End Source File
# Begin Source File

SOURCE=.\deriveddouble.h
# End Source File
# Begin Source File

SOURCE=.\derivedenum.h
# End Source File
# Begin Source File

SOURCE=.\derivedlong.h
# End Source File
# Begin Source File

SOURCE=.\derivedstring.h
# End Source File
# Begin Source File

SOURCE=.\fillstyle.h
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\fractionvf.h
# End Source File
# Begin Source File

SOURCE=.\gridlines.h
# End Source File
# Begin Source File

SOURCE=.\hiloopenclose.h
# End Source File
# Begin Source File

SOURCE=.\image.h
# End Source File
# Begin Source File

SOURCE=.\interior.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\labelcollection.h
# End Source File
# Begin Source File

SOURCE=.\legend.h
# End Source File
# Begin Source File

SOURCE=.\linestyle.h
# End Source File
# Begin Source File

SOURCE=.\location.h
# End Source File
# Begin Source File

SOURCE=.\marker.h
# End Source File
# Begin Source File

SOURCE=.\markercollection.h
# End Source File
# Begin Source File

SOURCE=.\numbervf.h
# End Source File
# Begin Source File

SOURCE=.\percentagevf.h
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\pie.h
# End Source File
# Begin Source File

SOURCE=.\plotarea.h
# End Source File
# Begin Source File

SOURCE=.\pointstylecollection.h
# End Source File
# Begin Source File

SOURCE=.\polar.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\scientificvf.h
# End Source File
# Begin Source File

SOURCE=.\slicestyle.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stringcollection.h
# End Source File
# Begin Source File

SOURCE=.\symbolstyle.h
# End Source File
# Begin Source File

SOURCE=.\timescale.h
# End Source File
# Begin Source File

SOURCE=.\title.h
# End Source File
# Begin Source File

SOURCE=.\valueformat.h
# End Source File
# Begin Source File

SOURCE=.\valuelabel.h
# End Source File
# Begin Source File

SOURCE=.\valuelabelcollection.h
# End Source File
# Begin Source File

SOURCE=.\view3d.h
# End Source File
# Begin Source File

SOURCE=.\vsflexdatasource.h
# End Source File
# Begin Source File

SOURCE=.\vsflexgrid.h
# End Source File
# Begin Source File

SOURCE=.\vsflexnode.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\EGOcx.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section EGOcx : {D86FB468-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartStyleCollection
# 	2:10:HeaderFile:chartstylecollection.h
# 	2:8:ImplFile:chartstylecollection.cpp
# End Section
# Section EGOcx : {5DF9BF61-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CDerivedBoolean
# 	2:10:HeaderFile:derivedboolean.h
# 	2:8:ImplFile:derivedboolean.cpp
# End Section
# Section EGOcx : {5DF9BF6D-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CCurrencyVF
# 	2:10:HeaderFile:currencyvf.h
# 	2:8:ImplFile:currencyvf.cpp
# End Section
# Section EGOcx : {92D71E9C-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CLabel
# 	2:10:HeaderFile:label.h
# 	2:8:ImplFile:label.cpp
# End Section
# Section EGOcx : {05478A98-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CMarkerCollection
# 	2:10:HeaderFile:markercollection.h
# 	2:8:ImplFile:markercollection.cpp
# End Section
# Section EGOcx : {FC2C9F10-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartArea
# 	2:10:HeaderFile:chartarea.h
# 	2:8:ImplFile:chartarea.cpp
# End Section
# Section EGOcx : {FC2C9F20-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDerivedDouble
# 	2:10:HeaderFile:deriveddouble.h
# 	2:8:ImplFile:deriveddouble.cpp
# End Section
# Section EGOcx : {FC2C9F1C-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CTimeScale
# 	2:10:HeaderFile:timescale.h
# 	2:8:ImplFile:timescale.cpp
# End Section
# Section EGOcx : {5DF9BF71-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CPercentageVF
# 	2:10:HeaderFile:percentagevf.h
# 	2:8:ImplFile:percentagevf.cpp
# End Section
# Section EGOcx : {7BF6C920-333E-11D0-8126-0020AF19EE14}
# 	2:5:Class:CPolar
# 	2:10:HeaderFile:polar.h
# 	2:8:ImplFile:polar.cpp
# End Section
# Section EGOcx : {F4CC7DE2-9AE0-11CF-B9E2-00A024169263}
# 	2:5:Class:CActionMapCollection
# 	2:10:HeaderFile:actionmapcollection.h
# 	2:8:ImplFile:actionmapcollection.cpp
# End Section
# Section EGOcx : {BCC503C6-2630-11D2-BD0B-006097CF02D2}
# 	2:5:Class:CBubble
# 	2:10:HeaderFile:bubble.h
# 	2:8:ImplFile:bubble.cpp
# End Section
# Section EGOcx : {D86FB46A-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLabelCollection
# 	2:10:HeaderFile:labelcollection.h
# 	2:8:ImplFile:labelcollection.cpp
# End Section
# Section EGOcx : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section EGOcx : {C23B5F18-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DCheckNewPointsResult
# 	2:10:HeaderFile:chart2dchecknewpointsresult.h
# 	2:8:ImplFile:chart2dchecknewpointsresult.cpp
# End Section
# Section EGOcx : {5DF9BF75-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CScientificVF
# 	2:10:HeaderFile:scientificvf.h
# 	2:8:ImplFile:scientificvf.cpp
# End Section
# Section EGOcx : {05478A92-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDataCoord
# 	2:10:HeaderFile:datacoord.h
# 	2:8:ImplFile:datacoord.cpp
# End Section
# Section EGOcx : {D86FB46E-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CValueLabelCollection
# 	2:10:HeaderFile:valuelabelcollection.h
# 	2:8:ImplFile:valuelabelcollection.cpp
# End Section
# Section EGOcx : {8847CF30-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CInterior
# 	2:10:HeaderFile:interior.h
# 	2:8:ImplFile:interior.cpp
# End Section
# Section EGOcx : {FC2C9F24-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLineStyle
# 	2:10:HeaderFile:linestyle.h
# 	2:8:ImplFile:linestyle.cpp
# End Section
# Section EGOcx : {FC2C9F14-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CView3D
# 	2:10:HeaderFile:view3d.h
# 	2:8:ImplFile:view3d.cpp
# End Section
# Section EGOcx : {C23B5F13-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DDataCoordResult
# 	2:10:HeaderFile:chart2ddatacoordresult.h
# 	2:8:ImplFile:chart2ddatacoordresult.cpp
# End Section
# Section EGOcx : {92D71E98-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CBorder
# 	2:10:HeaderFile:border.h
# 	2:8:ImplFile:border.cpp
# End Section
# Section EGOcx : {D86FB462-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartGroup
# 	2:10:HeaderFile:chartgroup.h
# 	2:8:ImplFile:chartgroup.cpp
# End Section
# Section EGOcx : {D86FB472-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartLabelCollection
# 	2:10:HeaderFile:chartlabelcollection.h
# 	2:8:ImplFile:chartlabelcollection.cpp
# End Section
# Section EGOcx : {245631F0-F6C4-11CF-811C-0020AF19EE14}
# 	2:5:Class:CHiLoOpenClose
# 	2:10:HeaderFile:hiloopenclose.h
# 	2:8:ImplFile:hiloopenclose.cpp
# End Section
# Section EGOcx : {92D71E93-25A8-11CF-A640-9986B64D9618}
# 	2:21:DefaultSinkHeaderFile:chart2d.h
# 	2:16:DefaultSinkClass:CChart2D
# End Section
# Section EGOcx : {FC2C9F28-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CGridLines
# 	2:10:HeaderFile:gridlines.h
# 	2:8:ImplFile:gridlines.cpp
# End Section
# Section EGOcx : {8847CF34-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CLocation
# 	2:10:HeaderFile:location.h
# 	2:8:ImplFile:location.cpp
# End Section
# Section EGOcx : {FC2C9F18-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CFillStyle
# 	2:10:HeaderFile:fillstyle.h
# 	2:8:ImplFile:fillstyle.cpp
# End Section
# Section EGOcx : {781CFCA0-3165-11D2-AF2B-00A024C3C1C2}
# 	2:5:Class:CSliceStyle
# 	2:10:HeaderFile:slicestyle.h
# 	2:8:ImplFile:slicestyle.cpp
# End Section
# Section EGOcx : {5DF9BF79-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CValueFormat
# 	2:10:HeaderFile:valueformat.h
# 	2:8:ImplFile:valueformat.cpp
# End Section
# Section EGOcx : {2BB6A8B0-268F-11CF-A641-E996DF2C9318}
# 	2:5:Class:CLegend
# 	2:10:HeaderFile:legend.h
# 	2:8:ImplFile:legend.cpp
# End Section
# Section EGOcx : {5DF9BF6B-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CNumberVF
# 	2:10:HeaderFile:numbervf.h
# 	2:8:ImplFile:numbervf.cpp
# End Section
# Section EGOcx : {D86FB466-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartStyle
# 	2:10:HeaderFile:chartstyle.h
# 	2:8:ImplFile:chartstyle.cpp
# End Section
# Section EGOcx : {92D71E9A-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CTitle
# 	2:10:HeaderFile:title.h
# 	2:8:ImplFile:title.cpp
# End Section
# Section EGOcx : {05478A96-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CCoord
# 	2:10:HeaderFile:coord.h
# 	2:8:ImplFile:coord.cpp
# End Section
# Section EGOcx : {DA683211-1FD4-11D2-AF1F-00A024C3C1C2}
# 	2:5:Class:CAxisStyle
# 	2:10:HeaderFile:axisstyle.h
# 	2:8:ImplFile:axisstyle.cpp
# End Section
# Section EGOcx : {FC2C9F2A-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CAxisCollection
# 	2:10:HeaderFile:axiscollection.h
# 	2:8:ImplFile:axiscollection.cpp
# End Section
# Section EGOcx : {FC2C9F1A-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CPlotArea
# 	2:10:HeaderFile:plotarea.h
# 	2:8:ImplFile:plotarea.cpp
# End Section
# Section EGOcx : {53609215-4A3F-11D2-BD2A-006097CF02D2}
# 	2:5:Class:CPointStyleCollection
# 	2:10:HeaderFile:pointstylecollection.h
# 	2:8:ImplFile:pointstylecollection.cpp
# End Section
# Section EGOcx : {86E43510-1D29-11D0-B3FB-0020AFD59EF6}
# 	2:5:Class:CStringCollection
# 	2:10:HeaderFile:stringcollection.h
# 	2:8:ImplFile:stringcollection.cpp
# End Section
# Section EGOcx : {FC2C9F1E-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDerivedString
# 	2:10:HeaderFile:derivedstring.h
# 	2:8:ImplFile:derivedstring.cpp
# End Section
# Section EGOcx : {8027AB63-85DC-11D0-B4AF-0020AFD59EF6}
# 	2:5:Class:CDerivedEnum
# 	2:10:HeaderFile:derivedenum.h
# 	2:8:ImplFile:derivedenum.cpp
# End Section
# Section EGOcx : {5DF9BF73-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CFractionVF
# 	2:10:HeaderFile:fractionvf.h
# 	2:8:ImplFile:fractionvf.cpp
# End Section
# Section EGOcx : {5DF9BF6F-59A9-11D0-B476-0020AFD59EF6}
# 	2:5:Class:CAccountingVF
# 	2:10:HeaderFile:accountingvf.h
# 	2:8:ImplFile:accountingvf.cpp
# End Section
# Section EGOcx : {F4CC7DE0-9AE0-11CF-B9E2-00A024169263}
# 	2:5:Class:CActionMap
# 	2:10:HeaderFile:actionmap.h
# 	2:8:ImplFile:actionmap.cpp
# End Section
# Section EGOcx : {05478A90-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CMarker
# 	2:10:HeaderFile:marker.h
# 	2:8:ImplFile:marker.cpp
# End Section
# Section EGOcx : {FC2C9F22-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CAxis
# 	2:10:HeaderFile:axis.h
# 	2:8:ImplFile:axis.cpp
# End Section
# Section EGOcx : {FC2C9F12-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CBar
# 	2:10:HeaderFile:bar.h
# 	2:8:ImplFile:bar.cpp
# End Section
# Section EGOcx : {C23B5F11-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DCoordResult
# 	2:10:HeaderFile:chart2dcoordresult.h
# 	2:8:ImplFile:chart2dcoordresult.cpp
# End Section
# Section EGOcx : {D86FB470-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartLabel
# 	2:10:HeaderFile:chartlabel.h
# 	2:8:ImplFile:chartlabel.cpp
# End Section
# Section EGOcx : {D86FB460-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChart2DData
# 	2:10:HeaderFile:chart2ddata.h
# 	2:8:ImplFile:chart2ddata.cpp
# End Section
# Section EGOcx : {D86FB46C-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CValueLabel
# 	2:10:HeaderFile:valuelabel.h
# 	2:8:ImplFile:valuelabel.cpp
# End Section
# Section EGOcx : {FC2C9F16-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CPie
# 	2:10:HeaderFile:pie.h
# 	2:8:ImplFile:pie.cpp
# End Section
# Section EGOcx : {6E5B3FD4-4C5D-11D0-813A-0020AF19EE14}
# 	2:5:Class:CImage
# 	2:10:HeaderFile:image.h
# 	2:8:ImplFile:image.cpp
# End Section
# Section EGOcx : {92D71E91-25A8-11CF-A640-9986B64D9618}
# 	2:5:Class:CChart2D
# 	2:10:HeaderFile:chart2d.h
# 	2:8:ImplFile:chart2d.cpp
# End Section
# Section EGOcx : {C23B5F15-79F1-11D0-813F-0020AF19EE14}
# 	2:5:Class:CChart2DDataIndexResult
# 	2:10:HeaderFile:chart2ddataindexresult.h
# 	2:8:ImplFile:chart2ddataindexresult.cpp
# End Section
# Section EGOcx : {05478A94-26E2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CDataIndex
# 	2:10:HeaderFile:dataindex.h
# 	2:8:ImplFile:dataindex.cpp
# End Section
# Section EGOcx : {8847CF32-2610-11CF-A640-9986B64D9618}
# 	2:5:Class:CDerivedLong
# 	2:10:HeaderFile:derivedlong.h
# 	2:8:ImplFile:derivedlong.cpp
# End Section
# Section EGOcx : {FC2C9F26-26A2-11CF-A641-E996DF2C9318}
# 	2:5:Class:CSymbolStyle
# 	2:10:HeaderFile:symbolstyle.h
# 	2:8:ImplFile:symbolstyle.cpp
# End Section
# Section EGOcx : {245631F2-F6C4-11CF-811C-0020AF19EE14}
# 	2:5:Class:CCandle
# 	2:10:HeaderFile:candle.h
# 	2:8:ImplFile:candle.cpp
# End Section
# Section EGOcx : {D86FB464-2743-11CF-A641-E996DF2C9318}
# 	2:5:Class:CChartGroupCollection
# 	2:10:HeaderFile:chartgroupcollection.h
# 	2:8:ImplFile:chartgroupcollection.cpp
# End Section
