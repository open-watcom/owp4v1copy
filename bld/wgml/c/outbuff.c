/****************************************************************************
*
*                            Open Watcom Project
*
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ======================================================================
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
* Description:  Implements the functions used with to the output buffer (and
*               so with the output file or device).
*                   ob_flush()
*                   ob_insert_block()
*                   ob_insert_byte()
*                   ob_insert_ps_text_start()
*                   ob_setup()
*                   ob_teardown()
*               and this function declared in copfile.h:
*                   cop_tr_table()
*               as well as these local variables:
*                   buffout
*                   translated
*                   out_file_fb
*                   tr_table
*               and these local functions:
*                   buffer_overflow()
*                   ob_insert_ps_text()
*                   ob_insert_ps_cmd()
*                   ob_insert_ps_cmd_ot()
*                   ob_insert_def()
*                   ob_insert_def_ot()
*                   set_out_file()
*                   set_out_file_attr()
*
* Note:         The Wiki should be consulted for any term whose meaning is
*               not apparent. This should help in most cases.
****************************************************************************/

#define __STDC_WANT_LIB_EXT1__ 1
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "copfiles.h"
#include "gvars.h"
#include "outbuff.h"
#include "wgml.h"

/* Local variable declaration. */

static record_buffer    buffout     = { 0, 0, NULL };
static record_buffer    translated  = { 0, 0, NULL };
static FILE         *   out_file_fb;
static uint8_t          tr_table[0x100]; // .TR-controlled translation table

/* Static function definitions. */

/* Function buffer_overflow().
 * Handles error processing for output buffer overflow. This only occurs when
 * a multi-byte output translation or a PostScript language command is too
 * long to fit into an empty output buffer.
 *
 * Note:
 *      The message shown is, in fact, wgml 4.0 message IO--011.
 */
 
static void buffer_overflow( void )
{
    out_msg( "Output file's record size is too small for the device '%s'\n", \
                                                                    dev_name );
    err_count++;
    g_suicide();

    return;
}

/* Function ob_insert_ps_text().
 * This function inserts text for a PostScript device into the output buffer.
 * PS text elements are allowed to completely fill the output record without
 * regard to word breaks. If the text element is not closed by the end of the
 * record, a '\' is used to terminate the record and the next record starts
 * with the next character. Output translation is always done; if it produces
 * a multibyte translation, then the entire translation must be placed in the
 * same buffer.
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *      font contains the number of the font to be used.
 *
 * Notes:
 *      If this is the start of the text element, then the last item was "(" 
 *          and, if difference = 1, the resulting "(\" is correct for the end
 *          of the output record.
 *      If the text element ends on the last legal position, then inserting "\" 
 *          and moving the last character in the text element to the next buffer
 *          is correct.
 *      If the text element ends one position before the last legal position, 
 *          then the output record will correctly end with the ")" of ") sd"
 *          or ") shwd", as appropriate.
 *      Adjusting text_count as shown avoids a problem where a word was added 
 *          without a final '\', which is not how wgml 4.0 does it and which
 *          produces an extra space when the PS interpreter processes the text.
 *      If a multibyte translation is too large to fit into the buffer, then an
 *          error is reported and the program stops. This should never happen
 *          since, for device PS, the buffer has 79 characters and the longest
 *          output translations have four characters.
 */

