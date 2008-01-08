/////////////////////////////////////////////////////////////////////////////
// Name:        serport.h
// Purpose:     simple wrapper file
// Author:      Joachim Buermann
// Id:          $Id: serport.h,v 1.1.1.1 2004/11/24 10:30:11 jb Exp $
// Copyright:   (c) 2001 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_SERIALPORT_BASE_H
#define _WX_SERIALPORT_BASE_H

#if defined (WIN32)
# include "wx/ctb-0.13/win32/serport.h"
#else
# include "wx/ctb-0.13/linux/serport.h"
#endif

#endif
// __SERPORT_BASE_H

