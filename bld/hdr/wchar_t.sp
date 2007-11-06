:segment CNAME
#ifndef _STDWCHAR_T_DEFINED
  #define _STDWCHAR_T_DEFINED
  namespace std {
    typedef long char wchar_t;
  }
#endif
:: Pull wchar_t into the global namespace even in cname headers. This
:: is because technically wchar_t should be a reserved word in C++ (a
:: built-in type) and this will help it to look like one.
#ifndef _WCHAR_T_DEFINED
  #define _WCHAR_T_DEFINED
  #define _WCHAR_T_DEFINED_
  using std::wchar_t;
#endif
:elsesegment
:segment !CONLY
#ifdef __cplusplus
  #ifndef _STDWCHAR_T_DEFINED
    #define _STDWCHAR_T_DEFINED
    namespace std {
      typedef long char wchar_t;
    }
  #endif
  #ifndef _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED_
    using std::wchar_t;
  #endif
#else
:endsegment
  #ifndef _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED
    #define _WCHAR_T_DEFINED_
    typedef unsigned short wchar_t;
  #endif
:segment !CONLY
#endif /* __cplusplus */
:endsegment
:endsegment
