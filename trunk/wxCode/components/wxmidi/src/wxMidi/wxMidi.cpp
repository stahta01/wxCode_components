//=====================================================================================
// wxMidi: A MIDI interface based on PortMidi, the Portable Real-Time MIDI Library
// --------------------------------------------------------------------------------
//
// Author:      Cecilio Salmeron
// Copyright:   (c) 2005-2007 Cecilio Salmeron
// Licence:     wxWidgets licence
//=====================================================================================
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "wxMidi.h"
#endif

// For compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "wxMidi.h"

/*
	To do, when a new version of portmidi:
	1. Review error messages (method GetErrorText(), copied and adapted from pormidi.c)


*/

// Private global functions
//---------------------------------------------------------------------------------
wxMidiTimestamp wxMidiGetTime() { return Pt_Time(); }

//Define the new command event to inform that MIDI input data is available
DEFINE_EVENT_TYPE(wxEVT_MIDI_INPUT)

//================================================================================
// Implementation of classes wxMidiPmEvent, wxMidiShortEvent and wxMidiSysExEvent
//================================================================================

#define SYSEX_CHUNK_SIZE		4								// PmEvent size
#define SYSEX_BUFFER_SIZE		(SYSEX_CHUNK_SIZE * 1024)		// alloc in 4K chunks

wxMidiSysExMessage::wxMidiSysExMessage()
		: wxMidiMessage()
{
	m_type = wxMIDI_SYSEX_MSG;
	m_pMessage = (wxByte*) NULL;
	m_nSize = 0;
	m_nError = 	wxMIDI_NO_ERROR;
	m_timestamp = 0;
}


wxMidiSysExMessage::wxMidiSysExMessage(wxByte* msg, wxMidiTimestamp timestamp)
		: wxMidiMessage()
{
	m_type = wxMIDI_SYSEX_MSG;
	m_pMessage = (wxByte*) NULL;
	m_nSize = 0;
	m_nError = 	wxMIDI_NO_ERROR;
	m_timestamp = timestamp;

	//verify start of message
	if (*msg != 0xF0) {
		m_nError = wxMIDI_ERROR_BadSysExMsg_Start;
		return;
	}

	//determine size of message
	wxByte* pData = msg;
	for (; m_nSize < 10000 && *pData != 0xF7; pData++) {
		m_nSize++;
	}
	if (m_nSize == 10000) {
		m_nError = wxMIDI_ERROR_BadSysExMsg_Length;
		return;
	}

	// allocate the buffer
	m_nSize++;		//include the end of sysex byte 0xF7
	m_pMessage = new wxByte[m_nSize];

	// copy the data
	memcpy(m_pMessage, msg, m_nSize);

}

wxMidiSysExMessage::~wxMidiSysExMessage()
{
	if (m_pMessage) {
		delete [] m_pMessage;
		m_pMessage = (wxByte*) NULL;
	}
}

//================================================================================
// Implementation of class wxMidiDevice
//================================================================================


wxMidiDevice::wxMidiDevice(wxMidiDeviceID nDevice)
{
	m_nDevice = nDevice;
	m_pInfo = Pm_GetDeviceInfo(m_nDevice);
	m_stream = (PortMidiStream*)NULL;
}

wxMidiDevice::~wxMidiDevice()
{
	/*
    WARNING: Pm_GetDeviceInfo() returns a pointer to a PmDeviceInfo structure
    referring to the device specified by id. The returned structure is owned by
	the PortMidi implementation and must not be manipulated or freed.
	*/
	//if (m_pInfo) delete m_pInfo;
}

const wxString wxMidiDevice::DeviceName()
{
	if (m_pInfo) {
#if defined(_UNICODE) || defined(UNICODE)
		return wxString(m_pInfo->name, wxConvUTF8);
#else
		return m_pInfo->name;
#endif
	}
	else
		return _("Error: Device not properly created");
}

const wxString wxMidiDevice::InterfaceUsed()
{
	if (m_pInfo) {
#if defined(_UNICODE) || defined(UNICODE)
		return wxString(m_pInfo->interf, wxConvUTF8);
#else
		return m_pInfo->interf;
#endif
	}
	else
		return _("Error: Device not properly created");
}

bool wxMidiDevice::IsInputPort()
{
	if (m_pInfo)
		return (m_pInfo->input != 0);
	else
		return false;
}

bool wxMidiDevice::IsOutputPort()
{
	if (m_pInfo)
		return (m_pInfo->output != 0);
	else
		return false;
}


