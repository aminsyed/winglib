// gdc_chart.cpp

#include "stdafx.h"

#include "gdc.h"
#include "gdchart.h"
#include "gd/gd.h"

/// Unfortunately, the gdchart library is not thread safe
oexLock CGdcChart::m_lock;

CGdcChart::CGdcChart()
{_STT();
}

CGdcChart::~CGdcChart()
{_STT();
}

#define MAX_DIMENSIONS		9

sqbind::CSqMulti CGdcChart::CreateChart( const sqbind::stdString &x_sType,
									  	 const sqbind::stdString &x_sParams,
									  	 const sqbind::stdString &x_sData,
									  	 sqbind::CSqImage *x_pImg )
{_STT();
	if ( !x_pImg )
		return oexT( "" );

	sqbind::CSqMulti mParams( x_sParams.c_str() );
	sqbind::CSqMulti mData( x_sData.c_str() );

	int nDataPts = 0;
	oex::TMem< oex::oexCSTR > memLabels;
	oex::TMem< float > memPts[ MAX_DIMENSIONS + 1 ];

	int nDimensions = 0;
	for ( sqbind::CSqMulti::iterator it = mData.begin();
		  nDimensions < MAX_DIMENSIONS && mData.end() != it; it++ )
	{
		// Use smallest size
		if ( !nDataPts )
		{
			nDataPts = it->second[ oexT( "data" ) ].size();
			if ( !nDataPts )
			{	oexERROR( 0, oexT( "No data" ) );
				return oexT( "" );
			} // end if

			if ( !memLabels.OexNew( nDataPts ).Ptr() )
			{	oexERROR( 0, oexMks( oexT( "Unable to allocate memory block, size = " ), nDataPts ) );
				return oexT( "" );
			} // end if

		} // end if

		// Allocate space for data
		if ( !memPts[ nDimensions ].OexNew( nDataPts ).Ptr() )
		{	oexERROR( 0, oexMks( oexT( "Unable to allocate memory block, size = " ), nDataPts ) );
			return oexT( "" );
		} // end if
		memPts[ nDimensions ].Zero();

		// Get data points
		int i = 0;
		for ( sqbind::CSqMulti::iterator itPt = it->second[ oexT( "data" ) ].begin();
			  i < nDataPts && it->second[ oexT( "data" ) ].end() != itPt; itPt++ )
		{
			// Grab pointer to label on first dimension
			if ( !nDimensions )
				memLabels[ i ] = itPt->first.c_str();

			// Grab point value
			memPts[ nDimensions ][ i++ ] = oexStrToFloat( itPt->second.str().c_str() );

		} // end for

		// Count a dimension
		nDimensions++;

	} // end for

	// GDChart sometimes tries to read from an extra dimension???
	if ( !memPts[ nDimensions ].OexNew( nDataPts ).Ptr() )
	{	oexERROR( 0, oexMks( oexT( "Unable to allocate memory block, size = " ), nDataPts ) );
		return oexT( "" );
	} // end if
	memPts[ nDimensions ].Zero();

	// Lock the gdchart library
	oexAutoLock ll( m_lock );
	if ( !ll.IsLocked() )
		return oexT( "" );

	// Settings
	GDC_generate_gif = FALSE;
	GDC_hold_img = GDC_EXPOSE_IMAGE;

	// Defaults
	GDC_BGColor = 0xffffff;
	GDC_GridColor = 0x000000;
	GDC_LineColor = 0x000000;
	GDC_PlotColor = 0x00ff00;
	GDC_ylabel_fmt = (char*)"%.3f";

	unsigned long _SetColor[ MAX_DIMENSIONS + 9 ] = { 0xff0000, 0x00ff00, 0x00ffff,
													  0xffff00, 0x0000ff, 0xff00ff,
													  0xffffff, 0xa0a0a0, 0x808080 };
	GDC_SetColor = _SetColor;

	// User settings
	if ( mParams.isset( oexT( "col_bg" ) ) )
		GDC_BGColor = oexHtmlToRgb( mParams[ oexT( "col_bg" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_grid" ) ) )
		GDC_GridColor = oexHtmlToRgb( mParams[ oexT( "col_grid" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_labels" ) ) )
		GDC_LineColor = oexHtmlToRgb( mParams[ oexT( "col_labels" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_plot" ) ) )
		GDC_PlotColor = GDC_SetColor[ 0 ] = GDC_SetColor[ 1 ] = GDC_SetColor[ 2 ] = GDC_SetColor[ 3 ]
			= oexHtmlToRgb( mParams[ oexT( "col_plot" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_plot2" ) ) )
		GDC_SetColor[ 1 ] = GDC_SetColor[ 2 ] = GDC_SetColor[ 3 ]
			= oexHtmlToRgb( mParams[ oexT( "col_plot2" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_plot3" ) ) )
		GDC_SetColor[ 2 ] = GDC_SetColor[ 3 ]
			= oexHtmlToRgb( mParams[ oexT( "col_plot3" ) ].str().c_str() );
	if ( mParams.isset( oexT( "col_plot4" ) ) )
		GDC_SetColor[ 3 ]
			= oexHtmlToRgb( mParams[ oexT( "col_plot4" ) ].str().c_str() );
	
	// Write out the graph to the pipe
	switch ( nDimensions )
	{
	case 1:
		out_graph( oexStrToLong( mParams[ oexT( "width" ) ].str().c_str() ),
	   			   oexStrToLong( mParams[ oexT( "height" ) ].str().c_str() ),
				   0, (GDC_CHART_T)oexStrToLong( mParams[ oexT( "type" ) ].str().c_str() ),
				   nDataPts, (char**)memLabels.Ptr(), nDimensions,
				   memPts[ 0 ].Ptr(), memPts[ 1 ].Ptr() );
		break;

	case 2:
		out_graph( oexStrToLong( mParams[ oexT( "width" ) ].str().c_str() ),
	   			   oexStrToLong( mParams[ oexT( "height" ) ].str().c_str() ),
				   0, (GDC_CHART_T)oexStrToLong( mParams[ oexT( "type" ) ].str().c_str() ),
				   nDataPts, (char**)memLabels.Ptr(), nDimensions,
				   memPts[ 0 ].Ptr(), memPts[ 1 ].Ptr(), memPts[ 2 ].Ptr() );
		break;

	case 3:
		out_graph( oexStrToLong( mParams[ oexT( "width" ) ].str().c_str() ),
	   			   oexStrToLong( mParams[ oexT( "height" ) ].str().c_str() ),
				   0, (GDC_CHART_T)oexStrToLong( mParams[ oexT( "type" ) ].str().c_str() ),
				   nDataPts, (char**)memLabels.Ptr(), nDimensions,
				   memPts[ 0 ].Ptr(), memPts[ 1 ].Ptr(), memPts[ 2 ].Ptr(), memPts[ 3 ].Ptr() );
		break;

	case 4:
		out_graph( oexStrToLong( mParams[ oexT( "width" ) ].str().c_str() ),
	   			   oexStrToLong( mParams[ oexT( "height" ) ].str().c_str() ),
				   0, (GDC_CHART_T)oexStrToLong( mParams[ oexT( "type" ) ].str().c_str() ),
				   nDataPts, (char**)memLabels.Ptr(), nDimensions,
				   memPts[ 0 ].Ptr(), memPts[ 1 ].Ptr(), memPts[ 2 ].Ptr(), memPts[ 3 ].Ptr(),
				   memPts[ 4 ].Ptr() );
		break;

	default :
		oexERROR( 0, oexMks( oexT( "Invalid graph dimensions : " ), nDimensions ) );
		GDC_image = NULL;
		GDC_SetColor = NULL;
		return oexT( "" );
		break;

	} // end switch

	// Get the image buffer info
	gdImagePtr graph = (gdImagePtr)GDC_image;
	if ( !oexCHECK_PTR( graph ) || !oexCHECK_PTR( graph->pixels ) )
	{	oexERROR( 0, oexT( "Inavlid image pointer" ) );
		GDC_image = NULL;
		GDC_SetColor = NULL;
		return oexT( "" );
	} // end if

	// Create image to hold the data
	if ( !x_pImg->isValid()
		 || x_pImg->getWidth() != graph->sx
		 || x_pImg->getHeight() != graph->sy )
		if ( !x_pImg->Create( graph->sx, graph->sy ) )
			return oexT( "" );

	// Image size
	long lSize = x_pImg->Obj().GetImageSize();
	if ( 0 >= lSize )
	{	oexERROR( 0, oexMks( oexT( "Inavlid image size : " ), lSize ).Ptr() );
		GDC_image = NULL;
		GDC_SetColor = NULL;
		return oexT( "" );
	} // end if

	int sw = graph->sx;
	int sh = graph->sy;
	unsigned char **s = graph->pixels;
	unsigned char *d = (unsigned char*)x_pImg->Obj().GetBits();

	for ( int y = 0, u = sh - 1; y < sh; y++, u-- )
		for ( int x = 0; x < sw; x++, d += 3 )
		{
			unsigned char b = s[ u ][ x ];
			d[ 0 ] = graph->red[ b & 0xff ];
			d[ 1 ] = graph->green[ b & 0xff ];
			d[ 2 ] = graph->blue[ b & 0xff ];

		} // end for

	// Save image information
	sqbind::CSqMulti mImg;
	mImg[ oexT( "type" ) ].set( x_sType.c_str() );
	mImg[ oexT( "width" ) ].set( oexMks( graph->sx ).Ptr() );
	mImg[ oexT( "height" ) ].set( oexMks( graph->sy ).Ptr() );
	mImg[ oexT( "size" ) ].set( oexMks( lSize ).Ptr() );

	// Lose the image buffer
	gdImageDestroy( graph );
	GDC_image = NULL;
	GDC_SetColor = NULL;

	return mImg;
}

int CGdcChart::SaveChart(	const sqbind::stdString &x_sFile,
							const sqbind::stdString &x_sParams,
							const sqbind::stdString &x_sData )
{_STT();
	// Create the chart
	sqbind::CSqImage img;
	sqbind::CSqMulti mChart = CreateChart( x_sFile, x_sParams, x_sData, &img );
	if ( !img.isValid() )
		return -10;

	return img.Save( x_sFile, oexNULL );
}
