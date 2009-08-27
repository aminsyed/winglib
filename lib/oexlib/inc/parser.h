/*------------------------------------------------------------------
// parser.h
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

// Special List types
typedef TList< CStr > CStrList;
typedef TList< CStrW > CStrListW;
typedef TList< CStr8 > CStrList8;
typedef TList< CStr16 > CStrList16;
typedef TList< CStr32 > CStrList32;

typedef TAssoList< CStr, CStr > CStrAssoList;
typedef TAssoList< CStrW, CStrW > CStrAssoListW;
typedef TAssoList< CStr8, CStr8 > CStrAssoList8;
typedef TAssoList< CStr16, CStr16 > CStrAssoList16;
typedef TAssoList< CStr32, CStr32 > CStrAssoList32;

class CParser
{
private:

	CParser() {}

	virtual ~CParser() {}

public:

	/// Parsers a list by breaking it at separator strings
    template < typename T >
        static TList< TStr< T > > Explode( oexCONST T * pStr, oexCONST T * pSep )
	{	return Explode( pStr, zstr::Length( pStr ), pSep, zstr::Length( pSep ) ); }

    template < typename T >
        static TList< TStr< T > > Explode( TStr< T > sStr, oexCONST T * pSep )
	{	return Explode( sStr.Ptr(), sStr.Length(), pSep, zstr::Length( pSep ) ); }

    template < typename T >
        static TList< TStr< T > > Explode( TStr< T > sStr, TStr< T > sSep )
	{	return Explode( sStr.Ptr(), sStr.Length(), sSep.Ptr(), sSep.Length() ); }

    template < typename T >
        static TList< TStr< T > > Explode( oexCONST T *x_pStr, oexUINT x_uSize, oexCONST T *x_pSep, oexUINT x_uSep )
    {
	    TList< TStr< T > > lst;
	    if ( !oexVERIFY_PTR( x_pStr ) )
		    return lst;

	    if ( !x_pSep ) x_uSep = 0;
	    else if ( !oexVERIFY_PTR( x_pSep ) )
		    return lst;

	    // Breaking down to chars?
	    if ( !x_uSep )
	    {	while ( *x_pStr )
			    lst << *x_pStr, x_pStr++;
		    return lst;
	    } // end if

	    oexUINT i = 0;
	    while ( x_uSize )
	    {
		    // Separator?
		    if ( !str::CompareLen( &x_pStr[ i ], x_uSize, x_pSep, x_uSep, x_uSep ) )
		    {	lst << CStr( x_pStr, 0, i );
			    x_pStr = &x_pStr[ i + x_uSep ];
			    x_uSize -= x_uSep; i = 0;
		    } // end if

		    else i++, x_uSize--;

	    } // end while

	    // Add last item
	    if ( i ) lst << CStr( x_pStr, 0, i );

	    return lst;
    }

	template < typename T_LIST >
		static CStr Implode( oexCONST T_LIST &lst, oexCSTR pSep )
	{	CStr str;
		for ( typename T_LIST::iterator it; lst.Next( it ); )
		{	if ( str.Length() ) str << pSep;
			str << it->ToString();
		} // end for
		return str;
	}

/*
	template < typename T >
		static TStr< T > Implode( oexCONST TList< TStr< T > > &lst, oexCONST T *pSep )
	{	TStr< T > str;
		for ( typename TList< TStr< T > >::iterator it; lst.Next( it ); )
		{	if ( str.Length() ) str << pSep;
			str << it->ToString();
		} // end for
		return str;
	}
*/

	/// Returns a list of tokens from a string
	static CStrList GetTokens( oexCSTR pStr, oexCSTR pValid );

	/// Splits a string into an array by cutting on any character in pSep
    template < typename T >
        static TList< TStr< T > > Split( oexCONST T *x_pStr, oexUINT x_uSize, oexCONST T *x_pSep )
    {
	    TList< TStr< T > > lst;
	    if ( !oexVERIFY_PTR( x_pStr ) )
		    return lst;

	    oexUINT x_uSep = 0;
	    if ( !x_pSep ) x_uSep = 0;
	    else if ( !oexVERIFY_PTR( x_pSep ) )
		    return lst;
	    else x_uSep = zstr::Length( x_pSep );

	    // Breaking down to chars?
	    if ( !x_uSep )
	    {	while ( *x_pStr )
			    lst << *x_pStr, x_pStr++;
		    return lst;
	    } // end if

	    while ( x_uSize )
	    {
		    // Find a separator
		    oexINT nSep = str::FindCharacters( x_pStr, x_uSize, x_pSep, x_uSep );

		    if ( 0 > nSep )
		    {	lst << x_pStr;
			    return lst;
		    } // end if

		    else
		    {
		    	if ( nSep )
		    	{	x_uSize -= nSep;
                	lst << TStr< T >( x_pStr, 0, nSep );
				} // end if

                oexINT nEnd = str::SkipCharacters( &x_pStr[ nSep ], x_uSize, x_pSep, x_uSep );

			    if ( 0 <= nEnd )
			    	x_pStr += nSep + nEnd, x_uSize -= nEnd;

			    else
			    	return lst;

		    } // end else

	    } // end while

	    // Add last item
	    if ( *x_pStr ) lst << x_pStr;

	    return lst;
    }

    template < typename T >
        static TList< TStr< T > > Split( oexCONST T *pStr, oexCONST T *pSep )
	{	return Split< T >( pStr, zstr::Length( pStr ), pSep ); }

    template < typename T >
    	static TList< TStr< T > > Split( TStr< T > &str, oexCONST T *pSep )
	{	return Split< T >( str.Ptr(), str.Length(), pSep ); }

	/// Parses an array template
