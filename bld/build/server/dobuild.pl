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
use strict;

use Common;

my(@p4_messages);
my($OStype);
my($ext);
my($setenv);
my($WATCOM);
my($relsubdir);
my($buildlog);
my($bldbase);
my($bldlast);
my($build_platform);

if( $#ARGV == -1 ) {
    Common::read_config( "config.txt" );
} elsif( $#ARGV == 0 ) {
    Common::read_config( $ARGV[0] );
} else {
    print "Usage: dobuild [config_file]\n";
    exit 1;
}

my $home           = $Common::config{"HOME"};
my $OW             = $Common::config{"OW"};
my $report_archive = $Common::config{"REPORTS"};
my $GHOSTSCRIPT    = $Common::config{"GHOSTSCRIPT"};
my $SNAPSHOTPATH   = $Common::config{"SNAPSHOTPATH"};

if( $^O eq "MSWin32" ) {
    $OStype = "WIN32";
    $ext    = "bat";
    $setenv = "set";
    $build_platform = "win32-x86";
} elsif( $^O eq "linux" ) {
    $OStype = "UNIX";
    $ext    = "sh";
    $setenv = "export";
    $build_platform = "linux-x86";
} elsif( $^O eq "os2" ) {
    $OStype = "OS2";
    $ext    = "cmd";
    $setenv = "set";
    $build_platform = "os2-x86";
} else {
    print "Unsupported or unknown platform '$^O' !\n";
    print "Review dobuild.pl file and fix it for new platform!\n";
    exit 1;
}

my $build_batch_name     = "$home\/buildtmp.$ext";
my $build_installer_name = "$home\/bldinst.$ext";
my $test_batch_name      = "$home\/testtmp.$ext";
my $rotate_batch_name    = "$home\/rotate.$ext";
my $setvars              = "$OW\/setvars.$ext";

sub get_reldir
{
    if ($OStype eq "UNIX") {
        return "$OW\/$relsubdir";
    } else {
        return "$OW\\$relsubdir";
    }
}

sub make_build_batch
{
    my($pass1) = ($WATCOM eq $Common::config{"WATCOM"});

    open(BATCH, ">$build_batch_name") || die "Unable to open $build_batch_name file.";
    open(INPUT, "$setvars") || die "Unable to open $setvars file.";
    while (<INPUT>) {
        s/\r?\n/\n/;
        if    (/$setenv OWROOT/i) { print BATCH "$setenv OWROOT=", $OW, "\n"; }
        elsif (/$setenv WATCOM/i) { print BATCH "$setenv WATCOM=", $WATCOM, "\n"; }
        elsif (/$setenv DOC_BUILD/i) {
            if ($pass1)           { print BATCH "$setenv DOC_BUILD=1\n"; }
            else                  { print BATCH "$setenv DOC_BUILD=0\n"; }
        } elsif (/$setenv GHOSTSCRIPT/i) {
            if ($pass1)           { print BATCH "$setenv GHOSTSCRIPT=", $GHOSTSCRIPT, "\n"; }
            else                  { print BATCH; }
        } else                    { print BATCH; }
    }
    close(INPUT);
    # Add additional commands to do the build.
    print BATCH "\n";
    # Create fresh builder tools, to prevent lockup build server 
    # if builder tools from previous build are somehow broken
    print BATCH "cd $OW\ncd bld\n";
    if( $^O eq "MSWin32" ) {
        print BATCH "cd builder\ncd nt386\n";
    } elsif( $^O eq "linux" ) {
        print BATCH "cd builder\ncd linux386\n";
    } elsif( $^O eq "os2" ) {
        print BATCH "cd builder\ncd os2386\n";
    }
    print BATCH "wmake -h clean\n";
    print BATCH "wmake -h\n";
    # Remove release directory.
    print BATCH "$setenv RELROOT=", get_reldir(), "\n";
    print BATCH "rm -rf ", get_reldir(), "\n";
    print BATCH "\n";
    # Clean previous build.
    print BATCH "cd $OW\ncd bld\n";
    print BATCH "builder -i clean\n";
    # Create new builder tools, previous clean removed them.
    print BATCH "cd $OW\ncd bld\n";
    if( $^O eq "MSWin32" ) {
        print BATCH "cd builder\ncd nt386\n";
    } elsif( $^O eq "linux" ) {
        print BATCH "cd builder\ncd linux386\n";
    } elsif( $^O eq "os2" ) {
        print BATCH "cd builder\ncd os2386\n";
    }
    print BATCH "wmake -h\n";
    # Recreate Watcom DOS TCP/IP library.
    print BATCH "cd $OW\ncd contrib\ncd wattcp\ncd src\n";
    print BATCH "wmake -h clean\n";
    print BATCH "wmake -h -ms\n";
    # Start build process.
    print BATCH "cd $OW\ncd bld\n";
    if ($pass1) {
        print BATCH "builder -i pass1\n";
    } else {
        print BATCH "builder -i pass2\n";
    }
    close(BATCH);
    # On Windows it has no efect
    chmod 0777, $build_batch_name;
}

