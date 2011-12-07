///////////////////////////////////////////////////////////////////////////////
// Name:        sheetren.h
// Purpose:     wxSheet controls
// Author:      John Labenski, Paul Gammans, Roger Gammans
// Modified by: John Labenski
// Created:     11/04/2001
// RCS-ID:      $Id: sheetren.h,v 1.5 2007/12/12 05:22:38 jrl1 Exp $
// Copyright:   (c) John Labenski, The Computer Surgery (paul@compsurg.co.uk)
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#ifndef __WX_SHEETREN_H__
#define __WX_SHEETREN_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "sheetren.h"
#endif

#include "wx/sheet/sheetdef.h"
#include <wx/datetime.h>
#include <wx/bitmap.h>

class WXDLLIMPEXP_FWD_SHEET wxSheetCellRendererRefData;

// ----------------------------------------------------------------------------
// wxSheetCellRenderer:
// ----------------------------------------------------------------------------
// This class is responsible for actually drawing the cell in the sheet.
// You may pass it to a wxSheetCellAttr to change the format of one given cell
// or to wxSheet::SetDefaultRenderer() to change the view of all default cells.
//
// Rendering is done by the wxObject::m_refData which must be derived from
// wxSheetCellRendererRefData. All the functions are passed directly to the
// ref counted renderer.
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_SHEET wxSheetCellRenderer : public wxObject
{
public:
    wxSheetCellRenderer(wxSheetCellRendererRefData *renderer = NULL);
    wxSheetCellRenderer( const wxSheetCellRenderer& renderer ) { Ref(renderer); }

    void Destroy() { UnRef(); }

    bool Ok() const { return m_refData != NULL; }

    // draw the given cell on the provided DC inside the given rectangle
    // using the style specified by the attribute and the default or selected
    // state corresponding to the isSelected value.
    void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
              wxDC& dc, const wxRect& rect,
              const wxSheetCoords& coords, bool isSelected);

    // get the preferred size of the cell for its contents
    wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                       wxDC& dc, const wxSheetCoords& coords);

    // interpret renderer parameters: arbitrary string whose interpretation is
    // left to the derived classes
    void SetParameters(const wxString& params);

    bool Copy(const wxSheetCellRenderer& other);
    wxSheetCellRenderer  Copy() const  { wxSheetCellRenderer obj; obj.Copy(*this); return obj; }

    // operators
    bool operator == (const wxSheetCellRenderer& obj) const { return m_refData == obj.m_refData; }
    bool operator != (const wxSheetCellRenderer& obj) const { return m_refData != obj.m_refData; }
    wxSheetCellRenderer& operator = (const wxSheetCellRenderer& obj)
    {
        if ( (*this) != obj ) Ref(obj);
        return *this;
    }

    // wxSheet >= 1.1 : Function Clone() renamed to Copy() and NewClone() is now Clone()
    wxSheetCellRenderer& operator = (const wxSheetCellRenderer* obj)
    {
        wxFAIL_MSG(wxT("wxSheetCellRenderer a = &otherRenderer? Did you mean to use Copy() instead of Clone()"));
        return *(wxSheetCellRenderer*)obj;
    }

    wxSheetCellRenderer* Clone() const { return new wxSheetCellRenderer(Copy()); }
    DECLARE_DYNAMIC_CLASS(wxSheetCellRenderer)
};

WXDLLIMPEXP_DATA_SHEET(extern const wxSheetCellRenderer) wxNullSheetCellRenderer;

// ----------------------------------------------------------------------------
// wxSheetCellRendererRefData - base class for rendering a cell
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_SHEET wxSheetCellRendererRefData : public wxObjectRefData, public wxObject
{
public:
    wxSheetCellRendererRefData() {}

    // Draw whatever is necessary onto the wxDC in the wxRect pixel coords.
    // This function has a default implementation which will
    // prepare the DC using the given attribute: it will draw the cell rectangle
    // with the bg colour from attr and set the dc's text colour and font
    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Get the preferred size of the cell for its contents
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, const wxSheetCoords& coords);

    // Get a string representation of the cell value
    virtual wxString GetString(wxSheet& sheet, const wxSheetCoords& coords);

    // Interpret renderer parameters: arbitrary string whose interpretation is
    // left to the derived classes
    virtual void SetParameters(const wxString& WXUNUSED(params)) {}

    // Always define Copy for DECLARE_SHEETOBJREFDATA_COPY_CLASS
    bool Copy(const wxSheetCellRendererRefData& WXUNUSED(other)) { return true; }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellRendererRefData,
                                       wxSheetCellRendererRefData)
};

