/////////////////////////////////////////////////////////////////////////////
// Name:        gpibx.h
// Purpose:     base class for gpib devices
// Author:      Joachim Buermann
// Id:          $Id: gpibx.h,v 1.2 2004/11/30 12:40:35 jb Exp $
// Copyright:   (c) 2001,2004 Joachim Buermann
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __WX_GPIBX_H
#define __WX_GPIBX_H

#include "wx/ctb/iobase.h"

#define wxGPIB1 "gpib1"
#define wxGPIB2 "gpib2"

enum wxGPIB_Timeout
{
    wxGPIB_TO_NONE = 0, 
    wxGPIB_TO_10us, 
    wxGPIB_TO_30us, 
    wxGPIB_TO_100us,
    wxGPIB_TO_300us,
    wxGPIB_TO_1ms,  
    wxGPIB_TO_3ms,  
    wxGPIB_TO_10ms, 
    wxGPIB_TO_30ms, 
    wxGPIB_TO_100ms,
    wxGPIB_TO_300ms,
    wxGPIB_TO_1s,   
    wxGPIB_TO_3s,   
    wxGPIB_TO_10s,  
    wxGPIB_TO_30s,  
    wxGPIB_TO_100s, 
    wxGPIB_TO_300s, 
    wxGPIB_TO_1000s
};

/*!
  \struct wxGPIB_DCS

  The device control struct for the gpib communication class.
  This struct should be used, if you refer advanced parameter.
*/
struct wxGPIB_DCS
{
    /*! primary address of GPIB device */
    int m_address1;
    /*! secondary address of GPIB device */
    int m_address2;
    /*! I/O timeout */
    wxGPIB_Timeout m_timeout;
    /*! EOT enable */
    bool m_eot;
    /*! EOS character */
    unsigned char m_eosChar;
    /*! EOS handling */
    unsigned char m_eosMode;
    // to avoid memory leak warnings generated by swig 
    ~wxGPIB_DCS() {};
    wxGPIB_DCS() {
	   /*! set default device address to 1 */
	   m_address1 = 1;
	   m_address2 = 0;
	   /*! 
		set the timeout to a very short value to avoid blocking
		(default are 10usec)
	   */
	   m_timeout = wxGPIB_TO_10us;
	   m_eot = true;
	   m_eosChar = 0;//'\n';
	   m_eosMode = 0;
    };
    char* GetSettings(char* buf,size_t bufsize);
}; 

/*!
  \enum IOCTL calls for wxGPIB
  
  The following Ioctl calls are only valid for the wxGPIB
  class.
*/
enum {
    /*!
	 Set the adress of the via gpib connected device.
    */
    CTB_GPIB_SETADR = CTB_GPIB,
    /*!
	 Get the serial poll byte
    */
    CTB_GPIB_GETRSP,
    /*!
	 Get the GPIB status
    */
    CTB_GPIB_GETSTA,
    /*!
	 Get the last GPIB error number
    */
    CTB_GPIB_GETERR,
    /*!
	 Get the GPIB line status (hardware control lines) as an
	 integer. The lowest 8 bits correspond to the current state
	 of the lines.
    */
    CTB_GPIB_GETLINES,
    /*!
	 Set the GPIB specific timeout
    */
    CTB_GPIB_SETTIMEOUT,
    /*!
	 Forces the specified device to go to local program mode
    */
    CTB_GPIB_GTL,
    /*!
	 This routine can only be used if the specified GPIB 
	 Interface Board is the System Controller.
	 Remember that even though the REN line is asserted, 
	 the device(s) will not be put into remote state until is
	 addressed to listen by the Active Controller
    */
    CTB_GPIB_REN,
    /*!
	 The command asserts the GPIB interface clear (IFC) line for
	 ast least 100us if the GPIB board is the system controller.
	 This initializes the GPIB and makes the interface CIC and
	 active controller with ATN asserted.
	 Note! The IFC signal resets only the GPIB interface functions
	 of the bus devices and not the internal device functions.
	 For a device reset you should use the CTB_RESET command above.
    */
    CTB_GPIB_RESET_BUS,
};

/*!
   \class wxGPIB
   
   \brief the gpib base class
*/
class wxGPIB : public wxIOBase
{
protected:
    int m_board;
    int m_hd;
    int m_state;
    int m_error;
    int m_count;
    wxGPIB_DCS m_dcs;
    int CloseDevice();
    int OpenDevice(const char* devname, void* dcs);
public:
    wxGPIB() {
	   m_board = -1;
	   m_hd = -1;
	   m_state = m_count = m_error = 0;
    };
    virtual ~wxGPIB() {Close();};
    const char* ClassName() {return "wxGPIB";};
    virtual int GetError(char* buf,size_t buflen);
    virtual int GetSettingsAsString(char* str, size_t size);
    // This is only for internal usage
    int Ibrd(char* buf,size_t len);
    // This is only for internal usage
    int Ibwrt(char* buf,size_t len);
    virtual int Ioctl(int cmd,void* args);
    int IsOpen() {
	   return m_hd >= 0;
    };
    int Read(char* buf,size_t len);
    int Write(char* buf,size_t len);

    /*!
	 \brief FindListener returns all listening devices connected to
	 the GPIB bus of the given board.
	 This function is not member of the wxGPIB_x class, becauce it
	 should do it's job before you open any GPIB connection.
	 \param the board nummber. Default is the first board (=0). Valid
	 board numbers are 0 and 1.
	 \return -1 if an error occurred, otherwise a setting bit for
	 each listener address. Bit0 is always 0 (address 0 isn't valid,
	 Bit1 means address 1, Bit2 address 2 and so on...
	*/
    static int FindListeners(int board = 0);
};

#endif
