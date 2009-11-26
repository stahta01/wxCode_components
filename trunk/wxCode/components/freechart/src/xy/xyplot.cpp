/////////////////////////////////////////////////////////////////////////////
// Name:	xyplot.cpp
// Purpose: xy plot implementation
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008-2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/xy/xyplot.h>

XYPlot::XYPlot()
{
}

bool XYPlot::AcceptAxis(Axis *axis)
{
	// TODO temporary commented
	//return (wxDynamicCase(axis, NumberAxis) != NULL);
	return true;
}

bool XYPlot::AcceptDataset(Dataset *dataset)
{
	return (wxDynamicCast(dataset, XYDataset) != NULL);
}

void XYPlot::DrawDatasets(wxDC &dc, wxRect rc)
{
	for (int nData = 0; nData < GetDatasetCount(); nData++) {
		XYDataset *dataset = (XYDataset *) GetDataset(nData);
		DrawXYDataset(dc, rc, dataset);
	}
}

void XYPlot::DrawXYDataset(wxDC &dc, wxRect rc, XYDataset *dataset)
{
	XYRenderer *renderer = dataset->GetRenderer();
	wxCHECK_RET(renderer != NULL, wxT("no renderer for data"));

	Axis *vertAxis = GetDatasetVerticalAxis(dataset);
	Axis *horizAxis = GetDatasetHorizontalAxis(dataset);

	wxCHECK_RET(vertAxis != NULL, wxT("no axis for data"));
	wxCHECK_RET(horizAxis != NULL, wxT("no axis for data"));

	renderer->Draw(dc, rc, horizAxis, vertAxis, dataset);
}

