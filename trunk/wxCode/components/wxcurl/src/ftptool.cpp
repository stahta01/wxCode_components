/*
 *	ftptool.cpp
 *	wxCURL
 *
 *	Created by Casey O'Donnell on Fri Aug 13 2004.
 *	Copyright (c) 2004 Casey O'Donnell. All rights reserved.
 *
 *  wxCurlFTPTool makes use of ftpparse, a set of C functions
 *  created by D. J. Bernstein (djb@cr.yp.to).  The original
 *  site for these functions can be found at:
 *  http://cr.yp.to/ftpparse.html
 *
 *	Licence: wxWidgets Licence
 */

//////////////////////////////////////////////////////////////////////
// Headers
//////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif

#include <wx/curl/ftptool.h>

#include <wx/txtstrm.h>
#include <wx/mstream.h>

#include "ftpparse.h"

//////////////////////////////////////////////////////////////////////
// C Functions for LibCURL
//////////////////////////////////////////////////////////////////////
extern "C"
{
}

// davtool.cpp: implementation of the wxCurlFTPFs class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlFTPFs::wxCurlFTPFs()
: m_iContentLength(0)
{
}

wxCurlFTPFs::wxCurlFTPFs(const wxString&	szName			,
						 const bool&		bIsDir			,
						 const bool&		bIsFile			,
						 const time_t&		tLastModified	,
						 const long&		iContentLength	)
: m_szName(szName), m_bIsDir(bIsDir), m_bIsFile(bIsFile),
m_dtLastModified(tLastModified), m_iContentLength(iContentLength)
{
}

wxCurlFTPFs::~wxCurlFTPFs()
{
}

// ftptool.cpp: implementation of the wxCurlFTPTool class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

wxCurlFTPTool::wxCurlFTPTool(const wxString& szURL /*= wxEmptyString*/, const wxString& szUserName /*= wxEmptyString*/, const wxString& szPassword /*= wxEmptyString*/, wxEvtHandler* pEvtHandler /*= NULL*/, const bool& bSendUpdateEvents /*= false*/, const bool& bSendBeginEndEvents /*= false*/)
: wxCurlFTP(szURL, szUserName, szPassword, pEvtHandler, bSendUpdateEvents, bSendBeginEndEvents)
{
}

wxCurlFTPTool::~wxCurlFTPTool()
{
}

//////////////////////////////////////////////////////////////////////
// Member Data Access Methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Action Methods
//////////////////////////////////////////////////////////////////////

bool wxCurlFTPTool::GetFTPFs(wxArrayFTPFs& fs, const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	if(List(szRemoteLoc))
	{
		wxMemoryInputStream inStream(m_szResponseBody, m_szResponseBody.Len() * sizeof(wxChar));

		if(inStream.IsOk())
		{
			wxTextInputStream txtInStream(inStream);
			wxString szCurrentLine = txtInStream.ReadLine();

			while(!szCurrentLine.IsEmpty())
			{
				struct ftpparse ftppItem;

				if(ftpparse(&ftppItem, (char*)szCurrentLine.GetWriteBuf(szCurrentLine.Len()), szCurrentLine.Len()) != 0)
				{					
					fs.Add(wxCurlFTPFs((wxChar*)ftppItem.name,(ftppItem.flagtrycwd == 1),(ftppItem.flagtryretr == 1),ftppItem.mtime,ftppItem.size));
				}

				szCurrentLine.UngetWriteBuf();
				szCurrentLine = txtInStream.ReadLine();
			}

			return true;
		}
	}

	return false;
}

bool wxCurlFTPTool::Exists(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayFTPFs arrFs;

	if(GetFTPFs(arrFs, szRemoteLoc))
	{
		return true;
	}

	return false;
}

bool wxCurlFTPTool::IsDirectory(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayFTPFs arrFs;

	if(GetFTPFs(arrFs, szRemoteLoc))
	{
		if(arrFs.Count() > 1)
			return true;
	}

	return false;
}

wxDateTime wxCurlFTPTool::GetLastModified(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayFTPFs arrFs;

	if(GetFTPFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetLastModified();
	}

	return wxDateTime();
}

long wxCurlFTPTool::GetContentLength(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayFTPFs arrFs;

	if(GetFTPFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetContentLength();
	}

	return -1;
}

wxString wxCurlFTPTool::GetFileSuffix(const wxString& szRemoteLoc /*= wxEmptyString*/)
{
	wxArrayFTPFs arrFs;

	if(GetFTPFs(arrFs, szRemoteLoc))
	{
		return arrFs.Last().GetFileSuffix();
	}

	return wxEmptyString;
}

//////////////////////////////////////////////////////////////////////
// Helper Methods
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// wxArrayDAVFs Implementation
//////////////////////////////////////////////////////////////////////
#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!
WX_DEFINE_USER_EXPORTED_OBJARRAY(wxArrayFTPFs);





