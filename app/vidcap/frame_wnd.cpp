
#include "stdafx.h"
#include "frame_wnd.h"

/*
	/    \
	|~~~~|
	'-..-'
      ||
	 _||_
	`""""`
*/


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Event table
BEGIN_EVENT_TABLE( CFrameWnd, wxFrame )

	EVT_MENU( CFrameWnd::WXIDM_Exit, CFrameWnd::OnExit )

	EVT_MENU( CFrameWnd::WXIDM_Open, CFrameWnd::OnOpen )

	EVT_PAINT( CFrameWnd::OnPaint )

	EVT_ERASE_BACKGROUND( CFrameWnd::OnEraseBackground )

	EVT_TIMER( wxID_ANY, CFrameWnd::OnTimer )

END_EVENT_TABLE()

CFrameWnd::~CFrameWnd()
{_STT();
	if ( m_pTimer )
	{	delete m_pTimer;
		m_pTimer = NULL;
	} // end if

}

CFrameWnd::CFrameWnd( const wxString& x_sTitle, const wxPoint& x_ptWin, const wxSize& x_sizeWin )
	: wxFrame( (wxFrame*)NULL, -1, x_sTitle, x_ptWin, x_sizeWin,
				wxFULL_REPAINT_ON_RESIZE | wxDEFAULT_FRAME_STYLE )
{_STT();
	m_pTimer = NULL;

//	SetIcon( wxIcon( mondrian_xpm ) );

	wxImage::AddHandler( new wxJPEGHandler );

	wxMenu *pcMenuFile = new wxMenu;
	if ( pcMenuFile )
		pcMenuFile->Append( WXIDM_Open, _T( "&Open" ) ),
		pcMenuFile->AppendSeparator(),
		pcMenuFile->Append( WXIDM_Exit, _T( "E&xit" ) );

	wxMenuBar *pcMenuBar = new wxMenuBar;
	if ( pcMenuBar )
		pcMenuBar->Append( pcMenuFile, _T( "&File" ) ),
		SetMenuBar( pcMenuBar );

	CreateStatusBar();

//	SetStatusText( _T( "This is a status text" ) );

	// Open the capture device
	if ( !m_cCapture.Open( oexVIDSUB_AUTO, 0, 0, 320, 240, 0, 15, oex::oexTRUE ) )
//	if ( !m_cCapture.Open( oexVIDSUB_DSHOW, 0, 0, 320, 240, 24, 15, oex::oexTRUE ) )
//	if ( !m_cCapture.Open( oexVIDSUB_VFW, 0, 0, 320, 240, 24, 15, oex::oexTRUE ) )
		SetStatusText( _T( "Failed to open catpure device" ) );

	else
		SetStatusText( oexLocalTimeStr( oexT( "%W, %B %D, %Y - %h:%m:%s %A" ) ).Ptr() );

	if ( m_cCapture.IsOpen() )
	{	//m_cCapture.WaitForFrame();
		m_cCapture.StartCapture();

		// +++ Just a hack till we get the callbacks going
		m_pTimer = new wxTimer( this );
		m_pTimer->Start( 1000 / 15 );

	} // end if


//	wxMessageBox( oex::os::CTrace::GetBacktrace().Ptr(), _T( "Stack Trace" ), wxOK, this );

//	SetStatusText( m_v4lCap.GetLastErrorStr().Ptr() );
}

void CFrameWnd::OnExit( wxCommandEvent& x_wxCe )
{_STT();
	Close( TRUE );
}

void CFrameWnd::OnOpen( wxCommandEvent& x_wxCe )
{_STT();
	wxFileDialog fd( this, _T( "Choose AVI file" ),
					 wxEmptyString, wxEmptyString,
					 _T( "AVI Files (*.avi)|*.avi" )
					 _T( "|All Files (*.*)|*.*" ),
					 wxFD_OPEN, wxDefaultPosition );

	if ( wxID_OK != fd.ShowModal() )
		return;
}