//================================================================================
// Class wxMidiOutDevice implementation
//================================================================================

wxMidiError wxMidiOutDevice::Open(long latency, void *DriverInfo)
{
	// bufferSize = 0    How to set up an optimum value?
	return (wxMidiError)Pm_OpenOutput(&m_stream, m_nDevice, DriverInfo,
									  0, NULL, NULL, latency );
}

wxMidiError wxMidiOutDevice::Write(wxMidiShortMessage* pMsg)
{
	return (wxMidiError)Pm_Write(m_stream, pMsg->GetBuffer(), 1);
}

wxMidiError wxMidiOutDevice::Write(wxMidiSysExMessage* pMsg)
{
	wxMidiError nError = pMsg->Error();
	if (nError != wxMIDI_NO_ERROR)
		return nError;
	else
		return (wxMidiError)Pm_WriteSysEx(m_stream,
									  pMsg->GetTimestamp(),
									  (unsigned char *)pMsg->GetMessage() );
}

wxMidiError wxMidiOutDevice::Write(wxByte* msg, wxMidiTimestamp when)
{
	return (wxMidiError)Pm_WriteSysEx(m_stream,
									  when,
									  (unsigned char *)msg );
}

wxMidiError	wxMidiOutDevice::NoteOff(int channel, int note, int velocity)
{
	//0x80?0x8F		Note off
	wxMidiShortMessage msg(0x80+channel, note, velocity);
	return Write(&msg);
}

wxMidiError	wxMidiOutDevice::NoteOn(int channel, int note, int velocity)
{
    //0x90?0x9F		Note on
	wxMidiShortMessage msg(0x90+channel, note, velocity);
	return Write(&msg);
}

wxMidiError	wxMidiOutDevice::ProgramChange(int channel, int instrument)
{
    //0xC0?0xCF		Program change
	wxMidiShortMessage msg(0xC0+channel, instrument, 0);
	return Write(&msg);
}

wxMidiError wxMidiOutDevice::AllSoundsOff()
{
	wxMidiPmEvent buffer[32];
	wxMidiTimestamp now = ::wxMidiGetTime();
	int channel, i;
	for (i=0, channel=0; channel < 16; channel++) {
		buffer[i].message = Pm_Message(0xB0+channel, 0x78, 0);
		buffer[i].timestamp = now;
		i++;
		buffer[i].message = Pm_Message(0xB0+channel, 0x7B, 0);
		buffer[i].timestamp = now;
		i++;
	}
	return (wxMidiError)Pm_Write(m_stream, buffer, 32);
}


//================================================================================
// Class wxMidiInDevice implementation
//================================================================================

wxMidiInDevice::wxMidiInDevice(wxMidiDeviceID nDevice)
	: wxMidiDevice(nDevice)
{
	m_pThread = (wxMidiThread*)NULL;
	m_SysexBuffer = (wxByte*)NULL;
	m_fReadingSysex = false;
	m_fEventPending = false;
}

wxMidiInDevice::~wxMidiInDevice()
{
	if (m_pThread) {
		StopListening();
	}
	if (m_SysexBuffer) delete [] m_SysexBuffer;
}

wxMidiError wxMidiInDevice::Open(void *DriverInfo)
{
	// bufferSize = 50    How to set up an optimum value?
	return (wxMidiError)Pm_OpenInput(&m_stream, m_nDevice, DriverInfo,
									 50, NULL, NULL);
}

wxMidiError wxMidiInDevice::Read(wxMidiPmEvent *buffer, long* length )
{
	/*
	If no error, the real number of buffers read is returned in "length" and
	value PmNoError is returned.
	In case of error, the error type is returned.
	*/

	int nErr = (int) Pm_Read(m_stream, buffer, *length);
	/* Pm_Read() retrieves midi data into a buffer, and returns the number
    of events read. Result is a non-negative number unless an error occurs,
    in which case a wxMidiError value will be returned. */
	if (nErr < 0)
		//error
		return (wxMidiError)nErr;
	else {
		*length = (long)nErr;
		return wxMIDI_NO_ERROR;
	}

}

