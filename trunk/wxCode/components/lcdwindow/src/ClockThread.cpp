#include "ClockThread.h"
#include "LCDSampleApp.h"


CClockThread::CClockThread() : wxThread( wxTHREAD_DETACHED )
{
	Create();
	SetPriority( 0 );
}

CClockThread::~CClockThread()
{

}


void *CClockThread::Entry( void )
{
	while( ! TestDestroy() )
	{
		wxGetApp().UpdateClock();
	}

	return NULL;
}