static void ob_insert_ps_text( uint8_t * in_block, size_t count, uint8_t font )
{
    size_t              difference;
    translation *   *   cur_table   = NULL;
    translation *       cur_trans   = NULL;
    uint8_t             byte;
    uint32_t            i;

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    /* Adjust font if appropriate and initialize cur_trans. */

    if( font >= wgml_font_cnt ) font = 0;
    if( wgml_fonts[font].outtrans != NULL ) cur_table = \
                                            wgml_fonts[font].outtrans->table;
    for( i = 0; i < count; i++ ) {

        difference = buffout.length - buffout.current;
        if ( difference > 1 ) {

            /* buffout has room for at least one character. */

            byte = tr_table[in_block[i]];
            if( byte == in_block[i] ) {
                if( cur_table == NULL ) {

                    /* No output translation was found. */

                    buffout.text[buffout.current] = byte;
                    buffout.current++;
                } else {

                    /* An :OUTTRANS block exists. */

                    cur_trans = cur_table[byte];
                    if( cur_trans == NULL ) {

                        /* No output translation was found. */

                        buffout.text[buffout.current] = byte;
                        buffout.current++;
                    } else {

                        /* An :OUTTRANS block output translation was found. */

                        if( cur_trans->count == 1 ) {

                            /* This is a single-character output translation. */

                            buffout.text[buffout.current] = cur_trans->data[0];
                            buffout.current++;
                        } else {

                            /* This is a multi-character output translation. */

                            /* If it is too large to fit at all, report overflow. */

                            if( cur_trans->count > buffout.length ) buffer_overflow();

                            /* If it won't fit in the current buffer, finalize
                             * and flush the buffer.
                             */

                            if( (buffout.current + cur_trans->count + 1) > \
                                                            buffout.length ) {
                                buffout.text[buffout.current] = '\\';
                                buffout.current++;
                                ob_flush();
                            }

                            /* At this point, it is known that it will fit. */

                            memcpy_s( &buffout.text[buffout.current], \
                        cur_trans->count, cur_trans->data, cur_trans->count );
                            buffout.current += cur_trans->count;
                        }
                    }
                }
            } else {

                /* A single-byte .tr output translation was found. */

                buffout.text[buffout.current] = byte;
                buffout.current++;
            }
        } else {

            /* Finalize and flush the buffer. Adjust i so that, when
             * incremented at the top of the loop, it points to the same
             * character and that character is not skipped.
             */

            buffout.text[buffout.current] = '\\';
            buffout.current++;
            ob_flush();
            i--;
        }
    }

    return;
}

/* Function ob_insert_ps_cmd().
 * This function inserts PostScript language statements. No output
 * translation occurs.
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *
 * Notes:
 *      PostScript language statements must not be split in mid-token.
 *      When deciding whether or not a token will fit into the current output
 *          record, the space following that token must be included in the
 *          length of the token.
 *      The first space following the last token in an output record does not
 *          appear in any output record.
 */

static void ob_insert_ps_cmd( uint8_t * in_block, size_t count )
{
    size_t      current;
    size_t      difference;
    size_t      text_count;

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    /* Start at the beginning of text_block. */

    current = 0;
    text_count = count;
    while( (buffout.current + text_count) > buffout.length ) {

        difference = buffout.length - buffout.current;

        /* If the entire text_block will now fit, exit the loop. */

        if( text_count <= difference ) break;

        /* Insert any initial spaces. */

        while( in_block[current] == ' ' ) {
            if( difference == 0 ) break;
            buffout.text[buffout.current] = ' ';
            buffout.current++;
            current++;
            text_count--;
            difference--;
        } 

        /* If difference is "0", the buffer is full. */

        if( difference == 0 ) {
            ob_flush();
            continue;
        }

        /* Since in_block[current] now points at a non-space character,
         * an initial space will not cause the program to loop endlessly.
         */

        while( in_block[current + difference] != ' ' ) {
            if( difference == 0 ) {

                /* No spaces found. */

                if( buffout.current > 0 ) {

                    /* Flush the buffer, skip any space at text_block[current],
                     * and exit this loop.
                     */

                    ob_flush();
                    if( in_block[current] == ' ' ) current++;
                    break;
                } else {

                    /* The buffer is empty: the text just won't fit. */

                    buffer_overflow();
                }
            }

            difference--;
        } 

        /* If difference is "0", and we reach this point, then the buffer was
         * flushed: go to the top of the loop.
         */

        if( difference == 0 ) continue;

        if( (current + difference + 1) >= count ) {

        /* The space following the current token was the last character in
         * in_block and the token + space will not fit: the buffer is full.
         */

        } else {

            /* Copy up to the space character found. */

            memcpy_s( &buffout.text[buffout.current], difference, \
                                            &in_block[current], difference );
            buffout.current += difference;
            current+= difference;
            text_count -= difference;

            /* Skip the space character and copy any other space characters
             * up to the size of the buffer.
             */

            current++;
            text_count--;
            while( buffout.current < buffout.length ) {
                if( in_block[current] != ' ' ) break;
                    buffout.text[buffout.current] = in_block[current];
                    buffout.current++;
                    current++;
                    text_count--;
            }
        }

        /* Flush the buffer: full or not, it cannot hold the rest of
         * the remaining text.
         */

        ob_flush();
    }

    /* Insert any remaining text. */

    if( text_count > 0 ) {
        memcpy_s( &buffout.text[buffout.current], text_count, \
                                            &in_block[current], text_count );
        buffout.current += text_count;
    }

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    return;
}