wxMidiMessage* wxMidiInDevice::Read(wxMidiError* pError)
{
	/*
	According to pormidi documentation, portmidi is allowed to pass real-time MIDI
	messages embedded within the chunks of a sysex message, and it is up to the client
	to detect, process, and	remove these messages as they arrive.

	To deal with this, xMidiInDevice will maintain buffers with data not yet delivered.
	Flag m_fReadingSysex will signal that a sysex message was interrupted by a real-time one
	Flag m_fEventPending will signal that there is a PmEvent pending to be processed and
	delivered, as consecuence of a previous truncated sysex message.

	*/

	// get data from midi stream
	PmEvent buffer;
	wxMidiError nError;

	if (m_fEventPending) {
		buffer = m_event;
		m_fEventPending = false;
	}
	else {
		nError = (wxMidiError) Pm_Read( m_stream, &buffer, 1 );

		// if read was not successful return the error
		if (nError < wxMIDI_NO_ERROR) {
			*pError = nError;
			return (wxMidiMessage*)NULL;
		}

		// check if something read
		if (nError == 0) {
			*pError = wxMIDI_ERROR_NoDataAvailable;
			return (wxMidiMessage*)NULL;
		}
	}

	// check what type of message we are receiving
	if (m_fReadingSysex || (Pm_MessageStatus( buffer.message ) == 0xF0))
	{
		// Start or continuation of a Sysex message. Move data to buffer and
		// continue reading until end of message

		//create the message object
		wxMidiSysExMessage* pSysExMsg = new wxMidiSysExMessage();

		if (!m_fReadingSysex)
		{
			//store timestamp
			m_timestamp = buffer.timestamp;

			// alloc a new buffer
			m_SizeOfSysexBuffer = SYSEX_BUFFER_SIZE;
			m_SysexBuffer = new wxByte[ m_SizeOfSysexBuffer ];
			m_CurSysexDataPtr = m_SysexBuffer;

			m_fReadingSysex = true;
		}

		//move data to buffer and continue reading until end of sysex message
		bool fEndSysex = MoveDataToSysExBuffer(buffer.message);

		while(!fEndSysex) {
			nError = (wxMidiError) Pm_Read( m_stream, &buffer, 1 );
			if (nError < wxMIDI_NO_ERROR) {
				*pError = nError;
				delete pSysExMsg;
				delete [] m_SysexBuffer;
				m_SysexBuffer = (wxByte*)NULL;
				m_fReadingSysex = false;
				return (wxMidiMessage*)NULL;
			}

			// check if it is a real-time message inserted into the sysex chunks stream
			if (Pm_MessageStatus( buffer.message ) == 0xF8) {
				//it is a real time message. Deliver it inmediately and save sysex buffer
				*pError = wxMIDI_NO_ERROR;
				wxMidiShortMessage* pShortMsg = new wxMidiShortMessage(
														Pm_MessageStatus( buffer.message ),
														0, 0 );
				pShortMsg->SetTimestamp( buffer.timestamp );
				delete pSysExMsg;
				return pShortMsg;
			}

			/*
			When receiving sysex messages, if you get a non-real-time status byte
			but there was no EOX byte, it means the sysex message was somehow truncated.
			This is not	considered an error; e.g., a missing EOX can result from the user
			disconnecting a MIDI cable during sysex transmission.
			*/

			// lets check if there is a status byte different from end-of-sysex
			if (Pm_MessageStatus( buffer.message ) != 0xF7 &&
				Pm_MessageStatus( buffer.message ) & 0x80 )
			{
				//The systex message is somehow truncated. Return the sysex message
				// and store the new message received

				// save the PmEvent
				m_fEventPending = true;
				m_event.message = buffer.message;
				m_event.timestamp = buffer.timestamp;

				//terminate the current sysex message
				fEndSysex = true;
			}
			else {
				// it is a chunck of the sysex message. Move data to the sysex buffer
				fEndSysex = MoveDataToSysExBuffer(buffer.message);
			}
		}

		//prepare the sysex message to return it
		pSysExMsg->SetTimestamp( m_timestamp );
		pSysExMsg->SetBuffer( m_SysexBuffer );
		pSysExMsg->SetLength( m_CurSysexDataPtr - m_SysexBuffer );
		*pError = wxMIDI_NO_ERROR;

		//reset the wxMidiInDevice buffer
		m_SysexBuffer = (wxByte*)NULL;
		m_CurSysexDataPtr = (wxByte*)NULL;
		m_fReadingSysex = false;

		return pSysExMsg;
	}
	else
	{
		//it is not a sysex message
		*pError = wxMIDI_NO_ERROR;
		wxMidiShortMessage* pShortMsg = new wxMidiShortMessage(
												Pm_MessageStatus( buffer.message ),
												Pm_MessageData1( buffer.message ),
												Pm_MessageData2( buffer.message ) );
		pShortMsg->SetTimestamp( buffer.timestamp );
		return pShortMsg;
	}

}

