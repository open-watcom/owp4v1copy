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
/* invalid number of arguments to intrinsic function %s1  */
 8,16,18,5,122,6,43,10,0
/* dummy argument %s1 appears more than once  */
,7,67,8,0,92,133,76,96
/* argument number %d1 (%s2) in call to %s3 must be CHARACTER*%u4 or bigger but size passed was %d5  */
,20,8,18,56,90,3,80,6,49,17,9,255,133,12,255,219,26,33,54,31,225
/* argument number %d1 (%s2) in call to %s3 - actual argument was %s4 but dummy argument was %s5  */
,18,8,18,56,90,3,80,6,49,23,250,8,31,226,26,67,8,31,227
/* argument number %d1 - %s2 was passed to an intrinsic function  */
,11,8,18,56,23,13,31,54,6,14,43,10
/* argument number %d1 (%s2) in call to %s3 - a subroutine was passed but a function was expected  */
,18,8,18,56,90,3,80,6,49,23,7,99,31,54,26,7,10,31,105
/* number of arguments expected in call to %s1 was %d2, but found %d3  */
,14,18,5,122,105,3,80,6,0,31,255,94,26,73,224
/* argument number %d1 (%s2) in call to %s3 - a function was passed but a subroutine was expected  */
,18,8,18,56,90,3,80,6,49,23,7,10,31,54,26,7,99,31,105
/* argument number %d1 (%s2) in call to %s3 - %s4 argument was passed but %s5 argument was expected  */
,18,8,18,56,90,3,80,6,49,23,226,8,31,54,26,227,8,31,105
/* argument number %d1 (%s2) in call to %s3 - type of structure passed does not match type of structure exp  */
,22,8,18,56,90,3,80,6,49,23,24,5,29,54,72,1,95,24,5,29,255,255,3
,0
/* argument number %d1 (%s2) in call to %s3 - argument passed by value but expected by reference  */
,19,8,18,56,90,3,80,6,49,23,8,54,123,115,26,105,123,255,255,80
/* dummy argument %s1 must not appear before definition of ENTRY %s2  */
,11,67,8,0,17,1,102,147,66,5,238,13
};
static const char __FAR Msg256[] = {
/* %s1 was initialized in a block data subprogram but is not in COMMON  */
 13,0,31,152,3,7,37,41,55,26,4,1,3,44
/* %s1 statement is not permitted in a BLOCK DATA subprogram  */
,12,0,2,4,1,255,255,65,3,7,91,118,55
};
static const char __FAR Msg512[] = {
/* invalid character encountered in source input  */
 7,16,27,255,22,3,216,107
/* invalid character in statement number columns  */
,7,16,27,3,2,18,255,6
/* character in continuation column, but no statement to continue  */
,11,27,3,124,255,228,26,87,2,6,255,233
/* character encountered is not FORTRAN 77 standard  */
,6,27,255,22,4,1,11
/* columns 1-5 in a continuation line must be blank  */
,11,255,6,255,110,3,7,124,39,17,9,180
/* more than 19 continuation lines is not FORTRAN 77 standard  */
,9,133,76,255,111,124,208,4,1,11
/* end-of-line comment is not FORTRAN 77 standard  */
,7,255,250,255,7,4,1,11
/* D in column 1 is not FORTRAN 77 standard  */
,8,255,140,3,186,170,4,1,11
/* too many continuation lines  */
,5,113,255,52,124,208
};
static const char __FAR Msg768[] = {
/* %s1 already in COMMON  */
 4,0,60,3,44
/* initializing %s1 in COMMON outside of block data subprogram is not FORTRAN 77 standard  */
,15,255,255,31,0,3,44,255,64,5,37,41,55,4,1,11
/* character and non-character data in COMMON is not FORTRAN 77 standard  */
,10,27,61,255,57,41,3,44,4,1,11
/* COMMON block %s1 has been defined with a different size  */
,10,44,37,0,38,46,50,32,7,192,33
/* named COMMON block %s1 appears in more than one BLOCK DATA subprogram  */
,14,255,255,52,44,37,0,92,3,133,76,111,91,118,55
/* blank COMMON block has been defined with a different size  */
,10,180,44,37,38,46,50,32,7,192,33
};
static const char __FAR Msg1024[] = {
/* DOUBLE PRECISION COMPLEX constants are not FORTRAN 77 standard  */
 6,140,235,149,121,1,11
/* invalid floating-point constant %s1  */
,4,16,129,47,0
/* zero length character constants are not allowed  */
,7,89,53,27,149,121,1,19
/* invalid hexadecimal/octal constant  */
,4,16,255,39,47
/* hexadecimal/octal constant is not FORTRAN 77 standard  */
,6,255,39,47,4,1,11
};
static const char __FAR Msg1280[] = {
/* %s1 is already being included  */
 5,0,4,60,255,217
/* '%s1' option cannot take a NO prefix  */
,9,100,68,21,255,255,111,7,255,176
/* expecting an equals sign following the %s1 option  */
,11,15,14,255,27,163,255,255,12,30,0,68
/* the '%s1' option requires a number  */
,6,30,100,68,69,7,18
/* option '%s1' not recognized - ignored  */
,8,68,100,1,255,255,77,23,200
/* '%s1' option not allowed in source input stream  */
,10,100,68,1,19,3,216,107,255,255,101
/* nesting level exceeded for compiler directives  */
,10,255,56,255,255,43,84,35,189,255,18
/* mismatching compiler directives  */
,6,255,255,48,189,255,18
/* DATA option not allowed  */
,4,118,68,1,19
/* maximum limit exceeded in the '%s1' option - option ignored  */
,11,255,53,86,84,3,30,100,68,23,68,200
/* DATA option not allowed with OBJECT option  */
,8,118,68,1,19,32,255,178,68
};
static const char __FAR Msg1536[] = {
/* program abnormally terminated  */
 3,97,255,199
,0
,0
,0
,0
,0
,0
,0
/* dynamic memory not freed  */
,6,194,132,1,255,255,18
/* freeing unowned dynamic memory  */
,5,255,255,19,194,132
};
static const char __FAR Msg1792[] = {
/* CHARACTER variable %s1 with length (*) not allowed in this expression  */
 11,172,25,0,32,53,116,1,19,3,64,51
/* character variable %s1 with length (*) must be a subprogram argument  */
,11,27,25,0,32,53,116,17,9,7,55,8
/* left and right hand sides overlap in a character assignment statement  */
,12,255,48,61,215,255,255,24,3,7,27,145,2
};
static const char __FAR Msg2048[] = {
/* implied DO variable %s1 must be an integer variable  */
 9,201,59,25,0,17,9,14,63,25
/* repeat specification must be a positive integer  */
,9,212,48,17,9,7,255,255,67,63
/* %s1 appears in an expression but is not an implied DO variable  */
,12,0,92,3,14,51,26,4,1,14,201,59,25
/* %s1 in blank COMMON block cannot be initialized  */
,8,0,3,180,44,37,21,9,152
/* data initialization with hexadecimal constant is not FORTRAN 77 standard  */
,10,41,203,32,255,255,26,47,4,1,11
/* cannot initialize %s1 %s2  */
,5,21,255,46,0,13
/* data initialization in %s1 statement is not FORTRAN 77 standard  */
,8,41,203,3,0,2,4,1,11
,0
/* not enough constants for list of variables  */
,8,1,255,23,149,35,40,5,168
,0
/* too many constants for list of variables  */
,8,113,255,52,149,35,40,5,168
,0
/* cannot initialize %s1 variable with %s2 constant  */
,8,21,255,46,0,25,32,13,47
/* entity can only be initialized once during data initialization  */
,10,255,251,103,74,9,152,96,126,41,203
};
static const char __FAR Msg2304[] = {
/* using %s1 incorrectly in dimension expression  */
 7,167,0,202,3,255,17,51
/* array or array element (possibly substrung) associated with %s1 too small  */
,11,34,12,34,255,249,32,0,113,255,255,92
};
static const char __FAR Msg2560[] = {
/* statement number %i1 already defined in line %d2 - DO loop is backwards  */
 14,2,18,58,60,50,3,39,57,23,59,109,4,255,216
/* %s1 statement not allowed at termination of DO range  */
,11,0,2,1,19,79,255,255,114,5,59,98
/* improper nesting of DO loop  */
,7,255,44,255,56,5,59,109
/* ENDDO cannot terminate DO loop with statement label  */
,11,255,149,21,255,255,113,59,109,32,2,131
/* this DO loop form is not FORTRAN 77 standard  */
,9,64,59,109,255,255,13,4,1,11
/* expecting comma or DO variable  */
,5,15,188,12,59,25
/* DO variable cannot be redefined while DO loop is active  */
,13,59,25,21,9,255,255,79,255,87,59,109,4,177
/* incrementation parameter for DO-loop cannot be zero  */
,9,255,255,30,35,255,141,21,9,89
};
static const char __FAR Msg2816[] = {
/* equivalencing %s1 has caused extension of COMMON block %s2 to the left  */
 16,255,28,0,38,255,226,255,34,5,44,37,13,6,30,255,48
/* %s1 and %s2 in COMMON are equivalenced to each other  */
,12,0,61,13,3,44,121,195,6,255,248,255,62
};
static const char __FAR Msg3072[] = {
/* missing END statement  */
 3,36,237,2
};
static const char __FAR Msg3328[] = {
/* target of assignment is illegal  */
 7,255,255,112,5,145,4,52
/* cannot assign value to %s1  */
,6,21,255,212,115,6,0
/* illegal use of equal sign  */
,6,52,136,5,255,26,163
/* multiple assignment is not FORTRAN 77 standard  */
,6,255,55,145,4,1,11
/* expecting equals sign  */
,4,15,255,27,163
};
static const char __FAR Msg3584[] = {
/* %s1 has been equivalenced to 2 different relative positions  */
 10,0,38,46,195,6,171,192,255,255,81
/* EQUIVALENCE list must contain at least 2 names  */
,10,239,40,17,190,79,207,171,255,255,53
/* %s1 incorrectly subscripted in %s2 statement  */
,8,0,202,255,255,105,3,13,2
/* incorrect substring of %s1 in %s2 statement  */
,8,255,45,70,5,0,3,13,2
/* equivalencing CHARACTER and non-CHARACTER data is not FORTRAN 77 standard  */
,11,255,28,172,61,255,255,56,41,4,1,11
/* attempt to substring %s1 in EQUIVALENCE statement but type is %s2  */
,11,65,6,70,0,3,239,2,26,24,4,13
};
static const char __FAR Msg3840[] = {
/* zero**J where J <= 0 is not allowed  */
 9,255,255,139,223,255,170,4,1,19
/* X**Y where X < 0.0, Y is not of type INTEGER, is not allowed  */
,15,255,196,223,248,255,119,249,4,1,5,24,173,4,1,19
/* (0,0)**Y where Y is not real is not allowed  */
,12,255,98,223,249,4,1,255,255,76,4,1,19
};
static const char __FAR Msg4096[] = {
/* type of entry %s1 does not match type of function %s2  */
 12,24,5,255,25,0,72,1,95,24,5,10,13
/* ENTRY statement not allowed within structured control blocks  */
,11,238,2,1,19,169,255,255,103,82,255,2
/* size of entry %s1 does not match size of function %s2  */
,12,33,5,255,25,0,72,1,95,33,5,10,13
};
static const char __FAR Msg4352[] = {
/* missing delimiter in format string, comma assumed  */
 10,36,255,238,3,42,255,255,102,188,146
/* missing or invalid constant  */
,4,36,12,16,47
/* Ew.dDe format code is not FORTRAN 77 standard  */
,7,255,153,42,81,4,1,11
/* missing decimal point  */
,3,36,255,235
/* missing or invalid edit descriptor in format string  */
,8,36,12,16,104,125,3,42,217
/* unrecognizable edit descriptor in format string  */
,6,222,104,125,3,42,217
/* invalid repeat specification  */
,3,16,212,48
/* $ or \ format code is not FORTRAN 77 standard  */
,10,255,92,12,255,198,42,81,4,1,11
/* invalid field modifier  */
,5,16,93,255,255,51
/* expecting end of FORMAT statement but found more text  */
,11,15,83,5,78,2,26,73,133,255,255,115
/* repeat specification not allowed for this format code  */
,8,212,48,1,19,35,64,42,81
/* no statement number on FORMAT statement  */
,6,87,2,18,110,78,2
/* no closing quote on apostrophe edit descriptor  */
,9,87,185,255,71,110,255,207,104,125
/* field count greater than 256 is invalid  */
,9,93,255,14,199,76,255,114,4,16
/* invalid field width specification  */
,6,16,93,255,255,135,48
/* Z format code is not FORTRAN 77 standard  */
,7,255,197,42,81,4,1,11
/* FORMAT statement exceeds allotted storage size  */
,6,78,2,196,255,204,33
/* format specification not allowed on input  */
,6,42,48,1,19,110,107
/* FORMAT missing repeatable edit descriptor  */
,7,78,36,255,255,82,104,125
/* missing constant before X edit descriptor, 1 assumed  */
,9,36,47,147,248,104,255,239,170,146
/* Ew.dQe format code is not FORTRAN 77 standard  */
,7,255,154,42,81,4,1,11
/* Qw.d format code is not FORTRAN 77 standard  */
,7,255,182,42,81,4,1,11
};
static const char __FAR Msg4608[] = {
/* %s1 statement label may not appear in ASSIGN statement but did in line %d2  */
 15,0,2,131,156,1,102,3,232,2,26,255,241,3,39,57
/* ASSIGN of statement number %i1 in line %d2 not allowed  */
,10,232,5,2,18,58,3,39,57,1,19
/* expecting TO  */
,3,15,255,188
};
static const char __FAR Msg4864[] = {
/* hollerith constant is not FORTRAN 77 standard  */
 6,255,40,47,4,1,11
/* not enough characters for hollerith constant  */
,8,1,255,23,184,35,255,40,47
};
static const char __FAR Msg5120[] = {
/* ELSE block must be the last block in block IF  */
 14,255,147,37,17,9,30,255,255,40,37,3,37,255,165
/* expecting THEN  */
,3,15,255,187
};
static const char __FAR Msg5376[] = {
/* missing or invalid format/FMT specification  */
 7,36,12,16,255,255,14,48
/* the UNIT may not be an internal file for this statement  */
,11,30,176,156,1,9,14,153,22,35,64,2
/* %s1 statement cannot have %s2 specification  */
,6,0,2,21,150,13,48
/* variable must have a size of 4  */
,8,25,17,150,7,33,5,255,115
/* missing or unrecognizable control list item %s1  */
,7,36,12,222,82,40,154,0
/* attempt to specify control list item %s1 more than once  */
,12,65,6,255,255,96,82,40,154,0,133,76,96
/* implied DO loop has no input/output list  */
,7,201,59,109,38,87,85,40
/* list-directed input/output with internal files is not FORTRAN 77 standard  */
,9,255,50,85,32,153,128,4,1,11
/* FORTRAN 77 standard requires an asterisk for list-directed formatting  */
,10,11,69,14,179,35,255,50,255,255,16
/* missing or improper unit identification  */
,7,36,12,255,44,77,255,41
/* missing unit identification or file specification  */
,7,36,77,255,41,12,22,48
/* asterisk unit identifier not allowed in %s1 statement  */
,9,179,77,255,42,1,19,3,0,2
/* cannot have both UNIT and FILE specifier  */
,8,21,150,181,176,61,255,158,20
/* internal files require sequential access  */
,7,153,128,255,255,83,135,101
/* END specifier with REC specifier is not FORTRAN 77 standard  */
,8,237,20,32,144,20,4,1,11
/* %s1 specifier in i/o list is not FORTRAN 77 standard  */
,8,0,20,3,106,40,4,1,11
/* i/o list is not allowed with NAMELIST-directed format  */
,8,106,40,4,1,19,32,142,42
/* non-character array as format specifier is not FORTRAN 77 standard  */
,9,255,57,34,45,42,20,4,1,11
};
static const char __FAR Msg5632[] = {
/* illegal range of characters  */
 4,52,98,5,184
/* letter can only be implicitly declared once  */
,11,255,255,42,103,74,9,255,255,28,191,96
/* unrecognizable type  */
,2,222,24
/* (*) length specifier in an IMPLICIT statement is not FORTRAN 77 standard  */
,10,116,53,20,3,14,141,2,4,1,11
/* IMPLICIT NONE allowed once or not allowed with other IMPLICIT statements  */
,13,141,255,177,19,96,12,1,19,32,255,62,141,112
};
static const char __FAR Msg5888[] = {
/* BACKSPACE statement requires sequential access mode  */
 7,255,124,2,69,135,101,210
/* input/output is already active  */
,4,85,4,60,177
/* ENDFILE statement requires sequential access mode  */
,7,255,150,2,69,135,101,210
/* formatted connection requires formatted input/output statements  */
,7,198,255,10,69,198,85,112
/* unformatted connection requires unformatted input/output statements  */
,7,221,255,10,69,221,85,112
/* REWIND statement requires sequential access  */
,6,255,184,2,69,135,101
/* bad character in input field  */
,6,255,1,27,3,107,93
/* BLANK specifier requires FORM specifier to be 'FORMATTED'  */
,11,255,125,20,69,255,159,20,6,9,255,96
/* system file error - %s1  */
,5,219,22,62,23,0
/* format specification does not match data type  */
,7,42,48,72,1,95,41,24
/* input item does not match the data type of list variable  */
,11,107,154,72,1,95,30,41,24,5,40,25
/* internal file is full  */
,6,153,22,4,255,255,20
/* RECL specifier is invalid  */
,4,246,20,4,16
/* invalid STATUS specifier in CLOSE statement  */
,7,16,175,20,3,255,134,2
/* unit specified is not connected  */
,6,77,164,4,1,255,9
/* attempt to perform data transfer past end of file  */
,12,65,6,255,255,64,41,255,255,118,83,5,22
/* invalid RECL specifier/ACCESS specifier combination  */
,7,16,246,255,255,94,20,187
/* REC specifier required in direct access input/output statements  */
,10,144,20,255,75,3,255,243,101,85,112
/* REC specifier not allowed in sequential access input/output statements  */
,9,144,20,1,19,3,135,101,85,112
/* %s1 specifier may not change in a subsequent OPEN statement  */
,12,0,20,156,1,255,227,3,7,255,255,108,2
/* invalid STATUS specifier for given file  */
,7,16,175,20,35,255,37,22
/* invalid STATUS specifier/FILE specifier combination  */
,7,16,175,255,255,95,20,187
/* record size exceeded during unformatted input/output  */
,6,161,33,84,126,221,85
/* unit specified does not exist  */
,6,77,164,72,1,255,32
/* REC specifier is invalid  */
,4,144,20,4,16
/* UNIT specifier is invalid  */
,4,176,20,4,16
/* formatted record or format edit descriptor is too large for record size  */
,14,198,161,12,42,104,125,4,113,255,255,39,35,161,33
/* illegal '%s1=' specifier  */
,4,52,255,95,20
/* invalid CARRIAGECONTROL specifier/FORM specifier combination  */
,5,16,255,129,20,187
/* i/o operation not consistent with file attributes  */
,10,106,255,61,1,255,231,32,22,255,215
/* symbol %s1 not found in NAMELIST  */
,7,255,80,0,1,73,3,174
/* syntax error during NAMELIST-directed input  */
,7,255,255,110,62,126,142,107
/* subscripting error during NAMELIST-directed i/o  */
,7,255,255,106,62,126,142,106
/* substring error during NAMELIST-directed i/o  */
,5,70,62,126,142,106
/* BLOCKSIZE specifier is invalid  */
,5,255,126,20,4,16
/* invalid operation for files with no record structure  */
,9,16,255,61,35,128,32,87,161,29
/* integer overflow converting character data to integer  */
,8,63,159,255,12,27,41,6,63
/* range exceeded converting character data to floating-point  */
,8,98,84,255,12,27,41,6,129
};
static const char __FAR Msg6144[] = {
/* floating-point divide by zero  */
 5,129,255,19,123,89
/* floating-point overflow  */
,2,129,159
/* floating-point underflow  */
,4,129,255,255,124
/* integer divide by zero  */
,5,63,255,19,123,89
/* program interrupted from keyboard  */
,8,97,255,255,34,94,255,255,37
/* integer overflow  */
,2,63,159
/* maximum pages of output exceeded  */
,9,255,53,255,255,62,5,255,63,84
/* statement count has been exceeded  */
,6,2,255,14,38,46,84
/* time limit exceeded  */
,3,166,86,84
};
static const char __FAR Msg6400[] = {
/* argument must be greater than zero  */
 6,8,17,9,199,76,89
/* absolute value of argument to arcsine, arccosine must not exceed one  */
,14,255,200,115,5,8,6,255,209,17,1,255,255,0,111
/* argument must not be negative  */
,7,8,17,1,9,255,255,55
/* argument(s) must not be zero  */
,5,253,17,1,9,89
/* argument of CHAR must be in the range zero to 255  */
,13,8,5,255,130,17,9,3,30,98,89,6,255,113
/* %s1 intrinsic function cannot be passed 2 complex arguments  */
,10,0,43,10,21,9,54,171,255,8,122
/* argument types must be the same for the %s1 intrinsic function  */
,13,8,255,82,17,9,30,255,76,35,30,0,43,10
/* expecting numeric argument, but %s1 argument was found  */
,10,15,255,255,57,254,26,0,8,31,73
/* length of ICHAR argument greater than one  */
,8,53,5,255,164,8,199,76,111
/* cannot pass %s1 as argument to intrinsic function  */
,9,21,255,67,0,45,8,6,43,10
/* intrinsic function requires argument(s)  */
,4,43,10,69,253
/* %s1 argument type is invalid for this generic function  */
,11,0,8,24,4,16,35,64,255,255,23,10
/* this intrinsic function cannot be passed as an argument  */
,9,64,43,10,21,9,54,45,14,8
/* expecting %s1 argument, but %s2 argument was found  */
,8,15,0,254,26,13,8,31,73
/* intrinsic function was assigned wrong type  */
,6,43,10,31,255,213,24
/* intrinsic function %s1 is not FORTRAN 77 standard  */
,6,43,10,0,4,1,11
/* argument to ALLOCATED intrinsic function must be an allocatable array or character*(*) variable  */
,14,8,6,255,123,43,10,17,9,14,251,34,12,183,25
/* invalid argument to ISIZEOF intrinsic function  */
,7,16,8,6,255,168,43,10
};
static const char __FAR Msg6656[] = {
/* relational operator has a logical operand  */
 8,255,73,75,38,7,209,255,60
/* mixing DOUBLE PRECISION and COMPLEX types is not FORTRAN 77 standard  */
,11,255,255,50,140,61,235,255,82,4,1,11
/* operator not expecting %s1 operands  */
,5,75,1,15,0,211
/* operator not expecting %s1 and %s2 operands  */
,7,75,1,15,0,61,13,211
/* complex quantities can only be compared using .EQ. or .NE.  */
,16,255,8,255,255,73,103,74,9,255,229,167,255,107,12,255,108
/* unary operator not expecting %s1 type  */
,8,255,255,122,75,1,15,0,24
/* logical operator with integer operands is not FORTRAN 77 standard  */
,8,209,75,32,63,211,4,1,11
/* logical operator %s1 is not FORTRAN 77 standard  */
,6,209,75,0,4,1,11
};
static const char __FAR Msg6912[] = {
/* %s1 exceeds compiler limit of %u2 bytes  */
 8,0,196,189,86,5,138,255,3
/* out of memory  */
,3,158,5,132
/* dynamic memory exhausted due to length of this statement - statement ignored  */
,13,194,132,255,31,193,6,53,5,64,2,23,2,200
/* attempt to deallocate an unallocated array or character*(*) variable  */
,12,65,6,255,234,14,255,255,121,34,12,183,25
/* attempt to allocate an already allocated array or character*(*) variable  */
,11,65,6,255,203,14,60,252,34,12,183,25
/* object memory exhausted  */
,5,255,58,132,255,31
};
static const char __FAR Msg7168[] = {
/* missing or misplaced closing parenthesis  */
 6,36,12,255,54,185,160
/* missing or misplaced opening parenthesis  */
,7,36,12,255,54,255,59,160
/* unexpected parenthesis  */
,2,220,160
/* unmatched parentheses  */
,3,255,255,129
};
static const char __FAR Msg7424[] = {
/* expecting symbolic name  */
 3,15,165,28
/* illegal size specified for VALUE attribute  */
,8,52,33,164,35,255,194,255,0
/* illegal argument attribute  */
,4,52,8,255,0
/* continuation line must contain a comment character in column 1  */
,11,124,39,17,190,7,255,7,27,3,186,170
/* expecting '%s1' near '%s2'  */
,5,15,100,255,255,54
};
static const char __FAR Msg7680[] = {
/* alternate return specifier only allowed in subroutine  */
 8,255,205,214,20,74,19,3,99
/* RETURN statement in main program is not FORTRAN 77 standard  */
,8,247,2,3,155,97,4,1,11
};
static const char __FAR Msg7936[] = {
/* COMMON block %s1 saved but not properly defined  */
 11,44,37,0,255,77,26,1,255,255,72,50
/* COMMON block %s1 must be saved in every subprogram in which it appears  */
,16,44,37,0,17,9,255,77,3,255,254,55,3,255,255,134,92
/* name already appeared in a previous SAVE statement  */
,10,28,60,255,208,3,7,255,255,70,2
};
static const char __FAR Msg8192[] = {
/* statement function definition contains duplicate dummy arguments  */
 9,2,10,66,255,11,255,246,67,122
/* character length of statement function name must not be (*)  */
,10,27,53,5,2,10,28,17,1,9,116
/* statement function definition contains illegal dummy argument  */
,8,2,10,66,255,11,52,67,8
/* cannot pass %s1 %s2 to statement function  */
,8,21,255,67,0,13,6,2,10
/* %s1 actual argument was passed to %s2 dummy argument  */
,9,0,250,8,31,54,6,13,67,8
/* incorrect number of arguments passed to statement function %s1  */
,10,255,45,18,5,122,54,6,2,10,0
/* type of statement function name must not be a user-defined structure  */
,13,24,5,2,10,28,17,1,9,7,255,255,131,29
};
static const char __FAR Msg8448[] = {
/* system file error reading %s1 - %s2  */
 9,219,22,62,255,255,75,0,23,13
/* error opening file %s1 - %s2  */
,7,62,255,59,22,0,23,13
/* system file error writing %s1 - %s2  */
,8,219,22,62,255,89,0,23,13
/* error spawning %s1 - %s2  */
,7,62,255,255,93,0,23,13
/* error while linking  */
,6,62,255,87,255,255,44
,0
/* generation of browsing information failed  */
,10,255,255,22,5,255,222,151,255,255,8
};
static const char __FAR Msg8704[] = {
/* cannot have both ATEND and the END= specifier  */
 9,21,150,181,233,61,30,255,148,20
/* ATEND must immediately follow a READ statement  */
,10,233,17,255,43,255,36,7,255,183,2
/* block label must be a symbolic name  */
,7,37,131,17,9,7,165,28
/* could not find a structure to %s1 from  */
,11,255,13,1,255,255,10,7,29,6,0,94
/* REMOTE BLOCK is not allowed in the range of any control structure  */
,13,120,91,4,1,19,3,30,98,5,255,206,82,29
/* the SELECT statement must be followed immediately by a CASE statement  */
,15,30,255,186,2,17,9,255,255,11,255,43,123,7,234,2
/* cases are overlapping  */
,6,255,225,121,255,255,61
/* select structure requires at least one CASE statement  */
,10,255,255,87,29,69,79,207,111,234,2
/* cannot branch to %i1 from outside control structure in line %d2  */
,12,21,71,6,58,94,255,64,82,29,3,39,57
/* cannot branch to %i1 inside structure on line %d2  */
,11,21,71,6,58,255,255,32,29,110,39,57
/* low end of range exceeds the high end  */
,12,255,255,47,83,5,98,196,30,255,255,27,83
/* default case block must follow all case blocks  */
,14,255,236,255,4,37,17,255,36,255,202,255,4,255,2
/* attempt to branch out of a REMOTE BLOCK  */
,8,65,6,71,158,5,7,120,91
/* attempt to EXECUTE undefined REMOTE BLOCK %s1  */
,8,65,6,255,151,88,120,91,0
/* attempted to use REMOTE BLOCK recursively  */
,9,255,214,6,136,120,91,255,255,78
/* cannot RETURN from subprogram within a REMOTE BLOCK  */
,8,21,247,94,55,169,7,120,91
/* %s1 statement is not FORTRAN 77 standard  */
,5,0,2,4,1,11
/* %s1 block is unfinished  */
,6,0,37,4,255,255,127
/* %s1 statement does not match with %s2 statement  */
,8,0,2,72,1,95,32,13,2
/* incomplete control structure found at %s1 statement  */
,9,255,255,29,82,29,73,79,0,2
/* %s1 statement is not allowed in %s2 definition  */
,8,0,2,4,1,19,3,13,66
/* no such field name found in structure %s1  */
,10,87,255,255,109,93,28,73,3,29,0
/* multiple definition of field name %s1  */
,7,255,55,66,5,93,28,0
/* structure %s1 has not been defined  */
,6,29,0,38,1,46,50
/* structure %s1 has already been defined  */
,6,29,0,38,60,46,50
/* structure %s1 must contain at least one field  */
,8,29,0,17,190,79,207,111,93
/* recursion detected in definition of structure %s1  */
,7,255,72,3,66,5,29,0
/* illegal use of structure %s1 containing union  */
,7,52,136,5,29,0,255,232
/* allocatable arrays cannot be fields within structures  */
,12,251,255,211,21,9,255,255,9,169,255,255,104
/* an integer conditional expression is not FORTRAN 77 standard  */
,8,14,63,255,230,51,4,1,11
/* %s1 statement must be used within %s2 definition  */
,8,0,2,17,9,114,169,13,66
};
static const char __FAR Msg8960[] = {
/* name can only appear in an EXTERNAL statement once  */
 9,28,103,74,102,3,14,240,2,96
/* character function %s1 may not be called since size was declared as (*)  */
,13,27,10,0,156,1,9,255,223,33,31,191,45,116
/* %s1 can only be used an an argument to a subroutine  */
,11,0,103,74,9,114,14,14,8,6,7,99
/* name cannot appear in both an INTRINSIC and EXTERNAL statement  */
,10,28,21,102,3,181,14,243,61,240,2
/* expecting a subroutine name  */
,4,15,7,99,28
/* %s1 statement not allowed in main program  */
,7,0,2,1,19,3,155,97
/* not an intrinsic FUNCTION name  */
,6,1,14,43,255,160,28
/* name can only appear in an INTRINSIC statement once  */
,9,28,103,74,102,3,14,243,2,96
/* subprogram recursion detected  */
,3,55,255,72
/* two main program units in the same file  */
,12,255,255,120,155,97,255,84,3,30,255,76,22
/* only one unnamed %s1 is allowed in an executable program  */
,12,74,111,255,255,130,0,4,19,3,14,127,97
/* function referenced as a subroutine  */
,5,10,134,45,7,99
/* attempt to invoke active function/subroutine  */
,7,65,6,204,177,255,255,21
/* dummy argument %s1 is not in dummy argument list of entered subprogram  */
,13,67,8,0,4,1,3,67,8,40,5,255,24,55
/* function referenced as %s1 but defined to be %s2  */
,9,10,134,45,0,26,50,6,9,13
/* function referenced as CHARACTER*%u1 but defined to be CHARACTER*%u2  */
,11,10,134,45,255,131,26,50,6,9,255,132
/* missing main program  */
,3,36,155,97
/* subroutine referenced as a function  */
,5,99,134,45,7,10
/* attempt to invoke a block data subprogram  */
,7,65,6,204,7,37,41,55
/* structure type of function %s1 does not match expected structure type  */
,11,29,24,5,10,0,72,1,95,105,29,24
};
static const char __FAR Msg9216[] = {
/* substringing of function or statement function return value is not FORTRAN 77 standard  */
 12,255,79,5,10,12,2,10,214,115,4,1,11
/* substringing valid only for character variables and array elements  */
,13,255,79,255,255,132,74,35,27,168,61,34,255,21
/* subscript expression out of range; %s1 does not exist  */
,12,218,51,158,5,255,255,74,0,72,1,255,32
/* substring expression (%i1:%i2) is out of range  */
,8,70,51,255,97,4,158,5,98
};
static const char __FAR Msg9472[] = {
/* statement number %i1 has already been defined in line %d2  */
 10,2,18,58,38,60,46,50,3,39,57
/* statement function definition appears after first executable statement  */
,9,2,10,66,92,178,255,35,127,2
/* %s1 statement must not be branched to but was in line %d2  */
,13,0,2,17,1,9,255,220,6,26,31,3,39,57
/* branch to statement %i1 in line %d2 not allowed  */
,9,71,6,2,58,3,39,57,1,19
/* specification statement must appear before %s1 is initialized  */
,8,48,2,17,102,147,0,4,152
/* statement %i1 was referenced as a FORMAT statement in line %d2  */
,11,2,58,31,134,45,7,78,2,3,39,57
/* IMPLICIT statement appears too late  */
,6,141,2,92,113,255,47
/* this statement will never be executed due to the preceding branch  */
,13,64,2,255,88,9,255,30,193,6,30,255,69,71
/* expecting statement number  */
,3,15,2,18
/* statement number %i1 was not used as a FORMAT statement in line %d2  */
,13,2,18,58,31,1,114,45,7,78,2,3,39,57
/* specification statement appears too late  */
,6,48,2,92,113,255,47
/* %s1 statement not allowed after %s2 statement  */
,7,0,2,1,19,178,13,2
/* statement number must be 99999 or less  */
,10,2,18,17,9,255,118,12,255,255,41
/* statement number cannot be zero  */
,5,2,18,21,9,89
/* this statement could branch to itself  */
,9,64,2,255,13,71,6,255,255,36
/* missing statement number %i1 - used in line %d2  */
,9,36,2,18,58,23,114,3,39,57
/* undecodeable statement or misspelled word %s1  */
,9,255,255,123,2,12,255,255,49,0
/* statement %i1 will never be executed due to the preceding branch  */
,13,2,58,255,88,9,255,30,193,6,30,255,69,71
/* expecting keyword or symbolic name  */
,7,15,255,255,38,12,165,28
/* number in %s1 statement is longer than 5 digits  */
,10,18,3,0,2,4,255,51,76,255,116
/* position of DATA statement is not FORTRAN 77 standard  */
,9,255,255,66,5,118,2,4,1,11
/* no FORMAT statement with given label  */
,7,87,78,2,32,255,37,131
/* statement number not in list or not the label of an executable statement  */
,13,2,18,1,3,40,12,1,30,131,5,14,127,2
/* attempt to branch into a control structure  */
,9,65,6,71,255,255,35,7,82,29
};
static const char __FAR Msg9728[] = {
/* variable %s1 in array declarator must be in COMMON or a dummy argument  */
 14,25,0,3,34,255,16,17,9,3,44,12,7,67,8
/* adjustable/assumed size array %s1 must be a dummy argument  */
,10,255,201,33,34,0,17,9,7,67,8
/* invalid subscript expression  */
,3,16,218,51
/* invalid number of subscripts  */
,6,16,18,5,255,255,107
/* using %s1 name incorrectly without list  */
,8,167,0,28,202,255,255,137,40
/* cannot substring array name %s1  */
,5,21,70,34,28,0
/* %s1 treated as an assumed size array  */
,9,0,255,255,119,45,14,146,33,34
/* assumed size array %s1 cannot be used as an i/o list item or a format/unit identifier  */
,19,146,33,34,0,21,9,114,45,14,106,40,154,12,7,255,255,15,255,42
/* limit of 65535 elements per dimension has been exceeded  */
,13,86,5,255,117,255,21,255,68,255,17,38,46,84
};
static const char __FAR Msg9984[] = {
/* unexpected number or name %s1  */
 5,220,18,12,28,0
/* bad sequence of operators  */
,9,255,1,255,255,88,5,255,255,60
/* invalid operator  */
,2,16,75
/* expecting end of statement after right parenthesis  */
,7,15,83,5,2,178,215,160
/* expecting an asterisk  */
,3,15,14,179
/* expecting colon  */
,3,15,255,5
/* expecting colon or end of statement  */
,7,15,255,5,12,83,5,2
/* missing comma  */
,2,36,188
/* expecting end of statement  */
,4,15,83,5,2
/* expecting integer variable  */
,3,15,63,25
/* expecting %s1 name  */
,3,15,0,28
/* expecting an integer  */
,3,15,14,63
/* expecting INTEGER, REAL, or DOUBLE PRECISION variable  */
,6,15,173,245,12,140,25
/* missing operator  */
,2,36,75
/* expecting a slash  */
,5,15,7,255,255,91
/* expecting %s1 expression  */
,3,15,0,51
/* expecting a constant expression  */
,4,15,7,47,51
/* expecting INTEGER, REAL, or DOUBLE PRECISION expression  */
,6,15,173,245,12,140,51
/* expecting INTEGER or CHARACTER constant  */
,6,15,255,166,12,172,47
/* unexpected operator  */
,2,220,75
/* no closing quote on literal string  */
,9,87,185,255,71,110,255,255,45,217
/* missing or invalid constant  */
,4,36,12,16,47
/* expecting character constant  */
,3,15,27,47
};
static const char __FAR Msg10240[] = {
/* length specification before array declarator is not FORTRAN 77 standard  */
 9,53,48,147,34,255,16,4,1,11
/* %i1 is an illegal length for %s2 type  */
,8,58,4,14,52,53,35,13,24
/* length specifier in %s1 statement is not FORTRAN 77 standard  */
,8,53,20,3,0,2,4,1,11
/* length specification not allowed with type %s1  */
,7,53,48,1,19,32,24,0
/* type of %s1 has already been established as %s2  */
,10,24,5,0,38,60,46,255,29,45,13
/* type of %s1 has not been declared  */
,7,24,5,0,38,1,46,191
/* %s1 of type %s2 is illegal in %s3 statement  */
,9,0,5,24,13,4,52,3,49,2
};
static const char __FAR Msg10496[] = {
/* substring (%u1:%u2) of expression on right side of equal sign is undefined  */
 15,70,139,5,51,110,215,255,255,89,5,255,26,163,4,88
/* substring (%u1:%u2) of return value of character function %s3 is undefined  */
,11,70,139,5,214,115,5,27,10,49,4,88
/* substring (%u1:%u2) of an operand in a relational expression is undefined  */
,13,70,139,5,14,255,60,3,7,255,73,51,4,88
/* substring (%u1:%u2) of an argument to an intrinsic function is undefined  */
,11,70,139,5,14,8,6,14,43,10,4,88
/* the value of %s1 is undefined  */
,6,30,115,5,0,4,88
/* argument number %d1 passed to intrinsic function is undefined  */
,9,8,18,56,54,6,43,10,4,88
};
static const char __FAR Msg10752[] = {
/* illegal use of %s1 name %s2 in %s3 statement  */
 9,52,136,5,0,28,13,3,49,2
/* symbolic name %s1 is longer than 6 characters  */
,9,165,28,0,4,255,51,76,230,184
/* %s1 has already been defined as a %s2  */
,8,0,38,60,46,50,45,7,13
/* %s1 %s2 has not been defined  */
,6,0,13,38,1,46,50
/* %s1 is an unreferenced symbol  */
,7,0,4,14,255,85,255,80
/* %s1 already belongs to this NAMELIST group  */
,9,0,60,255,218,6,64,174,255,38
/* %s1 has been used but not defined  */
,7,0,38,46,114,26,1,50
/* dynamically allocating %s1 is not FORTRAN 77 standard  */
,6,255,247,0,4,1,11
/* %s1 in NAMELIST %s2 is illegal  */
,6,0,3,174,13,4,52
};
static const char __FAR Msg11008[] = {
 0
/* Usage: %s1 {- or /<option>} <file-spec> {- or /<option>} {<prog-parm>}  */
,16,255,193,0,255,90,12,229,255,120,255,90,12,229,255,255,140
/* generate a listing file  */
,4,130,7,108,22
/* listing file to printer  */
,6,108,22,6,255,255,71
/* listing file to terminal  */
,5,108,22,6,255,81
/* listing file to disk  */
,5,108,22,6,255,244
/* generate an error file  */
,4,130,14,62,22
/* issue extension messages  */
,4,205,255,34,157
/* issue unreferenced warning  */
,5,205,255,85,255,86
/* issue warning messages  */
,4,205,255,86,157
/* INTEGER/LOGICAL size 2/1  */
,5,255,167,33,255,112
/* list INCLUDE files  */
,3,40,242,128
/* display diagnostic messages  */
,3,255,245,157
/* explicit typing required  */
,5,255,255,4,255,75
/* extend line length  */
,4,255,33,39,53
/* invoke run-time debugger  */
,6,204,255,255,86,255,15
,0
/* define macro  */
,2,255,237
/* relax FORMAT type checking  */
,5,255,74,78,24,148
/* relax wild branch checking  */
,7,255,74,255,255,136,71,148
/* generate object code  */
,4,130,255,58,81
/* use editor interface  */
,6,136,255,20,255,255,33
/* generate an executable  */
,3,130,14,127
/* run the executable file  */
,6,255,255,85,30,127,22
/* unit 6 output to terminal  */
,7,77,230,255,63,6,255,81
/* log unit *  */
,6,255,255,46,77,255,99
/* undefined variable checking  */
,3,88,25,148
/* array type checking  */
,3,34,24,148
/* statement limit  */
,2,2,86
/* page limit  */
,3,255,66,86
/* time limit  */
,2,166,86
/* code size limit  */
,3,81,33,86
/* number of lines per page  */
,7,18,5,208,255,68,255,66
/* report integer overflows  */
,4,213,63,255,65
/* report float-pt. overflows  */
,4,213,197,255,65
/* report float-pt. underflows  */
,5,213,197,255,255,125
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
/* structure definition  */
,2,29,66
/* group name  */
,3,255,38,28
/* statement function  */
,2,2,10
/* statement function definition  */
,3,2,10,66
/* simple variable  */
,4,255,255,90,25
/* variable in COMMON  */
,3,25,3,44
/* equivalenced variable  */
,2,195,25
/* subprogram argument  */
,2,55,8
/* statement function argument  */
,3,2,10,8
/* array  */
,1,34
/* allocated array  */
,2,252,34
/* variable or array  */
,3,25,12,34
/* intrinsic function  */
,2,43,10
,0
/* external subprogram  */
,4,255,255,7,55
/* assignment  */
,1,145
/* arithmetic if  */
,2,255,210
/* at end of statement,  */
,6,79,83,5,255,255,98
/* Defining subprogram: %s1; first ISN is %u2  */
,7,255,144,255,35,244,4,138
/* library member %s1,  */
,3,255,49,137
/* column %d1,  */
,3,186,255,93
/* in %s1,  */
,2,3,137
/* Unable to start editor  */
,8,255,191,6,255,255,97,255,20
/* To exit, press Ctrl/Z and then press Enter  */
,14,255,189,255,70,255,139,61,255,255,116,255,70,255,152
/* unable to execute command  */
,6,255,83,6,255,255,1
/* %d1 statements, %i2 bytes, %d3 extensions, %d4 warnings, %d5 errors  */
,11,56,255,255,99,224,255,255,5,225,255,252
/* Code size (in bytes):  */
,5,255,136,33,117,182
/* Number of errors:  */
,4,143,5,255,253
/* Compile time (in seconds):  */
,6,255,138,166,117,255,78
/* Number of warnings:  */
,5,143,5,255,255,133
,0
/* Number of extensions:  */
,5,143,5,255,255,6
/* - Executing line %u1 in file %s2  */
,7,23,241,39,228,3,22,13
/* - Called from line %u1 in file %s2  */
,8,23,236,94,39,228,3,22,13
/* error writing to listing file - %s1  */
,8,62,255,89,6,108,22,23,0
/* - Executing in  */
,3,23,241,3
/* - Called from  */
,3,23,236,94
/* %s1, statement %u2 in library member %s3  */
,7,137,2,138,3,255,49,49
/* %s1, statement %u2 in file %s3  */
,6,137,2,138,3,22,49
/* Do you really want to stop (y/n)?  */
,6,255,146,6,255,255,100
/* *ERR* math coprocessor is not present\r\n  */
,7,255,104,4,1,255,255,69
/* invalid ISN  */
,2,16,244
/* Debugger entered at %s1  */
,6,255,143,255,24,79,0
/* Breakpoint set at %d1  */
,5,255,127,162,79,56
/* Returned from %s1  */
,4,255,185,94,0
/* Into[I key] Over[space bar] Next[N key] Exit[Enter key]  */
,9,255,169,206,255,180,206,255,156,206
/* variable not found  */
,3,25,1,73
/* array dimensions not yet established  */
,9,34,255,242,1,255,255,138,255,29
/* not understood  */
,4,1,255,255,126
/* invalid substring list  */
,3,16,70,40
/* invalid subscript list  */
,3,16,218,40
/* unit not found  */
,3,77,1,73
/* unable to open listing file  */
,8,255,83,6,255,255,58,108,22
/* Breakpoint[/c] [<isn>].....show/set/clear breakpoint  */
,2,255,128
/* Display [*|<isn>[:<isn>]]..display source  */
,3,255,145,216
/* Go [<isn>].................go [until <isn> reached]  */
,2,255,162
/* Help.......................display this help screen  */
,6,255,163,64,255,255,25
/* Logio [<unit>].............list logged units or toggle logging of unit  */
,10,255,175,255,84,12,255,255,117,5,77
/* Quit.......................quit debugger  */
,4,255,181,255,15
/* Trace......................enter trace mode  */
,3,255,190,210
/* Unit [<unit>]..............list information for connected unit(s)  */
,9,255,192,151,35,255,9,255,255,128
/* Where......................display current traceback information  */
,3,255,195,151
/* ! [<cmd>]..................system services  */
,2,255,91
/* / <x> <y>..................assign <y> to <x>  */
,7,255,109,231,255,121,6,231
/* ? [(z|a)] [<x>[,<x>]...]...print variables using specified format  */
,6,255,122,168,167,164,42
/* *File Management Options*  */
,3,255,105,119
/* *Diagnostic Options*  */
,3,255,103,119
/* *Debugging Options*  */
,3,255,102,119
/* *Miscellaneous Options*  */
,3,255,106,119
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
/* *Character Set Options*  */
,3,255,101,119
/* Chinese character set  */
,4,255,135,27,162
/* Japanese character set  */
,4,255,171,27,162
/* Korean character set  */
,4,255,172,27,162
,0
/* *Application Type*  */
,2,255,100
/* messages in resource file  */
,6,157,3,255,255,84,22
/* devices are carriage control  */
,6,255,240,121,255,224,82
/* operate quietly  */
,3,255,255,59
/* Execution time (in seconds):  */
,6,255,155,166,117,255,78
/* Local data size (in bytes):  */
,6,255,174,41,33,117,182
/* Global data size (in bytes):  */
,6,255,161,41,33,117,182
/* Object bytes free:  */
,7,255,179,255,3,255,255,17
/* Number of statements executed:  */
,6,143,5,112,255,255,2
/* generate browse information  */
,4,130,255,221,151
/* extend float-pt. precision  */
,6,255,33,197,255,255,68
,0
,0
/* LF with FF  */
,5,255,173,32,255,157
,0
,0
/* Codeview debugging info.  */
,2,255,137
,0
,0
/* DO-variables do not overflow  */
,4,255,142,1,159
/* path for INCLUDE files  */
,6,255,255,63,35,242,128
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
    ,Msg256
    ,Msg512
    ,Msg768
    ,Msg1024
    ,Msg1280
    ,Msg1536
    ,Msg1792
    ,Msg2048
    ,Msg2304
    ,Msg2560
    ,Msg2816
    ,Msg3072
    ,Msg3328
    ,Msg3584
    ,Msg3840
    ,Msg4096
    ,Msg4352
    ,Msg4608
    ,Msg4864
    ,Msg5120
    ,Msg5376
    ,Msg5632
    ,Msg5888
    ,Msg6144
    ,Msg6400
    ,Msg6656
    ,Msg6912
    ,Msg7168
    ,Msg7424
    ,Msg7680
    ,Msg7936
    ,Msg8192
    ,Msg8448
    ,Msg8704
    ,Msg8960
    ,Msg9216
    ,Msg9472
    ,Msg9728
    ,Msg9984
    ,Msg10240
    ,Msg10496
    ,Msg10752
    ,Msg11008
    };