//	static CPropertyBag CompileTemplate( CStr str );

	/// Compile the statement
//	static oexBOOL CompileStatement( CStr &str, CPropertyBag &pb );

	/// Encodes a string from a format definition
//	static CStr Encode( CPropertyBag &enc, CPropertyBag &fmt, CStr root = "*" );
//	static CStr Encode( CPropertyBag &enc, CStr str )
//	{	return Encode( enc, CompileTemplate( str ) ); }

	/// Encodes a single parameter
//	static oexBOOL EncodeParam( CStr& str, CStr &key, CStr &val, CPropertyBag fmt, oexBOOL bMore );
//	static oexBOOL EncodeParam( CStr& str, CStr &key, CPropertyBag &val, CPropertyBag &fmt, oexBOOL bFirst, oexBOOL bLast );


	/// Decodes url type params such as "a=b&c=d"
    template< typename T >
        static TPropertyBag< TStr< T > > DecodeUrlParams( oexCONST T *x_pStr )
        {   oexASSERT_PTR( x_pStr );
			return DecodeUrlParams( TStr< T >( x_pStr ) );
		}

    template< typename T >
        static TPropertyBag< TStr< T > > DecodeUrlParams( TStr< T > x_str )
    {
        TPropertyBag< TStr< T > > pb;
	    TStr< T > key, val;

	    TList< TStr< T > > lst = CParser::Split< T >( x_str, oexTT( T, "&" ) );

	    for ( typename TList< TStr< T > >::iterator it; lst.Next( it ); )
	    {
		    key = UrlDecode( it->Parse( oexTT( T, "=" ) ) );
		    if ( key.Length() ) (*it)++;
		    val = UrlDecode( it.Obj() );

		    // Key value pair
		    if ( key.Length() && val.Length() )
			    pb[ key ] = val;

		    // NULL key assignment
		    else if ( key.Length() )
			    pb[ key ] = oexTT( T, "" );

		    // Assume NULL key assignment
		    else if ( val.Length() )
			    pb[ val ] = oexTT( T, "" );

	    } // end while

	    return pb;
    }


	/// Encodes url type params such as "a=b&c=d"
    template< typename T >
        static TStr< T > EncodeUrlParams( TPropertyBag< TStr< T > > x_pb )
    {
	    TStr< T > str;

	    for( typename TPropertyBag< TStr< T > >::iterator it; x_pb.List().Next( it ); )
	    {
		    if ( str.Length() )
                str << oexTC( T, '&' );

		    str << UrlEncode( it.Node()->key ) << oexTT( T, "=" ) << UrlEncode( it->ToString() );

	    } // end for

	    return str;
    }

	/// Returns non-zero if the character is a valid url character
    template< typename T >
        static oexBOOL IsUrlChar( T x_ch )
    {   return  ( oexTC( T, 'a' ) <= x_ch && oexTC( T, 'z' ) >= x_ch ) ||
                ( oexTC( T, 'A' ) <= x_ch && oexTC( T, 'Z' ) >= x_ch ) ||
                ( oexTC( T, '0' ) <= x_ch && oexTC( T, '9' ) >= x_ch ) ||
                oexTC( T, '_' ) == x_ch || oexTC( T, '-' ) == x_ch ||
                oexTC( T, '.' ) == x_ch;
    }

	/// Encoded a url string "Hello World" -> "Hello%20World"
    template< typename T >
        static TStr< T > UrlEncode( oexCONST T *x_pStr )
        {	oexASSERT_PTR( x_pStr );
			return UrlEncode( TStr< T >( x_pStr ) );
		}

    template< typename T >
        static TStr< T > UrlEncode( TStr< T > x_str )
    {
	    TStr< T > ret, num;
	    oexINT nLen = x_str.Length();

	    while ( 0 < nLen-- )
	    {
		    if ( IsUrlChar( *x_str ) )
			    ret << *x_str;
		    else
			    ret << num.Fmt( oexTT( T, "%%%02X" ), (oexUCHAR)*x_str );

		    x_str++;

	    } // end while

	    return ret;
    }

	/// Decodes a url string "Hello%20World" -> "Hello World"
    template< typename T >
        static TStr< T > UrlDecode( oexCONST T *x_pStr )
        {   oexASSERT_PTR( x_pStr );
			return UrlDecode( TStr< T >( x_pStr ) );
		}

    template< typename T >
        static TStr< T > UrlDecode( TStr< T > x_str )
    {
	    TStr< T > ret, num;
	    oexINT nLen = x_str.Length();

	    while ( 0 < nLen-- )
	    {
			if ( oexTC( T, '+' ) == *x_str )
				ret << oexTC( T, ' ' );

		    else if ( oexTC( T, '%' ) != *x_str )
			    ret << *x_str;

		    else
		    {
			    x_str++;

			    if ( x_str.IsMatchAt( 0, oexTT( T, "0123456789abcdefABCDEF" ) ) )
				    ret << ( (oexTCHAR)num.Sub( x_str, 0, 2 ).ToLong( 16 ) ), x_str++, nLen -= 2;

			    else
				    ret << *x_str;

		    } // end else

		    x_str++;

	    } // end while

	    return ret;
    }

	/// Returns non-zero if the character is a valid html character
    template< typename T >
        static oexBOOL IsHtmlChar( T x_ch )
    {   if (	   ( oexTC( T, 'a' ) <= x_ch && oexTC( T, 'z' ) >= x_ch )
				|| ( oexTC( T, 'A' ) <= x_ch && oexTC( T, 'Z' ) >= x_ch )
				|| ( oexTC( T, '0' ) <= x_ch && oexTC( T, '9' ) >= x_ch	)
			) return oexTRUE;

		switch( x_ch )
		{
			// case oexTC( T, ' ' ) :
			case oexTC( T, '!' ) :
			case oexTC( T, '#' ) : case oexTC( T, '$' ) :
			case oexTC( T, '%' ) : case oexTC( T, '\'' ) :
			case oexTC( T, '(' ) : case oexTC( T, ')' ) :
			case oexTC( T, '*' ) : case oexTC( T, '+' ) :
			case oexTC( T, ',' ) : case oexTC( T, '-' ) :
			case oexTC( T, '.' ) : case oexTC( T, '/' ) :
			case oexTC( T, '\\' ) : case oexTC( T, ':' ) :
			case oexTC( T, ';' ) : case oexTC( T, '=' ) :
			case oexTC( T, '@' ) : case oexTC( T, '?' ) :
			case oexTC( T, '[' ) : case oexTC( T, ']' ) :
			case oexTC( T, '^' ) : case oexTC( T, '_' ) :
			case oexTC( T, '`' ) : case oexTC( T, '|' ) :
			case oexTC( T, '{' ) : case oexTC( T, '}' ) :
			case oexTC( T, '~' ) : case oexTC( T, ' ' ) :
				return oexTRUE;
		} // end switch

		// Invalid
		return oexFALSE;
    }

    template< typename T >
		static TStr< T > HtmlEncodeChar( T x_ch )
		{
			switch( x_ch )
			{
				case oexTC( T, ' ' ) :
					return oexTT( T, "&nbsp;" );

				case oexTC( T, '"' ) :
					return oexTT( T, "&quot;" );

				case oexTC( T, '&' ) :
					return oexTT( T, "&amp;" );

				case oexTC( T, '<' ) :
					return oexTT( T, "&lt;" );

				case oexTC( T, '>' ) :
					return oexTT( T, "&gt;" );

			} // end switch

			// Generic encode
			return oexFmt( oexTT( T, "&#" ), (oexUCHAR)x_ch, oexTT( T, ";" ) );
		}

	/// Encoded a string "<b>Hello World</b>" -> "&lt;b&gt;Hello&nbsp;World&lt;/b&gt;"
    template< typename T >
        static TStr< T > HtmlEncode( oexCONST T *x_pStr )
        {	oexASSERT_PTR( x_pStr );
			return HtmlEncode( TStr< T >( x_pStr ) );
		}

    template< typename T >
        static TStr< T > HtmlEncode( TStr< T > x_str )
    {
	    TStr< T > ret, num;
	    oexINT nLen = x_str.Length();

	    while ( 0 < nLen-- )
	    {
		    if ( IsHtmlChar( *x_str ) )
			    ret << *x_str;
		    else
			    ret << HtmlEncodeChar( *x_str );

		    x_str++;

	    } // end while

	    return ret;
    }

    template< typename T >
		static TStr< T > HtmlDecodeChar( oexCONST T *x_pStr )
        {	oexASSERT_PTR( x_pStr );
			return HtmlDecodeChar( TStr< T >( x_pStr ) );
		}


    template< typename T >
		struct SHtmlItem
		{
			oexCONST T		ch;
			oexCONST T		*s;
			oexINT			l;
		};


    template< typename T >
		static T HtmlDecodeChar( TStr< T > &x_str )
		{
			static const SHtmlItem< T > c_cnv[] =
			{
				{ oexTC( T, ' ' ), "&nbsp;", 6 },
				{ oexTC( T, '"' ), "&quot;", 6 },
				{ oexTC( T, '&' ), "&amp;", 5 },
				{ oexTC( T, '<' ), "&lt;", 4 },
				{ oexTC( T, '>' ), "&gt;", 4 },
				{ 0, oexNULL }
			};

			// Ensure it starts off looking like an escape sequence
			if ( oexTC( T, '&' ) != *x_str )
				return *( x_str.Slice( 1 ) );

			// Search for ;
			// We'll only consider up to 12 characters
			oexINT i;
			for ( i = 0; i < x_str.Length() && i < 12 && oexTC( T, ';' ) != x_str[ i ]; i++ )
				;

			// Quit if not found
			if ( oexTC( T, ';' ) != x_str[ i ] )
				return *( x_str.Slice( 1 ) );

			// Trim the ; too
			i++;

			// Check for number encode
			if ( oexTC( T, '#' ) == x_str[ 1 ] )
				return (T)x_str.Slice( i ).LTrim( 2 ).ToLong();

			// Scan for known sequence
			TStr< T > sub = x_str.SubStr( 0, i );
			for ( oexUINT c = 0; c < oexSizeOfArray( c_cnv ) && c_cnv[ c ].s; c++ )
				if ( sub == c_cnv[ c ].s )
				{	x_str.LTrim( i );
					return c_cnv[ c ].ch;
				} // end if

			// Eh, what can you do?
			return *( x_str.Slice( 1 ) );
		}


	/// Decodes an html string "&lt;b&gt;Hello&nbsp;World&lt;/b&gt;" -> "<b>Hello World</b>"
    template< typename T >
        static TStr< T > HtmlDecode( oexCONST T *x_pStr )
        {   return HtmlDecode( TStr< T >( x_pStr ) ); }

    template< typename T >
        static TStr< T > HtmlDecode( TStr< T > x_str )
    {
	    TStr< T > ret;

		do
		{
			// Find escape sequence
			oexINT o = x_str.FindChars( oexTT( T, "&" ) );

			// Found &
			if ( 0 <= o )
			{	if ( o )
					ret << x_str.Slice( o );
				ret << HtmlDecodeChar( x_str );
			} // end if

			// Just append the rest of the string
			else
				ret << x_str, x_str.Destroy();

		} while ( x_str.Length() );

		return ret;
    }

    /// Generic property bag deserializing
    template< typename T >
        static TPropertyBag< TStr< T > > Deserialize( oexCONST T *x_pStr, oexBOOL x_bMerge = oexFALSE )
    {   TPropertyBag< TStr< T > > pb;
        TStr< T > str( x_pStr );
        Deserialize( str, pb, x_bMerge );
        return pb;
    }

    /// Generic property bag deserializing
    template< typename T >
        static TPropertyBag< TStr< T > > Deserialize( oexCONST TStr< T > &x_sStr, oexBOOL x_bMerge = oexFALSE )
    {   TPropertyBag< TStr< T > > pb; Deserialize( x_sStr, pb, x_bMerge ); return pb; }

    template< typename T >
        static oexLONG Deserialize( oexCONST TStr< T > &x_sStr, TPropertyBag< TStr< T > > &x_pb, oexBOOL x_bMerge = oexFALSE, oexLONG *x_pLast = oexNULL )
    {
        // Lose previous contents
        if ( !x_bMerge )
            x_pb.Destroy();

        // Punt if null string
        if ( !x_sStr.Length() )
            return 0;

        oexLONG lItems = 0;
        oexLONG lLen = x_sStr.Length(), s = 0, e = 0;

        while ( e <= lLen )
        {
            switch( x_sStr[ e ] )
            {
                case oexTC( T, ',' ) : case oexTC( T, '}' ) : case 0 :
                {
                    if ( 0 < e - s )
                    {
                        // Find '='
                        oexLONG a = s;
                        while ( a < e && oexTC( T, '=' ) != x_sStr[ a ] ) 
							a++;

                        TStr< T > sKey, sVal;

						// NULL key?
						if ( e <= s )
							; // sKey = oexTT( T, "" );

                        // First character is separator
                        else if ( a == s )
                            sKey = UrlDecode( TStr< T >( x_sStr.Ptr( s + 1 ), e - s - 1 ).DropWhiteSpace() );

                        else sKey = UrlDecode( TStr< T >( x_sStr.Ptr( s ), a - s ).DropWhiteSpace() );

                        // Single token
                        if ( 1 >= e - a )
                            x_pb[ sKey ] = oexTT( T, "" );

                        // Both tokens present
                        else 
							x_pb[ sKey ] = UrlDecode( TStr< T >( x_sStr.Ptr( a + 1 ), e - a - 1 ).DropWhiteSpace() );

                        // Count one item
                        lItems++;

                    } // end if

                    // Next element
                    s = e + 1;

                    // Time to exit?
                    if ( oexTC( T, '}' ) == x_sStr[ e ] )
                    {   if ( x_pLast ) *x_pLast = e + 1; return lItems; }

                } break;

                case oexTC( T, '{' ) :
                {
                    // Get key
                    TStr< T > sKey;

                    // Find '='
                    oexLONG a = s;
                    while ( a < e && oexTC( T, '=' ) != x_sStr[ a ] ) 
						a++;

                    // NULL key?
					if ( e <= s )
						; // sKey = oexTT( T, "" );

                    else if ( a == s )
                        sKey = UrlDecode( TStr< T >( x_sStr.Ptr( s + 1 ), e - s - 1 ).DropWhiteSpace() );

                    // No default value
                    else if ( a >= e )
                        sKey = UrlDecode( TStr< T >( x_sStr.Ptr( s ), e - s ).DropWhiteSpace() );

                    else if ( a < e )
                    {   sKey = UrlDecode( TStr< T >( x_sStr.Ptr( s ), a - s ).DropWhiteSpace() );
                        x_pb[ sKey ] = UrlDecode( TStr< T >( x_sStr.Ptr( a + 1 ), e - a - 1 ).DropWhiteSpace() );
                        lItems++;
                    } // end if

                    // Do we have a key?
//                    if ( sKey.Length() )
                    {
                        // This will point to the end of the array we're about to decode
                        oexLONG lEnd = 0;

                        // Get the sub array
                        lItems += Deserialize(  TStr< T >( x_sStr.Ptr( e + 1 ) ),
                                                x_pb[ sKey ], oexTRUE, &lEnd );

                        // Skip the array we just decoded
                        e += lEnd;

                    } // end if

                    // Skip this token
                    s = e + 1;

                } break;

            } // end switch

            // Next i
            e++;

        } // end while

        return lItems;
    }

    template< typename T >
        static TStr< T > Serialize( TPropertyBag< TStr< T > > &x_pb )
    {
        // Just return default value if not an array
        if ( !x_pb.IsArray() )
            return TStr< T >();

        TStr< T > sStr;
        for ( typename TPropertyBag< TStr< T > >::iterator it; x_pb.List().Next( it ); )
        {
            // Add separator if needed
            if ( sStr.Length() )
                sStr << oexTC( T, ',' );

            // Set the key
            sStr << CParser::UrlEncode( it.Node()->key );

            // Encode default value
            if ( it->IsDefaultValue() )
                sStr << oexTC( T, '=' ) << CParser::UrlEncode( it->ToString() );

            // Recurse for array
            if ( it->IsArray() )
                sStr << oexTC( T, '{' ) << Serialize( *it ) << oexTC( T, '}' );

        } // end for

        return sStr;
    }



    /// Returns the next token in x_sStr if it is in lst
    static CStr GetToken( CStr &x_sStr, CStrList x_lst, oexBOOL x_bCaseSensitive );

    /// Returns the next token in x_sStr if it is in lst
    template< typename T >
        static TStr< T > ParseToken( TStr< T > &x_sStr, TList< TStr< T > > x_lst, oexBOOL x_bCaseSensitive )
    {
        if ( !x_lst.Size() )
            return TStr< T >();

        // Case sensitive?
        if ( x_bCaseSensitive )
        {   for ( typename TList< TStr< T > >::iterator it; x_lst.Next( it ); )
                if ( x_sStr.CmpLen( it->Ptr(), it->Length() ) )
                {   x_sStr.LTrim( it->Length() ); return it.Obj(); }
        } // end if

        else
	        for ( typename TList< TStr< T > >::iterator it; x_lst.Next( it ); )
                if ( x_sStr.ICmpLen( it->Ptr(), it->Length() ) )
                {   x_sStr.LTrim( it->Length() ); return it.Obj(); }

        return TStr< T >();
    }

    /// Decode MIME
    template< typename T >
        static TPropertyBag< TStr< T > > DecodeMIME( TStr< T > x_sStr )
    {
        TPropertyBag< TStr< T > > pb;

        while ( x_sStr.Length() )
        {
            TStr< T > sKey = x_sStr.Parse( oexTT( T, ":\r\n" ) ), sVal;
            if ( *x_sStr == oexTC( T, ':' ) ) x_sStr++;

            // Read value string
            do
            {
				// Skip to data
				if ( !x_sStr.IsMatchAt( 0, oexTT( T, "\r\n" ) ) )
				{
					// Skip any white space
					x_sStr.SkipWhiteSpace();

					// Just use the rest of the string if there is no more data
					if ( 0 > x_sStr.FindChars( oexTT( T, "\r\n" ) ) )
						sVal << x_sStr,
						x_sStr.Destroy();

					// Parse off our value
					else
						sVal << x_sStr.Parse( oexTT( T, "\r\n" ) ),
						x_sStr.NextLine();

				} // end if

				else
					x_sStr.NextLine();

            } while ( x_sStr.Length() && x_sStr.IsWhiteSpaceAt( 0 ) );

            // Ensure valid strings
            if ( sKey.Length() && sVal.Length() )
                pb[ sKey ] = sVal;

        } // end while

        return pb;
    }

    /// Encode MIME
    template< typename T >
        static TStr< T > EncodeMime( TPropertyBag< TStr< T > > &x_pb )
    {
	    TStr< T > str;

        // Write out each value
	    for( typename TPropertyBag< TStr< T > >::iterator it; x_pb.List().Next( it ); )

            if ( it.Node()->key.Length() && it->ToString().Length() )

                str << it.Node()->key << oexTT( T, ": " ) << it->ToString() << oexTT( T, "\r\n" );

	    return str;
    }

    /// Encode INI File
    template< typename T >
        static TStr< T > EncodeIni( TPropertyBag< TStr< T > > &x_pb, TStr< T > x_root = oexTT( T, "" ) )
    {
	    TStr< T > str;

        // Write out each value
	    for( typename TPropertyBag< TStr< T > >::iterator it; x_pb.List().Next( it ); )

            if ( it.Node()->key.Length() )
			{
				if ( it->IsArray() )
				{
					TStr< T > sFull;
					if ( x_root.Length() )
						sFull = ( x_root ).BuildPath( it.Node()->key.TrimWhiteSpace() );
					else
						sFull = it.Node()->key.TrimWhiteSpace();
					str << oexTT( T, "\r\n[" ) << sFull << oexTT( T, "]\r\n" );
					str << EncodeIni( *it, sFull );

				} // end if

				else
					str << it.Node()->key.TrimWhiteSpace() << oexTT( T, "=" ) << it->ToString().TrimWhiteSpace() << oexTT( T, "\r\n" );

			} // end if

	    return str;
    }

	/// Decodes a url string "Hello%20World" -> "Hello World"
    template< typename T >
        static TPropertyBag< TStr< T > > DecodeIni( oexCONST T *x_pStr )
        {   return DecodeIni( TStr< T >( x_pStr ) ); }

	/// Decode INI File
    template< typename T >
        static TPropertyBag< TStr< T > > DecodeIni( TStr< T > x_sStr )
    {
        TPropertyBag< TStr< T > > pb;
		TStr< T > sGroup;

		x_sStr << oexTT( T, "\r\n" );
        while ( x_sStr.Length() )
        {
			x_sStr.SkipWhiteSpace();

			// Comment
			if ( oexTC( T, ';' ) == x_sStr[ 0 ] || oexTC( T, '#' ) == x_sStr[ 0 ] )
				; // Ignore

			// Group
			else if ( oexTC( T, '[' ) == x_sStr[ 0 ] )
			{
				// Skip '['
				x_sStr++;

				// Switch groups
				sGroup = x_sStr.Parse( oexTT( T, "]\r\n" ) ).TrimWhiteSpace();

			} // end else if

			// Values
			else
			{
				TStr< T > sKey = x_sStr.Parse( oexTT( T, "=\r\n" ) ).TrimWhiteSpace(), sVal;
				if ( *x_sStr == oexTC( T, '=' ) ) x_sStr++;

				// Read value string
				sVal << x_sStr.SkipWhiteSpace().Parse( oexTT( T, "\r\n" ) ).TrimWhiteSpace();

				// Ensure valid strings
				if ( sKey.Length() && sVal.Length() )
				{
					if ( sGroup.Length() )
						pb[ sGroup ][ sKey ] = sVal;
					else
						pb[ sKey ] = sVal;

				} // end if

			} // end else

			x_sStr.NextLine();

        } // end while

        return pb;
    }


    /// Creates a list of all permutations of the specified character string
    /**
        \param [in] x_sStr  -   String of characters in which to permutate

        \return List of permutations

      \code

        CStrList lst = StringPermutations( _T( "abc" ) );

      \endcode
    */
    template < typename T >
        static TList< TStr< T > > StringPermutations( TStr< T > x_sStr )
        {
            // It really isn't practical to go higher
            oexASSERT( x_sStr.Length() <= 8 );

            TList< TStr< T > > lst;

#if defined( oexDEBUG )

            oexUINT u = StringPermutations( lst, (T*)x_sStr.Ptr(), (T*)0 );

            oexASSERT( u == lst.Size() );

            // Verify known lengths
            switch( x_sStr.Length() )
            {
                case 0 : oexASSERT( 0 == u ); break;
                case 1 : oexASSERT( 1 == u ); break;
                case 2 : oexASSERT( 2 == u ); break;
                case 3 : oexASSERT( 6 == u ); break;
                case 4 : oexASSERT( 24 == u ); break;
                case 5 : oexASSERT( 120 == u ); break;
                case 6 : oexASSERT( 720 == u ); break;
                case 7 : oexASSERT( 5040 == u ); break;
                case 8 : oexASSERT( 40320 == u ); break;
                case 9 : oexASSERT( 362880 == u ); break;
                case 10 : oexASSERT( 3628800 == u ); break;
            };
#else
            StringPermutations( lst, (T*)x_sStr.Ptr(), (T*)0 );

#endif
            return lst;
        }


    /// Creates a list of all permutations of the specified character string
    /**
        \param [in] x_lst   -   List that will contain permutations
        \param [in] x_sStr  -   String of characters in which to permutate
        \param [in] p       -   Internal parameter used for recursion

      \code

        CStrList lst;
        TCHAR s[] = _T( "abc" );
        int nNumPermutations = StringPermutations( lst, s );

      \endcode
    */
    template < typename T >
        static int StringPermutations( TList< TStr< T > > &x_lst, T *s, T *p = 0 )
    {
        if ( !p )
            p = s;

        int t = 0;
        T *n = p;
        while ( *p && *n )
        {
            cmn::Swap( *p, *n );

            if ( *( p + 1 ) && *( p + 2 ) )
                t += StringPermutations( x_lst, s, p + 1 );
            else
                t++, x_lst << s;

            cmn::Swap( *p, *n );

            n++;

        } // end while

        return t;
    }

