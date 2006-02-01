.chap Re-entrant Functions
.*
.np
The following functions in the C library are re-entrant:
.millust begin
abs            atoi         atol            bsearch
bsearch_s      div          fabs            _fmemccpy
_fmemchr       _fmemcmp     _fmemcpy        _fmemicmp
_fmemmove      _fmemset     _fstrcat        _fstrchr
_fstrcmp       _fstrcpy     _fstrcspn       _fstricmp
_fstrlen       _fstrlwr     _fstrncat       _fstrncmp
_fstrncpy      _fstrnicmp   _fstrnset       _fstrpbrk
_fstrrchr      _fstrrev     _fstrset        _fstrspn
_fstrstr       _fstrupr     isalnum         isalpha
isascii        isblank      iscntrl         isdigit
isgraph        islower      isprint         ispunct
isspace        isupper      isxdigit        itoa
labs           ldiv         lfind           longjmp
_lrotl         _lrotr       lsearch         ltoa
_makepath      mblen        mbstowcs        mbtowc
memccpy        memchr       memcmp          memcpy
memcpy_s       memicmp      memmove         memmove_s
memset         movedata     qsort           qsort_s
_rotl          _rotr        segread         setjmp
_splitpath     strcat       strcat_s        strchr
strcmp         strcoll      strcpy          strcpy_s
strcspn        strerror_s   strerrorlen_s   stricmp
strlen         strlwr       strncat         strncat_s
strncmp        strncpy      strncpy_s       strnicmp
strnlen_s      strnset      strpbrk         strrchr
strrev         strset       strspn          strstr
strtok_s       strupr       swab            tolower
toupper        ultoa        utoa            wcscat_s
wcscpy_s       wcsncat_s    wcsncpy_s       wcserror_s
wcserrorlen_s  wcsncat_s    wcsnlen_s       wcstok_s
wcstombs       wctomb       wmemcpy_s       wmemmove_s
.millust end
