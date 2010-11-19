///////////////////////////////////////////////////////////////////////////////
// Name:        app.cpp
// Purpose:     Simple wxSTEditor app
// Author:      John Labenski
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: wxstedit.cpp,v 1.18 2007/07/23 00:28:46 jrl1 Exp $
// Copyright:   (c) John Labenski
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// This nearly the absolute minimum of code to get an editor
/*
    // Create the options and tweak them as necessary
    wxSTEditorOptions steOptions(STE_DEFAULT_OPTIONS);
    steOptions.GetMenuManager()->SetMenuOptionType(STE_MENU_NOTEBOOK, true);
    // Create the frame for the editor
    wxSTEditorFrame *editor = new wxSTEditorFrame(NULL, wxID_ANY, wxT("Editor"));
    // Have the frame create the children and menus from the options
    // or you can do this part by hand
    editor->CreateOptions(steOptions);
    // optionally start with a file you load from disk or memory and show the frame
    editor->GetEditor()->LoadFile(wxT("textfile.txt"));
    editor->Show(true);
*/
// ----------------------------------------------------------------------------


// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/filename.h"

#include "wx/stedit/stedit.h"
#include "wx/stedit/steshell.h"
#include "../../art/pencil16.xpm"

#include "wx/cmdline.h"
#include "wx/config.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/html/htmlwin.h"
#include "wx/clipbrd.h"

#include "../../src/wxext.h" // wxLocale_Init()
#include "app.h"

#include "wxstedit_htm.hpp" // include docs
#include "readme_htm.hpp"

// ----------------------------------------------------------------------------

enum Menu_IDs
{
    ID_SHOW_HELP      = ID_STE__LAST, // IDs greater than this won't conflict
    ID_SHOW_README,
#if (wxVERSION_NUMBER >= 2900) || defined(__WXMSW__)
    ID_SHOW_USAGE,
#endif
    ID_TEST_STESHELL
};

// ----------------------------------------------------------------------------

IMPLEMENT_APP(wxStEditApp)

wxStEditApp::wxStEditApp() : wxApp(),
            m_steOptions(STE_DEFAULT_OPTIONS, STS_DEFAULT_OPTIONS, STN_DEFAULT_OPTIONS, STF_DEFAULT_OPTIONS)
{
}

