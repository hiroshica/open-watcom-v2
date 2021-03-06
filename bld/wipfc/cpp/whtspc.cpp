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
* Description:  A WHITESPACE element
* Note that only single spaces are elided; all others are significant
*
****************************************************************************/


#include "wipfc.hpp"
#include "whtspc.hpp"
#include "cell.hpp"
#include "document.hpp"

WhiteSpace::WhiteSpace( Document* d, Element* p, const std::wstring* f, unsigned int r,
    unsigned int c, const std::wstring& tx, Tag::WsHandling w, bool ts ) :
    Text( d, p, f, r, c, w, ts )
{
    if( tx[0] != L'\n' ) {
        _spaces = static_cast< unsigned char >( tx.size() );
    } else {
        _spaces = 0;
    }
    if( w == Tag::SPACES ) {
        _text = _document->addWord( new GlobalDictionaryWord( tx ) );   //insert into global dictionary
    }
}
/***************************************************************************/
Lexer::Token WhiteSpace::parse( Lexer* lexer )
{
    if( lexer->text()[0] != L'\n' ) {
        _spaces = static_cast< unsigned char >( lexer->text().size() ); //number of spaces
        if( _whiteSpace == Tag::SPACES ) {
            _text = _document->addWord( new GlobalDictionaryWord( lexer->text() ) );   //insert into global dictionary
        }
    }
    if( _whiteSpace != Tag::SPACES && !_document->autoSpacing() ) {
        _document->toggleAutoSpacing();
        Lexer::Token t( _document->lastToken() );
        if( t == Lexer::WORD || t == Lexer::ENTITY || t == Lexer::PUNCTUATION ) {
            _document->lastText()->setToggleSpacing();
        }
    }
    _document->setLastPrintable( Lexer::WHITESPACE, this );
    return _document->getNextToken();
}
/***************************************************************************/
void WhiteSpace::buildText( Cell* cell )
{
    if( _spaces ) {
        if( _whiteSpace == Tag::SPACES && _text ) {
            Text::buildText( cell );
        } else if( _col == 1 ) {
            for( unsigned char count = 0; count < _spaces / 2; ++count )
                cell->addByte( 0xFE );
            if( _spaces & 1 ) {
                cell->addByte( 0xFC );
                cell->addByte( 0xFE );
                cell->addByte( 0xFC );
            }
        } else if( _spaces > 1 ) {
            if( _spaces & 1 ) {
                for( unsigned char count = 0; count < _spaces / 2; ++count )
                    cell->addByte( 0xFE );
            } else {
                for( unsigned char count = 0; count < _spaces / 2 - 1; ++count )
                    cell->addByte( 0xFE );
                cell->addByte( 0xFC );
                cell->addByte( 0xFE );
                cell->addByte( 0xFC );
            }
        }
    } else if( _whiteSpace != Tag::NONE ) { //'\n'
        cell->addByte( 0xFD );
    }
    if( cell->textFull() ) {
        printError( ERR1_LARGEPAGE );
    }
}
/***************************************************************************/
std::pair< bool, bool > LiteralWhiteSpace::buildLocalDict( Page* page )
{
    (void)page;
    std::pair< bool, bool > retval( false, _toggleSpacing );
    return retval;
}
/***************************************************************************/
void LiteralWhiteSpace::buildText( Cell* cell )
{
    if( _toggleSpacing )
        cell->addByte( 0xFC );
    cell->addByte( 0xFE );
    if( cell->textFull() ) {
        printError( ERR1_LARGEPAGE );
    }
}