/* Function ob_insert_ps_cmd_ot().
 * This function inserts PostScript language statements, with output
 * translation.
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *      font contains the number of the font to be used.
 *
 * Notes:
 *      PostScript language tokens are translated before it is determined
 *          whether or not they will fit in the current output buffer.
 *      PostScript language statements must not be split in mid-token.
 *      When deciding whether or not a token will fit into the current output
 *          record, the space following that token must be included in the
 *          length of the token.
 *      The first space following the last token in an output record does not
 *          appear in any output record.
 */

static void ob_insert_ps_cmd_ot( uint8_t * in_block, size_t count, uint8_t font )
{
    size_t              test_length;
    size_t              text_count;
    translation *   *   cur_table   = NULL;
    translation *       cur_trans   = NULL;
    uint8_t             byte;
    uint32_t            i;
    uint32_t            j;
    uint32_t            k;

    /* Adjust font if necessary and initialize cur_table and text_count. */

    if( font >= wgml_font_cnt ) font = 0;
    if( wgml_fonts[font].outtrans != NULL ) cur_table = \
                                            wgml_fonts[font].outtrans->table;
    text_count = count;
    for( i = 0; i < count; i++ ) {

        /* If the buffer is full, flush it. */

        if( buffout.current == buffout.length ) ob_flush();

        /* First process any initial space. */

        if( in_block[i] == ' ' ) {

            /* Now check for an output translation. */

            byte = tr_table[in_block[i]];
            if( byte == in_block[i] ) {
                if( cur_table == NULL ) {

                    /* No output translation was found. */

                    buffout.text[buffout.current] = byte;
                    buffout.current++;
                } else {

                    /* An :OUTTRANS block exists. */

                    cur_trans = cur_table[byte];
                    if( cur_trans == NULL ) {

                        /* No output translation was found. */

                        buffout.text[buffout.current] = byte;
                        buffout.current++;
                    } else {

                        /* An :OUTTRANS block output translation was found. */

                        if( cur_trans->count == 1 ) {

                            /* This is a single-character output translation. */

                            buffout.text[buffout.current] = cur_trans->data[0];
                            buffout.current++;
                        } else {

                            /* This is a multi-character output translation. */

                            /* If it is too large to fit at all, report overflow. */

                            if( cur_trans->count > buffout.length ) buffer_overflow();

                            /* If it won't fit, flush the buffer. */

                            if( (buffout.current + cur_trans->count) > buffout.length ) ob_flush();

                            /* At this point, it is known that it will fit. */

                            memcpy_s( &buffout.text[buffout.current], \
                        cur_trans->count, cur_trans->data, cur_trans->count );
                            buffout.current += cur_trans->count;
                        }
                    }
                }
            } else {

                /* A single-byte .tr output translation was found. */

                buffout.text[buffout.current] = byte;
                buffout.current++;
            }
            text_count--;

            /* Process next character, might be another space. */

            continue;
        } 

        /* Get the next token and translate it. */

        k = 0;
        for( j = i; j < count; j++ ) {

            /* in_block[i] points to a non-space character. */ 

            if( in_block[j] == ' ' ) break;

            /* At least one character will be added to translated. */

            if( k >= translated.length ) {
                translated.length *= 2;
                translated.text = (uint8_t *) mem_realloc( translated.text, \
                                                        translated.length );
            }

            /* Add the non-space character to translated. */

            byte = tr_table[in_block[j]];
            if( byte == in_block[j] ) {
                if( wgml_fonts[font].outtrans == NULL ) {

                    /* No translation exists: copy the character. */

                    translated.text[k] = byte;
                    k++;
                } else {

                    /* An :OUTTRANS block exists. */

                    cur_trans = cur_table[byte];
                    if( cur_trans == NULL ) {

                        /* No output translation was found. */

                        translated.text[k] = byte;
                        k++;
                    } else {

                        /* An :OUTTRANS block output translation was found. */

                        if( cur_trans->count == 1 ) {

                            /* Single-char translation found. */

                            translated.text[k] = cur_trans->data[0];
                            k++;
                        } else {

                            /* Multi-char translation found. */

                            /* If it is too large to fit at all, report overflow. */

                            if( cur_trans->count > buffout.length ) buffer_overflow();

                            if( (k + cur_trans->count) >= translated.length ) {
                                translated.length *= 2;
                                translated.text = (uint8_t *) mem_realloc( \
                                        translated.text, translated.length );
                            }

                            memcpy_s( &translated.text[k], cur_trans->count, \
                                            cur_trans->data, cur_trans->count );
                            k += cur_trans->count;
                        }
                    }
                }
            } else {

                /* A single-byte .tr translation found. */

                translated.text[k] = byte;
                k++;
            }
        }
        translated.current = k;
        i = j;

        /* If a space followed the current token, test_length must include it. */

        test_length = buffout.current + translated.current;
        if( in_block[i] == ' ' ) test_length++;

        /* If the token won't fit, flush the buffer. The increment at the top
         * of the loop will skip any following space character; since this is
         * not correct except when the buffer has been flushed, decrement i.
         * text_count is decremented if the flush occurs because the space
         * will be skipped, but not otherwise since it will be processed.
         */

        if( test_length >= buffout.length ) {
            ob_flush();
            text_count--;
        } else {
            i--;
        }

        /* Now insert the translated token into the buffer. */

        memcpy_s( &buffout.text[buffout.current], translated.current, \
                                        translated.text, translated.current );
        buffout.current += translated.current;
        text_count -= translated.current;
    }
    return;
}

