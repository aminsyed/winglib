
_self.load_module( "ffmpeg", "" );

local rec_avi = 0;
local rec_enc = 0;
local rec_frame = 0;

local in_file = _self.get( "/", "cmdline.1" );
local out_file = in_file + ".avi";
local fps = _self.tolong( _self.get( "/", "cmdline.2" ) );
if ( 0 >= fps || 100 < fps ) fps = 15;

_self.echo( "input  : " + in_file );
_self.echo( "output : " + out_file );
_self.echo( "fps    : " + fps );

local data = CSqFile().get_contents_bin( in_file, 0 );
_self.echo( "fsize  : " + data.getUsed() );

local frame = CSqBinary(), inf = CSqMulti();
local i = 0, start = 0, end = 0, max = data.getUsed();

local img = CSqImage();

while ( 1 )
{
	// Find beginning and end of image
	start = data.findUSHORT( 0xd8ff, end, 0 );
	end = data.findUSHORT( 0xd9ff, start, 0 );

	// Did we find an image?
	if ( start == data.failed() || end == data.failed() )
	{	_self.echo( "Extracted " + i + " images." );
		return;
	} // end if

	end += 2;
	_self.echo( i + " : start = " + start + ", end = " + end );

	if ( data.Sub( frame, start, end - start ) )
	{
		if ( img.Decode( "jpg", frame ) )
		{
			if ( !rec_enc )
			{
				local width = img.getWidth();
				local height = img.getHeight();

				rec_avi = CFfContainer();
				rec_enc = CFfEncoder();
				rec_frame = CSqBinary();

				if ( !rec_avi.Create( out_file, "", CSqMulti() ) )
					_self.echo( "Failed to create avi" );
				else if ( 0 > rec_avi.AddVideoStream( CFfDecoder().LookupCodecId( "MP42" ), width, height, fps ) )
					_self.echo( "Failed to add video stream" );
				else if ( !rec_avi.InitWrite() )
					_self.echo( "Failed to initiailze avi" );

				else if ( !rec_enc.Create( rec_avi.getVideoCodecId(), CFfConvert().PIX_FMT_YUV420P,
										   width, height, fps, 1000000, CSqMulti() ) )
					_self.echo( "Failed to create encoder" );

			} // end if

			if ( rec_enc )
			{
				if ( !rec_enc.EncodeImage( img, rec_frame, inf ) )
					_self.echo( "Failed to encode frame" );

				else if ( !rec_avi.WriteFrame( rec_frame, inf ) )
					_self.echo( "Failed to write to avi file" );

			} // end if

		} // end if
	} // end if

	i++;

} // end while

