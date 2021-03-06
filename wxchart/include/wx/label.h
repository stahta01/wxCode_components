/////////////////////////////////////////////////////////////////////////////
// Name:        label.h
// Purpose:     wxChart
// Author:      Paolo Gava
// Modified by:
// Created:
// Copyright:   (C) 2006, Paolo Gava
// RCS-ID:      $Id: label.h,v 1.1 2006-06-13 12:51:50 pgava Exp $
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#if !defined( __LABEL_H__ )
#define __LABEL_H__

//----------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "label.h"
#endif

#include "wx/charttypes.h"

//----------------------------------------------------------------------------
// Consts
//----------------------------------------------------------------------------
enum LABEL_POSITION
{
        UP      = 0x01,
        DOWN    = 0x02,
        LEFT    = 0x04,
        RIGHT   = 0x08
};

//+++-S-cd-------------------------------------------------------------------
//  NAME:       wxLabel
//  DESC:       
//  INTERFACE:
//
//----------------------------------------------------------------------E-+++
WXDLLIMPEXP_CHART class wxLabel
{
public:
    void Draw(CHART_HPAINT hp, int x, int y, ChartColor c,
              wxString& lbl, LABEL_POSITION pos);
};

#endif // __LABEL_H__

