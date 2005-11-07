/////////////////////////////////////////////////////////////////////////////
// Name:        RelationPropertiesDlg.cpp
// Purpose:     
// Author:      Matías Szeftel
// Modified by: 
// Created:     Fri 09 Feb 2007 00:58:17 ART
// RCS-ID:      
// Copyright:   (c) 2006 Matías Szeftel <mszeftel@yahoo.com.ar>
// Licence:     GPL (GNU Public License)
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), Fri 09 Feb 2007 00:58:17 ART

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "RelationPropertiesDlg.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "../include/RelationPropertiesDlg.h"

////@begin XPM images
////@end XPM images

/*!
 * RelationPropertiesDlg type definition
 */

IMPLEMENT_DYNAMIC_CLASS( RelationPropertiesDlg, wxDialog )

/*!
 * RelationPropertiesDlg event table definition
 */

BEGIN_EVENT_TABLE( RelationPropertiesDlg, wxDialog )

////@begin RelationPropertiesDlg event table entries
    EVT_COMBOBOX( ID_CMBTABLES, RelationPropertiesDlg::OnCmbtablesSelected )

    EVT_UPDATE_UI( ID_CMBFOREIGN, RelationPropertiesDlg::OnCmbforeignUpdate )

    EVT_UPDATE_UI( wxID_OK, RelationPropertiesDlg::OnOkUpdate )

////@end RelationPropertiesDlg event table entries

END_EVENT_TABLE()

/*!
 * RelationPropertiesDlg constructors
 */

RelationPropertiesDlg::RelationPropertiesDlg( )
{
    Init();
}

RelationPropertiesDlg::RelationPropertiesDlg( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}

/*!
 * RelationPropertiesDlg creator
 */

bool RelationPropertiesDlg::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin RelationPropertiesDlg creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end RelationPropertiesDlg creation
    return true;
}

/*!
 * Member initialisation 
 */

void RelationPropertiesDlg::Init()
{
////@begin RelationPropertiesDlg member initialisation
    activeRecordGen = NULL;
    p_nodeHash = NULL;
    cmb_tables = NULL;
    tc_itemName = NULL;
    cmb_foreignId = NULL;
    tc_className = NULL;
    btn_OK = NULL;
////@end RelationPropertiesDlg member initialisation
}
/*!
 * Control creation for RelationPropertiesDlg
 */

