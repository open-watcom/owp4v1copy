:: the C++ compiler has knowledge of this type so if it changes from 'unsigned'
:: inform the C++ compiler team and tell them to check their op new diagnostics
:segment CNAME
#ifndef _STDSIZE_T_DEFINED
#define _STDSIZE_T_DEFINED
namespace std {
typedef unsigned size_t;
}
:: The type _w_size_t is an alias for std::size_t or size_t as
:: appropriate. It is intended to simplify the standard headers.
typedef std::size_t _w_size_t;
#endif
:elsesegment
#ifdef __cplusplus
#ifndef _STDSIZE_T_DEFINED
#define _STDSIZE_T_DEFINED
namespace std {
typedef unsigned size_t;
}
typedef std::size_t _w_size_t;
#endif
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
using std::size_t;
#endif
#else /* __cplusplus not defined */
#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
#define _SIZE_T_DEFINED_
typedef unsigned size_t;
typedef size_t   _w_size_t;
#endif
#endif /* __cplusplus */
:endsegment
