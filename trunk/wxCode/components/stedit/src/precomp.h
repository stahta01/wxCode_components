// precomp.h

#ifndef _STEDIT_PRECOMP_H_
#define _STEDIT_PRECOMP_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/menu.h>
    #include <wx/toolbar.h> 
    #include <wx/combobox.h> 
    #include <wx/msgdlg.h>
    #include <wx/filedlg.h>
#endif // WX_PRECOMP

#include <wx/tokenzr.h>
#include <wx/config.h>
#include <wx/filename.h>
#include <wx/stockitem.h>
#include <wx/docview.h> // wxFileHistory

// DO NOT INCLUDE ANY wxStEdit headers here
// We are precompiling other people's headers. If we include our headers 
// and change them (stedit development) the compile will be much slower.

#define wxFD_DEFAULT_STYLE_OPEN (wxFD_OPEN | wxFD_FILE_MUST_EXIST)  // http://trac.wxwidgets.org/ticket/11866
#define wxFD_DEFAULT_STYLE_SAVE (wxFD_SAVE | wxFD_OVERWRITE_PROMPT) // http://trac.wxwidgets.org/ticket/11866

#define wxMODIFIED_ASTERISK wxT("*")
#define wxDEFAULT_ASTERISK  wxT("*")

#endif // _STEDIT_PRECOMP_H_
