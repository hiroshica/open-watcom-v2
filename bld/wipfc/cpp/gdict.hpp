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
* Description:  Dictionary (vocabulary) data
* Words are stored in the Dictionary table in the help file
*
* GlobalDictionaryWord * GlobalDictionary[ IpfHeader.dictCount ];
*
* Each entry is formatted as
* struct {
*     unsigned char   size;
*     char            text[size - 1];
*     };
*
* The global dictionary owns the words that it contains, and is responsible
* for destroying them.
*
****************************************************************************/

#ifndef GLOBALDICTIONARY_INCLUDED
#define GLOBALDICTIONARY_INCLUDED

#include <cstdio>
#include <string>
#include <set>
#include <map>
#include "gdword.hpp"
#include "ptrops.hpp"

class Document;     // forward reference

class GlobalDictionary {
    typedef STD1::uint8_t   byte;
    typedef STD1::uint16_t  word;
    typedef STD1::uint32_t  dword;

public:
    GlobalDictionary() : _bytes( 0 ), _ftsBytes( 0 ) {};
    ~GlobalDictionary();
    //add a word to the collection
    GlobalDictionaryWord* insert( GlobalDictionaryWord* wordent );
    GlobalDictionaryWord* insert( const std::wstring& wordtxt );
    //after insertions are completed, assign an index to each element
    //of the collection as if it were an array
    void convert( std::size_t count );
    //get the index of a word in the collection
    STD1::uint16_t findIndex( GlobalDictionaryWord *wordent ) { return (*_words.find( wordent ))->index(); };
    STD1::uint16_t findIndex( const std::wstring& wordtxt );
    //get a word from the collection
    GlobalDictionaryWord* findWord( GlobalDictionaryWord *wordent ) { return *_words.find( wordent ); };
    GlobalDictionaryWord* findWord( const std::wstring& wordtxt );
    //the number of elements in the collection
    STD1::uint16_t size() const { return static_cast< word >( _words.size() ); };
    //the number of bytes written by the collection
    STD1::uint32_t length() const { return _bytes; };
    //the number of bytes of FTS data written by the collection
    STD1::uint32_t ftsLength() const { return _ftsBytes; };
    STD1::uint32_t write( std::FILE* out, Document *document );
    bool buildFTS();
    STD1::uint32_t writeFTS( std::FILE* out, bool big );
private:
    GlobalDictionary( const GlobalDictionary& rhs );            //no copy
    GlobalDictionary operator= ( const GlobalDictionary& rhs ); //no assigment
    std::set< GlobalDictionaryWord*, ptrLess< GlobalDictionaryWord* > > _words;
    typedef std::set< GlobalDictionaryWord*, ptrLess< GlobalDictionaryWord* > >::const_iterator ConstWordIter;
    typedef std::set< GlobalDictionaryWord*, ptrLess< GlobalDictionaryWord* > >::iterator WordIter;
    dword _bytes;
    dword _ftsBytes;
};

#endif //GLOBALDICTIONARY_INCLUDED

