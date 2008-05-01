/////////////////////////////////////////////////////////////////////////////
// Name:        json_defs.h
// Purpose:     shared build defines
// Author:      Luciano Cattani
// Created:     2007/10/20
// RCS-ID:      $Id: json_defs.h,v 1.6 2008/03/12 10:48:19 luccat Exp $
// Copyright:   (c) 2007 Luciano Cattani
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_JSON_DEFS_H_
#define _WX_JSON_DEFS_H_

// Defines for component version.
// The following symbols should be updated for each new component release
// since some kind of tests, like those of AM_WXCODE_CHECKFOR_COMPONENT_VERSION()
// for "configure" scripts under unix, use them.
#define wxJSON_MAJOR          0
#define wxJSON_MINOR          5
#define wxJSON_RELEASE        0

// For non-Unix systems (i.e. when building without a configure script),
// users of this component can use the following macro to check if the
// current version is at least major.minor.release
#define wxCHECK_JSON_VERSION(major,minor,release) \
    (wxJSON_MAJOR > (major) || \
    (wxJSON_MAJOR == (major) && wxJSON_MINOR > (minor)) || \
    (wxJSON_MAJOR == (major) && wxJSON_MINOR == (minor) && wxJSON_RELEASE >= (release)))


// Defines for shared builds.
// Simple reference for using these macros and for writin components
// which support shared builds:
//
// 1) use the WXDLLIMPEXP_MYCOMP in each class declaration:
//          class WXDLLIMPEXP_MYCOMP myCompClass {   [...]   };
//
// 2) use the WXDLLIMPEXP_MYCOMP in the declaration of each global function:
//          WXDLLIMPEXP_MYCOMP int myGlobalFunc();
//
// 3) use the WXDLLIMPEXP_DATA_MYCOMP() in the declaration of each global
//    variable:
//          WXDLLIMPEXP_DATA_MYCOMP(int) myGlobalIntVar;
//
#ifdef WXMAKINGDLL_JSON
    #define WXDLLIMPEXP_JSON                  WXEXPORT
    #define WXDLLIMPEXP_DATA_JSON(type)       WXEXPORT type
#elif defined(WXUSINGDLL)
    #define WXDLLIMPEXP_JSON                  WXIMPORT
    #define WXDLLIMPEXP_DATA_JSON(type)       WXIMPORT type
#else // not making nor using DLL
    #define WXDLLIMPEXP_JSON
    #define WXDLLIMPEXP_DATA_JSON(type)	    type
#endif

// the __PRETTY_FUNCTION__ macro expands to the full class's
// member name in the GNU GCC.
// For other compilers you have to set the correct macro
//#if !defined( __GNUC__ )
//  #define __PRETTY_FUNCTION__   __WXFUNCTION__
//#endif

#define __PRETTY_FUNCTION__   __WXFUNCTION__


// define wxJSON_USE_UNICODE if wxWidgets was built with
// unicode support
#if defined( wxJSON_USE_UNICODE )
  #undef wxJSON_USE_UNICODE
#endif

// do not modify the following lines
#if wxUSE_UNICODE == 1
  #define wxJSON_USE_UNICODE
#endif

// the following macro, if defined, cause the wxJSONValue to store
// pointers to C-strings as pointers to statically allocated
// C-strings
// by default this macro is not defined
// #define WXJSON_USE_CSTRING


// the following macro, if defined, cause the wxJSONvalue and its
// referenced data structure to store and increment a static
// progressive counter in the ctor.
// this is only usefull for debugging purposes
// #define WXJSON_USE_VALUE_COUNTER


// the following macro is used by wxJSON internally and you should not
// modify it. If the platform seems to support 64-bits integers,
// the following lines define the 'wxJSON_64BIT_INT' macro
#if defined( wxLongLong_t )
#define wxJSON_64BIT_INT
#endif


//
// the following macro, if defined, cause the wxJSON library to
// always use 32-bits integers also when the platform seems to
// have native 64-bits support: by default the macro if not defined
// #define wxJSON_NO_64BIT_INT
#if defined( wxJSON_NO_64BIT_INT ) && defined( wxJSON_64BIT_INT )
#undef wxJSON_64BIT_INT
#endif





#endif // _WX_JSON_DEFS_H_


