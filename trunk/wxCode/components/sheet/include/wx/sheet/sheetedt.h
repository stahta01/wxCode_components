///////////////////////////////////////////////////////////////////////////////
// Name:        sheetedt.h
// Purpose:     wxSheet controls
// Author:      John Labenski, Paul Gammans, Roger Gammans
// Modified by: John Labenski
// Created:     11/04/2001
// RCS-ID:      $Id: sheetedt.h,v 1.6 2007/12/12 05:22:38 jrl1 Exp $
// Copyright:   (c) John Labenski, The Computer Surgery (paul@compsurg.co.uk)
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_SHEETEDT_H__
#define __WX_SHEETEDT_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "sheetedt.h"
#endif

#include "wx/sheet/sheetdef.h"
#include <wx/datetime.h>
#include <wx/bitmap.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>

class WXDLLIMPEXP_FWD_SHEET wxSheetCellEditorRefData;

class WXDLLIMPEXP_FWD_CORE wxWindow;
class WXDLLIMPEXP_FWD_CORE wxCheckBox;
class WXDLLIMPEXP_FWD_CORE wxComboBox;
class WXDLLIMPEXP_FWD_CORE wxTextCtrl;
class WXDLLIMPEXP_FWD_CORE wxSpinCtrl;

// ----------------------------------------------------------------------------
// wxSheetCellEditorEvtHandler - an event handler for the editors that
//  forwards the keydown and char events to the wxSheetCellEditorRefData member.
//
// Note: We use a wxEvtHandler derived class rather than wxEvtHandler::Connect
// to get the events from the controls because we would like to store a
// pointer to the editor and the wxSheet parent.
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellEditorEvtHandler : public wxEvtHandler
{
public:
    wxSheetCellEditorEvtHandler() : m_sheet(NULL), m_editor(NULL) { }
    wxSheetCellEditorEvtHandler(wxSheet* sheet, const wxSheetCellEditor &editor);

    // We want all these events, typically we skip them
    void OnKeyDown(wxKeyEvent& event);
    void OnChar(wxKeyEvent& event);
    void OnDestroy(wxWindowDestroyEvent& event);

    wxSheet                  *m_sheet;
    // This is a pointer, not a refed editor since editor's destructor
    //  pops event handler which deletes this and we don't want recursion
    wxSheetCellEditorRefData *m_editor;

private:
    DECLARE_DYNAMIC_CLASS(wxSheetCellEditorEvtHandler)
    DECLARE_EVENT_TABLE()
    DECLARE_NO_COPY_CLASS(wxSheetCellEditorEvtHandler)
};

