# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.9 (http://bakefile.sourceforge.net)
#     Do not modify, all changes will be overwritten!
# =========================================================================



# -------------------------------------------------------------------------
# These are configurable options:
# -------------------------------------------------------------------------

# C++ compiler 
CXX = wpp386

# Standard flags for C++ 
CXXFLAGS = 

# Standard preprocessor flags (common for CC and CXX) 
CPPFLAGS = 

# Standard linker flags 
LDFLAGS = 

# The directory where wxWidgets library is installed 
WX_DIR = $(%WXWIN)

# Use DLL build of wx library to use? [0,1]
WX_SHARED = 0

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE = 0

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 26

# Use monolithic build of wxWidgets? [0,1]
WX_MONOLITHIC = 0



# -------------------------------------------------------------------------
# Do not modify the rest of this file!
# -------------------------------------------------------------------------

# Speed up compilation a bit:
!ifdef __LOADDLL__
!  loaddll wcc      wccd
!  loaddll wccaxp   wccdaxp
!  loaddll wcc386   wccd386
!  loaddll wpp      wppdi86
!  loaddll wppaxp   wppdaxp
!  loaddll wpp386   wppd386
!  loaddll wlink    wlink
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

WX3RDPARTYLIBPOSTFIX =
!ifeq WX_DEBUG 1
WX3RDPARTYLIBPOSTFIX = d
!endif
POSTFIX =
!ifeq WX_SHARED 0
POSTFIX = _lib
!endif
!ifeq WX_SHARED 1
POSTFIX = _dll
!endif
__httpengine_lib___depname =
!ifeq WX_SHARED 0
__httpengine_lib___depname = ..\lib\httpengine$(WXLIBPOSTFIX).lib
!endif
__httpengine_dll___depname =
!ifeq WX_SHARED 1
__httpengine_dll___depname = ..\lib\httpengine$(WXLIBPOSTFIX).dll
!endif
__WARNINGS =
!ifeq WX_DEBUG 0
__WARNINGS = 
!endif
!ifeq WX_DEBUG 1
__WARNINGS = -wx
!endif
__OPTIMIZEFLAG =
!ifeq WX_DEBUG 0
__OPTIMIZEFLAG = -ot -ox
!endif
!ifeq WX_DEBUG 1
__OPTIMIZEFLAG = -od
!endif
__DEBUGINFO =
!ifeq WX_DEBUG 0
__DEBUGINFO = -d0
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO = -d2
!endif
__DEBUGINFO_1 =
!ifeq WX_DEBUG 0
__DEBUGINFO_1 = 
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO_1 = debug all
!endif
__WXLIB_NET_NAME_p =
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)_net.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)u_net.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)d_net.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)ud_net.lib
!endif
!endif
!endif
__WXLIB_CORE_NAME_p =
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)u_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)d_core.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)ud_core.lib
!endif
!endif
!endif
__WXLIB_BASE_NAME_p =
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION).lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)u.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION).lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)u.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)d.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 0
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)ud.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)d.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)ud.lib
!endif
!endif
!endif
__WX_SHAREDDEFINE_p =
!ifeq WX_SHARED 1
__WX_SHAREDDEFINE_p = -dWXUSINGDLL
!endif
__WXUNICODE_DEFINE_p =
!ifeq WX_UNICODE 1
__WXUNICODE_DEFINE_p = -d_UNICODE
!endif
__WXDEBUG_DEFINE_p =
!ifeq WX_DEBUG 1
__WXDEBUG_DEFINE_p = -d__WXDEBUG__
!endif
WXLIBPOSTFIX =
!ifeq WX_DEBUG 0
!ifeq WX_UNICODE 1
WXLIBPOSTFIX = u
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_UNICODE 0
WXLIBPOSTFIX = d
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_UNICODE 1
WXLIBPOSTFIX = ud
!endif
!endif
WXLIBPATH =
!ifeq WX_SHARED 0
WXLIBPATH = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
WXLIBPATH = \lib\wat_dll
!endif

### Variables: ###

HTTPENGINE_LIB_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
HTTPENGINE_LIB_OBJECTS =  &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_base64.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_authdlg.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_httpbuilder.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_httpbuilderthread.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_hyperlinkctrl.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_pleasewaitdlg.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_proxysettingsdlg.obj
HTTPENGINE_DLL_CXXFLAGS = -bd $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -dWXMAKINGDLL_HTTPENGINE $(CPPFLAGS) &
	$(CXXFLAGS)
