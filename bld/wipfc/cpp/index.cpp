/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2009-2018 The Open Watcom Contributors. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  An index entry
*
****************************************************************************/


#include "wipfc.hpp"
#include <cwctype>
#include "index.hpp"
#include "errors.hpp"
#include "document.hpp"


IndexItem::IndexItem( Type t )
{
    if( t == PRIMARY )
        _hdr.primary = 1;
    else if( t == SECONDARY )
        _hdr.secondary = 1;
}
/***************************************************************************/
bool IndexItem::operator==( const IndexItem& rhs ) const
{
    if( _sortKey.empty() ) {
        if( rhs._sortKey.empty() )
            return wstricmp( _text.c_str(), rhs._text.c_str() ) == 0;
        else
            return wstricmp( _text.c_str(), rhs._sortKey.c_str() ) == 0;
    }
    else {
        if( rhs._sortKey.empty() )
            return wstricmp( _sortKey.c_str(), rhs._text.c_str() ) == 0;
        else
            return wstricmp( _sortKey.c_str(), rhs._sortKey.c_str() ) == 0;
    }
}
/***************************************************************************/
bool IndexItem::operator==( const std::wstring& rhs ) const
{
    if( _sortKey.empty() )
        return wstricmp( _text.c_str(), rhs.c_str() ) == 0;
    else
        return wstricmp( _sortKey.c_str(), rhs.c_str() ) == 0;
}
/***************************************************************************/
bool IndexItem::operator<( const IndexItem& rhs ) const
{
    if( _sortKey.empty() ) {
        if( rhs._sortKey.empty() )
            return wstricmp( _text.c_str(), rhs._text.c_str() ) < 0;
        else
            return wstricmp( _text.c_str(), rhs._sortKey.c_str() ) < 0;
    }
    else {
        if( rhs._sortKey.empty() )
            return wstricmp( _sortKey.c_str(), rhs._text.c_str() ) < 0;
        else
            return wstricmp( _sortKey.c_str(), rhs._sortKey.c_str() ) < 0;
    }
}
/***************************************************************************/
//return <0 if s < t, 0 if s == t, >0 if s > t
int IndexItem::wstricmp( const wchar_t *s, const wchar_t *t ) const
{
    wchar_t c1( std::towupper( *s ) );
    wchar_t c2( std::towupper( *t ) );
    while( c1 == c2 && c1 != L'\0' ) {
        ++s;
        ++t;
        c1 = std::towupper( *s );
        c2 = std::towupper( *t );
    }
    return( c1 - c2 );
}/***************************************************************************/
//calculate size of text
//write header
//variable length data follows:
//if sortKey bit set
//  char size2                          //size of sortKey text
//  char sortText[size2]                //sort key text
//char indexText[size or size-size2];   //index word [not zero-terminated]
//unsigned long synonyms[synonymCount]; //32 bit file offsets to synonyms referencing this word
std::size_t IndexItem::write( std::FILE* out, Document *document )
{
    std::string buffer1;
    std::string buffer2;
    std::size_t length1( 0 );
    std::size_t length2( 0 );
    if( _hdr.sortKey ) {
        document->wtomb_string( _sortKey, buffer1 );
        length1 = buffer1.size();
    }
    document->wtomb_string( _text, buffer2 );
    length2 = buffer2.size();
    if( length1 + length2 > 254 ) {
        length2 = length1 > 254 ? 0 : 254 - length1;
    }
    else if( _hdr.sortKey )
        _hdr.size = static_cast< STD1::uint8_t >( length1 + length2 + 1 );
    else
        _hdr.size = static_cast< STD1::uint8_t >( length2 );
    _hdr.synonymCount = static_cast< STD1::uint8_t >( _synonyms.size() );
    if( std::fwrite( &_hdr, sizeof( IndexHeader ), 1, out ) != 1 )
        throw FatalError( ERR_WRITE );
    std::size_t written( sizeof( IndexHeader ) );
    if( _hdr.sortKey ) {
        if( std::fputc( length1, out ) == EOF ||
            std::fwrite( buffer1.data(), sizeof( char ), length1, out ) != length1 )
            throw FatalError( ERR_WRITE );
        written += length1 + 1;
    }
    if( std::fwrite( buffer2.data(), sizeof( char ), length2, out ) != length2 )
        throw FatalError( ERR_WRITE );
    written += length2;
    if( !_synonyms.empty() &&
        std::fwrite( &_synonyms[0], sizeof( STD1::uint32_t ), _synonyms.size(), out ) != _synonyms.size() )
        throw FatalError( ERR_WRITE );
    written += _synonyms.size() * sizeof( STD1::uint32_t );
    return written;
}
