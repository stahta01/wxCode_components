/////////////////////////////////////////////////////////////////////////////
// Name:        checkedlistctrl.h
// Purpose:     wxCheckedListCtrl
// Author:      Uknown ? (find at http://wiki.wxwidgets.org/wiki.pl?WxListCtrl)
// Created:     2005/06/29
// RCS-ID:      $Id$
// Copyright:   (c) 2005 Francesco Montorsi
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////


#ifndef _WX_CHECKEDLISTCTRL_H_
#define _WX_CHECKEDLISTCTRL_H_

// optimization for GCC
#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "checkedlistctrl.h"
#endif

// wxWidgets headers
#include "wx/webupdatedef.h"		// for the WXDLLIMPEXP_WEBUPDATE macro
#include <wx/listctrl.h>
#include <wx/imaglist.h>

#if wxWU_USE_CHECKEDLISTCTRL

// image indexes (used internally by wxCheckedListCtrl)
#define wxCLC_UNCHECKED_IMGIDX				0		// unchecked & enabled
#define wxCLC_CHECKED_IMGIDX				1		// checked & enabled
#define wxCLC_DISABLED_UNCHECKED_IMGIDX		2		// unchecked & disabled
#define wxCLC_DISABLED_CHECKED_IMGIDX		3		// checked & disabled

// additional state flags (wx's defines should end at 0x0100; see listbase.h)
#define wxLIST_STATE_CHECKED				0x010000
#define wxLIST_STATE_ENABLED				0x100000

// additional wxCheckedListCtrl style flags
// (wx's defines should at 0x8000; see listbase.h)
#define wxCLC_CHECK_WHEN_SELECTING			0x10000


//! This is the class which performs all transactions with the server.
//! It uses the wxSocket facilities.
class WXDLLIMPEXP_WEBUPDATE wxCheckedListCtrl : public wxListCtrl
{
protected:
	
	// we have to keep a different array to keep track of the additional
	// states we support....
	wxArrayInt m_stateList;

	// our set of checkbox images...
	wxImageList m_imageList;

public:
	wxCheckedListCtrl(wxWindow *parent, wxWindowID id = -1, 
						const wxPoint& pt = wxDefaultPosition,
						const wxSize& sz = wxDefaultSize, 
						long style = wxCLC_CHECK_WHEN_SELECTING);
	virtual ~wxCheckedListCtrl() {}


public:			// utilities

	// core overloads (i.e. the most generic overloads)
	bool GetItem(wxListItem& info) const;
	bool SetItem(wxListItem& info);
    long InsertItem(wxListItem& info);
	bool DeleteItem(long item);	
	bool DeleteAllItems()
		{ m_stateList.Clear(); return wxListCtrl::DeleteAllItems(); }

	bool SortItems(wxListCtrlCompare, long)
		{ wxASSERT_MSG(0, wxT("Not implemented yet ! sorry... ")); return FALSE; }
	
	// shortcuts to the SetItemState function
	void Check(long item, bool checked);
	void Enable(long item, bool enable);
	void CheckAll(bool checked = true);
	void EnableAll(bool enable = true);

	// this needs to be redeclared otherwise it's hidden by our other Enable() function.
	// However you should use #EnableAll instead of this function if you want to get
	// good graphics (try to understand)
	virtual bool Enable(bool enable = true)
		{ return wxListCtrl::Enable(enable); }

	// shortcuts to the GetItemState function
	bool IsChecked(long item) const
		{ return GetItemState(item, wxLIST_STATE_CHECKED) != 0; }
	bool IsEnabled(long item) const
		{ return GetItemState(item, wxLIST_STATE_ENABLED) != 0; }

	// this needs to be redeclared otherwise it's hidden by our other IsEnabled() function.
	bool IsEnabled() const
		{ return wxWindow::IsEnabled(); }

	// we overload these so we are sure they will use our
	// #GetItem and #SetItem functions...
	bool SetItemState(long item, long state, long stateMask);
	int GetItemState(long item, long stateMask) const;
    long InsertItem( long index, const wxString& label, int imageIndex = -1);
	long SetItem(long index, int col, const wxString& label, int imageId = -1);

	// the image associated with an element is already in used by wxCheckedListCtrl
	// itself to show the checkbox and it cannot be handled by the user !
	bool SetItemImage(long, int)
		{ wxASSERT_MSG(0, wxT("This function cannot be used with wxCheckedListCtrl !")); return FALSE; }

protected:		// event handlers

	void OnMouseEvent(wxMouseEvent& event);

protected:		// internal utilities

	static int GetItemImageFromAdditionalState(int addstate);
	static int GetAndRemoveAdditionalState(long *state, int statemask);
	wxColour GetBgColourFromAdditionalState(int additionalstate);

private:
	DECLARE_CLASS(wxCheckedListCtrl)
	DECLARE_EVENT_TABLE()
};


#endif	// wxWU_USE_CHECKEDLISTCTRL

#endif	// _WX_CHECKEDLISTCTRL_H_

