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


#include "ftnstd.h"

static const char __FAR Msg0[] = {
 0
,0
/* argument number %d1 (%s2) in call to %s3 must be CHARACTER*%u4 or bigger but size passed was %d5  */
,18,5,12,15,25,3,22,2,16,40,18,165,13,210,21,68,24,14,144
/* argument number %d1 (%s2) in call to %s3 - actual argument was %s4 but dummy argument was %s5  */
,18,5,12,15,25,3,22,2,16,9,199,5,14,101,21,79,5,14,102
/* argument number %d1 - %s2 was passed to an intrinsic function  */
,11,5,12,15,9,45,14,24,2,26,83,20
/* argument number %d1 (%s2) in call to %s3 - a subroutine was passed but a function was expected  */
,18,5,12,15,25,3,22,2,16,9,17,94,14,24,21,17,20,14,38
/* number of arguments expected in call to %s1 was %d2, but found %d3  */
,13,12,4,205,38,3,22,2,8,14,142,21,61,143
/* argument number %d1 (%s2) in call to %s3 - a function was passed but a subroutine was expected  */
,18,5,12,15,25,3,22,2,16,9,17,20,14,24,21,17,94,14,38
/* argument number %d1 (%s2) in call to %s3 - %s4 argument was passed but %s5 argument was expected  */
,18,5,12,15,25,3,22,2,16,9,101,5,14,24,21,102,5,14,38
/* argument number %d1 (%s2) in call to %s3 - type of structure passed does not match type of structure exp  */
,20,5,12,15,25,3,22,2,16,9,31,4,42,24,35,0,65,31,4,42,230
,0
/* argument number %d1 (%s2) in call to %s3 - argument passed by value but expected by reference  */
,18,5,12,15,25,3,22,2,16,9,5,24,58,72,21,38,58,255,9
};
static const char __FAR Msg1536[] = {
/* program abnormally terminated  */
 2,135,197
};
static const char __FAR Msg1792[] = {
 0
,0
/* left and right hand sides overlap in a character assignment statement  */
,8,247,137,235,3,17,49,206,11
};
static const char __FAR Msg2048[] = {
 0
,0
,0
,0
,0
,0
,0
,0
/* not enough constants for list of variables  */
,7,0,224,118,23,28,4,98
,0
/* too many constants for list of variables  */
,7,96,250,118,23,28,4,98
,0
/* cannot initialize %s1 variable with %s2 constant  */
,7,114,240,8,55,56,45,78
/* entity can only be initialized once during data initialization  */
,6,225,18,241,51,27,239
};
static const char __FAR Msg2304[] = {
 0
/* array or array element (possibly substrung) associated with %s1 too small  */
,9,48,13,48,223,56,8,96,255,15
};
static const char __FAR Msg2560[] = {
 0
,0
,0
,0
,0
,0
,0
/* incrementation parameter for DO-loop cannot be zero  */
,6,238,23,167,114,18,44
};
static const char __FAR Msg3840[] = {
/* zero**J where J <= 0 is not allowed  */
 7,255,39,99,182,1,0,32
/* X**Y where X < 0.0, Y is not of type INTEGER, is not allowed  */
,13,194,99,108,153,109,1,0,4,31,179,1,0,32
/* (0,0)**Y where Y is not real is not allowed  */
,10,147,99,109,1,0,255,8,1,0,32
};
static const char __FAR Msg4352[] = {
/* missing delimiter in format string, comma assumed  */
 7,29,219,3,10,255,19,112
/* missing or invalid constant  */
,4,29,13,6,78
/* Ew.dDe format code is not FORTRAN 77 standard  */
,6,172,10,34,1,0,46
/* missing decimal point  */
,2,29,218
/* missing or invalid edit descriptor in format string  */
,8,29,13,6,36,50,3,10,138
/* unrecognizable edit descriptor in format string  */
,7,255,34,36,50,3,10,138
/* invalid repeat specification  */
,3,6,136,54
/* $ or \ format code is not FORTRAN 77 standard  */
,8,141,13,196,10,34,1,0,46
/* invalid field modifier  */
,4,6,59,255,0
,0
/* repeat specification not allowed for this format code  */
,8,136,54,0,32,23,139,10,34
,0
/* no closing quote on apostrophe edit descriptor  */
,6,86,214,87,202,36,50
/* field count greater than 256 is invalid  */
,6,59,120,81,151,1,6
/* invalid field width specification  */
,5,6,59,255,36,54
/* Z format code is not FORTRAN 77 standard  */
,6,195,10,34,1,0,46
/* FORMAT statement exceeds allotted storage size  */
,4,73,11,228,68
/* format specification not allowed on input  */
,6,10,54,0,32,87,63
/* FORMAT missing repeatable edit descriptor  */
,6,73,29,255,11,36,50
/* missing constant before X edit descriptor, 1 assumed  */
,7,29,78,209,108,36,220,112
/* Ew.dQe format code is not FORTRAN 77 standard  */
,6,173,10,34,1,0,46
/* Qw.d format code is not FORTRAN 77 standard  */
,6,187,10,34,1,0,46
};
static const char __FAR Msg5888[] = {
/* BACKSPACE statement requires sequential access mode  */
 6,156,11,41,67,47,85
/* input/output is already active  */
,4,39,1,111,110
/* ENDFILE statement requires sequential access mode  */
,6,171,11,41,67,47,85
/* formatted connection requires formatted input/output statements  */
,6,80,117,41,80,39,69
/* unformatted connection requires unformatted input/output statements  */
,6,97,117,41,97,39,69
/* REWIND statement requires sequential access  */
,5,188,11,41,67,47
/* bad character in input field  */
,5,208,49,3,63,59
/* BLANK specifier requires FORM specifier to be 'FORMATTED'  */
,8,157,7,41,175,7,2,18,145
/* system file error - %s1  */
,6,255,24,19,52,9,8
/* format specification does not match data type  */
,7,10,54,35,0,65,27,31
/* input item does not match the data type of list variable  */
,11,63,244,35,0,65,70,27,31,4,28,55
/* internal file is full  */
,4,242,19,1,233
/* RECL specifier is invalid  */
,4,107,7,1,6
/* invalid STATUS specifier in CLOSE statement  */
,6,6,76,7,3,166,11
/* unit specified is not connected  */
,5,71,92,1,0,116
/* attempt to perform data transfer past end of file  */
,9,33,2,255,5,27,255,27,4,19
/* invalid RECL specifier/ACCESS specifier combination  */
,6,6,107,255,16,7,77
/* REC specifier required in direct access input/output statements  */
,9,75,7,255,12,3,222,47,39,69
/* REC specifier not allowed in sequential access input/output statements  */
,9,75,7,0,32,3,67,47,39,69
/* %s1 specifier may not change in a subsequent OPEN statement  */
,10,8,7,252,0,213,3,17,255,21,11
/* invalid STATUS specifier for given file  */
,6,6,76,7,23,125,19
/* invalid STATUS specifier/FILE specifier combination  */
,6,6,76,255,17,7,77
/* record size exceeded during unformatted input/output  */
,6,66,68,37,51,97,39
/* unit specified does not exist  */
,5,71,92,35,0,124
/* REC specifier is invalid  */
,4,75,7,1,6
/* UNIT specifier is invalid  */
,4,191,7,1,6
/* formatted record or format edit descriptor is too large for record size  */
,12,80,66,13,10,36,50,1,96,246,23,66,68
/* illegal '%s1=' specifier  */
,2,237,7
/* invalid CARRIAGECONTROL specifier/FORM specifier combination  */
,4,6,161,7,77
/* i/o operation not consistent with file attributes  */
,7,82,134,0,215,56,19,207
/* symbol %s1 not found in NAMELIST  */
,7,255,22,8,0,61,3,184
/* syntax error during NAMELIST-directed input  */
,6,255,23,52,51,74,63
/* subscripting error during NAMELIST-directed i/o  */
,6,255,20,52,51,74,82
/* substring error during NAMELIST-directed i/o  */
,5,30,52,51,74,82
/* BLOCKSIZE specifier is invalid  */
,4,158,7,1,6
/* invalid operation for files with no record structure  */
,8,6,134,23,232,56,86,66,42
/* integer overflow converting character data to integer  */
,7,64,89,119,49,27,2,64
/* range exceeded converting character data to floating-point  */
,7,90,37,119,49,27,2,60
};
static const char __FAR Msg6144[] = {
/* floating-point divide by zero  */
 4,60,122,58,44
/* floating-point overflow  */
,2,60,89
/* floating-point underflow  */
,3,60,255,30
/* integer divide by zero  */
,4,64,122,58,44
/* program interrupted from keyboard  */
,4,135,243,62,245
/* integer overflow  */
,2,64,89
/* maximum pages of output exceeded  */
,5,251,4,255,4,37
/* statement count has been exceeded  */
,4,11,120,126,37
/* time limit exceeded  */
,4,255,25,130,37
};
static const char __FAR Msg6400[] = {
/* argument must be greater than zero  */
 5,5,40,18,81,44
/* absolute value of argument to arcsine, arccosine must not exceed one  */
,10,198,72,4,5,2,203,40,0,227,133
/* argument must not be negative  */
,6,5,40,0,18,255,1
/* argument(s) must not be zero  */
,5,204,40,0,18,44
/* argument of CHAR must be in the range zero to 255  */
,11,5,4,162,40,18,3,70,90,44,2,150
,0
,0
,0
/* length of ICHAR argument greater than one  */
,6,248,4,178,5,81,133
};
static const char __FAR Msg6912[] = {
 0
/* out of memory  */
,3,88,4,253
,0
/* attempt to deallocate an unallocated array or character*(*) variable  */
,10,33,2,217,26,255,29,48,13,115,55
/* attempt to allocate an already allocated array or character*(*) variable  */
,10,33,2,200,26,111,201,48,13,115,55
};
static const char __FAR Msg7168[] = {
 0
/* missing or misplaced opening parenthesis  */
,3,29,13,254
,0
/* unmatched parentheses  */
,2,255,33
};
static const char __FAR Msg8960[] = {
 0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
/* attempt to invoke active function/subroutine  */
,5,33,2,128,110,234
/* dummy argument %s1 is not in dummy argument list of entered subprogram  */
,12,79,5,8,1,0,3,79,5,28,4,123,93
/* function referenced as %s1 but defined to be %s2  */
,8,20,91,8,21,121,2,18,45
/* function referenced as CHARACTER*%u1 but defined to be CHARACTER*%u2  */
,8,20,91,163,21,121,2,18,164
,0
/* subroutine referenced as a function  */
,4,94,91,17,20
/* attempt to invoke a block data subprogram  */
,7,33,2,128,17,211,27,93
/* structure type of function %s1 does not match expected structure type  */
,11,42,31,4,20,8,35,0,65,38,42,31
};
static const char __FAR Msg9216[] = {
 0
,0
/* subscript expression out of range; %s1 does not exist  */
,10,95,53,88,4,255,7,8,35,0,124
/* substring expression (%i1:%i2) is out of range  */
,7,30,53,146,1,88,4,90
};
static const char __FAR Msg9472[] = {
 0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
/* no FORMAT statement with given label  */
,6,86,73,11,56,125,129
/* statement number not in list or not the label of an executable statement  */
,13,11,12,0,3,28,13,0,70,129,4,26,229,11
/* attempt to branch into a control structure  */
,6,33,2,212,17,216,42
};
static const char __FAR Msg9728[] = {
 0
,0
/* invalid subscript expression  */
,3,6,95,53
,0
,0
,0
,0
,0
/* limit of 65535 elements per dimension has been exceeded  */
,5,130,4,152,126,37
};
static const char __FAR Msg10496[] = {
/* substring (%u1:%u2) of expression on right side of equal sign is undefined  */
 12,30,57,4,53,87,137,255,14,4,226,1,43
/* substring (%u1:%u2) of return value of character function %s3 is undefined  */
,12,30,57,4,255,13,72,4,49,20,16,1,43
/* substring (%u1:%u2) of an operand in a relational expression is undefined  */
,13,30,57,4,26,255,3,3,17,255,10,53,1,43
/* substring (%u1:%u2) of an argument to an intrinsic function is undefined  */
,11,30,57,4,26,5,2,26,83,20,1,43
/* the value of %s1 is undefined  */
,6,70,72,4,8,1,43
/* argument number %d1 passed to intrinsic function is undefined  */
,9,5,12,15,24,2,83,20,1,43
};
static const char __FAR Msg11008[] = {
 0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
/* external subprogram  */
,2,231,93
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
/* - Executing line %u1 in file %s2  */
,6,9,106,131,3,19,45
/* - Called from line %u1 in file %s2  */
,7,9,105,62,131,3,19,45
/* error writing to listing file - %s1  */
,8,52,255,37,2,132,19,9,8
/* - Executing in  */
,3,9,106,3
/* - Called from  */
,3,9,105,62
/* %s1, statement %u2 in library member %s3  */
,6,100,11,103,3,249,16
/* %s1, statement %u2 in file %s3  */
,6,100,11,103,3,19,16
/* Do you really want to stop (y/n)?  */
,4,170,2,255,18
/* *ERR* math coprocessor is not present\r\n  */
,5,148,1,0,255,6
/* invalid ISN  */
,2,6,180
/* Debugger entered at %s1  */
,4,168,123,113,8
/* Breakpoint set at %d1  */
,3,159,113,15
/* Returned from %s1  */
,3,189,62,8
/* Into[I key] Over[space bar] Next[N key] Exit[Enter key]  */
,6,181,84,185,84,174,84
/* variable not found  */
,3,55,0,61
/* array dimensions not yet established  */
,5,48,221,0,255,38
/* not understood  */
,3,0,255,31
/* invalid substring list  */
,3,6,30,28
/* invalid subscript list  */
,3,6,95,28
/* unit not found  */
,3,71,0,61
/* unable to open listing file  */
,7,255,28,2,255,2,132,19
/* Breakpoint[/c] [<isn>].....show/set/clear breakpoint  */
,1,160
/* Display [*|<isn>[:<isn>]]..display source  */
,1,169
/* Go [<isn>].................go [until <isn> reached]  */
,1,176
/* Help.......................display this help screen  */
,3,177,139,236
/* Logio [<unit>].............list logged units or toggle logging of unit  */
,6,183,13,255,26,4,71
/* Quit.......................quit debugger  */
,1,186
/* Trace......................enter trace mode  */
,2,190,85
/* Unit [<unit>]..............list information for connected unit(s)  */
,6,192,127,23,116,255,32
/* Where......................display current traceback information  */
,2,193,127
/* ! [<cmd>]..................system services  */
,1,140
/* / <x> <y>..................assign <y> to <x>  */
,5,149,104,154,2,104
/* ? [(z|a)] [<x>[,<x>]...]...print variables using specified format  */
,6,155,98,255,35,92,10
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
,0
};


