BEGIN { FS = "'" }     # Split input lines on apostrophes

$1 ~ /\+\+/ \
&& $2 ~ /(^_|^)(\
|__iob|\
|__IsDBCS|\
|__MBCSIsTable|\
|_amblksiz|\
|_fileinfo|\
|_fmode|\
|_HugeValue|\
|_IsKTable|\
|_IsTable|\
|_osbuild|\
|_osmajor|\
|_osminor|\
|_pgmptr|\
|_osver|\
|_sys_errlist|\
|_sys_nerr|\
|_wenviron|\
|_winmajor|\
|_winminor|\
|_winver|\
|_wpgmptr|\
|daylight|\
|environ|\
|timezone|\
|tzname|\
)/ {
  if( br ) {
    if( os == "os2" ) {
      split( $4, mod, /\./ )
      $4 = mod[1]
    }
    printf( "++'%s_br'.'%s'..'%s'\n", $2, $4, $2 )
    next
  }
}

$1 ~ /\+\+/ {
  if( os == "os2" ) {
    split( $4, mod, /\./ )
    $4 = mod[1]
  }
  printf( "++'%s'.'%s'\n", $2, $4 )
}
