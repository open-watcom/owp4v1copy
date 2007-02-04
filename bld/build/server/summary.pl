#!/usr/bin/perl -w
############################################################################
#
#                            Open Watcom Project
#
#  Copyright (c) 2004-2006 The Open Watcom Contributors. All Rights Reserved.
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
#  Description: This program reads the builder log file and outputs a
#               summary of error and warning "records."
#
###########################################################################
#use strict;

use Common;

if( $#ARGV == 1 ) {
    Common::filename( "config.txt" );
} elsif( $#ARGV == 2 ) {
    Common::filename( $ARGV[2] );
} else {
    print "Usage: summary build_log summary_result [config_file]\n";
    exit 1;
}
open(INFILE, "$ARGV[0]") || die "Unable to open input file: $ARGV[0]";
open(OUTFILE, ">$ARGV[1]") || die "Unable to open output file: $ARGV[1]";

# Read the build log file a line at a time and output the error summary.
while (<INFILE>) {
    chomp;
    if (/^=====/) {
        @header = split;
        $current_project = $header[2];
        $source_location = $Common::config{"OW"};
        $source_location =~ s/\\/\\\\/g;
        $current_project =~ /$source_location\\(.*)/i;
        $current_project = $1;
    }
    if (/Warning|Error|Can not|ERROR|WARNING/) {
        print OUTFILE "\nPROJECT $current_project\n";
        print OUTFILE "$_\n";
    }
}

close(OUTFILE);
close(INFILE);
