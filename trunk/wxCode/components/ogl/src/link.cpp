// link.cpp
// Copyright (c) 2009 by Troels K. All rights reserved.
// License: wxWindows

#if defined(_MSC_VER) && defined(WXMAKINGDLL_OGL)

#include <wx/wx.h>

#if (wxVERSION_NUMBER >= 2900)
   #if wxUSE_UNICODE
      #ifdef __WXDEBUG__
         #pragma comment(lib, "wxbase29ud.lib")
         #pragma comment(lib, "wxmsw29ud_core.lib")
      #else
         #pragma comment(lib, "wxbase29u.lib")
         #pragma comment(lib, "wxbase29u_xml.lib")
         #pragma comment(lib, "wxmsw29u_core.lib")
         #pragma comment(lib, "wxmsw29u_adv.lib")
         #pragma comment(lib, "wxtiff.lib")
         #pragma comment(lib, "wxjpeg.lib")
         #pragma comment(lib, "wxpng.lib")
      #endif
   #else
      #ifdef __WXDEBUG__
         #pragma comment(lib, "wxbase29d.lib")
         #pragma comment(lib, "wxmsw29d_core.lib")
      #else
         #pragma comment(lib, "wxbase29.lib")
         #pragma comment(lib, "wxbase29_xml.lib")
         #pragma comment(lib, "wxmsw29_core.lib")
         #pragma comment(lib, "wxmsw29_adv.lib")
         #pragma comment(lib, "wxtiff.lib")
         #pragma comment(lib, "wxjpeg.lib")
         #pragma comment(lib, "wxpng.lib")
      #endif
   #endif
#elif (wxVERSION_NUMBER >= 2800)
   #if wxUSE_UNICODE
      #ifdef __WXDEBUG__
         #pragma comment(lib, "wxbase28ud.lib")
         #pragma comment(lib, "wxmsw28ud_core.lib")
         #pragma comment(lib, "wxbase28ud_xml.lib")
      #else
         #pragma comment(lib, "wxbase28u.lib")
         #pragma comment(lib, "wxbase28u_xml.lib")
         #pragma comment(lib, "wxmsw28u_core.lib")
         #pragma comment(lib, "wxmsw28u_adv.lib")
         #pragma comment(lib, "wxtiff.lib")
         #pragma comment(lib, "wxjpeg.lib")
         #pragma comment(lib, "wxpng.lib")
      #endif
   #else
      #ifdef __WXDEBUG__
         #pragma comment(lib, "wxbase28d.lib")
         #pragma comment(lib, "wxmsw28d_core.lib")
         #pragma comment(lib, "wxbase28d_xml.lib")
      #else
         #pragma comment(lib, "wxbase28.lib")
         #pragma comment(lib, "wxmsw28_core.lib")
         #pragma comment(lib, "wxmsw28_adv.lib")
/*
         #pragma comment(lib, "wxbase28.lib")
         #pragma comment(lib, "wxmsw28_core.lib")
         #pragma comment(lib, "wxbase28_xml.lib")
         #pragma comment(lib, "wxmsw28_adv.lib")
         #pragma comment(lib, "wxtiff.lib")
         #pragma comment(lib, "wxjpeg.lib")
         #pragma comment(lib, "wxpng.lib")
*/
      #endif
   #endif
#endif

#endif
