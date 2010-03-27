/* Very quick and dirty D16INFO structure declaration. */

typedef unsigned short  USHORT;
typedef short           SHORT;
typedef unsigned long   ULONG;
typedef unsigned char   UCHAR;

typedef struct {
    SHORT       version;
    USHORT      passdn0;
    USHORT      passdn255;
    void __far  *PassupIntP;
    void __far  *xfer_buff;
    USHORT      xfer_bsize;
    USHORT      delay_286;
    USHORT      delay_386;
    SHORT       switch_control;
    void __far  *D16MStkP;
    void __far  *D16ToRMP;
    void __far  *D16ToPMP;
    void __far  *D16rmInP;
    USHORT      bannerp;
    USHORT      D16rdata;
    char __far  *versionp;
    UCHAR       swmode;
    UCHAR       cpumod;
    USHORT      copyright;
    USHORT      dbginfop;
    UCHAR       D16_intno;
    UCHAR       A20;
    UCHAR       A20_rigor;
    UCHAR       A20_orig;
    USHORT      last_delseg;
    void __far  *tsp_ptr;
    UCHAR       has_87;
    UCHAR       slow_A20;
    USHORT      MSW_bits;
    void __far  *package_info_p;
    UCHAR       machine_type;
    UCHAR       miscellaneous;
    USHORT      xbufadj;
} d16info_t;