void RelationPropertiesDlg::CreateControls()
{    
////@begin RelationPropertiesDlg content construction
    // Generated by DialogBlocks, Fri 09 Feb 2007 01:56:27 ART (unregistered)

    RelationPropertiesDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Table:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* cmb_tablesStrings = NULL;
    cmb_tables = new wxComboBox( itemDialog1, ID_CMBTABLES, _T(""), wxDefaultPosition, wxDefaultSize, 0, cmb_tablesStrings, wxCB_READONLY );
    itemBoxSizer3->Add(cmb_tables, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText6 = new wxStaticText( itemDialog1, wxID_STATIC, _("Item name:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText6, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    tc_itemName = new wxTextCtrl( itemDialog1, ID_TCITEMNAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(tc_itemName, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer8, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Foreign ID:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemStaticText9, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    wxString* cmb_foreignIdStrings = NULL;
    cmb_foreignId = new wxComboBox( itemDialog1, ID_CMBFOREIGN, _T(""), wxDefaultPosition, wxDefaultSize, 0, cmb_foreignIdStrings, wxCB_READONLY );
    itemBoxSizer8->Add(cmb_foreignId, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Class Name:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxADJUST_MINSIZE, 5);

    tc_className = new wxTextCtrl( itemDialog1, ID_TCCLASSNAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer8->Add(tc_className, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer13 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer13, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    btn_OK = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    btn_OK->SetDefault();
    itemBoxSizer13->Add(btn_OK, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer13->Add(itemButton15, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end RelationPropertiesDlg content construction
}

/*!
 * wxEVT_COMMAND_COMBOBOX_SELECTED event handler for ID_CMBTABLES
 */

void RelationPropertiesDlg::OnCmbtablesSelected( wxCommandEvent& event )
{
	if(relation.relationType==AR_HAS_MANY){
		wxString table=cmb_tables->GetValue();
		cmb_foreignId->Clear();
		wxArrayString arr=activeRecordGen->GetColumns(table);
		for(unsigned int i=0;i<arr.Count();i++)
			cmb_foreignId->Append(arr.Item(i));
		if(p_nodeHash->count(table)){
			tc_className->SetValue((*p_nodeHash)[table]->m_className);
			tc_itemName->SetValue(wxActiveRecordGenerator::Pluralize((*p_nodeHash)[table]->m_className));
		}
		else{
			tc_className->SetValue(wxActiveRecordGenerator::DePluralize(wxActiveRecordGenerator::CapitalizeTable(table)));
			tc_itemName->SetValue(wxActiveRecordGenerator::CapitalizeTable(table));
		}
	}
	else if(relation.relationType==AR_BELONGS_TO){
		wxString table=cmb_tables->GetValue();
		cmb_foreignId->Clear();
		wxArrayString arr=activeRecordGen->GetColumns(thisTable);
		for(unsigned int i=0;i<arr.Count();i++)
			cmb_foreignId->Append(arr.Item(i));

		if(p_nodeHash->count(table)){
			tc_className->SetValue((*p_nodeHash)[table]->m_className);
			tc_itemName->SetValue((*p_nodeHash)[table]->m_className);
		}
		else{
			tc_className->SetValue(wxActiveRecordGenerator::DePluralize(wxActiveRecordGenerator::CapitalizeTable(table)));
			tc_itemName->SetValue(wxActiveRecordGenerator::DePluralize(wxActiveRecordGenerator::CapitalizeTable(table)));
		}
	}
}

/*!
 * wxEVT_UPDATE_UI event handler for ID_CMBFOREIGN
 */

void RelationPropertiesDlg::OnCmbforeignUpdate( wxUpdateUIEvent& event )
{
	if(cmb_tables->GetSelection()==wxNOT_FOUND)
		cmb_foreignId->Enable(false);
	else
		cmb_foreignId->Enable(true);
}

/*!
 * wxEVT_UPDATE_UI event handler for wxID_OK
 */

void RelationPropertiesDlg::OnOkUpdate( wxUpdateUIEvent& event )
{
	if(cmb_tables->GetSelection()==wxNOT_FOUND||cmb_foreignId->GetSelection()==wxNOT_FOUND||tc_itemName->GetValue().IsEmpty()||tc_className->GetValue().IsEmpty())
		btn_OK->Enable(false);
	else
		btn_OK->Enable(true);
}

/*!
 * Should we show tooltips?
 */

bool RelationPropertiesDlg::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap RelationPropertiesDlg::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin RelationPropertiesDlg bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end RelationPropertiesDlg bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon RelationPropertiesDlg::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin RelationPropertiesDlg icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end RelationPropertiesDlg icon retrieval
}

bool RelationPropertiesDlg::TransferDataToWindow(){
	if(activeRecordGen==NULL)
		return false;

	wxArrayString tables=activeRecordGen->GetTables();
	for(unsigned int i=0;i<tables.Count();++i)
		cmb_tables->Append(tables.Item(i));
	if(relation.relationType==AR_HAS_MANY && !relation.table.IsEmpty() ){
		wxArrayString arr=activeRecordGen->GetColumns(relation.table);
		for(unsigned int i=0;i<arr.Count();i++)
			cmb_foreignId->Append(arr.Item(i));
	}
	else if(relation.relationType==AR_BELONGS_TO){
		wxArrayString arr=activeRecordGen->GetColumns(thisTable);
		for(unsigned int i=0;i<arr.Count();i++)
			cmb_foreignId->Append(arr.Item(i));
	}

	cmb_tables->SetValue(relation.table);
	cmb_foreignId->SetValue(relation.foreignId);
	tc_itemName->SetValue(relation.itemName);
	tc_className->SetValue(relation.arClassName);

	return true;

}

bool RelationPropertiesDlg::TransferDataFromWindow(){
	relation.table=cmb_tables->GetValue();
	relation.foreignId=cmb_foreignId->GetValue();
	relation.itemName=tc_itemName->GetValue();
	relation.arClassName=tc_className->GetValue();
	
	if(!p_nodeHash->count(relation.table)){
		wxMessageDialog askDlg(this,wxString::Format(wxT("The %s table has to be generated and the Id field set before\nbeing able to make a Belongs To relation."),relation.table.c_str()),wxT("ActiveRecord missing"),wxOK|wxICON_INFORMATION);
		return false;
	}
	relation.keyId=(*p_nodeHash)[relation.table]->m_idField;

	return true;

}