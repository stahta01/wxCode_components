/***************************************************************
 * Name:      wxledpanel.h
 * Purpose:   Defines Class wxLEDPanel
 * Author:    Christian Gr�fe (info@mcs-soft.de)
 * Created:   2007-02-28
 * Copyright: Christian Gr�fe (www.mcs-soft.de)
 * License:	  wxWindows licence
 **************************************************************/
#ifndef WXLEDPANEL_H
#define WXLEDPANEL_H

#include <wx/wx.h>

#include "advancedmatrixobject.h"
#include "wxledfont.h"

enum wxLEDColour
{
	wxLED_COLOUR_RED = 1,
	wxLED_COLOUR_GREEN = 2,
	wxLED_COLOUR_BLUE = 3,
	wxLED_COLOUR_YELLOW = 4,
	wxLED_COLOUR_MAGENTA = 5,
	wxLED_COLOUR_CYAN = 6,
	wxLED_COLOUR_GREY = 7
};

enum wxLEDScrollDirection
{
	wxLED_SCROLL_NONE = 0,
	wxLED_SCROLL_UP = 1,
	wxLED_SCROLL_DOWN = 2,
	wxLED_SCROLL_LEFT = 3,
	wxLED_SCROLL_RIGHT = 4
};

enum wxLEDTextAlign
{
	wxLED_TEXTALIGN_LEFT = 0,
	wxLED_TEXTALIGN_RIGHT = 1,
	wxLED_TEXTALIGN_CENTER = 2
};

enum wxLEDTextVAlign
{
	wxLED_TEXTVALIGN_TOP = 0,
	wxLED_TEXTVALIGN_BOTTOM = 1,
	wxLED_TEXTVALIGN_CENTER = 2
};

class wxLEDPanel : public wxControl
{
	DECLARE_EVENT_TABLE()
	public:
		// Konstruktor
		wxLEDPanel();
		wxLEDPanel(wxWindow* parent, wxWindowID id,
					const wxSize& pointsize, const wxSize& fieldsize, int padding=0,
					const wxPoint& pos = wxDefaultPosition,
					long style = wxNO_BORDER,
					const wxValidator& validator = wxDefaultValidator);

		virtual ~wxLEDPanel() {}

		// Element erzeugen
		bool Create(wxWindow* parent, wxWindowID id,
					const wxSize& pointsize, const wxSize& fieldsize, int padding=0,
					const wxPoint& pos = wxDefaultPosition,
					long style = wxNO_BORDER,
					const wxValidator& validator = wxDefaultValidator);

		// Gr��e des Elements
		wxSize DoGetBestSize() const;

		// Daten Setzen
		bool SetDataAt(int x, int y, char data) {return m_field.SetDataAt(x,y,data);}
		bool SetDataAt(const wxPoint& punkt, char data) {return m_field.SetDataAt(punkt.x,punkt.y,data);}
		bool SetDatesAt(int x, int y, const MatrixObject& to) {return m_field.SetDatesAt(x,y,to);}
		bool SetDatesAt(const wxPoint& punkt, const MatrixObject& to) {return m_field.SetDatesAt(punkt.x,punkt.y,to);}

		// Feld leeren
		void Clear() {m_field.Clear();}

		// Inhalt bewegen
		void ShiftLeft();
		void ShiftRight();
		void ShiftUp();
		void ShiftDown();

		// Gr��enangaben
		wxSize GetFieldsize() const {return m_field.GetSize();}
		wxSize GetPointsize() const {return m_pointsize;}

		// Die Farbe einstellen
		void SetLEDColour(wxLEDColour colourID);
		const wxColour& GetLEDColour() const {return s_colour[m_activ_colour_id];}

		// Geschwindigkeit setzen
		void SetScrollSpeed(int speed);
		int GetScrollSpeed() const {return m_scrollspeed;}

		// Scrollrichtung
		void SetScrollDirection(wxLEDScrollDirection d);
		wxLEDScrollDirection GetScrollDirection() const {return m_scrolldirection;}

		// Draw Invertet (default behavior is false)
		void DrawInvertet(bool invert=true) {m_invert=invert;}

		// Show inactiv LEDs? (default behavior is true)
		void ShowInactivLEDs(bool show_inactivs=true) {m_show_inactivs=show_inactivs;}

		// Text Alignment if wxLED_SCROLL_NONE
		void SetTextAlign(int a);	// a -> wxAlignment e.g. wxALIGN_TOP|wxALIGN_RIGHT
		int GetTextAlign() const {return m_textalign;}

		// Text setzen
		void SetText(const wxString& text, int align=-1);	// align e.g. "wxLEFT|wxTOP", align=-1 -> Use corrent align
		const wxString& GetText() const {return m_text;}

		// TextPadding
		void SetTextPaddingLeft(int padLeft);
		void SetTextPaddingRight(int padRight);
		int GetTextPaddingLeft() const {return m_padLeft;}
		int GetTextPaddingRight() const {return m_padRight;}

	protected:
		// Drawing
		void DrawField(wxDC& dc);
		void OnEraseBackground(wxEraseEvent& event) {}
		void OnPaint(wxPaintEvent &event);

		// Das Angezeigte feld
		AdvancedMatrixObject m_field;

		// Control-Properties
		wxSize m_pointsize;
        int m_padding;
        int m_textalign;
        int m_padLeft;
        int m_padRight;
        wxLEDColour m_activ_colour_id;
        bool m_invert;
        bool m_show_inactivs;

        // Scroll-Properties
        int m_scrollspeed;
		wxLEDScrollDirection m_scrolldirection;
        wxTimer m_scrollTimer;
        void OnScrollTimer(wxTimerEvent& event);

		// Bitmaps with "LED on" and "LED off"
        wxBitmap m_bmp_led_on;
        wxBitmap m_bmp_led_off;

        // the text
        wxString m_text;
        MatrixObject m_text_mo;
        wxPoint m_text_pos;
        void ResetTextPos();
        wxLEDFont m_font;

        static const wxColour s_colour[7];
        static const wxColour s_colour_dark[7];
        static const wxColour s_colour_verydark[7];
        static const wxColour s_colour_light[7];
};



#endif // WXLEDPANEL_H
