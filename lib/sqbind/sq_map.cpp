/*------------------------------------------------------------------
// sq_map.cpp
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

#include "stdafx.h"

using namespace sqbind;

CSqMap::CSqMap()
{
}

CSqMap::CSqMap( const CSqMap::t_Obj &s )
{
	deserialize( s );
}


CSqMap::t_List& CSqMap::list() 
{   return m_lst; }

CSqMap::t_Obj& CSqMap::operator []( CSqMap::t_Obj &rObj )
{	return m_lst[ rObj ]; }

CSqMap::t_Obj& CSqMap::operator []( const char *p )
{	return m_lst[ p ]; }

void CSqMap::Register( SquirrelVM &vm )
{
    SqPlus::SQClassDef< CSqMap >( vm, oexT( "CSqMap" ) )
            . func( &CSqMap::serialize,     oexT( "serialize" ) )
            . func( &CSqMap::deserialize,   oexT( "deserialize" ) )
            . func( &CSqMap::merge,			oexT( "merge" ) )
            . func( &CSqMap::urlencode,		oexT( "urlencode" ) )
            . func( &CSqMap::urldecode,		oexT( "urldecode" ) )
            . func( &CSqMap::isset,         oexT( "isset" ) )
            . func( &CSqMap::set,           oexT( "set" ) )
            . func( &CSqMap::get,           oexT( "get" ) )
            . func( &CSqMap::find_sub_k,    oexT( "find_sub_k" ) )
            . func( &CSqMap::find_sub_v,    oexT( "find_sub_v" ) )
            . func( &CSqMap::_get,          oexT( "_get" ) )
            . func( &CSqMap::_nexti,        oexT( "_nexti" ) )
        ;
}

CSqMap::t_Obj CSqMap::serialize()
{
	oex::CPropertyBag pb;
	for ( t_List::iterator it = m_lst.begin(); it != m_lst.end(); it++ )
		pb[ it->first.c_str() ] = it->second.c_str();
	return oex::CParser::Serialize( pb ).Ptr();
}

void CSqMap::deserialize( const CSqMap::t_Obj &s )
{
	// Deserialize data
	oex::CPropertyBag pb = oex::CParser::Deserialize( s.c_str() );
	for ( oex::CPropertyBag::iterator it; pb.List().Next( it ); )
		m_lst[ it.Node()->key.Ptr() ] = it->ToString().Ptr(); 
}

void CSqMap::merge( const CSqMap::t_Obj &s )
{
	// Deserialize data
	oex::CPropertyBag pb = oex::CParser::Deserialize( s.c_str(), oex::oexTRUE );
	for ( oex::CPropertyBag::iterator it; pb.List().Next( it ); )
		m_lst[ it.Node()->key.Ptr() ] = it->ToString().Ptr(); 
}

CSqMap::t_Obj CSqMap::urlencode()
{
	oex::CPropertyBag pb;
	for ( t_List::iterator it = m_lst.begin(); it != m_lst.end(); it++ )
		pb[ it->first.c_str() ] = it->second.c_str();
	return oex::CParser::EncodeUrlParams( pb ).Ptr();
}

void CSqMap::urldecode( const CSqMap::t_Obj &s )
{
	// Deserialize data
	oex::CPropertyBag pb = oex::CParser::DecodeUrlParams( s.c_str() );;
	for ( oex::CPropertyBag::iterator it; pb.List().Next( it ); )
		m_lst[ it.Node()->key.Ptr() ] = it->ToString().Ptr(); 
}

void CSqMap::set( const t_Obj &k, const t_Obj &v )
{
    m_lst[ k ] = v;
}

oex::oexBOOL CSqMap::isset( const CSqMap::t_Obj &k )
{	return m_lst.end() != m_lst.find( k ); }

CSqMap::t_Obj CSqMap::get( const CSqMap::t_Obj &k )
{
    if ( m_lst.end() == m_lst.find( k ) )
        return oexT( "" );

    return m_lst[ k ];
}

/// Adds an element to the vector
CSqMap::t_Obj CSqMap::find_sub_k( const CSqMap::t_Obj &k )
{
    // For each item
    for ( t_List::iterator it = m_lst.begin();
          m_lst.end() != it;
          it++ )

        // Is the sub string in the key?
//                if ( _tcsstr( it->first.c_str(), k.c_str() ) )
        if ( match_pattern( it->first.c_str(), k.c_str() ) )
            return it->first;

    return oexT( "" );
}

/// Adds an element to the vector
CSqMap::t_Obj CSqMap::find_sub_v( const CSqMap::t_Obj &v )
{
    // For each item
    for ( t_List::iterator it = m_lst.begin();
          m_lst.end() != it;
          it++ )

        // Is the sub string in the value
//                if ( _tcsstr( it->second.c_str(), v.c_str() ) )
        if ( match_pattern( it->second.c_str(), v.c_str() ) )
            return it->first;

    return oexT( "" );
}

/// Internal squirrel function, returns value of specified item
SquirrelObject CSqMap::_get( HSQUIRRELVM v )
{
    StackHandler sa( v );

    const SQChar *pKey = sa.GetString( 2 );
    if ( !pKey || !*pKey )
        return SquirrelObject( v );

    t_List::iterator it = m_lst.find( pKey );
    if ( m_lst.end() == it )
        return SquirrelObject( v );

    // Stuff string
    SquirrelObject so( v );
    sq_pushstring( v, it->second.c_str(), (int)it->second.length() );
    so.AttachToStackObject( -1 );
    sq_pop( v, 1 );

    return so;
}

/// Internal squirrel function used to iterate list items
SquirrelObject CSqMap::_nexti( HSQUIRRELVM v )
{
    StackHandler sa( v );

    SQObjectType type = (SQObjectType)sa.GetType( 2 );

    switch( type )
    {
        case OT_NULL:
        {
            if ( !m_lst.size() )
                return SquirrelObject( v );

            SquirrelObject so( v );

            sq_pushstring( v, m_lst.begin()->first.c_str(), (int)m_lst.begin()->first.length() );
            so.AttachToStackObject( -1 );
            sq_pop( v, 1 );

            return so;

        } break;

        case OT_STRING:
        {
            const SQChar *pKey = sa.GetString( 2 );
            if ( !pKey || !*pKey )
                return SquirrelObject( v );

            t_List::iterator it = m_lst.find( pKey );
            if ( m_lst.end() == it )
                return SquirrelObject( v );

            it++;
            if ( m_lst.end() == it )
                return SquirrelObject( v );

            SquirrelObject so( v );

            sq_pushstring( v, it->first.c_str(), (int)it->first.length() );
            so.AttachToStackObject( -1 );
            sq_pop( v, 1 );

            return so;

        } break;

    } // end switch

    // ???
    sa.ThrowError( oexT( "Invalid index type" ) );
    return SquirrelObject( v );
}

/// Returns non-zero if pPattern matches pString
bool CSqMap::match_pattern( const oex::oexTCHAR *pString, const oex::oexTCHAR *pPattern)
{
    if ( !pString || !pPattern ) 
        return false;

    unsigned int i = 0, p = 0;

    // Skip multiple '*'
    while ( oexT( '*' ) == pPattern[ p ] && oexT( '*' ) == pPattern[ p + 1 ] ) 
        p++;

    // Check for the 'any' pattern
    if ( pPattern[ p ] == oexT( '*' ) && pPattern[ p + 1 ] == 0 )
        return true;

    // While we're not at the end
    while ( pString[ i ] != 0 )
    {
        // Are we on a wildcard?
        if ( oexT( '*' ) == pPattern[ p ] )
        {
	        // Are we matching everything?
	        if ( pPattern[ p + 1 ] == 0 ) 
                return true;

	        // Check for pattern advance
	        if (	pString[ i ] == pPattern[ p + 1 ] ||

			        (
				        pString[ i ] >= oexT( 'a' ) && pString[ i ] <= oexT( 'z' ) && 
				        ( pString[ i ] - ( oexT( 'a' ) - oexT( 'A' ) ) ) == pPattern[ p + 1 ] 
			        ) ||

			        (
				        pString[ i ] >= oexT( 'A' ) && pString[ i ] <= oexT( 'Z' ) && 
				        ( pString[ i ] + ( oexT( 'a' ) - oexT( 'A' ) ) ) == pPattern[ p + 1 ] 
			        ) 

		        ) p += 2;
				
        } // end if

        // Just accept this character
        else if ( oexT( '?' ) == pPattern[ p ] ) 
            p++;

        // Otherwise advance if equal
        else if ( pString[ i ] == pPattern[ p ] ) 
            p++;

        // Case insensitive
        else if (	(
				        pString[ i ] >= oexT( 'a' ) && pString[ i ] <= oexT( 'z' ) && 
				        ( pString[ i ] - ( oexT( 'a' ) - oexT( 'A' ) ) ) == pPattern[ p ] 
			        ) ||
			        (
				        pString[ i ] >= oexT( 'A' ) && pString[ i ] <= oexT( 'Z' ) && 
				        ( pString[ i ] + ( oexT( 'a' ) - oexT( 'A' ) ) ) == pPattern[ p ] 
			        ) 
		        ) p++;

        // Back up in the pattern
        else while ( p && pPattern[ p ] != oexT( '*' ) ) p--;

        // Return true if we're at the end of the pattern
        if ( pPattern[ p ] == 0 ) 
            return true;

        // Next char
        i++;

    } // end while

    // Skip wild cards
    while ( pPattern[ p ] == oexT( '*' ) ) p++;

    // Did we match?
    return ( pPattern[ p ] == 0 );
}

