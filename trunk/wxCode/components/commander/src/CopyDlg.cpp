//---------------------------------------------------------------------------
//
// Name:        CopyDelDlg.cpp
// Author:      Armando Urdiales Gonz�lez
// Created:     25/03/2007 12:23:12
// Description: CopyDelDlg class implementation
//
//---------------------------------------------------------------------------

#include "CopyDlg.h"

//Do not add custom headers
//wxDev-C++ designer will remove them
////Header Include Start
#include "Images/Self_CopyDlg_XPM.xpm"
////Header Include End
#include "Images/icon1.xpm"


//----------------------------------------------------------------------------
// CopyDlg
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(CopyDlg,wxDialog)
	////Manual Code Start
	////Manual Code End
	
	EVT_CLOSE(CopyDlg::OnClose)
	EVT_BUTTON(ID_BTNCANCEL,CopyDlg::btnCancelClick)
	EVT_BUTTON(ID_BTNCOPY,CopyDlg::btnCopyClick)
END_EVENT_TABLE()
////Event Table End

CopyDlg::CopyDlg(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
: wxDialog(parent, id, title, position, size, style)
{
	CreateGUIControls();
	//m_parent = parent;
}

CopyDlg::~CopyDlg()
{
   //delete(WxListCtrl1->GetImageList(wxIMAGE_LIST_SMALL)); // MEMORY LEAK
   //delete(WxListCtrl1->GetImageList(wxIMAGE_LIST_SMALL));// MEMORY LEAK

   delete(ckExistFiles);
   delete(ckReadFiles);
   delete(lblDetails);
   delete(lblDetails2);
   delete(lblDetails3);
   delete(lblDetails4);
   delete(btnCancel);
   delete(btnCopy);
   delete(lblDestination);
   delete(WxGauge1);
   delete(lblSource);
   delete(WxListCtrl1);
   delete(WxListCtrl2);
   //delete(this);
} 

void CopyDlg::CreateGUIControls()
{
	//Do not add custom code between
        //GUI Items Creation Start and GUI Items Creation End.
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start

	SetTitle(wxT("Copy Files (wxOpenCommander)"));
	SetIcon(Self_CopyDlg_XPM);
	SetSize(8,8,750,525);
	Center();
	

	lblDetails4 = new wxStaticText(this, ID_LBLDETAILS4, wxT(""), wxPoint(543,215), wxDefaultSize, 0, wxT("lblDetails4"));

	lblDetails3 = new wxStaticText(this, ID_LBLDETAILS3, wxT(""), wxPoint(359,214), wxDefaultSize, 0, wxT("lblDetails3"));

	lblDetails2 = new wxStaticText(this, ID_LBLDETAILS2, wxT(""), wxPoint(6,216), wxDefaultSize, 0, wxT("lblDetails2"));

	ckExistFiles = new wxCheckBox(this, ID_CKEXISTFILES, wxT("Overwrite existing files"), wxPoint(176,464), wxSize(146,17), 0, wxDefaultValidator, wxT("ckExistFiles"));
	ckExistFiles->Enable(false);
	ckExistFiles->SetValue(true);

	ckReadFiles = new wxCheckBox(this, ID_CKREADFILES, wxT("Overwrite Read only files"), wxPoint(6,463), wxSize(138,17), 0, wxDefaultValidator, wxT("ckReadFiles"));
	ckReadFiles->Enable(false);
	ckReadFiles->SetValue(true);

	lblDetails = new wxStaticText(this, ID_LBLDETAILS, wxT(""), wxPoint(7,199), wxDefaultSize, 0, wxT("lblDetails"));

	btnCancel = new wxButton(this, ID_BTNCANCEL, wxT("&Cancel"), wxPoint(634,458), wxSize(98,28), 0, wxDefaultValidator, wxT("btnCancel"));

	btnCopy = new wxButton(this, ID_BTNCOPY, wxT("Copy"), wxPoint(532,458), wxSize(98,28), 0, wxDefaultValidator, wxT("btnCopy"));

	lblDestination = new wxStaticText(this, ID_LBLDESTINATION, wxT("Destination"), wxPoint(9,261), wxDefaultSize, wxALIGN_CENTRE, wxT("lblDestination"));
	lblDestination->SetFont(wxFont(11, wxSWISS, wxNORMAL,wxNORMAL, FALSE));

	WxListCtrl2 = new wxListCtrl(this, ID_WXLISTCTRL2, wxPoint(8,282), wxSize(724,170), wxLC_REPORT);

	WxGauge1 = new wxGauge(this, ID_WXGAUGE1, 100, wxPoint(9,236), wxSize(721,21), wxGA_HORIZONTAL, wxDefaultValidator, wxT("WxGauge1"));
	WxGauge1->SetRange(100);
	WxGauge1->SetValue(0);

	lblSource = new wxStaticText(this, ID_LBLSOURCE, wxT("Source"), wxPoint(9,3), wxDefaultSize, wxALIGN_CENTRE, wxT("lblSource"));
	lblSource->SetFont(wxFont(11, wxSWISS, wxNORMAL,wxNORMAL, FALSE));

	WxListCtrl1 = new wxListCtrl(this, ID_WXLISTCTRL1, wxPoint(7,25), wxSize(724,168), wxLC_REPORT);
	////GUI Items Creation End

	autoInit = false;
	autoClose = false;
	blnCopping = false;
	totalSize=0;
	actualSize=0;
	numTotaFiles=0;
	numActualFile=0;
	WxListCtrl1->InsertColumn(0, "Files", wxLIST_FORMAT_LEFT, 1000);
   WxListCtrl2->InsertColumn(0, "Files", wxLIST_FORMAT_LEFT, 1000);
   
   
   int size = 20;
   wxImageList* imageList1 = new wxImageList(size, size, true);
   wxImageList* imageList2 = new wxImageList(size, size, true);

   wxIcon icons[1];
   icons[0] = wxIcon(icon1_xpm);

   int sizeOrig = icons[0].GetWidth();
   for ( size_t i = 0; i < WXSIZEOF(icons); i++ )
   {
       if ( size == sizeOrig )
       {
          imageList1->Add(icons[i]);
          imageList2->Add(icons[i]);
       }
       else
       {
          imageList1->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
          imageList2->Add(wxBitmap(wxBitmap(icons[i]).ConvertToImage().Rescale(size, size)));
       }
   }

   WxListCtrl1->AssignImageList(imageList1, wxIMAGE_LIST_SMALL);
   WxListCtrl2->AssignImageList(imageList2, wxIMAGE_LIST_SMALL);
}

void CopyDlg::OnClose(wxCloseEvent& /*event*/)
{
   //wxOpenCommanderFrm* parent = (wxOpenCommanderFrm*)m_parent;
   //parent->ListCtlUpdate();
	Destroy();
}

bool CopyDlg::onBeginCopyFile(const wxString& sourcePath, const wxString& destinationPath)
{
   wxFileName file(sourcePath);
   long long fileSize = getFileSize(sourcePath);
   numActualFile++;
   
   // Avance the gauge the haft size of the actual file. 
   long long estimatedSize = actualSize + (fileSize/2);
   int percent = (estimatedSize * 100) / totalSize;
   WxGauge1->SetValue(percent);
   
   lblDetails->SetLabel("Path: " + file.GetPath());
   lblDetails2->SetLabel("File: " + file.GetFullName());
   lblDetails3->SetLabel("Size: " + formatFileSize(actualSize) + " / " + formatFileSize(totalSize));
   lblDetails4->SetLabel("File: " + LongLongTowxString(numActualFile) + " / " + LongLongTowxString(numTotaFiles));
   return !blnCanceled;
}

void CopyDlg::onEndCopyFile(bool copy, const wxString& sourcePath, const wxString& destinationPath)
{
   if (!copy) return;
   
   long itemPos = WxListCtrl1->FindItem(-1, sourcePath);
   WxListCtrl1->DeleteItem(itemPos);
   int item = WxListCtrl2->GetItemCount();
   WxListCtrl2->InsertItem(item, destinationPath,0);
   
   // Update the wxGauge.
   long long fileSize = getFileSize(destinationPath);
   actualSize = actualSize + fileSize;
   int percent = (actualSize * 100) / totalSize;
   WxGauge1->SetValue(percent);
   
   wxWakeUpIdle();
}

void CopyDlg::onCopyThreadFinish()
{
   lblDetails->SetLabel("");
   lblDetails2->SetLabel("");

   if (actualSize == totalSize) WxGauge1->SetValue(100);

   int item = WxListCtrl1->GetItemCount();

   btnCopy->Enable(!blnCanceled && item != 0);
   blnCopping = false;
   btnCancel->SetLabel("Close");
   
   #ifdef __WXMSW__
      if (autoClose && !item) SendMessage((HWND)GetHWND(), WM_CLOSE, (WPARAM)TRUE, (LPARAM)NULL);
   #endif
}

void onThreadCopyFinish(wxThread* thread, void* contextParam, void* parent)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   return parentWindow->onCopyThreadFinish();   
}

