dnl ### begin block 00_header ###
dnl
dnl This macro was generated by
dnl Bakefile 0.1.4 (http://bakefile.sourceforge.net)
dnl Do not modify, all changes will be overwritten!

BAKEFILE_AUTOCONF_INC_M4_VERSION="0.1.4"

dnl ### begin block 20_COND_BUILD_DEBUG ###
    COND_BUILD_DEBUG="#"
    if test "x$BUILD" = "xdebug" ; then
        COND_BUILD_DEBUG=""
    fi
    AC_SUBST(COND_BUILD_DEBUG)
dnl ### begin block 20_COND_BUILD_DEBUG_UNICODE_0 ###
    COND_BUILD_DEBUG_UNICODE_0="#"
    if test "x$BUILD" = "xdebug" -a "x$UNICODE" = "x0" ; then
        COND_BUILD_DEBUG_UNICODE_0=""
    fi
    AC_SUBST(COND_BUILD_DEBUG_UNICODE_0)
dnl ### begin block 20_COND_BUILD_DEBUG_UNICODE_1 ###
    COND_BUILD_DEBUG_UNICODE_1="#"
    if test "x$BUILD" = "xdebug" -a "x$UNICODE" = "x1" ; then
        COND_BUILD_DEBUG_UNICODE_1=""
    fi
    AC_SUBST(COND_BUILD_DEBUG_UNICODE_1)
dnl ### begin block 20_COND_BUILD_RELEASE ###
    COND_BUILD_RELEASE="#"
    if test "x$BUILD" = "xrelease" ; then
        COND_BUILD_RELEASE=""
    fi
    AC_SUBST(COND_BUILD_RELEASE)
dnl ### begin block 20_COND_BUILD_RELEASE_UNICODE_1 ###
    COND_BUILD_RELEASE_UNICODE_1="#"
    if test "x$BUILD" = "xrelease" -a "x$UNICODE" = "x1" ; then
        COND_BUILD_RELEASE_UNICODE_1=""
    fi
    AC_SUBST(COND_BUILD_RELEASE_UNICODE_1)
dnl ### begin block 20_COND_DEPS_TRACKING_0 ###
    COND_DEPS_TRACKING_0="#"
    if test "x$DEPS_TRACKING" = "x0" ; then
        COND_DEPS_TRACKING_0=""
    fi
    AC_SUBST(COND_DEPS_TRACKING_0)
dnl ### begin block 20_COND_DEPS_TRACKING_1 ###
    COND_DEPS_TRACKING_1="#"
    if test "x$DEPS_TRACKING" = "x1" ; then
        COND_DEPS_TRACKING_1=""
    fi
    AC_SUBST(COND_DEPS_TRACKING_1)
dnl ### begin block 20_COND_PLATFORM_MAC_1 ###
    COND_PLATFORM_MAC_1="#"
    if test "x$PLATFORM_MAC" = "x1" ; then
        COND_PLATFORM_MAC_1=""
    fi
    AC_SUBST(COND_PLATFORM_MAC_1)
dnl ### begin block 20_COND_PLATFORM_OS2_1 ###
    COND_PLATFORM_OS2_1="#"
    if test "x$PLATFORM_OS2" = "x1" ; then
        COND_PLATFORM_OS2_1=""
    fi
    AC_SUBST(COND_PLATFORM_OS2_1)
dnl ### begin block 20_COND_SHARED_0 ###
    COND_SHARED_0="#"
    if test "x$SHARED" = "x0" ; then
        COND_SHARED_0=""
    fi
    AC_SUBST(COND_SHARED_0)
