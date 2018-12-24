# Microsoft Developer Studio Project File - Name="BackgroundRandomizer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=BackgroundRandomizer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BackgroundRandomizer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BackgroundRandomizer.mak" CFG="BackgroundRandomizer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BackgroundRandomizer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "BackgroundRandomizer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BackgroundRandomizer - Win32 Release"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\..\Source Code\paintlib\paintlib\win\paintlib" /I "..\..\Source Code\paintlib\paintlib\common" /I "..\..\Source Code\paintlib\paintlib\common\filter" /I "..\..\Source Code\paintlib\otherlib\libtiff\libtiff" /I "..\..\Source Code\paintlib\libjpeg" /I "..\..\Source Code\paintlib\otherlib\libpng" /I "..\..\Source Code\paintlib\otherlib\zlib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib libpng.lib libungif.lib vfw32.lib /nologo /subsystem:windows /machine:I386 /libpath:"..\..\Source Code\paintlib\lib\release"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "BackgroundRandomizer - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\Source Code\paintlib\paintlib\win\paintlib" /I "..\..\Source Code\paintlib\paintlib\common" /I "..\..\Source Code\paintlib\paintlib\common\filter" /I "..\..\Source Code\paintlib\otherlib\libtiff\libtiff" /I "..\..\Source Code\paintlib\libjpeg" /I "..\..\Source Code\paintlib\otherlib\libpng" /I "..\..\Source Code\paintlib\otherlib\zlib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 paintlib.lib libjpeg.lib libtiff.lib libpng.lib libungif.lib vfw32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"..\..\Source Code\paintlib\lib\debug"
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "BackgroundRandomizer - Win32 Release"
# Name "BackgroundRandomizer - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BackgroundRandomizer.cpp
# End Source File
# Begin Source File

SOURCE=.\BackgroundRandomizer.rc
# End Source File
# Begin Source File

SOURCE=.\BackgroundRandomizerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BgRandPopupAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorStaticST.cpp
# End Source File
# Begin Source File

SOURCE=.\DesktopInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAdjustImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgManageBlockedImages.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgRegister.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphicsConverter.cpp
# End Source File
# Begin Source File

SOURCE=..\Registration\RegCode.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SystemTray.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BackgroundRandomizer.h
# End Source File
# Begin Source File

SOURCE=.\BackgroundRandomizerDlg.h
# End Source File
# Begin Source File

SOURCE=.\BgRandPopupAbout.h
# End Source File
# Begin Source File

SOURCE=.\ColorStaticST.h
# End Source File
# Begin Source File

SOURCE=.\DesktopInterface.h
# End Source File
# Begin Source File

SOURCE=.\DlgAdjustImage.h
# End Source File
# Begin Source File

SOURCE=.\DlgManageBlockedImages.h
# End Source File
# Begin Source File

SOURCE=.\DlgRegister.h
# End Source File
# Begin Source File

SOURCE=.\GraphicsConverter.h
# End Source File
# Begin Source File

SOURCE=..\Registration\RegCode.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SystemTray.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\BackgroundRandomizer.ico
# End Source File
# Begin Source File

SOURCE=.\res\BackgroundRandomizer.rc2
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_c.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\creditcards.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