/* Function ob_insert_def().
 * This function inserts a block of bytes into the output buffer. This is done
 * byte-by-byte because the block may include nulls. If the entire block won't
 * fit, as much of it as possible is copied into the buffer, which is then 
 * flushed and the rest of the block processed. No output translation occurs. 
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 */

static void ob_insert_def( uint8_t * in_block, size_t count )
{
    size_t      current;
    size_t      difference;
    size_t      text_count;

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    /* Start at the beginning of text_block. */

    current = 0;
    text_count = count;
    while( (buffout.current + text_count) > buffout.length ) {

        difference = buffout.length - buffout.current;

        /* If the entire text_block will now fit, exit the loop. */

        if( text_count <= difference ) break;

        memcpy_s( &buffout.text[buffout.current], difference, \
                                            &in_block[current], difference );
        buffout.current += difference;
        current+= difference;
        text_count -= difference;
        ob_flush();
    }

    /* Insert any remaining text. */

    if( text_count > 0 ) {
        memcpy_s( &buffout.text[buffout.current], text_count, \
                                            &in_block[current], text_count );
        buffout.current += text_count;
    }

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    return;
}

/* Function ob_insert_def_ot().
 * This function inserts a block of bytes into the output buffer with output
 * translation. This is done byte-by-byte because the block may include nulls.
 * If the entire block won't fit, as much of it as possible is copied into the
 * buffer, which is then flushed and the rest of the block processed. 
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *      font contains the number of the font to be used.
 * Notes:
 *      Multi-character output translations cannot be split between output
 *          records, so a given output record may have fewer than the maximum
 *          allowed number of characters in it.
 */