bool wxStEditApp::OnInit()
{
    if (!wxApp::OnInit()) // parse command line
        return false;

    SetAppName(STE_APPNAME);
#if (wxVERSION_NUMBER >= 2900)
    SetAppDisplayName(STE_APPDISPLAYNAME);
#endif
    ::wxLocale_Init(&m_locale, STE_APPNAME, m_cmdline.m_lang);
    wxSTEditorOptions::SetGlobalDefaultFileName(wxString(_("unnamed")) + wxT(".txt")); // translated

    // Create a set of options for your editing "system."
    //  These options control what components will be automatically
    //  created and/or managed for you. For every window created the
    //  virtual function CreateOptions will be called.
    //  You can start with all the options "turned off" by using
    //  the default constructor.

    // For this simple editor we'll basicly use the defaults for everything
    //  (we reset it in cmd line parser to use either a single editor or notebook)
    wxSTEditorOptions::RegisterIds();

    // =======================================================================
    // A sample of things that you might do to change the behavior

    // no bookmark items in menus or toolbar
    //m_steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_BOOKMARK, false);
    //m_steOptions.GetMenuManager()->SetToolbarToolType(STE_MENU_BOOKMARK, false);

    // don't create a toolbar
    //m_steOptions.SetFrameOption(STF_TOOLBAR, false);
    // allow notebook to not have any pages
    //m_steOptions.SetNotebookOption(STN_ALLOW_NO_PAGES, true);
    // don't ask the user to save a modified document, close silently
    //m_steOptions.SetEditorOption(STE_QUERY_SAVE_MODIFIED, false);

    // Maybe we're editing only python files, set global initializers
    // wxSTEditorOptions::SetGlobalDefaultFileName(wxT("newfile.py"));
    // wxSTEditorOptions::SetGlobalDefaultFileExtensions(wxT("Python file (*.py)|*.py"));

    // maybe the editors that use these options are only for your ini files
    // m_steOptions.SetDefaultFileName(wxT("MyProgram.ini"));

    // Maybe you want your own special menu for the splitter?
    //  it'll delete the old one (if there was one) and replace it with yours.
    // m_steOptions.SetSplitterPopupMenu(myMenu, false);

    // Maybe you want this editor to not use the global preferences,
    //  create a new one, set it up the way you like it and push it onto the
    //  options so that every new editor sharing these options will use it.
    //  Remember, you can later detach a single editors to have them
    //  use some other prefs/styles/langs with STE::RegisterXXX(otherXXX)
    // wxSTEditorPrefs myPrefs(true);
    // myPrefs.SetPrefBool(STE_PREF_VIEW_EOL, true);
    // m_steOptions.SetEditorPrefs(myPrefs);

    // You can do the same for the styles and langs, though the languages
    //  are pretty generic and it probably won't be necessary.

    // Remember, the global versions are created to be used by a set of editors
    //  they are created because if a user likes their editor a
    //  certain way, you might as well make all of them look that way.
    //  There is nothing special about them and if you want to see what the
    //  defaults are just create a wxSTEditorPrefs/Styles/Langs(true).

    // etc... Ok, we set things up the way we like.

    // end sample code
    // =======================================================================

    // Remove the Help menu since wxMac will pull out the wxID_ABOUT to add to
    // the system menu and then hide the Help menu. Later on when we add items
    // to the help menu, they'll be hidden too.
    m_steOptions.GetMenuManager()->SetMenuItems(STE_MENU_HELP_MENU, 0);

    // create with the readonly menuitem, not set by default since I don't think
    //  it's generally useful, but good for debugging.
    m_steOptions.GetMenuManager()->SetMenuItemType(STE_MENU_EDIT_MENU, STE_MENU_EDIT_READONLY, true);
    m_steOptions.GetMenuManager()->SetToolbarToolType(STE_TOOLBAR_EDIT_FINDCOMBO, true);
    m_steOptions.GetMenuManager()->SetToolbarToolType(STE_TOOLBAR_PRINT, true);
    m_steOptions.GetMenuManager()->SetToolbarToolType(STE_TOOLBAR_EXIT, true);
    m_steOptions.SetNotebookOption(STN_ALPHABETICAL_TABS, false); // Ctrl+N -> append tab on the left always
    m_steOptions.GetMenuManager()->GetAcceleratorArray()->Add(wxAcceleratorEntry(wxACCEL_NORMAL, WXK_F10, wxID_EXIT)); // adding one 'custom' accelerator

    // ------------------------------------------------------------------------
    wxSTEditorFrame* frame = new wxSTEditorFrame(NULL, wxID_ANY);

    // ------------------------------------------------------------------------
    // load the prefs/style/langs from the config, if we're using one
    if (frame->GetConfigBase())
        m_steOptions.LoadConfig(*frame->GetConfigBase());

    // must call this if you want any of the options, else blank frame
    frame->CreateOptions(m_steOptions);

    // Get the "Help" menu
    wxMenu* menu = new wxMenu; //frame->GetMenuBar()->GetMenu(frame->GetMenuBar()->GetMenuCount()-1);

    wxMenuItem* item = new wxMenuItem(menu, wxID_ABOUT, _("&About..."), _("About this program"));
    item->SetBitmap(wxBitmap(pencil16_xpm));
    menu->Append(item);

    // Add our help dialogs
    menu->Append(ID_SHOW_HELP, _("Help..."), wxString::Format(_("Show help on using %s"), STE_APPDISPLAYNAME));
    menu->Append(ID_SHOW_README, _("Programming help..."), wxString::Format(_("Show help on the %s library"), STE_APPDISPLAYNAME));

    // just use connect here, we could also use static event tables, but this
    //  is easy enough to do.
    frame->Connect(ID_SHOW_HELP, wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(wxStEditApp::OnMenuEvent), NULL, this);
    frame->Connect(ID_SHOW_README, wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(wxStEditApp::OnMenuEvent), NULL, this);
#if (wxVERSION_NUMBER >= 2900) || defined(__WXMSW__)
    menu->Append(ID_SHOW_USAGE, _("C&ommand line usage..."), wxString::Format(_("Show command line help")));
    frame->Connect(ID_SHOW_USAGE, wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(wxStEditApp::OnMenuEvent), NULL, this);
#endif
    // add menu item for testing the shell
    menu->AppendSeparator();
    menu->Append(ID_TEST_STESHELL, _("Test STE shell..."), _("Test the STE shell component"));
    frame->Connect(ID_TEST_STESHELL, wxEVT_COMMAND_MENU_SELECTED,
                     wxCommandEventHandler(wxStEditApp::OnMenuEvent), NULL, this);

    frame->GetMenuBar()->Append(menu, _("&Help"));

    // ------------------------------------------------------------------------
    // handle loading the files
    size_t n;
    wxArrayString badFileNames;
    wxArrayString fileNames = m_cmdline.m_fileNames;

    // handle recursive file loading
    if (m_cmdline.m_recurse && frame->GetEditorNotebook())
    {
        int max_page_count = frame->GetEditorNotebook()->GetMaxPageCount();

        wxArrayString recurseFileNames;
        for (n = 0; n < fileNames.GetCount(); n++)
        {
            wxFileName fN(fileNames[n]);
            fN.MakeAbsolute();
            //wxPrintf(wxT("Loading file '%s' to '%s'\n"), fileNames[n].wx_str(), fN.GetFullPath().wx_str()); fflush(stdout);
            wxDir::GetAllFiles(fN.GetPath(), &recurseFileNames, fN.GetFullName());

            // if they did wxstedit /r c:\*.* stop the insanity...
            if ((int)recurseFileNames.GetCount() >= max_page_count)
            {
                wxString msg = wxString::Format(_("Opening %d files, unable to open any more."), max_page_count);
                wxMessageBox(msg, _("Maximum number of files"), wxOK|wxICON_ERROR, frame);
                recurseFileNames.RemoveAt(max_page_count - 1, recurseFileNames.GetCount() - max_page_count);
                break;
            }
        }

        //for (n=0; n < recurseFileNames.GetCount(); n++)
        //  { wxPrintf(wxT("Loading file '%s'\n"), recurseFileNames[n].wx_str()); fflush(stdout); }

        fileNames = recurseFileNames; // these are really the files to open
    }

    // if the files have *, ? or are directories, don't try to load them
    for (n = 0; n < fileNames.GetCount(); n++)
    {
        if (wxIsWild(fileNames[n]))
        {
            badFileNames.Add(fileNames[n]);
            fileNames.RemoveAt(n);
            n--;
        }
        else if (wxDirExists(fileNames[n]))
        {
            fileNames.RemoveAt(n);
            n--;
        }
    }

    // If there are any good files left, try to load them
    if (fileNames.GetCount() > 0u)
    {
        if (wxFileExists(fileNames[0]))
            frame->GetEditor()->LoadFile( fileNames[0] );
        else
        {
            // fix the path to the new file using the command line path
            wxFileName fn(fileNames[0]);
            fn.Normalize();
            frame->GetEditor()->NewFile( fn.GetFullPath() );
        }

        fileNames.RemoveAt(0);
        if (m_steOptions.HasFrameOption(STF_CREATE_NOTEBOOK) && fileNames.GetCount())
            frame->GetEditorNotebook()->LoadFiles( &fileNames );
    }
    //frame->ShowSidebar(false);
    //wxSTEditorOptions::m_path_display_format = wxPATH_UNIX; // trac.wxwidgets.org/ticket/11947
    frame->Show();
    SetTopWindow(frame);

    // filenames had *, ? or other junk so we didn't load them
    if (badFileNames.GetCount())
    {
        wxString msg(_("There was a problem trying to load file(s):\n"));
        for (n=0; n < badFileNames.GetCount(); n++)
            msg += wxT("'") + badFileNames[n] + wxT("'\n");

        wxMessageBox(msg, _("Unable to load file(s)"), wxOK|wxICON_ERROR,
                     frame);
    }

    return true;
}

