/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWindows sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: myminimal.cpp,v 1.1.1.1 2003-12-28 21:05:20 wyo Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------
#ifdef __GNUG__
    #pragma implementation "minimal.cpp"
    #pragma interface "minimal.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/spinctrl.h"
#include "wx/spinreal.h"

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
//#if defined(__WXGTK__) || defined(__WXMOTIF__)
//    #include "mondrian.xpm"
//#endif

// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
    // override base class virtuals
    // ----------------------------

    // this one is called on application startup and is a good place for the app
    // initialization (doing it here and not in the ctor allows to have an error
    // return: if OnInit() returns false, the application terminates)
    virtual bool OnInit();
};

// Define a new frame type: this is going to be our main frame
class MyFrame : public wxFrame
{
public:
    // ctor(s)
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSpinReal(wxSpinEvent& event);
//    RealSpinCtrl *realspinctrl;

private:
    // any class wishing to process wxWindows events must use this macro
    DECLARE_EVENT_TABLE()
};

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    Minimal_Quit = 1,
    Minimal_About,
    ID_SPINREAL
};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWindows
// ----------------------------------------------------------------------------

// the event tables connect the wxWindows events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
    EVT_MENU(Minimal_About, MyFrame::OnAbout)
    EVT_SPINCTRL(ID_SPINREAL, MyFrame::OnSpinReal)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWindows to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also declares the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------

// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
    // create the main application window
    MyFrame *frame = new MyFrame("Minimal wxWindows App",
                                 wxPoint(50, 50), wxSize(450, 340));

    // and show it (the frames, unlike simple controls, are not shown when
    // created initially)
    frame->Show(TRUE);

    // success: wxApp::OnRun() will be called which will enter the main message
    // loop and the application will run. If we returned FALSE here, the
    // application would exit immediately.
    return TRUE;
}

// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
       : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
#ifdef __WXMAC__
    // we need this in order to allow the about menu relocation, since ABOUT is
    // not the default id of the about menu
    wxApp::s_macAboutMenuItemId = Minimal_About;
#endif

    // set the frame icon
//    SetIcon(wxICON(mondrian));

    // create a menu bar
    wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);

    // the "About" item should be in the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(Minimal_About, "&About...\tCtrl-A", "Show about dialog");

    menuFile->Append(Minimal_Quit, "E&xit\tAlt-X", "Quit this program");

    // now append the freshly created menu to the menu bar...
    wxMenuBar *menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&File");
    menuBar->Append(helpMenu, "&Help");

    // ... and attach this menu bar to the frame
    SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
    // create a status bar just for fun (by default with 1 pane only)
    CreateStatusBar(2);
    SetStatusText("Welcome to wxWindows!");
#endif // wxUSE_STATUSBAR

    wxPanel *panel = new wxPanel(this, -1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    wxSpinCtrl *wxspinctrl1 = new wxSpinCtrl(panel, -1, "", wxPoint(10,10), wxDefaultSize);
    wxSpinCtrl *wxspinctrl2 = new wxSpinCtrl(panel, -1, "", wxPoint(140,10), wxSize(80,60));
    wxSpinCtrl *wxspinctrl3 = new wxSpinCtrl(panel, -1, "", wxPoint(240,10), wxSize(80,60));
    wxspinctrl3->SetFont(wxFont(30,wxDEFAULT, wxNORMAL, wxNORMAL));

    wxSpinButton *wxspinbutton1 = new wxSpinButton(panel, -1, wxPoint(340,10), wxDefaultSize);
    wxSpinButton *wxspinbutton2 = new wxSpinButton(panel, -1, wxPoint(370,10), wxSize(40,40));


    // default
    wxSpinReal *wxspinreal1 = new wxSpinReal(panel, ID_SPINREAL, "",
                                               wxPoint(10,70), wxDefaultSize);
    // some random values
    wxSpinReal *wxspinreal2 = new wxSpinReal(panel, ID_SPINREAL, "",
                                    wxPoint(150,70), wxSize(200,-1),
                                    0, -130.13, 1234.789, 123.4, .021, -1);

    wxSpinReal *wxspinreal3 = new wxSpinReal(panel, ID_SPINREAL, "",
                                    wxPoint(10,100), wxSize(80,40),
                                               0, -130.13, 1234.789, 123.456, .02, -1);
    wxspinreal3->SetIncrement( .1, 3 );

    wxSpinReal *wxspinreal4 = new wxSpinReal(panel, ID_SPINREAL, "",
                                    wxPoint(150,100), wxSize(-1,50),
                                             0, -130.13, 1234.789, 123.4, .02);
    wxspinreal4->SetSnap(TRUE);

    wxSpinReal *wxspinreal5 = new wxSpinReal(panel, ID_SPINREAL, "",
                                    wxPoint(10,150), wxSize(200,50),
                                    0, -130.13, 1234.789, 123.4, .0002);
    wxspinreal5->SetValue(321.);

    wxSpinReal *wxspinreal6 = new wxSpinReal(panel, ID_SPINREAL, "",
                                    wxPoint(220,150), wxSize(90,190),
                                    0, -130.13, 1234.789, 123.4, .1);
    wxspinreal6->SetIncrement(.003);
    wxspinreal6->SetValue(.123);


}

// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    // TRUE is to force the frame to close
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxString msg;
    msg.Printf( _T("This is the about dialog of minimal sample.\n")
                _T("Welcome to %s"), wxVERSION_STRING);

    wxMessageBox(msg, "About Minimal", wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnSpinReal(wxSpinEvent& event)
{

    GetStatusBar()->SetStatusText( event.GetString() );


}