// ----------------------------------------------------------------------------
// wxSheetCellEditor
// ----------------------------------------------------------------------------
// This class is responsible for providing and manipulating the in-place edit
// controls for the wxSheet.  The refdata instances of wxSheetCellEditor
// (actually, instances of derived classes since it is an ABC) can be
// associated with the cell attributes for individual cells, rows, columns, or
// even for the entire sheet (the default).
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellEditor : public wxObject
{
public:
    wxSheetCellEditor( wxSheetCellEditorRefData *editor = NULL );
    wxSheetCellEditor( const wxSheetCellEditor& editor ) { Ref(editor); }

    void Destroy() { UnRef(); }
    bool Ok() const { return m_refData != NULL; }

    // Is this editor ready to use (typically means the control is created)
    bool IsCreated() const;
    // Is the control currently shown
    bool IsShown() const;

    // Get a pointer to the control
    wxWindow* GetControl() const;
    // Destroy the old control (if any) and set the new one
    void SetControl(wxWindow* control);
    // Destroy the control, doesn't fail even if the control isn't created
    //  returns true if the control existed and was destroyed.
    bool DestroyControl();

    // Creates the actual edit control using the parent, id, and
    //  pushing the evtHandler onto it if !NULL.
    void CreateEditor(wxWindow* parent, wxWindowID id,
                      wxEvtHandler* evtHandler, wxSheet* sheet);

    // Size and position the edit control, uses attr.GetAlignment() if attr.Ok().
    void SetSize(const wxRect& rect, const wxSheetCellAttr& attr);
    // get the preferred size of the editor for its contents
    wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                       const wxSheetCoords& coords) const;
    // Show or hide the edit control, use the specified attributes to set
    // colours/fonts for it if attr is Ok.
    void Show(bool show, const wxSheetCellAttr &attr);
    // Draws the part of the cell not occupied by the control: the base class
    // version just fills it with background colour from the attribute
    void PaintBackground(wxSheet& sheet, const wxSheetCellAttr& attr,
                         wxDC& dc, const wxRect& rect,
                         const wxSheetCoords& coords, bool isSelected);

    // Initialize the editor with the colours and font
    void InitEditor(const wxSheetCoords& coords, const wxSheetCellAttr& attr,
                    wxSheet* sheet);
    // Fetch the value from the table and prepare the edit control
    // to begin editing. Set the focus to the edit control.
    void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    // Complete the editing of the current cell. Returns true if the value has
    // changed.  If necessary, the control may be destroyed.
    bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);
    // Reset the value in the control back to its starting value
    void ResetValue();

    // return true to allow the given key to start editing: the base class
    // version only checks that the event has no modifiers. The derived
    // classes are supposed to do "if ( base::IsAcceptedKey() && ... )" in
    // their IsAcceptedKey() implementation, although, of course, it is not a
    // mandatory requirment.
    //
    // NB: if the key is F2 (special), editing will always start and this
    //     method will not be called at all (but StartingKey() will)
    bool IsAcceptedKey(wxKeyEvent& event);
    // If the editor is enabled by pressing keys on the sheet, this will be
    // called to let the editor do something about that first key if desired
    void StartingKey(wxKeyEvent& event);
    // if the editor is enabled by clicking on the cell, this method will be
    // called
    void StartingClick(wxMouseEvent& event);
    // Some types of controls on some platforms may need some help
    // with the Return key.
    void HandleReturn(wxKeyEvent& event);

    // the wxSheetCellEditorEvtHandler passes the key event here first
    // return false to stop default EvtHandler processing
    bool OnKeyDown(wxKeyEvent& event);
    // the wxSheetCellEditorEvtHandler passes the key event here first
    // return false to stop default EvtHandler processing
    bool OnChar(wxKeyEvent& event);

    // interpret renderer parameters: arbitrary string whose interpretation is
    // left to the derived classes
    void SetParameters(const wxString& params);

    // Get the current value of the control as a string
    wxString GetValue() const;
    // Get the value that the editor started with
    wxString GetInitValue() const;

    bool Copy(const wxSheetCellEditor& other);
    wxSheetCellEditor Copy() const { wxSheetCellEditor obj; obj.Copy(*this); return obj; }

    // operators
    bool operator == (const wxSheetCellEditor& obj) const { return m_refData == obj.m_refData; }
    bool operator != (const wxSheetCellEditor& obj) const { return m_refData != obj.m_refData; }
    wxSheetCellEditor& operator = (const wxSheetCellEditor& obj)
    {
        if ( (*this) != obj ) Ref(obj);
        return *this;
    }

    // wxSheet >= 1.1 : Function Clone() renamed to Copy() and NewClone() is now Clone()
    wxSheetCellEditor& operator = (const wxSheetCellEditor* obj)
    {
        wxFAIL_MSG(wxT("wxSheetCellEditor a = &otherEditor? Did you mean to use Copy() instead of Clone()"));
        return *(wxSheetCellEditor*)obj;
    }

    wxSheetCellEditor* Clone() const { return new wxSheetCellEditor(Copy()); }
    DECLARE_DYNAMIC_CLASS(wxSheetCellEditor)
};

WXDLLIMPEXP_DATA_SHEET(extern const wxSheetCellEditor) wxNullSheetCellEditor;

// ----------------------------------------------------------------------------
// wxSheetCellEditorRefData
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_SHEET wxSheetCellEditorRefData : public wxObject, public wxObjectRefData
{
public:
    wxSheetCellEditorRefData() : m_control(NULL) {}
    virtual ~wxSheetCellEditorRefData();

    virtual bool IsCreated() const { return (GetControl() != NULL); }
    virtual bool IsShown() const;

    wxWindow* GetControl() const { return m_control; }
    virtual void SetControl(wxWindow* control);
    virtual bool DestroyControl();

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);
    virtual void SetSize(const wxRect& rect, const wxSheetCellAttr &attr);
    // get the preferred size of the cell for its contents
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               const wxSheetCoords& coords);

    virtual void Show(bool show, const wxSheetCellAttr &attr);
    virtual void PaintBackground(wxSheet& sheet, const wxSheetCellAttr& attr,
                                 wxDC& dc, const wxRect& rect,
                                 const wxSheetCoords& coords, bool isSelected);
    void InitEditor(const wxSheetCoords& coords, const wxSheetCellAttr& attr,
                    wxSheet* sheet);
    virtual void BeginEdit(const wxSheetCoords& WXUNUSED(coords), wxSheet* WXUNUSED(sheet)) {}
    virtual bool EndEdit(const wxSheetCoords& WXUNUSED(coords), wxSheet* WXUNUSED(sheet)) { return false; }
    virtual void ResetValue() {}

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void StartingKey(wxKeyEvent& event) { event.Skip(); }
    virtual bool OnKeyDown(wxKeyEvent& WXUNUSED(event)) { return true; }
    virtual bool OnChar(wxKeyEvent& WXUNUSED(event)) { return true; }
    virtual void StartingClick(wxMouseEvent& WXUNUSED(event)) {}
    virtual void HandleReturn(wxKeyEvent& event) { event.Skip(); }

    virtual void SetParameters(const wxString& WXUNUSED(params)) {}
    virtual wxString GetValue() const { return wxEmptyString; }
    virtual wxString GetInitValue() const { return wxEmptyString; }

    bool Copy(const wxSheetCellEditorRefData& WXUNUSED(other)) { return true; }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellEditorRefData,
                                       wxSheetCellEditorRefData)

    wxWindow* m_control;    // the control we show on screen
};