extern const char __FAR * const __FAR GroupTable[] = {
     Msg0
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,Msg1536
    ,Msg1792
    ,Msg2048
    ,Msg2304
    ,Msg2560
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,Msg3840
    ,NULL
    ,Msg4352
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,Msg5888
    ,Msg6144
    ,Msg6400
    ,NULL
    ,Msg6912
    ,Msg7168
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,NULL
    ,Msg8960
    ,Msg9216
    ,Msg9472
    ,Msg9728
    ,NULL
    ,NULL
    ,Msg10496
    ,NULL
    ,Msg11008
    };


extern const char __FAR ErrWord[] = {
          3,'n','o','t'
/* count= 35 Group:offset = AR: 9, DA: 8, EX: 0, EX: 1, EX: 2,... */
          ,2,'i','s'
/* count= 28 Group:offset = EX: 0, EX: 1, EX: 2, FM: 2, FM: 7,... */
          ,2,'t','o'
/* count= 28 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 6,... */
          ,2,'i','n'
/* count= 27 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,2,'o','f'
/* count= 27 Group:offset = AR: 6, AR: 9, DA: 8, DA:10, EX: 1,... */
          ,8,'a','r','g','u','m','e','n','t'
/* count= 22 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 7,... */
          ,7,'i','n','v','a','l','i','d'
/* count= 20 Group:offset = FM: 1, FM: 4, FM: 6, FM: 8, FM:13,... */
          ,9,'s','p','e','c','i','f','i','e','r'
/* count= 15 Group:offset = IO: 7, IO:12, IO:13, IO:16, IO:17,... */
          ,3,'%','s','1'
/* count= 14 Group:offset = AR: 6, DA:12, DM: 1, IO: 8, IO:19,... */
          ,1,'-'
/* count= 13 Group:offset = AR: 3, AR: 4, AR: 5, AR: 7, AR: 8,... */
          ,6,'f','o','r','m','a','t'
/* count= 13 Group:offset = FM: 0, FM: 2, FM: 4, FM: 5, FM: 7,... */
          ,9,'s','t','a','t','e','m','e','n','t'
/* count= 13 Group:offset = CV: 2, FM:16, IO: 0, IO: 2, IO: 5,... */
          ,6,'n','u','m','b','e','r'
/* count= 11 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 6,... */
          ,2,'o','r'
/* count= 11 Group:offset = AR: 2, DM: 1, FM: 1, FM: 4, FM: 7,... */
          ,3,'w','a','s'
/* count= 11 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 6,... */
          ,3,'%','d','1'
/* count= 10 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 7,... */
          ,3,'%','s','3'
/* count= 10 Group:offset = AR: 2, AR: 3, AR: 5, AR: 7, AR: 8,... */
          ,1,'a'
/* count= 10 Group:offset = AR: 5, AR: 7, CV: 2, IO:19, SR:17,... */
          ,2,'b','e'
/* count= 10 Group:offset = AR: 2, DA:13, DO: 7, IO: 7, LI: 0,... */
          ,4,'f','i','l','e'
/* count= 10 Group:offset = IO: 8, IO:11, IO:15, IO:20, IO:29,... */
          ,8,'f','u','n','c','t','i','o','n'
/* count= 10 Group:offset = AR: 4, AR: 5, AR: 7, SR:14, SR:15,... */
          ,3,'b','u','t'
/* count=  9 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,4,'c','a','l','l'
/* count=  8 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,3,'f','o','r'
/* count=  8 Group:offset = DA: 8, DA:10, DO: 7, FM:10, IO:20,... */
          ,6,'p','a','s','s','e','d'
/* count=  8 Group:offset = AR: 2, AR: 4, AR: 5, AR: 7, AR: 8,... */
          ,5,'(','%','s','2',')'
/* count=  7 Group:offset = AR: 2, AR: 3, AR: 5, AR: 7, AR: 8,... */
          ,2,'a','n'
/* count=  7 Group:offset = AR: 4, MO: 3, MO: 4, ST:22, UV: 2,... */
          ,4,'d','a','t','a'
/* count=  7 Group:offset = DA:13, IO: 9, IO:10, IO:15, IO:36,... */
          ,4,'l','i','s','t'
/* count=  7 Group:offset = DA: 8, DA:10, IO:10, SR:13, ST:22,... */
          ,7,'m','i','s','s','i','n','g'
/* count=  7 Group:offset = FM: 0, FM: 1, FM: 3, FM: 4, FM:18,... */
          ,9,'s','u','b','s','t','r','i','n','g'
/* count=  7 Group:offset = IO:33, SS: 3, UV: 0, UV: 1, UV: 2,... */
          ,4,'t','y','p','e'
/* count=  7 Group:offset = AR: 9, EX: 1, IO: 9, IO:10, SR:19,... */
          ,7,'a','l','l','o','w','e','d'
/* count=  6 Group:offset = EX: 0, EX: 1, EX: 2, FM:10, FM:17,... */
          ,7,'a','t','t','e','m','p','t'
/* count=  6 Group:offset = IO:15, MO: 3, MO: 4, SR:12, SR:18,... */
          ,4,'c','o','d','e'
/* count=  6 Group:offset = FM: 2, FM: 7, FM:10, FM:15, FM:20,... */
          ,4,'d','o','e','s'
/* count=  6 Group:offset = AR: 9, IO: 9, IO:10, IO:23, SR:19,... */
          ,4,'e','d','i','t'
/* count=  6 Group:offset = FM: 4, FM: 5, FM:12, FM:18, FM:19,... */
          ,8,'e','x','c','e','e','d','e','d'
/* count=  6 Group:offset = IO:22, IO:37, KO: 6, KO: 7, KO: 8,... */
          ,8,'e','x','p','e','c','t','e','d'
/* count=  6 Group:offset = AR: 5, AR: 6, AR: 7, AR: 8, AR:11,... */
          ,12,'i','n','p','u','t','/','o','u','t'
              ,'p','u','t'
/* count=  6 Group:offset = IO: 1, IO: 3, IO: 4, IO:17, IO:18,... */
          ,4,'m','u','s','t'
/* count=  6 Group:offset = AR: 2, LI: 0, LI: 1, LI: 2, LI: 3,... */
          ,8,'r','e','q','u','i','r','e','s'
/* count=  6 Group:offset = IO: 0, IO: 2, IO: 3, IO: 4, IO: 5,... */
          ,9,'s','t','r','u','c','t','u','r','e'
/* count=  6 Group:offset = AR: 9, IO:35, SR:19, ST:23,... */
          ,9,'u','n','d','e','f','i','n','e','d'
/* count=  6 Group:offset = UV: 0, UV: 1, UV: 2, UV: 3, UV: 4,... */
          ,4,'z','e','r','o'
/* count=  6 Group:offset = DO: 7, KO: 0, KO: 3, LI: 0, LI: 3,... */
          ,3,'%','s','2'
/* count=  5 Group:offset = AR: 4, DA:12, SR:14, MS:128, MS:129 */
          ,19,'F','O','R','T','R','A','N',' ','7'
              ,'7',' ','s','t','a','n','d','a','r','d'
/* count=  5 Group:offset = FM: 2, FM: 7, FM:15, FM:20, FM:21 */
          ,6,'a','c','c','e','s','s'
/* count=  5 Group:offset = IO: 0, IO: 2, IO: 5, IO:17, IO:18 */
          ,5,'a','r','r','a','y'
/* count=  5 Group:offset = DM: 1, MO: 3, MO: 4, MS:143 */
          ,9,'c','h','a','r','a','c','t','e','r'
/* count=  5 Group:offset = CV: 2, IO: 6, IO:36, IO:37, UV: 1 */
          ,10,'d','e','s','c','r','i','p','t','o'
              ,'r'
/* count=  5 Group:offset = FM: 4, FM: 5, FM:12, FM:18, IO:26 */
          ,6,'d','u','r','i','n','g'
/* count=  5 Group:offset = DA:13, IO:22, IO:31, IO:32, IO:33 */
          ,5,'e','r','r','o','r'
/* count=  5 Group:offset = IO: 8, IO:31, IO:32, IO:33, MS:130 */
          ,10,'e','x','p','r','e','s','s','i','o'
              ,'n'
/* count=  5 Group:offset = SS: 2, SS: 3, SV: 2, UV: 0, UV: 2 */
          ,13,'s','p','e','c','i','f','i','c','a'
              ,'t','i','o','n'
/* count=  5 Group:offset = FM: 6, FM:10, FM:14, FM:17, IO: 9 */
          ,8,'v','a','r','i','a','b','l','e'
/* count=  5 Group:offset = DA:12, IO:10, MO: 3, MO: 4, MS:142 */
          ,4,'w','i','t','h'
/* count=  5 Group:offset = DA:12, DM: 1, IO:29, IO:35, ST:21 */
          ,9,'(','%','u','1',':','%','u','2',')'
/* count=  4 Group:offset = UV: 0, UV: 1, UV: 2, UV: 3 */
          ,2,'b','y'
/* count=  4 Group:offset = AR:11, KO: 0, KO: 3 */
          ,5,'f','i','e','l','d'
/* count=  4 Group:offset = FM: 8, FM:13, FM:14, IO: 6 */
          ,14,'f','l','o','a','t','i','n','g','-'
              ,'p','o','i','n','t'
/* count=  4 Group:offset = IO:37, KO: 0, KO: 1, KO: 2 */
          ,5,'f','o','u','n','d'
/* count=  4 Group:offset = AR: 6, IO:30, MS:142, MS:147 */
          ,4,'f','r','o','m'
/* count=  4 Group:offset = KO: 4, MS:129, MS:132, MS:140 */
          ,5,'i','n','p','u','t'
/* count=  4 Group:offset = FM:17, IO: 6, IO:10, IO:31 */
          ,7,'i','n','t','e','g','e','r'
/* count=  4 Group:offset = IO:36, KO: 3, KO: 5 */
          ,5,'m','a','t','c','h'
/* count=  4 Group:offset = AR: 9, IO: 9, IO:10, SR:19 */
          ,6,'r','e','c','o','r','d'
/* count=  4 Group:offset = IO:22, IO:26, IO:35 */
          ,10,'s','e','q','u','e','n','t','i','a'
              ,'l'
/* count=  4 Group:offset = IO: 0, IO: 2, IO: 5, IO:18 */
          ,4,'s','i','z','e'
/* count=  4 Group:offset = AR: 2, FM:16, IO:22, IO:26 */
          ,10,'s','t','a','t','e','m','e','n','t'
              ,'s'
/* count=  4 Group:offset = IO: 3, IO: 4, IO:17, IO:18 */
          ,3,'t','h','e'
/* count=  4 Group:offset = IO:10, LI: 4, ST:22, UV: 4 */
          ,4,'u','n','i','t'
/* count=  4 Group:offset = IO:14, IO:23, MS:147, MS:153 */
          ,5,'v','a','l','u','e'
/* count=  4 Group:offset = AR:11, LI: 1, UV: 1, UV: 4 */
          ,6,'F','O','R','M','A','T'
/* count=  3 Group:offset = FM:16, FM:18, ST:21 */
          ,17,'N','A','M','E','L','I','S','T','-'
              ,'d','i','r','e','c','t','e','d'
/* count=  3 Group:offset = IO:31, IO:32, IO:33 */
          ,3,'R','E','C'
/* count=  3 Group:offset = IO:17, IO:18, IO:24 */
          ,6,'S','T','A','T','U','S'
/* count=  3 Group:offset = IO:13, IO:20, IO:21 */
          ,11,'c','o','m','b','i','n','a','t','i'
              ,'o','n'
/* count=  3 Group:offset = IO:16, IO:21, IO:28 */
          ,8,'c','o','n','s','t','a','n','t'
/* count=  3 Group:offset = DA:12, FM: 1, FM:19 */
          ,5,'d','u','m','m','y'
/* count=  3 Group:offset = AR: 3, SR:13 */
          ,9,'f','o','r','m','a','t','t','e','d'
/* count=  3 Group:offset = IO: 3, IO:26 */
          ,12,'g','r','e','a','t','e','r',' ','t'
              ,'h','a','n'
/* count=  3 Group:offset = FM:13, LI: 0, LI: 8 */
          ,3,'i','/','o'
/* count=  3 Group:offset = IO:29, IO:32, IO:33 */
          ,9,'i','n','t','r','i','n','s','i','c'
/* count=  3 Group:offset = AR: 4, UV: 3, UV: 5 */
          ,4,'k','e','y',']'
/* count=  3 Group:offset = MS:141 */
          ,4,'m','o','d','e'
/* count=  3 Group:offset = IO: 0, IO: 2, MS:155 */
          ,2,'n','o'
/* count=  3 Group:offset = FM:12, IO:35, ST:21 */
          ,2,'o','n'
/* count=  3 Group:offset = FM:12, FM:17, UV: 0 */
          ,3,'o','u','t'
/* count=  3 Group:offset = MO: 1, SS: 2, SS: 3 */
          ,8,'o','v','e','r','f','l','o','w'
/* count=  3 Group:offset = IO:36, KO: 1, KO: 5 */
          ,5,'r','a','n','g','e'
/* count=  3 Group:offset = IO:37, LI: 4, SS: 3 */
          ,13,'r','e','f','e','r','e','n','c','e'
              ,'d',' ','a','s'
/* count=  3 Group:offset = SR:14, SR:15, SR:17 */
          ,9,'s','p','e','c','i','f','i','e','d'
/* count=  3 Group:offset = IO:14, IO:23, MS:160 */
          ,10,'s','u','b','p','r','o','g','r','a'
              ,'m'
/* count=  3 Group:offset = SR:13, SR:18, MS:110 */
          ,10,'s','u','b','r','o','u','t','i','n'
              ,'e'
/* count=  3 Group:offset = AR: 5, AR: 7, SR:17 */
          ,9,'s','u','b','s','c','r','i','p','t'
/* count=  3 Group:offset = SS: 2, SV: 2, MS:146 */
          ,3,'t','o','o'
/* count=  3 Group:offset = DA:10, DM: 1, IO:26 */
          ,11,'u','n','f','o','r','m','a','t','t'
              ,'e','d'
/* count=  3 Group:offset = IO: 4, IO:22 */
          ,9,'v','a','r','i','a','b','l','e','s'
/* count=  3 Group:offset = DA: 8, DA:10, MS:160 */
          ,5,'w','h','e','r','e'
/* count=  3 Group:offset = EX: 0, EX: 1, EX: 2 */
          ,4,'%','s','1',','
/* count=  2 Group:offset = MS:133, MS:134 */
          ,3,'%','s','4'
/* count=  2 Group:offset = AR: 3, AR: 8 */
          ,3,'%','s','5'
/* count=  2 Group:offset = AR: 3, AR: 8 */
          ,3,'%','u','2'
/* count=  2 Group:offset = MS:133, MS:134 */
          ,3,'<','x','>'
/* count=  2 Group:offset = MS:159 */
          ,6,'C','a','l','l','e','d'
/* count=  2 Group:offset = MS:129, MS:132 */
          ,9,'E','x','e','c','u','t','i','n','g'
/* count=  2 Group:offset = MS:128, MS:131 */
          ,4,'R','E','C','L'
/* count=  2 Group:offset = IO:12, IO:16 */
          ,1,'X'
/* count=  2 Group:offset = EX: 1, FM:19 */
          ,1,'Y'
/* count=  2 Group:offset = EX: 1, EX: 2 */
          ,6,'a','c','t','i','v','e'
/* count=  2 Group:offset = IO: 1, SR:12 */
          ,7,'a','l','r','e','a','d','y'
/* count=  2 Group:offset = IO: 1, MO: 4 */
          ,7,'a','s','s','u','m','e','d'
/* count=  2 Group:offset = FM: 0, FM:19 */
          ,2,'a','t'
/* count=  2 Group:offset = MS:138, MS:139 */
          ,6,'c','a','n','n','o','t'
/* count=  2 Group:offset = DA:12, DO: 7 */
          ,13,'c','h','a','r','a','c','t','e','r'
              ,'*','(','*',')'
/* count=  2 Group:offset = MO: 3, MO: 4 */
          ,9,'c','o','n','n','e','c','t','e','d'
/* count=  2 Group:offset = IO:14, MS:156 */
          ,10,'c','o','n','n','e','c','t','i','o'
              ,'n'
/* count=  2 Group:offset = IO: 3, IO: 4 */
          ,9,'c','o','n','s','t','a','n','t','s'
/* count=  2 Group:offset = DA: 8, DA:10 */
          ,10,'c','o','n','v','e','r','t','i','n'
              ,'g'
/* count=  2 Group:offset = IO:36, IO:37 */
          ,5,'c','o','u','n','t'
/* count=  2 Group:offset = FM:13, KO: 7 */
          ,7,'d','e','f','i','n','e','d'
/* count=  2 Group:offset = SR:14, SR:15 */
          ,6,'d','i','v','i','d','e'
/* count=  2 Group:offset = KO: 0, KO: 3 */
          ,7,'e','n','t','e','r','e','d'
/* count=  2 Group:offset = SR:13, MS:138 */
          ,5,'e','x','i','s','t'
/* count=  2 Group:offset = IO:23, SS: 2 */
          ,5,'g','i','v','e','n'
/* count=  2 Group:offset = IO:20, ST:21 */
          ,8,'h','a','s',' ','b','e','e','n'
/* count=  2 Group:offset = KO: 7, SV: 8 */
          ,11,'i','n','f','o','r','m','a','t','i'
              ,'o','n'
/* count=  2 Group:offset = MS:156, MS:157 */
          ,6,'i','n','v','o','k','e'
/* count=  2 Group:offset = SR:12, SR:18 */
          ,5,'l','a','b','e','l'
/* count=  2 Group:offset = ST:21, ST:22 */
          ,5,'l','i','m','i','t'
/* count=  2 Group:offset = KO: 8, SV: 8 */
          ,8,'l','i','n','e',' ','%','u','1'
/* count=  2 Group:offset = MS:128, MS:129 */
          ,7,'l','i','s','t','i','n','g'
/* count=  2 Group:offset = MS:130, MS:148 */
          ,3,'o','n','e'
/* count=  2 Group:offset = LI: 1, LI: 8 */
          ,9,'o','p','e','r','a','t','i','o','n'
/* count=  2 Group:offset = IO:29, IO:35 */
          ,7,'p','r','o','g','r','a','m'
/* count=  2 Group:offset = CP: 0, KO: 4 */
          ,6,'r','e','p','e','a','t'
/* count=  2 Group:offset = FM: 6, FM:10 */
          ,5,'r','i','g','h','t'
/* count=  2 Group:offset = CV: 2, UV: 0 */
          ,6,'s','t','r','i','n','g'
/* count=  2 Group:offset = FM: 4, FM: 5 */
          ,4,'t','h','i','s'
/* count=  2 Group:offset = FM:10, MS:152 */
          ,42,'!',' ','[','<','c','m','d','>',']'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','s','y'
              ,'s','t','e','m',' ','s','e','r','v','i'
              ,'c','e','s'
/* count=  1 Group:offset = MS:158 */
          ,1,'$'
/* count=  1 Group:offset = FM: 7 */
          ,4,'%','d','2',','
/* count=  1 Group:offset = AR: 6 */
          ,3,'%','d','3'
/* count=  1 Group:offset = AR: 6 */
          ,3,'%','d','5'
/* count=  1 Group:offset = AR: 2 */
          ,11,'\'','F','O','R','M','A','T','T','E'
              ,'D','\''
/* count=  1 Group:offset = IO: 7 */
          ,9,'(','%','i','1',':','%','i','2',')'
/* count=  1 Group:offset = SS: 3 */
          ,8,'(','0',',','0',')','*','*','Y'
/* count=  1 Group:offset = EX: 2 */
          ,22,'*','E','R','R','*',' ','m','a','t'
              ,'h',' ','c','o','p','r','o','c','e','s'
              ,'s','o','r'
/* count=  1 Group:offset = MS:136 */
          ,1,'/'
/* count=  1 Group:offset = MS:159 */
          ,3,'2','5','5'
/* count=  1 Group:offset = LI: 4 */
          ,3,'2','5','6'
/* count=  1 Group:offset = FM:13 */
          ,28,'6','5','5','3','5',' ','e','l','e'
              ,'m','e','n','t','s',' ','p','e','r',' '
              ,'d','i','m','e','n','s','i','o','n'
/* count=  1 Group:offset = SV: 8 */
          ,6,'<',' ','0','.','0',','
/* count=  1 Group:offset = EX: 1 */
          ,31,'<','y','>','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','a','s','s','i','g','n',' ','<'
              ,'y','>'
/* count=  1 Group:offset = MS:159 */
          ,32,'?',' ','[','(','z','|','a',')',']'
              ,' ','[','<','x','>','[',',','<','x','>'
              ,']','.','.','.',']','.','.','.','p','r'
              ,'i','n','t'
/* count=  1 Group:offset = MS:160 */
          ,9,'B','A','C','K','S','P','A','C','E'
/* count=  1 Group:offset = IO: 0 */
          ,5,'B','L','A','N','K'
/* count=  1 Group:offset = IO: 7 */
          ,9,'B','L','O','C','K','S','I','Z','E'
/* count=  1 Group:offset = IO:34 */
          ,14,'B','r','e','a','k','p','o','i','n'
              ,'t',' ','s','e','t'
/* count=  1 Group:offset = MS:139 */
          ,52,'B','r','e','a','k','p','o','i','n'
              ,'t','[','/','c',']',' ','[','<','i','s'
              ,'n','>',']','.','.','.','.','.','s','h'
              ,'o','w','/','s','e','t','/','c','l','e'
              ,'a','r',' ','b','r','e','a','k','p','o'
              ,'i','n','t'
/* count=  1 Group:offset = MS:149 */
          ,30,'C','A','R','R','I','A','G','E','C'
              ,'O','N','T','R','O','L',' ','s','p','e'
              ,'c','i','f','i','e','r','/','F','O','R'
              ,'M'
/* count=  1 Group:offset = IO:28 */
          ,4,'C','H','A','R'
/* count=  1 Group:offset = LI: 4 */
          ,13,'C','H','A','R','A','C','T','E','R'
              ,'*','%','u','1'
/* count=  1 Group:offset = SR:15 */
          ,13,'C','H','A','R','A','C','T','E','R'
              ,'*','%','u','2'
/* count=  1 Group:offset = SR:15 */
          ,13,'C','H','A','R','A','C','T','E','R'
              ,'*','%','u','4'
/* count=  1 Group:offset = AR: 2 */
          ,5,'C','L','O','S','E'
/* count=  1 Group:offset = IO:13 */
          ,7,'D','O','-','l','o','o','p'
/* count=  1 Group:offset = DO: 7 */
          ,8,'D','e','b','u','g','g','e','r'
/* count=  1 Group:offset = MS:138 */
          ,41,'D','i','s','p','l','a','y',' ','['
              ,'*','|','<','i','s','n','>','[',':','<'
              ,'i','s','n','>',']',']','.','.','d','i'
              ,'s','p','l','a','y',' ','s','o','u','r'
              ,'c','e'
/* count=  1 Group:offset = MS:150 */
          ,18,'D','o',' ','y','o','u',' ','r','e'
              ,'a','l','l','y',' ','w','a','n','t'
/* count=  1 Group:offset = MS:135 */
          ,7,'E','N','D','F','I','L','E'
/* count=  1 Group:offset = IO: 2 */
          ,6,'E','w','.','d','D','e'
/* count=  1 Group:offset = FM: 2 */
          ,6,'E','w','.','d','Q','e'
/* count=  1 Group:offset = FM:20 */
          ,10,'E','x','i','t','[','E','n','t','e'
              ,'r'
/* count=  1 Group:offset = MS:141 */
          ,4,'F','O','R','M'
/* count=  1 Group:offset = IO: 7 */
          ,51,'G','o',' ','[','<','i','s','n','>'
              ,']','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','g','o'
              ,' ','[','u','n','t','i','l',' ','<','i'
              ,'s','n','>',' ','r','e','a','c','h','e'
              ,'d',']'
/* count=  1 Group:offset = MS:151 */
          ,34,'H','e','l','p','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','d','i'
              ,'s','p','l','a','y'
/* count=  1 Group:offset = MS:152 */
          ,5,'I','C','H','A','R'
/* count=  1 Group:offset = LI: 8 */
          ,8,'I','N','T','E','G','E','R',','
/* count=  1 Group:offset = EX: 1 */
          ,3,'I','S','N'
/* count=  1 Group:offset = MS:137 */
          ,6,'I','n','t','o','[','I'
/* count=  1 Group:offset = MS:141 */
          ,6,'J',' ','<','=',' ','0'
/* count=  1 Group:offset = EX: 0 */
          ,44,'L','o','g','i','o',' ','[','<','u'
              ,'n','i','t','>',']','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','l','i'
              ,'s','t',' ','l','o','g','g','e','d',' '
              ,'u','n','i','t','s'
/* count=  1 Group:offset = MS:153 */
          ,8,'N','A','M','E','L','I','S','T'
/* count=  1 Group:offset = IO:30 */
          ,22,'O','v','e','r','[','s','p','a','c'
              ,'e',' ','b','a','r',']',' ','N','e','x'
              ,'t','[','N'
/* count=  1 Group:offset = MS:141 */
          ,40,'Q','u','i','t','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','q','u'
              ,'i','t',' ','d','e','b','u','g','g','e'
              ,'r'
/* count=  1 Group:offset = MS:154 */
          ,4,'Q','w','.','d'
/* count=  1 Group:offset = FM:21 */
          ,6,'R','E','W','I','N','D'
/* count=  1 Group:offset = IO: 5 */
          ,8,'R','e','t','u','r','n','e','d'
/* count=  1 Group:offset = MS:140 */
          ,38,'T','r','a','c','e','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','e','n'
              ,'t','e','r',' ','t','r','a','c','e'
/* count=  1 Group:offset = MS:155 */
          ,4,'U','N','I','T'
/* count=  1 Group:offset = IO:25 */
          ,31,'U','n','i','t',' ','[','<','u','n'
              ,'i','t','>',']','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','l','i'
              ,'s','t'
/* count=  1 Group:offset = MS:156 */
          ,52,'W','h','e','r','e','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','d','i'
              ,'s','p','l','a','y',' ','c','u','r','r'
              ,'e','n','t',' ','t','r','a','c','e','b'
              ,'a','c','k'
/* count=  1 Group:offset = MS:157 */
          ,4,'X','*','*','Y'
/* count=  1 Group:offset = EX: 1 */
          ,1,'Z'
/* count=  1 Group:offset = FM:15 */
          ,1,'\\'
/* count=  1 Group:offset = FM: 7 */
          ,21,'a','b','n','o','r','m','a','l','l'
              ,'y',' ','t','e','r','m','i','n','a','t'
              ,'e','d'
/* count=  1 Group:offset = CP: 0 */
          ,8,'a','b','s','o','l','u','t','e'
/* count=  1 Group:offset = LI: 1 */
          ,6,'a','c','t','u','a','l'
/* count=  1 Group:offset = AR: 3 */
          ,8,'a','l','l','o','c','a','t','e'
/* count=  1 Group:offset = MO: 4 */
          ,9,'a','l','l','o','c','a','t','e','d'
/* count=  1 Group:offset = MO: 4 */
          ,10,'a','p','o','s','t','r','o','p','h'
              ,'e'
/* count=  1 Group:offset = FM:12 */
          ,18,'a','r','c','s','i','n','e',',',' '
              ,'a','r','c','c','o','s','i','n','e'
/* count=  1 Group:offset = LI: 1 */
          ,11,'a','r','g','u','m','e','n','t','('
              ,'s',')'
/* count=  1 Group:offset = LI: 3 */
          ,9,'a','r','g','u','m','e','n','t','s'
/* count=  1 Group:offset = AR: 6 */
          ,10,'a','s','s','i','g','n','m','e','n'
              ,'t'
/* count=  1 Group:offset = CV: 2 */
          ,10,'a','t','t','r','i','b','u','t','e'
              ,'s'
/* count=  1 Group:offset = IO:29 */
          ,3,'b','a','d'
/* count=  1 Group:offset = IO: 6 */
          ,6,'b','e','f','o','r','e'
/* count=  1 Group:offset = FM:19 */
          ,6,'b','i','g','g','e','r'
/* count=  1 Group:offset = AR: 2 */
          ,5,'b','l','o','c','k'
/* count=  1 Group:offset = SR:18 */
          ,11,'b','r','a','n','c','h',' ','i','n'
              ,'t','o'
/* count=  1 Group:offset = ST:23 */
          ,6,'c','h','a','n','g','e'
/* count=  1 Group:offset = IO:19 */
          ,13,'c','l','o','s','i','n','g',' ','q'
              ,'u','o','t','e'
/* count=  1 Group:offset = FM:12 */
          ,10,'c','o','n','s','i','s','t','e','n'
              ,'t'
/* count=  1 Group:offset = IO:29 */
          ,7,'c','o','n','t','r','o','l'
/* count=  1 Group:offset = ST:23 */
          ,10,'d','e','a','l','l','o','c','a','t'
              ,'e'
/* count=  1 Group:offset = MO: 3 */
          ,13,'d','e','c','i','m','a','l',' ','p'
              ,'o','i','n','t'
/* count=  1 Group:offset = FM: 3 */
          ,9,'d','e','l','i','m','i','t','e','r'
/* count=  1 Group:offset = FM: 0 */
          ,13,'d','e','s','c','r','i','p','t','o'
              ,'r',',',' ','1'
/* count=  1 Group:offset = FM:19 */
          ,10,'d','i','m','e','n','s','i','o','n'
              ,'s'
/* count=  1 Group:offset = MS:143 */
          ,6,'d','i','r','e','c','t'
/* count=  1 Group:offset = IO:17 */
          ,39,'e','l','e','m','e','n','t',' ','('
              ,'p','o','s','s','i','b','l','y',' ','s'
              ,'u','b','s','t','r','u','n','g',')',' '
              ,'a','s','s','o','c','i','a','t','e','d'
/* count=  1 Group:offset = DM: 1 */
          ,6,'e','n','o','u','g','h'
/* count=  1 Group:offset = DA: 8 */
          ,15,'e','n','t','i','t','y',' ','c','a'
              ,'n',' ','o','n','l','y'
/* count=  1 Group:offset = DA:13 */
          ,10,'e','q','u','a','l',' ','s','i','g'
              ,'n'
/* count=  1 Group:offset = UV: 0 */
          ,6,'e','x','c','e','e','d'
/* count=  1 Group:offset = LI: 1 */
          ,24,'e','x','c','e','e','d','s',' ','a'
              ,'l','l','o','t','t','e','d',' ','s','t'
              ,'o','r','a','g','e'
/* count=  1 Group:offset = FM:16 */
          ,10,'e','x','e','c','u','t','a','b','l'
              ,'e'
/* count=  1 Group:offset = ST:22 */
          ,3,'e','x','p'
/* count=  1 Group:offset = AR: 9 */
          ,8,'e','x','t','e','r','n','a','l'
/* count=  1 Group:offset = MS:110 */
          ,5,'f','i','l','e','s'
/* count=  1 Group:offset = IO:35 */
          ,4,'f','u','l','l'
/* count=  1 Group:offset = IO:11 */
          ,19,'f','u','n','c','t','i','o','n','/'
              ,'s','u','b','r','o','u','t','i','n','e'
/* count=  1 Group:offset = SR:12 */
          ,18,'h','a','n','d',' ','s','i','d','e'
              ,'s',' ','o','v','e','r','l','a','p'
/* count=  1 Group:offset = CV: 2 */
          ,11,'h','e','l','p',' ','s','c','r','e'
              ,'e','n'
/* count=  1 Group:offset = MS:152 */
          ,14,'i','l','l','e','g','a','l',' ','\''
              ,'%','s','1','=','\''
/* count=  1 Group:offset = IO:27 */
          ,24,'i','n','c','r','e','m','e','n','t'
              ,'a','t','i','o','n',' ','p','a','r','a'
              ,'m','e','t','e','r'
/* count=  1 Group:offset = DO: 7 */
          ,14,'i','n','i','t','i','a','l','i','z'
              ,'a','t','i','o','n'
/* count=  1 Group:offset = DA:13 */
          ,10,'i','n','i','t','i','a','l','i','z'
              ,'e'
/* count=  1 Group:offset = DA:12 */
          ,16,'i','n','i','t','i','a','l','i','z'
              ,'e','d',' ','o','n','c','e'
/* count=  1 Group:offset = DA:13 */
          ,8,'i','n','t','e','r','n','a','l'
/* count=  1 Group:offset = IO:11 */
          ,11,'i','n','t','e','r','r','u','p','t'
              ,'e','d'
/* count=  1 Group:offset = KO: 4 */
          ,4,'i','t','e','m'
/* count=  1 Group:offset = IO:10 */
          ,8,'k','e','y','b','o','a','r','d'
/* count=  1 Group:offset = KO: 4 */
          ,5,'l','a','r','g','e'
/* count=  1 Group:offset = IO:26 */
          ,8,'l','e','f','t',' ','a','n','d'
/* count=  1 Group:offset = CV: 2 */
          ,6,'l','e','n','g','t','h'
/* count=  1 Group:offset = LI: 8 */
          ,14,'l','i','b','r','a','r','y',' ','m'
              ,'e','m','b','e','r'
/* count=  1 Group:offset = MS:133 */
          ,4,'m','a','n','y'
/* count=  1 Group:offset = DA:10 */
          ,13,'m','a','x','i','m','u','m',' ','p'
              ,'a','g','e','s'
/* count=  1 Group:offset = KO: 6 */
          ,3,'m','a','y'
/* count=  1 Group:offset = IO:19 */
          ,6,'m','e','m','o','r','y'
/* count=  1 Group:offset = MO: 1 */
          ,29,'m','i','s','p','l','a','c','e','d'
              ,' ','o','p','e','n','i','n','g',' ','p'
              ,'a','r','e','n','t','h','e','s','i','s'
/* count=  1 Group:offset = PC: 1 */
          ,8,'m','o','d','i','f','i','e','r'
/* count=  1 Group:offset = FM: 8 */
          ,8,'n','e','g','a','t','i','v','e'
/* count=  1 Group:offset = LI: 2 */
          ,4,'o','p','e','n'
/* count=  1 Group:offset = MS:148 */
          ,7,'o','p','e','r','a','n','d'
/* count=  1 Group:offset = UV: 2 */
          ,6,'o','u','t','p','u','t'
/* count=  1 Group:offset = KO: 6 */
          ,7,'p','e','r','f','o','r','m'
/* count=  1 Group:offset = IO:15 */
          ,11,'p','r','e','s','e','n','t','\\','r'
              ,'\\','n'
/* count=  1 Group:offset = MS:136 */
          ,6,'r','a','n','g','e',';'
/* count=  1 Group:offset = SS: 2 */
          ,4,'r','e','a','l'
/* count=  1 Group:offset = EX: 2 */
          ,9,'r','e','f','e','r','e','n','c','e'
/* count=  1 Group:offset = AR:11 */
          ,10,'r','e','l','a','t','i','o','n','a'
              ,'l'
/* count=  1 Group:offset = UV: 2 */
          ,10,'r','e','p','e','a','t','a','b','l'
              ,'e'
/* count=  1 Group:offset = FM:18 */
          ,8,'r','e','q','u','i','r','e','d'
/* count=  1 Group:offset = IO:17 */
          ,6,'r','e','t','u','r','n'
/* count=  1 Group:offset = UV: 1 */
          ,4,'s','i','d','e'
/* count=  1 Group:offset = UV: 0 */
          ,5,'s','m','a','l','l'
/* count=  1 Group:offset = DM: 1 */
          ,16,'s','p','e','c','i','f','i','e','r'
              ,'/','A','C','C','E','S','S'
/* count=  1 Group:offset = IO:16 */
          ,14,'s','p','e','c','i','f','i','e','r'
              ,'/','F','I','L','E'
/* count=  1 Group:offset = IO:21 */
          ,11,'s','t','o','p',' ','(','y','/','n'
              ,')','?'
/* count=  1 Group:offset = MS:135 */
          ,13,'s','t','r','i','n','g',',',' ','c'
              ,'o','m','m','a'
/* count=  1 Group:offset = FM: 0 */
          ,12,'s','u','b','s','c','r','i','p','t'
              ,'i','n','g'
/* count=  1 Group:offset = IO:32 */
          ,15,'s','u','b','s','e','q','u','e','n'
              ,'t',' ','O','P','E','N'
/* count=  1 Group:offset = IO:19 */
          ,6,'s','y','m','b','o','l'
/* count=  1 Group:offset = IO:30 */
          ,6,'s','y','n','t','a','x'
/* count=  1 Group:offset = IO:31 */
          ,6,'s','y','s','t','e','m'
/* count=  1 Group:offset = IO: 8 */
          ,4,'t','i','m','e'
/* count=  1 Group:offset = KO: 8 */
          ,14,'t','o','g','g','l','e',' ','l','o'
              ,'g','g','i','n','g'
/* count=  1 Group:offset = MS:153 */
          ,17,'t','r','a','n','s','f','e','r',' '
              ,'p','a','s','t',' ','e','n','d'
/* count=  1 Group:offset = IO:15 */
          ,6,'u','n','a','b','l','e'
/* count=  1 Group:offset = MS:148 */
          ,11,'u','n','a','l','l','o','c','a','t'
              ,'e','d'
/* count=  1 Group:offset = MO: 3 */
          ,9,'u','n','d','e','r','f','l','o','w'
/* count=  1 Group:offset = KO: 2 */
          ,10,'u','n','d','e','r','s','t','o','o'
              ,'d'
/* count=  1 Group:offset = MS:144 */
          ,7,'u','n','i','t','(','s',')'
/* count=  1 Group:offset = MS:156 */
          ,21,'u','n','m','a','t','c','h','e','d'
              ,' ','p','a','r','e','n','t','h','e','s'
              ,'e','s'
/* count=  1 Group:offset = PC: 3 */
          ,14,'u','n','r','e','c','o','g','n','i'
              ,'z','a','b','l','e'
/* count=  1 Group:offset = FM: 5 */
          ,5,'u','s','i','n','g'
/* count=  1 Group:offset = MS:160 */
          ,5,'w','i','d','t','h'
/* count=  1 Group:offset = FM:14 */
          ,7,'w','r','i','t','i','n','g'
/* count=  1 Group:offset = MS:130 */
          ,15,'y','e','t',' ','e','s','t','a','b'
              ,'l','i','s','h','e','d'
/* count=  1 Group:offset = MS:143 */
          ,7,'z','e','r','o','*','*','J'
/* count=  1 Group:offset = EX: 0 */
                   };
/* Total number of phrases = 295 */
