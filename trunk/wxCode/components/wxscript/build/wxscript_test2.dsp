# Microsoft Developer Studio Project File - Name="wxscript_test2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=test2 - Win32 Default
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_test2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "wxscript_test2.mak" CFG="test2 - Win32 Default"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "test2 - Win32 DLL Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 DLL Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 DLL Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 Unicode Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 Unicode" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "test2 - Win32 Default" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "test2 - Win32 DLL Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswud" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 DLL Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\mswu" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "WXUSINGDLL" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 DLL Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_dll\mswd" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "WXUSINGDLL" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "_DEBUG" /d "WXUSINGDLL" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_dll\msw" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "WXUSINGDLL" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "WXUSINGDLL" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_dll\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_dll" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswud" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "_DEBUG" /d "_UNICODE" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswud" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptud.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptud.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26ud_core.lib wxbase26ud.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexud.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\mswu" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_UNICODE" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "_UNICODE" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswu" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscriptu.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26u_core.lib wxbase26u.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregexu.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MDd /GR /EHsc /W4 /Od /I "$(WXWIN)\lib\vc_lib\mswd" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Zi /Gm /GZ /Fd..\tests\test2\test2.pdb /D "WIN32" /D "_DEBUG" /D "__WXDEBUG__" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "_DEBUG" /d "__WXDEBUG__" /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\mswd" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscriptd.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console
# ADD LINK32 ..\lib\wxscriptd.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26d_core.lib wxbase26d.lib wxtiffd.lib wxjpegd.lib wxpngd.lib wxzlibd.lib wxregexd.lib wxexpatd.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /debug /subsystem:console

!ELSEIF  "$(CFG)" == "test2 - Win32 Default"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "..\tests\test2"
# PROP BASE Intermediate_Dir "msvc6prj\test2"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\tests\test2"
# PROP Intermediate_Dir "msvc6prj\test2"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD CPP /nologo /FD /MD /GR /EHsc /w /O2 /I "$(WXWIN)\lib\vc_lib\msw" /I "$(WXWIN)\include" /I "..\include" /I "c:\Python\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\lua\include" /I "..\thirdparty\tolua\include" /I "..\thirdparty\tolua\include" /I "c:\cint" /I "c:\ucc" /Fd..\tests\test2\test2.pdb /D "WIN32" /D "__WXMSW__" /D "wxSCRIPT_NO_UNDERC" /D "wxSCRIPT_NO_CINT" /D "_CONSOLE" /c
# ADD BASE RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
# ADD RSC /l 0x409 /d "__WXMSW__" /i "$(WXWIN)\lib\vc_lib\msw" /i "$(WXWIN)\include" /i "..\include" /i "c:\Python\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\lua\include" /i "..\thirdparty\tolua\include" /i "..\thirdparty\tolua\include" /i "c:\cint" /i "c:\ucc" /d "wxSCRIPT_NO_UNDERC" /d "wxSCRIPT_NO_CINT" /d _CONSOLE
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console
# ADD LINK32 ..\lib\wxscript.lib python24.lib lua.lib lualib.lib tolua.lib wxmsw26_core.lib wxbase26.lib wxtiff.lib wxjpeg.lib wxpng.lib wxzlib.lib wxregex.lib wxexpat.lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib oleacc.lib /nologo /machine:i386 /out:"..\tests\test2\test2.exe" /libpath:"$(WXWIN)\lib\vc_lib" /libpath:"..\lib" /libpath:"c:\Python\libs" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\lua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"..\thirdparty\tolua\lib" /libpath:"c:\cint" /libpath:"c:\ucc" /subsystem:console

!ENDIF

# Begin Target

# Name "test2 - Win32 DLL Unicode Debug"
# Name "test2 - Win32 DLL Unicode"
# Name "test2 - Win32 DLL Debug"
# Name "test2 - Win32 DLL"
# Name "test2 - Win32 Unicode Debug"
# Name "test2 - Win32 Unicode"
# Name "test2 - Win32 Debug"
# Name "test2 - Win32 Default"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\..\tests\test2\Test.cpp
# End Source File
# Begin Source File

SOURCE=.\..\tests\test2\stackwalker.cpp
# End Source File
# End Group
# End Target
# End Project

