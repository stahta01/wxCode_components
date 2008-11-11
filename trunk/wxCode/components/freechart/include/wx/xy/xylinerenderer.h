
/////////////////////////////////////////////////////////////////////////////
// Name:	xylinerenderer.h
// Purpose:
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef XYLINERENDERER_H_
#define XYLINERENDERER_H_

#include <wx/xy/xyrenderer.h>
#include <wx/art.h>

/**
 * Renderer for drawing lines from xy datasets. Can also draw symbols at line points.
 */
class XYLineRenderer : public XYRenderer
{
public:
	/**
	 * Constructs new line renderer.
	 * @param drawSymbols specifies whether to draw symbols at line points
	 * @param defaultWidth specifies default width for lines
	 * @param defaultStyle specifies default style for lines
	 */
	XYLineRenderer(bool drawSymbols = false, int defaultWidth = 1, int defaultStyle = wxSOLID);
	virtual ~XYLineRenderer();

	virtual void Draw(wxDC &dc, wxRect rc, Axis *horizAxis, Axis *vertAxis, XYDataset *dataset);

	void SetSeriePen(int serie, wxPen *pen);

	void SetSerieSymbol(int serie, Symbol *symbol);

	wxPen *GetSeriePen(int serie);

	wxColour GetSerieColor(int serie);

	Symbol *GetSerieSymbol(int serie);

private:
	void DrawSegment(wxDC &dc, int serie, wxCoord x0, wxCoord y0, wxCoord x1, wxCoord y1);

	bool m_drawSymbols;

	PenMap m_seriePens;
	SymbolMap m_serieSymbols;

	int m_defaultPenWidth;
	int m_defaultPenStyle;
};

#endif /*XYLINERENDERER_H_*/
