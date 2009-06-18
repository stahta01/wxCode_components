/***************************************************************
 * Name:      MainFrame.cpp
 * Purpose:   DynamicSettingsSample tutorial (application main frame)
 * Author:    Michal Bližňák (michal.bliznak@tiscali.cz)
 * Created:   2007-10-28
 * Copyright: Michal Bližňák
 * License:   wxWidgets license (www.wxwidgets.org)
 * Notes:
 **************************************************************/

#include "wx_pch.h"

#ifdef _DEBUG_MSVC
#define new DEBUG_NEW
#endif

#include "MainFrame.h"
#include "res/wx.xpm"

MainFrame::MainFrame( wxWindow* parent )
: _MainFrame( parent )
{
	SetIcon(wxIcon(wx_xpm));

	m_pTextCtrl->SetValue(wxT("This sample demonstrates how to load/save an application settings \
via the wxXmlSerializer class.\n\nThe text bellow \
shows serialized properties of 'Settings' class object stored in \
'settings.xml' file. \
So called 'Properties' (xsProperty class objects) encapsulate marked class members \
of the 'Settings' class (derived from 'xsSerializable' class ) which can be accessed \
in the standard. Class data members which should be serialized must be marked by \
'XS_SERIALIZE' macro or by similar ones.\n\n"));

	// Dump content of the Setting class. Of course, you can also use the wxGetApp().m_pSettings
	// class members in the standard way...
	DumpSerializableObject(wxGetApp().m_pSettings, m_pTextCtrl);

	Center();
}

void MainFrame::DumpSerializableObject(xsSerializable *obj, wxTextCtrl *memo)
{
	// dump info about serializable object
	memo->AppendText(wxString::Format(wxT("Serializable object of type '%s' with ID:%d at address 0x%x :\n"), obj->GetClassInfo()->GetClassName(), obj->GetId(), obj));

	// get serialized properties maintained by the Setting class
	xsProperty *prop;
	PropertyList::compatibility_iterator node = obj->GetProperties().GetFirst();
	while( node )
	{
		prop = node->GetData();
		memo->AppendText( wxString::Format( wxT("Property name: %s, Type: %s, Value: %s\n"),
											prop->m_sFieldName.c_str(),
											prop->m_sDataType.c_str(),
											prop->ToString().c_str() ) );
		node = node->GetNext();
	}

	memo->AppendText(wxT("\n"));
}

void MainFrame::OnOk( wxCommandEvent& WXUNUSED(event) )
{
	Destroy();
}
