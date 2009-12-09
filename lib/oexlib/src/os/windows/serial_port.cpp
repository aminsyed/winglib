/*------------------------------------------------------------------
// serial_port.cpp
//
// Copyright (c) 1997
// Robert Umbehant
// winglib@wheresjames.com
// http://www.wheresjames.com
//
// Redistribution and use in source and binary forms, with or
// without modification, are permitted for commercial and
// non-commercial purposes, provided that the following
// conditions are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * The names of the developers or contributors may not be used to
//   endorse or promote products derived from this software without
//   specific prior written permission.
//
//   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
//   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
//   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
//   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
//   NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
//   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
//   EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//----------------------------------------------------------------*/

#include "../../../oexlib.h"
#include "std_os.h"

OEX_USING_NAMESPACE
using namespace OEX_NAMESPACE::os;

// OS specific serial data
struct SSerialPortSettings
{
	/// Port handle
	HANDLE			hPort;

	/// Device control block
	DCB				dcb;
};


CSerialPort::CSerialPort()
{
	m_nError = 0;

	m_nTxBufferSize = eDefaultTxBufferSize;
	m_nRxBufferSize = eDefaultRxBufferSize;

	// Allocate settings object
	m_pSettings = OexAllocNew< SSerialPortSettings >( 1 ) ;
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS )
		return;

	pS->hPort = 0;

	SetDefaults();
}

CSerialPort::~CSerialPort()
{
	Destroy();

	// Ditch the settings object
	if ( m_pSettings )
	{	OexAllocDelete< SSerialPortSettings >( (SSerialPortSettings*)m_pSettings );
		m_pSettings = oexNULL;
	} // end if
}

void CSerialPort::Destroy()
{
	m_sPort.Destroy();

	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS )
		return;
}

oexBOOL CSerialPort::Open( oexCSTR x_pPort )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || !x_pPort )
		return oexFALSE;

	// Lose previous port
	Destroy();

	// Open the port
	pS->hPort = CreateFile(	x_pPort, 
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if ( INVALID_HANDLE_VALUE == pS->hPort )
	{	m_nError = GetLastError();
		Destroy();
		return oexFALSE;
	} // end if

	if ( !SetStatus() )
	{	Destroy();
		return oexFALSE;
	} // end if

	// Save the port name
	m_sPort = x_pPort;

	// Purge the port
	Purge();

	return oexTRUE;
}

oexBOOL CSerialPort::SetStatus()
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return oexFALSE;

	unsigned long ulCommErr;
	oexBOOL success = SetCommState( pS->hPort, &pS->dcb );
	oexRESULT err = GetLastError();

	SetupComm( pS->hPort, m_nRxBufferSize, m_nTxBufferSize );

	SetTimeout();

	ClearCommBreak( pS->hPort );
	ClearCommError( pS->hPort, &ulCommErr, NULL );

	return oexTRUE;

}

oexBOOL CSerialPort::SetTimeout( oexINT x_to )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return oexFALSE;

	// Initialize with current values
	COMMTIMEOUTS cto; oexZero( cto );
	GetCommTimeouts( pS->hPort, &cto );

	// Update timeout params
	cto.ReadIntervalTimeout = x_to;
	cto.ReadTotalTimeoutMultiplier = 0;
	cto.ReadTotalTimeoutConstant = x_to;
	cto.WriteTotalTimeoutMultiplier = 3;
	cto.WriteTotalTimeoutConstant = x_to;

	// Set new timeout values
	return SetCommTimeouts( pS->hPort, &cto );
}


oexBOOL CSerialPort::IsPort()
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return oexFALSE;

	return oexTRUE;
}

oexBOOL CSerialPort::Purge()
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return oexFALSE;

	PurgeComm( pS->hPort, PURGE_TXCLEAR | PURGE_TXABORT );
	PurgeComm( pS->hPort, PURGE_RXCLEAR | PURGE_RXABORT );

	return oexTRUE;
}

