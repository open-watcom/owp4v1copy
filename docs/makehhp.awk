BEGIN {
    print "[OPTIONS]";
    print "Compatibility=1.1 or later";
    if( length( hhcfile ) > 0 ) {
        print "Contents file=" hhcfile;
    }
    print "Create CHI file=Yes";
    print "Default Window=Main";
    print "Language=0x409 English (United States)";
}

/<TITLE> (.*) <\/TITLE>/ {
    title = $0;
    gsub( /<TITLE> /, "", title );
    gsub( / <\/TITLE>/, "", title );
    print "Title=" title;
    print "";
    print "[WINDOWS]";
    if( length( hhcfile ) > 0 ) {
        print "Main=\"" title "\",\"" hhcfile "\",,,,,,,,0x2020,,0x300e,,,,,,,,0";
    } else {
        print "Main=\"" title "\",,,,,,,,,0x2020,,0x300e,,,,,,,,0";
    }
    print "";
    print "[FILES]";
}

/<H. ID=\"(.*)\">/ {
    file = $0;
    gsub( /<H. ID=\"/, "", file );
    gsub( /\"> (.*) <\/H.>/, "", file );
    if( file == "Index_of_Topics" ) {
        next;
    }
    print file ".htm";
}
