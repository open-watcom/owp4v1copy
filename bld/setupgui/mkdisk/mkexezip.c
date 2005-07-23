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
* Description:  Utility to create installer with zip-file at end of it.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "zipint.h"

#define BUFFER_SIZE 16384

#pragma pack(push);
#pragma pack(1);
typedef struct {
    char signature[4];
    uint16_t disk_number; // not supported by libzip
    uint16_t disk_having_cd; // not supported by libzip
    uint16_t num_entries_on_disk;  // not supported by libzip
    uint16_t total_num_entries;

    uint32_t cd_size;
    uint32_t cd_offset;
    uint16_t comment_length;

} ZIP_END_OF_CENTRAL_DIRECTORY;

typedef struct {
    char signature[4];
    uint16_t version_made_by;
    uint16_t version_needed;
    uint16_t flags;
    uint16_t method;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;

    uint16_t file_name_length;
    uint16_t extra_field_length;
    uint16_t file_comment_length;

    uint16_t disk; // not supported by libzip

    uint16_t int_attrib;
    uint32_t ext_attrib;
    uint32_t offset;

} ZIP_CENTRAL_DIRECTORY_FILE_HEADER;

#pragma pack(pop);

static char usage[] = "%s <target exe> <source zip>\n";

static int find(char *buffer, uint32_t buffer_len, char *match, uint32_t match_len);
static int fix_cd(FILE *f, char *buffer, uint32_t buffer_len, uint32_t offset);

int main(int argc, char *argv[]) {
    FILE *ftarget;
    FILE *fsource;
    char *buffer = NULL;
    char *prg;
    char *target;
    char *source;

    uint32_t offset;
    uint32_t length;
    size_t n;
    int pos;

    prg = argv[0];

    if (argc < 3) {
	fprintf(stderr, usage, prg);
	exit(2);
    }

    target = argv[1];
    source = argv[2];

    ftarget = fopen(target, "r+b");
    if (ftarget == NULL) {
        fprintf(stderr, "can't open %s for writing\n", target);
        return 1;
    }

    fsource = fopen(source, "rb");
    if (fsource == NULL) {
        fprintf(stderr, "can't open %s for reading\n", source);
        fclose(ftarget);
        return 1;
    }

    // seek at end of target and save it's original size
    fseek(ftarget, 0, SEEK_END);
    offset = ftell(ftarget);

    buffer = (char *)malloc(BUFFER_SIZE);
    if (buffer == NULL) {
        fprintf(stderr, "not enough memory for buffer\n");
        fclose(ftarget);
        fclose(fsource);
        return 1;
    }

    // --- copy zip ---
    while((n = fread(buffer, 1, BUFFER_SIZE, fsource)) != 0) {
        if (fwrite(buffer, 1, n, ftarget) != n) {
            fprintf(stderr, "can't write to %s\n", target);
            fclose(ftarget);
            fclose(fsource);
            free(buffer);
            return 1;
        }
    }

    // source zip is now copied
    fclose(fsource);

    // save length of target for later usage
    length = ftell(ftarget);

    // --- fixup offsets ---

    // find end of central directory
    fseek(ftarget, -((length < BUFFER_SIZE) ? length : BUFFER_SIZE), SEEK_END);
    n = fread(buffer, 1, BUFFER_SIZE, ftarget);

    if (n == 0) {
        fprintf(stderr, "odd, failed to read data from end of file %s\n", target);
        fclose(ftarget);
        free(buffer);
        return 1;
    }

    pos = 0;
    while(pos != -1)
    {
        pos = find(buffer+pos, n-pos, EOCD_MAGIC, strlen(EOCD_MAGIC));
        if (pos == -1) {
            fprintf(stderr, "error, no zip magic found from %s\n", target);
            fclose(ftarget);
            free(buffer);
            return 1;
        }

        if (fix_cd(ftarget, buffer + pos, n - pos, offset))
        {
            // fixup also eocd
            ZIP_END_OF_CENTRAL_DIRECTORY eocd;
            uint32_t off;

            off = -((length < BUFFER_SIZE) ? length : BUFFER_SIZE);
            off+= pos;

            fseek(ftarget, off, SEEK_END);

            if (fread(&eocd, sizeof(ZIP_END_OF_CENTRAL_DIRECTORY), 1, ftarget) != 1)
            {
                fprintf(stderr, "file read error: %s\n", target);
                fclose(ftarget);
                free(buffer);
                return 1;
            }

            if (find((char *)&eocd, 4, EOCD_MAGIC, 4) != 0)
            {
                fprintf(stderr, "file read error while checking eocd magic: %s\n", target);
                fclose(ftarget);
                free(buffer);
                return 1;
            }

            eocd.cd_offset += offset;

            fseek(ftarget, off, SEEK_END);
            fwrite(&eocd, sizeof(ZIP_END_OF_CENTRAL_DIRECTORY), 1, ftarget);

            break;
        } else
        {
            pos++;
        }
    }

    // --- cleanup  ---
    fclose(ftarget);
    free(buffer);
    return 0;
}

static int find(char *buffer, uint32_t buffer_len, char *match, uint32_t match_len) {
    char *ptr = buffer;
    int i;

    while(ptr != NULL)
    {
        // find startup character
        ptr = memchr(ptr, match[0], buffer_len - (ptr - buffer));
        if (ptr == NULL) break;

        // make sure there is room for our match
        if (((ptr - buffer) + match_len) > buffer_len) {
            break;
        }

        // check for match
        for (i = 1; i < match_len; i++) {
            if (ptr[i] != match[i])
                break;
        }

        // if match found, return it's position in buffer
        if (i == match_len) {
            return ptr - buffer;
        }

        // skip to next occurance
        ptr++;
    }

    return -1;
}

static int fix_cd(FILE *f, char *buffer, uint32_t buffer_len, uint32_t offset)
{
    ZIP_END_OF_CENTRAL_DIRECTORY *eocd;
    ZIP_CENTRAL_DIRECTORY_FILE_HEADER fileheader;
    uint32_t header_pos;
    uint32_t i;

    eocd = (ZIP_END_OF_CENTRAL_DIRECTORY *)buffer;

    header_pos = offset + eocd->cd_offset;

    // fixup all files in central directory
    for (i = 0; i < eocd->total_num_entries; i++)
    {
        fseek(f, header_pos, SEEK_SET);

        if (fread(&fileheader, sizeof(ZIP_CENTRAL_DIRECTORY_FILE_HEADER), 1, f) != 1)
        {
            return 0;
        }

        // make sure we are at correct position in file
        if (find((char *)&fileheader, 4, CENTRAL_MAGIC, 4) != 0)
        {
            return 0;
        }

        fileheader.offset += offset;
        fseek(f, header_pos, SEEK_SET);
        fwrite(&fileheader, sizeof(ZIP_CENTRAL_DIRECTORY_FILE_HEADER), 1, f);

        // move to next entry
        header_pos += sizeof(ZIP_CENTRAL_DIRECTORY_FILE_HEADER);
        header_pos += fileheader.file_name_length;
        header_pos += fileheader.extra_field_length;
        header_pos += fileheader.file_comment_length;
    }

    return 1;
}