static void ob_insert_def_ot( uint8_t * in_block, size_t count, uint8_t font )
{
    size_t              text_count;
    translation *   *   cur_table   = NULL;
    translation *       cur_trans   = NULL;
    uint8_t             byte;
    uint32_t            i;

    /* Adjust font if necessary and initialize cur_table and text_count. */

    if( font >= wgml_font_cnt ) font = 0;
    if( wgml_fonts[font].outtrans != NULL ) cur_table = \
                                            wgml_fonts[font].outtrans->table;
    text_count = count;

    for( i = 0; i < count; i++ ) {

        /* If the buffer is full, flush it. */

        if( buffout.current == buffout.length ) ob_flush();

        /* Now check for an output translation. */

        byte = tr_table[in_block[i]];
        if( byte == in_block[i] ) {
            if( cur_table == NULL ) {

                /* No output translation was found. */

                buffout.text[buffout.current] = byte;
                buffout.current++;
            } else {

                /* An :OUTTRANS block exists. */

                cur_trans = cur_table[byte];
                if( cur_trans == NULL ) {

                    /* No output translation was found. */

                    buffout.text[buffout.current] = byte;
                    buffout.current++;
                } else {

                    /* An :OUTTRANS block output translation was found. */

                    if( cur_trans->count == 1 ) {

                        /* This is a single-character output translation. */

                        buffout.text[buffout.current] = cur_trans->data[0];
                        buffout.current++;
                    } else {

                        /* This is a multi-character output translation. */

                        /* If it is too large to fit at all, report overflow. */

                        if( cur_trans->count > buffout.length ) buffer_overflow();

                        /* If it won't fit in the current buffer, finalize and
                         * flush the buffer.
                         */

                        if( (buffout.current + cur_trans->count) > buffout.length ) ob_flush();

                        /* At this point, it is known that it will fit. */

                        memcpy_s( &buffout.text[buffout.current], \
                        cur_trans->count, cur_trans->data, cur_trans->count );
                        buffout.current += cur_trans->count;
                    }
                }
            }
        } else {

            /* A single-byte .tr output translation was found. */

            buffout.text[buffout.current] = byte;
            buffout.current++;
        }
        text_count--;
    }

    /* If the buffer is full, flush it. */

    if( buffout.current == buffout.length ) ob_flush();

    return;
}

/* Function set_out_file().
 * Sets the global out_file to the correct value. This can be anything from a
 * filename entered on the command line to one constructed using bits from the
 * command line and the output_filename field in the :DEVICE block.
 */

static void set_out_file( void )
{
    char        cmd_outfile[_MAX_PATH2];
    char    *   cmd_drive;
    char    *   cmd_dir;
    char    *   cmd_ext;
    char    *   cmd_fname;
    char        dev_outfile[_MAX_PATH2];
    char    *   dev_drive;
    char    *   dev_dir;
    char    *   dev_ext;
    char    *   dev_fname;
    char        doc_spec[_MAX_PATH2];
    char    *   doc_drive;
    char    *   doc_dir;
    char    *   doc_ext;
    char    *   doc_fname;
    char        temp_outfile[_MAX_PATH];

    /* Split the possible source names into their component parts. */

    if( master_fname == NULL ) {
        doc_spec[0] = '\0';
        doc_drive = &doc_spec[0];
        doc_spec[1] = '\0';
        doc_dir = &doc_spec[1];
        doc_spec[2] = '\0';
        doc_fname = &doc_spec[2];
        doc_spec[3] = '\0';
        doc_ext = &doc_spec[3];
    } else {
        _splitpath2( master_fname, doc_spec, &doc_drive, &doc_dir, &doc_fname, \
                                                                    &doc_ext );
    }

    if( out_file == NULL ) {
        cmd_outfile[0] = '\0';
        cmd_drive = &cmd_outfile[0];
        cmd_outfile[1] = '\0';
        cmd_dir = &cmd_outfile[1];
        cmd_outfile[2] = '\0';
        cmd_fname = &cmd_outfile[2];
        cmd_outfile[3] = '\0';
        cmd_ext = &cmd_outfile[3];
    } else {
        _splitpath2( out_file, cmd_outfile, &cmd_drive, &cmd_dir, &cmd_fname, \
                                                                    &cmd_ext );
    }

    if( bin_device->output_name == NULL ) {
        dev_outfile[0] = '\0';
        dev_drive = &dev_outfile[0];
        dev_outfile[1] = '\0';
        dev_dir = &dev_outfile[1];
        dev_outfile[2] = '\0';
        dev_fname = &dev_outfile[2];
        dev_outfile[3] = '\0';
        dev_ext = &dev_outfile[3];
    } else {
        _splitpath2( bin_device->output_name, dev_outfile, &dev_drive, &dev_dir, \
                                                        &dev_fname, &dev_ext );
    }

    /* Ensure it is possible to tell if a file name was constructed. */

    temp_outfile[0] = '\0';

    /* Construct the file name, if necessary. If the command-line option OUTput
     * was used and both a filename and and extension were given, then cmd_fname
     * will be used as-is and temp_outfile will not be touched.
     */

    if( *cmd_fname != '\0' ) {
        if( *cmd_fname != '*' ) {
            if( *cmd_ext != '\0' ) {

            /* If both name and extension were given on the command line, use
             * out_file as-is.
             */

            } else {

            /* If the name was given on the command line, use out_file with the
             * extension given in the :DEVICE block.
             */

                _makepath( temp_outfile, cmd_drive, cmd_dir, cmd_fname, \
                                                bin_device->output_extension );
            }
        } else {
            if( *cmd_ext != '\0' ) {

            /* If the name was not given but an extension was given on the command
             * line, use the document specification name with the extension given.
             */

                _makepath( temp_outfile, cmd_drive, cmd_dir, doc_fname, cmd_ext );
            } else {

            /* If neither a specific name nor an extension was given on the
             * command line, use use the document specification name with the
             * extension given in the :DEVICE block.
             */

                _makepath( temp_outfile, cmd_drive, cmd_dir, doc_fname, \
                                                bin_device->output_extension );
            }
        }
    } else {
        if( (*cmd_drive != '\0') || (*cmd_dir != '\0') ) {

            /* Command line OPTION was used with something like "c:" or "..\" but
             * with no filename or extension.
             */

                _makepath( temp_outfile, cmd_drive, cmd_dir, doc_fname, \
                                                bin_device->output_extension );
        } else {

            /* The situation here is that command-line option OUTPUT was not
             * used with only a drive letter and/or a path but with no file name,
             * not even "*", and no extension. In other words, it was not used
             * at all.
             */

            if( (*dev_fname != '*') && (*dev_fname != '\0') ) {

                /* If the :DEVICE block specified a file name then use the file
                 * name and any extension provided.
                 */

                _makepath( temp_outfile, "", "", bin_device->output_name, \
                                                bin_device->output_extension );
            } else {

                /* If the :DEVICE block did not specify a file name then use the
                 * document specification name with any extension provided.
                 */

                _makepath( temp_outfile, dev_drive, dev_dir, doc_fname, \
                                                bin_device->output_extension );
            }
        }
    }

    /* If a file name was constructed, update out_file with it. */

    if( temp_outfile[0] != '\0' ) {
        if( out_file != NULL ) mem_free( out_file );
        out_file = (char *) mem_alloc( strnlen_s( temp_outfile, _MAX_PATH ) + 1 );
        strcpy_s( out_file, _MAX_PATH, temp_outfile );
    }

    return;
}