public:

    template < typename T >
		static oexUINT ahtodw( oexCONST T* pBuffer, oexUINT uBytes )
		{
			oexUINT num = 0;

			// For Each ASCII Digit
			for ( oexUINT i = 0; i < uBytes; i++ )
			{
				// Convert ASCII Digit Between 0 And 9
				if ( pBuffer[ i ] >= oexTC( T, '0' ) && pBuffer[ i ] <= oexTC( T, '9' ) )
					num = ( num << 4 ) + ( pBuffer[ i ] - oexTC( T, '0' ) );

				// Convert ASCII Digit Between A And F
				else if ( pBuffer[ i ] >= oexTC( T, 'A' ) && pBuffer[ i ] <= oexTC( T, 'F' ) )
					num = ( num << 4 ) + ( pBuffer[ i ] - 'A' ) + 10;

				// Convert ASCII Digit Between a And f
				else if ( pBuffer[ i ] >= oexTC( T, 'a' ) && pBuffer[ i ] <= oexTC( T, 'f' ) )
					num = ( num << 4 ) + ( pBuffer[ i ] - oexTC( T, 'a' ) ) + 10;

			} // end for

			return num;
		}

    template < typename T >
		static oexUINT HtmlToRgb( oexCONST T* pHtml )
		{
			if ( !oexCHECK_PTR( pHtml ) || *pHtml != oexTC( T, '#' ) )
				return 0;

			// Convert to RGB value
			return ahtodw( &pHtml[ 1 ], 2 )
				   | ( ahtodw( &pHtml[ 3 ], 2 ) << 8 )
				   | ( ahtodw( &pHtml[ 5 ], 2 ) << 16 );
		}

    template < typename T >
		oexUINT dwtohstr( oexUINT num, T *buf, oexUINT index, oexUINT pad = 2, oexUINT max = 2 )
		{
			oexUINT i = 0;
			oexUINT	c = 8;

			// Correct pointer
			buf = &buf[ index ];

			while ( c )
			{
				buf[ i ] = (T)( oexTC( T, '0' ) + ( ( num & 0xf0000000 ) >> 28 ) );
				if ( buf[ i ] > oexTC( T, '9' ) ) buf[ i ] += oexTC( T, 'A' ) - oexTC( T, '9' ) - 1;
				if ( pad >= c || i || buf[ i ] != oexTC( T, '0' ) ) if ( i < max ) i++;
				num <<= 4;
 				c--;

			} // end while

			if ( !i ) buf[ i++ ] = oexTC( T, '0' );

			buf[ i ] = 0;

			return i;
		}

    template < typename T >
		TStr< T > RgbToHtml( oexUINT rgb )
		{
			oexUINT i = 0;

			TStr< T > sHtml;
			T *pHtml = sHtml.OexAlloc( 7 );
			if ( !oexCHECK_PTR( pHtml ) )
				return oexTT( T, "" );

			// Convert to HTML format
			pHtml[ i++ ] = oexTC( T, '#' );
			i += dwtohstr( rgb & 0x0000ff, pHtml, i );
			i += dwtohstr( ( rgb & 0x00ff00 ) >> 8, pHtml, i );
			i += dwtohstr( ( rgb & 0x00ff00 ) >> 16, pHtml, i );

			return sHtml;
		}

	// Match against list of file patterns
    template < typename T >
		oexBOOL MatchPatterns( TStr< T > x_s, TList< TStr< T > > x_lstPatterns, oexBOOL x_bIgnoreCase )
		{	for ( typename TList< TStr< T > >::iterator it; x_lstPatterns.Next( it ); )
				if ( x_s.MatchPattern( it.Obj(), x_bIgnoreCase ) )
					return oexTRUE;
			return oexFALSE;
		}


	/// Splits a string into an array by cutting on any character in m_pSep, while respecting quotes
	template < typename T >
		static TList< TStr< T > > SplitQuoted( oexCONST T *x_pStr, oexUINT x_uSize,
											   oexCONST T *x_pSep, oexCONST T *x_pOpen,
											   oexCONST T *x_pClose, oexCONST T *x_pEsc )
	{
		TList< TStr< T > > lst;
		if ( !oexVERIFY_PTR( x_pStr ) )
			return lst;

		oexUINT x_uSep = 0;
		if ( !x_pSep ) x_uSep = 0;
		else if ( !oexVERIFY_PTR( x_pSep ) )
			return lst;
		else x_uSep = zstr::Length( x_pSep );

		// Breaking down to chars?
		if ( !x_uSep )
			return lst;

		while ( x_uSize )
		{
			// Skip any separators
			oexINT nSkip = str::SkipCharacters( x_pStr, x_uSize, x_pSep, x_uSep );
			if ( 0 <= nSkip )
				x_pStr += nSkip, x_uSize -= nSkip;

			// Are we done?
			if ( !x_uSize )
				return lst;

			// Find a closing separator
			oexINT nSep = str::ParseWithQuoted( x_pStr, x_uSize, x_pSep, x_uSep,
												x_pOpen, zstr::Length( x_pOpen ),
												x_pClose, zstr::Length( x_pClose ),
												x_pEsc, x_pEsc ? zstr::Length( x_pEsc ) : 0 );

			if ( 0 > nSep )
			{	lst << x_pStr;
				return lst;
			} // end if

			else
			{
				if ( nSep )
				{	x_uSize -= nSep;
					lst << TStr< T >( x_pStr, 0, nSep );
					x_pStr += nSep;
				} // end if

			} // end else

		} // end while

		// Add last item
		if ( *x_pStr )
			lst << x_pStr;

		return lst;
	}

	/// Parses command line variables in a typical format
	/*
	 	example :

		-a -simple:param -file:'C:/Program Files/myfile.txt' -two:'hi and\\'hi' -empty: hello world

		parses to...

		Array( Size() == 7 )
		{
		  'a' = ''
		  'simple' = 'param'
		  'file' = 'C:/Program Files/myfile.txt'
		  'two' = 'hi and\'hi'
		  'empty' = ''
		  '0' = 'hello'
		  '1' = 'world'
		}

	*/

	typedef oexTCHAR T_tc;

	static TPropertyBag< TStr< T_tc > > ParseCommandLine( oexINT x_nNum, T_tc **x_pStr );

	static TPropertyBag< TStr< T_tc > > ParseCommandLine( oexCONST TStr< T_tc > &x_sStr );

	static oexLONG ParseCommandLine( oexCONST TStr< T_tc > &x_sStr, TPropertyBag< TStr< T_tc > > &x_pb, oexBOOL x_bMerge = oexFALSE );

