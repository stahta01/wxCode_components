# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.9 (http://www.bakefile.org)
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

# Use DLL build of wx library? [0,1]
#   0 - Static
#   1 - DLL
WX_SHARED = 0

# Use Unicode build of wxWidgets? [0,1]
#   0 - ANSI
#   1 - Unicode
WX_UNICODE = 0

# Use debug build of wxWidgets (define __WXDEBUG__)? [0,1]
#   0 - Release
#   1 - Debug
WX_DEBUG = 1

# Version of the wx library to build against. 
WX_VERSION = 28

# Use monolithic build of wxWidgets? [0,1]
#   0 - Multilib
#   1 - Monolithic
WX_MONOLITHIC = 0

# The directory where wxWidgets library is installed 
WX_DIR = $(%WXWIN)

# Build this wxCode component as DLL or as static library? [0,1]
#   0 - Static
#   1 - DLL
SHARED = 0



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
! if $(__VERSION__) >= 1280
!  loaddll wlink    wlinkd
! else
!  loaddll wlink    wlink
! endif
!  loaddll wlib     wlibd
!endif

# We need these variables in some bakefile-made rules:
WATCOM_CWD = $+ $(%cdrive):$(%cwd) $-

### Conditionally set variables: ###

WX3RDPARTYLIBPOSTFIX =
!ifeq WX_DEBUG 1
WX3RDPARTYLIBPOSTFIX = d
!endif
_BUILDDIR_SHARED_SUFFIX =
!ifeq SHARED 0
_BUILDDIR_SHARED_SUFFIX = 
!endif
!ifeq SHARED 1
_BUILDDIR_SHARED_SUFFIX = _dll
!endif
__stedit_lib___depname =
!ifeq WX_SHARED 0
__stedit_lib___depname = &
	..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib
!endif
__stedit_dll___depname =
!ifeq WX_SHARED 1
__stedit_dll___depname = &
	..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.dll
!endif
____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq SHARED 0
____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq SHARED 1
____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
!endif
VAR =
!ifeq WX_DEBUG 0
VAR = -ot -ox
!endif
!ifeq WX_DEBUG 1
VAR = -od
!endif
VAR_0 =
!ifeq WX_DEBUG 0
VAR_0 = -d0
!endif
!ifeq WX_DEBUG 1
VAR_0 = -d2
!endif
VAR_1 =
!ifeq WX_DEBUG 0
VAR_1 = 
!endif
!ifeq WX_DEBUG 1
VAR_1 = debug all
!endif
____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq WX_SHARED 0
____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq WX_SHARED 1
____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
!endif
__WXLIB_HTML_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_HTML_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib
!endif
__WXLIB_ADV_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_ADV_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_adv.lib
!endif
__WXLIB_CORE_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_CORE_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib
!endif
__WXLIB_BASE_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_BASE_NAME_p = wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib
!endif
!ifeq WX_MONOLITHIC 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX).lib
!endif
____WX_SHARED =
!ifeq WX_SHARED 0
____WX_SHARED = 
!endif
!ifeq WX_SHARED 1
____WX_SHARED = -dWXUSINGDLL
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

STEDIT_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include -i=$(WXWIN)\contrib\include &
	/fh=watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\prec_stedit_lib.pch &
	$(CPPFLAGS) $(CXXFLAGS)
STEDIT_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_precomp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steart.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedit.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedlgs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedlgs_wdr.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steexprt.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stefindr.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steframe.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stelangs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stemenum.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stenoteb.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steopts.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steprefs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steprint.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steshell.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stesplit.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stestyls.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_wxext.obj
STEDIT_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include -dWXMAKINGDLL_STEDIT -i=$(WXWIN)\contrib\include &
	/fh=watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\prec_stedit_dll.pch &
	$(CPPFLAGS) $(CXXFLAGS)
STEDIT_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_precomp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steart.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedit.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedlgs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedlgs_wdr.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steexprt.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stefindr.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steframe.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stelangs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stemenum.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stenoteb.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steopts.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steprefs.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steprint.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steshell.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stesplit.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stestyls.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_wxext.obj
WXSTEDIT_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_0) -wx -i=..\include -i=$(WXWIN)\contrib\include $(CPPFLAGS) &
	$(CXXFLAGS)
WXSTEDIT_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_link.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_stedoc.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_wxstedit.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild setup_h $(__stedit_lib___depname) $(__stedit_dll___depname) ..\samples\stedit\wxstedit.exe

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib del ..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib
	-if exist ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.dll del ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.dll
	-if exist ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib del ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib
	-if exist ..\samples\stedit\wxstedit.exe del ..\samples\stedit\wxstedit.exe

test_for_selected_wxbuild :  
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo ----------------------------------------------------------------------------
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo The selected wxWidgets build is not available!
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo Please use the options prefixed with WX_ to select another wxWidgets build.
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	echo ----------------------------------------------------------------------------
	@if not exist $(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX)\wx\setup.h \
	exit 1

