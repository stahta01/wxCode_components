# =========================================================================
#     This makefile was generated by
#     Bakefile 0.1.5 (http://bakefile.sourceforge.net)
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

# Should UNICODE be enabled ? [0,1]
UNICODE = 0

# Type of compiled binaries [debug,release]
BUILD = debug

# The wxWidgets library main folder 
WXWIN = c:\wxWidgets



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

WXSUBLIBPOSTFIX =
!ifeq BUILD debug
WXSUBLIBPOSTFIX = d
!endif
__DEBUGINFO_7 =
!ifeq BUILD debug
__DEBUGINFO_7 = debug all
!endif
!ifeq BUILD release
__DEBUGINFO_7 = 
!endif
__DEBUG_DEFINE_p =
!ifeq BUILD debug
__DEBUG_DEFINE_p = -d__WXDEBUG__
!endif
__WARNINGS =
!ifeq BUILD debug
__WARNINGS = -wx
!endif
!ifeq BUILD release
__WARNINGS = 
!endif
__OPTIMIZEFLAG =
!ifeq BUILD debug
__OPTIMIZEFLAG = -od
!endif
!ifeq BUILD release
__OPTIMIZEFLAG = -ot -ox
!endif
__DEBUGINFO =
!ifeq BUILD debug
__DEBUGINFO = -d2
!endif
!ifeq BUILD release
__DEBUGINFO = -d0
!endif
__UNICODE_DEFINE_p =
!ifeq UNICODE 1
__UNICODE_DEFINE_p = -d_UNICODE
!endif
WXLIBPOSTFIX =
!ifeq BUILD debug
!ifeq UNICODE 0
WXLIBPOSTFIX = d
!endif
!endif
!ifeq BUILD debug
!ifeq UNICODE 1
WXLIBPOSTFIX = ud
!endif
!endif
!ifeq BUILD release
!ifeq UNICODE 1
WXLIBPOSTFIX = u
!endif
!endif

### Variables: ###

KEYBINDER_CXXFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=..\include $(CPPFLAGS) &
	$(CXXFLAGS)
KEYBINDER_OBJECTS =  &
	watcom\keybinder_keybinder.obj &
	watcom\keybinder_menuutils.obj
MINIMAL_CXXFLAGS = $(__WARNINGS) $(__UNICODE_DEFINE_p) $(__OPTIMIZEFLAG) &
	$(__DEBUGINFO) -i=..\include -i=$(WXWIN)\include &
	-i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) $(__DEBUG_DEFINE_p) -d__WXMSW__ &
	$(CPPFLAGS) $(CXXFLAGS)
MINIMAL_OBJECTS =  &
	watcom\minimal_minimal.obj


all : watcom
watcom :
	-if not exist watcom mkdir watcom

### Targets: ###

all : .SYMBOLIC ..\lib\keybinder$(WXLIBPOSTFIX).lib ..\sample\minimal.exe

clean : .SYMBOLIC 
	-if exist watcom\*.obj del watcom\*.obj
	-if exist watcom\*.res del watcom\*.res
	-if exist watcom\*.lbc del watcom\*.lbc
	-if exist watcom\*.ilk del watcom\*.ilk
	-if exist watcom\*.pch del watcom\*.pch
	-if exist ..\lib\keybinder$(WXLIBPOSTFIX).lib del ..\lib\keybinder$(WXLIBPOSTFIX).lib
	-if exist ..\sample\minimal.exe del ..\sample\minimal.exe

tarball :  
	( cd .. && tar -cvzf ..\keybinder.tar.gz --exclude=*.pdb --exclude=*.log --exclude=*.o* * )

zip :  
	( cd .. && zip -r9 ..\keybinder.zip *  -x *.pdb -x *.log -x *.o* )

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
	( cd ..\docs && doxygen )

cleandocs :  
	-if exist ..\docs\html rmdir /S /Q ..\docs\html

..\lib\keybinder$(WXLIBPOSTFIX).lib :  $(KEYBINDER_OBJECTS)
	@%create watcom\keybinder.lbc
	@for %i in ($(KEYBINDER_OBJECTS)) do @%append watcom\keybinder.lbc +%i
	wlib -q -p4096 -n -b $^@ @watcom\keybinder.lbc

..\sample\minimal.exe :  $(MINIMAL_OBJECTS) watcom\minimal_minimal.res ..\lib\keybinder$(WXLIBPOSTFIX).lib
	@%create watcom\minimal.lbc
	@%append watcom\minimal.lbc option quiet
	@%append watcom\minimal.lbc name $^@
	@%append watcom\minimal.lbc option caseexact
	@%append watcom\minimal.lbc $(LDFLAGS) $(__DEBUGINFO_7) libpath ..\lib libpath $(WXWIN)\lib\wat_lib system nt_win ref '_WinMain@16'
	@for %i in ($(MINIMAL_OBJECTS)) do @%append watcom\minimal.lbc file %i
	@for %i in ( ..\lib\keybinder$(WXLIBPOSTFIX).lib wxmsw25$(WXLIBPOSTFIX)_core.lib wxmsw25$(WXLIBPOSTFIX)_html.lib wxbase25$(WXLIBPOSTFIX).lib wxtiff$(WXSUBLIBPOSTFIX).lib wxjpeg$(WXSUBLIBPOSTFIX).lib wxpng$(WXSUBLIBPOSTFIX).lib wxzlib$(WXSUBLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WXSUBLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watcom\minimal.lbc library %i
	@%append watcom\minimal.lbc option resource=watcom\minimal_minimal.res
	wlink @watcom\minimal.lbc

watcom\keybinder_keybinder.obj :  .AUTODEPEND .\..\src\keybinder.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_CXXFLAGS) $<

watcom\keybinder_menuutils.obj :  .AUTODEPEND .\..\src\menuutils.cpp
	$(CXX) -zq -fo=$^@ $(KEYBINDER_CXXFLAGS) $<

watcom\minimal_minimal.obj :  .AUTODEPEND .\..\sample\minimal.cpp
	$(CXX) -zq -fo=$^@ $(MINIMAL_CXXFLAGS) $<

watcom\minimal_minimal.res :  .AUTODEPEND .\..\sample\minimal.rc
	wrc -q -ad -bt=nt -r -fo=$^@ $(__UNICODE_DEFINE_p) -i=..\include -i=$(WXWIN)\include -i=$(WXWIN)\lib\wat_lib\msw$(WXLIBPOSTFIX) -i=..\sample $<