// ----------------------------------------------------------------------------
// wxSheetCellStringRendererRefData
// ----------------------------------------------------------------------------

// the default renderer for the cells containing string data
class WXDLLIMPEXP_SHEET wxSheetCellStringRendererRefData : public wxSheetCellRendererRefData
{
public:
    wxSheetCellStringRendererRefData();

    // Sraw the string
    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Return the string extent
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, const wxSheetCoords& coords);

    void DoDraw(wxSheet& sheet, const wxSheetCellAttr& attr,
                wxDC& dc, const wxRect& rect,
                const wxSheetCoords& coords, bool isSelected);

    // Set the text colours before drawing
    void SetTextColoursAndFont(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, bool isSelected);

    // Calc the string extent for given string/font
    wxSize DoGetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                         wxDC& dc, const wxString& text);

    bool Copy(const wxSheetCellStringRendererRefData& other)
        { return wxSheetCellRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellStringRendererRefData,
                                       wxSheetCellRendererRefData)
};

// ----------------------------------------------------------------------------
// wxSheetCellAutoWrapStringRendererRefData
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellAutoWrapStringRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellAutoWrapStringRendererRefData() : wxSheetCellStringRendererRefData() { }

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, const wxSheetCoords& coords);

    wxArrayString GetTextLines( wxSheet& sheet, wxDC& dc, const wxSheetCellAttr& attr,
                                const wxRect& rect, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellAutoWrapStringRendererRefData& other)
        { return wxSheetCellStringRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellAutoWrapStringRendererRefData,
                                       wxSheetCellRendererRefData)
};

// ----------------------------------------------------------------------------
// wxSheetCellNumberRendererRefData
// ----------------------------------------------------------------------------

// the default renderer for the cells containing numeric (long) data
class WXDLLIMPEXP_SHEET wxSheetCellNumberRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellNumberRendererRefData() {}

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    virtual wxString GetString(wxSheet& sheet, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellNumberRendererRefData& other)
        { return wxSheetCellStringRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellNumberRendererRefData,
                                       wxSheetCellRendererRefData)
};

// ----------------------------------------------------------------------------
// wxSheetCellFloatRendererRefData
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_SHEET wxSheetCellFloatRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellFloatRendererRefData(int width = -1, int precision = -1)
        : m_width(width), m_precision(precision) { }

    // Get/change formatting parameters "%[width].[precision]f"
    int  GetWidth() const            { return m_width; }
    void SetWidth(int width)         { m_width = width; m_format.clear(); }
    int  GetPrecision() const        { return m_precision; }
    void SetPrecision(int precision) { m_precision = precision; m_format.clear(); }

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Parameters string format is "width[,precision]"
    virtual void SetParameters(const wxString& params);

    virtual wxString GetString(wxSheet& sheet, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellFloatRendererRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellFloatRendererRefData,
                                       wxSheetCellRendererRefData)

protected:
    int m_width,
        m_precision;

    wxString m_format;
};

// ----------------------------------------------------------------------------
// wxSheetCellBitmapRendererRefData
// ----------------------------------------------------------------------------

enum wxSHEET_BMPREN_Type
{
    // The position of the bitmap relative to the text (if any)
    wxSHEET_BMPREN_BMPLEFT   = 0x000000, // bmp to left of text
    wxSHEET_BMPREN_BMPRIGHT  = 0x010000, // bmp to right of text
    wxSHEET_BMPREN_BMPABOVE  = 0x020000, // bmp above text
    wxSHEET_BMPREN_BMPBELOW  = 0x040000, // bmp below text
    wxSHEET_BMPREN_BMPEXPAND = 0x080000  // bmp aligned within extra space
};

class WXDLLIMPEXP_SHEET wxSheetCellBitmapRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellBitmapRendererRefData(const wxBitmap& bitmap = wxNullBitmap,
                                     int align = 0) : m_bitmap(bitmap), m_align(align) {}

    // Draw a the bitmap
    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Return the bitmap size
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, const wxSheetCoords& coords);

    // Get/Set the bitmap to draw
    wxBitmap GetBitmap() const { return m_bitmap; }
    void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }
    // Get/Set the bitmap alignment, wxALIGN_XXX | wxSHEET_BMPREN_Type | wxGROW
    //   The wxSHEET_BMPREN_Type determines the relative positions of bmp and text
    //   The wxALIGN_XXX sets the alignment of the bitmap in its area of the cell
    //   wxGROW will expand or shrink bitmap instead of clipping it
    //   The cell attribute's alignment sets the alignment of the text in its area of the cell
    int GetAlignment() const { return m_align; }
    void SetAlignment(int align) { m_align = align; }

    // Takes the input bounding rect, and the alignment for the bitmap and text
    //   The size of the bmpRect and textRect must match the size of the items
    //     but the position is unused.
    //   Fills the bmpRect and textRect with
    //     suitable values to best fit within the given rect
    void AlignBmpTextRects(const wxRect& rect, int bmp_align, int text_align,
                           int margin, wxRect& bmpRect, wxRect& textRect);

    bool Copy(const wxSheetCellBitmapRendererRefData& other)
        { SetBitmap(other.GetBitmap()); return wxSheetCellStringRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellBitmapRendererRefData,
                                       wxSheetCellRendererRefData)

    wxBitmap m_bitmap;
    int m_align;
};

