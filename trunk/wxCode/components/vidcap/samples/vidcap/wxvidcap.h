/////////////////////////////////////////////////////////////////////////////
// Name:        wxvidcap.h
// Author:      John Labenski
// Created:     07/06/01
// Modified:    01/14/03
// Copyright:   John Labenski
// License:     wxWidgets V2.0
/////////////////////////////////////////////////////////////////////////////

#ifndef __wxvidcap_H__
#define __wxvidcap_H__

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "wx/vidcap/vcapwin.h"
#include <wx/splitter.h>
#include <wx/image.h>

#include "wxvidcap_wdr.h"

class MyApp;
class MyFrame;
class MyVideoCaptureWindow;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
    // menu items
    ID_QUIT = 1,
    ID_ABOUT,
    ID_SHOWLOG,

    ID_VIDEOWIN,

    ID_DEVICE_ENUMERATE,
    ID_DEVICENONE,
    ID_DEVICE0,
    ID_DEVICE1,
    ID_DEVICE2,
    ID_DEVICE3,
    ID_DEVICE4,
    ID_DEVICE5,
    ID_DEVICE6,
    ID_DEVICE7,
    ID_DEVICE8,
    ID_DEVICE9,
    ID_DEVICE__MAX, // not a menu item

#ifdef WXVIDCAP_AUDIO_SUPPORT
    ID_SETCAPFILENAME,
    ID_SETCAPFILESIZE,
    ID_SAVECAPTUREDVIDEOAS,
#endif // WXVIDCAP_AUDIO_SUPPORT

    ID_PREVIEWBUTTON,
    ID_PREVIEW,
    ID_PREVIEWRATE,
    ID_PREVIEWSCALED,
    ID_PREVIEWIMAGE,
    ID_OVERLAY,

    ID_IMGPROCESS_MENU,
    ID_IMGPROCESS_NEGATIVE,
    ID_IMGPROCESS_EDGE,
    ID_IMGPROCESS_MOTION,

    ID_DLGSOURCE,
    ID_DLGFORMAT,
    ID_DLGCUSTOMFORMAT,
    ID_DLGDISPLAY,
    ID_DLGPROPERTIES,

    ID_SNAPTOWINDOW,
    ID_SNAPTOCLIPBOARD,
    ID_SNAPTOFILE,
    ID_SNAPTOBMP,

#ifdef WXVIDCAP_AVI_SUPPORT
    ID_CAPVIDEO,
    ID_CAPSINGLEFRAMES,
    ID_DLGCAPPREFERENCES,
    ID_DLGCOMPRESSION,
#endif // WXVIDCAP_AVI_SUPPORT

#ifdef WXVIDCAP_AUDIO_SUPPORT
    ID_DLGAUDIO,
#endif // WXVIDCAP_AUDIO_SUPPORT

    ID_NOPREVIEWONCAP

};

// ----------------------------------------------------------------------------
// resources
// ----------------------------------------------------------------------------
// the application icon
#if defined(__WXGTK__) || defined(__WXMOTIF__)
    #include "mondrian.xpm"
#endif

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:

    virtual bool OnInit();
};


//----------------------------------------------------------------------------
// MyFrame
//----------------------------------------------------------------------------

class MyFrame: public wxFrame
{
public:
    // constructors and destructors
    MyFrame( wxWindow *parent, wxWindowID id, const wxString &title,
            const wxPoint& pos = wxDefaultPosition,
            const wxSize& size = wxDefaultSize,
            long style = wxDEFAULT_FRAME_STYLE );

    virtual ~MyFrame();

    // event handlers (these functions should _not_ be virtual)
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnShowLog(wxCommandEvent& event);

    void OnChangeDevice(wxCommandEvent &event);

    void OnPreviewButton(wxCommandEvent &event);
    void OnPreview(wxCommandEvent &event);
    void OnPreviewImage(wxCommandEvent &event);
    void OnPreviewRate(wxCommandEvent &event);
    void OnPreviewScaled(wxCommandEvent &event);
    void OnOverlay(wxCommandEvent &event);

    void OnVideoDialogs(wxCommandEvent &event);

    void OnSnapshotToWindow(wxCommandEvent &event);
    void OnSnapshotToClipboard(wxCommandEvent &event);
    void OnSnapshotToFile(wxCommandEvent &event);
    void OnSnapshotToBMP(wxCommandEvent &event);

#ifdef WXVIDCAP_AVI_SUPPORT
    void OnSetCaptureFilename(wxCommandEvent &event);
    void OnSetCaptureFilesize(wxCommandEvent &event);

    void OnCaptureVideo(wxCommandEvent &event);
    void OnCaptureSingleFrames(wxCommandEvent &event);
#endif // WXVIDCAP_AVI_SUPPORT

    void OnIdle( wxIdleEvent &event );
    void OnSize( wxSizeEvent &event );
    void OnCloseWindow( wxCloseEvent &event );

    wxSplitterWindow*     m_splitterWin;
    MyVideoCaptureWindow* m_vidCapWin;
    wxPanel*              m_logPanel;
    wxTextCtrl*           m_logTextCtrl;

    void EnumerateDevices(); // Enumerate and add the devices to the menu
    void CreateMenus();      // initially create the menus
    void SetupVideoMenus();  // enable/check items, toolbar too
    wxMenuBar* m_menubar;
    wxMenu*    m_fileMenu;
    wxMenu*    m_videoMenu;
    wxMenu*    m_processMenu;
    wxMenu*    m_captureMenu;
    wxMenu*    m_helpMenu;

    void CreateToolbar();
    wxToolBar* m_toolBar;

    int m_preview_state;    // remember last preview/overlay state

    wxString m_image_filename;  // remember image filename
    wxString m_bmp_filename;    // remember BMP filename

private:
    DECLARE_EVENT_TABLE()
};


class MyVideoCaptureWindow: public wxVideoCaptureWindow
{
public:
    MyVideoCaptureWindow( wxWindow *parent, wxWindowID id = -1,
                    const wxPoint &pos = wxDefaultPosition,
                    const wxSize &size = wxDefaultSize,
                    long style = wxSIMPLE_BORDER,
                    const wxString &name = wxT("wxvidcap"));

    virtual ~MyVideoCaptureWindow();

    void OnVideoEvent( wxVideoCaptureEvent &event );
    void OnVideoStatusEvent( wxVideoCaptureEvent &event );
    void OnVideoFrameEvent( wxVideoCaptureEvent &event );
    void OnVideoStreamEvent( wxVideoCaptureEvent &event );
    void OnVideoErrorEvent( wxVideoCaptureEvent &event );

    bool ProcesswxImageFrame();

    MyFrame *m_frame;

private:
    DECLARE_EVENT_TABLE()
};

#endif
