# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.8 (http://bakefile.sourceforge.net)
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

WX3RDPARTLIBPOSTFIX =
!ifeq WX_DEBUG 1
WX3RDPARTLIBPOSTFIX = d
!endif
__keybinder_lib___depname =
!ifeq WX_SHARED 0
__keybinder_lib___depname = ..\lib\keybinder$(WXLIBPOSTFIX).lib
!endif
__keybinder_dll___depname =
!ifeq WX_SHARED 1
__keybinder_dll___depname = ..\lib\keybinder$(WXLIBPOSTFIX).dll
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

KEYBINDER_LIB_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
KEYBINDER_LIB_OBJECTS =  &
	watcom\keybinder_lib_keybinder.obj &
	watcom\keybinder_lib_menuutils.obj
KEYBINDER_DLL_CXXFLAGS = -bd $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include -dWXMAKINGDLL_KEYBINDER $(CPPFLAGS) &
	$(CXXFLAGS)
KEYBINDER_DLL_OBJECTS =  &
	watcom\keybinder_dll_keybinder.obj &
	watcom\keybinder_dll_menuutils.obj
MINIMAL_CXXFLAGS = $(__WARNINGS) $(__OPTIMIZEFLAG) $(__DEBUGINFO) -bm &
	$(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\include $(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watcom\minimal_minimal.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC $(__keybinder_lib___depname) $(__keybinder_dll___depname) ..\sample\minimal.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\lib\keybinder$(WXLIBPOSTFIX).lib del ..\lib\keybinder$(WXLIBPOSTFIX).lib
	-if exist ..\lib\keybinder$(WXLIBPOSTFIX).dll del ..\lib\keybinder$(WXLIBPOSTFIX).dll
	-if exist ..\lib\keybinder$(WXLIBPOSTFIX).lib del ..\lib\keybinder$(WXLIBPOSTFIX).lib
	-if exist ..\sample\minimal.exe del ..\sample\minimal.exe

tarball :  
	-cd ..
	-tar -cvzf ..\keybinder.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* *

zip :  
	-cd ..
	-zip -r9 ..\keybinder.zip *  -x *.pdb -x *.log -x *.o*

cleanbuilddirs :  
	-if exist msvc rmdir /S /Q msvc
	-if exist watcom rmdir /S /Q watcom
	-if exist borland rmdir /S /Q borland
	-if exist mingw rmdir /S /Q mingw
	-if exist msvc6prj rmdir /S /Q msvc6prj

cleanall :  clean cleanbuilddirs
	del /S /Q ..\*.log >NUL
	del /S /Q ..\*.lib >NUL
	del /S /Q ..\*.a >NUL
	del /S /Q ..\*.exe >NUL

docs :  
	-cd ..\docs
	-doxygen

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

!ifeq WX_SHARED 0
..\lib\keybinder$(WXLIBPOSTFIX).lib :  $(KEYBINDER_LIB_OBJECTS)
	@%create watcom\keybinder_lib.lbc
	@for %i in ($(KEYBINDER_LIB_OBJECTS)) do @%append watcom\keybinder_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\keybinder_lib.lbc
!endif

!ifeq WX_SHARED 1
..\lib\keybinder$(WXLIBPOSTFIX).dll :  $(KEYBINDER_DLL_OBJECTS)
	@%create watcom\keybinder_dll.lbc
	@%append watcom\keybinder_dll.lbc option quiet
	@%append watcom\keybinder_dll.lbc name $^@
	@%append watcom\keybinder_dll.lbc option caseexact
	@%append watcom\keybinder_dll.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib
	@for %i in ($(KEYBINDER_DLL_OBJECTS)) do @%append watcom\keybinder_dll.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTLIBPOSTFIX).lib wxjpeg$(WX3RDPARTLIBPOSTFIX).lib wxpng$(WX3RDPARTLIBPOSTFIX).lib wxzlib$(WX3RDPARTLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\keybinder_dll.lbc library %i
	@%append watcom\keybinder_dll.lbc
	@%append watcom\keybinder_dll.lbc system nt_dll
	wlink @watcom\keybinder_dll.lbc
	wlib -q -n -b ..\lib\keybinder$(WXLIBPOSTFIX).lib +$^@
!endif

..\sample\minimal.exe :  $(MINIMAL_OBJECTS) watcom\minimal_minimal.res $(__keybinder_lib___depname)
	@%create watcom\minimal.lbc
	@%append watcom\minimal.lbc option quiet
	@%append watcom\minimal.lbc name $^@
	@%append watcom\minimal.lbc option caseexact
	@%append watcom\minimal.lbc $(LDFLAGS) $(__DEBUGINFO_1)  libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) libpath ..\lib system nt_win ref '_WinMain@16'
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watcom\minimal.lbc file %i
	@for %i in ( ..\lib\keybinder$(WXLIBPOSTFIX).lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTLIBPOSTFIX).lib wxjpeg$(WX3RDPARTLIBPOSTFIX).lib wxpng$(WX3RDPARTLIBPOSTFIX).lib wxzlib$(WX3RDPARTLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal.lbc library %i
	@%append watcom\minimal.lbc option resource=watcom\minimal_minimal.res
	wlink @watcom\minimal.lbc

watcom\keybinder_lib_keybinder.obj :  .AUTODEPEND .\..\src\keybinder.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_LIB_CXXFLAGS) $<

watcom\keybinder_lib_menuutils.obj :  .AUTODEPEND .\..\src\menuutils.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_LIB_CXXFLAGS) $<

watcom\keybinder_dll_keybinder.obj :  .AUTODEPEND .\..\src\keybinder.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_DLL_CXXFLAGS) $<

watcom\keybinder_dll_menuutils.obj :  .AUTODEPEND .\..\src\menuutils.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_DLL_CXXFLAGS) $<

watcom\minimal_minimal.obj :  .AUTODEPEND .\..\sample\minimal.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal_minimal.res :  .AUTODEPEND .\..\sample\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__WX_SHAREDDEFINE_p) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\sample $<

