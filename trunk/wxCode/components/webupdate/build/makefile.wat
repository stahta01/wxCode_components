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

# If 1 then wxHTTPEngine component will be used for proxy/authentication stuff [1,0]
USE_HTTPENGINE = 0

# The path to the wxHTTPBuilder (httpengine) component 
HTTPENGINE_DIR = ..\..\wxhttpengine

# The directory where wxWidgets library is installed 
WX_DIR = $(%WXWIN)

# Use DLL build of wx library to use? [0,1]
WX_SHARED = 0

# Compile Unicode build of wxWidgets? [0,1]
WX_UNICODE = 1

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 26



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
__HTTPENGINEDEP_LIB_p =
!ifeq USE_HTTPENGINE 1
__HTTPENGINEDEP_LIB_p = httpengine$(WXLIBPOSTFIX).lib
!endif
__HTTPENGINEDEP_DEF_p =
!ifeq USE_HTTPENGINE 1
__HTTPENGINEDEP_DEF_p = -dwxUSE_HTTPENGINE
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
__DEBUGINFO_5 =
!ifeq WX_DEBUG 0
__DEBUGINFO_5 = 
!endif
!ifeq WX_DEBUG 1
__DEBUGINFO_5 = debug all
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
__WXLIBPATH_FILENAMES =
!ifeq WX_SHARED 0
__WXLIBPATH_FILENAMES = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
__WXLIBPATH_FILENAMES = \lib\wat_dll
!endif

### Variables: ###

WEBUPDATER_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=$(HTTPENGINE_DIR)\include &
	$(__HTTPENGINEDEP_DEF_p) $(CPPFLAGS) $(CXXFLAGS)
WEBUPDATER_OBJECTS =  &
	watcom\webupdater_webupdate.obj &
	watcom\webupdater_webupdatedlg.obj &
	watcom\webupdater_checkedlistctrl.obj &
	watcom\webupdater_md5.obj &
	watcom\webupdater_installer.obj &
	watcom\webupdater_download.obj &
	watcom\webupdater_stdactions.obj &
	watcom\webupdater_app.obj
SIMPLE_1_0_0_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=..\..\include $(CPPFLAGS) $(CXXFLAGS)
SIMPLE_1_0_0_OBJECTS =  &
	watcom\simple_1_0_0_minimal.obj
SIMPLE_2_0_3_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=..\..\include $(CPPFLAGS) $(CXXFLAGS)
SIMPLE_2_0_3_OBJECTS =  &
	watcom\simple_2_0_3_minimal.obj
ADVANCED_0_0_1_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=..\..\include $(CPPFLAGS) $(CXXFLAGS)
ADVANCED_0_0_1_OBJECTS =  &
	watcom\advanced_0_0_1_minimal.obj
ADVANCED_1_5_0_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -i=..\..\include $(CPPFLAGS) $(CXXFLAGS)
ADVANCED_1_5_0_OBJECTS =  &
	watcom\advanced_1_5_0_minimal.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC rep4_simple100 rep4_simple203 rep4_adv001 rep4_adv150 ..\src\webupdater.exe ..\samples\simple\v1.0.0\simple.exe ..\samples\simple\v2.0.3\simple.exe ..\samples\advanced\v0.0.1\advanced.exe ..\samples\advanced\v1.5.0\advanced.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\src\webupdater.exe del ..\src\webupdater.exe
	-if exist ..\samples\simple\v1.0.0\simple.exe del ..\samples\simple\v1.0.0\simple.exe
	-if exist ..\samples\simple\v2.0.3\simple.exe del ..\samples\simple\v2.0.3\simple.exe
	-if exist ..\samples\advanced\v0.0.1\advanced.exe del ..\samples\advanced\v0.0.1\advanced.exe
	-if exist ..\samples\advanced\v1.5.0\advanced.exe del ..\samples\advanced\v1.5.0\advanced.exe

tarball :  
	-cd ..
	-tar -cvzf ..\webupdate.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* *

zip :  
	-cd ..
	-zip -r9 ..\webupdate.zip *  -x *.pdb -x *.log -x *.o*

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