bool onThreadBeginCopyFile(void* parent, const wxString& sourcePath, const wxString& destinationPath)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   return parentWindow->onBeginCopyFile(sourcePath, destinationPath);
}

void onThreadEndCopyFile(void* parent, bool copy, const wxString& sourcePath, const wxString& destinationPath)
{
   CopyDlg* parentWindow;
   parentWindow = (CopyDlg*)parent;
   return parentWindow->onEndCopyFile(copy, sourcePath, destinationPath);
}

void CopyDlg::btnCopyClick(wxCommandEvent& event)
{
	size_t nFiles = m_pathsCopy.size();
   if (nFiles < 0) return;
   
   CThread* thread = new CThread();
   thread->setParent((void*) this);

   thread->setVectorCopyParams(m_pathsCopy);
   thread->setBeginCopyFileCallBackFunc(onThreadBeginCopyFile);
   thread->setEndCopyFileCallBackFunc(onThreadEndCopyFile);
   thread->setFinishCallBackFunc(onThreadCopyFinish, NULL);
   thread->Create();
   thread->Run();
   btnCopy->Enable(false);
   blnCopping = true;
}

void CopyDlg::btnCancelClick(wxCommandEvent& event)
{
   if (!blnCopping)
      Destroy();
   else   
      blnCanceled = true;   
}

