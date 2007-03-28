# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.2 (http://www.bakefile.org)
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
_BUILDDIR_SHARED_SUFFIX =
!ifeq WX_SHARED 0
_BUILDDIR_SHARED_SUFFIX = 
!endif
!ifeq WX_SHARED 1
_BUILDDIR_SHARED_SUFFIX = _dll
!endif
__gizmos_lib___depname =
!ifeq WX_SHARED 0
__gizmos_lib___depname = &
	..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib
!endif
__gizmos_dll___depname =
!ifeq WX_SHARED 1
__gizmos_dll___depname = &
	..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.dll
!endif
____gizmos_5 =
!ifeq WX_SHARED 0
____gizmos_5 = lib
!endif
!ifeq WX_SHARED 1
____gizmos_5 = dll
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
____gizmos =
!ifeq WX_DEBUG 0
____gizmos = -ot -ox
!endif
!ifeq WX_DEBUG 1
____gizmos = -od
!endif
____gizmos_14 =
!ifeq WX_DEBUG 0
____gizmos_14 = -d0
!endif
!ifeq WX_DEBUG 1
____gizmos_14 = -d2
!endif
____gizmos_15 =
!ifeq WX_DEBUG 0
____gizmos_15 = 
!endif
!ifeq WX_DEBUG 1
____gizmos_15 = debug all
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
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION).lib
!endif
!endif
!endif
!ifeq WX_DEBUG 0
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)u.lib
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
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)d.lib
!endif
!endif
!endif
!ifeq WX_DEBUG 1
!ifeq WX_MONOLITHIC 1
!ifeq WX_UNICODE 1
__WXLIB_BASE_NAME_p = wxmsw$(WX_VERSION)ud.lib
!endif
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

GIZMOS_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_ledctrl.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_editlbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_statpict.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_multicell.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_dynamicsash.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_splittree.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_xh_statpict.obj
GIZMOS_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include -dWXMAKINGDLL_GIZMOS &
	$(CPPFLAGS) $(CXXFLAGS)
GIZMOS_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_ledctrl.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_editlbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_statpict.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_multicell.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_dynamicsash.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_splittree.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_xh_statpict.obj
GIZMOS_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dynsash.obj
GIZMOS2_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS2_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2_dynsash_switch.obj
GIZMOS3_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS3_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3_test.obj
GIZMOS4_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS4_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4_led.obj
GIZMOS5_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS5_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5_mtest.obj
GIZMOS6_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include &
	$(____gizmos) $(____gizmos_14) -i=..\include $(CPPFLAGS) $(CXXFLAGS)
GIZMOS6_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6_tree.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild $(__gizmos_lib___depname) $(__gizmos_dll___depname) ..\samples\dynsash\gizmos.exe ..\samples\dynsash_switch\gizmos2.exe ..\samples\editlbox\gizmos3.exe ..\samples\led\gizmos4.exe ..\samples\multicell\gizmos5.exe ..\samples\splittree\gizmos6.exe

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib del ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib
	-if exist ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.dll del ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.dll
	-if exist ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib del ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib
	-if exist ..\samples\dynsash\gizmos.exe del ..\samples\dynsash\gizmos.exe
	-if exist ..\samples\dynsash_switch\gizmos2.exe del ..\samples\dynsash_switch\gizmos2.exe
	-if exist ..\samples\editlbox\gizmos3.exe del ..\samples\editlbox\gizmos3.exe
	-if exist ..\samples\led\gizmos4.exe del ..\samples\led\gizmos4.exe
	-if exist ..\samples\multicell\gizmos5.exe del ..\samples\multicell\gizmos5.exe
	-if exist ..\samples\splittree\gizmos6.exe del ..\samples\splittree\gizmos6.exe

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