void CFrameWnd::OnPaint( wxPaintEvent& x_wxPe )
{_STT();
	wxPaintDC dc( this );

	wxSize sizeClient = GetClientSize();

	wxRect rect;
	rect.x = 0; rect.y = 0;
	rect.width = sizeClient.GetWidth();
	rect.height = sizeClient.GetHeight();

	if ( !m_cCapture.IsOpen()
		 || !m_cCapture.WaitForFrame( 3000 )
		 || !m_cCapture.GetBuffer() )
	{
		dc.SetBrush( wxBrush( wxColor( 255, 0, 0 ) ) );
		dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
		return;
	} // end if

//	dc.SetBrush( wxBrush( wxColor( 0, 255, 0 ) ) );
//	dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );

	wxImage imgMem( m_cCapture.GetWidth(), m_cCapture.GetHeight(),
		            (unsigned char*)m_cCapture.GetBuffer(), true );

	// Go ahead and release the video buffer
	m_cCapture.ReleaseFrame();

	StretchDraw( dc, imgMem, rect );
/*

	wxBitmap	bmMem( imgMem );


	// Create a memory DC
	wxMemoryDC dcMem;
	dcMem.SelectObject( bmMem );
	dc.Blit( 0, 0, dcMem.GetSize().GetWidth(), dcMem.GetSize().GetHeight(), &dcMem, 0, 0 );

/ *

	// Attempt to open the device
	int dev = open( "/dev/video0" / *oexStrToMbPtr( oex::CStr().Fmt( oexT( "/dev/video%d" ), 0 ).Ptr() ) * /, O_RDONLY );
	if ( -1 == dev )
	{	dc.SetBrush( wxBrush( wxColor( 255, 255, 0 ) ) );
		dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
		return;
	} // end if

	close( dev );
* /

/ *
    video_capability vc;
	oexZeroMemory( &vc, sizeof( vc ) );
	if( -1 == ioctl( dev, VIDIOCGCAP, &vc ) )
	{	dc.SetBrush( wxBrush( wxColor( 255, 0, 0 ) ) );
		dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
		return;
	} // end if

	// Video buffer
	video_buffer vb;
	oexZeroMemory( &vb, sizeof( vb ) );

	if( -1 == ioctl( dev, VIDIOCGMBUF, &vb ) )
	{	dc.SetBrush( wxBrush( wxColor( 255, 0, 255 ) ) );
		dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
		return;
	} // end if

//  char vbuf[ 320 * 240 * 3 ];
//	read(

	video_window vw;
	oexZeroMemory( &vw, sizeof( vw ) );
	vw.width = 320;
	vw.height = 240;
	vw.

	if( -1 == ioctl( dev, VIDIOCSWIN, &vw ) )
	{	dc.SetBrush( wxBrush( wxColor( 255, 0, 255 ) ) );
		dc.DrawRectangle( rect.x, rect.y, rect.width, rect.height );
		return;
	} // end if
* /

//    struct video_picture vid_pic;
//    struct video_window vid_win;
//    struct video_mbuf vid_buf;
//    struct video_mmap vid_map;

/ *
	wxImage		imgMem( 100, 100, (unsigned char*)pData, true );
	wxBitmap	bmMem( imgMem );

	// Create a memory DC
	wxMemoryDC dcMem;
	dcMem.SelectObject( bmMem );
	dc.Blit( 0, 0, 100, 100, &dcMem, 0, 0 );
*/
//	StretchDraw( dc, m_cWxImg, rect );

}

void CFrameWnd::OnTimer( wxTimerEvent &x_wxTe )
{_STT();
	Refresh();

}


void CFrameWnd::OnEraseBackground( wxEraseEvent& x_wxEe )
{_STT();

}

bool CFrameWnd::StretchDraw(wxDC &x_dc, wxImage &x_img, wxRect &x_rect)
{_STT();
    if ( !x_img.Ok() ) return FALSE;

	// The slow but portable way...
	wxImage cWxImage = x_img.Copy();
	cWxImage.Rescale( x_rect.GetWidth(), x_rect.GetHeight() );
	x_dc.DrawBitmap( cWxImage, x_rect.x, x_rect.y );

	return TRUE;
}
