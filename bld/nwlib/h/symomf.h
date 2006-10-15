#ifndef _SYMOMF_H_CB7361EB_B731_457A_8FA8_C09DE19A9216
#define _SYMOMF_H_CB7361EB_B731_457A_8FA8_C09DE19A9216

extern void OMFSkipThisObject(arch_header *arch, libfile io);
extern void OMFLibWalk( libfile io, char *name, void (*rtn)( arch_header *arch, libfile io ) );

#endif /* _SYMOMF_H_CB7361EB_B731_457A_8FA8_C09DE19A9216 */