void CopyDlg::setAutoInit(bool init)
{
   autoInit = init;
}

void CopyDlg::setAutoClose(bool close)
{
   autoClose = close;
}

void CopyDlg::setPathsToCopy(vectorCopyParams pathsCopy)
{
   m_pathsCopy = pathsCopy;
   updateSourceListCtrl();
   if (autoInit) 
   {
      wxCommandEvent event;
      btnCopyClick(event);
   }
}

void CopyDlg::updateSourceListCtrl()
{
   WxListCtrl1->DeleteAllItems();  
   
   for (size_t i = 0 ; i < m_pathsCopy.size(); i++)
   {
      addFilesInSourceList(m_pathsCopy[i].sourcePath + "\\" + m_pathsCopy[i].item);
   }
   if (m_pathsCopy.size())
      lblDestination->SetLabel("Destination: " + m_pathsCopy[0].newPath + "\\");
   lblDetails->SetLabel("");
   lblDetails2->SetLabel("");
   lblDetails3->SetLabel("Size: 0 / " + formatFileSize(totalSize));
   lblDetails4->SetLabel("File: 0 / " + LongLongTowxString(numTotaFiles));
}

void CopyDlg::addFilesInSourceList(const wxString& filePath)
{
   if (wxDir::Exists(filePath))
   {
      wxDir dir;
      wxString filename;
      wxString wxStrFileName;

      dir.Open(filePath);
      if (!dir.IsOpened()) return;
      bool cont = dir.GetFirst(&filename);
      while (cont)
      {
         wxStrFileName = filePath + "\\" + filename;
         addFilesInSourceList(wxStrFileName);
         cont = dir.GetNext(&filename);
      }
   }
   else
   {
      if (wxFile::Exists(filePath))
      {
         int item = WxListCtrl1->GetItemCount();
         WxListCtrl1->InsertItem(item,filePath, 0);
         totalSize = totalSize + getFileSize(filePath);
         numTotaFiles++;
      }
   }
}

