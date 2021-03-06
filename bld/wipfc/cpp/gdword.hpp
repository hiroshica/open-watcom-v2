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
* Description:  A single word in the Dictionary (vocabulary) data
*
****************************************************************************/

#ifndef GDWORD_INCLUDED
#define GDWORD_INCLUDED

#include <cstdio>
#include <string>
#include <vector>
#include "fts.hpp"

class Document;     // forward reference

class GlobalDictionaryWord {
    typedef STD1::uint8_t   byte;
    typedef STD1::uint16_t  word;
    typedef STD1::uint32_t  dword;

public:
    GlobalDictionaryWord() : _index( 0 ) { };
    GlobalDictionaryWord( const std::wstring& text ) : _index( 0 ), _text( text ) { };
    //convert to upper case
    void toUpper();
    //set the index postion in the collection
    void setIndex( word index ) { _index = index; };
    STD1::uint16_t index() const { return _index; };
    const std::wstring& getText() const { return _text; };
    std::size_t writeWord( std::FILE* out, Document *document ) const;
    bool operator==( const GlobalDictionaryWord& rhs ) const { return _text == rhs._text; };
    bool operator==( const std::wstring& rhs ) const { return _text == rhs; };
    bool operator<( const GlobalDictionaryWord& rhs ) const;
    //Functions forwarded to FTSElement
    void setPages( std::size_t count ) { _fts.setPages( count ); };
    void onPage( std::size_t i ) { _fts.onPage( i ); };
    void buildFTS() { _fts.build(); };
    bool isBigFTS() { return _fts.isBigFTS(); };
    std::size_t writeFTS( std::FILE* out, bool big ) { return _fts.write( out, big ); };
private:
    GlobalDictionaryWord( const GlobalDictionaryWord& rhs );            //no copy
    GlobalDictionaryWord& operator=( const GlobalDictionaryWord& rhs ); //no assignment
    word _index;                //index in Dictionary
    std::wstring _text;          //the actual text
    FTSElement _fts;
    int wstricmp( const wchar_t *s, const wchar_t *t ) const;
};

#endif //GDWORD_INCLUDED
