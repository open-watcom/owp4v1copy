BEGIN {
    ORS = "\r\n";
    print "<HTML>";
    print "<BODY>";
    print "<UL>";
}

function transsym( str ) {
    gsub( / /, "_", file );
    gsub( /-/, "M", file );
    gsub( /\//, "D", file );
    gsub( /\+/, "P", file );
    gsub( /\[/, "U", file );
    gsub( /\]/, "V", file );
    gsub( /</, "X", file );
    gsub( />/, "Y", file );
    gsub( /\=/, "E", file );
    gsub( /{/, "_", file );
    gsub( /}/, "_", file );
    gsub( /,/, "_", file );
    gsub( /\#/, "_", file );
    gsub( /\?/, "_", file );
    gsub( /\./, "_", file );
    gsub( /\r/, "", file );
}

/\.ixchap / {
    file = $0;
    gsub( /\.ixchap /, "", file );
    transsym( file );
}

/\.ixsect / {
    file = $0;
    gsub( /\.ixsect /, "", file );
    transsym( file );
}

/\.ixline '(.*)' '(.*)'/ {
    name1 = $0;
    gsub( /.ixline '/, "", name1 );
    gsub( /' '(.*)'/, "", name1 );
    gsub( /\r/, "", name1 );
    name2 = $0;
    gsub( /.ixline '(.*)' '/, "", name2 );
    gsub( /'/, "", name2 );
    gsub( /\r/, "", name2 );
    print "<LI><OBJECT TYPE=\"text/sitemap\">";
    print "<PARAM NAME=\"Name\" VALUE=\"" name1 "\">";
    print "</OBJECT>";
    print "<UL>";
    print "<LI><OBJECT TYPE=\"text/sitemap\">";
    print "<PARAM NAME=\"Name\" VALUE=\"" name2 "\">";
    print "<PARAM NAME=\"Local\" VALUE=\"" file ".htm\">";
    print "</OBJECT>";
    print "</UL>";
    next;
}

/\.ixline '(.*)'/ {
    name = $0;
    gsub( /.ixline /, "", name );
    gsub( /'/, "", name );
    gsub( /\r/, "", name );
    print "<LI><OBJECT TYPE=\"text/sitemap\">";
    print "<PARAM NAME=\"Name\" VALUE=\"" name "\">";
    print "<PARAM NAME=\"Local\" VALUE=\"" file ".htm\">";
    print "</OBJECT>";
}

END {
    print "</UL>";
    print "</BODY>";
    print "</HTML>";
}
