/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
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
* Description:  Generate object code from symbolic instructions.
*
****************************************************************************/


#if 0
extern  void            FreeBlock( void );
extern  void            CodeLineNum(cg_linenum,bool);
extern  void            InitZeroPage( void );
extern  void            FiniZeroPage( void );
extern  void            TellReachedLabel(label_handle);
extern  void            InitStackDepth(block*);
extern  block           *FindBlockWithLbl( label_handle label );
extern  void            Zoiks( int );
extern  void            ClearBlockBits( block_class );
extern  bool_maybe      ReDefinedBy( instruction *, name * );

extern  void            *EdgeStackInit( void );
extern  void            EdgeStackFini( void * );
extern  bool            EdgeStackEmpty( void * );
extern  void            EdgeStackPush( void *, block_edge * );
extern  block_edge      *EdgeStackPop( void * );
#endif

extern  unsigned        DepthAlign( unsigned );
extern  void            GenCallLabel(pointer);
extern  void            GenLabelReturn( void );
extern  void            GenObjCode(instruction*);
extern  void            GenEpilog( void );
extern  void            TellCondemnedLabel(label_handle);

extern  void    GenObject( void );
extern  void    SortBlocks( void );