sub make_test_batch
{
    open(BATCH, ">$test_batch_name") || die "Unable to open $test_batch_name file.";
    open(INPUT, "$setvars") || die "Unable to open $setvars file.";
    while (<INPUT>) {
        s/\r?\n/\n/;
        if    (/$setenv OWROOT/i) { print BATCH "$setenv OWROOT=", $OW, "\n"; }
        elsif (/$setenv WATCOM/i) { print BATCH "$setenv WATCOM=", get_reldir(), "\n"; }
        else                      { print BATCH; }
    }
    close(INPUT);

    # Add additional commands to do the testing.
    print BATCH "\n";
    if( $^O eq "MSWin32" ) {
        print BATCH "$setenv EXTRA_ARCH=i86\n\n";
    }
    print BATCH "cd $OW\ncd bld\ncd ctest\n";
    print BATCH "rm *.log\n";
    print BATCH "wmake -h targ_env_386=cw386\n";
    print BATCH "cd $OW\ncd bld\ncd wasm\ncd test\n";
    print BATCH "rm *.log\n";
    print BATCH "wmake -h targ_env_386=cw386\n";
    print BATCH "cd $OW\ncd bld\ncd f77\ncd regress\n";
    print BATCH "rm *.log\n";
    print BATCH "wmake -h targ_env_386=cw386\n";
    print BATCH "cd $OW\ncd bld\ncd plustest\n";
    print BATCH "rm *.log\n";
    print BATCH "wmake -h targ_env_386=cw386\n";
    close(BATCH);
    # On Windows it has no efect
    chmod 0777, $test_batch_name;
}

sub make_installer_batch
{
    open(BATCH, ">$build_installer_name") || die "Unable to open $build_installer_name file.";
    open(INPUT, "$setvars") || die "Unable to open $setvars file.";
    while (<INPUT>) {
        s/\r?\n/\n/;
        if    (/$setenv OWROOT/i) { print BATCH "$setenv OWROOT=", $OW, "\n"; }
        elsif (/$setenv WATCOM/i) { print BATCH "$setenv WATCOM=", $Common::config{"WATCOM"}, "\n"; }
        else                      { print BATCH; }
    }
    close(INPUT);
    # Add additional commands to do installers.
    print BATCH "\n";
    print BATCH "$setenv RELROOT=", $SNAPSHOTPATH, "\n";
    if ($OStype eq "UNIX") {
        # set up max open file handle to be enough for uzip
        print BATCH "ulimit -n 4096\n";
    }
    print BATCH "\n";
    print BATCH "cd $OW\ncd distrib\ncd ow\n";
    print BATCH "builder missing\n";
    print BATCH "builder rel2 os_dos\n";
    print BATCH "builder rel2 os_nt\n";
    print BATCH "builder rel2 os_os2\n";
    print BATCH "builder rel2 os_linux\n";
    close(BATCH);
    # On Windows it has no efect
    chmod 0777, $build_installer_name;
}