//-----------------------------------------------------------------------------
// wxSheetCellTextEditorRefData - the editor for string/text data
//-----------------------------------------------------------------------------
#if wxUSE_TEXTCTRL

class WXDLLIMPEXP_SHEET wxSheetCellTextEditorRefData : public wxSheetCellEditorRefData
{
public:
    wxSheetCellTextEditorRefData() : m_maxChars(0) {}

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);
    virtual void SetSize(const wxRect& rect, const wxSheetCellAttr &attr);
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               const wxSheetCoords& coords);

    virtual void PaintBackground(wxSheet& , const wxSheetCellAttr& ,
                                 wxDC& , const wxRect& ,
                                 const wxSheetCoords& , bool ) {}

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);

    virtual void ResetValue();
    virtual void StartingKey(wxKeyEvent& event);
    virtual void HandleReturn(wxKeyEvent& event);
    virtual bool OnChar(wxKeyEvent& event);

    // parameters string format is "max_width"
    virtual void SetParameters(const wxString& params);

    virtual wxString GetValue() const;
    virtual wxString GetInitValue() const { return m_startValue; }

    bool Copy(const wxSheetCellTextEditorRefData& other)
        { return wxSheetCellEditorRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellTextEditorRefData,
                                       wxSheetCellEditorRefData)

    // parts of our virtual functions reused by the derived classes
    void DoBeginEdit(const wxString& startValue);
    void DoResetValue(const wxString& startValue);

    wxTextCtrl *GetTextCtrl() const { return wxDynamicCast(GetControl(), wxTextCtrl); }

    size_t   m_maxChars;        // max number of chars allowed
    wxString m_startValue;
    int      m_maxLength;       // largest length in the textctrl yet
};

// ----------------------------------------------------------------------------
// wxSheetCellAutoWrapStringEditorRefData
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellAutoWrapStringEditorRefData : public wxSheetCellTextEditorRefData
{
public:
    wxSheetCellAutoWrapStringEditorRefData() : wxSheetCellTextEditorRefData() { }

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);

    bool Copy(const wxSheetCellAutoWrapStringEditorRefData& other)
        { return wxSheetCellTextEditorRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellAutoWrapStringEditorRefData,
                                       wxSheetCellEditorRefData)
};

#endif // wxUSE_TEXTCTRL

//-----------------------------------------------------------------------------
// wxSheetCellNumberEditorRefData : the editor for numeric (long) data
//-----------------------------------------------------------------------------
#if defined(wxUSE_TEXTCTRL) && defined(wxUSE_SPINCTRL)

class WXDLLIMPEXP_SHEET wxSheetCellNumberEditorRefData : public wxSheetCellTextEditorRefData
{
public:
    // specify the range - if min == max == -1, no range checking is done
    wxSheetCellNumberEditorRefData(int min = -1, int max = -1)
        : m_min(min), m_max(max), m_startValue(0) {}

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               const wxSheetCoords& coords);

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);

    virtual void ResetValue();
    virtual void StartingKey(wxKeyEvent& event);

    // parameters string format is "min,max"
    virtual void SetParameters(const wxString& params);

    virtual wxString GetValue() const;
    int GetValueInt() const;
    virtual wxString GetInitValue() const;

    // if HasRange(), we use wxSpinCtrl - otherwise wxTextCtrl
    bool HasRange() const { return m_min != m_max; }

    bool Copy(const wxSheetCellNumberEditorRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellNumberEditorRefData,
                                       wxSheetCellEditorRefData)

    wxSpinCtrl *GetSpinCtrl() const { return wxDynamicCast(GetControl(), wxSpinCtrl); }

    int m_min,
        m_max;

    long m_startValue;
};
#endif //defined(wxUSE_TEXTCTRL) && defined(wxUSE_SPINCTRL)

