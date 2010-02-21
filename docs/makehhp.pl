#!/usr/bin/perl
############################################################################
#
#                            Open Watcom Project
#
#  Copyright (c) 2004-2010 The Open Watcom Contributors. All Rights Reserved.
#
#  ========================================================================
#
#    This file contains Original Code and/or Modifications of Original
#    Code as defined in and that are subject to the Sybase Open Watcom
#    Public License version 1.0 (the 'License'). You may not use this file
#    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
#    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
#    provided with the Original Code and Modifications, and is also
#    available at www.sybase.com/developer/opensource.
#
#    The Original Code and all software distributed under the License are
#    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
#    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
#    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
#    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
#    NON-INFRINGEMENT. Please see the License for the specific language
#    governing rights and limitations under the License.
#
#  ========================================================================
#
#  Description: This script is used to generate an HTML Help project file
#               from an HTML file containing help topics.
#
###########################################################################

$htmlfile = $ARGV[0];
$hhpfile = $ARGV[1];
open( HTMLFILE, "<", $htmlfile ) or die( "Cannot open $htmlfile" );
$fileopen = 0;
while( $line = <HTMLFILE> ) {
    if( $line =~ /<TITLE>(.*)<\/TITLE>/ ) {
        $title = $1;
        last;
    }
}

open( HHPFILE, ">", $hhpfile ) or die( "Cannot open $hhpfile" );
print HHPFILE "[OPTIONS]\n";
print HHPFILE "Compatibility=1.1 or later\n";
print HHPFILE "Create CHI file=Yes\n";
print HHPFILE "Default Window=Main\n";
print HHPFILE "Language=0x409 English (United States)\n";
print HHPFILE "Title=$title\n";
print HHPFILE "\n";
print HHPFILE "[WINDOWS]\n";
print HHPFILE "Main=\"$title\",,,,,,,,,0x2020,,0x300e,,,,,,,,0\n";
print HHPFILE "\n";
print HHPFILE "[FILES]\n";

while( $line = <HTMLFILE> ) {
    if( $line =~ /<H. ID=\"(.*)\"> (.*) <\/H.>/ ) {
	print HHPFILE "$1.htm\n";
    }
}
close HHPFILE;
close HTMLFILE;
