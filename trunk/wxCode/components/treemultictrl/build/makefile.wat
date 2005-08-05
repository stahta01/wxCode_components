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

TMCLIB_CXXFLAGS = $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\contrib\include\wx\treemultictrl -d2 -br &
	-dCHECKBOXVIEW=0 $(CPPFLAGS) $(CXXFLAGS)
TMCLIB_OBJECTS =  &
	lib\tmclib_TreeMultiItemBase.obj &
	lib\tmclib_TreeMultiItemNode.obj &
	lib\tmclib_TreeMultiItemWindow.obj &
	lib\tmclib_TreeMultiItemRoot.obj &
	lib\tmclib_wxTreeMultiCtrl.obj
MULTICTRLTEST_CXXFLAGS = $(__WXUNICODE_DEFINE_p) $(__WXDEBUG_DEFINE_p) &
	-d__WXMSW__ -i=$(WX_DIR)$(__WXLIBPATH_FILENAMES)\msw$(WXLIBPOSTFIX) &
	-i=$(WX_DIR)\include -i=..\contrib\include\wx\treemultictrl -d2 &
	-dCHECKBOXVIEW=0 -br $(CPPFLAGS) $(CXXFLAGS)
MULTICTRLTEST_OBJECTS =  &
	samples\multictrltest_MultiCtrlTest.obj


all : samples
samples :
	-if not exist samples mkdir samples

### Targets: ###

all : .SYMBOLIC lib\tmclib.lib samples\multictrltest.exe

clean : .SYMBOLIC 
	-if exist samples\*.obj del samples\*.obj
	-if exist samples\*.res del samples\*.res
	-if exist samples\*.lbc del samples\*.lbc
	-if exist samples\*.ilk del samples\*.ilk
	-if exist samples\*.pch del samples\*.pch
	-if exist lib\tmclib.lib del lib\tmclib.lib
	-if exist samples\multictrltest.exe del samples\multictrltest.exe

lib\tmclib.lib :  $(TMCLIB_OBJECTS)
	@%create lib\tmclib.lbc
	@for %i in ($(TMCLIB_OBJECTS)) do @%append lib\tmclib.lbc +%i
	wlib -q -p4096 -n -b $^@ @lib\tmclib.lbc

samples\multictrltest.exe :  $(MULTICTRLTEST_OBJECTS) lib\tmclib.lib lib\tmclib.lib
	@%create samples\multictrltest.lbc
	@%append samples\multictrltest.lbc option quiet
	@%append samples\multictrltest.lbc name $^@
	@%append samples\multictrltest.lbc option caseexact
	@%append samples\multictrltest.lbc $(LDFLAGS) libpath $(WX_DIR)$(__WXLIBPATH_FILENAMES) debug all system nt_win ref '_WinMain@16'
	@for %i in ($(MULTICTRLTEST_OBJECTS)) do @%append samples\multictrltest.lbc file %i
	@for %i in ( lib\tmclib.lib wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_core.lib wxbase$(WX_VERSION)$(WXLIBPOSTFIX).lib wxtiff$(WX3RDPARTLIBPOSTFIX).lib wxjpeg$(WX3RDPARTLIBPOSTFIX).lib wxpng$(WX3RDPARTLIBPOSTFIX).lib wxzlib$(WX3RDPARTLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append samples\multictrltest.lbc library %i
	@%append samples\multictrltest.lbc
	wlink @samples\multictrltest.lbc

lib\tmclib_TreeMultiItemBase.obj :  .AUTODEPEND .\..\contrib\src\treemultictrl\TreeMultiItemBase.cpp
	$(CXX) -zq -fo=$^@ $(TMCLIB_CXXFLAGS) $<

lib\tmclib_TreeMultiItemNode.obj :  .AUTODEPEND .\..\contrib\src\treemultictrl\TreeMultiItemNode.cpp
	$(CXX) -zq -fo=$^@ $(TMCLIB_CXXFLAGS) $<

lib\tmclib_TreeMultiItemWindow.obj :  .AUTODEPEND .\..\contrib\src\treemultictrl\TreeMultiItemWindow.cpp
	$(CXX) -zq -fo=$^@ $(TMCLIB_CXXFLAGS) $<

lib\tmclib_TreeMultiItemRoot.obj :  .AUTODEPEND .\..\contrib\src\treemultictrl\TreeMultiItemRoot.cpp
	$(CXX) -zq -fo=$^@ $(TMCLIB_CXXFLAGS) $<

lib\tmclib_wxTreeMultiCtrl.obj :  .AUTODEPEND .\..\contrib\src\treemultictrl\wxTreeMultiCtrl.cpp
	$(CXX) -zq -fo=$^@ $(TMCLIB_CXXFLAGS) $<

samples\multictrltest_MultiCtrlTest.obj :  .AUTODEPEND .\..\contrib\samples\treemultictrl\MultiCtrlTest.cpp
	$(CXX) -zq -fo=$^@ $(MULTICTRLTEST_CXXFLAGS) $<