// ----------------------------------------------------------------------------
// wxSheetCellBoolRendererRefData
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_SHEET wxSheetCellBoolRendererRefData : public wxSheetCellRendererRefData
{
public:
    wxSheetCellBoolRendererRefData() {}

    // Draw a check mark or nothing
    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Return the checkmark size
    virtual wxSize GetBestSize(wxSheet& sheet, const wxSheetCellAttr& attr,
                               wxDC& dc, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellBoolRendererRefData& other)
        { return wxSheetCellRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellBoolRendererRefData,
                                       wxSheetCellRendererRefData)

protected:
    static wxSize ms_sizeCheckMark;
};

// ----------------------------------------------------------------------------
// wxSheetCellDateTimeRendererRefData
// ----------------------------------------------------------------------------
#if wxUSE_DATETIME

// the default renderer for the cells containing Time and dates..
class WXDLLIMPEXP_SHEET wxSheetCellDateTimeRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellDateTimeRendererRefData(wxString outFormat = wxDefaultDateTimeFormat, //_T("%c"),
                                       wxString inFormat  = wxDefaultDateTimeFormat)
        : m_outFormat(outFormat), m_inFormat(inFormat),
          m_dateTime(wxDefaultDateTime), m_tz(wxDateTime::Local) {}

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Parameters string format is "width[,precision]"
    virtual void SetParameters(const wxString& params);

    wxString GetInFormat() const  { return m_inFormat; }
    wxString GetOutFormat() const { return m_outFormat; }
    void SetInFormat(const wxString& inFormat)   { m_inFormat = inFormat; }
    void SetOutFormat(const wxString& outFormat) { m_outFormat = outFormat; }

    virtual wxString GetString(wxSheet& sheet, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellDateTimeRendererRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellDateTimeRendererRefData,
                                       wxSheetCellRendererRefData)

protected:
    wxString m_outFormat;
    wxString m_inFormat;
    wxDateTime m_dateTime;
    wxDateTime::TimeZone m_tz;
};

#endif // wxUSE_DATETIME

// ----------------------------------------------------------------------------
// wxSheetCellEnumRendererRefData - renderers Time and dates..
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_SHEET wxSheetCellEnumRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellEnumRendererRefData( const wxString& choices = wxEmptyString );

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    // Parameters string format is "item1[,item2[...,itemN]]"
    virtual void SetParameters(const wxString& params);

    wxArrayString& GetChoices() { return m_choices; }
    virtual wxString GetString(wxSheet& sheet, const wxSheetCoords& coords);

    bool Copy(const wxSheetCellEnumRendererRefData& other);
    DECLARE_SHEETOBJREFDATA_COPY_CLASS(wxSheetCellEnumRendererRefData,
                                       wxSheetCellRendererRefData)
protected:
    wxArrayString m_choices;
};

// ----------------------------------------------------------------------------
// wxSheetCellRolColLabelRendererRefData
// ----------------------------------------------------------------------------

// the default renderer for the cells containing string data
class WXDLLIMPEXP_SHEET wxSheetCellRolColLabelRendererRefData : public wxSheetCellStringRendererRefData
{
public:
    wxSheetCellRolColLabelRendererRefData() {}

    virtual void Draw(wxSheet& sheet, const wxSheetCellAttr& attr,
                      wxDC& dc, const wxRect& rect,
                      const wxSheetCoords& coords, bool isSelected);

    bool Copy(const wxSheetCellRolColLabelRendererRefData& other)
        { return wxSheetCellStringRendererRefData::Copy(other); }
    DECLARE_SHEETOBJREFDATA_COPY_CLASS( wxSheetCellRolColLabelRendererRefData,
                                        wxSheetCellRendererRefData)
};

#endif //__WX_SHEETREN_H__
