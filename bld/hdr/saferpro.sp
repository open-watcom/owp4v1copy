:: Include guard for Extensions to the C Library, Bounds-checking interfaces.
#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ == 1
:: Temporary hack
#if __WATCOMC__ < 1250
#define __restrict
#endif