int wxStEditApp::OnExit()
{
    wxTheClipboard->Flush();
    delete wxConfigBase::Set(NULL);
    return wxApp::OnExit();
}

wxSTEditorFrame* wxStEditApp::GetTopWindow()
{
    return wxStaticCast(wxApp::GetTopWindow(), wxSTEditorFrame);
}

void wxStEditApp::CreateShell()
{
    wxSTEditorFrame* frame = GetTopWindow();

    wxDialog dialog(frame, wxID_ANY, wxT("wxSTEditorShell"),
                    wxDefaultPosition, wxDefaultSize,
                    wxDEFAULT_DIALOG_STYLE_RESIZE);
    wxSTEditorShell* shell = new wxSTEditorShell(&dialog, wxID_ANY);
    // Set the styles and langs to those of the frame (not necessary, but nice)
    // The prefs aren't shared since we want to control the look and feel.
    wxSTEditorPrefs prefs(true);
    prefs.SetPrefInt(STE_PREF_INDENT_GUIDES, 0);
    prefs.SetPrefInt(STE_PREF_EDGE_MODE, wxSTC_EDGE_NONE);
    prefs.SetPrefInt(STE_PREF_VIEW_LINEMARGIN, 0);
    prefs.SetPrefInt(STE_PREF_VIEW_MARKERMARGIN, 1);
    prefs.SetPrefInt(STE_PREF_VIEW_FOLDMARGIN, 0);
    shell->RegisterPrefs(prefs);
    shell->RegisterStyles(frame->GetOptions().GetEditorStyles());
    shell->RegisterLangs(frame->GetOptions().GetEditorLangs());
    shell->SetLanguage(STE_LANG_PYTHON); // arbitrarily set to python

    shell->BeginWriteable();
    shell->AppendText(_("Welcome to a test of the wxSTEditorShell.\n\n"));
    shell->AppendText(_("This simple test merely responds to the wxEVT_STESHELL_ENTER\n"));
    shell->AppendText(_("events and prints the contents of the line when you press enter.\n\n"));
    shell->AppendText(_("For demo purposes, the shell understands these simple commands.\n"));
    shell->AppendText(_(" SetMaxHistoryLines num : set the number of lines in history buffer\n"));
    shell->AppendText(_(" SetMaxLines num [overflow=2000] : set the number of lines displayed\n"));
    shell->AppendText(_("   and optionally the number of lines to overflow before deleting\n"));
    shell->AppendText(_(" Quit : quit the wxSTEditorShell demo\n"));
    shell->CheckPrompt(true); // add prompt
    shell->EndWriteable();

    shell->Connect(wxID_ANY, wxEVT_STESHELL_ENTER,
                   wxSTEditorEventHandler(wxStEditApp::OnSTEShellEvent), NULL, this);

    int width = shell->TextWidth(wxSTC_STYLE_DEFAULT,
                                 wxT(" SetMaxHistoryLines num : set the number of lines in history buffer  "));
    dialog.SetSize(width + 30, wxDefaultCoord);

    wxBoxSizer *topSizer = new wxBoxSizer( wxVERTICAL );
    topSizer->Add(shell, 1, wxEXPAND);
    dialog.SetSizer(topSizer);
    dialog.ShowModal();
}

