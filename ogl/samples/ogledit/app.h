/////////////////////////////////////////////////////////////////////////////
// Name:        contrib/samples/ogl/ogledit/ogledit.h
// Purpose:     OGL sample
// Author:      Julian Smart
// Modified by:
// Created:     12/07/98
// RCS-ID:      $Id: ogledit.h,v 1.1 2007/03/28 15:15:49 frm Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _OGLSAMPLE_OGLEDIT_H_
#define _OGLSAMPLE_OGLEDIT_H_

#include "wx/docview.h"
#include "wx/ogl/ogl.h" // base header of OGL, includes and adjusts wx/deprecated/setup.h
#include "wx29.h"

// Define a new application
class MyFrame;
class EditorToolPalette;
class MyApp : public wxAppEx
{
    typedef wxAppEx base;
public:
    MyApp(void);
    
    virtual ~MyApp(void);
    virtual bool OnInit(void);
    virtual int OnExit(void);

    EditorToolPalette* CreatePalette(wxFrame* parent);
    MyFrame* GetMainFrame(void);
protected:
    wxDocManager* CreateDocManager(void);
};

DECLARE_APP(MyApp)

// Define a new frame
class MyCanvas;
class MyFrame: public wxDocParentFrame
{
    typedef wxDocParentFrame base;
    DECLARE_CLASS(MyFrame)
public:
    wxMenu *editMenu;

    MyCanvas *canvas;
    EditorToolPalette *palette;

    MyFrame(wxDocManager*, wxFrame*parent, const wxString& title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE);
    virtual ~MyFrame();

    MyCanvas *CreateCanvas(wxView*, wxFrame*parent);
protected:
    void OnSize(wxSizeEvent&);
    void OnAbout(wxCommandEvent&);
    DECLARE_EVENT_TABLE()
};

// Menu/undo/redo commands

enum
{
    OGLEDIT_ADD_SHAPE = wxID_HIGHEST,
    OGLEDIT_ADD_LINE,
    OGLEDIT_EDIT_LABEL,
    OGLEDIT_CHANGE_BACKGROUND_COLOUR,
    OGLEDIT_ABOUT = wxID_ABOUT
};

#endif
    // _OGLSAMPLE_OGLEDIT_H_