/* Function set_out_file_attr().
 * Sets the global out_file_attr to the correct value. This will be either the
 * record type entered on the command line, the spec_rec field in the :DRIVER
 * block, or the default record type "t:132".
 */

static void set_out_file_attr( void )
{
    size_t      len;

    /* Construct the output file record type if necessary. If the command-line
     * option OUTput was used and a record type was given, then out_file_attr
     * will be used as-is. Otherwise, the rec_spec will be used if it is
     * properly formatted. If all else fails, the default will be used.
     */

    if( out_file_attr == NULL ) {
        if( bin_driver->rec_spec != NULL ) {
            len = strlen( bin_driver->rec_spec );
            if( (bin_driver->rec_spec[0] != '(') || \
                                        (bin_driver->rec_spec[len - 1] != ')')) {

                /* Use default if rec_spec is badly-formed. */

                len = 1 + strlen( "t:132" );
                out_file_attr = mem_alloc( len );
                strcpy_s( out_file_attr, len, "t:132" );

            } else {

                /* Copy the record type itself, without parentheses, into
                 * out_file_attr.
                 */

                len -= 1;
                out_file_attr = mem_alloc( len );
                memcpy_s( out_file_attr, len, \
                            &bin_driver->rec_spec[1], len - 1 );
                out_file_attr[len - 1] = '\0';
            }
        } else {

            /* Use default if rec_spec is missing. */

            size_t len = 1 + strlen( "t:132" );
            out_file_attr = mem_alloc( len );
            strcpy_s( out_file_attr, len, "t:132" );
        }
    }

    return;
}

/* Global function definitions. */