!ifeq WX_SHARED 0
..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib :  make_dir_gizmos_lib  $(GIZMOS_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib.lbc
	@for %i in ($(GIZMOS_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib.lbc
!endif

make_dir_gizmos_lib :  
	if not exist ..\lib\wat_$(____gizmos_5) mkdir ..\lib\wat_$(____gizmos_5)

!ifeq WX_SHARED 1
..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.dll :  make_dir_gizmos_dll  $(GIZMOS_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH)
	@for %i in ($(GIZMOS_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc file %i
	@for %i in ( $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib +$^@
!endif

make_dir_gizmos_dll :  
	if not exist ..\lib\wat_$(____gizmos_5) mkdir ..\lib\wat_$(____gizmos_5)

..\samples\dynsash\gizmos.exe :  $(GIZMOS_OBJECTS) make_sample_dir_gizmos  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos.lbc

make_sample_dir_gizmos :  
	if not exist ../samples/dynsash mkdir../samples/dynsash

..\samples\dynsash_switch\gizmos2.exe :  $(GIZMOS2_OBJECTS) make_sample_dir_gizmos2  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS2_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2.lbc

make_sample_dir_gizmos2 :  
	if not exist ../samples/dynsash_switch mkdir../samples/dynsash_switch

..\samples\editlbox\gizmos3.exe :  $(GIZMOS3_OBJECTS) make_sample_dir_gizmos3  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS3_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3.lbc

make_sample_dir_gizmos3 :  
	if not exist ../samples/editlbox mkdir../samples/editlbox

..\samples\led\gizmos4.exe :  $(GIZMOS4_OBJECTS) make_sample_dir_gizmos4  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS4_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4.lbc

make_sample_dir_gizmos4 :  
	if not exist ../samples/led mkdir../samples/led

..\samples\multicell\gizmos5.exe :  $(GIZMOS5_OBJECTS) make_sample_dir_gizmos5  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS5_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5.lbc

make_sample_dir_gizmos5 :  
	if not exist ../samples/multicell mkdir../samples/multicell

..\samples\splittree\gizmos6.exe :  $(GIZMOS6_OBJECTS) make_sample_dir_gizmos6  $(__gizmos_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc $(LDFLAGS) libpath $(WX_DIR)$(WXLIBPATH) $(____gizmos_15) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'
	@for %i in ($(GIZMOS6_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc file %i
	@for %i in ( ..\lib\wat_$(____gizmos_5)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_gizmos.lib $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc option resource=
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6.lbc

make_sample_dir_gizmos6 :  
	if not exist ../samples/splittree mkdir../samples/splittree

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_ledctrl.obj :  .AUTODEPEND ..\src\ledctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_editlbox.obj :  .AUTODEPEND ..\src\editlbox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_statpict.obj :  .AUTODEPEND ..\src\statpict.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_multicell.obj :  .AUTODEPEND ..\src\multicell.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_dynamicsash.obj :  .AUTODEPEND ..\src\dynamicsash.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_splittree.obj :  .AUTODEPEND ..\src\splittree.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_lib_xh_statpict.obj :  .AUTODEPEND ..\src\xh_statpict.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_ledctrl.obj :  .AUTODEPEND ..\src\ledctrl.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_editlbox.obj :  .AUTODEPEND ..\src\editlbox.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_statpict.obj :  .AUTODEPEND ..\src\statpict.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_multicell.obj :  .AUTODEPEND ..\src\multicell.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_dynamicsash.obj :  .AUTODEPEND ..\src\dynamicsash.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_splittree.obj :  .AUTODEPEND ..\src\splittree.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dll_xh_statpict.obj :  .AUTODEPEND ..\src\xh_statpict.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos_dynsash.obj :  .AUTODEPEND ..\samples\dynsash\dynsash.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos2_dynsash_switch.obj :  .AUTODEPEND ..\samples\dynsash_switch\dynsash_switch.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS2_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos3_test.obj :  .AUTODEPEND ..\samples\editlbox\test.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS3_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos4_led.obj :  .AUTODEPEND ..\samples\led\led.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS4_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos5_mtest.obj :  .AUTODEPEND ..\samples\multicell\mtest.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS5_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\gizmos6_tree.obj :  .AUTODEPEND ..\samples\splittree\tree.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(GIZMOS6_CXXFLAGS) $<

