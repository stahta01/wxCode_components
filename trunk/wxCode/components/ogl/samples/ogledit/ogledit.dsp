# Microsoft Developer Studio Project File - Name="ogledit" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ogledit - Win32 DLL Unicode Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ogledit.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ogledit.mak" CFG="ogledit - Win32 DLL Unicode Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ogledit - Win32 DLL Unicode Debug" (based on "Win32 (x86) Application")
!MESSAGE "ogledit - Win32 DLL Debug" (based on "Win32 (x86) Application")
!MESSAGE "ogledit - Win32 Unicode Release" (based on "Win32 (x86) Application")
!MESSAGE "ogledit - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ogledit - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc6_mswuddll"
# PROP BASE Intermediate_Dir "vc6_mswuddll\ogledit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc6_mswuddll"
# PROP Intermediate_Dir "vc6_mswuddll\ogledit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\..\..\lib\vc_dll\mswud" /I ".\..\..\..\..\include" /I "." /I ".\..\..\..\..\samples" /I ".\..\..\..\include" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Fd"vc6_mswuddll\ogledit.pdb" /FD /GZ /EHsc /c
# ADD CPP /MDd /W4 /Gm /GR /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\..\include" /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /Yu"precomp.h" /Fd"vc6_mswuddll\ogledit.pdb" /FD /GZ /EHsc /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "__WXDEBUG__" /D "_UNICODE" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\..\..\lib\vc_dll\mswud" /i ".\..\..\..\..\include" /i "." /i ".\..\..\..\..\samples" /i ".\..\..\..\include" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "_UNICODE" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /d "_DEBUG" /d "__WXDEBUG__" /d "_UNICODE" /d "WXUSINGDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28ud_ogl.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\..\..\lib\vc_dll"
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXWIN)\lib\vc_dll"

!ELSEIF  "$(CFG)" == "ogledit - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "vc6_mswddll"
# PROP BASE Intermediate_Dir "vc6_mswddll\ogledit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "vc6_mswddll"
# PROP Intermediate_Dir "vc6_mswddll\ogledit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W4 /Gm /GR /Zi /Od /I ".\..\..\..\..\lib\vc_dll\mswd" /I ".\..\..\..\..\include" /I "." /I ".\..\..\..\..\samples" /I ".\..\..\..\include" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /Fd"vc6_mswddll\ogledit.pdb" /FD /GZ /EHsc /c
# ADD CPP /MDd /W4 /Gm /GR /Zi /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /I "..\..\include" /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /Yu"precomp.h" /Fd"vc6_mswddll\ogledit.pdb" /FD /GZ /EHsc /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /D "__WXDEBUG__" /D "WXUSINGDLL" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\..\..\lib\vc_dll\mswd" /i ".\..\..\..\..\include" /i "." /i ".\..\..\..\..\samples" /i ".\..\..\..\include" /d "_DEBUG" /d "__WXMSW__" /d "__WXDEBUG__" /d "WXUSINGDLL" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /d "_DEBUG" /d "__WXDEBUG__" /d "WXUSINGDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28d_ogl.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /debug /machine:I386 /libpath:".\..\..\..\..\lib\vc_dll"
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\..\lib\vc_dll"

!ELSEIF  "$(CFG)" == "ogledit - Win32 Unicode Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc6_mswu"
# PROP BASE Intermediate_Dir "vc6_mswu\ogledit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc6_mswu"
# PROP Intermediate_Dir "vc6_mswu\ogledit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I ".\..\..\..\..\lib\vc_lib\mswu" /I ".\..\..\..\..\include" /I "." /I ".\..\..\..\..\samples" /I ".\..\..\..\include" /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /Fd"vc6_mswu\ogledit.pdb" /FD /EHsc /c
# ADD CPP /MT /W4 /GR /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\..\include" /D "_UNICODE" /Yu"precomp.h" /Fd"vc6_mswu\ogledit.pdb" /FD /EHsc /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "_UNICODE" /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\..\..\lib\vc_lib\mswu" /i ".\..\..\..\..\include" /i "." /i ".\..\..\..\..\samples" /i ".\..\..\..\include" /d "__WXMSW__" /d "_UNICODE" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /d "_UNICODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28u_ogl.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\..\..\lib\vc_lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(WXWIN)\lib\vc_lib"

!ELSEIF  "$(CFG)" == "ogledit - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "vc6_msw"
# PROP BASE Intermediate_Dir "vc6_msw\ogledit"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "vc6_msw"
# PROP Intermediate_Dir "vc6_msw\ogledit"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /O2 /I ".\..\..\..\..\lib\vc_lib\msw" /I ".\..\..\..\..\include" /I "." /I ".\..\..\..\..\samples" /I ".\..\..\..\include" /D "_WINDOWS" /D "NOPCH" /Fd"vc6_msw\ogledit.pdb" /FD /EHsc /c
# ADD CPP /MT /W4 /GR /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\..\include" /Yu"precomp.h" /Fd"vc6_msw\ogledit.pdb" /FD /EHsc /c
# SUBTRACT CPP /nologo
# ADD BASE MTL /nologo /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD MTL /nologo /D "_WINDOWS" /D "NOPCH" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /i ".\..\..\..\..\lib\vc_lib\msw" /i ".\..\..\..\..\include" /i "." /i ".\..\..\..\..\samples" /i ".\..\..\..\include" /d "__WXMSW__" /d "_WINDOWS" /d "NOPCH"
# ADD RSC /l 0x409 /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wxmsw28_ogl.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib /nologo /subsystem:windows /machine:I386 /libpath:".\..\..\..\..\lib\vc_lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /libpath:"$(WXWIN)\lib\vc_lib"

!ENDIF 

# Begin Target

# Name "ogledit - Win32 DLL Unicode Debug"
# Name "ogledit - Win32 DLL Debug"
# Name "ogledit - Win32 Unicode Release"
# Name "ogledit - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\doc.cpp
# End Source File
# Begin Source File

SOURCE=.\link.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\ogledit.cpp
# End Source File
# Begin Source File

SOURCE=.\ogledit.rc
# End Source File
# Begin Source File

SOURCE=.\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\precomp.cpp
# ADD CPP /Yc"precomp.h"
# End Source File
# Begin Source File

SOURCE=.\view.cpp
# End Source File
# End Group
# End Target
# End Project