wxFrame* wxStEditApp::CreateHelpFrame(const wxString& caption, const char* text)
{
    wxFrame *helpFrame = new wxFrame(NULL, wxID_ANY, caption,
                                     wxDefaultPosition, wxSize(600,400));
    wxHtmlWindow *htmlWin = new wxHtmlWindow(helpFrame);

    if (htmlWin->SetPage(wxConvertMB2WX(text)))
    {
        helpFrame->Centre();
        helpFrame->Show();
    }
    else
    {
        htmlWin->SetPage(_("Unable to display help, sorry!"));
    }
    return helpFrame;
}

void wxStEditApp::OnMenuEvent(wxCommandEvent& event)
{
    switch (event.GetId())
    {
        case ID_SHOW_HELP:
            CreateHelpFrame(wxString::Format(_("Help for %s"), STE_APPDISPLAYNAME), (const char*)wxstedit_htm);
            break;
        case ID_SHOW_README:
            CreateHelpFrame(wxString::Format(_("Programming help for %s"), STE_APPDISPLAYNAME), (const char*)readme_htm);
            break;
#if (wxVERSION_NUMBER >= 2900) || defined(__WXMSW__)
        case ID_SHOW_USAGE:
            ::wxCommandLineUsage(GetTopWindow());
            break;
#endif
        case ID_TEST_STESHELL :
            CreateShell();
            break;
        default:
            event.Skip();
            break;
    }
}

void wxStEditApp::OnSTEShellEvent(wxSTEditorEvent& event)
{
    // handle the event and for this example we just write it back
    wxSTEditorShell* shell = wxDynamicCast(event.GetEventObject(), wxSTEditorShell);
    wxString val = event.GetString();
    shell->AppendText(_("\nText Entered : '") + val + wxT("'\n"));

    // very simple mechanism to parse the line to do things, you may prefer
    //   using wxPython or wxLua and running the string as is.
    wxString token(val.BeforeFirst(wxT(' ')).Lower());

    if (val.Lower().Strip(wxString::both) == wxT("quit"))
    {
        wxCommandEvent quitEvent(wxEVT_COMMAND_BUTTON_CLICKED, wxID_OK);
        event.SetEventObject(shell);
        shell->GetEventHandler()->ProcessEvent(quitEvent);
    }
    else if (token == wxT("setmaxhistorylines"))
    {
        wxString num = val.AfterFirst(wxT(' '));
        long n = 0;
        if (num.ToLong(&n))
        {
            shell->SetMaxHistoryLines(n);
            shell->AppendText(wxString::Format(_("The maximum number of history lines is set to %d.\n"), n));
        }
        else
            shell->AppendText(_("ERR: Expected number, eg. SetMaxHistoryLines 10\n"));
    }
    else if (token == wxT("setmaxlines"))
    {
        wxString num1 = val.AfterFirst(wxT(' ')).Strip(wxString::both);
        wxString num2 = num1.AfterFirst(wxT(' ')).Strip(wxString::both);
        num1 = num1.BeforeFirst(wxT(' ')).Strip(wxString::both);

        long n1 = 0, n2 = 2000;
        if (num1.ToLong(&n1) && (num2.IsEmpty() || num2.ToLong(&n2)))
        {

            shell->SetMaxLines(n1, n2);
            shell->AppendText(wxString::Format(_("The maximum number of displayed lines is set to\n  %d with an overflow of %d lines before deleting up to max lines.\n"), n1, n2));
        }
        else
            shell->AppendText(_("ERR: Expected number, eg. SetMaxLines 10 [2000]\n"));
    }

    shell->CheckPrompt(true);
}
