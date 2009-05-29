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
* Description:  Implements the functions used with to the output buffer (and
*               so with the output file or device).
*                   ob_flush()
*                   ob_insert_block()
*                   ob_insert_byte()
*                   ob_setup()
*                   ob_teardown()
*               as well as these local variables:
*                   buffout
*                   out_file_fb
*               and these local functions:
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

static record_buffer    *   buffout;
static record_buffer    *   translated;
static FILE             *   out_file_fb;

/* Static function definitions. */

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

/* Function ob_flush().
 * This function actually flushes the output buffer to the output device/file.
 *
 * Notes:
 *      The output file is, and must be, opened in binary mode. This requires
 *          the explicit emission of "\c\n" for non-Linux versions. For Linux,
 *          "\n" is emitted, but, since I am not able to test the Linux version,
 *          it is not possible to tell is this is correct.
 *      Since PostScript is a printer language, it is possible that it will
 *          require "\c\n" even under Linux, even in a software interpreter.
 *          Then again, the Linux version of Ghostscript, for example, may
 *          well expect "\n". It is possible, then, that the Linux code will
 *          need to distinguish between PS and other devices.
 *      This implementation implicitly assumes that either a word will fit or
 *          it will be part of the next line. Handling over-long groups of
 *          letters is actually more complicated and may eventually require
 *          that a larger buffer be allocated than the record format calls for
 *          or that a second parameter be passed to suppress the "newline"
 *          when appropriate. This was discovered while attempting to discover
 *          whether a multicharacter output translation is split over two
 *          buffer flushes. It appears that it is not, so a second function that
 *          handles output translation will be needed eventually.
 */

void ob_flush( void )
{
    fwrite( buffout->text, sizeof( uint8_t ), buffout->current, out_file_fb );
    buffout->current = 0;
#ifdef __UNIX__
    fprintf_s( out_file_fb, "\n" );
#else
    fprintf_s( out_file_fb, "\r\n" );
#endif
    return;
}

/* Function ob_insert_block().
 * This function inserts a block of bytes into the output buffer. This is done
 * byte-by-byte because the block may include nulls.
 *
 * Parameter:
 *      in_block contains the bytes to be inserted.
 *      count contains the number of bytes in the block.
 *      out_trans is true if the bytes are to be translated before insertion.
 *      out_text is true is the bytes are to appear in the document itself.
 *
 * Notes:
 *      This implementation does not actually do anything special when out_text
 *          is set to "true".
 */

