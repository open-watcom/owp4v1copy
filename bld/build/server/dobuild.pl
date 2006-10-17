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
#  Description: This file is the main program for the build server.
#
###########################################################################
#use strict;

use Common;

$home    = $Common::config{"HOME"};
$OW      = $Common::config{"OW"};
$WATCOM  = $Common::config{"WATCOM"};
$report_archive = $Common::config{"REPORTS"};
$bldbase = "$home\\$Common::config{'BLDBASE'}";
$bldlast = "$home\\$Common::config{'BLDLAST'}";

$batch_name = "$home\\buildtmp.bat";

sub make_batch()
{
    open(BATCH, ">$batch_name") || die "Unable to open temporary batch file.";
    open(INPUT, "$OW\\setvars.bat") || die "Unable to open setvars.bat";
    while (<INPUT>) {
        if    (/set OWROOT/i) { print BATCH "set OWROOT=" . $OW . "\n"; }
        elsif (/set WATCOM/i) { print BATCH "set WATCOM=" . $WATCOM . "\n"; }
        else                  { print BATCH; }
    }
    close(INPUT);
    
    # Add additional commands to do the build.
    print BATCH "\n";
    print BATCH "cd $OW\n";
    print BATCH "rm -rf rel2\n";
    print BATCH "cd $OW\\bld\n";
    print BATCH "builder clean\n";
    print BATCH "cd $OW\\bld\\builder\\nt386\n";
    print BATCH "wmake\n";
    print BATCH "cd $OW\\contrib\\wattcp\\src\n";
    print BATCH "wmake -ms\n";
    print BATCH "cd $OW\\bld\n";
    print BATCH "builder rel2\n";
    close(BATCH);
}

sub get_date
{
    my(@now) = gmtime time;
    return sprintf "%04d-%02d-%02d", $now[5] + 1900, $now[4] + 1, $now[3];
}

sub get_datetime
{
    my(@now) = gmtime time;
    return sprintf "%04d-%02d-%02dT%02d:%02d",
        $now[5] + 1900, $now[4] + 1, $now[3], $now[2], $now[1];
}

# This test should be enhanced to deal properly with subfolders, etc.
if ($home eq $OW) {
    print "Error! The build system home folder can not be under $OW\n";
    exit 1;
}

$date_stamp = get_date();
$report_name = "$report_archive\\$date_stamp-report.txt";

open(REPORT, ">$report_name") || die "Unable to open report file.";
print REPORT "Open Watcom Build Report\n";
print REPORT "========================\n\n";

# Build a fresh version of the system from scratch.
if (system("p4 sync") != 0) {
    print REPORT "p4 failed!\n";
    exit 1;
}
$datetime_stamp = get_datetime();
make_batch();
print REPORT "CLEAN+BUILD STARTED: $datetime_stamp\n";
if (system($batch_name) != 0) {
    print REPORT "clean+build failed!\n";
    exit 1;
}
$datetime_stamp = get_datetime();
print REPORT "CLEAN+BUILD COMPLETED: $datetime_stamp\n\n";

# Analyze build result.
system("summary $OW\\bld\\build.log $bldlast");
open(CHANGES, "compare $bldbase $bldlast|");
while (<CHANGES>) {
    print REPORT;
}
close(CHANGES);

#: Run regression tests and analyze result.

#: Finalize report, store in report archive, send to appropriate parties.

close(REPORT);