sub process_log
{
    my($result)        = "success";
    my($project_name)  = "none";
    my($first_message) = "yes";
    my($arch_test)     = "";
    my(@fields);

    open(LOGFILE, $_[0]) || die "Can't open $_[0]";
    while (<LOGFILE>) {
        s/\r?\n/\n/;
        if (/^[=]+ .* [=]+$/) {
            if ($project_name ne "none") {
                if ($first_message eq "yes") {
                    print REPORT "Failed!\n";
                    $first_message = "no";
                }
                if ($arch_test ne "") {
                    print REPORT "\t\t$project_name\t$arch_test\n";
                }
                $result = "fail";
            }
            @fields = split;
            $project_name = Common::remove_OWloc($fields[2]);
            $arch_test = "";
        } elsif (/^TEST/) {
            @fields = split;
            $arch_test = $fields[1];
        } elsif (/^PASS/) {
            $project_name = "none";
        }
    }
    close(LOGFILE);

    # Handle the case where the failed test is the last one.
    if ($project_name ne "none") {
        if ($first_message eq "yes") {
            print REPORT "Failed!\n";
            $first_message = "no";
        }
        if ($arch_test ne "") {
            print REPORT "\t\t$project_name\t$arch_test\n";
        }
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
    my($message);

    print REPORT "p4 Messages\n";
    print REPORT "-----------\n\n";

    foreach $message (@p4_messages) {
        print REPORT "$message\n";
    }
}

sub run_tests
{
    my($result) = "success";

    # Run regression tests for the Fortran, C, C++ compilers and WASM.

    make_test_batch();
    print REPORT "REGRESSION TESTS STARTED  : ", get_datetime(), "\n";
    system("$test_batch_name");
    print REPORT "REGRESSION TESTS COMPLETED: ", get_datetime(), "\n\n";

    print REPORT "\tFortran Compiler: ";
    if (process_log("$OW\/bld\/f77\/regress\/result.log") ne "success") { $result = "fail"; }
    print REPORT "\tC Compiler      : ";
    if (process_log("$OW\/bld\/ctest\/result.log") ne "success") { $result = "fail"; }
    print REPORT "\tC++ Compiler    : ";
    if (process_log("$OW\/bld\/plustest\/result.log") ne "success") { $result = "fail"; }
    print REPORT "\tWASM            : ";
    if (process_log("$OW\/bld\/wasm\/test\/result.log") ne "success") { $result = "fail"; }
    print REPORT "\n";

    return $result;
}

sub run_build
{
    make_build_batch();
    print REPORT "CLEAN+BUILD STARTED  : ", get_datetime(), "\n";
    if (system($build_batch_name) != 0) {
        print REPORT "clean+build failed!\n";
        return "fail";
    } else {
        print REPORT "CLEAN+BUILD COMPLETED: ", get_datetime(), "\n\n";

        # Analyze build result.

        Common::process_summary($buildlog, $bldlast);
        # If 'compare' fails, end now. Don't test if there was a build failure.
        if (Common::process_compare($bldbase, $bldlast, \*REPORT)) {
            return "fail";
        } else {

            # Run regression tests

            $WATCOM = get_reldir();
            return run_tests();
        }
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

my $shortdate_stamp = get_shortdate();
my $date_stamp = get_date();
my $report_directory = "$report_archive\/$shortdate_stamp";
if (!stat($report_directory)) {
    mkdir($report_directory);
}
my $report_name = "$report_directory\/$date_stamp-report-$build_platform.txt";

open(REPORT, ">$report_name") || die "Unable to open $report_name file.";
print REPORT "Open Watcom Build Report (", $build_platform, ")\n";
print REPORT "=====================================\n\n";

# Do a p4 sync to get the latest changes.
#########################################

#force all files update to head
#open(SYNC, "p4 sync -f $OW\/...#head |");

open(SYNC, "p4 sync $OW\/... |");
while (<SYNC>) {
    my @fields = split;
    my $loc = Common::remove_OWloc($fields[-1]);
    if( $loc ne "" ) {
        push(@p4_messages, sprintf("%-8s %s", $fields[2], $loc));
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

############################################################
#
#  pass 1  Build and test full Open Watcom
#
############################################################

$WATCOM    = $Common::config{"WATCOM"};
$relsubdir = "pass1";
$buildlog  = "$OW\/bld\/pass1.log";
$bldbase   = "$home\/$Common::config{'BLDBASE'}";
$bldlast   = "$home\/$Common::config{'BLDLAST'}";

my $pass1_result = run_build();

############################################################
#
#  pass 2  Build and test Compilers and Tools only
#          it uses OW pass1 version
#
############################################################

print REPORT "\n";
print REPORT "Compilers and Tools (pass 2)\n";
print REPORT "============================\n\n";

$WATCOM    = get_reldir();
$relsubdir = "pass2";
$buildlog  = "$OW\/bld\/pass2.log";
$bldbase   = "$home\/$Common::config{'BLDBASE2'}";
$bldlast   = "$home\/$Common::config{'BLDLAST2'}";

my $pass2_result = run_build();

# Display p4 sync messages for reference.
##########################################

display_p4_messages();

close(REPORT);

# Rotate the freshly built system into position on the web site.
################################################################
if (($pass1_result eq "success") &&
    ($pass2_result eq "success")) {

    system("$rotate_batch_name");
    if (defined $SNAPSHOTPATH) {
        make_installer_batch();
        system("$build_installer_name");
    }
}