bool wxMidiInDevice::MoveDataToSysExBuffer(PmMessage message)
{
	/*
	Moves the chunk received to the sysex buffer, allocating a greater buffer
	if necessary.
	Returns true if with this chunk the sysex message is ended, false otherwise
	*/

	// if we are at the end of the buffer
	if( m_CurSysexDataPtr > (m_SysexBuffer + m_SizeOfSysexBuffer - SYSEX_CHUNK_SIZE) )
	{
		// keep previous size for usage
		long oldSize = m_SizeOfSysexBuffer;

		// increment size
		m_SizeOfSysexBuffer += SYSEX_BUFFER_SIZE;

		// allocate the new buffer
		wxByte* newSysexBuffer = new wxByte[ m_SizeOfSysexBuffer ];

		//wxLogMessage(wxString::Format(
		//	_T("Increasing buffer size from %d to %d"), oldSize, m_SizeOfSysexBuffer ));

		// move the data from old buffer
		memcpy( newSysexBuffer, m_SysexBuffer, oldSize );

		// delete the old buffer
		delete [] m_SysexBuffer;

		// point to the new buffer
		m_SysexBuffer = newSysexBuffer;

		// set the write data pointer
		m_CurSysexDataPtr = m_SysexBuffer + oldSize;
	}

	// move data to the buffer. See portmidi/pm_test/sysex.c function receive_sysex()
	int shift = 0;
	int data = 0;
	for (shift = 0; shift < 32 && (data != 0xF7); shift += 8)
	{
		data = (message >> shift) & 0xFF;
		*m_CurSysexDataPtr = data;
		++m_CurSysexDataPtr;

			/* portmidi:
			When receiving sysex messages, the sysex message is terminated
			by either an EOX status byte (anywhere in the 4 byte messages) or
			by a non-real-time status byte in the low order byte of the message.
			If you get a non-real-time status byte but there was no EOX byte, it
			means the sysex message was somehow truncated. This is not
			considered an error; e.g., a missing EOX can result from the user
			disconnecting a MIDI cable during sysex transmission.
			*/

        // if this is a status byte that is not EOX, the sysex
        // message is incomplete and there is no more sysex data
		//
		// WARNING: Is this true? Shouldn't portmidi send the status byte
		// in a new PmEvent? See portmidi/pm_test/sysex.c function receive_sysex()
		//
		// TODO: if we break without doing anything else, the status byte and
		// remaining bytes will be lost !!!
        if (data & 0x80 && data != 0xF0 && data != 0xF7) break;
	}

	return (data & 0x80 && data != 0xF0);

}

void wxMidiInDevice::Flush()
{
    // empty the buffer
	wxMidiPmEvent buffer[1];
	long nNumEvents;
    while (Poll()) {
		nNumEvents = 1;			//initialize with buffer size before calling Read
        Read(buffer, &nNumEvents);
    }

}

wxMidiError wxMidiInDevice::StartListening(wxWindow* pWindow, unsigned long nPollingRate)
{

	if (m_pThread) return wxMIDI_ERROR_AlreadyListening;

    //Create a new thread. The thread object is created in the suspended state
	m_pThread = new wxMidiThread(this, pWindow, nPollingRate);
	if ( m_pThread->Create() != wxTHREAD_NO_ERROR )
    {
        return wxMIDI_ERROR_CreateThread;		//Can't create thread!
    }

    //wxCriticalSectionLocker enter(wxGetApp().m_critsect);

	//Start the thread execution
    if (m_pThread->Run() != wxTHREAD_NO_ERROR )
    {
        return wxMIDI_ERROR_StartThread;		//Can't start thread!
    }

	return wxMIDI_NO_ERROR;
}

wxMidiError wxMidiInDevice::StopListening()
{
	//stop the thread and wait for its termination
	m_pThread->Delete();
	delete m_pThread;
	m_pThread = (wxMidiThread*)NULL;
	return wxMIDI_NO_ERROR;
}

//================================================================================
// Class wxMidiThread implementation
//================================================================================

// In MSWin32 all threads are joinable. So for platform independence I will
// always use joinable threads
wxMidiThread::wxMidiThread(wxMidiInDevice* pDev, wxWindow* pWindow, unsigned long milliseconds)
	: wxThread(wxTHREAD_JOINABLE)
{
	m_pDev = pDev;
	m_pWindow = pWindow;
	m_nMilliseconds = milliseconds;
}

