
/////////////////////////////////////////////////////////////////////////////
// Name:	multiplot.cpp
// Purpose:
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/multiplot.h>

#define FOREACH_SUBPLOT(index, subPlots) \
	for (int index = 0; n < subPlots.GetSize(); n++)

MultiPlot::MultiPlot(int rows, int cols, wxCoord horizGap, wxCoord vertGap)
{
	m_rows = rows;
	m_cols = cols;
	m_horizGap = horizGap;
	m_vertGap = vertGap;
}

MultiPlot::~MultiPlot()
{
}

void MultiPlot::PlotNeedRedraw(Plot *plot)
{
	FirePlotNeedRedraw();
}

bool MultiPlot::HasData()
{
	return (m_subPlots.GetSize() != 0);
}

void MultiPlot::DrawData(wxDC &dc, wxRect rc)
{
	wxCHECK_RET(m_rows != 0 || m_cols != 0, wxT("row and column count = 0"));

	int rows = m_rows;
	int cols = m_cols;
	bool vertical = false;

	if (cols == 0) {
		cols = 1;

		int row = 0;
		FOREACH_SUBPLOT(n, m_subPlots) {
			if (row >= rows) {
				row = 0;
				cols++;
			}
			row++;
		}

		vertical = true;
	}
	if (rows == 0) {
		rows = 1;

		int col = 0;
		FOREACH_SUBPLOT(n, m_subPlots) {
			if (col >= cols) {
				col = 0;

				rows++;
			}
			col++;
		}
	}

	wxCoord subWidth = (rc.width - (cols - 1) * m_horizGap) / cols;
	wxCoord subHeight = (rc.height - (rows - 1) * m_vertGap) / rows;

	wxCoord x = rc.x;
	wxCoord y = rc.y;

	int row = 0;
	int col = 0;
	FOREACH_SUBPLOT(n, m_subPlots) {
		// TODO untested!
		if (vertical) {
			if (row >= rows) {
				row = 0;
				y = rc.y;
				x += subWidth + m_horizGap;

				col++;
				if (col >= cols)
					break;
			}
		}
		else {
			if (col >= cols) {
				col = 0;
				x = rc.x;
				y += subHeight + m_vertGap;

				row++;
				if (row >= rows)
					break;
			}
		}

		wxRect subRc(x, y, subWidth, subHeight);

		m_subPlots[n]->Draw(dc, subRc);

		if (vertical) {
			row++;
			y += subHeight + m_vertGap;
		}
		else {
			col++;
			x += subWidth + m_horizGap;
		}
	}
}
