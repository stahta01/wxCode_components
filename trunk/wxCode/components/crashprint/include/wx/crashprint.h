//////////////////////////////////////////////////////////////////////////////
// File:        crashprint.h
// Purpose:     wxCrashPrint class
// Maintainer:  Wyo
// Created:     2004-09-28
// RCS-ID:      $Id: crashprint.h,v 1.4 2004-10-05 20:33:30 wyo Exp $
// Copyright:   (c) wxCode
// Licence:     wxWidgets licence
//////////////////////////////////////////////////////////////////////////////

#ifndef _WX_CRASHPRINT_H_
#define _WX_CRASHPRINT_H_

#ifdef __GNUG__
    #pragma implementation "crashprint.h"
#endif

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------
//! wxWidgets headers


//============================================================================
// declarations
//============================================================================
const int maxBtCount = 100;


//----------------------------------------------------------------------------
//!


//----------------------------------------------------------------------------
//! wxCrashPrint
class wxCrashPrint {

public:

    //! constructor
    wxCrashPrint (int flags = 0, const wxString &fname = wxEmptyString);

    //! destructor
    ~wxCrashPrint () {};

    // format and print report
    void Report ();

private:

    //! variables
    int m_flags;
    wxString m_fname;

    void *m_btBuffer [maxBtCount];
    char **m_btStrings;

};

#endif // _WX_CRASHPRINT_H_