wxMidiThread::~wxMidiThread()
{
}

void* wxMidiThread::Entry()
{
    while(true)
	{
        // check if the thread was asked to exit and do it
        if (TestDestroy()) break;

		// check if Midi data is available
		if ( m_pDev->Poll() ) {
			// Data available. Create a Midi event
			wxCommandEvent event( wxEVT_MIDI_INPUT );
			::wxPostEvent( m_pWindow, event );
			//m_pWindow->GetEventHandler()->AddPendingEvent(event);	// Add it to the queue
		}

        // pause the thread execution during polling rate interval
        wxThread::Sleep(m_nMilliseconds);
    }

    return NULL;
}



//================================================================================
// Class wxMidiSystem implementation
//
//	Acording to documentation Pm_Initialize and Pm_Terminate return a error code, but
//	looking at the source code they always return pmNoError. So it is useless to
//	try to preserve the return code by forcing the user to explicitly call
//	these methods. It is easier to initialize and terminate in
//	the constructor and destructor, respectively
//================================================================================

wxMidiSystem* wxMidiSystem::m_pInstance = (wxMidiSystem*)NULL;

wxMidiSystem::~wxMidiSystem()
{
	Terminate();
	wxMidiDatabaseGM* pGM = wxMidiDatabaseGM::GetInstance();
	delete pGM;
}

wxMidiError wxMidiSystem::Initialize()
{
	Pt_Start(1, 0, 0);						// start the timer
	return (wxMidiError)Pm_Initialize();	// initialize portmidi package
}

wxMidiError wxMidiSystem::Terminate()
{
	Pt_Stop();				//stop the timer
	return (wxMidiError)Pm_Terminate();
}

const wxString wxMidiSystem::GetErrorText( wxMidiError errnum )
{
    switch(errnum)
    {
    case wxMIDI_NO_ERROR:
		return _T("wxMidi: 'No error'");
    case wxMIDI_ERROR_HostError:
        return _("PortMidi: `Host error'");
    case wxMIDI_ERROR_InvalidDeviceId:
		return _("PortMidi: `Invalid device ID'");
    case wxMIDI_ERROR_InsufficientMemory:
        return _("PortMidi: `Insufficient memory'");
    case wxMIDI_ERROR_BufferTooSmall:
        return _("PortMidi: 'Buffer too small'");
    case wxMIDI_ERROR_BadPtr:
        return _("PortMidi: `Bad pointer'");
    case wxMIDI_ERROR_InternalError:
        return _("PortMidi: 'Internal PortMidi Error'");
    case wxMIDI_ERROR_BufferOverflow:
        return _("PortMidi: 'Buffer overflow'");
    case wxMIDI_ERROR_BadData:
		return _("PortMidi: 'Invalid MIDI message Data'");
	case wxMIDI_ERROR_BufferMaxSize:
		return _("PortMidi: 'Buffer is already as large as it can be'");

	// Additional error messages for wxMidi
	case wxMIDI_ERROR_AlreadyListening:
        return _("wxMidi: 'Already lisening!'");
	case wxMIDI_ERROR_CreateThread:
        return _("wxMidi: 'Error while creating the thread object'");
	case wxMIDI_ERROR_StartThread:
        return _("wxMidi: 'Error while starting the thread execution'");
	case wxMIDI_ERROR_BadSysExMsg_Start:
		return _("wxMidi. 'Bad sysex msg: It does not start with 0xF0'");
	case wxMIDI_ERROR_BadSysExMsg_Length:
		return _("wxMidi. 'Bad sysex msg: Length greater than 10000 or no final byte 0xF7'");
	case wxMIDI_ERROR_NoDataAvailable:
		return _("wxMidi. 'There are no MIDI messages pending to be read'");

	default:
        return _("wxMidi: 'Illegal error number'");
    }
}


wxString wxMidiSystem::GetHostErrorText()
{
	//TODO: review this
	char msg[1000];
	Pm_GetHostErrorText(&msg[0], 1000);
	return wxString( (const wxChar *)&msg[0], wxSTRING_MAXLEN );
}

wxMidiSystem* wxMidiSystem::GetInstance()
{
	if (!m_pInstance) {
		m_pInstance = new wxMidiSystem();
		m_pInstance->Initialize();
	}
	return m_pInstance;
}