rep4_simple100 : .SYMBOLIC 
	if not exist ..\samples\simple\v1.0.0 mkdir ..\samples\simple\v1.0.0
	for %f in (webupdater.exe webupdatedlg.xrc) do if not exist ..\samples\simple\v1.0.0\%f copy ..\src\%f ..\samples\simple\v1.0.0

rep4_simple203 : .SYMBOLIC 
	if not exist ..\samples\simple\v2.0.3 mkdir ..\samples\simple\v2.0.3
	for %f in (webupdater.exe webupdatedlg.xrc) do if not exist ..\samples\simple\v2.0.3\%f copy ..\src\%f ..\samples\simple\v2.0.3

rep4_adv001 : .SYMBOLIC 
	if not exist ..\samples\advanced\v0.0.1 mkdir ..\samples\advanced\v0.0.1
	for %f in (webupdater.exe webupdatedlg.xrc) do if not exist ..\samples\advanced\v0.0.1\%f copy ..\src\%f ..\samples\advanced\v0.0.1

rep4_adv150 : .SYMBOLIC 
	if not exist ..\samples\advanced\v1.5.0 mkdir ..\samples\advanced\v1.5.0
	for %f in (webupdater.exe webupdatedlg.xrc) do if not exist ..\samples\advanced\v1.5.0\%f copy ..\src\%f ..\samples\advanced\v1.5.0

..\src\webupdater.exe :  $(WEBUPDATER_OBJECTS) watcom\webupdater_app.res
	@%create watcom\webupdater.lbc
	@%append watcom\webupdater.lbc option quiet
	@%append watcom\webupdater.lbc name $^@
	@%append watcom\webupdater.lbc option caseexact
	@%append watcom\webupdater.lbc $(LDFLAGS) $(__DEBUGINFO_5)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib libpath $(HTTPENGINE_DIR)\lib system nt_win ref '_WinMain@16'
	@for %i in ($(WEBUPDATER_OBJECTS)) do @%append watcom\webupdater.lbc file %i
	@for %i in ( $(__HTTPENGINEDEP_LIB_p) wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_adv.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_xrc.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_xml.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_net.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\webupdater.lbc library %i
	@%append watcom\webupdater.lbc option resource=watcom\webupdater_app.res
	wlink @watcom\webupdater.lbc

..\samples\simple\v1.0.0\simple.exe :  $(SIMPLE_1_0_0_OBJECTS) watcom\simple_1_0_0_minimal.res
	@%create watcom\simple_1_0_0.lbc
	@%append watcom\simple_1_0_0.lbc option quiet
	@%append watcom\simple_1_0_0.lbc name $^@
	@%append watcom\simple_1_0_0.lbc option caseexact
	@%append watcom\simple_1_0_0.lbc $(LDFLAGS) $(__DEBUGINFO_5)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(SIMPLE_1_0_0_OBJECTS)) do @%append watcom\simple_1_0_0.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\simple_1_0_0.lbc library %i
	@%append watcom\simple_1_0_0.lbc option resource=watcom\simple_1_0_0_minimal.res
	wlink @watcom\simple_1_0_0.lbc

..\samples\simple\v2.0.3\simple.exe :  $(SIMPLE_2_0_3_OBJECTS) watcom\simple_2_0_3_minimal.res
	@%create watcom\simple_2_0_3.lbc
	@%append watcom\simple_2_0_3.lbc option quiet
	@%append watcom\simple_2_0_3.lbc name $^@
	@%append watcom\simple_2_0_3.lbc option caseexact
	@%append watcom\simple_2_0_3.lbc $(LDFLAGS) $(__DEBUGINFO_5)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(SIMPLE_2_0_3_OBJECTS)) do @%append watcom\simple_2_0_3.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\simple_2_0_3.lbc library %i
	@%append watcom\simple_2_0_3.lbc option resource=watcom\simple_2_0_3_minimal.res
	wlink @watcom\simple_2_0_3.lbc

