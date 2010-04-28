// precomp.h

#define WX_PRECOMP
#include "wx/wxprec.h"

#include "wx/tokenzr.h"
#include "wx/config.h"
#include "wx/filename.h"
#include "wx/stockitem.h"
#include "wx/docview.h"

#define wxFD_DEFAULT_STYLE_OPEN (wxFD_OPEN | wxFD_FILE_MUST_EXIST)  // http://trac.wxwidgets.org/ticket/11866
#define wxFD_DEFAULT_STYLE_SAVE (wxFD_SAVE | wxFD_OVERWRITE_PROMPT) // http://trac.wxwidgets.org/ticket/11866

#define wxMODIFIED_ASTERISK wxT("*")
#define wxDEFAULT_ASTERISK  wxT("*")