setup_h : .SYMBOLIC include\wx\stedit\setup.h

include\wx\stedit\setup.h :  
	if not exist include\wx\stedit\setup.h copy include\wx\stedit\setup0.h include\wx\stedit\setup.h

!ifeq WX_SHARED 0
..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib :  make_dir_stedit_lib  $(STEDIT_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib.lbc
	@for %i in ($(STEDIT_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib.lbc
!endif

make_dir_stedit_lib :  
	if not exist ..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)

!ifeq WX_SHARED 1
..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.dll :  make_dir_stedit_dll  $(STEDIT_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_1) libpath ..$(WXLIBPATH) $(LDFLAGS)
	@for %i in ($(STEDIT_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc file %i
	@for %i in ( wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_stc.lib $(__WXLIB_HTML_NAME_p) $(__WXLIB_ADV_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib +$^@
!endif

make_dir_stedit_dll :  
	if not exist ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____stedit_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)

..\samples\stedit\wxstedit.exe :  $(WXSTEDIT_OBJECTS) make_sample_dir_wxstedit watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_wxstedit.res $(__stedit_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_1) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(WXSTEDIT_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc file %i
	@for %i in ( ..\lib\wat_$(____stedit_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_stedit.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_stc.lib $(__WXLIB_HTML_NAME_p) $(__WXLIB_ADV_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc option resource=watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_wxstedit.res
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit.lbc

make_sample_dir_wxstedit :  
	if not exist ..\samples\stedit mkdir ..\samples\stedit

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_precomp.obj :  .AUTODEPEND ..\src\precomp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steart.obj :  .AUTODEPEND ..\src\steart.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedit.obj :  .AUTODEPEND ..\src\stedit.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedlgs.obj :  .AUTODEPEND ..\src\stedlgs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stedlgs_wdr.obj :  .AUTODEPEND ..\src\stedlgs_wdr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steexprt.obj :  .AUTODEPEND ..\src\steexprt.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stefindr.obj :  .AUTODEPEND ..\src\stefindr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steframe.obj :  .AUTODEPEND ..\src\steframe.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stelangs.obj :  .AUTODEPEND ..\src\stelangs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stemenum.obj :  .AUTODEPEND ..\src\stemenum.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stenoteb.obj :  .AUTODEPEND ..\src\stenoteb.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steopts.obj :  .AUTODEPEND ..\src\steopts.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steprefs.obj :  .AUTODEPEND ..\src\steprefs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steprint.obj :  .AUTODEPEND ..\src\steprint.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_steshell.obj :  .AUTODEPEND ..\src\steshell.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stesplit.obj :  .AUTODEPEND ..\src\stesplit.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_stestyls.obj :  .AUTODEPEND ..\src\stestyls.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_lib_wxext.obj :  .AUTODEPEND ..\src\wxext.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_precomp.obj :  .AUTODEPEND ..\src\precomp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steart.obj :  .AUTODEPEND ..\src\steart.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedit.obj :  .AUTODEPEND ..\src\stedit.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedlgs.obj :  .AUTODEPEND ..\src\stedlgs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stedlgs_wdr.obj :  .AUTODEPEND ..\src\stedlgs_wdr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steexprt.obj :  .AUTODEPEND ..\src\steexprt.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stefindr.obj :  .AUTODEPEND ..\src\stefindr.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steframe.obj :  .AUTODEPEND ..\src\steframe.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stelangs.obj :  .AUTODEPEND ..\src\stelangs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stemenum.obj :  .AUTODEPEND ..\src\stemenum.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stenoteb.obj :  .AUTODEPEND ..\src\stenoteb.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steopts.obj :  .AUTODEPEND ..\src\steopts.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steprefs.obj :  .AUTODEPEND ..\src\steprefs.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steprint.obj :  .AUTODEPEND ..\src\steprint.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_steshell.obj :  .AUTODEPEND ..\src\steshell.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stesplit.obj :  .AUTODEPEND ..\src\stesplit.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_stestyls.obj :  .AUTODEPEND ..\src\stestyls.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\stedit_dll_wxext.obj :  .AUTODEPEND ..\src\wxext.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(STEDIT_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_link.obj :  .AUTODEPEND ..\samples\stedit\link.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSTEDIT_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_stedoc.obj :  .AUTODEPEND ..\samples\stedit\stedoc.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSTEDIT_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_wxstedit.obj :  .AUTODEPEND ..\samples\stedit\wxstedit.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSTEDIT_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxstedit_wxstedit.res :  .AUTODEPEND ..\samples\stedit\wxstedit.rc
	wrc -q -ad -bt=nt -r -fo=$^@  $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ -i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include -i=..\include -i=..\samples\stedit -i=$(WXWIN)\contrib\include $<

