========================================================================
    CONSOLE APPLICATION : EgSendMail Project Overview
========================================================================

AppWizard has created this EgSendMail application for you.  

This file contains a summary of what you will find in each of the files that
make up your EgSendMail application.


EgSendMail.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

EgSendMail.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named EgSendMail.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

This programm for error reporting via mail
Default mailaddres is achuchev@egartech.com
Params via command line: 
-IP="192.168.8.3" - EgarSMTPServer
-DS="description"
-Mail="address@server.com"

Params from file egsmconfig.ini. This file must be located in directory, where this programm running.
-IP="192.168.8.3" - EgarSMTPServer
-Mail="address@server.com" - TO:


/////////////////////////////////////////////////////////////////////////////