HTTPENGINE_DLL_OBJECTS =  &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_base64.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_authdlg.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_httpbuilder.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_httpbuilderthread.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_hyperlinkctrl.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_pleasewaitdlg.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_proxysettingsdlg.obj
HTTPENGINETEST_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -dprova -i=provainc $(CPPFLAGS) &
	$(CXXFLAGS)
HTTPENGINETEST_OBJECTS =  &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpengineapp.obj &
	watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpenginedlg.obj


all : watcom$(WXLIBPOSTFIX)$(POSTFIX)
watcom$(WXLIBPOSTFIX)$(POSTFIX) :
	-if not exist watcom$(WXLIBPOSTFIX)$(POSTFIX) mkdir watcom$(WXLIBPOSTFIX)$(POSTFIX)

### Targets: ###

all : .SYMBOLIC $(__httpengine_lib___depname) $(__httpengine_dll___depname) ..\sample\httpenginetest.exe

clean : .SYMBOLIC 
	-if exist watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.obj del watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.obj
	-if exist watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.res del watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.res
	-if exist watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.lbc del watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.lbc
	-if exist watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.ilk del watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.ilk
	-if exist watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.pch del watcom$(WXLIBPOSTFIX)$(POSTFIX)\*.pch
	-if exist ..\lib\httpengine$(WXLIBPOSTFIX).lib del ..\lib\httpengine$(WXLIBPOSTFIX).lib
	-if exist ..\lib\httpengine$(WXLIBPOSTFIX).dll del ..\lib\httpengine$(WXLIBPOSTFIX).dll
	-if exist ..\lib\httpengine$(WXLIBPOSTFIX).lib del ..\lib\httpengine$(WXLIBPOSTFIX).lib
	-if exist ..\sample\httpenginetest.exe del ..\sample\httpenginetest.exe

tarball :  
	-cd ..
	-tar -cvzf ..\httpengine.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* *

zip :  
	-cd ..
	-zip -r9 ..\httpengine.zip *  -x *.pdb -x *.log -x *.o*

deepclean :  
	-del /S ..\*.a
	-del /S ..\*.lib
	-del /S ..\*.pdb
	-del /S ..\*.dll
	-del /S ..\*.exp
	-del /S ..\*.so*
	-del /S ..\*.exe
	-del /S ..\*.obj
	-del /S ..\*.o
	-del /S ..\*.log
	-del /S ..\*.manifest*
	-del /S ..\*.log
	-del /S ..\.bakefile_gen.state
	-del /S ..\*.pch
	-del /S ..\*.ncb
	-del /S ..\*.plg
	-del /S ..\*.ncb
	-del /S ..\*.aps
	-del /S ..\*.suo
	-del /S ..\*.user
	-del /S ..\*.res
	-del /S ..\*.il?
	-del /S ..\*.tds
	-del /S ..\*.idb
	-del /S ..\*.map
	-if exist ..\autom4te.cache rmdir /S /Q ..\autom4te.cache
	-if exist ..\.deps rmdir /S /Q ..\.deps
	-del /S ..\config.status
	-del /S ..\config.log
	-del /S ..\Makefile
	-del /S ..\bk-deps
	-del /S .\*.a
	-del /S .\*.lib
	-del /S .\*.pdb
	-del /S .\*.dll
	-del /S .\*.exp
	-del /S .\*.so*
	-del /S .\*.exe
	-del /S .\*.obj
	-del /S .\*.o
	-del /S .\*.log
	-del /S .\*.manifest*
	-del /S .\*.log
	-del /S .\.bakefile_gen.state
	-del /S .\*.pch
	-del /S .\*.ncb
	-del /S .\*.plg
	-del /S .\*.ncb
	-del /S .\*.aps
	-del /S .\*.suo
	-del /S .\*.user
	-del /S .\*.res
	-del /S .\*.il?
	-del /S .\*.tds
	-del /S .\*.idb
	-del /S .\*.map
	-if exist .\autom4te.cache rmdir /S /Q .\autom4te.cache
	-if exist .\.deps rmdir /S /Q .\.deps
	-del /S .\config.status
	-del /S .\config.log
	-del /S .\Makefile
	-del /S .\bk-deps

docs :  
	-cd ..\docs
	-doxygen

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

install : .SYMBOLIC all install-wxheaders
	copy /Y ..\lib\httpengine$(WXLIBPOSTFIX).lib $(WX_DIR)$(WXLIBPATH)
	copy /Y ..\lib\httpengine$(WXLIBPOSTFIX).dll $(WX_DIR)$(WXLIBPATH)

