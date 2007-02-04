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

if( $#ARGV == -1 ) {
    Common::filename( "config.txt" );
} elsif( $#ARGV == 0 ) {
    Common::filename( $ARGV[0] );
} else {
    print "Usage: dobuild [config_file]\n";
    exit 1;
}
$home    = $Common::config{"HOME"};
$OW      = $Common::config{"OW"};
$WATCOM  = $Common::config{"WATCOM"};
$report_archive = $Common::config{"REPORTS"};
$bldbase = "$home\\$Common::config{'BLDBASE'}";
$bldlast = "$home\\$Common::config{'BLDLAST'}";

$build_batch_name  = "$home\\buildtmp.bat";
$test_batch_name   = "$home\\testtmp.bat";
$rotate_batch_name = "$home\\rotate.bat";

sub make_build_batch()
{
    open(BATCH, ">$build_batch_name") || die "Unable to open temporary build batch file.";
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

sub make_test_batch()
{
    open(BATCH, ">$test_batch_name") || die "Unable to open temporary test batch file.";
    open(INPUT, "$OW\\setvars.bat") || die "Unable to open setvars.bat";
    while (<INPUT>) {
        if    (/set OWROOT/i) { print BATCH "set OWROOT=" . $OW . "\n"; }
        elsif (/set WATCOM/i) { print BATCH "set WATCOM=" . "$OW\\rel2" . "\n"; }
        else                  { print BATCH; }
    }
    close(INPUT);
    
    # Add additional commands to do the testing.
    print BATCH "\n";
    print BATCH "cd $OW\\bld\\f77\\regress\n";
    print BATCH "del *.log\n";
    print BATCH "cd $OW\\bld\\f77\\regress\\nist\n";
    print BATCH "call testrun\n";
    print BATCH "cd $OW\\bld\\ctest\n";
    print BATCH "del *.log\n";
    print BATCH "cd $OW\\bld\\ctest\\regress\n";
    print BATCH "call testrun\n";
    print BATCH "cd $OW\\bld\\plustest\n";
    print BATCH "del *.log\n";
    print BATCH "cd $OW\\bld\\plustest\\regress\n";
    print BATCH "call testrun\n";
    close(BATCH);
}

sub process_log
{
    my($result)        = "success";
    my($project_name)  = "none";
    my($first_message) = "yes";
    
    open(LOGFILE, $_[0]) || die "Can't open $_[0]";
    while (<LOGFILE>) {
        if (/^=====/) {
            if ($project_name ne "none") {
                if ($first_message eq "yes") {
                    print REPORT "Failed!\n";
                    $first_message = "no";
                }
                print REPORT "\t\t$project_name\n";
                $result = "fail";
            }
            @fields = split;
            $source_location = $OW;
            $source_location =~ s/\\/\\\\/g;
            $fields[2] =~ /$source_location\\(.*)/i; 
            $project_name = $1;
        }
        else {
            if (/^PASS/) {
                $project_name = "none";
            }
        }
    }
    close(LOGFILE);

    # Handle the case where the failed test is the last one.
    if ($project_name ne "none") {
        if ($first_message eq "yes") {
            print REPORT "Failed!\n";
            $first_message = "no";
        }
        print REPORT "\t\t$project_name\n";
        $result = "fail";
    }

    # This is what we want to see.
    if ($result eq "success") {
        print REPORT "Succeeded.\n";
    }
    return $result;
}

sub get_shortdate
{
    my(@now) = gmtime time;
    return sprintf "%04d-%02d", $now[5] + 1900, $now[4] + 1;
}

sub get_date
{
    my(@now) = gmtime time;
    return sprintf "%04d-%02d-%02d", $now[5] + 1900, $now[4] + 1, $now[3];
}

sub get_datetime
{
    my(@now) = gmtime time;
    return sprintf "%04d-%02d-%02d, %02d:%02d UTC",
        $now[5] + 1900, $now[4] + 1, $now[3], $now[2], $now[1];
}

sub display_p4_messages
{
    print REPORT "p4 Messages\n";
    print REPORT "-----------\n\n";

    foreach $message (@p4_messages) {
        print REPORT "$message\n";
    }
}

#######################
#      Main Script
#######################

# This test should be enhanced to deal properly with subfolders, etc.
if ($home eq $OW) {
    print "Error! The build system home folder can not be under $OW\n";
    exit 1;
}

$shortdate_stamp = get_shortdate();
$date_stamp = get_date();
$report_directory = "$report_archive\\$shortdate_stamp";
if (!stat($report_directory)) {
    mkdir($report_directory);
}
$report_name = "$report_directory\\$date_stamp-report.txt";

open(REPORT, ">$report_name") || die "Unable to open report file.";
print REPORT "Open Watcom Build Report\n";
print REPORT "========================\n\n";

# Do a p4 sync to get the latest changes.
#########################################

#force all files update to head
#open(SYNC, "p4 sync -f $OW\...#head |");

open(SYNC, "p4 sync $OW\... |");
while (<SYNC>) {
    @fields = split;
    $source_location = $OW;
    $source_location =~ s/\\/\\\\/g;
    $fields[-1] =~ /$source_location\\(.*)/i;
    if( defined( $1 ) ) {
        $fields[-1] = $1;    
        push(@p4_messages, sprintf("%-8s %s", $fields[2], $fields[-1]));    
    } else {
        push(@p4_messages, sprintf("%s", $_));    
    }
}
if (!close(SYNC)) {
    print REPORT "p4 failed!\n";
    close(REPORT);
    exit 1;
}
print REPORT "'p4 sync' Successful (messages below).\n";
open(LEVEL, "p4 counters|");
while (<LEVEL>) {
    if (/^change = (.*)/) {
        print REPORT "\tBuilding through change: $1\n";
    }
}
close(LEVEL);
print REPORT "\n";

# Build a fresh version of the system from scratch.
####################################################

make_build_batch();
$datetime_stamp = get_datetime();
print REPORT "CLEAN+BUILD STARTED  : $datetime_stamp\n";
if (system($build_batch_name) != 0) {
    print REPORT "clean+build failed!\n";
    display_p4_messages();
    close(REPORT);
    exit 1;
}
$datetime_stamp = get_datetime();
print REPORT "CLEAN+BUILD COMPLETED: $datetime_stamp\n\n";

# Analyze build result.
#######################

system("summary $OW\\bld\\build.log $bldlast");
open(CHANGES, "compare $bldbase $bldlast|");
while (<CHANGES>) {
    print REPORT;
}
# If 'compare' fails, end now. Don't test if there was a build failure.
if (!close(CHANGES)) {
    display_p4_messages();
    close(REPORT);
    exit 1;
}

# Run regression tests for the Fortran, C, and C++ compilers.
##############################################################
make_test_batch();
$datetime_stamp = get_datetime();
print REPORT "REGRESSION TESTS STARTED  : $datetime_stamp\n";
system("$test_batch_name");
$datetime_stamp = get_datetime();
print REPORT "REGRESSION TESTS COMPLETED: $datetime_stamp\n\n";

print REPORT "\tFortran Compiler: ";
$f_compiler = process_log("$OW\\bld\\f77\\regress\\positive.log");
print REPORT "\tC Compiler      : ";
$c_compiler = process_log("$OW\\bld\\ctest\\result.log");
print REPORT "\tC++ Compiler    : ";
$cpp_compiler = process_log("$OW\\bld\\plustest\\result.log");
print REPORT "\n";

# Display p4 sync messages for reference.
##########################################

display_p4_messages();

close(REPORT);

# Rotate the freshly built system into position on the web site.
################################################################
if (($f_compiler   eq "success") &&
    ($c_compiler   eq "success") &&
    ($cpp_compiler eq "success")) {
        
    system("$rotate_batch_name");
}

