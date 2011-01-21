# =========================================================================
#     This makefile was generated by
#     Bakefile 0.2.8 (http://www.bakefile.org)
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
__wxsmtp_lib___depname =
!ifeq SHARED 0
__wxsmtp_lib___depname = &
	..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib
!endif
__wxsmtp_dll___depname =
!ifeq SHARED 1
__wxsmtp_dll___depname = &
	..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.dll
!endif
__WXLIB_HTML_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_HTML_NAME_p = wxmsw$(WX_VERSION)$(WXLIBPOSTFIX)_html.lib
!endif
____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq SHARED 0
____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq SHARED 1
____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
!endif
____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES =
!ifeq WX_SHARED 0
____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = lib
!endif
!ifeq WX_SHARED 1
____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES = dll
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
VAR =
!ifeq WX_DEBUG 0
VAR = -ot -ox
!endif
!ifeq WX_DEBUG 1
VAR = -od
!endif
VAR_2 =
!ifeq WX_DEBUG 0
VAR_2 = -d0
!endif
!ifeq WX_DEBUG 1
VAR_2 = -d2
!endif
VAR_3 =
!ifeq WX_DEBUG 0
VAR_3 = 
!endif
!ifeq WX_DEBUG 1
VAR_3 = debug all
!endif
__WXLIB_NET_NAME_p =
!ifeq WX_MONOLITHIC 0
__WXLIB_NET_NAME_p = wxbase$(WX_VERSION)$(WXLIBPOSTFIX)_net.lib
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
WXLIBPATH =
!ifeq WX_SHARED 0
WXLIBPATH = \lib\wat_lib
!endif
!ifeq WX_SHARED 1
WXLIBPATH = \lib\wat_dll
!endif

### Variables: ###

WXSMTP_LIB_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
WXSMTP_LIB_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxcmdprot.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxmd5.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_charsetconv.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rfc2047.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rfc2231.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtpemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtpstates.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxpop3.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxpop3states.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_body.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentdescription.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentdisposition.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contenttransferencoding.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contenttype.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_fieldparam.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_header.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_message.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mimeentity.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mimeversion.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_strutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_utils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_version.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_base64.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_qp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_address.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_addresslist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_datetime.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_field.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_fieldvalue.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_group.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mailbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mailboxlist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_messageid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rheader.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rmessage.obj
WXSMTP_DLL_CXXFLAGS = -bd $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include -dWXMAKINGDLL_WXSMTP $(CPPFLAGS) $(CXXFLAGS)
WXSMTP_DLL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxcmdprot.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxmd5.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_charsetconv.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rfc2047.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rfc2231.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtpemail.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtpstates.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxpop3.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxpop3states.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_body.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentdescription.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentdisposition.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contenttransferencoding.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contenttype.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_fieldparam.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_header.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_message.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mimeentity.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mimeversion.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_strutils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_utils.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_version.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_base64.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_qp.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_address.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_addresslist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_datetime.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_field.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_fieldvalue.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_group.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mailbox.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mailboxlist.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_messageid.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rheader.obj &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rmessage.obj
SENDMAIL_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
SENDMAIL_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail_SendMail.obj
POP3CLIENT_CXXFLAGS = $(____WX_SHARED) $(__WXUNICODE_DEFINE_p) &
	$(__WXDEBUG_DEFINE_p) -d__WXMSW__ &
	-i=$(WX_DIR)$(WXLIBPATH)\msw$(WXLIBPOSTFIX) -i=$(WX_DIR)\include $(VAR) &
	$(VAR_2) -wx -i=..\include $(CPPFLAGS) $(CXXFLAGS)
POP3CLIENT_OBJECTS =  &
	watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client_Pop3Client.obj


all : watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)
watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) :
	-if not exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX) mkdir watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)

### Targets: ###

all : .SYMBOLIC test_for_selected_wxbuild $(__wxsmtp_lib___depname) $(__wxsmtp_dll___depname) ..\samples\SendMail\SendMail.exe ..\samples\Pop3Client\Pop3Client.exe

clean : .SYMBOLIC 
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.obj
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.res
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.lbc
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.ilk
	-if exist watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch del watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\*.pch
	-if exist ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib del ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib
	-if exist ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.dll del ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.dll
	-if exist ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib del ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib
	-if exist ..\samples\SendMail\SendMail.exe del ..\samples\SendMail\SendMail.exe
	-if exist ..\samples\Pop3Client\Pop3Client.exe del ..\samples\Pop3Client\Pop3Client.exe

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