/*	+++ The following code should work, but gcc is choking on it

	template< typename T >
		static TPropertyBag< TStr< T > > ParseCommandLine( oexINT x_nNum, oexCONST T *x_pStr )
	{
		if ( 2 > x_nNum || !oexCHECK_PTR( x_pStr ) )
			return TPropertyBag< TStr< T > >();

		// Build string
		oex::TStr< T > sCmdLine;
		for ( int i = 1; i < x_nNum; i++ )
			sCmdLine << x_pStr[ i ] << oexT( " " );

		// Parse the command line
		return ParseCommandLine( sCmdLine );
	}

	template< typename T >
		static TPropertyBag< TStr< T > > ParseCommandLine( oexCONST TStr< T > &x_sStr )
	{
		TPropertyBag< TStr< T > > pb;
		ParseCommandLine( x_sStr, pb );
		return pb;
	}

	template< typename T >
		static oexLONG ParseCommandLine( oexCONST TStr< T > &x_sStr, TPropertyBag< TStr< T > > &x_pb, oexBOOL x_bMerge = oexFALSE )
	{
		//typedef oex::CStr strType;

		// +++ This is the correct one, not the one above.
		//     Why doesn't it work on gcc ???!!!!??!
		typedef oex::TStr< T > strType;

		// Lose previous contents
		if ( !x_bMerge )
			x_pb.Destroy();

		// Punt if null string
		if ( !x_sStr.Length() )
			return 0;

		// Parse the command line items
		oex::TList< strType > strlst;
		strlst = CParser::SplitQuoted( x_sStr.Ptr(), x_sStr.Length(), " ", "\"'", "\"'", "\\" );

		// We get anything?
		if ( !strlst.Size() )
			return 0;

		oexINT i = 0;
		for ( oex::TList< strType >::iterator it; strlst.Next( it ); )
		{
			// Is it a switch?
			if ( oexTC( T, '-' ) == it.Obj()[ 0 ] )
			{
				it.Obj()++;
				TStr< T > sKey = it.Obj().Parse( oexTT( T, ":" ) );
				if ( *it.Obj() == oexTC( T, ':' ) )
					it.Obj()++, x_pb[ sKey ] = it.Obj().Unquote( oexTT( T, "\"'" ), oexTT( T, "\"'" ), oexTT( T, "\\" ) );
				else
					x_pb[ it.Obj() ] = oexTT( T, "" );

			} // end if

			// Add as number
			else
				x_pb[ ( TStr< T >() << i++ ) ] = it.Obj();

		} // end if

		return x_pb.Size();
	}
*/

protected:

};


