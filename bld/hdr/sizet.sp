:: the C++ compiler has knowledge of this type so if it changes from 'unsigned'
:: inform the C++ compiler team and tell them to check their op new diagnostics
:segment CNAME
#ifndef _STDSIZE_T_DEFINED
#define _STDSIZE_T_DEFINED
namespace std {
typedef unsigned size_t;
}
#endif
:elsesegment
#ifdef __cplusplus
#ifndef _STDSIZE_T_DEFINED
#define _STDSIZE_T_DEFINED
namespace std {
typedef unsigned size_t;
}
#endif
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
using std::size_t;
#endif
#else
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
typedef unsigned size_t;
#endif
#endif
:endsegment
