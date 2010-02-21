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
#  Description: This script is used to split an HTML file contained help
#               topics into separate files for each topic.
#
###########################################################################

$htmlfile = $ARGV[0];
$dir = $ARGV[1];
open( HTMLFILE, "<", $htmlfile ) or die( "Cannot open $htmlfile" );
$fileopen = 0;
while( $line = <HTMLFILE> ) {
    if( $line =~ /<H. ID=\"(.*)\"> (.*) <\/H.>/ ) {
	if( $fileopen ) {
	    print OUTFILE "</BODY>\n";
	    print OUTFILE "</HTML>\n";
	    close OUTFILE;
	}
	open( OUTFILE, ">", "$dir\\$1.htm" ) or die( "Cannot open $dir\\$1.htm" );
	$fileopen = 1;
	print OUTFILE "<HTML>\n";
	print OUTFILE "<HEAD>\n";
	print OUTFILE "<TITLE>$2</TITLE>\n";
	print OUTFILE "<BODY>\n";
	print OUTFILE "<H1>$2</H1>\n";
    } elsif( $fileopen ) {
	$line =~ s/HREF\=\"\#(.*)\"/HREF=\"\1.htm\"/g;
	print OUTFILE "$line\n";
    }
}
if( $fileopen ) {
    close OUTFILE;
}
close HTMLFILE;