extern const char __FAR ErrWord[] = {
          3,'%','s','1'
/* count=119 Group:offset = AR: 0, AR: 1, AR: 6, AR:12, BD: 0,... */
          ,3,'n','o','t'
/* count=111 Group:offset = AR: 9, AR:12, BD: 0, BD: 1, CC: 3,... */
          ,9,'s','t','a','t','e','m','e','n','t'
/* count= 94 Group:offset = BD: 1, CC: 1, CC: 2, CV: 2, DA: 6,... */
          ,2,'i','n'
/* count= 91 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,2,'i','s'
/* count= 80 Group:offset = BD: 0, BD: 1, CC: 3, CC: 5, CC: 6,... */
          ,2,'o','f'
/* count= 73 Group:offset = AR: 0, AR: 6, AR: 9, AR:12, CM: 1,... */
          ,2,'t','o'
/* count= 62 Group:offset = AR: 0, AR: 2, AR: 3, AR: 4, AR: 5,... */
          ,1,'a'
/* count= 42 Group:offset = AR: 5, AR: 7, BD: 0, BD: 1, CC: 4,... */
          ,8,'a','r','g','u','m','e','n','t'
/* count= 42 Group:offset = AR: 1, AR: 2, AR: 3, AR: 4, AR: 5,... */
          ,2,'b','e'
/* count= 41 Group:offset = AR: 2, CC: 4, CV: 1, DA: 0, DA: 1,... */
          ,8,'f','u','n','c','t','i','o','n'
/* count= 38 Group:offset = AR: 0, AR: 4, AR: 5, AR: 7, EY: 0,... */
          ,19,'F','O','R','T','R','A','N',' ','7'
              ,'7',' ','s','t','a','n','d','a','r','d'
/* count= 37 Group:offset = CC: 3, CC: 5, CC: 6, CC: 7, CM: 1,... */
          ,2,'o','r'
/* count= 35 Group:offset = AR: 2, DM: 1, DO: 5, FM: 1, FM: 4,... */
          ,3,'%','s','2'
/* count= 34 Group:offset = AR: 4, AR:12, DA: 5, DA:12, EC: 0,... */
          ,2,'a','n'
/* count= 32 Group:offset = AR: 4, CO: 2, DA: 0, DA: 2, IL: 1,... */
          ,9,'e','x','p','e','c','t','i','n','g'
/* count= 31 Group:offset = CO: 2, DO: 5, EQ: 4, FM: 9, GO: 2,... */
          ,7,'i','n','v','a','l','i','d'
/* count= 31 Group:offset = AR: 0, CC: 0, CC: 1, CN: 1, CN: 3,... */
          ,4,'m','u','s','t'
/* count= 31 Group:offset = AR: 2, AR:12, CC: 4, CV: 1, DA: 0,... */
          ,6,'n','u','m','b','e','r'
/* count= 28 Group:offset = AR: 0, AR: 2, AR: 3, AR: 4, AR: 5,... */
          ,7,'a','l','l','o','w','e','d'
/* count= 26 Group:offset = CN: 2, CO: 5, CO: 8, CO:10, CV: 0,... */
          ,9,'s','p','e','c','i','f','i','e','r'
/* count= 24 Group:offset = IL:12, IL:14, IL:15, IL:17, IM: 3,... */
          ,6,'c','a','n','n','o','t'
/* count= 23 Group:offset = CO: 1, DA: 3, DA: 5, DA:12, DO: 3,... */
          ,4,'f','i','l','e'
/* count= 23 Group:offset = IL: 1, IL:10, IO: 8, IO:11, IO:15,... */
          ,1,'-'
/* count= 22 Group:offset = AR: 3, AR: 4, AR: 5, AR: 7, AR: 8,... */
          ,4,'t','y','p','e'
/* count= 22 Group:offset = AR: 9, EV: 5, EX: 1, EY: 0, IM: 2,... */
          ,8,'v','a','r','i','a','b','l','e'
/* count= 22 Group:offset = CV: 0, CV: 1, DA: 0, DA: 2, DA:12,... */
          ,3,'b','u','t'
/* count= 20 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,9,'c','h','a','r','a','c','t','e','r'
/* count= 20 Group:offset = CC: 0, CC: 1, CC: 2, CC: 3, CM: 2,... */
          ,4,'n','a','m','e'
/* count= 20 Group:offset = PR: 0, SA: 2, SF: 1, SF: 6, SP: 2,... */
          ,9,'s','t','r','u','c','t','u','r','e'
/* count= 20 Group:offset = AR: 9, IO:35, SF: 6, SP: 3, SP: 4,... */
          ,3,'t','h','e'
/* count= 20 Group:offset = CO: 2, CO: 3, CO: 9, EC: 0, IF: 0,... */
          ,3,'w','a','s'
/* count= 20 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 6,... */
          ,4,'w','i','t','h'
/* count= 20 Group:offset = CM: 3, CM: 5, CO:10, CV: 0, CV: 1,... */
          ,4,'s','i','z','e'
/* count= 19 Group:offset = AR: 2, CM: 3, CM: 5, EY: 2, FM:16,... */
          ,5,'a','r','r','a','y'
/* count= 18 Group:offset = DM: 1, IL:17, LI:16, MO: 3, MO: 4,... */
          ,3,'f','o','r'
/* count= 18 Group:offset = CO: 6, DA: 8, DA:10, DO: 7, FM:10,... */
          ,7,'m','i','s','s','i','n','g'
/* count= 18 Group:offset = EN: 0, FM: 0, FM: 1, FM: 3, FM: 4,... */
          ,5,'b','l','o','c','k'
/* count= 16 Group:offset = BD: 0, CM: 1, CM: 3, CM: 4, CM: 5,... */
          ,3,'h','a','s'
/* count= 16 Group:offset = CM: 3, CM: 5, EC: 0, EV: 0, IL: 6,... */
          ,4,'l','i','n','e'
/* count= 16 Group:offset = CC: 4, DO: 0, GO: 0, GO: 1, PR: 3,... */
          ,4,'l','i','s','t'
/* count= 16 Group:offset = DA: 8, DA:10, EV: 1, IL: 4, IL: 5,... */
          ,4,'d','a','t','a'
/* count= 15 Group:offset = BD: 0, CM: 1, CM: 2, DA: 4, DA: 6,... */
          ,6,'f','o','r','m','a','t'
/* count= 15 Group:offset = FM: 0, FM: 2, FM: 4, FM: 5, FM: 7,... */
          ,9,'i','n','t','r','i','n','s','i','c'
/* count= 15 Group:offset = AR: 0, AR: 4, LI: 5, LI: 6, LI: 9,... */
          ,6,'C','O','M','M','O','N'
/* count= 14 Group:offset = BD: 0, CM: 0, CM: 1, CM: 2, CM: 3,... */
          ,2,'a','s'
/* count= 14 Group:offset = IL:17, LI: 9, LI:12, SR: 1, SR:11,... */
          ,4,'b','e','e','n'
/* count= 13 Group:offset = CM: 3, CM: 5, EV: 0, KO: 7, SP:23,... */
          ,8,'c','o','n','s','t','a','n','t'
/* count= 13 Group:offset = CN: 1, CN: 3, CN: 4, DA: 4, DA:12,... */
          ,13,'s','p','e','c','i','f','i','c','a'
              ,'t','i','o','n'
/* count= 13 Group:offset = DA: 1, FM: 6, FM:10, FM:14, FM:17,... */
          ,3,'%','s','3'
/* count= 12 Group:offset = AR: 2, AR: 3, AR: 5, AR: 7, AR: 8,... */
          ,7,'d','e','f','i','n','e','d'
/* count= 12 Group:offset = CM: 3, CM: 5, DO: 0, SA: 0, SP:23,... */
          ,10,'e','x','p','r','e','s','s','i','o'
              ,'n'
/* count= 12 Group:offset = CV: 0, DA: 2, DM: 0, SP:29, SS: 2,... */
          ,7,'i','l','l','e','g','a','l'
/* count= 12 Group:offset = EQ: 0, EQ: 2, IM: 0, IO:27, PR: 1,... */
          ,6,'l','e','n','g','t','h'
/* count= 12 Group:offset = CN: 2, CV: 0, CV: 1, IM: 3, LI: 8,... */
          ,6,'p','a','s','s','e','d'
/* count= 12 Group:offset = AR: 2, AR: 4, AR: 5, AR: 7, AR: 8,... */
          ,10,'s','u','b','p','r','o','g','r','a'
              ,'m'
/* count= 12 Group:offset = BD: 0, BD: 1, CM: 1, CM: 4, CV: 1,... */
          ,3,'%','d','1'
/* count= 11 Group:offset = AR: 2, AR: 3, AR: 4, AR: 5, AR: 7,... */
          ,3,'%','d','2'
/* count= 11 Group:offset = DO: 0, GO: 0, GO: 1, SP: 8, SP: 9,... */
          ,3,'%','i','1'
/* count= 11 Group:offset = DO: 0, GO: 1, SP: 8, SP: 9, ST: 0,... */
          ,2,'D','O'
/* count= 11 Group:offset = DA: 0, DA: 2, DO: 0, DO: 1, DO: 2,... */
          ,7,'a','l','r','e','a','d','y'
/* count= 11 Group:offset = CM: 0, CO: 0, DO: 0, IO: 1, MO: 4,... */
          ,3,'a','n','d'
/* count= 11 Group:offset = CM: 2, CV: 2, EC: 1, EV: 4, IL:12,... */
          ,5,'e','r','r','o','r'
/* count= 11 Group:offset = IO: 8, IO:31, IO:32, IO:33, SM: 0,... */
          ,7,'i','n','t','e','g','e','r'
/* count= 11 Group:offset = DA: 0, DA: 1, IO:36, KO: 3, KO: 5,... */
          ,4,'t','h','i','s'
/* count= 11 Group:offset = CV: 0, DO: 4, FM:10, IL: 1, LI:11,... */
          ,7,'a','t','t','e','m','p','t'
/* count= 10 Group:offset = EV: 5, IL: 5, IO:15, MO: 3, MO: 4,... */
          ,10,'d','e','f','i','n','i','t','i','o'
              ,'n'
/* count= 10 Group:offset = AR:12, SF: 0, SF: 2, SP:20, SP:22,... */
          ,5,'d','u','m','m','y'
/* count= 10 Group:offset = AR: 1, AR: 3, AR:12, SF: 0, SF: 2,... */
          ,6,'o','p','t','i','o','n'
/* count= 10 Group:offset = CO: 1, CO: 2, CO: 3, CO: 4, CO: 5,... */
          ,8,'r','e','q','u','i','r','e','s'
/* count= 10 Group:offset = CO: 3, IL: 8, IO: 0, IO: 2, IO: 3,... */
          ,9,'s','u','b','s','t','r','i','n','g'
/* count= 10 Group:offset = EV: 3, EV: 5, IO:33, SS: 3, SV: 5,... */
          ,6,'b','r','a','n','c','h'
/* count=  9 Group:offset = SP: 8, SP: 9, SP:12, ST: 3, ST: 7,... */
          ,4,'d','o','e','s'
/* count=  9 Group:offset = AR: 9, EY: 0, EY: 2, IO: 9, IO:10,... */
          ,5,'f','o','u','n','d'
/* count=  9 Group:offset = AR: 6, FM: 9, IO:30, LI: 7, LI:13,... */
          ,4,'o','n','l','y'
/* count=  9 Group:offset = DA:13, IM: 1, MD: 4, RE: 0, SR: 0,... */
          ,8,'o','p','e','r','a','t','o','r'
/* count=  9 Group:offset = MD: 0, MD: 2, MD: 3, MD: 5, MD: 6,... */
          ,4,'t','h','a','n'
/* count=  9 Group:offset = AR: 1, CC: 5, CM: 4, FM:13, IL: 5,... */
          ,4,'u','n','i','t'
/* count=  9 Group:offset = IL: 9, IL:10, IL:11, IO:14, IO:23,... */
          ,6,'F','O','R','M','A','T'
/* count=  8 Group:offset = FM: 9, FM:11, FM:16, FM:18, ST: 5,... */
          ,2,'a','t'
/* count=  8 Group:offset = DO: 1, EV: 1, SP: 7, SP:19, SP:25,... */
          ,4,'c','a','l','l'
/* count=  8 Group:offset = AR: 2, AR: 3, AR: 5, AR: 6, AR: 7,... */
          ,4,'c','o','d','e'
/* count=  8 Group:offset = FM: 2, FM: 7, FM:10, FM:15, FM:20,... */
          ,7,'c','o','n','t','r','o','l'
/* count=  8 Group:offset = EY: 1, IL: 4, IL: 5, SP: 4, SP: 8,... */
          ,3,'e','n','d'
/* count=  8 Group:offset = FM: 9, IO:15, SP:10, SX: 3, SX: 6,... */
          ,8,'e','x','c','e','e','d','e','d'
/* count=  8 Group:offset = CO: 6, CO: 9, IO:22, IO:37, KO: 6,... */
          ,12,'i','n','p','u','t','/','o','u','t'
              ,'p','u','t'
/* count=  8 Group:offset = IL: 6, IL: 7, IO: 1, IO: 3, IO: 4,... */
          ,5,'l','i','m','i','t'
/* count=  8 Group:offset = CO: 9, KO: 8, MO: 0, SV: 8, MS:28,... */
          ,2,'n','o'
/* count=  8 Group:offset = CC: 2, FM:11, FM:12, IL: 6, IO:35,... */
          ,9,'u','n','d','e','f','i','n','e','d'
/* count=  8 Group:offset = SP:13, UV: 0, UV: 1, UV: 2, UV: 3,... */
          ,4,'z','e','r','o'
/* count=  8 Group:offset = CN: 2, DO: 7, KO: 0, KO: 3, LI: 0,... */
          ,5,'(','%','s','2',')'
/* count=  7 Group:offset = AR: 2, AR: 3, AR: 5, AR: 7, AR: 8,... */
          ,5,'B','L','O','C','K'
/* count=  7 Group:offset = BD: 1, CM: 4, SP: 4, SP:12, SP:13,... */
          ,7,'a','p','p','e','a','r','s'
/* count=  7 Group:offset = AR: 1, CM: 4, DA: 2, SA: 1, ST: 1,... */
          ,5,'f','i','e','l','d'
/* count=  7 Group:offset = FM: 8, FM:13, FM:14, IO: 6, SP:21,... */
          ,4,'f','r','o','m'
/* count=  7 Group:offset = KO: 4, SP: 3, SP: 8, SP:15, MS:129,... */
          ,5,'m','a','t','c','h'
/* count=  7 Group:offset = AR: 9, EY: 0, EY: 2, IO: 9, IO:10,... */
          ,4,'o','n','c','e'
/* count=  7 Group:offset = AR: 1, DA:13, IL: 5, IM: 1, IM: 4,... */
          ,7,'p','r','o','g','r','a','m'
/* count=  7 Group:offset = CP: 0, KO: 4, RE: 1, SR: 5, SR: 9,... */
          ,5,'r','a','n','g','e'
/* count=  7 Group:offset = DO: 1, IM: 0, IO:37, LI: 4, SP: 4,... */
          ,10,'s','u','b','r','o','u','t','i','n'
              ,'e'
/* count=  7 Group:offset = AR: 5, AR: 7, RE: 0, SR: 2, SR: 4,... */
          ,5,'\'','%','s','1','\''
/* count=  6 Group:offset = CO: 1, CO: 3, CO: 4, CO: 5, CO: 9,... */
          ,6,'a','c','c','e','s','s'
/* count=  6 Group:offset = IL:13, IO: 0, IO: 2, IO: 5, IO:17,... */
          ,6,'a','p','p','e','a','r'
/* count=  6 Group:offset = AR:12, GO: 0, SR: 0, SR: 3, SR: 7,... */
          ,3,'c','a','n'
/* count=  6 Group:offset = DA:13, IM: 1, MD: 4, SR: 0, SR: 2,... */
          ,4,'e','d','i','t'
/* count=  6 Group:offset = FM: 4, FM: 5, FM:12, FM:18, FM:19,... */
          ,8,'e','x','p','e','c','t','e','d'
/* count=  6 Group:offset = AR: 5, AR: 6, AR: 7, AR: 8, AR:11,... */
          ,3,'i','/','o'
/* count=  6 Group:offset = IL:15, IL:16, IO:29, IO:32, IO:33,... */
          ,5,'i','n','p','u','t'
/* count=  6 Group:offset = CC: 0, CO: 5, FM:17, IO: 6, IO:10,... */
          ,7,'l','i','s','t','i','n','g'
/* count=  6 Group:offset = MS: 2, MS: 3, MS: 4, MS: 5, MS:130,... */
          ,4,'l','o','o','p'
/* count=  6 Group:offset = DO: 0, DO: 2, DO: 3, DO: 4, DO: 6,... */
          ,2,'o','n'
/* count=  6 Group:offset = FM:11, FM:12, FM:17, SP: 9, SX:20,... */
          ,3,'o','n','e'
/* count=  6 Group:offset = CM: 4, LI: 1, LI: 8, SP: 7, SP:25,... */
          ,10,'s','t','a','t','e','m','e','n','t'
              ,'s'
/* count=  6 Group:offset = IM: 4, IO: 3, IO: 4, IO:17, IO:18,... */
          ,3,'t','o','o'
/* count=  6 Group:offset = CC: 8, DA:10, DM: 1, IO:26, ST: 6,... */
          ,4,'u','s','e','d'
/* count=  6 Group:offset = SP:30, SR: 2, ST: 9, ST:15, SV: 7,... */
          ,5,'v','a','l','u','e'
/* count=  6 Group:offset = AR:11, EQ: 1, LI: 1, SS: 0, UV: 1,... */
          ,3,'(','*',')'
/* count=  5 Group:offset = CV: 0, CV: 1, IM: 3, SF: 1, SR: 1 */
          ,3,'(','i','n'
/* count=  5 Group:offset = MS:122, MS:124, MS:203, MS:204, MS:205 */
          ,4,'D','A','T','A'
/* count=  5 Group:offset = BD: 1, CM: 4, CO: 8, CO:10, ST:20 */
          ,8,'O','p','t','i','o','n','s','*'
/* count=  5 Group:offset = MS:161, MS:162, MS:163, MS:164, MS:194 */
          ,6,'R','E','M','O','T','E'
/* count=  5 Group:offset = SP: 4, SP:12, SP:13, SP:14, SP:15 */
          ,3,'a','r','e'
/* count=  5 Group:offset = CN: 0, CN: 2, EC: 1, SP: 6, MS:201 */
          ,9,'a','r','g','u','m','e','n','t','s'
/* count=  5 Group:offset = AR: 0, AR: 6, LI: 5, SF: 0, SF: 5 */
          ,2,'b','y'
/* count=  5 Group:offset = AR:11, KO: 0, KO: 3, SP: 5 */
          ,12,'c','o','n','t','i','n','u','a','t'
              ,'i','o','n'
/* count=  5 Group:offset = CC: 2, CC: 4, CC: 5, CC: 8, PR: 3 */
          ,10,'d','e','s','c','r','i','p','t','o'
              ,'r'
/* count=  5 Group:offset = FM: 4, FM: 5, FM:12, FM:18, IO:26 */
          ,6,'d','u','r','i','n','g'
/* count=  5 Group:offset = DA:13, IO:22, IO:31, IO:32, IO:33 */
          ,10,'e','x','e','c','u','t','a','b','l'
              ,'e'
/* count=  5 Group:offset = SR:10, ST: 1, ST:22, MS:22, MS:23 */
          ,5,'f','i','l','e','s'
/* count=  5 Group:offset = IL: 7, IL:13, IO:35, MS:11, MS:219 */
          ,14,'f','l','o','a','t','i','n','g','-'
              ,'p','o','i','n','t'
/* count=  5 Group:offset = CN: 1, IO:37, KO: 0, KO: 1, KO: 2 */
          ,8,'g','e','n','e','r','a','t','e'
/* count=  5 Group:offset = MS: 2, MS: 6, MS:20, MS:22, MS:208 */
          ,5,'l','a','b','e','l'
/* count=  5 Group:offset = DO: 3, GO: 0, SP: 2, ST:21, ST:22 */
          ,6,'m','e','m','o','r','y'
/* count=  5 Group:offset = CP: 8, CP: 9, MO: 1, MO: 2, MO: 5 */
          ,4,'m','o','r','e'
/* count=  5 Group:offset = AR: 1, CC: 5, CM: 4, FM: 9, IL: 5 */
          ,10,'r','e','f','e','r','e','n','c','e'
              ,'d'
/* count=  5 Group:offset = SR:11, SR:14, SR:15, SR:17, ST: 5 */
          ,10,'s','e','q','u','e','n','t','i','a'
              ,'l'
/* count=  5 Group:offset = IL:13, IO: 0, IO: 2, IO: 5, IO:18 */
          ,3,'u','s','e'
/* count=  5 Group:offset = EQ: 2, SP:14, SP:27, VA: 0, MS:21 */
          ,4,'%','s','1',','
/* count=  4 Group:offset = MS:115, MS:117, MS:133, MS:134 */
          ,3,'%','u','2'
/* count=  4 Group:offset = MO: 0, MS:114, MS:133, MS:134 */
          ,9,'(','%','u','1',':','%','u','2',')'
/* count=  4 Group:offset = UV: 0, UV: 1, UV: 2, UV: 3 */
          ,16,'D','O','U','B','L','E',' ','P','R'
              ,'E','C','I','S','I','O','N'
/* count=  4 Group:offset = CN: 0, MD: 1, SX:12, SX:17 */
          ,8,'I','M','P','L','I','C','I','T'
/* count=  4 Group:offset = IM: 3, IM: 4, ST: 6 */
          ,17,'N','A','M','E','L','I','S','T','-'
              ,'d','i','r','e','c','t','e','d'
/* count=  4 Group:offset = IL:16, IO:31, IO:32, IO:33 */
          ,6,'N','u','m','b','e','r'
/* count=  4 Group:offset = MS:123, MS:125, MS:127, MS:207 */
          ,3,'R','E','C'
/* count=  4 Group:offset = IL:14, IO:17, IO:18, IO:24 */
          ,10,'a','s','s','i','g','n','m','e','n'
              ,'t'
/* count=  4 Group:offset = CV: 2, EQ: 0, EQ: 3, MS:111 */
          ,7,'a','s','s','u','m','e','d'
/* count=  4 Group:offset = FM: 0, FM:19, SV: 6, SV: 7 */
          ,6,'b','e','f','o','r','e'
/* count=  4 Group:offset = AR:12, FM:19, ST: 4, TY: 0 */
          ,8,'c','h','e','c','k','i','n','g'
/* count=  4 Group:offset = MS:18, MS:19, MS:26, MS:27 */
          ,9,'c','o','n','s','t','a','n','t','s'
/* count=  4 Group:offset = CN: 0, CN: 2, DA: 8, DA:10 */
          ,4,'h','a','v','e'
/* count=  4 Group:offset = IL: 2, IL: 3, IL:12, SP: 0 */
          ,11,'i','n','f','o','r','m','a','t','i'
              ,'o','n'
/* count=  4 Group:offset = SM: 6, MS:156, MS:157, MS:208 */
          ,11,'i','n','i','t','i','a','l','i','z'
              ,'e','d'
/* count=  4 Group:offset = BD: 0, DA: 3, DA:13, ST: 4 */
          ,8,'i','n','t','e','r','n','a','l'
/* count=  4 Group:offset = IL: 1, IL: 7, IL:13, IO:11 */
          ,4,'i','t','e','m'
/* count=  4 Group:offset = IL: 4, IL: 5, IO:10, SV: 7 */
          ,4,'m','a','i','n'
/* count=  4 Group:offset = RE: 1, SR: 5, SR: 9, SR:16 */
          ,3,'m','a','y'
/* count=  4 Group:offset = GO: 0, IL: 1, IO:19, SR: 1 */
          ,8,'m','e','s','s','a','g','e','s'
/* count=  4 Group:offset = MS: 7, MS: 9, MS:12, MS:200 */
          ,3,'o','u','t'
/* count=  4 Group:offset = MO: 1, SP:12, SS: 2, SS: 3 */
          ,8,'o','v','e','r','f','l','o','w'
/* count=  4 Group:offset = IO:36, KO: 1, KO: 5, MS:218 */
          ,11,'p','a','r','e','n','t','h','e','s'
              ,'i','s'
/* count=  4 Group:offset = PC: 0, PC: 1, PC: 2, SX: 3 */
          ,6,'r','e','c','o','r','d'
/* count=  4 Group:offset = IO:22, IO:26, IO:35 */
          ,3,'s','e','t'
/* count=  4 Group:offset = MS:139, MS:195, MS:196, MS:197 */
          ,4,'s','i','g','n'
/* count=  4 Group:offset = CO: 2, EQ: 2, EQ: 4, UV: 0 */
          ,9,'s','p','e','c','i','f','i','e','d'
/* count=  4 Group:offset = IO:14, IO:23, PR: 1, MS:160 */
          ,8,'s','y','m','b','o','l','i','c'
/* count=  4 Group:offset = PR: 0, SP: 2, ST:18, VA: 1 */
          ,4,'t','i','m','e'
/* count=  4 Group:offset = KO: 8, MS:30, MS:124, MS:203 */
          ,5,'u','s','i','n','g'
/* count=  4 Group:offset = DM: 0, MD: 4, SV: 4, MS:160 */
          ,9,'v','a','r','i','a','b','l','e','s'
/* count=  4 Group:offset = DA: 8, DA:10, SS: 1, MS:160 */
          ,6,'w','i','t','h','i','n'
/* count=  4 Group:offset = EY: 1, SP:15, SP:28, SP:30 */
          ,1,'1'
/* count=  3 Group:offset = CC: 7, FM:19, PR: 3 */
          ,1,'2'
/* count=  3 Group:offset = EV: 0, EV: 1, LI: 5 */
          ,9,'C','H','A','R','A','C','T','E','R'
/* count=  3 Group:offset = CV: 0, EV: 4, SX:18 */
          ,8,'I','N','T','E','G','E','R',','
/* count=  3 Group:offset = EX: 1, SX:12, SX:17 */
          ,8,'N','A','M','E','L','I','S','T'
/* count=  3 Group:offset = IO:30, VA: 5, VA: 8 */
          ,6,'S','T','A','T','U','S'
/* count=  3 Group:offset = IO:13, IO:20, IO:21 */
          ,4,'U','N','I','T'
/* count=  3 Group:offset = IL: 1, IL:12, IO:25 */
          ,6,'a','c','t','i','v','e'
/* count=  3 Group:offset = DO: 6, IO: 1, SR:12 */
          ,5,'a','f','t','e','r'
/* count=  3 Group:offset = ST: 1, ST:11, SX: 3 */
          ,8,'a','s','t','e','r','i','s','k'
/* count=  3 Group:offset = IL: 8, IL:11, SX: 4 */
          ,5,'b','l','a','n','k'
/* count=  3 Group:offset = CC: 4, CM: 5, DA: 3 */
          ,4,'b','o','t','h'
/* count=  3 Group:offset = IL:12, SP: 0, SR: 3 */
          ,7,'b','y','t','e','s',')',':'
/* count=  3 Group:offset = MS:122, MS:204, MS:205 */
          ,13,'c','h','a','r','a','c','t','e','r'
              ,'*','(','*',')'
/* count=  3 Group:offset = LI:16, MO: 3, MO: 4 */
          ,10,'c','h','a','r','a','c','t','e','r'
              ,'s'
/* count=  3 Group:offset = HO: 1, IM: 0, VA: 1 */
          ,7,'c','l','o','s','i','n','g'
/* count=  3 Group:offset = FM:12, PC: 0, SX:20 */
          ,6,'c','o','l','u','m','n'
/* count=  3 Group:offset = CC: 7, PR: 3, MS:116 */
          ,11,'c','o','m','b','i','n','a','t','i'
              ,'o','n'
/* count=  3 Group:offset = IO:16, IO:21, IO:28 */
          ,5,'c','o','m','m','a'
/* count=  3 Group:offset = DO: 5, FM: 0, SX: 7 */
          ,8,'c','o','m','p','i','l','e','r'
/* count=  3 Group:offset = CO: 6, CO: 7, MO: 0 */
          ,7,'c','o','n','t','a','i','n'
/* count=  3 Group:offset = EV: 1, PR: 3, SP:25 */
          ,8,'d','e','c','l','a','r','e','d'
/* count=  3 Group:offset = IM: 1, SR: 1, TY: 5 */
          ,9,'d','i','f','f','e','r','e','n','t'
/* count=  3 Group:offset = CM: 3, CM: 5, EV: 0 */
          ,3,'d','u','e'
/* count=  3 Group:offset = MO: 2, ST: 7, ST:17 */
          ,7,'d','y','n','a','m','i','c'
/* count=  3 Group:offset = CP: 8, CP: 9, MO: 2 */
          ,12,'e','q','u','i','v','a','l','e','n'
              ,'c','e','d'
/* count=  3 Group:offset = EC: 1, EV: 0, MS:102 */
          ,7,'e','x','c','e','e','d','s'
/* count=  3 Group:offset = FM:16, MO: 0, SP:10 */
          ,9,'f','l','o','a','t','-','p','t','.'
/* count=  3 Group:offset = MS:34, MS:35, MS:209 */
          ,9,'f','o','r','m','a','t','t','e','d'
/* count=  3 Group:offset = IO: 3, IO:26 */
          ,7,'g','r','e','a','t','e','r'
/* count=  3 Group:offset = FM:13, LI: 0, LI: 8 */
          ,7,'i','g','n','o','r','e','d'
/* count=  3 Group:offset = CO: 4, CO: 9, MO: 2 */
          ,7,'i','m','p','l','i','e','d'
/* count=  3 Group:offset = DA: 0, DA: 2, IL: 6 */
          ,11,'i','n','c','o','r','r','e','c','t'
              ,'l','y'
/* count=  3 Group:offset = DM: 0, EV: 2, SV: 4 */
          ,14,'i','n','i','t','i','a','l','i','z'
              ,'a','t','i','o','n'
/* count=  3 Group:offset = DA: 4, DA: 6, DA:13 */
          ,6,'i','n','v','o','k','e'
/* count=  3 Group:offset = SR:12, SR:18, MS:15 */
          ,5,'i','s','s','u','e'
/* count=  3 Group:offset = MS: 7, MS: 8, MS: 9 */
          ,4,'k','e','y',']'
/* count=  3 Group:offset = MS:141 */
          ,5,'l','e','a','s','t'
/* count=  3 Group:offset = EV: 1, SP: 7, SP:25 */
          ,5,'l','i','n','e','s'
/* count=  3 Group:offset = CC: 5, CC: 8, MS:32 */
          ,7,'l','o','g','i','c','a','l'
/* count=  3 Group:offset = MD: 0, MD: 6, MD: 7 */
          ,4,'m','o','d','e'
/* count=  3 Group:offset = IO: 0, IO: 2, MS:155 */
          ,8,'o','p','e','r','a','n','d','s'
/* count=  3 Group:offset = MD: 2, MD: 3, MD: 6 */
          ,6,'r','e','p','e','a','t'
/* count=  3 Group:offset = DA: 1, FM: 6, FM:10 */
          ,6,'r','e','p','o','r','t'
/* count=  3 Group:offset = MS:33, MS:34, MS:35 */
          ,6,'r','e','t','u','r','n'
/* count=  3 Group:offset = RE: 0, SS: 0, UV: 1 */
          ,5,'r','i','g','h','t'
/* count=  3 Group:offset = CV: 2, SX: 3, UV: 0 */
          ,6,'s','o','u','r','c','e'
/* count=  3 Group:offset = CC: 0, CO: 5, MS:150 */
          ,6,'s','t','r','i','n','g'
/* count=  3 Group:offset = FM: 4, FM: 5, SX:20 */
          ,9,'s','u','b','s','c','r','i','p','t'
/* count=  3 Group:offset = SS: 2, SV: 2, MS:146 */
          ,6,'s','y','s','t','e','m'
/* count=  3 Group:offset = IO: 8, SM: 0, SM: 2 */
          ,10,'u','n','e','x','p','e','c','t','e'
              ,'d'
/* count=  3 Group:offset = PC: 2, SX: 0, SX:19 */
          ,11,'u','n','f','o','r','m','a','t','t'
              ,'e','d'
/* count=  3 Group:offset = IO: 4, IO:22 */
          ,14,'u','n','r','e','c','o','g','n','i'
              ,'z','a','b','l','e'
/* count=  3 Group:offset = FM: 5, IL: 4, IM: 2 */
          ,5,'w','h','e','r','e'
/* count=  3 Group:offset = EX: 0, EX: 1, EX: 2 */
          ,3,'%','d','3'
/* count=  2 Group:offset = AR: 6, MS:121 */
          ,3,'%','d','5'
/* count=  2 Group:offset = AR: 2, MS:121 */
          ,3,'%','s','4'
/* count=  2 Group:offset = AR: 3, AR: 8 */
          ,3,'%','s','5'
/* count=  2 Group:offset = AR: 3, AR: 8 */
          ,3,'%','u','1'
/* count=  2 Group:offset = MS:128, MS:129 */
          ,10,'/','<','o','p','t','i','o','n','>'
              ,'}'
/* count=  2 Group:offset = MS: 1 */
          ,1,'6'
/* count=  2 Group:offset = VA: 1, MS:24 */
          ,3,'<','x','>'
/* count=  2 Group:offset = MS:159 */
          ,6,'A','S','S','I','G','N'
/* count=  2 Group:offset = GO: 0, GO: 1 */
          ,5,'A','T','E','N','D'
/* count=  2 Group:offset = SP: 0, SP: 1 */
          ,4,'C','A','S','E'
/* count=  2 Group:offset = SP: 5, SP: 7 */
          ,7,'C','O','M','P','L','E','X'
/* count=  2 Group:offset = CN: 0, MD: 1 */
          ,6,'C','a','l','l','e','d'
/* count=  2 Group:offset = MS:129, MS:132 */
          ,3,'E','N','D'
/* count=  2 Group:offset = EN: 0, IL:14 */
          ,5,'E','N','T','R','Y'
/* count=  2 Group:offset = AR:12, EY: 1 */
          ,11,'E','Q','U','I','V','A','L','E','N'
              ,'C','E'
/* count=  2 Group:offset = EV: 1, EV: 5 */
          ,8,'E','X','T','E','R','N','A','L'
/* count=  2 Group:offset = SR: 0, SR: 3 */
          ,9,'E','x','e','c','u','t','i','n','g'
/* count=  2 Group:offset = MS:128, MS:131 */
          ,7,'I','N','C','L','U','D','E'
/* count=  2 Group:offset = MS:11, MS:219 */
          ,9,'I','N','T','R','I','N','S','I','C'
/* count=  2 Group:offset = SR: 3, SR: 7 */
          ,3,'I','S','N'
/* count=  2 Group:offset = MS:114, MS:137 */
          ,5,'R','E','A','L',','
/* count=  2 Group:offset = SX:12, SX:17 */
          ,4,'R','E','C','L'
/* count=  2 Group:offset = IO:12, IO:16 */
          ,6,'R','E','T','U','R','N'
/* count=  2 Group:offset = RE: 1, SP:15 */
          ,1,'X'
/* count=  2 Group:offset = EX: 1, FM:19 */
          ,1,'Y'
/* count=  2 Group:offset = EX: 1, EX: 2 */
          ,6,'a','c','t','u','a','l'
/* count=  2 Group:offset = AR: 3, SF: 4 */
          ,11,'a','l','l','o','c','a','t','a','b'
              ,'l','e'
/* count=  2 Group:offset = LI:16, SP:28 */
          ,9,'a','l','l','o','c','a','t','e','d'
/* count=  2 Group:offset = MO: 4, MS:106 */
          ,11,'a','r','g','u','m','e','n','t','('
              ,'s',')'
/* count=  2 Group:offset = LI: 3, LI:10 */
          ,9,'a','r','g','u','m','e','n','t',','
/* count=  2 Group:offset = LI: 7, LI:13 */
          ,9,'a','t','t','r','i','b','u','t','e'
/* count=  2 Group:offset = PR: 1, PR: 2 */
          ,3,'b','a','d'
/* count=  2 Group:offset = IO: 6, SX: 1 */
          ,6,'b','l','o','c','k','s'
/* count=  2 Group:offset = EY: 1, SP:11 */
          ,5,'b','y','t','e','s'
/* count=  2 Group:offset = MO: 0, MS:206 */
          ,4,'c','a','s','e'
/* count=  2 Group:offset = SP:11 */
          ,5,'c','o','l','o','n'
/* count=  2 Group:offset = SX: 5, SX: 6 */
          ,7,'c','o','l','u','m','n','s'
/* count=  2 Group:offset = CC: 1, CC: 4 */
          ,7,'c','o','m','m','e','n','t'
/* count=  2 Group:offset = CC: 6, PR: 3 */
          ,7,'c','o','m','p','l','e','x'
/* count=  2 Group:offset = LI: 5, MD: 4 */
          ,9,'c','o','n','n','e','c','t','e','d'
/* count=  2 Group:offset = IO:14, MS:156 */
          ,10,'c','o','n','n','e','c','t','i','o'
              ,'n'
/* count=  2 Group:offset = IO: 3, IO: 4 */
          ,8,'c','o','n','t','a','i','n','s'
/* count=  2 Group:offset = SF: 0, SF: 2 */
          ,10,'c','o','n','v','e','r','t','i','n'
              ,'g'
/* count=  2 Group:offset = IO:36, IO:37 */
          ,5,'c','o','u','l','d'
/* count=  2 Group:offset = SP: 3, ST:14 */
          ,5,'c','o','u','n','t'
/* count=  2 Group:offset = FM:13, KO: 7 */
          ,8,'d','e','b','u','g','g','e','r'
/* count=  2 Group:offset = MS:15, MS:154 */
          ,10,'d','e','c','l','a','r','a','t','o'
              ,'r'
/* count=  2 Group:offset = SV: 0, TY: 0 */
          ,9,'d','i','m','e','n','s','i','o','n'
/* count=  2 Group:offset = DM: 0, SV: 8 */
          ,10,'d','i','r','e','c','t','i','v','e'
              ,'s'
/* count=  2 Group:offset = CO: 6, CO: 7 */
          ,6,'d','i','v','i','d','e'
/* count=  2 Group:offset = KO: 0, KO: 3 */
          ,6,'e','d','i','t','o','r'
/* count=  2 Group:offset = MS:21, MS:118 */
          ,8,'e','l','e','m','e','n','t','s'
/* count=  2 Group:offset = SS: 1, SV: 8 */
          ,11,'e','n','c','o','u','n','t','e','r'
              ,'e','d'
/* count=  2 Group:offset = CC: 0, CC: 3 */
          ,6,'e','n','o','u','g','h'
/* count=  2 Group:offset = DA: 8, HO: 1 */
          ,7,'e','n','t','e','r','e','d'
/* count=  2 Group:offset = SR:13, MS:138 */
          ,5,'e','n','t','r','y'
/* count=  2 Group:offset = EY: 0, EY: 2 */
          ,5,'e','q','u','a','l'
/* count=  2 Group:offset = EQ: 2, UV: 0 */
          ,6,'e','q','u','a','l','s'
/* count=  2 Group:offset = CO: 2, EQ: 4 */
          ,13,'e','q','u','i','v','a','l','e','n'
              ,'c','i','n','g'
/* count=  2 Group:offset = EC: 0, EV: 4 */
          ,11,'e','s','t','a','b','l','i','s','h'
              ,'e','d'
/* count=  2 Group:offset = TY: 4, MS:143 */
          ,8,'e','x','e','c','u','t','e','d'
/* count=  2 Group:offset = ST: 7, ST:17 */
          ,9,'e','x','h','a','u','s','t','e','d'
/* count=  2 Group:offset = MO: 2, MO: 5 */
          ,5,'e','x','i','s','t'
/* count=  2 Group:offset = IO:23, SS: 2 */
          ,6,'e','x','t','e','n','d'
/* count=  2 Group:offset = MS:14, MS:209 */
          ,9,'e','x','t','e','n','s','i','o','n'
/* count=  2 Group:offset = EC: 0, MS: 7 */
          ,5,'f','i','r','s','t'
/* count=  2 Group:offset = ST: 1, MS:114 */
          ,6,'f','o','l','l','o','w'
/* count=  2 Group:offset = SP: 1, SP:11 */
          ,5,'g','i','v','e','n'
/* count=  2 Group:offset = IO:20, ST:21 */
          ,5,'g','r','o','u','p'
/* count=  2 Group:offset = VA: 5, MS:97 */
          ,17,'h','e','x','a','d','e','c','i','m'
              ,'a','l','/','o','c','t','a','l'
/* count=  2 Group:offset = CN: 3, CN: 4 */
          ,9,'h','o','l','l','e','r','i','t','h'
/* count=  2 Group:offset = HO: 0, HO: 1 */
          ,14,'i','d','e','n','t','i','f','i','c'
              ,'a','t','i','o','n'
/* count=  2 Group:offset = IL: 9, IL:10 */
          ,10,'i','d','e','n','t','i','f','i','e'
              ,'r'
/* count=  2 Group:offset = IL:11, SV: 7 */
          ,11,'i','m','m','e','d','i','a','t','e'
              ,'l','y'
/* count=  2 Group:offset = SP: 1, SP: 5 */
          ,8,'i','m','p','r','o','p','e','r'
/* count=  2 Group:offset = DO: 2, IL: 9 */
          ,9,'i','n','c','o','r','r','e','c','t'
/* count=  2 Group:offset = EV: 3, SF: 5 */
          ,10,'i','n','i','t','i','a','l','i','z'
              ,'e'
/* count=  2 Group:offset = DA: 5, DA:12 */
          ,4,'l','a','t','e'
/* count=  2 Group:offset = ST: 6, ST:10 */
          ,4,'l','e','f','t'
/* count=  2 Group:offset = CV: 2, EC: 0 */
          ,14,'l','i','b','r','a','r','y',' ','m'
              ,'e','m','b','e','r'
/* count=  2 Group:offset = MS:115, MS:133 */
          ,13,'l','i','s','t','-','d','i','r','e'
              ,'c','t','e','d'
/* count=  2 Group:offset = IL: 7, IL: 8 */
          ,6,'l','o','n','g','e','r'
/* count=  2 Group:offset = ST:19, VA: 1 */
          ,4,'m','a','n','y'
/* count=  2 Group:offset = CC: 8, DA:10 */
          ,7,'m','a','x','i','m','u','m'
/* count=  2 Group:offset = CO: 9, KO: 6 */
          ,9,'m','i','s','p','l','a','c','e','d'
/* count=  2 Group:offset = PC: 0, PC: 1 */
          ,8,'m','u','l','t','i','p','l','e'
/* count=  2 Group:offset = EQ: 3, SP:22 */
          ,7,'n','e','s','t','i','n','g'
/* count=  2 Group:offset = CO: 6, DO: 2 */
          ,13,'n','o','n','-','c','h','a','r','a'
              ,'c','t','e','r'
/* count=  2 Group:offset = CM: 2, IL:17 */
          ,6,'o','b','j','e','c','t'
/* count=  2 Group:offset = MO: 5, MS:20 */
          ,7,'o','p','e','n','i','n','g'
/* count=  2 Group:offset = PC: 1, SM: 1 */
          ,7,'o','p','e','r','a','n','d'
/* count=  2 Group:offset = MD: 0, UV: 2 */
          ,9,'o','p','e','r','a','t','i','o','n'
/* count=  2 Group:offset = IO:29, IO:35 */
          ,5,'o','t','h','e','r'
/* count=  2 Group:offset = EC: 1, IM: 4 */
          ,6,'o','u','t','p','u','t'
/* count=  2 Group:offset = KO: 6, MS:24 */
          ,7,'o','u','t','s','i','d','e'
/* count=  2 Group:offset = CM: 1, SP: 8 */
          ,9,'o','v','e','r','f','l','o','w','s'
/* count=  2 Group:offset = MS:33, MS:34 */
          ,4,'p','a','g','e'
/* count=  2 Group:offset = MS:29, MS:32 */
          ,4,'p','a','s','s'
/* count=  2 Group:offset = LI: 9, SF: 3 */
          ,3,'p','e','r'
/* count=  2 Group:offset = SV: 8, MS:32 */
          ,9,'p','r','e','c','e','d','i','n','g'
/* count=  2 Group:offset = ST: 7, ST:17 */
          ,5,'p','r','e','s','s'
/* count=  2 Group:offset = MS:119 */
          ,5,'q','u','o','t','e'
/* count=  2 Group:offset = FM:12, SX:20 */
          ,18,'r','e','c','u','r','s','i','o','n'
              ,' ','d','e','t','e','c','t','e','d'
/* count=  2 Group:offset = SP:26, SR: 8 */
          ,10,'r','e','l','a','t','i','o','n','a'
              ,'l'
/* count=  2 Group:offset = MD: 0, UV: 2 */
          ,5,'r','e','l','a','x'
/* count=  2 Group:offset = MS:18, MS:19 */
          ,8,'r','e','q','u','i','r','e','d'
/* count=  2 Group:offset = IO:17, MS:13 */
          ,4,'s','a','m','e'
/* count=  2 Group:offset = LI: 6, SR: 9 */
          ,5,'s','a','v','e','d'
/* count=  2 Group:offset = SA: 0, SA: 1 */
          ,9,'s','e','c','o','n','d','s',')',':'
/* count=  2 Group:offset = MS:124, MS:203 */
          ,12,'s','u','b','s','t','r','i','n','g'
              ,'i','n','g'
/* count=  2 Group:offset = SS: 0, SS: 1 */
          ,6,'s','y','m','b','o','l'
/* count=  2 Group:offset = IO:30, VA: 4 */
          ,8,'t','e','r','m','i','n','a','l'
/* count=  2 Group:offset = MS: 4, MS:24 */
          ,5,'t','y','p','e','s'
/* count=  2 Group:offset = LI: 6, MD: 1 */
          ,6,'u','n','a','b','l','e'
/* count=  2 Group:offset = MS:120, MS:148 */
          ,5,'u','n','i','t','s'
/* count=  2 Group:offset = SR: 9, MS:153 */
          ,12,'u','n','r','e','f','e','r','e','n'
              ,'c','e','d'
/* count=  2 Group:offset = VA: 4, MS: 8 */
          ,7,'w','a','r','n','i','n','g'
/* count=  2 Group:offset = MS: 8, MS: 9 */
          ,5,'w','h','i','l','e'
/* count=  2 Group:offset = DO: 6, SM: 4 */
          ,10,'w','i','l','l',' ','n','e','v','e'
              ,'r'
/* count=  2 Group:offset = ST: 7, ST:17 */
          ,7,'w','r','i','t','i','n','g'
/* count=  2 Group:offset = SM: 2, MS:130 */
          ,2,'{','-'
/* count=  2 Group:offset = MS: 1 */
          ,42,'!',' ','[','<','c','m','d','>',']'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','s','y'
              ,'s','t','e','m',' ','s','e','r','v','i'
              ,'c','e','s'
/* count=  1 Group:offset = MS:158 */
          ,1,'$'
/* count=  1 Group:offset = FM: 7 */
          ,4,'%','d','1',','
/* count=  1 Group:offset = MS:116 */
          ,4,'%','d','2',','
/* count=  1 Group:offset = AR: 6 */
          ,6,'\'','%','s','1','=','\''
/* count=  1 Group:offset = IO:27 */
          ,11,'\'','F','O','R','M','A','T','T','E'
              ,'D','\''
/* count=  1 Group:offset = IO: 7 */
          ,9,'(','%','i','1',':','%','i','2',')'
/* count=  1 Group:offset = SS: 3 */
          ,8,'(','0',',','0',')','*','*','Y'
/* count=  1 Group:offset = EX: 2 */
          ,1,'*'
/* count=  1 Group:offset = MS:25 */
          ,18,'*','A','p','p','l','i','c','a','t'
              ,'i','o','n',' ','T','y','p','e','*'
/* count=  1 Group:offset = MS:199 */
          ,14,'*','C','h','a','r','a','c','t','e'
              ,'r',' ','S','e','t'
/* count=  1 Group:offset = MS:194 */
          ,10,'*','D','e','b','u','g','g','i','n'
              ,'g'
/* count=  1 Group:offset = MS:163 */
          ,11,'*','D','i','a','g','n','o','s','t'
              ,'i','c'
/* count=  1 Group:offset = MS:162 */
          ,22,'*','E','R','R','*',' ','m','a','t'
              ,'h',' ','c','o','p','r','o','c','e','s'
              ,'s','o','r'
/* count=  1 Group:offset = MS:136 */
          ,16,'*','F','i','l','e',' ','M','a','n'
              ,'a','g','e','m','e','n','t'
/* count=  1 Group:offset = MS:161 */
          ,14,'*','M','i','s','c','e','l','l','a'
              ,'n','e','o','u','s'
/* count=  1 Group:offset = MS:164 */
          ,4,'.','E','Q','.'
/* count=  1 Group:offset = MD: 4 */
          ,4,'.','N','E','.'
/* count=  1 Group:offset = MD: 4 */
          ,1,'/'
/* count=  1 Group:offset = MS:159 */
          ,3,'1','-','5'
/* count=  1 Group:offset = CC: 4 */
          ,2,'1','9'
/* count=  1 Group:offset = CC: 5 */
          ,3,'2','/','1'
/* count=  1 Group:offset = MS:10 */
          ,3,'2','5','5'
/* count=  1 Group:offset = LI: 4 */
          ,3,'2','5','6'
/* count=  1 Group:offset = FM:13 */
          ,1,'4'
/* count=  1 Group:offset = IL: 3 */
          ,8,'5',' ','d','i','g','i','t','s'
/* count=  1 Group:offset = ST:19 */
          ,5,'6','5','5','3','5'
/* count=  1 Group:offset = SV: 8 */
          ,5,'9','9','9','9','9'
/* count=  1 Group:offset = ST:12 */
          ,6,'<',' ','0','.','0',','
/* count=  1 Group:offset = EX: 1 */
          ,11,'<','f','i','l','e','-','s','p','e'
              ,'c','>'
/* count=  1 Group:offset = MS: 1 */
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
          ,9,'A','L','L','O','C','A','T','E','D'
/* count=  1 Group:offset = LI:16 */
          ,9,'B','A','C','K','S','P','A','C','E'
/* count=  1 Group:offset = IO: 0 */
          ,5,'B','L','A','N','K'
/* count=  1 Group:offset = IO: 7 */
          ,9,'B','L','O','C','K','S','I','Z','E'
/* count=  1 Group:offset = IO:34 */
          ,10,'B','r','e','a','k','p','o','i','n'
              ,'t'
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
          ,7,'C','h','i','n','e','s','e'
/* count=  1 Group:offset = MS:195 */
          ,4,'C','o','d','e'
/* count=  1 Group:offset = MS:122 */
          ,24,'C','o','d','e','v','i','e','w',' '
              ,'d','e','b','u','g','g','i','n','g',' '
              ,'i','n','f','o','.'
/* count=  1 Group:offset = MS:215 */
          ,7,'C','o','m','p','i','l','e'
/* count=  1 Group:offset = MS:124 */
          ,6,'C','t','r','l','/','Z'
/* count=  1 Group:offset = MS:119 */
          ,1,'D'
/* count=  1 Group:offset = CC: 7 */
          ,7,'D','O','-','l','o','o','p'
/* count=  1 Group:offset = DO: 7 */
          ,15,'D','O','-','v','a','r','i','a','b'
              ,'l','e','s',' ','d','o'
/* count=  1 Group:offset = MS:218 */
          ,8,'D','e','b','u','g','g','e','r'
/* count=  1 Group:offset = MS:138 */
          ,25,'D','e','f','i','n','i','n','g',' '
              ,'s','u','b','p','r','o','g','r','a','m'
              ,':',' ','%','s','1',';'
/* count=  1 Group:offset = MS:114 */
          ,34,'D','i','s','p','l','a','y',' ','['
              ,'*','|','<','i','s','n','>','[',':','<'
              ,'i','s','n','>',']',']','.','.','d','i'
              ,'s','p','l','a','y'
/* count=  1 Group:offset = MS:150 */
          ,18,'D','o',' ','y','o','u',' ','r','e'
              ,'a','l','l','y',' ','w','a','n','t'
/* count=  1 Group:offset = MS:135 */
          ,4,'E','L','S','E'
/* count=  1 Group:offset = IF: 0 */
          ,4,'E','N','D','='
/* count=  1 Group:offset = SP: 0 */
          ,5,'E','N','D','D','O'
/* count=  1 Group:offset = DO: 3 */
          ,7,'E','N','D','F','I','L','E'
/* count=  1 Group:offset = IO: 2 */
          ,7,'E','X','E','C','U','T','E'
/* count=  1 Group:offset = SP:13 */
          ,5,'E','n','t','e','r'
/* count=  1 Group:offset = MS:119 */
          ,6,'E','w','.','d','D','e'
/* count=  1 Group:offset = FM: 2 */
          ,6,'E','w','.','d','Q','e'
/* count=  1 Group:offset = FM:20 */
          ,9,'E','x','e','c','u','t','i','o','n'
/* count=  1 Group:offset = MS:203 */
          ,10,'E','x','i','t','[','E','n','t','e'
              ,'r'
/* count=  1 Group:offset = MS:141 */
          ,2,'F','F'
/* count=  1 Group:offset = MS:212 */
          ,4,'F','I','L','E'
/* count=  1 Group:offset = IL:12 */
          ,4,'F','O','R','M'
/* count=  1 Group:offset = IO: 7 */
          ,8,'F','U','N','C','T','I','O','N'
/* count=  1 Group:offset = SR: 6 */
          ,6,'G','l','o','b','a','l'
/* count=  1 Group:offset = MS:205 */
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
          ,2,'I','F'
/* count=  1 Group:offset = IF: 0 */
          ,7,'I','N','T','E','G','E','R'
/* count=  1 Group:offset = SX:18 */
          ,15,'I','N','T','E','G','E','R','/','L'
              ,'O','G','I','C','A','L'
/* count=  1 Group:offset = MS:10 */
          ,7,'I','S','I','Z','E','O','F'
/* count=  1 Group:offset = LI:17 */
          ,6,'I','n','t','o','[','I'
/* count=  1 Group:offset = MS:141 */
          ,6,'J',' ','<','=',' ','0'
/* count=  1 Group:offset = EX: 0 */
          ,8,'J','a','p','a','n','e','s','e'
/* count=  1 Group:offset = MS:196 */
          ,6,'K','o','r','e','a','n'
/* count=  1 Group:offset = MS:197 */
          ,2,'L','F'
/* count=  1 Group:offset = MS:212 */
          ,5,'L','o','c','a','l'
/* count=  1 Group:offset = MS:204 */
          ,38,'L','o','g','i','o',' ','[','<','u'
              ,'n','i','t','>',']','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','l','i'
              ,'s','t',' ','l','o','g','g','e','d'
/* count=  1 Group:offset = MS:153 */
          ,9,'N','O',' ','p','r','e','f','i','x'
/* count=  1 Group:offset = CO: 1 */
          ,4,'N','O','N','E'
/* count=  1 Group:offset = IM: 4 */
          ,6,'O','B','J','E','C','T'
/* count=  1 Group:offset = CO:10 */
          ,6,'O','b','j','e','c','t'
/* count=  1 Group:offset = MS:206 */
          ,22,'O','v','e','r','[','s','p','a','c'
              ,'e',' ','b','a','r',']',' ','N','e','x'
              ,'t','[','N'
/* count=  1 Group:offset = MS:141 */
          ,31,'Q','u','i','t','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','q','u'
              ,'i','t'
/* count=  1 Group:offset = MS:154 */
          ,4,'Q','w','.','d'
/* count=  1 Group:offset = FM:21 */
          ,4,'R','E','A','D'
/* count=  1 Group:offset = SP: 1 */
          ,6,'R','E','W','I','N','D'
/* count=  1 Group:offset = IO: 5 */
          ,8,'R','e','t','u','r','n','e','d'
/* count=  1 Group:offset = MS:140 */
          ,6,'S','E','L','E','C','T'
/* count=  1 Group:offset = SP: 5 */
          ,4,'T','H','E','N'
/* count=  1 Group:offset = IF: 1 */
          ,2,'T','O'
/* count=  1 Group:offset = GO: 2 */
          ,8,'T','o',' ','e','x','i','t',','
/* count=  1 Group:offset = MS:119 */
          ,38,'T','r','a','c','e','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','e','n'
              ,'t','e','r',' ','t','r','a','c','e'
/* count=  1 Group:offset = MS:155 */
          ,6,'U','n','a','b','l','e'
/* count=  1 Group:offset = MS:118 */
          ,31,'U','n','i','t',' ','[','<','u','n'
              ,'i','t','>',']','.','.','.','.','.','.'
              ,'.','.','.','.','.','.','.','.','l','i'
              ,'s','t'
/* count=  1 Group:offset = MS:156 */
          ,6,'U','s','a','g','e',':'
/* count=  1 Group:offset = MS: 1 */
          ,5,'V','A','L','U','E'
/* count=  1 Group:offset = PR: 1 */
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
          ,18,'a','d','j','u','s','t','a','b','l'
              ,'e','/','a','s','s','u','m','e','d'
/* count=  1 Group:offset = SV: 1 */
          ,3,'a','l','l'
/* count=  1 Group:offset = SP:11 */
          ,8,'a','l','l','o','c','a','t','e'
/* count=  1 Group:offset = MO: 4 */
          ,16,'a','l','l','o','t','t','e','d',' '
              ,'s','t','o','r','a','g','e'
/* count=  1 Group:offset = FM:16 */
          ,9,'a','l','t','e','r','n','a','t','e'
/* count=  1 Group:offset = RE: 0 */
          ,3,'a','n','y'
/* count=  1 Group:offset = SP: 4 */
          ,10,'a','p','o','s','t','r','o','p','h'
              ,'e'
/* count=  1 Group:offset = FM:12 */
          ,8,'a','p','p','e','a','r','e','d'
/* count=  1 Group:offset = SA: 2 */
          ,18,'a','r','c','s','i','n','e',',',' '
              ,'a','r','c','c','o','s','i','n','e'
/* count=  1 Group:offset = LI: 1 */
          ,13,'a','r','i','t','h','m','e','t','i'
              ,'c',' ','i','f'
/* count=  1 Group:offset = MS:112 */
          ,6,'a','r','r','a','y','s'
/* count=  1 Group:offset = SP:28 */
          ,6,'a','s','s','i','g','n'
/* count=  1 Group:offset = EQ: 1 */
          ,14,'a','s','s','i','g','n','e','d',' '
              ,'w','r','o','n','g'
/* count=  1 Group:offset = LI:14 */
          ,9,'a','t','t','e','m','p','t','e','d'
/* count=  1 Group:offset = SP:14 */
          ,10,'a','t','t','r','i','b','u','t','e'
              ,'s'
/* count=  1 Group:offset = IO:29 */
          ,9,'b','a','c','k','w','a','r','d','s'
/* count=  1 Group:offset = DO: 0 */
          ,14,'b','e','i','n','g',' ','i','n','c'
              ,'l','u','d','e','d'
/* count=  1 Group:offset = CO: 0 */
          ,7,'b','e','l','o','n','g','s'
/* count=  1 Group:offset = VA: 5 */
          ,6,'b','i','g','g','e','r'
/* count=  1 Group:offset = AR: 2 */
          ,8,'b','r','a','n','c','h','e','d'
/* count=  1 Group:offset = ST: 2 */
          ,6,'b','r','o','w','s','e'
/* count=  1 Group:offset = MS:208 */
          ,8,'b','r','o','w','s','i','n','g'
/* count=  1 Group:offset = SM: 6 */
          ,12,'c','a','l','l','e','d',' ','s','i'
              ,'n','c','e'
/* count=  1 Group:offset = SR: 1 */
          ,8,'c','a','r','r','i','a','g','e'
/* count=  1 Group:offset = MS:201 */
          ,5,'c','a','s','e','s'
/* count=  1 Group:offset = SP: 6 */
          ,6,'c','a','u','s','e','d'
/* count=  1 Group:offset = EC: 0 */
          ,6,'c','h','a','n','g','e'
/* count=  1 Group:offset = IO:19 */
          ,7,'c','o','l','u','m','n',','
/* count=  1 Group:offset = CC: 2 */
          ,8,'c','o','m','p','a','r','e','d'
/* count=  1 Group:offset = MD: 4 */
          ,11,'c','o','n','d','i','t','i','o','n'
              ,'a','l'
/* count=  1 Group:offset = SP:29 */
          ,10,'c','o','n','s','i','s','t','e','n'
              ,'t'
/* count=  1 Group:offset = IO:29 */
          ,16,'c','o','n','t','a','i','n','i','n'
              ,'g',' ','u','n','i','o','n'
/* count=  1 Group:offset = SP:27 */
          ,8,'c','o','n','t','i','n','u','e'
/* count=  1 Group:offset = CC: 2 */
          ,10,'d','e','a','l','l','o','c','a','t'
              ,'e'
/* count=  1 Group:offset = MO: 3 */
          ,13,'d','e','c','i','m','a','l',' ','p'
              ,'o','i','n','t'
/* count=  1 Group:offset = FM: 3 */
          ,7,'d','e','f','a','u','l','t'
/* count=  1 Group:offset = SP:11 */
          ,12,'d','e','f','i','n','e',' ','m','a'
              ,'c','r','o'
/* count=  1 Group:offset = MS:17 */
          ,9,'d','e','l','i','m','i','t','e','r'
/* count=  1 Group:offset = FM: 0 */
          ,11,'d','e','s','c','r','i','p','t','o'
              ,'r',','
/* count=  1 Group:offset = FM:19 */
          ,7,'d','e','v','i','c','e','s'
/* count=  1 Group:offset = MS:201 */
          ,3,'d','i','d'
/* count=  1 Group:offset = GO: 0 */
          ,10,'d','i','m','e','n','s','i','o','n'
              ,'s'
/* count=  1 Group:offset = MS:143 */
          ,6,'d','i','r','e','c','t'
/* count=  1 Group:offset = IO:17 */
          ,4,'d','i','s','k'
/* count=  1 Group:offset = MS: 5 */
          ,18,'d','i','s','p','l','a','y',' ','d'
              ,'i','a','g','n','o','s','t','i','c'
/* count=  1 Group:offset = MS:12 */
          ,9,'d','u','p','l','i','c','a','t','e'
/* count=  1 Group:offset = SF: 0 */
          ,22,'d','y','n','a','m','i','c','a','l'
              ,'l','y',' ','a','l','l','o','c','a','t'
              ,'i','n','g'
/* count=  1 Group:offset = VA: 7 */
          ,4,'e','a','c','h'
/* count=  1 Group:offset = EC: 1 */
          ,39,'e','l','e','m','e','n','t',' ','('
              ,'p','o','s','s','i','b','l','y',' ','s'
              ,'u','b','s','t','r','u','n','g',')',' '
              ,'a','s','s','o','c','i','a','t','e','d'
/* count=  1 Group:offset = DM: 1 */
          ,11,'e','n','d','-','o','f','-','l','i'
              ,'n','e'
/* count=  1 Group:offset = CC: 6 */
          ,6,'e','n','t','i','t','y'
/* count=  1 Group:offset = DA:13 */
          ,6,'e','r','r','o','r','s'
/* count=  1 Group:offset = MS:121 */
          ,7,'e','r','r','o','r','s',':'
/* count=  1 Group:offset = MS:123 */
          ,5,'e','v','e','r','y'
/* count=  1 Group:offset = SA: 1 */
          ,6,'e','x','c','e','e','d'
/* count=  1 Group:offset = LI: 1 */
          ,15,'e','x','e','c','u','t','e',' ','c'
              ,'o','m','m','a','n','d'
/* count=  1 Group:offset = MS:120 */
          ,9,'e','x','e','c','u','t','e','d',':'
/* count=  1 Group:offset = MS:207 */
          ,3,'e','x','p'
/* count=  1 Group:offset = AR: 9 */
          ,15,'e','x','p','l','i','c','i','t',' '
              ,'t','y','p','i','n','g'
/* count=  1 Group:offset = MS:13 */
          ,25,'e','x','t','e','n','s','i','o','n'
              ,'s',',',' ','%','d','4',' ','w','a','r'
              ,'n','i','n','g','s',','
/* count=  1 Group:offset = MS:121 */
          ,11,'e','x','t','e','n','s','i','o','n'
              ,'s',':'
/* count=  1 Group:offset = MS:127 */
          ,8,'e','x','t','e','r','n','a','l'
/* count=  1 Group:offset = MS:110 */
          ,6,'f','a','i','l','e','d'
/* count=  1 Group:offset = SM: 6 */
          ,6,'f','i','e','l','d','s'
/* count=  1 Group:offset = SP:28 */
          ,4,'f','i','n','d'
/* count=  1 Group:offset = SP: 3 */
          ,8,'f','o','l','l','o','w','e','d'
/* count=  1 Group:offset = SP: 5 */
          ,9,'f','o','l','l','o','w','i','n','g'
/* count=  1 Group:offset = CO: 2 */
          ,4,'f','o','r','m'
/* count=  1 Group:offset = DO: 4 */
          ,10,'f','o','r','m','a','t','/','F','M'
              ,'T'
/* count=  1 Group:offset = IL: 0 */
          ,11,'f','o','r','m','a','t','/','u','n'
              ,'i','t'
/* count=  1 Group:offset = SV: 7 */
          ,10,'f','o','r','m','a','t','t','i','n'
              ,'g'
/* count=  1 Group:offset = IL: 8 */
          ,5,'f','r','e','e',':'
/* count=  1 Group:offset = MS:206 */
          ,5,'f','r','e','e','d'
/* count=  1 Group:offset = CP: 8 */
          ,15,'f','r','e','e','i','n','g',' ','u'
              ,'n','o','w','n','e','d'
/* count=  1 Group:offset = CP: 9 */
          ,4,'f','u','l','l'
/* count=  1 Group:offset = IO:11 */
          ,19,'f','u','n','c','t','i','o','n','/'
              ,'s','u','b','r','o','u','t','i','n','e'
/* count=  1 Group:offset = SR:12 */
          ,10,'g','e','n','e','r','a','t','i','o'
              ,'n'
/* count=  1 Group:offset = SM: 6 */
          ,7,'g','e','n','e','r','i','c'
/* count=  1 Group:offset = LI:11 */
          ,18,'h','a','n','d',' ','s','i','d','e'
              ,'s',' ','o','v','e','r','l','a','p'
/* count=  1 Group:offset = CV: 2 */
          ,11,'h','e','l','p',' ','s','c','r','e'
              ,'e','n'
/* count=  1 Group:offset = MS:152 */
          ,11,'h','e','x','a','d','e','c','i','m'
              ,'a','l'
/* count=  1 Group:offset = DA: 4 */
          ,4,'h','i','g','h'
/* count=  1 Group:offset = SP:10 */
          ,10,'i','m','p','l','i','c','i','t','l'
              ,'y'
/* count=  1 Group:offset = IM: 1 */
          ,10,'i','n','c','o','m','p','l','e','t'
              ,'e'
/* count=  1 Group:offset = SP:19 */
          ,24,'i','n','c','r','e','m','e','n','t'
              ,'a','t','i','o','n',' ','p','a','r','a'
              ,'m','e','t','e','r'
/* count=  1 Group:offset = DO: 7 */
          ,12,'i','n','i','t','i','a','l','i','z'
              ,'i','n','g'
/* count=  1 Group:offset = CM: 1 */
          ,6,'i','n','s','i','d','e'
/* count=  1 Group:offset = SP: 9 */
          ,9,'i','n','t','e','r','f','a','c','e'
/* count=  1 Group:offset = MS:21 */
          ,11,'i','n','t','e','r','r','u','p','t'
              ,'e','d'
/* count=  1 Group:offset = KO: 4 */
          ,4,'i','n','t','o'
/* count=  1 Group:offset = ST:23 */
          ,6,'i','t','s','e','l','f'
/* count=  1 Group:offset = ST:14 */
          ,8,'k','e','y','b','o','a','r','d'
/* count=  1 Group:offset = KO: 4 */
          ,7,'k','e','y','w','o','r','d'
/* count=  1 Group:offset = ST:18 */
          ,5,'l','a','r','g','e'
/* count=  1 Group:offset = IO:26 */
          ,4,'l','a','s','t'
/* count=  1 Group:offset = IF: 0 */
          ,4,'l','e','s','s'
/* count=  1 Group:offset = ST:12 */
          ,6,'l','e','t','t','e','r'
/* count=  1 Group:offset = IM: 1 */
          ,5,'l','e','v','e','l'
/* count=  1 Group:offset = CO: 6 */
          ,7,'l','i','n','k','i','n','g'
/* count=  1 Group:offset = SM: 4 */
          ,7,'l','i','t','e','r','a','l'
/* count=  1 Group:offset = SX:20 */
          ,3,'l','o','g'
/* count=  1 Group:offset = MS:25 */
          ,3,'l','o','w'
/* count=  1 Group:offset = SP:10 */
          ,11,'m','i','s','m','a','t','c','h','i'
              ,'n','g'
/* count=  1 Group:offset = CO: 7 */
          ,15,'m','i','s','s','p','e','l','l','e'
              ,'d',' ','w','o','r','d'
/* count=  1 Group:offset = ST:16 */
          ,6,'m','i','x','i','n','g'
/* count=  1 Group:offset = MD: 1 */
          ,8,'m','o','d','i','f','i','e','r'
/* count=  1 Group:offset = FM: 8 */
          ,5,'n','a','m','e','d'
/* count=  1 Group:offset = CM: 4 */
          ,5,'n','a','m','e','s'
/* count=  1 Group:offset = EV: 1 */
          ,10,'n','e','a','r',' ','\'','%','s','2'
              ,'\''
/* count=  1 Group:offset = PR: 4 */
          ,8,'n','e','g','a','t','i','v','e'
/* count=  1 Group:offset = LI: 2 */
          ,13,'n','o','n','-','C','H','A','R','A'
              ,'C','T','E','R'
/* count=  1 Group:offset = EV: 4 */
          ,7,'n','u','m','e','r','i','c'
/* count=  1 Group:offset = LI: 7 */
          ,4,'o','p','e','n'
/* count=  1 Group:offset = MS:148 */
          ,15,'o','p','e','r','a','t','e',' ','q'
              ,'u','i','e','t','l','y'
/* count=  1 Group:offset = MS:202 */
          ,9,'o','p','e','r','a','t','o','r','s'
/* count=  1 Group:offset = SX: 1 */
          ,11,'o','v','e','r','l','a','p','p','i'
              ,'n','g'
/* count=  1 Group:offset = SP: 6 */
          ,5,'p','a','g','e','s'
/* count=  1 Group:offset = KO: 6 */
          ,4,'p','a','t','h'
/* count=  1 Group:offset = MS:219 */
          ,7,'p','e','r','f','o','r','m'
/* count=  1 Group:offset = IO:15 */
          ,9,'p','e','r','m','i','t','t','e','d'
/* count=  1 Group:offset = BD: 1 */
          ,8,'p','o','s','i','t','i','o','n'
/* count=  1 Group:offset = ST:20 */
          ,8,'p','o','s','i','t','i','v','e'
/* count=  1 Group:offset = DA: 1 */
          ,9,'p','r','e','c','i','s','i','o','n'
/* count=  1 Group:offset = MS:209 */
          ,11,'p','r','e','s','e','n','t','\\','r'
              ,'\\','n'
/* count=  1 Group:offset = MS:136 */
          ,13,'p','r','e','v','i','o','u','s',' '
              ,'S','A','V','E'
/* count=  1 Group:offset = SA: 2 */
          ,7,'p','r','i','n','t','e','r'
/* count=  1 Group:offset = MS: 3 */
          ,8,'p','r','o','p','e','r','l','y'
/* count=  1 Group:offset = SA: 0 */
          ,10,'q','u','a','n','t','i','t','i','e'
              ,'s'
/* count=  1 Group:offset = MD: 4 */
          ,6,'r','a','n','g','e',';'
/* count=  1 Group:offset = SS: 2 */
          ,7,'r','e','a','d','i','n','g'
/* count=  1 Group:offset = SM: 0 */
          ,4,'r','e','a','l'
/* count=  1 Group:offset = EX: 2 */
          ,10,'r','e','c','o','g','n','i','z','e'
              ,'d'
/* count=  1 Group:offset = CO: 4 */
          ,11,'r','e','c','u','r','s','i','v','e'
              ,'l','y'
/* count=  1 Group:offset = SP:14 */
          ,9,'r','e','d','e','f','i','n','e','d'
/* count=  1 Group:offset = DO: 6 */
          ,9,'r','e','f','e','r','e','n','c','e'
/* count=  1 Group:offset = AR:11 */
          ,18,'r','e','l','a','t','i','v','e',' '
              ,'p','o','s','i','t','i','o','n','s'
/* count=  1 Group:offset = EV: 0 */
          ,10,'r','e','p','e','a','t','a','b','l'
              ,'e'
/* count=  1 Group:offset = FM:18 */
          ,7,'r','e','q','u','i','r','e'
/* count=  1 Group:offset = IL:13 */
          ,8,'r','e','s','o','u','r','c','e'
/* count=  1 Group:offset = MS:200 */
          ,3,'r','u','n'
/* count=  1 Group:offset = MS:23 */
          ,8,'r','u','n','-','t','i','m','e'
/* count=  1 Group:offset = MS:15 */
          ,6,'s','e','l','e','c','t'
/* count=  1 Group:offset = SP: 7 */
          ,8,'s','e','q','u','e','n','c','e'
/* count=  1 Group:offset = SX: 1 */
          ,4,'s','i','d','e'
/* count=  1 Group:offset = UV: 0 */
          ,6,'s','i','m','p','l','e'
/* count=  1 Group:offset = MS:100 */
          ,5,'s','l','a','s','h'
/* count=  1 Group:offset = SX:14 */
          ,5,'s','m','a','l','l'
/* count=  1 Group:offset = DM: 1 */
          ,8,'s','p','a','w','n','i','n','g'
/* count=  1 Group:offset = SM: 3 */
          ,16,'s','p','e','c','i','f','i','e','r'
              ,'/','A','C','C','E','S','S'
/* count=  1 Group:offset = IO:16 */
          ,14,'s','p','e','c','i','f','i','e','r'
              ,'/','F','I','L','E'
/* count=  1 Group:offset = IO:21 */
          ,7,'s','p','e','c','i','f','y'
/* count=  1 Group:offset = IL: 5 */
          ,5,'s','t','a','r','t'
/* count=  1 Group:offset = MS:118 */
          ,10,'s','t','a','t','e','m','e','n','t'
              ,','
/* count=  1 Group:offset = MS:113 */
          ,22,'s','t','a','t','e','m','e','n','t'
              ,'s',',',' ','%','i','2',' ','b','y','t'
              ,'e','s',','
/* count=  1 Group:offset = MS:121 */
          ,11,'s','t','o','p',' ','(','y','/','n'
              ,')','?'
/* count=  1 Group:offset = MS:135 */
          ,6,'s','t','r','e','a','m'
/* count=  1 Group:offset = CO: 5 */
          ,7,'s','t','r','i','n','g',','
/* count=  1 Group:offset = FM: 0 */
          ,10,'s','t','r','u','c','t','u','r','e'
              ,'d'
/* count=  1 Group:offset = EY: 1 */
          ,10,'s','t','r','u','c','t','u','r','e'
              ,'s'
/* count=  1 Group:offset = SP:28 */
          ,11,'s','u','b','s','c','r','i','p','t'
              ,'e','d'
/* count=  1 Group:offset = EV: 2 */
          ,12,'s','u','b','s','c','r','i','p','t'
              ,'i','n','g'
/* count=  1 Group:offset = IO:32 */
          ,10,'s','u','b','s','c','r','i','p','t'
              ,'s'
/* count=  1 Group:offset = SV: 3 */
          ,15,'s','u','b','s','e','q','u','e','n'
              ,'t',' ','O','P','E','N'
/* count=  1 Group:offset = IO:19 */
          ,4,'s','u','c','h'
/* count=  1 Group:offset = SP:21 */
          ,6,'s','y','n','t','a','x'
/* count=  1 Group:offset = IO:31 */
          ,4,'t','a','k','e'
/* count=  1 Group:offset = CO: 1 */
          ,6,'t','a','r','g','e','t'
/* count=  1 Group:offset = EQ: 0 */
          ,9,'t','e','r','m','i','n','a','t','e'
/* count=  1 Group:offset = DO: 3 */
          ,11,'t','e','r','m','i','n','a','t','i'
              ,'o','n'
/* count=  1 Group:offset = DO: 1 */
          ,4,'t','e','x','t'
/* count=  1 Group:offset = FM: 9 */
          ,4,'t','h','e','n'
/* count=  1 Group:offset = MS:119 */
          ,14,'t','o','g','g','l','e',' ','l','o'
              ,'g','g','i','n','g'
/* count=  1 Group:offset = MS:153 */
          ,13,'t','r','a','n','s','f','e','r',' '
              ,'p','a','s','t'
/* count=  1 Group:offset = IO:15 */
          ,7,'t','r','e','a','t','e','d'
/* count=  1 Group:offset = SV: 6 */
          ,3,'t','w','o'
/* count=  1 Group:offset = SR: 9 */
          ,11,'u','n','a','l','l','o','c','a','t'
              ,'e','d'
/* count=  1 Group:offset = MO: 3 */
          ,5,'u','n','a','r','y'
/* count=  1 Group:offset = MD: 5 */
          ,12,'u','n','d','e','c','o','d','e','a'
              ,'b','l','e'
/* count=  1 Group:offset = ST:16 */
          ,9,'u','n','d','e','r','f','l','o','w'
/* count=  1 Group:offset = KO: 2 */
          ,10,'u','n','d','e','r','f','l','o','w'
              ,'s'
/* count=  1 Group:offset = MS:35 */
          ,10,'u','n','d','e','r','s','t','o','o'
              ,'d'
/* count=  1 Group:offset = MS:144 */
          ,10,'u','n','f','i','n','i','s','h','e'
              ,'d'
/* count=  1 Group:offset = SP:17 */
          ,7,'u','n','i','t','(','s',')'
/* count=  1 Group:offset = MS:156 */
          ,21,'u','n','m','a','t','c','h','e','d'
              ,' ','p','a','r','e','n','t','h','e','s'
              ,'e','s'
/* count=  1 Group:offset = PC: 3 */
          ,7,'u','n','n','a','m','e','d'
/* count=  1 Group:offset = SR:10 */
          ,12,'u','s','e','r','-','d','e','f','i'
              ,'n','e','d'
/* count=  1 Group:offset = SF: 6 */
          ,5,'v','a','l','i','d'
/* count=  1 Group:offset = SS: 1 */
          ,9,'w','a','r','n','i','n','g','s',':'
/* count=  1 Group:offset = MS:125 */
          ,8,'w','h','i','c','h',' ','i','t'
/* count=  1 Group:offset = SA: 1 */
          ,5,'w','i','d','t','h'
/* count=  1 Group:offset = FM:14 */
          ,4,'w','i','l','d'
/* count=  1 Group:offset = MS:19 */
          ,7,'w','i','t','h','o','u','t'
/* count=  1 Group:offset = SV: 4 */
          ,3,'y','e','t'
/* count=  1 Group:offset = MS:143 */
          ,7,'z','e','r','o','*','*','J'
/* count=  1 Group:offset = EX: 0 */
          ,13,'{','<','p','r','o','g','-','p','a'
              ,'r','m','>','}'
/* count=  1 Group:offset = MS: 1 */
                   };
/* Total number of phrases = 651 */
