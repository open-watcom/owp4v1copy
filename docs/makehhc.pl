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
#  Description: This script is used to generate an HTML Help contents file
#               from an HTML file containing help topics.
#
###########################################################################

$htmlfile = $ARGV[0];
$hhcfile = $ARGV[1];
open( HTMLFILE, "<", $htmlfile ) or die( "Cannot open $htmlfile" );
open( HHCFILE, ">", $hhcfile ) or die( "Cannot open $hhcfile" );
print HHCFILE "<HTML>\n";
print HHCFILE "<BODY>\n";
print HHCFILE "<OBJECT TYPE=\"text/site properties\">\n";
print HHCFILE "<PARAM NAME=\"ImageType\" VALUE=\"Folder\">\n";
print HHCFILE "</OBJECT>\n";
print HHCFILE "<UL>\n";

$level = 1;
while( $line = <HTMLFILE> ) {
    if( $line =~ /<H(.) ID=\"(.*)\"> (.*) <\/H.>/ ) {
        if( $1 < $level ) {
	    while( $level > $1 ) {
                print HHCFILE "</UL>\n";
                $level--;
            }
	} elsif( $1 > $level ) {
            print HHCFILE "<UL>\n";
            $level = $1;
        }
        print HHCFILE "<LI><OBJECT TYPE=\"text/sitemap\">\n";
        print HHCFILE "<PARAM NAME=\"Name\" VALUE=\"$3\">\n";
        print HHCFILE "<PARAM NAME=\"Local\" VALUE=\"$2.htm\">\n";
        print HHCFILE "</OBJECT>\n";
    }
}
while( $level > 0 ) {
    print HHCFILE "</UL>\n";
    $level--;
}
print HHCFILE "</BODY>\n";
print HHCFILE "</HTML>\n";
close HHCFILE;
close HTMLFILE;