extern void ob_insert_block( uint8_t * in_block, size_t count, bool out_trans, \
                              bool out_text, uint32_t font )
{
    char    *   text_block  = NULL;
    size_t      current;
    size_t      difference;
    size_t      text_count;

    if( out_trans == true ) {

        /* Do the output translation of in_block. */

        translated = cop_out_trans( in_block, count, translated, font );

        text_block = translated->text;
        text_count = translated->current;

    } else {

        /* No output translation needed. */

        text_block = in_block;
        text_count = count;
    }

    /* Start at the beginning of text_block. */

    current = 0;

    /* If the block won't fit, copy as much as possible into the buffer
     * and then flush it.
     */

    while( (buffout->current + text_count) > buffout->length ) {
        difference = buffout->length - buffout->current;

        /* Distinguish PS devices from non-PS devices. */

        if( ps_device )  {

            /* For PS devices, out_text matters. */

            if( out_text ) {

                /* Implement when text output is being implemented. */

            } else {

                /* PS language statements must not be split in mid-word.
                 * In fact, the out_buff must end with a space since the
                 * out_buff cannot contain the entire remaining text_block.
                 *
                 * At this point, these invariants hold:
                 *  buffout->current + text_count > buffout->length
                 *  difference = buffout->length - buffout->current
                 * Thus: buffout->current + difference = buffout->length
                 *  current <= buffout->current
                 *  text_block[current + difference] <=
                 *      text_block[buffout->current + difference] =
                 *      text_block[buffout->length] <=
                 *      text_block[buffout->current + text_count]
                 * Thus: text_block[current + difference] must access a
                 *  valid location in text_block.
                 */

                if( text_block[current + difference] != ' ' ) {
                    while( text_block[current + difference] != ' ' ) {
                        difference--;
                        if( difference == 0 ) {

                            /* No spaces found. If buffout is not empty, then
                             * flush it, skip any initial space in text_block,
                             * and exit this loop.
                             */

                            if( buffout->current > 0 ) {
                                ob_flush();
                                if( text_block[current] == ' ' ) current++;
                                break;
                            } else {

                            /* It just won't fit. */
                                out_msg( "Output file's record size is too "\
                                     "small for the device '%s'\n", dev_name );
                                err_count++;
                                g_suicide();
                            }
                        }
                    }

                    /* If difference is "0", and we reach this point, then
                     * the buffer was flushed: go to the top of the loop.
                     */

                    if( difference == 0 ) continue;
                } 
                
                /* Copy up to the space character found. */

                memcpy_s( &buffout->text[buffout->current], difference, \
                                            &text_block[current], difference );
                buffout->current += difference;
                current+= difference;
                text_count -= difference;

                /* Skip the space character and copy any other space
                 * characters up to the size of the buffer.
                 */

                current++;
                while( buffout->current < buffout->length ) {
                    if( text_block[current] != ' ' ) break;
                    buffout->text[buffout->current] = ' ';
                    buffout->current++;
                }

                /* If the buffer is full, flush it. */

                if( buffout->current == buffout->length ) ob_flush();
            }

        } else {

            /* For non-PS devices, stuff the buffer. */

            memcpy_s( &buffout->text[buffout->current], difference, \
                                            &text_block[current], difference );
            buffout->current += difference;
            current+= difference;
            text_count -= difference;
            ob_flush();
        }
    }

    /* Insert any remaining text, but do not flush the buffer. */

    if( text_count > 0 ) {
        memcpy_s( &buffout->text[buffout->current], text_count, \
                                            &text_block[current], text_count );

        buffout->current += text_count;
    }
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

    if( buffout->current == buffout->length ) ob_flush();

    /* Insert the character and increment the current position pointer. */

    buffout->text[buffout->current] = in_char;
    buffout->current++;

    return;
}

/* Function ob_setup().
 * Determines the output file/device name and format. Opens the output
 * file/device (if appropriate) and initializes the output buffer itself.
 */

extern void ob_setup( void )
{
    int     i;
    size_t  count;

    /* Finalize out_file and out_file_attr. */

    set_out_file();
    set_out_file_attr();

    /* Validate out_file_attr. */

    buffout = NULL;

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

    buffout = (record_buffer *) mem_alloc( sizeof( record_buffer ) );
    buffout->current = 0;
    buffout->length = strtoul( &out_file_attr[2], NULL, 0 );
    if( errno == ERANGE ) {
        out_msg( "Output buffer length is too large, max is %i\n", ULONG_MAX );
        err_count++;
        g_suicide();
    }
    buffout->text = (uint8_t * ) mem_alloc( buffout->length );

    translated = (record_buffer *) mem_alloc( sizeof( record_buffer ) );
    translated->current = 0;
    translated->length = 80;
    translated->text = (uint8_t * ) mem_alloc( translated->length );

    /* Create (truncate) the output file. */

    fopen_s( &out_file_fb, out_file, "uwb" );

    if( out_file_fb == NULL ) {
        out_msg( "Unable to open out-file %s\n", out_file );
        err_count++;
        g_suicide();
    }

    return;
}

/* Function ob_teardown().
 * Determines the output file/device name and format. Opens the output
 * file/device (if appropriate) and initializes the output buffer itself.
 */
extern void ob_teardown( void )
{
    if( buffout != NULL ) {
        mem_free( buffout->text );
        mem_free( buffout );
        buffout = NULL;
    }

    if(translated != NULL ) { 
        mem_free( translated->text );
        mem_free( translated );
        translated = NULL;
    }

    if( out_file_fb != NULL ) {
        fclose( out_file_fb );
        out_file_fb = NULL;
    }

    return;
}

