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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "..\h\topic.h"
#include "..\h\scanning.h"

Text_attr::Text_attr( text_flags t, unsigned long d )
        : type( t )
{
    data = 0;
    data += d;
}

TopicLink::TopicLink( HFTopic *topic_file, unsigned char type )
{
    format = topic_file->format();
    data = topic_file->data();
    rec_type = type;
    myoffset = topic_file->offset();
    mycharoffset = topic_file->charoff();
    header_size = 21 + format->size();
    size_ = header_size + data->size();
    text_size = data->truesize();
    next = NULL;
    prev = topic_file->lastlink();
}

TopicLink::~TopicLink()
{
    delete format;
    delete data;
}

unsigned long TopicLink::size()
{
    format->prep_block();
    data->prep_block();
    return size_ + format->size() + data->size();
}

HFTopic::HFTopic( FileList * other, int comp, int phrases )
{
    size_ = 12L;
    num_pages = 1;
    other_files = other;
    use_compress = comp;
    use_phrases = phrases;
    head = NULL;
    tail = NULL;
    if( comp ){
        my_writer = new CompWriter;
        my_reader = new CompReader( my_writer );
    } else {
        my_writer = NULL;
        my_reader = NULL;
    }
    cur_link = NULL;
    cur_offset = cur_charoffset = 0L;

    other->dir_file->add_file( this, "|TOPIC" );
}

HFTopic::~HFTopic()
{
    if( my_writer ) delete my_writer;
    if( my_reader ) delete my_reader;
}

unsigned long HFTopic::size()
{
    if( size_ == 0 ){
        TopicLink       *current = head;
        while( current != NULL ){
            size += current->size_;
            size += current->format->prep_block();
            size += current->data->prep_block();
            current = current->next;
        }
    }
    return size_;
}

int HFTopic::dump( OutFile * dest )
{
    if( use_compress ){
        if( my_writer ) delete my_writer;
        my_writer = new CompOutFile( dest );
        my_reader->reset( my_writer );
    }

    unsigned long       page_nums[3] = { 0xFFFFFFFF, 0x0000000C, 0x00000000 };
    TopicLink           *current = head;
    unsigned long       page_size = 12L
    unsigned long       block_size;

    while( current != NULL ){
        if( current->is_firstlink ){
            page_nums[2] = current->myoffset;
            if( use_compress ){
                my_reader->flush();
            }
            while( page_size < 4096 ){
                dest->writech( 0 );
                page_size++;
            }
            page_size = 12L;
            dest->writebuf( page_nums, 4, 3 );
        }
        if( current->is_lasttopic ){
            page_nums[3] = current->myoffset;
        }
        if( current->is_lastlink ){
            page_nums[1] = current->myoffset;
        }

        if( use_compress ){
            block_size = current->dumpto( block );
            page_size += my_reader->add( block, block_size );
            block_size = current->format->size_;
            page_size += my_reader->add( current->format->data_block,
                                              block_size );
            block_size = current->data->size_;
            page_size += my_reader->compress( current->data->data_block,
                                              block_size );
        } else {
            page_size += ( block_size = current->dumpto( block ) );
            dest->writebuf( block, 1, block_size );
            page_size += ( block_size = current->format->size_ );
            dest->writebuf( current->format->data_block, 1, block_size );
            page_size += ( block_size = current->data->size_ );
            dest->writebuf( current->format->data_block, 1, block_size );
        }

        current = current->next;
    }
    return 1;
}

HFTopic::DealWithRtf( Scanner * source )
{
    Token       *next_token;
}
