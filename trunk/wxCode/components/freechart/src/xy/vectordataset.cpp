/////////////////////////////////////////////////////////////////////////////
// Name:	vectordataset.cpp
// Purpose:
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// RCS-ID:	$Id: wxAdvTable.h,v 1.3 2008/11/07 16:42:58 moskvichev Exp $
// Copyright:	(c) 2008-2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/xy/vectordataset.h>

VectorDataset::VectorDataset()
{
}

VectorDataset::~VectorDataset()
{
}

double VectorDataset::GetX(int index)
{
	return index + 1;
}

double VectorDataset::GetY(int index)
{
	return values[index];
}

int VectorDataset::GetCount()
{
	return (int) values.size();
}
