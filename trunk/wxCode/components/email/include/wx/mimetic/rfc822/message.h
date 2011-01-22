/***************************************************************************
    copyright            : (C) 2002-2008 by Stefano Barbato
    email                : stefano@codesink.org

    $Id: message.h,v 1.9 2008-10-07 11:06:27 tat Exp $
 ***************************************************************************/

/***************************************************************************

 Licence:     wxWidgets licence

 This file has been copied from the project Mimetic
 (http://codesink.org/mimetic_mime_library.html) and relicenced from the MIT
 licence to the wxWidgets one with authorisation received from Stefano Barbato

 ***************************************************************************/

#ifndef _MIMETIC_RFC822_MESSAGE_H_
#define _MIMETIC_RFC822_MESSAGE_H_

#include <string>
#include <iostream>
#include <wx/mimetic/rfc822/header.h>
#include <wx/mimetic/rfc822/body.h>

namespace mimetic
{

/// Simple RFC 822 message type
struct Message
{
    Message() {}
    virtual ~Message(){}
    Rfc822Header& header();
    const Rfc822Header& header() const;
    Rfc822Body& body();
    const Rfc822Body& body() const;

    friend std::ostream& operator<<(std::ostream&, const Message&);
private:
    Rfc822Header m_header;
    Rfc822Body m_body;
};

}

#endif