!ifeq SHARED 0
..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib :  make_dir_wxsmtp_lib  $(WXSMTP_LIB_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib.lbc
	@for %i in ($(WXSMTP_LIB_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib.lbc +%i
	wlib -q -p4096 -n -b $^@ @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib.lbc
!endif

make_dir_wxsmtp_lib :  
	if not exist ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)

!ifeq SHARED 1
..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.dll :  make_dir_wxsmtp_dll  $(WXSMTP_DLL_OBJECTS)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) $(LDFLAGS)
	@for %i in ($(WXSMTP_DLL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc file %i
	@for %i in ( $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc system nt_dll
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll.lbc
	wlib -q -n -b ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib +$^@
!endif

make_dir_wxsmtp_dll :  
	if not exist ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES) mkdir ..\lib\wat_$(____wxsmtp_dll__DIRNAME_SHARED_SUFFIX_FILENAMES)

..\samples\SendMail\SendMail.exe :  $(SENDMAIL_OBJECTS) make_sample_dir_SendMail $(__wxsmtp_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(SENDMAIL_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail.lbc

make_sample_dir_SendMail :  
	if not exist ..\samples\SendMail mkdir ..\samples\SendMail

..\samples\Pop3Client\Pop3Client.exe :  $(POP3CLIENT_OBJECTS) make_sample_dir_Pop3Client $(__wxsmtp_lib___depname)
	@%create watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc option quiet
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc name $^@
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc option caseexact
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc  libpath $(WX_DIR)$(WXLIBPATH) $(VAR_3) libpath ..$(WXLIBPATH) system nt_win ref '_WinMain@16'  $(LDFLAGS)
	@for %i in ($(POP3CLIENT_OBJECTS)) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc file %i
	@for %i in ( ..\lib\wat_$(____wxsmtp_lib__DIRNAME_SHARED_SUFFIX_FILENAMES)\wxcode_msw$(WX_VERSION)$(WXLIBPOSTFIX)_wxsmtp.lib $(__WXLIB_HTML_NAME_p) $(__WXLIB_NET_NAME_p) $(__WXLIB_CORE_NAME_p) $(__WXLIB_BASE_NAME_p) wxtiff$(WX3RDPARTYLIBPOSTFIX).lib wxjpeg$(WX3RDPARTYLIBPOSTFIX).lib wxpng$(WX3RDPARTYLIBPOSTFIX).lib wxzlib$(WX3RDPARTYLIBPOSTFIX).lib wxregex$(WXLIBPOSTFIX).lib wxexpat$(WX3RDPARTYLIBPOSTFIX).lib kernel32.lib user32.lib gdi32.lib comdlg32.lib winspool.lib winmm.lib shell32.lib comctl32.lib ole32.lib oleaut32.lib uuid.lib rpcrt4.lib advapi32.lib wsock32.lib odbc32.lib) do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc library %i
	@%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc
	@for %i in () do @%append watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc option stack=%i
	wlink @watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client.lbc

make_sample_dir_Pop3Client :  
	if not exist ..\samples\Pop3Client mkdir ..\samples\Pop3Client

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxemail.obj :  .AUTODEPEND ..\src\wxemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxcmdprot.obj :  .AUTODEPEND ..\src\utils\wxcmdprot.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxmd5.obj :  .AUTODEPEND ..\src\utils\wxmd5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_charsetconv.obj :  .AUTODEPEND ..\src\codec\charsetconv.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rfc2047.obj :  .AUTODEPEND ..\src\codec\rfc2047.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rfc2231.obj :  .AUTODEPEND ..\src\codec\rfc2231.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtp.obj :  .AUTODEPEND ..\src\smtp\wxsmtp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtpemail.obj :  .AUTODEPEND ..\src\smtp\wxsmtpemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxsmtpstates.obj :  .AUTODEPEND ..\src\smtp\wxsmtpstates.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxpop3.obj :  .AUTODEPEND ..\src\pop3\wxpop3.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_wxpop3states.obj :  .AUTODEPEND ..\src\pop3\wxpop3states.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_body.obj :  .AUTODEPEND ..\src\mimetic\body.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentdescription.obj :  .AUTODEPEND ..\src\mimetic\contentdescription.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentdisposition.obj :  .AUTODEPEND ..\src\mimetic\contentdisposition.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contentid.obj :  .AUTODEPEND ..\src\mimetic\contentid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contenttransferencoding.obj :  .AUTODEPEND ..\src\mimetic\contenttransferencoding.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_contenttype.obj :  .AUTODEPEND ..\src\mimetic\contenttype.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_fieldparam.obj :  .AUTODEPEND ..\src\mimetic\fieldparam.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_header.obj :  .AUTODEPEND ..\src\mimetic\header.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_message.obj :  .AUTODEPEND ..\src\mimetic\message.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mimeentity.obj :  .AUTODEPEND ..\src\mimetic\mimeentity.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mimeversion.obj :  .AUTODEPEND ..\src\mimetic\mimeversion.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_strutils.obj :  .AUTODEPEND ..\src\mimetic\strutils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_utils.obj :  .AUTODEPEND ..\src\mimetic\utils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_version.obj :  .AUTODEPEND ..\src\mimetic\version.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_base64.obj :  .AUTODEPEND ..\src\mimetic\codec\base64.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_qp.obj :  .AUTODEPEND ..\src\mimetic\codec\qp.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_address.obj :  .AUTODEPEND ..\src\mimetic\rfc822\address.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_addresslist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\addresslist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_datetime.obj :  .AUTODEPEND ..\src\mimetic\rfc822\datetime.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_field.obj :  .AUTODEPEND ..\src\mimetic\rfc822\field.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_fieldvalue.obj :  .AUTODEPEND ..\src\mimetic\rfc822\fieldvalue.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_group.obj :  .AUTODEPEND ..\src\mimetic\rfc822\group.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mailbox.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailbox.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_mailboxlist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailboxlist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_messageid.obj :  .AUTODEPEND ..\src\mimetic\rfc822\messageid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rheader.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rheader.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_lib_rmessage.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rmessage.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_LIB_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxemail.obj :  .AUTODEPEND ..\src\wxemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxcmdprot.obj :  .AUTODEPEND ..\src\utils\wxcmdprot.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxmd5.obj :  .AUTODEPEND ..\src\utils\wxmd5.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_charsetconv.obj :  .AUTODEPEND ..\src\codec\charsetconv.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rfc2047.obj :  .AUTODEPEND ..\src\codec\rfc2047.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rfc2231.obj :  .AUTODEPEND ..\src\codec\rfc2231.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtp.obj :  .AUTODEPEND ..\src\smtp\wxsmtp.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtpemail.obj :  .AUTODEPEND ..\src\smtp\wxsmtpemail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxsmtpstates.obj :  .AUTODEPEND ..\src\smtp\wxsmtpstates.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxpop3.obj :  .AUTODEPEND ..\src\pop3\wxpop3.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_wxpop3states.obj :  .AUTODEPEND ..\src\pop3\wxpop3states.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_body.obj :  .AUTODEPEND ..\src\mimetic\body.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentdescription.obj :  .AUTODEPEND ..\src\mimetic\contentdescription.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentdisposition.obj :  .AUTODEPEND ..\src\mimetic\contentdisposition.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contentid.obj :  .AUTODEPEND ..\src\mimetic\contentid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contenttransferencoding.obj :  .AUTODEPEND ..\src\mimetic\contenttransferencoding.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_contenttype.obj :  .AUTODEPEND ..\src\mimetic\contenttype.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_fieldparam.obj :  .AUTODEPEND ..\src\mimetic\fieldparam.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_header.obj :  .AUTODEPEND ..\src\mimetic\header.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_message.obj :  .AUTODEPEND ..\src\mimetic\message.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mimeentity.obj :  .AUTODEPEND ..\src\mimetic\mimeentity.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mimeversion.obj :  .AUTODEPEND ..\src\mimetic\mimeversion.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_strutils.obj :  .AUTODEPEND ..\src\mimetic\strutils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_utils.obj :  .AUTODEPEND ..\src\mimetic\utils.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_version.obj :  .AUTODEPEND ..\src\mimetic\version.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_base64.obj :  .AUTODEPEND ..\src\mimetic\codec\base64.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_qp.obj :  .AUTODEPEND ..\src\mimetic\codec\qp.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_address.obj :  .AUTODEPEND ..\src\mimetic\rfc822\address.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_addresslist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\addresslist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_datetime.obj :  .AUTODEPEND ..\src\mimetic\rfc822\datetime.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_field.obj :  .AUTODEPEND ..\src\mimetic\rfc822\field.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_fieldvalue.obj :  .AUTODEPEND ..\src\mimetic\rfc822\fieldvalue.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_group.obj :  .AUTODEPEND ..\src\mimetic\rfc822\group.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mailbox.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailbox.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_mailboxlist.obj :  .AUTODEPEND ..\src\mimetic\rfc822\mailboxlist.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_messageid.obj :  .AUTODEPEND ..\src\mimetic\rfc822\messageid.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rheader.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rheader.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\wxsmtp_dll_rmessage.obj :  .AUTODEPEND ..\src\mimetic\rfc822\rmessage.cxx
	$(CXX) -bt=nt -zq -fo=$^@ $(WXSMTP_DLL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\SendMail_SendMail.obj :  .AUTODEPEND ..\samples\SendMail\SendMail.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(SENDMAIL_CXXFLAGS) $<

watmsw$(WXLIBPOSTFIX)$(_BUILDDIR_SHARED_SUFFIX)\Pop3Client_Pop3Client.obj :  .AUTODEPEND ..\samples\Pop3Client\Pop3Client.cpp
	$(CXX) -bt=nt -zq -fo=$^@ $(POP3CLIENT_CXXFLAGS) $<

