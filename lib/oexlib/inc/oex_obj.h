/*------------------------------------------------------------------
// obj_info.h
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

#pragma once

/// Special object functions
namespace obj
{
	// For defining type sizes in bits
#	define OSZBITS( b ) ( ( ( b & 0x7 ) << 8 ) | ( ( ( b & 0xff ) >> 3 ) + ( ( b & 0x7 ) ? 1 : 0 ) ) )

    // iii Unfortunately, Doxygen craps out on these next
    //     function templates.

	// Data type masks
    enum
    {
		// Data element size in bytes
		eTypeSize			= 0x00ff,

		// Extra for size in bits
		eTypeSizeBits		= 0x0700,

		// Elements are signed values
		eTypeSigned			= 0x0800,

		// Array element
		eTypeElement		= 0x1000,

		// Floating point
		eTypeFloating		= 0x2000,

		// Array
		eTypeArray			= 0x4000,

		// Null terminated
		eTypeNullTerm		= 0x8000,

	};

	enum
	{
		// Types
        tInvalid        = 0,

        tBin            = ( eTypeArray | 1 ),

        tInt            = ( eTypeSigned | sizeof( oexINT ) ),

        tUInt           = ( sizeof( oexUINT ) ),

        tLong           = ( eTypeSigned | sizeof( oexLONG ) ),

        tULong          = ( sizeof( oexULONG ) ),

        tLongLong       = ( eTypeSigned | sizeof( oexINT64 ) ),

        tULongLong      = ( sizeof( oexUINT64 ) ),

		tInt1           = ( eTypeSigned | OSZBITS( 1 ) ),

        tUInt1          = ( OSZBITS( 1 ) ),

		tInt2           = ( eTypeSigned | OSZBITS( 2 ) ),

        tUInt2          = ( OSZBITS( 2 ) ),

		tInt4           = ( eTypeSigned | OSZBITS( 4 ) ),

        tUInt4          = ( OSZBITS( 4 ) ),

		tInt8           = ( eTypeSigned | 1 ),

        tUInt8          = ( 1 ),

        tInt16          = ( eTypeSigned | 2 ),

        tUInt16         = ( 2 ),

        tInt24          = ( eTypeSigned | 3 ),

        tUInt24         = ( 3 ),

        tInt32          = ( eTypeSigned | 4 ),

        tUInt32         = ( 4 ),

        tInt64          = ( eTypeSigned | 8 ),

        tUInt64         = ( 8 ),

        tInt128         = ( eTypeSigned | 16 ),

        tUInt128        = ( 16 ),

        tChar           = ( eTypeSigned | eTypeElement | sizeof( oexCHAR ) ),

        tUChar          = ( eTypeElement | sizeof( oexUCHAR ) ),

        tChar8          = ( eTypeSigned | eTypeElement | 1 ),

        tUChar8         = ( eTypeElement | 1 ),

        tChar16         = ( eTypeSigned | eTypeElement | 2 ),

        tUChar16        = ( eTypeElement | 2 ),

        tChar24         = ( eTypeSigned | eTypeElement | 3 ),

        tUChar24        = ( eTypeElement | 3 ),

        tChar32         = ( eTypeSigned | eTypeElement | 4 ),

        tUChar32        = ( eTypeElement | 4 ),

        tChar64         = ( eTypeSigned | eTypeElement | 8 ),

        tUChar64        = ( eTypeElement | 8 ),

        tFloat8          = ( eTypeFloating | 1 ),

        tFloat16          = ( eTypeFloating | 2 ),

        tFloat24          = ( eTypeFloating | 3 ),

        tFloat32          = ( eTypeFloating | 4 ),

        tFloat64          = ( eTypeFloating | 8 ),

        tFloat128         = ( eTypeFloating | 16 ),

        tFloat          = ( eTypeFloating | sizeof( oexFLOAT ) ),

        tDouble         = ( eTypeFloating | sizeof( oexDOUBLE ) ),

        tLongDouble     = ( eTypeFloating | sizeof( oexLONGDOUBLE ) ),

        tStr			= ( eTypeArray | eTypeNullTerm | sizeof( oexTCHAR ) ),

		tStrW           = ( eTypeArray | eTypeNullTerm | sizeof( oexCHARW ) ),

        tStr8           = ( eTypeArray | eTypeNullTerm | 1 ),

        tStr16          = ( eTypeArray | eTypeNullTerm | 2 ),

        tStr24          = ( eTypeArray | eTypeNullTerm | 3 ),

        tStr32          = ( eTypeArray | eTypeNullTerm | 4 ),

        tStr64          = ( eTypeArray | eTypeNullTerm | 8 ),

        tStr128          = ( eTypeArray | eTypeNullTerm | 16 ),

        tGuid           = ( 16 )
    };

	template< typename T >
		oexSIZE_T StaticSize( T t )
	{
		return ( t & eTypeSize );
	}

	template< typename T >
		oexSIZE_T StaticSizeBits( T t )
	{	return ( t & eTypeSizeBits )
			   ? ( ( ( ( t & eTypeSize ) - 1 ) << 3 ) | ( ( t & eTypeSizeBits ) >> 8 ) )
			   : ( ( t & eTypeSize ) << 3 );
	}

	//==============================================================
	// Size()
	//==============================================================
	/// Returns the size of the object
	/**
		\param [in] x_obj		-	The object whose size will be
								    returned.
	*/
	template < typename T >
		static oexSIZE_T Size( oexCONST T *x_obj )
        {   return sizeof( T ); }

    // +++ These next two are dangerous since they could be invoked by actual
	//     pointers to chars as opposed to strings.  Please fix!
    template<> oexITS oexSIZE_T Size< oexCHAR8 >( oexCONST oexCHAR8 *x_obj )
    {   return zstr::Length( x_obj ); }

    template<> oexITS oexSIZE_T Size< oexCHARW >( oexCONST oexCHARW *x_obj )
    {   return zstr::Length( x_obj ) * sizeof( oexCHARW ); }

	template < typename T >
		static oexSIZE_T Size( oexCONST T **x_obj )
        {   return sizeof( T ); }

    template<> oexITS oexSIZE_T Size< oexCHAR8 >( oexCONST oexCHAR8 **x_obj )
    {   return zstr::Length( *x_obj ); }

    template<> oexITS oexSIZE_T Size< oexCHARW >( oexCONST oexCHARW **x_obj )
    {   return zstr::Length( *x_obj ) * sizeof( oexCHARW ); }

	//==============================================================
	// Terminator()
	//==============================================================
	/// Returns the size of the object terminator
	/**
		\param [in] x_obj		-	Returns the length of a strings
                                    NULL terminator.  Returns zero
                                    for other objects.
	*/
	template < typename T >
		static oexSIZE_T Terminator( oexCONST T *x_obj )
        {   return 0; }

    template<> oexITS oexSIZE_T Terminator< oexCSTR8 >( oexCONST oexCSTR8 *x_obj )
    {   return 1; }

    template<> oexITS oexSIZE_T Terminator< oexCSTRW >( oexCONST oexCSTRW *x_obj )
    {   return 2; }

	//==============================================================
	// Ptr()
	//==============================================================
	/// Returns a pointer to the memory
	/**
		\param [in] x_ptr		-	The object whose pointer will be
									returned.
	*/
	template < typename T >
		static oexPVOID Ptr( oexCONST T *x_ptr )
        {   return (oexPVOID)x_ptr; }

	template < typename T >
		static oexPVOID Ptr( oexCONST T **x_ptr )
        {   return (oexPVOID)x_ptr; }

    template<> oexITS oexPVOID Ptr< oexCHAR8 >( oexCONST oexCHAR8 **x_ptr )
    {   return (oexPVOID)*x_ptr; }

    template<> oexITS oexPVOID Ptr< oexCHARW >( oexCONST oexCHARW **x_ptr )
    {   return (oexPVOID)*x_ptr; }

	//==============================================================
	// Compare()
	//==============================================================
    /// Compares objects
    template < typename T >
        static oexBOOL Compare( oexCONST T &k1,oexCONST T &k2 )
        {   return !os::CSys::MemCmp( &k1, &k2, sizeof( T ) );
        }

    template<> oexITS oexBOOL Compare< oexCSTR8 >( oexCONST oexCSTR8 &k1, oexCONST oexCSTR8 &k2 )
    {   return !str::Compare( k1, zstr::Length( k1 ), k2, zstr::Length( k2 ) ); }

    template<> oexITS oexBOOL Compare< oexCSTRW >( oexCONST oexCSTRW &k1, oexCONST oexCSTRW &k2 )
    {   return !str::Compare( k1, zstr::Length( k1 ), k2, zstr::Length( k2 ) ); }


    // Invalid param type
    template< typename T >
        static oexUINT GetType()
        {   oexASSERT( 0 );
            return tInvalid;
        }

    template<> oexITS oexUINT GetType< oexCHAR >()
    {	switch( sizeof( oexCHAR ) )
		{	case 1 : return tChar8;
			case 2 : return tChar16;
			case 4 : return tChar32;
			case 8 : return tChar64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexUCHAR >()
    {	switch( sizeof( oexUCHAR ) )
		{	case 1 : return tUChar8;
			case 2 : return tUChar16;
			case 4 : return tUChar32;
			case 8 : return tUChar64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexCHARW >()
    {	switch( sizeof( oexCHARW ) )
		{	case 1 : return tChar8;
			case 2 : return tChar16;
			case 4 : return tChar32;
			case 8 : return tChar64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexSHORT >()
    {	switch( sizeof( oexSHORT ) )
		{	case 1 : return tInt8;
			case 2 : return tInt16;
			case 4 : return tInt32;
			case 8 : return tInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexUSHORT >()
    {	switch( sizeof( oexUSHORT ) )
		{	case 1 : return tUInt8;
			case 2 : return tUInt16;
			case 4 : return tUInt32;
			case 8 : return tUInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexINT >()
    {	switch( sizeof( oexINT ) )
		{	case 1 : return tInt8;
			case 2 : return tInt16;
			case 4 : return tInt32;
			case 8 : return tInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexUINT >()
    {	switch( sizeof( oexUINT ) )
		{	case 1 : return tUInt8;
			case 2 : return tUInt16;
			case 4 : return tUInt32;
			case 8 : return tUInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexLONG >()
    {	switch( sizeof( oexLONG ) )
		{	case 1 : return tInt8;
			case 2 : return tInt16;
			case 4 : return tInt32;
			case 8 : return tInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexULONG >()
    {	switch( sizeof( oexULONG ) )
		{	case 1 : return tUInt8;
			case 2 : return tUInt16;
			case 4 : return tUInt32;
			case 8 : return tUInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexINT64 >()
    {	switch( sizeof( oexLONG ) )
		{	case 1 : return tInt8;
			case 2 : return tInt16;
			case 4 : return tInt32;
			case 8 : return tInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexUINT64 >()
    {	switch( sizeof( oexULONG ) )
		{	case 1 : return tUInt8;
			case 2 : return tUInt16;
			case 4 : return tUInt32;
			case 8 : return tUInt64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexFLOAT >()
    {   return tFloat; }

    template<> oexITS oexUINT GetType< oexDOUBLE >()
    {   return tDouble; }

    template<> oexITS oexUINT GetType< oexLONGDOUBLE >()
    {   return tDouble; }

    template<> oexITS oexUINT GetType< oexCONST oexCHAR * >()
    {	switch( sizeof( oexCHAR ) )
		{	case 1 : return tStr8;
			case 2 : return tStr16;
			case 4 : return tStr32;
			case 8 : return tStr64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexCONST oexCHARW * >()
    {	switch( sizeof( oexCHARW ) )
		{	case 1 : return tStr8;
			case 2 : return tStr16;
			case 4 : return tStr32;
			case 8 : return tStr64;
		} // end switch
		return tInvalid;
    }

    template<> oexITS oexUINT GetType< oexGUID >()
    {   return tGuid; }

};