..\samples\advanced\v0.0.1\advanced.exe :  $(ADVANCED_0_0_1_OBJECTS) watcom\advanced_0_0_1_minimal.res
	@%create watcom\advanced_0_0_1.lbc
	@%append watcom\advanced_0_0_1.lbc option quiet
	@%append watcom\advanced_0_0_1.lbc name $^@
	@%append watcom\advanced_0_0_1.lbc option caseexact
	@%append watcom\advanced_0_0_1.lbc $(LDFLAGS) $(__DEBUGINFO_5)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(ADVANCED_0_0_1_OBJECTS)) do @%append watcom\advanced_0_0_1.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\advanced_0_0_1.lbc library %i
	@%append watcom\advanced_0_0_1.lbc option resource=watcom\advanced_0_0_1_minimal.res
	wlink @watcom\advanced_0_0_1.lbc

..\samples\advanced\v1.5.0\advanced.exe :  $(ADVANCED_1_5_0_OBJECTS) watcom\advanced_1_5_0_minimal.res
	@%create watcom\advanced_1_5_0.lbc
	@%append watcom\advanced_1_5_0.lbc option quiet
	@%append watcom\advanced_1_5_0.lbc name $^@
	@%append watcom\advanced_1_5_0.lbc option caseexact
	@%append watcom\advanced_1_5_0.lbc $(LDFLAGS) $(__DEBUGINFO_5)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(ADVANCED_1_5_0_OBJECTS)) do @%append watcom\advanced_1_5_0.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\advanced_1_5_0.lbc library %i
	@%append watcom\advanced_1_5_0.lbc option resource=watcom\advanced_1_5_0_minimal.res
	wlink @watcom\advanced_1_5_0.lbc

watcom\webupdater_app.res :  .AUTODEPEND ..\src\..\src\app.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=$(HTTPENGINE_DIR)\include $(__HTTPENGINEDEP_DEF_p) -i=..\src $<

watcom\webupdater_webupdate.obj :  .AUTODEPEND ..\src\webupdate.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_webupdatedlg.obj :  .AUTODEPEND ..\src\webupdatedlg.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_checkedlistctrl.obj :  .AUTODEPEND ..\src\checkedlistctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_md5.obj :  .AUTODEPEND ..\src\md5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_installer.obj :  .AUTODEPEND ..\src\installer.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_download.obj :  .AUTODEPEND ..\src\download.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_stdactions.obj :  .AUTODEPEND ..\src\stdactions.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\webupdater_app.obj :  .AUTODEPEND ..\src\app.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WEBUPDATER_CXXFLAGS) $<

watcom\simple_1_0_0_minimal.obj :  .AUTODEPEND ..\src\..\samples\simple\v1.0.0\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SIMPLE_1_0_0_CXXFLAGS) $<

watcom\simple_1_0_0_minimal.res :  .AUTODEPEND ..\src\..\samples\simple\v1.0.0\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\..\include -i=..\samples\simple\v1.0.0 $<

watcom\simple_2_0_3_minimal.obj :  .AUTODEPEND ..\src\..\samples\simple\v2.0.3\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SIMPLE_2_0_3_CXXFLAGS) $<

watcom\simple_2_0_3_minimal.res :  .AUTODEPEND ..\src\..\samples\simple\v2.0.3\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\..\include -i=..\samples\simple\v2.0.3 $<

watcom\advanced_0_0_1_minimal.obj :  .AUTODEPEND ..\src\..\samples\advanced\v0.0.1\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(ADVANCED_0_0_1_CXXFLAGS) $<

watcom\advanced_0_0_1_minimal.res :  .AUTODEPEND ..\src\..\samples\advanced\v0.0.1\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\..\include -i=..\samples\advanced\v0.0.1 $<

watcom\advanced_1_5_0_minimal.obj :  .AUTODEPEND ..\src\..\samples\advanced\v1.5.0\minimal.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(ADVANCED_1_5_0_CXXFLAGS) $<

watcom\advanced_1_5_0_minimal.res :  .AUTODEPEND ..\src\..\samples\advanced\v1.5.0\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\..\include -i=..\samples\advanced\v1.5.0 $<

