BEGIN { FS = "'" }      # Split input lines on apostrophes

/\+\+/ {                # Delete .dll suffix, toss ordinals
    split( $4, mod, /\./ )
    printf( "++'%s'.%s..'%s'\n", $2, mod[1], $2 )
}
