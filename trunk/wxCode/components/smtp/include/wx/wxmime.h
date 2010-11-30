/****************************************************************************

 Project     :
 Author      :
 Description :

 VERSION INFORMATION:
 File    : $Source$
 Version : $Revision$
 Date    : $Date$
 Author  : $Author$
 Licence : wxWidgets licence

 History:
 $Log: wxmime.h,v $
 Revision 1.3  2003/11/14 15:43:09  tavasti
 Sending email with alternatives works

 Revision 1.2  2003/11/07 09:17:40  tavasti
 - K��ntyv� versio, fileheaderit lis�tty.


****************************************************************************/

/*
 * Purpose: wxWindows mime implementation
 * Author:  Frank Bu�
 * Created: 2002
 */

#ifndef _WX_MIME_H
#define _WX_MIME_H

#include <wx/wx.h>
#include <wx/filename.h>
#include <wx/wfstream.h>

#include "wxsmtpdef.h"

/**
 * Public helper class for MIME encoding and type determining.
 */
class WXDLLIMPEXP_SMTP wxMimePart
{
   public:
      /**
      * Constructs a MIME part and tries to determine the type from the filename
      * extension. If this doesn't work, it sets the default type
      * application/octet-stream. The specified file will be read if Encode is called.
      * \param fileName File to be encoded as a MIME part.
      */
      wxMimePart(const wxFileName& fileName,
                 bool is_inline=FALSE,const wxString& mainType="",
                 const wxString& subType = "");

      wxMimePart(const wxString& data,
                 bool is_inline,const wxString& mainType,
                 const wxString& subType,const wxString& fileName = "");

      /**
      * Writes the file MIME encoded (currently only base64) to the specified stream.
      * \param out The stream, to which the MIME part will be written.
      */
      void Encode(wxOutputStream& out);

   private:

      wxFileName m_fileName;
      wxString m_data;
      bool     m_inline;
      wxString m_mainType;
      wxString m_subType;
};

//TODO : check that this macro is compatible with WXDLLIMPEXP_SMTP
//TODO : in fact, simply replace this table with a std template library array...
WX_DECLARE_OBJARRAY(wxMimePart, wxArrayMimePart);

#endif
