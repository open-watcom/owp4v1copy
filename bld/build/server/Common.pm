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
#  Description: Common definitions for the various scripts.
#
###########################################################################

package Common;

sub read_config {
    my($filename) = $_[0];
    my(@fields);

    open(CONFIG_FILE, $filename) || die "Unable to open configuration file.";
    while (<CONFIG_FILE>) {
        chomp;
        s/#.*//;
        if (/^\s*$/) { next; }
        @fields = split /=/;
        $Common::config{$fields[0]} = $fields[1];
    }
    close(CONFIG_FILE);
}

sub check_real_error
{
    if ($_[0] =~ /\*\*\* WARNING: Hyperlink name too long on line [0-9]+./) {
       # ignore anoying warning, which has changed line number when
       # some change is done into documentation
       # it is more likely note than something danger and there is
       # no reason why we should mark the build as unsuccesful
       return 0;
    } else {
       return 1;
    }
}

sub process_summary {

    my($inp_filename) = $_[0];
    my($out_filename) = $_[1];
    my(@header, $current_project, $source_location);

    open(INFILE, $inp_filename) || die "Unable to open input file: $inp_filename";
    open(OUTFILE, ">", $out_filename) || die "Unable to open output file: $out_filename";

    # Read the build log file a line at a time and output the error summary.
    while (<INFILE>) {
        chomp;
        if (/^[=]+ .* [=]+$/) {
            @header = split;
            $current_project = $header[2];
            $source_location = $Common::config{"OW"};
            $source_location =~ s/\\/\\\\/g;
            $current_project =~ /$source_location\\(.*)/i;
            $current_project = $1;
        }
        if (/Warning|Error|Can not|ERROR|WARNING/ and check_real_error($_)) {
            print OUTFILE "\nPROJECT $current_project\n";
            print OUTFILE "$_\n";
        }
    }

    close(OUTFILE);
    close(INFILE);
}

# Read the next two line record from the summary file.
sub read_record
{
    my($file) = $_[0];
    my($record, $line);
    while (<$file>) {
        chomp;
        s/#.*//;
        if (/^\s*$/) { next; }
        $record = $_;
        $line = <$file>;
        chomp($line);
        $record = $record . "|" . $line;
        return $record;
    }
    return "EOF";
}

# Display a combined record as two lines.
sub print_record
{
    my($record) = $_[0];
    my($fh) = $_[1];

    my(@fields) = split /\|/, $record;
    print $fh "$fields[0]\n$fields[1]\n\n";
}

sub process_compare
{
    my($filename1) = $_[0];
    my($filename2) = $_[1];
    my($fh) = $_[2];
    my($record, @old_records, @new_records, $found, $candidate);
    my($something_added, $something_removed, $first_added);
    my($first_removed, $exit_status);

    $fh ||= \*STDOUT;

    # Read both the old and new summaries into memory.
    open(OLDFILE, $filename1) || die "Unable to open input file: $ARGV[0]";
    while (($record = read_record(\*OLDFILE)) ne "EOF") {
        push @old_records, $record;
    }
    close(OLDFILE);

    open(NEWFILE, $filename2) || die "Unable to open output file: $ARGV[1]";
    while (($record = read_record(\*NEWFILE)) ne "EOF") {
        push @new_records, $record;
    }
    close(NEWFILE);

    # Now compare the summaries. This runs in O(n^2) where n is the # of records.
    #############################################################################

    $something_added   = "no";
    $something_removed = "no";
    $first_added       = "yes";
    $first_removed     = "yes";

    $something_added = "no";
    foreach $record (@new_records) {
        $found = "no";
        foreach $candidate (@old_records) {
            if ($record eq $candidate) { $found = "yes"; }
        }
        if (($found eq "no") and check_real_error($record)) {
            if ($first_added eq "yes") {
                print $fh "Messages Added\n";
                print $fh "--------------\n\n";
                $first_added = "no";
            }   
            print_record($record, $fh);
            $something_added = "yes";
        }
    }

    # If there are new errors, don't bother computing removed messages. Some
    # messages might appear to vanish because certain compilations failed to
    # finish. Only trust the removal list if there are no additional errors.
    #
    if ($something_added eq "no") {
        foreach $record (@old_records) {
            $found = "no";
            foreach $candidate (@new_records) {
                if ($record eq $candidate) { $found = "yes"; }
            }
            if (($found eq "no") and check_real_error($record)) {
                if ($first_removed eq "yes") {
                    print $fh "Messages Removed\n";
                    print $fh "----------------\n\n";
                    $first_removed = "no";
                }   
                print_record($record, $fh);
                $something_removed = "yes";
            }
        }
    }

    $exit_status = 1;  # Assume failure.

    # This is what we like to see.
    if ($something_added eq "no" && $something_removed eq "no") {
        print $fh "Build Successful\n\n";
        $exit_status = 0;
    }
    return $exit_status;
}

1;
