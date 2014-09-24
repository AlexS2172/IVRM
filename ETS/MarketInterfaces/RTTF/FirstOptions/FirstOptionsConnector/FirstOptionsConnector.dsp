# Microsoft Developer Studio Project File - Name="FirstOptionsConnector" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=FirstOptionsConnector - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FirstOptionsConnector.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FirstOptionsConnector.mak" CFG="FirstOptionsConnector - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FirstOptionsConnector - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "FirstOptionsConnector - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/ETS/Production 2.1 CIBC/MarketInterfaces/RTTF/FirstOptions/FirstOptionsConnector", YOZCAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FirstOptionsConnector - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\include" /I "../../../../../EgarCommonLibrary/Include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib Version.lib /nologo /subsystem:console /pdb:"../../../bin/FirstOptionsConnector.pdb" /debug /machine:I386 /out:"../../../bin/FocConnector.exe"
# SUBTRACT LINK32 /pdb:none /map

!ELSEIF  "$(CFG)" == "FirstOptionsConnector - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "..\include" /I "../../../../../EgarCommonLibrary/Include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ws2_32.lib Version.lib /nologo /subsystem:console /incremental:no /pdb:"../../../bin/FirstOptionsConnectorD.pdb" /map:"../Debug/FirstOptionsConnectorD.map" /debug /machine:I386 /out:"../../../bin/FocConnectorD.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "FirstOptionsConnector - Win32 Release"
# Name "FirstOptionsConnector - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "XDR"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XDR.cpp
# End Source File
# Begin Source File

SOURCE=.\XDRFloat.cpp
# End Source File
# Begin Source File

SOURCE=.\XDRRec.cpp
# End Source File
# End Group
# Begin Group "FttfTypes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CsMsgs.cpp
# End Source File
# Begin Source File

SOURCE=.\CsTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\RttfMsgs.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\crds.rc
# End Source File
# Begin Source File

SOURCE=.\CRS.cpp
# End Source File
# Begin Source File

SOURCE=.\lasttrade.cpp
# End Source File
# Begin Source File

SOURCE=.\portfolio.cpp
# End Source File
# Begin Source File

SOURCE=.\PositionFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PubImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\Publisher.cpp
# End Source File
# Begin Source File

SOURCE=.\RttfClient.cpp
# End Source File
# Begin Source File

SOURCE=.\rttfproc.cpp
# End Source File
# Begin Source File

SOURCE=.\rttfthreads.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\trace.cpp
# End Source File
# Begin Source File

SOURCE=.\TraceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\crs.h
# End Source File
# Begin Source File

SOURCE=.\lasttrade.h
# End Source File
# Begin Source File

SOURCE=.\portfolio.h
# End Source File
# Begin Source File

SOURCE=.\PositionFile.h
# End Source File
# Begin Source File

SOURCE=.\Publisher.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\RttfClient.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\trace.h
# End Source File
# Begin Source File

SOURCE=.\TraceItem.h
# End Source File
# Begin Source File

SOURCE=.\VersionInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
