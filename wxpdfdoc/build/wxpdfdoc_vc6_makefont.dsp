# Microsoft Developer Studio Project File - Name="wxpdfdoc_vc6_makefont" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=makefont - Win32 Static ANSI Release Multilib Static
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_makefont.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxpdfdoc_vc6_makefont.mak" CFG="makefont - Win32 Static ANSI Release Multilib Static"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "makefont - Win32 DLL Unicode Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL Unicode Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 DLL ANSI Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static Unicode Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Debug Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Debug Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Debug Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Debug Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Release Monolithic DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Release Monolithic Static" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Release Multilib DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "makefont - Win32 Static ANSI Release Multilib Static" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "makefont - Win32 DLL Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 DLL ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib\vc_dll" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxmsw28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswud\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswud\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28ud_pdfdoc.lib wxbase28ud_xml.lib wxmsw28ud_core.lib wxbase28ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxmsw28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static Unicode Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswu\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswu\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28u_pdfdoc.lib wxbase28u_xml.lib wxmsw28u_core.lib wxbase28u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Debug Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Debug Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxmsw28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Debug Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Debug Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmswd\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmswd\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /Od /Gm /W4 /I "..\include" /I "..\makefont" /Zi /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXDEBUG__" /D "__WXMSW__" /D "_CONSOLE" /D "_DEBUG" /c
# ADD BASE RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
# ADD RSC /l 0x409 /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i "..\makefont" /d _DEBUG
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28d_pdfdoc.lib wxbase28d_xml.lib wxmsw28d_core.lib wxbase28d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /debug /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Release Monolithic DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Release Monolithic Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxmsw28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Release Multilib DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw_dll\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw_dll\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_dll\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ELSEIF  "$(CFG)" == "makefont - Win32 Static ANSI Release Multilib Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\makefont"
# PROP BASE Intermediate_Dir "vcmsw\makefont"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\makefont"
# PROP Intermediate_Dir "vcmsw\makefont"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /O2 /W4 /I "..\include" /I "..\makefont" /Fd..\makefont\makefont.pdb /D "WIN32" /D "__WXMSW__" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /d "_CONSOLE" /i ..\makefont
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"
# ADD LINK32 ..\lib\vc_lib\wxcode_msw28_pdfdoc.lib wxbase28_xml.lib wxmsw28_core.lib wxbase28.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib oleacc.lib /nologo /machine:i386 /out:"..\makefont\makefont.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib\vc_lib" /subsystem:console /pdb:"..\makefont\makefont.pdb"

!ENDIF

# Begin Target

# Name "makefont - Win32 DLL Unicode Debug Monolithic DLL"
# Name "makefont - Win32 DLL Unicode Debug Monolithic Static"
# Name "makefont - Win32 DLL Unicode Debug Multilib DLL"
# Name "makefont - Win32 DLL Unicode Debug Multilib Static"
# Name "makefont - Win32 DLL Unicode Release Monolithic DLL"
# Name "makefont - Win32 DLL Unicode Release Monolithic Static"
# Name "makefont - Win32 DLL Unicode Release Multilib DLL"
# Name "makefont - Win32 DLL Unicode Release Multilib Static"
# Name "makefont - Win32 DLL ANSI Debug Monolithic DLL"
# Name "makefont - Win32 DLL ANSI Debug Monolithic Static"
# Name "makefont - Win32 DLL ANSI Debug Multilib DLL"
# Name "makefont - Win32 DLL ANSI Debug Multilib Static"
# Name "makefont - Win32 DLL ANSI Release Monolithic DLL"
# Name "makefont - Win32 DLL ANSI Release Monolithic Static"
# Name "makefont - Win32 DLL ANSI Release Multilib DLL"
# Name "makefont - Win32 DLL ANSI Release Multilib Static"
# Name "makefont - Win32 Static Unicode Debug Monolithic DLL"
# Name "makefont - Win32 Static Unicode Debug Monolithic Static"
# Name "makefont - Win32 Static Unicode Debug Multilib DLL"
# Name "makefont - Win32 Static Unicode Debug Multilib Static"
# Name "makefont - Win32 Static Unicode Release Monolithic DLL"
# Name "makefont - Win32 Static Unicode Release Monolithic Static"
# Name "makefont - Win32 Static Unicode Release Multilib DLL"
# Name "makefont - Win32 Static Unicode Release Multilib Static"
# Name "makefont - Win32 Static ANSI Debug Monolithic DLL"
# Name "makefont - Win32 Static ANSI Debug Monolithic Static"
# Name "makefont - Win32 Static ANSI Debug Multilib DLL"
# Name "makefont - Win32 Static ANSI Debug Multilib Static"
# Name "makefont - Win32 Static ANSI Release Monolithic DLL"
# Name "makefont - Win32 Static ANSI Release Monolithic Static"
# Name "makefont - Win32 Static ANSI Release Multilib DLL"
# Name "makefont - Win32 Static ANSI Release Multilib Static"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\makefont\makefont.cpp
# End Source File
# Begin Source File

SOURCE=..\makefont\makefont.rc
# End Source File
# End Group
# End Target
# End Project