oexBOOL CSerialPort::SetDefaults()
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS )
		return oexFALSE;
						   
	// Initialize structure to zero
	oexZero( pS->dcb );

	// Set the structure size
	pS->dcb.DCBlength = sizeof( pS->dcb );	

	// Common defaults
	pS->dcb.ByteSize = 8;
	pS->dcb.BaudRate = eBaud_9600;
	pS->dcb.Parity = eParity_None;
	pS->dcb.StopBits = eStopBits_One;
	pS->dcb.fDtrControl = eXCtrlDisable;
	pS->dcb.fRtsControl = eXCtrlDisable;

	// Not so common settings
	pS->dcb.fBinary = FALSE;
	pS->dcb.fParity = FALSE;
	pS->dcb.fOutxCtsFlow = FALSE;
	pS->dcb.fOutxDsrFlow = FALSE;
	pS->dcb.fDsrSensitivity = FALSE;
	pS->dcb.fOutX = FALSE;
	pS->dcb.fInX = FALSE;
	pS->dcb.fErrorChar = FALSE;
	pS->dcb.fNull = FALSE;
	pS->dcb.fAbortOnError = FALSE;

	return oexTRUE;
}

oexBOOL CSerialPort::ClearErrors()
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return oexFALSE;

	unsigned long ulCommErr;

	ClearCommBreak( pS->hPort );
	ClearCommError( pS->hPort, &ulCommErr, NULL );

	return oexTRUE;
}

oexSIZE_T CSerialPort::Write( oexCPVOID x_pBuf, oexSIZE_T x_nSize )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !x_pBuf || !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return 0;
	
	// Attempt to write the data
	DWORD dwWritten = (DWORD)x_nSize;
	if ( !WriteFile( pS->hPort, x_pBuf, (DWORD)x_nSize, &dwWritten, oexNULL ) ) 
	{	m_nError = GetLastError();
		return 0;
	} // end if

	return (oexSIZE_T)dwWritten;
}

/// Reads data from the port
oexSIZE_T CSerialPort::Read( oexPVOID x_pBuf, oexSIZE_T x_nSize )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !x_pBuf || !x_nSize || !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return 0;
	
	// Attempt to write the data
	DWORD dwRead = (DWORD)x_nSize;
	if ( !ReadFile( pS->hPort, x_pBuf, (DWORD)x_nSize, &dwRead, oexNULL ) ) 
		return 0;

	// Did we read anything?
	return (oexSIZE_T)dwRead;
}

CStr8 CSerialPort::Read( oexSIZE_T x_nSize )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return CStr8();
	
	CStr8 s;
	oexSTR8 p = s.Allocate( x_nSize );
	if ( !p )
		return CStr8();

	// Attempt to write the data
	DWORD dwRead = (DWORD)x_nSize;
	if ( !ReadFile( pS->hPort, p, (DWORD)x_nSize, &dwRead, oexNULL ) ) 
		return CStr8();

	// Set length of data read
	s.SetLength( dwRead );

	// Did we read anything?
	return s;
}

/// Reads data into a string object
oexSIZE_T CSerialPort::ReadBin( CBin *bin, oexSIZE_T x_nSize )
{
	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !bin || !x_nSize || !pS || INVALID_HANDLE_VALUE == pS->hPort )
		return 0;

	// Ensure space
	if ( bin->Size() < x_nSize )
		if ( !bin->Allocate( x_nSize ) )
			return 0;

	// Attempt to write the data
	DWORD dwRead = (DWORD)x_nSize;
	if ( !ReadFile( pS->hPort, bin->_Ptr(), (DWORD)x_nSize, &dwRead, oexNULL ) ) 
		return 0;

	bin->setUsed( dwRead );

	// Did we read anything?
	return (oexSIZE_T)dwRead;
}

oexINT CSerialPort::getByteSize()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.ByteSize;
}

void CSerialPort::setByteSize( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.ByteSize = v;
}

oexINT CSerialPort::getBaudRate()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.BaudRate;
}

void CSerialPort::setBaudRate( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.BaudRate = v;
}

oexINT CSerialPort::getStopBits()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.StopBits;
}

void CSerialPort::setStopBits( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.StopBits = v;
}

oexINT CSerialPort::getParity()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.Parity;
}

void CSerialPort::setParity( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.Parity = v;
}

oexINT CSerialPort::getRtsCtrl()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.fRtsControl;
}

void CSerialPort::setRtsCtrl( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.fRtsControl = v;
}

oexINT CSerialPort::getDtrCtrl()
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( !pS ) return -1;
	return (oexINT)pS->dcb.fDtrControl;
}

void CSerialPort::setDtrCtrl( oexINT v )
{	SSerialPortSettings *pS = (SSerialPortSettings*)m_pSettings;
	if ( pS ) pS->dcb.fDtrControl = v;
}