/* Function cop_tr_table().
 * Updates tr_table as specified by the data.
 *
 * Parameters:
 *      data contains any data associated with the .TR control word.
 *      count contains the number of bytes to process.
 *
 * Note:
 *      Whatever terminated the .TR record must not be part of the data.
 */

void cop_tr_table( uint8_t * data, uint32_t count )
{

    bool        first_found;
    bool        no_data;
    int         i;
    int         j;
    size_t      token_length;
    uint8_t     first_char;
    uint8_t     hex_string[3];
    uint8_t     token_char;

    /* Set up the variables. count must be "0" if there is no data. no_data
     * will cause the table to be reset if it is still "true" after the loop.
     */

    if( data == NULL ) count = 0;
    first_found = false;
    no_data = true;

    for( i = 0; i < count; i++ ) {

        /* Get the next token, if any. */

        if( isspace( data[i] ) ) continue;

        /* A token! */

        token_length = 0;
        for( j = i; j < count; j++) {
            if( isspace( data[j] ) ) break;
            token_length += 1;
        }

        /* If there was no token, then we are done. */

        if( token_length == 0 ) break;

        /* Validate the token. */

        if( token_length > 2 ) {

            /* The maximum allowed token length is 2. */

            uint8_t * text = (uint8_t *) mem_alloc( token_length + 1 );
            memcpy_s( text, token_length, &data[i], token_length );
            text[token_length] = '\0';        

            out_msg( "A single character or a two character hexadecimal value "\
                                    "must be specified: '%s'\n", text );
            mem_free( text );
            err_count++;
            g_suicide();
        }

        /* If the token length is 2, it must be a hexadecimal number. */

        if( token_length == 2 ) {

            hex_string[0] = data[i];
            hex_string[1] = data[i + 1];
            hex_string[2] = '\0';

            if( !isxdigit( data[i] ) || !isxdigit( data[i + 1] ) ) {
                out_msg( "A single character or a two character hexadecimal " \
                            "value must be specified: '%s'\n", hex_string );
                err_count++;
                g_suicide();
            }

            token_char = (uint8_t) strtol( hex_string, NULL, 16 );
        } else {
            token_char = data[i];
        }

        /* Reset i to skip the token and note that data has been found. */

        i = j;
        no_data = false;

        /* If we have an unused first_char, then we just found the char it
         * is to be converted into. Otherwise, we found a new first_char.
         */

        if( first_found ) {
            tr_table[first_char] = token_char;
            first_found = false;
        } else {
            first_char = token_char;
            first_found = true;
        }
    }

    /* If first_found is true at this point, then a single character was
     * found at the end of the data and the table must be updated to return
     * the same character.
     */

    if( first_found ) tr_table[first_char] = first_char;

    /* If there was no data, reset the table. */

    if( no_data ) for( i = 0; i < 0x100; i++ ) tr_table[i] = i;

    return;
}

/* Function ob_flush().
 * This function actually flushes the output buffer to the output device/file.
 *
 * Notes:
 *      The output file is, and must be, opened in binary mode. This requires
 *          the explicit emission of "\r\n" for non-Linux versions. For Linux,
 *          "\n" is emitted, but, since I am not able to test the Linux version,
 *          it is not possible to tell is this is correct.
 *      Since PostScript is a printer language, it is possible that it will
 *          require "\r\n" even under Linux, even in a software interpreter.
 *          Then again, the Linux version of Ghostscript, for example, may
 *          well expect "\n". It is possible, then, that the Linux code will
 *          need to distinguish between PS and other devices.
 */

void ob_flush( void )
{
    
    fwrite( buffout.text, sizeof( uint8_t ), buffout.current, out_file_fb );
    buffout.current = 0;
#ifdef __UNIX__
    fprintf_s( out_file_fb, "\n" );
#else
    fprintf_s( out_file_fb, "\r\n" );
#endif
    return;
}

/* Function ob_insert_block().
 * This function inserts a block of bytes into the output buffer by invoking
 * the appropriate static function based on flags and parameters. The block
 * of bytes may contains nulls.
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *      out_trans is true if the bytes are to be translated before insertion.
 *      out_text is true is the bytes are to appear in the document itself.
 */

