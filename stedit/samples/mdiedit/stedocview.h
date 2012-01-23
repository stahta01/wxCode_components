///////////////////////////////////////////////////////////////////////////////
// Name:        stedocview.h
// Purpose:     MDI wxSTEditor app
// Author:      Troels K
// Modified by:
// Created:     2012-01-19
// Copyright:   (c) Troels K
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __STEDOCVIEW_H__
#define __STEDOCVIEW_H__

#ifdef _STEDIT_H_

// ----------------------------------------------------------------------------
// A very simple text document class
// ----------------------------------------------------------------------------

class EditorDoc : public wxSTEditorDoc
{
public:
    EditorDoc();

    wxSTEditor* GetTextCtrl() const;

    virtual ~EditorDoc();
    virtual bool OnCreate(const wxString& path, long flags);
    virtual bool OnNewDocument();
    virtual wxString GetUserReadableName() const
    {
        return GetFilename().IsOk() ? GetFilename().GetFullPath() : wxSTEditorDoc::GetUserReadableName();
    }

protected:
    virtual bool DoSaveDocument(const wxString& filename);
    virtual bool DoOpenDocument(const wxString& filename);

    //void OnTextChange(wxCommandEvent& event);

    DECLARE_DYNAMIC_CLASS(EditorDoc)
};

// ----------------------------------------------------------------------------
// Text view classes
// ----------------------------------------------------------------------------

class EditorView : public wxSTEditorView
{
public:
    EditorView();

    virtual ~EditorView();
    virtual bool OnCreate(wxDocument*, long flags);
    virtual bool OnClose(bool deleteWindow = true);
    virtual void OnChangeFilename();
    virtual wxPrintout* OnCreatePrintout();

    EditorDoc* GetDocument()
    {
        return wxStaticCast(wxSTEditorView::GetDocument(), EditorDoc);
    }
    virtual wxWindow* GetWindow() const { return m_text; }
    
    wxSTEditor* GetEditor() const { return wxStaticCast(GetWindow(), wxSTEditor); }

private:
    void OnCopy(wxCommandEvent& WXUNUSED(event)) { m_text->Copy(); }
    void OnPaste(wxCommandEvent& WXUNUSED(event)) { m_text->Paste(); }
    void OnSelectAll(wxCommandEvent& WXUNUSED(event)) { m_text->SelectAll(); }

    wxSTEditor* m_text;

    DECLARE_EVENT_TABLE()
    DECLARE_DYNAMIC_CLASS(EditorView)
};
#endif // _STEDIT_H_

class EditorChildFrame : public wxDocMDIChildFrame
{
    DECLARE_DYNAMIC_CLASS(EditorChildFrame)
public:
    bool Create(wxView*, wxMDIParentFrame*);

    virtual ~EditorChildFrame();
};

#ifdef _STEOPTS_H_

class EditorDocTemplate : public wxDocTemplate
{
    DECLARE_CLASS(EditorDocTemplate)
    static wxDocTemplate* ms_instance;
protected:
    wxClassInfo* m_frameClassInfo;

    EditorDocTemplate(wxDocManager*, wxClassInfo* frameClassInfo);

    virtual wxFrame* CreateViewFrame(wxView*);
public:
    static wxDocTemplate* Create(wxDocManager*);
    static wxDocTemplate* GetInstance() { return ms_instance; }

    wxSTEditorOptions m_steOptions;

    friend class EditorView;
};
#endif

#endif // __STEDOCVIEW_H__