install-wxheaders :  
	mkdir $(WX_DIR)\include\wx
	cd .
	copy /Y  ..\include\wx\*.h $(WX_DIR)\include\wx

uninstall : .SYMBOLIC 
	-if exist $(WX_DIR)\include\wx\authdlg.h \
	$(WX_DIR)\include\wx\base64.h \
	$(WX_DIR)\include\wx\httpbuilder.h \
	$(WX_DIR)\include\wx\httpbuilderthread.h \
	$(WX_DIR)\include\wx\hyperlinkctrl.h \
	$(WX_DIR)\include\wx\proxysettingsdlg.h \
	$(WX_DIR)\include\wx\httpenginedef.h del /Q $(WX_DIR)\include\wx\authdlg.h \
	$(WX_DIR)\include\wx\base64.h \
	$(WX_DIR)\include\wx\httpbuilder.h \
	$(WX_DIR)\include\wx\httpbuilderthread.h \
	$(WX_DIR)\include\wx\hyperlinkctrl.h \
	$(WX_DIR)\include\wx\proxysettingsdlg.h \
	$(WX_DIR)\include\wx\httpenginedef.h
	-if exist $(WX_DIR)$(WXLIBPATH)\*httpengine* del /Q $(WX_DIR)$(WXLIBPATH)\*httpengine*

!ifeq WX_SHARED 0
..\lib\httpengine$(WXLIBPOSTFIX).lib :  $(HTTPENGINE_LIB_OBJECTS)
	@%create watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib.lbc
	@for %i in ($(HTTPENGINE_LIB_OBJECTS)) do @%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib.lbc
!endif

!ifeq WX_SHARED 1
..\lib\httpengine$(WXLIBPOSTFIX).dll :  $(HTTPENGINE_DLL_OBJECTS)
	@%create watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc option quiet
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc name $^@
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc option caseexact
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib
	@for %i in ($(HTTPENGINE_DLL_OBJECTS)) do @%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc file %i
	@for %i in ( $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc library %i
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc system nr_dll
	wlink @watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll.lbc
	wlib -q -n -b ..\lib\httpengine$(WXLIBPOSTFIX).lib +$^@
!endif

..\sample\httpenginetest.exe :  $(HTTPENGINETEST_OBJECTS) watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpengine.res $(__httpengine_lib___depname)
	@%create watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc option quiet
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc name $^@
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc option caseexact
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(WXLIBPATH) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(HTTPENGINETEST_OBJECTS)) do @%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc file %i
	@for %i in ( ..\lib\httpengine$(WXLIBPOSTFIX).lib $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc library %i
	@%append watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc option resource=watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpengine.res
	wlink @watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest.lbc

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_base64.obj :  .AUTODEPEND .\..\src\base64.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_authdlg.obj :  .AUTODEPEND .\..\src\authdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_httpbuilder.obj :  .AUTODEPEND .\..\src\httpbuilder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_httpbuilderthread.obj :  .AUTODEPEND .\..\src\httpbuilderthread.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_hyperlinkctrl.obj :  .AUTODEPEND .\..\src\hyperlinkctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_pleasewaitdlg.obj :  .AUTODEPEND .\..\src\pleasewaitdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_lib_proxysettingsdlg.obj :  .AUTODEPEND .\..\src\proxysettingsdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_LIB_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_base64.obj :  .AUTODEPEND .\..\src\base64.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_authdlg.obj :  .AUTODEPEND .\..\src\authdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_httpbuilder.obj :  .AUTODEPEND .\..\src\httpbuilder.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_httpbuilderthread.obj :  .AUTODEPEND .\..\src\httpbuilderthread.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_hyperlinkctrl.obj :  .AUTODEPEND .\..\src\hyperlinkctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_pleasewaitdlg.obj :  .AUTODEPEND .\..\src\pleasewaitdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpengine_dll_proxysettingsdlg.obj :  .AUTODEPEND .\..\src\proxysettingsdlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINE_DLL_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpengineapp.obj :  .AUTODEPEND .\..\sample\httpengineapp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINETEST_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpenginedlg.obj :  .AUTODEPEND .\..\sample\httpenginedlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(HTTPENGINETEST_CXXFLAGS) $<

watcom$(WXLIBPOSTFIX)$(POSTFIX)\httpenginetest_httpengine.res :  .AUTODEPEND .\..\sample\httpengine.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\sample -dprova -i=provainc $<

