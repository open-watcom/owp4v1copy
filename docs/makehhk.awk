BEGIN {
    ORS = "\r\n";
    print "<HTML>";
    print "<BODY>";
    print "<UL>";
}

function transsym( str ) {
    targ = str;
    gsub( / /, "_", targ );
    gsub( /-/, "M", targ );
    gsub( /\//, "D", targ );
    gsub( /\+/, "P", targ );
    gsub( /\[/, "U", targ );
    gsub( /\]/, "V", targ );
    gsub( /</, "X", targ );
    gsub( />/, "Y", targ );
    gsub( /\=/, "E", targ );
    gsub( /{/, "_", targ );
    gsub( /}/, "_", targ );
    gsub( /,/, "_", targ );
    gsub( /\#/, "_", targ );
    gsub( /\?/, "_", targ );
    gsub( /\./, "_", targ );
    gsub( /\|/, "_", targ );
    gsub( /\"/, "_", targ );
    gsub( /:/, "_", targ );
    gsub( /\r/, "", targ );
    return targ;
}

/\.helppref / {
    prefix = $0
    gsub( /\.helppref /, "", prefix );
    if( length( prefix ) )
       prefix = transsym( prefix ) "_";
}

/\.ixchap / {
    file = $0;
    gsub( /\.ixchap /, "", file );
    file = transsym( file );
}

/\.ixsect / {
    file = $0;
    gsub( /\.ixsect /, "", file );
    file = transsym( file );
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
    print "<PARAM NAME=\"Local\" VALUE=\"" prefix file ".htm\">";
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
    print "<PARAM NAME=\"Local\" VALUE=\"" prefix file ".htm\">";
    print "</OBJECT>";
}

END {
    print "</UL>";
    print "</BODY>";
    print "</HTML>";
}
