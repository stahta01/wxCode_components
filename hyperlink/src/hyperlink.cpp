//////////////////////////////////////////////////////////////////////////////
// File:        hyperlink.cpp
// Purpose:     wxHyperLink control
// Maintainer:  Wyo
// Created:     2003-04-07
// RCS-ID:      $Id: hyperlink.cpp,v 1.1 2004-04-30 18:14:06 wyo Exp $
// Copyright:   (c) wxGuide
// Licence:     wxWidgets licence
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// information
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// headers
//----------------------------------------------------------------------------

// For compilers that support precompilation, includes <wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all 'standard' wxWidgets headers)
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// wxWidgets headers
#include <wx/mimetype.h> // mimetype support

// hyperlink headers
#include "wx/hyperlink.h"   // wxHyperLink control


//----------------------------------------------------------------------------
// resources
//----------------------------------------------------------------------------


//============================================================================
// declarations
//============================================================================


//============================================================================
// implementation
//============================================================================

//----------------------------------------------------------------------------
// wxHyperLink
//----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS (wxHyperLink, wxStaticText)

BEGIN_EVENT_TABLE    (wxHyperLink, wxStaticText)
    EVT_ENTER_WINDOW (wxHyperLink::OnWindowEnter)
    EVT_LEAVE_WINDOW (wxHyperLink::OnWindowLeave)
    EVT_LEFT_DCLICK  (wxHyperLink::OnLinkActivate)
    EVT_LEFT_DOWN    (wxHyperLink::OnLinkActivate)
END_EVENT_TABLE()

bool wxHyperLink::Create (wxWindow *parent,
                          wxWindowID id,
                          const wxString &label,
                          const wxPoint &pos,
                          const wxSize &size,
                          long style,
                          const wxString &name) {
    bool okay = FALSE;

    // create static text
    okay = wxStaticText::Create (parent, id, label, pos, size, style, name);
    wxASSERT_MSG (okay, _("Failed to create wxStaticText, needed by wxHyperLink!"));

    // initialize variables
    m_URL = wxEmptyString;
    m_Marked = false;
    m_Visited = false;
    m_MarkedColour = wxColour ("DARK GREY");
    m_NormalColour = wxColour ("BLUE");
    m_VisitedColour = wxColour ("PURPLE");
    m_HoverCursor = wxCursor (wxCURSOR_HAND);

    // set foreground colour
    SetForegroundColour (m_NormalColour);
    wxFont font = GetFont();
    font.SetUnderlined (true);
    SetFont (font);

    // get background colour
    m_BackgroundColour = GetBackgroundColour ();

    return okay;
} // Create

//----------------------------------------------------------------------------
// event handlers

void wxHyperLink::OnWindowEnter (wxMouseEvent &WXUNUSED(event)) {
    SetCursor (m_HoverCursor);
    Refresh();
}

void wxHyperLink::OnWindowLeave (wxMouseEvent &WXUNUSED(event)) {
    SetCursor (wxNullCursor);
    Refresh();
}

void wxHyperLink::OnLinkActivate (wxMouseEvent &WXUNUSED(event)) {
    m_Visited = TRUE;
    SetForegroundColour (m_VisitedColour);
    SetBackgroundColour (m_BackgroundColour);
    Refresh();
    if (m_URL.IsEmpty()) {
       ExecuteLink (GetLabel());
    }else{
       ExecuteLink (m_URL);
    }
}

//----------------------------------------------------------------------------
// settings functions

wxCursor wxHyperLink::GetHoverCursor () {
    return m_HoverCursor;
}

void wxHyperLink::SetHoverCursor (wxCursor cursor) {
    m_HoverCursor = cursor;
}

wxColour wxHyperLink::GetMarkedColour () {
    return m_MarkedColour;
}

void wxHyperLink::SetMarkedColour (wxColour colour) {
    m_MarkedColour = colour;
}

wxColour wxHyperLink::GetNormalColour () {
    return m_NormalColour;
}

void wxHyperLink::SetNormalColour (wxColour colour) {
    m_NormalColour = colour;
    if (!m_Visited) {
        SetForegroundColour (m_NormalColour);
    }else{
        SetForegroundColour (m_VisitedColour);
    }
    Refresh();
}

wxColour wxHyperLink::GetVisitedColour () {
    return m_VisitedColour;
}

void wxHyperLink::SetVisitedColour (wxColour colour) {
    m_VisitedColour = colour;
    if (!m_Visited) {
        SetForegroundColour (m_NormalColour);
    }else{
        SetForegroundColour (m_VisitedColour);
    }
    Refresh();
}

wxString wxHyperLink::GetURL () {
    return m_URL;
}

void wxHyperLink::SetURL (const wxString &url) {
    m_URL = url;
}

//----------------------------------------------------------------------------
// private functions

void wxHyperLink::ExecuteLink (const wxString &link) {
    wxString mimetype = wxEmptyString;
    if (link.StartsWith (_T("http://"))) {
        mimetype = _T("text/html");
    }else if (link.StartsWith (_T("ftp://"))) {
        mimetype = _T("text/html");
    }else if (link.StartsWith (_T("mailto:"))) {
        mimetype = _T("message/rfc822");
    }else{
        return;
    }
    wxFileType *filetype = wxTheMimeTypesManager->GetFileTypeFromMimeType (mimetype);
    if (filetype) {
        wxString cmd;
        if (filetype->GetOpenCommand (&cmd, wxFileType::MessageParameters (link))) {
            cmd.Replace(_T("file://"), wxEmptyString);
            ::wxExecute(cmd);
        }
    }
}