//-----------------------------------------------------------------------------
// wxSheetCellFloatEditorRefData: the editor for floating point numbers (double) data
//-----------------------------------------------------------------------------
#if wxUSE_TEXTCTRL

class WXDLLIMPEXP_SHEET wxSheetCellFloatEditorRefData : public wxSheetCellTextEditorRefData
{
public:
    wxSheetCellFloatEditorRefData(int width = -1, int precision = -1)
        : m_width(width), m_precision(precision), m_startValue(0) {}

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);

    virtual void ResetValue();
    virtual void StartingKey(wxKeyEvent& event);

    // parameters string format is "width,precision"
    virtual void SetParameters(const wxString& params);

    // string representation of a double value
    virtual wxString GetInitValue() const { return GetStringValue(m_startValue); }
    virtual wxString GetStringValue(double value) const;

    bool Copy(const wxSheetCellFloatEditorRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellFloatEditorRefData,
                                       wxSheetCellEditorRefData)

    int m_width,
        m_precision;
    double m_startValue;
};

#endif // wxUSE_TEXTCTRL

//-----------------------------------------------------------------------------
// wxSheetCellBoolEditorRefData: the editor for boolean data
//-----------------------------------------------------------------------------
#if wxUSE_CHECKBOX

class WXDLLIMPEXP_SHEET wxSheetCellBoolEditorRefData : public wxSheetCellEditorRefData
{
public:
    wxSheetCellBoolEditorRefData() : m_startValue(false) {}

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);

    virtual void SetSize(const wxRect& rect, const wxSheetCellAttr &attr);
    virtual void Show(bool show, const wxSheetCellAttr &attr);

    virtual bool IsAcceptedKey(wxKeyEvent& event);
    virtual void StartingKey(wxKeyEvent& event);
    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);

    virtual void ResetValue();
    virtual void StartingClick(wxMouseEvent& event);

    virtual wxString GetValue() const;

    bool Copy(const wxSheetCellBoolEditorRefData& other)
        { return wxSheetCellEditorRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellBoolEditorRefData,
                                       wxSheetCellEditorRefData)

    wxCheckBox *GetCheckBox() const { return (wxCheckBox *)GetControl(); }

    bool m_startValue;
};

#endif // wxUSE_CHECKBOX

//-----------------------------------------------------------------------------
// wxSheetCellChoiceEditorRefData: choose from a list of strings
//-----------------------------------------------------------------------------
#if wxUSE_COMBOBOX

class WXDLLIMPEXP_SHEET wxSheetCellChoiceEditorRefData : public wxSheetCellEditorRefData
{
public:
    // if !allowOthers, user can't type a string not in choices array
    wxSheetCellChoiceEditorRefData(size_t count = 0,
                                   const wxString choices[] = NULL,
                                   bool allowOthers = false);
    wxSheetCellChoiceEditorRefData(const wxArrayString& choices,
                                   bool allowOthers = false);

    virtual void CreateEditor(wxWindow* parent, wxWindowID id,
                              wxEvtHandler* evtHandler, wxSheet* sheet);

    virtual void PaintBackground(wxSheet& sheet, const wxSheetCellAttr& attr,
                                 wxDC& dc, const wxRect& rect,
                                 const wxSheetCoords& coords, bool isSelected);

    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);

    virtual void ResetValue();

    // parameters string format is "item1[,item2[...,itemN]]"
    virtual void SetParameters(const wxString& params);

    virtual wxString GetValue() const;

    bool Copy(const wxSheetCellChoiceEditorRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellChoiceEditorRefData,
                                       wxSheetCellEditorRefData)

    wxComboBox *GetComboBox() const { return (wxComboBox *)GetControl(); }
    wxString        m_startValue;
    wxArrayString   m_choices;
    bool            m_allowOthers;
};

// ----------------------------------------------------------------------------
// wxSheetCellEnumEditorRefData
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellEnumEditorRefData : public wxSheetCellChoiceEditorRefData
{
public:
    wxSheetCellEnumEditorRefData( const wxString& choices = wxEmptyString );

    virtual bool EndEdit(const wxSheetCoords& coords, wxSheet* sheet);
    virtual void BeginEdit(const wxSheetCoords& coords, wxSheet* sheet);

    bool Copy(const wxSheetCellEnumEditorRefData& other)
        { return wxSheetCellChoiceEditorRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellEnumEditorRefData,
                                       wxSheetCellEditorRefData)

    long int m_startint;
};

#endif // wxUSE_COMBOBOX

#endif //__WX_SHEETEDT_H__