void ob_insert_block( uint8_t * in_block, size_t count, bool out_trans, \
                              bool out_text, uint8_t font )
{
    /* Select and invoke the proper static function. */

    if( ps_device )  {
        if( out_text ) {
            ob_insert_ps_text( in_block, count, font );
        } else {
            if( out_trans ) {
                ob_insert_ps_cmd_ot( in_block, count, font );
            } else {
                ob_insert_ps_cmd( in_block, count );
            }
        }
    } else {
        if( out_trans ) {
            ob_insert_def_ot( in_block, count, font );
        } else {
            ob_insert_def( in_block, count );
        }
    }
    return;
}

/* Function ob_insert_byte().
 * This function inserts a single byte into the output buffer.
 *
 * Parameter:
 *      in_char contains the byte to be inserted.
 */

void ob_insert_byte( uint8_t in_char )
{
    /* Flush the buffer if it is full. */

    if( buffout.current == buffout.length ) ob_flush();

    /* Insert the character and increment the current position pointer. */

    buffout.text[buffout.current] = in_char;
    buffout.current++;

    return;
}

/* Function ob_insert_ps_text_start().
 * This function inserts the text start character for the PS device.
 *
 * Note:
 *      This is only called by pre_text_output() for the PS device, to insert
 *      the "(" before text. It turns out that ending the record with "("
 *      results in the newline produced by ob_flush() appearing as an extra
 *      space in the document, and that this is not how it is done by wgml 4.0.
 */

void ob_insert_ps_text_start( void )
{
    /* Flush the buffer if it is full or has only one character position left. */

    if( buffout.current >= (buffout.length - 1) ) ob_flush();

    /* Insert '(' and increment the current position pointer. */

    buffout.text[buffout.current] = '(';
    buffout.current++;

    return;
}

/* Function ob_setup().
 * Determines the output file/device name and format. Opens the output
 * file/device (if appropriate) and initializes the output buffer itself.
 */

void ob_setup( void )
{
    int     i;
    size_t  count;

    /* Finalize out_file and out_file_attr. */

    set_out_file();
    set_out_file_attr();

    /* Only record type "t" is currently supported. */

    if( tolower( out_file_attr[0] ) != 't' ) {
        out_msg( "File type %c is not currently supported\n", out_file_attr[0] );
        err_count++;
        g_suicide();
    }

    /* The record type must have only one character and be followed by ":" */

    if( out_file_attr[1] != ':' ) {
        out_msg( "File type %c%c is not currently supported\n", \
                                            out_file_attr[0], out_file_attr[1] );
        err_count++;
        g_suicide();
    }

    /* The rest of the record type must be numeric. */

    count = 0;

    for( i = 2; i < strlen( out_file_attr ); i++ ) {
        if( !isdigit( out_file_attr[i] ) ) {
            out_msg( "File type %s is not formatted correctly\n", out_file_attr );
            err_count++;
            g_suicide();
        }
        count++;
    }

    /* Initialize the local variables. */

    buffout.current = 0;
    buffout.length = strtoul( &out_file_attr[2], NULL, 0 );
    if( errno == ERANGE ) {
        out_msg( "Output record length is too large, max is %i\n", ULONG_MAX );
        err_count++;
        g_suicide();
    }
    buffout.text = (uint8_t *) mem_alloc( buffout.length );

    translated.current = 0;
    translated.length = 80;
    translated.text = (uint8_t *) mem_alloc( translated.length );

    /* Create (truncate) the output file. */

    fopen_s( &out_file_fb, out_file, "uwb" );

    if( out_file_fb == NULL ) {
        out_msg( "Unable to open out-file %s\n", out_file );
        err_count++;
        g_suicide();
    }

    /* Initialize tr_table. */

    for( i = 0; i < 0x100; i++) {
        tr_table[i] = i;
    }


    return;
}

/* Function ob_teardown().
 * Determines the output file/device name and format. Opens the output
 * file/device (if appropriate) and initializes the output buffer itself.
 */

void ob_teardown( void )
{
    if( buffout.text != NULL ) {
        mem_free( buffout.text );
        buffout.text = NULL;
    }

    if(translated.text != NULL ) { 
        mem_free( translated.text );
        translated.text = NULL;
    }

    if( out_file_fb != NULL ) {
        fclose( out_file_fb );
        out_file_fb = NULL;
    }

    return;
}

